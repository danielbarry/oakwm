cmake_minimum_required(VERSION 3.0)

project(oakwm)

# Target OS
set(CMAKE_SYSTEM_NAME Linux)

# Libraries
find_package(Git)
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

# Compile flags
set(CXX_FLAGS "-L/usr/X11/lib -lXext -lX11 -lXmu -lm")
set_property(CACHE CMAKE_CXX_FLAGS  PROPERTY VALUE "${CXX_FLAGS}")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "MinSizeRel")

# Commit hash info
execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=40 --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# Commit date
execute_process(COMMAND
  "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_DATE
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# Commit message
execute_process(COMMAND
  "${GIT_EXECUTABLE}" log -1 --format=%s
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# Generate version information
configure_file("src/version.cc.in" "src/version.cc" @ONLY)

# Binary
add_executable(oakwm         src/main.cc)
add_executable(oakwm-toolbar src/toolbar.cc src/version.hh src/version.cc)
