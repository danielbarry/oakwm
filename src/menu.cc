#include "menu.hh"

#include "util.hh"

Menu::Menu(
  JSON* cfg,
  JSON* iCfg,
  XFontStruct* strFont,
  Display* display,
  int disScreen
){
  config = cfg;
  mName = iCfg->get("name")->value("");
  type = Type::NORMAL;
  if(mName.compare("Network") == 0){
    type = Type::NETWORK;
  }
  if(mName.compare("Windows") == 0){
    type = Type::WINDOWS;
  }
  xOffset = 0;
  yOffset = 0;
  width = std::atoi(cfg->get("menu")->get("max-width")->value("128").c_str());
  background = Util::strToLong(
    cfg->get("colours")->get("background")->value("FFFFFF").c_str(),
    16
  );
  foreground = Util::strToLong(
    cfg->get("colours")->get("foreground")->value("000000").c_str(),
    16
  );
  highlight = Util::strToLong(
    cfg->get("colours")->get("highlight")->value("888888").c_str(),
    16
  );
  font = strFont;
  textHeight = font->ascent + font->descent;
  maxItems = std::atoi(cfg->get("menu")->get("max-items")->value("16").c_str());
  select = -1;
  dis = display;
  screen = disScreen;
  win = (Window)NULL;
  gc = NULL;
  focus = false;
  active = false;
  /* Build menu */
  for(int z = 0; z < iCfg->get("menu")->length(); z++){
    addItem(
      iCfg->get("menu")->get(z)->get("text")->value("Unknown"),
      iCfg->get("menu")->get(z)->get("command")->value(""),
      (Window)NULL
    );
  }
  pathBatt = cfg->get("power")->get("battery")->value("");
  pathDC = cfg->get("power")->get("dc")->value("");
}

Menu::~Menu(){
  if(win != (Window)NULL){
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    win = (Window)NULL;
  }
}

void Menu::setXY(int x, int y){
  xOffset = x;
  yOffset = y;
}

bool Menu::addItem(std::string text, std::string cmd, Window win){
  /* Make sure we can add */
  if(items.size() + 1 >= maxItems){
    return false;
  }
  /* Create new item */
  items.emplace_back(Item());
  int index = items.size() - 1;
  /* Load in universal values */
  items[index].text = text;
  items[index].cmd = cmd;
  items[index].win = win;
  /* Figure out the type of this item */
  items[index].type = ItemType::NONE;
  if(cmd.size() > 0){
    items[index].type = ItemType::COMMAND;
  }
  if(text.compare("$POWER$") == 0){
    items[index].type = ItemType::POWER;
  }
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
  /* Check if we should perform some action */
  if(win != (Window)NULL && focus && !active && select >= 0){
    Item selection = items[select];
    /* Check if it's to bring a window to the front */
    if(type == Type::WINDOWS){
      XRaiseWindow(dis, selection.win);
      XMapRaised(dis, selection.win);
    }
    /* If command set, run it */
    if(selection.type == ItemType::COMMAND){
      system(selection.cmd.c_str());
    }
  }
  /* Tell the caller if something changed */
  return select != prev;
}

void Menu::draw(Display* dis){
  if(active){
    /* Open window if not open */
    if(win == (Window)NULL){
      /* If we are a special menu, we need to populate the list */
      ifaddrs* ifa;
      switch(type){
        case NETWORK :
          /* Remove any existing */
          items.clear();
          /* TODO: Add list of network devices and their state. */
          ifa = Util::getNetworkState();
          while(ifa != NULL){
            if(
              ifa->ifa_addr->sa_family == AF_INET ||
              ifa->ifa_addr->sa_family == AF_INET6
            ){
              /* Add the item */
              addItem(std::string(ifa->ifa_name), "", (Window)NULL);
            }
            /* Now let's get the next device */
            ifa = ifa->ifa_next;
          }
          break;
        case WINDOWS :
          /* Remove any existing */
          items.clear();
          /* Query list of windows */
          Window wThis = RootWindow(dis, (Window)NULL);
          Window wRoot;
          Window wParent;
          Window* wChilds;
          unsigned int nChilds = 0;
          XQueryTree(dis, wThis, &wRoot, &wParent, &wChilds, &nChilds);
          char* name;
          for(int x = 0; x < nChilds; x++){
            Window* iChilds;
            unsigned int zChilds = 0;
            XQueryTree(dis, wChilds[x], &wRoot, &wParent, &iChilds, &zChilds);
            for(int z = 0; z < zChilds; z++){
              XFetchName(dis, iChilds[z], &name);
              if(name != NULL){
                std::string wName(name);
                XFree(name);
                /* TODO: Move max text length to configuration. */
                /* Make sure we don't have infinitely long window names */
                if(wName.size() > 10){
                  wName = wName.substr(0, 8);
                  wName += "..";
                }
                addItem(wName, "", iChilds[z]);
              }
            }
            XFree(iChilds);
          }
          XFree(wChilds);
          break;
      }
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
      /* Read state if required */
      int cap;
      switch(items[x].type){
        case POWER :
          cap = Util::getPowerState(pathBatt, pathDC);
          if(cap >= 0){
            items[x].text = "Battery: " + std::to_string(cap) + "%";
          }else{
            items[x].text = "Charge: " + std::to_string(-cap) + "%";
          }
          break;
        default :
          /* Do nothing */
          break;
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
    if(win != (Window)NULL){
      XFreeGC(dis, gc);
      XDestroyWindow(dis, win);
      win = (Window)NULL;
    }
  }
}
