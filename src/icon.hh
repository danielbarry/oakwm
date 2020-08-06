#pragma once

#include <map>
#include <string>
#include <X11/Xlib.h>
#include <vector>

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
    std::map<std::string, int> mods;
    std::vector<char*> data;
    std::vector<XImage*> imgs;
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
     * addModifier()
     *
     * Add a colour modifier from the baseline based on a mask filter.
     *
     * @param name The name of the modifier.
     * @param mask The mask to be applied.
     * @param dis The display.
     **/
    void addModifier(std::string name, unsigned long mask, Display* dis);

    /**
     * setXY()
     *
     * Set the X and Y offset positions for drawing.
     *
     * @param x The X offset.
     * @param y The Y offset.
     **/
    void setXY(int x, int y);

    /**
     * getWidth()
     *
     * Get the width of the icon.
     *
     * @return The width of the icon.
     **/
    int getWidth();

    /**
     * getHeight()
     *
     * Get the height of the icon.
     *
     * @return The height of the icon.
     **/
    int getHeight();

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
