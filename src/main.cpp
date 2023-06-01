#define TITLE "PowerClicker"
#define VERSION "1.0"

#define WINDOW_SIZE_X_PX 540.0f
#define WINDOW_SIZE_Y_PX 380.0f

#define WINDOW_SIZE_X_SHARE WINDOW_SIZE_X_PX / 1920.0f
#define WINDOW_SIZE_Y_SHARE WINDOW_SIZE_Y_PX / 1080.0f

#define POS_IDLE -1

#include "Window.h"
#include "Context.h"
#include "WindowSize.h"
#include "Keys.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

PowerClicker::Window Window;

const char* MainPageTitle = TITLE " - " VERSION;

int MsDelay = 1;
int MouseButton = 0;
int PosX = POS_IDLE;
int PosY = POS_IDLE;
int ToggleKey = NULL;
int TerminationKey = NULL;

bool BlockInputOnToggleKeyPressed = true;
bool Stopped = true;
bool IconifyWindowOnStart = true;
bool Enabled = false;
bool Running = true;
bool EnabledOnstart = true;

HHOOK Hook = nullptr;

const char* MouseButtons = "Left Mouse Button\0Right Mouse Button\0Middle Mouse Button";
const char* TimeUnits = "Second\0Minute\0Hour";


extern int ToMilliseconds(int, int);
extern void MouseAction(int, int, int);


const char* MouseButtonLabelText = "Mouse Button";
const char* ClicksLabelText = "Clicks";
const char* TimeUnitLabelText = "Time";
const char* SpecifyCoordinatesLabelText = "Specify Coordinates";
const char* CoordinatesInputLabelText = "X Y";
const char* HotkeyLabelText = "Toggle Key";
const char* IconifyWindowOnStartLabelText = "Minimize Window on Start";
const char* BlockInputOnToggleKeyPressedLabelText = "Block Input when Toggle Key Pressed"; 
const char* TerminationKeyLabelText = "Termination Key";
const char* StartButtonText = "Start";
const char* StopButtonText = "Stop";

const ImVec4 BackgroundColor = ImVec4(0.080f * 3.0f, 0.080f * 3.0f, 0.080f * 3, 1.0f);
const ImVec4 InactiveButtonColor = ImVec4(0.080f * 1.0f, 0.080f * 1.0f, 0.080f * 1.0f, 1.0f);
const ImVec4 StartButtonColor = ImVec4(0.080f * 1.0f, 0.080f * 3.0f, 0.080f * 1.0f, 1.0f);
const ImVec4 StopButtonColor = ImVec4(0.080f * 3.0f, 0.080f * 1.0f, 0.080f * 1.0f, 1.0f);
const ImVec4 InputColor = ImVec4(0.080f * 1.6f, 0.080f * 1.6f, 0.080f * 1.6f, 1.0f);
const ImVec4 StartButtonHoveredColor = ImVec4(0.080f * 1.0f, 0.080f * 6.0f, 0.080f * 1.0f, 1.0f);
const ImVec4 StopButtonHoveredColor = ImVec4(0.080f * 6.0f, 0.080f * 1.0f, 0.080f * 1.0f, 1.0f);
const ImVec4 StartButtonActiveColor = ImVec4(0.080f * 1.0f, 0.080f * 9.0f, 0.080f * 1.0f, 1.0f);
const ImVec4 StopButtonActiveColor = ImVec4(0.080f * 9.0f, 0.080f * 1.0f, 0.080f * 1.0f, 1.0f);


void Start();

void Stop();

LRESULT CALLBACK HookProc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
);


int WINAPI WinMain(  
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR     lpCmdLine,
   _In_ int       nShowCmd)
{
   PowerClicker::Window::Init();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

   std::thread HookThread(
        []()
        {
            Hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, NULL, 0);

            MSG Message;
            while (GetMessage(&Message, 0, 0, 0));

        }
   );

   HookThread.detach();

    std::thread ClickThread(
        []()
        {
            while (true)
            {
                Sleep(MsDelay);
                if (Stopped || !Enabled)
                {
                    continue;
                }
                MouseAction(MouseButton, PosX, PosY);
            }
        }
    );

    ClickThread.detach();

    const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    PowerClicker::WindowSize Size{WINDOW_SIZE_X_SHARE, WINDOW_SIZE_Y_SHARE};

    Window = PowerClicker::Window(Size, MainPageTitle);
    Window.Create();
    Window.MakeContextCurrent();

    ImVec2 ButtonSize = ImVec2(
        204.0f,
        60.0f
    );

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Window.m_Window, true);
    ImGui_ImplOpenGL3_Init();


    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    while (!Window.ShouldClose())
    {

        if (!Running)
        {
            break;
        }

        PowerClicker::Window::PollEvents();

        PowerClicker::Context::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)Window.m_Width, (float)Window.m_Height));

        ImGui::PushStyleColor(ImGuiCol_WindowBg ,BackgroundColor);

        ImGui::Begin(
            "Test", 
            nullptr, 
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
        );
    

        ImGui::PopStyleColor();

        static bool SpecificCoordinates = false;
        static int Clicks = 1;
        static int Unit = 0;
        static int SelectedToggleKey = 0;
        static int SelectedTerminationKey = 0;

        ImGui::PushStyleColor(ImGuiCol_FrameBg, InputColor);

        if (ImGui::Combo(
            MouseButtonLabelText,
            &MouseButton,
            MouseButtons
        ))
            Stop();

        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        if (ImGui::InputInt(
            ClicksLabelText,
            &Clicks
        ))
            Stop();

        if (Clicks <= 0)
        {
            Clicks = 1;
        }

        if (ImGui::Combo(
            TimeUnitLabelText,
            &Unit,
            TimeUnits
        ))
            Stop();

        MsDelay = ToMilliseconds(Clicks, Unit);

        if (ImGui::Checkbox(SpecifyCoordinatesLabelText, &SpecificCoordinates))
            Stop();

        if (SpecificCoordinates)
        {
            static int Pos[2] = {};
            if (ImGui::InputInt2(CoordinatesInputLabelText, Pos))
                Stop();
            

            if (Pos[0] < 0)
                Pos[0] = 0;

            if (Pos[1] < 0)
                Pos[1] = 0;

            if (Pos[0] > VideoMode->width)
                Pos[0] = VideoMode->width;

            if (Pos[1] > VideoMode->height)
                Pos[1] = VideoMode->height;

            PosX = Pos[0];
            PosY = Pos[1];
        }else{
            PosX = POS_IDLE;
            PosY = POS_IDLE;
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
        }

        if (ImGui::Combo(
            HotkeyLabelText,
            &SelectedToggleKey,
            Keys
        ))
            Stop();
        
        ToggleKey = KeyCodes[SelectedToggleKey];

        if (ImGui::Combo(
            TerminationKeyLabelText,
            &SelectedTerminationKey,
            Keys
        ))
            Stop();

        TerminationKey = KeyCodes[SelectedTerminationKey];

        ImGui::Dummy(ImVec2(0.0f, 20.0f));
    
        if (ImGui::Checkbox(IconifyWindowOnStartLabelText, &IconifyWindowOnStart))
            Stop();

        if (ImGui::Checkbox(BlockInputOnToggleKeyPressedLabelText, &BlockInputOnToggleKeyPressed))
            Stop();

        if (ImGui::Checkbox("Enable on Start", &EnabledOnstart))
            Stop();

        ImGui::PopStyleColor();



        ImGui::Dummy(ImVec2(0.0f, 20.0f));


        if (Stopped)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, StartButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StartButtonHoveredColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, StartButtonActiveColor);
        }
        else if (!Stopped)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, InactiveButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, InactiveButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, InactiveButtonColor);
        }


        if (ImGui::Button(StartButtonText, ButtonSize))
            Start();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();


        if (Stopped)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, InactiveButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, InactiveButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, InactiveButtonColor);
        }
        else if (!Stopped)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, StopButtonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StopButtonHoveredColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, StopButtonActiveColor);
        }


        if (ImGui::Button(StopButtonText, ButtonSize))
            Stop();

        ImGui::PopStyleColor(3);

        ImGui::End();
        

        PowerClicker::Context::Render();

        Window.SwapBuffers();
    }
    Window.Destroy();

    PowerClicker::Context::Shutdown();

    PowerClicker::Window::Terminate();
    return 0;
}

void Start()
{
    Stopped = false;
    Enabled = EnabledOnstart;
    if (IconifyWindowOnStart)
    {
        Window.Iconify();
    }
}

void Stop()
{
    Stopped = true;
}

LRESULT CALLBACK HookProc(
    int nCode,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (nCode < 0 || Stopped)
    {
        return CallNextHookEx(Hook, nCode, wParam, lParam);        
    }


    if (wParam == WM_KEYDOWN)
    {
        int VkCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;

        if (VkCode == ToggleKey)
        {
            Enabled = !Enabled;
            if (BlockInputOnToggleKeyPressed)
            {
                return -1;
            }
        }
        else if (VkCode == TerminationKey)
        {
            Running = false;
            return -1;
        }
    }

    return CallNextHookEx(Hook, nCode, wParam, lParam);
}