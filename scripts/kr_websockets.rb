#!/bin/env ruby

require 'faye/websocket'
require 'eventmachine'
require 'rubygems'
require 'json'

def put_wayland(ws)
  crate = '{
        "put": "/xpdr/Wayland",
        "xpdr_path_info": {
            "wayland": {
                "display_name": "",
                "state": "wl_connected"
            }
        }
  }';

  ws.send(crate)
end

def put_wayland_path(ws, name, w, h, fullscreen)
  crate = '{
        "put": "/xpdr/Wayland/' + name + '",
        "xpdr_path_info": {
            "wayland_out": {
                "width": ' + w.to_s + ',
                "height": ' + h.to_s + ',
                "fullscreen": ' + fullscreen.to_s + '
            }
        }
  }'
  p "#{crate}"
  ws.send(crate)
end

def patch_wayland(ws)
  crate = '    {
        "patch": "/xpdr/Wayland/TestWindow",
        "xpdr_path_info": [
            {
                "op": "replace",
                "path": "/width",
                "value": 1280
            }
            {
                "op": "replace",
                "path": "/height",
                "value": 720
            }
        ]
  }'
  ws.send(crate)
end

def delete_wayland(ws)
  crate = '{"delete": "/xpdr/Wayland/TestWindow"}'
  ws.send(crate)
end

def put_v4l2(ws, name, dev)
  crate = '{
        "put": "/xpdr/' + name + '",
        "xpdr_path_info": {
            "v4l2": {
                "dev": ' + dev.to_s + ',
                "priority": 0,
                "state": "v4l2_void"
            }
        }
  }'
  ws.send(crate)
end

def put_v4l2_path(ws, vname, name,  w, h)
  crate = '    {
        "put": "/xpdr/' + vname + '/' + name + '",
        "xpdr_path_info": {
            "v4l2_in": {
                "width": ' + w.to_s + ',
                "height": ' + h.to_s + ',
                "num": 30,
                "den": 1,
                "format": 0
            }
        }
  }'
  ws.send(crate)
end

def put_v4l2_wayland_compositor(ws, wlname, vname)
  crate = '{
        "put": "/compositor/' + wlname + '/' + vname + '",
        "compositor_path_info": {
           "com_input": {
             "opacity" : 1.00,
             "pos": {"x" : 0,"y" : 0,"w" : 7680,"h" : 4320},
             "crop": {"x" : 0,"y" : 0,"w" : 7680,"h" : 4320},
             "view": {"top_left": {"x" : 0,"y" : 0},"top_right": {"x" : 0,"y" : 0},"bottom_left": {"x" : 0,"y" : 0},"bottom_right": {"x" : 0,"y" : 0}},
             "z" : 0,
             "rotation" : 0.00}
        }
   }'
   ws.send(crate)
end

angle = 0
def spin_and_fade_in_out(ws, angle, del, opacity, odel, fade_to, wlname, vname)
  angle = angle + del
  angle = angle % 360
  if fade_to == 0
    opacity = opacity - odel
    if opacity < 0
      opacity = 0
      fade_to = 1
    end
  else
    opacity = opacity + odel
    if opacity > 1
      opacity = 1
      fade_to = 0
    end
  end

  crate = '{
        "patch": "/compositor/' + wlname + '/' + vname + '",
        "compositor_input_info": [
            {
                "op": "replace",
                "path": "/rotation",
                "value": ' + sprintf('%.2f', angle) + '
            },
            {
                "op": "replace",
                "path": "/opacity",
                "value": ' + sprintf('%.2f', opacity) + '
            }
        ]
  }'
  ws.send(crate)
  return [angle, opacity, fade_to]
end

def get_stuff(ws)
  getcmd = '{"GET":"/compositor"}'
  ws.send(getcmd)
end

def crates(ws, filename)
  file = open(filename)
  json = file.read

  parsed = JSON.parse(json)

  parsed.each do |crate|
    ws.send(JSON.generate(crate))
  end
end

def on_message(ws, event)
p [:message, event.data]
end


krad_build_dir = ENV["KRAD_BUILD_DIR"]
krad_test_dir = ENV["KRAD_TEST_DATA"]
krad_web_host = "localhost"
krad_web_port = ENV["KRAD_WEB_PORT"]

if krad_build_dir == nil
  #p "you must have env variable KRAD_BUILD_DIR set to krad source"
  #abort
end

if krad_test_dir == nil
  #p "you must have env variable KRAD_TEST_DATA set"
  #abort
end

if krad_web_port == nil
  p "you must have env variable KRAD_WEB_PORT set"
  abort
end

EM.run {

  Signal.trap("INT") { EventMachine.stop }
  Signal.trap("TERM") { EventMachine.stop }

  ws_url = "ws://" + krad_web_host + ":" + krad_web_port
  ws_proto = "krad-ws-api"

  ws = Faye::WebSocket::Client.new(ws_url, [ws_proto])

  puts "Opening: " + ws_url + " Protocol: " + ws_proto

  ws.on :open do |event|
    p [:open]
    puts "Websocket Version: #{ws.version}\nWebsocket Protocol: #{ws.protocol}"
    #get_stuff(ws)
    #crates(ws, krad_code_dir + '/crates2.json')
    #put_wayland(ws)
    #put_v4l2(ws, "V4L2", 1)
    #put_v4l2(ws, "V4L2_1", 0)
    angle = 0
    opacity = 1.0
    fade_to = 0
    ret_array = [angle,opacity,fade_to]
    #timer = EventMachine::Timer.new(1) { put_wayland_path(ws, "TestWindow", 1280, 720, 0)}
    #timer = EventMachine::Timer.new(1.5) { put_v4l2_path(ws, "V4L2_1", "Capture2", 1280, 720)}
    #timer = EventMachine::Timer.new(2) { put_v4l2_wayland_compositor(ws, "TestWindow1", "Capture2")}
    #timer = EventMachine::Timer.new(2.5) { put_v4l2_path(ws, "V4L2", "Capture", 1280, 720)}
    #timer = EventMachine::Timer.new(3) { put_v4l2_wayland_compositor(ws, "TestWindow", "Capture")}
    #timer = EventMachine::Timer.new(10) { put_wayland_path(ws, "TestWindow2", 1280, 720, 0)}
    timer = EventMachine::Timer.new(4) {
      timer = EventMachine::PeriodicTimer.new(0.2) {
        ret_array = spin_and_fade_in_out(ws, ret_array[0], 1, ret_array[1],
         #0.01, ret_array[2], "TestWindow", "Capture")
         0.01, ret_array[2], "TestWindow", "CamA")
      }
    }
  end

  ws.on :message do |event|
    on_message(ws, event)
  end

  ws.on :close do |event|
    puts ""
    puts "Closing.."
    p [:close, event.code, event.reason]
    ws = nil
    EventMachine.stop
  end
}
