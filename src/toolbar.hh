#pragma once

#include "icon.hh"
#include "json.hh"

#include <vector>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/**
 * toolbar.hh
 *
 * Create a toolbar that accepts click/touch input, offering important
 * information and allowing various configuration updates. The toolbar can also
 * be used to launch selected programs.
 **/
class Toolbar{
  private:
    JSON* json;
    Display* dis;
    int screen;
    Window win;
    GC gc;
    long eventDelay;
    int width;
    int height;
    unsigned long background;
    unsigned long foreground;
    long eventMask;
    std::vector<Icon*> icons;
    XFontStruct* font;
    long long lastTimeUpdate;
    char tBuff[16];

  public:
    /**
     * Toolbar()
     *
     * Construct the toolbar and register with X11 where appropriate.
     *
     * @param filename The location of the configuration file, otherwise NULL.
     **/
    Toolbar(char* filename);

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
