// Copyright (c) 2025 Case Technologies

#define AZ_VERSION "2.0.0"
#define AZ_VERSION_NUM 20000
#define AZ_HAS_TABLE
#define AZ_HAS_TEXTURES

#define AZ_HAS_VIEWPORT
#define AZ_HAS_DOCK

#pragma once

#ifdef AZ_USER_CONFIG
#include AZ_USER_CONFIG
#endif

#ifndef AZ_DISABLE

#include <float.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#ifndef AZ_API
#define AZ_API
#endif
#ifndef AZ_IMPL_API
#define AZ_IMPL_API AZ_API
#endif

#ifndef AZ_ASSERT
#include <assert.h>
#define AZ_ASSERT(_EXPR) assert(_EXPR)

#endif
#define AZ_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

#define AZ_UNUSED(_VAR) ((void)(_VAR))

#define AZ_CHECKVERSION()                                                      \
    Az::DebugCheckVersionAndDataLayout(AZ_VERSION,                             \
                                       sizeof(AzIO),                           \
                                       sizeof(AzStyle),                        \
                                       sizeof(AzVec2),                         \
                                       sizeof(AzVec4),                         \
                                       sizeof(AzDrawVert),                     \
                                       sizeof(AzDrawIdx))

#if !defined(AZ_USE_STB_SPRINTF) && defined(__MINGW32__) && !defined(__clang__)
#define AZ_FMTARGS(FMT) __attribute__((format(gnu_printf, FMT, FMT + 1)))
#define AZ_FMTLIST(FMT) __attribute__((format(gnu_printf, FMT, 0)))
#elif !defined(AZ_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__))
#define AZ_FMTARGS(FMT) __attribute__((format(printf, FMT, FMT + 1)))
#define AZ_FMTLIST(FMT) __attribute__((format(printf, FMT, 0)))
#else
#define AZ_FMTARGS(FMT)
#define AZ_FMTLIST(FMT)
#endif

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)     \
        && !defined(AZ_DEBUG_PARANOID)
#define AZ_MSVC_RUNTIME_CHECKS_OFF                                             \
    __pragma(runtime_checks("", off)) __pragma(check_stack(off))               \
            __pragma(strict_gs_check(push, off))
#define AZ_MSVC_RUNTIME_CHECKS_RESTORE                                         \
    __pragma(runtime_checks("", restore)) __pragma(check_stack())              \
            __pragma(strict_gs_check(pop))
#else
#define AZ_MSVC_RUNTIME_CHECKS_OFF
#define AZ_MSVC_RUNTIME_CHECKS_RESTORE
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26495)

#endif
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wfloat-equal"

#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"

#pragma clang diagnostic ignored "-Wreserved-identifier"

#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"

#pragma clang diagnostic ignored "-Wnontrivial-memaccess"

#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"

#pragma GCC diagnostic ignored "-Wfloat-equal"

#pragma GCC diagnostic ignored "-Wclass-memaccess"

#endif

typedef unsigned int AzID;

typedef signed char AzS8;
typedef unsigned char AzU8;
typedef signed short AzS16;
typedef unsigned short AzU16;
typedef signed int AzS32;
typedef unsigned int AzU32;

typedef signed long long AzS64;
typedef unsigned long long AzU64;

struct AzDrawChannel;

struct AzDrawCmd;

struct AzDrawData;

struct AzDrawList;

struct AzDrawListSharedData;

struct AzDrawListSplitter;

struct AzDrawVert;

struct AzFont;
struct AzFontAtlas;

struct AzFontAtlasBuilder;
struct AzFontAtlasRect;

struct AzFontBaked;
struct AzFontConfig;
struct AzFontGlyph;

struct AzFontGlyphRangesBuilder;

struct AzFontLoader;

struct AzTextureData;

struct AzTextureRect;
struct AzColor;

struct AzContext;

struct AzIO;

struct AzInputTextCallbackData;

struct AzKeyData;

struct AzListClipper;
struct AzMultiSelectIO;

struct AzOnceUponAFrame;

struct AzPayload;
struct AzPlatformIO;

struct AzPlatformAzeData;

struct AzPlatformMonitor;

struct AzSelectionBasicStorage;

struct AzSelectionExternalStorage;

struct AzSelectionRequest;

struct AzSizeCallbackData;

struct AzStorage;
struct AzStoragePair;
struct AzStyle;
struct AzTableSortSpecs;

struct AzTableColumnSortSpecs;

struct AzTextBuffer;

struct AzTextFilter;

struct AzViewport;

struct AzWindowClass;

enum AzDir : int;

enum AzKey : int;

enum AzMouseSource : int;

enum AzSortDirection : AzU8;

typedef int AzCol;

typedef int AzCond;

typedef int AzDataType;

typedef int AzMouseButton;

typedef int AzMouseCursor;

typedef int AzStyleVar;

typedef int AzTableBgTarget;

typedef int AzDrawFlags;

typedef int AzDrawListFlags;

typedef int AzFontFlags;
typedef int AzFontAtlasFlags;

typedef int AzBackendFlags;

typedef int AzButtonFlags;

typedef int AzChildFlags;

typedef int AzColorEditFlags;

typedef int AzConfigFlags;

typedef int AzComboFlags;

typedef int AzDockNodeFlags;

typedef int AzDragDropFlags;

typedef int AzFocusedFlags;

typedef int AzHoveredFlags;

typedef int AzInputFlags;

typedef int AzInputTextFlags;

typedef int AzItemFlags;

typedef int AzKeyChord;

typedef int AzPopupFlags;

typedef int AzMultiSelectFlags;

typedef int AzSelectableFlags;

typedef int AzSliderFlags;

typedef int AzTabBarFlags;

typedef int AzTabItemFlags;

typedef int AzTableFlags;

typedef int AzTableColumnFlags;

typedef int AzTableRowFlags;

typedef int AzTreeNodeFlags;

typedef int AzViewportFlags;

typedef int AzWindowFlags;

typedef unsigned int AzWchar32;

typedef unsigned short AzWchar16;

#ifdef AZ_USE_WCHAR32

typedef AzWchar32 AzWchar;
#else
typedef AzWchar16 AzWchar;
#endif

typedef AzS64 AzSelectionUserData;

typedef int (*AzInputTextCallback)(AzInputTextCallbackData* data);

typedef void (*AzSizeCallback)(AzSizeCallbackData* data);

typedef void* (*AzMemAllocFunc)(size_t sz, void* user_data);

typedef void (*AzMemFreeFunc)(void* ptr, void* user_data);

AZ_MSVC_RUNTIME_CHECKS_OFF
struct AzVec2
{
    float x, y;
    constexpr AzVec2() : x(0.0f), y(0.0f)
    {}
    constexpr AzVec2(float _x, float _y) : x(_x), y(_y)
    {}
    float& operator[](size_t idx)
    {
        AZ_ASSERT(idx == 0 || idx == 1);
        return ((float*)(void*)(char*)this)[idx];
    }
    float operator[](size_t idx) const
    {
        AZ_ASSERT(idx == 0 || idx == 1);
        return ((const float*)(const void*)(const char*)this)[idx];
    }
#ifdef AZ_VEC2_CLASS_EXTRA
    AZ_VEC2_CLASS_EXTRA

#endif
};

struct AzVec4
{
    float x, y, z, w;
    constexpr AzVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
    {}
    constexpr AzVec4(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w)
    {}
#ifdef AZ_VEC4_CLASS_EXTRA
    AZ_VEC4_CLASS_EXTRA

#endif
};
AZ_MSVC_RUNTIME_CHECKS_RESTORE

#ifndef AzTextureID
typedef AzU64 AzTextureID;

#endif

#ifndef AzTextureID_Invalid
#define AzTextureID_Invalid ((AzTextureID)0)
#endif

AZ_MSVC_RUNTIME_CHECKS_OFF
struct AzTextureRef
{
    AzTextureRef()
    {
        _TexData = NULL;
        _TexID = AzTextureID_Invalid;
    }
    AzTextureRef(AzTextureID tex_id)
    {
        _TexData = NULL;
        _TexID = tex_id;
    }
#if !defined(AZ_DISABLE_OBSOLETE_FUNCTIONS) && !defined(AzTextureID)
    AzTextureRef(void* tex_id)
    {
        _TexData = NULL;
        _TexID = (AzTextureID)(size_t)tex_id;
    }
#endif

    inline AzTextureID GetTexID() const;

    AzTextureData* _TexData;

    AzTextureID _TexID;
};
AZ_MSVC_RUNTIME_CHECKS_RESTORE

namespace Az
{

    AZ_API AzContext* CreateContext(AzFontAtlas* shared_font_atlas = NULL);
    AZ_API void DestroyContext(AzContext* ctx = NULL);

    AZ_API AzContext* GetCurrentContext();
    AZ_API void SetCurrentContext(AzContext* ctx);

    AZ_API AzIO& GetIO();

    AZ_API AzPlatformIO& GetPlatformIO();

    AZ_API AzStyle& GetStyle();

    AZ_API void NewFrame();

    AZ_API void EndFrame();

    AZ_API void Render();

    AZ_API AzDrawData* GetDrawData();

    AZ_API void ShowMetricsWindow(bool* p_open = NULL);

    AZ_API void ShowDebugLogWindow(bool* p_open = NULL);

    AZ_API void ShowIDStackToolWindow(bool* p_open = NULL);

    AZ_API void ShowAboutWindow(bool* p_open = NULL);

    AZ_API void ShowStyleEditor(AzStyle* ref = NULL);

    AZ_API bool ShowStyleSelector(const char* label);

    AZ_API void ShowFontSelector(const char* label);

    AZ_API void ShowUserGuide();

    AZ_API const char* GetVersion();

    AZ_API void StyleColorsDark(AzStyle* dst = NULL);

    AZ_API void StyleColorsLight(AzStyle* dst = NULL);

    AZ_API void StyleColorsClassic(AzStyle* dst = NULL);

    AZ_API bool Begin(const char* name,
                      bool* p_open = NULL,
                      AzWindowFlags flags = 0);
    AZ_API void End();

    AZ_API bool BeginChild(const char* str_id,
                           const AzVec2& size = AzVec2(0, 0),
                           AzChildFlags child_flags = 0,
                           AzWindowFlags window_flags = 0);
    AZ_API bool BeginChild(AzID id,
                           const AzVec2& size = AzVec2(0, 0),
                           AzChildFlags child_flags = 0,
                           AzWindowFlags window_flags = 0);
    AZ_API void EndChild();

    AZ_API bool IsWindowAppearing();
    AZ_API bool IsWindowCollapsed();
    AZ_API bool IsWindowFocused(AzFocusedFlags flags = 0);

    AZ_API bool IsWindowHovered(AzHoveredFlags flags = 0);

    AZ_API AzDrawList* GetWindowDrawList();

    AZ_API float GetWindowDpiScale();

    AZ_API AzVec2 GetWindowPos();

    AZ_API AzVec2 GetWindowSize();

    AZ_API float GetWindowWidth();

    AZ_API float GetWindowHeight();

    AZ_API AzViewport* GetWindowViewport();

    AZ_API void SetNextWindowPos(const AzVec2& pos,
                                 AzCond cond = 0,
                                 const AzVec2& pivot = AzVec2(0, 0));

    AZ_API void SetNextWindowSize(const AzVec2& size, AzCond cond = 0);

    AZ_API void SetNextWindowSizeConstraints(
            const AzVec2& size_min,
            const AzVec2& size_max,
            AzSizeCallback custom_callback = NULL,
            void* custom_callback_data = NULL);

    AZ_API void SetNextWindowContentSize(const AzVec2& size);

    AZ_API void SetNextWindowCollapsed(bool collapsed, AzCond cond = 0);

    AZ_API void SetNextWindowFocus();

    AZ_API void SetNextWindowScroll(const AzVec2& scroll);

    AZ_API void SetNextWindowBgAlpha(float alpha);

    AZ_API void SetNextWindowViewport(AzID viewport_id);
    AZ_API void SetWindowPos(const AzVec2& pos, AzCond cond = 0);

    AZ_API void SetWindowSize(const AzVec2& size, AzCond cond = 0);

    AZ_API void SetWindowCollapsed(bool collapsed, AzCond cond = 0);

    AZ_API void SetWindowFocus();

    AZ_API void SetWindowPos(const char* name,
                             const AzVec2& pos,
                             AzCond cond = 0);

    AZ_API void SetWindowSize(const char* name,
                              const AzVec2& size,
                              AzCond cond = 0);

    AZ_API void SetWindowCollapsed(const char* name,
                                   bool collapsed,
                                   AzCond cond = 0);

    AZ_API void SetWindowFocus(const char* name);

    AZ_API float GetScrollX();
    AZ_API float GetScrollY();
    AZ_API void SetScrollX(float scroll_x);

    AZ_API void SetScrollY(float scroll_y);

    AZ_API float GetScrollMaxX();

    AZ_API float GetScrollMaxY();

    AZ_API void SetScrollHereX(float center_x_ratio = 0.5f);

    AZ_API void SetScrollHereY(float center_y_ratio = 0.5f);

    AZ_API void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f);

    AZ_API void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);

    AZ_API void PushFont(AzFont* font, float font_size_base = -1);

    AZ_API void PopFont();
    AZ_API void PushFontSize(float font_size_base);
    AZ_API void PopFontSize();

    AZ_API void PushStyleColor(AzCol idx, AzU32 col);

    AZ_API void PushStyleColor(AzCol idx, const AzVec4& col);
    AZ_API void PopStyleColor(int count = 1);
    AZ_API void PushStyleVar(AzStyleVar idx, float val);

    AZ_API void PushStyleVar(AzStyleVar idx, const AzVec2& val);

    AZ_API void PushStyleVarX(AzStyleVar idx, float val_x);

    AZ_API void PushStyleVarY(AzStyleVar idx, float val_y);

    AZ_API void PopStyleVar(int count = 1);
    AZ_API void PushItemFlag(AzItemFlags option, bool enabled);

    AZ_API void PopItemFlag();

    AZ_API void PushItemWidth(float item_width);

    AZ_API void PopItemWidth();
    AZ_API void SetNextItemWidth(float item_width);

    AZ_API float CalcItemWidth();

    AZ_API void PushTextWrapPos(float wrap_local_pos_x = 0.0f);

    AZ_API void PopTextWrapPos();

    AZ_API AzFont* GetFont();
    AZ_API float GetFontSize();

    AZ_API AzVec2 GetFontTexUvWhitePixel();

    AZ_API AzFontBaked* GetFontBaked();

    AZ_API AzU32 GetColorU32(AzCol idx, float alpha_mul = 1.0f);

    AZ_API AzU32 GetColorU32(const AzVec4& col);

    AZ_API AzU32 GetColorU32(AzU32 col, float alpha_mul = 1.0f);

    AZ_API const AzVec4& GetStyleColorVec4(AzCol idx);

    AZ_API AzVec2 GetCursorScreenPos();

    AZ_API void SetCursorScreenPos(const AzVec2& pos);

    AZ_API AzVec2 GetContentRegionAvail();

    AZ_API AzVec2 GetCursorPos();

    AZ_API float GetCursorPosX();
    AZ_API float GetCursorPosY();
    AZ_API void SetCursorPos(const AzVec2& local_pos);
    AZ_API void SetCursorPosX(float local_x);
    AZ_API void SetCursorPosY(float local_y);
    AZ_API AzVec2 GetCursorStartPos();

    AZ_API void Separator();

    AZ_API void SameLine(float offset_from_start_x = 0.0f,
                         float spacing = -1.0f);

    AZ_API void NewLine();

    AZ_API void Spacing();
    AZ_API void Dummy(const AzVec2& size);

    AZ_API void Indent(float indent_w = 0.0f);

    AZ_API void Unindent(float indent_w = 0.0f);

    AZ_API void BeginGroup();
    AZ_API void EndGroup();

    AZ_API void AlignTextToFramePadding();

    AZ_API float GetTextLineHeight();
    AZ_API float GetTextLineHeightWithSpacing();

    AZ_API float GetFrameHeight();
    AZ_API float GetFrameHeightWithSpacing();

    AZ_API void PushID(const char* str_id);

    AZ_API void PushID(const char* str_id_begin, const char* str_id_end);

    AZ_API void PushID(const void* ptr_id);

    AZ_API void PushID(int int_id);

    AZ_API void PopID();
    AZ_API AzID GetID(const char* str_id);

    AZ_API AzID GetID(const char* str_id_begin, const char* str_id_end);
    AZ_API AzID GetID(const void* ptr_id);
    AZ_API AzID GetID(int int_id);

    AZ_API void TextUnformatted(const char* text, const char* text_end = NULL);

    AZ_API void Text(const char* fmt, ...) AZ_FMTARGS(1);
    AZ_API void TextV(const char* fmt, va_list args) AZ_FMTLIST(1);
    AZ_API void TextColored(const AzVec4& col, const char* fmt, ...)
            AZ_FMTARGS(2);

    AZ_API void TextColoredV(const AzVec4& col, const char* fmt, va_list args)
            AZ_FMTLIST(2);
    AZ_API void TextDisabled(const char* fmt, ...) AZ_FMTARGS(1);

    AZ_API void TextDisabledV(const char* fmt, va_list args) AZ_FMTLIST(1);
    AZ_API void TextWrapped(const char* fmt, ...) AZ_FMTARGS(1);

    AZ_API void TextWrappedV(const char* fmt, va_list args) AZ_FMTLIST(1);
    AZ_API void LabelText(const char* label, const char* fmt, ...)
            AZ_FMTARGS(2);

    AZ_API void LabelTextV(const char* label, const char* fmt, va_list args)
            AZ_FMTLIST(2);
    AZ_API void BulletText(const char* fmt, ...) AZ_FMTARGS(1);
    AZ_API void BulletTextV(const char* fmt, va_list args) AZ_FMTLIST(1);
    AZ_API void SeparatorText(const char* label);

    AZ_API bool Button(const char* label, const AzVec2& size = AzVec2(0, 0));
    AZ_API bool SmallButton(const char* label);

    AZ_API bool InvisibleButton(const char* str_id,
                                const AzVec2& size,
                                AzButtonFlags flags = 0);

    AZ_API bool ArrowButton(const char* str_id, AzDir dir);

    AZ_API bool Checkbox(const char* label, bool* v);
    AZ_API bool CheckboxFlags(const char* label, int* flags, int flags_value);
    AZ_API bool CheckboxFlags(const char* label,
                              unsigned int* flags,
                              unsigned int flags_value);
    AZ_API bool RadioButton(const char* label, bool active);

    AZ_API bool RadioButton(const char* label, int* v, int v_button);

    AZ_API void ProgressBar(float fraction,
                            const AzVec2& size_arg = AzVec2(-FLT_MIN, 0),
                            const char* overlay = NULL);
    AZ_API void Bullet();

    AZ_API bool TextLink(const char* label);

    AZ_API bool TextLinkOpenURL(const char* label, const char* url = NULL);

    AZ_API void Azage(AzTextureRef tex_ref,
                      const AzVec2& image_size,
                      const AzVec2& uv0 = AzVec2(0, 0),
                      const AzVec2& uv1 = AzVec2(1, 1));
    AZ_API void AzageWithBg(AzTextureRef tex_ref,
                            const AzVec2& image_size,
                            const AzVec2& uv0 = AzVec2(0, 0),
                            const AzVec2& uv1 = AzVec2(1, 1),
                            const AzVec4& bg_col = AzVec4(0, 0, 0, 0),
                            const AzVec4& tint_col = AzVec4(1, 1, 1, 1));
    AZ_API bool AzageButton(const char* str_id,
                            AzTextureRef tex_ref,
                            const AzVec2& image_size,
                            const AzVec2& uv0 = AzVec2(0, 0),
                            const AzVec2& uv1 = AzVec2(1, 1),
                            const AzVec4& bg_col = AzVec4(0, 0, 0, 0),
                            const AzVec4& tint_col = AzVec4(1, 1, 1, 1));

    AZ_API bool BeginCombo(const char* label,
                           const char* preview_value,
                           AzComboFlags flags = 0);
    AZ_API void EndCombo();

    AZ_API bool Combo(const char* label,
                      int* current_item,
                      const char* const items[],
                      int items_count,
                      int popup_max_height_in_items = -1);
    AZ_API bool Combo(const char* label,
                      int* current_item,
                      const char* items_separated_by_zeros,
                      int popup_max_height_in_items = -1);

    AZ_API bool Combo(const char* label,
                      int* current_item,
                      const char* (*getter)(void* user_data, int idx),
                      void* user_data,
                      int items_count,
                      int popup_max_height_in_items = -1);

    AZ_API bool DragFloat(const char* label,
                          float* v,
                          float v_speed = 1.0f,
                          float v_min = 0.0f,
                          float v_max = 0.0f,
                          const char* format = "%.3f",
                          AzSliderFlags flags = 0);

    AZ_API bool DragFloat2(const char* label,
                           float v[2],
                           float v_speed = 1.0f,
                           float v_min = 0.0f,
                           float v_max = 0.0f,
                           const char* format = "%.3f",
                           AzSliderFlags flags = 0);
    AZ_API bool DragFloat3(const char* label,
                           float v[3],
                           float v_speed = 1.0f,
                           float v_min = 0.0f,
                           float v_max = 0.0f,
                           const char* format = "%.3f",
                           AzSliderFlags flags = 0);
    AZ_API bool DragFloat4(const char* label,
                           float v[4],
                           float v_speed = 1.0f,
                           float v_min = 0.0f,
                           float v_max = 0.0f,
                           const char* format = "%.3f",
                           AzSliderFlags flags = 0);
    AZ_API bool DragFloatRange2(const char* label,
                                float* v_current_min,
                                float* v_current_max,
                                float v_speed = 1.0f,
                                float v_min = 0.0f,
                                float v_max = 0.0f,
                                const char* format = "%.3f",
                                const char* format_max = NULL,
                                AzSliderFlags flags = 0);
    AZ_API bool DragInt(const char* label,
                        int* v,
                        float v_speed = 1.0f,
                        int v_min = 0,
                        int v_max = 0,
                        const char* format = "%d",
                        AzSliderFlags flags = 0);

    AZ_API bool DragInt2(const char* label,
                         int v[2],
                         float v_speed = 1.0f,
                         int v_min = 0,
                         int v_max = 0,
                         const char* format = "%d",
                         AzSliderFlags flags = 0);
    AZ_API bool DragInt3(const char* label,
                         int v[3],
                         float v_speed = 1.0f,
                         int v_min = 0,
                         int v_max = 0,
                         const char* format = "%d",
                         AzSliderFlags flags = 0);
    AZ_API bool DragInt4(const char* label,
                         int v[4],
                         float v_speed = 1.0f,
                         int v_min = 0,
                         int v_max = 0,
                         const char* format = "%d",
                         AzSliderFlags flags = 0);
    AZ_API bool DragIntRange2(const char* label,
                              int* v_current_min,
                              int* v_current_max,
                              float v_speed = 1.0f,
                              int v_min = 0,
                              int v_max = 0,
                              const char* format = "%d",
                              const char* format_max = NULL,
                              AzSliderFlags flags = 0);
    AZ_API bool DragScalar(const char* label,
                           AzDataType data_type,
                           void* p_data,
                           float v_speed = 1.0f,
                           const void* p_min = NULL,
                           const void* p_max = NULL,
                           const char* format = NULL,
                           AzSliderFlags flags = 0);
    AZ_API bool DragScalarN(const char* label,
                            AzDataType data_type,
                            void* p_data,
                            int components,
                            float v_speed = 1.0f,
                            const void* p_min = NULL,
                            const void* p_max = NULL,
                            const char* format = NULL,
                            AzSliderFlags flags = 0);

    AZ_API bool SliderFloat(const char* label,
                            float* v,
                            float v_min,
                            float v_max,
                            const char* format = "%.3f",
                            AzSliderFlags flags = 0);

    AZ_API bool SliderFloat2(const char* label,
                             float v[2],
                             float v_min,
                             float v_max,
                             const char* format = "%.3f",
                             AzSliderFlags flags = 0);
    AZ_API bool SliderFloat3(const char* label,
                             float v[3],
                             float v_min,
                             float v_max,
                             const char* format = "%.3f",
                             AzSliderFlags flags = 0);
    AZ_API bool SliderFloat4(const char* label,
                             float v[4],
                             float v_min,
                             float v_max,
                             const char* format = "%.3f",
                             AzSliderFlags flags = 0);
    AZ_API bool SliderAngle(const char* label,
                            float* v_rad,
                            float v_degrees_min = -360.0f,
                            float v_degrees_max = +360.0f,
                            const char* format = "%.0f deg",
                            AzSliderFlags flags = 0);
    AZ_API bool SliderInt(const char* label,
                          int* v,
                          int v_min,
                          int v_max,
                          const char* format = "%d",
                          AzSliderFlags flags = 0);
    AZ_API bool SliderInt2(const char* label,
                           int v[2],
                           int v_min,
                           int v_max,
                           const char* format = "%d",
                           AzSliderFlags flags = 0);
    AZ_API bool SliderInt3(const char* label,
                           int v[3],
                           int v_min,
                           int v_max,
                           const char* format = "%d",
                           AzSliderFlags flags = 0);
    AZ_API bool SliderInt4(const char* label,
                           int v[4],
                           int v_min,
                           int v_max,
                           const char* format = "%d",
                           AzSliderFlags flags = 0);
    AZ_API bool SliderScalar(const char* label,
                             AzDataType data_type,
                             void* p_data,
                             const void* p_min,
                             const void* p_max,
                             const char* format = NULL,
                             AzSliderFlags flags = 0);
    AZ_API bool SliderScalarN(const char* label,
                              AzDataType data_type,
                              void* p_data,
                              int components,
                              const void* p_min,
                              const void* p_max,
                              const char* format = NULL,
                              AzSliderFlags flags = 0);
    AZ_API bool VSliderFloat(const char* label,
                             const AzVec2& size,
                             float* v,
                             float v_min,
                             float v_max,
                             const char* format = "%.3f",
                             AzSliderFlags flags = 0);
    AZ_API bool VSliderInt(const char* label,
                           const AzVec2& size,
                           int* v,
                           int v_min,
                           int v_max,
                           const char* format = "%d",
                           AzSliderFlags flags = 0);
    AZ_API bool VSliderScalar(const char* label,
                              const AzVec2& size,
                              AzDataType data_type,
                              void* p_data,
                              const void* p_min,
                              const void* p_max,
                              const char* format = NULL,
                              AzSliderFlags flags = 0);

    AZ_API bool InputText(const char* label,
                          char* buf,
                          size_t buf_size,
                          AzInputTextFlags flags = 0,
                          AzInputTextCallback callback = NULL,
                          void* user_data = NULL);
    AZ_API bool InputTextMultiline(const char* label,
                                   char* buf,
                                   size_t buf_size,
                                   const AzVec2& size = AzVec2(0, 0),
                                   AzInputTextFlags flags = 0,
                                   AzInputTextCallback callback = NULL,
                                   void* user_data = NULL);
    AZ_API bool InputTextWithHint(const char* label,
                                  const char* hint,
                                  char* buf,
                                  size_t buf_size,
                                  AzInputTextFlags flags = 0,
                                  AzInputTextCallback callback = NULL,
                                  void* user_data = NULL);
    AZ_API bool InputFloat(const char* label,
                           float* v,
                           float step = 0.0f,
                           float step_fast = 0.0f,
                           const char* format = "%.3f",
                           AzInputTextFlags flags = 0);
    AZ_API bool InputFloat2(const char* label,
                            float v[2],
                            const char* format = "%.3f",
                            AzInputTextFlags flags = 0);
    AZ_API bool InputFloat3(const char* label,
                            float v[3],
                            const char* format = "%.3f",
                            AzInputTextFlags flags = 0);
    AZ_API bool InputFloat4(const char* label,
                            float v[4],
                            const char* format = "%.3f",
                            AzInputTextFlags flags = 0);
    AZ_API bool InputInt(const char* label,
                         int* v,
                         int step = 1,
                         int step_fast = 100,
                         AzInputTextFlags flags = 0);
    AZ_API bool InputInt2(const char* label,
                          int v[2],
                          AzInputTextFlags flags = 0);
    AZ_API bool InputInt3(const char* label,
                          int v[3],
                          AzInputTextFlags flags = 0);
    AZ_API bool InputInt4(const char* label,
                          int v[4],
                          AzInputTextFlags flags = 0);
    AZ_API bool InputDouble(const char* label,
                            double* v,
                            double step = 0.0,
                            double step_fast = 0.0,
                            const char* format = "%.6f",
                            AzInputTextFlags flags = 0);
    AZ_API bool InputScalar(const char* label,
                            AzDataType data_type,
                            void* p_data,
                            const void* p_step = NULL,
                            const void* p_step_fast = NULL,
                            const char* format = NULL,
                            AzInputTextFlags flags = 0);
    AZ_API bool InputScalarN(const char* label,
                             AzDataType data_type,
                             void* p_data,
                             int components,
                             const void* p_step = NULL,
                             const void* p_step_fast = NULL,
                             const char* format = NULL,
                             AzInputTextFlags flags = 0);

    AZ_API bool ColorEdit3(const char* label,
                           float col[3],
                           AzColorEditFlags flags = 0);
    AZ_API bool ColorEdit4(const char* label,
                           float col[4],
                           AzColorEditFlags flags = 0);
    AZ_API bool ColorPicker3(const char* label,
                             float col[3],
                             AzColorEditFlags flags = 0);
    AZ_API bool ColorPicker4(const char* label,
                             float col[4],
                             AzColorEditFlags flags = 0,
                             const float* ref_col = NULL);
    AZ_API bool ColorButton(const char* desc_id,
                            const AzVec4& col,
                            AzColorEditFlags flags = 0,
                            const AzVec2& size = AzVec2(0, 0));

    AZ_API void SetColorEditOptions(AzColorEditFlags flags);

    AZ_API bool TreeNode(const char* label);
    AZ_API bool TreeNode(const char* str_id, const char* fmt, ...)
            AZ_FMTARGS(2);

    AZ_API bool TreeNode(const void* ptr_id, const char* fmt, ...)
            AZ_FMTARGS(2);
    AZ_API bool TreeNodeV(const char* str_id, const char* fmt, va_list args)
            AZ_FMTLIST(2);
    AZ_API bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
            AZ_FMTLIST(2);
    AZ_API bool TreeNodeEx(const char* label, AzTreeNodeFlags flags = 0);
    AZ_API bool TreeNodeEx(const char* str_id,
                           AzTreeNodeFlags flags,
                           const char* fmt,
                           ...) AZ_FMTARGS(3);
    AZ_API bool TreeNodeEx(const void* ptr_id,
                           AzTreeNodeFlags flags,
                           const char* fmt,
                           ...) AZ_FMTARGS(3);
    AZ_API bool TreeNodeExV(const char* str_id,
                            AzTreeNodeFlags flags,
                            const char* fmt,
                            va_list args) AZ_FMTLIST(3);
    AZ_API bool TreeNodeExV(const void* ptr_id,
                            AzTreeNodeFlags flags,
                            const char* fmt,
                            va_list args) AZ_FMTLIST(3);
    AZ_API void TreePush(const char* str_id);

    AZ_API void TreePush(const void* ptr_id);
    AZ_API void TreePop();
    AZ_API float GetTreeNodeToLabelSpacing();

    AZ_API bool CollapsingHeader(const char* label, AzTreeNodeFlags flags = 0);

    AZ_API bool CollapsingHeader(const char* label,
                                 bool* p_visible,
                                 AzTreeNodeFlags flags = 0);

    AZ_API void SetNextItemOpen(bool is_open, AzCond cond = 0);

    AZ_API void SetNextItemStorageID(AzID storage_id);

    AZ_API bool Selectable(const char* label,
                           bool selected = false,
                           AzSelectableFlags flags = 0,
                           const AzVec2& size = AzVec2(0, 0));

    AZ_API bool Selectable(const char* label,
                           bool* p_selected,
                           AzSelectableFlags flags = 0,
                           const AzVec2& size = AzVec2(0, 0));

    AZ_API AzMultiSelectIO* BeginMultiSelect(AzMultiSelectFlags flags,
                                             int selection_size = -1,
                                             int items_count = -1);
    AZ_API AzMultiSelectIO* EndMultiSelect();
    AZ_API void SetNextItemSelectionUserData(
            AzSelectionUserData selection_user_data);
    AZ_API bool IsItemToggledSelection();

    AZ_API bool BeginListBox(const char* label,
                             const AzVec2& size = AzVec2(0, 0));
    AZ_API void EndListBox();

    AZ_API bool ListBox(const char* label,
                        int* current_item,
                        const char* const items[],
                        int items_count,
                        int height_in_items = -1);
    AZ_API bool ListBox(const char* label,
                        int* current_item,
                        const char* (*getter)(void* user_data, int idx),
                        void* user_data,
                        int items_count,
                        int height_in_items = -1);

    AZ_API void PlotLines(const char* label,
                          const float* values,
                          int values_count,
                          int values_offset = 0,
                          const char* overlay_text = NULL,
                          float scale_min = FLT_MAX,
                          float scale_max = FLT_MAX,
                          AzVec2 graph_size = AzVec2(0, 0),
                          int stride = sizeof(float));
    AZ_API void PlotLines(const char* label,
                          float (*values_getter)(void* data, int idx),
                          void* data,
                          int values_count,
                          int values_offset = 0,
                          const char* overlay_text = NULL,
                          float scale_min = FLT_MAX,
                          float scale_max = FLT_MAX,
                          AzVec2 graph_size = AzVec2(0, 0));
    AZ_API void PlotHistogram(const char* label,
                              const float* values,
                              int values_count,
                              int values_offset = 0,
                              const char* overlay_text = NULL,
                              float scale_min = FLT_MAX,
                              float scale_max = FLT_MAX,
                              AzVec2 graph_size = AzVec2(0, 0),
                              int stride = sizeof(float));
    AZ_API void PlotHistogram(const char* label,
                              float (*values_getter)(void* data, int idx),
                              void* data,
                              int values_count,
                              int values_offset = 0,
                              const char* overlay_text = NULL,
                              float scale_min = FLT_MAX,
                              float scale_max = FLT_MAX,
                              AzVec2 graph_size = AzVec2(0, 0));

    AZ_API void Value(const char* prefix, bool b);
    AZ_API void Value(const char* prefix, int v);
    AZ_API void Value(const char* prefix, unsigned int v);
    AZ_API void Value(const char* prefix,
                      float v,
                      const char* float_format = NULL);

    AZ_API bool BeginMenuBar();

    AZ_API void EndMenuBar();

    AZ_API bool BeginMainMenuBar();

    AZ_API void EndMainMenuBar();

    AZ_API bool BeginMenu(const char* label, bool enabled = true);

    AZ_API void EndMenu();

    AZ_API bool MenuItem(const char* label,
                         const char* shortcut = NULL,
                         bool selected = false,
                         bool enabled = true);
    AZ_API bool MenuItem(const char* label,
                         const char* shortcut,
                         bool* p_selected,
                         bool enabled = true);

    AZ_API bool BeginTooltip();
    AZ_API void EndTooltip();

    AZ_API void SetTooltip(const char* fmt, ...) AZ_FMTARGS(1);

    AZ_API void SetTooltipV(const char* fmt, va_list args) AZ_FMTLIST(1);

    AZ_API bool BeginItemTooltip();

    AZ_API void SetItemTooltip(const char* fmt, ...) AZ_FMTARGS(1);

    AZ_API void SetItemTooltipV(const char* fmt, va_list args) AZ_FMTLIST(1);

    AZ_API bool BeginPopup(const char* str_id, AzWindowFlags flags = 0);

    AZ_API bool BeginPopupModal(const char* name,
                                bool* p_open = NULL,
                                AzWindowFlags flags = 0);

    AZ_API void EndPopup();

    AZ_API void OpenPopup(const char* str_id, AzPopupFlags popup_flags = 0);

    AZ_API void OpenPopup(AzID id, AzPopupFlags popup_flags = 0);

    AZ_API void OpenPopupOnItemClick(const char* str_id = NULL,
                                     AzPopupFlags popup_flags = 1);

    AZ_API void CloseCurrentPopup();

    AZ_API bool BeginPopupContextItem(const char* str_id = NULL,
                                      AzPopupFlags popup_flags = 1);

    AZ_API bool BeginPopupContextWindow(const char* str_id = NULL,
                                        AzPopupFlags popup_flags = 1);

    AZ_API bool BeginPopupContextVoid(const char* str_id = NULL,
                                      AzPopupFlags popup_flags = 1);

    AZ_API bool IsPopupOpen(const char* str_id, AzPopupFlags flags = 0);

    AZ_API bool BeginTable(const char* str_id,
                           int columns,
                           AzTableFlags flags = 0,
                           const AzVec2& outer_size = AzVec2(0.0f, 0.0f),
                           float inner_width = 0.0f);
    AZ_API void EndTable();

    AZ_API void TableNextRow(AzTableRowFlags row_flags = 0,
                             float min_row_height = 0.0f);

    AZ_API bool TableNextColumn();

    AZ_API bool TableSetColumnIndex(int column_n);

    AZ_API void TableSetupColumn(const char* label,
                                 AzTableColumnFlags flags = 0,
                                 float init_width_or_weight = 0.0f,
                                 AzID user_id = 0);
    AZ_API void TableSetupScrollFreeze(int cols, int rows);
    AZ_API void TableHeader(const char* label);

    AZ_API void TableHeadersRow();

    AZ_API void TableAngledHeadersRow();

    AZ_API AzTableSortSpecs* TableGetSortSpecs();

    AZ_API int TableGetColumnCount();

    AZ_API int TableGetColumnIndex();
    AZ_API int TableGetRowIndex();
    AZ_API const char* TableGetColumnName(int column_n = -1);

    AZ_API AzTableColumnFlags TableGetColumnFlags(int column_n = -1);

    AZ_API void TableSetColumnEnabled(int column_n, bool v);

    AZ_API int TableGetHoveredColumn();

    AZ_API void TableSetBgColor(AzTableBgTarget target,
                                AzU32 color,
                                int column_n = -1);

    AZ_API void Columns(int count = 1,
                        const char* id = NULL,
                        bool borders = true);
    AZ_API void NextColumn();

    AZ_API int GetColumnIndex();
    AZ_API float GetColumnWidth(int column_index = -1);

    AZ_API void SetColumnWidth(int column_index, float width);

    AZ_API float GetColumnOffset(int column_index = -1);

    AZ_API void SetColumnOffset(int column_index, float offset_x);

    AZ_API int GetColumnsCount();

    AZ_API bool BeginTabBar(const char* str_id, AzTabBarFlags flags = 0);

    AZ_API void EndTabBar();

    AZ_API bool BeginTabItem(const char* label,
                             bool* p_open = NULL,
                             AzTabItemFlags flags = 0);

    AZ_API void EndTabItem();

    AZ_API bool TabItemButton(const char* label, AzTabItemFlags flags = 0);

    AZ_API void SetTabItemClosed(const char* tab_or_docked_window_label);

    AZ_API AzID DockSpace(AzID dockspace_id,
                          const AzVec2& size = AzVec2(0, 0),
                          AzDockNodeFlags flags = 0,
                          const AzWindowClass* window_class = NULL);
    AZ_API AzID DockSpaceOverViewport(AzID dockspace_id = 0,
                                      const AzViewport* viewport = NULL,
                                      AzDockNodeFlags flags = 0,
                                      const AzWindowClass* window_class = NULL);
    AZ_API void SetNextWindowDockID(AzID dock_id, AzCond cond = 0);

    AZ_API void SetNextWindowClass(const AzWindowClass* window_class);

    AZ_API AzID GetWindowDockID();
    AZ_API bool IsWindowDocked();

    AZ_API void LogToTTY(int auto_open_depth = -1);

    AZ_API void LogToFile(int auto_open_depth = -1,
                          const char* filename = NULL);

    AZ_API void LogToClipboard(int auto_open_depth = -1);

    AZ_API void LogFinish();
    AZ_API void LogButtons();

    AZ_API void LogText(const char* fmt, ...) AZ_FMTARGS(1);

    AZ_API void LogTextV(const char* fmt, va_list args) AZ_FMTLIST(1);

    AZ_API bool BeginDragDropSource(AzDragDropFlags flags = 0);

    AZ_API bool SetDragDropPayload(const char* type,
                                   const void* data,
                                   size_t sz,
                                   AzCond cond = 0);

    AZ_API void EndDragDropSource();

    AZ_API bool BeginDragDropTarget();

    AZ_API const AzPayload* AcceptDragDropPayload(const char* type,
                                                  AzDragDropFlags flags = 0);

    AZ_API void EndDragDropTarget();

    AZ_API const AzPayload* GetDragDropPayload();

    AZ_API void BeginDisabled(bool disabled = true);
    AZ_API void EndDisabled();

    AZ_API void PushClipRect(const AzVec2& clip_rect_min,
                             const AzVec2& clip_rect_max,
                             bool intersect_with_current_clip_rect);
    AZ_API void PopClipRect();

    AZ_API void SetItemDefaultFocus();

    AZ_API void SetKeyboardFocusHere(int offset = 0);

    AZ_API void SetNavCursorVisible(bool visible);

    AZ_API void SetNextItemAllowOverlap();

    AZ_API bool IsItemHovered(AzHoveredFlags flags = 0);

    AZ_API bool IsItemActive();

    AZ_API bool IsItemFocused();

    AZ_API bool IsItemClicked(AzMouseButton mouse_button = 0);

    AZ_API bool IsItemVisible();

    AZ_API bool IsItemEdited();

    AZ_API bool IsItemActivated();

    AZ_API bool IsItemDeactivated();

    AZ_API bool IsItemDeactivatedAfterEdit();

    AZ_API bool IsItemToggledOpen();

    AZ_API bool IsAnyItemHovered();
    AZ_API bool IsAnyItemActive();
    AZ_API bool IsAnyItemFocused();
    AZ_API AzID GetItemID();

    AZ_API AzVec2 GetItemRectMin();

    AZ_API AzVec2 GetItemRectMax();

    AZ_API AzVec2 GetItemRectSize();

    AZ_API AzViewport* GetMainViewport();

    AZ_API AzDrawList* GetBackgroundDrawList(AzViewport* viewport = NULL);

    AZ_API AzDrawList* GetForegroundDrawList(AzViewport* viewport = NULL);

    AZ_API bool IsRectVisible(const AzVec2& size);

    AZ_API bool IsRectVisible(const AzVec2& rect_min, const AzVec2& rect_max);

    AZ_API double GetTime();

    AZ_API int GetFrameCount();

    AZ_API AzDrawListSharedData* GetDrawListSharedData();

    AZ_API const char* GetStyleColorName(AzCol idx);

    AZ_API void SetStateStorage(AzStorage* storage);

    AZ_API AzStorage* GetStateStorage();

    AZ_API AzVec2 CalcTextSize(const char* text,
                               const char* text_end = NULL,
                               bool hide_text_after_double_hash = false,
                               float wrap_width = -1.0f);

    AZ_API AzVec4 ColorConvertU32ToFloat4(AzU32 in);
    AZ_API AzU32 ColorConvertFloat4ToU32(const AzVec4& in);
    AZ_API void ColorConvertRGBtoHSV(float r,
                                     float g,
                                     float b,
                                     float& out_h,
                                     float& out_s,
                                     float& out_v);
    AZ_API void ColorConvertHSVtoRGB(float h,
                                     float s,
                                     float v,
                                     float& out_r,
                                     float& out_g,
                                     float& out_b);

    AZ_API bool IsKeyDown(AzKey key);
    AZ_API bool IsKeyPressed(AzKey key, bool repeat = true);

    AZ_API bool IsKeyReleased(AzKey key);

    AZ_API bool IsKeyChordPressed(AzKeyChord key_chord);

    AZ_API int GetKeyPressedAmount(AzKey key, float repeat_delay, float rate);

    AZ_API const char* GetKeyName(AzKey key);

    AZ_API void SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard);

    AZ_API bool Shortcut(AzKeyChord key_chord, AzInputFlags flags = 0);
    AZ_API void SetNextItemShortcut(AzKeyChord key_chord,
                                    AzInputFlags flags = 0);

    AZ_API void SetItemKeyOwner(AzKey key);

    AZ_API bool IsMouseDown(AzMouseButton button);

    AZ_API bool IsMouseClicked(AzMouseButton button, bool repeat = false);

    AZ_API bool IsMouseReleased(AzMouseButton button);

    AZ_API bool IsMouseDoubleClicked(AzMouseButton button);

    AZ_API bool IsMouseReleasedWithDelay(AzMouseButton button, float delay);

    AZ_API int GetMouseClickedCount(AzMouseButton button);

    AZ_API bool IsMouseHoveringRect(const AzVec2& r_min,
                                    const AzVec2& r_max,
                                    bool clip = true);

    AZ_API bool IsMousePosValid(const AzVec2* mouse_pos = NULL);

    AZ_API bool IsAnyMouseDown();

    AZ_API AzVec2 GetMousePos();

    AZ_API AzVec2 GetMousePosOnOpeningCurrentPopup();

    AZ_API bool IsMouseDragging(AzMouseButton button,
                                float lock_threshold = -1.0f);

    AZ_API AzVec2 GetMouseDragDelta(AzMouseButton button = 0,
                                    float lock_threshold = -1.0f);

    AZ_API void ResetMouseDragDelta(AzMouseButton button = 0);
    AZ_API AzMouseCursor GetMouseCursor();

    AZ_API void SetMouseCursor(AzMouseCursor cursor_type);
    AZ_API void SetNextFrameWantCaptureMouse(bool want_capture_mouse);

    AZ_API const char* GetClipboardText();
    AZ_API void SetClipboardText(const char* text);

    AZ_API void LoadIniSettingsFromDisk(const char* ini_filename);

    AZ_API void LoadIniSettingsFromMemory(const char* ini_data,
                                          size_t ini_size = 0);

    AZ_API void SaveIniSettingsToDisk(const char* ini_filename);

    AZ_API const char* SaveIniSettingsToMemory(size_t* out_ini_size = NULL);

    AZ_API void DebugTextEncoding(const char* text);
    AZ_API void DebugFlashStyleColor(AzCol idx);
    AZ_API void DebugStartItemPicker();
    AZ_API bool DebugCheckVersionAndDataLayout(const char* version_str,
                                               size_t sz_io,
                                               size_t sz_style,
                                               size_t sz_vec2,
                                               size_t sz_vec4,
                                               size_t sz_drawvert,
                                               size_t sz_drawidx);
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AZ_API void DebugLog(const char* fmt, ...) AZ_FMTARGS(1);

    AZ_API void DebugLogV(const char* fmt, va_list args) AZ_FMTLIST(1);
#endif

    AZ_API void SetAllocatorFunctions(AzMemAllocFunc alloc_func,
                                      AzMemFreeFunc free_func,
                                      void* user_data = NULL);
    AZ_API void GetAllocatorFunctions(AzMemAllocFunc* p_alloc_func,
                                      AzMemFreeFunc* p_free_func,
                                      void** p_user_data);
    AZ_API void* MemAlloc(size_t size);
    AZ_API void MemFree(void* ptr);

    AZ_API void UpdatePlatformWindows();

    AZ_API void RenderPlatformWindowsDefault(void* platform_render_arg = NULL,
                                             void* renderer_render_arg = NULL);

    AZ_API void DestroyPlatformWindows();

    AZ_API AzViewport* FindViewportByID(AzID id);

    AZ_API AzViewport* FindViewportByPlatformHandle(void* platform_handle);

} // namespace Az

enum AzWindowFlags_
{
    AzWindowFlags_None = 0,
    AzWindowFlags_NoTitleBar = 1 << 0,
    AzWindowFlags_NoResize = 1 << 1,

    AzWindowFlags_NoMove = 1 << 2,
    AzWindowFlags_NoScrollbar = 1 << 3,

    AzWindowFlags_NoScrollWithMouse = 1 << 4,

    AzWindowFlags_NoCollapse = 1 << 5,

    AzWindowFlags_AlwaysAutoResize = 1 << 6,

    AzWindowFlags_NoBackground = 1 << 7,

    AzWindowFlags_NoSavedSettings = 1 << 8,

    AzWindowFlags_NoMouseInputs = 1 << 9,

    AzWindowFlags_MenuBar = 1 << 10,
    AzWindowFlags_HorizontalScrollbar = 1 << 11,

    AzWindowFlags_NoFocusOnAppearing = 1 << 12,

    AzWindowFlags_NoBringToFrontOnFocus = 1 << 13,

    AzWindowFlags_AlwaysVerticalScrollbar = 1 << 14,

    AzWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,

    AzWindowFlags_NoNavInputs = 1 << 16,

    AzWindowFlags_NoNavFocus = 1 << 17,

    AzWindowFlags_UnsavedDocument = 1 << 18,

    AzWindowFlags_NoDocking = 1 << 19,
    AzWindowFlags_NoNav = AzWindowFlags_NoNavInputs | AzWindowFlags_NoNavFocus,
    AzWindowFlags_NoDecoration = AzWindowFlags_NoTitleBar
                                 | AzWindowFlags_NoResize
                                 | AzWindowFlags_NoScrollbar
                                 | AzWindowFlags_NoCollapse,
    AzWindowFlags_NoInputs = AzWindowFlags_NoMouseInputs
                             | AzWindowFlags_NoNavInputs
                             | AzWindowFlags_NoNavFocus,

    AzWindowFlags_DockNodeHost = 1 << 23,

    AzWindowFlags_ChildWindow = 1 << 24,

    AzWindowFlags_Tooltip = 1 << 25,

    AzWindowFlags_Popup = 1 << 26,

    AzWindowFlags_Modal = 1 << 27,

    AzWindowFlags_ChildMenu = 1 << 28,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzWindowFlags_NavFlattened = 1 << 29,

    AzWindowFlags_AlwaysUseWindowPadding = 1 << 30,

#endif
};

enum AzChildFlags_
{
    AzChildFlags_None = 0,
    AzChildFlags_Borders = 1 << 0,

    AzChildFlags_AlwaysUseWindowPadding = 1 << 1,

    AzChildFlags_ResizeX = 1 << 2,

    AzChildFlags_ResizeY = 1 << 3,

    AzChildFlags_AutoResizeX = 1 << 4,

    AzChildFlags_AutoResizeY = 1 << 5,

    AzChildFlags_AlwaysAutoResize = 1 << 6,

    AzChildFlags_FrameStyle = 1 << 7,

    AzChildFlags_NavFlattened = 1 << 8,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzChildFlags_Border = AzChildFlags_Borders,

#endif
};

enum AzItemFlags_
{
    AzItemFlags_None = 0,
    AzItemFlags_NoTabStop = 1 << 0,

    AzItemFlags_NoNav = 1 << 1,

    AzItemFlags_NoNavDefaultFocus = 1 << 2,

    AzItemFlags_ButtonRepeat = 1 << 3,

    AzItemFlags_AutoClosePopups = 1 << 4,

    AzItemFlags_AllowDuplicateId = 1 << 5,
};

enum AzInputTextFlags_
{
    AzInputTextFlags_None = 0,
    AzInputTextFlags_CharsDecimal = 1 << 0,
    AzInputTextFlags_CharsHexadecimal = 1 << 1,
    AzInputTextFlags_CharsScientific = 1 << 2,

    AzInputTextFlags_CharsUppercase = 1 << 3,
    AzInputTextFlags_CharsNoBlank = 1 << 4,

    AzInputTextFlags_AllowTabInput = 1 << 5,

    AzInputTextFlags_EnterReturnsTrue = 1 << 6,

    AzInputTextFlags_EscapeClearsAll = 1 << 7,

    AzInputTextFlags_CtrlEnterForNewLine = 1 << 8,

    AzInputTextFlags_ReadOnly = 1 << 9,
    AzInputTextFlags_Password = 1 << 10,

    AzInputTextFlags_AlwaysOverwrite = 1 << 11,
    AzInputTextFlags_AutoSelectAll = 1 << 12,

    AzInputTextFlags_ParseEmptyRefVal = 1 << 13,

    AzInputTextFlags_DisplayEmptyRefVal = 1 << 14,

    AzInputTextFlags_NoHorizontalScroll = 1 << 15,

    AzInputTextFlags_NoUndoRedo = 1 << 16,

    AzInputTextFlags_ElideLeft = 1 << 17,

    AzInputTextFlags_CallbackCompletion = 1 << 18,
    AzInputTextFlags_CallbackHistory = 1 << 19,

    AzInputTextFlags_CallbackAlways = 1 << 20,

    AzInputTextFlags_CallbackCharFilter = 1 << 21,

    AzInputTextFlags_CallbackResize = 1 << 22,

    AzInputTextFlags_CallbackEdit = 1 << 23,
};

enum AzTreeNodeFlags_
{
    AzTreeNodeFlags_None = 0,
    AzTreeNodeFlags_Selected = 1 << 0,
    AzTreeNodeFlags_Framed = 1 << 1,

    AzTreeNodeFlags_AllowOverlap = 1 << 2,

    AzTreeNodeFlags_NoTreePushOnOpen = 1 << 3,

    AzTreeNodeFlags_NoAutoOpenOnLog = 1 << 4,

    AzTreeNodeFlags_DefaultOpen = 1 << 5,
    AzTreeNodeFlags_OpenOnDoubleClick = 1 << 6,

    AzTreeNodeFlags_OpenOnArrow = 1 << 7,

    AzTreeNodeFlags_Leaf = 1 << 8,

    AzTreeNodeFlags_Bullet = 1 << 9,

    AzTreeNodeFlags_FramePadding = 1 << 10,

    AzTreeNodeFlags_SpanAvailWidth = 1 << 11,

    AzTreeNodeFlags_SpanFullWidth = 1 << 12,

    AzTreeNodeFlags_SpanLabelWidth = 1 << 13,

    AzTreeNodeFlags_SpanAllColumns = 1 << 14,

    AzTreeNodeFlags_LabelSpanAllColumns = 1 << 15,

    AzTreeNodeFlags_NavLeftJumpsToParent = 1 << 17,

    AzTreeNodeFlags_CollapsingHeader = AzTreeNodeFlags_Framed
                                       | AzTreeNodeFlags_NoTreePushOnOpen
                                       | AzTreeNodeFlags_NoAutoOpenOnLog,

    AzTreeNodeFlags_DrawLinesNone = 1 << 18,
    AzTreeNodeFlags_DrawLinesFull = 1 << 19,

    AzTreeNodeFlags_DrawLinesToNodes = 1 << 20,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzTreeNodeFlags_NavLeftJumpsBackHere = AzTreeNodeFlags_NavLeftJumpsToParent,
    AzTreeNodeFlags_SpanTextWidth = AzTreeNodeFlags_SpanLabelWidth,
    AzTreeNodeFlags_AllowItemOverlap = AzTreeNodeFlags_AllowOverlap,
#endif
};

enum AzPopupFlags_
{
    AzPopupFlags_None = 0,
    AzPopupFlags_MouseButtonLeft = 0,

    AzPopupFlags_MouseButtonRight = 1,

    AzPopupFlags_MouseButtonMiddle = 2,

    AzPopupFlags_MouseButtonMask_ = 0x1F,
    AzPopupFlags_MouseButtonDefault_ = 1,
    AzPopupFlags_NoReopen = 1 << 5,

    AzPopupFlags_NoOpenOverExistingPopup = 1 << 7,

    AzPopupFlags_NoOpenOverItems = 1 << 8,

    AzPopupFlags_AnyPopupId = 1 << 10,

    AzPopupFlags_AnyPopupLevel = 1 << 11,

    AzPopupFlags_AnyPopup = AzPopupFlags_AnyPopupId
                            | AzPopupFlags_AnyPopupLevel,
};

enum AzSelectableFlags_
{
    AzSelectableFlags_None = 0,
    AzSelectableFlags_NoAutoClosePopups = 1 << 0,

    AzSelectableFlags_SpanAllColumns = 1 << 1,

    AzSelectableFlags_AllowDoubleClick = 1 << 2,

    AzSelectableFlags_Disabled = 1 << 3,

    AzSelectableFlags_AllowOverlap = 1 << 4,

    AzSelectableFlags_Highlight = 1 << 5,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzSelectableFlags_DontClosePopups = AzSelectableFlags_NoAutoClosePopups,
    AzSelectableFlags_AllowItemOverlap = AzSelectableFlags_AllowOverlap,
#endif
};

enum AzComboFlags_
{
    AzComboFlags_None = 0,
    AzComboFlags_PopupAlignLeft = 1 << 0,

    AzComboFlags_HeightSmall = 1 << 1,

    AzComboFlags_HeightRegular = 1 << 2,
    AzComboFlags_HeightLarge = 1 << 3,
    AzComboFlags_HeightLargest = 1 << 4,
    AzComboFlags_NoArrowButton = 1 << 5,

    AzComboFlags_NoPreview = 1 << 6,
    AzComboFlags_WidthFitPreview = 1 << 7,

    AzComboFlags_HeightMask_ = AzComboFlags_HeightSmall
                               | AzComboFlags_HeightRegular
                               | AzComboFlags_HeightLarge
                               | AzComboFlags_HeightLargest,
};

enum AzTabBarFlags_
{
    AzTabBarFlags_None = 0,
    AzTabBarFlags_Reorderable = 1 << 0,

    AzTabBarFlags_AutoSelectNewTabs = 1 << 1,

    AzTabBarFlags_TabListPopupButton = 1 << 2,

    AzTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,

    AzTabBarFlags_NoTabListScrollingButtons = 1 << 4,

    AzTabBarFlags_NoTooltip = 1 << 5,
    AzTabBarFlags_DrawSelectedOverline = 1 << 6,

    AzTabBarFlags_FittingPolicyResizeDown = 1 << 7,

    AzTabBarFlags_FittingPolicyScroll = 1 << 8,

    AzTabBarFlags_FittingPolicyMask_ = AzTabBarFlags_FittingPolicyResizeDown
                                       | AzTabBarFlags_FittingPolicyScroll,
    AzTabBarFlags_FittingPolicyDefault_ = AzTabBarFlags_FittingPolicyResizeDown,
};

enum AzTabItemFlags_
{
    AzTabItemFlags_None = 0,
    AzTabItemFlags_UnsavedDocument = 1 << 0,

    AzTabItemFlags_SetSelected = 1 << 1,

    AzTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2,

    AzTabItemFlags_NoPushId = 1 << 3,

    AzTabItemFlags_NoTooltip = 1 << 4,
    AzTabItemFlags_NoReorder = 1 << 5,

    AzTabItemFlags_Leading = 1 << 6,

    AzTabItemFlags_Trailing = 1 << 7,

    AzTabItemFlags_NoAssumedClosure = 1 << 8,
};

enum AzFocusedFlags_
{
    AzFocusedFlags_None = 0,
    AzFocusedFlags_ChildWindows = 1 << 0,

    AzFocusedFlags_RootWindow = 1 << 1,

    AzFocusedFlags_AnyWindow = 1 << 2,

    AzFocusedFlags_NoPopupHierarchy = 1 << 3,

    AzFocusedFlags_DockHierarchy = 1 << 4,

    AzFocusedFlags_RootAndChildWindows = AzFocusedFlags_RootWindow
                                         | AzFocusedFlags_ChildWindows,
};

enum AzHoveredFlags_
{
    AzHoveredFlags_None = 0,

    AzHoveredFlags_ChildWindows = 1 << 0,

    AzHoveredFlags_RootWindow = 1 << 1,

    AzHoveredFlags_AnyWindow = 1 << 2,

    AzHoveredFlags_NoPopupHierarchy = 1 << 3,

    AzHoveredFlags_DockHierarchy = 1 << 4,

    AzHoveredFlags_AllowWhenBlockedByPopup = 1 << 5,

    AzHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 7,

    AzHoveredFlags_AllowWhenOverlappedByItem = 1 << 8,

    AzHoveredFlags_AllowWhenOverlappedByWindow = 1 << 9,

    AzHoveredFlags_AllowWhenDisabled = 1 << 10,

    AzHoveredFlags_NoNavOverride = 1 << 11,

    AzHoveredFlags_AllowWhenOverlapped =
            AzHoveredFlags_AllowWhenOverlappedByItem
            | AzHoveredFlags_AllowWhenOverlappedByWindow,
    AzHoveredFlags_RectOnly = AzHoveredFlags_AllowWhenBlockedByPopup
                              | AzHoveredFlags_AllowWhenBlockedByActiveItem
                              | AzHoveredFlags_AllowWhenOverlapped,
    AzHoveredFlags_RootAndChildWindows = AzHoveredFlags_RootWindow
                                         | AzHoveredFlags_ChildWindows,

    AzHoveredFlags_ForTooltip = 1 << 12,

    AzHoveredFlags_Stationary = 1 << 13,

    AzHoveredFlags_DelayNone = 1 << 14,

    AzHoveredFlags_DelayShort = 1 << 15,

    AzHoveredFlags_DelayNormal = 1 << 16,

    AzHoveredFlags_NoSharedDelay = 1 << 17,
};

enum AzDockNodeFlags_
{
    AzDockNodeFlags_None = 0,
    AzDockNodeFlags_KeepAliveOnly = 1 << 0,

    AzDockNodeFlags_NoDockingOverCentralNode = 1 << 2,

    AzDockNodeFlags_PassthruCentralNode = 1 << 3,

    AzDockNodeFlags_NoDockingSplit = 1 << 4,

    AzDockNodeFlags_NoResize = 1 << 5,

    AzDockNodeFlags_AutoHideTabBar = 1 << 6,

    AzDockNodeFlags_NoUndocking = 1 << 7,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzDockNodeFlags_NoSplit = AzDockNodeFlags_NoDockingSplit,

    AzDockNodeFlags_NoDockingInCentralNode =
            AzDockNodeFlags_NoDockingOverCentralNode,
#endif
};

enum AzDragDropFlags_
{
    AzDragDropFlags_None = 0,

    AzDragDropFlags_SourceNoPreviewTooltip = 1 << 0,

    AzDragDropFlags_SourceNoDisableHover = 1 << 1,

    AzDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,

    AzDragDropFlags_SourceAllowNullID = 1 << 3,

    AzDragDropFlags_SourceExtern = 1 << 4,

    AzDragDropFlags_PayloadAutoExpire = 1 << 5,

    AzDragDropFlags_PayloadNoCrossContext = 1 << 6,

    AzDragDropFlags_PayloadNoCrossProcess = 1 << 7,

    AzDragDropFlags_AcceptBeforeDelivery = 1 << 10,

    AzDragDropFlags_AcceptNoDrawDefaultRect = 1 << 11,

    AzDragDropFlags_AcceptNoPreviewTooltip = 1 << 12,

    AzDragDropFlags_AcceptPeekOnly = AzDragDropFlags_AcceptBeforeDelivery
                                     | AzDragDropFlags_AcceptNoDrawDefaultRect,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzDragDropFlags_SourceAutoExpirePayload = AzDragDropFlags_PayloadAutoExpire,
#endif
};

#define AZ_PAYLOAD_TYPE_COLOR_3F "_COL3F"

#define AZ_PAYLOAD_TYPE_COLOR_4F "_COL4F"

enum AzDataType_
{
    AzDataType_S8,
    AzDataType_U8,
    AzDataType_S16,
    AzDataType_U16,
    AzDataType_S32,
    AzDataType_U32,
    AzDataType_S64,
    AzDataType_U64,
    AzDataType_Float,
    AzDataType_Double,
    AzDataType_Bool,

    AzDataType_String,

    AzDataType_COUNT
};

enum AzDir : int
{
    AzDir_None = -1,
    AzDir_Left = 0,
    AzDir_Right = 1,
    AzDir_Up = 2,
    AzDir_Down = 3,
    AzDir_COUNT
};

enum AzSortDirection : AzU8
{
    AzSortDirection_None = 0,
    AzSortDirection_Ascending = 1,
    AzSortDirection_Descending = 2
};

enum AzKey : int
{
    AzKey_None = 0,
    AzKey_NamedKey_BEGIN = 512,

    AzKey_Tab = 512,
    AzKey_LeftArrow,
    AzKey_RightArrow,
    AzKey_UpArrow,
    AzKey_DownArrow,
    AzKey_PageUp,
    AzKey_PageDown,
    AzKey_Home,
    AzKey_End,
    AzKey_Insert,
    AzKey_Delete,
    AzKey_Backspace,
    AzKey_Space,
    AzKey_Enter,
    AzKey_Escape,
    AzKey_LeftCtrl,
    AzKey_LeftShift,
    AzKey_LeftAlt,
    AzKey_LeftSuper,
    AzKey_RightCtrl,
    AzKey_RightShift,
    AzKey_RightAlt,
    AzKey_RightSuper,
    AzKey_Menu,
    AzKey_0,
    AzKey_1,
    AzKey_2,
    AzKey_3,
    AzKey_4,
    AzKey_5,
    AzKey_6,
    AzKey_7,
    AzKey_8,
    AzKey_9,
    AzKey_A,
    AzKey_B,
    AzKey_C,
    AzKey_D,
    AzKey_E,
    AzKey_F,
    AzKey_G,
    AzKey_H,
    AzKey_I,
    AzKey_J,
    AzKey_K,
    AzKey_L,
    AzKey_M,
    AzKey_N,
    AzKey_O,
    AzKey_P,
    AzKey_Q,
    AzKey_R,
    AzKey_S,
    AzKey_T,
    AzKey_U,
    AzKey_V,
    AzKey_W,
    AzKey_X,
    AzKey_Y,
    AzKey_Z,
    AzKey_F1,
    AzKey_F2,
    AzKey_F3,
    AzKey_F4,
    AzKey_F5,
    AzKey_F6,
    AzKey_F7,
    AzKey_F8,
    AzKey_F9,
    AzKey_F10,
    AzKey_F11,
    AzKey_F12,
    AzKey_F13,
    AzKey_F14,
    AzKey_F15,
    AzKey_F16,
    AzKey_F17,
    AzKey_F18,
    AzKey_F19,
    AzKey_F20,
    AzKey_F21,
    AzKey_F22,
    AzKey_F23,
    AzKey_F24,
    AzKey_Apostrophe,
    AzKey_Comma,
    AzKey_Minus,
    AzKey_Period,
    AzKey_Slash,
    AzKey_Semicolon,
    AzKey_Equal,
    AzKey_LeftBracket,
    AzKey_Backslash,

    AzKey_RightBracket,
    AzKey_GraveAccent,
    AzKey_CapsLock,
    AzKey_ScrollLock,
    AzKey_NumLock,
    AzKey_PrintScreen,
    AzKey_Pause,
    AzKey_Keypad0,
    AzKey_Keypad1,
    AzKey_Keypad2,
    AzKey_Keypad3,
    AzKey_Keypad4,
    AzKey_Keypad5,
    AzKey_Keypad6,
    AzKey_Keypad7,
    AzKey_Keypad8,
    AzKey_Keypad9,
    AzKey_KeypadDecimal,
    AzKey_KeypadDivide,
    AzKey_KeypadMultiply,
    AzKey_KeypadSubtract,
    AzKey_KeypadAdd,
    AzKey_KeypadEnter,
    AzKey_KeypadEqual,
    AzKey_AppBack,

    AzKey_AppForward,
    AzKey_Oem102,

    AzKey_GamepadStart,
    AzKey_GamepadBack,
    AzKey_GamepadFaceLeft,

    AzKey_GamepadFaceRight,

    AzKey_GamepadFaceUp,

    AzKey_GamepadFaceDown,

    AzKey_GamepadDpadLeft,

    AzKey_GamepadDpadRight,

    AzKey_GamepadDpadUp,

    AzKey_GamepadDpadDown,

    AzKey_GamepadL1,

    AzKey_GamepadR1,

    AzKey_GamepadL2,
    AzKey_GamepadR2,
    AzKey_GamepadL3,
    AzKey_GamepadR3,
    AzKey_GamepadLStickLeft,
    AzKey_GamepadLStickRight,
    AzKey_GamepadLStickUp,
    AzKey_GamepadLStickDown,
    AzKey_GamepadRStickLeft,
    AzKey_GamepadRStickRight,
    AzKey_GamepadRStickUp,
    AzKey_GamepadRStickDown,

    AzKey_MouseLeft,
    AzKey_MouseRight,
    AzKey_MouseMiddle,
    AzKey_MouseX1,
    AzKey_MouseX2,
    AzKey_MouseWheelX,
    AzKey_MouseWheelY,

    AzKey_ReservedForModCtrl,
    AzKey_ReservedForModShift,
    AzKey_ReservedForModAlt,
    AzKey_ReservedForModSuper,
    AzKey_NamedKey_END,

    AzMod_None = 0,
    AzMod_Ctrl = 1 << 12,
    AzMod_Shift = 1 << 13,
    AzMod_Alt = 1 << 14,
    AzMod_Super = 1 << 15,
    AzMod_Mask_ = 0xF000,

    AzKey_NamedKey_COUNT = AzKey_NamedKey_END - AzKey_NamedKey_BEGIN,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzKey_COUNT = AzKey_NamedKey_END,

    AzMod_Shortcut = AzMod_Ctrl,

    AzKey_ModCtrl = AzMod_Ctrl,
    AzKey_ModShift = AzMod_Shift,
    AzKey_ModAlt = AzMod_Alt,
    AzKey_ModSuper = AzMod_Super,

#endif
};

enum AzInputFlags_
{
    AzInputFlags_None = 0,
    AzInputFlags_Repeat = 1 << 0,

    AzInputFlags_RouteActive = 1 << 10,
    AzInputFlags_RouteFocused = 1 << 11,

    AzInputFlags_RouteGlobal = 1 << 12,

    AzInputFlags_RouteAlways = 1 << 13,

    AzInputFlags_RouteOverFocused = 1 << 14,

    AzInputFlags_RouteOverActive = 1 << 15,

    AzInputFlags_RouteUnlessBgFocused = 1 << 16,

    AzInputFlags_RouteFromRootWindow = 1 << 17,

    AzInputFlags_Tooltip = 1 << 18,
};

enum AzConfigFlags_
{
    AzConfigFlags_None = 0,
    AzConfigFlags_NavEnableKeyboard = 1 << 0,

    AzConfigFlags_NavEnableGamepad = 1 << 1,

    AzConfigFlags_NoMouse = 1 << 4,

    AzConfigFlags_NoMouseCursorChange = 1 << 5,

    AzConfigFlags_NoKeyboard = 1 << 6,

    AzConfigFlags_DockingEnable = 1 << 7,

    AzConfigFlags_ViewportsEnable = 1 << 10,

    AzConfigFlags_IsSRGB = 1 << 20,
    AzConfigFlags_IsTouchScreen = 1 << 21,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzConfigFlags_NavEnableSetMousePos = 1 << 2,

    AzConfigFlags_NavNoCaptureKeyboard = 1 << 3,

    AzConfigFlags_DpiEnableScaleFonts = 1 << 14,

    AzConfigFlags_DpiEnableScaleViewports = 1 << 15,

#endif
};

enum AzBackendFlags_
{
    AzBackendFlags_None = 0,
    AzBackendFlags_HasGamepad = 1 << 0,

    AzBackendFlags_HasMouseCursors = 1 << 1,

    AzBackendFlags_HasSetMousePos = 1 << 2,

    AzBackendFlags_RendererHasVtxOffset = 1 << 3,

    AzBackendFlags_RendererHasTextures = 1 << 4,

    AzBackendFlags_PlatformHasViewports = 1 << 10,
    AzBackendFlags_HasMouseHoveredViewport = 1 << 11,

    AzBackendFlags_RendererHasViewports = 1 << 12,
};

enum AzCol_
{
    AzCol_Text,
    AzCol_TextDisabled,
    AzCol_WindowBg,
    AzCol_ChildBg,
    AzCol_PopupBg,
    AzCol_Border,
    AzCol_BorderShadow,
    AzCol_FrameBg,

    AzCol_FrameBgHovered,
    AzCol_FrameBgActive,
    AzCol_TitleBg,
    AzCol_TitleBgActive,
    AzCol_TitleBgCollapsed,
    AzCol_MenuBarBg,
    AzCol_ScrollbarBg,
    AzCol_ScrollbarGrab,
    AzCol_ScrollbarGrabHovered,
    AzCol_ScrollbarGrabActive,
    AzCol_CheckMark,
    AzCol_SliderGrab,
    AzCol_SliderGrabActive,
    AzCol_Button,
    AzCol_ButtonHovered,
    AzCol_ButtonActive,
    AzCol_Header,

    AzCol_HeaderHovered,
    AzCol_HeaderActive,
    AzCol_Separator,
    AzCol_SeparatorHovered,
    AzCol_SeparatorActive,
    AzCol_ResizeGrip,

    AzCol_ResizeGripHovered,
    AzCol_ResizeGripActive,
    AzCol_InputTextCursor,
    AzCol_TabHovered,
    AzCol_Tab,
    AzCol_TabSelected,

    AzCol_TabSelectedOverline,

    AzCol_TabDimmed,

    AzCol_TabDimmedSelected,

    AzCol_TabDimmedSelectedOverline,

    AzCol_DockingPreview,

    AzCol_DockingEmptyBg,

    AzCol_PlotLines,
    AzCol_PlotLinesHovered,
    AzCol_PlotHistogram,
    AzCol_PlotHistogramHovered,
    AzCol_TableHeaderBg,
    AzCol_TableBorderStrong,

    AzCol_TableBorderLight,

    AzCol_TableRowBg,
    AzCol_TableRowBgAlt,
    AzCol_TextLink,
    AzCol_TextSelectedBg,
    AzCol_TreeLines,

    AzCol_DragDropTarget,
    AzCol_NavCursor,

    AzCol_NavWindowingHighlight,
    AzCol_NavWindowingDimBg,

    AzCol_ModalWindowDimBg,

    AzCol_COUNT,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzCol_TabActive = AzCol_TabSelected,
    AzCol_TabUnfocused = AzCol_TabDimmed,
    AzCol_TabUnfocusedActive = AzCol_TabDimmedSelected,

    AzCol_NavHighlight = AzCol_NavCursor,
#endif
};

enum AzStyleVar_
{
    AzStyleVar_Alpha,
    AzStyleVar_DisabledAlpha,
    AzStyleVar_WindowPadding,
    AzStyleVar_WindowRounding,
    AzStyleVar_WindowBorderSize,
    AzStyleVar_WindowMinSize,
    AzStyleVar_WindowTitleAlign,
    AzStyleVar_ChildRounding,
    AzStyleVar_ChildBorderSize,
    AzStyleVar_PopupRounding,
    AzStyleVar_PopupBorderSize,
    AzStyleVar_FramePadding,
    AzStyleVar_FrameRounding,
    AzStyleVar_FrameBorderSize,
    AzStyleVar_ItemSpacing,
    AzStyleVar_ItemInnerSpacing,
    AzStyleVar_IndentSpacing,
    AzStyleVar_CellPadding,
    AzStyleVar_ScrollbarSize,
    AzStyleVar_ScrollbarRounding,
    AzStyleVar_GrabMinSize,
    AzStyleVar_GrabRounding,
    AzStyleVar_AzageBorderSize,
    AzStyleVar_TabRounding,
    AzStyleVar_TabBorderSize,
    AzStyleVar_TabBarBorderSize,
    AzStyleVar_TabBarOverlineSize,
    AzStyleVar_TableAngledHeadersAngle,
    AzStyleVar_TableAngledHeadersTextAlign,

    AzStyleVar_TreeLinesSize,
    AzStyleVar_TreeLinesRounding,
    AzStyleVar_ButtonTextAlign,
    AzStyleVar_SelectableTextAlign,
    AzStyleVar_SeparatorTextBorderSize,
    AzStyleVar_SeparatorTextAlign,
    AzStyleVar_SeparatorTextPadding,
    AzStyleVar_DockingSeparatorSize,
    AzStyleVar_COUNT
};

enum AzButtonFlags_
{
    AzButtonFlags_None = 0,
    AzButtonFlags_MouseButtonLeft = 1 << 0,

    AzButtonFlags_MouseButtonRight = 1 << 1,
    AzButtonFlags_MouseButtonMiddle = 1 << 2,
    AzButtonFlags_MouseButtonMask_ = AzButtonFlags_MouseButtonLeft
                                     | AzButtonFlags_MouseButtonRight
                                     | AzButtonFlags_MouseButtonMiddle,
    AzButtonFlags_EnableNav = 1 << 3,
};

enum AzColorEditFlags_
{
    AzColorEditFlags_None = 0,
    AzColorEditFlags_NoAlpha = 1 << 1,

    AzColorEditFlags_NoPicker = 1 << 2,

    AzColorEditFlags_NoOptions = 1 << 3,

    AzColorEditFlags_NoSmallPreview = 1 << 4,

    AzColorEditFlags_NoInputs = 1 << 5,

    AzColorEditFlags_NoTooltip = 1 << 6,

    AzColorEditFlags_NoLabel = 1 << 7,

    AzColorEditFlags_NoSidePreview = 1 << 8,

    AzColorEditFlags_NoDragDrop = 1 << 9,

    AzColorEditFlags_NoBorder = 1 << 10,

    AzColorEditFlags_AlphaOpaque = 1 << 11,

    AzColorEditFlags_AlphaNoBg = 1 << 12,

    AzColorEditFlags_AlphaPreviewHalf = 1 << 13,

    AzColorEditFlags_AlphaBar = 1 << 16,

    AzColorEditFlags_HDR = 1 << 19,

    AzColorEditFlags_DisplayRGB = 1 << 20,

    AzColorEditFlags_DisplayHSV = 1 << 21,
    AzColorEditFlags_DisplayHex = 1 << 22,
    AzColorEditFlags_Uint8 = 1 << 23,

    AzColorEditFlags_Float = 1 << 24,

    AzColorEditFlags_PickerHueBar = 1 << 25,

    AzColorEditFlags_PickerHueWheel = 1 << 26,

    AzColorEditFlags_InputRGB = 1 << 27,

    AzColorEditFlags_InputHSV = 1 << 28,

    AzColorEditFlags_DefaultOptions_ = AzColorEditFlags_Uint8
                                       | AzColorEditFlags_DisplayRGB
                                       | AzColorEditFlags_InputRGB
                                       | AzColorEditFlags_PickerHueBar,

    AzColorEditFlags_AlphaMask_ = AzColorEditFlags_NoAlpha
                                  | AzColorEditFlags_AlphaOpaque
                                  | AzColorEditFlags_AlphaNoBg
                                  | AzColorEditFlags_AlphaPreviewHalf,
    AzColorEditFlags_DisplayMask_ = AzColorEditFlags_DisplayRGB
                                    | AzColorEditFlags_DisplayHSV
                                    | AzColorEditFlags_DisplayHex,
    AzColorEditFlags_DataTypeMask_ = AzColorEditFlags_Uint8
                                     | AzColorEditFlags_Float,
    AzColorEditFlags_PickerMask_ = AzColorEditFlags_PickerHueWheel
                                   | AzColorEditFlags_PickerHueBar,
    AzColorEditFlags_InputMask_ = AzColorEditFlags_InputRGB
                                  | AzColorEditFlags_InputHSV,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzColorEditFlags_AlphaPreview = 0,

#endif
};

enum AzSliderFlags_
{
    AzSliderFlags_None = 0,
    AzSliderFlags_Logarithmic = 1 << 5,

    AzSliderFlags_NoRoundToFormat = 1 << 6,

    AzSliderFlags_NoInput = 1 << 7,

    AzSliderFlags_WrapAround = 1 << 8,

    AzSliderFlags_ClampOnInput = 1 << 9,

    AzSliderFlags_ClampZeroRange = 1 << 10,

    AzSliderFlags_NoSpeedTweaks = 1 << 11,

    AzSliderFlags_AlwaysClamp = AzSliderFlags_ClampOnInput
                                | AzSliderFlags_ClampZeroRange,
    AzSliderFlags_InvalidMask_ = 0x7000000F,
};

enum AzMouseButton_
{
    AzMouseButton_Left = 0,
    AzMouseButton_Right = 1,
    AzMouseButton_Middle = 2,
    AzMouseButton_COUNT = 5
};

enum AzMouseCursor_
{
    AzMouseCursor_None = -1,
    AzMouseCursor_Arrow = 0,
    AzMouseCursor_TextInput,
    AzMouseCursor_ResizeAll,
    AzMouseCursor_ResizeNS,
    AzMouseCursor_ResizeEW,

    AzMouseCursor_ResizeNESW,

    AzMouseCursor_ResizeNWSE,

    AzMouseCursor_Hand,

    AzMouseCursor_Wait,
    AzMouseCursor_Progress,

    AzMouseCursor_NotAllowed,

    AzMouseCursor_COUNT
};

enum AzMouseSource : int
{
    AzMouseSource_Mouse = 0,
    AzMouseSource_TouchScreen,

    AzMouseSource_Pen,

    AzMouseSource_COUNT
};

enum AzCond_
{
    AzCond_None = 0,

    AzCond_Always = 1 << 0,

    AzCond_Once = 1 << 1,

    AzCond_FirstUseEver = 1 << 2,

    AzCond_Appearing = 1 << 3,
};

enum AzTableFlags_
{
    AzTableFlags_None = 0,
    AzTableFlags_Resizable = 1 << 0,
    AzTableFlags_Reorderable = 1 << 1,

    AzTableFlags_Hideable = 1 << 2,

    AzTableFlags_Sortable = 1 << 3,

    AzTableFlags_NoSavedSettings = 1 << 4,

    AzTableFlags_ContextMenuInBody = 1 << 5,

    AzTableFlags_RowBg = 1 << 6,

    AzTableFlags_BordersInnerH = 1 << 7,

    AzTableFlags_BordersOuterH = 1 << 8,

    AzTableFlags_BordersInnerV = 1 << 9,

    AzTableFlags_BordersOuterV = 1 << 10,

    AzTableFlags_BordersH = AzTableFlags_BordersInnerH
                            | AzTableFlags_BordersOuterH,
    AzTableFlags_BordersV = AzTableFlags_BordersInnerV
                            | AzTableFlags_BordersOuterV,

    AzTableFlags_BordersInner = AzTableFlags_BordersInnerV
                                | AzTableFlags_BordersInnerH,
    AzTableFlags_BordersOuter = AzTableFlags_BordersOuterV
                                | AzTableFlags_BordersOuterH,
    AzTableFlags_Borders = AzTableFlags_BordersInner
                           | AzTableFlags_BordersOuter,

    AzTableFlags_NoBordersInBody = 1 << 11,

    AzTableFlags_NoBordersInBodyUntilResize = 1 << 12,

    AzTableFlags_SizingFixedFit = 1 << 13,

    AzTableFlags_SizingFixedSame = 2 << 13,

    AzTableFlags_SizingStretchProp = 3 << 13,

    AzTableFlags_SizingStretchSame = 4 << 13,

    AzTableFlags_NoHostExtendX = 1 << 16,

    AzTableFlags_NoHostExtendY = 1 << 17,

    AzTableFlags_NoKeepColumnsVisible = 1 << 18,

    AzTableFlags_PreciseWidths = 1 << 19,

    AzTableFlags_NoClip = 1 << 20,

    AzTableFlags_PadOuterX = 1 << 21,

    AzTableFlags_NoPadOuterX = 1 << 22,

    AzTableFlags_NoPadInnerX = 1 << 23,

    AzTableFlags_ScrollX = 1 << 24,

    AzTableFlags_ScrollY = 1 << 25,

    AzTableFlags_SortMulti = 1 << 26,

    AzTableFlags_SortTristate = 1 << 27,

    AzTableFlags_HighlightHoveredColumn = 1 << 28,

    AzTableFlags_SizingMask_ = AzTableFlags_SizingFixedFit
                               | AzTableFlags_SizingFixedSame
                               | AzTableFlags_SizingStretchProp
                               | AzTableFlags_SizingStretchSame,
};

enum AzTableColumnFlags_
{
    AzTableColumnFlags_None = 0,
    AzTableColumnFlags_Disabled = 1 << 0,

    AzTableColumnFlags_DefaultHide = 1 << 1,

    AzTableColumnFlags_DefaultSort = 1 << 2,
    AzTableColumnFlags_WidthStretch = 1 << 3,

    AzTableColumnFlags_WidthFixed = 1 << 4,

    AzTableColumnFlags_NoResize = 1 << 5,
    AzTableColumnFlags_NoReorder = 1 << 6,

    AzTableColumnFlags_NoHide = 1 << 7,

    AzTableColumnFlags_NoClip = 1 << 8,

    AzTableColumnFlags_NoSort = 1 << 9,

    AzTableColumnFlags_NoSortAscending = 1 << 10,
    AzTableColumnFlags_NoSortDescending = 1 << 11,
    AzTableColumnFlags_NoHeaderLabel = 1 << 12,

    AzTableColumnFlags_NoHeaderWidth = 1 << 13,

    AzTableColumnFlags_PreferSortAscending = 1 << 14,

    AzTableColumnFlags_PreferSortDescending = 1 << 15,

    AzTableColumnFlags_IndentEnable = 1 << 16,

    AzTableColumnFlags_IndentDisable = 1 << 17,

    AzTableColumnFlags_AngledHeader = 1 << 18,

    AzTableColumnFlags_IsEnabled = 1 << 24,

    AzTableColumnFlags_IsVisible = 1 << 25,

    AzTableColumnFlags_IsSorted = 1 << 26,

    AzTableColumnFlags_IsHovered = 1 << 27,

    AzTableColumnFlags_WidthMask_ = AzTableColumnFlags_WidthStretch
                                    | AzTableColumnFlags_WidthFixed,
    AzTableColumnFlags_IndentMask_ = AzTableColumnFlags_IndentEnable
                                     | AzTableColumnFlags_IndentDisable,
    AzTableColumnFlags_StatusMask_ = AzTableColumnFlags_IsEnabled
                                     | AzTableColumnFlags_IsVisible
                                     | AzTableColumnFlags_IsSorted
                                     | AzTableColumnFlags_IsHovered,
    AzTableColumnFlags_NoDirectResize_ = 1 << 30,
};

enum AzTableRowFlags_
{
    AzTableRowFlags_None = 0,
    AzTableRowFlags_Headers = 1 << 0,
};

enum AzTableBgTarget_
{
    AzTableBgTarget_None = 0,
    AzTableBgTarget_RowBg0 = 1,

    AzTableBgTarget_RowBg1 = 2,

    AzTableBgTarget_CellBg = 3,
};

struct AzTableSortSpecs
{
    const AzTableColumnSortSpecs* Specs;
    int SpecsCount;

    bool SpecsDirty;

    AzTableSortSpecs()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzTableColumnSortSpecs
{
    AzID ColumnUserID;

    AzS16 ColumnIndex;
    AzS16 SortOrder;

    AzSortDirection SortDirection;

    AzTableColumnSortSpecs()
    {
        memset(this, 0, sizeof(*this));
    }
};

#ifndef AZ_DISABLE_DEBUG_TOOLS
#define AZ_DEBUG_LOG(...) Az::DebugLog(__VA_ARGS__)
#else
#define AZ_DEBUG_LOG(...) ((void)0)
#endif

struct AzNewWrapper
{};
inline void* operator new(size_t, AzNewWrapper, void* ptr)
{
    return ptr;
}
inline void operator delete(void*, AzNewWrapper, void*)
{}
#define AZ_ALLOC(_SIZE) Az::MemAlloc(_SIZE)
#define AZ_FREE(_PTR) Az::MemFree(_PTR)
#define AZ_PLACEMENT_NEW(_PTR) new (AzNewWrapper(), _PTR)
#define AZ_NEW(_TYPE) new (AzNewWrapper(), Az::MemAlloc(sizeof(_TYPE))) _TYPE
template <typename T>
void AZ_DELETE(T* p)
{
    if (p)
    {
        p->~T();
        Az::MemFree(p);
    }
}

AZ_MSVC_RUNTIME_CHECKS_OFF
template <typename T>
struct AzVector
{
    int Size;
    int Capacity;
    T* Data;

    typedef T value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;

    inline AzVector()
    {
        Size = Capacity = 0;
        Data = NULL;
    }
    inline AzVector(const AzVector<T>& src)
    {
        Size = Capacity = 0;
        Data = NULL;
        operator=(src);
    }
    inline AzVector<T>& operator=(const AzVector<T>& src)
    {
        clear();
        resize(src.Size);
        if (src.Data)
            memcpy(Data, src.Data, (size_t)Size * sizeof(T));
        return *this;
    }
    inline ~AzVector()
    {
        if (Data)
            AZ_FREE(Data);
    }

    inline void clear()
    {
        if (Data)
        {
            Size = Capacity = 0;
            AZ_FREE(Data);
            Data = NULL;
        }
    }
    inline void clear_delete()
    {
        for (int n = 0; n < Size; n++)
            AZ_DELETE(Data[n]);
        clear();
    }
    inline void clear_destruct()
    {
        for (int n = 0; n < Size; n++)
            Data[n].~T();
        clear();
    }

    inline bool empty() const
    {
        return Size == 0;
    }
    inline int size() const
    {
        return Size;
    }
    inline int size_in_bytes() const
    {
        return Size * (int)sizeof(T);
    }
    inline int max_size() const
    {
        return 0x7FFFFFFF / (int)sizeof(T);
    }
    inline int capacity() const
    {
        return Capacity;
    }
    inline T& operator[](int i)
    {
        AZ_ASSERT(i >= 0 && i < Size);
        return Data[i];
    }
    inline const T& operator[](int i) const
    {
        AZ_ASSERT(i >= 0 && i < Size);
        return Data[i];
    }

    inline T* begin()
    {
        return Data;
    }
    inline const T* begin() const
    {
        return Data;
    }
    inline T* end()
    {
        return Data + Size;
    }
    inline const T* end() const
    {
        return Data + Size;
    }
    inline T& front()
    {
        AZ_ASSERT(Size > 0);
        return Data[0];
    }
    inline const T& front() const
    {
        AZ_ASSERT(Size > 0);
        return Data[0];
    }
    inline T& back()
    {
        AZ_ASSERT(Size > 0);
        return Data[Size - 1];
    }
    inline const T& back() const
    {
        AZ_ASSERT(Size > 0);
        return Data[Size - 1];
    }
    inline void swap(AzVector<T>& rhs)
    {
        int rhs_size = rhs.Size;
        rhs.Size = Size;
        Size = rhs_size;
        int rhs_cap = rhs.Capacity;
        rhs.Capacity = Capacity;
        Capacity = rhs_cap;
        T* rhs_data = rhs.Data;
        rhs.Data = Data;
        Data = rhs_data;
    }

    inline int _grow_capacity(int sz) const
    {
        int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8;
        return new_capacity > sz ? new_capacity : sz;
    }
    inline void resize(int new_size)
    {
        if (new_size > Capacity)
            reserve(_grow_capacity(new_size));
        Size = new_size;
    }
    inline void resize(int new_size, const T& v)
    {
        if (new_size > Capacity)
            reserve(_grow_capacity(new_size));
        if (new_size > Size)
            for (int n = Size; n < new_size; n++)
                memcpy(&Data[n], &v, sizeof(v));
        Size = new_size;
    }
    inline void shrink(int new_size)
    {
        AZ_ASSERT(new_size <= Size);
        Size = new_size;
    }

    inline void reserve(int new_capacity)
    {
        if (new_capacity <= Capacity)
            return;
        T* new_data = (T*)AZ_ALLOC((size_t)new_capacity * sizeof(T));
        if (Data)
        {
            memcpy(new_data, Data, (size_t)Size * sizeof(T));
            AZ_FREE(Data);
        }
        Data = new_data;
        Capacity = new_capacity;
    }
    inline void reserve_discard(int new_capacity)
    {
        if (new_capacity <= Capacity)
            return;
        if (Data)
            AZ_FREE(Data);
        Data = (T*)AZ_ALLOC((size_t)new_capacity * sizeof(T));
        Capacity = new_capacity;
    }

    inline void push_back(const T& v)
    {
        if (Size == Capacity)
            reserve(_grow_capacity(Size + 1));
        memcpy(&Data[Size], &v, sizeof(v));
        Size++;
    }
    inline void pop_back()
    {
        AZ_ASSERT(Size > 0);
        Size--;
    }
    inline void push_front(const T& v)
    {
        if (Size == 0)
            push_back(v);
        else
            insert(Data, v);
    }
    inline T* erase(const T* it)
    {
        AZ_ASSERT(it >= Data && it < Data + Size);
        const ptrdiff_t off = it - Data;
        memmove(Data + off,
                Data + off + 1,
                ((size_t)Size - (size_t)off - 1) * sizeof(T));
        Size--;
        return Data + off;
    }
    inline T* erase(const T* it, const T* it_last)
    {
        AZ_ASSERT(it >= Data && it < Data + Size && it_last >= it
                  && it_last <= Data + Size);
        const ptrdiff_t count = it_last - it;
        const ptrdiff_t off = it - Data;
        memmove(Data + off,
                Data + off + count,
                ((size_t)Size - (size_t)off - (size_t)count) * sizeof(T));
        Size -= (int)count;
        return Data + off;
    }
    inline T* erase_unsorted(const T* it)
    {
        AZ_ASSERT(it >= Data && it < Data + Size);
        const ptrdiff_t off = it - Data;
        if (it < Data + Size - 1)
            memcpy(Data + off, Data + Size - 1, sizeof(T));
        Size--;
        return Data + off;
    }
    inline T* insert(const T* it, const T& v)
    {
        AZ_ASSERT(it >= Data && it <= Data + Size);
        const ptrdiff_t off = it - Data;
        if (Size == Capacity)
            reserve(_grow_capacity(Size + 1));
        if (off < (int)Size)
            memmove(Data + off + 1,
                    Data + off,
                    ((size_t)Size - (size_t)off) * sizeof(T));
        memcpy(&Data[off], &v, sizeof(v));
        Size++;
        return Data + off;
    }
    inline bool contains(const T& v) const
    {
        const T* data = Data;
        const T* data_end = Data + Size;
        while (data < data_end)
            if (*data++ == v)
                return true;
        return false;
    }
    inline T* find(const T& v)
    {
        T* data = Data;
        const T* data_end = Data + Size;
        while (data < data_end)
            if (*data == v)
                break;
            else
                ++data;
        return data;
    }
    inline const T* find(const T& v) const
    {
        const T* data = Data;
        const T* data_end = Data + Size;
        while (data < data_end)
            if (*data == v)
                break;
            else
                ++data;
        return data;
    }
    inline int find_index(const T& v) const
    {
        const T* data_end = Data + Size;
        const T* it = find(v);
        if (it == data_end)
            return -1;
        const ptrdiff_t off = it - Data;
        return (int)off;
    }
    inline bool find_erase(const T& v)
    {
        const T* it = find(v);
        if (it < Data + Size)
        {
            erase(it);
            return true;
        }
        return false;
    }
    inline bool find_erase_unsorted(const T& v)
    {
        const T* it = find(v);
        if (it < Data + Size)
        {
            erase_unsorted(it);
            return true;
        }
        return false;
    }
    inline int index_from_ptr(const T* it) const
    {
        AZ_ASSERT(it >= Data && it < Data + Size);
        const ptrdiff_t off = it - Data;
        return (int)off;
    }
};
AZ_MSVC_RUNTIME_CHECKS_RESTORE

struct AzStyle
{
    float FontSizeBase;

    float FontScaleMain;

    float FontScaleDpi;

    float Alpha;
    float DisabledAlpha;

    AzVec2 WindowPadding;
    float WindowRounding;

    float WindowBorderSize;

    float WindowBorderHoverPadding;

    AzVec2 WindowMinSize;

    AzVec2 WindowTitleAlign;

    AzDir WindowMenuButtonPosition;

    float ChildRounding;

    float ChildBorderSize;

    float PopupRounding;

    float PopupBorderSize;

    AzVec2 FramePadding;

    float FrameRounding;

    float FrameBorderSize;

    AzVec2 ItemSpacing;

    AzVec2 ItemInnerSpacing;

    AzVec2 CellPadding;

    AzVec2 TouchExtraPadding;

    float IndentSpacing;

    float ColumnsMinSpacing;

    float ScrollbarSize;

    float ScrollbarRounding;
    float GrabMinSize;

    float GrabRounding;

    float LogSliderDeadzone;

    float AzageBorderSize;
    float TabRounding;

    float TabBorderSize;
    float TabCloseButtonMinWidthSelected;

    float TabCloseButtonMinWidthUnselected;

    float TabBarBorderSize;

    float TabBarOverlineSize;

    float TableAngledHeadersAngle;

    AzVec2 TableAngledHeadersTextAlign;

    AzTreeNodeFlags TreeLinesFlags;

    float TreeLinesSize;

    float TreeLinesRounding;

    AzDir ColorButtonPosition;

    AzVec2 ButtonTextAlign;

    AzVec2 SelectableTextAlign;

    float SeparatorTextBorderSize;
    AzVec2 SeparatorTextAlign;

    AzVec2 SeparatorTextPadding;

    AzVec2 DisplayWindowPadding;

    AzVec2 DisplaySafeAreaPadding;

    float DockingSeparatorSize;

    float MouseCursorScale;

    bool AntiAliasedLines;

    bool AntiAliasedLinesUseTex;

    bool AntiAliasedFill;

    float CurveTessellationTol;

    float CircleTessellationMaxError;

    AzVec4 Colors[AzCol_COUNT];

    float HoverStationaryDelay;

    float HoverDelayShort;

    float HoverDelayNormal;

    AzHoveredFlags HoverFlagsForTooltipMouse;

    AzHoveredFlags HoverFlagsForTooltipNav;

    float _MainScale;

    float _NextFrameFontSizeBase;

    AZ_API AzStyle();
    AZ_API void ScaleAllSizes(float scale_factor);

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

#endif
};

struct AzKeyData
{
    bool Down;
    float DownDuration;

    float DownDurationPrev;
    float AnalogValue;
};

struct AzIO
{
    AzConfigFlags ConfigFlags;

    AzBackendFlags BackendFlags;

    AzVec2 DisplaySize;

    AzVec2 DisplayFramebufferScale;

    float DeltaTime;

    float IniSavingRate;

    const char* IniFilename;

    const char* LogFilename;

    void* UserData;

    AzFontAtlas* Fonts;

    AzFont* FontDefault;

    bool FontAllowUserScaling;

    bool ConfigNavSwapGamepadButtons;

    bool ConfigNavMoveSetMousePos;

    bool ConfigNavCaptureKeyboard;

    bool ConfigNavEscapeClearFocusItem;

    bool ConfigNavEscapeClearFocusWindow;

    bool ConfigNavCursorVisibleAuto;

    bool ConfigNavCursorVisibleAlways;

    bool ConfigDockingNoSplit;

    bool ConfigDockingWithShift;

    bool ConfigDockingAlwaysTabBar;

    bool ConfigDockingTransparentPayload;

    bool ConfigViewportsNoAutoMerge;

    bool ConfigViewportsNoTaskBarIcon;

    bool ConfigViewportsNoDecoration;

    bool ConfigViewportsNoDefaultParent;

    bool ConfigDpiScaleFonts;

    bool ConfigDpiScaleViewports;

    bool MouseDrawCursor;

    bool ConfigMacOSXBehaviors;

    bool ConfigInputTrickleEventQueue;

    bool ConfigInputTextCursorBlink;

    bool ConfigInputTextEnterKeepActive;

    bool ConfigDragClickToInputText;

    bool ConfigWindowsResizeFromEdges;

    bool ConfigWindowsMoveFromTitleBarOnly;

    bool ConfigWindowsCopyContentsWithCtrlC;

    bool ConfigScrollbarScrollByPage;

    float ConfigMemoryCompactTimer;

    float MouseDoubleClickTime;

    float MouseDoubleClickMaxDist;

    float MouseDragThreshold;

    float KeyRepeatDelay;

    float KeyRepeatRate;

    bool ConfigErrorRecovery;

    bool ConfigErrorRecoveryEnableAssert;

    bool ConfigErrorRecoveryEnableDebugLog;

    bool ConfigErrorRecoveryEnableTooltip;

    bool ConfigDebugIsDebuggerPresent;

    bool ConfigDebugHighlightIdConflicts;

    bool ConfigDebugHighlightIdConflictsShowItemPicker;

    bool ConfigDebugBeginReturnValueOnce;

    bool ConfigDebugBeginReturnValueLoop;

    bool ConfigDebugIgnoreFocusLoss;

    bool ConfigDebugIniSettings;

    const char* BackendPlatformName;
    const char* BackendRendererName;
    void* BackendPlatformUserData;

    void* BackendRendererUserData;

    void* BackendLanguageUserData;

    AZ_API void AddKeyEvent(AzKey key, bool down);

    AZ_API void AddKeyAnalogEvent(AzKey key, bool down, float v);

    AZ_API void AddMousePosEvent(float x, float y);

    AZ_API void AddMouseButtonEvent(int button, bool down);

    AZ_API void AddMouseWheelEvent(float wheel_x, float wheel_y);

    AZ_API void AddMouseSourceEvent(AzMouseSource source);

    AZ_API void AddMouseViewportEvent(AzID id);

    AZ_API void AddFocusEvent(bool focused);

    AZ_API void AddInputCharacter(unsigned int c);

    AZ_API void AddInputCharacterUTF16(AzWchar16 c);

    AZ_API void AddInputCharactersUTF8(const char* str);

    AZ_API void SetKeyEventNativeData(AzKey key,
                                      int native_keycode,
                                      int native_scancode,
                                      int native_legacy_index = -1);

    AZ_API void SetAppAcceptingEvents(bool accepting_events);

    AZ_API void ClearEventsQueue();
    AZ_API void ClearInputKeys();

    AZ_API void ClearInputMouse();
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AZ_API void ClearInputCharacters();

#endif

    bool WantCaptureMouse;

    bool WantCaptureKeyboard;

    bool WantTextInput;

    bool WantSetMousePos;

    bool WantSaveIniSettings;

    bool NavActive;

    bool NavVisible;

    float Framerate;

    int MetricsRenderVertices;
    int MetricsRenderIndices;

    int MetricsRenderWindows;
    int MetricsActiveWindows;
    AzVec2 MouseDelta;

    AzContext* Ctx;

    AzVec2 MousePos;

    bool MouseDown[5];

    float MouseWheel;

    float MouseWheelH;

    AzMouseSource MouseSource;

    AzID MouseHoveredViewport;

    bool KeyCtrl;
    bool KeyShift;
    bool KeyAlt;
    bool KeySuper;

    AzKeyChord KeyMods;

    AzKeyData KeysData[AzKey_NamedKey_COUNT];

    bool WantCaptureMouseUnlessPopupClose;

    AzVec2 MousePosPrev;

    AzVec2 MouseClickedPos[5];
    double MouseClickedTime[5];

    bool MouseClicked[5];

    bool MouseDoubleClicked[5];

    AzU16 MouseClickedCount[5];

    AzU16 MouseClickedLastCount[5];

    bool MouseReleased[5];
    double MouseReleasedTime[5];

    bool MouseDownOwned[5];

    bool MouseDownOwnedUnlessPopupClose[5];

    bool MouseWheelRequestAxisSwap;

    bool MouseCtrlLeftAsRightClick;

    float MouseDownDuration[5];

    float MouseDownDurationPrev[5];

    AzVec2 MouseDragMaxDistanceAbs[5];

    float MouseDragMaxDistanceSqr[5];

    float PenPressure;

    bool AppFocusLost;
    bool AppAcceptingEvents;
    AzWchar16 InputQueueSurrogate;
    AzVector<AzWchar> InputQueueCharacters;

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    float FontGlobalScale;

    const char* (*GetClipboardTextFn)(void* user_data);
    void (*SetClipboardTextFn)(void* user_data, const char* text);
    void* ClipboardUserData;
#endif

    AZ_API AzIO();
};

struct AzInputTextCallbackData
{
    AzContext* Ctx;
    AzInputTextFlags EventFlag;

    AzInputTextFlags Flags;

    void* UserData;

    AzWchar EventChar;

    AzKey EventKey;

    char* Buf;

    int BufTextLen;

    int BufSize;

    bool BufDirty;

    int CursorPos;

    int SelectionStart;

    int SelectionEnd;

    AZ_API AzInputTextCallbackData();
    AZ_API void DeleteChars(int pos, int bytes_count);
    AZ_API void InsertChars(int pos,
                            const char* text,
                            const char* text_end = NULL);
    void SelectAll()
    {
        SelectionStart = 0;
        SelectionEnd = BufTextLen;
    }
    void ClearSelection()
    {
        SelectionStart = SelectionEnd = BufTextLen;
    }
    bool HasSelection() const
    {
        return SelectionStart != SelectionEnd;
    }
};

struct AzSizeCallbackData
{
    void* UserData;

    AzVec2 Pos;
    AzVec2 CurrentSize;
    AzVec2 DesiredSize;
};

struct AzWindowClass
{
    AzID ClassId;

    AzID ParentViewportId;

    AzID FocusRouteParentWindowId;

    AzViewportFlags ViewportFlagsOverrideSet;

    AzViewportFlags ViewportFlagsOverrideClear;

    AzTabItemFlags TabItemFlagsOverrideSet;

    AzDockNodeFlags DockNodeFlagsOverrideSet;

    bool DockingAlwaysTabBar;

    bool DockingAllowUnclassed;

    AzWindowClass()
    {
        memset(this, 0, sizeof(*this));
        ParentViewportId = (AzID)-1;
        DockingAllowUnclassed = true;
    }
};

struct AzPayload
{
    void* Data;
    int DataSize;

    AzID SourceId;
    AzID SourceParentId;
    int DataFrameCount;
    char DataType[32 + 1];

    bool Preview;

    bool Delivery;

    AzPayload()
    {
        Clear();
    }
    void Clear()
    {
        SourceId = SourceParentId = 0;
        Data = NULL;
        DataSize = 0;
        memset(DataType, 0, sizeof(DataType));
        DataFrameCount = -1;
        Preview = Delivery = false;
    }
    bool IsDataType(const char* type) const
    {
        return DataFrameCount != -1 && strcmp(type, DataType) == 0;
    }
    bool IsPreview() const
    {
        return Preview;
    }
    bool IsDelivery() const
    {
        return Delivery;
    }
};

#define AZ_UNICODE_CODEPOINT_INVALID 0xFFFD
#ifdef AZ_USE_WCHAR32
#define AZ_UNICODE_CODEPOINT_MAX 0x10FFFF
#else
#define AZ_UNICODE_CODEPOINT_MAX 0xFFFF
#endif

struct AzOnceUponAFrame
{
    AzOnceUponAFrame()
    {
        RefFrame = -1;
    }
    mutable int RefFrame;
    operator bool() const
    {
        int current_frame = Az::GetFrameCount();
        if (RefFrame == current_frame)
            return false;
        RefFrame = current_frame;
        return true;
    }
};

struct AzTextFilter
{
    AZ_API AzTextFilter(const char* default_filter = "");
    AZ_API bool Draw(const char* label = "Filter (inc,-exc)",
                     float width = 0.0f);
    AZ_API bool PassFilter(const char* text, const char* text_end = NULL) const;
    AZ_API void Build();
    void Clear()
    {
        InputBuf[0] = 0;
        Build();
    }
    bool IsActive() const
    {
        return !Filters.empty();
    }

    struct AzTextRange
    {
        const char* b;
        const char* e;

        AzTextRange()
        {
            b = e = NULL;
        }
        AzTextRange(const char* _b, const char* _e)
        {
            b = _b;
            e = _e;
        }
        bool empty() const
        {
            return b == e;
        }
        AZ_API void split(char separator, AzVector<AzTextRange>* out) const;
    };
    char InputBuf[256];
    AzVector<AzTextRange> Filters;
    int CountGrep;
};

struct AzTextBuffer
{
    AzVector<char> Buf;
    AZ_API static char EmptyString[1];

    AzTextBuffer()
    {}
    inline char operator[](int i) const
    {
        AZ_ASSERT(Buf.Data != NULL);
        return Buf.Data[i];
    }
    const char* begin() const
    {
        return Buf.Data ? &Buf.front() : EmptyString;
    }
    const char* end() const
    {
        return Buf.Data ? &Buf.back() : EmptyString;
    }
    int size() const
    {
        return Buf.Size ? Buf.Size - 1 : 0;
    }
    bool empty() const
    {
        return Buf.Size <= 1;
    }
    void clear()
    {
        Buf.clear();
    }
    void resize(int size)
    {
        if (Buf.Size > size)
            Buf.Data[size] = 0;
        Buf.resize(size ? size + 1 : 0, 0);
    }
    void reserve(int capacity)
    {
        Buf.reserve(capacity);
    }
    const char* c_str() const
    {
        return Buf.Data ? Buf.Data : EmptyString;
    }
    AZ_API void append(const char* str, const char* str_end = NULL);
    AZ_API void appendf(const char* fmt, ...) AZ_FMTARGS(2);
    AZ_API void appendfv(const char* fmt, va_list args) AZ_FMTLIST(2);
};

struct AzStoragePair
{
    AzID key;
    union
    {
        int val_i;
        float val_f;
        void* val_p;
    };
    AzStoragePair(AzID _key, int _val)
    {
        key = _key;
        val_i = _val;
    }
    AzStoragePair(AzID _key, float _val)
    {
        key = _key;
        val_f = _val;
    }
    AzStoragePair(AzID _key, void* _val)
    {
        key = _key;
        val_p = _val;
    }
};

struct AzStorage
{
    AzVector<AzStoragePair> Data;

    void Clear()
    {
        Data.clear();
    }
    AZ_API int GetInt(AzID key, int default_val = 0) const;
    AZ_API void SetInt(AzID key, int val);
    AZ_API bool GetBool(AzID key, bool default_val = false) const;
    AZ_API void SetBool(AzID key, bool val);
    AZ_API float GetFloat(AzID key, float default_val = 0.0f) const;
    AZ_API void SetFloat(AzID key, float val);
    AZ_API void* GetVoidPtr(AzID key) const;
    AZ_API void SetVoidPtr(AzID key, void* val);

    AZ_API int* GetIntRef(AzID key, int default_val = 0);
    AZ_API bool* GetBoolRef(AzID key, bool default_val = false);
    AZ_API float* GetFloatRef(AzID key, float default_val = 0.0f);
    AZ_API void** GetVoidPtrRef(AzID key, void* default_val = NULL);

    AZ_API void BuildSortByKey();

    AZ_API void SetAllInt(int val);

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

#endif
};

struct AzListClipper
{
    AzContext* Ctx;
    int DisplayStart;
    int DisplayEnd;
    int ItemsCount;
    float ItemsHeight;

    double StartPosY;

    double StartSeekOffsetY;

    void* TempData;

    AZ_API AzListClipper();
    AZ_API ~AzListClipper();
    AZ_API void Begin(int items_count, float items_height = -1.0f);
    AZ_API void End();

    AZ_API bool Step();

    inline void IncludeItemByIndex(int item_index)
    {
        IncludeItemsByIndex(item_index, item_index + 1);
    }
    AZ_API void IncludeItemsByIndex(int item_begin, int item_end);

    AZ_API void SeekCursorForItem(int item_index);

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    inline void IncludeRangeByIndices(int item_begin, int item_end)
    {
        IncludeItemsByIndex(item_begin, item_end);
    }

#endif
};

#ifdef AZ_DEFINE_MATH_OPERATORS
#define AZ_DEFINE_MATH_OPERATORS_AZPLEMENTED
AZ_MSVC_RUNTIME_CHECKS_OFF

static inline AzVec2 operator*(const AzVec2& lhs, const float rhs)
{
    return AzVec2(lhs.x * rhs, lhs.y * rhs);
}
static inline AzVec2 operator/(const AzVec2& lhs, const float rhs)
{
    return AzVec2(lhs.x / rhs, lhs.y / rhs);
}
static inline AzVec2 operator+(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
static inline AzVec2 operator-(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
static inline AzVec2 operator*(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}
static inline AzVec2 operator/(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x / rhs.x, lhs.y / rhs.y);
}
static inline AzVec2 operator-(const AzVec2& lhs)
{
    return AzVec2(-lhs.x, -lhs.y);
}
static inline AzVec2& operator*=(AzVec2& lhs, const float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}
static inline AzVec2& operator/=(AzVec2& lhs, const float rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}
static inline AzVec2& operator+=(AzVec2& lhs, const AzVec2& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}
static inline AzVec2& operator-=(AzVec2& lhs, const AzVec2& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}
static inline AzVec2& operator*=(AzVec2& lhs, const AzVec2& rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}
static inline AzVec2& operator/=(AzVec2& lhs, const AzVec2& rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}
static inline bool operator==(const AzVec2& lhs, const AzVec2& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
static inline bool operator!=(const AzVec2& lhs, const AzVec2& rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

static inline AzVec4 operator*(const AzVec4& lhs, const float rhs)
{
    return AzVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}
static inline AzVec4 operator/(const AzVec4& lhs, const float rhs)
{
    return AzVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}
static inline AzVec4 operator+(const AzVec4& lhs, const AzVec4& rhs)
{
    return AzVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
static inline AzVec4 operator-(const AzVec4& lhs, const AzVec4& rhs)
{
    return AzVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
static inline AzVec4 operator*(const AzVec4& lhs, const AzVec4& rhs)
{
    return AzVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
static inline AzVec4 operator/(const AzVec4& lhs, const AzVec4& rhs)
{
    return AzVec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
static inline AzVec4 operator-(const AzVec4& lhs)
{
    return AzVec4(-lhs.x, -lhs.y, -lhs.z, -lhs.w);
}
static inline bool operator==(const AzVec4& lhs, const AzVec4& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}
static inline bool operator!=(const AzVec4& lhs, const AzVec4& rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE
#endif

#ifndef AZ_COL32_R_SHIFT
#ifdef AZ_USE_BGRA_PACKED_COLOR
#define AZ_COL32_R_SHIFT 16
#define AZ_COL32_G_SHIFT 8
#define AZ_COL32_B_SHIFT 0
#define AZ_COL32_A_SHIFT 24
#define AZ_COL32_A_MASK 0xFF000000
#else
#define AZ_COL32_R_SHIFT 0
#define AZ_COL32_G_SHIFT 8
#define AZ_COL32_B_SHIFT 16
#define AZ_COL32_A_SHIFT 24
#define AZ_COL32_A_MASK 0xFF000000
#endif
#endif
#define AZ_COL32(R, G, B, A)                                                   \
    (((AzU32)(A) << AZ_COL32_A_SHIFT) | ((AzU32)(B) << AZ_COL32_B_SHIFT)       \
     | ((AzU32)(G) << AZ_COL32_G_SHIFT) | ((AzU32)(R) << AZ_COL32_R_SHIFT))
#define AZ_COL32_WHITE AZ_COL32(255, 255, 255, 255)
#define AZ_COL32_BLACK AZ_COL32(0, 0, 0, 255)
#define AZ_COL32_BLACK_TRANS AZ_COL32(0, 0, 0, 0)

struct AzColor
{
    AzVec4 Value;

    constexpr AzColor()
    {}
    constexpr AzColor(float r, float g, float b, float a = 1.0f)
        : Value(r, g, b, a)
    {}
    constexpr AzColor(const AzVec4& col) : Value(col)
    {}
    constexpr AzColor(int r, int g, int b, int a = 255)
        : Value((float)r * (1.0f / 255.0f),
                (float)g * (1.0f / 255.0f),
                (float)b * (1.0f / 255.0f),
                (float)a * (1.0f / 255.0f))
    {}
    constexpr AzColor(AzU32 rgba)
        : Value((float)((rgba >> AZ_COL32_R_SHIFT) & 0xFF) * (1.0f / 255.0f),
                (float)((rgba >> AZ_COL32_G_SHIFT) & 0xFF) * (1.0f / 255.0f),
                (float)((rgba >> AZ_COL32_B_SHIFT) & 0xFF) * (1.0f / 255.0f),
                (float)((rgba >> AZ_COL32_A_SHIFT) & 0xFF) * (1.0f / 255.0f))
    {}
    inline operator AzU32() const
    {
        return Az::ColorConvertFloat4ToU32(Value);
    }
    inline operator AzVec4() const
    {
        return Value;
    }

    inline void SetHSV(float h, float s, float v, float a = 1.0f)
    {
        Az::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z);
        Value.w = a;
    }
    static AzColor HSV(float h, float s, float v, float a = 1.0f)
    {
        float r, g, b;
        Az::ColorConvertHSVtoRGB(h, s, v, r, g, b);
        return AzColor(r, g, b, a);
    }
};

enum AzMultiSelectFlags_
{
    AzMultiSelectFlags_None = 0,
    AzMultiSelectFlags_SingleSelect = 1 << 0,

    AzMultiSelectFlags_NoSelectAll = 1 << 1,

    AzMultiSelectFlags_NoRangeSelect = 1 << 2,

    AzMultiSelectFlags_NoAutoSelect = 1 << 3,

    AzMultiSelectFlags_NoAutoClear = 1 << 4,

    AzMultiSelectFlags_NoAutoClearOnReselect = 1 << 5,

    AzMultiSelectFlags_BoxSelect1d = 1 << 6,

    AzMultiSelectFlags_BoxSelect2d = 1 << 7,

    AzMultiSelectFlags_BoxSelectNoScroll = 1 << 8,
    AzMultiSelectFlags_ClearOnEscape = 1 << 9,

    AzMultiSelectFlags_ClearOnClickVoid = 1 << 10,

    AzMultiSelectFlags_ScopeWindow = 1 << 11,

    AzMultiSelectFlags_ScopeRect = 1 << 12,

    AzMultiSelectFlags_SelectOnClick = 1 << 13,

    AzMultiSelectFlags_SelectOnClickRelease = 1 << 14,

    AzMultiSelectFlags_NavWrapX = 1 << 16,
};

struct AzMultiSelectIO
{
    AzVector<AzSelectionRequest> Requests;

    AzSelectionUserData RangeSrcItem;

    AzSelectionUserData NavIdItem;

    bool NavIdSelected;

    bool RangeSrcReset;

    int ItemsCount;
};

enum AzSelectionRequestType
{
    AzSelectionRequestType_None = 0,
    AzSelectionRequestType_SetAll,

    AzSelectionRequestType_SetRange,
};

struct AzSelectionRequest
{
    AzSelectionRequestType Type;

    bool Selected;

    AzS8 RangeDirection;

    AzSelectionUserData RangeFirstItem;

    AzSelectionUserData RangeLastItem;
};

struct AzSelectionBasicStorage
{
    int Size;

    bool PreserveOrder;

    void* UserData;

    AzID (*AdapterIndexToStorageId)(AzSelectionBasicStorage* self, int idx);

    int _SelectionOrder;

    AzStorage _Storage;

    AZ_API AzSelectionBasicStorage();
    AZ_API void ApplyRequests(AzMultiSelectIO* ms_io);

    AZ_API bool Contains(AzID id) const;

    AZ_API void Clear();
    AZ_API void Swap(AzSelectionBasicStorage& r);
    AZ_API void SetItemSelected(AzID id, bool selected);

    AZ_API bool GetNextSelectedItem(void** opaque_it, AzID* out_id);

    inline AzID GetStorageIdFromIndex(int idx)
    {
        return AdapterIndexToStorageId(this, idx);
    }
};

struct AzSelectionExternalStorage
{
    void* UserData;

    void (*AdapterSetItemSelected)(AzSelectionExternalStorage* self,
                                   int idx,
                                   bool selected);

    AZ_API AzSelectionExternalStorage();
    AZ_API void ApplyRequests(AzMultiSelectIO* ms_io);
};

#ifndef AZ_DRAWLIST_TEX_LINES_WIDTH_MAX
#define AZ_DRAWLIST_TEX_LINES_WIDTH_MAX (32)
#endif

#ifndef AzDrawIdx
typedef unsigned short AzDrawIdx;

#endif

#ifndef AzDrawCallback
typedef void (*AzDrawCallback)(const AzDrawList* parent_list,
                               const AzDrawCmd* cmd);
#endif

#define AzDrawCallback_ResetRenderState (AzDrawCallback)(-8)

struct AzDrawCmd
{
    AzVec4 ClipRect;

    AzTextureRef TexRef;

    unsigned int VtxOffset;

    unsigned int IdxOffset;
    unsigned int ElemCount;

    AzDrawCallback UserCallback;

    void* UserCallbackData;

    int UserCallbackDataSize;

    int UserCallbackDataOffset;

    AzDrawCmd()
    {
        memset(this, 0, sizeof(*this));
    }

    inline AzTextureID GetTexID() const;
};

#ifndef AZ_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct AzDrawVert
{
    AzVec2 pos;
    AzVec2 uv;
    AzU32 col;
};
#else

AZ_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

struct AzDrawCmdHeader
{
    AzVec4 ClipRect;
    AzTextureRef TexRef;
    unsigned int VtxOffset;
};

struct AzDrawChannel
{
    AzVector<AzDrawCmd> _CmdBuffer;
    AzVector<AzDrawIdx> _IdxBuffer;
};

struct AzDrawListSplitter
{
    int _Current;
    int _Count;
    AzVector<AzDrawChannel> _Channels;

    inline AzDrawListSplitter()
    {
        memset(this, 0, sizeof(*this));
    }
    inline ~AzDrawListSplitter()
    {
        ClearFreeMemory();
    }
    inline void Clear()
    {
        _Current = 0;
        _Count = 1;
    }
    AZ_API void ClearFreeMemory();
    AZ_API void Split(AzDrawList* draw_list, int count);
    AZ_API void Merge(AzDrawList* draw_list);
    AZ_API void SetCurrentChannel(AzDrawList* draw_list, int channel_idx);
};

enum AzDrawFlags_
{
    AzDrawFlags_None = 0,
    AzDrawFlags_Closed = 1 << 0,

    AzDrawFlags_RoundCornersTopLeft = 1 << 4,

    AzDrawFlags_RoundCornersTopRight = 1 << 5,

    AzDrawFlags_RoundCornersBottomLeft = 1 << 6,

    AzDrawFlags_RoundCornersBottomRight = 1 << 7,

    AzDrawFlags_RoundCornersNone = 1 << 8,

    AzDrawFlags_RoundCornersTop = AzDrawFlags_RoundCornersTopLeft
                                  | AzDrawFlags_RoundCornersTopRight,
    AzDrawFlags_RoundCornersBottom = AzDrawFlags_RoundCornersBottomLeft
                                     | AzDrawFlags_RoundCornersBottomRight,
    AzDrawFlags_RoundCornersLeft = AzDrawFlags_RoundCornersBottomLeft
                                   | AzDrawFlags_RoundCornersTopLeft,
    AzDrawFlags_RoundCornersRight = AzDrawFlags_RoundCornersBottomRight
                                    | AzDrawFlags_RoundCornersTopRight,
    AzDrawFlags_RoundCornersAll = AzDrawFlags_RoundCornersTopLeft
                                  | AzDrawFlags_RoundCornersTopRight
                                  | AzDrawFlags_RoundCornersBottomLeft
                                  | AzDrawFlags_RoundCornersBottomRight,
    AzDrawFlags_RoundCornersDefault_ = AzDrawFlags_RoundCornersAll,

    AzDrawFlags_RoundCornersMask_ = AzDrawFlags_RoundCornersAll
                                    | AzDrawFlags_RoundCornersNone,
};

enum AzDrawListFlags_
{
    AzDrawListFlags_None = 0,
    AzDrawListFlags_AntiAliasedLines = 1 << 0,

    AzDrawListFlags_AntiAliasedLinesUseTex = 1 << 1,

    AzDrawListFlags_AntiAliasedFill = 1 << 2,

    AzDrawListFlags_AllowVtxOffset = 1 << 3,
};

struct AzDrawList
{
    AzVector<AzDrawCmd> CmdBuffer;

    AzVector<AzDrawIdx> IdxBuffer;

    AzVector<AzDrawVert> VtxBuffer;
    AzDrawListFlags Flags;

    unsigned int _VtxCurrentIdx;

    AzDrawListSharedData* _Data;

    AzDrawVert* _VtxWritePtr;

    AzDrawIdx* _IdxWritePtr;

    AzVector<AzVec2> _Path;
    AzDrawCmdHeader _CmdHeader;

    AzDrawListSplitter _Splitter;

    AzVector<AzVec4> _ClipRectStack;
    AzVector<AzTextureRef> _TextureStack;
    AzVector<AzU8> _CallbacksDataBuf;
    float _FringeScale;

    const char* _OwnerName;

    AZ_API AzDrawList(AzDrawListSharedData* shared_data);
    AZ_API ~AzDrawList();

    AZ_API void PushClipRect(const AzVec2& clip_rect_min,
                             const AzVec2& clip_rect_max,
                             bool intersect_with_current_clip_rect = false);

    AZ_API void PushClipRectFullScreen();
    AZ_API void PopClipRect();
    AZ_API void PushTexture(AzTextureRef tex_ref);
    AZ_API void PopTexture();
    inline AzVec2 GetClipRectMin() const
    {
        const AzVec4& cr = _ClipRectStack.back();
        return AzVec2(cr.x, cr.y);
    }
    inline AzVec2 GetClipRectMax() const
    {
        const AzVec4& cr = _ClipRectStack.back();
        return AzVec2(cr.z, cr.w);
    }

    AZ_API void AddLine(const AzVec2& p1,
                        const AzVec2& p2,
                        AzU32 col,
                        float thickness = 1.0f);
    AZ_API void AddRect(const AzVec2& p_min,
                        const AzVec2& p_max,
                        AzU32 col,
                        float rounding = 0.0f,
                        AzDrawFlags flags = 0,
                        float thickness = 1.0f);

    AZ_API void AddRectFilled(const AzVec2& p_min,
                              const AzVec2& p_max,
                              AzU32 col,
                              float rounding = 0.0f,
                              AzDrawFlags flags = 0);

    AZ_API void AddRectFilledMultiColor(const AzVec2& p_min,
                                        const AzVec2& p_max,
                                        AzU32 col_upr_left,
                                        AzU32 col_upr_right,
                                        AzU32 col_bot_right,
                                        AzU32 col_bot_left);
    AZ_API void AddQuad(const AzVec2& p1,
                        const AzVec2& p2,
                        const AzVec2& p3,
                        const AzVec2& p4,
                        AzU32 col,
                        float thickness = 1.0f);
    AZ_API void AddQuadFilled(const AzVec2& p1,
                              const AzVec2& p2,
                              const AzVec2& p3,
                              const AzVec2& p4,
                              AzU32 col);
    AZ_API void AddTriangle(const AzVec2& p1,
                            const AzVec2& p2,
                            const AzVec2& p3,
                            AzU32 col,
                            float thickness = 1.0f);
    AZ_API void AddTriangleFilled(const AzVec2& p1,
                                  const AzVec2& p2,
                                  const AzVec2& p3,
                                  AzU32 col);
    AZ_API void AddCircle(const AzVec2& center,
                          float radius,
                          AzU32 col,
                          int num_segments = 0,
                          float thickness = 1.0f);
    AZ_API void AddCircleFilled(const AzVec2& center,
                                float radius,
                                AzU32 col,
                                int num_segments = 0);
    AZ_API void AddNgon(const AzVec2& center,
                        float radius,
                        AzU32 col,
                        int num_segments,
                        float thickness = 1.0f);
    AZ_API void AddNgonFilled(const AzVec2& center,
                              float radius,
                              AzU32 col,
                              int num_segments);
    AZ_API void AddEllipse(const AzVec2& center,
                           const AzVec2& radius,
                           AzU32 col,
                           float rot = 0.0f,
                           int num_segments = 0,
                           float thickness = 1.0f);
    AZ_API void AddEllipseFilled(const AzVec2& center,
                                 const AzVec2& radius,
                                 AzU32 col,
                                 float rot = 0.0f,
                                 int num_segments = 0);
    AZ_API void AddText(const AzVec2& pos,
                        AzU32 col,
                        const char* text_begin,
                        const char* text_end = NULL);
    AZ_API void AddText(AzFont* font,
                        float font_size,
                        const AzVec2& pos,
                        AzU32 col,
                        const char* text_begin,
                        const char* text_end = NULL,
                        float wrap_width = 0.0f,
                        const AzVec4* cpu_fine_clip_rect = NULL);
    AZ_API void AddBezierCubic(const AzVec2& p1,
                               const AzVec2& p2,
                               const AzVec2& p3,
                               const AzVec2& p4,
                               AzU32 col,
                               float thickness,
                               int num_segments = 0);

    AZ_API void AddBezierQuadratic(const AzVec2& p1,
                                   const AzVec2& p2,
                                   const AzVec2& p3,
                                   AzU32 col,
                                   float thickness,
                                   int num_segments = 0);

    AZ_API void AddPolyline(const AzVec2* points,
                            int num_points,
                            AzU32 col,
                            AzDrawFlags flags,
                            float thickness);
    AZ_API void AddConvexPolyFilled(const AzVec2* points,
                                    int num_points,
                                    AzU32 col);
    AZ_API void AddConcavePolyFilled(const AzVec2* points,
                                     int num_points,
                                     AzU32 col);

    AZ_API void AddAzage(AzTextureRef tex_ref,
                         const AzVec2& p_min,
                         const AzVec2& p_max,
                         const AzVec2& uv_min = AzVec2(0, 0),
                         const AzVec2& uv_max = AzVec2(1, 1),
                         AzU32 col = AZ_COL32_WHITE);
    AZ_API void AddAzageQuad(AzTextureRef tex_ref,
                             const AzVec2& p1,
                             const AzVec2& p2,
                             const AzVec2& p3,
                             const AzVec2& p4,
                             const AzVec2& uv1 = AzVec2(0, 0),
                             const AzVec2& uv2 = AzVec2(1, 0),
                             const AzVec2& uv3 = AzVec2(1, 1),
                             const AzVec2& uv4 = AzVec2(0, 1),
                             AzU32 col = AZ_COL32_WHITE);
    AZ_API void AddAzageRounded(AzTextureRef tex_ref,
                                const AzVec2& p_min,
                                const AzVec2& p_max,
                                const AzVec2& uv_min,
                                const AzVec2& uv_max,
                                AzU32 col,
                                float rounding,
                                AzDrawFlags flags = 0);

    inline void PathClear()
    {
        _Path.Size = 0;
    }
    inline void PathLineTo(const AzVec2& pos)
    {
        _Path.push_back(pos);
    }
    inline void PathLineToMergeDuplicate(const AzVec2& pos)
    {
        if (_Path.Size == 0
            || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0)
            _Path.push_back(pos);
    }
    inline void PathFillConvex(AzU32 col)
    {
        AddConvexPolyFilled(_Path.Data, _Path.Size, col);
        _Path.Size = 0;
    }
    inline void PathFillConcave(AzU32 col)
    {
        AddConcavePolyFilled(_Path.Data, _Path.Size, col);
        _Path.Size = 0;
    }
    inline void PathStroke(AzU32 col,
                           AzDrawFlags flags = 0,
                           float thickness = 1.0f)
    {
        AddPolyline(_Path.Data, _Path.Size, col, flags, thickness);
        _Path.Size = 0;
    }
    AZ_API void PathArcTo(const AzVec2& center,
                          float radius,
                          float a_min,
                          float a_max,
                          int num_segments = 0);
    AZ_API void PathArcToFast(const AzVec2& center,
                              float radius,
                              int a_min_of_12,
                              int a_max_of_12);

    AZ_API void PathEllipticalArcTo(const AzVec2& center,
                                    const AzVec2& radius,
                                    float rot,
                                    float a_min,
                                    float a_max,
                                    int num_segments = 0);
    AZ_API void PathBezierCubicCurveTo(const AzVec2& p2,
                                       const AzVec2& p3,
                                       const AzVec2& p4,
                                       int num_segments = 0);
    AZ_API void PathBezierQuadraticCurveTo(const AzVec2& p2,
                                           const AzVec2& p3,
                                           int num_segments = 0);
    AZ_API void PathRect(const AzVec2& rect_min,
                         const AzVec2& rect_max,
                         float rounding = 0.0f,
                         AzDrawFlags flags = 0);

    AZ_API void AddCallback(AzDrawCallback callback,
                            void* userdata,
                            size_t userdata_size = 0);

    AZ_API void AddDrawCmd();

    AZ_API AzDrawList* CloneOutput() const;

    inline void ChannelsSplit(int count)
    {
        _Splitter.Split(this, count);
    }
    inline void ChannelsMerge()
    {
        _Splitter.Merge(this);
    }
    inline void ChannelsSetCurrent(int n)
    {
        _Splitter.SetCurrentChannel(this, n);
    }

    AZ_API void PrimReserve(int idx_count, int vtx_count);
    AZ_API void PrimUnreserve(int idx_count, int vtx_count);
    AZ_API void PrimRect(const AzVec2& a, const AzVec2& b, AzU32 col);

    AZ_API void PrimRectUV(const AzVec2& a,
                           const AzVec2& b,
                           const AzVec2& uv_a,
                           const AzVec2& uv_b,
                           AzU32 col);
    AZ_API void PrimQuadUV(const AzVec2& a,
                           const AzVec2& b,
                           const AzVec2& c,
                           const AzVec2& d,
                           const AzVec2& uv_a,
                           const AzVec2& uv_b,
                           const AzVec2& uv_c,
                           const AzVec2& uv_d,
                           AzU32 col);
    inline void PrimWriteVtx(const AzVec2& pos, const AzVec2& uv, AzU32 col)
    {
        _VtxWritePtr->pos = pos;
        _VtxWritePtr->uv = uv;
        _VtxWritePtr->col = col;
        _VtxWritePtr++;
        _VtxCurrentIdx++;
    }
    inline void PrimWriteIdx(AzDrawIdx idx)
    {
        *_IdxWritePtr = idx;
        _IdxWritePtr++;
    }
    inline void PrimVtx(const AzVec2& pos, const AzVec2& uv, AzU32 col)
    {
        PrimWriteIdx((AzDrawIdx)_VtxCurrentIdx);
        PrimWriteVtx(pos, uv, col);
    }

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AZ_API void PushTextureID(AzTextureRef tex_ref)
    {
        PushTexture(tex_ref);
    }
    AZ_API void PopTextureID()
    {
        PopTexture();
    }
#endif

    AZ_API void _SetDrawListSharedData(AzDrawListSharedData* data);
    AZ_API void _ResetForNewFrame();
    AZ_API void _ClearFreeMemory();
    AZ_API void _PopUnusedDrawCmd();
    AZ_API void _TryMergeDrawCmds();
    AZ_API void _OnChangedClipRect();
    AZ_API void _OnChangedTexture();
    AZ_API void _OnChangedVtxOffset();
    AZ_API void _SetTexture(AzTextureRef tex_ref);
    AZ_API int _CalcCircleAutoSegmentCount(float radius) const;
    AZ_API void _PathArcToFastEx(const AzVec2& center,
                                 float radius,
                                 int a_min_sample,
                                 int a_max_sample,
                                 int a_step);
    AZ_API void _PathArcToN(const AzVec2& center,
                            float radius,
                            float a_min,
                            float a_max,
                            int num_segments);
};

struct AzDrawData
{
    bool Valid;

    int CmdListsCount;

    int TotalIdxCount;

    int TotalVtxCount;

    AzVector<AzDrawList*> CmdLists;

    AzVec2 DisplayPos;

    AzVec2 DisplaySize;

    AzVec2 FramebufferScale;

    AzViewport* OwnerViewport;

    AzVector<AzTextureData*>* Textures;

    AzDrawData()
    {
        Clear();
    }
    AZ_API void Clear();
    AZ_API void AddDrawList(AzDrawList* draw_list);

    AZ_API void DeIndexAllBuffers();

    AZ_API void ScaleClipRects(const AzVec2& fb_scale);
};

enum AzTextureFormat
{
    AzTextureFormat_RGBA32,

    AzTextureFormat_Alpha8,
};

enum AzTextureStatus
{
    AzTextureStatus_OK,
    AzTextureStatus_Destroyed,
    AzTextureStatus_WantCreate,

    AzTextureStatus_WantUpdates,

    AzTextureStatus_WantDestroy,
};

struct AzTextureRect
{
    unsigned short x, y;
    unsigned short w, h;
};

struct AzTextureData
{
    int UniqueID;

    AzTextureStatus Status;

    void* BackendUserData;

    AzTextureID TexID;

    AzTextureFormat Format;

    int Width;
    int Height;
    int BytesPerPixel;
    unsigned char* Pixels;

    AzTextureRect UsedRect;

    AzTextureRect UpdateRect;

    AzVector<AzTextureRect> Updates;
    int UnusedFrames;

    unsigned short RefCount;

    bool UseColors;

    bool WantDestroyNextFrame;

    AzTextureData()
    {
        memset(this, 0, sizeof(*this));
    }
    ~AzTextureData()
    {
        DestroyPixels();
    }
    AZ_API void Create(AzTextureFormat format, int w, int h);
    AZ_API void DestroyPixels();
    unsigned char* GetPixels()
    {
        AZ_ASSERT(Pixels != NULL);
        return Pixels;
    }
    unsigned char* GetPixelsAt(int x, int y)
    {
        AZ_ASSERT(Pixels != NULL);
        return Pixels + (x + y * Width) * BytesPerPixel;
    }
    int GetSizeInBytes() const
    {
        return Width * Height * BytesPerPixel;
    }
    int GetPitch() const
    {
        return Width * BytesPerPixel;
    }
    AzTextureRef GetTexRef()
    {
        AzTextureRef tex_ref;
        tex_ref._TexData = this;
        tex_ref._TexID = AzTextureID_Invalid;
        return tex_ref;
    }
    AzTextureID GetTexID() const
    {
        return TexID;
    }

    void SetTexID(AzTextureID tex_id)
    {
        TexID = tex_id;
    }

    void SetStatus(AzTextureStatus status)
    {
        Status = status;
    }
};

struct AzFontConfig
{
    char Name[40];

    void* FontData;
    int FontDataSize;
    bool FontDataOwnedByAtlas;

    bool MergeMode;

    bool PixelSnapH;

    bool PixelSnapV;

    AzS8 FontNo;
    AzS8 OversampleH;

    AzS8 OversampleV;

    float SizePixels;

    const AzWchar* GlyphRanges;

    const AzWchar* GlyphExcludeRanges;

    AzVec2 GlyphOffset;

    float GlyphMinAdvanceX;

    float GlyphMaxAdvanceX;
    float GlyphExtraAdvanceX;

    unsigned int FontLoaderFlags;

    float RasterizerMultiply;

    float RasterizerDensity;

    AzWchar EllipsisChar;

    AzFontFlags Flags;

    AzFont* DstFont;

    const AzFontLoader* FontLoader;

    void* FontLoaderData;

    AZ_API AzFontConfig();
};

struct AzFontGlyph
{
    unsigned int Colored : 1;

    unsigned int Visible : 1;

    unsigned int SourceIdx : 4;
    unsigned int Codepoint : 26;
    float AdvanceX;
    float X0, Y0, X1, Y1;
    float U0, V0, U1, V1;

    int PackId;

    AzFontGlyph()
    {
        memset(this, 0, sizeof(*this));
        PackId = -1;
    }
};

struct AzFontGlyphRangesBuilder
{
    AzVector<AzU32> UsedChars;

    AzFontGlyphRangesBuilder()
    {
        Clear();
    }
    inline void Clear()
    {
        int size_in_bytes = (AZ_UNICODE_CODEPOINT_MAX + 1) / 8;
        UsedChars.resize(size_in_bytes / (int)sizeof(AzU32));
        memset(UsedChars.Data, 0, (size_t)size_in_bytes);
    }
    inline bool GetBit(size_t n) const
    {
        int off = (int)(n >> 5);
        AzU32 mask = 1u << (n & 31);
        return (UsedChars[off] & mask) != 0;
    }
    inline void SetBit(size_t n)
    {
        int off = (int)(n >> 5);
        AzU32 mask = 1u << (n & 31);
        UsedChars[off] |= mask;
    }
    inline void AddChar(AzWchar c)
    {
        SetBit(c);
    }
    AZ_API void AddText(const char* text, const char* text_end = NULL);

    AZ_API void AddRanges(const AzWchar* ranges);

    AZ_API void BuildRanges(AzVector<AzWchar>* out_ranges);
};

typedef int AzFontAtlasRectId;
#define AzFontAtlasRectId_Invalid -1

struct AzFontAtlasRect
{
    unsigned short x, y;
    unsigned short w, h;
    AzVec2 uv0, uv1;

    AzFontAtlasRect()
    {
        memset(this, 0, sizeof(*this));
    }
};

enum AzFontAtlasFlags_
{
    AzFontAtlasFlags_None = 0,
    AzFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,

    AzFontAtlasFlags_NoMouseCursors = 1 << 1,

    AzFontAtlasFlags_NoBakedLines = 1 << 2,
};

struct AzFontAtlas
{
    AZ_API AzFontAtlas();
    AZ_API ~AzFontAtlas();
    AZ_API AzFont* AddFont(const AzFontConfig* font_cfg);
    AZ_API AzFont* AddFontDefault(const AzFontConfig* font_cfg = NULL);
    AZ_API AzFont* AddFontFromFileTTF(const char* filename,
                                      float size_pixels = 0.0f,
                                      const AzFontConfig* font_cfg = NULL,
                                      const AzWchar* glyph_ranges = NULL);
    AZ_API AzFont* AddFontFromMemoryTTF(void* font_data,
                                        int font_data_size,
                                        float size_pixels = 0.0f,
                                        const AzFontConfig* font_cfg = NULL,
                                        const AzWchar* glyph_ranges = NULL);

    AZ_API AzFont* AddFontFromMemoryCompressedTTF(
            const void* compressed_font_data,
            int compressed_font_data_size,
            float size_pixels = 0.0f,
            const AzFontConfig* font_cfg = NULL,
            const AzWchar* glyph_ranges = NULL);

    AZ_API AzFont* AddFontFromMemoryCompressedBase85TTF(
            const char* compressed_font_data_base85,
            float size_pixels = 0.0f,
            const AzFontConfig* font_cfg = NULL,
            const AzWchar* glyph_ranges = NULL);

    AZ_API void RemoveFont(AzFont* font);

    AZ_API void Clear();

    AZ_API void CompactCache();

    AZ_API void ClearInputData();

    AZ_API void ClearFonts();

    AZ_API void ClearTexData();

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

    AZ_API bool Build();

    AZ_API void GetTexDataAsAlpha8(unsigned char** out_pixels,
                                   int* out_width,
                                   int* out_height,
                                   int* out_bytes_per_pixel = NULL);
    AZ_API void GetTexDataAsRGBA32(unsigned char** out_pixels,
                                   int* out_width,
                                   int* out_height,
                                   int* out_bytes_per_pixel = NULL);
    void SetTexID(AzTextureID id)
    {
        AZ_ASSERT(TexRef._TexID == AzTextureID_Invalid);
        TexRef._TexData->TexID = id;
    }
    void SetTexID(AzTextureRef id)
    {
        AZ_ASSERT(TexRef._TexID == AzTextureID_Invalid && id._TexData == NULL);
        TexRef._TexData->TexID = id._TexID;
    }
    bool IsBuilt() const
    {
        return Fonts.Size > 0 && TexIsBuilt;
    }

#endif

    AZ_API const AzWchar* GetGlyphRangesDefault();

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

    AZ_API const AzWchar* GetGlyphRangesGreek();

    AZ_API const AzWchar* GetGlyphRangesKorean();

    AZ_API const AzWchar* GetGlyphRangesJapanese();

    AZ_API const AzWchar* GetGlyphRangesChineseFull();

    AZ_API const AzWchar* GetGlyphRangesChineseSimplifiedCommon();

    AZ_API const AzWchar* GetGlyphRangesCyrillic();

    AZ_API const AzWchar* GetGlyphRangesThai();
    AZ_API const AzWchar* GetGlyphRangesVietnamese();

#endif

    AZ_API AzFontAtlasRectId AddCustomRect(int width,
                                           int height,
                                           AzFontAtlasRect* out_r = NULL);

    AZ_API void RemoveCustomRect(AzFontAtlasRectId id);

    AZ_API bool GetCustomRect(AzFontAtlasRectId id,
                              AzFontAtlasRect* out_r) const;

    AzFontAtlasFlags Flags;
    AzTextureFormat TexDesiredFormat;

    int TexGlyphPadding;

    int TexMinWidth;

    int TexMinHeight;

    int TexMaxWidth;

    int TexMaxHeight;

    void* UserData;

#ifdef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzTextureRef TexRef;
#else
    union
    {
        AzTextureRef TexRef;
        AzTextureRef TexID;
    };

#endif
    AzTextureData* TexData;

    AzVector<AzTextureData*> TexList;

    bool Locked;

    bool RendererHasTextures;

    bool TexIsBuilt;

    bool TexPixelsUseColors;

    AzVec2 TexUvScale;

    AzVec2 TexUvWhitePixel;

    AzVector<AzFont*> Fonts;

    AzVector<AzFontConfig> Sources;
    AzVec4 TexUvLines[AZ_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];

    int TexNextUniqueID;
    int FontNextUniqueID;
    AzVector<AzDrawListSharedData*> DrawListSharedDatas;

    AzFontAtlasBuilder* Builder;

    const AzFontLoader* FontLoader;

    const char* FontLoaderName;

    void* FontLoaderData;
    unsigned int FontLoaderFlags;

    int RefCount;
    AzContext* OwnerContext;

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

    AzFontAtlasRect TempRect;
    inline AzFontAtlasRectId AddCustomRectRegular(int w, int h)
    {
        return AddCustomRect(w, h);
    }
    inline const AzFontAtlasRect* GetCustomRectByIndex(AzFontAtlasRectId id)
    {
        return GetCustomRect(id, &TempRect) ? &TempRect : NULL;
    }
    inline void CalcCustomRectUV(const AzFontAtlasRect* r,
                                 AzVec2* out_uv_min,
                                 AzVec2* out_uv_max) const
    {
        *out_uv_min = r->uv0;
        *out_uv_max = r->uv1;
    }
    AZ_API AzFontAtlasRectId
    AddCustomRectFontGlyph(AzFont* font,
                           AzWchar codepoint,
                           int w,
                           int h,
                           float advance_x,
                           const AzVec2& offset = AzVec2(0, 0));

    AZ_API AzFontAtlasRectId
    AddCustomRectFontGlyphForSize(AzFont* font,
                                  float font_size,
                                  AzWchar codepoint,
                                  int w,
                                  int h,
                                  float advance_x,
                                  const AzVec2& offset = AzVec2(0, 0));
#endif
};

struct AzFontBaked
{
    AzVector<float> IndexAdvanceX;

    float FallbackAdvanceX;
    float Size;

    float RasterizerDensity;

    AzVector<AzU16> IndexLookup;

    AzVector<AzFontGlyph> Glyphs;
    int FallbackGlyphIndex;

    float Ascent, Descent;

    unsigned int MetricsTotalSurface : 26;

    unsigned int WantDestroy : 1;
    unsigned int LockLoadingFallback : 1;
    int LastUsedFrame;
    AzID BakedId;
    AzFont* ContainerFont;
    void* FontLoaderDatas;

    AZ_API AzFontBaked();
    AZ_API void ClearOutputData();
    AZ_API AzFontGlyph* FindGlyph(AzWchar c);

    AZ_API AzFontGlyph* FindGlyphNoFallback(AzWchar c);
    AZ_API float GetCharAdvance(AzWchar c);
    AZ_API bool IsGlyphLoaded(AzWchar c);
};

enum AzFontFlags_
{
    AzFontFlags_None = 0,
    AzFontFlags_DefaultToLegacySize = 1 << 0,

    AzFontFlags_NoLoadError = 1 << 1,

    AzFontFlags_NoLoadGlyphs = 1 << 2,
    AzFontFlags_LockBakedSizes = 1 << 3,
};

struct AzFont
{
    AzFontBaked* LastBaked;

    AzFontAtlas* ContainerAtlas;
    AzFontFlags Flags;
    float CurrentRasterizerDensity;

    AzID FontId;
    float LegacySize;

    AzVector<AzFontConfig*> Sources;

    AzWchar EllipsisChar;

    AzWchar FallbackChar;

    AzU8 Used8kPagesMap[(AZ_UNICODE_CODEPOINT_MAX + 1) / 8192 / 8];

    bool EllipsisAutoBake;

    AzStorage RemapPairs;

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    float Scale;

#endif

    AZ_API AzFont();
    AZ_API ~AzFont();
    AZ_API bool IsGlyphInFont(AzWchar c);
    bool IsLoaded() const
    {
        return ContainerAtlas != NULL;
    }
    const char* GetDebugName() const
    {
        return Sources.Size ? Sources[0]->Name : "<unknown>";
    }

    AZ_API AzFontBaked* GetFontBaked(float font_size, float density = -1.0f);
    AZ_API AzVec2 CalcTextSizeA(float size,
                                float max_width,
                                float wrap_width,
                                const char* text_begin,
                                const char* text_end = NULL,
                                const char** remaining = NULL);
    AZ_API const char* CalcWordWrapPosition(float size,
                                            const char* text,
                                            const char* text_end,
                                            float wrap_width);
    AZ_API void RenderChar(AzDrawList* draw_list,
                           float size,
                           const AzVec2& pos,
                           AzU32 col,
                           AzWchar c,
                           const AzVec4* cpu_fine_clip = NULL);
    AZ_API void RenderText(AzDrawList* draw_list,
                           float size,
                           const AzVec2& pos,
                           AzU32 col,
                           const AzVec4& clip_rect,
                           const char* text_begin,
                           const char* text_end,
                           float wrap_width = 0.0f,
                           bool cpu_fine_clip = false);
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    inline const char* CalcWordWrapPositionA(float scale,
                                             const char* text,
                                             const char* text_end,
                                             float wrap_width)
    {
        return CalcWordWrapPosition(LegacySize * scale,
                                    text,
                                    text_end,
                                    wrap_width);
    }
#endif

    AZ_API void ClearOutputData();
    AZ_API void AddRemapChar(AzWchar from_codepoint, AzWchar to_codepoint);

    AZ_API bool IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);
};

inline AzTextureID AzTextureRef::GetTexID() const
{
    AZ_ASSERT(!(_TexData != NULL && _TexID != AzTextureID_Invalid));
    return _TexData ? _TexData->TexID : _TexID;
}

inline AzTextureID AzDrawCmd::GetTexID() const
{
    AzTextureID tex_id = TexRef._TexData ? TexRef._TexData->TexID
                                         : TexRef._TexID;
    if (TexRef._TexData != NULL)
        AZ_ASSERT(tex_id != AzTextureID_Invalid && "AzDrawCmd is referring to AzTextureData that wasn't uploaded to graphics system. Backend must call AzTextureData::SetTexID() after handling AzTextureStatus_WantCreate request!");
    return tex_id;
}

enum AzViewportFlags_
{
    AzViewportFlags_None = 0,
    AzViewportFlags_IsPlatformWindow = 1 << 0,
    AzViewportFlags_IsPlatformMonitor = 1 << 1,

    AzViewportFlags_OwnedByApp = 1 << 2,

    AzViewportFlags_NoDecoration = 1 << 3,

    AzViewportFlags_NoTaskBarIcon = 1 << 4,

    AzViewportFlags_NoFocusOnAppearing = 1 << 5,

    AzViewportFlags_NoFocusOnClick = 1 << 6,

    AzViewportFlags_NoInputs = 1 << 7,

    AzViewportFlags_NoRendererClear = 1 << 8,

    AzViewportFlags_NoAutoMerge = 1 << 9,

    AzViewportFlags_TopMost = 1 << 10,

    AzViewportFlags_CanHostOtherWindows = 1 << 11,

    AzViewportFlags_IsMinimized = 1 << 12,

    AzViewportFlags_IsFocused = 1 << 13,
};

struct AzViewport
{
    AzID ID;
    AzViewportFlags Flags;
    AzVec2 Pos;

    AzVec2 Size;
    AzVec2 FramebufferScale;

    AzVec2 WorkPos;

    AzVec2 WorkSize;

    float DpiScale;
    AzID ParentViewportId;

    AzDrawData* DrawData;

    void* RendererUserData;

    void* PlatformUserData;

    void* PlatformHandle;

    void* PlatformHandleRaw;

    bool PlatformWindowCreated;

    bool PlatformRequestMove;

    bool PlatformRequestResize;

    bool PlatformRequestClose;

    AzViewport()
    {
        memset(this, 0, sizeof(*this));
    }
    ~AzViewport()
    {
        AZ_ASSERT(PlatformUserData == NULL && RendererUserData == NULL);
    }

    AzVec2 GetCenter() const
    {
        return AzVec2(Pos.x + Size.x * 0.5f, Pos.y + Size.y * 0.5f);
    }
    AzVec2 GetWorkCenter() const
    {
        return AzVec2(WorkPos.x + WorkSize.x * 0.5f,
                      WorkPos.y + WorkSize.y * 0.5f);
    }
};

struct AzPlatformIO
{
    AZ_API AzPlatformIO();

    const char* (*Platform_GetClipboardTextFn)(AzContext* ctx);
    void (*Platform_SetClipboardTextFn)(AzContext* ctx, const char* text);
    void* Platform_ClipboardUserData;

    bool (*Platform_OpenInShellFn)(AzContext* ctx, const char* path);
    void* Platform_OpenInShellUserData;

    void (*Platform_SetAzeDataFn)(AzContext* ctx,
                                  AzViewport* viewport,
                                  AzPlatformAzeData* data);
    void* Platform_AzeUserData;

    AzWchar Platform_LocaleDecimalPoint;

    int Renderer_TextureMaxWidth;
    int Renderer_TextureMaxHeight;

    void* Renderer_RenderState;

    void (*Platform_CreateWindow)(AzViewport* vp);

    void (*Platform_DestroyWindow)(AzViewport* vp);
    void (*Platform_ShowWindow)(AzViewport* vp);

    void (*Platform_SetWindowPos)(AzViewport* vp, AzVec2 pos);

    AzVec2 (*Platform_GetWindowPos)(AzViewport* vp);
    void (*Platform_SetWindowSize)(AzViewport* vp, AzVec2 size);

    AzVec2 (*Platform_GetWindowSize)(AzViewport* vp);

    AzVec2 (*Platform_GetWindowFramebufferScale)(AzViewport* vp);

    void (*Platform_SetWindowFocus)(AzViewport* vp);

    bool (*Platform_GetWindowFocus)(AzViewport* vp);
    bool (*Platform_GetWindowMinimized)(AzViewport* vp);

    void (*Platform_SetWindowTitle)(AzViewport* vp, const char* str);

    void (*Platform_SetWindowAlpha)(AzViewport* vp, float alpha);

    void (*Platform_UpdateWindow)(AzViewport* vp);

    void (*Platform_RenderWindow)(AzViewport* vp, void* render_arg);

    void (*Platform_SwapBuffers)(AzViewport* vp, void* render_arg);

    float (*Platform_GetWindowDpiScale)(AzViewport* vp);

    void (*Platform_OnChangedViewport)(AzViewport* vp);

    AzVec4 (*Platform_GetWindowWorkAreaInsets)(AzViewport* vp);

    int (*Platform_CreateVkSurface)(AzViewport* vp,
                                    AzU64 vk_inst,
                                    const void* vk_allocators,
                                    AzU64* out_vk_surface);

    void (*Renderer_CreateWindow)(AzViewport* vp);

    void (*Renderer_DestroyWindow)(AzViewport* vp);

    void (*Renderer_SetWindowSize)(AzViewport* vp, AzVec2 size);

    void (*Renderer_RenderWindow)(AzViewport* vp, void* render_arg);

    void (*Renderer_SwapBuffers)(AzViewport* vp, void* render_arg);

    AzVector<AzPlatformMonitor> Monitors;

    AzVector<AzTextureData*> Textures;

    AzVector<AzViewport*> Viewports;
};

struct AzPlatformMonitor
{
    AzVec2 MainPos, MainSize;

    AzVec2 WorkPos, WorkSize;

    float DpiScale;
    void* PlatformHandle;

    AzPlatformMonitor()
    {
        MainPos = MainSize = WorkPos = WorkSize = AzVec2(0, 0);
        DpiScale = 1.0f;
        PlatformHandle = NULL;
    }
};

struct AzPlatformAzeData
{
    bool WantVisible;
    bool WantTextInput;

    AzVec2 InputPos;
    float InputLineHeight;
    AzID ViewportId;

    AzPlatformAzeData()
    {
        memset(this, 0, sizeof(*this));
    }
};

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
namespace Az
{

    AZ_API void SetWindowFontScale(float scale);

    AZ_API void Azage(AzTextureRef tex_ref,
                      const AzVec2& image_size,
                      const AzVec2& uv0,
                      const AzVec2& uv1,
                      const AzVec4& tint_col,
                      const AzVec4& border_col);

    static inline void PushButtonRepeat(bool repeat)
    {
        PushItemFlag(AzItemFlags_ButtonRepeat, repeat);
    }
    static inline void PopButtonRepeat()
    {
        PopItemFlag();
    }
    static inline void PushTabStop(bool tab_stop)
    {
        PushItemFlag(AzItemFlags_NoTabStop, !tab_stop);
    }
    static inline void PopTabStop()
    {
        PopItemFlag();
    }
    AZ_API AzVec2 GetContentRegionMax();

    AZ_API AzVec2 GetWindowContentRegionMin();

    AZ_API AzVec2 GetWindowContentRegionMax();

    static inline bool BeginChildFrame(AzID id,
                                       const AzVec2& size,
                                       AzWindowFlags window_flags = 0)
    {
        return BeginChild(id, size, AzChildFlags_FrameStyle, window_flags);
    }
    static inline void EndChildFrame()
    {
        EndChild();
    }

    static inline void ShowStackToolWindow(bool* p_open = NULL)
    {
        ShowIDStackToolWindow(p_open);
    }
    AZ_API bool Combo(const char* label,
                      int* current_item,
                      bool (*old_callback)(void* user_data,
                                           int idx,
                                           const char** out_text),
                      void* user_data,
                      int items_count,
                      int popup_max_height_in_items = -1);
    AZ_API bool ListBox(const char* label,
                        int* current_item,
                        bool (*old_callback)(void* user_data,
                                             int idx,
                                             const char** out_text),
                        void* user_data,
                        int items_count,
                        int height_in_items = -1);

    AZ_API void SetItemAllowOverlap();

} // namespace Az

typedef AzFontAtlasRect AzFontAtlasCustomRect;

#define AZ_OFFSETOF(_TYPE, _MEMBER) offsetof(_TYPE, _MEMBER)

#endif

#ifdef AZ_DISABLE_METRICS_WINDOW
#error AZ_DISABLE_METRICS_WINDOW was renamed to AZ_DISABLE_DEBUG_TOOLS, please use new name.
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef AZ_INCLUDE_AZ_USER_H
#ifdef AZ_USER_H_FILENAME
#include AZ_USER_H_FILENAME
#else
#include "imgui_user.h"
#endif
#endif

#endif
