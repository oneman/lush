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
