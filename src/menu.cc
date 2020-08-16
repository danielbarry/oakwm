#include "menu.hh"

Menu::Menu(
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
){
  mName = name;
  xOffset = xOff;
  yOffset = yOff;
  width = maxWidth;
  background = bg;
  foreground = fg;
  highlight = hc;
  font = strFont;
  textHeight = font->ascent + font->descent;
  maxItems = max;
  select = -1;
  dis = display;
  screen = disScreen;
  win = NULL;
  gc = NULL;
  focus = false;
  active = false;
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

bool Menu::addItem(std::string text, std::string cmd, Window win){
  if(items.size() + 1 >= maxItems){
    return false;
  }
  items.emplace_back(Item());
  items[items.size() - 1].text = text;
  items[items.size() - 1].cmd = cmd;
  items[items.size() - 1].win = win;
  return true;
}

bool Menu::setState(bool fState, bool aState, int x, int y){
  focus = fState;
  active = aState;
  y -= yOffset;
  /* Check if highlighted has changed */
  int prev = select;
  if(y >= 0 && y < textHeight * items.size()){
    select = y / textHeight;
  }else{
    select = -1;
  }
  /* Check if we should execute command */
  if(win != NULL && focus && !active && select >= 0){
    system(items[select].cmd.c_str());
  }
  /* Tell the caller if something changed */
  return select != prev;
}

void Menu::draw(Display* dis){
  if(active){
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
        (textHeight * items.size()) + 2,
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
      /* Set the font to be used */
      XSetFont(dis, gc, font->fid);
    }
    /* Loop over items to be printed */
    for(int x = 0; x < items.size(); x++){
      /* Setup colours for drop down menu */
      if(select != x){
        XSetForeground(dis, gc, background);
        XFillRectangle(dis, win, gc, 0, x * textHeight, width, textHeight);
        XSetForeground(dis, gc, foreground);
      }else{
        XSetForeground(dis, gc, highlight);
        XFillRectangle(dis, win, gc, 0, x * textHeight, width, textHeight);
        XSetForeground(dis, gc, background);
      }
      /* Draw time */
      XDrawString(
        dis,
        win,
        gc,
        (width - XTextWidth(font, items[x].text.c_str(), items[x].text.length())) / 2,
        (textHeight * (x + 1)) - font->descent,
        items[x].text.c_str(),
        items[x].text.length()
      );
      /* Reset foreground colour */
      XSetForeground(dis, gc, foreground);
    }
    /* TODO: This is a hack, two or more windows doing this would fight. */
    XRaiseWindow(dis, win);
    XFlush(dis);
  }else{
    /* Close window and free resource if it's open */
    if(win != NULL){
      XFreeGC(dis, gc);
      XDestroyWindow(dis, win);
      win = NULL;
    }
  }
}
