#!/bin/sh
./oakwm-toolbar &
xclock -fg yellow -bg black -geometry 10x5-50-50 -d -brief -twentyfour -padding 2 &
xterm  -fg purple -bg black -geometry 10x5 -e "watch -d -t --interval=10 \"ip -o a | cut -d' ' -f2\"" &
xterm  -fg green  -bg black -geometry 60x1-40+0 -e "watch -d -t --interval=10 acpi -b" &
xterm  -fg white  -bg black -geometry 80x20-40+30 &
exec /home/vagrant/oakwm
