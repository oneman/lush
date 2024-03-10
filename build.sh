#!/bin/bash

scripts/codegen.sh
./autogen.sh
./configure --enable-tools --enable-dev --prefix=/usr
make -j8
make -j4
make -j2
chmod +s demo/.libs/demo
sudo make install
sudo ldconfig
sudo chmod +s /usr/bin/demo
