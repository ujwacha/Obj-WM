
#include <X11/X.h>
#include <X11/Xlib.h>
#include <iostream>

typedef struct
{
  int xpos;
  int ypos;
  int width;
  int height;
} dimension;

class WindowClass {
public:
  WindowClass(Window w): this_win_(w) {};
  WindowClass(): this_win_(0) {}; // NULL


  bool operator== (const WindowClass& other) const{
    return this->this_win_ == other.this_win_;
  };


  bool operator= (const WindowClass& other) {
    return this->this_win_ = other.this_win_;
  };

  Window get_window() {
    return this_win_;
  }
  
  dimension dim;

  void plot_window(Display* display_);

  void display() {
    std::cout << "[WINDOW] : " << (long int) this_win_ << std::endl;
  };

private:
  Window this_win_;
  int setborder();
};


