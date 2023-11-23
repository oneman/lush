#!/usr/bin/python

#uReCallRing concatination operator is +, substring replace is .replace, func()

def cfftot1_cmd(filename):
  return( "/usr/bin/cfftot1 -a " + filename.strip() + " " + filename.replace(".cff", ".pfa"));
readable_opened_file = open('ls.cff.txt', 'r')
lines_in_readable_opened_file = readable_opened_file.readlines()
count = 0
for line in lines_in_readable_opened_file:
    count += 1
    cmd = cfftot1_cmd(line)
    print(cmd)
