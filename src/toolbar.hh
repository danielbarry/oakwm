#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* NOTE: Hack for parsing non-strings into strings. */
#define STR1(x) #x
#define STR(x) STR1(x)

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
     * Main window loop and handle incoming events.
     **/
    void loop();

    /**
     * redraw()
     *
     * Redraw the window completely.
     **/
    void redraw();
};
