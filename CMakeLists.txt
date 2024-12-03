cmake_minimum_required(VERSION 3.0)
project(KingGrassRipper)

set(OpenGL_GL_PREFERENCE LEGACY)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -gdwarf-4)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Freetype MODULE REQUIRED)

include_directories("src/")
include_directories("src/tracy/")

add_definitions(-DTRACY_ENABLE)

set(sources ${sources}

    src/main.cpp
    src/gl_wrapper/shader/Shader.cpp
    src/objloader.cpp
    src/utils/Camera.cpp
    src/utils/Movement.cpp
    src/utils/Window.cpp
    src/utils/json/Parser.cpp
    src/Game.cpp
    src/graphics/Model.cpp
    src/graphics/image/Image.cpp
    src/models/ObjectModel.cpp

    src/ecs/archetypal/TypeSet.cpp
    src/ecs/archetypal/Archetype.cpp
    src/tracy/TracyClient.cpp
)

set(libraries ${libraries}
    OpenGL::GL glfw GLEW::glew png fmt::fmt-header-only Freetype::Freetype
)

add_executable(kinggrassripper ${sources})

target_link_libraries(kinggrassripper ${libraries})

Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

add_executable(tests

    tests/ecs/ecs.cpp
    tests/ecs/TypeSet.cpp
    tests/ecs/Query.cpp
    tests/ecs/Archetype.cpp
    tests/ecs/Arena.cpp
    tests/ecs/World.cpp

    src/ecs/archetypal/TypeSet.cpp
    src/ecs/archetypal/Archetype.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${libraries})
target_compile_definitions(tests PUBLIC TEST_MODE)
include(CTest)
include(Catch)
catch_discover_tests(tests)
