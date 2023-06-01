#ifndef _WINDOW_H
#define _WINDOW_H


#include "WindowSize.h"
#include <GLFW/glfw3.h>


namespace PowerClicker
{
    class Window
    {
        public:
            int m_Width = 0;

            int m_Height = 0;
            
            const char* m_Title = nullptr;

            GLFWwindow* m_Window = nullptr;

            GLFWmonitor* m_Monitor = nullptr;

            GLFWwindow* m_Share = nullptr;

            Window(
                int Width,
                int Height,
                const char* Title,
                GLFWmonitor* Monitor = nullptr,
                GLFWwindow* Share = nullptr  
            );

            Window(
                WindowSize Size,
                const char* Title,
                GLFWmonitor* Monitor = nullptr,
                GLFWwindow* Share = nullptr
            );

            Window() = default;

            inline void Create()
            {
                m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, m_Monitor, m_Share);
            }

            inline void SetTitle(const char* Title) const
            {
                glfwSetWindowTitle(m_Window, Title);
            }

            inline void Iconify() const
            {
                glfwIconifyWindow(m_Window);
            }

            inline void Destroy() const
            {
                glfwDestroyWindow(m_Window);
            }

            inline void SwapBuffers() const
            {
                glfwSwapBuffers(m_Window);
            }

            inline int ShouldClose() const
            {
                return glfwWindowShouldClose(m_Window);
            }

            inline void MakeContextCurrent() const
            {
                glfwMakeContextCurrent(m_Window);
            }

            inline static void PollEvents()
            {
                glfwPollEvents();
            }

            inline static void Init()
            {
                glfwInit();
            }

            inline static void Terminate()
            {
                glfwTerminate();
            }

            void operator=(const Window& rhs);

    };
}

#endif