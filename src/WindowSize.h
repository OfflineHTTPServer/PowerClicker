#ifndef _WINDOW_SIZE_H
#define _WINDOW_SIZE_H

#include <GLFW/glfw3.h>

namespace PowerClicker
{
    struct WindowSize
    {
    public:
        float SizeX;
        float SizeY;

        WindowSize(float SizeX, float SizeY);
    };
}


#endif