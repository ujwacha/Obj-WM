extern "C" {
#include <X11/Xlib.h>
}

#include <window.hpp>

#define border_pixel 5 
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

		setborder();
		XMoveResizeWindow(display_, this_win_, xpos, ypos, winW, winH);
		XMapWindow(display_, this_win_);
}

int WindowClass::setborder()
{
  return 0;
}
