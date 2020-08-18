#pragma once

#include "json.hh"

#include <string>
#include <X11/Xlib.h>
#include <vector>

/**
 * menu.hh
 *
 * Display status and buttons on an X11 window.
 **/
class Menu{
  private:
    enum Type{
      NETWORK,
      NORMAL,
      WINDOWS
    };

    enum ItemType{
      COMMAND,
      NONE,
      POWER
    };

    struct Item{
      ItemType type;
      std::string text;
      std::string cmd;
      Window win;
    };

    JSON* config;
    std::string mName;
    Display* dis;
    int screen;
    Window win;
    GC gc;
    Type type;
    int xOffset;
    int yOffset;
    int width;
    unsigned long background;
    unsigned long foreground;
    unsigned long highlight;
    XFontStruct* font;
    int textHeight;
    int maxItems;
    std::vector<Item> items;
    int select;
    bool focus;
    bool active;
    std::string pathBatt;
    std::string pathDC;

  public:
    /**
     * Menu()
     *
     * Setup the menu with options.
     *
     * @param cfg The configuration for the toolbar.
     * @param iCfg The configuration for this menu.
     * @param strFont The font to be used for the drop down.
     * @param display The display to show the window on.
     * @param disScreen The screen to show the window on.
     **/
    Menu(
      JSON* cfg,
      JSON* iCfg,
      XFontStruct* strFont,
      Display* display,
      int disScreen
    );

    /**
     * ~Menu()
     *
     * Release resources for the menu.
     **/
    ~Menu();

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
     * addItem()
     *
     * Add a menu item as an option.
     *
     * @param text The text to be displayed in the drop down menu.
     * @param cmd The command to be executed.
     * @param win The window attached to the task, NULL if not applicable.
     * @return True if successful, otherwise false.
     **/
    bool addItem(std::string text, std::string cmd, Window win);

    /**
     * setState()
     *
     * Inform the menu that it should update the internal state.
     *
     * @param fState True if menu focused, otherwise false.
     * @param aState True if menu active, otherwise false.
     * @param x The X location of the mouse.
     * @param y The Y location of the mouse.
     * @return True if the internal state changes, otherwise false.
     **/
    bool setState(bool fState, bool aState, int x, int y);

    /**
     * draw()
     *
     * Render to the window graphics.
     *
     * @param dis The display.
     **/
    void draw(Display* dis);
};
