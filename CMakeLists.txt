cmake_minimum_required(VERSION 3.0)
project(VenomGUI)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_executable()

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES})