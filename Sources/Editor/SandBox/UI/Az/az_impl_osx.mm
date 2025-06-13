#import "imgui.h"
#ifndef AZ_DISABLE
#import "imgui_impl_osx.h"
#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#import <GameController/GameController.h>
#import <time.h>

#define APPLE_HAS_BUTTON_OPTIONS (__IPHONE_OS_VERSION_MIN_REQUIRED >= 130000 || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500 || __TV_OS_VERSION_MIN_REQUIRED >= 130000)
#define APPLE_HAS_CONTROLLER     (__IPHONE_OS_VERSION_MIN_REQUIRED >= 140000 || __MAC_OS_X_VERSION_MIN_REQUIRED >= 110000 || __TV_OS_VERSION_MIN_REQUIRED >= 140000)
#define APPLE_HAS_THUMBSTICKS    (__IPHONE_OS_VERSION_MIN_REQUIRED >= 120100 || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101401 || __TV_OS_VERSION_MIN_REQUIRED >= 120100)

@class AzObserver;
@class KeyEventResponder;

struct Az_AzplOSX_Data
{
    CFTimeInterval              Time;
    NSCursor*                   MouseCursors[AzMouseCursor_COUNT];
    bool                        MouseCursorHidden;
    AzObserver*              Observer;
    KeyEventResponder*          KeyEventResponder;
    NSTextInputContext*         InputContext;
    id                          Monitor;
    NSWindow*                   Window;

    Az_AzplOSX_Data()        { memset((void*)this, 0, sizeof(*this)); }
};

static Az_AzplOSX_Data*      Az_AzplOSX_GetBackendData()      { return (Az_AzplOSX_Data*)Az::GetIO().BackendPlatformUserData; }
static void                     Az_AzplOSX_DestroyBackendData()  { AZ_DELETE(Az_AzplOSX_GetBackendData()); }

static inline CFTimeInterval    GetMachAbsoluteTimeInSeconds()      { return (CFTimeInterval)(double)(clock_gettime_nsec_np(CLOCK_UPTAZE_RAW) / 1e9); }

static void Az_AzplOSX_InitMultiViewportSupport();
static void Az_AzplOSX_ShutdownMultiViewportSupport();
static void Az_AzplOSX_UpdateMonitors();
static void Az_AzplOSX_AddTrackingArea(NSView* _Nonnull view);
static bool Az_AzplOSX_HandleEvent(NSEvent* event, NSView* view);

@interface NSCursor()
+ (id)_windowResizeNorthWestSouthEastCursor;
+ (id)_windowResizeNorthEastSouthWestCursor;
+ (id)_windowResizeNorthSouthCursor;
+ (id)_windowResizeEastWestCursor;
@end

@interface KeyEventResponder: NSView<NSTextInputClient>
@end

@implementation KeyEventResponder
{
    float _posX;
    float _posY;
    NSRect _imeRect;
}

#pragma mark - Public

- (void)setAzePosX:(float)posX imePosY:(float)posY
{
    _posX = posX;
    _posY = posY;
}

- (void)updateAzePosWithView:(NSView *)view
{
    NSWindow* window = view.window;
    if (!window)
        return;

    AzIO& io = Az::GetIO();
    if (io.ConfigFlags & AzConfigFlags_ViewportsEnable)
    {
        NSRect frame = window.frame;
        NSRect contentRect = window.contentLayoutRect;
        if (window.styleMask & NSWindowStyleMaskFullSizeContentView) 
            contentRect = frame;

        NSRect firstScreenFrame = NSScreen.screens[0].frame;
        _imeRect = NSMakeRect(_posX, _posY, 0, 0);
        _imeRect.origin.y = firstScreenFrame.size.height - _imeRect.size.height - _imeRect.origin.y; 
    }
    else
    {
        NSRect contentRect = [window contentRectForFrameRect:window.frame];
        NSRect rect = NSMakeRect(_posX, contentRect.size.height - _posY, 0, 0);
        _imeRect = [window convertRectToScreen:rect];
    }
}

- (void)viewDidMoveToWindow
{

    [self.window makeFirstResponder:self];
}

- (void)keyDown:(NSEvent*)event
{
    if (!Az_AzplOSX_HandleEvent(event, self))
        [super keyDown:event];

    [self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent*)event
{
    if (!Az_AzplOSX_HandleEvent(event, self))
        [super keyUp:event];
}

- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange
{
    AzIO& io = Az::GetIO();

    NSString* characters;
    if ([aString isKindOfClass:[NSAttributedString class]])
        characters = [aString string];
    else
        characters = (NSString*)aString;

    io.AddInputCharactersUTF8(characters.UTF8String);
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)doCommandBySelector:(SEL)myselector
{
}

- (nullable NSAttributedString*)attributedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
{
    return nil;
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point
{
    return 0;
}

- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
{
    return _imeRect;
}

- (BOOL)hasMarkedText
{
    return NO;
}

- (NSRange)markedRange
{
    return NSMakeRange(NSNotFound, 0);
}

- (NSRange)selectedRange
{
    return NSMakeRange(NSNotFound, 0);
}

- (void)setMarkedText:(nonnull id)string selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange
{
}

- (void)unmarkText
{
}

- (nonnull NSArray<NSAttributedStringKey>*)validAttributesForMarkedText
{
    return @[];
}

@end

@interface AzObserver : NSObject

- (void)onApplicationBecomeActive:(NSNotification*)aNotification;
- (void)onApplicationBecomeInactive:(NSNotification*)aNotification;
- (void)displaysDidChange:(NSNotification*)aNotification;

@end

@implementation AzObserver

- (void)onApplicationBecomeActive:(NSNotification*)aNotification
{
    AzIO& io = Az::GetIO();
    io.AddFocusEvent(true);
}

- (void)onApplicationBecomeInactive:(NSNotification*)aNotification
{
    AzIO& io = Az::GetIO();
    io.AddFocusEvent(false);
}

- (void)displaysDidChange:(NSNotification*)aNotification
{
    Az_AzplOSX_UpdateMonitors();
}

@end

AzKey Az_AzplOSX_KeyCodeToAzKey(int key_code);
AzKey Az_AzplOSX_KeyCodeToAzKey(int key_code)
{
    switch (key_code)
    {
        case kVK_ANSI_A: return AzKey_A;
        case kVK_ANSI_S: return AzKey_S;
        case kVK_ANSI_D: return AzKey_D;
        case kVK_ANSI_F: return AzKey_F;
        case kVK_ANSI_H: return AzKey_H;
        case kVK_ANSI_G: return AzKey_G;
        case kVK_ANSI_Z: return AzKey_Z;
        case kVK_ANSI_X: return AzKey_X;
        case kVK_ANSI_C: return AzKey_C;
        case kVK_ANSI_V: return AzKey_V;
        case kVK_ANSI_B: return AzKey_B;
        case kVK_ANSI_Q: return AzKey_Q;
        case kVK_ANSI_W: return AzKey_W;
        case kVK_ANSI_E: return AzKey_E;
        case kVK_ANSI_R: return AzKey_R;
        case kVK_ANSI_Y: return AzKey_Y;
        case kVK_ANSI_T: return AzKey_T;
        case kVK_ANSI_1: return AzKey_1;
        case kVK_ANSI_2: return AzKey_2;
        case kVK_ANSI_3: return AzKey_3;
        case kVK_ANSI_4: return AzKey_4;
        case kVK_ANSI_6: return AzKey_6;
        case kVK_ANSI_5: return AzKey_5;
        case kVK_ANSI_Equal: return AzKey_Equal;
        case kVK_ANSI_9: return AzKey_9;
        case kVK_ANSI_7: return AzKey_7;
        case kVK_ANSI_Minus: return AzKey_Minus;
        case kVK_ANSI_8: return AzKey_8;
        case kVK_ANSI_0: return AzKey_0;
        case kVK_ANSI_RightBracket: return AzKey_RightBracket;
        case kVK_ANSI_O: return AzKey_O;
        case kVK_ANSI_U: return AzKey_U;
        case kVK_ANSI_LeftBracket: return AzKey_LeftBracket;
        case kVK_ANSI_I: return AzKey_I;
        case kVK_ANSI_P: return AzKey_P;
        case kVK_ANSI_L: return AzKey_L;
        case kVK_ANSI_J: return AzKey_J;
        case kVK_ANSI_Quote: return AzKey_Apostrophe;
        case kVK_ANSI_K: return AzKey_K;
        case kVK_ANSI_Semicolon: return AzKey_Semicolon;
        case kVK_ANSI_Backslash: return AzKey_Backslash;
        case kVK_ANSI_Comma: return AzKey_Comma;
        case kVK_ANSI_Slash: return AzKey_Slash;
        case kVK_ANSI_N: return AzKey_N;
        case kVK_ANSI_M: return AzKey_M;
        case kVK_ANSI_Period: return AzKey_Period;
        case kVK_ANSI_Grave: return AzKey_GraveAccent;
        case kVK_ANSI_KeypadDecimal: return AzKey_KeypadDecimal;
        case kVK_ANSI_KeypadMultiply: return AzKey_KeypadMultiply;
        case kVK_ANSI_KeypadPlus: return AzKey_KeypadAdd;
        case kVK_ANSI_KeypadClear: return AzKey_NumLock;
        case kVK_ANSI_KeypadDivide: return AzKey_KeypadDivide;
        case kVK_ANSI_KeypadEnter: return AzKey_KeypadEnter;
        case kVK_ANSI_KeypadMinus: return AzKey_KeypadSubtract;
        case kVK_ANSI_KeypadEquals: return AzKey_KeypadEqual;
        case kVK_ANSI_Keypad0: return AzKey_Keypad0;
        case kVK_ANSI_Keypad1: return AzKey_Keypad1;
        case kVK_ANSI_Keypad2: return AzKey_Keypad2;
        case kVK_ANSI_Keypad3: return AzKey_Keypad3;
        case kVK_ANSI_Keypad4: return AzKey_Keypad4;
        case kVK_ANSI_Keypad5: return AzKey_Keypad5;
        case kVK_ANSI_Keypad6: return AzKey_Keypad6;
        case kVK_ANSI_Keypad7: return AzKey_Keypad7;
        case kVK_ANSI_Keypad8: return AzKey_Keypad8;
        case kVK_ANSI_Keypad9: return AzKey_Keypad9;
        case kVK_Return: return AzKey_Enter;
        case kVK_Tab: return AzKey_Tab;
        case kVK_Space: return AzKey_Space;
        case kVK_Delete: return AzKey_Backspace;
        case kVK_Escape: return AzKey_Escape;
        case kVK_CapsLock: return AzKey_CapsLock;
        case kVK_Control: return AzKey_LeftCtrl;
        case kVK_Shift: return AzKey_LeftShift;
        case kVK_Option: return AzKey_LeftAlt;
        case kVK_Command: return AzKey_LeftSuper;
        case kVK_RightControl: return AzKey_RightCtrl;
        case kVK_RightShift: return AzKey_RightShift;
        case kVK_RightOption: return AzKey_RightAlt;
        case kVK_RightCommand: return AzKey_RightSuper;

        case kVK_F1: return AzKey_F1;
        case kVK_F2: return AzKey_F2;
        case kVK_F3: return AzKey_F3;
        case kVK_F4: return AzKey_F4;
        case kVK_F5: return AzKey_F5;
        case kVK_F6: return AzKey_F6;
        case kVK_F7: return AzKey_F7;
        case kVK_F8: return AzKey_F8;
        case kVK_F9: return AzKey_F9;
        case kVK_F10: return AzKey_F10;
        case kVK_F11: return AzKey_F11;
        case kVK_F12: return AzKey_F12;
        case kVK_F13: return AzKey_F13;
        case kVK_F14: return AzKey_F14;
        case kVK_F15: return AzKey_F15;
        case kVK_F16: return AzKey_F16;
        case kVK_F17: return AzKey_F17;
        case kVK_F18: return AzKey_F18;
        case kVK_F19: return AzKey_F19;
        case kVK_F20: return AzKey_F20;
        case 0x6E: return AzKey_Menu;
        case kVK_Help: return AzKey_Insert;
        case kVK_Home: return AzKey_Home;
        case kVK_PageUp: return AzKey_PageUp;
        case kVK_ForwardDelete: return AzKey_Delete;
        case kVK_End: return AzKey_End;
        case kVK_PageDown: return AzKey_PageDown;
        case kVK_LeftArrow: return AzKey_LeftArrow;
        case kVK_RightArrow: return AzKey_RightArrow;
        case kVK_DownArrow: return AzKey_DownArrow;
        case kVK_UpArrow: return AzKey_UpArrow;
        default: return AzKey_None;
    }
}

#ifdef AZ_AZPL_METAL_CPP_EXTENSIONS

AZ_AZPL_API bool Az_AzplOSX_Init(void* _Nonnull view) {
    return Az_AzplOSX_Init((__bridge NSView*)(view));
}

AZ_AZPL_API void Az_AzplOSX_NewFrame(void* _Nullable view) {
    return Az_AzplOSX_NewFrame((__bridge NSView*)(view));
}

#endif

bool Az_AzplOSX_Init(NSView* view)
{
    AzIO& io = Az::GetIO();
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    AZ_CHECKVERSION();
    AZ_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    Az_AzplOSX_Data* bd = AZ_NEW(Az_AzplOSX_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_osx";
    io.BackendFlags |= AzBackendFlags_HasMouseCursors;           

    io.BackendFlags |= AzBackendFlags_PlatformHasViewports;      

    bd->Observer = [AzObserver new];
    bd->Window = view.window ?: NSApp.orderedWindows.firstObject;
    AzViewport* main_viewport = Az::GetMainViewport();
    main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw = (__bridge_retained void*)bd->Window;
    Az_AzplOSX_UpdateMonitors();
    Az_AzplOSX_InitMultiViewportSupport();

    bd->MouseCursorHidden = false;
    bd->MouseCursors[AzMouseCursor_Arrow] = [NSCursor arrowCursor];
    bd->MouseCursors[AzMouseCursor_TextInput] = [NSCursor IBeamCursor];
    bd->MouseCursors[AzMouseCursor_ResizeAll] = [NSCursor closedHandCursor];
    bd->MouseCursors[AzMouseCursor_ResizeNS] = [NSCursor respondsToSelector:@selector(_windowResizeNorthSouthCursor)] ? [NSCursor _windowResizeNorthSouthCursor] : [NSCursor resizeUpDownCursor];
    bd->MouseCursors[AzMouseCursor_ResizeEW] = [NSCursor respondsToSelector:@selector(_windowResizeEastWestCursor)] ? [NSCursor _windowResizeEastWestCursor] : [NSCursor resizeLeftRightCursor];
    bd->MouseCursors[AzMouseCursor_ResizeNESW] = [NSCursor respondsToSelector:@selector(_windowResizeNorthEastSouthWestCursor)] ? [NSCursor _windowResizeNorthEastSouthWestCursor] : [NSCursor closedHandCursor];
    bd->MouseCursors[AzMouseCursor_ResizeNWSE] = [NSCursor respondsToSelector:@selector(_windowResizeNorthWestSouthEastCursor)] ? [NSCursor _windowResizeNorthWestSouthEastCursor] : [NSCursor closedHandCursor];
    bd->MouseCursors[AzMouseCursor_Hand] = [NSCursor pointingHandCursor];
    bd->MouseCursors[AzMouseCursor_NotAllowed] = [NSCursor operationNotAllowedCursor];

    platform_io.Platform_SetClipboardTextFn = [](AzContext*, const char* str) -> void
    {
        NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
        [pasteboard declareTypes:[NSArray arrayWithObject:NSPasteboardTypeString] owner:nil];
        [pasteboard setString:[NSString stringWithUTF8String:str] forType:NSPasteboardTypeString];
    };

    platform_io.Platform_GetClipboardTextFn = [](AzContext*) -> const char*
    {
        NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
        NSString* available = [pasteboard availableTypeFromArray: [NSArray arrayWithObject:NSPasteboardTypeString]];
        if (![available isEqualToString:NSPasteboardTypeString])
            return nullptr;

        NSString* string = [pasteboard stringForType:NSPasteboardTypeString];
        if (string == nil)
            return nullptr;

        const char* string_c = (const char*)[string UTF8String];
        size_t string_len = strlen(string_c);
        static AzVector<char> s_clipboard;
        s_clipboard.resize((int)string_len + 1);
        strcpy(s_clipboard.Data, string_c);
        return s_clipboard.Data;
    };

    [[NSNotificationCenter defaultCenter] addObserver:bd->Observer
                                             selector:@selector(onApplicationBecomeActive:)
                                                 name:NSApplicationDidBecomeActiveNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:bd->Observer
                                             selector:@selector(onApplicationBecomeInactive:)
                                                 name:NSApplicationDidResignActiveNotification
                                               object:nil];

    bd->KeyEventResponder = [[KeyEventResponder alloc] initWithFrame:NSZeroRect];
    bd->InputContext = [[NSTextInputContext alloc] initWithClient:bd->KeyEventResponder];
    [view addSubview:bd->KeyEventResponder];
    Az_AzplOSX_AddTrackingArea(view);

    platform_io.Platform_SetAzeDataFn = [](AzContext*, AzViewport*, AzPlatformAzeData* data) -> void
    {
        Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
        if (data->WantVisible)
        {
            [bd->InputContext activate];
        }
        else
        {
            [bd->InputContext discardMarkedText];
            [bd->InputContext invalidateCharacterCoordinates];
            [bd->InputContext deactivate];
        }
        [bd->KeyEventResponder setAzePosX:data->InputPos.x imePosY:data->InputPos.y + data->InputLineHeight];
    };

    return true;
}

void Az_AzplOSX_Shutdown()
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    AZ_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");

    [[NSNotificationCenter defaultCenter] removeObserver:bd->Observer];
    bd->Observer = nullptr;
    if (bd->Monitor != nullptr)
    {
        [NSEvent removeMonitor:bd->Monitor];
        bd->Monitor = nullptr;
    }

    Az_AzplOSX_ShutdownMultiViewportSupport();
    Az_AzplOSX_DestroyBackendData();
    AzIO& io = Az::GetIO();
    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(AzBackendFlags_HasMouseCursors | AzBackendFlags_HasGamepad | AzBackendFlags_PlatformHasViewports);
}

static void Az_AzplOSX_UpdateMouseCursor()
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    AzIO& io = Az::GetIO();
    if (io.ConfigFlags & AzConfigFlags_NoMouseCursorChange)
        return;

    AzMouseCursor imgui_cursor = Az::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == AzMouseCursor_None)
    {

        if (!bd->MouseCursorHidden)
        {
            bd->MouseCursorHidden = true;
            [NSCursor hide];
        }
    }
    else
    {
        NSCursor* desired = bd->MouseCursors[imgui_cursor] ?: bd->MouseCursors[AzMouseCursor_Arrow];

        if (desired != NSCursor.currentCursor)
        {
            [desired set];
        }
        if (bd->MouseCursorHidden)
        {
            bd->MouseCursorHidden = false;
            [NSCursor unhide];
        }
    }
}

static void Az_AzplOSX_UpdateGamepads()
{
    AzIO& io = Az::GetIO();

#if APPLE_HAS_CONTROLLER
    GCController* controller = GCController.current;
#else
    GCController* controller = GCController.controllers.firstObject;
#endif
    if (controller == nil || controller.extendedGamepad == nil)
    {
        io.BackendFlags &= ~AzBackendFlags_HasGamepad;
        return;
    }

    GCExtendedGamepad* gp = controller.extendedGamepad;

    #define AZ_SATURATE(V)                        (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
    #define MAP_BUTTON(KEY_NO, BUTTON_NAME)       { io.AddKeyEvent(KEY_NO, gp.BUTTON_NAME.isPressed); }
    #define MAP_ANALOG(KEY_NO, AXIS_NAME, V0, V1) { float vn = (float)(gp.AXIS_NAME.value - V0) / (float)(V1 - V0); vn = AZ_SATURATE(vn); io.AddKeyAnalogEvent(KEY_NO, vn > 0.1f, vn); }
    const float thumb_dead_zone = 0.0f;

#if APPLE_HAS_BUTTON_OPTIONS
    MAP_BUTTON(AzKey_GamepadBack,            buttonOptions);
#endif
    MAP_BUTTON(AzKey_GamepadFaceLeft,        buttonX);              
    MAP_BUTTON(AzKey_GamepadFaceRight,       buttonB);              
    MAP_BUTTON(AzKey_GamepadFaceUp,          buttonY);              
    MAP_BUTTON(AzKey_GamepadFaceDown,        buttonA);              
    MAP_BUTTON(AzKey_GamepadDpadLeft,        dpad.left);
    MAP_BUTTON(AzKey_GamepadDpadRight,       dpad.right);
    MAP_BUTTON(AzKey_GamepadDpadUp,          dpad.up);
    MAP_BUTTON(AzKey_GamepadDpadDown,        dpad.down);
    MAP_ANALOG(AzKey_GamepadL1,              leftShoulder, 0.0f, 1.0f);
    MAP_ANALOG(AzKey_GamepadR1,              rightShoulder, 0.0f, 1.0f);
    MAP_ANALOG(AzKey_GamepadL2,              leftTrigger,  0.0f, 1.0f);
    MAP_ANALOG(AzKey_GamepadR2,              rightTrigger, 0.0f, 1.0f);
#if APPLE_HAS_THUMBSTICKS
    MAP_BUTTON(AzKey_GamepadL3,              leftThumbstickButton);
    MAP_BUTTON(AzKey_GamepadR3,              rightThumbstickButton);
#endif
    MAP_ANALOG(AzKey_GamepadLStickLeft,      leftThumbstick.xAxis,  -thumb_dead_zone, -1.0f);
    MAP_ANALOG(AzKey_GamepadLStickRight,     leftThumbstick.xAxis,  +thumb_dead_zone, +1.0f);
    MAP_ANALOG(AzKey_GamepadLStickUp,        leftThumbstick.yAxis,  +thumb_dead_zone, +1.0f);
    MAP_ANALOG(AzKey_GamepadLStickDown,      leftThumbstick.yAxis,  -thumb_dead_zone, -1.0f);
    MAP_ANALOG(AzKey_GamepadRStickLeft,      rightThumbstick.xAxis, -thumb_dead_zone, -1.0f);
    MAP_ANALOG(AzKey_GamepadRStickRight,     rightThumbstick.xAxis, +thumb_dead_zone, +1.0f);
    MAP_ANALOG(AzKey_GamepadRStickUp,        rightThumbstick.yAxis, +thumb_dead_zone, +1.0f);
    MAP_ANALOG(AzKey_GamepadRStickDown,      rightThumbstick.yAxis, -thumb_dead_zone, -1.0f);
    #undef MAP_BUTTON
    #undef MAP_ANALOG

    io.BackendFlags |= AzBackendFlags_HasGamepad;
}

static void Az_AzplOSX_UpdateAzePosWithView(NSView* view)
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    AzIO& io = Az::GetIO();
    if (io.WantTextInput)
        [bd->KeyEventResponder updateAzePosWithView:view];
}

void Az_AzplOSX_NewFrame(NSView* view)
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    AZ_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call Az_AzplOSX_Init()?");
    AzIO& io = Az::GetIO();

    if (view)
    {
        const float fb_scale = (float)[view.window backingScaleFactor];
        io.DisplaySize = AzVec2((float)view.bounds.size.width, (float)view.bounds.size.height);
        io.DisplayFramebufferScale = AzVec2(fb_scale, fb_scale);
    }

    if (bd->Time == 0.0)
        bd->Time = GetMachAbsoluteTimeInSeconds();

    double current_time = GetMachAbsoluteTimeInSeconds();
    io.DeltaTime = (float)(current_time - bd->Time);
    bd->Time = current_time;

    Az_AzplOSX_UpdateMouseCursor();
    Az_AzplOSX_UpdateGamepads();
    Az_AzplOSX_UpdateAzePosWithView(view);
}

static AzMouseSource GetMouseSource(NSEvent* event)
{
    switch (event.subtype)
    {
        case NSEventSubtypeTabletPoint:
            return AzMouseSource_Pen;

        case NSEventSubtypeMouseEvent:
        default:
            return AzMouseSource_Mouse;
    }
}

static bool Az_AzplOSX_HandleEvent(NSEvent* event, NSView* view)
{
    AzIO& io = Az::GetIO();

    if (event.type == NSEventTypeLeftMouseDown || event.type == NSEventTypeRightMouseDown || event.type == NSEventTypeOtherMouseDown)
    {
        int button = (int)[event buttonNumber];
        if (button >= 0 && button < AzMouseButton_COUNT)
        {
            io.AddMouseSourceEvent(GetMouseSource(event));
            io.AddMouseButtonEvent(button, true);
        }
        return io.WantCaptureMouse;
    }

    if (event.type == NSEventTypeLeftMouseUp || event.type == NSEventTypeRightMouseUp || event.type == NSEventTypeOtherMouseUp)
    {
        int button = (int)[event buttonNumber];
        if (button >= 0 && button < AzMouseButton_COUNT)
        {
            io.AddMouseSourceEvent(GetMouseSource(event));
            io.AddMouseButtonEvent(button, false);
        }
        return io.WantCaptureMouse;
    }

    if (event.type == NSEventTypeMouseMoved || event.type == NSEventTypeLeftMouseDragged || event.type == NSEventTypeRightMouseDragged || event.type == NSEventTypeOtherMouseDragged)
    {
        NSPoint mousePoint;
        if (io.ConfigFlags & AzConfigFlags_ViewportsEnable)
        {
            mousePoint = NSEvent.mouseLocation;
            mousePoint.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - mousePoint.y; 
        }
        else
        {
            mousePoint = event.locationInWindow;
            if (event.window == nil)
                mousePoint = [[view window] convertPointFromScreen:mousePoint];
            mousePoint = [view convertPoint:mousePoint fromView:nil]; 
            if ([view isFlipped])
                mousePoint = NSMakePoint(mousePoint.x, mousePoint.y);
            else
                mousePoint = NSMakePoint(mousePoint.x, view.bounds.size.height - mousePoint.y);
        }
        io.AddMouseSourceEvent(GetMouseSource(event));
        io.AddMousePosEvent((float)mousePoint.x, (float)mousePoint.y);
        return io.WantCaptureMouse;
    }

    if (event.type == NSEventTypeScrollWheel)
    {

        if (event.phase == NSEventPhaseCancelled)
            return false;

        double wheel_dx = 0.0;
        double wheel_dy = 0.0;

        #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
        if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
        {
            wheel_dx = [event scrollingDeltaX];
            wheel_dy = [event scrollingDeltaY];
            if ([event hasPreciseScrollingDeltas])
            {
                wheel_dx *= 0.01;
                wheel_dy *= 0.01;
            }
        }
        else
        #endif 
        {
            wheel_dx = [event deltaX] * 0.1;
            wheel_dy = [event deltaY] * 0.1;
        }
        if (wheel_dx != 0.0 || wheel_dy != 0.0)
            io.AddMouseWheelEvent((float)wheel_dx, (float)wheel_dy);

        return io.WantCaptureMouse;
    }

    if (event.type == NSEventTypeKeyDown || event.type == NSEventTypeKeyUp)
    {
        if ([event isARepeat])
            return io.WantCaptureKeyboard;

        int key_code = (int)[event keyCode];
        AzKey key = Az_AzplOSX_KeyCodeToAzKey(key_code);
        io.AddKeyEvent(key, event.type == NSEventTypeKeyDown);
        io.SetKeyEventNativeData(key, key_code, -1); 

        return io.WantCaptureKeyboard;
    }

    if (event.type == NSEventTypeFlagsChanged)
    {
        unsigned short key_code = [event keyCode];
        NSEventModifierFlags modifier_flags = [event modifierFlags];

        io.AddKeyEvent(AzMod_Shift, (modifier_flags & NSEventModifierFlagShift)   != 0);
        io.AddKeyEvent(AzMod_Ctrl,  (modifier_flags & NSEventModifierFlagControl) != 0);
        io.AddKeyEvent(AzMod_Alt,   (modifier_flags & NSEventModifierFlagOption)  != 0);
        io.AddKeyEvent(AzMod_Super, (modifier_flags & NSEventModifierFlagCommand) != 0);

        AzKey key = Az_AzplOSX_KeyCodeToAzKey(key_code);
        if (key != AzKey_None)
        {

            NSEventModifierFlags mask = 0;
            switch (key)
            {
                case AzKey_LeftCtrl:   mask = 0x0001; break;
                case AzKey_RightCtrl:  mask = 0x2000; break;
                case AzKey_LeftShift:  mask = 0x0002; break;
                case AzKey_RightShift: mask = 0x0004; break;
                case AzKey_LeftSuper:  mask = 0x0008; break;
                case AzKey_RightSuper: mask = 0x0010; break;
                case AzKey_LeftAlt:    mask = 0x0020; break;
                case AzKey_RightAlt:   mask = 0x0040; break;
                default:
                    return io.WantCaptureKeyboard;
            }
            io.AddKeyEvent(key, (modifier_flags & mask) != 0);
            io.SetKeyEventNativeData(key, key_code, -1); 
        }

        return io.WantCaptureKeyboard;
    }

    return false;
}

static void Az_AzplOSX_AddTrackingArea(NSView* _Nonnull view)
{

    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    if (bd->Monitor)
        return;
    NSEventMask eventMask = 0;
    eventMask |= NSEventMaskMouseMoved | NSEventMaskScrollWheel;
    eventMask |= NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp | NSEventMaskLeftMouseDragged;
    eventMask |= NSEventMaskRightMouseDown | NSEventMaskRightMouseUp | NSEventMaskRightMouseDragged;
    eventMask |= NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp | NSEventMaskOtherMouseDragged;
    eventMask |= NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged;
    bd->Monitor = [NSEvent addLocalMonitorForEventsMatchingMask:eventMask
                                                        handler:^NSEvent* _Nullable(NSEvent* event)
    {
        Az_AzplOSX_HandleEvent(event, view);
        return event;
    }];
}

struct Az_AzplOSX_ViewportData
{
    NSWindow*        Window;
    bool             WindowOwned;

    Az_AzplOSX_ViewportData()  { WindowOwned = false; }
    ~Az_AzplOSX_ViewportData() { AZ_ASSERT(Window == nil); }
};

@interface Az_AzplOSX_Window: NSWindow
@end

@implementation Az_AzplOSX_Window

- (BOOL)canBecomeKeyWindow
{
    return YES;
}

@end

static void ConvertNSRect(NSRect* r)
{
    NSRect firstScreenFrame = NSScreen.screens[0].frame;
    AZ_ASSERT(firstScreenFrame.origin.x == 0 && firstScreenFrame.origin.y == 0);
    r->origin.y = firstScreenFrame.size.height - r->origin.y - r->size.height;
}

static void Az_AzplOSX_CreateWindow(AzViewport* viewport)
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    Az_AzplOSX_ViewportData* vd = AZ_NEW(Az_AzplOSX_ViewportData)();
    viewport->PlatformUserData = vd;

    NSScreen* screen = bd->Window.screen;
    NSRect rect = NSMakeRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);
    ConvertNSRect(&rect);

    NSWindowStyleMask styleMask = 0;
    if (viewport->Flags & AzViewportFlags_NoDecoration)
        styleMask |= NSWindowStyleMaskBorderless;
    else
        styleMask |= NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;

    NSWindow* window = [[Az_AzplOSX_Window alloc] initWithContentRect:rect
                                                               styleMask:styleMask
                                                                 backing:NSBackingStoreBuffered
                                                                   defer:YES
                                                                  screen:screen];
    if (viewport->Flags & AzViewportFlags_TopMost)
        [window setLevel:NSFloatingWindowLevel];

    window.title = @"Untitled";
    window.opaque = YES;

    KeyEventResponder* view = [[KeyEventResponder alloc] initWithFrame:rect];
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6 && ceil(NSAppKitVersionNumber) < NSAppKitVersionNumber10_15)
        [view setWantsBestResolutionOpenGLSurface:YES];

    window.contentView = view;

    vd->Window = window;
    vd->WindowOwned = true;
    viewport->PlatformRequestResize = false;
    viewport->PlatformHandle = viewport->PlatformHandleRaw = (__bridge_retained void*)window;
}

static void Az_AzplOSX_DestroyWindow(AzViewport* viewport)
{
    NSWindow* window = (__bridge_transfer NSWindow*)viewport->PlatformHandleRaw;
    window = nil;

    if (Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData)
    {
        NSWindow* window = vd->Window;
        if (window != nil && vd->WindowOwned)
        {
            window.contentView = nil;
            window.contentViewController = nil;
            [window orderOut:nil];
        }
        vd->Window = nil;
        AZ_DELETE(vd);
    }
    viewport->PlatformUserData = viewport->PlatformHandle = viewport->PlatformHandleRaw = nullptr;
}

static void Az_AzplOSX_ShowWindow(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    if (viewport->Flags & AzViewportFlags_NoFocusOnAppearing)
        [vd->Window orderFront:nil];
    else
        [vd->Window makeKeyAndOrderFront:nil];

    [vd->Window setIsVisible:YES];
}

static AzVec2 Az_AzplOSX_GetWindowPos(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    NSWindow* window = vd->Window;
    NSRect frame = window.frame;
    NSRect contentRect = window.contentLayoutRect;
    if (window.styleMask & NSWindowStyleMaskFullSizeContentView) 
        contentRect = frame;

    NSRect firstScreenFrame = NSScreen.screens[0].frame;
    return AzVec2(frame.origin.x, firstScreenFrame.size.height - frame.origin.y - contentRect.size.height);
}

static void Az_AzplOSX_SetWindowPos(AzViewport* viewport, AzVec2 pos)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    NSWindow* window = vd->Window;
    NSSize size = window.frame.size;

    NSRect r = NSMakeRect(pos.x, pos.y, size.width, size.height);
    ConvertNSRect(&r);
    [window setFrameOrigin:r.origin];
}

static AzVec2 Az_AzplOSX_GetWindowSize(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    NSWindow* window = vd->Window;
    NSSize size = window.contentLayoutRect.size;
    return AzVec2(size.width, size.height);
}

static void Az_AzplOSX_SetWindowSize(AzViewport* viewport, AzVec2 size)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    NSWindow* window = vd->Window;
    NSRect rect = window.frame;
    rect.origin.y -= (size.y - rect.size.height);
    rect.size.width = size.x;
    rect.size.height = size.y;
    [window setFrame:rect display:YES];
}

static AzVec2 Az_AzplOSX_GetWindowFramebufferScale(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    NSWindow* window = vd->Window;
    const float fb_scale = (float)[window backingScaleFactor];
    return AzVec2(fb_scale, fb_scale);
}

static void Az_AzplOSX_SetWindowFocus(AzViewport* viewport)
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);
    [vd->Window makeKeyAndOrderFront:bd->Window];
}

static bool Az_AzplOSX_GetWindowFocus(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    return vd->Window.isKeyWindow;
}

static bool Az_AzplOSX_GetWindowMinimized(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    return vd->Window.isMiniaturized;
}

static void Az_AzplOSX_SetWindowTitle(AzViewport* viewport, const char* title)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    vd->Window.title = [NSString stringWithUTF8String:title];
}

static void Az_AzplOSX_SetWindowAlpha(AzViewport* viewport, float alpha)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);
    AZ_ASSERT(alpha >= 0.0f && alpha <= 1.0f);

    vd->Window.alphaValue = alpha;
}

static float Az_AzplOSX_GetWindowDpiScale(AzViewport* viewport)
{
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)viewport->PlatformUserData;
    AZ_ASSERT(vd->Window != 0);

    return vd->Window.backingScaleFactor;
}

static void Az_AzplOSX_UpdateMonitors()
{
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Monitors.resize(0);

    NSRect firstScreenFrame = NSScreen.screens[0].frame;
    AZ_ASSERT(firstScreenFrame.origin.x == 0 && firstScreenFrame.origin.y == 0);

    for (NSScreen* screen in NSScreen.screens)
    {
        NSRect frame = screen.frame;
        NSRect visibleFrame = screen.visibleFrame;
        ConvertNSRect(&frame);
        ConvertNSRect(&visibleFrame);

        AzPlatformMonitor imgui_monitor;
        imgui_monitor.MainPos = AzVec2(frame.origin.x, frame.origin.y);
        imgui_monitor.MainSize = AzVec2(frame.size.width, frame.size.height);
        imgui_monitor.WorkPos = AzVec2(visibleFrame.origin.x, visibleFrame.origin.y);
        imgui_monitor.WorkSize = AzVec2(visibleFrame.size.width, visibleFrame.size.height);
        imgui_monitor.DpiScale = screen.backingScaleFactor;
        imgui_monitor.PlatformHandle = (__bridge_retained void*)screen;

        platform_io.Monitors.push_back(imgui_monitor);
    }
}

static void Az_AzplOSX_InitMultiViewportSupport()
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Platform_CreateWindow = Az_AzplOSX_CreateWindow;
    platform_io.Platform_DestroyWindow = Az_AzplOSX_DestroyWindow;
    platform_io.Platform_ShowWindow = Az_AzplOSX_ShowWindow;
    platform_io.Platform_SetWindowPos = Az_AzplOSX_SetWindowPos;
    platform_io.Platform_GetWindowPos = Az_AzplOSX_GetWindowPos;
    platform_io.Platform_SetWindowSize = Az_AzplOSX_SetWindowSize;
    platform_io.Platform_GetWindowSize = Az_AzplOSX_GetWindowSize;
    platform_io.Platform_GetWindowFramebufferScale = Az_AzplOSX_GetWindowFramebufferScale;
    platform_io.Platform_SetWindowFocus = Az_AzplOSX_SetWindowFocus;
    platform_io.Platform_GetWindowFocus = Az_AzplOSX_GetWindowFocus;
    platform_io.Platform_GetWindowMinimized = Az_AzplOSX_GetWindowMinimized;
    platform_io.Platform_SetWindowTitle = Az_AzplOSX_SetWindowTitle;
    platform_io.Platform_SetWindowAlpha = Az_AzplOSX_SetWindowAlpha;
    platform_io.Platform_GetWindowDpiScale = Az_AzplOSX_GetWindowDpiScale; 

    AzViewport* main_viewport = Az::GetMainViewport();
    Az_AzplOSX_ViewportData* vd = AZ_NEW(Az_AzplOSX_ViewportData)();
    vd->Window = bd->Window;
    vd->WindowOwned = false;
    main_viewport->PlatformUserData = vd;
    main_viewport->PlatformHandle = (__bridge void*)bd->Window;

    [NSNotificationCenter.defaultCenter addObserver:bd->Observer
                                           selector:@selector(displaysDidChange:)
                                               name:NSApplicationDidChangeScreenParametersNotification
                                             object:nil];
}

static void Az_AzplOSX_ShutdownMultiViewportSupport()
{
    Az_AzplOSX_Data* bd = Az_AzplOSX_GetBackendData();
    [NSNotificationCenter.defaultCenter removeObserver:bd->Observer
                                                  name:NSApplicationDidChangeScreenParametersNotification
                                                object:nil];
    bd->Observer = nullptr;
    bd->Window = nullptr;
    if (bd->Monitor != nullptr)
    {
        [NSEvent removeMonitor:bd->Monitor];
        bd->Monitor = nullptr;
    }

    AzViewport* main_viewport = Az::GetMainViewport();
    Az_AzplOSX_ViewportData* vd = (Az_AzplOSX_ViewportData*)main_viewport->PlatformUserData;
    AZ_DELETE(vd);
    main_viewport->PlatformUserData = nullptr;
    Az::DestroyPlatformWindows();
}

#endif 