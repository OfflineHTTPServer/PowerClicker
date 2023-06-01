#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace PowerClicker
{
    class Context
    {
    public:
        Context() = delete;
        

        static void NewFrame();

        static void Render();

        static void Shutdown();
    };
}

#endif