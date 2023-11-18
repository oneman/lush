/*$(document).ready(function() {
  rack_units.push({'constructor': KRStats, 'aspect': { dev: [2,1],
   audio: null }, 'page': {dev: 'all', audio: null},
   'position': {dev: [0, 0], audio: null}});
});*/

function KRStats(info_object) {
  this.title = "KRStats";
  this.description = "Display performance statistics.";
  this.id = "stats";
  this.div = "<div class='RCU' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  info_object['parent_div'].append(this.div);
  this.stats = new Stats();
  this.stats.setMode(0);
  this.stats.domElement.style.position = 'absolute';
  this.stats.domElement.style.left = '0px';
  this.stats.domElement.style.top = '0px';
  this.stats.domElement.style.width = '100%';
  this.stats.domElement.style.height = '100%';
  this.container = document.getElementById(this.id);
  this.container.appendChild(this.stats.domElement);
  var stats = this.stats;
  (function animloop() {
    stats.begin();
    requestAnimFrame(animloop);
    stats.end();
  })();
}

KRStats.prototype.update = function() {
}

KRStats.prototype.share = function() {
}

