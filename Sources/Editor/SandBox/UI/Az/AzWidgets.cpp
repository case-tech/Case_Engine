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

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#pragma warning(disable : 4996)
#if defined(_MSC_VER) && _MSC_VER >= 1922
#pragma warning(disable : 5054)
#endif
#pragma warning(disable : 26451)
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
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-macros"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wenum-enum-conversion"
#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"
#pragma clang diagnostic ignored "-Wswitch-default"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif

static const float DRAGDROP_HOLD_TO_OPEN_TAZER = 0.70f;
static const float DRAG_MOUSE_THRESHOLD_FACTOR = 0.50f;

static const signed char AZ_S8_MIN = -128;
static const signed char AZ_S8_MAX = 127;
static const unsigned char AZ_U8_MIN = 0;
static const unsigned char AZ_U8_MAX = 0xFF;
static const signed short AZ_S16_MIN = -32768;
static const signed short AZ_S16_MAX = 32767;
static const unsigned short AZ_U16_MIN = 0;
static const unsigned short AZ_U16_MAX = 0xFFFF;
static const AzS32 AZ_S32_MIN = INT_MIN;
static const AzS32 AZ_S32_MAX = INT_MAX;
static const AzU32 AZ_U32_MIN = 0;
static const AzU32 AZ_U32_MAX = UINT_MAX;
#ifdef LLONG_MIN
static const AzS64 AZ_S64_MIN = LLONG_MIN;
static const AzS64 AZ_S64_MAX = LLONG_MAX;
#else
static const AzS64 AZ_S64_MIN = -9223372036854775807LL - 1;
static const AzS64 AZ_S64_MAX = 9223372036854775807LL;
#endif
static const AzU64 AZ_U64_MIN = 0;
#ifdef ULLONG_MAX
static const AzU64 AZ_U64_MAX = ULLONG_MAX;
#else
static const AzU64 AZ_U64_MAX = (2ULL * 9223372036854775807LL + 1);
#endif

static bool InputTextFilterCharacter(AzContext* ctx,
                                     unsigned int* p_char,
                                     AzInputTextFlags flags,
                                     AzInputTextCallback callback,
                                     void* user_data,
                                     bool input_source_is_clipboard = false);
static int InputTextCalcTextLenAndLineCount(const char* text_begin,
                                            const char** out_text_end);
static AzVec2 InputTextCalcTextSize(AzContext* ctx,
                                    const char* text_begin,
                                    const char* text_end,
                                    const char** remaining = NULL,
                                    AzVec2* out_offset = NULL,
                                    bool stop_on_new_line = false);

void Az::TextEx(const char* text, const char* text_end, AzTextFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    AzContext& g = *GAz;

    if (text == text_end)
        text = text_end = "";

    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + AzStrlen(text);

    const AzVec2 text_pos(window->DC.CursorPos.x,
                          window->DC.CursorPos.y
                                  + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    if (text_end - text <= 2000 || wrap_enabled)
    {
        const float wrap_width =
                wrap_enabled
                        ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x)
                        : 0.0f;
        const AzVec2 text_size =
                CalcTextSize(text_begin, text_end, false, wrap_width);

        AzRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        if (!ItemAdd(bb, 0))
            return;

        RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
    } else
    {
        const char* line = text;
        const float line_height = GetTextLineHeight();
        AzVec2 text_size(0, 0);

        AzVec2 pos = text_pos;
        if (!g.LogEnabled)
        {
            int lines_skippable = (int)((window->ClipRect.Min.y - text_pos.y)
                                        / line_height);
            if (lines_skippable > 0)
            {
                int lines_skipped = 0;
                while (line < text_end && lines_skipped < lines_skippable)
                {
                    const char* line_end = (const char*)
                            AzMemchr(line, '\n', text_end - line);
                    if (!line_end)
                        line_end = text_end;
                    if ((flags & AzTextFlags_NoWidthForLargeClippedText) == 0)
                        text_size.x = AzMax(text_size.x,
                                            CalcTextSize(line, line_end).x);
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }
        }

        if (line < text_end)
        {
            AzRect line_rect(pos, pos + AzVec2(FLT_MAX, line_height));
            while (line < text_end)
            {
                if (IsClippedEx(line_rect, 0))
                    break;

                const char* line_end = (const char*)AzMemchr(line,
                                                             '\n',
                                                             text_end - line);
                if (!line_end)
                    line_end = text_end;
                text_size.x = AzMax(text_size.x,
                                    CalcTextSize(line, line_end).x);
                RenderText(pos, line, line_end, false);
                line = line_end + 1;
                line_rect.Min.y += line_height;
                line_rect.Max.y += line_height;
                pos.y += line_height;
            }

            int lines_skipped = 0;
            while (line < text_end)
            {
                const char* line_end = (const char*)AzMemchr(line,
                                                             '\n',
                                                             text_end - line);
                if (!line_end)
                    line_end = text_end;
                if ((flags & AzTextFlags_NoWidthForLargeClippedText) == 0)
                    text_size.x = AzMax(text_size.x,
                                        CalcTextSize(line, line_end).x);
                line = line_end + 1;
                lines_skipped++;
            }
            pos.y += lines_skipped * line_height;
        }
        text_size.y = (pos - text_pos).y;

        AzRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        ItemAdd(bb, 0);
    }
}

void Az::TextUnformatted(const char* text, const char* text_end)
{
    TextEx(text, text_end, AzTextFlags_NoWidthForLargeClippedText);
}

void Az::Text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextV(fmt, args);
    va_end(args);
}

void Az::TextV(const char* fmt, va_list args)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const char *text, *text_end;
    AzFormatStringToTempBufferV(&text, &text_end, fmt, args);
    TextEx(text, text_end, AzTextFlags_NoWidthForLargeClippedText);
}

void Az::TextColored(const AzVec4& col, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextColoredV(col, fmt, args);
    va_end(args);
}

void Az::TextColoredV(const AzVec4& col, const char* fmt, va_list args)
{
    PushStyleColor(AzCol_Text, col);
    TextV(fmt, args);
    PopStyleColor();
}

void Az::TextDisabled(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextDisabledV(fmt, args);
    va_end(args);
}

void Az::TextDisabledV(const char* fmt, va_list args)
{
    AzContext& g = *GAz;
    PushStyleColor(AzCol_Text, g.Style.Colors[AzCol_TextDisabled]);
    TextV(fmt, args);
    PopStyleColor();
}

void Az::TextWrapped(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextWrappedV(fmt, args);
    va_end(args);
}

void Az::TextWrappedV(const char* fmt, va_list args)
{
    AzContext& g = *GAz;
    const bool need_backup = (g.CurrentWindow->DC.TextWrapPos < 0.0f);
    if (need_backup)
        PushTextWrapPos(0.0f);
    TextV(fmt, args);
    if (need_backup)
        PopTextWrapPos();
}

void Az::TextAligned(float align_x, float size_x, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextAlignedV(align_x, size_x, fmt, args);
    va_end(args);
}

void Az::TextAlignedV(float align_x,
                      float size_x,
                      const char* fmt,
                      va_list args)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const char *text, *text_end;
    AzFormatStringToTempBufferV(&text, &text_end, fmt, args);
    const AzVec2 text_size = CalcTextSize(text, text_end);
    size_x = CalcItemSize(AzVec2(size_x, 0.0f), 0.0f, text_size.y).x;

    AzVec2 pos(window->DC.CursorPos.x,
               window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    AzVec2 pos_max(pos.x + size_x, window->ClipRect.Max.y);
    AzVec2 size(AzMin(size_x, text_size.x), text_size.y);
    window->DC.CursorMaxPos.x = AzMax(window->DC.CursorMaxPos.x,
                                      pos.x + text_size.x);
    window->DC.IdealMaxPos.x = AzMax(window->DC.IdealMaxPos.x,
                                     pos.x + text_size.x);
    if (align_x > 0.0f && text_size.x < size_x)
        pos.x += AzTrunc((size_x - text_size.x) * align_x);
    RenderTextEllipsis(window->DrawList,
                       pos,
                       pos_max,
                       pos_max.x,
                       text,
                       text_end,
                       &text_size);

    const AzVec2 backup_max_pos = window->DC.CursorMaxPos;
    ItemSize(size);
    ItemAdd(AzRect(pos, pos + size), 0);
    window->DC.CursorMaxPos.x = backup_max_pos.x;

    if (size_x < text_size.x
        && IsItemHovered(AzHoveredFlags_NoNavOverride
                         | AzHoveredFlags_AllowWhenDisabled
                         | AzHoveredFlags_ForTooltip))
        SetTooltip("%.*s", (int)(text_end - text), text);
}

void Az::LabelText(const char* label, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    LabelTextV(label, fmt, args);
    va_end(args);
}

void Az::LabelTextV(const char* label, const char* fmt, va_list args)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const float w = CalcItemWidth();

    const char *value_text_begin, *value_text_end;
    AzFormatStringToTempBufferV(&value_text_begin, &value_text_end, fmt, args);
    const AzVec2 value_size = CalcTextSize(value_text_begin,
                                           value_text_end,
                                           false);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);

    const AzVec2 pos = window->DC.CursorPos;
    const AzRect value_bb(
            pos,
            pos + AzVec2(w, value_size.y + style.FramePadding.y * 2));
    const AzRect total_bb(
            pos,
            pos
                    + AzVec2(w
                                     + (label_size.x > 0.0f
                                                ? style.ItemInnerSpacing.x
                                                          + label_size.x
                                                : 0.0f),
                             AzMax(value_size.y, label_size.y)
                                     + style.FramePadding.y * 2));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0))
        return;

    RenderTextClipped(value_bb.Min + style.FramePadding,
                      value_bb.Max,
                      value_text_begin,
                      value_text_end,
                      &value_size,
                      AzVec2(0.0f, 0.0f));
    if (label_size.x > 0.0f)
        RenderText(AzVec2(value_bb.Max.x + style.ItemInnerSpacing.x,
                          value_bb.Min.y + style.FramePadding.y),
                   label);
}

void Az::BulletText(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    BulletTextV(fmt, args);
    va_end(args);
}

void Az::BulletTextV(const char* fmt, va_list args)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;

    const char *text_begin, *text_end;
    AzFormatStringToTempBufferV(&text_begin, &text_end, fmt, args);
    const AzVec2 label_size = CalcTextSize(text_begin, text_end, false);
    const AzVec2 total_size = AzVec2(
            g.FontSize
                    + (label_size.x > 0.0f
                               ? (label_size.x + style.FramePadding.x * 2)
                               : 0.0f),
            label_size.y);
    AzVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(total_size, 0.0f);
    const AzRect bb(pos, pos + total_size);
    if (!ItemAdd(bb, 0))
        return;

    AzU32 text_col = GetColorU32(AzCol_Text);
    RenderBullet(window->DrawList,
                 bb.Min
                         + AzVec2(style.FramePadding.x + g.FontSize * 0.5f,
                                  g.FontSize * 0.5f),
                 text_col);
    RenderText(bb.Min + AzVec2(g.FontSize + style.FramePadding.x * 2, 0.0f),
               text_begin,
               text_end,
               false);
}

bool Az::ButtonBehavior(const AzRect& bb,
                        AzID id,
                        bool* out_hovered,
                        bool* out_held,
                        AzButtonFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();

    AzItemFlags item_flags = (g.LastItemData.ID == id ? g.LastItemData.ItemFlags
                                                      : g.CurrentItemFlags);
    if (flags & AzButtonFlags_AllowOverlap)
        item_flags |= AzItemFlags_AllowOverlap;

    if ((flags & AzButtonFlags_MouseButtonMask_) == 0)
        flags |= AzButtonFlags_MouseButtonLeft;

    if ((flags & AzButtonFlags_PressedOnMask_) == 0)
        flags |= (item_flags & AzItemFlags_ButtonRepeat)
                         ? AzButtonFlags_PressedOnClick
                         : AzButtonFlags_PressedOnDefault_;

    AzWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children =
            (flags & AzButtonFlags_FlattenChildren) && g.HoveredWindow
            && g.HoveredWindow->RootWindowDockTree
                       == window->RootWindowDockTree;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef AZ_ENABLE_TEST_ENGINE

    if (g.LastItemData.ID != id)
        AZ_TEST_ENGINE_ITEM_ADD(id, bb, NULL);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id, item_flags);

    if (g.DragDropActive && (flags & AzButtonFlags_PressedOnDragDropHold)
        && !(g.DragDropSourceFlags & AzDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(AzHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (g.HoveredIdTimer - g.IO.DeltaTime <= DRAGDROP_HOLD_TO_OPEN_TAZER
                && g.HoveredIdTimer >= DRAGDROP_HOLD_TO_OPEN_TAZER)
            {
                pressed = true;
                g.DragDropHoldJustPressedId = id;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    const AzID test_owner_id = (flags & AzButtonFlags_NoTestKeyOwner)
                                       ? AzKeyOwner_Any
                                       : id;
    if (hovered)
    {
        AZ_ASSERT(id != 0);

        int mouse_button_clicked = -1;
        int mouse_button_released = -1;
        for (int button = 0; button < 3; button++)
            if (flags & (AzButtonFlags_MouseButtonLeft << button))
            {
                if (IsMouseClicked(button, AzInputFlags_None, test_owner_id)
                    && mouse_button_clicked == -1)
                {
                    mouse_button_clicked = button;
                }
                if (IsMouseReleased(button, test_owner_id)
                    && mouse_button_released == -1)
                {
                    mouse_button_released = button;
                }
            }

        const bool mods_ok = !(flags & AzButtonFlags_NoKeyModsAllowed)
                             || (!g.IO.KeyCtrl && !g.IO.KeyShift
                                 && !g.IO.KeyAlt);
        if (mods_ok)
        {
            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (!(flags & AzButtonFlags_NoSetKeyOwner))
                    SetKeyOwner(MouseButtonToKey(mouse_button_clicked), id);
                if (flags
                    & (AzButtonFlags_PressedOnClickRelease
                       | AzButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & AzButtonFlags_NoNavFocus))
                    {
                        SetFocusID(id, window);
                        FocusWindow(window);
                    } else
                    {
                        FocusWindow(window,
                                    AzFocusRequestFlags_RestoreFocusedChild);
                    }
                }
                if ((flags & AzButtonFlags_PressedOnClick)
                    || ((flags & AzButtonFlags_PressedOnDoubleClick)
                        && g.IO.MouseClickedCount[mouse_button_clicked] == 2))
                {
                    pressed = true;
                    if (flags & AzButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & AzButtonFlags_NoNavFocus))
                    {
                        SetFocusID(id, window);
                        FocusWindow(window);
                    } else
                    {
                        FocusWindow(window,
                                    AzFocusRequestFlags_RestoreFocusedChild);
                    }
                }
            }
            if (flags & AzButtonFlags_PressedOnRelease)
            {
                if (mouse_button_released != -1)
                {
                    const bool has_repeated_at_least_once =
                            (item_flags & AzItemFlags_ButtonRepeat)
                            && g.IO.MouseDownDurationPrev[mouse_button_released]
                                       >= g.IO.KeyRepeatDelay;
                    if (!has_repeated_at_least_once)
                        pressed = true;
                    if (!(flags & AzButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    ClearActiveID();
                }
            }

            if (g.ActiveId == id && (item_flags & AzItemFlags_ButtonRepeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f
                    && IsMouseClicked(g.ActiveIdMouseButton,
                                      AzInputFlags_Repeat,
                                      test_owner_id))
                    pressed = true;
        }

        if (pressed && g.IO.ConfigNavCursorVisibleAuto)
            g.NavCursorVisible = false;
    }

    if (g.NavId == id && g.NavCursorVisible && g.NavHighlightItemUnderNav)
        if (!(flags & AzButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = (g.NavActivatePressedId == id);
        if (!nav_activated_by_inputs && (item_flags & AzItemFlags_ButtonRepeat))
        {
            const AzKeyData* key1 = GetKeyData(AzKey_Space);
            const AzKeyData* key2 = GetKeyData(AzKey_Enter);
            const AzKeyData* key3 = GetKeyData(AzKey_NavGamepadActivate);
            const float t1 = AzMax(AzMax(key1->DownDuration,
                                         key2->DownDuration),
                                   key3->DownDuration);
            nav_activated_by_inputs =
                    CalcTypematicRepeatAmount(t1 - g.IO.DeltaTime,
                                              t1,
                                              g.IO.KeyRepeatDelay,
                                              g.IO.KeyRepeatRate)
                    > 0;
        }
        if (nav_activated_by_code || nav_activated_by_inputs)
        {
            pressed = true;
            SetActiveID(id, window);
            g.ActiveIdSource = g.NavInputSource;
            if (!(flags & AzButtonFlags_NoNavFocus)
                && !(g.NavActivateFlags & AzActivateFlags_FromShortcut))
                SetFocusID(id, window);
            if (g.NavActivateFlags & AzActivateFlags_FromShortcut)
                g.ActiveIdFromShortcut = true;
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == AzInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            if (mouse_button == -1)
            {
                ClearActiveID();
            } else if (IsMouseDown(mouse_button, test_owner_id))
            {
                held = true;
            } else
            {
                bool release_in = hovered
                                  && (flags
                                      & AzButtonFlags_PressedOnClickRelease)
                                             != 0;
                bool release_anywhere =
                        (flags & AzButtonFlags_PressedOnClickReleaseAnywhere)
                        != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release =
                            (flags & AzButtonFlags_PressedOnDoubleClick)
                            && g.IO.MouseReleased[mouse_button]
                            && g.IO.MouseClickedLastCount[mouse_button] == 2;
                    bool is_repeating_already =
                            (item_flags & AzItemFlags_ButtonRepeat)
                            && g.IO.MouseDownDurationPrev[mouse_button]
                                       >= g.IO.KeyRepeatDelay;
                    bool is_button_avail_or_owned =
                            TestKeyOwner(MouseButtonToKey(mouse_button),
                                         test_owner_id);
                    if (!is_double_click_release && !is_repeating_already
                        && is_button_avail_or_owned)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & AzButtonFlags_NoNavFocus)
                && g.IO.ConfigNavCursorVisibleAuto)
                g.NavCursorVisible = false;
        } else if (g.ActiveIdSource == AzInputSource_Keyboard
                   || g.ActiveIdSource == AzInputSource_Gamepad)
        {
            if (g.NavActivateDownId == id)
                held = true;
            else
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

    if (g.NavHighlightActivatedId == id)
        hovered = true;

    if (out_hovered)
        *out_hovered = hovered;
    if (out_held)
        *out_held = held;

    return pressed;
}

bool Az::ButtonEx(const char* label,
                  const AzVec2& size_arg,
                  AzButtonFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);

    AzVec2 pos = window->DC.CursorPos;
    if ((flags & AzButtonFlags_AlignTextBaseLine)
        && style.FramePadding.y < window->DC.CurrLineTextBaseOffset)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    AzVec2 size = CalcItemSize(size_arg,
                               label_size.x + style.FramePadding.x * 2.0f,
                               label_size.y + style.FramePadding.y * 2.0f);

    const AzRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    const AzU32 col = GetColorU32((held && hovered) ? AzCol_ButtonActive
                                  : hovered         ? AzCol_ButtonHovered
                                                    : AzCol_Button);
    RenderNavCursor(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding,
                      bb.Max - style.FramePadding,
                      label,
                      NULL,
                      &label_size,
                      style.ButtonTextAlign,
                      &bb);

    AZ_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool Az::Button(const char* label, const AzVec2& size_arg)
{
    return ButtonEx(label, size_arg, AzButtonFlags_None);
}

bool Az::SmallButton(const char* label)
{
    AzContext& g = *GAz;
    float backup_padding_y = g.Style.FramePadding.y;
    g.Style.FramePadding.y = 0.0f;
    bool pressed = ButtonEx(label,
                            AzVec2(0, 0),
                            AzButtonFlags_AlignTextBaseLine);
    g.Style.FramePadding.y = backup_padding_y;
    return pressed;
}

bool Az::InvisibleButton(const char* str_id,
                         const AzVec2& size_arg,
                         AzButtonFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AZ_ASSERT(size_arg.x != 0.0f && size_arg.y != 0.0f);

    const AzID id = window->GetID(str_id);
    AzVec2 size = CalcItemSize(size_arg, 0.0f, 0.0f);
    const AzRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(size);
    if (!ItemAdd(bb,
                 id,
                 NULL,
                 (flags & AzButtonFlags_EnableNav) ? AzItemFlags_None
                                                   : AzItemFlags_NoNav))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
    RenderNavCursor(bb, id);

    AZ_TEST_ENGINE_ITEM_INFO(id, str_id, g.LastItemData.StatusFlags);
    return pressed;
}

bool Az::ArrowButtonEx(const char* str_id,
                       AzDir dir,
                       AzVec2 size,
                       AzButtonFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const AzID id = window->GetID(str_id);
    const AzRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const float default_size = GetFrameHeight();
    ItemSize(size, (size.y >= default_size) ? g.Style.FramePadding.y : -1.0f);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    const AzU32 bg_col = GetColorU32((held && hovered) ? AzCol_ButtonActive
                                     : hovered         ? AzCol_ButtonHovered
                                                       : AzCol_Button);
    const AzU32 text_col = GetColorU32(AzCol_Text);
    RenderNavCursor(bb, id);
    RenderFrame(bb.Min, bb.Max, bg_col, true, g.Style.FrameRounding);
    RenderArrow(window->DrawList,
                bb.Min
                        + AzVec2(AzMax(0.0f, (size.x - g.FontSize) * 0.5f),
                                 AzMax(0.0f, (size.y - g.FontSize) * 0.5f)),
                text_col,
                dir);

    AZ_TEST_ENGINE_ITEM_INFO(id, str_id, g.LastItemData.StatusFlags);
    return pressed;
}

bool Az::ArrowButton(const char* str_id, AzDir dir)
{
    float sz = GetFrameHeight();
    return ArrowButtonEx(str_id, dir, AzVec2(sz, sz), AzButtonFlags_None);
}

bool Az::CloseButton(AzID id, const AzVec2& pos)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    const AzRect bb(pos, pos + AzVec2(g.FontSize, g.FontSize));
    AzRect bb_interact = bb;
    const float area_to_visible_ratio = window->OuterRectClipped.GetArea()
                                        / bb.GetArea();
    if (area_to_visible_ratio < 1.5f)
        bb_interact.Expand(AzTrunc(bb_interact.GetSize() * -0.25f));

    bool is_clipped = !ItemAdd(bb_interact, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb_interact, id, &hovered, &held);
    if (is_clipped)
        return pressed;

    AzU32 bg_col = GetColorU32(held ? AzCol_ButtonActive : AzCol_ButtonHovered);
    if (hovered)
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col);
    RenderNavCursor(bb, id, AzNavRenderCursorFlags_Compact);
    const AzU32 cross_col = GetColorU32(AzCol_Text);
    const AzVec2 cross_center = bb.GetCenter() - AzVec2(0.5f, 0.5f);
    const float cross_extent = g.FontSize * 0.5f * 0.7071f - 1.0f;
    const float cross_thickness = 1.0f;
    window->DrawList->AddLine(cross_center
                                      + AzVec2(+cross_extent, +cross_extent),
                              cross_center
                                      + AzVec2(-cross_extent, -cross_extent),
                              cross_col,
                              cross_thickness);
    window->DrawList->AddLine(cross_center
                                      + AzVec2(+cross_extent, -cross_extent),
                              cross_center
                                      + AzVec2(-cross_extent, +cross_extent),
                              cross_col,
                              cross_thickness);

    return pressed;
}

bool Az::CollapseButton(AzID id, const AzVec2& pos, AzDockNode* dock_node)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    AzRect bb(pos, pos + AzVec2(g.FontSize, g.FontSize));
    bool is_clipped = !ItemAdd(bb, id);
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, AzButtonFlags_None);
    if (is_clipped)
        return pressed;

    AzU32 bg_col = GetColorU32((held && hovered) ? AzCol_ButtonActive
                               : hovered         ? AzCol_ButtonHovered
                                                 : AzCol_Button);
    AzU32 text_col = GetColorU32(AzCol_Text);
    if (hovered || held)
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col);
    RenderNavCursor(bb, id, AzNavRenderCursorFlags_Compact);

    if (dock_node)
        RenderArrowDockMenu(window->DrawList, bb.Min, g.FontSize, text_col);
    else
        RenderArrow(window->DrawList,
                    bb.Min,
                    text_col,
                    window->Collapsed ? AzDir_Right : AzDir_Down,
                    1.0f);

    if (IsItemActive() && IsMouseDragging(0))
        StartMouseMovingWindowOrNode(window, dock_node, true);

    return pressed;
}

AzID Az::GetWindowScrollbarID(AzWindow* window, AzAxis axis)
{
    return window->GetID(axis == AzAxis_X ? "#SCROLLX" : "#SCROLLY");
}

AzRect Az::GetWindowScrollbarRect(AzWindow* window, AzAxis axis)
{
    AzContext& g = *GAz;
    const AzRect outer_rect = window->Rect();
    const AzRect inner_rect = window->InnerRect;
    const float scrollbar_size = window->ScrollbarSizes[axis ^ 1];
    AZ_ASSERT(scrollbar_size >= 0.0f);
    const float border_size = AZ_ROUND(window->WindowBorderSize * 0.5f);
    const float border_top = (window->Flags & AzWindowFlags_MenuBar)
                                     ? AZ_ROUND(g.Style.FrameBorderSize * 0.5f)
                                     : 0.0f;
    if (axis == AzAxis_X)
        return AzRect(inner_rect.Min.x + border_size,
                      AzMax(outer_rect.Min.y + border_size,
                            outer_rect.Max.y - border_size - scrollbar_size),
                      inner_rect.Max.x - border_size,
                      outer_rect.Max.y - border_size);
    else
        return AzRect(AzMax(outer_rect.Min.x,
                            outer_rect.Max.x - border_size - scrollbar_size),
                      inner_rect.Min.y + border_top,
                      outer_rect.Max.x - border_size,
                      inner_rect.Max.y - border_size);
}

void Az::Scrollbar(AzAxis axis)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    const AzID id = GetWindowScrollbarID(window, axis);

    AzRect bb = GetWindowScrollbarRect(window, axis);
    AzDrawFlags rounding_corners = AzDrawFlags_RoundCornersNone;
    if (axis == AzAxis_X)
    {
        rounding_corners |= AzDrawFlags_RoundCornersBottomLeft;
        if (!window->ScrollbarY)
            rounding_corners |= AzDrawFlags_RoundCornersBottomRight;
    } else
    {
        if ((window->Flags & AzWindowFlags_NoTitleBar)
            && !(window->Flags & AzWindowFlags_MenuBar))
            rounding_corners |= AzDrawFlags_RoundCornersTopRight;
        if (!window->ScrollbarX)
            rounding_corners |= AzDrawFlags_RoundCornersBottomRight;
    }
    float size_visible = window->InnerRect.Max[axis]
                         - window->InnerRect.Min[axis];
    float size_contents = window->ContentSize[axis]
                          + window->WindowPadding[axis] * 2.0f;
    AzS64 scroll = (AzS64)window->Scroll[axis];
    ScrollbarEx(bb,
                id,
                axis,
                &scroll,
                (AzS64)size_visible,
                (AzS64)size_contents,
                rounding_corners);
    window->Scroll[axis] = (float)scroll;
}

bool Az::ScrollbarEx(const AzRect& bb_frame,
                     AzID id,
                     AzAxis axis,
                     AzS64* p_scroll_v,
                     AzS64 size_visible_v,
                     AzS64 size_contents_v,
                     AzDrawFlags draw_rounding_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    const float bb_frame_width = bb_frame.GetWidth();
    const float bb_frame_height = bb_frame.GetHeight();
    if (bb_frame_width <= 0.0f || bb_frame_height <= 0.0f)
        return false;

    float alpha = 1.0f;
    if ((axis == AzAxis_Y) && bb_frame_height < bb_frame_width)
        alpha = AzSaturate(bb_frame_height
                           / AzMax(bb_frame_width * 2.0f, 1.0f));
    if (alpha <= 0.0f)
        return false;

    const AzStyle& style = g.Style;
    const bool allow_interaction = (alpha >= 1.0f);

    AzRect bb = bb_frame;
    bb.Expand(AzVec2(
            -AzClamp(AZ_TRUNC((bb_frame_width - 2.0f) * 0.5f), 0.0f, 3.0f),
            -AzClamp(AZ_TRUNC((bb_frame_height - 2.0f) * 0.5f), 0.0f, 3.0f)));

    const float scrollbar_size_v = (axis == AzAxis_X) ? bb.GetWidth()
                                                      : bb.GetHeight();

    AZ_ASSERT(AzMax(size_contents_v, size_visible_v) > 0.0f);
    const AzS64 win_size_v = AzMax(AzMax(size_contents_v, size_visible_v),
                                   (AzS64)1);
    const float grab_h_minsize = AzMin(bb.GetSize()[axis], style.GrabMinSize);
    const float grab_h_pixels = AzClamp(
            scrollbar_size_v * ((float)size_visible_v / (float)win_size_v),
            grab_h_minsize,
            scrollbar_size_v);
    const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

    bool held = false;
    bool hovered = false;
    ItemAdd(bb_frame, id, NULL, AzItemFlags_NoNav);
    ButtonBehavior(bb, id, &hovered, &held, AzButtonFlags_NoNavFocus);

    const AzS64 scroll_max = AzMax((AzS64)1, size_contents_v - size_visible_v);
    float scroll_ratio = AzSaturate((float)*p_scroll_v / (float)scroll_max);
    float grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels)
                        / scrollbar_size_v;
    if (held && allow_interaction && grab_h_norm < 1.0f)
    {
        const float scrollbar_pos_v = bb.Min[axis];
        const float mouse_pos_v = g.IO.MousePos[axis];

        const float clicked_v_norm = AzSaturate((mouse_pos_v - scrollbar_pos_v)
                                                / scrollbar_size_v);

        const int held_dir = (clicked_v_norm < grab_v_norm)                 ? -1
                             : (clicked_v_norm > grab_v_norm + grab_h_norm) ? +1
                                                                            : 0;
        if (g.ActiveIdIsJustActivated)
        {
            const bool scroll_to_clicked_location =
                    (g.IO.ConfigScrollbarScrollByPage == false || g.IO.KeyShift
                     || held_dir == 0);
            g.ScrollbarSeekMode = scroll_to_clicked_location ? 0
                                                             : (short)held_dir;
            g.ScrollbarClickDeltaToGrabCenter =
                    (held_dir == 0 && !g.IO.KeyShift)
                            ? clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f
                            : 0.0f;
        }

        if (g.ScrollbarSeekMode == 0)
        {
            const float scroll_v_norm = AzSaturate(
                    (clicked_v_norm - g.ScrollbarClickDeltaToGrabCenter
                     - grab_h_norm * 0.5f)
                    / (1.0f - grab_h_norm));
            *p_scroll_v = (AzS64)(scroll_v_norm * scroll_max);
        } else
        {
            if (IsMouseClicked(AzMouseButton_Left, AzInputFlags_Repeat)
                && held_dir == g.ScrollbarSeekMode)
            {
                float page_dir = (g.ScrollbarSeekMode > 0.0f) ? +1.0f : -1.0f;
                *p_scroll_v = AzClamp(
                        *p_scroll_v + (AzS64)(page_dir * size_visible_v),
                        (AzS64)0,
                        scroll_max);
            }
        }

        scroll_ratio = AzSaturate((float)*p_scroll_v / (float)scroll_max);
        grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels)
                      / scrollbar_size_v;
    }

    const AzU32 bg_col = GetColorU32(AzCol_ScrollbarBg);
    const AzU32 grab_col = GetColorU32(held      ? AzCol_ScrollbarGrabActive
                                       : hovered ? AzCol_ScrollbarGrabHovered
                                                 : AzCol_ScrollbarGrab,
                                       alpha);
    window->DrawList->AddRectFilled(bb_frame.Min,
                                    bb_frame.Max,
                                    bg_col,
                                    window->WindowRounding,
                                    draw_rounding_flags);
    AzRect grab_rect;
    if (axis == AzAxis_X)
        grab_rect = AzRect(AzLerp(bb.Min.x, bb.Max.x, grab_v_norm),
                           bb.Min.y,
                           AzLerp(bb.Min.x, bb.Max.x, grab_v_norm)
                                   + grab_h_pixels,
                           bb.Max.y);
    else
        grab_rect = AzRect(bb.Min.x,
                           AzLerp(bb.Min.y, bb.Max.y, grab_v_norm),
                           bb.Max.x,
                           AzLerp(bb.Min.y, bb.Max.y, grab_v_norm)
                                   + grab_h_pixels);
    window->DrawList->AddRectFilled(grab_rect.Min,
                                    grab_rect.Max,
                                    grab_col,
                                    style.ScrollbarRounding);

    return held;
}

void Az::AzageWithBg(AzTextureRef tex_ref,
                     const AzVec2& image_size,
                     const AzVec2& uv0,
                     const AzVec2& uv1,
                     const AzVec4& bg_col,
                     const AzVec4& tint_col)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const AzVec2 padding(g.Style.AzageBorderSize, g.Style.AzageBorderSize);
    const AzRect bb(window->DC.CursorPos,
                    window->DC.CursorPos + image_size + padding * 2.0f);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    if (g.Style.AzageBorderSize > 0.0f)
        window->DrawList->AddRect(bb.Min,
                                  bb.Max,
                                  GetColorU32(AzCol_Border),
                                  0.0f,
                                  AzDrawFlags_None,
                                  g.Style.AzageBorderSize);
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(bb.Min + padding,
                                        bb.Max - padding,
                                        GetColorU32(bg_col));
    window->DrawList->AddAzage(tex_ref,
                               bb.Min + padding,
                               bb.Max - padding,
                               uv0,
                               uv1,
                               GetColorU32(tint_col));
}

void Az::Azage(AzTextureRef tex_ref,
               const AzVec2& image_size,
               const AzVec2& uv0,
               const AzVec2& uv1)
{
    AzageWithBg(tex_ref, image_size, uv0, uv1);
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
void Az::Azage(AzTextureRef tex_ref,
               const AzVec2& image_size,
               const AzVec2& uv0,
               const AzVec2& uv1,
               const AzVec4& tint_col,
               const AzVec4& border_col)
{
    AzContext& g = *GAz;
    PushStyleVar(AzStyleVar_AzageBorderSize,
                 (border_col.w > 0.0f) ? AzMax(1.0f, g.Style.AzageBorderSize)
                                       : 0.0f);
    PushStyleColor(AzCol_Border, border_col);
    AzageWithBg(tex_ref, image_size, uv0, uv1, AzVec4(0, 0, 0, 0), tint_col);
    PopStyleColor();
    PopStyleVar();
}
#endif

bool Az::AzageButtonEx(AzID id,
                       AzTextureRef tex_ref,
                       const AzVec2& image_size,
                       const AzVec2& uv0,
                       const AzVec2& uv1,
                       const AzVec4& bg_col,
                       const AzVec4& tint_col,
                       AzButtonFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const AzVec2 padding = g.Style.FramePadding;
    const AzRect bb(window->DC.CursorPos,
                    window->DC.CursorPos + image_size + padding * 2.0f);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    const AzU32 col = GetColorU32((held && hovered) ? AzCol_ButtonActive
                                  : hovered         ? AzCol_ButtonHovered
                                                    : AzCol_Button);
    RenderNavCursor(bb, id);
    RenderFrame(bb.Min,
                bb.Max,
                col,
                true,
                AzClamp((float)AzMin(padding.x, padding.y),
                        0.0f,
                        g.Style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(bb.Min + padding,
                                        bb.Max - padding,
                                        GetColorU32(bg_col));
    window->DrawList->AddAzage(tex_ref,
                               bb.Min + padding,
                               bb.Max - padding,
                               uv0,
                               uv1,
                               GetColorU32(tint_col));

    return pressed;
}

bool Az::AzageButton(const char* str_id,
                     AzTextureRef tex_ref,
                     const AzVec2& image_size,
                     const AzVec2& uv0,
                     const AzVec2& uv1,
                     const AzVec4& bg_col,
                     const AzVec4& tint_col)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    return AzageButtonEx(window->GetID(str_id),
                         tex_ref,
                         image_size,
                         uv0,
                         uv1,
                         bg_col,
                         tint_col);
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

#endif

bool Az::Checkbox(const char* label, bool* v)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const AzVec2 pos = window->DC.CursorPos;
    const AzRect total_bb(
            pos,
            pos
                    + AzVec2(square_sz
                                     + (label_size.x > 0.0f
                                                ? style.ItemInnerSpacing.x
                                                          + label_size.x
                                                : 0.0f),
                             label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    const bool is_visible = ItemAdd(total_bb, id);
    const bool is_multi_select = (g.LastItemData.ItemFlags
                                  & AzItemFlags_IsMultiSelect)
                                 != 0;
    if (!is_visible)
        if (!is_multi_select || !g.BoxSelectState.UnclipMode
            || !g.BoxSelectState.UnclipRect.Overlaps(total_bb))
        {
            AZ_TEST_ENGINE_ITEM_INFO(id,
                                     label,
                                     g.LastItemData.StatusFlags
                                             | AzItemStatusFlags_Checkable
                                             | (*v ? AzItemStatusFlags_Checked
                                                   : 0));
            return false;
        }

    bool checked = *v;
    if (is_multi_select)
        MultiSelectItemHeader(id, &checked, NULL);

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    if (is_multi_select)
        MultiSelectItemFooter(id, &checked, &pressed);
    else if (pressed)
        checked = !checked;

    if (*v != checked)
    {
        *v = checked;
        pressed = true;
        MarkItemEdited(id);
    }

    const AzRect check_bb(pos, pos + AzVec2(square_sz, square_sz));
    const bool mixed_value = (g.LastItemData.ItemFlags & AzItemFlags_MixedValue)
                             != 0;
    if (is_visible)
    {
        RenderNavCursor(total_bb, id);
        RenderFrame(check_bb.Min,
                    check_bb.Max,
                    GetColorU32((held && hovered) ? AzCol_FrameBgActive
                                : hovered         ? AzCol_FrameBgHovered
                                                  : AzCol_FrameBg),
                    true,
                    style.FrameRounding);
        AzU32 check_col = GetColorU32(AzCol_CheckMark);
        if (mixed_value)
        {
            AzVec2 pad(AzMax(1.0f, AZ_TRUNC(square_sz / 3.6f)),
                       AzMax(1.0f, AZ_TRUNC(square_sz / 3.6f)));
            window->DrawList->AddRectFilled(check_bb.Min + pad,
                                            check_bb.Max - pad,
                                            check_col,
                                            style.FrameRounding);
        } else if (*v)
        {
            const float pad = AzMax(1.0f, AZ_TRUNC(square_sz / 6.0f));
            RenderCheckMark(window->DrawList,
                            check_bb.Min + AzVec2(pad, pad),
                            check_col,
                            square_sz - pad * 2.0f);
        }
    }
    const AzVec2 label_pos = AzVec2(check_bb.Max.x + style.ItemInnerSpacing.x,
                                    check_bb.Min.y + style.FramePadding.y);
    if (g.LogEnabled)
        LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
    if (is_visible && label_size.x > 0.0f)
        RenderText(label_pos, label);

    AZ_TEST_ENGINE_ITEM_INFO(id,
                             label,
                             g.LastItemData.StatusFlags
                                     | AzItemStatusFlags_Checkable
                                     | (*v ? AzItemStatusFlags_Checked : 0));
    return pressed;
}

template <typename T>
bool Az::CheckboxFlagsT(const char* label, T* flags, T flags_value)
{
    bool all_on = (*flags & flags_value) == flags_value;
    bool any_on = (*flags & flags_value) != 0;
    bool pressed;
    if (!all_on && any_on)
    {
        AzContext& g = *GAz;
        g.NextItemData.ItemFlags |= AzItemFlags_MixedValue;
        pressed = Checkbox(label, &all_on);
    } else
    {
        pressed = Checkbox(label, &all_on);
    }
    if (pressed)
    {
        if (all_on)
            *flags |= flags_value;
        else
            *flags &= ~flags_value;
    }
    return pressed;
}

bool Az::CheckboxFlags(const char* label, int* flags, int flags_value)
{
    return CheckboxFlagsT(label, flags, flags_value);
}

bool Az::CheckboxFlags(const char* label,
                       unsigned int* flags,
                       unsigned int flags_value)
{
    return CheckboxFlagsT(label, flags, flags_value);
}

bool Az::CheckboxFlags(const char* label, AzS64* flags, AzS64 flags_value)
{
    return CheckboxFlagsT(label, flags, flags_value);
}

bool Az::CheckboxFlags(const char* label, AzU64* flags, AzU64 flags_value)
{
    return CheckboxFlagsT(label, flags, flags_value);
}

bool Az::RadioButton(const char* label, bool active)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const AzVec2 pos = window->DC.CursorPos;
    const AzRect check_bb(pos, pos + AzVec2(square_sz, square_sz));
    const AzRect total_bb(
            pos,
            pos
                    + AzVec2(square_sz
                                     + (label_size.x > 0.0f
                                                ? style.ItemInnerSpacing.x
                                                          + label_size.x
                                                : 0.0f),
                             label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    AzVec2 center = check_bb.GetCenter();
    center.x = AZ_ROUND(center.x);
    center.y = AZ_ROUND(center.y);
    const float radius = (square_sz - 1.0f) * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        MarkItemEdited(id);

    RenderNavCursor(total_bb, id);
    const int num_segment = window->DrawList->_CalcCircleAutoSegmentCount(
            radius);
    window->DrawList->AddCircleFilled(center,
                                      radius,
                                      GetColorU32((held && hovered)
                                                          ? AzCol_FrameBgActive
                                                  : hovered
                                                          ? AzCol_FrameBgHovered
                                                          : AzCol_FrameBg),
                                      num_segment);
    if (active)
    {
        const float pad = AzMax(1.0f, AZ_TRUNC(square_sz / 6.0f));
        window->DrawList->AddCircleFilled(center,
                                          radius - pad,
                                          GetColorU32(AzCol_CheckMark));
    }

    if (style.FrameBorderSize > 0.0f)
    {
        window->DrawList->AddCircle(center + AzVec2(1, 1),
                                    radius,
                                    GetColorU32(AzCol_BorderShadow),
                                    num_segment,
                                    style.FrameBorderSize);
        window->DrawList->AddCircle(center,
                                    radius,
                                    GetColorU32(AzCol_Border),
                                    num_segment,
                                    style.FrameBorderSize);
    }

    AzVec2 label_pos = AzVec2(check_bb.Max.x + style.ItemInnerSpacing.x,
                              check_bb.Min.y + style.FramePadding.y);
    if (g.LogEnabled)
        LogRenderedText(&label_pos, active ? "(x)" : "( )");
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);

    AZ_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool Az::RadioButton(const char* label, int* v, int v_button)
{
    const bool pressed = RadioButton(label, *v == v_button);
    if (pressed)
        *v = v_button;
    return pressed;
}

void Az::ProgressBar(float fraction,
                     const AzVec2& size_arg,
                     const char* overlay)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;

    AzVec2 pos = window->DC.CursorPos;
    AzVec2 size = CalcItemSize(size_arg,
                               CalcItemWidth(),
                               g.FontSize + style.FramePadding.y * 2.0f);
    AzRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    const bool is_indeterminate = (fraction < 0.0f);
    if (!is_indeterminate)
        fraction = AzSaturate(fraction);

    float fill_n0 = 0.0f;
    float fill_n1 = (fraction == fraction) ? fraction : 0.0f;

    if (is_indeterminate)
    {
        const float fill_width_n = 0.2f;
        fill_n0 = AzFmod(-fraction, 1.0f) * (1.0f + fill_width_n)
                  - fill_width_n;
        fill_n1 = AzSaturate(fill_n0 + fill_width_n);
        fill_n0 = AzSaturate(fill_n0);
    }

    RenderFrame(bb.Min,
                bb.Max,
                GetColorU32(AzCol_FrameBg),
                true,
                style.FrameRounding);
    bb.Expand(AzVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    RenderRectFilledRangeH(window->DrawList,
                           bb,
                           GetColorU32(AzCol_PlotHistogram),
                           fill_n0,
                           fill_n1,
                           style.FrameRounding);

    char overlay_buf[32];
    if (!is_indeterminate || overlay != NULL)
    {
        if (!overlay)
        {
            AzFormatString(overlay_buf,
                           AZ_ARRAYSIZE(overlay_buf),
                           "%.0f%%",
                           fraction * 100 + 0.01f);
            overlay = overlay_buf;
        }

        AzVec2 overlay_size = CalcTextSize(overlay, NULL);
        if (overlay_size.x > 0.0f)
        {
            float text_x = is_indeterminate
                                   ? (bb.Min.x + bb.Max.x - overlay_size.x)
                                             * 0.5f
                                   : AzLerp(bb.Min.x, bb.Max.x, fill_n1)
                                             + style.ItemSpacing.x;
            RenderTextClipped(
                    AzVec2(AzClamp(text_x,
                                   bb.Min.x,
                                   bb.Max.x - overlay_size.x
                                           - style.ItemInnerSpacing.x),
                           bb.Min.y),
                    bb.Max,
                    overlay,
                    NULL,
                    &overlay_size,
                    AzVec2(0.0f, 0.5f),
                    &bb);
        }
    }
}

void Az::Bullet()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const float line_height = AzMax(AzMin(window->DC.CurrLineSize.y,
                                          g.FontSize
                                                  + style.FramePadding.y * 2),
                                    g.FontSize);
    const AzRect bb(window->DC.CursorPos,
                    window->DC.CursorPos + AzVec2(g.FontSize, line_height));
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
    {
        SameLine(0, style.FramePadding.x * 2);
        return;
    }

    AzU32 text_col = GetColorU32(AzCol_Text);
    RenderBullet(window->DrawList,
                 bb.Min
                         + AzVec2(style.FramePadding.x + g.FontSize * 0.5f,
                                  line_height * 0.5f),
                 text_col);
    SameLine(0, style.FramePadding.x * 2.0f);
}

bool Az::TextLink(const char* label)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzID id = window->GetID(label);
    const char* label_end = FindRenderedTextEnd(label);

    AzVec2 pos(window->DC.CursorPos.x,
               window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    AzVec2 size = CalcTextSize(label, label_end, true);
    AzRect bb(pos, pos + size);
    ItemSize(size, 0.0f);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    RenderNavCursor(bb, id);

    if (hovered)
        SetMouseCursor(AzMouseCursor_Hand);

    AzVec4 text_colf = g.Style.Colors[AzCol_TextLink];
    AzVec4 line_colf = text_colf;
    {
        float h, s, v;
        ColorConvertRGBtoHSV(text_colf.x, text_colf.y, text_colf.z, h, s, v);
        if (held || hovered)
        {
            v = AzSaturate(v + (held ? 0.4f : 0.3f));
            h = AzFmod(h + 0.02f, 1.0f);
        }
        ColorConvertHSVtoRGB(h, s, v, text_colf.x, text_colf.y, text_colf.z);
        v = AzSaturate(v - 0.20f);
        ColorConvertHSVtoRGB(h, s, v, line_colf.x, line_colf.y, line_colf.z);
    }

    float line_y = bb.Max.y
                   + AzFloor(g.FontBaked->Descent * g.FontBakedScale * 0.20f);
    window->DrawList->AddLine(AzVec2(bb.Min.x, line_y),
                              AzVec2(bb.Max.x, line_y),
                              GetColorU32(line_colf));

    PushStyleColor(AzCol_Text, GetColorU32(text_colf));
    RenderText(bb.Min, label, label_end);
    PopStyleColor();

    AZ_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool Az::TextLinkOpenURL(const char* label, const char* url)
{
    AzContext& g = *GAz;
    if (url == NULL)
        url = label;
    bool pressed = TextLink(label);
    if (pressed && g.PlatformIO.Platform_OpenInShellFn != NULL)
        g.PlatformIO.Platform_OpenInShellFn(&g, url);
    SetItemTooltip(LocalizeGetMsg(AzLocKey_OpenLink_s), url);
    if (BeginPopupContextItem())
    {
        if (MenuItem(LocalizeGetMsg(AzLocKey_CopyLink)))
            SetClipboardText(url);
        EndPopup();
    }
    return pressed;
}

void Az::Spacing()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ItemSize(AzVec2(0, 0));
}

void Az::Dummy(const AzVec2& size)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const AzRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(size);
    ItemAdd(bb, 0);
}

void Az::NewLine()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    const AzLayoutType backup_layout_type = window->DC.LayoutType;
    window->DC.LayoutType = AzLayoutType_Vertical;
    window->DC.IsSameLine = false;
    if (window->DC.CurrLineSize.y > 0.0f)
        ItemSize(AzVec2(0, 0));
    else
        ItemSize(AzVec2(0.0f, g.FontSize));
    window->DC.LayoutType = backup_layout_type;
}

void Az::AlignTextToFramePadding()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    window->DC.CurrLineSize.y = AzMax(window->DC.CurrLineSize.y,
                                      g.FontSize + g.Style.FramePadding.y * 2);
    window->DC.CurrLineTextBaseOffset = AzMax(window->DC.CurrLineTextBaseOffset,
                                              g.Style.FramePadding.y);
}

void Az::SeparatorEx(AzSeparatorFlags flags, float thickness)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    AzContext& g = *GAz;
    AZ_ASSERT(AzIsPowerOfTwo(
            flags & (AzSeparatorFlags_Horizontal | AzSeparatorFlags_Vertical)));
    AZ_ASSERT(thickness > 0.0f);

    if (flags & AzSeparatorFlags_Vertical)
    {
        float y1 = window->DC.CursorPos.y;
        float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
        const AzRect bb(AzVec2(window->DC.CursorPos.x, y1),
                        AzVec2(window->DC.CursorPos.x + thickness, y2));
        ItemSize(AzVec2(thickness, 0.0f));
        if (!ItemAdd(bb, 0))
            return;

        window->DrawList->AddRectFilled(bb.Min,
                                        bb.Max,
                                        GetColorU32(AzCol_Separator));
        if (g.LogEnabled)
            LogText(" |");
    } else if (flags & AzSeparatorFlags_Horizontal)
    {
        float x1 = window->DC.CursorPos.x;
        float x2 = window->WorkRect.Max.x;

        AzOldColumns* columns = (flags & AzSeparatorFlags_SpanAllColumns)
                                        ? window->DC.CurrentColumns
                                        : NULL;
        if (columns)
        {
            x1 = window->Pos.x + window->DC.Indent.x;
            x2 = window->Pos.x + window->Size.x;
            PushColumnsBackground();
        }

        const float thickness_for_layout = (thickness == 1.0f) ? 0.0f
                                                               : thickness;
        const AzRect bb(AzVec2(x1, window->DC.CursorPos.y),
                        AzVec2(x2, window->DC.CursorPos.y + thickness));
        ItemSize(AzVec2(0.0f, thickness_for_layout));

        if (ItemAdd(bb, 0))
        {
            window->DrawList->AddRectFilled(bb.Min,
                                            bb.Max,
                                            GetColorU32(AzCol_Separator));
            if (g.LogEnabled)
                LogRenderedText(&bb.Min, "--------------------------------\n");
        }
        if (columns)
        {
            PopColumnsBackground();
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }
}

void Az::Separator()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    AzSeparatorFlags flags = (window->DC.LayoutType == AzLayoutType_Horizontal)
                                     ? AzSeparatorFlags_Vertical
                                     : AzSeparatorFlags_Horizontal;

    if (window->DC.CurrentColumns)
        flags |= AzSeparatorFlags_SpanAllColumns;

    SeparatorEx(flags, 1.0f);
}

void Az::SeparatorTextEx(AzID id,
                         const char* label,
                         const char* label_end,
                         float extra_w)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzStyle& style = g.Style;

    const AzVec2 label_size = CalcTextSize(label, label_end, false);
    const AzVec2 pos = window->DC.CursorPos;
    const AzVec2 padding = style.SeparatorTextPadding;

    const float separator_thickness = style.SeparatorTextBorderSize;
    const AzVec2 min_size(label_size.x + extra_w + padding.x * 2.0f,
                          AzMax(label_size.y + padding.y * 2.0f,
                                separator_thickness));
    const AzRect bb(pos, AzVec2(window->WorkRect.Max.x, pos.y + min_size.y));
    const float text_baseline_y = AzTrunc((bb.GetHeight() - label_size.y)
                                                  * style.SeparatorTextAlign.y
                                          + 0.99999f);
    ItemSize(min_size, text_baseline_y);
    if (!ItemAdd(bb, id))
        return;

    const float sep1_x1 = pos.x;
    const float sep2_x2 = bb.Max.x;
    const float seps_y = AzTrunc((bb.Min.y + bb.Max.y) * 0.5f + 0.99999f);

    const float label_avail_w = AzMax(0.0f,
                                      sep2_x2 - sep1_x1 - padding.x * 2.0f);
    const AzVec2 label_pos(
            pos.x + padding.x
                    + AzMax(0.0f,
                            (label_avail_w - label_size.x - extra_w)
                                    * style.SeparatorTextAlign.x),
            pos.y + text_baseline_y);

    window->DC.CursorPosPrevLine.x = label_pos.x + label_size.x;

    const AzU32 separator_col = GetColorU32(AzCol_Separator);
    if (label_size.x > 0.0f)
    {
        const float sep1_x2 = label_pos.x - style.ItemSpacing.x;
        const float sep2_x1 = label_pos.x + label_size.x + extra_w
                              + style.ItemSpacing.x;
        if (sep1_x2 > sep1_x1 && separator_thickness > 0.0f)
            window->DrawList->AddLine(AzVec2(sep1_x1, seps_y),
                                      AzVec2(sep1_x2, seps_y),
                                      separator_col,
                                      separator_thickness);
        if (sep2_x2 > sep2_x1 && separator_thickness > 0.0f)
            window->DrawList->AddLine(AzVec2(sep2_x1, seps_y),
                                      AzVec2(sep2_x2, seps_y),
                                      separator_col,
                                      separator_thickness);
        if (g.LogEnabled)
            LogSetNextTextDecoration("---", NULL);
        RenderTextEllipsis(window->DrawList,
                           label_pos,
                           AzVec2(bb.Max.x, bb.Max.y + style.ItemSpacing.y),
                           bb.Max.x,
                           label,
                           label_end,
                           &label_size);
    } else
    {
        if (g.LogEnabled)
            LogText("---");
        if (separator_thickness > 0.0f)
            window->DrawList->AddLine(AzVec2(sep1_x1, seps_y),
                                      AzVec2(sep2_x2, seps_y),
                                      separator_col,
                                      separator_thickness);
    }
}

void Az::SeparatorText(const char* label)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    SeparatorTextEx(0, label, FindRenderedTextEnd(label), 0.0f);
}

bool Az::SplitterBehavior(const AzRect& bb,
                          AzID id,
                          AzAxis axis,
                          float* size1,
                          float* size2,
                          float min_size1,
                          float min_size2,
                          float hover_extend,
                          float hover_visibility_delay,
                          AzU32 bg_col)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    if (!ItemAdd(bb, id, NULL, AzItemFlags_NoNav))
        return false;

    AzButtonFlags button_flags = AzButtonFlags_FlattenChildren;
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    button_flags |= AzButtonFlags_AllowOverlap;
#endif

    bool hovered, held;
    AzRect bb_interact = bb;
    bb_interact.Expand(axis == AzAxis_Y ? AzVec2(0.0f, hover_extend)
                                        : AzVec2(hover_extend, 0.0f));
    ButtonBehavior(bb_interact, id, &hovered, &held, button_flags);
    if (hovered)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredRect;

    if (held
        || (hovered && g.HoveredIdPreviousFrame == id
            && g.HoveredIdTimer >= hover_visibility_delay))
        SetMouseCursor(axis == AzAxis_Y ? AzMouseCursor_ResizeNS
                                        : AzMouseCursor_ResizeEW);

    AzRect bb_render = bb;
    if (held)
    {
        float mouse_delta = (g.IO.MousePos - g.ActiveIdClickOffset
                             - bb_interact.Min)[axis];

        float size_1_maximum_delta = AzMax(0.0f, *size1 - min_size1);
        float size_2_maximum_delta = AzMax(0.0f, *size2 - min_size2);
        if (mouse_delta < -size_1_maximum_delta)
            mouse_delta = -size_1_maximum_delta;
        if (mouse_delta > size_2_maximum_delta)
            mouse_delta = size_2_maximum_delta;

        if (mouse_delta != 0.0f)
        {
            *size1 = AzMax(*size1 + mouse_delta, min_size1);
            *size2 = AzMax(*size2 - mouse_delta, min_size2);
            bb_render.Translate((axis == AzAxis_X) ? AzVec2(mouse_delta, 0.0f)
                                                   : AzVec2(0.0f, mouse_delta));
            MarkItemEdited(id);
        }
    }

    if (bg_col & AZ_COL32_A_MASK)
        window->DrawList->AddRectFilled(bb_render.Min,
                                        bb_render.Max,
                                        bg_col,
                                        0.0f);
    const AzU32 col = GetColorU32(
            held ? AzCol_SeparatorActive
            : (hovered && g.HoveredIdTimer >= hover_visibility_delay)
                    ? AzCol_SeparatorHovered
                    : AzCol_Separator);
    window->DrawList->AddRectFilled(bb_render.Min, bb_render.Max, col, 0.0f);

    return held;
}

static int AZ_CDECL ShrinkWidthItemComparer(const void* lhs, const void* rhs)
{
    const AzShrinkWidthItem* a = (const AzShrinkWidthItem*)lhs;
    const AzShrinkWidthItem* b = (const AzShrinkWidthItem*)rhs;
    if (int d = (int)(b->Width - a->Width))
        return d;
    return (b->Index - a->Index);
}

void Az::ShrinkWidths(AzShrinkWidthItem* items, int count, float width_excess)
{
    if (count == 1)
    {
        if (items[0].Width >= 0.0f)
            items[0].Width = AzMax(items[0].Width - width_excess, 1.0f);
        return;
    }
    AzQsort(items,
            (size_t)count,
            sizeof(AzShrinkWidthItem),
            ShrinkWidthItemComparer);
    int count_same_width = 1;
    while (width_excess > 0.0f && count_same_width < count)
    {
        while (count_same_width < count
               && items[0].Width <= items[count_same_width].Width)
            count_same_width++;
        float max_width_to_remove_per_item =
                (count_same_width < count
                 && items[count_same_width].Width >= 0.0f)
                        ? (items[0].Width - items[count_same_width].Width)
                        : (items[0].Width - 1.0f);
        if (max_width_to_remove_per_item <= 0.0f)
            break;
        float width_to_remove_per_item = AzMin(width_excess / count_same_width,
                                               max_width_to_remove_per_item);
        for (int item_n = 0; item_n < count_same_width; item_n++)
            items[item_n].Width -= width_to_remove_per_item;
        width_excess -= width_to_remove_per_item * count_same_width;
    }

    width_excess = 0.0f;
    for (int n = 0; n < count; n++)
    {
        float width_rounded = AzTrunc(items[n].Width);
        width_excess += items[n].Width - width_rounded;
        items[n].Width = width_rounded;
    }
    while (width_excess > 0.0f)
        for (int n = 0; n < count && width_excess > 0.0f; n++)
        {
            float width_to_add = AzMin(items[n].InitialWidth - items[n].Width,
                                       1.0f);
            items[n].Width += width_to_add;
            width_excess -= width_to_add;
        }
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    AzContext& g = *GAz;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count
           - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool Az::BeginCombo(const char* label,
                    const char* preview_value,
                    AzComboFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();

    AzNextWindowDataFlags backup_next_window_data_flags =
            g.NextWindowData.HasFlags;
    g.NextWindowData.ClearFlags();
    if (window->SkipItems)
        return false;

    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    AZ_ASSERT((flags & (AzComboFlags_NoArrowButton | AzComboFlags_NoPreview))
              != (AzComboFlags_NoArrowButton | AzComboFlags_NoPreview));
    if (flags & AzComboFlags_WidthFitPreview)
        AZ_ASSERT((flags
                   & (AzComboFlags_NoPreview
                      | (AzComboFlags)AzComboFlags_CustomPreview))
                  == 0);

    const float arrow_size = (flags & AzComboFlags_NoArrowButton)
                                     ? 0.0f
                                     : GetFrameHeight();
    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    const float preview_width =
            ((flags & AzComboFlags_WidthFitPreview) && (preview_value != NULL))
                    ? CalcTextSize(preview_value, NULL, true).x
                    : 0.0f;
    const float w = (flags & AzComboFlags_NoPreview)
                            ? arrow_size
                            : ((flags & AzComboFlags_WidthFitPreview)
                                       ? (arrow_size + preview_width
                                          + style.FramePadding.x * 2.0f)
                                       : CalcItemWidth());
    const AzRect bb(
            window->DC.CursorPos,
            window->DC.CursorPos
                    + AzVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const AzRect total_bb(bb.Min,
                          bb.Max
                                  + AzVec2(label_size.x > 0.0f
                                                   ? style.ItemInnerSpacing.x
                                                             + label_size.x
                                                   : 0.0f,
                                           0.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &bb))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    const AzID popup_id = AzHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, AzPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, AzPopupFlags_None);
        popup_open = true;
    }

    const AzU32 frame_col = GetColorU32(hovered ? AzCol_FrameBgHovered
                                                : AzCol_FrameBg);
    const float value_x2 = AzMax(bb.Min.x, bb.Max.x - arrow_size);
    RenderNavCursor(bb, id);
    if (!(flags & AzComboFlags_NoPreview))
        window->DrawList->AddRectFilled(bb.Min,
                                        AzVec2(value_x2, bb.Max.y),
                                        frame_col,
                                        style.FrameRounding,
                                        (flags & AzComboFlags_NoArrowButton)
                                                ? AzDrawFlags_RoundCornersAll
                                                : AzDrawFlags_RoundCornersLeft);
    if (!(flags & AzComboFlags_NoArrowButton))
    {
        AzU32 bg_col = GetColorU32((popup_open || hovered) ? AzCol_ButtonHovered
                                                           : AzCol_Button);
        AzU32 text_col = GetColorU32(AzCol_Text);
        window->DrawList->AddRectFilled(
                AzVec2(value_x2, bb.Min.y),
                bb.Max,
                bg_col,
                style.FrameRounding,
                (w <= arrow_size) ? AzDrawFlags_RoundCornersAll
                                  : AzDrawFlags_RoundCornersRight);
        if (value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x)
            RenderArrow(window->DrawList,
                        AzVec2(value_x2 + style.FramePadding.y,
                               bb.Min.y + style.FramePadding.y),
                        text_col,
                        AzDir_Down,
                        1.0f);
    }
    RenderFrameBorder(bb.Min, bb.Max, style.FrameRounding);

    if (flags & AzComboFlags_CustomPreview)
    {
        g.ComboPreviewData.PreviewRect =
                AzRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
        AZ_ASSERT(preview_value == NULL || preview_value[0] == 0);
        preview_value = NULL;
    }

    if (preview_value != NULL && !(flags & AzComboFlags_NoPreview))
    {
        if (g.LogEnabled)
            LogSetNextTextDecoration("{", "}");
        RenderTextClipped(bb.Min + style.FramePadding,
                          AzVec2(value_x2, bb.Max.y),
                          preview_value,
                          NULL,
                          NULL);
    }
    if (label_size.x > 0)
        RenderText(AzVec2(bb.Max.x + style.ItemInnerSpacing.x,
                          bb.Min.y + style.FramePadding.y),
                   label);

    if (!popup_open)
        return false;

    g.NextWindowData.HasFlags = backup_next_window_data_flags;
    return BeginComboPopup(popup_id, bb, flags);
}

bool Az::BeginComboPopup(AzID popup_id, const AzRect& bb, AzComboFlags flags)
{
    AzContext& g = *GAz;
    if (!IsPopupOpen(popup_id, AzPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    float w = bb.GetWidth();
    if (g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x =
                AzMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    } else
    {
        if ((flags & AzComboFlags_HeightMask_) == 0)
            flags |= AzComboFlags_HeightRegular;
        AZ_ASSERT(AzIsPowerOfTwo(flags & AzComboFlags_HeightMask_));
        int popup_max_height_in_items = -1;
        if (flags & AzComboFlags_HeightRegular)
            popup_max_height_in_items = 8;
        else if (flags & AzComboFlags_HeightSmall)
            popup_max_height_in_items = 4;
        else if (flags & AzComboFlags_HeightLarge)
            popup_max_height_in_items = 20;
        AzVec2 constraint_min(0.0f, 0.0f), constraint_max(FLT_MAX, FLT_MAX);
        if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSize) == 0
            || g.NextWindowData.SizeVal.x <= 0.0f)
            constraint_min.x = w;
        if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasSize) == 0
            || g.NextWindowData.SizeVal.y <= 0.0f)
            constraint_max.y = CalcMaxPopupHeightFromItemCount(
                    popup_max_height_in_items);
        SetNextWindowSizeConstraints(constraint_min, constraint_max);
    }

    char name[16];
    AzFormatString(name, AZ_ARRAYSIZE(name), "##Combo_%02d", g.BeginComboDepth);

    if (AzWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            AzVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags
                                                  & AzComboFlags_PopupAlignLeft)
                                                         ? AzDir_Left
                                                         : AzDir_Down;
            AzRect r_outer = GetPopupAllowedExtentRect(popup_window);
            AzVec2 pos = FindBestWindowPosForPopupEx(
                    bb.GetBL(),
                    size_expected,
                    &popup_window->AutoPosLastDirection,
                    r_outer,
                    bb,
                    AzPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }

    AzWindowFlags window_flags = AzWindowFlags_AlwaysAutoResize
                                 | AzWindowFlags_Popup
                                 | AzWindowFlags_NoTitleBar
                                 | AzWindowFlags_NoResize
                                 | AzWindowFlags_NoSavedSettings
                                 | AzWindowFlags_NoMove;
    PushStyleVarX(AzStyleVar_WindowPadding, g.Style.FramePadding.x);
    bool ret = Begin(name, NULL, window_flags);
    PopStyleVar();
    if (!ret)
    {
        EndPopup();
        AZ_ASSERT(0);
        return false;
    }
    g.BeginComboDepth++;
    return true;
}

void Az::EndCombo()
{
    AzContext& g = *GAz;
    EndPopup();
    g.BeginComboDepth--;
}

bool Az::BeginComboPreview()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzComboPreviewData* preview_data = &g.ComboPreviewData;

    if (window->SkipItems
        || !(g.LastItemData.StatusFlags & AzItemStatusFlags_Visible))
        return false;
    AZ_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x
              && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y);
    if (!window->ClipRect.Overlaps(preview_data->PreviewRect))
        return false;

    preview_data->BackupCursorPos = window->DC.CursorPos;
    preview_data->BackupCursorMaxPos = window->DC.CursorMaxPos;
    preview_data->BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
    preview_data->BackupPrevLineTextBaseOffset =
            window->DC.PrevLineTextBaseOffset;
    preview_data->BackupLayout = window->DC.LayoutType;
    window->DC.CursorPos = preview_data->PreviewRect.Min + g.Style.FramePadding;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.LayoutType = AzLayoutType_Horizontal;
    window->DC.IsSameLine = false;
    PushClipRect(preview_data->PreviewRect.Min,
                 preview_data->PreviewRect.Max,
                 true);

    return true;
}

void Az::EndComboPreview()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzComboPreviewData* preview_data = &g.ComboPreviewData;

    AzDrawList* draw_list = window->DrawList;
    if (window->DC.CursorMaxPos.x < preview_data->PreviewRect.Max.x
        && window->DC.CursorMaxPos.y < preview_data->PreviewRect.Max.y)
        if (draw_list->CmdBuffer.Size > 1)
        {
            draw_list->_CmdHeader.ClipRect =
                    draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1]
                            .ClipRect =
                            draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 2]
                                    .ClipRect;
            draw_list->_TryMergeDrawCmds();
        }
    PopClipRect();
    window->DC.CursorPos = preview_data->BackupCursorPos;
    window->DC.CursorMaxPos = AzMax(window->DC.CursorMaxPos,
                                    preview_data->BackupCursorMaxPos);
    window->DC.CursorPosPrevLine = preview_data->BackupCursorPosPrevLine;
    window->DC.PrevLineTextBaseOffset =
            preview_data->BackupPrevLineTextBaseOffset;
    window->DC.LayoutType = preview_data->BackupLayout;
    window->DC.IsSameLine = false;
    preview_data->PreviewRect = AzRect();
}

static const char* Items_ArrayGetter(void* data, int idx)
{
    const char* const* items = (const char* const*)data;
    return items[idx];
}

static const char* Items_SingleStringGetter(void* data, int idx)
{
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += AzStrlen(p) + 1;
        items_count++;
    }
    return *p ? p : NULL;
}

bool Az::Combo(const char* label,
               int* current_item,
               const char* (*getter)(void* user_data, int idx),
               void* user_data,
               int items_count,
               int popup_max_height_in_items)
{
    AzContext& g = *GAz;

    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        preview_value = getter(user_data, *current_item);

    if (popup_max_height_in_items != -1
        && !(g.NextWindowData.HasFlags
             & AzNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(
                AzVec2(0, 0),
                AzVec2(FLT_MAX,
                       CalcMaxPopupHeightFromItemCount(
                               popup_max_height_in_items)));

    if (!BeginCombo(label, preview_value, AzComboFlags_None))
        return false;

    bool value_changed = false;
    AzListClipper clipper;
    clipper.Begin(items_count);
    clipper.IncludeItemByIndex(*current_item);
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const char* item_text = getter(user_data, i);
            if (item_text == NULL)
                item_text = "*Unknown item*";

            PushID(i);
            const bool item_selected = (i == *current_item);
            if (Selectable(item_text, item_selected) && *current_item != i)
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }

    EndCombo();
    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

bool Az::Combo(const char* label,
               int* current_item,
               const char* const items[],
               int items_count,
               int height_in_items)
{
    const bool value_changed = Combo(label,
                                     current_item,
                                     Items_ArrayGetter,
                                     (void*)items,
                                     items_count,
                                     height_in_items);
    return value_changed;
}

bool Az::Combo(const char* label,
               int* current_item,
               const char* items_separated_by_zeros,
               int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        p += AzStrlen(p) + 1;
        items_count++;
    }
    bool value_changed = Combo(label,
                               current_item,
                               Items_SingleStringGetter,
                               (void*)items_separated_by_zeros,
                               items_count,
                               height_in_items);
    return value_changed;
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

struct AzGetNameFromIndexOldToNewCallbackData
{
    void* UserData;
    bool (*OldCallback)(void*, int, const char**);
};
static const char* AzGetNameFromIndexOldToNewCallback(void* user_data, int idx)
{
    AzGetNameFromIndexOldToNewCallbackData* data =
            (AzGetNameFromIndexOldToNewCallbackData*)user_data;
    const char* s = NULL;
    data->OldCallback(data->UserData, idx, &s);
    return s;
}

bool Az::ListBox(const char* label,
                 int* current_item,
                 bool (*old_getter)(void*, int, const char**),
                 void* user_data,
                 int items_count,
                 int height_in_items)
{
    AzGetNameFromIndexOldToNewCallbackData old_to_new_data = {user_data,
                                                              old_getter};
    return ListBox(label,
                   current_item,
                   AzGetNameFromIndexOldToNewCallback,
                   &old_to_new_data,
                   items_count,
                   height_in_items);
}
bool Az::Combo(const char* label,
               int* current_item,
               bool (*old_getter)(void*, int, const char**),
               void* user_data,
               int items_count,
               int popup_max_height_in_items)
{
    AzGetNameFromIndexOldToNewCallbackData old_to_new_data = {user_data,
                                                              old_getter};
    return Combo(label,
                 current_item,
                 AzGetNameFromIndexOldToNewCallback,
                 &old_to_new_data,
                 items_count,
                 popup_max_height_in_items);
}

#endif

static const AzDataTypeInfo GDataTypeInfo[] = {
        {sizeof(char), "S8", "%d", "%d"},
        {sizeof(unsigned char), "U8", "%u", "%u"},
        {sizeof(short), "S16", "%d", "%d"},
        {sizeof(unsigned short), "U16", "%u", "%u"},
        {sizeof(int), "S32", "%d", "%d"},
        {sizeof(unsigned int), "U32", "%u", "%u"},
#ifdef _MSC_VER
        {sizeof(AzS64), "S64", "%I64d", "%I64d"},
        {sizeof(AzU64), "U64", "%I64u", "%I64u"},
#else
        {sizeof(AzS64), "S64", "%lld", "%lld"},
        {sizeof(AzU64), "U64", "%llu", "%llu"},
#endif
        {sizeof(float), "float", "%.3f", "%f"},
        {sizeof(double), "double", "%f", "%lf"},
        {sizeof(bool), "bool", "%d", "%d"},
        {0, "char*", "%s", "%s"},
};
AZ_STATIC_ASSERT(AZ_ARRAYSIZE(GDataTypeInfo) == AzDataType_COUNT);

const AzDataTypeInfo* Az::DataTypeGetInfo(AzDataType data_type)
{
    AZ_ASSERT(data_type >= 0 && data_type < AzDataType_COUNT);
    return &GDataTypeInfo[data_type];
}

int Az::DataTypeFormatString(char* buf,
                             int buf_size,
                             AzDataType data_type,
                             const void* p_data,
                             const char* format)
{
    if (data_type == AzDataType_S32 || data_type == AzDataType_U32)
        return AzFormatString(buf, buf_size, format, *(const AzU32*)p_data);
    if (data_type == AzDataType_S64 || data_type == AzDataType_U64)
        return AzFormatString(buf, buf_size, format, *(const AzU64*)p_data);
    if (data_type == AzDataType_Float)
        return AzFormatString(buf, buf_size, format, *(const float*)p_data);
    if (data_type == AzDataType_Double)
        return AzFormatString(buf, buf_size, format, *(const double*)p_data);
    if (data_type == AzDataType_S8)
        return AzFormatString(buf, buf_size, format, *(const AzS8*)p_data);
    if (data_type == AzDataType_U8)
        return AzFormatString(buf, buf_size, format, *(const AzU8*)p_data);
    if (data_type == AzDataType_S16)
        return AzFormatString(buf, buf_size, format, *(const AzS16*)p_data);
    if (data_type == AzDataType_U16)
        return AzFormatString(buf, buf_size, format, *(const AzU16*)p_data);
    AZ_ASSERT(0);
    return 0;
}

void Az::DataTypeApplyOp(AzDataType data_type,
                         int op,
                         void* output,
                         const void* arg1,
                         const void* arg2)
{
    AZ_ASSERT(op == '+' || op == '-');
    switch (data_type)
    {
    case AzDataType_S8:
        if (op == '+')
        {
            *(AzS8*)output = AzAddClampOverflow(*(const AzS8*)arg1,
                                                *(const AzS8*)arg2,
                                                AZ_S8_MIN,
                                                AZ_S8_MAX);
        }
        if (op == '-')
        {
            *(AzS8*)output = AzSubClampOverflow(*(const AzS8*)arg1,
                                                *(const AzS8*)arg2,
                                                AZ_S8_MIN,
                                                AZ_S8_MAX);
        }
        return;
    case AzDataType_U8:
        if (op == '+')
        {
            *(AzU8*)output = AzAddClampOverflow(*(const AzU8*)arg1,
                                                *(const AzU8*)arg2,
                                                AZ_U8_MIN,
                                                AZ_U8_MAX);
        }
        if (op == '-')
        {
            *(AzU8*)output = AzSubClampOverflow(*(const AzU8*)arg1,
                                                *(const AzU8*)arg2,
                                                AZ_U8_MIN,
                                                AZ_U8_MAX);
        }
        return;
    case AzDataType_S16:
        if (op == '+')
        {
            *(AzS16*)output = AzAddClampOverflow(*(const AzS16*)arg1,
                                                 *(const AzS16*)arg2,
                                                 AZ_S16_MIN,
                                                 AZ_S16_MAX);
        }
        if (op == '-')
        {
            *(AzS16*)output = AzSubClampOverflow(*(const AzS16*)arg1,
                                                 *(const AzS16*)arg2,
                                                 AZ_S16_MIN,
                                                 AZ_S16_MAX);
        }
        return;
    case AzDataType_U16:
        if (op == '+')
        {
            *(AzU16*)output = AzAddClampOverflow(*(const AzU16*)arg1,
                                                 *(const AzU16*)arg2,
                                                 AZ_U16_MIN,
                                                 AZ_U16_MAX);
        }
        if (op == '-')
        {
            *(AzU16*)output = AzSubClampOverflow(*(const AzU16*)arg1,
                                                 *(const AzU16*)arg2,
                                                 AZ_U16_MIN,
                                                 AZ_U16_MAX);
        }
        return;
    case AzDataType_S32:
        if (op == '+')
        {
            *(AzS32*)output = AzAddClampOverflow(*(const AzS32*)arg1,
                                                 *(const AzS32*)arg2,
                                                 AZ_S32_MIN,
                                                 AZ_S32_MAX);
        }
        if (op == '-')
        {
            *(AzS32*)output = AzSubClampOverflow(*(const AzS32*)arg1,
                                                 *(const AzS32*)arg2,
                                                 AZ_S32_MIN,
                                                 AZ_S32_MAX);
        }
        return;
    case AzDataType_U32:
        if (op == '+')
        {
            *(AzU32*)output = AzAddClampOverflow(*(const AzU32*)arg1,
                                                 *(const AzU32*)arg2,
                                                 AZ_U32_MIN,
                                                 AZ_U32_MAX);
        }
        if (op == '-')
        {
            *(AzU32*)output = AzSubClampOverflow(*(const AzU32*)arg1,
                                                 *(const AzU32*)arg2,
                                                 AZ_U32_MIN,
                                                 AZ_U32_MAX);
        }
        return;
    case AzDataType_S64:
        if (op == '+')
        {
            *(AzS64*)output = AzAddClampOverflow(*(const AzS64*)arg1,
                                                 *(const AzS64*)arg2,
                                                 AZ_S64_MIN,
                                                 AZ_S64_MAX);
        }
        if (op == '-')
        {
            *(AzS64*)output = AzSubClampOverflow(*(const AzS64*)arg1,
                                                 *(const AzS64*)arg2,
                                                 AZ_S64_MIN,
                                                 AZ_S64_MAX);
        }
        return;
    case AzDataType_U64:
        if (op == '+')
        {
            *(AzU64*)output = AzAddClampOverflow(*(const AzU64*)arg1,
                                                 *(const AzU64*)arg2,
                                                 AZ_U64_MIN,
                                                 AZ_U64_MAX);
        }
        if (op == '-')
        {
            *(AzU64*)output = AzSubClampOverflow(*(const AzU64*)arg1,
                                                 *(const AzU64*)arg2,
                                                 AZ_U64_MIN,
                                                 AZ_U64_MAX);
        }
        return;
    case AzDataType_Float:
        if (op == '+')
        {
            *(float*)output = *(const float*)arg1 + *(const float*)arg2;
        }
        if (op == '-')
        {
            *(float*)output = *(const float*)arg1 - *(const float*)arg2;
        }
        return;
    case AzDataType_Double:
        if (op == '+')
        {
            *(double*)output = *(const double*)arg1 + *(const double*)arg2;
        }
        if (op == '-')
        {
            *(double*)output = *(const double*)arg1 - *(const double*)arg2;
        }
        return;
    case AzDataType_COUNT:
        break;
    }
    AZ_ASSERT(0);
}

bool Az::DataTypeApplyFromText(const char* buf,
                               AzDataType data_type,
                               void* p_data,
                               const char* format,
                               void* p_data_when_empty)
{
    const AzDataTypeInfo* type_info = DataTypeGetInfo(data_type);
    AzDataTypeStorage data_backup;
    memcpy(&data_backup, p_data, type_info->Size);

    while (AzCharIsBlankA(*buf))
        buf++;
    if (!buf[0])
    {
        if (p_data_when_empty != NULL)
        {
            memcpy(p_data, p_data_when_empty, type_info->Size);
            return memcmp(&data_backup, p_data, type_info->Size) != 0;
        }
        return false;
    }

    char format_sanitized[32];
    if (data_type == AzDataType_Float || data_type == AzDataType_Double)
        format = type_info->ScanFmt;
    else
        format = AzParseFormatSanitizeForScanning(format,
                                                  format_sanitized,
                                                  AZ_ARRAYSIZE(
                                                          format_sanitized));

    int v32 = 0;
    if (sscanf(buf, format, type_info->Size >= 4 ? p_data : &v32) < 1)
        return false;
    if (type_info->Size < 4)
    {
        if (data_type == AzDataType_S8)
            *(AzS8*)p_data = (AzS8)AzClamp(v32, (int)AZ_S8_MIN, (int)AZ_S8_MAX);
        else if (data_type == AzDataType_U8)
            *(AzU8*)p_data = (AzU8)AzClamp(v32, (int)AZ_U8_MIN, (int)AZ_U8_MAX);
        else if (data_type == AzDataType_S16)
            *(AzS16*)p_data = (AzS16)AzClamp(v32,
                                             (int)AZ_S16_MIN,
                                             (int)AZ_S16_MAX);
        else if (data_type == AzDataType_U16)
            *(AzU16*)p_data = (AzU16)AzClamp(v32,
                                             (int)AZ_U16_MIN,
                                             (int)AZ_U16_MAX);
        else
            AZ_ASSERT(0);
    }

    return memcmp(&data_backup, p_data, type_info->Size) != 0;
}

template <typename T>
static int DataTypeCompareT(const T* lhs, const T* rhs)
{
    if (*lhs < *rhs)
        return -1;
    if (*lhs > *rhs)
        return +1;
    return 0;
}

int Az::DataTypeCompare(AzDataType data_type,
                        const void* arg_1,
                        const void* arg_2)
{
    switch (data_type)
    {
    case AzDataType_S8:
        return DataTypeCompareT<AzS8>((const AzS8*)arg_1, (const AzS8*)arg_2);
    case AzDataType_U8:
        return DataTypeCompareT<AzU8>((const AzU8*)arg_1, (const AzU8*)arg_2);
    case AzDataType_S16:
        return DataTypeCompareT<AzS16>((const AzS16*)arg_1,
                                       (const AzS16*)arg_2);
    case AzDataType_U16:
        return DataTypeCompareT<AzU16>((const AzU16*)arg_1,
                                       (const AzU16*)arg_2);
    case AzDataType_S32:
        return DataTypeCompareT<AzS32>((const AzS32*)arg_1,
                                       (const AzS32*)arg_2);
    case AzDataType_U32:
        return DataTypeCompareT<AzU32>((const AzU32*)arg_1,
                                       (const AzU32*)arg_2);
    case AzDataType_S64:
        return DataTypeCompareT<AzS64>((const AzS64*)arg_1,
                                       (const AzS64*)arg_2);
    case AzDataType_U64:
        return DataTypeCompareT<AzU64>((const AzU64*)arg_1,
                                       (const AzU64*)arg_2);
    case AzDataType_Float:
        return DataTypeCompareT<float>((const float*)arg_1,
                                       (const float*)arg_2);
    case AzDataType_Double:
        return DataTypeCompareT<double>((const double*)arg_1,
                                        (const double*)arg_2);
    case AzDataType_COUNT:
        break;
    }
    AZ_ASSERT(0);
    return 0;
}

template <typename T>
static bool DataTypeClampT(T* v, const T* v_min, const T* v_max)
{
    if (v_min && *v < *v_min)
    {
        *v = *v_min;
        return true;
    }
    if (v_max && *v > *v_max)
    {
        *v = *v_max;
        return true;
    }
    return false;
}

bool Az::DataTypeClamp(AzDataType data_type,
                       void* p_data,
                       const void* p_min,
                       const void* p_max)
{
    switch (data_type)
    {
    case AzDataType_S8:
        return DataTypeClampT<AzS8>((AzS8*)p_data,
                                    (const AzS8*)p_min,
                                    (const AzS8*)p_max);
    case AzDataType_U8:
        return DataTypeClampT<AzU8>((AzU8*)p_data,
                                    (const AzU8*)p_min,
                                    (const AzU8*)p_max);
    case AzDataType_S16:
        return DataTypeClampT<AzS16>((AzS16*)p_data,
                                     (const AzS16*)p_min,
                                     (const AzS16*)p_max);
    case AzDataType_U16:
        return DataTypeClampT<AzU16>((AzU16*)p_data,
                                     (const AzU16*)p_min,
                                     (const AzU16*)p_max);
    case AzDataType_S32:
        return DataTypeClampT<AzS32>((AzS32*)p_data,
                                     (const AzS32*)p_min,
                                     (const AzS32*)p_max);
    case AzDataType_U32:
        return DataTypeClampT<AzU32>((AzU32*)p_data,
                                     (const AzU32*)p_min,
                                     (const AzU32*)p_max);
    case AzDataType_S64:
        return DataTypeClampT<AzS64>((AzS64*)p_data,
                                     (const AzS64*)p_min,
                                     (const AzS64*)p_max);
    case AzDataType_U64:
        return DataTypeClampT<AzU64>((AzU64*)p_data,
                                     (const AzU64*)p_min,
                                     (const AzU64*)p_max);
    case AzDataType_Float:
        return DataTypeClampT<float>((float*)p_data,
                                     (const float*)p_min,
                                     (const float*)p_max);
    case AzDataType_Double:
        return DataTypeClampT<double>((double*)p_data,
                                      (const double*)p_min,
                                      (const double*)p_max);
    case AzDataType_COUNT:
        break;
    }
    AZ_ASSERT(0);
    return false;
}

bool Az::DataTypeIsZero(AzDataType data_type, const void* p_data)
{
    AzContext& g = *GAz;
    return DataTypeCompare(data_type, p_data, &g.DataTypeZeroValue) == 0;
}

static float GetMinimumStepAtDecimalPrecision(int decimal_precision)
{
    static const float min_steps[10] = {1.0f,
                                        0.1f,
                                        0.01f,
                                        0.001f,
                                        0.0001f,
                                        0.00001f,
                                        0.000001f,
                                        0.0000001f,
                                        0.00000001f,
                                        0.000000001f};
    if (decimal_precision < 0)
        return FLT_MIN;
    return (decimal_precision < AZ_ARRAYSIZE(min_steps))
                   ? min_steps[decimal_precision]
                   : AzPow(10.0f, (float)-decimal_precision);
}

template <typename TYPE>
TYPE Az::RoundScalarWithFormatT(const char* format,
                                AzDataType data_type,
                                TYPE v)
{
    AZ_UNUSED(data_type);
    AZ_ASSERT(data_type == AzDataType_Float || data_type == AzDataType_Double);
    const char* fmt_start = AzParseFormatFindStart(format);
    if (fmt_start[0] != '%' || fmt_start[1] == '%')
        return v;

    char fmt_sanitized[32];
    AzParseFormatSanitizeForPrinting(fmt_start,
                                     fmt_sanitized,
                                     AZ_ARRAYSIZE(fmt_sanitized));
    fmt_start = fmt_sanitized;

    char v_str[64];
    AzFormatString(v_str, AZ_ARRAYSIZE(v_str), fmt_start, v);
    const char* p = v_str;
    while (*p == ' ')
        p++;
    v = (TYPE)AzAtof(p);

    return v;
}

template <typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
bool Az::DragBehaviorT(AzDataType data_type,
                       TYPE* v,
                       float v_speed,
                       const TYPE v_min,
                       const TYPE v_max,
                       const char* format,
                       AzSliderFlags flags)
{
    AzContext& g = *GAz;
    const AzAxis axis = (flags & AzSliderFlags_Vertical) ? AzAxis_Y : AzAxis_X;
    const bool is_bounded = (v_min < v_max)
                            || ((v_min == v_max)
                                && (v_min != 0.0f
                                    || (flags & AzSliderFlags_ClampZeroRange)));
    const bool is_wrapped = is_bounded && (flags & AzSliderFlags_WrapAround);
    const bool is_logarithmic = (flags & AzSliderFlags_Logarithmic) != 0;
    const bool is_floating_point = (data_type == AzDataType_Float)
                                   || (data_type == AzDataType_Double);

    if (v_speed == 0.0f && is_bounded && (v_max - v_min < FLT_MAX))
        v_speed = (float)((v_max - v_min) * g.DragSpeedDefaultRatio);

    float adjust_delta = 0.0f;
    if (g.ActiveIdSource == AzInputSource_Mouse && IsMousePosValid()
        && IsMouseDragPastThreshold(0,
                                    g.IO.MouseDragThreshold
                                            * DRAG_MOUSE_THRESHOLD_FACTOR))
    {
        adjust_delta = g.IO.MouseDelta[axis];
        if (g.IO.KeyAlt && !(flags & AzSliderFlags_NoSpeedTweaks))
            adjust_delta *= 1.0f / 100.0f;
        if (g.IO.KeyShift && !(flags & AzSliderFlags_NoSpeedTweaks))
            adjust_delta *= 10.0f;
    } else if (g.ActiveIdSource == AzInputSource_Keyboard
               || g.ActiveIdSource == AzInputSource_Gamepad)
    {
        const int decimal_precision = is_floating_point
                                              ? AzParseFormatPrecision(format,
                                                                       3)
                                              : 0;
        const bool tweak_slow = IsKeyDown(
                (g.NavInputSource == AzInputSource_Gamepad)
                        ? AzKey_NavGamepadTweakSlow
                        : AzKey_NavKeyboardTweakSlow);
        const bool tweak_fast = IsKeyDown(
                (g.NavInputSource == AzInputSource_Gamepad)
                        ? AzKey_NavGamepadTweakFast
                        : AzKey_NavKeyboardTweakFast);
        const float tweak_factor = (flags & AzSliderFlags_NoSpeedTweaks) ? 1.0f
                                   : tweak_slow ? 1.0f / 10.0f
                                   : tweak_fast ? 10.0f
                                                : 1.0f;
        adjust_delta = GetNavTweakPressedAmount(axis) * tweak_factor;
        v_speed = AzMax(v_speed,
                        GetMinimumStepAtDecimalPrecision(decimal_precision));
    }
    adjust_delta *= v_speed;

    if (axis == AzAxis_Y)
        adjust_delta = -adjust_delta;

    if (is_logarithmic && (v_max - v_min < FLT_MAX)
        && ((v_max - v_min) > 0.000001f))
        adjust_delta /= (float)(v_max - v_min);

    const bool is_just_activated = g.ActiveIdIsJustActivated;
    const bool is_already_past_limits_and_pushing_outward =
            is_bounded && !is_wrapped
            && ((*v >= v_max && adjust_delta > 0.0f)
                || (*v <= v_min && adjust_delta < 0.0f));
    if (is_just_activated || is_already_past_limits_and_pushing_outward)
    {
        g.DragCurrentAccum = 0.0f;
        g.DragCurrentAccumDirty = false;
    } else if (adjust_delta != 0.0f)
    {
        g.DragCurrentAccum += adjust_delta;
        g.DragCurrentAccumDirty = true;
    }

    if (!g.DragCurrentAccumDirty)
        return false;

    TYPE v_cur = *v;
    FLOATTYPE v_old_ref_for_accum_remainder = (FLOATTYPE)0.0f;

    float logarithmic_zero_epsilon = 0.0f;
    const float zero_deadzone_halfsize = 0.0f;
    if (is_logarithmic)
    {
        const int decimal_precision = is_floating_point
                                              ? AzParseFormatPrecision(format,
                                                                       3)
                                              : 1;
        logarithmic_zero_epsilon = AzPow(0.1f, (float)decimal_precision);

        float v_old_parametric =
                ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                        data_type,
                        v_cur,
                        v_min,
                        v_max,
                        is_logarithmic,
                        logarithmic_zero_epsilon,
                        zero_deadzone_halfsize);
        float v_new_parametric = v_old_parametric + g.DragCurrentAccum;
        v_cur = ScaleValueFromRatioT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                data_type,
                v_new_parametric,
                v_min,
                v_max,
                is_logarithmic,
                logarithmic_zero_epsilon,
                zero_deadzone_halfsize);
        v_old_ref_for_accum_remainder = v_old_parametric;
    } else
    {
        v_cur += (SIGNEDTYPE)g.DragCurrentAccum;
    }

    if (is_floating_point && !(flags & AzSliderFlags_NoRoundToFormat))
        v_cur = RoundScalarWithFormatT<TYPE>(format, data_type, v_cur);

    g.DragCurrentAccumDirty = false;
    if (is_logarithmic)
    {
        float v_new_parametric =
                ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                        data_type,
                        v_cur,
                        v_min,
                        v_max,
                        is_logarithmic,
                        logarithmic_zero_epsilon,
                        zero_deadzone_halfsize);
        g.DragCurrentAccum -= (float)(v_new_parametric
                                      - v_old_ref_for_accum_remainder);
    } else
    {
        g.DragCurrentAccum -= (float)((SIGNEDTYPE)v_cur - (SIGNEDTYPE)*v);
    }

    if (v_cur == (TYPE)-0)
        v_cur = (TYPE)0;

    if (*v != v_cur && is_bounded)
    {
        if (is_wrapped)
        {
            if (v_cur < v_min)
                v_cur += v_max - v_min + (is_floating_point ? 0 : 1);
            if (v_cur > v_max)
                v_cur -= v_max - v_min + (is_floating_point ? 0 : 1);
        } else
        {
            if (v_cur < v_min
                || (v_cur > *v && adjust_delta < 0.0f && !is_floating_point))
                v_cur = v_min;
            if (v_cur > v_max
                || (v_cur < *v && adjust_delta > 0.0f && !is_floating_point))
                v_cur = v_max;
        }
    }

    if (*v == v_cur)
        return false;
    *v = v_cur;
    return true;
}

bool Az::DragBehavior(AzID id,
                      AzDataType data_type,
                      void* p_v,
                      float v_speed,
                      const void* p_min,
                      const void* p_max,
                      const char* format,
                      AzSliderFlags flags)
{
    AZ_ASSERT((flags == 1 || (flags & AzSliderFlags_InvalidMask_) == 0) && "Invalid AzSliderFlags flags! Has the legacy 'float power' argument been mistakenly cast to flags? Call function with AzSliderFlags_Logarithmic flags instead.");

    AzContext& g = *GAz;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == AzInputSource_Mouse && !g.IO.MouseDown[0])
            ClearActiveID();
        else if ((g.ActiveIdSource == AzInputSource_Keyboard
                  || g.ActiveIdSource == AzInputSource_Gamepad)
                 && g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
            ClearActiveID();
    }
    if (g.ActiveId != id)
        return false;
    if ((g.LastItemData.ItemFlags & AzItemFlags_ReadOnly)
        || (flags & AzSliderFlags_ReadOnly))
        return false;

    switch (data_type)
    {
    case AzDataType_S8: {
        AzS32 v32 = (AzS32) * (AzS8*)p_v;
        bool r = DragBehaviorT<AzS32, AzS32, float>(AzDataType_S32,
                                                    &v32,
                                                    v_speed,
                                                    p_min ? *(const AzS8*)p_min
                                                          : AZ_S8_MIN,
                                                    p_max ? *(const AzS8*)p_max
                                                          : AZ_S8_MAX,
                                                    format,
                                                    flags);
        if (r)
            *(AzS8*)p_v = (AzS8)v32;
        return r;
    }
    case AzDataType_U8: {
        AzU32 v32 = (AzU32) * (AzU8*)p_v;
        bool r = DragBehaviorT<AzU32, AzS32, float>(AzDataType_U32,
                                                    &v32,
                                                    v_speed,
                                                    p_min ? *(const AzU8*)p_min
                                                          : AZ_U8_MIN,
                                                    p_max ? *(const AzU8*)p_max
                                                          : AZ_U8_MAX,
                                                    format,
                                                    flags);
        if (r)
            *(AzU8*)p_v = (AzU8)v32;
        return r;
    }
    case AzDataType_S16: {
        AzS32 v32 = (AzS32) * (AzS16*)p_v;
        bool r = DragBehaviorT<AzS32, AzS32, float>(AzDataType_S32,
                                                    &v32,
                                                    v_speed,
                                                    p_min ? *(const AzS16*)p_min
                                                          : AZ_S16_MIN,
                                                    p_max ? *(const AzS16*)p_max
                                                          : AZ_S16_MAX,
                                                    format,
                                                    flags);
        if (r)
            *(AzS16*)p_v = (AzS16)v32;
        return r;
    }
    case AzDataType_U16: {
        AzU32 v32 = (AzU32) * (AzU16*)p_v;
        bool r = DragBehaviorT<AzU32, AzS32, float>(AzDataType_U32,
                                                    &v32,
                                                    v_speed,
                                                    p_min ? *(const AzU16*)p_min
                                                          : AZ_U16_MIN,
                                                    p_max ? *(const AzU16*)p_max
                                                          : AZ_U16_MAX,
                                                    format,
                                                    flags);
        if (r)
            *(AzU16*)p_v = (AzU16)v32;
        return r;
    }
    case AzDataType_S32:
        return DragBehaviorT<AzS32, AzS32, float>(data_type,
                                                  (AzS32*)p_v,
                                                  v_speed,
                                                  p_min ? *(const AzS32*)p_min
                                                        : AZ_S32_MIN,
                                                  p_max ? *(const AzS32*)p_max
                                                        : AZ_S32_MAX,
                                                  format,
                                                  flags);
    case AzDataType_U32:
        return DragBehaviorT<AzU32, AzS32, float>(data_type,
                                                  (AzU32*)p_v,
                                                  v_speed,
                                                  p_min ? *(const AzU32*)p_min
                                                        : AZ_U32_MIN,
                                                  p_max ? *(const AzU32*)p_max
                                                        : AZ_U32_MAX,
                                                  format,
                                                  flags);
    case AzDataType_S64:
        return DragBehaviorT<AzS64, AzS64, double>(data_type,
                                                   (AzS64*)p_v,
                                                   v_speed,
                                                   p_min ? *(const AzS64*)p_min
                                                         : AZ_S64_MIN,
                                                   p_max ? *(const AzS64*)p_max
                                                         : AZ_S64_MAX,
                                                   format,
                                                   flags);
    case AzDataType_U64:
        return DragBehaviorT<AzU64, AzS64, double>(data_type,
                                                   (AzU64*)p_v,
                                                   v_speed,
                                                   p_min ? *(const AzU64*)p_min
                                                         : AZ_U64_MIN,
                                                   p_max ? *(const AzU64*)p_max
                                                         : AZ_U64_MAX,
                                                   format,
                                                   flags);
    case AzDataType_Float:
        return DragBehaviorT<float, float, float>(data_type,
                                                  (float*)p_v,
                                                  v_speed,
                                                  p_min ? *(const float*)p_min
                                                        : -FLT_MAX,
                                                  p_max ? *(const float*)p_max
                                                        : FLT_MAX,
                                                  format,
                                                  flags);
    case AzDataType_Double:
        return DragBehaviorT<double, double, double>(
                data_type,
                (double*)p_v,
                v_speed,
                p_min ? *(const double*)p_min : -DBL_MAX,
                p_max ? *(const double*)p_max : DBL_MAX,
                format,
                flags);
    case AzDataType_COUNT:
        break;
    }
    AZ_ASSERT(0);
    return false;
}

bool Az::DragScalar(const char* label,
                    AzDataType data_type,
                    void* p_data,
                    float v_speed,
                    const void* p_min,
                    const void* p_max,
                    const char* format,
                    AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    const float w = CalcItemWidth();

    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    const AzRect frame_bb(
            window->DC.CursorPos,
            window->DC.CursorPos
                    + AzVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const AzRect total_bb(frame_bb.Min,
                          frame_bb.Max
                                  + AzVec2(label_size.x > 0.0f
                                                   ? style.ItemInnerSpacing.x
                                                             + label_size.x
                                                   : 0.0f,
                                           0.0f));

    const bool temp_input_allowed = (flags & AzSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb,
                 id,
                 &frame_bb,
                 temp_input_allowed ? AzItemFlags_Inputable : 0))
        return false;

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        const bool clicked = hovered
                             && IsMouseClicked(0, AzInputFlags_None, id);
        const bool double_clicked = (hovered && g.IO.MouseClickedCount[0] == 2
                                     && TestKeyOwner(AzKey_MouseLeft, id));
        const bool make_active = (clicked || double_clicked
                                  || g.NavActivateId == id);
        if (make_active && (clicked || double_clicked))
            SetKeyOwner(AzKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl) || double_clicked
                || (g.NavActivateId == id
                    && (g.NavActivateFlags & AzActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (g.IO.ConfigDragClickToInputText && temp_input_allowed
            && !temp_input_is_active)
            if (g.ActiveId == id && hovered && g.IO.MouseReleased[0]
                && !IsMouseDragPastThreshold(
                        0,
                        g.IO.MouseDragThreshold * DRAG_MOUSE_THRESHOLD_FACTOR))
            {
                g.NavActivateId = id;
                g.NavActivateFlags = AzActivateFlags_PreferInput;
                temp_input_is_active = true;
            }

        if (make_active)
            memcpy(&g.ActiveIdValueOnActivation,
                   p_data,
                   DataTypeGetInfo(data_type)->Size);

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask = (1 << AzDir_Left) | (1 << AzDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        bool clamp_enabled = false;
        if ((flags & AzSliderFlags_ClampOnInput)
            && (p_min != NULL || p_max != NULL))
        {
            const int clamp_range_dir = (p_min != NULL && p_max != NULL)
                                                ? DataTypeCompare(data_type,
                                                                  p_min,
                                                                  p_max)
                                                : 0;
            if (p_min == NULL || p_max == NULL || clamp_range_dir < 0)
                clamp_enabled = true;
            else if (clamp_range_dir == 0)
                clamp_enabled =
                        DataTypeIsZero(data_type, p_min)
                                ? ((flags & AzSliderFlags_ClampZeroRange) != 0)
                                : true;
        }
        return TempInputScalar(frame_bb,
                               id,
                               label,
                               data_type,
                               p_data,
                               format,
                               clamp_enabled ? p_min : NULL,
                               clamp_enabled ? p_max : NULL);
    }

    const AzU32 frame_col = GetColorU32(g.ActiveId == id ? AzCol_FrameBgActive
                                        : hovered        ? AzCol_FrameBgHovered
                                                         : AzCol_FrameBg);
    RenderNavCursor(frame_bb, id);
    RenderFrame(frame_bb.Min,
                frame_bb.Max,
                frame_col,
                true,
                style.FrameRounding);

    const bool value_changed = DragBehavior(id,
                                            data_type,
                                            p_data,
                                            v_speed,
                                            p_min,
                                            p_max,
                                            format,
                                            flags);
    if (value_changed)
        MarkItemEdited(id);

    char value_buf[64];
    const char* value_buf_end = value_buf
                                + DataTypeFormatString(value_buf,
                                                       AZ_ARRAYSIZE(value_buf),
                                                       data_type,
                                                       p_data,
                                                       format);
    if (g.LogEnabled)
        LogSetNextTextDecoration("{", "}");
    RenderTextClipped(frame_bb.Min,
                      frame_bb.Max,
                      value_buf,
                      value_buf_end,
                      NULL,
                      AzVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(AzVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                          frame_bb.Min.y + style.FramePadding.y),
                   label);

    AZ_TEST_ENGINE_ITEM_INFO(id,
                             label,
                             g.LastItemData.StatusFlags
                                     | (temp_input_allowed
                                                ? AzItemStatusFlags_Inputable
                                                : 0));
    return value_changed;
}

bool Az::DragScalarN(const char* label,
                     AzDataType data_type,
                     void* p_data,
                     int components,
                     float v_speed,
                     const void* p_min,
                     const void* p_max,
                     const char* format,
                     AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = GDataTypeInfo[data_type].Size;
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= DragScalar("",
                                    data_type,
                                    p_data,
                                    v_speed,
                                    p_min,
                                    p_max,
                                    format,
                                    flags);
        PopID();
        PopItemWidth();
        p_data = (void*)((char*)p_data + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool Az::DragFloat(const char* label,
                   float* v,
                   float v_speed,
                   float v_min,
                   float v_max,
                   const char* format,
                   AzSliderFlags flags)
{
    return DragScalar(label,
                      AzDataType_Float,
                      v,
                      v_speed,
                      &v_min,
                      &v_max,
                      format,
                      flags);
}

bool Az::DragFloat2(const char* label,
                    float v[2],
                    float v_speed,
                    float v_min,
                    float v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return DragScalarN(label,
                       AzDataType_Float,
                       v,
                       2,
                       v_speed,
                       &v_min,
                       &v_max,
                       format,
                       flags);
}

bool Az::DragFloat3(const char* label,
                    float v[3],
                    float v_speed,
                    float v_min,
                    float v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return DragScalarN(label,
                       AzDataType_Float,
                       v,
                       3,
                       v_speed,
                       &v_min,
                       &v_max,
                       format,
                       flags);
}

bool Az::DragFloat4(const char* label,
                    float v[4],
                    float v_speed,
                    float v_min,
                    float v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return DragScalarN(label,
                       AzDataType_Float,
                       v,
                       4,
                       v_speed,
                       &v_min,
                       &v_max,
                       format,
                       flags);
}

bool Az::DragFloatRange2(const char* label,
                         float* v_current_min,
                         float* v_current_max,
                         float v_speed,
                         float v_min,
                         float v_max,
                         const char* format,
                         const char* format_max,
                         AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2, CalcItemWidth());

    float min_min = (v_min >= v_max) ? -FLT_MAX : v_min;
    float min_max = (v_min >= v_max) ? *v_current_max
                                     : AzMin(v_max, *v_current_max);
    AzSliderFlags min_flags = flags
                              | ((min_min == min_max) ? AzSliderFlags_ReadOnly
                                                      : 0);
    bool value_changed = DragScalar("##min",
                                    AzDataType_Float,
                                    v_current_min,
                                    v_speed,
                                    &min_min,
                                    &min_max,
                                    format,
                                    min_flags);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    float max_min = (v_min >= v_max) ? *v_current_min
                                     : AzMax(v_min, *v_current_min);
    float max_max = (v_min >= v_max) ? FLT_MAX : v_max;
    AzSliderFlags max_flags = flags
                              | ((max_min == max_max) ? AzSliderFlags_ReadOnly
                                                      : 0);
    value_changed |= DragScalar("##max",
                                AzDataType_Float,
                                v_current_max,
                                v_speed,
                                &max_min,
                                &max_max,
                                format_max ? format_max : format,
                                max_flags);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextEx(label, FindRenderedTextEnd(label));
    EndGroup();
    PopID();

    return value_changed;
}

bool Az::DragInt(const char* label,
                 int* v,
                 float v_speed,
                 int v_min,
                 int v_max,
                 const char* format,
                 AzSliderFlags flags)
{
    return DragScalar(label,
                      AzDataType_S32,
                      v,
                      v_speed,
                      &v_min,
                      &v_max,
                      format,
                      flags);
}

bool Az::DragInt2(const char* label,
                  int v[2],
                  float v_speed,
                  int v_min,
                  int v_max,
                  const char* format,
                  AzSliderFlags flags)
{
    return DragScalarN(label,
                       AzDataType_S32,
                       v,
                       2,
                       v_speed,
                       &v_min,
                       &v_max,
                       format,
                       flags);
}

bool Az::DragInt3(const char* label,
                  int v[3],
                  float v_speed,
                  int v_min,
                  int v_max,
                  const char* format,
                  AzSliderFlags flags)
{
    return DragScalarN(label,
                       AzDataType_S32,
                       v,
                       3,
                       v_speed,
                       &v_min,
                       &v_max,
                       format,
                       flags);
}

bool Az::DragInt4(const char* label,
                  int v[4],
                  float v_speed,
                  int v_min,
                  int v_max,
                  const char* format,
                  AzSliderFlags flags)
{
    return DragScalarN(label,
                       AzDataType_S32,
                       v,
                       4,
                       v_speed,
                       &v_min,
                       &v_max,
                       format,
                       flags);
}

bool Az::DragIntRange2(const char* label,
                       int* v_current_min,
                       int* v_current_max,
                       float v_speed,
                       int v_min,
                       int v_max,
                       const char* format,
                       const char* format_max,
                       AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2, CalcItemWidth());

    int min_min = (v_min >= v_max) ? INT_MIN : v_min;
    int min_max = (v_min >= v_max) ? *v_current_max
                                   : AzMin(v_max, *v_current_max);
    AzSliderFlags min_flags = flags
                              | ((min_min == min_max) ? AzSliderFlags_ReadOnly
                                                      : 0);
    bool value_changed = DragInt("##min",
                                 v_current_min,
                                 v_speed,
                                 min_min,
                                 min_max,
                                 format,
                                 min_flags);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    int max_min = (v_min >= v_max) ? *v_current_min
                                   : AzMax(v_min, *v_current_min);
    int max_max = (v_min >= v_max) ? INT_MAX : v_max;
    AzSliderFlags max_flags = flags
                              | ((max_min == max_max) ? AzSliderFlags_ReadOnly
                                                      : 0);
    value_changed |= DragInt("##max",
                             v_current_max,
                             v_speed,
                             max_min,
                             max_max,
                             format_max ? format_max : format,
                             max_flags);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextEx(label, FindRenderedTextEnd(label));
    EndGroup();
    PopID();

    return value_changed;
}

template <typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
float Az::ScaleRatioFromValueT(AzDataType data_type,
                               TYPE v,
                               TYPE v_min,
                               TYPE v_max,
                               bool is_logarithmic,
                               float logarithmic_zero_epsilon,
                               float zero_deadzone_halfsize)
{
    if (v_min == v_max)
        return 0.0f;
    AZ_UNUSED(data_type);

    const TYPE v_clamped = (v_min < v_max) ? AzClamp(v, v_min, v_max)
                                           : AzClamp(v, v_max, v_min);
    if (is_logarithmic)
    {
        bool flipped = v_max < v_min;

        if (flipped)
            AzSwap(v_min, v_max);

        FLOATTYPE v_min_fudged = (AzAbs((FLOATTYPE)v_min)
                                  < logarithmic_zero_epsilon)
                                         ? ((v_min < 0.0f)
                                                    ? -logarithmic_zero_epsilon
                                                    : logarithmic_zero_epsilon)
                                         : (FLOATTYPE)v_min;
        FLOATTYPE v_max_fudged = (AzAbs((FLOATTYPE)v_max)
                                  < logarithmic_zero_epsilon)
                                         ? ((v_max < 0.0f)
                                                    ? -logarithmic_zero_epsilon
                                                    : logarithmic_zero_epsilon)
                                         : (FLOATTYPE)v_max;

        if ((v_min == 0.0f) && (v_max < 0.0f))
            v_min_fudged = -logarithmic_zero_epsilon;
        else if ((v_max == 0.0f) && (v_min < 0.0f))
            v_max_fudged = -logarithmic_zero_epsilon;

        float result;
        if (v_clamped <= v_min_fudged)
            result = 0.0f;
        else if (v_clamped >= v_max_fudged)
            result = 1.0f;
        else if ((v_min * v_max) < 0.0f)
        {
            float zero_point_center = (-(float)v_min)
                                      / ((float)v_max - (float)v_min);
            float zero_point_snap_L = zero_point_center
                                      - zero_deadzone_halfsize;
            float zero_point_snap_R = zero_point_center
                                      + zero_deadzone_halfsize;
            if (v == 0.0f)
                result = zero_point_center;
            else if (v < 0.0f)
                result = (1.0f
                          - (float)(AzLog(-(FLOATTYPE)v_clamped
                                          / logarithmic_zero_epsilon)
                                    / AzLog(-v_min_fudged
                                            / logarithmic_zero_epsilon)))
                         * zero_point_snap_L;
            else
                result = zero_point_snap_R
                         + ((float)(AzLog((FLOATTYPE)v_clamped
                                          / logarithmic_zero_epsilon)
                                    / AzLog(v_max_fudged
                                            / logarithmic_zero_epsilon))
                            * (1.0f - zero_point_snap_R));
        } else if ((v_min < 0.0f) || (v_max < 0.0f))
            result = 1.0f
                     - (float)(AzLog(-(FLOATTYPE)v_clamped / -v_max_fudged)
                               / AzLog(-v_min_fudged / -v_max_fudged));
        else
            result = (float)(AzLog((FLOATTYPE)v_clamped / v_min_fudged)
                             / AzLog(v_max_fudged / v_min_fudged));

        return flipped ? (1.0f - result) : result;
    } else
    {
        return (float)((FLOATTYPE)(SIGNEDTYPE)(v_clamped - v_min)
                       / (FLOATTYPE)(SIGNEDTYPE)(v_max - v_min));
    }
}

template <typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
TYPE Az::ScaleValueFromRatioT(AzDataType data_type,
                              float t,
                              TYPE v_min,
                              TYPE v_max,
                              bool is_logarithmic,
                              float logarithmic_zero_epsilon,
                              float zero_deadzone_halfsize)
{
    if (t <= 0.0f || v_min == v_max)
        return v_min;
    if (t >= 1.0f)
        return v_max;

    TYPE result = (TYPE)0;
    if (is_logarithmic)
    {
        FLOATTYPE v_min_fudged = (AzAbs((FLOATTYPE)v_min)
                                  < logarithmic_zero_epsilon)
                                         ? ((v_min < 0.0f)
                                                    ? -logarithmic_zero_epsilon
                                                    : logarithmic_zero_epsilon)
                                         : (FLOATTYPE)v_min;
        FLOATTYPE v_max_fudged = (AzAbs((FLOATTYPE)v_max)
                                  < logarithmic_zero_epsilon)
                                         ? ((v_max < 0.0f)
                                                    ? -logarithmic_zero_epsilon
                                                    : logarithmic_zero_epsilon)
                                         : (FLOATTYPE)v_max;

        const bool flipped = v_max < v_min;
        if (flipped)
            AzSwap(v_min_fudged, v_max_fudged);

        if ((v_max == 0.0f) && (v_min < 0.0f))
            v_max_fudged = -logarithmic_zero_epsilon;

        float t_with_flip = flipped ? (1.0f - t) : t;

        if ((v_min * v_max) < 0.0f)
        {
            float zero_point_center = (-(float)AzMin(v_min, v_max))
                                      / AzAbs((float)v_max - (float)v_min);
            float zero_point_snap_L = zero_point_center
                                      - zero_deadzone_halfsize;
            float zero_point_snap_R = zero_point_center
                                      + zero_deadzone_halfsize;
            if (t_with_flip >= zero_point_snap_L
                && t_with_flip <= zero_point_snap_R)
                result = (TYPE)0.0f;
            else if (t_with_flip < zero_point_center)
                result = (TYPE)
                         - (logarithmic_zero_epsilon
                            * AzPow(-v_min_fudged / logarithmic_zero_epsilon,
                                    (FLOATTYPE)(1.0f
                                                - (t_with_flip
                                                   / zero_point_snap_L))));
            else
                result = (TYPE)(logarithmic_zero_epsilon
                                * AzPow(v_max_fudged / logarithmic_zero_epsilon,
                                        (FLOATTYPE)((t_with_flip
                                                     - zero_point_snap_R)
                                                    / (1.0f
                                                       - zero_point_snap_R))));
        } else if ((v_min < 0.0f) || (v_max < 0.0f))
            result = (TYPE)
                     - (-v_max_fudged
                        * AzPow(-v_min_fudged / -v_max_fudged,
                                (FLOATTYPE)(1.0f - t_with_flip)));
        else
            result = (TYPE)(v_min_fudged
                            * AzPow(v_max_fudged / v_min_fudged,
                                    (FLOATTYPE)t_with_flip));
    } else
    {
        const bool is_floating_point = (data_type == AzDataType_Float)
                                       || (data_type == AzDataType_Double);
        if (is_floating_point)
        {
            result = AzLerp(v_min, v_max, t);
        } else if (t < 1.0)
        {
            FLOATTYPE v_new_off_f = (SIGNEDTYPE)(v_max - v_min) * t;
            result = (TYPE)((SIGNEDTYPE)v_min
                            + (SIGNEDTYPE)(v_new_off_f
                                           + (FLOATTYPE)(v_min > v_max ? -0.5
                                                                       : 0.5)));
        }
    }

    return result;
}

template <typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
bool Az::SliderBehaviorT(const AzRect& bb,
                         AzID id,
                         AzDataType data_type,
                         TYPE* v,
                         const TYPE v_min,
                         const TYPE v_max,
                         const char* format,
                         AzSliderFlags flags,
                         AzRect* out_grab_bb)
{
    AzContext& g = *GAz;
    const AzStyle& style = g.Style;

    const AzAxis axis = (flags & AzSliderFlags_Vertical) ? AzAxis_Y : AzAxis_X;
    const bool is_logarithmic = (flags & AzSliderFlags_Logarithmic) != 0;
    const bool is_floating_point = (data_type == AzDataType_Float)
                                   || (data_type == AzDataType_Double);
    const float v_range_f = (float)(v_min < v_max ? v_max - v_min
                                                  : v_min - v_max);

    const float grab_padding = 2.0f;
    const float slider_sz = (bb.Max[axis] - bb.Min[axis]) - grab_padding * 2.0f;
    float grab_sz = style.GrabMinSize;
    if (!is_floating_point && v_range_f >= 0.0f)
        grab_sz = AzMax(slider_sz / (v_range_f + 1), style.GrabMinSize);
    grab_sz = AzMin(grab_sz, slider_sz);
    const float slider_usable_sz = slider_sz - grab_sz;
    const float slider_usable_pos_min = bb.Min[axis] + grab_padding
                                        + grab_sz * 0.5f;
    const float slider_usable_pos_max = bb.Max[axis] - grab_padding
                                        - grab_sz * 0.5f;

    float logarithmic_zero_epsilon = 0.0f;
    float zero_deadzone_halfsize = 0.0f;
    if (is_logarithmic)
    {
        const int decimal_precision = is_floating_point
                                              ? AzParseFormatPrecision(format,
                                                                       3)
                                              : 1;
        logarithmic_zero_epsilon = AzPow(0.1f, (float)decimal_precision);
        zero_deadzone_halfsize = (style.LogSliderDeadzone * 0.5f)
                                 / AzMax(slider_usable_sz, 1.0f);
    }

    bool value_changed = false;
    if (g.ActiveId == id)
    {
        bool set_new_value = false;
        float clicked_t = 0.0f;
        if (g.ActiveIdSource == AzInputSource_Mouse)
        {
            if (!g.IO.MouseDown[0])
            {
                ClearActiveID();
            } else
            {
                const float mouse_abs_pos = g.IO.MousePos[axis];
                if (g.ActiveIdIsJustActivated)
                {
                    float grab_t =
                            ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                                    data_type,
                                    *v,
                                    v_min,
                                    v_max,
                                    is_logarithmic,
                                    logarithmic_zero_epsilon,
                                    zero_deadzone_halfsize);
                    if (axis == AzAxis_Y)
                        grab_t = 1.0f - grab_t;
                    const float grab_pos = AzLerp(slider_usable_pos_min,
                                                  slider_usable_pos_max,
                                                  grab_t);
                    const bool clicked_around_grab =
                            (mouse_abs_pos >= grab_pos - grab_sz * 0.5f - 1.0f)
                            && (mouse_abs_pos
                                <= grab_pos + grab_sz * 0.5f + 1.0f);
                    g.SliderGrabClickOffset = (clicked_around_grab
                                               && is_floating_point)
                                                      ? mouse_abs_pos - grab_pos
                                                      : 0.0f;
                }
                if (slider_usable_sz > 0.0f)
                    clicked_t = AzSaturate((mouse_abs_pos
                                            - g.SliderGrabClickOffset
                                            - slider_usable_pos_min)
                                           / slider_usable_sz);
                if (axis == AzAxis_Y)
                    clicked_t = 1.0f - clicked_t;
                set_new_value = true;
            }
        } else if (g.ActiveIdSource == AzInputSource_Keyboard
                   || g.ActiveIdSource == AzInputSource_Gamepad)
        {
            if (g.ActiveIdIsJustActivated)
            {
                g.SliderCurrentAccum = 0.0f;
                g.SliderCurrentAccumDirty = false;
            }

            float input_delta = (axis == AzAxis_X)
                                        ? GetNavTweakPressedAmount(axis)
                                        : -GetNavTweakPressedAmount(axis);
            if (input_delta != 0.0f)
            {
                const bool tweak_slow = IsKeyDown(
                        (g.NavInputSource == AzInputSource_Gamepad)
                                ? AzKey_NavGamepadTweakSlow
                                : AzKey_NavKeyboardTweakSlow);
                const bool tweak_fast = IsKeyDown(
                        (g.NavInputSource == AzInputSource_Gamepad)
                                ? AzKey_NavGamepadTweakFast
                                : AzKey_NavKeyboardTweakFast);
                const int decimal_precision =
                        is_floating_point ? AzParseFormatPrecision(format, 3)
                                          : 0;
                if (decimal_precision > 0)
                {
                    input_delta /= 100.0f;
                    if (tweak_slow)
                        input_delta /= 10.0f;
                } else
                {
                    if ((v_range_f >= -100.0f && v_range_f <= 100.0f
                         && v_range_f != 0.0f)
                        || tweak_slow)
                        input_delta = ((input_delta < 0.0f) ? -1.0f : +1.0f)
                                      / v_range_f;
                    else
                        input_delta /= 100.0f;
                }
                if (tweak_fast)
                    input_delta *= 10.0f;

                g.SliderCurrentAccum += input_delta;
                g.SliderCurrentAccumDirty = true;
            }

            float delta = g.SliderCurrentAccum;
            if (g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
            {
                ClearActiveID();
            } else if (g.SliderCurrentAccumDirty)
            {
                clicked_t = ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                        data_type,
                        *v,
                        v_min,
                        v_max,
                        is_logarithmic,
                        logarithmic_zero_epsilon,
                        zero_deadzone_halfsize);

                if ((clicked_t >= 1.0f && delta > 0.0f)
                    || (clicked_t <= 0.0f && delta < 0.0f))
                {
                    set_new_value = false;
                    g.SliderCurrentAccum = 0.0f;
                } else
                {
                    set_new_value = true;
                    float old_clicked_t = clicked_t;
                    clicked_t = AzSaturate(clicked_t + delta);

                    TYPE v_new =
                            ScaleValueFromRatioT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                                    data_type,
                                    clicked_t,
                                    v_min,
                                    v_max,
                                    is_logarithmic,
                                    logarithmic_zero_epsilon,
                                    zero_deadzone_halfsize);
                    if (is_floating_point
                        && !(flags & AzSliderFlags_NoRoundToFormat))
                        v_new = RoundScalarWithFormatT<TYPE>(format,
                                                             data_type,
                                                             v_new);
                    float new_clicked_t =
                            ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                                    data_type,
                                    v_new,
                                    v_min,
                                    v_max,
                                    is_logarithmic,
                                    logarithmic_zero_epsilon,
                                    zero_deadzone_halfsize);

                    if (delta > 0)
                        g.SliderCurrentAccum -= AzMin(new_clicked_t
                                                              - old_clicked_t,
                                                      delta);
                    else
                        g.SliderCurrentAccum -= AzMax(new_clicked_t
                                                              - old_clicked_t,
                                                      delta);
                }

                g.SliderCurrentAccumDirty = false;
            }
        }

        if (set_new_value)
            if ((g.LastItemData.ItemFlags & AzItemFlags_ReadOnly)
                || (flags & AzSliderFlags_ReadOnly))
                set_new_value = false;

        if (set_new_value)
        {
            TYPE v_new = ScaleValueFromRatioT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                    data_type,
                    clicked_t,
                    v_min,
                    v_max,
                    is_logarithmic,
                    logarithmic_zero_epsilon,
                    zero_deadzone_halfsize);

            if (is_floating_point && !(flags & AzSliderFlags_NoRoundToFormat))
                v_new = RoundScalarWithFormatT<TYPE>(format, data_type, v_new);

            if (*v != v_new)
            {
                *v = v_new;
                value_changed = true;
            }
        }
    }

    if (slider_sz < 1.0f)
    {
        *out_grab_bb = AzRect(bb.Min, bb.Min);
    } else
    {
        float grab_t = ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(
                data_type,
                *v,
                v_min,
                v_max,
                is_logarithmic,
                logarithmic_zero_epsilon,
                zero_deadzone_halfsize);
        if (axis == AzAxis_Y)
            grab_t = 1.0f - grab_t;
        const float grab_pos = AzLerp(slider_usable_pos_min,
                                      slider_usable_pos_max,
                                      grab_t);
        if (axis == AzAxis_X)
            *out_grab_bb = AzRect(grab_pos - grab_sz * 0.5f,
                                  bb.Min.y + grab_padding,
                                  grab_pos + grab_sz * 0.5f,
                                  bb.Max.y - grab_padding);
        else
            *out_grab_bb = AzRect(bb.Min.x + grab_padding,
                                  grab_pos - grab_sz * 0.5f,
                                  bb.Max.x - grab_padding,
                                  grab_pos + grab_sz * 0.5f);
    }

    return value_changed;
}

bool Az::SliderBehavior(const AzRect& bb,
                        AzID id,
                        AzDataType data_type,
                        void* p_v,
                        const void* p_min,
                        const void* p_max,
                        const char* format,
                        AzSliderFlags flags,
                        AzRect* out_grab_bb)
{
    AZ_ASSERT((flags == 1 || (flags & AzSliderFlags_InvalidMask_) == 0) && "Invalid AzSliderFlags flags! Has the legacy 'float power' argument been mistakenly cast to flags? Call function with AzSliderFlags_Logarithmic flags instead.");
    AZ_ASSERT((flags & AzSliderFlags_WrapAround) == 0);

    switch (data_type)
    {
    case AzDataType_S8: {
        AzS32 v32 = (AzS32) * (AzS8*)p_v;
        bool r = SliderBehaviorT<AzS32, AzS32, float>(bb,
                                                      id,
                                                      AzDataType_S32,
                                                      &v32,
                                                      *(const AzS8*)p_min,
                                                      *(const AzS8*)p_max,
                                                      format,
                                                      flags,
                                                      out_grab_bb);
        if (r)
            *(AzS8*)p_v = (AzS8)v32;
        return r;
    }
    case AzDataType_U8: {
        AzU32 v32 = (AzU32) * (AzU8*)p_v;
        bool r = SliderBehaviorT<AzU32, AzS32, float>(bb,
                                                      id,
                                                      AzDataType_U32,
                                                      &v32,
                                                      *(const AzU8*)p_min,
                                                      *(const AzU8*)p_max,
                                                      format,
                                                      flags,
                                                      out_grab_bb);
        if (r)
            *(AzU8*)p_v = (AzU8)v32;
        return r;
    }
    case AzDataType_S16: {
        AzS32 v32 = (AzS32) * (AzS16*)p_v;
        bool r = SliderBehaviorT<AzS32, AzS32, float>(bb,
                                                      id,
                                                      AzDataType_S32,
                                                      &v32,
                                                      *(const AzS16*)p_min,
                                                      *(const AzS16*)p_max,
                                                      format,
                                                      flags,
                                                      out_grab_bb);
        if (r)
            *(AzS16*)p_v = (AzS16)v32;
        return r;
    }
    case AzDataType_U16: {
        AzU32 v32 = (AzU32) * (AzU16*)p_v;
        bool r = SliderBehaviorT<AzU32, AzS32, float>(bb,
                                                      id,
                                                      AzDataType_U32,
                                                      &v32,
                                                      *(const AzU16*)p_min,
                                                      *(const AzU16*)p_max,
                                                      format,
                                                      flags,
                                                      out_grab_bb);
        if (r)
            *(AzU16*)p_v = (AzU16)v32;
        return r;
    }
    case AzDataType_S32:
        AZ_ASSERT(*(const AzS32*)p_min >= AZ_S32_MIN / 2
                  && *(const AzS32*)p_max <= AZ_S32_MAX / 2);
        return SliderBehaviorT<AzS32, AzS32, float>(bb,
                                                    id,
                                                    data_type,
                                                    (AzS32*)p_v,
                                                    *(const AzS32*)p_min,
                                                    *(const AzS32*)p_max,
                                                    format,
                                                    flags,
                                                    out_grab_bb);
    case AzDataType_U32:
        AZ_ASSERT(*(const AzU32*)p_max <= AZ_U32_MAX / 2);
        return SliderBehaviorT<AzU32, AzS32, float>(bb,
                                                    id,
                                                    data_type,
                                                    (AzU32*)p_v,
                                                    *(const AzU32*)p_min,
                                                    *(const AzU32*)p_max,
                                                    format,
                                                    flags,
                                                    out_grab_bb);
    case AzDataType_S64:
        AZ_ASSERT(*(const AzS64*)p_min >= AZ_S64_MIN / 2
                  && *(const AzS64*)p_max <= AZ_S64_MAX / 2);
        return SliderBehaviorT<AzS64, AzS64, double>(bb,
                                                     id,
                                                     data_type,
                                                     (AzS64*)p_v,
                                                     *(const AzS64*)p_min,
                                                     *(const AzS64*)p_max,
                                                     format,
                                                     flags,
                                                     out_grab_bb);
    case AzDataType_U64:
        AZ_ASSERT(*(const AzU64*)p_max <= AZ_U64_MAX / 2);
        return SliderBehaviorT<AzU64, AzS64, double>(bb,
                                                     id,
                                                     data_type,
                                                     (AzU64*)p_v,
                                                     *(const AzU64*)p_min,
                                                     *(const AzU64*)p_max,
                                                     format,
                                                     flags,
                                                     out_grab_bb);
    case AzDataType_Float:
        AZ_ASSERT(*(const float*)p_min >= -FLT_MAX / 2.0f
                  && *(const float*)p_max <= FLT_MAX / 2.0f);
        return SliderBehaviorT<float, float, float>(bb,
                                                    id,
                                                    data_type,
                                                    (float*)p_v,
                                                    *(const float*)p_min,
                                                    *(const float*)p_max,
                                                    format,
                                                    flags,
                                                    out_grab_bb);
    case AzDataType_Double:
        AZ_ASSERT(*(const double*)p_min >= -DBL_MAX / 2.0f
                  && *(const double*)p_max <= DBL_MAX / 2.0f);
        return SliderBehaviorT<double, double, double>(bb,
                                                       id,
                                                       data_type,
                                                       (double*)p_v,
                                                       *(const double*)p_min,
                                                       *(const double*)p_max,
                                                       format,
                                                       flags,
                                                       out_grab_bb);
    case AzDataType_COUNT:
        break;
    }
    AZ_ASSERT(0);
    return false;
}

bool Az::SliderScalar(const char* label,
                      AzDataType data_type,
                      void* p_data,
                      const void* p_min,
                      const void* p_max,
                      const char* format,
                      AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    const float w = CalcItemWidth();

    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    const AzRect frame_bb(
            window->DC.CursorPos,
            window->DC.CursorPos
                    + AzVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const AzRect total_bb(frame_bb.Min,
                          frame_bb.Max
                                  + AzVec2(label_size.x > 0.0f
                                                   ? style.ItemInnerSpacing.x
                                                             + label_size.x
                                                   : 0.0f,
                                           0.0f));

    const bool temp_input_allowed = (flags & AzSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb,
                 id,
                 &frame_bb,
                 temp_input_allowed ? AzItemFlags_Inputable : 0))
        return false;

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        const bool clicked = hovered
                             && IsMouseClicked(0, AzInputFlags_None, id);
        const bool make_active = (clicked || g.NavActivateId == id);
        if (make_active && clicked)
            SetKeyOwner(AzKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl)
                || (g.NavActivateId == id
                    && (g.NavActivateFlags & AzActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (make_active)
            memcpy(&g.ActiveIdValueOnActivation,
                   p_data,
                   DataTypeGetInfo(data_type)->Size);

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << AzDir_Left) | (1 << AzDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        const bool clamp_enabled = (flags & AzSliderFlags_ClampOnInput) != 0;
        return TempInputScalar(frame_bb,
                               id,
                               label,
                               data_type,
                               p_data,
                               format,
                               clamp_enabled ? p_min : NULL,
                               clamp_enabled ? p_max : NULL);
    }

    const AzU32 frame_col = GetColorU32(g.ActiveId == id ? AzCol_FrameBgActive
                                        : hovered        ? AzCol_FrameBgHovered
                                                         : AzCol_FrameBg);
    RenderNavCursor(frame_bb, id);
    RenderFrame(frame_bb.Min,
                frame_bb.Max,
                frame_col,
                true,
                g.Style.FrameRounding);

    AzRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb,
                                              id,
                                              data_type,
                                              p_data,
                                              p_min,
                                              p_max,
                                              format,
                                              flags,
                                              &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(
                grab_bb.Min,
                grab_bb.Max,
                GetColorU32(g.ActiveId == id ? AzCol_SliderGrabActive
                                             : AzCol_SliderGrab),
                style.GrabRounding);

    char value_buf[64];
    const char* value_buf_end = value_buf
                                + DataTypeFormatString(value_buf,
                                                       AZ_ARRAYSIZE(value_buf),
                                                       data_type,
                                                       p_data,
                                                       format);
    if (g.LogEnabled)
        LogSetNextTextDecoration("{", "}");
    RenderTextClipped(frame_bb.Min,
                      frame_bb.Max,
                      value_buf,
                      value_buf_end,
                      NULL,
                      AzVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(AzVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                          frame_bb.Min.y + style.FramePadding.y),
                   label);

    AZ_TEST_ENGINE_ITEM_INFO(id,
                             label,
                             g.LastItemData.StatusFlags
                                     | (temp_input_allowed
                                                ? AzItemStatusFlags_Inputable
                                                : 0));
    return value_changed;
}

bool Az::SliderScalarN(const char* label,
                       AzDataType data_type,
                       void* v,
                       int components,
                       const void* v_min,
                       const void* v_max,
                       const char* format,
                       AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = GDataTypeInfo[data_type].Size;
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |=
                SliderScalar("", data_type, v, v_min, v_max, format, flags);
        PopID();
        PopItemWidth();
        v = (void*)((char*)v + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool Az::SliderFloat(const char* label,
                     float* v,
                     float v_min,
                     float v_max,
                     const char* format,
                     AzSliderFlags flags)
{
    return SliderScalar(label,
                        AzDataType_Float,
                        v,
                        &v_min,
                        &v_max,
                        format,
                        flags);
}

bool Az::SliderFloat2(const char* label,
                      float v[2],
                      float v_min,
                      float v_max,
                      const char* format,
                      AzSliderFlags flags)
{
    return SliderScalarN(label,
                         AzDataType_Float,
                         v,
                         2,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::SliderFloat3(const char* label,
                      float v[3],
                      float v_min,
                      float v_max,
                      const char* format,
                      AzSliderFlags flags)
{
    return SliderScalarN(label,
                         AzDataType_Float,
                         v,
                         3,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::SliderFloat4(const char* label,
                      float v[4],
                      float v_min,
                      float v_max,
                      const char* format,
                      AzSliderFlags flags)
{
    return SliderScalarN(label,
                         AzDataType_Float,
                         v,
                         4,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::SliderAngle(const char* label,
                     float* v_rad,
                     float v_degrees_min,
                     float v_degrees_max,
                     const char* format,
                     AzSliderFlags flags)
{
    if (format == NULL)
        format = "%.0f deg";
    float v_deg = (*v_rad) * 360.0f / (2 * AZ_PI);
    bool value_changed = SliderFloat(label,
                                     &v_deg,
                                     v_degrees_min,
                                     v_degrees_max,
                                     format,
                                     flags);
    if (value_changed)
        *v_rad = v_deg * (2 * AZ_PI) / 360.0f;
    return value_changed;
}

bool Az::SliderInt(const char* label,
                   int* v,
                   int v_min,
                   int v_max,
                   const char* format,
                   AzSliderFlags flags)
{
    return SliderScalar(label,
                        AzDataType_S32,
                        v,
                        &v_min,
                        &v_max,
                        format,
                        flags);
}

bool Az::SliderInt2(const char* label,
                    int v[2],
                    int v_min,
                    int v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return SliderScalarN(label,
                         AzDataType_S32,
                         v,
                         2,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::SliderInt3(const char* label,
                    int v[3],
                    int v_min,
                    int v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return SliderScalarN(label,
                         AzDataType_S32,
                         v,
                         3,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::SliderInt4(const char* label,
                    int v[4],
                    int v_min,
                    int v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return SliderScalarN(label,
                         AzDataType_S32,
                         v,
                         4,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::VSliderScalar(const char* label,
                       const AzVec2& size,
                       AzDataType data_type,
                       void* p_data,
                       const void* p_min,
                       const void* p_max,
                       const char* format,
                       AzSliderFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);

    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    const AzRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const AzRect bb(frame_bb.Min,
                    frame_bb.Max
                            + AzVec2(label_size.x > 0.0f
                                             ? style.ItemInnerSpacing.x
                                                       + label_size.x
                                             : 0.0f,
                                     0.0f));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
    const bool clicked = hovered && IsMouseClicked(0, AzInputFlags_None, id);
    if (clicked || g.NavActivateId == id)
    {
        if (clicked)
            SetKeyOwner(AzKey_MouseLeft, id);
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << AzDir_Up) | (1 << AzDir_Down);
    }

    const AzU32 frame_col = GetColorU32(g.ActiveId == id ? AzCol_FrameBgActive
                                        : hovered        ? AzCol_FrameBgHovered
                                                         : AzCol_FrameBg);
    RenderNavCursor(frame_bb, id);
    RenderFrame(frame_bb.Min,
                frame_bb.Max,
                frame_col,
                true,
                g.Style.FrameRounding);

    AzRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb,
                                              id,
                                              data_type,
                                              p_data,
                                              p_min,
                                              p_max,
                                              format,
                                              flags | AzSliderFlags_Vertical,
                                              &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    if (grab_bb.Max.y > grab_bb.Min.y)
        window->DrawList->AddRectFilled(
                grab_bb.Min,
                grab_bb.Max,
                GetColorU32(g.ActiveId == id ? AzCol_SliderGrabActive
                                             : AzCol_SliderGrab),
                style.GrabRounding);

    char value_buf[64];
    const char* value_buf_end = value_buf
                                + DataTypeFormatString(value_buf,
                                                       AZ_ARRAYSIZE(value_buf),
                                                       data_type,
                                                       p_data,
                                                       format);
    RenderTextClipped(AzVec2(frame_bb.Min.x,
                             frame_bb.Min.y + style.FramePadding.y),
                      frame_bb.Max,
                      value_buf,
                      value_buf_end,
                      NULL,
                      AzVec2(0.5f, 0.0f));
    if (label_size.x > 0.0f)
        RenderText(AzVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                          frame_bb.Min.y + style.FramePadding.y),
                   label);

    return value_changed;
}

bool Az::VSliderFloat(const char* label,
                      const AzVec2& size,
                      float* v,
                      float v_min,
                      float v_max,
                      const char* format,
                      AzSliderFlags flags)
{
    return VSliderScalar(label,
                         size,
                         AzDataType_Float,
                         v,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

bool Az::VSliderInt(const char* label,
                    const AzVec2& size,
                    int* v,
                    int v_min,
                    int v_max,
                    const char* format,
                    AzSliderFlags flags)
{
    return VSliderScalar(label,
                         size,
                         AzDataType_S32,
                         v,
                         &v_min,
                         &v_max,
                         format,
                         flags);
}

const char* AzParseFormatFindStart(const char* fmt)
{
    while (char c = fmt[0])
    {
        if (c == '%' && fmt[1] != '%')
            return fmt;
        else if (c == '%')
            fmt++;
        fmt++;
    }
    return fmt;
}

const char* AzParseFormatFindEnd(const char* fmt)
{
    if (fmt[0] != '%')
        return fmt;
    const unsigned int ignored_uppercase_mask = (1 << ('I' - 'A'))
                                                | (1 << ('L' - 'A'));
    const unsigned int ignored_lowercase_mask = (1 << ('h' - 'a'))
                                                | (1 << ('j' - 'a'))
                                                | (1 << ('l' - 'a'))
                                                | (1 << ('t' - 'a'))
                                                | (1 << ('w' - 'a'))
                                                | (1 << ('z' - 'a'));
    for (char c; (c = *fmt) != 0; fmt++)
    {
        if (c >= 'A' && c <= 'Z'
            && ((1 << (c - 'A')) & ignored_uppercase_mask) == 0)
            return fmt + 1;
        if (c >= 'a' && c <= 'z'
            && ((1 << (c - 'a')) & ignored_lowercase_mask) == 0)
            return fmt + 1;
    }
    return fmt;
}

const char* AzParseFormatTrimDecorations(const char* fmt,
                                         char* buf,
                                         size_t buf_size)
{
    const char* fmt_start = AzParseFormatFindStart(fmt);
    if (fmt_start[0] != '%')
        return "";
    const char* fmt_end = AzParseFormatFindEnd(fmt_start);
    if (fmt_end[0] == 0)
        return fmt_start;
    AzStrncpy(buf,
              fmt_start,
              AzMin((size_t)(fmt_end - fmt_start) + 1, buf_size));
    return buf;
}

void AzParseFormatSanitizeForPrinting(const char* fmt_in,
                                      char* fmt_out,
                                      size_t fmt_out_size)
{
    const char* fmt_end = AzParseFormatFindEnd(fmt_in);
    AZ_UNUSED(fmt_out_size);
    AZ_ASSERT((size_t)(fmt_end - fmt_in + 1) < fmt_out_size);
    while (fmt_in < fmt_end)
    {
        char c = *fmt_in++;
        if (c != '\'' && c != '$' && c != '_')
            *(fmt_out++) = c;
    }
    *fmt_out = 0;
}

const char* AzParseFormatSanitizeForScanning(const char* fmt_in,
                                             char* fmt_out,
                                             size_t fmt_out_size)
{
    const char* fmt_end = AzParseFormatFindEnd(fmt_in);
    const char* fmt_out_begin = fmt_out;
    AZ_UNUSED(fmt_out_size);
    AZ_ASSERT((size_t)(fmt_end - fmt_in + 1) < fmt_out_size);
    bool has_type = false;
    while (fmt_in < fmt_end)
    {
        char c = *fmt_in++;
        if (!has_type
            && ((c >= '0' && c <= '9') || c == '.' || c == '+' || c == '#'))
            continue;
        has_type |= ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
        if (c != '\'' && c != '$' && c != '_')
            *(fmt_out++) = c;
    }
    *fmt_out = 0;
    return fmt_out_begin;
}

template <typename TYPE>
static const char* AzAtoi(const char* src, TYPE* output)
{
    int negative = 0;
    if (*src == '-')
    {
        negative = 1;
        src++;
    }
    if (*src == '+')
    {
        src++;
    }
    TYPE v = 0;
    while (*src >= '0' && *src <= '9')
        v = (v * 10) + (*src++ - '0');
    *output = negative ? -v : v;
    return src;
}

int AzParseFormatPrecision(const char* fmt, int default_precision)
{
    fmt = AzParseFormatFindStart(fmt);
    if (fmt[0] != '%')
        return default_precision;
    fmt++;
    while (*fmt >= '0' && *fmt <= '9')
        fmt++;
    int precision = INT_MAX;
    if (*fmt == '.')
    {
        fmt = AzAtoi<int>(fmt + 1, &precision);
        if (precision < 0 || precision > 99)
            precision = default_precision;
    }
    if (*fmt == 'e' || *fmt == 'E')
        precision = -1;
    if ((*fmt == 'g' || *fmt == 'G') && precision == INT_MAX)
        precision = -1;
    return (precision == INT_MAX) ? default_precision : precision;
}

bool Az::TempInputText(const AzRect& bb,
                       AzID id,
                       const char* label,
                       char* buf,
                       int buf_size,
                       AzInputTextFlags flags)
{
    AzContext& g = *GAz;
    const bool init = (g.TempInputId != id);
    if (init)
        ClearActiveID();

    g.CurrentWindow->DC.CursorPos = bb.Min;
    g.LastItemData.ItemFlags |= AzItemFlags_AllowDuplicateId;
    bool value_changed = InputTextEx(label,
                                     NULL,
                                     buf,
                                     buf_size,
                                     bb.GetSize(),
                                     flags | AzInputTextFlags_MergedItem);
    if (init)
    {
        AZ_ASSERT(g.ActiveId == id);
        g.TempInputId = g.ActiveId;
    }
    return value_changed;
}

bool Az::TempInputScalar(const AzRect& bb,
                         AzID id,
                         const char* label,
                         AzDataType data_type,
                         void* p_data,
                         const char* format,
                         const void* p_clamp_min,
                         const void* p_clamp_max)
{
    AzContext& g = *GAz;
    const AzDataTypeInfo* type_info = DataTypeGetInfo(data_type);
    char fmt_buf[32];
    char data_buf[32];
    format = AzParseFormatTrimDecorations(format,
                                          fmt_buf,
                                          AZ_ARRAYSIZE(fmt_buf));
    if (format[0] == 0)
        format = type_info->PrintFmt;
    DataTypeFormatString(data_buf,
                         AZ_ARRAYSIZE(data_buf),
                         data_type,
                         p_data,
                         format);
    AzStrTrimBlanks(data_buf);

    AzInputTextFlags flags = AzInputTextFlags_AutoSelectAll
                             | (AzInputTextFlags)
                                     AzInputTextFlags_LocalizeDecimalPoint;
    g.LastItemData.ItemFlags |= AzItemFlags_NoMarkEdited;
    bool value_changed = false;
    if (TempInputText(bb, id, label, data_buf, AZ_ARRAYSIZE(data_buf), flags))
    {
        size_t data_type_size = type_info->Size;
        AzDataTypeStorage data_backup;
        memcpy(&data_backup, p_data, data_type_size);

        DataTypeApplyFromText(data_buf, data_type, p_data, format, NULL);
        if (p_clamp_min || p_clamp_max)
        {
            if (p_clamp_min && p_clamp_max
                && DataTypeCompare(data_type, p_clamp_min, p_clamp_max) > 0)
                AzSwap(p_clamp_min, p_clamp_max);
            DataTypeClamp(data_type, p_data, p_clamp_min, p_clamp_max);
        }

        g.LastItemData.ItemFlags &= ~AzItemFlags_NoMarkEdited;
        value_changed = memcmp(&data_backup, p_data, data_type_size) != 0;
        if (value_changed)
            MarkItemEdited(id);
    }
    return value_changed;
}

void Az::SetNextItemRefVal(AzDataType data_type, void* p_data)
{
    AzContext& g = *GAz;
    g.NextItemData.HasFlags |= AzNextItemDataFlags_HasRefVal;
    memcpy(&g.NextItemData.RefVal, p_data, DataTypeGetInfo(data_type)->Size);
}

bool Az::InputScalar(const char* label,
                     AzDataType data_type,
                     void* p_data,
                     const void* p_step,
                     const void* p_step_fast,
                     const char* format,
                     AzInputTextFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    AzStyle& style = g.Style;
    AZ_ASSERT((flags & AzInputTextFlags_EnterReturnsTrue) == 0);

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    void* p_data_default = (g.NextItemData.HasFlags
                            & AzNextItemDataFlags_HasRefVal)
                                   ? &g.NextItemData.RefVal
                                   : &g.DataTypeZeroValue;

    char buf[64];
    if ((flags & AzInputTextFlags_DisplayEmptyRefVal)
        && DataTypeCompare(data_type, p_data, p_data_default) == 0)
        buf[0] = 0;
    else
        DataTypeFormatString(buf, AZ_ARRAYSIZE(buf), data_type, p_data, format);

    g.NextItemData.ItemFlags |= AzItemFlags_NoMarkEdited;
    flags |= AzInputTextFlags_AutoSelectAll
             | (AzInputTextFlags)AzInputTextFlags_LocalizeDecimalPoint;

    bool value_changed = false;
    if (p_step == NULL)
    {
        if (InputText(label, buf, AZ_ARRAYSIZE(buf), flags))
            value_changed = DataTypeApplyFromText(
                    buf,
                    data_type,
                    p_data,
                    format,
                    (flags & AzInputTextFlags_ParseEmptyRefVal) ? p_data_default
                                                                : NULL);
    } else
    {
        const float button_size = GetFrameHeight();

        BeginGroup();
        PushID(label);
        SetNextItemWidth(
                AzMax(1.0f,
                      CalcItemWidth()
                              - (button_size + style.ItemInnerSpacing.x) * 2));
        if (InputText("", buf, AZ_ARRAYSIZE(buf), flags))
            value_changed = DataTypeApplyFromText(
                    buf,
                    data_type,
                    p_data,
                    format,
                    (flags & AzInputTextFlags_ParseEmptyRefVal) ? p_data_default
                                                                : NULL);
        AZ_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID,
                                 label,
                                 g.LastItemData.StatusFlags
                                         | AzItemStatusFlags_Inputable);

        const AzVec2 backup_frame_padding = style.FramePadding;
        style.FramePadding.x = style.FramePadding.y;
        if (flags & AzInputTextFlags_ReadOnly)
            BeginDisabled();
        PushItemFlag(AzItemFlags_ButtonRepeat, true);
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("-", AzVec2(button_size, button_size)))
        {
            DataTypeApplyOp(data_type,
                            '-',
                            p_data,
                            p_data,
                            g.IO.KeyCtrl && p_step_fast ? p_step_fast : p_step);
            value_changed = true;
        }
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("+", AzVec2(button_size, button_size)))
        {
            DataTypeApplyOp(data_type,
                            '+',
                            p_data,
                            p_data,
                            g.IO.KeyCtrl && p_step_fast ? p_step_fast : p_step);
            value_changed = true;
        }
        PopItemFlag();
        if (flags & AzInputTextFlags_ReadOnly)
            EndDisabled();

        const char* label_end = FindRenderedTextEnd(label);
        if (label != label_end)
        {
            SameLine(0, style.ItemInnerSpacing.x);
            TextEx(label, label_end);
        }
        style.FramePadding = backup_frame_padding;

        PopID();
        EndGroup();
    }

    g.LastItemData.ItemFlags &= ~AzItemFlags_NoMarkEdited;
    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

bool Az::InputScalarN(const char* label,
                      AzDataType data_type,
                      void* p_data,
                      int components,
                      const void* p_step,
                      const void* p_step_fast,
                      const char* format,
                      AzInputTextFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = GDataTypeInfo[data_type].Size;
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= InputScalar("",
                                     data_type,
                                     p_data,
                                     p_step,
                                     p_step_fast,
                                     format,
                                     flags);
        PopID();
        PopItemWidth();
        p_data = (void*)((char*)p_data + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0.0f, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool Az::InputFloat(const char* label,
                    float* v,
                    float step,
                    float step_fast,
                    const char* format,
                    AzInputTextFlags flags)
{
    return InputScalar(label,
                       AzDataType_Float,
                       (void*)v,
                       (void*)(step > 0.0f ? &step : NULL),
                       (void*)(step_fast > 0.0f ? &step_fast : NULL),
                       format,
                       flags);
}

bool Az::InputFloat2(const char* label,
                     float v[2],
                     const char* format,
                     AzInputTextFlags flags)
{
    return InputScalarN(label,
                        AzDataType_Float,
                        v,
                        2,
                        NULL,
                        NULL,
                        format,
                        flags);
}

bool Az::InputFloat3(const char* label,
                     float v[3],
                     const char* format,
                     AzInputTextFlags flags)
{
    return InputScalarN(label,
                        AzDataType_Float,
                        v,
                        3,
                        NULL,
                        NULL,
                        format,
                        flags);
}

bool Az::InputFloat4(const char* label,
                     float v[4],
                     const char* format,
                     AzInputTextFlags flags)
{
    return InputScalarN(label,
                        AzDataType_Float,
                        v,
                        4,
                        NULL,
                        NULL,
                        format,
                        flags);
}

bool Az::InputInt(const char* label,
                  int* v,
                  int step,
                  int step_fast,
                  AzInputTextFlags flags)
{
    const char* format = (flags & AzInputTextFlags_CharsHexadecimal) ? "%08X"
                                                                     : "%d";
    return InputScalar(label,
                       AzDataType_S32,
                       (void*)v,
                       (void*)(step > 0 ? &step : NULL),
                       (void*)(step_fast > 0 ? &step_fast : NULL),
                       format,
                       flags);
}

bool Az::InputInt2(const char* label, int v[2], AzInputTextFlags flags)
{
    return InputScalarN(label, AzDataType_S32, v, 2, NULL, NULL, "%d", flags);
}

bool Az::InputInt3(const char* label, int v[3], AzInputTextFlags flags)
{
    return InputScalarN(label, AzDataType_S32, v, 3, NULL, NULL, "%d", flags);
}

bool Az::InputInt4(const char* label, int v[4], AzInputTextFlags flags)
{
    return InputScalarN(label, AzDataType_S32, v, 4, NULL, NULL, "%d", flags);
}

bool Az::InputDouble(const char* label,
                     double* v,
                     double step,
                     double step_fast,
                     const char* format,
                     AzInputTextFlags flags)
{
    return InputScalar(label,
                       AzDataType_Double,
                       (void*)v,
                       (void*)(step > 0.0 ? &step : NULL),
                       (void*)(step_fast > 0.0 ? &step_fast : NULL),
                       format,
                       flags);
}

namespace AzStb
{
#include <Az/azstb_textedit.hpp>
}

bool Az::InputText(const char* label,
                   char* buf,
                   size_t buf_size,
                   AzInputTextFlags flags,
                   AzInputTextCallback callback,
                   void* user_data)
{
    AZ_ASSERT(!(flags & AzInputTextFlags_Multiline));
    return InputTextEx(label,
                       NULL,
                       buf,
                       (int)buf_size,
                       AzVec2(0, 0),
                       flags,
                       callback,
                       user_data);
}

bool Az::InputTextMultiline(const char* label,
                            char* buf,
                            size_t buf_size,
                            const AzVec2& size,
                            AzInputTextFlags flags,
                            AzInputTextCallback callback,
                            void* user_data)
{
    return InputTextEx(label,
                       NULL,
                       buf,
                       (int)buf_size,
                       size,
                       flags | AzInputTextFlags_Multiline,
                       callback,
                       user_data);
}

bool Az::InputTextWithHint(const char* label,
                           const char* hint,
                           char* buf,
                           size_t buf_size,
                           AzInputTextFlags flags,
                           AzInputTextCallback callback,
                           void* user_data)
{
    AZ_ASSERT(!(flags & AzInputTextFlags_Multiline));
    return InputTextEx(label,
                       hint,
                       buf,
                       (int)buf_size,
                       AzVec2(0, 0),
                       flags,
                       callback,
                       user_data);
}

static int InputTextCalcTextLenAndLineCount(const char* text_begin,
                                            const char** out_text_end)
{
    int line_count = 0;
    const char* s = text_begin;
    while (true)
    {
        const char* s_eol = strchr(s, '\n');
        line_count++;
        if (s_eol == NULL)
        {
            s = s + AzStrlen(s);
            break;
        }
        s = s_eol + 1;
    }
    *out_text_end = s;
    return line_count;
}

static AzVec2 InputTextCalcTextSize(AzContext* ctx,
                                    const char* text_begin,
                                    const char* text_end,
                                    const char** remaining,
                                    AzVec2* out_offset,
                                    bool stop_on_new_line)
{
    AzContext& g = *ctx;

    AzFontBaked* baked = g.FontBaked;
    const float line_height = g.FontSize;
    const float scale = line_height / baked->Size;

    AzVec2 text_size = AzVec2(0, 0);
    float line_width = 0.0f;

    const char* s = text_begin;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
            s += 1;
        else
            s += AzTextCharFromUtf8(&c, s, text_end);

        if (c == '\n')
        {
            text_size.x = AzMax(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0.0f;
            if (stop_on_new_line)
                break;
            continue;
        }
        if (c == '\r')
            continue;

        line_width += baked->GetCharAdvance((AzWchar)c) * scale;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (out_offset)
        *out_offset = AzVec2(line_width, text_size.y + line_height);

    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

namespace AzStb
{
    static int STB_TEXTEDIT_STRINGLEN(const AzInputTextState* obj)
    {
        return obj->TextLen;
    }
    static char STB_TEXTEDIT_GETCHAR(const AzInputTextState* obj, int idx)
    {
        AZ_ASSERT(idx <= obj->TextLen);
        return obj->TextSrc[idx];
    }
    static float STB_TEXTEDIT_GETWIDTH(AzInputTextState* obj,
                                       int line_start_idx,
                                       int char_idx)
    {
        unsigned int c;
        AzTextCharFromUtf8(&c,
                           obj->TextSrc + line_start_idx + char_idx,
                           obj->TextSrc + obj->TextLen);
        if ((AzWchar)c == '\n')
            return AZSTB_TEXTEDIT_GETWIDTH_NEWLINE;
        AzContext& g = *obj->Ctx;
        return g.FontBaked->GetCharAdvance((AzWchar)c) * g.FontBakedScale;
    }
    static char STB_TEXTEDIT_NEWLINE = '\n';
    static void STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r,
                                       AzInputTextState* obj,
                                       int line_start_idx)
    {
        const char* text = obj->TextSrc;
        const char* text_remaining = NULL;
        const AzVec2 size = InputTextCalcTextSize(obj->Ctx,
                                                  text + line_start_idx,
                                                  text + obj->TextLen,
                                                  &text_remaining,
                                                  NULL,
                                                  true);
        r->x0 = 0.0f;
        r->x1 = size.x;
        r->baseline_y_delta = size.y;
        r->ymin = 0.0f;
        r->ymax = size.y;
        r->num_chars = (int)(text_remaining - (text + line_start_idx));
    }

#define AZSTB_TEXTEDIT_GETNEXTCHARINDEX AZSTB_TEXTEDIT_GETNEXTCHARINDEX_AZPL
#define AZSTB_TEXTEDIT_GETPREVCHARINDEX AZSTB_TEXTEDIT_GETPREVCHARINDEX_AZPL

    static int AZSTB_TEXTEDIT_GETNEXTCHARINDEX_AZPL(AzInputTextState* obj,
                                                    int idx)
    {
        if (idx >= obj->TextLen)
            return obj->TextLen + 1;
        unsigned int c;
        return idx
               + AzTextCharFromUtf8(&c,
                                    obj->TextSrc + idx,
                                    obj->TextSrc + obj->TextLen);
    }

    static int AZSTB_TEXTEDIT_GETPREVCHARINDEX_AZPL(AzInputTextState* obj,
                                                    int idx)
    {
        if (idx <= 0)
            return -1;
        const char* p = AzTextFindPreviousUtf8Codepoint(obj->TextSrc,
                                                        obj->TextSrc + idx);
        return (int)(p - obj->TextSrc);
    }

    static bool AzCharIsSeparatorW(unsigned int c)
    {
        static const unsigned int separator_list[] = {
                ',', 0x3001, '.',    0x3002, ';',  0xFF1B, '(',  0xFF08,
                ')', 0xFF09, '{',    0xFF5B, '}',  0xFF5D, '[',  0x300C,
                ']', 0x300D, '|',    0xFF5C, '!',  0xFF01, '\\', 0xFFE5,
                '/', 0x30FB, 0xFF0F, '\n',   '\r',
        };
        for (unsigned int separator : separator_list)
            if (c == separator)
                return true;
        return false;
    }

    static int is_word_boundary_from_right(AzInputTextState* obj, int idx)
    {
        if ((obj->Flags & AzInputTextFlags_Password) || idx <= 0)
            return 0;

        const char* curr_p = obj->TextSrc + idx;
        const char* prev_p = AzTextFindPreviousUtf8Codepoint(obj->TextSrc,
                                                             curr_p);
        unsigned int curr_c;
        AzTextCharFromUtf8(&curr_c, curr_p, obj->TextSrc + obj->TextLen);
        unsigned int prev_c;
        AzTextCharFromUtf8(&prev_c, prev_p, obj->TextSrc + obj->TextLen);

        bool prev_white = AzCharIsBlankW(prev_c);
        bool prev_separ = AzCharIsSeparatorW(prev_c);
        bool curr_white = AzCharIsBlankW(curr_c);
        bool curr_separ = AzCharIsSeparatorW(curr_c);
        return ((prev_white || prev_separ) && !(curr_separ || curr_white))
               || (curr_separ && !prev_separ);
    }
    static int is_word_boundary_from_left(AzInputTextState* obj, int idx)
    {
        if ((obj->Flags & AzInputTextFlags_Password) || idx <= 0)
            return 0;

        const char* curr_p = obj->TextSrc + idx;
        const char* prev_p = AzTextFindPreviousUtf8Codepoint(obj->TextSrc,
                                                             curr_p);
        unsigned int prev_c;
        AzTextCharFromUtf8(&prev_c, curr_p, obj->TextSrc + obj->TextLen);
        unsigned int curr_c;
        AzTextCharFromUtf8(&curr_c, prev_p, obj->TextSrc + obj->TextLen);

        bool prev_white = AzCharIsBlankW(prev_c);
        bool prev_separ = AzCharIsSeparatorW(prev_c);
        bool curr_white = AzCharIsBlankW(curr_c);
        bool curr_separ = AzCharIsSeparatorW(curr_c);
        return ((prev_white) && !(curr_separ || curr_white))
               || (curr_separ && !prev_separ);
    }
    static int STB_TEXTEDIT_MOVEWORDLEFT_AZPL(AzInputTextState* obj, int idx)
    {
        idx = AZSTB_TEXTEDIT_GETPREVCHARINDEX(obj, idx);
        while (idx >= 0 && !is_word_boundary_from_right(obj, idx))
            idx = AZSTB_TEXTEDIT_GETPREVCHARINDEX(obj, idx);
        return idx < 0 ? 0 : idx;
    }
    static int STB_TEXTEDIT_MOVEWORDRIGHT_MAC(AzInputTextState* obj, int idx)
    {
        int len = obj->TextLen;
        idx = AZSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
        while (idx < len && !is_word_boundary_from_left(obj, idx))
            idx = AZSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
        return idx > len ? len : idx;
    }
    static int STB_TEXTEDIT_MOVEWORDRIGHT_WIN(AzInputTextState* obj, int idx)
    {
        idx = AZSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
        int len = obj->TextLen;
        while (idx < len && !is_word_boundary_from_right(obj, idx))
            idx = AZSTB_TEXTEDIT_GETNEXTCHARINDEX(obj, idx);
        return idx > len ? len : idx;
    }
    static int STB_TEXTEDIT_MOVEWORDRIGHT_AZPL(AzInputTextState* obj, int idx)
    {
        AzContext& g = *obj->Ctx;
        if (g.IO.ConfigMacOSXBehaviors)
            return STB_TEXTEDIT_MOVEWORDRIGHT_MAC(obj, idx);
        else
            return STB_TEXTEDIT_MOVEWORDRIGHT_WIN(obj, idx);
    }
#define STB_TEXTEDIT_MOVEWORDLEFT STB_TEXTEDIT_MOVEWORDLEFT_AZPL
#define STB_TEXTEDIT_MOVEWORDRIGHT STB_TEXTEDIT_MOVEWORDRIGHT_AZPL

    static void STB_TEXTEDIT_DELETECHARS(AzInputTextState* obj, int pos, int n)
    {
        AZ_ASSERT(obj->TextSrc == obj->TextA.Data);
        char* dst = obj->TextA.Data + pos;
        char* src = obj->TextA.Data + pos + n;
        memmove(dst, src, obj->TextLen - n - pos + 1);
        obj->Edited = true;
        obj->TextLen -= n;
    }

    static bool STB_TEXTEDIT_INSERTCHARS(AzInputTextState* obj,
                                         int pos,
                                         const char* new_text,
                                         int new_text_len)
    {
        const bool is_resizable = (obj->Flags & AzInputTextFlags_CallbackResize)
                                  != 0;
        const int text_len = obj->TextLen;
        AZ_ASSERT(pos <= text_len);

        if (!is_resizable
            && (new_text_len + obj->TextLen + 1 > obj->BufCapacity))
            return false;

        AZ_ASSERT(obj->TextSrc == obj->TextA.Data);
        if (new_text_len + text_len + 1 > obj->TextA.Size)
        {
            if (!is_resizable)
                return false;
            obj->TextA.resize(
                    text_len
                    + AzClamp(new_text_len, 32, AzMax(256, new_text_len)) + 1);
            obj->TextSrc = obj->TextA.Data;
        }

        char* text = obj->TextA.Data;
        if (pos != text_len)
            memmove(text + pos + new_text_len,
                    text + pos,
                    (size_t)(text_len - pos));
        memcpy(text + pos, new_text, (size_t)new_text_len);

        obj->Edited = true;
        obj->TextLen += new_text_len;
        obj->TextA[obj->TextLen] = '\0';

        return true;
    }

#define STB_TEXTEDIT_K_LEFT 0x200000
#define STB_TEXTEDIT_K_RIGHT 0x200001
#define STB_TEXTEDIT_K_UP 0x200002
#define STB_TEXTEDIT_K_DOWN 0x200003
#define STB_TEXTEDIT_K_LINESTART 0x200004
#define STB_TEXTEDIT_K_LINEEND 0x200005
#define STB_TEXTEDIT_K_TEXTSTART 0x200006
#define STB_TEXTEDIT_K_TEXTEND 0x200007
#define STB_TEXTEDIT_K_DELETE 0x200008
#define STB_TEXTEDIT_K_BACKSPACE 0x200009
#define STB_TEXTEDIT_K_UNDO 0x20000A
#define STB_TEXTEDIT_K_REDO 0x20000B
#define STB_TEXTEDIT_K_WORDLEFT 0x20000C
#define STB_TEXTEDIT_K_WORDRIGHT 0x20000D
#define STB_TEXTEDIT_K_PGUP 0x20000E
#define STB_TEXTEDIT_K_PGDOWN 0x20000F
#define STB_TEXTEDIT_K_SHIFT 0x400000

#define AZSTB_TEXTEDIT_IMPLEMENTATION
#define AZSTB_TEXTEDIT_memmove memmove
#include <Az/azstb_textedit.hpp>

    static void stb_textedit_replace(AzInputTextState* str,
                                     STB_TexteditState* state,
                                     const AZSTB_TEXTEDIT_CHARTYPE* text,
                                     int text_len)
    {
        stb_text_makeundo_replace(str, state, 0, str->TextLen, text_len);
        AzStb::STB_TEXTEDIT_DELETECHARS(str, 0, str->TextLen);
        state->cursor = state->select_start = state->select_end = 0;
        if (text_len <= 0)
            return;
        if (AzStb::STB_TEXTEDIT_INSERTCHARS(str, 0, text, text_len))
        {
            state->cursor = state->select_start = state->select_end = text_len;
            state->has_preferred_x = 0;
            return;
        }
        AZ_ASSERT(0);
    }

} // namespace AzStb

AzInputTextState::AzInputTextState()
{
    memset(this, 0, sizeof(*this));
    Stb = AZ_NEW(AzStbTexteditState);
    memset(Stb, 0, sizeof(*Stb));
}

AzInputTextState::~AzInputTextState()
{
    AZ_DELETE(Stb);
}

void AzInputTextState::OnKeyPressed(int key)
{
    stb_textedit_key(this, Stb, key);
    CursorFollow = true;
    CursorAnimReset();
}

void AzInputTextState::OnCharPressed(unsigned int c)
{
    char utf8[5];
    AzTextCharToUtf8(utf8, c);
    stb_textedit_text(this, Stb, utf8, (int)AzStrlen(utf8));
    CursorFollow = true;
    CursorAnimReset();
}

void AzInputTextState::CursorAnimReset()
{
    CursorAnim = -0.30f;
}
void AzInputTextState::CursorClamp()
{
    Stb->cursor = AzMin(Stb->cursor, TextLen);
    Stb->select_start = AzMin(Stb->select_start, TextLen);
    Stb->select_end = AzMin(Stb->select_end, TextLen);
}
bool AzInputTextState::HasSelection() const
{
    return Stb->select_start != Stb->select_end;
}
void AzInputTextState::ClearSelection()
{
    Stb->select_start = Stb->select_end = Stb->cursor;
}
int AzInputTextState::GetCursorPos() const
{
    return Stb->cursor;
}
int AzInputTextState::GetSelectionStart() const
{
    return Stb->select_start;
}
int AzInputTextState::GetSelectionEnd() const
{
    return Stb->select_end;
}
void AzInputTextState::SelectAll()
{
    Stb->select_start = 0;
    Stb->cursor = Stb->select_end = TextLen;
    Stb->has_preferred_x = 0;
}
void AzInputTextState::ReloadUserBufAndSelectAll()
{
    WantReloadUserBuf = true;
    ReloadSelectionStart = 0;
    ReloadSelectionEnd = INT_MAX;
}
void AzInputTextState::ReloadUserBufAndKeepSelection()
{
    WantReloadUserBuf = true;
    ReloadSelectionStart = Stb->select_start;
    ReloadSelectionEnd = Stb->select_end;
}
void AzInputTextState::ReloadUserBufAndMoveToEnd()
{
    WantReloadUserBuf = true;
    ReloadSelectionStart = ReloadSelectionEnd = INT_MAX;
}

AzInputTextCallbackData::AzInputTextCallbackData()
{
    memset(this, 0, sizeof(*this));
}

void AzInputTextCallbackData::DeleteChars(int pos, int bytes_count)
{
    AZ_ASSERT(pos + bytes_count <= BufTextLen);
    char* dst = Buf + pos;
    const char* src = Buf + pos + bytes_count;
    memmove(dst, src, BufTextLen - bytes_count - pos + 1);

    if (CursorPos >= pos + bytes_count)
        CursorPos -= bytes_count;
    else if (CursorPos >= pos)
        CursorPos = pos;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty = true;
    BufTextLen -= bytes_count;
}

void AzInputTextCallbackData::InsertChars(int pos,
                                          const char* new_text,
                                          const char* new_text_end)
{
    if (new_text == new_text_end)
        return;

    const bool is_resizable = (Flags & AzInputTextFlags_CallbackResize) != 0;
    const int new_text_len = new_text_end ? (int)(new_text_end - new_text)
                                          : (int)AzStrlen(new_text);
    if (new_text_len + BufTextLen >= BufSize)
    {
        if (!is_resizable)
            return;

        AzContext& g = *Ctx;
        AzInputTextState* edit_state = &g.InputTextState;
        AZ_ASSERT(edit_state->ID != 0 && g.ActiveId == edit_state->ID);
        AZ_ASSERT(Buf == edit_state->TextA.Data);
        int new_buf_size = BufTextLen
                           + AzClamp(new_text_len * 4,
                                     32,
                                     AzMax(256, new_text_len))
                           + 1;
        edit_state->TextA.resize(new_buf_size + 1);
        edit_state->TextSrc = edit_state->TextA.Data;
        Buf = edit_state->TextA.Data;
        BufSize = edit_state->BufCapacity = new_buf_size;
    }

    if (BufTextLen != pos)
        memmove(Buf + pos + new_text_len,
                Buf + pos,
                (size_t)(BufTextLen - pos));
    memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
    Buf[BufTextLen + new_text_len] = '\0';

    if (CursorPos >= pos)
        CursorPos += new_text_len;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty = true;
    BufTextLen += new_text_len;
}

void Az::PushPasswordFont()
{
    AzContext& g = *GAz;
    AzFontBaked* backup = &g.InputTextPasswordFontBackupBaked;
    AZ_ASSERT(backup->IndexAdvanceX.Size == 0 && backup->IndexLookup.Size == 0);
    AzFontGlyph* glyph = g.FontBaked->FindGlyph('*');
    g.InputTextPasswordFontBackupFlags = g.Font->Flags;
    backup->FallbackGlyphIndex = g.FontBaked->FallbackGlyphIndex;
    backup->FallbackAdvanceX = g.FontBaked->FallbackAdvanceX;
    backup->IndexLookup.swap(g.FontBaked->IndexLookup);
    backup->IndexAdvanceX.swap(g.FontBaked->IndexAdvanceX);
    g.Font->Flags |= AzFontFlags_NoLoadGlyphs;
    g.FontBaked->FallbackGlyphIndex = g.FontBaked->Glyphs.index_from_ptr(glyph);
    g.FontBaked->FallbackAdvanceX = glyph->AdvanceX;
}

void Az::PopPasswordFont()
{
    AzContext& g = *GAz;
    AzFontBaked* backup = &g.InputTextPasswordFontBackupBaked;
    g.Font->Flags = g.InputTextPasswordFontBackupFlags;
    g.FontBaked->FallbackGlyphIndex = backup->FallbackGlyphIndex;
    g.FontBaked->FallbackAdvanceX = backup->FallbackAdvanceX;
    g.FontBaked->IndexLookup.swap(backup->IndexLookup);
    g.FontBaked->IndexAdvanceX.swap(backup->IndexAdvanceX);
    AZ_ASSERT(backup->IndexAdvanceX.Size == 0 && backup->IndexLookup.Size == 0);
}

static bool InputTextFilterCharacter(AzContext* ctx,
                                     unsigned int* p_char,
                                     AzInputTextFlags flags,
                                     AzInputTextCallback callback,
                                     void* user_data,
                                     bool input_source_is_clipboard)
{
    unsigned int c = *p_char;

    bool apply_named_filters = true;
    if (c < 0x20)
    {
        bool pass = false;
        pass |= (c == '\n') && (flags & AzInputTextFlags_Multiline) != 0;
        if (c == '\n' && input_source_is_clipboard
            && (flags & AzInputTextFlags_Multiline) == 0)
        {
            c = *p_char = ' ';
            pass = true;
        }
        pass |= (c == '\n') && (flags & AzInputTextFlags_Multiline) != 0;
        pass |= (c == '\t') && (flags & AzInputTextFlags_AllowTabInput) != 0;
        if (!pass)
            return false;
        apply_named_filters = false;
    }

    if (input_source_is_clipboard == false)
    {
        if (c == 127)
            return false;

        if (c >= 0xE000 && c <= 0xF8FF)
            return false;
    }

    if (c > AZ_UNICODE_CODEPOINT_MAX)
        return false;

    if (apply_named_filters
        && (flags
            & (AzInputTextFlags_CharsDecimal | AzInputTextFlags_CharsHexadecimal
               | AzInputTextFlags_CharsUppercase | AzInputTextFlags_CharsNoBlank
               | AzInputTextFlags_CharsScientific
               | (AzInputTextFlags)AzInputTextFlags_LocalizeDecimalPoint)))
    {
        AzContext& g = *ctx;
        const unsigned c_decimal_point =
                (unsigned int)g.PlatformIO.Platform_LocaleDecimalPoint;
        if (flags
            & (AzInputTextFlags_CharsDecimal | AzInputTextFlags_CharsScientific
               | (AzInputTextFlags)AzInputTextFlags_LocalizeDecimalPoint))
            if (c == '.' || c == ',')
                c = c_decimal_point;

        if (flags
            & (AzInputTextFlags_CharsDecimal | AzInputTextFlags_CharsScientific
               | AzInputTextFlags_CharsHexadecimal))
            if (c >= 0xFF01 && c <= 0xFF5E)
                c = c - 0xFF01 + 0x21;

        if (flags & AzInputTextFlags_CharsDecimal)
            if (!(c >= '0' && c <= '9') && (c != c_decimal_point) && (c != '-')
                && (c != '+') && (c != '*') && (c != '/'))
                return false;

        if (flags & AzInputTextFlags_CharsScientific)
            if (!(c >= '0' && c <= '9') && (c != c_decimal_point) && (c != '-')
                && (c != '+') && (c != '*') && (c != '/') && (c != 'e')
                && (c != 'E'))
                return false;

        if (flags & AzInputTextFlags_CharsHexadecimal)
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f')
                && !(c >= 'A' && c <= 'F'))
                return false;

        if (flags & AzInputTextFlags_CharsUppercase)
            if (c >= 'a' && c <= 'z')
                c += (unsigned int)('A' - 'a');

        if (flags & AzInputTextFlags_CharsNoBlank)
            if (AzCharIsBlankW(c))
                return false;

        *p_char = c;
    }

    if (flags & AzInputTextFlags_CallbackCharFilter)
    {
        AzContext& g = *GAz;
        AzInputTextCallbackData callback_data;
        callback_data.Ctx = &g;
        callback_data.EventFlag = AzInputTextFlags_CallbackCharFilter;
        callback_data.EventChar = (AzWchar)c;
        callback_data.Flags = flags;
        callback_data.UserData = user_data;
        if (callback(&callback_data) != 0)
            return false;
        *p_char = callback_data.EventChar;
        if (!callback_data.EventChar)
            return false;
    }

    return true;
}

static void InputTextReconcileUndoState(AzInputTextState* state,
                                        const char* old_buf,
                                        int old_length,
                                        const char* new_buf,
                                        int new_length)
{
    const int shorter_length = AzMin(old_length, new_length);
    int first_diff;
    for (first_diff = 0; first_diff < shorter_length; first_diff++)
        if (old_buf[first_diff] != new_buf[first_diff])
            break;
    if (first_diff == old_length && first_diff == new_length)
        return;

    int old_last_diff = old_length - 1;
    int new_last_diff = new_length - 1;
    for (; old_last_diff >= first_diff && new_last_diff >= first_diff;
         old_last_diff--, new_last_diff--)
        if (old_buf[old_last_diff] != new_buf[new_last_diff])
            break;

    const int insert_len = new_last_diff - first_diff + 1;
    const int delete_len = old_last_diff - first_diff + 1;
    if (insert_len > 0 || delete_len > 0)
        if (AZSTB_TEXTEDIT_CHARTYPE* p =
                    stb_text_createundo(&state->Stb->undostate,
                                        first_diff,
                                        delete_len,
                                        insert_len))
            for (int i = 0; i < delete_len; i++)
                p[i] = old_buf[first_diff + i];
}

void Az::InputTextDeactivateHook(AzID id)
{
    AzContext& g = *GAz;
    AzInputTextState* state = &g.InputTextState;
    if (id == 0 || state->ID != id)
        return;
    g.InputTextDeactivatedState.ID = state->ID;
    if (state->Flags & AzInputTextFlags_ReadOnly)
    {
        g.InputTextDeactivatedState.TextA.resize(0);
    } else
    {
        AZ_ASSERT(state->TextA.Data != 0);
        AZ_ASSERT(state->TextA[state->TextLen] == 0);
        g.InputTextDeactivatedState.TextA.resize(state->TextLen + 1);
        memcpy(g.InputTextDeactivatedState.TextA.Data,
               state->TextA.Data,
               state->TextLen + 1);
    }
}

bool Az::InputTextEx(const char* label,
                     const char* hint,
                     char* buf,
                     int buf_size,
                     const AzVec2& size_arg,
                     AzInputTextFlags flags,
                     AzInputTextCallback callback,
                     void* callback_user_data)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AZ_ASSERT(buf != NULL && buf_size >= 0);
    AZ_ASSERT(!((flags & AzInputTextFlags_CallbackHistory)
                && (flags & AzInputTextFlags_Multiline)));
    AZ_ASSERT(!((flags & AzInputTextFlags_CallbackCompletion)
                && (flags & AzInputTextFlags_AllowTabInput)));
    AZ_ASSERT(!((flags & AzInputTextFlags_ElideLeft)
                && (flags & AzInputTextFlags_Multiline)));

    AzContext& g = *GAz;
    AzIO& io = g.IO;
    const AzStyle& style = g.Style;

    const bool RENDER_SELECTION_WHEN_INACTIVE = false;
    const bool is_multiline = (flags & AzInputTextFlags_Multiline) != 0;

    if (is_multiline)
        BeginGroup();
    const AzID id = window->GetID(label);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    const AzVec2 frame_size =
            CalcItemSize(size_arg,
                         CalcItemWidth(),
                         (is_multiline ? g.FontSize * 8.0f : label_size.y)
                                 + style.FramePadding.y * 2.0f);
    const AzVec2 total_size =
            AzVec2(frame_size.x
                           + (label_size.x > 0.0f
                                      ? style.ItemInnerSpacing.x + label_size.x
                                      : 0.0f),
                   frame_size.y);

    const AzRect frame_bb(window->DC.CursorPos,
                          window->DC.CursorPos + frame_size);
    const AzRect total_bb(frame_bb.Min, frame_bb.Min + total_size);

    AzWindow* draw_window = window;
    AzVec2 inner_size = frame_size;
    AzLastItemData item_data_backup;
    if (is_multiline)
    {
        AzVec2 backup_pos = window->DC.CursorPos;
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb, AzItemFlags_Inputable))
        {
            EndGroup();
            return false;
        }
        item_data_backup = g.LastItemData;
        window->DC.CursorPos = backup_pos;

        if (g.NavActivateId == id
            && (g.NavActivateFlags & AzActivateFlags_FromTabbing)
            && (flags & AzInputTextFlags_AllowTabInput))
            g.NavActivateId = 0;

        const AzID backup_activate_id = g.NavActivateId;
        if (g.ActiveId == id)
            g.NavActivateId = 0;

        PushStyleColor(AzCol_ChildBg, style.Colors[AzCol_FrameBg]);
        PushStyleVar(AzStyleVar_ChildRounding, style.FrameRounding);
        PushStyleVar(AzStyleVar_ChildBorderSize, style.FrameBorderSize);
        PushStyleVar(AzStyleVar_WindowPadding, AzVec2(0, 0));
        bool child_visible = BeginChildEx(label,
                                          id,
                                          frame_bb.GetSize(),
                                          AzChildFlags_Borders,
                                          AzWindowFlags_NoMove);
        g.NavActivateId = backup_activate_id;
        PopStyleVar(3);
        PopStyleColor();
        if (!child_visible)
        {
            EndChild();
            EndGroup();
            return false;
        }
        draw_window = g.CurrentWindow;
        draw_window->DC.NavLayersActiveMaskNext |=
                (1 << draw_window->DC.NavLayerCurrent);
        draw_window->DC.CursorPos += style.FramePadding;
        inner_size.x -= draw_window->ScrollbarSizes.x;
    } else
    {
        ItemSize(total_bb, style.FramePadding.y);
        if (!(flags & AzInputTextFlags_MergedItem))
            if (!ItemAdd(total_bb, id, &frame_bb, AzItemFlags_Inputable))
                return false;
    }

    bool hovered = ItemHoverable(frame_bb,
                                 id,
                                 g.LastItemData.ItemFlags
                                         | AzItemFlags_NoNavDisableMouseHover);
    if (hovered)
        SetMouseCursor(AzMouseCursor_TextInput);
    if (hovered && g.NavHighlightItemUnderNav)
        hovered = false;

    AzInputTextState* state = GetInputTextState(id);

    if (g.LastItemData.ItemFlags & AzItemFlags_ReadOnly)
        flags |= AzInputTextFlags_ReadOnly;
    const bool is_readonly = (flags & AzInputTextFlags_ReadOnly) != 0;
    const bool is_password = (flags & AzInputTextFlags_Password) != 0;
    const bool is_undoable = (flags & AzInputTextFlags_NoUndoRedo) == 0;
    const bool is_resizable = (flags & AzInputTextFlags_CallbackResize) != 0;
    if (is_resizable)
        AZ_ASSERT(callback != NULL);

    const bool input_requested_by_nav =
            (g.ActiveId != id)
            && ((g.NavActivateId == id)
                && ((g.NavActivateFlags & AzActivateFlags_PreferInput)
                    || (g.NavInputSource == AzInputSource_Keyboard)));

    const bool user_clicked = hovered && io.MouseClicked[0];
    const bool user_scroll_finish =
            is_multiline && state != NULL && g.ActiveId == 0
            && g.ActiveIdPreviousFrame
                       == GetWindowScrollbarID(draw_window, AzAxis_Y);
    const bool user_scroll_active =
            is_multiline && state != NULL
            && g.ActiveId == GetWindowScrollbarID(draw_window, AzAxis_Y);
    bool clear_active_id = false;
    bool select_all = false;

    float scroll_y = is_multiline ? draw_window->Scroll.y : FLT_MAX;

    const bool init_reload_from_user_buf = (state != NULL
                                            && state->WantReloadUserBuf);
    const bool init_changed_specs = (state != NULL
                                     && state->Stb->single_line
                                                != !is_multiline);
    const bool init_make_active = (user_clicked || user_scroll_finish
                                   || input_requested_by_nav);
    const bool init_state = (init_make_active || user_scroll_active);
    if (init_reload_from_user_buf)
    {
        int new_len = (int)AzStrlen(buf);
        AZ_ASSERT(new_len + 1 <= buf_size
                  && "Is your input buffer properly zero-terminated?");
        state->WantReloadUserBuf = false;
        InputTextReconcileUndoState(state,
                                    state->TextA.Data,
                                    state->TextLen,
                                    buf,
                                    new_len);
        state->TextA.resize(buf_size + 1);
        state->TextLen = new_len;
        memcpy(state->TextA.Data, buf, state->TextLen + 1);
        state->Stb->select_start = state->ReloadSelectionStart;
        state->Stb->cursor = state->Stb->select_end = state->ReloadSelectionEnd;
        state->CursorClamp();
    } else if ((init_state && g.ActiveId != id) || init_changed_specs)
    {
        state = &g.InputTextState;
        state->CursorAnimReset();

        InputTextDeactivateHook(state->ID);

        const int buf_len = (int)AzStrlen(buf);
        AZ_ASSERT(buf_len + 1 <= buf_size
                  && "Is your input buffer properly zero-terminated?");
        state->TextToRevertTo.resize(buf_len + 1);
        memcpy(state->TextToRevertTo.Data, buf, buf_len + 1);

        bool recycle_state = (state->ID == id && !init_changed_specs);
        if (recycle_state
            && (state->TextLen != buf_len
                || (state->TextA.Data == NULL
                    || strncmp(state->TextA.Data, buf, buf_len) != 0)))
            recycle_state = false;

        state->ID = id;
        state->TextLen = buf_len;
        if (!is_readonly)
        {
            state->TextA.resize(buf_size + 1);
            memcpy(state->TextA.Data, buf, state->TextLen + 1);
        }

        state->Scroll = AzVec2(0.0f, 0.0f);
        if (flags & AzInputTextFlags_ElideLeft)
            state->Scroll.x += AzMax(0.0f,
                                     CalcTextSize(buf).x - frame_size.x
                                             + style.FramePadding.x * 2.0f);

        if (recycle_state)
            state->CursorClamp();
        else
            stb_textedit_initialize_state(state->Stb, !is_multiline);

        if (!is_multiline)
        {
            if (flags & AzInputTextFlags_AutoSelectAll)
                select_all = true;
            if (input_requested_by_nav
                && (!recycle_state
                    || !(g.NavActivateFlags
                         & AzActivateFlags_TryToPreserveState)))
                select_all = true;
            if (user_clicked && io.KeyCtrl)
                select_all = true;
        }

        if (flags & AzInputTextFlags_AlwaysOverwrite)
            state->Stb->insert_mode = 1;
    }

    const bool is_osx = io.ConfigMacOSXBehaviors;
    if (g.ActiveId != id && init_make_active)
    {
        AZ_ASSERT(state && state->ID == id);
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
    }
    if (g.ActiveId == id)
    {
        const AzKey always_owned_keys[] = {AzKey_LeftArrow,
                                           AzKey_RightArrow,
                                           AzKey_Enter,
                                           AzKey_KeypadEnter,
                                           AzKey_Delete,
                                           AzKey_Backspace,
                                           AzKey_Home,
                                           AzKey_End};
        for (AzKey key : always_owned_keys)
            SetKeyOwner(key, id);
        if (user_clicked)
            SetKeyOwner(AzKey_MouseLeft, id);
        g.ActiveIdUsingNavDirMask |= (1 << AzDir_Left) | (1 << AzDir_Right);
        if (is_multiline || (flags & AzInputTextFlags_CallbackHistory))
        {
            g.ActiveIdUsingNavDirMask |= (1 << AzDir_Up) | (1 << AzDir_Down);
            SetKeyOwner(AzKey_UpArrow, id);
            SetKeyOwner(AzKey_DownArrow, id);
        }
        if (is_multiline)
        {
            SetKeyOwner(AzKey_PageUp, id);
            SetKeyOwner(AzKey_PageDown, id);
        }

        if (is_osx)
            SetKeyOwner(AzMod_Alt, id);

        if (is_multiline && state != NULL)
            state->Scroll.y = draw_window->Scroll.y;

        if (is_readonly && state != NULL)
            state->TextLen = (int)AzStrlen(buf);
    }
    if (state != NULL)
        state->TextSrc = is_readonly ? buf : state->TextA.Data;

    if (g.ActiveId == id && state == NULL)
        ClearActiveID();

    if (g.ActiveId == id && io.MouseClicked[0] && !init_state
        && !init_make_active)
        clear_active_id = true;

    bool render_cursor = (g.ActiveId == id) || (state && user_scroll_active);
    bool render_selection = state && (state->HasSelection() || select_all)
                            && (RENDER_SELECTION_WHEN_INACTIVE
                                || render_cursor);
    bool value_changed = false;
    bool validated = false;

    const bool buf_display_from_state = (render_cursor || render_selection
                                         || g.ActiveId == id)
                                        && !is_readonly && state;
    bool is_displaying_hint =
            (hint != NULL
             && (buf_display_from_state ? state->TextA.Data : buf)[0] == 0);

    if (is_password && !is_displaying_hint)
        PushPasswordFont();

    if (g.ActiveId == id)
    {
        AZ_ASSERT(state != NULL);
        state->Edited = false;
        state->BufCapacity = buf_size;
        state->Flags = flags;

        g.ActiveIdAllowOverlap = !io.MouseDown[0];

        const float mouse_x = (io.MousePos.x - frame_bb.Min.x
                               - style.FramePadding.x)
                              + state->Scroll.x;
        const float mouse_y = (is_multiline ? (io.MousePos.y
                                               - draw_window->DC.CursorPos.y)
                                            : (g.FontSize * 0.5f));

        if (select_all)
        {
            state->SelectAll();
            state->SelectedAllMouseLock = true;
        } else if (hovered && io.MouseClickedCount[0] >= 2 && !io.KeyShift)
        {
            stb_textedit_click(state, state->Stb, mouse_x, mouse_y);
            const int multiclick_count = (io.MouseClickedCount[0] - 2);
            if ((multiclick_count % 2) == 0)
            {
                const bool is_bol =
                        (state->Stb->cursor == 0)
                        || AzStb::STB_TEXTEDIT_GETCHAR(state,
                                                       state->Stb->cursor - 1)
                                   == '\n';
                if (STB_TEXT_HAS_SELECTION(state->Stb) || !is_bol)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);

                if (!STB_TEXT_HAS_SELECTION(state->Stb))
                    AzStb::stb_textedit_prep_selection_at_cursor(state->Stb);
                state->Stb->cursor = AzStb::STB_TEXTEDIT_MOVEWORDRIGHT_MAC(
                        state,
                        state->Stb->cursor);
                state->Stb->select_end = state->Stb->cursor;
                AzStb::stb_textedit_clamp(state, state->Stb);
            } else
            {
                const bool is_eol =
                        AzStb::STB_TEXTEDIT_GETCHAR(state, state->Stb->cursor)
                        == '\n';
                state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART);
                state->OnKeyPressed(STB_TEXTEDIT_K_LINEEND
                                    | STB_TEXTEDIT_K_SHIFT);
                state->OnKeyPressed(STB_TEXTEDIT_K_RIGHT
                                    | STB_TEXTEDIT_K_SHIFT);
                if (!is_eol && is_multiline)
                {
                    AzSwap(state->Stb->select_start, state->Stb->select_end);
                    state->Stb->cursor = state->Stb->select_end;
                }
                state->CursorFollow = false;
            }
            state->CursorAnimReset();
        } else if (io.MouseClicked[0] && !state->SelectedAllMouseLock)
        {
            if (hovered)
            {
                if (io.KeyShift)
                    stb_textedit_drag(state, state->Stb, mouse_x, mouse_y);
                else
                    stb_textedit_click(state, state->Stb, mouse_x, mouse_y);
                state->CursorAnimReset();
            }
        } else if (io.MouseDown[0] && !state->SelectedAllMouseLock
                   && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(state, state->Stb, mouse_x, mouse_y);
            state->CursorAnimReset();
            state->CursorFollow = true;
        }
        if (state->SelectedAllMouseLock && !io.MouseDown[0])
            state->SelectedAllMouseLock = false;

        if ((flags & AzInputTextFlags_AllowTabInput) && !is_readonly)
        {
            if (Shortcut(AzKey_Tab, AzInputFlags_Repeat, id))
            {
                unsigned int c = '\t';
                if (InputTextFilterCharacter(&g,
                                             &c,
                                             flags,
                                             callback,
                                             callback_user_data))
                    state->OnCharPressed(c);
            }
        }

        const bool ignore_char_inputs = (io.KeyCtrl && !io.KeyAlt)
                                        || (is_osx && io.KeyCtrl);
        if (io.InputQueueCharacters.Size > 0)
        {
            if (!ignore_char_inputs && !is_readonly && !input_requested_by_nav)
                for (int n = 0; n < io.InputQueueCharacters.Size; n++)
                {
                    unsigned int c = (unsigned int)io.InputQueueCharacters[n];
                    if (c == '\t')
                        continue;
                    if (InputTextFilterCharacter(&g,
                                                 &c,
                                                 flags,
                                                 callback,
                                                 callback_user_data))
                        state->OnCharPressed(c);
                }

            io.InputQueueCharacters.resize(0);
        }
    }

    bool revert_edit = false;
    if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
    {
        AZ_ASSERT(state != NULL);

        const int row_count_per_page =
                AzMax((int)((inner_size.y - style.FramePadding.y) / g.FontSize),
                      1);
        state->Stb->row_count_per_page = row_count_per_page;

        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl;
        const bool is_startend_key_down = is_osx && io.KeyCtrl && !io.KeySuper
                                          && !io.KeyAlt;

        const AzInputFlags f_repeat = AzInputFlags_Repeat;
        const bool is_cut =
                (Shortcut(AzMod_Ctrl | AzKey_X, f_repeat, id)
                 || Shortcut(AzMod_Shift | AzKey_Delete, f_repeat, id))
                && !is_readonly && !is_password
                && (!is_multiline || state->HasSelection());
        const bool is_copy = (Shortcut(AzMod_Ctrl | AzKey_C, 0, id)
                              || Shortcut(AzMod_Ctrl | AzKey_Insert, 0, id))
                             && !is_password
                             && (!is_multiline || state->HasSelection());
        const bool is_paste =
                (Shortcut(AzMod_Ctrl | AzKey_V, f_repeat, id)
                 || Shortcut(AzMod_Shift | AzKey_Insert, f_repeat, id))
                && !is_readonly;
        const bool is_undo = (Shortcut(AzMod_Ctrl | AzKey_Z, f_repeat, id))
                             && !is_readonly && is_undoable;
        const bool is_redo =
                (Shortcut(AzMod_Ctrl | AzKey_Y, f_repeat, id)
                 || Shortcut(AzMod_Ctrl | AzMod_Shift | AzKey_Z, f_repeat, id))
                && !is_readonly && is_undoable;
        const bool is_select_all = Shortcut(AzMod_Ctrl | AzKey_A, 0, id);

        const bool nav_gamepad_active =
                (io.ConfigFlags & AzConfigFlags_NavEnableGamepad) != 0
                && (io.BackendFlags & AzBackendFlags_HasGamepad) != 0;
        const bool is_enter_pressed = IsKeyPressed(AzKey_Enter, true)
                                      || IsKeyPressed(AzKey_KeypadEnter, true);
        const bool is_gamepad_validate =
                nav_gamepad_active
                && (IsKeyPressed(AzKey_NavGamepadActivate, false)
                    || IsKeyPressed(AzKey_NavGamepadInput, false));
        const bool is_cancel =
                Shortcut(AzKey_Escape, f_repeat, id)
                || (nav_gamepad_active
                    && Shortcut(AzKey_NavGamepadCancel, f_repeat, id));

        if (IsKeyPressed(AzKey_LeftArrow))
        {
            state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART
                                 : is_wordmove_key_down
                                         ? STB_TEXTEDIT_K_WORDLEFT
                                         : STB_TEXTEDIT_K_LEFT)
                                | k_mask);
        } else if (IsKeyPressed(AzKey_RightArrow))
        {
            state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND
                                 : is_wordmove_key_down
                                         ? STB_TEXTEDIT_K_WORDRIGHT
                                         : STB_TEXTEDIT_K_RIGHT)
                                | k_mask);
        } else if (IsKeyPressed(AzKey_UpArrow) && is_multiline)
        {
            if (io.KeyCtrl)
                SetScrollY(draw_window,
                           AzMax(draw_window->Scroll.y - g.FontSize, 0.0f));
            else
                state->OnKeyPressed((is_startend_key_down
                                             ? STB_TEXTEDIT_K_TEXTSTART
                                             : STB_TEXTEDIT_K_UP)
                                    | k_mask);
        } else if (IsKeyPressed(AzKey_DownArrow) && is_multiline)
        {
            if (io.KeyCtrl)
                SetScrollY(draw_window,
                           AzMin(draw_window->Scroll.y + g.FontSize,
                                 GetScrollMaxY()));
            else
                state->OnKeyPressed((is_startend_key_down
                                             ? STB_TEXTEDIT_K_TEXTEND
                                             : STB_TEXTEDIT_K_DOWN)
                                    | k_mask);
        } else if (IsKeyPressed(AzKey_PageUp) && is_multiline)
        {
            state->OnKeyPressed(STB_TEXTEDIT_K_PGUP | k_mask);
            scroll_y -= row_count_per_page * g.FontSize;
        } else if (IsKeyPressed(AzKey_PageDown) && is_multiline)
        {
            state->OnKeyPressed(STB_TEXTEDIT_K_PGDOWN | k_mask);
            scroll_y += row_count_per_page * g.FontSize;
        } else if (IsKeyPressed(AzKey_Home))
        {
            state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask
                                           : STB_TEXTEDIT_K_LINESTART | k_mask);
        } else if (IsKeyPressed(AzKey_End))
        {
            state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask
                                           : STB_TEXTEDIT_K_LINEEND | k_mask);
        } else if (IsKeyPressed(AzKey_Delete) && !is_readonly && !is_cut)
        {
            if (!state->HasSelection())
            {
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT
                                        | STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask);
        } else if (IsKeyPressed(AzKey_Backspace) && !is_readonly)
        {
            if (!state->HasSelection())
            {
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT
                                        | STB_TEXTEDIT_K_SHIFT);
                else if (is_osx && io.KeyCtrl && !io.KeyAlt && !io.KeySuper)
                    state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART
                                        | STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        } else if (is_enter_pressed || is_gamepad_validate)
        {
            bool ctrl_enter_for_new_line =
                    (flags & AzInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || is_gamepad_validate
                || (ctrl_enter_for_new_line && !io.KeyCtrl)
                || (!ctrl_enter_for_new_line && io.KeyCtrl))
            {
                validated = true;
                if (io.ConfigInputTextEnterKeepActive && !is_multiline)
                    state->SelectAll();
                else
                    clear_active_id = true;
            } else if (!is_readonly)
            {
                unsigned int c = '\n';
                if (InputTextFilterCharacter(&g,
                                             &c,
                                             flags,
                                             callback,
                                             callback_user_data))
                    state->OnCharPressed(c);
            }
        } else if (is_cancel)
        {
            if (flags & AzInputTextFlags_EscapeClearsAll)
            {
                if (buf[0] != 0)
                {
                    revert_edit = true;
                } else
                {
                    render_cursor = render_selection = false;
                    clear_active_id = true;
                }
            } else
            {
                clear_active_id = revert_edit = true;
                render_cursor = render_selection = false;
            }
        } else if (is_undo || is_redo)
        {
            state->OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO
                                        : STB_TEXTEDIT_K_REDO);
            state->ClearSelection();
        } else if (is_select_all)
        {
            state->SelectAll();
            state->CursorFollow = true;
        } else if (is_cut || is_copy)
        {
            if (g.PlatformIO.Platform_SetClipboardTextFn != NULL)
            {
                const int ib = state->HasSelection()
                                       ? AzMin(state->Stb->select_start,
                                               state->Stb->select_end)
                                       : 0;
                const int ie = state->HasSelection()
                                       ? AzMax(state->Stb->select_start,
                                               state->Stb->select_end)
                                       : state->TextLen;
                g.TempBuffer.reserve(ie - ib + 1);
                memcpy(g.TempBuffer.Data, state->TextSrc + ib, ie - ib);
                g.TempBuffer.Data[ie - ib] = 0;
                SetClipboardText(g.TempBuffer.Data);
            }
            if (is_cut)
            {
                if (!state->HasSelection())
                    state->SelectAll();
                state->CursorFollow = true;
                stb_textedit_cut(state, state->Stb);
            }
        } else if (is_paste)
        {
            if (const char* clipboard = GetClipboardText())
            {
                const int clipboard_len = (int)AzStrlen(clipboard);
                AzVector<char> clipboard_filtered;
                clipboard_filtered.reserve(clipboard_len + 1);
                for (const char* s = clipboard; *s != 0;)
                {
                    unsigned int c;
                    int in_len = AzTextCharFromUtf8(&c, s, NULL);
                    s += in_len;
                    if (!InputTextFilterCharacter(&g,
                                                  &c,
                                                  flags,
                                                  callback,
                                                  callback_user_data,
                                                  true))
                        continue;
                    char c_utf8[5];
                    AzTextCharToUtf8(c_utf8, c);
                    int out_len = (int)AzStrlen(c_utf8);
                    clipboard_filtered.resize(clipboard_filtered.Size
                                              + out_len);
                    memcpy(clipboard_filtered.Data + clipboard_filtered.Size
                                   - out_len,
                           c_utf8,
                           out_len);
                }
                if (clipboard_filtered.Size > 0)
                {
                    clipboard_filtered.push_back(0);
                    stb_textedit_paste(state,
                                       state->Stb,
                                       clipboard_filtered.Data,
                                       clipboard_filtered.Size - 1);
                    state->CursorFollow = true;
                }
            }
        }

        render_selection |= state->HasSelection()
                            && (RENDER_SELECTION_WHEN_INACTIVE
                                || render_cursor);
    }

    const char* apply_new_text = NULL;
    int apply_new_text_length = 0;
    if (g.ActiveId == id)
    {
        AZ_ASSERT(state != NULL);
        if (revert_edit && !is_readonly)
        {
            if (flags & AzInputTextFlags_EscapeClearsAll)
            {
                AZ_ASSERT(buf[0] != 0);
                apply_new_text = "";
                apply_new_text_length = 0;
                value_changed = true;
                AZSTB_TEXTEDIT_CHARTYPE empty_string;
                stb_textedit_replace(state, state->Stb, &empty_string, 0);
            } else if (strcmp(buf, state->TextToRevertTo.Data) != 0)
            {
                apply_new_text = state->TextToRevertTo.Data;
                apply_new_text_length = state->TextToRevertTo.Size - 1;

                value_changed = true;
                stb_textedit_replace(state,
                                     state->Stb,
                                     state->TextToRevertTo.Data,
                                     state->TextToRevertTo.Size - 1);
            }
        }

        const bool apply_edit_back_to_user_buffer = true;
        if (apply_edit_back_to_user_buffer)
        {
            if ((flags
                 & (AzInputTextFlags_CallbackCompletion
                    | AzInputTextFlags_CallbackHistory
                    | AzInputTextFlags_CallbackEdit
                    | AzInputTextFlags_CallbackAlways))
                != 0)
            {
                AZ_ASSERT(callback != NULL);

                AzInputTextFlags event_flag = 0;
                AzKey event_key = AzKey_None;
                if ((flags & AzInputTextFlags_CallbackCompletion) != 0
                    && Shortcut(AzKey_Tab, 0, id))
                {
                    event_flag = AzInputTextFlags_CallbackCompletion;
                    event_key = AzKey_Tab;
                } else if ((flags & AzInputTextFlags_CallbackHistory) != 0
                           && IsKeyPressed(AzKey_UpArrow))
                {
                    event_flag = AzInputTextFlags_CallbackHistory;
                    event_key = AzKey_UpArrow;
                } else if ((flags & AzInputTextFlags_CallbackHistory) != 0
                           && IsKeyPressed(AzKey_DownArrow))
                {
                    event_flag = AzInputTextFlags_CallbackHistory;
                    event_key = AzKey_DownArrow;
                } else if ((flags & AzInputTextFlags_CallbackEdit)
                           && state->Edited)
                {
                    event_flag = AzInputTextFlags_CallbackEdit;
                } else if (flags & AzInputTextFlags_CallbackAlways)
                {
                    event_flag = AzInputTextFlags_CallbackAlways;
                }

                if (event_flag)
                {
                    AzInputTextCallbackData callback_data;
                    callback_data.Ctx = &g;
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags = flags;
                    callback_data.UserData = callback_user_data;

                    char* callback_buf = is_readonly ? buf : state->TextA.Data;
                    AZ_ASSERT(callback_buf == state->TextSrc);
                    state->CallbackTextBackup.resize(state->TextLen + 1);
                    memcpy(state->CallbackTextBackup.Data,
                           callback_buf,
                           state->TextLen + 1);

                    callback_data.EventKey = event_key;
                    callback_data.Buf = callback_buf;
                    callback_data.BufTextLen = state->TextLen;
                    callback_data.BufSize = state->BufCapacity;
                    callback_data.BufDirty = false;

                    const int utf8_cursor_pos = callback_data.CursorPos =
                            state->Stb->cursor;
                    const int utf8_selection_start =
                            callback_data.SelectionStart =
                                    state->Stb->select_start;
                    const int utf8_selection_end = callback_data.SelectionEnd =
                            state->Stb->select_end;

                    callback(&callback_data);

                    callback_buf = is_readonly ? buf : state->TextA.Data;
                    AZ_ASSERT(callback_data.Buf == callback_buf);
                    AZ_ASSERT(callback_data.BufSize == state->BufCapacity);
                    AZ_ASSERT(callback_data.Flags == flags);
                    const bool buf_dirty = callback_data.BufDirty;
                    if (callback_data.CursorPos != utf8_cursor_pos || buf_dirty)
                    {
                        state->Stb->cursor = callback_data.CursorPos;
                        state->CursorFollow = true;
                    }
                    if (callback_data.SelectionStart != utf8_selection_start
                        || buf_dirty)
                    {
                        state->Stb->select_start =
                                (callback_data.SelectionStart
                                 == callback_data.CursorPos)
                                        ? state->Stb->cursor
                                        : callback_data.SelectionStart;
                    }
                    if (callback_data.SelectionEnd != utf8_selection_end
                        || buf_dirty)
                    {
                        state->Stb->select_end =
                                (callback_data.SelectionEnd
                                 == callback_data.SelectionStart)
                                        ? state->Stb->select_start
                                        : callback_data.SelectionEnd;
                    }
                    if (buf_dirty)
                    {
                        AZ_ASSERT(callback_data.BufTextLen
                                  == (int)AzStrlen(callback_data.Buf));
                        InputTextReconcileUndoState(
                                state,
                                state->CallbackTextBackup.Data,
                                state->CallbackTextBackup.Size - 1,
                                callback_data.Buf,
                                callback_data.BufTextLen);
                        state->TextLen = callback_data.BufTextLen;
                        state->CursorAnimReset();
                    }
                }
            }

            if (!is_readonly && strcmp(state->TextSrc, buf) != 0)
            {
                apply_new_text = state->TextSrc;
                apply_new_text_length = state->TextLen;
                value_changed = true;
            }
        }
    }

    if (g.InputTextDeactivatedState.ID == id)
    {
        if (g.ActiveId != id && IsItemDeactivatedAfterEdit() && !is_readonly
            && strcmp(g.InputTextDeactivatedState.TextA.Data, buf) != 0)
        {
            apply_new_text = g.InputTextDeactivatedState.TextA.Data;
            apply_new_text_length = g.InputTextDeactivatedState.TextA.Size - 1;
            value_changed = true;
        }
        g.InputTextDeactivatedState.ID = 0;
    }

    if (apply_new_text != NULL)
    {
        AZ_ASSERT(apply_new_text_length >= 0);
        if (is_resizable)
        {
            AzInputTextCallbackData callback_data;
            callback_data.Ctx = &g;
            callback_data.EventFlag = AzInputTextFlags_CallbackResize;
            callback_data.Flags = flags;
            callback_data.Buf = buf;
            callback_data.BufTextLen = apply_new_text_length;
            callback_data.BufSize = AzMax(buf_size, apply_new_text_length + 1);
            callback_data.UserData = callback_user_data;
            callback(&callback_data);
            buf = callback_data.Buf;
            buf_size = callback_data.BufSize;
            apply_new_text_length = AzMin(callback_data.BufTextLen,
                                          buf_size - 1);
            AZ_ASSERT(apply_new_text_length <= buf_size);
        }

        AzStrncpy(buf,
                  apply_new_text,
                  AzMin(apply_new_text_length + 1, buf_size));
    }

    if (g.ActiveId == id && clear_active_id)
        ClearActiveID();

    if (!is_multiline)
    {
        RenderNavCursor(frame_bb, id);
        RenderFrame(frame_bb.Min,
                    frame_bb.Max,
                    GetColorU32(AzCol_FrameBg),
                    true,
                    style.FrameRounding);
    }

    const AzVec4 clip_rect(frame_bb.Min.x,
                           frame_bb.Min.y,
                           frame_bb.Min.x + inner_size.x,
                           frame_bb.Min.y + inner_size.y);
    AzVec2 draw_pos = is_multiline ? draw_window->DC.CursorPos
                                   : frame_bb.Min + style.FramePadding;
    AzVec2 text_size(0.0f, 0.0f);

    const int buf_display_max_length = 2 * 1024 * 1024;
    const char* buf_display = buf_display_from_state ? state->TextA.Data : buf;
    const char* buf_display_end = NULL;

    const bool new_is_displaying_hint =
            (hint != NULL
             && (buf_display_from_state ? state->TextA.Data : buf)[0] == 0);
    if (new_is_displaying_hint != is_displaying_hint)
    {
        if (is_password && !is_displaying_hint)
            PopPasswordFont();
        is_displaying_hint = new_is_displaying_hint;
        if (is_password && !is_displaying_hint)
            PushPasswordFont();
    }
    if (is_displaying_hint)
    {
        buf_display = hint;
        buf_display_end = hint + AzStrlen(hint);
    }

    if (render_cursor || render_selection)
    {
        AZ_ASSERT(state != NULL);
        if (!is_displaying_hint)
            buf_display_end = buf_display + state->TextLen;

        const char* text_begin = buf_display;
        const char* text_end = text_begin + state->TextLen;
        AzVec2 cursor_offset, select_start_offset;

        {
            int cursor_line_no = render_cursor ? -1 : -1000;
            int selmin_line_no = render_selection ? -1 : -1000;
            const char* cursor_ptr = render_cursor
                                             ? text_begin + state->Stb->cursor
                                             : NULL;
            const char* selmin_ptr =
                    render_selection ? text_begin
                                               + AzMin(state->Stb->select_start,
                                                       state->Stb->select_end)
                                     : NULL;

            int line_count = 1;
            if (is_multiline)
            {
                for (const char* s = text_begin;
                     (s = (const char*)
                              AzMemchr(s, '\n', (size_t)(text_end - s)))
                     != NULL;
                     s++)
                {
                    if (cursor_line_no == -1 && s >= cursor_ptr)
                    {
                        cursor_line_no = line_count;
                    }
                    if (selmin_line_no == -1 && s >= selmin_ptr)
                    {
                        selmin_line_no = line_count;
                    }
                    line_count++;
                }
            }
            if (cursor_line_no == -1)
                cursor_line_no = line_count;
            if (selmin_line_no == -1)
                selmin_line_no = line_count;

            cursor_offset.x = InputTextCalcTextSize(&g,
                                                    AzStrbol(cursor_ptr,
                                                             text_begin),
                                                    cursor_ptr)
                                      .x;
            cursor_offset.y = cursor_line_no * g.FontSize;
            if (selmin_line_no >= 0)
            {
                select_start_offset.x =
                        InputTextCalcTextSize(&g,
                                              AzStrbol(selmin_ptr, text_begin),
                                              selmin_ptr)
                                .x;
                select_start_offset.y = selmin_line_no * g.FontSize;
            }

            if (is_multiline)
                text_size = AzVec2(inner_size.x, line_count * g.FontSize);
        }

        if (render_cursor && state->CursorFollow)
        {
            if (!(flags & AzInputTextFlags_NoHorizontalScroll))
            {
                const float scroll_increment_x = inner_size.x * 0.25f;
                const float visible_width = inner_size.x - style.FramePadding.x;
                if (cursor_offset.x < state->Scroll.x)
                    state->Scroll.x = AZ_TRUNC(
                            AzMax(0.0f, cursor_offset.x - scroll_increment_x));
                else if (cursor_offset.x - visible_width >= state->Scroll.x)
                    state->Scroll.x = AZ_TRUNC(cursor_offset.x - visible_width
                                               + scroll_increment_x);
            } else
            {
                state->Scroll.y = 0.0f;
            }

            if (is_multiline)
            {
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = AzMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y
                                 - (inner_size.y - style.FramePadding.y * 2.0f)
                         >= scroll_y)
                    scroll_y = cursor_offset.y - inner_size.y
                               + style.FramePadding.y * 2.0f;
                const float scroll_max_y = AzMax((text_size.y
                                                  + style.FramePadding.y * 2.0f)
                                                         - inner_size.y,
                                                 0.0f);
                scroll_y = AzClamp(scroll_y, 0.0f, scroll_max_y);
                draw_pos.y += (draw_window->Scroll.y - scroll_y);
                draw_window->Scroll.y = scroll_y;
            }

            state->CursorFollow = false;
        }

        const AzVec2 draw_scroll = AzVec2(state->Scroll.x, 0.0f);
        if (render_selection)
        {
            const char* text_selected_begin = text_begin
                                              + AzMin(state->Stb->select_start,
                                                      state->Stb->select_end);
            const char* text_selected_end = text_begin
                                            + AzMax(state->Stb->select_start,
                                                    state->Stb->select_end);

            AzU32 bg_color = GetColorU32(AzCol_TextSelectedBg,
                                         render_cursor ? 1.0f : 0.6f);
            float bg_offy_up = is_multiline ? 0.0f : -1.0f;
            float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            AzVec2 rect_pos = draw_pos + select_start_offset - draw_scroll;
            for (const char* p = text_selected_begin; p < text_selected_end;)
            {
                if (rect_pos.y > clip_rect.w + g.FontSize)
                    break;
                if (rect_pos.y < clip_rect.y)
                {
                    p = (const char*)AzMemchr((void*)p,
                                              '\n',
                                              text_selected_end - p);
                    p = p ? p + 1 : text_selected_end;
                } else
                {
                    AzVec2 rect_size = InputTextCalcTextSize(&g,
                                                             p,
                                                             text_selected_end,
                                                             &p,
                                                             NULL,
                                                             true);
                    if (rect_size.x <= 0.0f)
                        rect_size.x = AZ_TRUNC(
                                g.FontBaked->GetCharAdvance((AzWchar)' ')
                                * 0.50f);
                    AzRect rect(rect_pos
                                        + AzVec2(0.0f, bg_offy_up - g.FontSize),
                                rect_pos + AzVec2(rect_size.x, bg_offy_dn));
                    rect.ClipWith(clip_rect);
                    if (rect.Overlaps(clip_rect))
                        draw_window->DrawList->AddRectFilled(rect.Min,
                                                             rect.Max,
                                                             bg_color);
                    rect_pos.x = draw_pos.x - draw_scroll.x;
                }
                rect_pos.y += g.FontSize;
            }
        }

        if (is_multiline
            || (buf_display_end - buf_display) < buf_display_max_length)
        {
            AzU32 col = GetColorU32(is_displaying_hint ? AzCol_TextDisabled
                                                       : AzCol_Text);
            draw_window->DrawList->AddText(g.Font,
                                           g.FontSize,
                                           draw_pos - draw_scroll,
                                           col,
                                           buf_display,
                                           buf_display_end,
                                           0.0f,
                                           is_multiline ? NULL : &clip_rect);
        }

        if (render_cursor)
        {
            state->CursorAnim += io.DeltaTime;
            bool cursor_is_visible = (!g.IO.ConfigInputTextCursorBlink)
                                     || (state->CursorAnim <= 0.0f)
                                     || AzFmod(state->CursorAnim, 1.20f)
                                                <= 0.80f;
            AzVec2 cursor_screen_pos = AzTrunc(draw_pos + cursor_offset
                                               - draw_scroll);
            AzRect cursor_screen_rect(cursor_screen_pos.x,
                                      cursor_screen_pos.y - g.FontSize + 0.5f,
                                      cursor_screen_pos.x + 1.0f,
                                      cursor_screen_pos.y - 1.5f);
            if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
                draw_window->DrawList->AddLine(cursor_screen_rect.Min,
                                               cursor_screen_rect.GetBL(),
                                               GetColorU32(
                                                       AzCol_InputTextCursor),
                                               1.0f);

            if (!is_readonly && g.ActiveId == id)
            {
                AzPlatformAzeData* ime_data = &g.PlatformAzeData;
                ime_data->WantVisible = true;
                ime_data->WantTextInput = true;
                ime_data->InputPos = AzVec2(cursor_screen_pos.x - 1.0f,
                                            cursor_screen_pos.y - g.FontSize);
                ime_data->InputLineHeight = g.FontSize;
                ime_data->ViewportId = window->Viewport->ID;
            }
        }
    } else
    {
        if (is_multiline)
            text_size =
                    AzVec2(inner_size.x,
                           InputTextCalcTextLenAndLineCount(buf_display,
                                                            &buf_display_end)
                                   * g.FontSize);
        else if (!is_displaying_hint && g.ActiveId == id)
            buf_display_end = buf_display + state->TextLen;
        else if (!is_displaying_hint)
            buf_display_end = buf_display + AzStrlen(buf_display);

        if (is_multiline
            || (buf_display_end - buf_display) < buf_display_max_length)
        {
            if (flags & AzInputTextFlags_ElideLeft)
                draw_pos.x = AzMin(draw_pos.x,
                                   frame_bb.Max.x
                                           - CalcTextSize(buf_display, NULL).x
                                           - style.FramePadding.x);

            const AzVec2 draw_scroll = AzVec2(0.0f, 0.0f);
            AzU32 col = GetColorU32(is_displaying_hint ? AzCol_TextDisabled
                                                       : AzCol_Text);
            draw_window->DrawList->AddText(g.Font,
                                           g.FontSize,
                                           draw_pos - draw_scroll,
                                           col,
                                           buf_display,
                                           buf_display_end,
                                           0.0f,
                                           is_multiline ? NULL : &clip_rect);
        }
    }

    if (is_password && !is_displaying_hint)
        PopPasswordFont();

    if (is_multiline)
    {
        Dummy(AzVec2(text_size.x, text_size.y + style.FramePadding.y));
        g.NextItemData.ItemFlags |= (AzItemFlags)AzItemFlags_Inputable
                                    | AzItemFlags_NoTabStop;
        EndChild();
        item_data_backup.StatusFlags |= (g.LastItemData.StatusFlags
                                         & AzItemStatusFlags_HoveredWindow);

        EndGroup();
        if (g.LastItemData.ID == 0
            || g.LastItemData.ID != GetWindowScrollbarID(draw_window, AzAxis_Y))
        {
            g.LastItemData.ID = id;
            g.LastItemData.ItemFlags = item_data_backup.ItemFlags;
            g.LastItemData.StatusFlags = item_data_backup.StatusFlags;
        }
    }
    if (state)
        state->TextSrc = NULL;

    if (g.LogEnabled && (!is_password || is_displaying_hint))
    {
        LogSetNextTextDecoration("{", "}");
        LogRenderedText(&draw_pos, buf_display, buf_display_end);
    }

    if (label_size.x > 0)
        RenderText(AzVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                          frame_bb.Min.y + style.FramePadding.y),
                   label);

    if (value_changed)
        MarkItemEdited(id);

    AZ_TEST_ENGINE_ITEM_INFO(id,
                             label,
                             g.LastItemData.StatusFlags
                                     | AzItemStatusFlags_Inputable);
    if ((flags & AzInputTextFlags_EnterReturnsTrue) != 0)
        return validated;
    else
        return value_changed;
}

void Az::DebugNodeInputTextState(AzInputTextState* state)
{
#ifndef AZ_DISABLE_DEBUG_TOOLS
    AzContext& g = *GAz;
    AzStb::STB_TexteditState* stb_state = state->Stb;
    AzStb::StbUndoState* undo_state = &stb_state->undostate;
    Text("ID: 0x%08X, ActiveID: 0x%08X", state->ID, g.ActiveId);
    DebugLocateItemOnHover(state->ID);
    Text("CurLenA: %d, Cursor: %d, Selection: %d..%d",
         state->TextLen,
         stb_state->cursor,
         stb_state->select_start,
         stb_state->select_end);
    Text("BufCapacityA: %d", state->BufCapacity);
    Text("(Internal Buffer: TextA Size: %d, Capacity: %d)",
         state->TextA.Size,
         state->TextA.Capacity);
    Text("has_preferred_x: %d (%.2f)",
         stb_state->has_preferred_x,
         stb_state->preferred_x);
    Text("undo_point: %d, redo_point: %d, undo_char_point: %d, "
         "redo_char_point: %d",
         undo_state->undo_point,
         undo_state->redo_point,
         undo_state->undo_char_point,
         undo_state->redo_char_point);
    if (BeginChild("undopoints",
                   AzVec2(0.0f, GetTextLineHeight() * 10),
                   AzChildFlags_Borders | AzChildFlags_ResizeY))
    {
        PushStyleVar(AzStyleVar_ItemSpacing, AzVec2(0, 0));
        for (int n = 0; n < AZSTB_TEXTEDIT_UNDOSTATECOUNT; n++)
        {
            AzStb::StbUndoRecord* undo_rec = &undo_state->undo_rec[n];
            const char undo_rec_type = (n < undo_state->undo_point)    ? 'u'
                                       : (n >= undo_state->redo_point) ? 'r'
                                                                       : ' ';
            if (undo_rec_type == ' ')
                BeginDisabled();
            const int buf_preview_len = (undo_rec_type != ' '
                                         && undo_rec->char_storage != -1)
                                                ? undo_rec->insert_length
                                                : 0;
            const char* buf_preview_str = undo_state->undo_char
                                          + undo_rec->char_storage;
            Text("%c [%02d] where %03d, insert %03d, delete %03d, char_storage "
                 "%03d \"%.*s\"",
                 undo_rec_type,
                 n,
                 undo_rec->where,
                 undo_rec->insert_length,
                 undo_rec->delete_length,
                 undo_rec->char_storage,
                 buf_preview_len,
                 buf_preview_str);
            if (undo_rec_type == ' ')
                EndDisabled();
        }
        PopStyleVar();
    }
    EndChild();
#else
    AZ_UNUSED(state);
#endif
}

bool Az::ColorEdit3(const char* label, float col[3], AzColorEditFlags flags)
{
    return ColorEdit4(label, col, flags | AzColorEditFlags_NoAlpha);
}

static void ColorEditRestoreH(const float* col, float* H)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.ColorEditCurrentID != 0);
    if (g.ColorEditSavedID != g.ColorEditCurrentID
        || g.ColorEditSavedColor
                   != Az::ColorConvertFloat4ToU32(
                           AzVec4(col[0], col[1], col[2], 0)))
        return;
    *H = g.ColorEditSavedHue;
}

static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
{
    AzContext& g = *GAz;
    AZ_ASSERT(g.ColorEditCurrentID != 0);
    if (g.ColorEditSavedID != g.ColorEditCurrentID
        || g.ColorEditSavedColor
                   != Az::ColorConvertFloat4ToU32(
                           AzVec4(col[0], col[1], col[2], 0)))
        return;

    if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
        *H = g.ColorEditSavedHue;

    if (*V == 0.0f)
        *S = g.ColorEditSavedSat;
}

bool Az::ColorEdit4(const char* label, float col[4], AzColorEditFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const float square_sz = GetFrameHeight();
    const char* label_display_end = FindRenderedTextEnd(label);
    float w_full = CalcItemWidth();
    g.NextItemData.ClearFlags();

    BeginGroup();
    PushID(label);
    const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
    if (set_current_color_edit_id)
        g.ColorEditCurrentID = window->IDStack.back();

    const AzColorEditFlags flags_untouched = flags;
    if (flags & AzColorEditFlags_NoInputs)
        flags = (flags & (~AzColorEditFlags_DisplayMask_))
                | AzColorEditFlags_DisplayRGB | AzColorEditFlags_NoOptions;

    if (!(flags & AzColorEditFlags_NoOptions))
        ColorEditOptionsPopup(col, flags);

    if (!(flags & AzColorEditFlags_DisplayMask_))
        flags |= (g.ColorEditOptions & AzColorEditFlags_DisplayMask_);
    if (!(flags & AzColorEditFlags_DataTypeMask_))
        flags |= (g.ColorEditOptions & AzColorEditFlags_DataTypeMask_);
    if (!(flags & AzColorEditFlags_PickerMask_))
        flags |= (g.ColorEditOptions & AzColorEditFlags_PickerMask_);
    if (!(flags & AzColorEditFlags_InputMask_))
        flags |= (g.ColorEditOptions & AzColorEditFlags_InputMask_);
    flags |= (g.ColorEditOptions
              & ~(AzColorEditFlags_DisplayMask_ | AzColorEditFlags_DataTypeMask_
                  | AzColorEditFlags_PickerMask_
                  | AzColorEditFlags_InputMask_));
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_DisplayMask_));
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_InputMask_));

    const bool alpha = (flags & AzColorEditFlags_NoAlpha) == 0;
    const bool hdr = (flags & AzColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;
    const float w_button = (flags & AzColorEditFlags_NoSmallPreview)
                                   ? 0.0f
                                   : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = AzMax(w_full - w_button, 1.0f);
    w_full = w_inputs + w_button;

    float f[4] = {col[0], col[1], col[2], alpha ? col[3] : 1.0f};
    if ((flags & AzColorEditFlags_InputHSV)
        && (flags & AzColorEditFlags_DisplayRGB))
        ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    else if ((flags & AzColorEditFlags_InputRGB)
             && (flags & AzColorEditFlags_DisplayHSV))
    {
        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
        ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
    }
    int i[4] = {AZ_F32_TO_INT8_UNBOUND(f[0]),
                AZ_F32_TO_INT8_UNBOUND(f[1]),
                AZ_F32_TO_INT8_UNBOUND(f[2]),
                AZ_F32_TO_INT8_UNBOUND(f[3])};

    bool value_changed = false;
    bool value_changed_as_float = false;

    const AzVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = (style.ColorButtonPosition == AzDir_Left)
                                          ? w_button
                                          : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;

    if ((flags & (AzColorEditFlags_DisplayRGB | AzColorEditFlags_DisplayHSV))
                != 0
        && (flags & AzColorEditFlags_NoInputs) == 0)
    {
        const float w_items = w_inputs
                              - style.ItemInnerSpacing.x * (components - 1);

        const bool hide_prefix =
                (AZ_TRUNC(w_items / components)
                 <= CalcTextSize((flags & AzColorEditFlags_Float) ? "M:0.000"
                                                                  : "M:000")
                            .x);
        static const char* ids[4] = {"##X", "##Y", "##Z", "##W"};
        static const char* fmt_table_int[3][4] = {
                {"%3d", "%3d", "%3d", "%3d"},
                {"R:%3d", "G:%3d", "B:%3d", "A:%3d"},
                {"H:%3d", "S:%3d", "V:%3d", "A:%3d"}};
        static const char* fmt_table_float[3][4] = {
                {"%0.3f", "%0.3f", "%0.3f", "%0.3f"},
                {"R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f"},
                {"H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f"}};
        const int fmt_idx = hide_prefix                             ? 0
                            : (flags & AzColorEditFlags_DisplayHSV) ? 2
                                                                    : 1;

        float prev_split = 0.0f;
        for (int n = 0; n < components; n++)
        {
            if (n > 0)
                SameLine(0, style.ItemInnerSpacing.x);
            float next_split = AZ_TRUNC(w_items * (n + 1) / components);
            SetNextItemWidth(AzMax(next_split - prev_split, 1.0f));
            prev_split = next_split;

            if (flags & AzColorEditFlags_Float)
            {
                value_changed |= DragFloat(ids[n],
                                           &f[n],
                                           1.0f / 255.0f,
                                           0.0f,
                                           hdr ? 0.0f : 1.0f,
                                           fmt_table_float[fmt_idx][n]);
                value_changed_as_float |= value_changed;
            } else
            {
                value_changed |= DragInt(ids[n],
                                         &i[n],
                                         1.0f,
                                         0,
                                         hdr ? 0 : 255,
                                         fmt_table_int[fmt_idx][n]);
            }
            if (!(flags & AzColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", AzPopupFlags_MouseButtonRight);
        }
    } else if ((flags & AzColorEditFlags_DisplayHex) != 0
               && (flags & AzColorEditFlags_NoInputs) == 0)
    {
        char buf[64];
        if (alpha)
            AzFormatString(buf,
                           AZ_ARRAYSIZE(buf),
                           "#%02X%02X%02X%02X",
                           AzClamp(i[0], 0, 255),
                           AzClamp(i[1], 0, 255),
                           AzClamp(i[2], 0, 255),
                           AzClamp(i[3], 0, 255));
        else
            AzFormatString(buf,
                           AZ_ARRAYSIZE(buf),
                           "#%02X%02X%02X",
                           AzClamp(i[0], 0, 255),
                           AzClamp(i[1], 0, 255),
                           AzClamp(i[2], 0, 255));
        SetNextItemWidth(w_inputs);
        if (InputText("##Text",
                      buf,
                      AZ_ARRAYSIZE(buf),
                      AzInputTextFlags_CharsUppercase))
        {
            value_changed = true;
            char* p = buf;
            while (*p == '#' || AzCharIsBlankA(*p))
                p++;
            i[0] = i[1] = i[2] = 0;
            i[3] = 0xFF;
            int r;
            if (alpha)
                r = sscanf(p,
                           "%02X%02X%02X%02X",
                           (unsigned int*)&i[0],
                           (unsigned int*)&i[1],
                           (unsigned int*)&i[2],
                           (unsigned int*)&i[3]);
            else
                r = sscanf(p,
                           "%02X%02X%02X",
                           (unsigned int*)&i[0],
                           (unsigned int*)&i[1],
                           (unsigned int*)&i[2]);
            AZ_UNUSED(r);
        }
        if (!(flags & AzColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", AzPopupFlags_MouseButtonRight);
    }

    AzWindow* picker_active_window = NULL;
    if (!(flags & AzColorEditFlags_NoSmallPreview))
    {
        const float button_offset_x =
                ((flags & AzColorEditFlags_NoInputs)
                 || (style.ColorButtonPosition == AzDir_Left))
                        ? 0.0f
                        : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = AzVec2(pos.x + button_offset_x, pos.y);

        const AzVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (ColorButton("##ColorButton", col_v4, flags))
        {
            if (!(flags & AzColorEditFlags_NoPicker))
            {
                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(g.LastItemData.Rect.GetBL()
                                 + AzVec2(0.0f, style.ItemSpacing.y));
            }
        }
        if (!(flags & AzColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", AzPopupFlags_MouseButtonRight);

        if (BeginPopup("picker"))
        {
            if (g.CurrentWindow->BeginCount == 1)
            {
                picker_active_window = g.CurrentWindow;
                if (label != label_display_end)
                {
                    TextEx(label, label_display_end);
                    Spacing();
                }
                AzColorEditFlags picker_flags_to_forward =
                        AzColorEditFlags_DataTypeMask_
                        | AzColorEditFlags_PickerMask_
                        | AzColorEditFlags_InputMask_ | AzColorEditFlags_HDR
                        | AzColorEditFlags_NoAlpha | AzColorEditFlags_AlphaBar;
                AzColorEditFlags picker_flags =
                        (flags_untouched & picker_flags_to_forward)
                        | AzColorEditFlags_DisplayMask_
                        | AzColorEditFlags_NoLabel
                        | AzColorEditFlags_AlphaPreviewHalf;
                SetNextItemWidth(square_sz * 12.0f);
                value_changed |= ColorPicker4("##picker",
                                              col,
                                              picker_flags,
                                              &g.ColorPickerRef.x);
            }
            EndPopup();
        }
    }

    if (label != label_display_end && !(flags & AzColorEditFlags_NoLabel))
    {
        SameLine(0.0f, style.ItemInnerSpacing.x);
        window->DC.CursorPos.x =
                pos.x
                + ((flags & AzColorEditFlags_NoInputs)
                           ? w_button
                           : w_full + style.ItemInnerSpacing.x);
        TextEx(label, label_display_end);
    }

    if (value_changed && picker_active_window == NULL)
    {
        if (!value_changed_as_float)
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if ((flags & AzColorEditFlags_DisplayHSV)
            && (flags & AzColorEditFlags_InputRGB))
        {
            g.ColorEditSavedHue = f[0];
            g.ColorEditSavedSat = f[1];
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32(
                    AzVec4(f[0], f[1], f[2], 0));
        }
        if ((flags & AzColorEditFlags_DisplayRGB)
            && (flags & AzColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        col[0] = f[0];
        col[1] = f[1];
        col[2] = f[2];
        if (alpha)
            col[3] = f[3];
    }

    if (set_current_color_edit_id)
        g.ColorEditCurrentID = 0;
    PopID();
    EndGroup();

    if ((g.LastItemData.StatusFlags & AzItemStatusFlags_HoveredRect)
        && !(g.LastItemData.ItemFlags & AzItemFlags_ReadOnly)
        && !(flags & AzColorEditFlags_NoDragDrop) && BeginDragDropTarget())
    {
        bool accepted_drag_drop = false;
        if (const AzPayload* payload = AcceptDragDropPayload(
                    AZ_PAYLOAD_TYPE_COLOR_3F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * 3);
            value_changed = accepted_drag_drop = true;
        }
        if (const AzPayload* payload = AcceptDragDropPayload(
                    AZ_PAYLOAD_TYPE_COLOR_4F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * components);
            value_changed = accepted_drag_drop = true;
        }

        if (accepted_drag_drop && (flags & AzColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(col[0],
                                 col[1],
                                 col[2],
                                 col[0],
                                 col[1],
                                 col[2]);
        EndDragDropTarget();
    }

    if (picker_active_window && g.ActiveId != 0
        && g.ActiveIdWindow == picker_active_window)
        g.LastItemData.ID = g.ActiveId;

    if (value_changed && g.LastItemData.ID != 0)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

bool Az::ColorPicker3(const char* label, float col[3], AzColorEditFlags flags)
{
    float col4[4] = {col[0], col[1], col[2], 1.0f};
    if (!ColorPicker4(label, col4, flags | AzColorEditFlags_NoAlpha))
        return false;
    col[0] = col4[0];
    col[1] = col4[1];
    col[2] = col4[2];
    return true;
}

static void RenderArrowsForVerticalBar(AzDrawList* draw_list,
                                       AzVec2 pos,
                                       AzVec2 half_sz,
                                       float bar_w,
                                       float alpha)
{
    AzU32 alpha8 = AZ_F32_TO_INT8_SAT(alpha);
    Az::RenderArrowPointingAt(draw_list,
                              AzVec2(pos.x + half_sz.x + 1, pos.y),
                              AzVec2(half_sz.x + 2, half_sz.y + 1),
                              AzDir_Right,
                              AZ_COL32(0, 0, 0, alpha8));
    Az::RenderArrowPointingAt(draw_list,
                              AzVec2(pos.x + half_sz.x, pos.y),
                              half_sz,
                              AzDir_Right,
                              AZ_COL32(255, 255, 255, alpha8));
    Az::RenderArrowPointingAt(draw_list,
                              AzVec2(pos.x + bar_w - half_sz.x - 1, pos.y),
                              AzVec2(half_sz.x + 2, half_sz.y + 1),
                              AzDir_Left,
                              AZ_COL32(0, 0, 0, alpha8));
    Az::RenderArrowPointingAt(draw_list,
                              AzVec2(pos.x + bar_w - half_sz.x, pos.y),
                              half_sz,
                              AzDir_Left,
                              AZ_COL32(255, 255, 255, alpha8));
}

bool Az::ColorPicker4(const char* label,
                      float col[4],
                      AzColorEditFlags flags,
                      const float* ref_col)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzDrawList* draw_list = window->DrawList;
    AzStyle& style = g.Style;
    AzIO& io = g.IO;

    const float width = CalcItemWidth();
    const bool is_readonly = ((g.NextItemData.ItemFlags | g.CurrentItemFlags)
                              & AzItemFlags_ReadOnly)
                             != 0;
    g.NextItemData.ClearFlags();

    PushID(label);
    const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
    if (set_current_color_edit_id)
        g.ColorEditCurrentID = window->IDStack.back();
    BeginGroup();

    if (!(flags & AzColorEditFlags_NoSidePreview))
        flags |= AzColorEditFlags_NoSmallPreview;

    if (!(flags & AzColorEditFlags_NoOptions))
        ColorPickerOptionsPopup(col, flags);

    if (!(flags & AzColorEditFlags_PickerMask_))
        flags |= ((g.ColorEditOptions & AzColorEditFlags_PickerMask_)
                          ? g.ColorEditOptions
                          : AzColorEditFlags_DefaultOptions_)
                 & AzColorEditFlags_PickerMask_;
    if (!(flags & AzColorEditFlags_InputMask_))
        flags |= ((g.ColorEditOptions & AzColorEditFlags_InputMask_)
                          ? g.ColorEditOptions
                          : AzColorEditFlags_DefaultOptions_)
                 & AzColorEditFlags_InputMask_;
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_PickerMask_));
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_InputMask_));
    if (!(flags & AzColorEditFlags_NoOptions))
        flags |= (g.ColorEditOptions & AzColorEditFlags_AlphaBar);

    int components = (flags & AzColorEditFlags_NoAlpha) ? 3 : 4;
    bool alpha_bar = (flags & AzColorEditFlags_AlphaBar)
                     && !(flags & AzColorEditFlags_NoAlpha);
    AzVec2 picker_pos = window->DC.CursorPos;
    float square_sz = GetFrameHeight();
    float bars_width = square_sz;
    float sv_picker_size =
            AzMax(bars_width * 1,
                  width
                          - (alpha_bar ? 2 : 1)
                                    * (bars_width + style.ItemInnerSpacing.x));
    float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
    float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
    float bars_triangles_half_sz = AZ_TRUNC(bars_width * 0.20f);

    float backup_initial_col[4];
    memcpy(backup_initial_col, col, components * sizeof(float));

    float wheel_thickness = sv_picker_size * 0.08f;
    float wheel_r_outer = sv_picker_size * 0.50f;
    float wheel_r_inner = wheel_r_outer - wheel_thickness;
    AzVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width) * 0.5f,
                        picker_pos.y + sv_picker_size * 0.5f);

    float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
    AzVec2 triangle_pa = AzVec2(triangle_r, 0.0f);
    AzVec2 triangle_pb = AzVec2(triangle_r * -0.5f, triangle_r * -0.866025f);
    AzVec2 triangle_pc = AzVec2(triangle_r * -0.5f, triangle_r * +0.866025f);

    float H = col[0], S = col[1], V = col[2];
    float R = col[0], G = col[1], B = col[2];
    if (flags & AzColorEditFlags_InputRGB)
    {
        ColorConvertRGBtoHSV(R, G, B, H, S, V);
        ColorEditRestoreHS(col, &H, &S, &V);
    } else if (flags & AzColorEditFlags_InputHSV)
    {
        ColorConvertHSVtoRGB(H, S, V, R, G, B);
    }

    bool value_changed = false, value_changed_h = false,
         value_changed_sv = false;

    PushItemFlag(AzItemFlags_NoNav, true);
    if (flags & AzColorEditFlags_PickerHueWheel)
    {
        InvisibleButton("hsv",
                        AzVec2(sv_picker_size + style.ItemInnerSpacing.x
                                       + bars_width,
                               sv_picker_size));
        if (IsItemActive() && !is_readonly)
        {
            AzVec2 initial_off = g.IO.MouseClickedPos[0] - wheel_center;
            AzVec2 current_off = g.IO.MousePos - wheel_center;
            float initial_dist2 = AzLengthSqr(initial_off);
            if (initial_dist2 >= (wheel_r_inner - 1) * (wheel_r_inner - 1)
                && initial_dist2 <= (wheel_r_outer + 1) * (wheel_r_outer + 1))
            {
                H = AzAtan2(current_off.y, current_off.x) / AZ_PI * 0.5f;
                if (H < 0.0f)
                    H += 1.0f;
                value_changed = value_changed_h = true;
            }
            float cos_hue_angle = AzCos(-H * 2.0f * AZ_PI);
            float sin_hue_angle = AzSin(-H * 2.0f * AZ_PI);
            if (AzTriangleContainsPoint(
                        triangle_pa,
                        triangle_pb,
                        triangle_pc,
                        AzRotate(initial_off, cos_hue_angle, sin_hue_angle)))
            {
                AzVec2 current_off_unrotated = AzRotate(current_off,
                                                        cos_hue_angle,
                                                        sin_hue_angle);
                if (!AzTriangleContainsPoint(triangle_pa,
                                             triangle_pb,
                                             triangle_pc,
                                             current_off_unrotated))
                    current_off_unrotated =
                            AzTriangleClosestPoint(triangle_pa,
                                                   triangle_pb,
                                                   triangle_pc,
                                                   current_off_unrotated);
                float uu, vv, ww;
                AzTriangleBarycentricCoords(triangle_pa,
                                            triangle_pb,
                                            triangle_pc,
                                            current_off_unrotated,
                                            uu,
                                            vv,
                                            ww);
                V = AzClamp(1.0f - vv, 0.0001f, 1.0f);
                S = AzClamp(uu / V, 0.0001f, 1.0f);
                value_changed = value_changed_sv = true;
            }
        }
        if (!(flags & AzColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", AzPopupFlags_MouseButtonRight);
    } else if (flags & AzColorEditFlags_PickerHueBar)
    {
        InvisibleButton("sv", AzVec2(sv_picker_size, sv_picker_size));
        if (IsItemActive() && !is_readonly)
        {
            S = AzSaturate((io.MousePos.x - picker_pos.x)
                           / (sv_picker_size - 1));
            V = 1.0f
                - AzSaturate((io.MousePos.y - picker_pos.y)
                             / (sv_picker_size - 1));
            ColorEditRestoreH(col, &H);
            value_changed = value_changed_sv = true;
        }
        if (!(flags & AzColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", AzPopupFlags_MouseButtonRight);

        SetCursorScreenPos(AzVec2(bar0_pos_x, picker_pos.y));
        InvisibleButton("hue", AzVec2(bars_width, sv_picker_size));
        if (IsItemActive() && !is_readonly)
        {
            H = AzSaturate((io.MousePos.y - picker_pos.y)
                           / (sv_picker_size - 1));
            value_changed = value_changed_h = true;
        }
    }

    if (alpha_bar)
    {
        SetCursorScreenPos(AzVec2(bar1_pos_x, picker_pos.y));
        InvisibleButton("alpha", AzVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            col[3] = 1.0f
                     - AzSaturate((io.MousePos.y - picker_pos.y)
                                  / (sv_picker_size - 1));
            value_changed = true;
        }
    }
    PopItemFlag();

    if (!(flags & AzColorEditFlags_NoSidePreview))
    {
        SameLine(0, style.ItemInnerSpacing.x);
        BeginGroup();
    }

    if (!(flags & AzColorEditFlags_NoLabel))
    {
        const char* label_display_end = FindRenderedTextEnd(label);
        if (label != label_display_end)
        {
            if ((flags & AzColorEditFlags_NoSidePreview))
                SameLine(0, style.ItemInnerSpacing.x);
            TextEx(label, label_display_end);
        }
    }

    if (!(flags & AzColorEditFlags_NoSidePreview))
    {
        PushItemFlag(AzItemFlags_NoNavDefaultFocus, true);
        AzVec4 col_v4(col[0],
                      col[1],
                      col[2],
                      (flags & AzColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if ((flags & AzColorEditFlags_NoLabel))
            Text("Current");

        AzColorEditFlags sub_flags_to_forward = AzColorEditFlags_InputMask_
                                                | AzColorEditFlags_HDR
                                                | AzColorEditFlags_AlphaMask_
                                                | AzColorEditFlags_NoTooltip;
        ColorButton("##current",
                    col_v4,
                    (flags & sub_flags_to_forward),
                    AzVec2(square_sz * 3, square_sz * 2));
        if (ref_col != NULL)
        {
            Text("Original");
            AzVec4 ref_col_v4(ref_col[0],
                              ref_col[1],
                              ref_col[2],
                              (flags & AzColorEditFlags_NoAlpha) ? 1.0f
                                                                 : ref_col[3]);
            if (ColorButton("##original",
                            ref_col_v4,
                            (flags & sub_flags_to_forward),
                            AzVec2(square_sz * 3, square_sz * 2)))
            {
                memcpy(col, ref_col, components * sizeof(float));
                value_changed = true;
            }
        }
        PopItemFlag();
        EndGroup();
    }

    if (value_changed_h || value_changed_sv)
    {
        if (flags & AzColorEditFlags_InputRGB)
        {
            ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
            g.ColorEditSavedHue = H;
            g.ColorEditSavedSat = S;
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32(
                    AzVec4(col[0], col[1], col[2], 0));
        } else if (flags & AzColorEditFlags_InputHSV)
        {
            col[0] = H;
            col[1] = S;
            col[2] = V;
        }
    }

    bool value_changed_fix_hue_wrap = false;
    if ((flags & AzColorEditFlags_NoInputs) == 0)
    {
        PushItemWidth((alpha_bar ? bar1_pos_x : bar0_pos_x) + bars_width
                      - picker_pos.x);
        AzColorEditFlags sub_flags_to_forward =
                AzColorEditFlags_DataTypeMask_ | AzColorEditFlags_InputMask_
                | AzColorEditFlags_HDR | AzColorEditFlags_AlphaMask_
                | AzColorEditFlags_NoOptions | AzColorEditFlags_NoTooltip
                | AzColorEditFlags_NoSmallPreview;
        AzColorEditFlags sub_flags = (flags & sub_flags_to_forward)
                                     | AzColorEditFlags_NoPicker;
        if (flags & AzColorEditFlags_DisplayRGB
            || (flags & AzColorEditFlags_DisplayMask_) == 0)
            if (ColorEdit4("##rgb",
                           col,
                           sub_flags | AzColorEditFlags_DisplayRGB))
            {
                value_changed_fix_hue_wrap = (g.ActiveId != 0
                                              && !g.ActiveIdAllowOverlap);
                value_changed = true;
            }
        if (flags & AzColorEditFlags_DisplayHSV
            || (flags & AzColorEditFlags_DisplayMask_) == 0)
            value_changed |= ColorEdit4("##hsv",
                                        col,
                                        sub_flags
                                                | AzColorEditFlags_DisplayHSV);
        if (flags & AzColorEditFlags_DisplayHex
            || (flags & AzColorEditFlags_DisplayMask_) == 0)
            value_changed |= ColorEdit4("##hex",
                                        col,
                                        sub_flags
                                                | AzColorEditFlags_DisplayHex);
        PopItemWidth();
    }

    if (value_changed_fix_hue_wrap && (flags & AzColorEditFlags_InputRGB))
    {
        float new_H, new_S, new_V;
        ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
        if (new_H <= 0 && H > 0)
        {
            if (new_V <= 0 && V != new_V)
                ColorConvertHSVtoRGB(H,
                                     S,
                                     new_V <= 0 ? V * 0.5f : new_V,
                                     col[0],
                                     col[1],
                                     col[2]);
            else if (new_S <= 0)
                ColorConvertHSVtoRGB(H,
                                     new_S <= 0 ? S * 0.5f : new_S,
                                     new_V,
                                     col[0],
                                     col[1],
                                     col[2]);
        }
    }

    if (value_changed)
    {
        if (flags & AzColorEditFlags_InputRGB)
        {
            R = col[0];
            G = col[1];
            B = col[2];
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            ColorEditRestoreHS(col, &H, &S, &V);
        } else if (flags & AzColorEditFlags_InputHSV)
        {
            H = col[0];
            S = col[1];
            V = col[2];
            ColorConvertHSVtoRGB(H, S, V, R, G, B);
        }
    }

    const int style_alpha8 = AZ_F32_TO_INT8_SAT(style.Alpha);
    const AzU32 col_black = AZ_COL32(0, 0, 0, style_alpha8);
    const AzU32 col_white = AZ_COL32(255, 255, 255, style_alpha8);
    const AzU32 col_midgrey = AZ_COL32(128, 128, 128, style_alpha8);
    const AzU32 col_hues[6 + 1] = {AZ_COL32(255, 0, 0, style_alpha8),
                                   AZ_COL32(255, 255, 0, style_alpha8),
                                   AZ_COL32(0, 255, 0, style_alpha8),
                                   AZ_COL32(0, 255, 255, style_alpha8),
                                   AZ_COL32(0, 0, 255, style_alpha8),
                                   AZ_COL32(255, 0, 255, style_alpha8),
                                   AZ_COL32(255, 0, 0, style_alpha8)};

    AzVec4 hue_color_f(1, 1, 1, style.Alpha);
    ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
    AzU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
    AzU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(
            AzVec4(R, G, B, style.Alpha));

    AzVec2 sv_cursor_pos;

    if (flags & AzColorEditFlags_PickerHueWheel)
    {
        const float aeps = 0.5f / wheel_r_outer;
        const int segment_per_arc = AzMax(4, (int)wheel_r_outer / 12);
        for (int n = 0; n < 6; n++)
        {
            const float a0 = (n) / 6.0f * 2.0f * AZ_PI - aeps;
            const float a1 = (n + 1.0f) / 6.0f * 2.0f * AZ_PI + aeps;
            const int vert_start_idx = draw_list->VtxBuffer.Size;
            draw_list->PathArcTo(wheel_center,
                                 (wheel_r_inner + wheel_r_outer) * 0.5f,
                                 a0,
                                 a1,
                                 segment_per_arc);
            draw_list->PathStroke(col_white, 0, wheel_thickness);
            const int vert_end_idx = draw_list->VtxBuffer.Size;

            AzVec2 gradient_p0(wheel_center.x + AzCos(a0) * wheel_r_inner,
                               wheel_center.y + AzSin(a0) * wheel_r_inner);
            AzVec2 gradient_p1(wheel_center.x + AzCos(a1) * wheel_r_inner,
                               wheel_center.y + AzSin(a1) * wheel_r_inner);
            ShadeVertsLinearColorGradientKeepAlpha(draw_list,
                                                   vert_start_idx,
                                                   vert_end_idx,
                                                   gradient_p0,
                                                   gradient_p1,
                                                   col_hues[n],
                                                   col_hues[n + 1]);
        }

        float cos_hue_angle = AzCos(H * 2.0f * AZ_PI);
        float sin_hue_angle = AzSin(H * 2.0f * AZ_PI);
        AzVec2 hue_cursor_pos(
                wheel_center.x
                        + cos_hue_angle * (wheel_r_inner + wheel_r_outer)
                                  * 0.5f,
                wheel_center.y
                        + sin_hue_angle * (wheel_r_inner + wheel_r_outer)
                                  * 0.5f);
        float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f
                                               : wheel_thickness * 0.55f;
        int hue_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(
                hue_cursor_rad);
        draw_list->AddCircleFilled(hue_cursor_pos,
                                   hue_cursor_rad,
                                   hue_color32,
                                   hue_cursor_segments);
        draw_list->AddCircle(hue_cursor_pos,
                             hue_cursor_rad + 1,
                             col_midgrey,
                             hue_cursor_segments);
        draw_list->AddCircle(hue_cursor_pos,
                             hue_cursor_rad,
                             col_white,
                             hue_cursor_segments);

        AzVec2 tra = wheel_center
                     + AzRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
        AzVec2 trb = wheel_center
                     + AzRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
        AzVec2 trc = wheel_center
                     + AzRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
        AzVec2 uv_white = GetFontTexUvWhitePixel();
        draw_list->PrimReserve(3, 3);
        draw_list->PrimVtx(tra, uv_white, hue_color32);
        draw_list->PrimVtx(trb, uv_white, col_black);
        draw_list->PrimVtx(trc, uv_white, col_white);
        draw_list->AddTriangle(tra, trb, trc, col_midgrey, 1.5f);
        sv_cursor_pos = AzLerp(AzLerp(trc, tra, AzSaturate(S)),
                               trb,
                               AzSaturate(1 - V));
    } else if (flags & AzColorEditFlags_PickerHueBar)
    {
        draw_list->AddRectFilledMultiColor(picker_pos,
                                           picker_pos
                                                   + AzVec2(sv_picker_size,
                                                            sv_picker_size),
                                           col_white,
                                           hue_color32,
                                           hue_color32,
                                           col_white);
        draw_list->AddRectFilledMultiColor(picker_pos,
                                           picker_pos
                                                   + AzVec2(sv_picker_size,
                                                            sv_picker_size),
                                           0,
                                           0,
                                           col_black,
                                           col_black);
        RenderFrameBorder(picker_pos,
                          picker_pos + AzVec2(sv_picker_size, sv_picker_size),
                          0.0f);
        sv_cursor_pos.x = AzClamp(AZ_ROUND(picker_pos.x
                                           + AzSaturate(S) * sv_picker_size),
                                  picker_pos.x + 2,
                                  picker_pos.x + sv_picker_size - 2);
        sv_cursor_pos.y = AzClamp(
                AZ_ROUND(picker_pos.y + AzSaturate(1 - V) * sv_picker_size),
                picker_pos.y + 2,
                picker_pos.y + sv_picker_size - 2);

        for (int i = 0; i < 6; ++i)
            draw_list->AddRectFilledMultiColor(
                    AzVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)),
                    AzVec2(bar0_pos_x + bars_width,
                           picker_pos.y + (i + 1) * (sv_picker_size / 6)),
                    col_hues[i],
                    col_hues[i],
                    col_hues[i + 1],
                    col_hues[i + 1]);
        float bar0_line_y = AZ_ROUND(picker_pos.y + H * sv_picker_size);
        RenderFrameBorder(AzVec2(bar0_pos_x, picker_pos.y),
                          AzVec2(bar0_pos_x + bars_width,
                                 picker_pos.y + sv_picker_size),
                          0.0f);
        RenderArrowsForVerticalBar(draw_list,
                                   AzVec2(bar0_pos_x - 1, bar0_line_y),
                                   AzVec2(bars_triangles_half_sz + 1,
                                          bars_triangles_half_sz),
                                   bars_width + 2.0f,
                                   style.Alpha);
    }

    float sv_cursor_rad = value_changed_sv ? wheel_thickness * 0.55f
                                           : wheel_thickness * 0.40f;
    int sv_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(
            sv_cursor_rad);
    draw_list->AddCircleFilled(sv_cursor_pos,
                               sv_cursor_rad,
                               user_col32_striped_of_alpha,
                               sv_cursor_segments);
    draw_list->AddCircle(sv_cursor_pos,
                         sv_cursor_rad + 1,
                         col_midgrey,
                         sv_cursor_segments);
    draw_list->AddCircle(sv_cursor_pos,
                         sv_cursor_rad,
                         col_white,
                         sv_cursor_segments);

    if (alpha_bar)
    {
        float alpha = AzSaturate(col[3]);
        AzRect bar1_bb(bar1_pos_x,
                       picker_pos.y,
                       bar1_pos_x + bars_width,
                       picker_pos.y + sv_picker_size);
        RenderColorRectWithAlphaCheckerboard(draw_list,
                                             bar1_bb.Min,
                                             bar1_bb.Max,
                                             0,
                                             bar1_bb.GetWidth() / 2.0f,
                                             AzVec2(0.0f, 0.0f));
        draw_list->AddRectFilledMultiColor(bar1_bb.Min,
                                           bar1_bb.Max,
                                           user_col32_striped_of_alpha,
                                           user_col32_striped_of_alpha,
                                           user_col32_striped_of_alpha
                                                   & ~AZ_COL32_A_MASK,
                                           user_col32_striped_of_alpha
                                                   & ~AZ_COL32_A_MASK);
        float bar1_line_y = AZ_ROUND(picker_pos.y
                                     + (1.0f - alpha) * sv_picker_size);
        RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
        RenderArrowsForVerticalBar(draw_list,
                                   AzVec2(bar1_pos_x - 1, bar1_line_y),
                                   AzVec2(bars_triangles_half_sz + 1,
                                          bars_triangles_half_sz),
                                   bars_width + 2.0f,
                                   style.Alpha);
    }

    EndGroup();

    if (value_changed
        && memcmp(backup_initial_col, col, components * sizeof(float)) == 0)
        value_changed = false;
    if (value_changed && g.LastItemData.ID != 0)
        MarkItemEdited(g.LastItemData.ID);

    if (set_current_color_edit_id)
        g.ColorEditCurrentID = 0;
    PopID();

    return value_changed;
}

bool Az::ColorButton(const char* desc_id,
                     const AzVec4& col,
                     AzColorEditFlags flags,
                     const AzVec2& size_arg)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzID id = window->GetID(desc_id);
    const float default_size = GetFrameHeight();
    const AzVec2 size(size_arg.x == 0.0f ? default_size : size_arg.x,
                      size_arg.y == 0.0f ? default_size : size_arg.y);
    const AzRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb, (size.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    if (flags & (AzColorEditFlags_NoAlpha | AzColorEditFlags_AlphaOpaque))
        flags &= ~(AzColorEditFlags_AlphaNoBg
                   | AzColorEditFlags_AlphaPreviewHalf);

    AzVec4 col_rgb = col;
    if (flags & AzColorEditFlags_InputHSV)
        ColorConvertHSVtoRGB(col_rgb.x,
                             col_rgb.y,
                             col_rgb.z,
                             col_rgb.x,
                             col_rgb.y,
                             col_rgb.z);

    AzVec4 col_rgb_without_alpha(col_rgb.x, col_rgb.y, col_rgb.z, 1.0f);
    float grid_step = AzMin(size.x, size.y) / 2.99f;
    float rounding = AzMin(g.Style.FrameRounding, grid_step * 0.5f);
    AzRect bb_inner = bb;
    float off = 0.0f;
    if ((flags & AzColorEditFlags_NoBorder) == 0)
    {
        off = -0.75f;
        bb_inner.Expand(off);
    }
    if ((flags & AzColorEditFlags_AlphaPreviewHalf) && col_rgb.w < 1.0f)
    {
        float mid_x = AZ_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
        if ((flags & AzColorEditFlags_AlphaNoBg) == 0)
            RenderColorRectWithAlphaCheckerboard(
                    window->DrawList,
                    AzVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y),
                    bb_inner.Max,
                    GetColorU32(col_rgb),
                    grid_step,
                    AzVec2(-grid_step + off, off),
                    rounding,
                    AzDrawFlags_RoundCornersRight);
        else
            window->DrawList->AddRectFilled(AzVec2(bb_inner.Min.x + grid_step,
                                                   bb_inner.Min.y),
                                            bb_inner.Max,
                                            GetColorU32(col_rgb),
                                            rounding,
                                            AzDrawFlags_RoundCornersRight);
        window->DrawList->AddRectFilled(bb_inner.Min,
                                        AzVec2(mid_x, bb_inner.Max.y),
                                        GetColorU32(col_rgb_without_alpha),
                                        rounding,
                                        AzDrawFlags_RoundCornersLeft);
    } else
    {
        AzVec4 col_source = (flags & AzColorEditFlags_AlphaOpaque)
                                    ? col_rgb_without_alpha
                                    : col_rgb;
        if (col_source.w < 1.0f && (flags & AzColorEditFlags_AlphaNoBg) == 0)
            RenderColorRectWithAlphaCheckerboard(window->DrawList,
                                                 bb_inner.Min,
                                                 bb_inner.Max,
                                                 GetColorU32(col_source),
                                                 grid_step,
                                                 AzVec2(off, off),
                                                 rounding);
        else
            window->DrawList->AddRectFilled(bb_inner.Min,
                                            bb_inner.Max,
                                            GetColorU32(col_source),
                                            rounding);
    }
    RenderNavCursor(bb, id);
    if ((flags & AzColorEditFlags_NoBorder) == 0)
    {
        if (g.Style.FrameBorderSize > 0.0f)
            RenderFrameBorder(bb.Min, bb.Max, rounding);
        else
            window->DrawList->AddRect(bb.Min,
                                      bb.Max,
                                      GetColorU32(AzCol_FrameBg),
                                      rounding);
    }

    if (g.ActiveId == id && !(flags & AzColorEditFlags_NoDragDrop)
        && BeginDragDropSource())
    {
        if (flags & AzColorEditFlags_NoAlpha)
            SetDragDropPayload(AZ_PAYLOAD_TYPE_COLOR_3F,
                               &col_rgb,
                               sizeof(float) * 3,
                               AzCond_Once);
        else
            SetDragDropPayload(AZ_PAYLOAD_TYPE_COLOR_4F,
                               &col_rgb,
                               sizeof(float) * 4,
                               AzCond_Once);
        ColorButton(desc_id, col, flags);
        SameLine();
        TextEx("Color");
        EndDragDropSource();
    }

    if (!(flags & AzColorEditFlags_NoTooltip) && hovered
        && IsItemHovered(AzHoveredFlags_ForTooltip))
        ColorTooltip(desc_id,
                     &col.x,
                     flags
                             & (AzColorEditFlags_InputMask_
                                | AzColorEditFlags_AlphaMask_));

    return pressed;
}

void Az::SetColorEditOptions(AzColorEditFlags flags)
{
    AzContext& g = *GAz;
    if ((flags & AzColorEditFlags_DisplayMask_) == 0)
        flags |= AzColorEditFlags_DefaultOptions_
                 & AzColorEditFlags_DisplayMask_;
    if ((flags & AzColorEditFlags_DataTypeMask_) == 0)
        flags |= AzColorEditFlags_DefaultOptions_
                 & AzColorEditFlags_DataTypeMask_;
    if ((flags & AzColorEditFlags_PickerMask_) == 0)
        flags |= AzColorEditFlags_DefaultOptions_
                 & AzColorEditFlags_PickerMask_;
    if ((flags & AzColorEditFlags_InputMask_) == 0)
        flags |= AzColorEditFlags_DefaultOptions_ & AzColorEditFlags_InputMask_;
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_DisplayMask_));
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_DataTypeMask_));
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_PickerMask_));
    AZ_ASSERT(AzIsPowerOfTwo(flags & AzColorEditFlags_InputMask_));
    g.ColorEditOptions = flags;
}

void Az::ColorTooltip(const char* text,
                      const float* col,
                      AzColorEditFlags flags)
{
    AzContext& g = *GAz;

    if (!BeginTooltipEx(AzTooltipFlags_OverridePrevious, AzWindowFlags_None))
        return;
    const char* text_end = text ? FindRenderedTextEnd(text, NULL) : text;
    if (text_end > text)
    {
        TextEx(text, text_end);
        Separator();
    }

    AzVec2 sz(g.FontSize * 3 + g.Style.FramePadding.y * 2,
              g.FontSize * 3 + g.Style.FramePadding.y * 2);
    AzVec4 cf(col[0],
              col[1],
              col[2],
              (flags & AzColorEditFlags_NoAlpha) ? 1.0f : col[3]);
    int cr = AZ_F32_TO_INT8_SAT(col[0]), cg = AZ_F32_TO_INT8_SAT(col[1]),
        cb = AZ_F32_TO_INT8_SAT(col[2]),
        ca = (flags & AzColorEditFlags_NoAlpha) ? 255
                                                : AZ_F32_TO_INT8_SAT(col[3]);
    AzColorEditFlags flags_to_forward = AzColorEditFlags_InputMask_
                                        | AzColorEditFlags_AlphaMask_;
    ColorButton("##preview",
                cf,
                (flags & flags_to_forward) | AzColorEditFlags_NoTooltip,
                sz);
    SameLine();
    if ((flags & AzColorEditFlags_InputRGB)
        || !(flags & AzColorEditFlags_InputMask_))
    {
        if (flags & AzColorEditFlags_NoAlpha)
            Text("#%02X%02X%02X\nR: %d, G: %d, B: %d\n(%.3f, %.3f, %.3f)",
                 cr,
                 cg,
                 cb,
                 cr,
                 cg,
                 cb,
                 col[0],
                 col[1],
                 col[2]);
        else
            Text("#%02X%02X%02X%02X\nR:%d, G:%d, B:%d, A:%d\n(%.3f, %.3f, "
                 "%.3f, %.3f)",
                 cr,
                 cg,
                 cb,
                 ca,
                 cr,
                 cg,
                 cb,
                 ca,
                 col[0],
                 col[1],
                 col[2],
                 col[3]);
    } else if (flags & AzColorEditFlags_InputHSV)
    {
        if (flags & AzColorEditFlags_NoAlpha)
            Text("H: %.3f, S: %.3f, V: %.3f", col[0], col[1], col[2]);
        else
            Text("H: %.3f, S: %.3f, V: %.3f, A: %.3f",
                 col[0],
                 col[1],
                 col[2],
                 col[3]);
    }
    EndTooltip();
}

void Az::ColorEditOptionsPopup(const float* col, AzColorEditFlags flags)
{
    bool allow_opt_inputs = !(flags & AzColorEditFlags_DisplayMask_);
    bool allow_opt_datatype = !(flags & AzColorEditFlags_DataTypeMask_);
    if ((!allow_opt_inputs && !allow_opt_datatype) || !BeginPopup("context"))
        return;

    AzContext& g = *GAz;
    PushItemFlag(AzItemFlags_NoMarkEdited, true);
    AzColorEditFlags opts = g.ColorEditOptions;
    if (allow_opt_inputs)
    {
        if (RadioButton("RGB", (opts & AzColorEditFlags_DisplayRGB) != 0))
            opts = (opts & ~AzColorEditFlags_DisplayMask_)
                   | AzColorEditFlags_DisplayRGB;
        if (RadioButton("HSV", (opts & AzColorEditFlags_DisplayHSV) != 0))
            opts = (opts & ~AzColorEditFlags_DisplayMask_)
                   | AzColorEditFlags_DisplayHSV;
        if (RadioButton("Hex", (opts & AzColorEditFlags_DisplayHex) != 0))
            opts = (opts & ~AzColorEditFlags_DisplayMask_)
                   | AzColorEditFlags_DisplayHex;
    }
    if (allow_opt_datatype)
    {
        if (allow_opt_inputs)
            Separator();
        if (RadioButton("0..255", (opts & AzColorEditFlags_Uint8) != 0))
            opts = (opts & ~AzColorEditFlags_DataTypeMask_)
                   | AzColorEditFlags_Uint8;
        if (RadioButton("0.00..1.00", (opts & AzColorEditFlags_Float) != 0))
            opts = (opts & ~AzColorEditFlags_DataTypeMask_)
                   | AzColorEditFlags_Float;
    }

    if (allow_opt_inputs || allow_opt_datatype)
        Separator();
    if (Button("Copy as..", AzVec2(-1, 0)))
        OpenPopup("Copy");
    if (BeginPopup("Copy"))
    {
        int cr = AZ_F32_TO_INT8_SAT(col[0]), cg = AZ_F32_TO_INT8_SAT(col[1]),
            cb = AZ_F32_TO_INT8_SAT(col[2]),
            ca = (flags & AzColorEditFlags_NoAlpha)
                         ? 255
                         : AZ_F32_TO_INT8_SAT(col[3]);
        char buf[64];
        AzFormatString(buf,
                       AZ_ARRAYSIZE(buf),
                       "(%.3ff, %.3ff, %.3ff, %.3ff)",
                       col[0],
                       col[1],
                       col[2],
                       (flags & AzColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if (Selectable(buf))
            SetClipboardText(buf);
        AzFormatString(buf, AZ_ARRAYSIZE(buf), "(%d,%d,%d,%d)", cr, cg, cb, ca);
        if (Selectable(buf))
            SetClipboardText(buf);
        AzFormatString(buf, AZ_ARRAYSIZE(buf), "#%02X%02X%02X", cr, cg, cb);
        if (Selectable(buf))
            SetClipboardText(buf);
        if (!(flags & AzColorEditFlags_NoAlpha))
        {
            AzFormatString(buf,
                           AZ_ARRAYSIZE(buf),
                           "#%02X%02X%02X%02X",
                           cr,
                           cg,
                           cb,
                           ca);
            if (Selectable(buf))
                SetClipboardText(buf);
        }
        EndPopup();
    }

    g.ColorEditOptions = opts;
    PopItemFlag();
    EndPopup();
}

void Az::ColorPickerOptionsPopup(const float* ref_col, AzColorEditFlags flags)
{
    bool allow_opt_picker = !(flags & AzColorEditFlags_PickerMask_);
    bool allow_opt_alpha_bar = !(flags & AzColorEditFlags_NoAlpha)
                               && !(flags & AzColorEditFlags_AlphaBar);
    if ((!allow_opt_picker && !allow_opt_alpha_bar) || !BeginPopup("context"))
        return;

    AzContext& g = *GAz;
    PushItemFlag(AzItemFlags_NoMarkEdited, true);
    if (allow_opt_picker)
    {
        AzVec2 picker_size(g.FontSize * 8,
                           AzMax(g.FontSize * 8
                                         - (GetFrameHeight()
                                            + g.Style.ItemInnerSpacing.x),
                                 1.0f));
        PushItemWidth(picker_size.x);
        for (int picker_type = 0; picker_type < 2; picker_type++)
        {
            if (picker_type > 0)
                Separator();
            PushID(picker_type);
            AzColorEditFlags picker_flags = AzColorEditFlags_NoInputs
                                            | AzColorEditFlags_NoOptions
                                            | AzColorEditFlags_NoLabel
                                            | AzColorEditFlags_NoSidePreview
                                            | (flags
                                               & AzColorEditFlags_NoAlpha);
            if (picker_type == 0)
                picker_flags |= AzColorEditFlags_PickerHueBar;
            if (picker_type == 1)
                picker_flags |= AzColorEditFlags_PickerHueWheel;
            AzVec2 backup_pos = GetCursorScreenPos();
            if (Selectable("##selectable", false, 0, picker_size))
                g.ColorEditOptions = (g.ColorEditOptions
                                      & ~AzColorEditFlags_PickerMask_)
                                     | (picker_flags
                                        & AzColorEditFlags_PickerMask_);
            SetCursorScreenPos(backup_pos);
            AzVec4 previewing_ref_col;
            memcpy(&previewing_ref_col,
                   ref_col,
                   sizeof(float)
                           * ((picker_flags & AzColorEditFlags_NoAlpha) ? 3
                                                                        : 4));
            ColorPicker4("##previewing_picker",
                         &previewing_ref_col.x,
                         picker_flags);
            PopID();
        }
        PopItemWidth();
    }
    if (allow_opt_alpha_bar)
    {
        if (allow_opt_picker)
            Separator();
        CheckboxFlags("Alpha Bar",
                      &g.ColorEditOptions,
                      AzColorEditFlags_AlphaBar);
    }
    PopItemFlag();
    EndPopup();
}

bool Az::TreeNode(const char* str_id, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(str_id, 0, fmt, args);
    va_end(args);
    return is_open;
}

bool Az::TreeNode(const void* ptr_id, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(ptr_id, 0, fmt, args);
    va_end(args);
    return is_open;
}

bool Az::TreeNode(const char* label)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    AzID id = window->GetID(label);
    return TreeNodeBehavior(id, AzTreeNodeFlags_None, label, NULL);
}

bool Az::TreeNodeV(const char* str_id, const char* fmt, va_list args)
{
    return TreeNodeExV(str_id, 0, fmt, args);
}

bool Az::TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
{
    return TreeNodeExV(ptr_id, 0, fmt, args);
}

bool Az::TreeNodeEx(const char* label, AzTreeNodeFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    AzID id = window->GetID(label);
    return TreeNodeBehavior(id, flags, label, NULL);
}

bool Az::TreeNodeEx(const char* str_id,
                    AzTreeNodeFlags flags,
                    const char* fmt,
                    ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(str_id, flags, fmt, args);
    va_end(args);
    return is_open;
}

bool Az::TreeNodeEx(const void* ptr_id,
                    AzTreeNodeFlags flags,
                    const char* fmt,
                    ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(ptr_id, flags, fmt, args);
    va_end(args);
    return is_open;
}

bool Az::TreeNodeExV(const char* str_id,
                     AzTreeNodeFlags flags,
                     const char* fmt,
                     va_list args)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzID id = window->GetID(str_id);
    const char *label, *label_end;
    AzFormatStringToTempBufferV(&label, &label_end, fmt, args);
    return TreeNodeBehavior(id, flags, label, label_end);
}

bool Az::TreeNodeExV(const void* ptr_id,
                     AzTreeNodeFlags flags,
                     const char* fmt,
                     va_list args)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzID id = window->GetID(ptr_id);
    const char *label, *label_end;
    AzFormatStringToTempBufferV(&label, &label_end, fmt, args);
    return TreeNodeBehavior(id, flags, label, label_end);
}

bool Az::TreeNodeGetOpen(AzID storage_id)
{
    AzContext& g = *GAz;
    AzStorage* storage = g.CurrentWindow->DC.StateStorage;
    return storage->GetInt(storage_id, 0) != 0;
}

void Az::TreeNodeSetOpen(AzID storage_id, bool open)
{
    AzContext& g = *GAz;
    AzStorage* storage = g.CurrentWindow->DC.StateStorage;
    storage->SetInt(storage_id, open ? 1 : 0);
}

bool Az::TreeNodeUpdateNextOpen(AzID storage_id, AzTreeNodeFlags flags)
{
    if (flags & AzTreeNodeFlags_Leaf)
        return true;

    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzStorage* storage = window->DC.StateStorage;

    bool is_open;
    if (g.NextItemData.HasFlags & AzNextItemDataFlags_HasOpen)
    {
        if (g.NextItemData.OpenCond & AzCond_Always)
        {
            is_open = g.NextItemData.OpenVal;
            TreeNodeSetOpen(storage_id, is_open);
        } else
        {
            const int stored_value = storage->GetInt(storage_id, -1);
            if (stored_value == -1)
            {
                is_open = g.NextItemData.OpenVal;
                TreeNodeSetOpen(storage_id, is_open);
            } else
            {
                is_open = stored_value != 0;
            }
        }
    } else
    {
        is_open = storage->GetInt(storage_id,
                                  (flags & AzTreeNodeFlags_DefaultOpen) ? 1 : 0)
                  != 0;
    }

    if (g.LogEnabled && !(flags & AzTreeNodeFlags_NoAutoOpenOnLog)
        && (window->DC.TreeDepth - g.LogDepthRef) < g.LogDepthToExpand)
        is_open = true;

    return is_open;
}

static void TreeNodeStoreStackData(AzTreeNodeFlags flags, float x1)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    g.TreeNodeStack.resize(g.TreeNodeStack.Size + 1);
    AzTreeNodeStackData* tree_node_data =
            &g.TreeNodeStack.Data[g.TreeNodeStack.Size - 1];
    tree_node_data->ID = g.LastItemData.ID;
    tree_node_data->TreeFlags = flags;
    tree_node_data->ItemFlags = g.LastItemData.ItemFlags;
    tree_node_data->NavRect = g.LastItemData.NavRect;

    const bool draw_lines = (flags
                             & (AzTreeNodeFlags_DrawLinesFull
                                | AzTreeNodeFlags_DrawLinesToNodes))
                            != 0;
    tree_node_data->DrawLinesX1 = draw_lines ? (x1 + g.FontSize * 0.5f
                                                + g.Style.FramePadding.x)
                                             : +FLT_MAX;
    tree_node_data->DrawLinesTableColumn =
            (draw_lines && g.CurrentTable)
                    ? (AzTableColumnIdx)g.CurrentTable->CurrentColumn
                    : -1;
    tree_node_data->DrawLinesToNodesY2 = -FLT_MAX;
    window->DC.TreeHasStackDataDepthMask |= (1 << window->DC.TreeDepth);
    if (flags & AzTreeNodeFlags_DrawLinesToNodes)
        window->DC.TreeRecordsClippedNodesY2Mask |= (1 << window->DC.TreeDepth);
}

bool Az::TreeNodeBehavior(AzID id,
                          AzTreeNodeFlags flags,
                          const char* label,
                          const char* label_end)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const bool display_frame = (flags & AzTreeNodeFlags_Framed) != 0;
    const AzVec2 padding =
            (display_frame || (flags & AzTreeNodeFlags_FramePadding))
                    ? style.FramePadding
                    : AzVec2(style.FramePadding.x,
                             AzMin(window->DC.CurrLineTextBaseOffset,
                                   style.FramePadding.y));

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const AzVec2 label_size = CalcTextSize(label, label_end, false);

    const float text_offset_x = g.FontSize
                                + (display_frame ? padding.x * 3
                                                 : padding.x * 2);
    const float text_offset_y = AzMax(padding.y,
                                      window->DC.CurrLineTextBaseOffset);
    const float text_width = g.FontSize + label_size.x + padding.x * 2;

    const float frame_height = AzMax(AzMin(window->DC.CurrLineSize.y,
                                           g.FontSize
                                                   + style.FramePadding.y * 2),
                                     label_size.y + padding.y * 2);
    const bool span_all_columns = (flags & AzTreeNodeFlags_SpanAllColumns) != 0
                                  && (g.CurrentTable != NULL);
    const bool span_all_columns_label = (flags
                                         & AzTreeNodeFlags_LabelSpanAllColumns)
                                                != 0
                                        && (g.CurrentTable != NULL);
    AzRect frame_bb;
    frame_bb.Min.x = span_all_columns ? window->ParentWorkRect.Min.x
                     : (flags & AzTreeNodeFlags_SpanFullWidth)
                             ? window->WorkRect.Min.x
                             : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y;
    frame_bb.Max.x = span_all_columns ? window->ParentWorkRect.Max.x
                     : (flags & AzTreeNodeFlags_SpanLabelWidth)
                             ? window->DC.CursorPos.x + text_width + padding.x
                             : window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
    if (display_frame)
    {
        const float outer_extend = AZ_TRUNC(window->WindowPadding.x * 0.5f);
        frame_bb.Min.x -= outer_extend;
        frame_bb.Max.x += outer_extend;
    }

    AzVec2 text_pos(window->DC.CursorPos.x + text_offset_x,
                    window->DC.CursorPos.y + text_offset_y);
    ItemSize(AzVec2(text_width, frame_height), padding.y);

    AzRect interact_bb = frame_bb;
    if ((flags
         & (AzTreeNodeFlags_Framed | AzTreeNodeFlags_SpanAvailWidth
            | AzTreeNodeFlags_SpanFullWidth | AzTreeNodeFlags_SpanLabelWidth
            | AzTreeNodeFlags_SpanAllColumns))
        == 0)
        interact_bb.Max.x = frame_bb.Min.x + text_width
                            + (label_size.x > 0.0f ? style.ItemSpacing.x * 2.0f
                                                   : 0.0f);

    AzID storage_id = (g.NextItemData.HasFlags
                       & AzNextItemDataFlags_HasStorageID)
                              ? g.NextItemData.StorageId
                              : id;
    bool is_open = TreeNodeUpdateNextOpen(storage_id, flags);

    bool is_visible;
    if (span_all_columns || span_all_columns_label)
    {
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd(interact_bb, id);
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    } else
    {
        is_visible = ItemAdd(interact_bb, id);
    }
    g.LastItemData.StatusFlags |= AzItemStatusFlags_HasDisplayRect;
    g.LastItemData.DisplayRect = frame_bb;

    bool store_tree_node_stack_data = false;
    if ((flags & AzTreeNodeFlags_DrawLinesMask_) == 0)
        flags |= g.Style.TreeLinesFlags;
    const bool draw_tree_lines = (flags
                                  & (AzTreeNodeFlags_DrawLinesFull
                                     | AzTreeNodeFlags_DrawLinesToNodes))
                                 && (frame_bb.Min.y < window->ClipRect.Max.y)
                                 && (g.Style.TreeLinesSize > 0.0f);
    if (!(flags & AzTreeNodeFlags_NoTreePushOnOpen))
    {
        store_tree_node_stack_data = draw_tree_lines;
        if ((flags & AzTreeNodeFlags_NavLeftJumpsToParent) && !g.NavIdIsAlive)
            if (g.NavMoveDir == AzDir_Left && g.NavWindow == window
                && NavMoveRequestButNoResultYet())
                store_tree_node_stack_data = true;
    }

    const bool is_leaf = (flags & AzTreeNodeFlags_Leaf) != 0;
    if (!is_visible)
    {
        if ((flags & AzTreeNodeFlags_DrawLinesToNodes)
            && (window->DC.TreeRecordsClippedNodesY2Mask
                & (1 << (window->DC.TreeDepth - 1))))
        {
            AzTreeNodeStackData* parent_data =
                    &g.TreeNodeStack.Data[g.TreeNodeStack.Size - 1];
            parent_data->DrawLinesToNodesY2 =
                    AzMax(parent_data->DrawLinesToNodesY2,
                          window->DC.CursorPos.y);
            if (frame_bb.Min.y >= window->ClipRect.Max.y)
                window->DC.TreeRecordsClippedNodesY2Mask &= ~(
                        1 << (window->DC.TreeDepth - 1));
        }
        if (is_open && store_tree_node_stack_data)
            TreeNodeStoreStackData(flags, text_pos.x - text_offset_x);
        if (is_open && !(flags & AzTreeNodeFlags_NoTreePushOnOpen))
            TreePushOverrideID(id);
        AZ_TEST_ENGINE_ITEM_INFO(
                g.LastItemData.ID,
                label,
                g.LastItemData.StatusFlags
                        | (is_leaf ? 0 : AzItemStatusFlags_Openable)
                        | (is_open ? AzItemStatusFlags_Opened : 0));
        return is_open;
    }

    if (span_all_columns || span_all_columns_label)
    {
        TablePushBackgroundChannel();
        g.LastItemData.StatusFlags |= AzItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    AzButtonFlags button_flags = AzTreeNodeFlags_None;
    if ((flags & AzTreeNodeFlags_AllowOverlap)
        || (g.LastItemData.ItemFlags & AzItemFlags_AllowOverlap))
        button_flags |= AzButtonFlags_AllowOverlap;
    if (!is_leaf)
        button_flags |= AzButtonFlags_PressedOnDragDropHold;

    const float arrow_hit_x1 = (text_pos.x - text_offset_x)
                               - style.TouchExtraPadding.x;
    const float arrow_hit_x2 = (text_pos.x - text_offset_x)
                               + (g.FontSize + padding.x * 2.0f)
                               + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1
                                        && g.IO.MousePos.x < arrow_hit_x2);

    const bool is_multi_select = (g.LastItemData.ItemFlags
                                  & AzItemFlags_IsMultiSelect)
                                 != 0;
    if (is_multi_select)
        flags |= (flags & AzTreeNodeFlags_OpenOnMask_) == 0
                         ? AzTreeNodeFlags_OpenOnArrow
                                   | AzTreeNodeFlags_OpenOnDoubleClick
                         : AzTreeNodeFlags_OpenOnArrow;

    if (is_mouse_x_over_arrow)
        button_flags |= AzButtonFlags_PressedOnClick;
    else if (flags & AzTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= AzButtonFlags_PressedOnClickRelease
                        | AzButtonFlags_PressedOnDoubleClick;
    else
        button_flags |= AzButtonFlags_PressedOnClickRelease;
    if (flags & AzTreeNodeFlags_NoNavFocus)
        button_flags |= AzButtonFlags_NoNavFocus;

    bool selected = (flags & AzTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;

    if (is_multi_select)
    {
        MultiSelectItemHeader(id, &selected, &button_flags);
        if (is_mouse_x_over_arrow)
            button_flags = (button_flags | AzButtonFlags_PressedOnClick)
                           & ~AzButtonFlags_PressedOnClickRelease;
    } else
    {
        if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
            button_flags |= AzButtonFlags_NoKeyModsAllowed;
    }

    bool hovered, held;
    bool pressed =
            ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    bool toggled = false;
    if (!is_leaf)
    {
        if (pressed && g.DragDropHoldJustPressedId != id)
        {
            if ((flags & AzTreeNodeFlags_OpenOnMask_) == 0
                || (g.NavActivateId == id && !is_multi_select))
                toggled = true;
            if (flags & AzTreeNodeFlags_OpenOnArrow)
                toggled |= is_mouse_x_over_arrow && !g.NavHighlightItemUnderNav;
            if ((flags & AzTreeNodeFlags_OpenOnDoubleClick)
                && g.IO.MouseClickedCount[0] == 2)
                toggled = true;
        } else if (pressed && g.DragDropHoldJustPressedId == id)
        {
            AZ_ASSERT(button_flags & AzButtonFlags_PressedOnDragDropHold);
            if (!is_open)
                toggled = true;
            else
                pressed = false;
        }

        if (g.NavId == id && g.NavMoveDir == AzDir_Left && is_open)
        {
            toggled = true;
            NavClearPreferredPosForAxis(AzAxis_X);
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveDir == AzDir_Right && !is_open)
        {
            toggled = true;
            NavClearPreferredPosForAxis(AzAxis_X);
            NavMoveRequestCancel();
        }

        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(storage_id, is_open);
            g.LastItemData.StatusFlags |= AzItemStatusFlags_ToggledOpen;
        }
    }

    if (is_multi_select)
    {
        bool pressed_copy = pressed && !toggled;
        MultiSelectItemFooter(id, &selected, &pressed_copy);
        if (pressed)
            SetNavID(id,
                     window->DC.NavLayerCurrent,
                     g.CurrentFocusScopeId,
                     interact_bb);
    }

    if (selected != was_selected)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_ToggledSelection;

    {
        const AzU32 text_col = GetColorU32(AzCol_Text);
        AzNavRenderCursorFlags nav_render_cursor_flags =
                AzNavRenderCursorFlags_Compact;
        if (is_multi_select)
            nav_render_cursor_flags |= AzNavRenderCursorFlags_AlwaysDraw;
        if (display_frame)
        {
            const AzU32 bg_col = GetColorU32((held && hovered)
                                                     ? AzCol_HeaderActive
                                             : hovered ? AzCol_HeaderHovered
                                                       : AzCol_Header);
            RenderFrame(frame_bb.Min,
                        frame_bb.Max,
                        bg_col,
                        true,
                        style.FrameRounding);
            RenderNavCursor(frame_bb, id, nav_render_cursor_flags);
            if (span_all_columns && !span_all_columns_label)
                TablePopBackgroundChannel();
            if (flags & AzTreeNodeFlags_Bullet)
                RenderBullet(window->DrawList,
                             AzVec2(text_pos.x - text_offset_x * 0.60f,
                                    text_pos.y + g.FontSize * 0.5f),
                             text_col);
            else if (!is_leaf)
                RenderArrow(window->DrawList,
                            AzVec2(text_pos.x - text_offset_x + padding.x,
                                   text_pos.y),
                            text_col,
                            is_open ? ((flags & AzTreeNodeFlags_UpsideDownArrow)
                                               ? AzDir_Up
                                               : AzDir_Down)
                                    : AzDir_Right,
                            1.0f);
            else
                text_pos.x -= text_offset_x - padding.x;
            if (flags & AzTreeNodeFlags_ClipLabelForTrailingButton)
                frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
            if (g.LogEnabled)
                LogSetNextTextDecoration("###", "###");
        } else
        {
            if (hovered || selected)
            {
                const AzU32 bg_col = GetColorU32((held && hovered)
                                                         ? AzCol_HeaderActive
                                                 : hovered ? AzCol_HeaderHovered
                                                           : AzCol_Header);
                RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
            }
            RenderNavCursor(frame_bb, id, nav_render_cursor_flags);
            if (span_all_columns && !span_all_columns_label)
                TablePopBackgroundChannel();
            if (flags & AzTreeNodeFlags_Bullet)
                RenderBullet(window->DrawList,
                             AzVec2(text_pos.x - text_offset_x * 0.5f,
                                    text_pos.y + g.FontSize * 0.5f),
                             text_col);
            else if (!is_leaf)
                RenderArrow(window->DrawList,
                            AzVec2(text_pos.x - text_offset_x + padding.x,
                                   text_pos.y + g.FontSize * 0.15f),
                            text_col,
                            is_open ? ((flags & AzTreeNodeFlags_UpsideDownArrow)
                                               ? AzDir_Up
                                               : AzDir_Down)
                                    : AzDir_Right,
                            0.70f);
            if (g.LogEnabled)
                LogSetNextTextDecoration(">", NULL);
        }

        if (draw_tree_lines)
            TreeNodeDrawLineToChildNode(
                    AzVec2(text_pos.x - text_offset_x + padding.x,
                           text_pos.y + g.FontSize * 0.5f));

        if (display_frame)
            RenderTextClipped(text_pos,
                              frame_bb.Max,
                              label,
                              label_end,
                              &label_size);
        else
            RenderText(text_pos, label, label_end, false);

        if (span_all_columns_label)
            TablePopBackgroundChannel();
    }

    if (is_open && store_tree_node_stack_data)
        TreeNodeStoreStackData(flags, text_pos.x - text_offset_x);
    if (is_open && !(flags & AzTreeNodeFlags_NoTreePushOnOpen))
        TreePushOverrideID(id);

    AZ_TEST_ENGINE_ITEM_INFO(
            id,
            label,
            g.LastItemData.StatusFlags
                    | (is_leaf ? 0 : AzItemStatusFlags_Openable)
                    | (is_open ? AzItemStatusFlags_Opened : 0));
    return is_open;
}

void Az::TreeNodeDrawLineToChildNode(const AzVec2& target_pos)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if ((window->DC.TreeHasStackDataDepthMask
         & (1 << (window->DC.TreeDepth - 1)))
        == 0)
        return;

    AzTreeNodeStackData* parent_data =
            &g.TreeNodeStack.Data[g.TreeNodeStack.Size - 1];
    float x1 = AzTrunc(parent_data->DrawLinesX1);
    float x2 = AzTrunc(target_pos.x - g.Style.ItemInnerSpacing.x);
    float y = AzTrunc(target_pos.y);
    float rounding = (g.Style.TreeLinesRounding > 0.0f)
                             ? AzMin(x2 - x1, g.Style.TreeLinesRounding)
                             : 0.0f;
    parent_data->DrawLinesToNodesY2 = AzMax(parent_data->DrawLinesToNodesY2,
                                            y - rounding);
    if (x1 >= x2)
        return;
    if (rounding > 0.0f)
    {
        x1 += 0.5f + rounding;
        window->DrawList->PathArcToFast(AzVec2(x1, y - rounding),
                                        rounding,
                                        6,
                                        3);
        if (x1 < x2)
            window->DrawList->PathLineTo(AzVec2(x2, y));
        window->DrawList->PathStroke(GetColorU32(AzCol_TreeLines),
                                     AzDrawFlags_None,
                                     g.Style.TreeLinesSize);
    } else
    {
        window->DrawList->AddLine(AzVec2(x1, y),
                                  AzVec2(x2, y),
                                  GetColorU32(AzCol_TreeLines),
                                  g.Style.TreeLinesSize);
    }
}

void Az::TreeNodeDrawLineToTreePop(const AzTreeNodeStackData* data)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    float y1 = AzMax(data->NavRect.Max.y, window->ClipRect.Min.y);
    float y2 = data->DrawLinesToNodesY2;
    if (data->TreeFlags & AzTreeNodeFlags_DrawLinesFull)
    {
        float y2_full = window->DC.CursorPos.y;
        if (g.CurrentTable)
            y2_full = AzMax(g.CurrentTable->RowPosY2, y2_full);
        y2_full = AzTrunc(y2_full - g.Style.ItemSpacing.y - g.FontSize * 0.5f);
        if (y2 + (g.Style.ItemSpacing.y + g.Style.TreeLinesRounding) < y2_full)
            y2 = y2_full;
    }
    y2 = AzMin(y2, window->ClipRect.Max.y);
    if (y2 <= y1)
        return;
    float x = AzTrunc(data->DrawLinesX1);
    if (data->DrawLinesTableColumn != -1)
        TablePushColumnChannel(data->DrawLinesTableColumn);
    window->DrawList->AddLine(AzVec2(x, y1),
                              AzVec2(x, y2),
                              GetColorU32(AzCol_TreeLines),
                              g.Style.TreeLinesSize);
    if (data->DrawLinesTableColumn != -1)
        TablePopColumnChannel();
}

void Az::TreePush(const char* str_id)
{
    AzWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(str_id);
}

void Az::TreePush(const void* ptr_id)
{
    AzWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(ptr_id);
}

void Az::TreePushOverrideID(AzID id)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    Indent();
    window->DC.TreeDepth++;
    PushOverrideID(id);
}

void Az::TreePop()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    Unindent();

    window->DC.TreeDepth--;
    AzU32 tree_depth_mask = (1 << window->DC.TreeDepth);

    if (window->DC.TreeHasStackDataDepthMask & tree_depth_mask)
    {
        const AzTreeNodeStackData* data =
                &g.TreeNodeStack.Data[g.TreeNodeStack.Size - 1];
        AZ_ASSERT(data->ID == window->IDStack.back());

        if (data->TreeFlags & AzTreeNodeFlags_NavLeftJumpsToParent)
            if (g.NavIdIsAlive && g.NavMoveDir == AzDir_Left
                && g.NavWindow == window && NavMoveRequestButNoResultYet())
                NavMoveRequestResolveWithPastTreeNode(&g.NavMoveResultLocal,
                                                      data);

        if (data->DrawLinesX1 != +FLT_MAX
            && window->DC.CursorPos.y >= window->ClipRect.Min.y)
            TreeNodeDrawLineToTreePop(data);

        g.TreeNodeStack.pop_back();
        window->DC.TreeHasStackDataDepthMask &= ~tree_depth_mask;
        window->DC.TreeRecordsClippedNodesY2Mask &= ~tree_depth_mask;
    }

    AZ_ASSERT(window->IDStack.Size > 1);
    PopID();
}

float Az::GetTreeNodeToLabelSpacing()
{
    AzContext& g = *GAz;
    return g.FontSize + (g.Style.FramePadding.x * 2.0f);
}

void Az::SetNextItemOpen(bool is_open, AzCond cond)
{
    AzContext& g = *GAz;
    if (g.CurrentWindow->SkipItems)
        return;
    g.NextItemData.HasFlags |= AzNextItemDataFlags_HasOpen;
    g.NextItemData.OpenVal = is_open;
    g.NextItemData.OpenCond = (AzU8)(cond ? cond : AzCond_Always);
}

void Az::SetNextItemStorageID(AzID storage_id)
{
    AzContext& g = *GAz;
    if (g.CurrentWindow->SkipItems)
        return;
    g.NextItemData.HasFlags |= AzNextItemDataFlags_HasStorageID;
    g.NextItemData.StorageId = storage_id;
}

bool Az::CollapsingHeader(const char* label, AzTreeNodeFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    AzID id = window->GetID(label);
    return TreeNodeBehavior(id,
                            flags | AzTreeNodeFlags_CollapsingHeader,
                            label);
}

bool Az::CollapsingHeader(const char* label,
                          bool* p_visible,
                          AzTreeNodeFlags flags)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    if (p_visible && !*p_visible)
        return false;

    AzID id = window->GetID(label);
    flags |= AzTreeNodeFlags_CollapsingHeader;
    if (p_visible)
        flags |= AzTreeNodeFlags_AllowOverlap
                 | (AzTreeNodeFlags)AzTreeNodeFlags_ClipLabelForTrailingButton;
    bool is_open = TreeNodeBehavior(id, flags, label);
    if (p_visible != NULL)
    {
        AzContext& g = *GAz;
        AzLastItemData last_item_backup = g.LastItemData;
        float button_size = g.FontSize;
        float button_x = AzMax(g.LastItemData.Rect.Min.x,
                               g.LastItemData.Rect.Max.x
                                       - g.Style.FramePadding.x - button_size);
        float button_y = g.LastItemData.Rect.Min.y + g.Style.FramePadding.y;
        AzID close_button_id = GetIDWithSeed("#CLOSE", NULL, id);
        if (CloseButton(close_button_id, AzVec2(button_x, button_y)))
            *p_visible = false;
        g.LastItemData = last_item_backup;
    }

    return is_open;
}

bool Az::Selectable(const char* label,
                    bool selected,
                    AzSelectableFlags flags,
                    const AzVec2& size_arg)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;

    AzID id = window->GetID(label);
    AzVec2 label_size = CalcTextSize(label, NULL, true);
    AzVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x,
                size_arg.y != 0.0f ? size_arg.y : label_size.y);
    AzVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    const bool span_all_columns = (flags & AzSelectableFlags_SpanAllColumns)
                                  != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x
                                         : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & AzSelectableFlags_SpanAvailWidth))
        size.x = AzMax(label_size.x, max_x - min_x);

    AzRect bb(min_x, pos.y, min_x + size.x, pos.y + size.y);
    if ((flags & AzSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = AZ_TRUNC(spacing_x * 0.50f);
        const float spacing_U = AZ_TRUNC(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }

    const bool disabled_item = (flags & AzSelectableFlags_Disabled) != 0;
    const AzItemFlags extra_item_flags =
            disabled_item ? (AzItemFlags)AzItemFlags_Disabled
                          : AzItemFlags_None;
    bool is_visible;
    if (span_all_columns)
    {
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd(bb, id, NULL, extra_item_flags);
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    } else
    {
        is_visible = ItemAdd(bb, id, NULL, extra_item_flags);
    }

    const bool is_multi_select = (g.LastItemData.ItemFlags
                                  & AzItemFlags_IsMultiSelect)
                                 != 0;
    if (!is_visible)
        if (!is_multi_select || !g.BoxSelectState.UnclipMode
            || !g.BoxSelectState.UnclipRect.Overlaps(bb))
            return false;

    const bool disabled_global = (g.CurrentItemFlags & AzItemFlags_Disabled)
                                 != 0;
    if (disabled_item && !disabled_global)
        BeginDisabled();

    if (span_all_columns)
    {
        if (g.CurrentTable)
            TablePushBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PushColumnsBackground();
        g.LastItemData.StatusFlags |= AzItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    AzButtonFlags button_flags = 0;
    if (flags & AzSelectableFlags_NoHoldingActiveID)
    {
        button_flags |= AzButtonFlags_NoHoldingActiveId;
    }
    if (flags & AzSelectableFlags_NoSetKeyOwner)
    {
        button_flags |= AzButtonFlags_NoSetKeyOwner;
    }
    if (flags & AzSelectableFlags_SelectOnClick)
    {
        button_flags |= AzButtonFlags_PressedOnClick;
    }
    if (flags & AzSelectableFlags_SelectOnRelease)
    {
        button_flags |= AzButtonFlags_PressedOnRelease;
    }
    if (flags & AzSelectableFlags_AllowDoubleClick)
    {
        button_flags |= AzButtonFlags_PressedOnClickRelease
                        | AzButtonFlags_PressedOnDoubleClick;
    }
    if ((flags & AzSelectableFlags_AllowOverlap)
        || (g.LastItemData.ItemFlags & AzItemFlags_AllowOverlap))
    {
        button_flags |= AzButtonFlags_AllowOverlap;
    }

    const bool was_selected = selected;
    if (is_multi_select)
    {
        MultiSelectItemHeader(id, &selected, &button_flags);
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

    if (is_multi_select)
    {
        MultiSelectItemFooter(id, &selected, &pressed);
    } else
    {
        if ((flags & AzSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0
            && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
            if (g.NavJustMovedToId == id)
                selected = pressed = true;
    }

    if (pressed || (hovered && (flags & AzSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavHighlightItemUnderNav && g.NavWindow == window
            && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id,
                     window->DC.NavLayerCurrent,
                     g.CurrentFocusScopeId,
                     WindowRectAbsToRel(window, bb));
            if (g.IO.ConfigNavCursorVisibleAuto)
                g.NavCursorVisible = false;
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (selected != was_selected)
        g.LastItemData.StatusFlags |= AzItemStatusFlags_ToggledSelection;

    if (is_visible)
    {
        const bool highlighted = hovered
                                 || (flags & AzSelectableFlags_Highlight);
        if (highlighted || selected)
        {
            AzU32 col = GetColorU32((held && highlighted) ? AzCol_HeaderActive
                                    : highlighted         ? AzCol_HeaderHovered
                                                          : AzCol_Header);
            RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
        }
        if (g.NavId == id)
        {
            AzNavRenderCursorFlags nav_render_cursor_flags =
                    AzNavRenderCursorFlags_Compact
                    | AzNavRenderCursorFlags_NoRounding;
            if (is_multi_select)
                nav_render_cursor_flags |= AzNavRenderCursorFlags_AlwaysDraw;
            RenderNavCursor(bb, id, nav_render_cursor_flags);
        }
    }

    if (span_all_columns)
    {
        if (g.CurrentTable)
            TablePopBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PopColumnsBackground();
    }

    if (is_visible)
        RenderTextClipped(pos,
                          AzVec2(AzMin(pos.x + size.x, window->WorkRect.Max.x),
                                 pos.y + size.y),
                          label,
                          NULL,
                          &label_size,
                          style.SelectableTextAlign,
                          &bb);

    if (pressed && (window->Flags & AzWindowFlags_Popup)
        && !(flags & AzSelectableFlags_NoAutoClosePopups)
        && (g.LastItemData.ItemFlags & AzItemFlags_AutoClosePopups))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();

    AZ_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool Az::Selectable(const char* label,
                    bool* p_selected,
                    AzSelectableFlags flags,
                    const AzVec2& size_arg)
{
    if (Selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

AzTypingSelectRequest* Az::GetTypingSelectRequest(AzTypingSelectFlags flags)
{
    AzContext& g = *GAz;
    AzTypingSelectState* data = &g.TypingSelectState;
    AzTypingSelectRequest* out_request = &data->Request;

    const float TYPING_SELECT_RESET_TAZER = 1.80f;
    const int TYPING_SELECT_SINGLE_CHAR_COUNT_FOR_LOCK = 4;
    if (data->SearchBuffer[0] != 0)
    {
        bool clear_buffer = false;
        clear_buffer |= (g.NavFocusScopeId != data->FocusScope);
        clear_buffer |= (data->LastRequestTime + TYPING_SELECT_RESET_TAZER
                         < g.Time);
        clear_buffer |= g.NavAnyRequest;
        clear_buffer |= g.ActiveId != 0 && g.NavActivateId == 0;
        clear_buffer |= IsKeyPressed(AzKey_Escape) || IsKeyPressed(AzKey_Enter);
        clear_buffer |= IsKeyPressed(AzKey_Backspace)
                        && (flags & AzTypingSelectFlags_AllowBackspace) == 0;

        if (clear_buffer)
            data->Clear();
    }

    const int buffer_max_len = AZ_ARRAYSIZE(data->SearchBuffer) - 1;
    int buffer_len = (int)AzStrlen(data->SearchBuffer);
    bool select_request = false;
    for (AzWchar w : g.IO.InputQueueCharacters)
    {
        const int w_len = AzTextCountUtf8BytesFromStr(&w, &w + 1);
        if (w < 32 || (buffer_len == 0 && AzCharIsBlankW(w))
            || (buffer_len + w_len > buffer_max_len))
            continue;
        char w_buf[5];
        AzTextCharToUtf8(w_buf, (unsigned int)w);
        if (data->SingleCharModeLock && w_len == out_request->SingleCharSize
            && memcmp(w_buf, data->SearchBuffer, w_len) == 0)
        {
            select_request = true;
            continue;
        }
        if (data->SingleCharModeLock)
        {
            data->Clear();
            buffer_len = 0;
        }
        memcpy(data->SearchBuffer + buffer_len, w_buf, w_len + 1);
        buffer_len += w_len;
        select_request = true;
    }
    g.IO.InputQueueCharacters.resize(0);

    if ((flags & AzTypingSelectFlags_AllowBackspace)
        && IsKeyPressed(AzKey_Backspace, AzInputFlags_Repeat))
    {
        char* p = (char*)(void*)AzTextFindPreviousUtf8Codepoint(
                data->SearchBuffer,
                data->SearchBuffer + buffer_len);
        *p = 0;
        buffer_len = (int)(p - data->SearchBuffer);
    }

    if (buffer_len == 0)
        return NULL;
    if (select_request)
    {
        data->FocusScope = g.NavFocusScopeId;
        data->LastRequestFrame = g.FrameCount;
        data->LastRequestTime = (float)g.Time;
    }
    out_request->Flags = flags;
    out_request->SearchBufferLen = buffer_len;
    out_request->SearchBuffer = data->SearchBuffer;
    out_request->SelectRequest = (data->LastRequestFrame == g.FrameCount);
    out_request->SingleCharMode = false;
    out_request->SingleCharSize = 0;

    if (flags & AzTypingSelectFlags_AllowSingleCharMode)
    {
        const char* buf_begin = out_request->SearchBuffer;
        const char* buf_end = out_request->SearchBuffer
                              + out_request->SearchBufferLen;
        const int c0_len = AzTextCountUtf8BytesFromChar(buf_begin, buf_end);
        const char* p = buf_begin + c0_len;
        for (; p < buf_end; p += c0_len)
            if (memcmp(buf_begin, p, (size_t)c0_len) != 0)
                break;
        const int single_char_count = (p == buf_end)
                                              ? (out_request->SearchBufferLen
                                                 / c0_len)
                                              : 0;
        out_request->SingleCharMode = (single_char_count > 0
                                       || data->SingleCharModeLock);
        out_request->SingleCharSize = (AzS8)c0_len;
        data->SingleCharModeLock |=
                (single_char_count >= TYPING_SELECT_SINGLE_CHAR_COUNT_FOR_LOCK);
    }

    return out_request;
}

static int AzStrimatchlen(const char* s1, const char* s1_end, const char* s2)
{
    int match_len = 0;
    while (s1 < s1_end && AzToUpper(*s1++) == AzToUpper(*s2++))
        match_len++;
    return match_len;
}

int Az::TypingSelectFindMatch(AzTypingSelectRequest* req,
                              int items_count,
                              const char* (*get_item_name_func)(void*, int),
                              void* user_data,
                              int nav_item_idx)
{
    if (req == NULL || req->SelectRequest == false)
        return -1;
    int idx = -1;
    if (req->SingleCharMode
        && (req->Flags & AzTypingSelectFlags_AllowSingleCharMode))
        idx = TypingSelectFindNextSingleCharMatch(req,
                                                  items_count,
                                                  get_item_name_func,
                                                  user_data,
                                                  nav_item_idx);
    else
        idx = TypingSelectFindBestLeadingMatch(req,
                                               items_count,
                                               get_item_name_func,
                                               user_data);
    if (idx != -1)
        SetNavCursorVisibleAfterMove();
    return idx;
}

int Az::TypingSelectFindNextSingleCharMatch(
        AzTypingSelectRequest* req,
        int items_count,
        const char* (*get_item_name_func)(void*, int),
        void* user_data,
        int nav_item_idx)
{
    int first_match_idx = -1;
    bool return_next_match = false;
    for (int idx = 0; idx < items_count; idx++)
    {
        const char* item_name = get_item_name_func(user_data, idx);
        if (AzStrimatchlen(req->SearchBuffer,
                           req->SearchBuffer + req->SingleCharSize,
                           item_name)
            < req->SingleCharSize)
            continue;
        if (return_next_match)
            return idx;
        if (first_match_idx == -1 && nav_item_idx == -1)
            return idx;
        if (first_match_idx == -1)
            first_match_idx = idx;
        if (nav_item_idx == idx)
            return_next_match = true;
    }
    return first_match_idx;
}

int Az::TypingSelectFindBestLeadingMatch(
        AzTypingSelectRequest* req,
        int items_count,
        const char* (*get_item_name_func)(void*, int),
        void* user_data)
{
    int longest_match_idx = -1;
    int longest_match_len = 0;
    for (int idx = 0; idx < items_count; idx++)
    {
        const char* item_name = get_item_name_func(user_data, idx);
        const int match_len = AzStrimatchlen(req->SearchBuffer,
                                             req->SearchBuffer
                                                     + req->SearchBufferLen,
                                             item_name);
        if (match_len <= longest_match_len)
            continue;
        longest_match_idx = idx;
        longest_match_len = match_len;
        if (match_len == req->SearchBufferLen)
            break;
    }
    return longest_match_idx;
}

void Az::DebugNodeTypingSelectState(AzTypingSelectState* data)
{
#ifndef AZ_DISABLE_DEBUG_TOOLS
    Text("SearchBuffer = \"%s\"", data->SearchBuffer);
    Text("SingleCharMode = %d, Size = %d, Lock = %d",
         data->Request.SingleCharMode,
         data->Request.SingleCharSize,
         data->SingleCharModeLock);
    Text("LastRequest = time: %.2f, frame: %d",
         data->LastRequestTime,
         data->LastRequestFrame);
#else
    AZ_UNUSED(data);
#endif
}

static void BoxSelectPreStartDrag(AzID id, AzSelectionUserData clicked_item)
{
    AzContext& g = *GAz;
    AzBoxSelectState* bs = &g.BoxSelectState;
    bs->ID = id;
    bs->IsStarting = true;
    bs->IsStartedFromVoid = (clicked_item == AzSelectionUserData_Invalid);
    bs->IsStartedSetNavIdOnce = bs->IsStartedFromVoid;
    bs->KeyMods = g.IO.KeyMods;
    bs->StartPosRel = bs->EndPosRel = Az::WindowPosAbsToRel(g.CurrentWindow,
                                                            g.IO.MousePos);
    bs->ScrollAccum = AzVec2(0.0f, 0.0f);
}

static void BoxSelectActivateDrag(AzBoxSelectState* bs, AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_DEBUG_LOG_SELECTION("[selection] BeginBoxSelect() 0X%08X: Activate\n",
                           bs->ID);
    bs->IsActive = true;
    bs->Window = window;
    bs->IsStarting = false;
    Az::SetActiveID(bs->ID, window);
    Az::SetActiveIdUsingAllKeyboardKeys();
    if (bs->IsStartedFromVoid
        && (bs->KeyMods & (AzMod_Ctrl | AzMod_Shift)) == 0)
        bs->RequestClear = true;
}

static void BoxSelectDeactivateDrag(AzBoxSelectState* bs)
{
    AzContext& g = *GAz;
    bs->IsActive = bs->IsStarting = false;
    if (g.ActiveId == bs->ID)
    {
        AZ_DEBUG_LOG_SELECTION(
                "[selection] BeginBoxSelect() 0X%08X: Deactivate\n",
                bs->ID);
        Az::ClearActiveID();
    }
    bs->ID = 0;
}

static void BoxSelectScrollWithMouseDrag(AzBoxSelectState* bs,
                                         AzWindow* window,
                                         const AzRect& inner_r)
{
    AzContext& g = *GAz;
    AZ_ASSERT(bs->Window == window);
    for (int n = 0; n < 2; n++)
    {
        const float mouse_pos = g.IO.MousePos[n];
        const float dist = (mouse_pos > inner_r.Max[n])
                                   ? mouse_pos - inner_r.Max[n]
                           : (mouse_pos < inner_r.Min[n])
                                   ? mouse_pos - inner_r.Min[n]
                                   : 0.0f;
        const float scroll_curr = window->Scroll[n];
        if (dist == 0.0f || (dist < 0.0f && scroll_curr < 0.0f)
            || (dist > 0.0f && scroll_curr >= window->ScrollMax[n]))
            continue;

        const float speed_multiplier = AzLinearRemapClamp(g.FontSize,
                                                          g.FontSize * 5.0f,
                                                          1.0f,
                                                          4.0f,
                                                          AzAbs(dist));
        const float scroll_step = g.FontSize * 35.0f * speed_multiplier
                                  * AzSign(dist) * g.IO.DeltaTime;
        bs->ScrollAccum[n] += scroll_step;

        const float scroll_step_i = AzFloor(bs->ScrollAccum[n]);
        if (scroll_step_i == 0.0f)
            continue;
        if (n == 0)
            Az::SetScrollX(window, scroll_curr + scroll_step_i);
        else
            Az::SetScrollY(window, scroll_curr + scroll_step_i);
        bs->ScrollAccum[n] -= scroll_step_i;
    }
}

bool Az::BeginBoxSelect(const AzRect& scope_rect,
                        AzWindow* window,
                        AzID box_select_id,
                        AzMultiSelectFlags ms_flags)
{
    AzContext& g = *GAz;
    AzBoxSelectState* bs = &g.BoxSelectState;
    KeepAliveID(box_select_id);
    if (bs->ID != box_select_id)
        return false;

    bs->UnclipMode = false;
    bs->RequestClear = false;
    if (bs->IsStarting && IsMouseDragPastThreshold(0))
        BoxSelectActivateDrag(bs, window);
    else if ((bs->IsStarting || bs->IsActive) && g.IO.MouseDown[0] == false)
        BoxSelectDeactivateDrag(bs);
    if (!bs->IsActive)
        return false;

    AzVec2 start_pos_abs = WindowPosRelToAbs(window, bs->StartPosRel);
    AzVec2 prev_end_pos_abs = WindowPosRelToAbs(window, bs->EndPosRel);
    AzVec2 curr_end_pos_abs = g.IO.MousePos;
    if (ms_flags & AzMultiSelectFlags_ScopeWindow)
        curr_end_pos_abs = AzClamp(curr_end_pos_abs,
                                   scope_rect.Min,
                                   scope_rect.Max);
    bs->BoxSelectRectPrev.Min = AzMin(start_pos_abs, prev_end_pos_abs);
    bs->BoxSelectRectPrev.Max = AzMax(start_pos_abs, prev_end_pos_abs);
    bs->BoxSelectRectCurr.Min = AzMin(start_pos_abs, curr_end_pos_abs);
    bs->BoxSelectRectCurr.Max = AzMax(start_pos_abs, curr_end_pos_abs);

    if (ms_flags & AzMultiSelectFlags_BoxSelect2d)
        if (bs->BoxSelectRectPrev.Min.x != bs->BoxSelectRectCurr.Min.x
            || bs->BoxSelectRectPrev.Max.x != bs->BoxSelectRectCurr.Max.x)
        {
            bs->UnclipMode = true;
            bs->UnclipRect = bs->BoxSelectRectPrev;
            bs->UnclipRect.Add(bs->BoxSelectRectCurr);
        }

    return true;
}

void Az::EndBoxSelect(const AzRect& scope_rect, AzMultiSelectFlags ms_flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzBoxSelectState* bs = &g.BoxSelectState;
    AZ_ASSERT(bs->IsActive);
    bs->UnclipMode = false;

    bs->EndPosRel = WindowPosAbsToRel(window,
                                      AzClamp(g.IO.MousePos,
                                              scope_rect.Min,
                                              scope_rect.Max));
    AzRect box_select_r = bs->BoxSelectRectCurr;
    box_select_r.ClipWith(scope_rect);
    window->DrawList->AddRectFilled(box_select_r.Min,
                                    box_select_r.Max,
                                    GetColorU32(AzCol_SeparatorHovered, 0.30f));
    window->DrawList->AddRect(box_select_r.Min,
                              box_select_r.Max,
                              GetColorU32(AzCol_NavCursor));

    const bool enable_scroll = (ms_flags & AzMultiSelectFlags_ScopeWindow)
                               && (ms_flags
                                   & AzMultiSelectFlags_BoxSelectNoScroll)
                                          == 0;
    if (enable_scroll)
    {
        AzRect scroll_r = scope_rect;
        scroll_r.Expand(-g.FontSize);

        if (!scroll_r.Contains(g.IO.MousePos))
            BoxSelectScrollWithMouseDrag(bs, window, scroll_r);
    }
}

static void DebugLogMultiSelectRequests(const char* function,
                                        const AzMultiSelectIO* io)
{
    AzContext& g = *GAz;
    AZ_UNUSED(function);
    for (const AzSelectionRequest& req : io->Requests)
    {
        if (req.Type == AzSelectionRequestType_SetAll)
            AZ_DEBUG_LOG_SELECTION(
                    "[selection] %s: Request: SetAll %d (= %s)\n",
                    function,
                    req.Selected,
                    req.Selected ? "SelectAll" : "Clear");
        if (req.Type == AzSelectionRequestType_SetRange)
            AZ_DEBUG_LOG_SELECTION(
                    "[selection] %s: Request: SetRange %" AZ_PRId64
                    "..%" AZ_PRId64 " (0x%" AZ_PRIX64 "..0x%" AZ_PRIX64
                    ") = %d (dir %d)\n",
                    function,
                    req.RangeFirstItem,
                    req.RangeLastItem,
                    req.RangeFirstItem,
                    req.RangeLastItem,
                    req.Selected,
                    req.RangeDirection);
    }
}

static AzRect CalcScopeRect(AzMultiSelectTempData* ms, AzWindow* window)
{
    AzContext& g = *GAz;
    if (ms->Flags & AzMultiSelectFlags_ScopeRect)
    {
        return AzRect(ms->ScopeRectMin,
                      AzMax(window->DC.CursorMaxPos, ms->ScopeRectMin));
    } else
    {
        AzRect scope_rect = window->InnerClipRect;
        if (g.CurrentTable != NULL)
            scope_rect = g.CurrentTable->HostClipRect;

        scope_rect.Min = AzMin(scope_rect.Min
                                       + AzVec2(window->DecoInnerSizeX1,
                                                window->DecoInnerSizeY1),
                               scope_rect.Max);
        return scope_rect;
    }
}

AzMultiSelectIO* Az::BeginMultiSelect(AzMultiSelectFlags flags,
                                      int selection_size,
                                      int items_count)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    if (++g.MultiSelectTempDataStacked > g.MultiSelectTempData.Size)
        g.MultiSelectTempData.resize(g.MultiSelectTempDataStacked,
                                     AzMultiSelectTempData());
    AzMultiSelectTempData* ms =
            &g.MultiSelectTempData[g.MultiSelectTempDataStacked - 1];
    AZ_STATIC_ASSERT(offsetof(AzMultiSelectTempData, IO) == 0);
    g.CurrentMultiSelect = ms;
    if ((flags
         & (AzMultiSelectFlags_ScopeWindow | AzMultiSelectFlags_ScopeRect))
        == 0)
        flags |= AzMultiSelectFlags_ScopeWindow;
    if (flags & AzMultiSelectFlags_SingleSelect)
        flags &= ~(AzMultiSelectFlags_BoxSelect2d
                   | AzMultiSelectFlags_BoxSelect1d);
    if (flags & AzMultiSelectFlags_BoxSelect2d)
        flags &= ~AzMultiSelectFlags_BoxSelect1d;

    if (AzTable* table = g.CurrentTable)
        if (table->CurrentColumn != -1)
            TableEndCell(table);

    const AzID id = window->IDStack.back();
    ms->Clear();
    ms->FocusScopeId = id;
    ms->Flags = flags;
    ms->IsFocused = (ms->FocusScopeId == g.NavFocusScopeId);
    ms->BackupCursorMaxPos = window->DC.CursorMaxPos;
    ms->ScopeRectMin = window->DC.CursorMaxPos = window->DC.CursorPos;
    PushFocusScope(ms->FocusScopeId);
    if (flags & AzMultiSelectFlags_ScopeWindow)
        window->DC.NavLayersActiveMask |= 1 << AzNavLayer_Main;

    ms->KeyMods = g.NavJustMovedToId ? (g.NavJustMovedToIsTabbing
                                                ? 0
                                                : g.NavJustMovedToKeyMods)
                                     : g.IO.KeyMods;
    if (flags & AzMultiSelectFlags_NoRangeSelect)
        ms->KeyMods &= ~AzMod_Shift;

    AzMultiSelectState* storage = g.MultiSelectStorage.GetOrAddByKey(id);
    storage->ID = id;
    storage->LastFrameActive = g.FrameCount;
    storage->LastSelectionSize = selection_size;
    storage->Window = window;
    ms->Storage = storage;

    ms->IO.Requests.resize(0);
    ms->IO.RangeSrcItem = storage->RangeSrcItem;
    ms->IO.NavIdItem = storage->NavIdItem;
    ms->IO.NavIdSelected = (storage->NavIdSelected == 1) ? true : false;
    ms->IO.ItemsCount = items_count;

    bool request_clear = false;
    bool request_select_all = false;
    if (g.NavJustMovedToId != 0
        && g.NavJustMovedToFocusScopeId == ms->FocusScopeId
        && g.NavJustMovedToHasSelectionData)
    {
        if (ms->KeyMods & AzMod_Shift)
            ms->IsKeyboardSetRange = true;
        if (ms->IsKeyboardSetRange)
            AZ_ASSERT(storage->RangeSrcItem != AzSelectionUserData_Invalid);
        if ((ms->KeyMods & (AzMod_Ctrl | AzMod_Shift)) == 0
            && (flags
                & (AzMultiSelectFlags_NoAutoClear
                   | AzMultiSelectFlags_NoAutoSelect))
                       == 0)
            request_clear = true;
    } else if (g.NavJustMovedFromFocusScopeId == ms->FocusScopeId)
    {
        if ((ms->KeyMods & (AzMod_Ctrl | AzMod_Shift)) == 0
            && (flags
                & (AzMultiSelectFlags_NoAutoClear
                   | AzMultiSelectFlags_NoAutoSelect))
                       == 0)
            request_clear = true;
    }

    AzBoxSelectState* bs = &g.BoxSelectState;
    if (flags
        & (AzMultiSelectFlags_BoxSelect1d | AzMultiSelectFlags_BoxSelect2d))
    {
        ms->BoxSelectId = GetID("##BoxSelect");
        if (BeginBoxSelect(CalcScopeRect(ms, window),
                           window,
                           ms->BoxSelectId,
                           flags))
            request_clear |= bs->RequestClear;
    }

    if (ms->IsFocused)
    {
        if (flags & AzMultiSelectFlags_ClearOnEscape)
        {
            if (selection_size != 0 || bs->IsActive)
                if (Shortcut(AzKey_Escape,
                             AzInputFlags_None,
                             bs->IsActive ? bs->ID : 0))
                {
                    request_clear = true;
                    if (bs->IsActive)
                        BoxSelectDeactivateDrag(bs);
                }
        }

        if (!(flags & AzMultiSelectFlags_SingleSelect)
            && !(flags & AzMultiSelectFlags_NoSelectAll))
            if (Shortcut(AzMod_Ctrl | AzKey_A))
                request_select_all = true;
    }

    if (request_clear || request_select_all)
    {
        MultiSelectAddSetAll(ms, request_select_all);
        if (!request_select_all)
            storage->LastSelectionSize = 0;
    }
    ms->LoopRequestSetAll = request_select_all ? 1 : request_clear ? 0 : -1;
    ms->LastSubmittedItem = AzSelectionUserData_Invalid;

    if (g.DebugLogFlags & AzDebugLogFlags_EventSelection)
        DebugLogMultiSelectRequests("BeginMultiSelect", &ms->IO);

    return &ms->IO;
}

AzMultiSelectIO* Az::EndMultiSelect()
{
    AzContext& g = *GAz;
    AzMultiSelectTempData* ms = g.CurrentMultiSelect;
    AzMultiSelectState* storage = ms->Storage;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT_USER_ERROR(ms->FocusScopeId == g.CurrentFocusScopeId,
                         "EndMultiSelect() FocusScope mismatch!");
    AZ_ASSERT(g.CurrentMultiSelect != NULL
              && storage->Window == g.CurrentWindow);
    AZ_ASSERT(g.MultiSelectTempDataStacked > 0
              && &g.MultiSelectTempData[g.MultiSelectTempDataStacked - 1]
                         == g.CurrentMultiSelect);

    AzRect scope_rect = CalcScopeRect(ms, window);
    if (ms->IsFocused)
    {
        if (ms->IO.RangeSrcReset
            || (ms->RangeSrcPassedBy == false
                && ms->IO.RangeSrcItem != AzSelectionUserData_Invalid))
        {
            AZ_DEBUG_LOG_SELECTION(
                    "[selection] EndMultiSelect: Reset RangeSrcItem.\n");
            storage->RangeSrcItem = AzSelectionUserData_Invalid;
        }
        if (ms->NavIdPassedBy == false
            && storage->NavIdItem != AzSelectionUserData_Invalid)
        {
            AZ_DEBUG_LOG_SELECTION(
                    "[selection] EndMultiSelect: Reset NavIdItem.\n");
            storage->NavIdItem = AzSelectionUserData_Invalid;
            storage->NavIdSelected = -1;
        }

        if ((ms->Flags
             & (AzMultiSelectFlags_BoxSelect1d
                | AzMultiSelectFlags_BoxSelect2d))
            && GetBoxSelectState(ms->BoxSelectId))
            EndBoxSelect(scope_rect, ms->Flags);
    }

    if (ms->IsEndIO == false)
        ms->IO.Requests.resize(0);

    bool scope_hovered = IsWindowHovered()
                         && window->InnerRect.Contains(g.IO.MousePos);
    if (scope_hovered && (ms->Flags & AzMultiSelectFlags_ScopeRect))
        scope_hovered &= scope_rect.Contains(g.IO.MousePos);
    if (scope_hovered && g.HoveredId == 0 && g.ActiveId == 0)
    {
        if (ms->Flags
            & (AzMultiSelectFlags_BoxSelect1d | AzMultiSelectFlags_BoxSelect2d))
        {
            if (!g.BoxSelectState.IsActive && !g.BoxSelectState.IsStarting
                && g.IO.MouseClickedCount[0] == 1)
            {
                BoxSelectPreStartDrag(ms->BoxSelectId,
                                      AzSelectionUserData_Invalid);
                FocusWindow(window, AzFocusRequestFlags_UnlessBelowModal);
                SetHoveredID(ms->BoxSelectId);
                if (ms->Flags & AzMultiSelectFlags_ScopeRect)
                    SetNavID(0,
                             AzNavLayer_Main,
                             ms->FocusScopeId,
                             AzRect(g.IO.MousePos, g.IO.MousePos));
            }
        }

        if (ms->Flags & AzMultiSelectFlags_ClearOnClickVoid)
            if (IsMouseReleased(0) && IsMouseDragPastThreshold(0) == false
                && g.IO.KeyMods == AzMod_None)
                MultiSelectAddSetAll(ms, false);
    }

    if (ms->Flags & AzMultiSelectFlags_NavWrapX)
    {
        AZ_ASSERT(ms->Flags & AzMultiSelectFlags_ScopeWindow);
        Az::NavMoveRequestTryWrapping(Az::GetCurrentWindow(),
                                      AzNavMoveFlags_WrapX);
    }

    window->DC.CursorMaxPos = AzMax(ms->BackupCursorMaxPos,
                                    window->DC.CursorMaxPos);
    PopFocusScope();

    if (g.DebugLogFlags & AzDebugLogFlags_EventSelection)
        DebugLogMultiSelectRequests("EndMultiSelect", &ms->IO);

    ms->FocusScopeId = 0;
    ms->Flags = AzMultiSelectFlags_None;
    g.CurrentMultiSelect =
            (--g.MultiSelectTempDataStacked > 0)
                    ? &g.MultiSelectTempData[g.MultiSelectTempDataStacked - 1]
                    : NULL;

    return &ms->IO;
}

void Az::SetNextItemSelectionUserData(AzSelectionUserData selection_user_data)
{
    AzContext& g = *GAz;
    g.NextItemData.SelectionUserData = selection_user_data;
    g.NextItemData.FocusScopeId = g.CurrentFocusScopeId;

    if (AzMultiSelectTempData* ms = g.CurrentMultiSelect)
    {
        g.NextItemData.ItemFlags |= AzItemFlags_HasSelectionUserData
                                    | AzItemFlags_IsMultiSelect;
        if (ms->IO.RangeSrcItem == selection_user_data)
            ms->RangeSrcPassedBy = true;
    } else
    {
        g.NextItemData.ItemFlags |= AzItemFlags_HasSelectionUserData;
    }
}

void Az::MultiSelectItemHeader(AzID id,
                               bool* p_selected,
                               AzButtonFlags* p_button_flags)
{
    AzContext& g = *GAz;
    AzMultiSelectTempData* ms = g.CurrentMultiSelect;

    bool selected = *p_selected;
    if (ms->IsFocused)
    {
        AzMultiSelectState* storage = ms->Storage;
        AzSelectionUserData item_data = g.NextItemData.SelectionUserData;
        AZ_ASSERT(g.NextItemData.FocusScopeId == g.CurrentFocusScopeId && "Forgot to call SetNextItemSelectionUserData() prior to item, required in BeginMultiSelect()/EndMultiSelect() scope");

        if (ms->LoopRequestSetAll != -1)
            selected = (ms->LoopRequestSetAll == 1);

        if (ms->IsKeyboardSetRange)
        {
            AZ_ASSERT(id != 0 && (ms->KeyMods & AzMod_Shift) != 0);
            const bool is_range_dst = (ms->RangeDstPassedBy == false)
                                      && g.NavJustMovedToId == id;
            if (is_range_dst)
                ms->RangeDstPassedBy = true;
            if (is_range_dst
                && storage->RangeSrcItem == AzSelectionUserData_Invalid)
            {
                storage->RangeSrcItem = item_data;
                storage->RangeSelected = selected ? 1 : 0;
            }
            const bool is_range_src = storage->RangeSrcItem == item_data;
            if (is_range_src || is_range_dst
                || ms->RangeSrcPassedBy != ms->RangeDstPassedBy)
            {
                AZ_ASSERT(storage->RangeSrcItem != AzSelectionUserData_Invalid
                          && storage->RangeSelected != -1);
                selected = (storage->RangeSelected != 0);
            } else if ((ms->KeyMods & AzMod_Ctrl) == 0
                       && (ms->Flags & AzMultiSelectFlags_NoAutoClear) == 0)
            {
                selected = false;
            }
        }
        *p_selected = selected;
    }

    if (p_button_flags != NULL)
    {
        AzButtonFlags button_flags = *p_button_flags;
        button_flags |= AzButtonFlags_NoHoveredOnFocus;
        if ((!selected || (g.ActiveId == id && g.ActiveIdHasBeenPressedBefore))
            && !(ms->Flags & AzMultiSelectFlags_SelectOnClickRelease))
            button_flags = (button_flags | AzButtonFlags_PressedOnClick)
                           & ~AzButtonFlags_PressedOnClickRelease;
        else
            button_flags |= AzButtonFlags_PressedOnClickRelease;
        *p_button_flags = button_flags;
    }
}

void Az::MultiSelectItemFooter(AzID id, bool* p_selected, bool* p_pressed)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    bool selected = *p_selected;
    bool pressed = *p_pressed;
    AzMultiSelectTempData* ms = g.CurrentMultiSelect;
    AzMultiSelectState* storage = ms->Storage;
    if (pressed)
        ms->IsFocused = true;

    bool hovered = false;
    if (g.LastItemData.StatusFlags & AzItemStatusFlags_HoveredRect)
        hovered = IsItemHovered(AzHoveredFlags_AllowWhenBlockedByPopup);
    if (!ms->IsFocused && !hovered)
        return;

    AzSelectionUserData item_data = g.NextItemData.SelectionUserData;

    AzMultiSelectFlags flags = ms->Flags;
    const bool is_singleselect = (flags & AzMultiSelectFlags_SingleSelect) != 0;
    bool is_ctrl = (ms->KeyMods & AzMod_Ctrl) != 0;
    bool is_shift = (ms->KeyMods & AzMod_Shift) != 0;

    bool apply_to_range_src = false;

    if (g.NavId == id && storage->RangeSrcItem == AzSelectionUserData_Invalid)
        apply_to_range_src = true;
    if (ms->IsEndIO == false)
    {
        ms->IO.Requests.resize(0);
        ms->IsEndIO = true;
    }

    if (g.NavJustMovedToId == id)
    {
        if ((flags & AzMultiSelectFlags_NoAutoSelect) == 0)
        {
            if (is_ctrl && is_shift)
                pressed = true;
            else if (!is_ctrl)
                selected = pressed = true;
        } else
        {
            if (is_shift)
                pressed = true;
            else if (!is_ctrl)
                apply_to_range_src = true;
        }
    }

    if (apply_to_range_src)
    {
        storage->RangeSrcItem = item_data;
        storage->RangeSelected = selected;
    }

    if (ms->BoxSelectId != 0)
        if (AzBoxSelectState* bs = GetBoxSelectState(ms->BoxSelectId))
        {
            const bool rect_overlap_curr = bs->BoxSelectRectCurr.Overlaps(
                    g.LastItemData.Rect);
            const bool rect_overlap_prev = bs->BoxSelectRectPrev.Overlaps(
                    g.LastItemData.Rect);
            if ((rect_overlap_curr && !rect_overlap_prev && !selected)
                || (rect_overlap_prev && !rect_overlap_curr))
            {
                if (storage->LastSelectionSize <= 0
                    && bs->IsStartedSetNavIdOnce)
                {
                    pressed = true;
                    bs->IsStartedSetNavIdOnce = false;
                } else
                {
                    selected = !selected;
                    MultiSelectAddSetRange(ms,
                                           selected,
                                           +1,
                                           item_data,
                                           item_data);
                }
                storage->LastSelectionSize = AzMax(storage->LastSelectionSize
                                                           + 1,
                                                   1);
            }
        }

    if (hovered && IsMouseClicked(1)
        && (flags & AzMultiSelectFlags_NoAutoSelect) == 0)
    {
        if (g.ActiveId != 0 && g.ActiveId != id)
            ClearActiveID();
        SetFocusID(id, window);
        if (!pressed && !selected)
        {
            pressed = true;
            is_ctrl = is_shift = false;
        }
    }

    const bool enter_pressed = pressed && (g.NavActivateId == id)
                               && (g.NavActivateFlags
                                   & AzActivateFlags_PreferInput);

    if (pressed && (!enter_pressed || !selected))
    {
        AzInputSource input_source = (g.NavJustMovedToId == id
                                      || g.NavActivateId == id)
                                             ? g.NavInputSource
                                             : AzInputSource_Mouse;
        if (flags
            & (AzMultiSelectFlags_BoxSelect1d | AzMultiSelectFlags_BoxSelect2d))
            if (selected == false && !g.BoxSelectState.IsActive
                && !g.BoxSelectState.IsStarting
                && input_source == AzInputSource_Mouse
                && g.IO.MouseClickedCount[0] == 1)
                BoxSelectPreStartDrag(ms->BoxSelectId, item_data);

        if ((flags & AzMultiSelectFlags_NoAutoClear) == 0)
        {
            bool request_clear = false;
            if (is_singleselect)
                request_clear = true;
            else if ((input_source == AzInputSource_Mouse
                      || g.NavActivateId == id)
                     && !is_ctrl)
                request_clear = (flags
                                 & AzMultiSelectFlags_NoAutoClearOnReselect)
                                        ? !selected
                                        : true;
            else if ((input_source == AzInputSource_Keyboard
                      || input_source == AzInputSource_Gamepad)
                     && is_shift && !is_ctrl)
                request_clear = true;
            if (request_clear)
                MultiSelectAddSetAll(ms, false);
        }

        int range_direction;
        bool range_selected;
        if (is_shift && !is_singleselect)
        {
            if (storage->RangeSrcItem == AzSelectionUserData_Invalid)
                storage->RangeSrcItem = item_data;
            if ((flags & AzMultiSelectFlags_NoAutoSelect) == 0)
            {
                range_selected = (is_ctrl && storage->RangeSelected != -1)
                                         ? (storage->RangeSelected != 0)
                                         : true;
            } else
            {
                if (ms->IsKeyboardSetRange)
                    range_selected = (storage->RangeSelected != -1)
                                             ? (storage->RangeSelected != 0)
                                             : true;
                else
                    range_selected = !selected;
            }
            range_direction = ms->RangeSrcPassedBy ? +1 : -1;
        } else
        {
            if ((flags & AzMultiSelectFlags_NoAutoSelect) == 0)
                selected = is_ctrl ? !selected : true;
            else
                selected = !selected;
            storage->RangeSrcItem = item_data;
            range_selected = selected;
            range_direction = +1;
        }
        MultiSelectAddSetRange(ms,
                               range_selected,
                               range_direction,
                               storage->RangeSrcItem,
                               item_data);
    }

    if (storage->RangeSrcItem == item_data)
        storage->RangeSelected = selected ? 1 : 0;

    if (g.NavId == id)
    {
        storage->NavIdItem = item_data;
        storage->NavIdSelected = selected ? 1 : 0;
    }
    if (storage->NavIdItem == item_data)
        ms->NavIdPassedBy = true;
    ms->LastSubmittedItem = item_data;

    *p_selected = selected;
    *p_pressed = pressed;
}

void Az::MultiSelectAddSetAll(AzMultiSelectTempData* ms, bool selected)
{
    AzSelectionRequest req = {AzSelectionRequestType_SetAll,
                              selected,
                              0,
                              AzSelectionUserData_Invalid,
                              AzSelectionUserData_Invalid};
    ms->IO.Requests.resize(0);
    ms->IO.Requests.push_back(req);
}

void Az::MultiSelectAddSetRange(AzMultiSelectTempData* ms,
                                bool selected,
                                int range_dir,
                                AzSelectionUserData first_item,
                                AzSelectionUserData last_item)
{
    if (ms->IO.Requests.Size > 0 && first_item == last_item
        && (ms->Flags & AzMultiSelectFlags_NoRangeSelect) == 0)
    {
        AzSelectionRequest* prev =
                &ms->IO.Requests.Data[ms->IO.Requests.Size - 1];
        if (prev->Type == AzSelectionRequestType_SetRange
            && prev->RangeLastItem == ms->LastSubmittedItem
            && prev->Selected == selected)
        {
            prev->RangeLastItem = last_item;
            return;
        }
    }

    AzSelectionRequest req = {AzSelectionRequestType_SetRange,
                              selected,
                              (AzS8)range_dir,
                              (range_dir > 0) ? first_item : last_item,
                              (range_dir > 0) ? last_item : first_item};
    ms->IO.Requests.push_back(req);
}

void Az::DebugNodeMultiSelectState(AzMultiSelectState* storage)
{
#ifndef AZ_DISABLE_DEBUG_TOOLS
    const bool is_active = (storage->LastFrameActive >= GetFrameCount() - 2);
    if (!is_active)
    {
        PushStyleColor(AzCol_Text, GetStyleColorVec4(AzCol_TextDisabled));
    }
    bool open = TreeNode((void*)(intptr_t)storage->ID,
                         "MultiSelect 0x%08X in '%s'%s",
                         storage->ID,
                         storage->Window ? storage->Window->Name : "N/A",
                         is_active ? "" : " *Inactive*");
    if (!is_active)
    {
        PopStyleColor();
    }
    if (!open)
        return;
    Text("RangeSrcItem = %" AZ_PRId64 " (0x%" AZ_PRIX64 "), RangeSelected = %d",
         storage->RangeSrcItem,
         storage->RangeSrcItem,
         storage->RangeSelected);
    Text("NavIdItem = %" AZ_PRId64 " (0x%" AZ_PRIX64 "), NavIdSelected = %d",
         storage->NavIdItem,
         storage->NavIdItem,
         storage->NavIdSelected);
    Text("LastSelectionSize = %d", storage->LastSelectionSize);
    TreePop();
#else
    AZ_UNUSED(storage);
#endif
}

AzSelectionBasicStorage::AzSelectionBasicStorage()
{
    Size = 0;
    PreserveOrder = false;
    UserData = NULL;
    AdapterIndexToStorageId = [](AzSelectionBasicStorage*, int idx) {
        return (AzID)idx;
    };
    _SelectionOrder = 1;
}

void AzSelectionBasicStorage::Clear()
{
    Size = 0;
    _SelectionOrder = 1;
    _Storage.Data.resize(0);
}

void AzSelectionBasicStorage::Swap(AzSelectionBasicStorage& r)
{
    AzSwap(Size, r.Size);
    AzSwap(_SelectionOrder, r._SelectionOrder);
    _Storage.Data.swap(r._Storage.Data);
}

bool AzSelectionBasicStorage::Contains(AzID id) const
{
    return _Storage.GetInt(id, 0) != 0;
}

static int AZ_CDECL PairComparerByValueInt(const void* lhs, const void* rhs)
{
    int lhs_v = ((const AzStoragePair*)lhs)->val_i;
    int rhs_v = ((const AzStoragePair*)rhs)->val_i;
    return (lhs_v > rhs_v ? +1 : lhs_v < rhs_v ? -1 : 0);
}

bool AzSelectionBasicStorage::GetNextSelectedItem(void** opaque_it,
                                                  AzID* out_id)
{
    AzStoragePair* it = (AzStoragePair*)*opaque_it;
    AzStoragePair* it_end = _Storage.Data.Data + _Storage.Data.Size;
    if (PreserveOrder && it == NULL && it_end != NULL)
        AzQsort(_Storage.Data.Data,
                (size_t)_Storage.Data.Size,
                sizeof(AzStoragePair),
                PairComparerByValueInt);
    if (it == NULL)
        it = _Storage.Data.Data;
    AZ_ASSERT(it >= _Storage.Data.Data && it <= it_end);
    if (it != it_end)
        while (it->val_i == 0 && it < it_end)
            it++;
    const bool has_more = (it != it_end);
    *opaque_it = has_more ? (void**)(it + 1) : (void**)(it);
    *out_id = has_more ? it->key : 0;
    if (PreserveOrder && !has_more)
        _Storage.BuildSortByKey();
    return has_more;
}

void AzSelectionBasicStorage::SetItemSelected(AzID id, bool selected)
{
    int* p_int = _Storage.GetIntRef(id, 0);
    if (selected && *p_int == 0)
    {
        *p_int = _SelectionOrder++;
        Size++;
    } else if (!selected && *p_int != 0)
    {
        *p_int = 0;
        Size--;
    }
}

static void AzSelectionBasicStorage_BatchSetItemSelected(
        AzSelectionBasicStorage* selection,
        AzID id,
        bool selected,
        int size_before_amends,
        int selection_order)
{
    AzStorage* storage = &selection->_Storage;
    AzStoragePair* it = AzLowerBound(storage->Data.Data,
                                     storage->Data.Data + size_before_amends,
                                     id);
    const bool is_contained = (it != storage->Data.Data + size_before_amends)
                              && (it->key == id);
    if (selected == (is_contained && it->val_i != 0))
        return;
    if (selected && !is_contained)
        storage->Data.push_back(AzStoragePair(id, selection_order));
    else if (is_contained)
        it->val_i = selected ? selection_order : 0;
    selection->Size += selected ? +1 : -1;
}

static void AzSelectionBasicStorage_BatchFinish(
        AzSelectionBasicStorage* selection,
        bool selected,
        int size_before_amends)
{
    AzStorage* storage = &selection->_Storage;
    if (selected && selection->Size != size_before_amends)
        storage->BuildSortByKey();
}

void AzSelectionBasicStorage::ApplyRequests(AzMultiSelectIO* ms_io)
{
    AZ_ASSERT(ms_io->ItemsCount != -1
              && "Missing value for items_count in BeginMultiSelect() call!");
    AZ_ASSERT(AdapterIndexToStorageId != NULL);

    for (AzSelectionRequest& req : ms_io->Requests)
    {
        if (req.Type == AzSelectionRequestType_SetAll)
        {
            Clear();
            if (req.Selected)
            {
                _Storage.Data.reserve(ms_io->ItemsCount);
                const int size_before_amends = _Storage.Data.Size;
                for (int idx = 0; idx < ms_io->ItemsCount;
                     idx++, _SelectionOrder++)
                    AzSelectionBasicStorage_BatchSetItemSelected(
                            this,
                            GetStorageIdFromIndex(idx),
                            req.Selected,
                            size_before_amends,
                            _SelectionOrder);
                AzSelectionBasicStorage_BatchFinish(this,
                                                    req.Selected,
                                                    size_before_amends);
            }
        } else if (req.Type == AzSelectionRequestType_SetRange)
        {
            const int selection_changes = (int)req.RangeLastItem
                                          - (int)req.RangeFirstItem + 1;

            if (selection_changes == 1 || (selection_changes < Size / 100))
            {
                for (int idx = (int)req.RangeFirstItem;
                     idx <= (int)req.RangeLastItem;
                     idx++)
                    SetItemSelected(GetStorageIdFromIndex(idx), req.Selected);
            } else
            {
                const int size_before_amends = _Storage.Data.Size;
                int selection_order = _SelectionOrder
                                      + ((req.RangeDirection < 0)
                                                 ? selection_changes - 1
                                                 : 0);
                for (int idx = (int)req.RangeFirstItem;
                     idx <= (int)req.RangeLastItem;
                     idx++, selection_order += req.RangeDirection)
                    AzSelectionBasicStorage_BatchSetItemSelected(
                            this,
                            GetStorageIdFromIndex(idx),
                            req.Selected,
                            size_before_amends,
                            selection_order);
                if (req.Selected)
                    _SelectionOrder += selection_changes;
                AzSelectionBasicStorage_BatchFinish(this,
                                                    req.Selected,
                                                    size_before_amends);
            }
        }
    }
}

AzSelectionExternalStorage::AzSelectionExternalStorage()
{
    UserData = NULL;
    AdapterSetItemSelected = NULL;
}

void AzSelectionExternalStorage::ApplyRequests(AzMultiSelectIO* ms_io)
{
    AZ_ASSERT(AdapterSetItemSelected);
    for (AzSelectionRequest& req : ms_io->Requests)
    {
        if (req.Type == AzSelectionRequestType_SetAll)
            for (int idx = 0; idx < ms_io->ItemsCount; idx++)
                AdapterSetItemSelected(this, idx, req.Selected);
        if (req.Type == AzSelectionRequestType_SetRange)
            for (int idx = (int)req.RangeFirstItem;
                 idx <= (int)req.RangeLastItem;
                 idx++)
                AdapterSetItemSelected(this, idx, req.Selected);
    }
}

bool Az::BeginListBox(const char* label, const AzVec2& size_arg)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const AzStyle& style = g.Style;
    const AzID id = GetID(label);
    const AzVec2 label_size = CalcTextSize(label, NULL, true);

    AzVec2 size = AzTrunc(CalcItemSize(size_arg,
                                       CalcItemWidth(),
                                       GetTextLineHeightWithSpacing() * 7.25f
                                               + style.FramePadding.y * 2.0f));
    AzVec2 frame_size = AzVec2(size.x, AzMax(size.y, label_size.y));
    AzRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
    AzRect bb(frame_bb.Min,
              frame_bb.Max
                      + AzVec2(label_size.x > 0.0f
                                       ? style.ItemInnerSpacing.x + label_size.x
                                       : 0.0f,
                               0.0f));
    g.NextItemData.ClearFlags();

    if (!IsRectVisible(bb.Min, bb.Max))
    {
        ItemSize(bb.GetSize(), style.FramePadding.y);
        ItemAdd(bb, 0, &frame_bb);
        g.NextWindowData.ClearFlags();
        return false;
    }

    BeginGroup();
    if (label_size.x > 0.0f)
    {
        AzVec2 label_pos = AzVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                                  frame_bb.Min.y + style.FramePadding.y);
        RenderText(label_pos, label);
        window->DC.CursorMaxPos = AzMax(window->DC.CursorMaxPos,
                                        label_pos + label_size);
        AlignTextToFramePadding();
    }

    BeginChild(id, frame_bb.GetSize(), AzChildFlags_FrameStyle);
    return true;
}

void Az::EndListBox()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT((window->Flags & AzWindowFlags_ChildWindow) && "Mismatched BeginListBox/EndListBox calls. Did you test the return value of BeginListBox?");
    AZ_UNUSED(window);

    EndChild();
    EndGroup();
}

bool Az::ListBox(const char* label,
                 int* current_item,
                 const char* const items[],
                 int items_count,
                 int height_items)
{
    const bool value_changed = ListBox(label,
                                       current_item,
                                       Items_ArrayGetter,
                                       (void*)items,
                                       items_count,
                                       height_items);
    return value_changed;
}

bool Az::ListBox(const char* label,
                 int* current_item,
                 const char* (*getter)(void* user_data, int idx),
                 void* user_data,
                 int items_count,
                 int height_in_items)
{
    AzContext& g = *GAz;

    if (height_in_items < 0)
        height_in_items = AzMin(items_count, 7);
    float height_in_items_f = height_in_items + 0.25f;
    AzVec2 size(0.0f,
                AzTrunc(GetTextLineHeightWithSpacing() * height_in_items_f
                        + g.Style.FramePadding.y * 2.0f));

    if (!BeginListBox(label, size))
        return false;

    bool value_changed = false;
    AzListClipper clipper;
    clipper.Begin(items_count, GetTextLineHeightWithSpacing());
    clipper.IncludeItemByIndex(*current_item);
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const char* item_text = getter(user_data, i);
            if (item_text == NULL)
                item_text = "*Unknown item*";

            PushID(i);
            const bool item_selected = (i == *current_item);
            if (Selectable(item_text, item_selected))
            {
                *current_item = i;
                value_changed = true;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }
    EndListBox();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

int Az::PlotEx(AzPlotType plot_type,
               const char* label,
               float (*values_getter)(void* data, int idx),
               void* data,
               int values_count,
               int values_offset,
               const char* overlay_text,
               float scale_min,
               float scale_max,
               const AzVec2& size_arg)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return -1;

    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);

    const AzVec2 label_size = CalcTextSize(label, NULL, true);
    const AzVec2 frame_size =
            CalcItemSize(size_arg,
                         CalcItemWidth(),
                         label_size.y + style.FramePadding.y * 2.0f);

    const AzRect frame_bb(window->DC.CursorPos,
                          window->DC.CursorPos + frame_size);
    const AzRect inner_bb(frame_bb.Min + style.FramePadding,
                          frame_bb.Max - style.FramePadding);
    const AzRect total_bb(frame_bb.Min,
                          frame_bb.Max
                                  + AzVec2(label_size.x > 0.0f
                                                   ? style.ItemInnerSpacing.x
                                                             + label_size.x
                                                   : 0.0f,
                                           0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, AzItemFlags_NoNav))
        return -1;
    bool hovered;
    ButtonBehavior(frame_bb, id, &hovered, NULL);

    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data, i);
            if (v != v)
                continue;
            v_min = AzMin(v_min, v);
            v_max = AzMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    RenderFrame(frame_bb.Min,
                frame_bb.Max,
                GetColorU32(AzCol_FrameBg),
                true,
                style.FrameRounding);

    const int values_count_min = (plot_type == AzPlotType_Lines) ? 2 : 1;
    int idx_hovered = -1;
    if (values_count >= values_count_min)
    {
        int res_w = AzMin((int)frame_size.x, values_count)
                    + ((plot_type == AzPlotType_Lines) ? -1 : 0);
        int item_count = values_count
                         + ((plot_type == AzPlotType_Lines) ? -1 : 0);

        if (hovered && inner_bb.Contains(g.IO.MousePos))
        {
            const float t = AzClamp((g.IO.MousePos.x - inner_bb.Min.x)
                                            / (inner_bb.Max.x - inner_bb.Min.x),
                                    0.0f,
                                    0.9999f);
            const int v_idx = (int)(t * item_count);
            AZ_ASSERT(v_idx >= 0 && v_idx < values_count);

            const float v0 = values_getter(data,
                                           (v_idx + values_offset)
                                                   % values_count);
            const float v1 = values_getter(data,
                                           (v_idx + 1 + values_offset)
                                                   % values_count);
            if (plot_type == AzPlotType_Lines)
                SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx + 1, v1);
            else if (plot_type == AzPlotType_Histogram)
                SetTooltip("%d: %8.4g", v_idx, v0);
            idx_hovered = v_idx;
        }

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max)
                                        ? 0.0f
                                        : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        AzVec2 tp0 = AzVec2(t0,
                            1.0f - AzSaturate((v0 - scale_min) * inv_scale));
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f)
                                              ? (1 + scale_min * inv_scale)
                                              : (scale_min < 0.0f ? 0.0f
                                                                  : 1.0f);

        const AzU32 col_base = GetColorU32((plot_type == AzPlotType_Lines)
                                                   ? AzCol_PlotLines
                                                   : AzCol_PlotHistogram);
        const AzU32 col_hovered = GetColorU32(
                (plot_type == AzPlotType_Lines) ? AzCol_PlotLinesHovered
                                                : AzCol_PlotHistogramHovered);

        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            AZ_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data,
                                           (v1_idx + values_offset + 1)
                                                   % values_count);
            const AzVec2 tp1 =
                    AzVec2(t1, 1.0f - AzSaturate((v1 - scale_min) * inv_scale));

            AzVec2 pos0 = AzLerp(inner_bb.Min, inner_bb.Max, tp0);
            AzVec2 pos1 = AzLerp(inner_bb.Min,
                                 inner_bb.Max,
                                 (plot_type == AzPlotType_Lines)
                                         ? tp1
                                         : AzVec2(tp1.x,
                                                  histogram_zero_line_t));
            if (plot_type == AzPlotType_Lines)
            {
                window->DrawList->AddLine(pos0,
                                          pos1,
                                          idx_hovered == v1_idx ? col_hovered
                                                                : col_base);
            } else if (plot_type == AzPlotType_Histogram)
            {
                if (pos1.x >= pos0.x + 2.0f)
                    pos1.x -= 1.0f;
                window->DrawList->AddRectFilled(pos0,
                                                pos1,
                                                idx_hovered == v1_idx
                                                        ? col_hovered
                                                        : col_base);
            }

            t0 = t1;
            tp0 = tp1;
        }
    }

    if (overlay_text)
        RenderTextClipped(AzVec2(frame_bb.Min.x,
                                 frame_bb.Min.y + style.FramePadding.y),
                          frame_bb.Max,
                          overlay_text,
                          NULL,
                          NULL,
                          AzVec2(0.5f, 0.0f));

    if (label_size.x > 0.0f)
        RenderText(AzVec2(frame_bb.Max.x + style.ItemInnerSpacing.x,
                          inner_bb.Min.y),
                   label);

    return idx_hovered;
}

struct AzPlotArrayGetterData
{
    const float* Values;
    int Stride;

    AzPlotArrayGetterData(const float* values, int stride)
    {
        Values = values;
        Stride = stride;
    }
};

static float Plot_ArrayGetter(void* data, int idx)
{
    AzPlotArrayGetterData* plot_data = (AzPlotArrayGetterData*)data;
    const float v = *(
            const float*)(const void*)((const unsigned char*)plot_data->Values
                                       + (size_t)idx * plot_data->Stride);
    return v;
}

void Az::PlotLines(const char* label,
                   const float* values,
                   int values_count,
                   int values_offset,
                   const char* overlay_text,
                   float scale_min,
                   float scale_max,
                   AzVec2 graph_size,
                   int stride)
{
    AzPlotArrayGetterData data(values, stride);
    PlotEx(AzPlotType_Lines,
           label,
           &Plot_ArrayGetter,
           (void*)&data,
           values_count,
           values_offset,
           overlay_text,
           scale_min,
           scale_max,
           graph_size);
}

void Az::PlotLines(const char* label,
                   float (*values_getter)(void* data, int idx),
                   void* data,
                   int values_count,
                   int values_offset,
                   const char* overlay_text,
                   float scale_min,
                   float scale_max,
                   AzVec2 graph_size)
{
    PlotEx(AzPlotType_Lines,
           label,
           values_getter,
           data,
           values_count,
           values_offset,
           overlay_text,
           scale_min,
           scale_max,
           graph_size);
}

void Az::PlotHistogram(const char* label,
                       const float* values,
                       int values_count,
                       int values_offset,
                       const char* overlay_text,
                       float scale_min,
                       float scale_max,
                       AzVec2 graph_size,
                       int stride)
{
    AzPlotArrayGetterData data(values, stride);
    PlotEx(AzPlotType_Histogram,
           label,
           &Plot_ArrayGetter,
           (void*)&data,
           values_count,
           values_offset,
           overlay_text,
           scale_min,
           scale_max,
           graph_size);
}

void Az::PlotHistogram(const char* label,
                       float (*values_getter)(void* data, int idx),
                       void* data,
                       int values_count,
                       int values_offset,
                       const char* overlay_text,
                       float scale_min,
                       float scale_max,
                       AzVec2 graph_size)
{
    PlotEx(AzPlotType_Histogram,
           label,
           values_getter,
           data,
           values_count,
           values_offset,
           overlay_text,
           scale_min,
           scale_max,
           graph_size);
}

void Az::Value(const char* prefix, bool b)
{
    Text("%s: %s", prefix, (b ? "true" : "false"));
}

void Az::Value(const char* prefix, int v)
{
    Text("%s: %d", prefix, v);
}

void Az::Value(const char* prefix, unsigned int v)
{
    Text("%s: %d", prefix, v);
}

void Az::Value(const char* prefix, float v, const char* float_format)
{
    if (float_format)
    {
        char fmt[64];
        AzFormatString(fmt, AZ_ARRAYSIZE(fmt), "%%s: %s", float_format);
        Text(fmt, prefix, v);
    } else
    {
        Text("%s: %.3f", prefix, v);
    }
}

void AzMenuColumns::Update(float spacing, bool window_reappearing)
{
    if (window_reappearing)
        memset(Widths, 0, sizeof(Widths));
    Spacing = (AzU16)spacing;
    CalcNextTotalWidth(true);
    memset(Widths, 0, sizeof(Widths));
    TotalWidth = NextTotalWidth;
    NextTotalWidth = 0;
}

void AzMenuColumns::CalcNextTotalWidth(bool update_offsets)
{
    AzU16 offset = 0;
    bool want_spacing = false;
    for (int i = 0; i < AZ_ARRAYSIZE(Widths); i++)
    {
        AzU16 width = Widths[i];
        if (want_spacing && width > 0)
            offset += Spacing;
        want_spacing |= (width > 0);
        if (update_offsets)
        {
            if (i == 1)
            {
                OffsetLabel = offset;
            }
            if (i == 2)
            {
                OffsetShortcut = offset;
            }
            if (i == 3)
            {
                OffsetMark = offset;
            }
        }
        offset += width;
    }
    NextTotalWidth = offset;
}

float AzMenuColumns::DeclColumns(float w_icon,
                                 float w_label,
                                 float w_shortcut,
                                 float w_mark)
{
    Widths[0] = AzMax(Widths[0], (AzU16)w_icon);
    Widths[1] = AzMax(Widths[1], (AzU16)w_label);
    Widths[2] = AzMax(Widths[2], (AzU16)w_shortcut);
    Widths[3] = AzMax(Widths[3], (AzU16)w_mark);
    CalcNextTotalWidth(false);
    return (float)AzMax(TotalWidth, NextTotalWidth);
}

bool Az::BeginMenuBar()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (!(window->Flags & AzWindowFlags_MenuBar))
        return false;

    AZ_ASSERT(!window->DC.MenuBarAppending);
    BeginGroup();
    PushID("##MenuBar");

    const float border_top = AzMax(AZ_ROUND(window->WindowBorderSize * 0.5f
                                            - window->TitleBarHeight),
                                   0.0f);
    const float border_half = AZ_ROUND(window->WindowBorderSize * 0.5f);
    AzRect bar_rect = window->MenuBarRect();
    AzRect clip_rect(AzFloor(bar_rect.Min.x + border_half),
                     AzFloor(bar_rect.Min.y + border_top),
                     AzFloor(AzMax(bar_rect.Min.x,
                                   bar_rect.Max.x
                                           - AzMax(window->WindowRounding,
                                                   border_half))),
                     AzFloor(bar_rect.Max.y));
    clip_rect.ClipWith(window->OuterRectClipped);
    PushClipRect(clip_rect.Min, clip_rect.Max, false);

    window->DC.CursorPos = window->DC.CursorMaxPos =
            AzVec2(bar_rect.Min.x + window->DC.MenuBarOffset.x,
                   bar_rect.Min.y + window->DC.MenuBarOffset.y);
    window->DC.LayoutType = AzLayoutType_Horizontal;
    window->DC.IsSameLine = false;
    window->DC.NavLayerCurrent = AzNavLayer_Menu;
    window->DC.MenuBarAppending = true;
    AlignTextToFramePadding();
    return true;
}

void Az::EndMenuBar()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    AzContext& g = *GAz;

    AZ_MSVC_WARNING_SUPPRESS(6011);
    AZ_ASSERT(window->Flags & AzWindowFlags_MenuBar);
    AZ_ASSERT(window->DC.MenuBarAppending);

    if (NavMoveRequestButNoResultYet()
        && (g.NavMoveDir == AzDir_Left || g.NavMoveDir == AzDir_Right)
        && (g.NavWindow->Flags & AzWindowFlags_ChildMenu))
    {
        AzWindow* nav_earliest_child = g.NavWindow;
        while (nav_earliest_child->ParentWindow
               && (nav_earliest_child->ParentWindow->Flags
                   & AzWindowFlags_ChildMenu))
            nav_earliest_child = nav_earliest_child->ParentWindow;
        if (nav_earliest_child->ParentWindow == window
            && nav_earliest_child->DC.ParentLayoutType
                       == AzLayoutType_Horizontal
            && (g.NavMoveFlags & AzNavMoveFlags_Forwarded) == 0)
        {
            const AzNavLayer layer = AzNavLayer_Menu;
            AZ_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer));
            FocusWindow(window);
            SetNavID(window->NavLastIds[layer],
                     layer,
                     0,
                     window->NavRectRel[layer]);

            if (g.NavCursorVisible)
            {
                g.NavCursorVisible = false;
                g.NavCursorHideFrames = 2;
            }
            g.NavHighlightItemUnderNav = g.NavMousePosDirty = true;
            NavMoveRequestForward(g.NavMoveDir,
                                  g.NavMoveClipDir,
                                  g.NavMoveFlags,
                                  g.NavMoveScrollFlags);
        }
    }

    PopClipRect();
    PopID();
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x;

    AzGroupData& group_data = g.GroupStack.back();
    group_data.EmitItem = false;
    AzVec2 restore_cursor_max_pos = group_data.BackupCursorMaxPos;
    window->DC.IdealMaxPos.x = AzMax(window->DC.IdealMaxPos.x,
                                     window->DC.CursorMaxPos.x
                                             - window->Scroll.x);
    EndGroup();
    window->DC.LayoutType = AzLayoutType_Vertical;
    window->DC.IsSameLine = false;
    window->DC.NavLayerCurrent = AzNavLayer_Main;
    window->DC.MenuBarAppending = false;
    window->DC.CursorMaxPos = restore_cursor_max_pos;
}

bool Az::BeginViewportSideBar(const char* name,
                              AzViewport* viewport_p,
                              AzDir dir,
                              float axis_size,
                              AzWindowFlags window_flags)
{
    AZ_ASSERT(dir != AzDir_None);

    AzWindow* bar_window = FindWindowByName(name);
    AzViewportP* viewport = (AzViewportP*)(void*)(viewport_p
                                                          ? viewport_p
                                                          : GetMainViewport());
    if (bar_window == NULL || bar_window->BeginCount == 0)
    {
        AzRect avail_rect = viewport->GetBuildWorkRect();
        AzAxis axis = (dir == AzDir_Up || dir == AzDir_Down) ? AzAxis_Y
                                                             : AzAxis_X;
        AzVec2 pos = avail_rect.Min;
        if (dir == AzDir_Right || dir == AzDir_Down)
            pos[axis] = avail_rect.Max[axis] - axis_size;
        AzVec2 size = avail_rect.GetSize();
        size[axis] = axis_size;
        SetNextWindowPos(pos);
        SetNextWindowSize(size);

        if (dir == AzDir_Up || dir == AzDir_Left)
            viewport->BuildWorkInsetMin[axis] += axis_size;
        else if (dir == AzDir_Down || dir == AzDir_Right)
            viewport->BuildWorkInsetMax[axis] += axis_size;
    }

    window_flags |= AzWindowFlags_NoTitleBar | AzWindowFlags_NoResize
                    | AzWindowFlags_NoMove | AzWindowFlags_NoDocking;
    SetNextWindowViewport(viewport->ID);
    PushStyleVar(AzStyleVar_WindowRounding, 0.0f);
    PushStyleVar(AzStyleVar_WindowMinSize, AzVec2(0, 0));
    bool is_open = Begin(name, NULL, window_flags);
    PopStyleVar(2);

    return is_open;
}

bool Az::BeginMainMenuBar()
{
    AzContext& g = *GAz;
    AzViewportP* viewport = (AzViewportP*)(void*)GetMainViewport();

    SetCurrentViewport(NULL, viewport);

    g.NextWindowData.MenuBarOffsetMinVal = AzVec2(
            g.Style.DisplaySafeAreaPadding.x,
            AzMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y,
                  0.0f));
    AzWindowFlags window_flags = AzWindowFlags_NoScrollbar
                                 | AzWindowFlags_NoSavedSettings
                                 | AzWindowFlags_MenuBar;
    float height = GetFrameHeight();
    bool is_open = BeginViewportSideBar("##MainMenuBar",
                                        viewport,
                                        AzDir_Up,
                                        height,
                                        window_flags);
    g.NextWindowData.MenuBarOffsetMinVal = AzVec2(0.0f, 0.0f);
    if (!is_open)
    {
        End();
        return false;
    }

    g.CurrentWindow->Flags &= ~AzWindowFlags_NoSavedSettings;
    BeginMenuBar();
    return is_open;
}

void Az::EndMainMenuBar()
{
    AzContext& g = *GAz;
    if (!g.CurrentWindow->DC.MenuBarAppending)
    {
        AZ_ASSERT_USER_ERROR(0,
                             "Calling EndMainMenuBar() not from a menu-bar!");
        return;
    }

    EndMenuBar();
    g.CurrentWindow->Flags |= AzWindowFlags_NoSavedSettings;

    if (g.CurrentWindow == g.NavWindow && g.NavLayer == AzNavLayer_Main
        && !g.NavAnyRequest && g.ActiveId == 0)
        FocusTopMostWindowUnderOne(
                g.NavWindow,
                NULL,
                NULL,
                AzFocusRequestFlags_UnlessBelowModal
                        | AzFocusRequestFlags_RestoreFocusedChild);

    End();
}

static bool IsRootOfOpenMenuSet()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if ((g.OpenPopupStack.Size <= g.BeginPopupStack.Size)
        || (window->Flags & AzWindowFlags_ChildMenu))
        return false;

    const AzPopupData* upper_popup = &g.OpenPopupStack[g.BeginPopupStack.Size];
    if (window->DC.NavLayerCurrent != upper_popup->ParentNavLayer)
        return false;
    return upper_popup->Window
           && (upper_popup->Window->Flags & AzWindowFlags_ChildMenu)
           && Az::IsWindowChildOf(upper_popup->Window, window, true, false);
}

bool Az::BeginMenuEx(const char* label, const char* icon, bool enabled)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    const AzStyle& style = g.Style;
    const AzID id = window->GetID(label);
    bool menu_is_open = IsPopupOpen(id, AzPopupFlags_None);

    AzWindowFlags window_flags = AzWindowFlags_ChildMenu
                                 | AzWindowFlags_AlwaysAutoResize
                                 | AzWindowFlags_NoMove
                                 | AzWindowFlags_NoTitleBar
                                 | AzWindowFlags_NoSavedSettings
                                 | AzWindowFlags_NoNavFocus;
    if (window->Flags & AzWindowFlags_ChildMenu)
        window_flags |= AzWindowFlags_ChildWindow;

    if (g.MenusIdSubmittedThisFrame.contains(id))
    {
        if (menu_is_open)
            menu_is_open = BeginPopupMenuEx(id, label, window_flags);
        else
            g.NextWindowData.ClearFlags();
        return menu_is_open;
    }

    g.MenusIdSubmittedThisFrame.push_back(id);

    AzVec2 label_size = CalcTextSize(label, NULL, true);

    const bool menuset_is_open = IsRootOfOpenMenuSet();
    if (menuset_is_open)
        PushItemFlag(AzItemFlags_NoWindowHoverableCheck, true);

    AzVec2 popup_pos, pos = window->DC.CursorPos;
    PushID(label);
    if (!enabled)
        BeginDisabled();
    const AzMenuColumns* offsets = &window->DC.MenuColumns;
    bool pressed;

    const AzSelectableFlags selectable_flags =
            AzSelectableFlags_NoHoldingActiveID
            | AzSelectableFlags_NoSetKeyOwner | AzSelectableFlags_SelectOnClick
            | AzSelectableFlags_NoAutoClosePopups;
    if (window->DC.LayoutType == AzLayoutType_Horizontal)
    {
        popup_pos = AzVec2(pos.x - 1.0f - AZ_TRUNC(style.ItemSpacing.x * 0.5f),
                           pos.y - style.FramePadding.y
                                   + window->MenuBarHeight);
        window->DC.CursorPos.x += AZ_TRUNC(style.ItemSpacing.x * 0.5f);
        PushStyleVarX(AzStyleVar_ItemSpacing, style.ItemSpacing.x * 2.0f);
        float w = label_size.x;
        AzVec2 text_pos(window->DC.CursorPos.x + offsets->OffsetLabel,
                        window->DC.CursorPos.y
                                + window->DC.CurrLineTextBaseOffset);
        pressed = Selectable("",
                             menu_is_open,
                             selectable_flags,
                             AzVec2(w, label_size.y));
        LogSetNextTextDecoration("[", "]");
        RenderText(text_pos, label);
        PopStyleVar();
        window->DC.CursorPos.x += AZ_TRUNC(style.ItemSpacing.x
                                           * (-1.0f + 0.5f));
    } else
    {
        popup_pos = AzVec2(pos.x, pos.y - style.WindowPadding.y);
        float icon_w = (icon && icon[0]) ? CalcTextSize(icon, NULL).x : 0.0f;
        float checkmark_w = AZ_TRUNC(g.FontSize * 1.20f);
        float min_w = window->DC.MenuColumns.DeclColumns(icon_w,
                                                         label_size.x,
                                                         0.0f,
                                                         checkmark_w);
        float extra_w = AzMax(0.0f, GetContentRegionAvail().x - min_w);
        AzVec2 text_pos(window->DC.CursorPos.x + offsets->OffsetLabel,
                        window->DC.CursorPos.y
                                + window->DC.CurrLineTextBaseOffset);
        pressed = Selectable("",
                             menu_is_open,
                             selectable_flags
                                     | AzSelectableFlags_SpanAvailWidth,
                             AzVec2(min_w, label_size.y));
        LogSetNextTextDecoration("", ">");
        RenderText(text_pos, label);
        if (icon_w > 0.0f)
            RenderText(pos + AzVec2(offsets->OffsetIcon, 0.0f), icon);
        RenderArrow(window->DrawList,
                    pos
                            + AzVec2(offsets->OffsetMark + extra_w
                                             + g.FontSize * 0.30f,
                                     0.0f),
                    GetColorU32(AzCol_Text),
                    AzDir_Right);
    }
    if (!enabled)
        EndDisabled();

    const bool hovered = (g.HoveredId == id) && enabled
                         && !g.NavHighlightItemUnderNav;
    if (menuset_is_open)
        PopItemFlag();

    bool want_open = false;
    bool want_open_nav_init = false;
    bool want_close = false;
    if (window->DC.LayoutType == AzLayoutType_Vertical)
    {
        bool moving_toward_child_menu = false;
        AzPopupData* child_popup =
                (g.BeginPopupStack.Size < g.OpenPopupStack.Size)
                        ? &g.OpenPopupStack[g.BeginPopupStack.Size]
                        : NULL;
        AzWindow* child_menu_window = (child_popup && child_popup->Window
                                       && child_popup->Window->ParentWindow
                                                  == window)
                                              ? child_popup->Window
                                              : NULL;
        if (g.HoveredWindow == window && child_menu_window != NULL)
        {
            const float ref_unit = g.FontSize;
            const float child_dir = (window->Pos.x < child_menu_window->Pos.x)
                                            ? 1.0f
                                            : -1.0f;
            const AzRect next_window_rect = child_menu_window->Rect();
            AzVec2 ta = (g.IO.MousePos - g.IO.MouseDelta);
            AzVec2 tb = (child_dir > 0.0f) ? next_window_rect.GetTL()
                                           : next_window_rect.GetTR();
            AzVec2 tc = (child_dir > 0.0f) ? next_window_rect.GetBL()
                                           : next_window_rect.GetBR();
            const float pad_farmost_h = AzClamp(AzFabs(ta.x - tb.x) * 0.30f,
                                                ref_unit * 0.5f,
                                                ref_unit * 2.5f);
            ta.x += child_dir * -0.5f;
            tb.x += child_dir * ref_unit;
            tc.x += child_dir * ref_unit;
            tb.y = ta.y
                   + AzMax((tb.y - pad_farmost_h) - ta.y, -ref_unit * 8.0f);
            tc.y = ta.y
                   + AzMin((tc.y + pad_farmost_h) - ta.y, +ref_unit * 8.0f);
            moving_toward_child_menu =
                    AzTriangleContainsPoint(ta, tb, tc, g.IO.MousePos);
        }

        if (menu_is_open && !hovered && g.HoveredWindow == window
            && !moving_toward_child_menu && !g.NavHighlightItemUnderNav
            && g.ActiveId == 0)
            want_close = true;

        if (!menu_is_open && pressed)
            want_open = true;
        else if (!menu_is_open && hovered && !moving_toward_child_menu)
            want_open = true;
        else if (!menu_is_open && hovered && g.HoveredIdTimer >= 0.30f
                 && g.MouseStationaryTimer >= 0.30f)
            want_open = true;
        if (g.NavId == id && g.NavMoveDir == AzDir_Right)
        {
            want_open = want_open_nav_init = true;
            NavMoveRequestCancel();
            SetNavCursorVisibleAfterMove();
        }
    } else
    {
        if (menu_is_open && pressed && menuset_is_open)
        {
            want_close = true;
            want_open = menu_is_open = false;
        } else if (pressed || (hovered && menuset_is_open && !menu_is_open))
        {
            want_open = true;
        } else if (g.NavId == id && g.NavMoveDir == AzDir_Down)
        {
            want_open = true;
            NavMoveRequestCancel();
        }
    }

    if (!enabled)
        want_close = true;
    if (want_close && IsPopupOpen(id, AzPopupFlags_None))
        ClosePopupToLevel(g.BeginPopupStack.Size, true);

    AZ_TEST_ENGINE_ITEM_INFO(id,
                             label,
                             g.LastItemData.StatusFlags
                                     | AzItemStatusFlags_Openable
                                     | (menu_is_open ? AzItemStatusFlags_Opened
                                                     : 0));
    PopID();

    if (want_open && !menu_is_open
        && g.OpenPopupStack.Size > g.BeginPopupStack.Size)
    {
        OpenPopup(label);
    } else if (want_open)
    {
        menu_is_open = true;
        OpenPopup(label, AzPopupFlags_NoReopen);
    }

    if (menu_is_open)
    {
        AzLastItemData last_item_in_parent = g.LastItemData;
        SetNextWindowPos(popup_pos, AzCond_Always);
        PushStyleVar(AzStyleVar_ChildRounding, style.PopupRounding);
        menu_is_open = BeginPopupMenuEx(id, label, window_flags);
        PopStyleVar();
        if (menu_is_open)
        {
            if (want_open && want_open_nav_init && !g.NavInitRequest)
            {
                FocusWindow(g.CurrentWindow,
                            AzFocusRequestFlags_UnlessBelowModal);
                NavInitWindow(g.CurrentWindow, false);
            }

            g.LastItemData = last_item_in_parent;
            if (g.HoveredWindow == window)
                g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredWindow;
        }
    } else
    {
        g.NextWindowData.ClearFlags();
    }

    return menu_is_open;
}

bool Az::BeginMenu(const char* label, bool enabled)
{
    return BeginMenuEx(label, NULL, enabled);
}

void Az::EndMenu()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(window->Flags & AzWindowFlags_Popup);
    AzWindow* parent_window = window->ParentWindow;
    if (window->BeginCount == window->BeginCountPreviousFrame)
        if (g.NavMoveDir == AzDir_Left && NavMoveRequestButNoResultYet())
            if (g.NavWindow && (g.NavWindow->RootWindowForNav == window)
                && parent_window->DC.LayoutType == AzLayoutType_Vertical)
            {
                ClosePopupToLevel(g.BeginPopupStack.Size - 1, true);
                NavMoveRequestCancel();
            }

    EndPopup();
}

bool Az::MenuItemEx(const char* label,
                    const char* icon,
                    const char* shortcut,
                    bool selected,
                    bool enabled)
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    AzContext& g = *GAz;
    AzStyle& style = g.Style;
    AzVec2 pos = window->DC.CursorPos;
    AzVec2 label_size = CalcTextSize(label, NULL, true);

    const bool menuset_is_open = IsRootOfOpenMenuSet();
    if (menuset_is_open)
        PushItemFlag(AzItemFlags_NoWindowHoverableCheck, true);

    bool pressed;
    PushID(label);
    if (!enabled)
        BeginDisabled();

    const AzSelectableFlags selectable_flags =
            AzSelectableFlags_SelectOnRelease | AzSelectableFlags_NoSetKeyOwner
            | AzSelectableFlags_SetNavIdOnHover;
    const AzMenuColumns* offsets = &window->DC.MenuColumns;
    if (window->DC.LayoutType == AzLayoutType_Horizontal)
    {
        float w = label_size.x;
        window->DC.CursorPos.x += AZ_TRUNC(style.ItemSpacing.x * 0.5f);
        AzVec2 text_pos(window->DC.CursorPos.x + offsets->OffsetLabel,
                        window->DC.CursorPos.y
                                + window->DC.CurrLineTextBaseOffset);
        PushStyleVarX(AzStyleVar_ItemSpacing, style.ItemSpacing.x * 2.0f);
        pressed = Selectable("", selected, selectable_flags, AzVec2(w, 0.0f));
        PopStyleVar();
        if (g.LastItemData.StatusFlags & AzItemStatusFlags_Visible)
            RenderText(text_pos, label);
        window->DC.CursorPos.x += AZ_TRUNC(style.ItemSpacing.x
                                           * (-1.0f + 0.5f));
    } else
    {
        float icon_w = (icon && icon[0]) ? CalcTextSize(icon, NULL).x : 0.0f;
        float shortcut_w = (shortcut && shortcut[0])
                                   ? CalcTextSize(shortcut, NULL).x
                                   : 0.0f;
        float checkmark_w = AZ_TRUNC(g.FontSize * 1.20f);
        float min_w = window->DC.MenuColumns.DeclColumns(icon_w,
                                                         label_size.x,
                                                         shortcut_w,
                                                         checkmark_w);
        float stretch_w = AzMax(0.0f, GetContentRegionAvail().x - min_w);
        pressed = Selectable("",
                             false,
                             selectable_flags
                                     | AzSelectableFlags_SpanAvailWidth,
                             AzVec2(min_w, label_size.y));
        if (g.LastItemData.StatusFlags & AzItemStatusFlags_Visible)
        {
            RenderText(pos + AzVec2(offsets->OffsetLabel, 0.0f), label);
            if (icon_w > 0.0f)
                RenderText(pos + AzVec2(offsets->OffsetIcon, 0.0f), icon);
            if (shortcut_w > 0.0f)
            {
                PushStyleColor(AzCol_Text, style.Colors[AzCol_TextDisabled]);
                LogSetNextTextDecoration("(", ")");
                RenderText(pos
                                   + AzVec2(offsets->OffsetShortcut + stretch_w,
                                            0.0f),
                           shortcut,
                           NULL,
                           false);
                PopStyleColor();
            }
            if (selected)
                RenderCheckMark(window->DrawList,
                                pos
                                        + AzVec2(offsets->OffsetMark + stretch_w
                                                         + g.FontSize * 0.40f,
                                                 g.FontSize * 0.134f * 0.5f),
                                GetColorU32(AzCol_Text),
                                g.FontSize * 0.866f);
        }
    }
    AZ_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID,
                             label,
                             g.LastItemData.StatusFlags
                                     | AzItemStatusFlags_Checkable
                                     | (selected ? AzItemStatusFlags_Checked
                                                 : 0));
    if (!enabled)
        EndDisabled();
    PopID();
    if (menuset_is_open)
        PopItemFlag();

    return pressed;
}

bool Az::MenuItem(const char* label,
                  const char* shortcut,
                  bool selected,
                  bool enabled)
{
    return MenuItemEx(label, NULL, shortcut, selected, enabled);
}

bool Az::MenuItem(const char* label,
                  const char* shortcut,
                  bool* p_selected,
                  bool enabled)
{
    if (MenuItemEx(label,
                   NULL,
                   shortcut,
                   p_selected ? *p_selected : false,
                   enabled))
    {
        if (p_selected)
            *p_selected = !*p_selected;
        return true;
    }
    return false;
}

struct AzTabBarSection
{
    int TabCount;
    float Width;
    float Spacing;

    AzTabBarSection()
    {
        memset(this, 0, sizeof(*this));
    }
};

namespace Az
{
    static void TabBarLayout(AzTabBar* tab_bar);
    static AzU32 TabBarCalcTabID(AzTabBar* tab_bar,
                                 const char* label,
                                 AzWindow* docked_window);
    static float TabBarCalcMaxTabWidth();
    static float TabBarScrollClamp(AzTabBar* tab_bar, float scrolling);
    static void TabBarScrollToTab(AzTabBar* tab_bar,
                                  AzID tab_id,
                                  AzTabBarSection* sections);
    static AzTabItem* TabBarScrollingButtons(AzTabBar* tab_bar);
    static AzTabItem* TabBarTabListPopupButton(AzTabBar* tab_bar);
} // namespace Az

AzTabBar::AzTabBar()
{
    memset(this, 0, sizeof(*this));
    CurrFrameVisible = PrevFrameVisible = -1;
    LastTabItemIdx = -1;
}

static inline int TabItemGetSectionIdx(const AzTabItem* tab)
{
    return (tab->Flags & AzTabItemFlags_Leading)    ? 0
           : (tab->Flags & AzTabItemFlags_Trailing) ? 2
                                                    : 1;
}

static int AZ_CDECL TabItemComparerBySection(const void* lhs, const void* rhs)
{
    const AzTabItem* a = (const AzTabItem*)lhs;
    const AzTabItem* b = (const AzTabItem*)rhs;
    const int a_section = TabItemGetSectionIdx(a);
    const int b_section = TabItemGetSectionIdx(b);
    if (a_section != b_section)
        return a_section - b_section;
    return (int)(a->IndexDuringLayout - b->IndexDuringLayout);
}

static int AZ_CDECL TabItemComparerByBeginOrder(const void* lhs,
                                                const void* rhs)
{
    const AzTabItem* a = (const AzTabItem*)lhs;
    const AzTabItem* b = (const AzTabItem*)rhs;
    return (int)(a->BeginOrder - b->BeginOrder);
}

static AzTabBar* GetTabBarFromTabBarRef(const AzPtrOrIndex& ref)
{
    AzContext& g = *GAz;
    return ref.Ptr ? (AzTabBar*)ref.Ptr : g.TabBars.GetByIndex(ref.Index);
}

static AzPtrOrIndex GetTabBarRefFromTabBar(AzTabBar* tab_bar)
{
    AzContext& g = *GAz;
    if (g.TabBars.Contains(tab_bar))
        return AzPtrOrIndex(g.TabBars.GetIndex(tab_bar));
    return AzPtrOrIndex(tab_bar);
}

bool Az::BeginTabBar(const char* str_id, AzTabBarFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    AzID id = window->GetID(str_id);
    AzTabBar* tab_bar = g.TabBars.GetOrAddByKey(id);
    AzRect tab_bar_bb = AzRect(window->DC.CursorPos.x,
                               window->DC.CursorPos.y,
                               window->WorkRect.Max.x,
                               window->DC.CursorPos.y + g.FontSize
                                       + g.Style.FramePadding.y * 2);
    tab_bar->ID = id;
    tab_bar->SeparatorMinX = tab_bar->BarRect.Min.x
                             - AZ_TRUNC(window->WindowPadding.x * 0.5f);
    tab_bar->SeparatorMaxX = tab_bar->BarRect.Max.x
                             + AZ_TRUNC(window->WindowPadding.x * 0.5f);

    flags |= AzTabBarFlags_IsFocused;
    return BeginTabBarEx(tab_bar, tab_bar_bb, flags);
}

bool Az::BeginTabBarEx(AzTabBar* tab_bar,
                       const AzRect& tab_bar_bb,
                       AzTabBarFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    AZ_ASSERT(tab_bar->ID != 0);
    if ((flags & AzTabBarFlags_DockNode) == 0)
        PushOverrideID(tab_bar->ID);

    g.CurrentTabBarStack.push_back(GetTabBarRefFromTabBar(tab_bar));
    g.CurrentTabBar = tab_bar;
    tab_bar->Window = window;

    tab_bar->BackupCursorPos = window->DC.CursorPos;
    if (tab_bar->CurrFrameVisible == g.FrameCount)
    {
        window->DC.CursorPos = AzVec2(tab_bar->BarRect.Min.x,
                                      tab_bar->BarRect.Max.y
                                              + tab_bar->ItemSpacingY);
        tab_bar->BeginCount++;
        return true;
    }

    if ((flags & AzTabBarFlags_Reorderable)
                != (tab_bar->Flags & AzTabBarFlags_Reorderable)
        || (tab_bar->TabsAddedNew && !(flags & AzTabBarFlags_Reorderable)))
        if ((flags & AzTabBarFlags_DockNode) == 0)
            AzQsort(tab_bar->Tabs.Data,
                    tab_bar->Tabs.Size,
                    sizeof(AzTabItem),
                    TabItemComparerByBeginOrder);
    tab_bar->TabsAddedNew = false;

    if ((flags & AzTabBarFlags_FittingPolicyMask_) == 0)
        flags |= AzTabBarFlags_FittingPolicyDefault_;

    tab_bar->Flags = flags;
    tab_bar->BarRect = tab_bar_bb;
    tab_bar->WantLayout = true;
    tab_bar->PrevFrameVisible = tab_bar->CurrFrameVisible;
    tab_bar->CurrFrameVisible = g.FrameCount;
    tab_bar->PrevTabsContentsHeight = tab_bar->CurrTabsContentsHeight;
    tab_bar->CurrTabsContentsHeight = 0.0f;
    tab_bar->ItemSpacingY = g.Style.ItemSpacing.y;
    tab_bar->FramePadding = g.Style.FramePadding;
    tab_bar->TabsActiveCount = 0;
    tab_bar->LastTabItemIdx = -1;
    tab_bar->BeginCount = 1;

    window->DC.CursorPos = AzVec2(tab_bar->BarRect.Min.x,
                                  tab_bar->BarRect.Max.y
                                          + tab_bar->ItemSpacingY);

    const AzU32 col = GetColorU32((flags & AzTabBarFlags_IsFocused)
                                          ? AzCol_TabSelected
                                          : AzCol_TabDimmedSelected);
    if (g.Style.TabBarBorderSize > 0.0f)
    {
        const float y = tab_bar->BarRect.Max.y;
        window->DrawList->AddRectFilled(AzVec2(tab_bar->SeparatorMinX,
                                               y - g.Style.TabBarBorderSize),
                                        AzVec2(tab_bar->SeparatorMaxX, y),
                                        col);
    }
    return true;
}

void Az::EndTabBar()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    AzTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        AZ_ASSERT_USER_ERROR(tab_bar != NULL,
                             "Mismatched BeginTabBar()/EndTabBar()!");
        return;
    }

    if (tab_bar->WantLayout)
        TabBarLayout(tab_bar);

    const bool tab_bar_appearing = (tab_bar->PrevFrameVisible + 1
                                    < g.FrameCount);
    if (tab_bar->VisibleTabWasSubmitted || tab_bar->VisibleTabId == 0
        || tab_bar_appearing)
    {
        tab_bar->CurrTabsContentsHeight =
                AzMax(window->DC.CursorPos.y - tab_bar->BarRect.Max.y,
                      tab_bar->CurrTabsContentsHeight);
        window->DC.CursorPos.y = tab_bar->BarRect.Max.y
                                 + tab_bar->CurrTabsContentsHeight;
    } else
    {
        window->DC.CursorPos.y = tab_bar->BarRect.Max.y
                                 + tab_bar->PrevTabsContentsHeight;
    }
    if (tab_bar->BeginCount > 1)
        window->DC.CursorPos = tab_bar->BackupCursorPos;

    tab_bar->LastTabItemIdx = -1;
    if ((tab_bar->Flags & AzTabBarFlags_DockNode) == 0)
        PopID();

    g.CurrentTabBarStack.pop_back();
    g.CurrentTabBar = g.CurrentTabBarStack.empty()
                              ? NULL
                              : GetTabBarFromTabBarRef(
                                        g.CurrentTabBarStack.back());
}

static float TabBarCalcScrollableWidth(AzTabBar* tab_bar,
                                       AzTabBarSection* sections)
{
    return tab_bar->BarRect.GetWidth() - sections[0].Width - sections[2].Width
           - sections[1].Spacing;
}

static void Az::TabBarLayout(AzTabBar* tab_bar)
{
    AzContext& g = *GAz;
    tab_bar->WantLayout = false;

    int tab_dst_n = 0;
    bool need_sort_by_section = false;
    AzTabBarSection sections[3];
    for (int tab_src_n = 0; tab_src_n < tab_bar->Tabs.Size; tab_src_n++)
    {
        AzTabItem* tab = &tab_bar->Tabs[tab_src_n];
        if (tab->LastFrameVisible < tab_bar->PrevFrameVisible || tab->WantClose)
        {
            if (tab_bar->VisibleTabId == tab->ID)
            {
                tab_bar->VisibleTabId = 0;
            }
            if (tab_bar->SelectedTabId == tab->ID)
            {
                tab_bar->SelectedTabId = 0;
            }
            if (tab_bar->NextSelectedTabId == tab->ID)
            {
                tab_bar->NextSelectedTabId = 0;
            }
            continue;
        }
        if (tab_dst_n != tab_src_n)
            tab_bar->Tabs[tab_dst_n] = tab_bar->Tabs[tab_src_n];

        tab = &tab_bar->Tabs[tab_dst_n];
        tab->IndexDuringLayout = (AzS16)tab_dst_n;

        int curr_tab_section_n = TabItemGetSectionIdx(tab);
        if (tab_dst_n > 0)
        {
            AzTabItem* prev_tab = &tab_bar->Tabs[tab_dst_n - 1];
            int prev_tab_section_n = TabItemGetSectionIdx(prev_tab);
            if (curr_tab_section_n == 0 && prev_tab_section_n != 0)
                need_sort_by_section = true;
            if (prev_tab_section_n == 2 && curr_tab_section_n != 2)
                need_sort_by_section = true;
        }

        sections[curr_tab_section_n].TabCount++;
        tab_dst_n++;
    }
    if (tab_bar->Tabs.Size != tab_dst_n)
        tab_bar->Tabs.resize(tab_dst_n);

    if (need_sort_by_section)
        AzQsort(tab_bar->Tabs.Data,
                tab_bar->Tabs.Size,
                sizeof(AzTabItem),
                TabItemComparerBySection);

    sections[0].Spacing = sections[0].TabCount > 0
                                          && (sections[1].TabCount
                                              + sections[2].TabCount)
                                                     > 0
                                  ? g.Style.ItemInnerSpacing.x
                                  : 0.0f;
    sections[1].Spacing = sections[1].TabCount > 0 && sections[2].TabCount > 0
                                  ? g.Style.ItemInnerSpacing.x
                                  : 0.0f;

    AzID scroll_to_tab_id = 0;
    if (tab_bar->NextSelectedTabId)
    {
        tab_bar->SelectedTabId = tab_bar->NextSelectedTabId;
        tab_bar->NextSelectedTabId = 0;
        scroll_to_tab_id = tab_bar->SelectedTabId;
    }

    if (tab_bar->ReorderRequestTabId != 0)
    {
        if (TabBarProcessReorder(tab_bar))
            if (tab_bar->ReorderRequestTabId == tab_bar->SelectedTabId)
                scroll_to_tab_id = tab_bar->ReorderRequestTabId;
        tab_bar->ReorderRequestTabId = 0;
    }

    const bool tab_list_popup_button = (tab_bar->Flags
                                        & AzTabBarFlags_TabListPopupButton)
                                       != 0;
    if (tab_list_popup_button)
        if (AzTabItem* tab_to_select = TabBarTabListPopupButton(tab_bar))
            scroll_to_tab_id = tab_bar->SelectedTabId = tab_to_select->ID;

    int shrink_buffer_indexes[3] = {0,
                                    sections[0].TabCount + sections[2].TabCount,
                                    sections[0].TabCount};
    g.ShrinkWidthBuffer.resize(tab_bar->Tabs.Size);

    AzTabItem* most_recently_selected_tab = NULL;
    int curr_section_n = -1;
    bool found_selected_tab_id = false;
    for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
    {
        AzTabItem* tab = &tab_bar->Tabs[tab_n];
        AZ_ASSERT(tab->LastFrameVisible >= tab_bar->PrevFrameVisible);

        if ((most_recently_selected_tab == NULL
             || most_recently_selected_tab->LastFrameSelected
                        < tab->LastFrameSelected)
            && !(tab->Flags & AzTabItemFlags_Button))
            most_recently_selected_tab = tab;
        if (tab->ID == tab_bar->SelectedTabId)
            found_selected_tab_id = true;
        if (scroll_to_tab_id == 0 && g.NavJustMovedToId == tab->ID)
            scroll_to_tab_id = tab->ID;

        const char* tab_name = TabBarGetTabName(tab_bar, tab);
        const bool has_close_button_or_unsaved_marker =
                (tab->Flags & AzTabItemFlags_NoCloseButton) == 0
                || (tab->Flags & AzTabItemFlags_UnsavedDocument);
        tab->ContentWidth =
                (tab->RequestedWidth >= 0.0f)
                        ? tab->RequestedWidth
                        : TabItemCalcSize(tab_name,
                                          has_close_button_or_unsaved_marker)
                                  .x;

        int section_n = TabItemGetSectionIdx(tab);
        AzTabBarSection* section = &sections[section_n];
        section->Width += tab->ContentWidth
                          + (section_n == curr_section_n
                                     ? g.Style.ItemInnerSpacing.x
                                     : 0.0f);
        curr_section_n = section_n;

        AZ_MSVC_WARNING_SUPPRESS(6385);
        AzShrinkWidthItem* shrink_width_item =
                &g.ShrinkWidthBuffer[shrink_buffer_indexes[section_n]++];
        shrink_width_item->Index = tab_n;
        shrink_width_item->Width = shrink_width_item->InitialWidth =
                tab->ContentWidth;
        tab->Width = AzMax(tab->ContentWidth, 1.0f);
    }

    tab_bar->WidthAllTabsIdeal = 0.0f;
    for (int section_n = 0; section_n < 3; section_n++)
        tab_bar->WidthAllTabsIdeal += sections[section_n].Width
                                      + sections[section_n].Spacing;

    if ((tab_bar->WidthAllTabsIdeal > tab_bar->BarRect.GetWidth()
         && tab_bar->Tabs.Size > 1)
        && !(tab_bar->Flags & AzTabBarFlags_NoTabListScrollingButtons)
        && (tab_bar->Flags & AzTabBarFlags_FittingPolicyScroll))
        if (AzTabItem* scroll_and_select_tab = TabBarScrollingButtons(tab_bar))
        {
            scroll_to_tab_id = scroll_and_select_tab->ID;
            if ((scroll_and_select_tab->Flags & AzTabItemFlags_Button) == 0)
                tab_bar->SelectedTabId = scroll_to_tab_id;
        }

    float section_0_w = sections[0].Width + sections[0].Spacing;
    float section_1_w = sections[1].Width + sections[1].Spacing;
    float section_2_w = sections[2].Width + sections[2].Spacing;
    bool central_section_is_visible = (section_0_w + section_2_w)
                                      < tab_bar->BarRect.GetWidth();
    float width_excess;
    if (central_section_is_visible)
        width_excess = AzMax(section_1_w
                                     - (tab_bar->BarRect.GetWidth()
                                        - section_0_w - section_2_w),
                             0.0f);
    else
        width_excess = (section_0_w + section_2_w)
                       - tab_bar->BarRect.GetWidth();

    if (width_excess >= 1.0f
        && ((tab_bar->Flags & AzTabBarFlags_FittingPolicyResizeDown)
            || !central_section_is_visible))
    {
        int shrink_data_count = (central_section_is_visible
                                         ? sections[1].TabCount
                                         : sections[0].TabCount
                                                   + sections[2].TabCount);
        int shrink_data_offset = (central_section_is_visible
                                          ? sections[0].TabCount
                                                    + sections[2].TabCount
                                          : 0);
        ShrinkWidths(g.ShrinkWidthBuffer.Data + shrink_data_offset,
                     shrink_data_count,
                     width_excess);

        for (int tab_n = shrink_data_offset;
             tab_n < shrink_data_offset + shrink_data_count;
             tab_n++)
        {
            AzTabItem* tab = &tab_bar->Tabs[g.ShrinkWidthBuffer[tab_n].Index];
            float shrinked_width = AZ_TRUNC(g.ShrinkWidthBuffer[tab_n].Width);
            if (shrinked_width < 0.0f)
                continue;

            shrinked_width = AzMax(1.0f, shrinked_width);
            int section_n = TabItemGetSectionIdx(tab);
            sections[section_n].Width -= (tab->Width - shrinked_width);
            tab->Width = shrinked_width;
        }
    }

    int section_tab_index = 0;
    float tab_offset = 0.0f;
    tab_bar->WidthAllTabs = 0.0f;
    for (int section_n = 0; section_n < 3; section_n++)
    {
        AzTabBarSection* section = &sections[section_n];
        if (section_n == 2)
            tab_offset = AzMin(
                    AzMax(0.0f, tab_bar->BarRect.GetWidth() - section->Width),
                    tab_offset);

        for (int tab_n = 0; tab_n < section->TabCount; tab_n++)
        {
            AzTabItem* tab = &tab_bar->Tabs[section_tab_index + tab_n];
            tab->Offset = tab_offset;
            tab->NameOffset = -1;
            tab_offset += tab->Width
                          + (tab_n < section->TabCount - 1
                                     ? g.Style.ItemInnerSpacing.x
                                     : 0.0f);
        }
        tab_bar->WidthAllTabs += AzMax(section->Width + section->Spacing, 0.0f);
        tab_offset += section->Spacing;
        section_tab_index += section->TabCount;
    }

    tab_bar->TabsNames.Buf.resize(0);

    if (found_selected_tab_id == false)
        tab_bar->SelectedTabId = 0;
    if (tab_bar->SelectedTabId == 0 && tab_bar->NextSelectedTabId == 0
        && most_recently_selected_tab != NULL)
        scroll_to_tab_id = tab_bar->SelectedTabId =
                most_recently_selected_tab->ID;

    tab_bar->VisibleTabId = tab_bar->SelectedTabId;
    tab_bar->VisibleTabWasSubmitted = false;

    if (g.NavWindowingTarget != NULL && g.NavWindowingTarget->DockNode
        && g.NavWindowingTarget->DockNode->TabBar == tab_bar)
        tab_bar->VisibleTabId = scroll_to_tab_id = g.NavWindowingTarget->TabId;

    if (scroll_to_tab_id != 0)
        TabBarScrollToTab(tab_bar, scroll_to_tab_id, sections);
    else if ((tab_bar->Flags & AzTabBarFlags_FittingPolicyScroll)
             && IsMouseHoveringRect(tab_bar->BarRect.Min,
                                    tab_bar->BarRect.Max,
                                    true)
             && IsWindowContentHoverable(g.CurrentWindow))
    {
        const float wheel = g.IO.MouseWheelRequestAxisSwap ? g.IO.MouseWheel
                                                           : g.IO.MouseWheelH;
        const AzKey wheel_key = g.IO.MouseWheelRequestAxisSwap
                                        ? AzKey_MouseWheelY
                                        : AzKey_MouseWheelX;
        if (TestKeyOwner(wheel_key, tab_bar->ID) && wheel != 0.0f)
        {
            const float scroll_step =
                    wheel * TabBarCalcScrollableWidth(tab_bar, sections) / 3.0f;
            tab_bar->ScrollingTargetDistToVisibility = 0.0f;
            tab_bar->ScrollingTarget =
                    TabBarScrollClamp(tab_bar,
                                      tab_bar->ScrollingTarget - scroll_step);
        }
        SetKeyOwner(wheel_key, tab_bar->ID);
    }

    tab_bar->ScrollingAnim = TabBarScrollClamp(tab_bar, tab_bar->ScrollingAnim);
    tab_bar->ScrollingTarget = TabBarScrollClamp(tab_bar,
                                                 tab_bar->ScrollingTarget);
    if (tab_bar->ScrollingAnim != tab_bar->ScrollingTarget)
    {
        tab_bar->ScrollingSpeed = AzMax(tab_bar->ScrollingSpeed,
                                        70.0f * g.FontSize);
        tab_bar->ScrollingSpeed = AzMax(tab_bar->ScrollingSpeed,
                                        AzFabs(tab_bar->ScrollingTarget
                                               - tab_bar->ScrollingAnim)
                                                / 0.3f);
        const bool teleport = (tab_bar->PrevFrameVisible + 1 < g.FrameCount)
                              || (tab_bar->ScrollingTargetDistToVisibility
                                  > 10.0f * g.FontSize);
        tab_bar->ScrollingAnim =
                teleport ? tab_bar->ScrollingTarget
                         : AzLinearSweep(tab_bar->ScrollingAnim,
                                         tab_bar->ScrollingTarget,
                                         g.IO.DeltaTime
                                                 * tab_bar->ScrollingSpeed);
    } else
    {
        tab_bar->ScrollingSpeed = 0.0f;
    }
    tab_bar->ScrollingRectMinX = tab_bar->BarRect.Min.x + sections[0].Width
                                 + sections[0].Spacing;
    tab_bar->ScrollingRectMaxX = tab_bar->BarRect.Max.x - sections[2].Width
                                 - sections[1].Spacing;

    AzWindow* window = g.CurrentWindow;
    window->DC.CursorPos = tab_bar->BarRect.Min;
    ItemSize(AzVec2(tab_bar->WidthAllTabs, tab_bar->BarRect.GetHeight()),
             tab_bar->FramePadding.y);
    window->DC.IdealMaxPos.x = AzMax(window->DC.IdealMaxPos.x,
                                     tab_bar->BarRect.Min.x
                                             + tab_bar->WidthAllTabsIdeal);
}

static AzU32 Az::TabBarCalcTabID(AzTabBar* tab_bar,
                                 const char* label,
                                 AzWindow* docked_window)
{
    if (docked_window != NULL)
    {
        AZ_UNUSED(tab_bar);
        AZ_ASSERT(tab_bar->Flags & AzTabBarFlags_DockNode);
        AzID id = docked_window->TabId;
        KeepAliveID(id);
        return id;
    } else
    {
        AzWindow* window = GAz->CurrentWindow;
        return window->GetID(label);
    }
}

static float Az::TabBarCalcMaxTabWidth()
{
    AzContext& g = *GAz;
    return g.FontSize * 20.0f;
}

AzTabItem* Az::TabBarFindTabByID(AzTabBar* tab_bar, AzID tab_id)
{
    if (tab_id != 0)
        for (int n = 0; n < tab_bar->Tabs.Size; n++)
            if (tab_bar->Tabs[n].ID == tab_id)
                return &tab_bar->Tabs[n];
    return NULL;
}

AzTabItem* Az::TabBarFindTabByOrder(AzTabBar* tab_bar, int order)
{
    if (order < 0 || order >= tab_bar->Tabs.Size)
        return NULL;
    return &tab_bar->Tabs[order];
}

AzTabItem* Az::TabBarFindMostRecentlySelectedTabForActiveWindow(
        AzTabBar* tab_bar)
{
    AzTabItem* most_recently_selected_tab = NULL;
    for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
    {
        AzTabItem* tab = &tab_bar->Tabs[tab_n];
        if (most_recently_selected_tab == NULL
            || most_recently_selected_tab->LastFrameSelected
                       < tab->LastFrameSelected)
            if (tab->Window && tab->Window->WasActive)
                most_recently_selected_tab = tab;
    }
    return most_recently_selected_tab;
}

AzTabItem* Az::TabBarGetCurrentTab(AzTabBar* tab_bar)
{
    if (tab_bar->LastTabItemIdx < 0
        || tab_bar->LastTabItemIdx >= tab_bar->Tabs.Size)
        return NULL;
    return &tab_bar->Tabs[tab_bar->LastTabItemIdx];
}

const char* Az::TabBarGetTabName(AzTabBar* tab_bar, AzTabItem* tab)
{
    if (tab->Window)
        return tab->Window->Name;
    if (tab->NameOffset == -1)
        return "N/A";
    AZ_ASSERT(tab->NameOffset < tab_bar->TabsNames.Buf.Size);
    return tab_bar->TabsNames.Buf.Data + tab->NameOffset;
}

void Az::TabBarAddTab(AzTabBar* tab_bar,
                      AzTabItemFlags tab_flags,
                      AzWindow* window)
{
    AzContext& g = *GAz;
    AZ_ASSERT(TabBarFindTabByID(tab_bar, window->TabId) == NULL);
    AZ_ASSERT(g.CurrentTabBar != tab_bar);

    if (!window->HasCloseButton)
        tab_flags |= AzTabItemFlags_NoCloseButton;

    AzTabItem new_tab;
    new_tab.ID = window->TabId;
    new_tab.Flags = tab_flags;
    new_tab.LastFrameVisible = tab_bar->CurrFrameVisible;
    if (new_tab.LastFrameVisible == -1)
        new_tab.LastFrameVisible = g.FrameCount - 1;
    new_tab.Window = window;
    tab_bar->Tabs.push_back(new_tab);
}

void Az::TabBarRemoveTab(AzTabBar* tab_bar, AzID tab_id)
{
    if (AzTabItem* tab = TabBarFindTabByID(tab_bar, tab_id))
        tab_bar->Tabs.erase(tab);
    if (tab_bar->VisibleTabId == tab_id)
    {
        tab_bar->VisibleTabId = 0;
    }
    if (tab_bar->SelectedTabId == tab_id)
    {
        tab_bar->SelectedTabId = 0;
    }
    if (tab_bar->NextSelectedTabId == tab_id)
    {
        tab_bar->NextSelectedTabId = 0;
    }
}

void Az::TabBarCloseTab(AzTabBar* tab_bar, AzTabItem* tab)
{
    if (tab->Flags & AzTabItemFlags_Button)
        return;

    if ((tab->Flags
         & (AzTabItemFlags_UnsavedDocument | AzTabItemFlags_NoAssumedClosure))
        == 0)
    {
        tab->WantClose = true;
        if (tab_bar->VisibleTabId == tab->ID)
        {
            tab->LastFrameVisible = -1;
            tab_bar->SelectedTabId = tab_bar->NextSelectedTabId = 0;
        }
    } else
    {
        if (tab_bar->VisibleTabId != tab->ID)
            TabBarQueueFocus(tab_bar, tab);
    }
}

static float Az::TabBarScrollClamp(AzTabBar* tab_bar, float scrolling)
{
    scrolling = AzMin(scrolling,
                      tab_bar->WidthAllTabs - tab_bar->BarRect.GetWidth());
    return AzMax(scrolling, 0.0f);
}

static void Az::TabBarScrollToTab(AzTabBar* tab_bar,
                                  AzID tab_id,
                                  AzTabBarSection* sections)
{
    AzTabItem* tab = TabBarFindTabByID(tab_bar, tab_id);
    if (tab == NULL)
        return;
    if (tab->Flags & AzTabItemFlags_SectionMask_)
        return;

    AzContext& g = *GAz;
    float margin = g.FontSize * 1.0f;
    int order = TabBarGetTabOrder(tab_bar, tab);

    float scrollable_width = TabBarCalcScrollableWidth(tab_bar, sections);

    float tab_x1 = tab->Offset - sections[0].Width
                   + (order > sections[0].TabCount - 1 ? -margin : 0.0f);
    float tab_x2 = tab->Offset - sections[0].Width + tab->Width
                   + (order + 1 < tab_bar->Tabs.Size - sections[2].TabCount
                              ? margin
                              : 1.0f);
    tab_bar->ScrollingTargetDistToVisibility = 0.0f;
    if (tab_bar->ScrollingTarget > tab_x1
        || (tab_x2 - tab_x1 >= scrollable_width))
    {
        tab_bar->ScrollingTargetDistToVisibility = AzMax(tab_bar->ScrollingAnim
                                                                 - tab_x2,
                                                         0.0f);
        tab_bar->ScrollingTarget = tab_x1;
    } else if (tab_bar->ScrollingTarget < tab_x2 - scrollable_width)
    {
        tab_bar->ScrollingTargetDistToVisibility =
                AzMax((tab_x1 - scrollable_width) - tab_bar->ScrollingAnim,
                      0.0f);
        tab_bar->ScrollingTarget = tab_x2 - scrollable_width;
    }
}

void Az::TabBarQueueFocus(AzTabBar* tab_bar, AzTabItem* tab)
{
    tab_bar->NextSelectedTabId = tab->ID;
}

void Az::TabBarQueueFocus(AzTabBar* tab_bar, const char* tab_name)
{
    AZ_ASSERT((tab_bar->Flags & AzTabBarFlags_DockNode) == 0);
    AzID tab_id = TabBarCalcTabID(tab_bar, tab_name, NULL);
    tab_bar->NextSelectedTabId = tab_id;
}

void Az::TabBarQueueReorder(AzTabBar* tab_bar, AzTabItem* tab, int offset)
{
    AZ_ASSERT(offset != 0);
    AZ_ASSERT(tab_bar->ReorderRequestTabId == 0);
    tab_bar->ReorderRequestTabId = tab->ID;
    tab_bar->ReorderRequestOffset = (AzS16)offset;
}

void Az::TabBarQueueReorderFromMousePos(AzTabBar* tab_bar,
                                        AzTabItem* src_tab,
                                        AzVec2 mouse_pos)
{
    AzContext& g = *GAz;
    AZ_ASSERT(tab_bar->ReorderRequestTabId == 0);
    if ((tab_bar->Flags & AzTabBarFlags_Reorderable) == 0)
        return;

    const bool is_central_section = (src_tab->Flags
                                     & AzTabItemFlags_SectionMask_)
                                    == 0;
    const float bar_offset = tab_bar->BarRect.Min.x
                             - (is_central_section ? tab_bar->ScrollingTarget
                                                   : 0);

    const int dir = (bar_offset + src_tab->Offset) > mouse_pos.x ? -1 : +1;
    const int src_idx = tab_bar->Tabs.index_from_ptr(src_tab);
    int dst_idx = src_idx;
    for (int i = src_idx; i >= 0 && i < tab_bar->Tabs.Size; i += dir)
    {
        const AzTabItem* dst_tab = &tab_bar->Tabs[i];
        if (dst_tab->Flags & AzTabItemFlags_NoReorder)
            break;
        if ((dst_tab->Flags & AzTabItemFlags_SectionMask_)
            != (src_tab->Flags & AzTabItemFlags_SectionMask_))
            break;
        dst_idx = i;

        const float x1 = bar_offset + dst_tab->Offset
                         - g.Style.ItemInnerSpacing.x;
        const float x2 = bar_offset + dst_tab->Offset + dst_tab->Width
                         + g.Style.ItemInnerSpacing.x;

        if ((dir < 0 && mouse_pos.x > x1) || (dir > 0 && mouse_pos.x < x2))
            break;
    }

    if (dst_idx != src_idx)
        TabBarQueueReorder(tab_bar, src_tab, dst_idx - src_idx);
}

bool Az::TabBarProcessReorder(AzTabBar* tab_bar)
{
    AzTabItem* tab1 = TabBarFindTabByID(tab_bar, tab_bar->ReorderRequestTabId);
    if (tab1 == NULL || (tab1->Flags & AzTabItemFlags_NoReorder))
        return false;

    int tab2_order = TabBarGetTabOrder(tab_bar, tab1)
                     + tab_bar->ReorderRequestOffset;
    if (tab2_order < 0 || tab2_order >= tab_bar->Tabs.Size)
        return false;

    AzTabItem* tab2 = &tab_bar->Tabs[tab2_order];
    if (tab2->Flags & AzTabItemFlags_NoReorder)
        return false;
    if ((tab1->Flags & AzTabItemFlags_SectionMask_)
        != (tab2->Flags & AzTabItemFlags_SectionMask_))
        return false;

    AzTabItem item_tmp = *tab1;
    AzTabItem* src_tab = (tab_bar->ReorderRequestOffset > 0) ? tab1 + 1 : tab2;
    AzTabItem* dst_tab = (tab_bar->ReorderRequestOffset > 0) ? tab1 : tab2 + 1;
    const int move_count = (tab_bar->ReorderRequestOffset > 0)
                                   ? tab_bar->ReorderRequestOffset
                                   : -tab_bar->ReorderRequestOffset;
    memmove(dst_tab, src_tab, move_count * sizeof(AzTabItem));
    *tab2 = item_tmp;

    if (tab_bar->Flags & AzTabBarFlags_SaveSettings)
        MarkIniSettingsDirty();
    return true;
}

static AzTabItem* Az::TabBarScrollingButtons(AzTabBar* tab_bar)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    const AzVec2 arrow_button_size(g.FontSize - 2.0f,
                                   g.FontSize + g.Style.FramePadding.y * 2.0f);
    const float scrolling_buttons_width = arrow_button_size.x * 2.0f;

    const AzVec2 backup_cursor_pos = window->DC.CursorPos;

    int select_dir = 0;
    AzVec4 arrow_col = g.Style.Colors[AzCol_Text];
    arrow_col.w *= 0.5f;

    PushStyleColor(AzCol_Text, arrow_col);
    PushStyleColor(AzCol_Button, AzVec4(0, 0, 0, 0));
    PushItemFlag(AzItemFlags_ButtonRepeat, true);
    const float backup_repeat_delay = g.IO.KeyRepeatDelay;
    const float backup_repeat_rate = g.IO.KeyRepeatRate;
    g.IO.KeyRepeatDelay = 0.250f;
    g.IO.KeyRepeatRate = 0.200f;
    float x = AzMax(tab_bar->BarRect.Min.x,
                    tab_bar->BarRect.Max.x - scrolling_buttons_width);
    window->DC.CursorPos = AzVec2(x, tab_bar->BarRect.Min.y);
    if (ArrowButtonEx("##<",
                      AzDir_Left,
                      arrow_button_size,
                      AzButtonFlags_PressedOnClick))
        select_dir = -1;
    window->DC.CursorPos = AzVec2(x + arrow_button_size.x,
                                  tab_bar->BarRect.Min.y);
    if (ArrowButtonEx("##>",
                      AzDir_Right,
                      arrow_button_size,
                      AzButtonFlags_PressedOnClick))
        select_dir = +1;
    PopItemFlag();
    PopStyleColor(2);
    g.IO.KeyRepeatRate = backup_repeat_rate;
    g.IO.KeyRepeatDelay = backup_repeat_delay;

    AzTabItem* tab_to_scroll_to = NULL;
    if (select_dir != 0)
        if (AzTabItem* tab_item = TabBarFindTabByID(tab_bar,
                                                    tab_bar->SelectedTabId))
        {
            int selected_order = TabBarGetTabOrder(tab_bar, tab_item);
            int target_order = selected_order + select_dir;

            while (tab_to_scroll_to == NULL)
            {
                tab_to_scroll_to =
                        &tab_bar->Tabs[(target_order >= 0
                                        && target_order < tab_bar->Tabs.Size)
                                               ? target_order
                                               : selected_order];

                if (tab_to_scroll_to->Flags & AzTabItemFlags_Button)
                {
                    target_order += select_dir;
                    selected_order += select_dir;
                    tab_to_scroll_to = (target_order < 0
                                        || target_order >= tab_bar->Tabs.Size)
                                               ? tab_to_scroll_to
                                               : NULL;
                }
            }
        }
    window->DC.CursorPos = backup_cursor_pos;
    tab_bar->BarRect.Max.x -= scrolling_buttons_width + 1.0f;

    return tab_to_scroll_to;
}

static AzTabItem* Az::TabBarTabListPopupButton(AzTabBar* tab_bar)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;

    const float tab_list_popup_button_width = g.FontSize
                                              + g.Style.FramePadding.y;
    const AzVec2 backup_cursor_pos = window->DC.CursorPos;
    window->DC.CursorPos = AzVec2(tab_bar->BarRect.Min.x
                                          - g.Style.FramePadding.y,
                                  tab_bar->BarRect.Min.y);
    tab_bar->BarRect.Min.x += tab_list_popup_button_width;

    AzVec4 arrow_col = g.Style.Colors[AzCol_Text];
    arrow_col.w *= 0.5f;
    PushStyleColor(AzCol_Text, arrow_col);
    PushStyleColor(AzCol_Button, AzVec4(0, 0, 0, 0));
    bool open = BeginCombo("##v",
                           NULL,
                           AzComboFlags_NoPreview | AzComboFlags_HeightLargest);
    PopStyleColor(2);

    AzTabItem* tab_to_select = NULL;
    if (open)
    {
        for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
        {
            AzTabItem* tab = &tab_bar->Tabs[tab_n];
            if (tab->Flags & AzTabItemFlags_Button)
                continue;

            const char* tab_name = TabBarGetTabName(tab_bar, tab);
            if (Selectable(tab_name, tab_bar->SelectedTabId == tab->ID))
                tab_to_select = tab;
        }
        EndCombo();
    }

    window->DC.CursorPos = backup_cursor_pos;
    return tab_to_select;
}

bool Az::BeginTabItem(const char* label, bool* p_open, AzTabItemFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    AzTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                tab_bar,
                "Needs to be called between BeginTabBar() and EndTabBar()!");
        return false;
    }
    AZ_ASSERT((flags & AzTabItemFlags_Button) == 0);

    bool ret = TabItemEx(tab_bar, label, p_open, flags, NULL);
    if (ret && !(flags & AzTabItemFlags_NoPushId))
    {
        AzTabItem* tab = &tab_bar->Tabs[tab_bar->LastTabItemIdx];
        PushOverrideID(tab->ID);
    }
    return ret;
}

void Az::EndTabItem()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    AzTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                tab_bar != NULL,
                "Needs to be called between BeginTabBar() and EndTabBar()!");
        return;
    }
    AZ_ASSERT(tab_bar->LastTabItemIdx >= 0);
    AzTabItem* tab = &tab_bar->Tabs[tab_bar->LastTabItemIdx];
    if (!(tab->Flags & AzTabItemFlags_NoPushId))
        PopID();
}

bool Az::TabItemButton(const char* label, AzTabItemFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    AzTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                tab_bar != NULL,
                "Needs to be called between BeginTabBar() and EndTabBar()!");
        return false;
    }
    return TabItemEx(tab_bar,
                     label,
                     NULL,
                     flags | AzTabItemFlags_Button | AzTabItemFlags_NoReorder,
                     NULL);
}

void Az::TabItemSpacing(const char* str_id, AzTabItemFlags flags, float width)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    AzTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                tab_bar != NULL,
                "Needs to be called between BeginTabBar() and EndTabBar()!");
        return;
    }
    SetNextItemWidth(width);
    TabItemEx(tab_bar,
              str_id,
              NULL,
              flags | AzTabItemFlags_Button | AzTabItemFlags_NoReorder
                      | AzTabItemFlags_Invisible,
              NULL);
}

bool Az::TabItemEx(AzTabBar* tab_bar,
                   const char* label,
                   bool* p_open,
                   AzTabItemFlags flags,
                   AzWindow* docked_window)
{
    AzContext& g = *GAz;
    if (tab_bar->WantLayout)
    {
        AzNextItemData backup_next_item_data = g.NextItemData;
        TabBarLayout(tab_bar);
        g.NextItemData = backup_next_item_data;
    }
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    const AzStyle& style = g.Style;
    const AzID id = TabBarCalcTabID(tab_bar, label, docked_window);

    AZ_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    if (p_open && !*p_open)
    {
        ItemAdd(AzRect(), id, NULL, AzItemFlags_NoNav);
        return false;
    }

    AZ_ASSERT(!p_open || !(flags & AzTabItemFlags_Button));
    AZ_ASSERT((flags & (AzTabItemFlags_Leading | AzTabItemFlags_Trailing))
              != (AzTabItemFlags_Leading | AzTabItemFlags_Trailing));

    if (flags & AzTabItemFlags_NoCloseButton)
        p_open = NULL;
    else if (p_open == NULL)
        flags |= AzTabItemFlags_NoCloseButton;

    AzTabItem* tab = TabBarFindTabByID(tab_bar, id);
    bool tab_is_new = false;
    if (tab == NULL)
    {
        tab_bar->Tabs.push_back(AzTabItem());
        tab = &tab_bar->Tabs.back();
        tab->ID = id;
        tab_bar->TabsAddedNew = tab_is_new = true;
    }
    tab_bar->LastTabItemIdx = (AzS16)tab_bar->Tabs.index_from_ptr(tab);

    AzVec2 size = TabItemCalcSize(
            label,
            (p_open != NULL) || (flags & AzTabItemFlags_UnsavedDocument));
    tab->RequestedWidth = -1.0f;
    if (g.NextItemData.HasFlags & AzNextItemDataFlags_HasWidth)
        size.x = tab->RequestedWidth = g.NextItemData.Width;
    if (tab_is_new)
        tab->Width = AzMax(1.0f, size.x);
    tab->ContentWidth = size.x;
    tab->BeginOrder = tab_bar->TabsActiveCount++;

    const bool tab_bar_appearing = (tab_bar->PrevFrameVisible + 1
                                    < g.FrameCount);
    const bool tab_bar_focused = (tab_bar->Flags & AzTabBarFlags_IsFocused)
                                 != 0;
    const bool tab_appearing = (tab->LastFrameVisible + 1 < g.FrameCount);
    const bool tab_just_unsaved = (flags & AzTabItemFlags_UnsavedDocument)
                                  && !(tab->Flags
                                       & AzTabItemFlags_UnsavedDocument);
    const bool is_tab_button = (flags & AzTabItemFlags_Button) != 0;
    tab->LastFrameVisible = g.FrameCount;
    tab->Flags = flags;
    tab->Window = docked_window;

    if (docked_window != NULL)
    {
        AZ_ASSERT(tab_bar->Flags & AzTabBarFlags_DockNode);
        tab->NameOffset = -1;
    } else
    {
        tab->NameOffset = (AzS32)tab_bar->TabsNames.size();
        tab_bar->TabsNames.append(label, label + AzStrlen(label) + 1);
    }

    if (!is_tab_button)
    {
        if (tab_appearing && (tab_bar->Flags & AzTabBarFlags_AutoSelectNewTabs)
            && tab_bar->NextSelectedTabId == 0)
            if (!tab_bar_appearing || tab_bar->SelectedTabId == 0)
                TabBarQueueFocus(tab_bar, tab);
        if ((flags & AzTabItemFlags_SetSelected)
            && (tab_bar->SelectedTabId != id))
            TabBarQueueFocus(tab_bar, tab);
    }

    bool tab_contents_visible = (tab_bar->VisibleTabId == id);
    if (tab_contents_visible)
        tab_bar->VisibleTabWasSubmitted = true;

    if (!tab_contents_visible && tab_bar->SelectedTabId == 0
        && tab_bar_appearing && docked_window == NULL)
        if (tab_bar->Tabs.Size == 1
            && !(tab_bar->Flags & AzTabBarFlags_AutoSelectNewTabs))
            tab_contents_visible = true;

    if (tab_appearing && (!tab_bar_appearing || tab_is_new))
    {
        ItemAdd(AzRect(), id, NULL, AzItemFlags_NoNav);
        if (is_tab_button)
            return false;
        return tab_contents_visible;
    }

    if (tab_bar->SelectedTabId == id)
        tab->LastFrameSelected = g.FrameCount;

    const AzVec2 backup_main_cursor_pos = window->DC.CursorPos;

    const bool is_central_section = (tab->Flags & AzTabItemFlags_SectionMask_)
                                    == 0;
    size.x = tab->Width;
    if (is_central_section)
        window->DC.CursorPos =
                tab_bar->BarRect.Min
                + AzVec2(AZ_TRUNC(tab->Offset - tab_bar->ScrollingAnim), 0.0f);
    else
        window->DC.CursorPos = tab_bar->BarRect.Min + AzVec2(tab->Offset, 0.0f);
    AzVec2 pos = window->DC.CursorPos;
    AzRect bb(pos, pos + size);

    const bool want_clip_rect = is_central_section
                                && (bb.Min.x < tab_bar->ScrollingRectMinX
                                    || bb.Max.x > tab_bar->ScrollingRectMaxX);
    if (want_clip_rect)
        PushClipRect(AzVec2(AzMax(bb.Min.x, tab_bar->ScrollingRectMinX),
                            bb.Min.y - 1),
                     AzVec2(tab_bar->ScrollingRectMaxX, bb.Max.y),
                     true);

    AzVec2 backup_cursor_max_pos = window->DC.CursorMaxPos;
    ItemSize(bb.GetSize(), style.FramePadding.y);
    window->DC.CursorMaxPos = backup_cursor_max_pos;

    if (!ItemAdd(bb, id))
    {
        if (want_clip_rect)
            PopClipRect();
        window->DC.CursorPos = backup_main_cursor_pos;
        return tab_contents_visible;
    }

    AzButtonFlags button_flags = ((is_tab_button
                                           ? AzButtonFlags_PressedOnClickRelease
                                           : AzButtonFlags_PressedOnClick)
                                  | AzButtonFlags_AllowOverlap);
    if (g.DragDropActive
        && !g.DragDropPayload.IsDataType(AZ_PAYLOAD_TYPE_WINDOW))
        button_flags |= AzButtonFlags_PressedOnDragDropHold;
    bool hovered, held, pressed;
    if (flags & AzTabItemFlags_Invisible)
        hovered = held = pressed = false;
    else
        pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);
    if (pressed && !is_tab_button)
        TabBarQueueFocus(tab_bar, tab);

    if (held && docked_window && g.ActiveId == id && g.ActiveIdIsJustActivated)
        g.ActiveIdWindow = docked_window;

    AzDockNode* node = docked_window ? docked_window->DockNode : NULL;
    const bool single_floating_window_node = node && node->IsFloatingNode()
                                             && (node->Windows.Size == 1);
    if (held && single_floating_window_node && IsMouseDragging(0, 0.0f))
    {
        StartMouseMovingWindow(docked_window);
    } else if (held && !tab_appearing && IsMouseDragging(0))
    {
        int drag_dir = 0;
        float drag_distance_from_edge_x = 0.0f;
        if (!g.DragDropActive
            && ((tab_bar->Flags & AzTabBarFlags_Reorderable)
                || (docked_window != NULL)))
        {
            if (g.IO.MouseDelta.x < 0.0f && g.IO.MousePos.x < bb.Min.x)
            {
                drag_dir = -1;
                drag_distance_from_edge_x = bb.Min.x - g.IO.MousePos.x;
                TabBarQueueReorderFromMousePos(tab_bar, tab, g.IO.MousePos);
            } else if (g.IO.MouseDelta.x > 0.0f && g.IO.MousePos.x > bb.Max.x)
            {
                drag_dir = +1;
                drag_distance_from_edge_x = g.IO.MousePos.x - bb.Max.x;
                TabBarQueueReorderFromMousePos(tab_bar, tab, g.IO.MousePos);
            }
        }

        const bool can_undock = docked_window != NULL
                                && !(docked_window->Flags
                                     & AzWindowFlags_NoMove)
                                && !(node->MergedFlags
                                     & AzDockNodeFlags_NoUndocking);
        if (can_undock)
        {
            bool undocking_tab = (g.DragDropActive
                                  && g.DragDropPayload.SourceId == id);
            if (!undocking_tab)
            {
                float threshold_base = g.FontSize;
                float threshold_x = (threshold_base * 2.2f);
                float threshold_y =
                        (threshold_base * 1.5f)
                        + AzClamp((AzFabs(g.IO.MouseDragMaxDistanceAbs[0].x)
                                   - threshold_base * 2.0f)
                                          * 0.20f,
                                  0.0f,
                                  threshold_base * 4.0f);

                float distance_from_edge_y = AzMax(bb.Min.y - g.IO.MousePos.y,
                                                   g.IO.MousePos.y - bb.Max.y);
                if (distance_from_edge_y >= threshold_y)
                    undocking_tab = true;
                if (drag_distance_from_edge_x > threshold_x)
                    if ((drag_dir < 0 && TabBarGetTabOrder(tab_bar, tab) == 0)
                        || (drag_dir > 0
                            && TabBarGetTabOrder(tab_bar, tab)
                                       == tab_bar->Tabs.Size - 1))
                        undocking_tab = true;
            }

            if (undocking_tab)
            {
                DockContextQueueUndockWindow(&g, docked_window);
                g.MovingWindow = docked_window;
                SetActiveID(g.MovingWindow->MoveId, g.MovingWindow);
                g.ActiveIdClickOffset -= g.MovingWindow->Pos - bb.Min;
                g.ActiveIdNoClearOnFocusLoss = true;
                SetActiveIdUsingAllKeyboardKeys();
            }
        }
    }

#if 0
    if (hovered && g.HoveredIdNotActiveTimer > TOOLTIP_DELAY && bb.GetWidth() < tab->ContentWidth)
    {

        bb.Max.x = bb.Min.x + AZ_TRUNC(AzLerp(bb.GetWidth(), tab->ContentWidth, AzSaturate((g.HoveredIdNotActiveTimer - 0.40f) * 6.0f)));
        display_draw_list = GetForegroundDrawList(window);
        TabItemBackground(display_draw_list, bb, flags, GetColorU32(AzCol_TitleBgActive));
    }
#endif

    const bool is_visible = (g.LastItemData.StatusFlags
                             & AzItemStatusFlags_Visible)
                            && !(flags & AzTabItemFlags_Invisible);
    if (is_visible)
    {
        AzDrawList* display_draw_list = window->DrawList;
        const AzU32 tab_col = GetColorU32(
                (held || hovered) ? AzCol_TabHovered
                : tab_contents_visible
                        ? (tab_bar_focused ? AzCol_TabSelected
                                           : AzCol_TabDimmedSelected)
                        : (tab_bar_focused ? AzCol_Tab : AzCol_TabDimmed));
        TabItemBackground(display_draw_list, bb, flags, tab_col);
        if (tab_contents_visible
            && (tab_bar->Flags & AzTabBarFlags_DrawSelectedOverline)
            && style.TabBarOverlineSize > 0.0f)
        {
            AzVec2 tl = bb.GetTL() + AzVec2(0, 1.0f * g.CurrentDpiScale);
            AzVec2 tr = bb.GetTR() + AzVec2(0, 1.0f * g.CurrentDpiScale);
            AzU32 overline_col = GetColorU32(
                    tab_bar_focused ? AzCol_TabSelectedOverline
                                    : AzCol_TabDimmedSelectedOverline);
            if (style.TabRounding > 0.0f)
            {
                float rounding = style.TabRounding;
                display_draw_list->PathArcToFast(tl
                                                         + AzVec2(+rounding,
                                                                  +rounding),
                                                 rounding,
                                                 7,
                                                 9);
                display_draw_list->PathArcToFast(tr
                                                         + AzVec2(-rounding,
                                                                  +rounding),
                                                 rounding,
                                                 9,
                                                 11);
                display_draw_list->PathStroke(overline_col,
                                              0,
                                              style.TabBarOverlineSize);
            } else
            {
                display_draw_list->AddLine(tl - AzVec2(0.5f, 0.5f),
                                           tr - AzVec2(0.5f, 0.5f),
                                           overline_col,
                                           style.TabBarOverlineSize);
            }
        }
        RenderNavCursor(bb, id);

        const bool hovered_unblocked = IsItemHovered(
                AzHoveredFlags_AllowWhenBlockedByPopup);
        if (tab_bar->SelectedTabId != tab->ID && hovered_unblocked
            && (IsMouseClicked(1) || IsMouseReleased(1)) && !is_tab_button)
            TabBarQueueFocus(tab_bar, tab);

        if (tab_bar->Flags & AzTabBarFlags_NoCloseWithMiddleMouseButton)
            flags |= AzTabItemFlags_NoCloseWithMiddleMouseButton;

        const AzID close_button_id =
                p_open ? GetIDWithSeed("#CLOSE",
                                       NULL,
                                       docked_window ? docked_window->ID : id)
                       : 0;
        bool just_closed;
        bool text_clipped;
        TabItemLabelAndCloseButton(display_draw_list,
                                   bb,
                                   tab_just_unsaved
                                           ? (flags
                                              & ~AzTabItemFlags_UnsavedDocument)
                                           : flags,
                                   tab_bar->FramePadding,
                                   label,
                                   id,
                                   close_button_id,
                                   tab_contents_visible,
                                   &just_closed,
                                   &text_clipped);
        if (just_closed && p_open != NULL)
        {
            *p_open = false;
            TabBarCloseTab(tab_bar, tab);
        }

        if (docked_window && (hovered || g.HoveredId == close_button_id))
            g.LastItemData.StatusFlags |= AzItemStatusFlags_HoveredWindow;

        if (text_clipped && g.HoveredId == id && !held)
            if (!(tab_bar->Flags & AzTabBarFlags_NoTooltip)
                && !(tab->Flags & AzTabItemFlags_NoTooltip))
                SetItemTooltip("%.*s",
                               (int)(FindRenderedTextEnd(label) - label),
                               label);
    }

    if (want_clip_rect)
        PopClipRect();
    window->DC.CursorPos = backup_main_cursor_pos;

    AZ_ASSERT(!is_tab_button
              || !(tab_bar->SelectedTabId == tab->ID && is_tab_button));
    if (is_tab_button)
        return pressed;
    return tab_contents_visible;
}

void Az::SetTabItemClosed(const char* label)
{
    AzContext& g = *GAz;
    bool is_within_manual_tab_bar = g.CurrentTabBar
                                    && !(g.CurrentTabBar->Flags
                                         & AzTabBarFlags_DockNode);
    if (is_within_manual_tab_bar)
    {
        AzTabBar* tab_bar = g.CurrentTabBar;
        AzID tab_id = TabBarCalcTabID(tab_bar, label, NULL);
        if (AzTabItem* tab = TabBarFindTabByID(tab_bar, tab_id))
            tab->WantClose = true;
    } else if (AzWindow* window = FindWindowByName(label))
    {
        if (window->DockIsActive)
            if (AzDockNode* node = window->DockNode)
            {
                AzID tab_id = TabBarCalcTabID(node->TabBar, label, window);
                TabBarRemoveTab(node->TabBar, tab_id);
                window->DockTabWantClose = true;
            }
    }
}

AzVec2 Az::TabItemCalcSize(const char* label,
                           bool has_close_button_or_unsaved_marker)
{
    AzContext& g = *GAz;
    AzVec2 label_size = CalcTextSize(label, NULL, true);
    AzVec2 size = AzVec2(label_size.x + g.Style.FramePadding.x,
                         label_size.y + g.Style.FramePadding.y * 2.0f);
    if (has_close_button_or_unsaved_marker)
        size.x += g.Style.FramePadding.x
                  + (g.Style.ItemInnerSpacing.x + g.FontSize);
    else
        size.x += g.Style.FramePadding.x + 1.0f;
    return AzVec2(AzMin(size.x, TabBarCalcMaxTabWidth()), size.y);
}

AzVec2 Az::TabItemCalcSize(AzWindow* window)
{
    return TabItemCalcSize(window->Name,
                           window->HasCloseButton
                                   || (window->Flags
                                       & AzWindowFlags_UnsavedDocument));
}

void Az::TabItemBackground(AzDrawList* draw_list,
                           const AzRect& bb,
                           AzTabItemFlags flags,
                           AzU32 col)
{
    AzContext& g = *GAz;
    const float width = bb.GetWidth();
    AZ_UNUSED(flags);
    AZ_ASSERT(width > 0.0f);
    const float rounding = AzMax(0.0f,
                                 AzMin((flags & AzTabItemFlags_Button)
                                               ? g.Style.FrameRounding
                                               : g.Style.TabRounding,
                                       width * 0.5f - 1.0f));
    const float y1 = bb.Min.y + 1.0f;
    const float y2 = bb.Max.y - g.Style.TabBarBorderSize;
    draw_list->PathLineTo(AzVec2(bb.Min.x, y2));
    draw_list->PathArcToFast(AzVec2(bb.Min.x + rounding, y1 + rounding),
                             rounding,
                             6,
                             9);
    draw_list->PathArcToFast(AzVec2(bb.Max.x - rounding, y1 + rounding),
                             rounding,
                             9,
                             12);
    draw_list->PathLineTo(AzVec2(bb.Max.x, y2));
    draw_list->PathFillConvex(col);
    if (g.Style.TabBorderSize > 0.0f)
    {
        draw_list->PathLineTo(AzVec2(bb.Min.x + 0.5f, y2));
        draw_list->PathArcToFast(AzVec2(bb.Min.x + rounding + 0.5f,
                                        y1 + rounding + 0.5f),
                                 rounding,
                                 6,
                                 9);
        draw_list->PathArcToFast(AzVec2(bb.Max.x - rounding - 0.5f,
                                        y1 + rounding + 0.5f),
                                 rounding,
                                 9,
                                 12);
        draw_list->PathLineTo(AzVec2(bb.Max.x - 0.5f, y2));
        draw_list->PathStroke(GetColorU32(AzCol_Border),
                              0,
                              g.Style.TabBorderSize);
    }
}

void Az::TabItemLabelAndCloseButton(AzDrawList* draw_list,
                                    const AzRect& bb,
                                    AzTabItemFlags flags,
                                    AzVec2 frame_padding,
                                    const char* label,
                                    AzID tab_id,
                                    AzID close_button_id,
                                    bool is_contents_visible,
                                    bool* out_just_closed,
                                    bool* out_text_clipped)
{
    AzContext& g = *GAz;
    AzVec2 label_size = CalcTextSize(label, NULL, true);

    if (out_just_closed)
        *out_just_closed = false;
    if (out_text_clipped)
        *out_text_clipped = false;

    if (bb.GetWidth() <= 1.0f)
        return;

#if 0
    const float backup_alpha = g.Style.Alpha;
    if (!is_contents_visible)
        g.Style.Alpha *= 0.7f;
#endif

    AzRect text_ellipsis_clip_bb(bb.Min.x + frame_padding.x,
                                 bb.Min.y + frame_padding.y,
                                 bb.Max.x - frame_padding.x,
                                 bb.Max.y);

    if (out_text_clipped)
    {
        *out_text_clipped = (text_ellipsis_clip_bb.Min.x + label_size.x)
                            > text_ellipsis_clip_bb.Max.x;
    }

    const float button_sz = g.FontSize;
    const AzVec2 button_pos(AzMax(bb.Min.x,
                                  bb.Max.x - frame_padding.x - button_sz),
                            bb.Min.y + frame_padding.y);

    bool close_button_pressed = false;
    bool close_button_visible = false;
    bool is_hovered = g.HoveredId == tab_id || g.HoveredId == close_button_id
                      || g.ActiveId == tab_id || g.ActiveId == close_button_id;

    if (close_button_id != 0)
    {
        if (is_contents_visible)
            close_button_visible =
                    (g.Style.TabCloseButtonMinWidthSelected < 0.0f)
                            ? true
                            : (is_hovered
                               && bb.GetWidth() >= AzMax(
                                          button_sz,
                                          g.Style.TabCloseButtonMinWidthSelected));
        else
            close_button_visible =
                    (g.Style.TabCloseButtonMinWidthUnselected < 0.0f)
                            ? true
                            : (is_hovered
                               && bb.GetWidth() >= AzMax(
                                          button_sz,
                                          g.Style.TabCloseButtonMinWidthUnselected));
    }

    const bool unsaved_marker_visible =
            (flags & AzTabItemFlags_UnsavedDocument) != 0
            && (button_pos.x + button_sz <= bb.Max.x)
            && (!close_button_visible || !is_hovered);
    if (unsaved_marker_visible)
    {
        const AzRect bullet_bb(button_pos,
                               button_pos + AzVec2(button_sz, button_sz));
        RenderBullet(draw_list, bullet_bb.GetCenter(), GetColorU32(AzCol_Text));
    } else if (close_button_visible)
    {
        AzLastItemData last_item_backup = g.LastItemData;
        if (CloseButton(close_button_id, button_pos))
            close_button_pressed = true;
        g.LastItemData = last_item_backup;

        if (is_hovered && !(flags & AzTabItemFlags_NoCloseWithMiddleMouseButton)
            && IsMouseClicked(2))
            close_button_pressed = true;
    }

    float ellipsis_max_x = text_ellipsis_clip_bb.Max.x;
    if (close_button_visible || unsaved_marker_visible)
    {
        const bool visible_without_hover =
                unsaved_marker_visible
                || (is_contents_visible
                            ? g.Style.TabCloseButtonMinWidthSelected
                            : g.Style.TabCloseButtonMinWidthUnselected)
                           < 0.0f;
        if (visible_without_hover)
        {
            text_ellipsis_clip_bb.Max.x -= button_sz * 0.90f;
            ellipsis_max_x -= button_sz * 0.90f;
        } else
        {
            text_ellipsis_clip_bb.Max.x -= button_sz * 1.00f;
        }
    }
    LogSetNextTextDecoration("/", "\\");
    RenderTextEllipsis(draw_list,
                       text_ellipsis_clip_bb.Min,
                       text_ellipsis_clip_bb.Max,
                       ellipsis_max_x,
                       label,
                       NULL,
                       &label_size);

#if 0
    if (!is_contents_visible)
        g.Style.Alpha = backup_alpha;
#endif

    if (out_just_closed)
        *out_just_closed = close_button_pressed;
}

#endif
