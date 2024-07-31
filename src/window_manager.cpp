#include "window_manager.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <algorithm>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <glog/logging.h>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <vector>

using ::std::unique_ptr;
unsigned int swidth;
unsigned int sheight;

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

void WindowManager::kill_window(Window &w)
{
  XDestroyWindow(display_, w);
}

void WindowManager::keypress(XKeyEvent &e)
{
  if(e.state & Mod1Mask)
  {
	if(isKey("D"))
	{
	  std::cout << "starting up dmenu\n";
	  system("dmenu_run&");
	}
	else if (isKey("Q"))
	{
	  XCloseDisplay(display_);
	  //kill_window(e.window);	
	}
  }
}

void WindowManager::setkeys()
{
  MOD1BIND("D");//open dmenu
  MOD1BIND("Q");
}

void WindowManager::frame(Window &w)
{
  return;
}

void WindowManager::on_map_request(XMapRequestEvent &e)
{
  if(e.parent ==  root_)
  {
	WindowClass w = &e.window;
	workspaces.add_window(w);
  }
  manage();
}

void WindowManager::manage()
{
  std::vector<WindowClass>& current_windows = workspaces.get_all_current_windows();
  for(int i=0;i<current_windows.size();i++)
  {
	Window *w = current_windows[i].get_window();
	XMoveResizeWindow(display_, *w, swidth*i/2, sheight*i/2, swidth/2, sheight/2);
	XMapWindow(display_, *w);
  }
}

// Adds mousebutton listeners
void WindowManager::setbuttons()
{
	XGrabButton(display_, 1, Mod1Mask, root_, True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
}

void WindowManager::handle_events(XEvent &e)
{
  std::cout << "event handler called\n";
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
	  std::cout << "keypress request seen";
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
}


void WindowManager::Run() {
  std::cout << "Running\n";
  Screen *scr;
  scr = DefaultScreenOfDisplay(display_);
  sheight = scr->height;
  swidth = scr->width;

  wm_detected_ = false;

  XSetErrorHandler(&WindowManager::on_wm_detected);
  XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask);

  XSync(display_, false);
  setkeys();
  setbuttons();

  if (wm_detected_) {
	LOG(ERROR) << "[Error] Another WM detected " << XDisplayString(display_);

	return;
  }

  XSetErrorHandler(&WindowManager::on_x_err);
  XEvent e;

  for(;;){
	XNextEvent(display_,&e);
	handle_events(e);
	XSync(display_, False);
  }

}
