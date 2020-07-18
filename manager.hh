#pragma once

#include "general.hh"
#include "listmacro2.hh"

class Client;
declarePList(ClientList, Client);

class WindowManager{
  public:
    /**
     * WindowManager()
     *
     * Start the window manager, setup internal variables, check configuration
     * is correct, setup X11 and run main loop.
     **/
    WindowManager();

    /**
     * ~WindowManager
     *
     * Release allocated resources.
     **/
    ~WindowManager();

    /**
     * fatal()
     *
     * Display message and return with exit code.
     *
     * @param message The error message to be displayed.
     **/
    void fatal(const char* message);

    /**
     * windowToClient()
     *
     * Convert a window to a client, or create a client if requested.
     *
     * @param w The window to be searched and have a client generated for.
     * @param create If true, create a client if one has not been created.
     * Otherwise return NULL.
     * @return The client mapped to the given window, otherwise NULL.
     **/
    Client* windowToClient(Window w, Boolean create = False);

    /**
     * activeClient()
     * TODO
     **/
    Client* activeClient(){
      return m_activeClient;
    }

    /**
     * raiseTransients()
     * TODO
     **/
    Boolean raiseTransients(Client*); // true if raised any

    /**
     * timestamp()
     * TODO
     **/
    Time timestamp(Boolean reset);

    /**
     * clearFocus()
     * TODO
     **/
    void clearFocus();

    /**
     * setActiveClient()
     * TODO
     **/
    void setActiveClient(Client* const c){
      m_activeClient = c;
    }

    /**
     * addToHiddenList()
     * TODO
     **/
    void addToHiddenList(Client*);

    /**
     * removeFromHiddenList()
     * TODO
     **/
    void removeFromHiddenList(Client*);

    /**
     * skipInRevert()
     * TODO
     **/
    void skipInRevert(Client*, Client*);

    /**
     * display()
     * TODO
     **/
    Display* display(){
      return m_display;
    }

    /**
     * root()
     * TODO
     **/
    Window root(){
      return m_root;
    }

    /**
     * RootCursor()
     * TODO
     **/
    enum RootCursor{
      NormalCursor, DeleteCursor, DownCursor, RightCursor, DownrightCursor
    };

    /**
     * installCursor()
     * TODO
     **/
    void installCursor(RootCursor);

    /**
     * installCursorOnWindow()
     * TODO
     **/
    void installCursorOnWindow(RootCursor, Window);

    /**
     * installColormap()
     * TODO
     **/
    void installColormap(Colormap);

    /**
     * allocateColour()
     * TODO
     **/
    unsigned long allocateColour(char*, char*);

    /**
     * considerFocusChange()
     * TODO
     **/
    void considerFocusChange(Client*, Window, Time timestamp);

    /**
     * stopConsideringFocus()
     * TODO
     **/
    void stopConsideringFocus();

    /**
     * attemptGrab()
     * TODO
     **/
    // shouldn't really be public
    int attemptGrab(Window, Window, int, int);

    /**
     * releaseGrab()
     * TODO
     **/
    void releaseGrab(XButtonEvent*);

    /**
     * exentExposure()
     * TODO
     **/
    void eventExposure(XExposeEvent*); // for exposures during client grab

    /**
     * showGeometry()
     * TODO
     **/
    void showGeometry(int, int);

    /**
     * removeGeometry()
     * TODO
     **/
    void removeGeometry();

  private:
    Display *m_display;
    int m_screenNumber;
    Window m_root;
    Colormap m_defaultColormap;
    int m_minimumColormaps;
    Cursor m_cursor;
    Cursor m_xCursor;
    Cursor m_vCursor;
    Cursor m_hCursor;
    Cursor m_vhCursor;
    char *m_terminal;
    char *m_shell;
    ClientList m_clients;
    ClientList m_hiddenClients;
    Client *m_activeClient;
    int m_shapeEvent;
    int m_currentTime;
    Boolean m_looping;
    int m_returnCode;
    static Boolean m_initialising;
    GC m_menuGC;
    Window m_menuWindow;
    XFontStruct *m_menuFont;
    unsigned long m_menuForegroundPixel;
    unsigned long m_menuBackgroundPixel;
    unsigned long m_menuBorderPixel;
    static const char *const m_menuCreateLabel;
    static int m_signalled;
    Boolean m_focusChanging; // checking times for focus change
    Client* m_focusCandidate;
    Window m_focusCandidateWindow;
    Time m_focusTimestamp; // time of last crossing event
    Boolean m_focusPointerMoved;
    Boolean m_focusPointerNowStill;

    /**
     * loop()
     * TODO
     **/
    int loop();

    /**
     * release()
     * TODO
     **/
    void release();

    /**
     * errorHandler()
     * TODO
     **/
    static int errorHandler(Display*, XErrorEvent*);

    /**
     * sigHandler()
     * TODO
     **/
    static void sigHandler();

    /**
     * initialiseScreen()
     * TODO
     **/
    void initialiseScreen();

    /**
     * scanInitialWindows()
     * TODO
     **/
    void scanInitialWindows();

    /**
     * menuLabel()
     * TODO
     **/
    const char *const menuLabel(int);

    /**
     * menu()
     * TODO
     **/
    void menu(XButtonEvent*);

    /**
     * spawn()
     * TODO
     **/
    void spawn();

    /**
     * circulate()
     * TODO
     **/
    void circulate(Boolean activeFirst);

    /**
     * checkDelaysForFocus()
     * TODO
     **/
    void checkDelaysForFocus();

    /**
     * nextEvent()
     * TODO
     **/
    void nextEvent(XEvent*); // return

    /**
     * eventButton()
     * TODO
     **/
    void eventButton(XButtonEvent*);

    /**
     * eventMapRequest()
     * TODO
     **/
    void eventMapRequest(XMapRequestEvent*);

    /**
     * eventConfigureRequest()
     * TODO
     **/
    void eventConfigureRequest(XConfigureRequestEvent*);

    /**
     * eventUnmap()
     * TODO
     **/
    void eventUnmap(XUnmapEvent*);

    /**
     * eventCreate()
     * TODO
     **/
    void eventCreate(XCreateWindowEvent*);

    /**
     * eventDestroy()
     * TODO
     **/
    void eventDestroy(XDestroyWindowEvent*);

    /**
     * eventClient()
     * TODO
     **/
    void eventClient(XClientMessageEvent*);

    /**
     * eventColormap()
     * TODO
     **/
    void eventColormap(XColormapEvent*);

    /**
     * eventProperty()
     * TODO
     **/
    void eventProperty(XPropertyEvent*);

    /**
     * eventEnter()
     * TODO
     **/
    void eventEnter(XCrossingEvent*);

    /**
     * eventReparent()
     * TODO
     **/
    void eventReparent(XReparentEvent*);

    /**
     * eventFocusIn()
     * TODO
     **/
    void eventFocusIn(XFocusInEvent*);
};
