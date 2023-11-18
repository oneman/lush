$(document).ready(function() {
  rack_units.push({'constructor': Eq});
});

var MAX_BANDS = 32;

function sinh(x){
  return (Math.exp(x) - Math.exp(-x)) / 2;
}

Math.sinh = Math.sinh || sinh;

CanvasRenderingContext2D.prototype.dashedLine = function
 (x1, y1, x2, y2, dashLen) {
  if (dashLen == undefined) dashLen = 2;
  dashLen = dashLen / window.devicePixelRatio;
  this.moveTo(x1, y1);
  var dX = x2 - x1;
  var dY = y2 - y1;
  var dashes = Math.floor(Math.sqrt(dX * dX + dY * dY) / dashLen);
  var dashX = dX / dashes;
  var dashY = dY / dashes;
  var q = 0;
  while (q++ < dashes) {
    x1 += dashX;
    y1 += dashY;
    if (q % 2 == 0) {
      this.moveTo(x1, y1);
    } else {
      this.lineTo(x1, y1);
    }
  }
  if (q % 2 == 0) {
    this.moveTo(x2, y2);
  } else {
    this.lineTo(x2, y2);
  }
};

function Eq(info_object) {
  this.title = "Eq";
  this.FULL_SCALE = [1420.0, 840.0];
  this.description = "Control the Eq.";
  this.id = "kr_eq";
  this.div = "<div class='RCU' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  this.paths = {};
  this.patched_paths = {};
  this.selected_path = null;
  this.clicked_band = null;
  info_object['parent_div'].append(this.div);
  $(this.sel).append('\
    <div class="eq" id="eq_' + this.id + '">\
      <div class="kr_eq_list offwhite">\
        <div class="kr_list select_list play" id="eq_path_list"></div>\
      </div>\
      <div class="eq_canvas_container" id="eq_canvas_container">\
        <canvas class="eq_canvas" id="eq_grid_canvas"></canvas>\
        <canvas class="eq_canvas" id="eq_canvas_' + this.id + '"></canvas>\
      </div>\
    </div>');
  this.path_list = new Selectable(document.getElementById('eq_path_list'));
  this.canvas_container = document.getElementById('eq_canvas_container');
  this.canvas = document.getElementById("eq_canvas_" + this.id);
  this.grid_canvas = document.getElementById("eq_grid_canvas");
  kr.eq = this;
  this.grid_line_width = this.GRID_LINE_WIDTH = 1;
  this.line_width = this.LINE_WIDTH = 2;
  this.handle_radius = this.HANDLE_RADIUS = 12;
  this.bw_handle_width = this.BW_HANDLE_WIDTH = 6;
  this.bw_handle_height = this.BW_HANDLE_HEIGHT = 30;
  this.FONT_SIZE = 14;
  this.hz_max = 20000;
  this.hz_min = 20;
  this.db_max = 12;
  this.db_min = -12;
  this.npoints = 200;
  this.nextra = 4;
  this.nex_extra = 4;
  this.label_padding = this.LABEL_PADDING = 80;
  this.font = 'normal 18pt play';
  this.label_color = '#f0f0f0';
  this.views = [];
  this.peak_color_scheme = this.color_scheme
   = new KRMonoColorScheme(MIXER_BG_COLOR);
  this.handle_color_scheme = new KRMonoColorScheme(EQ_HANDLE_COLOR);
  this.canvas.addEventListener('mousedown',
   create_handler(this, this.on_handle_mousedown));
  window.addEventListener('mouseup',
   create_handler(this, this.on_handle_mouseup));
  window.addEventListener('mousemove',
   create_handler(this, this.on_handle_mousemove));
  var mouse_wheel_evt=(/Firefox/i.test(navigator.userAgent)) ?
   "DOMMouseScroll" : "mousewheel";
  this.canvas.addEventListener(mouse_wheel_evt,
   create_handler(this, this.on_mousescroll));
  this.resize_needed = true;
  (function animloop() {
    requestAnimFrame(animloop);
    kr.eq.redraw();
  })();
}

Eq.prototype.add_view = function(view) {
  this.views.push(view);
}

Eq.prototype.remove_view = function(view) {
  if (this.views.indexOf(view) > -1) {
    this.views.splice(this.views.indexOf(view), 1);
  }
}

Eq.prototype.damage = function() {
  this.damaged = true;
}

Eq.prototype.zoom_range = function(del) {
  var new_pos = Math.min(60, (Math.max(3, this.db_max + del)));
  this.db_max = new_pos;
  this.db_min = -this.db_max;
  this.damaged = true;
  this.resize_needed = true;
  for (var i = 0, len = this.views.length; i < len; i++) {
    var v = this.views[i];
    v.damage();
  }
}

Eq.prototype.on_mousescroll = function(evt) {
  var evt=window.event || evt;
  var coords = this.event_to_canvas(evt);
  var delta=evt.detail? -32 * evt.detail : evt.wheelDelta;
  evt.preventDefault();
  var delx = delta > 0 ? 3 : -3;
  this.zoom_range(delx);
}

Eq.prototype.select_path = function(path_name) {
  if (typeof this.paths[path_name] != 'undefined') {
    this.paths[path_name].select();
  }
}

Eq.prototype.resize_canvas = function(elem, canvas) {
  var canvases = canvas !== undefined ? [canvas] :
   [this.canvas, this.grid_canvas];
  var w = parseInt(getComputedStyle(elem).getPropertyValue('width'), 10);
  var h = parseInt(getComputedStyle(elem).getPropertyValue('height'), 10);
  if (isNaN(w)) {
    w = parseInt(elem.style.width, 10);
  }
  if (isNaN(h)) {
    h = parseInt(elem.style.height, 10);
  }
  this.dpr = window.devicePixelRatio;
  for (var i = 0; i < canvases.length; i++) {
    var c = canvases[i];
    if (w == 0) {
      c.style.width = '100%';
    } else {
      c.style.width = w + 'px';
    }
    if (h == 0) {
      c.style.height = '100%';
    } else {
      c.style.height = h + 'px';
    }
    c.width = w * this.dpr;
    c.height = h * this.dpr;
    var gc = c.getContext("2d");
    gc.scale(this.dpr, this.dpr);
  }
}

Eq.prototype.set_canvas_sizes = function(canvas) {
  this.dpr = window.devicePixelRatio;
  var w = parseInt(canvas.style.width, 10);
  var h = parseInt(canvas.style.height, 10);
  this.canvas_height = h;
  this.canvas_width = w;
  var x_r = parseInt(canvas.style.width, 10) / this.FULL_SCALE[0];
  var y_r = parseInt(canvas.style.height, 10) / this.FULL_SCALE[1];
  this.scale = x_r;
  this.label_padding = x_r * this.LABEL_PADDING;
  this.min_x = this.hz_to_canvas(this.hz_min);
  this.max_x = this.hz_to_canvas(this.hz_max);
  this.max_y = this.db_to_canvas(this.db_min);
  this.min_y = this.db_to_canvas(this.db_max);
  this.grid_line_width = x_r * this.GRID_LINE_WIDTH;
  this.line_width = x_r * this.LINE_WIDTH;
  this.handle_radius = x_r * this.HANDLE_RADIUS;
  this.bw_handle_width = x_r * this.BW_HANDLE_WIDTH;
  this.bw_handle_height = x_r * this.BW_HANDLE_HEIGHT;
  this.font = 'normal ' + Math.round(x_r * this.FONT_SIZE) +'pt play';
}

Eq.prototype.window_resize = function() {
  this.page_resize();
  for (var key in this.paths) {
    var p = this.paths[key];
    p.meter_container.window_resize();
  }
};

Eq.prototype.event_to_canvas = function(evt) {
  var rect = this.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

Eq.prototype.canvas_to_db = function(y) {
  var db;
  if (y <= this.db_to_canvas(0)) {
    db = this.db_max * (1 - ((y - 0.5 * this.label_padding)
     / (0.5*(this.canvas_height - 1.5 * this.label_padding))));
  } else {
    db = this.db_min * (((y - 0.5 * this.label_padding)
     / (0.5*(this.canvas_height - 1.5 * this.label_padding))) - 1);
  }
  return db;
}

Eq.prototype.db_to_canvas = function(db) {
  if (db >= 0) {
    return 0.5 * this.label_padding + (0.5 * (this.canvas_height - 1.5 * this.label_padding))
     * (1 - (db  / this.db_max));
  } else {
    return 0.5 * this.label_padding + (0.5 * (this.canvas_height - 1.5 * this.label_padding))
     * (1 + (db / this.db_min));
  }
}

Eq.prototype.canvas_to_hz = function(x) {
  return Math.exp(((x - this.label_padding)
   * (Math.log(this.hz_max) - Math.log(this.hz_min))
   / (this.canvas_width - 1.5 * this.label_padding)) + Math.log(this.hz_min));
}

Eq.prototype.hz_to_canvas = function(hz) {
  return this.label_padding + ((this.canvas_width - 1.5 * this.label_padding)
   * (Math.log(hz) - Math.log(this.hz_min))
   / (Math.log(this.hz_max) - Math.log(this.hz_min)));
}

Eq.prototype.db_len_to_canvas_len = function(db_len) {
  return this.db_to_canvas(0) - this.db_to_canvas(db_len)
}

Eq.prototype.draw_hz_hash = function(value, label) {
  var context = this.grid_canvas.getContext('2d');
  context.save();
  context.lineWidth = this.grid_line_width;
  context.strokeStyle = context.fillStyle = this.label_color;
  context.beginPath();
  var pos = 0.5 + Math.floor(this.hz_to_canvas(value));
  context.moveTo(pos, this.db_to_canvas(this.db_max));
  context.lineTo(pos, 1.0 + Math.floor(this.db_to_canvas(this.db_min)));
  context.stroke();
  var text = label;
  if (text != '') {
    context.font = this.font;
    var w = context.measureText(text).width;
    var h = parseInt(context.font, 10);
    context.fillText(text, pos - 0.5 * w,
        this.db_to_canvas(this.db_min) + 0.5 * this.label_padding + 0.5 * h);
  }
  context.restore();
}

Eq.prototype.left_to_hz = function(left) {
  return this.canvas_to_hz(this.left_to_canvas(left) - this.label_padding);
}

Eq.prototype.draw_db_hash = function(value) {
  var context = this.grid_canvas.getContext('2d');
  context.save();
  context.lineWidth = this.grid_line_width;
  context.fillStyle = context.strokeStyle = this.label_color;
  context.beginPath();
  var pos = 0.5 + Math.floor(this.db_to_canvas(value));
  if (value == 0) {
    context.dashedLine(this.hz_to_canvas(this.hz_min), pos,
     this.hz_to_canvas(this.hz_max), pos, 4);
    context.stroke();
  } else {
    context.moveTo(this.hz_to_canvas(this.hz_min), pos);
    context.lineTo(this.hz_to_canvas(this.hz_max), pos);
    context.stroke();
  }
  context.font = this.font;
  var text = value > 0 ? '+' + value : value;
  var w = context.measureText(text).width;
  var h = parseInt(context.font, 10);
  context.textBaseline = 'alphabetic';
  context.fillText(text, 0.5 * (this.label_padding - w),
      pos + 0.5 * h);
  context.restore();
}

Eq.prototype.draw_grid = function() {
  lines = [20, '20', 30, '', 40, '', 50, '50', 60, '', 70, '', 80, '', 90, '',
   100, '100', 200, '200', 300, '', 400, '', 500, '500', 600, '', 700, '', 800,
   '', 900, '', 1000, '1k', 2000, '2k', 3000, '', 4000, '', 5000, '5k',
   6000, '', 7000, '', 8000, '', 9000, '', 10000, '10k', 20000, '20k'];
  while (lines.length) {
    var freq = lines.shift();
    var label = lines.shift();
    this.draw_hz_hash(freq, label);
  }
  var i = this.db_max;
  while (i >= this.db_min) {
    this.draw_db_hash(i);
    i = i - 3;
  }
}

Eq.prototype.clear_canvas = function() {
  this.canvas.getContext("2d").clearRect(0, 0, this.canvas.width / this.dpr,
   this.canvas.height / this.dpr);
}

Eq.prototype.redraw = function() {
  if (this.damaged) {
    this.damaged = false;
    if (this.resize_needed) {
      var container = this.canvas_container;
      this.resize_canvas(container);
      this.set_canvas_sizes(this.canvas);
      this.draw_grid();
      this.resize_needed = false;
      for (var i = 0, len = this.views.length; i < len; i++) {
        var v = this.views[i];
        v.resize();
      }
    } else {
      this.clear_canvas();
    }
    if (this.selected_path != null) {
      this.render_path(this.selected_path);
    }
    for (var key in this.patched_paths) {
      var p = this.patched_paths[key];
      for (var i = 0, len = this.views.length; i < len; i++) {
        var v = this.views[i];
        if (v.path === p.path) {
          v.damage();
        }
      }
      delete p;
    }
  }
}

Eq.prototype.render_path_to_canvas = function(name, canvas) {
  for (var key in this.paths) {
    var p = this.paths[key];
    if (p.name === name) {
      canvas.getContext('2d').clearRect(0, 0, parseInt(canvas.style.width, 10),
       parseInt(canvas.style.height, 10));
      this.set_canvas_sizes(canvas);
      this.render_path(p, canvas);
      this.set_canvas_sizes(this.canvas);
    }
  }
}

Eq.prototype.render_path = function(eq_path, canvas) {
  var range = {min:this.hz_min, max:this.hz_max};
  var context = canvas !== undefined ? canvas.getContext("2d") :
   this.canvas.getContext("2d");
  var lp = eq_path.low_pass;
  var hp = eq_path.high_pass;
  for (var i = 0, len = eq_path.band.length; i < len; i++) {
    var b = eq_path.band[i];
    var clicked = b.is_clicked();
    b.graph(context, range, this.npoints);
  }
  lp.graph(context, range, this.npoints);
  hp.graph(context, range, this.npoints);
  for (var i = 0, len = eq_path.band.length; i < len; i++) {
    var b = eq_path.band[i];
    var clicked = b.is_clicked();
    b.draw_bw_handles(context, clicked);
  }
  for (var i = 0, len = eq_path.band.length; i < len; i++) {
    var b = eq_path.band[i];
    var clicked = b.is_clicked();
    b.draw_handle(context, clicked);
  }
  var clicked = lp.is_clicked();
  lp.draw_handle(context, clicked);
  lp.draw_bw_handles(context, clicked);
  clicked = hp.is_clicked();
  hp.draw_handle(context, clicked);
  hp.draw_bw_handles(context, clicked);
  eq_path.graph_total(context, range, 3 * this.npoints);
}

EqBand.prototype.is_clicked = function() {
  return this.clicked || this.lbw_handle.clicked
   || this.rbw_handle.clicked;
}

Eq.prototype.page_resize = function() {
  this.resize_needed = true;
  this.damaged = true;
}

Eq.prototype.on_new_path = function(path, crate) {
  this.paths[path] = new EqPath(this, path, crate);
}

Eq.prototype.on_del = function(path) {
  if (this.paths[path] != undefined) {
    var eq_path = this.paths[path];
    eq_path.delete();
    delete eq_path;
  }
}

Eq.prototype.visible = function() {
  for (var key in this.paths) {
    var p = this.paths[key];
    p.meter_container.visible();
  }
}

Eq.prototype.hidden = function() {
  for (var key in this.paths) {
    var p = this.paths[key];
    p.meter_container.hidden();
  }
}

Eq.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/mixer/")) return;
  if (crate.meth == "PUT") {
    var path = crate.path;
    this.on_new_path(path, crate);
  } else if (crate.meth == "PATCH") {
    var path = crate.path;
    this.on_patch(path, crate);
  } else if (crate.meth == "DELETE") {
    this.on_del(crate.path);
  }
  for (var key in this.paths) {
    var p = this.paths[key];
    if (p) {
      if (p.meter_container) {
        p.meter_container.update(crate);
      }
    }
  }
}

Eq.prototype.on_patch = function(path, crate) {
  if (this.paths[path]) {
    var eq_path = this.paths[path];
    for (var i = 0, len = crate.data.length; i < len; i++) {
      var info = crate.data[i];
      var p = info.path;
      var op = info.op;
      var val = info.value;
      if (op == 'replace') {
        var names = p.split('/');
        if (names[1] == 'afx' && (names[2] == 0 || names[2] == 1)
         && names[3] == 'pass'
         && names[4] == 'high') {
          var prop = names[5];
          var hp = eq_path.high_pass;
          if (prop == 'hz') {
            hp.set_hz_db(val, 0, true);
            this.patched_paths[path] = eq_path;
          } else if (prop == 'bw') {
            if (Math.abs(hp.bw).toFixed(2) != val) {
              var new_bw_hz = Math.pow(2, val/2) * hp.hz;
              if (new_bw_hz > this.hz_max || new_bw_hz < this.hz_min) {
                hp.set_bw(hp.hz * hp.hz / new_bw_hz, true);
                this.patched_paths[path] = eq_path;
              } else {
                hp.set_bw(new_bw_hz, true);
                this.patched_paths[path] = eq_path;
              }
            }
          }
        } else if (names[1] == 'afx' && (names[2] == 0 || names[2] == 1)
         && names[3] == 'pass'
         && names[4] == 'low') {
          var prop = names[5];
          var lp = eq_path.low_pass;
          if (prop == 'hz') {
            lp.set_hz_db(val, 0, true);
            this.patched_paths[path] = eq_path;
          } else if (prop == 'bw') {
            if (Math.abs(lp.bw).toFixed(2) != val) {
              var new_bw_hz = Math.pow(2, val/2) * lp.hz;
              if (new_bw_hz > this.hz_max || new_bw_hz < this.hz_min) {
                lp.set_bw(lp.hz * lp.hz / new_bw_hz, true);
                this.patched_paths[path] = eq_path;
              } else {
                lp.set_bw(new_bw_hz, true);
                this.patched_paths[path] = eq_path;
              }
            }
          }
        } else if (names[1] == 'afx' && (names[2] == 0 || names[2] == 1)
         && names[3] == 'eq'
         && names[4] == 'band') {
          var band = names[5];
          var prop = names[6];
          var b = eq_path.band[band];
          if (typeof(b) == 'undefined') return;
          if (prop == 'hz') {
            b.set_hz_db(val, b.db, true);
            this.patched_paths[path] = eq_path;
          }
          if (prop == 'db') {
            b.set_hz_db(b.hz, val, true);
            this.patched_paths[path] = eq_path;
          }
          if (prop == 'bw') {
            if (Math.abs(b.bw).toFixed(2) != val) {
              var new_bw_hz = Math.pow(2, val/2) * b.hz;
              if (new_bw_hz > this.hz_max || new_bw_hz < this.hz_min) {
                b.set_bw(b.hz * b.hz / new_bw_hz, true);
                this.patched_paths[path] = eq_path;
              } else {
                b.set_bw(new_bw_hz, true);
                this.patched_paths[path] = eq_path;
              }
            }
          }
        }
      }
    }
  }
}

Eq.prototype.on_handle_mousemove = function(evt) {
  if (this.high_pass_clicked) {
    evt.preventDefault();
    evt.stopPropagation();
    var pass = this.selected_path.high_pass;
    var delx = (1/this.dpr) * (evt.screenX - pass.mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + pass.mdown.x - pass.mdown.off_x));
    pass.set_hz_db(this.canvas_to_hz(new_x), 0);
    return;
  }
  if (this.high_pass_lbw_clicked) {
    evt.preventDefault();
    evt.stopPropagation();
    var pass = this.selected_path.high_pass;
    var delx = (1/this.dpr) * (evt.screenX - pass.lbw_mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + pass.lbw_mdown.x - pass.lbw_mdown.off_x));
    var hz = this.canvas_to_hz(new_x);
    var bw_max_hz = Math.pow(2, -0.33) * pass.hz;
    var bw_min_hz = Math.pow(2, -2) * pass.hz;
    hz = Math.min(bw_max_hz, Math.max(bw_min_hz, hz));
    pass.set_bw(hz);
    return;
  }
  if (this.high_pass_rbw_clicked) {
    evt.preventDefault();
    evt.stopPropagation();
    var pass = this.selected_path.high_pass;
    var delx = (1/this.dpr) * (evt.screenX - pass.rbw_mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + pass.rbw_mdown.x - pass.rbw_mdown.off_x));
    var hz = this.canvas_to_hz(new_x);
    var bw_max_hz = Math.pow(2, 2) * pass.hz;
    var bw_min_hz = Math.pow(2, 0.33) * pass.hz;
    hz = Math.min(bw_max_hz, Math.max(bw_min_hz, hz));
    pass.set_bw(hz);
    return
  }
  if (this.low_pass_clicked) {
    evt.preventDefault();
    evt.stopPropagation();
    var pass = this.selected_path.low_pass;
    var delx = (1/this.dpr) * (evt.screenX - pass.mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + pass.mdown.x - pass.mdown.off_x));
    pass.set_hz_db(this.canvas_to_hz(new_x), 0);
    return;
  }
  if (this.low_pass_lbw_clicked) {
    evt.preventDefault();
    evt.stopPropagation();
    var pass = this.selected_path.low_pass;
    var delx = (1/this.dpr) * (evt.screenX - pass.lbw_mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + pass.lbw_mdown.x - pass.lbw_mdown.off_x));
    var hz = this.canvas_to_hz(new_x);
    var bw_max_hz = Math.pow(2, -0.33) * pass.hz;
    var bw_min_hz = Math.pow(2, -2) * pass.hz;
    hz = Math.min(bw_max_hz, Math.max(bw_min_hz, hz));
    pass.set_bw(hz);
    return;
  }
  if (this.low_pass_rbw_clicked) {
    evt.preventDefault();
    evt.stopPropagation();
    var pass = this.selected_path.low_pass;
    var delx = (1/this.dpr) * (evt.screenX - pass.rbw_mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + pass.rbw_mdown.x - pass.rbw_mdown.off_x));
    var hz = this.canvas_to_hz(new_x);
    var bw_max_hz = Math.pow(2, 2) * pass.hz;
    var bw_min_hz = Math.pow(2, 0.33) * pass.hz;
    hz = Math.min(bw_max_hz, Math.max(bw_min_hz, hz));
    pass.set_bw(hz);
    return
  }
  if (this.clicked_band != null) {
    evt.preventDefault();
    evt.stopPropagation();
    var delx = (1/this.dpr) * (evt.screenX - this.clicked_band.mdown.screen_x);
    var dely = (1/this.dpr) * (evt.screenY - this.clicked_band.mdown.screen_y);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + this.clicked_band.mdown.x - this.clicked_band.mdown.off_x));
    var new_y = Math.min(this.max_y, Math.max(this.min_y,
     this.clicked_band.mdown.y + dely - this.clicked_band.mdown.off_y));
    this.clicked_band.set_hz_db(this.canvas_to_hz(new_x), this.canvas_to_db(new_y));
    return;
  }
  if (this.lbw_clicked_band != null) {
    evt.preventDefault();
    evt.stopPropagation();
    var b = this.lbw_clicked_band;
    var delx = (1/this.dpr) * (evt.screenX - b.lbw_mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + b.lbw_mdown.x - b.lbw_mdown.off_x));
    var hz = this.canvas_to_hz(new_x);
    var bw_max_hz = Math.pow(2, -0.25) * b.hz;
    var bw_min_hz = Math.pow(2, -2) * b.hz;
    hz = Math.min(bw_max_hz, Math.max(bw_min_hz, hz));
    b.set_bw(hz);
    return;
  }
  if (this.rbw_clicked_band != null) {
    evt.preventDefault();
    evt.stopPropagation();
    var b = this.rbw_clicked_band;
    var delx = (1/this.dpr) *  (evt.screenX - b.rbw_mdown.screen_x);
    var new_x = Math.min(this.max_x, Math.max(this.min_x,
     delx + b.rbw_mdown.x - b.rbw_mdown.off_x));
    var hz = this.canvas_to_hz(new_x);
    var bw_max_hz = Math.pow(2, 2) * b.hz;
    var bw_min_hz = Math.pow(2, 0.25) * b.hz;
    hz = Math.min(bw_max_hz, Math.max(bw_min_hz, hz));
    b.set_bw(hz);
    return;
  }
}

Eq.prototype.on_handle_mousedown = function(evt) {
  if (this.selected_path != null) {
    var eq_path = this.selected_path;
    var pass = eq_path.high_pass;
    if (pass.click_in(evt)) {
      if(pass.remove_ready) {
        pass.remove();
        return;
      }
      this.high_pass_clicked = true;
      return;
    }
    pass = eq_path.low_pass;
    if (pass.click_in(evt)) {
      if(pass.remove_ready) {
        pass.remove();
        return;
      }
      this.low_pass_clicked = true;
      return;
    }
    pass = eq_path.high_pass;
    if (pass.lbw_click_in(evt)) {
      this.high_pass_lbw_clicked = true;
      return;
    }
    if (pass.rbw_click_in(evt)) {
      this.high_pass_rbw_clicked = true;
      return;
    }
    pass = eq_path.low_pass;
    if (pass.lbw_click_in(evt)) {
      this.low_pass_lbw_clicked = true;
      return;
    }
    if (pass.rbw_click_in(evt)) {
      this.low_pass_rbw_clicked = true;
      return;
    }
    for (var j = eq_path.band.length - 1; j > -1; j--) {
      var band = eq_path.band[j];
      if (band.click_in(evt)) {
        if (band.remove_ready) {
          band.remove();
          return;
        }
        this.clicked_band = band;
        return;
      }
    }
    for (var j = eq_path.band.length - 1; j > -1; j--) {
      var band = eq_path.band[j];
      if (band.lbw_click_in(evt)) {
        this.lbw_clicked_band = band;
        return;
      }
      if (band.rbw_click_in(evt)) {
        this.rbw_clicked_band = band;
        return;
      }
    }
  }
}

Eq.prototype.on_handle_mouseup = function(evt) {
  if (this.high_pass_clicked) {
    this.selected_path.high_pass.on_handle_mouseup(evt);
    this.high_pass_clicked = false;
  }
  if (this.high_pass_lbw_clicked) {
    this.selected_path.high_pass.on_lbw_mouseup(evt);
    this.high_pass_lbw_clicked = false;
  }
  if (this.high_pass_rbw_clicked) {
    this.selected_path.high_pass.on_rbw_mouseup(evt);
    this.high_pass_rbw_clicked = false;
  }
  if (this.low_pass_clicked) {
    this.selected_path.low_pass.on_handle_mouseup(evt);
    this.low_pass_clicked = false;
  }
  if (this.low_pass_lbw_clicked) {
    this.selected_path.low_pass.on_lbw_mouseup(evt);
    this.low_pass_lbw_clicked = false;
  }
  if (this.low_pass_rbw_clicked) {
    this.selected_path.low_pass.on_rbw_mouseup(evt);
    this.low_pass_rbw_clicked = false;
  }
  if (this.clicked_band) {
    this.clicked_band.on_handle_mouseup(evt);
    this.clicked_band = null;
    this.damaged = true;
  }
  if (this.lbw_clicked_band) {
    this.lbw_clicked_band.on_lbw_mouseup(evt);
    this.lbw_clicked_band = null;
    this.damaged = true;
  }
  if (this.rbw_clicked_band) {
    this.rbw_clicked_band.on_rbw_mouseup(evt);
    this.rbw_clicked_band = null;
    this.damaged = true;
  }
}

function EqPath(rack_unit, path, crate) {
  this.eq = rack_unit;
  this.path = path;
  this.crate = crate;
  var names = path.split("/");
  names.shift();
  names.shift();
  this.name = names.join("/");
  this.list_elem = document.createElement('div');
  this.list_elem_label = document.createElement('div');
  this.list_elem_label.classList.add("label");
  this.list_elem_label.innerHTML = this.name;
  this.list_elem_meter_container = document.createElement('div');
  this.list_elem_meter_container.className = 'small_meter_container';
  this.list_elem.appendChild(this.list_elem_meter_container);
  this.meter_container =
   new CanvasViewContainer(rack_unit, this.list_elem_meter_container,
    this.path + '_little_eq_meter');
  var nchannels = 2;
  if (this.crate.data.channels === "mono") nchannels = 1;
  this.meter_model = new KRPeakMeterModel(this.path, this.name, nchannels);
  this.meter_view = new KRPeakMeterMinimalView(this.meter_container,
   this.meter_model, this.name, this.meter_container.context,
   new KRBoundBox(0, 0, 1, 1),
   CanvasViewContainer.colors_from_color(this.eq.peak_color_scheme.next()));
  this.meter_container.add_view(this.path, this.meter_view);
  this.list_elem_controls = document.createElement('div');
  this.list_elem_controls.classList.add("controls");
  this.band_remove_button = document.createElement('div');
  this.band_remove_button.innerHTML = "<em>REMOVE</em>";
  this.band_remove_button.className = 'red kr_push_button';
  this.band_remove_button.addEventListener('click', create_handler(this,
   this.on_band_remove_click));
  this.band_add_button = document.createElement('div');
  this.band_add_button.innerHTML = "<em>ADD</em>";
  this.band_add_button.className = 'blue kr_push_button';
  this.band_add_button.addEventListener('click', create_handler(this,
   this.on_band_add_click));
  this.list_elem_controls.appendChild(this.band_remove_button);
  this.list_elem_controls.appendChild(this.band_add_button);
  this.list_elem.appendChild(this.list_elem_label);
  this.list_elem.appendChild(this.list_elem_controls);
  this.eq.path_list.add(this, this.path_list_selected, this.list_elem);
  this.band = [];
  this.mdown = {};
  this.hz_default = 440;
  this.hz_off = 0.0;
  this.db_default = 0;
  this.bw_default = 2;
  this.removing = false;
  var eq_info = this.crate.data.afx[0].eq;
  if (eq_info.band.length > 0) {
    for (var i = 0, len = eq_info.band.length; i < len; i++) {
      this.band[i] = new EqBand(this, i, eq_info.band[i].hz,
       eq_info.band[i].db, eq_info.band[i].bw);
    }
  } else {
    for (var i = 0; i < MAX_BANDS; i++) {
      this.band[i] = new EqBand(this, i, this.hz_off,
       this.db_default, this.bw_default);
    }
  }
  var pass_info = this.crate.data.afx[0].pass;
  this.high_pass = new EqHighPass(this, pass_info.high.hz,
   pass_info.high.bw);
  this.low_pass = new EqLowPass(this, pass_info.low.hz,
   pass_info.low.bw);
}

EqPath.prototype.delete = function() {
  if (this == this.eq.selected_path) {
    /*FIXME: should be a function on eq to select/de-select the selected
     path.*/
    this.eq.selected_path = null;
    this.eq.damaged = true;
  }
  this.eq.path_list.remove(this.list_elem);
}

EqPath.prototype.graph_total = function(context, range, nsteps) {
  var delx;
  var x;
  var y;
  var hz;
  var db;
  var xmin = this.eq.hz_to_canvas(range.min);
  var xmax = this.eq.hz_to_canvas(range.max);
  var xpeaks = this.get_xpeaks();
  context.save();
  nsteps = nsteps;
  var nextra = this.eq.nextra;
  var nex_extra = 0;
  delx = (xmax - xmin) / nsteps;
  x = xmin;
  hz = this.eq.canvas_to_hz(x);
  db = this.get_total_db(hz);
  y = this.eq.db_to_canvas(db);
  context.lineCap = "round";
  context.beginPath();
  context.moveTo(x, y);
  for (var i = 1; i < nsteps + 1; i++) {
    var xold = x;
    var testx_old = x - 1 * delx;
    var testx_future = x + 2 * delx
    x = xmin + i * delx;
    for (var p = 0; p < xpeaks.length; p++) {
      if (testx_old < xpeaks[p] && testx_future > xpeaks[p]) {
        var ex_delx = delx / nextra;
        var ex_x = xold;
        for (var j = 1; j < nextra + 1; j++) {
          var ex_xold = ex_x;
          var ex_testx_old = ex_x - 1 * ex_delx;
          var ex_testx_future = ex_x + 2 * ex_delx;
          ex_x = xold + j * ex_delx;
          for (var p2 = 0; p2 < xpeaks.length; p2++) {
            if (ex_testx_old < xpeaks[p2] && ex_testx_future > xpeaks[p2]) {
              var ex_ex_delx = ex_delx / nex_extra;
              var ex_ex_x = ex_xold;
              for (k = 1; k < nex_extra + 1; k++) {
                var ex_ex_testx_old = ex_ex_x;
                var ex_ex_testx_future = ex_ex_x + ex_ex_delx;
                for (var p3 = 0; p3 < xpeaks.length; p3++) {
                  if (ex_ex_testx_old < xpeaks[p3] && ex_ex_testx_future > xpeaks[p3]) {
                    var peak_db =
                     this.get_total_db(this.eq.canvas_to_hz(xpeaks[p3]));
                    var peak_y = this.eq.db_to_canvas(peak_db);
                    context.lineTo(xpeaks[p3], peak_y);
                    break;
                  }
                }
                ex_ex_x = ex_xold + k * ex_ex_delx;
                var ex_ex_db = this.get_total_db(this.eq.canvas_to_hz(ex_ex_x));
                var ex_ex_y = this.eq.db_to_canvas(ex_ex_db);
                context.lineTo(ex_ex_x, ex_ex_y);
              }
              break;
            }
          }
          var ex_db = this.get_total_db(this.eq.canvas_to_hz(ex_x));
          var ex_y = this.eq.db_to_canvas(ex_db);
          context.lineTo(ex_x, ex_y);
        }
        break;
      }
    }
    hz = this.eq.canvas_to_hz(x);
    db = this.get_total_db(hz);
    y = this.eq.db_to_canvas(db);
    context.lineTo(x, y);
  }
  var c = new KRColor(242, 242, 242, 1);
  context.lineWidth = this.eq.line_width;
  context.lineJoin = 'round';
  context.strokeStyle = c.style();
  context.stroke();
  y = this.eq.db_to_canvas(0);
  context.lineTo(x, y);
  x = xmin;
  context.lineTo(x, y);
  context.closePath();
  context.fillStyle = c.copy_with_alpha(0.3666).style();
  context.fill();
  context.restore();
}

EqPath.prototype.get_xpeaks = function() {
  var xpeaks = [];
  for (var key in this.band) {
    if (this.band[key].active) {
      xpeaks.push(this.eq.hz_to_canvas(this.band[key].hz));
    }
  }
  xpeaks.push(this.eq.hz_to_canvas(this.low_pass.hz));
  xpeaks.push(this.eq.hz_to_canvas(this.high_pass.hz));
  return xpeaks;
}

EqPath.prototype.get_total_db = function(hz) {
  var tot = 0;
  for (var key in this.band) {
    if (this.band[key].active) {
      tot = tot + this.band[key].amplitude(hz);
    }
  }
  if (this.low_pass.is_active()) {
    tot = tot + this.low_pass.amplitude(hz);
  }
  if (this.high_pass.is_active()) {
    tot = tot + this.high_pass.amplitude(hz);
  }
  return tot;
}

EqPath.prototype.on_band_remove_click = function(evt) {
  if (this.removing) {
    for (var key in this.band) {
      var b = this.band[key];
      if (b.active && b.remove_ready) {
        b.unready_for_remove();
      }
    }
    this.band_remove_button.classList.remove('flashing');
    this.removing = false;
  } else {
    for (var key in this.band) {
      var b = this.band[key];
      if (b.active) {
        b.ready_for_remove();
      }
    }
    this.band_remove_button.classList.add('flashing');
    this.removing = true;
  }
}

EqPath.prototype.on_band_add_click = function(evt) {
  if (this.removing) {
    this.on_band_remove_click({});
  }
  var id = this.get_unused_band();
  if (id > -1) {
    this.band[id].set_hz_db(this.hz_default, this.db_default);
    this.band[id].set_bw(2 * this.hz_default);
  } else {
    alert("Sorry, out of bands.");
  }
}

EqPath.prototype.get_unused_band = function() {
  for (var i = 0; i < MAX_BANDS; i++) {
    if (this.band[i].active == false) {
      return i;
    }
  }
  return -1;
}

EqPath.prototype.path_list_selected = function() {
  if (this.eq.selected_path != null) {
    this.eq.selected_path.list_elem_controls.classList.remove('active');
  }
  this.eq.selected_path = this;
  this.eq.selected_path.list_elem_controls.classList.add('active');
  this.eq.damaged = true;
}

EqPath.prototype.select = function() {
  this.list_elem.click();
}

function EqHighPass(eq_path, hz, bw) {
 this.eq_path = eq_path;
  this.path = eq_path.path;
  this.eq = this.eq_path.eq;
  this.hz = hz;
  this.max_hz = 20000;
  this.min_hz = 20;
  this.max_x = this.eq.hz_to_canvas(this.max_hz);
  this.min_x = this.eq.hz_to_canvas(this.min_hz);
  this.rbw_handle = {clicked: false};
  this.lbw_handle = {clicked: false};
  var new_bw_hz = Math.pow(2, bw/2) * hz;
  if ((new_bw_hz != 0) && (new_bw_hz > this.eq.hz_max
   || new_bw_hz < this.eq.hz_min)) {
    this.set_bw(hz * hz / new_bw_hz);
  } else {
    this.bw = bw;
  }
  this.get_coefficients();
  this.handle = {};
  this.set_handle(hz, 0);
  this.set_bw_handles();
  this.mdown = {};
  this.rbw_mdown = {};
  this.lbw_mdown = {};
  this.clicked = false;
  this.remove_ready = false;
  this.remove_fill = 'rgba(150, 50, 50, 0.5)';
  this.remove_stroke = 'rgba(150, 50, 50, 0.9)';
  this.set_colors();
  this.eq.damaged = true;
}

EqBand.prototype.set_colors = function() {
  var colors = this.random_fill_stroke_colors();
  this.clicked_handle_fill = colors.handle_fill_clicked;
  this.unclicked_handle_fill = colors.handle_fill_unclicked;
  this.clicked_handle_stroke = colors.handle_stroke_clicked;
  this.unclicked_handle_stroke = colors.handle_stroke_unclicked;
  this.clicked_fill = colors.fill_clicked;
  this.unclicked_fill = colors.fill_unclicked;
  this.clicked_stroke = colors.stroke_clicked;
  this.unclicked_stroke = colors.stroke_unclicked;
}

EqHighPass.prototype = Object.create(EqBand.prototype);
EqHighPass.prototype.constructor = EqHighPass;

EqHighPass.prototype.graph = function(context, range, nsteps) {
  if (this.is_active()) {
    EqBand.prototype.graph.call(this, context, range, nsteps);
  }
}

EqHighPass.prototype.is_active = function() {
  if (this.hz > this.eq.hz_min && this.bw > 0.666) {
    return true;
  } else {
    return false;
  }
}

EqHighPass.prototype.get_coefficients = function() {
  this.SR = 48000;
  this.active = (this.hz > 0);
  if (!this.active) return;
  this.W0 = 2.0 * Math.PI * this.hz / this.SR;
  this.alpha = Math.sin(this.W0)*Math.sinh( Math.log(2)/2.0
   * Math.abs(this.bw) * this.W0/Math.sin(this.W0));
  this.b0 = 0.5 * (1.0 + Math.cos(this.W0));
  this.b1 = -(1.0 + Math.cos(this.W0));
  this.b2 = this.b0;
  this.a0 = 1.0 + this.alpha;
  this.a1 = -2.0 * Math.cos(this.W0);
  this.a2 = 1.0 - this.alpha;
  this.b0 /= this.a0;
  this.b1 /= this.a0;
  this.b2 /= this.a0;
  this.a1 /= this.a0;
  this.a2 /= this.a0;
  this.a0 = 1;
}

EqHighPass.prototype.set_bw_handles = function() {
  var w = this.eq.bw_handle_width;
  var h = this.eq.bw_handle_height;
  this.lbw_handle.x_c = this.eq.hz_to_canvas(Math.pow(2, -this.bw/2) * this.hz);
  this.lbw_handle.y_c = this.eq.db_to_canvas(0);
  this.lbw_handle.x_p = w;
  this.lbw_handle.y_p = h;
  this.rbw_handle.x_c = this.eq.hz_to_canvas(Math.pow(2, this.bw/2) * this.hz);
  this.rbw_handle.y_c = this.eq.db_to_canvas(0);
  this.rbw_handle.x_p = w;
  this.rbw_handle.y_p = h;
}

EqHighPass.prototype.set_handle = function(hz, db) {
  this.handle.x = this.eq.hz_to_canvas(hz);
  this.handle.y = this.eq.db_to_canvas(0);
}

EqHighPass.prototype.set_bw = function(hz, supress) {
  var bw;
  bw = Math.log((hz * hz) / (this.hz * this.hz)) / Math.LN2;
  this.bw = Math.abs(bw);
  this.get_coefficients();
  this.set_bw_handles();
  this.eq.damaged = true;
  if (!supress) {
    var patches = [];
    patches.push(['/afx/0/pass/high/bw', '', this.bw]);
    patches.push(['/afx/1/pass/high/bw', '', this.bw]);
    kr.ctrl_mix(patches, this.path, this.eq);
  }
}

EqHighPass.prototype.click_in = function(evt) {
  this.set_handle(this.hz, this.db);
  var coords = this.eq.event_to_canvas(evt);
  if (coords.x >= this.handle.x - this.eq.handle_radius
   && coords.x <= this.handle.x + this.eq.handle_radius
   && coords.y >= this.handle.y - 1.5 * this.eq.handle_radius
   && coords.y <= this.handle.y + this.eq.handle_radius) {
    var ymax;
    var x = (coords.x - this.handle.x) / this.eq.handle_radius;
    if (x <= 0) {
      ymax = this.handle.y - 1.5 * this.eq.handle_radius
       - 0.5 * this.eq.handle_radius * x;
    } else {
      ymax = this.handle.y - 1.5 * this.eq.handle_radius
       + 0.5 * this.eq.handle_radius * x;
    }
    if (coords.y >= ymax) {
      this.clicked = true;
      this.eq.canvas.classList.add('cursor_none');
      this.mdown.x = coords.x;
      this.mdown.y = coords.y;
      this.mdown.screen_x = evt.screenX;
      this.mdown.screen_y = evt.screenY;
      this.mdown.off_x = coords.x - this.eq.hz_to_canvas(this.hz);
      this.mdown.off_y = coords.y - this.eq.db_to_canvas(this.db);
      this.eq.damaged = true;
      return true;
    }
  }
}

EqHighPass.prototype.draw_handle = function(context, clicked) {
  this.set_handle(this.hz, 0);
  context.save();
  context.beginPath();
  context.beginPath();
  context.moveTo(this.handle.x - this.eq.handle_radius,
   this.handle.y + this.eq.handle_radius);
  context.lineTo(this.handle.x + this.eq.handle_radius,
   this.handle.y + this.eq.handle_radius);
  context.lineTo(this.handle.x + this.eq.handle_radius,
   this.handle.y - this.eq.handle_radius);
  context.lineTo(this.handle.x, this.handle.y - 1.5 * this.eq.handle_radius);
  context.lineTo(this.handle.x - this.eq.handle_radius,
   this.handle.y - this.eq.handle_radius);
  context.lineTo(this.handle.x - this.eq.handle_radius,
   this.handle.y + this.eq.handle_radius);
  context.closePath();
  context.closePath();
  if (this.remove_ready) {
    context.fillStyle = this.remove_fill;
    context.strokeStyle = this.remove_stroke;
  } else {
    context.fillStyle = clicked ? this.clicked_handle_fill
     : this.unclicked_handle_fill;
    context.strokeStyle = clicked ? this.clicked_stroke
     : this.unclicked_stroke;
  }
  context.fill();
  context.lineWidth = this.eq.line_width;
  context.lineJoin = 'round';
  context.stroke();
  context.restore();
}

EqHighPass.prototype.set_hz_db = function(hz, db, supress) {
  this.hz = hz;
  this.get_coefficients();
  this.eq.damaged = true;
  if (!supress) {
    var patches = [];
    patches.push(['/afx/0/pass/high/hz', '', hz]);
    patches.push(['/afx/1/pass/high/hz', '', hz]);
    kr.ctrl_mix(patches, this.path, this.eq);
  }
}

function EqLowPass(eq_path, hz, bw) {
  this.eq_path = eq_path;
  this.path = eq_path.path;
  this.eq = this.eq_path.eq;
  this.hz = hz;
  this.rbw_handle = {clicked: false};
  this.lbw_handle = {clicked: false};
  var new_bw_hz = Math.pow(2, bw/2) * hz;
  if ((new_bw_hz != 0) && (new_bw_hz > this.eq.hz_max
   || new_bw_hz < this.eq.hz_min)) {
    this.set_bw(hz * hz / new_bw_hz, true);
  } else {
    this.bw = bw;
  }
  this.get_coefficients();
  this.handle = {};
  this.set_handle(hz, 0);
  this.set_bw_handles();
  this.mdown = {};
  this.rbw_mdown = {};
  this.lbw_mdown = {};
  this.clicked = false;
  this.remove_ready = false;
  this.remove_fill = 'rgba(242, 0, 0, 0.5)';
  this.remove_stroke = 'rgba(242, 0, 0, 0.9)';
  this.set_colors();
  this.eq.damaged = true;
}

EqLowPass.prototype = Object.create(EqBand.prototype);
EqLowPass.prototype.constructor = EqLowPass;

EqLowPass.prototype.graph = function(context, range, nsteps) {
  if (this.is_active()) {
    EqBand.prototype.graph.call(this, context, range, nsteps);
  }
}

EqLowPass.prototype.is_active = function() {
  if (this.hz < this.eq.hz_max && this.bw > 0.666) {
    return true;
  } else {
    return false;
  }
}

EqLowPass.prototype.get_coefficients = function() {
  this.SR = 48000;
  this.active = (this.hz > 0);
  if (!this.active) return;
  this.W0 = 2.0 * Math.PI * this.hz / this.SR;
  this.alpha = Math.sin(this.W0)*Math.sinh( Math.log(2)/2.0
   * Math.abs(this.bw) * this.W0/Math.sin(this.W0));
  this.b0 = 0.5 * (1.0 - Math.cos(this.W0));
  this.b1 = 1.0 - Math.cos(this.W0);
  this.b2 = this.b0;
  this.a0 = 1.0 + this.alpha;
  this.a1 = -2.0 * Math.cos(this.W0);
  this.a2 = 1.0 - this.alpha;
  this.b0 /= this.a0;
  this.b1 /= this.a0;
  this.b2 /= this.a0;
  this.a1 /= this.a0;
  this.a2 /= this.a0;
  this.a0 = 1;
}

EqLowPass.prototype.draw_handle = function(context, clicked) {
  this.set_handle(this.hz, 0);
  context.save();
  context.beginPath();
  context.moveTo(this.handle.x - this.eq.handle_radius,
   this.handle.y + this.eq.handle_radius);
  context.lineTo(this.handle.x, this.handle.y + 1.5 * this.eq.handle_radius);
  context.lineTo(this.handle.x + this.eq.handle_radius,
   this.handle.y + this.eq.handle_radius);
  context.lineTo(this.handle.x + this.eq.handle_radius,
   this.handle.y - this.eq.handle_radius);
  context.lineTo(this.handle.x - this.eq.handle_radius,
   this.handle.y - this.eq.handle_radius);
  context.lineTo(this.handle.x - this.eq.handle_radius,
   this.handle.y + this.eq.handle_radius);
  context.closePath();
  if (this.remove_ready) {
    context.fillStyle = this.remove_fill;
    context.strokeStyle = this.remove_stroke;
  } else {
    context.fillStyle = clicked ? this.clicked_handle_fill
     : this.unclicked_handle_fill;
    context.strokeStyle = clicked ? this.clicked_stroke
     : this.unclicked_stroke;
  }
  context.fill();
  context.lineWidth = this.eq.line_width;
  context.lineJoin = 'round';
  context.stroke();
  context.restore();
}

EqLowPass.prototype.click_in = function(evt) {
  this.set_handle(this.hz, this.db);
  var coords = this.eq.event_to_canvas(evt);
  if (coords.x >= this.handle.x - this.eq.handle_radius
   && coords.x <= this.handle.x + this.eq.handle_radius
   && coords.y >= this.handle.y - this.eq.handle_radius
   && coords.y <= this.handle.y + 1.5 * this.eq.handle_radius) {
    var ymax;
    var x = (coords.x - this.handle.x) / this.eq.handle_radius;
    if (x <= 0) {
      ymax = this.handle.y + 1.5 * this.eq.handle_radius
       + 0.5 * this.eq.handle_radius * x;
    } else {
      ymax = this.handle.y + 1.5 * this.eq.handle_radius
       - 0.5 * this.eq.handle_radius * x;
    }
    if (coords.y <= ymax) {
      this.clicked = true;
      this.eq.canvas.classList.add('cursor_none');
      this.mdown.x = coords.x;
      this.mdown.y = coords.y;
      this.mdown.screen_x = evt.screenX;
      this.mdown.screen_y = evt.screenY;
      this.mdown.off_x = coords.x - this.eq.hz_to_canvas(this.hz);
      this.mdown.off_y = coords.y - this.eq.db_to_canvas(this.db);
      this.eq.damaged = true;
      return true;
    }
  }
if (coords.x <= this.handle.x + this.eq.handle_radius
      && coords.y >= this.handle.y - this.eq.handle_radius
      && coords.y <= this.handle.y + this.eq.handle_radius) {
    var xmax;
    var y = this.eq.db_to_canvas(0) - coords.y;
    if (y >= 0) {
      xmax = this.handle.x - 1.5 * this.eq.handle_radius + 0.5 * y;
    } else {
      xmax = this.handle.x - 1.5 * this.eq.handle_radius - 0.5 * y;
    }
    if (coords.x >= xmax) {
      this.clicked = true;
      this.eq.canvas.classList.add('cursor_none');
      this.mdown.x = coords.x;
      this.mdown.y = coords.y;
      this.mdown.screen_x = evt.screenX;
      this.mdown.screen_y = evt.screenY;
      this.mdown.off_x = coords.x - this.eq.hz_to_canvas(this.hz);
      this.mdown.off_y = coords.y - this.eq.db_to_canvas(this.db);
      this.eq.damaged = true;
      return true;
    }
  }
}

EqLowPass.prototype.set_bw_handles = function() {
  var w = this.eq.bw_handle_width;
  var h = this.eq.bw_handle_height;
  this.lbw_handle.x_c = this.eq.hz_to_canvas(Math.pow(2, -this.bw/2) * this.hz);
  this.lbw_handle.y_c = this.eq.db_to_canvas(0);
  this.lbw_handle.x_p = w;
  this.lbw_handle.y_p = h;
  this.rbw_handle.x_c = this.eq.hz_to_canvas(Math.pow(2, this.bw/2) * this.hz);
  this.rbw_handle.y_c = this.eq.db_to_canvas(0);
  this.rbw_handle.x_p = w;
  this.rbw_handle.y_p = h;
}

EqLowPass.prototype.set_handle = function(hz, db) {
  this.handle.x = this.eq.hz_to_canvas(hz);
  this.handle.y = this.eq.db_to_canvas(0);
}

EqLowPass.prototype.set_bw = function(hz, supress) {
  var bw;
  bw = Math.log((hz * hz) / (this.hz * this.hz)) / Math.LN2;
  this.bw = Math.abs(bw);
  this.get_coefficients();
  this.set_bw_handles();
  this.eq.damaged = true;
  if (!supress) {
    var patches = [];
    patches.push(['/afx/0/pass/low/bw', '', this.bw]);
    patches.push(['/afx/1/pass/low/bw', '', this.bw]);
    kr.ctrl_mix(patches, this.path, this.eq);
  }
}

EqLowPass.prototype.set_hz_db = function(hz, db, supress) {
  this.hz = hz;
  this.get_coefficients();
  this.eq.damaged = true;
  if (!supress) {
    var patches = [];
    patches.push(['/afx/0/pass/low/hz', '', hz]);
    patches.push(['/afx/1/pass/low/hz', '', hz]);
    kr.ctrl_mix(patches, this.path);//, this.eq);
  }
}

function EqBand(eq_path, band, hz, db, bw)  {
  this.eq_path = eq_path;
  this.path = eq_path.path;
  this.band = band;
  this.eq = this.eq_path.eq;
  this.hz = hz;
  this.db = db;
  this.rbw_handle = {clicked: false};
  this.lbw_handle = {clicked: false};
  var new_bw_hz = Math.pow(2, bw/2) * hz;
  if ((new_bw_hz != 0) && (new_bw_hz > this.eq.hz_max
   || new_bw_hz < this.eq.hz_min)) {
    this.set_bw(hz * hz / new_bw_hz, true);
  } else {
    this.bw = bw;
  }
  this.get_coefficients();
  this.handle = {};
  this.set_handle(hz, db);
  this.set_bw_handles();
  this.mdown = {};
  this.rbw_mdown = {};
  this.lbw_mdown = {};
  this.clicked = false;
  this.remove_ready = false;
  this.remove_fill = 'rgba(222, 222, 222, 0.5)';
  this.remove_stroke = 'rgba(222, 222, 222, 0.9)';
  this.set_colors();
  this.eq.damaged = true;
}

EqBand.prototype.random_fill_stroke_colors = function() {
  var colors = {};
  var c = this.eq.handle_color_scheme.next().copy_with_alpha(0.3666);
  colors.handle_fill_clicked = c.to_string();
  colors.handle_fill_unclicked = c.to_string();
  c.a = 1.0;
  colors.handle_stroke_clicked = c.to_string();
  colors.handle_stroke_unclicked = c.to_string();
  c.a = 0.1666;
  colors.fill_clicked = c.to_string();
  colors.fill_unclicked = c.copy_with_alpha(0.0).to_string();
  c.a = 1.0;
  colors.stroke_clicked = c.to_string();
  colors.stroke_unclicked = c.to_string();
  return colors;
}

EqBand.prototype.ready_for_remove = function() {
  this.remove_ready = true;
  this.eq.damaged = true;
}

EqBand.prototype.unready_for_remove = function() {
  this.remove_ready = false;
  this.eq.damaged = true;
}

EqBand.prototype.get_coefficients = function() {
  this.SR = 48000;
  this.active = (this.hz > 0);
  if (!this.active) return;
  this.A = Math.pow(10.0, this.db/40.0);
  this.W0 = 2.0 * Math.PI * this.hz / this.SR;
  this.alpha = Math.sin(this.W0)*Math.sinh( Math.log(2)/2.0
   * Math.abs(this.bw) * this.W0/Math.sin(this.W0));
  this.b0 = 1.0 + this.alpha * this.A;
  this.b1 = -2.0 * Math.cos(this.W0);
  this.b2 = 1.0 - this.alpha * this.A;
  this.a0 = 1.0 + this.alpha / this.A;
  this.a1 = -2.0 * Math.cos(this.W0);
  this.a2 = 1.0 - this.alpha / this.A;
  this.b0 /= this.a0;
  this.b1 /= this.a0;
  this.b2 /= this.a0;
  this.a1 /= this.a0;
  this.a2 /= this.a0;
  this.a0 = 1;
}

EqBand.prototype.amplitude = function(freq) {
  var w;
  var numerator;
  var denominator;
  var magnitude;
  w = 2.0*Math.PI*freq / this.SR;
  numerator = this.b0*this.b0 + this.b1*this.b1
    + this.b2*this.b2 + 2.0*(this.b0*this.b1 + this.b1*this.b2)*Math.cos(w)
    + 2.0*this.b0*this.b2*Math.cos(2.0*w);
  denominator = 1.0 + this.a1*this.a1 + this.a2*this.a2
    + 2.0*(this.a1 + this.a1*this.a2)*Math.cos(w)
    + 2.0*this.a2*Math.cos(2.0*w);
  magnitude = Math.sqrt(numerator / denominator);
  return 20 * Math.log(magnitude)/Math.log(10);
}

EqBand.prototype.graph = function(context, range, nsteps) {
  if (this.hz <= this.eq_path.hz_off) return;
  var eq = this.eq;
  var context = context;
  var xmin = eq.hz_to_canvas(range.min);
  var xmax = eq.hz_to_canvas(range.max);
  var xpeak = eq.hz_to_canvas(this.hz);
  var delx;
  var hz;
  var x = xmin;
  var db;
  var y;
  nsteps = nsteps;
  var nextra = this.eq.nextra;
  var nex_extra = 0;//this.eq.nex_extra;
  delx = (xmax - xmin) / nsteps;
  x = xmin;
  hz = eq.canvas_to_hz(xmin);
  context.save();
  context.lineCap = "round";
  context.beginPath();
  context.moveTo(x, eq.db_to_canvas(this.amplitude(this.eq.canvas_to_hz(x))));
  for (var i = 1; i < nsteps + 1; i++) {
    var xold = x;
    var testx_old = x - 3 * delx;
    var testx_future = x + 3 * delx
    x = xmin + i * delx;
    if (testx_old < xpeak && testx_future > xpeak) {
      var ex_delx = delx / nextra;
      var ex_x = xold;
      for (var j = 1; j < nextra + 1; j++) {
        var ex_xold = ex_x;
        var ex_testx_old = ex_x - 3 * ex_delx;
        var ex_testx_future = ex_x + 4 * ex_delx;
        ex_x = xold + j * ex_delx;
        if (ex_testx_old < xpeak && ex_testx_future > xpeak) {
          var ex_ex_delx = ex_delx / nex_extra;
          for (k = 1; k < nex_extra + 1; k++) {
            var ex_ex_testx_old = ex_ex_x;
            var ex_ex_testx_future = ex_ex_x + ex_ex_delx;
            var ex_ex_x = ex_xold + k * ex_ex_delx;
            if (ex_ex_testx_old < xpeak && ex_ex_testx_future > xpeak) {
              var peak_db = this.amplitude(this.eq.canvas_to_hz(xpeak));
              var peak_y = this.eq.db_to_canvas(peak_db);
               context.lineTo(xpeak, peak_y);
            }
            var ex_ex_db = this.amplitude(this.eq.canvas_to_hz(ex_ex_x));
            var ex_ex_y = this.eq.db_to_canvas(ex_ex_db);
            context.lineTo(ex_ex_x, ex_ex_y);
          }
        }
        var ex_db = this.amplitude(this.eq.canvas_to_hz(ex_x));
        var ex_y = eq.db_to_canvas(ex_db);
        context.lineTo(ex_x, ex_y);
      }
    }
    db = this.amplitude(this.eq.canvas_to_hz(x));
    y = eq.db_to_canvas(db);
    context.lineTo(x, y);
  }
  var clicked = this.clicked || this.lbw_handle.clicked
   || this.rbw_handle.clicked;
  context.lineWidth = this.eq.line_width;
  context.lineJoin = 'round';
  context.strokeStyle = clicked ? this.clicked_stroke
   : this.unclicked_stroke;
  context.stroke();
  context.strokeStyle = 'rgba(0,0,0,0)';
  y = eq.db_to_canvas(0);
  context.lineTo(x,y);
  x = xmin;
  context.lineTo(x, y);
  context.closePath();
  context.fillStyle = clicked ? this.clicked_fill : this.unclicked_fill;
  context.fill();
  context.restore();
}

EqBand.prototype.draw_handle = function(context, clicked) {
  this.set_handle(this.hz, this.db);
  context.beginPath();
  context.arc(this.handle.x, this.handle.y, this.eq.handle_radius, 0, 2 * Math.PI);
  if (this.remove_ready) {
    context.fillStyle = this.remove_fill;
    context.strokeStyle = this.remove_stroke;
  } else {
    context.fillStyle = clicked ? this.clicked_handle_fill
     : this.unclicked_handle_fill;
    context.strokeStyle = clicked ? this.clicked_handle_stroke
     : this.unclicked_handle_stroke;
  }
  context.fill();
  context.lineWidth = this.eq.line_width;
  context.lineJoin = 'round';
  context.stroke();
}

EqBand.prototype.remove = function() {
  this.set_hz_db(this.eq_path.hz_off, this.eq_path.db_default);
  this.remove_ready = false;
  this.on_handle_mouseup();
}

EqBand.prototype.on_handle_mouseup = function(evt) {
  this.eq.canvas.classList.remove('cursor_none');
  this.clicked = false;
  this.eq.damaged = true;
}

EqBand.prototype.on_lbw_mouseup = function(evt) {
  this.eq.canvas.classList.remove('cursor_none');
  this.lbw_handle.clicked = false;
  this.eq.damaged = true;
}

EqBand.prototype.on_rbw_mouseup = function(evt) {
  this.eq.canvas.classList.remove('cursor_none');
  this.rbw_handle.clicked = false;
  this.eq.damaged = true;
}

EqBand.prototype.draw_bw_handles = function(context, clicked) {
  this.set_bw_handles();
  var handles = [this.lbw_handle, this.rbw_handle];
  context.save();
  for (var i = 0; i < 2; i++) {
    var h = handles[i];
    context.beginPath();
    context.moveTo(h.x_c - h.x_p, h.y_c + h.y_p);
    context.lineTo(h.x_c + h.x_p, h.y_c + h.y_p);
    context.lineTo(h.x_c + h.x_p, h.y_c - h.y_p);
    context.lineTo(h.x_c - h.x_p, h.y_c - h.y_p);
    context.lineTo(h.x_c - h.x_p, h.y_c + h.y_p);
    context.closePath();
    context.fillStyle = clicked ? this.clicked_handle_fill
     : this.unclicked_handle_fill;
    context.fill();
    context.lineWidth = this.eq.line_width;
    context.lineJoin = 'round';
    context.strokeStyle = clicked ? this.clicked_stroke
     : this.unclicked_stroke;
    context.stroke();
  }
  context.restore();
}

EqBand.prototype.lbw_click_in = function(evt) {
  this.set_bw_handles();
  var h = this.lbw_handle;
  var coords = this.eq.event_to_canvas(evt);
  if (coords.x >= h.x_c - h.x_p
   && coords.x <= h.x_c + h.x_p
   && coords.y >= h.y_c - h.y_p
   && coords.y <= h.y_c + h.y_p) {
    this.lbw_handle.clicked = true;
    this.eq.canvas.classList.add('cursor_none');
    this.lbw_mdown.x = coords.x;
    this.lbw_mdown.screen_x = evt.screenX;
    this.lbw_mdown.off_x =  coords.x - h.x_c;
    this.eq.damaged = true;
    return true;
  }
}

EqBand.prototype.rbw_click_in = function(evt) {
  this.set_bw_handles();
  var h = this.rbw_handle;
  var coords = this.eq.event_to_canvas(evt);
  if (coords.x >= h.x_c - h.x_p
   && coords.x <= h.x_c + h.x_p
   && coords.y >= h.y_c - h.y_p
   && coords.y <= h.y_c + h.y_p) {
    this.rbw_handle.clicked = true;
    this.eq.canvas.classList.add('cursor_none');
    this.rbw_mdown.x = coords.x;
    this.rbw_mdown.screen_x = evt.screenX;
    this.rbw_mdown.off_x = coords.x - h.x_c;
    this.eq.damaged = true;
    return true;
  }
}

EqBand.prototype.click_in = function(evt) {
  this.set_handle(this.hz, this.db);
  var coords = this.eq.event_to_canvas(evt);
  if (Math.sqrt(Math.pow(coords.x - this.handle.x, 2)
   + Math.pow(coords.y - this.handle.y, 2)) <= this.eq.handle_radius) {
    this.clicked = true;
    this.eq.canvas.classList.add('cursor_none');
    this.mdown.x = coords.x;
    this.mdown.y = coords.y;
    this.mdown.screen_x = evt.screenX;
    this.mdown.screen_y = evt.screenY;
    this.mdown.off_x = coords.x - this.eq.hz_to_canvas(this.hz);
    this.mdown.off_y = coords.y - this.eq.db_to_canvas(this.db);
    this.eq.damaged = true;
    return true;
  }
}

EqBand.prototype.set_bw = function(hz, supress) {
  var bw;
  bw = Math.log((hz * hz) / (this.hz * this.hz)) / Math.LN2;
  this.bw = Math.abs(bw);
  this.get_coefficients();
  this.set_bw_handles();
  this.eq.damaged = true;
  if (!supress) {
    var patches = [];
    patches.push(['/afx/0/eq/band/' + this.band + '/bw', '', this.bw]);
    patches.push(['/afx/1/eq/band/' + this.band + '/bw', '', this.bw]);
    kr.ctrl_mix(patches, this.path, this.eq);
  }
}

EqBand.prototype.set_hz_db = function(hz, db, supress) {
  this.hz = hz;
  this.db = db;
  this.get_coefficients();
  this.eq.damaged = true;
  if (!supress) {
    var patches = [];
    patches.push(['/afx/0/eq/band/' + this.band + '/hz', '', hz]);
    patches.push(['/afx/1/eq/band/' + this.band + '/hz', '', hz]);
    patches.push(['/afx/0/eq/band/' + this.band + '/db', '', db]);
    patches.push(['/afx/1/eq/band/' + this.band + '/db', '', db]);
    kr.ctrl_mix(patches, this.path, this.eq);
  }
}

EqBand.prototype.set_bw_handles = function() {
  var w = this.eq.bw_handle_width;
  var h = this.eq.bw_handle_height;
  this.lbw_handle.x_c = this.eq.hz_to_canvas(Math.pow(2, -this.bw/2) * this.hz);
  this.lbw_handle.y_c = this.eq.db_to_canvas(this.db);
  this.lbw_handle.x_p = w;
  this.lbw_handle.y_p = h;
  this.rbw_handle.x_c = this.eq.hz_to_canvas(Math.pow(2, this.bw/2) * this.hz);
  this.rbw_handle.y_c = this.eq.db_to_canvas(this.db);
  this.rbw_handle.x_p = w;
  this.rbw_handle.y_p = h;
}

EqBand.prototype.set_handle = function(hz, db) {
  this.handle.x = this.eq.hz_to_canvas(hz);
  this.handle.y = this.eq.db_to_canvas(db);
}
