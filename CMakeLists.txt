cmake_minimum_required(VERSION 3.0)
project(VenomGui)

# Finding gz-rendering
find_package(ignition-rendering6 REQUIRED)
find_package(ignition-common4 REQUIRED)

# Finding OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Adding glfw
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory(ext/glfw)

# Adding imgui
set(IMGUI_SOURCE_PATH ext/imgui)
include_directories(
    ${IMGUI_SOURCE_PATH}
    ${IMGUI_SOURCE_PATH}/backends)
set(IMGUI_SRCS
    ${IMGUI_SOURCE_PATH}/imgui.cpp
    ${IMGUI_SOURCE_PATH}/imgui_widgets.cpp
    ${IMGUI_SOURCE_PATH}/imgui_tables.cpp
    ${IMGUI_SOURCE_PATH}/imgui_draw.cpp
    ${IMGUI_SOURCE_PATH}/imgui_demo.cpp
    ${IMGUI_SOURCE_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SOURCE_PATH}/backends/imgui_impl_opengl3.cpp
    )

# Setting VenomGui headers
include_directories(include)

# Setting VenomGui sources
set(VENOM_SRCS src/main.cpp)

# ignition-rendering examples use this...
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

add_executable(${PROJECT_NAME} ${VENOM_SRCS} ${IMGUI_SRCS})

# Linking libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw
    ${IGNITION-COMMON_LIBRARIES}
    ${IGNITION-RENDERING_LIBRARIES}
)