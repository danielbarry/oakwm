#include "toolbar.hh"

#include "log.hh"
#include "version.hh"

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
  LOGM("Build SHA1: ", Version::GIT_SHA1.c_str());
  LOGM("Build Date: ", Version::GIT_DATE.c_str());
  LOGM("Build Subject: ", Version::GIT_COMMIT_SUBJECT.c_str());
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
  /* Get default colours from X11 */
  unsigned long black = BlackPixel(dis, screen);
  unsigned long white = WhitePixel(dis, screen);
  /* Setup window attributes (remove border) */
  XSetWindowAttributes attr;
  attr.override_redirect = 1;
  /* Create the window for the given display */
  win = XCreateWindow(
    dis,
    RootWindow(dis, 0),
    0,
    0,
    XWidthOfScreen(ScreenOfDisplay(dis, 0)),
    16,
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
    ExposureMask    |
    ButtonPressMask |
    KeyPressMask    |
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
  /* Enter main loop */
  loop();
}

Toolbar::~Toolbar(){
  /* Do nothing */
}

void Toolbar::loop(){
  XEvent event;
  while(true){
    /* Get window events */
    XNextEvent(dis, &event);
    /* Do we need to redraw the window? */
    if(event.type == Expose && event.xexpose.count == 0){
      /* TODO: Redraw window information. */
      LOG("Requested to redraw the window");
    }
    /* Did somebody try to overwrite us? */
    if(event.type == VisibilityNotify){
      /* TODO: This is a hack, two or more windows doing this would fight. */
      XRaiseWindow(dis, win);
      XFlush(dis);
    }
    /* TODO: Handle inputs. */
    /* TODO: Handle main logic. */
  }
}
