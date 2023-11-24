#!/bin/bash

# lushash sha256

rm ls.sha256sum.txt
touch ls.sha256sum.txt

for file in $PWD/*
do
  /usr/bin/sha256sum "$file" >> ls.sha256sum.txt
done
