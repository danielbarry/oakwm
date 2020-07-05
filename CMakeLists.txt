cmake_minimum_required(VERSION 3.0)

project(oakwm)

# Target OS
set(CMAKE_SYSTEM_NAME Linux)

# Libraries
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

# Compile flags
set(CXX_FLAGS "-L/usr/X11/lib -lXext -lX11 -lXmu -lm")
set_property(CACHE CMAKE_CXX_FLAGS  PROPERTY VALUE "${CXX_FLAGS}")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "MinSizeRel")

# Binary
add_executable(oakwm main.cc)
