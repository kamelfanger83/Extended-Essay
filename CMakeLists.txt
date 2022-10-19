cmake_minimum_required(VERSION 3.23)
project(Extended_Essay)

set(CMAKE_CXX_STANDARD 14)

include_directories("Visual Studio Project")
include_directories("Visual Studio Project/imgui")
include_directories("Visual Studio Project/Libraries")
include_directories("Visual Studio Project/Libraries/include")
include_directories("Visual Studio Project/Libraries/include/glad")
include_directories("Visual Studio Project/Libraries/include/GLFW")
include_directories("Visual Studio Project/Libraries/include/KHR")

add_executable(Extended_Essay
        "Visual Studio Project/imgui/imconfig.h"
        "Visual Studio Project/imgui/imgui.cpp"
        "Visual Studio Project/imgui/imgui.h"
        "Visual Studio Project/imgui/imgui_demo.cpp"
        "Visual Studio Project/imgui/imgui_draw.cpp"
        "Visual Studio Project/imgui/imgui_impl_glfw.cpp"
        "Visual Studio Project/imgui/imgui_impl_glfw.h"
        "Visual Studio Project/imgui/imgui_impl_opengl3.cpp"
        "Visual Studio Project/imgui/imgui_impl_opengl3.h"
        "Visual Studio Project/imgui/imgui_impl_opengl3_loader.h"
        "Visual Studio Project/imgui/imgui_internal.h"
        "Visual Studio Project/imgui/imgui_tables.cpp"
        "Visual Studio Project/imgui/imgui_widgets.cpp"
        "Visual Studio Project/imgui/imstb_rectpack.h"
        "Visual Studio Project/imgui/imstb_textedit.h"
        "Visual Studio Project/imgui/imstb_truetype.h"
        "Visual Studio Project/Libraries/include/glad/glad.h"
        "Visual Studio Project/Libraries/include/GLFW/glfw3.h"
        "Visual Studio Project/Libraries/include/GLFW/glfw3native.h"
        "Visual Studio Project/Libraries/include/KHR/khrplatform.h"
        "Visual Studio Project/animals.cpp"
        "Visual Studio Project/animals.h"
        "Visual Studio Project/constants.cpp"
        "Visual Studio Project/constants.h"
        "Visual Studio Project/glad.c"
        "Visual Studio Project/Main.cpp"
        "Visual Studio Project/utils.cpp"
        "Visual Studio Project/utils.h")
target_link_libraries(Extended_Essay "Visual Studio Project/Libraries/lib/glfw3.lib")

