extern "C" {
#include <X11/Xlib.h>
#include <X11/Xutil.h>
}

#include <window.hpp>

#define border_pixel 2
#define swidth 10
#define sheight 10

void WindowClass::plot_window(Display* display_)
{
		int xpos = dim.xpos;
		int ypos = dim.ypos;
		unsigned int winW = dim.width;
		 unsigned int winH = dim.height;
		int oxpos = xpos + winW;
		int oypos = ypos + winH;
		if (xpos == 0)
		{
			xpos += border_pixel;
			winW -= border_pixel;
		}
		if (ypos == 0)
		{
			ypos += border_pixel;
			winH -= border_pixel;
		}
		if (oxpos > swidth - 5 && oxpos < swidth + 5)
			winW -= border_pixel;
		if (oypos > sheight - 5 && oypos < sheight + 5)
			winH -= (border_pixel - 5);

		xpos += border_pixel;
		ypos += border_pixel;
		winW -= (2 * border_pixel + 5);
		winH -= (2 * border_pixel + 5);

		setborder(display_);
		XMoveResizeWindow(display_, this_win_, xpos, ypos, winW, winH);
		XMapWindow(display_, this_win_);
}

int WindowClass::setborder(Display* display)
{

  Colormap cmap = DefaultColormap(display, DefaultScreen(display));

  XColor border_color_r, exactColor_r;
  Status status = XAllocNamedColor(display, cmap, "red", &border_color_r, &exactColor_r);

  XColor border_color_b, exactColor_b;
  Status another_status = XAllocNamedColor(display, cmap, "blue", &border_color_b, &exactColor_b);

  if (focused)
  {
    std::cout << "[FOCUSED<Border>]: " << this_win_ << std::endl;
    std::cout << "Setting the border color to red: " << this_win_ << std::endl;
    XSetWindowBorder(display, this_win_, border_color_r.pixel);
    XSetInputFocus(display, focused, RevertToParent, CurrentTime);
    XRaiseWindow(display, focused);
    XSetWindowBorderWidth(display, this_win_, border_pixel);
    return 0;
  }
  else
  {
    std::cout << "[Unfocused<Border>]: " << this_win_ << std::endl;
    std::cout << "Setting the border color to blue: " << this_win_ << std::endl;
    XSetWindowBorder(display, this_win_, border_color_b.pixel);
    XSetWindowBorderWidth(display, this_win_, border_pixel);
    return 1;
  }

}
