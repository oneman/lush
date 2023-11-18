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


