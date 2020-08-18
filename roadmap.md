# Roadmap

## Requirements

* Low-RAM and CPU usage
* Touch gestures and keyboard/mouse inputs
* Screen rotation (i.e. via an IMU or manual request)

## Assumptions

* Single window only
* X11 server running

## Tasks

**Basic desktop-only:**

* `[x]` Source code building
* `[x]` Window manager running in VM
* `[x]` Run window manager without root
* `[~]` Clean-up source code
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
* `[x]` Design user interface
* `[~]` Basic toolbar
  * `[x]` Width of display
  * `[x]` Always on top
  * `[x]` Log to terminal
  * `[x]` Log to disk
  * `[x]` Log git build information
  * `[x]` Set window font
  * `[x]` Force redraw based on timer
  * `[x]` Display time
  * `[x]` Draw basic icons
  * `[x]` Icon hover and click
  * `[~]` Display battery state
  * `[x]` Display connection state
  * `[x]` Drop down menu
  * `[x]` Execute commands based on selection
* `[x]` Configuration file
  * `[x]` Convert Java JSON config project
  * `[x]` Pull in JSON strings
  * `[x]` Conversion of JSON strings
* `[ ]` Window manager
  * `[ ]` Remove root background click
  * `[ ]` Remove session exit
  * `[ ]` Modernize cursor
  * `[ ]` Modernize window decoration
  * `[ ]` Move configuration into JSON config file
  * `[ ]` Check for config file changes and reload
* `[ ]` Settings
  * `[ ]` Window to display setting types
  * `[ ]` Adjustment of settings updates configuration file
  * `[ ]` Template settings (min, max, default, value, type, etc)

**Basic desktop and mobile:**

* `[ ]` Detect orientation change
* `[ ]` Rotate display
  * `[ ]` Switch orientation of windows
  * `[ ]` Switch orientation of toolbar
* `[ ]` Detect mobile mode
* `[ ]` Virtual keyboard
* `[ ]` Remove mouse mode
* `[ ]` Snap window functionality
* `[ ]` Auto-snap when entering mobile mode
* `[x]` Switch between programs
* `[ ]` Volume
* `[ ]` Power saving
  * `[ ]` Perform some actions when in low power scenario
  * `[ ]` Display brightness
  * `[ ]` Aeroplane mode (transmitters off)

**Usability:**

* `[ ]` Some automated install process for the window manager onto a system
