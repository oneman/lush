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
