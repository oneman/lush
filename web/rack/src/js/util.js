function D(id) {
  return document.getElementById(id);
}

function C(element) {
  return document.getElementById(element).innerHTML;
}

function A(element, content) {
  var e;
  e = document.getElementById(element);
  e.insertAdjacentHTML("beforeend", content);
}

function R(id, content) {
  var e;
  e = document.getElementById(id);
  e.innerHTML = content;
}

function T(id, val) {
  var e;
  e = document.getElementById(id);
  e.value = val;
}


