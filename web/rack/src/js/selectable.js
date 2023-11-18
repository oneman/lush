function Selectable(list_elem) {
  this.list_elem = list_elem;
  this.list_elem.className += ' selectable';
}

Selectable.prototype.add = function(obj, func, item_elem) {
  item_elem.className += ' selectee';
  this.list_elem.appendChild(item_elem);
  item_elem.addEventListener('click', create_handler(obj, func));
  item_elem.addEventListener('click', create_handler(this, this.on_click));
}

Selectable.prototype.remove = function(item_elem) {
  this.list_elem.removeChild(item_elem);
}

Selectable.prototype.on_click = function(evt) {
  if (!evt.defaultPrevented) {
    var target = evt.target;
    while (!target.classList.contains("selectee")) {
      if (target.parentElement) {
        target = target.parentElement;
      } else {
        target = null;
        break;
      }
    }
    if (target) {
      var matches = this.list_elem.querySelectorAll('.selected');
      target.classList.add("selected");
      for (var i = 0; i < matches.length; i++) {
        if (target !== matches[i]) {
          matches[i].classList.remove("selected");
        }
      }
    }
  }
}
