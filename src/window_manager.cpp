#include "window_manager.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <csignal>
#include <cstdlib>
#include <glog/logging.h>
#include <memory>
#include <iostream>

using ::std::unique_ptr;

#define totalKeys 1
const char keyBindings[totalKeys][2] = {'D'};
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

void WindowManager::keypress(const XKeyEvent &e)
{
  if(e.state & Mod1Mask)
  {
	if(isKey(keyBindings[0]))
	{
	 // std::cout << "starting up dmenu";
	 // system("dmenu_run&");
	 XDestroyWindow(display_, e.window);
	}
  }
}

void WindowManager::frame(Window w)
{
  const unsigned int border_width = 3;
  const unsigned long border_color = 0xffffff;
  const unsigned long bg_color = 0x000000; 

  XWindowAttributes x_window_attrs;
  CHECK(XGetWindowAttributes(display_, w, &x_window_attrs));

  const Window frame = XCreateSimpleWindow(display_,
	  root_,
	  x_window_attrs.x,
	  x_window_attrs.y,
	  x_window_attrs.width,
	  x_window_attrs.height,
	  border_width,
	  border_color,
	  bg_color);

  XSelectInput(display_, w, SubstructureNotifyMask | SubstructureRedirectMask);

  XAddToSaveSet(display_, w);

  XReparentWindow(display_, w, frame, 0, 0);

  XMapWindow(display_, frame);
}

void WindowManager::on_map_request(const XMapRequestEvent &e)
{
  frame(e.window);
  
  XMapWindow(display_, e.window);
}


void WindowManager::Run() {
  std::cout << "Running\n";

  wm_detected_ = false;

  XSetErrorHandler(&WindowManager::on_wm_detected);
  XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask);

  XSync(display_, false);

  if (wm_detected_) {
	LOG(ERROR) << "[Error] Another WM detected " << XDisplayString(display_);

	return;
  }

  XSetErrorHandler(&WindowManager::on_x_err);

  while (1) {
	std::cout << "entered the main event loop \n";
	XEvent e;
	XNextEvent(display_,&e);

	switch (e.type)
	{
	  case CreateNotify:
		break;
	  case ConfigureRequest:
		break;
	  case MapRequest:
		on_map_request(e.xmaprequest);
		break;
	  case DestroyNotify:
		break;
	  case KeyPress:
		LOG(INFO)<<"key pressed";
		keypress(e.xkey);
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
