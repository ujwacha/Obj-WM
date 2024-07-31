
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

  bool is_focused()
  {return focused;}

  void focus(){
    std::cout << "[FOCUSED focus()]: " << this_win_ << std::endl;
    focused = true;
  };

  void unfocus(){
    std::cout << "[UN-FOCUSED unfocus()]: " << this_win_ << std::endl;
    focused = false;
  };
  float power = 1;

private:
  Window this_win_;
  int setborder(Display* display);
  bool focused = false;
};


