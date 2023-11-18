
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
