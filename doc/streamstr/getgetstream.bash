#!/bin/bash

while [ 1 ]
do
  echo "we beging getgetstream.bash ing here"
  uname -a
  date
  wget "http://192.168.1.1:8000/stream"
  echo "hrm waiting 26 seconds"
  uname -a
  date
  sleep 26
done
