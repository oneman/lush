cat /home/demo/funstuff/cdrom/QUAKE/ID1/CONFIG.CFG > /home/demo/config.cfg
cat /home/demo/funstuff/cdrom/Quake2/baseq2/config.cfg >> /home/demo/config.cfg
cat /home/demo/funstuff/cdrom/Caveless/mirc.ini >> /home/demo/config.cfg
cd /home/demo/src/lush
cat config.cfg > config.broke
cat config.broke | tools/nukecr > config.cfg
rm config.broke
cat doc/guestbook >> config.cfg
cat ~/.bashrc >> config.cfg
cat ~/.vimrc >> config.cfg
