#!/usr/bin/env python

import sys
import subprocess

def subscript(args):
  sub = subprocess.Popen(args, stdout=subprocess.PIPE, universal_newlines=True)
  msg = sub.communicate(timeout=1)
  ret = msg[0]
  return ret
ss = subscript
#uReCallRing concatination operator is +, substring replace is .replace, func()

if (len(sys.argv) != 2):
  print("Usage: splitflac.py filename.flac\n")
  exit()

filename = sys.argv[1]
cmd = ["ls", "-s", filename]
ret = ss(cmd)
ret = ret.split()
ret = ret[0]
max_ns = int(ret) * 126

split_ns = (((7800000/26000)-(7800000/52000))*48000);

cmd = ""
cmds = cmd + "\n"

skip_ns = int(0)
until_ns = split_ns
count = 1

while skip_ns < max_ns:
  nn = filename.replace(".flac", ".")
  nn += str(int(skip_ns)) + "-"
  nn += str(int(until_ns)) + ".wav"
  cmd = "flac "
  cmd += "-d " + filename + " "
  cmd += "--until=" + str(int(until_ns)) + " --skip="
  cmd += str(int(skip_ns)) + " -o " + nn
  cmds = cmds + "\n" + cmd
  cmd = "flac " + nn
  cmds = cmds + "\n" + cmd
  cmd = "rm " + nn
  cmds = cmds + "\n" + cmd
  skip_ns += split_ns
  until_ns += split_ns
  count += 1

print(cmds)
