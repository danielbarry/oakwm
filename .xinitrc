#!/bin/sh
cd /home/vagrant/dev
./oakwm-toolbar cfg/default.json &
xterm -geometry 80x24-40+30 -e /bin/bash -l -i -c 'cd /home/vagrant/dev; git describe --always --dirty --abbrev=40 --match="NoTagWithThisName"; exec bash' &
exec ./oakwm
