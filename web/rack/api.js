function create_handler(inst, func, arg) {
  return (function(e){
    func.call(inst, e, arg);
  });
}

window.requestAnimFrame = (function(){
  return window.requestAnimationFrame
   || window.webkitRequestAnimationFrame
   || window.mozRequestAnimationFrame
   || window.oRequestAnimationFrame
   || window.msRequestAnimationFrame
   || function(callback){
      window.setTimeout(callback, 1000 / 60);
    };
})();

Kr.prototype.ws_do_reconnect = function() {
  return;
  if (this.ws_connected != true) {
    this.ws_reconnection_attempts += 1;
    if (this.ws_reconnection_attempts == 3) {
      $('#kradradio').append("<div id='websockets_connection_problem'>"
       + "<h2>Websockets connection problem using port " + window.location.port
       + "</h2></div>");
    }
    this.ws_connect();
  }
}

Kr.prototype.ws_connect = function() {
  this.ws_stay_connected = true;
  if (this.ws_connected != true) {
    if (this.ws_connecting != true) {
      this.ws_connecting = true;
      this.debug("Connecting..");
      this.ws = new WebSocket(this.ws_uri, "krad");
      this.ws.onopen = create_handler(this, this.ws_on_open);
      this.ws.onclose = create_handler(this, this.ws_on_close);
      this.ws.onmessage = create_handler(this, this.ws_on_message);
      this.ws.onerror = create_handler(this, this.ws_on_error);
    } else {
      this.debug("Tried to connect but in the process of connecting.");
    }
  } else {
    this.debug("Tried to connect when already connected.");
  }
}

Kr.prototype.ws_disconnect = function() {
  this.ws_connected = false;
  this.ws_connecting = false;
  this.ws_stay_connected = false;
  this.debug("Disconnecting..");
  if (this.ws_reconnect != false) {
    window.clearInterval(this.ws_reconnect);
    this.ws_reconnect = false;
  }
  this.ws.close();
}

Kr.prototype.ws_on_open = function(evt) {
  this.ws_connected = true;
  this.debug("Connected!");
  this.ws_connecting = false;
  this.ws_reconnection_attempts = 0;

  if (this.ws_reconnect != false) {
    window.clearInterval(this.ws_reconnect);
    this.ws_reconnect = false;
  }
  if ($('#websockets_connection_problem')) {
    $('#websockets_connection_problem').remove();
  }
  this.load_interface();
  this.ux.got_sysname('bongococonutbanana');
  this.list_all();
}

Kr.prototype.ws_on_close = function(evt) {
  this.ws_connected = false;
  this.debug("Disconnected!");
  this.unload_interface();
  this.ws_connecting = false;
  if (this.ws_stay_connected == true) {
    if (this.ws_reconnect == false) {
      this.ws_reconnect = setInterval(
       create_handler(this, this.ws_do_reconnect), 1000);
    }
  }
}

Kr.prototype.ws_on_error = function(evt) {
  this.debug("Error! " + evt.data);
}

Kr.prototype.debug = function(message) {
  console.log(message);
}

Kr.prototype.ws_on_message = function(evt) {
  this.listen(this.listeners_recv, evt.data);
  if (evt.data != "") {
    var text = evt.data;
    try {
      var msg = JSON.parse(text);
      this.ux.on_crate(msg);
    } catch(e) {
      kr.debug(e);
    }
  }
}

function Crate(meth, path, data) {
  this.meth = meth;
  this.path = path;
  this.meta = {};
  this.data = data;
}

Kr.prototype.get = function(path) {
  var cmd = '{"meth":"GET","path":"' + path + '"}';
  kr.send_json(cmd);
}

Kr.prototype.rm = function(path, sender) {
  var cmd = '{"meth":"DELETE","path":"' + path + '"}';
  kr.send_json(cmd, sender);
}

Kr.prototype.crate_path_root_match = function(crate, string) {
  return crate.path.indexOf(string) === 0;
}

Kr.prototype.crate_path_match = function(crate, string) {
  return crate.path.toLowerCase().indexOf(string.toLowerCase()) !== -1;
}

Kr.prototype.list_all = function() {
  var components = ["/info", "/xpdr", "/mixer", "/compositor", "/studio",
   "/studio/tracker"];
  components.forEach(function(entry) {
    kr.get(entry);
  });
}

Kr.prototype.ctrl_mix = function(patchset, path, sender) {
  var prefix = '{"meth":"PATCH","path":"' + path + '","meta":{},"data":';
  var suffix = '}';
  var patches = [];
  for (var i = 0; i < patchset.length; i++) {
    var patch_path = patchset[i][0];
    var type = patchset[i][1];
    var val = patchset[i][2];
    val = type == 'INT' ? val.toFixed(0) : val.toFixed(2);
    var cmd = {};
      cmd = '{\
       "op": "replace",\
       "path": "' + patch_path + '",\
       "value": ' + val + '\
       }';
      patches[patches.length] = cmd;
  }
  if (patches.length > 0) {
    var cmds_str = prefix + '[' + patches.join(',\n') + ']\n' + suffix;
    this.send_json(cmds_str, sender);
  } else {
    kr.debug('kr.ctrl called without valid patchset');
  }
}

Kr.prototype.send_json = function(json, sender) {
  this.ws.send(json);
  this.listen(this.listeners_send, json);
  var crate = {};
  try {
    crate = JSON.parse(json);
  } catch (e) {
    this.debug(e.message);
    return;
  }
  if (crate.meth != "GET") {
    this.ux.on_crate(crate, sender);
  }
}

Kr.prototype.add_listener_send = function(listener) {
  this.add_listener(this.listeners_send, listener);
}

Kr.prototype.add_listener_recv = function(listener) {
  this.add_listener(this.listeners_recv, listener);
}

Kr.prototype.add_listener = function(list, listener) {
  list.push(listener);
}

Kr.prototype.remove_listener_send = function(listener) {
  this.remove_listener(this.listeners_send, listener);
}

Kr.prototype.remove_listener_send = function(listener) {
  this.remove_listener(this.listeners_recv, listener);
}

Kr.prototype.remove_listener = function(list, listener) {
  var idx = list.indexOf(listener);
  if (idx > -1) {
    list.splice(idx, 1);
  }
}

Kr.prototype.listen = function(list, json) {
  for (var i = 0, len = list.length; i < len; i++) {
    list[i].event(json);
  }
}

function Kr() {
  this.resize_timeout = 10;
  this.ux = {};
  this.listeners_send = [];
  this.listeners_recv = [];
  this.ws_uri = (window.location.protocol === "https:") ? "wss://" : "ws://";
  this.ws_uri += location.hostname + ':' + window.location.port + '/';
  this.ws = "";
  this.ws_reconnection_attempts = 0;
  this.ws_reconnect = false;
  this.ws_connected = false;
  this.ws_connecting = false;
  this.ws_stay_connected = true;
  this.debug("Krad Radio client created");
  this.ws_connect();
}

Kr.prototype.unload_interface = function() {
	if (this.ux) {
    this.ux.destroy();
	  this.ux = false;
  }
}
