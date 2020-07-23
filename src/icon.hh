#pragma once

#include <string>
#include <X11/Xlib.h>

/**
 * icon.hh
 *
 * Handle small images to be draw to an X11 window.
 **/
class Icon{
  private:
    int destX;
    int destY;
    bool interact;
    int width;
    int height;
    char* data;
    XImage* img;
    bool focus;
    bool active;

  public:
    /**
     * Icon()
     *
     * Load an image from disk.
     *
     * @param path The location of the image to be loaded.
     * @param x The X offset to start rendering the image to.
     * @param y The Y offset to start rendering the image to.
     * @param inter True if this icon is interactive.
     * @param dis The display.
     * @param win The window.
     * @param gc The graphics object.
     **/
    Icon(std::string path, int x, int y, bool inter, Display* dis, Window win, GC gc);

    /**
     * ~Icon()
     *
     * Release resources for the icon.
     **/
    ~Icon();

    /**
     * interactive()
     *
     * Check whether this icon is supposed to be interactive.
     *
     * @return True if it is, otherwise false.
     **/
    bool interactive();

    /**
     * insideBounds()
     *
     * Check if the mouse is within the bounds of the icon.
     *
     * @param x The X location of the mouse.
     * @param Y The X location of the mouse.
     * @return True if within the bounds, otherwise false.
     **/
    bool insideBounds(int x, int y);

    /**
     * setFocused()
     *
     * Inform the icon that it should update the internal focus state.
     *
     * @param state True if icon focused, otherwise false.
     **/
    void setFocused(bool state);

    /**
     * setActive()
     *
     * Inform the icon that it should update the internal active state.
     *
     * @param state True if the icon active, otherwise false.
     **/
    void setActive(bool state);

    /**
     * draw()
     *
     * Render to the window graphics.
     *
     * @param dis The display.
     * @param win The window.
     * @param gc The graphics object.
     **/
    void draw(Display* dis, Window win, GC gc);
};
