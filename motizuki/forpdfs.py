#!/usr/bin/python

#uReCallRing concatination operator is +, substring replace is .replace, func()

def to_cmds(filename):
    fn = filename.strip()
    cmds = ""
    cmds = cmds + "mutool info " + fn + " > " + fn.replace("pdf","pdf.mutool_info.txt")
    cmds = cmds + "mutool extract " + fn
    return cmds
readable_opened_file = open('ls_pdfs.txt', 'r')
lines_in_readable_opened_file = readable_opened_file.readlines()
for line in lines_in_readable_opened_file:
    cmds = to_cmds(line)
    print(cmds)
