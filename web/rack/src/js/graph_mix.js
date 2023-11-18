$(document).ready(function() {
  rack_units.push({'constructor': GrapherMix, 'aspect': [32,14],
    'page': 'agraph', 'id': 'mixer', 'position': [0, 1] });
});


function GrapherMix(info_object, id) {
  Grapher.call(this, info_object, id);
}

GrapherMix.prototype = Object.create(Grapher.prototype);

GrapherMix.prototype.constructor = GrapherMix;

GrapherMix.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/mixer/")) return;
  if (crate.meth == "PUT") {
    this.on_put(crate);
  } else if (crate.meth == "DELETE") {
    var path = crate.path;
    var names = path.split("/");
    if (names.length == 4) {
      this.on_del_input(path);
    } else {
      this.on_del_path(path);
    }
  }
}

GrapherMix.prototype.on_put = function(crate) {
  var path = crate.path;
  if (crate.data.type == "input") {
    this.new_input(path, crate);
  }
  if (crate.data.type == "output") {
    this.new_output(path);
  }
  if (crate.data.type == "source") {
    this.new_source(path);
  }
  if (crate.data.type == "bus") {
    this.new_bus(path);
  }
}

GrapherMix.prototype.on_del_input = function(path) {
  var names = path.split('/');
  var from_name = names.pop();
  var to_name = names.pop();
  this.remove_input_by_names(from_name, to_name);
  this.redraw();
}

GrapherMix.prototype.create_bus = function(name) {
  var r = {};
  var bus = {};
  r.meth = "PUT";
  r.path = "/mixer/" + name;
  r.meta = {};
  bus.type = "bus";
  bus.channels = "stereo";
  bus.afx = [];
  bus.afx[0] = {};
  bus.afx[0].pass = {};
  bus.afx[0].pass.low = {};
  bus.afx[0].pass.low.bw = 0.66;
  bus.afx[0].pass.low.hz = 20000.0;
  bus.afx[0].pass.high = {};
  bus.afx[0].pass.high.bw = 0.66;
  bus.afx[0].pass.high.hz = 20.0;
  bus.afx[0].analog = {};
  bus.afx[0].analog.drive = 0;
  bus.afx[0].analog.blend = 0;
  bus.afx[0].eq = {};
  bus.afx[0].eq.band = [];
  bus.afx[0].volume = {};
  bus.afx[0].volume.gain = 0;
  bus.afx[0].volume.fader = 0;
  bus.afx[1] = bus.afx[0];
  r.data = bus;
  this.new_bus(r.path);
  kr.send_json(JSON.stringify(r), this);
}

GrapherMix.prototype.insert = function(from_path, to_path) {
  var to_name = to_path.name;
  var from_name = from_path.name;
  var r = {};
  var insert = {};
  r.meth = "PUT";
  r.path = '/mixer/' + to_name + '/' + from_name;
  r.meta = {};
  insert.type = "input";
  insert.channels = "stereo";
  insert.afx = [];
  insert.afx[0] = {};
  insert.afx[0].pass = {};
  insert.afx[0].pass.low = {};
  insert.afx[0].pass.low.bw = 0.66;
  insert.afx[0].pass.low.hz = 20000.0;
  insert.afx[0].pass.high = {};
  insert.afx[0].pass.high.bw = 0.66;
  insert.afx[0].pass.high.hz = 20.0;
  insert.afx[0].analog = {};
  insert.afx[0].analog.drive = 0;
  insert.afx[0].analog.blend = 0;
  insert.afx[0].eq = {};
  insert.afx[0].eq.band = [];
  insert.afx[0].volume = {};
  insert.afx[0].volume.gain = 0;
  insert.afx[0].volume.fader = 0;
  insert.afx[1] = insert.afx[0];
  r.data = insert;
  this.on_put(r);
  kr.send_json(JSON.stringify(r), this);
}

GrapherMix.prototype.on_scene_click = function(name, evt) {
  evt.preventDefault();
  document.getElementById('eq').click();
  kr.eq.select_path('/mixer/' + name);
  return false;
}
