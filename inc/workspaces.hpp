#include <vector>
#include "window.hpp"

enum layout
{
  master_stack,
  tree,
  centered_master
};
class Workspaces {
public:

  Workspaces() {
    workspac_.push_back(std::vector<WindowClass>()); // add the first workspace
	layout_.push_back(master_stack);
  }
  
  int change_workspace(int nuber);
  std::vector<WindowClass>& get_all_current_windows();
  int remove_window(WindowClass window, int space=-1);
  int add_window(WindowClass window, int space=-1);
  int remove_all_window(WindowClass window);
  int move_front(WindowClass window);
  int move_back(WindowClass window);
  int move_window_to_next(WindowClass window);
  int move_window_to_prev(WindowClass window);
  layout get_current_layout();
  void set_current_layout(layout l);
  int total_windows(int workspace_no);
  int focus_window(WindowClass w);

private:
  std::vector<std::vector<WindowClass>> workspac_;
  std::vector<layout> layout_;
  int current = 0;

  int total_workspaces();

};
