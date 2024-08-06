cmake_minimum_required(VERSION 3.12)
project(cairo-playground VERSION 1.0.0 LANGUAGES C CXX)

include(cmake/acfutils.cmake)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

use_static_libc()
find_acfutils(${LIBACFUTILS})

add_subdirectory(lib/glfw)
add_subdirectory(src/glutils)



add_executable(playground src/playground/main.c)
target_link_libraries(playground
PRIVATE
    ${GLFW_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
    glutils
    acfutils
)

add_custom_target(run
    COMMAND playground
    DEPENDS playground
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(debug
    COMMAND lldb $<TARGET_FILE:playground>
    DEPENDS playground
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

