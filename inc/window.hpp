
#include <X11/X.h>


class WindowClass {
public:
  WindowClass(Window* w): this_win_(w) {};

private:
  const Window* this_win_;
  

};
