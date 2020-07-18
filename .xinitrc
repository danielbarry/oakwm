#!/bin/sh
xclock -d -brief -twentyfour -padding 2 &
xterm -geometry 80x24+70+0 &
exec /home/vagrant/oakwm
