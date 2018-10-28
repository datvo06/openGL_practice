cmake_minimum_required (VERSION 3.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project (OpenGLPracticeUtilsTut2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER /usr/bin/gcc-6 CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER /usr/bin/g++-6 CACHE PATH "" FORCE)


get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
include_directories("/usr/include/c++/6.4.0")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PARENT_DIR}/include)
link_directories(${PARENT_DIR}/lib)

add_executable(hello_triangle.exe hello_triangle.cpp)
target_link_libraries(hello_triangle.exe glfw3 GL GLU X11 pthread Xrandr Xi glad dl datstringutils)
