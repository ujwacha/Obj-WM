#include <cstdlib>
#include <iostream>
#include <memory>
#include "glog/logging.h"
#include "window_manager.hpp"

int main(int argc, char** argv) {

  ::google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Starting ObjWM";


  std::unique_ptr<WindowManager> manager(WindowManager::Create());


  if (!manager) {
    LOG(ERROR) << "failled to initialize display";
    return EXIT_FAILURE;
  }

  manager->Run();
  
  return EXIT_SUCCESS;
}
