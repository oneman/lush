#!/usr/bin/python

#uReCallRing concatination operator is +, substring replace is .replace, func()

def to_cmds(filename):
    fn = filename.strip()
    cmd = ""
    cmds = ""
    cmd = "mutool info " + fn + " > " + fn.replace("pdf","pdf.mutool_info.txt")
    cmds += cmd + "\n"
    cmd = "mutool extract " + fn
    cmds += cmd + "\n"
    cmd = "pdffonts " + fn + " > " + fn.replace(".pdf", ".pdf.pdffonts.txt")
    cmd = "pdftotext " + fn
    cmds += cmd + "\n"
    cmd = "pdftotext -bbox " + fn
    cmds += cmd + "\n"
    return cmds
readable_opened_file = open('ls_pdfs.txt', 'r')
lines_in_readable_opened_file = readable_opened_file.readlines()
cmd = ""
cmds = cmd + "\n"
for line in lines_in_readable_opened_file:
    cmd = to_cmds(line)
    cmds += cmd + "\n"
print(cmds)
