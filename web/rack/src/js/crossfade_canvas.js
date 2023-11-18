function KRCrossfadeModel(pos, paths, label, prop, opts) {
  this.inheritFrom = KRSliderModel;
  this.inheritFrom(pos, "", label, prop, opts);
  this.paths = paths;
  var parts = this.paths['bus'].split('/');
  this.name = parts.pop();
  this.input_fader_values = {};
  this.active = false;
}

KRCrossfadeModel.prototype = Object.create(KRSliderModel.prototype);
KRCrossfadeModel.prototype.constructor = KRCrossfadeModel;

KRCrossfadeModel.prototype.add_input = function(crate) {
  if (this.paths['left'] === undefined) {
    this.paths['left'] = crate.path;
    this.input_fader_values['left'] = crate.data.afx[0].volume.fader;
    if (this.paths['right'] !== undefined) {
      this.active = true;
      this.pos_from_input('left');
    }
  } else if (this.paths['right'] === undefined) {
    this.paths['right'] = crate.path;
    this.input_fader_values['right'] = crate.data.afx[0].volume.fader;
    if (this.paths['left'] !== undefined) {
      this.active = true;
      this.pos_from_input('right');
    }
  } else {
    return -1;
  }
  return 0;
}

KRCrossfadeModel.prototype.set_from_pos = function(new_pos) {
  var new_val = this.pos_to_value(new_pos);
  if (this.active && new_val != this.last_patched && !isNaN(new_val)) {
    this.set_pos(new_val);
    var left_fade = this.left_fade();
    var right_fade = this.right_fade();
    kr.ctrl_mix([["/afx/0/volume/fader", "", left_fade]], this.paths['left'],
        this.sender);
    kr.ctrl_mix([["/afx/1/volume/fader", "", left_fade]], this.paths['left'],
        this.sender);
    kr.ctrl_mix([["/afx/0/volume/fader", "", right_fade]], this.paths['right'],
        this.sender);
    kr.ctrl_mix([["/afx/1/volume/fader", "", right_fade]], this.paths['right'],
        this.sender);
    this.last_patched = new_val;
  }
}

KRCrossfadeModel.prototype.left_fade = function() {
  return this.clean_value(this.linear_to_db(this.pos));
}

KRCrossfadeModel.prototype.right_fade = function() {
  return this.clean_value(this.linear_to_db(1.0 - this.pos));
}

KRCrossfadeModel.prototype.on_patch = function(crate) {
  var which;
  var ret;
  ret = -1;
  if (crate.path === this.paths['left']) {
    which = 'left';
  } else if (crate.path === this.paths['right']) {
    which = 'right';
  } else {
    return -1;
  }
  for (var i = 0, len = crate.data.length; i < len; i++) {
    var p = crate.data[i];
    if (p !== undefined) {
      if (p.op == "replace") {
        if (p.path === "/afx/0/volume/fader"
         || p.path === "/afx/1/volume/fader") {
          this.input_fader_values[which] = p.value;
          if (this.pos_from_input(which) === 0) {
            ret = 0;
          }
        }
      }
    }
  }
  return ret;
}

KRCrossfadeModel.prototype.clean_value = function(value) {
  value = isFinite(value) ? value : value < 0 ? -120 : 120;
  return value;
}

KRCrossfadeModel.prototype.pos_from_input = function(which) {
  if (this.input_fader_values[which] === undefined) {
    return -1;
  }
  var new_pos;
  if (which === 'left') {
    new_pos = this.db_to_linear(this.input_fader_values.left);
  } else {
    new_pos = 1 - this.db_to_linear(this.input_fader_values.right);
  }
  var new_val = this.pos_to_value(new_pos);
  kr.debug('value: ' + new_val);
  if (this.active && new_val != this.last_patched && !isNaN(new_val)) {
    this.set_pos(new_val);
    return 0;
  }
  return -1;
}

KRCrossfadeModel.prototype.close_enough = function(val1, val2) {
  return true;
}

KRCrossfadeModel.prototype.set_pos = function(value) {
  this.pos = this.pos_from_value(value);
  for (var i = 0; i < this.views.length; i++) {
    this.views[i].set_handle();
  }
}
