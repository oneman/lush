function KRSlider(container_elem, path, label, prop, opts) {
  this.path = path;
  this.label = label;
  this.prop = prop;
  this.container_elem = container_elem;
  this.mdown = false;
  this.last_patched;
  this.labelled_slider_elem = document.createElement('div');
  this.labelled_slider_elem.classList.add('labelled_slider');
  this.value_elem = document.createElement('div');
  this.value_elem.classList.add('slider_value');
  this.labelled_slider_elem.appendChild(this.value_elem);
  this.slider_elem = document.createElement('div');
  this.slider_elem.classList.add('slider');
  this.trough_elem = document.createElement('div');
  this.trough_elem.classList.add('slider_trough');
  this.trough_elem.classList.add('absolute_center');
  this.handle_elem = document.createElement('div');
  this.handle_elem.classList.add('handle');
  this.trough_elem.appendChild(this.handle_elem)
  this.slider_elem.appendChild(this.trough_elem);
  this.labelled_slider_elem.appendChild(this.slider_elem);
  this.label_elem = document.createElement('div');
  this.label_elem.classList.add('slider_label');
  this.label_elem.innerHTML = this.label;
  this.labelled_slider_elem.appendChild(this.label_elem);
  this.container_elem.appendChild(this.labelled_slider_elem);
  this.max_top =
   parseInt(window.getComputedStyle(this.trough_elem, null).getPropertyValue('height'), 10);
  this.max_top = this.max_top -
   parseInt(window.getComputedStyle(this.handle_elem, null).getPropertyValue('height'), 10);
  this.min_top = 0;
  this.handle_top = 0;
  this.linear_db = false;
  this.range = null;
  this.sender = undefined;
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
    this.set_handle(opts.initial_value);
  }
  this.trough_elem.addEventListener('mousedown', create_handler(this,
   this.on_trough_mouse_down));
  this.handle_elem.addEventListener('mousedown', create_handler(this,
   this.on_handle_mouse_down));
  var mouse_wheel_evt=(/Firefox/i.test(navigator.userAgent)) ?
   "DOMMouseScroll" : "mousewheel";
  this.slider_elem.addEventListener(mouse_wheel_evt,
   create_handler(this, this.on_mousescroll));
  window.addEventListener('mousemove', create_handler(this,
    this.on_mousemove));
  window.addEventListener('mouseup', create_handler(this,
    this.on_mouseup));
}

KRSlider.prototype.delete = function() {
  var l = this.labelled_slider_elem;
  l.parentElement.removeChild(l);
}

KRSlider.prototype.activate = function() {
  this.labelled_slider_elem.classList.add('active');
}

KRSlider.prototype.deactivate = function() {
  this.labelled_slider_elem.classList.remove('active');
}

KRSlider.prototype.set_from_top = function(new_top) {
  var new_val = this.top_scale_to_value(new_top);
  this.set_handle(new_val);
  if (new_val != this.last_patched) {
    kr.ctrl_mix([[this.prop, '', new_val]], this.path, this.sender);
    this.last_patched = new_val;
  }
}

KRSlider.prototype.on_mousescroll = function(evt) {
  var evt=window.event || evt;
  var delta=evt.detail? -32 * evt.detail : evt.wheelDelta;
  var patches = [];
  evt.preventDefault();
  var dely = delta / 360;
  var new_top = Math.min(this.max_top, (Math.max(this.min_top,
  this.handle_top - dely)));
  this.set_from_top(new_top);
}

KRSlider.prototype.on_trough_mouse_down = function(evt) {
  var offset = evt.clientY -
   this.trough_elem.getBoundingClientRect().top;
  var new_top = Math.min(this.max_top, (Math.max(this.min_top,
   offset - 0.5 * this.handle_elem.offsetHeight)));
  this.set_from_top(new_top);
  this.on_handle_mouse_down(evt);
}

KRSlider.prototype.on_handle_mouse_down = function(evt) {
  evt.stopPropagation();
  evt.preventDefault();
  var top = this.handle_elem.getBoundingClientRect().top;
  this.mdown_y = evt.clientY;
  this.mdown_top = this.handle_top;
  this.mdown = true;
}

KRSlider.prototype.on_mouseup = function(evt) {
  this.mdown = false;
}

KRSlider.prototype.on_mousemove = function(evt) {
  if (this.mdown == true) {
    var patches = [];
    evt.preventDefault();
    var dely = evt.clientY - this.mdown_y;
    var new_top = Math.min(this.max_top, (Math.max(this.min_top,
     this.mdown_top + dely)));
    this.set_from_top(new_top);
    evt.stopPropagation();
  }
}

KRSlider.prototype.format_number = function(value) {
  var fixed_digits = 2;
  return value.toFixed(fixed_digits);
}

KRSlider.prototype.set_handle = function(value) {
  this.value_elem.innerHTML = this.format_number(value);
  var new_top = this.top_scale_from_value(value);
  this.handle_top = new_top;
  this.handle_elem.style.top = this.handle_top + 'px';
}

KRSlider.prototype.get_handle = function() {
  return this.handle_top;
}

KRSlider.prototype.db_to_linear = function(db) {
  return Math.pow(10, (db / 20.0));
}

KRSlider.prototype.linear_to_db = function(val) {
  return 20.0 * Math.log(val) / Math.LN10;
}

KRSlider.prototype.top_scale_to_value = function(top) {
  var x;
  x = top / this.max_top;
  if (this.range.knee.move_fraction != 0
   && x <= this.range.knee.move_fraction) {
    var maxx = this.range.knee.move_fraction;
    var max = this.db_to_linear(this.range.max);
    var min = this.db_to_linear(this.range.max - this.range.knee.range);
    return this.linear_to_db(max - x * (max - min) / maxx);
  } else {
    var minx = this.range.knee.move_fraction;
    var maxx = 1;
    var max = this.range.max - this.range.knee.range;
    var min = this.range.min;
    if (maxx === minx) return max;
    return max - (x - minx) * (max - min) / (maxx - minx);
  }
}

KRSlider.prototype.top_scale_from_value = function(val) {
  if (val > (this.range.max - this.range.knee.range)) {
    var maxx = this.db_to_linear(this.range.max);
    var max = this.db_to_linear(this.range.max);
    var min = this.db_to_linear(this.range.max - this.range.knee.range);
    var x = (max - this.db_to_linear(val)) / (max - min);
    return this.max_top * this.range.knee.move_fraction * x;
  } else {
    var maxx = this.max_top;
    var minx = this.max_top * this.range.knee.move_fraction;
    var max = this.range.max - this.range.knee.range;
    var min = this.range.min;
    if (max == min) return minx;
    return minx + (maxx - minx) * (max - val) / (max - min);
  }
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


