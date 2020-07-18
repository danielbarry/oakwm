#!/bin/bash

# Update local packages
sudo apt-get -y update
sudo apt-get -y upgrade

# Install required files
sudo apt-get -y install \
  cmake                 \ # Build script generator
  make                  \ # Build script
  libx11-dev            \ # Compile X11
  libxmu-dev            \
  libxtst-dev           \
  xorg                    # X11

# Set the window manager details
sudo ln -s /home/vagrant/.xinitrc /root/.xinitrc
