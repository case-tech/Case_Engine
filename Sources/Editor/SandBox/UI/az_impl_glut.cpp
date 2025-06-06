// dear az: Platform Backend for GLUT/FreeGLUT
// This needs to be used along with a Renderer (e.g. OpenGL2)

// !!! GLUT/FreeGLUT IS OBSOLETE PREHISTORIC SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT today, you are being abused. Please show some resistance. !!!
// !!! Nowadays, prefer using GLFW or SDL instead!

// Implemented features:
//  [X] Platform: Partial keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass AzKey values to all key functions e.g. Az::IsKeyPressed(AzKey_Space). [Legacy GLUT values are obsolete since 1.87 and not supported since 1.91.5]
// Missing features or Issues:
//  [ ] Platform: GLUT is unable to distinguish e.g. Backspace from CTRL+H or TAB from CTRL+I
//  [ ] Platform: Missing horizontal mouse wheel support.
//  [ ] Platform: Missing mouse cursor shape/visibility support.
//  [ ] Platform: Missing clipboard support (not supported by Glut).
//  [ ] Platform: Missing gamepad support.

// You can use unmodified az_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire az/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear Az:
// - FAQ                  https://dearaz.com/faq
// - Getting Started      https://dearaz.com/getting-started
// - Documentation        https://dearaz.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of az.cpp

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2023-04-17: BREAKING: Removed call to Az::NewFrame() from Az_ImplGLUT_NewFrame(). Needs to be called from the main application loop, like with every other backends.
//  2022-09-26: Inputs: Renamed AzKey_ModXXX introduced in 1.87 to AzMod_XXX (old names still supported).
//  2022-01-26: Inputs: replaced short-lived io.AddKeyModsEvent() (added two weeks ago) with io.AddKeyEvent() using AzKey_ModXXX flags. Sorry for the confusion.
//  2022-01-17: Inputs: calling new io.AddMousePosEvent(), io.AddMouseButtonEvent(), io.AddMouseWheelEvent() API (1.87+).
//  2022-01-10: Inputs: calling new io.AddKeyEvent(), io.AddKeyModsEvent() + io.SetKeyEventNativeData() API (1.87+). Support for full AzKey range.
//  2019-04-03: Misc: Renamed az_impl_freeglut.cpp/.h to az_impl_glut.cpp/.h.
//  2019-03-25: Misc: Made io.DeltaTime always above zero.
//  2018-11-30: Misc: Setting up io.BackendPlatformName so it can be displayed in the About Window.
//  2018-03-22: Added GLUT Platform binding.

#include "az.h"
#ifndef AZGUI_DISABLE
#include "az_impl_glut.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#endif

static int g_Time = 0;          // Current time, in milliseconds

// Glut has one function for characters and one for "special keys". We map the characters in the 0..255 range and the keys above.
static AzKey Az_ImplGLUT_KeyToAzKey(int key)
{
    switch (key)
    {
        case '\t':                      return AzKey_Tab;
        case 256 + GLUT_KEY_LEFT:       return AzKey_LeftArrow;
        case 256 + GLUT_KEY_RIGHT:      return AzKey_RightArrow;
        case 256 + GLUT_KEY_UP:         return AzKey_UpArrow;
        case 256 + GLUT_KEY_DOWN:       return AzKey_DownArrow;
        case 256 + GLUT_KEY_PAGE_UP:    return AzKey_PageUp;
        case 256 + GLUT_KEY_PAGE_DOWN:  return AzKey_PageDown;
        case 256 + GLUT_KEY_HOME:       return AzKey_Home;
        case 256 + GLUT_KEY_END:        return AzKey_End;
        case 256 + GLUT_KEY_INSERT:     return AzKey_Insert;
        case 127:                       return AzKey_Delete;
        case 8:                         return AzKey_Backspace;
        case ' ':                       return AzKey_Space;
        case 13:                        return AzKey_Enter;
        case 27:                        return AzKey_Escape;
        case 39:                        return AzKey_Apostrophe;
        case 44:                        return AzKey_Comma;
        case 45:                        return AzKey_Minus;
        case 46:                        return AzKey_Period;
        case 47:                        return AzKey_Slash;
        case 59:                        return AzKey_Semicolon;
        case 61:                        return AzKey_Equal;
        case 91:                        return AzKey_LeftBracket;
        case 92:                        return AzKey_Backslash;
        case 93:                        return AzKey_RightBracket;
        case 96:                        return AzKey_GraveAccent;
        //case 0:                         return AzKey_CapsLock;
        //case 0:                         return AzKey_ScrollLock;
        case 256 + 0x006D:              return AzKey_NumLock;
        //case 0:                         return AzKey_PrintScreen;
        //case 0:                         return AzKey_Pause;
        //case '0':                       return AzKey_Keypad0;
        //case '1':                       return AzKey_Keypad1;
        //case '2':                       return AzKey_Keypad2;
        //case '3':                       return AzKey_Keypad3;
        //case '4':                       return AzKey_Keypad4;
        //case '5':                       return AzKey_Keypad5;
        //case '6':                       return AzKey_Keypad6;
        //case '7':                       return AzKey_Keypad7;
        //case '8':                       return AzKey_Keypad8;
        //case '9':                       return AzKey_Keypad9;
        //case 46:                        return AzKey_KeypadDecimal;
        //case 47:                        return AzKey_KeypadDivide;
        case 42:                        return AzKey_KeypadMultiply;
        //case 45:                        return AzKey_KeypadSubtract;
        case 43:                        return AzKey_KeypadAdd;
        //case 13:                        return AzKey_KeypadEnter;
        //case 0:                         return AzKey_KeypadEqual;
        case 256 + 0x0072:              return AzKey_LeftCtrl;
        case 256 + 0x0070:              return AzKey_LeftShift;
        case 256 + 0x0074:              return AzKey_LeftAlt;
        //case 0:                         return AzKey_LeftSuper;
        case 256 + 0x0073:              return AzKey_RightCtrl;
        case 256 + 0x0071:              return AzKey_RightShift;
        case 256 + 0x0075:              return AzKey_RightAlt;
        //case 0:                         return AzKey_RightSuper;
        //case 0:                         return AzKey_Menu;
        case '0':                       return AzKey_0;
        case '1':                       return AzKey_1;
        case '2':                       return AzKey_2;
        case '3':                       return AzKey_3;
        case '4':                       return AzKey_4;
        case '5':                       return AzKey_5;
        case '6':                       return AzKey_6;
        case '7':                       return AzKey_7;
        case '8':                       return AzKey_8;
        case '9':                       return AzKey_9;
        case 'A': case 'a':             return AzKey_A;
        case 'B': case 'b':             return AzKey_B;
        case 'C': case 'c':             return AzKey_C;
        case 'D': case 'd':             return AzKey_D;
        case 'E': case 'e':             return AzKey_E;
        case 'F': case 'f':             return AzKey_F;
        case 'G': case 'g':             return AzKey_G;
        case 'H': case 'h':             return AzKey_H;
        case 'I': case 'i':             return AzKey_I;
        case 'J': case 'j':             return AzKey_J;
        case 'K': case 'k':             return AzKey_K;
        case 'L': case 'l':             return AzKey_L;
        case 'M': case 'm':             return AzKey_M;
        case 'N': case 'n':             return AzKey_N;
        case 'O': case 'o':             return AzKey_O;
        case 'P': case 'p':             return AzKey_P;
        case 'Q': case 'q':             return AzKey_Q;
        case 'R': case 'r':             return AzKey_R;
        case 'S': case 's':             return AzKey_S;
        case 'T': case 't':             return AzKey_T;
        case 'U': case 'u':             return AzKey_U;
        case 'V': case 'v':             return AzKey_V;
        case 'W': case 'w':             return AzKey_W;
        case 'X': case 'x':             return AzKey_X;
        case 'Y': case 'y':             return AzKey_Y;
        case 'Z': case 'z':             return AzKey_Z;
        case 256 + GLUT_KEY_F1:         return AzKey_F1;
        case 256 + GLUT_KEY_F2:         return AzKey_F2;
        case 256 + GLUT_KEY_F3:         return AzKey_F3;
        case 256 + GLUT_KEY_F4:         return AzKey_F4;
        case 256 + GLUT_KEY_F5:         return AzKey_F5;
        case 256 + GLUT_KEY_F6:         return AzKey_F6;
        case 256 + GLUT_KEY_F7:         return AzKey_F7;
        case 256 + GLUT_KEY_F8:         return AzKey_F8;
        case 256 + GLUT_KEY_F9:         return AzKey_F9;
        case 256 + GLUT_KEY_F10:        return AzKey_F10;
        case 256 + GLUT_KEY_F11:        return AzKey_F11;
        case 256 + GLUT_KEY_F12:        return AzKey_F12;
        default:                        return AzKey_None;
    }
}

bool Az_ImplGLUT_Init()
{
    AzIO& io = Az::GetIO();
    AZGUI_CHECKVERSION();

#ifdef FREEGLUT
    io.BackendPlatformName = "az_impl_glut (freeglut)";
#else
    io.BackendPlatformName = "az_impl_glut";
#endif
    g_Time = 0;

    return true;
}

void Az_ImplGLUT_InstallFuncs()
{
    glutReshapeFunc(Az_ImplGLUT_ReshapeFunc);
    glutMotionFunc(Az_ImplGLUT_MotionFunc);
    glutPassiveMotionFunc(Az_ImplGLUT_MotionFunc);
    glutMouseFunc(Az_ImplGLUT_MouseFunc);
#ifdef __FREEGLUT_EXT_H__
    glutMouseWheelFunc(Az_ImplGLUT_MouseWheelFunc);
#endif
    glutKeyboardFunc(Az_ImplGLUT_KeyboardFunc);
    glutKeyboardUpFunc(Az_ImplGLUT_KeyboardUpFunc);
    glutSpecialFunc(Az_ImplGLUT_SpecialFunc);
    glutSpecialUpFunc(Az_ImplGLUT_SpecialUpFunc);
}

void Az_ImplGLUT_Shutdown()
{
    AzIO& io = Az::GetIO();
    io.BackendPlatformName = nullptr;
}

void Az_ImplGLUT_NewFrame()
{
    // Setup time step
    AzIO& io = Az::GetIO();
    int current_time = glutGet(GLUT_ELAPSED_TAZE);
    int delta_time_ms = (current_time - g_Time);
    if (delta_time_ms <= 0)
        delta_time_ms = 1;
    io.DeltaTime = delta_time_ms / 1000.0f;
    g_Time = current_time;
}

static void Az_ImplGLUT_UpdateKeyModifiers()
{
    AzIO& io = Az::GetIO();
    int glut_key_mods = glutGetModifiers();
    io.AddKeyEvent(AzMod_Ctrl, (glut_key_mods & GLUT_ACTIVE_CTRL) != 0);
    io.AddKeyEvent(AzMod_Shift, (glut_key_mods & GLUT_ACTIVE_SHIFT) != 0);
    io.AddKeyEvent(AzMod_Alt, (glut_key_mods & GLUT_ACTIVE_ALT) != 0);
}

static void Az_ImplGLUT_AddKeyEvent(AzKey key, bool down, int native_keycode)
{
    AzIO& io = Az::GetIO();
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, -1); // To support legacy indexing (<1.87 user code)
}

void Az_ImplGLUT_KeyboardFunc(unsigned char c, int x, int y)
{
    // Send character to az
    //printf("char_down_func %d '%c'\n", c, c);
    AzIO& io = Az::GetIO();
    if (c >= 32)
        io.AddInputCharacter((unsigned int)c);

    AzKey key = Az_ImplGLUT_KeyToAzKey(c);
    Az_ImplGLUT_AddKeyEvent(key, true, c);
    Az_ImplGLUT_UpdateKeyModifiers();
    (void)x; (void)y; // Unused
}

void Az_ImplGLUT_KeyboardUpFunc(unsigned char c, int x, int y)
{
    //printf("char_up_func %d '%c'\n", c, c);
    AzKey key = Az_ImplGLUT_KeyToAzKey(c);
    Az_ImplGLUT_AddKeyEvent(key, false, c);
    Az_ImplGLUT_UpdateKeyModifiers();
    (void)x; (void)y; // Unused
}

void Az_ImplGLUT_SpecialFunc(int key, int x, int y)
{
    //printf("key_down_func %d\n", key);
    AzKey az_key = Az_ImplGLUT_KeyToAzKey(key + 256);
    Az_ImplGLUT_AddKeyEvent(az_key, true, key + 256);
    Az_ImplGLUT_UpdateKeyModifiers();
    (void)x; (void)y; // Unused
}

void Az_ImplGLUT_SpecialUpFunc(int key, int x, int y)
{
    //printf("key_up_func %d\n", key);
    AzKey az_key = Az_ImplGLUT_KeyToAzKey(key + 256);
    Az_ImplGLUT_AddKeyEvent(az_key, false, key + 256);
    Az_ImplGLUT_UpdateKeyModifiers();
    (void)x; (void)y; // Unused
}

void Az_ImplGLUT_MouseFunc(int glut_button, int state, int x, int y)
{
    AzIO& io = Az::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
    int button = -1;
    if (glut_button == GLUT_LEFT_BUTTON) button = 0;
    if (glut_button == GLUT_RIGHT_BUTTON) button = 1;
    if (glut_button == GLUT_MIDDLE_BUTTON) button = 2;
    if (button != -1 && (state == GLUT_DOWN || state == GLUT_UP))
        io.AddMouseButtonEvent(button, state == GLUT_DOWN);
}

#ifdef __FREEGLUT_EXT_H__
void Az_ImplGLUT_MouseWheelFunc(int button, int dir, int x, int y)
{
    AzIO& io = Az::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
    if (dir != 0)
        io.AddMouseWheelEvent(0.0f, dir > 0 ? 1.0f : -1.0f);
    (void)button; // Unused
}
#endif

void Az_ImplGLUT_ReshapeFunc(int w, int h)
{
    AzIO& io = Az::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);
}

void Az_ImplGLUT_MotionFunc(int x, int y)
{
    AzIO& io = Az::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
}

//-----------------------------------------------------------------------------

#endif // #ifndef AZGUI_DISABLE
