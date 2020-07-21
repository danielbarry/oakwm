#include "toolbar.hh"

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
  Toolbar* toolbar = new Toolbar();
  /* Exit success */
  return 0;
}

Toolbar::Toolbar(){
  /* Create the X connection */
  dis = XOpenDisplay(NULL);
  screen = DefaultScreen(dis);
  /* Get default colours from X11 */
  unsigned long black = BlackPixel(dis, screen);
  unsigned long white = WhitePixel(dis, screen);
  /* Create the window for the given display */
  win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 200, 300, 5, white, black);
  /* Set window properties */
  XSetStandardProperties(dis, win, "My Window", "HI!", None, NULL, 0, NULL);
  /* Select allowed input methods */
  XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
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
      redraw();
    }
    /* TODO: Handle inputs. */
    /* TODO: Handle main logic. */
  }
}