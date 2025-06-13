#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef AZ_DEFINE_MATH_OPERATORS
#define AZ_DEFINE_MATH_OPERATORS
#endif

#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include <Az/AzInternal.hpp>
#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32) && !defined(_MSC_VER)                                      \
        && !defined(AZ_ENABLE_WIN32_DEFAULT_AZE_FUNCTIONS)                     \
        && !defined(AZ_DISABLE_WIN32_DEFAULT_AZE_FUNCTIONS)
#define AZ_DISABLE_WIN32_DEFAULT_AZE_FUNCTIONS
#endif

#if defined(_WIN32) && defined(AZ_DISABLE_DEFAULT_FILE_FUNCTIONS)              \
        && defined(AZ_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS)               \
        && defined(AZ_DISABLE_WIN32_DEFAULT_AZE_FUNCTIONS)                     \
        && defined(AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS)                         \
        && !defined(AZ_DISABLE_WIN32_FUNCTIONS)
#define AZ_DISABLE_WIN32_FUNCTIONS
#endif
#if defined(_WIN32) && !defined(AZ_DISABLE_WIN32_FUNCTIONS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef __MINGW32__
#include <Windows.h>
#else
#include <windows.h>
#endif
#if defined(WINAPI_FAMILY)                                                     \
        && ((defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP) \
            || (defined(WINAPI_FAMILY_GAMES)                                   \
                && WINAPI_FAMILY == WINAPI_FAMILY_GAMES))

#define AZ_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
#define AZ_DISABLE_WIN32_DEFAULT_AZE_FUNCTIONS
#define AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS
#endif
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#pragma warning(disable : 4996)

#if defined(_MSC_VER) && _MSC_VER >= 1922
#pragma warning(disable : 5054)

#endif
#pragma warning(disable : 26451)

#pragma warning(disable : 26495)

#pragma warning(disable : 26812)

#endif

#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"

#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wold-style-cast"

#pragma clang diagnostic ignored "-Wfloat-equal"

#pragma clang diagnostic ignored "-Wformat"

#pragma clang diagnostic ignored "-Wformat-nonliteral"

#pragma clang diagnostic ignored "-Wformat-pedantic"

#pragma clang diagnostic ignored "-Wexit-time-destructors"

#pragma clang diagnostic ignored "-Wglobal-constructors"

#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"

#pragma clang diagnostic ignored "-Wdouble-promotion"

#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"

#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"

#pragma clang diagnostic ignored "-Wnontrivial-memaccess"

#pragma clang diagnostic ignored "-Wswitch-default"
#elif defined(__GNUC__)

#pragma GCC diagnostic ignored "-Wpragmas"

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#pragma GCC diagnostic ignored "-Wfloat-equal"

#pragma GCC diagnostic ignored "-Wformat"

#pragma GCC diagnostic ignored "-Wdouble-promotion"

#pragma GCC diagnostic ignored "-Wconversion"

#pragma GCC diagnostic ignored "-Wformat-nonliteral"

#pragma GCC diagnostic ignored "-Wstrict-overflow"

#pragma GCC diagnostic ignored "-Wclass-memaccess"

#pragma GCC diagnostic ignored "-Wcast-qual"

#endif

#define AZ_DEBUG_NAV_SCORING 0

#define AZ_DEBUG_NAV_RECTS 0

static const float FONT_DEFAULT_SIZE = 20.0f;

static const float NAV_WINDOWING_HIGHLIGHT_DELAY = 0.20f;
static const float NAV_WINDOWING_LIST_APPEAR_DELAY = 0.15f;
static const float NAV_ACTIVATE_HIGHLIGHT_TAZER = 0.10f;
static const float WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TAZER = 0.04f;

static const float WINDOWS_MOUSE_WHEEL_SCROLL_LOCK_TAZER = 0.70f;

static const AzVec2 TOOLTIP_DEFAULT_OFFSET_MOUSE = AzVec2(16, 10);
static const AzVec2 TOOLTIP_DEFAULT_OFFSET_TOUCH = AzVec2(0, -20);
static const AzVec2 TOOLTIP_DEFAULT_PIVOT_TOUCH = AzVec2(0.5f, 1.0f);

static const float DOCKING_TRANSPARENT_PAYLOAD_ALPHA = 0.50f;

static void SetCurrentWindow(AzWindow* window);
static AzWindow* CreateNewWindow(const char* name, AzWindowFlags flags);
static AzVec2 CalcNextScrollFromScrollTargetAndClamp(AzWindow* window);

static void AddWindowToSortBuffer(AzVector<AzWindow*>* out_sorted_windows,
                                  AzWindow* window);

static void WindowSettingsHandler_ClearAll(AzContext*, AzSettingsHandler*);
static void* WindowSettingsHandler_ReadOpen(AzContext*,
                                            AzSettingsHandler*,
                                            const char* name);
static void WindowSettingsHandler_ReadLine(AzContext*,
                                           AzSettingsHandler*,
                                           void* entry,
                                           const char* line);
static void WindowSettingsHandler_ApplyAll(AzContext*, AzSettingsHandler*);
static void WindowSettingsHandler_WriteAll(AzContext*,
                                           AzSettingsHandler*,
                                           AzTextBuffer* buf);

static const char* Platform_GetClipboardTextFn_DefaultAzpl(AzContext* ctx);
static void Platform_SetClipboardTextFn_DefaultAzpl(AzContext* ctx,
                                                    const char* text);
static void Platform_SetAzeDataFn_DefaultAzpl(AzContext* ctx,
                                              AzViewport* viewport,
                                              AzPlatformAzeData* data);
static bool Platform_OpenInShellFn_DefaultAzpl(AzContext* ctx,
                                               const char* path);

namespace Az
{

    static void ItemHandleShortcut(AzID id);

    static int FindWindowFocusIndex(AzWindow* window);
    static void UpdateWindowInFocusOrderList(AzWindow* window,
                                             bool just_created,
                                             AzWindowFlags new_flags);

    static void NavUpdate();
    static void NavUpdateWindowing();
    static void NavUpdateWindowingApplyFocus(AzWindow* window);
    static void NavUpdateWindowingOverlay();
    static void NavUpdateCancelRequest();
    static void NavUpdateCreateMoveRequest();
    static void NavUpdateCreateTabbingRequest();
    static float NavUpdatePageUpPageDown();
    static inline void NavUpdateAnyRequestFlag();
    static void NavUpdateCreateWrappingRequest();
    static void NavEndFrame();
    static bool NavScoreItem(AzNavItemData* result);
    static void NavApplyItemToResult(AzNavItemData* result);
    static void NavProcessItem();
    static void NavProcessItemForTabbingRequest(AzID id,
                                                AzItemFlags item_flags,
                                                AzNavMoveFlags move_flags);
    static AzInputSource NavCalcPreferredRefPosSource();
    static AzVec2 NavCalcPreferredRefPos();
    static void NavSaveLastChildNavWindowIntoParent(AzWindow* nav_window);
    static AzWindow* NavRestoreLastChildNavWindow(AzWindow* window);
    static void NavRestoreLayer(AzNavLayer layer);

    static void ErrorCheckNewFrameSanityChecks();
    static void ErrorCheckEndFrameSanityChecks();
#ifndef AZ_DISABLE_DEBUG_TOOLS
    static void UpdateDebugToolItemPicker();
    static void UpdateDebugToolStackQueries();
    static void UpdateDebugToolFlashStyleColor();
#endif

    static void UpdateKeyboardInputs();
    static void UpdateMouseInputs();
    static void UpdateMouseWheel();
    static void UpdateKeyRoutingTable(AzKeyRoutingTable* rt);

    static void UpdateFontsNewFrame();
    static void UpdateFontsEndFrame();
    static void UpdateTexturesNewFrame();
    static void UpdateTexturesEndFrame();
    static void UpdateSettings();
    static int UpdateWindowManualResize(AzWindow* window,
                                        const AzVec2& size_auto_fit,
                                        int* border_hovered,
                                        int* border_held,
                                        int resize_grip_count,
                                        AzU32 resize_grip_col[4],
                                        const AzRect& visibility_rect);
    static void RenderWindowOuterBorders(AzWindow* window);
    static void RenderWindowDecorations(AzWindow* window,
                                        const AzRect& title_bar_rect,
                                        bool title_bar_is_highlight,
                                        bool handle_borders_and_resize_grips,
                                        int resize_grip_count,
                                        const AzU32 resize_grip_col[4],
                                        float resize_grip_draw_size);
    static void RenderWindowTitleBarContents(AzWindow* window,
                                             const AzRect& title_bar_rect,
                                             const char* name,
                                             bool* p_open);
    static void RenderDimmedBackgroundBehindWindow(AzWindow* window, AzU32 col);
    static void RenderDimmedBackgrounds();
    static void SetLastItemDataForWindow(AzWindow* window, const AzRect& rect);
    static void SetLastItemDataForChildWindowItem(AzWindow* window,
                                                  const AzRect& rect);

    const AzID AZ_VIEWPORT_DEFAULT_ID = 0x11111111;

    static AzViewportP* AddUpdateViewport(AzWindow* window,
                                          AzID id,
                                          const AzVec2& platform_pos,
                                          const AzVec2& size,
                                          AzViewportFlags flags);
    static void DestroyViewport(AzViewportP* viewport);
    static void UpdateViewportsNewFrame();
    static void UpdateViewportsEndFrame();
    static void WindowSelectViewport(AzWindow* window);
    static void WindowSyncOwnedViewport(AzWindow* window,
                                        AzWindow* parent_window_in_stack);
    static bool UpdateTryMergeWindowIntoHostViewport(
            AzWindow* window,
            AzViewportP* host_viewport);
    static bool UpdateTryMergeWindowIntoHostViewports(AzWindow* window);
    static bool GetWindowAlwaysWantOwnViewport(AzWindow* window);
    static int FindPlatformMonitorForPos(const AzVec2& pos);
    static int FindPlatformMonitorForRect(const AzRect& r);
    static void UpdateViewportPlatformMonitor(AzViewportP* viewport);

} // namespace Az

#ifndef GAz
AzContext* GAz = NULL;
#endif

#ifndef AZ_DISABLE_DEFAULT_ALLOCATORS
static void* MallocWrapper(size_t size, void* user_data)
{
    AZ_UNUSED(user_data);
    return malloc(size);
}
static void FreeWrapper(void* ptr, void* user_data)
{
    AZ_UNUSED(user_data);
    free(ptr);
}
#else
static void* MallocWrapper(size_t size, void* user_data)
{
    AZ_UNUSED(user_data);
    AZ_UNUSED(size);
    AZ_ASSERT(0);
    return NULL;
}
static void FreeWrapper(void* ptr, void* user_data)
{
    AZ_UNUSED(user_data);
    AZ_UNUSED(ptr);
    AZ_ASSERT(0);
}
#endif
static AzMemAllocFunc GAzAllocatorAllocFunc = MallocWrapper;
static AzMemFreeFunc GAzAllocatorFreeFunc = FreeWrapper;
static void* GAzAllocatorUserData = NULL;

AzStyle::AzStyle()
{
    FontSizeBase = 0.0f;
    FontScaleMain = 1.0f;

    FontScaleDpi = 1.0f;

    Alpha = 1.0f;
    DisabledAlpha = 0.60f;

    WindowPadding = AzVec2(8, 8);
    WindowRounding = 6.0f;

    WindowBorderSize = 0.0f;

    WindowBorderHoverPadding = 4.0f;

    WindowMinSize = AzVec2(32, 32);
    WindowTitleAlign = AzVec2(0.5f, 0.5f);
    WindowMenuButtonPosition = AzDir_Left;

    ChildRounding = 4.0f;

    ChildBorderSize = 0.0f;

    PopupRounding = 4.0f;

    PopupBorderSize = 0.0f;

    FramePadding = AzVec2(6, 4);

    FrameRounding = 4.0f;

    FrameBorderSize = 0.0f;

    ItemSpacing = AzVec2(6, 4);

    ItemInnerSpacing = AzVec2(4, 4);

    CellPadding = AzVec2(4, 2);

    TouchExtraPadding = AzVec2(0, 0);

    IndentSpacing = 21.0f;

    ColumnsMinSpacing = 6.0f;

    ScrollbarSize = 12.0f;

    ScrollbarRounding = 9.0f;
    GrabMinSize = 8.0f;

    GrabRounding = 4.0f;

    LogSliderDeadzone = 4.0f;

    AzageBorderSize = 0.0f;
    TabRounding = 4.0f;

    TabBorderSize = 0.0f;
    TabCloseButtonMinWidthSelected = -1.0f;

    TabCloseButtonMinWidthUnselected = 0.0f;

    TabBarBorderSize = 1.0f;

    TabBarOverlineSize = 1.0f;

    TableAngledHeadersAngle = 35.0f * (AZ_PI / 180.0f);

    TableAngledHeadersTextAlign = AzVec2(0.5f, 0.0f);

    TreeLinesFlags = AzTreeNodeFlags_DrawLinesNone;
    TreeLinesSize = 1.0f;

    TreeLinesRounding = 0.0f;

    ColorButtonPosition = AzDir_Right;

    ButtonTextAlign = AzVec2(0.5f, 0.5f);

    SelectableTextAlign = AzVec2(0.0f, 0.0f);

    SeparatorTextBorderSize = 3.0f;
    SeparatorTextAlign = AzVec2(0.0f, 0.5f);

    SeparatorTextPadding = AzVec2(20.0f, 3.f);

    DisplayWindowPadding = AzVec2(19, 19);

    DisplaySafeAreaPadding = AzVec2(3, 3);

    DockingSeparatorSize = 2.0f;

    MouseCursorScale = 1.0f;

    AntiAliasedLines = true;

    AntiAliasedLinesUseTex = true;

    AntiAliasedFill = true;

    CurveTessellationTol = 1.25f;

    CircleTessellationMaxError = 0.30f;

    HoverStationaryDelay = 0.15f;

    HoverDelayShort = 0.15f;

    HoverDelayNormal = 0.40f;

    HoverFlagsForTooltipMouse = AzHoveredFlags_Stationary
                                | AzHoveredFlags_DelayShort
                                | AzHoveredFlags_AllowWhenDisabled;

    HoverFlagsForTooltipNav = AzHoveredFlags_NoSharedDelay
                              | AzHoveredFlags_DelayNormal
                              | AzHoveredFlags_AllowWhenDisabled;

    _MainScale = 1.0f;
    _NextFrameFontSizeBase = 0.0f;

    Az::StyleColorsDark(this);
}

void AzStyle::ScaleAllSizes(float scale_factor)
{
    _MainScale *= scale_factor;
    WindowPadding = AzTrunc(WindowPadding * scale_factor);
    WindowRounding = AzTrunc(WindowRounding * scale_factor);
    WindowMinSize = AzTrunc(WindowMinSize * scale_factor);
    WindowBorderHoverPadding = AzTrunc(WindowBorderHoverPadding * scale_factor);
    ChildRounding = AzTrunc(ChildRounding * scale_factor);
    PopupRounding = AzTrunc(PopupRounding * scale_factor);
    FramePadding = AzTrunc(FramePadding * scale_factor);
    FrameRounding = AzTrunc(FrameRounding * scale_factor);
    ItemSpacing = AzTrunc(ItemSpacing * scale_factor);
    ItemInnerSpacing = AzTrunc(ItemInnerSpacing * scale_factor);
    CellPadding = AzTrunc(CellPadding * scale_factor);
    TouchExtraPadding = AzTrunc(TouchExtraPadding * scale_factor);
    IndentSpacing = AzTrunc(IndentSpacing * scale_factor);
    ColumnsMinSpacing = AzTrunc(ColumnsMinSpacing * scale_factor);
    ScrollbarSize = AzTrunc(ScrollbarSize * scale_factor);
    ScrollbarRounding = AzTrunc(ScrollbarRounding * scale_factor);
    GrabMinSize = AzTrunc(GrabMinSize * scale_factor);
    GrabRounding = AzTrunc(GrabRounding * scale_factor);
    LogSliderDeadzone = AzTrunc(LogSliderDeadzone * scale_factor);
    AzageBorderSize = AzTrunc(AzageBorderSize * scale_factor);
    TabRounding = AzTrunc(TabRounding * scale_factor);
    TabCloseButtonMinWidthSelected =
            (TabCloseButtonMinWidthSelected > 0.0f
             && TabCloseButtonMinWidthSelected != FLT_MAX)
                    ? AzTrunc(TabCloseButtonMinWidthSelected * scale_factor)
                    : TabCloseButtonMinWidthSelected;
    TabCloseButtonMinWidthUnselected =
            (TabCloseButtonMinWidthUnselected > 0.0f
             && TabCloseButtonMinWidthUnselected != FLT_MAX)
                    ? AzTrunc(TabCloseButtonMinWidthUnselected * scale_factor)
                    : TabCloseButtonMinWidthUnselected;
    TabBarOverlineSize = AzTrunc(TabBarOverlineSize * scale_factor);
    TreeLinesRounding = AzTrunc(TreeLinesRounding * scale_factor);
    SeparatorTextPadding = AzTrunc(SeparatorTextPadding * scale_factor);
    DockingSeparatorSize = AzTrunc(DockingSeparatorSize * scale_factor);
    DisplayWindowPadding = AzTrunc(DisplayWindowPadding * scale_factor);
    DisplaySafeAreaPadding = AzTrunc(DisplaySafeAreaPadding * scale_factor);
    MouseCursorScale = AzTrunc(MouseCursorScale * scale_factor);
}

AzIO::AzIO()
{
    memset(this, 0, sizeof(*this));
    AZ_STATIC_ASSERT(AZ_ARRAYSIZE(AzIO::MouseDown) == AzMouseButton_COUNT
                     && AZ_ARRAYSIZE(AzIO::MouseClicked)
                                == AzMouseButton_COUNT);

    ConfigFlags = AzConfigFlags_None;
    BackendFlags = AzBackendFlags_None;
    DisplaySize = AzVec2(-1.0f, -1.0f);
    DeltaTime = 1.0f / 60.0f;
    IniSavingRate = 5.0f;
    IniFilename = "az.ini";

    LogFilename = "az_log.txt";
    UserData = NULL;

    Fonts = NULL;
    FontDefault = NULL;
    FontAllowUserScaling = false;
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    FontGlobalScale = 1.0f;
#endif
    DisplayFramebufferScale = AzVec2(1.0f, 1.0f);

    ConfigNavSwapGamepadButtons = false;
    ConfigNavMoveSetMousePos = false;
    ConfigNavCaptureKeyboard = true;
    ConfigNavEscapeClearFocusItem = true;
    ConfigNavEscapeClearFocusWindow = false;
    ConfigNavCursorVisibleAuto = true;
    ConfigNavCursorVisibleAlways = false;

    ConfigDockingNoSplit = false;
    ConfigDockingWithShift = false;
    ConfigDockingAlwaysTabBar = false;
    ConfigDockingTransparentPayload = false;

    ConfigViewportsNoAutoMerge = false;
    ConfigViewportsNoTaskBarIcon = false;
    ConfigViewportsNoDecoration = true;
    ConfigViewportsNoDefaultParent = false;

    MouseDrawCursor = false;
#ifdef __APPLE__
    ConfigMacOSXBehaviors = true;

#else
    ConfigMacOSXBehaviors = false;
#endif
    ConfigInputTrickleEventQueue = true;
    ConfigInputTextCursorBlink = true;
    ConfigInputTextEnterKeepActive = false;
    ConfigDragClickToInputText = false;
    ConfigWindowsResizeFromEdges = true;
    ConfigWindowsMoveFromTitleBarOnly = false;
    ConfigWindowsCopyContentsWithCtrlC = false;
    ConfigScrollbarScrollByPage = true;
    ConfigMemoryCompactTimer = 60.0f;
    ConfigDebugIsDebuggerPresent = false;
    ConfigDebugHighlightIdConflicts = true;
    ConfigDebugHighlightIdConflictsShowItemPicker = true;
    ConfigDebugBeginReturnValueOnce = false;
    ConfigDebugBeginReturnValueLoop = false;

    ConfigErrorRecovery = true;
    ConfigErrorRecoveryEnableAssert = true;
    ConfigErrorRecoveryEnableDebugLog = true;
    ConfigErrorRecoveryEnableTooltip = true;

    MouseDoubleClickTime = 0.30f;
    MouseDoubleClickMaxDist = 6.0f;
    MouseDragThreshold = 6.0f;
    KeyRepeatDelay = 0.275f;
    KeyRepeatRate = 0.050f;

    BackendPlatformName = BackendRendererName = NULL;
    BackendPlatformUserData = BackendRendererUserData =
            BackendLanguageUserData = NULL;

    MousePos = AzVec2(-FLT_MAX, -FLT_MAX);
    MousePosPrev = AzVec2(-FLT_MAX, -FLT_MAX);
    MouseSource = AzMouseSource_Mouse;
    for (int i = 0; i < AZ_ARRAYSIZE(MouseDownDuration); i++)
        MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.0f;
    for (int i = 0; i < AZ_ARRAYSIZE(KeysData); i++)
    {
        KeysData[i].DownDuration = KeysData[i].DownDurationPrev = -1.0f;
    }
    AppAcceptingEvents = true;
}

void AzIO::AddInputCharacter(unsigned int c)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;
    if (c == 0 || !AppAcceptingEvents)
        return;

    AzInputEvent e;
    e.Type = AzInputEventType_Text;
    e.Source = AzInputSource_Keyboard;
    e.EventId = g.InputEventsNextEventId++;
    e.Text.Char = c;
    g.InputEventsQueue.push_back(e);
}

void AzIO::AddInputCharacterUTF16(AzWchar16 c)
{
    if ((c == 0 && InputQueueSurrogate == 0) || !AppAcceptingEvents)
        return;

    if ((c & 0xFC00) == 0xD800)
    {
        if (InputQueueSurrogate != 0)
            AddInputCharacter(AZ_UNICODE_CODEPOINT_INVALID);
        InputQueueSurrogate = c;
        return;
    }

    AzWchar cp = c;
    if (InputQueueSurrogate != 0)
    {
        if ((c & 0xFC00) != 0xDC00)
        {
            AddInputCharacter(AZ_UNICODE_CODEPOINT_INVALID);
        } else
        {
#if AZ_UNICODE_CODEPOINT_MAX == 0xFFFF
            cp = AZ_UNICODE_CODEPOINT_INVALID;

#else
            cp = (AzWchar)(((InputQueueSurrogate - 0xD800) << 10) + (c - 0xDC00)
                           + 0x10000);
#endif
        }

        InputQueueSurrogate = 0;
    }
    AddInputCharacter((unsigned)cp);
}

void AzIO::AddInputCharactersUTF8(const char* utf8_chars)
{
    if (!AppAcceptingEvents)
        return;
    while (*utf8_chars != 0)
    {
        unsigned int c = 0;
        utf8_chars += AzTextCharFromUtf8(&c, utf8_chars, NULL);
        AddInputCharacter(c);
    }
}

void AzIO::ClearEventsQueue()
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;
    g.InputEventsQueue.clear();
}

void AzIO::ClearInputKeys()
{
    AzContext& g = *Ctx;
    for (int key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END; key++)
    {
        if (Az::IsMouseKey((AzKey)key))
            continue;
        AzKeyData* key_data = &g.IO.KeysData[key - AzKey_NamedKey_BEGIN];
        key_data->Down = false;
        key_data->DownDuration = -1.0f;
        key_data->DownDurationPrev = -1.0f;
    }
    KeyCtrl = KeyShift = KeyAlt = KeySuper = false;
    KeyMods = AzMod_None;
    InputQueueCharacters.resize(0);
}

void AzIO::ClearInputMouse()
{
    for (AzKey key = AzKey_Mouse_BEGIN; key < AzKey_Mouse_END;
         key = (AzKey)(key + 1))
    {
        AzKeyData* key_data = &KeysData[key - AzKey_NamedKey_BEGIN];
        key_data->Down = false;
        key_data->DownDuration = -1.0f;
        key_data->DownDurationPrev = -1.0f;
    }
    MousePos = AzVec2(-FLT_MAX, -FLT_MAX);
    for (int n = 0; n < AZ_ARRAYSIZE(MouseDown); n++)
    {
        MouseDown[n] = false;
        MouseDownDuration[n] = MouseDownDurationPrev[n] = -1.0f;
    }
    MouseWheel = MouseWheelH = 0.0f;
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
void AzIO::ClearInputCharacters()
{
    InputQueueCharacters.resize(0);
}
#endif

static AzInputEvent* FindLatestInputEvent(AzContext* ctx,
                                          AzInputEventType type,
                                          int arg = -1)
{
    AzContext& g = *ctx;
    for (int n = g.InputEventsQueue.Size - 1; n >= 0; n--)
    {
        AzInputEvent* e = &g.InputEventsQueue[n];
        if (e->Type != type)
            continue;
        if (type == AzInputEventType_Key && e->Key.Key != arg)
            continue;
        if (type == AzInputEventType_MouseButton
            && e->MouseButton.Button != arg)
            continue;
        return e;
    }
    return NULL;
}

void AzIO::AddKeyAnalogEvent(AzKey key, bool down, float analog_value)
{
    AZ_ASSERT(Ctx != NULL);
    if (key == AzKey_None || !AppAcceptingEvents)
        return;
    AzContext& g = *Ctx;
    AZ_ASSERT(Az::IsNamedKeyOrMod(key));

    AZ_ASSERT(Az::IsAliasKey(key) == false);

    if (g.IO.ConfigMacOSXBehaviors)
    {
        if (key == AzMod_Super)
        {
            key = AzMod_Ctrl;
        } else if (key == AzMod_Ctrl)
        {
            key = AzMod_Super;
        } else if (key == AzKey_LeftSuper)
        {
            key = AzKey_LeftCtrl;
        } else if (key == AzKey_RightSuper)
        {
            key = AzKey_RightCtrl;
        } else if (key == AzKey_LeftCtrl)
        {
            key = AzKey_LeftSuper;
        } else if (key == AzKey_RightCtrl)
        {
            key = AzKey_RightSuper;
        }
    }

    const AzInputEvent* latest_event =
            FindLatestInputEvent(&g, AzInputEventType_Key, (int)key);
    const AzKeyData* key_data = Az::GetKeyData(&g, key);
    const bool latest_key_down = latest_event ? latest_event->Key.Down
                                              : key_data->Down;
    const float latest_key_analog = latest_event ? latest_event->Key.AnalogValue
                                                 : key_data->AnalogValue;
    if (latest_key_down == down && latest_key_analog == analog_value)
        return;

    AzInputEvent e;
    e.Type = AzInputEventType_Key;
    e.Source = Az::IsGamepadKey(key) ? AzInputSource_Gamepad
                                     : AzInputSource_Keyboard;
    e.EventId = g.InputEventsNextEventId++;
    e.Key.Key = key;
    e.Key.Down = down;
    e.Key.AnalogValue = analog_value;
    g.InputEventsQueue.push_back(e);
}

void AzIO::AddKeyEvent(AzKey key, bool down)
{
    if (!AppAcceptingEvents)
        return;
    AddKeyAnalogEvent(key, down, down ? 1.0f : 0.0f);
}

void AzIO::SetKeyEventNativeData(AzKey key,
                                 int native_keycode,
                                 int native_scancode,
                                 int native_legacy_index)
{
    if (key == AzKey_None)
        return;
    AZ_ASSERT(Az::IsNamedKey(key));
    AZ_ASSERT(native_legacy_index == -1
              || Az::IsLegacyKey((AzKey)native_legacy_index));
    AZ_UNUSED(key);
    AZ_UNUSED(native_keycode);
    AZ_UNUSED(native_scancode);
    AZ_UNUSED(native_legacy_index);
}

void AzIO::SetAppAcceptingEvents(bool accepting_events)
{
    AppAcceptingEvents = accepting_events;
}

void AzIO::AddMousePosEvent(float x, float y)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;
    if (!AppAcceptingEvents)
        return;

    AzVec2 pos((x > -FLT_MAX) ? AzFloor(x) : x,
               (y > -FLT_MAX) ? AzFloor(y) : y);

    const AzInputEvent* latest_event =
            FindLatestInputEvent(&g, AzInputEventType_MousePos);
    const AzVec2 latest_pos = latest_event ? AzVec2(latest_event->MousePos.PosX,
                                                    latest_event->MousePos.PosY)
                                           : g.IO.MousePos;
    if (latest_pos.x == pos.x && latest_pos.y == pos.y)
        return;

    AzInputEvent e;
    e.Type = AzInputEventType_MousePos;
    e.Source = AzInputSource_Mouse;
    e.EventId = g.InputEventsNextEventId++;
    e.MousePos.PosX = pos.x;
    e.MousePos.PosY = pos.y;
    e.MousePos.MouseSource = g.InputEventsNextMouseSource;
    g.InputEventsQueue.push_back(e);
}

void AzIO::AddMouseButtonEvent(int mouse_button, bool down)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;
    AZ_ASSERT(mouse_button >= 0 && mouse_button < AzMouseButton_COUNT);
    if (!AppAcceptingEvents)
        return;

    if (ConfigMacOSXBehaviors && mouse_button == 0 && MouseCtrlLeftAsRightClick)
    {
        mouse_button = 1;
        if (!down)
            MouseCtrlLeftAsRightClick = false;
    }

    const AzInputEvent* latest_event =
            FindLatestInputEvent(&g,
                                 AzInputEventType_MouseButton,
                                 (int)mouse_button);
    const bool latest_button_down = latest_event
                                            ? latest_event->MouseButton.Down
                                            : g.IO.MouseDown[mouse_button];
    if (latest_button_down == down)
        return;

    if (ConfigMacOSXBehaviors && mouse_button == 0 && down)
    {
        const AzInputEvent* latest_super_event =
                FindLatestInputEvent(&g,
                                     AzInputEventType_Key,
                                     (int)AzMod_Super);
        if (latest_super_event ? latest_super_event->Key.Down : g.IO.KeySuper)
        {
            AZ_DEBUG_LOG_IO("[io] Super+Left Click aliased into Right Click\n");
            MouseCtrlLeftAsRightClick = true;
            AddMouseButtonEvent(1, true);

            return;
        }
    }

    AzInputEvent e;
    e.Type = AzInputEventType_MouseButton;
    e.Source = AzInputSource_Mouse;
    e.EventId = g.InputEventsNextEventId++;
    e.MouseButton.Button = mouse_button;
    e.MouseButton.Down = down;
    e.MouseButton.MouseSource = g.InputEventsNextMouseSource;
    g.InputEventsQueue.push_back(e);
}

void AzIO::AddMouseWheelEvent(float wheel_x, float wheel_y)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;

    if (!AppAcceptingEvents || (wheel_x == 0.0f && wheel_y == 0.0f))
        return;

    AzInputEvent e;
    e.Type = AzInputEventType_MouseWheel;
    e.Source = AzInputSource_Mouse;
    e.EventId = g.InputEventsNextEventId++;
    e.MouseWheel.WheelX = wheel_x;
    e.MouseWheel.WheelY = wheel_y;
    e.MouseWheel.MouseSource = g.InputEventsNextMouseSource;
    g.InputEventsQueue.push_back(e);
}

void AzIO::AddMouseSourceEvent(AzMouseSource source)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;
    g.InputEventsNextMouseSource = source;
}

void AzIO::AddMouseViewportEvent(AzID viewport_id)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;

    if (!AppAcceptingEvents)
        return;

    const AzInputEvent* latest_event =
            FindLatestInputEvent(&g, AzInputEventType_MouseViewport);
    const AzID latest_viewport_id =
            latest_event ? latest_event->MouseViewport.HoveredViewportID
                         : g.IO.MouseHoveredViewport;
    if (latest_viewport_id == viewport_id)
        return;

    AzInputEvent e;
    e.Type = AzInputEventType_MouseViewport;
    e.Source = AzInputSource_Mouse;
    e.MouseViewport.HoveredViewportID = viewport_id;
    g.InputEventsQueue.push_back(e);
}

void AzIO::AddFocusEvent(bool focused)
{
    AZ_ASSERT(Ctx != NULL);
    AzContext& g = *Ctx;

    const AzInputEvent* latest_event =
            FindLatestInputEvent(&g, AzInputEventType_Focus);
    const bool latest_focused = latest_event ? latest_event->AppFocused.Focused
                                             : !g.IO.AppFocusLost;
    if (latest_focused == focused || (ConfigDebugIgnoreFocusLoss && !focused))
        return;

    AzInputEvent e;
    e.Type = AzInputEventType_Focus;
    e.EventId = g.InputEventsNextEventId++;
    e.AppFocused.Focused = focused;
    g.InputEventsQueue.push_back(e);
}

AzPlatformIO::AzPlatformIO()
{
    memset(this, 0, sizeof(*this));
    Platform_LocaleDecimalPoint = '.';
}

AzVec2 AzBezierCubicClosestPoint(const AzVec2& p1,
                                 const AzVec2& p2,
                                 const AzVec2& p3,
                                 const AzVec2& p4,
                                 const AzVec2& p,
                                 int num_segments)
{
    AZ_ASSERT(num_segments > 0);
    AzVec2 p_last = p1;
    AzVec2 p_closest;
    float p_closest_dist2 = FLT_MAX;
    float t_step = 1.0f / (float)num_segments;
    for (int i_step = 1; i_step <= num_segments; i_step++)
    {
        AzVec2 p_current = AzBezierCubicCalc(p1, p2, p3, p4, t_step * i_step);
        AzVec2 p_line = AzLineClosestPoint(p_last, p_current, p);
        float dist2 = AzLengthSqr(p - p_line);
        if (dist2 < p_closest_dist2)
        {
            p_closest = p_line;
            p_closest_dist2 = dist2;
        }
        p_last = p_current;
    }
    return p_closest;
}

static void AzBezierCubicClosestPointCasteljauStep(const AzVec2& p,
                                                   AzVec2& p_closest,
                                                   AzVec2& p_last,
                                                   float& p_closest_dist2,
                                                   float x1,
                                                   float y1,
                                                   float x2,
                                                   float y2,
                                                   float x3,
                                                   float y3,
                                                   float x4,
                                                   float y4,
                                                   float tess_tol,
                                                   int level)
{
    float dx = x4 - x1;
    float dy = y4 - y1;
    float d2 = ((x2 - x4) * dy - (y2 - y4) * dx);
    float d3 = ((x3 - x4) * dy - (y3 - y4) * dx);
    d2 = (d2 >= 0) ? d2 : -d2;
    d3 = (d3 >= 0) ? d3 : -d3;
    if ((d2 + d3) * (d2 + d3) < tess_tol * (dx * dx + dy * dy))
    {
        AzVec2 p_current(x4, y4);
        AzVec2 p_line = AzLineClosestPoint(p_last, p_current, p);
        float dist2 = AzLengthSqr(p - p_line);
        if (dist2 < p_closest_dist2)
        {
            p_closest = p_line;
            p_closest_dist2 = dist2;
        }
        p_last = p_current;
    } else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x34 = (x3 + x4) * 0.5f, y34 = (y3 + y4) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
        float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
        AzBezierCubicClosestPointCasteljauStep(p,
                                               p_closest,
                                               p_last,
                                               p_closest_dist2,
                                               x1,
                                               y1,
                                               x12,
                                               y12,
                                               x123,
                                               y123,
                                               x1234,
                                               y1234,
                                               tess_tol,
                                               level + 1);
        AzBezierCubicClosestPointCasteljauStep(p,
                                               p_closest,
                                               p_last,
                                               p_closest_dist2,
                                               x1234,
                                               y1234,
                                               x234,
                                               y234,
                                               x34,
                                               y34,
                                               x4,
                                               y4,
                                               tess_tol,
                                               level + 1);
    }
}

AzVec2 AzBezierCubicClosestPointCasteljau(const AzVec2& p1,
                                          const AzVec2& p2,
                                          const AzVec2& p3,
                                          const AzVec2& p4,
                                          const AzVec2& p,
                                          float tess_tol)
{
    AZ_ASSERT(tess_tol > 0.0f);
    AzVec2 p_last = p1;
    AzVec2 p_closest;
    float p_closest_dist2 = FLT_MAX;
    AzBezierCubicClosestPointCasteljauStep(p,
                                           p_closest,
                                           p_last,
                                           p_closest_dist2,
                                           p1.x,
                                           p1.y,
                                           p2.x,
                                           p2.y,
                                           p3.x,
                                           p3.y,
                                           p4.x,
                                           p4.y,
                                           tess_tol,
                                           0);
    return p_closest;
}

AzVec2 AzLineClosestPoint(const AzVec2& a, const AzVec2& b, const AzVec2& p)
{
    AzVec2 ap = p - a;
    AzVec2 ab_dir = b - a;
    float dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
    if (dot < 0.0f)
        return a;
    float ab_len_sqr = ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y;
    if (dot > ab_len_sqr)
        return b;
    return a + ab_dir * dot / ab_len_sqr;
}

bool AzTriangleContainsPoint(const AzVec2& a,
                             const AzVec2& b,
                             const AzVec2& c,
                             const AzVec2& p)
{
    bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.0f;
    bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.0f;
    bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.0f;
    return ((b1 == b2) && (b2 == b3));
}

void AzTriangleBarycentricCoords(const AzVec2& a,
                                 const AzVec2& b,
                                 const AzVec2& c,
                                 const AzVec2& p,
                                 float& out_u,
                                 float& out_v,
                                 float& out_w)
{
    AzVec2 v0 = b - a;
    AzVec2 v1 = c - a;
    AzVec2 v2 = p - a;
    const float denom = v0.x * v1.y - v1.x * v0.y;
    out_v = (v2.x * v1.y - v1.x * v2.y) / denom;
    out_w = (v0.x * v2.y - v2.x * v0.y) / denom;
    out_u = 1.0f - out_v - out_w;
}

AzVec2 AzTriangleClosestPoint(const AzVec2& a,
                              const AzVec2& b,
                              const AzVec2& c,
                              const AzVec2& p)
{
    AzVec2 proj_ab = AzLineClosestPoint(a, b, p);
    AzVec2 proj_bc = AzLineClosestPoint(b, c, p);
    AzVec2 proj_ca = AzLineClosestPoint(c, a, p);
    float dist2_ab = AzLengthSqr(p - proj_ab);
    float dist2_bc = AzLengthSqr(p - proj_bc);
    float dist2_ca = AzLengthSqr(p - proj_ca);
    float m = AzMin(dist2_ab, AzMin(dist2_bc, dist2_ca));
    if (m == dist2_ab)
        return proj_ab;
    if (m == dist2_bc)
        return proj_bc;
    return proj_ca;
}

int AzStricmp(const char* str1, const char* str2)
{
    int d;
    while ((d = AzToUpper(*str2) - AzToUpper(*str1)) == 0 && *str1)
    {
        str1++;
        str2++;
    }
    return d;
}

int AzStrnicmp(const char* str1, const char* str2, size_t count)
{
    int d = 0;
    while (count > 0 && (d = AzToUpper(*str2) - AzToUpper(*str1)) == 0 && *str1)
    {
        str1++;
        str2++;
        count--;
    }
    return d;
}

void AzStrncpy(char* dst, const char* src, size_t count)
{
    if (count < 1)
        return;
    if (count > 1)
        strncpy(dst, src, count - 1);
    dst[count - 1] = 0;
}

char* AzStrdup(const char* str)
{
    size_t len = AzStrlen(str);
    void* buf = AZ_ALLOC(len + 1);
    return (char*)memcpy(buf, (const void*)str, len + 1);
}

void* AzMemdup(const void* src, size_t size)
{
    void* dst = AZ_ALLOC(size);
    return memcpy(dst, src, size);
}

char* AzStrdupcpy(char* dst, size_t* p_dst_size, const char* src)
{
    size_t dst_buf_size = p_dst_size ? *p_dst_size : AzStrlen(dst) + 1;
    size_t src_size = AzStrlen(src) + 1;
    if (dst_buf_size < src_size)
    {
        AZ_FREE(dst);
        dst = (char*)AZ_ALLOC(src_size);
        if (p_dst_size)
            *p_dst_size = src_size;
    }
    return (char*)memcpy(dst, (const void*)src, src_size);
}

const char* AzStrchrRange(const char* str, const char* str_end, char c)
{
    const char* p = (const char*)AzMemchr(str, (int)c, str_end - str);
    return p;
}

int AzStrlenW(const AzWchar* str)
{
    int n = 0;
    while (*str++)
        n++;
    return n;
}

const char* AzStreolRange(const char* str, const char* str_end)
{
    const char* p = (const char*)AzMemchr(str, '\n', str_end - str);
    return p ? p : str_end;
}

const char* AzStrbol(const char* buf_mid_line, const char* buf_begin)
{
    AZ_ASSERT_PARANOID(buf_mid_line >= buf_begin
                       && buf_mid_line <= buf_begin + AzStrlen(buf_begin));
    while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n')
        buf_mid_line--;
    return buf_mid_line;
}

const char* AzStristr(const char* haystack,
                      const char* haystack_end,
                      const char* needle,
                      const char* needle_end)
{
    if (!needle_end)
        needle_end = needle + AzStrlen(needle);

    const char un0 = (char)AzToUpper(*needle);
    while ((!haystack_end && *haystack)
           || (haystack_end && haystack < haystack_end))
    {
        if (AzToUpper(*haystack) == un0)
        {
            const char* b = needle + 1;
            for (const char* a = haystack + 1; b < needle_end; a++, b++)
                if (AzToUpper(*a) != AzToUpper(*b))
                    break;
            if (b == needle_end)
                return haystack;
        }
        haystack++;
    }
    return NULL;
}

void AzStrTrimBlanks(char* buf)
{
    char* p = buf;
    while (p[0] == ' ' || p[0] == '\t')
        p++;
    char* p_start = p;
    while (*p != 0)
        p++;
    while (p > p_start && (p[-1] == ' ' || p[-1] == '\t'))
        p--;
    if (p_start != buf)
        memmove(buf, p_start, p - p_start);
    buf[p - p_start] = 0;
}

const char* AzStrSkipBlank(const char* str)
{
    while (str[0] == ' ' || str[0] == '\t')
        str++;
    return str;
}

#ifndef AZ_DISABLE_DEFAULT_FORMAT_FUNCTIONS

#ifdef AZ_USE_STB_SPRINTF
#ifndef AZ_DISABLE_STB_SPRINTF_AZPLEMENTATION
#define STB_SPRINTF_AZPLEMENTATION
#endif
#ifdef AZ_STB_SPRINTF_FILENAME
#include AZ_STB_SPRINTF_FILENAME
#else
#include "stb_sprintf.h"
#endif
#endif

#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf _vsnprintf
#endif

int AzFormatString(char* buf, size_t buf_size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
#ifdef AZ_USE_STB_SPRINTF
    int w = stbsp_vsnprintf(buf, (int)buf_size, fmt, args);
#else
    int w = vsnprintf(buf, buf_size, fmt, args);
#endif
    va_end(args);
    if (buf == NULL)
        return w;
    if (w == -1 || w >= (int)buf_size)
        w = (int)buf_size - 1;
    buf[w] = 0;
    return w;
}

int AzFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args)
{
#ifdef AZ_USE_STB_SPRINTF
    int w = stbsp_vsnprintf(buf, (int)buf_size, fmt, args);
#else
    int w = vsnprintf(buf, buf_size, fmt, args);
#endif
    if (buf == NULL)
        return w;
    if (w == -1 || w >= (int)buf_size)
        w = (int)buf_size - 1;
    buf[w] = 0;
    return w;
}
#endif

void AzFormatStringToTempBuffer(const char** out_buf,
                                const char** out_buf_end,
                                const char* fmt,
                                ...)
{
    va_list args;
    va_start(args, fmt);
    AzFormatStringToTempBufferV(out_buf, out_buf_end, fmt, args);
    va_end(args);
}

void AzFormatStringToTempBufferV(const char** out_buf,
                                 const char** out_buf_end,
                                 const char* fmt,
                                 va_list args)
{
    AzContext& g = *GAz;
    if (fmt[0] == '%' && fmt[1] == 's' && fmt[2] == 0)
    {
        const char* buf = va_arg(args, const char*);

        if (buf == NULL)
            buf = "(null)";
        *out_buf = buf;
        if (out_buf_end)
        {
            *out_buf_end = buf + AzStrlen(buf);
        }
    } else if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '*' && fmt[3] == 's'
               && fmt[4] == 0)
    {
        int buf_len = va_arg(args, int);
        const char* buf = va_arg(args, const char*);
        if (buf == NULL)
        {
            buf = "(null)";
            buf_len = AzMin(buf_len, 6);
        }
        *out_buf = buf;
        *out_buf_end = buf + buf_len;

    } else
    {
        int buf_len = AzFormatStringV(g.TempBuffer.Data,
                                      g.TempBuffer.Size,
                                      fmt,
                                      args);
        *out_buf = g.TempBuffer.Data;
        if (out_buf_end)
        {
            *out_buf_end = g.TempBuffer.Data + buf_len;
        }
    }
}

#ifndef AZ_ENABLE_SSE4_2_CRC

static const AzU32 GCrc32LookupTable[256] = {
#ifdef AZ_USE_LEGACY_CRC32_ADLER

        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
        0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
        0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
        0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
        0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
        0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
        0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
        0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
        0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
        0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
        0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
        0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
        0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
        0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
        0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
        0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
        0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
        0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
        0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
        0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
#else

        0x00000000, 0xF26B8303, 0xE13B70F7, 0x1350F3F4, 0xC79A971F, 0x35F1141C,
        0x26A1E7E8, 0xD4CA64EB, 0x8AD958CF, 0x78B2DBCC, 0x6BE22838, 0x9989AB3B,
        0x4D43CFD0, 0xBF284CD3, 0xAC78BF27, 0x5E133C24, 0x105EC76F, 0xE235446C,
        0xF165B798, 0x030E349B, 0xD7C45070, 0x25AFD373, 0x36FF2087, 0xC494A384,
        0x9A879FA0, 0x68EC1CA3, 0x7BBCEF57, 0x89D76C54, 0x5D1D08BF, 0xAF768BBC,
        0xBC267848, 0x4E4DFB4B, 0x20BD8EDE, 0xD2D60DDD, 0xC186FE29, 0x33ED7D2A,
        0xE72719C1, 0x154C9AC2, 0x061C6936, 0xF477EA35, 0xAA64D611, 0x580F5512,
        0x4B5FA6E6, 0xB93425E5, 0x6DFE410E, 0x9F95C20D, 0x8CC531F9, 0x7EAEB2FA,
        0x30E349B1, 0xC288CAB2, 0xD1D83946, 0x23B3BA45, 0xF779DEAE, 0x05125DAD,
        0x1642AE59, 0xE4292D5A, 0xBA3A117E, 0x4851927D, 0x5B016189, 0xA96AE28A,
        0x7DA08661, 0x8FCB0562, 0x9C9BF696, 0x6EF07595, 0x417B1DBC, 0xB3109EBF,
        0xA0406D4B, 0x522BEE48, 0x86E18AA3, 0x748A09A0, 0x67DAFA54, 0x95B17957,
        0xCBA24573, 0x39C9C670, 0x2A993584, 0xD8F2B687, 0x0C38D26C, 0xFE53516F,
        0xED03A29B, 0x1F682198, 0x5125DAD3, 0xA34E59D0, 0xB01EAA24, 0x42752927,
        0x96BF4DCC, 0x64D4CECF, 0x77843D3B, 0x85EFBE38, 0xDBFC821C, 0x2997011F,
        0x3AC7F2EB, 0xC8AC71E8, 0x1C661503, 0xEE0D9600, 0xFD5D65F4, 0x0F36E6F7,
        0x61C69362, 0x93AD1061, 0x80FDE395, 0x72966096, 0xA65C047D, 0x5437877E,
        0x4767748A, 0xB50CF789, 0xEB1FCBAD, 0x197448AE, 0x0A24BB5A, 0xF84F3859,
        0x2C855CB2, 0xDEEEDFB1, 0xCDBE2C45, 0x3FD5AF46, 0x7198540D, 0x83F3D70E,
        0x90A324FA, 0x62C8A7F9, 0xB602C312, 0x44694011, 0x5739B3E5, 0xA55230E6,
        0xFB410CC2, 0x092A8FC1, 0x1A7A7C35, 0xE811FF36, 0x3CDB9BDD, 0xCEB018DE,
        0xDDE0EB2A, 0x2F8B6829, 0x82F63B78, 0x709DB87B, 0x63CD4B8F, 0x91A6C88C,
        0x456CAC67, 0xB7072F64, 0xA457DC90, 0x563C5F93, 0x082F63B7, 0xFA44E0B4,
        0xE9141340, 0x1B7F9043, 0xCFB5F4A8, 0x3DDE77AB, 0x2E8E845F, 0xDCE5075C,
        0x92A8FC17, 0x60C37F14, 0x73938CE0, 0x81F80FE3, 0x55326B08, 0xA759E80B,
        0xB4091BFF, 0x466298FC, 0x1871A4D8, 0xEA1A27DB, 0xF94AD42F, 0x0B21572C,
        0xDFEB33C7, 0x2D80B0C4, 0x3ED04330, 0xCCBBC033, 0xA24BB5A6, 0x502036A5,
        0x4370C551, 0xB11B4652, 0x65D122B9, 0x97BAA1BA, 0x84EA524E, 0x7681D14D,
        0x2892ED69, 0xDAF96E6A, 0xC9A99D9E, 0x3BC21E9D, 0xEF087A76, 0x1D63F975,
        0x0E330A81, 0xFC588982, 0xB21572C9, 0x407EF1CA, 0x532E023E, 0xA145813D,
        0x758FE5D6, 0x87E466D5, 0x94B49521, 0x66DF1622, 0x38CC2A06, 0xCAA7A905,
        0xD9F75AF1, 0x2B9CD9F2, 0xFF56BD19, 0x0D3D3E1A, 0x1E6DCDEE, 0xEC064EED,
        0xC38D26C4, 0x31E6A5C7, 0x22B65633, 0xD0DDD530, 0x0417B1DB, 0xF67C32D8,
        0xE52CC12C, 0x1747422F, 0x49547E0B, 0xBB3FFD08, 0xA86F0EFC, 0x5A048DFF,
        0x8ECEE914, 0x7CA56A17, 0x6FF599E3, 0x9D9E1AE0, 0xD3D3E1AB, 0x21B862A8,
        0x32E8915C, 0xC083125F, 0x144976B4, 0xE622F5B7, 0xF5720643, 0x07198540,
        0x590AB964, 0xAB613A67, 0xB831C993, 0x4A5A4A90, 0x9E902E7B, 0x6CFBAD78,
        0x7FAB5E8C, 0x8DC0DD8F, 0xE330A81A, 0x115B2B19, 0x020BD8ED, 0xF0605BEE,
        0x24AA3F05, 0xD6C1BC06, 0xC5914FF2, 0x37FACCF1, 0x69E9F0D5, 0x9B8273D6,
        0x88D28022, 0x7AB90321, 0xAE7367CA, 0x5C18E4C9, 0x4F48173D, 0xBD23943E,
        0xF36E6F75, 0x0105EC76, 0x12551F82, 0xE03E9C81, 0x34F4F86A, 0xC69F7B69,
        0xD5CF889D, 0x27A40B9E, 0x79B737BA, 0x8BDCB4B9, 0x988C474D, 0x6AE7C44E,
        0xBE2DA0A5, 0x4C4623A6, 0x5F16D052, 0xAD7D5351
#endif
};
#endif

AzID AzHashData(const void* data_p, size_t data_size, AzID seed)
{
    AzU32 crc = ~seed;
    const unsigned char* data = (const unsigned char*)data_p;
    const unsigned char* data_end = (const unsigned char*)data_p + data_size;
#ifndef AZ_ENABLE_SSE4_2_CRC
    const AzU32* crc32_lut = GCrc32LookupTable;
    while (data < data_end)
        crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *data++];
    return ~crc;
#else
    while (data + 4 <= data_end)
    {
        crc = _mm_crc32_u32(crc, *(AzU32*)data);
        data += 4;
    }
    while (data < data_end)
        crc = _mm_crc32_u8(crc, *data++);
    return ~crc;
#endif
}

AzID AzHashStr(const char* data_p, size_t data_size, AzID seed)
{
    seed = ~seed;
    AzU32 crc = seed;
    const unsigned char* data = (const unsigned char*)data_p;
#ifndef AZ_ENABLE_SSE4_2_CRC
    const AzU32* crc32_lut = GCrc32LookupTable;
#endif
    if (data_size != 0)
    {
        while (data_size-- != 0)
        {
            unsigned char c = *data++;
            if (c == '#' && data_size >= 2 && data[0] == '#' && data[1] == '#')
                crc = seed;
#ifndef AZ_ENABLE_SSE4_2_CRC
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
#else
            crc = _mm_crc32_u8(crc, c);
#endif
        }
    } else
    {
        while (unsigned char c = *data++)
        {
            if (c == '#' && data[0] == '#' && data[1] == '#')
                crc = seed;
#ifndef AZ_ENABLE_SSE4_2_CRC
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
#else
            crc = _mm_crc32_u8(crc, c);
#endif
        }
    }
    return ~crc;
}

#ifndef AZ_DISABLE_DEFAULT_FILE_FUNCTIONS

AzFileHandle AzFileOpen(const char* filename, const char* mode)
{
#if defined(_WIN32) && !defined(AZ_DISABLE_WIN32_FUNCTIONS)                    \
        && (defined(__MINGW32__)                                               \
            || (!defined(__CYGWIN__) && !defined(__GNUC__)))

    const int filename_wsize =
            ::MultiByteToWideChar(CP_UTF8, 0, filename, -1, NULL, 0);
    const int mode_wsize = ::MultiByteToWideChar(CP_UTF8, 0, mode, -1, NULL, 0);

    wchar_t local_temp_stack[FILENAME_MAX];
    AzVector<wchar_t> local_temp_heap;
    if (filename_wsize + mode_wsize > AZ_ARRAYSIZE(local_temp_stack))
        local_temp_heap.resize(filename_wsize + mode_wsize);
    wchar_t* filename_wbuf = local_temp_heap.Data ? local_temp_heap.Data
                                                  : local_temp_stack;
    wchar_t* mode_wbuf = filename_wbuf + filename_wsize;
    ::MultiByteToWideChar(CP_UTF8,
                          0,
                          filename,
                          -1,
                          filename_wbuf,
                          filename_wsize);
    ::MultiByteToWideChar(CP_UTF8, 0, mode, -1, mode_wbuf, mode_wsize);
    return ::_wfopen(filename_wbuf, mode_wbuf);
#else
    return fopen(filename, mode);
#endif
}

bool AzFileClose(AzFileHandle f)
{
    return fclose(f) == 0;
}
AzU64 AzFileGetSize(AzFileHandle f)
{
    long off = 0, sz = 0;
    return ((off = ftell(f)) != -1 && !fseek(f, 0, SEEK_END)
            && (sz = ftell(f)) != -1 && !fseek(f, off, SEEK_SET))
                   ? (AzU64)sz
                   : (AzU64)-1;
}
AzU64 AzFileRead(void* data, AzU64 sz, AzU64 count, AzFileHandle f)
{
    return fread(data, (size_t)sz, (size_t)count, f);
}
AzU64 AzFileWrite(const void* data, AzU64 sz, AzU64 count, AzFileHandle f)
{
    return fwrite(data, (size_t)sz, (size_t)count, f);
}
#endif

void* AzFileLoadToMemory(const char* filename,
                         const char* mode,
                         size_t* out_file_size,
                         int padding_bytes)
{
    AZ_ASSERT(filename && mode);
    if (out_file_size)
        *out_file_size = 0;

    AzFileHandle f;
    if ((f = AzFileOpen(filename, mode)) == NULL)
        return NULL;

    size_t file_size = (size_t)AzFileGetSize(f);
    if (file_size == (size_t)-1)
    {
        AzFileClose(f);
        return NULL;
    }

    void* file_data = AZ_ALLOC(file_size + padding_bytes);
    if (file_data == NULL)
    {
        AzFileClose(f);
        return NULL;
    }
    if (AzFileRead(file_data, 1, file_size, f) != file_size)
    {
        AzFileClose(f);
        AZ_FREE(file_data);
        return NULL;
    }
    if (padding_bytes > 0)
        memset((void*)(((char*)file_data) + file_size),
               0,
               (size_t)padding_bytes);

    AzFileClose(f);
    if (out_file_size)
        *out_file_size = file_size;

    return file_data;
}

AZ_MSVC_RUNTIME_CHECKS_OFF

int AzTextCharFromUtf8(unsigned int* out_char,
                       const char* in_text,
                       const char* in_text_end)
{
    static const char lengths[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                     1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
                                     0, 0, 2, 2, 2, 2, 3, 3, 4, 0};
    static const int masks[] = {0x00, 0x7f, 0x1f, 0x0f, 0x07};
    static const uint32_t mins[] = {0x400000, 0, 0x80, 0x800, 0x10000};
    static const int shiftc[] = {0, 18, 12, 6, 0};
    static const int shifte[] = {0, 6, 4, 2, 0};
    int len = lengths[*(const unsigned char*)in_text >> 3];
    int wanted = len + (len ? 0 : 1);

    if (in_text_end == NULL)
        in_text_end = in_text + wanted;

    unsigned char s[4];
    s[0] = in_text + 0 < in_text_end ? in_text[0] : 0;
    s[1] = in_text + 1 < in_text_end ? in_text[1] : 0;
    s[2] = in_text + 2 < in_text_end ? in_text[2] : 0;
    s[3] = in_text + 3 < in_text_end ? in_text[3] : 0;

    *out_char = (uint32_t)(s[0] & masks[len]) << 18;
    *out_char |= (uint32_t)(s[1] & 0x3f) << 12;
    *out_char |= (uint32_t)(s[2] & 0x3f) << 6;
    *out_char |= (uint32_t)(s[3] & 0x3f) << 0;
    *out_char >>= shiftc[len];

    int e = 0;
    e = (*out_char < mins[len]) << 6;
    e |= ((*out_char >> 11) == 0x1b) << 7;
    e |= (*out_char > AZ_UNICODE_CODEPOINT_MAX) << 8;
    e |= (s[1] & 0xc0) >> 2;
    e |= (s[2] & 0xc0) >> 4;
    e |= (s[3]) >> 6;
    e ^= 0x2a;
    e >>= shifte[len];

    if (e)
    {
        wanted = AzMin(wanted, !!s[0] + !!s[1] + !!s[2] + !!s[3]);
        *out_char = AZ_UNICODE_CODEPOINT_INVALID;
    }

    return wanted;
}

int AzTextStrFromUtf8(AzWchar* buf,
                      int buf_size,
                      const char* in_text,
                      const char* in_text_end,
                      const char** in_text_remaining)
{
    AzWchar* buf_out = buf;
    AzWchar* buf_end = buf + buf_size;
    while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end)
           && *in_text)
    {
        unsigned int c;
        in_text += AzTextCharFromUtf8(&c, in_text, in_text_end);
        *buf_out++ = (AzWchar)c;
    }
    *buf_out = 0;
    if (in_text_remaining)
        *in_text_remaining = in_text;
    return (int)(buf_out - buf);
}

int AzTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
    int char_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c;
        in_text += AzTextCharFromUtf8(&c, in_text, in_text_end);
        char_count++;
    }
    return char_count;
}

static inline int AzTextCharToUtf8_inline(char* buf,
                                          int buf_size,
                                          unsigned int c)
{
    if (c < 0x80)
    {
        buf[0] = (char)c;
        return 1;
    }
    if (c < 0x800)
    {
        if (buf_size < 2)
            return 0;
        buf[0] = (char)(0xc0 + (c >> 6));
        buf[1] = (char)(0x80 + (c & 0x3f));
        return 2;
    }
    if (c < 0x10000)
    {
        if (buf_size < 3)
            return 0;
        buf[0] = (char)(0xe0 + (c >> 12));
        buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
        buf[2] = (char)(0x80 + ((c) & 0x3f));
        return 3;
    }
    if (c <= 0x10FFFF)
    {
        if (buf_size < 4)
            return 0;
        buf[0] = (char)(0xf0 + (c >> 18));
        buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
        buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
        buf[3] = (char)(0x80 + ((c) & 0x3f));
        return 4;
    }

    return 0;
}

const char* AzTextCharToUtf8(char out_buf[5], unsigned int c)
{
    int count = AzTextCharToUtf8_inline(out_buf, 5, c);
    out_buf[count] = 0;
    return out_buf;
}

int AzTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end)
{
    unsigned int unused = 0;
    return AzTextCharFromUtf8(&unused, in_text, in_text_end);
}

static inline int AzTextCountUtf8BytesFromChar(unsigned int c)
{
    if (c < 0x80)
        return 1;
    if (c < 0x800)
        return 2;
    if (c < 0x10000)
        return 3;
    if (c <= 0x10FFFF)
        return 4;
    return 3;
}

int AzTextStrToUtf8(char* out_buf,
                    int out_buf_size,
                    const AzWchar* in_text,
                    const AzWchar* in_text_end)
{
    char* buf_p = out_buf;
    const char* buf_end = out_buf + out_buf_size;
    while (buf_p < buf_end - 1 && (!in_text_end || in_text < in_text_end)
           && *in_text)
    {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            *buf_p++ = (char)c;
        else
            buf_p += AzTextCharToUtf8_inline(buf_p,
                                             (int)(buf_end - buf_p - 1),
                                             c);
    }
    *buf_p = 0;
    return (int)(buf_p - out_buf);
}

int AzTextCountUtf8BytesFromStr(const AzWchar* in_text,
                                const AzWchar* in_text_end)
{
    int bytes_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            bytes_count++;
        else
            bytes_count += AzTextCountUtf8BytesFromChar(c);
    }
    return bytes_count;
}

const char* AzTextFindPreviousUtf8Codepoint(const char* in_text_start,
                                            const char* in_text_curr)
{
    while (in_text_curr > in_text_start)
    {
        in_text_curr--;
        if ((*in_text_curr & 0xC0) != 0x80)
            return in_text_curr;
    }
    return in_text_start;
}

int AzTextCountLines(const char* in_text, const char* in_text_end)
{
    if (in_text_end == NULL)
        in_text_end = in_text + AzStrlen(in_text);

    int count = 0;
    while (in_text < in_text_end)
    {
        const char* line_end = (const char*)AzMemchr(in_text,
                                                     '\n',
                                                     in_text_end - in_text);
        in_text = line_end ? line_end + 1 : in_text_end;
        count++;
    }
    return count;
}

AZ_MSVC_RUNTIME_CHECKS_RESTORE

AZ_API AzU32 AzAlphaBlendColors(AzU32 col_a, AzU32 col_b)
{
    float t = ((col_b >> AZ_COL32_A_SHIFT) & 0xFF) / 255.f;
    int r = AzLerp((int)(col_a >> AZ_COL32_R_SHIFT) & 0xFF,
                   (int)(col_b >> AZ_COL32_R_SHIFT) & 0xFF,
                   t);
    int g = AzLerp((int)(col_a >> AZ_COL32_G_SHIFT) & 0xFF,
                   (int)(col_b >> AZ_COL32_G_SHIFT) & 0xFF,
                   t);
    int b = AzLerp((int)(col_a >> AZ_COL32_B_SHIFT) & 0xFF,
                   (int)(col_b >> AZ_COL32_B_SHIFT) & 0xFF,
                   t);
    return AZ_COL32(r, g, b, 0xFF);
}

AzVec4 Az::ColorConvertU32ToFloat4(AzU32 in)
{
    float s = 1.0f / 255.0f;
    return AzVec4(((in >> AZ_COL32_R_SHIFT) & 0xFF) * s,
                  ((in >> AZ_COL32_G_SHIFT) & 0xFF) * s,
                  ((in >> AZ_COL32_B_SHIFT) & 0xFF) * s,
                  ((in >> AZ_COL32_A_SHIFT) & 0xFF) * s);
}

AzU32 Az::ColorConvertFloat4ToU32(const AzVec4& in)
{
    AzU32 out;
    out = ((AzU32)AZ_F32_TO_INT8_SAT(in.x)) << AZ_COL32_R_SHIFT;
    out |= ((AzU32)AZ_F32_TO_INT8_SAT(in.y)) << AZ_COL32_G_SHIFT;
    out |= ((AzU32)AZ_F32_TO_INT8_SAT(in.z)) << AZ_COL32_B_SHIFT;
    out |= ((AzU32)AZ_F32_TO_INT8_SAT(in.w)) << AZ_COL32_A_SHIFT;
    return out;
}

void Az::ColorConvertRGBtoHSV(float r,
                              float g,
                              float b,
                              float& out_h,
                              float& out_s,
                              float& out_v)
{
    float K = 0.f;
    if (g < b)
    {
        AzSwap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        AzSwap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = AzFabs(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

void Az::ColorConvertHSVtoRGB(float h,
                              float s,
                              float v,
                              float& out_r,
                              float& out_g,
                              float& out_b)
{
    if (s == 0.0f)
    {
        out_r = out_g = out_b = v;
        return;
    }

    h = AzFmod(h, 1.0f) / (60.0f / 360.0f);
    int i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0:
        out_r = v;
        out_g = t;
        out_b = p;
        break;
    case 1:
        out_r = q;
        out_g = v;
        out_b = p;
        break;
    case 2:
        out_r = p;
        out_g = v;
        out_b = t;
        break;
    case 3:
        out_r = p;
        out_g = q;
        out_b = v;
        break;
    case 4:
        out_r = t;
        out_g = p;
        out_b = v;
        break;
    case 5:
    default:
        out_r = v;
        out_g = p;
        out_b = q;
        break;
    }
}

AzStoragePair* AzLowerBound(AzStoragePair* in_begin,
                            AzStoragePair* in_end,
                            AzID key)
{
    AzStoragePair* in_p = in_begin;
    for (size_t count = (size_t)(in_end - in_p); count > 0;)
    {
        size_t count2 = count >> 1;
        AzStoragePair* mid = in_p + count2;
        if (mid->key < key)
        {
            in_p = ++mid;
            count -= count2 + 1;
        } else
        {
            count = count2;
        }
    }
    return in_p;
}

AZ_MSVC_RUNTIME_CHECKS_OFF
static int AZ_CDECL PairComparerByID(const void* lhs, const void* rhs)
{
    AzID lhs_v = ((const AzStoragePair*)lhs)->key;
    AzID rhs_v = ((const AzStoragePair*)rhs)->key;
    return (lhs_v > rhs_v ? +1 : lhs_v < rhs_v ? -1 : 0);
}

void AzStorage::BuildSortByKey()
{
    AzQsort(Data.Data,
            (size_t)Data.Size,
            sizeof(AzStoragePair),
            PairComparerByID);
}

int AzStorage::GetInt(AzID key, int default_val) const
{
    AzStoragePair* it = AzLowerBound(const_cast<AzStoragePair*>(Data.Data),
                                     const_cast<AzStoragePair*>(Data.Data
                                                                + Data.Size),
                                     key);
    if (it == Data.Data + Data.Size || it->key != key)
        return default_val;
    return it->val_i;
}

bool AzStorage::GetBool(AzID key, bool default_val) const
{
    return GetInt(key, default_val ? 1 : 0) != 0;
}

float AzStorage::GetFloat(AzID key, float default_val) const
{
    AzStoragePair* it = AzLowerBound(const_cast<AzStoragePair*>(Data.Data),
                                     const_cast<AzStoragePair*>(Data.Data
                                                                + Data.Size),
                                     key);
    if (it == Data.Data + Data.Size || it->key != key)
        return default_val;
    return it->val_f;
}

void* AzStorage::GetVoidPtr(AzID key) const
{
    AzStoragePair* it = AzLowerBound(const_cast<AzStoragePair*>(Data.Data),
                                     const_cast<AzStoragePair*>(Data.Data
                                                                + Data.Size),
                                     key);
    if (it == Data.Data + Data.Size || it->key != key)
        return NULL;
    return it->val_p;
}

int* AzStorage::GetIntRef(AzID key, int default_val)
{
    AzStoragePair* it = AzLowerBound(Data.Data, Data.Data + Data.Size, key);
    if (it == Data.Data + Data.Size || it->key != key)
        it = Data.insert(it, AzStoragePair(key, default_val));
    return &it->val_i;
}

bool* AzStorage::GetBoolRef(AzID key, bool default_val)
{
    return (bool*)GetIntRef(key, default_val ? 1 : 0);
}

float* AzStorage::GetFloatRef(AzID key, float default_val)
{
    AzStoragePair* it = AzLowerBound(Data.Data, Data.Data + Data.Size, key);
    if (it == Data.Data + Data.Size || it->key != key)
        it = Data.insert(it, AzStoragePair(key, default_val));
    return &it->val_f;
}

void** AzStorage::GetVoidPtrRef(AzID key, void* default_val)
{
    AzStoragePair* it = AzLowerBound(Data.Data, Data.Data + Data.Size, key);
    if (it == Data.Data + Data.Size || it->key != key)
        it = Data.insert(it, AzStoragePair(key, default_val));
    return &it->val_p;
}

void AzStorage::SetInt(AzID key, int val)
{
    AzStoragePair* it = AzLowerBound(Data.Data, Data.Data + Data.Size, key);
    if (it == Data.Data + Data.Size || it->key != key)
        Data.insert(it, AzStoragePair(key, val));
    else
        it->val_i = val;
}

void AzStorage::SetBool(AzID key, bool val)
{
    SetInt(key, val ? 1 : 0);
}

void AzStorage::SetFloat(AzID key, float val)
{
    AzStoragePair* it = AzLowerBound(Data.Data, Data.Data + Data.Size, key);
    if (it == Data.Data + Data.Size || it->key != key)
        Data.insert(it, AzStoragePair(key, val));
    else
        it->val_f = val;
}

void AzStorage::SetVoidPtr(AzID key, void* val)
{
    AzStoragePair* it = AzLowerBound(Data.Data, Data.Data + Data.Size, key);
    if (it == Data.Data + Data.Size || it->key != key)
        Data.insert(it, AzStoragePair(key, val));
    else
        it->val_p = val;
}

void AzStorage::SetAllInt(int v)
{
    for (int i = 0; i < Data.Size; i++)
        Data[i].val_i = v;
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

AzTextFilter::AzTextFilter(const char* default_filter)
{
    InputBuf[0] = 0;
    CountGrep = 0;
    if (default_filter)
    {
        AzStrncpy(InputBuf, default_filter, AZ_ARRAYSIZE(InputBuf));
        Build();
    }
}

bool AzTextFilter::Draw(const char* label, float width)
{
    if (width != 0.0f)
        Az::SetNextItemWidth(width);
    bool value_changed = Az::InputText(label, InputBuf, AZ_ARRAYSIZE(InputBuf));
    if (value_changed)
        Build();
    return value_changed;
}

void AzTextFilter::AzTextRange::split(char separator,
                                      AzVector<AzTextRange>* out) const
{
    out->resize(0);
    const char* wb = b;
    const char* we = wb;
    while (we < e)
    {
        if (*we == separator)
        {
            out->push_back(AzTextRange(wb, we));
            wb = we + 1;
        }
        we++;
    }
    if (wb != we)
        out->push_back(AzTextRange(wb, we));
}

void AzTextFilter::Build()
{
    Filters.resize(0);
    AzTextRange input_range(InputBuf, InputBuf + AzStrlen(InputBuf));
    input_range.split(',', &Filters);

    CountGrep = 0;
    for (AzTextRange& f : Filters)
    {
        while (f.b < f.e && AzCharIsBlankA(f.b[0]))
            f.b++;
        while (f.e > f.b && AzCharIsBlankA(f.e[-1]))
            f.e--;
        if (f.empty())
            continue;
        if (f.b[0] != '-')
            CountGrep += 1;
    }
}

bool AzTextFilter::PassFilter(const char* text, const char* text_end) const
{
    if (Filters.Size == 0)
        return true;

    if (text == NULL)
        text = text_end = "";

    for (const AzTextRange& f : Filters)
    {
        if (f.b == f.e)
            continue;
        if (f.b[0] == '-')
        {
            if (AzStristr(text, text_end, f.b + 1, f.e) != NULL)
                return false;
        } else
        {
            if (AzStristr(text, text_end, f.b, f.e) != NULL)
                return true;
        }
    }

    if (CountGrep == 0)
        return true;

    return false;
}

#ifndef va_copy
#if defined(__GNUC__) || defined(__clang__)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#else
#define va_copy(dest, src) (dest = src)
#endif
#endif

char AzTextBuffer::EmptyString[1] = {0};

void AzTextBuffer::append(const char* str, const char* str_end)
{
    int len = str_end ? (int)(str_end - str) : (int)AzStrlen(str);

    const int write_off = (Buf.Size != 0) ? Buf.Size : 1;
    const int needed_sz = write_off + len;
    if (write_off + len >= Buf.Capacity)
    {
        int new_capacity = Buf.Capacity * 2;
        Buf.reserve(needed_sz > new_capacity ? needed_sz : new_capacity);
    }

    Buf.resize(needed_sz);
    memcpy(&Buf[write_off - 1], str, (size_t)len);
    Buf[write_off - 1 + len] = 0;
}

void AzTextBuffer::appendf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendfv(fmt, args);
    va_end(args);
}

void AzTextBuffer::appendfv(const char* fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    int len = AzFormatStringV(NULL, 0, fmt, args);

    if (len <= 0)
    {
        va_end(args_copy);
        return;
    }

    const int write_off = (Buf.Size != 0) ? Buf.Size : 1;
    const int needed_sz = write_off + len;
    if (write_off + len >= Buf.Capacity)
    {
        int new_capacity = Buf.Capacity * 2;
        Buf.reserve(needed_sz > new_capacity ? needed_sz : new_capacity);
    }

    Buf.resize(needed_sz);
    AzFormatStringV(&Buf[write_off - 1], (size_t)len + 1, fmt, args_copy);
    va_end(args_copy);
}

void AzTextIndex::append(const char* base, int old_size, int new_size)
{
    AZ_ASSERT(old_size >= 0 && new_size >= old_size && new_size >= EndOffset);
    if (old_size == new_size)
        return;
    if (EndOffset == 0 || base[EndOffset - 1] == '\n')
        LineOffsets.push_back(EndOffset);
    const char* base_end = base + new_size;
    for (const char* p = base + old_size;
         (p = (const char*)AzMemchr(p, '\n', base_end - p)) != 0;)
        if (++p < base_end)
            LineOffsets.push_back((int)(intptr_t)(p - base));
    EndOffset = AzMax(EndOffset, new_size);
}

static bool GetSkipItemForListClipping()
{
    AzContext& g = *GAz;
    return (g.CurrentTable ? g.CurrentTable->HostSkipItems
                           : g.CurrentWindow->SkipItems);
}

static void AzListClipper_SortAndFuseRanges(
        AzVector<AzListClipperRange>& ranges,
        int offset = 0)
{
    if (ranges.Size - offset <= 1)
        return;

    for (int sort_end = ranges.Size - offset - 1; sort_end > 0; --sort_end)
        for (int i = offset; i < sort_end + offset; ++i)
            if (ranges[i].Min > ranges[i + 1].Min)
                AzSwap(ranges[i], ranges[i + 1]);

    for (int i = 1 + offset; i < ranges.Size; i++)
    {
        AZ_ASSERT(!ranges[i].PosToIndexConvert
                  && !ranges[i - 1].PosToIndexConvert);
        if (ranges[i - 1].Max < ranges[i].Min)
            continue;
        ranges[i - 1].Min = AzMin(ranges[i - 1].Min, ranges[i].Min);
        ranges[i - 1].Max = AzMax(ranges[i - 1].Max, ranges[i].Max);
        ranges.erase(ranges.Data + i);
        i--;
    }
}

static void AzListClipper_SeekCursorAndSetupPrevLine(float pos_y,
                                                     float line_height)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    float off_y = pos_y - window->DC.CursorPos.y;
    window->DC.CursorPos.y = pos_y;
    window->DC.CursorMaxPos.y = AzMax(window->DC.CursorMaxPos.y,
                                      pos_y - g.Style.ItemSpacing.y);
    window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y - line_height;

    window->DC.PrevLineSize.y = (line_height - g.Style.ItemSpacing.y);

    if (AzOldColumns* columns = window->DC.CurrentColumns)
        columns->LineMinY = window->DC.CursorPos.y;

    if (AzTable* table = g.CurrentTable)
    {
        if (table->IsInsideRow)
            Az::TableEndRow(table);
        table->RowPosY2 = window->DC.CursorPos.y;
        const int row_increase = (int)((off_y / line_height) + 0.5f);

        table->RowBgColorCounter += row_increase;
    }
}

AzListClipper::AzListClipper()
{
    memset(this, 0, sizeof(*this));
}

AzListClipper::~AzListClipper()
{
    End();
}

void AzListClipper::Begin(int items_count, float items_height)
{
    if (Ctx == NULL)
        Ctx = Az::GetCurrentContext();

    AzContext& g = *Ctx;
    AzWindow* window = g.CurrentWindow;
    AZ_DEBUG_LOG_CLIPPER("Clipper: Begin(%d,%.2f) in '%s'\n",
                         items_count,
                         items_height,
                         window->Name);

    if (AzTable* table = g.CurrentTable)
        if (table->IsInsideRow)
            Az::TableEndRow(table);

    StartPosY = window->DC.CursorPos.y;
    ItemsHeight = items_height;
    ItemsCount = items_count;
    DisplayStart = -1;
    DisplayEnd = 0;

    if (++g.ClipperTempDataStacked > g.ClipperTempData.Size)
        g.ClipperTempData.resize(g.ClipperTempDataStacked, AzListClipperData());
    AzListClipperData* data = &g.ClipperTempData[g.ClipperTempDataStacked - 1];
    data->Reset(this);
    data->LossynessOffset = window->DC.CursorStartPosLossyness.y;
    TempData = data;
    StartSeekOffsetY = data->LossynessOffset;
}

void AzListClipper::End()
{
    if (AzListClipperData* data = (AzListClipperData*)TempData)
    {
        AzContext& g = *Ctx;
        AZ_DEBUG_LOG_CLIPPER("Clipper: End() in '%s'\n", g.CurrentWindow->Name);
        if (ItemsCount >= 0 && ItemsCount < INT_MAX && DisplayStart >= 0)
            SeekCursorForItem(ItemsCount);

        AZ_ASSERT(data->ListClipper == this);
        data->StepNo = data->Ranges.Size;
        if (--g.ClipperTempDataStacked > 0)
        {
            data = &g.ClipperTempData[g.ClipperTempDataStacked - 1];
            data->ListClipper->TempData = data;
        }
        TempData = NULL;
    }
    ItemsCount = -1;
}

void AzListClipper::IncludeItemsByIndex(int item_begin, int item_end)
{
    AzListClipperData* data = (AzListClipperData*)TempData;
    AZ_ASSERT(DisplayStart < 0);

    AZ_ASSERT(item_begin <= item_end);
    if (item_begin < item_end)
        data->Ranges.push_back(
                AzListClipperRange::FromIndices(item_begin, item_end));
}

void AzListClipper::SeekCursorForItem(int item_n)
{
    float pos_y = (float)((double)StartPosY + StartSeekOffsetY
                          + (double)item_n * ItemsHeight);
    AzListClipper_SeekCursorAndSetupPrevLine(pos_y, ItemsHeight);
}

static bool AzListClipper_StepInternal(AzListClipper* clipper)
{
    AzContext& g = *clipper->Ctx;
    AzWindow* window = g.CurrentWindow;
    AzListClipperData* data = (AzListClipperData*)clipper->TempData;
    AZ_ASSERT(data != NULL && "Called AzListClipper::Step() too many times, or before AzListClipper::Begin() ?");

    AzTable* table = g.CurrentTable;
    if (table && table->IsInsideRow)
        Az::TableEndRow(table);

    if (clipper->ItemsCount == 0 || GetSkipItemForListClipping())
        return false;

    if (data->StepNo == 0 && table != NULL && !table->IsUnfrozenRows)
    {
        clipper->DisplayStart = data->ItemsFrozen;
        clipper->DisplayEnd = AzMin(data->ItemsFrozen + 1, clipper->ItemsCount);
        if (clipper->DisplayStart < clipper->DisplayEnd)
            data->ItemsFrozen++;
        return true;
    }

    bool calc_clipping = false;
    if (data->StepNo == 0)
    {
        clipper->StartPosY = window->DC.CursorPos.y;
        if (clipper->ItemsHeight <= 0.0f)
        {
            data->Ranges.push_front(
                    AzListClipperRange::FromIndices(data->ItemsFrozen,
                                                    data->ItemsFrozen + 1));
            clipper->DisplayStart = AzMax(data->Ranges[0].Min,
                                          data->ItemsFrozen);
            clipper->DisplayEnd = AzMin(data->Ranges[0].Max,
                                        clipper->ItemsCount);
            data->StepNo = 1;
            return true;
        }
        calc_clipping = true;
    }

    if (clipper->ItemsHeight <= 0.0f)
    {
        AZ_ASSERT(data->StepNo == 1);
        if (table)
            AZ_ASSERT(table->RowPosY1 == clipper->StartPosY
                      && table->RowPosY2 == window->DC.CursorPos.y);

        bool affected_by_floating_point_precision =
                AzIsFloatAboveGuaranteedIntegerPrecision(
                        (float)clipper->StartPosY)
                || AzIsFloatAboveGuaranteedIntegerPrecision(
                        window->DC.CursorPos.y);
        if (affected_by_floating_point_precision)
        {
            clipper->ItemsHeight = window->DC.PrevLineSize.y
                                   + g.Style.ItemSpacing.y;

            window->DC.CursorPos.y = (float)(clipper->StartPosY
                                             + clipper->ItemsHeight);
        } else
        {
            clipper->ItemsHeight =
                    (float)(window->DC.CursorPos.y - clipper->StartPosY)
                    / (float)(clipper->DisplayEnd - clipper->DisplayStart);
        }
        if (clipper->ItemsHeight == 0.0f && clipper->ItemsCount == INT_MAX)

            return false;
        AZ_ASSERT(clipper->ItemsHeight > 0.0f && "Unable to calculate item height! First item hasn't moved the cursor vertically!");
        calc_clipping = true;
    }

    const int already_submitted = clipper->DisplayEnd;
    if (calc_clipping)
    {
        clipper->StartSeekOffsetY = (double)data->LossynessOffset
                                    - data->ItemsFrozen
                                              * (double)clipper->ItemsHeight;

        if (g.LogEnabled)
        {
            data->Ranges.push_back(
                    AzListClipperRange::FromIndices(0, clipper->ItemsCount));
        } else
        {
            const bool is_nav_request =
                    (g.NavMoveScoringItems && g.NavWindow
                     && g.NavWindow->RootWindowForNav
                                == window->RootWindowForNav);
            if (is_nav_request)
            {
                data->Ranges.push_back(AzListClipperRange::FromPositions(
                        g.NavScoringRect.Min.y,
                        g.NavScoringRect.Max.y,
                        0,
                        0));
                data->Ranges.push_back(AzListClipperRange::FromPositions(
                        g.NavScoringNoClipRect.Min.y,
                        g.NavScoringNoClipRect.Max.y,
                        0,
                        0));
            }
            if (is_nav_request && (g.NavMoveFlags & AzNavMoveFlags_IsTabbing)
                && g.NavTabbingDir == -1)
                data->Ranges.push_back(
                        AzListClipperRange::FromIndices(clipper->ItemsCount - 1,
                                                        clipper->ItemsCount));

            AzRect nav_rect_abs = Az::WindowRectRelToAbs(window,
                                                         window->NavRectRel[0]);
            if (g.NavId != 0 && window->NavLastIds[0] == g.NavId)
                data->Ranges.push_back(
                        AzListClipperRange::FromPositions(nav_rect_abs.Min.y,
                                                          nav_rect_abs.Max.y,
                                                          0,
                                                          0));

            float min_y = window->ClipRect.Min.y;
            float max_y = window->ClipRect.Max.y;

            AzBoxSelectState* bs = &g.BoxSelectState;
            if (bs->IsActive && bs->Window == window)
            {
                min_y -= g.Style.ItemSpacing.y;
                max_y += g.Style.ItemSpacing.y;

                if (bs->UnclipMode)
                    data->Ranges.push_back(AzListClipperRange::FromPositions(
                            bs->UnclipRect.Min.y,
                            bs->UnclipRect.Max.y,
                            0,
                            0));
            }

            const int off_min = (is_nav_request && g.NavMoveClipDir == AzDir_Up)
                                        ? -1
                                        : 0;
            const int off_max = (is_nav_request
                                 && g.NavMoveClipDir == AzDir_Down)
                                        ? 1
                                        : 0;
            data->Ranges.push_back(AzListClipperRange::FromPositions(min_y,
                                                                     max_y,
                                                                     off_min,
                                                                     off_max));
        }

        for (AzListClipperRange& range : data->Ranges)
            if (range.PosToIndexConvert)
            {
                int m1 = (int)(((double)range.Min - window->DC.CursorPos.y
                                - data->LossynessOffset)
                               / clipper->ItemsHeight);
                int m2 = (int)((((double)range.Max - window->DC.CursorPos.y
                                 - data->LossynessOffset)
                                / clipper->ItemsHeight)
                               + 0.999999f);
                range.Min = AzClamp(already_submitted + m1
                                            + range.PosToIndexOffsetMin,
                                    already_submitted,
                                    clipper->ItemsCount - 1);
                range.Max = AzClamp(already_submitted + m2
                                            + range.PosToIndexOffsetMax,
                                    range.Min + 1,
                                    clipper->ItemsCount);
                range.PosToIndexConvert = false;
            }
        AzListClipper_SortAndFuseRanges(data->Ranges, data->StepNo);
    }

    while (data->StepNo < data->Ranges.Size)
    {
        clipper->DisplayStart = AzMax(data->Ranges[data->StepNo].Min,
                                      already_submitted);
        clipper->DisplayEnd = AzMin(data->Ranges[data->StepNo].Max,
                                    clipper->ItemsCount);
        data->StepNo++;
        if (clipper->DisplayStart >= clipper->DisplayEnd)
            continue;
        if (clipper->DisplayStart > already_submitted)
            clipper->SeekCursorForItem(clipper->DisplayStart);
        return true;
    }

    if (clipper->ItemsCount < INT_MAX)
        clipper->SeekCursorForItem(clipper->ItemsCount);

    return false;
}

bool AzListClipper::Step()
{
    AzContext& g = *Ctx;
    bool need_items_height = (ItemsHeight <= 0.0f);
    bool ret = AzListClipper_StepInternal(this);
    if (ret && (DisplayStart >= DisplayEnd))
        ret = false;
    if (g.CurrentTable && g.CurrentTable->IsUnfrozenRows == false)
        AZ_DEBUG_LOG_CLIPPER("Clipper: Step(): inside frozen table row.\n");
    if (need_items_height && ItemsHeight > 0.0f)
        AZ_DEBUG_LOG_CLIPPER("Clipper: Step(): computed ItemsHeight: %.2f.\n",
                             ItemsHeight);
    if (ret)
    {
        AZ_DEBUG_LOG_CLIPPER("Clipper: Step(): display %d to %d.\n",
                             DisplayStart,
                             DisplayEnd);
    } else
    {
        AZ_DEBUG_LOG_CLIPPER("Clipper: Step(): End.\n");
        End();
    }
    return ret;
}

AzStyle& Az::GetStyle()
{
    AZ_ASSERT(GAz != NULL && "No current context. Did you call Az::CreateContext() and Az::SetCurrentContext() ?");
    return GAz->Style;
}

AzU32 Az::GetColorU32(AzCol idx, float alpha_mul)
{
    AzStyle& style = GAz->Style;
    AzVec4 c = style.Colors[idx];
    c.w *= style.Alpha * alpha_mul;
    return ColorConvertFloat4ToU32(c);
}

AzU32 Az::GetColorU32(const AzVec4& col)
{
    AzStyle& style = GAz->Style;
    AzVec4 c = col;
    c.w *= style.Alpha;
    return ColorConvertFloat4ToU32(c);
}

const AzVec4& Az::GetStyleColorVec4(AzCol idx)
{
    AzStyle& style = GAz->Style;
    return style.Colors[idx];
}

AzU32 Az::GetColorU32(AzU32 col, float alpha_mul)
{
    AzStyle& style = GAz->Style;
    alpha_mul *= style.Alpha;
    if (alpha_mul >= 1.0f)
        return col;
    AzU32 a = (col & AZ_COL32_A_MASK) >> AZ_COL32_A_SHIFT;
    a = (AzU32)(a * alpha_mul);

    return (col & ~AZ_COL32_A_MASK) | (a << AZ_COL32_A_SHIFT);
}

void Az::PushStyleColor(AzCol idx, AzU32 col)
{
    AzContext& g = *GAz;
    AzColorMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorStack.push_back(backup);
    if (g.DebugFlashStyleColorIdx != idx)
        g.Style.Colors[idx] = ColorConvertU32ToFloat4(col);
}

void Az::PushStyleColor(AzCol idx, const AzVec4& col)
{
    AzContext& g = *GAz;
    AzColorMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorStack.push_back(backup);
    if (g.DebugFlashStyleColorIdx != idx)
        g.Style.Colors[idx] = col;
}

void Az::PopStyleColor(int count)
{
    AzContext& g = *GAz;
    if (g.ColorStack.Size < count)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopStyleColor() too many times!");
        count = g.ColorStack.Size;
    }
    while (count > 0)
    {
        AzColorMod& backup = g.ColorStack.back();
        g.Style.Colors[backup.Col] = backup.BackupValue;
        g.ColorStack.pop_back();
        count--;
    }
}

static const AzCol GWindowDockStyleColors[AzWindowDockStyleCol_COUNT] = {
        AzCol_Text,
        AzCol_TabHovered,
        AzCol_Tab,
        AzCol_TabSelected,
        AzCol_TabSelectedOverline,
        AzCol_TabDimmed,
        AzCol_TabDimmedSelected,
        AzCol_TabDimmedSelectedOverline,
};

static const AzStyleVarInfo GStyleVarsInfo[] = {
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, Alpha)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, DisabledAlpha)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, WindowPadding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, WindowRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, WindowBorderSize)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, WindowMinSize)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, WindowTitleAlign)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, ChildRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, ChildBorderSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, PopupRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, PopupBorderSize)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, FramePadding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, FrameRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, FrameBorderSize)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, ItemSpacing)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, ItemInnerSpacing)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, IndentSpacing)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, CellPadding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, ScrollbarSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, ScrollbarRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, GrabMinSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, GrabRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, AzageBorderSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, TabRounding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, TabBorderSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, TabBarBorderSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, TabBarOverlineSize)},
        {1,
         AzDataType_Float,
         (AzU32)offsetof(AzStyle, TableAngledHeadersAngle)},
        {2,
         AzDataType_Float,
         (AzU32)offsetof(AzStyle, TableAngledHeadersTextAlign)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, TreeLinesSize)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, TreeLinesRounding)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, ButtonTextAlign)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, SelectableTextAlign)},
        {1,
         AzDataType_Float,
         (AzU32)offsetof(AzStyle, SeparatorTextBorderSize)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, SeparatorTextAlign)},
        {2, AzDataType_Float, (AzU32)offsetof(AzStyle, SeparatorTextPadding)},
        {1, AzDataType_Float, (AzU32)offsetof(AzStyle, DockingSeparatorSize)},
};

const AzStyleVarInfo* Az::GetStyleVarInfo(AzStyleVar idx)
{
    AZ_ASSERT(idx >= 0 && idx < AzStyleVar_COUNT);
    AZ_STATIC_ASSERT(AZ_ARRAYSIZE(GStyleVarsInfo) == AzStyleVar_COUNT);
    return &GStyleVarsInfo[idx];
}

void Az::PushStyleVar(AzStyleVar idx, float val)
{
    AzContext& g = *GAz;
    const AzStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->DataType != AzDataType_Float || var_info->Count != 1)
    {
        AZ_ASSERT_USER_ERROR(0,
                             "Calling PushStyleVar() variant with wrong type!");
        return;
    }
    float* pvar = (float*)var_info->GetVarPtr(&g.Style);
    g.StyleVarStack.push_back(AzStyleMod(idx, *pvar));
    *pvar = val;
}

void Az::PushStyleVarX(AzStyleVar idx, float val_x)
{
    AzContext& g = *GAz;
    const AzStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->DataType != AzDataType_Float || var_info->Count != 2)
    {
        AZ_ASSERT_USER_ERROR(0,
                             "Calling PushStyleVar() variant with wrong type!");
        return;
    }
    AzVec2* pvar = (AzVec2*)var_info->GetVarPtr(&g.Style);
    g.StyleVarStack.push_back(AzStyleMod(idx, *pvar));
    pvar->x = val_x;
}

void Az::PushStyleVarY(AzStyleVar idx, float val_y)
{
    AzContext& g = *GAz;
    const AzStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->DataType != AzDataType_Float || var_info->Count != 2)
    {
        AZ_ASSERT_USER_ERROR(0,
                             "Calling PushStyleVar() variant with wrong type!");
        return;
    }
    AzVec2* pvar = (AzVec2*)var_info->GetVarPtr(&g.Style);
    g.StyleVarStack.push_back(AzStyleMod(idx, *pvar));
    pvar->y = val_y;
}

void Az::PushStyleVar(AzStyleVar idx, const AzVec2& val)
{
    AzContext& g = *GAz;
    const AzStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->DataType != AzDataType_Float || var_info->Count != 2)
    {
        AZ_ASSERT_USER_ERROR(0,
                             "Calling PushStyleVar() variant with wrong type!");
        return;
    }
    AzVec2* pvar = (AzVec2*)var_info->GetVarPtr(&g.Style);
    g.StyleVarStack.push_back(AzStyleMod(idx, *pvar));
    *pvar = val;
}

void Az::PopStyleVar(int count)
{
    AzContext& g = *GAz;
    if (g.StyleVarStack.Size < count)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopStyleVar() too many times!");
        count = g.StyleVarStack.Size;
    }
    while (count > 0)
    {
        AzStyleMod& backup = g.StyleVarStack.back();
        const AzStyleVarInfo* var_info = GetStyleVarInfo(backup.VarIdx);
        void* data = var_info->GetVarPtr(&g.Style);
        if (var_info->DataType == AzDataType_Float && var_info->Count == 1)
        {
            ((float*)data)[0] = backup.BackupFloat[0];
        } else if (var_info->DataType == AzDataType_Float
                   && var_info->Count == 2)
        {
            ((float*)data)[0] = backup.BackupFloat[0];
            ((float*)data)[1] = backup.BackupFloat[1];
        }
        g.StyleVarStack.pop_back();
        count--;
    }
}

const char* Az::GetStyleColorName(AzCol idx)
{
    switch (idx)
    {
    case AzCol_Text:
        return "Text";
    case AzCol_TextDisabled:
        return "TextDisabled";
    case AzCol_WindowBg:
        return "WindowBg";
    case AzCol_ChildBg:
        return "ChildBg";
    case AzCol_PopupBg:
        return "PopupBg";
    case AzCol_Border:
        return "Border";
    case AzCol_BorderShadow:
        return "BorderShadow";
    case AzCol_FrameBg:
        return "FrameBg";
    case AzCol_FrameBgHovered:
        return "FrameBgHovered";
    case AzCol_FrameBgActive:
        return "FrameBgActive";
    case AzCol_TitleBg:
        return "TitleBg";
    case AzCol_TitleBgActive:
        return "TitleBgActive";
    case AzCol_TitleBgCollapsed:
        return "TitleBgCollapsed";
    case AzCol_MenuBarBg:
        return "MenuBarBg";
    case AzCol_ScrollbarBg:
        return "ScrollbarBg";
    case AzCol_ScrollbarGrab:
        return "ScrollbarGrab";
    case AzCol_ScrollbarGrabHovered:
        return "ScrollbarGrabHovered";
    case AzCol_ScrollbarGrabActive:
        return "ScrollbarGrabActive";
    case AzCol_CheckMark:
        return "CheckMark";
    case AzCol_SliderGrab:
        return "SliderGrab";
    case AzCol_SliderGrabActive:
        return "SliderGrabActive";
    case AzCol_Button:
        return "Button";
    case AzCol_ButtonHovered:
        return "ButtonHovered";
    case AzCol_ButtonActive:
        return "ButtonActive";
    case AzCol_Header:
        return "Header";
    case AzCol_HeaderHovered:
        return "HeaderHovered";
    case AzCol_HeaderActive:
        return "HeaderActive";
    case AzCol_Separator:
        return "Separator";
    case AzCol_SeparatorHovered:
        return "SeparatorHovered";
    case AzCol_SeparatorActive:
        return "SeparatorActive";
    case AzCol_ResizeGrip:
        return "ResizeGrip";
    case AzCol_ResizeGripHovered:
        return "ResizeGripHovered";
    case AzCol_ResizeGripActive:
        return "ResizeGripActive";
    case AzCol_InputTextCursor:
        return "InputTextCursor";
    case AzCol_TabHovered:
        return "TabHovered";
    case AzCol_Tab:
        return "Tab";
    case AzCol_TabSelected:
        return "TabSelected";
    case AzCol_TabSelectedOverline:
        return "TabSelectedOverline";
    case AzCol_TabDimmed:
        return "TabDimmed";
    case AzCol_TabDimmedSelected:
        return "TabDimmedSelected";
    case AzCol_TabDimmedSelectedOverline:
        return "TabDimmedSelectedOverline";
    case AzCol_DockingPreview:
        return "DockingPreview";
    case AzCol_DockingEmptyBg:
        return "DockingEmptyBg";
    case AzCol_PlotLines:
        return "PlotLines";
    case AzCol_PlotLinesHovered:
        return "PlotLinesHovered";
    case AzCol_PlotHistogram:
        return "PlotHistogram";
    case AzCol_PlotHistogramHovered:
        return "PlotHistogramHovered";
    case AzCol_TableHeaderBg:
        return "TableHeaderBg";
    case AzCol_TableBorderStrong:
        return "TableBorderStrong";
    case AzCol_TableBorderLight:
        return "TableBorderLight";
    case AzCol_TableRowBg:
        return "TableRowBg";
    case AzCol_TableRowBgAlt:
        return "TableRowBgAlt";
    case AzCol_TextLink:
        return "TextLink";
    case AzCol_TextSelectedBg:
        return "TextSelectedBg";
    case AzCol_TreeLines:
        return "TreeLines";
    case AzCol_DragDropTarget:
        return "DragDropTarget";
    case AzCol_NavCursor:
        return "NavCursor";
    case AzCol_NavWindowingHighlight:
        return "NavWindowingHighlight";
    case AzCol_NavWindowingDimBg:
        return "NavWindowingDimBg";
    case AzCol_ModalWindowDimBg:
        return "ModalWindowDimBg";
    }
    AZ_ASSERT(0);
    return "Unknown";
}

const char* Az::FindRenderedTextEnd(const char* text, const char* text_end)
{
    const char* text_display_end = text;
    if (!text_end)
        text_end = (const char*)-1;

    while (text_display_end < text_end && *text_display_end != '\0'
           && (text_display_end[0] != '#' || text_display_end[1] != '#'))
        text_display_end++;
    return text_display_end;
}

void Az::RenderText(AzVec2 pos,
                    const char* text,
                    const char* text_end,
                    bool hide_text_after_hash)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    const char* text_display_end;
    if (hide_text_after_hash)
    {
        text_display_end = FindRenderedTextEnd(text, text_end);
    } else
    {
        if (!text_end)
            text_end = text + AzStrlen(text);
        text_display_end = text_end;
    }

    if (text != text_display_end)
    {
        window->DrawList->AddText(g.Font,
                                  g.FontSize,
                                  pos,
                                  GetColorU32(AzCol_Text),
                                  text,
                                  text_display_end);
        if (g.LogEnabled)
            LogRenderedText(&pos, text, text_display_end);
    }
}

void Az::RenderTextWrapped(AzVec2 pos,
                           const char* text,
                           const char* text_end,
                           float wrap_width)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    if (!text_end)
        text_end = text + AzStrlen(text);

    if (text != text_end)
    {
        window->DrawList->AddText(g.Font,
                                  g.FontSize,
                                  pos,
                                  GetColorU32(AzCol_Text),
                                  text,
                                  text_end,
                                  wrap_width);
        if (g.LogEnabled)
            LogRenderedText(&pos, text, text_end);
    }
}

void Az::RenderTextClippedEx(AzDrawList* draw_list,
                             const AzVec2& pos_min,
                             const AzVec2& pos_max,
                             const char* text,
                             const char* text_display_end,
                             const AzVec2* text_size_if_known,
                             const AzVec2& align,
                             const AzRect* clip_rect)
{
    AzVec2 pos = pos_min;
    const AzVec2 text_size = text_size_if_known ? *text_size_if_known
                                                : CalcTextSize(text,
                                                               text_display_end,
                                                               false,
                                                               0.0f);

    const AzVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
    const AzVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
    bool need_clipping = (pos.x + text_size.x >= clip_max->x)
                         || (pos.y + text_size.y >= clip_max->y);
    if (clip_rect)

        need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

    if (align.x > 0.0f)
        pos.x = AzMax(pos.x,
                      pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
    if (align.y > 0.0f)
        pos.y = AzMax(pos.y,
                      pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

    if (need_clipping)
    {
        AzVec4 fine_clip_rect(clip_min->x,
                              clip_min->y,
                              clip_max->x,
                              clip_max->y);
        draw_list->AddText(NULL,
                           0.0f,
                           pos,
                           GetColorU32(AzCol_Text),
                           text,
                           text_display_end,
                           0.0f,
                           &fine_clip_rect);
    } else
    {
        draw_list->AddText(NULL,
                           0.0f,
                           pos,
                           GetColorU32(AzCol_Text),
                           text,
                           text_display_end,
                           0.0f,
                           NULL);
    }
}

void Az::RenderTextClipped(const AzVec2& pos_min,
                           const AzVec2& pos_max,
                           const char* text,
                           const char* text_end,
                           const AzVec2* text_size_if_known,
                           const AzVec2& align,
                           const AzRect* clip_rect)
{
    const char* text_display_end = FindRenderedTextEnd(text, text_end);
    const int text_len = (int)(text_display_end - text);
    if (text_len == 0)
        return;

    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    RenderTextClippedEx(window->DrawList,
                        pos_min,
                        pos_max,
                        text,
                        text_display_end,
                        text_size_if_known,
                        align,
                        clip_rect);
    if (g.LogEnabled)
        LogRenderedText(&pos_min, text, text_display_end);
}

void Az::RenderTextEllipsis(AzDrawList* draw_list,
                            const AzVec2& pos_min,
                            const AzVec2& pos_max,
                            float ellipsis_max_x,
                            const char* text,
                            const char* text_end_full,
                            const AzVec2* text_size_if_known)
{
    AzContext& g = *GAz;
    if (text_end_full == NULL)
        text_end_full = FindRenderedTextEnd(text);
    const AzVec2 text_size =
            text_size_if_known ? *text_size_if_known
                               : CalcTextSize(text, text_end_full, false, 0.0f);

    if (text_size.x > pos_max.x - pos_min.x)
    {
        AzFont* font = draw_list->_Data->Font;
        const float font_size = draw_list->_Data->FontSize;
        const float font_scale = draw_list->_Data->FontScale;
        const char* text_end_ellipsis = NULL;
        AzFontBaked* baked = font->GetFontBaked(font_size);
        const float ellipsis_width = baked->GetCharAdvance(font->EllipsisChar)
                                     * font_scale;

        const float text_avail_width = AzMax(
                (AzMax(pos_max.x, ellipsis_max_x) - ellipsis_width) - pos_min.x,
                1.0f);
        float text_size_clipped_x = font->CalcTextSizeA(font_size,
                                                        text_avail_width,
                                                        0.0f,
                                                        text,
                                                        text_end_full,
                                                        &text_end_ellipsis)
                                            .x;
        while (text_end_ellipsis > text
               && AzCharIsBlankA(text_end_ellipsis[-1]))
        {
            text_end_ellipsis--;
            text_size_clipped_x -= font->CalcTextSizeA(font_size,
                                                       FLT_MAX,
                                                       0.0f,
                                                       text_end_ellipsis,
                                                       text_end_ellipsis + 1)
                                           .x;
        }

        RenderTextClippedEx(draw_list,
                            pos_min,
                            pos_max,
                            text,
                            text_end_ellipsis,
                            &text_size,
                            AzVec2(0.0f, 0.0f));
        AzVec4 cpu_fine_clip_rect(pos_min.x, pos_min.y, pos_max.x, pos_max.y);
        AzVec2 ellipsis_pos = AzTrunc(
                AzVec2(pos_min.x + text_size_clipped_x, pos_min.y));
        font->RenderChar(draw_list,
                         font_size,
                         ellipsis_pos,
                         GetColorU32(AzCol_Text),
                         font->EllipsisChar,
                         &cpu_fine_clip_rect);
    } else
    {
        RenderTextClippedEx(draw_list,
                            pos_min,
                            pos_max,
                            text,
                            text_end_full,
                            &text_size,
                            AzVec2(0.0f, 0.0f));
    }

    if (g.LogEnabled)
        LogRenderedText(&pos_min, text, text_end_full);
}

void Az::RenderFrame(AzVec2 p_min,
                     AzVec2 p_max,
                     AzU32 fill_col,
                     bool borders,
                     float rounding)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
    const float border_size = g.Style.FrameBorderSize;
    if (borders && border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min + AzVec2(1, 1),
                                  p_max + AzVec2(1, 1),
                                  GetColorU32(AzCol_BorderShadow),
                                  rounding,
                                  0,
                                  border_size);
        window->DrawList->AddRect(p_min,
                                  p_max,
                                  GetColorU32(AzCol_Border),
                                  rounding,
                                  0,
                                  border_size);
    }
}

void Az::RenderFrameBorder(AzVec2 p_min, AzVec2 p_max, float rounding)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    const float border_size = g.Style.FrameBorderSize;
    if (border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min + AzVec2(1, 1),
                                  p_max + AzVec2(1, 1),
                                  GetColorU32(AzCol_BorderShadow),
                                  rounding,
                                  0,
                                  border_size);
        window->DrawList->AddRect(p_min,
                                  p_max,
                                  GetColorU32(AzCol_Border),
                                  rounding,
                                  0,
                                  border_size);
    }
}

void Az::RenderNavCursor(const AzRect& bb,
                         AzID id,
                         AzNavRenderCursorFlags flags)
{
    AzContext& g = *GAz;
    if (id != g.NavId)
        return;
    if (!g.NavCursorVisible && !(flags & AzNavRenderCursorFlags_AlwaysDraw))
        return;
    if (id == g.LastItemData.ID
        && (g.LastItemData.ItemFlags & AzItemFlags_NoNav))
        return;
    AzWindow* window = g.CurrentWindow;
    if (window->DC.NavHideHighlightOneFrame)
        return;

    float rounding = (flags & AzNavRenderCursorFlags_NoRounding)
                             ? 0.0f
                             : g.Style.FrameRounding;
    AzRect display_rect = bb;
    display_rect.ClipWith(window->ClipRect);
    const float thickness = 2.0f;
    if (flags & AzNavRenderCursorFlags_Compact)
    {
        window->DrawList->AddRect(display_rect.Min,
                                  display_rect.Max,
                                  GetColorU32(AzCol_NavCursor),
                                  rounding,
                                  0,
                                  thickness);
    } else
    {
        const float distance = 3.0f + thickness * 0.5f;
        display_rect.Expand(AzVec2(distance, distance));
        bool fully_visible = window->ClipRect.Contains(display_rect);
        if (!fully_visible)
            window->DrawList->PushClipRect(display_rect.Min, display_rect.Max);
        window->DrawList->AddRect(display_rect.Min,
                                  display_rect.Max,
                                  GetColorU32(AzCol_NavCursor),
                                  rounding,
                                  0,
                                  thickness);
        if (!fully_visible)
            window->DrawList->PopClipRect();
    }
}

void Az::RenderMouseCursor(AzVec2 base_pos,
                           float base_scale,
                           AzMouseCursor mouse_cursor,
                           AzU32 col_fill,
                           AzU32 col_border,
                           AzU32 col_shadow)
{
    AzContext& g = *GAz;
    if (mouse_cursor <= AzMouseCursor_None
        || mouse_cursor >= AzMouseCursor_COUNT)

        mouse_cursor = AzMouseCursor_Arrow;
    AzFontAtlas* font_atlas = g.DrawListSharedData.Font->ContainerAtlas;
    for (AzViewportP* viewport : g.Viewports)
    {
        AzVec2 offset, size, uv[4];
        if (!AzFontAtlasGetMouseCursorTexData(font_atlas,
                                              mouse_cursor,
                                              &offset,
                                              &size,
                                              &uv[0],
                                              &uv[2]))
            continue;
        const AzVec2 pos = base_pos - offset;
        const float scale = base_scale * viewport->DpiScale;
        if (!viewport->GetMainRect().Overlaps(
                    AzRect(pos, pos + AzVec2(size.x + 2, size.y + 2) * scale)))
            continue;
        AzDrawList* draw_list = GetForegroundDrawList(viewport);
        AzTextureRef tex_ref = font_atlas->TexRef;
        draw_list->PushTexture(tex_ref);
        draw_list->AddAzage(tex_ref,
                            pos + AzVec2(1, 0) * scale,
                            pos + (AzVec2(1, 0) + size) * scale,
                            uv[2],
                            uv[3],
                            col_shadow);
        draw_list->AddAzage(tex_ref,
                            pos + AzVec2(2, 0) * scale,
                            pos + (AzVec2(2, 0) + size) * scale,
                            uv[2],
                            uv[3],
                            col_shadow);
        draw_list->AddAzage(tex_ref,
                            pos,
                            pos + size * scale,
                            uv[2],
                            uv[3],
                            col_border);
        draw_list->AddAzage(tex_ref,
                            pos,
                            pos + size * scale,
                            uv[0],
                            uv[1],
                            col_fill);
        if (mouse_cursor == AzMouseCursor_Wait
            || mouse_cursor == AzMouseCursor_Progress)
        {
            float a_min = AzFmod((float)g.Time * 5.0f, 2.0f * AZ_PI);
            float a_max = a_min + AZ_PI * 1.65f;
            draw_list->PathArcTo(pos + AzVec2(14, -1) * scale,
                                 6.0f * scale,
                                 a_min,
                                 a_max);
            draw_list->PathStroke(col_fill, AzDrawFlags_None, 3.0f * scale);
        }
        draw_list->PopTexture();
    }
}

AzContext* Az::GetCurrentContext()
{
    return GAz;
}

void Az::SetCurrentContext(AzContext* ctx)
{
#ifdef AZ_SET_CURRENT_CONTEXT_FUNC
    AZ_SET_CURRENT_CONTEXT_FUNC(ctx);

#else
    GAz = ctx;
#endif
}

void Az::SetAllocatorFunctions(AzMemAllocFunc alloc_func,
                               AzMemFreeFunc free_func,
                               void* user_data)
{
    GAzAllocatorAllocFunc = alloc_func;
    GAzAllocatorFreeFunc = free_func;
    GAzAllocatorUserData = user_data;
}

void Az::GetAllocatorFunctions(AzMemAllocFunc* p_alloc_func,
                               AzMemFreeFunc* p_free_func,
                               void** p_user_data)
{
    *p_alloc_func = GAzAllocatorAllocFunc;
    *p_free_func = GAzAllocatorFreeFunc;
    *p_user_data = GAzAllocatorUserData;
}

AzContext* Az::CreateContext(AzFontAtlas* shared_font_atlas)
{
    AzContext* prev_ctx = GetCurrentContext();
    AzContext* ctx = AZ_NEW(AzContext)(shared_font_atlas);
    SetCurrentContext(ctx);
    Initialize();
    if (prev_ctx != NULL)
        SetCurrentContext(prev_ctx);

    return ctx;
}

void Az::DestroyContext(AzContext* ctx)
{
    AzContext* prev_ctx = GetCurrentContext();
    if (ctx == NULL)
        ctx = prev_ctx;
    SetCurrentContext(ctx);
    Shutdown();
    SetCurrentContext((prev_ctx != ctx) ? prev_ctx : NULL);
    AZ_DELETE(ctx);
}

static const AzLocEntry GLocalizationEntriesEnUS[] = {
        {AzLocKey_VersionStr,
         "Dear Az " AZ_VERSION " (" AZ_STRINGIFY(AZ_VERSION_NUM) ")"},
        {AzLocKey_TableSizeOne, "Size column to fit###SizeOne"},
        {AzLocKey_TableSizeAllFit, "Size all columns to fit###SizeAll"},
        {AzLocKey_TableSizeAllDefault, "Size all columns to default###SizeAll"},
        {AzLocKey_TableResetOrder, "Reset order###ResetOrder"},
        {AzLocKey_WindowingMainMenuBar, "(Main menu bar)"},
        {AzLocKey_WindowingPopup, "(Popup)"},
        {AzLocKey_WindowingUntitled, "(Untitled)"},
        {AzLocKey_OpenLink_s, "Open '%s'"},
        {AzLocKey_CopyLink, "Copy Link###CopyLink"},
        {AzLocKey_DockingHideTabBar, "Hide tab bar###HideTabBar"},
        {AzLocKey_DockingHoldShiftToDock,
         "Hold SHIFT to enable Docking window."},
        {AzLocKey_DockingDragToUndockOrMoveNode,
         "Click and drag to move or undock whole node."},
};

AzContext::AzContext(AzFontAtlas* shared_font_atlas)
{
    IO.Ctx = this;
    InputTextState.Ctx = this;

    Initialized = false;
    ConfigFlagsCurrFrame = ConfigFlagsLastFrame = AzConfigFlags_None;

    Font = NULL;
    FontBaked = NULL;
    FontSize = FontSizeBase = FontBakedScale = CurrentDpiScale = 0.0f;
    FontRasterizerDensity = 1.0f;
    IO.Fonts = shared_font_atlas ? shared_font_atlas : AZ_NEW(AzFontAtlas)();
    if (shared_font_atlas == NULL)
        IO.Fonts->OwnerContext = this;
    Time = 0.0f;
    FrameCount = 0;
    FrameCountEnded = FrameCountPlatformEnded = FrameCountRendered = -1;
    WithinEndChildID = 0;
    WithinFrameScope = WithinFrameScopeWithAzplicitWindow = false;
    GcCompactAll = false;
    TestEngineHookItems = false;
    TestEngine = NULL;
    memset(ContextName, 0, sizeof(ContextName));

    InputEventsNextMouseSource = AzMouseSource_Mouse;
    InputEventsNextEventId = 1;

    WindowsActiveCount = 0;
    WindowsBorderHoverPadding = 0.0f;
    CurrentWindow = NULL;
    HoveredWindow = NULL;
    HoveredWindowUnderMovingWindow = NULL;
    HoveredWindowBeforeClear = NULL;
    MovingWindow = NULL;
    WheelingWindow = NULL;
    WheelingWindowStartFrame = WheelingWindowScrolledFrame = -1;
    WheelingWindowReleaseTimer = 0.0f;

    DebugDrawIdConflicts = 0;
    DebugHookIdInfo = 0;
    HoveredId = HoveredIdPreviousFrame = 0;
    HoveredIdPreviousFrameItemCount = 0;
    HoveredIdAllowOverlap = false;
    HoveredIdIsDisabled = false;
    HoveredIdTimer = HoveredIdNotActiveTimer = 0.0f;
    ItemUnclipByLog = false;
    ActiveId = 0;
    ActiveIdIsAlive = 0;
    ActiveIdTimer = 0.0f;
    ActiveIdIsJustActivated = false;
    ActiveIdAllowOverlap = false;
    ActiveIdNoClearOnFocusLoss = false;
    ActiveIdHasBeenPressedBefore = false;
    ActiveIdHasBeenEditedBefore = false;
    ActiveIdHasBeenEditedThisFrame = false;
    ActiveIdFromShortcut = false;
    ActiveIdClickOffset = AzVec2(-1, -1);
    ActiveIdWindow = NULL;
    ActiveIdSource = AzInputSource_None;
    ActiveIdMouseButton = -1;
    ActiveIdPreviousFrame = 0;
    memset(&DeactivatedItemData, 0, sizeof(DeactivatedItemData));
    memset(&ActiveIdValueOnActivation, 0, sizeof(ActiveIdValueOnActivation));
    LastActiveId = 0;
    LastActiveIdTimer = 0.0f;

    LastKeyboardKeyPressTime = LastKeyModsChangeTime =
            LastKeyModsChangeFromNoneTime = -1.0;

    ActiveIdUsingNavDirMask = 0x00;
    ActiveIdUsingAllKeyboardKeys = false;

    CurrentFocusScopeId = 0;
    CurrentItemFlags = AzItemFlags_None;
    DebugShowGroupRects = false;

    CurrentViewport = NULL;
    MouseViewport = MouseLastHoveredViewport = NULL;
    PlatformLastFocusedViewportId = 0;
    ViewportCreatedCount = PlatformWindowsCreatedCount = 0;
    ViewportFocusedStampCount = 0;

    NavCursorVisible = false;
    NavHighlightItemUnderNav = false;
    NavMousePosDirty = false;
    NavIdIsAlive = false;
    NavId = 0;
    NavWindow = NULL;
    NavFocusScopeId = NavActivateId = NavActivateDownId = NavActivatePressedId =
            0;
    NavLayer = AzNavLayer_Main;
    NavNextActivateId = 0;
    NavActivateFlags = NavNextActivateFlags = AzActivateFlags_None;
    NavHighlightActivatedId = 0;
    NavHighlightActivatedTimer = 0.0f;
    NavInputSource = AzInputSource_Keyboard;
    NavLastValidSelectionUserData = AzSelectionUserData_Invalid;
    NavCursorHideFrames = 0;

    NavAnyRequest = false;
    NavInitRequest = false;
    NavInitRequestFromMove = false;
    NavMoveSubmitted = false;
    NavMoveScoringItems = false;
    NavMoveForwardToNextFrame = false;
    NavMoveFlags = AzNavMoveFlags_None;
    NavMoveScrollFlags = AzScrollFlags_None;
    NavMoveKeyMods = AzMod_None;
    NavMoveDir = NavMoveDirForDebug = NavMoveClipDir = AzDir_None;
    NavScoringDebugCount = 0;
    NavTabbingDir = 0;
    NavTabbingCounter = 0;

    NavJustMovedFromFocusScopeId = NavJustMovedToId =
            NavJustMovedToFocusScopeId = 0;
    NavJustMovedToKeyMods = AzMod_None;
    NavJustMovedToIsTabbing = false;
    NavJustMovedToHasSelectionData = false;

    ConfigNavWindowingWithGamepad = true;
    ConfigNavWindowingKeyNext = IO.ConfigMacOSXBehaviors
                                        ? (AzMod_Super | AzKey_Tab)
                                        : (AzMod_Ctrl | AzKey_Tab);
    ConfigNavWindowingKeyPrev =
            IO.ConfigMacOSXBehaviors ? (AzMod_Super | AzMod_Shift | AzKey_Tab)
                                     : (AzMod_Ctrl | AzMod_Shift | AzKey_Tab);
    NavWindowingTarget = NavWindowingTargetAnim = NavWindowingListWindow = NULL;
    NavWindowingInputSource = AzInputSource_None;
    NavWindowingTimer = NavWindowingHighlightAlpha = 0.0f;
    NavWindowingToggleLayer = false;
    NavWindowingToggleKey = AzKey_None;

    DimBgRatio = 0.0f;

    DragDropActive = DragDropWithinSource = DragDropWithinTarget = false;
    DragDropSourceFlags = AzDragDropFlags_None;
    DragDropSourceFrameCount = -1;
    DragDropMouseButton = -1;
    DragDropTargetId = 0;
    DragDropAcceptFlags = AzDragDropFlags_None;
    DragDropAcceptIdCurrRectSurface = 0.0f;
    DragDropAcceptIdPrev = DragDropAcceptIdCurr = 0;
    DragDropAcceptFrameCount = -1;
    DragDropHoldJustPressedId = 0;
    memset(DragDropPayloadBufLocal, 0, sizeof(DragDropPayloadBufLocal));

    ClipperTempDataStacked = 0;

    CurrentTable = NULL;
    TablesTempDataStacked = 0;
    CurrentTabBar = NULL;
    CurrentMultiSelect = NULL;
    MultiSelectTempDataStacked = 0;

    HoverItemDelayId = HoverItemDelayIdPreviousFrame =
            HoverItemUnlockedStationaryId = HoverWindowUnlockedStationaryId = 0;
    HoverItemDelayTimer = HoverItemDelayClearTimer = 0.0f;

    MouseCursor = AzMouseCursor_Arrow;
    MouseStationaryTimer = 0.0f;

    InputTextPasswordFontBackupFlags = AzFontFlags_None;
    TempInputId = 0;
    memset(&DataTypeZeroValue, 0, sizeof(DataTypeZeroValue));
    BeginMenuDepth = BeginComboDepth = 0;
    ColorEditOptions = AzColorEditFlags_DefaultOptions_;
    ColorEditCurrentID = ColorEditSavedID = 0;
    ColorEditSavedHue = ColorEditSavedSat = 0.0f;
    ColorEditSavedColor = 0;
    WindowResizeRelativeMode = false;
    ScrollbarSeekMode = 0;
    ScrollbarClickDeltaToGrabCenter = 0.0f;
    SliderGrabClickOffset = 0.0f;
    SliderCurrentAccum = 0.0f;
    SliderCurrentAccumDirty = false;
    DragCurrentAccumDirty = false;
    DragCurrentAccum = 0.0f;
    DragSpeedDefaultRatio = 1.0f / 100.0f;
    DisabledAlphaBackup = 0.0f;
    DisabledStackSize = 0;
    TooltipOverrideCount = 0;
    TooltipPreviousWindow = NULL;

    PlatformAzeData.InputPos = AzVec2(0.0f, 0.0f);
    PlatformAzeDataPrev.InputPos = AzVec2(-1.0f, -1.0f);

    DockNodeWindowMenuHandler = NULL;

    SettingsLoaded = false;
    SettingsDirtyTimer = 0.0f;
    HookIdNext = 0;

    memset(LocalizationTable, 0, sizeof(LocalizationTable));

    LogEnabled = false;
    LogFlags = AzLogFlags_None;
    LogWindow = NULL;
    LogNextPrefix = LogNextSuffix = NULL;
    LogFile = NULL;
    LogLinePosY = FLT_MAX;
    LogLineFirstItem = false;
    LogDepthRef = 0;
    LogDepthToExpand = LogDepthToExpandDefault = 2;

    ErrorCallback = NULL;
    ErrorCallbackUserData = NULL;
    ErrorFirst = true;
    ErrorCountCurrentFrame = 0;
    StackSizesInBeginForCurrentWindow = NULL;

    DebugDrawIdConflictsCount = 0;
    DebugLogFlags = AzDebugLogFlags_EventError | AzDebugLogFlags_OutputToTTY;
    DebugLocateId = 0;
    DebugLogSkippedErrors = 0;
    DebugLogAutoDisableFlags = AzDebugLogFlags_None;
    DebugLogAutoDisableFrames = 0;
    DebugLocateFrames = 0;
    DebugBeginReturnValueCullDepth = -1;
    DebugItemPickerActive = false;
    DebugItemPickerMouseButton = AzMouseButton_Left;
    DebugItemPickerBreakId = 0;
    DebugFlashStyleColorTime = 0.0f;
    DebugFlashStyleColorIdx = AzCol_COUNT;
    DebugHoveredDockNode = NULL;

    DebugBreakInWindow = 0;
    DebugBreakInTable = 0;
    DebugBreakInLocateId = false;
    DebugBreakKeyChord = AzKey_Pause;
    DebugBreakInShortcutRouting = AzKey_None;

    memset(FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
    FramerateSecPerFrameIdx = FramerateSecPerFrameCount = 0;
    FramerateSecPerFrameAccum = 0.0f;
    WantCaptureMouseNextFrame = WantCaptureKeyboardNextFrame =
            WantTextInputNextFrame = -1;
    memset(TempKeychordName, 0, sizeof(TempKeychordName));
}

void Az::Initialize()
{
    AzContext& g = *GAz;
    AZ_ASSERT(!g.Initialized && !g.SettingsLoaded);

    {
        AzSettingsHandler ini_handler;
        ini_handler.TypeName = "Window";
        ini_handler.TypeHash = AzHashStr("Window");
        ini_handler.ClearAllFn = WindowSettingsHandler_ClearAll;
        ini_handler.ReadOpenFn = WindowSettingsHandler_ReadOpen;
        ini_handler.ReadLineFn = WindowSettingsHandler_ReadLine;
        ini_handler.ApplyAllFn = WindowSettingsHandler_ApplyAll;
        ini_handler.WriteAllFn = WindowSettingsHandler_WriteAll;
        AddSettingsHandler(&ini_handler);
    }
    TableSettingsAddSettingsHandler();

    LocalizeRegisterEntries(GLocalizationEntriesEnUS,
                            AZ_ARRAYSIZE(GLocalizationEntriesEnUS));

    g.PlatformIO.Platform_GetClipboardTextFn =
            Platform_GetClipboardTextFn_DefaultAzpl;

    g.PlatformIO.Platform_SetClipboardTextFn =
            Platform_SetClipboardTextFn_DefaultAzpl;
    g.PlatformIO.Platform_OpenInShellFn = Platform_OpenInShellFn_DefaultAzpl;
    g.PlatformIO.Platform_SetAzeDataFn = Platform_SetAzeDataFn_DefaultAzpl;

    AzViewportP* viewport = AZ_NEW(AzViewportP)();
    viewport->ID = AZ_VIEWPORT_DEFAULT_ID;
    viewport->Idx = 0;
    viewport->PlatformWindowCreated = true;
    viewport->Flags = AzViewportFlags_OwnedByApp;
    g.Viewports.push_back(viewport);
    g.TempBuffer.resize(1024 * 3 + 1, 0);
    g.ViewportCreatedCount++;
    g.PlatformIO.Viewports.push_back(g.Viewports[0]);

    for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
         key = (AzKey)(key + 1))
        if ((key >= AzKey_0 && key <= AzKey_9)
            || (key >= AzKey_A && key <= AzKey_Z)
            || (key >= AzKey_Keypad0 && key <= AzKey_Keypad9)
            || key == AzKey_Tab || key == AzKey_Space || key == AzKey_Apostrophe
            || key == AzKey_Comma || key == AzKey_Minus || key == AzKey_Period
            || key == AzKey_Slash || key == AzKey_Semicolon
            || key == AzKey_Equal || key == AzKey_LeftBracket
            || key == AzKey_RightBracket || key == AzKey_GraveAccent
            || key == AzKey_KeypadDecimal || key == AzKey_KeypadDivide
            || key == AzKey_KeypadMultiply || key == AzKey_KeypadSubtract
            || key == AzKey_KeypadAdd || key == AzKey_KeypadEqual)
            g.KeysMayBeCharInput.SetBit(key);

#ifdef AZ_HAS_DOCK

    DockContextInitialize(&g);
#endif

    AzFontAtlas* atlas = g.IO.Fonts;
    g.DrawListSharedData.Context = &g;
    RegisterFontAtlas(atlas);

    g.Initialized = true;
}

void Az::Shutdown()
{
    AzContext& g = *GAz;
    AZ_ASSERT_USER_ERROR(g.IO.BackendPlatformUserData == NULL,
                         "Forgot to shutdown Platform backend?");
    AZ_ASSERT_USER_ERROR(g.IO.BackendRendererUserData == NULL,
                         "Forgot to shutdown Renderer backend?");

    for (AzFontAtlas* atlas : g.FontAtlases)
    {
        UnregisterFontAtlas(atlas);
        if (atlas->OwnerContext == &g)
        {
            atlas->Locked = false;
            AZ_DELETE(atlas);
        }
    }
    g.DrawListSharedData.TempBuffer.clear();

    if (!g.Initialized)
        return;

    if (g.SettingsLoaded && g.IO.IniFilename != NULL)
        SaveIniSettingsToDisk(g.IO.IniFilename);

    DestroyPlatformWindows();

    DockContextShutdown(&g);

    CallContextHooks(&g, AzContextHookType_Shutdown);

    g.Windows.clear_delete();
    g.WindowsFocusOrder.clear();
    g.WindowsTempSortBuffer.clear();
    g.CurrentWindow = NULL;
    g.CurrentWindowStack.clear();
    g.WindowsById.Clear();
    g.NavWindow = NULL;
    g.HoveredWindow = g.HoveredWindowUnderMovingWindow = NULL;
    g.ActiveIdWindow = NULL;
    g.MovingWindow = NULL;

    g.KeysRoutingTable.Clear();

    g.ColorStack.clear();
    g.StyleVarStack.clear();
    g.FontStack.clear();
    g.OpenPopupStack.clear();
    g.BeginPopupStack.clear();
    g.TreeNodeStack.clear();

    g.CurrentViewport = g.MouseViewport = g.MouseLastHoveredViewport = NULL;
    g.Viewports.clear_delete();

    g.TabBars.Clear();
    g.CurrentTabBarStack.clear();
    g.ShrinkWidthBuffer.clear();

    g.ClipperTempData.clear_destruct();

    g.Tables.Clear();
    g.TablesTempData.clear_destruct();
    g.DrawChannelsTempMergeBuffer.clear();

    g.MultiSelectStorage.Clear();
    g.MultiSelectTempData.clear_destruct();

    g.ClipboardHandlerData.clear();
    g.MenusIdSubmittedThisFrame.clear();
    g.InputTextState.ClearFreeMemory();
    g.InputTextDeactivatedState.ClearFreeMemory();

    g.SettingsWindows.clear();
    g.SettingsHandlers.clear();

    if (g.LogFile)
    {
#ifndef AZ_DISABLE_TTY_FUNCTIONS
        if (g.LogFile != stdout)
#endif
            AzFileClose(g.LogFile);
        g.LogFile = NULL;
    }
    g.LogBuffer.clear();
    g.DebugLogBuf.clear();
    g.DebugLogIndex.clear();

    g.Initialized = false;
}

AzID Az::AddContextHook(AzContext* ctx, const AzContextHook* hook)
{
    AzContext& g = *ctx;
    AZ_ASSERT(hook->Callback != NULL && hook->HookId == 0
              && hook->Type != AzContextHookType_PendingRemoval_);
    g.Hooks.push_back(*hook);
    g.Hooks.back().HookId = ++g.HookIdNext;
    return g.HookIdNext;
}

void Az::RemoveContextHook(AzContext* ctx, AzID hook_id)
{
    AzContext& g = *ctx;
    AZ_ASSERT(hook_id != 0);
    for (AzContextHook& hook : g.Hooks)
        if (hook.HookId == hook_id)
            hook.Type = AzContextHookType_PendingRemoval_;
}

void Az::CallContextHooks(AzContext* ctx, AzContextHookType hook_type)
{
    AzContext& g = *ctx;
    for (AzContextHook& hook : g.Hooks)
        if (hook.Type == hook_type)
            hook.Callback(&g, &hook);
}

AzWindow::AzWindow(AzContext* ctx, const char* name) : DrawListInst(NULL)
{
    memset(this, 0, sizeof(*this));
    Ctx = ctx;
    Name = AzStrdup(name);
    NameBufLen = (int)AzStrlen(name) + 1;
    ID = AzHashStr(name);
    IDStack.push_back(ID);
    ViewportAllowPlatformMonitorExtend = -1;
    ViewportPos = AzVec2(FLT_MAX, FLT_MAX);
    MoveId = GetID("#MOVE");
    TabId = GetID("#TAB");
    ScrollTarget = AzVec2(FLT_MAX, FLT_MAX);
    ScrollTargetCenterRatio = AzVec2(0.5f, 0.5f);
    AutoFitFramesX = AutoFitFramesY = -1;
    AutoPosLastDirection = AzDir_None;
    SetWindowPosAllowFlags = SetWindowSizeAllowFlags =
            SetWindowCollapsedAllowFlags = SetWindowDockAllowFlags = 0;
    SetWindowPosVal = SetWindowPosPivot = AzVec2(FLT_MAX, FLT_MAX);
    LastFrameActive = -1;
    LastFrameJustFocused = -1;
    LastTimeActive = -1.0f;
    FontRefSize = 0.0f;
    FontWindowScale = FontWindowScaleParents = 1.0f;
    SettingsOffset = -1;
    DockOrder = -1;
    DrawList = &DrawListInst;
    DrawList->_OwnerName = Name;
    DrawList->_SetDrawListSharedData(&Ctx->DrawListSharedData);
    NavPreferredScoringPosRel[0] = NavPreferredScoringPosRel[1] =
            AzVec2(FLT_MAX, FLT_MAX);
    AZ_PLACEMENT_NEW(&WindowClass) AzWindowClass();
}

AzWindow::~AzWindow()
{
    AZ_ASSERT(DrawList == &DrawListInst);
    AZ_DELETE(Name);
    ColumnsStorage.clear_destruct();
}

static void SetCurrentWindow(AzWindow* window)
{
    AzContext& g = *GAz;
    g.CurrentWindow = window;
    g.StackSizesInBeginForCurrentWindow =
            g.CurrentWindow ? &g.CurrentWindowStack.back().StackSizesInBegin
                            : NULL;
    g.CurrentTable = window && window->DC.CurrentTableIdx != -1
                             ? g.Tables.GetByIndex(window->DC.CurrentTableIdx)
                             : NULL;
    if (window)
    {
        if (g.IO.BackendFlags & AzBackendFlags_RendererHasTextures)
        {
            AzViewport* viewport = window->Viewport;
            g.FontRasterizerDensity = (viewport->FramebufferScale.x != 0.0f)
                                              ? viewport->FramebufferScale.x
                                              : g.IO.DisplayFramebufferScale.x;
        }
        Az::UpdateCurrentFontSize(0.0f);
        Az::NavUpdateCurrentWindowIsScrollPushableX();
    }
}

void Az::GcCompactTransientMiscBuffers()
{
    AzContext& g = *GAz;
    g.ItemFlagsStack.clear();
    g.GroupStack.clear();
    g.MultiSelectTempDataStacked = 0;
    g.MultiSelectTempData.clear_destruct();
    TableGcCompactSettings();
    for (AzFontAtlas* atlas : g.FontAtlases)
        atlas->CompactCache();
}

void Az::GcCompactTransientWindowBuffers(AzWindow* window)
{
    window->MemoryCompacted = true;
    window->MemoryDrawListIdxCapacity = window->DrawList->IdxBuffer.Capacity;
    window->MemoryDrawListVtxCapacity = window->DrawList->VtxBuffer.Capacity;
    window->IDStack.clear();
    window->DrawList->_ClearFreeMemory();
    window->DC.ChildWindows.clear();
    window->DC.ItemWidthStack.clear();
    window->DC.TextWrapPosStack.clear();
}

void Az::GcAwakeTransientWindowBuffers(AzWindow* window)
{
    window->MemoryCompacted = false;
    window->DrawList->IdxBuffer.reserve(window->MemoryDrawListIdxCapacity);
    window->DrawList->VtxBuffer.reserve(window->MemoryDrawListVtxCapacity);
    window->MemoryDrawListIdxCapacity = window->MemoryDrawListVtxCapacity = 0;
}

void Az::SetActiveID(AzID id, AzWindow* window)
{
    AzContext& g = *GAz;

    if (g.ActiveId != 0)
    {
        if (g.MovingWindow != NULL && g.ActiveId == g.MovingWindow->MoveId)
        {
            AZ_DEBUG_LOG_ACTIVEID("SetActiveID() cancel MovingWindow\n");
            g.MovingWindow = NULL;
        }

        AzDeactivatedItemData* deactivated_data = &g.DeactivatedItemData;
        deactivated_data->ID = g.ActiveId;
        deactivated_data->ElapseFrame = (g.LastItemData.ID == g.ActiveId)
                                                ? g.FrameCount
                                                : g.FrameCount + 1;
        deactivated_data->HasBeenEditedBefore = g.ActiveIdHasBeenEditedBefore;
        deactivated_data->IsAlive = (g.ActiveIdIsAlive == g.ActiveId);

        if (g.InputTextState.ID == g.ActiveId)
            InputTextDeactivateHook(g.ActiveId);
    }

    g.ActiveIdIsJustActivated = (g.ActiveId != id);
    if (g.ActiveIdIsJustActivated)
    {
        AZ_DEBUG_LOG_ACTIVEID(
                "SetActiveID() old:0x%08X (window \"%s\") -> new:0x%08X "
                "(window \"%s\")\n",
                g.ActiveId,
                g.ActiveIdWindow ? g.ActiveIdWindow->Name : "",
                id,
                window ? window->Name : "");
        g.ActiveIdTimer = 0.0f;
        g.ActiveIdHasBeenPressedBefore = false;
        g.ActiveIdHasBeenEditedBefore = false;
        g.ActiveIdMouseButton = -1;
        if (id != 0)
        {
            g.LastActiveId = id;
            g.LastActiveIdTimer = 0.0f;
        }
    }
    g.ActiveId = id;
    g.ActiveIdAllowOverlap = false;
    g.ActiveIdNoClearOnFocusLoss = false;
    g.ActiveIdWindow = window;
    g.ActiveIdHasBeenEditedThisFrame = false;
    g.ActiveIdFromShortcut = false;
    if (id)
    {
        g.ActiveIdIsAlive = id;
        g.ActiveIdSource = (g.NavActivateId == id || g.NavJustMovedToId == id)
                                   ? g.NavInputSource
                                   : AzInputSource_Mouse;
        AZ_ASSERT(g.ActiveIdSource != AzInputSource_None);
    }

    g.ActiveIdUsingNavDirMask = 0x00;
    g.ActiveIdUsingAllKeyboardKeys = false;
}

void Az::ClearActiveID()
{
    SetActiveID(0, NULL);
}

void Az::SetHoveredID(AzID id)
{
    AzContext& g = *GAz;
    g.HoveredId = id;
    g.HoveredIdAllowOverlap = false;
    if (id != 0 && g.HoveredIdPreviousFrame != id)
        g.HoveredIdTimer = g.HoveredIdNotActiveTimer = 0.0f;
}

AzID Az::GetHoveredID()
{
    AzContext& g = *GAz;
    return g.HoveredId ? g.HoveredId : g.HoveredIdPreviousFrame;
}

void Az::MarkItemEdited(AzID id)
{
    AzContext& g = *GAz;
    if (g.LastItemData.ItemFlags & AzItemFlags_NoMarkEdited)
        return;
    if (g.ActiveId == id || g.ActiveId == 0)
    {
        g.ActiveIdHasBeenEditedThisFrame = true;
        g.ActiveIdHasBeenEditedBefore = true;
        if (g.DeactivatedItemData.ID == id)
            g.DeactivatedItemData.HasBeenEditedBefore = true;
    }

    AZ_ASSERT(g.DragDropActive || g.ActiveId == id || g.ActiveId == 0
              || g.ActiveIdPreviousFrame == id
              || (g.CurrentMultiSelect != NULL && g.BoxSelectState.IsActive));

    g.LastItemData.StatusFlags |= AzItemStatusFlags_Edited;
}

bool Az::IsWindowContentHoverable(AzWindow* window, AzHoveredFlags flags)
{
    AzContext& g = *GAz;
    if (g.NavWindow)
        if (AzWindow* focused_root_window = g.NavWindow->RootWindowDockTree)
            if (focused_root_window->WasActive
                && focused_root_window != window->RootWindowDockTree)
            {
                bool want_inhibit = false;
                if (focused_root_window->Flags & AzWindowFlags_Modal)
                    want_inhibit = true;
                else if ((focused_root_window->Flags & AzWindowFlags_Popup)
                         && !(flags & AzHoveredFlags_AllowWhenBlockedByPopup))
                    want_inhibit = true;

                if (want_inhibit)
                    if (!IsWindowWithinBeginStackOf(window->RootWindow,
                                                    focused_root_window))
                        return false;
            }

    if (window->Viewport != g.MouseViewport)
        if (g.MovingWindow == NULL
            || window->RootWindowDockTree != g.MovingWindow->RootWindowDockTree)
            return false;

    return true;
}

static inline float CalcDelayFromHoveredFlags(AzHoveredFlags flags)
{
    AzContext& g = *GAz;
    if (flags & AzHoveredFlags_DelayNormal)
        return g.Style.HoverDelayNormal;
    if (flags & AzHoveredFlags_DelayShort)
        return g.Style.HoverDelayShort;
    return 0.0f;
}

static AzHoveredFlags ApplyHoverFlagsForTooltip(AzHoveredFlags user_flags,
                                                AzHoveredFlags shared_flags)
{
    if (user_flags
        & (AzHoveredFlags_DelayNone | AzHoveredFlags_DelayShort
           | AzHoveredFlags_DelayNormal))
        shared_flags &= ~(AzHoveredFlags_DelayNone | AzHoveredFlags_DelayShort
                          | AzHoveredFlags_DelayNormal);
    return user_flags | shared_flags;
}

bool Az::IsItemHovered(AzHoveredFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT_USER_ERROR((flags & ~AzHoveredFlags_AllowedMaskForIsItemHovered)
                                 == 0,
                         "Invalid flags for IsItemHovered()!");

    if (g.NavHighlightItemUnderNav && g.NavCursorVisible
        && !(flags & AzHoveredFlags_NoNavOverride))
    {
        if (!IsItemFocused())
            return false;
        if ((g.LastItemData.ItemFlags & AzItemFlags_Disabled)
            && !(flags & AzHoveredFlags_AllowWhenDisabled))
            return false;

        if (flags & AzHoveredFlags_ForTooltip)
            flags = ApplyHoverFlagsForTooltip(flags,
                                              g.Style.HoverFlagsForTooltipNav);
    } else
    {
        AzItemStatusFlags status_flags = g.LastItemData.StatusFlags;
        if (!(status_flags & AzItemStatusFlags_HoveredRect))
            return false;

        if (flags & AzHoveredFlags_ForTooltip)
            flags = ApplyHoverFlagsForTooltip(
                    flags,
                    g.Style.HoverFlagsForTooltipMouse);

        if (g.HoveredWindow != window
            && (status_flags & AzItemStatusFlags_HoveredWindow) == 0)
            if ((flags & AzHoveredFlags_AllowWhenOverlappedByWindow) == 0)
                return false;

        const AzID id = g.LastItemData.ID;
        if ((flags & AzHoveredFlags_AllowWhenBlockedByActiveItem) == 0)
            if (g.ActiveId != 0 && g.ActiveId != id && !g.ActiveIdAllowOverlap)
                if (g.ActiveId != window->MoveId && g.ActiveId != window->TabId)
                    return false;

        if (!IsWindowContentHoverable(window, flags)
            && !(g.LastItemData.ItemFlags & AzItemFlags_NoWindowHoverableCheck))
            return false;

        if ((g.LastItemData.ItemFlags & AzItemFlags_Disabled)
            && !(flags & AzHoveredFlags_AllowWhenDisabled))
            return false;

        if (id == window->MoveId && window->WriteAccessed)
            return false;

        if ((g.LastItemData.ItemFlags & AzItemFlags_AllowOverlap) && id != 0)
            if ((flags & AzHoveredFlags_AllowWhenOverlappedByItem) == 0)
                if (g.HoveredIdPreviousFrame != g.LastItemData.ID)
                    return false;
    }

    const float delay = CalcDelayFromHoveredFlags(flags);
    if (delay > 0.0f || (flags & AzHoveredFlags_Stationary))
    {
        AzID hover_delay_id = (g.LastItemData.ID != 0)
                                      ? g.LastItemData.ID
                                      : window->GetIDFromPos(
                                                g.LastItemData.Rect.Min);
        if ((flags & AzHoveredFlags_NoSharedDelay)
            && (g.HoverItemDelayIdPreviousFrame != hover_delay_id))
            g.HoverItemDelayTimer = 0.0f;
        g.HoverItemDelayId = hover_delay_id;

        if ((flags & AzHoveredFlags_Stationary) != 0
            && g.HoverItemUnlockedStationaryId != hover_delay_id)
            return false;

        if (g.HoverItemDelayTimer < delay)
            return false;
    }

    return true;
}

bool Az::ItemHoverable(const AzRect& bb, AzID id, AzItemFlags item_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (id != 0 && g.HoveredIdPreviousFrame == id
        && (item_flags & AzItemFlags_AllowDuplicateId) == 0)
    {
        g.HoveredIdPreviousFrameItemCount++;
        if (g.DebugDrawIdConflicts == id)
            window->DrawList->AddRect(bb.Min - AzVec2(1, 1),
                                      bb.Max + AzVec2(1, 1),
                                      AZ_COL32(255, 0, 0, 255),
                                      0.0f,
                                      AzDrawFlags_None,
                                      2.0f);
    }
#endif

    if (g.HoveredWindow != window)
        return false;
    if (!IsMouseHoveringRect(bb.Min, bb.Max))
        return false;

    if (g.HoveredId != 0 && g.HoveredId != id && !g.HoveredIdAllowOverlap)
        return false;
    if (g.ActiveId != 0 && g.ActiveId != id && !g.ActiveIdAllowOverlap)
        if (!g.ActiveIdFromShortcut)
            return false;

    if (!(item_flags & AzItemFlags_NoWindowHoverableCheck)
        && !IsWindowContentHoverable(window, AzHoveredFlags_None))
    {
        g.HoveredIdIsDisabled = true;
        return false;
    }

    if (id != 0)
    {
        if (g.DragDropActive && g.DragDropPayload.SourceId == id
            && !(g.DragDropSourceFlags & AzDragDropFlags_SourceNoDisableHover))
            return false;

        SetHoveredID(id);

        if (item_flags & AzItemFlags_AllowOverlap)
        {
            g.HoveredIdAllowOverlap = true;
            if (g.HoveredIdPreviousFrame != id)
                return false;
        }

        if (id == g.LastItemData.ID
            && (g.LastItemData.StatusFlags & AzItemStatusFlags_HasShortcut)
            && g.ActiveId != id)
            if (IsItemHovered(AzHoveredFlags_ForTooltip
                              | AzHoveredFlags_DelayNormal))
                SetTooltip("%s", GetKeyChordName(g.LastItemData.Shortcut));
    }

    if (item_flags & AzItemFlags_Disabled)
    {
        if (g.ActiveId == id && id != 0)
            ClearActiveID();
        g.HoveredIdIsDisabled = true;
        return false;
    }

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (id != 0)
    {
        if (g.DebugItemPickerActive && g.HoveredIdPreviousFrame == id)
            GetForegroundDrawList()->AddRect(bb.Min,
                                             bb.Max,
                                             AZ_COL32(255, 255, 0, 255));
        if (g.DebugItemPickerBreakId == id)
            AZ_DEBUG_BREAK();
    }
#endif

    if (g.NavHighlightItemUnderNav
        && (item_flags & AzItemFlags_NoNavDisableMouseHover) == 0)
        return false;

    return true;
}

bool Az::IsClippedEx(const AzRect& bb, AzID id)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (!bb.Overlaps(window->ClipRect))
        if (id == 0
            || (id != g.ActiveId && id != g.ActiveIdPreviousFrame
                && id != g.NavId && id != g.NavActivateId))
            if (!g.ItemUnclipByLog)
                return true;
    return false;
}

void Az::SetLastItemData(AzID item_id,
                         AzItemFlags item_flags,
                         AzItemStatusFlags status_flags,
                         const AzRect& item_rect)
{
    AzContext& g = *GAz;
    g.LastItemData.ID = item_id;
    g.LastItemData.ItemFlags = item_flags;
    g.LastItemData.StatusFlags = status_flags;
    g.LastItemData.Rect = g.LastItemData.NavRect = item_rect;
}

static void Az::SetLastItemDataForWindow(AzWindow* window, const AzRect& rect)
{
    AzContext& g = *GAz;
    if (window->DockIsActive)
        SetLastItemData(window->MoveId,
                        g.CurrentItemFlags,
                        window->DC.DockTabItemStatusFlags,
                        window->DC.DockTabItemRect);
    else
        SetLastItemData(window->MoveId,
                        g.CurrentItemFlags,
                        window->DC.WindowItemStatusFlags,
                        rect);
}

static void Az::SetLastItemDataForChildWindowItem(AzWindow* window,
                                                  const AzRect& rect)
{
    AzContext& g = *GAz;
    SetLastItemData(window->ChildId,
                    g.CurrentItemFlags,
                    window->DC.ChildItemStatusFlags,
                    rect);
}

float Az::CalcWrapWidthForPos(const AzVec2& pos, float wrap_pos_x)
{
    if (wrap_pos_x < 0.0f)
        return 0.0f;

    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (wrap_pos_x == 0.0f)
    {
        wrap_pos_x = window->WorkRect.Max.x;
    } else if (wrap_pos_x > 0.0f)
    {
        wrap_pos_x += window->Pos.x - window->Scroll.x;
    }

    return AzMax(wrap_pos_x - pos.x, 1.0f);
}

void* Az::MemAlloc(size_t size)
{
    void* ptr = (*GAzAllocatorAllocFunc)(size, GAzAllocatorUserData);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (AzContext* ctx = GAz)
        DebugAllocHook(&ctx->DebugAllocInfo, ctx->FrameCount, ptr, size);
#endif
    return ptr;
}

void Az::MemFree(void* ptr)
{
#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (ptr != NULL)
        if (AzContext* ctx = GAz)
            DebugAllocHook(&ctx->DebugAllocInfo,
                           ctx->FrameCount,
                           ptr,
                           (size_t)-1);
#endif
    return (*GAzAllocatorFreeFunc)(ptr, GAzAllocatorUserData);
}

void Az::DebugAllocHook(AzDebugAllocInfo* info,
                        int frame_count,
                        void* ptr,
                        size_t size)
{
    AzDebugAllocEntry* entry = &info->LastEntriesBuf[info->LastEntriesIdx];
    AZ_UNUSED(ptr);
    if (entry->FrameCount != frame_count)
    {
        info->LastEntriesIdx = (info->LastEntriesIdx + 1)
                               % AZ_ARRAYSIZE(info->LastEntriesBuf);
        entry = &info->LastEntriesBuf[info->LastEntriesIdx];
        entry->FrameCount = frame_count;
        entry->AllocCount = entry->FreeCount = 0;
    }
    if (size != (size_t)-1)
    {
        entry->AllocCount++;
        info->TotalAllocCount++;
    } else
    {
        entry->FreeCount++;
        info->TotalFreeCount++;
    }
}

const char* Az::GetClipboardText()
{
    AzContext& g = *GAz;
    return g.PlatformIO.Platform_GetClipboardTextFn
                   ? g.PlatformIO.Platform_GetClipboardTextFn(&g)
                   : "";
}

void Az::SetClipboardText(const char* text)
{
    AzContext& g = *GAz;
    if (g.PlatformIO.Platform_SetClipboardTextFn != NULL)
        g.PlatformIO.Platform_SetClipboardTextFn(&g, text);
}

const char* Az::GetVersion()
{
    return AZ_VERSION;
}

AzIO& Az::GetIO()
{
    AZ_ASSERT(GAz != NULL && "No current context. Did you call Az::CreateContext() and Az::SetCurrentContext() ?");
    return GAz->IO;
}

AzIO& Az::GetIO(AzContext* ctx)
{
    AZ_ASSERT(ctx != NULL);
    return ctx->IO;
}

AzPlatformIO& Az::GetPlatformIO()
{
    AZ_ASSERT(GAz != NULL && "No current context. Did you call Az::CreateContext() and Az::SetCurrentContext()?");
    return GAz->PlatformIO;
}

AzPlatformIO& Az::GetPlatformIO(AzContext* ctx)
{
    AZ_ASSERT(ctx != NULL);
    return ctx->PlatformIO;
}

AzDrawData* Az::GetDrawData()
{
    AzContext& g = *GAz;
    AzViewportP* viewport = g.Viewports[0];
    return viewport->DrawDataP.Valid ? &viewport->DrawDataP : NULL;
}

double Az::GetTime()
{
    return GAz->Time;
}

int Az::GetFrameCount()
{
    return GAz->FrameCount;
}

static AzDrawList* GetViewportBgFgDrawList(AzViewportP* viewport,
                                           size_t drawlist_no,
                                           const char* drawlist_name)
{
    AzContext& g = *GAz;
    AZ_ASSERT(drawlist_no < AZ_ARRAYSIZE(viewport->BgFgDrawLists));
    AzDrawList* draw_list = viewport->BgFgDrawLists[drawlist_no];
    if (draw_list == NULL)
    {
        draw_list = AZ_NEW(AzDrawList)(&g.DrawListSharedData);
        draw_list->_OwnerName = drawlist_name;
        viewport->BgFgDrawLists[drawlist_no] = draw_list;
    }

    if (viewport->BgFgDrawListsLastFrame[drawlist_no] != g.FrameCount)
    {
        draw_list->_ResetForNewFrame();
        draw_list->PushTexture(g.IO.Fonts->TexRef);
        draw_list->PushClipRect(viewport->Pos,
                                viewport->Pos + viewport->Size,
                                false);
        viewport->BgFgDrawListsLastFrame[drawlist_no] = g.FrameCount;
    }
    return draw_list;
}

AzDrawList* Az::GetBackgroundDrawList(AzViewport* viewport)
{
    if (viewport == NULL)
        viewport = GAz->CurrentWindow->Viewport;
    return GetViewportBgFgDrawList((AzViewportP*)viewport, 0, "##Background");
}

AzDrawList* Az::GetForegroundDrawList(AzViewport* viewport)
{
    if (viewport == NULL)
        viewport = GAz->CurrentWindow->Viewport;
    return GetViewportBgFgDrawList((AzViewportP*)viewport, 1, "##Foreground");
}

AzDrawListSharedData* Az::GetDrawListSharedData()
{
    return &GAz->DrawListSharedData;
}

void Az::StartMouseMovingWindow(AzWindow* window)
{
    AzContext& g = *GAz;
    FocusWindow(window);
    SetActiveID(window->MoveId, window);
    if (g.IO.ConfigNavCursorVisibleAuto)
        g.NavCursorVisible = false;
    g.ActiveIdClickOffset = g.IO.MouseClickedPos[0]
                            - window->RootWindowDockTree->Pos;
    g.ActiveIdNoClearOnFocusLoss = true;
    SetActiveIdUsingAllKeyboardKeys();

    bool can_move_window = true;
    if ((window->Flags & AzWindowFlags_NoMove)
        || (window->RootWindowDockTree->Flags & AzWindowFlags_NoMove))
        can_move_window = false;
    if (AzDockNode* node = window->DockNodeAsHost)
        if (node->VisibleWindow
            && (node->VisibleWindow->Flags & AzWindowFlags_NoMove))
            can_move_window = false;
    if (can_move_window)
        g.MovingWindow = window;
}

void Az::StartMouseMovingWindowOrNode(AzWindow* window,
                                      AzDockNode* node,
                                      bool undock)
{
    AzContext& g = *GAz;
    bool can_undock_node = false;
    if (undock && node != NULL && node->VisibleWindow
        && (node->VisibleWindow->Flags & AzWindowFlags_NoMove) == 0
        && (node->MergedFlags & AzDockNodeFlags_NoUndocking) == 0)
    {
        AzDockNode* root_node = DockNodeGetRootNode(node);
        if (root_node->OnlyNodeWithWindows != node
            || root_node->CentralNode != NULL)

            can_undock_node = true;
    }

    const bool clicked = IsMouseClicked(0);
    const bool dragging = IsMouseDragging(0);
    if (can_undock_node && dragging)
        DockContextQueueUndockNode(&g, node);

    else if (!can_undock_node && (clicked || dragging)
             && g.MovingWindow != window)
        StartMouseMovingWindow(window);
}

void Az::UpdateMouseMovingWindowNewFrame()
{
    AzContext& g = *GAz;
    if (g.MovingWindow != NULL)
    {
        KeepAliveID(g.ActiveId);
        AZ_ASSERT(g.MovingWindow && g.MovingWindow->RootWindowDockTree);
        AzWindow* moving_window = g.MovingWindow->RootWindowDockTree;

        const bool window_disappared = (!moving_window->WasActive
                                        && !moving_window->Active);
        if (g.IO.MouseDown[0] && IsMousePosValid(&g.IO.MousePos)
            && !window_disappared)
        {
            AzVec2 pos = g.IO.MousePos - g.ActiveIdClickOffset;
            if (moving_window->Pos.x != pos.x || moving_window->Pos.y != pos.y)
            {
                SetWindowPos(moving_window, pos, AzCond_Always);
                if (moving_window->Viewport && moving_window->ViewportOwned)

                {
                    moving_window->Viewport->Pos = pos;
                    moving_window->Viewport->UpdateWorkRect();
                }
            }
            FocusWindow(g.MovingWindow);
        } else
        {
            if (!window_disappared)
            {
                if (g.ConfigFlagsCurrFrame & AzConfigFlags_ViewportsEnable)
                    UpdateTryMergeWindowIntoHostViewport(moving_window,
                                                         g.MouseViewport);

                if (moving_window->Viewport
                    && !IsDragDropPayloadBeingAccepted())
                    g.MouseViewport = moving_window->Viewport;

                if (moving_window->Viewport)
                    moving_window->Viewport->Flags &= ~AzViewportFlags_NoInputs;
            }

            g.MovingWindow = NULL;
            ClearActiveID();
        }
    } else
    {
        if (g.ActiveIdWindow && g.ActiveIdWindow->MoveId == g.ActiveId)
        {
            KeepAliveID(g.ActiveId);
            if (!g.IO.MouseDown[0])
                ClearActiveID();
        }
    }
}

void Az::UpdateMouseMovingWindowEndFrame()
{
    AzContext& g = *GAz;
    if (g.ActiveId != 0 || (g.HoveredId != 0 && !g.HoveredIdIsDisabled))
        return;

    if (g.NavWindow && g.NavWindow->Appearing)
        return;

    if (g.IO.MouseClicked[0])
    {
        AzWindow* root_window = g.HoveredWindow ? g.HoveredWindow->RootWindow
                                                : NULL;
        const bool is_closed_popup =
                root_window && (root_window->Flags & AzWindowFlags_Popup)
                && !IsPopupOpen(root_window->PopupId,
                                AzPopupFlags_AnyPopupLevel);

        if (root_window != NULL && !is_closed_popup)
        {
            StartMouseMovingWindow(g.HoveredWindow);

            if (g.IO.ConfigWindowsMoveFromTitleBarOnly)
                if (!(root_window->Flags & AzWindowFlags_NoTitleBar)
                    || root_window->DockIsActive)
                    if (!root_window->TitleBarRect().Contains(
                                g.IO.MouseClickedPos[0]))
                        g.MovingWindow = NULL;

            if (g.HoveredIdIsDisabled)
                g.MovingWindow = NULL;
        } else if (root_window == NULL && g.NavWindow != NULL)
        {
            FocusWindow(NULL, AzFocusRequestFlags_UnlessBelowModal);
        }
    }

    if (g.IO.MouseClicked[1] && g.HoveredId == 0)
    {
        AzWindow* modal = GetTopMostPopupModal();
        bool hovered_window_above_modal = g.HoveredWindow
                                          && (modal == NULL
                                              || IsWindowAbove(g.HoveredWindow,
                                                               modal));
        ClosePopupsOverWindow(hovered_window_above_modal ? g.HoveredWindow
                                                         : modal,
                              true);
    }
}

static void TranslateWindow(AzWindow* window, const AzVec2& delta)
{
    window->Pos += delta;
    window->ClipRect.Translate(delta);
    window->OuterRectClipped.Translate(delta);
    window->InnerRect.Translate(delta);
    window->DC.CursorPos += delta;
    window->DC.CursorStartPos += delta;
    window->DC.CursorMaxPos += delta;
    window->DC.IdealMaxPos += delta;
}

static void ScaleWindow(AzWindow* window, float scale)
{
    AzVec2 origin = window->Viewport->Pos;
    window->Pos = AzFloor((window->Pos - origin) * scale + origin);
    window->Size = AzTrunc(window->Size * scale);
    window->SizeFull = AzTrunc(window->SizeFull * scale);
    window->ContentSize = AzTrunc(window->ContentSize * scale);
}

static bool IsWindowActiveAndVisible(AzWindow* window)
{
    return (window->Active) && (!window->Hidden);
}

void Az::UpdateHoveredWindowAndCaptureFlags(const AzVec2& mouse_pos)
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;

    g.WindowsBorderHoverPadding = AzMax(AzMax(g.Style.TouchExtraPadding.x,
                                              g.Style.TouchExtraPadding.y),
                                        g.Style.WindowBorderHoverPadding);

    bool clear_hovered_windows = false;
    FindHoveredWindowEx(mouse_pos,
                        false,
                        &g.HoveredWindow,
                        &g.HoveredWindowUnderMovingWindow);
    AZ_ASSERT(g.HoveredWindow == NULL || g.HoveredWindow == g.MovingWindow
              || g.HoveredWindow->Viewport == g.MouseViewport);
    g.HoveredWindowBeforeClear = g.HoveredWindow;

    AzWindow* modal_window = GetTopMostPopupModal();
    if (modal_window && g.HoveredWindow
        && !IsWindowWithinBeginStackOf(g.HoveredWindow->RootWindow,
                                       modal_window))

        clear_hovered_windows = true;

    if (io.ConfigFlags & AzConfigFlags_NoMouse)
        clear_hovered_windows = true;

    const bool has_open_popup = (g.OpenPopupStack.Size > 0);
    const bool has_open_modal = (modal_window != NULL);
    int mouse_earliest_down = -1;
    bool mouse_any_down = false;
    for (int i = 0; i < AZ_ARRAYSIZE(io.MouseDown); i++)
    {
        if (io.MouseClicked[i])
        {
            io.MouseDownOwned[i] = (g.HoveredWindow != NULL) || has_open_popup;
            io.MouseDownOwnedUnlessPopupClose[i] = (g.HoveredWindow != NULL)
                                                   || has_open_modal;
        }
        mouse_any_down |= io.MouseDown[i];
        if (io.MouseDown[i] || io.MouseReleased[i])

            if (mouse_earliest_down == -1
                || io.MouseClickedTime[i]
                           < io.MouseClickedTime[mouse_earliest_down])
                mouse_earliest_down = i;
    }
    const bool mouse_avail = (mouse_earliest_down == -1)
                             || io.MouseDownOwned[mouse_earliest_down];
    const bool mouse_avail_unless_popup_close =
            (mouse_earliest_down == -1)
            || io.MouseDownOwnedUnlessPopupClose[mouse_earliest_down];

    const bool mouse_dragging_extern_payload =
            g.DragDropActive
            && (g.DragDropSourceFlags & AzDragDropFlags_SourceExtern) != 0;
    if (!mouse_avail && !mouse_dragging_extern_payload)
        clear_hovered_windows = true;

    if (clear_hovered_windows)
        g.HoveredWindow = g.HoveredWindowUnderMovingWindow = NULL;

    if (g.WantCaptureMouseNextFrame != -1)
    {
        io.WantCaptureMouse = io.WantCaptureMouseUnlessPopupClose =
                (g.WantCaptureMouseNextFrame != 0);
    } else
    {
        io.WantCaptureMouse = (mouse_avail
                               && (g.HoveredWindow != NULL || mouse_any_down))
                              || has_open_popup;
        io.WantCaptureMouseUnlessPopupClose = (mouse_avail_unless_popup_close
                                               && (g.HoveredWindow != NULL
                                                   || mouse_any_down))
                                              || has_open_modal;
    }

    io.WantCaptureKeyboard = false;
    if ((io.ConfigFlags & AzConfigFlags_NoKeyboard) == 0)
    {
        if ((g.ActiveId != 0) || (modal_window != NULL))
            io.WantCaptureKeyboard = true;
        else if (io.NavActive
                 && (io.ConfigFlags & AzConfigFlags_NavEnableKeyboard)
                 && io.ConfigNavCaptureKeyboard)
            io.WantCaptureKeyboard = true;
    }
    if (g.WantCaptureKeyboardNextFrame != -1)
        io.WantCaptureKeyboard = (g.WantCaptureKeyboardNextFrame != 0);

    io.WantTextInput = (g.WantTextInputNextFrame != -1)
                               ? (g.WantTextInputNextFrame != 0)
                               : false;
}

static void Az::UpdateTexturesNewFrame()
{
    AzContext& g = *GAz;
    const bool has_textures = (g.IO.BackendFlags
                               & AzBackendFlags_RendererHasTextures)
                              != 0;
    for (AzFontAtlas* atlas : g.FontAtlases)
    {
        if (atlas->OwnerContext == &g)
        {
            atlas->RendererHasTextures = has_textures;
            AzFontAtlasUpdateNewFrame(atlas, g.FrameCount);
        } else
        {
            AZ_ASSERT(atlas->Builder != NULL && atlas->Builder->FrameCount != -1 && "If you manage font atlases yourself you need to call AzFontAtlasUpdateNewFrame() on it.");
            AZ_ASSERT(atlas->RendererHasTextures == has_textures && "If you manage font atlases yourself make sure atlas->RendererHasTextures is set consistently with all contexts using it.");
        }
    }
}

static void Az::UpdateTexturesEndFrame()
{
    AzContext& g = *GAz;
    g.PlatformIO.Textures.resize(0);
    for (AzFontAtlas* atlas : g.FontAtlases)
        for (AzTextureData* tex : atlas->TexList)
        {
            tex->RefCount = (unsigned short)atlas->RefCount;
            g.PlatformIO.Textures.push_back(tex);
        }
    for (AzTextureData* tex : g.UserTextures)
        g.PlatformIO.Textures.push_back(tex);
}

static void SetupDrawListSharedData()
{
    AzContext& g = *GAz;
    AzRect virtual_space(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (AzViewportP* viewport : g.Viewports)
        virtual_space.Add(viewport->GetMainRect());
    g.DrawListSharedData.ClipRectFullscreen = virtual_space.ToVec4();
    g.DrawListSharedData.CurveTessellationTol = g.Style.CurveTessellationTol;
    g.DrawListSharedData.SetCircleTessellationMaxError(
            g.Style.CircleTessellationMaxError);
    g.DrawListSharedData.InitialFlags = AzDrawListFlags_None;
    if (g.Style.AntiAliasedLines)
        g.DrawListSharedData.InitialFlags |= AzDrawListFlags_AntiAliasedLines;
    if (g.Style.AntiAliasedLinesUseTex
        && !(g.IO.Fonts->Flags & AzFontAtlasFlags_NoBakedLines))
        g.DrawListSharedData.InitialFlags |=
                AzDrawListFlags_AntiAliasedLinesUseTex;
    if (g.Style.AntiAliasedFill)
        g.DrawListSharedData.InitialFlags |= AzDrawListFlags_AntiAliasedFill;
    if (g.IO.BackendFlags & AzBackendFlags_RendererHasVtxOffset)
        g.DrawListSharedData.InitialFlags |= AzDrawListFlags_AllowVtxOffset;
    g.DrawListSharedData.InitialFringeScale = 1.0f;
}

void Az::NewFrame()
{
    AZ_ASSERT(GAz != NULL && "No current context. Did you call Az::CreateContext() and Az::SetCurrentContext() ?");
    AzContext& g = *GAz;

    for (int n = g.Hooks.Size - 1; n >= 0; n--)
        if (g.Hooks[n].Type == AzContextHookType_PendingRemoval_)
            g.Hooks.erase(&g.Hooks[n]);

    CallContextHooks(&g, AzContextHookType_NewFramePre);

    g.ConfigFlagsLastFrame = g.ConfigFlagsCurrFrame;
    ErrorCheckNewFrameSanityChecks();
    g.ConfigFlagsCurrFrame = g.IO.ConfigFlags;

    UpdateSettings();

    g.Time += g.IO.DeltaTime;
    g.FrameCount += 1;
    g.TooltipOverrideCount = 0;
    g.WindowsActiveCount = 0;
    g.MenusIdSubmittedThisFrame.resize(0);

    g.FramerateSecPerFrameAccum +=
            g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
    g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
    g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1)
                                % AZ_ARRAYSIZE(g.FramerateSecPerFrame);
    g.FramerateSecPerFrameCount = AzMin(g.FramerateSecPerFrameCount + 1,
                                        AZ_ARRAYSIZE(g.FramerateSecPerFrame));
    g.IO.Framerate = (g.FramerateSecPerFrameAccum > 0.0f)
                             ? (1.0f
                                / (g.FramerateSecPerFrameAccum
                                   / (float)g.FramerateSecPerFrameCount))
                             : FLT_MAX;

    g.InputEventsTrail.resize(0);
    UpdateInputEvents(g.IO.ConfigInputTrickleEventQueue);

    UpdateViewportsNewFrame();

    UpdateTexturesNewFrame();

    SetupDrawListSharedData();
    UpdateFontsNewFrame();

    g.WithinFrameScope = true;

    for (AzViewportP* viewport : g.Viewports)
    {
        viewport->DrawData = NULL;
        viewport->DrawDataP.Valid = false;
    }

    if (g.DragDropActive && g.DragDropPayload.SourceId == g.ActiveId)
        KeepAliveID(g.DragDropPayload.SourceId);

    if (!g.IO.ConfigDebugHighlightIdConflicts || !g.IO.KeyCtrl)
        g.DebugDrawIdConflicts = 0;
    if (g.IO.ConfigDebugHighlightIdConflicts
        && g.HoveredIdPreviousFrameItemCount > 1)
        g.DebugDrawIdConflicts = g.HoveredIdPreviousFrame;

    if (!g.HoveredIdPreviousFrame)
        g.HoveredIdTimer = 0.0f;
    if (!g.HoveredIdPreviousFrame || (g.HoveredId && g.ActiveId == g.HoveredId))
        g.HoveredIdNotActiveTimer = 0.0f;
    if (g.HoveredId)
        g.HoveredIdTimer += g.IO.DeltaTime;
    if (g.HoveredId && g.ActiveId != g.HoveredId)
        g.HoveredIdNotActiveTimer += g.IO.DeltaTime;
    g.HoveredIdPreviousFrame = g.HoveredId;
    g.HoveredIdPreviousFrameItemCount = 0;
    g.HoveredId = 0;
    g.HoveredIdAllowOverlap = false;
    g.HoveredIdIsDisabled = false;

    if (g.ActiveId != 0 && g.ActiveIdIsAlive != g.ActiveId
        && g.ActiveIdPreviousFrame == g.ActiveId)
    {
        AZ_DEBUG_LOG_ACTIVEID(
                "NewFrame(): ClearActiveID() because it isn't marked alive "
                "anymore!\n");
        ClearActiveID();
    }

    if (g.ActiveId)
        g.ActiveIdTimer += g.IO.DeltaTime;
    g.LastActiveIdTimer += g.IO.DeltaTime;
    g.ActiveIdPreviousFrame = g.ActiveId;
    g.ActiveIdIsAlive = 0;
    g.ActiveIdHasBeenEditedThisFrame = false;
    g.ActiveIdIsJustActivated = false;
    if (g.TempInputId != 0 && g.ActiveId != g.TempInputId)
        g.TempInputId = 0;
    if (g.ActiveId == 0)
    {
        g.ActiveIdUsingNavDirMask = 0x00;
        g.ActiveIdUsingAllKeyboardKeys = false;
    }
    if (g.DeactivatedItemData.ElapseFrame < g.FrameCount)
        g.DeactivatedItemData.ID = 0;
    g.DeactivatedItemData.IsAlive = false;

    if (g.HoverItemDelayId != 0
        && g.MouseStationaryTimer >= g.Style.HoverStationaryDelay)
        g.HoverItemUnlockedStationaryId = g.HoverItemDelayId;
    else if (g.HoverItemDelayId == 0)
        g.HoverItemUnlockedStationaryId = 0;
    if (g.HoveredWindow != NULL
        && g.MouseStationaryTimer >= g.Style.HoverStationaryDelay)
        g.HoverWindowUnlockedStationaryId = g.HoveredWindow->ID;
    else if (g.HoveredWindow == NULL)
        g.HoverWindowUnlockedStationaryId = 0;

    g.HoverItemDelayIdPreviousFrame = g.HoverItemDelayId;
    if (g.HoverItemDelayId != 0)
    {
        g.HoverItemDelayTimer += g.IO.DeltaTime;
        g.HoverItemDelayClearTimer = 0.0f;
        g.HoverItemDelayId = 0;
    } else if (g.HoverItemDelayTimer > 0.0f)
    {
        g.HoverItemDelayClearTimer += g.IO.DeltaTime;
        if (g.HoverItemDelayClearTimer >= AzMax(0.25f, g.IO.DeltaTime * 2.0f))

            g.HoverItemDelayTimer = g.HoverItemDelayClearTimer = 0.0f;
    }

    g.DragDropAcceptIdPrev = g.DragDropAcceptIdCurr;
    g.DragDropAcceptIdCurr = 0;
    g.DragDropAcceptIdCurrRectSurface = FLT_MAX;
    g.DragDropWithinSource = false;
    g.DragDropWithinTarget = false;
    g.DragDropHoldJustPressedId = 0;
    g.TooltipPreviousWindow = NULL;

    UpdateKeyboardInputs();

    NavUpdate();

    UpdateMouseInputs();

    DockContextNewFrameUpdateUndocking(&g);

    AZ_ASSERT(g.WindowsFocusOrder.Size <= g.Windows.Size);
    const float memory_compact_start_time =
            (g.GcCompactAll || g.IO.ConfigMemoryCompactTimer < 0.0f)
                    ? FLT_MAX
                    : (float)g.Time - g.IO.ConfigMemoryCompactTimer;
    for (AzWindow* window : g.Windows)
    {
        window->WasActive = window->Active;
        window->Active = false;
        window->WriteAccessed = false;
        window->BeginCountPreviousFrame = window->BeginCount;
        window->BeginCount = 0;

        if (!window->WasActive && !window->MemoryCompacted
            && window->LastTimeActive < memory_compact_start_time)
            GcCompactTransientWindowBuffers(window);
    }

    UpdateHoveredWindowAndCaptureFlags(g.IO.MousePos);

    UpdateMouseMovingWindowNewFrame();

    if (GetTopMostPopupModal() != NULL
        || (g.NavWindowingTarget != NULL
            && g.NavWindowingHighlightAlpha > 0.0f))
        g.DimBgRatio = AzMin(g.DimBgRatio + g.IO.DeltaTime * 6.0f, 1.0f);
    else
        g.DimBgRatio = AzMax(g.DimBgRatio - g.IO.DeltaTime * 10.0f, 0.0f);

    g.MouseCursor = AzMouseCursor_Arrow;
    g.WantCaptureMouseNextFrame = g.WantCaptureKeyboardNextFrame =
            g.WantTextInputNextFrame = -1;

    g.PlatformAzeDataPrev = g.PlatformAzeData;
    g.PlatformAzeData.WantVisible = g.PlatformAzeData.WantTextInput = false;

    UpdateMouseWheel();

    for (int i = 0; i < g.TablesLastTimeActive.Size; i++)
        if (g.TablesLastTimeActive[i] >= 0.0f
            && g.TablesLastTimeActive[i] < memory_compact_start_time)
            TableGcCompactTransientBuffers(g.Tables.GetByIndex(i));
    for (AzTableTempData& table_temp_data : g.TablesTempData)
        if (table_temp_data.LastTimeActive >= 0.0f
            && table_temp_data.LastTimeActive < memory_compact_start_time)
            TableGcCompactTransientBuffers(&table_temp_data);
    if (g.GcCompactAll)
        GcCompactTransientMiscBuffers();
    g.GcCompactAll = false;

    if (g.NavWindow && !g.NavWindow->WasActive)
        FocusTopMostWindowUnderOne(NULL,
                                   NULL,
                                   NULL,
                                   AzFocusRequestFlags_RestoreFocusedChild);

    g.CurrentWindowStack.resize(0);
    g.BeginPopupStack.resize(0);
    g.ItemFlagsStack.resize(0);
    g.ItemFlagsStack.push_back(AzItemFlags_AutoClosePopups);
    g.CurrentItemFlags = g.ItemFlagsStack.back();
    g.GroupStack.resize(0);

    DockContextNewFrameUpdateDocking(&g);

#ifndef AZ_DISABLE_DEBUG_TOOLS
    UpdateDebugToolItemPicker();
    UpdateDebugToolStackQueries();
    UpdateDebugToolFlashStyleColor();
    if (g.DebugLocateFrames > 0 && --g.DebugLocateFrames == 0)
    {
        g.DebugLocateId = 0;
        g.DebugBreakInLocateId = false;
    }
    if (g.DebugLogAutoDisableFrames > 0 && --g.DebugLogAutoDisableFrames == 0)
    {
        DebugLog(
                "(Debug Log: Auto-disabled some AzDebugLogFlags after 2 "
                "frames)\n");
        g.DebugLogFlags &= ~g.DebugLogAutoDisableFlags;
        g.DebugLogAutoDisableFlags = AzDebugLogFlags_None;
    }
#endif

    g.WithinFrameScopeWithAzplicitWindow = true;
    SetNextWindowSize(AzVec2(400, 400), AzCond_FirstUseEver);
    Begin("Debug##Default");
    AZ_ASSERT(g.CurrentWindow->IsFallbackWindow == true);

    g.ErrorCountCurrentFrame = 0;
    ErrorRecoveryStoreState(&g.StackSizesInNewFrame);

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (g.IO.ConfigDebugBeginReturnValueLoop)
        g.DebugBeginReturnValueCullDepth =
                (g.DebugBeginReturnValueCullDepth == -1)
                        ? 0
                        : ((g.DebugBeginReturnValueCullDepth
                            + ((g.FrameCount % 4) == 0 ? 1 : 0))
                           % 10);
    else
        g.DebugBeginReturnValueCullDepth = -1;
#endif

    CallContextHooks(&g, AzContextHookType_NewFramePost);
}

static int AZ_CDECL ChildWindowComparer(const void* lhs, const void* rhs)
{
    const AzWindow* const a = *(const AzWindow* const*)lhs;
    const AzWindow* const b = *(const AzWindow* const*)rhs;
    if (int d = (a->Flags & AzWindowFlags_Popup)
                - (b->Flags & AzWindowFlags_Popup))
        return d;
    if (int d = (a->Flags & AzWindowFlags_Tooltip)
                - (b->Flags & AzWindowFlags_Tooltip))
        return d;
    return (a->BeginOrderWithinParent - b->BeginOrderWithinParent);
}

static void AddWindowToSortBuffer(AzVector<AzWindow*>* out_sorted_windows,
                                  AzWindow* window)
{
    out_sorted_windows->push_back(window);
    if (window->Active)
    {
        int count = window->DC.ChildWindows.Size;
        AzQsort(window->DC.ChildWindows.Data,
                (size_t)count,
                sizeof(AzWindow*),
                ChildWindowComparer);
        for (int i = 0; i < count; i++)
        {
            AzWindow* child = window->DC.ChildWindows[i];
            if (child->Active)
                AddWindowToSortBuffer(out_sorted_windows, child);
        }
    }
}

static void AddWindowToDrawData(AzWindow* window, int layer)
{
    AzContext& g = *GAz;
    AzViewportP* viewport = window->Viewport;
    AZ_ASSERT(viewport != NULL);
    g.IO.MetricsRenderWindows++;
    if (window->DrawList->_Splitter._Count > 1)
        window->DrawList->ChannelsMerge();

    Az::AddDrawListToDrawDataEx(&viewport->DrawDataP,
                                viewport->DrawDataBuilder.Layers[layer],
                                window->DrawList);
    for (AzWindow* child : window->DC.ChildWindows)
        if (IsWindowActiveAndVisible(child))
            AddWindowToDrawData(child, layer);
}

static inline int GetWindowDisplayLayer(AzWindow* window)
{
    return (window->Flags & AzWindowFlags_Tooltip) ? 1 : 0;
}

static inline void AddRootWindowToDrawData(AzWindow* window)
{
    AddWindowToDrawData(window, GetWindowDisplayLayer(window));
}

static void FlattenDrawDataIntoSingleLayer(AzDrawDataBuilder* builder)
{
    int n = builder->Layers[0]->Size;
    int full_size = n;
    for (int i = 1; i < AZ_ARRAYSIZE(builder->Layers); i++)
        full_size += builder->Layers[i]->Size;
    builder->Layers[0]->resize(full_size);
    for (int layer_n = 1; layer_n < AZ_ARRAYSIZE(builder->Layers); layer_n++)
    {
        AzVector<AzDrawList*>* layer = builder->Layers[layer_n];
        if (layer->empty())
            continue;
        memcpy(builder->Layers[0]->Data + n,
               layer->Data,
               layer->Size * sizeof(AzDrawList*));
        n += layer->Size;
        layer->resize(0);
    }
}

static void InitViewportDrawData(AzViewportP* viewport)
{
    AzIO& io = Az::GetIO();
    AzDrawData* draw_data = &viewport->DrawDataP;

    viewport->DrawData = draw_data;
    viewport->DrawDataBuilder.Layers[0] = &draw_data->CmdLists;
    viewport->DrawDataBuilder.Layers[1] = &viewport->DrawDataBuilder.LayerData1;
    viewport->DrawDataBuilder.Layers[0]->resize(0);
    viewport->DrawDataBuilder.Layers[1]->resize(0);

    const bool is_minimized = (viewport->Flags & AzViewportFlags_IsMinimized)
                              != 0;

    draw_data->Valid = true;
    draw_data->CmdListsCount = 0;
    draw_data->TotalVtxCount = draw_data->TotalIdxCount = 0;
    draw_data->DisplayPos = viewport->Pos;
    draw_data->DisplaySize = is_minimized ? AzVec2(0.0f, 0.0f) : viewport->Size;
    draw_data->FramebufferScale = (viewport->FramebufferScale.x != 0.0f)
                                          ? viewport->FramebufferScale
                                          : io.DisplayFramebufferScale;
    draw_data->OwnerViewport = viewport;
    draw_data->Textures = &Az::GetPlatformIO().Textures;
}

void Az::PushClipRect(const AzVec2& clip_rect_min,
                      const AzVec2& clip_rect_max,
                      bool intersect_with_current_clip_rect)
{
    AzWindow* window = GetCurrentWindow();
    window->DrawList->PushClipRect(clip_rect_min,
                                   clip_rect_max,
                                   intersect_with_current_clip_rect);
    window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void Az::PopClipRect()
{
    AzWindow* window = GetCurrentWindow();
    window->DrawList->PopClipRect();
    window->ClipRect = window->DrawList->_ClipRectStack.back();
}

static AzWindow* FindFrontMostVisibleChildWindow(AzWindow* window)
{
    for (int n = window->DC.ChildWindows.Size - 1; n >= 0; n--)
        if (IsWindowActiveAndVisible(window->DC.ChildWindows[n]))
            return FindFrontMostVisibleChildWindow(window->DC.ChildWindows[n]);
    return window;
}

static void Az::RenderDimmedBackgroundBehindWindow(AzWindow* window, AzU32 col)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    AzViewportP* viewport = window->Viewport;
    AzRect viewport_rect = viewport->GetMainRect();

    {
        AzDrawList* draw_list = window->RootWindowDockTree->DrawList;
        draw_list->ChannelsMerge();
        if (draw_list->CmdBuffer.Size == 0)
            draw_list->AddDrawCmd();
        draw_list->PushClipRect(viewport_rect.Min - AzVec2(1, 1),
                                viewport_rect.Max + AzVec2(1, 1),
                                false);

        draw_list->AddRectFilled(viewport_rect.Min, viewport_rect.Max, col);
        AzDrawCmd cmd = draw_list->CmdBuffer.back();
        AZ_ASSERT(cmd.ElemCount == 6);
        draw_list->CmdBuffer.pop_back();
        draw_list->CmdBuffer.push_front(cmd);
        draw_list->AddDrawCmd();

        draw_list->PopClipRect();
    }

    if (window->RootWindow->DockIsActive)
    {
        AzDrawList* draw_list = FindFrontMostVisibleChildWindow(
                                        window->RootWindowDockTree)
                                        ->DrawList;
        draw_list->ChannelsMerge();
        if (draw_list->CmdBuffer.Size == 0)
            draw_list->AddDrawCmd();
        draw_list->PushClipRect(viewport_rect.Min, viewport_rect.Max, false);
        RenderRectFilledWithHole(draw_list,
                                 window->RootWindowDockTree->Rect(),
                                 window->RootWindow->Rect(),
                                 col,
                                 0.0f);
        draw_list->PopClipRect();
    }
}

AzWindow* Az::FindBottomMostVisibleWindowWithinBeginStack(
        AzWindow* parent_window)
{
    AzContext& g = *GAz;
    AzWindow* bottom_most_visible_window = parent_window;
    for (int i = FindWindowDisplayIndex(parent_window); i >= 0; i--)
    {
        AzWindow* window = g.Windows[i];
        if (window->Flags & AzWindowFlags_ChildWindow)
            continue;
        if (!IsWindowWithinBeginStackOf(window, parent_window))
            break;
        if (IsWindowActiveAndVisible(window)
            && GetWindowDisplayLayer(window)
                       <= GetWindowDisplayLayer(parent_window))
            bottom_most_visible_window = window;
    }
    return bottom_most_visible_window;
}

static void Az::RenderDimmedBackgrounds()
{
    AzContext& g = *GAz;
    AzWindow* modal_window = GetTopMostAndVisiblePopupModal();
    if (g.DimBgRatio <= 0.0f && g.NavWindowingHighlightAlpha <= 0.0f)
        return;
    const bool dim_bg_for_modal = (modal_window != NULL);
    const bool dim_bg_for_window_list = (g.NavWindowingTargetAnim != NULL
                                         && g.NavWindowingTargetAnim->Active);
    if (!dim_bg_for_modal && !dim_bg_for_window_list)
        return;

    AzViewport* viewports_already_dimmed[2] = {NULL, NULL};
    if (dim_bg_for_modal)
    {
        AzWindow* dim_behind_window =
                FindBottomMostVisibleWindowWithinBeginStack(modal_window);
        RenderDimmedBackgroundBehindWindow(
                dim_behind_window,
                GetColorU32(modal_window->DC.ModalDimBgColor, g.DimBgRatio));
        viewports_already_dimmed[0] = modal_window->Viewport;
    } else if (dim_bg_for_window_list)
    {
        RenderDimmedBackgroundBehindWindow(g.NavWindowingTargetAnim,
                                           GetColorU32(AzCol_NavWindowingDimBg,
                                                       g.DimBgRatio));
        if (g.NavWindowingListWindow != NULL
            && g.NavWindowingListWindow->Viewport
            && g.NavWindowingListWindow->Viewport
                       != g.NavWindowingTargetAnim->Viewport)
            RenderDimmedBackgroundBehindWindow(
                    g.NavWindowingListWindow,
                    GetColorU32(AzCol_NavWindowingDimBg, g.DimBgRatio));
        viewports_already_dimmed[0] = g.NavWindowingTargetAnim->Viewport;
        viewports_already_dimmed[1] =
                g.NavWindowingListWindow ? g.NavWindowingListWindow->Viewport
                                         : NULL;

        AzWindow* window = g.NavWindowingTargetAnim;
        AzViewport* viewport = window->Viewport;
        float distance = g.FontSize;
        AzRect bb = window->Rect();
        bb.Expand(distance);
        if (bb.GetWidth() >= viewport->Size.x
            && bb.GetHeight() >= viewport->Size.y)
            bb.Expand(-distance - 1.0f);

        window->DrawList->ChannelsMerge();
        if (window->DrawList->CmdBuffer.Size == 0)
            window->DrawList->AddDrawCmd();
        window->DrawList->PushClipRect(viewport->Pos,
                                       viewport->Pos + viewport->Size);
        window->DrawList->AddRect(bb.Min,
                                  bb.Max,
                                  GetColorU32(AzCol_NavWindowingHighlight,
                                              g.NavWindowingHighlightAlpha),
                                  window->WindowRounding,
                                  0,
                                  3.0f);
        window->DrawList->PopClipRect();
    }

    for (AzViewportP* viewport : g.Viewports)
    {
        if (viewport == viewports_already_dimmed[0]
            || viewport == viewports_already_dimmed[1])
            continue;
        if (modal_window && viewport->Window
            && IsWindowAbove(viewport->Window, modal_window))
            continue;
        AzDrawList* draw_list = GetForegroundDrawList(viewport);
        const AzU32 dim_bg_col = GetColorU32(dim_bg_for_modal
                                                     ? AzCol_ModalWindowDimBg
                                                     : AzCol_NavWindowingDimBg,
                                             g.DimBgRatio);
        draw_list->AddRectFilled(viewport->Pos,
                                 viewport->Pos + viewport->Size,
                                 dim_bg_col);
    }
}

void Az::EndFrame()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.Initialized);

    if (g.FrameCountEnded == g.FrameCount)
        return;
    if (!g.WithinFrameScope)
    {
        AZ_ASSERT_USER_ERROR(g.WithinFrameScope,
                             "Forgot to call Az::NewFrame()?");
        return;
    }

    CallContextHooks(&g, AzContextHookType_EndFramePre);

    if (g.IO.ConfigErrorRecovery)
        ErrorRecoveryTryToRecoverState(&g.StackSizesInNewFrame);
    ErrorCheckEndFrameSanityChecks();
    ErrorCheckEndFrameFinalizeErrorTooltip();

    AzPlatformAzeData* ime_data = &g.PlatformAzeData;
    if (g.PlatformIO.Platform_SetAzeDataFn != NULL
        && memcmp(ime_data, &g.PlatformAzeDataPrev, sizeof(AzPlatformAzeData))
                   != 0)
    {
        AzViewport* viewport = FindViewportByID(ime_data->ViewportId);
        AZ_DEBUG_LOG_IO(
                "[io] Calling Platform_SetAzeDataFn(): WantVisible: %d, "
                "InputPos (%.2f,%.2f)\n",
                ime_data->WantVisible,
                ime_data->InputPos.x,
                ime_data->InputPos.y);
        if (viewport == NULL)
            viewport = GetMainViewport();
        g.PlatformIO.Platform_SetAzeDataFn(&g, viewport, ime_data);
    }
    g.WantTextInputNextFrame = ime_data->WantTextInput ? 1 : 0;

    g.WithinFrameScopeWithAzplicitWindow = false;
    if (g.CurrentWindow && !g.CurrentWindow->WriteAccessed)
        g.CurrentWindow->Active = false;
    End();

    NavEndFrame();

    DockContextEndFrame(&g);

    SetCurrentViewport(NULL, NULL);

    if (g.DragDropActive)
    {
        bool is_delivered = g.DragDropPayload.Delivery;
        bool is_elapsed = (g.DragDropSourceFrameCount + 1 < g.FrameCount)
                          && ((g.DragDropSourceFlags
                               & AzDragDropFlags_PayloadAutoExpire)
                              || g.DragDropMouseButton == -1
                              || !IsMouseDown(g.DragDropMouseButton));
        if (is_delivered || is_elapsed)
            ClearDragDrop();
    }

    if (g.DragDropActive && g.DragDropSourceFrameCount + 1 < g.FrameCount
        && !(g.DragDropSourceFlags & AzDragDropFlags_SourceNoPreviewTooltip))
    {
        g.DragDropWithinSource = true;
        SetTooltip("...");
        g.DragDropWithinSource = false;
    }

    g.WithinFrameScope = false;
    g.FrameCountEnded = g.FrameCount;
    UpdateFontsEndFrame();

    UpdateMouseMovingWindowEndFrame();

    UpdateViewportsEndFrame();

    g.WindowsTempSortBuffer.resize(0);
    g.WindowsTempSortBuffer.reserve(g.Windows.Size);
    for (AzWindow* window : g.Windows)
    {
        if (window->Active && (window->Flags & AzWindowFlags_ChildWindow))

            continue;
        AddWindowToSortBuffer(&g.WindowsTempSortBuffer, window);
    }

    AZ_ASSERT(g.Windows.Size == g.WindowsTempSortBuffer.Size);
    g.Windows.swap(g.WindowsTempSortBuffer);
    g.IO.MetricsActiveWindows = g.WindowsActiveCount;

    UpdateTexturesEndFrame();

    for (AzFontAtlas* atlas : g.FontAtlases)
        atlas->Locked = false;

    g.IO.MousePosPrev = g.IO.MousePos;
    g.IO.AppFocusLost = false;
    g.IO.MouseWheel = g.IO.MouseWheelH = 0.0f;
    g.IO.InputQueueCharacters.resize(0);

    CallContextHooks(&g, AzContextHookType_EndFramePost);
}

void Az::Render()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.Initialized);

    if (g.FrameCountEnded != g.FrameCount)
        EndFrame();
    if (g.FrameCountRendered == g.FrameCount)
        return;
    g.FrameCountRendered = g.FrameCount;

    g.IO.MetricsRenderWindows = 0;
    CallContextHooks(&g, AzContextHookType_RenderPre);

    for (AzViewportP* viewport : g.Viewports)
    {
        InitViewportDrawData(viewport);
        if (viewport->BgFgDrawLists[0] != NULL)
            AddDrawListToDrawDataEx(&viewport->DrawDataP,
                                    viewport->DrawDataBuilder.Layers[0],
                                    GetBackgroundDrawList(viewport));
    }

    RenderDimmedBackgrounds();

    AzWindow* windows_to_render_top_most[2];
    windows_to_render_top_most[0] =
            (g.NavWindowingTarget
             && !(g.NavWindowingTarget->Flags
                  & AzWindowFlags_NoBringToFrontOnFocus))
                    ? g.NavWindowingTarget->RootWindowDockTree
                    : NULL;
    windows_to_render_top_most[1] = (g.NavWindowingTarget
                                             ? g.NavWindowingListWindow
                                             : NULL);
    for (AzWindow* window : g.Windows)
    {
        AZ_MSVC_WARNING_SUPPRESS(6011);

        if (IsWindowActiveAndVisible(window)
            && (window->Flags & AzWindowFlags_ChildWindow) == 0
            && window != windows_to_render_top_most[0]
            && window != windows_to_render_top_most[1])
            AddRootWindowToDrawData(window);
    }
    for (int n = 0; n < AZ_ARRAYSIZE(windows_to_render_top_most); n++)
        if (windows_to_render_top_most[n]
            && IsWindowActiveAndVisible(windows_to_render_top_most[n]))

            AddRootWindowToDrawData(windows_to_render_top_most[n]);

    if (g.IO.MouseDrawCursor && g.MouseCursor != AzMouseCursor_None)
        RenderMouseCursor(g.IO.MousePos,
                          g.Style.MouseCursorScale,
                          g.MouseCursor,
                          AZ_COL32_WHITE,
                          AZ_COL32_BLACK,
                          AZ_COL32(0, 0, 0, 48));

    g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices = 0;
    for (AzViewportP* viewport : g.Viewports)
    {
        FlattenDrawDataIntoSingleLayer(&viewport->DrawDataBuilder);

        if (viewport->BgFgDrawLists[1] != NULL)
            AddDrawListToDrawDataEx(&viewport->DrawDataP,
                                    viewport->DrawDataBuilder.Layers[0],
                                    GetForegroundDrawList(viewport));

        AzDrawData* draw_data = &viewport->DrawDataP;
        AZ_ASSERT(draw_data->CmdLists.Size == draw_data->CmdListsCount);
        for (AzDrawList* draw_list : draw_data->CmdLists)
            draw_list->_PopUnusedDrawCmd();

        g.IO.MetricsRenderVertices += draw_data->TotalVtxCount;
        g.IO.MetricsRenderIndices += draw_data->TotalIdxCount;
    }

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (g.IO.BackendFlags & AzBackendFlags_RendererHasTextures)
        for (AzFontAtlas* atlas : g.FontAtlases)
            AzFontAtlasDebugLogTextureRequests(atlas);
#endif

    CallContextHooks(&g, AzContextHookType_RenderPost);
}

AzVec2 Az::CalcTextSize(const char* text,
                        const char* text_end,
                        bool hide_text_after_double_hash,
                        float wrap_width)
{
    AzContext& g = *GAz;

    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = FindRenderedTextEnd(text, text_end);

    else
        text_display_end = text_end;

    AzFont* font = g.Font;
    const float font_size = g.FontSize;
    if (text == text_display_end)
        return AzVec2(0.0f, font_size);
    AzVec2 text_size = font->CalcTextSizeA(font_size,
                                           FLT_MAX,
                                           wrap_width,
                                           text,
                                           text_display_end,
                                           NULL);

    text_size.x = AZ_TRUNC(text_size.x + 0.99999f);

    return text_size;
}

void Az::FindHoveredWindowEx(const AzVec2& pos,
                             bool find_first_and_in_any_viewport,
                             AzWindow** out_hovered_window,
                             AzWindow** out_hovered_window_under_moving_window)
{
    AzContext& g = *GAz;
    AzWindow* hovered_window = NULL;
    AzWindow* hovered_window_under_moving_window = NULL;

    AzViewportP* backup_moving_window_viewport = NULL;
    if (find_first_and_in_any_viewport == false && g.MovingWindow)
    {
        backup_moving_window_viewport = g.MovingWindow->Viewport;
        g.MovingWindow->Viewport = g.MouseViewport;
        if (!(g.MovingWindow->Flags & AzWindowFlags_NoMouseInputs))
            hovered_window = g.MovingWindow;
    }

    AzVec2 padding_regular = g.Style.TouchExtraPadding;
    AzVec2 padding_for_resize = AzMax(g.Style.TouchExtraPadding,
                                      AzVec2(g.Style.WindowBorderHoverPadding,
                                             g.Style.WindowBorderHoverPadding));
    for (int i = g.Windows.Size - 1; i >= 0; i--)
    {
        AzWindow* window = g.Windows[i];
        AZ_MSVC_WARNING_SUPPRESS(28182);
        if (!window->WasActive || window->Hidden)
            continue;
        if (window->Flags & AzWindowFlags_NoMouseInputs)
            continue;
        AZ_ASSERT(window->Viewport);
        if (window->Viewport != g.MouseViewport)
            continue;

        AzVec2 hit_padding = (window->Flags
                              & (AzWindowFlags_NoResize
                                 | AzWindowFlags_AlwaysAutoResize))
                                     ? padding_regular
                                     : padding_for_resize;
        if (!window->OuterRectClipped.ContainsWithPad(pos, hit_padding))
            continue;

        if (window->HitTestHoleSize.x != 0)
        {
            AzVec2 hole_pos(window->Pos.x + (float)window->HitTestHoleOffset.x,
                            window->Pos.y + (float)window->HitTestHoleOffset.y);
            AzVec2 hole_size((float)window->HitTestHoleSize.x,
                             (float)window->HitTestHoleSize.y);
            if (AzRect(hole_pos, hole_pos + hole_size).Contains(pos))
                continue;
        }

        if (find_first_and_in_any_viewport)
        {
            hovered_window = window;
            break;
        } else
        {
            if (hovered_window == NULL)
                hovered_window = window;
            AZ_MSVC_WARNING_SUPPRESS(28182);
            if (hovered_window_under_moving_window == NULL
                && (!g.MovingWindow
                    || window->RootWindowDockTree
                               != g.MovingWindow->RootWindowDockTree))
                hovered_window_under_moving_window = window;
            if (hovered_window && hovered_window_under_moving_window)
                break;
        }
    }

    *out_hovered_window = hovered_window;
    if (out_hovered_window_under_moving_window != NULL)
        *out_hovered_window_under_moving_window =
                hovered_window_under_moving_window;
    if (find_first_and_in_any_viewport == false && g.MovingWindow)
        g.MovingWindow->Viewport = backup_moving_window_viewport;
}

bool Az::IsItemActive()
{
    AzContext& g = *GAz;
    if (g.ActiveId)
        return g.ActiveId == g.LastItemData.ID;
    return false;
}

bool Az::IsItemActivated()
{
    AzContext& g = *GAz;
    if (g.ActiveId)
        if (g.ActiveId == g.LastItemData.ID
            && g.ActiveIdPreviousFrame != g.LastItemData.ID)
            return true;
    return false;
}

bool Az::IsItemDeactivated()
{
    AzContext& g = *GAz;
    if (g.LastItemData.StatusFlags & AzItemStatusFlags_HasDeactivated)
        return (g.LastItemData.StatusFlags & AzItemStatusFlags_Deactivated)
               != 0;
    return (g.DeactivatedItemData.ID == g.LastItemData.ID
            && g.LastItemData.ID != 0
            && g.DeactivatedItemData.ElapseFrame >= g.FrameCount);
}

bool Az::IsItemDeactivatedAfterEdit()
{
    AzContext& g = *GAz;
    return IsItemDeactivated() && g.DeactivatedItemData.HasBeenEditedBefore;
}

bool Az::IsItemFocused()
{
    AzContext& g = *GAz;
    if (g.NavId != g.LastItemData.ID || g.NavId == 0)
        return false;

    AzWindow* window = g.CurrentWindow;
    if (g.LastItemData.ID == window->ID && window->WriteAccessed)
        return false;

    return true;
}

bool Az::IsItemClicked(AzMouseButton mouse_button)
{
    return IsMouseClicked(mouse_button) && IsItemHovered(AzHoveredFlags_None);
}

bool Az::IsItemToggledOpen()
{
    AzContext& g = *GAz;
    return (g.LastItemData.StatusFlags & AzItemStatusFlags_ToggledOpen) ? true
                                                                        : false;
}

bool Az::IsItemToggledSelection()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.CurrentMultiSelect != NULL);

    return (g.LastItemData.StatusFlags & AzItemStatusFlags_ToggledSelection)
                   ? true
                   : false;
}

bool Az::IsAnyItemHovered()
{
    AzContext& g = *GAz;
    return g.HoveredId != 0 || g.HoveredIdPreviousFrame != 0;
}

bool Az::IsAnyItemActive()
{
    AzContext& g = *GAz;
    return g.ActiveId != 0;
}

bool Az::IsAnyItemFocused()
{
    AzContext& g = *GAz;
    return g.NavId != 0 && g.NavCursorVisible;
}

bool Az::IsItemVisible()
{
    AzContext& g = *GAz;
    return (g.LastItemData.StatusFlags & AzItemStatusFlags_Visible) != 0;
}

bool Az::IsItemEdited()
{
    AzContext& g = *GAz;
    return (g.LastItemData.StatusFlags & AzItemStatusFlags_Edited) != 0;
}

void Az::SetNextItemAllowOverlap()
{
    AzContext& g = *GAz;
    g.NextItemData.ItemFlags |= AzItemFlags_AllowOverlap;
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

void Az::SetItemAllowOverlap()
{
    AzContext& g = *GAz;
    AzID id = g.LastItemData.ID;
    if (g.HoveredId == id)
        g.HoveredIdAllowOverlap = true;
    if (g.ActiveId == id)

        g.ActiveIdAllowOverlap = true;
}
#endif

void Az::SetActiveIdUsingAllKeyboardKeys()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.ActiveId != 0);
    g.ActiveIdUsingNavDirMask = (1 << AzDir_COUNT) - 1;
    g.ActiveIdUsingAllKeyboardKeys = true;
    NavMoveRequestCancel();
}

AzID Az::GetItemID()
{
    AzContext& g = *GAz;
    return g.LastItemData.ID;
}

AzVec2 Az::GetItemRectMin()
{
    AzContext& g = *GAz;
    return g.LastItemData.Rect.Min;
}

AzVec2 Az::GetItemRectMax()
{
    AzContext& g = *GAz;
    return g.LastItemData.Rect.Max;
}

AzVec2 Az::GetItemRectSize()
{
    AzContext& g = *GAz;
    return g.LastItemData.Rect.GetSize();
}

bool Az::BeginChild(const char* str_id,
                    const AzVec2& size_arg,
                    AzChildFlags child_flags,
                    AzWindowFlags window_flags)
{
    AzID id = GetCurrentWindow()->GetID(str_id);
    return BeginChildEx(str_id, id, size_arg, child_flags, window_flags);
}

bool Az::BeginChild(AzID id,
                    const AzVec2& size_arg,
                    AzChildFlags child_flags,
                    AzWindowFlags window_flags)
{
    return BeginChildEx(NULL, id, size_arg, child_flags, window_flags);
}

bool Az::BeginChildEx(const char* name,
                      AzID id,
                      const AzVec2& size_arg,
                      AzChildFlags child_flags,
                      AzWindowFlags window_flags)
{
    AzContext& g = *GAz;
    AzWindow* parent_window = g.CurrentWindow;
    AZ_ASSERT(id != 0);

    const AzChildFlags AzChildFlags_SupportedMask_ =
            AzChildFlags_Borders | AzChildFlags_AlwaysUseWindowPadding
            | AzChildFlags_ResizeX | AzChildFlags_ResizeY
            | AzChildFlags_AutoResizeX | AzChildFlags_AutoResizeY
            | AzChildFlags_AlwaysAutoResize | AzChildFlags_FrameStyle
            | AzChildFlags_NavFlattened;
    AZ_UNUSED(AzChildFlags_SupportedMask_);
    AZ_ASSERT((child_flags & ~AzChildFlags_SupportedMask_) == 0 && "Illegal AzChildFlags value. Did you pass AzWindowFlags values instead of AzChildFlags?");
    AZ_ASSERT((window_flags & AzWindowFlags_AlwaysAutoResize) == 0 && "Cannot specify AzWindowFlags_AlwaysAutoResize for BeginChild(). Use AzChildFlags_AlwaysAutoResize!");
    if (child_flags & AzChildFlags_AlwaysAutoResize)
    {
        AZ_ASSERT((child_flags & (AzChildFlags_ResizeX | AzChildFlags_ResizeY)) == 0 && "Cannot use AzChildFlags_ResizeX or AzChildFlags_ResizeY with AzChildFlags_AlwaysAutoResize!");
        AZ_ASSERT((child_flags & (AzChildFlags_AutoResizeX | AzChildFlags_AutoResizeY)) != 0 && "Must use AzChildFlags_AutoResizeX or AzChildFlags_AutoResizeY with AzChildFlags_AlwaysAutoResize!");
    }
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    if (window_flags & AzWindowFlags_AlwaysUseWindowPadding)
        child_flags |= AzChildFlags_AlwaysUseWindowPadding;
    if (window_flags & AzWindowFlags_NavFlattened)
        child_flags |= AzChildFlags_NavFlattened;
#endif
    if (child_flags & AzChildFlags_AutoResizeX)
        child_flags &= ~AzChildFlags_ResizeX;
    if (child_flags & AzChildFlags_AutoResizeY)
        child_flags &= ~AzChildFlags_ResizeY;

    window_flags |= AzWindowFlags_ChildWindow | AzWindowFlags_NoTitleBar
                    | AzWindowFlags_NoDocking;
    window_flags |= (parent_window->Flags & AzWindowFlags_NoMove);
    if (child_flags
        & (AzChildFlags_AutoResizeX | AzChildFlags_AutoResizeY
           | AzChildFlags_AlwaysAutoResize))
        window_flags |= AzWindowFlags_AlwaysAutoResize;
    if ((child_flags & (AzChildFlags_ResizeX | AzChildFlags_ResizeY)) == 0)
        window_flags |= AzWindowFlags_NoResize | AzWindowFlags_NoSavedSettings;

    if (child_flags & AzChildFlags_FrameStyle)
    {
        PushStyleColor(AzCol_ChildBg, g.Style.Colors[AzCol_FrameBg]);
        PushStyleVar(AzStyleVar_ChildRounding, g.Style.FrameRounding);
        PushStyleVar(AzStyleVar_ChildBorderSize, g.Style.FrameBorderSize);
        PushStyleVar(AzStyleVar_WindowPadding, g.Style.FramePadding);
        child_flags |= AzChildFlags_Borders
                       | AzChildFlags_AlwaysUseWindowPadding;
        window_flags |= AzWindowFlags_NoMove;
    }

    const AzVec2 size_avail = GetContentRegionAvail();
    const AzVec2 size_default(
            (child_flags & AzChildFlags_AutoResizeX) ? 0.0f : size_avail.x,
            (child_flags & AzChildFlags_AutoResizeY) ? 0.0f : size_avail.y);
    AzVec2 size = CalcItemSize(size_arg, size_default.x, size_default.y);

    if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSize) != 0
        && (g.NextWindowData.SizeCond & AzCond_Always) != 0)
    {
        if (g.NextWindowData.SizeVal.x > 0.0f)
        {
            size.x = g.NextWindowData.SizeVal.x;
            child_flags &= ~AzChildFlags_ResizeX;
        }
        if (g.NextWindowData.SizeVal.y > 0.0f)
        {
            size.y = g.NextWindowData.SizeVal.y;
            child_flags &= ~AzChildFlags_ResizeY;
        }
    }
    SetNextWindowSize(size);

    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasChildFlags)
        g.NextWindowData.ChildFlags |= child_flags;
    else
        g.NextWindowData.ChildFlags = child_flags;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasChildFlags;

    const char* temp_window_name;

    if (name)
        AzFormatStringToTempBuffer(&temp_window_name,
                                   NULL,
                                   "%s/%s_%08X",
                                   parent_window->Name,
                                   name,
                                   id);
    else
        AzFormatStringToTempBuffer(&temp_window_name,
                                   NULL,
                                   "%s/%08X",
                                   parent_window->Name,
                                   id);

    const float backup_border_size = g.Style.ChildBorderSize;
    if ((child_flags & AzChildFlags_Borders) == 0)
        g.Style.ChildBorderSize = 0.0f;

    const bool ret = Begin(temp_window_name, NULL, window_flags);

    g.Style.ChildBorderSize = backup_border_size;
    if (child_flags & AzChildFlags_FrameStyle)
    {
        PopStyleVar(3);
        PopStyleColor();
    }

    AzWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;

    if (child_window->BeginCount == 1)
        parent_window->DC.CursorPos = child_window->Pos;

    const AzID temp_id_for_activation = AzHashStr("##Child", 0, id);
    if (g.ActiveId == temp_id_for_activation)
        ClearActiveID();
    if (g.NavActivateId == id && !(child_flags & AzChildFlags_NavFlattened)
        && (child_window->DC.NavLayersActiveMask != 0
            || child_window->DC.NavWindowHasScrollY))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(temp_id_for_activation, child_window);

        g.ActiveIdSource = g.NavInputSource;
    }
    return ret;
}

void Az::EndChild()
{
    AzContext& g = *GAz;
    AzWindow* child_window = g.CurrentWindow;

    const AzID backup_within_end_child_id = g.WithinEndChildID;
    AZ_ASSERT(child_window->Flags & AzWindowFlags_ChildWindow);

    g.WithinEndChildID = child_window->ID;
    AzVec2 child_size = child_window->Size;
    End();
    if (child_window->BeginCount == 1)
    {
        AzWindow* parent_window = g.CurrentWindow;
        AzRect bb(parent_window->DC.CursorPos,
                  parent_window->DC.CursorPos + child_size);
        ItemSize(child_size);
        const bool nav_flattened = (child_window->ChildFlags
                                    & AzChildFlags_NavFlattened)
                                   != 0;
        if ((child_window->DC.NavLayersActiveMask != 0
             || child_window->DC.NavWindowHasScrollY)
            && !nav_flattened)
        {
            ItemAdd(bb, child_window->ChildId);
            RenderNavCursor(bb, child_window->ChildId);

            if (child_window->DC.NavLayersActiveMask == 0
                && child_window == g.NavWindow)
                RenderNavCursor(AzRect(bb.Min - AzVec2(2, 2),
                                       bb.Max + AzVec2(2, 2)),
                                g.NavId,
                                AzNavRenderCursorFlags_Compact);
        } else
        {
            ItemAdd(bb, child_window->ChildId, NULL, AzItemFlags_NoNav);

            if (nav_flattened)
                parent_window->DC.NavLayersActiveMaskNext |=
                        child_window->DC.NavLayersActiveMaskNext;
        }
        if (g.HoveredWindow == child_window)
            g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredWindow;
        child_window->DC.ChildItemStatusFlags = g.LastItemData.StatusFlags;

    } else
    {
        SetLastItemDataForChildWindowItem(child_window, child_window->Rect());
    }

    g.WithinEndChildID = backup_within_end_child_id;
    g.LogLinePosY = -FLT_MAX;
}

static void SetWindowConditionAllowFlags(AzWindow* window,
                                         AzCond flags,
                                         bool enabled)
{
    window->SetWindowPosAllowFlags = enabled ? (window->SetWindowPosAllowFlags
                                                | flags)
                                             : (window->SetWindowPosAllowFlags
                                                & ~flags);
    window->SetWindowSizeAllowFlags = enabled ? (window->SetWindowSizeAllowFlags
                                                 | flags)
                                              : (window->SetWindowSizeAllowFlags
                                                 & ~flags);
    window->SetWindowCollapsedAllowFlags =
            enabled ? (window->SetWindowCollapsedAllowFlags | flags)
                    : (window->SetWindowCollapsedAllowFlags & ~flags);
    window->SetWindowDockAllowFlags = enabled ? (window->SetWindowDockAllowFlags
                                                 | flags)
                                              : (window->SetWindowDockAllowFlags
                                                 & ~flags);
}

AzWindow* Az::FindWindowByID(AzID id)
{
    AzContext& g = *GAz;
    return (AzWindow*)g.WindowsById.GetVoidPtr(id);
}

AzWindow* Az::FindWindowByName(const char* name)
{
    AzID id = AzHashStr(name);
    return FindWindowByID(id);
}

static void ApplyWindowSettings(AzWindow* window, AzWindowSettings* settings)
{
    const AzViewport* main_viewport = Az::GetMainViewport();
    window->ViewportPos = main_viewport->Pos;
    if (settings->ViewportId)
    {
        window->ViewportId = settings->ViewportId;
        window->ViewportPos = AzVec2(settings->ViewportPos.x,
                                     settings->ViewportPos.y);
    }
    window->Pos = AzTrunc(AzVec2(settings->Pos.x + window->ViewportPos.x,
                                 settings->Pos.y + window->ViewportPos.y));
    if (settings->Size.x > 0 && settings->Size.y > 0)
        window->Size = window->SizeFull = AzTrunc(
                AzVec2(settings->Size.x, settings->Size.y));
    window->Collapsed = settings->Collapsed;
    window->DockId = settings->DockId;
    window->DockOrder = settings->DockOrder;
}

static void InitOrLoadWindowSettings(AzWindow* window,
                                     AzWindowSettings* settings)
{
    const AzViewport* main_viewport = Az::GetMainViewport();
    window->Pos = main_viewport->Pos + AzVec2(60, 60);
    window->Size = window->SizeFull = AzVec2(0, 0);
    window->ViewportPos = main_viewport->Pos;
    window->SetWindowPosAllowFlags = window->SetWindowSizeAllowFlags =
            window->SetWindowCollapsedAllowFlags =
                    window->SetWindowDockAllowFlags = AzCond_Always
                                                      | AzCond_Once
                                                      | AzCond_FirstUseEver
                                                      | AzCond_Appearing;

    if (settings != NULL)
    {
        SetWindowConditionAllowFlags(window, AzCond_FirstUseEver, false);
        ApplyWindowSettings(window, settings);
    }
    window->DC.CursorStartPos = window->DC.CursorMaxPos =
            window->DC.IdealMaxPos = window->Pos;

    if ((window->Flags & AzWindowFlags_AlwaysAutoResize) != 0)
    {
        window->AutoFitFramesX = window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = false;
    } else
    {
        if (window->Size.x <= 0.0f)
            window->AutoFitFramesX = 2;
        if (window->Size.y <= 0.0f)
            window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = (window->AutoFitFramesX > 0)
                                   || (window->AutoFitFramesY > 0);
    }
}

static AzWindow* CreateNewWindow(const char* name, AzWindowFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = AZ_NEW(AzWindow)(&g, name);
    window->Flags = flags;
    g.WindowsById.SetVoidPtr(window->ID, window);

    AzWindowSettings* settings = NULL;
    if (!(flags & AzWindowFlags_NoSavedSettings))
        if ((settings = Az::FindWindowSettingsByWindow(window)) != 0)
            window->SettingsOffset = g.SettingsWindows.offset_from_ptr(
                    settings);

    InitOrLoadWindowSettings(window, settings);

    if (flags & AzWindowFlags_NoBringToFrontOnFocus)
        g.Windows.push_front(window);
    else
        g.Windows.push_back(window);

    return window;
}

static AzWindow* GetWindowForTitleDisplay(AzWindow* window)
{
    return window->DockNodeAsHost ? window->DockNodeAsHost->VisibleWindow
                                  : window;
}

static AzWindow* GetWindowForTitleAndMenuHeight(AzWindow* window)
{
    return (window->DockNodeAsHost && window->DockNodeAsHost->VisibleWindow)
                   ? window->DockNodeAsHost->VisibleWindow
                   : window;
}

static inline AzVec2 CalcWindowMinSize(AzWindow* window)
{
    AzContext& g = *GAz;
    AzVec2 size_min;
    if ((window->Flags & AzWindowFlags_ChildWindow)
        && !(window->Flags & AzWindowFlags_Popup))
    {
        size_min.x = (window->ChildFlags & AzChildFlags_ResizeX)
                             ? g.Style.WindowMinSize.x
                             : 4.0f;
        size_min.y = (window->ChildFlags & AzChildFlags_ResizeY)
                             ? g.Style.WindowMinSize.y
                             : 4.0f;
    } else
    {
        size_min.x = ((window->Flags & AzWindowFlags_AlwaysAutoResize) == 0)
                             ? g.Style.WindowMinSize.x
                             : 4.0f;
        size_min.y = ((window->Flags & AzWindowFlags_AlwaysAutoResize) == 0)
                             ? g.Style.WindowMinSize.y
                             : 4.0f;
    }

    AzWindow* window_for_height = GetWindowForTitleAndMenuHeight(window);
    size_min.y = AzMax(size_min.y,
                       window_for_height->TitleBarHeight
                               + window_for_height->MenuBarHeight
                               + AzMax(0.0f, g.Style.WindowRounding - 1.0f));
    return size_min;
}

static AzVec2 CalcWindowSizeAfterConstraint(AzWindow* window,
                                            const AzVec2& size_desired)
{
    AzContext& g = *GAz;
    AzVec2 new_size = size_desired;
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSizeConstraint)
    {
        AzRect cr = g.NextWindowData.SizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0)
                             ? AzClamp(new_size.x, cr.Min.x, cr.Max.x)
                             : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0)
                             ? AzClamp(new_size.y, cr.Min.y, cr.Max.y)
                             : window->SizeFull.y;
        if (g.NextWindowData.SizeCallback)
        {
            AzSizeCallbackData data;
            data.UserData = g.NextWindowData.SizeCallbackUserData;
            data.Pos = window->Pos;
            data.CurrentSize = window->SizeFull;
            data.DesiredSize = new_size;
            g.NextWindowData.SizeCallback(&data);
            new_size = data.DesiredSize;
        }
        new_size.x = AZ_TRUNC(new_size.x);
        new_size.y = AZ_TRUNC(new_size.y);
    }

    AzVec2 size_min = CalcWindowMinSize(window);
    return AzMax(new_size, size_min);
}

static void CalcWindowContentSizes(AzWindow* window,
                                   AzVec2* content_size_current,
                                   AzVec2* content_size_ideal)
{
    bool preserve_old_content_sizes = false;
    if (window->Collapsed && window->AutoFitFramesX <= 0
        && window->AutoFitFramesY <= 0)
        preserve_old_content_sizes = true;
    else if (window->Hidden && window->HiddenFramesCannotSkipItems == 0
             && window->HiddenFramesCanSkipItems > 0)
        preserve_old_content_sizes = true;
    if (preserve_old_content_sizes)
    {
        *content_size_current = window->ContentSize;
        *content_size_ideal = window->ContentSizeIdeal;
        return;
    }

    content_size_current->x = (window->ContentSizeExplicit.x != 0.0f)
                                      ? window->ContentSizeExplicit.x
                                      : AzTrunc64(
                                                window->DC.CursorMaxPos.x
                                                - window->DC.CursorStartPos.x);
    content_size_current->y = (window->ContentSizeExplicit.y != 0.0f)
                                      ? window->ContentSizeExplicit.y
                                      : AzTrunc64(
                                                window->DC.CursorMaxPos.y
                                                - window->DC.CursorStartPos.y);
    content_size_ideal->x = (window->ContentSizeExplicit.x != 0.0f)
                                    ? window->ContentSizeExplicit.x
                                    : AzTrunc64(AzMax(window->DC.CursorMaxPos.x,
                                                      window->DC.IdealMaxPos.x)
                                                - window->DC.CursorStartPos.x);
    content_size_ideal->y = (window->ContentSizeExplicit.y != 0.0f)
                                    ? window->ContentSizeExplicit.y
                                    : AzTrunc64(AzMax(window->DC.CursorMaxPos.y,
                                                      window->DC.IdealMaxPos.y)
                                                - window->DC.CursorStartPos.y);
}

static AzVec2 CalcWindowAutoFitSize(AzWindow* window,
                                    const AzVec2& size_contents)
{
    AzContext& g = *GAz;
    AzStyle& style = g.Style;
    const float decoration_w_without_scrollbars = window->DecoOuterSizeX1
                                                  + window->DecoOuterSizeX2
                                                  - window->ScrollbarSizes.x;
    const float decoration_h_without_scrollbars = window->DecoOuterSizeY1
                                                  + window->DecoOuterSizeY2
                                                  - window->ScrollbarSizes.y;
    AzVec2 size_pad = window->WindowPadding * 2.0f;
    AzVec2 size_desired = size_contents + size_pad
                          + AzVec2(decoration_w_without_scrollbars,
                                   decoration_h_without_scrollbars);

    AzVec2 size_max = AzVec2(FLT_MAX, FLT_MAX);
    if ((window->Flags & AzWindowFlags_ChildWindow) == 0
        || (window->Flags & AzWindowFlags_Popup) != 0)
    {
        if (!window->ViewportOwned)
            size_max = Az::GetMainViewport()->WorkSize
                       - style.DisplaySafeAreaPadding * 2.0f;
        const int monitor_idx = window->ViewportAllowPlatformMonitorExtend;
        if (monitor_idx >= 0 && monitor_idx < g.PlatformIO.Monitors.Size)
            size_max = g.PlatformIO.Monitors[monitor_idx].WorkSize
                       - style.DisplaySafeAreaPadding * 2.0f;
    }

    if (window->Flags & AzWindowFlags_Tooltip)
    {
        return AzMin(size_desired, size_max);
    } else
    {
        AzVec2 size_min = CalcWindowMinSize(window);
        AzVec2 size_auto_fit = AzClamp(size_desired,
                                       AzMin(size_min, size_max),
                                       size_max);

        if ((window->ChildFlags & AzChildFlags_ResizeX)
            && !(window->ChildFlags & AzChildFlags_ResizeY))
            size_auto_fit.y = window->SizeFull.y;
        else if (!(window->ChildFlags & AzChildFlags_ResizeX)
                 && (window->ChildFlags & AzChildFlags_ResizeY))
            size_auto_fit.x = window->SizeFull.x;

        AzVec2 size_auto_fit_after_constraint =
                CalcWindowSizeAfterConstraint(window, size_auto_fit);
        bool will_have_scrollbar_x =
                (size_auto_fit_after_constraint.x - size_pad.x
                                 - decoration_w_without_scrollbars
                         < size_contents.x
                 && !(window->Flags & AzWindowFlags_NoScrollbar)
                 && (window->Flags & AzWindowFlags_HorizontalScrollbar))
                || (window->Flags & AzWindowFlags_AlwaysHorizontalScrollbar);
        bool will_have_scrollbar_y =
                (size_auto_fit_after_constraint.y - size_pad.y
                                 - decoration_h_without_scrollbars
                         < size_contents.y
                 && !(window->Flags & AzWindowFlags_NoScrollbar))
                || (window->Flags & AzWindowFlags_AlwaysVerticalScrollbar);
        if (will_have_scrollbar_x)
            size_auto_fit.y += style.ScrollbarSize;
        if (will_have_scrollbar_y)
            size_auto_fit.x += style.ScrollbarSize;
        return size_auto_fit;
    }
}

AzVec2 Az::CalcWindowNextAutoFitSize(AzWindow* window)
{
    AzVec2 size_contents_current;
    AzVec2 size_contents_ideal;
    CalcWindowContentSizes(window,
                           &size_contents_current,
                           &size_contents_ideal);
    AzVec2 size_auto_fit = CalcWindowAutoFitSize(window, size_contents_ideal);
    AzVec2 size_final = CalcWindowSizeAfterConstraint(window, size_auto_fit);
    return size_final;
}

static AzCol GetWindowBgColorIdx(AzWindow* window)
{
    if (window->Flags & (AzWindowFlags_Tooltip | AzWindowFlags_Popup))
        return AzCol_PopupBg;
    if ((window->Flags & AzWindowFlags_ChildWindow) && !window->DockIsActive)
        return AzCol_ChildBg;
    return AzCol_WindowBg;
}

static void CalcResizePosSizeFromAnyCorner(AzWindow* window,
                                           const AzVec2& corner_target,
                                           const AzVec2& corner_norm,
                                           AzVec2* out_pos,
                                           AzVec2* out_size)
{
    AzVec2 pos_min = AzLerp(corner_target, window->Pos, corner_norm);
    AzVec2 pos_max = AzLerp(window->Pos + window->Size,
                            corner_target,
                            corner_norm);
    AzVec2 size_expected = pos_max - pos_min;
    AzVec2 size_constrained = CalcWindowSizeAfterConstraint(window,
                                                            size_expected);
    *out_pos = pos_min;
    if (corner_norm.x == 0.0f)
        out_pos->x -= (size_constrained.x - size_expected.x);
    if (corner_norm.y == 0.0f)
        out_pos->y -= (size_constrained.y - size_expected.y);
    *out_size = size_constrained;
}

struct AzResizeGripDef
{
    AzVec2 CornerPosN;
    AzVec2 InnerDir;
    int AngleMin12, AngleMax12;
};
static const AzResizeGripDef resize_grip_def[4] = {
        {AzVec2(1, 1), AzVec2(-1, -1), 0, 3},
        {AzVec2(0, 1), AzVec2(+1, -1), 3, 6},
        {AzVec2(0, 0), AzVec2(+1, +1), 6, 9},
        {AzVec2(1, 0), AzVec2(-1, +1), 9, 12}};

struct AzResizeBorderDef
{
    AzVec2 InnerDir;
    AzVec2 SegmentN1, SegmentN2;
    float OuterAngle;
};
static const AzResizeBorderDef resize_border_def[4] = {
        {AzVec2(+1, 0), AzVec2(0, 1), AzVec2(0, 0), AZ_PI * 1.00f},
        {AzVec2(-1, 0), AzVec2(1, 0), AzVec2(1, 1), AZ_PI * 0.00f},
        {AzVec2(0, +1), AzVec2(0, 0), AzVec2(1, 0), AZ_PI * 1.50f},
        {AzVec2(0, -1), AzVec2(1, 1), AzVec2(0, 1), AZ_PI * 0.50f}};

static AzRect GetResizeBorderRect(AzWindow* window,
                                  int border_n,
                                  float perp_padding,
                                  float thickness)
{
    AzRect rect = window->Rect();
    if (thickness == 0.0f)
        rect.Max -= AzVec2(1, 1);
    if (border_n == AzDir_Left)
    {
        return AzRect(rect.Min.x - thickness,
                      rect.Min.y + perp_padding,
                      rect.Min.x + thickness,
                      rect.Max.y - perp_padding);
    }
    if (border_n == AzDir_Right)
    {
        return AzRect(rect.Max.x - thickness,
                      rect.Min.y + perp_padding,
                      rect.Max.x + thickness,
                      rect.Max.y - perp_padding);
    }
    if (border_n == AzDir_Up)
    {
        return AzRect(rect.Min.x + perp_padding,
                      rect.Min.y - thickness,
                      rect.Max.x - perp_padding,
                      rect.Min.y + thickness);
    }
    if (border_n == AzDir_Down)
    {
        return AzRect(rect.Min.x + perp_padding,
                      rect.Max.y - thickness,
                      rect.Max.x - perp_padding,
                      rect.Max.y + thickness);
    }
    AZ_ASSERT(0);
    return AzRect();
}

AzID Az::GetWindowResizeCornerID(AzWindow* window, int n)
{
    AZ_ASSERT(n >= 0 && n < 4);
    AzID id = window->DockIsActive ? window->DockNode->HostWindow->ID
                                   : window->ID;
    id = AzHashStr("#RESIZE", 0, id);
    id = AzHashData(&n, sizeof(int), id);
    return id;
}

AzID Az::GetWindowResizeBorderID(AzWindow* window, AzDir dir)
{
    AZ_ASSERT(dir >= 0 && dir < 4);
    int n = (int)dir + 4;
    AzID id = window->DockIsActive ? window->DockNode->HostWindow->ID
                                   : window->ID;
    id = AzHashStr("#RESIZE", 0, id);
    id = AzHashData(&n, sizeof(int), id);
    return id;
}

static int Az::UpdateWindowManualResize(AzWindow* window,
                                        const AzVec2& size_auto_fit,
                                        int* border_hovered,
                                        int* border_held,
                                        int resize_grip_count,
                                        AzU32 resize_grip_col[4],
                                        const AzRect& visibility_rect)
{
    AzContext& g = *GAz;
    AzWindowFlags flags = window->Flags;

    if ((flags & AzWindowFlags_NoResize)
        || (flags & AzWindowFlags_AlwaysAutoResize)
        || window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
        return false;
    if (window->WasActive == false)

        return false;

    int ret_auto_fit_mask = 0x00;
    const float grip_draw_size = AZ_TRUNC(
            AzMax(g.FontSize * 1.35f,
                  window->WindowRounding + 1.0f + g.FontSize * 0.2f));
    const float grip_hover_inner_size = (resize_grip_count > 0)
                                                ? AZ_TRUNC(grip_draw_size
                                                           * 0.75f)
                                                : 0.0f;
    const float grip_hover_outer_size = g.WindowsBorderHoverPadding;

    AzRect clamp_rect = visibility_rect;
    const bool window_move_from_title_bar =
            g.IO.ConfigWindowsMoveFromTitleBarOnly
            && !(window->Flags & AzWindowFlags_NoTitleBar);
    if (window_move_from_title_bar)
        clamp_rect.Min.y -= window->TitleBarHeight;

    AzVec2 pos_target(FLT_MAX, FLT_MAX);
    AzVec2 size_target(FLT_MAX, FLT_MAX);

    const bool clip_with_viewport_rect =
            !(g.IO.BackendFlags & AzBackendFlags_HasMouseHoveredViewport)
            || (g.IO.MouseHoveredViewport != window->ViewportId)
            || !(window->Viewport->Flags & AzViewportFlags_NoDecoration);
    if (clip_with_viewport_rect)
        window->ClipRect = window->Viewport->GetMainRect();

    window->DC.NavLayerCurrent = AzNavLayer_Menu;

    PushID("#RESIZE");
    for (int resize_grip_n = 0; resize_grip_n < resize_grip_count;
         resize_grip_n++)
    {
        const AzResizeGripDef& def = resize_grip_def[resize_grip_n];
        const AzVec2 corner = AzLerp(window->Pos,
                                     window->Pos + window->Size,
                                     def.CornerPosN);

        bool hovered, held;
        AzRect resize_rect(corner - def.InnerDir * grip_hover_outer_size,
                           corner + def.InnerDir * grip_hover_inner_size);
        if (resize_rect.Min.x > resize_rect.Max.x)
            AzSwap(resize_rect.Min.x, resize_rect.Max.x);
        if (resize_rect.Min.y > resize_rect.Max.y)
            AzSwap(resize_rect.Min.y, resize_rect.Max.y);
        AzID resize_grip_id = window->GetID(resize_grip_n);
        ItemAdd(resize_rect, resize_grip_id, NULL, AzItemFlags_NoNav);
        ButtonBehavior(resize_rect,
                       resize_grip_id,
                       &hovered,
                       &held,
                       AzButtonFlags_FlattenChildren
                               | AzButtonFlags_NoNavFocus);

        if (hovered || held)
            SetMouseCursor((resize_grip_n & 1) ? AzMouseCursor_ResizeNESW
                                               : AzMouseCursor_ResizeNWSE);

        if (held && g.IO.MouseDoubleClicked[0])
        {
            size_target = CalcWindowSizeAfterConstraint(window, size_auto_fit);
            ret_auto_fit_mask = 0x03;
            ClearActiveID();
        } else if (held)
        {
            AzVec2 clamp_min = AzVec2(def.CornerPosN.x == 1.0f
                                              ? clamp_rect.Min.x
                                              : -FLT_MAX,
                                      (def.CornerPosN.y == 1.0f
                                       || (def.CornerPosN.y == 0.0f
                                           && window_move_from_title_bar))
                                              ? clamp_rect.Min.y
                                              : -FLT_MAX);
            AzVec2 clamp_max = AzVec2(
                    def.CornerPosN.x == 0.0f ? clamp_rect.Max.x : +FLT_MAX,
                    def.CornerPosN.y == 0.0f ? clamp_rect.Max.y : +FLT_MAX);
            AzVec2 corner_target =
                    g.IO.MousePos - g.ActiveIdClickOffset
                    + AzLerp(def.InnerDir * grip_hover_outer_size,
                             def.InnerDir * -grip_hover_inner_size,
                             def.CornerPosN);

            corner_target = AzClamp(corner_target, clamp_min, clamp_max);
            CalcResizePosSizeFromAnyCorner(window,
                                           corner_target,
                                           def.CornerPosN,
                                           &pos_target,
                                           &size_target);
        }

        if (resize_grip_n == 0 || held || hovered)
            resize_grip_col[resize_grip_n] = GetColorU32(
                    held      ? AzCol_ResizeGripActive
                    : hovered ? AzCol_ResizeGripHovered
                              : AzCol_ResizeGrip);
    }

    int resize_border_mask = 0x00;
    if (window->Flags & AzWindowFlags_ChildWindow)
        resize_border_mask |=
                ((window->ChildFlags & AzChildFlags_ResizeX) ? 0x02 : 0)
                | ((window->ChildFlags & AzChildFlags_ResizeY) ? 0x08 : 0);
    else
        resize_border_mask = g.IO.ConfigWindowsResizeFromEdges ? 0x0F : 0x00;
    for (int border_n = 0; border_n < 4; border_n++)
    {
        if ((resize_border_mask & (1 << border_n)) == 0)
            continue;
        const AzResizeBorderDef& def = resize_border_def[border_n];
        const AzAxis axis = (border_n == AzDir_Left || border_n == AzDir_Right)
                                    ? AzAxis_X
                                    : AzAxis_Y;

        bool hovered, held;
        AzRect border_rect = GetResizeBorderRect(window,
                                                 border_n,
                                                 grip_hover_inner_size,
                                                 g.WindowsBorderHoverPadding);
        AzID border_id = window->GetID(border_n + 4);
        ItemAdd(border_rect, border_id, NULL, AzItemFlags_NoNav);
        ButtonBehavior(border_rect,
                       border_id,
                       &hovered,
                       &held,
                       AzButtonFlags_FlattenChildren
                               | AzButtonFlags_NoNavFocus);

        if (hovered
            && g.HoveredIdTimer <= WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TAZER)
            hovered = false;
        if (hovered || held)
            SetMouseCursor((axis == AzAxis_X) ? AzMouseCursor_ResizeEW
                                              : AzMouseCursor_ResizeNS);
        if (held && g.IO.MouseDoubleClicked[0])
        {
            if (border_n == 1 || border_n == 3)
            {
                size_target[axis] =
                        CalcWindowSizeAfterConstraint(window,
                                                      size_auto_fit)[axis];
                ret_auto_fit_mask |= (1 << axis);
                hovered = held = false;
            }
            ClearActiveID();
        } else if (held)
        {
            const bool just_scrolled_manually_while_resizing =
                    (g.WheelingWindow != NULL
                     && g.WheelingWindowScrolledFrame == g.FrameCount
                     && IsWindowChildOf(window, g.WheelingWindow, false, true));
            if (g.ActiveIdIsJustActivated
                || just_scrolled_manually_while_resizing)
            {
                g.WindowResizeBorderExpectedRect = border_rect;
                g.WindowResizeRelativeMode = false;
            }
            if ((window->Flags & AzWindowFlags_ChildWindow)
                && memcmp(&g.WindowResizeBorderExpectedRect,
                          &border_rect,
                          sizeof(AzRect))
                           != 0)
                g.WindowResizeRelativeMode = true;

            const AzVec2 border_curr = (window->Pos
                                        + AzMin(def.SegmentN1, def.SegmentN2)
                                                  * window->Size);
            const float border_target_rel_mode_for_axis =
                    border_curr[axis] + g.IO.MouseDelta[axis];
            const float border_target_abs_mode_for_axis =
                    g.IO.MousePos[axis] - g.ActiveIdClickOffset[axis]
                    + g.WindowsBorderHoverPadding;

            AzVec2 border_target = window->Pos;
            border_target[axis] = border_target_abs_mode_for_axis;

            bool ignore_resize = false;
            if (g.WindowResizeRelativeMode)
            {
                border_target[axis] = border_target_rel_mode_for_axis;
                if (g.IO.MouseDelta[axis] == 0.0f
                    || (g.IO.MouseDelta[axis] > 0.0f)
                               == (border_target_rel_mode_for_axis
                                   > border_target_abs_mode_for_axis))
                    ignore_resize = true;
            }

            AzVec2 clamp_min(border_n == AzDir_Right ? clamp_rect.Min.x
                                                     : -FLT_MAX,
                             border_n == AzDir_Down
                                             || (border_n == AzDir_Up
                                                 && window_move_from_title_bar)
                                     ? clamp_rect.Min.y
                                     : -FLT_MAX);
            AzVec2 clamp_max(border_n == AzDir_Left ? clamp_rect.Max.x
                                                    : +FLT_MAX,
                             border_n == AzDir_Up ? clamp_rect.Max.y
                                                  : +FLT_MAX);
            border_target = AzClamp(border_target, clamp_min, clamp_max);
            if (flags & AzWindowFlags_ChildWindow)

            {
                AzWindow* parent_window = window->ParentWindow;
                AzWindowFlags parent_flags = parent_window->Flags;
                AzRect border_limit_rect = parent_window->InnerRect;
                border_limit_rect.Expand(
                        AzVec2(-AzMax(parent_window->WindowPadding.x,
                                      parent_window->WindowBorderSize),
                               -AzMax(parent_window->WindowPadding.y,
                                      parent_window->WindowBorderSize)));
                if ((axis == AzAxis_X)
                    && ((parent_flags
                         & (AzWindowFlags_HorizontalScrollbar
                            | AzWindowFlags_AlwaysHorizontalScrollbar))
                                == 0
                        || (parent_flags & AzWindowFlags_NoScrollbar)))
                    border_target.x = AzClamp(border_target.x,
                                              border_limit_rect.Min.x,
                                              border_limit_rect.Max.x);
                if ((axis == AzAxis_Y)
                    && (parent_flags & AzWindowFlags_NoScrollbar))
                    border_target.y = AzClamp(border_target.y,
                                              border_limit_rect.Min.y,
                                              border_limit_rect.Max.y);
            }
            if (!ignore_resize)
                CalcResizePosSizeFromAnyCorner(window,
                                               border_target,
                                               AzMin(def.SegmentN1,
                                                     def.SegmentN2),
                                               &pos_target,
                                               &size_target);
        }
        if (hovered)
            *border_hovered = border_n;
        if (held)
            *border_held = border_n;
    }
    PopID();

    window->DC.NavLayerCurrent = AzNavLayer_Main;

    if (g.NavWindowingTarget
        && g.NavWindowingTarget->RootWindowDockTree == window)
    {
        AzVec2 nav_resize_dir;
        if (g.NavInputSource == AzInputSource_Keyboard && g.IO.KeyShift)
            nav_resize_dir = GetKeyMagnitude2d(AzKey_LeftArrow,
                                               AzKey_RightArrow,
                                               AzKey_UpArrow,
                                               AzKey_DownArrow);
        if (g.NavInputSource == AzInputSource_Gamepad)
            nav_resize_dir = GetKeyMagnitude2d(AzKey_GamepadDpadLeft,
                                               AzKey_GamepadDpadRight,
                                               AzKey_GamepadDpadUp,
                                               AzKey_GamepadDpadDown);
        if (nav_resize_dir.x != 0.0f || nav_resize_dir.y != 0.0f)
        {
            const float NAV_RESIZE_SPEED = 600.0f;
            const float resize_step = NAV_RESIZE_SPEED * g.IO.DeltaTime
                                      * AzMin(g.IO.DisplayFramebufferScale.x,
                                              g.IO.DisplayFramebufferScale.y);
            g.NavWindowingAccumDeltaSize += nav_resize_dir * resize_step;
            g.NavWindowingAccumDeltaSize = AzMax(g.NavWindowingAccumDeltaSize,
                                                 clamp_rect.Min - window->Pos
                                                         - window->Size);

            g.NavWindowingToggleLayer = false;
            g.NavHighlightItemUnderNav = true;
            resize_grip_col[0] = GetColorU32(AzCol_ResizeGripActive);
            AzVec2 accum_floored = AzTrunc(g.NavWindowingAccumDeltaSize);
            if (accum_floored.x != 0.0f || accum_floored.y != 0.0f)
            {
                size_target = CalcWindowSizeAfterConstraint(
                        window,
                        window->SizeFull + accum_floored);
                g.NavWindowingAccumDeltaSize -= accum_floored;
            }
        }
    }

    const AzVec2 curr_pos = window->Pos;
    const AzVec2 curr_size = window->SizeFull;
    if (size_target.x != FLT_MAX
        && (window->Size.x != size_target.x
            || window->SizeFull.x != size_target.x))
        window->Size.x = window->SizeFull.x = size_target.x;
    if (size_target.y != FLT_MAX
        && (window->Size.y != size_target.y
            || window->SizeFull.y != size_target.y))
        window->Size.y = window->SizeFull.y = size_target.y;
    if (pos_target.x != FLT_MAX && window->Pos.x != AzTrunc(pos_target.x))
        window->Pos.x = AzTrunc(pos_target.x);
    if (pos_target.y != FLT_MAX && window->Pos.y != AzTrunc(pos_target.y))
        window->Pos.y = AzTrunc(pos_target.y);
    if (curr_pos.x != window->Pos.x || curr_pos.y != window->Pos.y
        || curr_size.x != window->SizeFull.x
        || curr_size.y != window->SizeFull.y)
        MarkIniSettingsDirty(window);

    if (*border_held != -1)
        g.WindowResizeBorderExpectedRect =
                GetResizeBorderRect(window,
                                    *border_held,
                                    grip_hover_inner_size,
                                    g.WindowsBorderHoverPadding);

    return ret_auto_fit_mask;
}

static inline void ClampWindowPos(AzWindow* window,
                                  const AzRect& visibility_rect)
{
    AzContext& g = *GAz;
    AzVec2 size_for_clamping = window->Size;
    if (g.IO.ConfigWindowsMoveFromTitleBarOnly && window->DockNodeAsHost)
        size_for_clamping.y = Az::GetFrameHeight();

    else if (g.IO.ConfigWindowsMoveFromTitleBarOnly
             && !(window->Flags & AzWindowFlags_NoTitleBar))
        size_for_clamping.y = window->TitleBarHeight;
    window->Pos = AzClamp(window->Pos,
                          visibility_rect.Min - size_for_clamping,
                          visibility_rect.Max);
}

static void RenderWindowOuterSingleBorder(AzWindow* window,
                                          int border_n,
                                          AzU32 border_col,
                                          float border_size)
{
    const AzResizeBorderDef& def = resize_border_def[border_n];
    const float rounding = window->WindowRounding;
    const AzRect border_r =
            GetResizeBorderRect(window, border_n, rounding, 0.0f);
    window->DrawList->PathArcTo(
            AzLerp(border_r.Min, border_r.Max, def.SegmentN1)
                    + AzVec2(0.5f, 0.5f) + def.InnerDir * rounding,
            rounding,
            def.OuterAngle - AZ_PI * 0.25f,
            def.OuterAngle);
    window->DrawList->PathArcTo(
            AzLerp(border_r.Min, border_r.Max, def.SegmentN2)
                    + AzVec2(0.5f, 0.5f) + def.InnerDir * rounding,
            rounding,
            def.OuterAngle,
            def.OuterAngle + AZ_PI * 0.25f);
    window->DrawList->PathStroke(border_col, AzDrawFlags_None, border_size);
}

static void Az::RenderWindowOuterBorders(AzWindow* window)
{
    AzContext& g = *GAz;
    const float border_size = window->WindowBorderSize;
    const AzU32 border_col = GetColorU32(AzCol_Border);
    if (border_size > 0.0f && (window->Flags & AzWindowFlags_NoBackground) == 0)
        window->DrawList->AddRect(window->Pos,
                                  window->Pos + window->Size,
                                  border_col,
                                  window->WindowRounding,
                                  0,
                                  window->WindowBorderSize);
    else if (border_size > 0.0f)
    {
        if (window->ChildFlags & AzChildFlags_ResizeX)

            RenderWindowOuterSingleBorder(window, 1, border_col, border_size);
        if (window->ChildFlags & AzChildFlags_ResizeY)
            RenderWindowOuterSingleBorder(window, 3, border_col, border_size);
    }
    if (window->ResizeBorderHovered != -1 || window->ResizeBorderHeld != -1)
    {
        const int border_n = (window->ResizeBorderHeld != -1)
                                     ? window->ResizeBorderHeld
                                     : window->ResizeBorderHovered;
        const AzU32 border_col_resizing = GetColorU32(
                (window->ResizeBorderHeld != -1) ? AzCol_SeparatorActive
                                                 : AzCol_SeparatorHovered);
        RenderWindowOuterSingleBorder(window,
                                      border_n,
                                      border_col_resizing,
                                      AzMax(2.0f, window->WindowBorderSize));
    }
    if (g.Style.FrameBorderSize > 0
        && !(window->Flags & AzWindowFlags_NoTitleBar) && !window->DockIsActive)
    {
        float y = window->Pos.y + window->TitleBarHeight - 1;
        window->DrawList->AddLine(
                AzVec2(window->Pos.x + border_size * 0.5f, y),
                AzVec2(window->Pos.x + window->Size.x - border_size * 0.5f, y),
                border_col,
                g.Style.FrameBorderSize);
    }
}

void Az::RenderWindowDecorations(AzWindow* window,
                                 const AzRect& title_bar_rect,
                                 bool title_bar_is_highlight,
                                 bool handle_borders_and_resize_grips,
                                 int resize_grip_count,
                                 const AzU32 resize_grip_col[4],
                                 float resize_grip_draw_size)
{
    AzContext& g = *GAz;
    AzStyle& style = g.Style;
    AzWindowFlags flags = window->Flags;

    AZ_ASSERT(window->BeginCount == 0);
    window->SkipItems = false;
    window->DC.NavLayerCurrent = AzNavLayer_Menu;

    const float window_rounding = window->WindowRounding;
    const float window_border_size = window->WindowBorderSize;
    if (window->Collapsed)
    {
        const float backup_border_size = style.FrameBorderSize;
        g.Style.FrameBorderSize = window->WindowBorderSize;
        AzU32 title_bar_col = GetColorU32(
                (title_bar_is_highlight && g.NavCursorVisible)
                        ? AzCol_TitleBgActive
                        : AzCol_TitleBgCollapsed);
        if (window->ViewportOwned)
            title_bar_col |= AZ_COL32_A_MASK;

        RenderFrame(title_bar_rect.Min,
                    title_bar_rect.Max,
                    title_bar_col,
                    true,
                    window_rounding);
        g.Style.FrameBorderSize = backup_border_size;
    } else
    {
        if (!(flags & AzWindowFlags_NoBackground))
        {
            bool is_docking_transparent_payload = false;
            if (g.DragDropActive
                && (g.FrameCount - g.DragDropAcceptFrameCount) <= 1
                && g.IO.ConfigDockingTransparentPayload)
                if (g.DragDropPayload.IsDataType(AZ_PAYLOAD_TYPE_WINDOW)
                    && *(AzWindow**)g.DragDropPayload.Data == window)
                    is_docking_transparent_payload = true;

            AzU32 bg_col = GetColorU32(GetWindowBgColorIdx(window));
            if (window->ViewportOwned)
            {
                bg_col |= AZ_COL32_A_MASK;
                if (is_docking_transparent_payload)
                    window->Viewport->Alpha *=
                            DOCKING_TRANSPARENT_PAYLOAD_ALPHA;
            } else
            {
                bool override_alpha = false;
                float alpha = 1.0f;
                if (g.NextWindowData.HasFlags
                    & AzNextWindowDataFlags_HasBgAlpha)
                {
                    alpha = g.NextWindowData.BgAlphaVal;
                    override_alpha = true;
                }
                if (is_docking_transparent_payload)
                {
                    alpha *= DOCKING_TRANSPARENT_PAYLOAD_ALPHA;

                    override_alpha = true;
                }
                if (override_alpha)
                    bg_col = (bg_col & ~AZ_COL32_A_MASK)
                             | (AZ_F32_TO_INT8_SAT(alpha) << AZ_COL32_A_SHIFT);
            }

            if (window->DockIsActive)
                window->DockNode->LastBgColor = bg_col;
            AzDrawList* bg_draw_list =
                    window->DockIsActive
                            ? window->DockNode->HostWindow->DrawList
                            : window->DrawList;
            if (window->DockIsActive || (flags & AzWindowFlags_DockNodeHost))
                bg_draw_list->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_BG);
            bg_draw_list->AddRectFilled(
                    window->Pos + AzVec2(0, window->TitleBarHeight),
                    window->Pos + window->Size,
                    bg_col,
                    window_rounding,
                    (flags & AzWindowFlags_NoTitleBar)
                            ? 0
                            : AzDrawFlags_RoundCornersBottom);
            if (window->DockIsActive || (flags & AzWindowFlags_DockNodeHost))
                bg_draw_list->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_FG);
        }
        if (window->DockIsActive)
            window->DockNode->IsBgDrawnThisFrame = true;

        if (!(flags & AzWindowFlags_NoTitleBar) && !window->DockIsActive)
        {
            AzU32 title_bar_col = GetColorU32(title_bar_is_highlight
                                                      ? AzCol_TitleBgActive
                                                      : AzCol_TitleBg);
            if (window->ViewportOwned)
                title_bar_col |= AZ_COL32_A_MASK;
            window->DrawList->AddRectFilled(title_bar_rect.Min,
                                            title_bar_rect.Max,
                                            title_bar_col,
                                            window_rounding,
                                            AzDrawFlags_RoundCornersTop);
        }

        if (flags & AzWindowFlags_MenuBar)
        {
            AzRect menu_bar_rect = window->MenuBarRect();
            menu_bar_rect.ClipWith(window->Rect());

            window->DrawList->AddRectFilled(menu_bar_rect.Min,
                                            menu_bar_rect.Max,
                                            GetColorU32(AzCol_MenuBarBg),
                                            (flags & AzWindowFlags_NoTitleBar)
                                                    ? window_rounding
                                                    : 0.0f,
                                            AzDrawFlags_RoundCornersTop);
            if (style.FrameBorderSize > 0.0f
                && menu_bar_rect.Max.y < window->Pos.y + window->Size.y)
                window->DrawList->AddLine(
                        menu_bar_rect.GetBL()
                                + AzVec2(window_border_size * 0.5f, 0.0f),
                        menu_bar_rect.GetBR()
                                - AzVec2(window_border_size * 0.5f, 0.0f),
                        GetColorU32(AzCol_Border),
                        style.FrameBorderSize);
        }

        AzDockNode* node = window->DockNode;
        if (window->DockIsActive && node->IsHiddenTabBar()
            && !node->IsNoTabBar())
        {
            float unhide_sz_draw = AzTrunc(g.FontSize * 0.70f);
            float unhide_sz_hit = AzTrunc(g.FontSize * 0.55f);
            AzVec2 p = node->Pos;
            AzRect r(p, p + AzVec2(unhide_sz_hit, unhide_sz_hit));
            AzID unhide_id = window->GetID("#UNHIDE");
            KeepAliveID(unhide_id);
            bool hovered, held;
            if (ButtonBehavior(r,
                               unhide_id,
                               &hovered,
                               &held,
                               AzButtonFlags_FlattenChildren))
                node->WantHiddenTabBarToggle = true;
            else if (held && IsMouseDragging(0))
                StartMouseMovingWindowOrNode(window, node, true);

            AzU32 col = GetColorU32(
                    ((held && hovered) || (node->IsFocused && !hovered))
                            ? AzCol_ButtonActive
                    : hovered ? AzCol_ButtonHovered
                              : AzCol_Button);
            window->DrawList->AddTriangleFilled(
                    p,
                    p + AzVec2(unhide_sz_draw, 0.0f),
                    p + AzVec2(0.0f, unhide_sz_draw),
                    col);
        }

        if (window->ScrollbarX)
            Scrollbar(AzAxis_X);
        if (window->ScrollbarY)
            Scrollbar(AzAxis_Y);

        if (handle_borders_and_resize_grips
            && !(flags & AzWindowFlags_NoResize))
        {
            for (int resize_grip_n = 0; resize_grip_n < resize_grip_count;
                 resize_grip_n++)
            {
                const AzU32 col = resize_grip_col[resize_grip_n];
                if ((col & AZ_COL32_A_MASK) == 0)
                    continue;
                const AzResizeGripDef& grip = resize_grip_def[resize_grip_n];
                const AzVec2 corner = AzLerp(window->Pos,
                                             window->Pos + window->Size,
                                             grip.CornerPosN);
                const float border_inner = AZ_ROUND(window_border_size * 0.5f);
                window->DrawList->PathLineTo(
                        corner
                        + grip.InnerDir
                                  * ((resize_grip_n & 1)
                                             ? AzVec2(border_inner,
                                                      resize_grip_draw_size)
                                             : AzVec2(resize_grip_draw_size,
                                                      border_inner)));
                window->DrawList->PathLineTo(
                        corner
                        + grip.InnerDir
                                  * ((resize_grip_n & 1)
                                             ? AzVec2(resize_grip_draw_size,
                                                      border_inner)
                                             : AzVec2(border_inner,
                                                      resize_grip_draw_size)));
                window->DrawList->PathArcToFast(
                        AzVec2(corner.x
                                       + grip.InnerDir.x
                                                 * (window_rounding
                                                    + border_inner),
                               corner.y
                                       + grip.InnerDir.y
                                                 * (window_rounding
                                                    + border_inner)),
                        window_rounding,
                        grip.AngleMin12,
                        grip.AngleMax12);
                window->DrawList->PathFillConvex(col);
            }
        }

        if (handle_borders_and_resize_grips && !window->DockNodeAsHost)
            RenderWindowOuterBorders(window);
    }
    window->DC.NavLayerCurrent = AzNavLayer_Main;
}

void Az::RenderWindowTitleBarContents(AzWindow* window,
                                      const AzRect& title_bar_rect,
                                      const char* name,
                                      bool* p_open)
{
    AzContext& g = *GAz;
    AzStyle& style = g.Style;
    AzWindowFlags flags = window->Flags;

    const bool has_close_button = (p_open != NULL);
    const bool has_collapse_button = !(flags & AzWindowFlags_NoCollapse)
                                     && (style.WindowMenuButtonPosition
                                         != AzDir_None);

    const AzItemFlags item_flags_backup = g.CurrentItemFlags;
    g.CurrentItemFlags |= AzItemFlags_NoNavDefaultFocus;
    window->DC.NavLayerCurrent = AzNavLayer_Menu;

    float pad_l = style.FramePadding.x;
    float pad_r = style.FramePadding.x;
    float button_sz = g.FontSize;
    AzVec2 close_button_pos;
    AzVec2 collapse_button_pos;
    if (has_close_button)
    {
        close_button_pos = AzVec2(title_bar_rect.Max.x - pad_r - button_sz,
                                  title_bar_rect.Min.y + style.FramePadding.y);
        pad_r += button_sz + style.ItemInnerSpacing.x;
    }
    if (has_collapse_button && style.WindowMenuButtonPosition == AzDir_Right)
    {
        collapse_button_pos = AzVec2(title_bar_rect.Max.x - pad_r - button_sz,
                                     title_bar_rect.Min.y
                                             + style.FramePadding.y);
        pad_r += button_sz + style.ItemInnerSpacing.x;
    }
    if (has_collapse_button && style.WindowMenuButtonPosition == AzDir_Left)
    {
        collapse_button_pos = AzVec2(title_bar_rect.Min.x + pad_l,
                                     title_bar_rect.Min.y
                                             + style.FramePadding.y);
        pad_l += button_sz + style.ItemInnerSpacing.x;
    }

    if (has_collapse_button)
        if (CollapseButton(window->GetID("#COLLAPSE"),
                           collapse_button_pos,
                           NULL))
            window->WantCollapseToggle = true;

    if (has_close_button)
        if (CloseButton(window->GetID("#CLOSE"), close_button_pos))
            *p_open = false;

    window->DC.NavLayerCurrent = AzNavLayer_Main;
    g.CurrentItemFlags = item_flags_backup;

    const float marker_size_x = (flags & AzWindowFlags_UnsavedDocument)
                                        ? button_sz * 0.80f
                                        : 0.0f;
    const AzVec2 text_size = CalcTextSize(name, NULL, true)
                             + AzVec2(marker_size_x, 0.0f);

    if (pad_l > style.FramePadding.x)
        pad_l += g.Style.ItemInnerSpacing.x;
    if (pad_r > style.FramePadding.x)
        pad_r += g.Style.ItemInnerSpacing.x;
    if (style.WindowTitleAlign.x > 0.0f && style.WindowTitleAlign.x < 1.0f)
    {
        float centerness = AzSaturate(
                1.0f - AzFabs(style.WindowTitleAlign.x - 0.5f) * 2.0f);
        float pad_extend = AzMin(AzMax(pad_l, pad_r),
                                 title_bar_rect.GetWidth() - pad_l - pad_r
                                         - text_size.x);
        pad_l = AzMax(pad_l, pad_extend * centerness);
        pad_r = AzMax(pad_r, pad_extend * centerness);
    }

    AzRect layout_r(title_bar_rect.Min.x + pad_l,
                    title_bar_rect.Min.y,
                    title_bar_rect.Max.x - pad_r,
                    title_bar_rect.Max.y);
    AzRect clip_r(layout_r.Min.x,
                  layout_r.Min.y,
                  AzMin(layout_r.Max.x + g.Style.ItemInnerSpacing.x,
                        title_bar_rect.Max.x),
                  layout_r.Max.y);
    if (flags & AzWindowFlags_UnsavedDocument)
    {
        AzVec2 marker_pos;
        marker_pos.x = AzClamp(layout_r.Min.x
                                       + (layout_r.GetWidth() - text_size.x)
                                                 * style.WindowTitleAlign.x
                                       + text_size.x,
                               layout_r.Min.x,
                               layout_r.Max.x);
        marker_pos.y = (layout_r.Min.y + layout_r.Max.y) * 0.5f;
        if (marker_pos.x > layout_r.Min.x)
        {
            RenderBullet(window->DrawList, marker_pos, GetColorU32(AzCol_Text));
            clip_r.Max.x = AzMin(clip_r.Max.x,
                                 marker_pos.x - (int)(marker_size_x * 0.5f));
        }
    }

    RenderTextClipped(layout_r.Min,
                      layout_r.Max,
                      name,
                      NULL,
                      &text_size,
                      style.WindowTitleAlign,
                      &clip_r);
}

void Az::UpdateWindowParentAndRootLinks(AzWindow* window,
                                        AzWindowFlags flags,
                                        AzWindow* parent_window)
{
    window->ParentWindow = parent_window;
    window->RootWindow = window->RootWindowPopupTree =
            window->RootWindowDockTree =
                    window->RootWindowForTitleBarHighlight =
                            window->RootWindowForNav = window;
    if (parent_window && (flags & AzWindowFlags_ChildWindow)
        && !(flags & AzWindowFlags_Tooltip))
    {
        window->RootWindowDockTree = parent_window->RootWindowDockTree;
        if (!window->DockIsActive
            && !(parent_window->Flags & AzWindowFlags_DockNodeHost))
            window->RootWindow = parent_window->RootWindow;
    }
    if (parent_window && (flags & AzWindowFlags_Popup))
        window->RootWindowPopupTree = parent_window->RootWindowPopupTree;
    if (parent_window && !(flags & AzWindowFlags_Modal)
        && (flags & (AzWindowFlags_ChildWindow | AzWindowFlags_Popup)))
        window->RootWindowForTitleBarHighlight =
                parent_window->RootWindowForTitleBarHighlight;
    while (window->RootWindowForNav->ChildFlags & AzChildFlags_NavFlattened)
    {
        AZ_ASSERT(window->RootWindowForNav->ParentWindow != NULL);
        window->RootWindowForNav = window->RootWindowForNav->ParentWindow;
    }
}

void Az::UpdateWindowSkipRefresh(AzWindow* window)
{
    AzContext& g = *GAz;
    window->SkipRefresh = false;
    if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasRefreshPolicy)
        == 0)
        return;
    if (g.NextWindowData.RefreshFlagsVal
        & AzWindowRefreshFlags_TryToAvoidRefresh)
    {
        if (window->Appearing)
            return;
        if (window->Hidden)
            return;
        if ((g.NextWindowData.RefreshFlagsVal
             & AzWindowRefreshFlags_RefreshOnHover)
            && g.HoveredWindow)
            if (window->RootWindow == g.HoveredWindow->RootWindow
                || IsWindowWithinBeginStackOf(g.HoveredWindow->RootWindow,
                                              window))
                return;
        if ((g.NextWindowData.RefreshFlagsVal
             & AzWindowRefreshFlags_RefreshOnFocus)
            && g.NavWindow)
            if (window->RootWindow == g.NavWindow->RootWindow
                || IsWindowWithinBeginStackOf(g.NavWindow->RootWindow, window))
                return;
        window->DrawList = NULL;
        window->SkipRefresh = true;
    }
}

static void SetWindowActiveForSkipRefresh(AzWindow* window)
{
    window->Active = true;
    for (AzWindow* child : window->DC.ChildWindows)
        if (!child->Hidden)
        {
            child->Active = child->SkipRefresh = true;
            SetWindowActiveForSkipRefresh(child);
        }
}

bool Az::Begin(const char* name, bool* p_open, AzWindowFlags flags)
{
    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    AZ_ASSERT(name != NULL && name[0] != '\0');
    AZ_ASSERT(g.WithinFrameScope);
    AZ_ASSERT(g.FrameCountEnded != g.FrameCount);

    AzWindow* window = FindWindowByName(name);
    const bool window_just_created = (window == NULL);
    if (window_just_created)
        window = CreateNewWindow(name, flags);

    if (g.DebugBreakInWindow == window->ID)
        AZ_DEBUG_BREAK();

    if ((flags & AzWindowFlags_NoInputs) == AzWindowFlags_NoInputs)
        flags |= AzWindowFlags_NoMove | AzWindowFlags_NoResize;

    const int current_frame = g.FrameCount;
    const bool first_begin_of_the_frame = (window->LastFrameActive
                                           != current_frame);
    window->IsFallbackWindow = (g.CurrentWindowStack.Size == 0
                                && g.WithinFrameScopeWithAzplicitWindow);

    bool window_just_activated_by_user = (window->LastFrameActive
                                          < current_frame - 1);

    if (flags & AzWindowFlags_Popup)
    {
        AzPopupData& popup_ref = g.OpenPopupStack[g.BeginPopupStack.Size];
        window_just_activated_by_user |= (window->PopupId != popup_ref.PopupId);

        window_just_activated_by_user |= (window != popup_ref.Window);
    }

    const bool window_was_appearing = window->Appearing;
    if (first_begin_of_the_frame)
    {
        UpdateWindowInFocusOrderList(window, window_just_created, flags);
        window->Appearing = window_just_activated_by_user;
        if (window->Appearing)
            SetWindowConditionAllowFlags(window, AzCond_Appearing, true);
        window->FlagsPreviousFrame = window->Flags;
        window->Flags = (AzWindowFlags)flags;
        window->ChildFlags = (g.NextWindowData.HasFlags
                              & AzNextWindowDataFlags_HasChildFlags)
                                     ? g.NextWindowData.ChildFlags
                                     : 0;
        window->LastFrameActive = current_frame;
        window->LastTimeActive = (float)g.Time;
        window->BeginOrderWithinParent = 0;
        window->BeginOrderWithinContext = (short)(g.WindowsActiveCount++);
    } else
    {
        flags = window->Flags;
    }

    AZ_ASSERT(window->DockNode == NULL || window->DockNodeAsHost == NULL);
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasDock)
        SetWindowDock(window,
                      g.NextWindowData.DockId,
                      g.NextWindowData.DockCond);
    if (first_begin_of_the_frame)
    {
        bool has_dock_node = (window->DockId != 0 || window->DockNode != NULL);
        bool new_auto_dock_node = !has_dock_node
                                  && GetWindowAlwaysWantOwnTabBar(window);
        bool dock_node_was_visible = window->DockNodeIsVisible;
        bool dock_tab_was_visible = window->DockTabIsVisible;
        if (has_dock_node || new_auto_dock_node)
        {
            BeginDocked(window, p_open);
            flags = window->Flags;
            if (window->DockIsActive)
            {
                AZ_ASSERT(window->DockNode != NULL);
                g.NextWindowData.HasFlags &=
                        ~AzNextWindowDataFlags_HasSizeConstraint;
            }

            if (window->DockTabIsVisible && !dock_tab_was_visible
                && dock_node_was_visible && !window->Appearing
                && !window_was_appearing)
            {
                window->Appearing = true;
                SetWindowConditionAllowFlags(window, AzCond_Appearing, true);
            }
        } else
        {
            window->DockIsActive = window->DockNodeIsVisible =
                    window->DockTabIsVisible = false;
        }
    }

    AzWindow* parent_window_in_stack =
            (window->DockIsActive && window->DockNode->HostWindow)
                    ? window->DockNode->HostWindow
            : g.CurrentWindowStack.empty() ? NULL
                                           : g.CurrentWindowStack.back().Window;
    AzWindow* parent_window = first_begin_of_the_frame
                                      ? ((flags
                                          & (AzWindowFlags_ChildWindow
                                             | AzWindowFlags_Popup))
                                                 ? parent_window_in_stack
                                                 : NULL)
                                      : window->ParentWindow;
    AZ_ASSERT(parent_window != NULL || !(flags & AzWindowFlags_ChildWindow));

    if (window->IDStack.Size == 0)
        window->IDStack.push_back(window->ID);

    g.CurrentWindow = window;
    g.CurrentWindowStack.resize(g.CurrentWindowStack.Size + 1);
    AzWindowStackData& window_stack_data = g.CurrentWindowStack.back();
    window_stack_data.Window = window;
    window_stack_data.ParentLastItemDataBackup = g.LastItemData;
    window_stack_data.DisabledOverrideReenable = (flags & AzWindowFlags_Tooltip)
                                                 && (g.CurrentItemFlags
                                                     & AzItemFlags_Disabled);
    window_stack_data.DisabledOverrideReenableAlphaBackup = 0.0f;
    ErrorRecoveryStoreState(&window_stack_data.StackSizesInBegin);
    g.StackSizesInBeginForCurrentWindow = &window_stack_data.StackSizesInBegin;
    if (flags & AzWindowFlags_ChildMenu)
        g.BeginMenuDepth++;

    if (first_begin_of_the_frame)
    {
        UpdateWindowParentAndRootLinks(window, flags, parent_window);
        window->ParentWindowInBeginStack = parent_window_in_stack;

        window->ParentWindowForFocusRoute = (window->RootWindow != window)
                                                    ? parent_window_in_stack
                                                    : NULL;
        if (window->ParentWindowForFocusRoute == NULL
            && window->DockNode != NULL)
            if (window->DockNode->MergedFlags
                & AzDockNodeFlags_DockedWindowsInFocusRoute)
                window->ParentWindowForFocusRoute =
                        window->DockNode->HostWindow;

        if (window->WindowClass.FocusRouteParentWindowId != 0)
        {
            window->ParentWindowForFocusRoute = FindWindowByID(
                    window->WindowClass.FocusRouteParentWindowId);
            AZ_ASSERT(window->ParentWindowForFocusRoute != 0);
        }

        window->FontWindowScaleParents =
                parent_window ? parent_window->FontWindowScaleParents
                                        * parent_window->FontWindowScale
                              : 1.0f;
    }

    PushFocusScope((window->ChildFlags & AzChildFlags_NavFlattened)
                           ? g.CurrentFocusScopeId
                           : window->ID);
    window->NavRootFocusScopeId = g.CurrentFocusScopeId;

    if (flags & AzWindowFlags_Popup)
    {
        AzPopupData& popup_ref = g.OpenPopupStack[g.BeginPopupStack.Size];
        popup_ref.Window = window;
        popup_ref.ParentNavLayer = parent_window_in_stack->DC.NavLayerCurrent;
        g.BeginPopupStack.push_back(popup_ref);
        window->PopupId = popup_ref.PopupId;
    }

    bool window_pos_set_by_api = false;
    bool window_size_x_set_by_api = false, window_size_y_set_by_api = false;
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasPos)
    {
        window_pos_set_by_api = (window->SetWindowPosAllowFlags
                                 & g.NextWindowData.PosCond)
                                != 0;
        if (window_pos_set_by_api
            && AzLengthSqr(g.NextWindowData.PosPivotVal) > 0.00001f)
        {
            window->SetWindowPosVal = g.NextWindowData.PosVal;
            window->SetWindowPosPivot = g.NextWindowData.PosPivotVal;
            window->SetWindowPosAllowFlags &= ~(
                    AzCond_Once | AzCond_FirstUseEver | AzCond_Appearing);
        } else
        {
            SetWindowPos(window,
                         g.NextWindowData.PosVal,
                         g.NextWindowData.PosCond);
        }
    }
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSize)
    {
        window_size_x_set_by_api = (window->SetWindowSizeAllowFlags
                                    & g.NextWindowData.SizeCond)
                                           != 0
                                   && (g.NextWindowData.SizeVal.x > 0.0f);
        window_size_y_set_by_api = (window->SetWindowSizeAllowFlags
                                    & g.NextWindowData.SizeCond)
                                           != 0
                                   && (g.NextWindowData.SizeVal.y > 0.0f);
        if ((window->ChildFlags & AzChildFlags_ResizeX)
            && (window->SetWindowSizeAllowFlags & AzCond_FirstUseEver) == 0)
            g.NextWindowData.SizeVal.x = window->SizeFull.x;
        if ((window->ChildFlags & AzChildFlags_ResizeY)
            && (window->SetWindowSizeAllowFlags & AzCond_FirstUseEver) == 0)
            g.NextWindowData.SizeVal.y = window->SizeFull.y;
        SetWindowSize(window,
                      g.NextWindowData.SizeVal,
                      g.NextWindowData.SizeCond);
    }
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasScroll)
    {
        if (g.NextWindowData.ScrollVal.x >= 0.0f)
        {
            window->ScrollTarget.x = g.NextWindowData.ScrollVal.x;
            window->ScrollTargetCenterRatio.x = 0.0f;
        }
        if (g.NextWindowData.ScrollVal.y >= 0.0f)
        {
            window->ScrollTarget.y = g.NextWindowData.ScrollVal.y;
            window->ScrollTargetCenterRatio.y = 0.0f;
        }
    }
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasContentSize)
        window->ContentSizeExplicit = g.NextWindowData.ContentSizeVal;
    else if (first_begin_of_the_frame)
        window->ContentSizeExplicit = AzVec2(0.0f, 0.0f);
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasWindowClass)
        window->WindowClass = g.NextWindowData.WindowClass;
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasCollapsed)
        SetWindowCollapsed(window,
                           g.NextWindowData.CollapsedVal,
                           g.NextWindowData.CollapsedCond);
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasFocus)
        FocusWindow(window);
    if (window->Appearing)
        SetWindowConditionAllowFlags(window, AzCond_Appearing, false);

    UpdateWindowSkipRefresh(window);

    if (window_stack_data.DisabledOverrideReenable
        && window->RootWindow == window)
        BeginDisabledOverrideReenable();

    g.CurrentWindow = NULL;

    if (first_begin_of_the_frame && !window->SkipRefresh)
    {
        const bool window_is_child_tooltip = (flags & AzWindowFlags_ChildWindow)
                                             && (flags & AzWindowFlags_Tooltip);

        const bool window_just_appearing_after_hidden_for_resize =
                (window->HiddenFramesCannotSkipItems > 0);
        window->Active = true;
        window->HasCloseButton = (p_open != NULL);
        window->ClipRect = AzVec4(-FLT_MAX, -FLT_MAX, +FLT_MAX, +FLT_MAX);
        window->IDStack.resize(1);
        window->DrawList->_ResetForNewFrame();
        window->DC.CurrentTableIdx = -1;
        if (flags & AzWindowFlags_DockNodeHost)
        {
            window->DrawList->ChannelsSplit(2);
            window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_FG);
        }

        if (window->MemoryCompacted)
            GcAwakeTransientWindowBuffers(window);

        bool window_title_visible_elsewhere = false;
        if ((window->Viewport && window->Viewport->Window == window)
            || (window->DockIsActive))
            window_title_visible_elsewhere = true;
        else if (g.NavWindowingListWindow != NULL
                 && (flags & AzWindowFlags_NoNavFocus) == 0)
            window_title_visible_elsewhere = true;
        else if (flags & AzWindowFlags_ChildMenu)
            window_title_visible_elsewhere = true;
        if (window_title_visible_elsewhere && !window_just_created
            && strcmp(name, window->Name) != 0)
        {
            size_t buf_len = (size_t)window->NameBufLen;
            window->Name = AzStrdupcpy(window->Name, &buf_len, name);
            window->NameBufLen = (int)buf_len;
        }

        CalcWindowContentSizes(window,
                               &window->ContentSize,
                               &window->ContentSizeIdeal);

        if (window->HiddenFramesCanSkipItems > 0)
            window->HiddenFramesCanSkipItems--;
        if (window->HiddenFramesCannotSkipItems > 0)
            window->HiddenFramesCannotSkipItems--;
        if (window->HiddenFramesForRenderOnly > 0)
            window->HiddenFramesForRenderOnly--;

        if (window_just_created
            && (!window_size_x_set_by_api || !window_size_y_set_by_api))
            window->HiddenFramesCannotSkipItems = 1;

        if (window_just_activated_by_user
            && (flags & (AzWindowFlags_Popup | AzWindowFlags_Tooltip)) != 0)
        {
            window->HiddenFramesCannotSkipItems = 1;
            if (flags & AzWindowFlags_AlwaysAutoResize)
            {
                if (!window_size_x_set_by_api)
                    window->Size.x = window->SizeFull.x = 0.f;
                if (!window_size_y_set_by_api)
                    window->Size.y = window->SizeFull.y = 0.f;
                window->ContentSize = window->ContentSizeIdeal = AzVec2(0.f,
                                                                        0.f);
            }
        }

        WindowSelectViewport(window);
        SetCurrentViewport(window, window->Viewport);
        SetCurrentWindow(window);
        flags = window->Flags;

        if (!window->DockIsActive && (flags & AzWindowFlags_ChildWindow))
            window->WindowBorderSize = style.ChildBorderSize;
        else
            window->WindowBorderSize =
                    ((flags & (AzWindowFlags_Popup | AzWindowFlags_Tooltip))
                     && !(flags & AzWindowFlags_Modal))
                            ? style.PopupBorderSize
                            : style.WindowBorderSize;
        window->WindowPadding = style.WindowPadding;
        if (!window->DockIsActive && (flags & AzWindowFlags_ChildWindow)
            && !(flags & AzWindowFlags_Popup)
            && !(window->ChildFlags & AzChildFlags_AlwaysUseWindowPadding)
            && window->WindowBorderSize == 0.0f)
            window->WindowPadding = AzVec2(0.0f,
                                           (flags & AzWindowFlags_MenuBar)
                                                   ? style.WindowPadding.y
                                                   : 0.0f);

        window->DC.MenuBarOffset.x =
                AzMax(AzMax(window->WindowPadding.x, style.ItemSpacing.x),
                      g.NextWindowData.MenuBarOffsetMinVal.x);
        window->DC.MenuBarOffset.y = g.NextWindowData.MenuBarOffsetMinVal.y;
        window->TitleBarHeight = (flags & AzWindowFlags_NoTitleBar)
                                         ? 0.0f
                                         : g.FontSize
                                                   + g.Style.FramePadding.y
                                                             * 2.0f;
        window->MenuBarHeight = (flags & AzWindowFlags_MenuBar)
                                        ? window->DC.MenuBarOffset.y
                                                  + g.FontSize
                                                  + g.Style.FramePadding.y
                                                            * 2.0f
                                        : 0.0f;
        window->FontRefSize = g.FontSize;

        bool use_current_size_for_scrollbar_x = window_just_created;
        bool use_current_size_for_scrollbar_y = window_just_created;
        if (window_size_x_set_by_api && window->ContentSizeExplicit.x != 0.0f)
            use_current_size_for_scrollbar_x = true;
        if (window_size_y_set_by_api && window->ContentSizeExplicit.y != 0.0f)
            use_current_size_for_scrollbar_y = true;

        if (!(flags & AzWindowFlags_NoTitleBar)
            && !(flags & AzWindowFlags_NoCollapse) && !window->DockIsActive)
        {
            AzRect title_bar_rect = window->TitleBarRect();
            if (g.HoveredWindow == window && g.HoveredId == 0
                && g.HoveredIdPreviousFrame == 0 && g.ActiveId == 0
                && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max))
                if (g.IO.MouseClickedCount[0] == 2
                    && GetKeyOwner(AzKey_MouseLeft) == AzKeyOwner_NoOwner)
                    window->WantCollapseToggle = true;
            if (window->WantCollapseToggle)
            {
                window->Collapsed = !window->Collapsed;
                if (!window->Collapsed)
                    use_current_size_for_scrollbar_y = true;
                MarkIniSettingsDirty(window);
            }
        } else
        {
            window->Collapsed = false;
        }
        window->WantCollapseToggle = false;

        const AzVec2 scrollbar_sizes_from_last_frame = window->ScrollbarSizes;
        window->DecoOuterSizeX1 = 0.0f;
        window->DecoOuterSizeX2 = 0.0f;
        window->DecoOuterSizeY1 = window->TitleBarHeight
                                  + window->MenuBarHeight;
        window->DecoOuterSizeY2 = 0.0f;
        window->ScrollbarSizes = AzVec2(0.0f, 0.0f);

        const AzVec2 size_auto_fit =
                CalcWindowAutoFitSize(window, window->ContentSizeIdeal);
        if ((flags & AzWindowFlags_AlwaysAutoResize) && !window->Collapsed)
        {
            if (!window_size_x_set_by_api)
            {
                window->SizeFull.x = size_auto_fit.x;
                use_current_size_for_scrollbar_x = true;
            }
            if (!window_size_y_set_by_api)
            {
                window->SizeFull.y = size_auto_fit.y;
                use_current_size_for_scrollbar_y = true;
            }
        } else if (window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
        {
            if (!window_size_x_set_by_api && window->AutoFitFramesX > 0)
            {
                window->SizeFull.x = window->AutoFitOnlyGrows
                                             ? AzMax(window->SizeFull.x,
                                                     size_auto_fit.x)
                                             : size_auto_fit.x;
                use_current_size_for_scrollbar_x = true;
            }
            if (!window_size_y_set_by_api && window->AutoFitFramesY > 0)
            {
                window->SizeFull.y = window->AutoFitOnlyGrows
                                             ? AzMax(window->SizeFull.y,
                                                     size_auto_fit.y)
                                             : size_auto_fit.y;
                use_current_size_for_scrollbar_y = true;
            }
            if (!window->Collapsed)
                MarkIniSettingsDirty(window);
        }

        window->SizeFull = CalcWindowSizeAfterConstraint(window,
                                                         window->SizeFull);
        window->Size = window->Collapsed && !(flags & AzWindowFlags_ChildWindow)
                               ? window->TitleBarRect().GetSize()
                               : window->SizeFull;

        if (window_just_activated_by_user)
        {
            window->AutoPosLastDirection = AzDir_None;
            if ((flags & AzWindowFlags_Popup) != 0
                && !(flags & AzWindowFlags_Modal) && !window_pos_set_by_api)

                window->Pos = g.BeginPopupStack.back().OpenPopupPos;
        }

        if (flags & AzWindowFlags_ChildWindow)
        {
            AZ_ASSERT(parent_window && parent_window->Active);
            window->BeginOrderWithinParent =
                    (short)parent_window->DC.ChildWindows.Size;
            parent_window->DC.ChildWindows.push_back(window);
            if (!(flags & AzWindowFlags_Popup) && !window_pos_set_by_api
                && !window_is_child_tooltip)
                window->Pos = parent_window->DC.CursorPos;
        }

        const bool window_pos_with_pivot =
                (window->SetWindowPosVal.x != FLT_MAX
                 && window->HiddenFramesCannotSkipItems == 0);
        if (window_pos_with_pivot)
            SetWindowPos(window,
                         window->SetWindowPosVal
                                 - window->Size * window->SetWindowPosPivot,
                         0);
        else if ((flags & AzWindowFlags_ChildMenu) != 0)
            window->Pos = FindBestWindowPosForPopup(window);
        else if ((flags & AzWindowFlags_Popup) != 0 && !window_pos_set_by_api
                 && window_just_appearing_after_hidden_for_resize)
            window->Pos = FindBestWindowPosForPopup(window);
        else if ((flags & AzWindowFlags_Tooltip) != 0 && !window_pos_set_by_api
                 && !window_is_child_tooltip)
            window->Pos = FindBestWindowPosForPopup(window);

        if (window->ViewportAllowPlatformMonitorExtend >= 0
            && !window->ViewportOwned
            && !(window->Viewport->Flags & AzViewportFlags_IsMinimized))
            if (!window->Viewport->GetMainRect().Contains(window->Rect()))
            {
                window->Viewport =
                        AddUpdateViewport(window,
                                          window->ID,
                                          window->Pos,
                                          window->Size,
                                          AzViewportFlags_NoFocusOnAppearing);

                SetCurrentViewport(window, window->Viewport);
                SetCurrentWindow(window);
            }

        if (window->ViewportOwned)
            WindowSyncOwnedViewport(window, parent_window_in_stack);

        AzRect viewport_rect(window->Viewport->GetMainRect());
        AzRect viewport_work_rect(window->Viewport->GetWorkRect());
        AzVec2 visibility_padding = AzMax(style.DisplayWindowPadding,
                                          style.DisplaySafeAreaPadding);
        AzRect visibility_rect(viewport_work_rect.Min + visibility_padding,
                               viewport_work_rect.Max - visibility_padding);

        if (!window_pos_set_by_api && !(flags & AzWindowFlags_ChildWindow))
        {
            if (!window->ViewportOwned && viewport_rect.GetWidth() > 0
                && viewport_rect.GetHeight() > 0.0f)
            {
                ClampWindowPos(window, visibility_rect);
            } else if (window->ViewportOwned && g.PlatformIO.Monitors.Size > 0)
            {
                if (g.MovingWindow != NULL
                    && window->RootWindowDockTree
                               == g.MovingWindow->RootWindowDockTree)
                {
                    visibility_rect = g.PlatformMonitorsFullWorkRect;
                } else
                {
                    const AzPlatformMonitor* monitor =
                            GetViewportPlatformMonitor(window->Viewport);
                    visibility_rect = AzRect(monitor->WorkPos,
                                             monitor->WorkPos
                                                     + monitor->WorkSize);
                }
                visibility_rect.Expand(-visibility_padding);
                ClampWindowPos(window, visibility_rect);
            }
        }
        window->Pos = AzTrunc(window->Pos);

        if (window->ViewportOwned || window->DockIsActive)
            window->WindowRounding = 0.0f;
        else
            window->WindowRounding = (flags & AzWindowFlags_ChildWindow)
                                             ? style.ChildRounding
                                     : ((flags & AzWindowFlags_Popup)
                                        && !(flags & AzWindowFlags_Modal))
                                             ? style.PopupRounding
                                             : style.WindowRounding;

        bool want_focus = false;
        if (window_just_activated_by_user
            && !(flags & AzWindowFlags_NoFocusOnAppearing))
        {
            if (flags & AzWindowFlags_Popup)
                want_focus = true;
            else if ((window->DockIsActive
                      || (flags & AzWindowFlags_ChildWindow) == 0)
                     && !(flags & AzWindowFlags_Tooltip))
                want_focus = true;
        }

#ifdef AZ_ENABLE_TEST_ENGINE
        if (g.TestEngineHookItems)
        {
            AZ_ASSERT(window->IDStack.Size == 1);
            window->IDStack.Size = 0;

            window->DC.NavLayerCurrent = AzNavLayer_Menu;
            AZ_TEST_ENGINE_ITEM_ADD(window->ID, window->Rect(), NULL);
            AZ_TEST_ENGINE_ITEM_INFO(window->ID,
                                     window->Name,
                                     (g.HoveredWindow == window)
                                             ? AzItemStatusFlags_HoveredRect
                                             : 0);
            window->IDStack.Size = 1;
            window->DC.NavLayerCurrent = AzNavLayer_Main;
        }
#endif

        const bool handle_borders_and_resize_grips = (window->DockNodeAsHost
                                                      || !window->DockIsActive);

        int border_hovered = -1, border_held = -1;
        AzU32 resize_grip_col[4] = {};
        const int resize_grip_count = ((flags & AzWindowFlags_ChildWindow)
                                       && !(flags & AzWindowFlags_Popup))
                                              ? 0
                                      : g.IO.ConfigWindowsResizeFromEdges ? 2
                                                                          : 1;

        const float resize_grip_draw_size = AZ_TRUNC(
                AzMax(g.FontSize * 1.10f,
                      window->WindowRounding + 1.0f + g.FontSize * 0.2f));
        if (handle_borders_and_resize_grips && !window->Collapsed)
            if (int auto_fit_mask =
                        UpdateWindowManualResize(window,
                                                 size_auto_fit,
                                                 &border_hovered,
                                                 &border_held,
                                                 resize_grip_count,
                                                 &resize_grip_col[0],
                                                 visibility_rect))
            {
                if (auto_fit_mask & (1 << AzAxis_X))
                    use_current_size_for_scrollbar_x = true;
                if (auto_fit_mask & (1 << AzAxis_Y))
                    use_current_size_for_scrollbar_y = true;
            }
        window->ResizeBorderHovered = (signed char)border_hovered;
        window->ResizeBorderHeld = (signed char)border_held;

        if (window->ViewportOwned)
        {
            if (!window->Viewport->PlatformRequestMove)
                window->Viewport->Pos = window->Pos;
            if (!window->Viewport->PlatformRequestResize)
                window->Viewport->Size = window->Size;
            window->Viewport->UpdateWorkRect();
            viewport_rect = window->Viewport->GetMainRect();
        }

        window->ViewportPos = window->Viewport->Pos;

        if (!window->Collapsed)
        {
            AzVec2 avail_size_from_current_frame =
                    AzVec2(window->SizeFull.x,
                           window->SizeFull.y
                                   - (window->DecoOuterSizeY1
                                      + window->DecoOuterSizeY2));
            AzVec2 avail_size_from_last_frame =
                    window->InnerRect.GetSize()
                    + scrollbar_sizes_from_last_frame;
            AzVec2 needed_size_from_last_frame =
                    window_just_created
                            ? AzVec2(0, 0)
                            : window->ContentSize
                                      + window->WindowPadding * 2.0f;
            float size_x_for_scrollbars =
                    use_current_size_for_scrollbar_x
                            ? avail_size_from_current_frame.x
                            : avail_size_from_last_frame.x;
            float size_y_for_scrollbars =
                    use_current_size_for_scrollbar_y
                            ? avail_size_from_current_frame.y
                            : avail_size_from_last_frame.y;
            bool scrollbar_x_prev = window->ScrollbarX;

            window->ScrollbarY = (flags & AzWindowFlags_AlwaysVerticalScrollbar)
                                 || ((needed_size_from_last_frame.y
                                      > size_y_for_scrollbars)
                                     && !(flags & AzWindowFlags_NoScrollbar));
            window->ScrollbarX =
                    (flags & AzWindowFlags_AlwaysHorizontalScrollbar)
                    || ((needed_size_from_last_frame.x
                         > size_x_for_scrollbars
                                   - (window->ScrollbarY ? style.ScrollbarSize
                                                         : 0.0f))
                        && !(flags & AzWindowFlags_NoScrollbar)
                        && (flags & AzWindowFlags_HorizontalScrollbar));

            window->ScrollbarXStabilizeToggledHistory <<= 1;
            window->ScrollbarXStabilizeToggledHistory |= (scrollbar_x_prev
                                                          != window->ScrollbarX)
                                                                 ? 0x01
                                                                 : 0x00;
            const bool scrollbar_x_stabilize =
                    (window->ScrollbarXStabilizeToggledHistory != 0)
                    && AzCountSetBits(window->ScrollbarXStabilizeToggledHistory)
                               >= 4;
            if (scrollbar_x_stabilize)
                window->ScrollbarX = true;

            window->ScrollbarXStabilizeEnabled = scrollbar_x_stabilize;

            if (window->ScrollbarX && !window->ScrollbarY)
                window->ScrollbarY = (needed_size_from_last_frame.y
                                      > size_y_for_scrollbars
                                                - style.ScrollbarSize)
                                     && !(flags & AzWindowFlags_NoScrollbar);
            window->ScrollbarSizes =
                    AzVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f,
                           window->ScrollbarX ? style.ScrollbarSize : 0.0f);

            window->DecoOuterSizeX2 += window->ScrollbarSizes.x;
            window->DecoOuterSizeY2 += window->ScrollbarSizes.y;
        }

        const AzRect host_rect = ((flags & AzWindowFlags_ChildWindow)
                                  && !(flags & AzWindowFlags_Popup)
                                  && !window_is_child_tooltip)
                                         ? parent_window->ClipRect
                                         : viewport_rect;
        const AzRect outer_rect = window->Rect();
        const AzRect title_bar_rect = window->TitleBarRect();
        window->OuterRectClipped = outer_rect;
        if (window->DockIsActive)
            window->OuterRectClipped.Min.y += window->TitleBarHeight;
        window->OuterRectClipped.ClipWith(host_rect);

        window->InnerRect.Min.x = window->Pos.x + window->DecoOuterSizeX1;
        window->InnerRect.Min.y = window->Pos.y + window->DecoOuterSizeY1;
        window->InnerRect.Max.x = window->Pos.x + window->Size.x
                                  - window->DecoOuterSizeX2;
        window->InnerRect.Max.y = window->Pos.y + window->Size.y
                                  - window->DecoOuterSizeY2;

        float top_border_size = (((flags & AzWindowFlags_MenuBar)
                                  || !(flags & AzWindowFlags_NoTitleBar))
                                         ? style.FrameBorderSize
                                         : window->WindowBorderSize);

        window->InnerClipRect.Min.x = AzFloor(0.5f + window->InnerRect.Min.x
                                              + window->WindowBorderSize
                                                        * 0.5f);
        window->InnerClipRect.Min.y = AzFloor(0.5f + window->InnerRect.Min.y
                                              + top_border_size * 0.5f);
        window->InnerClipRect.Max.x = AzFloor(
                window->InnerRect.Max.x - window->WindowBorderSize * 0.5f);
        window->InnerClipRect.Max.y = AzFloor(
                window->InnerRect.Max.y - window->WindowBorderSize * 0.5f);
        window->InnerClipRect.ClipWithFull(host_rect);

        window->ScrollMax.x = AzMax(0.0f,
                                    window->ContentSize.x
                                            + window->WindowPadding.x * 2.0f
                                            - window->InnerRect.GetWidth());
        window->ScrollMax.y = AzMax(0.0f,
                                    window->ContentSize.y
                                            + window->WindowPadding.y * 2.0f
                                            - window->InnerRect.GetHeight());

        window->Scroll = CalcNextScrollFromScrollTargetAndClamp(window);
        window->ScrollTarget = AzVec2(FLT_MAX, FLT_MAX);
        window->DecoInnerSizeX1 = window->DecoInnerSizeY1 = 0.0f;

        AZ_ASSERT(window->DrawList->CmdBuffer.Size == 1
                  && window->DrawList->CmdBuffer[0].ElemCount == 0);
        window->DrawList->PushTexture(g.Font->ContainerAtlas->TexRef);
        PushClipRect(host_rect.Min, host_rect.Max, false);

        const bool is_undocked_or_docked_visible = !window->DockIsActive
                                                   || window->DockTabIsVisible;
        if (is_undocked_or_docked_visible)
        {
            bool render_decorations_in_parent = false;
            if ((flags & AzWindowFlags_ChildWindow)
                && !(flags & AzWindowFlags_Popup) && !window_is_child_tooltip)
            {
                AzWindow* previous_child =
                        parent_window->DC.ChildWindows.Size >= 2
                                ? parent_window->DC.ChildWindows
                                          [parent_window->DC.ChildWindows.Size
                                           - 2]
                                : NULL;
                bool previous_child_overlapping =
                        previous_child ? previous_child->Rect().Overlaps(
                                                 window->Rect())
                                       : false;
                bool parent_is_empty = (parent_window->DrawList->VtxBuffer.Size
                                        == 0);
                if (window->DrawList->CmdBuffer.back().ElemCount == 0
                    && !parent_is_empty && !previous_child_overlapping)
                    render_decorations_in_parent = true;
            }
            if (render_decorations_in_parent)
                window->DrawList = parent_window->DrawList;

            const AzWindow* window_to_highlight = g.NavWindowingTarget
                                                          ? g.NavWindowingTarget
                                                          : g.NavWindow;
            const bool title_bar_is_highlight =
                    want_focus
                    || (window_to_highlight
                        && (window->RootWindowForTitleBarHighlight
                                    == window_to_highlight
                                               ->RootWindowForTitleBarHighlight
                            || (window->DockNode
                                && window->DockNode
                                           == window_to_highlight->DockNode)));
            RenderWindowDecorations(window,
                                    title_bar_rect,
                                    title_bar_is_highlight,
                                    handle_borders_and_resize_grips,
                                    resize_grip_count,
                                    resize_grip_col,
                                    resize_grip_draw_size);

            if (render_decorations_in_parent)
                window->DrawList = &window->DrawListInst;
        }

        const bool allow_scrollbar_x = !(flags & AzWindowFlags_NoScrollbar)
                                       && (flags
                                           & AzWindowFlags_HorizontalScrollbar);
        const bool allow_scrollbar_y = !(flags & AzWindowFlags_NoScrollbar);
        const float work_rect_size_x =
                (window->ContentSizeExplicit.x != 0.0f
                         ? window->ContentSizeExplicit.x
                         : AzMax(allow_scrollbar_x ? window->ContentSize.x
                                                   : 0.0f,
                                 window->Size.x - window->WindowPadding.x * 2.0f
                                         - (window->DecoOuterSizeX1
                                            + window->DecoOuterSizeX2)));
        const float work_rect_size_y =
                (window->ContentSizeExplicit.y != 0.0f
                         ? window->ContentSizeExplicit.y
                         : AzMax(allow_scrollbar_y ? window->ContentSize.y
                                                   : 0.0f,
                                 window->Size.y - window->WindowPadding.y * 2.0f
                                         - (window->DecoOuterSizeY1
                                            + window->DecoOuterSizeY2)));
        window->WorkRect.Min.x = AzTrunc(
                window->InnerRect.Min.x - window->Scroll.x
                + AzMax(window->WindowPadding.x, window->WindowBorderSize));
        window->WorkRect.Min.y = AzTrunc(
                window->InnerRect.Min.y - window->Scroll.y
                + AzMax(window->WindowPadding.y, window->WindowBorderSize));
        window->WorkRect.Max.x = window->WorkRect.Min.x + work_rect_size_x;
        window->WorkRect.Max.y = window->WorkRect.Min.y + work_rect_size_y;
        window->ParentWorkRect = window->WorkRect;

        window->ContentRegionRect.Min.x = window->Pos.x - window->Scroll.x
                                          + window->WindowPadding.x
                                          + window->DecoOuterSizeX1;
        window->ContentRegionRect.Min.y = window->Pos.y - window->Scroll.y
                                          + window->WindowPadding.y
                                          + window->DecoOuterSizeY1;
        window->ContentRegionRect.Max.x =
                window->ContentRegionRect.Min.x
                + (window->ContentSizeExplicit.x != 0.0f
                           ? window->ContentSizeExplicit.x
                           : (window->Size.x - window->WindowPadding.x * 2.0f
                              - (window->DecoOuterSizeX1
                                 + window->DecoOuterSizeX2)));
        window->ContentRegionRect.Max.y =
                window->ContentRegionRect.Min.y
                + (window->ContentSizeExplicit.y != 0.0f
                           ? window->ContentSizeExplicit.y
                           : (window->Size.y - window->WindowPadding.y * 2.0f
                              - (window->DecoOuterSizeY1
                                 + window->DecoOuterSizeY2)));

        window->DC.Indent.x = window->DecoOuterSizeX1 + window->WindowPadding.x
                              - window->Scroll.x;
        window->DC.GroupOffset.x = 0.0f;
        window->DC.ColumnsOffset.x = 0.0f;

        double start_pos_highp_x = (double)window->Pos.x
                                   + window->WindowPadding.x
                                   - (double)window->Scroll.x
                                   + window->DecoOuterSizeX1
                                   + window->DC.ColumnsOffset.x;
        double start_pos_highp_y = (double)window->Pos.y
                                   + window->WindowPadding.y
                                   - (double)window->Scroll.y
                                   + window->DecoOuterSizeY1;
        window->DC.CursorStartPos = AzVec2((float)start_pos_highp_x,
                                           (float)start_pos_highp_y);
        window->DC.CursorStartPosLossyness = AzVec2(
                (float)(start_pos_highp_x - window->DC.CursorStartPos.x),
                (float)(start_pos_highp_y - window->DC.CursorStartPos.y));
        window->DC.CursorPos = window->DC.CursorStartPos;
        window->DC.CursorPosPrevLine = window->DC.CursorPos;
        window->DC.CursorMaxPos = window->DC.CursorStartPos;
        window->DC.IdealMaxPos = window->DC.CursorStartPos;
        window->DC.CurrLineSize = window->DC.PrevLineSize = AzVec2(0.0f, 0.0f);
        window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset =
                0.0f;
        window->DC.IsSameLine = window->DC.IsSetPos = false;

        window->DC.NavLayerCurrent = AzNavLayer_Main;
        window->DC.NavLayersActiveMask = window->DC.NavLayersActiveMaskNext;
        window->DC.NavLayersActiveMaskNext = 0x00;
        window->DC.NavIsScrollPushableX = true;
        window->DC.NavHideHighlightOneFrame = false;
        window->DC.NavWindowHasScrollY = (window->ScrollMax.y > 0.0f);

        window->DC.MenuBarAppending = false;
        window->DC.MenuColumns.Update(style.ItemSpacing.x,
                                      window_just_activated_by_user);
        window->DC.TreeDepth = 0;
        window->DC.TreeHasStackDataDepthMask =
                window->DC.TreeRecordsClippedNodesY2Mask = 0x00;
        window->DC.ChildWindows.resize(0);
        window->DC.StateStorage = &window->StateStorage;
        window->DC.CurrentColumns = NULL;
        window->DC.LayoutType = AzLayoutType_Vertical;
        window->DC.ParentLayoutType = parent_window
                                              ? parent_window->DC.LayoutType
                                              : AzLayoutType_Vertical;

        if (window->Size.x > 0.0f && !(flags & AzWindowFlags_Tooltip)
            && !(flags & AzWindowFlags_AlwaysAutoResize))
            window->ItemWidthDefault = AzTrunc(window->Size.x * 0.65f);
        else
            window->ItemWidthDefault = AzTrunc(g.FontSize * 16.0f);
        window->DC.ItemWidth = window->ItemWidthDefault;
        window->DC.TextWrapPos = -1.0f;
        window->DC.ItemWidthStack.resize(0);
        window->DC.TextWrapPosStack.resize(0);
        if (flags & AzWindowFlags_Modal)
            window->DC.ModalDimBgColor = ColorConvertFloat4ToU32(
                    GetStyleColorVec4(AzCol_ModalWindowDimBg));

        if (window->AutoFitFramesX > 0)
            window->AutoFitFramesX--;
        if (window->AutoFitFramesY > 0)
            window->AutoFitFramesY--;

        g.NextWindowData.ClearFlags();

        if (want_focus)
            FocusWindow(window, AzFocusRequestFlags_UnlessBelowModal);
        if (want_focus && window == g.NavWindow)
            NavInitWindow(window, false);

        if (p_open != NULL && window->Viewport->PlatformRequestClose
            && window->Viewport != GetMainViewport())
        {
            AZ_DEBUG_LOG_VIEWPORT(
                    "[viewport] Window '%s' closed by PlatformRequestClose\n",
                    window->Name);
            *p_open = false;
            g.NavWindowingToggleLayer = false;
        }

        if (g.IO.ConfigWindowsCopyContentsWithCtrlC)
            if (g.NavWindow && g.NavWindow->RootWindow == window
                && g.ActiveId == 0 && Shortcut(AzMod_Ctrl | AzKey_C))
                LogToClipboard(0);

        if (!(flags & AzWindowFlags_NoTitleBar) && !window->DockIsActive)
            RenderWindowTitleBarContents(
                    window,
                    AzRect(title_bar_rect.Min.x + window->WindowBorderSize,
                           title_bar_rect.Min.y,
                           title_bar_rect.Max.x - window->WindowBorderSize,
                           title_bar_rect.Max.y),
                    name,
                    p_open);
        else if (!(flags & AzWindowFlags_NoTitleBar) && window->DockIsActive)
            LogText("%.*s\n",
                    (int)(FindRenderedTextEnd(window->Name) - window->Name),
                    window->Name);

        window->HitTestHoleSize.x = window->HitTestHoleSize.y = 0;

        if (flags & AzWindowFlags_Tooltip)
            g.TooltipPreviousWindow = window;

        if (g.IO.ConfigFlags & AzConfigFlags_DockingEnable)
        {
            if (g.MovingWindow == window
                && (window->RootWindowDockTree->Flags & AzWindowFlags_NoDocking)
                           == 0)
                BeginDockableDragDropSource(window);

            if (g.DragDropActive && !(flags & AzWindowFlags_NoDocking))
                if (g.MovingWindow == NULL
                    || g.MovingWindow->RootWindowDockTree != window)
                    if ((window == window->RootWindowDockTree)
                        && !(window->Flags & AzWindowFlags_DockNodeHost))
                        BeginDockableDragDropTarget(window);
        }

        window->DC.WindowItemStatusFlags = AzItemStatusFlags_None;
        window->DC.WindowItemStatusFlags |=
                IsMouseHoveringRect(title_bar_rect.Min,
                                    title_bar_rect.Max,
                                    false)
                        ? AzItemStatusFlags_HoveredRect
                        : 0;
        SetLastItemDataForWindow(window, title_bar_rect);

#ifndef AZ_DISABLE_DEBUG_TOOLS
        if (g.DebugLocateId != 0
            && (window->ID == g.DebugLocateId
                || window->MoveId == g.DebugLocateId))
            DebugLocateItemResolveWithLastItem();
#endif

#ifdef AZ_ENABLE_TEST_ENGINE
        if (!(window->Flags & AzWindowFlags_NoTitleBar))
        {
            window->DC.NavLayerCurrent = AzNavLayer_Menu;
            AZ_TEST_ENGINE_ITEM_ADD(g.LastItemData.ID,
                                    g.LastItemData.Rect,
                                    &g.LastItemData);
            window->DC.NavLayerCurrent = AzNavLayer_Main;
        }
#endif
    } else
    {
        if (window->SkipRefresh)
            SetWindowActiveForSkipRefresh(window);

        SetCurrentViewport(window, window->Viewport);
        SetCurrentWindow(window);
        g.NextWindowData.ClearFlags();
        SetLastItemDataForWindow(window, window->TitleBarRect());
    }

    if (!(flags & AzWindowFlags_DockNodeHost) && !window->SkipRefresh)
        PushClipRect(window->InnerClipRect.Min,
                     window->InnerClipRect.Max,
                     true);

    window->WriteAccessed = false;
    window->BeginCount++;

    if (first_begin_of_the_frame && !window->SkipRefresh)
    {
        if (window->DockIsActive && !window->DockTabIsVisible)
        {
            if (window->LastFrameJustFocused == g.FrameCount)
                window->HiddenFramesCannotSkipItems = 1;
            else
                window->HiddenFramesCanSkipItems = 1;
        }

        if ((flags & AzWindowFlags_ChildWindow)
            && !(flags & AzWindowFlags_ChildMenu))
        {
            AZ_ASSERT((flags & AzWindowFlags_NoTitleBar) != 0
                      || window->DockIsActive);
            const bool nav_request =
                    (window->ChildFlags & AzChildFlags_NavFlattened)
                    && (g.NavAnyRequest && g.NavWindow
                        && g.NavWindow->RootWindowForNav
                                   == window->RootWindowForNav);
            if (!g.LogEnabled && !nav_request)
                if (window->OuterRectClipped.Min.x
                            >= window->OuterRectClipped.Max.x
                    || window->OuterRectClipped.Min.y
                               >= window->OuterRectClipped.Max.y)
                {
                    if (window->AutoFitFramesX > 0
                        || window->AutoFitFramesY > 0)
                        window->HiddenFramesCannotSkipItems = 1;
                    else
                        window->HiddenFramesCanSkipItems = 1;
                }

            if (parent_window
                && (parent_window->Collapsed
                    || parent_window->HiddenFramesCanSkipItems > 0))
                window->HiddenFramesCanSkipItems = 1;
            if (parent_window && parent_window->HiddenFramesCannotSkipItems > 0)
                window->HiddenFramesCannotSkipItems = 1;
        }

        if (style.Alpha <= 0.0f)
            window->HiddenFramesCanSkipItems = 1;

        bool hidden_regular = (window->HiddenFramesCanSkipItems > 0)
                              || (window->HiddenFramesCannotSkipItems > 0);
        window->Hidden = hidden_regular
                         || (window->HiddenFramesForRenderOnly > 0);

        if (window->DisableInputsFrames > 0)
        {
            window->DisableInputsFrames--;
            window->Flags |= AzWindowFlags_NoInputs;
        }

        bool skip_items = false;
        if (window->Collapsed || !window->Active || hidden_regular)
            if (window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0
                && window->HiddenFramesCannotSkipItems <= 0)
                skip_items = true;
        window->SkipItems = skip_items;

        if (window->SkipItems)
            window->DC.NavLayersActiveMaskNext = window->DC.NavLayersActiveMask;

        if (window->SkipItems && !window->Appearing)
            AZ_ASSERT(window->Appearing == false);

    } else if (first_begin_of_the_frame)
    {
        window->SkipItems = true;
    }

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (!window->IsFallbackWindow)
        if ((g.IO.ConfigDebugBeginReturnValueOnce && window_just_created)
            || (g.IO.ConfigDebugBeginReturnValueLoop
                && g.DebugBeginReturnValueCullDepth
                           == g.CurrentWindowStack.Size))
        {
            if (window->AutoFitFramesX > 0)
            {
                window->AutoFitFramesX++;
            }
            if (window->AutoFitFramesY > 0)
            {
                window->AutoFitFramesY++;
            }
            return false;
        }
#endif

    return !window->SkipItems;
}

void Az::End()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    if (g.CurrentWindowStack.Size <= 1 && g.WithinFrameScopeWithAzplicitWindow)
    {
        AZ_ASSERT_USER_ERROR(g.CurrentWindowStack.Size > 1,
                             "Calling End() too many times!");
        return;
    }
    AzWindowStackData& window_stack_data = g.CurrentWindowStack.back();

    if ((window->Flags & AzWindowFlags_ChildWindow)
        && !(window->Flags & AzWindowFlags_DockNodeHost)
        && !window->DockIsActive)
        AZ_ASSERT_USER_ERROR(g.WithinEndChildID == window->ID,
                             "Must call EndChild() and not End()!");

    if (window->DC.CurrentColumns)
        EndColumns();
    if (!(window->Flags & AzWindowFlags_DockNodeHost) && !window->SkipRefresh)
        PopClipRect();
    PopFocusScope();
    if (window_stack_data.DisabledOverrideReenable
        && window->RootWindow == window)
        EndDisabledOverrideReenable();

    if (window->SkipRefresh)
    {
        AZ_ASSERT(window->DrawList == NULL);
        window->DrawList = &window->DrawListInst;
    }

    if (g.LogWindow == window)
        LogFinish();

    if (window->DC.IsSetPos)
        ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

    if (window->DockNode && window->DockTabIsVisible)
        if (AzWindow* host_window = window->DockNode->HostWindow)
            host_window->DC.CursorMaxPos = window->DC.CursorMaxPos
                                           + window->WindowPadding
                                           - host_window->WindowPadding;

    g.LastItemData = window_stack_data.ParentLastItemDataBackup;
    if (window->Flags & AzWindowFlags_ChildMenu)
        g.BeginMenuDepth--;
    if (window->Flags & AzWindowFlags_Popup)
        g.BeginPopupStack.pop_back();

    if (g.IO.ConfigErrorRecovery)
        ErrorRecoveryTryToRecoverWindowState(
                &window_stack_data.StackSizesInBegin);

    g.CurrentWindowStack.pop_back();
    SetCurrentWindow(g.CurrentWindowStack.Size == 0
                             ? NULL
                             : g.CurrentWindowStack.back().Window);
    if (g.CurrentWindow)
        SetCurrentViewport(g.CurrentWindow, g.CurrentWindow->Viewport);
}

void Az::PushItemFlag(AzItemFlags option, bool enabled)
{
    AzContext& g = *GAz;
    AzItemFlags item_flags = g.CurrentItemFlags;
    AZ_ASSERT(item_flags == g.ItemFlagsStack.back());
    if (enabled)
        item_flags |= option;
    else
        item_flags &= ~option;
    g.CurrentItemFlags = item_flags;
    g.ItemFlagsStack.push_back(item_flags);
}

void Az::PopItemFlag()
{
    AzContext& g = *GAz;
    if (g.ItemFlagsStack.Size <= 1)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopItemFlag() too many times!");
        return;
    }
    g.ItemFlagsStack.pop_back();
    g.CurrentItemFlags = g.ItemFlagsStack.back();
}

void Az::BeginDisabled(bool disabled)
{
    AzContext& g = *GAz;
    bool was_disabled = (g.CurrentItemFlags & AzItemFlags_Disabled) != 0;
    if (!was_disabled && disabled)
    {
        g.DisabledAlphaBackup = g.Style.Alpha;
        g.Style.Alpha *= g.Style.DisabledAlpha;
    }
    if (was_disabled || disabled)
        g.CurrentItemFlags |= AzItemFlags_Disabled;
    g.ItemFlagsStack.push_back(g.CurrentItemFlags);

    g.DisabledStackSize++;
}

void Az::EndDisabled()
{
    AzContext& g = *GAz;
    if (g.DisabledStackSize <= 0)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling EndDisabled() too many times!");
        return;
    }
    g.DisabledStackSize--;
    bool was_disabled = (g.CurrentItemFlags & AzItemFlags_Disabled) != 0;

    g.ItemFlagsStack.pop_back();
    g.CurrentItemFlags = g.ItemFlagsStack.back();
    if (was_disabled && (g.CurrentItemFlags & AzItemFlags_Disabled) == 0)
        g.Style.Alpha = g.DisabledAlphaBackup;
}

void Az::BeginDisabledOverrideReenable()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.CurrentItemFlags & AzItemFlags_Disabled);
    g.CurrentWindowStack.back().DisabledOverrideReenableAlphaBackup =
            g.Style.Alpha;
    g.Style.Alpha = g.DisabledAlphaBackup;
    g.CurrentItemFlags &= ~AzItemFlags_Disabled;
    g.ItemFlagsStack.push_back(g.CurrentItemFlags);
    g.DisabledStackSize++;
}

void Az::EndDisabledOverrideReenable()
{
    AzContext& g = *GAz;
    g.DisabledStackSize--;
    AZ_ASSERT(g.DisabledStackSize > 0);
    g.ItemFlagsStack.pop_back();
    g.CurrentItemFlags = g.ItemFlagsStack.back();
    g.Style.Alpha =
            g.CurrentWindowStack.back().DisabledOverrideReenableAlphaBackup;
}

void Az::PushTextWrapPos(float wrap_pos_x)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    window->DC.TextWrapPosStack.push_back(window->DC.TextWrapPos);
    window->DC.TextWrapPos = wrap_pos_x;
}

void Az::PopTextWrapPos()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->DC.TextWrapPosStack.Size <= 0)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopTextWrapPos() too many times!");
        return;
    }
    window->DC.TextWrapPos = window->DC.TextWrapPosStack.back();
    window->DC.TextWrapPosStack.pop_back();
}

static AzWindow* GetCombinedRootWindow(AzWindow* window,
                                       bool popup_hierarchy,
                                       bool dock_hierarchy)
{
    AzWindow* last_window = NULL;
    while (last_window != window)
    {
        last_window = window;
        window = window->RootWindow;
        if (popup_hierarchy)
            window = window->RootWindowPopupTree;
        if (dock_hierarchy)
            window = window->RootWindowDockTree;
    }
    return window;
}

bool Az::IsWindowChildOf(AzWindow* window,
                         AzWindow* potential_parent,
                         bool popup_hierarchy,
                         bool dock_hierarchy)
{
    AzWindow* window_root = GetCombinedRootWindow(window,
                                                  popup_hierarchy,
                                                  dock_hierarchy);
    if (window_root == potential_parent)
        return true;
    while (window != NULL)
    {
        if (window == potential_parent)
            return true;
        if (window == window_root)
            return false;
        window = window->ParentWindow;
    }
    return false;
}

bool Az::IsWindowWithinBeginStackOf(AzWindow* window,
                                    AzWindow* potential_parent)
{
    if (window->RootWindow == potential_parent)
        return true;
    while (window != NULL)
    {
        if (window == potential_parent)
            return true;
        window = window->ParentWindowInBeginStack;
    }
    return false;
}

bool Az::IsWindowAbove(AzWindow* potential_above, AzWindow* potential_below)
{
    AzContext& g = *GAz;

    const int display_layer_delta = GetWindowDisplayLayer(potential_above)
                                    - GetWindowDisplayLayer(potential_below);
    if (display_layer_delta != 0)
        return display_layer_delta > 0;

    for (int i = g.Windows.Size - 1; i >= 0; i--)
    {
        AzWindow* candidate_window = g.Windows[i];
        if (candidate_window == potential_above)
            return true;
        if (candidate_window == potential_below)
            return false;
    }
    return false;
}

bool Az::IsWindowHovered(AzHoveredFlags flags)
{
    AzContext& g = *GAz;
    AZ_ASSERT_USER_ERROR((flags & ~AzHoveredFlags_AllowedMaskForIsWindowHovered)
                                 == 0,
                         "Invalid flags for IsWindowHovered()!");

    AzWindow* ref_window = g.HoveredWindow;
    AzWindow* cur_window = g.CurrentWindow;
    if (ref_window == NULL)
        return false;

    if ((flags & AzHoveredFlags_AnyWindow) == 0)
    {
        AZ_ASSERT(cur_window);
        const bool popup_hierarchy = (flags & AzHoveredFlags_NoPopupHierarchy)
                                     == 0;
        const bool dock_hierarchy = (flags & AzHoveredFlags_DockHierarchy) != 0;
        if (flags & AzHoveredFlags_RootWindow)
            cur_window = GetCombinedRootWindow(cur_window,
                                               popup_hierarchy,
                                               dock_hierarchy);

        bool result;
        if (flags & AzHoveredFlags_ChildWindows)
            result = IsWindowChildOf(ref_window,
                                     cur_window,
                                     popup_hierarchy,
                                     dock_hierarchy);
        else
            result = (ref_window == cur_window);
        if (!result)
            return false;
    }

    if (!IsWindowContentHoverable(ref_window, flags))
        return false;
    if (!(flags & AzHoveredFlags_AllowWhenBlockedByActiveItem))
        if (g.ActiveId != 0 && !g.ActiveIdAllowOverlap
            && g.ActiveId != ref_window->MoveId)
            return false;

    if (flags & AzHoveredFlags_ForTooltip)
        flags = ApplyHoverFlagsForTooltip(flags,
                                          g.Style.HoverFlagsForTooltipMouse);
    if ((flags & AzHoveredFlags_Stationary) != 0
        && g.HoverWindowUnlockedStationaryId != ref_window->ID)
        return false;

    return true;
}

AzID Az::GetWindowDockID()
{
    AzContext& g = *GAz;
    return g.CurrentWindow->DockId;
}

bool Az::IsWindowDocked()
{
    AzContext& g = *GAz;
    return g.CurrentWindow->DockIsActive;
}

float Az::GetWindowWidth()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->Size.x;
}

float Az::GetWindowHeight()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->Size.y;
}

AzVec2 Az::GetWindowPos()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    return window->Pos;
}

void Az::SetWindowPos(AzWindow* window, const AzVec2& pos, AzCond cond)
{
    if (cond && (window->SetWindowPosAllowFlags & cond) == 0)
        return;

    AZ_ASSERT(cond == 0 || AzIsPowerOfTwo(cond));

    window->SetWindowPosAllowFlags &= ~(AzCond_Once | AzCond_FirstUseEver
                                        | AzCond_Appearing);
    window->SetWindowPosVal = AzVec2(FLT_MAX, FLT_MAX);

    const AzVec2 old_pos = window->Pos;
    window->Pos = AzTrunc(pos);
    AzVec2 offset = window->Pos - old_pos;
    if (offset.x == 0.0f && offset.y == 0.0f)
        return;
    MarkIniSettingsDirty(window);

    window->DC.CursorPos += offset;

    window->DC.CursorMaxPos += offset;

    window->DC.IdealMaxPos += offset;
    window->DC.CursorStartPos += offset;
}

void Az::SetWindowPos(const AzVec2& pos, AzCond cond)
{
    AzWindow* window = GetCurrentWindowRead();
    SetWindowPos(window, pos, cond);
}

void Az::SetWindowPos(const char* name, const AzVec2& pos, AzCond cond)
{
    if (AzWindow* window = FindWindowByName(name))
        SetWindowPos(window, pos, cond);
}

AzVec2 Az::GetWindowSize()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->Size;
}

void Az::SetWindowSize(AzWindow* window, const AzVec2& size, AzCond cond)
{
    if (cond && (window->SetWindowSizeAllowFlags & cond) == 0)
        return;

    AZ_ASSERT(cond == 0 || AzIsPowerOfTwo(cond));

    window->SetWindowSizeAllowFlags &= ~(AzCond_Once | AzCond_FirstUseEver
                                         | AzCond_Appearing);

    if ((window->Flags & AzWindowFlags_ChildWindow) == 0 || window->Appearing
        || (window->ChildFlags & AzChildFlags_AlwaysAutoResize) != 0)
        window->AutoFitFramesX = (size.x <= 0.0f) ? 2 : 0;
    if ((window->Flags & AzWindowFlags_ChildWindow) == 0 || window->Appearing
        || (window->ChildFlags & AzChildFlags_AlwaysAutoResize) != 0)
        window->AutoFitFramesY = (size.y <= 0.0f) ? 2 : 0;

    AzVec2 old_size = window->SizeFull;
    if (size.x <= 0.0f)
        window->AutoFitOnlyGrows = false;
    else
        window->SizeFull.x = AZ_TRUNC(size.x);
    if (size.y <= 0.0f)
        window->AutoFitOnlyGrows = false;
    else
        window->SizeFull.y = AZ_TRUNC(size.y);
    if (old_size.x != window->SizeFull.x || old_size.y != window->SizeFull.y)
        MarkIniSettingsDirty(window);
}

void Az::SetWindowSize(const AzVec2& size, AzCond cond)
{
    SetWindowSize(GAz->CurrentWindow, size, cond);
}

void Az::SetWindowSize(const char* name, const AzVec2& size, AzCond cond)
{
    if (AzWindow* window = FindWindowByName(name))
        SetWindowSize(window, size, cond);
}

void Az::SetWindowCollapsed(AzWindow* window, bool collapsed, AzCond cond)
{
    if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0)
        return;
    window->SetWindowCollapsedAllowFlags &= ~(AzCond_Once | AzCond_FirstUseEver
                                              | AzCond_Appearing);

    if (window->WantCollapseToggle)
        window->Collapsed ^= 1;
    window->WantCollapseToggle = (window->Collapsed != collapsed);
}

void Az::SetWindowHitTestHole(AzWindow* window,
                              const AzVec2& pos,
                              const AzVec2& size)
{
    AZ_ASSERT(window->HitTestHoleSize.x == 0);
    window->HitTestHoleSize = AzVec2ih(size);
    window->HitTestHoleOffset = AzVec2ih(pos - window->Pos);
}

void Az::SetWindowHiddenAndSkipItemsForCurrentFrame(AzWindow* window)
{
    window->Hidden = window->SkipItems = true;
    window->HiddenFramesCanSkipItems = 1;
}

void Az::SetWindowCollapsed(bool collapsed, AzCond cond)
{
    SetWindowCollapsed(GAz->CurrentWindow, collapsed, cond);
}

bool Az::IsWindowCollapsed()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->Collapsed;
}

bool Az::IsWindowAppearing()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->Appearing;
}

void Az::SetWindowCollapsed(const char* name, bool collapsed, AzCond cond)
{
    if (AzWindow* window = FindWindowByName(name))
        SetWindowCollapsed(window, collapsed, cond);
}

void Az::SetNextWindowPos(const AzVec2& pos, AzCond cond, const AzVec2& pivot)
{
    AzContext& g = *GAz;
    AZ_ASSERT(cond == 0 || AzIsPowerOfTwo(cond));

    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasPos;
    g.NextWindowData.PosVal = pos;
    g.NextWindowData.PosPivotVal = pivot;
    g.NextWindowData.PosCond = cond ? cond : AzCond_Always;
    g.NextWindowData.PosUndock = true;
}

void Az::SetNextWindowSize(const AzVec2& size, AzCond cond)
{
    AzContext& g = *GAz;
    AZ_ASSERT(cond == 0 || AzIsPowerOfTwo(cond));

    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasSize;
    g.NextWindowData.SizeVal = size;
    g.NextWindowData.SizeCond = cond ? cond : AzCond_Always;
}

void Az::SetNextWindowSizeConstraints(const AzVec2& size_min,
                                      const AzVec2& size_max,
                                      AzSizeCallback custom_callback,
                                      void* custom_callback_user_data)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasSizeConstraint;
    g.NextWindowData.SizeConstraintRect = AzRect(size_min, size_max);
    g.NextWindowData.SizeCallback = custom_callback;
    g.NextWindowData.SizeCallbackUserData = custom_callback_user_data;
}

void Az::SetNextWindowContentSize(const AzVec2& size)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasContentSize;
    g.NextWindowData.ContentSizeVal = AzTrunc(size);
}

void Az::SetNextWindowScroll(const AzVec2& scroll)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasScroll;
    g.NextWindowData.ScrollVal = scroll;
}

void Az::SetNextWindowCollapsed(bool collapsed, AzCond cond)
{
    AzContext& g = *GAz;
    AZ_ASSERT(cond == 0 || AzIsPowerOfTwo(cond));

    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasCollapsed;
    g.NextWindowData.CollapsedVal = collapsed;
    g.NextWindowData.CollapsedCond = cond ? cond : AzCond_Always;
}

void Az::SetNextWindowBgAlpha(float alpha)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasBgAlpha;
    g.NextWindowData.BgAlphaVal = alpha;
}

void Az::SetNextWindowViewport(AzID id)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasViewport;
    g.NextWindowData.ViewportId = id;
}

void Az::SetNextWindowDockID(AzID id, AzCond cond)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasDock;
    g.NextWindowData.DockCond = cond ? cond : AzCond_Always;
    g.NextWindowData.DockId = id;
}

void Az::SetNextWindowClass(const AzWindowClass* window_class)
{
    AzContext& g = *GAz;
    AZ_ASSERT((window_class->ViewportFlagsOverrideSet
               & window_class->ViewportFlagsOverrideClear)
              == 0);
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasWindowClass;
    g.NextWindowData.WindowClass = *window_class;
}

void Az::SetNextWindowRefreshPolicy(AzWindowRefreshFlags flags)
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasRefreshPolicy;
    g.NextWindowData.RefreshFlagsVal = flags;
}

AzDrawList* Az::GetWindowDrawList()
{
    AzWindow* window = GetCurrentWindow();
    return window->DrawList;
}

float Az::GetWindowDpiScale()
{
    AzContext& g = *GAz;
    return g.CurrentDpiScale;
}

AzViewport* Az::GetWindowViewport()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.CurrentViewport != NULL
              && g.CurrentViewport == g.CurrentWindow->Viewport);
    return g.CurrentViewport;
}

AzFont* Az::GetFont()
{
    return GAz->Font;
}

AzFontBaked* Az::GetFontBaked()
{
    return GAz->FontBaked;
}

float Az::GetFontSize()
{
    return GAz->FontSize;
}

AzVec2 Az::GetFontTexUvWhitePixel()
{
    return GAz->DrawListSharedData.TexUvWhitePixel;
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
void Az::SetWindowFontScale(float scale)
{
    AZ_ASSERT(scale > 0.0f);
    AzWindow* window = GetCurrentWindow();
    window->FontWindowScale = scale;
    UpdateCurrentFontSize(0.0f);
}
#endif

void Az::PushFocusScope(AzID id)
{
    AzContext& g = *GAz;
    AzFocusScopeData data;
    data.ID = id;
    data.WindowID = g.CurrentWindow->ID;
    g.FocusScopeStack.push_back(data);
    g.CurrentFocusScopeId = id;
}

void Az::PopFocusScope()
{
    AzContext& g = *GAz;
    if (g.FocusScopeStack.Size
        <= g.StackSizesInBeginForCurrentWindow->SizeOfFocusScopeStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopFocusScope() too many times!");
        return;
    }
    g.FocusScopeStack.pop_back();
    g.CurrentFocusScopeId = g.FocusScopeStack.Size ? g.FocusScopeStack.back().ID
                                                   : 0;
}

void Az::SetNavFocusScope(AzID focus_scope_id)
{
    AzContext& g = *GAz;
    g.NavFocusScopeId = focus_scope_id;
    g.NavFocusRoute.resize(0);
    if (focus_scope_id == 0)
        return;
    AZ_ASSERT(g.NavWindow != NULL);

    if (focus_scope_id == g.CurrentFocusScopeId)
    {
        for (int n = g.FocusScopeStack.Size - 1;
             n >= 0
             && g.FocusScopeStack.Data[n].WindowID == g.CurrentWindow->ID;
             n--)
            g.NavFocusRoute.push_back(g.FocusScopeStack.Data[n]);
    } else if (focus_scope_id == g.NavWindow->NavRootFocusScopeId)
        g.NavFocusRoute.push_back({focus_scope_id, g.NavWindow->ID});
    else
        return;

    for (AzWindow* window = g.NavWindow->ParentWindowForFocusRoute;
         window != NULL;
         window = window->ParentWindowForFocusRoute)
        g.NavFocusRoute.push_back({window->NavRootFocusScopeId, window->ID});
    AZ_ASSERT(g.NavFocusRoute.Size < 100);
}

void Az::FocusItem()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_DEBUG_LOG_FOCUS("FocusItem(0x%08x) in window \"%s\"\n",
                       g.LastItemData.ID,
                       window->Name);
    if (g.DragDropActive || g.MovingWindow != NULL)
    {
        AZ_DEBUG_LOG_FOCUS("FocusItem() ignored while DragDropActive!\n");
        return;
    }

    AzNavMoveFlags move_flags = AzNavMoveFlags_IsTabbing
                                | AzNavMoveFlags_FocusApi
                                | AzNavMoveFlags_NoSetNavCursorVisible
                                | AzNavMoveFlags_NoSelect;
    AzScrollFlags scroll_flags =
            window->Appearing ? AzScrollFlags_KeepVisibleEdgeX
                                        | AzScrollFlags_AlwaysCenterY
                              : AzScrollFlags_KeepVisibleEdgeX
                                        | AzScrollFlags_KeepVisibleEdgeY;
    SetNavWindow(window);
    NavMoveRequestSubmit(AzDir_None, AzDir_Up, move_flags, scroll_flags);
    NavMoveRequestResolveWithLastItem(&g.NavMoveResultLocal);
}

void Az::ActivateItemByID(AzID id)
{
    AzContext& g = *GAz;
    g.NavNextActivateId = id;
    g.NavNextActivateFlags = AzActivateFlags_None;
}

void Az::SetKeyboardFocusHere(int offset)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(offset >= -1);
    AZ_DEBUG_LOG_FOCUS("SetKeyboardFocusHere(%d) in window \"%s\"\n",
                       offset,
                       window->Name);

    if (g.DragDropActive || g.MovingWindow != NULL)
    {
        AZ_DEBUG_LOG_FOCUS(
                "SetKeyboardFocusHere() ignored while DragDropActive!\n");
        return;
    }

    SetNavWindow(window);

    AzNavMoveFlags move_flags = AzNavMoveFlags_IsTabbing
                                | AzNavMoveFlags_Activate
                                | AzNavMoveFlags_FocusApi
                                | AzNavMoveFlags_NoSetNavCursorVisible;
    AzScrollFlags scroll_flags =
            window->Appearing ? AzScrollFlags_KeepVisibleEdgeX
                                        | AzScrollFlags_AlwaysCenterY
                              : AzScrollFlags_KeepVisibleEdgeX
                                        | AzScrollFlags_KeepVisibleEdgeY;
    NavMoveRequestSubmit(AzDir_None,
                         offset < 0 ? AzDir_Up : AzDir_Down,
                         move_flags,
                         scroll_flags);

    if (offset == -1)
    {
        NavMoveRequestResolveWithLastItem(&g.NavMoveResultLocal);
    } else
    {
        g.NavTabbingDir = 1;
        g.NavTabbingCounter = offset + 1;
    }
}

void Az::SetItemDefaultFocus()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (!window->Appearing)
        return;
    if (g.NavWindow != window->RootWindowForNav
        || (!g.NavInitRequest && g.NavInitResult.ID == 0)
        || g.NavLayer != window->DC.NavLayerCurrent)
        return;

    g.NavInitRequest = false;
    NavApplyItemToResult(&g.NavInitResult);
    NavUpdateAnyRequestFlag();

    if (!window->ClipRect.Contains(g.LastItemData.Rect))
        ScrollToRectEx(window, g.LastItemData.Rect, AzScrollFlags_None);
}

void Az::SetStateStorage(AzStorage* tree)
{
    AzWindow* window = GAz->CurrentWindow;
    window->DC.StateStorage = tree ? tree : &window->StateStorage;
}

AzStorage* Az::GetStateStorage()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->DC.StateStorage;
}

bool Az::IsRectVisible(const AzVec2& size)
{
    AzWindow* window = GAz->CurrentWindow;
    return window->ClipRect.Overlaps(
            AzRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool Az::IsRectVisible(const AzVec2& rect_min, const AzVec2& rect_max)
{
    AzWindow* window = GAz->CurrentWindow;
    return window->ClipRect.Overlaps(AzRect(rect_min, rect_max));
}

void Az::UpdateFontsNewFrame()
{
    AzContext& g = *GAz;
    if ((g.IO.BackendFlags & AzBackendFlags_RendererHasTextures) == 0)
        for (AzFontAtlas* atlas : g.FontAtlases)
            atlas->Locked = true;

    if (g.Style._NextFrameFontSizeBase != 0.0f)
    {
        g.Style.FontSizeBase = g.Style._NextFrameFontSizeBase;
        g.Style._NextFrameFontSizeBase = 0.0f;
    }

    AzFont* font = Az::GetDefaultFont();
    if (g.Style.FontSizeBase <= 0.0f)
        g.Style.FontSizeBase = (font->LegacySize > 0.0f ? font->LegacySize
                                                        : FONT_DEFAULT_SIZE);

    g.Font = font;
    g.FontSizeBase = g.Style.FontSizeBase;
    g.FontSize = 0.0f;
    AzFontStackData font_stack_data = {font,
                                       g.Style.FontSizeBase,
                                       g.Style.FontSizeBase};
    SetCurrentFont(font_stack_data.Font,
                   font_stack_data.FontSizeBeforeScaling,
                   0.0f);
    g.FontStack.push_back(font_stack_data);
    AZ_ASSERT(g.Font->IsLoaded());
}

void Az::UpdateFontsEndFrame()
{
    PopFont();
}

AzFont* Az::GetDefaultFont()
{
    AzContext& g = *GAz;
    AzFontAtlas* atlas = g.IO.Fonts;
    if (atlas->Builder == NULL || atlas->Fonts.Size == 0)
        AzFontAtlasBuildMain(atlas);
    return g.IO.FontDefault ? g.IO.FontDefault : atlas->Fonts[0];
}

void Az::RegisterUserTexture(AzTextureData* tex)
{
    AzContext& g = *GAz;
    AZ_ASSERT(tex->RefCount > 0);
    g.UserTextures.push_back(tex);
}

void Az::UnregisterUserTexture(AzTextureData* tex)
{
    AzContext& g = *GAz;
    g.UserTextures.find_erase(tex);
}

void Az::RegisterFontAtlas(AzFontAtlas* atlas)
{
    AzContext& g = *GAz;
    if (g.FontAtlases.Size == 0)
        AZ_ASSERT(atlas == g.IO.Fonts);
    atlas->RefCount++;
    g.FontAtlases.push_back(atlas);
    AzFontAtlasAddDrawListSharedData(atlas, &g.DrawListSharedData);
}

void Az::UnregisterFontAtlas(AzFontAtlas* atlas)
{
    AzContext& g = *GAz;
    AZ_ASSERT(atlas->RefCount > 0);
    AzFontAtlasRemoveDrawListSharedData(atlas, &g.DrawListSharedData);
    g.FontAtlases.find_erase(atlas);
    atlas->RefCount--;
}

void Az::SetCurrentFont(AzFont* font,
                        float font_size_before_scaling,
                        float font_size_after_scaling)
{
    AzContext& g = *GAz;
    g.Font = font;
    g.FontSizeBase = font_size_before_scaling;
    UpdateCurrentFontSize(font_size_after_scaling);

    if (font != NULL)
    {
        AZ_ASSERT(font && font->IsLoaded());

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
        AZ_ASSERT(font->Scale > 0.0f);
#endif
        g.DrawListSharedData.Font = g.Font;
        AzFontAtlasUpdateDrawListsSharedData(g.Font->ContainerAtlas);
        if (g.CurrentWindow != NULL)
            g.CurrentWindow->DrawList->_SetTexture(
                    font->ContainerAtlas->TexRef);
    }
}

void Az::UpdateCurrentFontSize(float restore_font_size_after_scaling)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    g.Style.FontSizeBase = g.FontSizeBase;

    if (window != NULL && window->SkipItems)
        if (g.CurrentTable == NULL || g.CurrentTable->CurrentColumn != -1)

        {
            g.FontBaked = NULL;
            return;
        }

    float final_size = (restore_font_size_after_scaling > 0.0f)
                               ? restore_font_size_after_scaling
                               : 0.0f;
    if (final_size == 0.0f)
    {
        final_size = g.FontSizeBase;

        final_size *= g.Style.FontScaleMain;
        final_size *= g.Style.FontScaleDpi;

        if (window != NULL)
            final_size *= window->FontWindowScale;

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
        final_size *= g.IO.FontGlobalScale;
        if (g.Font != NULL)
            final_size *= g.Font->Scale;
#endif
    }

    final_size = GetRoundedFontSize(final_size);
    final_size = AzMax(1.0f, final_size);
    if (g.Font != NULL
        && (g.IO.BackendFlags & AzBackendFlags_RendererHasTextures))
        g.Font->CurrentRasterizerDensity = g.FontRasterizerDensity;
    g.FontSize = final_size;
    g.FontBaked = (g.Font != NULL && window != NULL)
                          ? g.Font->GetFontBaked(final_size)
                          : NULL;
    g.FontBakedScale = (g.Font != NULL && window != NULL)
                               ? (g.FontSize / g.FontBaked->Size)
                               : 0.0f;
    g.DrawListSharedData.FontSize = g.FontSize;
    g.DrawListSharedData.FontScale = g.FontBakedScale;
}

void Az::SetFontRasterizerDensity(float rasterizer_density)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.IO.BackendFlags & AzBackendFlags_RendererHasTextures);
    if (g.FontRasterizerDensity == rasterizer_density)
        return;
    g.FontRasterizerDensity = rasterizer_density;
    UpdateCurrentFontSize(0.0f);
}

void Az::PushFont(AzFont* font, float font_size_base)
{
    AzContext& g = *GAz;
    g.FontStack.push_back({g.Font, g.FontSizeBase, g.FontSize});
    if (font == NULL)
        font = GetDefaultFont();
    if (font_size_base <= 0.0f)
    {
        if (font->Flags & AzFontFlags_DefaultToLegacySize)
            font_size_base = font->LegacySize;

        else
            font_size_base = g.FontSizeBase;
    }
    SetCurrentFont(font, font_size_base, 0.0f);
}

void Az::PopFont()
{
    AzContext& g = *GAz;
    if (g.FontStack.Size <= 0)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopFont() too many times!");
        return;
    }
    AzFontStackData* font_stack_data = &g.FontStack.back();
    SetCurrentFont(font_stack_data->Font,
                   font_stack_data->FontSizeBeforeScaling,
                   font_stack_data->FontSizeAfterScaling);
    g.FontStack.pop_back();
}

void Az::PushFontSize(float font_size_base)
{
    AzContext& g = *GAz;
    PushFont(g.Font, font_size_base);
}

void Az::PopFontSize()
{
    PopFont();
}

AZ_MSVC_RUNTIME_CHECKS_OFF
AzID AzWindow::GetID(const char* str, const char* str_end)
{
    AzID seed = IDStack.back();
    AzID id = AzHashStr(str, str_end ? (str_end - str) : 0, seed);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *Ctx;
    if (g.DebugHookIdInfo == id)
        Az::DebugHookIdInfo(id, AzDataType_String, str, str_end);
#endif
    return id;
}

AzID AzWindow::GetID(const void* ptr)
{
    AzID seed = IDStack.back();
    AzID id = AzHashData(&ptr, sizeof(void*), seed);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *Ctx;
    if (g.DebugHookIdInfo == id)
        Az::DebugHookIdInfo(id, AzDataType_Pointer, ptr, NULL);
#endif
    return id;
}

AzID AzWindow::GetID(int n)
{
    AzID seed = IDStack.back();
    AzID id = AzHashData(&n, sizeof(n), seed);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *Ctx;
    if (g.DebugHookIdInfo == id)
        Az::DebugHookIdInfo(id, AzDataType_S32, (void*)(intptr_t)n, NULL);
#endif
    return id;
}

AzID AzWindow::GetIDFromPos(const AzVec2& p_abs)
{
    AzID seed = IDStack.back();
    AzVec2 p_rel = Az::WindowPosAbsToRel(this, p_abs);
    AzID id = AzHashData(&p_rel, sizeof(p_rel), seed);
    return id;
}

AzID AzWindow::GetIDFromRectangle(const AzRect& r_abs)
{
    AzID seed = IDStack.back();
    AzRect r_rel = Az::WindowRectAbsToRel(this, r_abs);
    AzID id = AzHashData(&r_rel, sizeof(r_rel), seed);
    return id;
}

void Az::PushID(const char* str_id)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzID id = window->GetID(str_id);
    window->IDStack.push_back(id);
}

void Az::PushID(const char* str_id_begin, const char* str_id_end)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzID id = window->GetID(str_id_begin, str_id_end);
    window->IDStack.push_back(id);
}

void Az::PushID(const void* ptr_id)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzID id = window->GetID(ptr_id);
    window->IDStack.push_back(id);
}

void Az::PushID(int int_id)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzID id = window->GetID(int_id);
    window->IDStack.push_back(id);
}

void Az::PushOverrideID(AzID id)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (g.DebugHookIdInfo == id)
        DebugHookIdInfo(id, AzDataType_ID, NULL, NULL);
#endif
    window->IDStack.push_back(id);
}

AzID Az::GetIDWithSeed(const char* str, const char* str_end, AzID seed)
{
    AzID id = AzHashStr(str, str_end ? (str_end - str) : 0, seed);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *GAz;
    if (g.DebugHookIdInfo == id)
        DebugHookIdInfo(id, AzDataType_String, str, str_end);
#endif
    return id;
}

AzID Az::GetIDWithSeed(int n, AzID seed)
{
    AzID id = AzHashData(&n, sizeof(n), seed);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *GAz;
    if (g.DebugHookIdInfo == id)
        DebugHookIdInfo(id, AzDataType_S32, (void*)(intptr_t)n, NULL);
#endif
    return id;
}

void Az::PopID()
{
    AzWindow* window = GAz->CurrentWindow;
    if (window->IDStack.Size <= 1)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopID() too many times!");
        return;
    }
    window->IDStack.pop_back();
}

AzID Az::GetID(const char* str_id)
{
    AzWindow* window = GAz->CurrentWindow;
    return window->GetID(str_id);
}

AzID Az::GetID(const char* str_id_begin, const char* str_id_end)
{
    AzWindow* window = GAz->CurrentWindow;
    return window->GetID(str_id_begin, str_id_end);
}

AzID Az::GetID(const void* ptr_id)
{
    AzWindow* window = GAz->CurrentWindow;
    return window->GetID(ptr_id);
}

AzID Az::GetID(int int_id)
{
    AzWindow* window = GAz->CurrentWindow;
    return window->GetID(int_id);
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

static AzKeyChord GetModForLRModKey(AzKey key)
{
    if (key == AzKey_LeftCtrl || key == AzKey_RightCtrl)
        return AzMod_Ctrl;
    if (key == AzKey_LeftShift || key == AzKey_RightShift)
        return AzMod_Shift;
    if (key == AzKey_LeftAlt || key == AzKey_RightAlt)
        return AzMod_Alt;
    if (key == AzKey_LeftSuper || key == AzKey_RightSuper)
        return AzMod_Super;
    return AzMod_None;
}

AzKeyChord Az::FixupKeyChord(AzKeyChord key_chord)
{
    AzKey key = (AzKey)(key_chord & ~AzMod_Mask_);
    if (IsLRModKey(key))
        key_chord |= GetModForLRModKey(key);
    return key_chord;
}

AzKeyData* Az::GetKeyData(AzContext* ctx, AzKey key)
{
    AzContext& g = *ctx;

    if (key & AzMod_Mask_)
        key = ConvertSingleModFlagToKey(key);

    AZ_ASSERT(IsNamedKey(key) && "Support for user key indices was dropped in favor of AzKey. Please update backend & user code.");
    return &g.IO.KeysData[key - AzKey_NamedKey_BEGIN];
}

static const char* const GKeyNames[] = {
        "Tab",
        "LeftArrow",
        "RightArrow",
        "UpArrow",
        "DownArrow",
        "PageUp",
        "PageDown",
        "Home",
        "End",
        "Insert",
        "Delete",
        "Backspace",
        "Space",
        "Enter",
        "Escape",
        "LeftCtrl",
        "LeftShift",
        "LeftAlt",
        "LeftSuper",
        "RightCtrl",
        "RightShift",
        "RightAlt",
        "RightSuper",
        "Menu",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "Apostrophe",
        "Comma",
        "Minus",
        "Period",
        "Slash",
        "Semicolon",
        "Equal",
        "LeftBracket",
        "Backslash",
        "RightBracket",
        "GraveAccent",
        "CapsLock",
        "ScrollLock",
        "NumLock",
        "PrintScreen",
        "Pause",
        "Keypad0",
        "Keypad1",
        "Keypad2",
        "Keypad3",
        "Keypad4",
        "Keypad5",
        "Keypad6",
        "Keypad7",
        "Keypad8",
        "Keypad9",
        "KeypadDecimal",
        "KeypadDivide",
        "KeypadMultiply",
        "KeypadSubtract",
        "KeypadAdd",
        "KeypadEnter",
        "KeypadEqual",
        "AppBack",
        "AppForward",
        "Oem102",
        "GamepadStart",
        "GamepadBack",
        "GamepadFaceLeft",
        "GamepadFaceRight",
        "GamepadFaceUp",
        "GamepadFaceDown",
        "GamepadDpadLeft",
        "GamepadDpadRight",
        "GamepadDpadUp",
        "GamepadDpadDown",
        "GamepadL1",
        "GamepadR1",
        "GamepadL2",
        "GamepadR2",
        "GamepadL3",
        "GamepadR3",
        "GamepadLStickLeft",
        "GamepadLStickRight",
        "GamepadLStickUp",
        "GamepadLStickDown",
        "GamepadRStickLeft",
        "GamepadRStickRight",
        "GamepadRStickUp",
        "GamepadRStickDown",
        "MouseLeft",
        "MouseRight",
        "MouseMiddle",
        "MouseX1",
        "MouseX2",
        "MouseWheelX",
        "MouseWheelY",
        "ModCtrl",
        "ModShift",
        "ModAlt",
        "ModSuper",
};
AZ_STATIC_ASSERT(AzKey_NamedKey_COUNT == AZ_ARRAYSIZE(GKeyNames));

const char* Az::GetKeyName(AzKey key)
{
    if (key == AzKey_None)
        return "None";
    AZ_ASSERT(IsNamedKeyOrMod(key) && "Support for user key indices was dropped in favor of AzKey. Please update backend and user code.");
    if (key & AzMod_Mask_)
        key = ConvertSingleModFlagToKey(key);
    if (!IsNamedKey(key))
        return "Unknown";

    return GKeyNames[key - AzKey_NamedKey_BEGIN];
}

const char* Az::GetKeyChordName(AzKeyChord key_chord)
{
    AzContext& g = *GAz;

    const AzKey key = (AzKey)(key_chord & ~AzMod_Mask_);
    if (IsLRModKey(key))
        key_chord &= ~GetModForLRModKey(key);

    AzFormatString(g.TempKeychordName,
                   AZ_ARRAYSIZE(g.TempKeychordName),
                   "%s%s%s%s%s",
                   (key_chord & AzMod_Ctrl) ? "Ctrl+" : "",
                   (key_chord & AzMod_Shift) ? "Shift+" : "",
                   (key_chord & AzMod_Alt) ? "Alt+" : "",
                   (key_chord & AzMod_Super) ? "Super+" : "",
                   (key != AzKey_None || key_chord == AzKey_None)
                           ? GetKeyName(key)
                           : "");
    size_t len;
    if (key == AzKey_None && key_chord != 0)
        if ((len = AzStrlen(g.TempKeychordName)) != 0)
            g.TempKeychordName[len - 1] = 0;
    return g.TempKeychordName;
}

int Az::CalcTypematicRepeatAmount(float t0,
                                  float t1,
                                  float repeat_delay,
                                  float repeat_rate)
{
    if (t1 == 0.0f)
        return 1;
    if (t0 >= t1)
        return 0;
    if (repeat_rate <= 0.0f)
        return (t0 < repeat_delay) && (t1 >= repeat_delay);
    const int count_t0 = (t0 < repeat_delay)
                                 ? -1
                                 : (int)((t0 - repeat_delay) / repeat_rate);
    const int count_t1 = (t1 < repeat_delay)
                                 ? -1
                                 : (int)((t1 - repeat_delay) / repeat_rate);
    const int count = count_t1 - count_t0;
    return count;
}

void Az::GetTypematicRepeatRate(AzInputFlags flags,
                                float* repeat_delay,
                                float* repeat_rate)
{
    AzContext& g = *GAz;
    switch (flags & AzInputFlags_RepeatRateMask_)
    {
    case AzInputFlags_RepeatRateNavMove:
        *repeat_delay = g.IO.KeyRepeatDelay * 0.72f;
        *repeat_rate = g.IO.KeyRepeatRate * 0.80f;
        return;
    case AzInputFlags_RepeatRateNavTweak:
        *repeat_delay = g.IO.KeyRepeatDelay * 0.72f;
        *repeat_rate = g.IO.KeyRepeatRate * 0.30f;
        return;
    case AzInputFlags_RepeatRateDefault:
    default:
        *repeat_delay = g.IO.KeyRepeatDelay * 1.00f;
        *repeat_rate = g.IO.KeyRepeatRate * 1.00f;
        return;
    }
}

int Az::GetKeyPressedAmount(AzKey key, float repeat_delay, float repeat_rate)
{
    AzContext& g = *GAz;
    const AzKeyData* key_data = GetKeyData(key);
    if (!key_data->Down)

        return 0;
    const float t = key_data->DownDuration;
    return CalcTypematicRepeatAmount(t - g.IO.DeltaTime,
                                     t,
                                     repeat_delay,
                                     repeat_rate);
}

AzVec2 Az::GetKeyMagnitude2d(AzKey key_left,
                             AzKey key_right,
                             AzKey key_up,
                             AzKey key_down)
{
    return AzVec2(GetKeyData(key_right)->AnalogValue
                          - GetKeyData(key_left)->AnalogValue,
                  GetKeyData(key_down)->AnalogValue
                          - GetKeyData(key_up)->AnalogValue);
}

static void Az::UpdateKeyRoutingTable(AzKeyRoutingTable* rt)
{
    AzContext& g = *GAz;
    rt->EntriesNext.resize(0);
    for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
         key = (AzKey)(key + 1))
    {
        const int new_routing_start_idx = rt->EntriesNext.Size;
        AzKeyRoutingData* routing_entry;
        for (int old_routing_idx = rt->Index[key - AzKey_NamedKey_BEGIN];
             old_routing_idx != -1;
             old_routing_idx = routing_entry->NextEntryIndex)
        {
            routing_entry = &rt->Entries[old_routing_idx];
            routing_entry->RoutingCurrScore = routing_entry->RoutingNextScore;
            routing_entry->RoutingCurr = routing_entry->RoutingNext;

            routing_entry->RoutingNext = AzKeyOwner_NoOwner;
            routing_entry->RoutingNextScore = 255;
            if (routing_entry->RoutingCurr == AzKeyOwner_NoOwner)
                continue;
            rt->EntriesNext.push_back(*routing_entry);

            if (routing_entry->Mods == g.IO.KeyMods)
            {
                AzKeyOwnerData* owner_data = GetKeyOwnerData(&g, key);
                if (owner_data->OwnerCurr == AzKeyOwner_NoOwner)
                {
                    owner_data->OwnerCurr = routing_entry->RoutingCurr;
                }
            }
        }

        rt->Index[key - AzKey_NamedKey_BEGIN] =
                (AzKeyRoutingIndex)(new_routing_start_idx < rt->EntriesNext.Size
                                            ? new_routing_start_idx
                                            : -1);
        for (int n = new_routing_start_idx; n < rt->EntriesNext.Size; n++)
            rt->EntriesNext[n].NextEntryIndex =
                    (AzKeyRoutingIndex)((n + 1 < rt->EntriesNext.Size) ? n + 1
                                                                       : -1);
    }
    rt->Entries.swap(rt->EntriesNext);
}

static inline AzID GetRoutingIdFromOwnerId(AzID owner_id)
{
    AzContext& g = *GAz;
    return (owner_id != AzKeyOwner_NoOwner && owner_id != AzKeyOwner_Any)
                   ? owner_id
                   : g.CurrentFocusScopeId;
}

AzKeyRoutingData* Az::GetShortcutRoutingData(AzKeyChord key_chord)
{
    AzContext& g = *GAz;
    AzKeyRoutingTable* rt = &g.KeysRoutingTable;
    AzKeyRoutingData* routing_data;
    AzKey key = (AzKey)(key_chord & ~AzMod_Mask_);
    AzKey mods = (AzKey)(key_chord & AzMod_Mask_);
    if (key == AzKey_None)
        key = ConvertSingleModFlagToKey(mods);
    AZ_ASSERT(IsNamedKey(key));

    for (AzKeyRoutingIndex idx = rt->Index[key - AzKey_NamedKey_BEGIN];
         idx != -1;
         idx = routing_data->NextEntryIndex)
    {
        routing_data = &rt->Entries[idx];
        if (routing_data->Mods == mods)
            return routing_data;
    }

    AzKeyRoutingIndex routing_data_idx = (AzKeyRoutingIndex)rt->Entries.Size;
    rt->Entries.push_back(AzKeyRoutingData());
    routing_data = &rt->Entries[routing_data_idx];
    routing_data->Mods = (AzU16)mods;
    routing_data->NextEntryIndex = rt->Index[key - AzKey_NamedKey_BEGIN];
    rt->Index[key - AzKey_NamedKey_BEGIN] = routing_data_idx;
    return routing_data;
}

static int CalcRoutingScore(AzID focus_scope_id,
                            AzID owner_id,
                            AzInputFlags flags)
{
    AzContext& g = *GAz;
    if (flags & AzInputFlags_RouteFocused)
    {
        if (owner_id != 0 && g.ActiveId == owner_id)
            return 1;

        if (focus_scope_id == 0)
            return 255;
        for (int index_in_focus_path = 0;
             index_in_focus_path < g.NavFocusRoute.Size;
             index_in_focus_path++)
            if (g.NavFocusRoute.Data[index_in_focus_path].ID == focus_scope_id)
                return 3 + index_in_focus_path;
        return 255;
    } else if (flags & AzInputFlags_RouteActive)
    {
        if (owner_id != 0 && g.ActiveId == owner_id)
            return 1;
        return 255;
    } else if (flags & AzInputFlags_RouteGlobal)
    {
        if (flags & AzInputFlags_RouteOverActive)
            return 0;
        if (flags & AzInputFlags_RouteOverFocused)
            return 2;
        return 254;
    }
    AZ_ASSERT(0);
    return 0;
}

static bool IsKeyChordPotentiallyCharInput(AzKeyChord key_chord)
{
    AzContext& g = *GAz;

    AzKey mods = (AzKey)(key_chord & AzMod_Mask_);
    const bool ignore_char_inputs = ((mods & AzMod_Ctrl) && !(mods & AzMod_Alt))
                                    || (g.IO.ConfigMacOSXBehaviors
                                        && (mods & AzMod_Ctrl));
    if (ignore_char_inputs)
        return false;

    AzKey key = (AzKey)(key_chord & ~AzMod_Mask_);
    if (key == AzKey_None)
        return false;
    return g.KeysMayBeCharInput.TestBit(key);
}

bool Az::SetShortcutRouting(AzKeyChord key_chord,
                            AzInputFlags flags,
                            AzID owner_id)
{
    AzContext& g = *GAz;
    if ((flags & AzInputFlags_RouteTypeMask_) == 0)
        flags |= AzInputFlags_RouteGlobal | AzInputFlags_RouteOverFocused
                 | AzInputFlags_RouteOverActive;

    else
        AZ_ASSERT(AzIsPowerOfTwo(flags & AzInputFlags_RouteTypeMask_));

    AZ_ASSERT(owner_id != AzKeyOwner_Any && owner_id != AzKeyOwner_NoOwner);
    if (flags
        & (AzInputFlags_RouteOverFocused | AzInputFlags_RouteOverActive
           | AzInputFlags_RouteUnlessBgFocused))
        AZ_ASSERT(flags & AzInputFlags_RouteGlobal);

    key_chord = FixupKeyChord(key_chord);

    if (g.DebugBreakInShortcutRouting == key_chord)
        AZ_DEBUG_BREAK();

    if (flags & AzInputFlags_RouteUnlessBgFocused)
        if (g.NavWindow == NULL)
            return false;

    if (flags & AzInputFlags_RouteAlways)
    {
        AZ_DEBUG_LOG_INPUTROUTING(
                "SetShortcutRouting(%s, flags=%04X, owner_id=0x%08X) -> "
                "always, no register\n",
                GetKeyChordName(key_chord),
                flags,
                owner_id);
        return true;
    }

    if (g.ActiveId != 0 && g.ActiveId != owner_id)
    {
        if (flags & AzInputFlags_RouteActive)
            return false;

        if (g.IO.WantTextInput && IsKeyChordPotentiallyCharInput(key_chord))
        {
            AZ_DEBUG_LOG_INPUTROUTING(
                    "SetShortcutRouting(%s, flags=%04X, owner_id=0x%08X) -> "
                    "filtered as potential char input\n",
                    GetKeyChordName(key_chord),
                    flags,
                    owner_id);
            return false;
        }

        if ((flags & AzInputFlags_RouteOverActive) == 0
            && g.ActiveIdUsingAllKeyboardKeys)
        {
            AzKey key = (AzKey)(key_chord & ~AzMod_Mask_);
            if (key == AzKey_None)
                key = ConvertSingleModFlagToKey(
                        (AzKey)(key_chord & AzMod_Mask_));
            if (key >= AzKey_Keyboard_BEGIN && key < AzKey_Keyboard_END)
                return false;
        }
    }

    AzID focus_scope_id = g.CurrentFocusScopeId;
    if (flags & AzInputFlags_RouteFromRootWindow)
        focus_scope_id = g.CurrentWindow->RootWindow->ID;

    const int score = CalcRoutingScore(focus_scope_id, owner_id, flags);
    AZ_DEBUG_LOG_INPUTROUTING(
            "SetShortcutRouting(%s, flags=%04X, owner_id=0x%08X) -> score %d\n",
            GetKeyChordName(key_chord),
            flags,
            owner_id,
            score);
    if (score == 255)
        return false;

    AzKeyRoutingData* routing_data = GetShortcutRoutingData(key_chord);

    if (score < routing_data->RoutingNextScore)
    {
        routing_data->RoutingNext = owner_id;
        routing_data->RoutingNextScore = (AzU8)score;
    }

    if (routing_data->RoutingCurr == owner_id)
        AZ_DEBUG_LOG_INPUTROUTING("--> granting current route\n");
    return routing_data->RoutingCurr == owner_id;
}

bool Az::TestShortcutRouting(AzKeyChord key_chord, AzID owner_id)
{
    const AzID routing_id = GetRoutingIdFromOwnerId(owner_id);
    key_chord = FixupKeyChord(key_chord);
    AzKeyRoutingData* routing_data = GetShortcutRoutingData(key_chord);
    return routing_data->RoutingCurr == routing_id;
}

bool Az::IsKeyDown(AzKey key)
{
    return IsKeyDown(key, AzKeyOwner_Any);
}

bool Az::IsKeyDown(AzKey key, AzID owner_id)
{
    const AzKeyData* key_data = GetKeyData(key);
    if (!key_data->Down)
        return false;
    if (!TestKeyOwner(key, owner_id))
        return false;
    return true;
}

bool Az::IsKeyPressed(AzKey key, bool repeat)
{
    return IsKeyPressed(key,
                        repeat ? AzInputFlags_Repeat : AzInputFlags_None,
                        AzKeyOwner_Any);
}

bool Az::IsKeyPressed(AzKey key, AzInputFlags flags, AzID owner_id)
{
    const AzKeyData* key_data = GetKeyData(key);
    if (!key_data->Down)

        return false;
    const float t = key_data->DownDuration;
    if (t < 0.0f)
        return false;
    AZ_ASSERT((flags & ~AzInputFlags_SupportedByIsKeyPressed) == 0);
    if (flags & (AzInputFlags_RepeatRateMask_ | AzInputFlags_RepeatUntilMask_))

        flags |= AzInputFlags_Repeat;

    bool pressed = (t == 0.0f);
    if (!pressed && (flags & AzInputFlags_Repeat) != 0)
    {
        float repeat_delay, repeat_rate;
        GetTypematicRepeatRate(flags, &repeat_delay, &repeat_rate);
        pressed = (t > repeat_delay)
                  && GetKeyPressedAmount(key, repeat_delay, repeat_rate) > 0;
        if (pressed && (flags & AzInputFlags_RepeatUntilMask_))
        {
            AzContext& g = *GAz;
            double key_pressed_time = g.Time - t + 0.00001f;
            if ((flags & AzInputFlags_RepeatUntilKeyModsChange)
                && (g.LastKeyModsChangeTime > key_pressed_time))
                pressed = false;
            if ((flags & AzInputFlags_RepeatUntilKeyModsChangeFromNone)
                && (g.LastKeyModsChangeFromNoneTime > key_pressed_time))
                pressed = false;
            if ((flags & AzInputFlags_RepeatUntilOtherKeyPress)
                && (g.LastKeyboardKeyPressTime > key_pressed_time))
                pressed = false;
        }
    }
    if (!pressed)
        return false;
    if (!TestKeyOwner(key, owner_id))
        return false;
    return true;
}

bool Az::IsKeyReleased(AzKey key)
{
    return IsKeyReleased(key, AzKeyOwner_Any);
}

bool Az::IsKeyReleased(AzKey key, AzID owner_id)
{
    const AzKeyData* key_data = GetKeyData(key);
    if (key_data->DownDurationPrev < 0.0f || key_data->Down)
        return false;
    if (!TestKeyOwner(key, owner_id))
        return false;
    return true;
}

bool Az::IsMouseDown(AzMouseButton button)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDown[button]
           && TestKeyOwner(MouseButtonToKey(button), AzKeyOwner_Any);
}

bool Az::IsMouseDown(AzMouseButton button, AzID owner_id)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDown[button]
           && TestKeyOwner(MouseButtonToKey(button), owner_id);
}

bool Az::IsMouseClicked(AzMouseButton button, bool repeat)
{
    return IsMouseClicked(button,
                          repeat ? AzInputFlags_Repeat : AzInputFlags_None,
                          AzKeyOwner_Any);
}

bool Az::IsMouseClicked(AzMouseButton button, AzInputFlags flags, AzID owner_id)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    if (!g.IO.MouseDown[button])

        return false;
    const float t = g.IO.MouseDownDuration[button];
    if (t < 0.0f)
        return false;
    AZ_ASSERT((flags & ~AzInputFlags_SupportedByIsMouseClicked) == 0);

    const bool repeat = (flags & AzInputFlags_Repeat) != 0;
    const bool pressed = (t == 0.0f)
                         || (repeat && t > g.IO.KeyRepeatDelay
                             && CalcTypematicRepeatAmount(t - g.IO.DeltaTime,
                                                          t,
                                                          g.IO.KeyRepeatDelay,
                                                          g.IO.KeyRepeatRate)
                                        > 0);
    if (!pressed)
        return false;

    if (!TestKeyOwner(MouseButtonToKey(button), owner_id))
        return false;

    return true;
}

bool Az::IsMouseReleased(AzMouseButton button)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseReleased[button]
           && TestKeyOwner(MouseButtonToKey(button), AzKeyOwner_Any);
}

bool Az::IsMouseReleased(AzMouseButton button, AzID owner_id)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseReleased[button]
           && TestKeyOwner(MouseButtonToKey(button), owner_id);
}

bool Az::IsMouseReleasedWithDelay(AzMouseButton button, float delay)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    const float time_since_release = (float)(g.Time
                                             - g.IO.MouseReleasedTime[button]);
    return !IsMouseDown(button) && (time_since_release - g.IO.DeltaTime < delay)
           && (time_since_release >= delay);
}

bool Az::IsMouseDoubleClicked(AzMouseButton button)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseClickedCount[button] == 2
           && TestKeyOwner(MouseButtonToKey(button), AzKeyOwner_Any);
}

bool Az::IsMouseDoubleClicked(AzMouseButton button, AzID owner_id)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseClickedCount[button] == 2
           && TestKeyOwner(MouseButtonToKey(button), owner_id);
}

int Az::GetMouseClickedCount(AzMouseButton button)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseClickedCount[button];
}

bool Az::IsMouseHoveringRect(const AzVec2& r_min,
                             const AzVec2& r_max,
                             bool clip)
{
    AzContext& g = *GAz;

    AzRect rect_clipped(r_min, r_max);
    if (clip)
        rect_clipped.ClipWith(g.CurrentWindow->ClipRect);

    if (!rect_clipped.ContainsWithPad(g.IO.MousePos, g.Style.TouchExtraPadding))
        return false;
    if (!g.MouseViewport->GetMainRect().Overlaps(rect_clipped))
        return false;
    return true;
}

bool Az::IsMouseDragPastThreshold(AzMouseButton button, float lock_threshold)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    if (lock_threshold < 0.0f)
        lock_threshold = g.IO.MouseDragThreshold;
    return g.IO.MouseDragMaxDistanceSqr[button]
           >= lock_threshold * lock_threshold;
}

bool Az::IsMouseDragging(AzMouseButton button, float lock_threshold)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    if (!g.IO.MouseDown[button])
        return false;
    return IsMouseDragPastThreshold(button, lock_threshold);
}

AzVec2 Az::GetMousePos()
{
    AzContext& g = *GAz;
    return g.IO.MousePos;
}

void Az::TeleportMousePos(const AzVec2& pos)
{
    AzContext& g = *GAz;
    g.IO.MousePos = g.IO.MousePosPrev = pos;
    g.IO.MouseDelta = AzVec2(0.0f, 0.0f);
    g.IO.WantSetMousePos = true;
}

AzVec2 Az::GetMousePosOnOpeningCurrentPopup()
{
    AzContext& g = *GAz;
    if (g.BeginPopupStack.Size > 0)
        return g.OpenPopupStack[g.BeginPopupStack.Size - 1].OpenMousePos;
    return g.IO.MousePos;
}

bool Az::IsMousePosValid(const AzVec2* mouse_pos)
{
    AZ_ASSERT(GAz != NULL);
    const float MOUSE_INVALID = -256000.0f;
    AzVec2 p = mouse_pos ? *mouse_pos : GAz->IO.MousePos;
    return p.x >= MOUSE_INVALID && p.y >= MOUSE_INVALID;
}

bool Az::IsAnyMouseDown()
{
    AzContext& g = *GAz;
    for (int n = 0; n < AZ_ARRAYSIZE(g.IO.MouseDown); n++)
        if (g.IO.MouseDown[n])
            return true;
    return false;
}

AzVec2 Az::GetMouseDragDelta(AzMouseButton button, float lock_threshold)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));
    if (lock_threshold < 0.0f)
        lock_threshold = g.IO.MouseDragThreshold;
    if (g.IO.MouseDown[button] || g.IO.MouseReleased[button])
        if (g.IO.MouseDragMaxDistanceSqr[button]
            >= lock_threshold * lock_threshold)
            if (IsMousePosValid(&g.IO.MousePos)
                && IsMousePosValid(&g.IO.MouseClickedPos[button]))
                return g.IO.MousePos - g.IO.MouseClickedPos[button];
    return AzVec2(0.0f, 0.0f);
}

void Az::ResetMouseDragDelta(AzMouseButton button)
{
    AzContext& g = *GAz;
    AZ_ASSERT(button >= 0 && button < AZ_ARRAYSIZE(g.IO.MouseDown));

    g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

AzMouseCursor Az::GetMouseCursor()
{
    AzContext& g = *GAz;
    return g.MouseCursor;
}

void Az::SetMouseCursor(AzMouseCursor cursor_type)
{
    AzContext& g = *GAz;
    g.MouseCursor = cursor_type;
}

static void UpdateAliasKey(AzKey key, bool v, float analog_value)
{
    AZ_ASSERT(Az::IsAliasKey(key));
    AzKeyData* key_data = Az::GetKeyData(key);
    key_data->Down = v;
    key_data->AnalogValue = analog_value;
}

static AzKeyChord GetMergedModsFromKeys()
{
    AzKeyChord mods = 0;
    if (Az::IsKeyDown(AzMod_Ctrl))
    {
        mods |= AzMod_Ctrl;
    }
    if (Az::IsKeyDown(AzMod_Shift))
    {
        mods |= AzMod_Shift;
    }
    if (Az::IsKeyDown(AzMod_Alt))
    {
        mods |= AzMod_Alt;
    }
    if (Az::IsKeyDown(AzMod_Super))
    {
        mods |= AzMod_Super;
    }
    return mods;
}

static void Az::UpdateKeyboardInputs()
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;

    if (io.ConfigFlags & AzConfigFlags_NoKeyboard)
        io.ClearInputKeys();

    for (int n = 0; n < AzMouseButton_COUNT; n++)
        UpdateAliasKey(MouseButtonToKey(n),
                       io.MouseDown[n],
                       io.MouseDown[n] ? 1.0f : 0.0f);
    UpdateAliasKey(AzKey_MouseWheelX, io.MouseWheelH != 0.0f, io.MouseWheelH);
    UpdateAliasKey(AzKey_MouseWheelY, io.MouseWheel != 0.0f, io.MouseWheel);

    const AzKeyChord prev_key_mods = io.KeyMods;
    io.KeyMods = GetMergedModsFromKeys();
    io.KeyCtrl = (io.KeyMods & AzMod_Ctrl) != 0;
    io.KeyShift = (io.KeyMods & AzMod_Shift) != 0;
    io.KeyAlt = (io.KeyMods & AzMod_Alt) != 0;
    io.KeySuper = (io.KeyMods & AzMod_Super) != 0;
    if (prev_key_mods != io.KeyMods)
        g.LastKeyModsChangeTime = g.Time;
    if (prev_key_mods != io.KeyMods && prev_key_mods == 0)
        g.LastKeyModsChangeFromNoneTime = g.Time;

    if ((io.BackendFlags & AzBackendFlags_HasGamepad) == 0)
        for (int key = AzKey_Gamepad_BEGIN; key < AzKey_Gamepad_END; key++)
        {
            io.KeysData[key - AzKey_NamedKey_BEGIN].Down = false;
            io.KeysData[key - AzKey_NamedKey_BEGIN].AnalogValue = 0.0f;
        }

    for (int key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END; key++)
    {
        AzKeyData* key_data = &io.KeysData[key - AzKey_NamedKey_BEGIN];
        key_data->DownDurationPrev = key_data->DownDuration;
        key_data->DownDuration = key_data->Down
                                         ? (key_data->DownDuration < 0.0f
                                                    ? 0.0f
                                                    : key_data->DownDuration
                                                              + io.DeltaTime)
                                         : -1.0f;
        if (key_data->DownDuration == 0.0f)
        {
            if (IsKeyboardKey((AzKey)key))
                g.LastKeyboardKeyPressTime = g.Time;
            else if (key == AzKey_ReservedForModCtrl
                     || key == AzKey_ReservedForModShift
                     || key == AzKey_ReservedForModAlt
                     || key == AzKey_ReservedForModSuper)
                g.LastKeyboardKeyPressTime = g.Time;
        }
    }

    for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
         key = (AzKey)(key + 1))
    {
        AzKeyData* key_data = &io.KeysData[key - AzKey_NamedKey_BEGIN];
        AzKeyOwnerData* owner_data =
                &g.KeysOwnerData[key - AzKey_NamedKey_BEGIN];
        owner_data->OwnerCurr = owner_data->OwnerNext;
        if (!key_data->Down)

            owner_data->OwnerNext = AzKeyOwner_NoOwner;
        owner_data->LockThisFrame = owner_data->LockUntilRelease =
                owner_data->LockUntilRelease && key_data->Down;
    }

    UpdateKeyRoutingTable(&g.KeysRoutingTable);
}

static void Az::UpdateMouseInputs()
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;

    io.MouseWheelRequestAxisSwap = io.KeyShift && !io.ConfigMacOSXBehaviors;

    if (IsMousePosValid(&io.MousePos))
        io.MousePos = g.MouseLastValidPos = AzFloor(io.MousePos);

    if (IsMousePosValid(&io.MousePos) && IsMousePosValid(&io.MousePosPrev))
        io.MouseDelta = io.MousePos - io.MousePosPrev;
    else
        io.MouseDelta = AzVec2(0.0f, 0.0f);

    const float mouse_stationary_threshold = (io.MouseSource
                                              == AzMouseSource_Mouse)
                                                     ? 2.0f
                                                     : 3.0f;

    const bool mouse_stationary = (AzLengthSqr(io.MouseDelta)
                                   <= mouse_stationary_threshold
                                              * mouse_stationary_threshold);
    g.MouseStationaryTimer = mouse_stationary
                                     ? (g.MouseStationaryTimer + io.DeltaTime)
                                     : 0.0f;

    if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
        g.NavHighlightItemUnderNav = false;

    for (int i = 0; i < AZ_ARRAYSIZE(io.MouseDown); i++)
    {
        io.MouseClicked[i] = io.MouseDown[i] && io.MouseDownDuration[i] < 0.0f;
        io.MouseClickedCount[i] = 0;
        io.MouseReleased[i] = !io.MouseDown[i]
                              && io.MouseDownDuration[i] >= 0.0f;
        if (io.MouseReleased[i])
            io.MouseReleasedTime[i] = g.Time;
        io.MouseDownDurationPrev[i] = io.MouseDownDuration[i];
        io.MouseDownDuration[i] = io.MouseDown[i]
                                          ? (io.MouseDownDuration[i] < 0.0f
                                                     ? 0.0f
                                                     : io.MouseDownDuration[i]
                                                               + io.DeltaTime)
                                          : -1.0f;
        if (io.MouseClicked[i])
        {
            bool is_repeated_click = false;
            if ((float)(g.Time - io.MouseClickedTime[i])
                < io.MouseDoubleClickTime)
            {
                AzVec2 delta_from_click_pos =
                        IsMousePosValid(&io.MousePos)
                                ? (io.MousePos - io.MouseClickedPos[i])
                                : AzVec2(0.0f, 0.0f);
                if (AzLengthSqr(delta_from_click_pos)
                    < io.MouseDoubleClickMaxDist * io.MouseDoubleClickMaxDist)
                    is_repeated_click = true;
            }
            if (is_repeated_click)
                io.MouseClickedLastCount[i]++;
            else
                io.MouseClickedLastCount[i] = 1;
            io.MouseClickedTime[i] = g.Time;
            io.MouseClickedPos[i] = io.MousePos;
            io.MouseClickedCount[i] = io.MouseClickedLastCount[i];
            io.MouseDragMaxDistanceAbs[i] = AzVec2(0.0f, 0.0f);
            io.MouseDragMaxDistanceSqr[i] = 0.0f;
        } else if (io.MouseDown[i])
        {
            AzVec2 delta_from_click_pos = IsMousePosValid(&io.MousePos)
                                                  ? (io.MousePos
                                                     - io.MouseClickedPos[i])
                                                  : AzVec2(0.0f, 0.0f);
            io.MouseDragMaxDistanceSqr[i] =
                    AzMax(io.MouseDragMaxDistanceSqr[i],
                          AzLengthSqr(delta_from_click_pos));
            io.MouseDragMaxDistanceAbs[i].x = AzMax(
                    io.MouseDragMaxDistanceAbs[i].x,
                    delta_from_click_pos.x < 0.0f ? -delta_from_click_pos.x
                                                  : delta_from_click_pos.x);
            io.MouseDragMaxDistanceAbs[i].y = AzMax(
                    io.MouseDragMaxDistanceAbs[i].y,
                    delta_from_click_pos.y < 0.0f ? -delta_from_click_pos.y
                                                  : delta_from_click_pos.y);
        }

        io.MouseDoubleClicked[i] = (io.MouseClickedCount[i] == 2);

        if (io.MouseClicked[i])
            g.NavHighlightItemUnderNav = false;
    }
}

static void LockWheelingWindow(AzWindow* window, float wheel_amount)
{
    AzContext& g = *GAz;
    if (window)
        g.WheelingWindowReleaseTimer =
                AzMin(g.WheelingWindowReleaseTimer
                              + AzAbs(wheel_amount)
                                        * WINDOWS_MOUSE_WHEEL_SCROLL_LOCK_TAZER,
                      WINDOWS_MOUSE_WHEEL_SCROLL_LOCK_TAZER);
    else
        g.WheelingWindowReleaseTimer = 0.0f;
    if (g.WheelingWindow == window)
        return;
    AZ_DEBUG_LOG_IO("[io] LockWheelingWindow() \"%s\"\n",
                    window ? window->Name : "NULL");
    g.WheelingWindow = window;
    g.WheelingWindowRefMousePos = g.IO.MousePos;
    if (window == NULL)
    {
        g.WheelingWindowStartFrame = -1;
        g.WheelingAxisAvg = AzVec2(0.0f, 0.0f);
    }
}

static AzWindow* FindBestWheelingWindow(const AzVec2& wheel)
{
    AzContext& g = *GAz;
    AzWindow* windows[2] = {NULL, NULL};
    for (int axis = 0; axis < 2; axis++)
        if (wheel[axis] != 0.0f)
            for (AzWindow* window = windows[axis] = g.HoveredWindow;
                 window->Flags & AzWindowFlags_ChildWindow;
                 window = windows[axis] = window->ParentWindow)
            {
                const bool has_scrolling = (window->ScrollMax[axis] != 0.0f);
                const bool inputs_disabled =
                        (window->Flags & AzWindowFlags_NoScrollWithMouse)
                        && !(window->Flags & AzWindowFlags_NoMouseInputs);

                if (has_scrolling && !inputs_disabled)
                    break;
            }
    if (windows[0] == NULL && windows[1] == NULL)
        return NULL;

    if (windows[0] == windows[1] || windows[0] == NULL || windows[1] == NULL)
        return windows[1] ? windows[1] : windows[0];

    if (g.WheelingWindowStartFrame == -1)
        g.WheelingWindowStartFrame = g.FrameCount;
    if ((g.WheelingWindowStartFrame == g.FrameCount && wheel.x != 0.0f
         && wheel.y != 0.0f)
        || (g.WheelingAxisAvg.x == g.WheelingAxisAvg.y))
    {
        g.WheelingWindowWheelRemainder = wheel;
        return NULL;
    }
    return (g.WheelingAxisAvg.x > g.WheelingAxisAvg.y) ? windows[0]
                                                       : windows[1];
}

void Az::UpdateMouseWheel()
{
    AzContext& g = *GAz;
    if (g.WheelingWindow != NULL)
    {
        g.WheelingWindowReleaseTimer -= g.IO.DeltaTime;
        if (IsMousePosValid()
            && AzLengthSqr(g.IO.MousePos - g.WheelingWindowRefMousePos)
                       > g.IO.MouseDragThreshold * g.IO.MouseDragThreshold)
            g.WheelingWindowReleaseTimer = 0.0f;
        if (g.WheelingWindowReleaseTimer <= 0.0f)
            LockWheelingWindow(NULL, 0.0f);
    }

    AzVec2 wheel;
    wheel.x = TestKeyOwner(AzKey_MouseWheelX, AzKeyOwner_NoOwner)
                      ? g.IO.MouseWheelH
                      : 0.0f;
    wheel.y = TestKeyOwner(AzKey_MouseWheelY, AzKeyOwner_NoOwner)
                      ? g.IO.MouseWheel
                      : 0.0f;

    AzWindow* mouse_window = g.WheelingWindow ? g.WheelingWindow
                                              : g.HoveredWindow;
    if (!mouse_window || mouse_window->Collapsed)
        return;

    if (wheel.y != 0.0f && g.IO.KeyCtrl && g.IO.FontAllowUserScaling)
    {
        LockWheelingWindow(mouse_window, wheel.y);
        AzWindow* window = mouse_window;
        const float new_font_scale = AzClamp(window->FontWindowScale
                                                     + g.IO.MouseWheel * 0.10f,
                                             0.50f,
                                             2.50f);
        const float scale = new_font_scale / window->FontWindowScale;
        window->FontWindowScale = new_font_scale;
        if (window == window->RootWindow)
        {
            const AzVec2 offset = window->Size * (1.0f - scale)
                                  * (g.IO.MousePos - window->Pos)
                                  / window->Size;
            SetWindowPos(window, window->Pos + offset, 0);
            window->Size = AzTrunc(window->Size * scale);
            window->SizeFull = AzTrunc(window->SizeFull * scale);
        }
        return;
    }
    if (g.IO.KeyCtrl)
        return;

    if (g.IO.MouseWheelRequestAxisSwap)
        wheel = AzVec2(wheel.y, 0.0f);

    g.WheelingAxisAvg.x = AzExponentialMovingAverage(g.WheelingAxisAvg.x,
                                                     AzAbs(wheel.x),
                                                     30);
    g.WheelingAxisAvg.y = AzExponentialMovingAverage(g.WheelingAxisAvg.y,
                                                     AzAbs(wheel.y),
                                                     30);

    wheel += g.WheelingWindowWheelRemainder;
    g.WheelingWindowWheelRemainder = AzVec2(0.0f, 0.0f);
    if (wheel.x == 0.0f && wheel.y == 0.0f)
        return;

    if (AzWindow* window = (g.WheelingWindow ? g.WheelingWindow
                                             : FindBestWheelingWindow(wheel)))
        if (!(window->Flags & AzWindowFlags_NoScrollWithMouse)
            && !(window->Flags & AzWindowFlags_NoMouseInputs))
        {
            bool do_scroll[2] = {wheel.x != 0.0f && window->ScrollMax.x != 0.0f,
                                 wheel.y != 0.0f
                                         && window->ScrollMax.y != 0.0f};
            if (do_scroll[AzAxis_X] && do_scroll[AzAxis_Y])
                do_scroll[(g.WheelingAxisAvg.x > g.WheelingAxisAvg.y)
                                  ? AzAxis_Y
                                  : AzAxis_X] = false;
            if (do_scroll[AzAxis_X])
            {
                LockWheelingWindow(window, wheel.x);
                float max_step = window->InnerRect.GetWidth() * 0.67f;
                float scroll_step = AzTrunc(
                        AzMin(2 * window->FontRefSize, max_step));
                SetScrollX(window, window->Scroll.x - wheel.x * scroll_step);
                g.WheelingWindowScrolledFrame = g.FrameCount;
            }
            if (do_scroll[AzAxis_Y])
            {
                LockWheelingWindow(window, wheel.y);
                float max_step = window->InnerRect.GetHeight() * 0.67f;
                float scroll_step = AzTrunc(
                        AzMin(5 * window->FontRefSize, max_step));
                SetScrollY(window, window->Scroll.y - wheel.y * scroll_step);
                g.WheelingWindowScrolledFrame = g.FrameCount;
            }
        }
}

void Az::SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard)
{
    AzContext& g = *GAz;
    g.WantCaptureKeyboardNextFrame = want_capture_keyboard ? 1 : 0;
}

void Az::SetNextFrameWantCaptureMouse(bool want_capture_mouse)
{
    AzContext& g = *GAz;
    g.WantCaptureMouseNextFrame = want_capture_mouse ? 1 : 0;
}

#ifndef AZ_DISABLE_DEBUG_TOOLS
static const char* GetInputSourceName(AzInputSource source)
{
    const char* input_source_names[] = {"None", "Mouse", "Keyboard", "Gamepad"};
    AZ_ASSERT(AZ_ARRAYSIZE(input_source_names) == AzInputSource_COUNT
              && source >= 0 && source < AzInputSource_COUNT);
    return input_source_names[source];
}
static const char* GetMouseSourceName(AzMouseSource source)
{
    const char* mouse_source_names[] = {"Mouse", "TouchScreen", "Pen"};
    AZ_ASSERT(AZ_ARRAYSIZE(mouse_source_names) == AzMouseSource_COUNT
              && source >= 0 && source < AzMouseSource_COUNT);
    return mouse_source_names[source];
}
static void DebugPrintInputEvent(const char* prefix, const AzInputEvent* e)
{
    AzContext& g = *GAz;
    if (e->Type == AzInputEventType_MousePos)
    {
        if (e->MousePos.PosX == -FLT_MAX && e->MousePos.PosY == -FLT_MAX)
            AZ_DEBUG_LOG_IO("[io] %s: MousePos (-FLT_MAX, -FLT_MAX)\n", prefix);
        else
            AZ_DEBUG_LOG_IO("[io] %s: MousePos (%.1f, %.1f) (%s)\n",
                            prefix,
                            e->MousePos.PosX,
                            e->MousePos.PosY,
                            GetMouseSourceName(e->MousePos.MouseSource));
        return;
    }
    if (e->Type == AzInputEventType_MouseButton)
    {
        AZ_DEBUG_LOG_IO("[io] %s: MouseButton %d %s (%s)\n",
                        prefix,
                        e->MouseButton.Button,
                        e->MouseButton.Down ? "Down" : "Up",
                        GetMouseSourceName(e->MouseButton.MouseSource));
        return;
    }
    if (e->Type == AzInputEventType_MouseWheel)
    {
        AZ_DEBUG_LOG_IO("[io] %s: MouseWheel (%.3f, %.3f) (%s)\n",
                        prefix,
                        e->MouseWheel.WheelX,
                        e->MouseWheel.WheelY,
                        GetMouseSourceName(e->MouseWheel.MouseSource));
        return;
    }
    if (e->Type == AzInputEventType_MouseViewport)
    {
        AZ_DEBUG_LOG_IO("[io] %s: MouseViewport (0x%08X)\n",
                        prefix,
                        e->MouseViewport.HoveredViewportID);
        return;
    }
    if (e->Type == AzInputEventType_Key)
    {
        AZ_DEBUG_LOG_IO("[io] %s: Key \"%s\" %s\n",
                        prefix,
                        Az::GetKeyName(e->Key.Key),
                        e->Key.Down ? "Down" : "Up");
        return;
    }
    if (e->Type == AzInputEventType_Text)
    {
        AZ_DEBUG_LOG_IO("[io] %s: Text: %c (U+%08X)\n",
                        prefix,
                        e->Text.Char,
                        e->Text.Char);
        return;
    }
    if (e->Type == AzInputEventType_Focus)
    {
        AZ_DEBUG_LOG_IO("[io] %s: AppFocused %d\n",
                        prefix,
                        e->AppFocused.Focused);
        return;
    }
}
#endif

void Az::UpdateInputEvents(bool trickle_fast_inputs)
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;

    const bool trickle_interleaved_nonchar_keys_and_text =
            (trickle_fast_inputs && g.WantTextInputNextFrame == 1);

    bool mouse_moved = false, mouse_wheeled = false, key_changed = false,
         key_changed_nonchar = false, text_inputted = false;
    int mouse_button_changed = 0x00;
    AzBitArray<AzKey_NamedKey_COUNT> key_changed_mask;

    int event_n = 0;
    for (; event_n < g.InputEventsQueue.Size; event_n++)
    {
        AzInputEvent* e = &g.InputEventsQueue[event_n];
        if (e->Type == AzInputEventType_MousePos)
        {
            if (g.IO.WantSetMousePos)
                continue;

            AzVec2 event_pos(e->MousePos.PosX, e->MousePos.PosY);
            if (trickle_fast_inputs
                && (mouse_button_changed != 0 || mouse_wheeled || key_changed
                    || text_inputted))
                break;
            io.MousePos = event_pos;
            io.MouseSource = e->MousePos.MouseSource;
            mouse_moved = true;
        } else if (e->Type == AzInputEventType_MouseButton)
        {
            const AzMouseButton button = e->MouseButton.Button;
            AZ_ASSERT(button >= 0 && button < AzMouseButton_COUNT);
            if (trickle_fast_inputs
                && ((mouse_button_changed & (1 << button)) || mouse_wheeled))
                break;
            if (trickle_fast_inputs
                && e->MouseButton.MouseSource == AzMouseSource_TouchScreen
                && mouse_moved)
                break;
            io.MouseDown[button] = e->MouseButton.Down;
            io.MouseSource = e->MouseButton.MouseSource;
            mouse_button_changed |= (1 << button);
        } else if (e->Type == AzInputEventType_MouseWheel)
        {
            if (trickle_fast_inputs
                && (mouse_moved || mouse_button_changed != 0))
                break;
            io.MouseWheelH += e->MouseWheel.WheelX;
            io.MouseWheel += e->MouseWheel.WheelY;
            io.MouseSource = e->MouseWheel.MouseSource;
            mouse_wheeled = true;
        } else if (e->Type == AzInputEventType_MouseViewport)
        {
            io.MouseHoveredViewport = e->MouseViewport.HoveredViewportID;
        } else if (e->Type == AzInputEventType_Key)
        {
            if (io.ConfigFlags & AzConfigFlags_NoKeyboard)
                continue;
            AzKey key = e->Key.Key;
            AZ_ASSERT(key != AzKey_None);
            AzKeyData* key_data = GetKeyData(key);
            const int key_data_index = (int)(key_data - g.IO.KeysData);
            if (trickle_fast_inputs && key_data->Down != e->Key.Down
                && (key_changed_mask.TestBit(key_data_index)
                    || mouse_button_changed != 0))
                break;

            const bool key_is_potentially_for_char_input =
                    IsKeyChordPotentiallyCharInput(GetMergedModsFromKeys()
                                                   | key);
            if (trickle_interleaved_nonchar_keys_and_text
                && (text_inputted && !key_is_potentially_for_char_input))
                break;

            if (key_data->Down != e->Key.Down)

            {
                key_changed = true;
                key_changed_mask.SetBit(key_data_index);
                if (trickle_interleaved_nonchar_keys_and_text
                    && !key_is_potentially_for_char_input)
                    key_changed_nonchar = true;
            }

            key_data->Down = e->Key.Down;
            key_data->AnalogValue = e->Key.AnalogValue;
        } else if (e->Type == AzInputEventType_Text)
        {
            if (io.ConfigFlags & AzConfigFlags_NoKeyboard)
                continue;

            if (trickle_fast_inputs
                && (mouse_button_changed != 0 || mouse_moved || mouse_wheeled))
                break;
            if (trickle_interleaved_nonchar_keys_and_text
                && key_changed_nonchar)
                break;
            unsigned int c = e->Text.Char;
            io.InputQueueCharacters.push_back(
                    c <= AZ_UNICODE_CODEPOINT_MAX
                            ? (AzWchar)c
                            : AZ_UNICODE_CODEPOINT_INVALID);
            if (trickle_interleaved_nonchar_keys_and_text)
                text_inputted = true;
        } else if (e->Type == AzInputEventType_Focus)
        {
            const bool focus_lost = !e->AppFocused.Focused;
            io.AppFocusLost = focus_lost;
        } else
        {
            AZ_ASSERT(0 && "Unknown event!");
        }
    }

    for (int n = 0; n < event_n; n++)
        g.InputEventsTrail.push_back(g.InputEventsQueue[n]);

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (event_n != 0 && (g.DebugLogFlags & AzDebugLogFlags_EventIO))
        for (int n = 0; n < g.InputEventsQueue.Size; n++)
            DebugPrintInputEvent(n < event_n ? "Processed" : "Remaining",
                                 &g.InputEventsQueue[n]);
#endif

    if (event_n == g.InputEventsQueue.Size)
        g.InputEventsQueue.resize(0);
    else
        g.InputEventsQueue.erase(g.InputEventsQueue.Data,
                                 g.InputEventsQueue.Data + event_n);

    if (g.IO.AppFocusLost)
    {
        g.IO.ClearInputKeys();
        g.IO.ClearInputMouse();
    }
}

AzID Az::GetKeyOwner(AzKey key)
{
    if (!IsNamedKeyOrMod(key))
        return AzKeyOwner_NoOwner;

    AzContext& g = *GAz;
    AzKeyOwnerData* owner_data = GetKeyOwnerData(&g, key);
    AzID owner_id = owner_data->OwnerCurr;

    if (g.ActiveIdUsingAllKeyboardKeys && owner_id != g.ActiveId
        && owner_id != AzKeyOwner_Any)
        if (key >= AzKey_Keyboard_BEGIN && key < AzKey_Keyboard_END)
            return AzKeyOwner_NoOwner;

    return owner_id;
}

bool Az::TestKeyOwner(AzKey key, AzID owner_id)
{
    if (!IsNamedKeyOrMod(key))
        return true;

    AzContext& g = *GAz;
    if (g.ActiveIdUsingAllKeyboardKeys && owner_id != g.ActiveId
        && owner_id != AzKeyOwner_Any)
        if (key >= AzKey_Keyboard_BEGIN && key < AzKey_Keyboard_END)
            return false;

    AzKeyOwnerData* owner_data = GetKeyOwnerData(&g, key);
    if (owner_id == AzKeyOwner_Any)
        return (owner_data->LockThisFrame == false);

    if (owner_data->OwnerCurr != owner_id)
    {
        if (owner_data->LockThisFrame)
            return false;
        if (owner_data->OwnerCurr != AzKeyOwner_NoOwner)
            return false;
    }

    return true;
}

void Az::SetKeyOwner(AzKey key, AzID owner_id, AzInputFlags flags)
{
    AzContext& g = *GAz;
    AZ_ASSERT(IsNamedKeyOrMod(key)
              && (owner_id != AzKeyOwner_Any
                  || (flags
                      & (AzInputFlags_LockThisFrame
                         | AzInputFlags_LockUntilRelease))));

    AZ_ASSERT((flags & ~AzInputFlags_SupportedBySetKeyOwner) == 0);

    AzKeyOwnerData* owner_data = GetKeyOwnerData(&g, key);
    owner_data->OwnerCurr = owner_data->OwnerNext = owner_id;

    owner_data->LockUntilRelease = (flags & AzInputFlags_LockUntilRelease) != 0;
    owner_data->LockThisFrame = (flags & AzInputFlags_LockThisFrame) != 0
                                || (owner_data->LockUntilRelease);
}

void Az::SetKeyOwnersForKeyChord(AzKeyChord key_chord,
                                 AzID owner_id,
                                 AzInputFlags flags)
{
    if (key_chord & AzMod_Ctrl)
    {
        SetKeyOwner(AzMod_Ctrl, owner_id, flags);
    }
    if (key_chord & AzMod_Shift)
    {
        SetKeyOwner(AzMod_Shift, owner_id, flags);
    }
    if (key_chord & AzMod_Alt)
    {
        SetKeyOwner(AzMod_Alt, owner_id, flags);
    }
    if (key_chord & AzMod_Super)
    {
        SetKeyOwner(AzMod_Super, owner_id, flags);
    }
    if (key_chord & ~AzMod_Mask_)
    {
        SetKeyOwner((AzKey)(key_chord & ~AzMod_Mask_), owner_id, flags);
    }
}

void Az::SetItemKeyOwner(AzKey key, AzInputFlags flags)
{
    AzContext& g = *GAz;
    AzID id = g.LastItemData.ID;
    if (id == 0 || (g.HoveredId != id && g.ActiveId != id))
        return;
    if ((flags & AzInputFlags_CondMask_) == 0)
        flags |= AzInputFlags_CondDefault_;
    if ((g.HoveredId == id && (flags & AzInputFlags_CondHovered))
        || (g.ActiveId == id && (flags & AzInputFlags_CondActive)))
    {
        AZ_ASSERT((flags & ~AzInputFlags_SupportedBySetItemKeyOwner) == 0);
        SetKeyOwner(key, id, flags & ~AzInputFlags_CondMask_);
    }
}

void Az::SetItemKeyOwner(AzKey key)
{
    SetItemKeyOwner(key, AzInputFlags_None);
}

bool Az::IsKeyChordPressed(AzKeyChord key_chord)
{
    return IsKeyChordPressed(key_chord, AzInputFlags_None, AzKeyOwner_Any);
}

bool Az::IsKeyChordPressed(AzKeyChord key_chord,
                           AzInputFlags flags,
                           AzID owner_id)
{
    AzContext& g = *GAz;
    key_chord = FixupKeyChord(key_chord);
    AzKey mods = (AzKey)(key_chord & AzMod_Mask_);
    if (g.IO.KeyMods != mods)
        return false;

    AzKey key = (AzKey)(key_chord & ~AzMod_Mask_);
    if (key == AzKey_None)
        key = ConvertSingleModFlagToKey(mods);
    if (!IsKeyPressed(key, (flags & AzInputFlags_RepeatMask_), owner_id))
        return false;
    return true;
}

void Az::SetNextItemShortcut(AzKeyChord key_chord, AzInputFlags flags)
{
    AzContext& g = *GAz;
    g.NextItemData.HasFlags |= AzNextItemDataFlags_HasShortcut;
    g.NextItemData.Shortcut = key_chord;
    g.NextItemData.ShortcutFlags = flags;
}

void Az::ItemHandleShortcut(AzID id)
{
    AzContext& g = *GAz;
    AzInputFlags flags = g.NextItemData.ShortcutFlags;
    AZ_ASSERT((flags & ~AzInputFlags_SupportedBySetNextItemShortcut) == 0);

    if (g.LastItemData.ItemFlags & AzItemFlags_Disabled)
        return;
    if (flags & AzInputFlags_Tooltip)
    {
        g.LastItemData.StatusFlags |= AzItemStatusFlags_HasShortcut;
        g.LastItemData.Shortcut = g.NextItemData.Shortcut;
    }
    if (!Shortcut(g.NextItemData.Shortcut,
                  flags & AzInputFlags_SupportedByShortcut,
                  id)
        || g.NavActivateId != 0)
        return;

    g.NavActivateId = id;
    g.NavActivateFlags = AzActivateFlags_PreferInput
                         | AzActivateFlags_FromShortcut;

    g.NavActivateDownId = g.NavActivatePressedId = id;
    NavHighlightActivated(id);
}

bool Az::Shortcut(AzKeyChord key_chord, AzInputFlags flags)
{
    return Shortcut(key_chord, flags, AzKeyOwner_Any);
}

bool Az::Shortcut(AzKeyChord key_chord, AzInputFlags flags, AzID owner_id)
{
    AzContext& g = *GAz;

    if ((flags & AzInputFlags_RouteTypeMask_) == 0)
        flags |= AzInputFlags_RouteFocused;

    if (owner_id == AzKeyOwner_Any || owner_id == AzKeyOwner_NoOwner)
        owner_id = GetRoutingIdFromOwnerId(owner_id);

    if (g.CurrentItemFlags & AzItemFlags_Disabled)
        return false;

    if (!SetShortcutRouting(key_chord, flags, owner_id))
        return false;

    if ((flags & AzInputFlags_Repeat) != 0
        && (flags & AzInputFlags_RepeatUntilMask_) == 0)
        flags |= AzInputFlags_RepeatUntilKeyModsChange;

    if (!IsKeyChordPressed(key_chord, flags, owner_id))
        return false;

    SetKeyOwnersForKeyChord(key_chord & AzMod_Mask_, owner_id);

    AZ_ASSERT((flags & ~AzInputFlags_SupportedByShortcut) == 0);
    return true;
}

bool Az::DebugCheckVersionAndDataLayout(const char* version,
                                        size_t sz_io,
                                        size_t sz_style,
                                        size_t sz_vec2,
                                        size_t sz_vec4,
                                        size_t sz_vert,
                                        size_t sz_idx)
{
    bool error = false;
    if (strcmp(version, AZ_VERSION) != 0)
    {
        error = true;
        AZ_ASSERT(strcmp(version, AZ_VERSION) == 0
                  && "Mismatched version string!");
    }
    if (sz_io != sizeof(AzIO))
    {
        error = true;
        AZ_ASSERT(sz_io == sizeof(AzIO) && "Mismatched struct layout!");
    }
    if (sz_style != sizeof(AzStyle))
    {
        error = true;
        AZ_ASSERT(sz_style == sizeof(AzStyle) && "Mismatched struct layout!");
    }
    if (sz_vec2 != sizeof(AzVec2))
    {
        error = true;
        AZ_ASSERT(sz_vec2 == sizeof(AzVec2) && "Mismatched struct layout!");
    }
    if (sz_vec4 != sizeof(AzVec4))
    {
        error = true;
        AZ_ASSERT(sz_vec4 == sizeof(AzVec4) && "Mismatched struct layout!");
    }
    if (sz_vert != sizeof(AzDrawVert))
    {
        error = true;
        AZ_ASSERT(sz_vert == sizeof(AzDrawVert) && "Mismatched struct layout!");
    }
    if (sz_idx != sizeof(AzDrawIdx))
    {
        error = true;
        AZ_ASSERT(sz_idx == sizeof(AzDrawIdx) && "Mismatched struct layout!");
    }
    return !error;
}

void Az::ErrorCheckUsingSetCursorPosToExtendParentBoundaries()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(window->DC.IsSetPos);
    window->DC.IsSetPos = false;
#ifdef AZ_DISABLE_OBSOLETE_FUNCTIONS
    if (window->DC.CursorPos.x <= window->DC.CursorMaxPos.x
        && window->DC.CursorPos.y <= window->DC.CursorMaxPos.y)
        return;
    if (window->SkipItems)
        return;
    AZ_ASSERT(0 && "Code uses SetCursorPos()/SetCursorScreenPos() to extend window/parent boundaries. Please submit an item e.g. Dummy() to validate extent.");
#else
    window->DC.CursorMaxPos = AzMax(window->DC.CursorMaxPos,
                                    window->DC.CursorPos);
#endif
}

static void Az::ErrorCheckNewFrameSanityChecks()
{
    AzContext& g = *GAz;

    if (true)
        AZ_ASSERT(1);
    else
        AZ_ASSERT(0);

#ifdef __EMSCRIPTEN__
    if (g.IO.DeltaTime <= 0.0f && g.FrameCount > 0)
        g.IO.DeltaTime = 0.00001f;
#endif

    AZ_ASSERT(g.Initialized);
    AZ_ASSERT((g.IO.DeltaTime > 0.0f || g.FrameCount == 0)
              && "Need a positive DeltaTime!");
    AZ_ASSERT((g.FrameCount == 0 || g.FrameCountEnded == g.FrameCount)  && "Forgot to call Render() or EndFrame() at the end of the previous frame?");
    AZ_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f
              && "Invalid DisplaySize value!");
    AZ_ASSERT(g.Style.CurveTessellationTol > 0.0f && "Invalid style setting!");
    AZ_ASSERT(g.Style.CircleTessellationMaxError > 0.0f
              && "Invalid style setting!");
    AZ_ASSERT(g.Style.Alpha >= 0.0f && g.Style.Alpha <= 1.0f
              && "Invalid style setting!");

    AZ_ASSERT(g.Style.WindowMinSize.x >= 1.0f && g.Style.WindowMinSize.y >= 1.0f
              && "Invalid style setting!");
    AZ_ASSERT(g.Style.WindowBorderHoverPadding > 0.0f
              && "Invalid style setting!");

    AZ_ASSERT(g.Style.WindowMenuButtonPosition == AzDir_None
              || g.Style.WindowMenuButtonPosition == AzDir_Left
              || g.Style.WindowMenuButtonPosition == AzDir_Right);
    AZ_ASSERT(g.Style.ColorButtonPosition == AzDir_Left
              || g.Style.ColorButtonPosition == AzDir_Right);
    AZ_ASSERT(g.Style.TreeLinesFlags == AzTreeNodeFlags_DrawLinesNone
              || g.Style.TreeLinesFlags == AzTreeNodeFlags_DrawLinesFull
              || g.Style.TreeLinesFlags == AzTreeNodeFlags_DrawLinesToNodes);

    if (g.IO.ConfigErrorRecovery)
        AZ_ASSERT(g.IO.ConfigErrorRecoveryEnableAssert
                  || g.IO.ConfigErrorRecoveryEnableDebugLog
                  || g.IO.ConfigErrorRecoveryEnableTooltip
                  || g.ErrorCallback != NULL);

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    if (g.IO.FontGlobalScale > 1.0f)
        AZ_ASSERT(g.Style.FontScaleMain == 1.0f && "Since 1.92: use style.FontScaleMain instead of g.IO.FontGlobalScale!");

    if (g.IO.ConfigFlags & AzConfigFlags_NavEnableSetMousePos)
    {
        g.IO.ConfigNavMoveSetMousePos = true;
        g.IO.ConfigFlags &= ~AzConfigFlags_NavEnableSetMousePos;
    }
    if (g.IO.ConfigFlags & AzConfigFlags_NavNoCaptureKeyboard)
    {
        g.IO.ConfigNavCaptureKeyboard = false;
        g.IO.ConfigFlags &= ~AzConfigFlags_NavNoCaptureKeyboard;
    }
    if (g.IO.ConfigFlags & AzConfigFlags_DpiEnableScaleFonts)
    {
        g.IO.ConfigDpiScaleFonts = false;
        g.IO.ConfigFlags &= ~AzConfigFlags_DpiEnableScaleFonts;
    }
    if (g.IO.ConfigFlags & AzConfigFlags_DpiEnableScaleViewports)
    {
        g.IO.ConfigDpiScaleViewports = false;
        g.IO.ConfigFlags &= ~AzConfigFlags_DpiEnableScaleViewports;
    }

    if (g.IO.GetClipboardTextFn != NULL
        && (g.PlatformIO.Platform_GetClipboardTextFn == NULL
            || g.PlatformIO.Platform_GetClipboardTextFn
                       == Platform_GetClipboardTextFn_DefaultAzpl))
        g.PlatformIO.Platform_GetClipboardTextFn = [](AzContext* ctx) {
            return ctx->IO.GetClipboardTextFn(ctx->IO.ClipboardUserData);
        };
    if (g.IO.SetClipboardTextFn != NULL
        && (g.PlatformIO.Platform_SetClipboardTextFn == NULL
            || g.PlatformIO.Platform_SetClipboardTextFn
                       == Platform_SetClipboardTextFn_DefaultAzpl))
        g.PlatformIO.Platform_SetClipboardTextFn = [](AzContext* ctx,
                                                      const char* text) {
            return ctx->IO.SetClipboardTextFn(ctx->IO.ClipboardUserData, text);
        };
#endif

    if (g.FrameCount == 1 && (g.IO.ConfigFlags & AzConfigFlags_DockingEnable)
        && (g.ConfigFlagsLastFrame & AzConfigFlags_DockingEnable) == 0)
        AZ_ASSERT(0 && "Please set DockingEnable before the first call to NewFrame()! Otherwise you will lose your .ini settings!");
    if (g.FrameCount == 1 && (g.IO.ConfigFlags & AzConfigFlags_ViewportsEnable)
        && (g.ConfigFlagsLastFrame & AzConfigFlags_ViewportsEnable) == 0)
        AZ_ASSERT(0 && "Please set ViewportsEnable before the first call to NewFrame()! Otherwise you will lose your .ini settings!");

    if (g.IO.ConfigFlags & AzConfigFlags_ViewportsEnable)
    {
        if ((g.IO.BackendFlags & AzBackendFlags_PlatformHasViewports)
            && (g.IO.BackendFlags & AzBackendFlags_RendererHasViewports))
        {
            AZ_ASSERT((g.FrameCount == 0 || g.FrameCount == g.FrameCountPlatformEnded) && "Forgot to call UpdatePlatformWindows() in main loop after EndFrame()? Check examples/ applications for reference.");
            AZ_ASSERT(g.PlatformIO.Platform_CreateWindow != NULL
                      && "Platform init didn't install handlers?");
            AZ_ASSERT(g.PlatformIO.Platform_DestroyWindow != NULL
                      && "Platform init didn't install handlers?");
            AZ_ASSERT(g.PlatformIO.Platform_GetWindowPos != NULL
                      && "Platform init didn't install handlers?");
            AZ_ASSERT(g.PlatformIO.Platform_SetWindowPos != NULL
                      && "Platform init didn't install handlers?");
            AZ_ASSERT(g.PlatformIO.Platform_GetWindowSize != NULL
                      && "Platform init didn't install handlers?");
            AZ_ASSERT(g.PlatformIO.Platform_SetWindowSize != NULL
                      && "Platform init didn't install handlers?");
            AZ_ASSERT(g.PlatformIO.Monitors.Size > 0
                      && "Platform init didn't setup Monitors list?");
            AZ_ASSERT((g.Viewports[0]->PlatformUserData != NULL
                       || g.Viewports[0]->PlatformHandle != NULL)
                      && "Platform init didn't setup main viewport.");
            if (g.IO.ConfigDockingTransparentPayload
                && (g.IO.ConfigFlags & AzConfigFlags_DockingEnable))
                AZ_ASSERT(g.PlatformIO.Platform_SetWindowAlpha != NULL && "Platform_SetWindowAlpha handler is required to use io.ConfigDockingTransparent!");
        } else
        {
            g.IO.ConfigFlags &= ~AzConfigFlags_ViewportsEnable;
        }

        for (AzPlatformMonitor& mon : g.PlatformIO.Monitors)
        {
            AZ_UNUSED(mon);
            AZ_ASSERT(mon.MainSize.x > 0.0f && mon.MainSize.y > 0.0f
                      && "Monitor main bounds not setup properly.");
            AZ_ASSERT(AzRect(mon.MainPos, mon.MainPos + mon.MainSize).Contains(AzRect(mon.WorkPos, mon.WorkPos + mon.WorkSize)) && "Monitor work bounds not setup properly. If you don't have work area information, just copy MainPos/MainSize into them.");
            AZ_ASSERT(mon.DpiScale > 0.0f && mon.DpiScale < 99.0f
                      && "Monitor DpiScale is invalid.");
        }
    }
}

static void Az::ErrorCheckEndFrameSanityChecks()
{
    AzContext& g = *GAz;
    const AzKeyChord key_mods = GetMergedModsFromKeys();
    AZ_UNUSED(g);
    AZ_UNUSED(key_mods);
    AZ_ASSERT((key_mods == 0 || g.IO.KeyMods == key_mods) && "Mismatching io.KeyCtrl/io.KeyShift/io.KeyAlt/io.KeySuper vs io.KeyMods");
    AZ_UNUSED(key_mods);

    AZ_ASSERT(g.CurrentWindowStack.Size == 1);
    AZ_ASSERT(g.CurrentWindowStack[0].Window->IsFallbackWindow);
}

void Az::ErrorRecoveryStoreState(AzErrorRecoveryState* state_out)
{
    AzContext& g = *GAz;
    state_out->SizeOfWindowStack = (short)g.CurrentWindowStack.Size;
    state_out->SizeOfIDStack = (short)g.CurrentWindow->IDStack.Size;
    state_out->SizeOfTreeStack = (short)g.CurrentWindow->DC.TreeDepth;

    state_out->SizeOfColorStack = (short)g.ColorStack.Size;
    state_out->SizeOfStyleVarStack = (short)g.StyleVarStack.Size;
    state_out->SizeOfFontStack = (short)g.FontStack.Size;
    state_out->SizeOfFocusScopeStack = (short)g.FocusScopeStack.Size;
    state_out->SizeOfGroupStack = (short)g.GroupStack.Size;
    state_out->SizeOfItemFlagsStack = (short)g.ItemFlagsStack.Size;
    state_out->SizeOfBeginPopupStack = (short)g.BeginPopupStack.Size;
    state_out->SizeOfDisabledStack = (short)g.DisabledStackSize;
}

void Az::ErrorRecoveryTryToRecoverState(const AzErrorRecoveryState* state_in)
{
    AzContext& g = *GAz;
    while (g.CurrentWindowStack.Size > state_in->SizeOfWindowStack)
    {
        AzWindow* window = g.CurrentWindow;
        if (window->Flags & AzWindowFlags_ChildWindow)
        {
            if (g.CurrentTable != NULL
                && g.CurrentTable->InnerWindow == g.CurrentWindow)
            {
                AZ_ASSERT_USER_ERROR(0, "Missing EndTable()");
                EndTable();
            } else
            {
                AZ_ASSERT_USER_ERROR(0, "Missing EndChild()");
                EndChild();
            }
        } else
        {
            AZ_ASSERT_USER_ERROR(0, "Missing End()");
            End();
        }
    }
    if (g.CurrentWindowStack.Size == state_in->SizeOfWindowStack)
        ErrorRecoveryTryToRecoverWindowState(state_in);
}

void Az::ErrorRecoveryTryToRecoverWindowState(
        const AzErrorRecoveryState* state_in)
{
    AzContext& g = *GAz;

    while (g.CurrentTable != NULL
           && g.CurrentTable->InnerWindow == g.CurrentWindow)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing EndTable()");
        EndTable();
    }

    AzWindow* window = g.CurrentWindow;

    while (g.CurrentTabBar != NULL && g.CurrentTabBar->Window == window)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing EndTabBar()");
        EndTabBar();
    }
    while (g.CurrentMultiSelect != NULL
           && g.CurrentMultiSelect->Storage->Window == window)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing EndMultiSelect()");
        EndMultiSelect();
    }
    if (window->DC.MenuBarAppending)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing EndMenuBar()");
        EndMenuBar();
    }
    while (window->DC.TreeDepth > state_in->SizeOfTreeStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing TreePop()");
        TreePop();
    }
    while (g.GroupStack.Size > state_in->SizeOfGroupStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing EndGroup()");
        EndGroup();
    }
    AZ_ASSERT(g.GroupStack.Size == state_in->SizeOfGroupStack);
    while (window->IDStack.Size > state_in->SizeOfIDStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing PopID()");
        PopID();
    }
    while (g.DisabledStackSize > state_in->SizeOfDisabledStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing EndDisabled()");
        if (g.CurrentItemFlags & AzItemFlags_Disabled)
            EndDisabled();
        else
        {
            EndDisabledOverrideReenable();
            g.CurrentWindowStack.back().DisabledOverrideReenable = false;
        }
    }
    AZ_ASSERT(g.DisabledStackSize == state_in->SizeOfDisabledStack);
    while (g.ColorStack.Size > state_in->SizeOfColorStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing PopStyleColor()");
        PopStyleColor();
    }
    while (g.ItemFlagsStack.Size > state_in->SizeOfItemFlagsStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing PopItemFlag()");
        PopItemFlag();
    }
    while (g.StyleVarStack.Size > state_in->SizeOfStyleVarStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing PopStyleVar()");
        PopStyleVar();
    }
    while (g.FontStack.Size > state_in->SizeOfFontStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing PopFont()");
        PopFont();
    }
    while (g.FocusScopeStack.Size > state_in->SizeOfFocusScopeStack)
    {
        AZ_ASSERT_USER_ERROR(0, "Missing PopFocusScope()");
        PopFocusScope();
    }
}

bool Az::ErrorLog(const char* msg)
{
    AzContext& g = *GAz;

#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzWindow* window = g.CurrentWindow;

    if (g.IO.ConfigErrorRecoveryEnableDebugLog)
    {
        if (g.ErrorFirst)
            AZ_DEBUG_LOG_ERROR(
                    "[imgui-error] (current settings: Assert=%d, Log=%d, "
                    "Tooltip=%d)\n",
                    g.IO.ConfigErrorRecoveryEnableAssert,
                    g.IO.ConfigErrorRecoveryEnableDebugLog,
                    g.IO.ConfigErrorRecoveryEnableTooltip);
        AZ_DEBUG_LOG_ERROR("[imgui-error] In window '%s': %s\n",
                           window ? window->Name : "NULL",
                           msg);
    }
    g.ErrorFirst = false;

    if (g.IO.ConfigErrorRecoveryEnableTooltip)
    {
        if (g.WithinFrameScope && BeginErrorTooltip())
        {
            if (g.ErrorCountCurrentFrame < 20)
            {
                Text("In window '%s': %s", window ? window->Name : "NULL", msg);
                if (window && (!window->IsFallbackWindow || window->WasActive))
                    GetForegroundDrawList(window)->AddRect(
                            window->Pos,
                            window->Pos + window->Size,
                            AZ_COL32(255, 0, 0, 255));
            }
            if (g.ErrorCountCurrentFrame == 20)
                Text("(and more errors)");

            EndErrorTooltip();
        }
        g.ErrorCountCurrentFrame++;
    }
#endif

    if (g.ErrorCallback != NULL)
        g.ErrorCallback(&g, g.ErrorCallbackUserData, msg);

    return g.IO.ConfigErrorRecoveryEnableAssert;
}

void Az::ErrorCheckEndFrameFinalizeErrorTooltip()
{
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *GAz;
    if (g.DebugDrawIdConflicts != 0 && g.IO.KeyCtrl == false)
        g.DebugDrawIdConflictsCount = g.HoveredIdPreviousFrameItemCount;
    if (g.DebugDrawIdConflicts != 0 && g.DebugItemPickerActive == false
        && BeginErrorTooltip())
    {
        Text("Programmer error: %d visible items with conflicting ID!",
             g.DebugDrawIdConflictsCount);
        BulletText(
                "Code should use PushID()/PopID() in loops, or append \"##xx\" "
                "to same-label identifiers!");
        BulletText(
                "Empty label e.g. Button(\"\") == same ID as parent "
                "widget/node. Use Button(\"##xx\") instead!");

        BulletText(
                "Set io.ConfigDebugHighlightIdConflicts=false to disable this "
                "warning in non-programmers builds.");
        Separator();
        if (g.IO.ConfigDebugHighlightIdConflictsShowItemPicker)
        {
            Text("(Hold CTRL to: use ");
            SameLine(0.0f, 0.0f);
            if (SmallButton("Item Picker"))
                DebugStartItemPicker();
            SameLine(0.0f, 0.0f);
            Text(" to break in item call-stack, or ");
        } else
        {
            Text("(Hold CTRL to ");
        }
        SameLine(0.0f, 0.0f);
        if (SmallButton("Open FAQ->About ID Stack System")
            && g.PlatformIO.Platform_OpenInShellFn != NULL)
            g.PlatformIO.Platform_OpenInShellFn(
                    &g,
                    "https://github.com/ocornut/imgui/blob/master/docs/"
                    "FAQ.md#qa-usage");
        SameLine(0.0f, 0.0f);
        Text(")");
        EndErrorTooltip();
    }

    if (g.ErrorCountCurrentFrame > 0 && BeginErrorTooltip())
    {
        Separator();
        Text("(Hold CTRL to:");
        SameLine();
        if (SmallButton("Enable Asserts"))
            g.IO.ConfigErrorRecoveryEnableAssert = true;

        SameLine(0, 0);
        Text(")");
        EndErrorTooltip();
    }
#endif
}

bool Az::BeginErrorTooltip()
{
    AzContext& g = *GAz;
    AzWindow* window = FindWindowByName("##Tooltip_Error");
    const bool use_locked_pos = (g.IO.KeyCtrl && window && window->WasActive);
    PushStyleColor(AzCol_PopupBg,
                   AzLerp(g.Style.Colors[AzCol_PopupBg],
                          AzVec4(1.0f, 0.0f, 0.0f, 1.0f),
                          0.15f));
    if (use_locked_pos)
        SetNextWindowPos(g.ErrorTooltipLockedPos);
    bool is_visible = Begin("##Tooltip_Error",
                            NULL,
                            AzWindowFlags_Tooltip | AzWindowFlags_NoDecoration
                                    | AzWindowFlags_NoMove
                                    | AzWindowFlags_NoResize
                                    | AzWindowFlags_NoSavedSettings
                                    | AzWindowFlags_AlwaysAutoResize);
    PopStyleColor();
    if (is_visible && g.CurrentWindow->BeginCount == 1)
    {
        SeparatorText("MESSAGE FROM DEAR AZ");
        BringWindowToDisplayFront(g.CurrentWindow);
        BringWindowToFocusFront(g.CurrentWindow);
        g.ErrorTooltipLockedPos = GetWindowPos();
    } else if (!is_visible)
    {
        End();
    }
    return is_visible;
}

void Az::EndErrorTooltip()
{
    End();
}

void Az::KeepAliveID(AzID id)
{
    AzContext& g = *GAz;
    if (g.ActiveId == id)
        g.ActiveIdIsAlive = id;
    if (g.DeactivatedItemData.ID == id)
        g.DeactivatedItemData.IsAlive = true;
}

AZ_MSVC_RUNTIME_CHECKS_OFF
bool Az::ItemAdd(const AzRect& bb,
                 AzID id,
                 const AzRect* nav_bb_arg,
                 AzItemFlags extra_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    g.LastItemData.ID = id;
    g.LastItemData.Rect = bb;
    g.LastItemData.NavRect = nav_bb_arg ? *nav_bb_arg : bb;
    g.LastItemData.ItemFlags = g.CurrentItemFlags | g.NextItemData.ItemFlags
                               | extra_flags;
    g.LastItemData.StatusFlags = AzItemStatusFlags_None;

    if (id != 0)
    {
        KeepAliveID(id);

        if (!(g.LastItemData.ItemFlags & AzItemFlags_NoNav))
        {
            window->DC.NavLayersActiveMaskNext |=
                    (1 << window->DC.NavLayerCurrent);
            if (g.NavId == id || g.NavAnyRequest)
                if (g.NavWindow->RootWindowForNav == window->RootWindowForNav)
                    if (window == g.NavWindow
                        || ((window->ChildFlags | g.NavWindow->ChildFlags)
                            & AzChildFlags_NavFlattened))
                        NavProcessItem();
        }

        if (g.NextItemData.HasFlags & AzNextItemDataFlags_HasShortcut)
            ItemHandleShortcut(id);
    }

    g.NextItemData.HasFlags = AzNextItemDataFlags_None;
    g.NextItemData.ItemFlags = AzItemFlags_None;

#ifdef AZ_ENABLE_TEST_ENGINE
    if (id != 0)
        AZ_TEST_ENGINE_ITEM_ADD(id, g.LastItemData.NavRect, &g.LastItemData);
#endif

    const bool is_rect_visible = bb.Overlaps(window->ClipRect);
    if (!is_rect_visible)
        if (id == 0
            || (id != g.ActiveId && id != g.ActiveIdPreviousFrame
                && id != g.NavId && id != g.NavActivateId))
            if (!g.ItemUnclipByLog)
                return false;

#ifndef AZ_DISABLE_DEBUG_TOOLS
    if (id != 0)
    {
        if (id == g.DebugLocateId)
            DebugLocateItemResolveWithLastItem();

        AZ_ASSERT(id != window->ID && "Cannot have an empty ID at the root of a window. If you need an empty label, use ## and read the FAQ about how the ID Stack works!");
    }

#endif

    if (id != 0 && g.DeactivatedItemData.ID == id)
        g.DeactivatedItemData.ElapseFrame = g.FrameCount;

    if (is_rect_visible)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_Visible;
    if (IsMouseHoveringRect(bb.Min, bb.Max))
        g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredRect;
    return true;
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

AZ_MSVC_RUNTIME_CHECKS_OFF
void Az::ItemSize(const AzVec2& size, float text_baseline_y)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    const float offset_to_match_baseline_y =
            (text_baseline_y >= 0)
                    ? AzMax(0.0f,
                            window->DC.CurrLineTextBaseOffset - text_baseline_y)
                    : 0.0f;

    const float line_y1 = window->DC.IsSameLine ? window->DC.CursorPosPrevLine.y
                                                : window->DC.CursorPos.y;
    const float line_height = AzMax(window->DC.CurrLineSize.y,
                                    window->DC.CursorPos.y - line_y1 + size.y
                                            + offset_to_match_baseline_y);

    window->DC.CursorPosPrevLine.x = window->DC.CursorPos.x + size.x;
    window->DC.CursorPosPrevLine.y = line_y1;
    window->DC.CursorPos.x = AZ_TRUNC(window->Pos.x + window->DC.Indent.x
                                      + window->DC.ColumnsOffset.x);
    window->DC.CursorPos.y = AZ_TRUNC(line_y1 + line_height
                                      + g.Style.ItemSpacing.y);
    window->DC.CursorMaxPos.x = AzMax(window->DC.CursorMaxPos.x,
                                      window->DC.CursorPosPrevLine.x);
    window->DC.CursorMaxPos.y = AzMax(window->DC.CursorMaxPos.y,
                                      window->DC.CursorPos.y
                                              - g.Style.ItemSpacing.y);

    window->DC.PrevLineSize.y = line_height;
    window->DC.CurrLineSize.y = 0.0f;
    window->DC.PrevLineTextBaseOffset = AzMax(window->DC.CurrLineTextBaseOffset,
                                              text_baseline_y);
    window->DC.CurrLineTextBaseOffset = 0.0f;
    window->DC.IsSameLine = window->DC.IsSetPos = false;

    if (window->DC.LayoutType == AzLayoutType_Horizontal)
        SameLine();
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

void Az::SameLine(float offset_from_start_x, float spacing_w)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    if (offset_from_start_x != 0.0f)
    {
        if (spacing_w < 0.0f)
            spacing_w = 0.0f;
        window->DC.CursorPos.x = window->Pos.x - window->Scroll.x
                                 + offset_from_start_x + spacing_w
                                 + window->DC.GroupOffset.x
                                 + window->DC.ColumnsOffset.x;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    } else
    {
        if (spacing_w < 0.0f)
            spacing_w = g.Style.ItemSpacing.x;
        window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    window->DC.CurrLineSize = window->DC.PrevLineSize;
    window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
    window->DC.IsSameLine = true;
}

AzVec2 Az::GetCursorScreenPos()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos;
}

void Az::SetCursorScreenPos(const AzVec2& pos)
{
    AzWindow* window = GetCurrentWindow();
    window->DC.CursorPos = pos;

    window->DC.IsSetPos = true;
}

AzVec2 Az::GetCursorPos()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos - window->Pos + window->Scroll;
}

float Az::GetCursorPosX()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float Az::GetCursorPosY()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}

void Az::SetCursorPos(const AzVec2& local_pos)
{
    AzWindow* window = GetCurrentWindow();
    window->DC.CursorPos = window->Pos - window->Scroll + local_pos;

    window->DC.IsSetPos = true;
}

void Az::SetCursorPosX(float x)
{
    AzWindow* window = GetCurrentWindow();
    window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;

    window->DC.IsSetPos = true;
}

void Az::SetCursorPosY(float y)
{
    AzWindow* window = GetCurrentWindow();
    window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;

    window->DC.IsSetPos = true;
}

AzVec2 Az::GetCursorStartPos()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CursorStartPos - window->Pos;
}

void Az::Indent(float indent_w)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    window->DC.Indent.x += (indent_w != 0.0f) ? indent_w
                                              : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.Indent.x
                             + window->DC.ColumnsOffset.x;
}

void Az::Unindent(float indent_w)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    window->DC.Indent.x -= (indent_w != 0.0f) ? indent_w
                                              : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.Indent.x
                             + window->DC.ColumnsOffset.x;
}

void Az::SetNextItemWidth(float item_width)
{
    AzContext& g = *GAz;
    g.NextItemData.HasFlags |= AzNextItemDataFlags_HasWidth;
    g.NextItemData.Width = item_width;
}

void Az::PushItemWidth(float item_width)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
    window->DC.ItemWidth = (item_width == 0.0f ? window->ItemWidthDefault
                                               : item_width);
    g.NextItemData.HasFlags &= ~AzNextItemDataFlags_HasWidth;
}

void Az::PushMultiItemsWidths(int components, float w_full)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(components > 0);
    const AzStyle& style = g.Style;
    window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
    float w_items = w_full - style.ItemInnerSpacing.x * (components - 1);
    float prev_split = w_items;
    for (int i = components - 1; i > 0; i--)
    {
        float next_split = AZ_TRUNC(w_items * i / components);
        window->DC.ItemWidthStack.push_back(
                AzMax(prev_split - next_split, 1.0f));
        prev_split = next_split;
    }
    window->DC.ItemWidth = AzMax(prev_split, 1.0f);
    g.NextItemData.HasFlags &= ~AzNextItemDataFlags_HasWidth;
}

void Az::PopItemWidth()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->DC.ItemWidthStack.Size <= 0)
    {
        AZ_ASSERT_USER_ERROR(0, "Calling PopItemWidth() too many times!");
        return;
    }
    window->DC.ItemWidth = window->DC.ItemWidthStack.back();
    window->DC.ItemWidthStack.pop_back();
}

float Az::CalcItemWidth()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    float w;
    if (g.NextItemData.HasFlags & AzNextItemDataFlags_HasWidth)
        w = g.NextItemData.Width;
    else
        w = window->DC.ItemWidth;
    if (w < 0.0f)
    {
        float region_avail_x = GetContentRegionAvail().x;
        w = AzMax(1.0f, region_avail_x + w);
    }
    w = AZ_TRUNC(w);
    return w;
}

AzVec2 Az::CalcItemSize(AzVec2 size, float default_w, float default_h)
{
    AzVec2 avail;
    if (size.x < 0.0f || size.y < 0.0f)
        avail = GetContentRegionAvail();

    if (size.x == 0.0f)
        size.x = default_w;
    else if (size.x < 0.0f)
        size.x = AzMax(4.0f, avail.x + size.x);

    if (size.y == 0.0f)
        size.y = default_h;
    else if (size.y < 0.0f)
        size.y = AzMax(4.0f, avail.y + size.y);

    return size;
}

float Az::GetTextLineHeight()
{
    AzContext& g = *GAz;
    return g.FontSize;
}

float Az::GetTextLineHeightWithSpacing()
{
    AzContext& g = *GAz;
    return g.FontSize + g.Style.ItemSpacing.y;
}

float Az::GetFrameHeight()
{
    AzContext& g = *GAz;
    return g.FontSize + g.Style.FramePadding.y * 2.0f;
}

float Az::GetFrameHeightWithSpacing()
{
    AzContext& g = *GAz;
    return g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y;
}

AzVec2 Az::GetContentRegionAvail()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzVec2 mx = (window->DC.CurrentColumns || g.CurrentTable)
                        ? window->WorkRect.Max
                        : window->ContentRegionRect.Max;
    return mx - window->DC.CursorPos;
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

AzVec2 Az::GetContentRegionMax()
{
    return GetContentRegionAvail() + GetCursorScreenPos() - GetWindowPos();
}

AzVec2 Az::GetWindowContentRegionMin()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->ContentRegionRect.Min - window->Pos;
}

AzVec2 Az::GetWindowContentRegionMax()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->ContentRegionRect.Max - window->Pos;
}
#endif

void Az::BeginGroup()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    g.GroupStack.resize(g.GroupStack.Size + 1);
    AzGroupData& group_data = g.GroupStack.back();
    group_data.WindowID = window->ID;
    group_data.BackupCursorPos = window->DC.CursorPos;
    group_data.BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
    group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
    group_data.BackupIndent = window->DC.Indent;
    group_data.BackupGroupOffset = window->DC.GroupOffset;
    group_data.BackupCurrLineSize = window->DC.CurrLineSize;
    group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
    group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
    group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
    group_data.BackupIsSameLine = window->DC.IsSameLine;
    group_data.BackupDeactivatedIdIsAlive = g.DeactivatedItemData.IsAlive;
    group_data.EmitItem = true;

    window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x
                               - window->DC.ColumnsOffset.x;
    window->DC.Indent = window->DC.GroupOffset;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.CurrLineSize = AzVec2(0.0f, 0.0f);
    if (g.LogEnabled)
        g.LogLinePosY = -FLT_MAX;
}

void Az::EndGroup()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(g.GroupStack.Size > 0);

    AzGroupData& group_data = g.GroupStack.back();
    AZ_ASSERT(group_data.WindowID == window->ID);

    if (window->DC.IsSetPos)
        ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

    AzRect group_bb(group_data.BackupCursorPos,
                    AzMax(AzMax(window->DC.CursorMaxPos,
                                g.LastItemData.Rect.Max),
                          group_data.BackupCursorPos));
    window->DC.CursorPos = group_data.BackupCursorPos;
    window->DC.CursorPosPrevLine = group_data.BackupCursorPosPrevLine;
    window->DC.CursorMaxPos = AzMax(group_data.BackupCursorMaxPos,
                                    group_bb.Max);
    window->DC.Indent = group_data.BackupIndent;
    window->DC.GroupOffset = group_data.BackupGroupOffset;
    window->DC.CurrLineSize = group_data.BackupCurrLineSize;
    window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
    window->DC.IsSameLine = group_data.BackupIsSameLine;
    if (g.LogEnabled)
        g.LogLinePosY = -FLT_MAX;

    if (!group_data.EmitItem)
    {
        g.GroupStack.pop_back();
        return;
    }

    window->DC.CurrLineTextBaseOffset =
            AzMax(window->DC.PrevLineTextBaseOffset,
                  group_data.BackupCurrLineTextBaseOffset);

    ItemSize(group_bb.GetSize());
    ItemAdd(group_bb, 0, NULL, AzItemFlags_NoTabStop);

    const bool group_contains_curr_active_id =
            (group_data.BackupActiveIdIsAlive != g.ActiveId)
            && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
    const bool group_contains_deactivated_id =
            (group_data.BackupDeactivatedIdIsAlive == false)
            && (g.DeactivatedItemData.IsAlive == true);
    if (group_contains_curr_active_id)
        g.LastItemData.ID = g.ActiveId;
    else if (group_contains_deactivated_id)
        g.LastItemData.ID = g.DeactivatedItemData.ID;
    g.LastItemData.Rect = group_bb;

    const bool group_contains_curr_hovered_id =
            (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
    if (group_contains_curr_hovered_id)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredWindow;

    if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_Edited;

    g.LastItemData.StatusFlags |= AzItemStatusFlags_HasDeactivated;
    if (group_contains_deactivated_id)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_Deactivated;

    g.GroupStack.pop_back();
    if (g.DebugShowGroupRects)
        window->DrawList->AddRect(group_bb.Min,
                                  group_bb.Max,
                                  AZ_COL32(255, 0, 255, 255));
}

static float CalcScrollEdgeSnap(float target,
                                float snap_min,
                                float snap_max,
                                float snap_threshold,
                                float center_ratio)
{
    if (target <= snap_min + snap_threshold)
        return AzLerp(snap_min, target, center_ratio);
    if (target >= snap_max - snap_threshold)
        return AzLerp(target, snap_max, center_ratio);
    return target;
}

static AzVec2 CalcNextScrollFromScrollTargetAndClamp(AzWindow* window)
{
    AzVec2 scroll = window->Scroll;
    AzVec2 decoration_size(window->DecoOuterSizeX1 + window->DecoInnerSizeX1
                                   + window->DecoOuterSizeX2,
                           window->DecoOuterSizeY1 + window->DecoInnerSizeY1
                                   + window->DecoOuterSizeY2);
    for (int axis = 0; axis < 2; axis++)
    {
        if (window->ScrollTarget[axis] < FLT_MAX)
        {
            float center_ratio = window->ScrollTargetCenterRatio[axis];
            float scroll_target = window->ScrollTarget[axis];
            if (window->ScrollTargetEdgeSnapDist[axis] > 0.0f)
            {
                float snap_min = 0.0f;
                float snap_max = window->ScrollMax[axis]
                                 + window->SizeFull[axis]
                                 - decoration_size[axis];
                scroll_target = CalcScrollEdgeSnap(
                        scroll_target,
                        snap_min,
                        snap_max,
                        window->ScrollTargetEdgeSnapDist[axis],
                        center_ratio);
            }
            scroll[axis] = scroll_target
                           - center_ratio
                                     * (window->SizeFull[axis]
                                        - decoration_size[axis]);
        }
        scroll[axis] = AzRound64(AzMax(scroll[axis], 0.0f));
        if (!window->Collapsed && !window->SkipItems)
            scroll[axis] = AzMin(scroll[axis], window->ScrollMax[axis]);
    }
    return scroll;
}

void Az::ScrollToItem(AzScrollFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    ScrollToRectEx(window, g.LastItemData.NavRect, flags);
}

void Az::ScrollToRect(AzWindow* window,
                      const AzRect& item_rect,
                      AzScrollFlags flags)
{
    ScrollToRectEx(window, item_rect, flags);
}

AzVec2 Az::ScrollToRectEx(AzWindow* window,
                          const AzRect& item_rect,
                          AzScrollFlags flags)
{
    AzContext& g = *GAz;
    AzRect scroll_rect(window->InnerRect.Min - AzVec2(1, 1),
                       window->InnerRect.Max + AzVec2(1, 1));
    scroll_rect.Min.x = AzMin(scroll_rect.Min.x + window->DecoInnerSizeX1,
                              scroll_rect.Max.x);
    scroll_rect.Min.y = AzMin(scroll_rect.Min.y + window->DecoInnerSizeY1,
                              scroll_rect.Max.y);

    AZ_ASSERT((flags & AzScrollFlags_MaskX_) == 0
              || AzIsPowerOfTwo(flags & AzScrollFlags_MaskX_));
    AZ_ASSERT((flags & AzScrollFlags_MaskY_) == 0
              || AzIsPowerOfTwo(flags & AzScrollFlags_MaskY_));

    AzScrollFlags in_flags = flags;
    if ((flags & AzScrollFlags_MaskX_) == 0 && window->ScrollbarX)
        flags |= AzScrollFlags_KeepVisibleEdgeX;
    if ((flags & AzScrollFlags_MaskY_) == 0)
        flags |= window->Appearing ? AzScrollFlags_AlwaysCenterY
                                   : AzScrollFlags_KeepVisibleEdgeY;

    const bool fully_visible_x = item_rect.Min.x >= scroll_rect.Min.x
                                 && item_rect.Max.x <= scroll_rect.Max.x;
    const bool fully_visible_y = item_rect.Min.y >= scroll_rect.Min.y
                                 && item_rect.Max.y <= scroll_rect.Max.y;
    const bool can_be_fully_visible_x =
            (item_rect.GetWidth() + g.Style.ItemSpacing.x * 2.0f)
                    <= scroll_rect.GetWidth()
            || (window->AutoFitFramesX > 0)
            || (window->Flags & AzWindowFlags_AlwaysAutoResize) != 0;
    const bool can_be_fully_visible_y =
            (item_rect.GetHeight() + g.Style.ItemSpacing.y * 2.0f)
                    <= scroll_rect.GetHeight()
            || (window->AutoFitFramesY > 0)
            || (window->Flags & AzWindowFlags_AlwaysAutoResize) != 0;

    if ((flags & AzScrollFlags_KeepVisibleEdgeX) && !fully_visible_x)
    {
        if (item_rect.Min.x < scroll_rect.Min.x || !can_be_fully_visible_x)
            SetScrollFromPosX(window,
                              item_rect.Min.x - g.Style.ItemSpacing.x
                                      - window->Pos.x,
                              0.0f);
        else if (item_rect.Max.x >= scroll_rect.Max.x)
            SetScrollFromPosX(window,
                              item_rect.Max.x + g.Style.ItemSpacing.x
                                      - window->Pos.x,
                              1.0f);
    } else if (((flags & AzScrollFlags_KeepVisibleCenterX) && !fully_visible_x)
               || (flags & AzScrollFlags_AlwaysCenterX))
    {
        if (can_be_fully_visible_x)
            SetScrollFromPosX(window,
                              AzTrunc((item_rect.Min.x + item_rect.Max.x)
                                      * 0.5f)
                                      - window->Pos.x,
                              0.5f);
        else
            SetScrollFromPosX(window, item_rect.Min.x - window->Pos.x, 0.0f);
    }

    if ((flags & AzScrollFlags_KeepVisibleEdgeY) && !fully_visible_y)
    {
        if (item_rect.Min.y < scroll_rect.Min.y || !can_be_fully_visible_y)
            SetScrollFromPosY(window,
                              item_rect.Min.y - g.Style.ItemSpacing.y
                                      - window->Pos.y,
                              0.0f);
        else if (item_rect.Max.y >= scroll_rect.Max.y)
            SetScrollFromPosY(window,
                              item_rect.Max.y + g.Style.ItemSpacing.y
                                      - window->Pos.y,
                              1.0f);
    } else if (((flags & AzScrollFlags_KeepVisibleCenterY) && !fully_visible_y)
               || (flags & AzScrollFlags_AlwaysCenterY))
    {
        if (can_be_fully_visible_y)
            SetScrollFromPosY(window,
                              AzTrunc((item_rect.Min.y + item_rect.Max.y)
                                      * 0.5f)
                                      - window->Pos.y,
                              0.5f);
        else
            SetScrollFromPosY(window, item_rect.Min.y - window->Pos.y, 0.0f);
    }

    AzVec2 next_scroll = CalcNextScrollFromScrollTargetAndClamp(window);
    AzVec2 delta_scroll = next_scroll - window->Scroll;

    if (!(flags & AzScrollFlags_NoScrollParent)
        && (window->Flags & AzWindowFlags_ChildWindow))
    {
        if ((in_flags
             & (AzScrollFlags_AlwaysCenterX | AzScrollFlags_KeepVisibleCenterX))
            != 0)
            in_flags = (in_flags & ~AzScrollFlags_MaskX_)
                       | AzScrollFlags_KeepVisibleEdgeX;
        if ((in_flags
             & (AzScrollFlags_AlwaysCenterY | AzScrollFlags_KeepVisibleCenterY))
            != 0)
            in_flags = (in_flags & ~AzScrollFlags_MaskY_)
                       | AzScrollFlags_KeepVisibleEdgeY;
        delta_scroll += ScrollToRectEx(window->ParentWindow,
                                       AzRect(item_rect.Min - delta_scroll,
                                              item_rect.Max - delta_scroll),
                                       in_flags);
    }

    return delta_scroll;
}

float Az::GetScrollX()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->Scroll.x;
}

float Az::GetScrollY()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->Scroll.y;
}

float Az::GetScrollMaxX()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->ScrollMax.x;
}

float Az::GetScrollMaxY()
{
    AzWindow* window = GAz->CurrentWindow;
    return window->ScrollMax.y;
}

void Az::SetScrollX(AzWindow* window, float scroll_x)
{
    window->ScrollTarget.x = scroll_x;
    window->ScrollTargetCenterRatio.x = 0.0f;
    window->ScrollTargetEdgeSnapDist.x = 0.0f;
}

void Az::SetScrollY(AzWindow* window, float scroll_y)
{
    window->ScrollTarget.y = scroll_y;
    window->ScrollTargetCenterRatio.y = 0.0f;
    window->ScrollTargetEdgeSnapDist.y = 0.0f;
}

void Az::SetScrollX(float scroll_x)
{
    AzContext& g = *GAz;
    SetScrollX(g.CurrentWindow, scroll_x);
}

void Az::SetScrollY(float scroll_y)
{
    AzContext& g = *GAz;
    SetScrollY(g.CurrentWindow, scroll_y);
}

void Az::SetScrollFromPosX(AzWindow* window,
                           float local_x,
                           float center_x_ratio)
{
    AZ_ASSERT(center_x_ratio >= 0.0f && center_x_ratio <= 1.0f);
    window->ScrollTarget.x = AZ_TRUNC(local_x - window->DecoOuterSizeX1
                                      - window->DecoInnerSizeX1
                                      + window->Scroll.x);
    window->ScrollTargetCenterRatio.x = center_x_ratio;
    window->ScrollTargetEdgeSnapDist.x = 0.0f;
}

void Az::SetScrollFromPosY(AzWindow* window,
                           float local_y,
                           float center_y_ratio)
{
    AZ_ASSERT(center_y_ratio >= 0.0f && center_y_ratio <= 1.0f);
    window->ScrollTarget.y = AZ_TRUNC(local_y - window->DecoOuterSizeY1
                                      - window->DecoInnerSizeY1
                                      + window->Scroll.y);
    window->ScrollTargetCenterRatio.y = center_y_ratio;
    window->ScrollTargetEdgeSnapDist.y = 0.0f;
}

void Az::SetScrollFromPosX(float local_x, float center_x_ratio)
{
    AzContext& g = *GAz;
    SetScrollFromPosX(g.CurrentWindow, local_x, center_x_ratio);
}

void Az::SetScrollFromPosY(float local_y, float center_y_ratio)
{
    AzContext& g = *GAz;
    SetScrollFromPosY(g.CurrentWindow, local_y, center_y_ratio);
}

void Az::SetScrollHereX(float center_x_ratio)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    float spacing_x = AzMax(window->WindowPadding.x, g.Style.ItemSpacing.x);
    float target_pos_x = AzLerp(g.LastItemData.Rect.Min.x - spacing_x,
                                g.LastItemData.Rect.Max.x + spacing_x,
                                center_x_ratio);
    SetScrollFromPosX(window, target_pos_x - window->Pos.x, center_x_ratio);

    window->ScrollTargetEdgeSnapDist.x = AzMax(0.0f,
                                               window->WindowPadding.x
                                                       - spacing_x);
}

void Az::SetScrollHereY(float center_y_ratio)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    float spacing_y = AzMax(window->WindowPadding.y, g.Style.ItemSpacing.y);
    float target_pos_y = AzLerp(window->DC.CursorPosPrevLine.y - spacing_y,
                                window->DC.CursorPosPrevLine.y
                                        + window->DC.PrevLineSize.y + spacing_y,
                                center_y_ratio);
    SetScrollFromPosY(window, target_pos_y - window->Pos.y, center_y_ratio);

    window->ScrollTargetEdgeSnapDist.y = AzMax(0.0f,
                                               window->WindowPadding.y
                                                       - spacing_y);
}

bool Az::BeginTooltip()
{
    return BeginTooltipEx(AzTooltipFlags_None, AzWindowFlags_None);
}

bool Az::BeginItemTooltip()
{
    if (!IsItemHovered(AzHoveredFlags_ForTooltip))
        return false;
    return BeginTooltipEx(AzTooltipFlags_None, AzWindowFlags_None);
}

bool Az::BeginTooltipEx(AzTooltipFlags tooltip_flags,
                        AzWindowFlags extra_window_flags)
{
    AzContext& g = *GAz;

    const bool is_dragdrop_tooltip = g.DragDropWithinSource
                                     || g.DragDropWithinTarget;
    if (is_dragdrop_tooltip)
    {
        const bool is_touchscreen = (g.IO.MouseSource
                                     == AzMouseSource_TouchScreen);
        if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasPos) == 0)
        {
            AzVec2 tooltip_pos =
                    is_touchscreen ? (g.IO.MousePos
                                      + TOOLTIP_DEFAULT_OFFSET_TOUCH
                                                * g.Style.MouseCursorScale)
                                   : (g.IO.MousePos
                                      + TOOLTIP_DEFAULT_OFFSET_MOUSE
                                                * g.Style.MouseCursorScale);
            AzVec2 tooltip_pivot = is_touchscreen ? TOOLTIP_DEFAULT_PIVOT_TOUCH
                                                  : AzVec2(0.0f, 0.0f);
            SetNextWindowPos(tooltip_pos, AzCond_None, tooltip_pivot);
        }

        SetNextWindowBgAlpha(g.Style.Colors[AzCol_PopupBg].w * 0.60f);

        tooltip_flags |= AzTooltipFlags_OverridePrevious;
    }

    const char* window_name_template = is_dragdrop_tooltip
                                               ? "##Tooltip_DragDrop_%02d"
                                               : "##Tooltip_%02d";
    char window_name[32];
    AzFormatString(window_name,
                   AZ_ARRAYSIZE(window_name),
                   window_name_template,
                   g.TooltipOverrideCount);
    if ((tooltip_flags & AzTooltipFlags_OverridePrevious)
        && g.TooltipPreviousWindow != NULL && g.TooltipPreviousWindow->Active)
    {
        SetWindowHiddenAndSkipItemsForCurrentFrame(g.TooltipPreviousWindow);
        AzFormatString(window_name,
                       AZ_ARRAYSIZE(window_name),
                       window_name_template,
                       ++g.TooltipOverrideCount);
    }

    AzWindowFlags flags = AzWindowFlags_Tooltip | AzWindowFlags_NoInputs
                          | AzWindowFlags_NoTitleBar | AzWindowFlags_NoMove
                          | AzWindowFlags_NoResize
                          | AzWindowFlags_NoSavedSettings
                          | AzWindowFlags_AlwaysAutoResize
                          | AzWindowFlags_NoDocking;
    Begin(window_name, NULL, flags | extra_window_flags);

    return true;
}

void Az::EndTooltip()
{
    AZ_ASSERT(GetCurrentWindowRead()->Flags & AzWindowFlags_Tooltip);

    End();
}

void Az::SetTooltip(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SetTooltipV(fmt, args);
    va_end(args);
}

void Az::SetTooltipV(const char* fmt, va_list args)
{
    if (!BeginTooltipEx(AzTooltipFlags_OverridePrevious, AzWindowFlags_None))
        return;
    TextV(fmt, args);
    EndTooltip();
}

void Az::SetItemTooltip(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (IsItemHovered(AzHoveredFlags_ForTooltip))
        SetTooltipV(fmt, args);
    va_end(args);
}

void Az::SetItemTooltipV(const char* fmt, va_list args)
{
    if (IsItemHovered(AzHoveredFlags_ForTooltip))
        SetTooltipV(fmt, args);
}

bool Az::IsPopupOpen(AzID id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    if (popup_flags & AzPopupFlags_AnyPopupId)
    {
        AZ_ASSERT(id == 0);
        if (popup_flags & AzPopupFlags_AnyPopupLevel)
            return g.OpenPopupStack.Size > 0;
        else
            return g.OpenPopupStack.Size > g.BeginPopupStack.Size;
    } else
    {
        if (popup_flags & AzPopupFlags_AnyPopupLevel)
        {
            for (AzPopupData& popup_data : g.OpenPopupStack)
                if (popup_data.PopupId == id)
                    return true;
            return false;
        } else
        {
            return g.OpenPopupStack.Size > g.BeginPopupStack.Size
                   && g.OpenPopupStack[g.BeginPopupStack.Size].PopupId == id;
        }
    }
}

bool Az::IsPopupOpen(const char* str_id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzID id = (popup_flags & AzPopupFlags_AnyPopupId)
                      ? 0
                      : g.CurrentWindow->GetID(str_id);
    if ((popup_flags & AzPopupFlags_AnyPopupLevel) && id != 0)
        AZ_ASSERT(0 && "Cannot use IsPopupOpen() with a string id and AzPopupFlags_AnyPopupLevel.");
    return IsPopupOpen(id, popup_flags);
}

AzWindow* Az::GetTopMostPopupModal()
{
    AzContext& g = *GAz;
    for (int n = g.OpenPopupStack.Size - 1; n >= 0; n--)
        if (AzWindow* popup = g.OpenPopupStack.Data[n].Window)
            if (popup->Flags & AzWindowFlags_Modal)
                return popup;
    return NULL;
}

AzWindow* Az::GetTopMostAndVisiblePopupModal()
{
    AzContext& g = *GAz;
    for (int n = g.OpenPopupStack.Size - 1; n >= 0; n--)
        if (AzWindow* popup = g.OpenPopupStack.Data[n].Window)
            if ((popup->Flags & AzWindowFlags_Modal)
                && IsWindowActiveAndVisible(popup))
                return popup;
    return NULL;
}

AzWindow* Az::FindBlockingModal(AzWindow* window)
{
    AzContext& g = *GAz;
    if (g.OpenPopupStack.Size <= 0)
        return NULL;

    for (AzPopupData& popup_data : g.OpenPopupStack)
    {
        AzWindow* popup_window = popup_data.Window;
        if (popup_window == NULL
            || !(popup_window->Flags & AzWindowFlags_Modal))
            continue;
        if (!popup_window->Active && !popup_window->WasActive)

            continue;
        if (window == NULL)

            return popup_window;
        if (IsWindowWithinBeginStackOf(window, popup_window))

            continue;
        return popup_window;
    }
    return NULL;
}

void Az::OpenPopup(const char* str_id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzID id = g.CurrentWindow->GetID(str_id);
    AZ_DEBUG_LOG_POPUP("[popup] OpenPopup(\"%s\" -> 0x%08X)\n", str_id, id);
    OpenPopupEx(id, popup_flags);
}

void Az::OpenPopup(AzID id, AzPopupFlags popup_flags)
{
    OpenPopupEx(id, popup_flags);
}

void Az::OpenPopupEx(AzID id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzWindow* parent_window = g.CurrentWindow;
    const int current_stack_size = g.BeginPopupStack.Size;

    if (popup_flags & AzPopupFlags_NoOpenOverExistingPopup)
        if (IsPopupOpen((AzID)0, AzPopupFlags_AnyPopupId))
            return;

    AzPopupData popup_ref;

    popup_ref.PopupId = id;
    popup_ref.Window = NULL;
    popup_ref.RestoreNavWindow = g.NavWindow;

    popup_ref.OpenFrameCount = g.FrameCount;
    popup_ref.OpenParentId = parent_window->IDStack.back();
    popup_ref.OpenPopupPos = NavCalcPreferredRefPos();
    popup_ref.OpenMousePos = IsMousePosValid(&g.IO.MousePos)
                                     ? g.IO.MousePos
                                     : popup_ref.OpenPopupPos;

    AZ_DEBUG_LOG_POPUP("[popup] OpenPopupEx(0x%08X)\n", id);
    if (g.OpenPopupStack.Size < current_stack_size + 1)
    {
        g.OpenPopupStack.push_back(popup_ref);
    } else
    {
        bool keep_existing = false;
        if (g.OpenPopupStack[current_stack_size].PopupId == id)
            if ((g.OpenPopupStack[current_stack_size].OpenFrameCount
                 == g.FrameCount - 1)
                || (popup_flags & AzPopupFlags_NoReopen))
                keep_existing = true;
        if (keep_existing)
        {
            g.OpenPopupStack[current_stack_size].OpenFrameCount =
                    popup_ref.OpenFrameCount;
        } else
        {
            ClosePopupToLevel(current_stack_size, true);
            g.OpenPopupStack.push_back(popup_ref);
        }
    }
}

void Az::ClosePopupsOverWindow(AzWindow* ref_window,
                               bool restore_focus_to_window_under_popup)
{
    AzContext& g = *GAz;
    if (g.OpenPopupStack.Size == 0)
        return;

    int popup_count_to_keep = 0;
    if (ref_window)
    {
        for (; popup_count_to_keep < g.OpenPopupStack.Size;
             popup_count_to_keep++)
        {
            AzPopupData& popup = g.OpenPopupStack[popup_count_to_keep];
            if (!popup.Window)
                continue;
            AZ_ASSERT((popup.Window->Flags & AzWindowFlags_Popup) != 0);

            bool ref_window_is_descendent_of_popup = false;
            for (int n = popup_count_to_keep; n < g.OpenPopupStack.Size; n++)
                if (AzWindow* popup_window = g.OpenPopupStack[n].Window)

                    if (IsWindowWithinBeginStackOf(ref_window, popup_window))
                    {
                        ref_window_is_descendent_of_popup = true;
                        break;
                    }
            if (!ref_window_is_descendent_of_popup)
                break;
        }
    }
    if (popup_count_to_keep < g.OpenPopupStack.Size)

    {
        AZ_DEBUG_LOG_POPUP("[popup] ClosePopupsOverWindow(\"%s\")\n",
                           ref_window ? ref_window->Name : "<NULL>");
        ClosePopupToLevel(popup_count_to_keep,
                          restore_focus_to_window_under_popup);
    }
}

void Az::ClosePopupsExceptModals()
{
    AzContext& g = *GAz;

    int popup_count_to_keep;
    for (popup_count_to_keep = g.OpenPopupStack.Size; popup_count_to_keep > 0;
         popup_count_to_keep--)
    {
        AzWindow* window = g.OpenPopupStack[popup_count_to_keep - 1].Window;
        if (!window || (window->Flags & AzWindowFlags_Modal))
            break;
    }
    if (popup_count_to_keep < g.OpenPopupStack.Size)

        ClosePopupToLevel(popup_count_to_keep, true);
}

void Az::ClosePopupToLevel(int remaining,
                           bool restore_focus_to_window_under_popup)
{
    AzContext& g = *GAz;
    AZ_DEBUG_LOG_POPUP("[popup] ClosePopupToLevel(%d), restore_under=%d\n",
                       remaining,
                       restore_focus_to_window_under_popup);
    AZ_ASSERT(remaining >= 0 && remaining < g.OpenPopupStack.Size);
    if (g.DebugLogFlags & AzDebugLogFlags_EventPopup)
        for (int n = remaining; n < g.OpenPopupStack.Size; n++)
            AZ_DEBUG_LOG_POPUP(
                    "[popup] - Closing PopupID 0x%08X Window \"%s\"\n",
                    g.OpenPopupStack[n].PopupId,
                    g.OpenPopupStack[n].Window
                            ? g.OpenPopupStack[n].Window->Name
                            : NULL);

    AzPopupData prev_popup = g.OpenPopupStack[remaining];
    g.OpenPopupStack.resize(remaining);

    if (restore_focus_to_window_under_popup && prev_popup.Window)
    {
        AzWindow* popup_window = prev_popup.Window;
        AzWindow* focus_window = (popup_window->Flags & AzWindowFlags_ChildMenu)
                                         ? popup_window->ParentWindow
                                         : prev_popup.RestoreNavWindow;
        if (focus_window && !focus_window->WasActive)
            FocusTopMostWindowUnderOne(popup_window,
                                       NULL,
                                       NULL,
                                       AzFocusRequestFlags_RestoreFocusedChild);
        else
            FocusWindow(focus_window,
                        (g.NavLayer == AzNavLayer_Main)
                                ? AzFocusRequestFlags_RestoreFocusedChild
                                : AzFocusRequestFlags_None);
    }
}

void Az::CloseCurrentPopup()
{
    AzContext& g = *GAz;
    int popup_idx = g.BeginPopupStack.Size - 1;
    if (popup_idx < 0 || popup_idx >= g.OpenPopupStack.Size
        || g.BeginPopupStack[popup_idx].PopupId
                   != g.OpenPopupStack[popup_idx].PopupId)
        return;

    while (popup_idx > 0)
    {
        AzWindow* popup_window = g.OpenPopupStack[popup_idx].Window;
        AzWindow* parent_popup_window = g.OpenPopupStack[popup_idx - 1].Window;
        bool close_parent = false;
        if (popup_window && (popup_window->Flags & AzWindowFlags_ChildMenu))
            if (parent_popup_window
                && !(parent_popup_window->Flags & AzWindowFlags_MenuBar))
                close_parent = true;
        if (!close_parent)
            break;
        popup_idx--;
    }
    AZ_DEBUG_LOG_POPUP("[popup] CloseCurrentPopup %d -> %d\n",
                       g.BeginPopupStack.Size - 1,
                       popup_idx);
    ClosePopupToLevel(popup_idx, true);

    if (AzWindow* window = g.NavWindow)
        window->DC.NavHideHighlightOneFrame = true;
}

bool Az::BeginPopupEx(AzID id, AzWindowFlags extra_window_flags)
{
    AzContext& g = *GAz;
    if (!IsPopupOpen(id, AzPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();

        return false;
    }

    char name[20];
    AZ_ASSERT((extra_window_flags & AzWindowFlags_ChildMenu) == 0);
    AzFormatString(name, AZ_ARRAYSIZE(name), "##Popup_%08x", id);

    bool is_open = Begin(name,
                         NULL,
                         extra_window_flags | AzWindowFlags_Popup
                                 | AzWindowFlags_NoDocking);
    if (!is_open)

        EndPopup();

    return is_open;
}

bool Az::BeginPopupMenuEx(AzID id,
                          const char* label,
                          AzWindowFlags extra_window_flags)
{
    AzContext& g = *GAz;
    if (!IsPopupOpen(id, AzPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();

        return false;
    }

    char name[128];
    AZ_ASSERT(extra_window_flags & AzWindowFlags_ChildMenu);
    AzFormatString(name,
                   AZ_ARRAYSIZE(name),
                   "%s###Menu_%02d",
                   label,
                   g.BeginMenuDepth);
    bool is_open = Begin(name, NULL, extra_window_flags | AzWindowFlags_Popup);
    if (!is_open)

        EndPopup();

    return is_open;
}

bool Az::BeginPopup(const char* str_id, AzWindowFlags flags)
{
    AzContext& g = *GAz;
    if (g.OpenPopupStack.Size <= g.BeginPopupStack.Size)
    {
        g.NextWindowData.ClearFlags();

        return false;
    }
    flags |= AzWindowFlags_AlwaysAutoResize | AzWindowFlags_NoTitleBar
             | AzWindowFlags_NoSavedSettings;
    AzID id = g.CurrentWindow->GetID(str_id);
    return BeginPopupEx(id, flags);
}

bool Az::BeginPopupModal(const char* name, bool* p_open, AzWindowFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    const AzID id = window->GetID(name);
    if (!IsPopupOpen(id, AzPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();

        if (p_open && *p_open)
            *p_open = false;
        return false;
    }

    if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasPos) == 0)
    {
        const AzViewport* viewport = window->WasActive ? window->Viewport
                                                       : GetMainViewport();

        SetNextWindowPos(viewport->GetCenter(),
                         AzCond_FirstUseEver,
                         AzVec2(0.5f, 0.5f));
    }

    flags |= AzWindowFlags_Popup | AzWindowFlags_Modal
             | AzWindowFlags_NoCollapse | AzWindowFlags_NoDocking;
    const bool is_open = Begin(name, p_open, flags);
    if (!is_open || (p_open && !*p_open))

    {
        EndPopup();
        if (is_open)
            ClosePopupToLevel(g.BeginPopupStack.Size, true);
        return false;
    }
    return is_open;
}

void Az::EndPopup()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if ((window->Flags & AzWindowFlags_Popup) == 0
        || g.BeginPopupStack.Size == 0)
    {
        AZ_ASSERT_USER_ERROR(
                0,
                "Calling EndPopup() too many times or in wrong window!");
        return;
    }

    if (g.NavWindow == window)
        NavMoveRequestTryWrapping(window, AzNavMoveFlags_LoopY);

    const AzID backup_within_end_child_id = g.WithinEndChildID;
    if (window->Flags & AzWindowFlags_ChildWindow)
        g.WithinEndChildID = window->ID;
    End();
    g.WithinEndChildID = backup_within_end_child_id;
}

void Az::OpenPopupOnItemClick(const char* str_id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    int mouse_button = (popup_flags & AzPopupFlags_MouseButtonMask_);
    if (IsMouseReleased(mouse_button)
        && IsItemHovered(AzHoveredFlags_AllowWhenBlockedByPopup))
    {
        AzID id = str_id ? window->GetID(str_id) : g.LastItemData.ID;

        AZ_ASSERT(id != 0);

        OpenPopupEx(id, popup_flags);
    }
}

bool Az::BeginPopupContextItem(const char* str_id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;
    AzID id = str_id ? window->GetID(str_id) : g.LastItemData.ID;

    AZ_ASSERT(id != 0);

    int mouse_button = (popup_flags & AzPopupFlags_MouseButtonMask_);
    if (IsMouseReleased(mouse_button)
        && IsItemHovered(AzHoveredFlags_AllowWhenBlockedByPopup))
        OpenPopupEx(id, popup_flags);
    return BeginPopupEx(id,
                        AzWindowFlags_AlwaysAutoResize
                                | AzWindowFlags_NoTitleBar
                                | AzWindowFlags_NoSavedSettings);
}

bool Az::BeginPopupContextWindow(const char* str_id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (!str_id)
        str_id = "window_context";
    AzID id = window->GetID(str_id);
    int mouse_button = (popup_flags & AzPopupFlags_MouseButtonMask_);
    if (IsMouseReleased(mouse_button)
        && IsWindowHovered(AzHoveredFlags_AllowWhenBlockedByPopup))
        if (!(popup_flags & AzPopupFlags_NoOpenOverItems)
            || !IsAnyItemHovered())
            OpenPopupEx(id, popup_flags);
    return BeginPopupEx(id,
                        AzWindowFlags_AlwaysAutoResize
                                | AzWindowFlags_NoTitleBar
                                | AzWindowFlags_NoSavedSettings);
}

bool Az::BeginPopupContextVoid(const char* str_id, AzPopupFlags popup_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (!str_id)
        str_id = "void_context";
    AzID id = window->GetID(str_id);
    int mouse_button = (popup_flags & AzPopupFlags_MouseButtonMask_);
    if (IsMouseReleased(mouse_button)
        && !IsWindowHovered(AzHoveredFlags_AnyWindow))
        if (GetTopMostPopupModal() == NULL)
            OpenPopupEx(id, popup_flags);
    return BeginPopupEx(id,
                        AzWindowFlags_AlwaysAutoResize
                                | AzWindowFlags_NoTitleBar
                                | AzWindowFlags_NoSavedSettings);
}

AzVec2 Az::FindBestWindowPosForPopupEx(const AzVec2& ref_pos,
                                       const AzVec2& size,
                                       AzDir* last_dir,
                                       const AzRect& r_outer,
                                       const AzRect& r_avoid,
                                       AzPopupPositionPolicy policy)
{
    AzVec2 base_pos_clamped = AzClamp(ref_pos, r_outer.Min, r_outer.Max - size);

    if (policy == AzPopupPositionPolicy_ComboBox)
    {
        const AzDir dir_prefered_order[AzDir_COUNT] = {AzDir_Down,
                                                       AzDir_Right,
                                                       AzDir_Left,
                                                       AzDir_Up};
        for (int n = (*last_dir != AzDir_None) ? -1 : 0; n < AzDir_COUNT; n++)
        {
            const AzDir dir = (n == -1) ? *last_dir : dir_prefered_order[n];
            if (n != -1 && dir == *last_dir)
                continue;
            AzVec2 pos;
            if (dir == AzDir_Down)
                pos = AzVec2(r_avoid.Min.x, r_avoid.Max.y);
            if (dir == AzDir_Right)
                pos = AzVec2(r_avoid.Min.x, r_avoid.Min.y - size.y);
            if (dir == AzDir_Left)
                pos = AzVec2(r_avoid.Max.x - size.x, r_avoid.Max.y);
            if (dir == AzDir_Up)
                pos = AzVec2(r_avoid.Max.x - size.x, r_avoid.Min.y - size.y);
            if (!r_outer.Contains(AzRect(pos, pos + size)))
                continue;
            *last_dir = dir;
            return pos;
        }
    }

    if (policy == AzPopupPositionPolicy_Tooltip
        || policy == AzPopupPositionPolicy_Default)
    {
        const AzDir dir_prefered_order[AzDir_COUNT] = {AzDir_Right,
                                                       AzDir_Down,
                                                       AzDir_Up,
                                                       AzDir_Left};
        for (int n = (*last_dir != AzDir_None) ? -1 : 0; n < AzDir_COUNT; n++)
        {
            const AzDir dir = (n == -1) ? *last_dir : dir_prefered_order[n];
            if (n != -1 && dir == *last_dir)
                continue;

            const float avail_w = (dir == AzDir_Left ? r_avoid.Min.x
                                                     : r_outer.Max.x)
                                  - (dir == AzDir_Right ? r_avoid.Max.x
                                                        : r_outer.Min.x);
            const float avail_h = (dir == AzDir_Up ? r_avoid.Min.y
                                                   : r_outer.Max.y)
                                  - (dir == AzDir_Down ? r_avoid.Max.y
                                                       : r_outer.Min.y);

            if (avail_w < size.x && (dir == AzDir_Left || dir == AzDir_Right))
                continue;
            if (avail_h < size.y && (dir == AzDir_Up || dir == AzDir_Down))
                continue;

            AzVec2 pos;
            pos.x = (dir == AzDir_Left)    ? r_avoid.Min.x - size.x
                    : (dir == AzDir_Right) ? r_avoid.Max.x
                                           : base_pos_clamped.x;
            pos.y = (dir == AzDir_Up)     ? r_avoid.Min.y - size.y
                    : (dir == AzDir_Down) ? r_avoid.Max.y
                                          : base_pos_clamped.y;

            pos.x = AzMax(pos.x, r_outer.Min.x);
            pos.y = AzMax(pos.y, r_outer.Min.y);

            *last_dir = dir;
            return pos;
        }
    }

    *last_dir = AzDir_None;

    if (policy == AzPopupPositionPolicy_Tooltip)
        return ref_pos + AzVec2(2, 2);

    AzVec2 pos = ref_pos;
    pos.x = AzMax(AzMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
    pos.y = AzMax(AzMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
    return pos;
}

AzRect Az::GetPopupAllowedExtentRect(AzWindow* window)
{
    AzContext& g = *GAz;
    AzRect r_screen;
    if (window->ViewportAllowPlatformMonitorExtend >= 0)
    {
        const AzPlatformMonitor& monitor =
                g.PlatformIO
                        .Monitors[window->ViewportAllowPlatformMonitorExtend];
        r_screen.Min = monitor.WorkPos;
        r_screen.Max = monitor.WorkPos + monitor.WorkSize;
    } else
    {
        r_screen = window->Viewport->GetMainRect();
    }
    AzVec2 padding = g.Style.DisplaySafeAreaPadding;
    r_screen.Expand(
            AzVec2((r_screen.GetWidth() > padding.x * 2) ? -padding.x : 0.0f,
                   (r_screen.GetHeight() > padding.y * 2) ? -padding.y : 0.0f));
    return r_screen;
}

AzVec2 Az::FindBestWindowPosForPopup(AzWindow* window)
{
    AzContext& g = *GAz;

    AzRect r_outer = GetPopupAllowedExtentRect(window);
    if (window->Flags & AzWindowFlags_ChildMenu)
    {
        AzWindow* parent_window = window->ParentWindow;
        float horizontal_overlap = g.Style.ItemInnerSpacing.x;

        AzRect r_avoid;
        if (parent_window->DC.MenuBarAppending)
            r_avoid = AzRect(-FLT_MAX,
                             parent_window->ClipRect.Min.y,
                             FLT_MAX,
                             parent_window->ClipRect.Max.y);

        else
            r_avoid = AzRect(parent_window->Pos.x + horizontal_overlap,
                             -FLT_MAX,
                             parent_window->Pos.x + parent_window->Size.x
                                     - horizontal_overlap
                                     - parent_window->ScrollbarSizes.x,
                             FLT_MAX);
        return FindBestWindowPosForPopupEx(window->Pos,
                                           window->Size,
                                           &window->AutoPosLastDirection,
                                           r_outer,
                                           r_avoid,
                                           AzPopupPositionPolicy_Default);
    }
    if (window->Flags & AzWindowFlags_Popup)
    {
        return FindBestWindowPosForPopupEx(window->Pos,
                                           window->Size,
                                           &window->AutoPosLastDirection,
                                           r_outer,
                                           AzRect(window->Pos, window->Pos),
                                           AzPopupPositionPolicy_Default);
    }
    if (window->Flags & AzWindowFlags_Tooltip)
    {
        AZ_ASSERT(g.CurrentWindow == window);
        const float scale = g.Style.MouseCursorScale;
        const AzVec2 ref_pos = NavCalcPreferredRefPos();

        if (g.IO.MouseSource == AzMouseSource_TouchScreen
            && NavCalcPreferredRefPosSource() == AzInputSource_Mouse)
        {
            AzVec2 tooltip_pos = ref_pos + TOOLTIP_DEFAULT_OFFSET_TOUCH * scale
                                 - (TOOLTIP_DEFAULT_PIVOT_TOUCH * window->Size);
            if (r_outer.Contains(
                        AzRect(tooltip_pos, tooltip_pos + window->Size)))
                return tooltip_pos;
        }

        AzVec2 tooltip_pos = ref_pos + TOOLTIP_DEFAULT_OFFSET_MOUSE * scale;
        AzRect r_avoid;
        if (g.NavCursorVisible && g.NavHighlightItemUnderNav
            && !g.IO.ConfigNavMoveSetMousePos)
            r_avoid = AzRect(ref_pos.x - 16,
                             ref_pos.y - 8,
                             ref_pos.x + 16,
                             ref_pos.y + 8);
        else
            r_avoid = AzRect(ref_pos.x - 16,
                             ref_pos.y - 8,
                             ref_pos.x + 24 * scale,
                             ref_pos.y + 24 * scale);

        return FindBestWindowPosForPopupEx(tooltip_pos,
                                           window->Size,
                                           &window->AutoPosLastDirection,
                                           r_outer,
                                           r_avoid,
                                           AzPopupPositionPolicy_Tooltip);
    }
    AZ_ASSERT(0);
    return window->Pos;
}

void Az::SetWindowFocus()
{
    FocusWindow(GAz->CurrentWindow);
}

void Az::SetWindowFocus(const char* name)
{
    if (name)
    {
        if (AzWindow* window = FindWindowByName(name))
            FocusWindow(window);
    } else
    {
        FocusWindow(NULL);
    }
}

void Az::SetNextWindowFocus()
{
    AzContext& g = *GAz;
    g.NextWindowData.HasFlags |= AzNextWindowDataFlags_HasFocus;
}

bool Az::IsWindowFocused(AzFocusedFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* ref_window = g.NavWindow;
    AzWindow* cur_window = g.CurrentWindow;

    if (ref_window == NULL)
        return false;
    if (flags & AzFocusedFlags_AnyWindow)
        return true;

    AZ_ASSERT(cur_window);
    const bool popup_hierarchy = (flags & AzFocusedFlags_NoPopupHierarchy) == 0;
    const bool dock_hierarchy = (flags & AzFocusedFlags_DockHierarchy) != 0;
    if (flags & AzHoveredFlags_RootWindow)
        cur_window = GetCombinedRootWindow(cur_window,
                                           popup_hierarchy,
                                           dock_hierarchy);

    if (flags & AzHoveredFlags_ChildWindows)
        return IsWindowChildOf(ref_window,
                               cur_window,
                               popup_hierarchy,
                               dock_hierarchy);
    else
        return (ref_window == cur_window);
}

static int Az::FindWindowFocusIndex(AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_UNUSED(g);
    int order = window->FocusOrder;
    AZ_ASSERT(window->RootWindow == window);

    AZ_ASSERT(g.WindowsFocusOrder[order] == window);
    return order;
}

static void Az::UpdateWindowInFocusOrderList(AzWindow* window,
                                             bool just_created,
                                             AzWindowFlags new_flags)
{
    AzContext& g = *GAz;

    const bool new_is_explicit_child =
            (new_flags & AzWindowFlags_ChildWindow) != 0
            && ((new_flags & AzWindowFlags_Popup) == 0
                || (new_flags & AzWindowFlags_ChildMenu) != 0);
    const bool child_flag_changed = new_is_explicit_child
                                    != window->IsExplicitChild;
    if ((just_created || child_flag_changed) && !new_is_explicit_child)
    {
        AZ_ASSERT(!g.WindowsFocusOrder.contains(window));
        g.WindowsFocusOrder.push_back(window);
        window->FocusOrder = (short)(g.WindowsFocusOrder.Size - 1);
    } else if (!just_created && child_flag_changed && new_is_explicit_child)
    {
        AZ_ASSERT(g.WindowsFocusOrder[window->FocusOrder] == window);
        for (int n = window->FocusOrder + 1; n < g.WindowsFocusOrder.Size; n++)
            g.WindowsFocusOrder[n]->FocusOrder--;
        g.WindowsFocusOrder.erase(g.WindowsFocusOrder.Data
                                  + window->FocusOrder);
        window->FocusOrder = -1;
    }
    window->IsExplicitChild = new_is_explicit_child;
}

void Az::BringWindowToFocusFront(AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_ASSERT(window == window->RootWindow);

    const int cur_order = window->FocusOrder;
    AZ_ASSERT(g.WindowsFocusOrder[cur_order] == window);
    if (g.WindowsFocusOrder.back() == window)
        return;

    const int new_order = g.WindowsFocusOrder.Size - 1;
    for (int n = cur_order; n < new_order; n++)
    {
        g.WindowsFocusOrder[n] = g.WindowsFocusOrder[n + 1];
        g.WindowsFocusOrder[n]->FocusOrder--;
        AZ_ASSERT(g.WindowsFocusOrder[n]->FocusOrder == n);
    }
    g.WindowsFocusOrder[new_order] = window;
    window->FocusOrder = (short)new_order;
}

void Az::BringWindowToDisplayFront(AzWindow* window)
{
    AzContext& g = *GAz;
    AzWindow* current_front_window = g.Windows.back();
    if (current_front_window == window
        || current_front_window->RootWindowDockTree == window)
        return;
    for (int i = g.Windows.Size - 2; i >= 0; i--)
        if (g.Windows[i] == window)
        {
            memmove(&g.Windows[i],
                    &g.Windows[i + 1],
                    (size_t)(g.Windows.Size - i - 1) * sizeof(AzWindow*));
            g.Windows[g.Windows.Size - 1] = window;
            break;
        }
}

void Az::BringWindowToDisplayBack(AzWindow* window)
{
    AzContext& g = *GAz;
    if (g.Windows[0] == window)
        return;
    for (int i = 0; i < g.Windows.Size; i++)
        if (g.Windows[i] == window)
        {
            memmove(&g.Windows[1],
                    &g.Windows[0],
                    (size_t)i * sizeof(AzWindow*));
            g.Windows[0] = window;
            break;
        }
}

void Az::BringWindowToDisplayBehind(AzWindow* window, AzWindow* behind_window)
{
    AZ_ASSERT(window != NULL && behind_window != NULL);
    AzContext& g = *GAz;
    window = window->RootWindow;
    behind_window = behind_window->RootWindow;
    int pos_wnd = FindWindowDisplayIndex(window);
    int pos_beh = FindWindowDisplayIndex(behind_window);
    if (pos_wnd < pos_beh)
    {
        size_t copy_bytes = (pos_beh - pos_wnd - 1) * sizeof(AzWindow*);
        memmove(&g.Windows.Data[pos_wnd],
                &g.Windows.Data[pos_wnd + 1],
                copy_bytes);
        g.Windows[pos_beh - 1] = window;
    } else
    {
        size_t copy_bytes = (pos_wnd - pos_beh) * sizeof(AzWindow*);
        memmove(&g.Windows.Data[pos_beh + 1],
                &g.Windows.Data[pos_beh],
                copy_bytes);
        g.Windows[pos_beh] = window;
    }
}

int Az::FindWindowDisplayIndex(AzWindow* window)
{
    AzContext& g = *GAz;
    return g.Windows.index_from_ptr(g.Windows.find(window));
}

void Az::FocusWindow(AzWindow* window, AzFocusRequestFlags flags)
{
    AzContext& g = *GAz;

    if ((flags & AzFocusRequestFlags_UnlessBelowModal)
        && (g.NavWindow != window))
        if (AzWindow* blocking_modal = FindBlockingModal(window))
        {
            AZ_DEBUG_LOG_FOCUS(
                    "[focus] FocusWindow(\"%s\", UnlessBelowModal): prevented "
                    "by \"%s\".\n",
                    window ? window->Name : "<NULL>",
                    blocking_modal->Name);
            if (window && window == window->RootWindow
                && (window->Flags & AzWindowFlags_NoBringToFrontOnFocus) == 0)
                BringWindowToDisplayBehind(window, blocking_modal);

            ClosePopupsOverWindow(GetTopMostPopupModal(), false);

            return;
        }

    if ((flags & AzFocusRequestFlags_RestoreFocusedChild) && window != NULL)
        window = NavRestoreLastChildNavWindow(window);

    if (g.NavWindow != window)
    {
        SetNavWindow(window);
        if (window && g.NavHighlightItemUnderNav)
            g.NavMousePosDirty = true;
        g.NavId = window ? window->NavLastIds[0] : 0;
        g.NavLayer = AzNavLayer_Main;
        SetNavFocusScope(window ? window->NavRootFocusScopeId : 0);
        g.NavIdIsAlive = false;
        g.NavLastValidSelectionUserData = AzSelectionUserData_Invalid;

        ClosePopupsOverWindow(window, false);
    }

    AZ_ASSERT(window == NULL || window->RootWindowDockTree != NULL);
    AzWindow* focus_front_window = window ? window->RootWindow : NULL;
    AzWindow* display_front_window = window ? window->RootWindowDockTree : NULL;
    AzDockNode* dock_node = window ? window->DockNode : NULL;
    bool active_id_window_is_dock_node_host = (g.ActiveIdWindow && dock_node
                                               && dock_node->HostWindow
                                                          == g.ActiveIdWindow);

    if (g.ActiveId != 0 && g.ActiveIdWindow
        && g.ActiveIdWindow->RootWindow != focus_front_window)
        if (!g.ActiveIdNoClearOnFocusLoss
            && !active_id_window_is_dock_node_host)
            ClearActiveID();

    if (!window)
        return;
    window->LastFrameJustFocused = g.FrameCount;

    BringWindowToFocusFront(focus_front_window);
    if (((window->Flags | focus_front_window->Flags
          | display_front_window->Flags)
         & AzWindowFlags_NoBringToFrontOnFocus)
        == 0)
        BringWindowToDisplayFront(display_front_window);
}

void Az::FocusTopMostWindowUnderOne(AzWindow* under_this_window,
                                    AzWindow* ignore_window,
                                    AzViewport* filter_viewport,
                                    AzFocusRequestFlags flags)
{
    AzContext& g = *GAz;
    int start_idx = g.WindowsFocusOrder.Size - 1;
    if (under_this_window != NULL)
    {
        int offset = -1;
        while (under_this_window->Flags & AzWindowFlags_ChildWindow)
        {
            under_this_window = under_this_window->ParentWindow;
            offset = 0;
        }
        start_idx = FindWindowFocusIndex(under_this_window) + offset;
    }
    for (int i = start_idx; i >= 0; i--)
    {
        AzWindow* window = g.WindowsFocusOrder[i];
        if (window == ignore_window || !window->WasActive)
            continue;
        if (filter_viewport != NULL && window->Viewport != filter_viewport)
            continue;
        if ((window->Flags
             & (AzWindowFlags_NoMouseInputs | AzWindowFlags_NoNavInputs))
            != (AzWindowFlags_NoMouseInputs | AzWindowFlags_NoNavInputs))
        {
            FocusWindow(window, flags);
            return;
        }
    }
    FocusWindow(NULL, flags);
}

void Az::SetNavCursorVisible(bool visible)
{
    AzContext& g = *GAz;
    if (g.IO.ConfigNavCursorVisibleAlways)
        visible = true;
    g.NavCursorVisible = visible;
}

void Az::SetNavCursorVisibleAfterMove()
{
    AzContext& g = *GAz;
    if (g.IO.ConfigNavCursorVisibleAuto)
        g.NavCursorVisible = true;
    g.NavHighlightItemUnderNav = g.NavMousePosDirty = true;
}

void Az::SetNavWindow(AzWindow* window)
{
    AzContext& g = *GAz;
    if (g.NavWindow != window)
    {
        AZ_DEBUG_LOG_FOCUS("[focus] SetNavWindow(\"%s\")\n",
                           window ? window->Name : "<NULL>");
        g.NavWindow = window;
        g.NavLastValidSelectionUserData = AzSelectionUserData_Invalid;
    }
    g.NavInitRequest = g.NavMoveSubmitted = g.NavMoveScoringItems = false;
    NavUpdateAnyRequestFlag();
}

void Az::NavHighlightActivated(AzID id)
{
    AzContext& g = *GAz;
    g.NavHighlightActivatedId = id;
    g.NavHighlightActivatedTimer = NAV_ACTIVATE_HIGHLIGHT_TAZER;
}

void Az::NavClearPreferredPosForAxis(AzAxis axis)
{
    AzContext& g = *GAz;
    g.NavWindow->RootWindowForNav->NavPreferredScoringPosRel[g.NavLayer][axis] =
            FLT_MAX;
}

void Az::SetNavID(AzID id,
                  AzNavLayer nav_layer,
                  AzID focus_scope_id,
                  const AzRect& rect_rel)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.NavWindow != NULL);
    AZ_ASSERT(nav_layer == AzNavLayer_Main || nav_layer == AzNavLayer_Menu);
    g.NavId = id;
    g.NavLayer = nav_layer;
    SetNavFocusScope(focus_scope_id);
    g.NavWindow->NavLastIds[nav_layer] = id;
    g.NavWindow->NavRectRel[nav_layer] = rect_rel;

    NavClearPreferredPosForAxis(AzAxis_X);
    NavClearPreferredPosForAxis(AzAxis_Y);
}

void Az::SetFocusID(AzID id, AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_ASSERT(id != 0);

    if (g.NavWindow != window)
        SetNavWindow(window);

    const AzNavLayer nav_layer = window->DC.NavLayerCurrent;
    g.NavId = id;
    g.NavLayer = nav_layer;
    SetNavFocusScope(g.CurrentFocusScopeId);
    window->NavLastIds[nav_layer] = id;
    if (g.LastItemData.ID == id)
        window->NavRectRel[nav_layer] =
                WindowRectAbsToRel(window, g.LastItemData.NavRect);

    if (g.ActiveIdSource == AzInputSource_Keyboard
        || g.ActiveIdSource == AzInputSource_Gamepad)
        g.NavHighlightItemUnderNav = true;
    else if (g.IO.ConfigNavCursorVisibleAuto)
        g.NavCursorVisible = false;

    NavClearPreferredPosForAxis(AzAxis_X);
    NavClearPreferredPosForAxis(AzAxis_Y);
}

static AzDir AzGetDirQuadrantFromDelta(float dx, float dy)
{
    if (AzFabs(dx) > AzFabs(dy))
        return (dx > 0.0f) ? AzDir_Right : AzDir_Left;
    return (dy > 0.0f) ? AzDir_Down : AzDir_Up;
}

static float inline NavScoreItemDistInterval(float cand_min,
                                             float cand_max,
                                             float curr_min,
                                             float curr_max)
{
    if (cand_max < curr_min)
        return cand_max - curr_min;
    if (curr_max < cand_min)
        return cand_min - curr_max;
    return 0.0f;
}

static bool Az::NavScoreItem(AzNavItemData* result)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (g.NavLayer != window->DC.NavLayerCurrent)
        return false;

    AzRect cand = g.LastItemData.NavRect;
    const AzRect curr = g.NavScoringRect;

    g.NavScoringDebugCount++;

    if (window->ParentWindow == g.NavWindow)
    {
        AZ_ASSERT((window->ChildFlags | g.NavWindow->ChildFlags)
                  & AzChildFlags_NavFlattened);
        if (!window->ClipRect.Overlaps(cand))
            return false;
        cand.ClipWithFull(window->ClipRect);
    }

    float dbx = NavScoreItemDistInterval(cand.Min.x,
                                         cand.Max.x,
                                         curr.Min.x,
                                         curr.Max.x);
    float dby = NavScoreItemDistInterval(AzLerp(cand.Min.y, cand.Max.y, 0.2f),
                                         AzLerp(cand.Min.y, cand.Max.y, 0.8f),
                                         AzLerp(curr.Min.y, curr.Max.y, 0.2f),
                                         AzLerp(curr.Min.y, curr.Max.y, 0.8f));

    if (dby != 0.0f && dbx != 0.0f)
        dbx = (dbx / 1000.0f) + ((dbx > 0.0f) ? +1.0f : -1.0f);
    float dist_box = AzFabs(dbx) + AzFabs(dby);

    float dcx = (cand.Min.x + cand.Max.x) - (curr.Min.x + curr.Max.x);
    float dcy = (cand.Min.y + cand.Max.y) - (curr.Min.y + curr.Max.y);
    float dist_center = AzFabs(dcx) + AzFabs(dcy);

    AzDir quadrant;
    float dax = 0.0f, day = 0.0f, dist_axial = 0.0f;
    if (dbx != 0.0f || dby != 0.0f)
    {
        dax = dbx;
        day = dby;
        dist_axial = dist_box;
        quadrant = AzGetDirQuadrantFromDelta(dbx, dby);
    } else if (dcx != 0.0f || dcy != 0.0f)
    {
        dax = dcx;
        day = dcy;
        dist_axial = dist_center;
        quadrant = AzGetDirQuadrantFromDelta(dcx, dcy);
    } else
    {
        quadrant = (g.LastItemData.ID < g.NavId) ? AzDir_Left : AzDir_Right;
    }

    const AzDir move_dir = g.NavMoveDir;
#if AZ_DEBUG_NAV_SCORING
    char buf[200];
    if (g.IO.KeyCtrl)

    {
        if (quadrant == move_dir)
        {
            AzFormatString(buf,
                           AZ_ARRAYSIZE(buf),
                           "%.0f/%.0f",
                           dist_box,
                           dist_center);
            AzDrawList* draw_list = GetForegroundDrawList(window);
            draw_list->AddRectFilled(cand.Min,
                                     cand.Max,
                                     AZ_COL32(255, 0, 0, 80));
            draw_list->AddRectFilled(cand.Min,
                                     cand.Min + CalcTextSize(buf),
                                     AZ_COL32(255, 0, 0, 200));
            draw_list->AddText(cand.Min, AZ_COL32(255, 255, 255, 255), buf);
        }
    }
    const bool debug_hovering = IsMouseHoveringRect(cand.Min, cand.Max);
    const bool debug_tty = (g.IO.KeyCtrl && IsKeyPressed(AzKey_Space));
    if (debug_hovering || debug_tty)
    {
        AzFormatString(
                buf,
                AZ_ARRAYSIZE(buf),
                "d-box    (%7.3f,%7.3f) -> %7.3f\nd-center (%7.3f,%7.3f) -> "
                "%7.3f\nd-axial  (%7.3f,%7.3f) -> %7.3f\nnav %c, quadrant %c",
                dbx,
                dby,
                dist_box,
                dcx,
                dcy,
                dist_center,
                dax,
                day,
                dist_axial,
                "-WENS"[move_dir + 1],
                "-WENS"[quadrant + 1]);
        if (debug_hovering)
        {
            AzDrawList* draw_list = GetForegroundDrawList(window);
            draw_list->AddRect(curr.Min, curr.Max, AZ_COL32(255, 200, 0, 100));
            draw_list->AddRect(cand.Min, cand.Max, AZ_COL32(255, 255, 0, 200));
            draw_list->AddRectFilled(cand.Max - AzVec2(4, 4),
                                     cand.Max + CalcTextSize(buf)
                                             + AzVec2(4, 4),
                                     AZ_COL32(40, 0, 0, 200));
            draw_list->AddText(cand.Max, ~0U, buf);
        }
        if (debug_tty)
        {
            AZ_DEBUG_LOG_NAV("id 0x%08X\n%s\n", g.LastItemData.ID, buf);
        }
    }
#endif

    bool new_best = false;
    if (quadrant == move_dir)
    {
        if (dist_box < result->DistBox)
        {
            result->DistBox = dist_box;
            result->DistCenter = dist_center;
            return true;
        }
        if (dist_box == result->DistBox)
        {
            if (dist_center < result->DistCenter)
            {
                result->DistCenter = dist_center;
                new_best = true;
            } else if (dist_center == result->DistCenter)
            {
                if (((move_dir == AzDir_Up || move_dir == AzDir_Down) ? dby
                                                                      : dbx)
                    < 0.0f)
                    new_best = true;
            }
        }
    }

    if (result->DistBox == FLT_MAX && dist_axial < result->DistAxial)
        if (g.NavLayer == AzNavLayer_Menu
            && !(g.NavWindow->Flags & AzWindowFlags_ChildMenu))
            if ((move_dir == AzDir_Left && dax < 0.0f)
                || (move_dir == AzDir_Right && dax > 0.0f)
                || (move_dir == AzDir_Up && day < 0.0f)
                || (move_dir == AzDir_Down && day > 0.0f))
            {
                result->DistAxial = dist_axial;
                new_best = true;
            }

    return new_best;
}

static void Az::NavApplyItemToResult(AzNavItemData* result)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    result->Window = window;
    result->ID = g.LastItemData.ID;
    result->FocusScopeId = g.CurrentFocusScopeId;
    result->ItemFlags = g.LastItemData.ItemFlags;
    result->RectRel = WindowRectAbsToRel(window, g.LastItemData.NavRect);
    if (result->ItemFlags & AzItemFlags_HasSelectionUserData)
    {
        AZ_ASSERT(g.NextItemData.SelectionUserData
                  != AzSelectionUserData_Invalid);
        result->SelectionUserData = g.NextItemData.SelectionUserData;
    }
}

void Az::NavUpdateCurrentWindowIsScrollPushableX()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    window->DC.NavIsScrollPushableX = (g.CurrentTable == NULL
                                       && window->DC.CurrentColumns == NULL);
}

static void Az::NavProcessItem()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    const AzID id = g.LastItemData.ID;
    const AzItemFlags item_flags = g.LastItemData.ItemFlags;

    if (window->DC.NavIsScrollPushableX == false)
    {
        g.LastItemData.NavRect.Min.x = AzClamp(g.LastItemData.NavRect.Min.x,
                                               window->ClipRect.Min.x,
                                               window->ClipRect.Max.x);
        g.LastItemData.NavRect.Max.x = AzClamp(g.LastItemData.NavRect.Max.x,
                                               window->ClipRect.Min.x,
                                               window->ClipRect.Max.x);
    }
    const AzRect nav_bb = g.LastItemData.NavRect;

    if (g.NavInitRequest && g.NavLayer == window->DC.NavLayerCurrent
        && (item_flags & AzItemFlags_Disabled) == 0)
    {
        const bool candidate_for_nav_default_focus =
                (item_flags & AzItemFlags_NoNavDefaultFocus) == 0;
        if (candidate_for_nav_default_focus || g.NavInitResult.ID == 0)
        {
            NavApplyItemToResult(&g.NavInitResult);
        }
        if (candidate_for_nav_default_focus)
        {
            g.NavInitRequest = false;
            NavUpdateAnyRequestFlag();
        }
    }

    if (g.NavMoveScoringItems && (item_flags & AzItemFlags_Disabled) == 0)
    {
        if ((g.NavMoveFlags & AzNavMoveFlags_FocusApi)
            || (window->Flags & AzWindowFlags_NoNavInputs) == 0)
        {
            const bool is_tabbing = (g.NavMoveFlags & AzNavMoveFlags_IsTabbing)
                                    != 0;
            if (is_tabbing)
            {
                NavProcessItemForTabbingRequest(id, item_flags, g.NavMoveFlags);
            } else if (g.NavId != id
                       || (g.NavMoveFlags & AzNavMoveFlags_AllowCurrentNavId))
            {
                AzNavItemData* result = (window == g.NavWindow)
                                                ? &g.NavMoveResultLocal
                                                : &g.NavMoveResultOther;
                if (NavScoreItem(result))
                    NavApplyItemToResult(result);

                const float VISIBLE_RATIO = 0.70f;
                if ((g.NavMoveFlags & AzNavMoveFlags_AlsoScoreVisibleSet)
                    && window->ClipRect.Overlaps(nav_bb))
                    if (AzClamp(nav_bb.Max.y,
                                window->ClipRect.Min.y,
                                window->ClipRect.Max.y)
                                - AzClamp(nav_bb.Min.y,
                                          window->ClipRect.Min.y,
                                          window->ClipRect.Max.y)
                        >= (nav_bb.Max.y - nav_bb.Min.y) * VISIBLE_RATIO)
                        if (NavScoreItem(&g.NavMoveResultLocalVisible))
                            NavApplyItemToResult(&g.NavMoveResultLocalVisible);
            }
        }
    }

    if (g.NavId == id)
    {
        if (g.NavWindow != window)
            SetNavWindow(window);

        g.NavLayer = window->DC.NavLayerCurrent;
        SetNavFocusScope(g.CurrentFocusScopeId);

        g.NavFocusScopeId = g.CurrentFocusScopeId;
        g.NavIdIsAlive = true;
        if (g.LastItemData.ItemFlags & AzItemFlags_HasSelectionUserData)
        {
            AZ_ASSERT(g.NextItemData.SelectionUserData
                      != AzSelectionUserData_Invalid);
            g.NavLastValidSelectionUserData = g.NextItemData.SelectionUserData;
        }
        window->NavRectRel[window->DC.NavLayerCurrent] =
                WindowRectAbsToRel(window, nav_bb);
    }
}

void Az::NavProcessItemForTabbingRequest(AzID id,
                                         AzItemFlags item_flags,
                                         AzNavMoveFlags move_flags)
{
    AzContext& g = *GAz;

    if ((move_flags & AzNavMoveFlags_FocusApi) == 0)
    {
        if (g.NavLayer != g.CurrentWindow->DC.NavLayerCurrent)
            return;
        if (g.NavFocusScopeId != g.CurrentFocusScopeId)
            return;
    }

    bool can_stop;
    if (move_flags & AzNavMoveFlags_FocusApi)
        can_stop = true;
    else
        can_stop = (item_flags & AzItemFlags_NoTabStop) == 0
                   && ((g.IO.ConfigFlags & AzConfigFlags_NavEnableKeyboard)
                       || (item_flags & AzItemFlags_Inputable));

    AzNavItemData* result = &g.NavMoveResultLocal;
    if (g.NavTabbingDir == +1)
    {
        if (can_stop && g.NavTabbingResultFirst.ID == 0)
            NavApplyItemToResult(&g.NavTabbingResultFirst);
        if (can_stop && g.NavTabbingCounter > 0 && --g.NavTabbingCounter == 0)
            NavMoveRequestResolveWithLastItem(result);
        else if (g.NavId == id)
            g.NavTabbingCounter = 1;
    } else if (g.NavTabbingDir == -1)
    {
        if (g.NavId == id)
        {
            if (result->ID)
            {
                g.NavMoveScoringItems = false;
                NavUpdateAnyRequestFlag();
            }
        } else if (can_stop)
        {
            NavApplyItemToResult(result);
        }
    } else if (g.NavTabbingDir == 0)
    {
        if (can_stop && g.NavId == id)
            NavMoveRequestResolveWithLastItem(result);
        if (can_stop && g.NavTabbingResultFirst.ID == 0)
            NavApplyItemToResult(&g.NavTabbingResultFirst);
    }
}

bool Az::NavMoveRequestButNoResultYet()
{
    AzContext& g = *GAz;
    return g.NavMoveScoringItems && g.NavMoveResultLocal.ID == 0
           && g.NavMoveResultOther.ID == 0;
}

void Az::NavMoveRequestSubmit(AzDir move_dir,
                              AzDir clip_dir,
                              AzNavMoveFlags move_flags,
                              AzScrollFlags scroll_flags)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.NavWindow != NULL);

    if (move_flags & AzNavMoveFlags_IsTabbing)
        move_flags |= AzNavMoveFlags_AllowCurrentNavId;

    g.NavMoveSubmitted = g.NavMoveScoringItems = true;
    g.NavMoveDir = move_dir;
    g.NavMoveDirForDebug = move_dir;
    g.NavMoveClipDir = clip_dir;
    g.NavMoveFlags = move_flags;
    g.NavMoveScrollFlags = scroll_flags;
    g.NavMoveForwardToNextFrame = false;
    g.NavMoveKeyMods = (move_flags & AzNavMoveFlags_FocusApi) ? 0
                                                              : g.IO.KeyMods;
    g.NavMoveResultLocal.Clear();
    g.NavMoveResultLocalVisible.Clear();
    g.NavMoveResultOther.Clear();
    g.NavTabbingCounter = 0;
    g.NavTabbingResultFirst.Clear();
    NavUpdateAnyRequestFlag();
}

void Az::NavMoveRequestResolveWithLastItem(AzNavItemData* result)
{
    AzContext& g = *GAz;
    g.NavMoveScoringItems = false;

    NavApplyItemToResult(result);
    NavUpdateAnyRequestFlag();
}

void Az::NavMoveRequestResolveWithPastTreeNode(
        AzNavItemData* result,
        const AzTreeNodeStackData* tree_node_data)
{
    AzContext& g = *GAz;
    g.NavMoveScoringItems = false;
    g.LastItemData.ID = tree_node_data->ID;
    g.LastItemData.ItemFlags = tree_node_data->ItemFlags
                               & ~AzItemFlags_HasSelectionUserData;

    g.LastItemData.NavRect = tree_node_data->NavRect;
    NavApplyItemToResult(result);

    NavClearPreferredPosForAxis(AzAxis_Y);
    NavUpdateAnyRequestFlag();
}

void Az::NavMoveRequestCancel()
{
    AzContext& g = *GAz;
    g.NavMoveSubmitted = g.NavMoveScoringItems = false;
    NavUpdateAnyRequestFlag();
}

void Az::NavMoveRequestForward(AzDir move_dir,
                               AzDir clip_dir,
                               AzNavMoveFlags move_flags,
                               AzScrollFlags scroll_flags)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.NavMoveForwardToNextFrame == false);
    NavMoveRequestCancel();
    g.NavMoveForwardToNextFrame = true;
    g.NavMoveDir = move_dir;
    g.NavMoveClipDir = clip_dir;
    g.NavMoveFlags = move_flags | AzNavMoveFlags_Forwarded;
    g.NavMoveScrollFlags = scroll_flags;
}

void Az::NavMoveRequestTryWrapping(AzWindow* window, AzNavMoveFlags wrap_flags)
{
    AzContext& g = *GAz;
    AZ_ASSERT((wrap_flags & AzNavMoveFlags_WrapMask_) != 0
              && (wrap_flags & ~AzNavMoveFlags_WrapMask_) == 0);

    if (g.NavWindow == window && g.NavMoveScoringItems
        && g.NavLayer == AzNavLayer_Main)
        g.NavMoveFlags = (g.NavMoveFlags & ~AzNavMoveFlags_WrapMask_)
                         | wrap_flags;
}

static void Az::NavSaveLastChildNavWindowIntoParent(AzWindow* nav_window)
{
    AzWindow* parent = nav_window;
    while (parent && parent->RootWindow != parent
           && (parent->Flags & (AzWindowFlags_Popup | AzWindowFlags_ChildMenu))
                      == 0)
        parent = parent->ParentWindow;
    if (parent && parent != nav_window)
        parent->NavLastChildNavWindow = nav_window;
}

static AzWindow* Az::NavRestoreLastChildNavWindow(AzWindow* window)
{
    if (window->NavLastChildNavWindow
        && window->NavLastChildNavWindow->WasActive)
        return window->NavLastChildNavWindow;
    if (window->DockNodeAsHost && window->DockNodeAsHost->TabBar)
        if (AzTabItem* tab = TabBarFindMostRecentlySelectedTabForActiveWindow(
                    window->DockNodeAsHost->TabBar))
            return tab->Window;
    return window;
}

void Az::NavRestoreLayer(AzNavLayer layer)
{
    AzContext& g = *GAz;
    if (layer == AzNavLayer_Main)
    {
        AzWindow* prev_nav_window = g.NavWindow;
        g.NavWindow = NavRestoreLastChildNavWindow(g.NavWindow);
        g.NavLastValidSelectionUserData = AzSelectionUserData_Invalid;
        if (prev_nav_window)
            AZ_DEBUG_LOG_FOCUS(
                    "[focus] NavRestoreLayer: from \"%s\" to "
                    "SetNavWindow(\"%s\")\n",
                    prev_nav_window->Name,
                    g.NavWindow->Name);
    }
    AzWindow* window = g.NavWindow;
    if (window->NavLastIds[layer] != 0)
    {
        SetNavID(window->NavLastIds[layer],
                 layer,
                 0,
                 window->NavRectRel[layer]);
    } else
    {
        g.NavLayer = layer;
        NavInitWindow(window, true);
    }
}

static inline void Az::NavUpdateAnyRequestFlag()
{
    AzContext& g = *GAz;
    g.NavAnyRequest = g.NavMoveScoringItems || g.NavInitRequest
                      || (AZ_DEBUG_NAV_SCORING && g.NavWindow != NULL);
    if (g.NavAnyRequest)
        AZ_ASSERT(g.NavWindow != NULL);
}

void Az::NavInitWindow(AzWindow* window, bool force_reinit)
{
    AzContext& g = *GAz;
    AZ_ASSERT(window == g.NavWindow);

    if (window->Flags & AzWindowFlags_NoNavInputs)
    {
        g.NavId = 0;
        SetNavFocusScope(window->NavRootFocusScopeId);
        return;
    }

    bool init_for_nav = false;
    if (window == window->RootWindow || (window->Flags & AzWindowFlags_Popup)
        || (window->NavLastIds[0] == 0) || force_reinit)
        init_for_nav = true;
    AZ_DEBUG_LOG_NAV(
            "[nav] NavInitRequest: from NavInitWindow(), init_for_nav=%d, "
            "window=\"%s\", layer=%d\n",
            init_for_nav,
            window->Name,
            g.NavLayer);
    if (init_for_nav)
    {
        SetNavID(0, g.NavLayer, window->NavRootFocusScopeId, AzRect());
        g.NavInitRequest = true;
        g.NavInitRequestFromMove = false;
        g.NavInitResult.ID = 0;
        NavUpdateAnyRequestFlag();
    } else
    {
        g.NavId = window->NavLastIds[0];
        SetNavFocusScope(window->NavRootFocusScopeId);
    }
}

static AzInputSource Az::NavCalcPreferredRefPosSource()
{
    AzContext& g = *GAz;
    AzWindow* window = g.NavWindow;
    const bool activated_shortcut = g.ActiveId != 0 && g.ActiveIdFromShortcut
                                    && g.ActiveId == g.LastItemData.ID;

    if ((!g.NavCursorVisible || !g.NavHighlightItemUnderNav || !window)
        && !activated_shortcut)
        return AzInputSource_Mouse;
    else
        return AzInputSource_Keyboard;
}

static AzVec2 Az::NavCalcPreferredRefPos()
{
    AzContext& g = *GAz;
    AzWindow* window = g.NavWindow;
    AzInputSource source = NavCalcPreferredRefPosSource();

    const bool activated_shortcut = g.ActiveId != 0 && g.ActiveIdFromShortcut
                                    && g.ActiveId == g.LastItemData.ID;

    if (source == AzInputSource_Mouse)
    {
        AzVec2 p = IsMousePosValid(&g.IO.MousePos) ? g.IO.MousePos
                                                   : g.MouseLastValidPos;
        return AzVec2(p.x + 1.0f, p.y);
    } else
    {
        AzRect ref_rect;
        if (activated_shortcut)
            ref_rect = g.LastItemData.NavRect;
        else
            ref_rect = WindowRectRelToAbs(window,
                                          window->NavRectRel[g.NavLayer]);

        if (window->LastFrameActive != g.FrameCount
            && (window->ScrollTarget.x != FLT_MAX
                || window->ScrollTarget.y != FLT_MAX))
        {
            AzVec2 next_scroll = CalcNextScrollFromScrollTargetAndClamp(window);
            ref_rect.Translate(window->Scroll - next_scroll);
        }
        AzVec2 pos = AzVec2(ref_rect.Min.x
                                    + AzMin(g.Style.FramePadding.x * 4,
                                            ref_rect.GetWidth()),
                            ref_rect.Max.y
                                    - AzMin(g.Style.FramePadding.y,
                                            ref_rect.GetHeight()));
        AzViewport* viewport = window->Viewport;
        return AzTrunc(
                AzClamp(pos, viewport->Pos, viewport->Pos + viewport->Size));
    }
}

float Az::GetNavTweakPressedAmount(AzAxis axis)
{
    AzContext& g = *GAz;
    float repeat_delay, repeat_rate;
    GetTypematicRepeatRate(AzInputFlags_RepeatRateNavTweak,
                           &repeat_delay,
                           &repeat_rate);

    AzKey key_less, key_more;
    if (g.NavInputSource == AzInputSource_Gamepad)
    {
        key_less = (axis == AzAxis_X) ? AzKey_GamepadDpadLeft
                                      : AzKey_GamepadDpadUp;
        key_more = (axis == AzAxis_X) ? AzKey_GamepadDpadRight
                                      : AzKey_GamepadDpadDown;
    } else
    {
        key_less = (axis == AzAxis_X) ? AzKey_LeftArrow : AzKey_UpArrow;
        key_more = (axis == AzAxis_X) ? AzKey_RightArrow : AzKey_DownArrow;
    }
    float amount =
            (float)GetKeyPressedAmount(key_more, repeat_delay, repeat_rate)
            - (float)GetKeyPressedAmount(key_less, repeat_delay, repeat_rate);
    if (amount != 0.0f && IsKeyDown(key_less) && IsKeyDown(key_more))

        amount = 0.0f;
    return amount;
}

static void Az::NavUpdate()
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;

    io.WantSetMousePos = false;

    const bool nav_gamepad_active =
            (io.ConfigFlags & AzConfigFlags_NavEnableGamepad) != 0
            && (io.BackendFlags & AzBackendFlags_HasGamepad) != 0;
    const AzKey nav_gamepad_keys_to_change_source[] = {AzKey_GamepadFaceRight,
                                                       AzKey_GamepadFaceLeft,
                                                       AzKey_GamepadFaceUp,
                                                       AzKey_GamepadFaceDown,
                                                       AzKey_GamepadDpadRight,
                                                       AzKey_GamepadDpadLeft,
                                                       AzKey_GamepadDpadUp,
                                                       AzKey_GamepadDpadDown};
    if (nav_gamepad_active)
        for (AzKey key : nav_gamepad_keys_to_change_source)
            if (IsKeyDown(key))
                g.NavInputSource = AzInputSource_Gamepad;
    const bool nav_keyboard_active = (io.ConfigFlags
                                      & AzConfigFlags_NavEnableKeyboard)
                                     != 0;
    const AzKey nav_keyboard_keys_to_change_source[] = {AzKey_Space,
                                                        AzKey_Enter,
                                                        AzKey_Escape,
                                                        AzKey_RightArrow,
                                                        AzKey_LeftArrow,
                                                        AzKey_UpArrow,
                                                        AzKey_DownArrow};
    if (nav_keyboard_active)
        for (AzKey key : nav_keyboard_keys_to_change_source)
            if (IsKeyDown(key))
                g.NavInputSource = AzInputSource_Keyboard;

    g.NavJustMovedToId = 0;
    g.NavJustMovedToFocusScopeId = g.NavJustMovedFromFocusScopeId = 0;
    if (g.NavInitResult.ID != 0)
        NavInitRequestApplyResult();
    g.NavInitRequest = false;
    g.NavInitRequestFromMove = false;
    g.NavInitResult.ID = 0;

    if (g.NavMoveSubmitted)
        NavMoveRequestApplyResult();
    g.NavTabbingCounter = 0;
    g.NavMoveSubmitted = g.NavMoveScoringItems = false;
    if (g.NavCursorHideFrames > 0)
        if (--g.NavCursorHideFrames == 0)
            g.NavCursorVisible = true;

    bool set_mouse_pos = false;
    if (g.NavMousePosDirty && g.NavIdIsAlive)
        if (g.NavCursorVisible && g.NavHighlightItemUnderNav && g.NavWindow)
            set_mouse_pos = true;
    g.NavMousePosDirty = false;
    AZ_ASSERT(g.NavLayer == AzNavLayer_Main || g.NavLayer == AzNavLayer_Menu);

    if (g.NavWindow)
        NavSaveLastChildNavWindowIntoParent(g.NavWindow);
    if (g.NavWindow && g.NavWindow->NavLastChildNavWindow != NULL
        && g.NavLayer == AzNavLayer_Main)
        g.NavWindow->NavLastChildNavWindow = NULL;

    NavUpdateWindowing();

    io.NavActive = (nav_keyboard_active || nav_gamepad_active) && g.NavWindow
                   && !(g.NavWindow->Flags & AzWindowFlags_NoNavInputs);
    io.NavVisible = (io.NavActive && g.NavId != 0 && g.NavCursorVisible)
                    || (g.NavWindowingTarget != NULL);

    NavUpdateCancelRequest();

    g.NavActivateId = g.NavActivateDownId = g.NavActivatePressedId = 0;
    g.NavActivateFlags = AzActivateFlags_None;
    if (g.NavId != 0 && g.NavCursorVisible && !g.NavWindowingTarget
        && g.NavWindow && !(g.NavWindow->Flags & AzWindowFlags_NoNavInputs))
    {
        const bool activate_down =
                (nav_keyboard_active
                 && IsKeyDown(AzKey_Space, AzKeyOwner_NoOwner))
                || (nav_gamepad_active
                    && IsKeyDown(AzKey_NavGamepadActivate, AzKeyOwner_NoOwner));
        const bool activate_pressed =
                activate_down
                && ((nav_keyboard_active
                     && IsKeyPressed(AzKey_Space, 0, AzKeyOwner_NoOwner))
                    || (nav_gamepad_active
                        && IsKeyPressed(AzKey_NavGamepadActivate,
                                        0,
                                        AzKeyOwner_NoOwner)));
        const bool input_down =
                (nav_keyboard_active
                 && (IsKeyDown(AzKey_Enter, AzKeyOwner_NoOwner)
                     || IsKeyDown(AzKey_KeypadEnter, AzKeyOwner_NoOwner)))
                || (nav_gamepad_active
                    && IsKeyDown(AzKey_NavGamepadInput, AzKeyOwner_NoOwner));
        const bool input_pressed =
                input_down
                && ((nav_keyboard_active
                     && (IsKeyPressed(AzKey_Enter, 0, AzKeyOwner_NoOwner)
                         || IsKeyPressed(AzKey_KeypadEnter,
                                         0,
                                         AzKeyOwner_NoOwner)))
                    || (nav_gamepad_active
                        && IsKeyPressed(AzKey_NavGamepadInput,
                                        0,
                                        AzKeyOwner_NoOwner)));
        if (g.ActiveId == 0 && activate_pressed)
        {
            g.NavActivateId = g.NavId;
            g.NavActivateFlags = AzActivateFlags_PreferTweak;
        }
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && input_pressed)
        {
            g.NavActivateId = g.NavId;
            g.NavActivateFlags = AzActivateFlags_PreferInput;
        }
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId)
            && (activate_down || input_down))
            g.NavActivateDownId = g.NavId;
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId)
            && (activate_pressed || input_pressed))
        {
            g.NavActivatePressedId = g.NavId;
            NavHighlightActivated(g.NavId);
        }
    }
    if (g.NavWindow && (g.NavWindow->Flags & AzWindowFlags_NoNavInputs))
        g.NavCursorVisible = false;
    else if (g.IO.ConfigNavCursorVisibleAlways && g.NavCursorHideFrames == 0)
        g.NavCursorVisible = true;
    if (g.NavActivateId != 0)
        AZ_ASSERT(g.NavActivateDownId == g.NavActivateId);

    if (g.NavHighlightActivatedTimer > 0.0f)
        g.NavHighlightActivatedTimer = AzMax(0.0f,
                                             g.NavHighlightActivatedTimer
                                                     - io.DeltaTime);
    if (g.NavHighlightActivatedTimer == 0.0f)
        g.NavHighlightActivatedId = 0;

    if (g.NavNextActivateId != 0)
    {
        g.NavActivateId = g.NavActivateDownId = g.NavActivatePressedId =
                g.NavNextActivateId;
        g.NavActivateFlags = g.NavNextActivateFlags;
    }
    g.NavNextActivateId = 0;

    NavUpdateCreateMoveRequest();
    if (g.NavMoveDir == AzDir_None)
        NavUpdateCreateTabbingRequest();
    NavUpdateAnyRequestFlag();
    g.NavIdIsAlive = false;

    if (g.NavWindow && !(g.NavWindow->Flags & AzWindowFlags_NoNavInputs)
        && !g.NavWindowingTarget)
    {
        AzWindow* window = g.NavWindow;
        const float scroll_speed = AZ_ROUND(window->FontRefSize * 100
                                            * io.DeltaTime);

        const AzDir move_dir = g.NavMoveDir;
        if (window->DC.NavLayersActiveMask == 0x00
            && window->DC.NavWindowHasScrollY && move_dir != AzDir_None)
        {
            if (move_dir == AzDir_Left || move_dir == AzDir_Right)
                SetScrollX(window,
                           AzTrunc(window->Scroll.x
                                   + ((move_dir == AzDir_Left) ? -1.0f : +1.0f)
                                             * scroll_speed));
            if (move_dir == AzDir_Up || move_dir == AzDir_Down)
                SetScrollY(window,
                           AzTrunc(window->Scroll.y
                                   + ((move_dir == AzDir_Up) ? -1.0f : +1.0f)
                                             * scroll_speed));
        }

        if (nav_gamepad_active)
        {
            const AzVec2 scroll_dir =
                    GetKeyMagnitude2d(AzKey_GamepadLStickLeft,
                                      AzKey_GamepadLStickRight,
                                      AzKey_GamepadLStickUp,
                                      AzKey_GamepadLStickDown);
            const float tweak_factor = IsKeyDown(AzKey_NavGamepadTweakSlow)
                                               ? 1.0f / 10.0f
                                       : IsKeyDown(AzKey_NavGamepadTweakFast)
                                               ? 10.0f
                                               : 1.0f;
            if (scroll_dir.x != 0.0f && window->ScrollbarX)
                SetScrollX(window,
                           AzTrunc(window->Scroll.x
                                   + scroll_dir.x * scroll_speed
                                             * tweak_factor));
            if (scroll_dir.y != 0.0f)
                SetScrollY(window,
                           AzTrunc(window->Scroll.y
                                   + scroll_dir.y * scroll_speed
                                             * tweak_factor));
        }
    }

    if (!nav_keyboard_active && !nav_gamepad_active)
    {
        g.NavCursorVisible = false;
        g.NavHighlightItemUnderNav = set_mouse_pos = false;
    }

    if (set_mouse_pos && io.ConfigNavMoveSetMousePos
        && (io.BackendFlags & AzBackendFlags_HasSetMousePos))
        TeleportMousePos(NavCalcPreferredRefPos());

    g.NavScoringDebugCount = 0;
#if AZ_DEBUG_NAV_RECTS
    if (AzWindow* debug_window = g.NavWindow)
    {
        AzDrawList* draw_list = GetForegroundDrawList(debug_window);
        int layer = g.NavLayer;
        {
            AzRect r = WindowRectRelToAbs(debug_window,
                                          debug_window->NavRectRel[layer]);
            draw_list->AddRect(r.Min, r.Max, AZ_COL32(255, 200, 0, 255));
        }
    }
#endif
}

void Az::NavInitRequestApplyResult()
{
    AzContext& g = *GAz;
    if (!g.NavWindow)
        return;

    AzNavItemData* result = &g.NavInitResult;
    if (g.NavId != result->ID)
    {
        g.NavJustMovedFromFocusScopeId = g.NavFocusScopeId;
        g.NavJustMovedToId = result->ID;
        g.NavJustMovedToFocusScopeId = result->FocusScopeId;
        g.NavJustMovedToKeyMods = 0;
        g.NavJustMovedToIsTabbing = false;
        g.NavJustMovedToHasSelectionData = (result->ItemFlags
                                            & AzItemFlags_HasSelectionUserData)
                                           != 0;
    }

    AZ_DEBUG_LOG_NAV(
            "[nav] NavInitRequest: ApplyResult: NavID 0x%08X in Layer %d "
            "Window \"%s\"\n",
            result->ID,
            g.NavLayer,
            g.NavWindow->Name);
    SetNavID(result->ID, g.NavLayer, result->FocusScopeId, result->RectRel);
    g.NavIdIsAlive = true;

    if (result->SelectionUserData != AzSelectionUserData_Invalid)
        g.NavLastValidSelectionUserData = result->SelectionUserData;
    if (g.NavInitRequestFromMove)
        SetNavCursorVisibleAfterMove();
}

static void NavBiasScoringRect(AzRect& r,
                               AzVec2& preferred_pos_rel,
                               AzDir move_dir,
                               AzNavMoveFlags move_flags)
{
    AzContext& g = *GAz;
    const AzVec2 rel_to_abs_offset = g.NavWindow->DC.CursorStartPos;

    if ((move_flags & AzNavMoveFlags_Forwarded) == 0)
    {
        if (preferred_pos_rel.x == FLT_MAX)
            preferred_pos_rel.x = AzMin(r.Min.x + 1.0f, r.Max.x)
                                  - rel_to_abs_offset.x;
        if (preferred_pos_rel.y == FLT_MAX)
            preferred_pos_rel.y = r.GetCenter().y - rel_to_abs_offset.y;
    }

    if ((move_dir == AzDir_Up || move_dir == AzDir_Down)
        && preferred_pos_rel.x != FLT_MAX)
        r.Min.x = r.Max.x = preferred_pos_rel.x + rel_to_abs_offset.x;
    else if ((move_dir == AzDir_Left || move_dir == AzDir_Right)
             && preferred_pos_rel.y != FLT_MAX)
        r.Min.y = r.Max.y = preferred_pos_rel.y + rel_to_abs_offset.y;
}

void Az::NavUpdateCreateMoveRequest()
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;
    AzWindow* window = g.NavWindow;
    const bool nav_gamepad_active =
            (io.ConfigFlags & AzConfigFlags_NavEnableGamepad) != 0
            && (io.BackendFlags & AzBackendFlags_HasGamepad) != 0;
    const bool nav_keyboard_active = (io.ConfigFlags
                                      & AzConfigFlags_NavEnableKeyboard)
                                     != 0;

    if (g.NavMoveForwardToNextFrame && window != NULL)
    {
        AZ_ASSERT(g.NavMoveDir != AzDir_None && g.NavMoveClipDir != AzDir_None);
        AZ_ASSERT(g.NavMoveFlags & AzNavMoveFlags_Forwarded);
        AZ_DEBUG_LOG_NAV("[nav] NavMoveRequestForward %d\n", g.NavMoveDir);
    } else
    {
        g.NavMoveDir = AzDir_None;
        g.NavMoveFlags = AzNavMoveFlags_None;
        g.NavMoveScrollFlags = AzScrollFlags_None;
        if (window && !g.NavWindowingTarget
            && !(window->Flags & AzWindowFlags_NoNavInputs))
        {
            const AzInputFlags repeat_mode =
                    AzInputFlags_Repeat
                    | (AzInputFlags)AzInputFlags_RepeatRateNavMove;
            if (!IsActiveIdUsingNavDir(AzDir_Left)
                && ((nav_gamepad_active
                     && IsKeyPressed(AzKey_GamepadDpadLeft,
                                     repeat_mode,
                                     AzKeyOwner_NoOwner))
                    || (nav_keyboard_active
                        && IsKeyPressed(AzKey_LeftArrow,
                                        repeat_mode,
                                        AzKeyOwner_NoOwner))))
            {
                g.NavMoveDir = AzDir_Left;
            }
            if (!IsActiveIdUsingNavDir(AzDir_Right)
                && ((nav_gamepad_active
                     && IsKeyPressed(AzKey_GamepadDpadRight,
                                     repeat_mode,
                                     AzKeyOwner_NoOwner))
                    || (nav_keyboard_active
                        && IsKeyPressed(AzKey_RightArrow,
                                        repeat_mode,
                                        AzKeyOwner_NoOwner))))
            {
                g.NavMoveDir = AzDir_Right;
            }
            if (!IsActiveIdUsingNavDir(AzDir_Up)
                && ((nav_gamepad_active
                     && IsKeyPressed(AzKey_GamepadDpadUp,
                                     repeat_mode,
                                     AzKeyOwner_NoOwner))
                    || (nav_keyboard_active
                        && IsKeyPressed(AzKey_UpArrow,
                                        repeat_mode,
                                        AzKeyOwner_NoOwner))))
            {
                g.NavMoveDir = AzDir_Up;
            }
            if (!IsActiveIdUsingNavDir(AzDir_Down)
                && ((nav_gamepad_active
                     && IsKeyPressed(AzKey_GamepadDpadDown,
                                     repeat_mode,
                                     AzKeyOwner_NoOwner))
                    || (nav_keyboard_active
                        && IsKeyPressed(AzKey_DownArrow,
                                        repeat_mode,
                                        AzKeyOwner_NoOwner))))
            {
                g.NavMoveDir = AzDir_Down;
            }
        }
        g.NavMoveClipDir = g.NavMoveDir;
        g.NavScoringNoClipRect = AzRect(+FLT_MAX, +FLT_MAX, -FLT_MAX, -FLT_MAX);
    }

    float scoring_rect_offset_y = 0.0f;
    if (window && g.NavMoveDir == AzDir_None && nav_keyboard_active)
        scoring_rect_offset_y = NavUpdatePageUpPageDown();
    if (scoring_rect_offset_y != 0.0f)
    {
        g.NavScoringNoClipRect = window->InnerRect;
        g.NavScoringNoClipRect.TranslateY(scoring_rect_offset_y);
    }

#if AZ_DEBUG_NAV_SCORING

    if (io.KeyCtrl)
    {
        if (g.NavMoveDir == AzDir_None)
            g.NavMoveDir = g.NavMoveDirForDebug;
        g.NavMoveClipDir = g.NavMoveDir;
        g.NavMoveFlags |= AzNavMoveFlags_DebugNoResult;
    }
#endif

    g.NavMoveForwardToNextFrame = false;
    if (g.NavMoveDir != AzDir_None)
        NavMoveRequestSubmit(g.NavMoveDir,
                             g.NavMoveClipDir,
                             g.NavMoveFlags,
                             g.NavMoveScrollFlags);

    if (g.NavMoveSubmitted && g.NavId == 0)
    {
        AZ_DEBUG_LOG_NAV(
                "[nav] NavInitRequest: from move, window \"%s\", layer=%d\n",
                window ? window->Name : "<NULL>",
                g.NavLayer);
        g.NavInitRequest = g.NavInitRequestFromMove = true;
        g.NavInitResult.ID = 0;
        if (g.IO.ConfigNavCursorVisibleAuto)
            g.NavCursorVisible = true;
    }

    if (g.NavMoveSubmitted && g.NavInputSource == AzInputSource_Gamepad
        && g.NavLayer == AzNavLayer_Main && window != NULL)
    {
        bool clamp_x = (g.NavMoveFlags
                        & (AzNavMoveFlags_LoopX | AzNavMoveFlags_WrapX))
                       == 0;
        bool clamp_y = (g.NavMoveFlags
                        & (AzNavMoveFlags_LoopY | AzNavMoveFlags_WrapY))
                       == 0;
        AzRect inner_rect_rel = WindowRectAbsToRel(
                window,
                AzRect(window->InnerRect.Min - AzVec2(1, 1),
                       window->InnerRect.Max + AzVec2(1, 1)));

        inner_rect_rel.Translate(CalcNextScrollFromScrollTargetAndClamp(window)
                                 - window->Scroll);

        if ((clamp_x || clamp_y)
            && !inner_rect_rel.Contains(window->NavRectRel[g.NavLayer]))
        {
            AZ_DEBUG_LOG_NAV(
                    "[nav] NavMoveRequest: clamp NavRectRel for gamepad "
                    "move\n");
            float pad_x = AzMin(inner_rect_rel.GetWidth(),
                                window->FontRefSize * 0.5f);
            float pad_y = AzMin(inner_rect_rel.GetHeight(),
                                window->FontRefSize * 0.5f);

            inner_rect_rel.Min.x = clamp_x ? (inner_rect_rel.Min.x + pad_x)
                                           : -FLT_MAX;
            inner_rect_rel.Max.x = clamp_x ? (inner_rect_rel.Max.x - pad_x)
                                           : +FLT_MAX;
            inner_rect_rel.Min.y = clamp_y ? (inner_rect_rel.Min.y + pad_y)
                                           : -FLT_MAX;
            inner_rect_rel.Max.y = clamp_y ? (inner_rect_rel.Max.y - pad_y)
                                           : +FLT_MAX;
            window->NavRectRel[g.NavLayer].ClipWithFull(inner_rect_rel);
            g.NavId = 0;
        }
    }

    AzRect scoring_rect;
    if (window != NULL)
    {
        AzRect nav_rect_rel = !window->NavRectRel[g.NavLayer].IsInverted()
                                      ? window->NavRectRel[g.NavLayer]
                                      : AzRect(0, 0, 0, 0);
        scoring_rect = WindowRectRelToAbs(window, nav_rect_rel);
        scoring_rect.TranslateY(scoring_rect_offset_y);
        if (g.NavMoveSubmitted)
            NavBiasScoringRect(scoring_rect,
                               window->RootWindowForNav
                                       ->NavPreferredScoringPosRel[g.NavLayer],
                               g.NavMoveDir,
                               g.NavMoveFlags);
        AZ_ASSERT(!scoring_rect.IsInverted());
    }
    g.NavScoringRect = scoring_rect;
}

void Az::NavUpdateCreateTabbingRequest()
{
    AzContext& g = *GAz;
    AzWindow* window = g.NavWindow;
    AZ_ASSERT(g.NavMoveDir == AzDir_None);
    if (window == NULL || g.NavWindowingTarget != NULL
        || (window->Flags & AzWindowFlags_NoNavInputs))
        return;

    const bool tab_pressed = IsKeyPressed(AzKey_Tab,
                                          AzInputFlags_Repeat,
                                          AzKeyOwner_NoOwner)
                             && !g.IO.KeyCtrl && !g.IO.KeyAlt;
    if (!tab_pressed)
        return;

    const bool nav_keyboard_active = (g.IO.ConfigFlags
                                      & AzConfigFlags_NavEnableKeyboard)
                                     != 0;
    if (nav_keyboard_active)
        g.NavTabbingDir = g.IO.KeyShift ? -1
                          : (g.NavCursorVisible == false && g.ActiveId == 0)
                                  ? 0
                                  : +1;
    else
        g.NavTabbingDir = g.IO.KeyShift ? -1 : (g.ActiveId == 0) ? 0 : +1;
    AzNavMoveFlags move_flags = AzNavMoveFlags_IsTabbing
                                | AzNavMoveFlags_Activate;
    AzScrollFlags scroll_flags =
            window->Appearing ? AzScrollFlags_KeepVisibleEdgeX
                                        | AzScrollFlags_AlwaysCenterY
                              : AzScrollFlags_KeepVisibleEdgeX
                                        | AzScrollFlags_KeepVisibleEdgeY;
    AzDir clip_dir = (g.NavTabbingDir < 0) ? AzDir_Up : AzDir_Down;
    NavMoveRequestSubmit(AzDir_None, clip_dir, move_flags, scroll_flags);

    g.NavTabbingCounter = -1;
}

void Az::NavMoveRequestApplyResult()
{
    AzContext& g = *GAz;
#if AZ_DEBUG_NAV_SCORING
    if (g.NavMoveFlags & AzNavMoveFlags_DebugNoResult)

        return;
#endif

    AzNavItemData* result = (g.NavMoveResultLocal.ID != 0)
                                    ? &g.NavMoveResultLocal
                            : (g.NavMoveResultOther.ID != 0)
                                    ? &g.NavMoveResultOther
                                    : NULL;

    if ((g.NavMoveFlags & AzNavMoveFlags_IsTabbing) && result == NULL)
        if ((g.NavTabbingCounter == 1 || g.NavTabbingDir == 0)
            && g.NavTabbingResultFirst.ID)
            result = &g.NavTabbingResultFirst;

    const AzAxis axis = (g.NavMoveDir == AzDir_Up || g.NavMoveDir == AzDir_Down)
                                ? AzAxis_Y
                                : AzAxis_X;
    if (result == NULL)
    {
        if (g.NavMoveFlags & AzNavMoveFlags_IsTabbing)
            g.NavMoveFlags |= AzNavMoveFlags_NoSetNavCursorVisible;
        if (g.NavId != 0
            && (g.NavMoveFlags & AzNavMoveFlags_NoSetNavCursorVisible) == 0)
            SetNavCursorVisibleAfterMove();
        NavClearPreferredPosForAxis(axis);
        AZ_DEBUG_LOG_NAV("[nav] NavMoveSubmitted but not led to a result!\n");
        return;
    }

    if (g.NavMoveFlags & AzNavMoveFlags_AlsoScoreVisibleSet)
        if (g.NavMoveResultLocalVisible.ID != 0
            && g.NavMoveResultLocalVisible.ID != g.NavId)
            result = &g.NavMoveResultLocalVisible;

    if (result != &g.NavMoveResultOther && g.NavMoveResultOther.ID != 0
        && g.NavMoveResultOther.Window->ParentWindow == g.NavWindow)
        if ((g.NavMoveResultOther.DistBox < result->DistBox)
            || (g.NavMoveResultOther.DistBox == result->DistBox
                && g.NavMoveResultOther.DistCenter < result->DistCenter))
            result = &g.NavMoveResultOther;
    AZ_ASSERT(g.NavWindow && result->Window);

    if (g.NavLayer == AzNavLayer_Main)
    {
        AzRect rect_abs = WindowRectRelToAbs(result->Window, result->RectRel);
        ScrollToRectEx(result->Window, rect_abs, g.NavMoveScrollFlags);

        if (g.NavMoveFlags & AzNavMoveFlags_ScrollToEdgeY)
        {
            float scroll_target = (g.NavMoveDir == AzDir_Up)
                                          ? result->Window->ScrollMax.y
                                          : 0.0f;
            SetScrollY(result->Window, scroll_target);
        }
    }

    if (g.NavWindow != result->Window)
    {
        AZ_DEBUG_LOG_FOCUS("[focus] NavMoveRequest: SetNavWindow(\"%s\")\n",
                           result->Window->Name);
        g.NavWindow = result->Window;
        g.NavLastValidSelectionUserData = AzSelectionUserData_Invalid;
    }

    if (g.ActiveId != result->ID
        && (g.NavMoveFlags & AzNavMoveFlags_NoClearActiveId) == 0)
        ClearActiveID();

    if ((g.NavId != result->ID || (g.NavMoveFlags & AzNavMoveFlags_IsPageMove))
        && (g.NavMoveFlags & AzNavMoveFlags_NoSelect) == 0)
    {
        g.NavJustMovedFromFocusScopeId = g.NavFocusScopeId;
        g.NavJustMovedToId = result->ID;
        g.NavJustMovedToFocusScopeId = result->FocusScopeId;
        g.NavJustMovedToKeyMods = g.NavMoveKeyMods;
        g.NavJustMovedToIsTabbing = (g.NavMoveFlags & AzNavMoveFlags_IsTabbing)
                                    != 0;
        g.NavJustMovedToHasSelectionData = (result->ItemFlags
                                            & AzItemFlags_HasSelectionUserData)
                                           != 0;
    }

    AZ_DEBUG_LOG_NAV(
            "[nav] NavMoveRequest: result NavID 0x%08X in Layer %d Window "
            "\"%s\"\n",
            result->ID,
            g.NavLayer,
            g.NavWindow->Name);
    AzVec2 preferred_scoring_pos_rel =
            g.NavWindow->RootWindowForNav
                    ->NavPreferredScoringPosRel[g.NavLayer];
    SetNavID(result->ID, g.NavLayer, result->FocusScopeId, result->RectRel);
    if (result->SelectionUserData != AzSelectionUserData_Invalid)
        g.NavLastValidSelectionUserData = result->SelectionUserData;

    if ((g.NavMoveFlags & AzNavMoveFlags_IsTabbing) == 0)
    {
        preferred_scoring_pos_rel[axis] = result->RectRel.GetCenter()[axis];
        g.NavWindow->RootWindowForNav->NavPreferredScoringPosRel[g.NavLayer] =
                preferred_scoring_pos_rel;
    }

    if ((g.NavMoveFlags & AzNavMoveFlags_IsTabbing)
        && (result->ItemFlags & AzItemFlags_Inputable) == 0)
        g.NavMoveFlags &= ~AzNavMoveFlags_Activate;

    if (g.NavMoveFlags & AzNavMoveFlags_Activate)
    {
        g.NavNextActivateId = result->ID;
        g.NavNextActivateFlags = AzActivateFlags_None;
        if (g.NavMoveFlags & AzNavMoveFlags_IsTabbing)
            g.NavNextActivateFlags |= AzActivateFlags_PreferInput
                                      | AzActivateFlags_TryToPreserveState
                                      | AzActivateFlags_FromTabbing;
    }

    if ((g.NavMoveFlags & AzNavMoveFlags_NoSetNavCursorVisible) == 0)
        SetNavCursorVisibleAfterMove();
}

static void Az::NavUpdateCancelRequest()
{
    AzContext& g = *GAz;
    const bool nav_gamepad_active =
            (g.IO.ConfigFlags & AzConfigFlags_NavEnableGamepad) != 0
            && (g.IO.BackendFlags & AzBackendFlags_HasGamepad) != 0;
    const bool nav_keyboard_active = (g.IO.ConfigFlags
                                      & AzConfigFlags_NavEnableKeyboard)
                                     != 0;
    if (!(nav_keyboard_active
          && IsKeyPressed(AzKey_Escape, 0, AzKeyOwner_NoOwner))
        && !(nav_gamepad_active
             && IsKeyPressed(AzKey_NavGamepadCancel, 0, AzKeyOwner_NoOwner)))
        return;

    AZ_DEBUG_LOG_NAV("[nav] NavUpdateCancelRequest()\n");
    if (g.ActiveId != 0)
    {
        ClearActiveID();
    } else if (g.NavLayer != AzNavLayer_Main)
    {
        NavRestoreLayer(AzNavLayer_Main);
        SetNavCursorVisibleAfterMove();
    } else if (g.NavWindow && g.NavWindow != g.NavWindow->RootWindow
               && !(g.NavWindow->RootWindowForNav->Flags & AzWindowFlags_Popup)
               && g.NavWindow->RootWindowForNav->ParentWindow)
    {
        AzWindow* child_window = g.NavWindow->RootWindowForNav;
        AzWindow* parent_window = child_window->ParentWindow;
        AZ_ASSERT(child_window->ChildId != 0);
        FocusWindow(parent_window);
        SetNavID(child_window->ChildId,
                 AzNavLayer_Main,
                 0,
                 WindowRectAbsToRel(parent_window, child_window->Rect()));
        SetNavCursorVisibleAfterMove();
    } else if (g.OpenPopupStack.Size > 0
               && g.OpenPopupStack.back().Window != NULL
               && !(g.OpenPopupStack.back().Window->Flags
                    & AzWindowFlags_Modal))
    {
        ClosePopupToLevel(g.OpenPopupStack.Size - 1, true);
    } else
    {
        if (g.IO.ConfigNavEscapeClearFocusItem
            || g.IO.ConfigNavEscapeClearFocusWindow)
            if (g.NavWindow && ((g.NavWindow->Flags & AzWindowFlags_Popup)))

                g.NavWindow->NavLastIds[0] = 0;

        if (g.IO.ConfigNavEscapeClearFocusItem
            || g.IO.ConfigNavEscapeClearFocusWindow)
            g.NavId = 0;
        if (g.IO.ConfigNavEscapeClearFocusWindow)
            FocusWindow(NULL);
    }
}

static float Az::NavUpdatePageUpPageDown()
{
    AzContext& g = *GAz;
    AzWindow* window = g.NavWindow;
    if ((window->Flags & AzWindowFlags_NoNavInputs)
        || g.NavWindowingTarget != NULL)
        return 0.0f;

    const bool page_up_held = IsKeyDown(AzKey_PageUp, AzKeyOwner_NoOwner);
    const bool page_down_held = IsKeyDown(AzKey_PageDown, AzKeyOwner_NoOwner);
    const bool home_pressed = IsKeyPressed(AzKey_Home,
                                           AzInputFlags_Repeat,
                                           AzKeyOwner_NoOwner);
    const bool end_pressed = IsKeyPressed(AzKey_End,
                                          AzInputFlags_Repeat,
                                          AzKeyOwner_NoOwner);
    if (page_up_held == page_down_held && home_pressed == end_pressed)

        return 0.0f;

    if (g.NavLayer != AzNavLayer_Main)
        NavRestoreLayer(AzNavLayer_Main);

    if ((window->DC.NavLayersActiveMask & (1 << AzNavLayer_Main)) == 0
        && window->DC.NavWindowHasScrollY)
    {
        if (IsKeyPressed(AzKey_PageUp, AzInputFlags_Repeat, AzKeyOwner_NoOwner))
            SetScrollY(window,
                       window->Scroll.y - window->InnerRect.GetHeight());
        else if (IsKeyPressed(AzKey_PageDown,
                              AzInputFlags_Repeat,
                              AzKeyOwner_NoOwner))
            SetScrollY(window,
                       window->Scroll.y + window->InnerRect.GetHeight());
        else if (home_pressed)
            SetScrollY(window, 0.0f);
        else if (end_pressed)
            SetScrollY(window, window->ScrollMax.y);
    } else
    {
        AzRect& nav_rect_rel = window->NavRectRel[g.NavLayer];
        const float page_offset_y = AzMax(0.0f,
                                          window->InnerRect.GetHeight()
                                                  - window->FontRefSize * 1.0f
                                                  + nav_rect_rel.GetHeight());
        float nav_scoring_rect_offset_y = 0.0f;
        if (IsKeyPressed(AzKey_PageUp, true))
        {
            nav_scoring_rect_offset_y = -page_offset_y;
            g.NavMoveDir = AzDir_Down;

            g.NavMoveClipDir = AzDir_Up;
            g.NavMoveFlags = AzNavMoveFlags_AllowCurrentNavId
                             | AzNavMoveFlags_AlsoScoreVisibleSet
                             | AzNavMoveFlags_IsPageMove;
        } else if (IsKeyPressed(AzKey_PageDown, true))
        {
            nav_scoring_rect_offset_y = +page_offset_y;
            g.NavMoveDir = AzDir_Up;

            g.NavMoveClipDir = AzDir_Down;
            g.NavMoveFlags = AzNavMoveFlags_AllowCurrentNavId
                             | AzNavMoveFlags_AlsoScoreVisibleSet
                             | AzNavMoveFlags_IsPageMove;
        } else if (home_pressed)
        {
            nav_rect_rel.Min.y = nav_rect_rel.Max.y = 0.0f;
            if (nav_rect_rel.IsInverted())
                nav_rect_rel.Min.x = nav_rect_rel.Max.x = 0.0f;
            g.NavMoveDir = AzDir_Down;
            g.NavMoveFlags = AzNavMoveFlags_AllowCurrentNavId
                             | AzNavMoveFlags_ScrollToEdgeY;

        } else if (end_pressed)
        {
            nav_rect_rel.Min.y = nav_rect_rel.Max.y = window->ContentSize.y;
            if (nav_rect_rel.IsInverted())
                nav_rect_rel.Min.x = nav_rect_rel.Max.x = 0.0f;
            g.NavMoveDir = AzDir_Up;
            g.NavMoveFlags = AzNavMoveFlags_AllowCurrentNavId
                             | AzNavMoveFlags_ScrollToEdgeY;
        }
        return nav_scoring_rect_offset_y;
    }
    return 0.0f;
}

static void Az::NavEndFrame()
{
    AzContext& g = *GAz;

    if (g.NavWindowingTarget != NULL)
        NavUpdateWindowingOverlay();

    if (g.NavWindow && NavMoveRequestButNoResultYet()
        && (g.NavMoveFlags & AzNavMoveFlags_WrapMask_)
        && (g.NavMoveFlags & AzNavMoveFlags_Forwarded) == 0)
        NavUpdateCreateWrappingRequest();
}

static void Az::NavUpdateCreateWrappingRequest()
{
    AzContext& g = *GAz;
    AzWindow* window = g.NavWindow;

    bool do_forward = false;
    AzRect bb_rel = window->NavRectRel[g.NavLayer];
    AzDir clip_dir = g.NavMoveDir;

    const AzNavMoveFlags move_flags = g.NavMoveFlags;

    if (g.NavMoveDir == AzDir_Left
        && (move_flags & (AzNavMoveFlags_WrapX | AzNavMoveFlags_LoopX)))
    {
        bb_rel.Min.x = bb_rel.Max.x = window->ContentSize.x
                                      + window->WindowPadding.x;
        if (move_flags & AzNavMoveFlags_WrapX)
        {
            bb_rel.TranslateY(-bb_rel.GetHeight());
            clip_dir = AzDir_Up;
        }
        do_forward = true;
    }
    if (g.NavMoveDir == AzDir_Right
        && (move_flags & (AzNavMoveFlags_WrapX | AzNavMoveFlags_LoopX)))
    {
        bb_rel.Min.x = bb_rel.Max.x = -window->WindowPadding.x;
        if (move_flags & AzNavMoveFlags_WrapX)
        {
            bb_rel.TranslateY(+bb_rel.GetHeight());
            clip_dir = AzDir_Down;
        }
        do_forward = true;
    }
    if (g.NavMoveDir == AzDir_Up
        && (move_flags & (AzNavMoveFlags_WrapY | AzNavMoveFlags_LoopY)))
    {
        bb_rel.Min.y = bb_rel.Max.y = window->ContentSize.y
                                      + window->WindowPadding.y;
        if (move_flags & AzNavMoveFlags_WrapY)
        {
            bb_rel.TranslateX(-bb_rel.GetWidth());
            clip_dir = AzDir_Left;
        }
        do_forward = true;
    }
    if (g.NavMoveDir == AzDir_Down
        && (move_flags & (AzNavMoveFlags_WrapY | AzNavMoveFlags_LoopY)))
    {
        bb_rel.Min.y = bb_rel.Max.y = -window->WindowPadding.y;
        if (move_flags & AzNavMoveFlags_WrapY)
        {
            bb_rel.TranslateX(+bb_rel.GetWidth());
            clip_dir = AzDir_Right;
        }
        do_forward = true;
    }
    if (!do_forward)
        return;
    window->NavRectRel[g.NavLayer] = bb_rel;
    NavClearPreferredPosForAxis(AzAxis_X);
    NavClearPreferredPosForAxis(AzAxis_Y);
    NavMoveRequestForward(g.NavMoveDir,
                          clip_dir,
                          move_flags,
                          g.NavMoveScrollFlags);
}

bool Az::IsWindowNavFocusable(AzWindow* window)
{
    return window->WasActive && window == window->RootWindow
           && !(window->Flags & AzWindowFlags_NoNavFocus);
}

static AzWindow* FindWindowNavFocusable(int i_start, int i_stop, int dir)
{
    AzContext& g = *GAz;
    for (int i = i_start; i >= 0 && i < g.WindowsFocusOrder.Size && i != i_stop;
         i += dir)
        if (Az::IsWindowNavFocusable(g.WindowsFocusOrder[i]))
            return g.WindowsFocusOrder[i];
    return NULL;
}

static void NavUpdateWindowingTarget(int focus_change_dir)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.NavWindowingTarget);
    if (g.NavWindowingTarget->Flags & AzWindowFlags_Modal)
        return;

    const int i_current = Az::FindWindowFocusIndex(g.NavWindowingTarget);
    AzWindow* window_target = FindWindowNavFocusable(i_current
                                                             + focus_change_dir,
                                                     -INT_MAX,
                                                     focus_change_dir);
    if (!window_target)
        window_target = FindWindowNavFocusable(
                (focus_change_dir < 0) ? (g.WindowsFocusOrder.Size - 1) : 0,
                i_current,
                focus_change_dir);
    if (window_target)

    {
        g.NavWindowingTarget = g.NavWindowingTargetAnim = window_target;
        g.NavWindowingAccumDeltaPos = g.NavWindowingAccumDeltaSize =
                AzVec2(0.0f, 0.0f);
    }
    g.NavWindowingToggleLayer = false;
}

static void Az::NavUpdateWindowingApplyFocus(AzWindow* apply_focus_window)
{
    AzContext& g = *GAz;
    if (g.NavWindow == NULL || apply_focus_window != g.NavWindow->RootWindow)
    {
        AzViewport* previous_viewport = g.NavWindow ? g.NavWindow->Viewport
                                                    : NULL;
        ClearActiveID();
        SetNavCursorVisibleAfterMove();
        ClosePopupsOverWindow(apply_focus_window, false);
        FocusWindow(apply_focus_window,
                    AzFocusRequestFlags_RestoreFocusedChild);
        apply_focus_window = g.NavWindow;
        if (apply_focus_window->NavLastIds[0] == 0)
            NavInitWindow(apply_focus_window, false);

        if (apply_focus_window->DC.NavLayersActiveMaskNext
            == (1 << AzNavLayer_Menu))
            g.NavLayer = AzNavLayer_Menu;

        if (apply_focus_window->Viewport != previous_viewport
            && g.PlatformIO.Platform_SetWindowFocus)
            g.PlatformIO.Platform_SetWindowFocus(apply_focus_window->Viewport);
    }
    g.NavWindowingTarget = NULL;
}

static void Az::NavUpdateWindowing()
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;

    AzWindow* apply_focus_window = NULL;
    bool apply_toggle_layer = false;

    AzWindow* modal_window = GetTopMostPopupModal();
    bool allow_windowing = (modal_window == NULL);

    if (!allow_windowing)
        g.NavWindowingTarget = NULL;

    if (g.NavWindowingTargetAnim && g.NavWindowingTarget == NULL)
    {
        g.NavWindowingHighlightAlpha = AzMax(g.NavWindowingHighlightAlpha
                                                     - io.DeltaTime * 10.0f,
                                             0.0f);
        if (g.DimBgRatio <= 0.0f && g.NavWindowingHighlightAlpha <= 0.0f)
            g.NavWindowingTargetAnim = NULL;
    }

    const AzID owner_id = AzHashStr("##NavUpdateWindowing");
    const bool nav_gamepad_active =
            (io.ConfigFlags & AzConfigFlags_NavEnableGamepad) != 0
            && (io.BackendFlags & AzBackendFlags_HasGamepad) != 0;
    const bool nav_keyboard_active = (io.ConfigFlags
                                      & AzConfigFlags_NavEnableKeyboard)
                                     != 0;
    const bool keyboard_next_window =
            allow_windowing && g.ConfigNavWindowingKeyNext
            && Shortcut(g.ConfigNavWindowingKeyNext,
                        AzInputFlags_Repeat | AzInputFlags_RouteAlways,
                        owner_id);
    const bool keyboard_prev_window =
            allow_windowing && g.ConfigNavWindowingKeyPrev
            && Shortcut(g.ConfigNavWindowingKeyPrev,
                        AzInputFlags_Repeat | AzInputFlags_RouteAlways,
                        owner_id);
    const bool start_windowing_with_gamepad =
            allow_windowing && nav_gamepad_active && !g.NavWindowingTarget
            && Shortcut(AzKey_NavGamepadMenu,
                        AzInputFlags_RouteAlways,
                        owner_id);
    const bool start_windowing_with_keyboard = allow_windowing
                                               && !g.NavWindowingTarget
                                               && (keyboard_next_window
                                                   || keyboard_prev_window);

    bool just_started_windowing_from_null_focus = false;
    if (start_windowing_with_gamepad || start_windowing_with_keyboard)
        if (AzWindow* window = g.NavWindow
                                       ? g.NavWindow
                                       : FindWindowNavFocusable(
                                                 g.WindowsFocusOrder.Size - 1,
                                                 -INT_MAX,
                                                 -1))
        {
            if (start_windowing_with_keyboard
                || g.ConfigNavWindowingWithGamepad)
                g.NavWindowingTarget = g.NavWindowingTargetAnim =
                        window->RootWindow;
            g.NavWindowingTimer = g.NavWindowingHighlightAlpha = 0.0f;
            g.NavWindowingAccumDeltaPos = g.NavWindowingAccumDeltaSize =
                    AzVec2(0.0f, 0.0f);
            g.NavWindowingToggleLayer = start_windowing_with_gamepad ? true
                                                                     : false;

            g.NavWindowingInputSource = g.NavInputSource =
                    start_windowing_with_keyboard ? AzInputSource_Keyboard
                                                  : AzInputSource_Gamepad;
            if (g.NavWindow == NULL)
                just_started_windowing_from_null_focus = true;

            if (keyboard_next_window || keyboard_prev_window)
                SetKeyOwnersForKeyChord((g.ConfigNavWindowingKeyNext
                                         | g.ConfigNavWindowingKeyPrev)
                                                & AzMod_Mask_,
                                        owner_id);
        }

    if ((g.NavWindowingTarget || g.NavWindowingToggleLayer)
        && g.NavWindowingInputSource == AzInputSource_Gamepad)
    {
        if (g.NavWindowingTarget != NULL)
        {
            g.NavWindowingTimer += io.DeltaTime;
            g.NavWindowingHighlightAlpha =
                    AzMax(g.NavWindowingHighlightAlpha,
                          AzSaturate((g.NavWindowingTimer
                                      - NAV_WINDOWING_HIGHLIGHT_DELAY)
                                     / 0.05f));

            const int focus_change_dir = (int)IsKeyPressed(AzKey_GamepadL1)
                                         - (int)IsKeyPressed(AzKey_GamepadR1);
            if (focus_change_dir != 0
                && !just_started_windowing_from_null_focus)
            {
                NavUpdateWindowingTarget(focus_change_dir);
                g.NavWindowingHighlightAlpha = 1.0f;
            }
        }

        if (!IsKeyDown(AzKey_NavGamepadMenu))
        {
            g.NavWindowingToggleLayer &= (g.NavWindowingHighlightAlpha < 1.0f);

            if (g.NavWindowingToggleLayer && g.NavWindow)
                apply_toggle_layer = true;
            else if (!g.NavWindowingToggleLayer)
                apply_focus_window = g.NavWindowingTarget;
            g.NavWindowingTarget = NULL;
            g.NavWindowingToggleLayer = false;
        }
    }

    if (g.NavWindowingTarget
        && g.NavWindowingInputSource == AzInputSource_Keyboard)
    {
        AzKeyChord shared_mods = ((g.ConfigNavWindowingKeyNext
                                           ? g.ConfigNavWindowingKeyNext
                                           : AzMod_Mask_)
                                  & (g.ConfigNavWindowingKeyPrev
                                             ? g.ConfigNavWindowingKeyPrev
                                             : AzMod_Mask_))
                                 & AzMod_Mask_;
        AZ_ASSERT(shared_mods != 0);

        g.NavWindowingTimer += io.DeltaTime;
        g.NavWindowingHighlightAlpha = AzMax(
                g.NavWindowingHighlightAlpha,
                AzSaturate((g.NavWindowingTimer - NAV_WINDOWING_HIGHLIGHT_DELAY)
                           / 0.05f));
        if ((keyboard_next_window || keyboard_prev_window)
            && !just_started_windowing_from_null_focus)
            NavUpdateWindowingTarget(keyboard_next_window ? -1 : +1);
        else if ((io.KeyMods & shared_mods) != shared_mods)
            apply_focus_window = g.NavWindowingTarget;
    }

    const AzKey windowing_toggle_keys[] = {AzKey_LeftAlt, AzKey_RightAlt};
    bool windowing_toggle_layer_start = false;
    if (g.NavWindow != NULL
        && !(g.NavWindow->Flags & AzWindowFlags_NoNavInputs))
        for (AzKey windowing_toggle_key : windowing_toggle_keys)
            if (nav_keyboard_active
                && IsKeyPressed(windowing_toggle_key, 0, AzKeyOwner_NoOwner))
            {
                windowing_toggle_layer_start = true;
                g.NavWindowingToggleLayer = true;
                g.NavWindowingToggleKey = windowing_toggle_key;
                g.NavWindowingInputSource = g.NavInputSource =
                        AzInputSource_Keyboard;
                break;
            }
    if (g.NavWindowingToggleLayer
        && g.NavWindowingInputSource == AzInputSource_Keyboard)
    {
        if (io.InputQueueCharacters.Size > 0 || io.KeyCtrl || io.KeyShift
            || io.KeySuper)
            g.NavWindowingToggleLayer = false;
        else if (windowing_toggle_layer_start == false
                 && g.LastKeyboardKeyPressTime == g.Time)
            g.NavWindowingToggleLayer = false;
        else if (TestKeyOwner(g.NavWindowingToggleKey, AzKeyOwner_NoOwner)
                         == false
                 || TestKeyOwner(AzMod_Alt, AzKeyOwner_NoOwner) == false)
            g.NavWindowingToggleLayer = false;

        if (IsKeyReleased(g.NavWindowingToggleKey) && g.NavWindowingToggleLayer)
            if (g.ActiveId == 0 || g.ActiveIdAllowOverlap)
                if (IsMousePosValid(&io.MousePos)
                    == IsMousePosValid(&io.MousePosPrev))
                    apply_toggle_layer = true;
        if (!IsKeyDown(g.NavWindowingToggleKey))
            g.NavWindowingToggleLayer = false;
    }

    if (g.NavWindowingTarget
        && !(g.NavWindowingTarget->Flags & AzWindowFlags_NoMove))
    {
        AzVec2 nav_move_dir;
        if (g.NavInputSource == AzInputSource_Keyboard && !io.KeyShift)
            nav_move_dir = GetKeyMagnitude2d(AzKey_LeftArrow,
                                             AzKey_RightArrow,
                                             AzKey_UpArrow,
                                             AzKey_DownArrow);
        if (g.NavInputSource == AzInputSource_Gamepad)
            nav_move_dir = GetKeyMagnitude2d(AzKey_GamepadLStickLeft,
                                             AzKey_GamepadLStickRight,
                                             AzKey_GamepadLStickUp,
                                             AzKey_GamepadLStickDown);
        if (nav_move_dir.x != 0.0f || nav_move_dir.y != 0.0f)
        {
            const float NAV_MOVE_SPEED = 800.0f;
            const float move_step = NAV_MOVE_SPEED * io.DeltaTime
                                    * AzMin(io.DisplayFramebufferScale.x,
                                            io.DisplayFramebufferScale.y);
            g.NavWindowingAccumDeltaPos += nav_move_dir * move_step;
            g.NavHighlightItemUnderNav = true;
            AzVec2 accum_floored = AzTrunc(g.NavWindowingAccumDeltaPos);
            if (accum_floored.x != 0.0f || accum_floored.y != 0.0f)
            {
                AzWindow* moving_window =
                        g.NavWindowingTarget->RootWindowDockTree;
                SetWindowPos(moving_window,
                             moving_window->Pos + accum_floored,
                             AzCond_Always);
                g.NavWindowingAccumDeltaPos -= accum_floored;
            }
        }
    }

    if (apply_focus_window)
        NavUpdateWindowingApplyFocus(apply_focus_window);

    if (apply_toggle_layer && g.NavWindow)
    {
        ClearActiveID();

        AzWindow* new_nav_window = g.NavWindow;
        while (new_nav_window->ParentWindow
               && (new_nav_window->DC.NavLayersActiveMask
                   & (1 << AzNavLayer_Menu))
                          == 0
               && (new_nav_window->Flags & AzWindowFlags_ChildWindow) != 0
               && (new_nav_window->Flags
                   & (AzWindowFlags_Popup | AzWindowFlags_ChildMenu))
                          == 0)
            new_nav_window = new_nav_window->ParentWindow;
        if (new_nav_window != g.NavWindow)
        {
            AzWindow* old_nav_window = g.NavWindow;
            FocusWindow(new_nav_window);
            new_nav_window->NavLastChildNavWindow = old_nav_window;
        }

        const AzNavLayer new_nav_layer = (g.NavWindow->DC.NavLayersActiveMask
                                          & (1 << AzNavLayer_Menu))
                                                 ? (AzNavLayer)((int)g.NavLayer
                                                                ^ 1)
                                                 : AzNavLayer_Main;
        if (new_nav_layer != g.NavLayer)
        {
            const bool preserve_layer_1_nav_id = (new_nav_window->DockNodeAsHost
                                                  != NULL);
            if (new_nav_layer == AzNavLayer_Menu && !preserve_layer_1_nav_id)
                g.NavWindow->NavLastIds[new_nav_layer] = 0;
            NavRestoreLayer(new_nav_layer);
            SetNavCursorVisibleAfterMove();
        }
    }
}

static const char* GetFallbackWindowNameForWindowingList(AzWindow* window)
{
    if (window->Flags & AzWindowFlags_Popup)
        return Az::LocalizeGetMsg(AzLocKey_WindowingPopup);
    if ((window->Flags & AzWindowFlags_MenuBar)
        && strcmp(window->Name, "##MainMenuBar") == 0)
        return Az::LocalizeGetMsg(AzLocKey_WindowingMainMenuBar);
    if (window->DockNodeAsHost)
        return "(Dock node)";
    return Az::LocalizeGetMsg(AzLocKey_WindowingUntitled);
}

void Az::NavUpdateWindowingOverlay()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.NavWindowingTarget != NULL);

    if (g.NavWindowingTimer < NAV_WINDOWING_LIST_APPEAR_DELAY)
        return;

    if (g.NavWindowingListWindow == NULL)
        g.NavWindowingListWindow = FindWindowByName("##NavWindowingOverlay");
    const AzViewport* viewport = GetMainViewport();
    SetNextWindowSizeConstraints(AzVec2(viewport->Size.x * 0.20f,
                                        viewport->Size.y * 0.20f),
                                 AzVec2(FLT_MAX, FLT_MAX));
    SetNextWindowPos(viewport->GetCenter(), AzCond_Always, AzVec2(0.5f, 0.5f));
    PushStyleVar(AzStyleVar_WindowPadding, g.Style.WindowPadding * 2.0f);
    Begin("##NavWindowingOverlay",
          NULL,
          AzWindowFlags_NoTitleBar | AzWindowFlags_NoFocusOnAppearing
                  | AzWindowFlags_NoResize | AzWindowFlags_NoMove
                  | AzWindowFlags_NoInputs | AzWindowFlags_AlwaysAutoResize
                  | AzWindowFlags_NoSavedSettings);
    if (g.ContextName[0] != 0)
        SeparatorText(g.ContextName);
    for (int n = g.WindowsFocusOrder.Size - 1; n >= 0; n--)
    {
        AzWindow* window = g.WindowsFocusOrder[n];
        AZ_ASSERT(window != NULL);
        if (!IsWindowNavFocusable(window))
            continue;
        const char* label = window->Name;
        if (label == FindRenderedTextEnd(label))
            label = GetFallbackWindowNameForWindowingList(window);
        Selectable(label, g.NavWindowingTarget == window);
    }
    End();
    PopStyleVar();
}

bool Az::IsDragDropActive()
{
    AzContext& g = *GAz;
    return g.DragDropActive;
}

void Az::ClearDragDrop()
{
    AzContext& g = *GAz;
    if (g.DragDropActive)
        AZ_DEBUG_LOG_ACTIVEID("[dragdrop] ClearDragDrop()\n");
    g.DragDropActive = false;
    g.DragDropPayload.Clear();
    g.DragDropAcceptFlags = AzDragDropFlags_None;
    g.DragDropAcceptIdCurr = g.DragDropAcceptIdPrev = 0;
    g.DragDropAcceptIdCurrRectSurface = FLT_MAX;
    g.DragDropAcceptFrameCount = -1;

    g.DragDropPayloadBufHeap.clear();
    memset(&g.DragDropPayloadBufLocal, 0, sizeof(g.DragDropPayloadBufLocal));
}

bool Az::BeginTooltipHidden()
{
    AzContext& g = *GAz;
    bool ret = Begin("##Tooltip_Hidden",
                     NULL,
                     AzWindowFlags_Tooltip | AzWindowFlags_NoInputs
                             | AzWindowFlags_NoTitleBar | AzWindowFlags_NoMove
                             | AzWindowFlags_NoResize
                             | AzWindowFlags_NoSavedSettings
                             | AzWindowFlags_AlwaysAutoResize);
    SetWindowHiddenAndSkipItemsForCurrentFrame(g.CurrentWindow);
    return ret;
}

bool Az::BeginDragDropSource(AzDragDropFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    AzMouseButton mouse_button = AzMouseButton_Left;

    bool source_drag_active = false;
    AzID source_id = 0;
    AzID source_parent_id = 0;
    if ((flags & AzDragDropFlags_SourceExtern) == 0)
    {
        source_id = g.LastItemData.ID;
        if (source_id != 0)
        {
            if (g.ActiveId != source_id)
                return false;
            if (g.ActiveIdMouseButton != -1)
                mouse_button = g.ActiveIdMouseButton;
            if (g.IO.MouseDown[mouse_button] == false || window->SkipItems)
                return false;
            g.ActiveIdAllowOverlap = false;
        } else
        {
            if (g.IO.MouseDown[mouse_button] == false || window->SkipItems)
                return false;
            if ((g.LastItemData.StatusFlags & AzItemStatusFlags_HoveredRect)
                        == 0
                && (g.ActiveId == 0 || g.ActiveIdWindow != window))
                return false;

            if (!(flags & AzDragDropFlags_SourceAllowNullID))
            {
                AZ_ASSERT(0);
                return false;
            }

            source_id = g.LastItemData.ID = window->GetIDFromRectangle(
                    g.LastItemData.Rect);
            KeepAliveID(source_id);
            bool is_hovered = ItemHoverable(g.LastItemData.Rect,
                                            source_id,
                                            g.LastItemData.ItemFlags);
            if (is_hovered && g.IO.MouseClicked[mouse_button])
            {
                SetActiveID(source_id, window);
                FocusWindow(window);
            }
            if (g.ActiveId == source_id)

                g.ActiveIdAllowOverlap = is_hovered;
        }
        if (g.ActiveId != source_id)
            return false;
        source_parent_id = window->IDStack.back();
        source_drag_active = IsMouseDragging(mouse_button);

        SetActiveIdUsingAllKeyboardKeys();
    } else
    {
        window = NULL;
        source_id = AzHashStr("#SourceExtern");
        source_drag_active = true;
        mouse_button = g.IO.MouseDown[0] ? 0 : -1;
        KeepAliveID(source_id);
        SetActiveID(source_id, NULL);
    }

    AZ_ASSERT(g.DragDropWithinTarget == false);

    if (!source_drag_active)
        return false;

    if (!g.DragDropActive)
    {
        AZ_ASSERT(source_id != 0);
        ClearDragDrop();
        AZ_DEBUG_LOG_ACTIVEID(
                "[dragdrop] BeginDragDropSource() DragDropActive = true, "
                "source_id = 0x%08X%s\n",
                source_id,
                (flags & AzDragDropFlags_SourceExtern) ? " (EXTERN)" : "");
        AzPayload& payload = g.DragDropPayload;
        payload.SourceId = source_id;
        payload.SourceParentId = source_parent_id;
        g.DragDropActive = true;
        g.DragDropSourceFlags = flags;
        g.DragDropMouseButton = mouse_button;
        if (payload.SourceId == g.ActiveId)
            g.ActiveIdNoClearOnFocusLoss = true;
    }
    g.DragDropSourceFrameCount = g.FrameCount;
    g.DragDropWithinSource = true;

    if (!(flags & AzDragDropFlags_SourceNoPreviewTooltip))
    {
        bool ret;
        if (g.DragDropAcceptIdPrev
            && (g.DragDropAcceptFlags & AzDragDropFlags_AcceptNoPreviewTooltip))
            ret = BeginTooltipHidden();
        else
            ret = BeginTooltip();
        AZ_ASSERT(ret);

        AZ_UNUSED(ret);
    }

    if (!(flags & AzDragDropFlags_SourceNoDisableHover)
        && !(flags & AzDragDropFlags_SourceExtern))
        g.LastItemData.StatusFlags &= ~AzItemStatusFlags_HoveredRect;

    return true;
}

void Az::EndDragDropSource()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.DragDropActive);
    AZ_ASSERT(g.DragDropWithinSource && "Not after a BeginDragDropSource()?");

    if (!(g.DragDropSourceFlags & AzDragDropFlags_SourceNoPreviewTooltip))
        EndTooltip();

    if (g.DragDropPayload.DataFrameCount == -1)
        ClearDragDrop();
    g.DragDropWithinSource = false;
}

bool Az::SetDragDropPayload(const char* type,
                            const void* data,
                            size_t data_size,
                            AzCond cond)
{
    AzContext& g = *GAz;
    AzPayload& payload = g.DragDropPayload;
    if (cond == 0)
        cond = AzCond_Always;

    AZ_ASSERT(type != NULL);
    AZ_ASSERT(AzStrlen(type) < AZ_ARRAYSIZE(payload.DataType)
              && "Payload type can be at most 32 characters long");
    AZ_ASSERT((data != NULL && data_size > 0)
              || (data == NULL && data_size == 0));
    AZ_ASSERT(cond == AzCond_Always || cond == AzCond_Once);
    AZ_ASSERT(payload.SourceId != 0);

    if (cond == AzCond_Always || payload.DataFrameCount == -1)
    {
        AzStrncpy(payload.DataType, type, AZ_ARRAYSIZE(payload.DataType));
        g.DragDropPayloadBufHeap.resize(0);
        if (data_size > sizeof(g.DragDropPayloadBufLocal))
        {
            g.DragDropPayloadBufHeap.resize((int)data_size);
            payload.Data = g.DragDropPayloadBufHeap.Data;
            memcpy(payload.Data, data, data_size);
        } else if (data_size > 0)
        {
            memset(&g.DragDropPayloadBufLocal,
                   0,
                   sizeof(g.DragDropPayloadBufLocal));
            payload.Data = g.DragDropPayloadBufLocal;
            memcpy(payload.Data, data, data_size);
        } else
        {
            payload.Data = NULL;
        }
        payload.DataSize = (int)data_size;
    }
    payload.DataFrameCount = g.FrameCount;

    return (g.DragDropAcceptFrameCount == g.FrameCount)
           || (g.DragDropAcceptFrameCount == g.FrameCount - 1);
}

bool Az::BeginDragDropTargetCustom(const AzRect& bb, AzID id)
{
    AzContext& g = *GAz;
    if (!g.DragDropActive)
        return false;

    AzWindow* window = g.CurrentWindow;
    AzWindow* hovered_window = g.HoveredWindowUnderMovingWindow;
    if (hovered_window == NULL
        || window->RootWindowDockTree != hovered_window->RootWindowDockTree)
        return false;
    AZ_ASSERT(id != 0);
    if (!IsMouseHoveringRect(bb.Min, bb.Max)
        || (id == g.DragDropPayload.SourceId))
        return false;
    if (window->SkipItems)
        return false;

    AZ_ASSERT(g.DragDropWithinTarget == false
              && g.DragDropWithinSource == false);

    g.DragDropTargetRect = bb;
    g.DragDropTargetClipRect = window->ClipRect;

    g.DragDropTargetId = id;
    g.DragDropWithinTarget = true;
    return true;
}

bool Az::BeginDragDropTarget()
{
    AzContext& g = *GAz;
    if (!g.DragDropActive)
        return false;

    AzWindow* window = g.CurrentWindow;
    if (!(g.LastItemData.StatusFlags & AzItemStatusFlags_HoveredRect))
        return false;
    AzWindow* hovered_window = g.HoveredWindowUnderMovingWindow;
    if (hovered_window == NULL
        || window->RootWindowDockTree != hovered_window->RootWindowDockTree
        || window->SkipItems)
        return false;

    const AzRect& display_rect = (g.LastItemData.StatusFlags
                                  & AzItemStatusFlags_HasDisplayRect)
                                         ? g.LastItemData.DisplayRect
                                         : g.LastItemData.Rect;
    AzID id = g.LastItemData.ID;
    if (id == 0)
    {
        id = window->GetIDFromRectangle(display_rect);
        KeepAliveID(id);
    }
    if (g.DragDropPayload.SourceId == id)
        return false;

    AZ_ASSERT(g.DragDropWithinTarget == false
              && g.DragDropWithinSource == false);

    g.DragDropTargetRect = display_rect;
    g.DragDropTargetClipRect = (g.LastItemData.StatusFlags
                                & AzItemStatusFlags_HasClipRect)
                                       ? g.LastItemData.ClipRect
                                       : window->ClipRect;
    g.DragDropTargetId = id;
    g.DragDropWithinTarget = true;
    return true;
}

bool Az::IsDragDropPayloadBeingAccepted()
{
    AzContext& g = *GAz;
    return g.DragDropActive && g.DragDropAcceptIdPrev != 0;
}

const AzPayload* Az::AcceptDragDropPayload(const char* type,
                                           AzDragDropFlags flags)
{
    AzContext& g = *GAz;
    AzPayload& payload = g.DragDropPayload;
    AZ_ASSERT(g.DragDropActive);

    AZ_ASSERT(payload.DataFrameCount != -1);
    if (type != NULL && !payload.IsDataType(type))
        return NULL;

    const bool was_accepted_previously = (g.DragDropAcceptIdPrev
                                          == g.DragDropTargetId);
    AzRect r = g.DragDropTargetRect;
    float r_surface = r.GetWidth() * r.GetHeight();
    if (r_surface > g.DragDropAcceptIdCurrRectSurface)
        return NULL;

    g.DragDropAcceptFlags = flags;
    g.DragDropAcceptIdCurr = g.DragDropTargetId;
    g.DragDropAcceptIdCurrRectSurface = r_surface;

    payload.Preview = was_accepted_previously;
    flags |= (g.DragDropSourceFlags & AzDragDropFlags_AcceptNoDrawDefaultRect);

    if (!(flags & AzDragDropFlags_AcceptNoDrawDefaultRect) && payload.Preview)
        RenderDragDropTargetRect(r, g.DragDropTargetClipRect);

    g.DragDropAcceptFrameCount = g.FrameCount;
    if ((g.DragDropSourceFlags & AzDragDropFlags_SourceExtern)
        && g.DragDropMouseButton == -1)
        payload.Delivery = was_accepted_previously
                           && (g.DragDropSourceFrameCount < g.FrameCount);
    else
        payload.Delivery = was_accepted_previously
                           && !IsMouseDown(g.DragDropMouseButton);

    if (!payload.Delivery && !(flags & AzDragDropFlags_AcceptBeforeDelivery))
        return NULL;

    if (payload.Delivery)
        AZ_DEBUG_LOG_ACTIVEID(
                "[dragdrop] AcceptDragDropPayload(): 0x%08X: payload "
                "delivery\n",
                g.DragDropTargetId);
    return &payload;
}

void Az::RenderDragDropTargetRect(const AzRect& bb,
                                  const AzRect& item_clip_rect)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzRect bb_display = bb;
    bb_display.ClipWith(item_clip_rect);

    bb_display.Expand(3.5f);
    bool push_clip_rect = !window->ClipRect.Contains(bb_display);
    if (push_clip_rect)
        window->DrawList->PushClipRectFullScreen();
    window->DrawList->AddRect(bb_display.Min,
                              bb_display.Max,
                              GetColorU32(AzCol_DragDropTarget),
                              0.0f,
                              0,
                              2.0f);
    if (push_clip_rect)
        window->DrawList->PopClipRect();
}

const AzPayload* Az::GetDragDropPayload()
{
    AzContext& g = *GAz;
    return (g.DragDropActive && g.DragDropPayload.DataFrameCount != -1)
                   ? &g.DragDropPayload
                   : NULL;
}

void Az::EndDragDropTarget()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.DragDropActive);
    AZ_ASSERT(g.DragDropWithinTarget);
    g.DragDropWithinTarget = false;

    if (g.DragDropPayload.Delivery)
        ClearDragDrop();
}

static inline void LogTextV(AzContext& g, const char* fmt, va_list args)
{
    if (g.LogFile)
    {
        g.LogBuffer.Buf.resize(0);
        g.LogBuffer.appendfv(fmt, args);
        AzFileWrite(g.LogBuffer.c_str(),
                    sizeof(char),
                    (AzU64)g.LogBuffer.size(),
                    g.LogFile);
    } else
    {
        g.LogBuffer.appendfv(fmt, args);
    }
}

void Az::LogText(const char* fmt, ...)
{
    AzContext& g = *GAz;
    if (!g.LogEnabled)
        return;

    va_list args;
    va_start(args, fmt);
    LogTextV(g, fmt, args);
    va_end(args);
}

void Az::LogTextV(const char* fmt, va_list args)
{
    AzContext& g = *GAz;
    if (!g.LogEnabled)
        return;

    LogTextV(g, fmt, args);
}

void Az::LogRenderedText(const AzVec2* ref_pos,
                         const char* text,
                         const char* text_end)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    const char* prefix = g.LogNextPrefix;
    const char* suffix = g.LogNextSuffix;
    g.LogNextPrefix = g.LogNextSuffix = NULL;

    if (!text_end)
        text_end = FindRenderedTextEnd(text, text_end);

    const bool log_new_line = ref_pos
                              && (ref_pos->y
                                  > g.LogLinePosY + g.Style.FramePadding.y + 1);
    if (ref_pos)
        g.LogLinePosY = ref_pos->y;
    if (log_new_line)
    {
        LogText(AZ_NEWLINE);
        g.LogLineFirstItem = true;
    }

    if (prefix)
        LogRenderedText(ref_pos, prefix, prefix + AzStrlen(prefix));

    if (g.LogDepthRef > window->DC.TreeDepth)
        g.LogDepthRef = window->DC.TreeDepth;
    const int tree_depth = (window->DC.TreeDepth - g.LogDepthRef);

    const char* text_remaining = text;
    for (;;)
    {
        const char* line_start = text_remaining;
        const char* line_end = AzStreolRange(line_start, text_end);
        const bool is_last_line = (line_end == text_end);
        if (line_start != line_end || !is_last_line)
        {
            const int line_length = (int)(line_end - line_start);
            const int indentation = g.LogLineFirstItem ? tree_depth * 4 : 1;
            LogText("%*s%.*s", indentation, "", line_length, line_start);
            g.LogLineFirstItem = false;
            if (*line_end == '\n')
            {
                LogText(AZ_NEWLINE);
                g.LogLineFirstItem = true;
            }
        }
        if (is_last_line)
            break;
        text_remaining = line_end + 1;
    }

    if (suffix)
        LogRenderedText(ref_pos, suffix, suffix + AzStrlen(suffix));
}

void Az::LogBegin(AzLogFlags flags, int auto_open_depth)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(g.LogEnabled == false);
    AZ_ASSERT(g.LogFile == NULL && g.LogBuffer.empty());
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzLogFlags_OutputMask_));

    g.LogEnabled = g.ItemUnclipByLog = true;
    g.LogFlags = flags;
    g.LogWindow = window;
    g.LogNextPrefix = g.LogNextSuffix = NULL;
    g.LogDepthRef = window->DC.TreeDepth;
    g.LogDepthToExpand = ((auto_open_depth >= 0) ? auto_open_depth
                                                 : g.LogDepthToExpandDefault);
    g.LogLinePosY = FLT_MAX;
    g.LogLineFirstItem = true;
}

void Az::LogSetNextTextDecoration(const char* prefix, const char* suffix)
{
    AzContext& g = *GAz;
    g.LogNextPrefix = prefix;
    g.LogNextSuffix = suffix;
}

void Az::LogToTTY(int auto_open_depth)
{
    AzContext& g = *GAz;
    if (g.LogEnabled)
        return;
    AZ_UNUSED(auto_open_depth);
#ifndef AZ_DISABLE_TTY_FUNCTIONS
    LogBegin(AzLogFlags_OutputTTY, auto_open_depth);
    g.LogFile = stdout;
#endif
}

void Az::LogToFile(int auto_open_depth, const char* filename)
{
    AzContext& g = *GAz;
    if (g.LogEnabled)
        return;

    if (!filename)
        filename = g.IO.LogFilename;
    if (!filename || !filename[0])
        return;
    AzFileHandle f = AzFileOpen(filename, "ab");
    if (!f)
    {
        AZ_ASSERT(0);
        return;
    }

    LogBegin(AzLogFlags_OutputFile, auto_open_depth);
    g.LogFile = f;
}

void Az::LogToClipboard(int auto_open_depth)
{
    AzContext& g = *GAz;
    if (g.LogEnabled)
        return;
    LogBegin(AzLogFlags_OutputClipboard, auto_open_depth);
}

void Az::LogToBuffer(int auto_open_depth)
{
    AzContext& g = *GAz;
    if (g.LogEnabled)
        return;
    LogBegin(AzLogFlags_OutputBuffer, auto_open_depth);
}

void Az::LogFinish()
{
    AzContext& g = *GAz;
    if (!g.LogEnabled)
        return;

    LogText(AZ_NEWLINE);
    switch (g.LogFlags & AzLogFlags_OutputMask_)
    {
    case AzLogFlags_OutputTTY:
#ifndef AZ_DISABLE_TTY_FUNCTIONS
        fflush(g.LogFile);
#endif
        break;
    case AzLogFlags_OutputFile:
        AzFileClose(g.LogFile);
        break;
    case AzLogFlags_OutputBuffer:
        break;
    case AzLogFlags_OutputClipboard:
        if (!g.LogBuffer.empty())
            SetClipboardText(g.LogBuffer.begin());
        break;
    default:
        AZ_ASSERT(0);
        break;
    }

    g.LogEnabled = g.ItemUnclipByLog = false;
    g.LogFlags = AzLogFlags_None;
    g.LogFile = NULL;
    g.LogBuffer.clear();
}

void Az::LogButtons()
{
    AzContext& g = *GAz;

    PushID("LogButtons");
#ifndef AZ_DISABLE_TTY_FUNCTIONS
    const bool log_to_tty = Button("Log To TTY");
    SameLine();
#else
    const bool log_to_tty = false;
#endif
    const bool log_to_file = Button("Log To File");
    SameLine();
    const bool log_to_clipboard = Button("Log To Clipboard");
    SameLine();
    PushItemFlag(AzItemFlags_NoTabStop, true);
    SetNextItemWidth(80.0f);
    SliderInt("Default Depth", &g.LogDepthToExpandDefault, 0, 9, NULL);
    PopItemFlag();
    PopID();

    if (log_to_tty)
        LogToTTY();
    if (log_to_file)
        LogToFile();
    if (log_to_clipboard)
        LogToClipboard();
}

void Az::UpdateSettings()
{
    AzContext& g = *GAz;
    if (!g.SettingsLoaded)
    {
        AZ_ASSERT(g.SettingsWindows.empty());
        if (g.IO.IniFilename)
            LoadIniSettingsFromDisk(g.IO.IniFilename);
        g.SettingsLoaded = true;
    }

    if (g.SettingsDirtyTimer > 0.0f)
    {
        g.SettingsDirtyTimer -= g.IO.DeltaTime;
        if (g.SettingsDirtyTimer <= 0.0f)
        {
            if (g.IO.IniFilename != NULL)
                SaveIniSettingsToDisk(g.IO.IniFilename);
            else
                g.IO.WantSaveIniSettings = true;

            g.SettingsDirtyTimer = 0.0f;
        }
    }
}

void Az::MarkIniSettingsDirty()
{
    AzContext& g = *GAz;
    if (g.SettingsDirtyTimer <= 0.0f)
        g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

void Az::MarkIniSettingsDirty(AzWindow* window)
{
    AzContext& g = *GAz;
    if (!(window->Flags & AzWindowFlags_NoSavedSettings))
        if (g.SettingsDirtyTimer <= 0.0f)
            g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

void Az::AddSettingsHandler(const AzSettingsHandler* handler)
{
    AzContext& g = *GAz;
    AZ_ASSERT(FindSettingsHandler(handler->TypeName) == NULL);
    g.SettingsHandlers.push_back(*handler);
}

void Az::RemoveSettingsHandler(const char* type_name)
{
    AzContext& g = *GAz;
    if (AzSettingsHandler* handler = FindSettingsHandler(type_name))
        g.SettingsHandlers.erase(handler);
}

AzSettingsHandler* Az::FindSettingsHandler(const char* type_name)
{
    AzContext& g = *GAz;
    const AzID type_hash = AzHashStr(type_name);
    for (AzSettingsHandler& handler : g.SettingsHandlers)
        if (handler.TypeHash == type_hash)
            return &handler;
    return NULL;
}

void Az::ClearIniSettings()
{
    AzContext& g = *GAz;
    g.SettingsIniData.clear();
    for (AzSettingsHandler& handler : g.SettingsHandlers)
        if (handler.ClearAllFn != NULL)
            handler.ClearAllFn(&g, &handler);
}

void Az::LoadIniSettingsFromDisk(const char* ini_filename)
{
    size_t file_data_size = 0;
    char* file_data = (char*)AzFileLoadToMemory(ini_filename,
                                                "rb",
                                                &file_data_size);
    if (!file_data)
        return;
    if (file_data_size > 0)
        LoadIniSettingsFromMemory(file_data, (size_t)file_data_size);
    AZ_FREE(file_data);
}

void Az::LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.Initialized);

    if (ini_size == 0)
        ini_size = AzStrlen(ini_data);
    g.SettingsIniData.Buf.resize((int)ini_size + 1);
    char* const buf = g.SettingsIniData.Buf.Data;
    char* const buf_end = buf + ini_size;
    memcpy(buf, ini_data, ini_size);
    buf_end[0] = 0;

    for (AzSettingsHandler& handler : g.SettingsHandlers)
        if (handler.ReadInitFn != NULL)
            handler.ReadInitFn(&g, &handler);

    void* entry_data = NULL;
    AzSettingsHandler* entry_handler = NULL;

    char* line_end = NULL;
    for (char* line = buf; line < buf_end; line = line_end + 1)
    {
        while (*line == '\n' || *line == '\r')
            line++;
        line_end = line;
        while (line_end < buf_end && *line_end != '\n' && *line_end != '\r')
            line_end++;
        line_end[0] = 0;
        if (line[0] == ';')
            continue;
        if (line[0] == '[' && line_end > line && line_end[-1] == ']')
        {
            line_end[-1] = 0;
            const char* name_end = line_end - 1;
            const char* type_start = line + 1;
            char* type_end = (char*)(void*)AzStrchrRange(type_start,
                                                         name_end,
                                                         ']');
            const char* name_start = type_end ? AzStrchrRange(type_end + 1,
                                                              name_end,
                                                              '[')
                                              : NULL;
            if (!type_end || !name_start)
                continue;
            *type_end = 0;
            name_start++;
            entry_handler = FindSettingsHandler(type_start);
            entry_data = entry_handler
                                 ? entry_handler->ReadOpenFn(&g,
                                                             entry_handler,
                                                             name_start)
                                 : NULL;
        } else if (entry_handler != NULL && entry_data != NULL)
        {
            entry_handler->ReadLineFn(&g, entry_handler, entry_data, line);
        }
    }
    g.SettingsLoaded = true;

    memcpy(buf, ini_data, ini_size);

    for (AzSettingsHandler& handler : g.SettingsHandlers)
        if (handler.ApplyAllFn != NULL)
            handler.ApplyAllFn(&g, &handler);
}

void Az::SaveIniSettingsToDisk(const char* ini_filename)
{
    AzContext& g = *GAz;
    g.SettingsDirtyTimer = 0.0f;
    if (!ini_filename)
        return;

    size_t ini_data_size = 0;
    const char* ini_data = SaveIniSettingsToMemory(&ini_data_size);
    AzFileHandle f = AzFileOpen(ini_filename, "wt");
    if (!f)
        return;
    AzFileWrite(ini_data, sizeof(char), ini_data_size, f);
    AzFileClose(f);
}

const char* Az::SaveIniSettingsToMemory(size_t* out_size)
{
    AzContext& g = *GAz;
    g.SettingsDirtyTimer = 0.0f;
    g.SettingsIniData.Buf.resize(0);
    g.SettingsIniData.Buf.push_back(0);
    for (AzSettingsHandler& handler : g.SettingsHandlers)
        handler.WriteAllFn(&g, &handler, &g.SettingsIniData);
    if (out_size)
        *out_size = (size_t)g.SettingsIniData.size();
    return g.SettingsIniData.c_str();
}

AzWindowSettings* Az::CreateNewWindowSettings(const char* name)
{
    AzContext& g = *GAz;

    if (g.IO.ConfigDebugIniSettings == false)
    {
        if (const char* p = strstr(name, "###"))
            name = p;
    }
    const size_t name_len = AzStrlen(name);

    const size_t chunk_size = sizeof(AzWindowSettings) + name_len + 1;
    AzWindowSettings* settings = g.SettingsWindows.alloc_chunk(chunk_size);
    AZ_PLACEMENT_NEW(settings) AzWindowSettings();
    settings->ID = AzHashStr(name, name_len);
    memcpy(settings->GetName(), name, name_len + 1);

    return settings;
}

AzWindowSettings* Az::FindWindowSettingsByID(AzID id)
{
    AzContext& g = *GAz;
    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
        if (settings->ID == id && !settings->WantDelete)
            return settings;
    return NULL;
}

AzWindowSettings* Az::FindWindowSettingsByWindow(AzWindow* window)
{
    AzContext& g = *GAz;
    if (window->SettingsOffset != -1)
        return g.SettingsWindows.ptr_from_offset(window->SettingsOffset);
    return FindWindowSettingsByID(window->ID);
}

void Az::ClearWindowSettings(const char* name)
{
    AzContext& g = *GAz;
    AzWindow* window = FindWindowByName(name);
    if (window != NULL)
    {
        window->Flags |= AzWindowFlags_NoSavedSettings;
        InitOrLoadWindowSettings(window, NULL);
        if (window->DockId != 0)
            DockContextProcessUndockWindow(&g, window, true);
    }
    if (AzWindowSettings* settings = window ? FindWindowSettingsByWindow(window)
                                            : FindWindowSettingsByID(
                                                      AzHashStr(name)))
        settings->WantDelete = true;
}

static void WindowSettingsHandler_ClearAll(AzContext* ctx, AzSettingsHandler*)
{
    AzContext& g = *ctx;
    for (AzWindow* window : g.Windows)
        window->SettingsOffset = -1;
    g.SettingsWindows.clear();
}

static void* WindowSettingsHandler_ReadOpen(AzContext*,
                                            AzSettingsHandler*,
                                            const char* name)
{
    AzID id = AzHashStr(name);
    AzWindowSettings* settings = Az::FindWindowSettingsByID(id);
    if (settings)
        *settings = AzWindowSettings();

    else
        settings = Az::CreateNewWindowSettings(name);
    settings->ID = id;
    settings->WantApply = true;
    return (void*)settings;
}

static void WindowSettingsHandler_ReadLine(AzContext*,
                                           AzSettingsHandler*,
                                           void* entry,
                                           const char* line)
{
    AzWindowSettings* settings = (AzWindowSettings*)entry;
    int x, y;
    int i;
    AzU32 u1;
    if (sscanf(line, "Pos=%i,%i", &x, &y) == 2)
    {
        settings->Pos = AzVec2ih((short)x, (short)y);
    } else if (sscanf(line, "Size=%i,%i", &x, &y) == 2)
    {
        settings->Size = AzVec2ih((short)x, (short)y);
    } else if (sscanf(line, "ViewportId=0x%08X", &u1) == 1)
    {
        settings->ViewportId = u1;
    } else if (sscanf(line, "ViewportPos=%i,%i", &x, &y) == 2)
    {
        settings->ViewportPos = AzVec2ih((short)x, (short)y);
    } else if (sscanf(line, "Collapsed=%d", &i) == 1)
    {
        settings->Collapsed = (i != 0);
    } else if (sscanf(line, "IsChild=%d", &i) == 1)
    {
        settings->IsChild = (i != 0);
    } else if (sscanf(line, "DockId=0x%X,%d", &u1, &i) == 2)
    {
        settings->DockId = u1;
        settings->DockOrder = (short)i;
    } else if (sscanf(line, "DockId=0x%X", &u1) == 1)
    {
        settings->DockId = u1;
        settings->DockOrder = -1;
    } else if (sscanf(line, "ClassId=0x%X", &u1) == 1)
    {
        settings->ClassId = u1;
    }
}

static void WindowSettingsHandler_ApplyAll(AzContext* ctx, AzSettingsHandler*)
{
    AzContext& g = *ctx;
    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
        if (settings->WantApply)
        {
            if (AzWindow* window = Az::FindWindowByID(settings->ID))
                ApplyWindowSettings(window, settings);
            settings->WantApply = false;
        }
}

static void WindowSettingsHandler_WriteAll(AzContext* ctx,
                                           AzSettingsHandler* handler,
                                           AzTextBuffer* buf)
{
    AzContext& g = *ctx;
    for (AzWindow* window : g.Windows)
    {
        if (window->Flags & AzWindowFlags_NoSavedSettings)
            continue;

        AzWindowSettings* settings = Az::FindWindowSettingsByWindow(window);
        if (!settings)
        {
            settings = Az::CreateNewWindowSettings(window->Name);
            window->SettingsOffset = g.SettingsWindows.offset_from_ptr(
                    settings);
        }
        AZ_ASSERT(settings->ID == window->ID);
        settings->Pos = AzVec2ih(window->Pos - window->ViewportPos);
        settings->Size = AzVec2ih(window->SizeFull);
        settings->ViewportId = window->ViewportId;
        settings->ViewportPos = AzVec2ih(window->ViewportPos);
        AZ_ASSERT(window->DockNode == NULL
                  || window->DockNode->ID == window->DockId);
        settings->DockId = window->DockId;
        settings->ClassId = window->WindowClass.ClassId;
        settings->DockOrder = window->DockOrder;
        settings->Collapsed = window->Collapsed;
        settings->IsChild = (window->RootWindow != window);

        settings->WantDelete = false;
    }

    buf->reserve(buf->size() + g.SettingsWindows.size() * 6);
    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
    {
        if (settings->WantDelete)
            continue;
        const char* settings_name = settings->GetName();
        buf->appendf("[%s][%s]\n", handler->TypeName, settings_name);
        if (settings->IsChild)
        {
            buf->appendf("IsChild=1\n");
            buf->appendf("Size=%d,%d\n", settings->Size.x, settings->Size.y);
        } else
        {
            if (settings->ViewportId != 0
                && settings->ViewportId != Az::AZ_VIEWPORT_DEFAULT_ID)
            {
                buf->appendf("ViewportPos=%d,%d\n",
                             settings->ViewportPos.x,
                             settings->ViewportPos.y);
                buf->appendf("ViewportId=0x%08X\n", settings->ViewportId);
            }
            if (settings->Pos.x != 0 || settings->Pos.y != 0
                || settings->ViewportId == Az::AZ_VIEWPORT_DEFAULT_ID)
                buf->appendf("Pos=%d,%d\n", settings->Pos.x, settings->Pos.y);
            if (settings->Size.x != 0 || settings->Size.y != 0)
                buf->appendf("Size=%d,%d\n",
                             settings->Size.x,
                             settings->Size.y);
            buf->appendf("Collapsed=%d\n", settings->Collapsed);
            if (settings->DockId != 0)
            {
                if (settings->DockOrder == -1)
                    buf->appendf("DockId=0x%08X\n", settings->DockId);
                else
                    buf->appendf("DockId=0x%08X,%d\n",
                                 settings->DockId,
                                 settings->DockOrder);
                if (settings->ClassId != 0)
                    buf->appendf("ClassId=0x%08X\n", settings->ClassId);
            }
        }
        buf->append("\n");
    }
}

void Az::LocalizeRegisterEntries(const AzLocEntry* entries, int count)
{
    AzContext& g = *GAz;
    for (int n = 0; n < count; n++)
        g.LocalizationTable[entries[n].Key] = entries[n].Text;
}

AzViewport* Az::GetMainViewport()
{
    AzContext& g = *GAz;
    return g.Viewports[0];
}

AzViewport* Az::FindViewportByID(AzID id)
{
    AzContext& g = *GAz;
    for (AzViewportP* viewport : g.Viewports)
        if (viewport->ID == id)
            return viewport;
    return NULL;
}

AzViewport* Az::FindViewportByPlatformHandle(void* platform_handle)
{
    AzContext& g = *GAz;
    for (AzViewportP* viewport : g.Viewports)
        if (viewport->PlatformHandle == platform_handle)
            return viewport;
    return NULL;
}

void Az::SetCurrentViewport(AzWindow* current_window, AzViewportP* viewport)
{
    AzContext& g = *GAz;
    (void)current_window;

    if (viewport)
        viewport->LastFrameActive = g.FrameCount;
    if (g.CurrentViewport == viewport)
        return;
    g.CurrentDpiScale = viewport ? viewport->DpiScale : 1.0f;
    g.CurrentViewport = viewport;
    AZ_ASSERT(g.CurrentDpiScale > 0.0f && g.CurrentDpiScale < 99.0f);

    if (g.CurrentViewport && g.PlatformIO.Platform_OnChangedViewport)
        g.PlatformIO.Platform_OnChangedViewport(g.CurrentViewport);
}

void Az::SetWindowViewport(AzWindow* window, AzViewportP* viewport)
{
    if (window->ViewportOwned && window->Viewport->Window == window)
        window->Viewport->Size = AzVec2(0.0f, 0.0f);

    window->Viewport = viewport;
    window->ViewportId = viewport->ID;
    window->ViewportOwned = (viewport->Window == window);
}

static bool Az::GetWindowAlwaysWantOwnViewport(AzWindow* window)
{
    AzContext& g = *GAz;
    if (g.IO.ConfigViewportsNoAutoMerge
        || (window->WindowClass.ViewportFlagsOverrideSet
            & AzViewportFlags_NoAutoMerge))
        if (g.ConfigFlagsCurrFrame & AzConfigFlags_ViewportsEnable)
            if (!window->DockIsActive)
                if ((window->Flags
                     & (AzWindowFlags_ChildWindow | AzWindowFlags_ChildMenu
                        | AzWindowFlags_Tooltip))
                    == 0)
                    if ((window->Flags & AzWindowFlags_Popup) == 0
                        || (window->Flags & AzWindowFlags_Modal) != 0)
                        return true;
    return false;
}

static bool Az::UpdateTryMergeWindowIntoHostViewport(AzWindow* window,
                                                     AzViewportP* viewport)
{
    AzContext& g = *GAz;
    if (window->Viewport == viewport)
        return false;
    if ((viewport->Flags & AzViewportFlags_CanHostOtherWindows) == 0)
        return false;
    if ((viewport->Flags & AzViewportFlags_IsMinimized) != 0)
        return false;
    if (!viewport->GetMainRect().Contains(window->Rect()))
        return false;
    if (GetWindowAlwaysWantOwnViewport(window))
        return false;

    for (AzWindow* window_behind : g.Windows)
    {
        if (window_behind == window)
            break;
        if (window_behind->WasActive && window_behind->ViewportOwned
            && !(window_behind->Flags & AzWindowFlags_ChildWindow))
            if (window_behind->Viewport->GetMainRect().Overlaps(window->Rect()))
                return false;
    }

    AzViewportP* old_viewport = window->Viewport;
    if (window->ViewportOwned)
        for (int n = 0; n < g.Windows.Size; n++)
            if (g.Windows[n]->Viewport == old_viewport)
                SetWindowViewport(g.Windows[n], viewport);
    SetWindowViewport(window, viewport);
    BringWindowToDisplayFront(window);

    return true;
}

static bool Az::UpdateTryMergeWindowIntoHostViewports(AzWindow* window)
{
    AzContext& g = *GAz;
    return UpdateTryMergeWindowIntoHostViewport(window, g.Viewports[0]);
}

void Az::TranslateWindowsInViewport(AzViewportP* viewport,
                                    const AzVec2& old_pos,
                                    const AzVec2& new_pos,
                                    const AzVec2& old_size,
                                    const AzVec2& new_size)
{
    AzContext& g = *GAz;

    AZ_ASSERT(viewport->Window == NULL
              && (viewport->Flags & AzViewportFlags_CanHostOtherWindows));

    const bool translate_all_windows = (g.ConfigFlagsCurrFrame
                                        & AzConfigFlags_ViewportsEnable)
                                       != (g.ConfigFlagsLastFrame
                                           & AzConfigFlags_ViewportsEnable);
    AzRect test_still_fit_rect(old_pos, old_pos + old_size);
    AzVec2 delta_pos = new_pos - old_pos;
    for (AzWindow* window : g.Windows)
        if (translate_all_windows
            || (window->Viewport == viewport
                && (old_size == new_size
                    || test_still_fit_rect.Contains(window->Rect()))))
            TranslateWindow(window, delta_pos);
}

void Az::ScaleWindowsInViewport(AzViewportP* viewport, float scale)
{
    AzContext& g = *GAz;

    if (viewport->Window)
    {
        ScaleWindow(viewport->Window, scale);
    } else
    {
        for (AzWindow* window : g.Windows)
            if (window->Viewport == viewport)
                ScaleWindow(window, scale);
    }
}

AzViewportP* Az::FindHoveredViewportFromPlatformWindowStack(
        const AzVec2& mouse_platform_pos)
{
    AzContext& g = *GAz;
    AzViewportP* best_candidate = NULL;
    for (AzViewportP* viewport : g.Viewports)
        if (!(viewport->Flags
              & (AzViewportFlags_NoInputs | AzViewportFlags_IsMinimized))
            && viewport->GetMainRect().Contains(mouse_platform_pos))
            if (best_candidate == NULL
                || best_candidate->LastFocusedStampCount
                           < viewport->LastFocusedStampCount)
                best_candidate = viewport;
    return best_candidate;
}

static void Az::UpdateViewportsNewFrame()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.PlatformIO.Viewports.Size <= g.Viewports.Size);

    const bool viewports_enabled = (g.ConfigFlagsCurrFrame
                                    & AzConfigFlags_ViewportsEnable)
                                   != 0;
    if (viewports_enabled)
    {
        AzViewportP* focused_viewport = NULL;
        for (AzViewportP* viewport : g.Viewports)
        {
            const bool platform_funcs_available =
                    viewport->PlatformWindowCreated;
            if (g.PlatformIO.Platform_GetWindowMinimized
                && platform_funcs_available)
            {
                bool is_minimized = g.PlatformIO.Platform_GetWindowMinimized(
                        viewport);
                if (is_minimized)
                    viewport->Flags |= AzViewportFlags_IsMinimized;
                else
                    viewport->Flags &= ~AzViewportFlags_IsMinimized;
            }

            if (g.PlatformIO.Platform_GetWindowFocus
                && platform_funcs_available)
            {
                bool is_focused = g.PlatformIO.Platform_GetWindowFocus(
                        viewport);
                if (is_focused)
                    viewport->Flags |= AzViewportFlags_IsFocused;
                else
                    viewport->Flags &= ~AzViewportFlags_IsFocused;
                if (is_focused)
                    focused_viewport = viewport;
            }
        }

        if (focused_viewport
            && g.PlatformLastFocusedViewportId != focused_viewport->ID)
        {
            AZ_DEBUG_LOG_VIEWPORT(
                    "[viewport] Focused viewport changed %08X -> %08X '%s', "
                    "attempting to apply our focus.\n",
                    g.PlatformLastFocusedViewportId,
                    focused_viewport->ID,
                    focused_viewport->Window ? focused_viewport->Window->Name
                                             : "n/a");
            const AzViewport* prev_focused_viewport = FindViewportByID(
                    g.PlatformLastFocusedViewportId);
            const bool prev_focused_has_been_destroyed =
                    (prev_focused_viewport == NULL)
                    || (prev_focused_viewport->PlatformWindowCreated == false);

            if (focused_viewport->LastFocusedStampCount
                != g.ViewportFocusedStampCount)
                focused_viewport->LastFocusedStampCount =
                        ++g.ViewportFocusedStampCount;
            g.PlatformLastFocusedViewportId = focused_viewport->ID;

            const bool apply_imgui_focus_on_focused_viewport =
                    !IsAnyMouseDown() && !prev_focused_has_been_destroyed;
            if (apply_imgui_focus_on_focused_viewport)
            {
                focused_viewport->LastFocusedHadNavWindow |=
                        (g.NavWindow != NULL)
                        && (g.NavWindow->Viewport == focused_viewport);

                AzFocusRequestFlags focus_request_flags =
                        AzFocusRequestFlags_UnlessBelowModal
                        | AzFocusRequestFlags_RestoreFocusedChild;
                if (focused_viewport->Window != NULL)
                    FocusWindow(focused_viewport->Window, focus_request_flags);
                else if (focused_viewport->LastFocusedHadNavWindow)
                    FocusTopMostWindowUnderOne(NULL,
                                               NULL,
                                               focused_viewport,
                                               focus_request_flags);
                else
                    FocusWindow(NULL, focus_request_flags);
            }
        }
        if (focused_viewport)
            focused_viewport->LastFocusedHadNavWindow =
                    (g.NavWindow != NULL)
                    && (g.NavWindow->Viewport == focused_viewport);
    }

    AzViewportP* main_viewport = g.Viewports[0];
    AZ_ASSERT(main_viewport->ID == AZ_VIEWPORT_DEFAULT_ID);
    AZ_ASSERT(main_viewport->Window == NULL);
    AzVec2 main_viewport_pos = viewports_enabled
                                       ? g.PlatformIO.Platform_GetWindowPos(
                                                 main_viewport)
                                       : AzVec2(0.0f, 0.0f);
    AzVec2 main_viewport_size = g.IO.DisplaySize;
    AzVec2 main_viewport_framebuffer_scale = g.IO.DisplayFramebufferScale;
    if (viewports_enabled
        && (main_viewport->Flags & AzViewportFlags_IsMinimized))
    {
        main_viewport_pos = main_viewport->Pos;

        main_viewport_size = main_viewport->Size;
        main_viewport_framebuffer_scale = main_viewport->FramebufferScale;
    }
    AddUpdateViewport(NULL,
                      AZ_VIEWPORT_DEFAULT_ID,
                      main_viewport_pos,
                      main_viewport_size,
                      AzViewportFlags_OwnedByApp
                              | AzViewportFlags_CanHostOtherWindows);

    g.CurrentDpiScale = 0.0f;
    g.CurrentViewport = NULL;
    g.MouseViewport = NULL;
    for (int n = 0; n < g.Viewports.Size; n++)
    {
        AzViewportP* viewport = g.Viewports[n];
        viewport->Idx = n;

        if (n > 0 && viewport->LastFrameActive < g.FrameCount - 2)
        {
            DestroyViewport(viewport);
            n--;
            continue;
        }

        const bool platform_funcs_available = viewport->PlatformWindowCreated;
        if (viewports_enabled)
        {
            if (!(viewport->Flags & AzViewportFlags_IsMinimized)
                && platform_funcs_available)
            {
                if (viewport->PlatformRequestMove)
                    viewport->Pos = viewport->LastPlatformPos =
                            g.PlatformIO.Platform_GetWindowPos(viewport);
                if (viewport->PlatformRequestResize)
                    viewport->Size = viewport->LastPlatformSize =
                            g.PlatformIO.Platform_GetWindowSize(viewport);
                if (g.PlatformIO.Platform_GetWindowFramebufferScale != NULL)
                    viewport->FramebufferScale =
                            g.PlatformIO.Platform_GetWindowFramebufferScale(
                                    viewport);
            }
        }

        UpdateViewportPlatformMonitor(viewport);

        viewport->WorkInsetMin = viewport->BuildWorkInsetMin;
        viewport->WorkInsetMax = viewport->BuildWorkInsetMax;
        viewport->BuildWorkInsetMin = viewport->BuildWorkInsetMax =
                AzVec2(0.0f, 0.0f);
        if (g.PlatformIO.Platform_GetWindowWorkAreaInsets != NULL
            && platform_funcs_available)
        {
            AzVec4 insets = g.PlatformIO.Platform_GetWindowWorkAreaInsets(
                    viewport);
            AZ_ASSERT(insets.x >= 0.0f && insets.y >= 0.0f && insets.z >= 0.0f
                      && insets.w >= 0.0f);
            viewport->BuildWorkInsetMin = AzVec2(insets.x, insets.y);
            viewport->BuildWorkInsetMax = AzVec2(insets.z, insets.w);
        }
        viewport->UpdateWorkRect();

        viewport->Alpha = 1.0f;

        const AzVec2 viewport_delta_pos = viewport->Pos - viewport->LastPos;
        if ((viewport->Flags & AzViewportFlags_CanHostOtherWindows)
            && (viewport_delta_pos.x != 0.0f || viewport_delta_pos.y != 0.0f))
            TranslateWindowsInViewport(viewport,
                                       viewport->LastPos,
                                       viewport->Pos,
                                       viewport->LastSize,
                                       viewport->Size);

        float new_dpi_scale;
        if (g.PlatformIO.Platform_GetWindowDpiScale && platform_funcs_available)
            new_dpi_scale = g.PlatformIO.Platform_GetWindowDpiScale(viewport);
        else if (viewport->PlatformMonitor != -1)
            new_dpi_scale =
                    g.PlatformIO.Monitors[viewport->PlatformMonitor].DpiScale;
        else
            new_dpi_scale = (viewport->DpiScale != 0.0f) ? viewport->DpiScale
                                                         : 1.0f;
        AZ_ASSERT(new_dpi_scale > 0.0f && new_dpi_scale < 99.0f);

        if (viewport->DpiScale != 0.0f && new_dpi_scale != viewport->DpiScale)
        {
            float scale_factor = new_dpi_scale / viewport->DpiScale;
            if (g.IO.ConfigDpiScaleViewports)
                ScaleWindowsInViewport(viewport, scale_factor);
        }
        viewport->DpiScale = new_dpi_scale;
    }

    g.PlatformMonitorsFullWorkRect =
            AzRect(+FLT_MAX, +FLT_MAX, -FLT_MAX, -FLT_MAX);
    if (g.PlatformIO.Monitors.Size == 0)
    {
        AzPlatformMonitor* monitor = &g.FallbackMonitor;
        monitor->MainPos = main_viewport->Pos;
        monitor->MainSize = main_viewport->Size;
        monitor->WorkPos = main_viewport->WorkPos;
        monitor->WorkSize = main_viewport->WorkSize;
        monitor->DpiScale = main_viewport->DpiScale;
        g.PlatformMonitorsFullWorkRect.Add(monitor->WorkPos);
        g.PlatformMonitorsFullWorkRect.Add(monitor->WorkPos
                                           + monitor->WorkSize);
    } else
    {
        g.FallbackMonitor = g.PlatformIO.Monitors[0];
    }
    for (AzPlatformMonitor& monitor : g.PlatformIO.Monitors)
    {
        g.PlatformMonitorsFullWorkRect.Add(monitor.WorkPos);
        g.PlatformMonitorsFullWorkRect.Add(monitor.WorkPos + monitor.WorkSize);
    }

    if (!viewports_enabled)
    {
        g.MouseViewport = main_viewport;
        return;
    }

    AzViewportP* viewport_hovered = NULL;
    if (g.IO.BackendFlags & AzBackendFlags_HasMouseHoveredViewport)
    {
        viewport_hovered = g.IO.MouseHoveredViewport
                                   ? (AzViewportP*)FindViewportByID(
                                             g.IO.MouseHoveredViewport)
                                   : NULL;
        if (viewport_hovered
            && (viewport_hovered->Flags & AzViewportFlags_NoInputs))
            viewport_hovered = FindHoveredViewportFromPlatformWindowStack(
                    g.IO.MousePos);

    } else
    {
        viewport_hovered = FindHoveredViewportFromPlatformWindowStack(
                g.IO.MousePos);
    }
    if (viewport_hovered != NULL)
        g.MouseLastHoveredViewport = viewport_hovered;
    else if (g.MouseLastHoveredViewport == NULL)
        g.MouseLastHoveredViewport = g.Viewports[0];

    if (g.MovingWindow && g.MovingWindow->Viewport)
        g.MouseViewport = g.MovingWindow->Viewport;
    else
        g.MouseViewport = g.MouseLastHoveredViewport;

    const bool is_mouse_dragging_with_an_expected_destination =
            g.DragDropActive;
    if (is_mouse_dragging_with_an_expected_destination
        && viewport_hovered == NULL)
        viewport_hovered = g.MouseLastHoveredViewport;
    if (is_mouse_dragging_with_an_expected_destination || g.ActiveId == 0
        || !IsAnyMouseDown())
        if (viewport_hovered != NULL && viewport_hovered != g.MouseViewport
            && !(viewport_hovered->Flags & AzViewportFlags_NoInputs))
            g.MouseViewport = viewport_hovered;

    AZ_ASSERT(g.MouseViewport != NULL);
}

static void Az::UpdateViewportsEndFrame()
{
    AzContext& g = *GAz;
    g.PlatformIO.Viewports.resize(0);
    for (int i = 0; i < g.Viewports.Size; i++)
    {
        AzViewportP* viewport = g.Viewports[i];
        viewport->LastPos = viewport->Pos;
        viewport->LastSize = viewport->Size;
        if (viewport->LastFrameActive < g.FrameCount || viewport->Size.x <= 0.0f
            || viewport->Size.y <= 0.0f)
            if (i > 0)
                continue;
        if (viewport->Window && !IsWindowActiveAndVisible(viewport->Window))
            continue;
        if (i > 0)
            AZ_ASSERT(viewport->Window != NULL);
        g.PlatformIO.Viewports.push_back(viewport);
    }
    g.Viewports[0]->ClearRequestFlags();
}

AzViewportP* Az::AddUpdateViewport(AzWindow* window,
                                   AzID id,
                                   const AzVec2& pos,
                                   const AzVec2& size,
                                   AzViewportFlags flags)
{
    AzContext& g = *GAz;
    AZ_ASSERT(id != 0);

    flags |= AzViewportFlags_IsPlatformWindow;
    if (window != NULL)
    {
        if (g.MovingWindow && g.MovingWindow->RootWindowDockTree == window)
            flags |= AzViewportFlags_NoInputs
                     | AzViewportFlags_NoFocusOnAppearing;
        if ((window->Flags & AzWindowFlags_NoMouseInputs)
            && (window->Flags & AzWindowFlags_NoNavInputs))
            flags |= AzViewportFlags_NoInputs;
        if (window->Flags & AzWindowFlags_NoFocusOnAppearing)
            flags |= AzViewportFlags_NoFocusOnAppearing;
    }

    AzViewportP* viewport = (AzViewportP*)FindViewportByID(id);
    if (viewport)
    {
        AzVec2 prev_pos = viewport->Pos;
        AzVec2 prev_size = viewport->Size;
        if (!viewport->PlatformRequestMove
            || viewport->ID == AZ_VIEWPORT_DEFAULT_ID)
            viewport->Pos = pos;
        if (!viewport->PlatformRequestResize
            || viewport->ID == AZ_VIEWPORT_DEFAULT_ID)
            viewport->Size = size;
        viewport->Flags = flags
                          | (viewport->Flags
                             & (AzViewportFlags_IsMinimized
                                | AzViewportFlags_IsFocused));

        if (prev_pos != viewport->Pos || prev_size != viewport->Size)
            UpdateViewportPlatformMonitor(viewport);
    } else
    {
        viewport = AZ_NEW(AzViewportP)();
        viewport->ID = id;
        viewport->Idx = g.Viewports.Size;
        viewport->Pos = viewport->LastPos = pos;
        viewport->Size = viewport->LastSize = size;
        viewport->Flags = flags;
        UpdateViewportPlatformMonitor(viewport);
        g.Viewports.push_back(viewport);
        g.ViewportCreatedCount++;
        AZ_DEBUG_LOG_VIEWPORT("[viewport] Add Viewport %08X '%s'\n",
                              id,
                              window ? window->Name : "<NULL>");

        g.DrawListSharedData.ClipRectFullscreen.x =
                AzMin(g.DrawListSharedData.ClipRectFullscreen.x,
                      viewport->Pos.x);
        g.DrawListSharedData.ClipRectFullscreen.y =
                AzMin(g.DrawListSharedData.ClipRectFullscreen.y,
                      viewport->Pos.y);
        g.DrawListSharedData.ClipRectFullscreen.z =
                AzMax(g.DrawListSharedData.ClipRectFullscreen.z,
                      viewport->Pos.x + viewport->Size.x);
        g.DrawListSharedData.ClipRectFullscreen.w =
                AzMax(g.DrawListSharedData.ClipRectFullscreen.w,
                      viewport->Pos.y + viewport->Size.y);

        viewport->DpiScale = GetViewportPlatformMonitor(viewport)->DpiScale;
    }

    viewport->Window = window;
    viewport->LastFrameActive = g.FrameCount;
    viewport->UpdateWorkRect();
    AZ_ASSERT(window == NULL || viewport->ID == window->ID);

    if (window != NULL)
        window->ViewportOwned = true;

    return viewport;
}

static void Az::DestroyViewport(AzViewportP* viewport)
{
    AzContext& g = *GAz;
    for (AzWindow* window : g.Windows)
    {
        if (window->Viewport != viewport)
            continue;
        window->Viewport = NULL;
        window->ViewportOwned = false;
    }
    if (viewport == g.MouseLastHoveredViewport)
        g.MouseLastHoveredViewport = NULL;

    AZ_DEBUG_LOG_VIEWPORT("[viewport] Delete Viewport %08X '%s'\n",
                          viewport->ID,
                          viewport->Window ? viewport->Window->Name : "n/a");
    DestroyPlatformWindow(viewport);

    AZ_ASSERT(g.PlatformIO.Viewports.contains(viewport) == false);
    AZ_ASSERT(g.Viewports[viewport->Idx] == viewport);
    g.Viewports.erase(g.Viewports.Data + viewport->Idx);
    AZ_DELETE(viewport);
}

static void Az::WindowSelectViewport(AzWindow* window)
{
    AzContext& g = *GAz;
    AzWindowFlags flags = window->Flags;
    window->ViewportAllowPlatformMonitorExtend = -1;

    AzViewportP* main_viewport = (AzViewportP*)(void*)GetMainViewport();
    if (!(g.ConfigFlagsCurrFrame & AzConfigFlags_ViewportsEnable))
    {
        SetWindowViewport(window, main_viewport);
        return;
    }
    window->ViewportOwned = false;

    if ((flags & (AzWindowFlags_Popup | AzWindowFlags_Tooltip))
        && window->Appearing)
    {
        window->Viewport = NULL;
        window->ViewportId = 0;
    }

    if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasViewport) == 0)
    {
        if (window->Viewport == NULL && window->ParentWindow
            && (!window->ParentWindow->IsFallbackWindow
                || window->ParentWindow->WasActive))
            window->Viewport = window->ParentWindow->Viewport;

        if (window->Viewport == NULL && window->ViewportId != 0)
        {
            window->Viewport = (AzViewportP*)FindViewportByID(
                    window->ViewportId);
            if (window->Viewport == NULL && window->ViewportPos.x != FLT_MAX
                && window->ViewportPos.y != FLT_MAX)
                window->Viewport = AddUpdateViewport(window,
                                                     window->ID,
                                                     window->ViewportPos,
                                                     window->Size,
                                                     AzViewportFlags_None);
        }
    }

    bool lock_viewport = false;
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasViewport)
    {
        window->Viewport = (AzViewportP*)FindViewportByID(
                g.NextWindowData.ViewportId);
        window->ViewportId = g.NextWindowData.ViewportId;

        if (window->Viewport
            && (window->Flags & AzWindowFlags_DockNodeHost) != 0
            && window->Viewport->Window != NULL)
        {
            window->Viewport->Window = window;
            window->Viewport->ID = window->ViewportId = window->ID;
        }
        lock_viewport = true;
    } else if ((flags & AzWindowFlags_ChildWindow)
               || (flags & AzWindowFlags_ChildMenu))
    {
        if (window->DockNode && window->DockNode->HostWindow)
            AZ_ASSERT(window->DockNode->HostWindow->Viewport
                      == window->ParentWindow->Viewport);
        window->Viewport = window->ParentWindow->Viewport;
    } else if (window->DockNode && window->DockNode->HostWindow)
    {
        window->Viewport = window->DockNode->HostWindow->Viewport;
    } else if (flags & AzWindowFlags_Tooltip)
    {
        window->Viewport = g.MouseViewport;
    } else if (GetWindowAlwaysWantOwnViewport(window))
    {
        window->Viewport = AddUpdateViewport(window,
                                             window->ID,
                                             window->Pos,
                                             window->Size,
                                             AzViewportFlags_None);
    } else if (g.MovingWindow && g.MovingWindow->RootWindowDockTree == window
               && IsMousePosValid())
    {
        if (window->Viewport != NULL && window->Viewport->Window == window)
            window->Viewport = AddUpdateViewport(window,
                                                 window->ID,
                                                 window->Pos,
                                                 window->Size,
                                                 AzViewportFlags_None);
    } else
    {
        bool try_to_merge_into_host_viewport =
                (window->Viewport && window == window->Viewport->Window
                 && (g.ActiveId == 0 || g.ActiveIdAllowOverlap));
        if (try_to_merge_into_host_viewport)
            UpdateTryMergeWindowIntoHostViewports(window);
    }

    if (window->Viewport == NULL)
        if (!UpdateTryMergeWindowIntoHostViewport(window, main_viewport))
            window->Viewport = AddUpdateViewport(window,
                                                 window->ID,
                                                 window->Pos,
                                                 window->Size,
                                                 AzViewportFlags_None);

    if (!lock_viewport)
    {
        if (flags & (AzWindowFlags_Tooltip | AzWindowFlags_Popup))
        {
            AzVec2 mouse_ref = (flags & AzWindowFlags_Tooltip)
                                       ? g.IO.MousePos
                                       : g.BeginPopupStack.back().OpenMousePos;
            bool use_mouse_ref = (!g.NavCursorVisible
                                  || !g.NavHighlightItemUnderNav
                                  || !g.NavWindow);
            bool mouse_valid = IsMousePosValid(&mouse_ref);
            if ((window->Appearing
                 || (flags & (AzWindowFlags_Tooltip | AzWindowFlags_ChildMenu)))
                && (!use_mouse_ref || mouse_valid))
                window->ViewportAllowPlatformMonitorExtend =
                        FindPlatformMonitorForPos(
                                (use_mouse_ref && mouse_valid)
                                        ? mouse_ref
                                        : NavCalcPreferredRefPos());
            else
                window->ViewportAllowPlatformMonitorExtend =
                        window->Viewport->PlatformMonitor;
        } else if (window->Viewport && window != window->Viewport->Window
                   && window->Viewport->Window
                   && !(flags & AzWindowFlags_ChildWindow)
                   && window->DockNode == NULL)
        {
            const bool will_be_visible = (window->DockIsActive
                                          && !window->DockTabIsVisible)
                                                 ? false
                                                 : true;
            if ((window->Flags & AzWindowFlags_DockNodeHost)
                && window->Viewport->LastFrameActive < g.FrameCount
                && will_be_visible)
            {
                AZ_DEBUG_LOG_VIEWPORT(
                        "[viewport] Window '%s' steal Viewport %08X from "
                        "Window '%s'\n",
                        window->Name,
                        window->Viewport->ID,
                        window->Viewport->Window->Name);
                window->Viewport->Window = window;
                window->Viewport->ID = window->ID;
                window->Viewport->LastNameHash = 0;
            } else if (!UpdateTryMergeWindowIntoHostViewports(window))
            {
                window->Viewport =
                        AddUpdateViewport(window,
                                          window->ID,
                                          window->Pos,
                                          window->Size,
                                          AzViewportFlags_NoFocusOnAppearing);
            }
        } else if (window->ViewportAllowPlatformMonitorExtend < 0
                   && (flags & AzWindowFlags_ChildWindow) == 0)
        {
            window->ViewportAllowPlatformMonitorExtend =
                    window->Viewport->PlatformMonitor;
        }
    }

    window->ViewportOwned = (window == window->Viewport->Window);
    window->ViewportId = window->Viewport->ID;
}

void Az::WindowSyncOwnedViewport(AzWindow* window,
                                 AzWindow* parent_window_in_stack)
{
    AzContext& g = *GAz;

    bool viewport_rect_changed = false;

    if (window->Viewport->PlatformRequestMove)
    {
        window->Pos = window->Viewport->Pos;
        MarkIniSettingsDirty(window);
    } else if (memcmp(&window->Viewport->Pos, &window->Pos, sizeof(window->Pos))
               != 0)
    {
        viewport_rect_changed = true;
        window->Viewport->Pos = window->Pos;
    }

    if (window->Viewport->PlatformRequestResize)
    {
        window->Size = window->SizeFull = window->Viewport->Size;
        MarkIniSettingsDirty(window);
    } else if (memcmp(&window->Viewport->Size,
                      &window->Size,
                      sizeof(window->Size))
               != 0)
    {
        viewport_rect_changed = true;
        window->Viewport->Size = window->Size;
    }
    window->Viewport->UpdateWorkRect();

    if (viewport_rect_changed)
        UpdateViewportPlatformMonitor(window->Viewport);

    const AzViewportFlags viewport_flags_to_clear =
            AzViewportFlags_TopMost | AzViewportFlags_NoTaskBarIcon
            | AzViewportFlags_NoDecoration | AzViewportFlags_NoRendererClear;
    AzViewportFlags viewport_flags = window->Viewport->Flags
                                     & ~viewport_flags_to_clear;
    AzWindowFlags window_flags = window->Flags;
    const bool is_modal = (window_flags & AzWindowFlags_Modal) != 0;
    const bool is_short_lived_floating_window = (window_flags
                                                 & (AzWindowFlags_ChildMenu
                                                    | AzWindowFlags_Tooltip
                                                    | AzWindowFlags_Popup))
                                                != 0;
    if (window_flags & AzWindowFlags_Tooltip)
        viewport_flags |= AzViewportFlags_TopMost;
    if ((g.IO.ConfigViewportsNoTaskBarIcon || is_short_lived_floating_window)
        && !is_modal)
        viewport_flags |= AzViewportFlags_NoTaskBarIcon;
    if (g.IO.ConfigViewportsNoDecoration || is_short_lived_floating_window)
        viewport_flags |= AzViewportFlags_NoDecoration;

    if (is_short_lived_floating_window && !is_modal)
        viewport_flags |= AzViewportFlags_NoFocusOnAppearing
                          | AzViewportFlags_NoFocusOnClick;

    if (window->WindowClass.ViewportFlagsOverrideSet)
        viewport_flags |= window->WindowClass.ViewportFlagsOverrideSet;
    if (window->WindowClass.ViewportFlagsOverrideClear)
        viewport_flags &= ~window->WindowClass.ViewportFlagsOverrideClear;

    if (!(window_flags & AzWindowFlags_NoBackground))
        viewport_flags |= AzViewportFlags_NoRendererClear;

    window->Viewport->Flags = viewport_flags;

    if (window->WindowClass.ParentViewportId != (AzID)-1)
        window->Viewport->ParentViewportId =
                window->WindowClass.ParentViewportId;
    else if ((window_flags & (AzWindowFlags_Popup | AzWindowFlags_Tooltip))
             && parent_window_in_stack
             && (!parent_window_in_stack->IsFallbackWindow
                 || parent_window_in_stack->WasActive))
        window->Viewport->ParentViewportId =
                parent_window_in_stack->Viewport->ID;
    else
        window->Viewport->ParentViewportId = g.IO.ConfigViewportsNoDefaultParent
                                                     ? 0
                                                     : AZ_VIEWPORT_DEFAULT_ID;
}

void Az::UpdatePlatformWindows()
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.FrameCountEnded == g.FrameCount && "Forgot to call Render() or EndFrame() before UpdatePlatformWindows()?");
    AZ_ASSERT(g.FrameCountPlatformEnded < g.FrameCount);
    g.FrameCountPlatformEnded = g.FrameCount;
    if (!(g.ConfigFlagsCurrFrame & AzConfigFlags_ViewportsEnable))
        return;

    for (int i = 1; i < g.Viewports.Size; i++)
    {
        AzViewportP* viewport = g.Viewports[i];

        bool destroy_platform_window = false;
        destroy_platform_window |= (viewport->LastFrameActive
                                    < g.FrameCount - 1);
        destroy_platform_window |= (viewport->Window
                                    && !IsWindowActiveAndVisible(
                                            viewport->Window));
        if (destroy_platform_window)
        {
            DestroyPlatformWindow(viewport);
            continue;
        }

        if (viewport->LastFrameActive < g.FrameCount || viewport->Size.x <= 0
            || viewport->Size.y <= 0)
            continue;

        const bool is_new_platform_window = (viewport->PlatformWindowCreated
                                             == false);
        if (is_new_platform_window)
        {
            AZ_DEBUG_LOG_VIEWPORT(
                    "[viewport] Create Platform Window %08X '%s'\n",
                    viewport->ID,
                    viewport->Window ? viewport->Window->Name : "n/a");
            g.PlatformIO.Platform_CreateWindow(viewport);
            if (g.PlatformIO.Renderer_CreateWindow != NULL)
                g.PlatformIO.Renderer_CreateWindow(viewport);
            g.PlatformWindowsCreatedCount++;
            viewport->LastNameHash = 0;
            viewport->LastPlatformPos = viewport->LastPlatformSize =
                    AzVec2(FLT_MAX, FLT_MAX);

            viewport->LastRendererSize = viewport->Size;

            viewport->PlatformWindowCreated = true;
        }

        if ((viewport->LastPlatformPos.x != viewport->Pos.x
             || viewport->LastPlatformPos.y != viewport->Pos.y)
            && !viewport->PlatformRequestMove)
            g.PlatformIO.Platform_SetWindowPos(viewport, viewport->Pos);
        if ((viewport->LastPlatformSize.x != viewport->Size.x
             || viewport->LastPlatformSize.y != viewport->Size.y)
            && !viewport->PlatformRequestResize)
            g.PlatformIO.Platform_SetWindowSize(viewport, viewport->Size);
        if ((viewport->LastRendererSize.x != viewport->Size.x
             || viewport->LastRendererSize.y != viewport->Size.y)
            && g.PlatformIO.Renderer_SetWindowSize)
            g.PlatformIO.Renderer_SetWindowSize(viewport, viewport->Size);
        viewport->LastPlatformPos = viewport->Pos;
        viewport->LastPlatformSize = viewport->LastRendererSize =
                viewport->Size;

        if (AzWindow* window_for_title = GetWindowForTitleDisplay(
                    viewport->Window))
        {
            const char* title_begin = window_for_title->Name;
            char* title_end = (char*)(intptr_t)FindRenderedTextEnd(title_begin);
            const AzID title_hash = AzHashStr(title_begin,
                                              title_end - title_begin);
            if (viewport->LastNameHash != title_hash)
            {
                char title_end_backup_c = *title_end;
                *title_end = 0;

                g.PlatformIO.Platform_SetWindowTitle(viewport, title_begin);
                *title_end = title_end_backup_c;
                viewport->LastNameHash = title_hash;
            }
        }

        if (viewport->LastAlpha != viewport->Alpha
            && g.PlatformIO.Platform_SetWindowAlpha)
            g.PlatformIO.Platform_SetWindowAlpha(viewport, viewport->Alpha);
        viewport->LastAlpha = viewport->Alpha;

        if (g.PlatformIO.Platform_UpdateWindow)
            g.PlatformIO.Platform_UpdateWindow(viewport);

        if (is_new_platform_window)
        {
            if (g.FrameCount < 3)
                viewport->Flags |= AzViewportFlags_NoFocusOnAppearing;

            g.PlatformIO.Platform_ShowWindow(viewport);

            if (viewport->LastFocusedStampCount != g.ViewportFocusedStampCount)
                viewport->LastFocusedStampCount = ++g.ViewportFocusedStampCount;
        }

        viewport->ClearRequestFlags();
    }
}

void Az::RenderPlatformWindowsDefault(void* platform_render_arg,
                                      void* renderer_render_arg)
{
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    for (int i = 1; i < platform_io.Viewports.Size; i++)
    {
        AzViewport* viewport = platform_io.Viewports[i];
        if (viewport->Flags & AzViewportFlags_IsMinimized)
            continue;
        if (platform_io.Platform_RenderWindow)
            platform_io.Platform_RenderWindow(viewport, platform_render_arg);
        if (platform_io.Renderer_RenderWindow)
            platform_io.Renderer_RenderWindow(viewport, renderer_render_arg);
    }
    for (int i = 1; i < platform_io.Viewports.Size; i++)
    {
        AzViewport* viewport = platform_io.Viewports[i];
        if (viewport->Flags & AzViewportFlags_IsMinimized)
            continue;
        if (platform_io.Platform_SwapBuffers)
            platform_io.Platform_SwapBuffers(viewport, platform_render_arg);
        if (platform_io.Renderer_SwapBuffers)
            platform_io.Renderer_SwapBuffers(viewport, renderer_render_arg);
    }
}

static int Az::FindPlatformMonitorForPos(const AzVec2& pos)
{
    AzContext& g = *GAz;
    for (int monitor_n = 0; monitor_n < g.PlatformIO.Monitors.Size; monitor_n++)
    {
        const AzPlatformMonitor& monitor = g.PlatformIO.Monitors[monitor_n];
        if (AzRect(monitor.MainPos, monitor.MainPos + monitor.MainSize)
                    .Contains(pos))
            return monitor_n;
    }
    return -1;
}

static int Az::FindPlatformMonitorForRect(const AzRect& rect)
{
    AzContext& g = *GAz;

    const int monitor_count = g.PlatformIO.Monitors.Size;
    if (monitor_count <= 1)
        return monitor_count - 1;

    const float surface_threshold = AzMax(rect.GetWidth() * rect.GetHeight()
                                                  * 0.5f,
                                          1.0f);
    int best_monitor_n = 0;
    float best_monitor_surface = 0.001f;

    for (int monitor_n = 0; monitor_n < g.PlatformIO.Monitors.Size
                            && best_monitor_surface < surface_threshold;
         monitor_n++)
    {
        const AzPlatformMonitor& monitor = g.PlatformIO.Monitors[monitor_n];
        const AzRect monitor_rect = AzRect(monitor.MainPos,
                                           monitor.MainPos + monitor.MainSize);
        if (monitor_rect.Contains(rect))
            return monitor_n;
        AzRect overlapping_rect = rect;
        overlapping_rect.ClipWithFull(monitor_rect);
        float overlapping_surface = overlapping_rect.GetWidth()
                                    * overlapping_rect.GetHeight();
        if (overlapping_surface < best_monitor_surface)
            continue;
        best_monitor_surface = overlapping_surface;
        best_monitor_n = monitor_n;
    }
    return best_monitor_n;
}

static void Az::UpdateViewportPlatformMonitor(AzViewportP* viewport)
{
    viewport->PlatformMonitor = (short)FindPlatformMonitorForRect(
            viewport->GetMainRect());
}

const AzPlatformMonitor* Az::GetViewportPlatformMonitor(AzViewport* viewport_p)
{
    AzContext& g = *GAz;
    AzViewportP* viewport = (AzViewportP*)(void*)viewport_p;
    int monitor_idx = viewport->PlatformMonitor;
    if (monitor_idx >= 0 && monitor_idx < g.PlatformIO.Monitors.Size)
        return &g.PlatformIO.Monitors[monitor_idx];
    return &g.FallbackMonitor;
}

void Az::DestroyPlatformWindow(AzViewportP* viewport)
{
    AzContext& g = *GAz;
    if (viewport->PlatformWindowCreated)
    {
        AZ_DEBUG_LOG_VIEWPORT("[viewport] Destroy Platform Window %08X '%s'\n",
                              viewport->ID,
                              viewport->Window ? viewport->Window->Name
                                               : "n/a");
        if (g.PlatformIO.Renderer_DestroyWindow)
            g.PlatformIO.Renderer_DestroyWindow(viewport);
        if (g.PlatformIO.Platform_DestroyWindow)
            g.PlatformIO.Platform_DestroyWindow(viewport);
        AZ_ASSERT(viewport->RendererUserData == NULL
                  && viewport->PlatformUserData == NULL);

        if (viewport->ID != AZ_VIEWPORT_DEFAULT_ID)
            viewport->PlatformWindowCreated = false;
    } else
    {
        AZ_ASSERT(viewport->RendererUserData == NULL
                  && viewport->PlatformUserData == NULL
                  && viewport->PlatformHandle == NULL);
    }
    viewport->RendererUserData = viewport->PlatformUserData =
            viewport->PlatformHandle = NULL;
    viewport->ClearRequestFlags();
}

void Az::DestroyPlatformWindows()
{
    AzContext& g = *GAz;
    for (AzViewportP* viewport : g.Viewports)
        DestroyPlatformWindow(viewport);
}

enum AzDockRequestType
{
    AzDockRequestType_None = 0,
    AzDockRequestType_Dock,
    AzDockRequestType_Undock,
    AzDockRequestType_Split
};

struct AzDockRequest
{
    AzDockRequestType Type;
    AzWindow* DockTargetWindow;

    AzDockNode* DockTargetNode;
    AzWindow* DockPayload;

    AzDir DockSplitDir;
    float DockSplitRatio;
    bool DockSplitOuter;
    AzWindow* UndockTargetWindow;
    AzDockNode* UndockTargetNode;

    AzDockRequest()
    {
        Type = AzDockRequestType_None;
        DockTargetWindow = DockPayload = UndockTargetWindow = NULL;
        DockTargetNode = UndockTargetNode = NULL;
        DockSplitDir = AzDir_None;
        DockSplitRatio = 0.5f;
        DockSplitOuter = false;
    }
};

struct AzDockPreviewData
{
    AzDockNode FutureNode;
    bool IsDropAllowed;
    bool IsCenterAvailable;
    bool IsSidesAvailable;
    bool IsSplitDirExplicit;

    AzDockNode* SplitNode;
    AzDir SplitDir;
    float SplitRatio;
    AzRect DropRectsDraw[AzDir_COUNT + 1];

    AzDockPreviewData() : FutureNode(0)
    {
        IsDropAllowed = IsCenterAvailable = IsSidesAvailable =
                IsSplitDirExplicit = false;
        SplitNode = NULL;
        SplitDir = AzDir_None;
        SplitRatio = 0.f;
        for (int n = 0; n < AZ_ARRAYSIZE(DropRectsDraw); n++)
            DropRectsDraw[n] = AzRect(+FLT_MAX, +FLT_MAX, -FLT_MAX, -FLT_MAX);
    }
};

struct AzDockNodeSettings
{
    AzID ID;
    AzID ParentNodeId;
    AzID ParentWindowId;
    AzID SelectedTabId;
    signed char SplitAxis;
    char Depth;
    AzDockNodeFlags Flags;

    AzVec2ih Pos;
    AzVec2ih Size;
    AzVec2ih SizeRef;
    AzDockNodeSettings()
    {
        memset(this, 0, sizeof(*this));
        SplitAxis = AzAxis_None;
    }
};

namespace Az
{

    static AzDockNode* DockContextAddNode(AzContext* ctx, AzID id);
    static void DockContextRemoveNode(AzContext* ctx,
                                      AzDockNode* node,
                                      bool merge_sibling_into_parent_node);
    static void DockContextQueueNotifyRemovedNode(AzContext* ctx,
                                                  AzDockNode* node);
    static void DockContextProcessDock(AzContext* ctx, AzDockRequest* req);
    static void DockContextPruneUnusedSettingsNodes(AzContext* ctx);
    static AzDockNode* DockContextBindNodeToWindow(AzContext* ctx,
                                                   AzWindow* window);
    static void DockContextBuildNodesFromSettings(
            AzContext* ctx,
            AzDockNodeSettings* node_settings_array,
            int node_settings_count);
    static void DockContextBuildAddWindowsToNodes(AzContext* ctx, AzID root_id);

    static void DockNodeAddWindow(AzDockNode* node,
                                  AzWindow* window,
                                  bool add_to_tab_bar);
    static void DockNodeMoveWindows(AzDockNode* dst_node, AzDockNode* src_node);
    static void DockNodeMoveChildNodes(AzDockNode* dst_node,
                                       AzDockNode* src_node);
    static AzWindow* DockNodeFindWindowByID(AzDockNode* node, AzID id);
    static void DockNodeApplyPosSizeToWindows(AzDockNode* node);
    static void DockNodeRemoveWindow(AzDockNode* node,
                                     AzWindow* window,
                                     AzID save_dock_id);
    static void DockNodeHideHostWindow(AzDockNode* node);
    static void DockNodeUpdate(AzDockNode* node);
    static void DockNodeUpdateForRootNode(AzDockNode* node);
    static void DockNodeUpdateFlagsAndCollapse(AzDockNode* node);
    static void DockNodeUpdateHasCentralNodeChild(AzDockNode* node);
    static void DockNodeUpdateTabBar(AzDockNode* node, AzWindow* host_window);
    static void DockNodeAddTabBar(AzDockNode* node);
    static void DockNodeRemoveTabBar(AzDockNode* node);
    static void DockNodeWindowMenuUpdate(AzDockNode* node, AzTabBar* tab_bar);
    static void DockNodeUpdateVisibleFlag(AzDockNode* node);
    static void DockNodeStartMouseMovingWindow(AzDockNode* node,
                                               AzWindow* window);
    static bool DockNodeIsDropAllowed(AzWindow* host_window,
                                      AzWindow* payload_window);
    static void DockNodePreviewDockSetup(AzWindow* host_window,
                                         AzDockNode* host_node,
                                         AzWindow* payload_window,
                                         AzDockNode* payload_node,
                                         AzDockPreviewData* preview_data,
                                         bool is_explicit_target,
                                         bool is_outer_docking);
    static void DockNodePreviewDockRender(
            AzWindow* host_window,
            AzDockNode* host_node,
            AzWindow* payload_window,
            const AzDockPreviewData* preview_data);
    static void DockNodeCalcTabBarLayout(const AzDockNode* node,
                                         AzRect* out_title_rect,
                                         AzRect* out_tab_bar_rect,
                                         AzVec2* out_window_menu_button_pos,
                                         AzVec2* out_close_button_pos);
    static void DockNodeCalcSplitRects(AzVec2& pos_old,
                                       AzVec2& size_old,
                                       AzVec2& pos_new,
                                       AzVec2& size_new,
                                       AzDir dir,
                                       AzVec2 size_new_desired);
    static bool DockNodeCalcDropRectsAndTestMousePos(const AzRect& parent,
                                                     AzDir dir,
                                                     AzRect& out_draw,
                                                     bool outer_docking,
                                                     AzVec2* test_mouse_pos);
    static const char* DockNodeGetHostWindowTitle(AzDockNode* node,
                                                  char* buf,
                                                  int buf_size)
    {
        AzFormatString(buf, buf_size, "##DockNode_%02X", node->ID);
        return buf;
    }
    static int DockNodeGetTabOrder(AzWindow* window);

    static void DockNodeTreeSplit(AzContext* ctx,
                                  AzDockNode* parent_node,
                                  AzAxis split_axis,
                                  int split_first_child,
                                  float split_ratio,
                                  AzDockNode* new_node);
    static void DockNodeTreeMerge(AzContext* ctx,
                                  AzDockNode* parent_node,
                                  AzDockNode* merge_lead_child);
    static void DockNodeTreeUpdatePosSize(
            AzDockNode* node,
            AzVec2 pos,
            AzVec2 size,
            AzDockNode* only_write_to_single_node = NULL);
    static void DockNodeTreeUpdateSplitter(AzDockNode* node);
    static AzDockNode* DockNodeTreeFindVisibleNodeByPos(AzDockNode* node,
                                                        AzVec2 pos);
    static AzDockNode* DockNodeTreeFindFallbackLeafNode(AzDockNode* node);

    static void DockSettingsRenameNodeReferences(AzID old_node_id,
                                                 AzID new_node_id);
    static void DockSettingsRemoveNodeReferences(AzID* node_ids,
                                                 int node_ids_count);
    static AzDockNodeSettings* DockSettingsFindNodeSettings(AzContext* ctx,
                                                            AzID node_id);
    static void DockSettingsHandler_ClearAll(AzContext*, AzSettingsHandler*);
    static void DockSettingsHandler_ApplyAll(AzContext*, AzSettingsHandler*);
    static void* DockSettingsHandler_ReadOpen(AzContext*,
                                              AzSettingsHandler*,
                                              const char* name);
    static void DockSettingsHandler_ReadLine(AzContext*,
                                             AzSettingsHandler*,
                                             void* entry,
                                             const char* line);
    static void DockSettingsHandler_WriteAll(AzContext* imgui_ctx,
                                             AzSettingsHandler* handler,
                                             AzTextBuffer* buf);
} // namespace Az

void Az::DockContextInitialize(AzContext* ctx)
{
    AzContext& g = *ctx;

    AzSettingsHandler ini_handler;
    ini_handler.TypeName = "Docking";
    ini_handler.TypeHash = AzHashStr("Docking");
    ini_handler.ClearAllFn = DockSettingsHandler_ClearAll;
    ini_handler.ReadInitFn = DockSettingsHandler_ClearAll;
    ini_handler.ReadOpenFn = DockSettingsHandler_ReadOpen;
    ini_handler.ReadLineFn = DockSettingsHandler_ReadLine;
    ini_handler.ApplyAllFn = DockSettingsHandler_ApplyAll;
    ini_handler.WriteAllFn = DockSettingsHandler_WriteAll;
    g.SettingsHandlers.push_back(ini_handler);

    g.DockNodeWindowMenuHandler = &DockNodeWindowMenuHandler_Default;
}

void Az::DockContextShutdown(AzContext* ctx)
{
    AzDockContext* dc = &ctx->DockContext;
    for (int n = 0; n < dc->Nodes.Data.Size; n++)
        if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
            AZ_DELETE(node);
}

void Az::DockContextClearNodes(AzContext* ctx,
                               AzID root_id,
                               bool clear_settings_refs)
{
    AZ_UNUSED(ctx);
    AZ_ASSERT(ctx == GAz);
    DockBuilderRemoveNodeDockedWindows(root_id, clear_settings_refs);
    DockBuilderRemoveNodeChildNodes(root_id);
}

void Az::DockContextRebuildNodes(AzContext* ctx)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &ctx->DockContext;
    AZ_DEBUG_LOG_DOCKING("[docking] DockContextRebuildNodes\n");
    SaveIniSettingsToMemory();
    AzID root_id = 0;
    DockContextClearNodes(ctx, root_id, false);
    DockContextBuildNodesFromSettings(ctx,
                                      dc->NodesSettings.Data,
                                      dc->NodesSettings.Size);
    DockContextBuildAddWindowsToNodes(ctx, root_id);
}

void Az::DockContextNewFrameUpdateUndocking(AzContext* ctx)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &ctx->DockContext;
    if (!(g.IO.ConfigFlags & AzConfigFlags_DockingEnable))
    {
        if (dc->Nodes.Data.Size > 0 || dc->Requests.Size > 0)
            DockContextClearNodes(ctx, 0, true);
        return;
    }

    if (g.IO.ConfigDockingNoSplit)
        for (int n = 0; n < dc->Nodes.Data.Size; n++)
            if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
                if (node->IsRootNode() && node->IsSplitNode())
                {
                    DockBuilderRemoveNodeChildNodes(node->ID);
                }

#if 0
    if (Az::IsKeyPressed(Az::GetKeyIndex(AzKey_C)))
        dc->WantFullRebuild = true;
#endif
    if (dc->WantFullRebuild)
    {
        DockContextRebuildNodes(ctx);
        dc->WantFullRebuild = false;
    }

    for (AzDockRequest& req : dc->Requests)
    {
        if (req.Type == AzDockRequestType_Undock && req.UndockTargetWindow)
            DockContextProcessUndockWindow(ctx, req.UndockTargetWindow);
        else if (req.Type == AzDockRequestType_Undock && req.UndockTargetNode)
            DockContextProcessUndockNode(ctx, req.UndockTargetNode);
    }
}

void Az::DockContextNewFrameUpdateDocking(AzContext* ctx)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &ctx->DockContext;
    if (!(g.IO.ConfigFlags & AzConfigFlags_DockingEnable))
        return;

    g.DebugHoveredDockNode = NULL;
    if (AzWindow* hovered_window = g.HoveredWindowUnderMovingWindow)
    {
        if (hovered_window->DockNodeAsHost)
            g.DebugHoveredDockNode = DockNodeTreeFindVisibleNodeByPos(
                    hovered_window->DockNodeAsHost,
                    g.IO.MousePos);
        else if (hovered_window->RootWindow->DockNode)
            g.DebugHoveredDockNode = hovered_window->RootWindow->DockNode;
    }

    for (AzDockRequest& req : dc->Requests)
        if (req.Type == AzDockRequestType_Dock)
            DockContextProcessDock(ctx, &req);
    dc->Requests.resize(0);

    for (int n = 0; n < dc->Nodes.Data.Size; n++)
        if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
            if (node->IsFloatingNode())
                DockNodeUpdate(node);
}

void Az::DockContextEndFrame(AzContext* ctx)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &g.DockContext;
    for (int n = 0; n < dc->Nodes.Data.Size; n++)
        if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
            if (node->LastFrameActive == g.FrameCount && node->IsVisible
                && node->HostWindow && node->IsLeafNode()
                && !node->IsBgDrawnThisFrame)
            {
                AzRect bg_rect(node->Pos + AzVec2(0.0f, GetFrameHeight()),
                               node->Pos + node->Size);
                AzDrawFlags bg_rounding_flags = CalcRoundingFlagsForRectInRect(
                        bg_rect,
                        node->HostWindow->Rect(),
                        g.Style.DockingSeparatorSize);
                node->HostWindow->DrawList->ChannelsSetCurrent(
                        DOCKING_HOST_DRAW_CHANNEL_BG);
                node->HostWindow->DrawList->AddRectFilled(
                        bg_rect.Min,
                        bg_rect.Max,
                        node->LastBgColor,
                        node->HostWindow->WindowRounding,
                        bg_rounding_flags);
            }
}

AzDockNode* Az::DockContextFindNodeByID(AzContext* ctx, AzID id)
{
    return (AzDockNode*)ctx->DockContext.Nodes.GetVoidPtr(id);
}

AzID Az::DockContextGenNodeID(AzContext* ctx)
{
    AzID id = 0x0001;
    while (DockContextFindNodeByID(ctx, id) != NULL)
        id++;
    return id;
}

static AzDockNode* Az::DockContextAddNode(AzContext* ctx, AzID id)
{
    AzContext& g = *ctx;
    if (id == 0)
        id = DockContextGenNodeID(ctx);
    else
        AZ_ASSERT(DockContextFindNodeByID(ctx, id) == NULL);

    AZ_DEBUG_LOG_DOCKING("[docking] DockContextAddNode 0x%08X\n", id);
    AzDockNode* node = AZ_NEW(AzDockNode)(id);
    ctx->DockContext.Nodes.SetVoidPtr(node->ID, node);
    return node;
}

static void Az::DockContextRemoveNode(AzContext* ctx,
                                      AzDockNode* node,
                                      bool merge_sibling_into_parent_node)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &ctx->DockContext;

    AZ_DEBUG_LOG_DOCKING("[docking] DockContextRemoveNode 0x%08X\n", node->ID);
    AZ_ASSERT(DockContextFindNodeByID(ctx, node->ID) == node);
    AZ_ASSERT(node->ChildNodes[0] == NULL && node->ChildNodes[1] == NULL);
    AZ_ASSERT(node->Windows.Size == 0);

    if (node->HostWindow)
        node->HostWindow->DockNodeAsHost = NULL;

    AzDockNode* parent_node = node->ParentNode;
    const bool merge = (merge_sibling_into_parent_node && parent_node != NULL);
    if (merge)
    {
        AZ_ASSERT(parent_node->ChildNodes[0] == node
                  || parent_node->ChildNodes[1] == node);
        AzDockNode* sibling_node = (parent_node->ChildNodes[0] == node
                                            ? parent_node->ChildNodes[1]
                                            : parent_node->ChildNodes[0]);
        DockNodeTreeMerge(&g, parent_node, sibling_node);
    } else
    {
        for (int n = 0;
             parent_node && n < AZ_ARRAYSIZE(parent_node->ChildNodes);
             n++)
            if (parent_node->ChildNodes[n] == node)
                node->ParentNode->ChildNodes[n] = NULL;
        dc->Nodes.SetVoidPtr(node->ID, NULL);
        AZ_DELETE(node);
    }
}

static int AZ_CDECL DockNodeComparerDepthMostFirst(const void* lhs,
                                                   const void* rhs)
{
    const AzDockNode* a = *(const AzDockNode* const*)lhs;
    const AzDockNode* b = *(const AzDockNode* const*)rhs;
    return Az::DockNodeGetDepth(b) - Az::DockNodeGetDepth(a);
}

struct AzDockContextPruneNodeData
{
    int CountWindows, CountChildWindows, CountChildNodes;
    AzID RootId;
    AzDockContextPruneNodeData()
    {
        CountWindows = CountChildWindows = CountChildNodes = 0;
        RootId = 0;
    }
};

static void Az::DockContextPruneUnusedSettingsNodes(AzContext* ctx)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &ctx->DockContext;
    AZ_ASSERT(g.Windows.Size == 0);

    AzPool<AzDockContextPruneNodeData> pool;
    pool.Reserve(dc->NodesSettings.Size);

    for (int settings_n = 0; settings_n < dc->NodesSettings.Size; settings_n++)
    {
        AzDockNodeSettings* settings = &dc->NodesSettings[settings_n];
        AzDockContextPruneNodeData* parent_data =
                settings->ParentNodeId ? pool.GetByKey(settings->ParentNodeId)
                                       : 0;
        pool.GetOrAddByKey(settings->ID)->RootId = parent_data
                                                           ? parent_data->RootId
                                                           : settings->ID;
        if (settings->ParentNodeId)
            pool.GetOrAddByKey(settings->ParentNodeId)->CountChildNodes++;
    }

    for (int settings_n = 0; settings_n < dc->NodesSettings.Size; settings_n++)
    {
        AzDockNodeSettings* settings = &dc->NodesSettings[settings_n];
        if (settings->ParentWindowId != 0)
            if (AzWindowSettings* window_settings = FindWindowSettingsByID(
                        settings->ParentWindowId))
                if (window_settings->DockId)
                    if (AzDockContextPruneNodeData* data = pool.GetByKey(
                                window_settings->DockId))
                        data->CountChildNodes++;
    }

    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
        if (AzID dock_id = settings->DockId)
            if (AzDockContextPruneNodeData* data = pool.GetByKey(dock_id))
            {
                data->CountWindows++;
                if (AzDockContextPruneNodeData* data_root =
                            (data->RootId == dock_id)
                                    ? data
                                    : pool.GetByKey(data->RootId))
                    data_root->CountChildWindows++;
            }

    for (int settings_n = 0; settings_n < dc->NodesSettings.Size; settings_n++)
    {
        AzDockNodeSettings* settings = &dc->NodesSettings[settings_n];
        AzDockContextPruneNodeData* data = pool.GetByKey(settings->ID);
        if (data->CountWindows > 1)
            continue;
        AzDockContextPruneNodeData* data_root = (data->RootId == settings->ID)
                                                        ? data
                                                        : pool.GetByKey(
                                                                  data->RootId);

        bool remove = false;
        remove |= (data->CountWindows == 1 && settings->ParentNodeId == 0
                   && data->CountChildNodes == 0
                   && !(settings->Flags & AzDockNodeFlags_CentralNode));

        remove |= (data->CountWindows == 0 && settings->ParentNodeId == 0
                   && data->CountChildNodes == 0);
        remove |= (data_root->CountChildWindows == 0);
        if (remove)
        {
            AZ_DEBUG_LOG_DOCKING(
                    "[docking] DockContextPruneUnusedSettingsNodes: Prune "
                    "0x%08X\n",
                    settings->ID);
            DockSettingsRemoveNodeReferences(&settings->ID, 1);
            settings->ID = 0;
        }
    }
}

static void Az::DockContextBuildNodesFromSettings(
        AzContext* ctx,
        AzDockNodeSettings* node_settings_array,
        int node_settings_count)
{
    for (int node_n = 0; node_n < node_settings_count; node_n++)
    {
        AzDockNodeSettings* settings = &node_settings_array[node_n];
        if (settings->ID == 0)
            continue;
        AzDockNode* node = DockContextAddNode(ctx, settings->ID);
        node->ParentNode =
                settings->ParentNodeId
                        ? DockContextFindNodeByID(ctx, settings->ParentNodeId)
                        : NULL;
        node->Pos = AzVec2(settings->Pos.x, settings->Pos.y);
        node->Size = AzVec2(settings->Size.x, settings->Size.y);
        node->SizeRef = AzVec2(settings->SizeRef.x, settings->SizeRef.y);
        node->AuthorityForPos = node->AuthorityForSize =
                node->AuthorityForViewport = AzDataAuthority_DockNode;
        if (node->ParentNode && node->ParentNode->ChildNodes[0] == NULL)
            node->ParentNode->ChildNodes[0] = node;
        else if (node->ParentNode && node->ParentNode->ChildNodes[1] == NULL)
            node->ParentNode->ChildNodes[1] = node;
        node->SelectedTabId = settings->SelectedTabId;
        node->SplitAxis = (AzAxis)settings->SplitAxis;
        node->SetLocalFlags(settings->Flags & AzDockNodeFlags_SavedFlagsMask_);

        char host_window_title[20];
        AzDockNode* root_node = DockNodeGetRootNode(node);
        node->HostWindow = FindWindowByName(
                DockNodeGetHostWindowTitle(root_node,
                                           host_window_title,
                                           AZ_ARRAYSIZE(host_window_title)));
    }
}

void Az::DockContextBuildAddWindowsToNodes(AzContext* ctx, AzID root_id)
{
    AzContext& g = *ctx;
    for (AzWindow* window : g.Windows)
    {
        if (window->DockId == 0 || window->LastFrameActive < g.FrameCount - 1)
            continue;
        if (window->DockNode != NULL)
            continue;

        AzDockNode* node = DockContextFindNodeByID(ctx, window->DockId);
        AZ_ASSERT(node != NULL);

        if (root_id == 0 || DockNodeGetRootNode(node)->ID == root_id)
            DockNodeAddWindow(node, window, true);
    }
}

void Az::DockContextQueueDock(AzContext* ctx,
                              AzWindow* target,
                              AzDockNode* target_node,
                              AzWindow* payload,
                              AzDir split_dir,
                              float split_ratio,
                              bool split_outer)
{
    AZ_ASSERT(target != payload);
    AzDockRequest req;
    req.Type = AzDockRequestType_Dock;
    req.DockTargetWindow = target;
    req.DockTargetNode = target_node;
    req.DockPayload = payload;
    req.DockSplitDir = split_dir;
    req.DockSplitRatio = split_ratio;
    req.DockSplitOuter = split_outer;
    ctx->DockContext.Requests.push_back(req);
}

void Az::DockContextQueueUndockWindow(AzContext* ctx, AzWindow* window)
{
    AzDockRequest req;
    req.Type = AzDockRequestType_Undock;
    req.UndockTargetWindow = window;
    ctx->DockContext.Requests.push_back(req);
}

void Az::DockContextQueueUndockNode(AzContext* ctx, AzDockNode* node)
{
    AzDockRequest req;
    req.Type = AzDockRequestType_Undock;
    req.UndockTargetNode = node;
    ctx->DockContext.Requests.push_back(req);
}

void Az::DockContextQueueNotifyRemovedNode(AzContext* ctx, AzDockNode* node)
{
    AzDockContext* dc = &ctx->DockContext;
    for (AzDockRequest& req : dc->Requests)
        if (req.DockTargetNode == node)
            req.Type = AzDockRequestType_None;
}

void Az::DockContextProcessDock(AzContext* ctx, AzDockRequest* req)
{
    AZ_ASSERT((req->Type == AzDockRequestType_Dock && req->DockPayload != NULL)
              || (req->Type == AzDockRequestType_Split
                  && req->DockPayload == NULL));
    AZ_ASSERT(req->DockTargetWindow != NULL || req->DockTargetNode != NULL);

    AzContext& g = *ctx;
    AZ_UNUSED(g);

    AzWindow* payload_window = req->DockPayload;
    AzWindow* target_window = req->DockTargetWindow;
    AzDockNode* node = req->DockTargetNode;
    if (payload_window)
        AZ_DEBUG_LOG_DOCKING(
                "[docking] DockContextProcessDock node 0x%08X target '%s' dock "
                "window '%s', split_dir %d\n",
                node ? node->ID : 0,
                target_window ? target_window->Name : "NULL",
                payload_window->Name,
                req->DockSplitDir);
    else
        AZ_DEBUG_LOG_DOCKING(
                "[docking] DockContextProcessDock node 0x%08X, split_dir %d\n",
                node ? node->ID : 0,
                req->DockSplitDir);

    AzID next_selected_id = 0;
    AzDockNode* payload_node = NULL;
    if (payload_window)
    {
        payload_node = payload_window->DockNodeAsHost;
        payload_window->DockNodeAsHost = NULL;

        if (payload_node && payload_node->IsLeafNode())
            next_selected_id = payload_node->TabBar->NextSelectedTabId
                                       ? payload_node->TabBar->NextSelectedTabId
                                       : payload_node->TabBar->SelectedTabId;
        if (payload_node == NULL)
            next_selected_id = payload_window->TabId;
    }

    if (node)
        AZ_ASSERT(node->LastFrameAlive <= g.FrameCount);
    if (node && target_window && node == target_window->DockNodeAsHost)
        AZ_ASSERT(node->Windows.Size > 0 || node->IsSplitNode()
                  || node->IsCentralNode());

    if (node == NULL)
    {
        node = DockContextAddNode(ctx, 0);
        node->Pos = target_window->Pos;
        node->Size = target_window->Size;
        if (target_window->DockNodeAsHost == NULL)
        {
            DockNodeAddWindow(node, target_window, true);
            node->TabBar->Tabs[0].Flags &= ~AzTabItemFlags_Unsorted;
            target_window->DockIsActive = true;
        }
    }

    AzDir split_dir = req->DockSplitDir;
    if (split_dir != AzDir_None)
    {
        const AzAxis split_axis = (split_dir == AzDir_Left
                                   || split_dir == AzDir_Right)
                                          ? AzAxis_X
                                          : AzAxis_Y;
        const int split_inheritor_child_idx = (split_dir == AzDir_Left
                                               || split_dir == AzDir_Up)
                                                      ? 1
                                                      : 0;

        const float split_ratio = req->DockSplitRatio;
        DockNodeTreeSplit(ctx,
                          node,
                          split_axis,
                          split_inheritor_child_idx,
                          split_ratio,
                          payload_node);
        AzDockNode* new_node = node->ChildNodes[split_inheritor_child_idx ^ 1];
        new_node->HostWindow = node->HostWindow;
        node = new_node;
    }
    node->SetLocalFlags(node->LocalFlags & ~AzDockNodeFlags_HiddenTabBar);

    if (node != payload_node)
    {
        if (node->Windows.Size > 0 && node->TabBar == NULL)
        {
            DockNodeAddTabBar(node);
            for (int n = 0; n < node->Windows.Size; n++)
                TabBarAddTab(node->TabBar,
                             AzTabItemFlags_None,
                             node->Windows[n]);
        }

        if (payload_node != NULL)
        {
            if (payload_node->IsSplitNode())
            {
                if (node->Windows.Size > 0)
                {
                    AZ_ASSERT(payload_node->OnlyNodeWithWindows != NULL);

                    AzDockNode* visible_node =
                            payload_node->OnlyNodeWithWindows;
                    if (visible_node->TabBar)
                        AZ_ASSERT(visible_node->TabBar->Tabs.Size > 0);
                    DockNodeMoveWindows(node, visible_node);
                    DockNodeMoveWindows(visible_node, node);
                    DockSettingsRenameNodeReferences(node->ID,
                                                     visible_node->ID);
                }
                if (node->IsCentralNode())
                {
                    AzDockNode* last_focused_node = DockContextFindNodeByID(
                            ctx,
                            payload_node->LastFocusedNodeId);
                    AZ_ASSERT(last_focused_node != NULL);
                    AzDockNode* last_focused_root_node = DockNodeGetRootNode(
                            last_focused_node);
                    AZ_ASSERT(last_focused_root_node
                              == DockNodeGetRootNode(payload_node));
                    last_focused_node->SetLocalFlags(
                            last_focused_node->LocalFlags
                            | AzDockNodeFlags_CentralNode);
                    node->SetLocalFlags(node->LocalFlags
                                        & ~AzDockNodeFlags_CentralNode);
                    last_focused_root_node->CentralNode = last_focused_node;
                }

                AZ_ASSERT(node->Windows.Size == 0);
                DockNodeMoveChildNodes(node, payload_node);
            } else
            {
                const AzID payload_dock_id = payload_node->ID;
                DockNodeMoveWindows(node, payload_node);
                DockSettingsRenameNodeReferences(payload_dock_id, node->ID);
            }
            DockContextRemoveNode(ctx, payload_node, true);
        } else if (payload_window)
        {
            const AzID payload_dock_id = payload_window->DockId;
            node->VisibleWindow = payload_window;
            DockNodeAddWindow(node, payload_window, true);
            if (payload_dock_id != 0)
                DockSettingsRenameNodeReferences(payload_dock_id, node->ID);
        }
    } else
    {
        node->WantHiddenTabBarUpdate = true;
    }

    if (AzTabBar* tab_bar = node->TabBar)
        tab_bar->NextSelectedTabId = next_selected_id;
    MarkIniSettingsDirty();
}

static AzVec2 FixLargeWindowsWhenUndocking(const AzVec2& size,
                                           AzViewport* ref_viewport)
{
    if (ref_viewport == NULL)
        return size;

    AzContext& g = *GAz;
    AzVec2 max_size = AzTrunc(ref_viewport->WorkSize * 0.90f);
    if (g.ConfigFlagsCurrFrame & AzConfigFlags_ViewportsEnable)
    {
        const AzPlatformMonitor* monitor = Az::GetViewportPlatformMonitor(
                ref_viewport);
        max_size = AzTrunc(monitor->WorkSize * 0.90f);
    }
    return AzMin(size, max_size);
}

void Az::DockContextProcessUndockWindow(AzContext* ctx,
                                        AzWindow* window,
                                        bool clear_persistent_docking_ref)
{
    AzContext& g = *ctx;
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockContextProcessUndockWindow window '%s', "
            "clear_persistent_docking_ref = %d\n",
            window->Name,
            clear_persistent_docking_ref);
    if (window->DockNode)
        DockNodeRemoveWindow(window->DockNode,
                             window,
                             clear_persistent_docking_ref ? 0 : window->DockId);
    else
        window->DockId = 0;
    window->Collapsed = false;
    window->DockIsActive = false;
    window->DockNodeIsVisible = window->DockTabIsVisible = false;
    window->Size = window->SizeFull =
            FixLargeWindowsWhenUndocking(window->SizeFull, window->Viewport);

    MarkIniSettingsDirty();
}

void Az::DockContextProcessUndockNode(AzContext* ctx, AzDockNode* node)
{
    AzContext& g = *ctx;
    AZ_DEBUG_LOG_DOCKING("[docking] DockContextProcessUndockNode node %08X\n",
                         node->ID);
    AZ_ASSERT(node->IsLeafNode());
    AZ_ASSERT(node->Windows.Size >= 1);

    if (node->IsRootNode() || node->IsCentralNode())
    {
        AzDockNode* new_node = DockContextAddNode(ctx, 0);
        new_node->Pos = node->Pos;
        new_node->Size = node->Size;
        new_node->SizeRef = node->SizeRef;
        DockNodeMoveWindows(new_node, node);
        DockSettingsRenameNodeReferences(node->ID, new_node->ID);
        node = new_node;
    } else
    {
        AZ_ASSERT(node->ParentNode->ChildNodes[0] == node
                  || node->ParentNode->ChildNodes[1] == node);
        int index_in_parent = (node->ParentNode->ChildNodes[0] == node) ? 0 : 1;
        node->ParentNode->ChildNodes[index_in_parent] = NULL;
        DockNodeTreeMerge(ctx,
                          node->ParentNode,
                          node->ParentNode->ChildNodes[index_in_parent ^ 1]);
        node->ParentNode->AuthorityForViewport = AzDataAuthority_Window;

        node->ParentNode = NULL;
    }
    for (AzWindow* window : node->Windows)
    {
        window->Flags &= ~AzWindowFlags_ChildWindow;
        if (window->ParentWindow)
            window->ParentWindow->DC.ChildWindows.find_erase(window);
        UpdateWindowParentAndRootLinks(window, window->Flags, NULL);
    }
    node->AuthorityForPos = node->AuthorityForSize = AzDataAuthority_DockNode;
    node->Size = FixLargeWindowsWhenUndocking(node->Size,
                                              node->Windows[0]->Viewport);
    node->WantMouseMove = true;
    MarkIniSettingsDirty();
}

bool Az::DockContextCalcDropPosForDocking(AzWindow* target,
                                          AzDockNode* target_node,
                                          AzWindow* payload_window,
                                          AzDockNode* payload_node,
                                          AzDir split_dir,
                                          bool split_outer,
                                          AzVec2* out_pos)
{
    if (target != NULL && target_node == NULL)
        target_node = target->DockNode;

    if (target_node && target_node->ParentNode == NULL
        && target_node->IsCentralNode() && split_dir != AzDir_None)
        split_outer = true;
    AzDockPreviewData split_data;
    DockNodePreviewDockSetup(target,
                             target_node,
                             payload_window,
                             payload_node,
                             &split_data,
                             false,
                             split_outer);
    if (split_data.DropRectsDraw[split_dir + 1].IsInverted())
        return false;
    *out_pos = split_data.DropRectsDraw[split_dir + 1].GetCenter();
    return true;
}

AzDockNode::AzDockNode(AzID id)
{
    ID = id;
    SharedFlags = LocalFlags = LocalFlagsInWindows = MergedFlags =
            AzDockNodeFlags_None;
    ParentNode = ChildNodes[0] = ChildNodes[1] = NULL;
    TabBar = NULL;
    SplitAxis = AzAxis_None;

    State = AzDockNodeState_Unknown;
    LastBgColor = AZ_COL32_WHITE;
    HostWindow = VisibleWindow = NULL;
    CentralNode = OnlyNodeWithWindows = NULL;
    CountNodeWithWindows = 0;
    LastFrameAlive = LastFrameActive = LastFrameFocused = -1;
    LastFocusedNodeId = 0;
    SelectedTabId = 0;
    WantCloseTabId = 0;
    RefViewportId = 0;
    AuthorityForPos = AuthorityForSize = AzDataAuthority_DockNode;
    AuthorityForViewport = AzDataAuthority_Auto;
    IsVisible = true;
    IsFocused = HasCloseButton = HasWindowMenuButton = HasCentralNodeChild =
            false;
    IsBgDrawnThisFrame = false;
    WantCloseAll = WantLockSizeOnce = WantMouseMove = WantHiddenTabBarUpdate =
            WantHiddenTabBarToggle = false;
}

AzDockNode::~AzDockNode()
{
    AZ_DELETE(TabBar);
    TabBar = NULL;
    ChildNodes[0] = ChildNodes[1] = NULL;
}

int Az::DockNodeGetTabOrder(AzWindow* window)
{
    AzTabBar* tab_bar = window->DockNode->TabBar;
    if (tab_bar == NULL)
        return -1;
    AzTabItem* tab = TabBarFindTabByID(tab_bar, window->TabId);
    return tab ? TabBarGetTabOrder(tab_bar, tab) : -1;
}

static void DockNodeHideWindowDuringHostWindowCreation(AzWindow* window)
{
    window->Hidden = true;
    window->HiddenFramesCanSkipItems = window->Active ? 1 : 2;
}

static void Az::DockNodeAddWindow(AzDockNode* node,
                                  AzWindow* window,
                                  bool add_to_tab_bar)
{
    AzContext& g = *GAz;
    (void)g;
    if (window->DockNode)
    {
        AZ_ASSERT(window->DockNode->ID != node->ID);
        DockNodeRemoveWindow(window->DockNode, window, 0);
    }
    AZ_ASSERT(window->DockNode == NULL || window->DockNodeAsHost == NULL);
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockNodeAddWindow node 0x%08X window '%s'\n",
            node->ID,
            window->Name);

    if (node->HostWindow == NULL && node->Windows.Size == 1
        && node->Windows[0]->WasActive == false)
        DockNodeHideWindowDuringHostWindowCreation(node->Windows[0]);

    node->Windows.push_back(window);
    node->WantHiddenTabBarUpdate = true;
    window->DockNode = node;
    window->DockId = node->ID;
    window->DockIsActive = (node->Windows.Size > 1);
    window->DockTabWantClose = false;

    if (node->HostWindow == NULL && node->IsFloatingNode())
    {
        if (node->AuthorityForPos == AzDataAuthority_Auto)
            node->AuthorityForPos = AzDataAuthority_Window;
        if (node->AuthorityForSize == AzDataAuthority_Auto)
            node->AuthorityForSize = AzDataAuthority_Window;
        if (node->AuthorityForViewport == AzDataAuthority_Auto)
            node->AuthorityForViewport = AzDataAuthority_Window;
    }

    if (add_to_tab_bar)
    {
        if (node->TabBar == NULL)
        {
            DockNodeAddTabBar(node);
            node->TabBar->SelectedTabId = node->TabBar->NextSelectedTabId =
                    node->SelectedTabId;

            for (int n = 0; n < node->Windows.Size - 1; n++)
                TabBarAddTab(node->TabBar,
                             AzTabItemFlags_None,
                             node->Windows[n]);
        }
        TabBarAddTab(node->TabBar, AzTabItemFlags_Unsorted, window);
    }

    DockNodeUpdateVisibleFlag(node);

    if (node->HostWindow)
        UpdateWindowParentAndRootLinks(window,
                                       window->Flags
                                               | AzWindowFlags_ChildWindow,
                                       node->HostWindow);
}

static void Az::DockNodeRemoveWindow(AzDockNode* node,
                                     AzWindow* window,
                                     AzID save_dock_id)
{
    AzContext& g = *GAz;
    AZ_ASSERT(window->DockNode == node);

    AZ_ASSERT(save_dock_id == 0 || save_dock_id == node->ID);
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockNodeRemoveWindow node 0x%08X window '%s'\n",
            node->ID,
            window->Name);

    window->DockNode = NULL;
    window->DockIsActive = window->DockTabWantClose = false;
    window->DockId = save_dock_id;
    window->Flags &= ~AzWindowFlags_ChildWindow;
    if (window->ParentWindow)
        window->ParentWindow->DC.ChildWindows.find_erase(window);
    UpdateWindowParentAndRootLinks(window, window->Flags, NULL);

    if (node->HostWindow && node->HostWindow->ViewportOwned)
    {
        window->Viewport = NULL;
        window->ViewportId = 0;
        window->ViewportOwned = false;
        window->Hidden = true;
    }

    bool erased = false;
    for (int n = 0; n < node->Windows.Size; n++)
        if (node->Windows[n] == window)
        {
            node->Windows.erase(node->Windows.Data + n);
            erased = true;
            break;
        }
    if (!erased)
        AZ_ASSERT(erased);
    if (node->VisibleWindow == window)
        node->VisibleWindow = NULL;

    node->WantHiddenTabBarUpdate = true;
    if (node->TabBar)
    {
        TabBarRemoveTab(node->TabBar, window->TabId);
        const int tab_count_threshold_for_tab_bar = node->IsCentralNode() ? 1
                                                                          : 2;
        if (node->Windows.Size < tab_count_threshold_for_tab_bar)
            DockNodeRemoveTabBar(node);
    }

    if (node->Windows.Size == 0 && !node->IsCentralNode()
        && !node->IsDockSpace() && window->DockId != node->ID)
    {
        DockContextRemoveNode(&g, node, true);
        return;
    }

    if (node->Windows.Size == 1 && !node->IsCentralNode() && node->HostWindow)
    {
        AzWindow* remaining_window = node->Windows[0];

        remaining_window->Collapsed = node->HostWindow->Collapsed;
    }

    DockNodeUpdateVisibleFlag(node);
}

static void Az::DockNodeMoveChildNodes(AzDockNode* dst_node,
                                       AzDockNode* src_node)
{
    AZ_ASSERT(dst_node->Windows.Size == 0);
    dst_node->ChildNodes[0] = src_node->ChildNodes[0];
    dst_node->ChildNodes[1] = src_node->ChildNodes[1];
    if (dst_node->ChildNodes[0])
        dst_node->ChildNodes[0]->ParentNode = dst_node;
    if (dst_node->ChildNodes[1])
        dst_node->ChildNodes[1]->ParentNode = dst_node;
    dst_node->SplitAxis = src_node->SplitAxis;
    dst_node->SizeRef = src_node->SizeRef;
    src_node->ChildNodes[0] = src_node->ChildNodes[1] = NULL;
}

static void Az::DockNodeMoveWindows(AzDockNode* dst_node, AzDockNode* src_node)
{
    AZ_ASSERT(src_node && dst_node && dst_node != src_node);
    AzTabBar* src_tab_bar = src_node->TabBar;
    if (src_tab_bar != NULL)
        AZ_ASSERT(src_node->Windows.Size <= src_node->TabBar->Tabs.Size);

    bool move_tab_bar = (src_tab_bar != NULL) && (dst_node->TabBar == NULL);
    if (move_tab_bar)
    {
        dst_node->TabBar = src_node->TabBar;
        src_node->TabBar = NULL;
    }

    for (AzWindow* window : src_node->Windows)
    {
        window->DockNode = NULL;
        window->DockIsActive = false;
        DockNodeAddWindow(dst_node, window, !move_tab_bar);
    }
    src_node->Windows.clear();

    if (!move_tab_bar && src_node->TabBar)
    {
        if (dst_node->TabBar)
            dst_node->TabBar->SelectedTabId = src_node->TabBar->SelectedTabId;
        DockNodeRemoveTabBar(src_node);
    }
}

static void Az::DockNodeApplyPosSizeToWindows(AzDockNode* node)
{
    for (AzWindow* window : node->Windows)
    {
        SetWindowPos(window, node->Pos, AzCond_Always);

        SetWindowSize(window, node->Size, AzCond_Always);
    }
}

static void Az::DockNodeHideHostWindow(AzDockNode* node)
{
    if (node->HostWindow)
    {
        if (node->HostWindow->DockNodeAsHost == node)
            node->HostWindow->DockNodeAsHost = NULL;
        node->HostWindow = NULL;
    }

    if (node->Windows.Size == 1)
    {
        node->VisibleWindow = node->Windows[0];
        node->Windows[0]->DockIsActive = false;
    }

    if (node->TabBar)
        DockNodeRemoveTabBar(node);
}

struct AzDockNodeTreeInfo
{
    AzDockNode* CentralNode;
    AzDockNode* FirstNodeWithWindows;
    int CountNodesWithWindows;

    AzDockNodeTreeInfo()
    {
        memset(this, 0, sizeof(*this));
    }
};

static void DockNodeFindInfo(AzDockNode* node, AzDockNodeTreeInfo* info)
{
    if (node->Windows.Size > 0)
    {
        if (info->FirstNodeWithWindows == NULL)
            info->FirstNodeWithWindows = node;
        info->CountNodesWithWindows++;
    }
    if (node->IsCentralNode())
    {
        AZ_ASSERT(info->CentralNode == NULL);
        AZ_ASSERT(node->IsLeafNode() && "If you get this assert: please submit .ini file + repro of actions leading to this.");
        info->CentralNode = node;
    }
    if (info->CountNodesWithWindows > 1 && info->CentralNode != NULL)
        return;
    if (node->ChildNodes[0])
        DockNodeFindInfo(node->ChildNodes[0], info);
    if (node->ChildNodes[1])
        DockNodeFindInfo(node->ChildNodes[1], info);
}

static AzWindow* Az::DockNodeFindWindowByID(AzDockNode* node, AzID id)
{
    AZ_ASSERT(id != 0);
    for (AzWindow* window : node->Windows)
        if (window->ID == id)
            return window;
    return NULL;
}

static void Az::DockNodeUpdateFlagsAndCollapse(AzDockNode* node)
{
    AzContext& g = *GAz;
    AZ_ASSERT(node->ParentNode == NULL
              || node->ParentNode->ChildNodes[0] == node
              || node->ParentNode->ChildNodes[1] == node);

    if (node->ParentNode)
        node->SharedFlags = node->ParentNode->SharedFlags
                            & AzDockNodeFlags_SharedFlagsInheritMask_;

    node->HasCentralNodeChild = false;
    if (node->ChildNodes[0])
        DockNodeUpdateFlagsAndCollapse(node->ChildNodes[0]);
    if (node->ChildNodes[1])
        DockNodeUpdateFlagsAndCollapse(node->ChildNodes[1]);

    node->LocalFlagsInWindows = AzDockNodeFlags_None;
    for (int window_n = 0; window_n < node->Windows.Size; window_n++)
    {
        AzWindow* window = node->Windows[window_n];
        AZ_ASSERT(window->DockNode == node);

        bool node_was_active = (node->LastFrameActive + 1 == g.FrameCount);
        bool remove = false;
        remove |= node_was_active
                  && (window->LastFrameActive + 1 < g.FrameCount);
        remove |= node_was_active
                  && (node->WantCloseAll
                      || node->WantCloseTabId == window->TabId)
                  && window->HasCloseButton
                  && !(window->Flags & AzWindowFlags_UnsavedDocument);

        remove |= (window->DockTabWantClose);
        if (remove)
        {
            window->DockTabWantClose = false;
            if (node->Windows.Size == 1 && !node->IsCentralNode())
            {
                DockNodeHideHostWindow(node);
                node->State =
                        AzDockNodeState_HostWindowHiddenBecauseSingleWindow;
                DockNodeRemoveWindow(node, window, node->ID);

                return;
            }
            DockNodeRemoveWindow(node, window, node->ID);
            window_n--;
            continue;
        }

        node->LocalFlagsInWindows |=
                window->WindowClass.DockNodeFlagsOverrideSet;
    }
    node->UpdateMergedFlags();

    AzDockNodeFlags node_flags = node->MergedFlags;
    if (node->WantHiddenTabBarUpdate && node->Windows.Size == 1
        && (node_flags & AzDockNodeFlags_AutoHideTabBar)
        && !node->IsHiddenTabBar())
        node->WantHiddenTabBarToggle = true;
    node->WantHiddenTabBarUpdate = false;

    if (node->WantHiddenTabBarToggle && node->VisibleWindow
        && (node->VisibleWindow->WindowClass.DockNodeFlagsOverrideSet
            & AzDockNodeFlags_HiddenTabBar))
        node->WantHiddenTabBarToggle = false;

    if (node->Windows.Size > 1)
        node->SetLocalFlags(node->LocalFlags & ~AzDockNodeFlags_HiddenTabBar);
    else if (node->WantHiddenTabBarToggle)
        node->SetLocalFlags(node->LocalFlags ^ AzDockNodeFlags_HiddenTabBar);
    node->WantHiddenTabBarToggle = false;

    DockNodeUpdateVisibleFlag(node);
}

static void Az::DockNodeUpdateHasCentralNodeChild(AzDockNode* node)
{
    node->HasCentralNodeChild = false;
    if (node->ChildNodes[0])
        DockNodeUpdateHasCentralNodeChild(node->ChildNodes[0]);
    if (node->ChildNodes[1])
        DockNodeUpdateHasCentralNodeChild(node->ChildNodes[1]);
    if (node->IsRootNode())
    {
        AzDockNode* mark_node = node->CentralNode;
        while (mark_node)
        {
            mark_node->HasCentralNodeChild = true;
            mark_node = mark_node->ParentNode;
        }
    }
}

static void Az::DockNodeUpdateVisibleFlag(AzDockNode* node)
{
    bool is_visible = (node->ParentNode == NULL) ? node->IsDockSpace()
                                                 : node->IsCentralNode();
    is_visible |= (node->Windows.Size > 0);
    is_visible |= (node->ChildNodes[0] && node->ChildNodes[0]->IsVisible);
    is_visible |= (node->ChildNodes[1] && node->ChildNodes[1]->IsVisible);
    node->IsVisible = is_visible;
}

static void Az::DockNodeStartMouseMovingWindow(AzDockNode* node,
                                               AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_ASSERT(node->WantMouseMove == true);
    StartMouseMovingWindow(window);
    g.ActiveIdClickOffset = g.IO.MouseClickedPos[0] - node->Pos;
    g.MovingWindow = window;

    node->WantMouseMove = false;
}

static void Az::DockNodeUpdateForRootNode(AzDockNode* node)
{
    DockNodeUpdateFlagsAndCollapse(node);

    AzDockNodeTreeInfo info;
    DockNodeFindInfo(node, &info);
    node->CentralNode = info.CentralNode;
    node->OnlyNodeWithWindows = (info.CountNodesWithWindows == 1)
                                        ? info.FirstNodeWithWindows
                                        : NULL;
    node->CountNodeWithWindows = info.CountNodesWithWindows;
    if (node->LastFocusedNodeId == 0 && info.FirstNodeWithWindows != NULL)
        node->LastFocusedNodeId = info.FirstNodeWithWindows->ID;

    if (AzDockNode* first_node_with_windows = info.FirstNodeWithWindows)
    {
        node->WindowClass = first_node_with_windows->Windows[0]->WindowClass;
        for (int n = 1; n < first_node_with_windows->Windows.Size; n++)
            if (first_node_with_windows->Windows[n]
                        ->WindowClass.DockingAllowUnclassed
                == false)
            {
                node->WindowClass =
                        first_node_with_windows->Windows[n]->WindowClass;
                break;
            }
    }

    AzDockNode* mark_node = node->CentralNode;
    while (mark_node)
    {
        mark_node->HasCentralNodeChild = true;
        mark_node = mark_node->ParentNode;
    }
}

static void DockNodeSetupHostWindow(AzDockNode* node, AzWindow* host_window)
{
    if (node->HostWindow && node->HostWindow != host_window
        && node->HostWindow->DockNodeAsHost == node)
        node->HostWindow->DockNodeAsHost = NULL;

    host_window->DockNodeAsHost = node;
    node->HostWindow = host_window;
}

static void Az::DockNodeUpdate(AzDockNode* node)
{
    AzContext& g = *GAz;
    AZ_ASSERT(node->LastFrameActive != g.FrameCount);
    node->LastFrameAlive = g.FrameCount;
    node->IsBgDrawnThisFrame = false;

    node->CentralNode = node->OnlyNodeWithWindows = NULL;
    if (node->IsRootNode())
        DockNodeUpdateForRootNode(node);

    if (node->TabBar && node->IsNoTabBar())
        DockNodeRemoveTabBar(node);

    bool want_to_hide_host_window = false;
    if (node->IsFloatingNode())
    {
        if (node->Windows.Size <= 1 && node->IsLeafNode())
            if (!g.IO.ConfigDockingAlwaysTabBar
                && (node->Windows.Size == 0
                    || !node->Windows[0]->WindowClass.DockingAlwaysTabBar))
                want_to_hide_host_window = true;
        if (node->CountNodeWithWindows == 0)
            want_to_hide_host_window = true;
    }
    if (want_to_hide_host_window)
    {
        if (node->Windows.Size == 1)
        {
            AzWindow* single_window = node->Windows[0];
            node->Pos = single_window->Pos;
            node->Size = single_window->SizeFull;
            node->AuthorityForPos = node->AuthorityForSize =
                    node->AuthorityForViewport = AzDataAuthority_Window;

            if (node->HostWindow && g.NavWindow == node->HostWindow)
                FocusWindow(single_window);
            if (node->HostWindow)
            {
                AZ_DEBUG_LOG_VIEWPORT(
                        "[viewport] Node %08X transfer Viewport %08X->%08X to "
                        "Window '%s'\n",
                        node->ID,
                        node->HostWindow->Viewport->ID,
                        single_window->ID,
                        single_window->Name);
                single_window->Viewport = node->HostWindow->Viewport;
                single_window->ViewportId = node->HostWindow->ViewportId;
                if (node->HostWindow->ViewportOwned)
                {
                    single_window->Viewport->ID = single_window->ID;
                    single_window->Viewport->Window = single_window;
                    single_window->ViewportOwned = true;
                }
            }
            node->RefViewportId = single_window->ViewportId;
        }

        DockNodeHideHostWindow(node);
        node->State = AzDockNodeState_HostWindowHiddenBecauseSingleWindow;
        node->WantCloseAll = false;
        node->WantCloseTabId = 0;
        node->HasCloseButton = node->HasWindowMenuButton = false;
        node->LastFrameActive = g.FrameCount;

        if (node->WantMouseMove && node->Windows.Size == 1)
            DockNodeStartMouseMovingWindow(node, node->Windows[0]);
        return;
    }

    if (node->IsVisible && node->HostWindow == NULL && node->IsFloatingNode()
        && node->IsLeafNode())
    {
        AZ_ASSERT(node->Windows.Size > 0);
        AzWindow* ref_window = NULL;
        if (node->SelectedTabId != 0)

            ref_window = DockNodeFindWindowByID(node, node->SelectedTabId);
        if (ref_window == NULL)
            ref_window = node->Windows[0];
        if (ref_window->AutoFitFramesX > 0 || ref_window->AutoFitFramesY > 0)
        {
            node->State =
                    AzDockNodeState_HostWindowHiddenBecauseWindowsAreResizing;
            return;
        }
    }

    const AzDockNodeFlags node_flags = node->MergedFlags;

    node->HasWindowMenuButton = (node->Windows.Size > 0)
                                && (node_flags
                                    & AzDockNodeFlags_NoWindowMenuButton)
                                           == 0;
    node->HasCloseButton = false;
    for (AzWindow* window : node->Windows)
    {
        node->HasCloseButton |= window->HasCloseButton;
        window->DockIsActive = (node->Windows.Size > 1);
    }
    if (node_flags & AzDockNodeFlags_NoCloseButton)
        node->HasCloseButton = false;

    AzWindow* host_window = NULL;
    bool beginned_into_host_window = false;
    if (node->IsDockSpace())
    {
        AZ_ASSERT(node->HostWindow);
        host_window = node->HostWindow;
    } else
    {
        if (node->IsRootNode() && node->IsVisible)
        {
            AzWindow* ref_window = (node->Windows.Size > 0) ? node->Windows[0]
                                                            : NULL;

            if (node->AuthorityForPos == AzDataAuthority_Window && ref_window)
                SetNextWindowPos(ref_window->Pos);
            else if (node->AuthorityForPos == AzDataAuthority_DockNode)
                SetNextWindowPos(node->Pos);

            if (node->AuthorityForSize == AzDataAuthority_Window && ref_window)
                SetNextWindowSize(ref_window->SizeFull);
            else if (node->AuthorityForSize == AzDataAuthority_DockNode)
                SetNextWindowSize(node->Size);

            if (node->AuthorityForSize == AzDataAuthority_Window && ref_window)
                SetNextWindowCollapsed(ref_window->Collapsed);

            if (node->AuthorityForViewport == AzDataAuthority_Window
                && ref_window)
                SetNextWindowViewport(ref_window->ViewportId);
            else if (node->AuthorityForViewport == AzDataAuthority_Window
                     && node->RefViewportId != 0)
                SetNextWindowViewport(node->RefViewportId);

            SetNextWindowClass(&node->WindowClass);

            char window_label[20];
            DockNodeGetHostWindowTitle(node,
                                       window_label,
                                       AZ_ARRAYSIZE(window_label));
            AzWindowFlags window_flags = AzWindowFlags_NoScrollbar
                                         | AzWindowFlags_NoScrollWithMouse
                                         | AzWindowFlags_DockNodeHost;
            window_flags |= AzWindowFlags_NoFocusOnAppearing;
            window_flags |= AzWindowFlags_NoSavedSettings
                            | AzWindowFlags_NoNavFocus
                            | AzWindowFlags_NoCollapse;
            window_flags |= AzWindowFlags_NoTitleBar;

            SetNextWindowBgAlpha(0.0f);

            PushStyleVar(AzStyleVar_WindowPadding, AzVec2(0, 0));
            Begin(window_label, NULL, window_flags);
            PopStyleVar();
            beginned_into_host_window = true;

            host_window = g.CurrentWindow;
            DockNodeSetupHostWindow(node, host_window);
            host_window->DC.CursorPos = host_window->Pos;
            node->Pos = host_window->Pos;
            node->Size = host_window->Size;

            if (node->HostWindow->Appearing)
                BringWindowToDisplayFront(node->HostWindow);

            node->AuthorityForPos = node->AuthorityForSize =
                    node->AuthorityForViewport = AzDataAuthority_Auto;
        } else if (node->ParentNode)
        {
            node->HostWindow = host_window = node->ParentNode->HostWindow;
            node->AuthorityForPos = node->AuthorityForSize =
                    node->AuthorityForViewport = AzDataAuthority_Auto;
        }
        if (node->WantMouseMove && node->HostWindow)
            DockNodeStartMouseMovingWindow(node, node->HostWindow);
    }
    node->RefViewportId = 0;

    if (node->IsSplitNode())
        AZ_ASSERT(node->TabBar == NULL);
    if (node->IsRootNode())
        if (AzWindow* p_window = g.NavWindow ? g.NavWindow->RootWindow : NULL)
            while (p_window != NULL && p_window->DockNode != NULL)
            {
                AzDockNode* p_node = DockNodeGetRootNode(p_window->DockNode);
                if (p_node == node)
                {
                    node->LastFocusedNodeId = p_window->DockNode->ID;

                    break;
                }
                p_window = p_node->HostWindow ? p_node->HostWindow->RootWindow
                                              : NULL;
            }

    AzDockNode* central_node = node->CentralNode;
    const bool central_node_hole = node->IsRootNode() && host_window
                                   && (node_flags
                                       & AzDockNodeFlags_PassthruCentralNode)
                                              != 0
                                   && central_node != NULL
                                   && central_node->IsEmpty();
    bool central_node_hole_register_hit_test_hole = central_node_hole;
    if (central_node_hole)
        if (const AzPayload* payload = Az::GetDragDropPayload())
            if (payload->IsDataType(AZ_PAYLOAD_TYPE_WINDOW)
                && DockNodeIsDropAllowed(host_window,
                                         *(AzWindow**)payload->Data))
                central_node_hole_register_hit_test_hole = false;
    if (central_node_hole_register_hit_test_hole)
    {
        AZ_ASSERT(node->IsDockSpace());

        AzDockNode* root_node = DockNodeGetRootNode(central_node);
        AzRect root_rect(root_node->Pos, root_node->Pos + root_node->Size);
        AzRect hole_rect(central_node->Pos,
                         central_node->Pos + central_node->Size);
        if (hole_rect.Min.x > root_rect.Min.x)
        {
            hole_rect.Min.x += g.WindowsBorderHoverPadding;
        }
        if (hole_rect.Max.x < root_rect.Max.x)
        {
            hole_rect.Max.x -= g.WindowsBorderHoverPadding;
        }
        if (hole_rect.Min.y > root_rect.Min.y)
        {
            hole_rect.Min.y += g.WindowsBorderHoverPadding;
        }
        if (hole_rect.Max.y < root_rect.Max.y)
        {
            hole_rect.Max.y -= g.WindowsBorderHoverPadding;
        }

        if (central_node_hole && !hole_rect.IsInverted())
        {
            SetWindowHitTestHole(host_window,
                                 hole_rect.Min,
                                 hole_rect.Max - hole_rect.Min);
            if (host_window->ParentWindow)
                SetWindowHitTestHole(host_window->ParentWindow,
                                     hole_rect.Min,
                                     hole_rect.Max - hole_rect.Min);
        }
    }

    if (node->IsRootNode() && host_window)
    {
        DockNodeTreeUpdatePosSize(node, host_window->Pos, host_window->Size);
        PushStyleColor(AzCol_Separator, g.Style.Colors[AzCol_Border]);
        PushStyleColor(AzCol_SeparatorActive,
                       g.Style.Colors[AzCol_ResizeGripActive]);
        PushStyleColor(AzCol_SeparatorHovered,
                       g.Style.Colors[AzCol_ResizeGripHovered]);
        DockNodeTreeUpdateSplitter(node);
        PopStyleColor(3);
    }

    if (host_window && node->IsEmpty() && node->IsVisible)
    {
        host_window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_BG);
        node->LastBgColor = (node_flags & AzDockNodeFlags_PassthruCentralNode)
                                    ? 0
                                    : GetColorU32(AzCol_DockingEmptyBg);
        if (node->LastBgColor != 0)
            host_window->DrawList->AddRectFilled(node->Pos,
                                                 node->Pos + node->Size,
                                                 node->LastBgColor);
        node->IsBgDrawnThisFrame = true;
    }

    const bool render_dockspace_bg = node->IsRootNode() && host_window
                                     && (node_flags
                                         & AzDockNodeFlags_PassthruCentralNode)
                                                != 0;
    if (render_dockspace_bg && node->IsVisible)
    {
        host_window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_BG);
        if (central_node_hole)
            RenderRectFilledWithHole(host_window->DrawList,
                                     node->Rect(),
                                     central_node->Rect(),
                                     GetColorU32(AzCol_WindowBg),
                                     0.0f);
        else
            host_window->DrawList->AddRectFilled(node->Pos,
                                                 node->Pos + node->Size,
                                                 GetColorU32(AzCol_WindowBg),
                                                 0.0f);
    }

    if (host_window)
        host_window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_FG);
    if (host_window && node->Windows.Size > 0)
    {
        DockNodeUpdateTabBar(node, host_window);
    } else
    {
        node->WantCloseAll = false;
        node->WantCloseTabId = 0;
        node->IsFocused = false;
    }
    if (node->TabBar && node->TabBar->SelectedTabId)
        node->SelectedTabId = node->TabBar->SelectedTabId;
    else if (node->Windows.Size > 0)
        node->SelectedTabId = node->Windows[0]->TabId;

    if (host_window && node->IsVisible)
        if (node->IsRootNode()
            && (g.MovingWindow == NULL
                || g.MovingWindow->RootWindowDockTree != host_window))
            BeginDockableDragDropTarget(host_window);

    node->LastFrameActive = g.FrameCount;

    if (host_window)
    {
        if (node->ChildNodes[0])
            DockNodeUpdate(node->ChildNodes[0]);
        if (node->ChildNodes[1])
            DockNodeUpdate(node->ChildNodes[1]);

        if (node->IsRootNode())
            RenderWindowOuterBorders(host_window);
    }

    if (beginned_into_host_window)
        End();
}

static int AZ_CDECL TabItemComparerByDockOrder(const void* lhs, const void* rhs)
{
    AzWindow* a = ((const AzTabItem*)lhs)->Window;
    AzWindow* b = ((const AzTabItem*)rhs)->Window;
    if (int d = ((a->DockOrder == -1) ? INT_MAX : a->DockOrder)
                - ((b->DockOrder == -1) ? INT_MAX : b->DockOrder))
        return d;
    return (a->BeginOrderWithinContext - b->BeginOrderWithinContext);
}

void Az::DockNodeWindowMenuHandler_Default(AzContext* ctx,
                                           AzDockNode* node,
                                           AzTabBar* tab_bar)
{
    AZ_UNUSED(ctx);
    if (tab_bar->Tabs.Size == 1)
    {
        if (MenuItem(LocalizeGetMsg(AzLocKey_DockingHideTabBar),
                     NULL,
                     node->IsHiddenTabBar()))
            node->WantHiddenTabBarToggle = true;
    } else
    {
        for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
        {
            AzTabItem* tab = &tab_bar->Tabs[tab_n];
            if (tab->Flags & AzTabItemFlags_Button)
                continue;
            if (Selectable(TabBarGetTabName(tab_bar, tab),
                           tab->ID == tab_bar->SelectedTabId))
                TabBarQueueFocus(tab_bar, tab);
            SameLine();
            Text("   ");
        }
    }
}

static void Az::DockNodeWindowMenuUpdate(AzDockNode* node, AzTabBar* tab_bar)
{
    AzContext& g = *GAz;
    if (g.Style.WindowMenuButtonPosition == AzDir_Left)
        SetNextWindowPos(AzVec2(node->Pos.x, node->Pos.y + GetFrameHeight()),
                         AzCond_Always,
                         AzVec2(0.0f, 0.0f));
    else
        SetNextWindowPos(AzVec2(node->Pos.x + node->Size.x,
                                node->Pos.y + GetFrameHeight()),
                         AzCond_Always,
                         AzVec2(1.0f, 0.0f));
    if (BeginPopup("#WindowMenu"))
    {
        node->IsFocused = true;
        g.DockNodeWindowMenuHandler(&g, node, tab_bar);
        EndPopup();
    }
}

bool Az::DockNodeBeginAmendTabBar(AzDockNode* node)
{
    if (node->TabBar == NULL || node->HostWindow == NULL)
        return false;
    if (node->MergedFlags & AzDockNodeFlags_KeepAliveOnly)
        return false;
    if (node->TabBar->ID == 0)
        return false;
    Begin(node->HostWindow->Name);
    PushOverrideID(node->ID);
    bool ret = BeginTabBarEx(node->TabBar,
                             node->TabBar->BarRect,
                             node->TabBar->Flags);
    AZ_UNUSED(ret);
    AZ_ASSERT(ret);
    return true;
}

void Az::DockNodeEndAmendTabBar()
{
    EndTabBar();
    PopID();
    End();
}

static bool IsDockNodeTitleBarHighlighted(AzDockNode* node,
                                          AzDockNode* root_node)
{
    AzContext& g = *GAz;
    if (g.NavWindowingTarget)
        return (g.NavWindowingTarget->DockNode == node);

    if (g.NavWindow && root_node->LastFocusedNodeId == node->ID)
    {
        AzWindow* parent_window = g.NavWindow->RootWindow;
        while (parent_window->Flags & AzWindowFlags_ChildMenu)
            parent_window = parent_window->ParentWindow->RootWindow;
        AzDockNode* start_parent_node = parent_window->DockNodeAsHost
                                                ? parent_window->DockNodeAsHost
                                                : parent_window->DockNode;
        for (AzDockNode* parent_node = start_parent_node; parent_node != NULL;
             parent_node =
                     parent_node->HostWindow
                             ? parent_node->HostWindow->RootWindow->DockNode
                             : NULL)
            if ((parent_node = Az::DockNodeGetRootNode(parent_node))
                == root_node)
                return true;
    }
    return false;
}

static void Az::DockNodeUpdateTabBar(AzDockNode* node, AzWindow* host_window)
{
    AzContext& g = *GAz;
    AzStyle& style = g.Style;

    const bool node_was_active = (node->LastFrameActive + 1 == g.FrameCount);
    const bool closed_all = node->WantCloseAll && node_was_active;
    const AzID closed_one = node->WantCloseTabId && node_was_active;
    node->WantCloseAll = false;
    node->WantCloseTabId = 0;

    bool is_focused = false;
    AzDockNode* root_node = DockNodeGetRootNode(node);
    if (IsDockNodeTitleBarHighlighted(node, root_node))
        is_focused = true;

    if (node->IsHiddenTabBar() || node->IsNoTabBar())
    {
        node->VisibleWindow = (node->Windows.Size > 0) ? node->Windows[0]
                                                       : NULL;
        node->IsFocused = is_focused;
        if (is_focused)
            node->LastFrameFocused = g.FrameCount;
        if (node->VisibleWindow)
        {
            if (is_focused || root_node->VisibleWindow == NULL)
                root_node->VisibleWindow = node->VisibleWindow;
            if (node->TabBar)
                node->TabBar->VisibleTabId = node->VisibleWindow->TabId;
        }
        return;
    }

    bool backup_skip_item = host_window->SkipItems;
    if (!node->IsDockSpace())
    {
        host_window->SkipItems = false;
        host_window->DC.NavLayerCurrent = AzNavLayer_Menu;
    }

    PushOverrideID(node->ID);
    AzTabBar* tab_bar = node->TabBar;
    bool tab_bar_is_recreated = (tab_bar == NULL);

    if (tab_bar == NULL)
    {
        DockNodeAddTabBar(node);
        tab_bar = node->TabBar;
    }

    AzID focus_tab_id = 0;
    node->IsFocused = is_focused;

    const AzDockNodeFlags node_flags = node->MergedFlags;
    const bool has_window_menu_button =
            (node_flags & AzDockNodeFlags_NoWindowMenuButton) == 0
            && (style.WindowMenuButtonPosition != AzDir_None);

    if (has_window_menu_button && IsPopupOpen("#WindowMenu"))
    {
        AzID next_selected_tab_id = tab_bar->NextSelectedTabId;
        DockNodeWindowMenuUpdate(node, tab_bar);
        if (tab_bar->NextSelectedTabId != 0
            && tab_bar->NextSelectedTabId != next_selected_tab_id)
            focus_tab_id = tab_bar->NextSelectedTabId;
        is_focused |= node->IsFocused;
    }

    AzRect title_bar_rect, tab_bar_rect;
    AzVec2 window_menu_button_pos;
    AzVec2 close_button_pos;
    DockNodeCalcTabBarLayout(node,
                             &title_bar_rect,
                             &tab_bar_rect,
                             &window_menu_button_pos,
                             &close_button_pos);

    const int tabs_count_old = tab_bar->Tabs.Size;
    for (int window_n = 0; window_n < node->Windows.Size; window_n++)
    {
        AzWindow* window = node->Windows[window_n];
        if (TabBarFindTabByID(tab_bar, window->TabId) == NULL)
            TabBarAddTab(tab_bar, AzTabItemFlags_Unsorted, window);
    }

    if (is_focused)
        node->LastFrameFocused = g.FrameCount;
    AzU32 title_bar_col = GetColorU32(host_window->Collapsed
                                              ? AzCol_TitleBgCollapsed
                                      : is_focused ? AzCol_TitleBgActive
                                                   : AzCol_TitleBg);
    AzDrawFlags rounding_flags =
            CalcRoundingFlagsForRectInRect(title_bar_rect,
                                           host_window->Rect(),
                                           g.Style.DockingSeparatorSize);
    host_window->DrawList->AddRectFilled(title_bar_rect.Min,
                                         title_bar_rect.Max,
                                         title_bar_col,
                                         host_window->WindowRounding,
                                         rounding_flags);

    if (has_window_menu_button)
    {
        if (CollapseButton(host_window->GetID("#COLLAPSE"),
                           window_menu_button_pos,
                           node))
            OpenPopup("#WindowMenu");
        if (IsItemActive())
            focus_tab_id = tab_bar->SelectedTabId;
        if (IsItemHovered(AzHoveredFlags_ForTooltip
                          | AzHoveredFlags_DelayNormal)
            && g.HoveredIdTimer > 0.5f)
            SetTooltip("%s",
                       LocalizeGetMsg(AzLocKey_DockingDragToUndockOrMoveNode));
    }

    int tabs_unsorted_start = tab_bar->Tabs.Size;
    for (int tab_n = tab_bar->Tabs.Size - 1;
         tab_n >= 0 && (tab_bar->Tabs[tab_n].Flags & AzTabItemFlags_Unsorted);
         tab_n--)
    {
        tab_bar->Tabs[tab_n].Flags &= ~AzTabItemFlags_Unsorted;
        tabs_unsorted_start = tab_n;
    }
    if (tab_bar->Tabs.Size > tabs_unsorted_start)
    {
        AZ_DEBUG_LOG_DOCKING(
                "[docking] In node 0x%08X: %d new appearing tabs:%s\n",
                node->ID,
                tab_bar->Tabs.Size - tabs_unsorted_start,
                (tab_bar->Tabs.Size > tabs_unsorted_start + 1) ? " (will sort)"
                                                               : "");
        for (int tab_n = tabs_unsorted_start; tab_n < tab_bar->Tabs.Size;
             tab_n++)
        {
            AzTabItem* tab = &tab_bar->Tabs[tab_n];
            AZ_UNUSED(tab);
            AZ_DEBUG_LOG_DOCKING("[docking] - Tab 0x%08X '%s' Order %d\n",
                                 tab->ID,
                                 TabBarGetTabName(tab_bar, tab),
                                 tab->Window ? tab->Window->DockOrder : -1);
        }
        AZ_DEBUG_LOG_DOCKING(
                "[docking] SelectedTabId = 0x%08X, NavWindow->TabId = 0x%08X\n",
                node->SelectedTabId,
                g.NavWindow ? g.NavWindow->TabId : -1);
        if (tab_bar->Tabs.Size > tabs_unsorted_start + 1)
            AzQsort(tab_bar->Tabs.Data + tabs_unsorted_start,
                    tab_bar->Tabs.Size - tabs_unsorted_start,
                    sizeof(AzTabItem),
                    TabItemComparerByDockOrder);
    }

    if (g.NavWindow && g.NavWindow->RootWindow->DockNode == node)
        tab_bar->SelectedTabId = g.NavWindow->RootWindow->TabId;

    if (tab_bar_is_recreated
        && TabBarFindTabByID(tab_bar, node->SelectedTabId) != NULL)
        tab_bar->SelectedTabId = tab_bar->NextSelectedTabId =
                node->SelectedTabId;
    else if (tab_bar->Tabs.Size > tabs_count_old)
        tab_bar->SelectedTabId = tab_bar->NextSelectedTabId =
                tab_bar->Tabs.back().Window->TabId;

    AzTabBarFlags tab_bar_flags = AzTabBarFlags_Reorderable
                                  | AzTabBarFlags_AutoSelectNewTabs;

    tab_bar_flags |= AzTabBarFlags_SaveSettings | AzTabBarFlags_DockNode;

    tab_bar_flags |= AzTabBarFlags_DrawSelectedOverline;
    if (!host_window->Collapsed && is_focused)
        tab_bar_flags |= AzTabBarFlags_IsFocused;
    tab_bar->ID = GetID("#TabBar");
    tab_bar->SeparatorMinX = node->Pos.x + host_window->WindowBorderSize;

    tab_bar->SeparatorMaxX = node->Pos.x + node->Size.x
                             - host_window->WindowBorderSize;
    BeginTabBarEx(tab_bar, tab_bar_rect, tab_bar_flags);

    AzVec4 backup_style_cols[AzWindowDockStyleCol_COUNT];
    for (int color_n = 0; color_n < AzWindowDockStyleCol_COUNT; color_n++)
        backup_style_cols[color_n] =
                g.Style.Colors[GWindowDockStyleColors[color_n]];

    node->VisibleWindow = NULL;
    for (int window_n = 0; window_n < node->Windows.Size; window_n++)
    {
        AzWindow* window = node->Windows[window_n];
        if ((closed_all || closed_one == window->TabId)
            && window->HasCloseButton
            && !(window->Flags & AzWindowFlags_UnsavedDocument))
            continue;
        if (window->LastFrameActive + 1 >= g.FrameCount || !node_was_active)
        {
            AzTabItemFlags tab_item_flags = 0;
            tab_item_flags |= window->WindowClass.TabItemFlagsOverrideSet;
            if (window->Flags & AzWindowFlags_UnsavedDocument)
                tab_item_flags |= AzTabItemFlags_UnsavedDocument;
            if (tab_bar->Flags & AzTabBarFlags_NoCloseWithMiddleMouseButton)
                tab_item_flags |= AzTabItemFlags_NoCloseWithMiddleMouseButton;

            for (int color_n = 0; color_n < AzWindowDockStyleCol_COUNT;
                 color_n++)
                g.Style.Colors[GWindowDockStyleColors[color_n]] =
                        ColorConvertU32ToFloat4(
                                window->DockStyle.Colors[color_n]);

            bool tab_open = true;
            TabItemEx(tab_bar,
                      window->Name,
                      window->HasCloseButton ? &tab_open : NULL,
                      tab_item_flags,
                      window);
            if (!tab_open)
                node->WantCloseTabId = window->TabId;
            if (tab_bar->VisibleTabId == window->TabId)
                node->VisibleWindow = window;

            window->DC.DockTabItemStatusFlags = g.LastItemData.StatusFlags;
            window->DC.DockTabItemRect = g.LastItemData.Rect;

            if (g.NavWindow && g.NavWindow->RootWindow == window
                && (window->DC.NavLayersActiveMask & (1 << AzNavLayer_Menu))
                           == 0)
                host_window->NavLastIds[1] = window->TabId;
        }
    }

    for (int color_n = 0; color_n < AzWindowDockStyleCol_COUNT; color_n++)
        g.Style.Colors[GWindowDockStyleColors[color_n]] =
                backup_style_cols[color_n];

    if (node->VisibleWindow)
        if (is_focused || root_node->VisibleWindow == NULL)
            root_node->VisibleWindow = node->VisibleWindow;

    const bool close_button_is_enabled = node->HasCloseButton
                                         && node->VisibleWindow
                                         && node->VisibleWindow->HasCloseButton;
    const bool close_button_is_visible = node->HasCloseButton;

    if (close_button_is_visible)
    {
        if (!close_button_is_enabled)
        {
            PushItemFlag(AzItemFlags_Disabled, true);
            PushStyleColor(AzCol_Text,
                           style.Colors[AzCol_Text]
                                   * AzVec4(1.0f, 1.0f, 1.0f, 0.4f));
        }
        if (CloseButton(host_window->GetID("#CLOSE"), close_button_pos))
        {
            node->WantCloseAll = true;
            for (int n = 0; n < tab_bar->Tabs.Size; n++)
                TabBarCloseTab(tab_bar, &tab_bar->Tabs[n]);
        }

        if (!close_button_is_enabled)
        {
            PopStyleColor();
            PopItemFlag();
        }
    }

    AzID title_bar_id = host_window->GetID("#TITLEBAR");
    if (g.HoveredId == 0 || g.HoveredId == title_bar_id
        || g.ActiveId == title_bar_id)
    {
        bool held;
        KeepAliveID(title_bar_id);
        ButtonBehavior(title_bar_rect,
                       title_bar_id,
                       NULL,
                       &held,
                       AzButtonFlags_AllowOverlap);
        if (g.HoveredId == title_bar_id)
        {
            g.LastItemData.ID = title_bar_id;
        }
        if (held)
        {
            if (IsMouseClicked(0))
                focus_tab_id = tab_bar->SelectedTabId;

            if (AzTabItem* tab = TabBarFindTabByID(tab_bar,
                                                   tab_bar->SelectedTabId))
                StartMouseMovingWindowOrNode(tab->Window ? tab->Window
                                                         : node->HostWindow,
                                             node,
                                             false);
        }
    }

    if (tab_bar->NextSelectedTabId)
        focus_tab_id = tab_bar->NextSelectedTabId;

    if (focus_tab_id != 0)
        if (AzTabItem* tab = TabBarFindTabByID(tab_bar, focus_tab_id))
            if (tab->Window)
            {
                FocusWindow(tab->Window);
                NavInitWindow(tab->Window, false);
            }

    EndTabBar();
    PopID();

    if (!node->IsDockSpace())
    {
        host_window->DC.NavLayerCurrent = AzNavLayer_Main;
        host_window->SkipItems = backup_skip_item;
    }
}

static void Az::DockNodeAddTabBar(AzDockNode* node)
{
    AZ_ASSERT(node->TabBar == NULL);
    node->TabBar = AZ_NEW(AzTabBar);
}

static void Az::DockNodeRemoveTabBar(AzDockNode* node)
{
    if (node->TabBar == NULL)
        return;
    AZ_DELETE(node->TabBar);
    node->TabBar = NULL;
}

static bool DockNodeIsDropAllowedOne(AzWindow* payload, AzWindow* host_window)
{
    if (host_window->DockNodeAsHost
        && host_window->DockNodeAsHost->IsDockSpace()
        && payload->BeginOrderWithinContext
                   < host_window->BeginOrderWithinContext)
        return false;

    AzWindowClass* host_class =
            host_window->DockNodeAsHost
                    ? &host_window->DockNodeAsHost->WindowClass
                    : &host_window->WindowClass;
    AzWindowClass* payload_class = &payload->WindowClass;
    if (host_class->ClassId != payload_class->ClassId)
    {
        bool pass = false;
        if (host_class->ClassId != 0 && host_class->DockingAllowUnclassed
            && payload_class->ClassId == 0)
            pass = true;
        if (payload_class->ClassId != 0 && payload_class->DockingAllowUnclassed
            && host_class->ClassId == 0)
            pass = true;
        if (!pass)
            return false;
    }

    AzContext& g = *GAz;
    for (int i = g.OpenPopupStack.Size - 1; i >= 0; i--)
        if (AzWindow* popup_window = g.OpenPopupStack[i].Window)
            if (Az::IsWindowWithinBeginStackOf(payload, popup_window))

                return false;

    return true;
}

static bool Az::DockNodeIsDropAllowed(AzWindow* host_window,
                                      AzWindow* root_payload)
{
    if (root_payload->DockNodeAsHost
        && root_payload->DockNodeAsHost->IsSplitNode())

        return true;

    const int payload_count =
            root_payload->DockNodeAsHost
                    ? root_payload->DockNodeAsHost->Windows.Size
                    : 1;
    for (int payload_n = 0; payload_n < payload_count; payload_n++)
    {
        AzWindow* payload =
                root_payload->DockNodeAsHost
                        ? root_payload->DockNodeAsHost->Windows[payload_n]
                        : root_payload;
        if (DockNodeIsDropAllowedOne(payload, host_window))
            return true;
    }
    return false;
}

static void Az::DockNodeCalcTabBarLayout(const AzDockNode* node,
                                         AzRect* out_title_rect,
                                         AzRect* out_tab_bar_rect,
                                         AzVec2* out_window_menu_button_pos,
                                         AzVec2* out_close_button_pos)
{
    AzContext& g = *GAz;
    AzStyle& style = g.Style;

    AzRect r = AzRect(node->Pos.x,
                      node->Pos.y,
                      node->Pos.x + node->Size.x,
                      node->Pos.y + g.FontSize + g.Style.FramePadding.y * 2.0f);
    if (out_title_rect)
    {
        *out_title_rect = r;
    }

    r.Min.x += style.WindowBorderSize;
    r.Max.x -= style.WindowBorderSize;

    float button_sz = g.FontSize;
    r.Min.x += style.FramePadding.x;
    r.Max.x -= style.FramePadding.x;
    AzVec2 window_menu_button_pos = AzVec2(r.Min.x,
                                           r.Min.y + style.FramePadding.y);
    if (node->HasCloseButton)
    {
        if (out_close_button_pos)
            *out_close_button_pos = AzVec2(r.Max.x - button_sz,
                                           r.Min.y + style.FramePadding.y);
        r.Max.x -= button_sz + style.ItemInnerSpacing.x;
    }
    if (node->HasWindowMenuButton
        && style.WindowMenuButtonPosition == AzDir_Left)
    {
        r.Min.x += button_sz + style.ItemInnerSpacing.x;
    } else if (node->HasWindowMenuButton
               && style.WindowMenuButtonPosition == AzDir_Right)
    {
        window_menu_button_pos = AzVec2(r.Max.x - button_sz,
                                        r.Min.y + style.FramePadding.y);
        r.Max.x -= button_sz + style.ItemInnerSpacing.x;
    }
    if (out_tab_bar_rect)
    {
        *out_tab_bar_rect = r;
    }
    if (out_window_menu_button_pos)
    {
        *out_window_menu_button_pos = window_menu_button_pos;
    }
}

void Az::DockNodeCalcSplitRects(AzVec2& pos_old,
                                AzVec2& size_old,
                                AzVec2& pos_new,
                                AzVec2& size_new,
                                AzDir dir,
                                AzVec2 size_new_desired)
{
    AzContext& g = *GAz;
    const float dock_spacing = g.Style.ItemInnerSpacing.x;
    const AzAxis axis = (dir == AzDir_Left || dir == AzDir_Right) ? AzAxis_X
                                                                  : AzAxis_Y;
    pos_new[axis ^ 1] = pos_old[axis ^ 1];
    size_new[axis ^ 1] = size_old[axis ^ 1];

    const float w_avail = size_old[axis] - dock_spacing;
    if (size_new_desired[axis] > 0.0f
        && size_new_desired[axis] <= w_avail * 0.5f)
    {
        size_new[axis] = size_new_desired[axis];
        size_old[axis] = AZ_TRUNC(w_avail - size_new[axis]);
    } else
    {
        size_new[axis] = AZ_TRUNC(w_avail * 0.5f);
        size_old[axis] = AZ_TRUNC(w_avail - size_new[axis]);
    }

    if (dir == AzDir_Right || dir == AzDir_Down)
    {
        pos_new[axis] = pos_old[axis] + size_old[axis] + dock_spacing;
    } else if (dir == AzDir_Left || dir == AzDir_Up)
    {
        pos_new[axis] = pos_old[axis];
        pos_old[axis] = pos_new[axis] + size_new[axis] + dock_spacing;
    }
}

bool Az::DockNodeCalcDropRectsAndTestMousePos(const AzRect& parent,
                                              AzDir dir,
                                              AzRect& out_r,
                                              bool outer_docking,
                                              AzVec2* test_mouse_pos)
{
    AzContext& g = *GAz;

    const float parent_smaller_axis = AzMin(parent.GetWidth(),
                                            parent.GetHeight());
    const float hs_for_central_nodes = AzMin(g.FontSize * 1.5f,
                                             AzMax(g.FontSize * 0.5f,
                                                   parent_smaller_axis / 8.0f));
    float hs_w;
    float hs_h;
    AzVec2 off;
    if (outer_docking)
    {
        hs_w = AzTrunc(hs_for_central_nodes * 1.50f);
        hs_h = AzTrunc(hs_for_central_nodes * 0.80f);
        off = AzTrunc(AzVec2(parent.GetWidth() * 0.5f - hs_h,
                             parent.GetHeight() * 0.5f - hs_h));
    } else
    {
        hs_w = AzTrunc(hs_for_central_nodes);
        hs_h = AzTrunc(hs_for_central_nodes * 0.90f);
        off = AzTrunc(AzVec2(hs_w * 2.40f, hs_w * 2.40f));
    }

    AzVec2 c = AzTrunc(parent.GetCenter());
    if (dir == AzDir_None)
    {
        out_r = AzRect(c.x - hs_w, c.y - hs_w, c.x + hs_w, c.y + hs_w);
    } else if (dir == AzDir_Up)
    {
        out_r = AzRect(c.x - hs_w,
                       c.y - off.y - hs_h,
                       c.x + hs_w,
                       c.y - off.y + hs_h);
    } else if (dir == AzDir_Down)
    {
        out_r = AzRect(c.x - hs_w,
                       c.y + off.y - hs_h,
                       c.x + hs_w,
                       c.y + off.y + hs_h);
    } else if (dir == AzDir_Left)
    {
        out_r = AzRect(c.x - off.x - hs_h,
                       c.y - hs_w,
                       c.x - off.x + hs_h,
                       c.y + hs_w);
    } else if (dir == AzDir_Right)
    {
        out_r = AzRect(c.x + off.x - hs_h,
                       c.y - hs_w,
                       c.x + off.x + hs_h,
                       c.y + hs_w);
    }

    if (test_mouse_pos == NULL)
        return false;

    AzRect hit_r = out_r;
    if (!outer_docking)
    {
        hit_r.Expand(AzTrunc(hs_w * 0.30f));
        AzVec2 mouse_delta = (*test_mouse_pos - c);
        float mouse_delta_len2 = AzLengthSqr(mouse_delta);
        float r_threshold_center = hs_w * 1.4f;
        float r_threshold_sides = hs_w * (1.4f + 1.2f);
        if (mouse_delta_len2 < r_threshold_center * r_threshold_center)
            return (dir == AzDir_None);
        if (mouse_delta_len2 < r_threshold_sides * r_threshold_sides)
            return (dir
                    == AzGetDirQuadrantFromDelta(mouse_delta.x, mouse_delta.y));
    }
    return hit_r.Contains(*test_mouse_pos);
}

static void Az::DockNodePreviewDockSetup(AzWindow* host_window,
                                         AzDockNode* host_node,
                                         AzWindow* payload_window,
                                         AzDockNode* payload_node,
                                         AzDockPreviewData* data,
                                         bool is_explicit_target,
                                         bool is_outer_docking)
{
    AzContext& g = *GAz;

    if (payload_node == NULL)
        payload_node = payload_window->DockNodeAsHost;
    AzDockNode* ref_node_for_rect = (host_node && !host_node->IsVisible)
                                            ? DockNodeGetRootNode(host_node)
                                            : host_node;
    if (ref_node_for_rect)
        AZ_ASSERT(ref_node_for_rect->IsVisible == true);

    AzDockNodeFlags src_node_flags =
            payload_node ? payload_node->MergedFlags
                         : payload_window->WindowClass.DockNodeFlagsOverrideSet;
    AzDockNodeFlags dst_node_flags =
            host_node ? host_node->MergedFlags
                      : host_window->WindowClass.DockNodeFlagsOverrideSet;
    data->IsCenterAvailable = true;
    if (is_outer_docking)
        data->IsCenterAvailable = false;
    else if (dst_node_flags & AzDockNodeFlags_NoDockingOverMe)
        data->IsCenterAvailable = false;
    else if (host_node
             && (dst_node_flags & AzDockNodeFlags_NoDockingOverCentralNode)
             && host_node->IsCentralNode())
        data->IsCenterAvailable = false;
    else if ((!host_node || !host_node->IsEmpty()) && payload_node
             && payload_node->IsSplitNode()
             && (payload_node->OnlyNodeWithWindows == NULL))
        data->IsCenterAvailable = false;
    else if ((src_node_flags & AzDockNodeFlags_NoDockingOverOther)
             && (!host_node || !host_node->IsEmpty()))
        data->IsCenterAvailable = false;
    else if ((src_node_flags & AzDockNodeFlags_NoDockingOverEmpty) && host_node
             && host_node->IsEmpty())
        data->IsCenterAvailable = false;

    data->IsSidesAvailable = true;
    if ((dst_node_flags & AzDockNodeFlags_NoDockingSplit)
        || g.IO.ConfigDockingNoSplit)
        data->IsSidesAvailable = false;
    else if (!is_outer_docking && host_node && host_node->ParentNode == NULL
             && host_node->IsCentralNode())
        data->IsSidesAvailable = false;
    else if (src_node_flags & AzDockNodeFlags_NoDockingSplitOther)
        data->IsSidesAvailable = false;

    data->FutureNode.HasCloseButton = (host_node ? host_node->HasCloseButton
                                                 : host_window->HasCloseButton)
                                      || (payload_window->HasCloseButton);
    data->FutureNode.HasWindowMenuButton =
            host_node ? true
                      : ((host_window->Flags & AzWindowFlags_NoCollapse) == 0);
    data->FutureNode.Pos = ref_node_for_rect ? ref_node_for_rect->Pos
                                             : host_window->Pos;
    data->FutureNode.Size = ref_node_for_rect ? ref_node_for_rect->Size
                                              : host_window->Size;

    AZ_ASSERT(AzDir_None == -1);
    data->SplitNode = host_node;
    data->SplitDir = AzDir_None;
    data->IsSplitDirExplicit = false;
    if (!host_window->Collapsed)
        for (int dir = AzDir_None; dir < AzDir_COUNT; dir++)
        {
            if (dir == AzDir_None && !data->IsCenterAvailable)
                continue;
            if (dir != AzDir_None && !data->IsSidesAvailable)
                continue;
            if (DockNodeCalcDropRectsAndTestMousePos(
                        data->FutureNode.Rect(),
                        (AzDir)dir,
                        data->DropRectsDraw[dir + 1],
                        is_outer_docking,
                        &g.IO.MousePos))
            {
                data->SplitDir = (AzDir)dir;
                data->IsSplitDirExplicit = true;
            }
        }

    data->IsDropAllowed = (data->SplitDir != AzDir_None)
                          || (data->IsCenterAvailable);
    if (!is_explicit_target && !data->IsSplitDirExplicit
        && !g.IO.ConfigDockingWithShift)
        data->IsDropAllowed = false;

    data->SplitRatio = 0.0f;
    if (data->SplitDir != AzDir_None)
    {
        AzDir split_dir = data->SplitDir;
        AzAxis split_axis = (split_dir == AzDir_Left
                             || split_dir == AzDir_Right)
                                    ? AzAxis_X
                                    : AzAxis_Y;
        AzVec2 pos_new, pos_old = data->FutureNode.Pos;
        AzVec2 size_new, size_old = data->FutureNode.Size;
        DockNodeCalcSplitRects(pos_old,
                               size_old,
                               pos_new,
                               size_new,
                               split_dir,
                               payload_window->Size);

        float split_ratio = AzSaturate(size_new[split_axis]
                                       / data->FutureNode.Size[split_axis]);
        data->FutureNode.Pos = pos_new;
        data->FutureNode.Size = size_new;
        data->SplitRatio = (split_dir == AzDir_Right || split_dir == AzDir_Down)
                                   ? (1.0f - split_ratio)
                                   : (split_ratio);
    }
}

static void Az::DockNodePreviewDockRender(AzWindow* host_window,
                                          AzDockNode* host_node,
                                          AzWindow* root_payload,
                                          const AzDockPreviewData* data)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.CurrentWindow == host_window);

    const bool is_transparent_payload = g.IO.ConfigDockingTransparentPayload;

    int overlay_draw_lists_count = 0;
    AzDrawList* overlay_draw_lists[2];
    overlay_draw_lists[overlay_draw_lists_count++] = GetForegroundDrawList(
            host_window->Viewport);
    if (host_window->Viewport != root_payload->Viewport
        && !is_transparent_payload)
        overlay_draw_lists[overlay_draw_lists_count++] = GetForegroundDrawList(
                root_payload->Viewport);

    const AzU32 overlay_col_main = GetColorU32(AzCol_DockingPreview,
                                               is_transparent_payload ? 0.60f
                                                                      : 0.40f);
    const AzU32 overlay_col_drop = GetColorU32(AzCol_DockingPreview,
                                               is_transparent_payload ? 0.90f
                                                                      : 0.70f);
    const AzU32 overlay_col_drop_hovered = GetColorU32(AzCol_DockingPreview,
                                                       is_transparent_payload
                                                               ? 1.20f
                                                               : 1.00f);
    const AzU32 overlay_col_lines = GetColorU32(AzCol_NavWindowingHighlight,
                                                is_transparent_payload ? 0.80f
                                                                       : 0.60f);

    const bool can_preview_tabs = (root_payload->DockNodeAsHost == NULL
                                   || root_payload->DockNodeAsHost->Windows.Size
                                              > 0);
    if (data->IsDropAllowed)
    {
        AzRect overlay_rect = data->FutureNode.Rect();
        if (data->SplitDir == AzDir_None && can_preview_tabs)
            overlay_rect.Min.y += GetFrameHeight();
        if (data->SplitDir != AzDir_None || data->IsCenterAvailable)
            for (int overlay_n = 0; overlay_n < overlay_draw_lists_count;
                 overlay_n++)
                overlay_draw_lists[overlay_n]->AddRectFilled(
                        overlay_rect.Min,
                        overlay_rect.Max,
                        overlay_col_main,
                        host_window->WindowRounding,
                        CalcRoundingFlagsForRectInRect(
                                overlay_rect,
                                host_window->Rect(),
                                g.Style.DockingSeparatorSize));
    }

    if (data->IsDropAllowed && can_preview_tabs && data->SplitDir == AzDir_None
        && data->IsCenterAvailable)
    {
        AzRect tab_bar_rect;
        DockNodeCalcTabBarLayout(&data->FutureNode,
                                 NULL,
                                 &tab_bar_rect,
                                 NULL,
                                 NULL);
        AzVec2 tab_pos = tab_bar_rect.Min;
        if (host_node && host_node->TabBar)
        {
            if (!host_node->IsHiddenTabBar() && !host_node->IsNoTabBar())
                tab_pos.x += host_node->TabBar->WidthAllTabs
                             + g.Style.ItemInnerSpacing.x;

            else
                tab_pos.x += g.Style.ItemInnerSpacing.x
                             + TabItemCalcSize(host_node->Windows[0]).x;
        } else if (!(host_window->Flags & AzWindowFlags_DockNodeHost))
        {
            tab_pos.x += g.Style.ItemInnerSpacing.x
                         + TabItemCalcSize(host_window).x;
        }

        if (root_payload->DockNodeAsHost)
            AZ_ASSERT(root_payload->DockNodeAsHost->Windows.Size
                      <= root_payload->DockNodeAsHost->TabBar->Tabs.Size);
        AzTabBar* tab_bar_with_payload =
                root_payload->DockNodeAsHost
                        ? root_payload->DockNodeAsHost->TabBar
                        : NULL;
        const int payload_count = tab_bar_with_payload
                                          ? tab_bar_with_payload->Tabs.Size
                                          : 1;
        for (int payload_n = 0; payload_n < payload_count; payload_n++)
        {
            AzWindow* payload_window =
                    tab_bar_with_payload
                            ? tab_bar_with_payload->Tabs[payload_n].Window
                            : root_payload;
            if (tab_bar_with_payload && payload_window == NULL)
                continue;
            if (!DockNodeIsDropAllowedOne(payload_window, host_window))
                continue;

            AzVec2 tab_size = TabItemCalcSize(payload_window);
            AzRect tab_bb(tab_pos.x,
                          tab_pos.y,
                          tab_pos.x + tab_size.x,
                          tab_pos.y + tab_size.y);
            tab_pos.x += tab_size.x + g.Style.ItemInnerSpacing.x;
            const AzU32 overlay_col_text = GetColorU32(
                    payload_window->DockStyle
                            .Colors[AzWindowDockStyleCol_Text]);
            const AzU32 overlay_col_tabs = GetColorU32(
                    payload_window->DockStyle
                            .Colors[AzWindowDockStyleCol_TabSelected]);
            PushStyleColor(AzCol_Text, overlay_col_text);
            for (int overlay_n = 0; overlay_n < overlay_draw_lists_count;
                 overlay_n++)
            {
                AzTabItemFlags tab_flags =
                        (payload_window->Flags & AzWindowFlags_UnsavedDocument)
                                ? AzTabItemFlags_UnsavedDocument
                                : 0;
                if (!tab_bar_rect.Contains(tab_bb))
                    overlay_draw_lists[overlay_n]
                            ->PushClipRect(tab_bar_rect.Min, tab_bar_rect.Max);
                TabItemBackground(overlay_draw_lists[overlay_n],
                                  tab_bb,
                                  tab_flags,
                                  overlay_col_tabs);
                TabItemLabelAndCloseButton(overlay_draw_lists[overlay_n],
                                           tab_bb,
                                           tab_flags,
                                           g.Style.FramePadding,
                                           payload_window->Name,
                                           0,
                                           0,
                                           false,
                                           NULL,
                                           NULL);
                if (!tab_bar_rect.Contains(tab_bb))
                    overlay_draw_lists[overlay_n]->PopClipRect();
            }
            PopStyleColor();
        }
    }

    const float overlay_rounding = AzMax(3.0f, g.Style.FrameRounding);
    for (int dir = AzDir_None; dir < AzDir_COUNT; dir++)
    {
        if (!data->DropRectsDraw[dir + 1].IsInverted())
        {
            AzRect draw_r = data->DropRectsDraw[dir + 1];
            AzRect draw_r_in = draw_r;
            draw_r_in.Expand(-2.0f);
            AzU32 overlay_col = (data->SplitDir == (AzDir)dir
                                 && data->IsSplitDirExplicit)
                                        ? overlay_col_drop_hovered
                                        : overlay_col_drop;
            for (int overlay_n = 0; overlay_n < overlay_draw_lists_count;
                 overlay_n++)
            {
                AzVec2 center = AzFloor(draw_r_in.GetCenter());
                overlay_draw_lists[overlay_n]->AddRectFilled(draw_r.Min,
                                                             draw_r.Max,
                                                             overlay_col,
                                                             overlay_rounding);
                overlay_draw_lists[overlay_n]->AddRect(draw_r_in.Min,
                                                       draw_r_in.Max,
                                                       overlay_col_lines,
                                                       overlay_rounding);
                if (dir == AzDir_Left || dir == AzDir_Right)
                    overlay_draw_lists[overlay_n]->AddLine(
                            AzVec2(center.x, draw_r_in.Min.y),
                            AzVec2(center.x, draw_r_in.Max.y),
                            overlay_col_lines);
                if (dir == AzDir_Up || dir == AzDir_Down)
                    overlay_draw_lists[overlay_n]->AddLine(
                            AzVec2(draw_r_in.Min.x, center.y),
                            AzVec2(draw_r_in.Max.x, center.y),
                            overlay_col_lines);
            }
        }

        if ((host_node
             && (host_node->MergedFlags & AzDockNodeFlags_NoDockingSplit))
            || g.IO.ConfigDockingNoSplit)
            return;
    }
}

void Az::DockNodeTreeSplit(AzContext* ctx,
                           AzDockNode* parent_node,
                           AzAxis split_axis,
                           int split_inheritor_child_idx,
                           float split_ratio,
                           AzDockNode* new_node)
{
    AzContext& g = *GAz;
    AZ_ASSERT(split_axis != AzAxis_None);

    AzDockNode* child_0 = (new_node && split_inheritor_child_idx != 0)
                                  ? new_node
                                  : DockContextAddNode(ctx, 0);
    child_0->ParentNode = parent_node;

    AzDockNode* child_1 = (new_node && split_inheritor_child_idx != 1)
                                  ? new_node
                                  : DockContextAddNode(ctx, 0);
    child_1->ParentNode = parent_node;

    AzDockNode* child_inheritor = (split_inheritor_child_idx == 0) ? child_0
                                                                   : child_1;
    DockNodeMoveChildNodes(child_inheritor, parent_node);
    parent_node->ChildNodes[0] = child_0;
    parent_node->ChildNodes[1] = child_1;
    parent_node->ChildNodes[split_inheritor_child_idx]->VisibleWindow =
            parent_node->VisibleWindow;
    parent_node->SplitAxis = split_axis;
    parent_node->VisibleWindow = NULL;
    parent_node->AuthorityForPos = parent_node->AuthorityForSize =
            AzDataAuthority_DockNode;

    float size_avail = (parent_node->Size[split_axis]
                        - g.Style.DockingSeparatorSize);
    size_avail = AzMax(size_avail, g.Style.WindowMinSize[split_axis] * 2.0f);
    AZ_ASSERT(size_avail > 0.0f);

    child_0->SizeRef = child_1->SizeRef = parent_node->Size;
    child_0->SizeRef[split_axis] = AzTrunc(size_avail * split_ratio);
    child_1->SizeRef[split_axis] = AzTrunc(size_avail
                                           - child_0->SizeRef[split_axis]);

    DockNodeMoveWindows(parent_node->ChildNodes[split_inheritor_child_idx],
                        parent_node);
    DockSettingsRenameNodeReferences(
            parent_node->ID,
            parent_node->ChildNodes[split_inheritor_child_idx]->ID);
    DockNodeUpdateHasCentralNodeChild(DockNodeGetRootNode(parent_node));
    DockNodeTreeUpdatePosSize(parent_node, parent_node->Pos, parent_node->Size);

    child_0->SharedFlags = parent_node->SharedFlags
                           & AzDockNodeFlags_SharedFlagsInheritMask_;
    child_1->SharedFlags = parent_node->SharedFlags
                           & AzDockNodeFlags_SharedFlagsInheritMask_;
    child_inheritor->LocalFlags = parent_node->LocalFlags
                                  & AzDockNodeFlags_LocalFlagsTransferMask_;
    parent_node->LocalFlags &= ~AzDockNodeFlags_LocalFlagsTransferMask_;
    child_0->UpdateMergedFlags();
    child_1->UpdateMergedFlags();
    parent_node->UpdateMergedFlags();
    if (child_inheritor->IsCentralNode())
        DockNodeGetRootNode(parent_node)->CentralNode = child_inheritor;
}

void Az::DockNodeTreeMerge(AzContext* ctx,
                           AzDockNode* parent_node,
                           AzDockNode* merge_lead_child)
{
    AzContext& g = *GAz;
    AzDockNode* child_0 = parent_node->ChildNodes[0];
    AzDockNode* child_1 = parent_node->ChildNodes[1];
    AZ_ASSERT(child_0 || child_1);
    AZ_ASSERT(merge_lead_child == child_0 || merge_lead_child == child_1);
    if ((child_0 && child_0->Windows.Size > 0)
        || (child_1 && child_1->Windows.Size > 0))
    {
        AZ_ASSERT(parent_node->TabBar == NULL);
        AZ_ASSERT(parent_node->Windows.Size == 0);
    }
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockNodeTreeMerge: 0x%08X + 0x%08X back into parent "
            "0x%08X\n",
            child_0 ? child_0->ID : 0,
            child_1 ? child_1->ID : 0,
            parent_node->ID);

    AzVec2 backup_last_explicit_size = parent_node->SizeRef;
    DockNodeMoveChildNodes(parent_node, merge_lead_child);
    if (child_0)
    {
        DockNodeMoveWindows(parent_node, child_0);

        DockSettingsRenameNodeReferences(child_0->ID, parent_node->ID);
    }
    if (child_1)
    {
        DockNodeMoveWindows(parent_node, child_1);
        DockSettingsRenameNodeReferences(child_1->ID, parent_node->ID);
    }
    DockNodeApplyPosSizeToWindows(parent_node);
    parent_node->AuthorityForPos = parent_node->AuthorityForSize =
            parent_node->AuthorityForViewport = AzDataAuthority_Auto;
    parent_node->VisibleWindow = merge_lead_child->VisibleWindow;
    parent_node->SizeRef = backup_last_explicit_size;

    parent_node->LocalFlags &= ~AzDockNodeFlags_LocalFlagsTransferMask_;

    parent_node->LocalFlags |= (child_0 ? child_0->LocalFlags : 0)
                               & AzDockNodeFlags_LocalFlagsTransferMask_;
    parent_node->LocalFlags |= (child_1 ? child_1->LocalFlags : 0)
                               & AzDockNodeFlags_LocalFlagsTransferMask_;
    parent_node->LocalFlagsInWindows = (child_0 ? child_0->LocalFlagsInWindows
                                                : 0)
                                       | (child_1 ? child_1->LocalFlagsInWindows
                                                  : 0);

    parent_node->UpdateMergedFlags();

    if (child_0)
    {
        ctx->DockContext.Nodes.SetVoidPtr(child_0->ID, NULL);
        AZ_DELETE(child_0);
    }
    if (child_1)
    {
        ctx->DockContext.Nodes.SetVoidPtr(child_1->ID, NULL);
        AZ_DELETE(child_1);
    }
}

void Az::DockNodeTreeUpdatePosSize(AzDockNode* node,
                                   AzVec2 pos,
                                   AzVec2 size,
                                   AzDockNode* only_write_to_single_node)
{
    AzContext& g = *GAz;
    const bool write_to_node = only_write_to_single_node == NULL
                               || only_write_to_single_node == node;
    if (write_to_node)
    {
        node->Pos = pos;
        node->Size = size;
    }

    if (node->IsLeafNode())
        return;

    AzDockNode* child_0 = node->ChildNodes[0];
    AzDockNode* child_1 = node->ChildNodes[1];
    AzVec2 child_0_pos = pos, child_1_pos = pos;
    AzVec2 child_0_size = size, child_1_size = size;

    const bool child_0_is_toward_single_node =
            (only_write_to_single_node != NULL
             && DockNodeIsInHierarchyOf(only_write_to_single_node, child_0));
    const bool child_1_is_toward_single_node =
            (only_write_to_single_node != NULL
             && DockNodeIsInHierarchyOf(only_write_to_single_node, child_1));
    const bool child_0_is_or_will_be_visible = child_0->IsVisible
                                               || child_0_is_toward_single_node;
    const bool child_1_is_or_will_be_visible = child_1->IsVisible
                                               || child_1_is_toward_single_node;

    if (child_0_is_or_will_be_visible && child_1_is_or_will_be_visible)
    {
        const float spacing = g.Style.DockingSeparatorSize;
        const AzAxis axis = (AzAxis)node->SplitAxis;
        const float size_avail = AzMax(size[axis] - spacing, 0.0f);

        const float size_min_each = AzTrunc(
                AzMin(size_avail, g.Style.WindowMinSize[axis] * 2.0f) * 0.5f);

        if (child_0->WantLockSizeOnce && !child_1->WantLockSizeOnce)
        {
            child_0_size[axis] = child_0->SizeRef[axis] =
                    AzMin(size_avail - 1.0f, child_0->Size[axis]);
            child_1_size[axis] = child_1->SizeRef[axis] =
                    (size_avail - child_0_size[axis]);
            AZ_ASSERT(child_0->SizeRef[axis] > 0.0f
                      && child_1->SizeRef[axis] > 0.0f);
        } else if (child_1->WantLockSizeOnce && !child_0->WantLockSizeOnce)
        {
            child_1_size[axis] = child_1->SizeRef[axis] =
                    AzMin(size_avail - 1.0f, child_1->Size[axis]);
            child_0_size[axis] = child_0->SizeRef[axis] =
                    (size_avail - child_1_size[axis]);
            AZ_ASSERT(child_0->SizeRef[axis] > 0.0f
                      && child_1->SizeRef[axis] > 0.0f);
        } else if (child_0->WantLockSizeOnce && child_1->WantLockSizeOnce)
        {
            float split_ratio = child_0_size[axis]
                                / (child_0_size[axis] + child_1_size[axis]);
            child_0_size[axis] = child_0->SizeRef[axis] = AzTrunc(
                    size_avail * split_ratio);
            child_1_size[axis] = child_1->SizeRef[axis] =
                    (size_avail - child_0_size[axis]);
            AZ_ASSERT(child_0->SizeRef[axis] > 0.0f
                      && child_1->SizeRef[axis] > 0.0f);
        }

        else if (child_0->SizeRef[axis] != 0.0f && child_1->HasCentralNodeChild)
        {
            child_0_size[axis] = AzMin(size_avail - size_min_each,
                                       child_0->SizeRef[axis]);
            child_1_size[axis] = (size_avail - child_0_size[axis]);
        } else if (child_1->SizeRef[axis] != 0.0f
                   && child_0->HasCentralNodeChild)
        {
            child_1_size[axis] = AzMin(size_avail - size_min_each,
                                       child_1->SizeRef[axis]);
            child_0_size[axis] = (size_avail - child_1_size[axis]);
        } else
        {
            float split_ratio = child_0->SizeRef[axis]
                                / (child_0->SizeRef[axis]
                                   + child_1->SizeRef[axis]);
            child_0_size[axis] = AzMax(size_min_each,
                                       AzTrunc(size_avail * split_ratio
                                               + 0.5f));
            child_1_size[axis] = (size_avail - child_0_size[axis]);
        }

        child_1_pos[axis] += spacing + child_0_size[axis];
    }

    if (only_write_to_single_node == NULL)
        child_0->WantLockSizeOnce = child_1->WantLockSizeOnce = false;

    const bool child_0_recurse = only_write_to_single_node
                                         ? child_0_is_toward_single_node
                                         : child_0->IsVisible;
    const bool child_1_recurse = only_write_to_single_node
                                         ? child_1_is_toward_single_node
                                         : child_1->IsVisible;
    if (child_0_recurse)
        DockNodeTreeUpdatePosSize(child_0, child_0_pos, child_0_size);
    if (child_1_recurse)
        DockNodeTreeUpdatePosSize(child_1, child_1_pos, child_1_size);
}

static void DockNodeTreeUpdateSplitterFindTouchingNode(
        AzDockNode* node,
        AzAxis axis,
        int side,
        AzVector<AzDockNode*>* touching_nodes)
{
    if (node->IsLeafNode())
    {
        touching_nodes->push_back(node);
        return;
    }
    if (node->ChildNodes[0]->IsVisible)
        if (node->SplitAxis != axis || side == 0
            || !node->ChildNodes[1]->IsVisible)
            DockNodeTreeUpdateSplitterFindTouchingNode(node->ChildNodes[0],
                                                       axis,
                                                       side,
                                                       touching_nodes);
    if (node->ChildNodes[1]->IsVisible)
        if (node->SplitAxis != axis || side == 1
            || !node->ChildNodes[0]->IsVisible)
            DockNodeTreeUpdateSplitterFindTouchingNode(node->ChildNodes[1],
                                                       axis,
                                                       side,
                                                       touching_nodes);
}

void Az::DockNodeTreeUpdateSplitter(AzDockNode* node)
{
    if (node->IsLeafNode())
        return;

    AzContext& g = *GAz;

    AzDockNode* child_0 = node->ChildNodes[0];
    AzDockNode* child_1 = node->ChildNodes[1];
    if (child_0->IsVisible && child_1->IsVisible)
    {
        const AzAxis axis = (AzAxis)node->SplitAxis;
        AZ_ASSERT(axis != AzAxis_None);
        AzRect bb;
        bb.Min = child_0->Pos;
        bb.Max = child_1->Pos;
        bb.Min[axis] += child_0->Size[axis];
        bb.Max[axis ^ 1] += child_1->Size[axis ^ 1];

        const AzDockNodeFlags merged_flags = child_0->MergedFlags
                                             | child_1->MergedFlags;
        const AzDockNodeFlags no_resize_axis_flag =
                (axis == AzAxis_X) ? AzDockNodeFlags_NoResizeX
                                   : AzDockNodeFlags_NoResizeY;
        if ((merged_flags & AzDockNodeFlags_NoResize)
            || (merged_flags & no_resize_axis_flag))
        {
            AzWindow* window = g.CurrentWindow;
            window->DrawList->AddRectFilled(bb.Min,
                                            bb.Max,
                                            GetColorU32(AzCol_Separator),
                                            g.Style.FrameRounding);
        } else
        {
            PushID(node->ID);

            AzVector<AzDockNode*> touching_nodes[2];
            float min_size = g.Style.WindowMinSize[axis];
            float resize_limits[2];
            resize_limits[0] = node->ChildNodes[0]->Pos[axis] + min_size;
            resize_limits[1] = node->ChildNodes[1]->Pos[axis]
                               + node->ChildNodes[1]->Size[axis] - min_size;

            AzID splitter_id = GetID("##Splitter");
            if (g.ActiveId == splitter_id)
            {
                DockNodeTreeUpdateSplitterFindTouchingNode(child_0,
                                                           axis,
                                                           1,
                                                           &touching_nodes[0]);
                DockNodeTreeUpdateSplitterFindTouchingNode(child_1,
                                                           axis,
                                                           0,
                                                           &touching_nodes[1]);
                for (int touching_node_n = 0;
                     touching_node_n < touching_nodes[0].Size;
                     touching_node_n++)
                    resize_limits[0] = AzMax(
                            resize_limits[0],
                            touching_nodes[0][touching_node_n]->Rect().Min[axis]
                                    + min_size);
                for (int touching_node_n = 0;
                     touching_node_n < touching_nodes[1].Size;
                     touching_node_n++)
                    resize_limits[1] = AzMin(
                            resize_limits[1],
                            touching_nodes[1][touching_node_n]->Rect().Max[axis]
                                    - min_size);
            }

            float cur_size_0 = child_0->Size[axis];
            float cur_size_1 = child_1->Size[axis];
            float min_size_0 = resize_limits[0] - child_0->Pos[axis];
            float min_size_1 = child_1->Pos[axis] + child_1->Size[axis]
                               - resize_limits[1];
            AzU32 bg_col = GetColorU32(AzCol_WindowBg);
            if (SplitterBehavior(bb,
                                 GetID("##Splitter"),
                                 axis,
                                 &cur_size_0,
                                 &cur_size_1,
                                 min_size_0,
                                 min_size_1,
                                 g.WindowsBorderHoverPadding,
                                 WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TAZER,
                                 bg_col))
            {
                if (touching_nodes[0].Size > 0 && touching_nodes[1].Size > 0)
                {
                    child_0->Size[axis] = child_0->SizeRef[axis] = cur_size_0;
                    child_1->Pos[axis] -= cur_size_1 - child_1->Size[axis];
                    child_1->Size[axis] = child_1->SizeRef[axis] = cur_size_1;

                    for (int side_n = 0; side_n < 2; side_n++)
                        for (int touching_node_n = 0;
                             touching_node_n < touching_nodes[side_n].Size;
                             touching_node_n++)
                        {
                            AzDockNode* touching_node =
                                    touching_nodes[side_n][touching_node_n];

                            while (touching_node->ParentNode != node)
                            {
                                if (touching_node->ParentNode->SplitAxis
                                    == axis)
                                {
                                    AzDockNode* node_to_preserve =
                                            touching_node->ParentNode
                                                    ->ChildNodes[side_n];
                                    node_to_preserve->WantLockSizeOnce = true;
                                }
                                touching_node = touching_node->ParentNode;
                            }
                        }

                    DockNodeTreeUpdatePosSize(child_0,
                                              child_0->Pos,
                                              child_0->Size);
                    DockNodeTreeUpdatePosSize(child_1,
                                              child_1->Pos,
                                              child_1->Size);
                    MarkIniSettingsDirty();
                }
            }
            PopID();
        }
    }

    if (child_0->IsVisible)
        DockNodeTreeUpdateSplitter(child_0);
    if (child_1->IsVisible)
        DockNodeTreeUpdateSplitter(child_1);
}

AzDockNode* Az::DockNodeTreeFindFallbackLeafNode(AzDockNode* node)
{
    if (node->IsLeafNode())
        return node;
    if (AzDockNode* leaf_node = DockNodeTreeFindFallbackLeafNode(
                node->ChildNodes[0]))
        return leaf_node;
    if (AzDockNode* leaf_node = DockNodeTreeFindFallbackLeafNode(
                node->ChildNodes[1]))
        return leaf_node;
    return NULL;
}

AzDockNode* Az::DockNodeTreeFindVisibleNodeByPos(AzDockNode* node, AzVec2 pos)
{
    if (!node->IsVisible)
        return NULL;

    const float dock_spacing = 0.0f;

    AzRect r(node->Pos, node->Pos + node->Size);
    r.Expand(dock_spacing * 0.5f);
    bool inside = r.Contains(pos);
    if (!inside)
        return NULL;

    if (node->IsLeafNode())
        return node;
    if (AzDockNode* hovered_node =
                DockNodeTreeFindVisibleNodeByPos(node->ChildNodes[0], pos))
        return hovered_node;
    if (AzDockNode* hovered_node =
                DockNodeTreeFindVisibleNodeByPos(node->ChildNodes[1], pos))
        return hovered_node;

    return node;
}

void Az::SetWindowDock(AzWindow* window, AzID dock_id, AzCond cond)
{
    if (cond && (window->SetWindowDockAllowFlags & cond) == 0)
        return;
    window->SetWindowDockAllowFlags &= ~(AzCond_Once | AzCond_FirstUseEver
                                         | AzCond_Appearing);

    if (window->DockId == dock_id)
        return;

    AzContext& g = *GAz;
    if (AzDockNode* new_node = DockContextFindNodeByID(&g, dock_id))
        if (new_node->IsSplitNode())
        {
            new_node = DockNodeGetRootNode(new_node);
            if (new_node->CentralNode)
            {
                AZ_ASSERT(new_node->CentralNode->IsCentralNode());
                dock_id = new_node->CentralNode->ID;
            } else
            {
                dock_id = new_node->LastFocusedNodeId;
            }
        }

    if (window->DockId == dock_id)
        return;

    if (window->DockNode)
        DockNodeRemoveWindow(window->DockNode, window, 0);
    window->DockId = dock_id;
}

AzID Az::DockSpace(AzID dockspace_id,
                   const AzVec2& size_arg,
                   AzDockNodeFlags flags,
                   const AzWindowClass* window_class)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindowRead();
    if (!(g.IO.ConfigFlags & AzConfigFlags_DockingEnable))
        return 0;

    if (window->SkipItems)
        flags |= AzDockNodeFlags_KeepAliveOnly;
    if ((flags & AzDockNodeFlags_KeepAliveOnly) == 0)
        window = GetCurrentWindow();

    AZ_ASSERT((flags & AzDockNodeFlags_DockSpace) == 0);

    AZ_ASSERT((flags & AzDockNodeFlags_CentralNode) == 0);

    AZ_ASSERT(dockspace_id != 0);
    AzDockNode* node = DockContextFindNodeByID(&g, dockspace_id);
    if (node == NULL)
    {
        AZ_DEBUG_LOG_DOCKING(
                "[docking] DockSpace: dockspace node 0x%08X created\n",
                dockspace_id);
        node = DockContextAddNode(&g, dockspace_id);
        node->SetLocalFlags(AzDockNodeFlags_CentralNode);
    }
    if (window_class && window_class->ClassId != node->WindowClass.ClassId)
        AZ_DEBUG_LOG_DOCKING(
                "[docking] DockSpace: dockspace node 0x%08X: setup WindowClass "
                "0x%08X -> 0x%08X\n",
                dockspace_id,
                node->WindowClass.ClassId,
                window_class->ClassId);
    node->SharedFlags = flags;
    node->WindowClass = window_class ? *window_class : AzWindowClass();

    if (node->LastFrameActive == g.FrameCount
        && !(flags & AzDockNodeFlags_KeepAliveOnly))
    {
        AZ_ASSERT(node->IsDockSpace() == false
                  && "Cannot call DockSpace() twice a frame with the same ID");
        node->SetLocalFlags(node->LocalFlags | AzDockNodeFlags_DockSpace);
        return dockspace_id;
    }
    node->SetLocalFlags(node->LocalFlags | AzDockNodeFlags_DockSpace);

    if (flags & AzDockNodeFlags_KeepAliveOnly)
    {
        node->LastFrameAlive = g.FrameCount;
        return dockspace_id;
    }

    const AzVec2 content_avail = GetContentRegionAvail();
    AzVec2 size = AzTrunc(size_arg);
    if (size.x <= 0.0f)
        size.x = AzMax(content_avail.x + size.x, 4.0f);

    if (size.y <= 0.0f)
        size.y = AzMax(content_avail.y + size.y, 4.0f);
    AZ_ASSERT(size.x > 0.0f && size.y > 0.0f);

    node->Pos = window->DC.CursorPos;
    node->Size = node->SizeRef = size;
    SetNextWindowPos(node->Pos);
    SetNextWindowSize(node->Size);
    g.NextWindowData.PosUndock = false;

    AzWindowFlags window_flags = AzWindowFlags_ChildWindow
                                 | AzWindowFlags_DockNodeHost;
    window_flags |= AzWindowFlags_NoSavedSettings | AzWindowFlags_NoResize
                    | AzWindowFlags_NoCollapse | AzWindowFlags_NoTitleBar;
    window_flags |= AzWindowFlags_NoScrollbar | AzWindowFlags_NoScrollWithMouse;
    window_flags |= AzWindowFlags_NoBackground;

    char title[256];
    AzFormatString(title,
                   AZ_ARRAYSIZE(title),
                   "%s/DockSpace_%08X",
                   window->Name,
                   dockspace_id);

    PushStyleVar(AzStyleVar_ChildBorderSize, 0.0f);
    Begin(title, NULL, window_flags);
    PopStyleVar();

    AzWindow* host_window = g.CurrentWindow;
    DockNodeSetupHostWindow(node, host_window);
    host_window->ChildId = window->GetID(title);
    node->OnlyNodeWithWindows = NULL;

    AZ_ASSERT(node->IsRootNode());

    if (node->IsLeafNode() && !node->IsCentralNode())
        node->SetLocalFlags(node->LocalFlags | AzDockNodeFlags_CentralNode);

    DockNodeUpdate(node);

    End();

    AzRect bb(node->Pos, node->Pos + size);
    ItemSize(size);
    ItemAdd(bb, dockspace_id, NULL, AzItemFlags_NoNav);

    if ((g.LastItemData.StatusFlags & AzItemStatusFlags_HoveredRect)
        && IsWindowChildOf(g.HoveredWindow, host_window, false, true))

        g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredWindow;

    return dockspace_id;
}

AzID Az::DockSpaceOverViewport(AzID dockspace_id,
                               const AzViewport* viewport,
                               AzDockNodeFlags dockspace_flags,
                               const AzWindowClass* window_class)
{
    if (viewport == NULL)
        viewport = GetMainViewport();

    SetNextWindowPos(viewport->WorkPos);
    SetNextWindowSize(viewport->WorkSize);
    SetNextWindowViewport(viewport->ID);

    AzWindowFlags host_window_flags = 0;
    host_window_flags |= AzWindowFlags_NoTitleBar | AzWindowFlags_NoCollapse
                         | AzWindowFlags_NoResize | AzWindowFlags_NoMove
                         | AzWindowFlags_NoDocking;
    host_window_flags |= AzWindowFlags_NoBringToFrontOnFocus
                         | AzWindowFlags_NoNavFocus;
    if (dockspace_flags & AzDockNodeFlags_PassthruCentralNode)
        host_window_flags |= AzWindowFlags_NoBackground;

    if (dockspace_flags & AzDockNodeFlags_KeepAliveOnly)
        host_window_flags |= AzWindowFlags_NoMouseInputs;

    char label[32];
    AzFormatString(label,
                   AZ_ARRAYSIZE(label),
                   "WindowOverViewport_%08X",
                   viewport->ID);

    PushStyleVar(AzStyleVar_WindowRounding, 0.0f);
    PushStyleVar(AzStyleVar_WindowBorderSize, 0.0f);
    PushStyleVar(AzStyleVar_WindowPadding, AzVec2(0.0f, 0.0f));
    Begin(label, NULL, host_window_flags);
    PopStyleVar(3);

    if (dockspace_id == 0)
        dockspace_id = GetID("DockSpace");
    DockSpace(dockspace_id, AzVec2(0.0f, 0.0f), dockspace_flags, window_class);

    End();

    return dockspace_id;
}

void Az::DockBuilderDockWindow(const char* window_name, AzID node_id)
{
    AzContext& g = *GAz;
    AZ_UNUSED(g);
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockBuilderDockWindow '%s' to node 0x%08X\n",
            window_name,
            node_id);
    AzID window_id = AzHashStr(window_name);
    if (AzWindow* window = FindWindowByID(window_id))
    {
        AzID prev_node_id = window->DockId;
        SetWindowDock(window, node_id, AzCond_Always);
        if (window->DockId != prev_node_id)
            window->DockOrder = -1;
    } else
    {
        AzWindowSettings* settings = FindWindowSettingsByID(window_id);
        if (settings == NULL)
            settings = CreateNewWindowSettings(window_name);
        if (settings->DockId != node_id)
            settings->DockOrder = -1;
        settings->DockId = node_id;
    }
}

AzDockNode* Az::DockBuilderGetNode(AzID node_id)
{
    AzContext& g = *GAz;
    return DockContextFindNodeByID(&g, node_id);
}

void Az::DockBuilderSetNodePos(AzID node_id, AzVec2 pos)
{
    AzContext& g = *GAz;
    AzDockNode* node = DockContextFindNodeByID(&g, node_id);
    if (node == NULL)
        return;
    node->Pos = pos;
    node->AuthorityForPos = AzDataAuthority_DockNode;
}

void Az::DockBuilderSetNodeSize(AzID node_id, AzVec2 size)
{
    AzContext& g = *GAz;
    AzDockNode* node = DockContextFindNodeByID(&g, node_id);
    if (node == NULL)
        return;
    AZ_ASSERT(size.x > 0.0f && size.y > 0.0f);
    node->Size = node->SizeRef = size;
    node->AuthorityForSize = AzDataAuthority_DockNode;
}

AzID Az::DockBuilderAddNode(AzID node_id, AzDockNodeFlags flags)
{
    AzContext& g = *GAz;
    AZ_UNUSED(g);
    AZ_DEBUG_LOG_DOCKING("[docking] DockBuilderAddNode 0x%08X flags=%08X\n",
                         node_id,
                         flags);

    if (node_id != 0)
        DockBuilderRemoveNode(node_id);

    AzDockNode* node = NULL;
    if (flags & AzDockNodeFlags_DockSpace)
    {
        DockSpace(node_id,
                  AzVec2(0, 0),
                  (flags & ~AzDockNodeFlags_DockSpace)
                          | AzDockNodeFlags_KeepAliveOnly);
        node = DockContextFindNodeByID(&g, node_id);
    } else
    {
        node = DockContextAddNode(&g, node_id);
        node->SetLocalFlags(flags);
    }
    node->LastFrameAlive = g.FrameCount;

    return node->ID;
}

void Az::DockBuilderRemoveNode(AzID node_id)
{
    AzContext& g = *GAz;
    AZ_UNUSED(g);
    AZ_DEBUG_LOG_DOCKING("[docking] DockBuilderRemoveNode 0x%08X\n", node_id);

    AzDockNode* node = DockContextFindNodeByID(&g, node_id);
    if (node == NULL)
        return;
    DockBuilderRemoveNodeDockedWindows(node_id, true);
    DockBuilderRemoveNodeChildNodes(node_id);

    node = DockContextFindNodeByID(&g, node_id);
    if (node == NULL)
        return;
    if (node->IsCentralNode() && node->ParentNode)
        node->ParentNode->SetLocalFlags(node->ParentNode->LocalFlags
                                        | AzDockNodeFlags_CentralNode);
    DockContextRemoveNode(&g, node, true);
}

void Az::DockBuilderRemoveNodeChildNodes(AzID root_id)
{
    AzContext& g = *GAz;
    AzDockContext* dc = &g.DockContext;

    AzDockNode* root_node = root_id ? DockContextFindNodeByID(&g, root_id)
                                    : NULL;
    if (root_id && root_node == NULL)
        return;
    bool has_central_node = false;

    AzDataAuthority backup_root_node_authority_for_pos =
            root_node ? root_node->AuthorityForPos : AzDataAuthority_Auto;
    AzDataAuthority backup_root_node_authority_for_size =
            root_node ? root_node->AuthorityForSize : AzDataAuthority_Auto;

    AzVector<AzDockNode*> nodes_to_remove;
    for (int n = 0; n < dc->Nodes.Data.Size; n++)
        if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
        {
            bool want_removal = (root_id == 0)
                                || (node->ID != root_id
                                    && DockNodeGetRootNode(node)->ID
                                               == root_id);
            if (want_removal)
            {
                if (node->IsCentralNode())
                    has_central_node = true;
                if (root_id != 0)
                    DockContextQueueNotifyRemovedNode(&g, node);
                if (root_node)
                {
                    DockNodeMoveWindows(root_node, node);
                    DockSettingsRenameNodeReferences(node->ID, root_node->ID);
                }
                nodes_to_remove.push_back(node);
            }
        }

    if (root_node)
    {
        root_node->AuthorityForPos = backup_root_node_authority_for_pos;
        root_node->AuthorityForSize = backup_root_node_authority_for_size;
    }

    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
        if (AzID window_settings_dock_id = settings->DockId)
            for (int n = 0; n < nodes_to_remove.Size; n++)
                if (nodes_to_remove[n]->ID == window_settings_dock_id)
                {
                    settings->DockId = root_id;
                    break;
                }

    if (nodes_to_remove.Size > 1)
        AzQsort(nodes_to_remove.Data,
                nodes_to_remove.Size,
                sizeof(AzDockNode*),
                DockNodeComparerDepthMostFirst);
    for (int n = 0; n < nodes_to_remove.Size; n++)
        DockContextRemoveNode(&g, nodes_to_remove[n], false);

    if (root_id == 0)
    {
        dc->Nodes.Clear();
        dc->Requests.clear();
    } else if (has_central_node)
    {
        root_node->CentralNode = root_node;
        root_node->SetLocalFlags(root_node->LocalFlags
                                 | AzDockNodeFlags_CentralNode);
    }
}

void Az::DockBuilderRemoveNodeDockedWindows(AzID root_id,
                                            bool clear_settings_refs)
{
    AzContext& g = *GAz;
    if (clear_settings_refs)
    {
        for (AzWindowSettings* settings = g.SettingsWindows.begin();
             settings != NULL;
             settings = g.SettingsWindows.next_chunk(settings))
        {
            bool want_removal = (root_id == 0) || (settings->DockId == root_id);
            if (!want_removal && settings->DockId != 0)
                if (AzDockNode* node =
                            DockContextFindNodeByID(&g, settings->DockId))
                    if (DockNodeGetRootNode(node)->ID == root_id)
                        want_removal = true;
            if (want_removal)
                settings->DockId = 0;
        }
    }

    for (int n = 0; n < g.Windows.Size; n++)
    {
        AzWindow* window = g.Windows[n];
        bool want_removal = (root_id == 0)
                            || (window->DockNode
                                && DockNodeGetRootNode(window->DockNode)->ID
                                           == root_id)
                            || (window->DockNodeAsHost
                                && window->DockNodeAsHost->ID == root_id);
        if (want_removal)
        {
            const AzID backup_dock_id = window->DockId;
            AZ_UNUSED(backup_dock_id);
            DockContextProcessUndockWindow(&g, window, clear_settings_refs);
            if (!clear_settings_refs)
                AZ_ASSERT(window->DockId == backup_dock_id);
        }
    }
}

AzID Az::DockBuilderSplitNode(AzID id,
                              AzDir split_dir,
                              float size_ratio_for_node_at_dir,
                              AzID* out_id_at_dir,
                              AzID* out_id_at_opposite_dir)
{
    AzContext& g = *GAz;
    AZ_ASSERT(split_dir != AzDir_None);
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockBuilderSplitNode: node 0x%08X, split_dir %d\n",
            id,
            split_dir);

    AzDockNode* node = DockContextFindNodeByID(&g, id);
    if (node == NULL)
    {
        AZ_ASSERT(node != NULL);
        return 0;
    }

    AzDockRequest req;
    req.Type = AzDockRequestType_Split;
    req.DockTargetWindow = NULL;
    req.DockTargetNode = node;
    req.DockPayload = NULL;
    req.DockSplitDir = split_dir;
    req.DockSplitRatio = AzSaturate(
            (split_dir == AzDir_Left || split_dir == AzDir_Up)
                    ? size_ratio_for_node_at_dir
                    : 1.0f - size_ratio_for_node_at_dir);
    req.DockSplitOuter = false;
    DockContextProcessDock(&g, &req);

    AzID id_at_dir =
            node->ChildNodes[(split_dir == AzDir_Left || split_dir == AzDir_Up)
                                     ? 0
                                     : 1]
                    ->ID;
    AzID id_at_opposite_dir =
            node->ChildNodes[(split_dir == AzDir_Left || split_dir == AzDir_Up)
                                     ? 1
                                     : 0]
                    ->ID;
    if (out_id_at_dir)
        *out_id_at_dir = id_at_dir;
    if (out_id_at_opposite_dir)
        *out_id_at_opposite_dir = id_at_opposite_dir;
    return id_at_dir;
}

static AzDockNode* DockBuilderCopyNodeRec(AzDockNode* src_node,
                                          AzID dst_node_id_if_known,
                                          AzVector<AzID>* out_node_remap_pairs)
{
    AzContext& g = *GAz;
    AzDockNode* dst_node = Az::DockContextAddNode(&g, dst_node_id_if_known);
    dst_node->SharedFlags = src_node->SharedFlags;
    dst_node->LocalFlags = src_node->LocalFlags;
    dst_node->LocalFlagsInWindows = AzDockNodeFlags_None;
    dst_node->Pos = src_node->Pos;
    dst_node->Size = src_node->Size;
    dst_node->SizeRef = src_node->SizeRef;
    dst_node->SplitAxis = src_node->SplitAxis;
    dst_node->UpdateMergedFlags();

    out_node_remap_pairs->push_back(src_node->ID);
    out_node_remap_pairs->push_back(dst_node->ID);

    for (int child_n = 0; child_n < AZ_ARRAYSIZE(src_node->ChildNodes);
         child_n++)
        if (src_node->ChildNodes[child_n])
        {
            dst_node->ChildNodes[child_n] =
                    DockBuilderCopyNodeRec(src_node->ChildNodes[child_n],
                                           0,
                                           out_node_remap_pairs);
            dst_node->ChildNodes[child_n]->ParentNode = dst_node;
        }

    AZ_DEBUG_LOG_DOCKING("[docking] Fork node %08X -> %08X (%d childs)\n",
                         src_node->ID,
                         dst_node->ID,
                         dst_node->IsSplitNode() ? 2 : 0);
    return dst_node;
}

void Az::DockBuilderCopyNode(AzID src_node_id,
                             AzID dst_node_id,
                             AzVector<AzID>* out_node_remap_pairs)
{
    AzContext& g = *GAz;
    AZ_ASSERT(src_node_id != 0);
    AZ_ASSERT(dst_node_id != 0);
    AZ_ASSERT(out_node_remap_pairs != NULL);

    DockBuilderRemoveNode(dst_node_id);

    AzDockNode* src_node = DockContextFindNodeByID(&g, src_node_id);
    AZ_ASSERT(src_node != NULL);

    out_node_remap_pairs->clear();
    DockBuilderCopyNodeRec(src_node, dst_node_id, out_node_remap_pairs);

    AZ_ASSERT((out_node_remap_pairs->Size % 2) == 0);
}

void Az::DockBuilderCopyWindowSettings(const char* src_name,
                                       const char* dst_name)
{
    AzWindow* src_window = FindWindowByName(src_name);
    if (src_window == NULL)
        return;
    if (AzWindow* dst_window = FindWindowByName(dst_name))
    {
        dst_window->Pos = src_window->Pos;
        dst_window->Size = src_window->Size;
        dst_window->SizeFull = src_window->SizeFull;
        dst_window->Collapsed = src_window->Collapsed;
    } else
    {
        AzWindowSettings* dst_settings = FindWindowSettingsByID(
                AzHashStr(dst_name));
        if (!dst_settings)
            dst_settings = CreateNewWindowSettings(dst_name);
        AzVec2ih window_pos_2ih = AzVec2ih(src_window->Pos);
        if (src_window->ViewportId != 0
            && src_window->ViewportId != AZ_VIEWPORT_DEFAULT_ID)
        {
            dst_settings->ViewportPos = window_pos_2ih;
            dst_settings->ViewportId = src_window->ViewportId;
            dst_settings->Pos = AzVec2ih(0, 0);
        } else
        {
            dst_settings->Pos = window_pos_2ih;
        }
        dst_settings->Size = AzVec2ih(src_window->SizeFull);
        dst_settings->Collapsed = src_window->Collapsed;
    }
}

void Az::DockBuilderCopyDockSpace(AzID src_dockspace_id,
                                  AzID dst_dockspace_id,
                                  AzVector<const char*>* in_window_remap_pairs)
{
    AzContext& g = *GAz;
    AZ_ASSERT(src_dockspace_id != 0);
    AZ_ASSERT(dst_dockspace_id != 0);
    AZ_ASSERT(in_window_remap_pairs != NULL);
    AZ_ASSERT((in_window_remap_pairs->Size % 2) == 0);

    AzVector<AzID> node_remap_pairs;
    DockBuilderCopyNode(src_dockspace_id, dst_dockspace_id, &node_remap_pairs);

    AzVector<AzID> src_windows;
    for (int remap_window_n = 0; remap_window_n < in_window_remap_pairs->Size;
         remap_window_n += 2)
    {
        const char* src_window_name = (*in_window_remap_pairs)[remap_window_n];
        const char* dst_window_name =
                (*in_window_remap_pairs)[remap_window_n + 1];
        AzID src_window_id = AzHashStr(src_window_name);
        src_windows.push_back(src_window_id);

        AzID src_dock_id = 0;
        if (AzWindow* src_window = FindWindowByID(src_window_id))
            src_dock_id = src_window->DockId;
        else if (AzWindowSettings* src_window_settings = FindWindowSettingsByID(
                         src_window_id))
            src_dock_id = src_window_settings->DockId;
        AzID dst_dock_id = 0;
        for (int dock_remap_n = 0; dock_remap_n < node_remap_pairs.Size;
             dock_remap_n += 2)
            if (node_remap_pairs[dock_remap_n] == src_dock_id)
            {
                dst_dock_id = node_remap_pairs[dock_remap_n + 1];

                break;
            }

        if (dst_dock_id != 0)
        {
            AZ_DEBUG_LOG_DOCKING(
                    "[docking] Remap live window '%s' 0x%08X -> '%s' 0x%08X\n",
                    src_window_name,
                    src_dock_id,
                    dst_window_name,
                    dst_dock_id);
            DockBuilderDockWindow(dst_window_name, dst_dock_id);
        } else
        {
            AZ_DEBUG_LOG_DOCKING(
                    "[docking] Remap window settings '%s' -> '%s'\n",
                    src_window_name,
                    dst_window_name);
            DockBuilderCopyWindowSettings(src_window_name, dst_window_name);
        }
    }

    struct DockRemainingWindowTask
    {
        AzWindow* Window;
        AzID DockId;
        DockRemainingWindowTask(AzWindow* window, AzID dock_id)
        {
            Window = window;
            DockId = dock_id;
        }
    };
    AzVector<DockRemainingWindowTask> dock_remaining_windows;
    for (int dock_remap_n = 0; dock_remap_n < node_remap_pairs.Size;
         dock_remap_n += 2)
        if (AzID src_dock_id = node_remap_pairs[dock_remap_n])
        {
            AzID dst_dock_id = node_remap_pairs[dock_remap_n + 1];
            AzDockNode* node = DockBuilderGetNode(src_dock_id);
            for (int window_n = 0; window_n < node->Windows.Size; window_n++)
            {
                AzWindow* window = node->Windows[window_n];
                if (src_windows.contains(window->ID))
                    continue;

                AZ_DEBUG_LOG_DOCKING(
                        "[docking] Remap window '%s' %08X -> %08X\n",
                        window->Name,
                        src_dock_id,
                        dst_dock_id);
                dock_remaining_windows.push_back(
                        DockRemainingWindowTask(window, dst_dock_id));
            }
        }
    for (const DockRemainingWindowTask& task : dock_remaining_windows)
        DockBuilderDockWindow(task.Window->Name, task.DockId);
}

void Az::DockBuilderFinish(AzID root_id)
{
    AzContext& g = *GAz;

    DockContextBuildAddWindowsToNodes(&g, root_id);
}

bool Az::GetWindowAlwaysWantOwnTabBar(AzWindow* window)
{
    AzContext& g = *GAz;
    if (g.IO.ConfigDockingAlwaysTabBar
        || window->WindowClass.DockingAlwaysTabBar)
        if ((window->Flags
             & (AzWindowFlags_ChildWindow | AzWindowFlags_NoTitleBar
                | AzWindowFlags_NoDocking))
            == 0)
            if (!window->IsFallbackWindow)

                return true;
    return false;
}

static AzDockNode* Az::DockContextBindNodeToWindow(AzContext* ctx,
                                                   AzWindow* window)
{
    AzContext& g = *ctx;
    AzDockNode* node = DockContextFindNodeByID(ctx, window->DockId);
    AZ_ASSERT(window->DockNode == NULL);

    if (node && node->IsSplitNode())
    {
        DockContextProcessUndockWindow(ctx, window);
        return NULL;
    }

    if (node == NULL)
    {
        node = DockContextAddNode(ctx, window->DockId);
        node->AuthorityForPos = node->AuthorityForSize =
                node->AuthorityForViewport = AzDataAuthority_Window;
        node->LastFrameAlive = g.FrameCount;
    }

    if (!node->IsVisible)
    {
        AzDockNode* ancestor_node = node;
        while (!ancestor_node->IsVisible && ancestor_node->ParentNode)
            ancestor_node = ancestor_node->ParentNode;
        AZ_ASSERT(ancestor_node->Size.x > 0.0f && ancestor_node->Size.y > 0.0f);
        DockNodeUpdateHasCentralNodeChild(DockNodeGetRootNode(ancestor_node));
        DockNodeTreeUpdatePosSize(ancestor_node,
                                  ancestor_node->Pos,
                                  ancestor_node->Size,
                                  node);
    }

    bool node_was_visible = node->IsVisible;
    DockNodeAddWindow(node, window, true);
    node->IsVisible = node_was_visible;

    AZ_ASSERT(node == window->DockNode);
    return node;
}

static void StoreDockStyleForWindow(AzWindow* window)
{
    AzContext& g = *GAz;
    for (int color_n = 0; color_n < AzWindowDockStyleCol_COUNT; color_n++)
        window->DockStyle.Colors[color_n] = Az::ColorConvertFloat4ToU32(
                g.Style.Colors[GWindowDockStyleColors[color_n]]);
}

void Az::BeginDocked(AzWindow* window, bool* p_open)
{
    AzContext& g = *GAz;

    window->DockIsActive = window->DockNodeIsVisible =
            window->DockTabIsVisible = false;

    const bool auto_dock_node = GetWindowAlwaysWantOwnTabBar(window);
    if (auto_dock_node)
    {
        if (window->DockId == 0)
        {
            AZ_ASSERT(window->DockNode == NULL);
            window->DockId = DockContextGenNodeID(&g);
        }
    } else
    {
        bool want_undock = false;
        want_undock |= (window->Flags & AzWindowFlags_NoDocking) != 0;
        want_undock |= (g.NextWindowData.HasFlags
                        & AzNextWindowDataFlags_HasPos)
                       && (window->SetWindowPosAllowFlags
                           & g.NextWindowData.PosCond)
                       && g.NextWindowData.PosUndock;
        if (want_undock)
        {
            DockContextProcessUndockWindow(&g, window);
            return;
        }
    }

    AzDockNode* node = window->DockNode;
    if (node != NULL)
        AZ_ASSERT(window->DockId == node->ID);
    if (window->DockId != 0 && node == NULL)
    {
        node = DockContextBindNodeToWindow(&g, window);
        if (node == NULL)
            return;
    }

#if 0

    if (node->IsCentralNode && (node->Flags & AzDockNodeFlags_NoDockingInCentralNode))
    {
        DockContextProcessUndockWindow(ctx, window);
        return;
    }
#endif

    if (node->LastFrameAlive < g.FrameCount)
    {
        AzDockNode* root_node = DockNodeGetRootNode(node);
        if (root_node->LastFrameAlive < g.FrameCount)
            DockContextProcessUndockWindow(&g, window);
        else
            window->DockIsActive = true;
        return;
    }

    StoreDockStyleForWindow(window);

    if (node->HostWindow == NULL)
    {
        if (node->State
            == AzDockNodeState_HostWindowHiddenBecauseWindowsAreResizing)
            window->DockIsActive = true;
        if (node->Windows.Size > 1 && window->Appearing)
            DockNodeHideWindowDuringHostWindowCreation(window);
        return;
    }

    AZ_ASSERT(node->HostWindow);
    AZ_ASSERT(node->IsLeafNode());
    AZ_ASSERT(node->Size.x >= 0.0f && node->Size.y >= 0.0f);
    node->State = AzDockNodeState_HostWindowVisible;

    if (!(node->MergedFlags & AzDockNodeFlags_KeepAliveOnly)
        && window->BeginOrderWithinContext
                   < node->HostWindow->BeginOrderWithinContext)
    {
        DockContextProcessUndockWindow(&g, window);
        return;
    }

    SetNextWindowPos(node->Pos);
    SetNextWindowSize(node->Size);
    g.NextWindowData.PosUndock = false;

    window->DockIsActive = true;
    window->DockNodeIsVisible = true;
    window->DockTabIsVisible = false;
    if (node->MergedFlags & AzDockNodeFlags_KeepAliveOnly)
        return;

    if (node->VisibleWindow == window)
        window->DockTabIsVisible = true;

    AZ_ASSERT((window->Flags & AzWindowFlags_ChildWindow) == 0);
    window->Flags |= AzWindowFlags_ChildWindow | AzWindowFlags_NoResize;
    window->ChildFlags |= AzChildFlags_AlwaysUseWindowPadding;
    if (node->IsHiddenTabBar() || node->IsNoTabBar())
        window->Flags |= AzWindowFlags_NoTitleBar;
    else
        window->Flags &= ~AzWindowFlags_NoTitleBar;

    if (node->TabBar && window->WasActive)
        window->DockOrder = (short)DockNodeGetTabOrder(window);

    if ((node->WantCloseAll || node->WantCloseTabId == window->TabId)
        && p_open != NULL)
        *p_open = false;

    AzWindow* parent_window = window->DockNode->HostWindow;
    window->ChildId = parent_window->GetID(window->Name);
}

void Az::BeginDockableDragDropSource(AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.ActiveId == window->MoveId);
    AZ_ASSERT(g.MovingWindow == window);
    AZ_ASSERT(g.CurrentWindow == window);

    if (g.IO.ConfigDockingWithShift != g.IO.KeyShift)
    {
        AZ_ASSERT(g.NextWindowData.HasFlags == 0);
        if (g.IO.ConfigDockingWithShift && g.MouseStationaryTimer >= 1.0f
            && g.ActiveId >= 1.0f)
            SetTooltip("%s", LocalizeGetMsg(AzLocKey_DockingHoldShiftToDock));
        return;
    }

    g.LastItemData.ID = window->MoveId;
    window = window->RootWindowDockTree;
    AZ_ASSERT((window->Flags & AzWindowFlags_NoDocking) == 0);
    bool is_drag_docking = (g.IO.ConfigDockingWithShift)
                           || AzRect(0, 0, window->SizeFull.x, GetFrameHeight())
                                      .Contains(g.ActiveIdClickOffset);

    AzDragDropFlags drag_drop_flags = AzDragDropFlags_SourceNoPreviewTooltip
                                      | AzDragDropFlags_SourceNoHoldToOpenOthers
                                      | AzDragDropFlags_PayloadAutoExpire
                                      | AzDragDropFlags_PayloadNoCrossContext
                                      | AzDragDropFlags_PayloadNoCrossProcess;
    if (is_drag_docking && BeginDragDropSource(drag_drop_flags))
    {
        SetDragDropPayload(AZ_PAYLOAD_TYPE_WINDOW, &window, sizeof(window));
        EndDragDropSource();
        StoreDockStyleForWindow(window);
    }
}

void Az::BeginDockableDragDropTarget(AzWindow* window)
{
    AzContext& g = *GAz;

    AZ_ASSERT((window->Flags & AzWindowFlags_NoDocking) == 0);
    if (!g.DragDropActive)
        return;

    if (!BeginDragDropTargetCustom(window->Rect(), window->ID))
        return;

    const AzPayload* payload = &g.DragDropPayload;
    if (!payload->IsDataType(AZ_PAYLOAD_TYPE_WINDOW)
        || !DockNodeIsDropAllowed(window, *(AzWindow**)payload->Data))
    {
        EndDragDropTarget();
        return;
    }

    AzWindow* payload_window = *(AzWindow**)payload->Data;
    if (AcceptDragDropPayload(
                AZ_PAYLOAD_TYPE_WINDOW,
                AzDragDropFlags_AcceptBeforeDelivery
                        | AzDragDropFlags_AcceptNoDrawDefaultRect))
    {
        bool dock_into_floating_window = false;
        AzDockNode* node = NULL;
        if (window->DockNodeAsHost)
        {
            node = DockNodeTreeFindVisibleNodeByPos(window->DockNodeAsHost,
                                                    g.IO.MousePos);

            if (node && node->IsDockSpace() && node->IsRootNode())
                node = (node->CentralNode && node->IsLeafNode())
                               ? node->CentralNode
                               : DockNodeTreeFindFallbackLeafNode(node);
        } else
        {
            if (window->DockNode)
                node = window->DockNode;
            else
                dock_into_floating_window = true;
        }

        const AzRect explicit_target_rect =
                (node && node->TabBar && !node->IsHiddenTabBar()
                 && !node->IsNoTabBar())
                        ? node->TabBar->BarRect
                        : AzRect(window->Pos,
                                 window->Pos
                                         + AzVec2(window->Size.x,
                                                  GetFrameHeight()));
        const bool is_explicit_target =
                g.IO.ConfigDockingWithShift
                || IsMouseHoveringRect(explicit_target_rect.Min,
                                       explicit_target_rect.Max);

        const bool do_preview = payload->IsPreview() || payload->IsDelivery();
        if (do_preview && (node != NULL || dock_into_floating_window))
        {
            AzDockPreviewData split_inner;
            AzDockPreviewData split_outer;
            AzDockPreviewData* split_data = &split_inner;
            if (node
                && (node->ParentNode || node->IsCentralNode()
                    || !node->IsLeafNode()))
                if (AzDockNode* root_node = DockNodeGetRootNode(node))
                {
                    DockNodePreviewDockSetup(window,
                                             root_node,
                                             payload_window,
                                             NULL,
                                             &split_outer,
                                             is_explicit_target,
                                             true);
                    if (split_outer.IsSplitDirExplicit)
                        split_data = &split_outer;
                }
            if (!node || node->IsLeafNode())
                DockNodePreviewDockSetup(window,
                                         node,
                                         payload_window,
                                         NULL,
                                         &split_inner,
                                         is_explicit_target,
                                         false);
            if (split_data == &split_outer)
                split_inner.IsDropAllowed = false;

            DockNodePreviewDockRender(window,
                                      node,
                                      payload_window,
                                      &split_inner);
            DockNodePreviewDockRender(window,
                                      node,
                                      payload_window,
                                      &split_outer);

            if (split_data->IsDropAllowed && payload->IsDelivery())
                DockContextQueueDock(&g,
                                     window,
                                     split_data->SplitNode,
                                     payload_window,
                                     split_data->SplitDir,
                                     split_data->SplitRatio,
                                     split_data == &split_outer);
        }
    }
    EndDragDropTarget();
}

static void Az::DockSettingsRenameNodeReferences(AzID old_node_id,
                                                 AzID new_node_id)
{
    AzContext& g = *GAz;
    AZ_DEBUG_LOG_DOCKING(
            "[docking] DockSettingsRenameNodeReferences: from 0x%08X -> to "
            "0x%08X\n",
            old_node_id,
            new_node_id);
    for (int window_n = 0; window_n < g.Windows.Size; window_n++)
    {
        AzWindow* window = g.Windows[window_n];
        if (window->DockId == old_node_id && window->DockNode == NULL)
            window->DockId = new_node_id;
    }

    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
        if (settings->DockId == old_node_id)
            settings->DockId = new_node_id;
}

static void Az::DockSettingsRemoveNodeReferences(AzID* node_ids,
                                                 int node_ids_count)
{
    AzContext& g = *GAz;
    int found = 0;

    for (AzWindowSettings* settings = g.SettingsWindows.begin();
         settings != NULL;
         settings = g.SettingsWindows.next_chunk(settings))
        for (int node_n = 0; node_n < node_ids_count; node_n++)
            if (settings->DockId == node_ids[node_n])
            {
                settings->DockId = 0;
                settings->DockOrder = -1;
                if (++found < node_ids_count)
                    break;
                return;
            }
}

static AzDockNodeSettings* Az::DockSettingsFindNodeSettings(AzContext* ctx,
                                                            AzID id)
{
    AzDockContext* dc = &ctx->DockContext;
    for (int n = 0; n < dc->NodesSettings.Size; n++)
        if (dc->NodesSettings[n].ID == id)
            return &dc->NodesSettings[n];
    return NULL;
}

static void Az::DockSettingsHandler_ClearAll(AzContext* ctx, AzSettingsHandler*)
{
    AzDockContext* dc = &ctx->DockContext;
    dc->NodesSettings.clear();
    DockContextClearNodes(ctx, 0, true);
}

static void Az::DockSettingsHandler_ApplyAll(AzContext* ctx, AzSettingsHandler*)
{
    AzDockContext* dc = &ctx->DockContext;
    if (ctx->Windows.Size == 0)
        DockContextPruneUnusedSettingsNodes(ctx);
    DockContextBuildNodesFromSettings(ctx,
                                      dc->NodesSettings.Data,
                                      dc->NodesSettings.Size);
    DockContextBuildAddWindowsToNodes(ctx, 0);
}

static void* Az::DockSettingsHandler_ReadOpen(AzContext*,
                                              AzSettingsHandler*,
                                              const char* name)
{
    if (strcmp(name, "Data") != 0)
        return NULL;
    return (void*)1;
}

static void Az::DockSettingsHandler_ReadLine(AzContext* ctx,
                                             AzSettingsHandler*,
                                             void*,
                                             const char* line)
{
    char c = 0;
    int x = 0, y = 0;
    int r = 0;

    AzDockNodeSettings node;
    line = AzStrSkipBlank(line);
    if (strncmp(line, "DockNode", 8) == 0)
    {
        line = AzStrSkipBlank(line + strlen("DockNode"));
    } else if (strncmp(line, "DockSpace", 9) == 0)
    {
        line = AzStrSkipBlank(line + strlen("DockSpace"));
        node.Flags |= AzDockNodeFlags_DockSpace;
    } else
        return;
    if (sscanf(line, "ID=0x%08X%n", &node.ID, &r) == 1)
    {
        line += r;
    } else
        return;
    if (sscanf(line, " Parent=0x%08X%n", &node.ParentNodeId, &r) == 1)
    {
        line += r;
        if (node.ParentNodeId == 0)
            return;
    }
    if (sscanf(line, " Window=0x%08X%n", &node.ParentWindowId, &r) == 1)
    {
        line += r;
        if (node.ParentWindowId == 0)
            return;
    }
    if (node.ParentNodeId == 0)
    {
        if (sscanf(line, " Pos=%i,%i%n", &x, &y, &r) == 2)
        {
            line += r;
            node.Pos = AzVec2ih((short)x, (short)y);
        } else
            return;
        if (sscanf(line, " Size=%i,%i%n", &x, &y, &r) == 2)
        {
            line += r;
            node.Size = AzVec2ih((short)x, (short)y);
        } else
            return;
    } else
    {
        if (sscanf(line, " SizeRef=%i,%i%n", &x, &y, &r) == 2)
        {
            line += r;
            node.SizeRef = AzVec2ih((short)x, (short)y);
        }
    }
    if (sscanf(line, " Split=%c%n", &c, &r) == 1)
    {
        line += r;
        if (c == 'X')
            node.SplitAxis = AzAxis_X;
        else if (c == 'Y')
            node.SplitAxis = AzAxis_Y;
    }
    if (sscanf(line, " NoResize=%d%n", &x, &r) == 1)
    {
        line += r;
        if (x != 0)
            node.Flags |= AzDockNodeFlags_NoResize;
    }
    if (sscanf(line, " CentralNode=%d%n", &x, &r) == 1)
    {
        line += r;
        if (x != 0)
            node.Flags |= AzDockNodeFlags_CentralNode;
    }
    if (sscanf(line, " NoTabBar=%d%n", &x, &r) == 1)
    {
        line += r;
        if (x != 0)
            node.Flags |= AzDockNodeFlags_NoTabBar;
    }
    if (sscanf(line, " HiddenTabBar=%d%n", &x, &r) == 1)
    {
        line += r;
        if (x != 0)
            node.Flags |= AzDockNodeFlags_HiddenTabBar;
    }
    if (sscanf(line, " NoWindowMenuButton=%d%n", &x, &r) == 1)
    {
        line += r;
        if (x != 0)
            node.Flags |= AzDockNodeFlags_NoWindowMenuButton;
    }
    if (sscanf(line, " NoCloseButton=%d%n", &x, &r) == 1)
    {
        line += r;
        if (x != 0)
            node.Flags |= AzDockNodeFlags_NoCloseButton;
    }
    if (sscanf(line, " Selected=0x%08X%n", &node.SelectedTabId, &r) == 1)
    {
        line += r;
    }
    if (node.ParentNodeId != 0)
        if (AzDockNodeSettings* parent_settings =
                    DockSettingsFindNodeSettings(ctx, node.ParentNodeId))
            node.Depth = parent_settings->Depth + 1;
    ctx->DockContext.NodesSettings.push_back(node);
}

static void DockSettingsHandler_DockNodeToSettings(AzDockContext* dc,
                                                   AzDockNode* node,
                                                   int depth)
{
    AzDockNodeSettings node_settings;
    AZ_ASSERT(depth < (1 << (sizeof(node_settings.Depth) << 3)));
    node_settings.ID = node->ID;
    node_settings.ParentNodeId = node->ParentNode ? node->ParentNode->ID : 0;
    node_settings.ParentWindowId = (node->IsDockSpace() && node->HostWindow
                                    && node->HostWindow->ParentWindow)
                                           ? node->HostWindow->ParentWindow->ID
                                           : 0;
    node_settings.SelectedTabId = node->SelectedTabId;
    node_settings.SplitAxis = (signed char)(node->IsSplitNode()
                                                    ? node->SplitAxis
                                                    : AzAxis_None);
    node_settings.Depth = (char)depth;
    node_settings.Flags = (node->LocalFlags & AzDockNodeFlags_SavedFlagsMask_);
    node_settings.Pos = AzVec2ih(node->Pos);
    node_settings.Size = AzVec2ih(node->Size);
    node_settings.SizeRef = AzVec2ih(node->SizeRef);
    dc->NodesSettings.push_back(node_settings);
    if (node->ChildNodes[0])
        DockSettingsHandler_DockNodeToSettings(dc,
                                               node->ChildNodes[0],
                                               depth + 1);
    if (node->ChildNodes[1])
        DockSettingsHandler_DockNodeToSettings(dc,
                                               node->ChildNodes[1],
                                               depth + 1);
}

static void Az::DockSettingsHandler_WriteAll(AzContext* ctx,
                                             AzSettingsHandler* handler,
                                             AzTextBuffer* buf)
{
    AzContext& g = *ctx;
    AzDockContext* dc = &ctx->DockContext;
    if (!(g.IO.ConfigFlags & AzConfigFlags_DockingEnable))
        return;

    dc->NodesSettings.resize(0);
    dc->NodesSettings.reserve(dc->Nodes.Data.Size);
    for (int n = 0; n < dc->Nodes.Data.Size; n++)
        if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
            if (node->IsRootNode())
                DockSettingsHandler_DockNodeToSettings(dc, node, 0);

    int max_depth = 0;
    for (int node_n = 0; node_n < dc->NodesSettings.Size; node_n++)
        max_depth = AzMax((int)dc->NodesSettings[node_n].Depth, max_depth);

    buf->appendf("[%s][Data]\n", handler->TypeName);
    for (int node_n = 0; node_n < dc->NodesSettings.Size; node_n++)
    {
        const int line_start_pos = buf->size();
        (void)line_start_pos;
        const AzDockNodeSettings* node_settings = &dc->NodesSettings[node_n];
        buf->appendf("%*s%s%*s",
                     node_settings->Depth * 2,
                     "",
                     (node_settings->Flags & AzDockNodeFlags_DockSpace)
                             ? "DockSpace"
                             : "DockNode ",
                     (max_depth - node_settings->Depth) * 2,
                     "");
        buf->appendf(" ID=0x%08X", node_settings->ID);
        if (node_settings->ParentNodeId)
        {
            buf->appendf(" Parent=0x%08X SizeRef=%d,%d",
                         node_settings->ParentNodeId,
                         node_settings->SizeRef.x,
                         node_settings->SizeRef.y);
        } else
        {
            if (node_settings->ParentWindowId)
                buf->appendf(" Window=0x%08X", node_settings->ParentWindowId);
            buf->appendf(" Pos=%d,%d Size=%d,%d",
                         node_settings->Pos.x,
                         node_settings->Pos.y,
                         node_settings->Size.x,
                         node_settings->Size.y);
        }
        if (node_settings->SplitAxis != AzAxis_None)
            buf->appendf(" Split=%c",
                         (node_settings->SplitAxis == AzAxis_X) ? 'X' : 'Y');
        if (node_settings->Flags & AzDockNodeFlags_NoResize)
            buf->appendf(" NoResize=1");
        if (node_settings->Flags & AzDockNodeFlags_CentralNode)
            buf->appendf(" CentralNode=1");
        if (node_settings->Flags & AzDockNodeFlags_NoTabBar)
            buf->appendf(" NoTabBar=1");
        if (node_settings->Flags & AzDockNodeFlags_HiddenTabBar)
            buf->appendf(" HiddenTabBar=1");
        if (node_settings->Flags & AzDockNodeFlags_NoWindowMenuButton)
            buf->appendf(" NoWindowMenuButton=1");
        if (node_settings->Flags & AzDockNodeFlags_NoCloseButton)
            buf->appendf(" NoCloseButton=1");
        if (node_settings->SelectedTabId)
            buf->appendf(" Selected=0x%08X", node_settings->SelectedTabId);

        if (g.IO.ConfigDebugIniSettings)
            if (AzDockNode* node = DockContextFindNodeByID(ctx,
                                                           node_settings->ID))
            {
                buf->appendf("%*s",
                             AzMax(2, (line_start_pos + 92) - buf->size()),
                             "");
                if (node->IsDockSpace() && node->HostWindow
                    && node->HostWindow->ParentWindow)
                    buf->appendf(" ; in '%s'",
                                 node->HostWindow->ParentWindow->Name);

                int contains_window = 0;
                for (AzWindowSettings* settings = g.SettingsWindows.begin();
                     settings != NULL;
                     settings = g.SettingsWindows.next_chunk(settings))
                    if (settings->DockId == node_settings->ID)
                    {
                        if (contains_window++ == 0)
                            buf->appendf(" ; contains ");
                        buf->appendf("'%s' ", settings->GetName());
                    }
            }

        buf->appendf("\n");
    }
    buf->appendf("\n");
}

#if defined(_WIN32) && !defined(AZ_DISABLE_WIN32_FUNCTIONS)                    \
        && !defined(AZ_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS)

#ifdef _MSC_VER
#pragma comment(lib, "user32")
#pragma comment(lib, "kernel32")
#endif

static const char* Platform_GetClipboardTextFn_DefaultAzpl(AzContext* ctx)
{
    AzContext& g = *ctx;
    g.ClipboardHandlerData.clear();
    if (!::OpenClipboard(NULL))
        return NULL;
    HANDLE wbuf_handle = ::GetClipboardData(CF_UNICODETEXT);
    if (wbuf_handle == NULL)
    {
        ::CloseClipboard();
        return NULL;
    }
    if (const WCHAR* wbuf_global = (const WCHAR*)::GlobalLock(wbuf_handle))
    {
        int buf_len = ::WideCharToMultiByte(CP_UTF8,
                                            0,
                                            wbuf_global,
                                            -1,
                                            NULL,
                                            0,
                                            NULL,
                                            NULL);
        g.ClipboardHandlerData.resize(buf_len);
        ::WideCharToMultiByte(CP_UTF8,
                              0,
                              wbuf_global,
                              -1,
                              g.ClipboardHandlerData.Data,
                              buf_len,
                              NULL,
                              NULL);
    }
    ::GlobalUnlock(wbuf_handle);
    ::CloseClipboard();
    return g.ClipboardHandlerData.Data;
}

static void Platform_SetClipboardTextFn_DefaultAzpl(AzContext*,
                                                    const char* text)
{
    if (!::OpenClipboard(NULL))
        return;
    const int wbuf_length =
            ::MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    HGLOBAL wbuf_handle = ::GlobalAlloc(GMEM_MOVEABLE,
                                        (SIZE_T)wbuf_length * sizeof(WCHAR));
    if (wbuf_handle == NULL)
    {
        ::CloseClipboard();
        return;
    }
    WCHAR* wbuf_global = (WCHAR*)::GlobalLock(wbuf_handle);
    ::MultiByteToWideChar(CP_UTF8, 0, text, -1, wbuf_global, wbuf_length);
    ::GlobalUnlock(wbuf_handle);
    ::EmptyClipboard();
    if (::SetClipboardData(CF_UNICODETEXT, wbuf_handle) == NULL)
        ::GlobalFree(wbuf_handle);
    ::CloseClipboard();
}

#elif defined(__APPLE__) && TARGET_OS_OSX                                      \
        && defined(AZ_ENABLE_OSX_DEFAULT_CLIPBOARD_FUNCTIONS)

#include <Carbon/Carbon.h>
static PasteboardRef main_clipboard = 0;

static void Platform_SetClipboardTextFn_DefaultAzpl(AzContext*,
                                                    const char* text)
{
    if (!main_clipboard)
        PasteboardCreate(kPasteboardClipboard, &main_clipboard);
    PasteboardClear(main_clipboard);
    CFDataRef cf_data = CFDataCreate(kCFAllocatorDefault,
                                     (const UInt8*)text,
                                     AzStrlen(text));
    if (cf_data)
    {
        PasteboardPutItemFlavor(main_clipboard,
                                (PasteboardItemID)1,
                                CFSTR("public.utf8-plain-text"),
                                cf_data,
                                0);
        CFRelease(cf_data);
    }
}

static const char* Platform_GetClipboardTextFn_DefaultAzpl(AzContext* ctx)
{
    AzContext& g = *ctx;
    if (!main_clipboard)
        PasteboardCreate(kPasteboardClipboard, &main_clipboard);
    PasteboardSynchronize(main_clipboard);

    ItemCount item_count = 0;
    PasteboardGetItemCount(main_clipboard, &item_count);
    for (ItemCount i = 0; i < item_count; i++)
    {
        PasteboardItemID item_id = 0;
        PasteboardGetItemIdentifier(main_clipboard, i + 1, &item_id);
        CFArrayRef flavor_type_array = 0;
        PasteboardCopyItemFlavors(main_clipboard, item_id, &flavor_type_array);
        for (CFIndex j = 0, nj = CFArrayGetCount(flavor_type_array); j < nj;
             j++)
        {
            CFDataRef cf_data;
            if (PasteboardCopyItemFlavorData(main_clipboard,
                                             item_id,
                                             CFSTR("public.utf8-plain-text"),
                                             &cf_data)
                == noErr)
            {
                g.ClipboardHandlerData.clear();
                int length = (int)CFDataGetLength(cf_data);
                g.ClipboardHandlerData.resize(length + 1);
                CFDataGetBytes(cf_data,
                               CFRangeMake(0, length),
                               (UInt8*)g.ClipboardHandlerData.Data);
                g.ClipboardHandlerData[length] = 0;
                CFRelease(cf_data);
                return g.ClipboardHandlerData.Data;
            }
        }
    }
    return NULL;
}

#else

static const char* Platform_GetClipboardTextFn_DefaultAzpl(AzContext* ctx)
{
    AzContext& g = *ctx;
    return g.ClipboardHandlerData.empty() ? NULL
                                          : g.ClipboardHandlerData.begin();
}

static void Platform_SetClipboardTextFn_DefaultAzpl(AzContext* ctx,
                                                    const char* text)
{
    AzContext& g = *ctx;
    g.ClipboardHandlerData.clear();
    const char* text_end = text + AzStrlen(text);
    g.ClipboardHandlerData.resize((int)(text_end - text) + 1);
    memcpy(&g.ClipboardHandlerData[0], text, (size_t)(text_end - text));
    g.ClipboardHandlerData[(int)(text_end - text)] = 0;
}

#endif

#ifndef AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS
#if defined(__APPLE__) && TARGET_OS_IPHONE
#define AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS
#endif
#if defined(__3DS__)
#define AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS
#endif
#if defined(_WIN32) && defined(AZ_DISABLE_WIN32_FUNCTIONS)
#define AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS
#endif
#endif

#ifndef AZ_DISABLE_DEFAULT_SHELL_FUNCTIONS
#ifdef _WIN32
#include <shellapi.h>
#ifdef _MSC_VER
#pragma comment(lib, "shell32")
#endif
static bool Platform_OpenInShellFn_DefaultAzpl(AzContext*, const char* path)
{
    const int path_wsize = ::MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    AzVector<wchar_t> path_wbuf;
    path_wbuf.resize(path_wsize);
    ::MultiByteToWideChar(CP_UTF8, 0, path, -1, path_wbuf.Data, path_wsize);
    return (INT_PTR)::ShellExecuteW(NULL,
                                    L"open",
                                    path_wbuf.Data,
                                    NULL,
                                    NULL,
                                    SW_SHOWDEFAULT)
           > 32;
}
#else
#include <sys/wait.h>
#include <unistd.h>
static bool Platform_OpenInShellFn_DefaultAzpl(AzContext*, const char* path)
{
#if defined(__APPLE__)
    const char* args[] {"open", "--", path, NULL};
#else
    const char* args[] {"xdg-open", path, NULL};
#endif
    pid_t pid = fork();
    if (pid < 0)
        return false;
    if (!pid)
    {
        execvp(args[0], const_cast<char**>(args));
        exit(-1);
    } else
    {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status) == 0;
    }
}
#endif
#else
static bool Platform_OpenInShellFn_DefaultAzpl(AzContext*, const char*)
{
    return false;
}
#endif

#if defined(_WIN32) && !defined(AZ_DISABLE_WIN32_FUNCTIONS)                    \
        && !defined(AZ_DISABLE_WIN32_DEFAULT_AZE_FUNCTIONS)

#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif

static void Platform_SetAzeDataFn_DefaultAzpl(AzContext*,
                                              AzViewport* viewport,
                                              AzPlatformAzeData* data)
{
    HWND hwnd = (HWND)viewport->PlatformHandleRaw;
    if (hwnd == 0)
        return;

    if (HIMC himc = ::ImmGetContext(hwnd))
    {
        COMPOSITIONFORM composition_form = {};
        composition_form.ptCurrentPos.x = (LONG)(data->InputPos.x
                                                 - viewport->Pos.x);
        composition_form.ptCurrentPos.y = (LONG)(data->InputPos.y
                                                 - viewport->Pos.y);
        composition_form.dwStyle = CFS_FORCE_POSITION;
        ::ImmSetCompositionWindow(himc, &composition_form);
        CANDIDATEFORM candidate_form = {};
        candidate_form.dwStyle = CFS_CANDIDATEPOS;
        candidate_form.ptCurrentPos.x = (LONG)(data->InputPos.x
                                               - viewport->Pos.x);
        candidate_form.ptCurrentPos.y = (LONG)(data->InputPos.y
                                               - viewport->Pos.y);
        ::ImmSetCandidateWindow(himc, &candidate_form);
        ::ImmReleaseContext(hwnd, himc);
    }
}

#else

static void Platform_SetAzeDataFn_DefaultAzpl(AzContext*,
                                              AzViewport*,
                                              AzPlatformAzeData*)
{}

#endif

#ifndef AZ_DISABLE_DEBUG_TOOLS

void Az::DebugRenderViewportThumbnail(AzDrawList* draw_list,
                                      AzViewportP* viewport,
                                      const AzRect& bb)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    AzVec2 scale = bb.GetSize() / viewport->Size;
    AzVec2 off = bb.Min - viewport->Pos * scale;
    float alpha_mul = (viewport->Flags & AzViewportFlags_IsMinimized) ? 0.30f
                                                                      : 1.00f;
    window->DrawList->AddRectFilled(bb.Min,
                                    bb.Max,
                                    GetColorU32(AzCol_Border,
                                                alpha_mul * 0.40f));
    for (AzWindow* thumb_window : g.Windows)
    {
        if (!thumb_window->WasActive
            || (thumb_window->Flags & AzWindowFlags_ChildWindow))
            continue;
        if (thumb_window->Viewport != viewport)
            continue;

        AzRect thumb_r = thumb_window->Rect();
        AzRect title_r = thumb_window->TitleBarRect();
        thumb_r = AzRect(AzTrunc(off + thumb_r.Min * scale),
                         AzTrunc(off + thumb_r.Max * scale));
        title_r = AzRect(AzTrunc(off + title_r.Min * scale),
                         AzTrunc(off
                                 + AzVec2(title_r.Max.x,
                                          title_r.Min.y
                                                  + title_r.GetHeight() * 3.0f)
                                           * scale));
        thumb_r.ClipWithFull(bb);
        title_r.ClipWithFull(bb);
        const bool window_is_focused =
                (g.NavWindow
                 && thumb_window->RootWindowForTitleBarHighlight
                            == g.NavWindow->RootWindowForTitleBarHighlight);
        window->DrawList->AddRectFilled(thumb_r.Min,
                                        thumb_r.Max,
                                        GetColorU32(AzCol_WindowBg, alpha_mul));
        window->DrawList->AddRectFilled(
                title_r.Min,
                title_r.Max,
                GetColorU32(window_is_focused ? AzCol_TitleBgActive
                                              : AzCol_TitleBg,
                            alpha_mul));
        window->DrawList->AddRect(thumb_r.Min,
                                  thumb_r.Max,
                                  GetColorU32(AzCol_Border, alpha_mul));
        window->DrawList->AddText(g.Font,
                                  g.FontSize * 1.0f,
                                  title_r.Min,
                                  GetColorU32(AzCol_Text, alpha_mul),
                                  thumb_window->Name,
                                  FindRenderedTextEnd(thumb_window->Name));
    }
    draw_list->AddRect(bb.Min, bb.Max, GetColorU32(AzCol_Border, alpha_mul));
    if (viewport->ID == g.DebugMetricsConfig.HighlightViewportID)
        window->DrawList->AddRect(bb.Min, bb.Max, AZ_COL32(255, 255, 0, 255));
}

static void RenderViewportsThumbnails()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    float SCALE = 1.0f / 8.0f;
    AzRect bb_full(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (AzPlatformMonitor& monitor : g.PlatformIO.Monitors)
        bb_full.Add(
                AzRect(monitor.MainPos, monitor.MainPos + monitor.MainSize));
    AzVec2 p = window->DC.CursorPos;
    AzVec2 off = p - bb_full.Min * SCALE;
    for (AzPlatformMonitor& monitor : g.PlatformIO.Monitors)
    {
        AzRect monitor_draw_bb(off + (monitor.MainPos) * SCALE,
                               off
                                       + (monitor.MainPos + monitor.MainSize)
                                                 * SCALE);
        window->DrawList->AddRect(monitor_draw_bb.Min,
                                  monitor_draw_bb.Max,
                                  (g.DebugMetricsConfig.HighlightMonitorIdx
                                   == g.PlatformIO.Monitors.index_from_ptr(
                                           &monitor))
                                          ? AZ_COL32(255, 255, 0, 255)
                                          : Az::GetColorU32(AzCol_Border),
                                  4.0f);
        window->DrawList->AddRectFilled(monitor_draw_bb.Min,
                                        monitor_draw_bb.Max,
                                        Az::GetColorU32(AzCol_Border, 0.10f),
                                        4.0f);
    }

    for (AzViewportP* viewport : g.Viewports)
    {
        AzRect viewport_draw_bb(off + (viewport->Pos) * SCALE,
                                off + (viewport->Pos + viewport->Size) * SCALE);
        Az::DebugRenderViewportThumbnail(window->DrawList,
                                         viewport,
                                         viewport_draw_bb);
    }
    Az::Dummy(bb_full.GetSize() * SCALE);
}

static int AZ_CDECL ViewportComparerByLastFocusedStampCount(const void* lhs,
                                                            const void* rhs)
{
    const AzViewportP* a = *(const AzViewportP* const*)lhs;
    const AzViewportP* b = *(const AzViewportP* const*)rhs;
    return b->LastFocusedStampCount - a->LastFocusedStampCount;
}

void Az::DebugRenderKeyboardPreview(AzDrawList* draw_list)
{
    const float scale = Az::GetFontSize() / 13.0f;
    const AzVec2 key_size = AzVec2(35.0f, 35.0f) * scale;
    const float key_rounding = 3.0f * scale;
    const AzVec2 key_face_size = AzVec2(25.0f, 25.0f) * scale;
    const AzVec2 key_face_pos = AzVec2(5.0f, 3.0f) * scale;
    const float key_face_rounding = 2.0f * scale;
    const AzVec2 key_label_pos = AzVec2(7.0f, 4.0f) * scale;
    const AzVec2 key_step = AzVec2(key_size.x - 1.0f, key_size.y - 1.0f);
    const float key_row_offset = 9.0f * scale;

    AzVec2 board_min = GetCursorScreenPos();
    AzVec2 board_max = AzVec2(board_min.x + 3 * key_step.x + 2 * key_row_offset
                                      + 10.0f,
                              board_min.y + 3 * key_step.y + 10.0f);
    AzVec2 start_pos = AzVec2(board_min.x + 5.0f - key_step.x, board_min.y);

    struct KeyLayoutData
    {
        int Row, Col;
        const char* Label;
        AzKey Key;
    };
    const KeyLayoutData keys_to_display[] = {{0, 0, "", AzKey_Tab},
                                             {0, 1, "Q", AzKey_Q},
                                             {0, 2, "W", AzKey_W},
                                             {0, 3, "E", AzKey_E},
                                             {0, 4, "R", AzKey_R},
                                             {1, 0, "", AzKey_CapsLock},
                                             {1, 1, "A", AzKey_A},
                                             {1, 2, "S", AzKey_S},
                                             {1, 3, "D", AzKey_D},
                                             {1, 4, "F", AzKey_F},
                                             {2, 0, "", AzKey_LeftShift},
                                             {2, 1, "Z", AzKey_Z},
                                             {2, 2, "X", AzKey_X},
                                             {2, 3, "C", AzKey_C},
                                             {2, 4, "V", AzKey_V}};

    Dummy(board_max - board_min);
    if (!IsItemVisible())
        return;
    draw_list->PushClipRect(board_min, board_max, true);
    for (int n = 0; n < AZ_ARRAYSIZE(keys_to_display); n++)
    {
        const KeyLayoutData* key_data = &keys_to_display[n];
        AzVec2 key_min = AzVec2(start_pos.x + key_data->Col * key_step.x
                                        + key_data->Row * key_row_offset,
                                start_pos.y + key_data->Row * key_step.y);
        AzVec2 key_max = key_min + key_size;
        draw_list->AddRectFilled(key_min,
                                 key_max,
                                 AZ_COL32(204, 204, 204, 255),
                                 key_rounding);
        draw_list->AddRect(key_min,
                           key_max,
                           AZ_COL32(24, 24, 24, 255),
                           key_rounding);
        AzVec2 face_min = AzVec2(key_min.x + key_face_pos.x,
                                 key_min.y + key_face_pos.y);
        AzVec2 face_max = AzVec2(face_min.x + key_face_size.x,
                                 face_min.y + key_face_size.y);
        draw_list->AddRect(face_min,
                           face_max,
                           AZ_COL32(193, 193, 193, 255),
                           key_face_rounding,
                           AzDrawFlags_None,
                           2.0f);
        draw_list->AddRectFilled(face_min,
                                 face_max,
                                 AZ_COL32(252, 252, 252, 255),
                                 key_face_rounding);
        AzVec2 label_min = AzVec2(key_min.x + key_label_pos.x,
                                  key_min.y + key_label_pos.y);
        draw_list->AddText(label_min,
                           AZ_COL32(64, 64, 64, 255),
                           key_data->Label);
        if (IsKeyDown(key_data->Key))
            draw_list->AddRectFilled(key_min,
                                     key_max,
                                     AZ_COL32(255, 0, 0, 128),
                                     key_rounding);
    }
    draw_list->PopClipRect();
}

void Az::DebugTextEncoding(const char* str)
{
    Text("Text: \"%s\"", str);
    if (!BeginTable("##DebugTextEncoding",
                    4,
                    AzTableFlags_Borders | AzTableFlags_RowBg
                            | AzTableFlags_SizingFixedFit
                            | AzTableFlags_Resizable))
        return;
    TableSetupColumn("Offset");
    TableSetupColumn("UTF-8");
    TableSetupColumn("Glyph");
    TableSetupColumn("Codepoint");
    TableHeadersRow();
    for (const char* p = str; *p != 0;)
    {
        unsigned int c;
        const int c_utf8_len = AzTextCharFromUtf8(&c, p, NULL);
        TableNextColumn();
        Text("%d", (int)(p - str));
        TableNextColumn();
        for (int byte_index = 0; byte_index < c_utf8_len; byte_index++)
        {
            if (byte_index > 0)
                SameLine();
            Text("0x%02X", (int)(unsigned char)p[byte_index]);
        }
        TableNextColumn();
        TextUnformatted(p, p + c_utf8_len);
        if (!GetFont()->IsGlyphInFont((AzWchar)c))
        {
            SameLine();
            TextUnformatted("[missing]");
        }
        TableNextColumn();
        Text("U+%04X", (int)c);
        p += c_utf8_len;
    }
    EndTable();
}

static void DebugFlashStyleColorStop()
{
    AzContext& g = *GAz;
    if (g.DebugFlashStyleColorIdx != AzCol_COUNT)
        g.Style.Colors[g.DebugFlashStyleColorIdx] =
                g.DebugFlashStyleColorBackup;
    g.DebugFlashStyleColorIdx = AzCol_COUNT;
}

void Az::DebugFlashStyleColor(AzCol idx)
{
    AzContext& g = *GAz;
    DebugFlashStyleColorStop();
    g.DebugFlashStyleColorTime = 0.5f;
    g.DebugFlashStyleColorIdx = idx;
    g.DebugFlashStyleColorBackup = g.Style.Colors[idx];
}

void Az::UpdateDebugToolFlashStyleColor()
{
    AzContext& g = *GAz;
    if (g.DebugFlashStyleColorTime <= 0.0f)
        return;
    ColorConvertHSVtoRGB(AzCos(g.DebugFlashStyleColorTime * 6.0f) * 0.5f + 0.5f,
                         0.5f,
                         0.5f,
                         g.Style.Colors[g.DebugFlashStyleColorIdx].x,
                         g.Style.Colors[g.DebugFlashStyleColorIdx].y,
                         g.Style.Colors[g.DebugFlashStyleColorIdx].z);
    g.Style.Colors[g.DebugFlashStyleColorIdx].w = 1.0f;
    if ((g.DebugFlashStyleColorTime -= g.IO.DeltaTime) <= 0.0f)
        DebugFlashStyleColorStop();
}

static const char* FormatTextureIDForDebugDisplay(char* buf,
                                                  int buf_size,
                                                  AzTextureID tex_id)
{
    union
    {
        void* ptr;
        int integer;
    } tex_id_opaque;
    memcpy(&tex_id_opaque, &tex_id, AzMin(sizeof(void*), sizeof(tex_id)));
    if (sizeof(tex_id) >= sizeof(void*))
        AzFormatString(buf, buf_size, "0x%p", tex_id_opaque.ptr);
    else
        AzFormatString(buf, buf_size, "0x%04X", tex_id_opaque.integer);
    return buf;
}

static const char* FormatTextureIDForDebugDisplay(char* buf,
                                                  int buf_size,
                                                  const AzDrawCmd* cmd)
{
    char* buf_end = buf + buf_size;
    if (cmd->TexRef._TexData != NULL)
        buf += AzFormatString(buf,
                              buf_end - buf,
                              "#%03d: ",
                              cmd->TexRef._TexData->UniqueID);
    return FormatTextureIDForDebugDisplay(buf,
                                          (int)(buf_end - buf),
                                          cmd->TexRef.GetTexID());
}

static void MetricsHelpMarker(const char* desc)
{
    Az::TextDisabled("(?)");
    if (Az::BeginItemTooltip())
    {
        Az::PushTextWrapPos(Az::GetFontSize() * 35.0f);
        Az::TextUnformatted(desc);
        Az::PopTextWrapPos();
        Az::EndTooltip();
    }
}

#ifdef AZ_ENABLE_FREETYPE
namespace AzFreeType
{
    AZ_API const AzFontLoader* GetFontLoader();
    AZ_API bool DebugEditFontLoaderFlags(unsigned int* p_font_builder_flags);
} // namespace AzFreeType
#endif

void Az::ShowFontAtlas(AzFontAtlas* atlas)
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;
    AzStyle& style = g.Style;

    BeginDisabled();
    CheckboxFlags("io.BackendFlags: RendererHasTextures",
                  &io.BackendFlags,
                  AzBackendFlags_RendererHasTextures);
    EndDisabled();

    if (DragFloat("FontSizeBase",
                  &style.FontSizeBase,
                  0.20f,
                  5.0f,
                  100.0f,
                  "%.0f"))
        style._NextFrameFontSizeBase = style.FontSizeBase;

    SameLine(0.0f, 0.0f);
    Text(" (out %.2f)", GetFontSize());
    SameLine();
    MetricsHelpMarker(
            "- This is scaling font only. General scaling will come later.");
    DragFloat("FontScaleMain", &style.FontScaleMain, 0.02f, 0.5f, 5.0f);

    DragFloat("FontScaleDpi", &style.FontScaleDpi, 0.02f, 0.5f, 5.0f);

    BulletText(
            "Warning: Font scaling will NOT be smooth, "
            "because\nAzBackendFlags_RendererHasTextures is not set!");
    BulletText("Load a nice font for better results!");
    BulletText("Please submit feedback:");
    SameLine();
    TextLinkOpenURL("#8465", "https://github.com/ocornut/imgui/issues/8465");
    BulletText("Read FAQ for more details:");
    SameLine();
    TextLinkOpenURL("dearimgui.com/faq", "https://www.dearimgui.com/faq/");

    SeparatorText("Font List");

    AzMetricsConfig* cfg = &g.DebugMetricsConfig;
    Checkbox("Show font preview", &cfg->ShowFontPreview);

    if (TreeNode("Loader",
                 "Loader: \'%s\'",
                 atlas->FontLoaderName ? atlas->FontLoaderName : "NULL"))
    {
        const AzFontLoader* loader_current = atlas->FontLoader;
        BeginDisabled(!atlas->RendererHasTextures);
#ifdef AZ_ENABLE_STB_TRUETYPE
        const AzFontLoader* loader_stbtruetype =
                AzFontAtlasGetFontLoaderForStbTruetype();
        if (RadioButton("stb_truetype", loader_current == loader_stbtruetype))
            AzFontAtlasBuildSetupFontLoader(atlas, loader_stbtruetype);
#else
        BeginDisabled();
        RadioButton("stb_truetype", false);
        SetItemTooltip("Requires #define AZ_ENABLE_STB_TRUETYPE");
        EndDisabled();
#endif
        SameLine();
#ifdef AZ_ENABLE_FREETYPE
        const AzFontLoader* loader_freetype = AzFreeType::GetFontLoader();
        if (RadioButton("FreeType", loader_current == loader_freetype))
            AzFontAtlasBuildSetupFontLoader(atlas, loader_freetype);
        if (loader_current == loader_freetype)
        {
            unsigned int loader_flags = atlas->FontLoaderFlags;
            Text("Shared FreeType Loader Flags:  0x%08", loader_flags);
            if (AzFreeType::DebugEditFontLoaderFlags(&loader_flags))
            {
                for (AzFont* font : atlas->Fonts)
                    AzFontAtlasFontDestroyOutput(atlas, font);
                atlas->FontLoaderFlags = loader_flags;
                for (AzFont* font : atlas->Fonts)
                    AzFontAtlasFontInitOutput(atlas, font);
            }
        }
#else
        BeginDisabled();
        RadioButton("FreeType", false);
        SetItemTooltip(
                "Requires #define AZ_ENABLE_FREETYPE + imgui_freetype.cpp.");
        EndDisabled();
#endif
        EndDisabled();
        TreePop();
    }

    for (AzFont* font : atlas->Fonts)
    {
        PushID(font);
        DebugNodeFont(font);
        PopID();
    }

    SeparatorText("Font Atlas");
    if (Button("Compact"))
        atlas->CompactCache();
    SameLine();
    if (Button("Grow"))
        AzFontAtlasTextureGrow(atlas);
    SameLine();
    if (Button("Clear All"))
        AzFontAtlasBuildClear(atlas);
    SetItemTooltip("Destroy cache and custom rectangles.");

    for (int tex_n = 0; tex_n < atlas->TexList.Size; tex_n++)
    {
        AzTextureData* tex = atlas->TexList[tex_n];
        if (tex_n > 0)
            SameLine();
        Text("Tex: %dx%d", tex->Width, tex->Height);
    }
    const int packed_surface_sqrt = (int)sqrtf(
            (float)atlas->Builder->RectsPackedSurface);
    const int discarded_surface_sqrt = (int)sqrtf(
            (float)atlas->Builder->RectsDiscardedSurface);
    Text("Packed rects: %d, area: about %d px ~%dx%d px",
         atlas->Builder->RectsPackedCount,
         atlas->Builder->RectsPackedSurface,
         packed_surface_sqrt,
         packed_surface_sqrt);
    Text("incl. Discarded rects: %d, area: about %d px ~%dx%d px",
         atlas->Builder->RectsDiscardedCount,
         atlas->Builder->RectsDiscardedSurface,
         discarded_surface_sqrt,
         discarded_surface_sqrt);

    AzFontAtlasRectId highlight_r_id = AzFontAtlasRectId_Invalid;
    if (TreeNode("Rects Index",
                 "Rects Index (%d)",
                 atlas->Builder->RectsPackedCount))

    {
        PushStyleVar(AzStyleVar_AzageBorderSize, 1.0f);
        if (BeginTable("##table",
                       2,
                       AzTableFlags_RowBg | AzTableFlags_Borders
                               | AzTableFlags_ScrollY,
                       AzVec2(0.0f, GetTextLineHeightWithSpacing() * 12)))
        {
            for (const AzFontAtlasRectEntry& entry : atlas->Builder->RectsIndex)
                if (entry.IsUsed)
                {
                    AzFontAtlasRectId id = AzFontAtlasRectId_Make(
                            atlas->Builder->RectsIndex.index_from_ptr(&entry),
                            entry.Generation);
                    AzFontAtlasRect r = {};
                    atlas->GetCustomRect(id, &r);
                    const char* buf;
                    AzFormatStringToTempBuffer(&buf,
                                               NULL,
                                               "ID:%08X, used:%d, { w:%3d, "
                                               "h:%3d } { x:%4d, y:%4d }",
                                               id,
                                               entry.IsUsed,
                                               r.w,
                                               r.h,
                                               r.x,
                                               r.y);
                    TableNextColumn();
                    Selectable(buf);
                    if (IsItemHovered())
                        highlight_r_id = id;
                    TableNextColumn();
                    Azage(atlas->TexRef, AzVec2(r.w, r.h), r.uv0, r.uv1);
                }
            EndTable();
        }
        PopStyleVar();
        TreePop();
    }

    AzFontAtlasRect highlight_r;
    if (highlight_r_id != AzFontAtlasRectId_Invalid)
        atlas->GetCustomRect(highlight_r_id, &highlight_r);
    for (int tex_n = 0; tex_n < atlas->TexList.Size; tex_n++)
    {
        if (tex_n == atlas->TexList.Size - 1)
            SetNextItemOpen(true, AzCond_Once);
        DebugNodeTexture(atlas->TexList[tex_n],
                         atlas->TexList.Size - 1 - tex_n,
                         (highlight_r_id != AzFontAtlasRectId_Invalid)
                                 ? &highlight_r
                                 : NULL);
    }
}

void Az::DebugNodeTexture(AzTextureData* tex,
                          int int_id,
                          const AzFontAtlasRect* highlight_rect)
{
    AzContext& g = *GAz;
    PushID(int_id);
    if (TreeNode("",
                 "Texture #%03d (%dx%d pixels)",
                 tex->UniqueID,
                 tex->Width,
                 tex->Height))
    {
        AzMetricsConfig* cfg = &g.DebugMetricsConfig;
        Checkbox("Show used rect", &cfg->ShowTextureUsedRect);
        PushStyleVar(AzStyleVar_AzageBorderSize,
                     AzMax(1.0f, g.Style.AzageBorderSize));
        AzVec2 p = GetCursorScreenPos();
        if (tex->WantDestroyNextFrame)
            Dummy(AzVec2((float)tex->Width, (float)tex->Height));
        else
            AzageWithBg(tex->GetTexRef(),
                        AzVec2((float)tex->Width, (float)tex->Height),
                        AzVec2(0.0f, 0.0f),
                        AzVec2(1.0f, 1.0f),
                        AzVec4(0.0f, 0.0f, 0.0f, 1.0f));
        if (cfg->ShowTextureUsedRect)
            GetWindowDrawList()->AddRect(
                    AzVec2(p.x + tex->UsedRect.x, p.y + tex->UsedRect.y),
                    AzVec2(p.x + tex->UsedRect.x + tex->UsedRect.w,
                           p.y + tex->UsedRect.y + tex->UsedRect.h),
                    AZ_COL32(255, 0, 255, 255));
        if (highlight_rect != NULL)
        {
            AzRect r_outer(p.x, p.y, p.x + tex->Width, p.y + tex->Height);
            AzRect r_inner(p.x + highlight_rect->x,
                           p.y + highlight_rect->y,
                           p.x + highlight_rect->x + highlight_rect->w,
                           p.y + highlight_rect->y + highlight_rect->h);
            RenderRectFilledWithHole(GetWindowDrawList(),
                                     r_outer,
                                     r_inner,
                                     AZ_COL32(0, 0, 0, 100),
                                     0.0f);
            GetWindowDrawList()->AddRect(r_inner.Min - AzVec2(1, 1),
                                         r_inner.Max + AzVec2(1, 1),
                                         AZ_COL32(255, 255, 0, 255));
        }
        PopStyleVar();

        char texid_desc[30];
        Text("Status = %s (%d), Format = %s (%d), UseColors = %d",
             AzTextureDataGetStatusName(tex->Status),
             tex->Status,
             AzTextureDataGetFormatName(tex->Format),
             tex->Format,
             tex->UseColors);
        Text("TexID = %s, BackendUserData = %p",
             FormatTextureIDForDebugDisplay(texid_desc,
                                            AZ_ARRAYSIZE(texid_desc),
                                            tex->TexID),
             tex->BackendUserData);
        TreePop();
    }
    PopID();
}

void Az::ShowMetricsWindow(bool* p_open)
{
    AzContext& g = *GAz;
    AzIO& io = g.IO;
    AzMetricsConfig* cfg = &g.DebugMetricsConfig;
    if (cfg->ShowDebugLog)
        ShowDebugLogWindow(&cfg->ShowDebugLog);
    if (cfg->ShowIDStackTool)
        ShowIDStackToolWindow(&cfg->ShowIDStackTool);

    if (!Begin("Dear Az Metrics/Debugger", p_open)
        || GetCurrentWindow()->BeginCount > 1)
    {
        End();
        return;
    }

    DebugBreakClearData();

    Text("Dear Az %s (%d)", AZ_VERSION, AZ_VERSION_NUM);
    if (g.ContextName[0] != 0)
    {
        SameLine();
        Text("(Context Name: \"%s\")", g.ContextName);
    }
    Text("Application average %.3f ms/frame (%.1f FPS)",
         1000.0f / io.Framerate,
         io.Framerate);
    Text("%d vertices, %d indices (%d triangles)",
         io.MetricsRenderVertices,
         io.MetricsRenderIndices,
         io.MetricsRenderIndices / 3);
    Text("%d visible windows, %d current allocations",
         io.MetricsRenderWindows,
         g.DebugAllocInfo.TotalAllocCount - g.DebugAllocInfo.TotalFreeCount);

    Separator();

    enum
    {
        WRT_OuterRect,
        WRT_OuterRectClipped,
        WRT_InnerRect,
        WRT_InnerClipRect,
        WRT_WorkRect,
        WRT_Content,
        WRT_ContentIdeal,
        WRT_ContentRegionRect,
        WRT_Count
    };
    const char* wrt_rects_names[WRT_Count] = {"OuterRect",
                                              "OuterRectClipped",
                                              "InnerRect",
                                              "InnerClipRect",
                                              "WorkRect",
                                              "Content",
                                              "ContentIdeal",
                                              "ContentRegionRect"};
    enum
    {
        TRT_OuterRect,
        TRT_InnerRect,
        TRT_WorkRect,
        TRT_HostClipRect,
        TRT_InnerClipRect,
        TRT_BackgroundClipRect,
        TRT_ColumnsRect,
        TRT_ColumnsWorkRect,
        TRT_ColumnsClipRect,
        TRT_ColumnsContentHeadersUsed,
        TRT_ColumnsContentHeadersIdeal,
        TRT_ColumnsContentFrozen,
        TRT_ColumnsContentUnfrozen,
        TRT_Count
    };
    const char* trt_rects_names[TRT_Count] = {"OuterRect",
                                              "InnerRect",
                                              "WorkRect",
                                              "HostClipRect",
                                              "InnerClipRect",
                                              "BackgroundClipRect",
                                              "ColumnsRect",
                                              "ColumnsWorkRect",
                                              "ColumnsClipRect",
                                              "ColumnsContentHeadersUsed",
                                              "ColumnsContentHeadersIdeal",
                                              "ColumnsContentFrozen",
                                              "ColumnsContentUnfrozen"};
    if (cfg->ShowWindowsRectsType < 0)
        cfg->ShowWindowsRectsType = WRT_WorkRect;
    if (cfg->ShowTablesRectsType < 0)
        cfg->ShowTablesRectsType = TRT_WorkRect;

    struct Funcs
    {
        static AzRect GetTableRect(AzTable* table, int rect_type, int n)
        {
            AzTableInstanceData* table_instance =
                    TableGetInstanceData(table, table->InstanceCurrent);

            if (rect_type == TRT_OuterRect)
            {
                return table->OuterRect;
            } else if (rect_type == TRT_InnerRect)
            {
                return table->InnerRect;
            } else if (rect_type == TRT_WorkRect)
            {
                return table->WorkRect;
            } else if (rect_type == TRT_HostClipRect)
            {
                return table->HostClipRect;
            } else if (rect_type == TRT_InnerClipRect)
            {
                return table->InnerClipRect;
            } else if (rect_type == TRT_BackgroundClipRect)
            {
                return table->BgClipRect;
            } else if (rect_type == TRT_ColumnsRect)
            {
                AzTableColumn* c = &table->Columns[n];
                return AzRect(c->MinX,
                              table->InnerClipRect.Min.y,
                              c->MaxX,
                              table->InnerClipRect.Min.y
                                      + table_instance->LastOuterHeight);
            } else if (rect_type == TRT_ColumnsWorkRect)
            {
                AzTableColumn* c = &table->Columns[n];
                return AzRect(c->WorkMinX,
                              table->WorkRect.Min.y,
                              c->WorkMaxX,
                              table->WorkRect.Max.y);
            } else if (rect_type == TRT_ColumnsClipRect)
            {
                AzTableColumn* c = &table->Columns[n];
                return c->ClipRect;
            } else if (rect_type == TRT_ColumnsContentHeadersUsed)
            {
                AzTableColumn* c = &table->Columns[n];
                return AzRect(
                        c->WorkMinX,
                        table->InnerClipRect.Min.y,
                        c->ContentMaxXHeadersUsed,
                        table->InnerClipRect.Min.y
                                + table_instance->LastTopHeadersRowHeight);
            } else if (rect_type == TRT_ColumnsContentHeadersIdeal)
            {
                AzTableColumn* c = &table->Columns[n];
                return AzRect(
                        c->WorkMinX,
                        table->InnerClipRect.Min.y,
                        c->ContentMaxXHeadersIdeal,
                        table->InnerClipRect.Min.y
                                + table_instance->LastTopHeadersRowHeight);
            } else if (rect_type == TRT_ColumnsContentFrozen)
            {
                AzTableColumn* c = &table->Columns[n];
                return AzRect(c->WorkMinX,
                              table->InnerClipRect.Min.y,
                              c->ContentMaxXFrozen,
                              table->InnerClipRect.Min.y
                                      + table_instance->LastFrozenHeight);
            } else if (rect_type == TRT_ColumnsContentUnfrozen)
            {
                AzTableColumn* c = &table->Columns[n];
                return AzRect(c->WorkMinX,
                              table->InnerClipRect.Min.y
                                      + table_instance->LastFrozenHeight,
                              c->ContentMaxXUnfrozen,
                              table->InnerClipRect.Max.y);
            }
            AZ_ASSERT(0);
            return AzRect();
        }

        static AzRect GetWindowRect(AzWindow* window, int rect_type)
        {
            if (rect_type == WRT_OuterRect)
            {
                return window->Rect();
            } else if (rect_type == WRT_OuterRectClipped)
            {
                return window->OuterRectClipped;
            } else if (rect_type == WRT_InnerRect)
            {
                return window->InnerRect;
            } else if (rect_type == WRT_InnerClipRect)
            {
                return window->InnerClipRect;
            } else if (rect_type == WRT_WorkRect)
            {
                return window->WorkRect;
            } else if (rect_type == WRT_Content)
            {
                AzVec2 min = window->InnerRect.Min - window->Scroll
                             + window->WindowPadding;
                return AzRect(min, min + window->ContentSize);
            } else if (rect_type == WRT_ContentIdeal)
            {
                AzVec2 min = window->InnerRect.Min - window->Scroll
                             + window->WindowPadding;
                return AzRect(min, min + window->ContentSizeIdeal);
            } else if (rect_type == WRT_ContentRegionRect)
            {
                return window->ContentRegionRect;
            }
            AZ_ASSERT(0);
            return AzRect();
        }
    };

    if (TreeNode("Tools"))
    {
        SeparatorTextEx(0,
                        "Debug breaks",
                        NULL,
                        CalcTextSize("(?)").x + g.Style.SeparatorTextPadding.x);
        SameLine();
        MetricsHelpMarker(
                "Will call the AZ_DEBUG_BREAK() macro to break in "
                "debugger.\nWarning: If you don't have a debugger attached, "
                "this will probably crash.");
        if (Checkbox("Show Item Picker", &g.DebugItemPickerActive)
            && g.DebugItemPickerActive)
            DebugStartItemPicker();
        Checkbox(
                "Show \"Debug Break\" buttons in other sections "
                "(io.ConfigDebugIsDebuggerPresent)",
                &g.IO.ConfigDebugIsDebuggerPresent);

        SeparatorText("Visualize");

        Checkbox("Show Debug Log", &cfg->ShowDebugLog);
        SameLine();
        MetricsHelpMarker(
                "You can also call Az::ShowDebugLogWindow() from your "
                "code.");

        Checkbox("Show ID Stack Tool", &cfg->ShowIDStackTool);
        SameLine();
        MetricsHelpMarker(
                "You can also call Az::ShowIDStackToolWindow() from your "
                "code.");

        Checkbox("Show windows begin order", &cfg->ShowWindowsBeginOrder);
        Checkbox("Show windows rectangles", &cfg->ShowWindowsRects);
        SameLine();
        SetNextItemWidth(GetFontSize() * 12);
        cfg->ShowWindowsRects |= Combo("##show_windows_rect_type",
                                       &cfg->ShowWindowsRectsType,
                                       wrt_rects_names,
                                       WRT_Count,
                                       WRT_Count);
        if (cfg->ShowWindowsRects && g.NavWindow != NULL)
        {
            BulletText("'%s':", g.NavWindow->Name);
            Indent();
            for (int rect_n = 0; rect_n < WRT_Count; rect_n++)
            {
                AzRect r = Funcs::GetWindowRect(g.NavWindow, rect_n);
                Text("(%6.1f,%6.1f) (%6.1f,%6.1f) Size (%6.1f,%6.1f) %s",
                     r.Min.x,
                     r.Min.y,
                     r.Max.x,
                     r.Max.y,
                     r.GetWidth(),
                     r.GetHeight(),
                     wrt_rects_names[rect_n]);
            }
            Unindent();
        }

        Checkbox("Show tables rectangles", &cfg->ShowTablesRects);
        SameLine();
        SetNextItemWidth(GetFontSize() * 12);
        cfg->ShowTablesRects |= Combo("##show_table_rects_type",
                                      &cfg->ShowTablesRectsType,
                                      trt_rects_names,
                                      TRT_Count,
                                      TRT_Count);
        if (cfg->ShowTablesRects && g.NavWindow != NULL)
        {
            for (int table_n = 0; table_n < g.Tables.GetMapSize(); table_n++)
            {
                AzTable* table = g.Tables.TryGetMapData(table_n);
                if (table == NULL || table->LastFrameActive < g.FrameCount - 1
                    || (table->OuterWindow != g.NavWindow
                        && table->InnerWindow != g.NavWindow))
                    continue;

                BulletText("Table 0x%08X (%d columns, in '%s')",
                           table->ID,
                           table->ColumnsCount,
                           table->OuterWindow->Name);
                if (IsItemHovered())
                    GetForegroundDrawList()->AddRect(table->OuterRect.Min
                                                             - AzVec2(1, 1),
                                                     table->OuterRect.Max
                                                             + AzVec2(1, 1),
                                                     AZ_COL32(255, 255, 0, 255),
                                                     0.0f,
                                                     0,
                                                     2.0f);
                Indent();
                char buf[128];
                for (int rect_n = 0; rect_n < TRT_Count; rect_n++)
                {
                    if (rect_n >= TRT_ColumnsRect)
                    {
                        if (rect_n != TRT_ColumnsRect
                            && rect_n != TRT_ColumnsClipRect)
                            continue;
                        for (int column_n = 0; column_n < table->ColumnsCount;
                             column_n++)
                        {
                            AzRect r = Funcs::GetTableRect(table,
                                                           rect_n,
                                                           column_n);
                            AzFormatString(buf,
                                           AZ_ARRAYSIZE(buf),
                                           "(%6.1f,%6.1f) (%6.1f,%6.1f) Size "
                                           "(%6.1f,%6.1f) Col %d %s",
                                           r.Min.x,
                                           r.Min.y,
                                           r.Max.x,
                                           r.Max.y,
                                           r.GetWidth(),
                                           r.GetHeight(),
                                           column_n,
                                           trt_rects_names[rect_n]);
                            Selectable(buf);
                            if (IsItemHovered())
                                GetForegroundDrawList()->AddRect(
                                        r.Min - AzVec2(1, 1),
                                        r.Max + AzVec2(1, 1),
                                        AZ_COL32(255, 255, 0, 255),
                                        0.0f,
                                        0,
                                        2.0f);
                        }
                    } else
                    {
                        AzRect r = Funcs::GetTableRect(table, rect_n, -1);
                        AzFormatString(buf,
                                       AZ_ARRAYSIZE(buf),
                                       "(%6.1f,%6.1f) (%6.1f,%6.1f) Size "
                                       "(%6.1f,%6.1f) %s",
                                       r.Min.x,
                                       r.Min.y,
                                       r.Max.x,
                                       r.Max.y,
                                       r.GetWidth(),
                                       r.GetHeight(),
                                       trt_rects_names[rect_n]);
                        Selectable(buf);
                        if (IsItemHovered())
                            GetForegroundDrawList()->AddRect(
                                    r.Min - AzVec2(1, 1),
                                    r.Max + AzVec2(1, 1),
                                    AZ_COL32(255, 255, 0, 255),
                                    0.0f,
                                    0,
                                    2.0f);
                    }
                }
                Unindent();
            }
        }
        Checkbox("Show groups rectangles", &g.DebugShowGroupRects);

        SeparatorText("Validate");

        Checkbox("Debug Begin/BeginChild return value",
                 &io.ConfigDebugBeginReturnValueLoop);
        SameLine();
        MetricsHelpMarker(
                "Some calls to Begin()/BeginChild() will return false.\n\nWill "
                "cycle through window depths then repeat. Windows should be "
                "flickering while running.");

        Checkbox("UTF-8 Encoding viewer", &cfg->ShowTextEncodingViewer);
        SameLine();
        MetricsHelpMarker(
                "You can also call Az::DebugTextEncoding() from your code "
                "with a given string to test that your UTF-8 encoding settings "
                "are correct.");
        if (cfg->ShowTextEncodingViewer)
        {
            static char buf[64] = "";
            SetNextItemWidth(-FLT_MIN);
            InputText("##DebugTextEncodingBuf", buf, AZ_ARRAYSIZE(buf));
            if (buf[0] != 0)
                DebugTextEncoding(buf);
        }

        TreePop();
    }

    if (TreeNode("Windows", "Windows (%d)", g.Windows.Size))
    {
        DebugNodeWindowsList(&g.Windows, "By display order");
        DebugNodeWindowsList(&g.WindowsFocusOrder,
                             "By focus order (root windows)");
        if (TreeNode("By submission order (begin stack)"))
        {
            AzVector<AzWindow*>& temp_buffer = g.WindowsTempSortBuffer;
            temp_buffer.resize(0);
            for (AzWindow* window : g.Windows)
                if (window->LastFrameActive + 1 >= g.FrameCount)
                    temp_buffer.push_back(window);
            struct Func
            {
                static int AZ_CDECL WindowComparerByBeginOrder(const void* lhs,
                                                               const void* rhs)
                {
                    return ((int)(*(const AzWindow* const*)lhs)
                                    ->BeginOrderWithinContext
                            - (*(const AzWindow* const*)rhs)
                                      ->BeginOrderWithinContext);
                }
            };
            AzQsort(temp_buffer.Data,
                    (size_t)temp_buffer.Size,
                    sizeof(AzWindow*),
                    Func::WindowComparerByBeginOrder);
            DebugNodeWindowsListByBeginStackParent(temp_buffer.Data,
                                                   temp_buffer.Size,
                                                   NULL);
            TreePop();
        }

        TreePop();
    }

    int drawlist_count = 0;
    for (AzViewportP* viewport : g.Viewports)
        drawlist_count += viewport->DrawDataP.CmdLists.Size;
    if (TreeNode("DrawLists", "DrawLists (%d)", drawlist_count))
    {
        Checkbox("Show AzDrawCmd mesh when hovering", &cfg->ShowDrawCmdMesh);
        Checkbox("Show AzDrawCmd bounding boxes when hovering",
                 &cfg->ShowDrawCmdBoundingBoxes);
        for (AzViewportP* viewport : g.Viewports)
        {
            bool viewport_has_drawlist = false;
            for (AzDrawList* draw_list : viewport->DrawDataP.CmdLists)
            {
                if (!viewport_has_drawlist)
                    Text("Active DrawLists in Viewport #%d, ID: 0x%08X",
                         viewport->Idx,
                         viewport->ID);
                viewport_has_drawlist = true;
                DebugNodeDrawList(NULL, viewport, draw_list, "DrawList");
            }
        }
        TreePop();
    }

    if (TreeNode("Viewports", "Viewports (%d)", g.Viewports.Size))
    {
        cfg->HighlightMonitorIdx = -1;
        bool open = TreeNode("Monitors",
                             "Monitors (%d)",
                             g.PlatformIO.Monitors.Size);
        SameLine();
        MetricsHelpMarker(
                "Dear Az uses monitor data:\n- to query DPI settings on a "
                "per monitor basis\n- to position popup/tooltips so they don't "
                "straddle monitors.");
        if (open)
        {
            for (int i = 0; i < g.PlatformIO.Monitors.Size; i++)
            {
                DebugNodePlatformMonitor(&g.PlatformIO.Monitors[i],
                                         "Monitor",
                                         i);
                if (IsItemHovered())
                    cfg->HighlightMonitorIdx = i;
            }
            DebugNodePlatformMonitor(&g.FallbackMonitor, "Fallback", 0);
            TreePop();
        }

        SetNextItemOpen(true, AzCond_Once);
        if (TreeNode("Windows Minimap"))
        {
            RenderViewportsThumbnails();
            TreePop();
        }
        cfg->HighlightViewportID = 0;

        BulletText(
                "MouseViewport: 0x%08X (UserHovered 0x%08X, LastHovered "
                "0x%08X)",
                g.MouseViewport ? g.MouseViewport->ID : 0,
                g.IO.MouseHoveredViewport,
                g.MouseLastHoveredViewport ? g.MouseLastHoveredViewport->ID
                                           : 0);
        if (TreeNode("Inferred Z order (front-to-back)"))
        {
            static AzVector<AzViewportP*> viewports;
            viewports.resize(g.Viewports.Size);
            memcpy(viewports.Data,
                   g.Viewports.Data,
                   g.Viewports.size_in_bytes());
            if (viewports.Size > 1)
                AzQsort(viewports.Data,
                        viewports.Size,
                        sizeof(AzViewport*),
                        ViewportComparerByLastFocusedStampCount);
            for (AzViewportP* viewport : viewports)
            {
                BulletText(
                        "Viewport #%d, ID: 0x%08X, LastFocused = %08d, "
                        "PlatformFocused = %s, Window: \"%s\"",
                        viewport->Idx,
                        viewport->ID,
                        viewport->LastFocusedStampCount,
                        (g.PlatformIO.Platform_GetWindowFocus
                         && viewport->PlatformWindowCreated)
                                ? (g.PlatformIO.Platform_GetWindowFocus(
                                           viewport)
                                           ? "1"
                                           : "0")
                                : "N/A",
                        viewport->Window ? viewport->Window->Name : "N/A");
                if (IsItemHovered())
                    cfg->HighlightViewportID = viewport->ID;
            }
            TreePop();
        }

        for (AzViewportP* viewport : g.Viewports)
            DebugNodeViewport(viewport);
        TreePop();
    }

    for (AzFontAtlas* atlas : g.FontAtlases)
        if (TreeNode((void*)atlas,
                     "Fonts (%d), Textures (%d)",
                     atlas->Fonts.Size,
                     atlas->TexList.Size))
        {
            ShowFontAtlas(atlas);
            TreePop();
        }

    if (TreeNode("Popups", "Popups (%d)", g.OpenPopupStack.Size))
    {
        for (const AzPopupData& popup_data : g.OpenPopupStack)
        {
            AzWindow* window = popup_data.Window;
            BulletText(
                    "PopupID: %08x, Window: '%s' (%s%s), RestoreNavWindow "
                    "'%s', ParentWindow '%s'",
                    popup_data.PopupId,
                    window ? window->Name : "NULL",
                    window && (window->Flags & AzWindowFlags_ChildWindow)
                            ? "Child;"
                            : "",
                    window && (window->Flags & AzWindowFlags_ChildMenu)
                            ? "Menu;"
                            : "",
                    popup_data.RestoreNavWindow
                            ? popup_data.RestoreNavWindow->Name
                            : "NULL",
                    window && window->ParentWindow ? window->ParentWindow->Name
                                                   : "NULL");
        }
        TreePop();
    }

    if (TreeNode("TabBars", "Tab Bars (%d)", g.TabBars.GetAliveCount()))
    {
        for (int n = 0; n < g.TabBars.GetMapSize(); n++)
            if (AzTabBar* tab_bar = g.TabBars.TryGetMapData(n))
            {
                PushID(tab_bar);
                DebugNodeTabBar(tab_bar, "TabBar");
                PopID();
            }
        TreePop();
    }

    if (TreeNode("Tables", "Tables (%d)", g.Tables.GetAliveCount()))
    {
        for (int n = 0; n < g.Tables.GetMapSize(); n++)
            if (AzTable* table = g.Tables.TryGetMapData(n))
                DebugNodeTable(table);
        TreePop();
    }

    if (TreeNode("InputText"))
    {
        DebugNodeInputTextState(&g.InputTextState);
        TreePop();
    }

    if (TreeNode("TypingSelect",
                 "TypingSelect (%d)",
                 g.TypingSelectState.SearchBuffer[0] != 0 ? 1 : 0))
    {
        DebugNodeTypingSelectState(&g.TypingSelectState);
        TreePop();
    }

    if (TreeNode("MultiSelect",
                 "MultiSelect (%d)",
                 g.MultiSelectStorage.GetAliveCount()))
    {
        AzBoxSelectState* bs = &g.BoxSelectState;
        BulletText("BoxSelect ID=0x%08X, Starting = %d, Active %d",
                   bs->ID,
                   bs->IsStarting,
                   bs->IsActive);
        for (int n = 0; n < g.MultiSelectStorage.GetMapSize(); n++)
            if (AzMultiSelectState* state = g.MultiSelectStorage.TryGetMapData(
                        n))
                DebugNodeMultiSelectState(state);
        TreePop();
    }

#ifdef AZ_HAS_DOCK
    if (TreeNode("Docking"))
    {
        static bool root_nodes_only = true;
        AzDockContext* dc = &g.DockContext;
        Checkbox("List root nodes", &root_nodes_only);
        Checkbox("Ctrl shows window dock info", &cfg->ShowDockingNodes);
        if (SmallButton("Clear nodes"))
        {
            DockContextClearNodes(&g, 0, true);
        }
        SameLine();
        if (SmallButton("Rebuild all"))
        {
            dc->WantFullRebuild = true;
        }
        for (int n = 0; n < dc->Nodes.Data.Size; n++)
            if (AzDockNode* node = (AzDockNode*)dc->Nodes.Data[n].val_p)
                if (!root_nodes_only || node->IsRootNode())
                    DebugNodeDockNode(node, "Node");
        TreePop();
    }
#endif

    if (TreeNode("Settings"))
    {
        if (SmallButton("Clear"))
            ClearIniSettings();
        SameLine();
        if (SmallButton("Save to memory"))
            SaveIniSettingsToMemory();
        SameLine();
        if (SmallButton("Save to disk"))
            SaveIniSettingsToDisk(g.IO.IniFilename);
        SameLine();
        if (g.IO.IniFilename)
            Text("\"%s\"", g.IO.IniFilename);
        else
            TextUnformatted("<NULL>");
        Checkbox("io.ConfigDebugIniSettings", &io.ConfigDebugIniSettings);
        Text("SettingsDirtyTimer %.2f", g.SettingsDirtyTimer);
        if (TreeNode("SettingsHandlers",
                     "Settings handlers: (%d)",
                     g.SettingsHandlers.Size))
        {
            for (AzSettingsHandler& handler : g.SettingsHandlers)
                BulletText("\"%s\"", handler.TypeName);
            TreePop();
        }
        if (TreeNode("SettingsWindows",
                     "Settings packed data: Windows: %d bytes",
                     g.SettingsWindows.size()))
        {
            for (AzWindowSettings* settings = g.SettingsWindows.begin();
                 settings != NULL;
                 settings = g.SettingsWindows.next_chunk(settings))
                DebugNodeWindowSettings(settings);
            TreePop();
        }

        if (TreeNode("SettingsTables",
                     "Settings packed data: Tables: %d bytes",
                     g.SettingsTables.size()))
        {
            for (AzTableSettings* settings = g.SettingsTables.begin();
                 settings != NULL;
                 settings = g.SettingsTables.next_chunk(settings))
                DebugNodeTableSettings(settings);
            TreePop();
        }

#ifdef AZ_HAS_DOCK
        if (TreeNode("SettingsDocking", "Settings packed data: Docking"))
        {
            AzDockContext* dc = &g.DockContext;
            Text("In SettingsWindows:");
            for (AzWindowSettings* settings = g.SettingsWindows.begin();
                 settings != NULL;
                 settings = g.SettingsWindows.next_chunk(settings))
                if (settings->DockId != 0)
                    BulletText("Window '%s' -> DockId %08X DockOrder=%d",
                               settings->GetName(),
                               settings->DockId,
                               settings->DockOrder);
            Text("In SettingsNodes:");
            for (int n = 0; n < dc->NodesSettings.Size; n++)
            {
                AzDockNodeSettings* settings = &dc->NodesSettings[n];
                const char* selected_tab_name = NULL;
                if (settings->SelectedTabId)
                {
                    if (AzWindow* window = FindWindowByID(
                                settings->SelectedTabId))
                        selected_tab_name = window->Name;
                    else if (AzWindowSettings* window_settings =
                                     FindWindowSettingsByID(
                                             settings->SelectedTabId))
                        selected_tab_name = window_settings->GetName();
                }
                BulletText("Node %08X, Parent %08X, SelectedTab %08X ('%s')",
                           settings->ID,
                           settings->ParentNodeId,
                           settings->SelectedTabId,
                           selected_tab_name         ? selected_tab_name
                           : settings->SelectedTabId ? "N/A"
                                                     : "");
            }
            TreePop();
        }
#endif

        if (TreeNode("SettingsIniData",
                     "Settings unpacked data (.ini): %d bytes",
                     g.SettingsIniData.size()))
        {
            InputTextMultiline("##Ini",
                               (char*)(void*)g.SettingsIniData.c_str(),
                               g.SettingsIniData.Buf.Size,
                               AzVec2(-FLT_MIN, GetTextLineHeight() * 20),
                               AzInputTextFlags_ReadOnly);
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Memory allocations"))
    {
        AzDebugAllocInfo* info = &g.DebugAllocInfo;
        Text("%d current allocations",
             info->TotalAllocCount - info->TotalFreeCount);
        if (SmallButton("GC now"))
        {
            g.GcCompactAll = true;
        }
        Text("Recent frames with allocations:");
        int buf_size = AZ_ARRAYSIZE(info->LastEntriesBuf);
        for (int n = buf_size - 1; n >= 0; n--)
        {
            AzDebugAllocEntry* entry =
                    &info->LastEntriesBuf[(info->LastEntriesIdx - n + buf_size)
                                          % buf_size];
            BulletText("Frame %06d: %+3d ( %2d alloc, %2d free )",
                       entry->FrameCount,
                       entry->AllocCount - entry->FreeCount,
                       entry->AllocCount,
                       entry->FreeCount);
            if (n == 0)
            {
                SameLine();
                Text("<- %d frames ago", g.FrameCount - entry->FrameCount);
            }
        }
        TreePop();
    }

    if (TreeNode("Inputs"))
    {
        Text("KEYBOARD/GAMEPAD/MOUSE KEYS");
        {
            Indent();
            Text("Keys down:");
            for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
                 key = (AzKey)(key + 1))
            {
                if (!IsKeyDown(key))
                    continue;
                SameLine();
                Text(IsNamedKey(key) ? "\"%s\"" : "\"%s\" %d",
                     GetKeyName(key),
                     key);
                SameLine();
                Text("(%.02f)", GetKeyData(key)->DownDuration);
            }
            Text("Keys pressed:");
            for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
                 key = (AzKey)(key + 1))
            {
                if (!IsKeyPressed(key))
                    continue;
                SameLine();
                Text(IsNamedKey(key) ? "\"%s\"" : "\"%s\" %d",
                     GetKeyName(key),
                     key);
            }
            Text("Keys released:");
            for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
                 key = (AzKey)(key + 1))
            {
                if (!IsKeyReleased(key))
                    continue;
                SameLine();
                Text(IsNamedKey(key) ? "\"%s\"" : "\"%s\" %d",
                     GetKeyName(key),
                     key);
            }
            Text("Keys mods: %s%s%s%s",
                 io.KeyCtrl ? "CTRL " : "",
                 io.KeyShift ? "SHIFT " : "",
                 io.KeyAlt ? "ALT " : "",
                 io.KeySuper ? "SUPER " : "");
            Text("Chars queue:");
            for (int i = 0; i < io.InputQueueCharacters.Size; i++)
            {
                AzWchar c = io.InputQueueCharacters[i];
                SameLine();
                Text("\'%c\' (0x%04X)",
                     (c > ' ' && c <= 255) ? (char)c : '?',
                     c);
            }

            DebugRenderKeyboardPreview(GetWindowDrawList());
            Unindent();
        }

        Text("MOUSE STATE");
        {
            Indent();
            if (IsMousePosValid())
                Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                Text("Mouse pos: <INVALID>");
            Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            int count = AZ_ARRAYSIZE(io.MouseDown);
            Text("Mouse down:");
            for (int i = 0; i < count; i++)
                if (IsMouseDown(i))
                {
                    SameLine();
                    Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
                }
            Text("Mouse clicked:");
            for (int i = 0; i < count; i++)
                if (IsMouseClicked(i))
                {
                    SameLine();
                    Text("b%d (%d)", i, io.MouseClickedCount[i]);
                }
            Text("Mouse released:");
            for (int i = 0; i < count; i++)
                if (IsMouseReleased(i))
                {
                    SameLine();
                    Text("b%d", i);
                }
            Text("Mouse wheel: %.1f", io.MouseWheel);
            Text("MouseStationaryTimer: %.2f", g.MouseStationaryTimer);
            Text("Mouse source: %s", GetMouseSourceName(io.MouseSource));
            Text("Pen Pressure: %.1f", io.PenPressure);
            Unindent();
        }

        Text("MOUSE WHEELING");
        {
            Indent();
            Text("WheelingWindow: '%s'",
                 g.WheelingWindow ? g.WheelingWindow->Name : "NULL");
            Text("WheelingWindowReleaseTimer: %.2f",
                 g.WheelingWindowReleaseTimer);
            Text("WheelingAxisAvg[] = { %.3f, %.3f }, Main Axis: %s",
                 g.WheelingAxisAvg.x,
                 g.WheelingAxisAvg.y,
                 (g.WheelingAxisAvg.x > g.WheelingAxisAvg.y)   ? "X"
                 : (g.WheelingAxisAvg.x < g.WheelingAxisAvg.y) ? "Y"
                                                               : "<none>");
            Unindent();
        }

        Text("KEY OWNERS");
        {
            Indent();
            if (BeginChild("##owners",
                           AzVec2(-FLT_MIN, GetTextLineHeightWithSpacing() * 8),
                           AzChildFlags_FrameStyle | AzChildFlags_ResizeY,
                           AzWindowFlags_NoSavedSettings))
                for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
                     key = (AzKey)(key + 1))
                {
                    AzKeyOwnerData* owner_data = GetKeyOwnerData(&g, key);
                    if (owner_data->OwnerCurr == AzKeyOwner_NoOwner)
                        continue;
                    Text("%s: 0x%08X%s",
                         GetKeyName(key),
                         owner_data->OwnerCurr,
                         owner_data->LockUntilRelease ? " LockUntilRelease"
                         : owner_data->LockThisFrame  ? " LockThisFrame"
                                                      : "");
                    DebugLocateItemOnHover(owner_data->OwnerCurr);
                }
            EndChild();
            Unindent();
        }
        Text("SHORTCUT ROUTING");
        SameLine();
        MetricsHelpMarker(
                "Declared shortcut routes automatically set key owner when "
                "mods matches.");
        {
            Indent();
            if (BeginChild("##routes",
                           AzVec2(-FLT_MIN, GetTextLineHeightWithSpacing() * 8),
                           AzChildFlags_FrameStyle | AzChildFlags_ResizeY,
                           AzWindowFlags_NoSavedSettings))
                for (AzKey key = AzKey_NamedKey_BEGIN; key < AzKey_NamedKey_END;
                     key = (AzKey)(key + 1))
                {
                    AzKeyRoutingTable* rt = &g.KeysRoutingTable;
                    for (AzKeyRoutingIndex idx =
                                 rt->Index[key - AzKey_NamedKey_BEGIN];
                         idx != -1;)
                    {
                        AzKeyRoutingData* routing_data = &rt->Entries[idx];
                        AzKeyChord key_chord = key | routing_data->Mods;
                        Text("%s: 0x%08X (scored %d)",
                             GetKeyChordName(key_chord),
                             routing_data->RoutingCurr,
                             routing_data->RoutingCurrScore);
                        DebugLocateItemOnHover(routing_data->RoutingCurr);
                        if (g.IO.ConfigDebugIsDebuggerPresent)
                        {
                            SameLine();
                            if (DebugBreakButton("**DebugBreak**",
                                                 "in SetShortcutRouting() for "
                                                 "this KeyChord"))
                                g.DebugBreakInShortcutRouting = key_chord;
                        }
                        idx = routing_data->NextEntryIndex;
                    }
                }
            EndChild();
            Text("(ActiveIdUsing: AllKeyboardKeys: %d, NavDirMask: 0x%X)",
                 g.ActiveIdUsingAllKeyboardKeys,
                 g.ActiveIdUsingNavDirMask);
            Unindent();
        }
        TreePop();
    }

    if (TreeNode("Internal state"))
    {
        Text("WINDOWING");
        Indent();
        Text("HoveredWindow: '%s'",
             g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
        Text("HoveredWindow->Root: '%s'",
             g.HoveredWindow ? g.HoveredWindow->RootWindowDockTree->Name
                             : "NULL");
        Text("HoveredWindowUnderMovingWindow: '%s'",
             g.HoveredWindowUnderMovingWindow
                     ? g.HoveredWindowUnderMovingWindow->Name
                     : "NULL");
        Text("HoveredDockNode: 0x%08X",
             g.DebugHoveredDockNode ? g.DebugHoveredDockNode->ID : 0);
        Text("MovingWindow: '%s'",
             g.MovingWindow ? g.MovingWindow->Name : "NULL");
        Text("MouseViewport: 0x%08X (UserHovered 0x%08X, LastHovered 0x%08X)",
             g.MouseViewport->ID,
             g.IO.MouseHoveredViewport,
             g.MouseLastHoveredViewport ? g.MouseLastHoveredViewport->ID : 0);
        Unindent();

        Text("ITEMS");
        Indent();
        Text("ActiveId: 0x%08X/0x%08X (%.2f sec), AllowOverlap: %d, Source: %s",
             g.ActiveId,
             g.ActiveIdPreviousFrame,
             g.ActiveIdTimer,
             g.ActiveIdAllowOverlap,
             GetInputSourceName(g.ActiveIdSource));
        DebugLocateItemOnHover(g.ActiveId);
        Text("ActiveIdWindow: '%s'",
             g.ActiveIdWindow ? g.ActiveIdWindow->Name : "NULL");
        Text("ActiveIdUsing: AllKeyboardKeys: %d, NavDirMask: %X",
             g.ActiveIdUsingAllKeyboardKeys,
             g.ActiveIdUsingNavDirMask);
        Text("HoveredId: 0x%08X (%.2f sec), AllowOverlap: %d",
             g.HoveredIdPreviousFrame,
             g.HoveredIdTimer,
             g.HoveredIdAllowOverlap);

        Text("HoverItemDelayId: 0x%08X, Timer: %.2f, ClearTimer: %.2f",
             g.HoverItemDelayId,
             g.HoverItemDelayTimer,
             g.HoverItemDelayClearTimer);
        Text("DragDrop: %d, SourceId = 0x%08X, Payload \"%s\" (%d bytes)",
             g.DragDropActive,
             g.DragDropPayload.SourceId,
             g.DragDropPayload.DataType,
             g.DragDropPayload.DataSize);
        DebugLocateItemOnHover(g.DragDropPayload.SourceId);
        Unindent();

        Text("NAV,FOCUS");
        Indent();
        Text("NavWindow: '%s'", g.NavWindow ? g.NavWindow->Name : "NULL");
        Text("NavId: 0x%08X, NavLayer: %d", g.NavId, g.NavLayer);
        DebugLocateItemOnHover(g.NavId);
        Text("NavInputSource: %s", GetInputSourceName(g.NavInputSource));
        Text("NavLastValidSelectionUserData = %" AZ_PRId64 " (0x%" AZ_PRIX64
             ")",
             g.NavLastValidSelectionUserData,
             g.NavLastValidSelectionUserData);
        Text("NavActive: %d, NavVisible: %d", g.IO.NavActive, g.IO.NavVisible);
        Text("NavActivateId/DownId/PressedId: %08X/%08X/%08X",
             g.NavActivateId,
             g.NavActivateDownId,
             g.NavActivatePressedId);
        Text("NavActivateFlags: %04X", g.NavActivateFlags);
        Text("NavCursorVisible: %d, NavHighlightItemUnderNav: %d",
             g.NavCursorVisible,
             g.NavHighlightItemUnderNav);
        Text("NavFocusScopeId = 0x%08X", g.NavFocusScopeId);
        Text("NavFocusRoute[] = ");
        for (int path_n = g.NavFocusRoute.Size - 1; path_n >= 0; path_n--)
        {
            const AzFocusScopeData& focus_scope = g.NavFocusRoute[path_n];
            SameLine(0.0f, 0.0f);
            Text("0x%08X/", focus_scope.ID);
            SetItemTooltip("In window \"%s\"",
                           FindWindowByID(focus_scope.WindowID)->Name);
        }
        Text("NavWindowingTarget: '%s'",
             g.NavWindowingTarget ? g.NavWindowingTarget->Name : "NULL");
        Unindent();

        TreePop();
    }

    if (cfg->ShowWindowsRects || cfg->ShowWindowsBeginOrder)
    {
        for (AzWindow* window : g.Windows)
        {
            if (!window->WasActive)
                continue;
            AzDrawList* draw_list = GetForegroundDrawList(window);
            if (cfg->ShowWindowsRects)
            {
                AzRect r = Funcs::GetWindowRect(window,
                                                cfg->ShowWindowsRectsType);
                draw_list->AddRect(r.Min, r.Max, AZ_COL32(255, 0, 128, 255));
            }
            if (cfg->ShowWindowsBeginOrder
                && !(window->Flags & AzWindowFlags_ChildWindow))
            {
                char buf[32];
                AzFormatString(buf,
                               AZ_ARRAYSIZE(buf),
                               "%d",
                               window->BeginOrderWithinContext);
                float font_size = GetFontSize();
                draw_list->AddRectFilled(window->Pos,
                                         window->Pos
                                                 + AzVec2(font_size, font_size),
                                         AZ_COL32(200, 100, 100, 255));
                draw_list->AddText(window->Pos,
                                   AZ_COL32(255, 255, 255, 255),
                                   buf);
            }
        }
    }

    if (cfg->ShowTablesRects)
    {
        for (int table_n = 0; table_n < g.Tables.GetMapSize(); table_n++)
        {
            AzTable* table = g.Tables.TryGetMapData(table_n);
            if (table == NULL || table->LastFrameActive < g.FrameCount - 1)
                continue;
            AzDrawList* draw_list = GetForegroundDrawList(table->OuterWindow);
            if (cfg->ShowTablesRectsType >= TRT_ColumnsRect)
            {
                for (int column_n = 0; column_n < table->ColumnsCount;
                     column_n++)
                {
                    AzRect r = Funcs::GetTableRect(table,
                                                   cfg->ShowTablesRectsType,
                                                   column_n);
                    AzU32 col = (table->HoveredColumnBody == column_n)
                                        ? AZ_COL32(255, 255, 128, 255)
                                        : AZ_COL32(255, 0, 128, 255);
                    float thickness = (table->HoveredColumnBody == column_n)
                                              ? 3.0f
                                              : 1.0f;
                    draw_list->AddRect(r.Min, r.Max, col, 0.0f, 0, thickness);
                }
            } else
            {
                AzRect r = Funcs::GetTableRect(table,
                                               cfg->ShowTablesRectsType,
                                               -1);
                draw_list->AddRect(r.Min, r.Max, AZ_COL32(255, 0, 128, 255));
            }
        }
    }

#ifdef AZ_HAS_DOCK

    if (cfg->ShowDockingNodes && g.IO.KeyCtrl && g.DebugHoveredDockNode)
    {
        char buf[64] = "";
        char* p = buf;
        AzDockNode* node = g.DebugHoveredDockNode;
        AzDrawList* overlay_draw_list =
                node->HostWindow ? GetForegroundDrawList(node->HostWindow)
                                 : GetForegroundDrawList(GetMainViewport());
        p += AzFormatString(p,
                            buf + AZ_ARRAYSIZE(buf) - p,
                            "DockId: %X%s\n",
                            node->ID,
                            node->IsCentralNode() ? " *CentralNode*" : "");
        p += AzFormatString(p,
                            buf + AZ_ARRAYSIZE(buf) - p,
                            "WindowClass: %08X\n",
                            node->WindowClass.ClassId);
        p += AzFormatString(p,
                            buf + AZ_ARRAYSIZE(buf) - p,
                            "Size: (%.0f, %.0f)\n",
                            node->Size.x,
                            node->Size.y);
        p += AzFormatString(p,
                            buf + AZ_ARRAYSIZE(buf) - p,
                            "SizeRef: (%.0f, %.0f)\n",
                            node->SizeRef.x,
                            node->SizeRef.y);
        int depth = DockNodeGetDepth(node);
        overlay_draw_list->AddRect(node->Pos + AzVec2(3, 3) * (float)depth,
                                   node->Pos + node->Size
                                           - AzVec2(3, 3) * (float)depth,
                                   AZ_COL32(200, 100, 100, 255));
        AzVec2 pos = node->Pos + AzVec2(3, 3) * (float)depth;
        overlay_draw_list->AddRectFilled(pos - AzVec2(1, 1),
                                         pos + CalcTextSize(buf) + AzVec2(1, 1),
                                         AZ_COL32(200, 100, 100, 255));
        overlay_draw_list->AddText(NULL,
                                   0.0f,
                                   pos,
                                   AZ_COL32(255, 255, 255, 255),
                                   buf);
    }
#endif

    End();
}

void Az::DebugBreakClearData()
{
    AzContext& g = *GAz;
    g.DebugBreakInWindow = 0;
    g.DebugBreakInTable = 0;
    g.DebugBreakInShortcutRouting = AzKey_None;
}

void Az::DebugBreakButtonTooltip(bool keyboard_only,
                                 const char* description_of_location)
{
    if (!BeginItemTooltip())
        return;
    Text("To call AZ_DEBUG_BREAK() %s:", description_of_location);
    Separator();
    TextUnformatted(keyboard_only ? "- Press 'Pause/Break' on keyboard."
                                  : "- Press 'Pause/Break' on keyboard.\n- or "
                                    "Click (may alter focus/active id).\n- or "
                                    "navigate using keyboard and press space.");
    Separator();
    TextUnformatted(
            "Choose one way that doesn't interfere with what you are trying to "
            "debug!\nYou need a debugger attached or this will crash!");
    EndTooltip();
}

bool Az::DebugBreakButton(const char* label,
                          const char* description_of_location)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzID id = window->GetID(label);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    AzVec2 pos = window->DC.CursorPos
                 + AzVec2(0.0f, window->DC.CurrLineTextBaseOffset);
    AzVec2 size = AzVec2(label_size.x + g.Style.FramePadding.x * 2.0f,
                         label_size.y);

    const AzRect bb(pos, pos + size);
    ItemSize(size, 0.0f);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered = ItemHoverable(bb, id, g.CurrentItemFlags);
    bool pressed = hovered
                   && (IsKeyChordPressed(g.DebugBreakKeyChord)
                       || IsMouseClicked(0) || g.NavActivateId == id);
    DebugBreakButtonTooltip(false, description_of_location);

    AzVec4 col4f = GetStyleColorVec4(hovered ? AzCol_ButtonHovered
                                             : AzCol_Button);
    AzVec4 hsv;
    ColorConvertRGBtoHSV(col4f.x, col4f.y, col4f.z, hsv.x, hsv.y, hsv.z);
    ColorConvertHSVtoRGB(hsv.x + 0.20f,
                         hsv.y,
                         hsv.z,
                         col4f.x,
                         col4f.y,
                         col4f.z);

    RenderNavCursor(bb, id);
    RenderFrame(bb.Min,
                bb.Max,
                GetColorU32(col4f),
                true,
                g.Style.FrameRounding);
    RenderTextClipped(bb.Min,
                      bb.Max,
                      label,
                      NULL,
                      &label_size,
                      g.Style.ButtonTextAlign,
                      &bb);

    AZ_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

void Az::DebugNodeColumns(AzOldColumns* columns)
{
    if (!TreeNode((void*)(uintptr_t)columns->ID,
                  "Columns Id: 0x%08X, Count: %d, Flags: 0x%04X",
                  columns->ID,
                  columns->Count,
                  columns->Flags))
        return;
    BulletText("Width: %.1f (MinX: %.1f, MaxX: %.1f)",
               columns->OffMaxX - columns->OffMinX,
               columns->OffMinX,
               columns->OffMaxX);
    for (AzOldColumnData& column : columns->Columns)
        BulletText("Column %02d: OffsetNorm %.3f (= %.1f px)",
                   (int)columns->Columns.index_from_ptr(&column),
                   column.OffsetNorm,
                   GetColumnOffsetFromNorm(columns, column.OffsetNorm));
    TreePop();
}

static void DebugNodeDockNodeFlags(AzDockNodeFlags* p_flags,
                                   const char* label,
                                   bool enabled)
{
    using namespace Az;
    PushID(label);
    PushStyleVar(AzStyleVar_FramePadding, AzVec2(0.0f, 0.0f));
    Text("%s:", label);
    if (!enabled)
        BeginDisabled();
    CheckboxFlags("NoResize", p_flags, AzDockNodeFlags_NoResize);
    CheckboxFlags("NoResizeX", p_flags, AzDockNodeFlags_NoResizeX);
    CheckboxFlags("NoResizeY", p_flags, AzDockNodeFlags_NoResizeY);
    CheckboxFlags("NoTabBar", p_flags, AzDockNodeFlags_NoTabBar);
    CheckboxFlags("HiddenTabBar", p_flags, AzDockNodeFlags_HiddenTabBar);
    CheckboxFlags("NoWindowMenuButton",
                  p_flags,
                  AzDockNodeFlags_NoWindowMenuButton);
    CheckboxFlags("NoCloseButton", p_flags, AzDockNodeFlags_NoCloseButton);
    CheckboxFlags("DockedWindowsInFocusRoute",
                  p_flags,
                  AzDockNodeFlags_DockedWindowsInFocusRoute);
    CheckboxFlags("NoDocking", p_flags, AzDockNodeFlags_NoDocking);
    CheckboxFlags("NoDockingSplit", p_flags, AzDockNodeFlags_NoDockingSplit);
    CheckboxFlags("NoDockingSplitOther",
                  p_flags,
                  AzDockNodeFlags_NoDockingSplitOther);
    CheckboxFlags("NoDockingOver", p_flags, AzDockNodeFlags_NoDockingOverMe);
    CheckboxFlags("NoDockingOverOther",
                  p_flags,
                  AzDockNodeFlags_NoDockingOverOther);
    CheckboxFlags("NoDockingOverEmpty",
                  p_flags,
                  AzDockNodeFlags_NoDockingOverEmpty);
    CheckboxFlags("NoUndocking", p_flags, AzDockNodeFlags_NoUndocking);
    if (!enabled)
        EndDisabled();
    PopStyleVar();
    PopID();
}

void Az::DebugNodeDockNode(AzDockNode* node, const char* label)
{
    AzContext& g = *GAz;
    const bool is_alive = (g.FrameCount - node->LastFrameAlive < 2);

    const bool is_active = (g.FrameCount - node->LastFrameActive < 2);
    if (!is_alive)
    {
        PushStyleColor(AzCol_Text, GetStyleColorVec4(AzCol_TextDisabled));
    }
    bool open;
    AzTreeNodeFlags tree_node_flags = node->IsFocused ? AzTreeNodeFlags_Selected
                                                      : AzTreeNodeFlags_None;
    if (node->Windows.Size > 0)
        open = TreeNodeEx((void*)(intptr_t)node->ID,
                          tree_node_flags,
                          "%s 0x%04X%s: %d windows (vis: '%s')",
                          label,
                          node->ID,
                          node->IsVisible ? "" : " (hidden)",
                          node->Windows.Size,
                          node->VisibleWindow ? node->VisibleWindow->Name
                                              : "NULL");
    else
        open = TreeNodeEx((void*)(intptr_t)node->ID,
                          tree_node_flags,
                          "%s 0x%04X%s: %s (vis: '%s')",
                          label,
                          node->ID,
                          node->IsVisible ? "" : " (hidden)",
                          (node->SplitAxis == AzAxis_X)   ? "horizontal split"
                          : (node->SplitAxis == AzAxis_Y) ? "vertical split"
                                                          : "empty",
                          node->VisibleWindow ? node->VisibleWindow->Name
                                              : "NULL");
    if (!is_alive)
    {
        PopStyleColor();
    }
    if (is_active && IsItemHovered())
        if (AzWindow* window = node->HostWindow ? node->HostWindow
                                                : node->VisibleWindow)
            GetForegroundDrawList(window)->AddRect(node->Pos,
                                                   node->Pos + node->Size,
                                                   AZ_COL32(255, 255, 0, 255));
    if (open)
    {
        AZ_ASSERT(node->ChildNodes[0] == NULL
                  || node->ChildNodes[0]->ParentNode == node);
        AZ_ASSERT(node->ChildNodes[1] == NULL
                  || node->ChildNodes[1]->ParentNode == node);
        BulletText("Pos (%.0f,%.0f), Size (%.0f, %.0f) Ref (%.0f, %.0f)",
                   node->Pos.x,
                   node->Pos.y,
                   node->Size.x,
                   node->Size.y,
                   node->SizeRef.x,
                   node->SizeRef.y);
        DebugNodeWindow(node->HostWindow, "HostWindow");
        DebugNodeWindow(node->VisibleWindow, "VisibleWindow");
        BulletText("SelectedTabID: 0x%08X, LastFocusedNodeID: 0x%08X",
                   node->SelectedTabId,
                   node->LastFocusedNodeId);
        BulletText("Misc:%s%s%s%s%s%s%s",
                   node->IsDockSpace() ? " IsDockSpace" : "",
                   node->IsCentralNode() ? " IsCentralNode" : "",
                   is_alive ? " IsAlive" : "",
                   is_active ? " IsActive" : "",
                   node->IsFocused ? " IsFocused" : "",
                   node->WantLockSizeOnce ? " WantLockSizeOnce" : "",
                   node->HasCentralNodeChild ? " HasCentralNodeChild" : "");
        if (TreeNode("flags",
                     "Flags Merged: 0x%04X, Local: 0x%04X, InWindows: 0x%04X, "
                     "Shared: 0x%04X",
                     node->MergedFlags,
                     node->LocalFlags,
                     node->LocalFlagsInWindows,
                     node->SharedFlags))
        {
            if (BeginTable("flags", 4))
            {
                TableNextColumn();
                DebugNodeDockNodeFlags(&node->MergedFlags,
                                       "MergedFlags",
                                       false);
                TableNextColumn();
                DebugNodeDockNodeFlags(&node->LocalFlags, "LocalFlags", true);
                TableNextColumn();
                DebugNodeDockNodeFlags(&node->LocalFlagsInWindows,
                                       "LocalFlagsInWindows",
                                       false);
                TableNextColumn();
                DebugNodeDockNodeFlags(&node->SharedFlags, "SharedFlags", true);
                EndTable();
            }
            TreePop();
        }
        if (node->ParentNode)
            DebugNodeDockNode(node->ParentNode, "ParentNode");
        if (node->ChildNodes[0])
            DebugNodeDockNode(node->ChildNodes[0], "Child[0]");
        if (node->ChildNodes[1])
            DebugNodeDockNode(node->ChildNodes[1], "Child[1]");
        if (node->TabBar)
            DebugNodeTabBar(node->TabBar, "TabBar");
        DebugNodeWindowsList(&node->Windows, "Windows");

        TreePop();
    }
}

void Az::DebugNodeDrawList(AzWindow* window,
                           AzViewportP* viewport,
                           const AzDrawList* draw_list,
                           const char* label)
{
    AzContext& g = *GAz;
    AzMetricsConfig* cfg = &g.DebugMetricsConfig;
    int cmd_count = draw_list->CmdBuffer.Size;
    if (cmd_count > 0 && draw_list->CmdBuffer.back().ElemCount == 0
        && draw_list->CmdBuffer.back().UserCallback == NULL)
        cmd_count--;
    bool node_open = TreeNode(draw_list,
                              "%s: '%s' %d vtx, %d indices, %d cmds",
                              label,
                              draw_list->_OwnerName ? draw_list->_OwnerName
                                                    : "",
                              draw_list->VtxBuffer.Size,
                              draw_list->IdxBuffer.Size,
                              cmd_count);
    if (draw_list == GetWindowDrawList())
    {
        SameLine();
        TextColored(AzVec4(1.0f, 0.4f, 0.4f, 1.0f), "CURRENTLY APPENDING");

        if (node_open)
            TreePop();
        return;
    }

    AzDrawList* fg_draw_list = viewport ? GetForegroundDrawList(viewport)
                                        : NULL;

    if (window && IsItemHovered() && fg_draw_list)
        fg_draw_list->AddRect(window->Pos,
                              window->Pos + window->Size,
                              AZ_COL32(255, 255, 0, 255));
    if (!node_open)
        return;

    if (window && !window->WasActive)
        TextDisabled(
                "Warning: owning Window is inactive. This DrawList is not "
                "being rendered!");

    for (const AzDrawCmd* pcmd = draw_list->CmdBuffer.Data;
         pcmd < draw_list->CmdBuffer.Data + cmd_count;
         pcmd++)
    {
        if (pcmd->UserCallback)
        {
            BulletText("Callback %p, user_data %p",
                       pcmd->UserCallback,
                       pcmd->UserCallbackData);
            continue;
        }

        char texid_desc[30];
        FormatTextureIDForDebugDisplay(texid_desc,
                                       AZ_ARRAYSIZE(texid_desc),
                                       pcmd);
        char buf[300];
        AzFormatString(buf,
                       AZ_ARRAYSIZE(buf),
                       "DrawCmd:%5d tris, Tex %s, ClipRect "
                       "(%4.0f,%4.0f)-(%4.0f,%4.0f)",
                       pcmd->ElemCount / 3,
                       texid_desc,
                       pcmd->ClipRect.x,
                       pcmd->ClipRect.y,
                       pcmd->ClipRect.z,
                       pcmd->ClipRect.w);
        bool pcmd_node_open = TreeNode((void*)(pcmd
                                               - draw_list->CmdBuffer.begin()),
                                       "%s",
                                       buf);
        if (IsItemHovered()
            && (cfg->ShowDrawCmdMesh || cfg->ShowDrawCmdBoundingBoxes)
            && fg_draw_list)
            DebugNodeDrawCmdShowMeshAndBoundingBox(
                    fg_draw_list,
                    draw_list,
                    pcmd,
                    cfg->ShowDrawCmdMesh,
                    cfg->ShowDrawCmdBoundingBoxes);
        if (!pcmd_node_open)
            continue;

        const AzDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0)
                                              ? draw_list->IdxBuffer.Data
                                              : NULL;
        const AzDrawVert* vtx_buffer = draw_list->VtxBuffer.Data
                                       + pcmd->VtxOffset;
        float total_area = 0.0f;
        for (unsigned int idx_n = pcmd->IdxOffset;
             idx_n < pcmd->IdxOffset + pcmd->ElemCount;)
        {
            AzVec2 triangle[3];
            for (int n = 0; n < 3; n++, idx_n++)
                triangle[n] =
                        vtx_buffer[idx_buffer ? idx_buffer[idx_n] : idx_n].pos;
            total_area += AzTriangleArea(triangle[0], triangle[1], triangle[2]);
        }

        AzFormatString(buf,
                       AZ_ARRAYSIZE(buf),
                       "Mesh: ElemCount: %d, VtxOffset: +%d, IdxOffset: +%d, "
                       "Area: ~%0.f px",
                       pcmd->ElemCount,
                       pcmd->VtxOffset,
                       pcmd->IdxOffset,
                       total_area);
        Selectable(buf);
        if (IsItemHovered() && fg_draw_list)
            DebugNodeDrawCmdShowMeshAndBoundingBox(fg_draw_list,
                                                   draw_list,
                                                   pcmd,
                                                   true,
                                                   false);

        AzListClipper clipper;
        clipper.Begin(pcmd->ElemCount / 3);

        while (clipper.Step())
            for (int prim = clipper.DisplayStart,
                     idx_i = pcmd->IdxOffset + clipper.DisplayStart * 3;
                 prim < clipper.DisplayEnd;
                 prim++)
            {
                char *buf_p = buf, *buf_end = buf + AZ_ARRAYSIZE(buf);
                AzVec2 triangle[3];
                for (int n = 0; n < 3; n++, idx_i++)
                {
                    const AzDrawVert& v =
                            vtx_buffer[idx_buffer ? idx_buffer[idx_i] : idx_i];
                    triangle[n] = v.pos;
                    buf_p += AzFormatString(buf_p,
                                            buf_end - buf_p,
                                            "%s %04d: pos (%8.2f,%8.2f), uv "
                                            "(%.6f,%.6f), col %08X\n",
                                            (n == 0) ? "Vert:" : "     ",
                                            idx_i,
                                            v.pos.x,
                                            v.pos.y,
                                            v.uv.x,
                                            v.uv.y,
                                            v.col);
                }

                Selectable(buf, false);
                if (fg_draw_list && IsItemHovered())
                {
                    AzDrawListFlags backup_flags = fg_draw_list->Flags;
                    fg_draw_list->Flags &= ~AzDrawListFlags_AntiAliasedLines;

                    fg_draw_list->AddPolyline(triangle,
                                              3,
                                              AZ_COL32(255, 255, 0, 255),
                                              AzDrawFlags_Closed,
                                              1.0f);
                    fg_draw_list->Flags = backup_flags;
                }
            }
        TreePop();
    }
    TreePop();
}

void Az::DebugNodeDrawCmdShowMeshAndBoundingBox(AzDrawList* out_draw_list,
                                                const AzDrawList* draw_list,
                                                const AzDrawCmd* draw_cmd,
                                                bool show_mesh,
                                                bool show_aabb)
{
    AZ_ASSERT(show_mesh || show_aabb);

    AzRect clip_rect = draw_cmd->ClipRect;
    AzRect vtxs_rect(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
    AzDrawListFlags backup_flags = out_draw_list->Flags;
    out_draw_list->Flags &= ~AzDrawListFlags_AntiAliasedLines;

    for (unsigned int idx_n = draw_cmd->IdxOffset,
                      idx_end = draw_cmd->IdxOffset + draw_cmd->ElemCount;
         idx_n < idx_end;)
    {
        AzDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0)
                                        ? draw_list->IdxBuffer.Data
                                        : NULL;

        AzDrawVert* vtx_buffer = draw_list->VtxBuffer.Data
                                 + draw_cmd->VtxOffset;

        AzVec2 triangle[3];
        for (int n = 0; n < 3; n++, idx_n++)
            vtxs_rect.Add(
                    (triangle[n] =
                             vtx_buffer[idx_buffer ? idx_buffer[idx_n] : idx_n]
                                     .pos));
        if (show_mesh)
            out_draw_list->AddPolyline(triangle,
                                       3,
                                       AZ_COL32(255, 255, 0, 255),
                                       AzDrawFlags_Closed,
                                       1.0f);
    }

    if (show_aabb)
    {
        out_draw_list->AddRect(AzTrunc(clip_rect.Min),
                               AzTrunc(clip_rect.Max),
                               AZ_COL32(255, 0, 255, 255));

        out_draw_list->AddRect(AzTrunc(vtxs_rect.Min),
                               AzTrunc(vtxs_rect.Max),
                               AZ_COL32(0, 255, 255, 255));
    }
    out_draw_list->Flags = backup_flags;
}

static int CalcFontGlyphSrcOverlapMask(AzFontAtlas* atlas,
                                       AzFont* font,
                                       unsigned int codepoint)
{
    int mask = 0, count = 0;
    for (int src_n = 0; src_n < font->Sources.Size; src_n++)
    {
        AzFontConfig* src = font->Sources[src_n];
        if (!(src->FontLoader ? src->FontLoader : atlas->FontLoader)
                     ->FontSrcContainsGlyph(atlas, src, (AzWchar)codepoint))
            continue;
        mask |= (1 << src_n);
        count++;
    }
    return count > 1 ? mask : 0;
}

void Az::DebugNodeFont(AzFont* font)
{
    AzContext& g = *GAz;
    AzMetricsConfig* cfg = &g.DebugMetricsConfig;
    AzFontAtlas* atlas = font->ContainerAtlas;
    bool opened = TreeNode(font,
                           "Font: \"%s\": %d sources(s)",
                           font->GetDebugName(),
                           font->Sources.Size);

    if (!opened)
        Indent();
    Indent();
    if (cfg->ShowFontPreview)
    {
        PushFont(font);
        Text("The quick brown fox jumps over the lazy dog");
        PopFont();
    }
    if (!opened)
    {
        Unindent();
        Unindent();
        return;
    }
    if (SmallButton("Set as default"))
        GetIO().FontDefault = font;
    SameLine();
    BeginDisabled(atlas->Fonts.Size <= 1 || atlas->Locked);
    if (SmallButton("Remove"))
        atlas->RemoveFont(font);
    EndDisabled();
    SameLine();
    if (SmallButton("Clear bakes"))
        AzFontAtlasFontDiscardBakes(atlas, font, 0);
    SameLine();
    if (SmallButton("Clear unused"))
        AzFontAtlasFontDiscardBakes(atlas, font, 2);

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    SetNextItemWidth(GetFontSize() * 8);
    DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");

#endif

    char c_str[5];
    Text("Fallback character: '%s' (U+%04X)",
         AzTextCharToUtf8(c_str, font->FallbackChar),
         font->FallbackChar);
    Text("Ellipsis character: '%s' (U+%04X)",
         AzTextCharToUtf8(c_str, font->EllipsisChar),
         font->EllipsisChar);

    for (int src_n = 0; src_n < font->Sources.Size; src_n++)
    {
        AzFontConfig* src = font->Sources[src_n];
        if (TreeNode(src,
                     "Input %d: \'%s\', Oversample: %d,%d, PixelSnapH: %d, "
                     "Offset: (%.1f,%.1f)",
                     src_n,
                     src->Name,
                     src->OversampleH,
                     src->OversampleV,
                     src->PixelSnapH,
                     src->GlyphOffset.x,
                     src->GlyphOffset.y))
        {
            const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                         : atlas->FontLoader;
            Text("Loader: '%s'", loader->Name ? loader->Name : "N/A");
#ifdef AZ_ENABLE_FREETYPE
            if (loader->Name != NULL && strcmp(loader->Name, "FreeType") == 0)
            {
                unsigned int loader_flags = src->FontLoaderFlags;
                Text("FreeType Loader Flags: 0x%08X", loader_flags);
                if (AzFreeType::DebugEditFontLoaderFlags(&loader_flags))
                {
                    AzFontAtlasFontDestroyOutput(atlas, font);
                    src->FontLoaderFlags = loader_flags;
                    AzFontAtlasFontInitOutput(atlas, font);
                }
            }
#endif
            TreePop();
        }
    }
    if (font->Sources.Size > 1
        && TreeNode("Input Glyphs Overlap Detection Tool"))
    {
        TextWrapped(
                "- First Input that contains the glyph is used.\n- Use "
                "AzFontConfig::GlyphExcludeRanges[] to specify ranges to "
                "ignore glyph in given Input.\n- This tool doesn't cache "
                "results and is slow, don't keep it open!");
        if (BeginTable("table", 2))
        {
            for (unsigned int c = 0; c < 0x10000; c++)
                if (int overlap_mask = CalcFontGlyphSrcOverlapMask(atlas,
                                                                   font,
                                                                   c))
                {
                    unsigned int c_end = c + 1;
                    while (c_end < 0x10000
                           && CalcFontGlyphSrcOverlapMask(atlas, font, c_end)
                                      == overlap_mask)
                        c_end++;
                    if (TableNextColumn()
                        && TreeNode((void*)(intptr_t)c,
                                    "U+%04X-U+%04X: %d codepoints in %d inputs",
                                    c,
                                    c_end - 1,
                                    c_end - c,
                                    AzCountSetBits(overlap_mask)))
                    {
                        char utf8_buf[5];
                        for (unsigned int n = c; n < c_end; n++)
                            BulletText("Codepoint U+%04X (%s)",
                                       n,
                                       AzTextCharToUtf8(utf8_buf, n));
                        TreePop();
                    }
                    TableNextColumn();
                    for (int src_n = 0; src_n < font->Sources.Size; src_n++)
                        if (overlap_mask & (1 << src_n))
                        {
                            Text("%d ", src_n);
                            SameLine();
                        }
                    c = c_end - 1;
                }
            EndTable();
        }
        TreePop();
    }

    for (int baked_n = 0; baked_n < atlas->Builder->BakedPool.Size; baked_n++)
    {
        AzFontBaked* baked = &atlas->Builder->BakedPool[baked_n];
        if (baked->ContainerFont != font)
            continue;
        PushID(baked_n);
        if (TreeNode("Glyphs",
                     "Baked at { %.2fpx, d.%.1f }: %d glyphs%s",
                     baked->Size,
                     baked->RasterizerDensity,
                     baked->Glyphs.Size,
                     (baked->LastUsedFrame < atlas->Builder->FrameCount - 1)
                             ? " *Unused*"
                             : ""))
        {
            if (SmallButton("Load all"))
                for (unsigned int base = 0; base <= AZ_UNICODE_CODEPOINT_MAX;
                     base++)
                    baked->FindGlyph((AzWchar)base);

            const int surface_sqrt = (int)AzSqrt(
                    (float)baked->MetricsTotalSurface);
            Text("Ascent: %f, Descent: %f, Ascent-Descent: %f",
                 baked->Ascent,
                 baked->Descent,
                 baked->Ascent - baked->Descent);
            Text("Texture Area: about %d px ~%dx%d px",
                 baked->MetricsTotalSurface,
                 surface_sqrt,
                 surface_sqrt);
            for (int src_n = 0; src_n < font->Sources.Size; src_n++)
            {
                AzFontConfig* src = font->Sources[src_n];
                int oversample_h, oversample_v;
                AzFontAtlasBuildGetOversampleFactors(src,
                                                     baked,
                                                     &oversample_h,
                                                     &oversample_v);
                BulletText(
                        "Input %d: \'%s\', Oversample: (%d=>%d,%d=>%d), "
                        "PixelSnapH: %d, Offset: (%.1f,%.1f)",
                        src_n,
                        src->Name,
                        src->OversampleH,
                        oversample_h,
                        src->OversampleV,
                        oversample_v,
                        src->PixelSnapH,
                        src->GlyphOffset.x,
                        src->GlyphOffset.y);
            }

            DebugNodeFontGlyphesForSrcMask(font, baked, ~0);
            TreePop();
        }
        PopID();
    }
    TreePop();
    Unindent();
}

void Az::DebugNodeFontGlyphesForSrcMask(AzFont* font,
                                        AzFontBaked* baked,
                                        int src_mask)
{
    AzDrawList* draw_list = GetWindowDrawList();
    const AzU32 glyph_col = GetColorU32(AzCol_Text);
    const float cell_size = baked->Size * 1;
    const float cell_spacing = GetStyle().ItemSpacing.y;
    for (unsigned int base = 0; base <= AZ_UNICODE_CODEPOINT_MAX; base += 256)
    {
        if (!(base & 8191) && font->IsGlyphRangeUnused(base, base + 8191))
        {
            base += 8192 - 256;
            continue;
        }

        int count = 0;
        for (unsigned int n = 0; n < 256; n++)
            if (const AzFontGlyph* glyph =
                        baked->IsGlyphLoaded((AzWchar)(base + n))
                                ? baked->FindGlyph((AzWchar)(base + n))
                                : NULL)
                if (src_mask & (1 << glyph->SourceIdx))
                    count++;
        if (count <= 0)
            continue;
        if (!TreeNode((void*)(intptr_t)base,
                      "U+%04X..U+%04X (%d %s)",
                      base,
                      base + 255,
                      count,
                      count > 1 ? "glyphs" : "glyph"))
            continue;

        AzVec2 base_pos = GetCursorScreenPos();
        for (unsigned int n = 0; n < 256; n++)
        {
            AzVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing),
                           base_pos.y + (n / 16) * (cell_size + cell_spacing));
            AzVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
            const AzFontGlyph* glyph = baked->IsGlyphLoaded((AzWchar)(base + n))
                                               ? baked->FindGlyph(
                                                         (AzWchar)(base + n))
                                               : NULL;
            draw_list->AddRect(cell_p1,
                               cell_p2,
                               glyph ? AZ_COL32(255, 255, 255, 100)
                                     : AZ_COL32(255, 255, 255, 50));
            if (!glyph || (src_mask & (1 << glyph->SourceIdx)) == 0)
                continue;
            font->RenderChar(draw_list,
                             cell_size,
                             cell_p1,
                             glyph_col,
                             (AzWchar)(base + n));
            if (IsMouseHoveringRect(cell_p1, cell_p2) && BeginTooltip())
            {
                DebugNodeFontGlyph(font, glyph);
                EndTooltip();
            }
        }
        Dummy(AzVec2((cell_size + cell_spacing) * 16,
                     (cell_size + cell_spacing) * 16));
        TreePop();
    }
}

void Az::DebugNodeFontGlyph(AzFont* font, const AzFontGlyph* glyph)
{
    Text("Codepoint: U+%04X", glyph->Codepoint);
    Separator();
    Text("Visible: %d", glyph->Visible);
    Text("AdvanceX: %.1f", glyph->AdvanceX);
    Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)",
         glyph->X0,
         glyph->Y0,
         glyph->X1,
         glyph->Y1);
    Text("UV: (%.3f,%.3f)->(%.3f,%.3f)",
         glyph->U0,
         glyph->V0,
         glyph->U1,
         glyph->V1);
    if (glyph->PackId >= 0)
    {
        AzTextureRect* r = AzFontAtlasPackGetRect(font->ContainerAtlas,
                                                  glyph->PackId);
        Text("PackId: %d (%dx%d rect at %d,%d)",
             glyph->PackId,
             r->w,
             r->h,
             r->x,
             r->y);
    }
    Text("SourceIdx: %d", glyph->SourceIdx);
}

void Az::DebugNodeStorage(AzStorage* storage, const char* label)
{
    if (!TreeNode(label,
                  "%s: %d entries, %d bytes",
                  label,
                  storage->Data.Size,
                  storage->Data.size_in_bytes()))
        return;
    for (const AzStoragePair& p : storage->Data)
    {
        BulletText("Key 0x%08X Value { i: %d }", p.key, p.val_i);

        DebugLocateItemOnHover(p.key);
    }
    TreePop();
}

void Az::DebugNodeTabBar(AzTabBar* tab_bar, const char* label)
{
    char buf[256];
    char* p = buf;
    const char* buf_end = buf + AZ_ARRAYSIZE(buf);
    const bool is_active = (tab_bar->PrevFrameVisible >= GetFrameCount() - 2);
    p += AzFormatString(p,
                        buf_end - p,
                        "%s 0x%08X (%d tabs)%s  {",
                        label,
                        tab_bar->ID,
                        tab_bar->Tabs.Size,
                        is_active ? "" : " *Inactive*");
    for (int tab_n = 0; tab_n < AzMin(tab_bar->Tabs.Size, 3); tab_n++)
    {
        AzTabItem* tab = &tab_bar->Tabs[tab_n];
        p += AzFormatString(p,
                            buf_end - p,
                            "%s'%s'",
                            tab_n > 0 ? ", " : "",
                            TabBarGetTabName(tab_bar, tab));
    }
    p += AzFormatString(p,
                        buf_end - p,
                        (tab_bar->Tabs.Size > 3) ? " ... }" : " } ");
    if (!is_active)
    {
        PushStyleColor(AzCol_Text, GetStyleColorVec4(AzCol_TextDisabled));
    }
    bool open = TreeNode(label, "%s", buf);
    if (!is_active)
    {
        PopStyleColor();
    }
    if (is_active && IsItemHovered())
    {
        AzDrawList* draw_list = GetForegroundDrawList();
        draw_list->AddRect(tab_bar->BarRect.Min,
                           tab_bar->BarRect.Max,
                           AZ_COL32(255, 255, 0, 255));
        draw_list->AddLine(AzVec2(tab_bar->ScrollingRectMinX,
                                  tab_bar->BarRect.Min.y),
                           AzVec2(tab_bar->ScrollingRectMinX,
                                  tab_bar->BarRect.Max.y),
                           AZ_COL32(0, 255, 0, 255));
        draw_list->AddLine(AzVec2(tab_bar->ScrollingRectMaxX,
                                  tab_bar->BarRect.Min.y),
                           AzVec2(tab_bar->ScrollingRectMaxX,
                                  tab_bar->BarRect.Max.y),
                           AZ_COL32(0, 255, 0, 255));
    }
    if (open)
    {
        for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
        {
            AzTabItem* tab = &tab_bar->Tabs[tab_n];
            PushID(tab);
            if (SmallButton("<"))
            {
                TabBarQueueReorder(tab_bar, tab, -1);
            }
            SameLine(0, 2);
            if (SmallButton(">"))
            {
                TabBarQueueReorder(tab_bar, tab, +1);
            }
            SameLine();
            Text("%02d%c Tab 0x%08X '%s' Offset: %.2f, Width: %.2f/%.2f",
                 tab_n,
                 (tab->ID == tab_bar->SelectedTabId) ? '*' : ' ',
                 tab->ID,
                 TabBarGetTabName(tab_bar, tab),
                 tab->Offset,
                 tab->Width,
                 tab->ContentWidth);
            PopID();
        }
        TreePop();
    }
}

void Az::DebugNodeViewport(AzViewportP* viewport)
{
    AzContext& g = *GAz;
    SetNextItemOpen(true, AzCond_Once);
    bool open =
            TreeNode((void*)(intptr_t)viewport->ID,
                     "Viewport #%d, ID: 0x%08X, Parent: 0x%08X, Window: \"%s\"",
                     viewport->Idx,
                     viewport->ID,
                     viewport->ParentViewportId,
                     viewport->Window ? viewport->Window->Name : "N/A");
    if (IsItemHovered())
        g.DebugMetricsConfig.HighlightViewportID = viewport->ID;
    if (open)
    {
        AzWindowFlags flags = viewport->Flags;
        BulletText(
                "Main Pos: (%.0f,%.0f), Size: (%.0f,%.0f)\nFrameBufferScale: "
                "(%.2f,%.2f)\nWorkArea Inset Left: %.0f Top: %.0f, Right: "
                "%.0f, Bottom: %.0f\nMonitor: %d, DpiScale: %.0f%%",
                viewport->Pos.x,
                viewport->Pos.y,
                viewport->Size.x,
                viewport->Size.y,
                viewport->FramebufferScale.x,
                viewport->FramebufferScale.y,
                viewport->WorkInsetMin.x,
                viewport->WorkInsetMin.y,
                viewport->WorkInsetMax.x,
                viewport->WorkInsetMax.y,
                viewport->PlatformMonitor,
                viewport->DpiScale * 100.0f);
        if (viewport->Idx > 0)
        {
            SameLine();
            if (SmallButton("Reset Pos"))
            {
                viewport->Pos = AzVec2(200, 200);
                viewport->UpdateWorkRect();
                if (viewport->Window)
                    viewport->Window->Pos = viewport->Pos;
            }
        }
        BulletText(
                "Flags: 0x%04X =%s%s%s%s%s%s%s%s%s%s%s%s%s",
                viewport->Flags,

                (flags & AzViewportFlags_IsPlatformMonitor)
                        ? " IsPlatformMonitor"
                        : "",
                (flags & AzViewportFlags_IsMinimized) ? " IsMinimized" : "",
                (flags & AzViewportFlags_IsFocused) ? " IsFocused" : "",
                (flags & AzViewportFlags_OwnedByApp) ? " OwnedByApp" : "",
                (flags & AzViewportFlags_NoDecoration) ? " NoDecoration" : "",
                (flags & AzViewportFlags_NoTaskBarIcon) ? " NoTaskBarIcon" : "",
                (flags & AzViewportFlags_NoFocusOnAppearing)
                        ? " NoFocusOnAppearing"
                        : "",
                (flags & AzViewportFlags_NoFocusOnClick) ? " NoFocusOnClick"
                                                         : "",
                (flags & AzViewportFlags_NoInputs) ? " NoInputs" : "",
                (flags & AzViewportFlags_NoRendererClear) ? " NoRendererClear"
                                                          : "",
                (flags & AzViewportFlags_NoAutoMerge) ? " NoAutoMerge" : "",
                (flags & AzViewportFlags_TopMost) ? " TopMost" : "",
                (flags & AzViewportFlags_CanHostOtherWindows)
                        ? " CanHostOtherWindows"
                        : "");
        for (AzDrawList* draw_list : viewport->DrawDataP.CmdLists)
            DebugNodeDrawList(NULL, viewport, draw_list, "DrawList");
        TreePop();
    }
}

void Az::DebugNodePlatformMonitor(AzPlatformMonitor* monitor,
                                  const char* label,
                                  int idx)
{
    BulletText(
            "%s %d: DPI %.0f%%\n MainMin (%.0f,%.0f), MainMax (%.0f,%.0f), "
            "MainSize (%.0f,%.0f)\n WorkMin (%.0f,%.0f), WorkMax (%.0f,%.0f), "
            "WorkSize (%.0f,%.0f)",
            label,
            idx,
            monitor->DpiScale * 100.0f,
            monitor->MainPos.x,
            monitor->MainPos.y,
            monitor->MainPos.x + monitor->MainSize.x,
            monitor->MainPos.y + monitor->MainSize.y,
            monitor->MainSize.x,
            monitor->MainSize.y,
            monitor->WorkPos.x,
            monitor->WorkPos.y,
            monitor->WorkPos.x + monitor->WorkSize.x,
            monitor->WorkPos.y + monitor->WorkSize.y,
            monitor->WorkSize.x,
            monitor->WorkSize.y);
}

void Az::DebugNodeWindow(AzWindow* window, const char* label)
{
    if (window == NULL)
    {
        BulletText("%s: NULL", label);
        return;
    }

    AzContext& g = *GAz;
    const bool is_active = window->WasActive;
    AzTreeNodeFlags tree_node_flags = (window == g.NavWindow)
                                              ? AzTreeNodeFlags_Selected
                                              : AzTreeNodeFlags_None;
    if (!is_active)
    {
        PushStyleColor(AzCol_Text, GetStyleColorVec4(AzCol_TextDisabled));
    }
    const bool open = TreeNodeEx(label,
                                 tree_node_flags,
                                 "%s '%s'%s",
                                 label,
                                 window->Name,
                                 is_active ? "" : " *Inactive*");
    if (!is_active)
    {
        PopStyleColor();
    }
    if (IsItemHovered() && is_active)
        GetForegroundDrawList(window)->AddRect(window->Pos,
                                               window->Pos + window->Size,
                                               AZ_COL32(255, 255, 0, 255));
    if (!open)
        return;

    if (window->MemoryCompacted)
        TextDisabled("Note: some memory buffers have been compacted/freed.");

    if (g.IO.ConfigDebugIsDebuggerPresent
        && DebugBreakButton("**DebugBreak**", "in Begin()"))
        g.DebugBreakInWindow = window->ID;

    AzWindowFlags flags = window->Flags;
    DebugNodeDrawList(window, window->Viewport, window->DrawList, "DrawList");
    BulletText(
            "Pos: (%.1f,%.1f), Size: (%.1f,%.1f), ContentSize (%.1f,%.1f) "
            "Ideal (%.1f,%.1f)",
            window->Pos.x,
            window->Pos.y,
            window->Size.x,
            window->Size.y,
            window->ContentSize.x,
            window->ContentSize.y,
            window->ContentSizeIdeal.x,
            window->ContentSizeIdeal.y);
    BulletText("Flags: 0x%08X (%s%s%s%s%s%s%s%s%s..)",
               flags,
               (flags & AzWindowFlags_ChildWindow) ? "Child " : "",
               (flags & AzWindowFlags_Tooltip) ? "Tooltip " : "",
               (flags & AzWindowFlags_Popup) ? "Popup " : "",
               (flags & AzWindowFlags_Modal) ? "Modal " : "",
               (flags & AzWindowFlags_ChildMenu) ? "ChildMenu " : "",
               (flags & AzWindowFlags_NoSavedSettings) ? "NoSavedSettings "
                                                       : "",
               (flags & AzWindowFlags_NoMouseInputs) ? "NoMouseInputs" : "",
               (flags & AzWindowFlags_NoNavInputs) ? "NoNavInputs" : "",
               (flags & AzWindowFlags_AlwaysAutoResize) ? "AlwaysAutoResize"
                                                        : "");
    if (flags & AzWindowFlags_ChildWindow)
        BulletText(
                "ChildFlags: 0x%08X (%s%s%s%s..)",
                window->ChildFlags,
                (window->ChildFlags & AzChildFlags_Borders) ? "Borders " : "",
                (window->ChildFlags & AzChildFlags_ResizeX) ? "ResizeX " : "",
                (window->ChildFlags & AzChildFlags_ResizeY) ? "ResizeY " : "",
                (window->ChildFlags & AzChildFlags_NavFlattened)
                        ? "NavFlattened "
                        : "");
    BulletText("WindowClassId: 0x%08X", window->WindowClass.ClassId);
    BulletText("Scroll: (%.2f/%.2f,%.2f/%.2f) Scrollbar:%s%s",
               window->Scroll.x,
               window->ScrollMax.x,
               window->Scroll.y,
               window->ScrollMax.y,
               window->ScrollbarX ? "X" : "",
               window->ScrollbarY ? "Y" : "");
    BulletText("Active: %d/%d, WriteAccessed: %d, BeginOrderWithinContext: %d",
               window->Active,
               window->WasActive,
               window->WriteAccessed,
               (window->Active || window->WasActive)
                       ? window->BeginOrderWithinContext
                       : -1);
    BulletText(
            "Appearing: %d, Hidden: %d (CanSkip %d Cannot %d), SkipItems: %d",
            window->Appearing,
            window->Hidden,
            window->HiddenFramesCanSkipItems,
            window->HiddenFramesCannotSkipItems,
            window->SkipItems);
    for (int layer = 0; layer < AzNavLayer_COUNT; layer++)
    {
        AzRect r = window->NavRectRel[layer];
        if (r.Min.x >= r.Max.x && r.Min.y >= r.Max.y)
            BulletText("NavLastIds[%d]: 0x%08X",
                       layer,
                       window->NavLastIds[layer]);
        else
            BulletText("NavLastIds[%d]: 0x%08X at +(%.1f,%.1f)(%.1f,%.1f)",
                       layer,
                       window->NavLastIds[layer],
                       r.Min.x,
                       r.Min.y,
                       r.Max.x,
                       r.Max.y);
        DebugLocateItemOnHover(window->NavLastIds[layer]);
    }
    const AzVec2* pr = window->NavPreferredScoringPosRel;
    for (int layer = 0; layer < AzNavLayer_COUNT; layer++)
        BulletText("NavPreferredScoringPosRel[%d] = {%.1f,%.1f)",
                   layer,
                   (pr[layer].x == FLT_MAX ? -99999.0f : pr[layer].x),
                   (pr[layer].y == FLT_MAX ? -99999.0f : pr[layer].y));

    BulletText("NavLayersActiveMask: %X, NavLastChildNavWindow: %s",
               window->DC.NavLayersActiveMask,
               window->NavLastChildNavWindow
                       ? window->NavLastChildNavWindow->Name
                       : "NULL");

    BulletText("Viewport: %d%s, ViewportId: 0x%08X, ViewportPos: (%.1f,%.1f)",
               window->Viewport ? window->Viewport->Idx : -1,
               window->ViewportOwned ? " (Owned)" : "",
               window->ViewportId,
               window->ViewportPos.x,
               window->ViewportPos.y);
    BulletText("ViewportMonitor: %d",
               window->Viewport ? window->Viewport->PlatformMonitor : -1);
    BulletText("DockId: 0x%04X, DockOrder: %d, Act: %d, Vis: %d",
               window->DockId,
               window->DockOrder,
               window->DockIsActive,
               window->DockTabIsVisible);
    if (window->DockNode || window->DockNodeAsHost)
        DebugNodeDockNode(window->DockNodeAsHost ? window->DockNodeAsHost
                                                 : window->DockNode,
                          window->DockNodeAsHost ? "DockNodeAsHost"
                                                 : "DockNode");

    if (window->RootWindow != window)
    {
        DebugNodeWindow(window->RootWindow, "RootWindow");
    }
    if (window->RootWindowDockTree != window->RootWindow)
    {
        DebugNodeWindow(window->RootWindowDockTree, "RootWindowDockTree");
    }
    if (window->ParentWindow != NULL)
    {
        DebugNodeWindow(window->ParentWindow, "ParentWindow");
    }
    if (window->ParentWindowForFocusRoute != NULL)
    {
        DebugNodeWindow(window->ParentWindowForFocusRoute,
                        "ParentWindowForFocusRoute");
    }
    if (window->DC.ChildWindows.Size > 0)
    {
        DebugNodeWindowsList(&window->DC.ChildWindows, "ChildWindows");
    }
    if (window->ColumnsStorage.Size > 0
        && TreeNode("Columns",
                    "Columns sets (%d)",
                    window->ColumnsStorage.Size))
    {
        for (AzOldColumns& columns : window->ColumnsStorage)
            DebugNodeColumns(&columns);
        TreePop();
    }
    DebugNodeStorage(&window->StateStorage, "Storage");
    TreePop();
}

void Az::DebugNodeWindowSettings(AzWindowSettings* settings)
{
    if (settings->WantDelete)
        BeginDisabled();
    Text("0x%08X \"%s\" Pos (%d,%d) Size (%d,%d) Collapsed=%d",
         settings->ID,
         settings->GetName(),
         settings->Pos.x,
         settings->Pos.y,
         settings->Size.x,
         settings->Size.y,
         settings->Collapsed);
    if (settings->WantDelete)
        EndDisabled();
}

void Az::DebugNodeWindowsList(AzVector<AzWindow*>* windows, const char* label)
{
    if (!TreeNode(label, "%s (%d)", label, windows->Size))
        return;
    for (int i = windows->Size - 1; i >= 0; i--)
    {
        PushID((*windows)[i]);
        DebugNodeWindow((*windows)[i], "Window");
        PopID();
    }
    TreePop();
}

void Az::DebugNodeWindowsListByBeginStackParent(AzWindow** windows,
                                                int windows_size,
                                                AzWindow* parent_in_begin_stack)
{
    for (int i = 0; i < windows_size; i++)
    {
        AzWindow* window = windows[i];
        if (window->ParentWindowInBeginStack != parent_in_begin_stack)
            continue;
        char buf[20];
        AzFormatString(buf,
                       AZ_ARRAYSIZE(buf),
                       "[%04d] Window",
                       window->BeginOrderWithinContext);

        DebugNodeWindow(window, buf);
        TreePush(buf);
        DebugNodeWindowsListByBeginStackParent(windows + i + 1,
                                               windows_size - i - 1,
                                               window);
        TreePop();
    }
}

void Az::DebugLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    DebugLogV(fmt, args);
    va_end(args);
}

void Az::DebugLogV(const char* fmt, va_list args)
{
    AzContext& g = *GAz;
    const int old_size = g.DebugLogBuf.size();
    if (g.ContextName[0] != 0)
        g.DebugLogBuf.appendf("[%s] [%05d] ", g.ContextName, g.FrameCount);
    else
        g.DebugLogBuf.appendf("[%05d] ", g.FrameCount);
    g.DebugLogBuf.appendfv(fmt, args);
    g.DebugLogIndex.append(g.DebugLogBuf.c_str(),
                           old_size,
                           g.DebugLogBuf.size());
    if (g.DebugLogFlags & AzDebugLogFlags_OutputToTTY)
        AZ_DEBUG_PRINTF("%s", g.DebugLogBuf.begin() + old_size);
#ifdef AZ_ENABLE_TEST_ENGINE

    const int new_size = g.DebugLogBuf.size();
    const bool trailing_carriage_return = (g.DebugLogBuf[new_size - 1] == '\n');
    if (g.DebugLogFlags & AzDebugLogFlags_OutputToTestEngine)
        AZ_TEST_ENGINE_LOG("%.*s",
                           new_size - old_size
                                   - (trailing_carriage_return ? 1 : 0),
                           g.DebugLogBuf.begin() + old_size);
#endif
}

static void SameLineOrWrap(const AzVec2& size)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzVec2 pos(window->DC.CursorPosPrevLine.x + g.Style.ItemSpacing.x,
               window->DC.CursorPosPrevLine.y);
    if (window->WorkRect.Contains(AzRect(pos, pos + size)))
        Az::SameLine();
}

static void ShowDebugLogFlag(const char* name, AzDebugLogFlags flags)
{
    AzContext& g = *GAz;
    AzVec2 size(Az::GetFrameHeight() + g.Style.ItemInnerSpacing.x
                        + Az::CalcTextSize(name).x,
                Az::GetFrameHeight());
    SameLineOrWrap(size);

    bool highlight_errors = (flags == AzDebugLogFlags_EventError
                             && g.DebugLogSkippedErrors > 0);
    if (highlight_errors)
        Az::PushStyleColor(AzCol_Text,
                           AzLerp(g.Style.Colors[AzCol_Text],
                                  AzVec4(1.0f, 0.0f, 0.0f, 1.0f),
                                  0.30f));
    if (Az::CheckboxFlags(name, &g.DebugLogFlags, flags) && g.IO.KeyShift
        && (g.DebugLogFlags & flags) != 0)
    {
        g.DebugLogAutoDisableFrames = 2;
        g.DebugLogAutoDisableFlags |= flags;
    }
    if (highlight_errors)
    {
        Az::PopStyleColor();
        Az::SetItemTooltip("%d past errors skipped.", g.DebugLogSkippedErrors);
    } else
    {
        Az::SetItemTooltip(
                "Hold SHIFT when clicking to enable for 2 frames only (useful "
                "for spammy log entries)");
    }
}

void Az::ShowDebugLogWindow(bool* p_open)
{
    AzContext& g = *GAz;
    if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSize) == 0)
        SetNextWindowSize(AzVec2(0.0f, GetFontSize() * 12.0f),
                          AzCond_FirstUseEver);
    if (!Begin("Dear Az Debug Log", p_open)
        || GetCurrentWindow()->BeginCount > 1)
    {
        End();
        return;
    }

    AzDebugLogFlags all_enable_flags = AzDebugLogFlags_EventMask_
                                       & ~AzDebugLogFlags_EventInputRouting;
    CheckboxFlags("All", &g.DebugLogFlags, all_enable_flags);
    SetItemTooltip("(except InputRouting which is spammy)");

    ShowDebugLogFlag("Errors", AzDebugLogFlags_EventError);
    ShowDebugLogFlag("ActiveId", AzDebugLogFlags_EventActiveId);
    ShowDebugLogFlag("Clipper", AzDebugLogFlags_EventClipper);
    ShowDebugLogFlag("Docking", AzDebugLogFlags_EventDocking);
    ShowDebugLogFlag("Focus", AzDebugLogFlags_EventFocus);
    ShowDebugLogFlag("IO", AzDebugLogFlags_EventIO);
    ShowDebugLogFlag("Font", AzDebugLogFlags_EventFont);
    ShowDebugLogFlag("Nav", AzDebugLogFlags_EventNav);
    ShowDebugLogFlag("Popup", AzDebugLogFlags_EventPopup);
    ShowDebugLogFlag("Selection", AzDebugLogFlags_EventSelection);
    ShowDebugLogFlag("Viewport", AzDebugLogFlags_EventViewport);
    ShowDebugLogFlag("InputRouting", AzDebugLogFlags_EventInputRouting);

    if (SmallButton("Clear"))
    {
        g.DebugLogBuf.clear();
        g.DebugLogIndex.clear();
        g.DebugLogSkippedErrors = 0;
    }
    SameLine();
    if (SmallButton("Copy"))
        SetClipboardText(g.DebugLogBuf.c_str());
    SameLine();
    if (SmallButton("Configure Outputs.."))
        OpenPopup("Outputs");
    if (BeginPopup("Outputs"))
    {
        CheckboxFlags("OutputToTTY",
                      &g.DebugLogFlags,
                      AzDebugLogFlags_OutputToTTY);
#ifndef AZ_ENABLE_TEST_ENGINE
        BeginDisabled();
#endif
        CheckboxFlags("OutputToTestEngine",
                      &g.DebugLogFlags,
                      AzDebugLogFlags_OutputToTestEngine);
#ifndef AZ_ENABLE_TEST_ENGINE
        EndDisabled();
#endif
        EndPopup();
    }

    BeginChild("##log",
               AzVec2(0.0f, 0.0f),
               AzChildFlags_Borders,
               AzWindowFlags_AlwaysVerticalScrollbar
                       | AzWindowFlags_AlwaysHorizontalScrollbar);

    const AzDebugLogFlags backup_log_flags = g.DebugLogFlags;
    g.DebugLogFlags &= ~AzDebugLogFlags_EventClipper;

    AzListClipper clipper;
    clipper.Begin(g.DebugLogIndex.size());
    while (clipper.Step())
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd;
             line_no++)
            DebugTextUnformattedWithLocateItem(
                    g.DebugLogIndex.get_line_begin(g.DebugLogBuf.c_str(),
                                                   line_no),
                    g.DebugLogIndex.get_line_end(g.DebugLogBuf.c_str(),
                                                 line_no));
    g.DebugLogFlags = backup_log_flags;
    if (GetScrollY() >= GetScrollMaxY())
        SetScrollHereY(1.0f);
    EndChild();

    End();
}

void Az::DebugTextUnformattedWithLocateItem(const char* line_begin,
                                            const char* line_end)
{
    TextUnformatted(line_begin, line_end);
    if (!IsItemHovered(AzHoveredFlags_AllowWhenBlockedByPopup
                       | AzHoveredFlags_AllowWhenBlockedByActiveItem))
        return;
    AzContext& g = *GAz;
    AzRect text_rect = g.LastItemData.Rect;
    for (const char* p = line_begin; p <= line_end - 10; p++)
    {
        AzID id = 0;
        if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X')
            || sscanf(p + 2, "%X", &id) != 1 || AzCharIsXdigitA(p[10]))
            continue;
        AzVec2 p0 = CalcTextSize(line_begin, p);
        AzVec2 p1 = CalcTextSize(p, p + 10);
        g.LastItemData.Rect = AzRect(text_rect.Min + AzVec2(p0.x, 0.0f),
                                     text_rect.Min + AzVec2(p0.x + p1.x, p1.y));
        if (IsMouseHoveringRect(g.LastItemData.Rect.Min,
                                g.LastItemData.Rect.Max,
                                true))
            DebugLocateItemOnHover(id);
        p += 10;
    }
}

void Az::DebugDrawCursorPos(AzU32 col)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzVec2 pos = window->DC.CursorPos;
    window->DrawList->AddLine(AzVec2(pos.x, pos.y - 3.0f),
                              AzVec2(pos.x, pos.y + 4.0f),
                              col,
                              1.0f);
    window->DrawList->AddLine(AzVec2(pos.x - 3.0f, pos.y),
                              AzVec2(pos.x + 4.0f, pos.y),
                              col,
                              1.0f);
}

void Az::DebugDrawLineExtents(AzU32 col)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    float curr_x = window->DC.CursorPos.x;
    float line_y1 = (window->DC.IsSameLine ? window->DC.CursorPosPrevLine.y
                                           : window->DC.CursorPos.y);
    float line_y2 = line_y1
                    + (window->DC.IsSameLine ? window->DC.PrevLineSize.y
                                             : window->DC.CurrLineSize.y);
    window->DrawList->AddLine(AzVec2(curr_x - 5.0f, line_y1),
                              AzVec2(curr_x + 5.0f, line_y1),
                              col,
                              1.0f);
    window->DrawList->AddLine(AzVec2(curr_x - 0.5f, line_y1),
                              AzVec2(curr_x - 0.5f, line_y2),
                              col,
                              1.0f);
    window->DrawList->AddLine(AzVec2(curr_x - 5.0f, line_y2),
                              AzVec2(curr_x + 5.0f, line_y2),
                              col,
                              1.0f);
}

void Az::DebugDrawItemRect(AzU32 col)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    GetForegroundDrawList(window)->AddRect(g.LastItemData.Rect.Min,
                                           g.LastItemData.Rect.Max,
                                           col);
}

static const AzU32 DEBUG_LOCATE_ITEM_COLOR = AZ_COL32(0, 255, 0, 255);

void Az::DebugLocateItem(AzID target_id)
{
    AzContext& g = *GAz;
    g.DebugLocateId = target_id;
    g.DebugLocateFrames = 2;
    g.DebugBreakInLocateId = false;
}

void Az::DebugLocateItemOnHover(AzID target_id)
{
    if (target_id == 0
        || !IsItemHovered(AzHoveredFlags_AllowWhenBlockedByActiveItem
                          | AzHoveredFlags_AllowWhenBlockedByPopup))
        return;
    AzContext& g = *GAz;
    DebugLocateItem(target_id);
    GetForegroundDrawList(g.CurrentWindow)
            ->AddRect(g.LastItemData.Rect.Min - AzVec2(3.0f, 3.0f),
                      g.LastItemData.Rect.Max + AzVec2(3.0f, 3.0f),
                      DEBUG_LOCATE_ITEM_COLOR);

    if (g.IO.ConfigDebugIsDebuggerPresent && g.MouseStationaryTimer > 1.0f)
    {
        DebugBreakButtonTooltip(false, "in ItemAdd()");
        if (IsKeyChordPressed(g.DebugBreakKeyChord))
            g.DebugBreakInLocateId = true;
    }
}

void Az::DebugLocateItemResolveWithLastItem()
{
    AzContext& g = *GAz;

    if (g.DebugBreakInLocateId)
        AZ_DEBUG_BREAK();

    AzLastItemData item_data = g.LastItemData;
    g.DebugLocateId = 0;
    AzDrawList* draw_list = GetForegroundDrawList(g.CurrentWindow);
    AzRect r = item_data.Rect;
    r.Expand(3.0f);
    AzVec2 p1 = g.IO.MousePos;
    AzVec2 p2 = AzVec2((p1.x < r.Min.x)   ? r.Min.x
                       : (p1.x > r.Max.x) ? r.Max.x
                                          : p1.x,
                       (p1.y < r.Min.y)   ? r.Min.y
                       : (p1.y > r.Max.y) ? r.Max.y
                                          : p1.y);
    draw_list->AddRect(r.Min, r.Max, DEBUG_LOCATE_ITEM_COLOR);
    draw_list->AddLine(p1, p2, DEBUG_LOCATE_ITEM_COLOR);
}

void Az::DebugStartItemPicker()
{
    AzContext& g = *GAz;
    g.DebugItemPickerActive = true;
}

void Az::UpdateDebugToolItemPicker()
{
    AzContext& g = *GAz;
    g.DebugItemPickerBreakId = 0;
    if (!g.DebugItemPickerActive)
        return;

    const AzID hovered_id = g.HoveredIdPreviousFrame;
    SetMouseCursor(AzMouseCursor_Hand);
    if (IsKeyPressed(AzKey_Escape))
        g.DebugItemPickerActive = false;
    const bool change_mapping = g.IO.KeyMods == (AzMod_Ctrl | AzMod_Shift);
    if (!change_mapping && IsMouseClicked(g.DebugItemPickerMouseButton)
        && hovered_id)
    {
        g.DebugItemPickerBreakId = hovered_id;
        g.DebugItemPickerActive = false;
    }
    for (int mouse_button = 0; mouse_button < 3; mouse_button++)
        if (change_mapping && IsMouseClicked(mouse_button))
            g.DebugItemPickerMouseButton = (AzU8)mouse_button;
    SetNextWindowBgAlpha(0.70f);
    if (!BeginTooltip())
        return;
    Text("HoveredId: 0x%08X", hovered_id);
    Text("Press ESC to abort picking.");
    const char* mouse_button_names[] = {"Left", "Right", "Middle"};
    if (change_mapping)
        Text("Remap w/ Ctrl+Shift: click anywhere to select new mouse button.");
    else
        TextColored(
                GetStyleColorVec4(hovered_id ? AzCol_Text : AzCol_TextDisabled),
                "Click %s Button to break in debugger! (remap w/ Ctrl+Shift)",
                mouse_button_names[g.DebugItemPickerMouseButton]);
    EndTooltip();
}

void Az::UpdateDebugToolStackQueries()
{
    AzContext& g = *GAz;
    AzIDStackTool* tool = &g.DebugIDStackTool;

    g.DebugHookIdInfo = 0;
    if (g.FrameCount != tool->LastActiveFrame + 1)
        return;

    const AzID query_id = g.HoveredIdPreviousFrame ? g.HoveredIdPreviousFrame
                                                   : g.ActiveId;
    if (tool->QueryId != query_id)
    {
        tool->QueryId = query_id;
        tool->StackLevel = -1;
        tool->Results.resize(0);
    }
    if (query_id == 0)
        return;

    int stack_level = tool->StackLevel;
    if (stack_level >= 0 && stack_level < tool->Results.Size)
        if (tool->Results[stack_level].QuerySuccess
            || tool->Results[stack_level].QueryFrameCount > 2)
            tool->StackLevel++;

    stack_level = tool->StackLevel;
    if (stack_level == -1)
        g.DebugHookIdInfo = query_id;
    if (stack_level >= 0 && stack_level < tool->Results.Size)
    {
        g.DebugHookIdInfo = tool->Results[stack_level].ID;
        tool->Results[stack_level].QueryFrameCount++;
    }
}

void Az::DebugHookIdInfo(AzID id,
                         AzDataType data_type,
                         const void* data_id,
                         const void* data_id_end)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzIDStackTool* tool = &g.DebugIDStackTool;

    if (tool->StackLevel == -1)
    {
        tool->StackLevel++;
        tool->Results.resize(window->IDStack.Size + 1, AzStackLevelInfo());
        for (int n = 0; n < window->IDStack.Size + 1; n++)
            tool->Results[n].ID = (n < window->IDStack.Size)
                                          ? window->IDStack[n]
                                          : id;
        return;
    }

    AZ_ASSERT(tool->StackLevel >= 0);
    if (tool->StackLevel != window->IDStack.Size)
        return;
    AzStackLevelInfo* info = &tool->Results[tool->StackLevel];
    AZ_ASSERT(info->ID == id && info->QueryFrameCount > 0);

    switch (data_type)
    {
    case AzDataType_S32:
        AzFormatString(info->Desc,
                       AZ_ARRAYSIZE(info->Desc),
                       "%d",
                       (int)(intptr_t)data_id);
        break;
    case AzDataType_String:
        AzFormatString(info->Desc,
                       AZ_ARRAYSIZE(info->Desc),
                       "%.*s",
                       data_id_end ? (int)((const char*)data_id_end
                                           - (const char*)data_id)
                                   : (int)AzStrlen((const char*)data_id),
                       (const char*)data_id);
        break;
    case AzDataType_Pointer:
        AzFormatString(info->Desc,
                       AZ_ARRAYSIZE(info->Desc),
                       "(void*)0x%p",
                       data_id);
        break;
    case AzDataType_ID:
        if (info->Desc[0] != 0)

            return;
        AzFormatString(info->Desc,
                       AZ_ARRAYSIZE(info->Desc),
                       "0x%08X [override]",
                       id);
        break;
    default:
        AZ_ASSERT(0);
    }
    info->QuerySuccess = true;
    info->DataType = data_type;
}

static int StackToolFormatLevelInfo(AzIDStackTool* tool,
                                    int n,
                                    bool format_for_ui,
                                    char* buf,
                                    size_t buf_size)
{
    AzStackLevelInfo* info = &tool->Results[n];
    AzWindow* window = (info->Desc[0] == 0 && n == 0)
                               ? Az::FindWindowByID(info->ID)
                               : NULL;
    if (window)

        return AzFormatString(buf,
                              buf_size,
                              format_for_ui ? "\"%s\" [window]" : "%s",
                              window->Name);
    if (info->QuerySuccess)

        return AzFormatString(buf,
                              buf_size,
                              (format_for_ui
                               && info->DataType == AzDataType_String)
                                      ? "\"%s\""
                                      : "%s",
                              info->Desc);
    if (tool->StackLevel < tool->Results.Size)

        return (*buf = 0);
#ifdef AZ_ENABLE_TEST_ENGINE
    if (const char* label = AzTestEngine_FindItemDebugLabel(GAz, info->ID))
        return AzFormatString(buf,
                              buf_size,
                              format_for_ui ? "??? \"%s\"" : "%s",
                              label);
#endif
    return AzFormatString(buf, buf_size, "???");
}

void Az::ShowIDStackToolWindow(bool* p_open)
{
    AzContext& g = *GAz;
    if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSize) == 0)
        SetNextWindowSize(AzVec2(0.0f, GetFontSize() * 8.0f),
                          AzCond_FirstUseEver);
    if (!Begin("Dear Az ID Stack Tool", p_open)
        || GetCurrentWindow()->BeginCount > 1)
    {
        End();
        return;
    }

    AzIDStackTool* tool = &g.DebugIDStackTool;

    tool->ResultPathBuf.resize(0);
    for (int stack_n = 0; stack_n < tool->Results.Size; stack_n++)
    {
        char level_desc[256];
        StackToolFormatLevelInfo(tool,
                                 stack_n,
                                 false,
                                 level_desc,
                                 AZ_ARRAYSIZE(level_desc));
        tool->ResultPathBuf.append(stack_n == 0 ? "//" : "/");
        for (int n = 0; level_desc[n]; n++)
        {
            if (level_desc[n] == '/')
                tool->ResultPathBuf.append("\\");
            tool->ResultPathBuf.append(level_desc + n, level_desc + n + 1);
        }
    }
    Text("0x%08X", tool->QueryId);
    SameLine();
    MetricsHelpMarker(
            "Hover an item with the mouse to display elements of the ID Stack "
            "leading to the item's final ID.\nEach level of the stack "
            "correspond to a PushID() call.\nAll levels of the stack are "
            "hashed together to make the final ID of a widget (ID displayed at "
            "the bottom level of the stack).\nRead FAQ entry about the ID "
            "stack for details.");

    const float time_since_copy = (float)g.Time - tool->CopyToClipboardLastTime;
    SameLine();
    PushStyleVarY(AzStyleVar_FramePadding, 0.0f);
    Checkbox("Ctrl+C: copy path", &tool->CopyToClipboardOnCtrlC);
    PopStyleVar();
    SameLine();
    TextColored((time_since_copy >= 0.0f && time_since_copy < 0.75f
                 && AzFmod(time_since_copy, 0.25f) < 0.25f * 0.5f)
                        ? AzVec4(1.f, 1.f, 0.3f, 1.f)
                        : AzVec4(),
                "*COPIED*");
    if (tool->CopyToClipboardOnCtrlC
        && Shortcut(AzMod_Ctrl | AzKey_C,
                    AzInputFlags_RouteGlobal | AzInputFlags_RouteOverFocused))
    {
        tool->CopyToClipboardLastTime = (float)g.Time;
        SetClipboardText(tool->ResultPathBuf.c_str());
    }

    Text("- Path \"%s\"", tool->ResultPathBuf.c_str());
#ifdef AZ_ENABLE_TEST_ENGINE
    Text("- Label \"%s\"",
         tool->QueryId ? AzTestEngine_FindItemDebugLabel(&g, tool->QueryId)
                       : "");
#endif

    Separator();

    tool->LastActiveFrame = g.FrameCount;
    if (tool->Results.Size > 0
        && BeginTable("##table", 3, AzTableFlags_Borders))
    {
        const float id_width = CalcTextSize("0xDDDDDDDD").x;
        TableSetupColumn("Seed", AzTableColumnFlags_WidthFixed, id_width);
        TableSetupColumn("PushID", AzTableColumnFlags_WidthStretch);
        TableSetupColumn("Result", AzTableColumnFlags_WidthFixed, id_width);
        TableHeadersRow();
        for (int n = 0; n < tool->Results.Size; n++)
        {
            AzStackLevelInfo* info = &tool->Results[n];
            TableNextColumn();
            Text("0x%08X", (n > 0) ? tool->Results[n - 1].ID : 0);
            TableNextColumn();
            StackToolFormatLevelInfo(tool,
                                     n,
                                     true,
                                     g.TempBuffer.Data,
                                     g.TempBuffer.Size);
            TextUnformatted(g.TempBuffer.Data);
            TableNextColumn();
            Text("0x%08X", info->ID);
            if (n == tool->Results.Size - 1)
                TableSetBgColor(AzTableBgTarget_CellBg,
                                GetColorU32(AzCol_Header));
        }
        EndTable();
    }
    End();
}

#else

void Az::ShowMetricsWindow(bool*)
{}
void Az::ShowFontAtlas(AzFontAtlas*)
{}
void Az::DebugNodeColumns(AzOldColumns*)
{}
void Az::DebugNodeDrawList(AzWindow*,
                           AzViewportP*,
                           const AzDrawList*,
                           const char*)
{}
void Az::DebugNodeDrawCmdShowMeshAndBoundingBox(AzDrawList*,
                                                const AzDrawList*,
                                                const AzDrawCmd*,
                                                bool,
                                                bool)
{}
void Az::DebugNodeFont(AzFont*)
{}
void Az::DebugNodeFontGlyphesForSrcMask(AzFont*, AzFontBaked*, int)
{}
void Az::DebugNodeStorage(AzStorage*, const char*)
{}
void Az::DebugNodeTabBar(AzTabBar*, const char*)
{}
void Az::DebugNodeWindow(AzWindow*, const char*)
{}
void Az::DebugNodeWindowSettings(AzWindowSettings*)
{}
void Az::DebugNodeWindowsList(AzVector<AzWindow*>*, const char*)
{}
void Az::DebugNodeViewport(AzViewportP*)
{}

void Az::ShowDebugLogWindow(bool*)
{}
void Az::ShowIDStackToolWindow(bool*)
{}
void Az::DebugStartItemPicker()
{}
void Az::DebugHookIdInfo(AzID, AzDataType, const void*, const void*)
{}

#endif

#ifdef AZ_INCLUDE_AZ_USER_INL
#include "imgui_user.inl"
#endif

#endif
