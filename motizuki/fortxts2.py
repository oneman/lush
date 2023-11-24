#!/usr/bin/python

#uReCallRing concatination operator is +, substring replace is .replace, func()

def to_cmds(filename):
    fn = filename.strip().replace("\"","")
    cmd = ""
    cmds = ""
    cmd = "/home/demo/src/rad/daemon/krad_radio " + "\"/home/demo/src/rad/motizuki/" + fn
    cmd += "\" > \"" + "/home/demo/src/rad/motizuki/" + fn.replace(".txt", ".uwls.txt")+ "\""
    cmds += cmd + "\n"
    return cmds
readable_opened_file = open('ls_txts.txt', 'r')
lines_in_readable_opened_file = readable_opened_file.readlines()
cmd = ""
cmds = cmd + "\n"
for line in lines_in_readable_opened_file:
    cmd = to_cmds(line)
    cmds += cmd + "\n"
print(cmds)
