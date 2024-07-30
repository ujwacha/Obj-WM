#include <X11/X.h>
extern "C" {
#include <X11/Xlib.h>
};

#include "memory"
#include "workspaces.hpp"

#define isKey(K) e.keycode == XKeysymToKeycode(display_, XStringToKeysym(K))
#define MOD1BIND(K) XGrabKey(display_, XKeysymToKeycode(display_, XStringToKeysym(K)), Mod1Mask, root_, True, GrabModeAsync, GrabModeAsync)



class WindowManager {
public:
  static ::std::unique_ptr<WindowManager> Create();

  ~WindowManager();

  void Run();

  static bool wm_detected_ ;
private:
  WindowManager(Display* display);
  static int on_x_err(Display* display, XErrorEvent* e);
  static int on_wm_detected(Display* display, XErrorEvent* e);
  void on_map_request(XMapRequestEvent &e);
  void frame(Window &w);
  void keypress(XKeyEvent &e);
  void setkeys();
  void setbuttons();
  void handle_events(XEvent &e);
  void kill_window(Window &w);


  Display* display_;
  const Window root_;
  Workspaces workspaces;
  Window focused;
};

