$(document).ready(function() {
  rack_units.push({'constructor': Mixer});
});

function Mixer(info_object) {
  this.is_visible = false;
  this.title = "Mixer";
  this.description = "Control the Mix";
  this.id = "kr_mixer";
  this.div = "<div class='RCU' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  this.full_size = [1920, 995];
  this.scale = 1;
  this.unit = this;
  this.which_view = 0;
  this.mixer_path_views = {};
  this.crossfades = {};
  this.xfade_inputs = {};
  this.transitions = [];
  this.color_scheme = new KRMonoColorScheme(MIXER_BG_COLOR);
  info_object['parent_div'].append(this.div);
  $(this.sel).append('\
      <div class="mixer_container"\
       id="' + this.id + '_container">\
      <canvas class="mixer_canvas" id="mixer_canvas"></canvas>\
      </div>');
  this.canvas_container = document.getElementById(this.id + '_container');
  this.canvas = document.getElementById("mixer_canvas");
  this.context = this.canvas.getContext("2d");
  this.init_touch_listeners();
  this.resize_canvas(this.canvas_container);
  this.canvas.addEventListener('mousedown',
   create_handler(this, this.on_mousedown));
  var mouse_wheel_evt=(/Firefox/i.test(navigator.userAgent)) ?
   "DOMMouseScroll" : "mousewheel";
  this.canvas.addEventListener(mouse_wheel_evt,
   create_handler(this, this.on_mousescroll));
  window.addEventListener('mouseup',
   create_handler(this, this.on_mouseup));
  window.addEventListener('mousemove',
   create_handler(this, this.on_mousemove));
  this.damaged = true;
  this.resize_needed = true;
  var _redraw = create_handler(this, this.redraw);
  document.body.addEventListener('keydown', create_handler(this,
   this.on_keydown));
  document.body.addEventListener('keyup', create_handler(this,
   this.on_keyup));
  document.body.addEventListener('keydown', create_handler(this,
   this.on_keypress));
  this.set_colors(Mixer.colors_from_color(this.color_scheme.next()));
  (function animloop() {
    requestAnimFrame(animloop);
    _redraw();
  })();
}

Mixer.prototype.init_touch_listeners = function() {
  var el = this.canvas;
  el.addEventListener("touchstart", create_handler(this, this.on_touchstart));
  el.addEventListener("touchend", create_handler(this, this.on_touchend));
  el.addEventListener("touchcancel", create_handler(this, this.on_touchcancel));
  el.addEventListener("touchmove", create_handler(this, this.on_touchmove));
}

Mixer.prototype.on_touchstart = function(evt) {
  var touches = evt.changedTouches;
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    for (var t = 0; t < touches.length; t++) {
      var touch = touches[t];
      var coords = p.event_to_canvas(touch);
      if (p.bbox.in(coords)) {
        p.on_touchstart(evt);
      }
    }
  }
}

Mixer.prototype.on_touchend = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_touchend(evt);
  }
}

Mixer.prototype.on_touchcancel = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_touchcancel(evt);
  }
}

Mixer.prototype.on_touchmove = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_touchmove(evt);
  }
}

Mixer.prototype.on_keypress = function(evt) {
  if (this.is_visible) {
    if (evt.which === 73) { /* i */
      this.no_inputs = this.no_inputs ? false : true;
      this.layout();
    } else if (evt.which === 67) { /* c */
      this.which_view = this.which_view === 1 ? 0 : 1;
      this.layout();
    }
  }
}

Mixer.prototype.on_keydown = function(evt) {
  var key = evt.key || evt.keyIdentifier;
  if (this.is_visible) {
    if (key == "h" || key == "U+0048") {
      this.h_pressed = true;
    }
    if (key == "s" || key == "U+0053") {
      this.s_pressed = true;
    }
    if (key == "v" || key == "U+0056") {
      this.v_pressed = true;
    }
  }
}

Mixer.prototype.on_keyup = function(evt) {
  var key = evt.key || evt.keyIdentifier;
  if (key == "h" || key == "U+0048") {
   this.h_pressed = false;
  }
   if (key == "s" || key == "U+0053") {
   this.s_pressed = false;
  }
  if (key == "v" || key == "U+0056") {
    this.v_pressed = false;
  }
}

Mixer.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.color_scheme = new KRMonoColorScheme(this.colors.trough_fill);
  for (var key in this.mixer_path_views) {
    for (var i = 0, len = this.mixer_path_views[key].length; i < len; i++) {
      var p = this.mixer_path_views[key][i];
      p.set_colors(colors);
      this.damage();
    }
  }
}

Mixer.prototype.change_h = function(del) {
  var colors = {};
  colors.trough_stroke = this.colors.trough_stroke.change_hue(del);
  colors.trough_fill = this.colors.trough_fill.change_hue(del);
  colors.handle_stroke = this.colors.handle_stroke.change_hue(del);
  colors.handle_fill = this.colors.handle_fill.change_hue(del);
  this.set_colors(colors);
}

Mixer.prototype.change_s = function(del) {
  var colors = {};
  colors.trough_stroke = this.colors.trough_stroke.change_s(del);
  colors.trough_fill = this.colors.trough_fill.change_s(del);
  colors.handle_stroke = this.colors.handle_stroke.change_s(del);
  colors.handle_fill = this.colors.handle_fill.change_s(del);
  this.set_colors(colors);
}

Mixer.prototype.change_v = function(del) {
  var colors = {};
  colors.trough_stroke = this.colors.trough_stroke.change_v(del);
  colors.trough_fill = this.colors.trough_fill.change_v(del);
  colors.handle_stroke = this.colors.handle_stroke.change_v(del);
  colors.handle_fill = this.colors.handle_fill.change_v(del);
  this.set_colors(colors);
}

Mixer.prototype.my_on_mousescroll = function(evt) {
  if (!this.active) return;
  var evt=window.event || evt;
  var delta=evt.detail? evt.detail : -evt.wheelDelta;
  var del = 0.003;
  del = delta > 0 ? del : -del;
  if (this.h_pressed) {
    this.change_h(-4 * del);
    return true;
  } else if (this.v_pressed) {
    this.change_v(-2 * del);
    return true;
  } else if (this.s_pressed) {
    this.change_s(-4 * del);
    return true;
  }
  return false;
}

Mixer.prototype.on_mousescroll = function(evt) {
  if (!this.active) return;
  if (this.my_on_mousescroll(evt)) {
    evt.preventDefault();
  } else {
    for (var key in this.mixer_path_views) {
      var v = this.mixer_path_views[key][this.which_view];
      if (v.on_mousescroll) {
        v.on_mousescroll(evt);
        if (v.damaged) {
          evt.preventDefault();
          this.damaged = true;
        }
      }
    }
  }
}


Mixer.prototype.damage = function() {
  this.damaged = true;
}

Mixer.prototype.on_mousedown = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_mousedown(evt);
    if (p.damaged) {
      this.damaged = true;
    }
  }
}

Mixer.prototype.on_mouseup = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_mouseup(evt);
    if (p.damaged) {
      this.damaged = true;
    }
  }
}

Mixer.prototype.on_mousemove = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_mousemove(evt);
    if (p.damaged) {
      this.damaged = true;
    }
  }
}

/*Mixer.prototype.on_mousescroll = function(evt) {
  for (var key in this.mixer_path_views) {
    var p = this.mixer_path_views[key][this.which_view];
    p.on_mousescroll(evt);
    if (p.damaged) {
      this.damaged = true;
    }
  }
}*/

Mixer.prototype.resize_canvas = function(elem) {
  var canvases = [this.canvas];
  var w = elem.offsetWidth;
  var h = elem.offsetHeight;
  if (w == 0 || h == 0) {
    this.resize_needed = true;
    this.damaged = true;
    return;
  }
  this.dpr = window.devicePixelRatio;
  for (var i = 0; i < canvases.length; i++) {
    var c = canvases[i];
    c.style.width = Math.round(w) + "px";
    c.style.height = Math.round(h) + "px";
    c.width = Math.round(w * this.dpr);
    c.height = Math.round(h * this.dpr);
    var gc = c.getContext("2d");
    gc.scale(this.dpr, this.dpr);
  }
  this.canvas_height = h;
  this.canvas_width = w;
  this.scale = this.calc_scale();
}

Mixer.prototype.calc_scale = function() {
  var scale = this.canvas_width / this.full_size[0];
  return scale;
}

Mixer.prototype.page_resize = function() {
  this.resize_needed = true;
  this.damaged = true;
}

Mixer.prototype.visible = function() {
  this.resize_needed = true;
  this.damaged = true;
  this.is_visible = true;
  for (key in this.mixer_path_views) {
    for (var i = 0, len = this.mixer_path_views[key].length; i < len; i++) {
      this.mixer_path_views[key][i].is_visible = true;
    }
  }
}

Mixer.prototype.hidden = function() {
  this.is_visible = false;
  for (key in this.mixer_path_views) {
    for (var i = 0, len = this.mixer_path_views[key].length; i < len; i++) {
      this.mixer_path_views[key][i].is_visible = false;
    }
  }
}

Mixer.prototype.redraw = function() {
  if (this.is_visible) {
    if (this.damaged) {
      if (this.resize_needed) {
        var container = this.canvas_container;
        this.resize_canvas(container);
        for (var key in this.mixer_path_views) {
          for (var i = 0, len = this.mixer_path_views[key].length; i < len; i++) {
            var view = this.mixer_path_views[key][i];
            if (view.resize) view.resize(this.scale);
            view.damaged = true;
          }
        }
        this.resize_needed = false;
      }
      for (var key in this.mixer_path_views) {
        var view = this.mixer_path_views[key][this.which_view];
        view.render();
      }
      this.damaged = false;
    }
  }
}

Mixer.prototype.window_resize = function() {
  this.page_resize();
};

Mixer.prototype.on_self_patch = function(crate) {
  if (this.mixer_path_views[crate.path]) {
    for (var i = 0, len = this.mixer_path_views[crate.path].length; i < len; i++) {
      var mc = this.mixer_path_views[crate.path][i];
      if (mc == this.mixer_path_views[crate.path][this.which_view]) continue;
      if (mc.meter.model.on_patch) {
        mc.meter.model.on_patch(crate);
      }
      for (var j = 0, len1 = crate.data.length; j < len1; j++) {
        var p = crate.data[j];
        if (p !== undefined) {
          if (p.op == "replace") {
            for (var k = 0, len2 = mc.subviews.length; k < len2; k++) {
              var sv = mc.subviews[k];
              if (sv.model instanceof KRSliderModel) {
                var idx = sv.model.prop.indexOf(p.path);
                if (idx > -1) {
                  sv.model.set_pos(p.value);
                  this.damaged = true;
                }
              }
            }
          }
        }
      }
    }
  } else if (this.xfade_inputs[crate.path]) {
    for (var i = 0, len = this.xfade_inputs[crate.path].length; i < len; i++) {
      var xfade_model = this.xfade_inputs[crate.path][i];
      var parts = crate.path.split('/');
      parts.pop();
      var crossfade_path = parts.join('/');
      if (xfade_model != this.crossfades[crossfade_path][this.which_view].xfade_view.model) {
        for (var j = 0, len1 = crate.data.length; j < len1; j++) {
          var p = crate.data[j];
          if (p !== undefined) {
            if (p.op == "replace") {
              xfade_model.on_patch(crate);
              this.damaged = true;
            }
          }
        }
      }
    }
  }
}

Mixer.prototype.on_patch = function(crate) {
  if (this.mixer_path_views[crate.path]) {
    for (var i = 0, len = this.mixer_path_views[crate.path].length; i < len; i++) {
      var mc = this.mixer_path_views[crate.path][i];
      if (mc.meter.model.on_patch) {
        mc.meter.model.on_patch(crate);
      }
      for (var j = 0, len1 = crate.data.length; j < len1; j++) {
        var p = crate.data[j];
        if (p !== undefined) {
          if (p.op == "replace") {
            for (var k = 0, len2 = mc.subviews.length; k < len2; k++) {
              var sv = mc.subviews[k];
              if (sv.model instanceof KRSliderModel) {
                var idx = sv.model.prop.indexOf(p.path);
                if (idx > -1) {
                  sv.model.set_pos(p.value);
                  this.damaged = true;
                }
              }
            }
          }
        }
      }
    }
  } else if (this.xfade_inputs[crate.path]) {
    for (var i = 0, len = this.xfade_inputs[crate.path].length; i < len; i++) {
      for (var j = 0, len1 = crate.data.length; j < len1; j++) {
        var p = crate.data[j];
        if (p !== undefined) {
          if (p.op == "replace") {
            this.xfade_inputs[crate.path][i].on_patch(crate);
            this.damaged = true;
          }
        }
      }
    }
  }
}

Mixer.prototype.self_update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/mixer/")) return;
  if (crate.meth == "PATCH") {
    this.on_self_patch(crate);
  }
}

Mixer.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/mixer/")) return;
  if (crate.meth == "PATCH") {
    this.on_patch(crate);
  } else if (crate.meth == "PUT") {
    this.on_new_path(crate);
  } else if (crate.meth == "DELETE") {
    this.on_del(crate);
  }
}

Mixer.prototype.on_del = function(crate) {
  delete this.mixer_path_views[crate.path];
  this.layout();
}

Mixer.prototype.on_new_path = function(crate) {
  var x = 0;
  var y = 0;
  var path_w = 0;
  var path_h = 0;
  var path = crate.path;
  var bbox = new KRBoundBox(x, y, path_w, path_h);
  var colors = Mixer.colors_from_color(this.color_scheme.next());
  if (crate.data.type == "bus" && kr.crate_path_match(crate, "cross")) {
    this.crossfades[path] = [new MixerPathCrossfadeBusView(this, crate,
     this.context, bbox, colors),new MixerPathCrossfadeBusView(this, crate,
     this.context, bbox, colors)];
    this.mixer_path_views[path] = [this.crossfades[path][0], this.crossfades[path][1]];
  } else if (crate.data.type == "input" && Object.keys(this.crossfades).length > 0) {
    var found = false;
    var parts = crate.path.split('/');
    var name = parts.pop();
    var into = parts.pop();
    for (var key in this.crossfades) {
      var c = this.crossfades[key];
      for (var i = 0, len = c.length; i < len; i++) {
        var model = c[i].xfade_view.model;
        if (model.name == into) {
          model.add_input(crate);
          if (this.xfade_inputs[crate.path] === undefined) {
            this.xfade_inputs[crate.path] = [ model ];
          } else {
            this.xfade_inputs[crate.path][this.xfade_inputs[crate.path].length] = model;
          }
          found = true;
        }
      }
    }
    if (!found) {
      this.mixer_path_views[crate.path] = [new MixerPathView(this, crate,
       this.context, bbox, colors), new MixerPathGainFaderView(this, crate,
       this.context, bbox, colors)];
    }
  } else {
    this.mixer_path_views[crate.path] = [new MixerPathView(this, crate,
     this.context, bbox, colors), new MixerPathGainFaderView(this, crate,
       this.context, bbox, colors)];
  }
  this.layout();
}

Mixer.prototype.layout = function() {
  var keys = Object.keys(this.mixer_path_views);
  var g = kr.grapher['mixer'];
  var sorted = [];
  for (var i = 0, len = g.sources.length; i < len; i++) {
    sorted[i] = '/mixer/' + g.sources[i];
  }
  for (var i = 0, len = g.buses.length; i < len; i++) {
    var bus = g.buses[i];
    if (!this.mixer_path_views['/mixer/'+bus][this.which_view].is_crossfade) {
      for (var j = 0, len2 = g.paths[bus].inputs.length; j < len2; j++) {
        var path = '/mixer/' + bus + '/' + g.paths[bus].inputs[j].name;
        if (!this.no_inputs) {
          this.mixer_path_views[path][this.which_view].activate();
          sorted[sorted.length] = path;
        } else {
          this.mixer_path_views[path][this.which_view].deactivate();
        }
      }
    }
    sorted[sorted.length] = '/mixer/' + bus;
  }
  for (var i = 0, len = g.outputs.length; i < len; i++) {
    var out = g.outputs[i];
    for (var j = 0, len2 = g.paths[out].inputs.length; j < len2; j++) {
      var path = '/mixer/' + out + '/' + g.paths[out].inputs[j].name;
      if (!this.no_inputs) {
        this.mixer_path_views[path][this.which_view].activate();
        sorted[sorted.length] = path;
      } else {
        this.mixer_path_views[path][this.which_view].deactivate();
      }
    }
    sorted[sorted.length] = '/mixer/' + out;
  }
  var len = sorted.length;
  var num_inputs = 0;
  for (var i = 0; i < len; i++) {
    var v = this.mixer_path_views[sorted[i]][this.which_view];
    if (v.crate.data.type === 'input') num_inputs++;
  }
  var den = g.sources.length + g.buses.length + g.outputs.length
    + 0.666*num_inputs;
  this.aspect = 4;
  var w_fs = Math.min((this.full_size[1]-195)/this.aspect,
      (this.full_size[0] - 40) / den);
  var h_fs = w_fs * this.aspect;
  var h = h_fs;
  var x = 20;
  var y = 20;
  var x_p, y_p, w_p, h_p;
  for (var i = 0; i < len; i++) {
    var j = this.which_view;
    var notj = this.which_view === 1 ? 0 : 1;
    var view = this.mixer_path_views[sorted[i]][j];
    if (view.crate.data.type === 'input') {
      w = w_fs * 0.666;
      h = h_fs * 0.666;
      y = 20 + h_fs * (1.0 - 0.666);
    } else {
      w = w_fs;
      h = h_fs;
      y = 20;
    }
    view.layout(new KRBoundBox(x,y,w,h));
    view.set_colors(Mixer.colors_from_color(this.color_scheme.next()));
    x = x + w;
  }
  this.active = true;
  this.resize_needed = true;
  this.damaged = true;
}

Mixer.colors_from_color = function(c) {
  var colors = {};
  colors.trough_fill = c.darken();
  colors.trough_stroke = c.lighten().lighten();
  colors.handle_fill = c.copy();
  colors.handle_stroke = colors.trough_stroke.copy();
  return colors;
}

Mixer.prototype.random_colors = function() {
  var colors = {};
  colors.trough_fill = KRColor.random_light(1);
  colors.trough_stroke = KRColor.copy_color_with_alpha(1.0);
  colors.handle_fill = KRColor.copy_color_with_alpha(
      colors.trough_fill, 0.666);
  colors.handle_stroke = KRColor.copy_color_with_alpha(colors.handle_fill, 1.0);
  return colors;
}

Mixer.prototype.make_crossfade = function(parent_view, path) {
  var colors = this.random_colors();
  var margin = 10
  var xfade_h = 50;
  var xfade_y = 10;
  var xfade_x = 10;
  var path_w = 200;
  var paths = { bus: path };
  var xfade_model = new KRCrossfadeModel(0, paths,
      "XFADE", [],
      {
        sender: this.unit,
        range: [1.0, -1.0],
        knee: { range: 0, move_fraction: 0 },
        initial_value: 0,
        tick_skip: 0.50,
        precision: 1
      });
  var xfade_view = new KRSliderHorizontalView(parent_view,
   xfade_model, this.context,
   new KRBoundBox(xfade_x, xfade_y,
   path_w - margin, xfade_h), colors);
  return xfade_view;
}

function MixerPathView(parent_view, crate, context, bbox, colors) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.active = false;
  this.colors = colors;
  this.path = crate.path;
  this.parent_view = parent_view;
  this.unit = this.parent_view.unit;
  this.full_size = [200, 800];
  this.scale = this.parent_view.scale;
  this.context = context;
  this.crate = crate;
  this.damaged = true;
  this.resize_needed = true;
  this.mouse_over = false;
  var parts = this.path.split('/');
  parts.shift();
  parts.shift();
  this.name = parts.join('/');
  this.subviews = [];
  this.background = new KRRectangleView(this, crate, context,
   new KRBoundBox(0, 0, 1, 1), colors);
  this.add_view(this.background);
  this.label = new KRLabelView(this, this.name, context,
   new KRBoundBox(0, 0, 1, 1), colors, 'V', {'rounded': true});
  this.add_view(this.label);
  this.eq_border = 0;
  this.eq_view = new EQView(this, kr.eq, this.path, this.name, context,
   new KRBoundBox(0, 0, 1, 1), colors, this.eq_border);
  this.add_view(this.eq_view);
  var nchannels = 2;
  if (crate.data.channels === "mono") {
    nchannels = 1;
  }
  var meter_model = new KRPeakMeterModel(this.path, this.name, nchannels);
  this.meter = new KRPeakMeterMinimalView(this, meter_model, this.name, context,
   new KRBoundBox(0, 0, 1, 1), colors);
  this.add_view(this.meter);
  var blend_model = new KRSliderModel(0,
   this.path, "BLEND", ['/afx/0/analog/blend', '/afx/1/analog/blend'],
   {
     sender: this.parent_view.unit,
     range: [10, -10],
     tick_skip: 5,
     knee: { range: 0, move_fraction: 0 },
     initial_value: crate.data.afx[0].analog.blend,
   })
  this.blend = new KRSliderVerticalGradientsView(this, blend_model, context,
   new KRBoundBox(0,0,1,1), colors);
  this.add_view(this.blend);
  var drive_model = new KRSliderModel(0,
   this.path, "DRIVE", ['/afx/0/analog/drive', '/afx/1/analog/drive'],
   {
     sender: this.parent_view.unit,
     range: [10, 0],
     tick_skip: 2.5,
     knee: { range: 0, move_fraction: 0 },
     initial_value: crate.data.afx[0].analog.drive,
   })
  this.drive = new KRSliderVerticalGradientsView(this, drive_model, context,
   new KRBoundBox(0,0,1,1), colors);
  this.add_view(this.drive);
   var gain_model = new KRSliderModel(0,
   this.path, "GAIN", ['/afx/0/volume/gain', '/afx/1/volume/gain'],
   {
     sender: this.parent_view.unit,
     range: [24.0, -24.0],
     tick_skip: 12,
     knee: { range: 0, move_fraction: 0 },
     initial_value: crate.data.afx[0].volume.gain,
   })
  this.gain = new KRSliderVerticalGradientsView(this, gain_model, context,
   new KRBoundBox(0,0,1,1), colors);
  this.add_view(this.gain);
  var fader_model = new KRSliderModel(0,
   this.path, "FADER", ['/afx/0/volume/fader', '/afx/1/volume/fader'],
   {
     sender: this.parent_view.unit,
     range: [0.0, -120],
     knee: { range: 120, move_fraction: 1.0 },
     initial_value: crate.data.afx[0].volume.fader,
   });
  this.fader = new KRSliderVerticalGradientsView(this, fader_model,
   context, new KRBoundBox(0, 0, 1, 1), colors);
  this.add_view(this.fader);
  this.c_pressed = false;
  //this.layout(bbox);
  document.body.addEventListener('keydown',
   create_handler(this, this.on_keydown));
  document.body.addEventListener('keyup',
   create_handler(this, this.on_keyup));
}

MixerPathView.prototype = Object.create(KRView.prototype);
MixerPathView.prototype.constructor = MixerPathView;

MixerPathView.prototype.on_keydown = function(evt) {
  //if (!this.active) return;
  //var coords = this.event_to_canvas(evt);
  var key = evt.key || evt.keyIdentifier;
  if (this.mouse_over) {
    if (key == "h" || key == "U+0048") {
      this.h_pressed = true;
    }
    if (key == "s" || key == "U+0053") {
      this.s_pressed = true;
    }
    if (key == "v" || key == "U+0056") {
      this.v_pressed = true;
    }
  }
}

MixerPathView.prototype.on_keyup = function(evt) {
  //if (!this.active) return;
  var key = evt.key || evt.keyIdentifier;
  if (key == "h" || key == "U+0048") {
   this.h_pressed = false;
  }
   if (key == "s" || key == "U+0053") {
   this.s_pressed = false;
  }
  if (key == "v" || key == "U+0056") {
    this.v_pressed = false;
  }
}

MixerPathView.prototype.layout = function(bbox) {
  if (this.bbox !== undefined) {
    this.context.clearRect(this.bbox.x, this.bbox.y,
     this.bbox.w, this.bbox.h);
  }
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.parent_view.scale);
  this.rel_size = this.BBOX.w/this.full_size[0];
  var margin = 10;
  var w = this.BBOX.w / 2.5;
  var x = this.BBOX.x;
  var y = this.BBOX.y;
  var h = this.BBOX.h;
  var path_w = this.BBOX.w;
  this.background.set_bbox(bbox);
  this.label.set_bbox(new KRBoundBox(this.BBOX.x, y, w/2, h));
  var aspect = kr.eq.FULL_SCALE[1]/kr.eq.FULL_SCALE[0];
  var eq_w = 2 * w;
  var eq_h = aspect * eq_w;
  this.eq_view.set_bbox(new KRBoundBox(x + w/2, y, eq_w, eq_h));
  var meter_h = 0.55 * (h - eq_h);
  var gain_h = 0.45 * (h - eq_h);
  var blend_h = gain_h;
  var drive_h = gain_h;
  var fader_h = meter_h;
  this.meter.set_bbox(new KRBoundBox(x + w/2 + w, y + eq_h + gain_h,
   w, meter_h));
  this.gain.set_bbox(new KRBoundBox(x + w/2, y + eq_h, w, gain_h));
  this.fader.set_bbox(new KRBoundBox(x + w/2, y + eq_h + gain_h,
   w, fader_h));
  this.drive.set_bbox(new KRBoundBox(x + 3 * w / 2, y + eq_h, w/2, drive_h));
  this.blend.set_bbox(new KRBoundBox(x + 2 * w, y + eq_h, w/2, drive_h));
  this.active = true;
  this.resize_needed = true;
  this.damage();
}

MixerPathView.prototype.on_touchstart = function(evt) {
  if (!this.active) return;
  var touches = evt.changedTouches;
  for (var t = 0; t < touches.length; t++) {
    var touch = touches[t];
    for (var i = 0; i < this.subviews.length; i++) {
      var v = this.subviews[i]
      var coords = this.event_to_canvas(touch);
      if (v.bbox.in(coords)) {
        if (v.on_touchstart) {
          v.on_touchstart(evt);
        }
      }
    }
  }
}

MixerPathView.prototype.on_touchend = function(evt) {
  if (!this.active) return;
  var touches = evt.changedTouches;
  for (var t = 0; t < touches.length; t++) {
    var touch = touches[t];
    for (var i = 0; i < this.subviews.length; i++) {
      var v = this.subviews[i];
      if (v.on_touchend) {
        v.on_touchend(evt);
      }
    }
  }
}

MixerPathView.prototype.on_touchmove = function(evt) {
  if (!this.active) return;
  var touches = evt.changedTouches;
  for (var t = 0; t < touches.length; t++) {
    var touch = touches[t];
    for (var i = 0; i < this.subviews.length; i++) {
      var v = this.subviews[i];
      if (v.on_touchmove) {
        v.on_touchmove(evt);
      }
    }
  }
}

MixerPathView.prototype.on_touchcancel = function(evt) {
  if (!this.active) return;
  for (var i = 0; i < this.subviews.length; i++) {
    var v = this.subviews[i];
    if (v.on_touchcancel) {
      v.on_touchcancel(evt);
    }
  }
}

MixerPathView.prototype.on_mousedown = function(evt) {
  if (!this.active) return;
  for (var i = 0; i < this.subviews.length; i++) {
    var v = this.subviews[i];
    if (v.on_mousedown) {
      v.on_mousedown(evt);
      if (v.damaged) {
        this.damaged = true;
      }
    }
  }
}

MixerPathView.prototype.on_mouseup = function(evt) {
  if (!this.active) return;
  for (var i = 0; i < this.subviews.length; i++) {
    var v = this.subviews[i];
    if (v.on_mouseup) {
      v.on_mouseup(evt);
      if (v.damaged) {
        this.damaged = true;
      }
    }
  }
}

MixerPathView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

MixerPathView.prototype.set_colors = function(colors) {
  KRView.prototype.set_colors.call(this, colors);
  this.backup_colors = Mixer.colors_from_color(colors.handle_fill);
  this.highlight_colors = Mixer.colors_from_color(colors.handle_fill.lighten().lighten());
}

MixerPathView.prototype.on_mousemove = function(evt) {
  if (!this.active || !this.is_visible) return;
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    if (this.mouse_over === false) {
      this.mouse_over = true;
    }
  } else {
    if (this.mouse_over === true) {
      this.mouse_over = false;
    }
  }
  for (var i = 0; i < this.subviews.length; i++) {
    var v = this.subviews[i];
    if (v.on_mousemove) {
      v.on_mousemove(evt);
      if (v.damaged) {
        this.damaged = true;
      }
    }
  }
}

MixerPathView.prototype.change_hue = function(del) {
  var colors = {};
  colors.trough_stroke = this.colors.trough_stroke.change_hue(del);
  colors.trough_fill = this.colors.trough_fill.change_hue(del);
  colors.handle_stroke = this.colors.handle_stroke.change_hue(del);
  colors.handle_fill = this.colors.handle_fill.change_hue(del);
  this.set_colors(colors);
}

MixerPathView.prototype.change_s = function(del) {
  var colors = {};
  colors.trough_stroke = this.colors.trough_stroke.change_s(del);
  colors.trough_fill = this.colors.trough_fill.change_s(del);
  colors.handle_stroke = this.colors.handle_stroke.change_s(del);
  colors.handle_fill = this.colors.handle_fill.change_s(del);
  this.set_colors(colors);
}

MixerPathView.prototype.change_v = function(del) {
  var colors = {};
  colors.trough_stroke = this.colors.trough_stroke.change_v(del);
  colors.trough_fill = this.colors.trough_fill.change_v(del);
  colors.handle_stroke = this.colors.handle_stroke.change_v(del);
  colors.handle_fill = this.colors.handle_fill.change_v(del);
  this.set_colors(colors);
}

MixerPathView.prototype.my_on_mousescroll = function(evt) {
  if (!this.active) return;
  var evt=window.event || evt;
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    var delta=evt.detail? evt.detail : -evt.wheelDelta;
    var del = 0.003;
    del = delta > 0 ? del : -del;
    if (this.h_pressed) {
      this.change_hue(-4 * del);
      return true;
    } else if (this.v_pressed) {
      this.change_v(-2 * del);
      return true;
    } else if (this.s_pressed) {
      this.change_s(-4 * del);
      return true;
    }
  }
  return false;
}

MixerPathView.prototype.on_mousescroll = function(evt) {
  if (!this.active) return;
  if (this.my_on_mousescroll(evt)) {
    evt.preventDefault();
  } else {
    for (var i = 0; i < this.subviews.length; i++) {
      var v = this.subviews[i];
      if (v.on_mousescroll) {
        v.on_mousescroll(evt);
        if (v.damaged) {
          evt.preventDefault();
          this.damaged = true;
        }
      }
    }
  }
}

MixerPathView.prototype.resize = function(scale) {
  if (!this.active) return;
  this.scale = scale;
  this.bbox = this.BBOX.scale(scale);
  for (var i = 0; i < this.subviews.length; i++) {
    var v = this.subviews[i];
    if (v.resize) v.resize(scale);
  }
  this.background.damage();
}

/*MixerPathView.prototype.render = function() {
  if (!this.active) {
    this.damage();
    return;
  } else if (this.damaged) {
    for (var i = 0, len = this.subviews.length; i < len; i++) {
      var v = this.subviews[i];
      v.damage();
      v.render();
    }
    this.context.restore();
    this.context.restore();
    this.damaged = false;
  }
}*/

function MixerPathCrossfadeBusView(parent_view, crate, context, bbox, colors) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.active = false;
  this.is_crossfade = true;
  this.colors = colors;
  this.backup_colors = Mixer.colors_from_color(colors.handle_fill);
  this.highlight_colors = Mixer.colors_from_color(colors.handle_fill.lighten().lighten());
  this.path = crate.path;
  this.parent_view = parent_view;
  this.full_size = [200, 800];
  this.scale = this.parent_view.scale;
  this.unit = this.parent_view.unit;
  this.context = context;
  this.crate = crate;
  this.damaged = true;
  this.resize_needed = true;
  var parts = this.path.split('/');
  parts.shift();
  parts.shift();
  this.name = parts.join('/');
  this.subviews = [];
  var bg_shape = new KRShape(new KRBoundBox(0,0,1,1),
   new KRStroke(colors.trough_stroke.copy(), 0, 'round'),
   new KRFill(colors.trough_fill.copy()));
  this.background = new KRRectangleView(this, crate, context,
   new KRBoundBox(0, 0, 1, 1), colors);
  this.background.z = -1;
  this.add_view(this.background);
  this.label = new KRLabelView(this, this.name, context,
   new KRBoundBox(0, 0, 1, 1), colors, 'H', {'rounded': true});
  this.add_view(this.label);
  this.xfade_view = this.parent_view.make_crossfade(this, crate.path);
  this.add_view(this.xfade_view);
  this.eq_border = 0;
  this.eq_view = new EQView(this, kr.eq, this.path, this.name, context,
   new KRBoundBox(0, 0, 1, 1), colors, this.eq_border);
  this.add_view(this.eq_view);
  var nchannels = 2;
  if (crate.data.channels === "mono") {
    nchannels = 1;
  }
  var meter_model = new KRPeakMeterModel(this.path, this.name, nchannels);
  this.meter = new KRPeakMeterMinimalView(this, meter_model, this.name, context,
  new KRBoundBox(0, 0, 1, 1), colors);
  this.add_view(this.meter);
  var gain_model = new KRSliderModel(0,
   this.path, "GAIN", ['/afx/0/volume/gain', '/afx/1/volume/gain'],
   {
     sender: this.parent_view.unit,
     range: [24.0, -24.0],
     tick_skip: 12,
     knee: { range: 0, move_fraction: 0 },
     initial_value: crate.data.afx[0].volume.gain,
   })
  this.gain = new KRSliderHorizontalView(this, gain_model, context,
   new KRBoundBox(0,0,1,1), colors);
  this.add_view(this.gain);
  var fader_model = new KRSliderModel(0,
   this.path, "FADER", ['/afx/0/volume/fader', '/afx/1/volume/fader'],
   {
     sender: this.parent_view.unit,
     range: [0.0, -120],
     tick_skip: 3,
     knee: { range: 120, move_fraction: 1.0 },
     initial_value: crate.data.afx[0].volume.fader,
   });
  this.fader = new KRSliderVerticalGradientsView(this, fader_model,
   context, new KRBoundBox(0, 0, 1, 1), colors);
  this.fader.z = 2;
  this.add_view(this.fader);
  this.c_pressed = false;
  document.body.addEventListener('keydown',
   create_handler(this, this.on_keydown));
  document.body.addEventListener('keyup',
   create_handler(this, this.on_keyup));
}

MixerPathCrossfadeBusView.prototype = Object.create(MixerPathView.prototype);
MixerPathCrossfadeBusView.prototype.constructor = MixerPathCrossfadeBusView;

MixerPathCrossfadeBusView.prototype.layout = function(bbox) {
  if (this.bbox !== undefined) {
    this.context.clearRect(this.bbox.x,this.bbox.y,
        this.bbox.w, this.bbox.h);
  }
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.parent_view.scale);
  this.rel_size = this.BBOX.w/this.full_size[0];
  var w = this.BBOX.w / 4.0;
  var x = this.BBOX.x;
  var y_s = this.BBOX.y;
  var h = this.BBOX.h;
  var label_h = this.BBOX.w / 3.0;
  var path_w = this.BBOX.w;
  var xfade_h = 2 * w;
  var y;
  this.background.set_bbox(bbox);
  y = y_s;
  this.label.set_bbox(new KRBoundBox(x, y, path_w, label_h));
  y = y_s + label_h;
  this.xfade_view.set_bbox(new KRBoundBox(x, y, path_w, xfade_h));
  var aspect = kr.eq.FULL_SCALE[1]/kr.eq.FULL_SCALE[0];
  var inner_w = path_w;
  var inner_h = aspect * inner_w;
  var eq_w = inner_w;
  var eq_h = inner_h;
  y = y_s + label_h + xfade_h;
  this.eq_view.set_bbox(new KRBoundBox(x, y, eq_w, eq_h));
  var gain_h = 2 * w;
  var meter_h = (h - label_h - eq_h - gain_h - xfade_h);
  var fader_h = meter_h;
  y = y_s + label_h + xfade_h + eq_h + gain_h;
  this.meter.set_bbox(new KRBoundBox(x + 2 * w, y, 2 * w, meter_h));
  y = y_s + label_h + xfade_h + eq_h;
  this.gain.set_bbox(new KRBoundBox(x, y, eq_w, gain_h));
  y = y_s + label_h + xfade_h + eq_h + gain_h;
  this.fader.set_bbox(new KRBoundBox(x, y, 2 * w, fader_h));
  this.active = true;
  this.resize_needed = true;
  this.damage();
}

function MixerPathGainFaderView(parent_view, crate, context, bbox, colors) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.active = false;
  this.colors = colors;
  this.backup_colors = Mixer.colors_from_color(colors.handle_fill);
  this.highlight_colors = Mixer.colors_from_color(colors.handle_fill.lighten().lighten());
  this.path = crate.path;
  this.parent_view = parent_view;
  this.full_size = [200, 800];
  this.scale = this.parent_view.scale;
  this.unit = this.parent_view.unit;
  this.context = context;
  this.crate = crate;
  this.damaged = true;
  this.resize_needed = true;
  var parts = this.path.split('/');
  parts.shift();
  parts.shift();
  this.name = parts.join('/');
  this.subviews = [];
  var bg_shape = new KRShape(new KRBoundBox(0,0,1,1),
   new KRStroke(colors.trough_stroke.copy(), 0, 'round'),
   new KRFill(colors.trough_fill.copy()));
  this.background = new KRRectangleView(this, crate, context,
   new KRBoundBox(0, 0, 1, 1), colors);
  this.subviews[this.subviews.length] = this.background;
  this.label = new KRLabelView(this, this.name, context,
   new KRBoundBox(0, 0, 1, 1), colors, 'H', {'rounded': true});
  this.subviews[this.subviews.length] = this.label;
  this.eq_border = 0;
  this.eq_view = new EQView(this, kr.eq, this.path, this.name, context,
      new KRBoundBox(0, 0, 1, 1), colors, this.eq_border);
  this.subviews[this.subviews.length] = this.eq_view;
  var nchannels = 2;
  if (crate.data.channels === "mono") {
    nchannels = 1;
  }
  var meter_model = new KRPeakMeterModel(this.path, this.name, nchannels);
  this.meter = new KRPeakMeterMinimalView(this, meter_model, this.name, context,
  new KRBoundBox(0, 0, 1, 1), colors);
  this.subviews[this.subviews.length] = this.meter;
  var gain_model = new KRSliderModel(0,
      this.path, "GAIN", ['/afx/0/volume/gain', '/afx/1/volume/gain'],
      {
        sender: this.parent_view.unit,
        range: [24.0, -24.0],
        tick_skip: 12,
        knee: { range: 0, move_fraction: 0 },
        initial_value: crate.data.afx[0].volume.gain,
      })
  this.gain = new KRSliderHorizontalView(this, gain_model, context,
      new KRBoundBox(0,0,1,1), colors);
  this.subviews[this.subviews.length] = this.gain;
  var fader_model = new KRSliderModel(0,
      this.path, "FADER", ['/afx/0/volume/fader', '/afx/1/volume/fader'],
      {
        sender: this.unit,
        range: [0.0, -120],
        tick_skip: 3,
        knee: { range: 120, move_fraction: 1.0 },
        initial_value: crate.data.afx[0].volume.fader,
      });
  this.fader = new KRSliderVerticalGradientsView(this, fader_model,
      context, new KRBoundBox(0, 0, 1, 1), colors);
  this.subviews[this.subviews.length] = this.fader;
  this.c_pressed = false;
  document.body.addEventListener('keydown',
      create_handler(this, this.on_keydown));
  document.body.addEventListener('keyup',
      create_handler(this, this.on_keyup));
}

MixerPathGainFaderView.prototype = Object.create(MixerPathView.prototype);
MixerPathGainFaderView.prototype.constructor = MixerPathGainFaderView;

MixerPathGainFaderView.prototype.layout = function(bbox, rot) {
  if (this.bbox !== undefined) {
    this.context.clearRect(this.bbox.x, this.bbox.y,
    this.bbox.w,this.bbox.h);
  }
  this.rot = rot;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.parent_view.scale);
  this.rel_size = this.BBOX.w/this.full_size[0];
  var w = this.BBOX.w / 4.0;
  var x = this.BBOX.x;
  var y_s = this.BBOX.y;
  var h = this.BBOX.h;
  var label_h = this.BBOX.w / 3.0;
  var path_w = this.BBOX.w;
  var y;
  var padding = 0;
  y = y_s;
  this.background.set_bbox(bbox);
  this.label.set_bbox(new KRBoundBox(x + padding, y + padding, path_w - 2* padding,
   label_h - 2 * padding), rot);
  var aspect = kr.eq.FULL_SCALE[1]/kr.eq.FULL_SCALE[0];
  var eq_w = path_w;
  var eq_h = eq_w * aspect;
  y = y_s + label_h;
  this.eq_view.set_bbox(new KRBoundBox(x, y, eq_w, eq_h));
  var gain_h = 2 * w;
  var meter_h = (h - label_h - eq_h - gain_h);
  var fader_h = meter_h;
  y = y_s + label_h + eq_h + gain_h;
  this.meter.set_bbox(new KRBoundBox(x + 2 * w, y, 2 * w, meter_h));
  y = y_s + label_h + eq_h;
  this.gain.set_bbox(new KRBoundBox(x, y, eq_w, gain_h));
  y = y_s + label_h + eq_h + gain_h;
  this.fader.set_bbox(new KRBoundBox(x, y, 2 * w, fader_h));
  this.active = true;
  this.resize_needed = true;
  this.damage();
}

EQView = function(parent_view, eq_model, path, name, context, bbox, colors,
 border_width) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.active = false;
  this.parent_view = parent_view;
  this.scale = this.parent_view.scale;
  this.eq = eq_model;
  this.eq.add_view(this);
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.path = path;
  this.name = name;
  this.colors = colors;
  this.context = context;
  var fill = new KRFill(this.colors.trough_fill);
  this.background = new KRRectangleView(this, null, context, bbox, colors,
   {'rounded': true});
  this.background.set_setting_mode('OUTSET');
  this.background.z = -1;
  this.bg_canvas = document.createElement('canvas');
  this.bg_context = this.bg_canvas.getContext('2d');
  this.fg_canvas = document.createElement('canvas');
  this.fg_context = this.fg_canvas.getContext('2d');
  this.label = "EQ";
  this.resize(this.scale);
}

EQView.prototype = Object.create(KRView.prototype);
EQView.prototype.constructor = EQView;

EQView.prototype.set_colors = function(colors) {
  this.colors = {};
  this.colors['bg_fill'] = colors.trough_fill;
  this.colors['bg_stroke'] = colors.trough_stroke;
  this.colors['trough_fill'] = colors.trough_fill.darken().darken();
  this.colors['trough_stroke'] = colors.trough_stroke;
  this.background.set_colors(this.colors);
  this.damaged = true;
  this.bg_damaged = true;
  this.parent_view.damage();
}

EQView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  var pad = this.parent_view.rel_size * 4 * SHADOW_LENGTH;
  var w = this.BBOX.w - 2 * pad;
  var new_bbox = new KRBoundBox(pad + this.BBOX.x, pad + this.BBOX.y,
   w, this.BBOX.h - 2 * pad);
  this.background.set_bbox(new_bbox);
  this.active = true;
  this.resize(this.parent_view.scale);
}

EQView.prototype.render_label = function() {
  //return;
  if (!this.active) return;
  this.context.save();
  this.context.font = 'normal ' + 0.5 * Math.round(this.bbox.h)
   + 'pt play';
  this.context.textBaseline = 'middle';
  this.context.textAlign = 'center';
  var w = this.context.measureText(this.label).width;
  var h = parseInt(this.context.font, 10);
  var fit = true;
  while (w > this.background.rect.shape.bbox.w
   - 2 * this.background.rect.shape.stroke.w) {
    h--;
    if (h <= 0) {
      fit = false;
      break;
    }
    this.context.font = 'normal ' + h + 'pt play';
    w = this.context.measureText(this.label).width;
  }
  if (fit) {
    this.fill_text_inset(this.label, this.colors.trough_fill,
     this.background.rect.shape.bbox.x + 0.5 * this.background.rect.shape.bbox.w,
     this.background.rect.shape.bbox.y + 0.5 * this.background.rect.shape.bbox.h);
  }
  this.context.restore();
}

EQView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

EQView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    this.mdown = true;
  }
}

EQView.prototype.on_mouseup = function(evt) {
  if (this.mdown) {
    var coords = this.event_to_canvas(evt);
    if (this.bbox.in(coords)) {
      if (kr.ux.has_page("EQ")) {
        kr.ux.switch_pages("EQ");
        this.eq.select_path(this.path);
      }
    }
  }
  this.mdown = false;
}

EQView.prototype.on_mousescroll = function(evt) {
  var evt=window.event || evt;
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    evt.preventDefault();
    var delta=evt.detail? -32 * evt.detail : evt.wheelDelta;
    var delx = delta > 0 ? 3 : -3;
    this.eq.zoom_range(delx);
  }
}


EQView.prototype.resize = function(scale) {
  if (scale === undefined) scale = this.parent_view.scale;
  this.scale = scale;
  this.dpr = window.devicePixelRatio;
  this.bbox = this.BBOX.scale(scale);
  this.background.resize(scale, this.parent_view.rel_size);
  this.fg_canvas.style.width = this.background.rect.shape.bbox.w + 'px';
  this.fg_canvas.style.height = this.background.rect.shape.bbox.h + 'px';
  this.fg_canvas.width = this.dpr * this.background.rect.shape.bbox.w;
  this.fg_canvas.height = this.dpr * this.background.rect.shape.bbox.h;
  this.fg_context.scale(this.dpr, this.dpr);
  this.bg_canvas.style.width = this.bbox.w + 'px';
  this.bg_canvas.style.height = this.bbox.h + 'px';
  this.bg_canvas.width = this.dpr * this.bbox.w;
  this.bg_canvas.height = this.dpr * this.bbox.h;
  this.bg_context.scale(this.dpr, this.dpr);
  this.bg_damaged = true;
  this.fg_damaged = true;
  this.damaged = true;
}

EQView.prototype.render_background = function() {
  if (!this.active) return;
  this.background.damage();
  this.context.save();
  this.context.fillStyle = this.colors.bg_fill.style();
  this.context.fillRect(this.bbox.x, this.bbox.y, this.bbox.w, this.bbox.h);
  this.context.restore();
  this.background.render();
  this.render_label();
  this.damaged = true;
}

EQView.prototype.render_foreground = function() {
  if (!this.active) return;
  this.eq.render_path_to_canvas(this.name, this.fg_canvas);
  this.fg_damaged = false;
  this.damaged = true;
  this.context.save();
  this.background.rect.clip(this.context, true);
  this.context.drawImage(this.fg_canvas,
   this.background.rect.shape.bbox.x,
   this.background.rect.shape.bbox.y,
   this.background.rect.shape.bbox.w,
   this.background.rect.shape.bbox.h);
  this.context.restore();
}

EQView.prototype.damage = function() {
  this.damaged = true;
  this.fg_damaged = true;
  this.parent_view.damage();
}

EQView.prototype.render = function() {
  if (!this.active) return;
  if (this.damaged) {
    this.render_background();
    this.render_foreground();
    this.damaged = false;
  }
}

