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
