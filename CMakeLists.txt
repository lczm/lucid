cmake_minimum_required(VERSION 3.10)

project(lucid)

# Default to debug build if release is not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_STANDARD 11)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Wshadow -Wno-unused-parameter -fdiagnostics-color -g")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/W3 /EHsc")
endif()

set (MAIN
    src/main.cc)

# Source files
file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cc)
list(FILTER SOURCE_FILES EXCLUDE REGEX "main.cc")

file(GLOB_RECURSE TEST_SOURCE_FILES tests/*.cc)

# GLM
file(GLOB_RECURSE GLM_HEADERS *.hpp)

set (GLAD_SOURCE_FILES
    lib/glfw/deps/glad_gl.c
)

# This includes the headers
set (IMGUI_SOURCE_FILES
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui.h
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_glfw.h
    lib/imgui/backends/imgui_impl_opengl3.cpp
    lib/imgui/backends/imgui_impl_opengl3.h
)

set (STB_IMAGE_HEADERS
    lib/stb_image/stb_image.h
)

# Import directories
include_directories(src/utils)
include_directories(src/constants)
include_directories(src/input)
include_directories(src/graphics)
include_directories(src/entity)
include_directories(src/sandbox)

# Setup stb_image
include_directories(lib/stb_image)

# Setup glm
include_directories(lib/glm)

# Setup stb_image
include_directories(lib/stb_image)

# Setup glm
include_directories(lib/glm)

# Setup glad
include_directories(lib/glad/include)

# Setup imgui
include_directories(lib/imgui)
include_directories(lib/imgui/examples)

# Setup glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
include_directories(lib/glfw/deps)

# Setup gtest
add_subdirectory(lib/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Executable target
add_executable(lucid
    ${MAIN}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${IMGUI_SOURCE_FILES}
    ${STB_IMAGE_HEADERS}
    ${GLAD_SOURCE_FILES}
    ${GLM_HEADERS})

# Link the executable to glfw
target_link_libraries(lucid glfw)

# Tests
# Add the all the source files to the executable as well
add_executable(tests
    ${TEST_SOURCE_FILES}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${IMGUI_SOURCE_FILES}
    ${STB_IMAGE_HEADERS}
    ${GLAD_SOURCE_FILES}
    ${GLM_HEADERS})

# Link test executable against gtest & gtest_main
target_link_libraries(tests gtest gtest_main glfw)
add_test(tests tests)
