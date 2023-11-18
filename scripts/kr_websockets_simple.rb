#!/bin/env ruby

require 'faye/websocket'
require 'eventmachine'
require 'rubygems'
require 'json'

kr = Hash.new
kr[:host] = ENV["KRAD_WEB_HOST"] ||= "localhost"
kr[:port] = ENV["KRAD_WEB_PORT"] ||= 5000
kr[:test_data] = ENV["KRAD_TEST_DATA"]

def kr_send(kr, msg)
  puts "Sending #{msg.length} byte message"
  kr[:ws].send(msg)
end

def kr_message(kr, event)
  puts "[#{event.data.length} bytes recv]: " + event.data
end

def kr_connected(kr, event)
  puts "Connected. Websocket Version: #{kr[:ws].version} Protocol: #{kr[:ws].protocol}"
end

def kr_disconnected(kr, event)
  puts "\nDisconnected: Code #{event.code} #{event.reason}"
  sleep 0.3
end

def kr_shutdown(kr)
  if kr[:ws]
    kr[:ws].close
  end
  puts "\nShuting down.."
  EventMachine.stop
end

EM.run {
  Signal.trap("INT") { kr_shutdown(kr) }
  Signal.trap("TERM") { kr_shutdown(kr) }
  ws_url = "ws://" + kr[:host] + ":" + kr[:port]
  ws_proto = "krad-ws-api"
  kr[:ws] = Faye::WebSocket::Client.new(ws_url, [ws_proto], { :ping => 10 })
  puts "Opening: " + ws_url + " Protocol: " + ws_proto
  kr[:ws].on :open do |event|
    kr_connected(kr, event)
  end
  kr[:ws].on :message do |event|
    kr_message(kr, event)
  end
  kr[:ws].on :close do |event|
    kr_disconnected(kr, event)
    kr[:ws] = nil
    EventMachine.stop
  end
}
