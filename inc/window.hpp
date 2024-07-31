
#include <X11/X.h>


class WindowClass {
public:
  WindowClass(Window* w): this_win_(w) {};
  WindowClass(): this_win_(nullptr) {}; // NULL


  bool operator== (const WindowClass& other) const{
    return this->this_win_ == other.this_win_;
  };


  bool operator= (const WindowClass& other) {
    return this->this_win_ = other.this_win_;
  };

  Window* get_window() {
    return this_win_;
  }
  
private:
  Window* this_win_;
};


