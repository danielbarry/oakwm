
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONFIG_NICE_FONT	  ((char*)"-*-lucida-bold-r-*-*-14-*-75-75-*-*-*-*")
#define CONFIG_NICE_MENU_FONT	  ((char*)"-*-lucida-medium-r-*-*-14-*-75-75-*-*-*-*")
#define CONFIG_NASTY_FONT	  ((char*)"fixed")

#define CONFIG_EXEC_USING_SHELL   False
#define CONFIG_NEW_WINDOW_COMMAND "xterm"
#define CONFIG_EVERYTHING_ON_ROOT_MENU False

// You can't have CLICK_TO_FOCUS without RAISE_ON_FOCUS but the other
// combinations should be okay.  If you set AUTO_RAISE you must leave
// the other two False; you'll then get focus-follows, auto-raise, and
// a delay on auto-raise as configured in the DELAY settings below.
#define CONFIG_CLICK_TO_FOCUS     True
#define CONFIG_RAISE_ON_FOCUS     True
#define CONFIG_AUTO_RAISE         False

// milliseconds.  In theory these only apply when using AUTO_RAISE,
// not when just using RAISE_ON_FOCUS without CLICK_TO_FOCUS.  First
// of these is the usual delay before raising; second is the delay
// after the pointer has stopped moving (only applicable over simple X
// windows such as xvt).
#define CONFIG_AUTO_RAISE_DELAY       400
#define CONFIG_POINTER_STOPPED_DELAY  80
#define CONFIG_DESTROY_WINDOW_DELAY   1500L

#define CONFIG_TAB_FOREGROUND	  ((char*)"black")
#define CONFIG_TAB_BACKGROUND     ((char*)"gray80")
#define CONFIG_FRAME_BACKGROUND   ((char*)"gray95")
#define CONFIG_BUTTON_BACKGROUND  ((char*)"gray95")
#define CONFIG_BORDERS            ((char*)"black")

#define CONFIG_MENU_FOREGROUND    ((char*)"black")
#define CONFIG_MENU_BACKGROUND    ((char*)"gray80")
#define CONFIG_MENU_BORDERS       ((char*)"black")

#define CONFIG_FRAME_THICKNESS    7

// If CONFIG_PROD_SHAPE is True, all frame element shapes will be
// recalculated afresh every time their focus changes.  This will
// probably slow things down hideously, but has been reported as
// necessary on some systems (possibly SunOS 4.x with OpenWindows).
#define CONFIG_PROD_SHAPE False

#endif

