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


