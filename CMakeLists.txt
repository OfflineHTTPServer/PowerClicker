cmake_minimum_required(VERSION 3.0.0)
project(PowerClicker VERSION 1.1.0)

include(CTest)
enable_testing()

add_executable(
    PowerClicker 
    src/main.cpp
    src/Window.cpp
    src/Context.cpp
    src/WindowSize.cpp
    src/ToMilliseconds.cpp
    
    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp 
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

add_subdirectory(lib/glfw)

target_link_libraries(
    PowerClicker
    PUBLIC
    glfw
    opengl32.lib
)


target_include_directories(
    PowerClicker
    PUBLIC
    lib/glfw/include
    lib/imgui
    lib/imgui/backends
)

target_link_options (PowerClicker PUBLIC "/SUBSYSTEM:WINDOWS")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
