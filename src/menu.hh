#pragma once

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
    struct Item{
      std::string text;
      std::string cmd;
    };

    Display* dis;
    int screen;
    Window win;
    GC gc;
    int yOffset;
    int maxItems;
    std::vector<Item> items;
    bool focus;
    bool active;

  public:
    /**
     * Menu()
     *
     * Setup the menu with options.
     *
     * @param yOff The Y offset to start drawing the menu.
     * @param max The maximum number of items to be displayed.
     * @param display The display to show the window on.
     * @param disScreen The screen to show the window on.
     **/
    Menu(int yOff, int max, Display* display, int disScreen);

    /**
     * ~Menu()
     *
     * Release resources for the menu.
     **/
    ~Menu();

    /**
     * addItem()
     *
     * Add a menu item as an option.
     *
     * @param text The text to be displayed in the drop down menu.
     * @param cmd The command to be executed.
     * @return True if successful, otherwise false.
     **/
    bool addItem(std::string text, std::string cmd);

    /**
     * setState()
     *
     * Inform the menu that it should update the internal state.
     *
     * @param fState True if menu focused, otherwise false.
     * @param aState True if menu active, otherwise false.
     * @param x The X location of the mouse.
     * @param y The Y location of the mouse.
     **/
    void setState(bool fState, bool aState, int x, int y);

    /**
     * draw()
     *
     * Render to the window graphics.
     *
     * @param dis The display.
     **/
    void draw(Display* dis);
};
