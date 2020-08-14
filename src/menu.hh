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

    int maxItems;
    std::vector<Item> items;

  public:
    /**
     * Menu()
     *
     * Setup the menu with options.
     *
     * @param max The maximum number of items to be displayed.
     **/
    Menu(int max);

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
     * focusItem()
     *
     * Allow an item to be focused based on mouse position.
     *
     * @param x The mouse X location.
     * @param y The mouse Y location.
     **/
    void focusItem(int x, int y);

    /**
     * selectItem()
     *
     * Select an item based on the mouse position.
     *
     * @param x The mouse X location.
     * @param y The mouse Y location.
     **/
    void selectItem(int x, int y);

    /**
     * draw()
     *
     * Render to the window graphics.
     *
     * @param dis The display.
     **/
    void draw(Display* dis);
};
