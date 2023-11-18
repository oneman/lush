make -C /lib/modules/`uname -r`/build M=$PWD
sudo make -C /lib/modules/`uname -r`/build M=$PWD modules_install
