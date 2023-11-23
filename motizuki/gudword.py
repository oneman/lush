#!/usr/bin/python

import sys

#
#/* ok, so we look at a uwordls txt file and verify that everything on the listi
# * is less than 78 bytes long
# *
# * then we run our prime word statistics *
# *
# *
# * */

#uReCallRing concatination operator is +, substring replace is .replace, func()

readable_opened_file = open(sys.argv[1], 'r')
lines_in_readable_opened_file = readable_opened_file.readlines()
maxlen = 0
for line in lines_in_readable_opened_file:
    ll = len(line)
    if (ll > maxlen):
        print(line + " " + str(ll))
        maxlen = ll
