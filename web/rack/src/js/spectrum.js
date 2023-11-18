function KRSpectrumWebAudioModel(size) {
  this.num_samples = size;
  this.views = [];
  this.enabled = false;
  var actxCall = window.AudioContext;
  this.acontext = new actxCall();
  this.smoothing = 0.666;
  this.fft = this.acontext.createAnalyser();
  this.fft.fftSize = this.num_samples;
  this.data = new Uint8Array(this.fft.fftSize/2);
  this.startMedia();
}

KRSpectrumWebAudioModel.prototype.enable = function() {
  var me = this;
  if (!this.enabled) {
    requestAnimFrame(create_handler(this, this.update));
    this.enabled = true;
  }
  return this;
}

KRSpectrumWebAudioModel.prototype.disable = function() {
  this.enabled = false;
  return this;
}

KRSpectrumWebAudioModel.prototype.add_view = function(view) {
  this.views.push(view);
}

KRSpectrumWebAudioModel.prototype.remove_view = function(view) {
  if (this.views.indexOf(view) > -1) {
    this.views.splice(this.views.indexOf(view), 1);
  }
}

KRSpectrumWebAudioModel.prototype.startMedia = function() {
  var constraints = {audio: true};
  getUserMedia(constraints, create_handler(this, this.handleUserMedia),
   create_handler(this, this.handleUserMediaError));
}

KRSpectrumWebAudioModel.prototype.handleUserMedia = function(stream) {
  this.microphone = this.acontext.createMediaStreamSource(stream);
  this.microphone.connect(this.fft);
  this.enable();
}

KRSpectrumWebAudioModel.handleUserMediaError = function(error){
  kr.debug('getUserMedia error: ' + error);
}

KRSpectrumWebAudioModel.prototype.getAudioNode = function () {
  return this.fft;
}

KRSpectrumWebAudioModel.prototype.connectNodeToInput = function(audioNode) {
  audioNode.connect(this.node);
}

KRSpectrumWebAudioModel.prototype.update = function(e) {
  if (this.enabled) {
    requestAnimFrame(create_handler(this, this.update));
  }
  var data = this.data;
  this.fft.smoothingTimeConstant = this.smoothing;
  this.fft.getByteFrequencyData(data);
  for (var i = 0; i < this.views.length; i++) {
    if (this.views[i].damage) this.views[i].damage();
  }
}

KRSpectrumView = function(parent_view, model, context, bbox, colors, opts) {
  this.parent_view = parent_view;
  this.model = model;
  this.num_bins = this.model.fft.frequencyBinCount;
  this.model.add_view(this);
  this.full_size = [100, 50];
  this.scale = this.parent_view.scale;
  this.PAD = 5;
  this.LINE_WIDTH = 0;
  this.TICK_WIDTH = 1;
  this.TICK_HEIGHT = 3;
  this.SHADOW_LENGTH = opts.shadow_length !== undefined ? opts.shadow_length :
   SHADOW_LENGTH;
  this.H_TICK = 1;
  this.H_SPACE = 0;
  this.TEXT_WIDTH = 50;
  this.BAR_WIDTH = 6;
  this.TICK_LEN = 4;
  this.BAR_SPACING = 1;
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.original_colors = colors;
  this.colors = colors;
  this.shadow_length = this.SHADOW_LENGTH * this.scale;
  var pad = this.PAD;
  var w = this.BBOX.w - 2 * pad;
  this.background = new KRRectangleView(this, null, context,
   bbox.copy(), colors, {'rounded': true, 'shadow_length': this.SHADOW_LENGTH
   });
  this.background.set_setting_mode('OUTSET');
  this.resize(this.scale);
}

KRSpectrumView.prototype = Object.create(KRView.prototype);
KRSpectrumView.prototype.constructor = KRSpectrumView;

KRSpectrumView.prototype.set_colors = function(colors) {
  this.colors = {};
  this.colors['bg_fill'] = colors.trough_fill;
  this.colors['bg_stroke'] = colors.trough_stroke;
  this.colors['trough_fill'] = colors.trough_fill.darken().darken();
  this.colors['trough_stroke'] = colors.trough_stroke;
  this.background.set_colors(this.colors);
  this.damage();
}

KRSpectrumView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  this.rel_size = this.BBOX.w / this.full_size[0];
  var pad = this.PAD;
  var w = this.BBOX.w - 2 * pad;
  var new_bbox = new KRBoundBox(pad + this.BBOX.x, pad + this.BBOX.y,
   w, this.BBOX.h - 2 * pad);
  this.background.set_bbox(new_bbox);
  this.resize(this.parent_view.scale);
}

KRSpectrumView.prototype.resize = function(scale) {
  this.rel_size = this.BBOX.w / this.full_size[0];
  this.dpr = window.devicePixelRatio;
  this.scale = scale;
  this.tick_height = this.rel_size * this.scale * this.TICK_HEIGHT;
  this.text_width = this.rel_size * this.scale * this.TEXT_WIDTH;
  this.h_tick = this.rel_size * this.H_TICK * scale;
  this.h_space = this.rel_size * this.H_SPACE * scale;
  this.w_space = this.rel_size * this.W_SPACE * scale;
  this.bar_width = this.rel_size * this.BAR_WIDTH * scale;
  this.tick_width = this.rel_size * this.TICK_WIDTH * scale;
  this.tick_len = this.rel_size * this.TICK_LEN * scale;
  this.bar_spacing = this.rel_size * this.BAR_SPACING * scale;
  this.background.resize(scale, this.rel_size);
  this.bbox = this.BBOX.scale(scale);
  this.width = this.background.rect.shape.bbox.w
  this.height = this.background.rect.shape.bbox.h;
  this.button_space = this.bbox.w/1.618;
  this.header_height = this.button_space * 1.2;
  this.footer_height = 0.05 * this.header_height;
  this.bar_length = this.height - this.header_height
   - 2 * this.scale * 4 * this.SHADOW_LENGTH - this.footer_height;
  this.bg_damaged = true;
  this.damage();
}

KRSpectrumView.prototype.damage = function() {
  this.background.damage();
  this.damaged = true;
  this.parent_view.damage();
}

KRSpectrumView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

KRSpectrumView.prototype.on_click = function(evt) {
  this.mdown = true;
  this.on_mouseup(evt);
}

KRSpectrumView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    evt.preventDefault();
    evt.stopPropagation();
    this.mdown = true;
  }
}

KRSpectrumView.prototype.on_mouseup = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.mdown && this.bbox.in(coords)) {
    evt.preventDefault();
    evt.stopPropagation();
    this.damage();
  }
  this.mdown = false;
}

KRSpectrumView.prototype.drawData = function(context) {
  context.save();
  context.fillStyle = 'green';
  // Break the samples up into bins
  var bg_rect = this.background.rect.shape.bbox;
  var bin_size = Math.floor(this.model.data.length / this.num_bins);
  for (var i=0; i < this.num_bins; ++i) {
    var sum = 0;
    for (var j=0; j < bin_size; ++j) {
      sum += this.model.data[(i * bin_size) + j];
    }
    // Calculate the average frequency of the samples in the bin
    var average = sum / bin_size;
    // Draw the bars on the canvas
    var bar_width = this.width / this.num_bins;
    var scaled_average = (average / 256) * this.height;
    context.fillRect(bg_rect.x + i * bar_width,
     bg_rect.y + this.height, bar_width
     - this.bar_spacing, -scaled_average);
  }
  context.restore();
}

KRSpectrumView.prototype.make_gradient = function(context, x, y) {
  var gradient
    = context.createLinearGradient(x, y, x, y + this.bar_length);
  var red = new KRColor(252, 21, 21, 1);
  var orange = new KRColor(252, 141, 55, 1);
  var yellow = new KRColor(252, 230, 72, 1);
  var light_green = new KRColor(16, 247, 67, 1);
  var dark_green = new KRColor(21, 136, 40, 1);
  var y_footer = this.bbox.y + this.bbox.h - this.footer_height;
  var y_start = this.bbox.y + this.header_height;
  gradient.addColorStop(0, red.style());
  var y_3 = y_footer - this.mapk20(this.model.db_to_value(-3));
  var percent = (y_3 - y_start) / this.bar_length;
  gradient.addColorStop(percent, red.style());
  gradient.addColorStop(percent, orange.style());
  var y_9 = y_footer - this.mapk20(this.model.db_to_value(-9));
  percent = (y_9 - y_start) / this.bar_length;
  gradient.addColorStop(percent, orange.style());
  gradient.addColorStop(percent, yellow.style());
  var y_20 = y_footer - this.mapk20(this.model.db_to_value(-20));
  percent = (y_20 - y_start) / this.bar_length;
  gradient.addColorStop(percent, yellow.style());
  gradient.addColorStop(percent, light_green.style());
  var y_40 = y_footer - this.mapk20(this.model.db_to_value(-40));
  percent = (y_40 - y_start) / this.bar_length;
  gradient.addColorStop(percent, light_green.style());
  gradient.addColorStop(percent, dark_green.style());
  gradient.addColorStop(1, dark_green.style());
  return gradient;
}

KRSpectrumView.prototype.gradient = function(tetrads, percent) {
  if (percent <= 1.0 && percent >= 0.75) {
    return tetrads[3].style();
  } else if (percent <= 0.75 && percent >= 0.62) {
    return tetrads[2].style();
  } else if (percent <= 0.62 && percent >= 0.333) {
    return tetrads[1].style();
  } else if (percent <= 0.333 && percent >= 0.0) {
    return tetrads[0].style();
  }
}

KRSpectrumView.prototype.render_background = function() {
  var bg_rect;
  this.context.save();
  this.context.fillStyle = this.colors.bg_fill.style();
  this.context.fillRect(this.bbox.x, this.bbox.y, this.bbox.w, this.bbox.h);
  this.context.restore();
  bg_rect = this.background;
  bg_rect.render(this.context);
}

KRSpectrumView.prototype.render = function() {
  if (this.damaged) {
    this.render_background();
    this.context.save();
    this.background.rect.clip(this.context, true);
    this.drawData(this.context);
    this.context.restore();
    this.damaged = false;
  }
}

