$(document).ready(function() {
  rack_units.push({'constructor': Poker, 'aspect': [16,13],
    'page': 'session'});
});

String.prototype.splice = function( idx, rem, s ) {
  return (this.slice(0,idx) + s + this.slice(idx + Math.abs(rem)));
};

function Poker(info_object) {
  this.title = "Poker";
  this.description = "Debug api commands.";
  this.id = "poker";
  this.div = "<div class='poker RCU' id='" + this.id + "'></div>";
  info_object['parent_div'].append(this.div);
  this.sel = "#" + this.id;
  this.x = 0;
  this.y = 1;
  this.resizeTimer;
  $(this.sel).append('\
   <div class="vbox robo_light offwhite darkblue">\
     <div class="hbox">\
       <div class="pk_recv">\
         <div class="big robo_bold">IN</div>\
         <div class="kr_button_group">\
           <div><div class="kr_push_button red" id="recv_log_button"><em>Log</em></div></div>\
           <div><div class="kr_push_button blue" id="recv_clear_button"><em>Clear</em></div></div>\
         </div>\
       </div>\
       <div class="pk_recv_log" id="pk_recv_log"></div>\
     </div>\
     <div class="hbox">\
       <div class="pk_sent">\
         <div class="big robo_bold">OUT</div>\
         <div class="kr_button_group">\
           <div><div class="kr_push_button red" id="sent_log_button"><em>Log</em></div></div>\
           <div><div class="kr_push_button blue" id="sent_clear_button"><em>Clear</em></div></div>\
         </div>\
       </div>\
       <div class="pk_sent_log" id="pk_sent_log"></div>\
     </div>\
     <textarea class="pk_send_text" id="pk_send_text"></textarea>\
     <div class="kr_button_group"><div class="kr_push_button red" id="send_text_button">\
       <em>Send</em></div></div>\
   </div>\
   ');
  this.send_textarea = document.getElementById("pk_send_text");
  this.send_button = document.getElementById("send_text_button");
  this.send_button.addEventListener('click', create_handler(this,
   this.on_send_click));
  this.recv_logging = false;
  this.recv_log = document.getElementById("pk_recv_log");
  this.recv_log_button = document.getElementById("recv_log_button");
  this.recv_log_button.addEventListener('click', create_handler(this,
   this.on_recv_log_click));
  this.clear_recv_log_button = document.getElementById("recv_clear_button");
  this.clear_recv_log_button.addEventListener('click', create_handler(this,
   this.on_clear_recv_log_click));
  this.sent_logging = false;
  this.sent_log = document.getElementById("pk_sent_log");
  this.sent_log_button = document.getElementById("sent_log_button");
  this.sent_log_button.addEventListener('click', create_handler(this,
   this.on_sent_log_click));
  this.clear_sent_log_button = document.getElementById("sent_clear_button");
  this.clear_sent_log_button.addEventListener('click', create_handler(this,
   this.on_clear_sent_log_click));
  this.recv_recorder = new ApiRecvResponder("poker_recv_listener", create_handler(
  this, this.on_recv));
  this.sent_recorder = new ApiSendResponder("poker_send_listener", create_handler(
   this, this.on_sent));
}

Poker.prototype.on_send_click = function(evt) {
  var text = this.send_textarea.value;
  kr.send_json(text);
}

Poker.prototype.update =  function(crate) {
}

Poker.prototype.on_recv_log_click = function(evt) {
  if (!this.recv_logging) {
    this.start_recv_log();
  } else {
    this.stop_recv_log();
  }
}

Poker.prototype.stop_recv_log = function() {
  this.recv_logging = false;
  this.recv_log_button.classList.remove("pushed");
}

Poker.prototype.start_recv_log = function() {
  this.recv_logging = true;
  this.recv_log_button.classList.add("pushed");
}

Poker.prototype.on_clear_recv_log_click = function(evt) {
  this.clear_recv_log();
}

Poker.prototype.clear_recv_log = function() {
  this.recv_log.innerHTML = "";
}

Poker.prototype.on_sent_log_click = function(evt) {
  if (!this.sent_logging) {
    this.start_sent_log();
  } else {
    this.stop_sent_log();
  }
}

Poker.prototype.stop_sent_log = function() {
  this.sent_logging = false;
  this.sent_log_button.classList.remove("pushed");
}

Poker.prototype.wrap_lines = function(text) {
  var len = 79;
  var start = 0;
  var end = len;
  var new_text = "";
  while (end < text.length) {
    new_text += text.slice(start,end) + "\n";
    start = end;
    end += len;
  }
  new_text += text.slice(start, text.length);
  return new_text;
}

Poker.prototype.log_append = function(log, content) {
  var doscroll = 0;
  if (log.clientHeight == log.scrollHeight - log.scrollTop) doscroll = 1;
  var pre = document.createElement("pre");
  pre.textContent = content;
  log.appendChild(pre);
  if (doscroll == 1) log.scrollTop = log.scrollHeight;
}

Poker.prototype.on_sent = function(json) {
  if (this.sent_logging) {
    var out = this.wrap_lines(json);
    this.log_append(this.sent_log, out);
  }
}

Poker.prototype.on_recv = function(json) {
  if (this.recv_logging) {
    var out = this.wrap_lines(json);
    this.log_append(this.recv_log, out);
  }
}

Poker.prototype.start_sent_log = function() {
  this.sent_logging = true;
  this.sent_log_button.classList.add("pushed");
}

Poker.prototype.on_clear_sent_log_click = function(evt) {
  this.clear_sent_log();
}

Poker.prototype.clear_sent_log = function() {
  this.sent_log.innerHTML = "";
}

function ApiRecvResponder(name, func) {
  this.name = name;
  this.event_cb = func;
  kr.add_listener_recv(this);
}

ApiRecvResponder.prototype.finish = function() {
  kr.remove_listener_recv(this);
}

ApiRecvResponder.prototype.event = function(json) {
  this.event_cb(json);
}

function ApiSendResponder(name, func) {
  this.name = name;
  this.event_cb = func;
  kr.add_listener_send(this);
}

ApiSendResponder.prototype.finish = function() {
  kr.remove_listener_send(this);
}

ApiSendResponder.prototype.event = function(json) {
  this.event_cb(json);
}

