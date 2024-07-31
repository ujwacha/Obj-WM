#include <exception>
#include <iostream>
#include <vector>
#include <window_manager.hpp>

float master_cm = 0.5;
float master = 0.6;
float odd = 0.25;
float even = 0.25;

void WindowManager::increase_height()
{
  std::cout << "height increase function called\n";
  std::vector<WindowClass>&windows = workspaces.get_all_current_windows();
  if(windows.size() == 0 || workspaces.get_current_layout() == tree || workspaces.get_current_layout() == centered_master) return;
  if(windows[0].is_focused()) return;

  for(int i = 1; i<windows.size();i++)
  {
	if(windows[i].is_focused())
	{
	  windows[i].power += 0.05;
	}
  }
  manage();
}

void WindowManager::decrease_height()
{
  std::cout << "height increase function called\n";
  std::vector<WindowClass>&windows = workspaces.get_all_current_windows();
  if(windows.size() == 0 || workspaces.get_current_layout() == tree || workspaces.get_current_layout() == centered_master) return;
  if(windows[0].is_focused()) return;

  for(int i = 1; i<windows.size();i++)
  {
	if(windows[i].is_focused())
	{
	  windows[i].power -= 0.05;
	}
  }
  manage();
}

void WindowManager::increase_size()
{
  std::vector<WindowClass>current_windows = workspaces.get_all_current_windows();
  if(current_windows.size() == 0 || workspaces.get_current_layout() == tree) return;

  if(workspaces.get_current_layout() == master_stack)
  {
	for (int i=0;i<current_windows.size();i++) {
	  if(current_windows[i].is_focused())
	  {
		if(i==0)
		{
		  if(master >= 0.8) return;
		  master += 0.005f;
		}
		else if (i>0) {
		  if(master <= 0.2) return;
		  master -= 0.005f;		
		}
	  }
	}
  }

  else if (workspaces.get_current_layout() == centered_master) {
	for(int i=0; i<current_windows.size();i++)
	{
	  if(current_windows[i].is_focused())
	  {
		if(i==0)
		{
		  if(master_cm >= 0.7 || odd<=0.15 || even <=0.15) return;
		  master_cm += 0.005f;
		  odd -= 0.0025f;
		  even -= 0.0025f;
		}

		else if (i%2==0) {
		  if(master_cm <= 0.3 || even >= 0.55 || odd<= 0.15) return;
		  master_cm -= 0.0025;
		  odd -= 0.0025;
		  even += 0.005;
		}

		else if(i%2 !=0)
		{
		  if(master_cm <= 0.3 || even >= 0.15 || odd<= 0.55) return;
		  master_cm -= 0.0025;
		  odd += 0.005;
		  even -= 0.0025;
		}
	  }

	}
  }
  manage();
}

void WindowManager::decrease_size()
{
  std::vector<WindowClass>current_windows = workspaces.get_all_current_windows();
  if(current_windows.size() == 0 || workspaces.get_current_layout() == tree) return;

  if(workspaces.get_current_layout() == master_stack)
  {
	for (int i=0;i<current_windows.size();i++) {
	  if(current_windows[i].is_focused())
	  {
		if(i==0)
		{
		  if(master <= 0.2) return;
		  master -= 0.005f;
		}
		else if (i>0) {
		  if(master >= 0.8) return;
		  master += 0.005f;		
		}
	  }
	}
  }

  else if (workspaces.get_current_layout() == centered_master) {
	for(int i=0; i<current_windows.size();i++)
	{
	  if(current_windows[i].is_focused())
	  {
		if(i==0)
		{
		  if(master_cm <= 0.3) return;
		  master_cm -= 0.005f;
		  odd += 0.0025f;
		  even += 0.0025f;
		}

		else if (i%2==0) {
		  if(even <= 0.1) return;
		  master_cm += 0.0025;
		  odd += 0.0025;
		  even -= 0.005;
		}

		else if(i%2 !=0)
		{
		  if(odd<0.1) return;
		  master_cm += 0.0025;
		  odd -= 0.005;
		  even += 0.0025;
		}
	  }

	}
  }
  manage();
}

int WindowManager::manage_centered_master()
{
  std::vector<WindowClass> current_windows = workspaces.get_all_current_windows();
  for (int i=0;i<current_windows.size();i++){
	if(current_windows[i] == 0) return -1;
  }

  if(current_windows.size() == 1)
  {
	current_windows[0].dim.xpos = 0;
	current_windows[0].dim.ypos = 0;
	current_windows[0].dim.width = swidth;
	current_windows[0].dim.height = sheight;
  }

  else if(current_windows.size() == 2)
  {
	current_windows[0].dim.xpos = 0;
	current_windows[0].dim.ypos = 0;
	current_windows[0].dim.width = master_cm*swidth;
	current_windows[0].dim.height = sheight;

	current_windows[1].dim.xpos = current_windows[0].dim.width;
	current_windows[1].dim.ypos = 0;
	current_windows[1].dim.width =swidth - current_windows[0].dim.width;
	current_windows[1].dim.height = sheight;
  }

  else if(current_windows.size() >2)
  {
	current_windows[0].dim.height = sheight;
	current_windows[0].dim.width = swidth*master_cm;
	current_windows[0].dim.xpos = swidth*even;
	current_windows[0].dim.ypos = 0;

	for (int i = current_windows.size()-1; i > 0; i--)
	{
	  if (i % 2 != 0)
	  {
		current_windows[i].dim.width = swidth * odd;
		current_windows[i].dim.height = (sheight) / (current_windows.size() / 2);
		current_windows[i].dim.xpos = swidth * (even + master_cm);
		current_windows[i].dim.ypos = ((sheight) / (current_windows.size() / 2)) * ((i - 1) / 2);
	  }

	  else
	  {
		current_windows[i].dim.width = swidth * even;
		current_windows[i].dim.height = (sheight) / ((current_windows.size()-1) / 2);
		current_windows[i].dim.xpos = 0;
		current_windows[i].dim.ypos = ((sheight) / ((current_windows.size()-1) / 2)) * ((i - 2) / 2);
	  } 
	}
  }

  for(int i=0;i<current_windows.size();i++)
  {
	current_windows[i].plot_window(display_);
  }
  XSetInputFocus(display_, focused.get_window(), RevertToParent, CurrentTime);
  XRaiseWindow(display_, focused.get_window());
  return 0;
}


int WindowManager::manage_master_stack()
{
  float slave = 1.0f-master;
  std::vector<WindowClass> current_windows = workspaces.get_all_current_windows();
  for (int i=0;i<current_windows.size();i++){
	if(current_windows[i] == 0) return -1;
  }
  float total_power;

  for(int i=1;i<current_windows.size();i++)
  {
	total_power += current_windows[i].power;
  }

  unsigned int total_windows_in_this_tag = current_windows.size();
  unsigned int total_stacks_in_this_tag = current_windows.size() - 1;

  // set winInfo for master window
  if (total_windows_in_this_tag == 1)
  { // take the full size if it is the only window
	current_windows[0].dim.width = swidth;
	current_windows[0].dim.height = sheight;
  }
  else
  {
	current_windows[0].dim.width = swidth * master;
	current_windows[0].dim.height = sheight;
  }
  current_windows[0].dim.xpos = 0;
  current_windows[0].dim.ypos = 0;

  // set winInfo for stacks
  for (int i = 1; i < current_windows.size(); i++)
  {
	current_windows[i].dim.height = sheight * current_windows[i].power/total_power;
	current_windows[i].dim.width = swidth * slave;
	current_windows[i].dim.xpos = swidth * master;
	if(i==1) current_windows[i].dim.ypos = 0;
	else
	 {
	   current_windows[i].dim.ypos = current_windows[i-1].dim.ypos + current_windows[i-1].dim.height;
	 };
  }

  for(int i=0;i<current_windows.size();i++)
  {
	current_windows[i].plot_window(display_);
  }


  std::cout << "[FOCUSED] : " << focused.get_window() << std::endl;

  XSetInputFocus(display_, focused.get_window(), RevertToParent, CurrentTime);
  XRaiseWindow(display_, focused.get_window());
  return 0;
}

int WindowManager::manage_tree()
{
  float master = 0.55f;
  float slave = 1.0f-master;
  std::vector<WindowClass> current_windows = workspaces.get_all_current_windows();
  for (int i=0;i<current_windows.size();i++){
	if(current_windows[i] == 0) return -1;
  }

  unsigned int total_windows_in_this_tag = current_windows.size();
  unsigned int total_stacks_in_this_tag = current_windows.size() - 1;

  current_windows[0].dim.height = sheight; // for master window
  current_windows[0].dim.width = swidth;
  current_windows[0].dim.xpos = 0;
  current_windows[0].dim.ypos = 0;

  for (int i = 1; i < total_windows_in_this_tag; i++)
  { // gives each window their required dimensions and position
	if (i % 2 == 1)
	{
	  current_windows[i].dim.width = current_windows[i-1].dim.width * (1 - master);
	  current_windows[i-1].dim.width *= master;
	  current_windows[i].dim.height = current_windows[i-1].dim.height;
	  current_windows[i].dim.xpos = current_windows[i-1].dim.xpos + current_windows[i-1].dim.width;
	  current_windows[i].dim.ypos = current_windows[i-1].dim.ypos;
	}
	else
	{
	  current_windows[i].dim.height = current_windows[i-1].dim.height * (1 - master);
	  current_windows[i-1].dim.height *= master;
	  current_windows[i].dim.width = current_windows[i-1].dim.width;
	  current_windows[i].dim.ypos = current_windows[i-1].dim.ypos + current_windows[i-1].dim.height;
	  current_windows[i].dim.xpos = current_windows[i-1].dim.xpos;
	}
  }

  for(int i=0;i<current_windows.size();i++)
  {
	current_windows[i].plot_window(display_);
  }
  XSetInputFocus(display_, focused.get_window(), RevertToParent, CurrentTime);
  XRaiseWindow(display_, focused.get_window());
  return 0;
}

