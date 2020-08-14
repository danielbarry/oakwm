#include "toolbar.hh"

#define LOG_NAME "toolbar"

#include "git-rev.hh"
#include "icon.cc"
#include "log.hh"
#include "menu.cc"
#include "util.hh"

#include <ctime>
#include <unistd.h>

/**
 * main()
 *
 * The main entry point into the program.
 *
 * @param arc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return The return code of the program.
 **/
int main(int argc, char **argv){
  /* Dump version information */
  LOGM("Build Date: ",   STR(BUILD_DATE));
  LOGM("Build Branch: ", STR(GIT_BRANCH_RAW));
  LOGM("Build Hash: ",   STR(GIT_HASH_RAW));
  /* Check command line parameters */
  char* config = NULL;
  if(argc == 2){
    config = argv[1];
  }
  /* Start toolbar */
  LOG("Starting okawm toolbar...");
  Toolbar* toolbar = new Toolbar(config);
  /* Exit success */
  return 0;
}

Toolbar::Toolbar(char* filename){
  /* Get settings if provided */
  if(filename != NULL){
    LOG("Loading configuration...");
    json = JSON::build(filename);
  }else{
    LOG("No configuration provided");
    json = new JSON("{}");
  }
  /* Create the X connection */
  dis = XOpenDisplay(NULL);
  if(!dis){
    WARN("Unable to open display, program may crash");
  }
  screen = DefaultScreen(dis);
  eventDelay = std::atol(json->get("delay-usec")->value("50000").c_str());
  width = XWidthOfScreen(ScreenOfDisplay(dis, screen));
  height = std::atoi(json->get("toolbar")->get("height")->value("32").c_str());
  /* Get default colours from X11 */
  background = Util::strToLong(
    json->get("colours")->get("background")->value("FFFFFF").c_str(),
    16
  );
  foreground = Util::strToLong(
    json->get("colours")->get("foreground")->value("000000").c_str(),
    16
  );
  /* Setup window attributes (remove border) */
  XSetWindowAttributes attr;
  attr.override_redirect = 1;
  /* Create the window for the given display */
  win = XCreateWindow(
    dis,
    RootWindow(dis, screen),
    0,
    0,
    width,
    height,
    0,
    CopyFromParent,
    CopyFromParent,
    CopyFromParent,
    CWOverrideRedirect,
    &attr
  );
  /* Set window properties */
  XSetWindowBackground(dis, win, background);
  /* Select allowed input methods */
  eventMask =
    ButtonPressMask   |
    ButtonReleaseMask |
    ExposureMask      |
    PointerMotionMask |
    VisibilityChangeMask;
  XSelectInput(
    dis,
    win,
    eventMask
  );
  /* Create graphics context */
  gc = XCreateGC(dis, win, 0, 0);
  /* Set window colours */
  XSetBackground(dis, gc, background);
  XSetForeground(dis, gc, foreground);
  /* Clear the window and bring it on top of the other windows */
  XClearWindow(dis, win);
  XMapRaised(dis, win);
  /* Generate some icons from the configuration */
  int leftOff = 0;
  int rightOff = width;
  for(int x = 0; x < json->get("toolbar")->get("icons")->length(); x++){
    JSON* iCfg = json->get("toolbar")->get("icons")->get(x);
    /* Build icon */
    bool alignLeft = iCfg->get("align")->value("left").compare("left") == 0;
    Icon* icon = new Icon(
      iCfg->get("image")->value(""),
      iCfg->get("name")->value(""),
      0,
      0,
      iCfg->get("interactive")->value("false").compare("true") == 0,
      new Menu(
        std::atoi(json->get("menu")->get("max-items")->value("16").c_str())
      ),
      dis,
      win,
      gc
    );
    /* Icon alignment */
    if(alignLeft){
      icon->setXY(leftOff, 0);
      leftOff += icon->getWidth();
    }else{
      rightOff -= icon->getWidth();
      icon->setXY(rightOff, 0);
    }
    /* Build menu */
    for(int z = 0; z < iCfg->get("menu")->length(); z++){
      /* TODO: Add menu items. */
    }
    /* Add to data structure */
    icons.emplace_back(icon);
  }
  activeIcon = NULL;
  /* Apply icon modifiers */
  for(int x = 0; x < json->get("toolbar")->get("modifiers")->length(); x++){
    JSON* mCfg = json->get("toolbar")->get("modifiers")->get(x);
    std::string name = mCfg->get("name")->value("");
    unsigned long mask = Util::strToLong(mCfg->get("mask")->value("").c_str(), 16);
    /* Update our icons with modifiers */
    for(int z = 0; z < icons.size(); z++){
      icons[z]->addModifier(name, mask, dis);
    }
  }
  /* Load font for text */
  font = XLoadQueryFont(
    dis,
    json->get("toolbar")->get("font")->value("fixed").c_str()
  );
  if(font != 0){
    XSetFont(dis, gc, font->fid);
  }else{
    WARN("Unable to load font");
  }
  /* Enter main loop */
  loop();
}

Toolbar::~Toolbar(){
  XFreeGC(dis, gc);
  XDestroyWindow(dis, win);
  XCloseDisplay(dis);
  exit(1);
}

void Toolbar::loop(){
  XEvent event;
  while(true){
    bool requestRedraw = LOG::CURRENT_TIME_MINUTES() > lastTimeUpdate;
    /* Get window events */
    if(
      !XCheckMaskEvent(dis, eventMask, &event) &&
      !requestRedraw
    ){
      usleep(eventDelay);
      continue;
    }
    /* Check and handle event type */
    int mouseX = -1;
    int mouseY = -1;
    int press = false;
    int type = event.type;
    switch(type){
      case Expose :
        if(event.xexpose.count == 0){
          requestRedraw = true;
        }
        break;
      case VisibilityNotify :
        /* TODO: This is a hack, two or more windows doing this would fight. */
        XRaiseWindow(dis, win);
        XFlush(dis);
        break;
      case ButtonPress :
      case ButtonRelease :
        mouseX = event.xbutton.x;
        mouseY = event.xbutton.y;
        press = type == ButtonPress;
        break;
      case MotionNotify :
        mouseX = event.xmotion.x;
        mouseY = event.xmotion.y;
        press = event.xmotion.state &
          (Button1Mask | Button2Mask | Button3Mask | Button4Mask | Button5Mask);
        break;
      default :
        WARN("Unhandled event triggered");
        break;
    }
    /* Update icon states if mouse event occurred */
    if(mouseX >= 0 && mouseY >= 0){
      Icon* prevActiveIcon = activeIcon;
      activeIcon = NULL;
      /* Update icons */
      for(int i = 0; i < icons.size(); i++){
        if(icons[i]->interactive() && icons[i]->insideBounds(mouseX, mouseY)){
          activeIcon = icons[i];
          /* Is it hover of press related? */
          if(type == MotionNotify && !press){
            icons[i]->setFocused(true);
          }else{
            icons[i]->setActive(press);
          }
        }else{
          icons[i]->setFocused(false);
        }
      }
      /* Update drop down menu */
      if(press){
        if(activeIcon != NULL){
          LOGM("Press detected for ", activeIcon->getName().c_str()); // TODO
          /* TODO: Create new window if required. */
          /* TODO: Highlight option if required. */
        }
      /* Update drop down on release */
      }else{
        LOG("Release detected"); // TODO
        if(prevActiveIcon != NULL){
          LOGM("Release for ", prevActiveIcon->getName().c_str()); // TODO
          /* TODO: Execute command if required. */
        }
      }
      /* As we moused over something, redraw to be safe */
      requestRedraw = true;
    }
    /* Perform one redraw if requested */
    if(requestRedraw){
      redraw();
    }
  }
}

void Toolbar::redraw(){
  /* Store current time */
  lastTimeUpdate = LOG::CURRENT_TIME_MINUTES();
  /* Loop icons */
  for(int x = 0; x < icons.size(); x++){
    icons[x]->draw(dis, win, gc);
  }
  /* Clear previous date string */
  int timeWidth = XTextWidth(font, tBuff, strlen(tBuff));
  int timeHeight = font->ascent + font->descent;
  XSetForeground(dis, gc, background);
  XFillRectangle(
    dis,
    win,
    gc,
    (width / 2) - (timeWidth / 2),
    0,
    timeWidth,
    height
  );
  XSetForeground(dis, gc, foreground);
  /* Get time string */
  time_t rawTime;
  struct tm* tInfo;
  time(&rawTime);
  tInfo = localtime(&rawTime);
  strftime(tBuff, sizeof(tBuff), "%d %b %R", tInfo);
  /* Draw time */
  timeWidth = XTextWidth(font, tBuff, strlen(tBuff));
  timeHeight = font->ascent + font->descent;
  XDrawString(
    dis,
    win,
    gc,
    (width / 2) - (timeWidth / 2),
    (height / 2) + (timeHeight / 2),
    tBuff,
    strlen(tBuff)
  );
}
