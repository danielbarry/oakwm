#pragma once

#include "menu.hh"

#include "json.hh"

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
    JSON* config;
    std::string nameId;
    int destX;
    int destY;
    bool interact;
    Menu* drop;
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
     * @param cfg The configuration for the toolbar.
     * @param iCfg The configuration for this menu.
     * @param font The font to be used for the drop down.
     * @param menu The menu and it's options to be displayed.
     * @param dis The display.
     * @param screen The screen to show the window on.
     * @param win The window.
     * @param gc The graphics object.
     **/
    Icon(
      JSON* cfg,
      JSON* iCfg,
      XFontStruct* font,
      Display* dis,
      int screen,
      Window win,
      GC gc
    );

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
     * getName()
     *
     * Get the assigned name for this icon.
     *
     * @return The name of this icon.
     **/
    std::string getName();

    /**
     * getMenu()
     *
     * Get the menu for this icon.
     *
     * @return A menu instance for this icon.
     **/
    Menu* getMenu();

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
     * @param y The Y location of the mouse.
     * @return True if within the bounds, otherwise false.
     **/
    bool insideBounds(int x, int y);

    /**
     * setFocused()
     *
     * Inform the icon that it should update the internal focus state.
     *
     * @param state True if icon focused, otherwise false.
     * @param x The X location of the mouse.
     * @param y The Y location of the mouse.
     * @return True if the internal state changes, otherwise false.
     **/
    bool setFocused(bool state, int x, int y);

    /**
     * setActive()
     *
     * Inform the icon that it should update the internal active state.
     *
     * @param state True if the icon active, otherwise false.
     * @param x The X location of the mouse.
     * @param y The Y location of the mouse.
     * @return True if the internal state changes, otherwise false.
     **/
    bool setActive(bool state, int x, int y);

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
