cmake_minimum_required (VERSION 3.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project (OpenGLPracticeUtils)
set(CMAKE_CXX_STANDARD 14)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PARENT_DIR}/include)
link_directories(${PARENT_DIR}/lib)


add_executable(hello_window.exe hello_window.cpp)
target_link_libraries(hello_window.exe glfw3 GL GLU X11 pthread Xrandr Xi glad dl)
