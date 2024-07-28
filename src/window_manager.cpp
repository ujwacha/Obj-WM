#include "window_manager.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <glog/logging.h>
#include <memory>
#include <iostream>

using ::std::unique_ptr;

bool WindowManager::wm_detected_ = false;

unique_ptr<WindowManager> WindowManager::Create() {
  Display* display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    std::cout << "Failed to open X display " << XDisplayName(nullptr);
    return nullptr;
  }
  return unique_ptr<WindowManager>(new WindowManager(display));
}

WindowManager::WindowManager(Display *display)
    : display_(CHECK_NOTNULL(display)), root_(DefaultRootWindow(display_)) {}

WindowManager::~WindowManager() {
  XCloseDisplay(display_);
}


int WindowManager::on_wm_detected(Display *display, XErrorEvent *event) {
  if (static_cast<int>(event->error_code) == BadAccess) {
    std::cout << "[Error] another WM detected\n" ;
  }

  wm_detected_ = true;
  
  return 0;
}

int WindowManager::on_x_err(Display* display, XErrorEvent* event) {

  char code = event->error_code;
  std::cout << "[Error] Error Code Recieved: " << code;
  return -1;
}



void WindowManager::Run() {
  std::cout << "Running\n";

  wm_detected_ = false;

  XSetErrorHandler(&WindowManager::on_wm_detected);
  XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask);

  XSync(display_, false);

  if (wm_detected_) {
    std::cout << "[Error] Another WM detected " << XDisplayString(display_) << std::endl;

    return;
  }

  XSetErrorHandler(&WindowManager::on_x_err);

  while (1) {
    XEvent e;


  switch (e.type)
  {
  case ConfigureRequest:
    break;
  case MapRequest:
    break;
  case DestroyNotify:
    break;
  case KeyPress:
    break;
  case FocusIn:
    printf("focusin event just came, 0x%lx gained focus \n\n\n", e.xfocus.window);
    break;
  case FocusOut:
    printf("focusout event just came, 0x%lx gained focus \n\n\n", e.xfocus.window);
    break;
  case MotionNotify:
  case ButtonPress:
    break;
  case ButtonRelease:
    break;
  }

  XSync(display_, False);
			       
  }
    
}
