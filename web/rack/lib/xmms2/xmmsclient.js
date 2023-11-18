(function() {
  var ActivePlaylist, Bindata, Client, Collection, CollectionChanged, CollectionNamespace, Error, MediainfoReaderStatus, MedialibEntryStatus, Message, PlaybackStatus, PlaylistChange, PluginType, PropDict, Result, Seek, Util, Value, net, url, xmmsclient,
    extend = function(child, parent) { for (var key in parent) { if (hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    hasProp = {}.hasOwnProperty;

  Message = (function() {
    Message.check_int = function(val) {
      if (typeof val !== "number") {
        throw new TypeError("Invalid argument, expected number");
      }
      if (val > 0x7fffffff || val < -0x80000000) {
        throw new TypeError("Invalid argument, number is out of range");
      }
      return val;
    };

    Message.check_binary = function(val) {
      if (!(val instanceof Bindata)) {
        throw new TypeError("Invalid argument, expected bindata");
      }
      return val;
    };

    Message.check_string = function(val) {
      if (!(val && val.toString)) {
        throw new TypeError("Invalid argument, expected string");
      }
      return val.toString();
    };

    Message.check_list = function(val, subtype) {
      var j, len, value;
      if (!Array.isArray(val)) {
        throw new TypeError("Invalid argument, expected list");
      }
      for (j = 0, len = val.length; j < len; j++) {
        value = val[j];
        if (typeof value !== subtype) {
          throw new TypeError("Invalid value in list, expected " + subtype);
        }
      }
      return val;
    };

    Message.check_dictionary = function(val, subtype) {
      var key, value;
      if (typeof val !== "object") {
        throw new TypeError("Invalid argument, expected dictionary");
      }
      for (key in val) {
        value = val[key];
        if (subtype) {
          if (typeof value !== subtype) {
            throw new TypeError("Invalid value in dict, expected " + subtype);
          }
        }
      }
      return val;
    };

    Message.check_collection = function(val) {
      if (!(val instanceof Collection)) {
        throw new TypeError("Invalid argument, expected collection");
      }
      return val;
    };

    function Message() {
      this.object_id = null;
      this.command_id = null;
      this.cookie = null;
      this.payload = null;
      this.payload_length = 0;
      this.args = [];
    }

    Message.prototype.assemble = function(cookie) {
      var header, payload;
      payload = new Bitbuffer();
      Value.serialize(payload, this.args);
      header = new Bitbuffer();
      header.write_int32(this.object_id);
      header.write_int32(this.command_id);
      header.write_int32(cookie);
      header.write_int32(payload.offset);
      var payload_actual = payload.buf.slice(0, payload.offset);
      header.buf.set(payload_actual, header.offset);
      header.offset += payload.offset;
      return header.buf.slice(0, header.offset);
    };

    return Message;

  })();

  var vos = 0;

  Value = (function() {
    function Value() {}

    Value.Type = {
      None: 0x00,
      Error: 0x01,
      Integer: 0x02,
      String: 0x03,
      Collection: 0x04,
      Binary: 0x05,
      List: 0x06,
      Dictionary: 0x07
    };

    Value.serialize_bin = function(bindata, val) {
      bindata.write_int32(val.data.length);
      return bindata.write_data(val.data);
    };

    Value.serialize_coll = function(bindata, val) {
      bindata.write_int32(val.type);
      this.serialize_dict(bindata, val.attributes);
      this.serialize_list(bindata, val.idlist, this.Type.Integer);
      if (val.type === val.Type.Reference) {
        return this.serialize_list(bindata, [], this.Type.Collection);
      } else {
        return this.serialize_list(bindata, val.operands, this.Type.Collection);
      }
    };

    Value.serialize_list = function(bindata, val, type) {
      var i, j, len, results, value;
      if (type == null) {
        type = this.Type.None;
      }
      bindata.write_int32(type);
      bindata.write_int32(val.length);
      results = [];
      for (i = j = 0, len = val.length; j < len; i = ++j) {
        value = val[i];
        if (type === this.Type.None) {
          results.push(this.serialize(bindata, value));
        } else {
          results.push(this.serialize_value(bindata, value));
        }
      }
      return results;
    };

    Value.serialize_dict = function(bindata, val) {
      var key, length, results, value;
      length = 0;
      for (key in val) {
        length += 1;
      }
      bindata.write_int32(length);
      results = [];
      for (key in val) {
        value = val[key];
        bindata.write_string(key);
        results.push(this.serialize(bindata, value));
      }
      return results;
    };

    Value.serialize = function(bindata, val) {
      this.serialize_type(bindata, val);
      return this.serialize_value(bindata, val);
    };

    Value.serialize_type = function(bindata, val) {
      switch (typeof val) {
        case "string":
          return bindata.write_int32(this.Type.String);
        case "number":
          return bindata.write_int32(this.Type.Integer);
        case "object":
          if (Array.isArray(val)) {
            return bindata.write_int32(this.Type.List);
          //} else if (val instanceof Bindata) {
          //  return bindata.write_int32(this.Type.Binary);
          } else if (val instanceof Collection) {
            return bindata.write_int32(this.Type.Collection);
          } else {
            return bindata.write_int32(this.Type.Dictionary);
          }
      }
    };

    Value.serialize_value = function(bindata, val) {
      switch (typeof val) {
        case "string":
          return bindata.write_string(val);
        case "number":
          //console.log("number: " + val);
          return bindata.write_int64(val);
        case "object":
          if (Array.isArray(val)) {
            return this.serialize_list(bindata, val);
          //} else if (val instanceof Bindata) {
          //  return this.serialize_bin(bindata, val);
          } else if (val instanceof Collection) {
            return this.serialize_coll(bindata, val);
          } else {
            return this.serialize_dict(bindata, val);
          }
      }
    };

    Value.deserialize_bin = function(bindata) {
      var subbin;
      var length;
      var blob;
      var url;
      length = this.deserialize_i32(bindata);
      subbin = bindata.buffer.slice(vos, vos + length);
      vos += length;
      blob = new Blob([subbin], {type: 'application/octet-binary'});
      url = URL.createObjectURL(blob);
      return url;
    };


    Value.deserialize_coll = function(bindata) {
      var coll, type;
      type = this.deserialize_i32(bindata);
      coll = new Collection(type);
      coll.attributes = this.deserialize_dict(bindata);
      coll.idlist = this.deserialize_list(bindata);
      coll.operands = this.deserialize_list(bindata);
      return coll;
    };

    Value.deserialize_list = function(bindata) {
      var i, j, length, list, ref1, type;
      type = this.deserialize_i32(bindata);
      length = this.deserialize_i32(bindata);
      list = [];
      for (i = j = 0, ref1 = length; 0 <= ref1 ? j < ref1 : j > ref1; i = 0 <= ref1 ? ++j : --j) {
        if (type === this.Type.None) {
          list[i] = this.deserialize(bindata);
        } else {
          list[i] = this.deserialize_value(bindata, type);
        }
      }
      return list;
    };

    Value.deserialize_string = function(bindata) {
      var len;
      len = this.deserialize_i32(bindata);
      key = "";
      ulen = 1;
      if (len > 1) {
        ulen = len - 1;
      } else {
        vos += 1;
        return "FAIL";
      }
      res = new StringView(bindata.buffer, "UTF-8", vos, len - 1);
      blen = res.rawData.byteLength;
      key = res.toString();
      vos += len;
      if (typeof(key) !== 'string') {
        s = new String(key);
        return s;
      }
      return key;
    }

    Value.deserialize_i32 = function(bindata) {
      var num;
      var num = bindata.byteLength > 0 ? bindata.getInt32(vos) : 0;
      vos += 4;
      return num;
    }

    Value.deserialize_dict = function(bindata) {
      var dict, i, j, key, length, ref1, value;
      dict = {};
      length = this.deserialize_i32(bindata);
      for (i = j = 0, ref1 = length; 0 <= ref1 ? j < ref1 : j > ref1; i = 0 <= ref1 ? ++j : --j) {
        key = this.deserialize_string(bindata);
        value = this.deserialize(bindata);
        dict[key] = value;
      }
      return dict;
    };

    Value.deserialize_value = function(bindata, type) {
      switch (type) {
        case Value.Type.None:
          return null;
        case Value.Type.Error:
          str = this.deserialize_string(bindata);
          return new Error(str);
        case Value.Type.Integer:
          os = vos + 4;
          vos += 8;
          x = bindata.getInt32(os);
          return x;
        case Value.Type.String:
          return this.deserialize_string(bindata);
        case Value.Type.Binary:
          return this.deserialize_bin(bindata);
        case Value.Type.Collection:
          return this.deserialize_coll(bindata);
        case Value.Type.List:
          return this.deserialize_list(bindata);
        case Value.Type.Dictionary:
          return this.deserialize_dict(bindata);
      }
    };

    Value.deserialize = function(bindata, reset) {
      var type;
      if (reset === 1) {
        vos = 0;
      }
      type = this.deserialize_i32(bindata);
      return this.deserialize_value(bindata, type);
    };

    return Value;

  })();

  Result = (function() {
    function Result(cookie1) {
      this.cookie = cookie1;
      this.is_signal = false;
      this.is_broadcast = false;
      this.onvalue = null;
      this.value = null;
    }

    return Result;

  })();

  Collection = (function() {
    var List, Operand;

    Collection.prototype.Type = {
      Reference: 0,
      Universe: 1,
      Union: 2,
      Intersection: 3,
      Complement: 4,
      Has: 5,
      Match: 6,
      Token: 7,
      Equals: 8,
      NotEqual: 9,
      Smaller: 10,
      SmallerEq: 11,
      Greater: 12,
      GreaterEq: 13,
      Order: 14,
      Limit: 15,
      MediaSet: 16,
      IDList: 17
    };

    function Collection(type1) {
      this.type = type1;
      this.reset();
    }

    Collection.prototype.reset = function() {
      this.attributes = {};
      this.operands = [];
      return this.idlist = [];
    };

    Collection.Universe = (function(superClass) {
      extend(Universe, superClass);

      function Universe() {
        Universe.__super__.constructor.call(this, this.Type.Universe);
      }

      return Universe;

    })(Collection);

    List = (function(superClass) {
      extend(List, superClass);

      function List(type, list, operands) {
        var item, j, k, len, len1, operand;
        if (type == null) {
          type = "list";
        }
        if (list == null) {
          list = [];
        }
        if (operands == null) {
          operands = [];
        }
        List.__super__.constructor.call(this, this.Type.IDList);
        this.attributes.type = type;
        for (j = 0, len = list.length; j < len; j++) {
          item = list[j];
          this.idlist.push(item);
        }
        for (k = 0, len1 = operands.length; k < len1; k++) {
          operand = operands[k];
          this.operands.push(operand);
        }
      }

      return List;

    })(Collection);

    Collection.IDList = (function(superClass) {
      extend(IDList, superClass);

      function IDList(list, operands) {
        IDList.__super__.constructor.call(this, "list", list, operands);
      }

      return IDList;

    })(List);

    Collection.Queue = (function(superClass) {
      extend(Queue, superClass);

      function Queue(list, operands) {
        Queue.__super__.constructor.call(this, "queue", list, operands);
      }

      return Queue;

    })(List);

    Collection.PartyShuffle = (function(superClass) {
      extend(PartyShuffle, superClass);

      function PartyShuffle() {
        return PartyShuffle.__super__.constructor.apply(this, arguments);
      }

      PartyShuffle.prototype.contructor = function(list, operands) {
        if (operands == null) {
          operands = [new Collection.Universe()];
        }
        return PartyShuffle.__super__.contructor.call(this, "pshuffle", list, operands);
      };

      return PartyShuffle;

    })(List);

    Collection.Reference = (function(superClass) {
      extend(Reference, superClass);

      function Reference(ref, ns) {
        if (ns == null) {
          ns = "Collections";
        }
        Reference.__super__.constructor.call(this, this.Type.Reference);
        this.attributes["reference"] = ref;
        this.attributes["namespace"] = ns;
      }

      return Reference;

    })(Collection);

    Collection.Complement = (function(superClass) {
      extend(Complement, superClass);

      function Complement(operand) {
        Complement.__super__.constructor.call(this, this.Type.Complement);
        if (operand) {
          this.operands.push(operand);
        }
      }

      return Complement;

    })(Collection);

    Collection.Intersection = (function(superClass) {
      extend(Intersection, superClass);

      function Intersection(operands) {
        var j, len, operand;
        if (operands == null) {
          operands = [];
        }
        Intersection.__super__.constructor.call(this, this.Type.Intersection);
        for (j = 0, len = operands.length; j < len; j++) {
          operand = operands[j];
          this.operands.push(operand);
        }
      }

      return Intersection;

    })(Collection);

    Collection.Union = (function(superClass) {
      extend(Union, superClass);

      function Union(operands) {
        var j, len, operand;
        if (operands == null) {
          operands = [];
        }
        Union.__super__.constructor.call(this, this.Type.Union);
        for (j = 0, len = operands.length; j < len; j++) {
          operand = operands[j];
          this.operands.push(operand);
        }
      }

      return Union;

    })(Collection);

    Operand = (function(superClass) {
      extend(Operand, superClass);

      function Operand(type, parent, attr) {
        var key, value;
        if (parent == null) {
          parent = new Collection.Universe();
        }
        Operand.__super__.constructor.call(this, type);
        for (key in attr) {
          value = attr[key];
          this.attributes[key] = value;
        }
        this.operands.push(parent);
      }

      return Operand;

    })(Collection);

    Collection.Has = (function(superClass) {
      extend(Has, superClass);

      function Has(parent, attr) {
        Has.__super__.constructor.call(this, this.Type.Has, parent, attr);
      }

      return Has;

    })(Operand);

    Collection.Match = (function(superClass) {
      extend(Match, superClass);

      function Match(parent, attr) {
        Match.__super__.constructor.call(this, this.Type.Match, parent, attr);
      }

      return Match;

    })(Operand);

    Collection.Token = (function(superClass) {
      extend(Token, superClass);

      function Token(parent, attr) {
        Token.__super__.constructor.call(this, this.Type.Token, parent, attr);
      }

      return Token;

    })(Operand);

    Collection.Equals = (function(superClass) {
      extend(Equals, superClass);

      function Equals(parent, attr) {
        Equals.__super__.constructor.call(this, this.Type.Equals, parent, attr);
      }

      return Equals;

    })(Operand);

    Collection.NotEqual = (function(superClass) {
      extend(NotEqual, superClass);

      function NotEqual(parent, attr) {
        NotEqual.__super__.constructor.call(this, this.Type.NotEqual, parent, attr);
      }

      return NotEqual;

    })(Operand);

    Collection.Smaller = (function(superClass) {
      extend(Smaller, superClass);

      function Smaller(parent, attr) {
        Smaller.__super__.constructor.call(this, this.Type.Smaller, parent, attr);
      }

      return Smaller;

    })(Operand);

    Collection.SmallerEqual = (function(superClass) {
      extend(SmallerEqual, superClass);

      function SmallerEqual(parent, attr) {
        SmallerEqual.__super__.constructor.call(this, this.Type.SmallerEq, parent, attr);
      }

      return SmallerEqual;

    })(Operand);

    Collection.Greater = (function(superClass) {
      extend(Greater, superClass);

      function Greater(parent, attr) {
        Greater.__super__.constructor.call(this, this.Type.Greater, parent, attr);
      }

      return Greater;

    })(Operand);

    Collection.GreaterEqual = (function(superClass) {
      extend(GreaterEqual, superClass);

      function GreaterEqual(parent, attr) {
        GreaterEqual.__super__.constructor.call(this, this.Type.GreaterEq, parent, attr);
      }

      return GreaterEqual;

    })(Operand);

    Collection.Order = (function(superClass) {
      extend(Order, superClass);

      function Order(parent, attr) {
        Order.__super__.constructor.call(this, this.Type.Order, parent, attr);
      }

      return Order;

    })(Operand);

    Collection.Limit = (function(superClass) {
      extend(Limit, superClass);

      function Limit(parent, start, length) {
        Limit.__super__.constructor.call(this, this.Type.Limit, parent, {
          start: start,
          length: length
        });
      }

      return Limit;

    })(Operand);

    Collection.Mediaset = (function(superClass) {
      extend(Mediaset, superClass);

      function Mediaset(parent, attr) {
        Mediaset.__super__.constructor.call(this, this.Type.Mediaset, parent, attr);
      }

      return Mediaset;

    })(Operand);

    return Collection;

  })();

  PropDict = (function() {
    PropDict.default_source_prefs = ["server", "client/*", "plugin/id3v2", "plugin/segment", "plugin/*", "*"];

    PropDict.flatten = function(dict, source_prefs) {
      var propdict;
      propdict = new PropDict(source_prefs);
      return propdict.transform(dict);
    };

    function PropDict(source_prefs1) {
      this.source_prefs = source_prefs1 != null ? source_prefs1 : PropDict.default_source_prefs;
    }

    PropDict.prototype.transform = function(dict) {
      var index, inner_dict, inner_key, key, lowest, newdict;
      newdict = {};
      for (key in dict) {
        inner_dict = dict[key];
        lowest = this.source_prefs.length + 1;
        for (inner_key in inner_dict) {
          index = this.find_matching_pattern(inner_key);
          if (index < lowest) {
            newdict[key] = inner_dict[inner_key];
            lowest = index;
          }
        }
      }
      return newdict;
    };

    PropDict.prototype.find_matching_pattern = function(key) {
      var i, j, len, pattern, ref1;
      ref1 = this.source_prefs;
      for (i = j = 0, len = ref1.length; j < len; i = ++j) {
        pattern = ref1[i];
        if (this.source_match_pattern(key, pattern)) {
          return i;
        }
      }
    };

    PropDict.prototype.source_match_pattern = function(source, pattern) {
      if (source === pattern) {
        return true;
      } else if (pattern[pattern.length - 1] !== "*") {
        return false;
      } else if (pattern.length === 1) {
        return true;
      } else {
        return source.slice(0, pattern.length - 1) === pattern.slice(0, pattern.length - 1);
      }
    };

    return PropDict;

  })();

  Bitbuffer = (function() {
    function Bitbuffer() {
      this.data = new ArrayBuffer(2048);
      this.buf = new Uint8Array(this.data, 0);
      this.dv = new DataView(this.buf.buffer, 0);
      this.offset = 0;
    }

    Bitbuffer.prototype.seek = function(offset) {
      this.offset = offset;
    };

    Bitbuffer.prototype.tell = function() {
      return this.offset;
    };

    Bitbuffer.prototype.write_byte = function(num) {
      this.dv.setUint8(this.offset, num)
      this.offset += 1;
    }

    Bitbuffer.prototype.write_int32 = function(num) {
      this.dv.setInt32(this.offset, num);
      this.offset += 4;
    }

    Bitbuffer.prototype.write_int64 = function(num) {
      if (num == -1) {
        this.write_byte(255);
        this.write_byte(255);
        this.write_byte(255);
        this.write_byte(255);
        this.write_byte(255);
        this.write_byte(255);
        this.write_byte(255);
        this.write_byte(255);
      }
      this.write_byte(0);
      this.write_byte(0);
      this.write_byte(0);
      this.write_byte(0);
      this.dv.setInt32(this.offset, num);
      this.offset += 4;
    }

    Bitbuffer.prototype.write_string = function(str) {
      var sv;
      var len;
      sv = new StringView(str);
      len = sv.rawData.byteLength;
      this.write_int32(len + 1);
      this.buf.set(sv.rawData, this.offset);
      this.offset += len;
      this.write_byte(0);
    }

    Bitbuffer.prototype.write_data = function(data) {
      var arr = Uint8Array.from(data);
      this.buf.set(arr, this.offset);
      this.offset += data.length;
    }

    return Bitbuffer;

  })();

  Error = (function() {
    function Error(msg1) {
      this.msg = msg1;
    }

    return Error;

  })();

  Client = (function() {
    Client.IPC = {};

    Client.prototype.protocol_version = 23;

    Client.prototype.max_cookie = 524288;

    function Client(clientname) {
      this.clientname = clientname;
      this.main = new Client.IPC.Main(this);
      this.xform = new Client.IPC.Xform(this);
      this.config = new Client.IPC.Config(this);
      this.bindata = new Client.IPC.Bindata(this);
      this.medialib = new Client.IPC.Medialib(this);
      this.playback = new Client.IPC.Playback(this);
      this.playlist = new Client.IPC.Playlist(this);
      this.collection = new Client.IPC.Collection(this);
      this.visualization = new Client.IPC.Visualization(this);
      this.mediainfo_reader = new Client.IPC.MediainfoReader(this);
      this.courier = new Client.IPC.Courier(this);
      this.ipc_manager = new Client.IPC.IpcManager(this);
    }

    Client.prototype.connect = function(ipcpath) {
      this.ipcpath = ipcpath;
      this.cookie = -1;
      this.results = [];
      this.input_buffer = new ArrayBuffer(4 * 2048);
      this.input_buffer_len = 0;
      this.sock = new WebSocket(this.ipcpath, "xmms");
      this.sock.binaryType = "arraybuffer";
      this.sock.onopen = (function(_this) {
        return function(event) {
          var result;
          result = _this.main.hello(_this.protocol_version, _this.clientname);
          return result.onvalue = _this.onconnect;
        };
      })(this);
      this.sock.onmessage = (function(_this) {
        return function(event) {
          return _this.process_packets(event.data);
        };
      })(this);
      this.sock.onclose = (function(_this) {
        return function(event) {
          return typeof _this.ondisconnect === "function" ? _this.ondisconnect(event.wasClean, event.reason) : void 0;
        };
      })(this);
      return this;
    };

    Client.prototype.disconnect = function() {
      if (!this.connected()) {
        return;
      }
      return this.sock.close();
    };

    Client.prototype.connected = function() {
      if (this.sock && this.sock.readyState === WebSocket.OPEN) {
        return true;
      } else {
        return false;
      }
    };

    Client.prototype.process_packets = function(pkt) {
      arr = new Uint8Array(this.input_buffer);
      pktarr = new Uint8Array(pkt);
      arr.set(pktarr, this.input_buffer_len);
      this.input_buffer_len += pkt.byteLength;

      while (1) {
        ret = this.process_packet(this.input_buffer, this.input_buffer_len);
        if (ret == 0) break;
        arr.copyWithin(0, ret, this.input_buffer_len);
        this.input_buffer_len -= ret;
      }
    }

    Client.prototype.process_packet = function(pkt, pkt_len) {
      var msg;
      var data;
      if (pkt_len < 16) {
        return 0;
      }
      data = new DataView(pkt, 0, pkt_len);
      msg = new Message();
      msg.object_id = data.getInt32(0);
      msg.command_id = data.getInt32(4);
      msg.cookie = data.getInt32(8);
      msg.payload_length = data.getInt32(12);
      if (pkt_len - 16 < msg.payload_length) {
        return 0;
      }
      msg.payload = pkt.slice(16, 16 + msg.payload_length);
      this.process_msg(msg);
      var used = 16 + msg.payload_length;
      return used;
    }

    Client.prototype.process_msg = function(msg) {
      var idx, j, len, ref1, restart, restart_msg, result, results, reusable;
      ref1 = this.results;
      results = [];
      for (j = 0, len = ref1.length; j < len; j++) {
        result = ref1[j];
        if (result.cookie === msg.cookie) {
          reusable = false;
          result.value = Value.deserialize(new DataView(msg.payload), 1);
          result.is_signal = msg.command_id === 32;
          result.is_broadcast = msg.command_id === 33;
          if (result.onvalue) {
            restart = typeof result.onvalue === "function" ? result.onvalue(result.value) : void 0;
            if (restart && result.is_signal) {
              restart_msg = new Message();
              restart_msg.object_id = msg.object_id;
              restart_msg.command_id = msg.command_id;
              restart_msg.args = [result.signal_id];
              this.send_signal_restart_message(restart_msg, result);
              reusable = true;
            }
          }
          if (!reusable && !result.is_broadcast) {
            idx = this.results.indexOf(result);
            if (idx !== -1) {
              this.results.splice(idx, 1);
              break;
            } else {
              results.push(void 0);
            }
          } else {
            results.push(void 0);
          }
        } else {
          results.push(void 0);
        }
      }
      return results;
    };

    Client.prototype.send = function(data) {
      return this.sock.send(data);
    };

    Client.prototype.send_message = function(msg) {
      var cookie, msgdata, result;
      cookie = this.next_cookie();
      msgdata = msg.assemble(cookie);
      result = new Result(cookie);
      this.results.push(result);
      this.send(msgdata);
      return result;
    };

    Client.prototype.send_signal_message = function(msg, signal_id) {
      var result;
      result = this.send_message(msg);
      result.signal_id = signal_id;
      return result;
    };

    Client.prototype.send_signal_restart_message = function(msg, result) {
      var cookie, msgdata;
      cookie = this.next_cookie();
      msgdata = msg.assemble(cookie);
      result.cookie = cookie;
      this.send(msgdata);
      return result;
    };

    Client.prototype.next_cookie = function() {
      if (this.cookie === this.max_cookie) {
        this.cookie = 0;
      } else {
        this.cookie += 1;
      }
      return this.cookie;
    };

    return Client;

  })();

  ActivePlaylist = "_active";

  CollectionChanged = {
    ADD: 0,
    UPDATE: 1,
    RENAME: 2,
    REMOVE: 3
  };

  CollectionNamespace = {
    ALL: "*",
    COLLECTIONS: "Collections",
    PLAYLISTS: "Playlists"
  };

  MedialibEntryStatus = {
    NEW: 0,
    OK: 1,
    RESOLVING: 2,
    NOT_AVAILABLE: 3,
    REHASH: 4
  };

  MediainfoReaderStatus = {
    IDLE: 0,
    RUNNING: 1
  };

  PlaybackStatus = {
    STOP: 0,
    PLAY: 1,
    PAUSE: 2
  };

  PlaylistChange = {
    ADD: 0,
    INSERT: 1,
    SHUFFLE: 2,
    REMOVE: 3,
    CLEAR: 4,
    MOVE: 5,
    SORT: 6,
    UPDATE: 7
  };

  Seek = {
    CUR: 1,
    SET: 2
  };

  PluginType = {
    ALL: 0,
    OUTPUT: 1,
    XFORM: 2
  };

  xmmsclient = {};

  xmmsclient.ActivePlaylist = ActivePlaylist;

  xmmsclient.Bindata = Bindata;

  xmmsclient.Client = Client;

  xmmsclient.Collection = Collection;

  xmmsclient.CollectionChanged = CollectionChanged;

  xmmsclient.CollectionNamespace = CollectionNamespace;

  xmmsclient.Error = Error;

  xmmsclient.MedialibEntryStatus = MedialibEntryStatus;

  xmmsclient.MediainfoReaderStatus = MediainfoReaderStatus;

  xmmsclient.Message = Message;

  xmmsclient.PlaybackStatus = PlaybackStatus;

  xmmsclient.PlaylistChange = PlaylistChange;

  xmmsclient.PluginType = PluginType;

  xmmsclient.PropDict = PropDict;

  xmmsclient.Result = Result;

  xmmsclient.Seek = Seek;

  xmmsclient.Value = Value;

  window.xmmsclient = xmmsclient;

}).call(this);
