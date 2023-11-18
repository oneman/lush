$(document).ready(function() {
  rack_units.push({'constructor': Pager});
});

function Pager(info_object) {
  this.title = "Pager";
  this.description = "Multiple rack pages, sir";
  this.div_text = "<div id='pager' class='RCU pager'></div>";
  this.id = 'pager';
  this.sel = "#pager";
  info_object['parent_div'].append(this.div_text);
  this.resizeTimer;
  this.pager_elem = document.getElementById('pager');
  this.pager_tabs = {};
}

Pager.prototype.create_span = function(name) {
  var span_elem = document.createElement('span');
  span_elem.classList.add('page');
  span_elem.id = name;
  span_elem.innerHTML = name;
  span_elem.addEventListener('click', create_handler(this,
        this.on_pager_click));
  return span_elem;
}

Pager.prototype.page_change = function(pageid) {
  this.select_page(pageid);
}

Pager.prototype.on_modechange = function(mode) {
  while (this.pager_elem.firstChild) {
    this.pager_elem.removeChild(this.pager_elem.firstChild);
  }
  var page_list = kr.ux.get_page_names();
  for (var i = 0; i < page_list.length; i++) {
    var span_elem = this.create_span(page_list[i]);
    this.pager_elem.appendChild(span_elem);
  }
}

Pager.prototype.select_page = function(id) {
  var target = document.getElementById(id);
  if (target === null) {
    setTimeout(create_handler(this, this.select_page), 5, id);
    return;
  }
  var matches = this.pager_elem.querySelectorAll('.selected');
  target.classList.add("selected");
  for (var i = 0; i < matches.length; i++) {
    if (target !== matches[i]) {
      matches[i].classList.remove("selected");
    }
  }
}

Pager.prototype.on_pager_click = function(evt) {
  var target = evt.target;
  while (!target.classList.contains("page")) {
    if (target.parentElement) {
      target = target.parentElement;
    } else {
      target = null;
      break;
    }
  }
  if (target) {
    kr.ux.switch_pages(target.id);
  }
}

Pager.prototype.window_resize = function() {
  requestAnimFrame(create_handler(this, this.page_resize));
};

Pager.prototype.page_resize = function() {
  $('.page').css('font-size', 0.6*$('#pager').height()+'px');
}

Pager.prototype.update = function(crate) {
}

Pager.prototype.shared = function(key, shared_object) {
}


