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
