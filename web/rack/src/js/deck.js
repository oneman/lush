function zpad(s) {
    if (s > 9) return s;
    return '0' + s;
}

function spad(s) {
    if (s > 9) return s;
    return ' ' + s;
}

function ms_to_padtime(ms) {
    var milisecs = ms % 1000;
    var s = (ms - milisecs) / 1000;
    var secs = s % 60;
    s = (s - secs) / 60;
    var mins = s % 60;
    var hours = (s - mins) / 60;
    if (hours) {
        return spad(hours) + ':' + zpad(mins) + ':' + zpad(secs);
    } else {
        return "   " + spad(mins) + ':' + zpad(secs);
    }
}

function local_mouse(elem, evt) {
    var rect = elem.getBoundingClientRect();
    var x = evt.pageX - rect.left - window.pageXOffset;
    var y = evt.pageY - rect.top - window.pageYOffset;
    return {
        x: x,
        y: y
    };
}

function Rect(x, y, w, h, cb) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.cb = cb;
}

Rect.prototype.hit = function (x, y) {
    if (this.x <= x && this.x + this.w >= x && this.y <= y && this.y + this.h >= y) {
        return true;
    } else {
        return false;
    }
};

function Control(rect, cb) {
    this.rect = rect;
    this.cb = cb;

}

PlaybackControl.prototype.push_prev = function () {
    console.log("prev! " + this.w);
    this.prev_pushed = 1;
    this.client.playlist.set_next_rel(-1).onvalue = create_handler(this, this.tickle_me_l8r);  
};

PlaybackControl.prototype.prev_button = function (c, x, y, label) {
    var w = 84;
    var t;
    var tx;
    var ty;
    c.shadowColor = "#444";
    c.shadowOffsetY = 2;
    c.shadowOffsetX = 1;
    c.shadowBlur = this.shadowBlur;
    if (this.prev_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
        this.prev_pushed++;
        this.damage++;
        if (this.prev_pushed > 2) this.prev_pushed = 0;
    }
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fillStyle = '#444';
    c.fill();
    c.strokeStyle = '#422';
    //c.lineWidth = 3.5;
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    c.shadowBlur = this.shadowBlur;
    if (this.prev_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
    }
    c.stroke();
    c.closePath();
    c.fillStyle = '#2fa';
    c.shadowColor = "#4fa";
    c.shadowBlur = this.shadowBlur;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.save();
    t = 9;
    c.beginPath();
    tx = x + 25;
    ty = y + 24;
    c.translate(tx, ty);
    c.translate(t * 4, t * 2);
    c.rotate(135 * (Math.PI * 1));
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    t = 9;
    c.beginPath();
    tx = tx + t * 2;
    ty = ty + t;
    c.translate(t * 2, 0);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    c.translate(t * 2, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.push_prev)));
};

PlaybackControl.prototype.tickle_me_l8r = function () {
  this.client.playback.tickle();
};

PlaybackControl.prototype.push_next = function () {
    console.log("next! " + this.w);
    this.next_pushed = 1;
    this.client.playlist.set_next_rel(1).onvalue = create_handler(this, this.tickle_me_l8r);  
};

PlaybackControl.prototype.next_button = function (c, x, y, label) {
    var w = 84;
    var t;
    var tx;
    var ty;
    c.shadowColor = "#444";
    c.shadowOffsetY = 2;
    c.shadowOffsetX = 1;
    c.shadowBlur = this.shadowBlur;
    if (this.next_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
        this.next_pushed++;
        this.damage++;
        if (this.next_pushed > 2) this.next_pushed = 0;
    }
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fillStyle = '#444';
    c.fill();
    c.strokeStyle = '#422';
    //c.lineWidth = 3.5;
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    c.shadowBlur = this.shadowBlur;
    if (this.next_pushed > 0) {
        c.shadowColor = "#444";
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.shadowBlur = 0;
    }
    c.stroke();
    c.closePath();
    c.fillStyle = '#2fa';
    c.shadowColor = "#4fa";
    c.shadowBlur = this.shadowBlur;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.save();
    t = 9;
    c.beginPath();
    tx = x + 25;
    ty = y + 24;
    c.translate(tx, ty);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    t = 9;
    c.beginPath();
    tx = tx + t * 2;
    ty = ty + t;
    c.translate(t * 2, 0);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    c.translate(t * 2, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.push_next)));
};

PlaybackControl.prototype.play_pushed = function () {
    console.log("play! " + this.w);
    if (this.playback_state == 1) {
      /*this.playback_state = 2;*/
      this.client.playback.pause();
    } else {
      /*this.playback_state = 1;*/
      this.client.playback.start();
    }
};

PlaybackControl.prototype.play_button = function (c, x, y, label) {
    var w = 84;
    var t;
    var tx;
    var ty;
    c.shadowColor = "#444";
    c.shadowOffsetY = 2;
    c.shadowOffsetX = 1;
    c.shadowBlur = this.shadowBlur;
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fillStyle = '#152';
    c.strokeStyle = '#422';
    if (this.playback_state == 1) {
        c.fillStyle = '#1f1';
        c.strokeStyle = '#1a2';
        c.shadowColor = "#af6";
        c.shadowBlur = 1;
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
    }
    if (this.playback_state == 2) {
        c.fillStyle = '#2b6';

        c.strokeStyle = 'yellow';
        c.shadowColor = "yellow";
        c.shadowBlur = 6;
    }
    c.fill();
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    //c.shadowBlur = this.shadowBlur;
    c.stroke();
    c.closePath();
    c.fillStyle = '#2fa';
    c.shadowColor = "#4fa";
    c.shadowBlur = 0;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.save();
    if (this.playback_state == 2) {
        c.fillStyle = 'yellow';

        c.strokeStyle = 'yellow';
        c.shadowColor = "yellow";
        c.shadowBlur = 0 + (this.nframes % 40) / 5;
    }
    t = 12;
    c.beginPath();
    tx = x + 18;
    ty = y + 22;
    c.translate(tx, ty);
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.lineTo(t * 2, t);
    c.fill();
    //c.fill();
    c.translate(t * 2.2, 0);
    c.save();
    c.lineWidth = 1.5;
    c.beginPath();
    c.strokeStyle = '#ddd';
    if (this.playback_state == 2) {
        c.strokeStyle = 'yellow';
    }
    c.moveTo(t / 2, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    c.lineWidth = 5;
    c.translate(t * 1.2, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    if (this.playback_state == 2) {
        c.strokeStyle = 'yellow';
    }
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.translate(t / 1.8, 0);
    c.beginPath();
    c.strokeStyle = '#ddd';
    if (this.playback_state == 2) {
        c.strokeStyle = 'yellow';
    }
    c.moveTo(0, 0);
    c.lineTo(0, (t * 2));
    c.stroke();
    c.restore();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.play_pushed)));
};

PlaybackControl.prototype.stop_pushed = function () {
    console.log("stop! " + this.w);
    this.client.playback.stop();
    /*this.playback_state = 0;*/
};

PlaybackControl.prototype.stop_button = function (c, x, y, label) {
    var w = 84;
    var shadow = this.shadowblur;
    c.strokeStyle = '#422';
    c.fillStyle = '#c10000';
    c.shadowColor = "#d11";

    c.shadowColor = "#666";
    c.shadowOffsetY = 1;
    c.shadowOffsetX = 1;
    c.shadowBlur = 4;


    if (this.playback_state === 0) {
        c.shadowOffsetY = 0;
        c.shadowOffsetX = 0;
        c.strokeStyle = '#900';
        c.fillStyle = '#f11';
        c.shadowColor = "#a44";
        shadow = 0;
    }
    c.shadowBlur = shadow;
    c.beginPath();
    c.rect(x, y, 84, 64);
    c.fill();
    c.shadowOffsetY = 0;
    c.shadowOffsetX = 0;
    c.stroke();
    c.closePath();
    c.fillStyle = '#222';
    c.shadowColor = "#4f0";
    c.shadowBlur = shadow;
    c.closePath();
    c.beginPath();
    c.fillStyle = '#ddd';
    c.shadowColor = "#ddd";
    c.rect(x + 30, y + 22, 22, 22);
    c.fill();
    this.controls.push(new Control(new Rect(x, y, 84, 64), create_handler(this, this.stop_pushed)));
};


PlaybackControl.prototype.divider = function (c) {

    c.save();


    if (this.playback_state === 0) {
        c.strokeStyle = '#c43';
    } else if (this.playback_state === 1) {
        c.strokeStyle = '#4f2';
    } else if (this.playback_state === 2) {
        c.strokeStyle = 'yellow';
    }


    c.shadowColor = "#eee";
    c.shadowOffsetY = 1;
    c.shadowOffsetX = 1;
    c.shadowBlur = 6;


    c.beginPath();
    c.moveTo(0, 70);
    c.lineTo(440, 70);
    c.lineWidth = 2;
    c.stroke();
    c.closePath();
    c.beginPath();
    c.moveTo(0, 120);
    c.lineTo(440, 120);
    c.lineWidth = 2;
    c.stroke();
    c.closePath();
    c.restore();
};

PlaybackControl.prototype.buttons = function (c) {
    var x = 32;
    var y = 142;
    var m = 100;
    c.save();
    c.font = "Bold 24px Sans";
    c.textBaseline = "middle";
    c.textAlign = "center";
    c.lineWidth = 2;
    this.shadowBlur = 5;
    this.prev_button(c, x, y, "PREV");
    x += m;
    this.next_button(c, x, y, "NEXT");
    x += m;
    this.play_button(c, x, y, "PLAY");
    x += m;
    this.stop_button(c, x, y, "STOP");
    c.restore();
};

PlaybackControl.prototype.render_playtime = function (c) {
    var tc = ms_to_padtime(this.playtime);
    c.fillStyle = '#eee';
    c.font = "Bold 32px Monospace";
    //c.textBaseline = "middle";
    c.textAlign = "left";
    c.fillText(tc, 32, 106);
};

PlaybackControl.prototype.tracktime = function (c) {
    var tc;
    if (this.track.duration === undefined) {
      tc = "Infinite";
    } else { 
      tc = ms_to_padtime(this.track.duration);
    }
    c.fillStyle = '#ddd';
    c.font = "Bold 32px Monospace";
    //c.textBaseline = "middle";
    c.textAlign = "left";
    c.fillText(tc, 256, 106);
};

PlaybackControl.prototype.render_track_info = function (c) {
    var artist;
    var title;
    if (this.track.artist === undefined) {
      artist = "No Data";
    } else { 
      artist = this.track.artist;
    }
    if (this.track.title === undefined) {
      title = "";
    } else { 
      title = this.track.title;
    }
    c.fillStyle = '#eee';
    c.font = "Bold 16px Sans";
    c.textBaseline = "middle";
    c.textAlign = "left";
    c.fillText(title, 32, 24);
    c.font = "Bold 24px Sans";
    c.fillText(artist, 32, 50);
};

PlaybackControl.prototype.render = function (c) {
    if (this.scale) {
        c.save();
        c.scale(window.devicePixelRatio, window.devicePixelRatio);
    }
    this.buttons(c);
    this.render_playtime(c);
    this.tracktime(c);
    this.divider(c);
    this.render_track_info(c);
    if (this.scale) c.restore();
};

PlaybackControl.prototype.push = function (e) {
    var pos = local_mouse(this.surface, e);
    if (!this.scale) {
        pos.x = Math.floor(pos.x * window.devicePixelRatio);
        pos.y = Math.floor(pos.y * window.devicePixelRatio);
    }
    for (var i = 0; i < this.controls.length; i++) {
        var control = this.controls[i];
        if (control.rect.hit(pos.x, pos.y)) {
            control.cb();
            this.damage = 1;
            break;
        }
    }
};

/*function create_handler(inst, func) {
    return (function (e) {
        func.call(inst, e);
    });
}*/

PlaybackControl.prototype.debug = function (c) {
    c.beginPath();
    c.fillStyle = '#fff';
    c.font = "14px Sans";
    c.fillText("Frame# " + this.nframes, this.w - this.w / 3, this.h - 14);
    c.closePath();
};

PlaybackControl.prototype.clear = function (c) {
    c.beginPath();
    c.rect(0, 0, this.w, this.h);
    c.fillStyle = '#999';
    c.fill();
    c.closePath();
};

PlaybackControl.prototype.tick = function () {
    this.now = new Date();
    this.hour = this.now.getHours();
    this.minute = this.now.getMinutes();
    this.second = this.now.getSeconds();
    this.ms = this.now.getMilliseconds();
};

PlaybackControl.prototype.update = function () {
    if (this.resized) {
        this.update_dimensions();
    }
    if (this.damage) {
        var c = this.surface.getContext("2d");
        this.clear(c);
        this.tick();
        this.render(c);
        /*this.debug(c);*/
        this.nframes = this.nframes + 1;
        this.damage--;
        if (this.damage < 0) this.damage = 0;
    }
    window.requestAnimationFrame(this.loop);
};

PlaybackControl.prototype.update_dimensions = function () {
    var dpr = window.devicePixelRatio;
    var rect = this.container.getBoundingClientRect();
    var w = Math.floor(rect.width);
    var h = Math.floor(rect.height);
    this.surface.style.width = w + "px";
    this.surface.style.height = h + "px";
    this.w = Math.floor(w * dpr);
    this.h = Math.floor(h * dpr);
    this.surface.width = this.w;
    this.surface.height = this.h;
    this.resized = 0;
};

PlaybackControl.prototype.handle_resize = function () {
    this.resized = 1;
    this.damage = 1;
};

PlaybackControl.prototype.setup = function (container) {
    this.controls = [];
    this.nframes = 0;
    this.scale = 1;
    this.container = container;
    this.surface = document.createElement("canvas");
    this.surface.id = "playbackcontrol_" + this.container.id;
    this.container.appendChild(this.surface);
    this.update_dimensions();
    this.surface.onmousedown = create_handler(this, this.push);
    window.onresize = create_handler(this, this.handle_resize);
    this.loop = create_handler(this, this.update);
    this.damage = 1;
    window.requestAnimationFrame(this.loop);
};

PlaybackControl.prototype.set_playback_state = function (state) {
    this.playback_state = state;
    this.damage = 1;
};

PlaybackControl.prototype.set_track_info = function (info) {
    this.track = info;
    this.damage = 1;
};

PlaybackControl.prototype.set_playtime = function (playtime) {
    this.playtime = playtime;
    this.damage = 1;
};

function PlaybackControl(div, client) {
    this.client = client;
    this.setup(div);
    this.playback_state = 0;
    this.next_pushed = 0;
    this.prev_pushed = 0;
    this.playtime = 0;
    this.track = {};
}
