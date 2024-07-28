#include "workspaces.hpp"
#include <vector>

int Workspaces::total_workspaces() { return workspac_.size();}

int Workspaces::total_windows(int workspace_no) {
  if (workspace_no >= workspac_.size())
    return -1;
  else
    return workspac_[workspace_no].size();
}

int Workspaces::change_workspace(int number) {
  if (number < 0) {
    throw -1;
  } else if (number == this->total_workspaces()) {
    workspac_.push_back(std::vector<WindowClass>());
    current = number;
  } else if (number > this->total_workspaces()) {
    throw -2;
  }

  return 0;
}


const std::vector<WindowClass> Workspaces::get_all_current_windows() {
  return workspac_[current];
}
