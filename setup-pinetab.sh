#!/bin/bash

# Rotate display and input
xrandr -o right
xinput set-prop 18 --type=float "Coordinate Transformation Matrix" 0 1 0 -1 0 1 0 0 1

# Start window manager
./oakwm-toolbar cfg/pinetab-v1-arch.json &
