#include "Windows.h"
#include <stdio.h>


void MouseAction(int MouseButton, int PosX, int PosY)
{
    if (PosX > -1 && PosY > -1)
    {
        SetCursorPos(PosX, PosY);
    }

    int MouseDownEvent;
    int MouseUpEvent;

    switch (MouseButton)
    {
    case 0:
        MouseDownEvent = MOUSEEVENTF_LEFTDOWN;
        MouseUpEvent = MOUSEEVENTF_LEFTUP;
        break;

    case 1:
        MouseDownEvent = MOUSEEVENTF_RIGHTDOWN;
        MouseUpEvent = MOUSEEVENTF_RIGHTUP;
        break;

    case 2:
        MouseDownEvent = MOUSEEVENTF_MIDDLEDOWN;
        MouseUpEvent = MOUSEEVENTF_MIDDLEUP;
        break;

    default:
        break;
    }

    mouse_event(MouseDownEvent, 0, 0, 0, 0);
    mouse_event(MouseUpEvent, 0, 0, 0, 0);
}