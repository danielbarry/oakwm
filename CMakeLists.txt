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

# The 'real' git information file
SET(GITREV_BARE_FILE src/git-rev.hh)
# The temporary git information file
SET(GITREV_BARE_TMP src/git-rev-tmp.hh)
SET(GITREV_FILE ${CMAKE_BINARY_DIR}/${GITREV_BARE_FILE})
SET(GITREV_TMP ${CMAKE_BINARY_DIR}/${GITREV_BARE_TMP})

add_custom_command(
  OUTPUT ${GITREV_TMP}
  COMMAND ${CMAKE_COMMAND} -E echo_append "#define GIT_BRANCH_RAW " > ${GITREV_TMP}
  COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD >> ${GITREV_TMP}
  COMMAND ${CMAKE_COMMAND} -E echo_append "#define GIT_HASH_RAW " >> ${GITREV_TMP}
  COMMAND ${GIT_EXECUTABLE} describe --always --dirty --abbrev=40 --match="NoTagWithThisName" >> ${GITREV_TMP}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GITREV_TMP} ${GITREV_FILE}
  COMMAND ${CMAKE_COMMAND} -E remove ${GITREV_TMP}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  VERBATIM
)

# Binary
add_executable(oakwm         src/main.cc)
add_executable(oakwm-toolbar src/toolbar.cc ${GITREV_TMP})
