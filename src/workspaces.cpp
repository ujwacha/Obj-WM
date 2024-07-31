#include "workspaces.hpp"
#include <X11/Xlib.h>
#include <cstdio>
#include <sched.h>
#include <vector>
#include <iostream>

#define default_layout master_stack

int Workspaces::total_workspaces() { return workspac_.size();}

int Workspaces::total_windows(int workspace_no) {
  if (workspace_no >= workspac_.size())
    return -1;
  else
    return workspac_[workspace_no].size();
}

int Workspaces::change_workspace(int number) {
  if (number < 0) 
    throw -1;
  else if (number == this->total_workspaces())
    {
      workspac_.push_back(std::vector<WindowClass>());
	  layout_.push_back(default_layout);
      current = number;
    }
  else if (number > this->total_workspaces())
    throw -2;
  else
    current = number;
  

  return 0;
}


std::vector<WindowClass>& Workspaces::get_all_current_windows() {
  return workspac_[current];
}



int Workspaces::remove_window(WindowClass window, int space) {
  if (space < 0) {
    space = current;
  }

  std::vector<WindowClass>& working_windows = workspac_[space];
  
  for (int i = 0; i < working_windows.size(); i++) {
    if (working_windows[i] == window) {
      // add a for loop here to make it work as expected
      // but i did this to make it just work for now
      working_windows[i] = working_windows[working_windows.size() - 1];
      working_windows.pop_back();

      std::cout << "******************REMOVEDDDDDDDDDDD****************" << std::endl;

    }
  }
  
  return 0;
}


int Workspaces::move_front(WindowClass window) {
  // making a temporary window because we'll have to do some
  WindowClass temp;

  std::vector<WindowClass>& working_windows = workspac_[current];

  for (int i = 0; i < working_windows.size(); i++) {
    if (working_windows[i] == window) {

      if (i == 0) {
	return -1;
      } else {
	temp = working_windows[i-1];
	working_windows[i-1] = working_windows[i];
	working_windows[i] = temp;
      }
    }
  }
  
  return 0;
}

int Workspaces::move_back(WindowClass window) {
  // making a temporary window because we'll have to do some
  WindowClass temp;

  std::vector<WindowClass>& working_windows = workspac_[current];

  for (int i = 0; i < working_windows.size(); i++) {
    if (working_windows[i] == window) {

      if (i == working_windows.size() - 1) {
	return -1;
      } else {
	temp = working_windows[i+1];
	working_windows[i+1] = working_windows[i];
	working_windows[i] = temp;
      }
    }
  }
  
  return 0;
}



int Workspaces::remove_all_window(WindowClass window) {
  for (int i = 0 ; i < workspac_.size(); i++) {
    this->remove_window(window, i);

    return 0;
  }
  throw -1;
}

int Workspaces::add_window(WindowClass window, int space) {
  std::cout << "add window called here \n";
  if (space < 0) {
    space = current;
  }

  std::cout << "space : " << space << std::endl;
  std::cout << "workspace_size: " << workspac_[space].size()<< std::endl;


  bool already = false;

  for (int i = 0; i < workspac_[space].size(); i++) 
  {
    if (workspac_[space][i] == window) throw -1;
  }

  workspac_[space].push_back(window);
  std::cout << "pushed the window back\n";


  for (int i = 0; i < workspac_[space].size(); i++) 
    workspac_[space][i].display();
  return 0;
}

int Workspaces::move_window_to_next(WindowClass window) {
  try {
    change_workspace(current + 1);
    return 0;
  } catch (int a) {
    std::cout << "[Error] when movint to next, CODE: "<< a <<"\n";
    return -1;
  }
}


int Workspaces::move_window_to_prev(WindowClass window) {
  try {
    change_workspace(current - 1);
    return 0;
  } catch (int a) {
    std::cout << "[Error] when movint to next, CODE: "<< a <<"\n";
    return -1;
  }
 
}

layout Workspaces::get_current_layout()
{
  std::cout << "\ncurrent: " << current << " total: " << layout_.size() << std::endl;
  return layout_[current];
}

void Workspaces::set_current_layout(layout l)
{
  layout_[current] = l;
}
