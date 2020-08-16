#!/bin/bash

# Remove unwanted packages
sudo apt-get -y autoremove
sudo apt-get -y autoclean

# Update local packages
sudo apt-get -y update
sudo apt-get -y upgrade

# Install required files
sudo apt-get -y install \
  acpi                  \
  cmake                 \
  make                  \
  libx11-dev            \
  libxmu-dev            \
  libxtst-dev           \
  xorg

# Install nice to have programs
sudo apt-get -y install \
  alsa-utils            \
  dillo                 \
  matchbox-keyboard     \
  thunderbird           \
  wicd-curses

# Remove unwanted packages
sudo apt-get -y autoremove
sudo apt-get -y autoclean

# Set the window manager details
sudo ln -s /home/vagrant/dev/.xinitrc /home/vagrant/.xinitrc || true
sudo ln -s /home/vagrant/dev/.xinitrc /root/.xinitrc || true
