#!/bin/sh
cd /home/vagrant/dev
./oakwm-toolbar &
xclock -geometry 10x5-50-50 -d -brief -twentyfour -padding 2 &
xterm  -geometry 10x5+0+20 -e "watch -d -t --interval=10 \"ip -o a | cut -d' ' -f2\"" &
xterm  -geometry 60x1-40+20 -e "watch -d -t --interval=10 acpi -b" &
xterm  -geometry 80x20-40+50 -e /bin/bash -l -i -c 'cd /home/vagrant/dev; git describe --always --dirty --abbrev=40 --match="NoTagWithThisName"; exec bash' &
exec ./oakwm
