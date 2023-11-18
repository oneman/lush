#!/bin/env ruby

require 'faye/websocket'
require 'eventmachine'
require 'rubygems'
require 'json'

kr = Hash.new
kr[:host] = "localhost"
kr[:port] = ENV["KRAD_WEB_PORT"] ||= 5000
kr[:test_data] = ENV["KRAD_TEST_DATA"]

def kr_send(kr, msg)
  puts "Sending #{msg.length} byte message"
  kr[:ws].send(msg)
end

def kr_message(kr, event)
  puts "Message: " + event.data
end

def kr_connected(kr, event)
  puts "Connected. Websocket Version: #{kr[:ws].version} Protocol: #{kr[:ws].protocol}"
end

def kr_disconnected(kr, event)
  puts "\nDisconnected: Code #{event.code} #{event.reason}"
end

EM.run {
  Signal.trap("INT") { EventMachine.stop }
  Signal.trap("TERM") { EventMachine.stop }
  ws_url = "ws://" + kr[:host] + ":" + kr[:port]
  ws_proto = "krad-ws-api"
  kr[:ws] = Faye::WebSocket::Client.new(ws_url, [ws_proto])
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
