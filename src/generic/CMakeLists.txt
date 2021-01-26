cmake_minimum_required((VERSION 3.10)

project(generic)

# Optimized flags
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_CXX_STANDARD 17)

# Entry point
set (MAIN
    src/main.cc)

# Source files
file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cc)

# TODO : Set this as a variable through cmake
file(GLOB_RECURSE HEADER_FILES_EXAMPLE examples/*.h)
file(GLOB_RECURSE SOURCE_FILES_EXAMPLE examples/*.cc)

# GLM
file(GLOB_RECURSE GLM_HEADERS *.hpp)

# Glad
set (GLAD_SOURCE_FILES
    lib/glfw/deps/glad_gl.c
)

set (STB_IMAGE_HEADERS
    lib/stb_image/stb_image.h
)

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

include_directories(examples/pong)
include_directories(lib/glm)
include_directories(lib/stb_image)
include_directories(lib/glm)
include_directories(lib/glad/include)
include_directories(lib/openal-soft/include)
include_directories(lib/libsndfile/build/include)
include_directories(lib/assimp/include)
include_directories(lib/cereal)

# Setup glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
include_directories(lib/glfw/deps)

# Executable target
add_executable(generic
    ${MAIN}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES_EXAMPLE}
    ${HEADER_FILES_EXAMPLE}
    ${STB_IMAGE_HEADERS}
    ${GLAD_SOURCE_FILES}
    ${GLM_HEADERS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(lucid glfw assimp sndfile OpenAL32 nfd)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_libraries(lucid glfw assimp* sndfile OpenAL32 nfd user32.lib gdi32.lib shell32.lib vcruntime.lib msvcrt.lib)
    else()
        target_link_libraries(lucid glfw assimp* sndfile OpenAL32 nfd)
    endif(CMAKE_BUILD_TYPE STREQUAL "Release")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")