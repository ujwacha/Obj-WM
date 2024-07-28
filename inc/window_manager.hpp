extern "C" {
#include <X11/Xlib.h>
};

#include "memory"
#include "workspaces.hpp"

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

  Display* display_;
  const Window root_;
  Workspaces workspaces;
};

