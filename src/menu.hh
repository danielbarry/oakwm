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
    enum Type{
      NONE,
      WINDOWS
    };

    struct Item{
      std::string text;
      std::string cmd;
      Window win;
    };

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

  public:
    /**
     * Menu()
     *
     * Setup the menu with options.
     *
     * @param name The name of the menu (should match the icon).
     * @param xOff The Y offset to start drawing the menu.
     * @param yOff The Y offset to start drawing the menu.
     * @param maxWidth The maximum width of the component.
     * @param bg Set the background colour for the menu.
     * @param fg Set the foreground colour for the menu.
     * @param hc Set the highlight colour of the menu.
     * @param strFont The font to be used for the drop down.
     * @param max The maximum number of items to be displayed.
     * @param display The display to show the window on.
     * @param disScreen The screen to show the window on.
     **/
    Menu(
      std::string name,
      int xOff,
      int yOff,
      int maxWidth,
      unsigned long bg,
      unsigned long fg,
      unsigned long hc,
      XFontStruct* strFont,
      int max,
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
