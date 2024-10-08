cmake_minimum_required(VERSION 3.0)
project(KingGrassRipper)

set(OpenGL_GL_PREFERENCE LEGACY)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)

include_directories("src/")

add_executable(kinggrassripper
    src/main.cpp
    src/gl_wrapper/shader/Shader.cpp
    src/objloader.cpp
    src/utils/Camera.cpp
    src/utils/Movement.cpp
    src/utils/Window.cpp
    src/utils/json/Parser.cpp
    src/Game.cpp
    src/graphics/Model.cpp
    src/models/ObjectModel.cpp
)

target_link_libraries(kinggrassripper OpenGL::GL glfw GLEW::glew png)