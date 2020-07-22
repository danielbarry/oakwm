#!/bin/bash

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

# Set the window manager details
sudo ln -s /home/vagrant/dev/.xinitrc /home/vagrant/.xinitrc || true
sudo ln -s /home/vagrant/dev/.xinitrc /root/.xinitrc || true
