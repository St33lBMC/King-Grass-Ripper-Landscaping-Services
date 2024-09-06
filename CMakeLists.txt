cmake_minimum_required (VERSION 3.0)
project (KingGrassRipper)

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(kinggrassripper src/main.cpp)
target_link_libraries(kinggrassripper OpenGL::GL glfw)
