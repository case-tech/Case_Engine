#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include "az_impl_win32.hpp"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <dwmapi.h>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

#ifndef AZ_IMPL_WIN32_DISABLE_GAMEPAD
#include <xinput.h>
typedef DWORD(WINAPI* PFN_XInputGetCapabilities)(DWORD,
                                                 DWORD,
                                                 XINPUT_CAPABILITIES*);
typedef DWORD(WINAPI* PFN_XInputGetState)(DWORD, XINPUT_STATE*);
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type"
#endif
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

static void Az_ImplWin32_InitMultiViewportSupport(bool platform_has_own_dc);
static void Az_ImplWin32_ShutdownMultiViewportSupport();
static void Az_ImplWin32_UpdateMonitors();

struct Az_ImplWin32_Data
{
    HWND hWnd;
    HWND MouseHwnd;
    int MouseTrackedArea;
    int MouseButtonsDown;
    INT64 Time;
    INT64 TicksPerSecond;
    AzMouseCursor LastMouseCursor;
    UINT32 KeyboardCodePage;
    bool WantUpdateMonitors;

#ifndef AZ_IMPL_WIN32_DISABLE_GAMEPAD
    bool HasGamepad;
    bool WantUpdateHasGamepad;
    HMODULE XInputDLL;
    PFN_XInputGetCapabilities XInputGetCapabilities;
    PFN_XInputGetState XInputGetState;
#endif

    Az_ImplWin32_Data()
    {
        memset((void*)this, 0, sizeof(*this));
    }
};

static Az_ImplWin32_Data* Az_ImplWin32_GetBackendData()
{
    return Az::GetCurrentContext()
                   ? (Az_ImplWin32_Data*)Az::GetIO().BackendPlatformUserData
                   : nullptr;
}
static Az_ImplWin32_Data* Az_ImplWin32_GetBackendData(AzIO& io)
{
    return (Az_ImplWin32_Data*)io.BackendPlatformUserData;
}

static void Az_AzplWin32_UpdateKeyboardCodePage(AzIO& io)
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData(io);
    HKL keyboard_layout = ::GetKeyboardLayout(0);
    LCID keyboard_lcid = MAKELCID(HIWORD(keyboard_layout), SORT_DEFAULT);
    if (::GetLocaleInfoA(keyboard_lcid,
                         (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE),
                         (LPSTR)&bd->KeyboardCodePage,
                         sizeof(bd->KeyboardCodePage))
        == 0)
        bd->KeyboardCodePage = CP_ACP;
}

static bool Az_AzplWin32_InitEx(void* hwnd, bool platform_has_own_dc)
{
    AzIO& io = Az::GetIO();
    AZ_CHECKVERSION();
    AZ_ASSERT(io.BackendPlatformUserData == nullptr
              && "Already initialized a platform backend!");

    INT64 perf_frequency, perf_counter;
    if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency))
        return false;
    if (!::QueryPerformanceCounter((LARGE_INTEGER*)&perf_counter))
        return false;

    Az_ImplWin32_Data* bd = AZ_NEW(Az_ImplWin32_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_win32";
    io.BackendFlags |= AzBackendFlags_HasMouseCursors;
    io.BackendFlags |= AzBackendFlags_HasSetMousePos;
    io.BackendFlags |= AzBackendFlags_PlatformHasViewports;
    io.BackendFlags |= AzBackendFlags_HasMouseHoveredViewport;

    bd->hWnd = (HWND)hwnd;
    bd->TicksPerSecond = perf_frequency;
    bd->Time = perf_counter;
    bd->LastMouseCursor = AzMouseCursor_COUNT;
    Az_AzplWin32_UpdateKeyboardCodePage(io);

    Az_ImplWin32_UpdateMonitors();

    AzViewport* main_viewport = Az::GetMainViewport();
    main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw =
            (void*)bd->hWnd;

    ::SetPropA(bd->hWnd, "AZ_CONTEXT", Az::GetCurrentContext());
    Az_ImplWin32_InitMultiViewportSupport(platform_has_own_dc);

#ifndef AZ_AZPL_WIN32_DISABLE_GAMEPAD
    bd->WantUpdateHasGamepad = true;
    const char* xinput_dll_names[] = {"xinput1_4.dll",
                                      "xinput1_3.dll",
                                      "xinput9_1_0.dll",
                                      "xinput1_2.dll",
                                      "xinput1_1.dll"};
    for (int n = 0; n < AZ_ARRAYSIZE(xinput_dll_names); n++)
        if (HMODULE dll = ::LoadLibraryA(xinput_dll_names[n]))
        {
            bd->XInputDLL = dll;
            bd->XInputGetCapabilities =
                    (PFN_XInputGetCapabilities)::GetProcAddress(
                            dll,
                            "XInputGetCapabilities");
            bd->XInputGetState =
                    (PFN_XInputGetState)::GetProcAddress(dll, "XInputGetState");
            break;
        }
#endif

    return true;
}

AZ_IMPL_API bool Az_AzplWin32_Init(void* hwnd)
{
    return Az_AzplWin32_InitEx(hwnd, false);
}

AZ_IMPL_API bool Az_AzplWin32_InitForOpenGL(void* hwnd)
{
    return Az_AzplWin32_InitEx(hwnd, true);
}

void Az_AzplWin32_Shutdown()
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData();
    AZ_ASSERT(bd != nullptr
              && "No platform backend to shutdown, or already shutdown?");
    AzIO& io = Az::GetIO();

    ::SetPropA(bd->hWnd, "AZ_CONTEXT", nullptr);
    Az_ImplWin32_ShutdownMultiViewportSupport();

#ifndef AZ_AZPL_WIN32_DISABLE_GAMEPAD
    if (bd->XInputDLL)
        ::FreeLibrary(bd->XInputDLL);
#endif

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(
            AzBackendFlags_HasMouseCursors | AzBackendFlags_HasSetMousePos
            | AzBackendFlags_HasGamepad | AzBackendFlags_PlatformHasViewports
            | AzBackendFlags_HasMouseHoveredViewport);
    AZ_DELETE(bd);
}

static bool Az_AzplWin32_UpdateMouseCursor(AzIO& io, AzMouseCursor imgui_cursor)
{
    if (io.ConfigFlags & AzConfigFlags_NoMouseCursorChange)
        return false;

    if (imgui_cursor == AzMouseCursor_None || io.MouseDrawCursor)
    {
        ::SetCursor(nullptr);
    } else
    {
        LPTSTR win32_cursor = IDC_ARROW;
        switch (imgui_cursor)
        {
        case AzMouseCursor_Arrow:
            win32_cursor = IDC_ARROW;
            break;
        case AzMouseCursor_TextInput:
            win32_cursor = IDC_IBEAM;
            break;
        case AzMouseCursor_ResizeAll:
            win32_cursor = IDC_SIZEALL;
            break;
        case AzMouseCursor_ResizeEW:
            win32_cursor = IDC_SIZEWE;
            break;
        case AzMouseCursor_ResizeNS:
            win32_cursor = IDC_SIZENS;
            break;
        case AzMouseCursor_ResizeNESW:
            win32_cursor = IDC_SIZENESW;
            break;
        case AzMouseCursor_ResizeNWSE:
            win32_cursor = IDC_SIZENWSE;
            break;
        case AzMouseCursor_Hand:
            win32_cursor = IDC_HAND;
            break;
        case AzMouseCursor_Wait:
            win32_cursor = IDC_WAIT;
            break;
        case AzMouseCursor_Progress:
            win32_cursor = IDC_APPSTARTING;
            break;
        case AzMouseCursor_NotAllowed:
            win32_cursor = IDC_NO;
            break;
        }
        ::SetCursor(::LoadCursor(nullptr, win32_cursor));
    }
    return true;
}

static bool IsVkDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) != 0;
}

static void Az_AzplWin32_AddKeyEvent(AzIO& io,
                                     AzKey key,
                                     bool down,
                                     int native_keycode,
                                     int native_scancode = -1)
{
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, native_scancode);
    AZ_UNUSED(native_scancode);
}

static void Az_AzplWin32_ProcessKeyEventsWorkarounds(AzIO& io)
{
    if (Az::IsKeyDown(AzKey_LeftShift) && !IsVkDown(VK_LSHIFT))
        Az_AzplWin32_AddKeyEvent(io, AzKey_LeftShift, false, VK_LSHIFT);
    if (Az::IsKeyDown(AzKey_RightShift) && !IsVkDown(VK_RSHIFT))
        Az_AzplWin32_AddKeyEvent(io, AzKey_RightShift, false, VK_RSHIFT);

    if (Az::IsKeyDown(AzKey_LeftSuper) && !IsVkDown(VK_LWIN))
        Az_AzplWin32_AddKeyEvent(io, AzKey_LeftSuper, false, VK_LWIN);
    if (Az::IsKeyDown(AzKey_RightSuper) && !IsVkDown(VK_RWIN))
        Az_AzplWin32_AddKeyEvent(io, AzKey_RightSuper, false, VK_RWIN);
}

static void Az_AzplWin32_UpdateKeyModifiers(AzIO& io)
{
    io.AddKeyEvent(AzMod_Ctrl, IsVkDown(VK_CONTROL));
    io.AddKeyEvent(AzMod_Shift, IsVkDown(VK_SHIFT));
    io.AddKeyEvent(AzMod_Alt, IsVkDown(VK_MENU));
    io.AddKeyEvent(AzMod_Super, IsVkDown(VK_LWIN) || IsVkDown(VK_RWIN));
}

static AzViewport* Az_AzplWin32_FindViewportByPlatformHandle(
        AzPlatformIO& platform_io,
        HWND hwnd)
{
    for (AzViewport* viewport : platform_io.Viewports)
        if (viewport->PlatformHandle == hwnd)
            return viewport;
    return nullptr;
}

static void Az_AzplWin32_UpdateMouseData(AzIO& io, AzPlatformIO& platform_io)
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData(io);
    AZ_ASSERT(bd->hWnd != 0);

    POINT mouse_screen_pos;
    bool has_mouse_screen_pos = ::GetCursorPos(&mouse_screen_pos) != 0;

    HWND focused_window = ::GetForegroundWindow();
    const bool is_app_focused =
            (focused_window
             && (focused_window == bd->hWnd
                 || ::IsChild(focused_window, bd->hWnd)
                 || Az_AzplWin32_FindViewportByPlatformHandle(platform_io,
                                                              focused_window)));
    if (is_app_focused)
    {
        if (io.WantSetMousePos)
        {
            POINT pos = {(int)io.MousePos.x, (int)io.MousePos.y};
            if ((io.ConfigFlags & AzConfigFlags_ViewportsEnable) == 0)
                ::ClientToScreen(focused_window, &pos);
            ::SetCursorPos(pos.x, pos.y);
        }

        if (!io.WantSetMousePos && bd->MouseTrackedArea == 0
            && has_mouse_screen_pos)
        {
            POINT mouse_pos = mouse_screen_pos;
            if (!(io.ConfigFlags & AzConfigFlags_ViewportsEnable))
                ::ScreenToClient(bd->hWnd, &mouse_pos);
            io.AddMousePosEvent((float)mouse_pos.x, (float)mouse_pos.y);
        }
    }

    AzID mouse_viewport_id = 0;
    if (has_mouse_screen_pos)
        if (HWND hovered_hwnd = ::WindowFromPoint(mouse_screen_pos))
            if (AzViewport* viewport =
                        Az_AzplWin32_FindViewportByPlatformHandle(platform_io,
                                                                  hovered_hwnd))
                mouse_viewport_id = viewport->ID;
    io.AddMouseViewportEvent(mouse_viewport_id);
}

static void Az_AzplWin32_UpdateGamepads(AzIO& io)
{
#ifndef AZ_AZPL_WIN32_DISABLE_GAMEPAD
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData(io);

    if (bd->WantUpdateHasGamepad)
    {
        XINPUT_CAPABILITIES caps = {};
        bd->HasGamepad =
                bd->XInputGetCapabilities
                        ? (bd->XInputGetCapabilities(0,
                                                     XINPUT_FLAG_GAMEPAD,
                                                     &caps)
                           == ERROR_SUCCESS)
                        : false;
        bd->WantUpdateHasGamepad = false;
    }

    io.BackendFlags &= ~AzBackendFlags_HasGamepad;
    XINPUT_STATE xinput_state;
    XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;
    if (!bd->HasGamepad || bd->XInputGetState == nullptr
        || bd->XInputGetState(0, &xinput_state) != ERROR_SUCCESS)
        return;
    io.BackendFlags |= AzBackendFlags_HasGamepad;

#define AZ_SATURATE(V) (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
#define MAP_BUTTON(KEY_NO, BUTTON_ENUM)                                        \
    {                                                                          \
        io.AddKeyEvent(KEY_NO, (gamepad.wButtons & BUTTON_ENUM) != 0);         \
    }
#define MAP_ANALOG(KEY_NO, VALUE, V0, V1)                                      \
    {                                                                          \
        float vn = (float)(VALUE - V0) / (float)(V1 - V0);                     \
        io.AddKeyAnalogEvent(KEY_NO, vn > 0.10f, AZ_SATURATE(vn));             \
    }
    MAP_BUTTON(AzKey_GamepadStart, XINPUT_GAMEPAD_START);
    MAP_BUTTON(AzKey_GamepadBack, XINPUT_GAMEPAD_BACK);
    MAP_BUTTON(AzKey_GamepadFaceLeft, XINPUT_GAMEPAD_X);
    MAP_BUTTON(AzKey_GamepadFaceRight, XINPUT_GAMEPAD_B);
    MAP_BUTTON(AzKey_GamepadFaceUp, XINPUT_GAMEPAD_Y);
    MAP_BUTTON(AzKey_GamepadFaceDown, XINPUT_GAMEPAD_A);
    MAP_BUTTON(AzKey_GamepadDpadLeft, XINPUT_GAMEPAD_DPAD_LEFT);
    MAP_BUTTON(AzKey_GamepadDpadRight, XINPUT_GAMEPAD_DPAD_RIGHT);
    MAP_BUTTON(AzKey_GamepadDpadUp, XINPUT_GAMEPAD_DPAD_UP);
    MAP_BUTTON(AzKey_GamepadDpadDown, XINPUT_GAMEPAD_DPAD_DOWN);
    MAP_BUTTON(AzKey_GamepadL1, XINPUT_GAMEPAD_LEFT_SHOULDER);
    MAP_BUTTON(AzKey_GamepadR1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
    MAP_ANALOG(AzKey_GamepadL2,
               gamepad.bLeftTrigger,
               XINPUT_GAMEPAD_TRIGGER_THRESHOLD,
               255);
    MAP_ANALOG(AzKey_GamepadR2,
               gamepad.bRightTrigger,
               XINPUT_GAMEPAD_TRIGGER_THRESHOLD,
               255);
    MAP_BUTTON(AzKey_GamepadL3, XINPUT_GAMEPAD_LEFT_THUMB);
    MAP_BUTTON(AzKey_GamepadR3, XINPUT_GAMEPAD_RIGHT_THUMB);
    MAP_ANALOG(AzKey_GamepadLStickLeft,
               gamepad.sThumbLX,
               -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               -32768);
    MAP_ANALOG(AzKey_GamepadLStickRight,
               gamepad.sThumbLX,
               +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               +32767);
    MAP_ANALOG(AzKey_GamepadLStickUp,
               gamepad.sThumbLY,
               +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               +32767);
    MAP_ANALOG(AzKey_GamepadLStickDown,
               gamepad.sThumbLY,
               -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               -32768);
    MAP_ANALOG(AzKey_GamepadRStickLeft,
               gamepad.sThumbRX,
               -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               -32768);
    MAP_ANALOG(AzKey_GamepadRStickRight,
               gamepad.sThumbRX,
               +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               +32767);
    MAP_ANALOG(AzKey_GamepadRStickUp,
               gamepad.sThumbRY,
               +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               +32767);
    MAP_ANALOG(AzKey_GamepadRStickDown,
               gamepad.sThumbRY,
               -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
               -32768);
#undef MAP_BUTTON
#undef MAP_ANALOG
#else
    AZ_UNUSED(io);
#endif
}

static BOOL CALLBACK Az_AzplWin32_UpdateMonitors_EnumFunc(HMONITOR monitor,
                                                          HDC,
                                                          LPRECT,
                                                          LPARAM)
{
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    if (!::GetMonitorInfo(monitor, &info))
        return TRUE;
    AzPlatformMonitor imgui_monitor;
    imgui_monitor.MainPos = AzVec2((float)info.rcMonitor.left,
                                   (float)info.rcMonitor.top);
    imgui_monitor.MainSize =
            AzVec2((float)(info.rcMonitor.right - info.rcMonitor.left),
                   (float)(info.rcMonitor.bottom - info.rcMonitor.top));
    imgui_monitor.WorkPos = AzVec2((float)info.rcWork.left,
                                   (float)info.rcWork.top);
    imgui_monitor.WorkSize =
            AzVec2((float)(info.rcWork.right - info.rcWork.left),
                   (float)(info.rcWork.bottom - info.rcWork.top));
    imgui_monitor.DpiScale = Az_AzplWin32_GetDpiScaleForMonitor(monitor);
    imgui_monitor.PlatformHandle = (void*)monitor;
    if (imgui_monitor.DpiScale <= 0.0f)
        return TRUE;
    AzPlatformIO& io = Az::GetPlatformIO();
    if (info.dwFlags & MONITORINFOF_PRIMARY)
        io.Monitors.push_front(imgui_monitor);
    else
        io.Monitors.push_back(imgui_monitor);
    return TRUE;
}

static void Az_ImplWin32_UpdateMonitors()
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData();
    Az::GetPlatformIO().Monitors.resize(0);
    ::EnumDisplayMonitors(nullptr,
                          nullptr,
                          Az_AzplWin32_UpdateMonitors_EnumFunc,
                          0);
    bd->WantUpdateMonitors = false;
}

void Az_AzplWin32_NewFrame()
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData();
    AZ_ASSERT(bd != nullptr && "Context or backend not initialized? Did you call Az_AzplWin32_Init()?");
    AzIO& io = Az::GetIO();
    AzPlatformIO& platform_io = Az::GetPlatformIO();

    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(bd->hWnd, &rect);
    io.DisplaySize = AzVec2((float)(rect.right - rect.left),
                            (float)(rect.bottom - rect.top));
    if (bd->WantUpdateMonitors)
        Az_ImplWin32_UpdateMonitors();

    INT64 current_time = 0;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    io.DeltaTime = (float)(current_time - bd->Time) / bd->TicksPerSecond;
    bd->Time = current_time;

    Az_AzplWin32_UpdateMouseData(io, platform_io);

    Az_AzplWin32_ProcessKeyEventsWorkarounds(io);

    AzMouseCursor mouse_cursor = io.MouseDrawCursor ? AzMouseCursor_None
                                                    : Az::GetMouseCursor();
    if (bd->LastMouseCursor != mouse_cursor)
    {
        bd->LastMouseCursor = mouse_cursor;
        Az_AzplWin32_UpdateMouseCursor(io, mouse_cursor);
    }

    Az_AzplWin32_UpdateGamepads(io);
}

AzKey Az_AzplWin32_KeyEventToAzKey(WPARAM wParam, LPARAM lParam);
AzKey Az_AzplWin32_KeyEventToAzKey(WPARAM wParam, LPARAM lParam)
{
    if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
        return AzKey_KeypadEnter;

    const int scancode = (int)LOBYTE(HIWORD(lParam));

    switch (wParam)
    {
    case VK_TAB:
        return AzKey_Tab;
    case VK_LEFT:
        return AzKey_LeftArrow;
    case VK_RIGHT:
        return AzKey_RightArrow;
    case VK_UP:
        return AzKey_UpArrow;
    case VK_DOWN:
        return AzKey_DownArrow;
    case VK_PRIOR:
        return AzKey_PageUp;
    case VK_NEXT:
        return AzKey_PageDown;
    case VK_HOME:
        return AzKey_Home;
    case VK_END:
        return AzKey_End;
    case VK_INSERT:
        return AzKey_Insert;
    case VK_DELETE:
        return AzKey_Delete;
    case VK_BACK:
        return AzKey_Backspace;
    case VK_SPACE:
        return AzKey_Space;
    case VK_RETURN:
        return AzKey_Enter;
    case VK_ESCAPE:
        return AzKey_Escape;

    case VK_OEM_COMMA:
        return AzKey_Comma;

    case VK_OEM_PERIOD:
        return AzKey_Period;

    case VK_CAPITAL:
        return AzKey_CapsLock;
    case VK_SCROLL:
        return AzKey_ScrollLock;
    case VK_NUMLOCK:
        return AzKey_NumLock;
    case VK_SNAPSHOT:
        return AzKey_PrintScreen;
    case VK_PAUSE:
        return AzKey_Pause;
    case VK_NUMPAD0:
        return AzKey_Keypad0;
    case VK_NUMPAD1:
        return AzKey_Keypad1;
    case VK_NUMPAD2:
        return AzKey_Keypad2;
    case VK_NUMPAD3:
        return AzKey_Keypad3;
    case VK_NUMPAD4:
        return AzKey_Keypad4;
    case VK_NUMPAD5:
        return AzKey_Keypad5;
    case VK_NUMPAD6:
        return AzKey_Keypad6;
    case VK_NUMPAD7:
        return AzKey_Keypad7;
    case VK_NUMPAD8:
        return AzKey_Keypad8;
    case VK_NUMPAD9:
        return AzKey_Keypad9;
    case VK_DECIMAL:
        return AzKey_KeypadDecimal;
    case VK_DIVIDE:
        return AzKey_KeypadDivide;
    case VK_MULTIPLY:
        return AzKey_KeypadMultiply;
    case VK_SUBTRACT:
        return AzKey_KeypadSubtract;
    case VK_ADD:
        return AzKey_KeypadAdd;
    case VK_LSHIFT:
        return AzKey_LeftShift;
    case VK_LCONTROL:
        return AzKey_LeftCtrl;
    case VK_LMENU:
        return AzKey_LeftAlt;
    case VK_LWIN:
        return AzKey_LeftSuper;
    case VK_RSHIFT:
        return AzKey_RightShift;
    case VK_RCONTROL:
        return AzKey_RightCtrl;
    case VK_RMENU:
        return AzKey_RightAlt;
    case VK_RWIN:
        return AzKey_RightSuper;
    case VK_APPS:
        return AzKey_Menu;
    case '0':
        return AzKey_0;
    case '1':
        return AzKey_1;
    case '2':
        return AzKey_2;
    case '3':
        return AzKey_3;
    case '4':
        return AzKey_4;
    case '5':
        return AzKey_5;
    case '6':
        return AzKey_6;
    case '7':
        return AzKey_7;
    case '8':
        return AzKey_8;
    case '9':
        return AzKey_9;
    case 'A':
        return AzKey_A;
    case 'B':
        return AzKey_B;
    case 'C':
        return AzKey_C;
    case 'D':
        return AzKey_D;
    case 'E':
        return AzKey_E;
    case 'F':
        return AzKey_F;
    case 'G':
        return AzKey_G;
    case 'H':
        return AzKey_H;
    case 'I':
        return AzKey_I;
    case 'J':
        return AzKey_J;
    case 'K':
        return AzKey_K;
    case 'L':
        return AzKey_L;
    case 'M':
        return AzKey_M;
    case 'N':
        return AzKey_N;
    case 'O':
        return AzKey_O;
    case 'P':
        return AzKey_P;
    case 'Q':
        return AzKey_Q;
    case 'R':
        return AzKey_R;
    case 'S':
        return AzKey_S;
    case 'T':
        return AzKey_T;
    case 'U':
        return AzKey_U;
    case 'V':
        return AzKey_V;
    case 'W':
        return AzKey_W;
    case 'X':
        return AzKey_X;
    case 'Y':
        return AzKey_Y;
    case 'Z':
        return AzKey_Z;
    case VK_F1:
        return AzKey_F1;
    case VK_F2:
        return AzKey_F2;
    case VK_F3:
        return AzKey_F3;
    case VK_F4:
        return AzKey_F4;
    case VK_F5:
        return AzKey_F5;
    case VK_F6:
        return AzKey_F6;
    case VK_F7:
        return AzKey_F7;
    case VK_F8:
        return AzKey_F8;
    case VK_F9:
        return AzKey_F9;
    case VK_F10:
        return AzKey_F10;
    case VK_F11:
        return AzKey_F11;
    case VK_F12:
        return AzKey_F12;
    case VK_F13:
        return AzKey_F13;
    case VK_F14:
        return AzKey_F14;
    case VK_F15:
        return AzKey_F15;
    case VK_F16:
        return AzKey_F16;
    case VK_F17:
        return AzKey_F17;
    case VK_F18:
        return AzKey_F18;
    case VK_F19:
        return AzKey_F19;
    case VK_F20:
        return AzKey_F20;
    case VK_F21:
        return AzKey_F21;
    case VK_F22:
        return AzKey_F22;
    case VK_F23:
        return AzKey_F23;
    case VK_F24:
        return AzKey_F24;
    case VK_BROWSER_BACK:
        return AzKey_AppBack;
    case VK_BROWSER_FORWARD:
        return AzKey_AppForward;
    default:
        break;
    }

    switch (scancode)
    {
    case 41:
        return AzKey_GraveAccent;
    case 12:
        return AzKey_Minus;
    case 13:
        return AzKey_Equal;
    case 26:
        return AzKey_LeftBracket;
    case 27:
        return AzKey_RightBracket;
    case 86:
        return AzKey_Oem102;
    case 43:
        return AzKey_Backslash;
    case 39:
        return AzKey_Semicolon;
    case 40:
        return AzKey_Apostrophe;
    case 51:
        return AzKey_Comma;
    case 52:
        return AzKey_Period;
    case 53:
        return AzKey_Slash;
    }

    return AzKey_None;
}

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

static AzMouseSource Az_AzplWin32_GetMouseSourceFromMessageExtraInfo()
{
    LPARAM extra_info = ::GetMessageExtraInfo();
    if ((extra_info & 0xFFFFFF80) == 0xFF515700)
        return AzMouseSource_Pen;
    if ((extra_info & 0xFFFFFF80) == 0xFF515780)
        return AzMouseSource_TouchScreen;
    return AzMouseSource_Mouse;
}

extern AZ_IMPL_API LRESULT Az_AzplWin32_WndProcHandler(HWND hWnd,
                                                       UINT msg,
                                                       WPARAM wParam,
                                                       LPARAM lParam);
extern AZ_IMPL_API LRESULT Az_AzplWin32_WndProcHandlerEx(HWND hWnd,
                                                         UINT msg,
                                                         WPARAM wParam,
                                                         LPARAM lParam,
                                                         AzIO& io);

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif

AZ_IMPL_API LRESULT Az_AzplWin32_WndProcHandler(HWND hwnd,
                                                UINT msg,
                                                WPARAM wParam,
                                                LPARAM lParam)
{
    if (Az::GetCurrentContext() == nullptr)
        return 0;
    return Az_AzplWin32_WndProcHandlerEx(hwnd,
                                         msg,
                                         wParam,
                                         lParam,
                                         Az::GetIO());
}

AZ_IMPL_API LRESULT Az_AzplWin32_WndProcHandlerEx(HWND hwnd,
                                                  UINT msg,
                                                  WPARAM wParam,
                                                  LPARAM lParam,
                                                  AzIO& io)
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData(io);
    if (bd == nullptr)
        return 0;
    switch (msg)
    {
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE: {
        AzMouseSource mouse_source =
                Az_AzplWin32_GetMouseSourceFromMessageExtraInfo();
        const int area = (msg == WM_MOUSEMOVE) ? 1 : 2;
        bd->MouseHwnd = hwnd;
        if (bd->MouseTrackedArea != area)
        {
            TRACKMOUSEEVENT tme_cancel = {sizeof(tme_cancel),
                                          TME_CANCEL,
                                          hwnd,
                                          0};
            TRACKMOUSEEVENT tme_track = {sizeof(tme_track),
                                         (DWORD)((area == 2) ? (TME_LEAVE
                                                                | TME_NONCLIENT)
                                                             : TME_LEAVE),
                                         hwnd,
                                         0};
            if (bd->MouseTrackedArea != 0)
                ::TrackMouseEvent(&tme_cancel);
            ::TrackMouseEvent(&tme_track);
            bd->MouseTrackedArea = area;
        }
        POINT mouse_pos = {(LONG)GET_X_LPARAM(lParam),
                           (LONG)GET_Y_LPARAM(lParam)};
        bool want_absolute_pos = (io.ConfigFlags
                                  & AzConfigFlags_ViewportsEnable)
                                 != 0;
        if (msg == WM_MOUSEMOVE && want_absolute_pos)
            ::ClientToScreen(hwnd, &mouse_pos);
        if (msg == WM_NCMOUSEMOVE && !want_absolute_pos)
            ::ScreenToClient(hwnd, &mouse_pos);
        io.AddMouseSourceEvent(mouse_source);
        io.AddMousePosEvent((float)mouse_pos.x, (float)mouse_pos.y);
        return 0;
    }
    case WM_MOUSELEAVE:
    case WM_NCMOUSELEAVE: {
        const int area = (msg == WM_MOUSELEAVE) ? 1 : 2;
        if (bd->MouseTrackedArea == area)
        {
            if (bd->MouseHwnd == hwnd)
                bd->MouseHwnd = nullptr;
            bd->MouseTrackedArea = 0;
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
        return 0;
    }
    case WM_DESTROY:
        if (bd->MouseHwnd == hwnd && bd->MouseTrackedArea != 0)
        {
            TRACKMOUSEEVENT tme_cancel = {sizeof(tme_cancel),
                                          TME_CANCEL,
                                          hwnd,
                                          0};
            ::TrackMouseEvent(&tme_cancel);
            bd->MouseHwnd = nullptr;
            bd->MouseTrackedArea = 0;
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
        return 0;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK: {
        AzMouseSource mouse_source =
                Az_AzplWin32_GetMouseSourceFromMessageExtraInfo();
        int button = 0;
        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
        {
            button = 0;
        }
        if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
        {
            button = 1;
        }
        if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK)
        {
            button = 2;
        }
        if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK)
        {
            button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
        }
        HWND hwnd_with_capture = ::GetCapture();
        if (bd->MouseButtonsDown != 0 && hwnd_with_capture != hwnd)
            bd->MouseButtonsDown = 0;
        if (bd->MouseButtonsDown == 0 && hwnd_with_capture == nullptr)
            ::SetCapture(hwnd);
        bd->MouseButtonsDown |= 1 << button;
        io.AddMouseSourceEvent(mouse_source);
        io.AddMouseButtonEvent(button, true);
        return 0;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP: {
        AzMouseSource mouse_source =
                Az_AzplWin32_GetMouseSourceFromMessageExtraInfo();
        int button = 0;
        if (msg == WM_LBUTTONUP)
        {
            button = 0;
        }
        if (msg == WM_RBUTTONUP)
        {
            button = 1;
        }
        if (msg == WM_MBUTTONUP)
        {
            button = 2;
        }
        if (msg == WM_XBUTTONUP)
        {
            button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
        }
        bd->MouseButtonsDown &= ~(1 << button);
        if (bd->MouseButtonsDown == 0 && ::GetCapture() == hwnd)
            ::ReleaseCapture();
        io.AddMouseSourceEvent(mouse_source);
        io.AddMouseButtonEvent(button, false);
        return 0;
    }
    case WM_MOUSEWHEEL:
        io.AddMouseWheelEvent(0.0f,
                              (float)GET_WHEEL_DELTA_WPARAM(wParam)
                                      / (float)WHEEL_DELTA);
        return 0;
    case WM_MOUSEHWHEEL:
        io.AddMouseWheelEvent(-(float)GET_WHEEL_DELTA_WPARAM(wParam)
                                      / (float)WHEEL_DELTA,
                              0.0f);
        return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP: {
        const bool is_key_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        if (wParam < 256)
        {
            Az_AzplWin32_UpdateKeyModifiers(io);

            const AzKey key = Az_AzplWin32_KeyEventToAzKey(wParam, lParam);
            const int vk = (int)wParam;
            const int scancode = (int)LOBYTE(HIWORD(lParam));

            if (key == AzKey_PrintScreen && !is_key_down)
                Az_AzplWin32_AddKeyEvent(io, key, true, vk, scancode);

            if (key != AzKey_None)
                Az_AzplWin32_AddKeyEvent(io, key, is_key_down, vk, scancode);

            if (vk == VK_SHIFT)
            {
                if (IsVkDown(VK_LSHIFT) == is_key_down)
                {
                    Az_AzplWin32_AddKeyEvent(io,
                                             AzKey_LeftShift,
                                             is_key_down,
                                             VK_LSHIFT,
                                             scancode);
                }
                if (IsVkDown(VK_RSHIFT) == is_key_down)
                {
                    Az_AzplWin32_AddKeyEvent(io,
                                             AzKey_RightShift,
                                             is_key_down,
                                             VK_RSHIFT,
                                             scancode);
                }
            } else if (vk == VK_CONTROL)
            {
                if (IsVkDown(VK_LCONTROL) == is_key_down)
                {
                    Az_AzplWin32_AddKeyEvent(io,
                                             AzKey_LeftCtrl,
                                             is_key_down,
                                             VK_LCONTROL,
                                             scancode);
                }
                if (IsVkDown(VK_RCONTROL) == is_key_down)
                {
                    Az_AzplWin32_AddKeyEvent(io,
                                             AzKey_RightCtrl,
                                             is_key_down,
                                             VK_RCONTROL,
                                             scancode);
                }
            } else if (vk == VK_MENU)
            {
                if (IsVkDown(VK_LMENU) == is_key_down)
                {
                    Az_AzplWin32_AddKeyEvent(io,
                                             AzKey_LeftAlt,
                                             is_key_down,
                                             VK_LMENU,
                                             scancode);
                }
                if (IsVkDown(VK_RMENU) == is_key_down)
                {
                    Az_AzplWin32_AddKeyEvent(io,
                                             AzKey_RightAlt,
                                             is_key_down,
                                             VK_RMENU,
                                             scancode);
                }
            }
        }
        return 0;
    }
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        io.AddFocusEvent(msg == WM_SETFOCUS);
        return 0;
    case WM_INPUTLANGCHANGE:
        Az_AzplWin32_UpdateKeyboardCodePage(io);
        return 0;
    case WM_CHAR:
        if (::IsWindowUnicode(hwnd))
        {
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacterUTF16((unsigned short)wParam);
        } else
        {
            wchar_t wch = 0;
            ::MultiByteToWideChar(bd->KeyboardCodePage,
                                  MB_PRECOMPOSED,
                                  (char*)&wParam,
                                  1,
                                  &wch,
                                  1);
            io.AddInputCharacter(wch);
        }
        return 0;
    case WM_SETCURSOR:

        if (LOWORD(lParam) == HTCLIENT
            && Az_AzplWin32_UpdateMouseCursor(io, bd->LastMouseCursor))
            return 1;
        return 0;
    case WM_DEVICECHANGE:
#ifndef AZ_AZPL_WIN32_DISABLE_GAMEPAD
        if ((UINT)wParam == DBT_DEVNODES_CHANGED)
            bd->WantUpdateHasGamepad = true;
#endif
        return 0;
    case WM_DISPLAYCHANGE:
        bd->WantUpdateMonitors = true;
        return 0;
    case WM_SETTINGCHANGE:
        if (wParam == SPI_SETWORKAREA)
            bd->WantUpdateMonitors = true;
        return 0;
    case WM_DPICHANGED: {
        const RECT* suggested_rect = (RECT*)lParam;
        if (io.ConfigDpiScaleViewports)
            ::SetWindowPos(hwnd,
                           nullptr,
                           suggested_rect->left,
                           suggested_rect->top,
                           suggested_rect->right - suggested_rect->left,
                           suggested_rect->bottom - suggested_rect->top,
                           SWP_NOZORDER | SWP_NOACTIVATE);
        return 0;
    }
    }
    return 0;
}

static BOOL _IsWindowsVersionOrGreater(WORD major, WORD minor, WORD)
{
    typedef LONG(WINAPI * PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*,
                                                    ULONG,
                                                    ULONGLONG);
    static PFN_RtlVerifyVersionInfo RtlVerifyVersionInfoFn = nullptr;
    if (RtlVerifyVersionInfoFn == nullptr)
        if (HMODULE ntdllModule = ::GetModuleHandleA("ntdll.dll"))
            RtlVerifyVersionInfoFn = (PFN_RtlVerifyVersionInfo)
                    GetProcAddress(ntdllModule, "RtlVerifyVersionInfo");
    if (RtlVerifyVersionInfoFn == nullptr)
        return FALSE;

    RTL_OSVERSIONINFOEXW versionInfo = {};
    ULONGLONG conditionMask = 0;
    versionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
    versionInfo.dwMajorVersion = major;
    versionInfo.dwMinorVersion = minor;
    VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    return (RtlVerifyVersionInfoFn(&versionInfo,
                                   VER_MAJORVERSION | VER_MINORVERSION,
                                   conditionMask)
            == 0)
                   ? TRUE
                   : FALSE;
}

#define _IsWindowsVistaOrGreater()                                             \
    _IsWindowsVersionOrGreater(HIBYTE(0x0600), LOBYTE(0x0600), 0)
#define _IsWindows8OrGreater()                                                 \
    _IsWindowsVersionOrGreater(HIBYTE(0x0602), LOBYTE(0x0602), 0)
#define _IsWindows8Point1OrGreater()                                           \
    _IsWindowsVersionOrGreater(HIBYTE(0x0603), LOBYTE(0x0603), 0)
#define _IsWindows10OrGreater()                                                \
    _IsWindowsVersionOrGreater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0)

#ifndef DPI_ENUMS_DECLARED
typedef enum
{
    PROCESS_DPI_UNAWARE = 0,
    PROCESS_SYSTEM_DPI_AWARE = 1,
    PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;
typedef enum
{
    MDT_EFFECTIVE_DPI = 0,
    MDT_ANGULAR_DPI = 1,
    MDT_RAW_DPI = 2,
    MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;
#endif
#ifndef _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE (DPI_AWARENESS_CONTEXT) - 3
#endif
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 (DPI_AWARENESS_CONTEXT) - 4
#endif
typedef HRESULT(WINAPI* PFN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);
typedef HRESULT(WINAPI* PFN_GetDpiForMonitor)(HMONITOR,
                                              MONITOR_DPI_TYPE,
                                              UINT*,
                                              UINT*);
typedef DPI_AWARENESS_CONTEXT(WINAPI* PFN_SetThreadDpiAwarenessContext)(
        DPI_AWARENESS_CONTEXT);

void Az_AzplWin32_EnableDpiAwareness()
{
    if (Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData())
        bd->WantUpdateMonitors = true;

    if (_IsWindows10OrGreater())
    {
        static HINSTANCE user32_dll = ::LoadLibraryA("user32.dll");
        if (PFN_SetThreadDpiAwarenessContext SetThreadDpiAwarenessContextFn =
                    (PFN_SetThreadDpiAwarenessContext)::GetProcAddress(
                            user32_dll,
                            "SetThreadDpiAwarenessContext"))
        {
            SetThreadDpiAwarenessContextFn(
                    DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll");
        if (PFN_SetProcessDpiAwareness SetProcessDpiAwarenessFn =
                    (PFN_SetProcessDpiAwareness)::GetProcAddress(
                            shcore_dll,
                            "SetProcessDpiAwareness"))
        {
            SetProcessDpiAwarenessFn(PROCESS_PER_MONITOR_DPI_AWARE);
            return;
        }
    }
#if _WIN32_WINNT >= 0x0600
    ::SetProcessDPIAware();
#endif
}

#if defined(_MSC_VER) && !defined(NOGDI)
#pragma comment(lib, "gdi32")
#endif

float Az_AzplWin32_GetDpiScaleForMonitor(void* monitor)
{
    UINT xdpi = 96, ydpi = 96;
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll");
        static PFN_GetDpiForMonitor GetDpiForMonitorFn = nullptr;
        if (GetDpiForMonitorFn == nullptr && shcore_dll != nullptr)
            GetDpiForMonitorFn =
                    (PFN_GetDpiForMonitor)::GetProcAddress(shcore_dll,
                                                           "GetDpiForMonitor");
        if (GetDpiForMonitorFn != nullptr)
        {
            GetDpiForMonitorFn((HMONITOR)monitor,
                               MDT_EFFECTIVE_DPI,
                               &xdpi,
                               &ydpi);
            AZ_ASSERT(xdpi == ydpi);
            return xdpi / 96.0f;
        }
    }
#ifndef NOGDI
    const HDC dc = ::GetDC(nullptr);
    xdpi = ::GetDeviceCaps(dc, LOGPIXELSX);
    ydpi = ::GetDeviceCaps(dc, LOGPIXELSY);
    AZ_ASSERT(xdpi == ydpi);
    ::ReleaseDC(nullptr, dc);
#endif
    return xdpi / 96.0f;
}

float Az_AzplWin32_GetDpiScaleForHwnd(void* hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow((HWND)hwnd,
                                           MONITOR_DEFAULTTONEAREST);
    return Az_AzplWin32_GetDpiScaleForMonitor(monitor);
}

#if defined(_MSC_VER)
#pragma comment(lib, "dwmapi")
#endif

void Az_AzplWin32_EnableAlphaCompositing(void* hwnd)
{
    if (!_IsWindowsVistaOrGreater())
        return;

    BOOL composition;
    if (FAILED(::DwmIsCompositionEnabled(&composition)) || !composition)
        return;

    BOOL opaque;
    DWORD color;
    if (_IsWindows8OrGreater()
        || (SUCCEEDED(::DwmGetColorizationColor(&color, &opaque)) && !opaque))
    {
        HRGN region = ::CreateRectRgn(0, 0, -1, -1);
        DWM_BLURBEHIND bb = {};
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = region;
        bb.fEnable = TRUE;
        ::DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
        ::DeleteObject(region);
    } else
    {
        DWM_BLURBEHIND bb = {};
        bb.dwFlags = DWM_BB_ENABLE;
        ::DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
    }
}

struct Az_AzplWin32_ViewportData
{
    HWND Hwnd;
    HWND HwndParent;
    bool HwndOwned;
    DWORD DwStyle;
    DWORD DwExStyle;

    Az_AzplWin32_ViewportData()
    {
        Hwnd = HwndParent = nullptr;
        HwndOwned = false;
        DwStyle = DwExStyle = 0;
    }
    ~Az_AzplWin32_ViewportData()
    {
        AZ_ASSERT(Hwnd == nullptr);
    }
};

static void Az_AzplWin32_GetWin32StyleFromViewportFlags(AzViewportFlags flags,
                                                        DWORD* out_style,
                                                        DWORD* out_ex_style)
{
    if (flags & AzViewportFlags_NoDecoration)
        *out_style = WS_POPUP;
    else
        *out_style = WS_OVERLAPPEDWINDOW;

    if (flags & AzViewportFlags_NoTaskBarIcon)
        *out_ex_style = WS_EX_TOOLWINDOW;
    else
        *out_ex_style = WS_EX_APPWINDOW;

    if (flags & AzViewportFlags_TopMost)
        *out_ex_style |= WS_EX_TOPMOST;
}

static HWND Az_AzplWin32_GetHwndFromViewportID(AzID viewport_id)
{
    if (viewport_id != 0)
        if (AzViewport* viewport = Az::FindViewportByID(viewport_id))
            return (HWND)viewport->PlatformHandle;
    return nullptr;
}

static void Az_AzplWin32_CreateWindow(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = AZ_NEW(Az_AzplWin32_ViewportData)();
    viewport->PlatformUserData = vd;

    Az_AzplWin32_GetWin32StyleFromViewportFlags(viewport->Flags,
                                                &vd->DwStyle,
                                                &vd->DwExStyle);
    vd->HwndParent = Az_AzplWin32_GetHwndFromViewportID(
            viewport->ParentViewportId);

    RECT rect = {(LONG)viewport->Pos.x,
                 (LONG)viewport->Pos.y,
                 (LONG)(viewport->Pos.x + viewport->Size.x),
                 (LONG)(viewport->Pos.y + viewport->Size.y)};
    ::AdjustWindowRectEx(&rect, vd->DwStyle, FALSE, vd->DwExStyle);
    vd->Hwnd = ::CreateWindowExW(vd->DwExStyle,
                                 L"Az Platform",
                                 L"Untitled",
                                 vd->DwStyle,
                                 rect.left,
                                 rect.top,
                                 rect.right - rect.left,
                                 rect.bottom - rect.top,
                                 vd->HwndParent,
                                 nullptr,
                                 ::GetModuleHandle(nullptr),
                                 nullptr);
    vd->HwndOwned = true;
    viewport->PlatformRequestResize = false;
    viewport->PlatformHandle = viewport->PlatformHandleRaw = vd->Hwnd;

    ::SetPropA(vd->Hwnd, "AZ_CONTEXT", Az::GetCurrentContext());
}

static void Az_AzplWin32_DestroyWindow(AzViewport* viewport)
{
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData();
    if (Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                                viewport->PlatformUserData)
    {
        if (::GetCapture() == vd->Hwnd)
        {
            ::ReleaseCapture();
            ::SetCapture(bd->hWnd);
        }
        if (vd->Hwnd && vd->HwndOwned)
            ::DestroyWindow(vd->Hwnd);
        vd->Hwnd = nullptr;
        AZ_DELETE(vd);
    }
    viewport->PlatformUserData = viewport->PlatformHandle = nullptr;
}

static void Az_AzplWin32_ShowWindow(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);

    if (vd->HwndParent != NULL)
        ::SetWindowLongPtr(vd->Hwnd, GWLP_HWNDPARENT, (LONG_PTR) nullptr);

    if (viewport->Flags & AzViewportFlags_NoFocusOnAppearing)
        ::ShowWindow(vd->Hwnd, SW_SHOWNA);
    else
        ::ShowWindow(vd->Hwnd, SW_SHOW);

    if (vd->HwndParent != NULL)
        ::SetWindowLongPtr(vd->Hwnd, GWLP_HWNDPARENT, (LONG_PTR)vd->HwndParent);
}

static void Az_AzplWin32_UpdateWindow(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);

    HWND new_parent = Az_AzplWin32_GetHwndFromViewportID(
            viewport->ParentViewportId);
    if (new_parent != vd->HwndParent)
    {
        vd->HwndParent = new_parent;
        ::SetWindowLongPtr(vd->Hwnd, GWLP_HWNDPARENT, (LONG_PTR)vd->HwndParent);
    }

    DWORD new_style;
    DWORD new_ex_style;
    Az_AzplWin32_GetWin32StyleFromViewportFlags(viewport->Flags,
                                                &new_style,
                                                &new_ex_style);

    if (vd->DwStyle != new_style || vd->DwExStyle != new_ex_style)
    {
        bool top_most_changed = (vd->DwExStyle & WS_EX_TOPMOST)
                                != (new_ex_style & WS_EX_TOPMOST);
        HWND insert_after = top_most_changed ? ((viewport->Flags
                                                 & AzViewportFlags_TopMost)
                                                        ? HWND_TOPMOST
                                                        : HWND_NOTOPMOST)
                                             : 0;
        UINT swp_flag = top_most_changed ? 0 : SWP_NOZORDER;

        vd->DwStyle = new_style;
        vd->DwExStyle = new_ex_style;
        ::SetWindowLong(vd->Hwnd, GWL_STYLE, vd->DwStyle);
        ::SetWindowLong(vd->Hwnd, GWL_EXSTYLE, vd->DwExStyle);
        RECT rect = {(LONG)viewport->Pos.x,
                     (LONG)viewport->Pos.y,
                     (LONG)(viewport->Pos.x + viewport->Size.x),
                     (LONG)(viewport->Pos.y + viewport->Size.y)};
        ::AdjustWindowRectEx(&rect, vd->DwStyle, FALSE, vd->DwExStyle);
        ::SetWindowPos(vd->Hwnd,
                       insert_after,
                       rect.left,
                       rect.top,
                       rect.right - rect.left,
                       rect.bottom - rect.top,
                       swp_flag | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        ::ShowWindow(vd->Hwnd, SW_SHOWNA);
        viewport->PlatformRequestMove = viewport->PlatformRequestResize = true;
    }
}

static AzVec2 Az_AzplWin32_GetWindowPos(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    POINT pos = {0, 0};
    ::ClientToScreen(vd->Hwnd, &pos);
    return AzVec2((float)pos.x, (float)pos.y);
}

static void Az_AzplWin32_UpdateWin32StyleFromWindow(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    vd->DwStyle = ::GetWindowLongW(vd->Hwnd, GWL_STYLE);
    vd->DwExStyle = ::GetWindowLongW(vd->Hwnd, GWL_EXSTYLE);
}

static void Az_AzplWin32_SetWindowPos(AzViewport* viewport, AzVec2 pos)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    RECT rect = {(LONG)pos.x, (LONG)pos.y, (LONG)pos.x, (LONG)pos.y};
    if (viewport->Flags & AzViewportFlags_OwnedByApp)
        Az_AzplWin32_UpdateWin32StyleFromWindow(viewport);
    ::AdjustWindowRectEx(&rect, vd->DwStyle, FALSE, vd->DwExStyle);
    ::SetWindowPos(vd->Hwnd,
                   nullptr,
                   rect.left,
                   rect.top,
                   0,
                   0,
                   SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

static AzVec2 Az_AzplWin32_GetWindowSize(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    RECT rect;
    ::GetClientRect(vd->Hwnd, &rect);
    return AzVec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
}

static void Az_AzplWin32_SetWindowSize(AzViewport* viewport, AzVec2 size)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    RECT rect = {0, 0, (LONG)size.x, (LONG)size.y};
    if (viewport->Flags & AzViewportFlags_OwnedByApp)
        Az_AzplWin32_UpdateWin32StyleFromWindow(viewport);
    ::AdjustWindowRectEx(&rect, vd->DwStyle, FALSE, vd->DwExStyle);
    ::SetWindowPos(vd->Hwnd,
                   nullptr,
                   0,
                   0,
                   rect.right - rect.left,
                   rect.bottom - rect.top,
                   SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
}

static void Az_AzplWin32_SetWindowFocus(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    ::BringWindowToTop(vd->Hwnd);
    ::SetForegroundWindow(vd->Hwnd);
    ::SetFocus(vd->Hwnd);
}

static bool Az_AzplWin32_GetWindowFocus(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    return ::GetForegroundWindow() == vd->Hwnd;
}

static bool Az_AzplWin32_GetWindowMinimized(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    return ::IsIconic(vd->Hwnd) != 0;
}

static void Az_AzplWin32_SetWindowTitle(AzViewport* viewport, const char* title)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    int n = ::MultiByteToWideChar(CP_UTF8, 0, title, -1, nullptr, 0);
    AzVector<wchar_t> title_w;
    title_w.resize(n);
    ::MultiByteToWideChar(CP_UTF8, 0, title, -1, title_w.Data, n);

    ::DefWindowProcW(vd->Hwnd, WM_SETTEXT, 0, (LPARAM)title_w.Data);
}

static void Az_AzplWin32_SetWindowAlpha(AzViewport* viewport, float alpha)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    AZ_ASSERT(alpha >= 0.0f && alpha <= 1.0f);
    if (alpha < 1.0f)
    {
        DWORD ex_style = ::GetWindowLongW(vd->Hwnd, GWL_EXSTYLE)
                         | WS_EX_LAYERED;
        ::SetWindowLongW(vd->Hwnd, GWL_EXSTYLE, ex_style);
        ::SetLayeredWindowAttributes(vd->Hwnd,
                                     0,
                                     (BYTE)(255 * alpha),
                                     LWA_ALPHA);
    } else
    {
        DWORD ex_style = ::GetWindowLongW(vd->Hwnd, GWL_EXSTYLE)
                         & ~WS_EX_LAYERED;
        ::SetWindowLongW(vd->Hwnd, GWL_EXSTYLE, ex_style);
    }
}

static float Az_AzplWin32_GetWindowDpiScale(AzViewport* viewport)
{
    Az_AzplWin32_ViewportData* vd = (Az_AzplWin32_ViewportData*)
                                            viewport->PlatformUserData;
    AZ_ASSERT(vd->Hwnd != 0);
    return Az_AzplWin32_GetDpiScaleForHwnd(vd->Hwnd);
}

static void Az_AzplWin32_OnChangedViewport(AzViewport* viewport)
{
    (void)viewport;
#if 0
    AzStyle default_style;

    default_style.ScaleAllSizes(viewport->DpiScale);
    AzStyle& style = Az::GetStyle();
    style = default_style;
#endif
}

namespace Az
{
    extern AzIO& GetIO(AzContext*);
    extern AzPlatformIO& GetPlatformIO(AzContext*);
} // namespace Az

static LRESULT CALLBACK
Az_AzplWin32_WndProcHandler_PlatformWindow(HWND hWnd,
                                           UINT msg,
                                           WPARAM wParam,
                                           LPARAM lParam)
{
    AzContext* ctx = (AzContext*)::GetPropA(hWnd, "AZ_CONTEXT");
    if (ctx == NULL)
        return DefWindowProc(hWnd, msg, wParam, lParam);

    AzIO& io = Az::GetIO(ctx);
    AzPlatformIO& platform_io = Az::GetPlatformIO(ctx);
    LRESULT result = 0;
    if (Az_AzplWin32_WndProcHandlerEx(hWnd, msg, wParam, lParam, io))
        result = 1;
    else if (AzViewport* viewport =
                     Az_AzplWin32_FindViewportByPlatformHandle(platform_io,
                                                               hWnd))
    {
        switch (msg)
        {
        case WM_CLOSE:
            viewport->PlatformRequestClose = true;
            return 0;
        case WM_MOVE:
            viewport->PlatformRequestMove = true;
            break;
        case WM_SIZE:
            viewport->PlatformRequestResize = true;
            break;
        case WM_MOUSEACTIVATE:
            if (viewport->Flags & AzViewportFlags_NoFocusOnClick)
                result = MA_NOACTIVATE;
            break;
        case WM_NCHITTEST:

            if (viewport->Flags & AzViewportFlags_NoInputs)
                result = HTTRANSPARENT;
            break;
        }
    }
    if (result == 0)
        result = DefWindowProc(hWnd, msg, wParam, lParam);
    return result;
}

static void Az_ImplWin32_InitMultiViewportSupport(bool platform_has_own_dc)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW | (platform_has_own_dc ? CS_OWNDC : 0);
    wcex.lpfnWndProc = Az_AzplWin32_WndProcHandler_PlatformWindow;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = ::GetModuleHandle(nullptr);
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"Az Platform";
    wcex.hIconSm = nullptr;
    ::RegisterClassExW(&wcex);

    Az_ImplWin32_UpdateMonitors();

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Platform_CreateWindow = Az_AzplWin32_CreateWindow;
    platform_io.Platform_DestroyWindow = Az_AzplWin32_DestroyWindow;
    platform_io.Platform_ShowWindow = Az_AzplWin32_ShowWindow;
    platform_io.Platform_SetWindowPos = Az_AzplWin32_SetWindowPos;
    platform_io.Platform_GetWindowPos = Az_AzplWin32_GetWindowPos;
    platform_io.Platform_SetWindowSize = Az_AzplWin32_SetWindowSize;
    platform_io.Platform_GetWindowSize = Az_AzplWin32_GetWindowSize;
    platform_io.Platform_SetWindowFocus = Az_AzplWin32_SetWindowFocus;
    platform_io.Platform_GetWindowFocus = Az_AzplWin32_GetWindowFocus;
    platform_io.Platform_GetWindowMinimized = Az_AzplWin32_GetWindowMinimized;
    platform_io.Platform_SetWindowTitle = Az_AzplWin32_SetWindowTitle;
    platform_io.Platform_SetWindowAlpha = Az_AzplWin32_SetWindowAlpha;
    platform_io.Platform_UpdateWindow = Az_AzplWin32_UpdateWindow;
    platform_io.Platform_GetWindowDpiScale = Az_AzplWin32_GetWindowDpiScale;
    platform_io.Platform_OnChangedViewport = Az_AzplWin32_OnChangedViewport;

    AzViewport* main_viewport = Az::GetMainViewport();
    Az_ImplWin32_Data* bd = Az_ImplWin32_GetBackendData();
    Az_AzplWin32_ViewportData* vd = AZ_NEW(Az_AzplWin32_ViewportData)();
    vd->Hwnd = bd->hWnd;
    vd->HwndOwned = false;
    main_viewport->PlatformUserData = vd;
}

static void Az_ImplWin32_ShutdownMultiViewportSupport()
{
    ::UnregisterClassW(L"Az Platform", ::GetModuleHandle(nullptr));
    Az::DestroyPlatformWindows();
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif
