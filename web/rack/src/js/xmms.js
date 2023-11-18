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
