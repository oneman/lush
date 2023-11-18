dualmodes = {
  Horz: 0x00,
  Vert: 0x01
};

trimodes = {
  Vert: 0,
  Top: 1,
  Bottom: 2,
  Left: 3,
  Right: 4
};

function Tilemate(div) {
  this.container = div;
  this.lists = [];
  this.dualmode = dualmodes.Horz;
  this.trimode = trimodes.Vert;
  this.layout = document.createElement("div");
  this.layout.classList.add("tile_layout");
  this.layout.innerHTML = "";
  this.container.appendChild(this.layout);
}

Tilemate.prototype.set_dualmode = function (dualmode) {
  this.dualmode = dualmode;
  this.place(this.lists);
};

Tilemate.prototype.set_trimode = function (trimode) {
  this.trimode = trimode;
  this.place(this.lists);
};

Tilemate.prototype.place = function (lists) {
  var i;
  var subsplit;
  var split;
  var num = lists.length;
  this.lists = lists;
  this.layout.innerHTML = "";
  if (num < 1) {
    return;
  }
  if (num == 1) {
    var full = document.createElement("div");
    full.classList.add("tile");
    full.appendChild(lists[0]);
    this.layout.appendChild(full);
    return;
  }
  if (num == 2) {
    split = document.createElement("div");
    if (this.dualmode == dualmodes.Horz) {
      split.classList.add("column");
    } else if (this.dualmode == dualmodes.Vert) {
      split.classList.add("row");
    }

    for (i = 0; i < num; i++) {
      var half = document.createElement("div");
      half.classList.add("tile");
      half.appendChild(lists[i]);
      split.appendChild(half);
    }
    this.layout.appendChild(split);
    return;
  }
  if (num >= 3) {
    split = document.createElement("div");
    if (this.trimode === trimodes.Vert) {
      split.classList.add("row");
      for (i = 0; i < 3; i++) {
        var third = document.createElement("div");
        third.classList.add("tile");
        third.appendChild(lists[i]);
        split.appendChild(third);
      }
      this.layout.appendChild(split);
      return;
    } else if (this.trimode === trimodes.Top || this.trimode === trimodes.Bottom) {
      split.classList.add("column");
      subsplit = document.createElement("div");
      subsplit.classList.add("row");
    } else if (this.trimode === trimodes.Left || this.trimode === trimodes.Right) {
      split.classList.add("row");
      subsplit = document.createElement("div");
      subsplit.classList.add("column");
    }

    var bigpart = document.createElement("div");
    bigpart.classList.add("tile");
    bigpart.appendChild(lists[0]);

    for (i = 1; i < 3; i++) {
      subpart = document.createElement("div");
      subpart.classList.add("tile");
      subpart.appendChild(lists[i]);
      subsplit.appendChild(subpart);
    }

    if (this.trimode === trimodes.Top || this.trimode === trimodes.Left) {
      split.appendChild(bigpart);
      split.appendChild(subsplit);
    }
    if (this.trimode === trimodes.Bottom || this.trimode === trimodes.Right) {
      split.appendChild(subsplit);
      split.appendChild(bigpart);
    }

    this.layout.appendChild(split);
    return;
  }
};
