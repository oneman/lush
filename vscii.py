#!/usr/bin/env python
# Virginia Standard Code for Information Integration
for i in range(1, 27):
  b = bin(i);
  print(chr(i + 96), b[2:].zfill(5))
