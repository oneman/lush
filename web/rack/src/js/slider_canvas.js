var MIXER_TEXT_COLOR = new KRColor(185, 185, 185, 1);
var MIXER_BG_COLOR = new KRColor(50, 100, 125, 1);
var EQ_HANDLE_COLOR = new KRColor(200, 200, 10, 1);
var SHADOW_LENGTH = 1.5;
var SHADOW_BLUR = 0.5;

function KRSliderModel(pos, path, label, prop, opts) {
  this.lowest_allowed_value = -120;
  this.path = path;
  this.label = label;
  this.prop = prop;
  this.mdown = false;
  this.last_patched;
  this.handle = {};
  this.len = 1;
  this.pos = pos;
  this.views = [];
  this.range = null;
  this.sender = undefined;
  this.precision = 0;
  if (opts.sender) {
    this.sender = opts.sender;
  }
  if (opts.range) {
    this.range = new KRRange(
     opts.range[1],
     opts.range[0],
     opts.knee);
  } else {
    return 0;
  }
  if (typeof(opts.initial_value) != 'undefined') {
    this.set_pos(opts.initial_value);
    this.last_patched = opts.initial_value;
  }
  if (opts.tick_skip != undefined) {
    this.tick_skip = opts.tick_skip;
  }
  if (opts.precision != undefined) {
    this.precision = opts.precision;
  }
}

KRSliderModel.prototype.set_from_pos = function(new_pos) {
  var new_val = this.pos_to_value(new_pos);
  if (new_val != this.last_patched && !isNaN(new_val)) {
    this.set_pos(new_val);
    kr.ctrl_mix([[this.prop[0], '', new_val]], this.path, this.sender);
    kr.ctrl_mix([[this.prop[1], '', new_val]], this.path, this.sender);
    this.last_patched = new_val;
  }
}

KRSliderModel.prototype.add_view = function(view) {
  this.views.push(view);
}

KRSliderModel.prototype.remove_view = function(view) {
  if (this.views.indexOf(view) > -1) {
    this.views.splice(this.views.indexOf(view), 1);
  }
}

KRSliderModel.prototype.set_pos = function(value) {
  this.pos = this.pos_from_value(value);
  for (var i = 0; i < this.views.length; i++) {
    this.views[i].set_handle();
  }
}

KRSliderModel.prototype.get_pos = function() {
  return this.pos;
}

KRSliderModel.prototype.db_to_linear = function(db) {
  return Math.pow(10, (db / 20.0));
}

KRSliderModel.prototype.linear_to_db = function(val) {
  return 20.0 * Math.log(val) / Math.LN10;
}

KRSliderModel.prototype.pos_to_value = function(pos) {
  var x = pos; // normalized between 0, 1
  if (x <= 0) return this.range.max;
  if (x >= 1) return this.range.min;
  if (x <= this.range.knee.move_fraction) {
    var maxx = this.range.knee.move_fraction;
    var max = this.db_to_linear(this.range.max);
    var min = this.db_to_linear(this.range.max - this.range.knee.range);
    return Math.min(this.range.max, Math.max(this.linear_to_db(max
     - x * (max - min) / maxx), this.range.min));
  } else {
    var minx = this.range.knee.move_fraction;
    var maxx = 1;
    var max = this.range.max - this.range.knee.range;
    var min = this.range.min;
    return Math.min(max, Math.max(max - (x - minx)
     * (max - min) / (maxx - minx), min));
  }
}

KRSliderModel.prototype.pos_from_value = function(val) {
  if (val <= this.range.min) return 1;
  if (val >= this.range.max) return 0;
  if (val >= (this.range.max - this.range.knee.range)) {
    var x = (this.db_to_linear(this.range.max) - this.db_to_linear(val))
     / (this.db_to_linear(this.range.max)
     - this.db_to_linear(this.range.max - this.range.knee.range));
    return this.len * this.range.knee.move_fraction * x;
  } else {
    var maxx = this.len;
    var minx = this.len * this.range.knee.move_fraction;
    var max = this.range.max;
    var min = this.range.min + this.range.knee.move_fraction
     * this.range.knee.range;
    return minx + (maxx - minx) * (max - val) / (max - min);
  }
}

function KRValueView(parent_view, model, context, rect, colors, opts) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.active = false;
  this.parent_view = parent_view;
  this.model = model;
  this.model.add_view(this);
  this.context = context;
  this.bg = new KRRectangleView(this, null, context, rect.shape.bbox.copy(),
   colors, {'rounded': true});
  this.bg.set_setting_mode('OUTSET');
  this.timer = null;
  this.show_model_label = opts.show_model_label;
  this.always_active = opts.always_active;
  if (this.always_active) this.activate();
  this.set_handle();
  this.set_colors(colors);
}

KRValueView.prototype = Object.create(KRView.prototype);
KRValueView.prototype.constructor = KRValueView;

KRValueView.prototype.damage = function() {
  this.bg.damage();
  this.damaged = true;
  this.parent_view.damage();
}

KRValueView.prototype.deactivate = function() {
  this.active = false;
  this.damage();
  this.timer = null;
}

KRValueView.prototype.activate = function(dur) {
  if (this.timer) {
    clearInterval(this.timer);
  }
  this.active = true;
  this.damage();
  if (dur !== undefined) {
    this.timer = setTimeout(create_handler(this, this.deactivate), dur);
  }
}

KRValueView.prototype.set_handle = function() {
  val = this.model.pos_to_value(this.model.pos);
  this.text = "";
  if (this.show_model_label) {
    this.text = this.model.label + ": ";
  }
  if (Math.round(val) <= -120) {
    this.text = this.text + "-∞";
  } else {
    this.text = this.text + val.toPrecision(3);
  }
  this.bg.damage();
  this.damage();
  if (!this.always_active) this.activate(2000);
}

KRValueView.prototype.resize = function(scale) {
  this.scale = scale;
  this.bg.resize(scale);
  this.damage();
}

KRValueView.prototype.in = function(coords) {
  return this.bg.rect.in(coords);
}

KRValueView.prototype.set_bbox = function(bbox) {
  this.bg.set_bbox(bbox);
  this.resize(this.parent_view.scale);
}

KRValueView.prototype.set_colors = function(colors) {
  this.colors = {};
  this.colors.trough_fill = colors.trough_fill.darken().darken();
  this.colors.trough_stroke = colors.trough_stroke.copy();
  this.bg.set_colors(this.colors);
  this.damage();
}

KRValueView.prototype.render_text = function() {
  this.context.save();
  this.context.textBaseline = 'middle';
  this.context.textAlign = 'center';
  this.context.font = 'normal ' + 0.75 * (this.bg.rect.shape.bbox.h -
      2*this.bg.rect.shape.stroke.w) + 'px play';
  var w = this.context.measureText(this.text).width;
  var h = parseInt(this.context.font, 10);
  var fit = true;
  while (w > 0.9 * (this.bg.rect.shape.bbox.w - 2*this.bg.rect.shape.stroke.w)) {
    h--;
    if (h <= 0) {
      fit = false;
      break;
    }
    this.context.font = 'normal ' + h + 'px play';
    w = this.context.measureText(this.text).width;
  }
  if (fit) {
    var x = this.bg.rect.shape.bbox.x + 0.5 * this.bg.rect.shape.bbox.w;
    var y = this.bg.rect.shape.bbox.y + 0.5 * this.bg.rect.shape.bbox.h;
    this.fill_text_outset(this.text, this.colors.trough_fill, x, y);
  }
  this.context.restore();
}

KRValueView.prototype.render = function() {
  if (this.active !== true) return;
  this.bg.render();
  this.render_text();
  this.damaged = false;
}

KRValueViewPeak = function(parent_view, model, context, rect, colors, opts) {
  this.inherit_from = KRValueView;
  this.inherit_from(parent_view, model, context, rect, colors, opts);
  this.clipped = false;
  this.clipped_colors = {
   trough_fill: new KRColor(252, 21, 21, 1).darken().darken(),
   trough_stroke: this.colors.trough_stroke.copy()
   };
}

KRValueViewPeak.prototype = Object.create(KRValueView.prototype);
KRValueViewPeak.prototype.constructor = KRValueViewPeak;

KRValueViewPeak.prototype.set_handle = function() {
  var val = new Number(20 * Math.log(this.parent_view.allTimePeak) / Math.LN10);
  this.text = "";
  if (this.show_model_label) {
    this.text = this.model.label + ": ";
  }
  if (Math.round(val) <= -120) {
    this.text = this.text + "-∞";
  } else {
    this.text = this.text + val.toPrecision(3);
  }
  if (val > 0) {
    this.clipped = true;
    this.bg.set_colors(this.clipped_colors);
  } else if (val <=0 && this.clipped) {
    this.clipped = false;
    this.bg.set_colors(this.colors);
  }
  this.bg.damage();
  this.damage();
  if (!this.always_active) this.activate(2000);
}

function KRView() {
  this.z = 0;
  this.subviews = [];
  this.active = false;
  this.damaged = false;
  this.mouse_over = false;
}

KRView.prototype.set_colors = function(colors) {
  this.colors = colors;
  for (var i = 0; i < this.subviews.length; i++) {
    this.subviews[i].set_colors(this.colors);
  }
  this.damage();
}

KRView.prototype.damage = function() {
  if (this.parent_view) this.parent_view.damage();
  if (this.unit) this.unit.damaged = true;
  this.damaged = true;
}

KRView.prototype.add_view = function(view) {
  var z = view.z ? view.z : 0;
  var done = false;
  var i;
  for (i = 0; i < this.subviews.length; i++) {
    var sv = this.subviews[i];
    var vz = sv.z ? sv.z : 0;
    if (z < vz) {
      break;
    }
  }
  this.subviews.splice(i, 0, view);
}

KRView.prototype.sort_views = function() {
  this.subviews.sort(KRView.z_order_sort);
}

KRView.z_order_sort = function(a, b) {
  var az = a.z || 0;
  var bz = b.z || 0;
  return (az - bz);
}

KRView.prototype.render = function() {
  if (!this.active) {
    this.damage();
    return
  } else if (this.damaged) {
    for (var i = 0, len = this.subviews.length; i < len; i++) {
      var v = this.subviews[i];
      //v.damage();
      v.render();
    }
    this.damaged = false;
  }
}

KRView.prototype.activate = function() {
  if (this.container_elem) {
    this.container_elem.classList.add('active');
  } else {
    if (this.parent_view.activate) {
       this.parent_view.activate();
    }
  }
  this.active = true;
}

KRView.prototype.deactivate = function() {
  if (this.container_elem) {
    this.container_elem.classList.remove('active');
  } else {
    if (this.parent_view.deactivate) {
       this.parent_view.deactivate();
    }
  }
  this.active = false;
}

KRView.prototype.fill_text_inset = function(text, text_color,
 x, y, context, orientation) {
  if (context === undefined) {
    context = this.context;
  }
  if (orientation === undefined) {
    orientation = 'H';
  }
  var shadow_length = SHADOW_LENGTH;//;; * this.scale * this.rel_size;
  var off_x = 0;
  var off_y = 0;
  if (orientation === 'V') {
    off_x = -shadow_length;
  } else if (orientation == 'H') {
    off_y = shadow_length;
  }
  context.save();
  context.shadowColor =
   text_color.lighten().lighten().style();
  context.shadowOffsetX = off_x;
  context.shadowOffsetY = off_y;
  context.shadowBlur = SHADOW_BLUR;
  context.fillStyle = text_color.darken().darken().style();
  context.fillText(text, x, y);
  context.restore();
}

KRView.prototype.fill_text_outset = function(text, color, x, y, context,
 orientation) {
  if (context === undefined) {
    context = this.context;
  }
  if (orientation === undefined) {
    orientation = 'H';
  }
  var text_color;
  text_color = MIXER_TEXT_COLOR;
  var off_x = 0;
  var off_y = 0;
  var shadow_length = SHADOW_LENGTH;
  if (orientation === 'V') {
    off_x = -shadow_length;
    off_y = 0;
  } else if (orientation == 'H') {
    off_x = 0;
    off_y = shadow_length;
  }
  context.save();
  context.shadowColor =
   text_color.darken().darken().darken().darken().style();
  context.shadowOffsetX = off_x;
  context.shadowOffsetY = off_y;
  context.shadowBlur = SHADOW_BLUR;
  context.fillStyle = text_color.style();
  context.fillText(text, x, y);
  context.restore();
}

function KRSliderView() {
}

KRSliderView.prototype = Object.create(KRView.prototype);
KRSliderView.prototype.constructor = KRSliderView;

/*Hrm.  not ideal because it is a function of the generic
 class and it sets different limits based on orientation.*/
KRSliderView.prototype.render_ticks = function(orientation) {
  var val = this.model.range.max;
  if (orientation == 'V') {
    this.last_drawn_pos = -666666;
    this.pos_edge = this.trough.rect.shape.bbox.y + this.trough.rect.shape.bbox.h
     - this.trough.rect.shape.stroke.w;
  } else {
    this.last_drawn_pos = 666666;
    this.pos_edge = this.trough.shape.bbox.x - this.bbox.x +
     0.5 * this.handle.shape.bbox.w;
  }
  var tick_skip = this.model.tick_skip || 6
  this.draw_tick(val, "first");
  val -= tick_skip;
  while (val > this.model.range.min) {
    this.draw_tick(val);
    val -= tick_skip;
  }
  if (orientation == 'V') {
    this.pos_edge = 6666666;
  } else {
    this.pos_edge = -6666666;
  }
  this.draw_tick(this.model.range.min, "last");
}

function KRSliderHorizontalView(parent_view, model, context, bbox, colors) {
  this.parent_view = parent_view;
  this.scale = parent_view.scale;
  this.full_size = [200, 100];
  this.model = model;
  this.model.add_view(this);
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.colors = colors;
  this.TRACK_WIDTH = 6;
  this.LINE_WIDTH = 0;
  this.TICK_WIDTH = 1;
  this.TICK_LEN = 4;
  this.line_width = Math.max(1, this.LINE_WIDTH * this.scale);
  var h = this.BBOX.h;
  var button_space = h / 1.618;
  this.button_space = button_space;
  this.contracted_w = 2 * button_space;
  this.expanded_w = this.BBOX.w;
  this.expansion_states = { EXPANDED: 0, CONTRACTED: 1, EXPANDING: 2,
   CONTRACTING: 3 };
  this.expansion_state = this.expansion_states.EXPANDED;
  var background_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y,
   this.BBOX.w, this.BBOX.h);
  var shape = new KRShape(background_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.background = new KRRectangle(shape);
  var trough_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y,
   this.BBOX.w, this.BBOX.h);
  var shape = new KRShape(trough_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.trough = new KRRectangle(shape);
  var height = this.BBOX.h - 2 * this.LINE_WIDTH;
  var handle_setup = { w: (1/3) * height, h: height };
  var y = this.BBOX.y + 0.5 * this.BBOX.h - 0.5 * handle_setup.h;
  var handle_shape = new KRShape(new KRBoundBox(trough_bbox.x, y,
   handle_setup.w, handle_setup.h),
   new KRStroke(colors.handle_stroke, 0, 'round'),
   new KRFill(colors.handle_fill));
  this.handle = new KRRectangle(handle_shape);
  this.len = this.trough.shape.bbox.w - this.handle.shape.bbox.w;
  this.resize(this.parent_view.scale);
  this.set_handle();
}

/*Inherat from slider view base class for common code to Vert and Horiz
 sliders.*/
KRSliderHorizontalView.prototype = Object.create(KRSliderView.prototype);
KRSliderHorizontalView.prototype.constructor = KRSliderHorizontalView;

KRSliderHorizontalView.prototype.damage = function() {
  this.damaged = true;
  this.parent_view.damage();
}

KRSliderHorizontalView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.handle.shape.stroke.color = new KRColor(185, 185, 185, 1)
  this.handle.shape.fill.color = this.handle.shape.stroke.color.copy();
  this.trough.shape.stroke.color = this.colors.trough_stroke;
  this.trough.shape.fill.color = this.colors.trough_fill.darken();
  this.background.shape.stroke.color = this.colors.trough_stroke;
  this.background.shape.fill.color = this.colors.trough_fill;
  this.damage();
}

KRSliderHorizontalView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  this.rel_size = this.BBOX.w / this.full_size[0];
  this.bbox = this.BBOX.scale(this.scale);
  this.background.set_bbox(this.BBOX.copy());
  var pad = 4 * SHADOW_LENGTH;
  var trough_bbox = new KRBoundBox(pad + this.BBOX.x, pad + this.BBOX.y
   + 0.25 * this.BBOX.h, this.BBOX.w - 2 * pad, 0.75 * this.BBOX.h - 2 * pad);
  this.trough.set_bbox(trough_bbox);
  var height = trough_bbox.h + SHADOW_LENGTH
   - 2 * this.rel_size * this.LINE_WIDTH;
  var width = height / 3;
  var y = trough_bbox.y + 0.5 * trough_bbox.h - 0.5 * height;
  this.handle.set_bbox(new KRBoundBox(trough_bbox.x, y,
   width, height));
  this.resize(this.scale);
  this.set_handle();
}

KRSliderHorizontalView.prototype.resize = function(scale) {
  this.scale = scale;
  this.tick_width = this.TICK_WIDTH * scale * this.rel_size;
  this.tick_len = this.TICK_LEN * scale * this.rel_size;
  this.line_width = this.LINE_WIDTH * scale * this.rel_size;
  this.track_width = this.TRACK_WIDTH * scale * this.rel_size;
  this.bbox = this.BBOX.scale(scale);
  this.trough.resize(scale, this.rel_size);
  this.handle.resize(scale, this.rel_size);
  this.background.resize(scale, this.rel_size);
  this.dpr = window.devicePixelRatio;
  var pad = 0;
  this.len = Math.max(0, this.trough.shape.bbox.w - pad
   - 2 * this.trough.shape.stroke.w - this.handle.shape.bbox.w);
  this.damage();
}

KRSliderHorizontalView.prototype.set_handle = function() {
  var ratio = this.model.pos;
  var x = this.len * (1 - ratio) + this.trough.shape.bbox.x
   + this.trough.shape.stroke.w;
  this.handle.BBOX.x = x / this.scale;
  this.handle.resize(this.scale, this.rel_size);
  this.damage();
}

KRSliderHorizontalView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

KRSliderHorizontalView.prototype.render_label = function() {
  var val = this.model.pos_to_value(this.model.pos);
  var text;
  if (Math.round(val) == -120) {
    text = '-∞';
  } else {
    text = val.toPrecision(2);
  }
  text = this.model.label + ': ' + text;
  this.context.save();
  this.context.font = 'normal ' + 0.2 * this.bbox.h + 'px play';
  this.context.textBaseline = 'middle';
  this.context.textAlign = 'center';
  var w = this.context.measureText(text).width;
  var h = parseInt(this.context.font, 10);
  var fit = true;
  while (w > 0.9 * this.trough.shape.bbox.w - 2 * this.trough.shape.stroke.w) {
    h--;
    if (h <= 0) {
      fit = false;
      break;
    }
    this.context.font = 'normal ' + h + 'px play';
    w = this.context.measureText(text).width;
  }
  if (fit) {
    var ratio = this.model.pos;
    var y;
    var x;
    x = this.len * (1 - ratio) + this.trough.shape.bbox.x
     + this.trough.shape.stroke.w + 0.5 * this.handle.shape.bbox.w;
    x = Math.min(this.trough.shape.bbox.x + this.trough.shape.bbox.w - 0.5 * w,
     Math.max(this.trough.shape.bbox.x + 0.5 * w, x));
    y = this.bbox.y + 0.125 * this.bbox.h + 2 * this.scale * SHADOW_LENGTH;
    this.fill_text_outset(text, this.colors.trough_fill, x, y);
  }
  this.context.restore();
}

KRSliderHorizontalView.prototype.draw_tick = function(val, pos) {
  return;
  var frac = this.model.pos_from_value(val);
  var x = this.trough.shape.bbox.x + this.trough.shape.stroke.w +
   (1 - frac) * this.len + 0.5 * this.handle.shape.bbox.w;
  var text;
  if (val == this.model.lowest_allowed_value) {
    text = "∞";
  } else {
    if (this.precision !== 0) {
      text = val.toPrecision(this.precision);
    } else {
      text = val.toFixed(0);
    }
  }
  this.context.save();
  this.context.fillStyle = this.colors.trough_stroke.style();
  this.context.textBaseline = 'middle';
  this.context.textAlign = 'center';
  var t_w = this.context.measureText(text).width;
  var t_h = 0.5 * this.track_width;
  var t_y = this.trough.shape.bbox.y
   + 0.5 * this.trough.shape.bbox.h;
  var t_x = x;
  var stroke = new KRStroke(this.colors.handle_stroke,
   this.tick_width, 'round');
  this.context.beginPath();
  this.context.moveTo(x, t_y - t_h - this.tick_len);
  this.context.lineTo(x, t_y - t_h);
  stroke.do(this.context);
  this.context.beginPath();
  this.context.moveTo(x, t_y + t_h);
  this.context.lineTo(x, t_y + t_h + this.tick_len);
  stroke.do(this.context);
  this.context.restore();
}

KRSliderHorizontalView.prototype.render_background = function() {
  //if (this.bg_damaged) {
    /*this.context.clearRect(this.trough.shape.bbox.x,
     this.trough.shape.bbox.y, this.trough.shape.bbox.w,
     this.trough.shape.bbox.h);*/
  //  this.bg_damaged = false;
  //  this.bg_image = this.bg_context.getImageData(0, 0,
  //   this.bg_context.canvas.width, this.bg_context.canvas.height);
  //}
  //this.context.putImageData(this.bg_image,
  // this.dpr * this.bbox.x, this.dpr * this.bbox.y);
}

KRSliderHorizontalView.prototype.render = function() {
  if (this.damaged) {
    var radius;
    var len = SHADOW_LENGTH;
    this.context.save();
    this.background.render(this.context);
    radius = 0.333 * Math.min(this.handle.shape.bbox.w, this.handle.shape.bbox.h);
    this.background.clip(this.context, radius);
    this.context.save();
    this.context.shadowOffsetY = -len;
    this.context.shadowBlur = SHADOW_BLUR;
    this.context.shadowColor
     = this.colors.trough_fill.darken().darken().darken().darken().style();
    this.trough.render(this.context, radius);
    this.context.shadowOffsetY = len;
    this.context.shadowBlur = SHADOW_BLUR;
    this.context.shadowColor
     = this.colors.trough_fill.lighten().lighten().style();
    this.trough.render(this.context, radius);
    this.context.restore();
    var stroke = new KRStroke(new KRColor(10,10,10,1), this.track_width,
     'round');
    var pad = SHADOW_LENGTH;
    var left_x = pad + this.trough.shape.bbox.x + 0.5 * this.handle.shape.bbox.w;
    var right_x = left_x + this.len - 2 * pad;
    var y = this.trough.shape.bbox.y + 0.5 * this.trough.shape.bbox.h;
    this.context.beginPath();
    this.context.moveTo(left_x, y);
    this.context.lineTo(right_x, y);
    this.context.lineCap = 'round';
    stroke.do(this.context);
    this.render_label();
    this.render_ticks('H');
    this.context.save();
    this.context.shadowOffsetY = len;
    this.context.shadowBlur = SHADOW_BLUR;
    this.context.shadowColor
     = this.colors.trough_fill.darken().darken().darken().style();
    var gradient = this.context.createLinearGradient(this.handle.shape.bbox.x,
     this.handle.shape.bbox.y, this.handle.shape.bbox.x, this.handle.shape.bbox.y
     + this.handle.shape.bbox.h);
    gradient.addColorStop(0, this.colors.handle_fill.lighten().lighten().style());
    gradient.addColorStop(0.5, this.colors.handle_fill.style());
    gradient.addColorStop(1, this.colors.handle_fill.darken().darken().darken().darken().style());
    this.handle.shape.fill = new KRFill(gradient);
    this.handle.render(this.context, radius);
    this.context.restore();
    this.context.restore();
    this.damaged = false;
  }
}

KRSliderHorizontalView.prototype.on_mousescroll = function(evt) {
  if (this.expansion_state === this.expansion_states.CONTRACTED) return;
  var evt=window.event || evt;
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    var delta=evt.detail? 40.352 * evt.detail : -evt.wheelDelta;
    evt.preventDefault();
    var delx = delta / 360;
    var new_pos = Math.min(this.len, (Math.max(0,
     this.handle.shape.bbox.x - (this.trough.shape.bbox.x
     + this.trough.shape.stroke.w) - delx)));
    this.model.set_from_pos( 1 - new_pos / this.len);
  }
}

KRSliderHorizontalView.prototype.on_trough_mousedown = function(evt) {
  if (this.expansion_state === this.expansion_states.CONTRACTED) return;
  this.on_handle_mousedown(evt);
}

KRSliderHorizontalView.prototype.on_handle_mousedown = function(evt) {
  if (this.expansion_state === this.expansion_states.CONTRACTED) return;
  evt.stopPropagation();
  evt.preventDefault();
  this.mdown_pos = evt.clientX;
  this.mdown_handle_x = this.handle.shape.bbox.x;
  this.mdown = true;
}

KRSliderHorizontalView.prototype.on_value_view_mousedown = function (evt) {
  evt.stopPropagation();
  evt.preventDefault();
  this.damaged = this.value_view.on_mousedown();
}

KRSliderHorizontalView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.handle.in(coords)) {
    this.on_handle_mousedown(evt);
  } else if (this.trough.in(coords)) {
    this.on_trough_mousedown(evt);
  }
}

KRSliderHorizontalView.prototype.on_mouseup = function(evt) {
  this.mdown = false;
  var coords = this.event_to_canvas(evt);
}

KRSliderHorizontalView.prototype.set_width = function(w) {
  this.bbox.w = w;
  this.trough.shape.bbox.w = w - 2 * this.button_space;
  this.resize();
  this.set_handle();
  this.damage();
}

KRSliderHorizontalView.prototype.on_mousemove = function(evt) {
  if (this.mdown === true && this.len > 0) {
    evt.preventDefault();
    evt.stopPropagation();
    var delx = evt.clientX - this.mdown_pos;
    var new_x = Math.min(this.trough.shape.bbox.x + this.trough.shape.bbox.w
     - this.trough.shape.stroke.w, (Math.max(this.trough.shape.bbox.x
     + this.trough.shape.stroke.w, this.mdown_handle_x + delx)));
    this.model.set_from_pos(1 - (new_x
     - (this.trough.shape.bbox.x + this.trough.shape.stroke.w))/this.len);
  }
}

function KRSliderVerticalGradientsView(parent_view, model, context, bbox, colors, opts) {
  if (opts === undefined) {
    opts = {};
  }
  this.parent_view = parent_view;
  this.scale = this.parent_view.scale;
  this.full_size = [100, 400];
  this.model = model;
  this.model.add_view(this);
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.colors = colors;
  this.initial_touches = [];
  this.initial_handle_y = [];
  this.TRACK_WIDTH = 6;
  this.LINE_WIDTH = 0;
  this.TICK_WIDTH = 1;
  this.TICK_LEN = 4;
  this.bg_canvas = document.createElement('canvas');
  this.bg_context = this.bg_canvas.getContext('2d');
  this.bg_damaged = true;
  var w = this.BBOX.w;
  var button_space = Math.round(w / 1.618);
  this.button_space = button_space;
  this.contracted_h = 2 * button_space;
  this.expanded_h = this.BBOX.h;
  this.expansion_states = { EXPANDED: 0, CONTRACTED: 1 };
  this.expansion_state = this.expansion_states.EXPANDED;
  var background_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y,
   this.BBOX.w, this.BBOX.h);
  var shape = new KRShape(background_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.background = new KRRectangle(shape);
  var vv_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y, this.BBOX.w,
   button_space);
  var trough_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y + button_space,
   w, this.BBOX.h - 2* button_space);
  var button_bbox = new KRBoundBox(this.BBOX.x, trough_bbox.y + trough_bbox.h,
   this.BBOX.w, button_space);
  var s = new KRShape(button_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  var s_on = new KRShape(button_bbox, new KRStroke(
   colors.trough_stroke.compliment(), this.LINE_WIDTH, 'round'),
   new KRFill(colors.trough_fill.compliment()));
  var shape = new KRShape(vv_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  var show_model_label = false;
  if (opts.show_model_label === true) {
    show_model_label = true;
  }
  this.value_view = new KRValueView(this, this.model, this.context,
   new KRRectangle(shape), colors, {always_active: true,
   'show_model_label': show_model_label});
  var shape = new KRShape(trough_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.trough = new KRRectangleView(this, null, context, trough_bbox.copy(),
   colors);
  var handle_line_width = 0;
  w = w - 2 * this.LINE_WIDTH;
  var handle_setup = { w: w, h: (1/1.618) * w };
  var x = this.BBOX.x + 0.5 * this.BBOX.w - 0.5 * handle_setup.w;
  var handle_shape = new KRShape(new KRBoundBox(x, this.BBOX.y,
   w, handle_setup.h),
   new KRStroke(colors.handle_stroke, 0, 'round'),
   new KRFill(colors.handle_fill));
  handle_shape.stroke.on = false;
  this.handle = new KRHandleGradientsView(this,
   new KRRectangle(handle_shape), colors);
  this.resize(this.scale);
  this.set_handle();
}

/*Inherat from slider view base class for common code to Vert and Horiz
 sliders.*/
KRSliderVerticalGradientsView.prototype = Object.create(KRSliderView.prototype);
KRSliderVerticalGradientsView.prototype.constructor = KRSliderVerticalGradientsView;

KRSliderVerticalGradientsView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.trough.rect.shape.stroke.color = this.colors.trough_stroke.copy();
  this.trough.rect.shape.fill.color = this.colors.trough_fill.darken();
  this.value_view.set_colors(colors);
  this.handle.set_colors(colors);
  this.background.shape.stroke.color = this.colors.trough_stroke.copy();
  this.background.shape.fill.color = this.colors.trough_fill.copy();
  this.bg_damaged = true;
  this.trough.damage();
  this.damage();
}

KRSliderVerticalGradientsView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  this.rel_size = this.BBOX.w / this.full_size[0];
  this.bbox = this.BBOX.scale(this.scale);
  var shadow_length = SHADOW_LENGTH * this.rel_size;
  var pad = 4 * shadow_length;
  var w = this.BBOX.w;
  var button_space = w / 1.618;
  this.button_space = button_space;
  this.background.set_bbox(this.BBOX.copy());
  var trough_bbox = new KRBoundBox(pad + this.BBOX.x,
   pad + this.BBOX.y + this.button_space,
   w - 2 * pad, this.BBOX.h - button_space - 2 * pad);
  var vv_bbox = new KRBoundBox(pad + this.BBOX.x,
   pad + this.BBOX.y, this.BBOX.w - 2 * pad, button_space - 2 * pad);
  this.trough.set_bbox(trough_bbox);
  this.value_view.set_bbox(vv_bbox);
  this.handle.set_bbox(new KRBoundBox(
   this.trough.rect.BBOX.x + this.rel_size * this.trough.rect.W,
   this.trough.rect.BBOX.y - shadow_length + this.rel_size * this.trough.rect.W,
   this.trough.rect.BBOX.w - 2 * this.rel_size * this.trough.rect.W,
   this.trough.rect.BBOX.h + shadow_length - 2 * this.rel_size * this.trough.rect.W));
  this.resize(this.scale);
  this.set_handle();
}

KRSliderVerticalGradientsView.prototype.in = function(coordinates) {
  
}

KRSliderVerticalGradientsView.prototype.damage = function() {
  this.damaged = true;
  this.parent_view.damage();
}

KRSliderVerticalGradientsView.prototype.resize = function(scale) {
  if (scale === undefined) scale = this.parent_view.scale;
  if (this.rel_size === undefined) this.rel_size = this.parent_view.rel_size;
  if (this.rel_size === undefined) this.rel_size = 1;
  this.scale = scale;
  this.track_width = this.rel_size * this.TRACK_WIDTH * scale;
  this.tick_width = this.rel_size * this.TICK_WIDTH * scale;
  this.tick_len = this.rel_size * this.TICK_LEN * scale;
  this.bbox = this.BBOX.scale(scale);
  this.trough.resize(scale, this.rel_size);
  this.handle.resize(scale, this.rel_size);
  this.background.resize(scale, this.rel_size);
  this.value_view.resize(scale, this.rel_size);
  this.dpr = window.devicePixelRatio;
  this.bg_canvas.style.width = this.bbox.w + "px";
  this.bg_canvas.style.height = this.bbox.h + "px";
  this.bg_canvas.width = this.dpr * this.bbox.w;
  this.bg_canvas.height = this.dpr * this.bbox.h;
  this.bg_context.scale(this.dpr, this.dpr);
  this.len = Math.max(0, this.trough.rect.shape.bbox.h
   - this.handle.inner.shape.bbox.h);
  this.trough.damage();
  this.damage();
}

KRSliderVerticalGradientsView.prototype.set_handle = function() {
  this.handle.set(this.model.pos);
  this.trough.damage();
  this.damage();
}

KRSliderVerticalGradientsView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

KRSliderVerticalGradientsView.prototype.draw_tick = function(val, pos) {
  return;
  var frac = this.model.pos_from_value(val);
  var y = this.trough.rect.shape.stroke.w + this.trough.rect.shape.bbox.y
    + frac * this.len + 0.5 * this.handle.inner.shape.bbox.h;
  var text;
  if (val == this.model.lowest_allowed_value) {
    text = "∞";
  } else {
    text = val.toPrecision(2);
  }
  this.context.save();
  this.context.fillStyle = this.colors.trough_stroke.style();
  this.context.font = 'normal ' + 0.125 * this.trough.rect.shape.bbox.w + 'pt play';
  this.context.textBaseline = 'middle';
  this.context.textAlign = 'center';
  var t_x = this.trough.rect.shape.bbox.x + 0.5 * this.trough.rect.shape.bbox.w;
  var t_y = y;
  var stroke = new KRStroke(this.colors.trough_stroke, this.tick_width,
   'round');
  this.context.beginPath();
  this.context.moveTo(this.trough.rect.shape.bbox.x + 0.5 * this.trough.rect.shape.bbox.w
  - 0.5 * this.track_width - this.tick_len, y);
  this.context.lineTo(this.trough.rect.shape.bbox.x + 0.5 * this.trough.rect.shape.bbox.w
  - 0.5 * this.track_width, y);
  stroke.do(this.context);
  this.context.beginPath();
  this.context.moveTo(this.trough.rect.shape.bbox.x + 0.5 * this.trough.rect.shape.bbox.w
  + 0.5 * this.track_width, y);
  this.context.lineTo(this.trough.rect.shape.bbox.x + 0.5 * this.trough.rect.shape.bbox.w
  + 0.5 * this.track_width + this.tick_len, y);
  stroke.do(this.context);
  this.context.restore();
}

KRSliderVerticalGradientsView.prototype.render_label = function(context) {
  context.save();
  context.textBaseline = 'middle';
  context.textAlign = 'center';
  context.font = 'normal ' + 0.25 * Math.round(this.trough.rect.shape.bbox.w)
    + 'px play';
  var w = context.measureText(this.model.label).width;
  var h = parseInt(context.font, 10);
  var fit = true;
  while (w > this.trough.rect.shape.bbox.h) {
    h--;
    if (h <= 0) {
      fit = false;
      break;
    }
    context.font = 'normal ' + h + 'pt play';
    w = context.measureText(this.model.label).width;
  }
  if (fit) {
    var x = this.trough.rect.shape.bbox.x + 0.75 * this.trough.rect.shape.bbox.w;
    var y = this.trough.rect.shape.bbox.y + 0.5 * this.trough.rect.shape.bbox.h;
    context.translate(x, y);
    context.rotate(-Math.PI/2);
    this.fill_text_inset(this.model.label, this.colors.trough_fill,
     0, 0, context, 'V');
  }
  context.restore();
}

KRSliderVerticalGradientsView.prototype.render_background = function() {
  //if (this.bg_damaged) {
    //this.context.clearRect(this.bbox.x, this.bbox.y, this.bbox.w, this.bbox.h);
    //this.trough.shape.fill.color = new KRColor(0,0,0,1);
   //this.render_label(this.context);
    this.bg_damaged = false;
 // }
}

KRSliderVerticalGradientsView.prototype.render = function() {
  if (this.damaged) {
    var radius;
    var len = window.devicePixelRatio * this.scale * SHADOW_LENGTH;
    this.context.save();
    this.background.render(this.context);
    radius = 0.2 * Math.min(this.handle.inner.shape.bbox.w,
     this.handle.inner.shape.bbox.h);
    this.background.clip(this.context, radius);
    this.context.save();
    this.context.shadowOffsetY = -len;
    this.context.shadowBlur = window.devicePixelRatio * this.scale * SHADOW_BLUR;
    this.context.shadowColor
     = this.colors.trough_fill.darken().darken().darken().darken().style();
    this.trough.damage();
    this.trough.render(radius);
    this.context.shadowOffsetY = len;
    this.context.shadowBlur = window.devicePixelRatio * this.scale * SHADOW_BLUR;
    this.context.shadowColor
     = this.colors.trough_fill.lighten().lighten().style();
    this.trough.damage();
    this.trough.render(radius);
    this.context.restore();
    this.context.save();
    var stroke = new KRStroke(new KRColor(10,10,10,1), this.track_width,
     'round');
    var x = this.trough.rect.shape.bbox.x + 0.5 * this.trough.rect.shape.bbox.w;
    var top_y = this.trough.rect.shape.stroke.w + this.trough.rect.shape.bbox.y
    + 0.5 * this.handle.inner.shape.bbox.h;
    var bottom_y = top_y + this.len;
    this.context.beginPath();
    this.context.moveTo(x, top_y);
    this.context.lineTo(x, bottom_y);
    this.context.lineCap = 'round';
    stroke.do(this.context);
    this.context.restore();
    this.render_ticks('V');
    this.context.save();
    this.context.shadowOffsetY = len;
    this.context.shadowBlur = window.devicePixelRatio * SHADOW_BLUR;
    this.context.shadowColor =
     this.colors.trough_fill.darken().darken().darken().style();
    this.handle.render(this.context, radius);
    this.context.restore();
    this.context.restore();
    this.value_view.damage
    this.value_view.render(this.context);
    this.context.restore();
   this.damaged = false;
  }
}

KRSliderVerticalGradientsView.prototype.on_mousescroll = function(evt) {
  if (this.contracted) return;
  var evt=window.event || evt;
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    var delta=evt.detail? -40.352 * evt.detail : evt.wheelDelta;
    evt.preventDefault();
    var dely = delta / 360;
    var new_pos = Math.min(this.trough.rect.shape.bbox.h, (Math.max(0,
     this.trough.rect.shape.bbox.h * this.model.pos - dely)));
    this.model.set_from_pos(new_pos / this.trough.rect.shape.bbox.h);
  }
}

KRSliderVerticalGradientsView.prototype.on_touchstart = function(evt) {
  kr.debug('touchstart in VerGradiantsView');
  var touches = evt.changedTouches;
  for (var i = 0, len = touches.length; i < len; i++) {
    var mdown_handle_y = this.model.pos
      * (this.handle.rect.shape.bbox.h - this.handle.inner.shape.bbox.h)
      + this.handle.rect.shape.bbox.y;
    this.initial_handle_y.push(mdown_handle_y);
    this.initial_touches.push(this.copy_touch(touches[i]));
  }
}

KRSliderVerticalGradientsView.prototype.on_touchend = function(evt) {
  kr.debug('touchend in VerGradiantsView');
  var touches = evt.changedTouches;
  for (var i = 0; i < touches.length; i++) {
    var touch = touches[i];
    var idx = this.initial_touch_index_by_id(touch.identifier);
    while (idx >= 0) {
       this.initial_handle_y.splice(idx, 1);
       this.initial_touches.splice(idx, 1);
       idx = this.initial_touch_index_by_id(touch.identifier);
    }
  }
}

KRSliderVerticalGradientsView.prototype.on_touchmove = function(evt) {
  kr.debug('touchmove in VerGradiantsView');
  var touches = evt.changedTouches;
  for (var i = 0; i < touches.length; i++) {
    var touch = touches[i];
    var idx = this.initial_touch_index_by_id(touch.identifier);
    kr.debug('touchmove ' + idx);
    if (idx >= 0) {
      evt.preventDefault();
      evt.stopPropagation();
      var dely = touch.pageY - this.initial_touches[idx].pageY;
      var new_y = Math.min(this.handle.rect.shape.bbox.y
          + this.handle.rect.shape.bbox.h - this.handle.inner.shape.bbox.h,
          (Math.max(this.handle.rect.shape.bbox.y,
                    this.initial_handle_y[idx] + dely)));
      this.model.set_from_pos((new_y - this.handle.rect.shape.bbox.y)
          / (this.handle.rect.shape.bbox.h - this.handle.inner.shape.bbox.h));
    } else {
      kr.debug("can't figure out which touch to continue");
    }
  }
}

KRView.prototype.initial_touch_index_by_id = function(idToFind) {
  for (var i = 0; i < this.initial_touches.length; i++) {
    var id = this.initial_touches[i].identifier;
    if (id == idToFind) {
      return i;
    }
  }
  return -1;
}

KRView.prototype.copy_touch = function(touch) {
  return {
   identifier: touch.identifier,
   pageX: touch.pageX,
   pageY: touch.pageY
  };
}

KRSliderVerticalGradientsView.prototype.on_trough_mousedown = function(evt) {
  this.on_handle_mousedown(evt);
}

KRSliderVerticalGradientsView.prototype.on_handle_mousedown = function(evt) {
  evt.stopPropagation();
  evt.preventDefault();
  this.mdown_pos = evt.clientY;
  this.mdown_handle_y = this.model.pos
   * (this.handle.rect.shape.bbox.h - this.handle.inner.shape.bbox.h)
   + this.handle.rect.shape.bbox.y;
  this.mdown = true;
}

KRSliderVerticalGradientsView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.handle.in(coords)) {
    this.on_handle_mousedown(evt);
  } else if (this.trough.rect.in(coords)) {
    this.on_trough_mousedown(evt);
  }
}

KRSliderVerticalGradientsView.prototype.on_mouseup = function(evt) {
  this.mdown = false;
  var coords = this.event_to_canvas(evt);
}

KRSliderVerticalGradientsView.prototype.on_mousemove = function(evt) {
  if (this.mdown === true) {
    evt.preventDefault();
    evt.stopPropagation();
    var dely = evt.clientY - this.mdown_pos;
    var new_y = Math.min(this.handle.rect.shape.bbox.y
     + this.handle.rect.shape.bbox.h - this.handle.inner.shape.bbox.h,
     (Math.max(this.handle.rect.shape.bbox.y,
     this.mdown_handle_y + dely)));
    this.model.set_from_pos((new_y - this.handle.rect.shape.bbox.y)
     / (this.handle.rect.shape.bbox.h - this.handle.inner.shape.bbox.h));
  }
}

function KRRectangleView(parent_view, crate, context, bbox, colors, opts) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.parent_view = parent_view;
  this.crate = crate;
  this.context = context;
  this.scale = parent_view.scale;
  this.colors = colors;
  this.setting_modes = { FLAT: 1, INSET: 2, OUTSET: 3 };
  this.setting_mode = this.setting_modes.FLAT;
  this.rounded = false;
  if (opts !== undefined) {
    if (opts.rounded === true) {
      this.rounded = true;
    }
    if (opts.shadow_length !== undefined) {
      this.SHADOW_LENGTH = opts.shadow_length;
    }
  }
  if (this.SHADOW_LENGTH === undefined) {
      this.SHADOW_LENGTH = SHADOW_LENGTH;
  }
  var bg_shape = new KRShape(new KRBoundBox(0,0,1,1),
   new KRStroke(colors.trough_stroke.copy(), 0, 'round'),
   new KRFill(colors.trough_fill.copy()));
  this.rect = new KRRectangle(bg_shape);
  this.damaged = false;
}

KRRectangleView.prototype = Object.create(KRView.prototype);
KRRectangleView.prototype.constructor = KRRectangleView;

KRRectangleView.prototype.set_bbox = function(bbox) {
  var bb = bbox.copy();
  if (this.setting_mode !== this.setting_modes.FLAT) {
    bb.y = bb.y + this.SHADOW_LENGTH;
    bb.h = bb.h - 2 * this.SHADOW_LENGTH;
  }
  this.BBOX = bbox.copy();
  this.bbox = this.BBOX.scale(this.scale);
  this.rect.set_bbox(bb);
  this.resize(this.scale);
  this.damage();
}

KRRectangleView.prototype.set_setting_mode = function(name) {
  this.setting_mode = this.setting_modes[name] || this.setting_modes.FLAT;
}

KRRectangleView.prototype.render = function(r) {
  if (this.damaged) {
  var radius;
  var len = window.devicePixelRatio * this.SHADOW_LENGTH * this.scale;// * this.parent_view.rel_size;
  if (r !== undefined && typeof(r) === 'number') {
    radius = r;
  } else if (this.rounded || r === true) {
    radius = 0.2 * Math.min(this.rect.shape.bbox.w,
      this.rect.shape.bbox.h);
  }
  this.context.save();
  if (this.setting_mode !== this.setting_modes.FLAT) {
    this.context.shadowOffsetY = -len;
    this.context.shadowBlur = window.devicePixelRatio * this.scale * SHADOW_BLUR;
    if (this.setting_mode === this.setting_modes.INSET) {
      this.context.shadowColor
        = this.rect.shape.fill.color.darken().darken().darken().darken().style();
      this.rect.render(this.context, radius);
    }
  }
  if (this.setting_mode !== this.setting_modes.FLAT) {
    this.context.shadowOffsetY = len;
    this.context.shadowBlur = window.devicePixelRatio * this.scale * SHADOW_BLUR;
    if (this.setting_mode === this.setting_modes.INSET) {
      this.context.shadowColor
        = this.rect.shape.fill.color.lighten().lighten().lighten().lighten().style();
    } else if (this.setting_mode === this.setting_modes.OUTSET) {
      this.context.shadowColor
        = this.rect.shape.fill.color.darken().darken().darken().darken().style();
    }
  }
  this.rect.render(this.context, radius);
  this.context.restore();
  this.damaged = false;
  }
}

KRRectangleView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.rect.shape.fill.color = colors.trough_fill.copy();
  this.rect.shape.stroke.color = colors.trough_stroke.copy();
  this.damage();
}

KRRectangleView.prototype.damage = function() {
  this.damaged = true;
}

KRRectangleView.prototype.resize = function(scale) {
  this.scale = scale;
  this.rect.resize(scale);
}

function KRHandleRectangleView(rect) {
  this.rect = rect;
}

KRHandleRectangleView.prototype.resize = function(scale) {
  this.rect.resize(scale);
}

KRHandleRectangleView.prototype.in = function(coords) {
  return this.rect.in(coords);
}

KRHandleRectangleView.prototype.set_colors = function(colors) {
  this.rect.shape.fill.color = colors.handle_fill.copy();
  this.rect.shape.stroke.color = colors.handle_stroke.copy();
}

KRHandleRectangleView.prototype.render = function(context) {
  return this.rect.render(context, true);
}

function KRSliderVerticalView(parent_view, model, context, bbox, colors) {
  this.parent_view = parent_view;
  this.scale = this.parent_view.scale;
  this.full_size = [100, 400];
  this.model = model;
  this.model.add_view(this);
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.colors = colors;
  this.LINE_WIDTH = 1;
  this.TICK_WIDTH = 1;
  this.TICK_LEN = 4;
  this.line_width = this.LINE_WIDTH * this.scale;
  this.bg_canvas = document.createElement('canvas');
  this.bg_context = this.bg_canvas.getContext('2d');
  this.bg_damaged = true;
  var w = this.BBOX.w;
  var button_space = w / 1.618;
  this.button_space = button_space;
  this.contracted_h = 2 * button_space;
  this.expanded_h = this.BBOX.h;
  this.expansion_states = { EXPANDED: 0, CONTRACTED: 1 };
  this.expansion_state = this.expansion_states.EXPANDED;
  var vv_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y, this.BBOX.w,
      button_space);
  var trough_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y,
      w, this.BBOX.h);
  var button_bbox = new KRBoundBox(this.BBOX.x, trough_bbox.y + trough_bbox.h,
      this.BBOX.w, button_space);
  var s = new KRShape(button_bbox, new KRStroke(colors.trough_stroke,
        this.LINE_WIDTH,
        'round'), new KRFill(colors.trough_fill));
  var s_on = new KRShape(button_bbox, new KRStroke(
        colors.trough_stroke.compliment(), this.LINE_WIDTH, 'round'),
      new KRFill(colors.trough_fill.compliment()));
  this.mute_button = new KRPushButton(this, 'M', new KRRectangle(s),
      new KRRectangle(s_on));
  var shape = new KRShape(vv_bbox, new KRStroke(colors.trough_stroke,
        this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  var shape_on = new KRShape(vv_bbox, new KRStroke(
        colors.trough_stroke.compliment(), this.LINE_WIDTH, 'round'),
      new KRFill(colors.trough_fill.compliment()));
  this.value_view = new KRPushButtonValueView(this.model, this, 'VALUE',
      new KRRectangle(shape), new KRRectangle(shape_on),
      KRPushButtonTypes.EXPAND_CONTRACT);
  var shape = new KRShape(trough_bbox, new KRStroke(colors.trough_stroke,
        this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.trough = new KRRectangle(shape);
  var handle_line_width = 0;
  w = w - 2 * this.LINE_WIDTH;
  var handle_setup = { w: w, h: (1/1.618) * w };
  var x = this.BBOX.x + 0.5 * this.BBOX.w - 0.5 * handle_setup.w;
  var handle_shape = new KRShape(new KRBoundBox(x, this.BBOX.y,
        w, handle_setup.h),
      new KRStroke(colors.handle_stroke, handle_line_width, 'round'),
      new KRFill(colors.handle_fill));
  handle_shape.stroke.on = false;
  this.handle = new KRHandleRectangleView(new KRRectangle(handle_shape));
  this.len = this.trough.shape.bbox.h - this.handle.rect.shape.bbox.h;
  this.resize(this.scale);
  this.set_handle();
}

/*Inherat from slider view base class for common code to Vert and Horiz
 *  sliders.*/
KRSliderVerticalView.prototype = Object.create(KRSliderView.prototype);
KRSliderVerticalView.prototype.constructor = KRSliderVerticalView;

KRSliderVerticalView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.trough.shape.stroke.color = this.colors.trough_stroke;
  this.trough.shape.fill.color = this.colors.trough_fill;
  this.value_view.set_colors(colors);
  this.mute_button.set_colors(colors);
  this.handle.set_colors(colors);
  this.bg_damaged = true;
  this.damage();
}

KRSliderVerticalView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  this.rel_size = this.BBOX.w / this.full_size[0];
  this.bbox = this.BBOX.scale(this.scale);
  this.LINE_WIDTH = 1;
  this.TICK_WIDTH = 1;
  this.TICK_LEN = 4;
  var w = this.BBOX.w;
  var button_space = w / 1.618;
  this.button_space = button_space;
  this.contracted_h = 2 * button_space;
  this.expanded_h = this.BBOX.h;
  var vv_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y, this.BBOX.w,
      button_space);
  var trough_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y + button_space,
      w, this.BBOX.h - 2 * button_space);
  var button_bbox = new KRBoundBox(this.BBOX.x, trough_bbox.y + trough_bbox.h,
      this.BBOX.w, button_space);
  this.mute_button.set_bbox(button_bbox);
  this.trough.set_bbox(trough_bbox);
  this.value_view.set_bbox(vv_bbox);
  var handle_line_width = 0;
  w = w - 2 * this.LINE_WIDTH;
  var handle_setup = { w: w, h: (1/1.618) * w };
  var x = this.BBOX.x + 0.5 * this.BBOX.w - 0.5 * handle_setup.w;
  var handle_shape = new KRShape(new KRBoundBox(x, this.BBOX.y,
        w, handle_setup.h),
      new KRStroke(this.colors.handle_stroke, handle_line_width, 'round'),
      new KRFill(this.colors.handle_fill));
  handle_shape.stroke.on = false;
  this.handle = new KRHandleRectangleView(new KRRectangle(handle_shape));
  this.len = this.trough.shape.bbox.h - this.handle.rect.shape.bbox.h;
  this.resize(this.scale);
  this.set_handle();
}

KRSliderVerticalView.prototype.damage = function() {
  this.damaged = true;
  this.parent_view.damage();
}

KRSliderVerticalView.prototype.resize = function(scale) {
  if (scale === undefined) scale = this.parent_view.scale;
  this.scale = scale;
  this.tick_width = this.rel_size * this.TICK_WIDTH * scale;
  this.tick_len = this.rel_size * this.TICK_LEN * scale;
  this.line_width = this.rel_size * this.LINE_WIDTH * scale;
  this.bbox = this.BBOX.scale(scale);
  this.trough.resize(scale);
  this.handle.resize(scale);
  this.mute_button.resize(scale);
  this.value_view.resize(scale);
  this.dpr = window.devicePixelRatio;
  this.bg_canvas.style.width = Math.round(this.bbox.w) + "px";
  this.bg_canvas.style.height = Math.round(this.bbox.h) + "px";
  this.bg_canvas.width = Math.round(this.dpr * this.bbox.w);
  this.bg_canvas.height = Math.round(this.dpr * this.bbox.h);
  this.bg_context.scale(this.dpr, this.dpr);
  this.len = Math.max(0, this.trough.shape.bbox.h - this.handle.rect.shape.bbox.h);
  this.bg_damaged = true;
  this.damage();
}

KRSliderVerticalView.prototype.set_handle = function() {
  var ratio = this.model.pos;
  var y = ratio * this.len + this.trough.shape.bbox.y;
  this.handle.rect.FULL_SIZE_BBOX.y = y / this.scale;
  this.handle.rect.resize(this.scale);
  this.damage();
}

KRSliderVerticalView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

KRSliderVerticalView.prototype.draw_tick = function(val) {
  var frac = this.model.pos_from_value(val);
  var y = this.trough.shape.bbox.y - this.bbox.y + frac * this.len
    + 0.5 * this.handle.rect.shape.bbox.h;
  var text;
  if (val == this.model.lowest_allowed_value) {
    text = "∞";
  } else {
    text = val.toPrecision(2);
  }
  this.bg_context.save();
  this.bg_context.fillStyle = this.colors.trough_stroke.style();
  this.bg_context.font = 'normal ' + this.rel_size * this.scale * 10
    + 'pt play';
  this.bg_context.textBaseline = 'middle';
  this.bg_context.textAlign = 'center';
  var ret = 0;
  var t_w = this.bg_context.measureText(text).width;
  var t_h = parseInt(this.bg_context.font, 10);
  var t_x = this.trough.shape.bbox.x - this.bbox.x
    + 0.5 * this.trough.shape.bbox.w;
  var t_y = y;
  if (t_w <= this.trough.shape.bbox.w) {
    var stroke = this.trough.shape.stroke.copy();
    stroke.w = this.tick_width;
    this.bg_context.beginPath();
    this.bg_context.moveTo(this.trough.shape.bbox.x - this.bbox.x
        + this.trough.shape.stroke.w, y);
    this.bg_context.lineTo(this.trough.shape.bbox.x
        - this.bbox.x + this.trough.shape.stroke.w + this.tick_len, y);
    stroke.do(this.bg_context);
    this.bg_context.beginPath();
    this.bg_context.moveTo(this.trough.shape.bbox.x - this.bbox.x
        + this.trough.shape.bbox.w - this.trough.shape.stroke.w
        - this.tick_len, y);
    this.bg_context.lineTo(this.trough.shape.bbox.x - this.bbox.x
        + this.trough.shape.bbox.w, y);
    stroke.do(this.bg_context);
    if (t_w <= this.trough.shape.bbox.w) {
      if ((t_y + 0.5 * t_h) < (this.pos_edge)
          && ((t_y - 0.5 * t_h) > this.last_drawn_pos)) {
        this.fill_text_inset(text, this.colors.trough_fill, t_x, t_y,
         this.bg_context);
        this.last_drawn_pos = t_y + 0.5 * t_h;
        ret = 1
      }
    }
  }
  this.bg_context.restore();
  return ret;
}

KRSliderVerticalView.prototype.render_label = function(context) {
  context.save();
  context.textBaseline = 'middle';
  context.textAlign = 'center';
  context.font = 'normal ' + Math.round(this.trough.shape.bbox.w)
    + 'pt play';
  var w = context.measureText(this.model.label).width;
  var h = parseInt(context.font, 10);
  var fit = true;
  while (w > this.trough.shape.bbox.h) {
    h--;
    if (h <= 0) {
      fit = false;
      break;
    }
    context.font = 'normal ' + h + 'pt play';
    w = context.measureText(this.model.label).width;
  }
  if (fit) {
    var x = this.trough.shape.bbox.x + 0.5 * this.trough.shape.bbox.w;
    var y = this.trough.shape.bbox.y + 0.5 * this.trough.shape.bbox.h;
    context.translate(x, y);
    context.rotate(-Math.PI/2);
    this.fill_text_inset(this.model.label, this.colors.trough_fill,
     0, 0, context, 'V');
  }
  context.restore();
}

KRSliderVerticalView.prototype.render_background = function() {
  if (this.bg_damaged) {
    this.bg_context.clearRect(0, 0, this.bg_context.canvas.width,
        this.bg_context.canvas.height);
    this.render_label();
    this.trough.render(this.bg_context, {x: this.bbox.x,
      y: this.bbox.y});
    this.render_ticks('V');
    this.bg_damaged = false;
    this.bg_image = this.bg_context.getImageData(0, 0,
        this.bg_context.canvas.width, this.bg_context.canvas.height);
  }
  this.context.putImageData(this.bg_image, this.dpr * this.bbox.x, this.dpr * this.bbox.y);
}

KRSliderVerticalView.prototype.render = function() {
  if (this.damaged) {
    this.render_background();
    this.handle.render(this.context);
    this.mute_button.render(this.context);
    this.value_view.render(this.context);
    this.damaged = false;
  }
}

KRSliderVerticalView.prototype.on_mousescroll = function(evt) {
  if (this.contracted) return;
  var evt=window.event || evt;
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    var delta=evt.detail? -40.352 * evt.detail : evt.wheelDelta;
    evt.preventDefault();
    var dely = delta / 360;
    var new_pos = Math.min(this.len, (Math.max(0,
            this.handle.rect.shape.bbox.y - this.trough.shape.bbox.y - dely)));
    this.model.set_from_pos(new_pos / this.len);
  }
}

KRSliderVerticalView.prototype.on_trough_mousedown = function(evt) {
  if (this.contracted) return;
  this.on_handle_mousedown(evt);
}

KRSliderVerticalView.prototype.on_handle_mousedown = function(evt) {
  if (this.contracted) return;
  evt.stopPropagation();
  evt.preventDefault();
  this.mdown_pos = evt.clientY;
  this.mdown_handle_y = this.handle.rect.shape.bbox.y;
  this.mdown = true;
}

KRSliderVerticalView.prototype.on_value_view_mousedown = function (evt) {
  evt.stopPropagation();
  evt.preventDefault();
  this.damaged = this.value_view.on_mousedown();
}

KRSliderVerticalView.prototype.on_mute_button_mousedown = function (evt) {
  evt.stopPropagation();
  evt.preventDefault();
  this.damaged = this.mute_button.on_mousedown();
}

KRSliderVerticalView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.handle.in(coords)) {
    this.on_handle_mousedown(evt);
  } else if (this.trough.in(coords)) {
    this.on_trough_mousedown(evt);
  }
  if (this.mute_button.in(coords)) {
    this.on_mute_button_mousedown(evt);
  } else if (this.value_view.in(coords)) {
    this.on_value_view_mousedown(evt);
  }
}

KRSliderVerticalView.prototype.on_mouseup = function(evt) {
  this.mdown = false;
  var coords = this.event_to_canvas(evt);
  this.damaged = this.mute_button.on_mouseup(this.mute_button.in(coords));
  this.damaged = this.damaged
    || this.value_view.on_mouseup(this.value_view.in(coords));
}

KRSliderVerticalView.prototype.set_height = function(h) {
  this.bbox.h = h;
  this.trough.shape.bbox.h = h - 2 * this.button_space;
  this.value_view.rect.shape.bbox.y = this.trough.shape.bbox.y
    + this.trough.shape.bbox.h;
  this.resize(this.parent_view.scale);
  this.set_handle();
  this.damaged = true;
}

KRSliderVerticalView.prototype.on_mousemove = function(evt) {
  if (this.mdown === true) {
    evt.preventDefault();
    evt.stopPropagation();
    var dely = evt.clientY - this.mdown_pos;
    var new_y = Math.min(this.trough.shape.bbox.y + this.len,
        (Math.max(this.trough.shape.bbox.y,
                  this.mdown_handle_y + dely)));
    this.model.set_from_pos((new_y - this.trough.shape.bbox.y)/this.len);
  }
}

function KRColor(r, g, b, a) {
  this.r = r;
  this.g = g;
  this.b = b;
  this.a = a;
}

KRColor.prototype.style = KRColor.prototype.to_string = function() {
  return "rgba( " + this.r + ", " + this.g + ", " + this.b + ", " + this.a + ")";
}

KRColor.prototype.compliment = function() {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  hsv[0] = (hsv[0] + 0.5) % 1.0;
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.change_hue = function(del) {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  var h = hsv[0] + del;
  h = h >= 0 ? h % 1.0 : h + 1.0;
  hsv[0] = h;
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.change_s = function(del) {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  hsv[1] = Math.min(1, Math.max(0, hsv[1] + del));
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.change_v = function(del) {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  hsv[2] = Math.min(1, Math.max(0, hsv[2] + del));
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.lighten = function() {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  hsv[2] = Math.min((hsv[2] * 1.25), 1);;
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.darken = function() {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  hsv[2] = 0.75 * hsv[2];
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.add_saturation = function() {
  var hsv = rgb2hsv(this.r, this.g, this.b);
  hsv[1] = Math.min((hsv[1] * 1.25), 1);;
  var rgb = hsv2rgb(hsv[0], hsv[1], hsv[2]);
  return new KRColor(rgb[0], rgb[1], rgb[2], this.a);
}

KRColor.prototype.interpolate = function(color, percent) {
  var rgb = [
   Math.round(percent * (color.r - this.r) + this.r),
   Math.round(percent * (color.g - this.g) + this.g),
   Math.round(percent * (color.b - this.b) + this.b) ];
  var a = percent * (color.a - this.a) + this.a;
  return new KRColor(rgb[0], rgb[1], rgb[2], a);
}

KRColor.prototype.triads = function() {
  var ret = [];
  ret[ret.length] = this.copy();
  var hsv = rgb2hsv(this.r, this.g, this.b);
  var hue = (hsv[0] + 0.333) % 1.0;
  var rgb = hsv2rgb(hue, hsv[1], hsv[2]);
  ret[ret.length] = new KRColor(rgb[0], rgb[1], rgb[2], this.a);
  var hue = (hsv[0] + 0.666) % 1.0;
  var rgb = hsv2rgb(hue, hsv[1], hsv[2]);
  ret[ret.length] = new KRColor(rgb[0], rgb[1], rgb[2], this.a);
  return ret;
}

KRColor.prototype.tetrads = function() {
  var ret = [];
  ret[ret.length] = this.copy();
  var hsv = rgb2hsv(this.r, this.g, this.b);
  var hue = (hsv[0] + 0.25) % 1.0;
  var rgb = hsv2rgb(hue, hsv[1], hsv[2]);
  ret[ret.length] = new KRColor(rgb[0], rgb[1], rgb[2], this.a);
  var hue = (hsv[0] + 0.5) % 1.0;
  var rgb = hsv2rgb(hue, hsv[1], hsv[2]);
  ret[ret.length] = new KRColor(rgb[0], rgb[1], rgb[2], this.a);
  var hue = (hsv[0] + 0.75) % 1.0;
  var rgb = hsv2rgb(hue, hsv[1], hsv[2]);
  ret[ret.length] = new KRColor(rgb[0], rgb[1], rgb[2], this.a);
  return ret;
}

function KRComplimentsColorScheme(c) {
  this.seq_num = 0;
  if (c === undefined) {
    c = KRColor.random(1.0);
  }
  this.colors = [c.copy(), c.compliment()];
}

KRComplimentsColorScheme.prototype.next = function() {
  return this.colors[this.seq_num++ % this.colors.length];
}

function KRMonoColorScheme(c) {
  if (c !== undefined) {
    this.color = c.copy();
  } else {
    this.color = KRColor.random(1.0);
  }
}

KRMonoColorScheme.prototype.next = function() {
  return this.color.copy();
}

function KRTriadsColorScheme(c) {
  this.seq_num = 0;
  if (c === undefined) {
    c = KRColor.random(1.0);
  }
  this.colors = c.triads();
}

KRTriadsColorScheme.prototype.next = function() {
  return this.colors[this.seq_num++ % this.colors.length];
}

function KRTetradColorScheme(c) {
  this.seq_num = 0;
  if (c === undefined) {
    c = KRColor.random(1.0);
  }
  this.colors = c.tetrads();
}

KRTetradColorScheme.prototype.next = function() {
  return this.colors[this.seq_num++ % this.colors.length];
}

function KRRainbowColorScheme(a) {
  this.seq_num = 0;
  this.alpha = a !== undefined ? a : 1.0;
  this.colors = [];
  this.colors[this.colors.length] = new KRColor(255, 0, 0, this.alpha);
  this.colors[this.colors.length] = new KRColor(255, 165, 0, this.alpha)
  this.colors[this.colors.length] = new KRColor(255, 255, 0, this.alpha)
  this.colors[this.colors.length] = new KRColor(0, 255, 0, this.alpha)
  this.colors[this.colors.length] = new KRColor(0, 0, 255, this.alpha)
  this.colors[this.colors.length] = new KRColor(165, 0, 255, this.alpha)
  this.colors[this.colors.length] = new KRColor(255, 0, 255, this.alpha)
}

KRRainbowColorScheme.prototype.next = function() {
  return this.colors[this.seq_num++ % this.colors.length];
}

KRColor.random = function(a) {
  if (a === undefined) a = Math.random();
  return new KRColor(Math.round(255 * Math.random()),
   Math.round(255 * Math.random()),
   Math.round(255 * Math.random()), a);
}

KRColor.random_light = function(a) {
  if (a === undefined) a = Math.random();
  var hue, sat, val;
  hue = Math.random();
  sat = 0.5 * (1 + Math.random());
  val = 0.5 * (1 + Math.random());
  var rgb = hsv2rgb(hue, sat, val);
  return new KRColor(rgb[0], rgb[1], rgb[2], a);
}

KRColor.prototype.copy = function() {
  return new KRColor(this.r, this.g, this.b, this.a);
}

KRColor.prototype.copy_with_alpha = function(new_alpha) {
  if (new_alpha === undefined) new_alpha = this.a;
  return new KRColor(this.r, this.g, this.b, new_alpha);
}

KRColor.copy_color_with_alpha = function(color, new_alpha) {
  if (new_alpha === undefined) new_alpha = color.a;
  return new KRColor(color.r, color.g, color.b, new_alpha);
}

function KRFill(fill_obj) {
  this.fill_methods = { COLOR: 1, GRADIENT: 2 };
  if (fill_obj === undefined) {
    this.on = false;
  } else {
    this.on = true;
    if (fill_obj instanceof KRColor) {
      this.color = fill_obj;
      this.fill_method = this.fill_methods.COLOR;
    } else {
      this.gradient = fill_obj;
      this.fill_method = this.fill_methods.GRADIENT;
    }
  }
}

KRFill.prototype.compliment = function() {
  if (this.color === undefined) return null;
  return new KRFill(this.color.compliment());
}

KRFill.prototype.do = function(context) {
  if (this.on === false) return;
  context.save();
  switch (this.fill_method) {
    case this.fill_methods.COLOR:
      context.fillStyle = this.color.style();
      break;
    case this.fill_methods.GRADIENT:
      context.fillStyle = this.gradient;
      break;
  }
  context.fill();
  context.restore();
}

function KRStroke(color, w, lineJoin) {
  this.on = true;
  this.color = color;
  this.w = w;
  this.lineJoin = lineJoin != undefined ? lineJoin : 'round';
}

KRStroke.prototype.copy = function() {
  return this.scale(1);
}

KRStroke.prototype.scale = function(scale) {
  return new KRStroke(this.color, scale * this.w, this.lineJoin);
}

KRStroke.prototype.compliment = function() {
  return new KRStroke(this.color.compliment(), this.w, this.lineJoin);
}

KRStroke.prototype.do = function(context) {
  if (this.w === 0 || this.on === false) return;
  context.save();
  context.lineJoin = this.lineJoin;
  context.strokeStyle = this.color.style();
  context.lineWidth = this.w;
  context.stroke();
  context.restore();
}

function KRShape(bbox, stroke, fill) {
  this.bbox = bbox;
  this.stroke = stroke;
  this.fill = fill;
}

KRShape.prototype.copy = function() {
  return new KRShape(this.bbox.copy(), this.stroke.copy(),
   this.fill);
}

KRShape.prototype.in = function(coords) {
  return this.bbox.in(coords);
}

KRShape.prototype.compliment = function() {
  return new KRShape(this.bbox, this.stroke.compliment(),
   this.fill.compliment());
}

KRShape.prototype.render = function(context) {
  this.fill.do(context);
  this.stroke.do(context);
}

function KRBoundBox(x, y, w, h) {
  this.x = x;
  this.y = y;
  this.w = w;
  this.h = h;
}

KRBoundBox.prototype.copy = function() {
  return this.scale(1);
}

KRBoundBox.prototype.scale = function(scale) {
  return new KRBoundBox(scale * this.x, scale * this.y,
   scale * this.w, scale * this.h);
}

KRBoundBox.prototype.in = function(coords) {
  if (this.x <= coords.x
   && this.x + this.w >= coords.x
   && this.y <= coords.y
   && this.y + this.h >= coords.y) {
    return true;
  } else {
    return false;
  }
}

function KRHandleGradientsView(parent_view, rect, colors) {
  this.parent_view = parent_view;
  this.scale = this.parent_view.scale;
  this.rel_size = this.parent_view.rel_size || 1;
  this.rect = rect;
  this.colors = colors;
  this.inner = new KRRectangle(new KRShape(
   new KRBoundBox(rect.shape.bbox.x, rect.shape.bbox.y,
   rect.shape.bbox.w, rect.shape.bbox.h / 10),
   new KRStroke(this.colors.handle_fill, 0,
    'round'),
   new KRFill(new KRColor(100, 0, 100, 1))));
}

KRHandleGradientsView.prototype.resize = function(scale, rel_size) {
  this.scale = scale;
  this.rel_size = rel_size !== undefined ? rel_size : 1;
  this.rect.resize(this.scale);
  this.inner.resize(this.scale, this.rel_size);
}

KRHandleGradientsView.prototype.in = function(coords) {
  return this.rect.in(coords);
}

KRHandleGradientsView.prototype.set = function(percent) {
  this.pos = percent;
  this.inner.BBOX.h = this.rect.BBOX.w / 3;
  var y = this.rect.BBOX.y + percent
   * (this.rect.BBOX.h - this.inner.BBOX.h);
  this.inner.BBOX.y = y;
  this.inner.BBOX.x = this.rect.BBOX.x;
  this.inner.BBOX.w = this.rect.BBOX.w;
  this.inner.resize(this.scale, this.rel_size);
}

KRHandleGradientsView.prototype.set_bbox = function(bbox) {
  this.rect.set_bbox(bbox);
  this.inner.set_bbox(new KRBoundBox(bbox.x, bbox.y, bbox.w, bbox.w / 3));
}

KRHandleGradientsView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.colors.handle_fill = new KRColor(185, 185, 185, 1);
  this.colors.handle_stroke = this.colors.handle_fill.copy();
}

KRHandleGradientsView.prototype.render = function(context, round) {
  context.save();
  /*var gradient = context.createLinearGradient(this.rect.shape.bbox.x
   + this.rect.shape.stroke.w,
   this.rect.shape.bbox.y + this.rect.shape.stroke.w,
   this.rect.shape.bbox.x + this.rect.shape.stroke.w,
   this.rect.shape.bbox.y + this.rect.shape.stroke.w
   + this.rect.shape.bbox.h);
  var fill_color = this.colors.handle_fill.copy_with_alpha(0.666);
  gradient.addColorStop(0, fill_color.darken().darken().darken().darken().style());
  gradient.addColorStop(Math.max(0, this.pos), fill_color.darken().darken().style());
  gradient.addColorStop(Math.min(1, this.pos), fill_color.style());
  gradient.addColorStop(1, fill_color.darken().darken().style());
  this.rect.shape.fill = new KRFill(gradient);
  this.rect.render(context);*/
  var gradient = context.createLinearGradient(this.inner.shape.bbox.x,
   this.inner.shape.bbox.y, this.inner.shape.bbox.x, this.inner.shape.bbox.y
   + this.inner.shape.bbox.h);
  gradient.addColorStop(0, this.colors.handle_fill.lighten().lighten().style());
  gradient.addColorStop(0.5, this.colors.handle_fill.style());
  gradient.addColorStop(1, this.colors.handle_fill.darken().darken().darken().darken().style());
  this.inner.shape.fill = new KRFill(gradient);
  this.inner.render(context, round);
  context.restore;
}

function KRRectangle(shape) {
  this.scale = 1;
  this.shape = shape;
  this.W = shape.stroke.w;
  this.set_bbox(shape.bbox);
}

KRRectangle.prototype = Object.create(KRView.prototype);
KRRectangle.prototype.constructor = KRRectangle;

KRRectangle.prototype.copy = function() {
  return new KRRectangle(this.shape.copy());
}

KRRectangle.prototype.compliment = function() {
  return new KRRectangle(this.shape.compliment());
}

KRRectangle.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox.copy();
  this.resize(this.scale);
}

KRRectangle.prototype.set_colors = function(colors) {
  this.shape.fill.color = colors.trough_fill;
  this.shape.stroke.color = colors.trough_stroke;
}

KRRectangle.prototype.resize = function(scale, rel_size) {
  this.scale = scale;
  if (rel_size === undefined) rel_size = 1;
  this.shape.bbox = this.BBOX.scale(this.scale);
  this.shape.stroke.w = this.scale * rel_size * this.W;
}

KRRectangle.prototype.in = function(coords) {
  return this.shape.in(coords);
}

KRRectangle.prototype.clip = function(context, round) {
  this.create_path(context, round);
  context.clip();
}

KRRectangle.prototype.render = function(context, round) {
  context.save();
  this.create_path(context, round);
  this.shape.render(context);
  context.restore();
}

KRRectangle.prototype.create_path = function(context, round) {
  var x = this.shape.bbox.x + 0.5 * this.shape.stroke.w;
  var y = this.shape.bbox.y + 0.5 * this.shape.stroke.w;
  var w = this.shape.bbox.w - this.shape.stroke.w;
  var h = this.shape.bbox.h - this.shape.stroke.w;
  context.beginPath();
  if (round === undefined) {
    context.rect(x, y, w, h);
  } else {
    var r = typeof round === 'number' ? round : Math.min(w,h)/5;
    context.arc(x+r, y+r, r, Math.PI, 1.5 * Math.PI);
    context.lineTo(x + w - r, y);
    context.arc(x+w-r, y + r, r, 1.5 * Math.PI, 0);
    context.lineTo(x+w, y + h - r);
    context.arc(x+w-r, y+h-r, r, 0, 0.5 * Math.PI);
    context.lineTo(x+r, y+h);
    context.arc(x+r, y+h-r, r, 0.5 * Math.PI, Math.PI);
    context.closePath();
  }
}

function KRLabelView(parent_view, text, context, bbox, colors, orientation,
 opts) {
  this.inherit_from = KRView;
  this.inherit_from();
  this.parent_view = parent_view;
  this.scale = this.parent_view.scale;
  this.text = text;
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.background = new KRRectangleView(this, null, context, bbox, colors);
  this.background.set_setting_mode('OUTSET');
  this.colors = colors;
  this.orientation = orientation || 'V';
  this.rounded = false;
  if (opts['rounded'] === true) {
    this.rounded = true;
  }
  var shape = new KRShape(this.BBOX, new KRStroke(colors.trough_stroke,
   0, 'round'), new KRFill(colors.trough_fill));
  this.resize(this.scale);
}

KRLabelView.prototype = Object.create(KRView.prototype);
KRLabelView.prototype.constructor = KRLabelView;

KRLabelView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.background.set_colors({
   trough_fill: colors.trough_fill.darken().darken(),
   trough_stroke: colors.trough_stroke.copy()});
  this.damage();
}

KRLabelView.prototype.render = function() {
  if (this.damaged) {
    this.context.save();
    this.background.damage();
    this.background.render(true);
    if (this.orientation === 'V') {
      this.context.textBaseline = 'middle';
      this.context.textAlign = 'center';
      this.context.font = 'normal ' + 0.8 * this.background.rect.shape.bbox.w
       + 'px play';
      var w = this.context.measureText(this.text).width;
      var h = parseInt(this.context.font, 10);
      var fit = true;
      while (w > 0.9 * this.background.rect.shape.bbox.h) {
        h--;
        if (h <= 0) {
          fit = false;
          break;
        }
        this.context.font = 'normal ' + h + 'px play';
        w = this.context.measureText(this.text).width;
      }
      if (fit) {
        var x = this.background.rect.shape.bbox.x + 0.5 * this.background.rect.shape.bbox.w;
        var y = this.background.rect.shape.bbox.y + 0.5 * this.background.rect.shape.bbox.h;
        this.context.translate(x, y);
        this.context.rotate(-Math.PI/2);
        this.fill_text_outset(this.text, this.colors.trough_fill,
         0, 0);
      }
    } else {
      this.context.textBaseline = 'middle';
      this.context.textAlign = 'center';
      this.context.font = 'normal ' + 0.8 * Math.round(this.background.rect.shape.bbox.h)
       + 'px play';
      var w = this.context.measureText(this.text).width;
      var h = parseInt(this.context.font, 10);
      var fit = true;
      while (w > 0.9 * this.background.rect.shape.bbox.w) {
        h--;
        if (h <= 0) {
          fit = false;
          break;
        }
        this.context.font = 'normal ' + h + 'px play';
        w = this.context.measureText(this.text).width;
      }
      if (fit) {
        var x = this.background.rect.shape.bbox.x + 0.5 * this.background.rect.shape.bbox.w;
        var y = this.background.rect.shape.bbox.y + 0.5 * this.background.rect.shape.bbox.h;
        this.fill_text_outset(this.text, this.colors.trough_fill, x, y);
      } else {
        var parts = [this.text.slice(0, this.text.length/2),
         this.text.slice(this.text.length/2)];
        this.context.font = 'normal ' + 0.2 * Math.round(this.background.rect.shape.bbox.h) + 'px play';
        var w = Math.max(this.context.measureText(parts[0]).width,
         this.context.measureText(parts[1]).width);
        var h = parseInt(this.context.font, 10);
        var fit = true;
        while (w > 0.9 * this.background.rect.shape.bbox.w) {
          h--;
          if (h <= 0) {
            fit = false;
            break;
          }
          this.context.font = 'normal ' + h + 'px play';
          w = Math.max(this.context.measureText(parts[0]).width,
         this.context.measureText(parts[1]).width);
        }
        if (fit) {
          var x = this.background.rect.shape.bbox.x + 0.5 * this.background.rect.shape.bbox.w;
          var y = this.background.rect.shape.bbox.y + 0.5 * this.background.rect.shape.bbox.h - 0.6 * h;
          this.fill_text_outset(parts[0], this.colors.trough_fill, x, y);
          y = this.background.rect.shape.bbox.y + 0.5 * this.background.rect.shape.bbox.h + 0.6 * h;
          this.fill_text_outset(parts[1], this.colors.trough_fill, x, y);
        }
      }
    }
    this.damaged = false;
    this.context.restore();
  }
}

KRLabelView.prototype.set_bbox = function(bbox, rot) {
  /*if (this.rot !== undefined && this.rect !== undefined) {
    this.context.save();
    this.context.translate(this.rect.shape.bbox.x
     + 0.5 * this.rect.shape.bbox.w,
     this.rect.shape.bbox.y + 0.5 * this.rect.shape.bbox.h);
    this.context.rotate(this.rot * Math.PI / 180);
    this.context.translate(-this.rect.shape.bbox.x
     - 0.5 * this.rect.shape.bbox.w,
     -this.rect.shape.bbox.y - 0.5 * this.rect.shape.bbox.h);
    this.context.clearRect(this.rect.shape.bbox.x, this.rect.shape.bbox.y,
     this.rect.shape.bbox.w, this.rect.shape.bbox.h);
    this.context.restore();
  }*/
  if (rot !== undefined) this.rot = rot;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  var pad = this.parent_view.rel_size * 4 * SHADOW_LENGTH;
  this.background.set_bbox(new KRBoundBox(pad + bbox.x, pad + bbox.y,
   bbox.w - 2 * pad, bbox.h - 2 * pad));
  this.resize(this.parent_view.scale);
  this.damaged = true;
}

KRLabelView.prototype.resize = function(scale) {
  this.scale = scale;
  this.background.resize(scale, this.parent_view.rel_size);
  this.damaged = true;
}

KRRange = function(min, max, knee) {
  this.min = min;
  this.max = max;
  this.knee = {};
  if (knee != undefined) {
    this.knee.range = knee.range;
    this.knee.move_fraction = knee.move_fraction;
  } else {
    this.knee.range = 0;
    this.knee.move_fraction = 0;
  }
}

KRRange.prototype.valid = function() {
  var len;
  if (this.min >= this.max) return false;
  len = this.max - this.min;
  if (this.knee.range > len) return false;
  if (this.knee.move_fraction < 0) return false;
  if (this.knee.range > 0 && this.knee.move_fraction < 0.01) return false;
  if (this.knee.move_fraction > 1.0) return false;
  return true;
}

KRPushButtonTypes = { MUTE: 0, EXPAND_CONTRACT: 1 };
KRPushButton = function(parent_view, label, rect, rect_on, type) {
  this.types = KRPushButtonTypes;
  if (type !== undefined) {
    this.type = type;
  } else {
    this.type = this.types.MUTE;
  }
  this.parent_view = parent_view;
  this.scale = this.parent_view.scale;
  this.full_size = [100, 161];
  this.label = label;
  this.rect = new KRRectangle(new KRShape(new KRBoundBox(
   rect.shape.bbox.x, rect.shape.bbox.y, rect.shape.bbox.w, rect.shape.bbox.h),
   new KRStroke(rect.shape.stroke.color.copy(),
   rect.shape.stroke.w, rect.shape.stroke.lineJoin),
   new KRFill(rect.shape.fill.color.copy())));
  this.RECT_DEFAULT = rect;
  this.RECT_ON = rect_on;
  this.resize(this.parent_view.scale);
  this.states = { DEFAULT: 0, ON: 1 };
  this.state = this.states.DEFAULT;
  this.mdown = false;
}

KRPushButton.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.RECT_DEFAULT.shape.stroke.color = this.colors.trough_stroke.copy();
  this.RECT_DEFAULT.shape.fill.color = this.colors.trough_fill.copy();
  this.RECT_ON.shape.stroke.color = this.colors.trough_stroke.compliment();
  this.RECT_ON.shape.fill.color = this.colors.trough_fill.compliment();
  if (this.state == this.states.DEFAULT) {
    this.rect.shape.stroke.color = this.RECT_DEFAULT.shape.stroke.color.copy();
    this.rect.shape.fill.color = this.RECT_DEFAULT.shape.fill.color.copy();
  } else {
    this.rect.shape.stroke.color = this.RECT_ON.shape.stroke.color.copy();
    this.rect.shape.fill.color = this.RECT_ON.shape.fill.color.copy();
  }
  this.damaged = true;
}

KRPushButton.prototype.set_bbox = function(bbox) {
  this.RECT_DEFAULT.BBOX = bbox;
  this.RECT_DEFAULT.scale(this.scale);
  this.RECT_ON.BBOX = bbox;
  this.RECT_ON.scale(this.scale);
  this.rect.BBOX = bbox;
  this.rel_size = bbox.w / this.full_size[0];
  this.rect.resize(this.scale);
}

KRPushButton.prototype.resize = function(scale) {
  this.rect.resize(scale);
  this.scale = scale;
}

KRPushButton.prototype.in = function(coords) {
  return this.rect.in(coords);
}

KRPushButton.prototype.on_mousedown = function() {
  var damaged = true;
  this.mdown = true;
  return damaged;
}

KRPushButton.prototype.on_mouseup = function(on_me) {
  var damaged = false;
  if (on_me) {
    if (this.mdown) {
      damaged = true;
      this.click();
    }
  }
  this.mdown = false;
  return damaged;
}

KRPushButton.prototype.done = function() {
  var damaged = true;
  this.state = this.states.DEFAULT;
  this.rect.shape.stroke.color.r = this.RECT_DEFAULT.shape.stroke.color.r;
  this.rect.shape.fill.color.r = this.RECT_DEFAULT.shape.fill.color.r;
  this.rect.shape.stroke.color.g = this.RECT_DEFAULT.shape.stroke.color.g;
  this.rect.shape.fill.color.g = this.RECT_DEFAULT.shape.fill.color.g;
  this.rect.shape.stroke.color.b = this.RECT_DEFAULT.shape.stroke.color.b;
  this.rect.shape.fill.color.b = this.RECT_DEFAULT.shape.fill.color.b;
  this.parent_view.damage();
}

KRPushButton.prototype.on_mute_progress = function() {
  if (this.state !== this.states.ON) return;
  if (this.type === this.types.MUTE) {
    this.parent_view.model.set_from_pos(this.volume_pos);
  } else if (this.type === this.types.EXPAND_CONTRACT) {
    this.parent_view.set_width(this.width_tween);
  }
}

KRPushButton.prototype.on_expand_done = function() {
  this.parent_view.expansion_state = this.parent_view.expansion_states.EXPANDED;
  this.done();
}

KRPushButton.prototype.on_contract_done = function() {
  this.parent_view.expansion_state = this.parent_view.expansion_states.CONTRACTED;
  this.done();
}

KRPushButton.prototype.on_expansion_progress = function() {
  if (this.state !== this.states.ON) return;
  if (this.type === this.types.EXPAND_CONTRACT) {
    this.parent_view.set_width(this.width_tween);
  }
  this.parent_view.damage();
}

Tweener.removeTweens = function(o) {
  var T = Tweener;
  for (var i = 0; i < T.twns.length; i++) {
    var t = T.twns[i];
    if (t.obj === o) {
      T.twns.splice(i, 1);
    }
  }
}

KRPushButton.prototype.click = function() {
  if (this.state === this.states.DEFAULT) {
    this.state = this.states.ON;
    if (this.type === this.types.MUTE) {
      this.volume_pos = this.parent_view.model.get_pos();
      this.volume_end = 1;
      Tweener.addTween(this, {
       volume_pos: this.volume_end, transition:'easeOutExpo', time: 1,
       onComplete: create_handler(this, this.done),
       onUpdate: create_handler(this, this.on_mute_progress)});
    } else if (this.type === this.types.EXPAND_CONTRACT) {
      if (this.parent_view.expansion_state ===
       this.parent_view.expansion_states.EXPANDED
       || this.parent_view.expansion_state ===
       this.parent_view.expansion_states.EXPANDING) {
        this.parent_view.expansion_state
         = this.parent_view.expansion_states.CONTRACTING
        this.width_tween = this.parent_view.trough.shape.bbox.w
         + 2 * this.parent_view.button_space;
        Tweener.addTween(this, {
        width_tween : this.parent_view.contracted_w, transition:'easeOutExpo', time:1,
         onComplete: create_handler(this, this.on_contract_done),
         onUpdate: create_handler(this, this.on_expansion_progress)});
      } else if (this.parent_view.expansion_state ===
       this.parent_view.expansion_states.CONTRACTED
       || this.parent_view.expansion_state ===
       this.parent_view.expansion_states.CONTRACTING) {
        this.parent_view.expansion_state
         = this.parent_view.expansion_states.EXPANDING
        this.width_tween = this.parent_view.bbox.w;
        Tweener.addTween(this, {
         width_tween: this.parent_view.expanded_w, transition:'easeOutExpo', time:1,
         onComplete: create_handler(this, this.on_expand_done),
         onUpdate: create_handler(this, this.on_expansion_progress)});
      }
    }
    this.rect.shape.stroke.color.r = this.RECT_ON.shape.stroke.color.r;
    this.rect.shape.fill.color.r = this.RECT_ON.shape.fill.color.r;
    this.rect.shape.stroke.color.g = this.RECT_ON.shape.stroke.color.g;
    this.rect.shape.fill.color.g = this.RECT_ON.shape.fill.color.g;
    this.rect.shape.stroke.color.b = this.RECT_ON.shape.stroke.color.b;
    this.rect.shape.fill.color.b = this.RECT_ON.shape.fill.color.b;
  } else {
    this.state = this.states.DEFAULT;
    Tweener.removeTweens(this);
    this.rect.shape.stroke.color.r = this.RECT_DEFAULT.shape.stroke.color.r;
    this.rect.shape.fill.color.r = this.RECT_DEFAULT.shape.fill.color.r;
    this.rect.shape.stroke.color.g = this.RECT_DEFAULT.shape.stroke.color.g;
    this.rect.shape.fill.color.g = this.RECT_DEFAULT.shape.fill.color.g;
    this.rect.shape.stroke.color.b = this.RECT_DEFAULT.shape.stroke.color.b;
    this.rect.shape.fill.color.b = this.RECT_DEFAULT.shape.fill.color.b;
  }
}

KRPushButton.prototype.render = function(context) {
  var rect;
  rect = this.rect;
  context.save();
  rect.render(context);
  context.font = 'normal ' + this.rel_size * this.scale * 10 + 'pt helv_b';
  context.textBaseline = 'middle';
  context.textAlign = 'center';
  var h = parseInt(context.font, 10);
  var w = context.measureText(this.label).width;
  var x = rect.shape.bbox.x + 0.5 * rect.shape.bbox.w;
  var y = rect.shape.bbox.y + 0.5 * rect.shape.bbox.h;
  context.fillStyle = rect.shape.stroke.color.style();
  context.fillText(this.label, x, y, rect.shape.bbox.w);
  context.restore();
  this.damaged = false;
}

function KRPushButtonValueView(model, parent_view, label, rect, rect_on, type) {
  this.model = model;
  model.add_view(this);
  KRPushButton.call(this, parent_view, label, rect, rect_on, type);
  this.set_handle();
}

KRPushButtonValueView.prototype = Object.create(KRPushButton.prototype);
KRPushButtonValueView.prototype.constructor = KRPushButtonValueView;

KRPushButtonValueView.prototype.set_handle = function() {
  var val = this.model.pos_to_value(this.model.pos);
  if (val == -120) {
    this.label = '-∞';
  } else {
    this.label = val.toPrecision(2);
  }
}

KRPushButtonValueView.prototype.set_bbox = function(bbox) {
  this.RECT_DEFAULT.BBOX = bbox;
  this.RECT_DEFAULT.scale(this.scale);
  this.RECT_ON.BBOX = bbox;
  this.RECT_ON.scale(this.scale);
  this.rect.BBOX = bbox;
  this.rel_size = bbox.w / this.full_size[0];
  this.rect.resize(this.scale);
}
/**
 *  * Converts an RGB color value to HSV. Conversion formula
 *   * adapted from http://en.wikipedia.org/wiki/HSV_color_space.
 *    * Assumes r, g, and b are contained in the set [0, 255] and
 *     * returns h, s, and v in the set [0, 1].
 *      *
 *       * @param   Number  r       The red color value
 *        * @param   Number  g       The green color value
 *         * @param   Number  b       The blue color value
 *          * @return  Array           The HSV representation
 *           */
function rgb2hsv(r, g, b){
  r = r/255, g = g/255, b = b/255;
  var max = Math.max(r, g, b), min = Math.min(r, g, b);
  var h, s, v = max;
  var d = max - min;
  s = max == 0 ? 0 : d / max;
  if(max == min){
    h = 0; // achromatic
  }else{
    switch(max){
      case r: h = (g - b) / d + (g < b ? 6 : 0); break;
      case g: h = (b - r) / d + 2; break;
      case b: h = (r - g) / d + 4; break;
    }
    h /= 6;
  }
  return [h, s, v];
}

/**
 *  * Converts an HSV color value to RGB. Conversion formula
 *   * adapted from http://en.wikipedia.org/wiki/HSV_color_space.
 *    * Assumes h, s, and v are contained in the set [0, 1] and
 *     * returns r, g, and b in the set [0, 255].
 *      *
 *       * @param   Number  h       The hue
 *        * @param   Number  s       The saturation
 *         * @param   Number  v       The value
 *          * @return  Array           The RGB representation
 *           */
function hsv2rgb(h, s, v){
  var r, g, b;
  var i = Math.floor(h * 6);
  var f = h * 6 - i;
  var p = v * (1 - s);
  var q = v * (1 - f * s);
  var t = v * (1 - (1 - f) * s);
  switch(i % 6){
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
  }
  return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}

function CanvasViewContainer(rack_unit, container_elem, id) {
  this.rack_unit = rack_unit;
  if (this.rack_unit.add_view) this.rack_unit.add_view(this);
  this.container_elem = container_elem;
  this.id = id;
  this.view = null;
  this.scale = 1.0;
  this.canvas_container = document.createElement('div');
  this.canvas_container.className = 'canvas_container eq_gray_darker';
  this.container_elem.appendChild(this.canvas_container);
  this.canvas_container.id = this.id + '_container';
  this.canvas = document.createElement('canvas');
  this.canvas.className = 'canvas_view_canvas'
  this.canvas_container.appendChild(this.canvas);
  this.context = this.canvas.getContext("2d");
  this.transitions = {};
  this.canvas.addEventListener('mousedown',
   create_handler(this, this.on_mousedown));
  var mouse_wheel_evt=(/Firefox/i.test(navigator.userAgent)) ?
   "DOMMouseScroll" : "mousewheel";
  this.canvas.addEventListener('click', create_handler(this, this.on_click));
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
   this.on_keypress));
  (function animloop() {
    requestAnimFrame(animloop);
    _redraw();
  })();
}

CanvasViewContainer.prototype = Object.create(KRView.prototype);
CanvasViewContainer.prototype.constructor = CanvasViewContainer;

CanvasViewContainer.prototype.add_view = function(path, view) {
  this.view = view;
  this.layout();
}

CanvasViewContainer.prototype.on_keypress = function(evt) {
  if (this.is_visible) {
    if (evt.which === 67) { /* c */
      this.layout();
    }
  }
}

CanvasViewContainer.prototype.on_click = function(evt) {
  var v = this.view;
  if (v === null) return;
  if (v.on_click) v.on_click(evt);
  if (v.damaged) {
    this.damaged = true;
  }
}

CanvasViewContainer.prototype.on_mousedown = function(evt) {
  var v = this.view;
  if (v === null) return;
  if (v.on_mousedown) v.on_mousedown(evt);
  if (v.damaged) {
    this.damaged = true;
  }
}

CanvasViewContainer.prototype.on_mouseup = function(evt) {
  var v = this.view;
  if (v === null) return;
  if (v.on_mouseup) v.on_mouseup(evt);
  if (v.damaged) {
    this.damaged = true;
  }
}

CanvasViewContainer.prototype.on_mousemove = function(evt) {
  var v = this.view;
  if (v === null) return;
  if (v.on_mousemove) v.on_mousemove(evt);
  if (v.damaged) {
    this.damaged = true;
  }
}

CanvasViewContainer.prototype.on_mousescroll = function(evt) {
  var v = this.view;
  if (v === null) return;
  if (v.on_mousescroll) v.on_mousescroll(evt);
  if (v.damaged) {
    this.damaged = true;
  }
}

CanvasViewContainer.prototype.resize = function() {
  this.resize_canvas();
  var v = this.view;
  if (v === null) return;
  if (v.resize) {
    v.resize(this.scale);
  }
}

CanvasViewContainer.prototype.resize_canvas = function() {
  var w = parseInt(window.getComputedStyle(this.container_elem, null).getPropertyValue('width'), 10);//this.container_elem.offsetWidth;
  var h = parseInt(window.getComputedStyle(this.container_elem, null).getPropertyValue('height'), 10);
  if (w == 0 || h == 0) {
    this.resize_needed = true;
    this.damage();
    return -1;
  }
  this.dpr = window.devicePixelRatio;
  var c = this.canvas;
  c.style.width = Math.round(w) + "px";
  c.style.height = Math.round(h) + "px";
  c.width = Math.round(w * this.dpr);
  c.height = Math.round(h * this.dpr);
  var gc = c.getContext("2d");
  gc.scale(this.dpr, this.dpr);
  this.damage();
  var view = this.view;
  view.set_bbox(new KRBoundBox(0, 0, w, h));
  this.canvas_height = h;
  this.canvas_width = w;
  this.scale = this.calc_scale();
  return 0;
}

CanvasViewContainer.prototype.calc_scale = function() {
  var scale = 1;
  return scale;
}

CanvasViewContainer.prototype.page_resize = function() {
  this.resize_needed = true;
  this.damaged = true;
}

CanvasViewContainer.prototype.visible = function() {
  this.is_visible = true;
  this.resize_needed = true;
  this.damage();
}

CanvasViewContainer.prototype.hidden = function() {
  this.is_visible = false;
}

CanvasViewContainer.prototype.redraw = function() {
  if (this.is_visible) {
    if (this.damaged) {
      if (this.needs_layout) {
        if (this.layout() === -1) {
          this.damage();
          return;
        }
      }
      if (this.resize_needed) {
        if (this.resize_canvas() === -1) {
          this.damage();
          return;
        }
        var view = this.view;
        if (view.resize) view.resize(this.scale);
        view.damaged = true;
        this.resize_needed = false;
      }
      this.clear_canvas();
      var view = this.view;
      view.damaged = true;
      view.render();
      this.damaged = false;
    }
  }
}

CanvasViewContainer.prototype.window_resize = function() {
  this.page_resize();
};

CanvasViewContainer.prototype.on_patch = function(crate) {
  for (var i = 0, len = crate.data.length; i < len; i++) {
    var p = crate.data[i];
    if (p !== undefined) {
      var v = this.view;
      v.model.on_patch(crate);
    }
  }
}

CanvasViewContainer.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/mixer/")) return;
  if (crate.meth == "PATCH") {
    this.on_patch(crate);
  } else if (crate.meth == "PUT") {
    this.on_new_path(crate);
  } else if (crate.meth == "DELETE") {
    this.on_del(crate);
  }
}

CanvasViewContainer.prototype.on_del = function(crate) {
}

CanvasViewContainer.prototype.on_new_path = function(crate) {
}

CanvasViewContainer.prototype.damage = function() {
  this.damaged = true;
  var v = this.view;
  if (v === null) return;
  v.damaged = true;
}

CanvasViewContainer.prototype.clear_canvas = function() {
  this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
}

CanvasViewContainer.prototype.layout = function() {
  var w_fs = parseInt(this.canvas.offsetWidth, 10);
  var h_fs = parseInt(this.canvas.offsetHeight, 10);
  if (isNaN(w_fs) || isNaN(h_fs) || w_fs === 0 || h_fs === 0) {
    this.needs_layout = true;
    return -1;
  }
  var view = this.view;
  var x = 0;
  var y = 0;
  var w = w_fs;
  var h = h_fs;
  view.set_bbox(new KRBoundBox(x, y, w, h));
  view.set_colors(CanvasViewContainer.colors_from_color(
   MIXER_BG_COLOR));
  this.damage();
  this.needs_layout = false;
}

CanvasViewContainer.colors_from_color = function(c) {
  var colors = {};
  colors.trough_fill = c.darken().darken().darken();
  colors.trough_stroke = c.lighten().lighten();
  colors.handle_fill = new KRColor(185, 185, 185, 1);
  colors.handle_stroke = colors.handle_fill.copy();
  return colors;
}

CanvasViewContainer.prototype.random_colors = function() {
  var colors = {};
  colors.trough_fill = KRColor.random_light(1);
  colors.trough_stroke = KRColor.copy_color_with_alpha(1.0);
  colors.handle_fill = KRColor.copy_color_with_alpha(
      colors.trough_fill, 0.666);
  colors.handle_stroke = KRColor.copy_color_with_alpha(colors.handle_fill, 1.0);
  return colors;
}

CanvasViewContainer.prototype.delete = function() {
  this.canvas_container.parent.removeChild(this.canvas_container);
  this.canvas_container.innerHTML = '';
}

