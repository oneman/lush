function KRPeakMeterModel(path, label, nchannels) {
  var n = this.nchannels = nchannels;
  this.path = path;
  this.label = label;
  this.avg = [];
  this.avg = nchannels === 2 ? ['/afx/0/meter/avg', '/afx/1/meter/avg']:
   ['/afx/0/meter/avg'];
  this.peak = nchannels === 2 ? ['/afx/0/meter/peak', '/afx/1/meter/peak']:
   ['/afx/0/meter/peak'];
  this.views = [];
  this.flag = new Array(n);
  this.rms = new Array(n);
  this.dpk = new Array(n);
  this.cnt = new Array(n);
  this.allTimePeak = 0;
  for (var i = 0; i < n; i++) {
    this.flag[i] = 0;
    this.rms[i] = 0;
    this.dpk[i] = 0;
    this.cnt[i] = 0;
  }
}

KRPeakMeterModel.prototype.db_to_value = function(db) {
  return Math.pow(10, (db/20));
}

KRPeakMeterModel.prototype.on_patch = function(crate) {
  var set = false;
  if (this.path === crate.path) {
    for (var i = 0, len = crate.data.length; i < len; i++) {
      var p = crate.data[i];
      if (p.op === "replace") {
        var idx = this.avg.indexOf(p.path);
        if (idx > -1) {
          this.rms[idx] = this.db_to_value(p.value);
          set = true;
        } else {
          idx = this.peak.indexOf(p.path);
          if (idx > -1) {
            this.dpk[idx] = this.db_to_value(p.value);
            set = true;
          }
        }
      }
    }
  }
  if (set) {
    for (var i = 0, len = this.views.length; i < len; i++) {
      var v = this.views[i];
      if (v.damage) v.damage();
    }
  }
}

KRPeakMeterModel.prototype.add_view = function(view) {
  this.views.push(view);
}

KRPeakMeterModel.prototype.remove_view = function(view) {
  if (this.views.indexOf(view) > -1) {
    this.views.splice(this.views.indexOf(view), 1);
  }
}

function KRPeakMeterWebAudioModel(path, nchannels) {
  this.path = path;
  var n = this.nchannels = nchannels;
  this.BUF_SIZE = 2048;
  var a_call = window.webkitAudioContext || window.AudioContext;
  this.acontext = new a_call();
  this.views = [];
  this.startMedia();
  this.node = this.acontext.createScriptProcessor(this.BUF_SIZE);
  this.bufSize = this.node.bufferSize;
  this.sampleRate = this.acontext.sampleRate;
  this.flag = new Array(n);
  this.rms = new Array(n);
  this.dpk = new Array(n);
  this.z1 = new Array(n);
  this.z2 = new Array(n);
  this.cnt = new Array(n);
  this.allTimePeak = 0;
  for (var i = 0; i < n; i++) {
    this.flag[i] = 0;
    this.rms[i] = 0;
    this.dpk[i] = 0;
    this.z1[i] = 0;
    this.z2[i] = 0;
    this.cnt[i] = 0;
  }
  this.omega = 9.72 / this.sampleRate;
  var t = this.bufSize/this.sampleRate;
  var holdTime = 0.33;
  var fallbackRate = 20; /*dB/s*/
  this.hold = Math.round(holdTime/t + 0.5);
  this.fall = Math.pow(10.0, -0.05 * fallbackRate * t);
  this.node.onaudioprocess = create_handler(this, this.process);
}

KRPeakMeterWebAudioModel.prototype.add_view = function(view) {
  this.views.push(view);
}

KRPeakMeterWebAudioModel.prototype.remove_view = function(view) {
  if (this.views.indexOf(view) > -1) {
    this.views.splice(this.views.indexOf(view), 1);
  }
}

KRPeakMeterWebAudioModel.prototype.startMedia = function() {
  var constraints = {audio: true};
  getUserMedia(constraints, create_handler(this, this.handleUserMedia),
   create_handler(this, this.handleUserMediaError));
}

KRPeakMeterWebAudioModel.prototype.handleUserMedia = function(stream) {
  this.microphone = this.acontext.createMediaStreamSource(stream);
  this.microphone.connect(this.node);
  this.node.connect(this.acontext.destination);
}

KRPeakMeterWebAudioModel.handleUserMediaError = function(error){
  kr.debug('getUserMedia error: ' + error);
}

KRPeakMeterWebAudioModel.prototype.getAudioNode = function () {
  return this.node;
}

KRPeakMeterWebAudioModel.prototype.connectNodeToInput = function(audioNode) {
  this.audioInputNodes.push(audioNode);
  audioNode.connect(this.node);
}

KRPeakMeterWebAudioModel.prototype.process = function(e) {
  var bufs = new Array();
  for (var i = 0; i < this.nchannels; i++ ) {
    bufs[i] = e.inputBuffer.getChannelData(i);
    var s, t, z1, z2;
    if (this.flag[i]) {
      this.rms[i] = 0;
      this.flag[i] = 0;
    }
    z1 = this.z1[i];
    z2 = this.z2[i];
    t = 0;
    var n = bufs[i].length / 4;
    var p = 0;
    while (n--) {
      s = bufs[i][p++];
      s *= s;
      if (t < s) t = s;
      z1 += this.omega * (s - z1);
      s = bufs[i][p++];
      s *= s;
      if (t < s) t = s;
      z1 += this.omega * (s - z1);
      s = bufs[i][p++];
      s *= s;
      if (t < s) t = s;
      z1 += this.omega * (s - z1);
      s = bufs[i][p++];
      s *= s;
      if (t < s) t = s;
      z1 += this.omega * (s - z1);
      z2 += 4 * this.omega * (z1 - z2);
    }
    t = Math.sqrt(t);
    this.z1[i] = z1 + 1e-20;
    this.z2[i] = z2 + 1e-20;
    s = Math.sqrt(2 * z2);
    if (s > this.rms[i]) this.rms[i] = s;
    if (t > this.dpk[i]) {
      this.dpk[i] = t;
      this.cnt[i] = this.hold;
    } else if (this.cnt[i]) {
      this.cnt[i]--;
    } else {
      this.dpk[i] *= this.fall;
      this.dpk[i] += 1e-10;
    }
  }
}

KRPeakMeterJKView = function(parent_view, model, label, context, bbox, colors) {
  this.parent_view = parent_view;
  this.model = model;
  this.label = label;
  this.model.add_view(this);
  this.full_size = [100, 666];
  this.scale = this.parent_view.scale;
  this.LINE_WIDTH = 1;
  this.TICK_WIDTH = 1;
  this.TICK_HEIGHT = 3;
  this.H_TICK = 1;
  this.H_SPACE = 0;
  this.BAR_WIDTH = 6;
  this.TICK_LEN = 4;
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.colors = colors;
  this.bg_canvas = document.createElement('canvas');
  this.outer_shape = new KRShape(this.BBOX.copy(),
   new KRStroke(colors.trough_stroke.copy(), this.LINE_WIDTH, 'round'),
   new KRFill(colors.trough_fill.copy()));
  this.background_rect = new KRRectangle(this.outer_shape);
  this.resize(this.scale);
}

KRPeakMeterJKView.prototype = Object.create(KRView.prototype);
KRPeakMeterJKView.prototype.constructor = KRPeakMeterJKView;

KRPeakMeterJKView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.outer_shape.stroke.color = this.colors.trough_stroke;
  this.outer_shape.fill.color = this.colors.trough_fill;
  this.bg_damaged = true;
  this.damage();
}

KRPeakMeterJKView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  this.rel_size = this.BBOX.w / this.full_size[0];
  this.bbox = this.BBOX.scale(this.scale);
  this.outer_shape.bbox = bbox;
  this.background_rect.set_bbox(bbox);
  this.resize(this.parent_view.scale);
}

KRPeakMeterJKView.prototype.resize = function(scale) {
  this.dpr = window.devicePixelRatio;
  this.scale = scale;
  this.tick_height = this.rel_size * this.scale * this.TICK_HEIGHT;
  this.h_tick = this.rel_size * this.H_TICK * scale;
  this.h_space = this.rel_size * this.H_SPACE * scale;
  this.w_space = this.rel_size * this.W_SPACE * scale;
  this.bar_width = this.rel_size * this.BAR_WIDTH * scale;
  this.tick_width = this.rel_size * this.TICK_WIDTH * scale;
  this.tick_len = this.rel_size * this.TICK_LEN * scale;
  this.background_rect.resize(scale);
  this.bbox = this.BBOX.scale(scale);
  this.bg_canvas.width = this.dpr * this.bbox.w;
  this.bg_canvas.height = this.dpr * this.bbox.h;
  this.bg_canvas.style.height = this.dpr * this.bbox.h + 'px';
  this.bg_canvas.style.width = this.dpr * this.bbox.w + 'px';
  this.bg_canvas.getContext('2d').scale(this.dpr, this.dpr);
  this.width = this.bbox.w
  this.height = this.bbox.h;
  this.button_space = this.bbox.w/1.618;
  this.header_height = this.button_space;
  this.footer_height = 0.25 * this.header_height;
  this.bar_length = this.height - this.header_height - this.footer_height;
  this.bg_damaged = true;
  this.damage();
}

KRPeakMeterJKView.prototype.damage = function() {
  this.damaged = true;
  this.parent_view.damage();
}

KRPeakMeterJKView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

KRPeakMeterJKView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    this.mdown = true;
  }
}

KRPeakMeterJKView.prototype.on_mouseup = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.mdown && this.bbox.in(coords)) {
    this.allTimePeak = -Infinity;
    this.damage()
  }
  this.mdown = false;
}

KRPeakMeterJKView.prototype.drawDataBg = function(context) {
  context.save();
  var blue = new KRColor(0, 0, 102, 1);
  var green = new KRColor(0, 102, 0, 1);
  var yellow = new KRColor(102, 102, 0, 1);
  var red = new KRColor(102, 0, 0, 1);
  var h = this.bbox.h;
  var colors = [blue, green, yellow, red];
  for (var i = 0; i < 2; i++) {
    var x = this.bbox.w/2 - this.tick_len/2 - this.bar_width
     + i * (this.tick_len + this.bar_width);
    var y = this.header_height;
    while (y + this.h_tick < this.bbox.h - this.footer_height) {
      var percent = 1 - (y - this.header_height) / this.bar_length;
      context.fillStyle = this.gradient(colors, percent);
      context.fillRect(x,y,this.bar_width,this.h_tick);
      y += this.h_space + this.h_tick;
    }
  }
  context.restore();
}

KRPeakMeterJKView.prototype.drawData = function(context) {
  context.save();
  var blue = new KRColor(0, 0, 256, 1);
  var green = new KRColor(0, 256, 0, 1);
  var yellow = new KRColor(256, 256, 0, 1);
  var red = new KRColor(256, 0, 0, 1);
  var h = this.bbox.h;
  var colors = [blue, green, yellow, red];
  for (var i = 0; i < 2; i++) {
    var w = this.bbox.w;
    var h = this.bbox.h;
    var kr = this.mapk20(this.model.rms[i]);
    var kp = this.mapk20(this.model.dpk[i]);
    var dp = kp - kr;
    var x = this.bbox.x + this.bbox.w/2 - this.tick_len/2 - this.bar_width
     + i * (this.tick_len + this.bar_width);
    var no_stroke = new KRStroke(this.outer_shape.fill.color, 0);
    no_stroke.on = false;
    var y = this.bbox.y + this.header_height;
    while (y + this.h_tick < this.bbox.y + this.bbox.h - this.footer_height) {
      var percent = 1 - ((y - (this.header_height + this.bbox.y))
       / (this.bar_length));
      if (y + this.h_tick >= this.bbox.y + this.bbox.h - kr
       - this.footer_height) {
        context.fillStyle = this.gradient(colors, percent);
        context.fillRect(x,y,this.bar_width,this.h_tick);
      }
      if (y + this.h_space + this.h_tick >= this.bbox.h + this.bbox.y - kp
          - this.footer_height
          && y <= this.bbox.h + this.bbox.y - kp - this.footer_height) {
        context.fillStyle = '#ccc';
        context.fillRect(x,y-0.5*this.tick_height,this.bar_width,
         this.tick_height);
      }
      y += this.h_space + this.h_tick;
    }
    this.model.flag[i] = 1;
  }
  context.restore();
}

KRPeakMeterJKView.prototype.gradient = function(tetrads, percent) {
  if (percent <= 1.0 && percent >= 0.62) {
    var p = (percent - 0.62) / 0.38;
    return tetrads[2].interpolate(tetrads[3],p).style();
  } else if (percent <= 0.62 && percent >= 0.5) {
    var p = (percent - 0.5) / 0.12;
    return tetrads[1].interpolate(tetrads[2],p).style();
  } else if (percent <= 0.5 && percent >= 0.0) {
    var p = (percent) / 0.5;
    return tetrads[0].interpolate(tetrads[1],p).style();
  }
}

KRPeakMeterJKView.prototype.drawTick = function(context, v, text, hex_color) {
  var mapped = this.mapk20(v);
  if (mapped < 1) mapped = 1;
  var y = this.bbox.y + this.header_height + this.bar_length - mapped;
  context.save();
  context.font = 'normal ' + this.rel_size * this.scale * 8 + 'pt play';
  context.textBaseline = 'middle';
  var stroke = this.background_rect.shape.stroke.copy();
  stroke.w = this.tick_width;
  var rgb = hexToRgb(hex_color);
  stroke.color = new KRColor(rgb.r, rgb.g, rgb.b, 1);
  context.fillStyle = stroke.color.style();
  context.beginPath();
  context.moveTo(this.bbox.x + this.bbox.w/2 - this.bar_width
   - 1.5 * this.tick_len, y);
  context.lineTo(this.bbox.x + this.bbox.w/2 - this.bar_width
   - 0.5 * this.tick_len, y);
  stroke.do(context);
  context.beginPath();
  context.moveTo(this.bbox.x + this.bbox.w/2 - 0.5 * this.tick_len, y);
  context.lineTo(this.bbox.x + this.bbox.w/2 + 0.5 * this.tick_len, y);
  stroke.do(context);
  context.beginPath();
  context.moveTo(this.bbox.x + this.bbox.w/2 + 0.5 * this.tick_len
   + this.bar_width, y);
  context.lineTo(this.bbox.x + this.bbox.w/2 + 1.5 * this.tick_len
   + this.bar_width, y);
  stroke.do(context);
  context.textAlign = 'end';
  var t_x = this.bbox.x + this.bbox.w/2 - this.bar_width
   - 2 * this.tick_len;
  var t_y = y;
  this.fill_text_outset(text, this.colors.trough_fill, t_x, t_y);
  context.textAlign = 'start';
  var t_x = this.bbox.x + this.bbox.w/2 + this.bar_width
   + 2 * this.tick_len;
  var t_y = y;
  this.fill_text_outset(text, this.colors.trough_fill, t_x, t_y);
  context.restore();
}

KRPeakMeterJKView.prototype.drawBarTicks = function(context) {
  this.drawTick(context, 0.001, "-60", "#6666ff");
  this.drawTick(context, 0.001778, "", "#6666ff");
  this.drawTick(context, 0.003162, "-50", "#6666ff");
  this.drawTick(context, 0.00562, "", "#00ff00");
  this.drawTick(context, 0.01, "-40", "#00ff00");
  this.drawTick(context, 0.01778, "", "#00ff00");
  this.drawTick(context, 0.03162, "-30", "#00ff00");
  this.drawTick(context, 0.0562, "", "#00ff00");
  this.drawTick(context, 0.1, "-20", "#ffff00");
  this.drawTick(context, 0.1778, "-15", "#FF4C00");
  this.drawTick(context, 0.3162, "-10", "#FF4C00");
  this.drawTick(context, 0.501, "-6", "#FF4C00");
  this.drawTick(context, 0.708, "-3", "#FF4C00");
  this.drawTick(context, 1, "0", "#FF4C00");
}

KRPeakMeterJKView.prototype.render_background = function() {
  if (this.bg_damaged) {
    this.bg_context = this.bg_canvas.getContext('2d');
    this.bg_context.clearRect(0, 0, this.bg_context.canvas.width,
     this.bg_context.canvas.height);
    var bg_rect;
    if (this.allTimePeak > 1) {
      bg_rect = this.background_rect.compliment();
    } else {
      bg_rect = this.background_rect;
    }
    bg_rect.render(this.bg_context,
     { x: this.background_rect.shape.bbox.x,
       y: this.background_rect.shape.bbox.y });
    this.drawDataBg(this.bg_context);
    this.bg_image = this.bg_context.getImageData(0, 0,
     this.bg_context.canvas.width, this.bg_context.canvas.height);
    this.bg_damaged = false;
  }
  this.context.putImageData(this.bg_image, this.dpr * this.bbox.x,
   this.dpr * this.bbox.y);
}

KRPeakMeterJKView.prototype.render = function() {
  if (this.damaged) {
    var dpk = Math.max(this.model.dpk[0], this.model.dpk[1]);
    if (dpk > this.allTimePeak) {
      if (dpk > 0) this.bg_damaged = true;
      this.allTimePeak = dpk;
    }
    this.render_background();
    this.drawData(this.context);
    this.drawBarTicks(this.context);
    this.drawMaxPeak();
    this.damaged = false;
  }
}

KRPeakMeterJKView.prototype.drawMaxPeak = function() {
  var w = this.width;
  var h = this.header_height;
  this.context.save();
  var peakdb = new Number(20 * Math.log(this.allTimePeak) / Math.log(10));
  this.text = Math.round(peakdb) <= -120 ? "-\u221E" :
   peakdb.toPrecision(2);
  this.context.fillStyle = peakdb > 0 ?
   this.outer_shape.stroke.color.compliment().style() :
   this.outer_shape.stroke.color.style();
  this.context.font = 'normal ' + 0.5 * Math.round(h) + 'pt play';
  this.context.textBaseline = "middle";
  this.context.textAlign = 'center';
  var t_w = this.context.measureText(this.text).width;
  var t_h = parseInt(this.context.font, 10);
  var t_x = this.background_rect.shape.bbox.x + 0.5 *
   this.background_rect.shape.bbox.w;
  var t_y = this.background_rect.shape.bbox.y + 0.5 * h;
  var fit = true;
  while (t_w > 0.9 * this.background_rect.shape.bbox.w) {
    t_h--;
    if (t_h <= 0) {
      fit = false;
      break;
    }
    this.context.font = 'normal ' + t_h + 'pt play';
    t_w = this.context.measureText(this.text).width;
  }
  if (fit) {
    this.fill_text_outset(this.text, this.colors.trough_fill, t_x, t_y);
  }
  this.context.restore();
}

KRPeakMeterJKView.prototype.mapk20 = function(v) {
  var ratio = this.bar_length / 450;
  if (v < 0.001) return ratio * (24000 * v);
  v = Math.log (v) / Math.log(10) + 3;
  if (v < 2.0) return ratio * (24.3 + v * (100 + v * 16));
  if (v > 3.0) v = 3.0;
  return ratio * (v * 161.7 - 35.1);
}

function hexToRgb(hex) {
  // Expand shorthand form (e.g. "03F") to full form (e.g. "0033FF")
  var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
  hex = hex.replace(shorthandRegex, function(m, r, g, b) {
    return r + r + g + g + b + b;
  });

  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? {
    r: parseInt(result[1], 16),
    g: parseInt(result[2], 16),
    b: parseInt(result[3], 16)
  } : null;
}

KRPeakMeterMinimalView = function(parent_view, model, label, context, bbox, colors) {
  this.parent_view = parent_view;
  this.model = model;
  this.label = label;
  this.model.add_view(this);
  this.full_size = [100, 666];
  this.scale = this.parent_view.scale;
  this.LINE_WIDTH = 0;
  this.TICK_WIDTH = 1;
  this.TICK_HEIGHT = 3;
  this.H_TICK = 1;
  this.H_SPACE = 0;
  this.TEXT_WIDTH = 50;
  this.BAR_WIDTH = 6;
  this.TICK_LEN = 4;
  this.context = context;
  this.BBOX = bbox;
  this.bbox = this.BBOX.scale(this.scale);
  this.original_colors = colors;
  this.colors = colors;
  var shadow_length = SHADOW_LENGTH * this.scale;
  var pad = 4 * shadow_length;
  var w = this.BBOX.w - 2 * pad;
  var button_space = Math.round(w / 1.618);
  this.button_space = button_space;
  this.bg_canvas = document.createElement('canvas');
  this.allTimePeak = -666666;
  var vv_bbox = new KRBoundBox(pad + this.BBOX.x, this.BBOX.y,
   this.BBOX.w - 2 * pad, button_space);
  var shape = new KRShape(vv_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.value_view = new KRValueViewPeak(this, this.model, this.context,
   new KRRectangle(shape), colors, {always_active: true});
  /*var trough_bbox = new KRBoundBox(this.BBOX.x, this.BBOX.y + button_space,
   w, this.BBOX.h - 2* button_space);
  var shape = new KRShape(trough_bbox, new KRStroke(colors.trough_stroke,
   this.LINE_WIDTH, 'round'), new KRFill(colors.trough_fill));
  this.trough = new KRRectangleView(this, null, context, trough_bbox, colors);*/
  this.outer_shape = new KRShape(this.BBOX.copy(),
   new KRStroke(colors.trough_stroke.copy(), this.LINE_WIDTH, 'round'),
   new KRFill(colors.trough_fill.copy()));
  this.background_rect = new KRRectangle(this.outer_shape);
  this.resize(this.scale);
}

KRPeakMeterMinimalView.prototype = Object.create(KRView.prototype);
KRPeakMeterMinimalView.prototype.constructor = KRPeakMeterMinimalView;

KRPeakMeterMinimalView.prototype.set_colors = function(colors) {
  this.colors = colors;
  this.outer_shape.stroke.color = this.colors.trough_stroke;
  this.outer_shape.fill.color = this.colors.trough_fill;
  this.value_view.set_colors(colors);
  this.bg_damaged = true;
  this.damage();
}

KRPeakMeterMinimalView.prototype.set_bbox = function(bbox) {
  this.BBOX = bbox;
  this.rel_size = this.BBOX.w / this.full_size[0];
  var shadow_length = SHADOW_LENGTH;
  var pad = 4 * shadow_length * this.rel_size;
  var w = this.BBOX.w;
  var button_space = w / 1.618;
  this.button_space = button_space;
  this.background_rect.set_bbox(this.BBOX.copy());
  var vv_bbox = new KRBoundBox(pad + this.BBOX.x,
   pad + this.BBOX.y, this.BBOX.w - 2 * pad,
   button_space - 2 * pad);
  this.value_view.set_bbox(vv_bbox);
  this.resize(this.parent_view.scale);
}

KRPeakMeterMinimalView.prototype.resize = function(scale) {
  this.rel_size = this.BBOX.w / this.full_size[0];
  this.dpr = window.devicePixelRatio;
  this.scale = scale;
  this.value_view.resize(scale, this.rel_size);
  this.tick_height = this.rel_size * this.scale * this.TICK_HEIGHT;
  this.text_width = this.rel_size * this.scale * this.TEXT_WIDTH;
  this.h_tick = this.rel_size * this.H_TICK * scale;
  this.h_space = this.rel_size * this.H_SPACE * scale;
  this.w_space = this.rel_size * this.W_SPACE * scale;
  this.bar_width = this.rel_size * this.BAR_WIDTH * scale;
  this.tick_width = this.rel_size * this.TICK_WIDTH * scale;
  this.tick_len = this.rel_size * this.TICK_LEN * scale;
  this.background_rect.resize(scale, this.rel_size);
  this.bbox = this.BBOX.scale(scale);
  this.bg_canvas.width = this.dpr * this.bbox.w;
  this.bg_canvas.height = this.dpr * this.bbox.h;
  this.bg_canvas.style.height = this.bbox.h + 'px';
  this.bg_canvas.style.width = this.bbox.w + 'px';
  this.bg_canvas.getContext('2d').scale(this.dpr, this.dpr);
  this.width = this.bbox.w
  this.height = this.background_rect.shape.bbox.h;
  this.button_space = this.bbox.w/1.618;
  this.header_height = this.button_space * 1.2;
  this.footer_height = 0.05 * this.header_height;
  this.bar_length = this.height - this.header_height
   - 2 * this.scale * 4 * SHADOW_LENGTH - this.footer_height;
  this.bg_damaged = true;
  this.damage();
}

KRPeakMeterMinimalView.prototype.damage = function() {
  this.damaged = true;
  this.parent_view.damage();
}

KRPeakMeterMinimalView.prototype.event_to_canvas = function(evt) {
  var rect = this.context.canvas.getBoundingClientRect();
  var x = evt.pageX - rect.left - window.pageXOffset;
  var y = evt.pageY - rect.top - window.pageYOffset;
  return {x: x, y: y};
}

KRPeakMeterMinimalView.prototype.on_click = function(evt) {
  this.mdown = true;
  this.on_mouseup(evt);
}

KRPeakMeterMinimalView.prototype.on_mousedown = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.bbox.in(coords)) {
    evt.preventDefault();
    evt.stopPropagation();
    this.mdown = true;
  }
}

KRPeakMeterMinimalView.prototype.on_mouseup = function(evt) {
  var coords = this.event_to_canvas(evt);
  if (this.mdown && this.bbox.in(coords)) {
    evt.preventDefault();
    evt.stopPropagation();
    this.allTimePeak = -Infinity;
    this.damage()
  }
  this.mdown = false;
}

KRPeakMeterMinimalView.prototype.drawDataBg = function(context) {
  context.save();
  var h = this.bbox.h;
  var y = this.background_rect.shape.bbox.y + this.header_height
   + this.scale * 4 * SHADOW_LENGTH;
  if (this.model.nchannels === 2) {
    for (var i = 0; i < 2; i++) {
      var x = this.background_rect.shape.bbox.x
       + this.background_rect.shape.bbox.w/2 - this.background_rect.shape.stroke.w
       - 0.5 * this.text_width - this.bar_width - 0.5 * this.tick_len
       + i * (this.tick_len + this.bar_width);
      var kr = this.mapk20(this.model.rms[i]);
      context.fillStyle = '#111';
      context.fillRect(x,y,this.bar_width, this.bar_length - kr);
    }
  } else if (this.model.nchannels === 1) {
    var x = this.background_rect.shape.bbox.x
      + this.background_rect.shape.bbox.w/2 - this.background_rect.shape.stroke.w
      - 0.5 * this.text_width - this.bar_width;
    var w = 2 * this.bar_width;
    var kr = this.mapk20(this.model.rms[0]);
    context.fillStyle = '#111';
    context.fillRect(x,y,w, this.bar_length-kr);
  }
  context.restore();
}

KRPeakMeterMinimalView.prototype.drawData = function(context) {
  context.save();
  if (this.model.nchannels === 2) {
    for (var i = 0; i < 2; i++) {
      var w = this.background_rect.shape.bbox.w;
      var h = this.background_rect.shape.bbox.h;
      var kr = this.mapk20(this.model.rms[i]);
      var kp = this.mapk20(this.model.dpk[i]);
      var dp = kp - kr;
      var x = this.background_rect.shape.bbox.x
        + this.background_rect.shape.bbox.w/2
        - this.background_rect.shape.stroke.w
        - 0.5 * this.text_width - this.bar_width - 0.5 * this.tick_len
        + i * (this.tick_len + this.bar_width);
      var no_stroke = new KRStroke(this.outer_shape.fill.color, 0);
      no_stroke.on = false;
      var y_s = this.bbox.y + this.header_height - this.scale * 4 * SHADOW_LENGTH;
      var y = this.bbox.y + this.bbox.h - kr - this.footer_height - this.scale
       * 4 * SHADOW_LENGTH;
      var gradient = this.make_gradient(context, x, y_s);
      context.fillStyle = gradient;
      context.fillRect(x,y,this.bar_width, kr);
      y = Math.max(this.bbox.y + this.header_height,
        this.bbox.y + this.bbox.h - kp - this.footer_height - this.scale
        * 4 * SHADOW_LENGTH);
      if (y  >= this.bbox.y + this.header_height
       + this.scale * 4 * SHADOW_LENGTH
          && y + this.h_tick <= this.bbox.h + this.bbox.y - this.footer_height
          - this.scale * 4 * SHADOW_LENGTH) {
        context.fillStyle = gradient;
        context.fillRect(x,y,this.bar_width,
            this.tick_height);
      }
      this.model.flag[i] = 1;
    }
  } else if (this.model.nchannels === 1) {
    var w = 2 * this.bar_width;
    var h = this.background_rect.shape.bbox.h;
    var kr = this.mapk20(this.model.rms[0]);
    var kp = this.mapk20(this.model.dpk[0]);
    var dp = kp - kr;
    var x = this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - this.background_rect.shape.stroke.w
     - 0.5 * this.text_width - this.bar_width;
    var no_stroke = new KRStroke(this.outer_shape.fill.color, 0);
    no_stroke.on = false;
    var y_s = this.bbox.y + this.header_height - this.scale * 4 * SHADOW_LENGTH;
    var y = this.bbox.y + this.bbox.h - kr - this.footer_height - this.scale
     * 4 * SHADOW_LENGTH;
    var grad = this.make_gradient(context, x, y_s);
    context.fillStyle = grad;
    context.fillRect(x,y,w,kr);
    y = Math.max(this.bbox.y + this.header_height,
     this.bbox.y + this.bbox.h - kp - this.footer_height - this.scale * 4 * SHADOW_LENGTH);
    if (y  >= this.bbox.y + this.header_height
     && y + this.h_tick <= this.bbox.h + this.bbox.y - this.footer_height
     - this.scale * 4 * SHADOW_LENGTH) {
      context.fillStyle = grad;
      context.fillRect(x,y,w, this.tick_height);
    }
    this.model.flag[0] = 1;
  }
  context.restore();
}

KRPeakMeterMinimalView.prototype.make_gradient = function(context, x, y) {
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

KRPeakMeterMinimalView.prototype.gradient = function(tetrads, percent) {
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

KRPeakMeterMinimalView.prototype.drawTick = function(context, v, text, hex_color) {
  var mapped = this.mapk20(v);
  var y = this.background_rect.shape.bbox.y
   + this.header_height + this.scale * 4 * SHADOW_LENGTH
   + this.bar_length - mapped;
  context.save();
  var stroke = this.background_rect.shape.stroke.copy();
  stroke.w = this.tick_width;
  var rgb = hexToRgb(hex_color);
  stroke.color = this.colors.handle_stroke;
  context.fillStyle = stroke.color.style();
  if (this.model.nchannels === 2) {
    context.beginPath();
    context.moveTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - 1.5 * this.tick_len
     - this.bar_width - 0.5 * this.text_width, y);
    context.lineTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - 0.5 * this.tick_len - this.bar_width
     - 0.5 * this.text_width, y);
    stroke.do(context);
    context.beginPath();
    context.moveTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - 0.5 * this.text_width
     - 0.5 * this.tick_len, y);
    context.lineTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - 0.5 * this.text_width
     + 0.5 * this.tick_len, y);
    stroke.do(context);
    context.beginPath();
    context.moveTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2
     - 0.5 * this.text_width + 0.5 * this.tick_len
     + this.bar_width, y);
    context.lineTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2
     - 0.5 * this.text_width + 1.5 * this.tick_len
     + this.bar_width, y);
    stroke.do(context);
  } else if (this.model.nchannels === 1) {
    context.beginPath();
    context.moveTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - 1.0 * this.tick_len
     - this.bar_width - 0.5 * this.text_width, y);
    context.lineTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2 - this.bar_width
     - 0.5 * this.text_width, y);
    stroke.do(context);
    context.beginPath();
    context.moveTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2
     - 0.5 * this.text_width
     + this.bar_width, y);
    context.lineTo(this.background_rect.shape.bbox.x
     + this.background_rect.shape.bbox.w/2
     - 0.5 * this.text_width + 1.0 * this.tick_len
     + this.bar_width, y);
    stroke.do(context);
  }
  context.restore();
  context.save();
  context.font = 'normal ' + this.rel_size * this.scale * 11 + 'pt play';
  context.textBaseline = 'middle';
  context.textAlign = 'center';
  var t_x = this.background_rect.shape.bbox.x
   + this.background_rect.shape.bbox.w - 0.5 * this.text_width;
  var t_y = y;
  this.fill_text_outset(text, this.colors.trough_fill, t_x, t_y);
  context.restore();
}

KRPeakMeterMinimalView.prototype.drawBarTicks = function(context) {
  this.drawTick(context, 0.0, "", "#6666ff");
  this.drawTick(context, 0.001, "-60", "#6666ff");
  this.drawTick(context, 0.001778, "", "#6666ff");
  this.drawTick(context, 0.003162, "-50", "#6666ff");
  this.drawTick(context, 0.00562, "", "#00ff00");
  this.drawTick(context, 0.01, "-40", "#00ff00");
  this.drawTick(context, 0.01778, "", "#00ff00");
  this.drawTick(context, 0.03162, "-30", "#00ff00");
  this.drawTick(context, 0.0562, "-25", "#00ff00");
  this.drawTick(context, 0.1, "-20", "#ffff00");
  this.drawTick(context, 0.1778, "-15", "#FF4C00");
  this.drawTick(context, 0.3162, "-10", "#FF4C00");
  this.drawTick(context, 0.501, "-6", "#FF4C00");
  this.drawTick(context, 0.708, "-3", "#FF4C00");
  this.drawTick(context, 1, "0", "#FF4C00");
}

KRPeakMeterMinimalView.prototype.render_background = function() {
  /*this.context.clearRect(this.background_rect.shape.bbox.x,
   this.background_rect.shape.bbox.y,
   this.background_rect.shape.bbox.w,
   this.background_rect.shape.bbox.h);*/
  var bg_rect;
  bg_rect = this.background_rect;
  bg_rect.render(this.context);
  this.drawDataBg(this.context);
  this.drawBarTicks(this.context);
}

KRPeakMeterMinimalView.prototype.render = function() {
  if (this.damaged) {
    var dpk = this.nchannels === 2 ?
     Math.max(this.model.dpk[0], this.model.dpk[1]):
     this.model.dpk[0];
    if (dpk > this.allTimePeak) {
      if (dpk > 0) this.bg_damaged = true;
      this.allTimePeak = dpk;
    }
    this.render_background();
    this.drawData(this.context);
    this.drawMaxPeak();
    this.damaged = false;
  }
}

KRPeakMeterMinimalView.prototype.drawMaxPeak = function() {
  var w = this.width;
  var h = this.header_height;
  var peakdb = new Number(20 * Math.log(this.allTimePeak) / Math.log(10));
  this.value_view.set_handle(peakdb);
  this.value_view.render();
}

KRPeakMeterMinimalView.prototype.mapk20 = function(v) {
  var ratio = this.bar_length / 450;
  if (v < 0.001) return ratio * (24000 * v);
  v = Math.log (v) / Math.log(10) + 3;
  if (v < 2.0) return ratio * (24.3 + v * (100 + v * 16));
  if (v > 3.0) v = 3.0;
  return ratio * (v * 161.7 - 35.1);
}
