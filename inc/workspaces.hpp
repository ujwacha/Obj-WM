#include <vector>
#include "window.hpp"

class Workspaces {
public:

  int change_workspace(int nuber);
  const std::vector<WindowClass> get_all_current_windows();
  
private:
  std::vector<std::vector<WindowClass>> workspac_;

  int current = 0;

  int total_workspaces();
  int total_windows(int workspace_no);
};
