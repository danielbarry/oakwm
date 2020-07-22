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
    int width;
    int height;
    char* data;
    XImage* img;

  public:
    /**
     * Icon()
     *
     * Load an image from disk.
     *
     * @param path The location of the image to be loaded.
     * @param x The X offset to start rendering the image to.
     * @param y The Y offset to start rendering the image to.
     * @param dis The display.
     * @param win The window.
     * @param gc The graphics object.
     **/
    Icon(std::string path, int x, int y, Display* dis, Window win, GC gc);

    /**
     * ~Icon()
     *
     * Release resources for the icon.
     **/
    ~Icon();

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
