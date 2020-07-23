#include "toolbar.hh"

#define LOG_NAME "toolbar"

#include "git-rev.hh"
#include "icon.cc"
#include "log.hh"

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
  /* Start toolbar */
  LOG("Starting okawm toolbar...");
  Toolbar* toolbar = new Toolbar();
  /* Exit success */
  return 0;
}

Toolbar::Toolbar(){
  /* Create the X connection */
  dis = XOpenDisplay(NULL);
  if(!dis){
    WARN("Unable to open display, program may crash");
  }
  screen = DefaultScreen(dis);
  int width = XWidthOfScreen(ScreenOfDisplay(dis, screen));
  /* Get default colours from X11 */
  unsigned long black = BlackPixel(dis, screen);
  unsigned long white = WhitePixel(dis, screen);
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
    32,
    0,
    CopyFromParent,
    CopyFromParent,
    CopyFromParent,
    CWOverrideRedirect,
    &attr
  );
  /* Set window properties */
  XSetWindowBackground(dis, win, white);
  /* Select allowed input methods */
  XSelectInput(
    dis,
    win,
    ButtonPressMask   |
    ButtonReleaseMask |
    ExposureMask      |
    PointerMotionMask |
    VisibilityChangeMask
  );
  /* Create graphics context */
  gc = XCreateGC(dis, win, 0, 0);
  /* Set window colours */
  XSetBackground(dis, gc, white);
  XSetForeground(dis, gc, black);
  /* Clear the window and bring it on top of the other windows */
  XClearWindow(dis, win);
  XMapRaised(dis, win);
  /* Generate some icons */
  icons.emplace_back(new Icon("res/plus.ppm", 0, 0, true, dis, win, gc));
  icons.emplace_back(new Icon("res/terminal.ppm", 32, 0, true, dis, win, gc));
  icons.emplace_back(new Icon("res/wifi.ppm", width - 160, 0, true, dis, win, gc));
  icons.emplace_back(new Icon("res/volume-up.ppm", width - 128, 0, true, dis, win, gc));
  icons.emplace_back(new Icon("res/battery-full.ppm", width - 96, 0, false, dis, win, gc));
  icons.emplace_back(new Icon("res/wrench.ppm", width - 64, 0, true, dis, win, gc));
  icons.emplace_back(new Icon("res/power.ppm", width - 32, 0, true, dis, win, gc));
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
    /* Get window events */
    XNextEvent(dis, &event);
    /* Check and handle event type */
    int mouseX = -1;
    int mouseY = -1;
    int press = false;
    int type = event.type;
    switch(type){
      case Expose :
        if(event.xexpose.count == 0){
          redraw();
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
        press = event.xmotion.state != 0;
        break;
      default :
        WARN("Unhandled event triggered");
        break;
    }
    /* Update icon states if mouse event occurred */
    if(mouseX >= 0 && mouseY >= 0){
      for(int i = 0; i < icons.size(); i++){
        if(icons[i]->interactive() && icons[i]->insideBounds(mouseX, mouseY)){
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
      /* As we moused over something, redraw to be safe */
      redraw();
    }
    /* TODO: Handle main logic. */
  }
}

void Toolbar::redraw(){
  /* Loop icons */
  for(int x = 0; x < icons.size(); x++){
    icons[x]->draw(dis, win, gc);
  }
}
