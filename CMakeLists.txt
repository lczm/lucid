cmake_minimum_required(VERSION 3.10)

project(lucid)

# Default to debug build if release is not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_STANDARD 17)

# Compiler flags
add_definitions(-DDEBUG=${DEBUG})
add_definitions(-DRELEASE=${RELEASE})

# This can be helpful for clangd
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Wshadow -Wno-unused-parameter -fdiagnostics-color -g")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/W3 /EHsc")
endif()

set (MAIN
    src/main.cc)

# Source files
file(GLOB_RECURSE HEADER_FILES 
        src/audio/*.h
        src/constants/*.h
        src/entity/*.h
        src/entity/*.h
        src/graphics/*.h
        src/input/*.h
        src/lucid/*.h
        src/serialize/*.h
        src/shaders/*.h
        src/systems/*.h
        src/utils/*.h)
file(GLOB_RECURSE SOURCE_FILES 
        src/audio/*.cc
        src/constants/*.cc
        src/entity/*.cc
        src/entity/*.cc
        src/graphics/*.cc
        src/input/*.cc
        src/lucid/*.cc
        src/serialize/*.cc
        src/shaders/*.cc
        src/systems/*.cc
        src/utils/*.cc)

file(GLOB_RECURSE STARTUP_HEADERS src/startup/*.h)

file(GLOB_RECURSE WORKSPACE_HEADER_FILES ${ROOT}/*.h)
file(GLOB_RECURSE WORKSPACE_SOURCE_FILES ${ROOT}/*.cc)

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

set (IMGUIZMO_HEADERS
    lib/ImGuizmo/ImGuizmo.h
    lib/ImGuizmo/ImGuizmo.cpp
)

# Import directories
include_directories(src/utils)
include_directories(src/constants)
include_directories(src/input)
include_directories(src/audio)
include_directories(src/graphics)
include_directories(src/entity)
include_directories(src/lucid)
include_directories(src/shaders)
include_directories(src/systems)
include_directories(src/ui)
include_directories(src/serialize)

if (${ROOT} STREQUAL ".")
    include_directories(src/startup)
else()
    include_directories(${ROOT})
    include_directories(${ROOT}/systems)
endif (${ROOT} STREQUAL ".")

# include_directories(src/startup)
# include_directories(${ROOT})
# include_directories(${ROOT}/systems)

# include_directories(${ROOT})

# Setup generic systems
# include_directories(${ROOT}/systems)

# Example directories
# include_directories(src/examples/pong)
# include_directories(examples/pong)

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
include_directories(lib/imgui/backends)

# Setup openal
include_directories(lib/openal-soft/include)

# Setup libsndfile
include_directories(lib/libsndfile/build/include)

# Setup assimp
include_directories(lib/assimp/include)

# Setup ImGuizmo
include_directories(lib/ImGuizmo)

# Setup nativefiledialog-extended
add_subdirectory(lib/nativefiledialog-extended)
include_directories(lib/nativefiledialog-extended/src/include)

# Setup Cereal
include_directories(lib/cereal)

# Setup Cereal
include_directories(lib/freetype2/include)

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
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Executable target
add_executable(lucid
    ${MAIN}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES_EXAMPLE}
    ${HEADER_FILES_EXAMPLE}
    ${IMGUI_SOURCE_FILES}
    ${STB_IMAGE_HEADERS}
    ${GLAD_SOURCE_FILES}
    ${GLM_HEADERS}
    ${IMGUIZMO_HEADERS}
    ${STARTUP_HEADERS}
    "src/startup/startup.h"
    "src/startup/engineStartup.h")

# Link the executable to glfw
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(lucid glfw assimp sndfile OpenAL32 nfd freetyped)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_libraries(lucid glfw assimp* sndfile OpenAL32 nfd freetyped user32.lib gdi32.lib shell32.lib vcruntime.lib msvcrt.lib)
    else()
        target_link_libraries(lucid glfw assimp* sndfile OpenAL32 nfd freetyped)
    endif(CMAKE_BUILD_TYPE STREQUAL "Release")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Tests
# Add the all the source files to the executable as well
add_executable(tests
    ${TEST_SOURCE_FILES}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES_EXAMPLE}
    ${HEADER_FILES_EXAMPLE}
    ${IMGUI_SOURCE_FILES}
    ${STB_IMAGE_HEADERS}
    ${GLAD_SOURCE_FILES}
    ${GLM_HEADERS}
    ${IMGUIZMO_HEADERS})

# Link test executable against gtest & gtest_main
target_link_libraries(tests gtest gtest_main glfw assimp* sndfile OpenAL32 nfd freetyped)
add_test(tests tests)

# Generic executable
add_executable(generic
    ${MAIN}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${WORKSPACE_HEADER_FILES}
    ${WORKSPACE_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
    ${STB_IMAGE_HEADERS}
    ${GLAD_SOURCE_FILES}
    ${GLM_HEADERS}
    ${IMGUIZMO_HEADERS})
    # ${ROOT}/startup.h
    # ${ROOT}/engineStartup.h)
    # "../lucid-test/startup.h"
    # "../lucid-test/engineStartup.h")

# Link the executable to glfw
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(generic glfw assimp sndfile OpenAL32 nfd freetyped)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_libraries(generic glfw assimp* sndfile OpenAL32 nfd freetyped user32.lib gdi32.lib shell32.lib vcruntime.lib msvcrt.lib)
    else()
        target_link_libraries(generic glfw assimp* sndfile OpenAL32 nfd freetyped)
    endif(CMAKE_BUILD_TYPE STREQUAL "Release")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")