#include <X11/X.h>
extern "C" {
#include <X11/Xlib.h>
};

#include "memory"
#include "workspaces.hpp"

#define isKey(K) e.keycode == XKeysymToKeycode(display_, XStringToKeysym(K))
#define MOD1BIND(K) XGrabKey(display_, XKeysymToKeycode(display_, XStringToKeysym(K)), Mod1Mask, root_, True, GrabModeAsync, GrabModeAsync)
#define SHIFTBIND(K) XGrabKey(display_, XKeysymToKeycode(display_, XStringToKeysym(K)), ShiftMask, root_, True, GrabModeAsync, GrabModeAsync)



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
  void frame(WindowClass &w);
  void keypress(XKeyEvent &e);
  void setkeys();
  void setbuttons();
  void handle_events(XEvent &e);
  void kill_window();
  int manage();
  int on_configure_request(XConfigureRequestEvent &e);
  int on_create_notify(XCreateWindowEvent &e);
  int on_destroy_notify(XDestroyWindowEvent &e);
  int manage_master_stack();
  int manage_tree();
  int manage_centered_master();
  int focus_next();
  int focus_prev();
  int front_move();
  int back_move();
  int unmap_all();
  int next_workspace();
  int prev_workspace();


  Display* display_;
  const Window root_;
  Workspaces workspaces;
  WindowClass focused;
  WindowClass prev_focused;
  void increase_size();
  void decrease_size();
  unsigned int swidth;
  unsigned int sheight;
};

