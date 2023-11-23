#!/usr/bin/python

#uReCallRing concatination operator is +, substring replace is .replace, func()

readable_opened_file = open('ls_txts.txt', 'r')
lines_in_readable_opened_file = readable_opened_file.readlines()
for line in lines_in_readable_opened_file:
  fn = line.strip().replace("\"", "")
  rof = open(fn, 'r')
  wof = open(fn.replace(".txt", ".uwordls.txt"), 'w')
  lirof = rof.readlines()
  for paperline in lirof:
    words = paperline.split()
    for word in words:
      wof.write(word + "\n")
