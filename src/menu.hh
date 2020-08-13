#pragma once

/**
 * menu.hh
 *
 * Display status and buttons on an X11 window.
 **/
class Menu{
  public:
    /**
     * Menu()
     *
     * Setup the menu with options.
     *
     * @param maxItems The maximum number of items to be displayed.
     **/
    Menu(int maxItems);

    /**
     * ~Menu()
     *
     * Release resources for the menu.
     **/
    ~Menu();
};
