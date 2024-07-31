#include <vector>
#include "window.hpp"

class Workspaces {
public:

  int change_workspace(int nuber);
  std::vector<WindowClass>& get_all_current_windows();
  int remove_window(WindowClass window, int space=-1);
  int add_window(WindowClass window, int space=-1);
  int remove_all_window(WindowClass window);
  int move_front(WindowClass window);
  int move_back(WindowClass window);
  int move_window_to_next(WindowClass window);
  int move_window_to_prev(WindowClass window);

private:
  std::vector<std::vector<WindowClass>> workspac_;
  int current = 0;

  int total_workspaces();
  int total_windows(int workspace_no);

};
