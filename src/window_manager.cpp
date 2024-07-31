#include "window_manager.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <algorithm>
#include <bits/types/wint_t.h>
#include <climits>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <glog/logging.h>
#include <memory>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <vector>

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


int WindowManager::focus_next() {
  return 0;
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

void WindowManager::kill_window(WindowClass &w)
{
  XDestroyWindow(display_, w.get_window());
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
	} else if (isKey("M")) {
	  std::cout << "Managing\n";
	  manage();
	} else if (isKey("O")) {
	  std::cout << "Terminal\n";
	}
	else if (isKey("=")) {
	  increase_size();
	}
	else if (isKey("-")) {
	  decrease_size();	
	}

  }
}

void WindowManager::setkeys()
{
  MOD1BIND("D");//open dmenu
  MOD1BIND("Q");
  MOD1BIND("M");
  MOD1BIND("O");
  MOD1BIND("=");
  MOD1BIND("-");

}

void WindowManager::frame(WindowClass &w)
{
  return;
}

void WindowManager::on_map_request(XMapRequestEvent &e)
{

  printf("on map request\n");
  focused = WindowClass(e.window);
  
  if(e.parent ==  root_)
  {
    try {
      workspaces.add_window(focused);
      workspaces.focus_window(focused);
    } catch (int a) {
      std::cout << "[ERROR] on on_map_request, add window throws " << a << std::endl;
    }
  };

  try {
    manage();
  } catch (const char* a) {
    std::cout << "[ERROR] manage(): " << a << std::endl;
  }
}
int WindowManager::manage()
{
  std::cout << "On manage function" << std::endl;
  std::cout << "Layout :" << workspaces.get_current_layout() << std::endl;

  if (workspaces.get_all_current_windows().size() == 0) return -1;

  switch (workspaces.get_current_layout()) {
	case master_stack:
	  manage_master_stack();
	  break;

	case tree:
	  manage_tree();
	  break;

	case centered_master:
	  manage_centered_master();

	default:
	  manage_master_stack();
	  break;
  }
  return 0;
}

// Adds mousebutton listeners
void WindowManager::setbuttons()
{
	XGrabButton(display_, 1, Mod1Mask, root_, True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
}

int WindowManager::on_configure_request(XConfigureRequestEvent &e) {

  XWindowChanges ch;
  ch.border_width = e.border_width;
  ch.width = e.width;
  ch.height = e.height;
  ch.x = e.x;
  ch.y = e.y;
  ch.sibling = e.above;
  ch.stack_mode = e.detail;

  XConfigureWindow(display_, e.window, e.value_mask, &ch);

  return 0;
}

//int WindowManager::on_create_notify(XCreateWindowEvent &e) {
//
//
  //focused = WindowClass(e.window);
  //if(e.parent ==  root_)
  //{
//
    //try {
      //workspaces.add_window(focused);
    //} catch (int a) {
      //std::cout << "[ERROR] on on_create_notify(), add window throws " << a << std::endl;
    //}
//
  //};
//
  //XMapWindow(display_, focused.get_window());
  //return 0;
//}


int WindowManager::on_destroy_notify(XDestroyWindowEvent &e) {

  workspaces.remove_all_window(WindowClass(e.window));

  std::vector<WindowClass> th = workspaces.get_all_current_windows();


  std::cout << "[[[[[Destroy Debug]]]]]" << std::endl;
  
  for (int i = 0; i < th.size(); i++) {
    std::cout << "[Window] : " << th[i].get_window() << std::endl;
  };

  manage();
  return 0;
}

void WindowManager::handle_events(XEvent &e)
{
  switch (e.type)
  {
	case CreateNotify:
	  std::cout << "createnotify" << std::endl;
//	  this->on_create_notify(e.xcreatewindow);
	  break;
	case ConfigureRequest:
	  std::cout << "configurerequest" << std::endl;
	  this->on_configure_request(e.xconfigurerequest);
	  break;
	case MapRequest:
	  std::cout << "maprequest" << std::endl;
	  on_map_request(e.xmaprequest);
	  break;
	case DestroyNotify:
	  std::cout << "destroynotify" << std::endl;
	  on_destroy_notify(e.xdestroywindow);
	  break;
	case KeyPress:
	  std::cout << "keypress";
	  keypress(e.xkey);
	  break;
	case FocusIn:
	  std::cout << "[FOCUSIN] event just came," <<  e.xfocus.window << "gained focus" << std::endl;
	  break;
	case FocusOut:
	  std::cout << "[FOCUSOUT] event just came," <<  e.xfocus.window << "gained focus" << std::endl;
	  break;
	case MotionNotify:
	  std::cout << "Motionnotify" << std::endl;
	case ButtonPress:
	  std::cout << "buttonpress" << std::endl;
	  break;
	case ButtonRelease:
	  std::cout << "buttonrelease" << std::endl;
	  break;
  }

  
  XSync(display_, false);
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
	XSync(display_, false);
  }

}
