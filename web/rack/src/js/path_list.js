$(document).ready(function() {
  rack_units.push({ 'constructor': PathList, 'aspect': { audio: [14, 9],
  dev: [16, 13]}, 'page': { dev: 'create', audio: 'system' },
  position: { dev: [0, 1], audio: [18, 6] }});
});

function PathList(info_object) {
  this.title = "List";
  this.id = "path_list";
  this.description = "List of kr paths.";
  this.aspect_ratio = [16,9];
  this.list_items = {};
  this.div_text = "<div id='path_list' class='RCU"
   + " path_list'></div>";
  this.sel = "#path_list";
  this.paths = {};
  info_object['parent_div'].append(this.div_text);
  $(this.sel).append("<div class='darkblue overflow_auto half'>"
   + "<div id='pl_list' class='kr_list play'></div></div>"
   + "<div id='pl_details' class='darkblue overflow_auto half'></div>");
  this.selectable_list = new Selectable(document.getElementById('pl_list'));
}

PathList.prototype.update_path_details = function(id) {
  var result = $("#pl_details").empty();
  if (id != undefined) {
    result.append('<div class="title">' + id + '</div><pre>'
     + JSON.stringify(this.paths[id].info, null, 2)
     + '</pre>');
  }
}

PathList.prototype.update = function(crate) {
  if (crate.data != undefined && crate.data.path) {
    for (var i=0; i < crate.data.len; i++) {
      var name = crate.data.path[i].name;
      kr.get(name);
    }
  } else {
    var path = crate.path;
    if (crate.meth == "DELETE") {
      if (this.paths[path] != undefined) {
        if (this.selected_path == path) {
          this.update_path_details();
          this.selected_path = null;
        }
        var i = this.paths[path].item_elem;
        i.parentElement.removeChild(i);
        delete this.paths[path];
      }
    } else if (crate.meth == "PATCH") {
      if (this.paths[path] != undefined) {
        jsonpatch.apply(this.paths[path].info, crate.data);
        if (this.selected_path == path) {
          this.update_path_details(path);
        }
      }
    } else if (crate.meth == "PUT") {
      kr.debug(crate);
      var del_button = 1;
      this.paths[crate.path] = new PathItem(this, crate);
    }
  }
}

PathList.prototype.shared = function(key, shared_object) {
}

function PathItem(pathList, crate) {
  this.pl = pathList;
  this.path = crate.path;
  this.info = crate.data;
  this.item_elem = document.createElement('div');
  this.item_elem.id = crate.path;
  var label_elem = document.createElement('div');
  label_elem.className = "label";
  label_elem.innerHTML = crate.path;
  this.item_elem.appendChild(label_elem);
  var button_elem = document.createElement('div');
  button_elem.className = "kr_push_button red";
  button_elem.innerHTML = "<em>Remove</em>";
  button_elem.addEventListener('click', create_handler(this,
   this.on_delete_click));
  this.item_elem.appendChild(button_elem);
  this.pl.selectable_list.add(this, this.selectable_list_selected,
   this.item_elem);
}

PathItem.prototype.on_delete_click = function(evt) {
  evt.stopPropagation();
  evt.preventDefault();
  kr.rm(this.path);
}

PathItem.prototype.selectable_list_selected = function(evt) {
  this.pl.update_path_details(this.path);
  this.pl.selected_path = this.path;
}
