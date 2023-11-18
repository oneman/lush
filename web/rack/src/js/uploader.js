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
