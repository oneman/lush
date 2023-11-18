$(document).ready(function() {
  rack_units.push({'constructor': StreamOutput});
});

function StreamOutput(info_object) {
  this.title = "StreamOutput";
  this.description = "Create stream output.";
  this.id = "stream_output";
  this.sel = "#" + this.id;
  this.main_elem = document.createElement('div');
  this.main_elem.classList.add('RCU');
  this.main_elem.id = this.id;
  $(info_object['parent_div'])[0].appendChild(this.main_elem);
  this.unit_container = document.createElement('div');
  this.unit_container.classList.add('unit_container');
  this.unit_container.classList.add('danred');
  this.unit_container.classList.add('offwhite');
  this.container_elem = document.createElement('div');
  this.container_elem.classList.add('stream_output_container');
  this.container_elem.id = this.id + '_container';
  this.unit_container.appendChild(this.container_elem);
  this.add_forms_elem = document.createElement('div');
  this.add_forms_elem.classList.add('add_forms_container');
  this.current_add_form = document.createElement('div');
  this.add_forms_elem.appendChild(this.current_add_form);
  this.unit_container.appendChild(this.add_forms_elem);
  this.main_elem.appendChild(this.unit_container);
  this.captures = {};
  this.muxes = {};
  this.encoders = {};
  this.outputs = {};
  this.make_layout();
  this.add_forms_elem_active = false;
}

StreamOutput.prototype.make_column = function(name, button_text) {
  var root_elem = document.createElement('div');
  root_elem.classList.add('column');
  var title = document.createElement('div');
  title.innerHTML = name;
  title.classList.add('verybig');
  title.classList.add('header');
  root_elem.appendChild(title);
  var list = document.createElement('div');
  list.classList.add('paths');
  root_elem.appendChild(list);
  return root_elem;
}

StreamOutput.prototype.make_layout = function() {
  this.path_elems = {};
  var col = this.make_column('Captures', 'Add Capture');
  this.path_elems.captures = col.getElementsByClassName('paths')[0];
  this.container_elem.appendChild(col);
  col = this.make_column('Encoders', 'Add Encoder');
  this.path_elems.encoders = col.getElementsByClassName('paths')[0];
  this.container_elem.appendChild(col);
  col = this.make_column('Muxers', 'Add Muxer');
  this.path_elems.muxes = col.getElementsByClassName('paths')[0];
  this.container_elem.appendChild(col);
  col = this.make_column('Outputs', 'Add Output');
  this.path_elems.outputs = col.getElementsByClassName('paths')[0];
  this.container_elem.appendChild(col);
}

StreamOutput.prototype.get_captures = function() {
  return Object.keys(this.captures);
}

StreamOutput.prototype.get_muxes = function() {
  return Object.keys(this.muxes);
}

StreamOutput.prototype.get_encoders = function() {
  return Object.keys(this.encoders);
}

StreamOutput.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/studio/") ||
   kr.crate_path_root_match(crate, "/studio/tracker/")) return;
  if (crate.meth == "PUT") {
    this.on_new_path(crate);
  }
}

StreamOutput.prototype.on_new_path = function(crate) {
  var processed = false;
  if (crate.data.capture !== undefined) {
    /*if (crate.data.capture.audio !== undefined) {
      var cap = new AudioCapture(crate);
      this.captures[crate.path] = cap;
      this.path_elems.captures.appendChild(cap.elem);
      processed = true;
    } else if (crate.data.capture.video !== undefined) {
      var cap = new VideoCapture(crate);
      this.captures[crate.path] = cap;
      this.path_elems.captures.appendChild(cap.elem);
      processed = true;
    }*/
    if (!processed) {
      var cap = new KRCEMO(crate);
      this.captures[crate.path] = cap;
      this.path_elems.captures.appendChild(cap.elem);
    }
  } else if (crate.data.encode !== undefined) {
    if (crate.data.encode.audio !== undefined) {
      if (crate.data.encode.audio.opus !== undefined) {
        var enc = new OpusEncoder(crate);
        this.encoders[crate.path] = enc;
        this.path_elems.encoders.appendChild(enc.elem);
        processed = true;
      } else if (crate.data.encode.audio.mp3 !== undefined) {
        var enc = new Mp3Encoder(crate);
        this.encoders[crate.path] = enc;
        this.path_elems.encoders.appendChild(enc.elem);
        processed = true;
      } else {
        var enc = new KRAudioEncoder(crate);
        this.encoders[crate.path] = enc;
        this.path_elems.encoders.appendChild(enc.elem);
        processed = true;
      }
    }
    if (!processed) {
      var enc = new KRCEMO(crate);
      this.encoders[crate.path] = enc;
      this.path_elems.encoders.appendChild(enc.elem);
    }
  } else if (crate.data.mux !== undefined) {
    var mux = new Mux(crate);
    this.muxes[crate.path] = mux;
    this.path_elems.muxes.appendChild(mux.elem);
    processed = true;
  } else if (crate.data.output !== undefined) {
    var out = new Output(crate);
    this.outputs[crate.path] = out;
    this.path_elems.outputs.appendChild(out.elem);
    processed = true;
  }
}

function Encoder(crate) {
}

function KRAudioEncoder(crate) {
  this.inherit_from = KRCEMO;
  this.inherit_from(crate);
}

KRAudioEncoder.prototype = Object.create(KRCEMO.prototype);
KRAudioEncoder.prototype.constructor = KRAudioEncoder;

KRCEMO.prototype.make_property_list = function() {
  this.attributes = {};
  for (var key in this.crate.data) {
    var elem = document.createElement('div');
    this.plist.appendChild(elem);
    this.attributes[key] = { 'elem': elem,
     'value': this.crate.data[key] };
  }
}

KRCEMO.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  /*for (var key in this.attributes) {
    var att = this.attributes[key];
    att.elem.innerHTML = key + ': ' + att.value;
  }*/
  this.name_elem.innerHTML = this.name;
  this.desc_elem.innerHTML =
  '<pre>' + JSON.stringify(this.crate, null, 2) + '</pre>';
}

KRCEMO.prototype.make_dom = function() {
  this.elem = document.createElement('div');
  this.elem.classList.add('output');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  this.plist = document.createElement('div');
  this.plist.classList.add('property_list');
  this.desc_elem.appendChild(this.plist);
}

function KRCEMO(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  this.make_dom();
  this.make_property_list();
  this.update();
}

function AudioCapture(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  this.channels = this.crate.data.capture.channels;
  this.elem = document.createElement('div');
  this.elem.classList.add('capture');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  this.update();
}

AudioCapture.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  this.desc_elem.innerHTML = 'Audio';
}

function VideoCapture(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  var video = this.crate.data.capture.video;
  this.width = video.width;
  this.height = video.height;
  this.fps_num = video.fps_num;
  this.fps_den = video.fps_den;
  this.format = video.format;
  this.elem = document.createElement('div');
  this.elem.classList.add('capture');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  this.update();
}

VideoCapture.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  this.desc_elem.innerHTML = this.width + ' x ' + this.height +
   ', ' + this.fps_num + ' / ' + this.fps_den + ', ' + this.format;
}

function OpusEncoder(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  var opus = this.crate.data.encode.audio.opus;
  this.attributes = {};
  this.elem = document.createElement('div');
  this.elem.classList.add('encode');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  for (var key in opus) {
    var elem = document.createElement('div');
    elem.classList.add('opus');
    this.attributes[key] = {'elem': elem, 'value': opus[key]};
    this.desc_elem.appendChild(elem);
  }
  this.update();
}

OpusEncoder.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  var att = this.attributes['channels'];
  att.elem.innerHTML = 'channels' + ': ' + att.value;
  var att = this.attributes['sample_rate'];
  att.elem.innerHTML = 'sample rate' + ': ' + kr_sample_rate_nstr(att.value);
  var att = this.attributes['bit_rate'];
  att.elem.innerHTML = 'bit rate' + ': ' + kr_bit_rate_bytes_advstr(att.value);
  var att = this.attributes['frame_size'];
  att.elem.innerHTML = 'frame size' + ': ' + kr_frame_size_advstr(att.value,
   this.attributes['sample_rate'].value);
  if (this.attributes['total_bytes'] !== undefined) {
    var att = this.attributes['total_bytes'];
    att.elem.innerHTML = 'total bytes' + ': ' + att.value;
  }
  if (this.attributes['total_frames']) {
    var att = this.attributes['total_frames'];
    att.elem.innerHTML = 'total frames' + ': ' + att.value;
  }
}


function Mp3Encoder(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  var mp3 = this.crate.data.encode.audio.mp3;
  this.attributes = {};
  this.elem = document.createElement('div');
  this.elem.classList.add('encode');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  for (var key in mp3) {
    var elem = document.createElement('div');
    elem.classList.add('mp3');
    this.attributes[key] = {'elem': elem, 'value': mp3[key]};
    this.desc_elem.appendChild(elem);
  }
  this.update();
}

Mp3Encoder.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  var att = this.attributes['channels'];
  att.elem.innerHTML = 'channels' + ': ' + att.value;
  var att = this.attributes['sample_rate'];
  att.elem.innerHTML = 'sample rate' + ': ' + kr_sample_rate_nstr(att.value);
  var att = this.attributes['bit_rate'];
  att.elem.innerHTML = 'bit rate' + ': ' + kr_bit_rate_bytes_advstr(att.value);
  if (this.attributes['total_bytes'] !== undefined) {
    var att = this.attributes['total_bytes'];
    att.elem.innerHTML = 'total bytes' + ': ' + att.value;
  }
  if (this.attributes['total_frames']) {
    var att = this.attributes['total_frames'];
    att.elem.innerHTML = 'total frames' + ': ' + att.value;
  }
}

function Mux(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  this.attributes = {};
  this.inputs = [];
  this.elem = document.createElement('div');
  this.elem.classList.add('mux');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  this.plist = document.createElement('div');
  this.plist.classList.add('property_list');
  var elem = document.createElement('div');
  this.plist.appendChild(elem);
  this.attributes.type = {'elem': elem, 'value': this.crate.data.mux.type};
  var elem = document.createElement('div');
  this.plist.appendChild(elem);
  this.attributes.ntracks = {'elem': elem, 'value':
   this.crate.data.mux.ntracks};
  var elem = document.createElement('div');
  this.plist.appendChild(elem);
  this.attributes.input = {'elem': elem, 'value': this.crate.data.mux.input};
  this.desc_elem.appendChild(this.plist);
  this.update();
}

Mux.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  for (var key in this.attributes) {
    var att = this.attributes[key];
    if (Array.isArray(att.value)) {
      var str = key + ': ';
      for (var i = 0; i < att.value.length; i++) {
        str = str + att.value[i].name + ' ';
      }
      att.elem.innerHTML = str;
    } else {
      att.elem.innerHTML = key + ': ' + att.value;
    }
  }
}

function Output(crate) {
  this.crate = crate;
  this.path = this.crate.path
  this.name = this.path.split('/').pop();
  this.attributes = {};
  this.elem = document.createElement('div');
  this.elem.classList.add('output');
  this.name_elem = document.createElement('div');
  this.desc_elem = document.createElement('div');
  this.name_elem.classList.add('name');
  this.desc_elem.classList.add('short_desc');
  this.elem.appendChild(this.name_elem);
  this.elem.appendChild(this.desc_elem);
  this.plist = document.createElement('div');
  this.plist.classList.add('property_list');
  for (var key in this.crate.data.output) {
    var elem = document.createElement('div');
    this.plist.appendChild(elem);
    this.attributes[key] = { 'elem': elem,
     'value': this.crate.data.output[key] };
  }
  if (this.crate.data.output.transmission !== undefined) {
    this.listener_count_elem = document.createElement('div');
    this.desc_elem.appendChild(this.listener_count_elem);
  }
  this.desc_elem.appendChild(this.plist);
  this.update_listeners();
}

Output.prototype.update_listeners = function() {
  $.get(api_url+this.path, create_handler(this, function(data) {
    if (this.attributes['transmission'] !== undefined) {
      this.attributes['transmission'].value.txmtr.clients =
       data.output.transmission.txmtr.clients;
      this.update();
      setTimeout(create_handler(this, this.update_listeners), 1000);
    }
  }));
}

Output.prototype.update = function() {
  this.name_elem.innerHTML = this.name;
  for (var key in this.attributes) {
    var att = this.attributes[key];
    if (key === "input") {
      att.elem.innerHTML = "input: " + att.value.name;
    } else if (key === "transmission") {
      att.elem.innerHTML = "<a href='" +
       window.location.protocol + '//' + window.location.hostname + ':' +
       window.location.port + '/' + att.value.mount + "'>"
       + att.value.mount + "</a>";
      this.listener_count_elem.innerHTML = "Listeners: "
       + att.value.txmtr.clients;
    } else if (key === "record") {
      att.elem.innerHTML = "file: " + att.value.name;
    } else {
      att.elem.innerHTML = key + ': ' + att.value;
    }
  }
}

function kr_frame_size_nstr(fsz, sr) {
  var ms;
  ms = (1000 / sr) * fsz;
  if (sr % 1000) ms = Math.round(ms);
  return ms + "ms";
}

function kr_frame_size_advstr(fsz, sr) {
  var ms;
  ms = (1000 / sr) * fsz;
  if (sr % 1000) ms = Math.round(ms * 10) / 10;
  return ms + "ms [" + fsz + " samples]";
}

function kr_bit_rate_nstr(bits) {
  if (bits < 320000) {
    return bits / 1000 + "kbps";
  } else {
    return bits / 1000000 + "mbps";
  }
}

function kr_sample_rate_nstr(sr) {
  return sr / 1000 + "khz";
}

function kr_bit_rate_bytes_advstr(bits) {
  bytes = bits / 8;
  str = "";
  if (bits < 320000) {
    str += bits / 1000 + "kbps";
  } else {
    str += bits / 1000000 + "mbps";
  }
  if (bits < 320000) {
    str += " [ " + bytes / 1000 + " KB/s ]";
  } else {
    str += " [ " + bytes / 1000000 + " MB/s ]";
  }
  return str;
}
