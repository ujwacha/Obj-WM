#include <window_manager.hpp>

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
	current_windows[0].dim.width = 0.6*swidth;
	current_windows[0].dim.height = sheight;

	current_windows[1].dim.xpos = current_windows[0].dim.width;
	current_windows[1].dim.ypos = 0;
	current_windows[1].dim.width =swidth - current_windows[0].dim.width;
	current_windows[1].dim.height = sheight;
  }

  else if(current_windows.size() >2)
  {
	current_windows[0].dim.height = sheight;
	current_windows[0].dim.width = swidth*0.5;
	current_windows[0].dim.xpos = swidth*0.25;
	current_windows[0].dim.ypos = 0;

	for (int i = current_windows.size()-1; i > 0; i--)
	{
	  if (i % 2 != 0)
	  {
		current_windows[i].dim.width = swidth * 0.25;
		current_windows[i].dim.height = (sheight) / (current_windows.size() / 2);
		current_windows[i].dim.xpos = swidth * 0.75;
		current_windows[i].dim.ypos = ((sheight) / (current_windows.size() / 2)) * ((i - 1) / 2);
	  }

	  else
	  {
		current_windows[i].dim.width = swidth * 0.25;
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
  float master = 0.55f;
  float slave = 1.0f-master;
  std::vector<WindowClass> current_windows = workspaces.get_all_current_windows();
  for (int i=0;i<current_windows.size();i++){
	if(current_windows[i] == 0) return -1;
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
		current_windows[i].dim.height = sheight / total_stacks_in_this_tag;
		current_windows[i].dim.width = swidth * slave;
		current_windows[i].dim.xpos = swidth * master;
		current_windows[i].dim.ypos = current_windows[i].dim.height * (i - 1);
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

