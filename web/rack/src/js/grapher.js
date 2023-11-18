$(document).ready(function() {
  rack_units.push({'constructor': Grapher, 'aspect': [32,14],
   'page': 'vgraph', 'id': 'compositor', 'position': [0, 1] });
});

var CONNECTOR_WIDTH = 20

function Grapher(info_object, id) {
  this.title = "Grapher";
  this.description = "Adjust properties of inputs to an output.";
  this.id = id;
  if (kr.grapher === undefined) {
    kr.grapher = {};
  }
  kr.grapher[id] = this;
  this.div = "<div class='grapher RCU overflow_auto' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  this.mode_listens_keydown = { 'HIDDEN': false, 'NORMAL': true, 'ACTION': false };
  this.inputs = [];
  this.outputs = [];
  this.buses = [];
  this.sources = [];
  this.paths = {};
  this.colors = {};
  this.column_counts = {};
  this.drawn = 0;
  this.drawn_outputs_count = {};
  this.drawn_inputs_count = {};
  this.on_keypress = create_handler(this, this.keypress);
  this.on_keypress_bus_create_cancel = create_handler(this,
   this.keypress_bus_create_cancel);
  info_object['parent_div'].append(this.div);
  $(this.sel).append('\
  <div class="grapher" id="gr_grapher_' + this.id + '">\
    <canvas class="graph" id="gr_graph_' + this.id + '"></canvas>\
    <div class="outputs" id="gr_outputs_' + this.id + '">\
    </div>\
    <div class="buses" id="gr_buses_' + this.id + '">\
    </div>\
    <div class="sources" id="gr_sources_' + this.id + '">\
    </div>\
  </div>');
  this.grapher = document.getElementById('gr_grapher_' + this.id);
  this.reticule = document.createElement("div");
  this.reticule.classList.add("reticule");
  this.reticule.innerHTML = "<h1>AIMED</h1>";
  this.targets = null;
  this.current_target = 0;
  this.current_shooter = null;
  this.canvas = document.getElementById("gr_graph_" + this.id);
  this.set_mode('HIDDEN');
  document.body.addEventListener('keydown', this.on_keypress);
}

Grapher.prototype.set_mode = function(mode_str) {
  if (typeof(this.mode_listens_keydown[mode_str]) != 'undefined') {
    this.mode = mode_str;
  }
}

Grapher.prototype.hidden = function() {
  this.set_mode('HIDDEN');
}

Grapher.prototype.visible = function() {
  this.set_mode('NORMAL');
}

Grapher.prototype.window_resize = function() {
  this.page_resize();
};

Grapher.prototype.page_resize = function() {
  this.redraw();
}

Grapher.prototype.column_counts_get_and_inc = function(level) {
  var ret;
  if (typeof this.column_counts[level] === 'undefined') {
    ret = 0
    this.column_counts[level] = 1;
  } else {
    ret = this.column_counts[level]++;
  }
  return ret;
}

Grapher.prototype.column_counts_clear = function() {
  this.column_counts = {};
}

Grapher.prototype.get_drawn_outputs_count = function(path) {
  if (typeof this.drawn_outputs_count[path.name] === 'undefined') {
    return 0;
  }
  return this.drawn_outputs_count[path.name];
}

Grapher.prototype.set_drawn_outputs_count = function(path, num) {
  this.drawn_outputs_count[path.name] = num;
}

Grapher.prototype.clear_drawn_outputs_count = function() {
  this.drawn_outputs_count = {};
}

Grapher.prototype.get_drawn_inputs_count = function(level) {
  if (typeof this.drawn_inputs_count[level] === 'undefined') {
    return 0;
  }
  return this.drawn_inputs_count[level];
}

Grapher.prototype.set_drawn_inputs_count = function(level, num) {
  this.drawn_inputs_count[level] = num;
}

Grapher.prototype.clear_drawn_inputs_count = function() {
  this.drawn_inputs_count = {};
}

Grapher.prototype.draw_connection = function(start, end, level, color) {
  var gc = this.canvas.getContext("2d");
  gc.lineWidth = this.connector_width;
  gc.strokeStyle = color;
  var dely = end.top - start.top;
  var delx = end.left - start.left;
  var bend = delx < 0 ? 'L' : delx > 0 ? 'R' : 'S';
  var first_length;
  if (bend == 'S') {
    first_length = Math.abs(dely);
  } else {
    var vertical_space = this.connector_width + this.connector_vertical_space;
    var num = this.get_drawn_inputs_count(level);
    first_length = Math.abs(dely) - this.connector_width - num * vertical_space;
    num++
    this.set_drawn_inputs_count(level, num);
  }
  gc.beginPath();
  gc.moveTo(start.left, start.top);
  gc.lineTo(start.left, start.top + ((end.top - start.top) / 3));
  gc.lineTo(end.left, start.top + ((end.top - start.top) / 1.5));
  gc.lineTo(end.left, end.top);
  gc.stroke();
}

Grapher.prototype.position_connection_target_names = function(from_name, to_name,
 to_location) {
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    var target_elem = this.inputs[i].target_elem;
    if (this.inputs[i].from_name == from_name
     && this.inputs[i].to_name == to_name) {
        this.position_connection_target(this.inputs[i], to_location);
        break;
    }
  }
}

Grapher.prototype.find_input = function(from_name, to_name) {
  var ret = null;
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    if (this.inputs[i].from_name == from_name
     && this.inputs[i].to_name == to_Name) {
      ret = this.inputs[i];
      break;
    }
  }
  return ret;
}

Grapher.prototype.select_connection = function(input) {
  input.target_elem.classList.add('selected');
  this.current_connection = input;
}

Grapher.prototype.deselect_connection = function(input) {
  input.target_elem.classList.remove('selected');
  this.current_connection = null;
}


Grapher.prototype.position_connection_target = function(input, to_location) {
  var elem = input.target_elem;
  var w = this.connector_width * 1.5;
  elem.style.width = w + 'px';
  elem.style.height = w + 'px';
  elem.style.top = (to_location.top - (w - 1)) + 'px';
  elem.style.left = (to_location.left - w/2) + 'px';
}

Grapher.prototype.remove_connection_target = function(input) {
  input.target_elem.parentNode.removeChild(input.target_elem);
  delete input.target_elem;
}

Grapher.prototype.clear_connection_targets = function(input) {
  /*for (var i = 0, len = this.inputs.length; i < len; i++) {
    var target_elem = this.inputs[i].target_elem;
    if (typeof target_elem != 'undefined') {
      this.remove_connection_target(this.inputs[i]);
    }
  }*/
}

Grapher.prototype.make_connections = function() {
  this.clear_drawn_outputs_count();
  this.clear_drawn_inputs_count();
  this.clear_connection_targets();
  var targets = this.outputs.concat(this.buses);
  targets.sort(function(a, b) { return a.level - b.level });
  for (var i = 0, len = targets.length; i < len; i++) {
    var to_path = this.paths[targets[i]];
    var num_inputs = to_path.inputs.length;
    var to_margin_tot = parseInt(to_path.elem.offsetWidth, 10)
     - this.connector_width * num_inputs;
    var to_margin = to_margin_tot / (num_inputs + 1);
    for (var j = 0; j < num_inputs; j++) {
      var from_path = to_path.inputs[j];
      if (typeof from_path == 'undefined') continue;
      var num_outputs = from_path.outputs.length;
      var from_margin_tot = parseInt(from_path.elem.offsetWidth, 10)
       - this.connector_width * num_outputs;
      var between_margin = this.connector_width / 3;
      var from_margin = (from_margin_tot - between_margin * (num_outputs - 1))
       / 2;
      var num_conn = this.get_drawn_outputs_count(from_path);
      var cs = getComputedStyle(from_path.elem);
      var container_cs = document.defaultView.getComputedStyle(this.grapher);
      var from_top = parseInt(cs.getPropertyValue('top'), 10);
      var from_left = parseInt(cs.getPropertyValue('left'), 10);
      var from_location = { 'top': from_top,
        'left': from_left + 0.5 * this.connector_width + from_margin
        + num_conn * (between_margin + this.connector_width)};
      num_conn++;
      this.set_drawn_outputs_count(from_path, num_conn);
      var cs = getComputedStyle(to_path.elem);
      var to_top = parseInt(cs.getPropertyValue('top'), 10);
      var to_left = parseInt(cs.getPropertyValue('left'), 10);
      var to_height = parseInt(cs.getPropertyValue('height'), 10);
      var to_location = { 'top': to_top + to_height,
        'left': to_left + 0.5 * this.connector_width + to_margin
        + j * (to_margin + this.connector_width)};
      var color = this.get_path_color(to_path);
      var to_level = to_path.level;
      this.draw_connection(from_location, to_location, to_level, color);
      this.position_connection_target_names(from_path.name, to_path.name,
       to_location);
    }
  }
}

Grapher.prototype.get_level_color = function(level) {
  var r = Math.floor(Math.random() * 85 + 85);
  var g = Math.floor(Math.random() * 85 + 85);
  var b = Math.floor(Math.random() * 85 + 85);
  return 'rgb(' + r + ', ' + g + ', ' + b + ')';
}

Grapher.prototype.get_path_color = function(path) {
  return this.get_level_color(this.get_path_level(path));
}

Grapher.prototype.get_path_level = function(path) {
  if (path instanceof GraphSource) {
    return -1;
  } else if (path instanceof GraphOutput) {
    return 0;
  }
  return this.get_bus_level(path);
}

Grapher.prototype.position_bus = function(bus, max_level, num_buses) {
  var grapher = $('#' + this.id);
  var grapher_top = grapher.offset().top;
  var h = $('#gr_buses_' + this.id).outerHeight(true) / (1 + max_level * 2);
  var w = $('#gr_buses_' + this.id).outerWidth(true) / (1 + num_buses * 2);
  bus.elem.style.height = h + 'px';
  bus.elem.style.width = w + 'px';
  var buses_offset_top = $('#gr_buses_' + this.id).position().top;
  var buses_offset_left = $('#gr_buses_' + this.id).position().left;
  bus.elem.style.top = (h * (2 * bus.level - 1) + buses_offset_top) + 'px';
  bus.elem.style.left = (w * (2 * this.drawn_buses + 1) + buses_offset_left)
   + 'px';
  this.drawn_buses++;
}

Grapher.prototype.get_bus_level = function(bus) {
  bus.level = 1;
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    var input_path = this.inputs[i];
    if (input_path.from_name == bus.name) {
      var to_name = input_path.to_name;
      var to_path = this.paths[to_name];
      if (to_path instanceof GraphBus) {
        var new_level = 1 + this.get_bus_level(to_path);
        if (new_level > bus.level) {
          bus.level = new_level;
        }
      }
    }
  }
  return bus.level;
}

Grapher.prototype.is_cyclic_util = function(path, visited, rec_stack) {
  if (visited[path] == false) {
    visited[path] = true;
    rec_stack[path] = true;
    var cur_path = this.paths[path];
    for (var p in cur_path.outputs) {
      var o = cur_path.outputs[p];
      if (!visited[o.name] && this.is_cyclic_util(o.name, visited, rec_stack)) {
        return true;
      } else if (rec_stack[o.name]) {
        return true;
      }
    }
  }
  rec_stack[path] = false;
  return false;
}

Grapher.prototype.creates_cycle = function(input) {
  var sources = this.buses.concat(this.sources);
  var from_path = this.paths[input.from_name];
  var to_path = this.paths[input.to_name];
  var visited = [];
  var rec_stack = [];
  from_path.add_output(to_path);
  for (var i in this.paths) {
    visited[i] = rec_stack[i] = false;
  }
  var ret = false;
  if (this.is_cyclic_util(input.from_name, visited, rec_stack)) {
    ret = true;
  }
  from_path.remove_output(to_path);
  return ret;
}

Grapher.prototype.remove_bus_create = function() {
  var form = document.getElementById('gr_form_create_path_' + this.id);
  form.parentElement.removeChild(form);
  this.set_mode("NORMAL");
}

Grapher.prototype.keypress_bus_create_cancel = function(evt) {
  if (evt.which == 27) { /*'Esc' -- cancel bus creation.*/
    this.remove_bus_create();
  }
}

Grapher.prototype.create_bus = function(name) {
  var r = {};
  var bus = {};
  r.meth = "PUT";
  r.path = "/compositor/" + name;
  r.meta = {};
  bus.opacity = 1.00;
  r.data = {};
  r.data.bus = bus;
  this.new_bus(r.path);
  kr.send_json(JSON.stringify(r), this);
}

Grapher.prototype.show_bus_create = function(type, opts) {
  this.set_mode("ACTION");
  var is_bus_insert = (type == 'I');
  var is_splice = (type == 'S');
  var is_bus_clone = (type == 'L');
  var is_bus_splice = (type == 'P');
  var is_bus_create = (type == 'C');
  var msg = "";
  if (is_bus_insert) {
    msg = ' from "' + opts.from_path.name + '" to "' + opts.to_path.name + '"';
  }
  $('#gr_grapher_' + this.id).append('\
<form name="gr_bus_create_' + this.id + '" class="creation_form" id="gr_form_create_path_' + this.id + '">\
New Bus ' + msg + ' \
Name:<input type="text" id="gr_com_bus_' + this.id + '" name="gr_com_bus_' + this.id + '"  required>\
<input type="submit" value="Create"/>\
</form>');
  document['gr_bus_create_' + this.id]['gr_com_bus_' + this.id].focus();
  $('#gr_form_create_path_' + this.id).submit(create_handler(this, function( event ) {
    if (is_bus_insert) {
      this.bus_insert($('#gr_com_bus_' + this.id).val(), opts.from_path, opts.to_path);
    } else if (is_splice) {
      var input = null;
      if (typeof opts.to_path != 'undefined') {
        for (var i = 0, len = this.inputs.length; i < len; i++) {
          if (this.inputs[i].to_name == opts.to_path.name
           && this.inputs[i].from_name == opts.from_path.name) {
            this.splice($('#gr_com_bus_' + this.id).val(), opts.to_path, [this.inputs[i]]);
            break;
          }
        }
      }
    } else if (is_bus_splice) {
      var inputs = [];
      if (typeof opts.to_path != 'undefined') {
        for (var i = 0, len = this.inputs.length; i < len; i++) {
          if (this.inputs[i].to_name == opts.to_path.name) {
            inputs.push(this.inputs[i]);
          }
        }
        this.splice($('#gr_com_bus_' + this.id).val(), opts.to_path, inputs);
      }
    } else if (is_bus_clone) {
      this.bus_clone($('#gr_com_bus_' + this.id).val(), opts.to_path);
    } else if (is_bus_create) {
      this.create_bus($('#gr_com_bus_' + this.id).val());
    }
    this.remove_bus_create();
    event.preventDefault();
  }));
}

Grapher.prototype.resize_canvas = function(elem) {
  var c = this.canvas;
  var w = elem.offsetWidth;
  var h = elem.offsetHeight;
  var dpr = window.devicePixelRatio;
  c.style.width = Math.round(w) + "px";
  c.style.height = Math.round(h) + "px";
  c.width = Math.round(w * dpr);
  c.height = Math.round(h * dpr);
  var gc = this.canvas.getContext("2d");
  gc.scale(dpr, dpr);
  this.connector_width = CONNECTOR_WIDTH / dpr;
  this.connector_vertical_space = this.connector_width / 2;
}

Grapher.prototype.redraw = function() {
  var grapher_elem = document.getElementById("gr_grapher_" + this.id);
  this.resize_canvas(grapher_elem);
  this.position_outputs();
  this.position_sources();
  this.column_counts_clear();
  this.drawn_buses = 0;
  var buses_length = this.buses.length;
  var me = this;
  for (var b = 0; b < buses_length; b++) {
    this.get_bus_level(this.paths[this.buses[b]]);
  }
  this.buses = this.buses.sort(function(a,b) { return (me.paths[a].level
   - me.paths[b].level); });
  var level_max = 0;
  if (this.buses.length > 0) {
    level_max = this.paths[this.buses[this.buses.length-1]].level;
  }
  for (var b = 0; b < buses_length; b++) {
    var bus_name = this.buses[b];
    var bus_path = this.paths[bus_name];
    this.get_bus_level(bus_path);
    this.position_bus(bus_path, level_max, buses_length);
  }
  this.make_connections();
}

Grapher.prototype.position_outputs = function() {
  var num_outputs = this.outputs.length;
  var margin = 50;
  var width = ($('#gr_outputs_' + this.id).outerWidth(true) - (num_outputs + 1) * margin)
   / num_outputs;
  var height = $('#gr_outputs_' + this.id).innerHeight();
  for (var i = 0, len = this.outputs.length; i < len; i++) {
    var output_name = this.outputs[i];
    var output_path = this.paths[output_name];
    if (output_path == undefined) continue;
    output_path.elem.style.top = 0 + 'px';
    output_path.elem.style.width = width + 'px';
    output_path.elem.style.height = height + 'px';
    output_path.elem.style.left = margin + i * (width + margin) + 'px';
  }
}

Grapher.prototype.position_sources = function() {
  var num_sources = this.sources.length;
  var margin = 50;
  var sources = document.getElementById('gr_sources_' + this.id);
  var width = (parseInt(getComputedStyle(sources).getPropertyValue('width'), 10)
   - ((num_sources + 1) * margin)) / num_sources;
  var container_cs = getComputedStyle(this.grapher);
  for (var i = 0, len = this.sources.length; i < len; i++) {
    var source_name = this.sources[i];
    var source_path = this.paths[source_name];
    if (source_path == undefined) continue;
    var cs = getComputedStyle(source_path.elem);
    var top = parseInt(container_cs.getPropertyValue('top'), 10)
     + parseInt(container_cs.getPropertyValue('height'), 10)
     - parseInt(cs.getPropertyValue('height'), 10);
    source_path.elem.style.top = top + 'px';
    source_path.elem.style.width = width + 'px';
    source_path.elem.style.left = margin + i * (width + margin) + 'px';
  }
}

Grapher.prototype.add_input = function(from_name, to_name, crate) {
  var input = new GraphInput(this, from_name, to_name, crate);
  this.inputs.push(input);
  var targets = this.buses.concat(this.outputs);
  for (var i = 0, len = targets.length; i < len; i++) {
    if (targets[i] == to_name) {
      var to_path = this.paths[to_name];
      var from_path = this.paths[from_name];
      if (typeof to_path == 'undefined' || typeof from_path == 'undefined') {
        continue;
      }
      to_path.add_input(from_path);
      from_path.add_output(to_path);
    }
  }
}

Grapher.prototype.new_input = function(path, crate) {
  var names = path.split('/');
  var from_name = names.pop();
  var to_name = names.pop();
  this.add_input(from_name, to_name, crate);
  this.redraw();
}

Grapher.prototype.remove_input = function(input) {
  var redraw = false;
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    if (this.inputs[i] === input) {
      this.remove_connection_target(this.inputs[i]);
      var num_conn = this.get_drawn_outputs_count(this.inputs[i].path);
      this.set_drawn_outputs_count(this.inputs[i].path, --num_conn);
      this.inputs.splice(i, 1);
      redraw = true;
      break;
    }
  }
  var targets = this.buses.concat(this.outputs);
  for (var i = 0, len = targets.length; i < len; i++) {
    if (targets[i] == input.to_name) {
      this.paths[input.to_name].remove_input(input.from_name);
      this.paths[input.from_name].remove_output(input.to_name);
      redraw = true;
    }
  }
  if (redraw) this.redraw();
}

Grapher.prototype.remove_input_by_names = function(from_name, to_name) {
  var redraw = false;
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    var input = this.inputs[i];
    if (input.from_name == from_name && input.to_name == to_name) {
      this.remove_connection_target(this.inputs[i]);
      this.inputs.splice(i, 1);
      redraw = true;
      break;
    }
  }
  var targets = this.buses.concat(this.outputs);
  for (var i = 0, len = targets.length; i < len; i++) {
    if (targets[i] == to_name) {
      this.paths[targets[i]].remove_input(from_name);
      this.paths[from_name].remove_output(to_name);
      redraw = true;
    }
  }
  if (redraw) this.redraw();
}

Grapher.prototype.remove_path = function(name) {
  var path = this.paths[name];
  if (path === undefined) return;
  var inputs = this.sources.concat(this.buses);
  for (var i = 0, len = inputs.length; i < len; i++) {
    var input = this.paths[inputs[i]];
    input.remove_output(name);
  }
  var outputs = this.buses.concat(this.outputs);
  for (var i = 0, len = outputs.length; i < len; i++) {
    var output = this.paths[outputs[i]];
    output.remove_input(name);
  }
  for (var i = 0, len = this.buses.length; i < len; i++) {
    if (this.buses[i] == name) {
      this.buses.splice(i, 1);
      break;
    }
  }
  for (var i = 0, len = this.outputs.length; i < len; i++) {
    if (this.outputs[i] == name) {
      this.outputs.splice(i, 1);
      break;
    }
  }
  if (this.current_shooter == path) {
    this.disarm_shooter();
  }
  path.elem.parentNode.removeChild(path.elem);
  delete this.paths[name];
  delete this.sources[name];
  this.redraw();
}

Grapher.prototype.on_del_path = function(path) {
  var name = path.split('/').pop();
  this.remove_path(name);
}

Grapher.prototype.on_del_input = function(path) {
  var names = path.split('/');
  var from_name = names.pop();
  var to_name = names.pop();
  this.remove_input_by_names(from_name, to_name);
}

Grapher.prototype.gen_path_elem = function(label) {
  var path_elem = document.createElement("div");
  path_elem.classList.add("path");
  var label_elem = document.createElement("div");
  label_elem.classList.add("path_label");
  label_elem.innerHTML = label;
  path_elem.appendChild(label_elem);
  return path_elem;
}

Grapher.prototype.new_output = function(path) {
  var name = path.split('/').pop();
  var output_elem = this.gen_path_elem(name);
  this.add_arm_button(output_elem);
  $('#gr_outputs_' + this.id).append(output_elem);
  var output = new GraphOutput(this, name, output_elem);
  this.paths[name] = output;
  this.outputs.push(name);
  var redraw_required = false;
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    if (this.inputs[i].to_name == name && typeof this.paths[this.inputs[i].from_name]
     !== 'undefined') {
      var input_path = this.paths[this.inputs[i].from_name]
      output.add_input(input_path);
      input_path.add_output(output);
      if (input_path instanceof GraphBus) {
        redraw_required = true;
      }
    }
  }
  if (redraw_required) {
    this.redraw();
  }
}

Grapher.prototype.new_bus = function(path) {
  var bus_name = path.split('/').pop();
  var bus_elem = this.gen_path_elem(bus_name);
  this.add_arm_button(bus_elem);
  bus_elem.classList.add('bus');
  $('#gr_buses_' + this.id).append(bus_elem);
  var bus = new GraphBus(this, bus_name, bus_elem);
  this.paths[bus_name] = bus;
  this.buses.push(bus_name);
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    var from_name = this.inputs[i].from_name;
    var from_path = this.paths[from_name];
    var to_name = this.inputs[i].to_name;
    if (to_name == bus_name && typeof from_path !== 'undefined') {
      bus.add_input(from_path);
      from_path.add_output(bus);
    }
    var to_name = this.inputs[i].to_name;
    var to_path = this.paths[to_name];
    var from_name = this.inputs[i].from_name;
    if (from_name == bus_name && typeof to_path !== 'undefined') {
      bus.add_output(to_path);
      to_path.add_input(bus);
    }
  }
  this.redraw();
}

Grapher.prototype.new_source = function(path) {
  var name = path.split('/').pop();
  var source_elem = this.gen_path_elem(name);
  this.add_arm_button(source_elem);
  $('#gr_sources_' + this.id).append(source_elem);
  var source = new GraphSource(this, name, source_elem);
  this.paths[name] = source;
  this.sources.push(name);
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    if (this.inputs[i].from_name == name &&
        typeof this.paths[this.inputs[i].to_name] !== 'undefined') {
      var to_path = this.paths[this.inputs[i].to_name];
      source.add_output(to_path);
      to_path.add_input(source);
      this.redraw();
    }
  }
}

Grapher.prototype.on_put = function(crate) {
  var path = crate.path;
  if (crate.data.input) {
    this.new_input(path, crate);
  }
  if (crate.data.output) {
    this.new_output(path);
  }
  if (crate.data.bus) {
    this.new_bus(path);
  }
  if (crate.data.source) {
    this.new_source(path);
  }
}

Grapher.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/compositor/")) return;
  if (crate.meth == "PUT") {
    this.on_put(crate);
  } else if (crate.meth == "DELETE") {
    var path = crate.path;
    var names = path.split("/");
    var len = names.length;
    if (len == 4) {
      this.on_del_input(path);
    } else {
      this.on_del_path(path);
    }
  }
}

Grapher.prototype.on_scene_click = function(name, evt) {
  evt.preventDefault();
  document.getElementById('scene').click();
  kr.scener.select_scene(name);
  return false;
}

Grapher.prototype.linkable_paths = function(path) {
  var paths = [];
  for (var i in this.paths) {
    if (this.armed_by_connection) {
      if (this.current_connection.to_name == this.paths[i].name)
        continue;
    }
    if ((!(this.paths[i] instanceof GraphSource))
     && (!(this.paths[i] == path))) {
      var input = new GraphInput(this, path.name, this.paths[i].name);
      if (!this.creates_cycle(input))
        paths.push(this.paths[i]);
      input.destroy();
    }
  }
  return paths;
}

Grapher.prototype.fire = function() {
  var to_name = this.targets[this.current_target].name;
  var from_name = this.current_shooter.name;
  for (var i = 0, len = this.inputs.length; i < len ; i++) {
    var input_path = this.inputs[i];
    if (input_path.from_name == from_name && input_path.to_name == to_name) {
      this.drop(input_path);
      return;
    }
  }
  this.insert(this.current_shooter, this.targets[this.current_target]);
}

Grapher.prototype.on_bus_insert = function() {
  this.show_bus_create('I', {'from_path': this.current_shooter,
   'to_path': this.targets[this.current_target]});
}

Grapher.prototype.on_bus_drop = function() {
  var shooter = this.current_shooter;
  if (shooter instanceof GraphBus) {
    this.bus_drop(shooter);
  }
}

Grapher.prototype.on_splice = function() {
  if (this.current_target > -1 && this.current_shooter != null
   && typeof this.targets[this.current_target] != 'undefined') {
    this.show_bus_create('S', {'from_path': this.current_shooter,
     'to_path': this.targets[this.current_target]});
  }
}

Grapher.prototype.on_unsplice = function() {
  this.unsplice(this.current_shooter);
}

Grapher.prototype.on_clone_bus = function() {
  this.show_bus_create('L', {'to_path': this.current_shooter});
}

Grapher.prototype.on_bus_splice = function() {
  this.show_bus_create('P', {'to_path': this.current_shooter});
}

Grapher.prototype.on_cancel = function() {
  this.disarm_shooter();
}

Grapher.prototype.on_insert_clone = function() {
  this.clone(this.current_connection, this.targets[this.current_target]);
}

Grapher.prototype.keypress = function(evt) {
  if (evt.defaultPrevented) {
    return; // Should do nothing if the key event was already consumed.
  }
  if (!this.mode_listens_keydown[this.mode]) {
    return;
  }
  var handled = false;
  if (evt.which == 66) { /*'b' -- create bus.*/
    this.show_bus_create('C');
    handled = true;
  } else if (this.current_shooter != null) {
    if (evt.which == 37) { /*Left Arrow.*/
      handled = true;
      this.change_aim(-1);
    } else if (evt.which == 39) { /*Right Arrow.*/
      handled = true;
      this.change_aim(1);
    } else if (evt.which == 27) { /*'Esc' -- cancel.*/
      this.on_cancel();
      handled = true;
    } else if (this.armed_by_connection) {
      if (evt.which == 67) { /*'c' -- clone insert.*/
        this.on_insert_clone();
        handled = true;
      }
    } else {
      if (evt.which == 70) { /*'f' -- fire.*/
        handled = true;
        this.fire();
      } else if (evt.which == 73) { /*'i' -- bus_insert.*/
        this.on_bus_insert();
        handled = true;
      } else if (evt.which == 68) { /*'d' -- drop bus.*/
        this.on_bus_drop();
        handled = true;
      } else if (evt.which == 83) { /*'s' -- splice.*/
        this.on_splice();
        handled = true;
      } else if (evt.which == 85) { /*'u' -- unsplice.*/
        this.on_unsplice();
        handled = true;
      } else if (evt.which == 76) { /*'l' -- clone bus.*/
        this.on_clone_bus();
        handled = true;
      } else if (evt.which == 80) { /*'p' -- bus splice.*/
        this.on_bus_splice();
        handled = true;
      }
    }
  }

  if (handled) {
    evt.preventDefault();
  }
}



Grapher.prototype.add_arm_button = function(elem) {
  var arm_button = document.createElement("div");
  arm_button.classList.add("path_select_button");
  arm_button.innerHTML = "Ready";
  elem.appendChild(arm_button);
}

Grapher.prototype.aim_down = function() {
  this.targets[this.current_target].elem.removeChild(this.reticule);
  this.targets[this.current_target].elem.classList.add("target");
}

Grapher.prototype.aim = function() {
  var path = this.targets[this.current_target];
  path.elem.classList.remove("target");
  path.elem.appendChild(this.reticule);
  this.reticule.classList.add("sighted");
}

Grapher.prototype.change_aim = function(del) {
  if (this.current_target == -1) {
    this.current_target = 0;
  } else {
    this.aim_down();
    this.current_target = (this.current_target + del) % this.targets.length;
    while (this.current_target < 0) {
      this.current_target = this.current_target + this.targets.length;
    }
  }
  this.aim()
}

Grapher.prototype.untarget_all = function() {
  if (this.targets != null && this.targets.length > 0) {
    for (var i = 0, len = this.targets.length; i < len; i++) {
      var path = this.targets[i];
      path.elem.classList.remove("target");
    }
    if (this.current_target > -1) {
      this.targets[this.current_target].elem.removeChild(this.reticule);
   }
  }
  this.targets = null;
}

Grapher.prototype.disarm_shooter = function() {
  this.untarget_all();
  if (this.armed_by_connection) {
    this.armed_by_connection = false;
    this.deselect_connection(this.current_connection);
  }
  if (this.current_shooter != null) {
    this.reticule.classList.remove("sighted");
    this.current_shooter.elem.classList.remove("ready");
    this.current_shooter = null;
    return;
  }
}

Grapher.prototype.arm_shooter = function(path, filter) {
  if (this.current_shooter != null) {
    this.disarm_shooter();
  }
  if (filter == 'C') {
    this.armed_by_connection = true;
  }
  path.elem.classList.add("ready");
  this.current_target = -1;
  this.current_shooter = path;
  if (!(this.current_shooter instanceof GraphOutput)) {
    this.targets = this.linkable_paths(path);
    for (var t in this.targets) {
      this.targets[t].elem.classList.add("target");
    }
  }
}

Grapher.prototype.shooting_off = function() {
  this.disarm_shooter();
}

Grapher.prototype.shooting_on = function(name) {
  var sources = this.sources.concat(this.buses).concat(this.outputs);
  for (var i = 0, len = sources.length; i < len; i++) {
    if (sources[i] == name) {
      var source_path = this.paths[sources[i]];
      this.arm_shooter(source_path);
      break;
    }
  }
}

Grapher.prototype.toggle_shooting = function(name) {
  var already_shooting = ((this.current_shooter != null)
   && (this.current_shooter.name == name));
  this.shooting_off();
  if (already_shooting) return;
  this.shooting_on(name);
}

Grapher.prototype.insert = function(from_path, to_path) {
  var to_name = to_path.name;
  var from_name = from_path.name;
  var r = {};
  var insert = {};
  r.meth = "PUT";
  r.path = '/compositor/' + to_name + '/' + from_name;
  r.meta = {};
  insert.input = {};
  insert.input.opacity = 1;
  insert.input.pos = { "x": 0, "y": 0, "w": 7680, "h": 4320 };
  insert.input.crop = { "x": 0,"y": 0, "w": 7680, "h" : 4320 };
  insert.input.view = { "top_left": {"x": 0, "y": 0}, "top_right": {"x": 0,
   "y": 0}, "bottom_left": {"x": 0, "y": 0}, "bottom_right": {"x": 0, "y": 0}};
  insert.input.z = 0;
  insert.input.rotation = 0;
  r.data = insert;
  this.on_put(r);
  kr.send_json(JSON.stringify(r), this);
}

Grapher.prototype.bus_insert = function(name, from_path, to_path) {
  this.create_bus(name);
  this.insert(from_path, this.paths[name]);
  this.insert(this.paths[name], to_path);
}

Grapher.prototype.splice = function(name, to_path, inputs) {
  this.create_bus(name);
  for (var i = 0, len = inputs.length; i < len; i++) {
    this.clone(inputs[i], this.paths[name]);
    this.drop(inputs[i]);
  }
  this.insert(this.paths[name], to_path);
}

Grapher.prototype.clone = function(old_input, to_path) {
  var crate = {};
  crate['put'] = '/compositor/' + to_path.name + '/' + old_input.from_name;
  crate['compositor_path_info'] = old_input.crate.compositor_path_info;
  kr.send_json(JSON.stringify(crate));
}

Grapher.prototype.bus_clone = function(name, bus) {
  this.create_bus(name);
  var inputs = this.inputs.slice(0);
  for (var i = 0, len = inputs.length; i < len; i++) {
    if (inputs[i].to_name == bus.name) {
      this.clone(inputs[i], this.paths[name]);
    }
  }
}

Grapher.prototype.drop = function(input_path) {
  this.remove_input(input_path);
  kr.rm(input_path.path, this);
}

Grapher.prototype.unsplice = function(bus) {
  var bus_outputs = bus.outputs.slice(0);
  var bus_inputs = bus.inputs.slice(0);
  var inputs = this.inputs.slice(0);
  for (var i = 0, len = bus_outputs.length; i < len; i++) {
    for (var j = 0, len2 = bus_inputs.length; j < len2; j++) {
      for (var k = 0, len3 = inputs.length; k < len3; k++) {
        if (inputs[k].from_name == bus_inputs[j].name
         && inputs[k].to_name == bus.name) {
          this.clone(inputs[k], bus_outputs[i]);
        }
      }
    }
  }
  this.bus_drop(bus);
}

Grapher.prototype.bus_drop = function(bus) {
  var bus_inputs = bus.inputs.slice(0);
  for (var i = 0, len = bus_inputs.length; i < len; i++) {
    var inputs = this.inputs.slice(0);
    for (var j = 0, len2 = inputs.length; j < len2; j++) {
      if (bus_inputs[i].name == inputs[j].from_name
       && bus.name == inputs[j].to_name) {
        this.drop(inputs[j]);
      }
    }
  }
  var bus_outputs = bus.outputs.slice(0);
  for (var i = 0, len = bus_outputs.length; i < len; i++) {
    var inputs = this.inputs.slice(0);
    for (var j = 0, len2 = inputs.length; j < len2; j++) {
      if (bus.name == inputs[j].from_name
       && bus_outputs[i].name == inputs[j].to_name) {
        this.drop(inputs[j]);
      }
    }
  }
  kr.rm('/' + this.id + '/' + bus.name, this);
  this.remove_path(bus.name);
}

GraphOutput.prototype.on_ready_click = function(evt) {
  this.grapher.toggle_shooting(this.name);
}

GraphBus.prototype.on_ready_click = function(evt) {
  this.grapher.toggle_shooting(this.name);
}

GraphSource.prototype.on_ready_click = function(evt) {
  this.grapher.toggle_shooting(this.name);
}

GraphBus.prototype.on_scene_click = function(evt) {
  this.grapher.on_scene_click(this.name, evt);
}

GraphOutput.prototype.on_scene_click = function(evt) {
  this.grapher.on_scene_click(this.name, evt);
}

Grapher.prototype.on_click_connection_target = function(input) {
  var target = input.target_elem;
  var already_selected = false;
  if (target.classList.contains('connection_target')
   && target.classList.contains('selected')) {
    already_selected = true;
  }
  if (this.current_shooter != null) {
    this.disarm_shooter(this.current_shooter);
  }
  if (already_selected) return;
  this.select_connection(input);
  this.arm_shooter(this.paths[input.from_name], 'C');
}

GraphInput.prototype.on_click = function(evt) {
 this.grapher.on_click_connection_target(this);
}

GraphInput.prototype.destroy = function() {
  this.target_elem.parentNode.removeChild(this.target_elem);
}

function GraphInput(grapher, from_name, to_name, crate) {
  this.grapher = grapher;
  this.path = '/' + this.grapher.id + '/' + to_name + '/' + from_name;
  this.from_name = from_name;
  this.to_name = to_name;
  this.crate = typeof crate == 'undefined' ? null : crate;
  this.target_elem = document.createElement("div");
  this.target_elem.classList.add('connection_target');
  var grapher_elem = document.getElementById('gr_grapher_' + this.grapher.id);
  grapher_elem.appendChild(this.target_elem);
  this.target_elem.addEventListener('click', create_handler(this,
   this.on_click));
}

function GraphBus(grapher, name, elem) {
  this.grapher = grapher;
  this.name = name;
  this.level = 1;
  this.outputs = [];
  this.inputs = [];
  this.targets = [];
  this.current_target = null;
  this.elem = elem;
  var labels = this.elem.getElementsByClassName('path_label');
  if (labels.length == 1) {
    labels[0].onclick = create_handler(this, this.on_scene_click);
  }
  var select_btns = this.elem.getElementsByClassName('path_select_button');
  if(select_btns.length == 1) {
    select_btns[0].onclick = create_handler(this, this.on_ready_click);
  }
}

GraphBus.prototype.remove_output = function(output) {
  for (var i = 0, len = this.outputs.length; i < len; i++) {
    if (this.outputs[i] == output) {
      this.outputs.splice(i, 1);
      return;
    }
  }
}

GraphBus.prototype.remove_input = function(input) {
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    if (this.inputs[i].name == input) {
      this.inputs.splice(i, 1);
      return;
    }
  }
}

GraphBus.prototype.add_input = function(input) {
  this.inputs.push(input);
}

GraphBus.prototype.add_output = function(input) {
  this.outputs.push(input);
}

GraphSource.prototype.on_scene_click = function(evt) {
  if (this.grapher.id == 'mixer') {
    this.grapher.on_scene_click(this.name, evt);
  }
}

function GraphSource(grapher, name, elem) {
  this.grapher = grapher;
  this.name = name;
  this.outputs = [];
  this.targets = [];
  this.current_target = null;
  this.elem = elem;
  var select_btns = this.elem.getElementsByClassName('path_select_button');
  var labels = this.elem.getElementsByClassName('path_label');
  if (labels.length == 1) {
    labels[0].onclick = create_handler(this, this.on_scene_click);
  }
  if(select_btns.length == 1) {
    select_btns[0].onclick = create_handler(this, this.on_ready_click);
  }
}

GraphSource.prototype.remove_output = function(output) {
  for (var i = 0, len = this.outputs.length; i < len; i++) {
    if (this.outputs[i] == output) {
      this.outputs.splice(i, 1);
      return;
    }
  }
}

GraphSource.prototype.add_output = function(output) {
  this.outputs.push(output);
}

function GraphOutput(grapher, name, elem) {
  this.grapher = grapher;
  this.name = name;
  this.inputs = [];
  this.elem = elem;
  var labels = this.elem.getElementsByClassName('path_label');
  if (labels.length == 1) {
    labels[0].onclick = create_handler(this, this.on_scene_click);
  }
  var select_btns = this.elem.getElementsByClassName('path_select_button');
  if(select_btns.length == 1) {
    select_btns[0].onclick = create_handler(this, this.on_ready_click);
  }
}

GraphOutput.prototype.remove_input = function(input) {
  for (var i = 0, len = this.inputs.length; i < len; i++) {
    if (this.inputs[i].name == input) {
      this.inputs.splice(i, 1);
      return;
    }
  }
}

GraphOutput.prototype.add_input = function(input) {
  this.inputs.push(input);
}
