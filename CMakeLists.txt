cmake_minimum_required(VERSION 3.13)

project(ObjWM C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(X11 REQUIRED)
find_package(glog REQUIRED)


include_directories(
  inc
  )

add_executable(ObjWM
  src/main.cpp
  src/window_manager.cpp
  src/window.cpp
  src/workspaces.cpp
  src/manager.cpp
  )

target_link_libraries(ObjWM
  X11
  glog
  )
