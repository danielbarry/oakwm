#!/bin/sh
xclock -d -brief -twentyfour -padding 2 &
xterm -geometry 60x1+60+0 -e "watch -t --interval=10 acpi -b" &
xterm -geometry 80x24+60+30 &
exec /home/vagrant/oakwm
