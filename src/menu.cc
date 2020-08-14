#include "menu.hh"

Menu::Menu(
  int xOff,
  int yOff,
  int maxWidth,
  unsigned long bg,
  unsigned long fg,
  int max,
  Display* display,
  int disScreen
){
  xOffset = xOff;
  yOffset = yOff;
  width = maxWidth;
  background = bg;
  foreground = fg;
  maxItems = max;
  dis = display;
  screen = disScreen;
  win = NULL;
  gc = NULL;
}

Menu::~Menu(){
  if(win != NULL){
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    win = NULL;
  }
}

void Menu::setXY(int x, int y){
  xOffset = x;
  yOffset = y;
}

bool Menu::addItem(std::string text, std::string cmd){
  if(items.size() + 1 >= maxItems){
    return false;
  }
  items.emplace_back(Item());
  items[items.size() - 1].text = text;
  items[items.size() - 1].cmd = cmd;
  return true;
}

bool Menu::setState(bool fState, bool aState, int x, int y){
  focus = fState;
  active = aState;
  /* TODO: Check if highlighted has changed. */
  /* TODO: Check if we should execute something. */
  if(win != NULL && !focus && !active){
    /* TODO: Execute command. */
  }
  return false;
}

void Menu::draw(Display* dis){
  if(focus || active){
    if(!focus){
      /* Open window if not open */
      if(win == NULL){
        /* Calculate X offset of the drop down */
        int winWidth = XWidthOfScreen(ScreenOfDisplay(dis, screen));
        int x = xOffset - (width / 2);
        if(x < 0){
          x = 0;
        }
        if(x + width >= winWidth){
          x = winWidth - width;
        }
        /* Setup window attributes (remove border) */
        XSetWindowAttributes attr;
        attr.override_redirect = 1;
        /* Create the window for the given display */
        win = XCreateWindow(
          dis,
          RootWindow(dis, screen),
          x,
          yOffset,
          width,
          /* TODO: Based on text size. */
          100,
          0,
          CopyFromParent,
          CopyFromParent,
          CopyFromParent,
          CWOverrideRedirect,
          &attr
        );
        /* Set window properties */
        XSetWindowBackground(dis, win, background);
        /* Create graphics context */
        gc = XCreateGC(dis, win, 0, 0);
        /* Set window colours */
        XSetBackground(dis, gc, background);
        XSetForeground(dis, gc, foreground);
        /* Clear the window and bring it on top of the other windows */
        XClearWindow(dis, win);
        XMapRaised(dis, win);
      }
      /* TODO: Highlight selection if applicable. */
      /* TODO: This is a hack, two or more windows doing this would fight. */
      XRaiseWindow(dis, win);
      XFlush(dis);
    }
  }else{
    /* Close window and free resource if it's open */
    if(win != NULL){
      XFreeGC(dis, gc);
      XDestroyWindow(dis, win);
      win = NULL;
    }
  }
}
