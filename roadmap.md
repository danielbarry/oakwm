# Roadmap

## Requirements

* Low-RAM and CPU usage
* Touch gestures and keyboard/mouse inputs
* Screen rotation (i.e. via an IMU or manual request)

## Assumptions

* Single window only
* X11 server running

## Tasks

* `[x]` Source code building
* `[x]` Window manager running in VM
* `[ ]` Clean-up source code
  * `[ ]` `border.cc`
  * `[ ]` `client.cc`
  * `[ ]` `cursors.hh`
  * `[ ]` `listmacro2.hh`
  * `[~]` `manager.hh`
  * `[ ]` `border.hh`
  * `[ ]` `client.hh`
  * `[ ]` `events.cc`
  * `[x]` `main.cc`
  * `[ ]` `rotated.cc`
  * `[ ]` `buttons.cc`
  * `[ ]` `config.hh`
  * `[ ]` `general.hh`
  * `[~]` `manager.cc`
  * `[ ]` `rotated.hh`
* `[ ]` Design user interface
* `[ ]` Basic toolbar
  * `[x]` Width of display
  * `[x]` Always on top
  * `[x]` Log to terminal
  * `[x]` Log to disk
  * `[x]` Log git build information
  * `[ ]` Force redraw based on timer
  * `[ ]` Display time
  * `[ ]` Draw basic icons
  * `[ ]` Display battery state
  * `[ ]` Display connection state
  * `[ ]` Drop down menu
* `[ ]` Configuration file
  * `[ ]` Convert Java JSON config project
  * `[ ]` Pull in JSON strings
  * `[ ]` Conversion of JSON strings
