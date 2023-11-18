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
