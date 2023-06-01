#include "Window.h"

PowerClicker::Window::Window(
    int Width,
    int Height,
    const char* Title,
    GLFWmonitor* Monitor,
    GLFWwindow* Share
)
    : m_Width(Width), m_Height(Height), m_Title(Title), m_Monitor(Monitor), m_Share(Share)
{}

PowerClicker::Window::Window(
    WindowSize Size,
    const char* Title,
    GLFWmonitor* Monitor,
    GLFWwindow* Share
)
    : m_Title(Title), m_Monitor(Monitor), m_Share(Share)
{
    const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    m_Width = (int)((float)(VideoMode->width) * Size.SizeX);
    m_Height = (int)((float)(VideoMode->height) * Size.SizeY);
}

void PowerClicker::Window::operator=(const Window& rhs)
{
    this->m_Height = rhs.m_Height;
    this->m_Width = rhs.m_Width;
    this->m_Title = rhs.m_Title;

    this->m_Monitor = rhs.m_Monitor;
    this->m_Share = rhs.m_Share;
}