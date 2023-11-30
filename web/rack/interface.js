$(document).ready(function() {
  rack_units.push({'constructor': Clumper, 'aspect': { dev: [16,13],
   audio: [18, 9]}, 'page': { dev: 'session', audio: 'system' },
   'position': { dev: [16, 1], audio: [0,6] }});
});

function Clumper(info_object) {
  this.title = "Clumper";
  this.description = "Save/Restore clumps of api commands.";
  this.id = "clumper";
  this.div = "<div class='kr_clumper RCU' id='" + this.id + "'></div>";
  info_object['parent_div'].append(this.div);
  this.sel = "#" + this.id;
  this.resizeTimer;
  this.current_recording = null;
  this.recordings = {};
  $(this.sel).append('\
   <div class="clumper robo_light offwhite streamypine">\
     <input type="file" accept=".krad" class= "cl_file_input"\
      name="cl_file_input" id="cl_file_input" multiple>\
     <div class="verybig bold">Command Recorder</div>\
     <div class="cl_recorder">\
       <div class="cl_new_recording">\
         <div>\
           <input class="big offwhite sans" type="text" id="recording_name"\
            name="recording_name" value="My Recording" required></input>\
           <div id="cl_controls">\
             <div class="kr_push_button red"\
              id="record_start_button"><em>RECORD</em></div>\
             <div class="kr_push_button red disabled"\
              id="record_abort_button"><em>ABORT</em></div>\
             <div class="kr_push_button blue disabled"\
              id="record_save_button"><em>SAVE</em></div>\
           </div>\
         </div>\
       </div>\
     </div>\
     <div class="cl_saved" id="cl_saved">\
       <div class="cl_export" id="cl_export"></div>\
       <div class="cl_export_controls">\
         <div class="button_group" id="import_buttons">\
           <div class="kr_push_button blue" id="import_button">\
             <em>Upload</em>\
           </div>\
         </div>\
          <div class="button_group" id="export_buttons">\
           <div class="kr_push_button red disabled" id="clear_export_button">\
             <em>CLEAR</em>\
           </div>\
           <div class="kr_push_button blue disabled" id="download_button">\
             <em>DOWNLOAD</em>\
           </div>\
           <div class="kr_push_button blue disabled"\
            id="download_all_button"><em>download all</em></div>\
         </div>\
      </div>\
       <div id="saved_recs"></div>\
     </div>\
   </div>');
  this.import_button = document.getElementById("import_button");
  this.import_button.addEventListener('click', create_handler(this,
   this.on_import_click));
  this.download_all_button = document.getElementById("download_all_button");
  this.download_all_button.addEventListener('click', create_handler(this,
   this.on_download_all_click));
  this.file_input = document.getElementById("cl_file_input");
  this.file_input.addEventListener('change', create_handler(this,
   this.on_import_file_select), false);
  this.clear_export_button = document.getElementById("clear_export_button");
  this.clear_export_button.addEventListener('click', create_handler(this,
   this.on_clear_export_click));
  this.download_button = document.getElementById("download_button");
  this.download_button.addEventListener('click', create_handler(this,
   this.on_download_click));
  this.export_elem = document.getElementById("cl_export");
  this.abort_button = document.getElementById("record_abort_button");
  this.abort_button.addEventListener('click', create_handler(this,
   this.on_abort_click));
  this.save_button = document.getElementById("record_save_button");
  this.save_button.addEventListener('click', create_handler(this,
   this.on_save_click));
  this.controls_elem = document.getElementById("cl_controls");
  this.saved_list = document.getElementById("saved_recs");
  this.name_input = document.getElementById("recording_name");
  this.record_button = document.getElementById("record_start_button");
  this.record_button.addEventListener('click', create_handler(this,
   this.on_record_click));
  this.load_saved_list();
  this.modes = {NORMAL: 0, RECORD: 1};
  this.set_mode(this.modes.NORMAL);
  this.viewed_export = null;
  this.viewing_export = false;
}

Clumper.prototype.on_import_click = function(evt) {
  this.file_input.click();
}

Clumper.prototype.download_to_file = function(file_name, text) {
  var text_blob = new Blob([text], {type:'text/plain'});
  var download_link = document.createElement("a");
  download_link.download = file_name;
  download_link.innerHTML = "Download File";
  // Firefox requires the link to
  // be added to the DOM
  // before it can be clicked.
  download_link.href = window.URL.createObjectURL(text_blob);
  download_link.addEventListener('click', create_handler(this,
        this.destroy_clicked_element));
  download_link.style.display = "none";
  document.body.appendChild(download_link);
  download_link.click();
}

Clumper.prototype.download = function(recording) {
  var objs = [];
  objs[0] = recording.export();
  var file_name = recording.name + ".krad";
  var text = JSON.stringify(objs, null, 2);
  this.download_to_file(file_name, text);
}

Clumper.prototype.download_all = function(recordings) {
  var objs = [];
  for (var i = 0, len = recordings.length; i < len; i++) {
    objs[i] = recordings[i].recording.export();
  }
  var file_name = "all.krad";
  var text = JSON.stringify(objs, null, 2);
  this.download_to_file(file_name, text);
}

Clumper.prototype.on_download_click = function(evt) {
  if (this.viewing_export) {
    this.download(this.viewed_export.recording);
    evt.preventDefault();
  }
}

Clumper.prototype.destroy_clicked_element = function(event) {
  document.body.removeChild(event.target);
}

Clumper.prototype.on_import_file_select = function() {
  for (var i = 0, len = this.file_input.files.length; i < len; i++) {
    var file = this.file_input.files[i];
    if (file) {
      var file_reader = new FileReader();
      file_reader.addEventListener('load', create_handler(this,
       this.on_import_file_load));
      file_reader.readAsText(file, "UTF-8");
    }
  }
}

Clumper.prototype.on_import_file_load = function(evt) {
  var text = evt.target.result;
  var objs = JSON.parse(text);
  if (objs.constructor === Array) {
    for (var i = 0, len = objs.length; i < len; i++) {
      var r = new ApiSendRecording(objs[i]);
      this.save(r);
    }
  } else {
    this.save(new ApiSendRecording(objs));
  }
}

Clumper.prototype.on_clear_export_click = function(evt) {
  if (this.viewing_export) {
    this.clear_export_preview();
  }
}

Clumper.prototype.clear_export_preview = function() {
  this.download_button.classList.add("disabled");
  this.clear_export_button.classList.add("disabled");
  this.export_elem.innerHTML = "";
  this.viewed_export = null;
  this.viewing_export = false;
}

Clumper.prototype.set_mode = function(mode) {
  this.mode = mode;
}

Clumper.prototype.load_saved_list = function() {
  for (var i = 0; i < localStorage.length; i++) {
    var name = localStorage.key(i);
    var rec = new ApiSendRecording(name);
    rec.load();
    this.save(rec);
  }
}

Clumper.prototype.on_record_click = function(evt) {
  if (this.mode != this.modes.RECORD) {
    var name = this.name_input.value || "testing123";
    this.record(name);
  }
}

Clumper.prototype.record = function(name) {
  this.current_recording = new ApiSendRecording(name);
  this.current_recording.start();
  this.record_button.classList.add("flashing");
  this.save_button.classList.remove("disabled");
  this.abort_button.classList.remove("disabled");
  this.set_mode(this.modes.RECORD);
}

Clumper.prototype.add_to_saved_list = function(rec) {
  var r = {};
  this.recordings[rec.name] = r;
  r.recording = rec;
  r.saved_elem = document.createElement("div");
  r.saved_elem.classList.add("rec_list_item");
  var name_elem = document.createElement("div");
  name_elem.classList.add("kindabig");
  name_elem.classList.add("label");
  name_elem.innerHTML = r.recording.name;
  r.saved_elem.appendChild(name_elem);
  r.button_group = document.createElement("div");
  r.button_group.classList.add("controls");
  r.playback_button = document.createElement("div");
  r.playback_button.classList.add("kr_push_button");
  r.playback_button.classList.add("blue");
  var content = document.createElement("em");
  content.innerHTML = "PLAY";
  r.playback_button.appendChild(content);
  r.playback_button.addEventListener('click', create_handler(this,
   this.on_playback_click));
  r.freewheel_button = document.createElement("div");
  r.freewheel_button.classList.add("kr_push_button");
  r.freewheel_button.classList.add("blue");
  var content = document.createElement("em");
  content.innerHTML = "FREEWHEEL";
  r.freewheel_button.appendChild(content);
  r.freewheel_button.addEventListener('click', create_handler(this,
   this.on_freewheel_click));
  r.export_button = document.createElement("div");
  r.export_button.classList.add("kr_push_button");
  r.export_button.classList.add("blue");
  var content = document.createElement("em");
  content.innerHTML = "EXPORT";
  r.export_button.appendChild(content);
  r.export_button.addEventListener('click', create_handler(this,
   this.on_export_click));
  r.remove_button = document.createElement("div");
  r.remove_button.classList.add("kr_push_button");
  r.remove_button.classList.add("red");
  var content = document.createElement("em");
  content.innerHTML = "REMOVE";
  r.remove_button.appendChild(content);
  r.remove_button.addEventListener('click', create_handler(this,
   this.on_remove_click));
  r.button_group.appendChild(r.remove_button);
  r.button_group.appendChild(r.export_button);
  r.button_group.appendChild(r.playback_button);
  r.button_group.appendChild(r.freewheel_button);
  r.saved_elem.appendChild(r.button_group);
  this.saved_list.appendChild(r.saved_elem);
  if (Object.keys(this.recordings).length > 0) {
    this.activate_download_all(true);
  }
}

Clumper.prototype.on_export_click = function(evt) {
  for (var key in this.recordings) {
    var r = this.recordings[key];
    var b = r.export_button;
    var e = evt.currentTarget;
    if (b == e) {
      if (this.viewing_export) {
        this.clear_export_preview();
      }
      this.download_button.classList.remove("disabled");
      this.clear_export_button.classList.remove("disabled");
      var elem = document.createElement("pre");
      elem.innerHTML = JSON.stringify(r.recording.export(), null, 2);
      this.export_elem.appendChild(elem);
      this.viewed_export = r;
      this.viewing_export = true;
      return;
    }
  }
}

Clumper.prototype.on_download_all_click = function(evt) {
  if (this.download_all_activated) {
    var recordings = [];
    for (var key in this.recordings) {
      recordings.push(this.recordings[key]);
    }
    this.download_all(recordings);
  }
}

Clumper.prototype.activate_download_all = function(on) {
  if (on) {
    this.download_all_activated = true;
    this.download_all_button.classList.remove("disabled");
  } else {
    this.download_all_activated = false;
    this.download_all_button.classList.add("disabled");
  }
}

Clumper.prototype.on_freewheel_click = function(evt) {
  for (var key in this.recordings) {
    var r = this.recordings[key];
    var b = r.freewheel_button;
    var e = evt.currentTarget;
    if (b == e) {
      r.recording.freewheel();
      return;
    }
  }
}

Clumper.prototype.on_playback_click = function(evt) {
  for (var key in this.recordings) {
    var r = this.recordings[key];
    var b = r.playback_button;
    var e = evt.currentTarget;
    if (b == e) {
      r.recording.replay();
      return;
    }
  }
}

Clumper.prototype.on_remove_click = function(evt) {
  for (var key in this.recordings) {
    var r = this.recordings[key];
    var b = r.remove_button;
    var e = evt.currentTarget;
    if (b == e) {
      this.remove_saved(r);
      return;
    }
  }
}

Clumper.prototype.on_abort_click = function(evt) {
  if (this.mode == this.modes.RECORD) {
    this.stop();
    this.reset();
  }
}

Clumper.prototype.on_save_click = function(evt) {
  if (this.mode == this.modes.RECORD) {
    this.stop();
    this.save(this.current_recording);
    this.reset();
  }
}

Clumper.prototype.remove_saved = function(r) {
  this.saved_list.removeChild(r.saved_elem);
  r.recording.remove_from_storage();
  delete this.recordings[r.recording.name];
  if (Object.keys(this.recordings).length === 0) {
    this.activate_download_all(false);
  }
}

Clumper.prototype.stop = function() {
  this.current_recording.stop();
  this.record_button.classList.remove("flashing");
  this.save_button.classList.add("disabled");
  this.abort_button.classList.add("disabled");
  this.set_mode(this.modes.NORMAL);
}

Clumper.prototype.reset = function() {
  this.current_recording = null;
}

Clumper.prototype.save = function(rec) {
  rec.save_to_storage();
  this.add_to_saved_list(rec);
}

Clumper.prototype.update = function(crate) {
}

Clumper.prototype.shared = function(key, shared_object) {
}

function ApiSendRecording(/*name || export object*/) {
  if (typeof(arguments[0]) === 'string') {
    this.name = arguments[0];
    this.json = [];
  } else {
    this.import(arguments[0]);
  }
  this.replay_list = null;
}

ApiSendRecording.prototype.set_mode = function(mode) {
  this.mode = mode;
}

ApiSendRecording.prototype.start = function() {
  kr.add_listener_send(this);
}

ApiSendRecording.prototype.stop = function() {
  kr.remove_listener_send(this);
}

ApiSendRecording.prototype.import = function(obj) {
  this.name = obj.header.name;
  this.json = obj.commands.slice(0);
}

ApiSendRecording.prototype.export = function() {
  var obj = {};
  obj.header = { name: this.name, commands: this.json.length,
   duration: this.json[this.json.length -1].timecode
   - this.json[0].timecode };
  obj.commands = this.json.slice(0);
  return obj;
}

function command_replacer(key, value) {
  if (key == "value") {
    return 0.005 + value;
  } else {
    return value;
  }
}

ApiSendRecording.prototype.freewheel = function() {
  if (this.replay_list == null) {
    this.replay_list = this.json.slice(0);
    while (this.replay_list.length > 0) {
      kr.send_json(JSON.stringify(this.replay_list.shift().command));
    }
    this.replay_list = null;
  }
}

ApiSendRecording.prototype.replay = function() {
  if (this.replay_list == null) {
    this.replay_list = this.json.slice(0);
    if (this.replay_list.length > 0) {
      this.first_command_time = this.replay_list[0].timecode;
      this.first_replay_time = Date.now();
      var tot_time = this.replay_list[this.replay_list.length-1].timecode
       - this.first_command_time;
      kr.debug('replay time: ' + tot_time);
      kr.send_json(JSON.stringify(this.replay_list.shift().command));
      this.replay();
    }
  } else {
    var delt = Date.now() - this.first_replay_time;
    var current = this.replay_list.shift();
    var delt_commands = current.timecode - this.first_command_time;
    if (delt_commands > delt) {
      this.replay_list.unshift(current);
    } else {
      while (delt_commands <= delt) {
        kr.send_json(JSON.stringify(current.command));
        if  (this.replay_list.length > 0) {
          var test_current = this.replay_list[0];
          delt_commands = test_current.timecode - this.first_command_time;
          if (delt_commands <= delt) current = this.replay_list.shift();
        } else {
          break;
        }
      }
    }
    if (this.replay_list.length > 0) {
      var del = this.replay_list[0].timecode - this.first_command_time;
      del = del - (Date.now() - this.first_replay_time);
      if (del <= 0) {
        this.replay();
      } else {
        setTimeout(create_handler(this, this.replay), del);
      }
    } else {
      this.replay_list = null;
      kr.debug('playback time: ' + (Date.now() - this.first_replay_time));
    }
  }
}

ApiSendRecording.prototype.save_to_storage = function() {
  localStorage.setItem(this.name, JSON.stringify(this.json));
}

ApiSendRecording.prototype.remove_from_storage = function() {
  localStorage.removeItem(this.name);
}

ApiSendRecording.prototype.load = function() {
  try {
    this.json = JSON.parse(localStorage.getItem(this.name));
  } catch(err) {
    kr.debug("couldn't parse localStorage json");
  }
}

ApiSendRecording.prototype.event = function(api_command) {
  if (this.json.length == 0) {
    this.first_time = Date.now();
  }
  var c = {'timecode': Date.now() - this.first_time,
   'command': JSON.parse(api_command)};
  this.json.push(c);
}

function CemoCreator() {
  this.encs = { 'audio': ['opus', 'flac', 'mp3', 'aac'],
                'video': ['vpx', 'y4m', 'h264', 'theora'],
                'opus': create_handler(this, this.gen_opus_input),
                'flac': create_handler(this, this.gen_flac_input),
                'mp3': create_handler(this, this.gen_mp3_input),
                'aac': create_handler(this, this.gen_aac_input),
                'vpx': create_handler(this, this.gen_vpx_input),
                'h264': create_handler(this, this.gen_h264_input),
                'theora': create_handler(this, this.gen_theora_input)
  };
}

CemoCreator.prototype.get_emo_tracks = function(cb) {
  tracks = {encoders: 0, muxers: 0, outputs: 0};
  var me = this;
  $.get(api_url+"/studio/tracker", function(data) {
    tracks.encoders = 0;
    tracks.muxers = 0;
    tracks.outputs = 0;
    var paths = data;
    paths.forEach(function(path, i) {
      $.get(api_url+path, function(data) {
        var spath = data;
        if (spath.coded_audio !== undefined
         || spath.coded_video !== undefined) tracks.encoders++;
        if (spath.hasOwnProperty("mux")) tracks.muxers++;
        if (i == (paths.length - 1)) {
          me.get_outputs(tracks, cb);
        }
      });
    });
  });
}

CemoCreator.prototype.get_plays = function(tracks, cb) {
  $.get(api_url+"/studio", function(data) {
    tracks.plays = 0;
    var paths = data;
    paths.forEach(function(path, i) {
      $.get(api_url+path, function(data) {
        var spath = data;
        if (spath.play !== undefined) tracks.plays++;
        if (i == (paths.length - 1)) {
          if (cb) cb(tracks);
        }
      });
    });
  });
}

CemoCreator.prototype.get_outputs = function(tracks, cb) {
  $.get(api_url+"/studio", function(data) {
    tracks.outputs = 0;
    var paths = data;
    paths.forEach(function(path, i) {
      $.get(api_url+path, function(data) {
        var spath = data;
        if (spath.output !== undefined) tracks.outputs++;
        if (i == (paths.length - 1)) {
          console.log(tracks);
          if (cb) cb(tracks);
        }
      });
    });
  });
}

CemoCreator.prototype.gen_audio_input = function() {
  var html;
  html = '  <h3>CHANNELS</h3> \
  <div class="channels"> \
  <span class="button" data-value="1" data-disabled="1">MONO</span> \
  <span class="button" data-value="2" data-selected="1">STEREO</span> \
  </div>';
  $('#cemo_creator div.capture div.av_options').html(html);
  $("#cemo_creator div.capture div.channels span.button").click(function() {
    $("#cemo_creator div.capture div.channels span.button[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
}

CemoCreator.prototype.gen_video_input = function() {
  var html;
  html = '  <h3>WIDTH</h3><div id="width"> \
  <input type="number" name="width" value=640></input> \
  </div>\
  <h3>HEIGHT</h3><div id="height"> \
  <input type="number" name="height" value=360></input> \
  </div>\
  <h3>FPS NUMERATOR</h3><div id="fps_num"> \
  <input type="number" name="fps_num" value=30000></input> \
  </div>\
  <h3>FPS DENOMINATOR</h3><div id="fps_den"> \
  <input type="number" name="fps_den" value=1001></input> \
  </div>\
  <h3>FORMAT</h3><div id="format"> \
  <div class="format"> \
    <span class="button" data-value="yuv420" data-selected="1">YUV420</span>\
    <span class="button" data-value="yuv422">YUV422</span>\
    <span class="button" data-value="yuv444">YUV444</span>\
    <span class="button" data-value="rgb">RGB</span>\
  </div>';
  $("#cemo_creator div.capture div.av_options").html(html);
  $("#cemo_creator div.capture #format div span.button").click(function() {
    $("#cemo_creator div.capture #format div span.button[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
}

CemoCreator.prototype.gen_brate_input = function(min, max, def, step) {
  var html;
  html = '<h3>BIT RATE</h3><div id="bitrate">\
  <input type="number" name="bitrate" min="'+min+'" max="'+max+'"\
   step="'+step+'" value="'+def+'" />\
  </div>';
  return html;
}

CemoCreator.prototype.gen_opus_input = function() {
  var html;
  html = '<h3>FRAME SIZE</h3><div id="framesizes">\
  <span class="button framesize" data-selected="1">480</span>\
  <span class="button framesize">960</span></div>';
  html += this.gen_brate_input(32, 320, 128, 32);
  $("#cemo_creator div.encode div.options").append(html);
  $("#cemo_creator div.encode span.button.framesize").click(function() {
    $("#cemo_creator div.encode span.button.framesize[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
}

CemoCreator.prototype.gen_aac_input = function() {
  var html;
  html = this.gen_brate_input(40, 576, 96, 8);
  $("#cemo_creator div.encode div.options").append(html);
}

CemoCreator.prototype.gen_mp3_input = function() {
  var html;
  html = this.gen_brate_input(32, 320, 64, 16);
  $("#cemo_creator div.encode div.options").append(html);
}

CemoCreator.prototype.gen_flac_input = function() {
  var html;
  html = '<h3>BIT DEPTH</h3><div id="bitdepths">\
  <span class="button bitdepth">16</span>\
  <span class="button bitdepth" data-selected="1">24</span></div>';
  $("#cemo_creator div.encode div.options").append(html);
  $("#cemo_creator div.encode span.button.bitdepth").click(function() {
    $("#cemo_creator div.encode span.button.bitdepth[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
}

CemoCreator.prototype.gen_h264_input =
 CemoCreator.prototype.gen_vpx_input = function() {
  var html;
  html = this.gen_brate_input(1000, 300000, 50000, 1000);
  html += '<h3>KEY RATE</h3><div id="key_rate">\
          <input type="number" name="key_rate" value=60></input> \
  </div>';
  $("#cemo_creator div.encode div.options").append(html);
}

CemoCreator.prototype.gen_theora_input = function() {
  var html;
  html = '<h3>KEY RATE</h3><div id="key_rate">\
          <input type="number" name="key_rate" value=60></input> \
          </div>\
          <h3>QUALITY</h3><div id="quality">\
          <input type="number" name="quality" value="5" min="0" max="10"\
          step="1"></input> \
          </div>\
          <h3>EOS</h3><div id="eos">\
          <input type="number" name="eos" value="0"></input> \
          </div>\
          ';
  $("#cemo_creator div.encode div.options").append(html);
}

CemoCreator.prototype.gen_xmit_input = function() {
  var html;
  html = '<h3>MOUNT</h3><input type="text"/>';
  $("#cemo_creator div.output div.options").html(html);
}

CemoCreator.prototype.gen_rec_input = function() {
  var html;
  html = '<h3>FILENAME</h3><input type="text"/>';
  $("#cemo_creator div.output div.options").html(html);
}

CemoCreator.prototype.gen_upl_input = function() {
  var html;
  html = '<h3>URL</h3><input type="text"/>';
  $("#cemo_creator div.output div.options").html(html);
}

CemoCreator.prototype.gen_capture_options = function(type) {
  switch(type) {
    case "audio":
      this.gen_audio_input();
      break;
    case "video":
      this.gen_video_input();
      break;
    default: break;
  }
}

CemoCreator.prototype.gen_encoders = function(type) {
  var html;
  $("#cemo_creator div.encode div.encoders").empty();
  html = "";
  for (var i = 0, len = this.encs[type].length; i < len; i++) {
    var enc = this.encs[type][i];
    html += '<span class="button encoder" data-value="' + enc + (i === 0 ? '" data-selected="1">' : '">') + enc.toUpperCase() + '</span>';
  }
  $("#cemo_creator div.encode div.encoders").html(html);
  this.gen_enc_options(type, $("#cemo_creator div.encoders span.button[data-selected='1']").attr("data-value"));
  $("#cemo_creator div.encoders span.button").click(create_handler(this, function(evt) {
    var encoder;
    $("#cemo_creator div.encoders span.button[data-selected='1']").attr("data-selected", "0");
    $(evt.target).attr("data-selected", "1");
    encoder = $(evt.target).attr("data-value");
    this.gen_enc_options(type, encoder);
  }));
}

CemoCreator.prototype.gen_enc_audio_options = function() {
  var html;
  html = ' \
  <h3>CHANNELS</h3>\
    <div class="channels">\
      <span class="button" data-value="1" data-disabled="1">MONO</span>\
      <span class="button" data-value="2" data-selected="1">STEREO</span>\
    </div>\
    <h3>INPUT TRACK</h3>\
    <div class="track">\
      <select></select>\
    </div>\
    <h3>SAMPLE RATE</h3>\
    <div class="samplerates">\
      <span class="button samplerate">44100</span>\
      <span class="button samplerate" data-selected="1">48000</span>\
    </div>';
  $('#cemo_creator div.encode div.options').html(html);
  $("#cemo_creator div.encode div.channels span.button").click(function() {
    if ($(this).attr("data-disabled")) return;
    $("#cemo_creator div.encode div.channels span.button[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
  $("#cemo_creator div.encode div.samplerates span.button").click(function() {
    $("#cemo_creator div.encode div.samplerates span.button[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
  this.get_input_tracks('audio');
}

CemoCreator.prototype.gen_enc_video_options = function() {
  var html;
  html = '<h3>INPUT TRACK</h3>\
   <div class="track">\
     <select></select>\
   </div>';
  $('#cemo_creator div.encode div.options').html(html);
  this.get_input_tracks('video');
}

CemoCreator.prototype.gen_enc_options = function(type, encoder) {
  switch(type) {
     case "audio":
     this.gen_enc_audio_options();
     break;
     case "video":
     this.gen_enc_video_options();
     break;
     default: break;
  }
  this.encs[encoder]();
}

CemoCreator.prototype.gen_out_options = function(encoder) {
  switch(encoder) {
    case "transmission":
    this.gen_xmit_input();
    break;
    case "record":
    this.gen_rec_input();
    break;
    case "uplink":
    this.gen_upl_input();
    break;
    default: break;
  }
}

CemoCreator.prototype.get_capture_info = function(capture) {
  var info = {};
  switch(capture) {
    case "audio":
      info.channels = parseInt($("#cemo_creator div.capture div.channels span.button[data-selected='1']").attr("data-value"));
      break;
    case "video" :
      info.width = parseInt($("#cemo_creator div.capture input[name='width']").val());
      info.height = parseInt($("#cemo_creator div.capture input[name='height']").val());
      info.fps_num = parseInt($("#cemo_creator div.capture input[name='fps_num']").val());
      info.fps_den = parseInt($("#cemo_creator div.capture input[name='fps_den']").val());
      info.format = $("#cemo_creator div.capture div.format span.button[data-selected='1']").attr('data-value');
      break;
    default: break;
  }
  return info;
}

CemoCreator.prototype.get_enc_info = function(av_type, encoder) {
  var info = {};
  switch(av_type) {
    case 'audio':
    info.channels = parseInt($("#cemo_creator div.encode div.channels span.button[data-selected='1']").attr("data-value"));
    info.sample_rate = parseInt($("#cemo_creator div.encode div.samplerates span.button[data-selected='1']").text());
    switch(encoder) {
      case "opus":
      info.bit_rate = parseInt($("#cemo_creator div.encode input[name='bitrate']").val()) * 1000;
      info.frame_size = parseInt($("#cemo_creator div.encode span.button.framesize[data-selected='1']").text());
      break;
      case "flac":
      info.bit_depth = parseInt($("#cemo_creator div.encode span.button.bitdepth[data-selected='1']").text());
      break;
      case "mp3":
      info.bit_rate = parseInt($("#cemo_creator div.encode input[name='bitrate']").val()) * 1000;
      break;
      case "aac":
      info.bit_rate = parseInt($("#cemo_creator div.encode input[name='bitrate']").val()) * 1000;
      break;
      default: break;
    }
    case "video":
    switch(encoder) {
        case "vpx":
          info.bit_rate = parseInt($("#cemo_creator div.encode input[name='bitrate']").val());
          info.key_rate = parseInt($("#cemo_creator div.encode input[name='key_rate']").val());
          break;
        case "h264":
          info.bit_rate = parseInt($("#cemo_creator div.encode input[name='bitrate']").val());
          info.key_rate = parseInt($("#cemo_creator div.encode input[name='key_rate']").val());
          break;
        case "theora":
          info.key_rate = parseInt($("#cemo_creator div.encode input[name='key_rate']").val());
          info.eos = parseInt($("#cemo_creator div.encode input[name='eos']").val());
          info.quality = parseInt($("#cemo_creator div.encode input[name='quality']").val());
          break;
        default: break;
    }
  }
  return info;
}

CemoCreator.prototype.get_out_info = function(output) {
  var info = {};
  switch(output) {
    case "transmission":
      info.mount = $("#cemo_creator div.output div.options input[type='text']").val();
      info.txmtr = {type: "http"};
      break;
    case "record":
      info.name = $("#cemo_creator div.output div.options input[type='text']").val();
      info.len = info.name.length;
      break;
    case "uplink":
      info.url = $("#cemo_creator div.output div.options input[type='text']").val();
      info.len = info.url.length;
      break;
    default: break;
  }
  return info;
}

CemoCreator.prototype.create_capture = function(track, type, info) {
  var obj = {};
  var data = {};
  if (!track || track == "") return;
  if (!type) return;
  if (!info) return;
  obj.meth = "PUT";
  obj.path = "/studio/"+track;
  obj.meta = {};
  data.capture = {};
  switch(type) {
    case "audio":
    data.capture.audio = {};
    data.capture.audio.channels = info.channels;
    data.capture.audio.track = {name: track, len: track.length};
    break;
    case "video":
    data.capture.video = {};
    data.capture.video.width = info.width;
    data.capture.video.height = info.height;
    data.capture.video.fps_num = info.fps_num;
    data.capture.video.fps_den = info.fps_den;
    data.capture.video.format = info.format;
    break;
    default: break;
  }
  obj.data = data;
  console.log(JSON.stringify(obj));
  kr.send_json(JSON.stringify(obj));
}

CemoCreator.prototype.create_encoder = function(track, type, av_type, info) {
  var obj = {};
  var data = {};
  if (!track || track == "") return;
  if (!type || type == "") return;
  if (!info) return;
  switch (av_type) {
    case 'audio':
      obj.meth = "PUT";
      obj.path = "";
      obj.meta = {};
      data.encode = {};
      data.encode.in = {name: track, len: track.length};
      data.encode.out = {name: "roar", len: 4};
      data.encode.audio = {};
      data.encode.audio[type] = info;
      obj.data = data;
      this.get_emo_tracks(function(tracks) {
        obj.path = "/studio/Encode"+tracks.encoders;
        console.log(JSON.stringify(obj));
        kr.send_json(JSON.stringify(obj));
      });
      break;
    case 'video':
      var in_path = '/studio/' + track;
      var me = this;
      $.get(api_url+in_path, function(result) {
          info.width = result.capture.video.width;
          info.height = result.capture.video.height;
          info.fps_num = result.capture.video.fps_num;
          info.fps_den = result.capture.video.fps_den;
          info.pixel_fmt = result.capture.video.format;
          obj.meth = "PUT";
          obj.path = "";
          obj.meta = {};
          data.encode = {};
          data.encode.in = {name: track, len: track.length};
          data.encode.out = {name: "roar", len: 4};
          data.encode.video = {};
          data.encode.video[type] = info;
          obj.data = data;
          me.get_emo_tracks(function(tracks) {
            obj.path = "/studio/Encode"+tracks.encoders;
            console.log(JSON.stringify(obj));
            kr.send_json(JSON.stringify(obj));
          });
        });
      break;
  }
}

CemoCreator.prototype.create_muxer = function(tracks, type) {
  var obj = {};
  var data = {};
  var ntracks;
  if (!tracks || tracks[0] == "") return;
  if (!type) return;
  obj.meth = "PUT";
  obj.path = "";
  obj.meta = {};
  data.mux = {};
  data.mux.input = [];
  ntracks = 0;
  for (var i = 0, len = tracks.length; i < len; i++) {
    if (tracks[i] !== "") {
      data.mux.input.push({name: tracks[i], len: tracks[i].length});
      ntracks++;
    }
  }
  data.mux.ntracks = ntracks;
  data.mux.type = type;
  obj.data = data;
  this.get_emo_tracks(function(tracks) {
    obj.path = "/studio/Mux"+tracks.muxers;
    console.log(JSON.stringify(obj));
    kr.send_json(JSON.stringify(obj));
  });
}

CemoCreator.prototype.create_output = function(track, type, info) {
  var obj = {};
  var data = {};
  if (!track || track == "") return;
  if (!type) return;
  if (!info) return;
  obj.meth = "PUT";
  obj.path = "";
  obj.meta = {};
  data.output = {};
  data.output.input = {name: track, len: track.length};
  data.output[type] = info;
  obj.data = data;
  this.get_emo_tracks(function(tracks) {
    obj.path = "/studio/Out"+tracks.outputs;
    console.log(JSON.stringify(obj));
    kr.send_json(JSON.stringify(obj));
  });
}

CemoCreator.prototype.create_play = function(track) {
  var obj = {};
  var data = {};
  var tracks = {};
  var ntracks;
  if (!track) return;
  obj.meth = "PUT";
  obj.path = "";
  obj.meta = {};
  data.play = {};
  data.play.input = ({name: track, len: track.length});
  obj.data = data;
  this.get_plays(tracks, function(retracks) {
    obj.path = "/studio/Play"+retracks.plays;
    console.log(JSON.stringify(obj));
    kr.send_json(JSON.stringify(obj));
  });
}


CemoCreator.prototype.get_input_tracks = function (type) {
  var inputs = {};
  switch(type) {
    case 'audio':
      $.get(api_url+"/studio/tracker", function(data) {
        var paths = data;
        paths.forEach(function(path) {
          $.get(api_url+path, function(data) {
            var mpath = data;
            if (mpath.audio !== undefined) {
              inputs[path] = data;
             $("#cemo_creator div.encode div.track select").empty();
              for (var key in inputs) {
                path = inputs[key].track.name;
                $("#cemo_creator div.encode div.track select").append('<option value="'+path+'">'+path+'</option>');
              }
            }
          });
        });
      });
      break;
    case 'video':
      $.get(api_url+"/studio/tracker", function(data) {
        var paths = data;
        paths.forEach(function(path) {
          $.get(api_url+path, function(data) {
            var mpath = data;
            if (mpath.video !== undefined) {
              inputs[path] = data;
              $("#cemo_creator div.encode div.track select").empty();
              for (var key in inputs) {
                path = mpath.track.name;
                $("#cemo_creator div.encode div.track select").append('<option value="'+path+'">'+path+'</option>');
              }
            }
          });
        });
      });
      break;
    default: break
  }
  return inputs;
}

CemoCreator.prototype.get_encode_tracks = function (type) {
  if (type === undefined) type = 'mux';
  var encoders = {};
  $.get(api_url+"/studio/tracker", function(data) {
    var paths = data;
    $("#cemo_creator div."+type+" div.track select").empty();
    $("#cemo_creator div."+type+" div.track select").append('<option value=""></option>');
    paths.forEach(function(path) {
      $.get(api_url+path, function(data) {
        var spath = data;
        if (spath.hasOwnProperty("coded_audio")
         || spath.hasOwnProperty("coded_video")) {
          encoders[path] = data;
          path = path.split("/").pop();
          $("#cemo_creator div."+type+" div.track select").append('<option value="'+path+'">'+path+'</option>');
        }
      });
    });
  });
}

CemoCreator.prototype.get_mux_tracks = function() {
  $.get(api_url+"/studio/tracker", function(data) {
    var paths = data;
    $("#cemo_creator div.mux div.track select").empty();
    paths.forEach(function(path) {
      $.get(api_url+path, function(data) {
        var spath = data;
        if (spath.hasOwnProperty("mux")) {
          path = path.split("/").pop();
          $("#cemo_creator div.output div.track select").append('<option value="'+path+'">'+path+'</option>');
        }
      });
    });
  });
}
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
function zpad(s) {
    if (s > 9) return s;
    return '0' + s;
}

function spad(s) {
    if (s > 9) return s;
    return ' ' + s;
}

function ms_to_padtime(ms) {
    var milisecs = ms % 1000;
    var s = (ms - milisecs) / 1000;
    var secs = s % 60;
    s = (s - secs) / 60;
    var mins = s % 60;
    var hours = (s - mins) / 60;
    if (hours) {
        return spad(hours) + ':' + zpad(mins) + ':' + zpad(secs);
    } else {
        return "   " + spad(mins) + ':' + zpad(secs);
    }
}

function local_mouse(elem, evt) {
    var rect = elem.getBoundingClientRect();
    var x = evt.pageX - rect.left - window.pageXOffset;
    var y = evt.pageY - rect.top - window.pageYOffset;
    return {
        x: x,
        y: y
    };
}

function Rect(x, y, w, h, cb) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.cb = cb;
}

Rect.prototype.hit = function (x, y) {
    if (this.x <= x && this.x + this.w >= x && this.y <= y && this.y + this.h >= y) {
        return true;
    } else {
        return false;
    }
};

function Control(rect, cb) {
    this.rect = rect;
    this.cb = cb;

}

PlaybackControl.prototype.push_prev = function () {
    console.log("prev! " + this.w);
    this.prev_pushed = 1;
    this.client.playlist.set_next_rel(-1).onvalue = create_handler(this, this.tickle_me_l8r);  
};

PlaybackControl.prototype.prev_button = function (c, x, y, label) {
    var w = 84;
    var t;
    var tx;
    var ty;
    c.shadowColor = "#444";
    c.shadowOffsetY = 2;
    c.shadowOffsetX = 1;
    c.shadowBlur = this.shadowBlur;
    if (this.prev_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
        this.prev_pushed++;
        this.damage++;
        if (this.prev_pushed > 2) this.prev_pushed = 0;
    }
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fillStyle = '#444';
    c.fill();
    c.strokeStyle = '#422';
    //c.lineWidth = 3.5;
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    c.shadowBlur = this.shadowBlur;
    if (this.prev_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
    }
    c.stroke();
    c.closePath();
    c.fillStyle = '#2fa';
    c.shadowColor = "#4fa";
    c.shadowBlur = this.shadowBlur;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.save();
    t = 9;
    c.beginPath();
    tx = x + 25;
    ty = y + 24;
    c.translate(tx, ty);
    c.translate(t * 4, t * 2);
    c.rotate(135 * (Math.PI * 1));
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    t = 9;
    c.beginPath();
    tx = tx + t * 2;
    ty = ty + t;
    c.translate(t * 2, 0);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    c.translate(t * 2, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.push_prev)));
};

PlaybackControl.prototype.tickle_me_l8r = function () {
  this.client.playback.tickle();
};

PlaybackControl.prototype.push_next = function () {
    console.log("next! " + this.w);
    this.next_pushed = 1;
    this.client.playlist.set_next_rel(1).onvalue = create_handler(this, this.tickle_me_l8r);  
};

PlaybackControl.prototype.next_button = function (c, x, y, label) {
    var w = 84;
    var t;
    var tx;
    var ty;
    c.shadowColor = "#444";
    c.shadowOffsetY = 2;
    c.shadowOffsetX = 1;
    c.shadowBlur = this.shadowBlur;
    if (this.next_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
        this.next_pushed++;
        this.damage++;
        if (this.next_pushed > 2) this.next_pushed = 0;
    }
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fillStyle = '#444';
    c.fill();
    c.strokeStyle = '#422';
    //c.lineWidth = 3.5;
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    c.shadowBlur = this.shadowBlur;
    if (this.next_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
    }
    c.stroke();
    c.closePath();
    c.fillStyle = '#2fa';
    c.shadowColor = "#4fa";
    c.shadowBlur = this.shadowBlur;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.save();
    t = 9;
    c.beginPath();
    tx = x + 25;
    ty = y + 24;
    c.translate(tx, ty);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    t = 9;
    c.beginPath();
    tx = tx + t * 2;
    ty = ty + t;
    c.translate(t * 2, 0);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    c.translate(t * 2, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.push_next)));
};

PlaybackControl.prototype.play_pushed = function () {
    console.log("play! " + this.w);
    if (this.playback_state == 1) {
      /*this.playback_state = 2;*/
      this.client.playback.pause();
    } else {
      /*this.playback_state = 1;*/
      this.client.playback.start();
    }
};

PlaybackControl.prototype.play_button = function (c, x, y, label) {
    var w = 84;
    var t;
    var tx;
    var ty;
    c.shadowColor = "#444";
    c.shadowOffsetY = 2;
    c.shadowOffsetX = 1;
    c.shadowBlur = this.shadowBlur;
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fillStyle = '#152';
    c.strokeStyle = '#422';
    if (this.playback_state == 1) {
        c.fillStyle = '#1f1';
        c.strokeStyle = '#1a2';
        c.shadowColor = "#af6";
        c.shadowBlur = 1;
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
    }
    if (this.playback_state == 2) {
        c.fillStyle = '#2b6';

        c.strokeStyle = 'yellow';
        c.shadowColor = "yellow";
        c.shadowBlur = 6;
    }
    c.fill();
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    //c.shadowBlur = this.shadowBlur;
    c.stroke();
    c.closePath();
    c.fillStyle = '#2fa';
    c.shadowColor = "#4fa";
    c.shadowBlur = 0;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.save();
    if (this.playback_state == 2) {
        c.fillStyle = 'yellow';

        c.strokeStyle = 'yellow';
        c.shadowColor = "yellow";
        c.shadowBlur = 0 + (this.nframes % 40) / 5;
    }
    t = 12;
    c.beginPath();
    tx = x + 18;
    ty = y + 22;
    c.translate(tx, ty);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    //c.fill();
    c.translate(t * 2.2, 0);
    c.save();
    c.lineWidth = 1.5;
    c.beginPath();
    c.strokeStyle = '#ddd';
    if (this.playback_state == 2) {
        c.strokeStyle = 'yellow';
    }
    c.moveTo(t / 2, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    c.lineWidth = 5;
    c.translate(t * 1.2, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    if (this.playback_state == 2) {
        c.strokeStyle = 'yellow';
    }
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.translate(t / 1.8, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    if (this.playback_state == 2) {
        c.strokeStyle = 'yellow';
    }
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.play_pushed)));
};

PlaybackControl.prototype.stop_pushed = function () {
    console.log("stop! " + this.w);
    this.client.playback.stop();
    /*this.playback_state = 0;*/
};

PlaybackControl.prototype.stop_button = function (c, x, y, label) {
    var w = 84;
    var shadow = this.shadowblur;
    c.strokeStyle = '#422';
    c.fillStyle = '#c10000';
    c.shadowColor = "#d11";

    c.shadowColor = "#666";
    c.shadowOffsetY = 1;
    c.shadowOffsetX = 1;
    c.shadowBlur = 4;


    if (this.playback_state === 0) {
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.strokeStyle = '#900';
        c.fillStyle = '#f11';
        c.shadowColor = "#a44";
        shadow = 0;
    }
    c.shadowBlur = shadow;
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fill();
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    c.stroke();
    c.closePath();
    c.fillStyle = '#222';
    c.shadowColor = "#4f0";
    c.shadowBlur = shadow;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.rect(x + 30, y + 22, 22, 22);
    c.fill();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.stop_pushed)));
};


PlaybackControl.prototype.divider = function (c) {

    c.save();


    if (this.playback_state === 0) {
        c.strokeStyle = '#c43';
    } else if (this.playback_state === 1) {
        c.strokeStyle = '#4f2';
    } else if (this.playback_state === 2) {
        c.strokeStyle = 'yellow';
    }


    c.shadowColor = "#eee";
    c.shadowOffsetY = 1;
    c.shadowOffsetX = 1;
    c.shadowBlur = 6;


    c.beginPath();
    c.moveTo(0, 70);
    c.lineTo(440, 70);
    c.lineWidth = 2;
    c.stroke();
    c.closePath();
    c.beginPath();
    c.moveTo(0, 120);
    c.lineTo(440, 120);
    c.lineWidth = 2;
    c.stroke();
    c.closePath();
    c.restore();
};

PlaybackControl.prototype.buttons = function (c) {
    var x = 32;
    var y = 142;
    var m = 100;
    c.save();
    c.font = "Bold 24px Sans";
    c.textBaseline = "middle";
    c.textAlign = "center";
    c.lineWidth = 2;
    this.shadowBlur = 5;
    this.prev_button(c, x, y, "PREV");
    x += m;
    this.next_button(c, x, y, "NEXT");
    x += m;
    this.play_button(c, x, y, "PLAY");
    x += m;
    this.stop_button(c, x, y, "STOP");
    c.restore();
};

PlaybackControl.prototype.render_playtime = function (c) {
    var tc = ms_to_padtime(this.playtime);
    c.fillStyle = '#eee';
    c.font = "Bold 32px Monospace";
    //c.textBaseline = "middle";
    c.textAlign = "left";
    c.fillText(tc, 32, 106);
};

PlaybackControl.prototype.tracktime = function (c) {
    var tc;
    if (this.track.duration === undefined) {
      tc = "Infinite";
    } else { 
      tc = ms_to_padtime(this.track.duration);
    }
    c.fillStyle = '#ddd';
    c.font = "Bold 32px Monospace";
    //c.textBaseline = "middle";
    c.textAlign = "left";
    c.fillText(tc, 256, 106);
};

PlaybackControl.prototype.render_track_info = function (c) {
    var artist;
    var title;
    if (this.track.artist === undefined) {
      artist = "No Data";
    } else { 
      artist = this.track.artist;
    }
    if (this.track.title === undefined) {
      title = "";
    } else { 
      title = this.track.title;
    }
    c.fillStyle = '#eee';
    c.font = "Bold 16px Sans";
    c.textBaseline = "middle";
    c.textAlign = "left";
    c.fillText(title, 32, 24);
    c.font = "Bold 24px Sans";
    c.fillText(artist, 32, 50);
};

PlaybackControl.prototype.render = function (c) {
    if (this.scale) {
        c.save();
        c.scale(window.devicePixelRatio, window.devicePixelRatio);
    }
    this.buttons(c);
    this.render_playtime(c);
    this.tracktime(c);
    this.divider(c);
    this.render_track_info(c);
    if (this.scale) c.restore();
};

PlaybackControl.prototype.push = function (e) {
    var pos = local_mouse(this.surface, e);
    if (!this.scale) {
        pos.x = Math.floor(pos.x * window.devicePixelRatio);
        pos.y = Math.floor(pos.y * window.devicePixelRatio);
    }
    for (var i = 0; i < this.controls.length; i++) {
        var control = this.controls[i];
        if (control.rect.hit(pos.x, pos.y)) {
            control.cb();
            this.damage = 1;
            break;
        }
    }
};

/*function create_handler(inst, func) {
    return (function (e) {
        func.call(inst, e);
    });
}*/

PlaybackControl.prototype.debug = function (c) {
    c.beginPath();
    c.fillStyle = '#fff';
    c.font = "14px Sans";
    c.fillText("Frame# " + this.nframes, this.w - this.w / 3, this.h - 14);
    c.closePath();
};

PlaybackControl.prototype.clear = function (c) {
    c.beginPath();
    c.rect(0, 0, this.w, this.h);
    c.fillStyle = '#999';
    c.fill();
    c.closePath();
};

PlaybackControl.prototype.tick = function () {
    this.now = new Date();
    this.hour = this.now.getHours();
    this.minute = this.now.getMinutes();
    this.second = this.now.getSeconds();
    this.ms = this.now.getMilliseconds();
};

PlaybackControl.prototype.update = function () {
    if (this.resized) {
        this.update_dimensions();
    }
    if (this.damage) {
        var c = this.surface.getContext("2d");
        this.clear(c);
        this.tick();
        this.render(c);
        /*this.debug(c);*/
        this.nframes = this.nframes + 1;
        this.damage--;
        if (this.damage < 0) this.damage = 0;
    }
    window.requestAnimationFrame(this.loop);
};

PlaybackControl.prototype.update_dimensions = function () {
    var dpr = window.devicePixelRatio;
    var rect = this.container.getBoundingClientRect();
    var w = Math.floor(rect.width);
    var h = Math.floor(rect.height);
    this.surface.style.width = w + "px";
    this.surface.style.height = h + "px";
    this.w = Math.floor(w * dpr);
    this.h = Math.floor(h * dpr);
    this.surface.width = this.w;
    this.surface.height = this.h;
    this.resized = 0;
};

PlaybackControl.prototype.handle_resize = function () {
    this.resized = 1;
    this.damage = 1;
};

PlaybackControl.prototype.setup = function (container) {
    this.controls = [];
    this.nframes = 0;
    this.scale = 1;
    this.container = container;
    this.surface = document.createElement("canvas");
    this.surface.id = "playbackcontrol_" + this.container.id;
    this.container.appendChild(this.surface);
    this.update_dimensions();
    this.surface.onmousedown = create_handler(this, this.push);
    window.onresize = create_handler(this, this.handle_resize);
    this.loop = create_handler(this, this.update);
    this.damage = 1;
    window.requestAnimationFrame(this.loop);
};

PlaybackControl.prototype.set_playback_state = function (state) {
    this.playback_state = state;
    this.damage = 1;
};

PlaybackControl.prototype.set_track_info = function (info) {
    this.track = info;
    this.damage = 1;
};

PlaybackControl.prototype.set_playtime = function (playtime) {
    this.playtime = playtime;
    this.damage = 1;
};

function PlaybackControl(div, client) {
    this.client = client;
    this.setup(div);
    this.playback_state = 0;
    this.next_pushed = 0;
    this.prev_pushed = 0;
    this.playtime = 0;
    this.track = {};
}
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
$(document).ready(function() {
  rack_units.push({'constructor': Maker, 'aspect': [16,13],
  'page': 'create', 'position': [16, 1]});
});

function Maker(info_object) {
  this.title = "Maker";
  this.description = "Make stuff.";
  this.id = 'maker';
  this.div = "<div class='maker RCU danred' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  this.address_masks = [];
  info_object['parent_div'].append(this.div);
  kr.maker = this;
  $(this.sel).append('\
<select id="make_what" onchange=kr.maker.handleSelect(this.value)>\
<option value="form_create_path_v4l2">V4l2</option>\
<option value="form_create_path_decklink">Decklink</option>\
<option value="form_create_path_wayland">Wayland Window</option>\
<option value="form_create_path_jack">Jack</option>\
</select>');
  $(this.sel).append('\
<form class="creation_form" id="form_create_path_v4l2">\
<div class="very_big">Create v4l2 Path</div>\
Camera Name:<input type="text" id="v4l2_cam_name" name="v4l2_cam_name" required><br>\
Capture Name:<input type="text" id="v4l2_cap_name" name="v4l2_cap_name" required><br>\
Device:\
<select name="device">\
<option value="0">/dev/video0</option>\
<option value="1">/dev/video1</option>\
<option value="2">/dev/video2</option>\
<option value="3">/dev/video3</option>\
</select>\
<br>\
Resolution and Framerate:\
<select name="mode">\
  <optgroup label="640x360">\
    <option value="640x360_30">640x360 30/1 fps</option>\
  </optgroup>\
  <optgroup label="640x480">\
    <option value="640x480_30">640x480 30/1 fps</option>\
  </optgroup>\
  <optgroup label="960x540">\
    <option value="960x540_30">960x540 30/1 fps</option>\
  </optgroup>\
  <optgroup label="1280x720">\
    <option value="1280x720_30">1280x720 30/1 fps</option>\
  </optgroup>\
  <optgroup label="1920x1080">\
    <option value="1920x1080_30">1920x1080 30/1 fps</option>\
  </optgroup>\
  <optgroup label="1920x540">\
    <option value="1920x540_30">1920x540 30/1 fps</option>\
  </optgroup>\
<</select>\
<br>\
<br>\
<input class="kr_push_button blue" type="submit" value="Create"\
 id="button_create_path_v4l2"></input>\
</form>');

  $('#form_create_path_v4l2').submit(function( event ) {
    event.preventDefault();
    var json;
    var dev = event.currentTarget.device.value;
    var cam = $('#v4l2_cam_name').val();
    var cap = $('#v4l2_cap_name').val();
    var obj = {};
    obj.meth = "PUT";
    obj.path = "/xpdr/" + cam;
    obj.meta = {};
    var data = {};
    data.v4l2 = {};
    data.v4l2.dev = parseInt(dev, 10);
    data.v4l2.priority = 0;
    data.v4l2.state = "void";
    obj.data = data;
    kr.send_json(JSON.stringify(obj));
    var w, h;
    if (event.currentTarget.mode.value == "1920x540_30") {
      w = 1920;
      h = 540;
    } else if (event.currentTarget.mode.value == "1920x1080_30") {
      w = 1920;
      h = 1080;
     } else if (event.currentTarget.mode.value == "960x540_30") {
      w = 960;
      h = 540;
     } else if (event.currentTarget.mode.value == "1280x720_30") {
      w = 1280;
      h = 720;
    } else if (event.currentTarget.mode.value == "640x360_30") {
      w = 640;
      h = 360;
    } else if (event.currentTarget.mode.value == "640x480_30") {
      w = 640;
      h = 480;
    }
    var obj = {};
    obj.meth = "PUT";
    obj.path = "/xpdr/" + cam + "/" + cap;
    obj.meta = {};
    var data = {};
    data.v4l2_in = {};
    data.v4l2_in.width = w;
    data.v4l2_in.height = h;
    data.v4l2_in.num = 30;
    data.v4l2_in.den = 1;
    data.v4l2_in.format = 0
    obj.data = data;
    kr.send_json(JSON.stringify(obj));
  });

  $(this.sel).append('\
<form style="display: none" class="creation_form" id="form_create_path_decklink">\
<div class="very_big">Create Decklink Path</div>\
Camera Name:<input type="text" id="dl_cam_name" name="dl_cam_name" required><br>\
Capture Name:<input type="text" id="dl_cap_name" name="dl_cap_name" required><br>\
Device Num:<input type="text" id="dl_dev_num" name="dl_dev_num" required><br>\
<br>\
Resolution and Framerate:\
<br>\
<br>\
Width:<input type="text" id="dl_cap_width" name="dl_cap_width" required><br>\
Height:<input type="text" id="dl_cap_height" name="dl_cap_height" required><br>\
Framerate Numerator:<input type="text" id="dl_fr_num" name="dl_fr_num" required><br>\
Framerate Denominator:<input type="text" id="dl_fr_den" name="dl_fr_den" required><br>\
<br>\
<input class="kr_push_button blue" type="submit" value="Create" id="button_create_path_decklink"></input>\
</form>');

  $('#form_create_path_decklink').submit(function( event ) {
    event.preventDefault();
    var json;
    var cam_name = $('#dl_cam_name').val();
    var cap_name = $('#dl_cap_name').val();
    var dev_num = $('#dl_dev_num').val();
    json = '{\
      "meth": "PUT",\
      "path": "/xpdr/' + cam_name + '",\
      "data": {\
        "decklink": {\
          "name": "' + cam_name + '",\
          "num": ' + dev_num + '\
        }\
      }\
    }';
    kr.send_json(json);

    var w = $('#dl_cap_width').val();
    var h = $('#dl_cap_height').val();

    var num = $('#dl_fr_num').val();
    var den = $('#dl_fr_den').val();

    json = '{\
        "meth": "PUT",\
        "path": "/xpdr/' + cam_name + '/' + cap_name + '",\
        "data": {\
            "decklink_video_in": {\
                "width": ' + w + ',\
                "height": ' + h + ',\
                "num": ' + num + ',\
                "den": ' + den + ',\
                "video_conn": "hdmi",\
                "audio_conn": "audio_default"\
            }\
        }\
    }';
    kr.send_json(json);
  });

  $(this.sel).append('\
<form style="display:none" class="creation_form" id="form_create_path_wayland">\
<div class="very_big">Create Wayland Window</div>\
Window Name:<input type="text" id="wl_name" name="v4l2_cam_name" required><br>\
Server: <select name="server">\
<option value="default" selected>default</option>\
<option value="another">another</option>\
</select><br>\
\
Fullscreen? <input type="checkbox" id="wl_fullscreen" name="fullscreen" value="false" /><br>\
\
Window Dimensions:<br>\
<select id="wxh" name="wxh">\
<option value="640x360" selected>640x360</option>\
<option value="1280x720">1280x720</option>\
<option value="1920x540">1920x540</option>\
<option value="1920x1080">1920x1080</option>\
</select>\
<br>\
<input class="kr_push_button blue" type="submit" value="Create"\
 id="button_wayland_create"></input>\
</form>');

  $('#form_create_path_wayland').submit(function( event ) {
    event.preventDefault();
    var w, h;
    w = 1920;
    h = 1080;
    var val = $('#wxh').val();
    if (val == "1280x720") {
      w = 1280;
      h = 720;
    } else if (val == "640x360") {
      w = 640;
      h = 360;
    } else if (val == "1920x1080") {
      w = 1920;
      h = 1080;
    } else if (val == "1920x540") {
      w = 1920;
      h = 540;
    }
    var obj = {};
    obj.meth = "PUT";
    obj.path = "/xpdr/Wayland";
    obj.meta = {};
    var data = {};
    data.wayland = {};
    data.wayland.display_name = "";
    data.wayland.state = "connected";
    obj.data = data;
    kr.send_json(JSON.stringify(obj));
    var checked = $('#wl_name').val();
    checked = checked ? 1 : 0;
    var name = $('#wl_name').val();
    var obj = {};
    obj.meth = "PUT";
    obj.path = "/xpdr/Wayland/" + name;
    obj.meta = {};
    var data = {};
    data.wayland_out = {};
    data.wayland_out.width = w;
    data.wayland_out.height = h;
    data.wayland_out.fullscreen = checked;
    obj.data = data;
    kr.send_json(JSON.stringify(obj));
  });


  $(this.sel).append('\
<form style="display:none" class="creation_form" id="form_create_path_jack">\
<h1>Create Jack Path</h1>\
Name:<input type="text" id="jack_name" name="jack_name" required><br>\
Number Channels: <select id="num_channels" name="num_channels">\
<option value="1">1</option>\
<option value="2"selected>2</option>\
<option value="3">3</option>\
<option value="4">4</option>\
<option value="5">5</option>\
<option value="6">6</option>\
<option value="7">7</option>\
<option value="8">8</option>\
</select><br>\
Direction: <select id="direction" name="direction">\
<option value="1" selected>INPUT</option>\
<option value="2">OUTPUT</option>\
</select>\
<br>\
<input type="submit" value="Create"/>\
</form>');

  $('#form_create_path_jack').submit(function( event ) {
    event.preventDefault();
    var obj = {};
    obj.meth = "PUT";
    obj.path = "/xpdr/Jack";
    obj.meta = {};
    var data = {};
    data.jack = {};
    data.jack.client_name = "krad radio";
    data.jack.server_name = "default";
    data.jack.state = "offline";
    data.jack.inputs = 0;
    data.jack.outputs = 0;
    data.jack.sample_rate = 0;
    data.jack.period_size = 0;
    data.jack.xruns = 0;
    data.jack.frames = 0;
    obj.data = data;
    kr.send_json(JSON.stringify(obj));

    var name = $('#jack_name').val();
    var nchannels = parseInt($('#num_channels').val());
    var direction = $('#direction').val() == "1" ?
     "input" : "output";
    var jack_crate = {};
    jack_crate.meth = "PUT"
    jack_crate.path = "/xpdr/Jack/" + name;
    jack_crate.meta = {};
    jack_crate.data = {};
    var tmp;
    tmp = {};
    tmp.name = name;
    tmp.channels = nchannels;
    tmp.direction = direction;
    if (direction == "input") {
      jack_crate.data.jack_in = tmp;
    } else {
      jack_crate.data.jack_out = tmp;
    }
    kr.send_json(JSON.stringify(jack_crate));
  });
}

Maker.prototype.handleSelect = function(value) {
  $('.creation_form').hide();
  $('#' + value).show();
}

Maker.prototype.shared = function(key, shared_object) {
}

Maker.prototype.destroy = function() {
}

Maker.prototype.update = function(crate) {
}
$(document).ready(function() {
  rack_units.push({'constructor': MixCtrl,  'aspect': [32,14],
    'page': 'mixctrl'});
});

function MixCtrl(info_object) {
  this.title = "MixCtrl";
  this.description = "Adjust mixer path properties";
  this.id = 'mixctrl';
  this.div = "<div class='mixctrl RCU' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  this.x = 0;
  this.y = 1;
  this.resizeTimer;
  this.paths = [];
  info_object['parent_div'].append(this.div);
  $(this.sel).append('<div class="mix_strip_container">\
    <div class="mix_strip" id="mc_inputs">\
      <div class="label">Inputs</div>\
      <div class="content"></div>\
    </div>\
    <div class="mix_strip" id="mc_buses">\
      <div class="label">Buses</div>\
      <div class="content"></div>\
    </div>\
    <div class="mix_strip" id="mc_sources">\
      <div class="label">Sources</div>\
      <div class="content"></div>\
    </div>\
    <div class="mix_strip" id="mc_outputs">\
      <div class="label">Outputs</div>\
      <div class="content"></div>\
    </div>\
  </div>');
}

MixCtrl.prototype.on_patch = function(path, crate) {
  if (this.paths[path]) {
    var mc = this.paths[path];
    for (var i = 0, len = crate.data.length; i < len; i++) {
      var p = crate.data[i];
      if (p.op == "replace") {
        if (p.path == "/afx/volume/gain") {
          mc.gain_slider_object.set_value(p.value);
        } else if (p.path == "/afx/volume/fader") {
          mc.fader_slider_object.set_value(p.value);
        }
      }
    }
  }
}

MixCtrl.prototype.on_del = function(path) {
  if (this.paths[path]) {
    var mc = this.paths[path];
    mc.delete();
    delete mc;
  }
}

MixCtrl.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/mixer/")) return;
  if (crate.meth == "PATCH") {
    this.on_patch(crate.path, crate);
  } else if (crate.meth == "PUT") {
      this.on_new_path(crate.path, crate);
  } else if (crate.meth == "DELETE") {
    this.on_del(crate.path);
  }
}

MixCtrl.prototype.on_new_path = function(path, crate) {
  this.paths[path] = new MCPath(path, crate, this);
}

function MCPath(path, crate, unit) {
  this.path = path;
  this.unit = unit;
  this.crate = crate;
  var parts = path.split('/');
  parts.shift();
  parts.shift();
  this.name = parts.join('/');
  var cid;
  if (crate.data.type == "input") {
    cid = "mc_inputs";
  } else if (crate.data.type == "bus") {
    cid = "mc_buses";
  } else if (crate.data.type == "source") {
    cid = "mc_sources";
  } else if (crate.data.type == "output") {
    cid = "mc_outputs";
  }
  this.slider_containers
   = document.getElementById(cid).getElementsByClassName('content')[0];
  this.sliders_container = document.createElement('div');
  this.sliders_container.classList.add('labelled_mixer_strip');
  this.sliders = document.createElement('div');
  this.sliders.classList.add('mixer_strip');
  this.sliders_container.appendChild(this.sliders);
  this.slider_containers.appendChild(this.sliders_container);
  this.gain_slider_object = new KRSlider(this.sliders,
   this.path, this.name + " GAIN", '/afx/volume/gain',
   {
     sender: this.unit,
     range: [24.0, -24.0],
     knee: { range: 0, move_fraction: 0 },
     initial_value: crate.data.afx[0].volume.gain,
   });
  this.gain_slider_object.activate();
  this.fader_slider_object = new KRSlider(this.sliders,
   this.path, this.name + " FADER", '/afx/volume/fader',
   {
     sender: this.unit,
     range: [6.0, -120.0],
     knee: { range: 126, move_fraction: 1 },
     linear_db: true,
     initial_value: crate.data.afx[0].volume.fader,
   });
  this.fader_slider_object.activate();
}

MCPath.prototype.delete = function() {
  this.slider_containers.removeChild(this.sliders_container);
}
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

$(document).ready(function() {
  rack_units.push({'constructor': Namer, 'aspect': { dev: [16,13],
    audio: [18, 5] }, 'page': { dev: 'upload', audio: 'system'},
    'position': { dev: [0, 1], audio: [0, 1] }});
});

function Namer(info_object) {
  this.title = "Namer";
  this.description = "Monitor and change system name.";
  this.id = "namer";
  this.sel = "#namer";
  info_object['parent_div'].append("<div class='namer RCU' id='" + this.id +
      "'></div>");
  $('#' + this.id).append('\
      <div class="vbox offwhite darkblue robo_light">\
        <div class="hbox sysname">\
          <div class="header" id="nmr_sysname"></div>\
          <div class="kr_push_button blue" id="nmr_edit_button">\
            <em>Edit</em>\
          </div>\
        </div>\
        <div class="hbox sysname_edit_container" id="sysname_edit_container">\
          <input id="sysname_edit" class="offwhite sans"\
           type="text" value="" />\
          <div class="kr_push_button red disabled" id="nmr_set_button">\
            <em>Set</em>\
          </div>\
          <div class="kr_push_button red" id="nmr_abort_button">\
            <em>Abort</em>\
          </div>\
        </div>\
        <div class="uptime"><br/>Uptime\
          <div id="radio_uptime"></div>\
        </div>\
      </div>');
  this.input_container = document.getElementById("sysname_edit_container");
  this.input = document.getElementById("sysname_edit");
  this.input.addEventListener('input', create_handler(this, this.on_input));
  this.edit_button = document.getElementById("nmr_edit_button");
  this.edit_button.addEventListener('click',
   create_handler(this, this.on_edit));
  this.set_button = document.getElementById("nmr_set_button");
  this.set_button.addEventListener("click", create_handler(this, this.on_set));
  this.abort_button = document.getElementById("nmr_abort_button");
  this.abort_button.addEventListener('click',
   create_handler(this, this.on_abort));
  this.input_ready_for_set = false;
  this.editting = false;
  this.uptime_interval = window.setInterval(create_handler(this,
   this.update_uptime), 1000);
}

Namer.prototype.send_patch = function(name) {
  alert("only pretending to patch sysname: " + name);
  this.info.callsign = this.info.name = name;
  this.update_ui();
}

Namer.prototype.on_set = function(evt) {
  if (this.editting && this.input_ready_for_set) {
    this.edit_stop();
    this.send_patch(this.input.value);
  }
}

Namer.prototype.on_edit = function(evt) {
  if (this.editting) return;
  this.edit_start();
}

Namer.prototype.on_abort = function(evt) {
  this.edit_abort();
}

Namer.prototype.edit_abort = function() {
  this.edit_stop();
}

Namer.prototype.edit_start = function() {
  this.editting = true;
  this.input.value = this.info.callsign;
  this.update_input_status();
  this.edit_button.classList.add("disabled");
  this.input_container.classList.add("active");
}

Namer.prototype.edit_stop = function() {
  this.editting = false;
  this.abort_button.classList.remove("flashing");
  this.edit_button.classList.remove("disabled");
  this.input_container.classList.remove("active");
}

Namer.prototype.set_info = function(info) {
  this.info = info;
  this.started_on = this.info.started_on * 1000;
  if (!this.editting) {
    T("sysname_edit", this.info.name);
  } else {
    this.abort_button.classList.add("flashing");
  }
  this.update_ui();
}

Namer.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, '/info')) return;
  if (crate.meth === 'PUT') {
    this.set_info(crate.data);
  } else if (crate.meth === 'PATCH') {
    jsonpatch.apply(this.info, crate.data);
    this.update_ui();
  }
}

Namer.prototype.update_uptime = function() {
  this.now_is = new Date().getTime();
  this.running_hours = Math.floor((this.now_is - this.started_on) / 3600000);
  this.running_mins = Math.floor((this.now_is - this.started_on) % 3600000
   / 60000);
  this.running_secs = Math.floor((this.now_is - this.started_on) % (3600000
   * 60000 ) % 60000 / 1000);
  R("radio_uptime", this.running_hours + " hour"
   + (this.running_hours == 1 ? "" : "s") + ", " + this.running_mins
   + " minute" + (this.running_mins == 1 ? "" : "s") + ", "
   + this.running_secs + " second" + (this.running_secs == 1 ? "" : "s"));
}

Namer.prototype.update_ui = function() {
  R("nmr_sysname", this.info.name);
  document.title = this.info.name;
  this.update_input_status();
}

Namer.prototype.update_input_status = function() {
  if (this.input.value !== this.info.name && this.input.value !== "") {
    this.input_ready_for_set = true;
    this.set_button.classList.remove("disabled");
  } else {
    this.input_ready_for_set = false;
    this.set_button.classList.add("disabled");
  }
}

Namer.prototype.on_input = function (evt) {
  this.update_input_status();
}


$(document).ready(function() {
  rack_units.push({'constructor': Pager});
});

function Pager(info_object) {
  this.title = "Pager";
  this.description = "Multiple rack pages, sir";
  this.div_text = "<div id='pager' class='RCU pager'></div>";
  this.id = 'pager';
  this.sel = "#pager";
  info_object['parent_div'].append(this.div_text);
  this.resizeTimer;
  this.pager_elem = document.getElementById('pager');
  this.pager_tabs = {};
}

Pager.prototype.create_span = function(name) {
  var span_elem = document.createElement('span');
  span_elem.classList.add('page');
  span_elem.id = name;
  span_elem.innerHTML = name;
  span_elem.addEventListener('click', create_handler(this,
        this.on_pager_click));
  return span_elem;
}

Pager.prototype.page_change = function(pageid) {
  this.select_page(pageid);
}

Pager.prototype.on_modechange = function(mode) {
  while (this.pager_elem.firstChild) {
    this.pager_elem.removeChild(this.pager_elem.firstChild);
  }
  var page_list = kr.ux.get_page_names();
  for (var i = 0; i < page_list.length; i++) {
    var span_elem = this.create_span(page_list[i]);
    this.pager_elem.appendChild(span_elem);
  }
}

Pager.prototype.select_page = function(id) {
  var target = document.getElementById(id);
  if (target === null) {
    setTimeout(create_handler(this, this.select_page), 5, id);
    return;
  }
  var matches = this.pager_elem.querySelectorAll('.selected');
  target.classList.add("selected");
  for (var i = 0; i < matches.length; i++) {
    if (target !== matches[i]) {
      matches[i].classList.remove("selected");
    }
  }
}

Pager.prototype.on_pager_click = function(evt) {
  var target = evt.target;
  while (!target.classList.contains("page")) {
    if (target.parentElement) {
      target = target.parentElement;
    } else {
      target = null;
      break;
    }
  }
  if (target) {
    kr.ux.switch_pages(target.id);
  }
}

Pager.prototype.window_resize = function() {
  requestAnimFrame(create_handler(this, this.page_resize));
};

Pager.prototype.page_resize = function() {
  $('.page').css('font-size', 0.6*$('#pager').height()+'px');
}

Pager.prototype.update = function(crate) {
}

Pager.prototype.shared = function(key, shared_object) {
}


$(document).ready(function() {
  rack_units.push({ 'constructor': PathList, 'aspect': { audio: [14, 9],
  dev: [16, 13]}, 'page': { dev: 'create', audio: 'system' },
  position: { dev: [0, 1], audio: [18, 6] }});
});

function PathList(info_object) {
  this.title = "List";
  this.id = "path_list";
  this.description = "List of kr paths.";
  this.aspect_ratio = [16,9];
  this.list_items = {};
  this.div_text = "<div id='path_list' class='RCU"
   + " path_list'></div>";
  this.sel = "#path_list";
  this.paths = {};
  info_object['parent_div'].append(this.div_text);
  $(this.sel).append("<div class='darkblue overflow_auto half'>"
   + "<div id='pl_list' class='kr_list play'></div></div>"
   + "<div id='pl_details' class='darkblue overflow_auto half'></div>");
  this.selectable_list = new Selectable(document.getElementById('pl_list'));
}

PathList.prototype.update_path_details = function(id) {
  var result = $("#pl_details").empty();
  if (id != undefined) {
    result.append('<div class="title">' + id + '</div><pre>'
     + JSON.stringify(this.paths[id].info, null, 2)
     + '</pre>');
  }
}

PathList.prototype.update = function(crate) {
  if (crate.data != undefined && crate.data.path) {
    for (var i=0; i < crate.data.len; i++) {
      var name = crate.data.path[i].name;
      kr.get(name);
    }
  } else {
    var path = crate.path;
    if (crate.meth == "DELETE") {
      if (this.paths[path] != undefined) {
        if (this.selected_path == path) {
          this.update_path_details();
          this.selected_path = null;
        }
        var i = this.paths[path].item_elem;
        i.parentElement.removeChild(i);
        delete this.paths[path];
      }
    } else if (crate.meth == "PATCH") {
      if (this.paths[path] != undefined) {
        jsonpatch.apply(this.paths[path].info, crate.data);
        if (this.selected_path == path) {
          this.update_path_details(path);
        }
      }
    } else if (crate.meth == "PUT") {
      kr.debug(crate);
      var del_button = 1;
      this.paths[crate.path] = new PathItem(this, crate);
    }
  }
}

PathList.prototype.shared = function(key, shared_object) {
}

function PathItem(pathList, crate) {
  this.pl = pathList;
  this.path = crate.path;
  this.info = crate.data;
  this.item_elem = document.createElement('div');
  this.item_elem.id = crate.path;
  var label_elem = document.createElement('div');
  label_elem.className = "label";
  label_elem.innerHTML = crate.path;
  this.item_elem.appendChild(label_elem);
  var button_elem = document.createElement('div');
  button_elem.className = "kr_push_button red";
  button_elem.innerHTML = "<em>Remove</em>";
  button_elem.addEventListener('click', create_handler(this,
   this.on_delete_click));
  this.item_elem.appendChild(button_elem);
  this.pl.selectable_list.add(this, this.selectable_list_selected,
   this.item_elem);
}

PathItem.prototype.on_delete_click = function(evt) {
  evt.stopPropagation();
  evt.preventDefault();
  kr.rm(this.path);
}

PathItem.prototype.selectable_list_selected = function(evt) {
  this.pl.update_path_details(this.path);
  this.pl.selected_path = this.path;
}
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
$(document).ready(function() {
  rack_units.push({'constructor': Poker, 'aspect': [16,13],
    'page': 'session'});
});

String.prototype.splice = function( idx, rem, s ) {
  return (this.slice(0,idx) + s + this.slice(idx + Math.abs(rem)));
};

function Poker(info_object) {
  this.title = "Poker";
  this.description = "Debug api commands.";
  this.id = "poker";
  this.div = "<div class='poker RCU' id='" + this.id + "'></div>";
  info_object['parent_div'].append(this.div);
  this.sel = "#" + this.id;
  this.x = 0;
  this.y = 1;
  this.resizeTimer;
  $(this.sel).append('\
   <div class="vbox robo_light offwhite darkblue">\
     <div class="hbox">\
       <div class="pk_recv">\
         <div class="big robo_bold">IN</div>\
         <div class="kr_button_group">\
           <div><div class="kr_push_button red" id="recv_log_button"><em>Log</em></div></div>\
           <div><div class="kr_push_button blue" id="recv_clear_button"><em>Clear</em></div></div>\
         </div>\
       </div>\
       <div class="pk_recv_log" id="pk_recv_log"></div>\
     </div>\
     <div class="hbox">\
       <div class="pk_sent">\
         <div class="big robo_bold">OUT</div>\
         <div class="kr_button_group">\
           <div><div class="kr_push_button red" id="sent_log_button"><em>Log</em></div></div>\
           <div><div class="kr_push_button blue" id="sent_clear_button"><em>Clear</em></div></div>\
         </div>\
       </div>\
       <div class="pk_sent_log" id="pk_sent_log"></div>\
     </div>\
     <textarea class="pk_send_text" id="pk_send_text"></textarea>\
     <div class="kr_button_group"><div class="kr_push_button red" id="send_text_button">\
       <em>Send</em></div></div>\
   </div>\
   ');
  this.send_textarea = document.getElementById("pk_send_text");
  this.send_button = document.getElementById("send_text_button");
  this.send_button.addEventListener('click', create_handler(this,
   this.on_send_click));
  this.recv_logging = false;
  this.recv_log = document.getElementById("pk_recv_log");
  this.recv_log_button = document.getElementById("recv_log_button");
  this.recv_log_button.addEventListener('click', create_handler(this,
   this.on_recv_log_click));
  this.clear_recv_log_button = document.getElementById("recv_clear_button");
  this.clear_recv_log_button.addEventListener('click', create_handler(this,
   this.on_clear_recv_log_click));
  this.sent_logging = false;
  this.sent_log = document.getElementById("pk_sent_log");
  this.sent_log_button = document.getElementById("sent_log_button");
  this.sent_log_button.addEventListener('click', create_handler(this,
   this.on_sent_log_click));
  this.clear_sent_log_button = document.getElementById("sent_clear_button");
  this.clear_sent_log_button.addEventListener('click', create_handler(this,
   this.on_clear_sent_log_click));
  this.recv_recorder = new ApiRecvResponder("poker_recv_listener", create_handler(
  this, this.on_recv));
  this.sent_recorder = new ApiSendResponder("poker_send_listener", create_handler(
   this, this.on_sent));
}

Poker.prototype.on_send_click = function(evt) {
  var text = this.send_textarea.value;
  kr.send_json(text);
}

Poker.prototype.update =  function(crate) {
}

Poker.prototype.on_recv_log_click = function(evt) {
  if (!this.recv_logging) {
    this.start_recv_log();
  } else {
    this.stop_recv_log();
  }
}

Poker.prototype.stop_recv_log = function() {
  this.recv_logging = false;
  this.recv_log_button.classList.remove("pushed");
}

Poker.prototype.start_recv_log = function() {
  this.recv_logging = true;
  this.recv_log_button.classList.add("pushed");
}

Poker.prototype.on_clear_recv_log_click = function(evt) {
  this.clear_recv_log();
}

Poker.prototype.clear_recv_log = function() {
  this.recv_log.innerHTML = "";
}

Poker.prototype.on_sent_log_click = function(evt) {
  if (!this.sent_logging) {
    this.start_sent_log();
  } else {
    this.stop_sent_log();
  }
}

Poker.prototype.stop_sent_log = function() {
  this.sent_logging = false;
  this.sent_log_button.classList.remove("pushed");
}

Poker.prototype.wrap_lines = function(text) {
  var len = 79;
  var start = 0;
  var end = len;
  var new_text = "";
  while (end < text.length) {
    new_text += text.slice(start,end) + "\n";
    start = end;
    end += len;
  }
  new_text += text.slice(start, text.length);
  return new_text;
}

Poker.prototype.log_append = function(log, content) {
  var doscroll = 0;
  if (log.clientHeight == log.scrollHeight - log.scrollTop) doscroll = 1;
  var pre = document.createElement("pre");
  pre.textContent = content;
  log.appendChild(pre);
  if (doscroll == 1) log.scrollTop = log.scrollHeight;
}

Poker.prototype.on_sent = function(json) {
  if (this.sent_logging) {
    var out = this.wrap_lines(json);
    this.log_append(this.sent_log, out);
  }
}

Poker.prototype.on_recv = function(json) {
  if (this.recv_logging) {
    var out = this.wrap_lines(json);
    this.log_append(this.recv_log, out);
  }
}

Poker.prototype.start_sent_log = function() {
  this.sent_logging = true;
  this.sent_log_button.classList.add("pushed");
}

Poker.prototype.on_clear_sent_log_click = function(evt) {
  this.clear_sent_log();
}

Poker.prototype.clear_sent_log = function() {
  this.sent_log.innerHTML = "";
}

function ApiRecvResponder(name, func) {
  this.name = name;
  this.event_cb = func;
  kr.add_listener_recv(this);
}

ApiRecvResponder.prototype.finish = function() {
  kr.remove_listener_recv(this);
}

ApiRecvResponder.prototype.event = function(json) {
  this.event_cb(json);
}

function ApiSendResponder(name, func) {
  this.name = name;
  this.event_cb = func;
  kr.add_listener_send(this);
}

ApiSendResponder.prototype.finish = function() {
  kr.remove_listener_send(this);
}

ApiSendResponder.prototype.event = function(json) {
  this.event_cb(json);
}

var rack_units = [];

Kr.prototype.load_interface = function() {
	this.ux = new Rack();
}

function Rack() {
  this.units = [];
  this.shared = {};
  this.positioned_units = [];
  this.selected_page = null;
  this.modes = [];
  window.addEventListener('hashchange',
   create_handler(this, this.on_hashchange));
  window.addEventListener('resize',
   create_handler(this, this.on_window_resize));
}

Rack.prototype.create_modes = function() {
  this.create_dev_mode();
  this.create_audio_mode();
  this.create_mix_mode();
  this.create_eq_mode();
}

Rack.prototype.create_dev_mode = function() {
  var mode = new KRRackMode("dev");
  var session = new KRRackPage("SESSION");
  var upload = new KRRackPage("UPLOAD");
  var create = new KRRackPage("CREATE");
  var stream = new KRRackPage("STREAM");
  var stream2 = new KRRackPage("STREAM2");
  var agraph = new KRRackPage("AGRAPH");
  var vgraph = new KRRackPage("VGRAPH");
  var decks = new KRRackPage("DECKS");
  var mix = new KRRackPage("MIX");
  var scene = new KRRackPage("SCENE");
  var eq = new KRRackPage("EQ");
  /*var spectrum = new KRRackPage("SPECT");*/
  var pager = this.get_unit_by_id("pager");
  var pager_space = new KRRackSpace(pager, 0, 0, 32, 1);
  session.add_space(pager_space);
  upload.add_space(pager_space);
  create.add_space(pager_space);
  stream.add_space(pager_space);
  stream2.add_space(pager_space);
  agraph.add_space(pager_space);
  vgraph.add_space(pager_space);
  mix.add_space(pager_space);
  scene.add_space(pager_space);
  eq.add_space(pager_space);
  /*spectrum.add_space(pager_space);*/
  var unit = this.get_unit_by_id("poker");
  var space = new KRRackSpace(unit, 0, 1, 16, 14);
  session.add_space(space);
  unit = this.get_unit_by_id("clumper");
  space = new KRRackSpace(unit, 16, 1, 16, 14);
  session.add_space(space);
  unit = this.get_unit_by_id("uploader");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  upload.add_space(space);
  unit = this.get_unit_by_id("path_list");
  space = new KRRackSpace(unit, 0, 1, 16, 14);
  create.add_space(space);
  unit = this.get_unit_by_id("maker");
  space = new KRRackSpace(unit, 16, 1, 16, 14);
  create.add_space(space);
  unit = this.get_unit_by_id("stream_input");
  space = new KRRackSpace(unit, 0, 1, 16, 14);
  stream.add_space(space);
  unit = this.get_unit_by_id("stream_output");
  space = new KRRackSpace(unit, 16, 1, 16, 14);
  stream.add_space(space);
  unit = this.get_unit_by_id("stream_output2");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  stream2.add_space(space);
  unit = this.get_unit_by_id("compositor");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  vgraph.add_space(space);
  unit = this.get_unit_by_id("mixer");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  agraph.add_space(space);
  unit = this.get_unit_by_id("kr_mixer");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  mix.add_space(space);
  unit = this.get_unit_by_id("scener");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  scene.add_space(space);
  unit = this.get_unit_by_id("kr_eq");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  eq.add_space(space);
  unit = this.get_unit_by_id("xmms");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  decks.add_space(space);
  decks.add_space(pager_space);
  /*unit = this.get_unit_by_id("spectrum_unit");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  spectrum.add_space(space);*/
  mode.add_page(session);
  mode.add_page(upload);
  mode.add_page(create);
  mode.add_page(stream);
  mode.add_page(stream2);
  mode.add_page(agraph);
  mode.add_page(vgraph);
  mode.add_page(decks);
  mode.add_page(mix);
  mode.add_page(scene);
  mode.add_page(eq);
  mode.set_default_page("SESSION");
  this.modes[mode.id] = mode;
}

Rack.prototype.create_mix_mode = function() {
  var mode = new KRRackMode("mix");
  var mix = new KRRackPage("MIX");
  var unit = this.get_unit_by_id("kr_mixer");
  var space = new KRRackSpace(unit, 0, 0, 32, 15);
  mix.add_space(space);
  mode.add_page(mix);
  mode.set_default_page("MIX");
  this.modes[mode.id] = mode;
}

Rack.prototype.create_eq_mode = function() {
  var mode = new KRRackMode("eq");
  var eq = new KRRackPage("EQ");
  var unit = this.get_unit_by_id("kr_eq");
  var space = new KRRackSpace(unit, 0, 0, 32, 15);
  eq.add_space(space);
  mode.add_page(eq);
  mode.set_default_page("EQ");
  this.modes[mode.id] = mode;
}

Rack.prototype.create_audio_mode = function() {
  var mode = new KRRackMode("audio");
  var station = new KRRackPage("STATION");
  var stream = new KRRackPage("STREAM");
  var route = new KRRackPage("ROUTE");
  var decks = new KRRackPage("DECKS");
  var mix = new KRRackPage("MIX");
  var eq = new KRRackPage("EQ");
  var pager = this.get_unit_by_id("pager");
  var pager_space = new KRRackSpace(pager, 0, 0, 32, 1);
  station.add_space(pager_space);
  var unit = this.get_unit_by_id("namer");
  var space = new KRRackSpace(unit, 18, 1, 14, 7);
  station.add_space(space);
  unit = this.get_unit_by_id("uploader");
  space = new KRRackSpace(unit, 18, 8, 14, 7);
  station.add_space(space);
  stream.add_space(pager_space);
  unit = this.get_unit_by_id("stream_input");
  space = new KRRackSpace(unit, 0, 1, 16, 14);
  stream.add_space(space);
  unit = this.get_unit_by_id("stream_output");
  space = new KRRackSpace(unit, 16, 1, 16, 14);
  stream.add_space(space);
  unit = this.get_unit_by_id("clumper");
  space = new KRRackSpace(unit, 0, 1, 18, 14);
  station.add_space(space);
  route.add_space(pager_space);
  unit = this.get_unit_by_id("xmms");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  decks.add_space(space);
  decks.add_space(pager_space);
  unit = this.get_unit_by_id("mixer");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  route.add_space(space);
  unit = this.get_unit_by_id("kr_mixer");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  mix.add_space(space);
  mix.add_space(pager_space);
  eq.add_space(pager_space);
  unit = this.get_unit_by_id("kr_eq");
  space = new KRRackSpace(unit, 0, 1, 32, 14);
  eq.add_space(space);
  mode.add_page(station);
  mode.add_page(stream);
  mode.add_page(route);
  mode.add_page(decks);
  mode.add_page(mix);
  mode.add_page(eq);
  mode.set_default_page("STATION");
  this.modes[mode.id] = mode;
}

Rack.prototype.determine_mode = function() {
  var fid = window.location.hash;
  fid = fid.substr(1);
  if (this.modes[fid] !== undefined) {
    return fid;
  } else {
    return "audio";
  }
}

Rack.prototype.on_window_resize = function() {
  this.calc_rack();
  this.selected_mode.resize();
}

Rack.prototype.share = function(key, value) {
  this.shared[key] = value;
  for (var i = 0; i < this.units.length; i++) {
    this.units[i].shared(key, this.shared);
  }
}

Rack.prototype.page_change = function(name) {
  for (var i = 0; i < this.units.length; i++) {
    if (this.units[i].page_change) {
      this.units[i].page_change(name);
    }
  }
}

Rack.prototype.hide_displayed_page = function() {
  var mode = this.selected_mode;
  mode.hide_page(mode.selected_page.id);
  this.selected_page = null;
}

Rack.prototype.display_page = function(page) {
  this.calc_rack();
  var mode = this.selected_mode;
  mode.display_page(page);
  this.selected_page = mode.selected_page;
  this.page_change(page);
  mode.resize();
}

Rack.prototype.has_page = function(id) {
  return this.selected_mode.has_page(id);
}

Rack.prototype.switch_pages = function(to_pageid) {
  if (this.selected_page && this.selected_page.id !== to_pageid) {
    this.hide_displayed_page();
    this.display_page(to_pageid);
  }
}

Rack.prototype.select_mode = function(mode) {
  if (this.selected_mode !== undefined
   && this.selected_mode.id === mode) return;
  if (this.modes[mode] === undefined) return;
  if (this.selected_mode) this.hide_displayed_page();
  this.selected_mode = this.modes[mode];
  for (var i = 0; i < this.units.length; i++) {
    var u = this.units[i];
    if (u.on_modechange) u.on_modechange(mode);
  }
  this.display_page(this.selected_mode.selected_page.id);
}

Rack.prototype.get_unit_by_id = function(id) {
  var ret = null;
  for (var i = 0, len = this.units.length; i < len; i++) {
    if (this.units[i].id === id) {
      ret = this.units[i];
      break;
    }
  }
  return ret;
}

Rack.prototype.calc_rack = function() {
  this.rack_density = 32;
  var view_width = window.innerWidth;
  var view_height = window.innerHeight;
  var rack_space_sz = view_width / this.rack_density;
  this.rack_width = Math.floor(view_width / rack_space_sz);
  this.rack_width_per = (rack_space_sz / view_width) * 100;
  this.rack_height = Math.floor(view_height / rack_space_sz);
  this.rack_height_per = (rack_space_sz / view_height) * 100;
}

Rack.prototype.construct_rack = function(info_object) {
  for (var i = 0; i < rack_units.length; i++) {
    var unit = new rack_units[i]['constructor'](info_object, rack_units[i]['id']);
    this.units.push(unit);
  }
}

Rack.prototype.got_sysname = function(sysname) {
  this.sysname = sysname;
  $('#kradradio').append("<div class='kradradio_station' id='"
   + this.sysname + "'></div>");
  this.construct_rack({'parent_div': $('#' + this.sysname)});
  this.create_modes();
  this.select_mode(this.determine_mode());
}

Rack.prototype.destroy = function() {
  $('#' + this.sysname).remove();
}

Rack.prototype.get_page_names = function() {
  var ret = [];
  for (var i = 0, len = this.selected_mode.pages.length; i < len; i++) {
    var page = this.selected_mode.pages[i];
    ret[ret.length] = page.id;
  }
  return ret;
}

Rack.prototype.on_crate = function(crate, sender) {
  for (var i=0; i < this.units.length; i++) {
    var u = this.units[i];
    if (u != sender && u.update) u.update(crate);
    else if (u == sender && u.self_update) u.self_update(crate);
  }
}

Rack.prototype.on_hashchange = function(evt) {
  this.select_mode(this.determine_mode());
}

function KRRackSpace(unit, x, y, w, h) {
  this.unit = unit;
  this.x = x;
  this.y = y;
  this.w = w;
  this.h = h;
}

function KRRackUnit(id) {
  this.views = [];
}

KRRackUnit.prototype.add_view = function(view) {
  this.views.push(view);
}

KRRackUnit.prototype.window_resize = function() {
  if (this.views) {
    for (var i = 0; i < this.views.length; i++) {
      var v = this.views[i];
      if (v.resize) v.resize();
    }
  }
}

KRRackSpace.prototype.visible = function() {
  if (this.unit.visible) this.unit.visible();
  if (this.unit.views) {
    for (var i = 0; i < this.unit.views.length; i++) {
      var v = this.unit.views[i];
      if (v.visible) v.visible();
    }
  }
}

KRRackSpace.prototype.hidden = function() {
  if (this.unit.hidden) this.unit.hidden();
  if (this.unit.views) {
    for (var i = 0; i < this.unit.views.length; i++) {
      var v = this.unit.views[i];
      if (v.hidden) v.hidden();
    }
  }
}

function KRRackPage(id) {
  this.id = id;
  this.spaces = [];
}

KRRackPage.prototype.add_space = function(space) {
  this.spaces[this.spaces.length] = space;
}

function KRRackMode(id) {
  this.id = id;
  this.pages = [];
}

KRRackMode.prototype.resize = function() {
  var p = this.selected_page;
  this.position_page(p.id);
  for (var i = 0, len = p.spaces.length; i < len; i++) {
    var s = p.spaces[i];
    if (s.unit.window_resize) {
      s.unit.window_resize();
    }
  }
}

KRRackMode.prototype.position_page = function(id) {
  for (var i = 0; i < this.pages.length; i++) {
    var p = this.pages[i];
    if (p.id === id) {
      for (var j = 0; j < p.spaces.length; j++) {
        var s = p.spaces[j];
        var top = kr.ux.rack_height_per * s.y;
        var left = kr.ux.rack_width_per * s.x;
        var height = kr.ux.rack_height_per * s.h;
        var width = kr.ux.rack_width_per * s.w;
        $(s.unit.sel).css({'top': top + '%' });
        $(s.unit.sel).css({ 'left': left + '%' });
        $(s.unit.sel).css({ 'width': width + '%' });
        $(s.unit.sel).css({ 'height': height + '%' });
      }
    }
  }
}

KRRackMode.prototype.display_page = function(id) {
  this.position_page(id);
  for (var i = 0; i < this.pages.length; i++) {
    var p = this.pages[i];
    if (p.id === id) {
      for (var j = 0; j < p.spaces.length; j++) {
        this.selected_page = p;
        var s = p.spaces[j];
        $(s.unit.sel).show();
        if (s.visible) {
          s.visible();
        }
      }
    }
  }
}

KRRackMode.prototype.has_page = function(id) {
  var ret = false;
  for (var i = 0; i < this.pages.length; i++) {
    var p = this.pages[i];
    if (p.id === id) {
      ret = true;
      break;
    }
  }
  return ret;
}


KRRackMode.prototype.hide_page = function(id) {
  for (var i = 0; i < this.pages.length; i++) {
    var p = this.pages[i];
    if (p.id === id) {
      for (var j = 0; j < p.spaces.length; j++) {
        var s = p.spaces[j];
        $(s.unit.sel).hide();
        if (s.hidden) {
          s.hidden();
        }
      }
    }
  }
}

KRRackMode.prototype.set_default_page = function(id) {
  for (var i = 0, len = this.pages.length; i < len; i++) {
    if (id === this.pages[i].id) {
      this.selected_page = this.pages[i];
    }
  }
}

KRRackMode.prototype.add_page = function(page) {
  this.pages[this.pages.length] = page;
}
$(document).ready(function() {
  rack_units.push({'constructor': Scener, 'aspect': [32,14],
   'page': 'scene', 'position': [0, 1] });
});

var kr_comp_w = 7680;
var kr_comp_h = 4320;

var kr_scener_w = 1280;
var kr_scener_h = 720;

function Scener(info_object) {
  this.title = "Scener";
  this.page = info_object.page;
  this.description = "Adjust properties of inputs to an output.";
  this.id = 'scener';
  this.div = "<div class='RCU' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  this.address_masks = [];
  this.scenes = {};
  this.inputs = {};
  this.selected_scene = null;
  this.unmatched_inputs = {};
  this.color_scheme = new KRMonoColorScheme(MIXER_BG_COLOR);
  info_object['parent_div'].append(this.div);
  var me = this;
  kr.scener = this;
  $(this.sel).append('\
   <div class="scener streamypine">\
     <div class="scnr_lists offwhite robo_light">\
       <div class="big">Scenes</div>\
       <div class="kr_list kindabig select_list" id="scnr_out_list"></div>\
       <div class="big">Inputs</div>\
       <div class="kindabig" id="scnr_in_lists"></div>\
     </div>\
     <div class="sliders_container" id="scnr_sliders"></div>\
     <div class="scnr_scenes">\
       <div class="krad_scene" id="scnr_krad_scene">\
         <div class="krad_output" id="scnr_krad_output">\
           <div class="offwhite scene_no_input_label" id="scene_no_input_label">\
            NO INPUT</div>\
         </div>\
       </div>\
       <div class="krad_scene" id="scnr_cropper_scene">\
         <div class="krad_output" id="scnr_cropper_output"></div>\
       </div>\
     </div>\
   </div>');
  this.no_input_elem = document.getElementById('scene_no_input_label');
  this.scene_list = new Selectable(document.getElementById('scnr_out_list'));
  this.input_lists = document.getElementById('scnr_in_lists');
  this.resizeTimer = null;
}

Scener.prototype.window_resize = function() {
  clearTimeout(this.resizeTimer);
  this.resizeTimer = setTimeout(create_handler(this, this.page_resize), 100);
};

Scener.prototype.page_resize = function() {
  this.no_input_elem.style.fontSize =  0.2*$('#scnr_krad_output').height()+'px';
}

Scener.prototype.hidden = function() {
  if (this.selected_scene != null) {
    this.selected_scene.deselect();
  }
}

Scener.prototype.visible = function() {
  if (this.selected_scene != null) {
    this.selected_scene.select();
  }
}

Scener.prototype.select_scene = function(name) {
  var key = '/compositor/' + name;
  if (typeof this.scenes[key] != 'undefined') {
    this.scenes[key].select();
  }
}

Scener.prototype.on_patch = function(path, crate) {
  if (this.inputs[path]) {
    var input = this.inputs[path];
    for (var i = 0, len = crate.data.length; i < len; i++) {
      var info = crate.data[i];
      var p = info.path;
      var op = info.op;
      var val = info.value;
      if (op == 'replace') {
        var names = p.split('/');
        if (names[2] == 'pos') {
          var prop = names[3];
          var new_rect = input.rectScaleToComp(input.getRect());
          new_rect[prop] = val;
          input.setRect(input.rectScaleFromComp(new_rect));
        } else if (names[2] == 'crop') {
          var prop = names[3];
          var rect = input.crop.rectScaleToComp(input.crop.getRect());
          rect[prop] = val;
          input.crop.setRect(input.crop.rectScaleFromComp(rect));
        } else if (names[2] == 'rotation') {
          input.set_rotation(val);
        } else if (names[2] == 'opacity') {
          input.set_opacity(val);
        }
      }
    }
  }
}

Scener.prototype.on_delete = function(crate) {
  var s = this.scenes[crate.path];
  if (s != undefined) {
    this.scene_list.remove(s.list_elem);
    if (this.selected_scene == s) {
      s.deselect();
      this.selected_scene == null;
    }
    delete s;
  }
  var input = this.inputs[crate.path];
  if (input != undefined) {
    var parts = crate.path.split('/');
    parts.pop();
    var scene_path = parts.join('/');
    if (this.scenes[scene_path]) {
      this.scenes[scene_path].remove_input(input);
    }
    input.delete();
    delete this.inputs[crate.path];
  }
}

Scener.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/compositor/")) return;
  if (crate.meth == "DELETE") {
    this.on_delete(crate);
  } else if (crate.meth == "PATCH") {
    var path = crate.path;
    this.on_patch(path, crate);
  } else if (crate.meth == "PUT") {
    var path = crate.path;
    if (crate.data.output || crate.data.bus) {
      var scene = new Scene(this, path);
      var parts = path.split('/');
      var name = parts.pop();
      this.scene_list.add(scene, scene.scene_list_selected, scene.list_elem);
      for (var input_path in this.unmatched_inputs) {
        var parts = input_path.split('/');
        parts.pop();
        var scene_path = parts.join('/');
        if (path == scene_path) {
          scene.add_input(this.unmatched_inputs[input_path]);
          delete this.unmatched_inputs[input_path];
       }
     }
     this.scenes[path] = scene;
   }
   if (crate.data.input) {
     var parts = path.split('/');
     parts.pop();
     var scene_path = parts.join('/');
     var input = new Input(this, path, crate.data.input.pos,
      crate.data.input.crop, crate.data.input.rotation,
      crate.data.input.opacity);
     this.inputs[path] = input;
     if (this.scenes[scene_path]) {
       this.scenes[scene_path].add_input(input);
     } else {
       this.unmatched_inputs[path] = input;
     }
   }
  }
}

function Scene(scener, path) {
  this.scener = scener;
  this.path = path;
  this.inputs = {};
  this.switchable = {};
  this.name = path.split('/').pop();
  this.selected_input = null;
  this.is_selected = false;
  this.no_input_elem = document.getElementById('scene_no_input_label');
  this.list_elem = document.createElement('div');
  this.list_elem_label = document.createElement('div');
  this.list_elem_label.classList.add("label");
  this.list_elem_label.innerHTML = this.name;
  this.list_elem.appendChild(this.list_elem_label);
  this.input_list_elem = document.createElement('div');
  this.input_list_elem.classList.add('kr_list');
  this.input_list_elem.classList.add('select_list');
  this.output_label_elem = document.createElement('div');
  this.output_label_elem.className = 'scene_output_label';
  this.input_list = new Selectable(this.input_list_elem);
  this.scener.input_lists.appendChild(this.input_list_elem);
}

Scene.prototype.add_input = function(input) {
  this.inputs[input.path] = input;
  if (this.is_selected) {
    this.inputs[input.path].activate();
  }
  this.input_list.add(input, input.on_selection_list_click, input.list_item);
  this.no_input_elem.classList.remove('active');
}

Scene.prototype.remove_input = function(input) {
  if (this.is_selected) {
    if (this.selected_input == input) {
      input.deselect();
      this.selected_input = null;
    }
    input.deactivate();
  }
  this.input_list.remove(input.list_item);
  if (Object.keys(this.inputs).length === 0) {
    this.no_input_elem.classList.add('active');
  }
  delete this.inputs[input.path];
  if (this.is_selected) {
    this.select();
  }
}

Scene.prototype.select = function() {
  this.is_selected = true;
  this.list_elem.click();
  if (Object.keys(this.inputs).length > 0) {
    for (var input in this.inputs) {
      this.inputs[input].activate();
    }
  } else {
    this.no_input_elem.classList.add('active');
  }
  this.input_list_elem.classList.add('active');
  if (this.selected_input != null) {
    this.selected_input.list_item.click();
  }
}

Scene.prototype.deselect = function() {
  this.is_selected = false;
  for (var input in this.inputs) {
    this.inputs[input].deactivate();
  }
  this.input_list_elem.classList.remove('active');
  this.no_input_elem.classList.remove('active');
}

Scene.prototype.scene_list_selected = function() {
  if (this.scener.selected_scene != null) {
    if (this == this.scener.selected_scene) return;
    this.scener.selected_scene.deselect();
  }
  this.scener.selected_scene = this;
  this.select();
}


function Input(scener, path, rect, crop, rotation, opacity) {
  this.is_selected = false;
  this.scener = scener;
  this.path = path;
  this.handle_size = 20;
  this.min_size = 5 * this.handle_size
  this.scene = $('#scnr_krad_scene');
  this.output_elem = document.getElementById('scnr_krad_output');
  this.slider_container = $('#scnr_sliders');
  this.slider_container_elem = document.getElementById('scnr_sliders');
  this.maxw = kr_scener_w;
  this.maxh = kr_scener_h;
  this.id = 'krad_input' + path.split('/').join('_');
  this.input_elem = document.createElement('div');
  this.input_elem.className = "krad_input";
  this.input_elem.id = this.id;
  this.output_elem.appendChild(this.input_elem);
  var hid = this.id + '_handle';
  this.handle_elem = document.createElement('div');
  this.handle_elem.className = "krad_input_handle";
  this.handle_elem.id = hid;
  this.input_elem.appendChild(this.handle_elem);
  this.mdown = false;
  this.mdown_region = "";
  this.mdown_rect = {};
  rect = this.rectScaleFromComp(rect);
  this.setRect(rect);
  var parts = this.path.split('/');
  var name = parts.pop();
  this._on_mouseup = create_handler(this, this.on_mouseup);
  this._on_mousedown = create_handler(this, this.on_mousedown);
  this._on_mousemove = create_handler(this, this.on_mousemove);
  window.addEventListener('mouseup', this._on_mouseup);
  window.addEventListener('mousedown', this._on_mousedown);
  window.addEventListener('mousemove', this._on_mousemove);
  this.list_item = document.createElement('div');
  this.list_item.classList.add('scnr_li');
  this.list_item_label = document.createElement('div');
  this.list_item_label.innerHTML = name;
  this.list_item_label.classList.add('label');
  this.list_item_controls = document.createElement('div');
  this.list_item_controls.classList.add('controls');
  this.input_switchable_button = document.createElement('div');
  this.input_switchable_button.classList.add('input_switchable_button');
  this.input_switch_to_button = document.createElement('div');
  this.input_switch_to_button.classList.add('input_switch_to_button');
  this.input_switchable_button.addEventListener('click', create_handler(this,
   this.on_switchable_click));
  this.input_switch_to_button.addEventListener('click', create_handler(this,
   this.on_switch_to_click));
  this.crop_button = document.createElement('div');
  this.crop_button.innerHTML = "<em>CROP</em>";
  this.crop_button.className = ' crop_button kr_push_button blue'
  this.crop_button.addEventListener('click', create_handler(this,
   this.on_crop_click));
  this.list_item_controls.appendChild(this.crop_button);
  this.list_item_controls.appendChild(this.input_switch_to_button);
  this.list_item_controls.appendChild(this.input_switchable_button);
  this.list_item.appendChild(this.list_item_label);
  this.list_item.appendChild(this.list_item_controls);
  this.rotation_slider_elem = document.createElement('div');
  this.rotation_slider_elem.className = "slider_container";
  this.slider_container_elem.appendChild(this.rotation_slider_elem);
  this.rotation_slider_container =
   new CanvasViewContainer(this.scener, this.rotation_slider_elem,
   this.path + '_rotation');
  var rotation_model = new KRCompositorSliderModel(0,
   this.path, "ROTATION", '/input/rotation',
   {
     sender: this.scener,
     range: [360.0, 0.0],
     knee: { range: 0, move_fraction: 0.0 },
     initial_value: rotation,
   });
  this.rotation_slider_view
   = new KRSliderVerticalGradientsView(this.rotation_slider_container,
   rotation_model, this.rotation_slider_container.context,
   new KRBoundBox(0, 0, 1, 1),
   CanvasViewContainer.colors_from_color(this.scener.color_scheme.next()),
   {show_model_label: true});
  this.rotation_slider_container.add_view(this.path, this.rotation_slider_view);
  this.opacity_slider_elem = document.createElement('div');
  this.opacity_slider_elem.className = "slider_container";
  this.slider_container_elem.appendChild(this.opacity_slider_elem);
  this.opacity_slider_container =
   new CanvasViewContainer(this.scener, this.opacity_slider_elem,
   this.path + '_opacity');
  var opacity_model = new KRCompositorSliderModel(0,
   this.path, "OPACITY", '/input/opacity',
   {
     sender: this.scener,
     range: [1.0, 0.0],
     knee: { range: 0, move_fraction: 0.0 },
     initial_value: opacity,
   });
  this.opacity_slider_view
   = new KRSliderVerticalGradientsView(this.opacity_slider_container,
   opacity_model, this.opacity_slider_container.context,
   new KRBoundBox(0, 0, 1, 1),
   CanvasViewContainer.colors_from_color(this.scener.color_scheme.next()),
   {show_model_slider: true});
  this.opacity_slider_container.add_view(this.path, this.opacity_slider_view);
  /* this.rotation_slider_object = new KRSlider(this.slider_container_elem,
   this.path, "ROTATION", '/input/rotation',
   {
     range: [360.0, 0],
     initial_value: rotation,
   });*/
  /*this.opacity_slider_object = new KRSlider(this.slider_container_elem,
   this.path, "OPACITY", '/input/opacity', {
     range: [1.0, 0],
     initial_value: opacity,
   });*/
  this.crop = new Crop(this, this.scener, path, crop);
}

function KRCompositorSliderModel(pos, paths, label, prop, opts) {
  this.inherits_from = KRSliderModel;
  this.inherits_from(pos, paths, label, prop, opts);
}

KRCompositorSliderModel.prototype = Object.create(KRSliderModel.prototype);
KRCompositorSliderModel.prototype.constructor = KRCompositorSliderModel;

KRCompositorSliderModel.prototype.set_from_pos = function(new_pos) {
  var new_val = this.pos_to_value(new_pos);
  if (new_val != this.last_patched && !isNaN(new_val)) {
    this.set_pos(new_val);
    kr.ctrl_mix([[this.prop, '', new_val]], this.path, this.sender);
    this.last_patched = new_val;
  }
}

Input.prototype.delete = function() {
  this.rotation_slider_elem.parent.removeChild(this.rotation_slider_elem);
  this.rotation_slider_container.delete();
  this.opacity_slider_elem.parent.removeChild(this.opacity_slider_elem);
  this.opacity_slider_container.delete();
  this.crop.delete();
  this.input_elem.parentNode.removeChild(this.input_elem);
  window.removeEventListener('mouseup', this._on_mouseup);
  window.removeEventListener('mousedown', this._on_mousedown);
  window.removeEventListener('mousemove', this._on_mousemove);
}

Input.prototype.set_rotation = function(val) {
  this.rotation_slider_view.model.set_pos(val);
}

Input.prototype.set_opacity = function(val) {
  this.opacity_slider_view.model.set_pos(val);
}

Input.prototype.on_crop_click = function(evt) {
  if (this.crop.selected) {
    this.hide_crop();
  } else {
    this.show_crop();
  }
  evt.stopPropagation();
}

Input.prototype.show_crop = function() {
 this.crop.select();
}

Input.prototype.hide_crop = function() {
  this.crop.deselect();
}

Input.prototype.on_selection_list_click = function(evt) {
  if (this.scener.selected_scene.selected_input != null) {
    this.scener.selected_scene.selected_input.deselect();
  }
  this.select();
  this.scener.selected_scene.selected_input = this;
}

Input.prototype.on_switchable_click = function(evt) {
  var btn = this.input_switchable_button;
  if (btn.classList.contains('on')) {
    this.make_unswitchable();
  } else {
    this.make_switchable();
  }
  evt.stopPropagation();
}

Input.prototype.make_unswitchable = function() {
  delete this.scener.selected_scene.switchable[this.path];
  if (this.input_switch_to_button.classList.contains('on')) {
    this.switch_to_off();
    for (var key in this.scener.selected_scene.switchable) {
      this.scener.selected_scene.switchable[key].switch_to_on();
      break;
    }
  }
  this.deactivate_switch_to();
  this.input_switchable_button.classList.remove('on');
}

Input.prototype.make_switchable = function() {
  this.scener.selected_scene.switchable[this.path] = this;
  this.input_switchable_button.classList.add('on');
  this.activate_switch_to();
}

Input.prototype.activate_switch_to = function() {
  this.input_switch_to_button.classList.add('active');
}

Input.prototype.deactivate_switch_to = function() {
  this.input_switch_to_button.classList.remove('active');
}

Input.prototype.switch_to_on = function() {
  this.scener.selected_scene.switched_to_input = this;
  this.input_switch_to_button.classList.add('on');
  kr.ctrl_mix([['/input/opacity', '', 1.0]], this.path);
}

Input.prototype.switch_to_off = function() {
  this.scener.selected_scene.switched_to_input = null;
  this.input_switch_to_button.classList.remove('on');
  kr.ctrl_mix([['/input/opacity', '', 0.0]], this.path);
}

Input.prototype.on_switch_to_click = function(evt) {
  var selected_scene = this.scener.selected_scene;
  if (selected_scene.switched_to_input != null) {
    if (selected_scene.switched_to_input != this) {
      selected_scene.switched_to_input.switch_to_off()
      this.switch_to_on();
    }
  } else {
    this.switch_to_on();
  }
  evt.stopPropagation();
}

Input.prototype.activate = function() {
  this.list_item.classList.add('active');
  this.input_elem.classList.add('opaque');
}

Input.prototype.deactivate = function() {
  this.input_elem.classList.remove('opaque');
  this.list_item.classList.remove('active');
  this.deselect();
}

Input.prototype.on_mouseup = function(evt) {
  if (this.is_selected) {
    this.mdown = false;
  }
}

Input.prototype.on_mousedown = function(evt) {
  if (this.is_selected) {
    var mouse = this.getMouse(evt);
    var mdown_location = this.handlebars(mouse.x, mouse.y, this.input_rect.w,
     this.input_rect.h, this.handle_size);
    this.mdown = true;
    this.mdown_x = evt.clientX;
    this.mdown_y = evt.clientY;
    this.mdown_rect.x = this.input_rect.x
    this.mdown_rect.y = this.input_rect.y;
    this.mdown_rect.w = this.input_rect.w;
    this.mdown_rect.h = this.input_rect.h;
    this.mdown_location = mdown_location;
  }
}

Input.prototype.on_mousemove = function(evt) {
  if (this.active && this.is_selected && !this.crop.selected) {
    var patches = [];
    var new_rect = this.getRect();
    this.addHandle(evt);
    if (this.mdown === true) {
      evt.preventDefault();
      evt.stopPropagation();
      if ((this.mdown_location == "n") || (this.mdown_location == "ne")
          || (this.mdown_location == "nw")) {
        var dely = evt.clientY - this.mdown_y;
        var new_h = Math.min(this.maxh, Math.max(this.min_size, this.mdown_rect.h
              - dely));
        var delh = new_h - this.mdown_rect.h;
        var new_top = Math.min(this.maxh, Math.max(-this.maxh, this.mdown_rect.y
              - delh));
        var del_top = new_top - this.mdown_rect.y;
        if (del_top != -delh) {
          delh = -del_top;
          new_h = this.mdown_rect.h + delh;
        }
        new_rect.h = new_h;
        new_rect.y = new_top;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/pos/y', 'INT', val_rect.y],
         ['/input/pos/h', 'INT', val_rect.h]);
      }
      if ((this.mdown_location == "s") || (this.mdown_location == "se")
          || (this.mdown_location == "sw"))  {
        var dely = evt.clientY - this.mdown_y;
        var new_h = Math.min(this.maxh, Math.max(this.min_size, this.mdown_rect.h + dely));
        new_rect.h = new_h;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/pos/h', 'INT', val_rect.h]);
      }
      if (this.mdown_location == "b") {
        var delx = evt.clientX - this.mdown_x;
        var dely = evt.clientY - this.mdown_y;
        var new_left = Math.min(this.maxw, Math.max(-this.maxw, this.mdown_rect.x
              + delx));
        var new_top = Math.min(this.maxh, Math.max(-this.maxh, this.mdown_rect.y
              + dely));
        new_rect.x = new_left;
        new_rect.y = new_top;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/pos/x', 'INT', val_rect.x],
         ['/input/pos/y', 'INT', val_rect.y]);
      }
      if ((this.mdown_location == "e") || (this.mdown_location == "se")
          || (this.mdown_location == "ne")) {
        var delw = evt.clientX - this.mdown_x;
        var new_w = Math.min(this.maxw, Math.max(this.min_size, this.mdown_rect.w + delw));
        new_rect.w = new_w;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/pos/w', 'INT', val_rect.w]);
      }
      if ((this.mdown_location == "w") || (this.mdown_location == "nw")
          || (this.mdown_location == "sw")) {
        var delx = evt.clientX - this.mdown_x;
        var new_w = Math.min(this.maxw, Math.max(this.min_size, this.mdown_rect.w - delx));
        var delw = new_w - this.mdown_rect.w;
        var new_left = Math.min(this.maxw, Math.max(-this.maxw, this.mdown_rect.x
              - delw));
        var del_left = new_left - this.mdown_rect.x;
        if (del_left != -delw) {
          delw = -del_left;
          new_w = this.mdown_rect.w + delw;
        }
        new_rect.w = new_w;
        new_rect.x = new_left;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/pos/x', 'INT', val_rect.x],
         ['/input/pos/w', 'INT', val_rect.w]);
      }
    }
    if (patches.length > 0) {
      this.setRect(new_rect);
      kr.ctrl_mix(patches, this.path);
    }
  }
}

Input.prototype.select = function() {
  this.active = true;
  this.is_selected = true;
  this.input_elem.classList.add('selected');
  this.rotation_slider_container.activate();
  this.rotation_slider_container.visible();
  this.opacity_slider_container.activate();
  this.opacity_slider_container.visible();
  this.slider_container.addClass('active');
  this.crop_button.classList.add('active');
  this.setRect(this.input_rect);
}

Input.prototype.deselect = function() {
  if (this.active == true) {
    if (this.crop.selected) {
      this.crop.deselect();
    }
    this.active = false;
    this.is_selected = false;
    this.rotation_slider_container.hidden();
    this.rotation_slider_container.deactivate();
    this.opacity_slider_container.hidden();
    this.opacity_slider_container.deactivate();
    this.input_elem.classList.remove('selected');
    this.handle_elem.classList.remove('opaque');
    this.slider_container.removeClass('active');
    this.crop_button.classList.remove('active');
    this.rotation_slider_container.deactivate();
    this.opacity_slider_container.deactivate();
    this.setRect(this.input_rect);
  }
}

Input.prototype.addHandle = function(evt) {
  var loc;
  var mouse = this.getMouse(evt);
  if (this.mdown) {
    loc = this.mdown_location;
  } else {
    loc = this.handlebars(mouse.x, mouse.y, this.input_rect.w,
     this.input_rect.h, this.handle_size);
  }
  if (loc == '') {
    this.handle_elem.classList.remove('opaque');
    return;
  }
  this.handle_elem.classList.add('opaque');
  if ((loc == 'n') || (loc == 'ne') || (loc == 'nw')) {
    this.handle_elem.classList.add('north');
  } else {
    this.handle_elem.classList.remove('north');
  }
  if ((loc == 'w') || (loc == 'nw') || (loc == 'sw')) {
    this.handle_elem.classList.add('west');
  } else {
    this.handle_elem.classList.remove('west');
  }
  if ((loc == 'e') || (loc == 'ne') || (loc == 'se')) {
    this.handle_elem.classList.add('east');
  } else {
    this.handle_elem.classList.remove('east');
  }
  if ((loc == 's') || (loc == 'se') || (loc == 'sw')) {
    this.handle_elem.classList.add('south');
  } else {
    this.handle_elem.classList.remove('south');
  }
  if (loc == 'b') {
    this.handle_elem.classList.add('body');
    this.handle_elem.style.top = this.handle_size + "px";
    this.handle_elem.style.left = this.handle_size + "px";
    this.handle_elem.style.width = (this.input_elem.offsetWidth - 2 * this.handle_size) + "px";
    this.handle_elem.style.height = (this.input_elem.offsetHeight - 2 * this.handle_size) + "px";
  } else {
    this.handle_elem.style.top = '';
    this.handle_elem.style.left = '';
    this.handle_elem.style.width = '';
    this.handle_elem.style.height = '';
    this.handle_elem.classList.remove('body');
  }
}

Input.prototype.setRect = function(rect) {
  var border_width;
  border_width = 1;//parseInt(this.input.css('borderLeftWidth'), 10);
  this.input_rect = rect;
  this.input_elem.style.width = (this.input_rect.w - 2 * border_width) + 'px';
  this.input_elem.style.height = (this.input_rect.h - 2 * border_width) + 'px';
  this.input_elem.style.left = this.input_rect.x + 'px';
  this.input_elem.style.top = this.input_rect.y + 'px';
}

Input.prototype.getRect = function() {
  return { 'x': this.input_rect.x,
           'y': this.input_rect.y,
           'w': this.input_rect.w,
           'h': this.input_rect.h };
}

Input.prototype.rectScaleToComp = function(rect) {
  return { 'x': (kr_comp_w / this.maxw) * rect.x,
           'y': (kr_comp_h / this.maxh) * rect.y,
           'w': (kr_comp_w / this.maxw) * rect.w,
           'h': (kr_comp_h / this.maxh) * rect.h };
}

Input.prototype.rectScaleFromComp = function(rect) {
  return { 'x': (this.maxw / kr_comp_w) * rect.x,
           'y': (this.maxh / kr_comp_h) * rect.y,
           'w': (this.maxw / kr_comp_w) * rect.w,
           'h': (this.maxh / kr_comp_h) * rect.h };
}

Input.prototype.handlebars = function (x, y, width, height, margin) {
  if ((x < 0) || (y < 0) || (x >= width) || (y >= height)) {
    return "";
  }
  if (x < 2 * margin) {
    if (y < 2* margin) {
      return ("nw");
    } else if (y > height - 2 * margin) {
      return ("sw");
    } else if (x < margin) {
    return ("w");
    }
  } else  if (y < 2 * margin) {
    if (x > width - 2 * margin) {
      return ("ne");
    } else if (y < margin) {
      return ("n");
    }
  }
  if (x > width - 2 * margin) {
    if (y > height - 2 * margin) {
      return ("se");
    } else if (width - x < margin) {
      return ("e");
    }
  }
  if (y > height - margin) {
    return ("s");
  }

  return ("b");
};


/*Creates an object with x and y defined, set to the mouse position
   relative to the input.  if you wanna be super-correct
   this can be tricky, we have to worry about padding and borders.*/
Input.prototype.getMouse = function(e) {
  var element = document.getElementById(this.id), offsetX = 0, offsetY = 0, mx, my;
  /*Compute the total offset.*/
  if (element.offsetParent !== undefined) {
    do {
      offsetX += element.offsetLeft;
      offsetY += element.offsetTop;
    } while (element = element.offsetParent);
  }

  /*Add padding and border style widths to offset.
    Also add the <html> offsets in case there's a position:fixed bar*/
  mx = e.pageX - offsetX;
  my = e.pageY - offsetY;

  /*We return a simple javascript object (a hash) with x and y defined.*/
  return {x: mx, y: my};
}

function Crop(input, scener, path, crop) {
  this.scener = scener;
  this.input = input;
  this.path = path;
  this.handle_size = 20;
  this.min_size = 5 * this.handle_size
  this.output_elem = document.getElementById('scnr_krad_output');
  this.maxw = kr_scener_w;
  this.maxh = kr_scener_h;
  this.id = 'cropper_rect' + path.split('/').join('_');
  this.crop_elem = document.createElement('div');
  this.crop_elem.id = this.id;
  this.crop_elem.className = 'krad_input';
  this.output_elem.appendChild(this.crop_elem);
  var hid = this.id + '_handle';
  this.handle_elem = document.createElement('div');
  this.handle_elem.id = hid;
  this.handle_elem.className = "krad_input_handle crop";
  this.crop_elem.appendChild(this.handle_elem);
  this.mdown = false;
  this.mdown_region = "";
  this.mdown_rect = {};
  this.crop_rect = this.rectScaleFromComp(crop);
  this.setRect(this.crop_rect);
  window.addEventListener('mouseup', create_handler(this, this.on_mouseup));
  this.crop_elem.addEventListener('mousedown', create_handler(this, this.on_mousedown));
  window.addEventListener('mousemove', create_handler(this, this.on_mousemove));
}

Crop.prototype.delete = function() {
  if (this.crop_elem.parentNode !== undefined) {
    this.crop_elem.parentNode.removeChild(this.crop_elem);
  }
}

Crop.prototype.activate = function() {
}

Crop.prototype.deactivate = function() {
}

Crop.prototype.add_handle = function(evt) {
  var loc;
  var mouse = this.getMouse(evt);
  if (this.mdown) {
    loc = this.mdown_location;
  } else {
    loc = this.handlebars(mouse.x, mouse.y, this.crop_rect.w,
     this.crop_rect.h, this.handle_size);
  }
  if (loc == '') {
    this.handle_elem.classList.remove('opaque');
    return;
  }
  this.handle_elem.classList.add('opaque');
  if ((loc == 'n') || (loc == 'ne') || (loc == 'nw')) {
    this.handle_elem.classList.add('north');
  } else {
    this.handle_elem.classList.remove('north');
  }
  if ((loc == 'w') || (loc == 'nw') || (loc == 'sw')) {
    this.handle_elem.classList.add('west');
  } else {
    this.handle_elem.classList.remove('west');
  }
  if ((loc == 'e') || (loc == 'ne') || (loc == 'se')) {
    this.handle_elem.classList.add('east');
  } else {
    this.handle_elem.classList.remove('east');
  }
  if ((loc == 's') || (loc == 'se') || (loc == 'sw')) {
    this.handle_elem.classList.add('south');
  } else {
    this.handle_elem.classList.remove('south');
  }
  if (loc == 'b') {
    this.handle_elem.classList.add('body');
    this.handle_elem.style.top = this.handle_size + 'px';
    this.handle_elem.style.left = this.handle_size + 'px';
    this.handle_elem.style.width = (this.crop_elem.offsetWidth - 2 * this.handle_size) + 'px';
    this.handle_elem.style.height = (this.crop_elem.offsetHeight - 2 * this.handle_size) + 'px';
  } else {
    this.handle_elem.style.top = '';
    this.handle_elem.style.left = '';
    this.handle_elem.style.width = '';
    this.handle_elem.style.height = '';
    this.handle_elem.classList.remove('body');
  }
}

Crop.prototype.on_mousedown = function(evt) {
  if (this.selected) {
    var mouse = this.getMouse(evt);
    var mdown_location = this.handlebars(mouse.x, mouse.y, this.crop_rect.w,
     this.crop_rect.h, this.handle_size);
    this.mdown = true;
    this.mdown_x = evt.clientX;
    this.mdown_y = evt.clientY;
    this.mdown_rect.x = this.crop_rect.x
      this.mdown_rect.y = this.crop_rect.y;
    this.mdown_rect.w = this.crop_rect.w;
    this.mdown_rect.h = this.crop_rect.h;
    this.mdown_location = mdown_location;
  }
}

Crop.prototype.on_mouseup = function(evt) {
  if (this.mdown) {
    this.mdown = false;
    this.add_handle(evt);
  }
}

Crop.prototype.on_mousemove = function(evt) {
  if (this.selected) {
    var patches = [];
    var new_rect = this.getRect();
    this.add_handle(evt);
    if (this.mdown) {
      evt.preventDefault();
      evt.stopPropagation();
      if ((this.mdown_location == "n") || (this.mdown_location == "ne")
          || (this.mdown_location == "nw")) {
        var dely = evt.clientY - this.mdown_y;
        var new_h = Math.min(this.maxh,
         Math.max(this.min_size, this.mdown_rect.h - dely));
        var delh = new_h - this.mdown_rect.h;
        var new_top = Math.min(this.maxh, Math.max(0, this.mdown_rect.y
              - delh));
        var del_top = new_top - this.mdown_rect.y;
        if (del_top != -delh) {
          delh = -del_top;
          new_h = this.mdown_rect.h + delh;
        }
        new_rect.h = new_h;
        new_rect.y = new_top;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/crop/y', 'INT', val_rect.y],
         ['/input/crop/h', 'INT', val_rect.h]);
      }
      if ((this.mdown_location == "w") || (this.mdown_location == "nw")
          || (this.mdown_location == "sw")) {
        var delx = evt.clientX - this.mdown_x;
        var new_w = Math.min(this.maxw,
         Math.max(this.min_size, this.mdown_rect.w - delx));
        var delw = new_w - this.mdown_rect.w;
        var new_left = Math.min(this.maxw,
         Math.max(0, this.mdown_rect.x - delw));
        var del_left = new_left - this.mdown_rect.x;
        if (del_left != -delw) {
          delw = -del_left;
          new_w = this.mdown_rect.w + delw;
        }
        new_rect.w = new_w;
        new_rect.x = new_left;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/crop/x', 'INT', val_rect.x], ['/input/crop/w', 'INT',
            val_rect.w]);
      }
      if ((this.mdown_location == "s") || (this.mdown_location == "se")
          || (this.mdown_location == "sw"))  {
        var dely = evt.clientY - this.mdown_y;
        var new_h = Math.min(this.maxh - this.mdown_rect.y, Math.max(this.min_size,
         this.mdown_rect.h + dely));
        new_rect.h = new_h;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/crop/h', 'INT', val_rect.h]);
      }
      if (this.mdown_location == "b") {
        var delx = evt.clientX - this.mdown_x;
        var dely = evt.clientY - this.mdown_y;
        var new_left = Math.min(this.maxw - this.mdown_rect.w,
         Math.max(0, this.mdown_rect.x + delx));
        var new_top = Math.min(this.maxh - this.mdown_rect.h,
         Math.max(0, this.mdown_rect.y + dely));
        new_rect.x = new_left;
        new_rect.y = new_top;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/crop/x', 'INT', val_rect.x], ['/input/crop/y', 'INT',
         val_rect.y]);
      }
      if ((this.mdown_location == "e") || (this.mdown_location == "se")
          || (this.mdown_location == "ne")) {
        var delw = evt.clientX - this.mdown_x;
        var new_w = Math.min(this.maxw - this.mdown_rect.x,
          Math.max(this.min_size, this.mdown_rect.w + delw));
        new_rect.w = new_w;
        var val_rect = this.rectScaleToComp(new_rect);
        patches.push(['/input/crop/w', 'INT', val_rect.w]);
      }
    }
    if (patches.length > 0) {
      this.setRect(new_rect);
      kr.ctrl_mix(patches, this.path);
    }
  }
}

Crop.prototype.deselect = function() {
  if (this.selected == true) {
    this.selected = false;
    this.scener.show_selected_scene();
    this.input.crop_button.classList.remove('on');
    this.crop_elem.classList.remove('selected');
    this.handle_elem.classList.remove('opaque');
    this.crop_elem.classList.remove('opaque');
    this.setRect(this.crop_rect);
  }
}

Input.prototype.show = function() {
  this.input_elem.style.display = 'block';
}

Input.prototype.hide = function() {
  this.input_elem.style.display = 'none';
}

Scener.prototype.show_selected_scene = function() {
  for (var key in this.selected_scene.inputs) {
    var input = this.selected_scene.inputs[key];
    input.show();
  }
}

Scener.prototype.hide_selected_scene = function() {
  for (var key in this.selected_scene.inputs) {
    var input = this.selected_scene.inputs[key];
    input.hide();
  }
}

Crop.prototype.select = function() {
  this.scener.hide_selected_scene();
  this.input.crop_button.classList.add('on');
  this.selected = true;
  this.crop_elem.classList.add('opaque');
  this.crop_elem.classList.add('selected');
  this.setRect(this.crop_rect);
}

Crop.prototype.on_mousedown = function(evt) {
  var mouse = this.getMouse(evt);
  var mdown_location = this.handlebars(mouse.x, mouse.y, this.crop_rect.w,
      this.crop_rect.h, this.handle_size);
  this.mdown = true;
  this.mdown_x = evt.clientX;
  this.mdown_y = evt.clientY;
  this.mdown_rect.x = this.crop_rect.x
    this.mdown_rect.y = this.crop_rect.y;
  this.mdown_rect.w = this.crop_rect.w;
  this.mdown_rect.h = this.crop_rect.h;
  this.mdown_location = mdown_location;
}

Crop.prototype.setRect = function(rect) {
  var border_width;
  border_width = 1;//parseInt(this.crop_elem.style.borderLeftWidth, 10);
  this.crop_rect = rect;
  this.crop_elem.style.width = (this.crop_rect.w - 2 * border_width) + 'px';
  this.crop_elem.style.height = (this.crop_rect.h - 2 * border_width) + 'px';
  this.crop_elem.style.left = this.crop_rect.x + 'px';
  this.crop_elem.style.top = this.crop_rect.y + 'px';
}

Crop.prototype.getRect = function() {
  return { 'x': this.crop_rect.x,
           'y': this.crop_rect.y,
           'w': this.crop_rect.w,
           'h': this.crop_rect.h };
}

Crop.prototype.rectScaleToComp = function(rect) {
  return { 'x': (kr_comp_w / this.maxw) * rect.x,
           'y': (kr_comp_h / this.maxh) * rect.y,
           'w': (kr_comp_w / this.maxw) * rect.w,
           'h': (kr_comp_h / this.maxh) * rect.h };
}

Crop.prototype.rectScaleFromComp = function(rect) {
  return { 'x': (this.maxw / kr_comp_w) * rect.x,
           'y': (this.maxh / kr_comp_h) * rect.y,
           'w': (this.maxw / kr_comp_w) * rect.w,
           'h': (this.maxh / kr_comp_h) * rect.h };
}

Crop.prototype.handlebars = function (x, y, width, height, margin) {
  if ((x < 0) || (y < 0) || (x >= width) || (y >= height)) {
    return "";
  }
  if (x < 2 * margin) {
    if (y < 2* margin) {
      return ("nw");
    } else if (y > height - 2 * margin) {
      return ("sw");
    } else if (x < margin) {
    return ("w");
    }
  } else  if (y < 2 * margin) {
    if (x > width - 2 * margin) {
      return ("ne");
    } else if (y < margin) {
      return ("n");
    }
  }
  if (x > width - 2 * margin) {
    if (y > height - 2 * margin) {
      return ("se");
    } else if (width - x < margin) {
      return ("e");
    }
  }
  if (y > height - margin) {
    return ("s");
  }

  return ("b");
};


/*Creates an object with x and y defined, set to the mouse position
   relative to the input.  if you wanna be super-correct
   this can be tricky, we have to worry about padding and borders.*/
Crop.prototype.getMouse = function(e) {
  var element = document.getElementById(this.id), offsetX = 0, offsetY = 0, mx, my;
  /*Compute the total offset.*/
  if (element.offsetParent !== undefined) {
    do {
      offsetX += element.offsetLeft;
      offsetY += element.offsetTop;
    } while (element = element.offsetParent);
  }

  /*Add padding and border style widths to offset.
    Also add the <html> offsets in case there's a position:fixed bar*/
  mx = e.pageX - offsetX;
  my = e.pageY - offsetY;

  /*We return a simple javascript object (a hash) with x and y defined.*/
  return {x: mx, y: my};
}
function Selectable(list_elem) {
  this.list_elem = list_elem;
  this.list_elem.className += ' selectable';
}

Selectable.prototype.add = function(obj, func, item_elem) {
  item_elem.className += ' selectee';
  this.list_elem.appendChild(item_elem);
  item_elem.addEventListener('click', create_handler(obj, func));
  item_elem.addEventListener('click', create_handler(this, this.on_click));
}

Selectable.prototype.remove = function(item_elem) {
  this.list_elem.removeChild(item_elem);
}

Selectable.prototype.on_click = function(evt) {
  if (!evt.defaultPrevented) {
    var target = evt.target;
    while (!target.classList.contains("selectee")) {
      if (target.parentElement) {
        target = target.parentElement;
      } else {
        target = null;
        break;
      }
    }
    if (target) {
      var matches = this.list_elem.querySelectorAll('.selected');
      target.classList.add("selected");
      for (var i = 0; i < matches.length; i++) {
        if (target !== matches[i]) {
          matches[i].classList.remove("selected");
        }
      }
    }
  }
}
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
  this.max_top = parseInt(window.getComputedStyle(this.trough_elem, null).getPropertyValue('height'), 10);
  this.max_top = this.max_top - parseInt(window.getComputedStyle(this.handle_elem, null).getPropertyValue('height'), 10);
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

KRRange = function(min, max, knee) {
  this.min = min;
  this.max = max;
  this.knee = {};
  this.knee.range = knee.range;
  this.knee.move_fraction = knee.move_fraction;
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

KRSlider.prototype.set_value = function(value) {
  this.set_handle(value);
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
  if (x <= this.range.knee.move_fraction) {
    var maxx = this.range.knee.move_fraction;
    var max = this.db_to_linear(this.range.max);
    var min = this.db_to_linear(this.range.max - this.range.knee.range);
    return this.linear_to_db(max - x * (max - min) / maxx);
  } else {
    var minx = this.range.knee.move_fraction;
    var maxx = 1;
    var max = this.range.max - this.range.knee.range;
    var min = this.range.min;
    return max - (x - minx) * (max - min) / (maxx - minx);
  }
}

KRSlider.prototype.top_scale_from_value = function(val) {
  if (val > (this.range.max - this.range.knee.range)) {
    var x = (this.db_to_linear(this.range.max) - this.db_to_linear(val))
     / (this.db_to_linear(this.range.max)
     - this.db_to_linear(this.range.max - this.range.knee.range));
    return this.max_top * this.range.knee.move_fraction * x;
  } else {
    var maxx = this.max_top;
    var minx = this.max_top * this.range.knee.move_fraction;
    var max = this.range.max - this.range.knee.range;
    var min = this.range.min;
    return minx + (maxx - minx) * (max - val) / (max - min);
  }
}
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

/*$(document).ready(function() {
  rack_units.push({'constructor': Spectrum_unit, 'id': 'spectrum_unit'});
});*/

function Spectrum_unit(info_object) {
  this.inherit_from = KRRackUnit;
  this.inherit_from();
  this.title = "Spectrum_unit";
  this.description = "For testing spectrum.";
  this.id = 'spectrum_unit'
  this.div_text = "<div class='RCU spectrum_unit' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  info_object['parent_div'].append(this.div_text);
  $(this.sel).append('\
   <div class="spectrum_unit danred offwhite robo_light">\
    <div class="spectrum_container" id="spectrum_container"></div>\
   </div>\
  </div>');
  this.spectrum_container_elem = document.getElementById("spectrum_container");
  this.spectrum_canvas_container =
   new CanvasViewContainer(this, this.spectrum_container_elem, 'spectrum');
  this.spectrum_model = new KRSpectrumWebAudioModel(64);
  this.spectrum_view = new KRSpectrumView(this.spectrum_canvas_container,
   this.spectrum_model, this.spectrum_canvas_container.context,
   new KRBoundBox(0, 0, 1, 1),
   CanvasViewContainer.colors_from_color(MIXER_BG_COLOR), {'shadow_length': 12});
  this.spectrum_canvas_container.add_view('spectrum_unit_spectrum_view', this.spectrum_view);
}

Spectrum_unit.prototype = Object.create(KRRackUnit.prototype);
Spectrum_unit.prototype.constructor = Spectrum_unit;
/*$(document).ready(function() {
  rack_units.push({'constructor': KRStats, 'aspect': { dev: [2,1],
   audio: null }, 'page': {dev: 'all', audio: null},
   'position': {dev: [0, 0], audio: null}});
});*/

function KRStats(info_object) {
  this.title = "KRStats";
  this.description = "Display performance statistics.";
  this.id = "stats";
  this.div = "<div class='RCU' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  info_object['parent_div'].append(this.div);
  this.stats = new Stats();
  this.stats.setMode(0);
  this.stats.domElement.style.position = 'absolute';
  this.stats.domElement.style.left = '0px';
  this.stats.domElement.style.top = '0px';
  this.stats.domElement.style.width = '100%';
  this.stats.domElement.style.height = '100%';
  this.container = document.getElementById(this.id);
  this.container.appendChild(this.stats.domElement);
  var stats = this.stats;
  (function animloop() {
    stats.begin();
    requestAnimFrame(animloop);
    stats.end();
  })();
}

KRStats.prototype.update = function() {
}

KRStats.prototype.share = function() {
}

$(document).ready(function() {
  rack_units.push({'constructor': StreamInput, 'aspect': [16, 13],
   'page': 'stream',
   'position': [0, 1]});
});

function StreamInput(info_object) {
  this.title = "StreamInput";
  this.description = "Create stream input.";
  this.id = "stream_input";
  this.sel = "#" + this.id;
  this.main_dom_elem = document.createElement('div');
  this.main_dom_elem.classList.add('RCU');
  this.main_dom_elem.id = this.id;
  $(info_object['parent_div'])[0].appendChild(this.main_dom_elem);
  this.container_elem = document.createElement('div');
  this.container_elem.id = this.id + '_container';
  this.container_elem.classList.add('stream_input_container');
  this.container_elem.classList.add('darkblue');
  this.container_elem.classList.add('offwhite');
  this.creator = new CemoCreator();
  this.main_dom_elem.appendChild(this.container_elem);
  this.paths = {};
  this.make_layout();
}

StreamInput.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/studio/")) return;
  if (crate.meth == "PUT") {
    this.on_new_path(crate);
  } else if (crate.meth == "PATCH") {
    if (this.paths[path] != undefined) {
      jsonpatch.apply(this.paths[path].data, crate.data);
      this.update_path_details(path);
    }
  }
}

StreamInput.prototype.on_new_path = function(crate) {
  this.paths[crate.path] = crate;
}

StreamInput.prototype.make_layout = function() {
  html = '<div id="cemo_creator"> \
    <div class="header"><span class="button header cap">Add Capture</span> \
    <span class="button header enc">Add Encoder</span> \
    <span class="button header mux">Add Muxer</span> \
    <span class="button header out">Add Output</span></div> \
    <div class="header"> \
    <span class="button header play">Add Player</span> \
    </div> \
    <div class="input"></div> \
  </div>';
  $("#stream_input_container").html(html);
  $("#cemo_creator span.button.header").click(function() {
    $("#cemo_creator span.button.header").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
  $("#cemo_creator span.button.cap").click(create_handler(this, this.on_add_capture_click));
  $("#cemo_creator span.button.enc").click(create_handler(this, this.on_add_encoder_click));
  $("#cemo_creator span.button.mux").click(create_handler(this, this.on_add_muxer_click));
  $("#cemo_creator span.button.out").click(create_handler(this, this.on_add_output_click));
  $("#cemo_creator span.button.play").click(create_handler(this, this.on_add_player_click));
}

StreamInput.prototype.on_add_capture_click = function() {
  var creator = this.creator;
  var html = '    <div class="capture">\
  <h3>TYPE</h3> \
  <div class="av_type"> \
  <span class="button" data-value="audio" data-selected="1">AUDIO</span> \
  <span class="button" data-value="video">VIDEO</span> \
  </div> \
  <div class="av_options"></div> \
  <h3>TRACK NAME</h3> \
  <div class="track"> \
  <input type="text"/> \
  </div> \
  <span class="button create">CREATE</span> \
  <span class="button cancel">CANCEL</span> \
  </div>';
  $("#cemo_creator div.input").html(html);
  creator.gen_capture_options($("#cemo_creator div.capture span.button[data-selected='1']").attr("data-value"));
  $("#cemo_creator div.capture div.av_type span.button").click(function() {
    var av_type;
    $("#cemo_creator div.capture div.av_type span.button[data-selected='1']").attr("data-selected", "0");
    $("#cemo_creator div.capture div.av_options").empty();
    $(this).attr("data-selected", "1");
    av_type = $(this).attr("data-value");
    creator.gen_capture_options(av_type);
  });
  $("#cemo_creator span.button.create").click(function() {
    var track;
    var info;
    var av_type;
    av_type = $("#cemo_creator div.capture span.button[data-selected='1']").attr("data-value");
    info = creator.get_capture_info(av_type);
    track = $("#cemo_creator div.track input").val();
    creator.create_capture(track, av_type, info);
  });
  $("#cemo_creator span.button.cancel").click(function() {
    $("#cemo_creator div.input").empty();
  });
}

StreamInput.prototype.on_add_encoder_click = function() {
    var creator = this.creator;
    var html = '    <div class="encode">\
      <h3>TYPE</h3> \
      <div class="av_type"> \
        <span class="button" data-value="audio" data-selected="1">AUDIO</span> \
        <span class="button" data-value="video">VIDEO</span> \
      </div> \
      <h3>ENCODERS</h3>\
      <div class="encoders">\
      </div>\
      <div class="options"></div>\
      <span class="button create">CREATE</span>\
      <span class="button cancel">CANCEL</span>\
    </div>';
  $("#cemo_creator div.input").html(html);
  creator.gen_encoders($("#cemo_creator div.encode div.av_type span.button[data-selected='1']").attr("data-value"));
  $("#cemo_creator div.encode div.av_type span.button").click(function() {
    var av_type;
    $("#cemo_creator div.encode div.av_type span.button[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
    av_type = $(this).attr("data-value");
    creator.gen_encoders(av_type);
  });
  var av_type;
  av_type = $("#cemo_creator div.encode div.av_type span.button[data-selected='1']").attr("data-value");
  creator.gen_enc_options(av_type, $("#cemo_creator div.encoders span.button[data-selected='1']").attr("data-value"));
  $("#cemo_creator div.encode span.button.create").click(function() {
    var type;
    var info;
    var track;
    var av_type;
    av_type = $("#cemo_creator div.encode div.av_type span.button[data-selected='1']").attr("data-value");
    track = $("#cemo_creator div.encode div.track option:selected").val();
    type = $("#cemo_creator div.encoders span.button[data-selected='1']").attr("data-value");
    info = creator.get_enc_info(av_type, type);
    creator.create_encoder(track, type, av_type, info);
  });
  $("#cemo_creator span.button.cancel").click(function() {
    $("#cemo_creator div.input").empty();
  });
}

StreamInput.prototype.on_add_muxer_click = function() {
  var creator = this.creator;
  var html = '    <div class="mux">\
      <h3>MUXERS</h3>\
        <div class="muxers">\
          <span class="button muxer" data-value="raw" data-selected="1">RAW</span>\
          <span class="button muxer" data-value="ogg">OGG</span>\
          <span class="button muxer" data-value="flac">FLAC</span>\
          <span class="button muxer" data-value="ts">MPEG-TS</span>\
          <span class="button muxer" data-value="mkv">MKV</span>\
          <span class="button muxer" data-value="mkv">WEBM</span>\
        </div>\
      <h3>INPUT TRACK</h3>\
        <div class="track" id="track1">\
          <select></select>\
        </div>\
      <h3>INPUT TRACK</h3>\
        <div class="track" id="track2">\
          <select></select>\
        </div>\
      <span class="button create">CREATE</span>\
      <span class="button cancel">CANCEL</span>\
    </div>';
  $("#cemo_creator div.input").html(html);
  $("#cemo_creator span.button.cancel").click(function() {
    $("#cemo_creator div.input").empty();
  });
  creator.get_encode_tracks();
  $("#cemo_creator div.muxers span.button").click(function() {
    $("#cemo_creator div.muxers span.button[data-selected='1']").attr("data-selected", "0");
    $(this).attr("data-selected", "1");
  });
  $("#cemo_creator div.mux span.button.create").click(function() {
    var type;
    var tracks;
    tracks = [];
    tracks[0] = $("#cemo_creator div.mux div.track#track1 option:selected").val();
    tracks[1] = $("#cemo_creator div.mux div.track#track2 option:selected").val();
    type = $("#cemo_creator div.muxers span.button[data-selected='1']").attr("data-value");
    creator.create_muxer(tracks, type);
  });
}

StreamInput.prototype.on_add_output_click = function() {
  var creator = this.creator;
  var html = '    <div class="output">\
      <h3>OUTPUTS</h3>\
      <div class="outputs">\
          <span class="button output" data-value="transmission" data-selected="1">XMIT</span>\
          <span class="button output" data-value="record">RECORD</span>\
          <span class="button output" data-value="uplink">UPLINK</span>\
      </div>\
      <h3>INPUT TRACK</h3>\
      <div class="track">\
        <select></select>\
      </div>\
      <div class="options"></div>\
      <span class="button create">CREATE</span>\
      <span class="button cancel">CANCEL</span>\
    </div>';
  $("#cemo_creator div.input").html(html);
  $("#cemo_creator span.button.cancel").click(function() {
    $("#cemo_creator div.input").empty();
  });
  creator.get_mux_tracks();
  creator.gen_out_options($("#cemo_creator div.outputs span.button[data-selected='1']").attr("data-value"));
  $("#cemo_creator div.output span.button.create").click(function() {
    var type;
    var info;
    var track;
    track = $("#cemo_creator div.output div.track option:selected").val();
    type = $("#cemo_creator div.outputs span.button[data-selected='1']").attr("data-value");
    info = creator.get_out_info(type);
    creator.create_output(track, type, info);
  });
  $("#cemo_creator div.outputs span.button").click(function() {
    var output;
    $("#cemo_creator div.outputs span.button[data-selected='1']").attr("data-selected", "0");
    $("#cemo_creator div.output div.options").empty();
    $(this).attr("data-selected", "1");
    output = $(this).attr("data-value");
    creator.gen_out_options(output);
  });
}

StreamInput.prototype.on_add_player_click = function() {
  var creator = this.creator;
  var html = '    <div class="player">\
      <h3>INPUT TRACK</h3>\
        <div class="track" id="track">\
          <select></select>\
        </div>\
      <span class="button create">CREATE</span>\
      <span class="button cancel">CANCEL</span>\
    </div>';
  $("#cemo_creator div.input").html(html);
  $("#cemo_creator span.button.cancel").click(function() {
    $("#cemo_creator div.input").empty();
  });
  creator.get_encode_tracks('player');
  $("#cemo_creator div.player span.button.create").click(function() {
    var type;
    var track;
    track = $("#cemo_creator div.player div.track#track option:selected").val();
    creator.create_play(track);
  });
}


$(document).ready(function() {
  rack_units.push({'constructor': StreamOutput2});
});

function StreamOutput2(info_object) {
  this.title = "StreamOutput2";
  this.description = "Create stream output.";
  this.id = "stream_output2";
  this.sel = "#" + this.id;
  this.main_elem = document.createElement('div');
  this.main_elem.classList.add('RCU');
  this.main_elem.id = this.id;
  $(info_object['parent_div'])[0].appendChild(this.main_elem);
  this.unit_container = document.createElement('div');
  this.unit_container.classList.add('unit_container');
  this.unit_container.classList.add('offwhite');
  this.container_elem = document.createElement('div');
  this.container_elem.classList.add('stream_output_container');
  this.container_elem.classList.add('darkblue');
  this.container_elem.id = this.id + '_container';
  this.tracker_list_elem = document.createElement('div');
  this.tracker_list_elem.classList.add('tracker_list_container');
  this.tracker_list_elem.classList.add('danred');
  this.tracker_list_elem.id = this.id + '_tracker_list';
  this.tracker_listview = new Listview(this, "Tracker Path List",
   this.on_tracker_list_click);
  this.tracker_listview.set_view_items(5);
  this.tiledarea = document.createElement('div');
  this.tiledarea.classList.add('tiledarea');
  this.tilemate = new Tilemate(this.tiledarea);
  this.tracker_list_elem.appendChild(this.tracker_listview.get_elem());
  this.tracker_list_elem.appendChild(this.tiledarea);
  this.unit_container.appendChild(this.tracker_list_elem);
  this.unit_container.appendChild(this.container_elem);
  this.main_elem.appendChild(this.unit_container);
  this.trackers = {};
  this.captures = {};
  this.muxes = {};
  this.encoders = {};
  this.outputs = {};
  this.make_layout();
  this.add_forms_elem_active = false;
}

StreamOutput2.prototype.on_tracker_list_click = function(obj) {
  var item = this.tracker_listview.get_item(obj.pos);
  var name = item.childNodes[0].textContent;
  var pre = document.createElement('pre');
  pre.innerHTML = JSON.stringify(this.trackers[name].data, null, 2);
  this.tilemate.place([pre]);
  this.tracker_listview.highlight_item(item);
}

StreamOutput2.prototype.make_column = function(name, button_text) {
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

StreamOutput2.prototype.make_layout = function() {
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

StreamOutput2.prototype.get_captures = function() {
  return Object.keys(this.captures);
}

StreamOutput2.prototype.get_muxes = function() {
  return Object.keys(this.muxes);
}

StreamOutput2.prototype.get_encoders = function() {
  return Object.keys(this.encoders);
}

StreamOutput2.prototype.update = function(crate) {
  if (!kr.crate_path_root_match(crate, "/studio/")) return;
  if (crate.meth == "PUT") {
    this.on_new_path(crate);
  }
}

StreamOutput2.prototype.on_new_path = function(crate) {
  var processed = false;
  if (kr.crate_path_root_match(crate, "/studio/tracker/")) {
    this.tracker_listview.add_str(crate.path);
    this.trackers[crate.path] = crate;
    kr.debug("got a tracker: " + crate);
  } else if (crate.data.capture !== undefined) {
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
dualmodes = {
  Horz: 0x00,
  Vert: 0x01
};

trimodes = {
  Vert: 0,
  Top: 1,
  Bottom: 2,
  Left: 3,
  Right: 4
};

function Tilemate(div) {
  this.container = div;
  this.lists = [];
  this.dualmode = dualmodes.Horz;
  this.trimode = trimodes.Vert;
  this.layout = document.createElement("div");
  this.layout.classList.add("tile_layout");
  this.layout.innerHTML = "";
  this.container.appendChild(this.layout);
}

Tilemate.prototype.set_dualmode = function (dualmode) {
  this.dualmode = dualmode;
  this.place(this.lists);
};

Tilemate.prototype.set_trimode = function (trimode) {
  this.trimode = trimode;
  this.place(this.lists);
};

Tilemate.prototype.place = function (lists) {
  var i;
  var subsplit;
  var split;
  var num = lists.length;
  this.lists = lists;
  this.layout.innerHTML = "";
  if (num < 1) {
    return;
  }
  if (num == 1) {
    var full = document.createElement("div");
    full.classList.add("tile");
    full.appendChild(lists[0]);
    this.layout.appendChild(full);
    return;
  }
  if (num == 2) {
    split = document.createElement("div");
    if (this.dualmode == dualmodes.Horz) {
      split.classList.add("column");
    } else if (this.dualmode == dualmodes.Vert) {
      split.classList.add("row");
    }

    for (i = 0; i < num; i++) {
      var half = document.createElement("div");
      half.classList.add("tile");
      half.appendChild(lists[i]);
      split.appendChild(half);
    }
    this.layout.appendChild(split);
    return;
  }
  if (num >= 3) {
    split = document.createElement("div");
    if (this.trimode === trimodes.Vert) {
      split.classList.add("row");
      for (i = 0; i < 3; i++) {
        var third = document.createElement("div");
        third.classList.add("tile");
        third.appendChild(lists[i]);
        split.appendChild(third);
      }
      this.layout.appendChild(split);
      return;
    } else if (this.trimode === trimodes.Top || this.trimode === trimodes.Bottom) {
      split.classList.add("column");
      subsplit = document.createElement("div");
      subsplit.classList.add("row");
    } else if (this.trimode === trimodes.Left || this.trimode === trimodes.Right) {
      split.classList.add("row");
      subsplit = document.createElement("div");
      subsplit.classList.add("column");
    }

    var bigpart = document.createElement("div");
    bigpart.classList.add("tile");
    bigpart.appendChild(lists[0]);

    for (i = 1; i < 3; i++) {
      subpart = document.createElement("div");
      subpart.classList.add("tile");
      subpart.appendChild(lists[i]);
      subsplit.appendChild(subpart);
    }

    if (this.trimode === trimodes.Top || this.trimode === trimodes.Left) {
      split.appendChild(bigpart);
      split.appendChild(subsplit);
    }
    if (this.trimode === trimodes.Bottom || this.trimode === trimodes.Right) {
      split.appendChild(subsplit);
      split.appendChild(bigpart);
    }

    this.layout.appendChild(split);
    return;
  }
};
$(document).ready(function() {
  rack_units.push({'constructor': Uploader});
});

function dequeue(list, item) {
  var index = list.indexOf(item);
  if (index > -1) {
    list.splice(index, 1);
  }
}

function Uploader(info_object) {
  this.title = "Uploader";
  this.description = "For file uploading.";
  this.id = 'uploader'
  this.div_text = "<div class='RCU uploader' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  info_object['parent_div'].append(this.div_text);
  this.queue = [];
  this.active = [];
  this.log = [];
  this.active_max = 1;
  this.num = 0;
  $(this.sel).append('\
   <div class="upload danred offwhite robo_light">\
    <input type="file" name="file_input" id="file_input" multiple="multiple">\
    <div class="in_progress">\
      <div class="select_button kr_push_button blue" id="select_button">\
        <em>FILE SELECT</em>\
      </div>\
      <div class="up_queue"><div class="big">Queued</div><ol id="up_queue"></ol></div>\
      <div class="up_active"><div class="big">Active</div><div class="up_active_item"></div></div>\
    </div>\
    <div class="log"><div class="big">Finished</div><ol id="up_log"></ol></div>\
    </div>\
  </div>');

  $('#file_input').on("change", create_handler(this, this.on_file_select));

  $(this.sel).on("dragover", function(e) {
    e.preventDefault();
    $(this).addClass('over');
  });

  $(this.sel).on("dragenter", function(e) {
    e.preventDefault();
    $(this).addClass('over');
  });

  $(this.sel).on("dragleave", function(e) {
    $(this).removeClass('over');
  });

  $(this.sel).on("drop", create_handler(this, this.on_drop));

  $('#select_button').bind("click", function(e) {
    var file_element = document.getElementById("file_input");
    file_element.click();
    e.preventDefault();
    return false;
  });
}

Uploader.prototype.update = function(crate) {
}

Uploader.prototype.shared = function(key, shared_object) {
}

Uploader.prototype.on_file_select = function() {
  for (var i = 0; i < document.getElementById('file_input').files.length; i++) {
    var file = document.getElementById('file_input').files[i];
    if (file) {
      this.add(file);
    }
  }
}

Uploader.prototype.on_drop = function(evt) {
  evt.preventDefault();
  $(this.sel).removeClass('over');
  for (var i = 0; i < evt.originalEvent.dataTransfer.files.length; i++) {
    var file = evt.originalEvent.dataTransfer.files[i];
    if (file) {
      this.add(file);
    }
  }
}

Uploader.prototype.add = function(file) {
  var u = new UploadItem(file, file.name, this);
  if (this.can_activate()) {
    u.activate();
    this.active.push(u);
  } else {
    u.queue();
    this.queue.push(u);
  }
}

Uploader.prototype.can_activate = function() {
  if (this.active.length < this.active_max) {
    return true;
  } else {
    return false;
  }
}

Uploader.prototype.process_next = function() {
  var next = this.queue.pop();
  if (next) {
    if (this.can_activate()) {
      next.activate();
      this.active.push(next);
    } else {
      next.queue();
      this.queue.push(next);
    }
  }
}

Uploader.prototype.finish = function(u) {
  dequeue(this.active, u);
  this.log.push(u);
  this.process_next();
}

Uploader.prototype.update_uploads = function() {
}

function UploadItem(file, name, rack_unit) {
  this.file = file;
  this.name = name;
  this.rack_unit = rack_unit;
  this.progress;
  this.speed;
  this.time_started;
  this.time_added = new Date();
  this.time_finished;
  this.file_size = 0;
  if (this.file.size > 1000 * 1000)
    this.file_size = (Math.round(this.file.size * 100 / (1000 * 1000)) / 100).toString() + 'MB';
  else
    this.file_size = (Math.round(this.file.size * 100 / 1000) / 100).toString() + 'KB';
}

UploadItem.prototype.on_progress = function(evt) {
  if (evt.lengthComputable) {
    this.percent_complete = Math.round(evt.loaded * 100 / evt.total);
  }
  if (this.prog_id) {
    kr.debug(this.prog_id + ': ' + this.percent_complete + '%');
    $('#' + this.prog_id).html(' ' + this.percent_complete + '%');
  }
}

UploadItem.prototype.on_complete = function(evt) {
  this.time_finished = new Date();
  this.rack_unit.finish(this);
  $('#' + this.active_id).remove();
  $('#up_log').append('<li>SUCCEEDED ' + this.name + '</li>');
}

UploadItem.prototype.on_fail = function(evt) {
  this.time_finished = new Date();
  this.rack_unit.finish(this);
  $('#' + this.active_id).remove();
  $('#up_log').append('<li>FAILED ' + this.name + '</li>');
}

UploadItem.prototype.on_cancel = function(evt) {
  this.time_finished = new Date();
  this.rack_unit.finish(this);
  $('#' + this.active_id).remove();
  $('#up_log').append('<li>CANCELED ' + this.name + '</li>');
}

UploadItem.prototype.queue = function () {
  this.queue_id = 'queue_' + this.num++;
  var li = '<li id="' + this.queue_id + '">' + this.name + '</li>';
  $('#up_queue').append(li);
}

UploadItem.prototype.transfer = function() {
  var ar;
  var fd;
  fd = new FormData();
  fd.append('methisfileuploadmkay', this.file);
  ar = new XMLHttpRequest();
  ar.upload.addEventListener("progress", create_handler(this, this.on_progress), false);
  ar.addEventListener("load", create_handler(this, this.on_complete), false);
  ar.addEventListener("error", create_handler(this, this.on_fail), false);
  ar.addEventListener("abort", create_handler(this, this.on_cancel), false);
  ar.open("POST", "/upload/" + this.name);
  ar.send(fd);
}

UploadItem.prototype.activate = function() {
  if (this.queue_id) {
    $('#' + this.queue_id).remove();
    this.queue_id = null;
  }
  var num = this.rack_unit.num++;
  this.active_id = 'active_' + num;
  this.prog_id = 'prog_' + num;
  var active_item = '<span id=' + this.active_id + '><span>' + this.name + '</span>'
   + '<span id=' + this.prog_id + '></span></span>';
  $('.up_active_item').append(active_item);
  this.time_started = new Date();
  this.transfer();
}
function D(id) {
  return document.getElementById(id);
}

function C(element) {
  return document.getElementById(element).innerHTML;
}

function A(element, content) {
  var e;
  e = document.getElementById(element);
  e.insertAdjacentHTML("beforeend", content);
}

function R(id, content) {
  var e;
  e = document.getElementById(id);
  e.innerHTML = content;
}

function T(id, val) {
  var e;
  e = document.getElementById(id);
  e.value = val;
}


$(document).ready(function() {
  rack_units.push({'constructor': XMMS});
});

XMMS.prototype.window_resize = function(evt) {
  for (var i = 0; i < this.ndecks; i++) {
    this.deck[i].controls.handle_resize();
  }
}

function XMMS(info_object) {
  this.title = "Xmms";
  this.description = "Control Xmms2 music players.";
  this.id = "xmms";
  this.sel = "#xmms";
  this.rcu_elem = document.createElement('div');
  this.rcu_elem.className = 'xmms RCU';
  this.rcu_elem.id = this.id;
  info_object['parent_div'][0].appendChild(this.rcu_elem);
  this.decks_elem = document.createElement('div');
  this.decks_elem.className = 'vbox';
  this.ndecks = 2;
  this.deck_elem = [];
  this.deck = [];
  for (var i = 0; i < this.ndecks; i++) {
    this.deck_elem[i] = document.createElement('div');
    this.deck_elem[i].className = "hbox deck offwhite robo_light";
    this.decks_elem.appendChild(this.deck_elem[i]);
    this.deck[i] = new KRDeck(this.deck_elem[i], kr.ws_uri + 'deck' + (i + 1),
     "DECK" + (i + 1));
  }
  this.rcu_elem.appendChild(this.decks_elem);
}

function KRDeck(deck_elem, url, title) {
  this.url = url;
  this.title = title;
  this.deck_elem = deck_elem;
  this.deck_elem.dataset.title = title;
  this.user_selected = [];
  this.controls_container = document.createElement('div');
  this.controls_container.classList.add('playback');
  this.controls_container.id = this.title + "_controls";
  this.controls_container.style.width = "100%";
  this.controls_container.style.height = "100%";
  this.active_playlist_selector = document.createElement('div');
  this.active_playlist_selector.innerHTML = "Playlist: ";
  this.active_playlist_select = document.createElement('select');
  this.active_playlist_select.addEventListener('change', create_handler(this,
   this.on_playlist_select_change));
  this.active_playlist_selector.appendChild(this.active_playlist_select);

  this.playlist_list_view = new Listview(this, "Playlist List",
   this.on_playlist_list_click);
  this.playlist_list_view.set_view_items(3);

  this.cur_play_art_elem = document.createElement('div');
  this.cur_play_art_elem.classList.add('album_art');

  this.tools = document.createElement('div');
  this.tools.classList.add('vbox');
  this.tools.classList.add('tools');
  this.tools.appendChild(this.playlist_list_view.get_elem());

  this.tiledarea = document.createElement('div');
  this.tiledarea.classList.add('tiledarea');
  this.tilemate = new Tilemate(this.tiledarea);

  this.tool_buttons = document.createElement('div');
  this.tool_buttons.className = "hbox";
  this.delete_button_elem = this.make_button("remove active playlist", ['red',
   'kr_push_button'], this, this.on_delete_playlist_click);
  this.clear_button_elem = this.make_button("clear", ['red', 'kr_push_button'],
   this, this.on_clear_click);
  this.remove_selected_button_elem = this.make_button("remove selected tracks",
   ['red', 'kr_push_button'], this, this.on_remove_selected_click);
  this.sort_button_elem = this.make_button("sort", ['blue',
   'kr_push_button'], this, this.on_sort_click);
  this.shuffle_button_elem = this.make_button("shuffle", ['blue',
   'kr_push_button'], this, this.on_shuffle_click);
  this.tool_buttons.appendChild(this.delete_button_elem);
  this.tool_buttons.appendChild(this.shuffle_button_elem);
  this.tool_buttons.appendChild(this.sort_button_elem);
  this.tool_buttons.appendChild(this.remove_selected_button_elem);
  this.tool_buttons.appendChild(this.clear_button_elem);
  this.tools.appendChild(this.tool_buttons);

  new Textinput(this.tools, 'Add Tracks from Medialib by Pattern: ',
   'add', this, this.on_add_click);
  new Textinput(this.tools, 'Add URL:', 'add', this, this.on_add_url_click);
  new Textinput(this.tools, 'Remove Tracks (positions as: 1,2,... or 1-n): ',
   'remove', this, this.on_remove_click);

  new Textinput(this.tools, 'Create a Playlist: ', 'create', this,
   this.on_add_playlist_click);
  new Textinput(this.tools, 'Rename Active Playlist: ', 'rename', this,
   this.on_rename_playlist_click);

  this.deck_elem.appendChild(this.controls_container);
  this.deck_elem.appendChild(this.tools);
  this.deck_elem.appendChild(this.tiledarea);

  this.client = new xmmsclient.Client("kr_web");
  this.client.onconnect = create_handler(this, this.on_connect);
  this.client.ondisconnect = create_handler(this, this.on_disconnect);
  this.client.connect(this.url);
  this.controls = new PlaybackControl(this.controls_container, this.client);
}

KRDeck.prototype.on_add_url_click = function(string) {
  this.add_url_to_list(xmmsclient.ActivePlaylist, string);
}

KRDeck.prototype.add_url_to_list = function(name, url) {
  var list_name;
  list_name = name ? name : xmmsclient.ActivePlaylist;
  this.client.playlist.add_url(list_name, url);
}

KRDeck.prototype.on_sort_click = function(evt) {
  this.sort_playlist();
}

KRDeck.prototype.sort_playlist = function(name) {
  var list_name;
  var reference, album_sort, artist_sort, tracknr_sort;
  list_name = name ? name : xmmsclient.ActivePlaylist;
  reference = new xmmsclient.Collection.Reference();
  reference.attributes.namespace = "Playlists";
  reference.attributes.reference = list_name;
  tracknr_sort = new xmmsclient.Collection.Order();
  tracknr_sort.attributes.type = "value";
  tracknr_sort.attributes.field = 'tracknr';
  tracknr_sort.operands[0] = reference;
  album_sort = new xmmsclient.Collection.Order();
  album_sort.attributes.type = "value";
  album_sort.attributes.field = 'album';
  album_sort.operands[0] = tracknr_sort;
  artist_sort = new xmmsclient.Collection.Order();
  artist_sort.attributes.type = "value";
  artist_sort.attributes.field = "artist";
  artist_sort.operands[0] = album_sort;
  this.client.playlist.replace(list_name,
   artist_sort, 1);
}

KRDeck.prototype.on_delete_playlist_click = function(evt) {
  var name;
  name = this.current_active;
  this.delete_playlist(name);
}

KRDeck.prototype.delete_playlist = function(name) {
  if (name === this.current_active) {
    var keys = Object.keys(this.playlist_views);
    if (keys) {
      for (var i = 0; i < keys.length; i++){
        var n = keys[i];
        if (n !== name) {
          this.client.playlist.load(n);
          break;
        }
      }
    }
  }
  this.client.collection.remove(name, "Playlists");
}

KRDeck.prototype.on_add_click = function(pattern) {
  this.add_matching_to_playlist(null, pattern);
}

KRDeck.prototype.add_matching_to_playlist = function(playlist_name, pattern) {
  var match;
  var list_name;
  list_name = playlist_name ? playlist_name : xmmsclient.ActivePlaylist;
  match = new xmmsclient.Collection.Match();
  match.attributes.value = pattern;
  this.client.playlist.add_collection(list_name, match);
}

KRDeck.prototype.on_remove_click = function(text) {
  var range_re = /\s*(\d+)\s*-\s*(\d+)/;
  var res;
  var listname;
  var len;
  listname = this.current_active;
  len = this.playlist_views[listname].items_elem.childNodes.length;
  res = range_re.exec(text);
  if (res != null && res.length === 3) {
    var idx1 = Number(res[1]) - 1;
    var idx2 = Number(res[2]);
    if (idx2 > idx1) {
      for (var i = idx2; i > idx1; i--) {
        if (i > 0 && i <= len) {
          this.remove_from_playlist(listname, i);
        }
      }
    }
  } else {
    var items = text.split(',');
    if (items.length > 1) {
      items.sort(function(a, b) {
        return Number(b) - Number(a);
      });
    }
    for (var i = 0, len1 = items.length; i < len1; i++) {
      idx = Number(items[i]);
      if (idx >= 1 && idx <= len) {
        this.remove_from_playlist(listname, idx);
      }
    }
  }
}

/*1-based indexing on these positions.*/
KRDeck.prototype.remove_from_playlist = function(playlist_name, position) {
  var pos;
  pos = Number(position) - 1;
  this.client.playlist.remove_entry(playlist_name, pos);
}

KRDeck.prototype.on_shuffle_click = function(evt) {
  this.playlist_shuffle();
}

KRDeck.prototype.playlist_shuffle = function(name) {
  var reference, shuffled;
  var list_name;
  list_name = name ? name : xmmsclient.ActivePlaylist;
  reference = new xmmsclient.Collection.Reference();
  reference.attributes.namespace = "Playlists";
  reference.attributes.reference = list_name;
  shuffled = new xmmsclient.Collection.Order();
  shuffled.attributes.type = "random";
  shuffled.operands[0] = reference;
  this.client.playlist.replace(list_name,
   shuffled, 2);
}

KRDeck.prototype.on_remove_selected_click = function(evt) {
  var listname;
  var items = this.user_selected;
  listname = xmmsclient.ActivePlaylist;
  if (items.length > 1) {
    items.sort(function(a, b) {
      return Number(b) - Number(a);
    });
  }
  for (var i = 0, len = items.length; i < len; i++) {
    idx = Number(items[i]);
    if (idx > -1) {
      this.remove_from_playlist(listname, idx+1);
    }
  }
}

KRDeck.prototype.on_clear_click = function(evt) {
  this.client.playlist.replace(this.current_active,
   new xmmsclient.Collection.IDList(), 0);
}

KRDeck.prototype.on_playlist_list_click = function(obj) {
  var item = this.playlist_list_view.get_item(obj.pos);
  var name = item.childNodes[0].textContent;
  this.client.playlist.load(name);
}

KRDeck.prototype.on_playlist_select_change = function(evt) {
  var selected = evt.target.selectedOptions[0];
  var name = selected.value;
  this.client.playlist.load(name);
}

KRDeck.prototype.on_disconnect = function() {
  clearInterval(this.playtime_interval);
  this.metadata = null;
};

KRDeck.prototype.on_quit = function() {
  console.log(this.title + " got quit broadcast!");
  this.client.disconnect();
  /* TODO clear deck screen
          upon getting that broadcast, and initiate a reconnection
          attempt interval .. 6.66 seconds */
}

KRDeck.prototype.on_connect = function() {
  this.current_id = null;
  this.playback_status = null;
  this.current_playlist = null;
  this.metadata = null;
  this.client.main.broadcast_quit().onvalue = create_handler(this,
   this.on_quit);
  this.client.playback.status().onvalue = create_handler(this,
   this.on_playback_status);
  this.client.playback.broadcast_status().onvalue = create_handler(this,
   this.on_playback_status);
  this.client.playback.current_id().onvalue = create_handler(this,
   this.on_current_id);
  this.client.playback.broadcast_current_id().onvalue = create_handler(this,
   this.on_current_id);
  this.playtime_interval = setInterval(create_handler(this,
   this.get_playtime), 1000);
  this.client.playback.signal_playtime().onvalue = create_handler(this,
   this.on_playtime);
  this.client.playlist.broadcast_playlist_changed().onvalue = create_handler(
   this, this.on_playlist_changed);
  this.setup_playlist_info();
  this.setup_playlist_list();
  this.setup_collections();
  this.setup_courier();
}

KRDeck.prototype.on_get_connected_clients = function(clients) {
  console.log(this.title + " got connected clients list");
  console.log(clients);
}

KRDeck.prototype.on_client_connected = function(client) {
  console.log(this.title + " got notice of another client connecting");
  console.log(client);
}

KRDeck.prototype.on_client_disconnected = function(client) {
  console.log(this.title + " got notice of another client disconnecting");
  console.log(client);
}

KRDeck.prototype.setup_courier = function() {
  this.client.courier.get_connected_clients().onvalue = create_handler(this,
   this.on_get_connected_clients);
  this.client.ipc_manager.broadcast_client_connected().onvalue = create_handler(this,
   this.on_client_connected);
  this.client.ipc_manager.broadcast_client_disconnected().onvalue = create_handler(this,
   this.on_client_disconnected);
}

KRDeck.prototype.setup_collections = function() {
  /*new Textinput(this.collections_elem, 'Add a Collection', 'add', this,
   this.on_add_collection_click);*/
  /*this.client.collection.list("Collections").onvalue = create_handler(this,
   this.on_collections_list);*/
  /*var u = new xmmsclient.Collection.Universe();
  this.client.collection.query_infos(u, 0, 0, ["artist"], ["artist"]).onvalue =
   create_handler(this, this.on_collection_query);*/
  this.client.collection.broadcast_changed().onvalue = create_handler(this,
   this.on_collection_changed);
}

KRDeck.prototype.on_collection_changed = function(data) {
  console.log(this.title + " got collection changed broadcast");
  var name = data.name;
  var type = data.type;
  switch (type) {
    case xmmsclient.CollectionChanged.RENAME:
      var namespace = data.namespace;
      var newname = data.newname;
      if (namespace == "Playlists") {
        this.change_playlist_name(name, newname);
      }
      break;
    case xmmsclient.CollectionChanged.ADD:
      kr.debug('Collection ADD: ' + JSON.stringify(data));
      var namespace = data.namespace;
      if (namespace === "Playlists") {
        this.create_playlist(name);
      }
      break;
    case xmmsclient.CollectionChanged.REMOVE:
      kr.debug('Collection REMOVE: ' + JSON.stringify(data));
      var namespace = data.namespace;
      if (namespace === "Playlists") {
        this.destroy_playlist(name);
      }
      break;
  }
}

KRDeck.prototype.destroy_playlist = function(name) {
  var opts = this.active_playlist_select.childNodes;
  var opts_array = Array.prototype.slice.call(opts); // converts NodeList to Array
  var copy = opts_array.slice();
  for (var i = 0, len = copy.length; i < len; i++) {
    var opt = copy[i];
    if (opt.value === name) {
      this.active_playlist_select.removeChild(opt);
    }
  }
  this.playlist_views[name].destroy();
  delete this.playlist_views[name];
  this.playlist_list_view.remove_str(name);
}

KRDeck.prototype.create_playlist = function(name) {
  /* playlists list */
  this.playlist_list_view.add_str(name);
  /* playlist view */
  this.playlist_views[name] = new Listview(this, name, this.on_playlist_click);
  this.client.playlist.list_entries(name).onvalue
    = create_handler(this, this.on_idlist_from_playlist, this.playlist_views[name]);
  /* drop down active playlist selector */
  var elem = document.createElement('option');
  elem.value = name;
  elem.text = name;
  this.active_playlist_select.appendChild(elem);
}

KRDeck.prototype.change_playlist_name = function(name, newname) {
  this.playlist_views[newname] = this.playlist_views[name];
  if (this.current_active === name) {
    this.current_active = newname;
  }
  delete this.playlist_views[name];
  this.playlist_views[newname].rename(newname);
  this.playlist_list_view.change_str(name, newname);
  var opts = this.active_playlist_select.childNodes;
  for (var i = 0, len = opts.length; i < len; i++) {
    if (opts[i].value == name) {
      opts[i].textContent = opts[i].value = opts[i].name = newname;
    }
  }
}

KRDeck.prototype.findcollectionbyname = function(name) {
  var collections = document.querySelectorAll('.deck_container[data-title='
   + this.title  + '] .collection');
  for (var i = 0; i < collections.length; i++) {
    var coll = collections[i];
    var collname = coll.getAttribute("data-collectionname");
    if (collname == name) return coll;
  }
  return null;
}

KRDeck.prototype.on_rename_playlist_click = function(text) {
  var newname = text;
  this.client.collection.rename(this.current_active, newname, "Playlists");
}

KRDeck.prototype.on_add_playlist_click = function(text) {
  var name = text;
  var pl = new xmmsclient.Collection.IDList();
  this.client.collection.save(name, "Playlists", pl);
}

KRDeck.prototype.on_add_collection_click = function(text) {
  var name = text;
  var u = new xmmsclient.Collection.Universe();
  var m = new xmmsclient.Collection.Match(null,
   {'artist': 'Stars'});
  this.client.collection.save("AllStars", "Collections", m);
}

KRDeck.prototype.on_collections_list = function(list) {
  for (var i = 0, len = list.length; i < len; i++) {
    var name = list[i];
    var list_view = new Listview(this, name);
    this.client.collection.get(name, "Collections").onvalue
     = create_handler(this, this.on_get_collection, list_view);
  }
}

function collection_type_to_str(coll, coll_type) {
  for (var key in coll.Type) {
    if (coll.Type[key] === coll_type) {
      return key;
    }
  }
}

function collection_to_str(coll) {
  var str = JSON.stringify(coll,
   function(key, value) {
     if (key === "type") {
       return collection_type_to_str(coll, value);
     }
     return value;
   }, 2);
  str += "\n\n";
  var type_str = Object.keys(coll.Type)[coll.type];
  //str += type_str + " \n";
  str += coll.attributes.field + " " + type_str + " " + coll.attributes.value;
  str += "\n"
  str += coll.operands.length + " operands\n";
  /*for (var i = 0; i < coll.operands.length; i++) {
    var op = coll.operands[i];
    str += " Op: " + op + "\n";
  }*/
  return str;
}

KRDeck.prototype.on_get_collection = function(coll, list_view) {
  if (coll instanceof Error) return;
/*  this.client.collection.query_infos(coll, 0, 0, ["artist"], ["artist"]).onvalue =
   create_handler(this, this.on_collection_query);*/
}

KRDeck.prototype.on_collection_query = function(list, list_view) {
  for (var i = 0, len = list.length; i < len; i++) {
    var obj = list[i];
    //   list_view.add_track({artist: obj.artist, title: obj.title, id: obj.mid});
    kr.debug(JSON.stringify(obj));
  }
}

KRDeck.prototype.setup_playlist_list = function() {
  this.client.collection.list("Playlists").onvalue = create_handler(this,
   this.on_playlist_list);
}

KRDeck.prototype.on_playlist_list = function(list) {
  this.playlist_views = {};
  this.active_playlist_select.innerHTML = "";
  for (var i = 0, len = list.length; i < len; i++) {
    if (list[i][0] === "_") continue;
    var name = list[i];
    this.create_playlist(name);
  }
  this.client.playlist.current_active().onvalue = create_handler(this,
   this.on_current_active_playlist);
  this.client.playlist.broadcast_loaded().onvalue = create_handler(
   this, this.on_current_active_playlist);
}

KRDeck.prototype.on_playlist_changed = function(data) {
  kr.debug(data);
  var name = data.name;
  var type = data.type;
  switch (type) {
    case xmmsclient.PlaylistChange.REMOVE:
      var pos = data.position;
      var idx = this.user_selected.indexOf(pos);
      if (idx > -1) {
        for (var i = 0, len = this.user_selected.length; i < len; i++) {
          var u = this.user_selected[i];
          if (u > pos) u--;
          this.user_selected[i] = u;
        }
      }
      this.user_selected.splice(idx, 1);
      this.playlist_views[name].remove_track(pos);
      break;
    case xmmsclient.PlaylistChange.ADD:
      var id = data.id;
      var pl_view = this.playlist_views[name];
      pl_view.add_track({id: id, artist:'', title:''});
      this.client.medialib.get_info(id).onvalue = create_handler(
       this, this.on_playlist_get_info, pl_view);
      break;
    case xmmsclient.PlaylistChange.INSERT:
      var pos = data.position;
      var id = data.id;
      this.playlist_views[name].insert_track({id: id,
       artist: '', title: ''}, pos);
      this.client.medialib.get_info(id).onvalue = create_handler(
       this, this.on_playlist_get_info, this.playlist_views[name]);
      break;
    case xmmsclient.PlaylistChange.MOVE:
      var pos = data.position;
      var new_pos = data.newposition;
      var pl_view = this.playlist_views[name];
      if (new_pos > pos) new_pos++;
      pl_view.move_track(pos, new_pos);
      break;
    case 8: /*8 is the type value I get for shuffle, clear, and sort.*/
      this.client.playlist.list_entries(name).onvalue
       = create_handler(this, this.on_idlist_from_playlist,
        this.playlist_views[name]);
      break;
  }
}

KRDeck.prototype.findplaylistbyname = function(playlistname) {
  var playlists = document.querySelectorAll('.deck_container[data-title='
   + this.title  + '] .playlist');
  for (var i = 0; i < playlists.length; i++) {
    var pl = playlists[i];
    var plname = pl.getAttribute("data-playlistname");
    if (plname == playlistname) return pl;
  }
  return null;
}

KRDeck.prototype.findplaylisttrackdivsbymediaid = function(playlist, mid) {
  return playlist.getElementsByClassName('.deck_container[data-title='
   + this.title + '] ' + " item_" + mid);
}

KRDeck.prototype.findalltracksbymediaid = function(mid) {
  return document.querySelectorAll('.deck_container[data-title='
   + this.title + '] .playlists > .items > .item_' + mid);
}

KRDeck.prototype.on_idlist_from_playlist = function(ids, pl_view) {
  var pl_ids = {};
  pl_view.clear();
  for (var i = 0, len = ids.length; i < len; i++) {
    pl_view.add_track({id: ids[i], artist: '', title: ''});
    pl_ids[ids[i]] = 1;
  }
  for (var key in pl_ids) {
    var id = parseInt(key);
    this.client.medialib.get_info(id).onvalue = create_handler(
     this, this.on_playlist_get_info, pl_view);
  }
  this.client.playlist.current_pos(pl_view.name).onvalue = create_handler(
   this, this.on_current_position_playlist);
}

KRDeck.prototype.on_playlist_get_info = function(propdict, pl_view) {
  var metadata;
  var artist;
  var title;
  title = "";
  artist = "";
  metadata = xmmsclient.PropDict.flatten(propdict);
  var id = metadata.id;
  if (metadata.title === undefined) {
    if (metadata.url !== undefined) {
      title = metadata.url;
    }
  } else {
    title = metadata.title;
  }
  artist = metadata.artist;
  pl_view.update_tracks_with_mid(id, {artist: artist,
   title: title});
}

KRDeck.prototype.setup_playlist_info = function() {
  this.current_pos = null;
  this.current_active = null;
  this.client.playlist.broadcast_current_pos().onvalue
   = create_handler(this, this.on_current_position_playlist);
}

KRDeck.prototype.on_current_active_playlist = function(name) {
  this.current_active = name;
  this.client.playlist.current_pos(name).onvalue = create_handler(
   this, this.on_current_position_playlist);
  this.tilemate.place([this.playlist_views[name].get_elem()]);
  var items = this.playlist_list_view.get_items();
  for (var i = 0, len = items.length; i < len; i++) {
    var item = items[i];
    if (item.textContent === name) {
      this.playlist_list_view.highlight_item(item);
      break;
    }
  }
  var opts = this.active_playlist_select.children;
  for (var i = 0, len = opts.length; i < len; i++) {
    if (opts[i].value == name) {
      opts[i].selected = true;
    } else {
      opts[i].selected = false;
    }
  }
}

KRDeck.prototype.on_current_position_playlist = function(data) {
  var listview = this.playlist_views[this.current_active];
  this.current_pos = data.position;
  if (this.current_pos < 0) return;
  listview.highlight_pos(this.current_pos);
}

KRDeck.prototype.get_playtime = function() {
  this.client.playback.playtime().onvalue = create_handler(this,
   this.on_playtime);
}

KRDeck.prototype.on_playtime = function(time) {
  if (this.metadata === null) return;
  if (this.metadata.duration === undefined) return;
  this.controls.set_playtime(time);
}

KRDeck.prototype.zpad = function(s) {
  if (s > 9) return s;
  return '0' + s;
}

KRDeck.prototype.playtime_to_string = function(time) {
  var milisecs = time % 1000;
  var s = (time - milisecs) / 1000;
  var secs = s % 60;
  s = (s - secs) / 60;
  var mins = s % 60;
  var hours = (s - mins) / 60;
  if (hours) {
    return this.zpad(hours) + ':' + this.zpad(mins) + ':' +
     this.zpad(secs);
  } else {
    return this.zpad(mins) + ':' + this.zpad(secs);
  }
}

KRDeck.prototype.on_get_info = function(propdict) {
  var metadata;
  metadata = xmmsclient.PropDict.flatten(propdict);
  this.metadata = metadata;
  this.client.playback.playtime().onvalue = create_handler(this,
   this.on_playtime);
  this.controls.set_track_info(metadata);
  /*if (this.metadata.picture_front) {
    this.client.bindata.retrieve(this.metadata.picture_front).onvalue
     = create_handler(this, this.on_picture_front);
  }*/
}

KRDeck.prototype.make_button = function(text, classes, user, cb) {
  var elem = document.createElement('div');
  for (var i = 0, len = classes.length; i < len; i++) {
    elem.classList.add(classes[i]);
  }
  elem.innerHTML = "<em>" + text + "</em>";
  elem.addEventListener('click', create_handler(user, cb));
  return elem;
}


KRDeck.prototype.on_picture_front = function(url) {
  var img = document.createElement('img');
  img.src = url;
  img.style.height = "120px";
  this.cur_play_art_elem.appendChild(img);
}

KRDeck.prototype.on_playback_status = function(status_num) {
  this.playback_status = status_num;
  switch(status_num) {
    case xmmsclient.PlaybackStatus.PLAY:
      this.controls.set_playback_state(1);
      break;
    case xmmsclient.PlaybackStatus.PAUSE:
      this.controls.set_playback_state(2);
      break;
    case xmmsclient.PlaybackStatus.STOP:
      this.controls.set_playback_state(0);
      break;
  }
}

KRDeck.prototype.on_playlist_click = function(obj) {
  if (this.dblclick_timer) {
    clearTimeout(this.dblclick_timer);
    this.dblclick_timer = null;
    this.on_playlist_dblclick(obj);
  } else {
    this.dblclick_timer = setTimeout(create_handler(this,
     this.on_playlist_dblclick_timeout, obj), 250);
  }
}


KRDeck.prototype.on_playlist_dblclick = function(obj) {
  this.jump_to_pos(obj.pos);
}

KRDeck.prototype.on_playlist_single_click = function(obj) {
  var e = obj.evt;
  var pos = obj.pos;
  if (e.shiftKey && this.user_selected.length > 0) {
    var most_recent = this.user_selected[this.user_selected.length-1];
    var hi, low;
    if (pos > most_recent) {
      low = most_recent;
      hi = pos;
    } else {
      low = pos;
      hi = most_recent;
    }
    for (var i = low; i <= hi; i++) {
      this.user_select(i);
    }
  } else if (e.ctrlKey) {
    var idx;
    idx = this.user_selected.indexOf(pos);
    if (idx > -1) {
      this.user_unselect(pos);
    } else {
      this.user_select(pos);
    }
  } else {
    var idx;
    idx = this.user_selected.indexOf(pos);
    if (idx > -1) {
      this.user_unselect_all();
    } else {
      this.user_unselect_all();
      this.user_select(pos);
    }
  }
}

KRDeck.prototype.on_playlist_dblclick_timeout = function(bogus, data) {
  var pos;
  var evt;
  pos = data.pos;
  evt = data.evt;
  this.dblclick_timer = null;
  this.on_playlist_single_click(data);
}

KRDeck.prototype.user_select = function(pos) {
  var listview = this.playlist_views[this.current_active];
  listview.user_select(pos);
  this.user_selected.push(pos);
}

KRDeck.prototype.user_unselect = function(pos) {
  var listview = this.playlist_views[this.current_active];
  listview.user_unselect(pos);
  var idx = this.user_selected.indexOf(pos);
  if (idx > -1) {
    this.user_selected.splice(idx, 1);
  }
}

KRDeck.prototype.user_unselect_all = function() {
  var items = this.user_selected.slice();
  if (items.length > 1) {
    items.sort(function(a, b) {
      return Number(b) - Number(a);
    });
  }
  for (var i = 0, len = items.length; i < len; i++) {
    idx = Number(items[i]);
    if (idx >= 1) {
      this.user_unselect(idx);
    }
  }
}

KRDeck.prototype.jump_to_pos = function(pos) {
  if (this.current_pos === pos) return;
  this.client.playlist.set_next(pos);
  this.client.playback.tickle();
}

KRDeck.prototype.on_current_id = function(id) {
  this.current_id = id;
  this.cur_play_art_elem.innerHTML = '';
  this.client.medialib.get_info(id).onvalue = create_handler(
   this, this.on_get_info);
}

Textinput.prototype.destroy = function() {
  this.div.remove();
}

Textinput.prototype.activate = function() {
  var val = this.textinput.value;
  this.callback.call(this.user, val);
  this.textinput.value = "";
}

Textinput.prototype.onkey = function(event) {
  if (event.keyCode != 13) return;
  this.activate();
}

function Textinput(container_elem, labeltext, button_labeltext, user, callback) {
  this.callback = callback;
  this.user = user;
  this.textinput = document.createElement('input');
  this.div = document.createElement('div');
  this.label = document.createElement('label');
  this.button = document.createElement('div');
  this.button_label = document.createElement('em');
  this.textinput.className = "textinput";
  this.label.innerHTML = labeltext;
  this.button.className = "kr_push_button";
  this.button_label.innerHTML = button_labeltext;
  this.button.appendChild(this.button_label);
  this.div.appendChild(this.label);
  this.label.appendChild(this.textinput);
  this.div.appendChild(this.button);
  this.button.onclick = create_handler(this, this.activate);
  this.textinput.onkeypress = create_handler(this, this.onkey);
  container_elem.appendChild(this.div);
}

Listview.prototype.user_select = function(pos) {
  var item = this.get_item(pos);
  item.classList.add('user_selected');
}

Listview.prototype.user_unselect = function(pos) {
  var item = this.get_item(pos);
  item.classList.remove('user_selected');
}

Listview.prototype.unhighlight_item = function(item) {
  item.classList.remove('highlight');
  if (item === this.highlighted_item) {
    this.highlighted_item = null;
  }
}

Listview.prototype.get_pos = function(item_elem) {
  for (var i = 0, len = this.items_elem.childNodes.length; i < len; i++) {
    if (item_elem === this.items_elem.childNodes[i]) {
      return i;
    }
  }
  return -1;
}

Listview.prototype.clear = function() {
  this.items = 0;
  this.items_elem.innerHTML = "";
}

Listview.prototype.get_items = function() {
  return this.items_elem.childNodes;
}

Listview.prototype.get_item = function(pos) {
  return this.items_elem.childNodes[pos];
}

Listview.prototype.highlight_pos = function(pos) {
  this.highlight_item(this.items_elem.childNodes[pos]);
}

Listview.prototype.highlight_item = function(item) {
  if (item === undefined) return;
  if (this.highlighted_item !== null) {
    this.unhighlight_item(this.highlighted_item);
  }
  item.classList.add('highlight');
  this.make_visible(this.get_pos(item));
  this.highlighted_item = item;
}

Listview.prototype.make_visible = function(pos) {
  if (pos < 0) return;
  var scroll_top = this.view.scrollTop / this.item_height;
  if (scroll_top > pos) {
    this.scroll(null, pos);
  } else if (scroll_top + this.view_items <= pos) {
    this.scroll(null, pos + 1 - this.view_items);
  }
}

Listview.prototype.scroll_rel = function(evt, num) {
  console.log(this.playlist_name + " rel scroll by " + num);
  this.view.scrollTop += num * this.item_height;
};

Listview.prototype.scroll = function(evt, num) {
  console.log(this.playlist_name + " scroll to " + num);
  this.view.scrollTop = num * this.item_height;
};

Listview.prototype.remove_track = function(position) {
  var items = this.get_items();
  this.items_elem.removeChild(items[position]);
  for (var i = position; i < items.length; i++) {
    var item = items[i];
    var pos = item.childNodes[0];
    pos.textContent = new Number(pos.textContent) - 1;
  }
  this.items--;
}

Listview.prototype.insert_track = function(track, position) {
  this.add_track(track);
  this.move_track(this.items_elem.childNodes.length-1, position);
}

Listview.prototype.move_track = function(old_position, new_position) {
  var item = this.items_elem.childNodes[old_position];
  var item_at_new_position = this.items_elem.childNodes[new_position];
  if (new_position === old_position) return;
  this.items_elem.removeChild(item);
  this.items_elem.insertBefore(item, item_at_new_position);
  item.childNodes[0].textContent = new_position + 1;
  if (new_position > old_position) {
    for (var i = old_position; i < new_position; i++) {
      var it = this.items_elem.childNodes[i];
      var pos = it.childNodes[0];
      pos.textContent = new Number(pos.textContent) - 1;
    }
  } else {
    for (var i = new_position + 1; i < old_position + 1; i++) {
      var it = this.items_elem.childNodes[i];
      var pos = it.childNodes[0];
      pos.textContent = new Number(pos.textContent) + 1;
    }
  }
}

Listview.prototype.add_track = function(track) {
  var item = document.createElement("div");
  var pos = document.createElement("div");
  var title_elem = document.createElement("div");
  var artist_elem = document.createElement("div");
  item.classList.add("item");
  if (track.id) {
    item.classList.add("item_" + track.id);
  }
  pos.className = "number";
  title_elem.className = "title";
  artist_elem.className = "artist";
  pos.textContent = this.items + 1;
  title_elem.textContent = track.title;
  artist_elem.textContent = track.artist;
  item.appendChild(pos);
  item.appendChild(title_elem);
  item.appendChild(artist_elem);
  this.items_elem.appendChild(item);
  this.items++;
};

Listview.prototype.findtrackdivsbymediaid = function(mid) {
  return this.items_elem.getElementsByClassName('item_' + mid);
}

Listview.prototype.update_tracks_with_mid = function(mid, track) {
  var item_elems = this.findtrackdivsbymediaid(mid);
  for (var i = 0, len = item_elems.length; i < len; i++) {
    var item_elem = item_elems[i];
    var title_elem = item_elem.getElementsByClassName('title')[0];
    var artist_elem = item_elem.getElementsByClassName('artist')[0];
    title_elem.textContent = track.title;
    artist_elem.textContent = track.artist;
  }
}

Listview.prototype.change_str = function(str, newstr) {
  for (var i = 0, len = this.items_elem.childNodes.length; i < len; i++) {
    var item = this.items_elem.childNodes[i];
    var child = item.childNodes[0];
    if (child.classList.contains("single")) {
      if (child.textContent === str) {
        child.textContent = newstr;
      }
    }
  }
}

Listview.prototype.remove_str = function(str) {
  for (var i = 0, len = this.items_elem.childNodes.length; i < len; i++) {
    var item = this.items_elem.childNodes[i];
    var child = item.childNodes[0];
    if (child.classList.contains("single")) {
      if (child.textContent === str) {
        this.items_elem.removeChild(item);
        this.items--;
      }
    }
  }
}

Listview.prototype.add_str = function(str) {
  var item = document.createElement("div");
  var str_elem = document.createElement("div");
  str_elem.dataset.str = str;
  item.className = "item";
  str_elem.className = "single";
  str_elem.textContent = str;
  item.appendChild(str_elem);
  this.items_elem.appendChild(item);
  this.items++;
};

Listview.prototype.handle_click = function(e) {
  var item;
  if (e.target.classList.contains("item")) {
    item = e.target;
  } else if (e.target.classList.contains("items")) {
    return;
  } else {
    item = e.target.parentElement;
  }
  var pos = this.get_pos(item);
  if (this.click_cb) {
    var obj = { evt: e, pos: pos };
    this.click_cb.call(this.user, obj);
  }
};

Listview.prototype.get_elem = function() {
  return this.list_elem;
};

Listview.prototype.set_view_items = function(num) {
  this.view_items = num;
  this.view.style.height = this.item_height * this.view_items + "px";
  this.page_up_elem.onclick =
   create_handler(this, this.scroll_rel, -1 * this.view_items);
  this.page_down_elem.onclick =
   create_handler(this, this.scroll_rel, 1 * this.view_items);
};

Listview.prototype.rename = function(name) {
  this.name = name;
  this.list_elem.dataset.listname = name;
  this.list_name_elem.textContent = name;
}

Listview.prototype.destroy = function() {
}

function Listview(user, name, click_cb) {
  this.user = user;
  this.click_cb = click_cb;
  this.item_height = 32;
  this.items = 0;
  this.name = name;
  this.highlighted_item = null;
  this.list_elem = document.createElement("div");
  this.list_elem.dataset.listname = this.name;
  this.list_elem.className = "listview";
  this.list_name_elem = document.createElement("div");
  this.list_name_elem.className = "listview_name";
  this.list_name_elem.textContent = this.name;
  this.view = document.createElement("div");
  this.page_up_elem = document.createElement("div");
  this.page_up_elem.className = "up";
  this.page_up_elem.textContent = "PAGE UP";
  this.page_down_elem = document.createElement("div");
  this.page_down_elem.className = "down";
  this.page_down_elem.textContent = "PAGE DOWN";
  this.set_view_items(10);
  this.items_elem = document.createElement("div");
  this.controls_elem = document.createElement("div");
  this.controls_elem.className = "controls";
  this.up_elem = document.createElement("div");
  this.up_elem.className = "up";
  this.up_elem.textContent = "UP";
  this.down_elem = document.createElement("div");
  this.down_elem.className = "down";
  this.down_elem.textContent = "DOWN";
  this.up_elem.onclick = create_handler(this, this.scroll_rel, -1);
  this.down_elem.onclick = create_handler(this, this.scroll_rel, 1);
  this.controls_elem.appendChild(this.up_elem);
  this.controls_elem.appendChild(this.down_elem);
  this.controls_elem.appendChild(this.page_up_elem);
  this.controls_elem.appendChild(this.page_down_elem);
  this.view.className = "view";
  this.items_elem.className = "items";
  this.list_elem.appendChild(this.list_name_elem);
  this.list_elem.appendChild(this.view);
  this.list_elem.appendChild(this.controls_elem);
  this.view.appendChild(this.items_elem);
  this.items_elem.addEventListener('click',
   create_handler(this, this.handle_click));
}
function Kr_alsa() {
  this.card = 0;
  this.name = "";
}

function Kr_alsa_path() {
  this.card_num = 0;
}

function Kr_decklink() {
  this.name = "";
  this.num = 0;
}

function Kr_decklink_path() {
  this.width = 0;
  this.height = 0;
  this.num = 0;
  this.den = 0;
  this.video_conn = "sdi";
  this.audio_conn = "embedded";
}

function Kr_v4l2_path() {
  this.width = 0;
  this.height = 0;
  this.num = 0;
  this.den = 0;
  this.format = 0;
}

function Kr_v4l2() {
  this.dev = 0;
  this.priority = 0;
  this.state = "open";
}

function Kr_aux() {
  this.state = 0;
}

function Kr_aux_path() {
  this.state = "connected";
}

function Kr_jack() {
  this.client_name = "";
  this.server_name = "";
  this.state = "online";
  this.inputs = 0;
  this.outputs = 0;
  this.sample_rate = 0;
  this.period_size = 0;
  this.xruns = 0;
  this.frames = 0;
}

function Kr_jack_path() {
  this.name = "";
  this.channels = 0;
  this.direction = "input";
}

function Kr_wayland() {
  this.display_name = "";
  this.state = "disconnected";
}

function Kr_wayland_path() {
  this.width = 0;
  this.height = 0;
  this.fullscreen = 0;
}

function Kr_x11() {
  this.display = "";
}

function Kr_x11_path() {
  this.display = "";
  this.width = 0;
  this.height = 0;
  this.num = 0;
  this.den = 0;
  this.x = 0;
  this.y = 0;
}

function Kr_eq_band() {
  this.db = 0.00;
  this.bw = 1.00;
  this.hz = 0.00;
}

function Kr_eq() {
  this.band = [kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init(), kr_eq_band_init()];
}

function Kr_lowpass() {
  this.bw = 0.66;
  this.hz = 20000.00;
}

function Kr_highpass() {
  this.bw = 0.66;
  this.hz = 20.00;
}

function Kr_pass() {
  this.low = new Kr_lowpass();
  this.high = new Kr_highpass();
}

function Kr_volume() {
  this.gain = 0.00;
  this.fader = 0.00;
}

function Kr_analog() {
  this.drive = 0.00;
  this.blend = 0.00;
}

function Kr_meter() {
  this.avg = -120.00;
  this.peak = -120.00;
}

function Kr_afx() {
  this.pass = new Kr_pass();
  this.analog = new Kr_analog();
  this.eq = new Kr_eq();
  this.volume = new Kr_volume();
  this.meter = new Kr_meter();
}

function Kr_codec() {
  this.type = "audio";
  this.audio = "flac";
  this.video = "daala";
}

function Kr_audio() {
  this.channels = 0;
  this.bit_depth = 0;
  this.sample_rate = 0;
  this.samples = 0;
}

function Kr_video() {
  this.width = 0;
  this.height = 0;
  this.fps_num = 0;
  this.fps_den = 0;
  this.pixel_fmt = "argb";
}

function Kr_av() {
  this.type = "audio";
  this.audio = new Kr_audio();
  this.video = new Kr_video();
}

function Kr_daala_encoder() {
  this.width = 0;
  this.height = 0;
  this.pixel_fmt = "argb";
  this.fps_num = 0;
  this.fps_den = 0;
  this.bitrate = 0;
  this.key_rate = 0;
}

function Kr_vpx_encoder() {
  this.width = 0;
  this.height = 0;
  this.pixel_fmt = "argb";
  this.fps_num = 0;
  this.fps_den = 0;
  this.bit_rate = 0;
  this.key_rate = 0;
}

function Kr_h264_encoder() {
  this.width = 0;
  this.height = 0;
  this.pixel_fmt = "argb";
  this.fps_num = 0;
  this.fps_den = 0;
  this.bit_rate = 0;
  this.key_rate = 0;
}

function Kr_theora_encoder() {
  this.width = 0;
  this.height = 0;
  this.pixel_fmt = "argb";
  this.fps_num = 0;
  this.fps_den = 0;
  this.key_rate = 0;
  this.quality = 0;
  this.eos = 0;
}

function Kr_y4m_encoder() {
  this.width = 0;
  this.height = 0;
  this.pixel_fmt = "argb";
  this.fps_num = 0;
  this.fps_den = 0;
}

function Kr_flac() {
  this.channels = 0;
  this.bit_depth = 0;
  this.sample_rate = 0;
  this.frame_size = 0;
  this.total_frames = 0;
  this.total_bytes = 0;
  this.total_samples = 0;
}

function Kr_opus() {
  this.channels = 0;
  this.sample_rate = 0;
  this.bit_rate = 0;
  this.frame_size = 0;
  this.app = "audio";
  this.total_frames = 0;
  this.total_bytes = 0;
}

function Kr_vorbis() {
  this.channels = 0;
  this.sample_rate = 0;
  this.quality = 0.00;
  this.small_block_sz = 0;
  this.large_block_sz = 0;
  this.total_frames = 0;
  this.total_bytes = 0;
}

function Kr_mp3() {
  this.channels = 0;
  this.sample_rate = 0;
  this.bit_rate = 0;
  this.total_frames = 0;
  this.total_bytes = 0;
  this.total_samples = 0;
}

function Kr_aac() {
  this.channels = 0;
  this.sample_rate = 0;
  this.bit_rate = 576000;
  this.total_samples = 0;
  this.total_bytes = 0;
}

function Kr_lavc_encoder() {
  this.bongo = 0;
}

function Kr_compositor_output() {
  this.opacity = 1.00;
}

function Kr_compositor_bus() {
  this.opacity = 1.00;
}

function Kr_compositor_input() {
  this.opacity = 1.00;
  this.pos = new Kr_rect();
  this.crop = new Kr_rect();
  this.view = new Kr_perspective_view();
  this.z = 0;
  this.rotation = 0.00;
}

function Kr_compositor_source() {
  this.opacity = 1.00;
  this.crop = new Kr_rect();
  this.view = new Kr_perspective_view();
}

function Kr_compositor_overlay() {
  this.type = "text";
  this.text = new Kr_text();
  this.vector = new Kr_vector();
}

function Kr_compositor_path() {
  this.type = "source";
  this.source = new Kr_compositor_source();
  this.overlay = new Kr_compositor_overlay();
  this.input = new Kr_compositor_input();
  this.bus = new Kr_compositor_bus();
  this.output = new Kr_compositor_output();
}

function Kr_mts_packet() {
  this.type = "video";
  this.id = 0;
  this.track = 0;
  this.pts = 0;
  this.dts = 0;
  this.sz = 0;
  this.pos = 0;
}

function Kr_mts_stream() {
  this.id = 0;
  this.type_str = "";
  this.codec = new Kr_codec();
  this.type = "video";
  this.video = new Kr_audio();
  this.audio = new Kr_video();
  this.other = 0;
}

function Kr_mts_program() {
  this.id = 0;
  this.streams = [kr_mts_stream_init(), kr_mts_stream_init(), kr_mts_stream_init(), kr_mts_stream_init(), kr_mts_stream_init(), kr_mts_stream_init(), kr_mts_stream_init(), kr_mts_stream_init()];
  this.nstreams = 0;
}

function Kr_mts() {
  this.npackets = 0;
  this.programs = [kr_mts_program_init(), kr_mts_program_init()];
  this.nprograms = 0;
}

function Kr_text() {
  this.string = "";
  this.font = "";
  this.red = 0.00;
  this.green = 0.00;
  this.blue = 0.00;
}

function Kr_vector() {
  this.type = "hex";
  this.red = 0.00;
  this.green = 0.00;
  this.blue = 0.00;
}

function Kr_feature_aspect() {
  this.id = "";
  this.type = "tag";
  this.value = "";
}

function Kr_feature() {
  this.id = "";
  this.name = "";
  this.aspects = [kr_feature_aspect_init()];
}

function Kr_image_stash_item() {
  this.type = "enc";
  this.enc = "png";
  this.raw = "argb";
  this.width = 0;
  this.height = 0;
  this.sz = 0;
  this.created_on = 0;
}

function Kr_rect() {
  this.x = 0;
  this.y = 0;
  this.w = 7680;
  this.h = 4320;
}

function Kr_image() {
  this.w = 0;
  this.h = 0;
  this.fmt = "argb";
}

function Kr_coded_image() {
  this.fmt = "png";
  this.sz = 0;
  this.image = new Kr_image();
}

function Kr_file2() {
  this.access_mode = "append";
  this.len = 0;
  this.path = "";
  this.sz = 0;
  this.wrote = 0;
  this.remote_fs = 0;
  this.lastmod = 0;
  this.type = "";
}

function Kr_mixer_path() {
  this.type = "source";
  this.channels = "mono";
  this.afx = [kr_afx_init(), kr_afx_init()];
}

function Kr_media_file() {
  this.name = "";
}

function Kr_media_metadata() {
  this.name = "";
  this.duration = 0.00;
  this.width = 0;
  this.height = 0;
}

function Kr_media_url() {
  this.url = "";
}

function Kr_media_item() {
  this.create_time = new Kr_timestamp();
  this.task = "";
  this.type = "url";
  this.url = new Kr_media_url();
  this.metadata = new Kr_media_metadata();
  this.file = new Kr_media_file();
}

function Kr_media() {
  this.id = "";
  this.nitems = 0;
  this.create_time = new Kr_timestamp();
  this.item = [kr_media_item_init(), kr_media_item_init(), kr_media_item_init(), kr_media_item_init()];
  this.media_dir = "";
}

function Kr_ops() {
  this.sysname = "";
}

function Kr_pos() {
  this.x = 0;
  this.y = 0;
}

function Kr_perspective_view() {
  this.top_left = new Kr_pos();
  this.top_right = new Kr_pos();
  this.bottom_left = new Kr_pos();
  this.bottom_right = new Kr_pos();
}

function Kr_m3u_tag_prim() {
  this.value = "";
}

function Kr_m3u_tag_alist() {
  this.name = "method";
  this.value = "";
}

function M3u_tag() {
  this.name = "extm3u";
  this.vtype = "primitive";
  this.primitive = [kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init(), kr_m3u_tag_prim_init()];
  this.attr_list = [kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init(), kr_m3u_tag_alist_init()];
  this.count = 0;
}

function M3u_el() {
  this.type = "uri";
  this.uri = "";
  this.tag = new M3u_tag();
}

function Kr_m3u() {
  this.elements = [m3u_el_init(), m3u_el_init()];
  this.el_count = 0;
}

function Kr_limit() {
  this.label = "";
  this.value = 0;
}

function Kr_limits() {
  this.limits = [kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init(), kr_limit_init()];
  this.len = 0;
}

function Kr_avconv_status() {
  this.input = "";
  this.output = "";
}

function Kr_avconv_params() {
  this.input = "";
  this.output = "";
}

function Kr_avconv() {
  this.input = "";
  this.output = "";
}

function Kr_avprobe_format() {
  this.filename = "";
  this.nb_streams = 0;
  this.format_name = "";
  this.format_long_name = "";
  this.start_time = "";
  this.duration = "";
  this.size = "";
  this.bit_rate = "";
}

function Kr_avprobe_stream() {
  this.index = 0;
  this.codec_name = "";
  this.codec_type = "audio";
  this.width = 0;
  this.height = 0;
  this.start_time = "";
  this.duration = "";
  this.channels = 0;
  this.sample_rate = "";
  this.display_aspect_ratio = "";
  this.pix_fmt = "";
}

function Kr_avprobe_media() {
  this.format = new Kr_avprobe_format();
  this.streams = [kr_avprobe_stream_init()];
}

function Kr_avprobe_params() {
  this.filename = "";
}

function Kr_avprobe_status() {
  this.media = new Kr_avprobe_media();
}

function Kr_avprobe() {
  this.params = new Kr_avprobe_params();
  this.status = new Kr_avprobe_status();
}

function Kr_curl_status() {
  this.url = "";
  this.dest = "";
  this.percent_complete = 0;
}

function Kr_curl_params() {
  this.url = "";
  this.dest = "";
  this.percent_complete = 0;
}

function Kr_curl() {
  this.url = "";
  this.dest = "";
  this.percent_complete = 0;
}

function Kr_livestreamer_params() {
  this.save_file = "";
  this.url = "";
}

function Kr_livestreamer_status() {
  this.save_file = "";
  this.url = "";
}

function Kr_livestreamer() {
  this.save_file = "";
  this.url = "";
}

function Kr_media_version() {
  this.url = "";
  this.id = "";
  this.container = "";
  this.video_encoding = "";
  this.audio_encoding = "";
  this.video_bitrate = 0.00;
  this.audio_bitrate = 0.00;
  this.video_height = 0.00;
  this.video_width = 0.00;
}

function Kr_quvi_media() {
  this.id = "";
  this.title = "";
  this.thumbnail_url = "";
  this.start_time_ms = 0.00;
  this.duration_ms = 0.00;
  this.versions = 0;
  this.version = [kr_media_version_init()];
}

function Kr_quvi_status() {
  this.media_info = new Kr_quvi_media();
}

function Kr_quvi_params() {
  this.url = "";
}

function Kr_quvi() {
  this.params = new Kr_quvi_params();
  this.status = new Kr_quvi_status();
}

function Kr_transmission_cli_status() {
  this.file = "";
}

function Kr_transmission_cli_params() {
  this.file = "";
}

function Kr_transmission_cli() {
  this.file = "";
}

function Kr_fail() {
  this.reason = "";
  this.details = "";
  this.subfault = "";
  this.should_retry = 0;
  this.attempt = 0;
}

function Kr_task_status() {
  this.state = "ready";
  this.create_time = new Kr_timestamp();
  this.start_time = new Kr_timestamp();
  this.stop_time = new Kr_timestamp();
  this.fail = new Kr_fail();
}

function Kr_task_params() {
  this.type = "valgrind_stressor";
  this.valgrind_stressor = new Kr_valgrind_stressor_params();
  this.quvi = new Kr_quvi_params();
  this.avconv = new Kr_avconv_params();
  this.xmms = new Kr_xmms_params();
  this.curl = new Kr_curl_params();
  this.transmission_cli = new Kr_transmission_cli_params();
  this.livestreamer = new Kr_livestreamer_params();
  this.avprobe = new Kr_avprobe_params();
  this.ydl = new Kr_ydl_params();
}

function Kr_task() {
  this.id = "";
  this.status = new Kr_task_status();
  this.params = new Kr_task_params();
  this.type = "valgrind_stressor";
  this.valgrind_stressor = new Kr_valgrind_stressor();
  this.quvi = new Kr_quvi();
  this.avconv = new Kr_avconv();
  this.xmms = new Kr_xmms();
  this.curl = new Kr_curl();
  this.transmission_cli = new Kr_transmission_cli();
  this.livestreamer = new Kr_livestreamer();
  this.avprobe = new Kr_avprobe();
  this.ydl = new Kr_ydl();
}

function Kr_valgrind_stressor_status() {
}

function Kr_valgrind_stressor_params() {
}

function Kr_valgrind_stressor() {
}

function Kr_xmms_params() {
  this.filename = "";
  this.env = "";
}

function Kr_xmms_status() {
  this.coconut = 0;
}

function Kr_xmms() {
  this.params = new Kr_xmms_params();
  this.status = new Kr_xmms_status();
}

function Kr_ydl_format() {
  this.url = "";
  this.ext = "";
  this.format_id = "";
  this.format = "";
  this.vcodec = "";
  this.width = 0;
  this.height = 0;
  this.abr = 0;
}

function Kr_ydl_video() {
  this.duration = 0;
  this.description = "";
  this.uploader_id = "";
  this.uploader = "";
  this.view_count = 0;
  this.title = "";
  this.ext = "";
  this.id = "";
  this.thumbnail = "";
  this.width = 0;
  this.height = 0;
  this.upload_date = "";
  this.formats = [kr_ydl_format_init()];
}

function Kr_ydl_params() {
  this.url = "";
}

function Kr_ydl_status() {
  this.video = new Kr_ydl_video();
}

function Kr_ydl() {
  this.params = new Kr_ydl_params();
  this.status = new Kr_ydl_status();
}

function Kr_timestamp() {
  this.seconds = 0;
  this.nanoseconds = 0;
}

function Kr_media_source() {
  this.path = "";
}

function Kr_media_packet() {
  this.type = "header";
  this.track = 0;
  this.pts = 0;
  this.dts = 0;
  this.buf = 0;
  this.sz = 0;
}

function Kr_track_name() {
  this.name = "";
  this.len = 0;
}

function Kr_mux_name() {
  this.name = "";
  this.len = 0;
}

function Kr_demux() {
  this.type = "raw";
  this.source = 0;
}

function Kr_mux() {
  this.input = [kr_track_name_init(), kr_track_name_init(), kr_track_name_init(), kr_track_name_init()];
  this.type = "raw";
  this.ntracks = 0;
}

function Kr_video_encode() {
  this.codec = "daala";
  this.daala = new Kr_daala_encoder();
  this.vpx = new Kr_vpx_encoder();
  this.h264 = new Kr_h264_encoder();
  this.theora = new Kr_theora_encoder();
  this.y4m = new Kr_y4m_encoder();
}

function Kr_audio_encode() {
  this.codec = "flac";
  this.flac = new Kr_flac();
  this.opus = new Kr_opus();
  this.vorbis = new Kr_vorbis();
  this.mp3 = new Kr_mp3();
  this.aac = new Kr_aac();
}

function Kr_encode() {
  this.in = new Kr_track_name();
  this.out = new Kr_track_name();
  this.type = "audio";
  this.audio = new Kr_audio_encode();
  this.video = new Kr_video_encode();
}

function Kr_record() {
  this.name = "";
  this.len = 0;
}

function Kr_transmission() {
  this.txmtr = new Kr_xfer_transmitter();
  this.mount = "";
}

function Kr_output() {
  this.input = new Kr_mux_name();
  this.type = "record";
  this.record = new Kr_record();
  this.uplink = new Kr_uplink();
  this.transmission = new Kr_transmission();
}

function Kr_capture_audio() {
  this.track = new Kr_track_name();
  this.channels = 2;
  this.total_frames = 0;
}

function Kr_capture_video() {
  this.track = new Kr_track_name();
  this.width = 0;
  this.height = 0;
  this.fps_num = 0;
  this.fps_den = 0;
  this.format = "argb";
  this.total_frames = 0;
}

function Kr_capture_image() {
  this.width = 0;
  this.height = 0;
  this.format = "argb";
}

function Kr_capture() {
  this.type = "audio";
  this.audio = new Kr_capture_audio();
  this.video = new Kr_capture_video();
  this.image = new Kr_capture_image();
}

function Kr_source_file() {
  this.path = "";
  this.size = 0;
  this.lastmod = 0;
}

function Kr_source_remote_file() {
  this.path = "";
}

function Kr_source_stream() {
  this.path = "";
  this.len = 0;
  this.type = "accept";
  this.num = 0;
  this.bytes = 0;
}

function Kr_studio_source() {
  this.type = "local_file";
  this.local_file = new Kr_source_file();
  this.remote_file = new Kr_source_remote_file();
  this.stream = new Kr_source_stream();
}

function Kr_decode() {
  this.bongo = 0;
}

function Kr_play() {
  this.input = new Kr_track_name();
}

function Kr_render_image() {
  this.id = "";
}

function Kr_render_video() {
  this.id = "";
}

function Kr_render_audio() {
  this.id = "";
}

function Kr_render() {
  this.type = "image";
  this.image = new Kr_render_image();
  this.video = new Kr_render_video();
  this.audio = new Kr_render_audio();
}

function Kr_generate_audio() {
  this.type = "noise";
  this.channels = 0;
}

function Kr_generate_color() {
  this.r = 0;
  this.g = 0;
  this.b = 0;
}

function Kr_generate_video() {
  this.type = "color";
  this.color = new Kr_generate_color();
  this.no_signal = 0;
  this.width = 0;
  this.height = 0;
  this.fps_num = 0;
  this.fps_den = 0;
  this.format = "argb";
}

function Kr_generate() {
  this.type = "audio";
  this.audio = new Kr_generate_audio();
  this.video = new Kr_generate_video();
}

function Kr_studio_path() {
  this.type = "source";
  this.source = new Kr_studio_source();
  this.generate = new Kr_generate();
  this.capture = new Kr_capture();
  this.render = new Kr_render();
  this.play = new Kr_play();
  this.encode = new Kr_encode();
  this.decode = new Kr_decode();
  this.demux = new Kr_demux();
  this.mux = new Kr_mux();
  this.output = new Kr_output();
}

function Kr_track() {
  this.track = new Kr_track_name();
  this.codec = new Kr_codec();
  this.mode = "finite";
  this.duration = 0;
  this.type = "coded_audio";
  this.coded_audio = new Kr_audio();
  this.coded_video = new Kr_video();
  this.mux = 0;
  this.audio = 0;
  this.video = 0;
}

function Kr_track_block() {
  this.tc = 0;
}

function Kr_hls_variant() {
  this.name = "";
  this.url = "";
  this.codecs = "";
  this.bitrate = 0;
  this.width = 0;
  this.height = 0;
  this.audio = 0;
  this.video = 0;
  this.subtitles = 0;
}

function Kr_hls_rendition() {
  this.name = "";
  this.group = "";
  this.url = "";
  this.type = "audio";
  this.def = 0;
}

function Kr_hls_master_playlist() {
  this.url = "";
  this.nrenditions = 0;
  this.nvariants = 0;
  this.rendition = [kr_hls_rendition_init(), kr_hls_rendition_init()];
  this.variant = [kr_hls_variant_init(), kr_hls_variant_init()];
}

function Kr_hls_media_segment() {
  this.url = "";
  this.sequence_num = 0;
  this.duration = 0.00;
}

function Kr_hls_media_playlist() {
  this.type = "vod";
  this.max_duration = 0;
  this.sequence_num = 0;
  this.nsegments = 0;
  this.segment = [kr_hls_media_segment_init(), kr_hls_media_segment_init()];
}

function Kr_hls_playlist() {
  this.type = "master";
  this.master = new Kr_hls_master_playlist();
  this.media = new Kr_hls_media_playlist();
}

function Kr_hls_tuner() {
}

function Kr_fps() {
  this.num = 30;
  this.den = 1;
}

function Kr_radio() {
  this.callsign = "";
  this.name = "";
  this.started_on = 0;
  this.mode = "audio";
  this.version = 0;
  this.commit = "";
}

function Kr_web_server() {
  this.type = "http";
  this.port = 0;
  this.addr = new Array(16);
  this.iface = "";
  this.conn_total = 0;
  this.created_on = 0;
}

function Kr_web_server_setup() {
  this.type = "http";
  this.port = 0;
  this.addr = new Array(16);
  this.iface = "";
}

function Kr_web_remote() {
  this.port = 0;
  this.addr = new Array(16);
  this.host = "";
  this.host_len = 0;
}

function Kr_web_socket_io_state() {
  this.frame = 0;
  this.sz = 0;
  this.pos = 0;
}

function Kr_web_socket_state() {
  this.type = "binary";
  this.proto = "krad";
  this.input = new Kr_web_socket_io_state();
  this.output = new Kr_web_socket_io_state();
}

function Kr_web_http_io_state() {
  this.state = "content";
  this.sz = 0;
  this.pos = 0;
  this.num = 0;
  this.chunked = 0;
  this.multipart = "init";
}

function Kr_web_http_state() {
  this.input = new Kr_web_http_io_state();
  this.output = new Kr_web_http_io_state();
}

function Kr_web_client() {
  this.type = "accept";
  this.state = "resolving";
  this.protocol = "http";
  this.http = new Kr_web_http_state();
  this.websocket = new Kr_web_socket_state();
  this.created_on = 0;
  this.bytes_in = 0;
  this.bytes_out = 0;
  this.port = 0;
  this.remote = new Kr_web_remote();
}

function Kr_web_client_setup() {
  this.protocol = "http";
  this.remote = new Kr_web_remote();
}

function Kr_web_path() {
  this.type = "server";
  this.server = new Kr_web_server();
  this.client = new Kr_web_client();
}

function Kr_web_path_setup() {
  this.type = "server";
  this.server = new Kr_web_server_setup();
  this.client = new Kr_web_client_setup();
}

function Kr_path() {
  this.name = "";
}

function Kr_path_list() {
  this.len = 0;
  this.path = [kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init(), kr_path_init()];
}

function Kr_upload_file() {
  this.url = "";
  this.url_len = 0;
  this.path = "";
  this.path_len = 0;
}

function Kr_upload_stash() {
  this.url = "";
  this.url_len = 0;
  this.id = "";
  this.id_len = 0;
}

function Kr_download_file() {
  this.url = "";
  this.url_len = 0;
  this.path = "";
  this.path_len = 0;
}

function Kr_download_stash() {
  this.url = "";
  this.url_len = 0;
  this.id = "";
  this.id_len = 0;
}

function Kr_upload() {
  this.type = "file";
  this.file = new Kr_upload_file();
  this.stash = new Kr_upload_stash();
}

function Kr_download() {
  this.type = "file";
  this.file = new Kr_download_file();
  this.stash = new Kr_download_stash();
}

function Kr_accept() {
  this.path = "";
  this.path_len = 0;
  this.pos = 0;
  this.len = 0;
}

function Kr_serve_range() {
  this.start = 0;
  this.end = 0;
}

function Kr_serve_ranges() {
  this.pos = 0;
  this.len = 0;
  this.sz = 0;
  this.nranges = 0;
  this.range = [kr_serve_range_init(), kr_serve_range_init(), kr_serve_range_init(), kr_serve_range_init()];
}

function Kr_serve_full() {
  this.pos = 0;
  this.len = 0;
}

function Kr_serve_index() {
  this.cnt = 0;
  this.format = "json";
}

function Kr_serve_redirect() {
  this.coconut = 0;
}

function Kr_serve() {
  this.type = "full";
  this.full = new Kr_serve_full();
  this.ranges = new Kr_serve_ranges();
  this.index = new Kr_serve_index();
  this.redirect = new Kr_serve_redirect();
}

function Kr_downlink() {
  this.url = "";
  this.len = 0;
}

function Kr_uplink() {
  this.url = "";
  this.len = 0;
  this.legacy = 0;
}

function Kr_source() {
  this.coconut = 0;
}

function Kr_transmit() {
  this.packed_header = 0;
}

function Kr_xfer_reconnect() {
  this.max_tries = 0;
  this.ms = 0;
}

function Kr_xfer_client() {
  this.rx = 0;
  this.tx = 0;
  this.type = "download";
  this.download = new Kr_download();
  this.upload = new Kr_upload();
  this.accept = new Kr_accept();
  this.serve = new Kr_serve();
  this.downlink = new Kr_downlink();
  this.uplink = new Kr_uplink();
  this.source = new Kr_source();
  this.transmit = new Kr_transmit();
  this.path = "";
  this.client = new Kr_web_client();
  this.reconnect = new Kr_xfer_reconnect();
}

function Kr_xfer_acceptor_dir() {
  this.path = "";
  this.len = 0;
}

function Kr_xfer_acceptor_file() {
  this.coconut = 0;
}

function Kr_xfer_acceptor_buf() {
  this.coconut = 0;
}

function Kr_xfer_share_dir() {
  this.path = "";
  this.len = 0;
}

function Kr_xfer_share_file() {
  this.coconut = 0;
}

function Kr_xfer_share_buf() {
  this.coconut = 0;
}

function Kr_xfer_acceptor() {
  this.type = "dir";
  this.dir = new Kr_xfer_acceptor_dir();
  this.file = new Kr_xfer_acceptor_file();
  this.buf = new Kr_xfer_acceptor_buf();
}

function Kr_xfer_receiver() {
  this.sources = 0;
}

function Kr_xfer_transmitter() {
  this.type = "http";
  this.clients = 0;
}

function Kr_xfer_share() {
  this.type = "dir";
  this.dir = new Kr_xfer_share_dir();
  this.file = new Kr_xfer_share_file();
  this.buf = new Kr_xfer_share_buf();
}

function Kr_xfer_server() {
  this.path = "";
  this.len = 0;
  this.type = "sharing";
  this.sharing = new Kr_xfer_share();
  this.accepting = new Kr_xfer_acceptor();
  this.transmitter = new Kr_xfer_transmitter();
  this.receiver = new Kr_xfer_receiver();
}

function Kr_xfer_path() {
  this.type = "server";
  this.server = new Kr_xfer_server();
  this.client = new Kr_xfer_client();
}

function Kr_xpdr_path() {
  this.type = "jack";
  this.jack = new Kr_jack();
  this.wayland = new Kr_wayland();
  this.v4l2 = new Kr_v4l2();
  this.decklink = new Kr_decklink();
  this.x11 = new Kr_x11();
  this.alsa = new Kr_alsa();
  this.aux = new Kr_aux();
  this.jack_in = new Kr_jack_path();
  this.jack_out = new Kr_jack_path();
  this.wayland_out = new Kr_wayland_path();
  this.v4l2_in = new Kr_v4l2_path();
  this.decklink_video_in = new Kr_decklink_path();
  this.decklink_audio_in = new Kr_decklink_path();
  this.x11_in = new Kr_x11_path();
  this.alsa_in = new Kr_alsa_path();
  this.alsa_out = new Kr_alsa_path();
  this.aux_video_in = new Kr_aux_path();
  this.aux_video_out = new Kr_aux_path();
  this.aux_audio_in = new Kr_aux_path();
  this.aux_audio_out = new Kr_aux_path();
}

