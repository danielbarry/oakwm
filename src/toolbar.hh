#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

class Toolbar{
  private:
    Display* dis;
    int screen;
    Window win;
    GC gc;

  public:
    /**
     * Toolbar()
     *
     * Construct the toolbar and register with X11 where appropriate.
     **/
    Toolbar();

    /**
     * ~Toolbar()
     *
     * Release resources for toolbar.
     **/
    ~Toolbar();

  private:
    /**
     * loop()
     *
     * Main window loop.
     **/
    void loop();
};
