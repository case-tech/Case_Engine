#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef AZ_DEFINE_MATH_OPERATORS
#define AZ_DEFINE_MATH_OPERATORS
#endif

#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include <Az/AzInternal.hpp>
#ifdef AZ_ENABLE_FREETYPE
#include "misc/freetype/imgui_freetype.h"
#endif

#include <stdint.h>
#include <stdio.h>

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#pragma warning(disable : 4505)
#pragma warning(disable : 4996)
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
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wcomma"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wswitch-default"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wstack-protector"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif

#ifdef AZ_STB_NAMESPACE
namespace AZ_STB_NAMESPACE
{
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4456)
#pragma warning(disable : 6011)
#pragma warning(disable : 6385)
#pragma warning(disable : 28182)
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#ifndef STB_RECT_PACK_IMPLEMENTATION
#ifndef AZ_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STBRP_ASSERT(x)                                                        \
    do                                                                         \
    {                                                                          \
        AZ_ASSERT(x);                                                          \
    } while (0)
#define STBRP_SORT AzQsort
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#ifdef AZ_STB_RECT_PACK_FILENAME
#include AZ_STB_RECT_PACK_FILENAME
#else
#include <Az/azstb_rectpack.hpp>
#endif
#endif

#ifdef AZ_ENABLE_STB_TRUETYPE
#ifndef STB_TRUETYPE_IMPLEMENTATION
#ifndef AZ_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_malloc(x, u) ((void)(u), AZ_ALLOC(x))
#define STBTT_free(x, u) ((void)(u), AZ_FREE(x))
#define STBTT_assert(x)                                                        \
    do                                                                         \
    {                                                                          \
        AZ_ASSERT(x);                                                          \
    } while (0)
#define STBTT_fmod(x, y) AzFmod(x, y)
#define STBTT_sqrt(x) AzSqrt(x)
#define STBTT_pow(x, y) AzPow(x, y)
#define STBTT_fabs(x) AzFabs(x)
#define STBTT_ifloor(x) ((int)AzFloor(x))
#define STBTT_iceil(x) ((int)AzCeil(x))
#define STBTT_strlen(x) AzStrlen(x)
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#ifdef AZ_STB_TRUETYPE_FILENAME
#include AZ_STB_TRUETYPE_FILENAME
#else
#include <Az/azstb_truetype.hpp>
#endif
#endif
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#ifdef AZ_STB_NAMESPACE
}
using namespace AZ_STB_NAMESPACE;
#endif

void Az::StyleColorsDark(AzStyle* dst)
{
    AzStyle* style = dst ? dst : &Az::GetStyle();
    AzVec4* colors = style->Colors;

    colors[AzCol_Text] = AzVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[AzCol_TextDisabled] = AzVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[AzCol_WindowBg] = AzVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[AzCol_ChildBg] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_PopupBg] = AzVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[AzCol_Border] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_BorderShadow] = AzVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[AzCol_FrameBg] = AzVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[AzCol_FrameBgHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_FrameBgActive] = AzVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[AzCol_TitleBg] = AzVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[AzCol_TitleBgActive] = AzVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[AzCol_TitleBgCollapsed] = AzVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[AzCol_MenuBarBg] = AzVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[AzCol_ScrollbarBg] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_ScrollbarGrab] = AzVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[AzCol_ScrollbarGrabHovered] = AzVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[AzCol_ScrollbarGrabActive] = AzVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[AzCol_CheckMark] = AzVec4(0.00f, 0.48f, 0.78f, 1.00f); 
    colors[AzCol_SliderGrab] = AzVec4(0.00f, 0.40f, 0.70f, 1.00f);
    colors[AzCol_SliderGrabActive] = AzVec4(0.00f, 0.50f, 0.80f, 1.00f);
    colors[AzCol_Button] = AzVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[AzCol_ButtonHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_ButtonActive] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_Header] = AzVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[AzCol_HeaderHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_HeaderActive] = AzVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[AzCol_Separator] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_SeparatorHovered] = AzVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[AzCol_SeparatorActive] = AzVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[AzCol_ResizeGrip] = AzVec4(0.20f, 0.20f, 0.20f, 0.25f);
    colors[AzCol_ResizeGripHovered] = AzVec4(0.30f, 0.30f, 0.30f, 0.67f);
    colors[AzCol_ResizeGripActive] = AzVec4(0.40f, 0.40f, 0.40f, 0.95f);
    colors[AzCol_InputTextCursor] = colors[AzCol_Text];
    colors[AzCol_TabHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_TabActive] = AzVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[AzCol_TabUnfocused] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_TabUnfocusedActive] = AzVec4(0.12f, 0.12f, 0.12f, 1.00f);

    colors[AzCol_Tab] = AzVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[AzCol_TabSelected] = AzLerp(colors[AzCol_HeaderActive],
                                       colors[AzCol_TitleBgActive],
                                       0.60f);
    colors[AzCol_TabSelectedOverline] = colors[AzCol_HeaderActive];
    colors[AzCol_TabDimmed] = AzLerp(colors[AzCol_Tab],
                                     colors[AzCol_TitleBg],
                                     0.80f);
    colors[AzCol_TabDimmedSelected] = AzLerp(colors[AzCol_TabSelected],
                                             colors[AzCol_TitleBg],
                                             0.40f);
    colors[AzCol_TabDimmedSelectedOverline] =
            AzVec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[AzCol_PlotLines] = AzVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[AzCol_PlotLinesHovered] = AzVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[AzCol_PlotHistogram] = AzVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[AzCol_PlotHistogramHovered] = AzVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[AzCol_TableHeaderBg] = AzVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[AzCol_TableBorderStrong] = AzVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[AzCol_TableBorderLight] = AzVec4(0.23f, 0.23f, 0.23f, 1.00f);
    colors[AzCol_TableRowBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[AzCol_TableRowBgAlt] = AzVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[AzCol_TextLink] = colors[AzCol_HeaderActive];
    colors[AzCol_TextSelectedBg] = AzVec4(0.00f, 0.48f, 0.78f, 0.35f);
    colors[AzCol_TreeLines] = colors[AzCol_Border];
    colors[AzCol_DragDropTarget] = AzVec4(0.00f, 0.48f, 0.78f, 0.90f);
    colors[AzCol_NavCursor] = AzVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[AzCol_NavWindowingHighlight] = AzVec4(0.80f, 0.80f, 0.80f, 0.30f);
    colors[AzCol_NavWindowingDimBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.60f);
    colors[AzCol_ModalWindowDimBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.50f);
    colors[AzCol_NavHighlight] = AzVec4(0.00f, 0.48f, 0.78f, 1.00f);
    colors[AzCol_DockingEmptyBg] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_DockingPreview] = AzVec4(0.00f, 0.48f, 0.78f, 0.28f);

}

void Az::StyleColorsClassic(AzStyle* dst)
{
    AzStyle* style = dst ? dst : &Az::GetStyle();
    AzVec4* colors = style->Colors;

    colors[AzCol_Text] = AzVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[AzCol_TextDisabled] = AzVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[AzCol_WindowBg] = AzVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[AzCol_ChildBg] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_PopupBg] = AzVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[AzCol_Border] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_BorderShadow] = AzVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[AzCol_FrameBg] = AzVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[AzCol_FrameBgHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_FrameBgActive] = AzVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[AzCol_TitleBg] = AzVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[AzCol_TitleBgActive] = AzVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[AzCol_TitleBgCollapsed] = AzVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[AzCol_MenuBarBg] = AzVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[AzCol_ScrollbarBg] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_ScrollbarGrab] = AzVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[AzCol_ScrollbarGrabHovered] = AzVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[AzCol_ScrollbarGrabActive] = AzVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[AzCol_CheckMark] = AzVec4(0.00f, 0.48f, 0.78f, 1.00f);
    colors[AzCol_SliderGrab] = AzVec4(0.00f, 0.40f, 0.70f, 1.00f);
    colors[AzCol_SliderGrabActive] = AzVec4(0.00f, 0.50f, 0.80f, 1.00f);
    colors[AzCol_Button] = AzVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[AzCol_ButtonHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_ButtonActive] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_Header] = AzVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[AzCol_HeaderHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_HeaderActive] = AzVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[AzCol_Separator] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_SeparatorHovered] = AzVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[AzCol_SeparatorActive] = AzVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[AzCol_ResizeGrip] = AzVec4(0.20f, 0.20f, 0.20f, 0.25f);
    colors[AzCol_ResizeGripHovered] = AzVec4(0.30f, 0.30f, 0.30f, 0.67f);
    colors[AzCol_ResizeGripActive] = AzVec4(0.40f, 0.40f, 0.40f, 0.95f);
    colors[AzCol_InputTextCursor] = colors[AzCol_Text];
    colors[AzCol_TabHovered] = AzVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[AzCol_TabActive] = AzVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[AzCol_TabUnfocused] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_TabUnfocusedActive] = AzVec4(0.12f, 0.12f, 0.12f, 1.00f);

    colors[AzCol_Tab] = AzVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[AzCol_TabSelected] = AzLerp(colors[AzCol_HeaderActive],
                                       colors[AzCol_TitleBgActive],
                                       0.60f);
    colors[AzCol_TabSelectedOverline] = colors[AzCol_HeaderActive];
    colors[AzCol_TabDimmed] = AzLerp(colors[AzCol_Tab],
                                     colors[AzCol_TitleBg],
                                     0.80f);
    colors[AzCol_TabDimmedSelected] = AzLerp(colors[AzCol_TabSelected],
                                             colors[AzCol_TitleBg],
                                             0.40f);
    colors[AzCol_TabDimmedSelectedOverline] =
            AzVec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[AzCol_PlotLines] = AzVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[AzCol_PlotLinesHovered] = AzVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[AzCol_PlotHistogram] = AzVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[AzCol_PlotHistogramHovered] = AzVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[AzCol_TableHeaderBg] = AzVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[AzCol_TableBorderStrong] = AzVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[AzCol_TableBorderLight] = AzVec4(0.23f, 0.23f, 0.23f, 1.00f);
    colors[AzCol_TableRowBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[AzCol_TableRowBgAlt] = AzVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[AzCol_TextLink] = colors[AzCol_HeaderActive];
    colors[AzCol_TextSelectedBg] = AzVec4(0.00f, 0.48f, 0.78f, 0.35f);
    colors[AzCol_TreeLines] = colors[AzCol_Border];
    colors[AzCol_DragDropTarget] = AzVec4(0.00f, 0.48f, 0.78f, 0.90f);
    colors[AzCol_NavCursor] = AzVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[AzCol_NavWindowingHighlight] = AzVec4(0.80f, 0.80f, 0.80f, 0.30f);
    colors[AzCol_NavWindowingDimBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.60f);
    colors[AzCol_ModalWindowDimBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.50f);
    colors[AzCol_NavHighlight] = AzVec4(0.00f, 0.48f, 0.78f, 1.00f);
    colors[AzCol_DockingEmptyBg] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_DockingPreview] = AzVec4(0.00f, 0.48f, 0.78f, 0.28f);
}

void Az::StyleColorsLight(AzStyle* dst)
{
    AzStyle* style = dst ? dst : &Az::GetStyle();
    AzVec4* colors = style->Colors;

    colors[AzCol_Text] = AzVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[AzCol_TextDisabled] =
            AzVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[AzCol_WindowBg] = AzVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[AzCol_ChildBg] = AzVec4(0.98f,
                                   0.98f,
                                   0.98f,
                                   1.00f);
    colors[AzCol_PopupBg] = AzVec4(0.97f, 0.97f, 0.97f, 1.00f);
    colors[AzCol_Border] = AzVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[AzCol_BorderShadow] = AzVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[AzCol_FrameBg] = AzVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[AzCol_FrameBgHovered] = AzVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[AzCol_FrameBgActive] = AzVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[AzCol_TitleBg] = AzVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[AzCol_TitleBgActive] = AzVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[AzCol_TitleBgCollapsed] = AzVec4(0.92f, 0.92f, 0.92f, 0.75f);
    colors[AzCol_MenuBarBg] = AzVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[AzCol_ScrollbarBg] = AzVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[AzCol_ScrollbarGrab] = AzVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[AzCol_ScrollbarGrabHovered] = AzVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[AzCol_ScrollbarGrabActive] = AzVec4(0.55f, 0.55f, 0.55f, 1.00f);
    const AzVec4 accent_color(0.00f, 0.47f, 0.80f, 1.00f);
    colors[AzCol_CheckMark] = accent_color;
    colors[AzCol_SliderGrab] = accent_color;
    colors[AzCol_SliderGrabActive] = AzVec4(0.00f, 0.38f, 0.70f, 1.00f);
    colors[AzCol_Button] = AzVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[AzCol_ButtonHovered] = AzVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[AzCol_ButtonActive] = AzVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[AzCol_Header] = AzVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[AzCol_HeaderHovered] = AzVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[AzCol_HeaderActive] = AzVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[AzCol_Separator] = AzVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[AzCol_SeparatorHovered] = AzVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[AzCol_SeparatorActive] = AzVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[AzCol_ResizeGrip] = AzVec4(0.80f, 0.80f, 0.80f, 0.25f);
    colors[AzCol_ResizeGripHovered] = AzVec4(0.70f, 0.70f, 0.70f, 0.67f);
    colors[AzCol_ResizeGripActive] = AzVec4(0.60f, 0.60f, 0.60f, 0.95f);
    colors[AzCol_Tab] = AzVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[AzCol_TabHovered] = AzVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[AzCol_TabActive] = AzVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[AzCol_TabUnfocused] = AzVec4(0.93f, 0.93f, 0.93f, 1.00f);
    colors[AzCol_TabUnfocusedActive] = AzVec4(0.94f, 0.94f, 0.94f, 1.00f);

    colors[AzCol_TabSelected] = AzVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[AzCol_TabSelectedOverline] = accent_color;
    colors[AzCol_TabDimmed] = AzVec4(0.88f, 0.88f, 0.88f, 1.00f);
    colors[AzCol_TabDimmedSelected] = AzVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[AzCol_TabDimmedSelectedOverline] =
            AzVec4(0.70f, 0.70f, 0.70f, 1.00f);

    colors[AzCol_PlotLines] = AzVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[AzCol_PlotLinesHovered] = AzVec4(0.90f, 0.30f, 0.20f, 1.00f);
    colors[AzCol_PlotHistogram] = AzVec4(0.80f, 0.60f, 0.00f, 1.00f);
    colors[AzCol_PlotHistogramHovered] = AzVec4(0.90f, 0.50f, 0.00f, 1.00f);
    colors[AzCol_TableHeaderBg] = AzVec4(0.88f, 0.88f, 0.88f, 1.00f);
    colors[AzCol_TableBorderStrong] = AzVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[AzCol_TableBorderLight] = AzVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[AzCol_TableRowBg] = AzVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[AzCol_TableRowBgAlt] = AzVec4(0.00f, 0.00f, 0.00f, 0.04f);
    colors[AzCol_TextLink] = accent_color;
    colors[AzCol_TextSelectedBg] = AzVec4(0.00f, 0.47f, 0.80f, 0.25f);
    colors[AzCol_TreeLines] = AzVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[AzCol_DragDropTarget] = AzVec4(0.00f, 0.47f, 0.80f, 0.60f);
    colors[AzCol_NavCursor] = accent_color;
    colors[AzCol_NavWindowingHighlight] = AzVec4(0.20f, 0.20f, 0.20f, 0.30f);
    colors[AzCol_NavWindowingDimBg] = AzVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[AzCol_ModalWindowDimBg] = AzVec4(0.20f, 0.20f, 0.20f, 0.35f);
    colors[AzCol_NavHighlight] = accent_color;
    colors[AzCol_DockingEmptyBg] = AzVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[AzCol_DockingPreview] = AzVec4(0.00f, 0.47f, 0.80f, 0.20f);
    colors[AzCol_InputTextCursor] = colors[AzCol_Text];
}

AzDrawListSharedData::AzDrawListSharedData()
{
    memset(this, 0, sizeof(*this));
    InitialFringeScale = 1.0f;
    for (int i = 0; i < AZ_ARRAYSIZE(ArcFastVtx); i++)
    {
        const float a = ((float)i * 2 * AZ_PI)
                        / (float)AZ_ARRAYSIZE(ArcFastVtx);
        ArcFastVtx[i] = AzVec2(AzCos(a), AzSin(a));
    }
    ArcFastRadiusCutoff = AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(
            AZ_DRAWLIST_ARCFAST_SAMPLE_MAX,
            CircleSegmentMaxError);
}

AzDrawListSharedData::~AzDrawListSharedData()
{
    AZ_ASSERT(DrawLists.Size == 0);
}

void AzDrawListSharedData::SetCircleTessellationMaxError(float max_error)
{
    if (CircleSegmentMaxError == max_error)
        return;

    AZ_ASSERT(max_error > 0.0f);
    CircleSegmentMaxError = max_error;
    for (int i = 0; i < AZ_ARRAYSIZE(CircleSegmentCounts); i++)
    {
        const float radius = (float)i;
        CircleSegmentCounts[i] =
                (AzU8)((i > 0) ? AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(
                                         radius,
                                         CircleSegmentMaxError)
                               : AZ_DRAWLIST_ARCFAST_SAMPLE_MAX);
    }
    ArcFastRadiusCutoff = AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(
            AZ_DRAWLIST_ARCFAST_SAMPLE_MAX,
            CircleSegmentMaxError);
}

AzDrawList::AzDrawList(AzDrawListSharedData* shared_data)
{
    memset(this, 0, sizeof(*this));
    _SetDrawListSharedData(shared_data);
}

AzDrawList::~AzDrawList()
{
    _ClearFreeMemory();
    _SetDrawListSharedData(NULL);
}

void AzDrawList::_SetDrawListSharedData(AzDrawListSharedData* data)
{
    if (_Data != NULL)
        _Data->DrawLists.find_erase_unsorted(this);
    _Data = data;
    if (_Data != NULL)
        _Data->DrawLists.push_back(this);
}

void AzDrawList::_ResetForNewFrame()
{
    AZ_STATIC_ASSERT(offsetof(AzDrawCmd, ClipRect) == 0);
    AZ_STATIC_ASSERT(offsetof(AzDrawCmd, TexRef) == sizeof(AzVec4));
    AZ_STATIC_ASSERT(offsetof(AzDrawCmd, VtxOffset)
                     == sizeof(AzVec4) + sizeof(AzTextureRef));
    if (_Splitter._Count > 1)
        _Splitter.Merge(this);

    CmdBuffer.resize(0);
    IdxBuffer.resize(0);
    VtxBuffer.resize(0);
    Flags = _Data->InitialFlags;
    memset(&_CmdHeader, 0, sizeof(_CmdHeader));
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.resize(0);
    _TextureStack.resize(0);
    _CallbacksDataBuf.resize(0);
    _Path.resize(0);
    _Splitter.Clear();
    CmdBuffer.push_back(AzDrawCmd());
    _FringeScale = _Data->InitialFringeScale;
}

void AzDrawList::_ClearFreeMemory()
{
    CmdBuffer.clear();
    IdxBuffer.clear();
    VtxBuffer.clear();
    Flags = AzDrawListFlags_None;
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.clear();
    _TextureStack.clear();
    _CallbacksDataBuf.clear();
    _Path.clear();
    _Splitter.ClearFreeMemory();
}

AzDrawList* AzDrawList::CloneOutput() const
{
    AzDrawList* dst = AZ_NEW(AzDrawList(_Data));
    dst->CmdBuffer = CmdBuffer;
    dst->IdxBuffer = IdxBuffer;
    dst->VtxBuffer = VtxBuffer;
    dst->Flags = Flags;
    return dst;
}

void AzDrawList::AddDrawCmd()
{
    AzDrawCmd draw_cmd;
    draw_cmd.ClipRect = _CmdHeader.ClipRect;
    draw_cmd.TexRef = _CmdHeader.TexRef;
    draw_cmd.VtxOffset = _CmdHeader.VtxOffset;
    draw_cmd.IdxOffset = IdxBuffer.Size;

    AZ_ASSERT(draw_cmd.ClipRect.x <= draw_cmd.ClipRect.z
              && draw_cmd.ClipRect.y <= draw_cmd.ClipRect.w);
    CmdBuffer.push_back(draw_cmd);
}

void AzDrawList::_PopUnusedDrawCmd()
{
    while (CmdBuffer.Size > 0)
    {
        AzDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
        if (curr_cmd->ElemCount != 0 || curr_cmd->UserCallback != NULL)
            return;
        CmdBuffer.pop_back();
    }
}

void AzDrawList::AddCallback(AzDrawCallback callback,
                             void* userdata,
                             size_t userdata_size)
{
    AZ_ASSERT_PARANOID(CmdBuffer.Size > 0);
    AzDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    AZ_ASSERT(curr_cmd->UserCallback == NULL);
    if (curr_cmd->ElemCount != 0)
    {
        AddDrawCmd();
        curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    }

    curr_cmd->UserCallback = callback;
    if (userdata_size == 0)
    {
        curr_cmd->UserCallbackData = userdata;
        curr_cmd->UserCallbackDataSize = 0;
        curr_cmd->UserCallbackDataOffset = -1;
    } else
    {
        AZ_ASSERT(userdata != NULL);
        AZ_ASSERT(userdata_size < (1u << 31));
        curr_cmd->UserCallbackData = NULL;
        curr_cmd->UserCallbackDataSize = (int)userdata_size;
        curr_cmd->UserCallbackDataOffset = _CallbacksDataBuf.Size;
        _CallbacksDataBuf.resize(_CallbacksDataBuf.Size + (int)userdata_size);
        memcpy(_CallbacksDataBuf.Data
                       + (size_t)curr_cmd->UserCallbackDataOffset,
               userdata,
               userdata_size);
    }

    AddDrawCmd();
}

#define AzDrawCmd_HeaderSize                                                   \
    (offsetof(AzDrawCmd, VtxOffset) + sizeof(unsigned int))
#define AzDrawCmd_HeaderCompare(CMD_LHS, CMD_RHS)                              \
    (memcmp(CMD_LHS, CMD_RHS, AzDrawCmd_HeaderSize))
#define AzDrawCmd_HeaderCopy(CMD_DST, CMD_SRC)                                 \
    (memcpy(CMD_DST, CMD_SRC, AzDrawCmd_HeaderSize))
#define AzDrawCmd_AreSequentialIdxOffset(CMD_0, CMD_1)                         \
    (CMD_0->IdxOffset + CMD_0->ElemCount == CMD_1->IdxOffset)

void AzDrawList::_TryMergeDrawCmds()
{
    AZ_ASSERT_PARANOID(CmdBuffer.Size > 0);
    AzDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    AzDrawCmd* prev_cmd = curr_cmd - 1;
    if (AzDrawCmd_HeaderCompare(curr_cmd, prev_cmd) == 0
        && AzDrawCmd_AreSequentialIdxOffset(prev_cmd, curr_cmd)
        && curr_cmd->UserCallback == NULL && prev_cmd->UserCallback == NULL)
    {
        prev_cmd->ElemCount += curr_cmd->ElemCount;
        CmdBuffer.pop_back();
    }
}

void AzDrawList::_OnChangedClipRect()
{
    AZ_ASSERT_PARANOID(CmdBuffer.Size > 0);
    AzDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount != 0
        && memcmp(&curr_cmd->ClipRect, &_CmdHeader.ClipRect, sizeof(AzVec4))
                   != 0)
    {
        AddDrawCmd();
        return;
    }
    AZ_ASSERT(curr_cmd->UserCallback == NULL);

    AzDrawCmd* prev_cmd = curr_cmd - 1;
    if (curr_cmd->ElemCount == 0 && CmdBuffer.Size > 1
        && AzDrawCmd_HeaderCompare(&_CmdHeader, prev_cmd) == 0
        && AzDrawCmd_AreSequentialIdxOffset(prev_cmd, curr_cmd)
        && prev_cmd->UserCallback == NULL)
    {
        CmdBuffer.pop_back();
        return;
    }
    curr_cmd->ClipRect = _CmdHeader.ClipRect;
}

void AzDrawList::_OnChangedTexture()
{
    AZ_ASSERT_PARANOID(CmdBuffer.Size > 0);
    AzDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount != 0 && curr_cmd->TexRef != _CmdHeader.TexRef)
    {
        AddDrawCmd();
        return;
    }

    if (curr_cmd->UserCallback != NULL)
        return;

    AzDrawCmd* prev_cmd = curr_cmd - 1;
    if (curr_cmd->ElemCount == 0 && CmdBuffer.Size > 1
        && AzDrawCmd_HeaderCompare(&_CmdHeader, prev_cmd) == 0
        && AzDrawCmd_AreSequentialIdxOffset(prev_cmd, curr_cmd)
        && prev_cmd->UserCallback == NULL)
    {
        CmdBuffer.pop_back();
        return;
    }
    curr_cmd->TexRef = _CmdHeader.TexRef;
}

void AzDrawList::_OnChangedVtxOffset()
{
    _VtxCurrentIdx = 0;
    AZ_ASSERT_PARANOID(CmdBuffer.Size > 0);
    AzDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];

    if (curr_cmd->ElemCount != 0)
    {
        AddDrawCmd();
        return;
    }
    AZ_ASSERT(curr_cmd->UserCallback == NULL);
    curr_cmd->VtxOffset = _CmdHeader.VtxOffset;
}

int AzDrawList::_CalcCircleAutoSegmentCount(float radius) const
{
    const int radius_idx = (int)(radius + 0.999999f);
    if (radius_idx >= 0
        && radius_idx < AZ_ARRAYSIZE(_Data->CircleSegmentCounts))
        return _Data->CircleSegmentCounts[radius_idx];
    else
        return AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(
                radius,
                _Data->CircleSegmentMaxError);
}

void AzDrawList::PushClipRect(const AzVec2& cr_min,
                              const AzVec2& cr_max,
                              bool intersect_with_current_clip_rect)
{
    AzVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
    if (intersect_with_current_clip_rect)
    {
        AzVec4 current = _CmdHeader.ClipRect;
        if (cr.x < current.x)
            cr.x = current.x;
        if (cr.y < current.y)
            cr.y = current.y;
        if (cr.z > current.z)
            cr.z = current.z;
        if (cr.w > current.w)
            cr.w = current.w;
    }
    cr.z = AzMax(cr.x, cr.z);
    cr.w = AzMax(cr.y, cr.w);

    _ClipRectStack.push_back(cr);
    _CmdHeader.ClipRect = cr;
    _OnChangedClipRect();
}

void AzDrawList::PushClipRectFullScreen()
{
    PushClipRect(AzVec2(_Data->ClipRectFullscreen.x,
                        _Data->ClipRectFullscreen.y),
                 AzVec2(_Data->ClipRectFullscreen.z,
                        _Data->ClipRectFullscreen.w));
}

void AzDrawList::PopClipRect()
{
    _ClipRectStack.pop_back();
    _CmdHeader.ClipRect =
            (_ClipRectStack.Size == 0)
                    ? _Data->ClipRectFullscreen
                    : _ClipRectStack.Data[_ClipRectStack.Size - 1];
    _OnChangedClipRect();
}

void AzDrawList::PushTexture(AzTextureRef tex_ref)
{
    _TextureStack.push_back(tex_ref);
    _CmdHeader.TexRef = tex_ref;
    if (tex_ref._TexData != NULL)
        AZ_ASSERT(tex_ref._TexData->WantDestroyNextFrame == false);
    _OnChangedTexture();
}

void AzDrawList::PopTexture()
{
    _TextureStack.pop_back();
    _CmdHeader.TexRef = (_TextureStack.Size == 0)
                                ? AzTextureRef()
                                : _TextureStack.Data[_TextureStack.Size - 1];
    _OnChangedTexture();
}

void AzDrawList::_SetTexture(AzTextureRef tex_ref)
{
    if (_CmdHeader.TexRef == tex_ref)
        return;
    _CmdHeader.TexRef = tex_ref;
    _OnChangedTexture();
}

void AzDrawList::PrimReserve(int idx_count, int vtx_count)
{
    AZ_ASSERT_PARANOID(idx_count >= 0 && vtx_count >= 0);
    if (sizeof(AzDrawIdx) == 2 && (_VtxCurrentIdx + vtx_count >= (1 << 16))
        && (Flags & AzDrawListFlags_AllowVtxOffset))
    {
        _CmdHeader.VtxOffset = VtxBuffer.Size;
        _OnChangedVtxOffset();
    }

    AzDrawCmd* draw_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    draw_cmd->ElemCount += idx_count;

    int vtx_buffer_old_size = VtxBuffer.Size;
    VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
    _VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

    int idx_buffer_old_size = IdxBuffer.Size;
    IdxBuffer.resize(idx_buffer_old_size + idx_count);
    _IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

void AzDrawList::PrimUnreserve(int idx_count, int vtx_count)
{
    AZ_ASSERT_PARANOID(idx_count >= 0 && vtx_count >= 0);

    AzDrawCmd* draw_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    draw_cmd->ElemCount -= idx_count;
    VtxBuffer.shrink(VtxBuffer.Size - vtx_count);
    IdxBuffer.shrink(IdxBuffer.Size - idx_count);
}

void AzDrawList::PrimRect(const AzVec2& a, const AzVec2& c, AzU32 col)
{
    AzVec2 b(c.x, a.y), d(a.x, c.y), uv(_Data->TexUvWhitePixel);
    AzDrawIdx idx = (AzDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx;
    _IdxWritePtr[1] = (AzDrawIdx)(idx + 1);
    _IdxWritePtr[2] = (AzDrawIdx)(idx + 2);
    _IdxWritePtr[3] = idx;
    _IdxWritePtr[4] = (AzDrawIdx)(idx + 2);
    _IdxWritePtr[5] = (AzDrawIdx)(idx + 3);
    _VtxWritePtr[0].pos = a;
    _VtxWritePtr[0].uv = uv;
    _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b;
    _VtxWritePtr[1].uv = uv;
    _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c;
    _VtxWritePtr[2].uv = uv;
    _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d;
    _VtxWritePtr[3].uv = uv;
    _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void AzDrawList::PrimRectUV(const AzVec2& a,
                            const AzVec2& c,
                            const AzVec2& uv_a,
                            const AzVec2& uv_c,
                            AzU32 col)
{
    AzVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
    AzDrawIdx idx = (AzDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx;
    _IdxWritePtr[1] = (AzDrawIdx)(idx + 1);
    _IdxWritePtr[2] = (AzDrawIdx)(idx + 2);
    _IdxWritePtr[3] = idx;
    _IdxWritePtr[4] = (AzDrawIdx)(idx + 2);
    _IdxWritePtr[5] = (AzDrawIdx)(idx + 3);
    _VtxWritePtr[0].pos = a;
    _VtxWritePtr[0].uv = uv_a;
    _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b;
    _VtxWritePtr[1].uv = uv_b;
    _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c;
    _VtxWritePtr[2].uv = uv_c;
    _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d;
    _VtxWritePtr[3].uv = uv_d;
    _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void AzDrawList::PrimQuadUV(const AzVec2& a,
                            const AzVec2& b,
                            const AzVec2& c,
                            const AzVec2& d,
                            const AzVec2& uv_a,
                            const AzVec2& uv_b,
                            const AzVec2& uv_c,
                            const AzVec2& uv_d,
                            AzU32 col)
{
    AzDrawIdx idx = (AzDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx;
    _IdxWritePtr[1] = (AzDrawIdx)(idx + 1);
    _IdxWritePtr[2] = (AzDrawIdx)(idx + 2);
    _IdxWritePtr[3] = idx;
    _IdxWritePtr[4] = (AzDrawIdx)(idx + 2);
    _IdxWritePtr[5] = (AzDrawIdx)(idx + 3);
    _VtxWritePtr[0].pos = a;
    _VtxWritePtr[0].uv = uv_a;
    _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b;
    _VtxWritePtr[1].uv = uv_b;
    _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c;
    _VtxWritePtr[2].uv = uv_c;
    _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d;
    _VtxWritePtr[3].uv = uv_d;
    _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

#define AZ_NORMALIZE2F_OVER_ZERO(VX, VY)                                       \
    {                                                                          \
        float d2 = VX * VX + VY * VY;                                          \
        if (d2 > 0.0f)                                                         \
        {                                                                      \
            float inv_len = AzRsqrt(d2);                                       \
            VX *= inv_len;                                                     \
            VY *= inv_len;                                                     \
        }                                                                      \
    }                                                                          \
    (void)0
#define AZ_FIXNORMAL2F_MAX_INVLEN2 100.0f
#define AZ_FIXNORMAL2F(VX, VY)                                                 \
    {                                                                          \
        float d2 = VX * VX + VY * VY;                                          \
        if (d2 > 0.000001f)                                                    \
        {                                                                      \
            float inv_len2 = 1.0f / d2;                                        \
            if (inv_len2 > AZ_FIXNORMAL2F_MAX_INVLEN2)                         \
                inv_len2 = AZ_FIXNORMAL2F_MAX_INVLEN2;                         \
            VX *= inv_len2;                                                    \
            VY *= inv_len2;                                                    \
        }                                                                      \
    }                                                                          \
    (void)0

void AzDrawList::AddPolyline(const AzVec2* points,
                             const int points_count,
                             AzU32 col,
                             AzDrawFlags flags,
                             float thickness)
{
    if (points_count < 2 || (col & AZ_COL32_A_MASK) == 0)
        return;

    const bool closed = (flags & AzDrawFlags_Closed) != 0;
    const AzVec2 opaque_uv = _Data->TexUvWhitePixel;
    const int count = closed ? points_count : points_count - 1;
    const bool thick_line = (thickness > _FringeScale);

    if (Flags & AzDrawListFlags_AntiAliasedLines)
    {
        const float AA_SIZE = _FringeScale;
        const AzU32 col_trans = col & ~AZ_COL32_A_MASK;

        thickness = AzMax(thickness, 1.0f);
        const int integer_thickness = (int)thickness;
        const float fractional_thickness = thickness - integer_thickness;

        const bool use_texture =
                (Flags & AzDrawListFlags_AntiAliasedLinesUseTex)
                && (integer_thickness < AZ_DRAWLIST_TEX_LINES_WIDTH_MAX)
                && (fractional_thickness <= 0.00001f) && (AA_SIZE == 1.0f);

        AZ_ASSERT_PARANOID(!use_texture
                           || !(_Data->Font->ContainerAtlas->Flags
                                & AzFontAtlasFlags_NoBakedLines));

        const int idx_count = use_texture
                                      ? (count * 6)
                                      : (thick_line ? count * 18 : count * 12);
        const int vtx_count = use_texture ? (points_count * 2)
                                          : (thick_line ? points_count * 4
                                                        : points_count * 3);
        PrimReserve(idx_count, vtx_count);

        _Data->TempBuffer.reserve_discard(
                points_count * ((use_texture || !thick_line) ? 3 : 5));
        AzVec2* temp_normals = _Data->TempBuffer.Data;
        AzVec2* temp_points = temp_normals + points_count;

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            AZ_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count - 1] = temp_normals[points_count - 2];

        if (use_texture || !thick_line)
        {
            const float half_draw_size = use_texture ? ((thickness * 0.5f) + 1)
                                                     : AA_SIZE;

            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * half_draw_size;
                temp_points[1] = points[0] - temp_normals[0] * half_draw_size;
                temp_points[(points_count - 1) * 2 + 0] =
                        points[points_count - 1]
                        + temp_normals[points_count - 1] * half_draw_size;
                temp_points[(points_count - 1) * 2 + 1] =
                        points[points_count - 1]
                        - temp_normals[points_count - 1] * half_draw_size;
            }

            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
                const unsigned int idx2 = ((i1 + 1) == points_count)
                                                  ? _VtxCurrentIdx
                                                  : (idx1
                                                     + (use_texture ? 2 : 3));

                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                AZ_FIXNORMAL2F(dm_x, dm_y);
                dm_x *= half_draw_size;
                dm_y *= half_draw_size;

                AzVec2* out_vtx = &temp_points[i2 * 2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                if (use_texture)
                {
                    _IdxWritePtr[0] = (AzDrawIdx)(idx2 + 0);
                    _IdxWritePtr[1] = (AzDrawIdx)(idx1 + 0);
                    _IdxWritePtr[2] = (AzDrawIdx)(idx1 + 1);
                    _IdxWritePtr[3] = (AzDrawIdx)(idx2 + 1);
                    _IdxWritePtr[4] = (AzDrawIdx)(idx1 + 1);
                    _IdxWritePtr[5] = (AzDrawIdx)(idx2 + 0);
                    _IdxWritePtr += 6;
                } else
                {
                    _IdxWritePtr[0] = (AzDrawIdx)(idx2 + 0);
                    _IdxWritePtr[1] = (AzDrawIdx)(idx1 + 0);
                    _IdxWritePtr[2] = (AzDrawIdx)(idx1 + 2);
                    _IdxWritePtr[3] = (AzDrawIdx)(idx1 + 2);
                    _IdxWritePtr[4] = (AzDrawIdx)(idx2 + 2);
                    _IdxWritePtr[5] = (AzDrawIdx)(idx2 + 0);
                    _IdxWritePtr[6] = (AzDrawIdx)(idx2 + 1);
                    _IdxWritePtr[7] = (AzDrawIdx)(idx1 + 1);
                    _IdxWritePtr[8] = (AzDrawIdx)(idx1 + 0);
                    _IdxWritePtr[9] = (AzDrawIdx)(idx1 + 0);
                    _IdxWritePtr[10] = (AzDrawIdx)(idx2 + 0);
                    _IdxWritePtr[11] = (AzDrawIdx)(idx2 + 1);
                    _IdxWritePtr += 12;
                }

                idx1 = idx2;
            }

            if (use_texture)
            {
                AzVec4 tex_uvs = _Data->TexUvLines[integer_thickness];

                AzVec2 tex_uv0(tex_uvs.x, tex_uvs.y);
                AzVec2 tex_uv1(tex_uvs.z, tex_uvs.w);
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = temp_points[i * 2 + 0];
                    _VtxWritePtr[0].uv = tex_uv0;
                    _VtxWritePtr[0].col = col;
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 1];
                    _VtxWritePtr[1].uv = tex_uv1;
                    _VtxWritePtr[1].col = col;
                    _VtxWritePtr += 2;
                }
            } else
            {
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = points[i];
                    _VtxWritePtr[0].uv = opaque_uv;
                    _VtxWritePtr[0].col = col;
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 0];
                    _VtxWritePtr[1].uv = opaque_uv;
                    _VtxWritePtr[1].col = col_trans;
                    _VtxWritePtr[2].pos = temp_points[i * 2 + 1];
                    _VtxWritePtr[2].uv = opaque_uv;
                    _VtxWritePtr[2].col = col_trans;
                    _VtxWritePtr += 3;
                }
            }
        } else
        {
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;

            if (!closed)
            {
                const int points_last = points_count - 1;
                temp_points[0] = points[0]
                                 + temp_normals[0]
                                           * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0]
                                 + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0]
                                 - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0]
                                 - temp_normals[0]
                                           * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 0] =
                        points[points_last]
                        + temp_normals[points_last]
                                  * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 1] =
                        points[points_last]
                        + temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 2] =
                        points[points_last]
                        - temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 3] =
                        points[points_last]
                        - temp_normals[points_last]
                                  * (half_inner_thickness + AA_SIZE);
            }

            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1 + 1) == points_count ? 0 : (i1 + 1);
                const unsigned int idx2 = (i1 + 1) == points_count
                                                  ? _VtxCurrentIdx
                                                  : (idx1 + 4);

                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                AZ_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                AzVec2* out_vtx = &temp_points[i2 * 4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                _IdxWritePtr[0] = (AzDrawIdx)(idx2 + 1);
                _IdxWritePtr[1] = (AzDrawIdx)(idx1 + 1);
                _IdxWritePtr[2] = (AzDrawIdx)(idx1 + 2);
                _IdxWritePtr[3] = (AzDrawIdx)(idx1 + 2);
                _IdxWritePtr[4] = (AzDrawIdx)(idx2 + 2);
                _IdxWritePtr[5] = (AzDrawIdx)(idx2 + 1);
                _IdxWritePtr[6] = (AzDrawIdx)(idx2 + 1);
                _IdxWritePtr[7] = (AzDrawIdx)(idx1 + 1);
                _IdxWritePtr[8] = (AzDrawIdx)(idx1 + 0);
                _IdxWritePtr[9] = (AzDrawIdx)(idx1 + 0);
                _IdxWritePtr[10] = (AzDrawIdx)(idx2 + 0);
                _IdxWritePtr[11] = (AzDrawIdx)(idx2 + 1);
                _IdxWritePtr[12] = (AzDrawIdx)(idx2 + 2);
                _IdxWritePtr[13] = (AzDrawIdx)(idx1 + 2);
                _IdxWritePtr[14] = (AzDrawIdx)(idx1 + 3);
                _IdxWritePtr[15] = (AzDrawIdx)(idx1 + 3);
                _IdxWritePtr[16] = (AzDrawIdx)(idx2 + 3);
                _IdxWritePtr[17] = (AzDrawIdx)(idx2 + 2);
                _IdxWritePtr += 18;

                idx1 = idx2;
            }

            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = temp_points[i * 4 + 0];
                _VtxWritePtr[0].uv = opaque_uv;
                _VtxWritePtr[0].col = col_trans;
                _VtxWritePtr[1].pos = temp_points[i * 4 + 1];
                _VtxWritePtr[1].uv = opaque_uv;
                _VtxWritePtr[1].col = col;
                _VtxWritePtr[2].pos = temp_points[i * 4 + 2];
                _VtxWritePtr[2].uv = opaque_uv;
                _VtxWritePtr[2].col = col;
                _VtxWritePtr[3].pos = temp_points[i * 4 + 3];
                _VtxWritePtr[3].uv = opaque_uv;
                _VtxWritePtr[3].col = col_trans;
                _VtxWritePtr += 4;
            }
        }
        _VtxCurrentIdx += (AzDrawIdx)vtx_count;
    } else
    {
        const int idx_count = count * 6;
        const int vtx_count = count * 4;
        PrimReserve(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            const AzVec2& p1 = points[i1];
            const AzVec2& p2 = points[i2];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            AZ_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            _VtxWritePtr[0].pos.x = p1.x + dy;
            _VtxWritePtr[0].pos.y = p1.y - dx;
            _VtxWritePtr[0].uv = opaque_uv;
            _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = p2.x + dy;
            _VtxWritePtr[1].pos.y = p2.y - dx;
            _VtxWritePtr[1].uv = opaque_uv;
            _VtxWritePtr[1].col = col;
            _VtxWritePtr[2].pos.x = p2.x - dy;
            _VtxWritePtr[2].pos.y = p2.y + dx;
            _VtxWritePtr[2].uv = opaque_uv;
            _VtxWritePtr[2].col = col;
            _VtxWritePtr[3].pos.x = p1.x - dy;
            _VtxWritePtr[3].pos.y = p1.y + dx;
            _VtxWritePtr[3].uv = opaque_uv;
            _VtxWritePtr[3].col = col;
            _VtxWritePtr += 4;

            _IdxWritePtr[0] = (AzDrawIdx)(_VtxCurrentIdx);
            _IdxWritePtr[1] = (AzDrawIdx)(_VtxCurrentIdx + 1);
            _IdxWritePtr[2] = (AzDrawIdx)(_VtxCurrentIdx + 2);
            _IdxWritePtr[3] = (AzDrawIdx)(_VtxCurrentIdx);
            _IdxWritePtr[4] = (AzDrawIdx)(_VtxCurrentIdx + 2);
            _IdxWritePtr[5] = (AzDrawIdx)(_VtxCurrentIdx + 3);
            _IdxWritePtr += 6;
            _VtxCurrentIdx += 4;
        }
    }
}

void AzDrawList::AddConvexPolyFilled(const AzVec2* points,
                                     const int points_count,
                                     AzU32 col)
{
    if (points_count < 3 || (col & AZ_COL32_A_MASK) == 0)
        return;

    const AzVec2 uv = _Data->TexUvWhitePixel;

    if (Flags & AzDrawListFlags_AntiAliasedFill)
    {
        const float AA_SIZE = _FringeScale;
        const AzU32 col_trans = col & ~AZ_COL32_A_MASK;
        const int idx_count = (points_count - 2) * 3 + points_count * 6;
        const int vtx_count = (points_count * 2);
        PrimReserve(idx_count, vtx_count);

        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (AzDrawIdx)(vtx_inner_idx);
            _IdxWritePtr[1] = (AzDrawIdx)(vtx_inner_idx + ((i - 1) << 1));
            _IdxWritePtr[2] = (AzDrawIdx)(vtx_inner_idx + (i << 1));
            _IdxWritePtr += 3;
        }

        _Data->TempBuffer.reserve_discard(points_count);
        AzVec2* temp_normals = _Data->TempBuffer.Data;
        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const AzVec2& p0 = points[i0];
            const AzVec2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            AZ_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const AzVec2& n0 = temp_normals[i0];
            const AzVec2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            AZ_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x);
            _VtxWritePtr[0].pos.y = (points[i1].y - dm_y);
            _VtxWritePtr[0].uv = uv;
            _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x);
            _VtxWritePtr[1].pos.y = (points[i1].y + dm_y);
            _VtxWritePtr[1].uv = uv;
            _VtxWritePtr[1].col = col_trans;
            _VtxWritePtr += 2;

            _IdxWritePtr[0] = (AzDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr[1] = (AzDrawIdx)(vtx_inner_idx + (i0 << 1));
            _IdxWritePtr[2] = (AzDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[3] = (AzDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[4] = (AzDrawIdx)(vtx_outer_idx + (i1 << 1));
            _IdxWritePtr[5] = (AzDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (AzDrawIdx)vtx_count;
    } else
    {
        const int idx_count = (points_count - 2) * 3;
        const int vtx_count = points_count;
        PrimReserve(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _VtxWritePtr[0].pos = points[i];
            _VtxWritePtr[0].uv = uv;
            _VtxWritePtr[0].col = col;
            _VtxWritePtr++;
        }
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (AzDrawIdx)(_VtxCurrentIdx);
            _IdxWritePtr[1] = (AzDrawIdx)(_VtxCurrentIdx + i - 1);
            _IdxWritePtr[2] = (AzDrawIdx)(_VtxCurrentIdx + i);
            _IdxWritePtr += 3;
        }
        _VtxCurrentIdx += (AzDrawIdx)vtx_count;
    }
}

void AzDrawList::_PathArcToFastEx(const AzVec2& center,
                                  float radius,
                                  int a_min_sample,
                                  int a_max_sample,
                                  int a_step)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }

    if (a_step <= 0)
        a_step = AZ_DRAWLIST_ARCFAST_SAMPLE_MAX
                 / _CalcCircleAutoSegmentCount(radius);

    a_step = AzClamp(a_step, 1, AZ_DRAWLIST_ARCFAST_TABLE_SIZE / 4);

    const int sample_range = AzAbs(a_max_sample - a_min_sample);
    const int a_next_step = a_step;

    int samples = sample_range + 1;
    bool extra_max_sample = false;
    if (a_step > 1)
    {
        samples = sample_range / a_step + 1;
        const int overstep = sample_range % a_step;

        if (overstep > 0)
        {
            extra_max_sample = true;
            samples++;

            if (sample_range > 0)
                a_step -= (a_step - overstep) / 2;
        }
    }

    _Path.resize(_Path.Size + samples);
    AzVec2* out_ptr = _Path.Data + (_Path.Size - samples);

    int sample_index = a_min_sample;
    if (sample_index < 0 || sample_index >= AZ_DRAWLIST_ARCFAST_SAMPLE_MAX)
    {
        sample_index = sample_index % AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (sample_index < 0)
            sample_index += AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;
    }

    if (a_max_sample >= a_min_sample)
    {
        for (int a = a_min_sample; a <= a_max_sample;
             a += a_step, sample_index += a_step, a_step = a_next_step)
        {
            if (sample_index >= AZ_DRAWLIST_ARCFAST_SAMPLE_MAX)
                sample_index -= AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const AzVec2 s = _Data->ArcFastVtx[sample_index];
            out_ptr->x = center.x + s.x * radius;
            out_ptr->y = center.y + s.y * radius;
            out_ptr++;
        }
    } else
    {
        for (int a = a_min_sample; a >= a_max_sample;
             a -= a_step, sample_index -= a_step, a_step = a_next_step)
        {
            if (sample_index < 0)
                sample_index += AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const AzVec2 s = _Data->ArcFastVtx[sample_index];
            out_ptr->x = center.x + s.x * radius;
            out_ptr->y = center.y + s.y * radius;
            out_ptr++;
        }
    }

    if (extra_max_sample)
    {
        int normalized_max_sample = a_max_sample
                                    % AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (normalized_max_sample < 0)
            normalized_max_sample += AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;

        const AzVec2 s = _Data->ArcFastVtx[normalized_max_sample];
        out_ptr->x = center.x + s.x * radius;
        out_ptr->y = center.y + s.y * radius;
        out_ptr++;
    }

    AZ_ASSERT_PARANOID(_Path.Data + _Path.Size == out_ptr);
}

void AzDrawList::_PathArcToN(const AzVec2& center,
                             float radius,
                             float a_min,
                             float a_max,
                             int num_segments)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }

    _Path.reserve(_Path.Size + (num_segments + 1));
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min
                        + ((float)i / (float)num_segments) * (a_max - a_min);
        _Path.push_back(AzVec2(center.x + AzCos(a) * radius,
                               center.y + AzSin(a) * radius));
    }
}

void AzDrawList::PathArcToFast(const AzVec2& center,
                               float radius,
                               int a_min_of_12,
                               int a_max_of_12)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }
    _PathArcToFastEx(center,
                     radius,
                     a_min_of_12 * AZ_DRAWLIST_ARCFAST_SAMPLE_MAX / 12,
                     a_max_of_12 * AZ_DRAWLIST_ARCFAST_SAMPLE_MAX / 12,
                     0);
}

void AzDrawList::PathArcTo(const AzVec2& center,
                           float radius,
                           float a_min,
                           float a_max,
                           int num_segments)
{
    if (radius < 0.5f)
    {
        _Path.push_back(center);
        return;
    }

    if (num_segments > 0)
    {
        _PathArcToN(center, radius, a_min, a_max, num_segments);
        return;
    }

    if (radius <= _Data->ArcFastRadiusCutoff)
    {
        const bool a_is_reverse = a_max < a_min;

        const float a_min_sample_f = AZ_DRAWLIST_ARCFAST_SAMPLE_MAX * a_min
                                     / (AZ_PI * 2.0f);
        const float a_max_sample_f = AZ_DRAWLIST_ARCFAST_SAMPLE_MAX * a_max
                                     / (AZ_PI * 2.0f);

        const int a_min_sample = a_is_reverse ? (int)AzFloor(a_min_sample_f)
                                              : (int)AzCeil(a_min_sample_f);
        const int a_max_sample = a_is_reverse ? (int)AzCeil(a_max_sample_f)
                                              : (int)AzFloor(a_max_sample_f);
        const int a_mid_samples = a_is_reverse
                                          ? AzMax(a_min_sample - a_max_sample,
                                                  0)
                                          : AzMax(a_max_sample - a_min_sample,
                                                  0);

        const float a_min_segment_angle = a_min_sample * AZ_PI * 2.0f
                                          / AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const float a_max_segment_angle = a_max_sample * AZ_PI * 2.0f
                                          / AZ_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const bool a_emit_start = AzAbs(a_min_segment_angle - a_min) >= 1e-5f;
        const bool a_emit_end = AzAbs(a_max - a_max_segment_angle) >= 1e-5f;

        _Path.reserve(_Path.Size
                      + (a_mid_samples + 1 + (a_emit_start ? 1 : 0)
                         + (a_emit_end ? 1 : 0)));
        if (a_emit_start)
            _Path.push_back(AzVec2(center.x + AzCos(a_min) * radius,
                                   center.y + AzSin(a_min) * radius));
        if (a_mid_samples > 0)
            _PathArcToFastEx(center, radius, a_min_sample, a_max_sample, 0);
        if (a_emit_end)
            _Path.push_back(AzVec2(center.x + AzCos(a_max) * radius,
                                   center.y + AzSin(a_max) * radius));
    } else
    {
        const float arc_length = AzAbs(a_max - a_min);
        const int circle_segment_count = _CalcCircleAutoSegmentCount(radius);
        const int arc_segment_count = AzMax(
                (int)AzCeil(circle_segment_count * arc_length / (AZ_PI * 2.0f)),
                (int)(2.0f * AZ_PI / arc_length));
        _PathArcToN(center, radius, a_min, a_max, arc_segment_count);
    }
}

void AzDrawList::PathEllipticalArcTo(const AzVec2& center,
                                     const AzVec2& radius,
                                     float rot,
                                     float a_min,
                                     float a_max,
                                     int num_segments)
{
    if (num_segments <= 0)
        num_segments = _CalcCircleAutoSegmentCount(AzMax(radius.x, radius.y));

    _Path.reserve(_Path.Size + (num_segments + 1));

    const float cos_rot = AzCos(rot);
    const float sin_rot = AzSin(rot);
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min
                        + ((float)i / (float)num_segments) * (a_max - a_min);
        AzVec2 point(AzCos(a) * radius.x, AzSin(a) * radius.y);
        const AzVec2 rel((point.x * cos_rot) - (point.y * sin_rot),
                         (point.x * sin_rot) + (point.y * cos_rot));
        point.x = rel.x + center.x;
        point.y = rel.y + center.y;
        _Path.push_back(point);
    }
}

AzVec2 AzBezierCubicCalc(const AzVec2& p1,
                         const AzVec2& p2,
                         const AzVec2& p3,
                         const AzVec2& p4,
                         float t)
{
    float u = 1.0f - t;
    float w1 = u * u * u;
    float w2 = 3 * u * u * t;
    float w3 = 3 * u * t * t;
    float w4 = t * t * t;
    return AzVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x,
                  w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y);
}

AzVec2 AzBezierQuadraticCalc(const AzVec2& p1,
                             const AzVec2& p2,
                             const AzVec2& p3,
                             float t)
{
    float u = 1.0f - t;
    float w1 = u * u;
    float w2 = 2 * u * t;
    float w3 = t * t;
    return AzVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x,
                  w1 * p1.y + w2 * p2.y + w3 * p3.y);
}

static void PathBezierCubicCurveToCasteljau(AzVector<AzVec2>* path,
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
    float d2 = (x2 - x4) * dy - (y2 - y4) * dx;
    float d3 = (x3 - x4) * dy - (y3 - y4) * dx;
    d2 = (d2 >= 0) ? d2 : -d2;
    d3 = (d3 >= 0) ? d3 : -d3;
    if ((d2 + d3) * (d2 + d3) < tess_tol * (dx * dx + dy * dy))
    {
        path->push_back(AzVec2(x4, y4));
    } else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x34 = (x3 + x4) * 0.5f, y34 = (y3 + y4) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
        float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
        PathBezierCubicCurveToCasteljau(path,
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
        PathBezierCubicCurveToCasteljau(path,
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

static void PathBezierQuadraticCurveToCasteljau(AzVector<AzVec2>* path,
                                                float x1,
                                                float y1,
                                                float x2,
                                                float y2,
                                                float x3,
                                                float y3,
                                                float tess_tol,
                                                int level)
{
    float dx = x3 - x1, dy = y3 - y1;
    float det = (x2 - x3) * dy - (y2 - y3) * dx;
    if (det * det * 4.0f < tess_tol * (dx * dx + dy * dy))
    {
        path->push_back(AzVec2(x3, y3));
    } else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        PathBezierQuadraticCurveToCasteljau(path,
                                            x1,
                                            y1,
                                            x12,
                                            y12,
                                            x123,
                                            y123,
                                            tess_tol,
                                            level + 1);
        PathBezierQuadraticCurveToCasteljau(path,
                                            x123,
                                            y123,
                                            x23,
                                            y23,
                                            x3,
                                            y3,
                                            tess_tol,
                                            level + 1);
    }
}

void AzDrawList::PathBezierCubicCurveTo(const AzVec2& p2,
                                        const AzVec2& p3,
                                        const AzVec2& p4,
                                        int num_segments)
{
    AzVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        AZ_ASSERT(_Data->CurveTessellationTol > 0.0f);
        PathBezierCubicCurveToCasteljau(&_Path,
                                        p1.x,
                                        p1.y,
                                        p2.x,
                                        p2.y,
                                        p3.x,
                                        p3.y,
                                        p4.x,
                                        p4.y,
                                        _Data->CurveTessellationTol,
                                        0);
    } else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
            _Path.push_back(AzBezierCubicCalc(p1, p2, p3, p4, t_step * i_step));
    }
}

void AzDrawList::PathBezierQuadraticCurveTo(const AzVec2& p2,
                                            const AzVec2& p3,
                                            int num_segments)
{
    AzVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        AZ_ASSERT(_Data->CurveTessellationTol > 0.0f);
        PathBezierQuadraticCurveToCasteljau(&_Path,
                                            p1.x,
                                            p1.y,
                                            p2.x,
                                            p2.y,
                                            p3.x,
                                            p3.y,
                                            _Data->CurveTessellationTol,
                                            0);
    } else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
            _Path.push_back(AzBezierQuadraticCalc(p1, p2, p3, t_step * i_step));
    }
}

static inline AzDrawFlags FixRectCornerFlags(AzDrawFlags flags)
{
    AZ_ASSERT((flags & 0x0F) == 0
              && "Misuse of legacy hardcoded AzDrawCornerFlags values!");

    if ((flags & AzDrawFlags_RoundCornersMask_) == 0)
        flags |= AzDrawFlags_RoundCornersAll;

    return flags;
}

void AzDrawList::PathRect(const AzVec2& a,
                          const AzVec2& b,
                          float rounding,
                          AzDrawFlags flags)
{
    if (rounding >= 0.5f)
    {
        flags = FixRectCornerFlags(flags);
        rounding = AzMin(
                rounding,
                AzFabs(b.x - a.x)
                                * (((flags & AzDrawFlags_RoundCornersTop)
                                    == AzDrawFlags_RoundCornersTop)
                                                   || ((flags
                                                        & AzDrawFlags_RoundCornersBottom)
                                                       == AzDrawFlags_RoundCornersBottom)
                                           ? 0.5f
                                           : 1.0f)
                        - 1.0f);
        rounding = AzMin(
                rounding,
                AzFabs(b.y - a.y)
                                * (((flags & AzDrawFlags_RoundCornersLeft)
                                    == AzDrawFlags_RoundCornersLeft)
                                                   || ((flags
                                                        & AzDrawFlags_RoundCornersRight)
                                                       == AzDrawFlags_RoundCornersRight)
                                           ? 0.5f
                                           : 1.0f)
                        - 1.0f);
    }
    if (rounding < 0.5f
        || (flags & AzDrawFlags_RoundCornersMask_)
                   == AzDrawFlags_RoundCornersNone)
    {
        PathLineTo(a);
        PathLineTo(AzVec2(b.x, a.y));
        PathLineTo(b);
        PathLineTo(AzVec2(a.x, b.y));
    } else
    {
        const float rounding_tl = (flags & AzDrawFlags_RoundCornersTopLeft)
                                          ? rounding
                                          : 0.0f;
        const float rounding_tr = (flags & AzDrawFlags_RoundCornersTopRight)
                                          ? rounding
                                          : 0.0f;
        const float rounding_br = (flags & AzDrawFlags_RoundCornersBottomRight)
                                          ? rounding
                                          : 0.0f;
        const float rounding_bl = (flags & AzDrawFlags_RoundCornersBottomLeft)
                                          ? rounding
                                          : 0.0f;
        PathArcToFast(AzVec2(a.x + rounding_tl, a.y + rounding_tl),
                      rounding_tl,
                      6,
                      9);
        PathArcToFast(AzVec2(b.x - rounding_tr, a.y + rounding_tr),
                      rounding_tr,
                      9,
                      12);
        PathArcToFast(AzVec2(b.x - rounding_br, b.y - rounding_br),
                      rounding_br,
                      0,
                      3);
        PathArcToFast(AzVec2(a.x + rounding_bl, b.y - rounding_bl),
                      rounding_bl,
                      3,
                      6);
    }
}

void AzDrawList::AddLine(const AzVec2& p1,
                         const AzVec2& p2,
                         AzU32 col,
                         float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;
    PathLineTo(p1 + AzVec2(0.5f, 0.5f));
    PathLineTo(p2 + AzVec2(0.5f, 0.5f));
    PathStroke(col, 0, thickness);
}

void AzDrawList::AddRect(const AzVec2& p_min,
                         const AzVec2& p_max,
                         AzU32 col,
                         float rounding,
                         AzDrawFlags flags,
                         float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;
    if (Flags & AzDrawListFlags_AntiAliasedLines)
        PathRect(p_min + AzVec2(0.50f, 0.50f),
                 p_max - AzVec2(0.50f, 0.50f),
                 rounding,
                 flags);
    else
        PathRect(p_min + AzVec2(0.50f, 0.50f),
                 p_max - AzVec2(0.49f, 0.49f),
                 rounding,
                 flags);
    PathStroke(col, AzDrawFlags_Closed, thickness);
}

void AzDrawList::AddRectFilled(const AzVec2& p_min,
                               const AzVec2& p_max,
                               AzU32 col,
                               float rounding,
                               AzDrawFlags flags)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;
    if (rounding < 0.5f
        || (flags & AzDrawFlags_RoundCornersMask_)
                   == AzDrawFlags_RoundCornersNone)
    {
        PrimReserve(6, 4);
        PrimRect(p_min, p_max, col);
    } else
    {
        PathRect(p_min, p_max, rounding, flags);
        PathFillConvex(col);
    }
}

void AzDrawList::AddRectFilledMultiColor(const AzVec2& p_min,
                                         const AzVec2& p_max,
                                         AzU32 col_upr_left,
                                         AzU32 col_upr_right,
                                         AzU32 col_bot_right,
                                         AzU32 col_bot_left)
{
    if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left)
         & AZ_COL32_A_MASK)
        == 0)
        return;

    const AzVec2 uv = _Data->TexUvWhitePixel;
    PrimReserve(6, 4);
    PrimWriteIdx((AzDrawIdx)(_VtxCurrentIdx));
    PrimWriteIdx((AzDrawIdx)(_VtxCurrentIdx + 1));
    PrimWriteIdx((AzDrawIdx)(_VtxCurrentIdx + 2));
    PrimWriteIdx((AzDrawIdx)(_VtxCurrentIdx));
    PrimWriteIdx((AzDrawIdx)(_VtxCurrentIdx + 2));
    PrimWriteIdx((AzDrawIdx)(_VtxCurrentIdx + 3));
    PrimWriteVtx(p_min, uv, col_upr_left);
    PrimWriteVtx(AzVec2(p_max.x, p_min.y), uv, col_upr_right);
    PrimWriteVtx(p_max, uv, col_bot_right);
    PrimWriteVtx(AzVec2(p_min.x, p_max.y), uv, col_bot_left);
}

void AzDrawList::AddQuad(const AzVec2& p1,
                         const AzVec2& p2,
                         const AzVec2& p3,
                         const AzVec2& p4,
                         AzU32 col,
                         float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathLineTo(p4);
    PathStroke(col, AzDrawFlags_Closed, thickness);
}

void AzDrawList::AddQuadFilled(const AzVec2& p1,
                               const AzVec2& p2,
                               const AzVec2& p3,
                               const AzVec2& p4,
                               AzU32 col)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathLineTo(p4);
    PathFillConvex(col);
}

void AzDrawList::AddTriangle(const AzVec2& p1,
                             const AzVec2& p2,
                             const AzVec2& p3,
                             AzU32 col,
                             float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathStroke(col, AzDrawFlags_Closed, thickness);
}

void AzDrawList::AddTriangleFilled(const AzVec2& p1,
                                   const AzVec2& p2,
                                   const AzVec2& p3,
                                   AzU32 col)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathFillConvex(col);
}

void AzDrawList::AddCircle(const AzVec2& center,
                           float radius,
                           AzU32 col,
                           int num_segments,
                           float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0 || radius < 0.5f)
        return;

    if (num_segments <= 0)
    {
        _PathArcToFastEx(center,
                         radius - 0.5f,
                         0,
                         AZ_DRAWLIST_ARCFAST_SAMPLE_MAX,
                         0);
        _Path.Size--;
    } else
    {
        num_segments = AzClamp(num_segments,
                               3,
                               AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);

        const float a_max = (AZ_PI * 2.0f) * ((float)num_segments - 1.0f)
                            / (float)num_segments;
        PathArcTo(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
    }

    PathStroke(col, AzDrawFlags_Closed, thickness);
}

void AzDrawList::AddCircleFilled(const AzVec2& center,
                                 float radius,
                                 AzU32 col,
                                 int num_segments)
{
    if ((col & AZ_COL32_A_MASK) == 0 || radius < 0.5f)
        return;

    if (num_segments <= 0)
    {
        _PathArcToFastEx(center, radius, 0, AZ_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
        _Path.Size--;
    } else
    {
        num_segments = AzClamp(num_segments,
                               3,
                               AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);

        const float a_max = (AZ_PI * 2.0f) * ((float)num_segments - 1.0f)
                            / (float)num_segments;
        PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    }

    PathFillConvex(col);
}

void AzDrawList::AddNgon(const AzVec2& center,
                         float radius,
                         AzU32 col,
                         int num_segments,
                         float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0 || num_segments <= 2)
        return;

    const float a_max = (AZ_PI * 2.0f) * ((float)num_segments - 1.0f)
                        / (float)num_segments;
    PathArcTo(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
    PathStroke(col, AzDrawFlags_Closed, thickness);
}

void AzDrawList::AddNgonFilled(const AzVec2& center,
                               float radius,
                               AzU32 col,
                               int num_segments)
{
    if ((col & AZ_COL32_A_MASK) == 0 || num_segments <= 2)
        return;

    const float a_max = (AZ_PI * 2.0f) * ((float)num_segments - 1.0f)
                        / (float)num_segments;
    PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    PathFillConvex(col);
}

void AzDrawList::AddEllipse(const AzVec2& center,
                            const AzVec2& radius,
                            AzU32 col,
                            float rot,
                            int num_segments,
                            float thickness)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    if (num_segments <= 0)
        num_segments = _CalcCircleAutoSegmentCount(AzMax(radius.x, radius.y));

    const float a_max = AZ_PI * 2.0f * ((float)num_segments - 1.0f)
                        / (float)num_segments;
    PathEllipticalArcTo(center, radius, rot, 0.0f, a_max, num_segments - 1);
    PathStroke(col, true, thickness);
}

void AzDrawList::AddEllipseFilled(const AzVec2& center,
                                  const AzVec2& radius,
                                  AzU32 col,
                                  float rot,
                                  int num_segments)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    if (num_segments <= 0)
        num_segments = _CalcCircleAutoSegmentCount(AzMax(radius.x, radius.y));

    const float a_max = AZ_PI * 2.0f * ((float)num_segments - 1.0f)
                        / (float)num_segments;
    PathEllipticalArcTo(center, radius, rot, 0.0f, a_max, num_segments - 1);
    PathFillConvex(col);
}

void AzDrawList::AddBezierCubic(const AzVec2& p1,
                                const AzVec2& p2,
                                const AzVec2& p3,
                                const AzVec2& p4,
                                AzU32 col,
                                float thickness,
                                int num_segments)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathBezierCubicCurveTo(p2, p3, p4, num_segments);
    PathStroke(col, 0, thickness);
}

void AzDrawList::AddBezierQuadratic(const AzVec2& p1,
                                    const AzVec2& p2,
                                    const AzVec2& p3,
                                    AzU32 col,
                                    float thickness,
                                    int num_segments)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathBezierQuadraticCurveTo(p2, p3, num_segments);
    PathStroke(col, 0, thickness);
}

void AzDrawList::AddText(AzFont* font,
                         float font_size,
                         const AzVec2& pos,
                         AzU32 col,
                         const char* text_begin,
                         const char* text_end,
                         float wrap_width,
                         const AzVec4* cpu_fine_clip_rect)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    if (text_begin == text_end || text_begin[0] == 0)
        return;

    if (font == NULL)
        font = _Data->Font;
    if (font_size == 0.0f)
        font_size = _Data->FontSize;

    AzVec4 clip_rect = _CmdHeader.ClipRect;
    if (cpu_fine_clip_rect)
    {
        clip_rect.x = AzMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = AzMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = AzMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = AzMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    font->RenderText(this,
                     font_size,
                     pos,
                     col,
                     clip_rect,
                     text_begin,
                     text_end,
                     wrap_width,
                     cpu_fine_clip_rect != NULL);
}

void AzDrawList::AddText(const AzVec2& pos,
                         AzU32 col,
                         const char* text_begin,
                         const char* text_end)
{
    AddText(_Data->Font, _Data->FontSize, pos, col, text_begin, text_end);
}

void AzDrawList::AddAzage(AzTextureRef tex_ref,
                          const AzVec2& p_min,
                          const AzVec2& p_max,
                          const AzVec2& uv_min,
                          const AzVec2& uv_max,
                          AzU32 col)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = tex_ref != _CmdHeader.TexRef;
    if (push_texture_id)
        PushTexture(tex_ref);

    PrimReserve(6, 4);
    PrimRectUV(p_min, p_max, uv_min, uv_max, col);

    if (push_texture_id)
        PopTexture();
}

void AzDrawList::AddAzageQuad(AzTextureRef tex_ref,
                              const AzVec2& p1,
                              const AzVec2& p2,
                              const AzVec2& p3,
                              const AzVec2& p4,
                              const AzVec2& uv1,
                              const AzVec2& uv2,
                              const AzVec2& uv3,
                              const AzVec2& uv4,
                              AzU32 col)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = tex_ref != _CmdHeader.TexRef;
    if (push_texture_id)
        PushTexture(tex_ref);

    PrimReserve(6, 4);
    PrimQuadUV(p1, p2, p3, p4, uv1, uv2, uv3, uv4, col);

    if (push_texture_id)
        PopTexture();
}

void AzDrawList::AddAzageRounded(AzTextureRef tex_ref,
                                 const AzVec2& p_min,
                                 const AzVec2& p_max,
                                 const AzVec2& uv_min,
                                 const AzVec2& uv_max,
                                 AzU32 col,
                                 float rounding,
                                 AzDrawFlags flags)
{
    if ((col & AZ_COL32_A_MASK) == 0)
        return;

    flags = FixRectCornerFlags(flags);
    if (rounding < 0.5f
        || (flags & AzDrawFlags_RoundCornersMask_)
                   == AzDrawFlags_RoundCornersNone)
    {
        AddAzage(tex_ref, p_min, p_max, uv_min, uv_max, col);
        return;
    }

    const bool push_texture_id = tex_ref != _CmdHeader.TexRef;
    if (push_texture_id)
        PushTexture(tex_ref);

    int vert_start_idx = VtxBuffer.Size;
    PathRect(p_min, p_max, rounding, flags);
    PathFillConvex(col);
    int vert_end_idx = VtxBuffer.Size;
    Az::ShadeVertsLinearUV(this,
                           vert_start_idx,
                           vert_end_idx,
                           p_min,
                           p_max,
                           uv_min,
                           uv_max,
                           true);

    if (push_texture_id)
        PopTexture();
}

enum AzTriangulatorNodeType
{
    AzTriangulatorNodeType_Convex,
    AzTriangulatorNodeType_Ear,
    AzTriangulatorNodeType_Reflex
};

struct AzTriangulatorNode
{
    AzTriangulatorNodeType Type;
    int Index;
    AzVec2 Pos;
    AzTriangulatorNode* Next;
    AzTriangulatorNode* Prev;

    void Unlink()
    {
        Next->Prev = Prev;
        Prev->Next = Next;
    }
};

struct AzTriangulatorNodeSpan
{
    AzTriangulatorNode** Data = NULL;
    int Size = 0;

    void push_back(AzTriangulatorNode* node)
    {
        Data[Size++] = node;
    }
    void find_erase_unsorted(int idx)
    {
        for (int i = Size - 1; i >= 0; i--)
            if (Data[i]->Index == idx)
            {
                Data[i] = Data[Size - 1];
                Size--;
                return;
            }
    }
};

struct AzTriangulator
{
    static int EstimateTriangleCount(int points_count)
    {
        return (points_count < 3) ? 0 : points_count - 2;
    }
    static int EstimateScratchBufferSize(int points_count)
    {
        return sizeof(AzTriangulatorNode) * points_count
               + sizeof(AzTriangulatorNode*) * points_count * 2;
    }

    void Init(const AzVec2* points, int points_count, void* scratch_buffer);
    void GetNextTriangle(unsigned int out_triangle[3]);

    void BuildNodes(const AzVec2* points, int points_count);
    void BuildReflexes();
    void BuildEars();
    void FlipNodeList();
    bool IsEar(int i0,
               int i1,
               int i2,
               const AzVec2& v0,
               const AzVec2& v1,
               const AzVec2& v2) const;
    void ReclassifyNode(AzTriangulatorNode* node);

    int _TrianglesLeft = 0;
    AzTriangulatorNode* _Nodes = NULL;
    AzTriangulatorNodeSpan _Ears;
    AzTriangulatorNodeSpan _Reflexes;
};

void AzTriangulator::Init(const AzVec2* points,
                          int points_count,
                          void* scratch_buffer)
{
    AZ_ASSERT(scratch_buffer != NULL && points_count >= 3);
    _TrianglesLeft = EstimateTriangleCount(points_count);
    _Nodes = (AzTriangulatorNode*)scratch_buffer;
    _Ears.Data = (AzTriangulatorNode**)(_Nodes + points_count);
    _Reflexes.Data = (AzTriangulatorNode**)(_Nodes + points_count)
                     + points_count;
    BuildNodes(points, points_count);
    BuildReflexes();
    BuildEars();
}

void AzTriangulator::BuildNodes(const AzVec2* points, int points_count)
{
    for (int i = 0; i < points_count; i++)
    {
        _Nodes[i].Type = AzTriangulatorNodeType_Convex;
        _Nodes[i].Index = i;
        _Nodes[i].Pos = points[i];
        _Nodes[i].Next = _Nodes + i + 1;
        _Nodes[i].Prev = _Nodes + i - 1;
    }
    _Nodes[0].Prev = _Nodes + points_count - 1;
    _Nodes[points_count - 1].Next = _Nodes;
}

void AzTriangulator::BuildReflexes()
{
    AzTriangulatorNode* n1 = _Nodes;
    for (int i = _TrianglesLeft; i >= 0; i--, n1 = n1->Next)
    {
        if (AzTriangleIsClockwise(n1->Prev->Pos, n1->Pos, n1->Next->Pos))
            continue;
        n1->Type = AzTriangulatorNodeType_Reflex;
        _Reflexes.push_back(n1);
    }
}

void AzTriangulator::BuildEars()
{
    AzTriangulatorNode* n1 = _Nodes;
    for (int i = _TrianglesLeft; i >= 0; i--, n1 = n1->Next)
    {
        if (n1->Type != AzTriangulatorNodeType_Convex)
            continue;
        if (!IsEar(n1->Prev->Index,
                   n1->Index,
                   n1->Next->Index,
                   n1->Prev->Pos,
                   n1->Pos,
                   n1->Next->Pos))
            continue;
        n1->Type = AzTriangulatorNodeType_Ear;
        _Ears.push_back(n1);
    }
}

void AzTriangulator::GetNextTriangle(unsigned int out_triangle[3])
{
    if (_Ears.Size == 0)
    {
        FlipNodeList();

        AzTriangulatorNode* node = _Nodes;
        for (int i = _TrianglesLeft; i >= 0; i--, node = node->Next)
            node->Type = AzTriangulatorNodeType_Convex;
        _Reflexes.Size = 0;
        BuildReflexes();
        BuildEars();

        if (_Ears.Size == 0)
        {
            AZ_ASSERT(_TrianglesLeft > 0);
            _Ears.Data[0] = _Nodes;
            _Ears.Size = 1;
        }
    }

    AzTriangulatorNode* ear = _Ears.Data[--_Ears.Size];
    out_triangle[0] = ear->Prev->Index;
    out_triangle[1] = ear->Index;
    out_triangle[2] = ear->Next->Index;

    ear->Unlink();
    if (ear == _Nodes)
        _Nodes = ear->Next;

    ReclassifyNode(ear->Prev);
    ReclassifyNode(ear->Next);
    _TrianglesLeft--;
}

void AzTriangulator::FlipNodeList()
{
    AzTriangulatorNode* prev = _Nodes;
    AzTriangulatorNode* temp = _Nodes;
    AzTriangulatorNode* current = _Nodes->Next;
    prev->Next = prev;
    prev->Prev = prev;
    while (current != _Nodes)
    {
        temp = current->Next;

        current->Next = prev;
        prev->Prev = current;
        _Nodes->Next = current;
        current->Prev = _Nodes;

        prev = current;
        current = temp;
    }
    _Nodes = prev;
}

bool AzTriangulator::IsEar(int i0,
                           int i1,
                           int i2,
                           const AzVec2& v0,
                           const AzVec2& v1,
                           const AzVec2& v2) const
{
    AzTriangulatorNode** p_end = _Reflexes.Data + _Reflexes.Size;
    for (AzTriangulatorNode** p = _Reflexes.Data; p < p_end; p++)
    {
        AzTriangulatorNode* reflex = *p;
        if (reflex->Index != i0 && reflex->Index != i1 && reflex->Index != i2)
            if (AzTriangleContainsPoint(v0, v1, v2, reflex->Pos))
                return false;
    }
    return true;
}

void AzTriangulator::ReclassifyNode(AzTriangulatorNode* n1)
{
    AzTriangulatorNodeType type;
    const AzTriangulatorNode* n0 = n1->Prev;
    const AzTriangulatorNode* n2 = n1->Next;
    if (!AzTriangleIsClockwise(n0->Pos, n1->Pos, n2->Pos))
        type = AzTriangulatorNodeType_Reflex;
    else if (IsEar(n0->Index, n1->Index, n2->Index, n0->Pos, n1->Pos, n2->Pos))
        type = AzTriangulatorNodeType_Ear;
    else
        type = AzTriangulatorNodeType_Convex;

    if (type == n1->Type)
        return;
    if (n1->Type == AzTriangulatorNodeType_Reflex)
        _Reflexes.find_erase_unsorted(n1->Index);
    else if (n1->Type == AzTriangulatorNodeType_Ear)
        _Ears.find_erase_unsorted(n1->Index);
    if (type == AzTriangulatorNodeType_Reflex)
        _Reflexes.push_back(n1);
    else if (type == AzTriangulatorNodeType_Ear)
        _Ears.push_back(n1);
    n1->Type = type;
}

void AzDrawList::AddConcavePolyFilled(const AzVec2* points,
                                      const int points_count,
                                      AzU32 col)
{
    if (points_count < 3 || (col & AZ_COL32_A_MASK) == 0)
        return;

    const AzVec2 uv = _Data->TexUvWhitePixel;
    AzTriangulator triangulator;
    unsigned int triangle[3];
    if (Flags & AzDrawListFlags_AntiAliasedFill)
    {
        const float AA_SIZE = _FringeScale;
        const AzU32 col_trans = col & ~AZ_COL32_A_MASK;
        const int idx_count = (points_count - 2) * 3 + points_count * 6;
        const int vtx_count = (points_count * 2);
        PrimReserve(idx_count, vtx_count);

        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;

        _Data->TempBuffer.reserve_discard(
                (AzTriangulator::EstimateScratchBufferSize(points_count)
                 + sizeof(AzVec2))
                / sizeof(AzVec2));
        triangulator.Init(points, points_count, _Data->TempBuffer.Data);
        while (triangulator._TrianglesLeft > 0)
        {
            triangulator.GetNextTriangle(triangle);
            _IdxWritePtr[0] = (AzDrawIdx)(vtx_inner_idx + (triangle[0] << 1));
            _IdxWritePtr[1] = (AzDrawIdx)(vtx_inner_idx + (triangle[1] << 1));
            _IdxWritePtr[2] = (AzDrawIdx)(vtx_inner_idx + (triangle[2] << 1));
            _IdxWritePtr += 3;
        }

        _Data->TempBuffer.reserve_discard(points_count);
        AzVec2* temp_normals = _Data->TempBuffer.Data;
        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const AzVec2& p0 = points[i0];
            const AzVec2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            AZ_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const AzVec2& n0 = temp_normals[i0];
            const AzVec2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            AZ_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x);
            _VtxWritePtr[0].pos.y = (points[i1].y - dm_y);
            _VtxWritePtr[0].uv = uv;
            _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x);
            _VtxWritePtr[1].pos.y = (points[i1].y + dm_y);
            _VtxWritePtr[1].uv = uv;
            _VtxWritePtr[1].col = col_trans;
            _VtxWritePtr += 2;

            _IdxWritePtr[0] = (AzDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr[1] = (AzDrawIdx)(vtx_inner_idx + (i0 << 1));
            _IdxWritePtr[2] = (AzDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[3] = (AzDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[4] = (AzDrawIdx)(vtx_outer_idx + (i1 << 1));
            _IdxWritePtr[5] = (AzDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (AzDrawIdx)vtx_count;
    } else
    {
        const int idx_count = (points_count - 2) * 3;
        const int vtx_count = points_count;
        PrimReserve(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _VtxWritePtr[0].pos = points[i];
            _VtxWritePtr[0].uv = uv;
            _VtxWritePtr[0].col = col;
            _VtxWritePtr++;
        }
        _Data->TempBuffer.reserve_discard(
                (AzTriangulator::EstimateScratchBufferSize(points_count)
                 + sizeof(AzVec2))
                / sizeof(AzVec2));
        triangulator.Init(points, points_count, _Data->TempBuffer.Data);
        while (triangulator._TrianglesLeft > 0)
        {
            triangulator.GetNextTriangle(triangle);
            _IdxWritePtr[0] = (AzDrawIdx)(_VtxCurrentIdx + triangle[0]);
            _IdxWritePtr[1] = (AzDrawIdx)(_VtxCurrentIdx + triangle[1]);
            _IdxWritePtr[2] = (AzDrawIdx)(_VtxCurrentIdx + triangle[2]);
            _IdxWritePtr += 3;
        }
        _VtxCurrentIdx += (AzDrawIdx)vtx_count;
    }
}

void AzDrawListSplitter::ClearFreeMemory()
{
    for (int i = 0; i < _Channels.Size; i++)
    {
        if (i == _Current)
            memset(&_Channels[i], 0, sizeof(_Channels[i]));
        _Channels[i]._CmdBuffer.clear();
        _Channels[i]._IdxBuffer.clear();
    }
    _Current = 0;
    _Count = 1;
    _Channels.clear();
}

void AzDrawListSplitter::Split(AzDrawList* draw_list, int channels_count)
{
    AZ_UNUSED(draw_list);
    AZ_ASSERT(_Current == 0 && _Count <= 1 && "Nested channel splitting is not supported. Please use separate instances of AzDrawListSplitter.");
    int old_channels_count = _Channels.Size;
    if (old_channels_count < channels_count)
    {
        _Channels.reserve(channels_count);
        _Channels.resize(channels_count);
    }
    _Count = channels_count;

    memset(&_Channels[0], 0, sizeof(AzDrawChannel));
    for (int i = 1; i < channels_count; i++)
    {
        if (i >= old_channels_count)
        {
            AZ_PLACEMENT_NEW(&_Channels[i]) AzDrawChannel();
        } else
        {
            _Channels[i]._CmdBuffer.resize(0);
            _Channels[i]._IdxBuffer.resize(0);
        }
    }
}

void AzDrawListSplitter::Merge(AzDrawList* draw_list)
{
    if (_Count <= 1)
        return;

    SetCurrentChannel(draw_list, 0);
    draw_list->_PopUnusedDrawCmd();

    int new_cmd_buffer_count = 0;
    int new_idx_buffer_count = 0;
    AzDrawCmd* last_cmd = (_Count > 0 && draw_list->CmdBuffer.Size > 0)
                                  ? &draw_list->CmdBuffer.back()
                                  : NULL;
    int idx_offset = last_cmd ? last_cmd->IdxOffset + last_cmd->ElemCount : 0;
    for (int i = 1; i < _Count; i++)
    {
        AzDrawChannel& ch = _Channels[i];
        if (ch._CmdBuffer.Size > 0 && ch._CmdBuffer.back().ElemCount == 0
            && ch._CmdBuffer.back().UserCallback == NULL)
            ch._CmdBuffer.pop_back();

        if (ch._CmdBuffer.Size > 0 && last_cmd != NULL)
        {
            AzDrawCmd* next_cmd = &ch._CmdBuffer[0];
            if (AzDrawCmd_HeaderCompare(last_cmd, next_cmd) == 0
                && last_cmd->UserCallback == NULL
                && next_cmd->UserCallback == NULL)
            {
                last_cmd->ElemCount += next_cmd->ElemCount;
                idx_offset += next_cmd->ElemCount;
                ch._CmdBuffer.erase(ch._CmdBuffer.Data);
            }
        }
        if (ch._CmdBuffer.Size > 0)
            last_cmd = &ch._CmdBuffer.back();
        new_cmd_buffer_count += ch._CmdBuffer.Size;
        new_idx_buffer_count += ch._IdxBuffer.Size;
        for (int cmd_n = 0; cmd_n < ch._CmdBuffer.Size; cmd_n++)
        {
            ch._CmdBuffer.Data[cmd_n].IdxOffset = idx_offset;
            idx_offset += ch._CmdBuffer.Data[cmd_n].ElemCount;
        }
    }
    draw_list->CmdBuffer.resize(draw_list->CmdBuffer.Size
                                + new_cmd_buffer_count);
    draw_list->IdxBuffer.resize(draw_list->IdxBuffer.Size
                                + new_idx_buffer_count);

    AzDrawCmd* cmd_write = draw_list->CmdBuffer.Data + draw_list->CmdBuffer.Size
                           - new_cmd_buffer_count;
    AzDrawIdx* idx_write = draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size
                           - new_idx_buffer_count;
    for (int i = 1; i < _Count; i++)
    {
        AzDrawChannel& ch = _Channels[i];
        if (int sz = ch._CmdBuffer.Size)
        {
            memcpy(cmd_write, ch._CmdBuffer.Data, sz * sizeof(AzDrawCmd));
            cmd_write += sz;
        }
        if (int sz = ch._IdxBuffer.Size)
        {
            memcpy(idx_write, ch._IdxBuffer.Data, sz * sizeof(AzDrawIdx));
            idx_write += sz;
        }
    }
    draw_list->_IdxWritePtr = idx_write;

    if (draw_list->CmdBuffer.Size == 0
        || draw_list->CmdBuffer.back().UserCallback != NULL)
        draw_list->AddDrawCmd();

    AzDrawCmd* curr_cmd =
            &draw_list->CmdBuffer.Data[draw_list->CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount == 0)
        AzDrawCmd_HeaderCopy(curr_cmd, &draw_list->_CmdHeader);
    else if (AzDrawCmd_HeaderCompare(curr_cmd, &draw_list->_CmdHeader) != 0)
        draw_list->AddDrawCmd();

    _Count = 1;
}

void AzDrawListSplitter::SetCurrentChannel(AzDrawList* draw_list, int idx)
{
    AZ_ASSERT(idx >= 0 && idx < _Count);
    if (_Current == idx)
        return;

    memcpy(&_Channels.Data[_Current]._CmdBuffer,
           &draw_list->CmdBuffer,
           sizeof(draw_list->CmdBuffer));
    memcpy(&_Channels.Data[_Current]._IdxBuffer,
           &draw_list->IdxBuffer,
           sizeof(draw_list->IdxBuffer));
    _Current = idx;
    memcpy(&draw_list->CmdBuffer,
           &_Channels.Data[idx]._CmdBuffer,
           sizeof(draw_list->CmdBuffer));
    memcpy(&draw_list->IdxBuffer,
           &_Channels.Data[idx]._IdxBuffer,
           sizeof(draw_list->IdxBuffer));
    draw_list->_IdxWritePtr = draw_list->IdxBuffer.Data
                              + draw_list->IdxBuffer.Size;

    AzDrawCmd* curr_cmd =
            (draw_list->CmdBuffer.Size == 0)
                    ? NULL
                    : &draw_list->CmdBuffer.Data[draw_list->CmdBuffer.Size - 1];
    if (curr_cmd == NULL)
        draw_list->AddDrawCmd();
    else if (curr_cmd->ElemCount == 0)
        AzDrawCmd_HeaderCopy(curr_cmd, &draw_list->_CmdHeader);
    else if (AzDrawCmd_HeaderCompare(curr_cmd, &draw_list->_CmdHeader) != 0)
        draw_list->AddDrawCmd();
}

void AzDrawData::Clear()
{
    Valid = false;
    CmdListsCount = TotalIdxCount = TotalVtxCount = 0;
    CmdLists.resize(0);
    DisplayPos = DisplaySize = FramebufferScale = AzVec2(0.0f, 0.0f);
    OwnerViewport = NULL;
    Textures = NULL;
}

void Az::AddDrawListToDrawDataEx(AzDrawData* draw_data,
                                 AzVector<AzDrawList*>* out_list,
                                 AzDrawList* draw_list)
{
    if (draw_list->CmdBuffer.Size == 0)
        return;
    if (draw_list->CmdBuffer.Size == 1 && draw_list->CmdBuffer[0].ElemCount == 0
        && draw_list->CmdBuffer[0].UserCallback == NULL)
        return;

    AZ_ASSERT(draw_list->VtxBuffer.Size == 0
              || draw_list->_VtxWritePtr
                         == draw_list->VtxBuffer.Data
                                    + draw_list->VtxBuffer.Size);
    AZ_ASSERT(draw_list->IdxBuffer.Size == 0
              || draw_list->_IdxWritePtr
                         == draw_list->IdxBuffer.Data
                                    + draw_list->IdxBuffer.Size);
    if (!(draw_list->Flags & AzDrawListFlags_AllowVtxOffset))
        AZ_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

    if (sizeof(AzDrawIdx) == 2)
        AZ_ASSERT(draw_list->_VtxCurrentIdx < (1 << 16) && "Too many vertices in AzDrawList using 16-bit indices. Read comment above");

    if (draw_list->_CallbacksDataBuf.Size > 0)
        for (AzDrawCmd& cmd : draw_list->CmdBuffer)
            if (cmd.UserCallback != NULL && cmd.UserCallbackDataOffset != -1
                && cmd.UserCallbackDataSize > 0)
                cmd.UserCallbackData = draw_list->_CallbacksDataBuf.Data
                                       + cmd.UserCallbackDataOffset;

    out_list->push_back(draw_list);
    draw_data->CmdListsCount++;
    draw_data->TotalVtxCount += draw_list->VtxBuffer.Size;
    draw_data->TotalIdxCount += draw_list->IdxBuffer.Size;
}

void AzDrawData::AddDrawList(AzDrawList* draw_list)
{
    AZ_ASSERT(CmdLists.Size == CmdListsCount);
    draw_list->_PopUnusedDrawCmd();
    Az::AddDrawListToDrawDataEx(this, &CmdLists, draw_list);
}

void AzDrawData::DeIndexAllBuffers()
{
    AzVector<AzDrawVert> new_vtx_buffer;
    TotalVtxCount = TotalIdxCount = 0;
    for (int i = 0; i < CmdListsCount; i++)
    {
        AzDrawList* cmd_list = CmdLists[i];
        if (cmd_list->IdxBuffer.empty())
            continue;
        new_vtx_buffer.resize(cmd_list->IdxBuffer.Size);
        for (int j = 0; j < cmd_list->IdxBuffer.Size; j++)
            new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
        cmd_list->VtxBuffer.swap(new_vtx_buffer);
        cmd_list->IdxBuffer.resize(0);
        TotalVtxCount += cmd_list->VtxBuffer.Size;
    }
}

void AzDrawData::ScaleClipRects(const AzVec2& fb_scale)
{
    for (AzDrawList* draw_list : CmdLists)
        for (AzDrawCmd& cmd : draw_list->CmdBuffer)
            cmd.ClipRect = AzVec4(cmd.ClipRect.x * fb_scale.x,
                                  cmd.ClipRect.y * fb_scale.y,
                                  cmd.ClipRect.z * fb_scale.x,
                                  cmd.ClipRect.w * fb_scale.y);
}

void Az::ShadeVertsLinearColorGradientKeepAlpha(AzDrawList* draw_list,
                                                int vert_start_idx,
                                                int vert_end_idx,
                                                AzVec2 gradient_p0,
                                                AzVec2 gradient_p1,
                                                AzU32 col0,
                                                AzU32 col1)
{
    AzVec2 gradient_extent = gradient_p1 - gradient_p0;
    float gradient_inv_length2 = 1.0f / AzLengthSqr(gradient_extent);
    AzDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    AzDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    const int col0_r = (int)(col0 >> AZ_COL32_R_SHIFT) & 0xFF;
    const int col0_g = (int)(col0 >> AZ_COL32_G_SHIFT) & 0xFF;
    const int col0_b = (int)(col0 >> AZ_COL32_B_SHIFT) & 0xFF;
    const int col_delta_r = ((int)(col1 >> AZ_COL32_R_SHIFT) & 0xFF) - col0_r;
    const int col_delta_g = ((int)(col1 >> AZ_COL32_G_SHIFT) & 0xFF) - col0_g;
    const int col_delta_b = ((int)(col1 >> AZ_COL32_B_SHIFT) & 0xFF) - col0_b;
    for (AzDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = AzDot(vert->pos - gradient_p0, gradient_extent);
        float t = AzClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = (int)(col0_r + col_delta_r * t);
        int g = (int)(col0_g + col_delta_g * t);
        int b = (int)(col0_b + col_delta_b * t);
        vert->col = (r << AZ_COL32_R_SHIFT) | (g << AZ_COL32_G_SHIFT)
                    | (b << AZ_COL32_B_SHIFT) | (vert->col & AZ_COL32_A_MASK);
    }
}

void Az::ShadeVertsLinearUV(AzDrawList* draw_list,
                            int vert_start_idx,
                            int vert_end_idx,
                            const AzVec2& a,
                            const AzVec2& b,
                            const AzVec2& uv_a,
                            const AzVec2& uv_b,
                            bool clamp)
{
    const AzVec2 size = b - a;
    const AzVec2 uv_size = uv_b - uv_a;
    const AzVec2 scale = AzVec2(size.x != 0.0f ? (uv_size.x / size.x) : 0.0f,
                                size.y != 0.0f ? (uv_size.y / size.y) : 0.0f);

    AzDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    AzDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    if (clamp)
    {
        const AzVec2 min = AzMin(uv_a, uv_b);
        const AzVec2 max = AzMax(uv_a, uv_b);
        for (AzDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = AzClamp(
                    uv_a
                            + AzMul(AzVec2(vertex->pos.x, vertex->pos.y) - a,
                                    scale),
                    min,
                    max);
    } else
    {
        for (AzDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = uv_a
                         + AzMul(AzVec2(vertex->pos.x, vertex->pos.y) - a,
                                 scale);
    }
}

void Az::ShadeVertsTransformPos(AzDrawList* draw_list,
                                int vert_start_idx,
                                int vert_end_idx,
                                const AzVec2& pivot_in,
                                float cos_a,
                                float sin_a,
                                const AzVec2& pivot_out)
{
    AzDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    AzDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    for (AzDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
        vertex->pos = AzRotate(vertex->pos - pivot_in, cos_a, sin_a)
                      + pivot_out;
}

AzFontConfig::AzFontConfig()
{
    memset(this, 0, sizeof(*this));
    FontDataOwnedByAtlas = true;
    OversampleH = 0;
    OversampleV = 0;
    GlyphMaxAdvanceX = FLT_MAX;
    RasterizerMultiply = 1.0f;
    RasterizerDensity = 1.0f;
    EllipsisChar = 0;
}

int AzTextureDataGetFormatBytesPerPixel(AzTextureFormat format)
{
    switch (format)
    {
    case AzTextureFormat_Alpha8:
        return 1;
    case AzTextureFormat_RGBA32:
        return 4;
    }
    AZ_ASSERT(0);
    return 0;
}

const char* AzTextureDataGetStatusName(AzTextureStatus status)
{
    switch (status)
    {
    case AzTextureStatus_OK:
        return "OK";
    case AzTextureStatus_Destroyed:
        return "Destroyed";
    case AzTextureStatus_WantCreate:
        return "WantCreate";
    case AzTextureStatus_WantUpdates:
        return "WantUpdates";
    case AzTextureStatus_WantDestroy:
        return "WantDestroy";
    }
    return "N/A";
}

const char* AzTextureDataGetFormatName(AzTextureFormat format)
{
    switch (format)
    {
    case AzTextureFormat_Alpha8:
        return "Alpha8";
    case AzTextureFormat_RGBA32:
        return "RGBA32";
    }
    return "N/A";
}

void AzTextureData::Create(AzTextureFormat format, int w, int h)
{
    DestroyPixels();
    Format = format;
    Width = w;
    Height = h;
    BytesPerPixel = AzTextureDataGetFormatBytesPerPixel(format);
    UseColors = false;
    Pixels = (unsigned char*)AZ_ALLOC(Width * Height * BytesPerPixel);
    AZ_ASSERT(Pixels != NULL);
    memset(Pixels, 0, Width * Height * BytesPerPixel);
    UsedRect.x = UsedRect.y = UsedRect.w = UsedRect.h = 0;
    UpdateRect.x = UpdateRect.y = (unsigned short)~0;
    UpdateRect.w = UpdateRect.h = 0;
}

void AzTextureData::DestroyPixels()
{
    if (Pixels)
        AZ_FREE(Pixels);
    Pixels = NULL;
    UseColors = false;
}

const int FONT_ATLAS_DEFAULT_TEX_DATA_W = 122;
const int FONT_ATLAS_DEFAULT_TEX_DATA_H = 27;
static const char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS
        [FONT_ATLAS_DEFAULT_TEX_DATA_W * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] = {
                "..-         -XXXXXXX-    X    -           X           "
                "-XXXXXXX          -          XXXXXXX-     XX          - XX    "
                "   XX "
                "..-         -X.....X-   X.X   -          X.X          "
                "-X.....X          -          X.....X-    X..X         -X..X   "
                "  X..X"
                "---         -XXX.XXX-  X...X  -         X...X         -X....X "
                "          -           X....X-    X..X         -X...X   X...X"
                "X           -  X.X  - X.....X -        X.....X        -X...X  "
                "          -            X...X-    X..X         - X...X X...X "
                "XX          -  X.X  -X.......X-       X.......X       -X..X.X "
                "          -           X.X..X-    X..X         -  X...X...X  "
                "X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X "
                "X.X          -          X.X X.X-    X..XXX       -   X.....X  "
                " "
                "X..X        -  X.X  -   X.X   -          X.X          -XX   "
                "X.X         -         X.X   XX-    X..X..XXX    -    X...X    "
                "X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      "
                "X.X        -        X.X      -    X..X..X..XX  -     X.X     "
                "X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       "
                "X.X       -       X.X       -    X..X..X..X.X -    X...X    "
                "X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -       "
                " X.X      -      X.X        -XXX X..X..X..X..X-   X.....X   "
                "X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -       "
                "  X.X   XX-XX   X.X         -X..XX........X..X-  X...X...X  "
                "X.......X   -  X.X  -   X.X   -X.....................X-       "
                "   X.X X.X-X.X X.X          -X...X...........X- X...X X...X "
                "X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -       "
                "    X.X..X-X..X.X           - X..............X-X...X   X...X"
                "X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -       "
                "     X...X-X...X            -  X.............X-X..X     X..X"
                "X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -       "
                "    X....X-X....X           -  X.............X- XX       XX "
                "X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -       "
                "   X.....X-X.....X          -   X............X--------------"
                "X...X..X    ---------   X.X   -          X.X          -       "
                "   XXXXXXX-XXXXXXX          -   X...........X -             "
                "X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       "
                "-------------------------------------    X..........X -       "
                "      "
                "X.X  X..X   -       -X.......X-       X.......X       -    XX "
                "          XX    -           -    X..........X -             "
                "XX    X..X  -       - X.....X -        X.....X        -   X.X "
                "          X.X   -           -     X........X  -             "
                "      X..X  -       -  X...X  -         X...X         -  X..X "
                "          X..X  -           -     X........X  -             "
                "       XX   -       -   X.X   -          X.X          - "
                "X...XXXXXXXXXXXXX...X -           -     XXXXXXXXXX  -         "
                "    "
                "-------------       -    X    -           X           "
                "-X.....................X-           -------------------       "
                "      "
                "                    ----------------------------------- "
                "X...XXXXXXXXXXXXX...X -                                       "
                "    "
                "                                                      -  X..X "
                "          X..X  -                                           "
                "                                                      -   X.X "
                "          X.X   -                                           "
                "                                                      -    XX "
                "          XX    -                                           "};

static const AzVec2 FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[AzMouseCursor_COUNT][3] =
        {

                {AzVec2(0, 3), AzVec2(12, 19), AzVec2(0, 0)},
                {AzVec2(13, 0), AzVec2(7, 16), AzVec2(1, 8)},
                {AzVec2(31, 0), AzVec2(23, 23), AzVec2(11, 11)},
                {AzVec2(21, 0), AzVec2(9, 23), AzVec2(4, 11)},
                {AzVec2(55, 18), AzVec2(23, 9), AzVec2(11, 4)},
                {AzVec2(73, 0), AzVec2(17, 17), AzVec2(8, 8)},
                {AzVec2(55, 0), AzVec2(17, 17), AzVec2(8, 8)},
                {AzVec2(91, 0), AzVec2(17, 22), AzVec2(5, 0)},
                {AzVec2(0, 3), AzVec2(12, 19), AzVec2(0, 0)},
                {AzVec2(0, 3), AzVec2(12, 19), AzVec2(0, 0)},
                {AzVec2(109, 0), AzVec2(13, 15), AzVec2(6, 7)},
};

#define AZ_FONTGLYPH_INDEX_UNUSED ((AzU16) - 1)
#define AZ_FONTGLYPH_INDEX_NOT_FOUND ((AzU16) - 2)

AzFontAtlas::AzFontAtlas()
{
    memset(this, 0, sizeof(*this));
    TexDesiredFormat = AzTextureFormat_RGBA32;
    TexGlyphPadding = 1;
    TexMinWidth = 512;
    TexMinHeight = 128;
    TexMaxWidth = 8192;
    TexMaxHeight = 8192;
    RendererHasTextures = false;
    TexNextUniqueID = 1;
    FontNextUniqueID = 1;
    Builder = NULL;
}

AzFontAtlas::~AzFontAtlas()
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    RendererHasTextures = false;
    ClearFonts();
    ClearTexData();
    TexList.clear_delete();
    TexData = NULL;
}

void AzFontAtlas::Clear()
{
    bool backup_renderer_has_textures = RendererHasTextures;
    RendererHasTextures = false;
    ClearFonts();
    ClearTexData();
    RendererHasTextures = backup_renderer_has_textures;
}

void AzFontAtlas::CompactCache()
{
    AzFontAtlasTextureCompact(this);
}

void AzFontAtlas::ClearInputData()
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");

    for (AzFont* font : Fonts)
        AzFontAtlasFontDestroyOutput(this, font);
    for (AzFontConfig& font_cfg : Sources)
        AzFontAtlasFontDestroySourceData(this, &font_cfg);
    for (AzFont* font : Fonts)
    {
        font->Sources.clear();
        font->Flags |= AzFontFlags_NoLoadGlyphs;
    }
    Sources.clear();
}

void AzFontAtlas::ClearTexData()
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    AZ_ASSERT(
            RendererHasTextures == false
            && "Not supported for dynamic atlases, but you may call Clear().");
    for (AzTextureData* tex : TexList)
        tex->DestroyPixels();
}

void AzFontAtlas::ClearFonts()
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    AzFontAtlasBuildDestroy(this);
    ClearInputData();
    Fonts.clear_delete();
    TexIsBuilt = false;
    for (AzDrawListSharedData* shared_data : DrawListSharedDatas)
    {
        shared_data->Font = NULL;
        shared_data->FontScale = shared_data->FontSize = 0.0f;
    }
}

static void AzFontAtlasBuildUpdateRendererHasTexturesFromContext(
        AzFontAtlas* atlas)
{
    for (AzDrawListSharedData* shared_data : atlas->DrawListSharedDatas)
        if (AzContext* imgui_ctx = shared_data->Context)
        {
            atlas->RendererHasTextures = (imgui_ctx->IO.BackendFlags
                                          & AzBackendFlags_RendererHasTextures)
                                         != 0;
            break;
        }
}

void AzFontAtlasUpdateNewFrame(AzFontAtlas* atlas, int frame_count)
{
    AZ_ASSERT(atlas->Builder == NULL
              || atlas->Builder->FrameCount < frame_count);

    if (atlas->RendererHasTextures)
    {
        atlas->TexIsBuilt = true;
        if (atlas->Builder == NULL)
            AzFontAtlasBuildMain(atlas);
    } else
    {
        AZ_ASSERT_USER_ERROR(
                atlas->TexIsBuilt,
                "Backend does not support AzBackendFlags_RendererHasTextures, "
                "and font atlas is not built! Update backend OR make sure you "
                "called Az_AzplXXXX_NewFrame() function for renderer backend, "
                "which should call io.Fonts->GetTexDataAsRGBA32() / "
                "GetTexDataAsAlpha8().");
    }
    if (atlas->TexIsBuilt && atlas->Builder->PreloadedAllGlyphsRanges)
        AZ_ASSERT_USER_ERROR(atlas->RendererHasTextures == false,
                             "Called AzFontAtlas::Build() before "
                             "AzBackendFlags_RendererHasTextures got set! With "
                             "new backends: you don't need to call Build().");

    AzFontAtlasBuilder* builder = atlas->Builder;
    builder->FrameCount = frame_count;
    for (AzFont* font : atlas->Fonts)
        font->LastBaked = NULL;

    if (builder->BakedDiscardedCount > 0)
    {
        int dst_n = 0, src_n = 0;
        for (; src_n < builder->BakedPool.Size; src_n++)
        {
            AzFontBaked* p_src = &builder->BakedPool[src_n];
            if (p_src->WantDestroy)
                continue;
            AzFontBaked* p_dst = &builder->BakedPool[dst_n++];
            if (p_dst == p_src)
                continue;
            memcpy(p_dst, p_src, sizeof(AzFontBaked));
            builder->BakedMap.SetVoidPtr(p_dst->BakedId, p_dst);
        }
        AZ_ASSERT(dst_n + builder->BakedDiscardedCount == src_n);
        builder->BakedPool.Size -= builder->BakedDiscardedCount;
        builder->BakedDiscardedCount = 0;
    }

    for (int tex_n = 0; tex_n < atlas->TexList.Size; tex_n++)
    {
        AzTextureData* tex = atlas->TexList[tex_n];
        bool remove_from_list = false;
        if (tex->Status == AzTextureStatus_OK)
        {
            tex->Updates.resize(0);
            tex->UpdateRect.x = tex->UpdateRect.y = (unsigned short)~0;
            tex->UpdateRect.w = tex->UpdateRect.h = 0;
        }
        if (tex->Status == AzTextureStatus_WantCreate
            && atlas->RendererHasTextures)
            AZ_ASSERT(tex->TexID == AzTextureID_Invalid && tex->BackendUserData == NULL && "Backend set texture's TexID/BackendUserData but did not update Status to OK.");

        if (tex->Status == AzTextureStatus_Destroyed)
        {
            AZ_ASSERT(tex->TexID == AzTextureID_Invalid && tex->BackendUserData == NULL && "Backend set texture Status to Destroyed but did not clear TexID/BackendUserData!");
            if (tex->WantDestroyNextFrame)
                remove_from_list = true;
            else
                tex->Status = AzTextureStatus_WantCreate;
        } else if (tex->WantDestroyNextFrame
                   && tex->Status != AzTextureStatus_WantDestroy)
        {
            AZ_ASSERT(tex->Status == AzTextureStatus_OK
                      || tex->Status == AzTextureStatus_WantCreate
                      || tex->Status == AzTextureStatus_WantUpdates);
            tex->Status = AzTextureStatus_WantDestroy;
        }

        if (tex->Status == AzTextureStatus_WantDestroy)
            tex->UnusedFrames++;

        if (tex->Status == AzTextureStatus_WantDestroy
            && tex->TexID == AzTextureID_Invalid
            && tex->BackendUserData == NULL)
            remove_from_list = true;

        if (remove_from_list)
        {
            tex->DestroyPixels();
            AZ_DELETE(tex);
            atlas->TexList.erase(atlas->TexList.begin() + tex_n);
            tex_n--;
        }
    }
}

void AzFontAtlasTextureBlockConvert(const unsigned char* src_pixels,
                                    AzTextureFormat src_fmt,
                                    int src_pitch,
                                    unsigned char* dst_pixels,
                                    AzTextureFormat dst_fmt,
                                    int dst_pitch,
                                    int w,
                                    int h)
{
    AZ_ASSERT(src_pixels != NULL && dst_pixels != NULL);
    if (src_fmt == dst_fmt)
    {
        int line_sz = w * AzTextureDataGetFormatBytesPerPixel(src_fmt);
        for (int ny = h; ny > 0;
             ny--, src_pixels += src_pitch, dst_pixels += dst_pitch)
            memcpy(dst_pixels, src_pixels, line_sz);
    } else if (src_fmt == AzTextureFormat_Alpha8
               && dst_fmt == AzTextureFormat_RGBA32)
    {
        for (int ny = h; ny > 0;
             ny--, src_pixels += src_pitch, dst_pixels += dst_pitch)
        {
            const AzU8* src_p = (const AzU8*)src_pixels;
            AzU32* dst_p = (AzU32*)(void*)dst_pixels;
            for (int nx = w; nx > 0; nx--)
                *dst_p++ = AZ_COL32(255, 255, 255, (unsigned int)(*src_p++));
        }
    } else if (src_fmt == AzTextureFormat_RGBA32
               && dst_fmt == AzTextureFormat_Alpha8)
    {
        for (int ny = h; ny > 0;
             ny--, src_pixels += src_pitch, dst_pixels += dst_pitch)
        {
            const AzU32* src_p = (const AzU32*)(void*)src_pixels;
            AzU8* dst_p = (AzU8*)dst_pixels;
            for (int nx = w; nx > 0; nx--)
                *dst_p++ = ((*src_p++) >> AZ_COL32_A_SHIFT) & 0xFF;
        }
    } else
    {
        AZ_ASSERT(0);
    }
}

void AzFontAtlasTextureBlockPostProcess(AzFontAtlasPostProcessData* data)
{
    if (data->FontSrc->RasterizerMultiply != 1.0f)
        AzFontAtlasTextureBlockPostProcessMultiply(
                data,
                data->FontSrc->RasterizerMultiply);
}

void AzFontAtlasTextureBlockPostProcessMultiply(
        AzFontAtlasPostProcessData* data,
        float multiply_factor)
{
    unsigned char* pixels = data->Pixels;
    int pitch = data->Pitch;
    if (data->Format == AzTextureFormat_Alpha8)
    {
        for (int ny = data->Height; ny > 0; ny--, pixels += pitch)
        {
            AzU8* p = (AzU8*)pixels;
            for (int nx = data->Width; nx > 0; nx--, p++)
            {
                unsigned int v = AzMin((unsigned int)(*p * multiply_factor),
                                       (unsigned int)255);
                *p = (unsigned char)v;
            }
        }
    } else if (data->Format == AzTextureFormat_RGBA32)
    {
        for (int ny = data->Height; ny > 0; ny--, pixels += pitch)
        {
            AzU32* p = (AzU32*)(void*)pixels;
            for (int nx = data->Width; nx > 0; nx--, p++)
            {
                unsigned int a = AzMin((unsigned int)(((*p >> AZ_COL32_A_SHIFT)
                                                       & 0xFF)
                                                      * multiply_factor),
                                       (unsigned int)255);
                *p = AZ_COL32((*p >> AZ_COL32_R_SHIFT) & 0xFF,
                              (*p >> AZ_COL32_G_SHIFT) & 0xFF,
                              (*p >> AZ_COL32_B_SHIFT) & 0xFF,
                              a);
            }
        }
    } else
    {
        AZ_ASSERT(0);
    }
}

void AzFontAtlasTextureBlockFill(AzTextureData* dst_tex,
                                 int dst_x,
                                 int dst_y,
                                 int w,
                                 int h,
                                 AzU32 col)
{
    if (dst_tex->Format == AzTextureFormat_Alpha8)
    {
        AzU8 col_a = (col >> AZ_COL32_A_SHIFT) & 0xFF;
        for (int y = 0; y < h; y++)
            memset((AzU8*)dst_tex->GetPixelsAt(dst_x, dst_y + y), col_a, w);
    } else
    {
        for (int y = 0; y < h; y++)
        {
            AzU32* p = (AzU32*)(void*)dst_tex->GetPixelsAt(dst_x, dst_y + y);
            for (int x = w; x > 0; x--, p++)
                *p = col;
        }
    }
}

void AzFontAtlasTextureBlockCopy(AzTextureData* src_tex,
                                 int src_x,
                                 int src_y,
                                 AzTextureData* dst_tex,
                                 int dst_x,
                                 int dst_y,
                                 int w,
                                 int h)
{
    AZ_ASSERT(src_tex->Pixels != NULL && dst_tex->Pixels != NULL);
    AZ_ASSERT(src_tex->Format == dst_tex->Format);
    AZ_ASSERT(src_x >= 0 && src_x + w <= src_tex->Width);
    AZ_ASSERT(src_y >= 0 && src_y + h <= src_tex->Height);
    AZ_ASSERT(dst_x >= 0 && dst_x + w <= dst_tex->Width);
    AZ_ASSERT(dst_y >= 0 && dst_y + h <= dst_tex->Height);
    for (int y = 0; y < h; y++)
        memcpy(dst_tex->GetPixelsAt(dst_x, dst_y + y),
               src_tex->GetPixelsAt(src_x, src_y + y),
               w * dst_tex->BytesPerPixel);
}

void AzFontAtlasTextureBlockQueueUpload(AzFontAtlas* atlas,
                                        AzTextureData* tex,
                                        int x,
                                        int y,
                                        int w,
                                        int h)
{
    AZ_ASSERT(tex->Status != AzTextureStatus_WantDestroy
              && tex->Status != AzTextureStatus_Destroyed);
    AZ_ASSERT(x >= 0 && x <= 0xFFFF && y >= 0 && y <= 0xFFFF && w >= 0
              && x + w <= 0x10000 && h >= 0 && y + h <= 0x10000);
    AZ_UNUSED(atlas);

    AzTextureRect req = {(unsigned short)x,
                         (unsigned short)y,
                         (unsigned short)w,
                         (unsigned short)h};
    int new_x1 = AzMax(tex->UpdateRect.w == 0
                               ? 0
                               : tex->UpdateRect.x + tex->UpdateRect.w,
                       req.x + req.w);
    int new_y1 = AzMax(tex->UpdateRect.h == 0
                               ? 0
                               : tex->UpdateRect.y + tex->UpdateRect.h,
                       req.y + req.h);
    tex->UpdateRect.x = AzMin(tex->UpdateRect.x, req.x);
    tex->UpdateRect.y = AzMin(tex->UpdateRect.y, req.y);
    tex->UpdateRect.w = (unsigned short)(new_x1 - tex->UpdateRect.x);
    tex->UpdateRect.h = (unsigned short)(new_y1 - tex->UpdateRect.y);
    tex->UsedRect.x = AzMin(tex->UsedRect.x, req.x);
    tex->UsedRect.y = AzMin(tex->UsedRect.y, req.y);
    tex->UsedRect.w = (unsigned short)(AzMax(tex->UsedRect.x + tex->UsedRect.w,
                                             req.x + req.w)
                                       - tex->UsedRect.x);
    tex->UsedRect.h = (unsigned short)(AzMax(tex->UsedRect.y + tex->UsedRect.h,
                                             req.y + req.h)
                                       - tex->UsedRect.y);
    atlas->TexIsBuilt = false;

    if (tex->Status == AzTextureStatus_OK
        || tex->Status == AzTextureStatus_WantUpdates)
    {
        tex->Status = AzTextureStatus_WantUpdates;
        tex->Updates.push_back(req);
    }
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
static void GetTexDataAsFormat(AzFontAtlas* atlas,
                               AzTextureFormat format,
                               unsigned char** out_pixels,
                               int* out_width,
                               int* out_height,
                               int* out_bytes_per_pixel)
{
    AzTextureData* tex = atlas->TexData;
    if (!atlas->TexIsBuilt || tex == NULL || tex->Pixels == NULL
        || atlas->TexDesiredFormat != format)
    {
        atlas->TexDesiredFormat = format;
        atlas->Build();
        tex = atlas->TexData;
    }
    if (out_pixels)
    {
        *out_pixels = (unsigned char*)tex->Pixels;
    };
    if (out_width)
    {
        *out_width = tex->Width;
    };
    if (out_height)
    {
        *out_height = tex->Height;
    };
    if (out_bytes_per_pixel)
    {
        *out_bytes_per_pixel = tex->BytesPerPixel;
    }
}

void AzFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels,
                                     int* out_width,
                                     int* out_height,
                                     int* out_bytes_per_pixel)
{
    GetTexDataAsFormat(this,
                       AzTextureFormat_Alpha8,
                       out_pixels,
                       out_width,
                       out_height,
                       out_bytes_per_pixel);
}

void AzFontAtlas::GetTexDataAsRGBA32(unsigned char** out_pixels,
                                     int* out_width,
                                     int* out_height,
                                     int* out_bytes_per_pixel)
{
    GetTexDataAsFormat(this,
                       AzTextureFormat_RGBA32,
                       out_pixels,
                       out_width,
                       out_height,
                       out_bytes_per_pixel);
}

bool AzFontAtlas::Build()
{
    AzFontAtlasBuildMain(this);
    return true;
}
#endif

AzFont* AzFontAtlas::AddFont(const AzFontConfig* font_cfg_in)
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    AZ_ASSERT((font_cfg_in->FontData != NULL && font_cfg_in->FontDataSize > 0)
              || (font_cfg_in->FontLoader != NULL));

    AZ_ASSERT(font_cfg_in->RasterizerDensity > 0.0f
              && "Is AzFontConfig struct correctly initialized?");
    if (font_cfg_in->GlyphOffset.x != 0.0f || font_cfg_in->GlyphOffset.y != 0.0f
        || font_cfg_in->GlyphMinAdvanceX != 0.0f
        || font_cfg_in->GlyphMaxAdvanceX != FLT_MAX)
        AZ_ASSERT(font_cfg_in->SizePixels != 0.0f && "Specifying glyph offset/advances requires a reference size to base it on.");

    if (Builder == NULL)
        AzFontAtlasBuildInit(this);

    AzFont* font;
    if (!font_cfg_in->MergeMode)
    {
        font = AZ_NEW(AzFont)();
        font->FontId = FontNextUniqueID++;
        font->Flags = font_cfg_in->Flags;
        font->LegacySize = font_cfg_in->SizePixels;
        font->CurrentRasterizerDensity = font_cfg_in->RasterizerDensity;
        Fonts.push_back(font);
    } else
    {
        AZ_ASSERT(Fonts.Size > 0 && "Cannot use MergeMode for the first font");
        font = Fonts.back();
    }

    Sources.push_back(*font_cfg_in);
    AzFontConfig* font_cfg = &Sources.back();
    if (font_cfg->DstFont == NULL)
        font_cfg->DstFont = font;
    font->Sources.push_back(font_cfg);
    AzFontAtlasBuildUpdatePointers(this);

    if (font_cfg->FontDataOwnedByAtlas == false)
    {
        font_cfg->FontDataOwnedByAtlas = true;
        font_cfg->FontData = AzMemdup(font_cfg->FontData,
                                      (size_t)font_cfg->FontDataSize);
    }

    if (font_cfg->GlyphExcludeRanges != NULL)
    {
        int size = 0;
        for (const AzWchar* p = font_cfg->GlyphExcludeRanges; p[0] != 0;
             p++, size++)
        {}
        AZ_ASSERT((size & 1) == 0
                  && "GlyphExcludeRanges[] size must be multiple of two!");
        AZ_ASSERT((size <= 64) && "GlyphExcludeRanges[] size must be small!");
        font_cfg->GlyphExcludeRanges = (AzWchar*)
                AzMemdup(font_cfg->GlyphExcludeRanges,
                         sizeof(font_cfg->GlyphExcludeRanges[0]) * (size + 1));
    }
    if (font_cfg->FontLoader != NULL)
    {
        AZ_ASSERT(font_cfg->FontLoader->FontBakedLoadGlyph != NULL);
        AZ_ASSERT(font_cfg->FontLoader->LoaderInit == NULL
                  && font_cfg->FontLoader->LoaderShutdown == NULL);
    }
    AZ_ASSERT(font_cfg->FontLoaderData == NULL);

    if (!AzFontAtlasFontSourceInit(this, font_cfg))
    {
        AzFontAtlasFontDestroySourceData(this, font_cfg);
        Sources.pop_back();
        font->Sources.pop_back();
        if (!font_cfg->MergeMode)
        {
            AZ_DELETE(font);
            Fonts.pop_back();
        }
        return NULL;
    }
    AzFontAtlasFontSourceAddToFont(this, font, font_cfg);

    return font;
}

static unsigned int stb_decompress_length(const unsigned char* input);
static unsigned int stb_decompress(unsigned char* output,
                                   const unsigned char* input,
                                   unsigned int length);
static unsigned int Decode85Byte(char c)
{
    return c >= '\\' ? c - 36 : c - 35;
}
static void Decode85(const unsigned char* src, unsigned char* dst)
{
    while (*src)
    {
        unsigned int tmp =
                Decode85Byte(src[0])
                + 85
                          * (Decode85Byte(src[1])
                             + 85
                                       * (Decode85Byte(src[2])
                                          + 85
                                                    * (Decode85Byte(src[3])
                                                       + 85
                                                                 * Decode85Byte(
                                                                         src[4]))));
        dst[0] = ((tmp >> 0) & 0xFF);
        dst[1] = ((tmp >> 8) & 0xFF);
        dst[2] = ((tmp >> 16) & 0xFF);
        dst[3] = ((tmp >> 24) & 0xFF);
        src += 5;
        dst += 4;
    }
}
#ifndef AZ_DISABLE_DEFAULT_FONT
static const char* GetDefaultCompressedFontDataTTF(int* out_size);
#endif

AzFont* AzFontAtlas::AddFontDefault(const AzFontConfig* font_cfg_template)
{
#ifndef AZ_DISABLE_DEFAULT_FONT
    AzFontConfig font_cfg = font_cfg_template ? *font_cfg_template
                                              : AzFontConfig();
    if (!font_cfg_template)
    {
        font_cfg.OversampleH = font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
    }
    if (font_cfg.SizePixels <= 0.0f)
        font_cfg.SizePixels = 16.0f * 1.0f;
    if (font_cfg.Name[0] == '\0')
        AzFormatString(font_cfg.Name,
                       AZ_ARRAYSIZE(font_cfg.Name),
                       "ProggyClean.ttf");
    font_cfg.EllipsisChar = (AzWchar)0x0085;
    font_cfg.GlyphOffset.y = 1.0f * AZ_TRUNC(font_cfg.SizePixels / 13.0f);

    int ttf_compressed_size = 16;
    const char* ttf_compressed = GetDefaultCompressedFontDataTTF(
            &ttf_compressed_size);
    const AzWchar* glyph_ranges = font_cfg.GlyphRanges != NULL
                                          ? font_cfg.GlyphRanges
                                          : GetGlyphRangesDefault();
    AzFont* font = AddFontFromMemoryCompressedTTF(ttf_compressed,
                                                  ttf_compressed_size,
                                                  font_cfg.SizePixels,
                                                  &font_cfg,
                                                  glyph_ranges);
    return font;
#else
    AZ_ASSERT(0 && "AddFontDefault() disabled in this build.");
    AZ_UNUSED(font_cfg_template);
    return NULL;
#endif
}

AzFont* AzFontAtlas::AddFontFromFileTTF(const char* filename,
                                        float size_pixels,
                                        const AzFontConfig* font_cfg_template,
                                        const AzWchar* glyph_ranges)
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    size_t data_size = 0;
    void* data = AzFileLoadToMemory(filename, "rb", &data_size, 0);
    if (!data)
    {
        if (font_cfg_template == NULL
            || (font_cfg_template->Flags & AzFontFlags_NoLoadError) == 0)
        {
            AZ_DEBUG_LOG("While loading '%s'\n", filename);
            AZ_ASSERT_USER_ERROR(0, "Could not load font file!");
        }
        return NULL;
    }
    AzFontConfig font_cfg = font_cfg_template ? *font_cfg_template
                                              : AzFontConfig();
    if (font_cfg.Name[0] == '\0')
    {
        const char* p;
        for (p = filename + AzStrlen(filename);
             p > filename && p[-1] != '/' && p[-1] != '\\';
             p--)
        {}
        AzFormatString(font_cfg.Name, AZ_ARRAYSIZE(font_cfg.Name), "%s", p);
    }
    return AddFontFromMemoryTTF(data,
                                (int)data_size,
                                size_pixels,
                                &font_cfg,
                                glyph_ranges);
}

AzFont* AzFontAtlas::AddFontFromMemoryTTF(void* font_data,
                                          int font_data_size,
                                          float size_pixels,
                                          const AzFontConfig* font_cfg_template,
                                          const AzWchar* glyph_ranges)
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    AzFontConfig font_cfg = font_cfg_template ? *font_cfg_template
                                              : AzFontConfig();
    AZ_ASSERT(font_cfg.FontData == NULL);
    AZ_ASSERT(font_data_size > 100 && "Incorrect value for font_data_size!");
    font_cfg.FontData = font_data;
    font_cfg.FontDataSize = font_data_size;
    font_cfg.SizePixels = size_pixels > 0.0f ? size_pixels
                                             : font_cfg.SizePixels;
    if (glyph_ranges)
        font_cfg.GlyphRanges = glyph_ranges;
    return AddFont(&font_cfg);
}

AzFont* AzFontAtlas::AddFontFromMemoryCompressedTTF(
        const void* compressed_ttf_data,
        int compressed_ttf_size,
        float size_pixels,
        const AzFontConfig* font_cfg_template,
        const AzWchar* glyph_ranges)
{
    const unsigned int buf_decompressed_size = stb_decompress_length(
            (const unsigned char*)compressed_ttf_data);
    unsigned char* buf_decompressed_data = (unsigned char*)AZ_ALLOC(
            buf_decompressed_size);
    stb_decompress(buf_decompressed_data,
                   (const unsigned char*)compressed_ttf_data,
                   (unsigned int)compressed_ttf_size);

    AzFontConfig font_cfg = font_cfg_template ? *font_cfg_template
                                              : AzFontConfig();
    AZ_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontDataOwnedByAtlas = true;
    return AddFontFromMemoryTTF(buf_decompressed_data,
                                (int)buf_decompressed_size,
                                size_pixels,
                                &font_cfg,
                                glyph_ranges);
}

AzFont* AzFontAtlas::AddFontFromMemoryCompressedBase85TTF(
        const char* compressed_ttf_data_base85,
        float size_pixels,
        const AzFontConfig* font_cfg,
        const AzWchar* glyph_ranges)
{
    int compressed_ttf_size = (((int)AzStrlen(compressed_ttf_data_base85) + 4)
                               / 5)
                              * 4;
    void* compressed_ttf = AZ_ALLOC((size_t)compressed_ttf_size);
    Decode85((const unsigned char*)compressed_ttf_data_base85,
             (unsigned char*)compressed_ttf);
    AzFont* font = AddFontFromMemoryCompressedTTF(compressed_ttf,
                                                  compressed_ttf_size,
                                                  size_pixels,
                                                  font_cfg,
                                                  glyph_ranges);
    AZ_FREE(compressed_ttf);
    return font;
}

static void AzFontAtlasBuildNotifySetFont(AzFontAtlas* atlas,
                                          AzFont* old_font,
                                          AzFont* new_font)
{
    for (AzDrawListSharedData* shared_data : atlas->DrawListSharedDatas)
    {
        if (shared_data->Font == old_font)
            shared_data->Font = new_font;
        if (AzContext* ctx = shared_data->Context)
        {
            if (ctx->IO.FontDefault == old_font)
                ctx->IO.FontDefault = new_font;
            if (ctx->Font == old_font)
            {
                AzContext* curr_ctx = Az::GetCurrentContext();
                bool need_bind_ctx = ctx != curr_ctx;
                if (need_bind_ctx)
                    Az::SetCurrentContext(ctx);
                Az::SetCurrentFont(new_font, ctx->FontSizeBase, ctx->FontSize);
                if (need_bind_ctx)
                    Az::SetCurrentContext(curr_ctx);
            }
            for (AzFontStackData& font_stack_data : ctx->FontStack)
                if (font_stack_data.Font == old_font)
                    font_stack_data.Font = new_font;
        }
    }
}

void AzFontAtlas::RemoveFont(AzFont* font)
{
    AZ_ASSERT(!Locked && "Cannot modify a locked AzFontAtlas!");
    font->ClearOutputData();

    AzFontAtlasFontDestroyOutput(this, font);
    for (AzFontConfig* src : font->Sources)
        AzFontAtlasFontDestroySourceData(this, src);
    for (int src_n = 0; src_n < Sources.Size; src_n++)
        if (Sources[src_n].DstFont == font)
            Sources.erase(&Sources[src_n--]);

    bool removed = Fonts.find_erase(font);
    AZ_ASSERT(removed);
    AZ_UNUSED(removed);

    AzFontAtlasBuildUpdatePointers(this);

    font->ContainerAtlas = NULL;
    AZ_DELETE(font);

    AzFont* new_current_font = Fonts.empty() ? NULL : Fonts[0];
    AzFontAtlasBuildNotifySetFont(this, font, new_current_font);
}

AzFontAtlasRectId AzFontAtlas::AddCustomRect(int width,
                                             int height,
                                             AzFontAtlasRect* out_r)
{
    AZ_ASSERT(width > 0 && width <= 0xFFFF);
    AZ_ASSERT(height > 0 && height <= 0xFFFF);

    if (Builder == NULL)
        AzFontAtlasBuildInit(this);

    AzFontAtlasRectId r_id = AzFontAtlasPackAddRect(this, width, height);
    if (r_id == AzFontAtlasRectId_Invalid)
        return AzFontAtlasRectId_Invalid;
    if (out_r != NULL)
        GetCustomRect(r_id, out_r);

    if (RendererHasTextures)
    {
        AzTextureRect* r = AzFontAtlasPackGetRect(this, r_id);
        AzFontAtlasTextureBlockQueueUpload(this,
                                           TexData,
                                           r->x,
                                           r->y,
                                           r->w,
                                           r->h);
    }
    return r_id;
}

void AzFontAtlas::RemoveCustomRect(AzFontAtlasRectId id)
{
    if (AzFontAtlasPackGetRectSafe(this, id) == NULL)
        return;
    AzFontAtlasPackDiscardRect(this, id);
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

AzFontAtlasRectId AzFontAtlas::AddCustomRectFontGlyph(AzFont* font,
                                                      AzWchar codepoint,
                                                      int width,
                                                      int height,
                                                      float advance_x,
                                                      const AzVec2& offset)
{
    float font_size = font->LegacySize;
    return AddCustomRectFontGlyphForSize(font,
                                         font_size,
                                         codepoint,
                                         width,
                                         height,
                                         advance_x,
                                         offset);
}

AzFontAtlasRectId AzFontAtlas::AddCustomRectFontGlyphForSize(
        AzFont* font,
        float font_size,
        AzWchar codepoint,
        int width,
        int height,
        float advance_x,
        const AzVec2& offset)
{
#ifdef AZ_USE_WCHAR32
    AZ_ASSERT(codepoint <= AZ_UNICODE_CODEPOINT_MAX);
#endif
    AZ_ASSERT(font != NULL);
    AZ_ASSERT(width > 0 && width <= 0xFFFF);
    AZ_ASSERT(height > 0 && height <= 0xFFFF);

    AzFontBaked* baked = font->GetFontBaked(font_size);

    AzFontAtlasRectId r_id = AzFontAtlasPackAddRect(this, width, height);
    if (r_id == AzFontAtlasRectId_Invalid)
        return AzFontAtlasRectId_Invalid;
    AzTextureRect* r = AzFontAtlasPackGetRect(this, r_id);
    if (RendererHasTextures)
        AzFontAtlasTextureBlockQueueUpload(this,
                                           TexData,
                                           r->x,
                                           r->y,
                                           r->w,
                                           r->h);

    if (baked->IsGlyphLoaded(codepoint))
        AzFontAtlasBakedDiscardFontGlyph(this,
                                         font,
                                         baked,
                                         baked->FindGlyph(codepoint));

    AzFontGlyph glyph;
    glyph.Codepoint = codepoint;
    glyph.AdvanceX = advance_x;
    glyph.X0 = offset.x;
    glyph.Y0 = offset.y;
    glyph.X1 = offset.x + r->w;
    glyph.Y1 = offset.y + r->h;
    glyph.Visible = true;
    glyph.Colored = true;
    glyph.PackId = r_id;
    AzFontAtlasBakedAddFontGlyph(this, baked, font->Sources[0], &glyph);
    return r_id;
}
#endif

bool AzFontAtlas::GetCustomRect(AzFontAtlasRectId id,
                                AzFontAtlasRect* out_r) const
{
    AzTextureRect* r = AzFontAtlasPackGetRectSafe((AzFontAtlas*)this, id);
    if (r == NULL)
        return false;
    AZ_ASSERT(TexData->Width > 0 && TexData->Height > 0);
    if (out_r == NULL)
        return true;
    out_r->x = r->x;
    out_r->y = r->y;
    out_r->w = r->w;
    out_r->h = r->h;
    out_r->uv0 = AzVec2((float)(r->x), (float)(r->y)) * TexUvScale;
    out_r->uv1 = AzVec2((float)(r->x + r->w), (float)(r->y + r->h))
                 * TexUvScale;
    return true;
}

bool AzFontAtlasGetMouseCursorTexData(AzFontAtlas* atlas,
                                      AzMouseCursor cursor_type,
                                      AzVec2* out_offset,
                                      AzVec2* out_size,
                                      AzVec2 out_uv_border[2],
                                      AzVec2 out_uv_fill[2])
{
    if (cursor_type <= AzMouseCursor_None || cursor_type >= AzMouseCursor_COUNT)
        return false;
    if (atlas->Flags & AzFontAtlasFlags_NoMouseCursors)
        return false;

    AzTextureRect* r =
            AzFontAtlasPackGetRect(atlas, atlas->Builder->PackIdMouseCursors);
    AzVec2 pos = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][0]
                 + AzVec2((float)r->x, (float)r->y);
    AzVec2 size = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][1];
    *out_size = size;
    *out_offset = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][2];
    out_uv_border[0] = (pos)*atlas->TexUvScale;
    out_uv_border[1] = (pos + size) * atlas->TexUvScale;
    pos.x += FONT_ATLAS_DEFAULT_TEX_DATA_W + 1;
    out_uv_fill[0] = (pos)*atlas->TexUvScale;
    out_uv_fill[1] = (pos + size) * atlas->TexUvScale;
    return true;
}

void AzFontAtlasBuildMain(AzFontAtlas* atlas)
{
    AZ_ASSERT(!atlas->Locked && "Cannot modify a locked AzFontAtlas!");
    if (atlas->TexData && atlas->TexData->Format != atlas->TexDesiredFormat)
    {
        AzVec2i new_tex_size = AzFontAtlasTextureGetSizeEstimate(atlas);
        AzFontAtlasBuildDestroy(atlas);
        AzFontAtlasTextureAdd(atlas, new_tex_size.x, new_tex_size.y);
    }

    if (atlas->Builder == NULL)
        AzFontAtlasBuildInit(atlas);

    if (atlas->Sources.Size == 0)
        atlas->AddFontDefault();

    AzFontAtlasBuildUpdateRendererHasTexturesFromContext(atlas);
    if (atlas->RendererHasTextures == false)
        AzFontAtlasBuildLegacyPreloadAllGlyphRanges(atlas);
    atlas->TexIsBuilt = true;
}

void AzFontAtlasBuildGetOversampleFactors(AzFontConfig* src,
                                          AzFontBaked* baked,
                                          int* out_oversample_h,
                                          int* out_oversample_v)
{
    const float raster_size = baked->Size * baked->RasterizerDensity
                              * src->RasterizerDensity;
    *out_oversample_h = (src->OversampleH != 0) ? src->OversampleH
                        : (raster_size > 36.0f || src->PixelSnapH) ? 1
                                                                   : 2;
    *out_oversample_v = (src->OversampleV != 0) ? src->OversampleV : 1;
}

void AzFontAtlasBuildSetupFontLoader(AzFontAtlas* atlas,
                                     const AzFontLoader* font_loader)
{
    if (atlas->FontLoader == font_loader)
        return;
    AZ_ASSERT(!atlas->Locked && "Cannot modify a locked AzFontAtlas!");

    for (AzFont* font : atlas->Fonts)
        AzFontAtlasFontDestroyOutput(atlas, font);
    if (atlas->Builder && atlas->FontLoader
        && atlas->FontLoader->LoaderShutdown)
        atlas->FontLoader->LoaderShutdown(atlas);

    atlas->FontLoader = font_loader;
    atlas->FontLoaderName = font_loader ? font_loader->Name : "NULL";
    AZ_ASSERT(atlas->FontLoaderData == NULL);

    if (atlas->Builder && atlas->FontLoader && atlas->FontLoader->LoaderInit)
        atlas->FontLoader->LoaderInit(atlas);
    for (AzFont* font : atlas->Fonts)
        AzFontAtlasFontInitOutput(atlas, font);
}

void AzFontAtlasBuildLegacyPreloadAllGlyphRanges(AzFontAtlas* atlas)
{
    atlas->Builder->PreloadedAllGlyphsRanges = true;
    for (AzFont* font : atlas->Fonts)
    {
        AzFontBaked* baked = font->GetFontBaked(font->LegacySize);
        if (font->FallbackChar != 0)
            baked->FindGlyph(font->FallbackChar);
        if (font->EllipsisChar != 0)
            baked->FindGlyph(font->EllipsisChar);
        for (AzFontConfig* src : font->Sources)
        {
            const AzWchar* ranges = src->GlyphRanges
                                            ? src->GlyphRanges
                                            : atlas->GetGlyphRangesDefault();
            for (; ranges[0]; ranges += 2)
                for (unsigned int c = ranges[0];
                     c <= ranges[1] && c <= AZ_UNICODE_CODEPOINT_MAX;
                     c++)
                    baked->FindGlyph((AzWchar)c);
        }
    }
}

void AzFontAtlasBuildUpdatePointers(AzFontAtlas* atlas)
{
    for (AzFont* font : atlas->Fonts)
        font->Sources.resize(0);
    for (AzFontConfig& src : atlas->Sources)
        src.DstFont->Sources.push_back(&src);
}

void AzFontAtlasBuildRenderBitmapFromString(AzFontAtlas* atlas,
                                            int x,
                                            int y,
                                            int w,
                                            int h,
                                            const char* in_str,
                                            char in_marker_char)
{
    AzTextureData* tex = atlas->TexData;
    AZ_ASSERT(x >= 0 && x + w <= tex->Width);
    AZ_ASSERT(y >= 0 && y + h <= tex->Height);

    switch (tex->Format)
    {
    case AzTextureFormat_Alpha8: {
        AzU8* out_p = tex->GetPixelsAt(x, y);
        for (int off_y = 0; off_y < h;
             off_y++, out_p += tex->Width, in_str += w)
            for (int off_x = 0; off_x < w; off_x++)
                out_p[off_x] = (in_str[off_x] == in_marker_char) ? 0xFF : 0x00;
        break;
    }
    case AzTextureFormat_RGBA32: {
        AzU32* out_p = (AzU32*)(void*)tex->GetPixelsAt(x, y);
        for (int off_y = 0; off_y < h;
             off_y++, out_p += tex->Width, in_str += w)
            for (int off_x = 0; off_x < w; off_x++)
                out_p[off_x] = (in_str[off_x] == in_marker_char)
                                       ? AZ_COL32_WHITE
                                       : AZ_COL32_BLACK_TRANS;
        break;
    }
    }
}

static void AzFontAtlasBuildUpdateBasicTexData(AzFontAtlas* atlas)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    AzVec2i pack_size = (atlas->Flags & AzFontAtlasFlags_NoMouseCursors)
                                ? AzVec2i(2, 2)
                                : AzVec2i(FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1,
                                          FONT_ATLAS_DEFAULT_TEX_DATA_H);

    AzFontAtlasRect r;
    bool add_and_draw = (atlas->GetCustomRect(builder->PackIdMouseCursors, &r)
                         == false);
    if (add_and_draw)
    {
        builder->PackIdMouseCursors = atlas->AddCustomRect(pack_size.x,
                                                           pack_size.y,
                                                           &r);
        AZ_ASSERT(builder->PackIdMouseCursors != AzFontAtlasRectId_Invalid);

        if (atlas->Flags & AzFontAtlasFlags_NoMouseCursors)
        {
            AzFontAtlasBuildRenderBitmapFromString(atlas,
                                                   r.x,
                                                   r.y,
                                                   2,
                                                   2,
                                                   "XX"
                                                   "XX",
                                                   'X');
        } else
        {
            const int x_for_white = r.x;
            const int x_for_black = r.x + FONT_ATLAS_DEFAULT_TEX_DATA_W + 1;
            AzFontAtlasBuildRenderBitmapFromString(
                    atlas,
                    x_for_white,
                    r.y,
                    FONT_ATLAS_DEFAULT_TEX_DATA_W,
                    FONT_ATLAS_DEFAULT_TEX_DATA_H,
                    FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS,
                    '.');
            AzFontAtlasBuildRenderBitmapFromString(
                    atlas,
                    x_for_black,
                    r.y,
                    FONT_ATLAS_DEFAULT_TEX_DATA_W,
                    FONT_ATLAS_DEFAULT_TEX_DATA_H,
                    FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS,
                    'X');
        }
    }

    atlas->TexUvWhitePixel = AzVec2((r.x + 0.5f) * atlas->TexUvScale.x,
                                    (r.y + 0.5f) * atlas->TexUvScale.y);
}

static void AzFontAtlasBuildUpdateLinesTexData(AzFontAtlas* atlas)
{
    if (atlas->Flags & AzFontAtlasFlags_NoBakedLines)
        return;

    AzTextureData* tex = atlas->TexData;
    AzFontAtlasBuilder* builder = atlas->Builder;

    AzFontAtlasRect r;
    bool add_and_draw = atlas->GetCustomRect(builder->PackIdLinesTexData, &r)
                        == false;
    if (add_and_draw)
    {
        AzVec2i pack_size = AzVec2i(AZ_DRAWLIST_TEX_LINES_WIDTH_MAX + 2,
                                    AZ_DRAWLIST_TEX_LINES_WIDTH_MAX + 1);
        builder->PackIdLinesTexData = atlas->AddCustomRect(pack_size.x,
                                                           pack_size.y,
                                                           &r);
        AZ_ASSERT(builder->PackIdLinesTexData != AzFontAtlasRectId_Invalid);
    }

    for (int n = 0; n < AZ_DRAWLIST_TEX_LINES_WIDTH_MAX + 1; n++)
    {
        const int y = n;
        const int line_width = n;
        const int pad_left = (r.w - line_width) / 2;
        const int pad_right = r.w - (pad_left + line_width);
        AZ_ASSERT(pad_left + line_width + pad_right == r.w && y < r.h);

        if (add_and_draw && tex->Format == AzTextureFormat_Alpha8)
        {
            AzU8* write_ptr = (AzU8*)tex->GetPixelsAt(r.x, r.y + y);
            for (int i = 0; i < pad_left; i++)
                *(write_ptr + i) = 0x00;

            for (int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = 0xFF;

            for (int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = 0x00;
        } else if (add_and_draw && tex->Format == AzTextureFormat_RGBA32)
        {
            AzU32* write_ptr = (AzU32*)(void*)tex->GetPixelsAt(r.x, r.y + y);
            for (int i = 0; i < pad_left; i++)
                *(write_ptr + i) = AZ_COL32(255, 255, 255, 0);

            for (int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = AZ_COL32_WHITE;

            for (int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width
                  + i) = AZ_COL32(255, 255, 255, 0);
        }

        AzVec2 uv0 = AzVec2((float)(r.x + pad_left - 1), (float)(r.y + y))
                     * atlas->TexUvScale;
        AzVec2 uv1 = AzVec2((float)(r.x + pad_left + line_width + 1),
                            (float)(r.y + y + 1))
                     * atlas->TexUvScale;
        float half_v = (uv0.y + uv1.y) * 0.5f;
        atlas->TexUvLines[n] = AzVec4(uv0.x, half_v, uv1.x, half_v);
    }
}

bool AzFontAtlasFontInitOutput(AzFontAtlas* atlas, AzFont* font)
{
    bool ret = true;
    for (AzFontConfig* src : font->Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        if (loader && loader->FontSrcInit != NULL
            && !loader->FontSrcInit(atlas, src))
            ret = false;
    }
    AZ_ASSERT(ret);
    return ret;
}

void AzFontAtlasFontDestroyOutput(AzFontAtlas* atlas, AzFont* font)
{
    font->ClearOutputData();
    for (AzFontConfig* src : font->Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        if (loader && loader->FontSrcDestroy != NULL)
            loader->FontSrcDestroy(atlas, src);
    }
}

bool AzFontAtlasFontSourceInit(AzFontAtlas* atlas, AzFontConfig* src)
{
    const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                 : atlas->FontLoader;
    if (loader->FontSrcInit != NULL && !loader->FontSrcInit(atlas, src))
        return false;
    return true;
}

void AzFontAtlasFontSourceAddToFont(AzFontAtlas* atlas,
                                    AzFont* font,
                                    AzFontConfig* src)
{
    if (src->MergeMode == false)
    {
        font->ClearOutputData();

        font->ContainerAtlas = atlas;
        AZ_ASSERT(font->Sources[0] == src);
    }
    atlas->TexIsBuilt = false;
    AzFontAtlasBuildSetupFontSpecialGlyphs(atlas, font, src);
}

void AzFontAtlasFontDestroySourceData(AzFontAtlas* atlas, AzFontConfig* src)
{
    AZ_UNUSED(atlas);
    if (src->FontDataOwnedByAtlas)
        AZ_FREE(src->FontData);
    src->FontData = NULL;
    if (src->GlyphExcludeRanges)
        AZ_FREE((void*)src->GlyphExcludeRanges);
    src->GlyphExcludeRanges = NULL;
}

static AzFontGlyph* AzFontAtlasBuildSetupFontBakedEllipsis(AzFontAtlas* atlas,
                                                           AzFontBaked* baked)
{
    AzFont* font = baked->ContainerFont;
    AZ_ASSERT(font->EllipsisChar != 0);

    const AzFontGlyph* dot_glyph = baked->FindGlyphNoFallback((AzWchar)'.');
    if (dot_glyph == NULL)
        dot_glyph = baked->FindGlyphNoFallback((AzWchar)0xFF0E);
    if (dot_glyph == NULL)
        return NULL;
    AzFontAtlasRectId dot_r_id = dot_glyph->PackId;
    AzTextureRect* dot_r = AzFontAtlasPackGetRect(atlas, dot_r_id);
    const int dot_spacing = 1;
    const float dot_step = (dot_glyph->X1 - dot_glyph->X0) + dot_spacing;

    AzFontAtlasRectId pack_id = AzFontAtlasPackAddRect(atlas,
                                                       (dot_r->w * 3
                                                        + dot_spacing * 2),
                                                       dot_r->h);
    AzTextureRect* r = AzFontAtlasPackGetRect(atlas, pack_id);

    AzFontGlyph glyph_in = {};
    AzFontGlyph* glyph = &glyph_in;
    glyph->Codepoint = font->EllipsisChar;
    glyph->AdvanceX = AzMax(dot_glyph->AdvanceX,
                            dot_glyph->X0 + dot_step * 3.0f - dot_spacing);
    glyph->X0 = dot_glyph->X0;
    glyph->Y0 = dot_glyph->Y0;
    glyph->X1 = dot_glyph->X0 + dot_step * 3 - dot_spacing;
    glyph->Y1 = dot_glyph->Y1;
    glyph->Visible = true;
    glyph->PackId = pack_id;
    glyph = AzFontAtlasBakedAddFontGlyph(atlas, baked, NULL, glyph);
    dot_glyph = NULL;

    dot_r = AzFontAtlasPackGetRect(atlas, dot_r_id);
    AzTextureData* tex = atlas->TexData;
    for (int n = 0; n < 3; n++)
        AzFontAtlasTextureBlockCopy(tex,
                                    dot_r->x,
                                    dot_r->y,
                                    tex,
                                    r->x + (dot_r->w + dot_spacing) * n,
                                    r->y,
                                    dot_r->w,
                                    dot_r->h);
    AzFontAtlasTextureBlockQueueUpload(atlas, tex, r->x, r->y, r->w, r->h);

    return glyph;
}

static void AzFontAtlasBuildSetupFontBakedFallback(AzFontBaked* baked)
{
    AZ_ASSERT(baked->FallbackGlyphIndex == -1);
    AZ_ASSERT(baked->FallbackAdvanceX == 0.0f);
    AzFont* font = baked->ContainerFont;
    AzFontGlyph* fallback_glyph = NULL;
    if (font->FallbackChar != 0)
        fallback_glyph = baked->FindGlyphNoFallback(font->FallbackChar);
    if (fallback_glyph == NULL)
    {
        AzFontGlyph* space_glyph = baked->FindGlyphNoFallback((AzWchar)' ');
        AzFontGlyph glyph;
        glyph.Codepoint = 0;
        glyph.AdvanceX = space_glyph ? space_glyph->AdvanceX
                                     : AZ_ROUND(baked->Size * 0.40f);
        fallback_glyph = AzFontAtlasBakedAddFontGlyph(font->ContainerAtlas,
                                                      baked,
                                                      NULL,
                                                      &glyph);
    }
    baked->FallbackGlyphIndex = baked->Glyphs.index_from_ptr(fallback_glyph);
    baked->FallbackAdvanceX = fallback_glyph->AdvanceX;
}

static void AzFontAtlasBuildSetupFontBakedBlanks(AzFontAtlas* atlas,
                                                 AzFontBaked* baked)
{
    AzFontGlyph* space_glyph = baked->FindGlyphNoFallback((AzWchar)' ');
    if (space_glyph != NULL)
        space_glyph->Visible = false;

    if (baked->FindGlyphNoFallback('\t') == NULL && space_glyph != NULL)
    {
        AzFontGlyph tab_glyph;
        tab_glyph.Codepoint = '\t';
        tab_glyph.AdvanceX = space_glyph->AdvanceX * AZ_TABSIZE;
        AzFontAtlasBakedAddFontGlyph(atlas, baked, NULL, &tab_glyph);
    }
}

void AzFontAtlasBuildSetupFontSpecialGlyphs(AzFontAtlas* atlas,
                                            AzFont* font,
                                            AzFontConfig* src)
{
    AZ_UNUSED(atlas);
    AZ_ASSERT(font->Sources.contains(src));

    const AzWchar fallback_chars[] = {font->FallbackChar,
                                      (AzWchar)AZ_UNICODE_CODEPOINT_INVALID,
                                      (AzWchar)'?',
                                      (AzWchar)' '};
    if (font->FallbackChar == 0)
        for (AzWchar candidate_char : fallback_chars)
            if (candidate_char != 0 && font->IsGlyphInFont(candidate_char))
            {
                font->FallbackChar = (AzWchar)candidate_char;
                break;
            }

    const AzWchar ellipsis_chars[] = {src->EllipsisChar,
                                      (AzWchar)0x2026,
                                      (AzWchar)0x0085};
    if (font->EllipsisChar == 0)
        for (AzWchar candidate_char : ellipsis_chars)
            if (candidate_char != 0 && font->IsGlyphInFont(candidate_char))
            {
                font->EllipsisChar = candidate_char;
                break;
            }
    if (font->EllipsisChar == 0)
    {
        font->EllipsisChar = 0x0085;
        font->EllipsisAutoBake = true;
    }
}

void AzFontAtlasBakedDiscardFontGlyph(AzFontAtlas* atlas,
                                      AzFont* font,
                                      AzFontBaked* baked,
                                      AzFontGlyph* glyph)
{
    if (glyph->PackId != AzFontAtlasRectId_Invalid)
    {
        AzFontAtlasPackDiscardRect(atlas, glyph->PackId);
        glyph->PackId = AzFontAtlasRectId_Invalid;
    }
    AzWchar c = (AzWchar)glyph->Codepoint;
    AZ_ASSERT(font->FallbackChar != c && font->EllipsisChar != c);
    AZ_ASSERT(glyph >= baked->Glyphs.Data
              && glyph < baked->Glyphs.Data + baked->Glyphs.Size);
    AZ_UNUSED(font);
    baked->IndexLookup[c] = AZ_FONTGLYPH_INDEX_UNUSED;
    baked->IndexAdvanceX[c] = baked->FallbackAdvanceX;
}

AzFontBaked* AzFontAtlasBakedAdd(AzFontAtlas* atlas,
                                 AzFont* font,
                                 float font_size,
                                 float font_rasterizer_density,
                                 AzID baked_id)
{
    AZ_DEBUG_LOG_FONT("[font] Created baked %.2fpx\n", font_size);
    AzFontBaked* baked = atlas->Builder->BakedPool.push_back(AzFontBaked());
    baked->Size = font_size;
    baked->RasterizerDensity = font_rasterizer_density;
    baked->BakedId = baked_id;
    baked->ContainerFont = font;
    baked->LastUsedFrame = atlas->Builder->FrameCount;

    size_t loader_data_size = 0;
    for (AzFontConfig* src : font->Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        loader_data_size += loader->FontBakedSrcLoaderDataSize;
    }
    baked->FontLoaderDatas = (loader_data_size > 0) ? AZ_ALLOC(loader_data_size)
                                                    : NULL;
    char* loader_data_p = (char*)baked->FontLoaderDatas;
    for (AzFontConfig* src : font->Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        if (loader->FontBakedInit)
            loader->FontBakedInit(atlas, src, baked, loader_data_p);
        loader_data_p += loader->FontBakedSrcLoaderDataSize;
    }

    AzFontAtlasBuildSetupFontBakedBlanks(atlas, baked);
    return baked;
}

AzFontBaked* AzFontAtlasBakedGetClosestMatch(AzFontAtlas* atlas,
                                             AzFont* font,
                                             float font_size,
                                             float font_rasterizer_density)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    for (int step_n = 0; step_n < 2; step_n++)
    {
        AzFontBaked* closest_larger_match = NULL;
        AzFontBaked* closest_smaller_match = NULL;
        for (int baked_n = 0; baked_n < builder->BakedPool.Size; baked_n++)
        {
            AzFontBaked* baked = &builder->BakedPool[baked_n];
            if (baked->ContainerFont != font || baked->WantDestroy)
                continue;
            if (step_n == 0
                && baked->RasterizerDensity != font_rasterizer_density)
                continue;
            if (baked->Size > font_size
                && (closest_larger_match == NULL
                    || baked->Size < closest_larger_match->Size))
                closest_larger_match = baked;
            if (baked->Size < font_size
                && (closest_smaller_match == NULL
                    || baked->Size > closest_smaller_match->Size))
                closest_smaller_match = baked;
        }
        if (closest_larger_match)
            if (closest_smaller_match == NULL
                || (closest_larger_match->Size >= font_size * 2.0f
                    && closest_smaller_match->Size > font_size * 0.5f))
                return closest_larger_match;
        if (closest_smaller_match)
            return closest_smaller_match;
    }
    return NULL;
}

void AzFontAtlasBakedDiscard(AzFontAtlas* atlas,
                             AzFont* font,
                             AzFontBaked* baked)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    AZ_DEBUG_LOG_FONT("[font] Discard baked %.2f for \"%s\"\n",
                      baked->Size,
                      font->GetDebugName());

    for (AzFontGlyph& glyph : baked->Glyphs)
        if (glyph.PackId != AzFontAtlasRectId_Invalid)
            AzFontAtlasPackDiscardRect(atlas, glyph.PackId);

    char* loader_data_p = (char*)baked->FontLoaderDatas;
    for (AzFontConfig* src : font->Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        if (loader->FontBakedDestroy)
            loader->FontBakedDestroy(atlas, src, baked, loader_data_p);
        loader_data_p += loader->FontBakedSrcLoaderDataSize;
    }
    if (baked->FontLoaderDatas)
    {
        AZ_FREE(baked->FontLoaderDatas);
        baked->FontLoaderDatas = NULL;
    }
    builder->BakedMap.SetVoidPtr(baked->BakedId, NULL);
    builder->BakedDiscardedCount++;
    baked->ClearOutputData();
    baked->WantDestroy = true;
    font->LastBaked = NULL;
}

void AzFontAtlasFontDiscardBakes(AzFontAtlas* atlas,
                                 AzFont* font,
                                 int unused_frames)
{
    if (AzFontAtlasBuilder* builder = atlas->Builder)
        for (int baked_n = 0; baked_n < builder->BakedPool.Size; baked_n++)
        {
            AzFontBaked* baked = &builder->BakedPool[baked_n];
            if (baked->LastUsedFrame + unused_frames
                > atlas->Builder->FrameCount)
                continue;
            if (baked->ContainerFont != font || baked->WantDestroy)
                continue;
            AzFontAtlasBakedDiscard(atlas, font, baked);
        }
}

void AzFontAtlasBuildDiscardBakes(AzFontAtlas* atlas, int unused_frames)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    for (int baked_n = 0; baked_n < builder->BakedPool.Size; baked_n++)
    {
        AzFontBaked* baked = &builder->BakedPool[baked_n];
        if (baked->LastUsedFrame + unused_frames > atlas->Builder->FrameCount)
            continue;
        if (baked->WantDestroy
            || (baked->ContainerFont->Flags & AzFontFlags_LockBakedSizes))
            continue;
        AzFontAtlasBakedDiscard(atlas, baked->ContainerFont, baked);
    }
}

void AzFontAtlasAddDrawListSharedData(AzFontAtlas* atlas,
                                      AzDrawListSharedData* data)
{
    AZ_ASSERT(!atlas->DrawListSharedDatas.contains(data));
    atlas->DrawListSharedDatas.push_back(data);
}

void AzFontAtlasRemoveDrawListSharedData(AzFontAtlas* atlas,
                                         AzDrawListSharedData* data)
{
    AZ_ASSERT(atlas->DrawListSharedDatas.contains(data));
    atlas->DrawListSharedDatas.find_erase(data);
}

void AzFontAtlasUpdateDrawListsTextures(AzFontAtlas* atlas,
                                        AzTextureRef old_tex,
                                        AzTextureRef new_tex)
{
    for (AzDrawListSharedData* shared_data : atlas->DrawListSharedDatas)
        for (AzDrawList* draw_list : shared_data->DrawLists)
        {
            if (draw_list->CmdBuffer.Size > 0
                && draw_list->_CmdHeader.TexRef == old_tex)
                draw_list->_SetTexture(new_tex);

            for (AzTextureRef& stacked_tex : draw_list->_TextureStack)
                if (stacked_tex == old_tex)
                    stacked_tex = new_tex;
        }
}

void AzFontAtlasUpdateDrawListsSharedData(AzFontAtlas* atlas)
{
    for (AzDrawListSharedData* shared_data : atlas->DrawListSharedDatas)
    {
        shared_data->TexUvWhitePixel = atlas->TexUvWhitePixel;
        shared_data->TexUvLines = atlas->TexUvLines;
    }
}

static void AzFontAtlasBuildSetTexture(AzFontAtlas* atlas, AzTextureData* tex)
{
    AzTextureRef old_tex_ref = atlas->TexRef;
    atlas->TexData = tex;
    atlas->TexUvScale = AzVec2(1.0f / tex->Width, 1.0f / tex->Height);
    atlas->TexRef._TexData = tex;

    AzFontAtlasUpdateDrawListsTextures(atlas, old_tex_ref, atlas->TexRef);
}

AzTextureData* AzFontAtlasTextureAdd(AzFontAtlas* atlas, int w, int h)
{
    AzTextureData* old_tex = atlas->TexData;
    AzTextureData* new_tex;

    {
        new_tex = AZ_NEW(AzTextureData)();
        new_tex->UniqueID = atlas->TexNextUniqueID++;
        atlas->TexList.push_back(new_tex);
    }
    if (old_tex != NULL)
    {
        old_tex->WantDestroyNextFrame = true;
        AZ_ASSERT(old_tex->Status == AzTextureStatus_OK
                  || old_tex->Status == AzTextureStatus_WantCreate
                  || old_tex->Status == AzTextureStatus_WantUpdates);
    }

    new_tex->Create(atlas->TexDesiredFormat, w, h);
    new_tex->Status = AzTextureStatus_WantCreate;
    atlas->TexIsBuilt = false;

    AzFontAtlasBuildSetTexture(atlas, new_tex);

    return new_tex;
}

#if 0
#define STB_AZAGE_WRITE_AZPLEMENTATION
#include "../stb/stb_image_write.h"
static void AzFontAtlasDebugWriteTexToDisk(AzTextureData* tex, const char* description)
{
    AzContext& g = *GAz;
    char buf[128];
    AzFormatString(buf, AZ_ARRAYSIZE(buf), "[%05d] Texture #%03d - %s.png", g.FrameCount, tex->UniqueID, description);
    stbi_write_png(buf, tex->Width, tex->Height, tex->BytesPerPixel, tex->Pixels, tex->GetPitch()); 
}
#endif

void AzFontAtlasTextureRepack(AzFontAtlas* atlas, int w, int h)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    builder->LockDisableResize = true;

    AzTextureData* old_tex = atlas->TexData;
    AzTextureData* new_tex = AzFontAtlasTextureAdd(atlas, w, h);
    new_tex->UseColors = old_tex->UseColors;
    AZ_DEBUG_LOG_FONT(
            "[font] Texture #%03d: resize+repack %dx%d => Texture #%03d: "
            "%dx%d\n",
            old_tex->UniqueID,
            old_tex->Width,
            old_tex->Height,
            new_tex->UniqueID,
            new_tex->Width,
            new_tex->Height);

    AzFontAtlasPackInit(atlas);
    AzVector<AzTextureRect> old_rects;
    AzVector<AzFontAtlasRectEntry> old_index = builder->RectsIndex;
    old_rects.swap(builder->Rects);

    for (AzFontAtlasRectEntry& index_entry : builder->RectsIndex)
    {
        if (index_entry.IsUsed == false)
            continue;
        AzTextureRect& old_r = old_rects[index_entry.TargetIndex];
        if (old_r.w == 0 && old_r.h == 0)
            continue;
        AzFontAtlasRectId new_r_id =
                AzFontAtlasPackAddRect(atlas, old_r.w, old_r.h, &index_entry);
        if (new_r_id == AzFontAtlasRectId_Invalid)
        {
            AZ_DEBUG_LOG_FONT(
                    "[font] Texture #%03d: resize failed. Will grow.\n",
                    new_tex->UniqueID);
            new_tex->WantDestroyNextFrame = true;
            builder->Rects.swap(old_rects);
            builder->RectsIndex = old_index;
            AzFontAtlasBuildSetTexture(atlas, old_tex);
            AzFontAtlasTextureGrow(atlas, w, h);
            return;
        }
        AZ_ASSERT(AzFontAtlasRectId_GetIndex(new_r_id)
                  == builder->RectsIndex.index_from_ptr(&index_entry));
        AzTextureRect* new_r = AzFontAtlasPackGetRect(atlas, new_r_id);
        AzFontAtlasTextureBlockCopy(old_tex,
                                    old_r.x,
                                    old_r.y,
                                    new_tex,
                                    new_r->x,
                                    new_r->y,
                                    new_r->w,
                                    new_r->h);
    }
    AZ_ASSERT(old_rects.Size
              == builder->Rects.Size + builder->RectsDiscardedCount);
    builder->RectsDiscardedCount = 0;
    builder->RectsDiscardedSurface = 0;

    for (int baked_n = 0; baked_n < builder->BakedPool.Size; baked_n++)
        for (AzFontGlyph& glyph : builder->BakedPool[baked_n].Glyphs)
            if (glyph.PackId != AzFontAtlasRectId_Invalid)
            {
                AzTextureRect* r = AzFontAtlasPackGetRect(atlas, glyph.PackId);
                glyph.U0 = (r->x) * atlas->TexUvScale.x;
                glyph.V0 = (r->y) * atlas->TexUvScale.y;
                glyph.U1 = (r->x + r->w) * atlas->TexUvScale.x;
                glyph.V1 = (r->y + r->h) * atlas->TexUvScale.y;
            }

    AzFontAtlasBuildUpdateLinesTexData(atlas);
    AzFontAtlasBuildUpdateBasicTexData(atlas);

    builder->LockDisableResize = false;
    AzFontAtlasUpdateDrawListsSharedData(atlas);
}

void AzFontAtlasTextureGrow(AzFontAtlas* atlas, int old_tex_w, int old_tex_h)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    if (old_tex_w == -1)
        old_tex_w = atlas->TexData->Width;
    if (old_tex_h == -1)
        old_tex_h = atlas->TexData->Height;

    AZ_ASSERT(AzIsPowerOfTwo(old_tex_w) && AzIsPowerOfTwo(old_tex_h));
    AZ_ASSERT(AzIsPowerOfTwo(atlas->TexMinWidth)
              && AzIsPowerOfTwo(atlas->TexMaxWidth)
              && AzIsPowerOfTwo(atlas->TexMinHeight)
              && AzIsPowerOfTwo(atlas->TexMaxHeight));

    int new_tex_w = (old_tex_h <= old_tex_w) ? old_tex_w : old_tex_w * 2;
    int new_tex_h = (old_tex_h <= old_tex_w) ? old_tex_h * 2 : old_tex_h;

    const int pack_padding = atlas->TexGlyphPadding;
    new_tex_w = AzMax(new_tex_w,
                      AzUpperPowerOfTwo(builder->MaxRectSize.x + pack_padding));
    new_tex_h = AzMax(new_tex_h,
                      AzUpperPowerOfTwo(builder->MaxRectSize.y + pack_padding));
    new_tex_w = AzClamp(new_tex_w, atlas->TexMinWidth, atlas->TexMaxWidth);
    new_tex_h = AzClamp(new_tex_h, atlas->TexMinHeight, atlas->TexMaxHeight);
    if (new_tex_w == old_tex_w && new_tex_h == old_tex_h)
        return;

    AzFontAtlasTextureRepack(atlas, new_tex_w, new_tex_h);
}

void AzFontAtlasTextureMakeSpace(AzFontAtlas* atlas)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    AzFontAtlasBuildDiscardBakes(atlas, 2);

    if (builder->RectsDiscardedSurface < builder->RectsPackedSurface * 0.20f)
        AzFontAtlasTextureGrow(atlas);
    else
        AzFontAtlasTextureRepack(atlas,
                                 atlas->TexData->Width,
                                 atlas->TexData->Height);
}

AzVec2i AzFontAtlasTextureGetSizeEstimate(AzFontAtlas* atlas)
{
    int min_w = AzUpperPowerOfTwo(atlas->TexMinWidth);
    int min_h = AzUpperPowerOfTwo(atlas->TexMinHeight);
    if (atlas->Builder == NULL || atlas->TexData == NULL
        || atlas->TexData->Status == AzTextureStatus_WantDestroy)
        return AzVec2i(min_w, min_h);

    AzFontAtlasBuilder* builder = atlas->Builder;
    min_w = AzMax(AzUpperPowerOfTwo(builder->MaxRectSize.x), min_w);
    min_h = AzMax(AzUpperPowerOfTwo(builder->MaxRectSize.y), min_h);
    const int surface_approx = builder->RectsPackedSurface
                               - builder->RectsDiscardedSurface;
    const int surface_sqrt = (int)sqrtf((float)surface_approx);

    int new_tex_w;
    int new_tex_h;
    if (min_w >= min_h)
    {
        new_tex_w = AzMax(min_w, AzUpperPowerOfTwo(surface_sqrt));
        new_tex_h = AzMax(min_h,
                          (int)((surface_approx + new_tex_w - 1) / new_tex_w));
        if ((atlas->Flags & AzFontAtlasFlags_NoPowerOfTwoHeight) == 0)
            new_tex_h = AzUpperPowerOfTwo(new_tex_h);
    } else
    {
        new_tex_h = AzMax(min_h, AzUpperPowerOfTwo(surface_sqrt));
        if ((atlas->Flags & AzFontAtlasFlags_NoPowerOfTwoHeight) == 0)
            new_tex_h = AzUpperPowerOfTwo(new_tex_h);
        new_tex_w = AzMax(min_w,
                          (int)((surface_approx + new_tex_h - 1) / new_tex_h));
    }

    AZ_ASSERT(AzIsPowerOfTwo(new_tex_w) && AzIsPowerOfTwo(new_tex_h));
    return AzVec2i(new_tex_w, new_tex_h);
}

void AzFontAtlasBuildClear(AzFontAtlas* atlas)
{
    AzVec2i new_tex_size = AzFontAtlasTextureGetSizeEstimate(atlas);
    AzFontAtlasBuildDestroy(atlas);
    AzFontAtlasTextureAdd(atlas, new_tex_size.x, new_tex_size.y);
    AzFontAtlasBuildInit(atlas);
    for (AzFontConfig& src : atlas->Sources)
        AzFontAtlasFontSourceInit(atlas, &src);
    for (AzFont* font : atlas->Fonts)
        for (AzFontConfig* src : font->Sources)
            AzFontAtlasFontSourceAddToFont(atlas, font, src);
}

void AzFontAtlasTextureCompact(AzFontAtlas* atlas)
{
    AzFontAtlasBuilder* builder = atlas->Builder;
    AzFontAtlasBuildDiscardBakes(atlas, 1);

    AzTextureData* old_tex = atlas->TexData;
    AzVec2i old_tex_size = AzVec2i(old_tex->Width, old_tex->Height);
    AzVec2i new_tex_size = AzFontAtlasTextureGetSizeEstimate(atlas);
    if (builder->RectsDiscardedCount == 0 && new_tex_size.x == old_tex_size.x
        && new_tex_size.y == old_tex_size.y)
        return;

    AzFontAtlasTextureRepack(atlas, new_tex_size.x, new_tex_size.y);
}

void AzFontAtlasBuildInit(AzFontAtlas* atlas)
{
    if (atlas->FontLoader == NULL)
    {
#ifdef AZ_ENABLE_FREETYPE
        AzFontAtlasBuildSetupFontLoader(atlas, AzFreeType::GetFontLoader());
#elif defined(AZ_ENABLE_STB_TRUETYPE)
        AzFontAtlasBuildSetupFontLoader(
                atlas,
                AzFontAtlasGetFontLoaderForStbTruetype());
#else
        AZ_ASSERT(0);
#endif
    }

    if (atlas->TexData == NULL || atlas->TexData->Pixels == NULL)
        AzFontAtlasTextureAdd(atlas,
                              AzUpperPowerOfTwo(atlas->TexMinWidth),
                              AzUpperPowerOfTwo(atlas->TexMinHeight));

    atlas->Builder = AZ_NEW(AzFontAtlasBuilder)();
    if (atlas->FontLoader->LoaderInit)
        atlas->FontLoader->LoaderInit(atlas);

    AzFontAtlasBuildUpdateRendererHasTexturesFromContext(atlas);

    AzFontAtlasPackInit(atlas);

    AzFontAtlasBuildUpdateLinesTexData(atlas);
    AzFontAtlasBuildUpdateBasicTexData(atlas);

    AzFontAtlasBuildUpdatePointers(atlas);

    AzFontAtlasUpdateDrawListsSharedData(atlas);
}

void AzFontAtlasBuildDestroy(AzFontAtlas* atlas)
{
    for (AzFont* font : atlas->Fonts)
        AzFontAtlasFontDestroyOutput(atlas, font);
    if (atlas->Builder && atlas->FontLoader
        && atlas->FontLoader->LoaderShutdown)
    {
        atlas->FontLoader->LoaderShutdown(atlas);
        AZ_ASSERT(atlas->FontLoaderData == NULL);
    }
    AZ_DELETE(atlas->Builder);
    atlas->Builder = NULL;
}

void AzFontAtlasPackInit(AzFontAtlas* atlas)
{
    AzTextureData* tex = atlas->TexData;
    AzFontAtlasBuilder* builder = atlas->Builder;

    const int pack_node_count = tex->Width / 2;
    builder->PackNodes.resize(pack_node_count);
    AZ_STATIC_ASSERT(sizeof(stbrp_context) <= sizeof(stbrp_context_opaque));
    stbrp_init_target((stbrp_context*)(void*)&builder->PackContext,
                      tex->Width,
                      tex->Height,
                      builder->PackNodes.Data,
                      builder->PackNodes.Size);
    builder->RectsPackedSurface = builder->RectsPackedCount = 0;
    builder->MaxRectSize = AzVec2i(0, 0);
    builder->MaxRectBounds = AzVec2i(0, 0);
}

static AzFontAtlasRectId AzFontAtlasPackAllocRectEntry(AzFontAtlas* atlas,
                                                       int rect_idx)
{
    AzFontAtlasBuilder* builder = (AzFontAtlasBuilder*)atlas->Builder;
    int index_idx;
    AzFontAtlasRectEntry* index_entry;
    if (builder->RectsIndexFreeListStart < 0)
    {
        builder->RectsIndex.resize(builder->RectsIndex.Size + 1);
        index_idx = builder->RectsIndex.Size - 1;
        index_entry = &builder->RectsIndex[index_idx];
        memset(index_entry, 0, sizeof(*index_entry));
    } else
    {
        index_idx = builder->RectsIndexFreeListStart;
        index_entry = &builder->RectsIndex[index_idx];
        AZ_ASSERT(index_entry->IsUsed == false && index_entry->Generation > 0);
        builder->RectsIndexFreeListStart = index_entry->TargetIndex;
    }
    index_entry->TargetIndex = rect_idx;
    index_entry->IsUsed = 1;
    return AzFontAtlasRectId_Make(index_idx, index_entry->Generation);
}

static AzFontAtlasRectId AzFontAtlasPackReuseRectEntry(
        AzFontAtlas* atlas,
        AzFontAtlasRectEntry* index_entry)
{
    AZ_ASSERT(index_entry->IsUsed);
    index_entry->TargetIndex = atlas->Builder->Rects.Size - 1;
    int index_idx = atlas->Builder->RectsIndex.index_from_ptr(index_entry);
    return AzFontAtlasRectId_Make(index_idx, index_entry->Generation);
}

void AzFontAtlasPackDiscardRect(AzFontAtlas* atlas, AzFontAtlasRectId id)
{
    AZ_ASSERT(id != AzFontAtlasRectId_Invalid);

    AzTextureRect* rect = AzFontAtlasPackGetRect(atlas, id);
    if (rect == NULL)
        return;

    AzFontAtlasBuilder* builder = atlas->Builder;
    int index_idx = AzFontAtlasRectId_GetIndex(id);
    AzFontAtlasRectEntry* index_entry = &builder->RectsIndex[index_idx];
    AZ_ASSERT(index_entry->IsUsed && index_entry->TargetIndex >= 0);
    index_entry->IsUsed = false;
    index_entry->TargetIndex = builder->RectsIndexFreeListStart;
    index_entry->Generation++;

    const int pack_padding = atlas->TexGlyphPadding;
    builder->RectsIndexFreeListStart = index_idx;
    builder->RectsDiscardedCount++;
    builder->RectsDiscardedSurface += (rect->w + pack_padding)
                                      * (rect->h + pack_padding);
    rect->w = rect->h = 0;
}

AzFontAtlasRectId AzFontAtlasPackAddRect(AzFontAtlas* atlas,
                                         int w,
                                         int h,
                                         AzFontAtlasRectEntry* overwrite_entry)
{
    AZ_ASSERT(w > 0 && w <= 0xFFFF);
    AZ_ASSERT(h > 0 && h <= 0xFFFF);

    AzFontAtlasBuilder* builder = (AzFontAtlasBuilder*)atlas->Builder;
    const int pack_padding = atlas->TexGlyphPadding;
    builder->MaxRectSize.x = AzMax(builder->MaxRectSize.x, w);
    builder->MaxRectSize.y = AzMax(builder->MaxRectSize.y, h);

    AzTextureRect r = {0, 0, (unsigned short)w, (unsigned short)h};
    for (int attempts_remaining = 3; attempts_remaining >= 0;
         attempts_remaining--)
    {
        stbrp_rect pack_r = {};
        pack_r.w = w + pack_padding;
        pack_r.h = h + pack_padding;
        stbrp_pack_rects((stbrp_context*)(void*)&builder->PackContext,
                         &pack_r,
                         1);
        r.x = (unsigned short)pack_r.x;
        r.y = (unsigned short)pack_r.y;
        if (pack_r.was_packed)
            break;

        if (attempts_remaining == 0 || builder->LockDisableResize)
        {
            AZ_DEBUG_LOG_FONT(
                    "[font] Failed packing %dx%d rectangle. Returning "
                    "fallback.\n",
                    w,
                    h);
            return AzFontAtlasRectId_Invalid;
        }

        AzFontAtlasTextureMakeSpace(atlas);
    }

    builder->MaxRectBounds.x = AzMax(builder->MaxRectBounds.x,
                                     r.x + r.w + pack_padding);
    builder->MaxRectBounds.y = AzMax(builder->MaxRectBounds.y,
                                     r.y + r.h + pack_padding);
    builder->RectsPackedCount++;
    builder->RectsPackedSurface += (w + pack_padding) * (h + pack_padding);

    builder->Rects.push_back(r);
    if (overwrite_entry != NULL)
        return AzFontAtlasPackReuseRectEntry(atlas, overwrite_entry);
    else
        return AzFontAtlasPackAllocRectEntry(atlas, builder->Rects.Size - 1);
}

AzTextureRect* AzFontAtlasPackGetRect(AzFontAtlas* atlas, AzFontAtlasRectId id)
{
    AZ_ASSERT(id != AzFontAtlasRectId_Invalid);
    int index_idx = AzFontAtlasRectId_GetIndex(id);
    AzFontAtlasBuilder* builder = (AzFontAtlasBuilder*)atlas->Builder;
    AzFontAtlasRectEntry* index_entry = &builder->RectsIndex[index_idx];
    AZ_ASSERT(index_entry->Generation == AzFontAtlasRectId_GetGeneration(id));
    AZ_ASSERT(index_entry->IsUsed);
    return &builder->Rects[index_entry->TargetIndex];
}

AzTextureRect* AzFontAtlasPackGetRectSafe(AzFontAtlas* atlas,
                                          AzFontAtlasRectId id)
{
    if (id == AzFontAtlasRectId_Invalid)
        return NULL;
    int index_idx = AzFontAtlasRectId_GetIndex(id);
    if (atlas->Builder == NULL)
        AzFontAtlasBuildInit(atlas);
    AzFontAtlasBuilder* builder = (AzFontAtlasBuilder*)atlas->Builder;
    if (index_idx >= builder->RectsIndex.Size)
        return NULL;
    AzFontAtlasRectEntry* index_entry = &builder->RectsIndex[index_idx];
    if (index_entry->Generation != AzFontAtlasRectId_GetGeneration(id)
        || !index_entry->IsUsed)
        return NULL;
    return &builder->Rects[index_entry->TargetIndex];
}

static bool AzFontAtlasBuildAcceptCodepointForSource(AzFontConfig* src,
                                                     AzWchar codepoint)
{
    if (const AzWchar* exclude_list = src->GlyphExcludeRanges)
        for (; exclude_list[0] != 0; exclude_list += 2)
            if (codepoint >= exclude_list[0] && codepoint <= exclude_list[1])
                return false;
    return true;
}

static void AzFontBaked_BuildGrowIndex(AzFontBaked* baked, int new_size)
{
    AZ_ASSERT(baked->IndexAdvanceX.Size == baked->IndexLookup.Size);
    if (new_size <= baked->IndexLookup.Size)
        return;
    baked->IndexAdvanceX.resize(new_size, -1.0f);
    baked->IndexLookup.resize(new_size, AZ_FONTGLYPH_INDEX_UNUSED);
}

static void AzFontAtlas_FontHookRemapCodepoint(AzFontAtlas* atlas,
                                               AzFont* font,
                                               AzWchar* c)
{
    AZ_UNUSED(atlas);
    if (font->RemapPairs.Data.Size != 0)
        *c = (AzWchar)font->RemapPairs.GetInt((AzID)*c, (int)*c);
}

static AzFontGlyph* AzFontBaked_BuildLoadGlyph(AzFontBaked* baked,
                                               AzWchar codepoint)
{
    AzFont* font = baked->ContainerFont;
    AzFontAtlas* atlas = font->ContainerAtlas;
    if (atlas->Locked || (font->Flags & AzFontFlags_NoLoadGlyphs))
    {
        if (baked->FallbackGlyphIndex == -1 && baked->LockLoadingFallback == 0)
            AzFontAtlasBuildSetupFontBakedFallback(baked);
        return NULL;
    }

    AzWchar src_codepoint = codepoint;
    AzFontAtlas_FontHookRemapCodepoint(atlas, font, &codepoint);

    if (codepoint == font->EllipsisChar && font->EllipsisAutoBake)
        if (AzFontGlyph* glyph = AzFontAtlasBuildSetupFontBakedEllipsis(atlas,
                                                                        baked))
            return glyph;

    char* loader_user_data_p = (char*)baked->FontLoaderDatas;
    int src_n = 0;
    for (AzFontConfig* src : font->Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        if (!src->GlyphExcludeRanges
            || AzFontAtlasBuildAcceptCodepointForSource(src, codepoint))
        {
            AzFontGlyph glyph_buf;
            if (loader->FontBakedLoadGlyph(atlas,
                                           src,
                                           baked,
                                           loader_user_data_p,
                                           codepoint,
                                           &glyph_buf))
            {
                glyph_buf.Codepoint = src_codepoint;
                glyph_buf.SourceIdx = src_n;
                return AzFontAtlasBakedAddFontGlyph(atlas,
                                                    baked,
                                                    src,
                                                    &glyph_buf);
            }
        }
        loader_user_data_p += loader->FontBakedSrcLoaderDataSize;
        src_n++;
    }

    if (baked->LockLoadingFallback)
        return NULL;
    if (baked->FallbackGlyphIndex == -1)
        AzFontAtlasBuildSetupFontBakedFallback(baked);

    AzFontBaked_BuildGrowIndex(baked, codepoint + 1);
    baked->IndexAdvanceX[codepoint] = baked->FallbackAdvanceX;
    baked->IndexLookup[codepoint] = AZ_FONTGLYPH_INDEX_NOT_FOUND;
    return NULL;
}

AZ_MSVC_RUNTIME_CHECKS_OFF
static float BuildLoadGlyphGetAdvanceOrFallback(AzFontBaked* baked,
                                                unsigned int codepoint)
{
    AzFontGlyph* glyph = AzFontBaked_BuildLoadGlyph(baked, (AzWchar)codepoint);
    return glyph ? glyph->AdvanceX : baked->FallbackAdvanceX;
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

#ifndef AZ_DISABLE_DEBUG_TOOLS
void AzFontAtlasDebugLogTextureRequests(AzFontAtlas* atlas)
{
    AzContext& g = *GAz;
    AZ_UNUSED(g);
    for (AzTextureData* tex : atlas->TexList)
    {
        if ((g.IO.BackendFlags & AzBackendFlags_RendererHasTextures) == 0)
            AZ_ASSERT(tex->Updates.Size == 0);
        if (tex->Status == AzTextureStatus_WantCreate)
            AZ_DEBUG_LOG_FONT("[font] Texture #%03d: create %dx%d\n",
                              tex->UniqueID,
                              tex->Width,
                              tex->Height);
        else if (tex->Status == AzTextureStatus_WantDestroy)
            AZ_DEBUG_LOG_FONT(
                    "[font] Texture #%03d: destroy %dx%d, texid=0x%" AZ_PRIX64
                    ", backend_data=%p\n",
                    tex->UniqueID,
                    tex->Width,
                    tex->Height,
                    tex->TexID,
                    tex->BackendUserData);
        else if (tex->Status == AzTextureStatus_WantUpdates)
        {
            AZ_DEBUG_LOG_FONT(
                    "[font] Texture #%03d: update %d regions, "
                    "texid=0x%" AZ_PRIX64 ", backend_data=0x%" AZ_PRIX64 "\n",
                    tex->UniqueID,
                    tex->Updates.Size,
                    tex->TexID,
                    (AzU64)(intptr_t)tex->BackendUserData);
            for (const AzTextureRect& r : tex->Updates)
            {
                AZ_UNUSED(r);
                AZ_ASSERT(r.x >= 0 && r.y >= 0);
                AZ_ASSERT(r.x + r.w <= tex->Width && r.y + r.h <= tex->Height);
            }
        }
    }
}
#endif

#ifdef AZ_ENABLE_STB_TRUETYPE

struct Az_AzplStbTrueType_FontSrcData
{
    stbtt_fontinfo FontInfo;
    float ScaleFactor;
};

static bool Az_AzplStbTrueType_FontSrcInit(AzFontAtlas* atlas,
                                           AzFontConfig* src)
{
    AZ_UNUSED(atlas);

    Az_AzplStbTrueType_FontSrcData* bd_font_data = AZ_NEW(
            Az_AzplStbTrueType_FontSrcData);
    AZ_ASSERT(src->FontLoaderData == NULL);

    const int font_offset =
            stbtt_GetFontOffsetForIndex((unsigned char*)src->FontData,
                                        src->FontNo);
    if (font_offset < 0)
    {
        AZ_DELETE(bd_font_data);
        AZ_ASSERT_USER_ERROR(0,
                             "stbtt_GetFontOffsetForIndex(): FontData is "
                             "incorrect, or FontNo cannot be found.");
        return false;
    }
    if (!stbtt_InitFont(&bd_font_data->FontInfo,
                        (unsigned char*)src->FontData,
                        font_offset))
    {
        AZ_DELETE(bd_font_data);
        AZ_ASSERT_USER_ERROR(0,
                             "stbtt_InitFont(): failed to parse FontData. It "
                             "is correct and complete? Check FontDataSize.");
        return false;
    }
    src->FontLoaderData = bd_font_data;

    if (src->MergeMode && src->SizePixels == 0.0f)
        src->SizePixels = src->DstFont->Sources[0]->SizePixels;

    if (src->SizePixels >= 0.0f)
        bd_font_data->ScaleFactor =
                stbtt_ScaleForPixelHeight(&bd_font_data->FontInfo, 1.0f);
    else
        bd_font_data->ScaleFactor =
                stbtt_ScaleForMappingEmToPixels(&bd_font_data->FontInfo, 1.0f);
    if (src->MergeMode && src->SizePixels != 0.0f)
        bd_font_data->ScaleFactor *= src->SizePixels
                                     / src->DstFont->Sources[0]->SizePixels;

    return true;
}

static void Az_AzplStbTrueType_FontSrcDestroy(AzFontAtlas* atlas,
                                              AzFontConfig* src)
{
    AZ_UNUSED(atlas);
    Az_AzplStbTrueType_FontSrcData* bd_font_data =
            (Az_AzplStbTrueType_FontSrcData*)src->FontLoaderData;
    AZ_DELETE(bd_font_data);
    src->FontLoaderData = NULL;
}

static bool Az_AzplStbTrueType_FontSrcContainsGlyph(AzFontAtlas* atlas,
                                                    AzFontConfig* src,
                                                    AzWchar codepoint)
{
    AZ_UNUSED(atlas);

    Az_AzplStbTrueType_FontSrcData* bd_font_data =
            (Az_AzplStbTrueType_FontSrcData*)src->FontLoaderData;
    AZ_ASSERT(bd_font_data != NULL);

    int glyph_index = stbtt_FindGlyphIndex(&bd_font_data->FontInfo,
                                           (int)codepoint);
    return glyph_index != 0;
}

static bool Az_AzplStbTrueType_FontBakedInit(AzFontAtlas* atlas,
                                             AzFontConfig* src,
                                             AzFontBaked* baked,
                                             void*)
{
    AZ_UNUSED(atlas);

    Az_AzplStbTrueType_FontSrcData* bd_font_data =
            (Az_AzplStbTrueType_FontSrcData*)src->FontLoaderData;
    if (src->MergeMode == false)
    {
        float scale_for_layout = bd_font_data->ScaleFactor * baked->Size;
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&bd_font_data->FontInfo,
                              &unscaled_ascent,
                              &unscaled_descent,
                              &unscaled_line_gap);
        baked->Ascent = AzCeil(unscaled_ascent * scale_for_layout);
        baked->Descent = AzFloor(unscaled_descent * scale_for_layout);
    }
    return true;
}

static bool Az_AzplStbTrueType_FontBakedLoadGlyph(AzFontAtlas* atlas,
                                                  AzFontConfig* src,
                                                  AzFontBaked* baked,
                                                  void*,
                                                  AzWchar codepoint,
                                                  AzFontGlyph* out_glyph)
{
    Az_AzplStbTrueType_FontSrcData* bd_font_data =
            (Az_AzplStbTrueType_FontSrcData*)src->FontLoaderData;
    AZ_ASSERT(bd_font_data);
    int glyph_index = stbtt_FindGlyphIndex(&bd_font_data->FontInfo,
                                           (int)codepoint);
    if (glyph_index == 0)
        return false;

    int oversample_h, oversample_v;
    AzFontAtlasBuildGetOversampleFactors(src,
                                         baked,
                                         &oversample_h,
                                         &oversample_v);
    const float scale_for_layout = bd_font_data->ScaleFactor * baked->Size;
    const float rasterizer_density = src->RasterizerDensity
                                     * baked->RasterizerDensity;
    const float scale_for_raster_x = bd_font_data->ScaleFactor * baked->Size
                                     * rasterizer_density * oversample_h;
    const float scale_for_raster_y = bd_font_data->ScaleFactor * baked->Size
                                     * rasterizer_density * oversample_v;

    int x0, y0, x1, y1;
    int advance, lsb;
    stbtt_GetGlyphBitmapBoxSubpixel(&bd_font_data->FontInfo,
                                    glyph_index,
                                    scale_for_raster_x,
                                    scale_for_raster_y,
                                    0,
                                    0,
                                    &x0,
                                    &y0,
                                    &x1,
                                    &y1);
    stbtt_GetGlyphHMetrics(&bd_font_data->FontInfo,
                           glyph_index,
                           &advance,
                           &lsb);
    const bool is_visible = (x0 != x1 && y0 != y1);

    out_glyph->Codepoint = codepoint;
    out_glyph->AdvanceX = advance * scale_for_layout;

    if (is_visible)
    {
        const int w = (x1 - x0 + oversample_h - 1);
        const int h = (y1 - y0 + oversample_v - 1);
        AzFontAtlasRectId pack_id = AzFontAtlasPackAddRect(atlas, w, h);
        if (pack_id == AzFontAtlasRectId_Invalid)
        {
            AZ_ASSERT(pack_id != AzFontAtlasRectId_Invalid
                      && "Out of texture memory.");
            return false;
        }
        AzTextureRect* r = AzFontAtlasPackGetRect(atlas, pack_id);

        stbtt_GetGlyphBitmapBox(&bd_font_data->FontInfo,
                                glyph_index,
                                scale_for_raster_x,
                                scale_for_raster_y,
                                &x0,
                                &y0,
                                &x1,
                                &y1);
        AzFontAtlasBuilder* builder = atlas->Builder;
        builder->TempBuffer.resize(w * h * 1);
        unsigned char* bitmap_pixels = builder->TempBuffer.Data;
        memset(bitmap_pixels, 0, w * h * 1);
        stbtt_MakeGlyphBitmapSubpixel(&bd_font_data->FontInfo,
                                      bitmap_pixels,
                                      r->w - oversample_h + 1,
                                      r->h - oversample_v + 1,
                                      w,
                                      scale_for_raster_x,
                                      scale_for_raster_y,
                                      0,
                                      0,
                                      glyph_index);

        if (oversample_h > 1)
            stbtt__h_prefilter(bitmap_pixels, r->w, r->h, r->w, oversample_h);
        if (oversample_v > 1)
            stbtt__v_prefilter(bitmap_pixels, r->w, r->h, r->w, oversample_v);

        const float ref_size = baked->ContainerFont->Sources[0]->SizePixels;
        const float offsets_scale = (ref_size != 0.0f)
                                            ? (baked->Size / ref_size)
                                            : 1.0f;
        float font_off_x = (src->GlyphOffset.x * offsets_scale);
        float font_off_y = (src->GlyphOffset.y * offsets_scale);
        if (src->PixelSnapH)
            font_off_x = AZ_ROUND(font_off_x);
        if (src->PixelSnapV)
            font_off_y = AZ_ROUND(font_off_y);
        font_off_x += stbtt__oversample_shift(oversample_h);
        font_off_y += stbtt__oversample_shift(oversample_v)
                      + AZ_ROUND(baked->Ascent);
        float recip_h = 1.0f / (oversample_h * rasterizer_density);
        float recip_v = 1.0f / (oversample_v * rasterizer_density);

        out_glyph->X0 = x0 * recip_h + font_off_x;
        out_glyph->Y0 = y0 * recip_v + font_off_y;
        out_glyph->X1 = (x0 + (int)r->w) * recip_h + font_off_x;
        out_glyph->Y1 = (y0 + (int)r->h) * recip_v + font_off_y;
        out_glyph->Visible = true;
        out_glyph->PackId = pack_id;
        AzFontAtlasBakedSetFontGlyphBitmap(atlas,
                                           baked,
                                           src,
                                           out_glyph,
                                           r,
                                           bitmap_pixels,
                                           AzTextureFormat_Alpha8,
                                           w);
    }

    return true;
}

const AzFontLoader* AzFontAtlasGetFontLoaderForStbTruetype()
{
    static AzFontLoader loader;
    loader.Name = "stb_truetype";
    loader.FontSrcInit = Az_AzplStbTrueType_FontSrcInit;
    loader.FontSrcDestroy = Az_AzplStbTrueType_FontSrcDestroy;
    loader.FontSrcContainsGlyph = Az_AzplStbTrueType_FontSrcContainsGlyph;
    loader.FontBakedInit = Az_AzplStbTrueType_FontBakedInit;
    loader.FontBakedDestroy = NULL;
    loader.FontBakedLoadGlyph = Az_AzplStbTrueType_FontBakedLoadGlyph;
    return &loader;
}

#endif

const AzWchar* AzFontAtlas::GetGlyphRangesDefault()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0,
    };
    return &ranges[0];
}

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
const AzWchar* AzFontAtlas::GetGlyphRangesGreek()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0x0370,
            0x03FF,
            0,
    };
    return &ranges[0];
}

const AzWchar* AzFontAtlas::GetGlyphRangesKorean()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0x3131,
            0x3163,
            0xAC00,
            0xD7A3,
            0xFFFD,
            0xFFFD,
            0,
    };
    return &ranges[0];
}

const AzWchar* AzFontAtlas::GetGlyphRangesChineseFull()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0x2000,
            0x206F,
            0x3000,
            0x30FF,
            0x31F0,
            0x31FF,
            0xFF00,
            0xFFEF,
            0xFFFD,
            0xFFFD,
            0x4e00,
            0x9FAF,
            0,
    };
    return &ranges[0];
}

static void UnpackAccumulativeOffsetsIntoRanges(
        int base_codepoint,
        const short* accumulative_offsets,
        int accumulative_offsets_count,
        AzWchar* out_ranges)
{
    for (int n = 0; n < accumulative_offsets_count; n++, out_ranges += 2)
    {
        out_ranges[0] = out_ranges[1] = (AzWchar)(base_codepoint
                                                  + accumulative_offsets[n]);
        base_codepoint += accumulative_offsets[n];
    }
    out_ranges[0] = 0;
}

const AzWchar* AzFontAtlas::GetGlyphRangesChineseSimplifiedCommon()
{
    static const short accumulative_offsets_from_0x4E00[] = {
            0,   1,   2,   4,   1,   1,   1,   1,  2,  1,   3,   2,   1,   2,
            2,   1,   1,   1,   1,   1,   5,   2,  1,  2,   3,   3,   3,   2,
            2,   4,   1,   1,   1,   2,   1,   5,  2,  3,   1,   2,   1,   2,
            1,   1,   2,   1,   1,   2,   2,   1,  4,  1,   1,   1,   1,   5,
            10,  1,   2,   19,  2,   1,   2,   1,  2,  1,   2,   1,   2,   1,
            5,   1,   6,   3,   2,   1,   2,   2,  1,  1,   1,   4,   8,   5,
            1,   1,   4,   1,   1,   3,   1,   2,  1,  5,   1,   2,   1,   1,
            1,   10,  1,   1,   5,   2,   4,   6,  1,  4,   2,   2,   2,   12,
            2,   1,   1,   6,   1,   1,   1,   4,  1,  1,   4,   6,   5,   1,
            4,   2,   2,   4,   10,  7,   1,   1,  4,  2,   4,   2,   1,   4,
            3,   6,   10,  12,  5,   7,   2,   14, 2,  9,   1,   1,   6,   7,
            10,  4,   7,   13,  1,   5,   4,   8,  4,  1,   1,   2,   28,  5,
            6,   1,   1,   5,   2,   5,   20,  2,  2,  9,   8,   11,  2,   9,
            17,  1,   8,   6,   8,   27,  4,   6,  9,  20,  11,  27,  6,   68,
            2,   2,   1,   1,   1,   2,   1,   2,  2,  7,   6,   11,  3,   3,
            1,   1,   3,   1,   2,   1,   1,   1,  1,  1,   3,   1,   1,   8,
            3,   4,   1,   5,   7,   2,   1,   4,  4,  8,   4,   2,   1,   2,
            1,   1,   4,   5,   6,   3,   6,   2,  12, 3,   1,   3,   9,   2,
            4,   3,   4,   1,   5,   3,   3,   1,  3,  7,   1,   5,   1,   1,
            1,   1,   2,   3,   4,   5,   2,   3,  2,  6,   1,   1,   2,   1,
            7,   1,   7,   3,   4,   5,   15,  2,  2,  1,   5,   3,   22,  19,
            2,   1,   1,   1,   1,   2,   5,   1,  1,  1,   6,   1,   1,   12,
            8,   2,   9,   18,  22,  4,   1,   1,  5,  1,   16,  1,   2,   7,
            10,  15,  1,   1,   6,   2,   4,   1,  2,  4,   1,   6,   1,   1,
            3,   2,   4,   1,   6,   4,   5,   1,  2,  1,   1,   2,   1,   10,
            3,   1,   3,   2,   1,   9,   3,   2,  5,  7,   2,   19,  4,   3,
            6,   1,   1,   1,   1,   1,   4,   3,  2,  1,   1,   1,   2,   5,
            3,   1,   1,   1,   2,   2,   1,   1,  2,  1,   1,   2,   1,   3,
            1,   1,   1,   3,   7,   1,   4,   1,  1,  2,   1,   1,   2,   1,
            2,   4,   4,   3,   8,   1,   1,   1,  2,  1,   3,   5,   1,   3,
            1,   3,   4,   6,   2,   2,   14,  4,  6,  6,   11,  9,   1,   15,
            3,   1,   28,  5,   2,   5,   5,   3,  1,  3,   4,   5,   4,   6,
            14,  3,   2,   3,   5,   21,  2,   7,  20, 10,  1,   2,   19,  2,
            4,   28,  28,  2,   3,   2,   1,   14, 4,  1,   26,  28,  42,  12,
            40,  3,   52,  79,  5,   14,  17,  3,  2,  2,   11,  3,   4,   6,
            3,   1,   8,   2,   23,  4,   5,   8,  10, 4,   2,   7,   3,   5,
            1,   1,   6,   3,   1,   2,   2,   2,  5,  28,  1,   1,   7,   7,
            20,  5,   3,   29,  3,   17,  26,  1,  8,  4,   27,  3,   6,   11,
            23,  5,   3,   4,   6,   13,  24,  16, 6,  5,   10,  25,  35,  7,
            3,   2,   3,   3,   14,  3,   6,   2,  6,  1,   4,   2,   3,   8,
            2,   1,   1,   3,   3,   3,   4,   1,  1,  13,  2,   2,   4,   5,
            2,   1,   14,  14,  1,   2,   2,   1,  4,  5,   2,   3,   1,   14,
            3,   12,  3,   17,  2,   16,  5,   1,  2,  1,   8,   9,   3,   19,
            4,   2,   2,   4,   17,  25,  21,  20, 28, 75,  1,   10,  29,  103,
            4,   1,   2,   1,   1,   4,   2,   4,  1,  2,   3,   24,  2,   2,
            2,   1,   1,   2,   1,   3,   8,   1,  1,  1,   2,   1,   1,   3,
            1,   1,   1,   6,   1,   5,   3,   1,  1,  1,   3,   4,   1,   1,
            5,   2,   1,   5,   6,   13,  9,   16, 1,  1,   1,   1,   3,   2,
            3,   2,   4,   5,   2,   5,   2,   2,  3,  7,   13,  7,   2,   2,
            1,   1,   1,   1,   2,   3,   3,   2,  1,  6,   4,   9,   2,   1,
            14,  2,   14,  2,   1,   18,  3,   4,  14, 4,   11,  41,  15,  23,
            15,  23,  176, 1,   3,   4,   1,   1,  1,  1,   5,   3,   1,   2,
            3,   7,   3,   1,   1,   2,   1,   2,  4,  4,   6,   2,   4,   1,
            9,   7,   1,   10,  5,   8,   16,  29, 1,  1,   2,   2,   3,   1,
            3,   5,   2,   4,   5,   4,   1,   1,  2,  2,   3,   3,   7,   1,
            6,   10,  1,   17,  1,   44,  4,   6,  2,  1,   1,   6,   5,   4,
            2,   10,  1,   6,   9,   2,   8,   1,  24, 1,   2,   13,  7,   8,
            8,   2,   1,   4,   1,   3,   1,   3,  3,  5,   2,   5,   10,  9,
            4,   9,   12,  2,   1,   6,   1,   10, 1,  1,   7,   7,   4,   10,
            8,   3,   1,   13,  4,   3,   1,   6,  1,  3,   5,   2,   1,   2,
            17,  16,  5,   2,   16,  6,   1,   4,  2,  1,   3,   3,   6,   8,
            5,   11,  11,  1,   3,   3,   2,   4,  6,  10,  9,   5,   7,   4,
            7,   4,   7,   1,   1,   4,   2,   1,  3,  6,   8,   7,   1,   6,
            11,  5,   5,   3,   24,  9,   4,   2,  7,  13,  5,   1,   8,   82,
            16,  61,  1,   1,   1,   4,   2,   2,  16, 10,  3,   8,   1,   1,
            6,   4,   2,   1,   3,   1,   1,   1,  4,  3,   8,   4,   2,   2,
            1,   1,   1,   1,   1,   6,   3,   5,  1,  1,   4,   6,   9,   2,
            1,   1,   1,   2,   1,   7,   2,   1,  6,  1,   5,   4,   4,   3,
            1,   8,   1,   3,   3,   1,   3,   2,  2,  2,   2,   3,   1,   6,
            1,   2,   1,   2,   1,   3,   7,   1,  8,  2,   1,   2,   1,   5,
            2,   5,   3,   5,   10,  1,   2,   1,  1,  3,   2,   5,   11,  3,
            9,   3,   5,   1,   1,   5,   9,   1,  2,  1,   5,   7,   9,   9,
            8,   1,   3,   3,   3,   6,   8,   2,  3,  2,   1,   1,   32,  6,
            1,   2,   15,  9,   3,   7,   13,  1,  3,  10,  13,  2,   14,  1,
            13,  10,  2,   1,   3,   10,  4,   15, 2,  15,  15,  10,  1,   3,
            9,   6,   9,   32,  25,  26,  47,  7,  3,  2,   3,   1,   6,   3,
            4,   3,   2,   8,   5,   4,   1,   9,  4,  2,   2,   19,  10,  6,
            2,   3,   8,   1,   2,   2,   4,   2,  1,  9,   4,   4,   4,   6,
            4,   8,   9,   2,   3,   1,   1,   1,  1,  3,   5,   5,   1,   3,
            8,   4,   6,   2,   1,   4,   12,  1,  5,  3,   7,   13,  2,   5,
            8,   1,   6,   1,   2,   5,   14,  6,  1,  5,   2,   4,   8,   15,
            5,   1,   23,  6,   62,  2,   10,  1,  1,  8,   1,   2,   2,   10,
            4,   2,   2,   9,   2,   1,   1,   3,  2,  3,   1,   5,   3,   3,
            2,   1,   3,   8,   1,   1,   1,   11, 3,  1,   1,   4,   3,   7,
            1,   14,  1,   2,   3,   12,  5,   2,  5,  1,   6,   7,   5,   7,
            14,  11,  1,   3,   1,   8,   9,   12, 2,  1,   11,  8,   4,   4,
            2,   6,   10,  9,   13,  1,   1,   3,  1,  5,   1,   3,   2,   4,
            4,   1,   18,  2,   3,   14,  11,  4,  29, 4,   2,   7,   1,   3,
            13,  9,   2,   2,   5,   3,   5,   20, 7,  16,  8,   5,   72,  34,
            6,   4,   22,  12,  12,  28,  45,  36, 9,  7,   39,  9,   191, 1,
            1,   1,   4,   11,  8,   4,   9,   2,  3,  22,  1,   1,   1,   1,
            4,   17,  1,   7,   7,   1,   11,  31, 10, 2,   4,   8,   2,   3,
            2,   1,   4,   2,   16,  4,   32,  2,  3,  19,  13,  4,   9,   1,
            5,   2,   14,  8,   1,   1,   3,   6,  19, 6,   5,   1,   16,  6,
            2,   10,  8,   5,   1,   2,   3,   1,  5,  5,   1,   11,  6,   6,
            1,   3,   3,   2,   6,   3,   8,   1,  1,  4,   10,  7,   5,   7,
            7,   5,   8,   9,   2,   1,   3,   4,  1,  1,   3,   1,   3,   3,
            2,   6,   16,  1,   4,   6,   3,   1,  10, 6,   1,   3,   15,  2,
            9,   2,   10,  25,  13,  9,   16,  6,  2,  2,   10,  11,  4,   3,
            9,   1,   2,   6,   6,   5,   4,   30, 40, 1,   10,  7,   12,  14,
            33,  6,   3,   6,   7,   3,   1,   3,  1,  11,  14,  4,   9,   5,
            12,  11,  49,  18,  51,  31,  140, 31, 2,  2,   1,   5,   1,   8,
            1,   10,  1,   4,   4,   3,   24,  1,  10, 1,   3,   6,   6,   16,
            3,   4,   5,   2,   1,   4,   2,   57, 10, 6,   22,  2,   22,  3,
            7,   22,  6,   10,  11,  36,  18,  16, 33, 36,  2,   5,   5,   1,
            1,   1,   4,   10,  1,   4,   13,  2,  7,  5,   2,   9,   3,   4,
            1,   7,   43,  3,   7,   3,   9,   14, 7,  9,   1,   11,  1,   1,
            3,   7,   4,   18,  13,  1,   14,  1,  3,  6,   10,  73,  2,   2,
            30,  6,   1,   11,  18,  19,  13,  22, 3,  46,  42,  37,  89,  7,
            3,   16,  34,  2,   2,   3,   9,   1,  7,  1,   1,   1,   2,   2,
            4,   10,  7,   3,   10,  3,   9,   5,  28, 9,   2,   6,   13,  7,
            3,   1,   3,   10,  2,   7,   2,   11, 3,  6,   21,  54,  85,  2,
            1,   4,   2,   2,   1,   39,  3,   21, 2,  2,   5,   1,   1,   1,
            4,   1,   1,   3,   4,   15,  1,   3,  2,  4,   4,   2,   3,   8,
            2,   20,  1,   8,   7,   13,  4,   1,  26, 6,   2,   9,   34,  4,
            21,  52,  10,  4,   4,   1,   5,   12, 2,  11,  1,   7,   2,   30,
            12,  44,  2,   30,  1,   1,   3,   6,  16, 9,   17,  39,  82,  2,
            2,   24,  7,   1,   7,   3,   16,  9,  14, 44,  2,   1,   2,   1,
            2,   3,   5,   2,   4,   1,   6,   7,  5,  3,   2,   6,   1,   11,
            5,   11,  2,   1,   18,  19,  8,   1,  3,  24,  29,  2,   1,   3,
            5,   2,   2,   1,   13,  6,   5,   1,  46, 11,  3,   5,   1,   1,
            5,   8,   2,   10,  6,   12,  6,   3,  7,  11,  2,   4,   16,  13,
            2,   5,   1,   1,   2,   2,   5,   2,  28, 5,   2,   23,  10,  8,
            4,   4,   22,  39,  95,  38,  8,   14, 9,  5,   1,   13,  5,   4,
            3,   13,  12,  11,  1,   9,   1,   27, 37, 2,   5,   4,   4,   63,
            211, 95,  2,   2,   2,   1,   3,   5,  2,  1,   1,   2,   2,   1,
            1,   1,   3,   2,   4,   1,   2,   1,  1,  5,   2,   2,   1,   1,
            2,   3,   1,   3,   1,   1,   1,   3,  1,  4,   2,   1,   3,   6,
            1,   1,   3,   7,   15,  5,   3,   2,  5,  3,   9,   11,  4,   2,
            22,  1,   6,   3,   8,   7,   1,   4,  28, 4,   16,  3,   3,   25,
            4,   4,   27,  27,  1,   4,   1,   2,  2,  7,   1,   3,   5,   2,
            28,  8,   2,   14,  1,   8,   6,   16, 25, 3,   3,   3,   14,  3,
            3,   1,   1,   2,   1,   4,   6,   3,  8,  4,   1,   1,   1,   2,
            3,   6,   10,  6,   2,   3,   18,  3,  2,  5,   5,   4,   3,   1,
            5,   2,   5,   4,   23,  7,   6,   12, 6,  4,   17,  11,  9,   5,
            1,   1,   10,  5,   12,  1,   1,   11, 26, 33,  7,   3,   6,   1,
            17,  7,   1,   5,   12,  1,   11,  2,  4,  1,   8,   14,  17,  23,
            1,   2,   1,   7,   8,   16,  11,  9,  6,  5,   2,   6,   4,   16,
            2,   8,   14,  1,   11,  8,   9,   1,  1,  1,   9,   25,  4,   11,
            19,  7,   2,   15,  2,   12,  8,   52, 7,  5,   19,  2,   16,  4,
            36,  8,   1,   16,  8,   24,  26,  4,  6,  2,   9,   5,   4,   36,
            3,   28,  12,  25,  15,  37,  27,  17, 12, 59,  38,  5,   32,  127,
            1,   2,   9,   17,  14,  4,   1,   2,  1,  1,   8,   11,  50,  4,
            14,  2,   19,  16,  4,   17,  5,   4,  5,  26,  12,  45,  2,   23,
            45,  104, 30,  12,  8,   3,   10,  2,  2,  3,   3,   1,   4,   20,
            7,   2,   9,   6,   15,  2,   20,  1,  3,  16,  4,   11,  15,  6,
            134, 2,   5,   59,  1,   2,   2,   2,  1,  9,   17,  3,   26,  137,
            10,  211, 59,  1,   2,   4,   1,   4,  1,  1,   1,   2,   6,   2,
            3,   1,   1,   2,   3,   2,   3,   1,  3,  4,   4,   2,   3,   3,
            1,   4,   3,   1,   7,   2,   2,   3,  1,  2,   1,   3,   3,   3,
            2,   2,   3,   2,   1,   3,   14,  6,  1,  3,   2,   9,   6,   15,
            27,  9,   34,  145, 1,   1,   2,   1,  1,  1,   1,   2,   1,   1,
            1,   1,   2,   2,   2,   3,   1,   2,  1,  1,   1,   2,   3,   5,
            8,   3,   5,   2,   4,   1,   3,   2,  2,  2,   12,  4,   1,   1,
            1,   10,  4,   5,   1,   20,  4,   16, 1,  15,  9,   5,   12,  2,
            9,   2,   5,   4,   2,   26,  19,  7,  1,  26,  4,   30,  12,  15,
            42,  1,   6,   8,   172, 1,   1,   4,  2,  1,   1,   11,  2,   2,
            4,   2,   1,   2,   1,   10,  8,   1,  2,  1,   4,   5,   1,   2,
            5,   1,   8,   4,   1,   3,   4,   2,  1,  6,   2,   1,   3,   4,
            1,   2,   1,   1,   1,   1,   12,  5,  7,  2,   4,   3,   1,   1,
            1,   3,   3,   6,   1,   2,   2,   3,  3,  3,   2,   1,   2,   12,
            14,  11,  6,   6,   4,   12,  2,   8,  1,  7,   10,  1,   35,  7,
            4,   13,  15,  4,   3,   23,  21,  28, 52, 5,   26,  5,   6,   1,
            7,   10,  2,   7,   53,  3,   2,   1,  1,  1,   2,   163, 532, 1,
            10,  11,  1,   3,   3,   4,   8,   2,  8,  6,   2,   2,   23,  22,
            4,   2,   2,   4,   2,   1,   3,   1,  3,  3,   5,   9,   8,   2,
            1,   2,   8,   1,   10,  2,   12,  21, 20, 15,  105, 2,   3,   1,
            1,   3,   2,   3,   1,   1,   2,   5,  1,  4,   15,  11,  19,  1,
            1,   1,   1,   5,   4,   5,   1,   1,  2,  5,   3,   5,   12,  1,
            2,   5,   1,   11,  1,   1,   15,  9,  1,  4,   5,   3,   26,  8,
            2,   1,   3,   1,   1,   15,  19,  2,  12, 1,   2,   5,   2,   7,
            2,   19,  2,   20,  6,   26,  7,   5,  2,  2,   7,   34,  21,  13,
            70,  2,   128, 1,   1,   2,   1,   1,  2,  1,   1,   3,   2,   2,
            2,   15,  1,   4,   1,   3,   4,   42, 10, 6,   1,   49,  85,  8,
            1,   2,   1,   1,   4,   4,   2,   3,  6,  1,   5,   7,   4,   3,
            211, 4,   1,   2,   1,   2,   5,   1,  2,  4,   2,   2,   6,   5,
            6,   10,  3,   4,   48,  100, 6,   2,  16, 296, 5,   27,  387, 2,
            2,   3,   7,   16,  8,   5,   38,  15, 39, 21,  9,   10,  3,   7,
            59,  13,  27,  21,  47,  5,   21,  6};
    static AzWchar base_ranges[] = {0x0020,
                                    0x00FF,
                                    0x2000,
                                    0x206F,
                                    0x3000,
                                    0x30FF,
                                    0x31F0,
                                    0x31FF,
                                    0xFF00,
                                    0xFFEF,
                                    0xFFFD,
                                    0xFFFD};
    static AzWchar
            full_ranges[AZ_ARRAYSIZE(base_ranges)
                        + AZ_ARRAYSIZE(accumulative_offsets_from_0x4E00) * 2
                        + 1] = {0};
    if (!full_ranges[0])
    {
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        UnpackAccumulativeOffsetsIntoRanges(
                0x4E00,
                accumulative_offsets_from_0x4E00,
                AZ_ARRAYSIZE(accumulative_offsets_from_0x4E00),
                full_ranges + AZ_ARRAYSIZE(base_ranges));
    }
    return &full_ranges[0];
}

const AzWchar* AzFontAtlas::GetGlyphRangesJapanese()
{
    static const short accumulative_offsets_from_0x4E00[] = {
            0,  1,  2,     4,   1,   1,  1,   1,  2,   1,   3,  3,  2,  2,  1,
            5,  3,  5,     7,   5,   6,  1,   2,  1,   7,   2,  6,  3,  1,  8,
            1,  1,  4,     1,   1,   18, 2,   11, 2,   6,   2,  1,  2,  1,  5,
            1,  2,  1,     3,   1,   2,  1,   2,  3,   3,   1,  1,  2,  3,  1,
            1,  1,  12,    7,   9,   1,  4,   5,  1,   1,   2,  1,  10, 1,  1,
            9,  2,  2,     4,   5,   6,  9,   3,  1,   1,   1,  1,  9,  3,  18,
            5,  2,  2,     2,   2,   1,  6,   3,  7,   1,   1,  1,  1,  2,  2,
            4,  2,  1,     23,  2,   10, 4,   3,  5,   2,   4,  10, 2,  4,  13,
            1,  6,  1,     9,   3,   1,  1,   6,  6,   7,   6,  3,  1,  2,  11,
            3,  2,  2,     3,   2,   15, 2,   2,  5,   4,   3,  6,  4,  1,  2,
            5,  2,  12,    16,  6,   13, 9,   13, 2,   1,   1,  7,  16, 4,  7,
            1,  19, 1,     5,   1,   2,  2,   7,  7,   8,   2,  6,  5,  4,  9,
            18, 7,  4,     5,   9,   13, 11,  8,  15,  2,   1,  1,  1,  2,  1,
            2,  2,  1,     2,   2,   8,  2,   9,  3,   3,   1,  1,  4,  4,  1,
            1,  1,  4,     9,   1,   4,  3,   5,  5,   2,   7,  5,  3,  4,  8,
            2,  1,  13,    2,   3,   3,  1,   14, 1,   1,   4,  5,  1,  3,  6,
            1,  5,  2,     1,   1,   3,  3,   3,  3,   1,   1,  2,  7,  6,  6,
            7,  1,  4,     7,   6,   1,  1,   1,  1,   1,   12, 3,  3,  9,  5,
            2,  6,  1,     5,   6,   1,  2,   3,  18,  2,   4,  14, 4,  1,  3,
            6,  1,  1,     6,   3,   5,  5,   3,  2,   2,   2,  2,  12, 3,  1,
            4,  2,  3,     2,   3,   11, 1,   7,  4,   1,   2,  1,  3,  17, 1,
            9,  1,  24,    1,   1,   4,  2,   2,  4,   1,   2,  7,  1,  1,  1,
            3,  1,  2,     2,   4,   15, 1,   1,  2,   1,   1,  2,  1,  5,  2,
            5,  20, 2,     5,   9,   1,  10,  8,  7,   6,   1,  1,  1,  1,  1,
            1,  6,  2,     1,   2,   8,  1,   1,  1,   1,   5,  1,  1,  3,  1,
            1,  1,  1,     3,   1,   1,  12,  4,  1,   3,   1,  1,  1,  1,  1,
            10, 3,  1,     7,   5,   13, 1,   2,  3,   4,   6,  1,  1,  30, 2,
            9,  9,  1,     15,  38,  11, 3,   1,  8,   24,  7,  1,  9,  8,  10,
            2,  1,  9,     31,  2,   13, 6,   2,  9,   4,   49, 5,  2,  15, 2,
            1,  10, 2,     1,   1,   1,  2,   2,  6,   15,  30, 35, 3,  14, 18,
            8,  1,  16,    10,  28,  12, 19,  45, 38,  1,   3,  2,  3,  13, 2,
            1,  7,  3,     6,   5,   3,  4,   3,  1,   5,   7,  8,  1,  5,  3,
            18, 5,  3,     6,   1,   21, 4,   24, 9,   24,  40, 3,  14, 3,  21,
            3,  2,  1,     2,   4,   2,  3,   1,  15,  15,  6,  5,  1,  1,  3,
            1,  5,  6,     1,   9,   7,  3,   3,  2,   1,   4,  3,  8,  21, 5,
            16, 4,  5,     2,   10,  11, 11,  3,  6,   3,   2,  9,  3,  6,  13,
            1,  2,  1,     1,   1,   1,  11,  12, 6,   6,   1,  4,  2,  6,  5,
            2,  1,  1,     3,   3,   6,  13,  3,  1,   1,   5,  1,  2,  3,  3,
            14, 2,  1,     2,   2,   2,  5,   1,  9,   5,   1,  1,  6,  12, 3,
            12, 3,  4,     13,  2,   14, 2,   8,  1,   17,  5,  1,  16, 4,  2,
            2,  21, 8,     9,   6,   23, 20,  12, 25,  19,  9,  38, 8,  3,  21,
            40, 25, 33,    13,  4,   3,  1,   4,  1,   2,   4,  1,  2,  5,  26,
            2,  1,  1,     2,   1,   3,  6,   2,  1,   1,   1,  1,  1,  1,  2,
            3,  1,  1,     1,   9,   2,  3,   1,  1,   1,   3,  6,  3,  2,  1,
            1,  6,  6,     1,   8,   2,  2,   2,  1,   4,   1,  2,  3,  2,  7,
            3,  2,  4,     1,   2,   1,  2,   2,  1,   1,   1,  1,  1,  3,  1,
            2,  5,  4,     10,  9,   4,  9,   1,  1,   1,   1,  1,  1,  5,  3,
            2,  1,  6,     4,   9,   6,  1,   10, 2,   31,  17, 8,  3,  7,  5,
            40, 1,  7,     7,   1,   6,  5,   2,  10,  7,   8,  4,  15, 39, 25,
            6,  28, 47,    18,  10,  7,  1,   3,  1,   1,   2,  1,  1,  1,  3,
            3,  3,  1,     1,   1,   3,  4,   2,  1,   4,   1,  3,  6,  10, 7,
            8,  6,  2,     2,   1,   3,  3,   2,  5,   8,   7,  9,  12, 2,  15,
            1,  1,  4,     1,   2,   1,  1,   1,  3,   2,   1,  3,  3,  5,  6,
            2,  3,  2,     10,  1,   4,  2,   8,  1,   1,   1,  11, 6,  1,  21,
            4,  16, 3,     1,   3,   1,  4,   2,  3,   6,   5,  1,  3,  1,  1,
            3,  3,  4,     6,   1,   1,  10,  4,  2,   7,   10, 4,  7,  4,  2,
            9,  4,  3,     1,   1,   1,  4,   1,  8,   3,   4,  1,  3,  1,  6,
            1,  4,  2,     1,   4,   7,  2,   1,  8,   1,   4,  5,  1,  1,  2,
            2,  4,  6,     2,   7,   1,  10,  1,  1,   3,   4,  11, 10, 8,  21,
            4,  6,  1,     3,   5,   2,  1,   2,  28,  5,   5,  2,  3,  13, 1,
            2,  3,  1,     4,   2,   1,  5,   20, 3,   8,   11, 1,  3,  3,  3,
            1,  8,  10,    9,   2,   10, 9,   2,  3,   1,   1,  2,  4,  1,  8,
            3,  6,  1,     7,   8,   6,  11,  1,  4,   29,  8,  4,  3,  1,  2,
            7,  13, 1,     4,   1,   6,  2,   6,  12,  12,  2,  20, 3,  2,  3,
            6,  4,  8,     9,   2,   7,  34,  5,  1,   18,  6,  1,  1,  4,  4,
            5,  7,  9,     1,   2,   2,  4,   3,  4,   1,   7,  2,  2,  2,  6,
            2,  3,  25,    5,   3,   6,  1,   4,  6,   7,   4,  2,  1,  4,  2,
            13, 6,  4,     4,   3,   1,  5,   3,  4,   4,   3,  2,  1,  1,  4,
            1,  2,  1,     1,   3,   1,  11,  1,  6,   3,   1,  7,  3,  6,  2,
            8,  8,  6,     9,   3,   4,  11,  3,  2,   10,  12, 2,  5,  11, 1,
            6,  4,  5,     3,   1,   8,  5,   4,  6,   6,   3,  5,  1,  1,  3,
            2,  1,  2,     2,   6,   17, 12,  1,  10,  1,   6,  12, 1,  6,  6,
            19, 9,  6,     16,  1,   13, 4,   4,  15,  7,   17, 6,  11, 9,  15,
            12, 6,  7,     2,   1,   2,  2,   15, 9,   3,   21, 4,  6,  49, 18,
            7,  3,  2,     3,   1,   6,  8,   2,  2,   6,   2,  9,  1,  3,  6,
            4,  4,  1,     2,   16,  2,  5,   2,  1,   6,   2,  3,  5,  3,  1,
            2,  5,  1,     2,   1,   9,  3,   1,  8,   6,   4,  8,  11, 3,  1,
            1,  1,  1,     3,   1,   13, 8,   4,  1,   3,   2,  2,  1,  4,  1,
            11, 1,  5,     2,   1,   5,  2,   5,  8,   6,   1,  1,  7,  4,  3,
            8,  3,  2,     7,   2,   1,  5,   1,  5,   2,   4,  7,  6,  2,  8,
            5,  1,  11,    4,   5,   3,  6,   18, 1,   2,   13, 3,  3,  1,  21,
            1,  1,  4,     1,   4,   1,  1,   1,  8,   1,   2,  2,  7,  1,  2,
            4,  2,  2,     9,   2,   1,  1,   1,  4,   3,   6,  3,  12, 5,  1,
            1,  1,  5,     6,   3,   2,  4,   8,  2,   2,   4,  2,  7,  1,  8,
            9,  5,  2,     3,   2,   1,  3,   2,  13,  7,   14, 6,  5,  1,  1,
            2,  1,  4,     2,   23,  2,  1,   1,  6,   3,   1,  4,  1,  15, 3,
            1,  7,  3,     9,   14,  1,  3,   1,  4,   1,   1,  5,  8,  1,  3,
            8,  3,  8,     15,  11,  4,  14,  4,  4,   2,   5,  5,  1,  7,  1,
            6,  14, 7,     7,   8,   5,  15,  4,  8,   6,   5,  6,  2,  1,  13,
            1,  20, 15,    11,  9,   2,  5,   6,  2,   11,  2,  6,  2,  5,  1,
            5,  8,  4,     13,  19,  25, 4,   1,  1,   11,  1,  34, 2,  5,  9,
            14, 6,  2,     2,   6,   1,  1,   14, 1,   3,   14, 13, 1,  6,  12,
            21, 14, 14,    6,   32,  17, 8,   32, 9,   28,  1,  2,  4,  11, 8,
            3,  1,  14,    2,   5,   15, 1,   1,  1,   1,   3,  6,  4,  1,  3,
            4,  11, 3,     1,   1,   11, 30,  1,  5,   1,   4,  1,  5,  8,  1,
            1,  3,  2,     4,   3,   17, 35,  2,  6,   12,  17, 3,  1,  6,  2,
            1,  1,  12,    2,   7,   3,  3,   2,  1,   16,  2,  8,  3,  6,  5,
            4,  7,  3,     3,   8,   1,  9,   8,  5,   1,   2,  1,  3,  2,  8,
            1,  2,  9,     12,  1,   1,  2,   3,  8,   3,   24, 12, 4,  3,  7,
            5,  8,  3,     3,   3,   3,  3,   3,  1,   23,  10, 3,  1,  2,  2,
            6,  3,  1,     16,  1,   16, 22,  3,  10,  4,   11, 6,  9,  7,  7,
            3,  6,  2,     2,   2,   4,  10,  2,  1,   1,   2,  8,  7,  1,  6,
            4,  1,  3,     3,   3,   5,  10,  12, 12,  2,   3,  12, 8,  15, 1,
            1,  16, 6,     6,   1,   5,  9,   11, 4,   11,  4,  2,  6,  12, 1,
            17, 5,  13,    1,   4,   9,  5,   1,  11,  2,   1,  8,  1,  5,  7,
            28, 8,  3,     5,   10,  2,  17,  3,  38,  22,  1,  2,  18, 12, 10,
            4,  38, 18,    1,   4,   44, 19,  4,  1,   8,   4,  1,  12, 1,  4,
            31, 12, 1,     14,  7,   75, 7,   5,  10,  6,   6,  13, 3,  2,  11,
            11, 3,  2,     5,   28,  15, 6,   18, 18,  5,   6,  4,  3,  16, 1,
            7,  18, 7,     36,  3,   5,  3,   1,  7,   1,   9,  1,  10, 7,  2,
            4,  2,  6,     2,   9,   7,  4,   3,  32,  12,  3,  7,  10, 2,  23,
            16, 3,  1,     12,  3,   31, 4,   11, 1,   3,   8,  9,  5,  1,  30,
            15, 6,  12,    3,   2,   2,  11,  19, 9,   14,  2,  6,  2,  3,  19,
            13, 17, 5,     3,   3,   25, 3,   14, 1,   1,   1,  36, 1,  3,  2,
            19, 3,  13,    36,  9,   13, 31,  6,  4,   16,  34, 2,  5,  4,  2,
            3,  3,  5,     1,   1,   1,  4,   3,  1,   17,  3,  2,  3,  5,  3,
            1,  3,  2,     3,   5,   6,  3,   12, 11,  1,   3,  1,  2,  26, 7,
            12, 7,  2,     14,  3,   3,  7,   7,  11,  25,  25, 28, 16, 4,  36,
            1,  2,  1,     6,   2,   1,  9,   3,  27,  17,  4,  3,  4,  13, 4,
            1,  3,  2,     2,   1,   10, 4,   2,  4,   6,   3,  8,  2,  1,  18,
            1,  1,  24,    2,   2,   4,  33,  2,  3,   63,  7,  1,  6,  40, 7,
            3,  4,  4,     2,   4,   15, 18,  1,  16,  1,   1,  11, 2,  41, 14,
            1,  3,  18,    13,  3,   2,  4,   16, 2,   17,  7,  15, 24, 7,  18,
            13, 44, 2,     2,   3,   6,  1,   1,  7,   5,   1,  7,  1,  4,  3,
            3,  5,  10,    8,   2,   3,  1,   8,  1,   1,   27, 4,  2,  1,  12,
            1,  2,  1,     10,  6,   1,  6,   7,  5,   2,   3,  7,  11, 5,  11,
            3,  6,  6,     2,   3,   15, 4,   9,  1,   1,   2,  1,  2,  11, 2,
            8,  12, 8,     5,   4,   2,  3,   1,  5,   2,   2,  1,  14, 1,  12,
            11, 4,  1,     11,  17,  17, 4,   3,  2,   5,   5,  7,  3,  1,  5,
            9,  9,  8,     2,   5,   6,  6,   13, 13,  2,   1,  2,  6,  1,  2,
            2,  49, 4,     9,   1,   2,  10,  16, 7,   8,   4,  3,  2,  23, 4,
            58, 3,  29,    1,   14,  19, 19,  11, 11,  2,   7,  5,  1,  3,  4,
            6,  2,  18,    5,   12,  12, 17,  17, 3,   3,   2,  4,  1,  6,  2,
            3,  4,  3,     1,   1,   1,  1,   5,  1,   1,   9,  1,  3,  1,  3,
            6,  1,  8,     1,   1,   2,  6,   4,  14,  3,   1,  4,  11, 4,  1,
            3,  32, 1,     2,   4,   13, 4,   1,  2,   4,   2,  1,  3,  1,  11,
            1,  4,  2,     1,   4,   4,  6,   3,  5,   1,   6,  5,  7,  6,  3,
            23, 3,  5,     3,   5,   3,  3,   13, 3,   9,   10, 1,  12, 10, 2,
            3,  18, 13,    7,   160, 52, 4,   2,  2,   3,   2,  14, 5,  4,  12,
            4,  6,  4,     1,   20,  4,  11,  6,  2,   12,  27, 1,  4,  1,  2,
            2,  7,  4,     5,   2,   28, 3,   7,  25,  8,   3,  19, 3,  6,  10,
            2,  2,  1,     10,  2,   5,  4,   1,  3,   4,   1,  5,  3,  2,  6,
            9,  3,  6,     2,   16,  3,  3,   16, 4,   5,   5,  3,  2,  1,  2,
            16, 15, 8,     2,   6,   21, 2,   4,  1,   22,  5,  8,  1,  1,  21,
            11, 2,  1,     11,  11,  19, 13,  12, 4,   2,   3,  2,  3,  6,  1,
            8,  11, 1,     4,   2,   9,  5,   2,  1,   11,  2,  9,  1,  1,  2,
            14, 31, 9,     3,   4,   21, 14,  4,  8,   1,   7,  2,  2,  2,  5,
            1,  4,  20,    3,   3,   4,  10,  1,  11,  9,   8,  2,  1,  4,  5,
            14, 12, 14,    2,   17,  9,  6,   31, 4,   14,  1,  20, 13, 26, 5,
            2,  7,  3,     6,   13,  2,  4,   2,  19,  6,   2,  2,  18, 9,  3,
            5,  12, 12,    14,  4,   6,  2,   3,  6,   9,   5,  22, 4,  5,  25,
            6,  4,  8,     5,   2,   6,  27,  2,  35,  2,   16, 3,  7,  8,  8,
            6,  6,  5,     9,   17,  2,  20,  6,  19,  2,   13, 3,  1,  1,  1,
            4,  17, 12,    2,   14,  7,  1,   4,  18,  12,  38, 33, 2,  10, 1,
            1,  2,  13,    14,  17,  11, 50,  6,  33,  20,  26, 74, 16, 23, 45,
            50, 13, 38,    33,  6,   6,  7,   4,  4,   2,   1,  3,  2,  5,  8,
            7,  8,  9,     3,   11,  21, 9,   13, 1,   3,   10, 6,  7,  1,  2,
            2,  18, 5,     5,   1,   9,  9,   2,  68,  9,   19, 13, 2,  5,  1,
            4,  4,  7,     4,   13,  3,  9,   10, 21,  17,  3,  26, 2,  1,  5,
            2,  4,  5,     4,   1,   7,  4,   7,  3,   4,   2,  1,  6,  1,  1,
            20, 4,  1,     9,   2,   2,  1,   3,  3,   2,   3,  2,  1,  1,  1,
            20, 2,  3,     1,   6,   2,  3,   6,  2,   4,   8,  1,  3,  2,  10,
            3,  5,  3,     4,   4,   3,  4,   16, 1,   6,   1,  10, 2,  4,  2,
            1,  1,  2,     10,  11,  2,  2,   3,  1,   24,  31, 4,  10, 10, 2,
            5,  12, 16,    164, 15,  4,  16,  7,  9,   15,  19, 17, 1,  2,  1,
            1,  5,  1,     1,   1,   1,  1,   3,  1,   4,   3,  1,  3,  1,  3,
            1,  2,  1,     1,   3,   3,  7,   2,  8,   1,   2,  2,  2,  1,  3,
            4,  3,  7,     8,   12,  92, 2,   10, 3,   1,   3,  14, 5,  25, 16,
            42, 4,  7,     7,   4,   2,  21,  5,  27,  26,  27, 21, 25, 30, 31,
            2,  1,  5,     13,  3,   22, 5,   6,  6,   11,  9,  12, 1,  5,  9,
            7,  5,  5,     22,  60,  3,  5,   13, 1,   1,   8,  1,  1,  3,  3,
            2,  1,  9,     3,   3,   18, 4,   1,  2,   3,   7,  6,  3,  1,  2,
            3,  9,  1,     3,   1,   3,  2,   1,  3,   1,   1,  1,  2,  1,  11,
            3,  1,  6,     9,   1,   3,  2,   3,  1,   2,   1,  5,  1,  1,  4,
            3,  4,  1,     2,   2,   4,  4,   1,  7,   2,   1,  2,  2,  3,  5,
            13, 18, 3,     4,   14,  9,  9,   4,  16,  3,   7,  5,  8,  2,  6,
            48, 28, 3,     1,   1,   4,  2,   14, 8,   2,   9,  2,  1,  15, 2,
            4,  3,  2,     10,  16,  12, 8,   7,  1,   1,   3,  1,  1,  1,  2,
            7,  4,  1,     6,   4,   38, 39,  16, 23,  7,   15, 15, 3,  2,  12,
            7,  21, 37,    27,  6,   5,  4,   8,  2,   10,  8,  8,  6,  5,  1,
            2,  1,  3,     24,  1,   16, 17,  9,  23,  10,  17, 6,  1,  51, 55,
            44, 13, 294,   9,   3,   6,  2,   4,  2,   2,   15, 1,  1,  1,  13,
            21, 17, 68,    14,  8,   9,  4,   1,  4,   9,   3,  11, 7,  1,  1,
            1,  5,  6,     3,   2,   1,  1,   1,  2,   3,   8,  1,  2,  2,  4,
            1,  5,  5,     2,   1,   4,  3,   7,  13,  4,   1,  4,  1,  3,  1,
            1,  1,  5,     5,   10,  1,  6,   1,  5,   2,   1,  5,  2,  4,  1,
            4,  5,  7,     3,   18,  2,  9,   11, 32,  4,   3,  3,  2,  4,  7,
            11, 16, 9,     11,  8,   13, 38,  32, 8,   4,   2,  1,  1,  2,  1,
            2,  4,  4,     1,   1,   1,  4,   1,  21,  3,   11, 1,  16, 1,  1,
            6,  1,  3,     2,   4,   9,  8,   57, 7,   44,  1,  3,  3,  13, 3,
            10, 1,  1,     7,   5,   2,  7,   21, 47,  63,  3,  15, 4,  7,  1,
            16, 1,  1,     2,   8,   2,  3,   42, 15,  4,   1,  29, 7,  22, 10,
            3,  78, 16,    12,  20,  18, 4,   67, 11,  5,   1,  3,  15, 6,  21,
            31, 32, 27,    18,  13,  71, 35,  5,  142, 4,   10, 1,  2,  50, 19,
            33, 16, 35,    37,  16,  19, 27,  7,  1,   133, 19, 1,  4,  8,  7,
            20, 1,  4,     4,   1,   10, 3,   1,  6,   1,   2,  51, 5,  40, 15,
            24, 43, 22928, 11,  1,   13, 154, 70, 3,   1,   1,  7,  4,  10, 1,
            2,  1,  1,     2,   1,   2,  1,   2,  2,   1,   1,  2,  1,  1,  1,
            1,  1,  2,     1,   1,   1,  1,   1,  1,   1,   1,  1,  1,  1,  1,
            1,  2,  1,     1,   1,   3,  2,   1,  1,   1,   1,  2,  1,  1,
    };
    static AzWchar base_ranges[] = {0x0020,
                                    0x00FF,
                                    0x3000,
                                    0x30FF,
                                    0x31F0,
                                    0x31FF,
                                    0xFF00,
                                    0xFFEF,
                                    0xFFFD,
                                    0xFFFD};
    static AzWchar
            full_ranges[AZ_ARRAYSIZE(base_ranges)
                        + AZ_ARRAYSIZE(accumulative_offsets_from_0x4E00) * 2
                        + 1] = {0};
    if (!full_ranges[0])
    {
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        UnpackAccumulativeOffsetsIntoRanges(
                0x4E00,
                accumulative_offsets_from_0x4E00,
                AZ_ARRAYSIZE(accumulative_offsets_from_0x4E00),
                full_ranges + AZ_ARRAYSIZE(base_ranges));
    }
    return &full_ranges[0];
}

const AzWchar* AzFontAtlas::GetGlyphRangesCyrillic()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0x0400,
            0x052F,
            0x2DE0,
            0x2DFF,
            0xA640,
            0xA69F,
            0,
    };
    return &ranges[0];
}

const AzWchar* AzFontAtlas::GetGlyphRangesThai()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0x2010,
            0x205E,
            0x0E00,
            0x0E7F,
            0,
    };
    return &ranges[0];
}

const AzWchar* AzFontAtlas::GetGlyphRangesVietnamese()
{
    static const AzWchar ranges[] = {
            0x0020,
            0x00FF,
            0x0102,
            0x0103,
            0x0110,
            0x0111,
            0x0128,
            0x0129,
            0x0168,
            0x0169,
            0x01A0,
            0x01A1,
            0x01AF,
            0x01B0,
            0x1EA0,
            0x1EF9,
            0,
    };
    return &ranges[0];
}
#endif

void AzFontGlyphRangesBuilder::AddText(const char* text, const char* text_end)
{
    while (text_end ? (text < text_end) : *text)
    {
        unsigned int c = 0;
        int c_len = AzTextCharFromUtf8(&c, text, text_end);
        text += c_len;
        if (c_len == 0)
            break;
        AddChar((AzWchar)c);
    }
}

void AzFontGlyphRangesBuilder::AddRanges(const AzWchar* ranges)
{
    for (; ranges[0]; ranges += 2)
        for (unsigned int c = ranges[0];
             c <= ranges[1] && c <= AZ_UNICODE_CODEPOINT_MAX;
             c++)
            AddChar((AzWchar)c);
}

void AzFontGlyphRangesBuilder::BuildRanges(AzVector<AzWchar>* out_ranges)
{
    const int max_codepoint = AZ_UNICODE_CODEPOINT_MAX;
    for (int n = 0; n <= max_codepoint; n++)
        if (GetBit(n))
        {
            out_ranges->push_back((AzWchar)n);
            while (n < max_codepoint && GetBit(n + 1))
                n++;
            out_ranges->push_back((AzWchar)n);
        }
    out_ranges->push_back(0);
}

AzFontBaked::AzFontBaked()
{
    memset(this, 0, sizeof(*this));
    FallbackGlyphIndex = -1;
}

void AzFontBaked::ClearOutputData()
{
    FallbackAdvanceX = 0.0f;
    Glyphs.clear();
    IndexAdvanceX.clear();
    IndexLookup.clear();
    FallbackGlyphIndex = -1;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
}

AzFont::AzFont()
{
    memset(this, 0, sizeof(*this));
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    Scale = 1.0f;
#endif
}

AzFont::~AzFont()
{
    ClearOutputData();
}

void AzFont::ClearOutputData()
{
    if (AzFontAtlas* atlas = ContainerAtlas)
        AzFontAtlasFontDiscardBakes(atlas, this, 0);
    FallbackChar = EllipsisChar = 0;
    memset(Used8kPagesMap, 0, sizeof(Used8kPagesMap));
    LastBaked = NULL;
}

bool AzFont::IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last)
{
    unsigned int page_begin = (c_begin / 8192);
    unsigned int page_last = (c_last / 8192);
    for (unsigned int page_n = page_begin; page_n <= page_last; page_n++)
        if ((page_n >> 3) < sizeof(Used8kPagesMap))
            if (Used8kPagesMap[page_n >> 3] & (1 << (page_n & 7)))
                return false;
    return true;
}

AzFontGlyph* AzFontAtlasBakedAddFontGlyph(AzFontAtlas* atlas,
                                          AzFontBaked* baked,
                                          AzFontConfig* src,
                                          const AzFontGlyph* in_glyph)
{
    int glyph_idx = baked->Glyphs.Size;
    baked->Glyphs.push_back(*in_glyph);
    AzFontGlyph* glyph = &baked->Glyphs[glyph_idx];
    AZ_ASSERT(baked->Glyphs.Size < 0xFFFE);

    if (glyph->PackId != AzFontAtlasRectId_Invalid)
    {
        AzTextureRect* r = AzFontAtlasPackGetRect(atlas, glyph->PackId);
        AZ_ASSERT(glyph->U0 == 0.0f && glyph->V0 == 0.0f && glyph->U1 == 0.0f
                  && glyph->V1 == 0.0f);
        glyph->U0 = (r->x) * atlas->TexUvScale.x;
        glyph->V0 = (r->y) * atlas->TexUvScale.y;
        glyph->U1 = (r->x + r->w) * atlas->TexUvScale.x;
        glyph->V1 = (r->y + r->h) * atlas->TexUvScale.y;
        baked->MetricsTotalSurface += r->w * r->h;
    }

    if (src != NULL)
    {
        const float ref_size = baked->ContainerFont->Sources[0]->SizePixels;
        const float offsets_scale = (ref_size != 0.0f)
                                            ? (baked->Size / ref_size)
                                            : 1.0f;
        float advance_x = AzClamp(glyph->AdvanceX,
                                  src->GlyphMinAdvanceX * offsets_scale,
                                  src->GlyphMaxAdvanceX * offsets_scale);
        if (advance_x != glyph->AdvanceX)
        {
            float char_off_x = src->PixelSnapH
                                       ? AzTrunc((advance_x - glyph->AdvanceX)
                                                 * 0.5f)
                                       : (advance_x - glyph->AdvanceX) * 0.5f;
            glyph->X0 += char_off_x;
            glyph->X1 += char_off_x;
        }

        if (src->PixelSnapH)
            advance_x = AZ_ROUND(advance_x);

        glyph->AdvanceX = advance_x + src->GlyphExtraAdvanceX;
    }
    if (glyph->Colored)
        atlas->TexPixelsUseColors = atlas->TexData->UseColors = true;

    const int codepoint = glyph->Codepoint;
    AzFontBaked_BuildGrowIndex(baked, codepoint + 1);
    baked->IndexAdvanceX[codepoint] = glyph->AdvanceX;
    baked->IndexLookup[codepoint] = (AzU16)glyph_idx;
    const int page_n = codepoint / 8192;
    baked->ContainerFont->Used8kPagesMap[page_n >> 3] |= 1 << (page_n & 7);

    return glyph;
}

void AzFontAtlasBakedSetFontGlyphBitmap(AzFontAtlas* atlas,
                                        AzFontBaked* baked,
                                        AzFontConfig* src,
                                        AzFontGlyph* glyph,
                                        AzTextureRect* r,
                                        const unsigned char* src_pixels,
                                        AzTextureFormat src_fmt,
                                        int src_pitch)
{
    AzTextureData* tex = atlas->TexData;
    AZ_ASSERT(r->x + r->w <= tex->Width && r->y + r->h <= tex->Height);
    AzFontAtlasTextureBlockConvert(src_pixels,
                                   src_fmt,
                                   src_pitch,
                                   tex->GetPixelsAt(r->x, r->y),
                                   tex->Format,
                                   tex->GetPitch(),
                                   r->w,
                                   r->h);
    AzFontAtlasPostProcessData pp_data = {atlas,
                                          baked->ContainerFont,
                                          src,
                                          baked,
                                          glyph,
                                          tex->GetPixelsAt(r->x, r->y),
                                          tex->Format,
                                          tex->GetPitch(),
                                          r->w,
                                          r->h};
    AzFontAtlasTextureBlockPostProcess(&pp_data);
    AzFontAtlasTextureBlockQueueUpload(atlas, tex, r->x, r->y, r->w, r->h);
}

void AzFont::AddRemapChar(AzWchar from_codepoint, AzWchar to_codepoint)
{
    RemapPairs.SetInt((AzID)from_codepoint, (int)to_codepoint);
}

AzFontGlyph* AzFontBaked::FindGlyph(AzWchar c)
{
    if (c < (size_t)IndexLookup.Size)
        AZ_LIKELY
        {
            const int i = (int)IndexLookup.Data[c];
            if (i == AZ_FONTGLYPH_INDEX_NOT_FOUND)
                return &Glyphs.Data[FallbackGlyphIndex];
            if (i != AZ_FONTGLYPH_INDEX_UNUSED)
                return &Glyphs.Data[i];
        }
    AzFontGlyph* glyph = AzFontBaked_BuildLoadGlyph(this, c);
    return glyph ? glyph : &Glyphs.Data[FallbackGlyphIndex];
}

AzFontGlyph* AzFontBaked::FindGlyphNoFallback(AzWchar c)
{
    if (c < (size_t)IndexLookup.Size)
        AZ_LIKELY
        {
            const int i = (int)IndexLookup.Data[c];
            if (i == AZ_FONTGLYPH_INDEX_NOT_FOUND)
                return NULL;
            if (i != AZ_FONTGLYPH_INDEX_UNUSED)
                return &Glyphs.Data[i];
        }
    LockLoadingFallback = true;
    AzFontGlyph* glyph = AzFontBaked_BuildLoadGlyph(this, c);
    LockLoadingFallback = false;
    return glyph;
}

bool AzFontBaked::IsGlyphLoaded(AzWchar c)
{
    if (c < (size_t)IndexLookup.Size)
        AZ_LIKELY
        {
            const int i = (int)IndexLookup.Data[c];
            if (i == AZ_FONTGLYPH_INDEX_NOT_FOUND)
                return false;
            if (i != AZ_FONTGLYPH_INDEX_UNUSED)
                return true;
        }
    return false;
}

bool AzFont::IsGlyphInFont(AzWchar c)
{
    AzFontAtlas* atlas = ContainerAtlas;
    AzFontAtlas_FontHookRemapCodepoint(atlas, this, &c);
    for (AzFontConfig* src : Sources)
    {
        const AzFontLoader* loader = src->FontLoader ? src->FontLoader
                                                     : atlas->FontLoader;
        if (loader->FontSrcContainsGlyph != NULL
            && loader->FontSrcContainsGlyph(atlas, src, c))
            return true;
    }
    return false;
}

AZ_MSVC_RUNTIME_CHECKS_OFF
float AzFontBaked::GetCharAdvance(AzWchar c)
{
    if ((int)c < IndexAdvanceX.Size)
    {
        const float x = IndexAdvanceX.Data[c];
        if (x >= 0.0f)
            return x;
    }

    const AzFontGlyph* glyph = AzFontBaked_BuildLoadGlyph(this, c);
    return glyph ? glyph->AdvanceX : FallbackAdvanceX;
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

AzID AzFontAtlasBakedGetId(AzID font_id,
                           float baked_size,
                           float rasterizer_density)
{
    struct
    {
        AzID FontId;
        float BakedSize;
        float RasterizerDensity;
    } hashed_data;
    hashed_data.FontId = font_id;
    hashed_data.BakedSize = baked_size;
    hashed_data.RasterizerDensity = rasterizer_density;
    return AzHashData(&hashed_data, sizeof(hashed_data));
}

AzFontBaked* AzFont::GetFontBaked(float size, float density)
{
    AzFontBaked* baked = LastBaked;

    size = Az::GetRoundedFontSize(size);

    if (density < 0.0f)
        density = CurrentRasterizerDensity;
    if (baked && baked->Size == size && baked->RasterizerDensity == density)
        return baked;

    AzFontAtlas* atlas = ContainerAtlas;
    AzFontAtlasBuilder* builder = atlas->Builder;
    baked = AzFontAtlasBakedGetOrAdd(atlas, this, size, density);
    if (baked == NULL)
        return NULL;
    baked->LastUsedFrame = builder->FrameCount;
    LastBaked = baked;
    return baked;
}

AzFontBaked* AzFontAtlasBakedGetOrAdd(AzFontAtlas* atlas,
                                      AzFont* font,
                                      float font_size,
                                      float font_rasterizer_density)
{
    AZ_ASSERT(font_size > 0.0f && font_rasterizer_density > 0.0f);
    AzID baked_id = AzFontAtlasBakedGetId(font->FontId,
                                          font_size,
                                          font_rasterizer_density);
    AzFontAtlasBuilder* builder = atlas->Builder;
    AzFontBaked** p_baked_in_map =
            (AzFontBaked**)builder->BakedMap.GetVoidPtrRef(baked_id);
    AzFontBaked* baked = *p_baked_in_map;
    if (baked != NULL)
    {
        AZ_ASSERT(baked->Size == font_size && baked->ContainerFont == font
                  && baked->BakedId == baked_id);
        return baked;
    }

    if ((font->Flags & AzFontFlags_LockBakedSizes) || atlas->Locked)
    {
        baked = AzFontAtlasBakedGetClosestMatch(atlas,
                                                font,
                                                font_size,
                                                font_rasterizer_density);
        if (baked != NULL)
            return baked;
        if (atlas->Locked)
        {
            AZ_ASSERT(!atlas->Locked && "Cannot use dynamic font size with a locked AzFontAtlas!");
            return NULL;
        }
    }

    baked = AzFontAtlasBakedAdd(atlas,
                                font,
                                font_size,
                                font_rasterizer_density,
                                baked_id);
    *p_baked_in_map = baked;
    return baked;
}

static inline const char* CalcWordWrapNextLineStartA(const char* text,
                                                     const char* text_end)
{
    while (text < text_end && AzCharIsBlankA(*text))
        text++;
    if (*text == '\n')
        text++;
    return text;
}

const char* AzFont::CalcWordWrapPosition(float size,
                                         const char* text,
                                         const char* text_end,
                                         float wrap_width)
{
    AzFontBaked* baked = GetFontBaked(size);
    const float scale = size / baked->Size;

    float line_width = 0.0f;
    float word_width = 0.0f;
    float blank_width = 0.0f;
    wrap_width /= scale;

    const char* word_end = text;
    const char* prev_word_end = NULL;
    bool inside_word = true;

    const char* s = text;
    AZ_ASSERT(text_end != NULL);
    while (s < text_end)
    {
        unsigned int c = (unsigned int)*s;
        const char* next_s;
        if (c < 0x80)
            next_s = s + 1;
        else
            next_s = s + AzTextCharFromUtf8(&c, s, text_end);

        if (c < 32)
        {
            if (c == '\n')
            {
                line_width = word_width = blank_width = 0.0f;
                inside_word = true;
                s = next_s;
                continue;
            }
            if (c == '\r')
            {
                s = next_s;
                continue;
            }
        }

        float char_width = (c < (unsigned int)baked->IndexAdvanceX.Size)
                                   ? baked->IndexAdvanceX.Data[c]
                                   : -1.0f;
        if (char_width < 0.0f)
            char_width = BuildLoadGlyphGetAdvanceOrFallback(baked, c);

        if (AzCharIsBlankW(c))
        {
            if (inside_word)
            {
                line_width += blank_width;
                blank_width = 0.0f;
                word_end = s;
            }
            blank_width += char_width;
            inside_word = false;
        } else
        {
            word_width += char_width;
            if (inside_word)
            {
                word_end = next_s;
            } else
            {
                prev_word_end = word_end;
                line_width += word_width + blank_width;
                word_width = blank_width = 0.0f;
            }

            inside_word = (c != '.' && c != ',' && c != ';' && c != '!'
                           && c != '?' && c != '\"' && c != 0x3001
                           && c != 0x3002);
        }

        if (line_width + word_width > wrap_width)
        {
            if (word_width < wrap_width)
                s = prev_word_end ? prev_word_end : word_end;
            break;
        }

        s = next_s;
    }

    if (s == text && text < text_end)
        return s + AzTextCountUtf8BytesFromChar(s, text_end);
    return s;
}

AzVec2 AzFont::CalcTextSizeA(float size,
                             float max_width,
                             float wrap_width,
                             const char* text_begin,
                             const char* text_end,
                             const char** remaining)
{
    if (!text_end)
        text_end = text_begin + AzStrlen(text_begin);

    const float line_height = size;
    AzFontBaked* baked = GetFontBaked(size);
    const float scale = size / baked->Size;

    AzVec2 text_size = AzVec2(0, 0);
    float line_width = 0.0f;

    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    const char* s = text_begin;
    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            if (!word_wrap_eol)
                word_wrap_eol = CalcWordWrapPosition(size,
                                                     s,
                                                     text_end,
                                                     wrap_width - line_width);

            if (s >= word_wrap_eol)
            {
                if (text_size.x < line_width)
                    text_size.x = line_width;
                text_size.y += line_height;
                line_width = 0.0f;
                word_wrap_eol = NULL;
                s = CalcWordWrapNextLineStartA(s, text_end);
                continue;
            }
        }

        const char* prev_s = s;
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
            s += 1;
        else
            s += AzTextCharFromUtf8(&c, s, text_end);

        if (c < 32)
        {
            if (c == '\n')
            {
                text_size.x = AzMax(text_size.x, line_width);
                text_size.y += line_height;
                line_width = 0.0f;
                continue;
            }
            if (c == '\r')
                continue;
        }

        float char_width = (c < (unsigned int)baked->IndexAdvanceX.Size)
                                   ? baked->IndexAdvanceX.Data[c]
                                   : -1.0f;
        if (char_width < 0.0f)
            char_width = BuildLoadGlyphGetAdvanceOrFallback(baked, c);
        char_width *= scale;

        if (line_width + char_width >= max_width)
        {
            s = prev_s;
            break;
        }

        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

void AzFont::RenderChar(AzDrawList* draw_list,
                        float size,
                        const AzVec2& pos,
                        AzU32 col,
                        AzWchar c,
                        const AzVec4* cpu_fine_clip)
{
    AzFontBaked* baked = GetFontBaked(size);
    const AzFontGlyph* glyph = baked->FindGlyph(c);
    if (!glyph || !glyph->Visible)
        return;
    if (glyph->Colored)
        col |= ~AZ_COL32_A_MASK;
    float scale = (size >= 0.0f) ? (size / baked->Size) : 1.0f;
    float x = AZ_TRUNC(pos.x);
    float y = AZ_TRUNC(pos.y);

    float x1 = x + glyph->X0 * scale;
    float x2 = x + glyph->X1 * scale;
    if (cpu_fine_clip && (x1 > cpu_fine_clip->z || x2 < cpu_fine_clip->x))
        return;
    float y1 = y + glyph->Y0 * scale;
    float y2 = y + glyph->Y1 * scale;
    float u1 = glyph->U0;
    float v1 = glyph->V0;
    float u2 = glyph->U1;
    float v2 = glyph->V1;

    if (cpu_fine_clip != NULL)
    {
        if (x1 < cpu_fine_clip->x)
        {
            u1 = u1 + (1.0f - (x2 - cpu_fine_clip->x) / (x2 - x1)) * (u2 - u1);
            x1 = cpu_fine_clip->x;
        }
        if (y1 < cpu_fine_clip->y)
        {
            v1 = v1 + (1.0f - (y2 - cpu_fine_clip->y) / (y2 - y1)) * (v2 - v1);
            y1 = cpu_fine_clip->y;
        }
        if (x2 > cpu_fine_clip->z)
        {
            u2 = u1 + ((cpu_fine_clip->z - x1) / (x2 - x1)) * (u2 - u1);
            x2 = cpu_fine_clip->z;
        }
        if (y2 > cpu_fine_clip->w)
        {
            v2 = v1 + ((cpu_fine_clip->w - y1) / (y2 - y1)) * (v2 - v1);
            y2 = cpu_fine_clip->w;
        }
        if (y1 >= y2)
            return;
    }
    draw_list->PrimReserve(6, 4);
    draw_list->PrimRectUV(AzVec2(x1, y1),
                          AzVec2(x2, y2),
                          AzVec2(u1, v1),
                          AzVec2(u2, v2),
                          col);
}

void AzFont::RenderText(AzDrawList* draw_list,
                        float size,
                        const AzVec2& pos,
                        AzU32 col,
                        const AzVec4& clip_rect,
                        const char* text_begin,
                        const char* text_end,
                        float wrap_width,
                        bool cpu_fine_clip)
{
begin:
    float x = AZ_TRUNC(pos.x);
    float y = AZ_TRUNC(pos.y);
    if (y > clip_rect.w)
        return;

    if (!text_end)
        text_end = text_begin + AzStrlen(text_begin);

    const float line_height = size;
    AzFontBaked* baked = GetFontBaked(size);

    const float scale = size / baked->Size;
    const float origin_x = x;
    const bool word_wrap_enabled = (wrap_width > 0.0f);

    const char* s = text_begin;
    if (y + line_height < clip_rect.y)
        while (y + line_height < clip_rect.y && s < text_end)
        {
            const char* line_end = (const char*)AzMemchr(s, '\n', text_end - s);
            if (word_wrap_enabled)
            {
                s = CalcWordWrapPosition(size,
                                         s,
                                         line_end ? line_end : text_end,
                                         wrap_width);
                s = CalcWordWrapNextLineStartA(s, text_end);
            } else
            {
                s = line_end ? line_end + 1 : text_end;
            }
            y += line_height;
        }

    if (text_end - s > 10000 && !word_wrap_enabled)
    {
        const char* s_end = s;
        float y_end = y;
        while (y_end < clip_rect.w && s_end < text_end)
        {
            s_end = (const char*)AzMemchr(s_end, '\n', text_end - s_end);
            s_end = s_end ? s_end + 1 : text_end;
            y_end += line_height;
        }
        text_end = s_end;
    }
    if (s == text_end)
        return;

    const int cmd_count = draw_list->CmdBuffer.Size;
    const int vtx_count_max = (int)(text_end - s) * 4;
    const int idx_count_max = (int)(text_end - s) * 6;
    const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
    draw_list->PrimReserve(idx_count_max, vtx_count_max);
    AzDrawVert* vtx_write = draw_list->_VtxWritePtr;
    AzDrawIdx* idx_write = draw_list->_IdxWritePtr;
    unsigned int vtx_index = draw_list->_VtxCurrentIdx;

    const AzU32 col_untinted = col | ~AZ_COL32_A_MASK;
    const char* word_wrap_eol = NULL;

    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            if (!word_wrap_eol)
                word_wrap_eol = CalcWordWrapPosition(size,
                                                     s,
                                                     text_end,
                                                     wrap_width
                                                             - (x - origin_x));

            if (s >= word_wrap_eol)
            {
                x = origin_x;
                y += line_height;
                if (y > clip_rect.w)
                    break;
                word_wrap_eol = NULL;
                s = CalcWordWrapNextLineStartA(s, text_end);
                continue;
            }
        }

        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
            s += 1;
        else
            s += AzTextCharFromUtf8(&c, s, text_end);

        if (c < 32)
        {
            if (c == '\n')
            {
                x = origin_x;
                y += line_height;
                if (y > clip_rect.w)
                    break;
                continue;
            }
            if (c == '\r')
                continue;
        }

        const AzFontGlyph* glyph = baked->FindGlyph((AzWchar)c);
        if (glyph == NULL)
            continue;

        float char_width = glyph->AdvanceX * scale;
        if (glyph->Visible)
        {
            float x1 = x + glyph->X0 * scale;
            float x2 = x + glyph->X1 * scale;
            float y1 = y + glyph->Y0 * scale;
            float y2 = y + glyph->Y1 * scale;
            if (x1 <= clip_rect.z && x2 >= clip_rect.x)
            {
                float u1 = glyph->U0;
                float v1 = glyph->V0;
                float u2 = glyph->U1;
                float v2 = glyph->V1;

                if (cpu_fine_clip)
                {
                    if (x1 < clip_rect.x)
                    {
                        u1 = u1
                             + (1.0f - (x2 - clip_rect.x) / (x2 - x1))
                                       * (u2 - u1);
                        x1 = clip_rect.x;
                    }
                    if (y1 < clip_rect.y)
                    {
                        v1 = v1
                             + (1.0f - (y2 - clip_rect.y) / (y2 - y1))
                                       * (v2 - v1);
                        y1 = clip_rect.y;
                    }
                    if (x2 > clip_rect.z)
                    {
                        u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
                        x2 = clip_rect.z;
                    }
                    if (y2 > clip_rect.w)
                    {
                        v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
                        y2 = clip_rect.w;
                    }
                    if (y1 >= y2)
                    {
                        x += char_width;
                        continue;
                    }
                }

                AzU32 glyph_col = glyph->Colored ? col_untinted : col;

                {
                    vtx_write[0].pos.x = x1;
                    vtx_write[0].pos.y = y1;
                    vtx_write[0].col = glyph_col;
                    vtx_write[0].uv.x = u1;
                    vtx_write[0].uv.y = v1;
                    vtx_write[1].pos.x = x2;
                    vtx_write[1].pos.y = y1;
                    vtx_write[1].col = glyph_col;
                    vtx_write[1].uv.x = u2;
                    vtx_write[1].uv.y = v1;
                    vtx_write[2].pos.x = x2;
                    vtx_write[2].pos.y = y2;
                    vtx_write[2].col = glyph_col;
                    vtx_write[2].uv.x = u2;
                    vtx_write[2].uv.y = v2;
                    vtx_write[3].pos.x = x1;
                    vtx_write[3].pos.y = y2;
                    vtx_write[3].col = glyph_col;
                    vtx_write[3].uv.x = u1;
                    vtx_write[3].uv.y = v2;
                    idx_write[0] = (AzDrawIdx)(vtx_index);
                    idx_write[1] = (AzDrawIdx)(vtx_index + 1);
                    idx_write[2] = (AzDrawIdx)(vtx_index + 2);
                    idx_write[3] = (AzDrawIdx)(vtx_index);
                    idx_write[4] = (AzDrawIdx)(vtx_index + 2);
                    idx_write[5] = (AzDrawIdx)(vtx_index + 3);
                    vtx_write += 4;
                    vtx_index += 4;
                    idx_write += 6;
                }
            }
        }
        x += char_width;
    }

    if (cmd_count != draw_list->CmdBuffer.Size)
    {
        AZ_ASSERT(draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ElemCount
                  == 0);
        draw_list->CmdBuffer.pop_back();
        draw_list->PrimUnreserve(idx_count_max, vtx_count_max);
        draw_list->AddDrawCmd();
        goto begin;
    }

    draw_list->VtxBuffer.Size = (int)(vtx_write - draw_list->VtxBuffer.Data);
    draw_list->IdxBuffer.Size = (int)(idx_write - draw_list->IdxBuffer.Data);
    draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ElemCount -=
            (idx_expected_size - draw_list->IdxBuffer.Size);
    draw_list->_VtxWritePtr = vtx_write;
    draw_list->_IdxWritePtr = idx_write;
    draw_list->_VtxCurrentIdx = vtx_index;
}

void Az::RenderArrow(AzDrawList* draw_list,
                     AzVec2 pos,
                     AzU32 col,
                     AzDir dir,
                     float scale)
{
    const float h = draw_list->_Data->FontSize * 1.00f;
    float r = h * 0.40f * scale;
    AzVec2 center = pos + AzVec2(h * 0.50f, h * 0.50f * scale);

    AzVec2 a, b, c;
    switch (dir)
    {
    case AzDir_Up:
    case AzDir_Down:
        if (dir == AzDir_Up)
            r = -r;
        a = AzVec2(+0.000f, +0.750f) * r;
        b = AzVec2(-0.866f, -0.750f) * r;
        c = AzVec2(+0.866f, -0.750f) * r;
        break;
    case AzDir_Left:
    case AzDir_Right:
        if (dir == AzDir_Left)
            r = -r;
        a = AzVec2(+0.750f, +0.000f) * r;
        b = AzVec2(-0.750f, +0.866f) * r;
        c = AzVec2(-0.750f, -0.866f) * r;
        break;
    case AzDir_None:
    case AzDir_COUNT:
        AZ_ASSERT(0);
        break;
    }
    draw_list->AddTriangleFilled(center + a, center + b, center + c, col);
}

void Az::RenderBullet(AzDrawList* draw_list, AzVec2 pos, AzU32 col)
{
    draw_list->AddCircleFilled(pos, draw_list->_Data->FontSize * 0.20f, col, 8);
}

void Az::RenderCheckMark(AzDrawList* draw_list, AzVec2 pos, AzU32 col, float sz)
{
    float thickness = AzMax(sz / 5.0f, 1.0f);
    sz -= thickness * 0.5f;
    pos += AzVec2(thickness * 0.25f, thickness * 0.25f);

    float third = sz / 3.0f;
    float bx = pos.x + third;
    float by = pos.y + sz - third * 0.5f;
    draw_list->PathLineTo(AzVec2(bx - third, by - third));
    draw_list->PathLineTo(AzVec2(bx, by));
    draw_list->PathLineTo(AzVec2(bx + third * 2.0f, by - third * 2.0f));
    draw_list->PathStroke(col, 0, thickness);
}

void Az::RenderArrowPointingAt(AzDrawList* draw_list,
                               AzVec2 pos,
                               AzVec2 half_sz,
                               AzDir direction,
                               AzU32 col)
{
    switch (direction)
    {
    case AzDir_Left:
        draw_list->AddTriangleFilled(AzVec2(pos.x + half_sz.x,
                                            pos.y - half_sz.y),
                                     AzVec2(pos.x + half_sz.x,
                                            pos.y + half_sz.y),
                                     pos,
                                     col);
        return;
    case AzDir_Right:
        draw_list->AddTriangleFilled(AzVec2(pos.x - half_sz.x,
                                            pos.y + half_sz.y),
                                     AzVec2(pos.x - half_sz.x,
                                            pos.y - half_sz.y),
                                     pos,
                                     col);
        return;
    case AzDir_Up:
        draw_list->AddTriangleFilled(AzVec2(pos.x + half_sz.x,
                                            pos.y + half_sz.y),
                                     AzVec2(pos.x - half_sz.x,
                                            pos.y + half_sz.y),
                                     pos,
                                     col);
        return;
    case AzDir_Down:
        draw_list->AddTriangleFilled(AzVec2(pos.x - half_sz.x,
                                            pos.y - half_sz.y),
                                     AzVec2(pos.x + half_sz.x,
                                            pos.y - half_sz.y),
                                     pos,
                                     col);
        return;
    case AzDir_None:
    case AzDir_COUNT:
        break;
    }
}

void Az::RenderArrowDockMenu(AzDrawList* draw_list,
                             AzVec2 p_min,
                             float sz,
                             AzU32 col)
{
    draw_list->AddRectFilled(p_min + AzVec2(sz * 0.20f, sz * 0.15f),
                             p_min + AzVec2(sz * 0.80f, sz * 0.30f),
                             col);
    RenderArrowPointingAt(draw_list,
                          p_min + AzVec2(sz * 0.50f, sz * 0.85f),
                          AzVec2(sz * 0.30f, sz * 0.40f),
                          AzDir_Down,
                          col);
}

static inline float AzAcos01(float x)
{
    if (x <= 0.0f)
        return AZ_PI * 0.5f;
    if (x >= 1.0f)
        return 0.0f;
    return AzAcos(x);
}

void Az::RenderRectFilledRangeH(AzDrawList* draw_list,
                                const AzRect& rect,
                                AzU32 col,
                                float x_start_norm,
                                float x_end_norm,
                                float rounding)
{
    if (x_end_norm == x_start_norm)
        return;
    if (x_start_norm > x_end_norm)
        AzSwap(x_start_norm, x_end_norm);

    AzVec2 p0 = AzVec2(AzLerp(rect.Min.x, rect.Max.x, x_start_norm),
                       rect.Min.y);
    AzVec2 p1 = AzVec2(AzLerp(rect.Min.x, rect.Max.x, x_end_norm), rect.Max.y);
    if (rounding == 0.0f)
    {
        draw_list->AddRectFilled(p0, p1, col, 0.0f);
        return;
    }

    rounding = AzClamp(AzMin((rect.Max.x - rect.Min.x) * 0.5f,
                             (rect.Max.y - rect.Min.y) * 0.5f)
                               - 1.0f,
                       0.0f,
                       rounding);
    const float inv_rounding = 1.0f / rounding;
    const float arc0_b = AzAcos01(1.0f - (p0.x - rect.Min.x) * inv_rounding);
    const float arc0_e = AzAcos01(1.0f - (p1.x - rect.Min.x) * inv_rounding);
    const float half_pi = AZ_PI * 0.5f;
    const float x0 = AzMax(p0.x, rect.Min.x + rounding);
    if (arc0_b == arc0_e)
    {
        draw_list->PathLineTo(AzVec2(x0, p1.y));
        draw_list->PathLineTo(AzVec2(x0, p0.y));
    } else if (arc0_b == 0.0f && arc0_e == half_pi)
    {
        draw_list->PathArcToFast(AzVec2(x0, p1.y - rounding), rounding, 3, 6);
        draw_list->PathArcToFast(AzVec2(x0, p0.y + rounding), rounding, 6, 9);
    } else
    {
        draw_list->PathArcTo(AzVec2(x0, p1.y - rounding),
                             rounding,
                             AZ_PI - arc0_e,
                             AZ_PI - arc0_b);
        draw_list->PathArcTo(AzVec2(x0, p0.y + rounding),
                             rounding,
                             AZ_PI + arc0_b,
                             AZ_PI + arc0_e);
    }
    if (p1.x > rect.Min.x + rounding)
    {
        const float arc1_b = AzAcos01(1.0f
                                      - (rect.Max.x - p1.x) * inv_rounding);
        const float arc1_e = AzAcos01(1.0f
                                      - (rect.Max.x - p0.x) * inv_rounding);
        const float x1 = AzMin(p1.x, rect.Max.x - rounding);
        if (arc1_b == arc1_e)
        {
            draw_list->PathLineTo(AzVec2(x1, p0.y));
            draw_list->PathLineTo(AzVec2(x1, p1.y));
        } else if (arc1_b == 0.0f && arc1_e == half_pi)
        {
            draw_list->PathArcToFast(AzVec2(x1, p0.y + rounding),
                                     rounding,
                                     9,
                                     12);
            draw_list->PathArcToFast(AzVec2(x1, p1.y - rounding),
                                     rounding,
                                     0,
                                     3);
        } else
        {
            draw_list->PathArcTo(AzVec2(x1, p0.y + rounding),
                                 rounding,
                                 -arc1_e,
                                 -arc1_b);
            draw_list->PathArcTo(AzVec2(x1, p1.y - rounding),
                                 rounding,
                                 +arc1_b,
                                 +arc1_e);
        }
    }
    draw_list->PathFillConvex(col);
}

void Az::RenderRectFilledWithHole(AzDrawList* draw_list,
                                  const AzRect& outer,
                                  const AzRect& inner,
                                  AzU32 col,
                                  float rounding)
{
    const bool fill_L = (inner.Min.x > outer.Min.x);
    const bool fill_R = (inner.Max.x < outer.Max.x);
    const bool fill_U = (inner.Min.y > outer.Min.y);
    const bool fill_D = (inner.Max.y < outer.Max.y);
    if (fill_L)
        draw_list->AddRectFilled(
                AzVec2(outer.Min.x, inner.Min.y),
                AzVec2(inner.Min.x, inner.Max.y),
                col,
                rounding,
                AzDrawFlags_RoundCornersNone
                        | (fill_U ? 0 : AzDrawFlags_RoundCornersTopLeft)
                        | (fill_D ? 0 : AzDrawFlags_RoundCornersBottomLeft));
    if (fill_R)
        draw_list->AddRectFilled(
                AzVec2(inner.Max.x, inner.Min.y),
                AzVec2(outer.Max.x, inner.Max.y),
                col,
                rounding,
                AzDrawFlags_RoundCornersNone
                        | (fill_U ? 0 : AzDrawFlags_RoundCornersTopRight)
                        | (fill_D ? 0 : AzDrawFlags_RoundCornersBottomRight));
    if (fill_U)
        draw_list->AddRectFilled(
                AzVec2(inner.Min.x, outer.Min.y),
                AzVec2(inner.Max.x, inner.Min.y),
                col,
                rounding,
                AzDrawFlags_RoundCornersNone
                        | (fill_L ? 0 : AzDrawFlags_RoundCornersTopLeft)
                        | (fill_R ? 0 : AzDrawFlags_RoundCornersTopRight));
    if (fill_D)
        draw_list->AddRectFilled(
                AzVec2(inner.Min.x, inner.Max.y),
                AzVec2(inner.Max.x, outer.Max.y),
                col,
                rounding,
                AzDrawFlags_RoundCornersNone
                        | (fill_L ? 0 : AzDrawFlags_RoundCornersBottomLeft)
                        | (fill_R ? 0 : AzDrawFlags_RoundCornersBottomRight));
    if (fill_L && fill_U)
        draw_list->AddRectFilled(AzVec2(outer.Min.x, outer.Min.y),
                                 AzVec2(inner.Min.x, inner.Min.y),
                                 col,
                                 rounding,
                                 AzDrawFlags_RoundCornersTopLeft);
    if (fill_R && fill_U)
        draw_list->AddRectFilled(AzVec2(inner.Max.x, outer.Min.y),
                                 AzVec2(outer.Max.x, inner.Min.y),
                                 col,
                                 rounding,
                                 AzDrawFlags_RoundCornersTopRight);
    if (fill_L && fill_D)
        draw_list->AddRectFilled(AzVec2(outer.Min.x, inner.Max.y),
                                 AzVec2(inner.Min.x, outer.Max.y),
                                 col,
                                 rounding,
                                 AzDrawFlags_RoundCornersBottomLeft);
    if (fill_R && fill_D)
        draw_list->AddRectFilled(AzVec2(inner.Max.x, inner.Max.y),
                                 AzVec2(outer.Max.x, outer.Max.y),
                                 col,
                                 rounding,
                                 AzDrawFlags_RoundCornersBottomRight);
}

AzDrawFlags Az::CalcRoundingFlagsForRectInRect(const AzRect& r_in,
                                               const AzRect& r_outer,
                                               float threshold)
{
    bool round_l = r_in.Min.x <= r_outer.Min.x + threshold;
    bool round_r = r_in.Max.x >= r_outer.Max.x - threshold;
    bool round_t = r_in.Min.y <= r_outer.Min.y + threshold;
    bool round_b = r_in.Max.y >= r_outer.Max.y - threshold;
    return AzDrawFlags_RoundCornersNone
           | ((round_t && round_l) ? AzDrawFlags_RoundCornersTopLeft : 0)
           | ((round_t && round_r) ? AzDrawFlags_RoundCornersTopRight : 0)
           | ((round_b && round_l) ? AzDrawFlags_RoundCornersBottomLeft : 0)
           | ((round_b && round_r) ? AzDrawFlags_RoundCornersBottomRight : 0);
}

void Az::RenderColorRectWithAlphaCheckerboard(AzDrawList* draw_list,
                                              AzVec2 p_min,
                                              AzVec2 p_max,
                                              AzU32 col,
                                              float grid_step,
                                              AzVec2 grid_off,
                                              float rounding,
                                              AzDrawFlags flags)
{
    if ((flags & AzDrawFlags_RoundCornersMask_) == 0)
        flags = AzDrawFlags_RoundCornersDefault_;
    if (((col & AZ_COL32_A_MASK) >> AZ_COL32_A_SHIFT) < 0xFF)
    {
        AzU32 col_bg1 = GetColorU32(
                AzAlphaBlendColors(AZ_COL32(204, 204, 204, 255), col));
        AzU32 col_bg2 = GetColorU32(
                AzAlphaBlendColors(AZ_COL32(128, 128, 128, 255), col));
        draw_list->AddRectFilled(p_min, p_max, col_bg1, rounding, flags);

        int yi = 0;
        for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
        {
            float y1 = AzClamp(y, p_min.y, p_max.y),
                  y2 = AzMin(y + grid_step, p_max.y);
            if (y2 <= y1)
                continue;
            for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step;
                 x < p_max.x;
                 x += grid_step * 2.0f)
            {
                float x1 = AzClamp(x, p_min.x, p_max.x),
                      x2 = AzMin(x + grid_step, p_max.x);
                if (x2 <= x1)
                    continue;
                AzDrawFlags cell_flags = AzDrawFlags_RoundCornersNone;
                if (y1 <= p_min.y)
                {
                    if (x1 <= p_min.x)
                        cell_flags |= AzDrawFlags_RoundCornersTopLeft;
                    if (x2 >= p_max.x)
                        cell_flags |= AzDrawFlags_RoundCornersTopRight;
                }
                if (y2 >= p_max.y)
                {
                    if (x1 <= p_min.x)
                        cell_flags |= AzDrawFlags_RoundCornersBottomLeft;
                    if (x2 >= p_max.x)
                        cell_flags |= AzDrawFlags_RoundCornersBottomRight;
                }

                cell_flags = (flags == AzDrawFlags_RoundCornersNone
                              || cell_flags == AzDrawFlags_RoundCornersNone)
                                     ? AzDrawFlags_RoundCornersNone
                                     : (cell_flags & flags);
                draw_list->AddRectFilled(AzVec2(x1, y1),
                                         AzVec2(x2, y2),
                                         col_bg2,
                                         rounding,
                                         cell_flags);
            }
        }
    } else
    {
        draw_list->AddRectFilled(p_min, p_max, col, rounding, flags);
    }
}

static unsigned int stb_decompress_length(const unsigned char* input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char *stb__barrier_out_e, *stb__barrier_out_b;
static const unsigned char* stb__barrier_in_b;
static unsigned char* stb__dout;
static void stb__match(const unsigned char* data, unsigned int length)
{
    AZ_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e)
    {
        stb__dout += length;
        return;
    }
    if (data < stb__barrier_out_b)
    {
        stb__dout = stb__barrier_out_e + 1;
        return;
    }
    while (length--)
        *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char* data, unsigned int length)
{
    AZ_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e)
    {
        stb__dout += length;
        return;
    }
    if (data < stb__barrier_in_b)
    {
        stb__dout = stb__barrier_out_e + 1;
        return;
    }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x) ((i[x] << 8) + i[(x) + 1])
#define stb__in3(x) ((i[x] << 16) + stb__in2((x) + 1))
#define stb__in4(x) ((i[x] << 24) + stb__in3((x) + 1))

static const unsigned char* stb_decompress_token(const unsigned char* i)
{
    if (*i >= 0x20)
    {
        if (*i >= 0x80)
            stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)
            stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1),
                    i += 3;
        else
            stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    } else
    {
        if (*i >= 0x18)
            stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1),
                    i += 4;
        else if (*i >= 0x10)
            stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1),
                       stb__in2(3) + 1),
                    i += 5;
        else if (*i >= 0x08)
            stb__lit(i + 2, stb__in2(0) - 0x0800 + 1),
                    i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)
            stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)
            stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
        else if (*i == 0x04)
            stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32,
                                unsigned char* buffer,
                                unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen = buflen % 5552;

    unsigned long i;
    while (buflen)
    {
        for (i = 0; i + 7 < blocklen; i += 8)
        {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char* output,
                                   const unsigned char* i,
                                   unsigned int)
{
    if (stb__in4(0) != 0x57bC0000)
        return 0;
    if (stb__in4(4) != 0)
        return 0;
    const unsigned int olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    for (;;)
    {
        const unsigned char* old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i)
        {
            if (*i == 0x05 && i[1] == 0xfa)
            {
                AZ_ASSERT(stb__dout == output + olen);
                if (stb__dout != output + olen)
                    return 0;
                if (stb_adler32(1, output, olen) != (unsigned int)stb__in4(2))
                    return 0;
                return olen;
            } else
            {
                AZ_ASSERT(0);
                return 0;
            }
        }
        AZ_ASSERT(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

#ifndef AZ_DISABLE_DEFAULT_FONT

static const unsigned int lsans_ttf_compressed_size = 52828;
static const unsigned char  lsans_ttf_compressed_data[52828] = {
        87,  188, 0,   0,   0,   0,   0,   0,   0,   0,   252, 96,  0,   4,
        0,   0,   37,  0,   1,   0,   0,   0,   17,  130, 4,   62,  4,   0,
        16,  68,  83,  73,  71,  179, 204, 180, 251, 0,   0,   231, 96,  0,
        0,   21,  0,   76,  84,  83,  72,  124, 26,  76,  210, 0,   0,   13,
        196, 130, 37,  44,  247, 79,  83,  47,  50,  106, 156, 123, 110, 0,
        0,   1,   152, 130, 15,  8,   60,  86,  86,  68,  77,  88,  111, 25,
        119, 164, 0,   0,   14,  188, 0,   0,   5,   224, 99,  109, 97,  112,
        11,  80,  200, 209, 0,   0,   8,   168, 0,   0,   3,   52,  99,  118,
        116, 32,  206, 79,  169, 234, 0,   0,   30,  212, 0,   0,   4,   192,
        102, 112, 103, 109, 223, 223, 75,  61,  0,   0,   26,  208, 130, 15,
        59,  3,   103, 108, 121, 102, 116, 101, 253, 26,  0,   0,   69,  112,
        0,   0,   159, 212, 104, 100, 109, 120, 50,  134, 241, 70,  0,   0,
        39,  130, 127, 41,  30,  16,  104, 101, 97,  100, 188, 201, 220, 79,
        130, 111, 32,  28,  130, 111, 33,  54,  104, 130, 16,  35,  13,  98,
        5,   13,  130, 15,  32,  84,  130, 15,  44,  36,  104, 109, 116, 120,
        92,  124, 124, 248, 0,   0,   35,  148, 130, 111, 44,  204, 108, 111,
        99,  97,  211, 158, 253, 125, 0,   0,   11,  220, 130, 35,  40,  232,
        109, 97,  120, 112, 3,   16,  6,   225, 130, 11,  32,  120, 130, 47,
        40,  32,  110, 97,  109, 101, 46,  246, 152, 134, 130, 15,  8,   32,
        240, 0,   0,   6,   181, 112, 111, 115, 116, 183, 156, 136, 10,  0,
        0,   229, 68,  0,   0,   2,   28,  112, 114, 101, 112, 182, 245, 45,
        201, 0,   0,   20,  156, 130, 31,  35,  52,  0,   1,   0,   130, 42,
        45,  0,   196, 18,  16,  144, 228, 95,  15,  60,  245, 0,   27,  8,
        0,   131, 0,   35,  163, 166, 147, 66,  131, 7,   43,  180, 81,  253,
        143, 253, 151, 254, 80,  8,   12,  7,   108, 130, 15,  32,  9,   132,
        47,  130, 8,   131, 7,   45,  7,   188, 254, 80,  0,   0,   8,   25,
        253, 151, 253, 176, 8,   12,  136, 25,  32,  0,   134, 0,   32,  243,
        132, 17,  54,  243, 0,   77,  0,   7,   0,   74,  0,   4,   0,   2,
        0,   16,  0,   19,  0,   56,  0,   0,   1,   199, 6,   52,  130, 13,
        50,  1,   0,   1,   3,   235, 1,   144, 0,   5,   0,   8,   4,   206,
        4,   41,  0,   0,   3,   22,  135, 7,   47,  0,   100, 3,   32,  8,
        2,   2,   11,  6,   2,   3,   5,   4,   2,   2,   4,   130, 81,  32,
        3,   130, 3,   136, 2,   53,  66,  38,  72,  32,  0,   64,  0,   32,
        240, 2,   6,   43,  1,   164, 0,   49,  7,   108, 1,   176, 32,  0,
        138, 129, 34,  40,  1,   230, 136, 13,  32,  122, 130, 54,  32,  1,
        130, 3,   131, 31,  34,  11,  0,   122, 134, 23,  32,  2,   130, 149,
        32,  133, 134, 11,  36,  3,   0,   30,  0,   140, 134, 11,  32,  4,
        130, 163, 32,  170, 134, 11,  36,  5,   0,   12,  0,   189, 134, 11,
        36,  6,   0,   10,  0,   201, 134, 11,  42,  7,   0,   58,  0,   211,
        0,   3,   0,   1,   4,   6,   130, 199, 34,  12,  1,   13,  134, 11,
        36,  4,   0,   22,  1,   25,  132, 11,  32,  7,   130, 23,  34,  16,
        1,   47,  134, 11,  131, 23,  32,  63,  132, 11,  32,  9,   130, 135,
        34,  244, 1,   85,  134, 11,  36,  1,   0,   22,  2,   73,  134, 11,
        36,  2,   0,   14,  2,   95,  134, 11,  36,  3,   0,   60,  2,   109,
        134, 11,  36,  4,   0,   38,  2,   169, 134, 11,  36,  5,   0,   24,
        2,   207, 134, 11,  36,  6,   0,   20,  2,   231, 134, 11,  36,  7,
        0,   116, 2,   251, 132, 11,  32,  10,  130, 119, 34,  12,  3,   111,
        134, 11,  130, 119, 33,  3,   123, 132, 11,  32,  11,  130, 23,  34,
        16,  3,   145, 134, 11,  131, 23,  32,  161, 132, 11,  32,  12,  132,
        47,  32,  183, 134, 11,  131, 23,  32,  195, 132, 11,  32,  16,  130,
        23,  34,  14,  3,   217, 134, 11,  131, 23,  133, 107, 32,  19,  130,
        23,  34,  18,  3,   253, 132, 23,  33,  19,  0,   130, 23,  33,  4,
        15,  132, 11,  32,  20,  130, 23,  34,  12,  4,   37,  134, 11,  131,
        23,  32,  49,  132, 11,  32,  29,  132, 23,  32,  71,  134, 11,  131,
        23,  32,  83,  131, 11,  33,  8,   22,  132, 23,  32,  105, 134, 11,
        131, 23,  32,  117, 131, 11,  32,  12,  132, 191, 33,  4,   139, 134,
        11,  131, 23,  32,  151, 132, 11,  132, 167, 33,  4,   173, 134, 11,
        131, 23,  8,   69,  185, 169, 32,  49,  57,  57,  49,  32,  66,  105,
        103, 101, 108, 111, 119, 32,  38,  32,  72,  111, 108, 109, 101, 115,
        32,  73,  110, 99,  46,  32,  80,  97,  116, 46,  32,  68,  101, 115,
        46,  32,  50,  56,  57,  44,  52,  50,  48,  46,  32,  65,  108, 108,
        32,  82,  105, 103, 104, 116, 115, 32,  82,  101, 115, 101, 114, 118,
        101, 100, 46,  32,  132, 68,  33,  48,  45,  132, 73,  46,  84,  121,
        112, 101, 32,  83,  111, 108, 117, 116, 105, 111, 110, 115, 44,  133,
        72,  147, 53,  49,  76,  117, 99,  105, 100, 97,  32,  83,  97,  110,
        115, 82,  101, 103, 117, 108, 97,  114, 138, 17,  130, 92,  132, 18,
        37,  58,  32,  66,  38,  72,  58,  132, 163, 146, 29,  35,  86,  101,
        114, 115, 130, 101, 36,  32,  49,  46,  54,  55,  133, 30,  131, 77,
        133, 9,   61,  168, 32,  105, 115, 32,  97,  32,  114, 101, 103, 105,
        115, 116, 101, 114, 101, 100, 32,  116, 114, 97,  100, 101, 109, 97,
        114, 107, 32,  111, 102, 149, 240, 53,  0,   110, 0,   111, 0,   114,
        0,   109, 0,   97,  0,   108, 0,   76,  0,   117, 0,   99,  0,   105,
        0,   100, 130, 13,  34,  32,  0,   83,  130, 5,   34,  110, 0,   115,
        130, 7,   32,  116, 132, 9,   131, 21,  32,  114, 130, 27,  149, 37,
        38,  169, 0,   32,  0,   49,  0,   57,  130, 1,   32,  49,  130, 9,
        32,  66,  130, 69,  34,  103, 0,   101, 130, 83,  34,  111, 0,   119,
        130, 15,  32,  38,  130, 3,   32,  72,  130, 105, 32,  108, 130, 105,
        32,  101, 130, 83,  34,  32,  0,   73,  130, 121, 34,  99,  0,   46,
        130, 23,  32,  80,  130, 93,  32,  116, 132, 9,   32,  68,  130, 49,
        32,  115, 132, 9,   34,  50,  0,   56,  130, 73,  38,  44,  0,   52,
        0,   50,  0,   48,  132, 17,  32,  65,  130, 75,  32,  108, 130, 45,
        32,  82,  132, 89,  34,  104, 0,   116, 132, 69,  32,  82,  132, 49,
        32,  101, 130, 193, 32,  118, 130, 9,   32,  100, 132, 41,  137, 137,
        34,  48,  0,   45,  138, 147, 36,  84,  0,   121, 0,   112, 130, 37,
        131, 217, 32,  111, 130, 77,  32,  117, 130, 67,  32,  105, 130, 147,
        32,  110, 130, 73,  32,  44,  130, 89,  137, 145, 167, 107, 65,  9,
        21,  131, 147, 34,  103, 0,   117, 130, 97,  32,  97,  130, 151, 149,
        35,  33,  32,  0,   141, 37,  32,  58,  130, 127, 37,  66,  0,   38,
        0,   72,  0,   131, 9,   65,  71,  7,   165, 59,  32,  86,  130, 211,
        32,  114, 130, 195, 133, 203, 32,  32,  130, 241, 36,  46,  0,   54,
        0,   55,  65,  205, 12,  65,  203, 7,   139, 81,  32,  174, 130, 115,
        32,  105, 130, 55,  34,  32,  0,   97,  130, 9,   32,  114, 130, 69,
        33,  103, 0,   131, 15,  32,  116, 132, 79,  34,  101, 0,   100, 130,
        21,  32,  116, 130, 193, 32,  97,  130, 9,   34,  101, 0,   109, 130,
        37,  34,  114, 0,   107, 130, 19,  34,  111, 0,   102, 65,  225, 44,
        32,  78,  66,  97,  32,  137, 33,  34,  97,  0,   108, 130, 131, 149,
        247, 171, 71,  32,  108, 130, 195, 149, 69,  33,  83,  0,   66,  205,
        9,   32,  97,  130, 221, 32,  100, 65,  39,  12,  66,  245, 9,   173,
        109, 149, 107, 229, 67,  32,  0,   131, 0,   32,  2,   70,  97,  6,
        34,  20,  0,   3,   131, 9,   37,  1,   26,  0,   0,   1,   6,   70,
        216, 9,   33,  1,   3,   130, 36,  33,  2,   0,   141, 0,   8,   230,
        1,   0,   0,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,
        14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,
        28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,
        42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,
        56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
        70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,
        84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,
        0,   98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
        111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
        125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
        139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
        153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166,
        167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
        181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
        195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
        209, 0,   210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
        222, 223, 224, 0,   4,   2,   26,  130, 244, 8,   80,  74,  0,   64,
        0,   5,   0,   10,  0,   126, 0,   174, 0,   182, 0,   255, 1,   49,
        1,   83,  1,   97,  1,   120, 1,   146, 2,   199, 2,   201, 2,   221,
        3,   192, 32,  20,  32,  26,  32,  30,  32,  34,  32,  38,  32,  48,
        32,  58,  32,  172, 33,  34,  33,  38,  34,  2,   34,  6,   34,  15,
        34,  17,  34,  21,  34,  26,  34,  30,  34,  43,  34,  72,  34,  96,
        34,  101, 37,  202, 240, 2,   255, 255, 130, 83,  38,  32,  0,   160,
        0,   176, 0,   184, 130, 75,  34,  82,  1,   96,  132, 75,  32,  198,
        130, 75,  32,  216, 130, 75,  38,  19,  32,  24,  32,  28,  32,  32,
        132, 75,  32,  57,  144, 75,  32,  25,  136, 75,  32,  100, 130, 75,
        36,  1,   255, 255, 255, 227, 130, 77,  130, 2,   51,  255, 165, 255,
        94,  255, 129, 255, 67,  255, 20,  0,   0,   254, 16,  0,   0,   252,
        219, 224, 159, 130, 22,  130, 2,   35,  224, 133, 224, 150, 130, 3,
        8,   32,  17,  223, 106, 223, 121, 222, 150, 222, 162, 222, 139, 222,
        136, 222, 167, 0,   0,   222, 116, 222, 113, 222, 95,  222, 47,  222,
        48,  218, 239, 16,  191, 0,   1,   130, 42,  36,  72,  0,   100, 0,
        112, 130, 7,   135, 2,   32,  244, 130, 8,   131, 3,   37,  0,   0,
        250, 0,   254, 1,   66,  1,   15,  32,  0,   134, 0,   32,  240, 134,
        7,   137, 6,   8,   72,  172, 0,   163, 0,   132, 0,   133, 0,   242,
        0,   150, 0,   227, 0,   134, 0,   142, 0,   139, 0,   157, 0,   169,
        0,   164, 0,   241, 0,   138, 0,   131, 0,   147, 0,   236, 0,   237,
        0,   141, 0,   151, 0,   136, 0,   221, 0,   235, 0,   158, 0,   170,
        0,   239, 0,   238, 0,   240, 0,   162, 0,   173, 0,   201, 0,   199,
        0,   174, 0,   98,  0,   99,  0,   144, 130, 143, 8,   64,  203, 0,
        101, 0,   200, 0,   202, 0,   207, 0,   204, 0,   205, 0,   206, 0,
        228, 0,   102, 0,   210, 0,   208, 0,   209, 0,   175, 0,   103, 0,
        234, 0,   145, 0,   213, 0,   211, 0,   212, 0,   104, 0,   230, 0,
        232, 0,   137, 0,   106, 0,   105, 0,   107, 0,   109, 0,   108, 0,
        110, 0,   160, 0,   111, 0,   113, 130, 209, 8,   84,  114, 0,   115,
        0,   117, 0,   116, 0,   118, 0,   119, 0,   229, 0,   120, 0,   122,
        0,   121, 0,   123, 0,   125, 0,   124, 0,   184, 0,   161, 0,   127,
        0,   126, 0,   128, 0,   129, 0,   231, 0,   233, 0,   186, 0,   215,
        0,   224, 0,   218, 0,   219, 0,   220, 0,   223, 0,   216, 0,   222,
        0,   182, 0,   183, 0,   196, 0,   180, 0,   181, 0,   197, 0,   130,
        0,   194, 0,   135, 0,   195, 0,   165, 0,   0,   0,   64,  134, 1,
        9,   78,  124, 0,   193, 1,   150, 2,   58,  3,   4,   3,   209, 3,
        247, 4,   67,  4,   143, 5,   10,  5,   56,  5,   112, 5,   139, 5,
        169, 5,   218, 6,   69,  6,   111, 6,   223, 7,   124, 7,   223, 8,
        73,  8,   248, 9,   53,  9,   228, 10,  148, 10,  192, 11,  14,  11,
        80,  11,  124, 11,  193, 12,  45,  12,  240, 13,  100, 13,  239, 14,
        84,  14,  171, 14,  233, 15,  35,  15,  141, 15,  202, 15,  237, 16,
        46,  16,  145, 16,  184, 17,  90,  17,  173, 18,  24,  18,  111, 18,
        247, 19,  121, 20,  19,  20,  66,  20,  156, 20,  248, 21,  151, 22,
        35,  22,  128, 22,  199, 22,  253, 23,  45,  23,  102, 23,  171, 23,
        200, 23,  242, 24,  154, 25,  44,  25,  153, 26,  42,  26,  168, 27,
        5,   27,  211, 28,  57,  28,  112, 28,  207, 29,  55,  29,  94,  30,
        15,  30,  122, 30,  229, 31,  120, 32,  20,  32,  104, 33,  12,  33,
        108, 33,  213, 34,  64,  34,  236, 35,  108, 35,  237, 36,  71,  36,
        188, 36,  226, 37,  96,  37,  181, 37,  205, 38,  127, 39,  42,  39,
        64,  39,  86,  39,  110, 39,  134, 39,  155, 39,  176, 39,  197, 39,
        220, 39,  241, 40,  8,   40,  176, 40,  197, 40,  218, 40,  239, 41,
        6,   41,  28,  41,  50,  41,  72,  41,  96,  41,  117, 41,  138, 41,
        159, 41,  180, 41,  203, 41,  224, 41,  245, 42,  10,  42,  31,  42,
        54,  42,  151, 42,  251, 43,  114, 43,  221, 44,  183, 44,  230, 45,
        44,  45,  219, 46,  131, 47,  68,  47,  207, 47,  244, 48,  33,  48,
        134, 48,  249, 49,  187, 50,  101, 50,  161, 50,  231, 51,  48,  51,
        181, 52,  2,   52,  174, 53,  5,   53,  61,  53,  153, 54,  6,   54,
        136, 54,  208, 55,  76,  56,  48,  56,  226, 57,  84,  57,  149, 57,
        181, 58,  30,  58,  128, 59,  38,  59,  121, 59,  251, 60,  124, 60,
        183, 130, 1,   8,   161, 205, 60,  227, 60,  249, 61,  159, 62,  92,
        62,  121, 62,  150, 62,  241, 63,  83,  63,  144, 63,  208, 63,  254,
        64,  102, 64,  125, 64,  149, 64,  199, 65,  119, 65,  202, 66,  27,
        66,  132, 66,  238, 67,  128, 67,  157, 67,  211, 68,  49,  69,  67,
        69,  89,  69,  111, 69,  133, 69,  157, 69,  179, 69,  201, 69,  223,
        69,  247, 70,  13,  70,  35,  70,  57,  70,  79,  70,  101, 70,  123,
        70,  145, 70,  185, 70,  237, 71,  73,  71,  104, 71,  154, 71,  183,
        71,  254, 72,  85,  72,  152, 72,  222, 73,  16,  73,  38,  73,  59,
        73,  110, 73,  228, 74,  155, 74,  177, 74,  193, 75,  34,  75,  191,
        76,  30,  76,  76,  76,  161, 77,  15,  77,  164, 78,  56,  79,  10,
        79,  37,  79,  234, 0,   0,   0,   243, 48,  1,   1,   1,   224, 255,
        255, 1,   228, 254, 255, 162, 239, 179, 1,   253, 255, 86,  50,  130,
        17,  134, 2,   33,  49,  206, 130, 8,   8,   70,  252, 250, 196, 255,
        1,   1,   219, 255, 1,   219, 221, 243, 219, 244, 245, 225, 1,   254,
        1,   254, 241, 253, 251, 248, 254, 247, 161, 1,   255, 255, 85,  248,
        255, 31,  255, 50,  253, 49,  254, 255, 255, 187, 253, 48,  255, 253,
        248, 187, 50,  50,  49,  187, 45,  255, 255, 218, 253, 255, 255, 255,
        249, 244, 254, 1,   196, 196, 1,   219, 225, 1,   251, 130, 13,  130,
        2,   33,  253, 254, 130, 0,   32,  253, 130, 0,   130, 38,  34,  50,
        50,  50,  130, 17,  33,  255, 233, 130, 93,  44,  250, 46,  250, 253,
        255, 255, 250, 255, 50,  1,   50,  1,   248, 130, 115, 37,  240, 253,
        255, 1,   255, 254, 132, 50,  42,  50,  255, 49,  1,   245, 182, 1,
        249, 254, 191, 255, 131, 74,  33,  50,  247, 131, 52,  54,  86,  86,
        1,   50,  255, 161, 252, 16,  244, 250, 255, 255, 233, 86,  86,  255,
        247, 196, 219, 196, 219, 219, 221, 130, 0,   130, 60,  42,  251, 251,
        251, 253, 255, 49,  50,  49,  49,  225, 44,  130, 59,  49,  241, 45,
        244, 163, 49,  161, 255, 254, 169, 1,   250, 1,   42,  252, 1,   1,
        255, 1,   130, 246, 35,  0,   1,   0,   1,   132, 7,   46,  12,  0,
        248, 8,   255, 0,   8,   0,   9,   255, 253, 0,   9,   0,   10,  130,
        5,   34,  10,  0,   11,  130, 5,   32,  11,  132, 5,   34,  12,  0,
        12,  130, 11,  32,  13,  132, 5,   34,  14,  0,   13,  130, 11,  34,
        15,  0,   14,  130, 5,   34,  16,  0,   15,  130, 5,   32,  17,  130,
        5,   42,  252, 0,   18,  0,   17,  255, 252, 0,   19,  0,   18,  130,
        5,   34,  20,  0,   19,  130, 5,   32,  21,  130, 5,   42,  251, 0,
        22,  0,   20,  255, 251, 0,   23,  0,   21,  130, 5,   34,  24,  0,
        22,  130, 5,   34,  25,  0,   24,  130, 5,   40,  26,  0,   25,  255,
        250, 0,   27,  0,   26,  130, 5,   32,  28,  132, 5,   34,  29,  0,
        27,  130, 11,  34,  30,  0,   28,  130, 5,   32,  31,  130, 5,   42,
        249, 0,   32,  0,   30,  255, 249, 0,   33,  0,   31,  130, 5,   34,
        34,  0,   32,  130, 5,   32,  35,  132, 5,   34,  36,  0,   33,  130,
        11,  40,  37,  0,   34,  255, 248, 0,   38,  0,   35,  130, 5,   34,
        39,  0,   36,  130, 5,   34,  40,  0,   37,  130, 5,   34,  41,  0,
        38,  130, 5,   34,  42,  0,   39,  130, 5,   34,  43,  0,   40,  130,
        5,   40,  44,  0,   41,  255, 247, 0,   45,  0,   43,  130, 5,   32,
        46,  132, 5,   40,  47,  0,   44,  255, 246, 0,   48,  0,   45,  130,
        5,   32,  49,  132, 5,   34,  50,  0,   46,  130, 11,  34,  51,  0,
        47,  130, 5,   38,  52,  0,   49,  255, 245, 0,   53,  132, 5,   34,
        54,  0,   50,  130, 11,  34,  55,  0,   51,  130, 5,   32,  56,  130,
        5,   42,  244, 0,   57,  0,   52,  255, 244, 0,   58,  0,   54,  130,
        5,   34,  59,  0,   56,  130, 5,   32,  60,  132, 5,   34,  61,  0,
        57,  130, 11,  40,  62,  0,   58,  255, 243, 0,   63,  0,   59,  130,
        5,   32,  64,  132, 5,   40,  65,  0,   60,  255, 242, 0,   66,  0,
        62,  130, 5,   32,  67,  132, 5,   34,  68,  0,   63,  130, 11,  34,
        69,  0,   64,  130, 5,   38,  70,  0,   65,  255, 241, 0,   71,  132,
        5,   34,  72,  0,   66,  130, 11,  34,  73,  0,   68,  130, 5,   32,
        74,  132, 5,   34,  75,  0,   70,  130, 11,  40,  76,  0,   71,  255,
        240, 0,   77,  0,   72,  130, 5,   32,  78,  132, 5,   34,  79,  0,
        73,  130, 11,  34,  80,  0,   75,  130, 5,   38,  81,  0,   76,  255,
        239, 0,   82,  132, 5,   34,  83,  0,   77,  130, 11,  34,  84,  0,
        78,  130, 5,   32,  85,  132, 5,   40,  86,  0,   79,  255, 238, 0,
        87,  0,   81,  130, 5,   34,  88,  0,   82,  130, 5,   32,  89,  132,
        5,   34,  90,  0,   83,  130, 11,  38,  91,  0,   85,  255, 237, 0,
        92,  132, 5,   34,  93,  0,   86,  130, 11,  34,  94,  0,   88,  130,
        5,   34,  95,  0,   89,  130, 5,   32,  96,  130, 5,   42,  236, 0,
        97,  0,   90,  255, 236, 0,   98,  0,   91,  130, 5,   34,  99,  0,
        92,  130, 5,   32,  100, 132, 5,   34,  101, 0,   94,  130, 11,  38,
        102, 0,   95,  255, 235, 0,   103, 132, 5,   34,  104, 0,   96,  130,
        11,  34,  105, 0,   97,  130, 5,   34,  106, 0,   98,  130, 5,   32,
        107, 130, 5,   42,  234, 0,   108, 0,   101, 255, 234, 0,   109, 0,
        102, 130, 5,   34,  110, 0,   103, 130, 5,   32,  111, 132, 5,   40,
        112, 0,   104, 255, 233, 0,   113, 0,   105, 130, 5,   32,  114, 132,
        5,   34,  115, 0,   107, 130, 11,  34,  116, 0,   108, 130, 5,   38,
        117, 0,   109, 255, 232, 0,   118, 132, 5,   34,  119, 0,   110, 130,
        11,  34,  120, 0,   111, 130, 5,   32,  121, 132, 5,   40,  122, 0,
        113, 255, 231, 0,   123, 0,   114, 130, 5,   34,  124, 0,   116, 130,
        5,   32,  125, 132, 5,   34,  126, 0,   117, 130, 11,  40,  127, 0,
        118, 255, 230, 0,   128, 0,   120, 130, 5,   32,  129, 132, 5,   34,
        130, 0,   121, 130, 11,  34,  131, 0,   122, 130, 5,   32,  132, 132,
        5,   40,  133, 0,   123, 255, 229, 0,   134, 0,   124, 130, 5,   34,
        135, 0,   126, 130, 5,   32,  136, 132, 5,   34,  137, 0,   127, 130,
        11,  38,  138, 0,   128, 255, 228, 0,   139, 132, 5,   34,  140, 0,
        129, 130, 11,  34,  141, 0,   131, 130, 5,   34,  142, 0,   133, 130,
        5,   32,  143, 130, 5,   42,  227, 0,   144, 0,   134, 255, 227, 0,
        145, 0,   135, 130, 5,   34,  146, 0,   136, 130, 5,   32,  147, 132,
        5,   44,  148, 0,   137, 255, 226, 0,   149, 0,   139, 255, 225, 0,
        150, 132, 5,   34,  151, 0,   140, 130, 11,  34,  152, 0,   141, 130,
        5,   38,  153, 0,   142, 255, 224, 0,   154, 132, 5,   34,  155, 0,
        143, 130, 11,  34,  156, 0,   145, 130, 5,   34,  157, 0,   146, 130,
        5,   34,  158, 0,   147, 130, 5,   40,  159, 0,   148, 255, 223, 0,
        160, 0,   149, 130, 5,   32,  161, 132, 5,   34,  162, 0,   150, 130,
        11,  40,  163, 0,   152, 255, 222, 0,   164, 0,   153, 130, 5,   32,
        165, 132, 5,   34,  166, 0,   154, 130, 11,  34,  167, 0,   155, 130,
        5,   32,  168, 130, 5,   42,  221, 0,   169, 0,   156, 255, 221, 0,
        170, 0,   158, 130, 5,   34,  171, 0,   159, 130, 5,   32,  172, 132,
        5,   40,  173, 0,   160, 255, 220, 0,   174, 0,   162, 130, 5,   32,
        175, 132, 5,   34,  176, 0,   163, 130, 11,  34,  177, 0,   165, 130,
        5,   38,  178, 0,   166, 255, 218, 0,   179, 130, 5,   42,  219, 0,
        180, 0,   167, 255, 219, 0,   181, 0,   168, 130, 5,   34,  182, 0,
        169, 130, 23,  32,  183, 130, 5,   42,  217, 0,   184, 0,   171, 255,
        217, 0,   185, 0,   172, 130, 17,  32,  186, 132, 5,   34,  187, 0,
        173, 130, 17,  40,  188, 0,   174, 255, 216, 0,   189, 0,   175, 130,
        5,   34,  190, 0,   176, 130, 17,  34,  191, 0,   178, 130, 5,   38,
        192, 0,   179, 255, 215, 0,   193, 132, 5,   34,  194, 0,   180, 130,
        11,  34,  195, 0,   181, 130, 35,  34,  196, 0,   182, 130, 11,  32,
        197, 130, 5,   42,  214, 0,   198, 0,   184, 255, 214, 0,   199, 0,
        185, 130, 5,   34,  200, 0,   186, 130, 23,  32,  201, 130, 5,   42,
        214, 0,   202, 0,   187, 255, 213, 0,   203, 0,   188, 130, 5,   32,
        204, 132, 5,   34,  205, 0,   190, 130, 35,  34,  206, 0,   191, 130,
        17,  38,  207, 0,   193, 255, 212, 0,   208, 132, 5,   34,  209, 0,
        194, 130, 11,  34,  210, 0,   195, 130, 23,  40,  211, 0,   196, 255,
        211, 0,   212, 0,   197, 130, 5,   34,  213, 0,   198, 130, 5,   130,
        91,  130, 5,   34,  215, 0,   199, 130, 5,   40,  216, 0,   200, 255,
        210, 0,   217, 0,   201, 130, 5,   34,  218, 0,   203, 130, 5,   32,
        219, 132, 5,   34,  220, 0,   204, 130, 11,  38,  221, 0,   205, 255,
        209, 0,   222, 132, 5,   34,  223, 0,   207, 130, 11,  34,  224, 0,
        208, 130, 5,   34,  225, 0,   210, 130, 5,   32,  226, 130, 5,   42,
        208, 0,   227, 0,   211, 255, 208, 0,   228, 0,   212, 130, 5,   32,
        229, 132, 5,   40,  230, 0,   213, 255, 207, 0,   231, 0,   214, 130,
        5,   34,  232, 0,   216, 130, 5,   32,  233, 132, 5,   34,  234, 0,
        217, 130, 11,  40,  235, 0,   218, 255, 206, 0,   236, 0,   219, 130,
        11,  32,  237, 132, 5,   34,  238, 0,   220, 130, 11,  34,  239, 0,
        222, 130, 23,  40,  240, 0,   223, 255, 205, 0,   241, 0,   224, 130,
        5,   34,  242, 0,   225, 130, 17,  34,  243, 0,   226, 130, 5,   32,
        244, 130, 5,   42,  205, 0,   245, 0,   227, 255, 204, 0,   246, 0,
        229, 130, 5,   34,  247, 0,   230, 130, 35,  32,  248, 132, 5,   40,
        249, 0,   231, 255, 203, 0,   250, 0,   232, 130, 5,   32,  251, 132,
        5,   34,  252, 0,   233, 130, 35,  34,  253, 0,   235, 130, 17,  38,
        254, 0,   236, 255, 202, 0,   255, 131, 5,   8,   76,  64,  54,  63,
        20,  63,  21,  79,  20,  79,  21,  128, 20,  128, 21,  6,   15,  30,
        15,  31,  63,  30,  63,  31,  191, 30,  191, 31,  6,   64,  24,  64,
        25,  144, 24,  144, 25,  192, 24,  192, 25,  6,   31,  26,  31,  27,
        2,   208, 104, 223, 197, 2,   192, 113, 207, 161, 207, 184, 1,   26,
        182, 3,   9,   15,  32,  1,   8,   5,   184, 2,   54,  178, 90,  24,
        31,  184, 2,   53,  133, 6,   32,  92,  133, 6,   34,  91,  178, 91,
        131, 20,  32,  90,  133, 6,   32,  58,  133, 6,   32,  89,  131, 6,
        47,  65,  9,   2,   63,  2,   60,  0,   66,  0,   31,  2,   62,  2,
        61,  0,   42,  130, 7,   35,  61,  178, 91,  87,  130, 65,  35,  60,
        178, 105, 72,  130, 6,   43,  59,  178, 105, 87,  31,  188, 2,   57,
        2,   56,  0,   172, 130, 28,  35,  56,  178, 126, 229, 130, 21,  50,
        55,  64,  13,  90,  72,  31,  0,   70,  70,  0,   0,   0,   18,  17,
        8,   64,  187, 2,   43,  130, 9,   59,  9,   1,   212, 178, 121, 43,
        31,  188, 1,   206, 0,   121, 4,   1,   0,   31,  1,   202, 178, 87,
        86,  31,  184, 1,   200, 178, 87,  82,  130, 6,   32,  137, 131, 28,
        44,  184, 1,   135, 178, 121, 137, 31,  65,  9,   1,   133, 0,   206,
        132, 36,  34,  128, 1,   205, 131, 119, 36,  1,   87,  178, 121, 59,
        130, 37,  35,  85,  178, 121, 103, 130, 6,   42,  76,  178, 87,  71,
        31,  65,  13,  1,   44,  0,   91,  132, 37,  32,  43,  134, 7,   36,
        37,  0,   87,  2,   171, 130, 90,  51,  6,   64,  9,   87,  65,  31,
        248, 121, 90,  31,  244, 206, 184, 4,   1,   178, 31,  242, 91,  187,
        131, 40,  40,  0,   240, 1,   205, 179, 80,  31,  238, 87,  132, 20,
        33,  204, 126, 132, 6,   51,  192, 91,  184, 1,   1,   181, 31,  180,
        121, 86,  31,  173, 185, 1,   205, 4,   1,   182, 31,  166, 130, 11,
        32,  164, 133, 29,  33,  147, 91,  130, 36,  38,  182, 31,  146, 126,
        187, 31,  145, 131, 54,  34,  182, 31,  144, 130, 175, 32,  142, 130,
        51,  43,  154, 178, 31,  138, 126, 184, 2,   171, 178, 31,  137, 89,
        132, 72,  32,  135, 133, 31,  36,  132, 126, 205, 31,  131, 133, 24,
        35,  128, 121, 184, 8,   130, 125, 32,  127, 131, 67,  37,  178, 31,
        125, 126, 184, 1,   130, 52,  32,  123, 133, 38,  32,  107, 130, 70,
        32,  106, 133, 92,  32,  98,  130, 10,  32,  94,  133, 147, 32,  88,
        133, 6,   32,  85,  131, 6,   8,   121, 64,  83,  31,  85,  20,  9,
        20,  9,   144, 225, 7,   144, 220, 7,   144, 183, 7,   144, 143, 7,
        144, 134, 7,   144, 130, 7,   144, 110, 7,   144, 101, 7,   144, 99,
        7,   144, 95,  7,   36,  8,   34,  8,   32,  8,   30,  8,   28,  8,
        26,  8,   24,  8,   22,  8,   20,  8,   18,  8,   16,  8,   14,  8,
        12,  8,   10,  8,   8,   8,   6,   8,   4,   8,   2,   8,   0,   8,
        24,  2,   1,   8,   2,   0,   74,  0,   119, 185, 2,   43,  0,   1,
        1,   115, 75,  176, 192, 99,  0,   75,  98,  32,  176, 246, 83,  35,
        184, 1,   10,  81,  90,  176, 5,   35,  66,  1,   176, 18,  75,  0,
        75,  84,  66,  185, 130, 33,  56,  255, 133, 141, 22,  118, 63,  24,
        63,  18,  75,  83,  88,  185, 0,   20,  255, 224, 64,  42,  0,   0,
        1,   0,   20,  22,  131, 5,   34,  22,  6,   16,  131, 6,   34,  6,
        4,   0,   130, 18,  32,  4,   132, 11,  33,  16,  2,   131, 11,  130,
        4,   130, 17,  37,  0,   62,  17,  57,  70,  68,  146, 4,   32,  96,
        137, 20,  35,  96,  68,  89,  43,  145, 0,   32,  24,  137, 18,  42,
        1,   75,  80,  121, 188, 0,   31,  1,   203, 0,   7,   130, 5,   37,
        138, 179, 7,   31,  129, 7,   130, 29,  33,  75,  83,  130, 21,  32,
        144, 132, 21,  33,  144, 1,   130, 21,  32,  144, 132, 21,  35,  24,
        29,  176, 150, 130, 168, 37,  176, 170, 29,  89,  176, 50,  131, 8,
        51,  255, 29,  89,  75,  176, 147, 83,  32,  92,  88,  185, 2,   45,
        2,   43,  69,  68,  185, 2,   44,  131, 6,   40,  89,  88,  185, 3,
        145, 2,   45,  69,  82,  131, 23,  35,  3,   145, 68,  89,  130, 38,
        32,  158, 132, 38,  35,  0,   90,  2,   44,  130, 38,  33,  0,   121,
        131, 6,   130, 38,  35,  4,   21,  0,   90,  131, 38,  35,  0,   90,
        4,   21,  132, 38,  32,  171, 133, 38,  32,  91,  130, 61,  35,  68,
        185, 0,   105, 131, 6,   131, 38,  34,  33,  0,   91,  132, 38,  34,
        91,  4,   33,  131, 38,  34,  184, 4,   1,   131, 39,  38,  177, 206,
        90,  69,  68,  177, 90,  130, 4,   130, 35,  35,  26,  64,  0,   206,
        132, 35,  34,  206, 26,  64,  132, 35,  32,  8,   132, 35,  34,  185,
        1,   205, 130, 59,  37,  68,  177, 91,  91,  69,  68,  130, 37,  35,
        49,  64,  1,   205, 131, 37,  34,  1,   205, 49,  132, 37,  33,  176,
        70,  132, 72,  33,  87,  87,  130, 72,  32,  126, 130, 4,   130, 34,
        35,  1,   63,  0,   87,  132, 72,  34,  87,  1,   63,  137, 147, 32,
        177, 132, 34,  32,  89,  133, 34,  33,  3,   18,  135, 34,  33,  3,
        18,  143, 34,  32,  119, 148, 34,  65,  133, 17,  155, 17,  46,  101,
        66,  179, 104, 197, 118, 169, 69,  101, 35,  69,  96,  35,  69,  101,
        130, 3,   45,  96,  176, 139, 118, 104, 24,  176, 128, 98,  32,  32,
        177, 118, 197, 131, 24,  39,  32,  176, 3,   38,  96,  98,  99,  104,
        131, 7,   45,  97,  101, 176, 197, 35,  101, 68,  176, 118, 35,  68,
        32,  177, 104, 132, 55,  142, 30,  32,  169, 131, 30,  62,  104, 35,
        68,  177, 0,   169, 69,  84,  88,  177, 169, 64,  101, 68,  178, 104,
        64,  104, 69,  35,  97,  68,  89,  185, 0,   113, 1,   26,  177, 103,
        198, 140, 110, 32,  137, 135, 110, 36,  185, 0,   103, 1,   26,  145,
        112, 34,  184, 1,   26,  131, 82,  32,  103, 131, 113, 32,  113, 132,
        58,  142, 113, 32,  198, 131, 30,  32,  113, 131, 113, 32,  198, 131,
        113, 32,  198, 131, 113, 34,  113, 64,  113, 132, 113, 48,  0,   1,
        69,  105, 83,  66,  1,   75,  80,  88,  177, 8,   0,   66,  89,  67,
        92,  133, 7,   48,  179, 2,   11,  10,  18,  67,  88,  96,  27,  33,
        89,  66,  22,  16,  112, 62,  176, 130, 12,  52,  185, 59,  33,  24,
        126, 27,  186, 4,   0,   1,   168, 0,   11,  43,  89,  176, 12,  35,
        66,  176, 13,  130, 3,   131, 26,  35,  45,  65,  45,  65,  131, 26,
        33,  4,   0,   132, 26,  32,  14,  130, 22,  32,  15,  134, 26,  39,
        24,  126, 59,  33,  27,  186, 1,   168, 134, 26,  32,  16,  130, 22,
        36,  17,  35,  66,  1,   1,   65,  117, 14,  32,  67,  133, 120, 37,
        94,  115, 94,  1,   116, 115, 131, 0,   51,  0,   176, 2,   69,  104,
        184, 2,   85,  69,  104, 176, 64,  139, 96,  176, 32,  35,  68,  176,
        6,   131, 16,  37,  86,  69,  104, 96,  176, 34,  130, 13,  32,  0,
        131, 13,  32,  88,  133, 30,  8,   74,  184, 2,   87,  35,  68,  64,
        52,  52,  51,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40,  39,
        38,  37,  36,  35,  34,  33,  32,  31,  30,  29,  28,  27,  26,  25,
        24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  14,  13,  12,  11,
        10,  9,   8,   7,   6,   5,   4,   3,   2,   1,   0,   44,  69,  35,
        70,  96,  32,  176, 38,  96,  176, 4,   38,  35,  72,  72,  45,  131,
        15,  33,  35,  97,  130, 16,  32,  97,  138, 16,  130, 127, 130, 18,
        32,  70,  141, 35,  33,  176, 32,  131, 55,  34,  97,  176, 32,  141,
        41,  32,  64,  130, 41,  32,  102, 142, 41,  32,  64,  133, 41,  32,
        64,  136, 41,  53,  1,   16,  32,  60,  0,   60,  45,  44,  32,  69,
        35,  32,  176, 205, 68,  35,  32,  184, 1,   90,  81,  88,  130, 11,
        38,  141, 68,  35,  89,  32,  176, 237, 132, 11,  32,  77,  132, 11,
        33,  4,   38,  132, 12,  32,  13,  130, 12,  33,  33,  33,  130, 49,
        8,   35,  32,  69,  24,  104, 68,  32,  176, 1,   96,  32,  69,  176,
        70,  118, 104, 138, 69,  96,  68,  45,  44,  1,   177, 11,  10,  67,
        35,  67,  101, 10,  45,  44,  0,   177, 10,  11,  130, 10,  32,  11,
        130, 9,   40,  176, 70,  35,  112, 177, 1,   70,  62,  1,   132, 8,
        40,  2,   70,  69,  58,  177, 2,   0,   8,   13,  130, 227, 39,  176,
        74,  35,  68,  69,  176, 73,  35,  130, 58,  130, 70,  61,  3,   37,
        69,  97,  100, 176, 80,  81,  88,  69,  68,  27,  33,  33,  89,  45,
        44,  176, 1,   67,  99,  35,  98,  176, 0,   35,  66,  176, 15,  43,
        130, 115, 35,  69,  176, 0,   67,  132, 102, 36,  176, 6,   67,  176,
        7,   132, 102, 34,  32,  105, 176, 130, 206, 55,  0,   139, 32,  177,
        44,  192, 138, 140, 184, 16,  0,   98,  96,  43,  12,  100, 35,  100,
        97,  92,  88,  176, 3,   97,  130, 67,  8,   38,  69,  176, 17,  43,
        176, 71,  35,  68,  176, 71,  122, 228, 24,  45,  44,  184, 1,   166,
        84,  88,  176, 9,   67,  184, 1,   0,   84,  88,  185, 0,   74,  255,
        128, 177, 73,  128, 68,  68,  89,  130, 41,  37,  138, 3,   69,  138,
        138, 135, 141, 46,  37,  45,  44,  75,  82,  88,  33,  130, 143, 36,
        35,  69,  140, 32,  176, 130, 160, 33,  82,  88,  132, 155, 130, 46,
        34,  1,   24,  47,  130, 146, 131, 19,  131, 192, 32,  69,  132, 202,
        34,  101, 35,  69,  131, 37,  8,   56,  96,  106, 32,  176, 9,   35,
        66,  35,  104, 138, 106, 96,  97,  32,  176, 26,  138, 176, 0,   82,
        121, 33,  178, 26,  74,  64,  185, 255, 224, 0,   74,  69,  32,  138,
        84,  88,  35,  33,  176, 63,  27,  35,  89,  97,  68,  28,  177, 20,
        0,   138, 82,  121, 179, 73,  64,  32,  73,  141, 25,  36,  45,  44,
        177, 16,  17,  65,  67,  5,   34,  177, 14,  15,  134, 8,   33,  12,
        13,  139, 8,   33,  101, 11,  130, 36,  132, 27,  132, 9,   132, 46,
        131, 9,   130, 181, 65,  68,  7,   32,  1,   131, 145, 40,  35,  73,
        176, 64,  96,  176, 32,  99,  32,  130, 137, 37,  88,  35,  176, 2,
        37,  56,  131, 4,   39,  101, 56,  0,   138, 99,  56,  27,  33,  131,
        0,   62,  89,  1,   45,  44,  69,  105, 176, 9,   67,  96,  138, 16,
        58,  45,  44,  1,   176, 5,   37,  16,  35,  32,  138, 245, 0,   176,
        1,   96,  35,  237, 236, 141, 17,  32,  97,  134, 17,  36,  6,   37,
        16,  245, 0,   131, 28,  32,  32,  130, 37,  66,  76,  8,   34,  176,
        1,   97,  135, 11,  46,  176, 43,  43,  176, 42,  42,  45,  44,  0,
        176, 7,   67,  176, 6,   67,  130, 173, 32,  62,  132, 15,  44,  53,
        45,  44,  118, 176, 75,  35,  112, 16,  32,  176, 75,  69,  130, 148,
        33,  80,  88,  130, 52,  48,  89,  58,  47,  24,  45,  44,  33,  33,
        12,  100, 35,  100, 139, 184, 64,  0,   98,  130, 12,  35,  176, 128,
        81,  88,  133, 15,  44,  32,  0,   98,  27,  178, 0,   64,  47,  43,
        89,  176, 2,   96,  131, 25,  32,  192, 135, 25,  33,  21,  85,  131,
        25,  32,  128, 135, 25,  136, 62,  46,  96,  35,  33,  45,  44,  180,
        0,   1,   0,   0,   0,   21,  176, 8,   38,  136, 2,   41,  15,  16,
        22,  19,  69,  104, 58,  176, 1,   22,  154, 30,  32,  101, 133, 31,
        54,  118, 70,  35,  70,  96,  138, 138, 70,  35,  32,  70,  138, 96,
        138, 97,  184, 255, 128, 98,  35,  32,  16,  35,  135, 178, 43,  184,
        255, 192, 139, 27,  176, 64,  140, 89,  104, 1,   58,  130, 187, 33,
        176, 51,  132, 238, 40,  0,   6,   43,  0,   25,  5,   200, 0,   37,
        131, 3,   39,  4,   62,  0,   25,  0,   0,   255, 219, 130, 3,   32,
        231, 130, 3,   36,  219, 254, 117, 255, 232, 130, 31,  50,  0,   254,
        216, 0,   0,   2,   222, 255, 238, 5,   219, 0,   18,  2,   228, 255,
        231, 5,   213, 132, 39,  42,  231, 2,   240, 0,   25,  6,   68,  0,
        0,   5,   3,   130, 183, 130, 2,   32,  255, 150, 0,   35,  2,   179,
        2,   130, 130, 30,  134, 2,   35,  254, 80,  2,   6,   130, 1,   34,
        80,  6,   80,  130, 85,  38,  31,  2,   31,  2,   105, 4,   210, 133,
        56,  33,  0,   200, 130, 34,  131, 3,   130, 6,   146, 2,   36,  148,
        0,   105, 0,   148, 130, 1,   38,  157, 0,   210, 0,   197, 255, 152,
        130, 9,   36,  148, 1,   90,  0,   104, 130, 41,  38,  123, 0,   202,
        0,   25,  0,   166, 130, 9,   46,  111, 0,   191, 0,   209, 0,   197,
        0,   123, 0,   40,  0,   80,  2,   7,   130, 209, 32,  123, 130, 13,
        38,  5,   0,   12,  0,   110, 0,   17,  130, 43,  36,  157, 0,   200,
        0,   223, 130, 43,  38,  148, 0,   58,  0,   165, 0,   170, 130, 27,
        62,  222, 0,   238, 1,   19,  0,   173, 0,   180, 1,   147, 2,   117,
        0,   150, 0,   225, 0,   157, 0,   173, 1,   73,  0,   74,  0,   100,
        0,   192, 1,   44,  130, 73,  34,  148, 0,   181, 130, 39,  42,  198,
        0,   252, 2,   31,  3,   71,  255, 133, 255, 231, 130, 63,  34,  43,
        0,   106, 130, 181, 42,  55,  0,   123, 253, 225, 255, 58,  0,   79,
        0,   103, 130, 75,  60,  198, 0,   247, 1,   40,  1,   175, 255, 153,
        0,   31,  0,   135, 0,   198, 0,   198, 2,   179, 0,   3,   0,   65,
        0,   94,  0,   111, 0,   144, 132, 27,  36,  114, 1,   133, 1,   139,
        130, 241, 58,  228, 0,   37,  0,   61,  0,   76,  0,   86,  0,   154,
        0,   189, 0,   213, 1,   213, 3,   213, 255, 28,  255, 66,  255, 145,
        255, 163, 130, 119, 32,  81,  130, 23,  38,  94,  0,   172, 0,   190,
        0,   210, 130, 79,  34,  40,  1,   77,  131, 51,  8,   53,  3,   120,
        5,   103, 5,   200, 254, 117, 255, 206, 255, 232, 0,   21,  0,   80,
        0,   82,  0,   98,  0,   139, 0,   166, 0,   197, 1,   160, 1,   188,
        3,   133, 4,   12,  6,   18,  255, 99,  255, 115, 255, 169, 255, 202,
        0,   2,   0,   12,  0,   99,  0,   128, 0,   148, 130, 31,  131, 71,
        44,  197, 0,   208, 0,   210, 0,   221, 0,   222, 0,   226, 0,   246,
        130, 137, 56,  110, 2,   0,   2,   12,  2,   80,  2,   117, 2,   154,
        0,   40,  0,   49,  0,   51,  0,   67,  0,   77,  0,   83,  0,   116,
        130, 237, 40,  185, 0,   189, 1,   114, 1,   117, 1,   237, 130, 1,
        32,  255, 130, 171, 60,  175, 3,   10,  4,   175, 6,   204, 6,   240,
        253, 237, 254, 166, 254, 216, 254, 247, 255, 13,  255, 70,  255, 160,
        255, 182, 255, 247, 0,   6,   130, 103, 34,  64,  0,   78,  130, 53,
        40,  126, 0,   130, 0,   151, 0,   154, 0,   157, 130, 1,   36,  170,
        0,   179, 0,   197, 130, 1,   131, 105, 34,  28,  1,   39,  130, 195,
        44,  65,  1,   71,  1,   89,  1,   102, 1,   200, 1,   244, 2,   5,
        130, 121, 56,  55,  2,   105, 2,   218, 2,   241, 3,   108, 5,   48,
        5,   52,  255, 194, 255, 242, 0,   10,  0,   29,  0,   64,  0,   68,
        130, 253, 32,  80,  130, 185, 36,  102, 0,   117, 0,   119, 130, 189,
        34,  157, 0,   173, 130, 1,   32,  179, 130, 183, 36,  224, 0,   237,
        0,   243, 130, 185, 34,  2,   1,   15,  130, 79,  8,   32,  78,  1,
        80,  1,   81,  1,   100, 1,   139, 1,   188, 1,   218, 2,   6,   2,
        56,  2,   87,  2,   100, 2,   141, 2,   147, 2,   179, 2,   225, 3,
        72,  3,   102, 130, 1,   56,  192, 4,   209, 5,   181, 5,   188, 6,
        6,   7,   83,  254, 56,  254, 142, 255, 102, 255, 225, 0,   8,   0,
        33,  0,   37,  130, 1,   32,  41,  130, 243, 32,  50,  130, 107, 34,
        99,  0,   105, 130, 101, 46,  156, 0,   167, 0,   178, 0,   191, 0,
        210, 0,   228, 0,   238, 1,   12,  130, 97,  42,  44,  1,   46,  1,
        65,  1,   70,  1,   77,  1,   87,  130, 183, 38,  90,  1,   111, 1,
        114, 1,   138, 130, 109, 34,  171, 1,   175, 130, 113, 44,  200, 1,
        213, 1,   230, 1,   255, 2,   31,  2,   54,  2,   80,  130, 1,   8,
        48,  88,  2,   127, 2,   130, 2,   135, 2,   228, 3,   46,  3,   59,
        3,   69,  3,   120, 3,   232, 3,   243, 4,   12,  4,   38,  4,   51,
        4,   61,  4,   100, 4,   103, 4,   236, 5,   14,  5,   15,  5,   215,
        5,   250, 8,   0,   252, 186, 253, 151, 130, 1,   54,  207, 254, 13,
        254, 75,  254, 90,  255, 7,   255, 78,  255, 83,  255, 228, 0,   0,
        0,   23,  0,   91,  0,   98,  130, 251, 34,  123, 0,   124, 130, 1,
        34,  137, 0,   198, 130, 1,   32,  225, 130, 243, 38,  243, 0,   244,
        0,   249, 0,   249, 130, 149, 36,  16,  1,   17,  1,   23,  130, 253,
        52,  40,  1,   54,  1,   63,  1,   99,  1,   120, 1,   125, 1,   126,
        1,   147, 1,   163, 1,   166, 1,   176, 130, 1,   38,  194, 1,   195,
        1,   196, 1,   202, 130, 157, 50,  213, 1,   225, 1,   243, 1,   244,
        2,   10,  2,   37,  2,   49,  2,   62,  2,   105, 2,   108, 130, 161,
        50,  181, 2,   204, 2,   208, 2,   212, 2,   228, 2,   255, 3,   15,
        3,   16,  3,   22,  3,   35,  130, 1,   42,  70,  3,   71,  3,   73,
        3,   83,  3,   95,  3,   119, 130, 185, 42,  145, 3,   160, 3,   182,
        3,   222, 3,   245, 4,   0,   130, 191, 32,  59,  130, 191, 38,  86,
        4,   107, 4,   123, 4,   136, 130, 1,   8,   50,  168, 4,   173, 4,
        229, 4,   243, 5,   0,   5,   41,  5,   111, 5,   139, 5,   176, 5,
        177, 5,   237, 6,   16,  6,   18,  6,   55,  6,   109, 6,   130, 6,
        211, 6,   254, 7,   21,  7,   59,  7,   103, 8,   0,   0,   200, 0,
        213, 0,   199, 0,   204, 130, 215, 137, 2,   46,  166, 0,   154, 0,
        181, 0,   161, 0,   173, 0,   172, 0,   185, 0,   180, 130, 7,   48,
        123, 0,   148, 0,   116, 1,   27,  4,   4,   1,   215, 2,   191, 0,
        247, 1,   226, 130, 173, 34,  189, 0,   2,   130, 1,   32,  148, 130,
        25,  8,   39,  2,   0,   1,   255, 152, 1,   213, 4,   136, 0,   120,
        0,   221, 0,   26,  0,   124, 0,   197, 6,   142, 0,   99,  0,   184,
        0,   160, 0,   149, 0,   197, 4,   167, 0,   193, 0,   99,  6,   0,
        1,   132, 97,  35,  2,   136, 0,   0,   134, 3,   40,  225, 2,   253,
        0,   111, 5,   15,  0,   15,  130, 3,   48,  216, 5,   89,  0,   12,
        5,   148, 0,   74,  1,   213, 0,   111, 2,   154, 0,   123, 130, 3,
        36,  74,  3,   219, 0,   110, 130, 27,  32,  105, 130, 47,  32,  200,
        130, 15,  32,  111, 131, 7,   33,  3,   244, 130, 39,  38,  15,  0,
        130, 5,   15,  1,   46,  130, 27,  32,  170, 130, 3,   32,  208, 130,
        3,   32,  105, 130, 3,   32,  249, 130, 3,   32,  137, 130, 3,   32,
        222, 130, 3,   32,  177, 130, 3,   32,  156, 130, 47,  32,  225, 131,
        3,   134, 31,  131, 35,  58,  105, 3,   96,  0,   37,  6,   222, 0,
        93,  5,   133, 0,   17,  4,   154, 0,   191, 5,   137, 0,   104, 5,
        254, 0,   191, 4,   86,  130, 3,   32,  74,  130, 15,  32,  200, 130,
        15,  44,  225, 0,   191, 2,   78,  0,   190, 2,   125, 255, 72,  5,
        57,  130, 23,  36,  68,  0,   191, 6,   228, 130, 27,  32,  233, 130,
        7,   36,  55,  0,   104, 4,   108, 133, 7,   130, 87,  36,  191, 4,
        79,  0,   100, 130, 7,   50,  18,  5,   139, 0,   179, 5,   59,  0,
        36,  6,   216, 0,   2,   5,   2,   0,   16,  4,   252, 130, 95,  34,
        214, 0,   95,  130, 183, 32,  197, 131, 179, 130, 7,   32,  74,  130,
        39,  52,  105, 4,   0,   0,   86,  4,   234, 1,   102, 4,   107, 0,
        103, 5,   9,   0,   197, 4,   25,  0,   111, 130, 7,   34,  111, 4,
        117, 130, 247, 44,  241, 0,   80,  4,   253, 0,   111, 4,   247, 0,
        197, 2,   80,  130, 3,   36,  111, 255, 102, 4,   173, 133, 11,  33,
        7,   120, 130, 43,  130, 23,  33,  4,   234, 132, 47,  32,  197, 131,
        51,  33,  3,   70,  130, 19,  38,  20,  0,   157, 2,   254, 0,   69,
        130, 51,  46,  185, 4,   36,  0,   18,  6,   42,  0,   12,  4,   232,
        0,   88,  4,   46,  130, 131, 34,  150, 0,   123, 130, 119, 34,  25,
        2,   31,  130, 67,  34,  154, 0,   136, 131, 127, 131, 247, 132, 3,
        130, 247, 131, 243, 32,  5,   134, 211, 131, 191, 131, 147, 148, 3,
        130, 163, 131, 159, 139, 3,   130, 159, 32,  123, 130, 3,   36,  25,
        2,   80,  255, 219, 130, 7,   32,  12,  130, 127, 32,  197, 131, 155,
        144, 3,   34,  247, 0,   185, 132, 155, 134, 7,   130, 131, 34,  228,
        2,   55,  130, 195, 34,  15,  0,   205, 130, 11,  32,  247, 130, 3,
        36,  221, 5,   15,  1,   244, 130, 7,   36,  216, 4,   201, 0,   197,
        130, 7,   44,  204, 6,   216, 0,   99,  6,   117, 0,   0,   4,   234,
        1,   200, 130, 3,   32,  89,  130, 19,  34,  105, 7,   65,  130, 219,
        38,  55,  0,   104, 6,   191, 0,   120, 65,  207, 11,  130, 11,  34,
        80,  5,   3,   132, 55,  44,  203, 5,   34,  0,   50,  6,   18,  0,
        117, 6,   128, 0,   63,  130, 83,  36,  12,  3,   96,  0,   72,  130,
        3,   40,  62,  6,   68,  0,   64,  6,   207, 0,   103, 131, 155, 130,
        15,  32,  37,  66,  19,  8,   34,  176, 0,   116, 130, 63,  32,  237,
        132, 79,  38,  77,  0,   11,  4,   49,  0,   111, 130, 3,   36,  74,
        8,   0,   0,   247, 130, 35,  36,  0,   5,   133, 0,   17,  131, 3,
        32,  6,   130, 119, 130, 19,  34,  104, 7,   109, 130, 31,  34,  0,
        0,   105, 130, 11,  36,  99,  2,   253, 0,   111, 132, 3,   34,  136,
        0,   200, 130, 43,  32,  200, 132, 67,  52,  15,  0,   105, 4,   46,
        0,   16,  4,   252, 0,   17,  0,   74,  253, 151, 5,   16,  255, 240,
        2,   167, 130, 39,  40,  167, 0,   74,  5,   40,  0,   80,  5,   46,
        130, 3,   38,  15,  0,   228, 5,   15,  2,   12,  131, 51,  131, 63,
        35,  8,   25,  0,   12,  131, 99,  35,  4,   86,  0,   191, 135, 7,
        131, 11,  39,  2,   78,  0,   122, 2,   78,  255, 218, 130, 7,   32,
        11,  130, 3,   32,  23,  131, 131, 135, 3,   35,  5,   139, 0,   179,
        135, 3,   39,  2,   80,  0,   197, 4,   234, 1,   40,  134, 3,   32,
        89,  133, 7,   33,  2,   18,  130, 7,   32,  213, 130, 3,   32,  254,
        130, 3,   32,  112, 130, 3,   32,  244, 132, 23,  50,  79,  0,   100,
        4,   20,  0,   157, 2,   31,  0,   197, 5,   254, 255, 225, 4,   210,
        0,   109, 131, 171, 132, 179, 32,  108, 130, 127, 32,  9,   130, 23,
        130, 195, 35,  3,   96,  0,   210, 130, 3,   32,  136, 130, 3,   36,
        154, 6,   105, 0,   37,  130, 3,   48,  99,  6,   253, 0,   123, 2,
        154, 0,   111, 5,   16,  0,   98,  0,   0,   0,   31,  130, 3,   49,
        248, 9,   10,  7,   0,   3,   3,   3,   4,   5,   6,   6,   7,   2,
        3,   3,   5,   6,   130, 12,  130, 11,  32,  6,   134, 0,   33,  3,
        3,   130, 8,   41,  3,   8,   7,   5,   6,   7,   5,   5,   7,   7,
        130, 14,  39,  5,   9,   7,   7,   5,   7,   6,   6,   130, 13,  45,
        9,   6,   5,   5,   3,   5,   3,   6,   5,   6,   4,   5,   4,   5,
        131, 2,   37,  2,   3,   5,   2,   8,   5,   130, 0,   37,  3,   4,
        4,   6,   4,   6,   130, 2,   37,  4,   3,   4,   6,   7,   7,   131,
        45,  33,  7,   4,   133, 0,   130, 29,  33,  5,   2,   130, 0,   131,
        7,   32,  5,   132, 106, 32,  5,   130, 41,  130, 33,  41,  4,   9,
        8,   10,  6,   6,   6,   8,   7,   8,   131, 5,   130, 25,  130, 8,
        130, 61,  35,  8,   6,   4,   3,   130, 16,  131, 128, 40,  9,   3,
        7,   7,   7,   9,   8,   5,   9,   130, 119, 130, 19,  39,  4,   5,
        2,   6,   4,   4,   6,   6,   130, 180, 130, 149, 33,  5,   7,   131,
        21,  32,  3,   131, 34,  130, 37,  33,  2,   9,   131, 69,  33,  7,
        6,   131, 5,   33,  4,   3,   130, 26,  34,  4,   5,   7,   130, 44,
        32,  4,   130, 93,  33,  3,   6,   130, 248, 34,  10,  11,  8,   132,
        247, 32,  6,   130, 157, 151, 247, 33,  4,   9,   130, 233, 137, 247,
        36,  8,   8,   5,   8,   6,   130, 46,  36,  7,   9,   7,   7,   6,
        133, 247, 131, 123, 32,  6,   130, 129, 37,  3,   3,   6,   3,   9,
        6,   130, 0,   32,  4,   130, 203, 33,  7,   9,   130, 36,  133, 247,
        36,  7,   5,   8,   8,   7,   65,  74,  5,   132, 233, 32,  3,   130,
        0,   133, 14,  132, 209, 32,  4,   132, 5,   35,  6,   9,   9,   11,
        130, 8,   34,  9,   8,   9,   130, 5,   132, 181, 43,  8,   6,   5,
        5,   8,   9,   7,   5,   3,   6,   7,   7,   131, 247, 42,  10,  3,
        7,   7,   8,   10,  10,  5,   10,  5,   4,   130, 105, 130, 20,  32,
        1,   132, 247, 36,  7,   6,   3,   4,   9,   130, 162, 133, 247, 32,
        8,   130, 106, 34,  7,   7,   3,   133, 247, 34,  7,   8,   7,   130,
        155, 35,  3,   8,   7,   7,   130, 31,  38,  6,   5,   4,   4,   8,
        8,   9,   132, 247, 34,  11,  12,  9,   132, 247, 130, 25,  37,  8,
        2,   3,   3,   5,   7,   131, 145, 130, 37,  134, 2,   33,  3,   3,
        130, 8,   33,  4,   10,  130, 110, 130, 136, 33,  8,   8,   130, 14,
        37,  6,   9,   8,   9,   6,   9,   130, 21,  33,  8,   7,   131, 5,
        32,  3,   131, 238, 33,  6,   6,   130, 15,  34,  7,   4,   7,   130,
        60,  130, 247, 132, 11,  32,  6,   130, 13,  131, 31,  33,  4,   3,
        130, 8,   35,  7,   8,   6,   8,   132, 135, 32,  6,   133, 41,  130,
        39,  132, 88,  134, 100, 131, 57,  32,  7,   130, 35,  131, 247, 35,
        7,   7,   10,  9,   133, 93,  36,  7,   7,   8,   10,  7,   130, 247,
        34,  10,  8,   5,   130, 132, 33,  8,   7,   130, 192, 40,  12,  3,
        7,   7,   9,   11,  11,  5,   11,  131, 247, 39,  7,   6,   7,   7,
        0,   7,   4,   4,   131, 68,  34,  3,   4,   10,  130, 14,  33,  6,
        6,   130, 188, 36,  3,   9,   9,   9,   8,   130, 169, 32,  7,   130,
        49,  130, 3,   131, 5,   133, 147, 32,  6,   130, 250, 42,  5,   4,
        9,   9,   10,  3,   7,   0,   0,   0,   12,  130, 247, 32,  4,   130,
        156, 130, 42,  132, 247, 36,  8,   3,   4,   3,   6,   131, 12,  133,
        3,   33,  4,   3,   130, 7,   47,  5,   10,  9,   7,   8,   9,   6,
        7,   9,   9,   3,   3,   8,   6,   11,  9,   130, 247, 130, 78,  34,
        8,   9,   11,  130, 5,   32,  4,   131, 247, 32,  8,   132, 96,  34,
        4,   8,   7,   130, 228, 34,  3,   11,  7,   130, 50,  40,  4,   6,
        5,   7,   7,   11,  7,   7,   7,   131, 90,  35,  9,   9,   8,   6,
        130, 3,   65,  77,  9,   133, 247, 132, 97,  131, 20,  34,  8,   4,
        8,   130, 65,  36,  9,   7,   9,   10,  11,  130, 7,   35,  10,  9,
        10,  8,   130, 0,   32,  7,   130, 114, 131, 247, 130, 101, 33,  5,
        4,   132, 16,  44,  6,   6,   12,  4,   9,   9,   9,   12,  12,  6,
        12,  5,   4,   130, 137, 130, 125, 34,  0,   8,   4,   131, 27,  130,
        119, 32,  12,  130, 148, 146, 247, 130, 56,  36,  7,   7,   6,   3,
        10,  130, 43,  42,  6,   7,   8,   5,   5,   5,   10,  10,  10,  4,
        8,   130, 247, 36,  13,  13,  10,  0,   4,   130, 156, 39,  9,   8,
        8,   9,   2,   4,   4,   7,   130, 247, 32,  4,   146, 247, 36,  8,
        9,   10,  7,   7,   130, 247, 130, 219, 32,  11,  130, 10,  34,  10,
        9,   8,   130, 13,  35,  11,  9,   7,   8,   137, 247, 34,  8,   4,
        8,   130, 60,  35,  8,   3,   11,  8,   130, 75,  37,  5,   7,   5,
        8,   7,   11,  130, 197, 32,  4,   130, 152, 130, 172, 35,  7,   9,
        10,  9,   134, 247, 130, 138, 33,  8,   3,   130, 0,   33,  8,   9,
        131, 0,   132, 252, 130, 57,  33,  9,   8,   130, 247, 33,  11,  13,
        130, 14,  34,  11,  10,  12,  130, 5,   32,  10,  130, 69,  39,  10,
        11,  8,   6,   6,   9,   11,  9,   130, 247, 35,  9,   9,   8,   7,
        133, 247, 37,  10,  13,  12,  6,   13,  5,   131, 86,  38,  8,   7,
        7,   1,   8,   4,   3,   130, 26,  130, 253, 32,  12,  130, 62,  33,
        7,   7,   130, 86,  32,  3,   130, 217, 130, 88,  33,  3,   8,   130,
        133, 130, 3,   131, 5,   34,  7,   4,   10,  130, 43,  40,  7,   9,
        8,   5,   5,   6,   10,  10,  11,  132, 247, 36,  14,  14,  11,  0,
        4,   130, 0,   44,  10,  9,   9,   10,  2,   5,   5,   7,   9,   3,
        4,   4,   7,   130, 55,  134, 2,   131, 86,  39,  9,   5,   13,  9,
        8,   10,  10,  7,   130, 3,   53,  3,   4,   9,   7,   11,  10,  11,
        7,   11,  9,   8,   9,   9,   9,   13,  9,   9,   8,   4,   7,   4,
        8,   130, 81,  130, 15,  34,  8,   5,   9,   135, 247, 33,  9,   9,
        131, 247, 33,  9,   11,  130, 119, 130, 90,  130, 41,  35,  10,  7,
        10,  11,  131, 129, 130, 208, 132, 2,   143, 247, 132, 109, 35,  8,
        9,   12,  13,  130, 86,  34,  12,  11,  12,  130, 14,  130, 247, 33,
        9,   11,  132, 247, 51,  12,  9,   6,   4,   9,   11,  9,   9,   9,
        6,   6,   13,  4,   9,   9,   11,  14,  13,  7,   14,  131, 247, 32,
        9,   130, 43,  34,  0,   9,   4,   130, 18,  32,  8,   130, 247, 32,
        13,  136, 247, 39,  11,  11,  11,  9,   9,   9,   3,   10,  130, 4,
        43,  8,   9,   9,   10,  9,   9,   8,   7,   4,   11,  9,   9,   130,
        31,  130, 65,  54,  6,   11,  11,  12,  4,   9,   0,   0,   0,   15,
        16,  12,  0,   5,   5,   4,   5,   10,  9,   10,  10,  3,   4,   131,
        247, 32,  5,   140, 247, 130, 86,  53,  9,   6,   13,  11,  10,  10,
        11,  8,   9,   11,  12,  5,   5,   10,  8,   13,  12,  12,  9,   12,
        11,  9,   130, 3,   32,  13,  130, 84,  130, 224, 130, 81,  34,  8,
        9,   8,   130, 235, 130, 2,   35,  5,   9,   5,   13,  131, 107, 131,
        22,  132, 172, 131, 247, 38,  11,  11,  10,  8,   12,  12,  12,  132,
        241, 32,  8,   130, 40,  131, 39,  35,  5,   5,   9,   10,  131, 0,
        32,  9,   131, 158, 32,  5,   130, 4,   37,  9,   10,  8,   11,  13,
        13,  130, 8,   33,  14,  12,  131, 5,   35,  10,  9,   9,   10,  130,
        8,   38,  6,   6,   9,   13,  10,  7,   4,   132, 74,  44,  6,   6,
        13,  5,   11,  11,  12,  15,  14,  8,   16,  6,   5,   131, 239, 50,
        9,   9,   0,   10,  4,   4,   10,  10,  8,   9,   4,   5,   15,  11,
        8,   11,  8,   8,   5,   130, 0,   32,  12,  132, 0,   32,  5,   130,
        64,  130, 66,  32,  10,  131, 6,   34,  7,   4,   12,  130, 87,  130,
        2,   39,  6,   6,   6,   12,  12,  13,  5,   10,  130, 247, 32,  16,
        132, 247, 132, 133, 41,  11,  3,   5,   5,   7,   10,  4,   5,   4,
        8,   132, 145, 132, 4,   130, 232, 34,  10,  10,  6,   130, 247, 32,
        11,  130, 58,  130, 50,  36,  5,   11,  9,   13,  13,  135, 247, 38,
        15,  10,  9,   10,  5,   8,   5,   131, 243, 35,  10,  9,   11,  9,
        130, 106, 130, 71,  131, 247, 33,  10,  11,  131, 22,  33,  9,   13,
        130, 106, 130, 247, 34,  10,  11,  11,  130, 53,  32,  12,  134, 123,
        130, 19,  143, 247, 33,  11,  5,   132, 107, 34,  9,   11,  14,  130,
        49,  35,  10,  15,  12,  13,  130, 14,  32,  9,   130, 3,   43,  12,
        13,  9,   7,   7,   11,  14,  10,  7,   5,   10,  12,  130, 192, 35,
        7,   7,   15,  5,   130, 247, 33,  16,  15,  132, 247, 37,  5,   10,
        11,  9,   9,   1,   132, 247, 32,  11,  132, 247, 32,  9,   130, 15,
        138, 247, 34,  12,  10,  9,   130, 1,   33,  10,  11,  130, 56,  33,
        8,   4,   130, 12,  41,  9,   9,   9,   10,  6,   7,   7,   12,  13,
        14,  132, 247, 40,  17,  18,  13,  0,   5,   5,   6,   7,   9,   130,
        81,  41,  3,   6,   6,   7,   11,  5,   6,   5,   8,   11,  136, 0,
        33,  6,   6,   130, 10,  35,  8,   14,  11,  10,  130, 131, 130, 3,
        130, 14,  43,  9,   17,  13,  13,  9,   13,  11,  9,   10,  13,  11,
        15,  130, 141, 37,  6,   8,   6,   11,  8,   11,  130, 115, 33,  11,
        9,   130, 42,  36,  6,   6,   10,  6,   16,  130, 49,  33,  11,  7,
        130, 13,  33,  9,   15,  130, 120, 34,  5,   4,   5,   130, 15,  34,
        12,  9,   13,  130, 53,  67,  62,  9,   32,  6,   130, 0,   136, 99,
        33,  11,  12,  131, 34,  36,  10,  13,  10,  11,  15,  131, 59,  34,
        16,  13,  14,  130, 26,  40,  12,  11,  10,  11,  13,  13,  11,  7,
        7,   130, 13,  130, 98,  130, 28,  130, 76,  32,  18,  130, 40,  39,
        13,  17,  16,  9,   17,  7,   7,   5,   130, 109, 34,  9,   10,  1,
        130, 174, 130, 184, 35,  11,  5,   7,   17,  131, 131, 32,  9,   130,
        86,  32,  6,   130, 105, 130, 2,   33,  6,   12,  134, 95,  130, 247,
        34,  9,   4,   13,  131, 5,   48,  11,  11,  8,   7,   7,   14,  14,
        15,  6,   11,  0,   0,   0,   18,  19,  14,  0,   130, 45,  36,  7,
        9,   11,  12,  12,  132, 247, 130, 12,  32,  9,   143, 247, 35,  16,
        13,  11,  12,  130, 58,  51,  13,  14,  6,   6,   12,  10,  17,  14,
        14,  10,  14,  12,  10,  12,  13,  13,  15,  12,  12,  11,  133, 247,
        45,  10,  12,  9,   12,  10,  6,   12,  12,  6,   6,   11,  6,   18,
        12,  130, 74,  33,  8,   8,   130, 120, 36,  15,  10,  11,  10,  6,
        130, 160, 37,  13,  13,  12,  10,  14,  14,  130, 64,  32,  10,  65,
        200, 6,   33,  10,  6,   130, 0,   133, 152, 32,  12,  131, 0,   130,
        206, 38,  12,  12,  13,  10,  12,  16,  16,  130, 121, 130, 95,  130,
        5,   131, 22,  49,  14,  15,  12,  8,   8,   13,  15,  11,  8,   6,
        11,  13,  12,  11,  12,  11,  11,  19,  130, 212, 38,  14,  18,  16,
        9,   18,  7,   7,   130, 105, 36,  11,  11,  12,  1,   11,  131, 7,
        38,  12,  11,  6,   7,   19,  13,  10,  130, 96,  130, 86,  33,  6,
        14,  130, 106, 35,  13,  13,  6,   10,  136, 196, 35,  10,  8,   5,
        14,  130, 61,  130, 16,  37,  8,   8,   7,   15,  14,  16,  132, 247,
        34,  19,  20,  15,  132, 247, 35,  11,  12,  12,  14,  130, 247, 32,
        10,  130, 186, 33,  6,   9,   131, 116, 133, 3,   32,  6,   130, 204,
        41,  12,  8,   16,  13,  11,  13,  14,  11,  11,  14,  133, 247, 35,
        15,  15,  10,  15,  130, 247, 32,  14,  132, 247, 36,  7,   9,   7,
        11,  10,  130, 13,  130, 3,   32,  7,   135, 247, 130, 58,  51,  8,
        10,  8,   12,  11,  15,  12,  11,  11,  5,   5,   5,   12,  13,  13,
        13,  11,  15,  15,  14,  67,  61,  9,   133, 247, 137, 101, 32,  5,
        131, 10,  130, 155, 33,  16,  16,  130, 203, 34,  18,  15,  15,  130,
        14,  32,  11,  130, 3,   33,  14,  16,  130, 247, 131, 4,   36,  6,
        12,  14,  13,  12,  134, 247, 36,  15,  20,  17,  10,  19,  131, 247,
        40,  12,  13,  11,  12,  2,   12,  6,   6,   13,  130, 26,  131, 247,
        35,  11,  13,  11,  11,  131, 247, 34,  15,  15,  15,  130, 250, 34,
        6,   11,  12,  130, 66,  132, 4,   35,  10,  10,  5,   15,  130, 8,
        33,  10,  12,  130, 77,  42,  9,   15,  15,  17,  6,   12,  0,   0,
        0,   20,  21,  134, 247, 33,  13,  13,  132, 247, 32,  13,  130, 58,
        32,  9,   66,  42,  5,   131, 5,   131, 86,  42,  13,  9,   18,  13,
        12,  14,  15,  12,  11,  14,  15,  130, 14,  39,  11,  17,  15,  16,
        11,  16,  13,  11,  130, 138, 32,  17,  130, 113, 130, 247, 46,  13,
        10,  12,  11,  13,  10,  13,  12,  8,   13,  13,  6,   6,   12,  6,
        130, 44,  33,  13,  13,  130, 247, 130, 235, 130, 137, 35,  7,   5,
        6,   13,  130, 90,  36,  12,  15,  16,  14,  11,  132, 0,   130, 126,
        32,  12,  130, 173, 130, 73,  32,  12,  131, 0,   130, 28,  34,  13,
        12,  5,   131, 5,   42,  14,  11,  13,  17,  16,  12,  13,  13,  18,
        16,  16,  130, 14,  32,  11,  130, 3,   49,  15,  17,  13,  8,   8,
        14,  18,  12,  8,   6,   13,  15,  13,  13,  12,  11,  11,  21,  130,
        75,  36,  16,  21,  19,  10,  20,  131, 247, 32,  13,  130, 131, 32,
        1,   130, 113, 133, 247, 34,  20,  13,  12,  130, 141, 130, 188, 35,
        6,   16,  16,  16,  132, 247, 32,  13,  131, 24,  131, 20,  32,  11,
        130, 247, 32,  12,  130, 61,  130, 149, 38,  8,   9,   16,  16,  17,
        6,   13,  130, 247, 35,  21,  21,  16,  0,   130, 69,  45,  8,   15,
        13,  14,  15,  4,   7,   7,   10,  13,  6,   7,   6,   11,  137, 247,
        32,  7,   130, 105, 130, 247, 32,  15,  130, 183, 41,  12,  11,  15,
        16,  6,   6,   14,  11,  18,  16,  133, 247, 34,  15,  15,  19,  130,
        107, 36,  7,   11,  7,   15,  11,  130, 7,   130, 3,   134, 247, 33,
        20,  13,  130, 0,   34,  9,   10,  9,   132, 247, 131, 225, 39,  13,
        15,  15,  15,  12,  16,  16,  15,  131, 230, 130, 127, 131, 6,   67,
        135, 9,   132, 106, 32,  5,   131, 5,   32,  15,  130, 242, 37,  19,
        13,  13,  13,  19,  16,  131, 5,   131, 131, 41,  17,  18,  13,  9,
        9,   14,  18,  13,  10,  7,   131, 247, 32,  13,  130, 247, 44,  7,
        15,  15,  16,  21,  20,  10,  21,  8,   8,   6,   6,   13,  130, 247,
        42,  0,   13,  7,   7,   14,  14,  13,  13,  6,   8,   21,  130, 162,
        65,  79,  5,   32,  16,  130, 106, 33,  15,  15,  130, 30,  33,  13,
        13,  130, 151, 130, 28,  35,  11,  10,  6,   16,  130, 119, 33,  11,
        13,  130, 77,  36,  9,   17,  17,  18,  7,   131, 247, 34,  22,  22,
        17,  133, 247, 34,  14,  14,  16,  131, 247, 33,  14,  6,   130, 247,
        32,  14,  136, 0,   33,  7,   6,   130, 10,  36,  9,   19,  15,  13,
        15,  130, 58,  45,  16,  16,  6,   7,   14,  11,  19,  16,  17,  12,
        17,  14,  12,  14,  130, 247, 130, 114, 132, 247, 39,  14,  13,  14,
        11,  14,  13,  8,   14,  130, 60,  34,  13,  6,   20,  130, 13,  42,
        14,  9,   11,  9,   14,  13,  19,  14,  13,  12,  7,   130, 50,  132,
        247, 34,  17,  15,  13,  132, 0,   130, 126, 130, 167, 130, 248, 130,
        145, 130, 16,  130, 94,  33,  14,  13,  131, 100, 36,  14,  16,  13,
        14,  19,  130, 49,  35,  14,  20,  17,  20,  130, 20,  32,  13,  130,
        69,  38,  17,  19,  14,  9,   10,  16,  19,  130, 247, 39,  14,  16,
        14,  14,  15,  11,  11,  22,  130, 247, 36,  17,  22,  21,  11,  22,
        131, 247, 130, 63,  34,  14,  1,   14,  135, 247, 32,  22,  136, 247,
        34,  17,  17,  17,  130, 112, 32,  6,   133, 110, 32,  14,  130, 70,
        35,  12,  11,  6,   17,  130, 78,  56,  12,  13,  14,  9,   9,   9,
        17,  18,  19,  7,   14,  0,   0,   0,   23,  23,  18,  0,   7,   7,
        7,   8,   16,  15,  15,  132, 247, 36,  15,  6,   7,   7,   12,  130,
        55,  134, 2,   33,  7,   6,   130, 8,   56,  10,  20,  15,  14,  16,
        17,  14,  13,  17,  19,  8,   8,   16,  13,  21,  19,  18,  13,  18,
        16,  12,  14,  18,  15,  19,  130, 159, 134, 247, 33,  15,  11,  130,
        247, 33,  15,  16,  130, 133, 38,  7,   22,  16,  14,  15,  14,  10,
        135, 247, 34,  8,   7,   8,   130, 65,  36,  16,  14,  19,  18,  18,
        138, 247, 130, 113, 33,  7,   16,  65,  92,  8,   32,  15,  131, 100,
        36,  14,  17,  15,  16,  19,  130, 81,  35,  15,  21,  18,  20,  130,
        49,  36,  13,  15,  15,  15,  18,  132, 247, 39,  20,  14,  10,  7,
        15,  17,  14,  15,  134, 247, 46,  18,  23,  21,  11,  23,  8,   8,
        7,   7,   15,  15,  13,  14,  1,   15,  131, 7,   42,  15,  14,  7,
        8,   23,  15,  14,  15,  14,  14,  8,   130, 0,   32,  18,  132, 0,
        135, 94,  130, 20,  34,  12,  11,  7,   131, 247, 42,  13,  13,  15,
        10,  10,  10,  19,  18,  20,  7,   15,  130, 247, 35,  24,  24,  18,
        0,   130, 69,  32,  8,   130, 117, 33,  17,  4,   131, 242, 142, 247,
        32,  7,   132, 247, 37,  18,  15,  17,  18,  14,  14,  135, 247, 40,
        19,  14,  19,  16,  13,  15,  18,  17,  19,  130, 27,  45,  8,   12,
        8,   15,  12,  15,  14,  16,  12,  15,  13,  8,   16,  16,  130, 133,
        46,  7,   24,  16,  14,  16,  15,  11,  13,  9,   14,  13,  19,  15,
        13,  13,  131, 247, 38,  18,  18,  17,  14,  19,  19,  18,  133, 231,
        33,  12,  13,  130, 0,   146, 247, 37,  15,  17,  15,  17,  20,  19,
        130, 222, 34,  22,  19,  21,  136, 247, 36,  15,  10,  11,  18,  21,
        137, 247, 40,  24,  8,   18,  18,  19,  24,  22,  12,  24,  134, 247,
        32,  15,  137, 247, 34,  24,  18,  14,  130, 96,  130, 246, 33,  8,
        19,  130, 106, 35,  18,  18,  7,   16,  130, 68,  130, 77,  38,  15,
        14,  15,  13,  13,  7,   18,  130, 5,   130, 12,  130, 247, 34,  20,
        19,  21,  132, 247, 53,  27,  28,  21,  0,   9,   9,   9,   11,  16,
        17,  18,  19,  7,   9,   9,   12,  17,  8,   9,   8,   13,  17,  136,
        0,   33,  9,   9,   130, 10,  50,  11,  23,  20,  16,  19,  20,  15,
        15,  20,  21,  9,   9,   18,  14,  25,  21,  21,  15,  21,  130, 101,
        42,  20,  19,  23,  17,  18,  16,  10,  13,  10,  18,  13,  130, 178,
        130, 3,   49,  10,  17,  18,  9,   9,   16,  9,   25,  18,  16,  17,
        17,  12,  14,  11,  16,  15,  21,  130, 123, 41,  9,   7,   9,   17,
        20,  20,  19,  15,  21,  21,  130, 64,  130, 138, 34,  15,  13,  15,
        130, 0,   130, 113, 34,  9,   18,  16,  135, 0,   48,  17,  7,   17,
        17,  18,  17,  20,  17,  18,  23,  24,  17,  16,  17,  25,  21,  23,
        130, 115, 130, 16,  62,  17,  20,  23,  17,  11,  12,  22,  23,  16,
        12,  9,   17,  19,  18,  17,  18,  15,  15,  28,  9,   20,  20,  21,
        27,  25,  14,  27,  11,  11,  8,   9,   130, 16,  33,  18,  2,   130,
        174, 40,  18,  18,  17,  17,  8,   11,  27,  20,  15,  130, 96,  131,
        87,  32,  21,  130, 106, 35,  20,  20,  9,   15,  130, 74,  42,  17,
        16,  16,  17,  17,  17,  14,  14,  7,   20,  15,  130, 61,  34,  17,
        17,  12,  130, 78,  41,  22,  24,  9,   17,  0,   0,   0,   29,  30,
        22,  132, 247, 39,  18,  18,  19,  20,  7,   10,  10,  14,  130, 186,
        34,  9,   14,  18,  136, 0,   130, 232, 48,  18,  18,  12,  25,  20,
        17,  20,  22,  16,  16,  21,  22,  9,   9,   19,  15,  26,  130, 137,
        38,  23,  19,  17,  18,  21,  19,  25,  130, 38,  39,  10,  14,  10,
        20,  14,  18,  16,  18,  130, 3,   39,  10,  18,  19,  9,   9,   17,
        9,   27,  130, 27,  52,  18,  13,  14,  11,  19,  15,  23,  17,  15,
        16,  9,   8,   9,   18,  20,  20,  20,  16,  22,  23,  21,  133, 230,
        32,  14,  131, 6,   131, 159, 33,  19,  17,  131, 0,   32,  19,  131,
        0,   32,  8,   130, 72,  43,  18,  21,  18,  19,  25,  27,  17,  18,
        18,  26,  23,  24,  130, 14,  45,  17,  19,  18,  19,  22,  24,  18,
        11,  12,  24,  24,  17,  13,  9,   130, 28,  36,  18,  18,  16,  16,
        30,  130, 247, 38,  23,  30,  26,  15,  29,  11,  11,  131, 152, 37,
        15,  18,  1,   18,  11,  11,  130, 64,  41,  18,  9,   11,  29,  20,
        16,  20,  16,  16,  9,   130, 0,   38,  23,  23,  23,  21,  21,  21,
        9,   130, 133, 130, 76,  32,  18,  130, 3,   49,  17,  14,  8,   23,
        16,  18,  15,  16,  18,  18,  13,  12,  12,  24,  23,  25,  9,   18,
        130, 247, 43,  32,  33,  24,  0,   10,  10,  9,   11,  21,  20,  21,
        23,  130, 247, 37,  15,  20,  9,   10,  9,   15,  130, 168, 134, 2,
        33,  10,  9,   130, 8,   34,  13,  28,  22,  131, 131, 57,  18,  23,
        24,  9,   10,  21,  17,  27,  24,  25,  17,  25,  20,  18,  20,  23,
        21,  27,  20,  20,  19,  10,  15,  10,  18,  16,  130, 13,  130, 3,
        33,  11,  20,  130, 60,  34,  19,  9,   29,  130, 49,  36,  20,  13,
        16,  12,  20,  130, 37,  44,  17,  19,  10,  8,   9,   20,  22,  22,
        22,  18,  24,  25,  23,  65,  76,  5,   33,  16,  18,  130, 0,   130,
        158, 32,  9,   137, 100, 42,  20,  8,   20,  20,  21,  21,  23,  19,
        20,  27,  28,  130, 20,  33,  29,  25,  130, 87,  130, 7,   59,  20,
        21,  25,  26,  20,  14,  14,  24,  27,  20,  13,  10,  20,  23,  20,
        20,  20,  17,  17,  31,  10,  22,  22,  25,  33,  30,  16,  32,  131,
        247, 130, 16,  36,  20,  1,   20,  11,  11,  130, 25,  40,  19,  9,
        11,  32,  22,  18,  22,  18,  18,  131, 87,  34,  25,  25,  25,  130,
        250, 36,  9,   18,  19,  20,  19,  130, 0,   130, 29,  35,  18,  16,
        8,   25,  130, 44,  33,  17,  20,  130, 77,  37,  13,  26,  26,  28,
        10,  20,  130, 247, 34,  33,  34,  25,  130, 247, 34,  10,  11,  21,
        130, 120, 40,  7,   10,  10,  15,  21,  9,   10,  9,   16,  130, 12,
        134, 2,   33,  10,  9,   130, 8,   40,  14,  28,  24,  20,  23,  25,
        18,  18,  24,  132, 247, 38,  28,  24,  26,  18,  26,  20,  19,  130,
        247, 48,  29,  20,  20,  20,  10,  16,  11,  20,  17,  20,  19,  21,
        18,  21,  19,  11,  21,  134, 247, 131, 50,  33,  17,  13,  136, 247,
        39,  21,  24,  24,  23,  18,  24,  26,  23,  130, 133, 130, 2,   32,
        18,  130, 3,   32,  19,  132, 247, 132, 98,  132, 253, 32,  8,   131,
        10,  35,  23,  19,  21,  28,  130, 247, 35,  21,  30,  26,  27,  66,
        170, 5,   35,  21,  25,  28,  21,  130, 247, 39,  29,  21,  15,  10,
        21,  23,  21,  21,  130, 247, 40,  33,  10,  24,  24,  26,  34,  32,
        16,  33,  131, 247, 130, 16,  36,  20,  2,   21,  11,  11,  130, 25,
        32,  20,  130, 247, 34,  24,  18,  24,  133, 247, 34,  26,  26,  26,
        131, 247, 35,  19,  21,  20,  21,  132, 92,  36,  20,  19,  17,  8,
        26,  130, 247, 33,  18,  20,  130, 77,  37,  14,  26,  26,  29,  10,
        21,  130, 247, 52,  37,  39,  28,  0,   12,  12,  12,  15,  23,  23,
        24,  26,  9,   13,  13,  17,  23,  11,  12,  11,  19,  130, 55,  134,
        2,   33,  12,  12,  130, 8,   8,   47,  15,  32,  26,  22,  26,  28,
        21,  21,  27,  28,  12,  12,  24,  20,  34,  28,  29,  20,  29,  24,
        20,  23,  27,  25,  33,  23,  23,  22,  13,  18,  13,  23,  18,  23,
        21,  23,  19,  23,  21,  14,  23,  22,  10,  11,  21,  10,  34,  22,
        131, 50,  43,  19,  15,  22,  19,  29,  22,  19,  20,  13,  10,  13,
        23,  130, 137, 34,  21,  28,  29,  131, 205, 35,  21,  21,  21,  19,
        130, 3,   33,  21,  10,  130, 0,   131, 38,  130, 64,  51,  22,  22,
        22,  24,  10,  23,  23,  24,  23,  28,  21,  24,  32,  35,  22,  22,
        23,  33,  29,  32,  130, 64,  40,  23,  22,  23,  24,  28,  30,  24,
        15,  16,  130, 13,  42,  16,  12,  23,  26,  24,  23,  24,  20,  20,
        39,  12,  130, 182, 38,  37,  35,  19,  37,  15,  15,  11,  130, 152,
        37,  19,  23,  2,   23,  13,  13,  131, 59,  39,  11,  15,  36,  26,
        21,  26,  21,  21,  130, 201, 40,  12,  29,  29,  29,  27,  27,  27,
        10,  24,  130, 65,  130, 2,   52,  24,  23,  23,  20,  19,  10,  27,
        22,  23,  19,  20,  23,  23,  16,  16,  16,  29,  30,  32,  12,  23,
        130, 247, 43,  42,  43,  32,  0,   13,  13,  13,  15,  28,  27,  27,
        29,  130, 247, 37,  19,  27,  12,  13,  12,  21,  130, 55,  134, 2,
        33,  13,  12,  130, 8,   8,   34,  17,  37,  30,  24,  29,  31,  23,
        23,  30,  31,  12,  13,  28,  23,  36,  31,  33,  23,  33,  26,  22,
        27,  30,  28,  38,  27,  27,  25,  14,  20,  14,  27,  20,  26,  23,
        130, 117, 8,   32,  23,  16,  26,  26,  12,  13,  25,  12,  38,  26,
        25,  26,  26,  17,  21,  16,  26,  24,  34,  25,  24,  24,  13,  11,
        13,  27,  30,  30,  29,  23,  31,  33,  30,  65,  74,  5,   33,  21,
        23,  130, 0,   131, 159, 33,  26,  25,  131, 0,   32,  26,  130, 0,
        48,  27,  11,  27,  27,  27,  26,  28,  25,  26,  36,  35,  25,  26,
        27,  38,  33,  35,  131, 14,  63,  26,  26,  27,  32,  34,  26,  17,
        18,  32,  36,  25,  18,  13,  27,  30,  26,  27,  26,  22,  22,  43,
        13,  30,  30,  33,  42,  39,  21,  42,  15,  15,  12,  130, 152, 38,
        24,  27,  1,   27,  14,  14,  27,  130, 59,  36,  12,  15,  41,  30,
        23,  130, 96,  131, 87,  39,  33,  33,  33,  30,  30,  30,  12,  27,
        130, 88,  131, 2,   51,  25,  26,  22,  21,  11,  31,  25,  27,  24,
        23,  26,  27,  18,  18,  18,  34,  34,  37,  13,  27,  130, 247, 53,
        46,  49,  35,  0,   15,  15,  15,  18,  28,  29,  30,  32,  10,  15,
        16,  22,  29,  13,  15,  14,  22,  29,  136, 0,   33,  15,  15,  130,
        10,  8,   47,  19,  40,  32,  26,  32,  34,  25,  25,  33,  34,  14,
        15,  30,  25,  41,  34,  36,  25,  36,  29,  25,  30,  33,  32,  40,
        30,  30,  28,  16,  22,  16,  30,  23,  28,  26,  29,  24,  29,  27,
        17,  29,  28,  13,  15,  27,  13,  43,  28,  131, 50,  50,  23,  18,
        28,  26,  38,  28,  26,  27,  15,  12,  14,  29,  32,  32,  32,  25,
        34,  36,  33,  133, 136, 32,  24,  130, 167, 33,  27,  13,  130, 0,
        131, 38,  34,  29,  29,  28,  130, 0,   42,  29,  13,  29,  29,  30,
        29,  30,  27,  29,  40,  40,  130, 20,  34,  42,  36,  39,  130, 64,
        8,   39,  30,  28,  29,  30,  36,  38,  29,  19,  20,  35,  41,  29,
        20,  15,  29,  33,  29,  29,  29,  25,  25,  49,  15,  32,  32,  36,
        46,  44,  23,  47,  18,  18,  14,  14,  29,  30,  26,  30,  2,   29,
        132, 160, 41,  29,  14,  18,  46,  32,  25,  32,  25,  25,  14,  130,
        0,   34,  36,  36,  36,  130, 250, 39,  13,  26,  28,  29,  28,  29,
        27,  28,  130, 5,   35,  25,  23,  12,  35,  130, 44,  42,  25,  30,
        29,  20,  19,  19,  37,  37,  40,  15,  29,  130, 247, 52,  50,  52,
        38,  0,   16,  16,  15,  19,  32,  32,  33,  35,  11,  16,  16,  24,
        32,  15,  16,  15,  25,  130, 168, 134, 2,   32,  16,  130, 105, 8,
        72,  32,  20,  43,  34,  29,  35,  37,  27,  27,  36,  37,  15,  16,
        33,  27,  44,  38,  39,  28,  39,  32,  28,  32,  36,  34,  44,  32,
        32,  30,  17,  24,  17,  30,  24,  31,  28,  32,  26,  32,  29,  18,
        32,  32,  15,  15,  30,  15,  47,  32,  31,  32,  32,  21,  26,  20,
        32,  28,  40,  30,  28,  29,  17,  14,  17,  32,  34,  34,  35,  27,
        38,  39,  36,  28,  132, 0,   32,  26,  130, 193, 130, 173, 35,  15,
        15,  32,  31,  131, 0,   132, 103, 33,  13,  32,  130, 12,  42,  33,
        29,  32,  43,  42,  31,  31,  32,  46,  39,  43,  130, 20,  32,  31,
        130, 69,  43,  38,  41,  32,  21,  21,  38,  44,  31,  21,  16,  32,
        36,  130, 83,  44,  26,  26,  52,  16,  34,  34,  39,  50,  47,  25,
        50,  19,  19,  130, 63,  130, 131, 36,  2,   32,  16,  16,  32,  130,
        60,  40,  15,  19,  49,  34,  27,  34,  27,  27,  15,  130, 0,   34,
        39,  39,  39,  130, 250, 34,  15,  31,  30,  132, 1,   130, 99,  39,
        28,  26,  14,  38,  31,  32,  28,  28,  130, 149, 38,  21,  21,  39,
        40,  44,  16,  32,  130, 247, 53,  54,  55,  41,  0,   17,  17,  17,
        20,  36,  34,  35,  37,  12,  17,  17,  24,  34,  15,  18,  16,  27,
        34,  136, 0,   33,  17,  17,  130, 10,  8,   32,  23,  47,  38,  32,
        37,  40,  29,  30,  39,  40,  15,  16,  36,  29,  46,  40,  42,  30,
        42,  33,  30,  34,  38,  36,  48,  35,  34,  33,  18,  26,  18,  33,
        26,  130, 13,  8,   35,  27,  34,  30,  19,  34,  33,  15,  16,  32,
        15,  49,  33,  33,  34,  34,  22,  27,  20,  33,  30,  42,  32,  30,
        30,  17,  15,  17,  34,  38,  38,  37,  29,  40,  42,  38,  30,  132,
        0,   32,  27,  131, 5,   130, 158, 33,  15,  33,  137, 0,   32,  15,
        132, 111, 35,  31,  34,  47,  45,  130, 59,  34,  49,  42,  46,  130,
        14,  130, 8,   8,   50,  35,  42,  43,  34,  23,  24,  40,  46,  33,
        23,  17,  34,  39,  35,  34,  34,  28,  28,  55,  17,  38,  38,  42,
        54,  50,  27,  55,  20,  20,  16,  16,  34,  35,  30,  34,  2,   34,
        17,  18,  34,  34,  33,  33,  16,  20,  55,  38,  29,  38,  29,  29,
        131, 87,  40,  42,  42,  42,  38,  38,  38,  15,  33,  34,  130, 1,
        32,  32,  131, 99,  8,   42,  30,  27,  15,  41,  33,  34,  30,  30,
        33,  34,  23,  23,  23,  44,  43,  47,  18,  34,  0,   0,   0,   58,
        59,  44,  0,   18,  18,  18,  22,  35,  37,  38,  41,  12,  19,  19,
        27,  37,  17,  19,  17,  28,  37,  136, 0,   33,  19,  18,  130, 10,
        8,   70,  24,  50,  40,  35,  40,  43,  33,  32,  42,  44,  18,  19,
        39,  32,  52,  44,  45,  33,  45,  37,  32,  37,  42,  38,  52,  36,
        37,  35,  20,  28,  20,  35,  29,  36,  32,  37,  30,  37,  32,  22,
        37,  37,  18,  18,  35,  18,  54,  37,  35,  37,  37,  24,  30,  23,
        35,  32,  46,  35,  32,  32,  19,  15,  18,  37,  40,  40,  40,  33,
        44,  45,  42,  66,  69,  5,   32,  30,  131, 6,   130, 113, 34,  18,
        37,  35,  135, 0,   33,  37,  16,  130, 100, 43,  36,  36,  34,  38,
        50,  48,  36,  36,  37,  53,  45,  49,  130, 14,  47,  35,  37,  36,
        37,  44,  48,  37,  24,  25,  43,  49,  35,  25,  19,  37,  41,  130,
        18,  47,  31,  31,  59,  18,  40,  40,  45,  59,  54,  29,  58,  22,
        22,  17,  18,  37,  130, 131, 35,  3,   37,  20,  20,  130, 25,  40,
        36,  17,  22,  59,  40,  33,  40,  33,  33,  131, 87,  39,  45,  45,
        45,  42,  42,  42,  18,  36,  131, 0,   54,  35,  36,  36,  35,  36,
        32,  30,  15,  43,  35,  37,  32,  33,  36,  37,  25,  24,  25,  47,
        46,  51,  19,  37,  130, 247, 52,  67,  70,  51,  0,   21,  21,  21,
        26,  42,  42,  44,  46,  14,  21,  22,  31,  42,  19,  22,  20,  33,
        130, 55,  134, 2,   33,  22,  21,  130, 8,   8,   47,  28,  58,  46,
        38,  46,  50,  36,  36,  48,  50,  20,  22,  44,  35,  59,  50,  52,
        36,  52,  41,  36,  42,  48,  43,  58,  40,  42,  41,  23,  32,  23,
        44,  32,  42,  38,  42,  34,  42,  38,  24,  42,  41,  19,  21,  39,
        19,  61,  41,  131, 50,  51,  34,  26,  41,  36,  52,  40,  36,  38,
        22,  18,  22,  42,  46,  46,  46,  36,  50,  52,  48,  38,  132, 0,
        32,  34,  131, 5,   32,  19,  130, 0,   131, 38,  34,  42,  42,  41,
        130, 0,   42,  44,  18,  42,  42,  43,  43,  43,  39,  42,  58,  55,
        130, 20,  34,  61,  52,  57,  130, 64,  8,   54,  42,  41,  42,  43,
        51,  54,  43,  27,  29,  51,  58,  42,  29,  22,  42,  48,  42,  42,
        43,  36,  36,  70,  21,  46,  46,  52,  67,  63,  34,  68,  26,  26,
        20,  20,  42,  42,  36,  42,  3,   42,  23,  22,  43,  43,  44,  42,
        20,  26,  67,  46,  36,  46,  36,  36,  20,  130, 0,   41,  52,  52,
        52,  48,  48,  48,  19,  40,  41,  42,  130, 1,   130, 93,  50,  41,
        36,  34,  18,  51,  41,  42,  36,  36,  43,  42,  29,  28,  28,  53,
        54,  59,  22,  42,  130, 247, 53,  75,  76,  57,  0,   24,  24,  23,
        27,  49,  47,  49,  52,  15,  23,  23,  34,  47,  21,  24,  22,  37,
        47,  136, 0,   33,  24,  23,  130, 10,  8,   70,  31,  65,  53,  44,
        52,  56,  41,  41,  54,  55,  21,  23,  49,  40,  64,  55,  58,  41,
        58,  46,  41,  47,  53,  49,  66,  46,  47,  45,  25,  36,  25,  47,
        37,  46,  42,  47,  38,  47,  43,  28,  47,  47,  21,  23,  44,  21,
        69,  47,  46,  47,  47,  31,  39,  29,  47,  39,  57,  45,  39,  43,
        23,  20,  22,  47,  53,  53,  52,  41,  55,  58,  53,  65,  74,  5,
        32,  38,  130, 225, 33,  43,  21,  130, 0,   33,  47,  46,  131, 0,
        130, 94,  49,  47,  48,  20,  47,  47,  48,  48,  49,  44,  47,  65,
        62,  45,  46,  47,  69,  58,  63,  130, 20,  32,  46,  130, 18,  8,
        49,  57,  62,  47,  31,  32,  56,  65,  46,  33,  24,  47,  54,  48,
        47,  48,  39,  39,  74,  24,  53,  53,  58,  75,  70,  38,  76,  27,
        27,  22,  22,  47,  47,  39,  47,  4,   47,  24,  24,  48,  48,  48,
        46,  22,  27,  75,  53,  41,  53,  41,  41,  130, 86,  40,  21,  58,
        58,  58,  53,  53,  53,  21,  45,  131, 93,  33,  45,  46,  130, 6,
        49,  41,  39,  20,  57,  45,  47,  39,  41,  46,  47,  32,  32,  32,
        61,  60,  66,  24,  47,  130, 247, 53,  83,  83,  63,  0,   26,  26,
        26,  30,  53,  52,  54,  58,  18,  25,  25,  39,  52,  24,  26,  24,
        41,  52,  136, 0,   33,  26,  26,  130, 10,  36,  34,  71,  57,  49,
        57,  130, 146, 8,   62,  60,  62,  24,  26,  55,  44,  70,  62,  64,
        46,  64,  53,  45,  52,  59,  53,  72,  52,  52,  50,  28,  40,  28,
        53,  41,  51,  46,  52,  42,  52,  46,  29,  52,  52,  24,  25,  49,
        24,  76,  52,  50,  52,  52,  34,  43,  32,  52,  43,  63,  51,  43,
        47,  26,  22,  26,  52,  57,  57,  57,  45,  62,  64,  59,  132, 231,
        33,  46,  42,  131, 6,   32,  24,  130, 0,   33,  52,  50,  131, 0,
        130, 94,  49,  52,  52,  22,  52,  52,  53,  52,  53,  49,  53,  71,
        68,  51,  51,  52,  76,  64,  70,  130, 20,  32,  51,  130, 18,  8,
        49,  64,  69,  52,  34,  36,  62,  71,  50,  36,  26,  52,  59,  52,
        52,  54,  43,  43,  83,  26,  57,  57,  64,  83,  77,  41,  83,  30,
        30,  24,  24,  52,  53,  43,  52,  3,   53,  28,  28,  53,  53,  52,
        51,  24,  30,  82,  57,  45,  57,  45,  45,  130, 86,  41,  24,  64,
        64,  64,  59,  59,  59,  24,  53,  51,  130, 0,   32,  50,  130, 3,
        50,  51,  45,  43,  22,  62,  49,  52,  43,  46,  51,  52,  35,  35,
        35,  66,  67,  73,  26,  53,  130, 247, 48,  92,  92,  69,  0,   29,
        29,  29,  34,  59,  58,  60,  65,  20,  28,  28,  43,  58,  130, 12,
        33,  46,  58,  136, 0,   33,  29,  29,  130, 10,  8,   70,  38,  79,
        63,  53,  64,  69,  50,  50,  66,  69,  27,  29,  61,  49,  78,  69,
        71,  50,  71,  59,  50,  57,  66,  59,  80,  57,  57,  56,  30,  45,
        31,  56,  46,  57,  51,  58,  47,  58,  52,  34,  58,  58,  27,  28,
        55,  27,  85,  58,  57,  58,  58,  38,  47,  37,  56,  46,  68,  56,
        46,  53,  28,  25,  28,  58,  63,  63,  64,  50,  69,  71,  66,  131,
        131, 35,  51,  51,  47,  52,  130, 0,   32,  27,  130, 0,   33,  58,
        57,  131, 0,   32,  56,  130, 0,   33,  57,  24,  130, 100, 43,  58,
        60,  54,  58,  79,  75,  56,  57,  58,  83,  71,  79,  130, 14,  63,
        56,  58,  57,  59,  70,  76,  58,  38,  39,  69,  79,  57,  40,  29,
        58,  65,  58,  58,  61,  48,  48,  92,  29,  63,  63,  71,  92,  86,
        46,  92,  34,  34,  131, 152, 49,  46,  57,  5,   58,  31,  30,  59,
        59,  57,  58,  29,  34,  92,  63,  50,  63,  50,  50,  130, 86,  39,
        27,  71,  71,  71,  66,  66,  66,  27,  131, 94,  55,  56,  55,  57,
        55,  56,  57,  50,  47,  25,  70,  56,  57,  46,  50,  58,  58,  39,
        39,  39,  74,  74,  80,  29,  58,  130, 247, 48,  100, 101, 75,  0,
        32,  32,  32,  37,  62,  63,  65,  70,  21,  31,  31,  46,  63,  130,
        12,  33,  49,  63,  136, 0,   33,  32,  31,  130, 10,  8,   71,  42,
        86,  69,  59,  69,  75,  55,  55,  72,  75,  30,  32,  66,  53,  86,
        75,  78,  55,  78,  63,  54,  62,  72,  65,  86,  61,  62,  60,  31,
        48,  32,  65,  49,  62,  55,  63,  51,  63,  56,  37,  63,  62,  29,
        31,  58,  29,  93,  62,  62,  63,  63,  41,  51,  40,  61,  50,  74,
        60,  50,  57,  32,  27,  31,  63,  69,  69,  69,  55,  75,  78,  72,
        55,  132, 0,   32,  51,  130, 232, 33,  56,  29,  130, 0,   32,  62,
        132, 0,   32,  61,  130, 0,   48,  64,  26,  63,  63,  64,  63,  64,
        58,  64,  86,  79,  61,  62,  63,  90,  78,  84,  130, 115, 130, 68,
        44,  64,  77,  82,  63,  41,  43,  75,  86,  62,  44,  32,  63,  72,
        130, 33,  55,  53,  53,  101, 32,  69,  69,  78,  101, 93,  50,  100,
        37,  37,  32,  32,  63,  63,  50,  62,  5,   63,  33,  33,  64,  130,
        0,   40,  32,  37,  100, 69,  55,  69,  55,  55,  30,  130, 0,   38,
        78,  78,  78,  72,  72,  72,  29,  135, 93,  51,  62,  61,  54,  51,
        27,  74,  60,  62,  50,  55,  63,  63,  42,  42,  43,  81,  80,  87,
        32,  63,  130, 247, 34,  0,   2,   1,   130, 5,   8,   35,  5,   0,
        5,   0,   0,   3,   0,   7,   0,   86,  177, 5,   4,   184, 1,   32,
        178, 0,   6,   7,   187, 1,   32,  0,   2,   0,   1,   2,   25,  181,
        3,   3,   0,   10,  6,   5,   130, 22,  38,  182, 3,   3,   2,   74,
        9,   7,   131, 33,  8,   35,  180, 1,   0,   73,  8,   9,   188, 2,
        12,  0,   33,  0,   249, 1,   24,  0,   24,  43,  43,  78,  244, 60,
        77,  253, 60,  78,  16,  246, 60,  77,  16,  253, 60,  0,   63,  60,
        130, 5,   32,  253, 131, 4,   38,  49,  48,  33,  17,  33,  17,  37,
        130, 4,   49,  1,   0,   4,   0,   252, 64,  3,   128, 252, 128, 5,
        0,   251, 0,   64,  4,   128, 0,   130, 98,  38,  225, 0,   0,   1,
        166, 5,   200, 130, 127, 55,  9,   0,   75,  64,  35,  7,   2,   9,
        148, 2,   106, 64,  0,   10,  11,  246, 3,   9,   100, 7,   26,  7,
        3,   106, 130, 41,  43,  100, 6,   26,  6,   0,   73,  10,  11,  242,
        33,  136, 196, 133, 107, 45,  75,  82,  88,  176, 0,   26,  89,  77,
        16,  236, 26,  16,  253, 60,  134, 13,  8,   39,  16,  236, 16,  230,
        0,   63,  26,  253, 230, 63,  49,  48,  51,  53,  51,  21,  3,   3,
        17,  51,  17,  3,   225, 197, 172, 25,  197, 24,  197, 197, 1,   139,
        3,   21,  1,   40,  254, 216, 252, 235, 131, 119, 38,  111, 4,   62,
        2,   142, 6,   43,  132, 247, 58,  96,  64,  36,  5,   2,   0,   4,
        3,   6,   9,   71,  71,  74,  7,   100, 26,  6,   106, 64,  4,   100,
        26,  5,   93,  3,   100, 26,  131, 139, 43,  100, 26,  1,   73,  8,
        86,  92,  24,  43,  78,  16,  244, 134, 105, 35,  77,  236, 26,  253,
        134, 10,  33,  236, 246, 133, 8,   36,  27,  176, 64,  26,  89,  138,
        22,  35,  78,  69,  101, 68,  130, 141, 39,  60,  63,  60,  49,  48,
        19,  3,   51,  132, 1,   32,  136, 131, 136, 40,  25,  198, 25,  4,
        62,  1,   237, 254, 19,  131, 3,   8,   116, 0,   2,   0,   15,  0,
        0,   5,   0,   5,   200, 0,   27,  0,   31,  1,   49,  64,  128, 16,
        167, 15,  95,  21,  1,   21,  139, 22,  80,  7,   1,   7,   139, 8,
        2,   167, 1,   1,   2,   21,  0,   9,   4,   3,   20,  0,   9,   5,
        6,   17,  0,   9,   8,   7,   16,  0,   9,   11,  7,   16,  27,  10,
        12,  7,   16,  24,  13,  15,  7,   16,  23,  14,  18,  6,   17,  23,
        14,  19,  3,   20,  23,  14,  22,  2,   21,  23,  14,  25,  2,   21,
        24,  13,  26,  2,   21,  27,  10,  28,  3,   20,  27,  10,  29,  3,
        20,  24,  13,  30,  6,   17,  24,  13,  31,  6,   17,  27,  10,  33,
        130, 229, 8,   44,  14,  14,  23,  23,  98,  24,  13,  20,  24,  24,
        13,  10,  27,  27,  98,  0,   9,   20,  0,   0,   9,   7,   16,  98,
        6,   17,  184, 1,   58,  64,  14,  14,  9,   10,  10,  13,  13,  14,
        4,   21,  2,   98,  64,  20,  3,   131, 18,  8,   60,  12,  0,   24,
        27,  27,  0,   10,  21,  156, 16,  224, 14,  33,  184, 2,   72,  64,
        29,  13,  182, 24,  27,  182, 10,  184, 23,  182, 14,  74,  33,  24,
        184, 128, 9,   182, 0,   7,   156, 2,   224, 0,   73,  32,  2,   120,
        158, 24,  43,  141, 25,  78,  16,  244, 77,  244, 24,  228, 16,  228,
        26,  236, 130, 11,  36,  246, 24,  77,  228, 244, 130, 12,  34,  25,
        228, 16,  130, 20,  42,  0,   63,  60,  16,  60,  16,  244, 60,  26,
        253, 60,  132, 9,   131, 11,  43,  253, 60,  135, 5,   46,  43,  125,
        16,  196, 135, 46,  24,  131, 6,   32,  1,   130, 53,  36,  69,  101,
        68,  230, 15,  142, 0,   34,  24,  77,  16,  130, 71,  34,  93,  16,
        228, 130, 2,   38,  49,  48,  51,  19,  33,  55,  33,  132, 3,   35,
        51,  3,   33,  19,  130, 3,   35,  7,   33,  3,   33,  130, 3,   8,
        39,  35,  19,  33,  3,   19,  33,  19,  33,  203, 111, 254, 213, 25,
        1,   49,  87,  254, 188, 25,  1,   73,  112, 127, 111, 1,   7,   111,
        128, 111, 1,   43,  24,  254, 206, 87,  1,   68,  25,  254, 183, 130,
        13,  50,  254, 249, 111, 142, 1,   7,   87,  254, 248, 1,   188, 124,
        1,   89,  123, 1,   188, 254, 68,  131, 3,   35,  123, 254, 167, 124,
        133, 9,   63,  2,   56,  1,   89,  0,   0,   3,   0,   216, 255, 133,
        4,   31,  6,   68,  0,   35,  0,   42,  0,   50,  0,   174, 64,  15,
        49,  8,   29,  41,  4,   36,  44,  132, 6,   50,  39,  47,  22,  65,
        11,  1,   37,  0,   21,  1,   3,   0,   15,  0,   25,  1,   30,  0,
        44,  130, 13,  49,  18,  0,   16,  1,   75,  180, 15,  4,   4,   204,
        3,   189, 1,   4,   0,   1,   0,   6,   130, 24,  8,   39,  36,  1,
        37,  178, 34,  34,  0,   184, 1,   75,  182, 1,   12,  22,  170, 31,
        192, 39,  184, 1,   79,  182, 35,  18,  26,  26,  36,  36,  35,  184,
        1,   198, 183, 0,   15,  43,  43,  6,   6,   16,  130, 33,  57,  79,
        183, 47,  192, 11,  170, 3,   73,  51,  243, 185, 1,   25,  0,   24,
        43,  78,  16,  244, 77,  244, 253, 246, 60,  60,  16,  131, 1,   32,
        253, 133, 6,   46,  246, 253, 228, 0,   63,  236, 60,  16,  253, 228,
        25,  16,  252, 24,  237, 130, 10,  134, 9,   37,  1,   17,  18,  23,
        57,  0,   131, 4,   8,   231, 49,  48,  5,   53,  34,  39,  53,  22,
        51,  17,  46,  2,   53,  52,  54,  54,  55,  53,  51,  21,  50,  22,
        23,  21,  38,  38,  39,  17,  22,  23,  22,  22,  21,  20,  6,   7,
        21,  17,  54,  54,  53,  52,  38,  39,  3,   17,  6,   6,   21,  20,
        22,  22,  2,   99,  176, 219, 226, 169, 166, 125, 61,  86,  147, 119,
        98,  46,  186, 89,  96,  132, 93,  54,  27,  173, 92,  180, 166, 92,
        81,  40,  133, 98,  93,  86,  33,  61,  123, 123, 83,  170, 105, 2,
        19,  102, 123, 150, 71,  88,  161, 97,  9,   124, 124, 33,  34,  161,
        42,  44,  7,   253, 241, 33,  16,  105, 151, 99,  148, 231, 35,  123,
        1,   27,  33,  124, 68,  47,  97,  92,  1,   6,   1,   200, 29,  114,
        67,  41,  88,  63,  0,   5,   0,   12,  255, 219, 5,   77,  5,   237,
        0,   3,   0,   15,  0,   27,  0,   39,  0,   51,  0,   248, 64,  47,
        104, 8,   1,   0,   0,   98,  3,   2,   20,  3,   3,   2,   16,  116,
        4,   186, 22,  116, 10,  1,   2,   5,   10,  4,   46,  116, 34,  186,
        40,  116, 28,  3,   0,   13,  28,  12,  53,  71,  71,  74,  37,  1,
        94,  2,   115, 37,  49,  184, 2,   63,  178, 31,  254, 43,  186, 2,
        63,  0,   37,  1,   64,  182, 7,   3,   94,  0,   115, 7,   19,  184,
        130, 21,  34,  13,  254, 25,  130, 6,   37,  181, 7,   73,  52,  97,
        102, 65,  48,  5,   41,  237, 253, 237, 25,  16,  244, 24,  237, 16,
        253, 135, 9,   33,  78,  16,  67,  157, 5,   35,  63,  60,  77,  16,
        130, 19,  34,  63,  63,  60,  131, 6,   33,  49,  48,  66,  129, 6,
        50,  24,  67,  121, 64,  82,  5,   51,  17,  15,  19,  99,  1,   27,
        5,   25,  99,  0,   21,  11,  130, 9,   33,  23,  9,   130, 9,   43,
        41,  39,  43,  99,  1,   51,  29,  49,  99,  0,   45,  35,  130, 9,
        33,  47,  33,  130, 9,   38,  18,  14,  16,  99,  0,   26,  6,   130,
        4,   38,  20,  12,  22,  99,  1,   24,  8,   130, 4,   38,  42,  38,
        40,  99,  0,   50,  30,  130, 4,   38,  44,  36,  46,  99,  1,   48,
        32,  130, 4,   33,  0,   43,  134, 0,   32,  1,   134, 7,   62,  43,
        129, 23,  1,   51,  1,   19,  34,  38,  53,  52,  54,  51,  50,  22,
        21,  20,  6,   39,  50,  54,  53,  52,  38,  35,  34,  6,   21,  20,
        22,  1,   150, 23,  8,   64,  24,  4,   142, 154, 251, 115, 148, 148,
        167, 168, 147, 146, 168, 167, 147, 77,  89,  90,  76,  77,  90,  90,
        3,   24,  147, 167, 167, 147, 147, 168, 167, 148, 77,  90,  91,  76,
        76,  90,  89,  37,  6,   18,  249, 238, 3,   9,   205, 165, 166, 204,
        204, 166, 165, 205, 99,  150, 121, 122, 150, 150, 122, 121, 150, 252,
        185, 144, 18,  8,   75,  0,   3,   0,   74,  255, 219, 5,   61,  5,
        237, 0,   33,  0,   45,  0,   55,  0,   241, 64,  18,  62,  8,   31,
        23,  21,  21,  20,  46,  46,  33,  1,   34,  37,  10,  0,   37,  0,
        0,   184, 1,   206, 64,  51,  33,  21,  20,  33,  33,  21,  34,  23,
        31,  1,   4,   3,   10,  37,  46,  21,  4,   16,  28,  29,  29,  3,
        51,  88,  16,  3,   33,  0,   10,  44,  88,  3,   11,  132, 26,  32,
        29,  132, 26,  37,  48,  40,  28,  106, 29,  115, 130, 55,  44,  137,
        179, 33,  74,  57,  19,  184, 2,   60,  178, 48,  208, 54,  131, 6,
        34,  12,  149, 40,  130, 6,   57,  179, 7,   73,  56,  57,  184, 2,
        24,  179, 33,  109, 232, 24,  43,  43,  78,  244, 77,  237, 244, 253,
        246, 237, 78,  16,  246, 130, 8,   36,  237, 17,  18,  23,  57,  130,
        2,   42,  0,   63,  237, 63,  60,  63,  237, 18,  57,  47,  60,  133,
        16,  48,  135, 5,   46,  43,  14,  125, 16,  196, 135, 14,  196, 196,
        196, 135, 16,  8,   196, 130, 7,   8,   47,  49,  48,  24,  67,  121,
        64,  44,  49,  53,  41,  43,  13,  18,  4,   6,   42,  38,  5,   37,
        14,  38,  43,  4,   40,  183, 0,   50,  17,  48,  99,  1,   52,  15,
        54,  99,  0,   41,  6,   44,  183, 0,   49,  18,  51,  99,  1,   53,
        13,  130, 4,   35,  0,   43,  43,  43,  65,  135, 6,   32,  129, 130,
        0,   42,  33,  39,  6,   35,  34,  38,  38,  53,  52,  18,  55,  130,
        4,   32,  54,  65,  142, 5,   47,  5,   18,  23,  62,  2,   53,  39,
        51,  16,  7,   22,  23,  37,  38,  38,  39,  67,  23,  5,   34,  51,
        50,  3,   65,  164, 8,   8,   136, 4,   37,  73,  164, 219, 146, 247,
        138, 160, 201, 100, 94,  165, 99,  145, 186, 254, 187, 149, 198, 37,
        33,  21,  1,   196, 196, 90,  115, 254, 73,  139, 163, 98,  135, 96,
        101, 161, 104, 139, 149, 212, 82,  72,  82,  70,  88,  125, 117, 228,
        132, 139, 1,   1,   68,  181, 120, 83,  145, 84,  174, 125, 222, 164,
        254, 248, 229, 54,  91,  150, 81,  72,  254, 184, 238, 114, 107, 202,
        171, 244, 170, 54,  155, 89,  96,  186, 96,  3,   78,  97,  161, 66,
        88,  100, 55,  102, 0,   0,   1,   0,   111, 4,   12,  1,   102, 6,
        43,  0,   3,   0,   47,  185, 0,   3,   1,   15,  64,  21,  1,   0,
        5,   71,  71,  74,  2,   108, 3,   94,  0,   108, 1,   73,  4,   5,
        180, 33,  86,  92,  65,  63,  5,   35,  244, 253, 244, 78,  66,  217,
        5,   33,  77,  237, 70,  118, 5,   41,  160, 49,  247, 50,  4,   12,
        2,   31,  253, 225, 131, 75,  36,  123, 254, 216, 2,   80,  130, 75,
        57,  17,  0,   82,  64,  32,  38,  2,   38,  8,   54,  2,   54,  8,
        4,   10,  94,  9,   16,  0,   94,  1,   18,  15,  19,  1,   19,  130,
        88,  56,  9,   0,   1,   1,   10,  9,   149, 14,  184, 2,   60,  64,
        9,   0,   5,   95,  5,   2,   5,   73,  18,  112, 185, 1,   28,  68,
        127, 5,   39,  93,  77,  253, 244, 60,  60,  16,  60,  67,  67,  5,
        37,  93,  0,   63,  77,  237, 63,  130, 108, 8,   56,  1,   93,  5,
        21,  38,  2,   2,   53,  52,  18,  18,  55,  21,  6,   6,   2,   21,
        20,  18,  22,  2,   80,  163, 202, 104, 104, 202, 163, 113, 108, 51,
        51,  108, 160, 136, 111, 1,   52,  1,   94,  169, 168, 1,   94,  1,
        52,  111, 136, 122, 231, 254, 223, 159, 160, 254, 223, 230, 131, 151,
        32,  74,  130, 151, 32,  31,  132, 151, 43,  83,  64,  28,  41,  2,
        41,  8,   57,  2,   57,  8,   4,   130, 147, 32,  16,  130, 155, 33,
        18,  0,   133, 151, 35,  56,  5,   1,   5,   131, 147, 34,  18,  14,
        149, 132, 162, 47,  15,  9,   95,  9,   2,   9,   73,  18,  109, 152,
        24,  43,  78,  16,  244, 93,  131, 148, 41,  77,  16,  244, 253, 93,
        78,  69,  101, 68,  230, 138, 152, 50,  19,  53,  22,  18,  18,  21,
        20,  2,   2,   7,   53,  54,  54,  18,  53,  52,  2,   38,  74,  135,
        151, 8,   37,  50,  50,  108, 5,   163, 136, 111, 254, 204, 254, 161,
        167, 168, 254, 161, 254, 203, 110, 136, 123, 230, 1,   34,  159, 157,
        1,   35,  231, 0,   5,   0,   110, 2,   239, 3,   109, 5,   200, 130,
        9,   8,   50,  11,  0,   17,  0,   23,  0,   30,  0,   125, 64,  57,
        5,   30,  100, 24,  26,  100, 25,  23,  188, 2,   188, 20,  14,  30,
        17,  26,  8,   11,  5,   8,   28,  12,  149, 7,   149, 128, 8,   239,
        64,  25,  181, 128, 28,  17,  189, 22,  8,   189, 20,  23,  130, 27,
        49,  11,  5,   2,   8,   22,  32,  184, 3,   239, 22,  184, 2,   31,
        32,  184, 1,   250, 179, 65,  170, 5,   50,  141, 244, 254, 228, 18,
        23,  57,  236, 16,  236, 0,   47,  26,  237, 26,  254, 26,  236, 236,
        130, 14,  51,  125, 230, 230, 49,  48,  1,   16,  228, 16,  228, 67,
        88,  64,  15,  6,   1,   6,   18,  2,   8,   131, 5,   8,   145, 23,
        1,   23,  18,  4,   0,   93,  94,  93,  89,  1,   23,  5,   55,  52,
        39,  19,  7,   3,   54,  54,  55,  3,   39,  55,  22,  22,  23,  37,
        55,  5,   6,   21,  23,  19,  51,  3,   38,  35,  34,  7,   3,   64,
        45,  254, 209, 3,   29,  229, 120, 157, 20,  41,  9,   239, 120, 206,
        9,   42,  19,  254, 135, 46,  1,   28,  29,  2,   7,   148, 31,  18,
        25,  25,  18,  5,   9,   141, 66,  15,  41,  26,  254, 186, 87,  1,
        11,  2,   29,  20,  254, 194, 87,  231, 21,  28,  2,   130, 141, 125,
        25,  42,  15,  1,   142, 254, 204, 11,  11,  0,   0,   1,   0,   105,
        0,   49,  4,   167, 4,   111, 0,   11,  0,   44,  64,  23,  5,   194,
        7,   4,   94,  10,  1,   194, 0,   8,   194, 6,   11,  85,  5,   0,
        194, 2,   73,  12,  86,  73,  12,  5,   39,  77,  244, 60,  253, 60,
        228, 0,   47,  132, 6,   57,  49,  48,  37,  17,  33,  53,  33,  17,
        51,  17,  33,  21,  33,  17,  2,   62,  254, 43,  1,   213, 148, 1,
        213, 254, 43,  49,  134, 7,   34,  148, 254, 43,  130, 91,  8,   52,
        200, 254, 191, 1,   191, 0,   247, 0,   12,  0,   67,  64,  41,  7,
        11,  23,  11,  39,  11,  55,  11,  87,  11,  5,   1,   200, 64,  0,
        139, 128, 9,   166, 7,   10,  6,   94,  64,  67,  14,  148, 0,   166,
        128, 10,  166, 7,   73,  13,  14,  180, 33,  120, 158, 66,  220, 5,
        8,   49,  253, 26,  237, 230, 118, 26,  237, 24,  0,   63,  237, 26,
        252, 26,  237, 49,  48,  1,   93,  19,  53,  62,  2,   53,  53,  35,
        53,  51,  21,  20,  6,   200, 32,  38,  26,  96,  247, 117, 254, 191,
        74,  9,   50,  76,  92,  20,  247, 214, 171, 170, 130, 111, 55,  111,
        2,   31,  2,   43,  2,   179, 0,   3,   0,   29,  64,  15,  1,   85,
        0,   2,   74,  5,   0,   73,  4,   5,   226, 65,  135, 5,   52,  78,
        228, 16,  230, 0,   47,  77,  253, 49,  48,  19,  53,  33,  21,  111,
        1,   188, 2,   31,  148, 148, 130, 53,  34,  200, 0,   0,   132, 165,
        46,  3,   0,   38,  64,  19,  2,   166, 0,   10,  5,   71,  71,  74,
        2,   180, 131, 57,  138, 143, 67,  106, 10,  32,  51,  130, 130, 35,
        200, 247, 247, 247, 130, 59,  38,  12,  254, 216, 3,   231, 6,   43,
        130, 113, 38,  68,  64,  12,  1,   0,   0,   85,  70,  211, 5,   39,
        0,   3,   184, 1,   113, 64,  16,  2,   130, 17,  34,  3,   1,   4,
        130, 134, 32,  1,   130, 134, 38,  238, 33,  184, 1,   168, 177, 217,
        130, 223, 32,  25,  131, 138, 42,  17,  18,  57,  57,  0,   24,  63,
        60,  77,  253, 57,  70,  137, 6,   130, 152, 8,   63,  1,   51,  1,
        12,  3,   57,  162, 252, 199, 254, 216, 7,   83,  248, 173, 0,   2,
        0,   130, 255, 219, 4,   142, 5,   237, 0,   15,  0,   27,  0,   114,
        64,  20,  67,  8,   22,  145, 8,   5,   16,  145, 0,   13,  19,  206,
        12,  74,  29,  25,  206, 4,   73,  28,  184, 1,   36,  177, 231, 24,
        43,  78,  16,  244, 77,  69,  79,  5,   8,   47,  0,   63,  237, 63,
        237, 49,  48,  67,  121, 64,  50,  1,   27,  14,  38,  2,   37,  10,
        37,  6,   38,  17,  15,  19,  225, 1,   27,  1,   25,  225, 0,   21,
        9,   19,  225, 1,   23,  7,   25,  225, 0,   18,  13,  16,  225, 0,
        26,  3,   130, 4,   38,  20,  11,  22,  225, 1,   24,  5,   130, 4,
        32,  43,  69,  43,  9,   8,   71,  43,  43,  129, 5,   34,  38,  2,
        53,  52,  18,  54,  51,  50,  22,  18,  21,  20,  2,   6,   39,  50,
        18,  17,  16,  2,   35,  34,  2,   17,  16,  18,  2,   136, 166, 238,
        114, 113, 240, 165, 166, 239, 113, 112, 239, 167, 162, 147, 147, 162,
        162, 146, 146, 37,  213, 1,   107, 201, 200, 1,   107, 214, 213, 254,
        149, 201, 200, 254, 151, 216, 148, 1,   90,  130, 118, 38,  27,  1,
        90,  254, 166, 254, 229, 130, 1,   8,   48,  166, 0,   0,   1,   1,
        46,  0,   0,   2,   222, 5,   213, 0,   5,   0,   53,  177, 1,   2,
        184, 1,   202, 183, 3,   170, 4,   4,   0,   12,  1,   0,   184, 1,
        205, 183, 5,   5,   4,   74,  7,   2,   73,  6,   186, 1,   139, 1,
        187, 0,   131, 217, 32,  228, 76,  107, 8,   8,   74,  63,  244, 253,
        60,  49,  48,  33,  17,  35,  53,  37,  17,  2,   24,  234, 1,   176,
        5,   52,  124, 37,  250, 43,  0,   1,   0,   170, 0,   0,   4,   34,
        5,   237, 0,   29,  0,   131, 64,  20,  22,  8,   24,  23,  3,   4,
        4,   10,  27,  1,   28,  0,   10,  145, 17,  64,  14,  204, 80,  15,
        184, 1,   4,   179, 17,  5,   28,  27,  184, 2,   57,  64,  12,  29,
        0,   12,  132, 33,  55,  15,  7,   206, 20,  184, 1,   30,  179, 29,
        74,  31,  27,  184, 1,   135, 180, 15,  170, 0,   73,  30,  186, 1,
        41,  73,  129, 8,   57,  228, 237, 78,  16,  246, 77,  244, 237, 18,
        23,  57,  0,   63,  60,  253, 60,  63,  252, 26,  237, 26,  16,  237,
        17,  18,  57,  73,  107, 5,   60,  67,  121, 64,  14,  18,  19,  8,
        9,   9,   18,  7,   143, 1,   8,   19,  10,  143, 1,   43,  1,   43,
        129, 129, 51,  53,  54,  37,  55,  54,  70,  59,  6,   34,  6,   7,
        53,  70,  100, 5,   8,   56,  6,   7,   7,   6,   6,   7,   33,  21,
        170, 86,  1,   6,   113, 123, 95,  152, 128, 55,  101, 115, 91,  197,
        195, 213, 246, 101, 199, 76,  83,  168, 23,  2,   131, 173, 201, 237,
        101, 112, 181, 98,  125, 141, 21,  46,  52,  174, 93,  234, 175, 114,
        202, 170, 65,  70,  201, 113, 173, 130, 223, 36,  208, 255, 219, 4,
        47,  130, 223, 8,   58,  39,  0,   200, 64,  20,  70,  8,   31,  12,
        9,   31,  15,  11,  12,  145, 9,   9,   3,   18,  145, 24,  20,  204,
        64,  21,  184, 1,   3,   64,  24,  13,  24,  5,   3,   145, 38,  1,
        204, 64,  0,   170, 13,  38,  13,  15,  192, 27,  170, 6,   206, 35,
        74,  41,  11,  184, 1,   75,  181, 21,  130, 219, 32,  40,  74,  92,
        10,  33,  228, 228, 131, 218, 36,  237, 244, 237, 0,   63,  76,  215,
        7,   34,  80,  26,  89,  130, 223, 33,  16,  237, 143, 16,  37,  253,
        17,  57,  47,  237, 1,   130, 244, 32,  0,   130, 3,   33,  49,  48,
        130, 244, 48,  51,  25,  37,  4,   17,  33,  37,  29,  38,  17,  25,
        15,  143, 1,   4,   37,  6,   130, 253, 8,   35,  32,  6,   143, 3,
        13,  30,  15,  143, 3,   16,  26,  18,  143, 1,   5,   36,  3,   143,
        0,   7,   34,  9,   143, 1,   32,  14,  28,  12,  143, 0,   30,  31,
        16,  60,  43,  60,  66,  94,  9,   37,  129, 129, 55,  53,  22,  51,
        73,  4,   5,   34,  35,  53,  55,  133, 8,   34,  34,  7,   53,  71,
        144, 6,   8,   169, 6,   6,   7,   30,  2,   21,  20,  2,   33,  34,
        208, 211, 121, 141, 179, 183, 254, 78,  69,  237, 173, 137, 135, 146,
        178, 127, 142, 68,  235, 222, 64,  130, 115, 145, 141, 72,  238, 254,
        233, 138, 11,  184, 84,  165, 131, 128, 184, 133, 1,   173, 116, 103,
        124, 83,  172, 43,  16,  188, 155, 87,  131, 113, 41,  34,  106, 160,
        92,  174, 254, 239, 0,   0,   2,   0,   105, 0,   0,   4,   130, 5,
        200, 0,   10,  0,   13,  0,   141, 64,  12,  5,   155, 13,  188, 13,
        2,   13,  11,  5,   13,  11,  11,  184, 1,   128, 64,  9,   3,   4,
        20,  3,   3,   4,   3,   7,   11,  190, 2,   56,  0,   8,   0,   2,
        1,   225, 0,   0,   0,   13,  1,   135, 180, 4,   4,   0,   12,  13,
        184, 1,   30,  179, 0,   240, 10,  4,   184, 1,   79,  178, 5,   5,
        10,  184, 1,   204, 179, 7,   74,  15,  11,  184, 2,   94,  179, 3,
        2,   73,  14,  184, 1,   127, 67,  125, 6,   32,  60,  67,  126, 5,
        8,   92,  244, 60,  16,  230, 16,  253, 228, 0,   63,  63,  237, 16,
        244, 60,  253, 60,  57,  135, 5,   46,  43,  4,   125, 16,  196, 135,
        196, 49,  48,  0,   93,  67,  88,  182, 24,  11,  24,  13,  148, 13,
        3,   0,   93,  89,  33,  17,  33,  53,  1,   51,  17,  51,  21,  35,
        17,  1,   33,  17,  3,   3,   253, 102, 2,   154, 185, 198, 198, 253,
        110, 1,   230, 1,   163, 149, 3,   144, 252, 124, 161, 254, 93,  2,
        68,  2,   146, 0,   1,   0,   249, 255, 219, 4,   28,  130, 197, 8,
        54,  23,  0,   133, 178, 34,  8,   16,  184, 1,   37,  179, 11,  11,
        22,  15,  184, 2,   57,  64,  32,  12,  4,   3,   145, 22,  1,   204,
        64,  0,   170, 11,  22,  13,  14,  170, 6,   206, 47,  19,  1,   19,
        74,  25,  15,  16,  240, 12,  11,  170, 0,   73,  24,  186, 1,   211,
        66,  208, 8,   131, 147, 37,  78,  16,  246, 93,  77,  237, 130, 163,
        65,  247, 16,  35,  237, 18,  57,  47,  68,  59,  5,   58,  24,  17,
        21,  4,   8,   4,   21,  6,   143, 1,   8,   17,  6,   225, 1,   5,
        20,  3,   143, 0,   7,   18,  9,   225, 1,   43,  43,  130, 2,   34,
        129, 129, 23,  65,  188, 8,   8,   58,  34,  7,   17,  33,  21,  33,
        17,  32,  0,   21,  20,  0,   33,  34,  249, 147, 133, 160, 153, 223,
        236, 53,  56,  2,   237, 253, 192, 1,   56,  1,   37,  254, 247, 254,
        230, 114, 6,   176, 59,  189, 133, 152, 182, 7,   2,   208, 172, 254,
        122, 254, 227, 209, 192, 254, 243, 0,   2,   0,   137, 130, 211, 8,
        71,  124, 5,   238, 0,   27,  0,   39,  0,   223, 64,  30,  19,  22,
        19,  23,  35,  22,  35,  23,  4,   36,  64,  4,   31,  70,  8,   0,
        37,  4,   145, 31,  31,  12,  22,  25,  145, 19,  23,  204, 64,  22,
        184, 1,   3,   183, 13,  19,  5,   37,  145, 12,  13,  23,  184, 1,
        4,   181, 28,  192, 8,   74,  41,  0,   184, 1,   118, 180, 34,  206,
        15,  73,  40,  69,  6,   10,  33,  253, 229, 130, 224, 132, 223, 66,
        200, 16,  33,  237, 17,  131, 224, 8,   46,  18,  57,  49,  48,  67,
        121, 64,  52,  35,  39,  26,  30,  5,   18,  10,  38,  17,  38,  6,
        37,  38,  11,  28,  143, 1,   36,  13,  34,  143, 0,   26,  18,  0,
        225, 0,   30,  5,   28,  143, 1,   39,  9,   37,  143, 0,   35,  14,
        130, 4,   40,  27,  16,  25,  225, 1,   29,  7,   31,  143, 130, 251,
        74,  76,  8,   58,  43,  129, 129, 129, 0,   73,  84,  121, 64,  9,
        32,  33,  1,   3,   2,   38,  33,  1,   31,  184, 1,   138, 179, 5,
        32,  3,   34,  130, 7,   41,  178, 0,   1,   0,   1,   16,  60,  43,
        0,   43,  130, 39,  51,  1,   93,  1,   62,  2,   51,  50,  22,  22,
        21,  20,  6,   6,   35,  32,  0,   17,  52,  18,  36,  130, 14,  38,
        23,  21,  38,  35,  34,  2,   1,   76,  6,   6,   8,   75,  18,  51,
        50,  54,  1,   96,  57,  104, 125, 69,  126, 207, 108, 126, 210, 155,
        254, 248, 255, 0,   124, 1,   0,   223, 69,  128, 135, 209, 135, 198,
        178, 2,   86,  163, 121, 100, 206, 143, 169, 122, 156, 2,   255, 74,
        71,  38,  120, 209, 129, 142, 248, 139, 1,   173, 1,   59,  208, 1,
        126, 221, 18,  38,  172, 80,  254, 178, 253, 185, 176, 179, 146, 140,
        143, 254, 244, 179, 130, 124, 8,   45,  222, 0,   0,   4,   148, 5,
        200, 0,   12,  0,   65,  183, 8,   6,   5,   4,   7,   12,  4,   5,
        184, 2,   57,  64,  13,  7,   6,   4,   0,   12,  4,   204, 7,   74,
        14,  12,  206, 0,   184, 1,   4,   181, 5,   73,  13,  243, 70,  66,
        6,   44,  244, 237, 78,  16,  246, 77,  237, 0,   63,  63,  60,  253,
        60,  67,  241, 9,   8,   98,  33,  54,  18,  55,  1,   33,  53,  33,
        21,  10,  2,   7,   1,   52,  34,  125, 113, 1,   148, 253, 6,   3,
        182, 192, 248, 172, 25,  201, 1,   5,   183, 2,   138, 185, 185, 254,
        230, 254, 62,  254, 115, 166, 0,   0,   3,   0,   177, 255, 219, 4,
        143, 5,   238, 0,   23,  0,   35,  0,   48,  0,   204, 64,  33,  176,
        25,  189, 37,  2,   59,  25,  53,  37,  123, 25,  117, 37,  4,   64,
        8,   9,   3,   24,  30,  15,  21,  36,  42,  24,  30,  36,  12,  0,
        0,   42,  30,  0,   187, 1,   86,  130, 55,  60,  12,  1,   86,  64,
        12,  9,   30,  145, 6,   5,   42,  145, 18,  13,  27,  192, 9,   184,
        1,   3,   183, 45,  192, 15,  74,  50,  33,  192, 3,   130, 11,  40,
        180, 39,  192, 21,  73,  49,  184, 1,   41,  70,  240, 8,   34,  125,
        244, 24,  133, 176, 131, 8,   51,  0,   63,  237, 63,  237, 1,   16,
        228, 16,  228, 0,   25,  17,  18,  57,  47,  60,  57,  18,  57,  130,
        7,   132, 3,   76,  54,  5,   8,   35,  48,  40,  44,  28,  32,  16,
        20,  4,   8,   43,  17,  45,  143, 1,   41,  19,  39,  143, 0,   29,
        7,   27,  143, 1,   31,  5,   33,  143, 0,   44,  16,  42,  143, 0,
        40,  20,  130, 4,   38,  28,  8,   30,  143, 1,   32,  4,   130, 4,
        71,  14,  8,   32,  129, 130, 0,   37,  1,   93,  0,   93,  1,   38,
        77,  193, 9,   39,  7,   22,  22,  21,  20,  4,   35,  34,  131, 17,
        32,  37,  69,  236, 7,   37,  21,  20,  22,  23,  6,   6,   130, 5,
        68,  218, 5,   8,   157, 39,  1,   215, 141, 76,  242, 202, 183, 227,
        116, 160, 186, 149, 254, 244, 232, 239, 251, 139, 1,   150, 98,  109,
        137, 114, 117, 129, 105, 41,  141, 63,  170, 128, 120, 172, 91,  155,
        3,   38,  98,  150, 86,  163, 215, 179, 141, 98,  191, 103, 91,  201,
        131, 172, 248, 237, 164, 117, 233, 154, 65,  156, 82,  86,  112, 115,
        77,  73,  131, 247, 109, 141, 80,  130, 155, 128, 113, 81,  118, 92,
        0,   0,   2,   0,   156, 255, 219, 4,   138, 5,   237, 0,   27,  0,
        40,  0,   227, 64,  38,  28,  0,   28,  1,   44,  0,   44,  1,   4,
        36,  64,  31,  11,  79,  8,   7,   38,  11,  145, 31,  31,  0,   38,
        145, 19,  5,   3,   145, 26,  1,   204, 64,  0,   170, 13,  26,  13,
        7,   184, 1,   118, 181, 34,  206, 22,  74,  42,  1,   184, 1,   4,
        180, 28,  192, 15,  73,  41,  184, 1,   129, 65,  71,  8,   38,  228,
        78,  16,  246, 77,  253, 229, 70,  9,   18,  32,  237, 67,  48,  10,
        8,   35,  58,  35,  40,  29,  30,  12,  25,  4,   6,   13,  37,  24,
        38,  5,   37,  36,  38,  17,  38,  30,  12,  28,  143, 0,   4,   25,
        7,   225, 1,   37,  20,  34,  143, 1,   39,  18,  130, 14,  48,  29,
        14,  31,  143, 0,   6,   23,  3,   225, 0,   35,  21,  38,  143, 1,
        40,  16,  130, 4,   72,  87,  12,  33,  43,  129, 67,  57,  10,  46,
        8,   10,  9,   38,  33,  8,   31,  184, 1,   138, 179, 4,   32,  10,
        34,  130, 7,   35,  178, 1,   8,   7,   67,  57,  10,  70,  38,  5,
        35,  18,  53,  14,  2,   77,  182, 5,   44,  54,  54,  51,  32,  18,
        17,  20,  2,   4,   35,  34,  3,   20,  70,  65,  6,   8,   71,  38,
        35,  34,  6,   233, 188, 132, 122, 172, 97,  66,  96,  115, 64,  123,
        209, 115, 127, 211, 149, 1,   9,   254, 151, 254, 242, 207, 116, 65,
        153, 133, 109, 193, 85,  133, 92,  131, 147, 9,   167, 65,  129, 1,
        35,  172, 81,  68,  36,  131, 212, 130, 137, 253, 136, 254, 87,  254,
        214, 227, 254, 111, 203, 4,   38,  166, 199, 158, 132, 114, 215, 91,
        186, 84,  167, 8,   56,  4,   62,  0,   3,   0,   7,   0,   54,  64,
        28,  2,   106, 0,   7,   106, 5,   6,   0,   10,  9,   71,  71,  74,
        2,   2,   130, 11,  36,  5,   0,   73,  8,   9,   84,  160, 9,   34,
        77,  16,  253, 77,  33,  6,   38,  0,   63,  63,  77,  237, 16,  237,
        84,  146, 6,   36,  53,  51,  21,  225, 197, 131, 0,   43,  3,   120,
        198, 198, 0,   2,   0,   225, 254, 191, 1,   166, 130, 87,  56,  12,
        0,   16,  0,   98,  177, 5,   11,  186, 255, 250, 0,   12,  255, 243,
        64,  42,  11,  12,  6,   2,   16,  106, 14,  6,   75,  24,  5,   54,
        8,   106, 64,  6,   10,  18,  193, 0,   106, 134, 5,   107, 9,   67,
        9,   15,  106, 14,  14,  6,   73,  17,  18,  141, 111, 47,  118, 16,
        237, 24,  26,  237, 228, 0,   63,  26,  237, 26,  252, 26,  237, 63,
        130, 115, 51,  1,   23,  56,  0,   56,  56,  1,   67,  88,  185, 0,
        11,  255, 248, 56,  89,  19,  53,  54,  54,  75,  48,  7,   33,  6,
        3,   131, 140, 53,  38,  33,  71,  197, 44,  96,  57,  197, 254, 191,
        74,  20,  89,  115, 23,  197, 166, 128, 117, 93,  4,   171, 130, 155,
        76,  7,   9,   49,  5,   0,   91,  64,  29,  0,   1,   1,   85,  4,
        5,   20,  4,   1,   2,   4,   5,   2,   131, 11,  32,  3,   130, 11,
        52,  0,   4,   3,   5,   0,   3,   2,   0,   187, 1,   15,  0,   1,
        0,   4,   1,   15,  181, 2,   7,   5,   130, 15,  60,  184, 2,   15,
        180, 4,   209, 1,   221, 6,   16,  246, 237, 244, 60,  57,  57,  0,
        25,  63,  24,  253, 60,  237, 18,  57,  18,  57,  135, 8,   70,  249,
        6,   8,   66,  8,   46,  24,  43,  4,   125, 16,  196, 37,  1,   1,
        21,  1,   1,   4,   167, 251, 194, 4,   62,  253, 13,  2,   243, 49,
        2,   31,  2,   31,  173, 254, 142, 254, 142, 0,   0,   2,   0,   105,
        1,   89,  4,   167, 3,   71,  0,   3,   0,   7,   0,   48,  64,  11,
        5,   85,  64,  4,   139, 128, 1,   85,  0,   7,   2,   184, 1,   175,
        130, 15,  33,  5,   63,  130, 120, 85,  158, 8,   39,  93,  60,  77,
        16,  253, 60,  0,   47,  131, 255, 75,  212, 6,   38,  1,   53,  33,
        21,  105, 4,   62,  131, 93,  39,  1,   89,  148, 148, 1,   90,  148,
        148, 76,  227, 11,  42,  5,   0,   97,  64,  29,  1,   2,   1,   0,
        2,   85,  71,  242, 5,   47,  1,   0,   1,   2,   0,   85,  5,   4,
        20,  5,   5,   4,   3,   2,   5,   0,   130, 103, 42,  15,  178, 1,
        234, 4,   184, 1,   15,  183, 0,   7,   130, 238, 130, 215, 131, 222,
        130, 156, 131, 222, 34,  60,  16,  244, 130, 214, 133, 224, 34,  25,
        230, 24,  133, 226, 34,  46,  43,  8,   130, 216, 33,  135, 4,   130,
        225, 131, 8,   37,  19,  1,   1,   53,  1,   1,   132, 132, 46,  2,
        243, 253, 13,  4,   111, 253, 225, 253, 225, 173, 1,   114, 1,   114,
        130, 68,  39,  0,   37,  0,   0,   3,   59,  5,   238, 130, 225, 45,
        32,  0,   117, 64,  47,  22,  8,   29,  26,  10,  7,   4,   15,  32,
        130, 107, 8,   52,  10,  29,  31,  26,  23,  12,  17,  131, 18,  108,
        15,  88,  20,  3,   32,  148, 2,   106, 0,   10,  1,   5,   173, 3,
        31,  239, 12,  173, 23,  74,  34,  17,  73,  33,  34,  184, 1,   253,
        179, 33,  187, 217, 24,  43,  43,  78,  228, 16,  246, 77,  253, 246,
        60,  130, 255, 45,  63,  253, 230, 63,  237, 252, 237, 1,   17,  18,
        57,  18,  57,  57,  73,  161, 5,   32,  23,  70,  222, 5,   45,  14,
        21,  22,  13,  14,  14,  21,  12,  99,  1,   13,  22,  15,  99,  74,
        151, 7,   38,  51,  21,  3,   53,  52,  54,  55,  74,  156, 7,   74,
        154, 8,   8,   174, 6,   7,   7,   6,   6,   21,  21,  250, 197, 197,
        57,  90,  63,  99,  58,  136, 121, 151, 172, 185, 162, 201, 242, 30,
        53,  84,  62,  92,  59,  197, 197, 1,   139, 54,  111, 145, 103, 73,
        113, 121, 61,  86,  107, 74,  167, 56,  183, 156, 53,  95,  80,  80,
        58,  88,  114, 106, 110, 0,   2,   0,   93,  255, 219, 6,   148, 5,
        237, 0,   64,  0,   76,  0,   164, 64,  14,  66,  65,  65,  85,  41,
        40,  20,  41,  41,  40,  40,  115, 35,  66,  184, 1,   119, 64,  43,
        68,  144, 64,  35,  139, 128, 53,  116, 10,  3,   46,  116, 18,  18,
        73,  107, 28,  64,  116, 0,   202, 71,  61,  116, 64,  28,  139, 128,
        2,   11,  64,  108, 0,   189, 41,  78,  71,  71,  74,  50,  116, 64,
        14,  184, 1,   238, 181, 128, 65,  202, 40,  171, 41,  186, 1,   245,
        0,   71,  2,   63,  64,  10,  64,  31,  182, 128, 57,  116, 6,   73,
        77,  78,  188, 2,   34,  0,   33,  0,   86,  1,   28,  88,  136, 5,
        39,  77,  237, 26,  252, 26,  237, 253, 228, 132, 6,   56,  78,  69,
        101, 68,  230, 77,  16,  244, 228, 0,   63,  26,  236, 26,  237, 118,
        245, 24,  237, 16,  237, 60,  16,  237, 63,  131, 29,  62,  253, 230,
        16,  228, 135, 14,  46,  43,  14,  125, 16,  196, 49,  48,  37,  6,
        35,  34,  36,  2,   53,  52,  0,   36,  51,  50,  4,   18,  21,  20,
        2,   130, 15,  38,  38,  53,  52,  55,  55,  35,  6,   133, 9,   40,
        18,  54,  51,  50,  23,  22,  51,  51,  3,   69,  201, 6,   32,  54,
        130, 43,  48,  35,  34,  4,   2,   21,  20,  22,  22,  51,  50,  54,
        55,  19,  55,  38,  35,  34,  131, 13,  130, 12,  8,   180, 4,   69,
        180, 173, 186, 254, 215, 164, 1,   6,   1,   184, 233, 186, 1,   43,
        171, 143, 235, 139, 89,  77,  20,  40,  12,  93,  204, 90,  90,  103,
        168, 224, 133, 31,  48,  50,  28,  137, 126, 6,   38,  38,  87,  166,
        88,  254, 180, 228, 203, 254, 131, 225, 137, 247, 168, 58,  134, 109,
        16,  39,  92,  68,  166, 163, 45,  38,  55,  200, 44,  81,  155, 1,
        43,  159, 229, 1,   204, 252, 164, 254, 220, 171, 156, 254, 225, 151,
        69,  46,  42,  63,  126, 179, 167, 127, 97,  156, 1,   83,  163, 3,
        3,   253, 132, 32,  30,  33,  34,  144, 200, 122, 236, 1,   42,  228,
        254, 118, 199, 133, 249, 130, 17,  55,  2,   175, 195, 33,  254, 176,
        164, 64,  78,  214, 0,   0,   2,   0,   17,  0,   0,   5,   110, 5,
        200, 0,   7,   0,   10,  0,   177, 64,  25,  5,   9,   10,  4,   8,
        6,   10,  10,  7,   6,   8,   1,   7,   5,   9,   4,   129, 10,  1,
        2,   32,  2,   10,  4,   4,   184, 2,   92,  64,  10,  78,  169, 5,
        8,   36,  1,   10,  7,   7,   184, 2,   53,  64,  16,  0,   1,   20,
        0,   0,   1,   10,  2,   1,   2,   9,   8,   89,  64,  5,   6,   184,
        1,   150, 64,  19,  7,   7,   4,   4,   3,   3,   0,   130, 62,  8,
        54,  0,   2,   4,   10,  12,  71,  71,  74,  3,   186, 1,   16,  0,
        10,  1,   16,  64,  11,  0,   32,  0,   73,  11,  12,  213, 33,  117,
        153, 24,  43,  43,  78,  228, 26,  25,  77,  16,  253, 253, 24,  78,
        69,  101, 68,  230, 18,  57,  57,  17,  57,  57,  0,   63,  60,  16,
        130, 1,   33,  77,  16,  87,  250, 6,   42,  57,  135, 5,   46,  43,
        135, 125, 196, 135, 46,  24,  131, 6,   32,  43,  130, 34,  32,  17,
        130, 41,  8,   55,  7,   16,  60,  60,  7,   60,  60,  49,  48,  51,
        1,   51,  1,   35,  3,   33,  3,   19,  33,  3,   17,  2,   75,  208,
        2,   66,  227, 161, 253, 141, 163, 224, 1,   249, 252, 5,   200, 250,
        56,  1,   154, 254, 102, 2,   54,  2,   126, 0,   0,   3,   0,   191,
        0,   0,   4,   69,  130, 231, 8,   68,  16,  0,   26,  0,   35,  0,
        173, 64,  20,  70,  8,   24,  29,  8,   3,   28,  26,  29,  24,  8,
        3,   31,  17,  137, 0,   35,  137, 1,   26,  184, 1,   76,  64,  32,
        31,  28,  47,  28,  2,   28,  28,  0,   1,   2,   0,   8,   5,   121,
        31,  155, 22,  121, 12,  74,  37,  35,  17,  90,  1,   0,   73,  36,
        37,  227, 33,  104, 185, 1,   27,  90,  208, 12,  44,  77,  253, 244,
        237, 0,   63,  63,  18,  57,  47,  93,  237, 16,  130, 1,   34,  1,
        17,  23,  87,  83,  7,   57,  67,  121, 64,  51,  3,   33,  14,  38,
        20,  37,  10,  37,  19,  15,  22,  130, 1,   24,  9,   22,  130, 3,
        29,  7,   31,  130, 130, 179, 58,  31,  130, 1,   21,  13,  18,  130,
        0,   23,  11,  25,  130, 1,   9,   30,  6,   28,  130, 0,   7,   8,
        32,  4,   34,  130, 1,   43,  76,  243, 5,   32,  1,   86,  5,   6,
        39,  129, 51,  17,  33,  50,  22,  21,  20,  76,  219, 5,   37,  6,
        6,   35,  39,  51,  50,  72,  56,  5,   33,  35,  53,  130, 9,   8,
        108, 53,  52,  38,  35,  35,  191, 1,   128, 251, 208, 140, 164, 143,
        143, 77,  106, 174, 186, 228, 43,  222, 134, 73,  235, 178, 59,  67,
        183, 170, 151, 187, 82,  5,   200, 193, 133, 108, 213, 62,  44,  113,
        159, 85,  103, 178, 89,  157, 49,  113, 71,  128, 179, 133, 157, 120,
        105, 111, 0,   1,   0,   104, 255, 219, 5,   23,  5,   237, 0,   26,
        0,   105, 64,  37,  35,  8,   12,  13,  0,   19,  16,  119, 10,  13,
        142, 12,  176, 10,  3,   22,  119, 4,   26,  142, 64,  0,   141, 80,
        4,   9,   0,   74,  28,  19,  121, 7,   73,  27,  118, 169, 80,  50,
        8,   38,  230, 0,   63,  26,  77,  252, 26,  130, 239, 33,  63,  252,
        132, 244, 33,  18,  57,  68,  64,  5,   43,  24,  17,  21,  5,   9,
        21,  5,   19,  110, 0,   17,  9,   130, 4,   43,  20,  6,   22,  110,
        0,   18,  8,   16,  110, 1,   0,   43,  76,  3,   5,   49,  37,  14,
        2,   35,  32,  0,   17,  16,  0,   33,  50,  5,   21,  38,  38,  35,
        32,  2,   130, 11,  130, 199, 8,   99,  54,  55,  5,   23,  91,  143,
        154, 90,  254, 138, 254, 165, 1,   104, 1,   116, 198, 1,   11,  224,
        169, 76,  254, 246, 239, 1,   19,  252, 70,  139, 138, 102, 76,  47,
        45,  21,  1,   186, 1,   78,  1,   85,  1,   181, 65,  197, 77,  28,
        254, 151, 254, 250, 254, 239, 254, 175, 25,  49,  55,  0,   2,   0,
        191, 0,   0,   5,   149, 5,   200, 0,   10,  0,   21,  0,   91,  64,
        25,  43,  8,   21,  137, 1,   2,   11,  137, 0,   8,   16,  121, 6,
        74,  23,  21,  11,  90,  1,   0,   73,  22,  104, 133, 191, 92,  137,
        6,   33,  77,  237, 80,  244, 9,   44,  30,  3,   19,  8,   38,  14,
        37,  4,   37,  18,  38,  13,  9,   130, 172, 33,  19,  3,   130, 4,
        34,  15,  7,   12,  130, 197, 35,  5,   20,  110, 1,   75,  189, 8,
        46,  129, 51,  17,  33,  32,  4,   18,  21,  20,  2,   4,   33,  37,
        33,  50,  84,  45,  5,   8,   48,  33,  35,  191, 1,   236, 1,   29,
        1,   38,  167, 153, 254, 200, 254, 234, 254, 227, 1,   13,  210, 209,
        118, 102, 228, 254, 209, 173, 5,   200, 165, 254, 195, 223, 211, 254,
        160, 212, 157, 133, 1,   32,  177, 156, 1,   1,   155, 0,   0,   1,
        131, 173, 33,  4,   37,  130, 173, 8,   33,  11,  0,   77,  64,  35,
        6,   5,   89,  8,   8,   7,   7,   0,   3,   4,   89,  2,   1,   2,
        10,  9,   89,  11,  0,   8,   7,   141, 3,   155, 10,  74,  13,  4,
        9,   131, 183, 41,  12,  13,  184, 1,   106, 179, 33,  104, 233, 24,
        93,  71,  10,  39,  77,  244, 228, 0,   63,  60,  253, 60,  131, 3,
        40,  18,  57,  47,  60,  16,  253, 60,  49,  48,  83,  92,  5,   57,
        33,  21,  33,  17,  33,  21,  191, 3,   59,  253, 151, 2,   5,   253,
        251, 2,   148, 5,   200, 157, 254, 37,  155, 253, 232, 157, 134, 123,
        33,  3,   250, 130, 123, 36,  9,   0,   74,  64,  32,  141, 123, 131,
        119, 37,  2,   74,  11,  176, 11,  1,   133, 120, 40,  10,  11,  188,
        1,   167, 0,   33,  0,   104, 66,  242, 11,  36,  113, 78,  16,  246,
        77,  130, 123, 150, 120, 133, 118, 33,  6,   253, 130, 98,  37,  157,
        254, 16,  155, 253, 96,  131, 115, 66,  103, 7,   43,  28,  0,   112,
        64,  39,  40,  8,   26,  23,  28,  28,  23,  66,  104, 8,   53,  0,
        176, 23,  119, 3,   9,   27,  26,  90,  13,  175, 28,  0,   74,  30,
        19,  121, 7,   73,  29,  118, 197, 66,  105, 8,   49,  246, 60,  77,
        228, 253, 60,  0,   63,  237, 236, 63,  252, 237, 16,  253, 17,  57,
        47,  77,  137, 6,   42,  28,  17,  22,  4,   9,   21,  38,  5,   37,
        22,  4,   66,  109, 9,   32,  23,  66,  109, 6,   65,  177, 6,   46,
        129, 129, 37,  6,   6,   35,  32,  36,  2,   53,  16,  0,   33,  50,
        23,  66,  111, 6,   8,   64,  20,  18,  22,  51,  50,  54,  55,  17,
        51,  5,   23,  194, 179, 96,  254, 246, 254, 207, 159, 1,   105, 1,
        113, 223, 244, 225, 169, 75,  254, 245, 238, 101, 238, 201, 51,  90,
        86,  209, 40,  56,  21,  192, 1,   106, 223, 1,   85,  1,   180, 67,
        194, 77,  27,  254, 149, 254, 254, 160, 254, 235, 182, 11,  26,  1,
        226, 65,  71,  6,   8,   43,  5,   35,  5,   200, 0,   11,  0,   75,
        64,  22,  4,   3,   89,  9,   10,  10,  8,   5,   2,   2,   11,  8,
        8,   13,  71,  71,  74,  7,   5,   8,   90,  7,   184, 1,   176, 183,
        0,   2,   11,  90,  0,   73,  12,  13,  130, 11,  34,  179, 33,  104,
        130, 214, 39,  43,  78,  244, 77,  253, 60,  16,  253, 73,  255, 9,
        38,  60,  63,  60,  18,  57,  47,  60,  130, 21,  38,  49,  48,  51,
        17,  51,  17,  33,  130, 3,   32,  35,  130, 5,   51,  191, 210, 2,
        192, 210, 210, 253, 64,  5,   200, 253, 144, 2,   112, 250, 56,  2,
        187, 253, 69,  131, 121, 32,  190, 130, 4,   32,  144, 130, 121, 41,
        3,   0,   46,  64,  17,  1,   2,   0,   8,   5,   130, 112, 43,  2,
        2,   3,   90,  1,   0,   73,  4,   5,   184, 1,   133, 136, 104, 66,
        233, 5,   87,  241, 5,   32,  63,  133, 92,  45,  190, 210, 5,   200,
        250, 56,  0,   1,   255, 72,  254, 216, 1,   197, 130, 69,  59,  14,
        0,   77,  64,  16,  1,   142, 64,  0,   218, 12,  3,   119, 13,  6,
        8,   9,   8,   2,   1,   0,   184, 1,   150, 64,  11,  7,   16,  130,
        87,  45,  10,  90,  7,   73,  15,  16,  186, 1,   133, 0,   33,  1,
        207, 177, 135, 191, 32,  78,  71,  54,  6,   58,  60,  0,   63,  60,
        63,  47,  237, 75,  81,  88,  176, 96,  26,  89,  252, 26,  237, 49,
        48,  3,   53,  22,  51,  50,  54,  54,  53,  130, 196, 58,  20,  6,
        35,  34,  184, 120, 94,  64,  92,  57,  210, 239, 218, 70,  254, 237,
        186, 39,  47,  109, 152, 5,   20,  250, 243, 255, 228, 65,  65,  7,
        32,  21,  130, 129, 54,  10,  0,   145, 179, 4,   5,   6,   6,   184,
        2,   53,  64,  12,  3,   4,   20,  3,   6,   7,   3,   4,   8,   7,
        131, 16,  58,  92,  64,  11,  9,   8,   20,  9,   6,   5,   9,   8,
        6,   1,   9,   184, 2,   73,  64,  30,  3,   3,   7,   5,   4,   4,
        2,   2,   130, 248, 58,  10,  10,  8,   8,   7,   8,   8,   6,   4,
        3,   10,  5,   141, 7,   74,  12,  3,   9,   10,  184, 2,   55,  179,
        0,   73,  11,  12,  65,  110, 5,   34,  153, 24,  43,  65,  110, 5,
        39,  60,  25,  78,  16,  246, 24,  77,  228, 83,  145, 5,   34,  16,
        60,  16,  94,  87,  6,   35,  18,  57,  16,  229, 74,  115, 5,   40,
        135, 5,   125, 196, 135, 8,   46,  24,  43,  131, 8,   65,  42,  5,
        8,   33,  1,   51,  1,   1,   33,  1,   17,  191, 197, 2,   87,  211,
        253, 188, 2,   171, 254, 246, 253, 121, 5,   200, 253, 40,  2,   216,
        253, 62,  252, 250, 2,   238, 253, 18,  134, 197, 33,  4,   43,  130,
        197, 47,  5,   0,   49,  64,  19,  1,   2,   4,   3,   89,  5,   0,
        8,   4,   74,  7,   65,  143, 5,   33,  6,   7,   67,  187, 5,   133,
        137, 65,  143, 5,   34,  230, 0,   63,  131, 8,   65,  144, 6,   42,
        33,  21,  191, 210, 2,   154, 5,   200, 250, 213, 157, 68,  25,  5,
        33,  6,   38,  130, 77,  8,   55,  12,  1,   9,   64,  73,  24,  8,
        25,  11,  152, 8,   153, 11,  184, 8,   185, 11,  6,   5,   2,   21,
        2,   25,  4,   34,  2,   53,  2,   67,  2,   77,  4,   85,  2,   90,
        4,   101, 2,   149, 2,   165, 2,   181, 2,   197, 2,   213, 2,   229,
        2,   16,  25,  2,   23,  4,   84,  130, 22,  62,  100, 2,   107, 4,
        116, 2,   123, 4,   151, 2,   152, 4,   183, 2,   245, 2,   250, 4,
        13,  184, 255, 116, 183, 3,   10,  9,   32,  2,   11,  10,  10,  71,
        201, 10,  38,  4,   8,   9,   9,   184, 2,   54,  82,  205, 7,   48,
        10,  9,   3,   184, 1,   51,  179, 0,   8,   175, 11,  184, 2,   65,
        64,  27,  2,   65,  103, 6,   36,  6,   7,   7,   12,  12,  130, 211,
        8,   43,  9,   8,   10,  2,   3,   11,  14,  71,  71,  74,  6,   184,
        2,   55,  180, 64,  63,  8,   1,   8,   184, 1,   155, 179, 112, 3,
        1,   3,   187, 1,   14,  0,   160, 0,   11,  2,   55,  64,  9,   0,
        73,  13,  14,  213, 66,  234, 9,   46,  26,  25,  253, 93,  253, 93,
        26,  24,  253, 78,  69,  101, 68,  230, 17,  71,  232, 12,  71,  238,
        7,   38,  253, 228, 124, 16,  237, 24,  57,  71,  238, 15,  52,  49,
        48,  1,   93,  113, 0,   93,  51,  17,  33,  1,   1,   33,  17,  35,
        17,  1,   35,  1,   17,  191, 130, 4,   58,  154, 1,   166, 1,   4,
        196, 254, 104, 203, 254, 116, 5,   200, 251, 135, 4,   121, 250, 56,
        4,   180, 251, 175, 4,   84,  251, 73,  65,  145, 6,   8,   51,  5,
        42,  5,   200, 0,   9,   0,   121, 64,  18,  5,   120, 2,   119, 7,
        150, 2,   153, 7,   182, 2,   185, 7,   6,   8,   2,   3,   3,   184,
        2,   92,  64,  20,  7,   8,   20,  7,   7,   8,   3,   8,   9,   4,
        2,   2,   9,   7,   8,   2,   7,   9,   4,   130, 24,  40,  55,  182,
        6,   6,   5,   74,  11,  8,   9,   131, 223, 39,  1,   0,   73,  10,
        11,  184, 2,   17,  67,  77,  14,  37,  246, 60,  77,  16,  253, 60,
        72,  172, 5,   33,  63,  60,  130, 206, 37,  135, 46,  43,  135, 125,
        196, 131, 171, 49,  67,  88,  180, 71,  2,   72,  7,   2,   0,   93,
        89,  51,  17,  51,  1,   17,  51,  17,  131, 176, 38,  206, 2,   233,
        180, 205, 253, 22,  130, 169, 34,  137, 4,   119, 130, 169, 8,   41,
        119, 251, 137, 0,   0,   2,   0,   104, 255, 219, 5,   208, 5,   237,
        0,   15,  0,   27,  0,   111, 64,  23,  67,  8,   22,  119, 8,   3,
        16,  119, 0,   9,   19,  121, 12,  74,  29,  25,  121, 4,   73,  28,
        71,  111, 10,  32,  246, 70,  173, 11,  87,  162, 13,  45,  110, 1,
        27,  1,   25,  110, 0,   21,  9,   19,  110, 1,   23,  7,   130, 9,
        38,  18,  13,  16,  110, 0,   26,  3,   130, 4,   38,  20,  11,  22,
        110, 1,   24,  5,   130, 4,   79,  74,  12,  39,  129, 5,   34,  36,
        2,   53,  52,  18,  74,  200, 7,   32,  4,   87,  162, 11,  8,   58,
        3,   28,  210, 254, 201, 171, 163, 1,   62,  211, 211, 1,   62,  163,
        161, 254, 197, 216, 224, 245, 247, 222, 222, 247, 245, 37,  188, 1,
        105, 228, 220, 1,   105, 196, 196, 254, 151, 220, 219, 254, 153, 199,
        157, 1,   78,  1,   32,  1,   31,  1,   75,  254, 181, 254, 225, 254,
        224, 254, 178, 130, 213, 8,   44,  191, 0,   0,   4,   72,  5,   200,
        0,   12,  0,   22,  0,   99,  64,  32,  40,  8,   22,  137, 1,   11,
        137, 14,  14,  0,   1,   2,   0,   8,   17,  121, 6,   74,  24,  22,
        12,  90,  1,   0,   73,  23,  24,  227, 33,  104, 67,  20,  11,  132,
        224, 37,  63,  18,  57,  47,  237, 16,  84,  75,  5,   47,  28,  3,
        20,  8,   38,  4,   37,  19,  38,  15,  9,   17,  130, 1,   20,  3,
        130, 4,   40,  16,  7,   14,  130, 0,   18,  5,   21,  130, 71,  144,
        8,   73,  27,  5,   40,  22,  21,  20,  6,   4,   35,  35,  17,  17,
        73,  14,  5,   8,   38,  38,  35,  35,  191, 1,   147, 215, 187, 100,
        133, 254, 218, 177, 93,  85,  209, 182, 58,  128, 161, 129, 5,   200,
        84,  174, 130, 152, 228, 124, 253, 180, 2,   235, 190, 140, 76,  105,
        65,  0,   130, 173, 8,   51,  104, 254, 216, 6,   102, 5,   237, 0,
        21,  0,   33,  0,   148, 64,  40,  71,  8,   0,   21,  19,  22,  3,
        2,   19,  25,  31,  28,  119, 12,  3,   21,  141, 22,  119, 4,   2,
        9,   4,   9,   0,   21,  222, 25,  121, 16,  74,  35,  31,  121, 8,
        73,  34,  65,  148, 13,  47,  244, 50,  0,   63,  63,  16,  237, 228,
        63,  237, 1,   17,  18,  57,  57,  0,   130, 4,   33,  17,  51,  84,
        40,  5,   8,   41,  23,  33,  5,   18,  6,   37,  14,  37,  10,  38,
        33,  5,   31,  110, 0,   27,  13,  25,  110, 1,   29,  11,  31,  110,
        0,   23,  18,  25,  110, 3,   32,  7,   22,  110, 0,   26,  15,  28,
        110, 1,   30,  9,   130, 4,   33,  24,  17,  130, 14,  35,  18,  19,
        16,  60,  65,  168, 11,  37,  129, 129, 1,   36,  39,  6,   76,  117,
        5,   65,  172, 8,   35,  7,   22,  5,   37,  89,  81,  10,  8,   60,
        5,   225, 254, 163, 236, 94,  51,  186, 254, 190, 163, 162, 1,   63,
        211, 212, 1,   63,  161, 175, 204, 255, 1,   18,  252, 182, 222, 247,
        247, 222, 222, 247, 245, 254, 216, 113, 156, 10,  195, 1,   106, 220,
        220, 1,   105, 196, 197, 254, 151, 222, 228, 254, 139, 105, 112, 48,
        249, 1,   75,  1,   33,  130, 1,   65,  188, 9,   65,  189, 6,   8,
        85,  250, 5,   200, 0,   14,  0,   24,  0,   175, 182, 40,  8,   17,
        12,  10,  9,   9,   184, 2,   92,  64,  11,  12,  11,  20,  12,  12,
        11,  9,   12,  13,  137, 16,  184, 1,   251, 64,  19,  10,  24,  137,
        1,   2,   14,  11,  11,  10,  8,   9,   12,  5,   14,  26,  71,  71,
        74,  11,  186, 1,   87,  0,   10,  1,   81,  178, 19,  121, 5,   184,
        2,   5,   64,  9,   0,   15,  13,  14,  90,  0,   73,  25,  26,  184,
        2,   14,  179, 65,  236, 7,   43,  77,  253, 60,  60,  16,  253, 237,
        25,  244, 24,  237, 25,  68,  16,  8,   56,  0,   24,  63,  60,  16,
        60,  63,  77,  237, 16,  253, 253, 57,  57,  135, 14,  46,  43,  5,
        125, 16,  196, 1,   18,  57,  83,  128, 5,   45,  27,  3,   22,  7,
        38,  21,  38,  17,  8,   19,  130, 1,   22,  3,   130, 4,   45,  18,
        6,   16,  130, 0,   8,   20,  4,   23,  130, 1,   0,   43,  60,  95,
        166, 5,   75,  37,  8,   33,  6,   7,   130, 213, 66,  11,  13,  8,
        96,  192, 249, 190, 67,  124, 107, 1,   238, 254, 254, 254, 91,  196,
        68,  233, 163, 61,  129, 142, 132, 5,   200, 223, 144, 84,  164, 128,
        44,  253, 75,  2,   114, 253, 142, 3,   15,  194, 116, 69,  103, 58,
        0,   0,   1,   0,   100, 255, 219, 3,   233, 5,   237, 0,   43,  0,
        178, 64,  28,  6,   32,  22,  34,  22,  36,  38,  36,  54,  12,  73,
        14,  102, 36,  118, 13,  117, 32,  134, 10,  134, 13,  150, 10,  12,
        43,  8,   23,  184, 255, 251, 64,  39,  1,   5,   11,  41,  26,  33,
        130, 138, 8,   46,  142, 22,  176, 26,  119, 20,  3,   1,   142, 0,
        176, 4,   119, 41,  9,   13,  32,  10,  35,  4,   16,  22,  23,  141,
        7,   121, 38,  74,  45,  29,  184, 1,   43,  64,  13,  16,  1,   16,
        175, 0,   73,  44,  45,  227, 33,  118, 169, 70,  152, 5,   34,  228,
        60,  16,  84,  137, 8,   40,  60,  18,  23,  57,  0,   63,  237, 252,
        237, 131, 3,   34,  17,  18,  57,  130, 2,   8,   36,  56,  56,  49,
        48,  67,  121, 64,  30,  39,  40,  27,  28,  17,  19,  5,   6,   18,
        38,  27,  19,  29,  130, 0,   5,   40,  7,   130, 1,   28,  17,  26,
        130, 1,   6,   39,  4,   130, 75,  87,  5,   32,  43,  84,  107, 5,
        34,  55,  53,  22,  82,  220, 5,   34,  39,  38,  39,  130, 2,   37,
        38,  53,  52,  54,  54,  51,  72,  247, 5,   8,   134, 34,  6,   21,
        20,  22,  22,  23,  23,  22,  23,  22,  21,  20,  4,   33,  34,  38,
        100, 178, 164, 70,  127, 147, 38,  39,  155, 102, 204, 70,  70,  111,
        187, 142, 171, 190, 165, 123, 61,  128, 123, 58,  98,  86,  106, 206,
        69,  69,  254, 249, 255, 0,   79,  148, 28,  208, 82,  34,  139, 97,
        73,  62,  61,  88,  59,  116, 96,  96,  117, 105, 184, 104, 57,  192,
        65,  27,  134, 70,  48,  99,  71,  49,  60,  117, 97,  98,  127, 173,
        254, 21,  0,   1,   0,   18,  0,   0,   4,   254, 5,   200, 0,   7,
        0,   58,  64,  31,  5,   2,   89,  4,   3,   2,   0,   8,   0,   9,
        16,  9,   2,   9,   71,  71,  74,  4,   251, 6,   7,   90,  1,   0,
        251, 2,   73,  8,   117, 153, 73,  215, 5,   35,  244, 60,  253, 60,
        96,  138, 5,   35,  113, 0,   63,  63,  72,  250, 5,   53,  33,  17,
        33,  53,  33,  21,  33,  17,  2,   31,  253, 243, 4,   236, 253, 243,
        5,   43,  157, 157, 250, 213, 130, 93,  36,  179, 255, 219, 4,   217,
        130, 93,  52,  20,  0,   112, 64,  20,  40,  8,   4,   5,   11,  10,
        10,  1,   1,   0,   2,   5,   119, 16,  9,   22,  130, 95,  49,  9,
        186, 2,   55,  0,   12,  1,   173, 181, 2,   90,  20,  73,  21,  22,
        184, 1,   173, 69,  178, 8,   44,  77,  237, 253, 237, 78,  69,  101,
        68,  230, 0,   63,  77,  237, 70,  109, 5,   8,   32,  16,  60,  49,
        48,  67,  121, 64,  28,  3,   19,  14,  38,  7,   37,  18,  37,  6,
        15,  9,   130, 1,   4,   17,  2,   130, 0,   8,   13,  5,   130, 0,
        3,   19,  130, 4,   74,  61,  6,   37,  43,  129, 19,  51,  17,  20,
        72,  206, 10,  8,   38,  6,   35,  34,  38,  38,  53,  179, 210, 182,
        163, 103, 142, 78,  184, 119, 217, 179, 191, 236, 120, 5,   200, 252,
        90,  244, 182, 71,  160, 188, 3,   173, 252, 86,  241, 213, 125, 130,
        226, 229, 130, 179, 36,  36,  0,   0,   5,   50,  130, 179, 45,  6,
        0,   142, 64,  9,   149, 3,   182, 3,   2,   2,   1,   0,   0,   71,
        123, 11,  72,  240, 5,   45,  182, 3,   4,   20,  3,   3,   4,   3,
        184, 1,   87,  64,  24,  6,   71,  112, 7,   42,  0,   8,   5,   3,
        0,   2,   1,   4,   6,   3,   8,   130, 223, 8,   33,  5,   186, 1,
        164, 0,   3,   1,   164, 179, 1,   73,  7,   8,   188, 1,   179, 0,
        33,  0,   117, 1,   29,  0,   24,  43,  43,  25,  78,  244, 24,  77,
        253, 253, 25,  79,  66,  10,  35,  56,  0,   24,  63,  72,  220, 6,
        34,  77,  16,  253, 79,  63,  13,  39,  49,  48,  0,   93,  33,  1,
        51,  1,   130, 2,   60,  2,   83,  253, 209, 210, 1,   202, 1,   186,
        184, 253, 231, 5,   200, 251, 68,  4,   188, 250, 56,  0,   0,   1,
        0,   2,   0,   0,   6,   213, 130, 183, 50,  12,  0,   253, 64,  18,
        176, 3,   176, 6,   191, 11,  3,   144, 3,   144, 6,   159, 11,  3,
        142, 192, 32,  5,   72,  75,  7,   48,  6,   5,   20,  6,   6,   5,
        7,   8,   9,   9,   184, 2,   53,  64,  10,  6,   7,   130, 14,  41,
        7,   4,   11,  12,  12,  184, 2,   54,  64,  11,  133, 223, 41,  8,
        7,   7,   5,   11,  184, 1,   51,  64,  10,  134, 226, 34,  3,   218,
        6,   131, 14,  41,  26,  0,   64,  9,   10,  10,  12,  12,  0,   8,
        131, 238, 40,  12,  3,   5,   10,  11,  7,   9,   6,   14,  130, 244,
        44,  8,   65,  9,   1,   14,  0,   32,  0,   6,   1,   11,  0,   11,
        130, 3,   45,  128, 0,   3,   1,   14,  180, 32,  1,   73,  13,  14,
        188, 2,   37,  65,  2,   9,   43,  78,  244, 26,  25,  77,  253, 26,
        24,  253, 253, 26,  25,  80,  75,  12,  72,  104, 12,  36,  26,  77,
        16,  253, 228, 65,  255, 5,   36,  237, 16,  60,  16,  60,  65,  23,
        13,  80,  94,  6,   134, 6,   35,  49,  48,  0,   93,  65,  38,  7,
        8,   100, 1,   51,  1,   35,  1,   1,   1,   127, 254, 131, 202, 1,
        47,  1,   23,  202, 1,   8,   1,   70,  171, 254, 97,  209, 254, 251,
        254, 239, 5,   200, 251, 111, 4,   145, 251, 127, 4,   129, 250, 56,
        4,   117, 251, 139, 0,   1,   0,   16,  0,   0,   4,   204, 5,   200,
        0,   11,  0,   218, 64,  52,  7,   7,   8,   10,  11,  10,  9,   11,
        6,   4,   4,   3,   1,   0,   1,   2,   0,   5,   7,   7,   6,   4,
        3,   4,   5,   3,   8,   10,  10,  11,  1,   2,   1,   0,   2,   9,
        7,   1,   10,  3,   2,   130, 4,   8,   75,  4,   4,   9,   11,  3,
        2,   9,   9,   184, 2,   92,  64,  10,  8,   3,   20,  8,   8,   3,
        5,   6,   11,  11,  184, 2,   53,  64,  42,  0,   5,   20,  0,   0,
        5,   6,   5,   5,   3,   3,   2,   2,   11,  9,   9,   8,   8,   0,
        8,   11,  9,   6,   5,   3,   2,   6,   0,   96,  13,  112, 13,  144,
        13,  3,   13,  71,  71,  74,  6,   175, 8,   184, 1,   18,  181, 2,
        218, 76,  109, 5,   36,  18,  179, 33,  117, 233, 66,  34,  7,   34,
        228, 253, 228, 66,  35,  5,   35,  93,  18,  23,  57,  69,  109, 5,
        75,  1,   7,   36,  135, 5,   77,  46,  43,  81,  98,  9,   104, 18,
        5,   130, 37,  38,  135, 8,   16,  60,  8,   196, 7,   131, 5,   32,
        60,  130, 11,  34,  196, 8,   196, 133, 5,   35,  49,  48,  51,  1,
        66,  66,  5,   8,   55,  1,   35,  1,   1,   16,  1,   235, 254, 45,
        248, 1,   100, 1,   118, 198, 254, 39,  1,   229, 248, 254, 136, 254,
        123, 2,   231, 2,   225, 253, 210, 2,   46,  253, 53,  253, 3,   2,
        75,  253, 181, 0,   1,   0,   17,  0,   0,   4,   226, 5,   200, 0,
        8,   0,   139, 178, 3,   82,  76,  5,   41,  11,  1,   2,   20,  1,
        4,   5,   1,   2,   5,   131, 15,  47,  53,  64,  51,  7,   6,   20,
        7,   4,   3,   7,   6,   7,   4,   1,   3,   0,   134, 237, 47,  0,
        8,   1,   1,   2,   4,   0,   5,   5,   8,   0,   10,  16,  10,  2,
        10,  131, 230, 34,  251, 7,   8,   68,  76,  5,   36,  9,   10,  184,
        1,   109, 130, 229, 72,  69,  5,   32,  77,  68,  82,  10,  36,  18,
        57,  47,  17,  57,  130, 4,   33,  0,   63,  65,  242, 5,   132, 206,
        130, 230, 34,  5,   125, 16,  75,  227, 5,   131, 8,   35,  49,  48,
        33,  17,  133, 200, 56,  17,  1,   255, 254, 18,  240, 1,   128, 1,
        158, 195, 253, 239, 2,   106, 3,   94,  253, 100, 2,   156, 252, 166,
        253, 146, 130, 185, 32,  95,  130, 185, 32,  118, 130, 185, 47,  9,
        0,   99,  182, 8,   11,  6,   2,   6,   7,   7,   184, 2,   92,  64,
        38,  131, 189, 46,  1,   2,   1,   7,   6,   2,   3,   89,  5,   4,
        2,   8,   7,   89,  9,   130, 172, 35,  0,   2,   7,   4,   130, 186,
        40,  9,   74,  11,  4,   155, 0,   73,  10,  118, 82,  43,  5,   44,
        78,  16,  244, 77,  228, 78,  16,  246, 60,  18,  57,  17,  57,  130,
        151, 75,  254, 6,   34,  60,  253, 60,  130, 15,  66,  137, 6,   8,
        34,  1,   17,  18,  57,  49,  48,  51,  53,  1,   33,  53,  33,  21,
        1,   33,  21,  95,  3,   27,  253, 22,  3,   230, 252, 229, 3,   27,
        168, 4,   131, 157, 157, 251, 125, 168, 130, 141, 32,  197, 101, 159,
        6,   8,   46,  7,   0,   72,  64,  24,  3,   4,   85,  2,   1,   16,
        6,   5,   85,  7,   0,   18,  15,  9,   1,   9,   71,  71,  74,  7,
        2,   193, 5,   4,   184, 2,   61,  183, 0,   0,   1,   1,   1,   73,
        8,   120, 185, 1,   28,  0,   24,  43,  130, 128, 40,  93,  60,  77,
        253, 60,  244, 60,  78,  69,  100, 250, 5,   131, 13,  32,  63,  130,
        128, 54,  49,  48,  19,  17,  33,  21,  35,  17,  51,  21,  197, 1,
        139, 222, 222, 254, 216, 7,   83,  148, 249, 213, 148, 130, 62,  32,
        0,   98,  167, 9,   33,  64,  64,  98,  167, 16,  37,  15,  2,   1,
        0,   0,   3,   98,  165, 8,   35,  97,  185, 1,   184, 68,  143, 5,
        34,  228, 16,  246, 98,  163, 17,  42,  1,   1,   51,  1,   3,   69,
        252, 199, 162, 3,   57,  98,  164, 6,   8,   35,  0,   1,   0,   74,
        254, 216, 1,   213, 6,   43,  0,   7,   0,   78,  64,  25,  5,   6,
        85,  0,   7,   16,  4,   3,   85,  1,   2,   18,  0,   9,   48,  9,
        144, 9,   3,   9,   130, 207, 33,  1,   0,   130, 204, 49,  64,  17,
        4,   5,   193, 2,   15,  7,   63,  7,   159, 7,   3,   7,   73,  8,
        109, 158, 135, 209, 32,  244, 130, 193, 145, 209, 53,  1,   17,  33,
        53,  51,  17,  35,  53,  1,   213, 254, 117, 222, 222, 6,   43,  248,
        173, 148, 6,   43,  148, 130, 113, 44,  105, 1,   139, 4,   167, 5,
        200, 0,   5,   0,   100, 64,  30,  130, 15,  40,  5,   1,   85,  2,
        3,   20,  2,   2,   3,   130, 17,  53,  1,   5,   85,  4,   3,   20,
        4,   4,   3,   1,   0,   5,   4,   4,   2,   184, 2,   15,  64,  21,
        3,   2,   130, 12,  34,  3,   2,   7,   130, 136, 50,  4,   150, 3,
        150, 2,   73,  6,   86,  92,  24,  43,  25,  78,  16,  244, 24,  77,
        244, 244, 67,  80,  5,   67,  79,  5,   37,  77,  228, 23,  57,  135,
        5,   87,  214, 6,   32,  5,   87,  214, 6,   34,  1,   1,   35,  130,
        2,   8,   39,  2,   136, 254, 135, 166, 2,   31,  2,   31,  166, 4,
        125, 253, 14,  4,   61,  251, 195, 0,   1,   0,   86,  255, 108, 3,
        169, 0,   0,   0,   3,   0,   34,  64,  18,  3,   0,   85,  2,   1,
        10,  65,  75,  6,   32,  151, 104, 235, 6,   33,  228, 16,  78,  37,
        6,   42,  49,  48,  23,  53,  33,  21,  86,  3,   83,  148, 148, 130,
        195, 39,  1,   102, 5,   3,   3,   34,  6,   43,  130, 57,  45,  54,
        64,  22,  0,   2,   3,   1,   1,   2,   181, 128, 3,   0,   5,   130,
        163, 44,  3,   226, 1,   73,  4,   5,   226, 33,  186, 1,   53,  1,
        114, 84,  162, 5,   32,  77,  77,  116, 5,   36,  0,   47,  60,  26,
        77,  93,  153, 5,   8,   100, 57,  1,   1,   51,  19,  2,   117, 254,
        241, 222, 222, 5,   3,   1,   40,  254, 216, 0,   2,   0,   103, 255,
        231, 4,   57,  4,   86,  0,   36,  0,   47,  0,   196, 64,  22,  77,
        8,   0,   25,  37,  17,  125, 64,  18,  201, 20,  12,  9,   87,  39,
        39,  3,   15,  87,  20,  7,   37,  184, 1,   6,   64,  26,  0,   191,
        45,  126, 3,   29,  87,  34,  34,  3,   11,  11,  37,  91,  24,  25,
        191, 31,  114, 32,  74,  49,  17,  18,  203, 42,  184, 2,   59,  64,
        9,   6,   73,  48,  49,  195, 33,  103, 216, 72,  220, 5,   58,  237,
        244, 60,  78,  16,  246, 77,  228, 244, 60,  253, 60,  0,   63,  60,
        16,  237, 16,  237, 244, 237, 63,  237, 18,  57,  47,  237, 96,  66,
        5,   36,  89,  16,  252, 26,  237, 66,  248, 5,   8,   58,  67,  121,
        64,  54,  35,  44,  21,  28,  4,   14,  13,  38,  44,  4,   42,  101,
        0,   14,  21,  11,  101, 1,   22,  23,  28,  35,  26,  101, 0,   27,
        26,  36,  0,   40,  8,   42,  95,  0,   43,  5,   45,  101, 0,   12,
        22,  15,  101, 1,   27,  36,  29,  101, 0,   41,  7,   39,  95,  1,
        0,   75,  48,  5,   36,  16,  60,  16,  60,  43,  130, 2,   38,  43,
        43,  129, 129, 129, 37,  6,   87,  144, 5,   40,  36,  33,  51,  53,
        52,  38,  38,  35,  34,  88,  176, 5,   41,  22,  21,  17,  20,  22,
        22,  51,  50,  55,  23,  131, 30,  36,  39,  17,  39,  34,  6,   93,
        114, 5,   8,   146, 2,   245, 88,  171, 82,  115, 198, 1,   20,  1,
        47,  46,  42,  89,  66,  153, 181, 199, 176, 96,  161, 96,  26,  47,
        31,  13,  25,  14,  67,  81,  54,  100, 51,  63,  155, 210, 103, 69,
        62,  128, 138, 81,  82,  152, 146, 160, 209, 131, 78,  71,  40,  96,
        163, 81,  61,  131, 145, 254, 20,  82,  57,  30,  4,   109, 32,  70,
        200, 1,   25,  2,   120, 103, 71,  98,  55,  0,   0,   2,   0,   197,
        255, 231, 4,   154, 6,   43,  0,   17,  0,   30,  0,   192, 64,  54,
        32,  64,  21,  13,  31,  64,  6,   27,  34,  8,   30,  18,  16,  3,
        4,   27,  21,  16,  24,  3,   1,   0,   27,  126, 6,   7,   0,   10,
        21,  87,  12,  11,  24,  105, 9,   74,  32,  16,  32,  1,   3,   17,
        122, 18,  147, 0,   73,  31,  32,  195, 33,  113, 198, 65,  66,  5,
        35,  253, 228, 60,  93,  95,  90,  6,   36,  237, 63,  63,  237, 63,
        95,  91,  6,   89,  171, 6,   43,  24,  22,  26,  7,   11,  22,  11,
        24,  95,  1,   26,  7,   130, 4,   40,  23,  10,  21,  95,  0,   25,
        8,   27,  95,  77,  29,  5,   32,  129, 96,  102, 5,   58,  18,  28,
        29,  4,   5,   29,  4,   27,  129, 5,   28,  5,   30,  129, 0,   29,
        30,  4,   3,   1,   16,  60,  16,  60,  43,  0,   43,  135, 33,  49,
        13,  20,  19,  15,  21,  129, 4,   14,  13,  20,  14,  18,  129, 0,
        19,  18,  15,  16,  134, 33,  130, 37,  52,  129, 23,  17,  51,  17,
        54,  54,  51,  50,  18,  21,  16,  2,   35,  34,  38,  38,  39,  7,
        19,  22,  74,  98,  5,   8,   130, 38,  35,  34,  6,   7,   197, 197,
        61,  181, 121, 201, 220, 249, 215, 63,  114, 89,  54,  24,  24,  101,
        120, 69,  145, 139, 123, 129, 82,  161, 79,  12,  6,   55,  253, 71,
        114, 114, 254, 216, 238, 254, 255, 254, 168, 30,  56,  62,  135, 1,
        15,  90,  46,  244, 189, 176, 206, 89,  90,  0,   1,   0,   111, 255,
        231, 3,   183, 4,   86,  0,   22,  0,   143, 64,  50,  233, 15,  233,
        19,  249, 15,  249, 19,  4,   37,  8,   12,  125, 9,   15,  11,  1,
        8,   11,  122, 80,  10,  14,  87,  64,  9,   7,   22,  125, 80,  0,
        124, 10,  0,   124, 20,  87,  2,   11,  12,  114, 0,   74,  24,  17,
        105, 6,   73,  23,  24,  184, 1,   105, 179, 33,  103, 199, 65,  37,
        5,   33,  237, 78,  84,  145, 5,   33,  237, 236, 66,  87,  6,   36,
        252, 26,  237, 63,  26,  66,  100, 7,   36,  26,  252, 94,  93,  94,
        78,  82,  5,   45,  26,  15,  19,  3,   8,   4,   37,  19,  3,   17,
        95,  0,   15,  8,   130, 4,   39,  18,  5,   20,  95,  0,   16,  7,
        14,  65,  50,  6,   38,  43,  129, 129, 1,   113, 37,  6,   130, 230,
        34,  2,   53,  52,  84,  47,  5,   130, 227, 33,  21,  20,  130, 239,
        8,   40,  55,  3,   183, 191, 139, 145, 242, 123, 1,   10,  1,   22,
        128, 156, 167, 105, 166, 168, 189, 151, 135, 143, 33,  58,  157, 1,
        0,   161, 240, 1,   65,  35,  165, 49,  247, 169, 183, 226, 69,  0,
        2,   131, 217, 33,  4,   68,  65,  253, 6,   8,   54,  191, 64,  53,
        31,  64,  27,  4,   31,  64,  10,  21,  34,  8,   30,  18,  1,   3,
        27,  14,  10,  21,  18,  17,  0,   15,  0,   21,  87,  10,  7,   0,
        10,  27,  126, 4,   11,  15,  0,   91,  17,  74,  32,  16,  32,  1,
        24,  105, 7,   73,  31,  32,  195, 33,  103, 161, 134, 214, 32,  93,
        131, 215, 35,  253, 60,  0,   63,  65,  251, 11,  33,  57,  18,  65,
        253, 9,   40,  5,   9,   26,  5,   24,  95,  0,   22,  9,   130, 4,
        39,  25,  6,   27,  95,  0,   23,  8,   21,  65,  253, 14,  49,  11,
        20,  12,  37,  19,  13,  21,  129, 5,   20,  11,  18,  129, 1,   19,
        18,  13,  14,  65,  219, 6,   33,  43,  43,  66,  31,  8,   47,  2,
        3,   29,  2,   27,  129, 4,   28,  3,   30,  129, 1,   29,  30,  2,
        1,   66,  31,  9,   52,  33,  53,  6,   6,   35,  34,  2,   53,  16,
        18,  51,  50,  22,  22,  23,  17,  51,  17,  3,   38,  38,  65,  18,
        7,   8,   53,  54,  55,  3,   126, 60,  181, 121, 200, 221, 248, 215,
        63,  114, 89,  54,  198, 198, 102, 119, 69,  144, 139, 123, 129, 82,
        161, 78,  204, 114, 115, 1,   39,  239, 1,   2,   1,   87,  30,  55,
        63,  2,   105, 249, 213, 3,   59,  90,  45,  242, 191, 177, 204, 90,
        89,  65,  33,  5,   62,  3,   230, 4,   87,  0,   21,  0,   28,  0,
        153, 64,  49,  59,  8,   13,  11,  21,  23,  22,  123, 13,  14,  14,
        18,  26,  87,  64,  9,   7,   21,  135, 65,  243, 5,   54,  18,  87,
        2,   11,  23,  147, 21,  114, 11,  74,  30,  22,  114, 14,  147, 5,
        73,  29,  30,  212, 33,  103, 230, 65,  29,  5,   41,  253, 228, 78,
        16,  246, 77,  228, 237, 0,   63,  65,  246, 13,  36,  253, 17,  57,
        47,  60,  68,  235, 5,   8,   43,  49,  48,  67,  121, 64,  41,  24,
        28,  3,   17,  16,  38,  7,   38,  17,  3,   14,  95,  0,   15,  14,
        25,  10,  23,  95,  1,   27,  8,   22,  95,  0,   28,  15,  4,   18,
        95,  0,   24,  11,  26,  95,  1,   28,  6,   130, 4,   38,  0,   43,
        43,  43,  1,   60,  43,  68,  69,  7,   32,  37,  130, 249, 51,  0,
        53,  52,  18,  54,  51,  50,  18,  17,  7,   33,  30,  2,   51,  50,
        54,  55,  1,   33,  52,  131, 255, 8,   115, 3,   225, 198, 141, 253,
        254, 222, 128, 207, 140, 188, 224, 1,   253, 83,  16,  92,  153, 118,
        60,  119, 123, 253, 96,  1,   223, 127, 99,  113, 127, 35,  60,  1,
        85,  232, 153, 1,   20,  134, 254, 242, 254, 243, 60,  144, 156, 87,
        21,  45,  1,   213, 163, 140, 151, 0,   0,   1,   0,   80,  0,   0,
        3,   45,  6,   68,  0,   23,  0,   114, 64,  25,  28,  8,   12,  135,
        64,  11,  122, 13,  14,  87,  9,   1,   21,  2,   123, 20,  3,   6,
        0,   10,  25,  71,  71,  74,  20,  186, 2,   64,  0,   11,  1,   222,
        64,  12,  18,  23,  91,  5,   0,   191, 3,   73,  24,  140, 216, 77,
        56,  10,  77,  57,  5,   77,  56,  6,   32,  63,  66,  232, 7,   33,
        252, 26,  66,  229, 5,   52,  18,  15,  17,  6,   8,   16,  37,  7,
        38,  15,  8,   18,  101, 0,   17,  6,   14,  101, 1,   43,  1,   130,
        219, 38,  129, 129, 51,  17,  35,  53,  51,  78,  18,  8,   8,   143,
        35,  34,  6,   6,   21,  21,  33,  21,  33,  17,  234, 154, 154, 69,
        157, 127, 100, 126, 133, 80,  46,  76,  46,  1,   15,  254, 241, 3,
        170, 148, 130, 134, 153, 101, 33,  156, 41,  29,  74,  116, 151, 148,
        252, 86,  0,   2,   0,   111, 254, 93,  4,   68,  4,   86,  0,   31,
        0,   44,  1,   19,  64,  29,  31,  64,  42,  10,  34,  64,  17,  36,
        58,  8,   1,   0,   33,  32,  7,   3,   42,  21,  22,  36,  87,  17,
        7,   22,  6,   42,  126, 64,  10,  184, 1,   67,  181, 45,  12,  1,
        125, 64,  0,   184, 1,   124, 64,  35,  12,  3,   87,  28,  15,  21,
        32,  6,   91,  24,  24,  23,  74,  46,  16,  46,  1,   1,   122, 48,
        0,   64,  0,   2,   0,   203, 39,  105, 13,  73,  45,  46,  195, 33,
        103, 185, 1,   26,  70,  178, 6,   38,  237, 244, 93,  228, 93,  78,
        16,  83,  102, 5,   33,  60,  0,   135, 222, 37,  27,  176, 80,  26,
        89,  253, 67,  213, 8,   8,   43,  16,  254, 26,  237, 63,  63,  237,
        17,  57,  17,  23,  57,  16,  201, 49,  48,  67,  121, 64,  42,  37,
        41,  25,  27,  11,  16,  4,   5,   26,  38,  15,  38,  41,  11,  39,
        95,  0,   4,   27,  6,   95,  1,   37,  16,  130, 9,   46,  40,  12,
        42,  95,  0,   5,   25,  3,   95,  0,   38,  14,  36,  95,  1,   104,
        80,  8,   32,  129, 98,  83,  7,   52,  20,  34,  35,  18,  20,  19,
        37,  34,  20,  36,  129, 5,   35,  18,  33,  129, 1,   34,  33,  20,
        21,  67,  42,  9,   67,  43,  6,   49,  43,  44,  8,   9,   44,  8,
        42,  129, 4,   43,  9,   32,  129, 1,   44,  32,  8,   7,   67,  43,
        9,   38,  19,  55,  22,  51,  50,  54,  53,  67,  49,  6,   51,  52,
        18,  54,  51,  50,  22,  22,  23,  53,  51,  17,  16,  6,   6,   35,
        34,  38,  38,  1,   17,  67,  58,  10,  8,   133, 201, 23,  171, 167,
        187, 145, 43,  182, 131, 223, 204, 120, 213, 137, 61,  105, 91,  56,
        198, 115, 245, 169, 55,  127, 99,  2,   100, 65,  141, 77,  131, 159,
        118, 133, 86,  161, 254, 151, 171, 81,  191, 162, 171, 90,  138, 1,
        65,  204, 155, 1,   13,  136, 28,  56,  64,  124, 252, 234, 254, 167,
        254, 116, 12,  19,  2,   252, 1,   195, 68,  67,  211, 200, 157, 197,
        90,  0,   0,   1,   0,   197, 0,   0,   4,   61,  6,   43,  0,   20,
        0,   135, 64,  42,  203, 18,  203, 19,  2,   31,  64,  6,   16,  22,
        8,   3,   19,  20,  1,   0,   16,  126, 6,   7,   20,  11,  10,  22,
        71,  71,  74,  11,  91,  10,  3,   20,  91,  10,  212, 0,   73,  21,
        22,  212, 33,  113, 65,  116, 10,  34,  253, 60,  16,  79,  40,  7,
        37,  60,  63,  77,  237, 63,  17,  91,  206, 6,   44,  14,  7,   15,
        14,  38,  15,  7,   12,  101, 1,   13,  8,   16,  66,  72,  5,   65,
        11,  6,   47,  17,  18,  4,   5,   18,  4,   16,  129, 5,   17,  5,
        19,  129, 0,   18,  19,  70,  87,  12,  33,  93,  51,  70,  54,  6,
        37,  22,  21,  17,  35,  17,  52,  132, 249, 8,   37,  7,   17,  197,
        197, 75,  199, 107, 138, 172, 197, 40,  72,  41,  73,  169, 99,  6,
        43,  253, 71,  109, 119, 175, 155, 252, 244, 2,   204, 113, 72,  39,
        90,  147, 253, 65,  0,   0,   2,   131, 203, 57,  1,   138, 5,   200,
        0,   3,   0,   7,   0,   74,  178, 6,   147, 7,   184, 1,   9,   64,
        29,  2,   2,   1,   6,   0,   10,  111, 74,  230, 5,   41,  2,   2,
        7,   3,   91,  1,   5,   111, 0,   127, 130, 47,  40,  73,  8,   9,
        184, 1,   44,  179, 33,  113, 69,  54,  5,   37,  93,  60,  60,  77,
        253, 60,  99,  91,  6,   42,  93,  0,   63,  63,  60,  77,  16,  246,
        237, 49,  48,  131, 144, 36,  3,   53,  51,  21,  197, 130, 0,   47,
        4,   62,  251, 194, 5,   3,   197, 197, 0,   2,   255, 102, 254, 93,
        1,   170, 130, 109, 42,  15,  0,   19,  0,   121, 180, 25,  8,   17,
        147, 16,  131, 111, 8,   43,  24,  8,   6,   1,   135, 64,  0,   124,
        14,  3,   87,  14,  15,  111, 21,  1,   21,  71,  71,  74,  19,  10,
        91,  16,  1,   184, 1,   223, 182, 127, 7,   1,   7,   73,  20,  21,
        186, 1,   44,  0,   33,  1,   134, 177, 134, 119, 33,  77,  230, 74,
        141, 10,  32,  77,  67,  148, 6,   32,  27,  89,  22,  6,   32,  63,
        131, 130, 51,  67,  121, 64,  16,  4,   13,  12,  38,  5,   37,  4,
        13,  7,   101, 1,   6,   11,  3,   101, 0,   67,  153, 5,   89,  51,
        12,  35,  6,   35,  34,  1,   130, 168, 52,  154, 138, 72,  49,  81,
        42,  198, 82,  165, 107, 105, 1,   5,   198, 254, 143, 156, 58,  37,
        81,  153, 131, 185, 36,  165, 161, 93,  6,   166, 130, 188, 65,  247,
        6,   45,  133, 6,   43,  0,   10,  0,   157, 178, 5,   6,   6,   184,
        2,   90,  89,  64,  15,  32,  89,  89,  64,  10,  63,  4,   9,   184,
        2,   74,  64,  38,  3,   3,   7,   5,   2,   1,   0,   5,   4,   6,
        0,   10,  10,  8,   8,   7,   10,  6,   4,   8,   3,   10,  5,   203,
        12,  130, 227, 38,  7,   3,   9,   10,  91,  64,  32,  130, 224, 40,
        184, 1,   108, 182, 32,  0,   73,  11,  113, 68,  120, 5,   48,  26,
        25,  77,  237, 93,  26,  24,  253, 60,  60,  25,  78,  16,  69,  101,
        68,  230, 89,  76,  14,  39,  63,  60,  25,  17,  18,  57,  47,  24,
        89,  77,  15,  77,  105, 5,   8,   36,  51,  17,  51,  17,  1,   51,
        1,   1,   35,  1,   17,  197, 197, 1,   204, 212, 254, 73,  2,   18,
        252, 254, 1,   6,   43,  252, 4,   2,   15,  254, 5,   253, 189, 2,
        45,  253, 211, 133, 207, 33,  1,   138, 74,  221, 6,   40,  24,  1,
        0,   0,   10,  111, 5,   1,   5,   130, 150, 35,  2,   3,   91,  1,
        65,  237, 6,   33,  4,   5,   65,  237, 13,  33,  77,  253, 76,  212,
        8,   89,  80,  6,   37,  197, 197, 6,   43,  249, 213, 133, 77,  8,
        57,  6,   191, 4,   87,  0,   35,  0,   247, 64,  57,  31,  64,  13,
        22,  31,  64,  6,   31,  44,  8,   11,  4,   31,  6,   24,  33,  22,
        25,  5,   3,   27,  35,  10,  18,  26,  22,  126, 13,  114, 31,  126,
        6,   1,   6,   6,   7,   17,  18,  18,  26,  26,  27,  27,  35,  35,
        0,   10,  37,  130, 117, 51,  17,  91,  18,  26,  91,  18,  186, 1,
        101, 0,   27,  1,   101, 182, 71,  35,  91,  0,   73,  36,  67,  51,
        6,   42,  78,  16,  244, 77,  237, 118, 244, 228, 24,  237, 16,  75,
        98,  6,   78,  105, 5,   43,  16,  60,  16,  60,  63,  63,  77,  16,
        237, 244, 237, 1,   83,  195, 5,   33,  57,  0,   104, 77,  9,   8,
        36,  67,  121, 64,  30,  29,  30,  14,  21,  7,   9,   8,   37,  21,
        14,  19,  101, 1,   30,  7,   28,  101, 1,   9,   10,  20,  15,  22,
        101, 1,   29,  9,   31,  101, 1,   43,  43,  1,   74,  196, 7,   53,
        0,   73,  84,  121, 64,  18,  32,  33,  4,   5,   33,  4,   31,  129,
        5,   32,  5,   34,  129, 0,   33,  34,  67,  101, 12,  132, 33,  49,
        23,  24,  11,  12,  24,  11,  22,  129, 5,   23,  12,  25,  129, 0,
        24,  25,  11,  10,  68,  147, 9,   36,  51,  17,  51,  21,  54,  68,
        136, 5,   132, 6,   67,  140, 5,   33,  35,  34,  130, 234, 35,  35,
        17,  52,  38,  132, 8,   8,   55,  197, 197, 60,  187, 123, 71,  106,
        86,  34,  60,  185, 119, 116, 186, 198, 91,  56,  70,  151, 100, 198,
        92,  58,  71,  145, 103, 4,   62,  204, 95,  133, 46,  84,  98,  98,
        131, 144, 177, 252, 234, 2,   247, 108, 78,  87,  130, 253, 40,  2,
        247, 109, 78,  86,  132, 253, 40,  68,  119, 8,   39,  4,   86,  0,
        20,  0,   147, 64,  48,  68,  119, 13,  32,  16,  65,  82,  5,   37,
        10,  11,  11,  20,  20,  0,   68,  124, 6,   34,  9,   10,  2,   68,
        125, 24,  32,  60,  68,  126, 8,   65,  67,  8,   68,  131, 66,  133,
        253, 68,  131, 25,  130, 226, 68,  130, 15,  73,  193, 5,   32,  123,
        130, 213, 8,   37,  15,  0,   27,  0,   122, 64,  33,  67,  8,   22,
        87,  8,   7,   16,  87,  0,   11,  19,  105, 12,  74,  29,  0,   29,
        16,  29,  32,  29,  3,   25,  105, 4,   73,  28,  29,  228, 33,  103,
        75,  170, 6,   33,  237, 93,  75,  168, 8,   113, 71,  19,  45,  95,
        1,   27,  1,   25,  95,  0,   21,  9,   19,  95,  1,   23,  7,   130,
        9,   38,  18,  13,  16,  95,  0,   26,  3,   130, 4,   38,  20,  11,
        22,  95,  1,   24,  5,   130, 4,   89,  164, 15,  113, 71,  15,  118,
        86,  8,   8,   56,  22,  2,   117, 155, 229, 134, 122, 233, 163, 155,
        230, 133, 124, 231, 163, 147, 162, 163, 146, 146, 162, 162, 25,  127,
        1,   17,  168, 158, 1,   15,  138, 126, 254, 238, 167, 160, 254, 241,
        137, 148, 219, 201, 202, 217, 217, 202, 201, 219, 0,   0,   2,   0,
        197, 254, 117, 4,   154, 130, 213, 39,  17,  0,   30,  0,   196, 64,
        56,  28,  76,  149, 6,   32,  37,  76,  149, 7,   47,  18,  30,  24,
        17,  27,  126, 6,   7,   1,   6,   21,  87,  13,  11,  0,   14,  76,
        150, 8,   37,  16,  17,  91,  0,   0,   1,   76,  151, 11,  36,  60,
        77,  16,  253, 60,  76,  153, 8,   34,  63,  237, 63,  88,  253, 9,
        74,  156, 6,   40,  26,  22,  26,  7,   12,  11,  38,  22,  12,  76,
        156, 22,  76,  123, 8,   76,  157, 32,  33,  16,  14,  76,  157, 5,
        76,  155, 15,  34,  43,  129, 19,  66,  220, 6,   33,  18,  21,  101,
        138, 5,   34,  39,  17,  17,  76,  153, 18,  37,  102, 215, 136, 54,
        177, 100, 76,  151, 9,   51,  254, 117, 5,   201, 204, 114, 114, 254,
        216, 238, 155, 254, 232, 166, 32,  116, 253, 250, 2,   142, 76,  151,
        8,   38,  2,   0,   111, 254, 117, 4,   68,  65,  37,  6,   34,  211,
        64,  11,  75,  189, 9,   8,   38,  1,   184, 1,   6,   64,  49,  30,
        14,  135, 18,  18,  30,  27,  14,  15,  21,  18,  30,  17,  0,   16,
        6,   21,  87,  10,  7,   27,  126, 4,   11,  0,   14,  14,  1,   0,
        91,  17,  17,  16,  75,  201, 25,  65,  52,  5,   35,  0,   63,  63,
        237, 77,  200, 6,   51,  57,  0,   17,  18,  57,  18,  57,  57,  16,
        237, 16,  253, 49,  48,  67,  121, 64,  24,  22,  26,  75,  209, 96,
        33,  1,   17,  75,  209, 12,  32,  53,  75,  209, 39,  35,  254, 117,
        2,   87,  75,  212, 11,  36,  124, 250, 55,  4,   198, 75,  211, 8,
        8,   57,  1,   0,   197, 0,   0,   3,   29,  4,   86,  0,   15,  0,
        113, 64,  31,  31,  64,  6,   11,  14,  3,   0,   2,   1,   6,   11,
        126, 6,   9,   147, 8,   114, 6,   7,   1,   6,   0,   10,  17,  71,
        71,  74,  224, 9,   1,   9,   184, 1,   224, 64,  10,  3,   14,  15,
        91,  0,   73,  16,  70,  94,  6,   58,  77,  253, 60,  60,  246, 93,
        78,  69,  101, 68,  230, 0,   63,  63,  63,  77,  244, 237, 16,  237,
        63,  60,  17,  57,  57,  49,  48,  69,  7,   5,   47,  12,  13,  4,
        5,   13,  4,   11,  129, 5,   12,  5,   14,  129, 0,   13,  14,  69,
        7,   11,  68,  229, 7,   33,  23,  21,  68,  209, 7,   62,  56,  179,
        105, 30,  33,  50,  40,  93,  157, 63,  4,   62,  204, 109, 119, 5,
        184, 17,  111, 111, 253, 52,  0,   0,   1,   0,   157, 255, 231, 3,
        119, 130, 167, 50,  41,  0,   205, 64,  53,  148, 9,   148, 34,  2,
        43,  8,   33,  30,  12,  9,   4,   24,  3,   132, 6,   59,  15,  36,
        24,  87,  18,  22,  125, 64,  21,  122, 11,  18,  7,   3,   87,  40,
        1,   125, 64,  0,   124, 11,  40,  11,  144, 43,  1,   43,  130, 190,
        51,  22,  201, 6,   184, 2,   59,  181, 16,  36,  224, 36,  2,   36,
        187, 1,   60,  0,   0,   0,   27,  130, 15,  38,  15,  114, 0,   73,
        42,  43,  186, 130, 15,  35,  33,  1,   39,  177, 76,  161, 5,   42,
        125, 77,  244, 24,  237, 16,  253, 93,  237, 125, 228, 102, 226, 5,
        32,  93,  109, 40,  12,  101, 60,  6,   113, 76,  15,  34,  17,  18,
        23,  116, 36,  9,   62,  30,  37,  39,  25,  26,  16,  17,  4,   5,
        38,  38,  25,  17,  27,  101, 0,   4,   39,  6,   101, 1,   26,  16,
        24,  101, 1,   5,   37,  3,   101, 0,   88,  165, 5,   32,  129, 130,
        0,   41,  0,   93,  55,  53,  22,  51,  50,  54,  53,  52,  89,  247,
        9,   34,  51,  50,  23,  65,  16,  5,   34,  21,  20,  23,  89,  246,
        7,   8,   159, 6,   6,   35,  34,  157, 193, 143, 84,  113, 28,  28,
        101, 115, 133, 61,  61,  190, 186, 91,  134, 41,  165, 98,  95,  94,
        32,  31,  81,  95,  191, 52,  52,  97,  189, 116, 135, 38,  181, 96,
        92,  69,  43,  41,  41,  42,  49,  55,  73,  72,  105, 131, 174, 25,
        8,   164, 49,  82,  56,  44,  35,  36,  35,  40,  81,  79,  79,  93,
        89,  152, 86,  0,   0,   1,   0,   69,  255, 231, 2,   174, 5,   22,
        0,   21,  0,   123, 64,  35,  19,  8,   11,  10,  205, 8,   16,  14,
        7,   123, 13,  8,   6,   19,  87,  2,   21,  135, 0,   122, 2,   11,
        144, 23,  1,   23,  71,  71,  74,  13,  114, 32,  21,  1,   21,  184,
        1,   215, 64,  18,  11,  16,  91,  10,  176, 5,   1,   5,   191, 159,
        7,   1,   7,   73,  22,  140, 199, 24,  43,  78,  16,  244, 93,  77,
        252, 93,  60,  253, 60,  246, 93,  228, 78,  84,  157, 6,   65,  254,
        6,   59,  253, 60,  75,  81,  88,  176, 128, 26,  89,  16,  252, 50,
        49,  48,  67,  121, 64,  12,  3,   18,  18,  3,   16,  101, 0,   17,
        4,   19,  130, 253, 8,   56,  1,   43,  129, 33,  6,   35,  34,  38,
        53,  17,  35,  53,  51,  53,  55,  21,  33,  21,  33,  17,  20,  22,
        51,  50,  55,  2,   169, 91,  67,  146, 172, 136, 136, 197, 1,   28,
        254, 228, 88,  96,  39,  56,  25,  169, 156, 2,   126, 148, 197, 19,
        216, 148, 253, 166, 114, 99,  13,  131, 191, 8,   56,  185, 255, 231,
        4,   49,  4,   62,  0,   20,  0,   142, 64,  26,  31,  64,  14,  4,
        22,  8,   1,   125, 17,  17,  9,   0,   20,  10,  19,  18,  18,  9,
        9,   8,   6,   14,  126, 4,   11,  22,  184, 1,   9,   181, 20,  17,
        1,   147, 64,  20,  184, 1,   169, 179, 192, 10,  91,  7,   130, 17,
        35,  182, 21,  22,  212, 74,  110, 5,   42,  246, 237, 26,  253, 26,
        253, 60,  16,  230, 0,   63,  90,  93,  6,   36,  63,  60,  17,  57,
        47,  67,  206, 5,   44,  14,  5,   13,  12,  38,  13,  5,   10,  101,
        0,   11,  6,   14,  132, 175, 79,  114, 7,   47,  15,  16,  2,   3,
        16,  2,   14,  129, 4,   15,  3,   17,  129, 1,   16,  17,  79,  114,
        17,  130, 212, 32,  51,  82,  172, 5,   8,   45,  54,  55,  17,  51,
        17,  3,   108, 76,  198, 107, 137, 173, 197, 40,  72,  42,  72,  169,
        99,  197, 204, 110, 119, 175, 156, 3,   12,  253, 52,  113, 72,  40,
        90,  147, 2,   192, 251, 194, 0,   1,   0,   18,  0,   0,   4,   24,
        130, 209, 8,   46,  6,   0,   173, 64,  20,  148, 3,   1,   8,   0,
        7,   6,   25,  0,   22,  6,   41,  0,   38,  6,   185, 0,   182, 6,
        8,   184, 255, 124, 180, 3,   0,   6,   32,  3,   184, 1,   85,  181,
        6,   12,  2,   1,   0,   0,   184, 2,   89,  105, 213, 7,   99,  128,
        5,   34,  58,  64,  27,  89,  176, 5,   41,  5,   4,   4,   2,   2,
        1,   6,   6,   0,   10,  90,  135, 12,  48,  56,  0,   3,   1,   56,
        182, 0,   1,   1,   1,   73,  7,   235, 185, 1,   66,  0,   85,  15,
        5,   32,  93,  90,  133, 15,  90,  132, 9,   32,  135, 88,  95,  13,
        47,  0,   75,  82,  88,  176, 128, 26,  89,  24,  16,  253, 43,  49,
        48,  1,   93,  90,  144, 8,   49,  1,   166, 254, 108, 197, 1,   60,
        1,   77,  184, 254, 83,  4,   62,  252, 179, 3,   77,  132, 213, 36,
        12,  0,   0,   6,   28,  130, 213, 51,  12,  1,   25,  64,  19,  0,
        11,  1,   150, 9,   1,   154, 4,   149, 12,  186, 4,   181, 12,  4,
        142, 193, 90,  144, 5,   34,  58,  64,  10,  90,  144, 11,  32,  90,
        90,  144, 13,  32,  91,  98,  235, 7,   58,  11,  4,   3,   187, 255,
        252, 0,   3,   0,   6,   1,   85,  183, 0,   12,  8,   7,   7,   5,
        5,   11,  184, 1,   85,  64,  31,  4,   130, 88,  130, 249, 90,  150,
        5,   132, 253, 90,  150, 15,  34,  151, 0,   128, 130, 54,  32,  92,
        130, 141, 46,  92,  0,   64,  0,   3,   1,   151, 183, 128, 0,   1,
        1,   1,   73,  13,  65,  17,  11,  38,  125, 26,  24,  77,  253, 26,
        253, 130, 2,   32,  24,  91,  158, 11,  90,  149, 6,   32,  24,  74,
        46,  5,   131, 5,   43,  75,  81,  88,  176, 64,  26,  89,  77,  16,
        237, 60,  16,  130, 1,   65,  31,  5,   32,  27,  131, 18,  36,  16,
        253, 60,  56,  56,  90,  169, 27,  34,  1,   93,  93,  91,  210, 6,
        34,  19,  1,   51,  130, 2,   60,  1,   35,  3,   1,   1,   67,  254,
        201, 193, 238, 1,   1,   197, 225, 1,   17,  169, 254, 158, 198, 230,
        254, 252, 4,   62,  252, 183, 3,   73,  131, 3,   33,  251, 194, 131,
        7,   8,   44,  0,   0,   1,   0,   88,  0,   0,   4,   139, 4,   62,
        0,   11,  0,   194, 177, 5,   7,   184, 255, 253, 64,  26,  1,   3,
        7,   11,  6,   3,   8,   10,  2,   9,   11,  6,   4,   3,   8,   0,
        5,   1,   0,   5,   2,   9,   90,  149, 5,   32,  89,  90,  149, 13,
        33,  90,  182, 90,  148, 5,   39,  7,   184, 1,   68,  64,  31,  1,
        1,   89,  175, 6,   47,  6,   8,   9,   9,   11,  11,  0,   10,  6,
        122, 8,   74,  13,  2,   114, 0,   130, 24,  40,  73,  12,  13,  184,
        1,   174, 179, 33,  140, 83,  171, 5,   33,  93,  77,  81,  182, 5,
        107, 232, 6,   65,  5,   5,   37,  25,  18,  57,  47,  24,  237, 141,
        232, 32,  15,  130, 0,   8,   40,  1,   56,  56,  49,  48,  67,  88,
        64,  27,  185, 0,   182, 2,   182, 6,   185, 8,   4,   134, 0,   137,
        2,   137, 6,   134, 8,   4,   118, 0,   121, 2,   121, 6,   118, 8,
        4,   0,   93,  93,  93,  89,  90,  145, 11,  8,   35,  88,  1,   157,
        254, 111, 234, 1,   61,  1,   31,  192, 254, 136, 1,   153, 234, 254,
        185, 254, 196, 2,   49,  2,   13,  254, 94,  1,   162, 253, 217, 253,
        233, 1,   174, 254, 82,  130, 255, 36,  16,  254, 117, 4,   34,  130,
        255, 8,   53,  7,   0,   210, 64,  14,  8,   1,   8,   2,   7,   3,
        7,   4,   4,   6,   70,  7,   1,   4,   184, 255, 240, 64,  13,  4,
        4,   3,   1,   5,   1,   2,   5,   0,   3,   2,   1,   1,   184, 2,
        89,  64,  12,  4,   3,   20,  4,   1,   0,   4,   3,   5,   6,   7,
        7,   131, 16,  32,  35,  65,  2,   5,   37,  15,  4,   1,   4,   4,
        1,   90,  175, 5,   37,  6,   1,   10,  7,   0,   14,  130, 32,  44,
        3,   4,   2,   9,   71,  71,  74,  6,   188, 1,   56,  0,   4,   130,
        3,   45,  0,   1,   84,  64,  10,  0,   2,   1,   2,   73,  8,   9,
        228, 33,  66,  38,  6,   39,  43,  25,  78,  244, 93,  24,  77,  228,
        93,  191, 7,   42,  17,  18,  57,  57,  17,  57,  0,   24,  63,  60,
        63,  65,  23,  5,   35,  18,  57,  47,  93,  67,  62,  8,   102, 154,
        7,   91,  133, 7,   49,  0,   56,  1,   93,  0,   176, 1,   70,  176,
        2,   70,  176, 64,  97,  82,  88,  177, 1,   132, 8,   39,  72,  89,
        1,   94,  93,  1,   19,  1,   65,  18,  5,   8,   47,  4,   176, 254,
        92,  213, 1,   55,  1,   76,  186, 253, 175, 254, 117, 1,   139, 4,
        62,  252, 206, 3,   50,  250, 55,  0,   0,   1,   0,   123, 0,   0,
        4,   27,  4,   62,  0,   9,   0,   137, 64,  10,  5,   217, 2,   212,
        7,   2,   6,   134, 235, 8,   41,  25,  7,   6,   20,  7,   7,   6,
        1,   7,   6,   4,   3,   3,   2,   123, 5,   5,   4,   6,   8,   7,
        123, 9,   0,   10,  2,   184, 1,   85,  64,  11,  5,   5,   6,   114,
        9,   74,  11,  192, 11,  1,   7,   130, 15,  47,  182, 1,   3,   122,
        0,   73,  10,  11,  186, 2,   8,   0,   33,  1,   34,  177, 75,  15,
        6,   43,  228, 60,  237, 113, 78,  16,  246, 77,  244, 60,  16,  237,
        106, 168, 6,   34,  16,  253, 60,  130, 227, 33,  18,  57,  101, 48,
        11,  45,  64,  9,   22,  1,   25,  6,   185, 2,   182, 7,   4,   0,
        93,  89,  90,  255, 9,   50,  123, 2,   160, 253, 127, 3,   116, 253,
        96,  2,   173, 148, 3,   22,  148, 148, 252, 234, 148, 130, 179, 8,
        65,  25,  254, 216, 2,   19,  6,   43,  0,   50,  0,   92,  64,  12,
        0,   5,   19,  3,   12,  50,  45,  31,  3,   25,  25,  38,  184, 1,
        113, 64,  37,  12,  16,  45,  5,   19,  31,  25,  5,   28,  42,  208,
        52,  1,   52,  71,  71,  74,  12,  38,  179, 22,  28,  173, 8,   42,
        179, 159, 50,  191, 50,  2,   50,  73,  51,  187, 152, 70,  113, 6,
        33,  244, 60,  91,  13,  9,   43,  17,  18,  23,  57,  0,   63,  77,
        237, 25,  57,  47,  23,  85,  93,  5,   58,  19,  51,  50,  53,  52,
        39,  39,  38,  53,  52,  54,  54,  55,  21,  35,  34,  6,   21,  20,
        23,  23,  22,  21,  20,  6,   7,   22,  130, 5,   33,  7,   7,   130,
        15,  8,   92,  22,  51,  51,  21,  46,  2,   53,  52,  55,  55,  54,
        53,  52,  35,  35,  25,  49,  153, 16,  19,  19,  85,  142, 131, 28,
        53,  105, 13,  17,  11,  70,  77,  77,  70,  11,  17,  13,  103, 55,
        28,  131, 142, 85,  19,  19,  16,  153, 49,  2,   204, 161, 68,  72,
        87,  85,  83,  78,  141, 80,  8,   148, 62,  62,  23,  72,  102, 65,
        92,  105, 142, 64,  65,  141, 108, 90,  65,  102, 72,  24,  60,  63,
        148, 8,   81,  140, 79,  82,  85,  87,  72,  70,  160, 0,   91,  233,
        5,   33,  1,   89,  130, 233, 37,  3,   0,   49,  185, 0,   3,   130,
        221, 24,  65,  213, 7,   34,  2,   3,   85,  130, 31,  35,  73,  4,
        5,   238, 126, 248, 7,   32,  60,  116, 75,  11,  41,  77,  237, 49,
        48,  19,  17,  51,  17,  197, 148, 91,  105, 9,   36,  136, 254, 216,
        2,   130, 130, 75,  55,  50,  0,   109, 64,  18,  22,  20,  5,   8,
        45,  50,  6,   12,  30,  28,  42,  45,  50,  5,   38,  25,  25,  12,
        184, 130, 93,  51,  44,  38,  16,  30,  20,  45,  5,   25,  5,   42,
        22,  159, 52,  191, 52,  208, 52,  3,   9,   0,   65,  67,  5,   57,
        0,   179, 8,   42,  128, 22,  28,  179, 12,  15,  38,  1,   8,   38,
        73,  51,  112, 217, 24,  43,  78,  16,  244, 94,  93,  60,  93,  113,
        10,  34,  93,  94,  93,  65,  69,  10,  32,  18,  65,  70,  6,   34,
        1,   35,  34,  65,  56,  7,   34,  6,   7,   53,  72,  158, 5,   38,
        39,  38,  53,  52,  54,  55,  38,  130, 5,   55,  55,  55,  54,  53,
        52,  38,  35,  35,  53,  30,  2,   21,  20,  7,   7,   6,   21,  20,
        51,  51,  2,   130, 50,  152, 65,  71,  6,   45,  55,  103, 13,  17,
        12,  69,  78,  77,  70,  12,  17,  13,  104, 54,  65,  71,  6,   36,
        152, 50,  2,   56,  162, 65,  71,  5,   59,  140, 81,  8,   148, 63,
        60,  26,  70,  102, 64,  94,  103, 141, 67,  65,  141, 106, 92,  64,
        102, 70,  27,  59,  63,  148, 8,   80,  141, 65,  71,  5,   54,  159,
        0,   0,   1,   0,   105, 1,   151, 4,   167, 3,   5,   0,   28,  0,
        83,  64,  36,  8,   6,   6,   85,  23,  131, 146, 51,  21,  6,   8,
        23,  21,  4,   18,  15,  108, 25,  88,  64,  3,   250, 128, 10,  88,
        0,   108, 18,  132, 18,  8,   33,  28,  14,  188, 2,   63,  0,   15,
        1,   175, 0,   28,  2,   63,  178, 0,   221, 29,  16,  246, 237, 253,
        237, 17,  23,  57,  0,   47,  228, 237, 26,  253, 26,  237, 228, 130,
        220, 32,  135, 114, 18,  8,   40,  19,  52,  54,  51,  50,  22,  31,
        2,   22,  131, 220, 8,   61,  53,  51,  6,   6,   35,  34,  38,  47,
        2,   38,  35,  34,  6,   21,  105, 156, 120, 52,  98,  65,  107, 128,
        49,  42,  47,  74,  148, 12,  152, 102, 55,  90,  100, 112, 119, 40,
        31,  44,  81,  1,   188, 143, 186, 37,  34,  56,  69,  22,  91,  87,
        7,   185, 148, 29,  51,  58,  62,  18,  84,  97,  255, 255, 113, 137,
        5,   37,  6,   241, 2,   50,  0,   36,  130, 184, 50,  23,  0,   142,
        0,   74,  1,   65,  0,   22,  64,  12,  3,   2,   11,  17,  1,   2,
        72,  51,  130, 7,   42,  32,  0,   63,  53,  53,  1,   43,  53,  53,
        0,   3,   134, 47,  8,   42,  227, 0,   2,   0,   21,  0,   33,  0,
        236, 64,  14,  1,   19,  18,  18,  2,   0,   20,  21,  21,  2,   16,
        2,   18,  18,  184, 2,   92,  64,  10,  17,  16,  20,  17,  17,  16,
        4,   2,   21,  21,  184, 2,   53,  98,  106, 6,   45,  2,   65,  10,
        2,   65,  0,   4,   0,   28,  1,   71,  0,   64,  0,   131, 11,  38,
        192, 0,   22,  1,   196, 64,  9,   130, 39,  53,  1,   0,   89,  64,
        19,  20,  184, 1,   150, 64,  21,  21,  17,  18,  21,  3,   8,   21,
        20,  0,   3,   3,   130, 88,  56,  3,   2,   35,  71,  71,  74,  17,
        184, 1,   16,  178, 2,   32,  25,  184, 1,   125, 179, 13,  176, 64,
        16,  184, 1,   74,  130, 14,  32,  31,  131, 14,  49,  7,   176, 64,
        4,   186, 1,   74,  0,   2,   1,   16,  64,  10,  32,  3,   73,  34,
        35,  113, 230, 7,   44,  244, 26,  25,  77,  253, 253, 26,  24,  252,
        237, 26,  25,  16,  136, 7,   75,  18,  5,   42,  18,  23,  57,  17,
        23,  57,  0,   63,  60,  60,  60,  113, 243, 8,   46,  237, 26,  253,
        26,  237, 16,  229, 135, 5,   46,  43,  135, 14,  125, 196, 130, 7,
        32,  24,  132, 8,   37,  7,   5,   16,  60,  60,  7,   130, 3,   37,
        49,  48,  1,   33,  3,   1,   121, 83,  12,  34,  1,   35,  3,   130,
        18,  32,  50,  79,  31,  9,   48,  1,   180, 1,   249, 252, 253, 96,
        2,   75,  28,  29,  96,  65,  65,  95,  28,  28,  114, 26,  6,   8,
        118, 1,   240, 36,  50,  50,  36,  36,  51,  51,  2,   54,  2,   126,
        251, 76,  5,   200, 26,  61,  36,  66,  94,  94,  66,  35,  62,  26,
        250, 56,  1,   154, 254, 102, 5,   236, 51,  36,  35,  51,  51,  35,
        35,  52,  0,   0,   1,   0,   104, 254, 117, 5,   23,  5,   237, 0,
        46,  0,   193, 64,  20,  62,  8,   4,   23,  13,  14,  22,  11,  36,
        119, 30,  33,  142, 64,  32,  176, 80,  30,  3,   11,  184, 1,   74,
        64,  16,  15,  223, 22,  141, 23,  175, 3,   46,  142, 64,  0,   141,
        80,  42,  137, 4,   184, 1,   70,  180, 3,   9,   4,   22,  13,  184,
        1,   11,  180, 19,  119, 7,   222, 23,  132, 38,  44,  3,   3,   27,
        33,  175, 0,   74,  48,  39,  121, 27,  73,  47,  104, 69,  12,  47,
        228, 18,  57,  47,  237, 244, 237, 253, 57,  57,  0,   63,  238, 237,
        26,  252, 130, 255, 36,  244, 244, 246, 237, 63,  132, 9,   32,  237,
        70,  23,  5,   61,  49,  48,  67,  121, 64,  44,  37,  41,  24,  29,
        17,  18,  8,   10,  9,   38,  25,  37,  17,  10,  19,  220, 1,   37,
        29,  39,  110, 0,   41,  24,  130, 4,   49,  18,  8,   16,  220, 0,
        38,  28,  36,  110, 1,   40,  26,  42,  110, 0,   24,  23,  16,  127,
        55,  9,   32,  129, 130, 0,   40,  37,  6,   6,   35,  7,   22,  22,
        21,  20,  130, 7,   33,  34,  39,  76,  46,  6,   36,  38,  39,  55,
        38,  36,  111, 28,  5,   113, 140, 16,  57,  143, 229, 81,  41,  125,
        59,  52,  120, 46,  62,  97,  53,  68,  51,  59,  88,  81,  71,  234,
        254, 255, 154, 1,   108, 1,   112, 113, 154, 16,  58,  76,  36,  68,
        29,  84,  34,  36,  72,  36,  34,  79,  19,  39,  25,  32,  56,  11,
        105, 19,  189, 1,   64,  228, 1,   107, 1,   175, 113, 170, 15,  33,
        255, 255, 112, 253, 5,   8,   35,  7,   108, 2,   50,  0,   40,  0,
        0,   1,   23,  0,   141, 255, 253, 1,   65,  0,   18,  64,  10,  1,
        12,  14,  1,   2,   72,  51,  1,   12,  32,  0,   63,  53,  1,   43,
        53,  133, 43,  35,  5,   42,  7,   46,  130, 43,  32,  49,  132, 43,
        34,  216, 0,   152, 134, 43,  35,  10,  23,  1,   5,   130, 43,  32,
        10,  137, 43,  38,  104, 255, 219, 5,   208, 6,   241, 130, 43,  32,
        50,  132, 43,  34,  142, 0,   167, 67,  65,  7,   40,  29,  35,  4,
        12,  72,  51,  3,   2,   28,  67,  65,  8,   130, 91,  36,  179, 255,
        219, 4,   217, 132, 47,  32,  56,  134, 47,  32,  80,  133, 47,  42,
        2,   1,   22,  28,  0,   11,  72,  51,  2,   1,   21,  139, 47,  38,
        103, 255, 231, 4,   57,  6,   43,  130, 95,  32,  68,  130, 47,  36,
        22,  0,   141, 219, 0,   131, 181, 36,  2,   48,  50,  6,   24,  130,
        44,  33,  48,  34,  136, 181, 143, 41,  32,  67,  134, 41,  33,  49,
        51,  159, 41,  33,  215, 182, 133, 83,  33,  53,  49,  132, 41,  32,
        53,  142, 83,  33,  5,   176, 136, 125, 34,  142, 195, 0,   68,  29,
        5,   33,  49,  55,  131, 42,  33,  3,   2,   132, 127, 36,  53,  1,
        43,  53,  53,  130, 219, 132, 129, 33,  5,   237, 136, 45,  32,  216,
        130, 45,  37,  18,  64,  10,  2,   48,  61,  149, 129, 32,  68,  136,
        41,  32,  220, 135, 87,  33,  51,  57,  143, 87,  8,   53,  0,   1,
        0,   111, 254, 122, 3,   183, 4,   86,  0,   43,  0,   206, 64,  23,
        59,  8,   4,   22,  12,  11,  21,  9,   33,  125, 80,  9,   15,  32,
        1,   8,   32,  122, 35,  87,  29,  7,   9,   187, 1,   5,   0,   64,
        0,   16,  1,   47,  64,  21,  22,  114, 3,   43,  130, 28,  8,   43,
        0,   0,   1,   8,   0,   124, 41,  125, 3,   11,  4,   21,  6,   11,
        184, 1,   42,  178, 19,  87,  6,   186, 2,   64,  0,   22,  1,   33,
        64,  14,  3,   3,   26,  33,  114, 0,   74,  45,  38,  105, 26,  73,
        44,  45,  93,  218, 18,  55,  18,  57,  47,  237, 244, 253, 244, 17,
        57,  57,  0,   63,  237, 252, 94,  93,  94,  26,  237, 16,  244, 244,
        26,  237, 135, 12,  67,  33,  10,  54,  42,  36,  40,  23,  28,  17,
        18,  7,   8,   24,  37,  17,  8,   19,  101, 1,   36,  28,  38,  95,
        0,   40,  23,  130, 4,   48,  18,  7,   16,  101, 0,   37,  27,  35,
        95,  1,   39,  25,  41,  95,  0,   23,  22,  67,  31,  9,   67,  30,
        6,   44,  7,   7,   22,  21,  20,  6,   35,  34,  39,  53,  23,  23,
        22,  70,  174, 5,   33,  55,  46,  94,  9,   5,   34,  22,  23,  21,
        92,  247, 8,   61,  55,  3,   183, 85,  122, 80,  51,  157, 106, 83,
        62,  60,  10,  22,  14,  20,  48,  53,  123, 72,  140, 215, 109, 1,
        10,  1,   27,  58,  93,  128, 94,  26,  5,   8,   51,  134, 144, 33,
        25,  29,  4,   83,  41,  104, 59,  78,  17,  77,  1,   4,   3,   41,
        32,  67,  23,  118, 15,  159, 240, 154, 245, 1,   64,  11,  24,  165,
        49,  247, 169, 182, 227, 69,  0,   255, 255, 0,   111, 255, 231, 3,
        230, 6,   43,  2,   50,  0,   72,  66,  83,  5,   46,  180, 0,   0,
        18,  64,  10,  2,   29,  31,  5,   12,  72,  51,  2,   29,  65,  255,
        9,   143, 41,  33,  67,  217, 133, 41,  33,  30,  32,  159, 41,  33,
        215, 230, 133, 41,  33,  34,  30,  132, 41,  32,  34,  142, 83,  33,
        5,   176, 136, 125, 32,  142, 130, 83,  38,  22,  64,  12,  3,   2,
        30,  36,  131, 42,  32,  3,   133, 127, 66,  83,  7,   36,  123, 0,
        0,   2,   55,  132, 171, 32,  214, 130, 171, 37,  23,  0,   141, 254,
        179, 0,   132, 89,  40,  1,   4,   7,   1,   2,   72,  51,  1,   4,
        137, 89,  32,  25,  130, 33,  32,  213, 138, 43,  32,  67,  153, 43,
        33,  255, 219, 130, 87,  32,  117, 138, 43,  32,  215, 136, 43,  33,
        9,   5,   132, 87,  32,  9,   137, 87,  32,  12,  130, 43,  32,  68,
        132, 177, 133, 131, 32,  142, 132, 43,  130, 179, 35,  2,   1,   5,
        11,  131, 44,  32,  2,   133, 133, 135, 179, 38,  197, 0,   0,   4,
        61,  5,   237, 130, 225, 32,  81,  130, 145, 35,  22,  0,   216, 37,
        133, 177, 35,  21,  34,  0,   10,  130, 177, 130, 6,   68,  105, 7,
        36,  111, 255, 231, 4,   123, 132, 133, 32,  82,  132, 41,  32,  141,
        133, 219, 40,  2,   28,  30,  4,   12,  72,  51,  2,   28,  65,  53,
        12,  140, 41,  32,  67,  134, 41,  33,  29,  31,  159, 41,  32,  215,
        134, 41,  33,  33,  29,  132, 41,  32,  33,  142, 83,  132, 215, 133,
        125, 32,  142, 130, 41,  130, 213, 33,  3,   2,   68,  185, 8,   131,
        43,  135, 213, 132, 171, 132, 213, 133, 45,  32,  216, 134, 87,  33,
        28,  41,  143, 129, 32,  185, 130, 213, 32,  49,  132, 213, 32,  88,
        139, 213, 36,  1,   21,  23,  8,   19,  141, 255, 143, 41,  134, 213,
        34,  1,   22,  24,  159, 41,  134, 213, 34,  1,   26,  22,  132, 41,
        32,  26,  137, 213, 132, 83,  132, 213, 133, 125, 134, 213, 35,  2,
        1,   22,  28,  131, 42,  34,  2,   1,   21,  136, 213, 8,   68,  0,
        1,   0,   228, 254, 216, 4,   43,  5,   200, 0,   11,  0,   136, 185,
        0,   0,   2,   16,  64,  16,  2,   10,  1,   100, 35,  9,   2,   85,
        3,   64,  7,   4,   100, 35,  8,   3,   184, 1,   97,  182, 5,   2,
        13,  71,  71,  74,  8,   184, 1,   50,  64,  16,  6,   11,  7,   10,
        100, 26,  11,  127, 0,   64,  4,   1,   100, 26,  5,   0,   130, 20,
        57,  181, 2,   73,  12,  136, 196, 24,  43,  78,  16,  244, 77,  246,
        60,  75,  82,  88,  176, 0,   26,  89,  252, 60,  26,  16,  253, 136,
        11,  35,  16,  60,  246, 78,  84,  105, 5,   136, 33,  34,  27,  176,
        64,  134, 37,  137, 50,  62,  16,  246, 49,  48,  1,   19,  5,   53,
        5,   3,   51,  3,   37,  21,  37,  19,  2,   37,  24,  254, 167, 1,
        89,  24,  197, 25,  1,   90,  254, 166, 25,  130, 172, 44,  111, 25,
        148, 24,  2,   30,  253, 226, 24,  148, 25,  251, 145, 130, 177, 39,
        0,   111, 4,   148, 1,   200, 5,   237, 130, 193, 49,  23,  0,   124,
        64,  23,  55,  8,   12,  162, 64,  0,   209, 192, 18,  162, 6,   5,
        25,  130, 180, 47,  15,  162, 9,   21,  162, 64,  9,   184, 1,   91,
        180, 192, 3,   73,  24,  25,  130, 8,   45,  179, 33,  86,  92,  24,
        43,  43,  78,  244, 26,  77,  237, 26,  237, 93,  131, 8,   130, 12,
        32,  253, 95,  206, 6,   46,  42,  1,   23,  13,  11,  15,  99,  1,
        23,  1,   21,  99,  0,   17,  7,   130, 9,   33,  19,  5,   130, 9,
        38,  14,  10,  12,  99,  0,   22,  2,   130, 4,   38,  16,  8,   18,
        99,  1,   20,  4,   130, 4,   88,  63,  8,   37,  129, 1,   34,  38,
        53,  52,  24,  80,  51,  19,  8,   122, 28,  73,  100, 99,  74,  73,
        99,  99,  73,  30,  56,  57,  29,  30,  57,  56,  4,   148, 105, 68,
        67,  105, 105, 67,  68,  105, 86,  49,  38,  38,  48,  48,  38,  38,
        49,  0,   2,   0,   205, 0,   0,   4,   22,  5,   200, 0,   32,  0,
        40,  0,   112, 64,  25,  23,  114, 34,  135, 15,  15,  17,  205, 18,
        124, 20,  125, 21,  24,  114, 33,  135, 1,   27,  135, 28,  124, 31,
        31,  1,   184, 1,   42,  64,  13,  0,   10,  1,   34,  33,  15,  4,
        0,   23,  24,  21,  20,  28,  187, 1,   47,  0,   17,  0,   32,  1,
        199, 180, 16,  0,   38,  105, 0,   184, 1,   232, 179, 7,   73,  41,
        113, 185, 1,   191, 0,   65,  147, 5,   60,  228, 237, 16,  60,  253,
        60,  244, 60,  18,  57,  57,  18,  23,  57,  0,   63,  244, 60,  16,
        252, 237, 16,  253, 228, 47,  253, 252, 228, 60,  130, 7,   39,  49,
        48,  33,  53,  38,  38,  39,  38,  131, 191, 8,   103, 55,  54,  55,
        54,  54,  55,  53,  51,  21,  22,  23,  21,  38,  39,  17,  50,  54,
        55,  21,  6,   6,   35,  21,  3,   17,  14,  2,   21,  20,  22,  2,
        151, 98,  92,  47,  109, 112, 63,  57,  51,  70,  41,  87,  89,  99,
        133, 151, 177, 107, 46,  162, 76,  81,  160, 43,  99,  74,  114, 54,
        118, 173, 9,   45,  35,  82,  250, 144, 108, 198, 73,  66,  43,  25,
        32,  12,  185, 185, 6,   40,  166, 60,  10,  252, 184, 32,  36,  150,
        31,  27,  173, 1,   68,  3,   52,  7,   104, 176, 125, 170, 198, 0,
        1,   0,   247, 130, 237, 8,   55,  68,  5,   237, 0,   33,  0,   120,
        64,  49,  29,  8,   3,   8,   31,  28,  5,   26,  7,   85,  25,  8,
        8,   14,  0,   17,  131, 16,  100, 19,  88,  14,  5,   32,  1,   131,
        0,   10,  17,  115, 32,  74,  35,  26,  193, 23,  28,  127, 10,  5,
        193, 8,   100, 0,   73,  34,  249, 104, 139, 5,   102, 228, 5,   80,
        218, 7,   39,  253, 60,  63,  237, 252, 237, 17,  18,  98,  136, 8,
        32,  18,  123, 26,  5,   52,  18,  20,  22,  11,  13,  21,  37,  12,
        38,  20,  13,  23,  99,  0,   22,  11,  19,  99,  1,   0,   43,  97,
        153, 6,   24,  76,  168, 7,   97,  157, 13,  8,   64,  17,  51,  21,
        35,  21,  20,  6,   7,   33,  21,  247, 81,  87,  42,  179, 179, 96,
        173, 124, 104, 123, 124, 115, 54,  83,  47,  216, 216, 84,  89,  2,
        99,  173, 26,  88,  109, 93,  227, 148, 215, 164, 174, 100, 30,  167,
        49,  40,  86,  104, 254, 237, 148, 127, 174, 168, 74,  173, 0,   2,
        0,   221, 254, 178, 4,   78,  130, 213, 48,  55,  0,   67,  0,   239,
        64,  38,  86,  8,   62,  56,  46,  56,  19,  5,   31,  5,   133, 7,
        8,   32,  43,  15,  29,  94,  64,  28,  108, 10,  31,  88,  26,  3,
        1,   131, 64,  0,   189, 10,  5,   88,  53,  8,   184, 2,   60,  182,
        50,  115, 43,  28,  29,  108, 58,  130, 10,  36,  179, 43,  74,  69,
        34,  130, 7,   39,  182, 23,  115, 15,  0,   1,   115, 64,  130, 10,
        42,  179, 15,  73,  68,  69,  184, 1,   62,  179, 33,  136, 130, 250,
        44,  43,  78,  244, 77,  237, 244, 60,  16,  244, 237, 78,  16,  246,
        134, 9,   34,  0,   47,  237, 67,  105, 5,   94,  251, 8,   142, 15,
        108, 98,  5,   33,  17,  18,  90,  42,  6,   56,  60,  47,  52,  32,
        37,  20,  25,  6,   11,  48,  37,  21,  37,  32,  25,  34,  99,  0,
        6,   52,  8,   99,  1,   10,  47,  130, 4,   8,   37,  36,  20,  34,
        99,  2,   33,  24,  31,  99,  1,   7,   51,  5,   99,  0,   9,   49,
        11,  99,  1,   47,  46,  10,  11,  35,  22,  37,  99,  0,   36,  37,
        20,  19,  16,  60,  16,  60,  43,  132, 4,   24,  81,  173, 12,  52,
        19,  53,  30,  2,   51,  50,  54,  53,  52,  38,  38,  39,  39,  36,
        53,  52,  54,  54,  55,  46,  2,   130, 6,   33,  51,  50,  87,  18,
        17,  41,  7,   30,  2,   21,  20,  6,   33,  34,  38,  1,   131, 47,
        8,   165, 39,  39,  6,   21,  20,  22,  23,  221, 102, 131, 120, 58,
        121, 169, 39,  58,  133, 177, 254, 223, 31,  53,  62,  60,  52,  29,
        234, 236, 150, 195, 202, 146, 128, 160, 33,  33,  109, 136, 232, 67,
        67,  45,  104, 64,  62,  38,  230, 254, 251, 86,  146, 1,   205, 74,
        81,  139, 222, 74,  105, 124, 254, 252, 180, 44,  41,  20,  98,  82,
        42,  63,  44,  58,  75,  124, 212, 49,  100, 87,  63,  40,  67,  87,
        46,  145, 220, 47,  160, 59,  104, 81,  55,  40,  40,  43,  55,  93,
        80,  81,  112, 61,  131, 116, 37,  76,  104, 51,  129, 214, 24,  2,
        139, 99,  95,  64,  95,  59,  93,  90,  95,  67,  111, 52,  0,   0,
        1,   1,   244, 1,   182, 3,   28,  2,   222, 0,   11,  0,   48,  185,
        0,   6,   1,   217, 181, 0,   13,  71,  71,  74,  9,   184, 1,   49,
        179, 3,   73,  12,  13,  188, 1,   49,  0,   33,  1,   55,  1,   188,
        99,  42,  6,   94,  122, 6,   36,  47,  77,  237, 49,  48,  67,  241,
        11,  46,  2,   136, 61,  87,  88,  60,  60,  88,  87,  1,   182, 87,
        61,  61,  87,  131, 3,   130, 93,  58,  0,   216, 254, 216, 4,   6,
        5,   200, 0,   14,  0,   83,  64,  19,  11,  0,   167, 15,  1,   186,
        8,   12,  13,  107, 8,   2,   16,  131, 100, 42,  9,   10,  184, 2,
        62,  180, 12,  11,  93,  13,  14,  130, 8,   46,  182, 1,   0,   168,
        4,   73,  15,  16,  188, 1,   166, 0,   33,  0,   136, 24,  89,  196,
        7,   37,  77,  244, 60,  253, 60,  246, 130, 3,   96,  4,   5,   41,
        0,   63,  77,  253, 60,  16,  237, 16,  244, 60,  130, 128, 42,  17,
        38,  38,  53,  52,  54,  54,  51,  33,  17,  35,  130, 1,   42,  2,
        123, 193, 226, 72,  153, 194, 1,   139, 123, 148, 130, 118, 44,  12,
        13,  219, 182, 96,  150, 80,  249, 16,  6,   117, 249, 139, 131, 139,
        8,   58,  197, 255, 231, 4,   140, 6,   68,  0,   50,  0,   207, 64,
        51,  55,  8,   40,  11,  37,  14,  35,  17,  34,  18,  8,   46,  28,
        34,  35,  35,  91,  17,  18,  20,  17,  17,  18,  46,  87,  5,   1,
        0,   10,  26,  125, 25,  124, 28,  87,  23,  11,  17,  18,  35,  34,
        40,  11,  6,   37,  52,  130, 172, 43,  20,  43,  65,  10,  2,   59,
        0,   8,   1,   84,  0,   31,  130, 7,   8,   38,  20,  1,   108, 0,
        0,   0,   14,  2,   59,  179, 37,  114, 26,  25,  184, 1,   35,  183,
        49,  50,  91,  1,   0,   73,  51,  52,  184, 1,   108, 179, 33,  113,
        216, 24,  43,  43,  78,  244, 60,  130, 171, 40,  246, 60,  244, 237,
        16,  253, 237, 244, 237, 133, 191, 32,  18,  81,  249, 5,   36,  244,
        237, 63,  63,  237, 79,  213, 7,   32,  0,   66,  153, 5,   8,   41,
        24,  67,  121, 64,  40,  44,  48,  29,  30,  21,  22,  2,   7,   3,
        38,  29,  22,  31,  101, 1,   45,  6,   43,  101, 1,   47,  4,   49,
        101, 0,   30,  21,  28,  101, 0,   44,  7,   46,  101, 1,   48,  2,
        130, 4,   32,  43,  66,  125, 6,   32,  129, 130, 0,   34,  51,  17,
        52,  95,  107, 5,   39,  20,  6,   7,   6,   6,   21,  20,  22,  82,
        58,  5,   32,  35,  77,  115, 10,  81,  4,   5,   8,   178, 54,  54,
        53,  52,  38,  35,  34,  6,   21,  17,  197, 70,  158, 139, 183, 181,
        36,  95,  40,  37,  25,  85,  161, 173, 167, 181, 141, 141, 183, 99,
        75,  87,  38,  77,  191, 125, 26,  74,  53,  30,  92,  78,  83,  83,
        4,   127, 192, 160, 101, 160, 104, 54,  115, 126, 52,  61,  28,  29,
        53,  73,  136, 146, 169, 126, 197, 58,  169, 79,  89,  69,  46,  72,
        66,  164, 108, 111, 38,  86,  114, 81,  97,  41,  70,  81,  111, 111,
        251, 46,  0,   4,   0,   204, 2,   117, 4,   68,  5,   237, 0,   15,
        0,   31,  0,   46,  0,   55,  0,   172, 178, 44,  43,  43,  184, 1,
        73,  64,  9,   42,  41,  20,  42,  42,  41,  41,  44,  45,  184, 1,
        71,  64,  10,  48,  47,  47,  33,  42,  43,  43,  46,  46,  32,  184,
        1,   70,  181, 16,  177, 64,  0,   227, 8,   188, 1,   192, 0,   55,
        1,   71,  0,   33,  1,   70,  64,  15,  24,  177, 8,   3,   41,  38,
        51,  57,  71,  71,  74,  12,  42,  218, 51,  130, 66,  47,  183, 38,
        223, 20,  177, 64,  12,  227, 4,   184, 1,   192, 178, 47,  55,  46,
        130, 18,  43,  64,  14,  33,  32,  223, 28,  177, 4,   73,  56,  57,
        227, 121, 247, 9,   46,  246, 60,  253, 60,  60,  26,  16,  253, 26,
        253, 246, 237, 25,  228, 24,  65,  112, 5,   40,  17,  18,  57,  0,
        63,  77,  253, 254, 237, 132, 24,  34,  254, 60,  16,  130, 1,   103,
        176, 5,   24,  80,  209, 9,   33,  1,   34,  66,  72,  6,   32,  50,
        78,  173, 5,   35,  39,  50,  54,  54,  107, 198, 5,   40,  6,   6,
        21,  20,  22,  22,  39,  17,  51,  133, 25,  37,  7,   23,  35,  39,
        35,  21,  119, 244, 6,   8,   97,  35,  35,  2,   136, 118, 203, 123,
        119, 205, 120, 120, 206, 118, 123, 203, 118, 85,  163, 98,  93,  165,
        88,  88,  165, 92,  97,  163, 94,  154, 89,  72,  43,  54,  55,  124,
        113, 113, 48,  43,  52,  53,  41,  60,  47,  2,   117, 115, 208, 121,
        119, 207, 118, 118, 207, 119, 121, 208, 115, 99,  87,  165, 93,  91,
        162, 93,  93,  162, 91,  93,  165, 87,  74,  2,   18,  29,  59,  50,
        48,  78,  29,  237, 216, 216, 1,   34,  52,  41,  34,  40,  0,   0,
        3,   0,   99,  255, 219, 6,   117, 65,  79,  6,   8,   64,  56,  0,
        203, 64,  46,  79,  8,   45,  107, 64,  44,  100, 14,  47,  116, 41,
        184, 24,  116, 8,   229, 0,   64,  56,  107, 80,  32,  108, 14,  54,
        116, 34,  184, 16,  116, 0,   11,  58,  71,  71,  74,  12,  44,  32,
        184, 20,  116, 12,  229, 4,   51,  184, 2,   63,  64,  13,  38,  184,
        28,  116, 4,   73,  57,  58,  229, 71,  241, 7,   33,  77,  237, 66,
        139, 5,   24,  72,  7,   8,   131, 16,  69,  58,  7,   40,  77,  26,
        89,  252, 26,  237, 26,  16,  253, 130, 36,  106, 213, 9,   8,   35,
        49,  48,  67,  121, 64,  58,  1,   31,  14,  38,  18,  37,  30,  38,
        2,   37,  10,  37,  22,  38,  26,  37,  6,   38,  17,  15,  20,  134,
        1,   31,  1,   28,  134, 0,   23,  9,   130, 9,   33,  25,  7,   130,
        9,   38,  19,  13,  16,  134, 0,   29,  3,   130, 4,   38,  21,  11,
        24,  134, 1,   27,  5,   130, 4,   72,  32,  8,   131, 8,   131, 3,
        122, 9,   18,  54,  36,  18,  53,  52,  2,   36,  35,  34,  4,   2,
        21,  20,  18,  4,   1,   6,   35,  34,  38,  38,  53,  52,  18,  92,
        78,  8,   32,  6,   107, 15,  6,   8,   108, 108, 201, 254, 155, 219,
        210, 1,   104, 207, 207, 1,   105, 209, 219, 254, 155, 201, 191, 1,
        38,  181, 176, 254, 215, 193, 192, 254, 215, 177, 186, 1,   35,  1,
        217, 141, 114, 120, 204, 106, 213, 220, 93,  133, 26,  136, 108, 80,
        141, 72,  167, 144, 113, 113, 37,  198, 1,   108, 215, 211, 1,   103,
        207, 207, 254, 153, 211, 215, 254, 148, 198, 111, 176, 1,   70,  164,
        162, 1,   68,  180, 180, 254, 188, 162, 167, 254, 183, 170, 1,   15,
        49,  112, 204, 123, 173, 1,   14,  26,  5,   123, 56,  82,  162, 97,
        152, 180, 62,  0,   0,   2,   130, 2,   8,   46,  228, 6,   6,   5,
        200, 0,   7,   0,   20,  0,   202, 64,  84,  37,  10,  41,  12,  52,
        10,  57,  12,  4,   20,  10,  24,  12,  23,  18,  3,   10,  19,  18,
        18,  138, 11,  10,  20,  11,  11,  10,  12,  16,  17,  17,  236, 11,
        12,  130, 10,  8,   72,  12,  11,  166, 17,  18,  189, 15,  8,   8,
        0,   186, 3,   6,   1,   107, 3,   16,  108, 19,  166, 9,   12,  9,
        3,   2,   8,   12,  27,  12,  2,   12,  16,  7,   10,  20,  10,  2,
        10,  19,  17,  18,  11,  22,  71,  71,  74,  14,  184, 2,   61,  64,
        9,   64,  16,  15,  224, 11,  149, 128, 19,  20,  186, 2,   62,  0,
        8,   1,   7,   179, 5,   149, 6,   7,   130, 25,  45,  182, 1,   0,
        149, 2,   73,  21,  22,  184, 1,   112, 179, 33,  97,  73,  178, 5,
        45,  77,  244, 60,  253, 60,  244, 246, 253, 60,  26,  252, 252, 60,
        26,  69,  140, 5,   37,  17,  51,  51,  17,  51,  93,  130, 2,   42,
        0,   63,  60,  60,  77,  16,  253, 228, 16,  253, 60,  131, 2,   34,
        60,  244, 60,  88,  225, 14,  8,   82,  49,  48,  1,   93,  93,  19,
        17,  35,  53,  33,  21,  35,  17,  33,  17,  51,  19,  19,  51,  17,
        35,  17,  3,   35,  3,   17,  247, 247, 2,   154, 247, 1,   115, 233,
        156, 150, 213, 173, 163, 118, 163, 2,   228, 2,   105, 123, 123, 253,
        151, 2,   228, 254, 85,  1,   171, 253, 28,  2,   25,  254, 37,  1,
        176, 254, 18,  0,   1,   1,   200, 5,   3,   3,   132, 6,   43,  0,
        3,   0,   44,  64,  17,  1,   3,   112, 11,  5,   24,  84,  194, 8,
        45,  186, 1,   54,  1,   22,  0,   24,  43,  43,  78,  228, 16,  230,
        0,   112, 3,   6,   33,  57,  57,  130, 120, 33,  19,  51,  130, 59,
        36,  222, 222, 254, 241, 5,   112, 3,   6,   33,  1,   89,  130, 73,
        34,  145, 5,   176, 130, 73,  8,   34,  7,   0,   55,  64,  14,  5,
        1,   131, 128, 4,   0,   34,  4,   131, 7,   3,   131, 64,  7,   184,
        1,   98,  180, 192, 0,   73,  8,   9,   188, 1,   98,  0,   33,  1,
        52,  134, 85,  74,  179, 7,   130, 234, 34,  26,  253, 60,  130, 86,
        35,  53,  51,  21,  51,  130, 3,   39,  1,   89,  173, 222, 173, 5,
        3,   173, 130, 0,   8,   109, 0,   1,   0,   105, 0,   148, 4,   167,
        4,   12,  0,   19,  0,   132, 64,  73,  2,   1,   14,  18,  7,   3,
        4,   11,  18,  7,   6,   5,   10,  18,  7,   9,   5,   10,  17,  8,
        12,  4,   11,  17,  8,   13,  1,   14,  17,  8,   16,  0,   15,  17,
        8,   19,  0,   15,  18,  7,   7,   18,  18,  85,  17,  8,   20,  17,
        17,  8,   8,   7,   224, 10,  5,   85,  11,  4,   148, 14,  1,   85,
        15,  0,   224, 18,  17,  14,  10,  7,   8,   224, 10,  184, 1,   175,
        183, 4,   0,   17,  18,  193, 0,   221, 20,  16,  246, 25,  244, 50,
        16,  60,  24,  253, 132, 6,   35,  0,   24,  47,  60,  70,  165, 7,
        41,  244, 60,  135, 5,   46,  43,  125, 16,  196, 15,  134, 0,   38,
        49,  48,  19,  53,  33,  55,  33,  130, 3,   36,  51,  7,   33,  21,
        33,  132, 3,   63,  35,  55,  105, 1,   153, 112, 253, 247, 2,   93,
        114, 168, 112, 1,   55,  254, 117, 112, 1,   251, 253, 178, 109, 174,
        112, 1,   89,  148, 198, 148, 197, 197, 132, 4,   44,  0,   0,   2,
        0,   18,  0,   0,   7,   16,  5,   200, 0,   15,  130, 9,   8,   43,
        155, 64,  12,  14,  16,  18,  18,  15,  14,  16,  15,  1,   18,  15,
        15,  184, 2,   53,  64,  14,  0,   1,   20,  0,   0,   1,   18,  4,
        5,   13,  14,  89,  17,  16,  184, 1,   201, 64,  46,  7,   5,   6,
        89,  8,   24,  67,  219, 12,  63,  15,  12,  12,  0,   8,   1,   15,
        0,   4,   9,   90,  12,  12,  18,  18,  19,  20,  7,   141, 3,   155,
        10,  74,  20,  0,   73,  19,  20,  184, 2,   38,  179, 117, 181, 5,
        54,  78,  228, 16,  246, 77,  244, 228, 17,  18,  57,  47,  60,  16,
        253, 60,  18,  57,  57,  0,   63,  60,  16,  60,  24,  67,  239, 10,
        35,  253, 60,  16,  246, 130, 14,  130, 33,  131, 226, 46,  135, 125,
        196, 1,   17,  57,  57,  135, 16,  196, 60,  49,  48,  51,  1,   130,
        218, 32,  17,  136, 3,   8,   45,  3,   1,   33,  17,  18,  3,   93,
        3,   115, 253, 150, 2,   6,   253, 250, 2,   152, 252, 150, 254, 29,
        241, 1,   74,  1,   138, 5,   200, 157, 254, 37,  155, 253, 232, 157,
        1,   158, 254, 98,  2,   57,  2,   159, 0,   0,   3,   126, 151, 6,
        8,   37,  238, 0,   27,  0,   35,  0,   47,  0,   230, 64,  18,  41,
        8,   12,  29,  28,  1,   0,   0,   13,  15,  47,  36,  26,  27,  27,
        14,  0,   27,  184, 255, 218, 64,  9,   2,   13,  14,  38,  130, 3,
        40,  27,  27,  184, 2,   54,  64,  70,  0,   13,  130, 251, 8,   61,
        13,  26,  22,  29,  47,  15,  28,  36,  1,   6,   40,  12,  31,  119,
        8,   13,  14,  175, 8,   3,   27,  0,   0,   40,  119, 22,  9,   1,
        5,   47,  12,  36,  29,  26,  28,  6,   34,  15,  43,  121, 19,  64,
        13,  142, 14,  32,  14,  19,  74,  49,  34,  121, 5,   64,  27,  142,
        0,   32,  0,   5,   73,  48,  85,  18,  6,   42,  60,  26,  25,  77,
        16,  237, 26,  24,  16,  237, 78,  130, 225, 135, 12,  48,  253, 57,
        17,  23,  57,  18,  57,  0,   63,  237, 60,  16,  60,  63,  244, 60,
        16,  134, 16,  135, 252, 42,  23,  56,  23,  56,  135, 14,  16,  60,
        60,  196, 60,  131, 6,   34,  196, 60,  60,  124, 59,  5,   60,  28,
        41,  42,  32,  33,  20,  21,  6,   7,   41,  21,  43,  110, 1,   32,
        7,   34,  110, 0,   42,  20,  40,  110, 0,   33,  6,   31,  110, 1,
        97,  65,  5,   32,  129, 130, 0,   49,  23,  55,  46,  2,   53,  16,
        0,   33,  50,  22,  22,  23,  55,  51,  7,   30,  2,   21,  130, 12,
        52,  34,  38,  38,  39,  7,   19,  1,   38,  35,  34,  2,   17,  20,
        19,  30,  2,   51,  50,  18,  17,  52,  130, 19,  8,   68,  104, 173,
        72,  64,  37,  1,   100, 1,   95,  74,  129, 115, 77,  88,  194, 173,
        70,  65,  38,  254, 144, 254, 180, 70,  126, 120, 86,  88,  108, 2,
        177, 129, 170, 224, 245, 171, 49,  87,  110, 56,  216, 249, 16,  27,
        35,  37,  226, 92,  155, 200, 103, 1,   73,  1,   194, 22,  44,  48,
        113, 225, 84,  150, 203, 106, 254, 163, 254, 75,  21,  130, 12,  46,
        1,   143, 3,   126, 104, 254, 178, 254, 234, 217, 254, 206, 43,  39,
        23,  130, 35,  8,   77,  35,  72,  138, 100, 70,  0,   3,   0,   120,
        0,   173, 6,   72,  3,   244, 0,   26,  0,   37,  0,   54,  0,   176,
        64,  43,  73,  8,   13,  30,  0,   27,  36,  51,  88,  16,  16,  30,
        88,  64,  10,  133, 32,  38,  234, 27,  168, 3,   44,  88,  23,  23,
        36,  88,  3,   38,  27,  13,  0,   4,   47,  33,  56,  71,  71,  74,
        20,  88,  47,  184, 2,   21,  183, 33,  88,  6,   73,  55,  112, 90,
        236, 5,   34,  77,  253, 246, 68,  154, 6,   51,  18,  23,  57,  0,
        47,  77,  237, 60,  16,  237, 25,  16,  244, 24,  230, 26,  25,  253,
        26,  24,  131, 13,  35,  17,  18,  57,  17,  76,  161, 5,   58,  54,
        45,  50,  31,  35,  17,  22,  4,   9,   8,   38,  18,  37,  49,  38,
        35,  4,   33,  99,  0,   45,  22,  47,  99,  1,   31,  9,   130, 9,
        33,  50,  17,  130, 9,   51,  34,  5,   36,  99,  0,   46,  21,  44,
        99,  0,   32,  7,   30,  99,  1,   48,  19,  51,  99,  1,   126, 179,
        13,  38,  129, 129, 1,   6,   6,   35,  34,  74,  12,  5,   36,  50,
        22,  23,  54,  54,  71,  175, 6,   130, 19,  36,  39,  39,  38,  38,
        35,  115, 118, 6,   37,  1,   22,  23,  23,  22,  22,  75,  166, 6,
        130, 20,  8,   145, 6,   3,   84,  111, 172, 98,  141, 210, 100, 161,
        99,  108, 177, 115, 121, 188, 72,  86,  168, 93,  207, 157, 90,  147,
        111, 124, 58,  175, 80,  78,  113, 128, 85,  141, 1,   77,  14,  3,
        73,  36,  125, 62,  76,  116, 51,  107, 51,  49,  102, 78,  1,   223,
        196, 110, 230, 168, 117, 211, 113, 137, 178, 215, 100, 107, 179, 117,
        181, 255, 88,  155, 179, 91,  171, 150, 132, 128, 126, 1,   16,  18,
        5,   102, 50,  98,  165, 113, 76,  123, 67,  63,  99,  0,   0,   2,
        0,   105, 0,   0,   4,   167, 4,   160, 0,   3,   0,   15,  0,   59,
        64,  32,  10,  182, 12,  7,   85,  13,  6,   182, 15,  93,  2,   1,
        85,  3,   0,   10,  2,   13,  194, 10,  15,  85,  9,   4,   194, 7,
        0,   73,  16,  86,  92,  24,  43,  78,  16,  244, 90,  226, 6,   39,
        60,  0,   63,  60,  253, 60,  246, 244, 130, 4,   57,  228, 49,  48,
        51,  53,  33,  21,  1,   17,  33,  53,  33,  17,  51,  17,  33,  21,
        33,  17,  105, 4,   62,  253, 151, 254, 43,  24,  90,  197, 7,   44,
        148, 1,   40,  1,   114, 148, 1,   114, 254, 142, 148, 254, 142, 136,
        119, 33,  5,   3,   130, 119, 39,  9,   0,   90,  64,  41,  5,   6,
        6,   130, 101, 38,  20,  9,   6,   7,   9,   4,   7,   131, 11,  32,
        8,   130, 11,  63,  5,   9,   8,   5,   4,   150, 6,   9,   150, 7,
        8,   188, 2,   85,  1,   10,  5,   7,   7,   0,   184, 2,   93,  180,
        10,  9,   1,   221, 10,  16,  246, 60,  130, 138, 32,  16,  130, 133,
        48,  253, 246, 50,  25,  244, 60,  244, 50,  135, 8,   46,  24,  43,
        4,   125, 16,  196, 136, 8,   34,  49,  48,  33,  131, 145, 36,  21,
        1,   1,   21,  1,   24,  79,  42,  9,   44,  251, 194, 148, 4,   111,
        165, 254, 134, 254, 135, 166, 2,   31,  143, 139, 34,  95,  64,  42,
        151, 139, 33,  7,   8,   131, 139, 33,  5,   4,   132, 139, 44,  6,
        209, 9,   9,   0,   184, 1,   175, 182, 1,   8,   4,   4,   134, 142,
        39,  60,  16,  253, 60,  16,  237, 0,   63,  159, 144, 36,  37,  53,
        1,   1,   53,  134, 144, 8,   50,  251, 194, 2,   242, 253, 14,  4,
        62,  148, 49,  166, 1,   121, 1,   122, 165, 253, 225, 0,   0,   1,
        0,   80,  0,   0,   4,   216, 5,   200, 0,   22,  0,   183, 178, 10,
        11,  11,  184, 2,   92,  64,  11,  8,   9,   20,  8,   11,  12,  8,
        9,   12,  131, 15,  8,   69,  54,  64,  20,  14,  13,  20,  14,  11,
        10,  14,  13,  13,  12,  12,  10,  10,  9,   2,   14,  8,   137, 16,
        6,   184, 2,   95,  180, 19,  3,   89,  20,  2,   184, 1,   220, 64,
        10,  0,   10,  11,  22,  0,   24,  71,  71,  74,  13,  188, 2,   70,
        0,   16,  1,   48,  0,   20,  1,   48,  178, 14,  18,  22,  184, 2,
        55,  178, 8,   4,   6,   188, 130, 17,  32,  2,   130, 3,   8,   49,
        0,   2,   70,  179, 9,   73,  23,  24,  186, 2,   81,  0,   33,  1,
        69,  177, 233, 24,  43,  43,  25,  78,  244, 77,  244, 24,  228, 228,
        60,  60,  253, 60,  60,  228, 228, 25,  244, 78,  69,  101, 68,  230,
        17,  18,  57,  0,   24,  63,  77,  244, 130, 20,  34,  124, 246, 24,
        130, 5,   95,  146, 5,   35,  135, 8,   46,  43,  122, 69,  16,  33,
        33,  53,  132, 1,   34,  1,   51,  1,   130, 2,   33,  33,  21,  132,
        1,   38,  17,  2,   31,  254, 216, 1,   40,  132, 3,   40,  49,  233,
        1,   104, 1,   132, 179, 254, 12,  134, 15,  45,  216, 1,   89,  148,
        99,  148, 2,   228, 253, 193, 2,   63,  253, 28,  130, 10,  8,   58,
        254, 167, 0,   1,   0,   197, 254, 117, 4,   61,  4,   62,  0,   21,
        0,   81,  64,  12,  9,   14,  10,  2,   6,   13,  10,  6,   126, 17,
        64,  3,   184, 1,   144, 64,  29,  20,  124, 67,  17,  11,  0,   14,
        10,  13,  91,  12,  12,  11,  74,  23,  2,   21,  91,  0,   0,   1,
        73,  22,  23,  100, 136, 6,   34,  78,  244, 60,  94,  5,   5,   32,
        246, 132, 7,   45,  0,   63,  63,  26,  252, 228, 26,  16,  237, 63,
        63,  60,  57,  57,  94,  17,  5,   8,   124, 20,  22,  51,  50,  54,
        55,  17,  51,  17,  35,  53,  6,   6,   35,  34,  38,  39,  17,  197,
        197, 110, 72,  83,  156, 73,  197, 197, 70,  158, 88,  51,  89,  38,
        254, 117, 5,   201, 253, 52,  123, 87,  111, 111, 2,   192, 251, 194,
        204, 114, 115, 25,  25,  254, 92,  0,   0,   2,   0,   203, 255, 231,
        4,   75,  6,   68,  0,   29,  0,   42,  0,   209, 64,  30,  36,  64,
        19,  33,  77,  8,   30,  40,  22,  19,  33,  22,  8,   30,  19,  88,
        33,  33,  12,  0,   167, 26,  132, 4,   1,   40,  88,  12,  11,  30,
        184, 2,   63,  179, 8,   74,  44,  37,  184, 2,   60,  181, 0,   108,
        15,  73,  43,  44,  184, 1,   62,  179, 94,  171, 7,   34,  77,  228,
        237, 107, 38,  9,   39,  228, 18,  57,  47,  237, 1,   17,  18,  105,
        8,   6,   8,   60,  49,  48,  67,  121, 64,  56,  34,  42,  24,  25,
        5,   18,  10,  38,  6,   37,  35,  37,  17,  38,  41,  11,  30,  183,
        1,   42,  30,  39,  13,  37,  99,  0,   25,  5,   23,  183, 1,   34,
        18,  37,  183, 0,   42,  9,   40,  183, 0,   38,  14,  40,  99,  0,
        24,  7,   26,  183, 1,   36,  16,  33,  183, 68,  131, 8,   34,  16,
        60,  43,  131, 0,   54,  129, 129, 129, 0,   73,  84,  121, 182, 31,
        32,  20,  21,  31,  21,  33,  184, 1,   203, 179, 5,   32,  20,  30,
        130, 7,   37,  180, 1,   31,  30,  21,  22,  109, 35,  9,   49,  19,
        62,  2,   51,  50,  22,  18,  21,  16,  2,   4,   35,  34,  38,  53,
        52,  18,  36,  130, 14,  32,  23,  77,  154, 5,   38,  6,   1,   38,
        38,  35,  34,  6,   74,  242, 5,   8,   114, 18,  247, 52,  126, 152,
        84,  131, 211, 96,  151, 254, 229, 161, 146, 155, 170, 1,   0,   124,
        44,  82,  67,  205, 140, 59,  121, 114, 2,   127, 49,  102, 53,  85,
        161, 107, 95,  85,  122, 228, 5,   18,  107, 126, 73,  164, 254, 183,
        201, 254, 242, 254, 91,  244, 202, 146, 168, 1,   85,  188, 26,  43,
        30,  200, 254, 30,  56,  253, 193, 52,  51,  133, 236, 112, 115, 154,
        1,   96,  0,   0,   1,   0,   50,  0,   0,   5,   4,   5,   200, 0,
        11,  0,   120, 178, 7,   8,   8,   184, 2,   92,  64,  9,   2,   3,
        20,  2,   2,   3,   8,   9,   9,   184, 2,   54,  64,  15,  124, 101,
        6,   8,   48,  2,   9,   3,   7,   89,  4,   2,   8,   184, 1,   196,
        64,  13,  2,   2,   7,   9,   146, 0,   8,   6,   155, 10,  74,  13,
        8,   186, 1,   87,  0,   2,   1,   243, 180, 3,   155, 1,   155, 12,
        184, 1,   69,  177, 153, 24,  43,  16,  244, 228, 81,  143, 5,   61,
        228, 0,   63,  253, 25,  17,  57,  47,  24,  237, 63,  253, 57,  17,
        18,  57,  135, 5,   46,  43,  14,  125, 16,  196, 135, 14,  46,  24,
        43,  5,   130, 8,   8,   40,  49,  48,  51,  53,  1,   1,   53,  33,
        21,  33,  1,   1,   33,  21,  50,  2,   108, 253, 199, 4,   109, 252,
        178, 2,   35,  253, 121, 3,   228, 187, 2,   17,  2,   95,  157, 157,
        253, 184, 253, 216, 187, 131, 173, 32,  117, 130, 173, 32,  158, 132,
        173, 8,   49,  68,  64,  30,  9,   10,  5,   2,   85,  4,   3,   2,
        11,  8,   10,  9,   8,   127, 6,   7,   156, 4,   74,  13,  10,  11,
        127, 1,   0,   156, 2,   73,  12,  13,  184, 2,   26,  179, 33,  112,
        152, 24,  43,  43,  78,  244, 77,  244, 60,  253, 60,  98,  8,   5,
        37,  253, 60,  0,   63,  60,  63,  130, 14,  33,  57,  57,  130, 121,
        74,  249, 6,   8,   96,  35,  17,  33,  17,  216, 99,  5,   41,  99,
        209, 253, 63,  5,   43,  157, 157, 250, 213, 5,   43,  250, 213, 0,
        1,   0,   63,  0,   0,   6,   38,  4,   62,  0,   23,  0,   106, 64,
        26,  12,  21,  21,  22,  22,  1,   125, 10,  9,   5,   244, 64,  6,
        201, 96,  9,   6,   15,  23,  16,  10,  25,  71,  71,  74,  10,  184,
        1,   47,  64,  10,  13,  15,  244, 64,  16,  201, 96,  20,  91,  13,
        184, 1,   169, 178, 23,  91,  0,   184, 1,   143, 179, 5,   73,  24,
        25,  184, 1,   181, 100, 123, 8,   41,  77,  246, 237, 253, 253, 26,
        252, 26,  237, 16,  67,  226, 5,   130, 140, 35,  60,  63,  26,  77,
        131, 16,  130, 147, 57,  16,  60,  16,  60,  49,  48,  33,  17,  35,
        34,  6,   7,   53,  54,  54,  51,  33,  21,  33,  17,  20,  23,  35,
        46,  2,   53,  130, 160, 8,   76,  1,   152, 40,  42,  159, 104, 67,
        136, 114, 4,   170, 254, 222, 100, 219, 31,  28,  19,  254, 31,  3,
        145, 33,  82,  215, 42,  31,  173, 253, 150, 177, 118, 43,  77,  134,
        44,  2,   103, 252, 111, 0,   0,   1,   1,   12,  254, 216, 4,   4,
        5,   237, 0,   42,  0,   90,  64,  25,  16,  107, 21,  107, 9,   167,
        43,  38,  107, 0,   107, 32,  3,   21,  19,  12,  0,   35,  41,  44,
        130, 181, 57,  5,   35,  184, 2,   60,  64,  20,  41,  221, 4,   115,
        5,   5,   6,   106, 27,  12,  131, 20,  221, 28,  27,  73,  43,  211,
        160, 70,  119, 6,   39,  246, 237, 16,  253, 60,  16,  244, 246, 80,
        167, 6,   34,  17,  18,  57,  79,  57,  6,   130, 188, 41,  253, 237,
        49,  48,  1,   39,  34,  6,   21,  19,  121, 146, 5,   32,  53,  80,
        111, 7,   44,  7,   23,  50,  54,  53,  39,  3,   53,  52,  18,  18,
        51,  50,  90,  202, 5,   8,   81,  38,  53,  52,  3,   82,  12,  47,
        54,  9,   162, 160, 81,  75,  26,  44,  26,  45,  43,  4,   14,  35,
        63,  3,   8,   35,  145, 146, 77,  76,  50,  49,  41,  43,  5,   116,
        1,   141, 224, 254, 129, 85,  254, 3,   254, 161, 88,  37,  25,  46,
        31,  58,  25,  7,   16,  2,   111, 224, 118, 1,   40,  85,  212, 1,
        123, 1,   13,  84,  41,  30,  75,  55,  29,  7,   0,   0,   2,   0,
        72,  2,   204, 3,   56,  130, 217, 8,   61,  35,  0,   46,  0,   128,
        64,  40,  41,  8,   1,   37,  107, 11,  11,  3,   15,  144, 20,  17,
        94,  18,  189, 20,  23,  44,  144, 3,   36,  107, 64,  0,   156, 16,
        3,   115, 34,  31,  107, 32,  100, 29,  94,  34,  21,  0,   12,  36,
        184, 2,   61,  64,  10,  24,  25,  221, 31,  74,  48,  18,  17,  108,
        41,  130, 249, 43,  179, 6,   73,  47,  48,  184, 1,   165, 179, 33,
        109, 217, 66,  36,  5,   124, 43,  6,   40,  246, 60,  253, 60,  57,
        0,   63,  237, 244, 130, 230, 50,  124, 75,  82,  88,  176, 77,  26,
        27,  176, 128, 26,  89,  252, 24,  26,  237, 16,  237, 63,  130, 22,
        35,  237, 18,  57,  47,  131, 255, 32,  93,  72,  99,  10,  35,  51,
        53,  52,  38,  123, 227, 16,  37,  21,  6,   35,  34,  39,  53,  72,
        107, 7,   8,   78,  54,  2,   56,  51,  126, 76,  112, 131, 93,  197,
        138, 58,  90,  73,  132, 144, 147, 161, 110, 122, 72,  15,  39,  23,
        5,   11,  64,  44,  104, 54,  49,  132, 132, 64,  44,  42,  117, 3,
        54,  53,  53,  130, 89,  77,  136, 67,  37,  73,  69,  83,  147, 59,
        50,  98,  111, 254, 226, 53,  34,  21,  1,   117, 20,  207, 157, 81,
        69,  49,  55,  47,  0,   2,   0,   62,  2,   204, 3,   34,  5,   87,
        225, 5,   52,  67,  64,  18,  121, 13,  120, 17,  119, 19,  118, 23,
        4,   18,  144, 6,   23,  12,  107, 0,   21,  15,  131, 222, 35,  9,
        74,  25,  21,  131, 7,   45,  3,   73,  24,  25,  188, 1,   165, 0,
        33,  0,   109, 1,   65,  0,   134, 233, 68,  255, 9,   35,  49,  48,
        1,   93,  87,  168, 24,  36,  176, 193, 177, 177, 193, 131, 3,   44,
        78,  107, 106, 79,  78,  107, 108, 2,   204, 242, 158, 159, 242, 131,
        3,   8,   63,  123, 132, 146, 144, 133, 133, 144, 146, 132, 0,   0,
        1,   0,   64,  0,   0,   6,   4,   5,   237, 0,   37,  0,   130, 64,
        24,  40,  8,   31,  119, 12,  3,   4,   0,   0,   24,  20,  89,  1,
        23,  8,   39,  71,  71,  74,  21,  155, 16,  121, 28,  184, 1,   141,
        178, 20,  142, 24,  184, 1,   214, 178, 4,   142, 1,   130, 13,  40,
        182, 34,  121, 8,   155, 2,   73,  38,  184, 71,  3,   5,   41,  78,
        16,  244, 77,  244, 253, 244, 237, 254, 237, 130, 5,   46,  78,  69,
        101, 68,  230, 0,   63,  60,  77,  253, 60,  60,  16,  60,  63,  112,
        204, 5,   57,  28,  29,  33,  9,   15,  14,  37,  10,  38,  30,  13,
        28,  110, 1,   32,  11,  34,  110, 0,   29,  15,  31,  110, 1,   33,
        9,   130, 4,   82,  252, 8,   37,  37,  21,  33,  53,  33,  46,  24,
        74,  89,  10,  35,  6,   6,   7,   33,  130, 19,  32,  62,  130, 18,
        58,  2,   35,  34,  2,   21,  20,  22,  22,  2,   153, 253, 167, 1,
        154, 168, 136, 53,  172, 1,   42,  215, 215, 1,   41,  173, 53,  137,
        130, 15,  8,   141, 253, 166, 142, 122, 62,  234, 228, 228, 234, 56,
        126, 157, 157, 157, 185, 216, 184, 105, 179, 1,   58,  177, 177, 254,
        198, 179, 105, 183, 217, 185, 157, 157, 156, 205, 213, 110, 213, 1,
        50,  254, 206, 213, 111, 199, 216, 0,   0,   3,   0,   103, 255, 231,
        6,   62,  4,   86,  0,   44,  0,   55,  0,   62,  1,   18,  178, 76,
        8,   33,  184, 255, 253, 64,  18,  0,   45,  31,  53,  23,  20,  60,
        23,  11,  0,   3,   46,  57,  56,  123, 31,  32,  10,  184, 1,   6,
        64,  32,  46,  114, 32,  32,  3,   20,  60,  87,  27,  27,  15,  87,
        20,  17,  125, 64,  18,  201, 12,  20,  7,   39,  135, 64,  40,  124,
        10,  36,  87,  42,  42,  0,   184, 1,   82,  64,  17,  53,  126, 3,
        11,  45,  10,  46,  147, 56,  130, 44,  58,  6,   57,  244, 40,  31,
        187, 1,   35,  0,   64,  0,   50,  2,   59,  182, 18,  203, 70,  6,
        73,  63,  64,  184, 1,   181, 179, 33,  122, 129, 6,   53,  118, 77,
        228, 24,  237, 16,  246, 60,  237, 18,  57,  47,  228, 253, 60,  60,
        0,   63,  237, 228, 60,  16,  86,  235, 8,   37,  80,  26,  89,  252,
        26,  237, 24,  97,  19,  15,  75,  110, 6,   43,  47,  244, 237, 16,
        60,  253, 60,  1,   18,  23,  57,  0,   67,  219, 5,   59,  57,  56,
        49,  48,  67,  121, 64,  54,  48,  55,  21,  22,  1,   14,  13,  38,
        54,  2,   45,  95,  1,   1,   0,   55,  45,  52,  4,   50,  127, 13,
        8,   55,  48,  8,   50,  95,  0,   55,  1,   53,  95,  2,   51,  5,
        53,  101, 0,   12,  22,  15,  101, 3,   49,  7,   47,  95,  70,  252,
        6,   34,  16,  60,  43,  87,  11,  6,   127, 3,   25,  41,  23,  62,
        2,   51,  50,  22,  22,  21,  33,  30,  130, 7,   34,  54,  55,  21,
        72,  9,   5,   127, 9,   8,   122, 223, 7,   35,  41,  95,  196, 102,
        127, 16,  12,  8,   42,  177, 96,  152, 56,  49,  85,  110, 59,  146,
        194, 78,  253, 96,  10,  83,  152, 120, 60,  128, 119, 193, 166, 135,
        224, 152, 63,  155, 210, 103, 69,  62,  128, 1,   14,  1,   200, 126,
        88,  104, 126, 193, 109, 109, 127, 34,  10,  48,  60,  61,  49,  47,
        25,  150, 250, 198, 142, 151, 96,  21,  50,  157, 62,  112, 158, 127,
        38,  7,   8,   68,  1,   201, 180, 135, 159, 0,   0,   3,   0,   111,
        255, 231, 4,   123, 4,   86,  0,   21,  0,   29,  0,   37,  0,   232,
        64,  18,  47,  8,   9,   23,  22,  1,   0,   0,   10,  12,  30,  31,
        20,  21,  21,  11,  0,   21,  184, 255, 160, 64,  82,  2,   10,  11,
        96,  2,   20,  18,  31,  30,  23,  22,  12,  1,   6,   33,  9,   7,
        25,  1,   3,   131, 12,  46,  20,  9,   6,   28,  12,  14,  36,  10,
        11,  21,  21,  123, 0,   10,  20,  130, 55,  8,   35,  25,  87,  7,
        11,  10,  10,  7,   7,   33,  87,  18,  21,  0,   0,   18,  11,  10,
        147, 11,  11,  36,  105, 14,  74,  39,  21,  147, 0,   0,   28,  105,
        3,   73,  38,  39,  228, 115, 181, 9,   33,  60,  16,  72,  144, 5,
        130, 7,   35,  0,   63,  60,  16,  130, 6,   133, 5,   78,  87,  7,
        40,  17,  18,  57,  18,  23,  57,  17,  57,  0,   135, 8,   40,  1,
        23,  56,  23,  56,  7,   14,  16,  60,  130, 0,   134, 6,   78,  105,
        5,   63,  32,  34,  35,  26,  27,  15,  17,  4,   6,   16,  38,  5,
        38,  34,  17,  36,  95,  1,   26,  6,   28,  95,  0,   35,  15,  33,
        95,  0,   27,  4,   25,  95,  78,  109, 6,   33,  43,  43,  78,  111,
        5,   43,  38,  53,  52,  18,  54,  51,  50,  23,  55,  51,  7,   22,
        114, 192, 5,   78,  107, 6,   52,  6,   21,  20,  1,   1,   22,  51,
        50,  54,  53,  52,  111, 127, 127, 122, 233, 162, 152, 130, 52,  185,
        130, 8,   32,  232, 130, 8,   8,   122, 53,  65,  1,   188, 78,  94,
        147, 165, 2,   64,  254, 68,  79,  93,  148, 165, 25,  169, 171, 227,
        158, 1,   16,  138, 70,  70,  169, 170, 228, 158, 254, 241, 139, 71,
        71,  1,   78,  2,   82,  59,  216, 200, 122, 1,   97,  253, 174, 60,
        217, 200, 121, 0,   2,   0,   37,  254, 80,  3,   59,  4,   62,  0,
        3,   0,   33,  0,   124, 64,  53,  22,  8,   29,  26,  10,  7,   4,
        33,  15,  26,  25,  10,  29,  3,   12,  7,   1,   32,  33,  148, 2,
        106, 3,   6,   17,  131, 18,  108, 15,  88,  20,  15,  18,  193, 5,
        3,   32,  173, 1,   5,   74,  35,  12,  173, 23,  100, 25,  73,  34,
        35,  185, 33,  109, 185, 88,  1,   9,   49,  237, 78,  16,  246, 60,
        77,  253, 60,  16,  228, 0,   63,  237, 252, 237, 63,  253, 230, 118,
        234, 7,   33,  18,  57,  87,  60,  6,   34,  67,  121, 64,  24,  90,
        72,  8,   39,  0,   13,  22,  15,  99,  0,   43,  1,   130, 247, 40,
        1,   21,  35,  53,  19,  21,  20,  6,   7,   87,  24,  5,   33,  51,
        50,  66,  127, 5,   8,   64,  53,  52,  54,  54,  55,  55,  62,  2,
        53,  53,  2,   102, 198, 198, 58,  89,  64,  98,  58,  135, 122, 149,
        174, 186, 162, 200, 242, 29,  52,  86,  61,  73,  54,  24,  4,   62,
        198, 198, 254, 117, 55,  110, 146, 103, 72,  112, 122, 62,  85,  108,
        75,  167, 56,  182, 156, 53,  94,  80,  81,  59,  69,  80,  87,  71,
        111, 130, 227, 36,  225, 254, 117, 1,   166, 132, 227, 42,  9,   0,
        83,  185, 0,   7,   1,   175, 64,  34,  9,   130, 214, 60,  64,  3,
        6,   4,   100, 6,   26,  11,  71,  71,  74,  0,   0,   6,   127, 64,
        9,   100, 7,   26,  7,   2,   73,  10,  11,  242, 33,  136, 196, 24,
        114, 43,  16,  32,  253, 85,  139, 6,   85,  113, 6,   42,  77,  16,
        236, 0,   63,  26,  253, 246, 253, 49,  48,  132, 186, 43,  19,  17,
        35,  17,  19,  1,   166, 197, 173, 24,  197, 25,  133, 146, 8,   57,
        252, 234, 254, 216, 1,   40,  3,   22,  0,   1,   0,   105, 1,   89,
        4,   167, 3,   71,  0,   5,   0,   33,  64,  11,  2,   85,  5,   139,
        4,   5,   4,   85,  3,   3,   2,   184, 1,   175, 178, 0,   221, 6,
        16,  246, 253, 60,  16,  253, 60,  0,   47,  244, 237, 49,  48,  19,
        53,  33,  130, 74,  42,  105, 4,   62,  148, 2,   179, 148, 254, 18,
        1,   90,  130, 63,  48,  116, 254, 216, 5,   227, 6,   80,  0,   8,
        0,   161, 64,  44,  8,   0,   0,   98,  73,  252, 5,   36,  4,   5,
        6,   6,   85,  111, 147, 5,   35,  7,   8,   8,   85,  74,  31,  5,
        43,  6,   3,   1,   0,   4,   5,   7,   0,   1,   8,   131, 64,  130,
        96,  53,  40,  64,  20,  1,   186, 32,  3,   166, 64,  6,   8,   1,
        7,   2,   4,   6,   3,   5,   74,  10,  3,   184, 130, 24,  48,  136,
        181, 32,  0,   1,   73,  9,   10,  188, 2,   27,  0,   33,  0,   112,
        1,   2,   70,  132, 5,   8,   35,  50,  26,  25,  77,  254, 24,  228,
        25,  78,  16,  230, 17,  57,  57,  18,  57,  17,  57,  0,   24,  47,
        26,  77,  237, 125, 26,  25,  253, 24,  244, 26,  24,  237, 17,  51,
        17,  104, 231, 6,   34,  5,   125, 16,  24,  89,  125, 7,   37,  135,
        14,  46,  24,  43,  14,  130, 15,  130, 191, 8,   33,  39,  37,  1,
        1,   51,  1,   35,  1,   181, 65,  1,   12,  1,   130, 2,   67,  158,
        253, 102, 104, 254, 93,  1,   70,  106, 157, 253, 119, 6,   140, 248,
        136, 2,   190, 130, 209, 8,   40,  237, 254, 216, 4,   189, 5,   237,
        0,   20,  0,   124, 64,  31,  1,   2,   18,  0,   5,   19,  2,   18,
        20,  15,  16,  3,   17,  20,  15,  4,   3,   17,  0,   5,   15,  20,
        20,  91,  0,   5,   20,  0,   130, 3,   8,   54,  184, 2,   19,  181,
        3,   18,  2,   123, 17,  3,   184, 1,   101, 179, 7,   10,  135, 9,
        184, 1,   68,  180, 12,  87,  7,   1,   22,  65,  14,  1,   126, 0,
        9,   1,   248, 0,   5,   1,   84,  0,   0,   1,   84,  0,   21,  0,
        22,  1,   161, 0,   33,  1,   213, 1,   189, 131, 207, 49,  25,  244,
        244, 24,  253, 230, 0,   63,  237, 253, 237, 16,  244, 60,  253, 60,
        16,  237, 74,  232, 7,   32,  15,  130, 0,   130, 172, 8,   92,  19,
        35,  53,  51,  55,  18,  33,  50,  23,  7,   38,  35,  34,  6,   7,
        7,   51,  21,  35,  3,   237, 201, 166, 196, 21,  109, 1,   131, 109,
        119, 30,  106, 93,  109, 133, 34,  39,  189, 218, 202, 254, 216, 3,
        244, 148, 106, 2,   35,  29,  156, 38,  138, 172, 196, 148, 252, 12,
        0,   2,   0,   105, 0,   197, 4,   167, 3,   214, 0,   28,  0,   57,
        0,   169, 177, 29,  0,   184, 255, 255, 64,  81,  2,   37,  35,  35,
        85,  52,  50,  20,  52,  52,  50,  8,   106, 82,  8,   8,   39,  35,
        52,  37,  50,  4,   47,  32,  6,   23,  8,   21,  4,   18,  15,  100,
        25,  94,  64,  3,   250, 128, 10,  94,  0,   108, 18,  188, 54,  94,
        64,  44,  108, 32,  250, 128, 39,  94,  29,  108, 47,  132, 39,  33,
        42,  57,  132, 39,  57,  28,  13,  94,  15,  15,  42,  94,  44,  184,
        1,   175, 64,  10,  29,  28,  94,  0,   0,   57,  94,  29,  221, 58,
        16,  246, 237, 130, 197, 33,  16,  253, 131, 5,   37,  17,  23,  57,
        17,  18,  23,  106, 127, 6,   35,  228, 26,  237, 254, 106, 135, 9,
        65,  163, 7,   75,  211, 8,   131, 8,   34,  1,   23,  56,  130, 242,
        32,  52,  106, 151, 24,  32,  3,   153, 26,  37,  105, 156, 120, 52,
        99,  64,  106, 178, 18,  32,  148, 130, 24,  106, 203, 9,   33,  151,
        103, 106, 203, 6,   42,  43,  82,  2,   142, 143, 185, 37,  33,  57,
        69,  21,  106, 203, 8,   36,  17,  84,  96,  254, 92,  106, 224, 6,
        8,   67,  92,  87,  6,   184, 149, 29,  52,  57,  62,  18,  84,  97,
        0,   0,   2,   0,   11,  0,   0,   5,   66,  5,   200, 0,   5,   0,
        8,   0,   120, 64,  61,  3,   8,   7,   7,   127, 4,   3,   20,  4,
        4,   3,   2,   1,   1,   138, 6,   8,   20,  6,   6,   8,   8,   1,
        4,   3,   0,   3,   2,   2,   7,   6,   127, 0,   10,  6,   2,   1,
        130, 12,  59,  7,   4,   8,   10,  115, 5,   5,   4,   128, 7,   194,
        8,   194, 6,   131, 1,   1,   0,   73,  9,   10,  184, 1,   64,  179,
        33,  97,  102, 67,  193, 5,   45,  77,  16,  253, 25,  244, 244, 24,
        253, 60,  16,  228, 17,  18,  57,  96,  87,  5,   35,  253, 60,  63,
        60,  66,  195, 17,  63,  49,  48,  51,  53,  1,   51,  1,   21,  37,
        33,  1,   11,  2,   53,  208, 2,   50,  251, 126, 3,   159, 254, 50,
        187, 5,   13,  250, 243, 187, 187, 4,   37,  130, 165, 38,  111, 0,
        99,  3,   231, 3,   219, 130, 165, 36,  11,  0,   190, 64,  53,  24,
        96,  215, 23,  43,  6,   7,   7,   85,  10,  11,  20,  10,  7,   8,
        10,  11,  130, 200, 34,  85,  10,  9,   130, 11,  47,  6,   10,  9,
        6,   11,  11,  0,   5,   184, 1,   152, 179, 10,  7,   4,   1,   130,
        7,   46,  64,  37,  32,  2,   3,   3,   8,   15,  9,   1,   9,   13,
        71,  71,  74,  130, 86,  130, 222, 49,  167, 1,   106, 4,   148, 7,
        11,  6,   6,   9,   8,   167, 7,   106, 10,  73,  12,  86,  24,  73,
        168, 8,   40,  77,  253, 244, 50,  60,  17,  51,  16,  246, 133, 7,
        41,  78,  16,  69,  101, 68,  230, 0,   47,  93,  51,  130, 20,  39,
        26,  25,  77,  253, 60,  60,  60,  253, 131, 32,  33,  135, 8,   67,
        141, 6,   154, 8,   55,  9,   2,   7,   1,   1,   5,   1,   1,   7,
        1,   1,   3,   231, 254, 216, 1,   40,  98,  254, 117, 1,   139, 254,
        215, 136, 10,  43,  3,   145, 254, 142, 254, 142, 74,  1,   188, 1,
        188, 74,  136, 9,   39,  0,   2,   0,   74,  0,   99,  3,   194, 65,
        3,   6,   46,  189, 64,  53,  2,   1,   1,   85,  4,   3,   20,  4,
        1,   0,   4,   3,   65,  15,  11,  140, 247, 65,  15,  11,  35,  9,
        9,   2,   3,   65,  3,   11,  8,   38,  40,  5,   32,  0,   5,   5,
        6,   15,  11,  1,   11,  13,  71,  71,  74,  10,  106, 7,   9,   8,
        8,   11,  6,   167, 7,   148, 4,   106, 1,   167, 2,   3,   2,   2,
        5,   0,   73,  12,  109, 83,  117, 5,   131, 221, 36,  77,  16,  244,
        253, 246, 65,  8,   5,   84,  188, 5,   65,  1,   9,   32,  16,  65,
        2,   44,  34,  55,  1,   1,   130, 2,   32,  37,  132, 5,   43,  74,
        1,   40,  254, 216, 99,  1,   138, 254, 118, 1,   40,  131, 10,  47,
        98,  1,   139, 254, 117, 173, 1,   114, 1,   114, 74,  254, 68,  254,
        68,  74,  136, 9,   41,  0,   3,   0,   247, 0,   0,   7,   9,   0,
        197, 130, 9,   51,  7,   0,   11,  0,   74,  64,  20,  10,  6,   6,
        2,   106, 3,   8,   7,   7,   4,   4,   3,   10,  133, 196, 39,  9,
        184, 1,   96,  178, 7,   106, 5,   130, 6,   34,  181, 3,   106, 130,
        192, 32,  249, 71,  37,  5,   37,  78,  16,  244, 77,  253, 246, 85,
        118, 7,   72,  86,  5,   33,  60,  77,  130, 183, 40,  16,  60,  49,
        48,  51,  53,  51,  21,  33,  134, 3,   39,  247, 197, 1,   226, 197,
        1,   225, 197, 133, 0,   42,  255, 255, 0,   17,  0,   0,   5,   110,
        7,   108, 2,   110, 3,   7,   41,  67,  0,   75,  1,   65,  0,   18,
        64,  10,  2,   107, 25,  5,   33,  2,   11,  106, 237, 9,   133, 43,
        32,  46,  136, 43,  34,  216, 0,   74,  134, 43,  37,  11,  24,  1,
        2,   72,  51,  139, 43,  37,  104, 255, 219, 5,   208, 7,   131, 43,
        40,  50,  0,   0,   1,   23,  0,   216, 0,   167, 134, 43,  102, 49,
        7,   134, 87,  34,  0,   2,   0,   130, 43,  8,   55,  7,   207, 5,
        237, 0,   26,  0,   41,  0,   196, 64,  17,  40,  8,   40,  21,  23,
        30,  18,  20,  19,  89,  22,  175, 21,  21,  26,  15,  28,  184, 1,
        14,  64,  22,  14,  32,  119, 64,  14,  141, 70,  10,  3,   17,  18,
        89,  16,  15,  2,   24,  23,  89,  25,  26,  8,   27,  131, 26,  8,
        42,  35,  0,   38,  119, 64,  0,   141, 70,  2,   9,   30,  40,  35,
        27,  28,  15,  26,  90,  18,  23,  23,  24,  6,   21,  141, 17,  155,
        24,  74,  43,  35,  121, 6,   73,  42,  43,  184, 2,   41,  179, 33,
        118, 233, 77,  16,  10,  88,  234, 6,   50,  253, 60,  57,  57,  17,
        57,  57,  0,   63,  26,  236, 26,  237, 16,  237, 63,  60,  253, 60,
        132, 3,   133, 14,  40,  17,  18,  57,  47,  244, 253, 60,  17,  57,
        130, 8,   52,  49,  48,  67,  121, 64,  28,  33,  37,  3,   9,   4,
        37,  8,   38,  37,  3,   35,  110, 0,   33,  9,   130, 4,   41,  36,
        5,   38,  110, 0,   34,  7,   32,  110, 1,   76,  149, 9,   33,  6,
        35,  24,  86,  238, 8,   35,  22,  22,  23,  53,  89,  24,  5,   33,
        37,  17,  131, 7,   62,  17,  52,  38,  38,  35,  34,  2,   17,  16,
        18,  51,  50,  54,  54,  4,   105, 156, 191, 194, 254, 191, 163, 163,
        1,   63,  207, 61,  110, 98,  67,  3,   24,  93,  65,  8,   8,   50,
        252, 154, 73,  151, 112, 219, 247, 245, 224, 106, 142, 85,  58,  95,
        195, 1,   106, 220, 220, 1,   105, 196, 17,  36,  41,  57,  157, 254,
        37,  155, 6,   253, 226, 157, 2,   57,  1,   86,  181, 172, 96,  254,
        180, 254, 224, 254, 226, 254, 178, 84,  176, 74,  221, 6,   8,   79,
        6,   220, 4,   86,  0,   36,  0,   49,  0,   56,  0,   209, 64,  52,
        78,  8,   25,  3,   0,   15,  37,  44,  51,  50,  123, 23,  24,  24,
        3,   11,  54,  87,  19,  19,  44,  87,  11,  7,   31,  135, 64,  32,
        124, 10,  28,  87,  34,  37,  87,  34,  3,   11,  0,   15,  41,  50,
        114, 24,  147, 41,  41,  7,   51,  244, 32,  23,  184, 1,   35,  182,
        58,  47,  105, 7,   73,  57,  58,  184, 2,   35,  76,  127, 8,   32,
        77,  76,  124, 7,   40,  253, 228, 18,  57,  57,  0,   63,  60,  237,
        76,  124, 16,  42,  237, 60,  16,  237, 17,  18,  57,  47,  60,  253,
        60,  130, 6,   76,  98,  6,   8,   54,  56,  38,  49,  1,   14,  39,
        37,  5,   37,  13,  37,  9,   38,  38,  2,   41,  95,  1,   1,   0,
        49,  4,   47,  95,  0,   43,  12,  41,  95,  1,   14,  15,  45,  10,
        47,  95,  0,   40,  1,   37,  95,  2,   48,  6,   37,  95,  0,   42,
        14,  44,  95,  3,   46,  8,   44,  76,  100, 13,  32,  43,  83,  99,
        5,   36,  37,  6,   6,   35,  34,  126, 165, 8,   32,  22,  76,  92,
        19,  37,  37,  50,  54,  54,  53,  52,  87,  244, 6,   76,  94,  7,
        8,   68,  211, 75,  171, 113, 148, 232, 129, 123, 233, 155, 77,  115,
        105, 55,  51,  101, 138, 75,  144, 195, 78,  253, 95,  11,  83,  152,
        119, 61,  128, 119, 192, 158, 105, 225, 254, 60,  76,  114, 68,  140,
        118, 138, 165, 164, 2,   93,  1,   200, 127, 87,  105, 125, 174, 114,
        85,  132, 1,   16,  164, 158, 1,   15,  138, 37,  81,  82,  84,  76,
        40,  76,  87,  9,   8,   34,  66,  82,  82,  169, 186, 208, 194, 215,
        204, 204, 216, 2,   13,  180, 135, 159, 0,   1,   0,   105, 2,   31,
        3,   151, 2,   154, 0,   3,   0,   32,  64,  10,  1,   98,  0,   93,
        20,  6,   35,  184, 1,   166, 179, 24,  113, 218, 19,  39,  105, 3,
        46,  2,   31,  123, 123, 0,   130, 57,  38,  99,  2,   31,  7,   158,
        2,   130, 134, 57,  32,  236, 136, 57,  33,  2,   40,  148, 57,  8,
        74,  99,  7,   59,  2,   31,  99,  99,  0,   0,   2,   0,   111, 4,
        37,  2,   142, 6,   43,  0,   12,  0,   25,  0,   100, 64,  53,  21,
        7,   106, 64,  19,  6,   139, 0,   128, 1,   14,  200, 13,  64,  13,
        0,   0,   27,  71,  71,  74,  1,   1,   0,   106, 128, 5,   107, 67,
        7,   106, 8,   93,  20,  64,  14,  13,  106, 128, 18,  107, 67,  20,
        106, 21,  73,  26,  27,  185, 134, 152, 43,  244, 77,  237, 118, 237,
        24,  26,  253, 60,  26,  16,  254, 134, 9,   70,  5,   6,   36,  63,
        60,  26,  77,  16,  131, 23,  33,  252, 60,  93,  112, 5,   44,  21,
        6,   6,   21,  21,  51,  21,  35,  53,  52,  54,  54,  37,  139, 12,
        60,  2,   142, 38,  33,  71,  198, 46,  95,  254, 223, 38,  32,  70,
        197, 45,  96,  6,   43,  74,  20,  89,  115, 23,  197, 166, 129, 117,
        92,  14,  137, 10,  143, 181, 34,  115, 64,  64,  130, 171, 45,  13,
        1,   200, 64,  0,   139, 6,   128, 6,   18,  106, 20,  20,  8,   130,
        183, 32,  1,   131, 185, 36,  18,  107, 22,  64,  67,  131, 176, 40,
        22,  106, 19,  93,  5,   107, 9,   64,  67,  131, 202, 36,  9,   106,
        15,  6,   1,   130, 2,   32,  6,   139, 192, 35,  93,  93,  77,  253,
        130, 160, 37,  118, 26,  16,  237, 24,  246, 136, 9,   40,  78,  69,
        101, 68,  230, 93,  0,   63,  60,  134, 195, 38,  26,  237, 60,  16,
        237, 49,  48,  24,  104, 114, 12,  34,  5,   53,  54,  24,  104, 127,
        9,   32,  111, 130, 195, 48,  197, 44,  96,  1,   32,  39,  32,  71,
        198, 45,  96,  4,   37,  74,  20,  90,  114, 130, 195, 33,  128, 118,
        136, 195, 130, 10,  8,   36,  0,   0,   1,   0,   200, 3,   244, 1,
        191, 6,   43,  0,   12,  0,   75,  64,  41,  8,   166, 64,  7,   139,
        128, 1,   200, 0,   0,   15,  14,  1,   14,  71,  71,  74,  8,   6,
        94,  132, 173, 39,  166, 128, 8,   180, 0,   9,   1,   9,   24,  116,
        53,  11,  34,  93,  77,  237, 135, 169, 65,  100, 5,   130, 160, 130,
        18,  33,  252, 26,  130, 153, 35,  1,   21,  14,  2,   65,  94,  7,
        51,  1,   191, 32,  39,  25,  96,  247, 115, 6,   43,  74,  9,   50,
        76,  91,  21,  246, 214, 168, 171, 141, 121, 54,  82,  64,  49,  7,
        11,  23,  11,  39,  11,  55,  11,  4,   1,   200, 64,  0,   139, 128,
        7,   166, 8,   0,   0,   133, 130, 47,  6,   94,  10,  64,  67,  1,
        0,   166, 128, 10,  166, 15,  7,   1,   7,   148, 24,  116, 183, 8,
        33,  246, 93,  65,  31,  16,  34,  77,  253, 26,  133, 126, 32,  93,
        24,  116, 190, 17,  55,  116, 3,   244, 74,  9,   49,  77,  91,  20,
        247, 214, 170, 170, 0,   0,   3,   0,   105, 0,   74,  4,   167, 4,
        86,  70,  167, 6,   58,  41,  64,  23,  0,   128, 1,   246, 4,   85,
        5,   246, 8,   128, 9,   6,   7,   168, 10,  3,   128, 9,   0,   168,
        4,   221, 12,  16,  90,  190, 5,   37,  0,   63,  253, 246, 253, 246,
        130, 219, 38,  37,  53,  51,  21,  1,   53,  33,  130, 3,   54,  51,
        21,  2,   25,  222, 253, 114, 4,   62,  253, 114, 222, 74,  222, 222,
        1,   188, 148, 148, 1,   114, 222, 222, 91,  49,  8,   32,  62,  132,
        91,  41,  159, 64,  45,  5,   4,   4,   85,  0,   3,   20,  130, 117,
        44,  4,   7,   7,   85,  1,   0,   20,  1,   1,   0,   5,   6,   6,
        76,  146, 7,   131, 19,  32,  2,   130, 19,  45,  2,   4,   0,   6,
        3,   2,   184, 1,   157, 179, 3,   5,   7,   1,   130, 7,   53,  64,
        25,  32,  0,   10,  5,   3,   7,   2,   1,   9,   71,  71,  74,  3,
        150, 0,   4,   6,   2,   150, 1,   24,  105, 147, 8,   38,  25,  77,
        244, 60,  60,  60,  244, 116, 16,  5,   42,  17,  18,  57,  17,  57,
        0,   63,  26,  25,  77,  254, 130, 20,  36,  254, 17,  57,  18,  57,
        76,  118, 8,   154, 8,   52,  49,  48,  33,  9,   6,   2,   136, 253,
        225, 2,   31,  2,   31,  253, 225, 1,   77,  254, 179, 254, 178, 133,
        11,  35,  254, 178, 1,   78,  131, 15,  57,  255, 255, 0,   16,  254,
        117, 4,   34,  5,   176, 2,   50,  0,   92,  0,   0,   1,   22,  0,
        142, 176, 0,   0,   22,  64,  12,  130, 151, 39,  15,  2,   6,   72,
        51,  2,   1,   8,   108, 141, 8,   71,  139, 5,   35,  4,   226, 6,
        241, 130, 45,  32,  60,  130, 45,  38,  23,  0,   142, 0,   4,   1,
        65,  134, 47,  32,  16,  133, 47,  33,  9,   32,  117, 143, 8,   46,
        1,   253, 151, 255, 219, 2,   154, 5,   237, 0,   3,   0,   69,  64,
        24,  24,  117, 215, 9,   39,  2,   1,   3,   0,   115, 3,   9,   5,
        130, 250, 8,   38,  2,   106, 1,   191, 1,   96,  0,   4,   0,   5,
        1,   180, 0,   33,  1,   185, 1,   160, 0,   24,  43,  43,  25,  246,
        24,  253, 25,  78,  69,  101, 68,  230, 0,   24,  63,  77,  228, 63,
        60,  96,  178, 6,   34,  5,   1,   51,  130, 84,  41,  4,   67,  192,
        251, 189, 36,  6,   17,  249, 238, 130, 132, 8,   147, 255, 240, 255,
        219, 4,   156, 5,   238, 0,   49,  0,   205, 64,  103, 106, 30,  1,
        2,   39,  3,   1,   1,   17,  0,   20,  47,  43,  28,  40,  25,  24,
        43,  40,  43,  88,  43,  136, 43,  4,   43,  43,  4,   38,  119, 34,
        135, 34,  151, 34,  167, 34,  183, 34,  5,   34,  35,  38,  145, 31,
        5,   136, 7,   152, 7,   168, 7,   184, 7,   4,   7,   23,  6,   135,
        6,   2,   0,   6,   4,   145, 9,   9,   39,  17,  87,  17,  2,   28,
        27,  20,  17,  4,   23,  119, 40,  1,   23,  0,   39,  0,   87,  0,
        3,   2,   40,  43,  47,  0,   4,   45,  35,  48,  41,  41,  45,  35,
        35,  6,   184, 255, 240, 182, 6,   51,  26,  18,  45,  206, 23,  47,
        237, 220, 60,  16,  222, 56,  60,  17,  18,  57,  47,  198, 17,  18,
        23,  57,  95,  93,  93,  130, 5,   45,  93,  0,   63,  237, 212, 95,
        93,  205, 93,  63,  237, 221, 205, 93,  131, 28,  37,  93,  60,  221,
        60,  16,  214, 130, 4,   8,   42,  49,  48,  1,   95,  93,  95,  93,
        1,   75,  176, 17,  80,  88,  64,  14,  118, 30,  1,   3,   9,   30,
        39,  13,  12,  11,  14,  11,  11,  14,  129, 16,  7,   60,  60,  8,
        94,  95,  93,  89,  1,   23,  22,  22,  80,  13,  6,   42,  46,  4,
        53,  53,  35,  55,  51,  53,  38,  53,  55,  131, 7,   8,   122, 55,
        18,  33,  50,  22,  23,  21,  38,  38,  35,  32,  3,   33,  7,   33,
        21,  7,   23,  21,  33,  7,   1,   90,  2,   43,  229, 168, 201, 191,
        175, 239, 89,  155, 132, 108, 83,  76,  139, 32,  90,  2,   2,   122,
        32,  107, 2,   114, 2,   17,  112, 178, 122, 133, 186, 93,  254, 173,
        83,  2,   154, 33,  253, 117, 2,   2,   2,   121, 31,  2,   16,  6,
        190, 205, 154, 181, 137, 32,  63,  93,  125, 198, 48,  6,   150, 6,
        21,  38,  54,  6,   149, 6,   2,   48,  43,  48,  196, 65,  64,  254,
        104, 149, 6,   64,  49,  6,   150, 0,   0,   1,   0,   111, 0,   99,
        2,   92,  3,   219, 0,   5,   0,   125, 64,  26,  74,  233, 11,  75,
        1,   12,  53,  5,   187, 1,   152, 0,   4,   0,   1,   1,   152, 64,
        23,  32,  2,   15,  3,   1,   3,   7,   71,  71,  74,  74,  197, 5,
        47,  167, 1,   106, 4,   73,  6,   7,   188, 1,   13,  0,   33,  0,
        86,  1,   28,  79,  112, 5,   43,  77,  253, 25,  244, 0,   50,  1,
        60,  0,   17,  51,  1,   68,  102, 5,   42,  0,   24,  47,  93,  51,
        26,  25,  77,  253, 60,  253, 93,  206, 5,   34,  14,  125, 16,  93,
        206, 5,   131, 8,   38,  9,   2,   7,   1,   1,   2,   92,  75,  167,
        19,  130, 165, 32,  74,  130, 165, 32,  55,  132, 165, 34,  122, 64,
        26,  140, 153, 32,  1,   24,  109, 131, 10,  32,  3,   139, 165, 36,
        0,   15,  5,   1,   5,   131, 165, 75,  111, 10,  45,  6,   7,   184,
        1,   13,  179, 33,  109, 92,  24,  43,  43,  25,  78,  136, 159, 35,
        77,  16,  244, 24,  75,  115, 9,   152, 162, 32,  55,  75,  82,  14,
        75,  71,  9,   8,   66,  0,   2,   0,   80,  0,   0,   4,   98,  6,
        68,  0,   25,  0,   29,  0,   119, 64,  50,  28,  8,   12,  29,  11,
        9,   14,  29,  147, 27,  10,  14,  87,  9,   1,   23,  2,   123, 20,
        3,   6,   25,  22,  10,  27,  23,  23,  22,  91,  21,  21,  28,  20,
        74,  31,  18,  25,  91,  5,   0,   191, 2,   73,  30,  31,  228, 33,
        140, 161, 90,  57,  12,  33,  60,  60,  75,  59,  5,   32,  0,   90,
        60,  5,   38,  63,  237, 63,  237, 17,  18,  57,  109, 83,  7,   24,
        78,  236, 45,  41,  17,  35,  17,  33,  17,  1,   53,  51,  21,  234,
        24,  78,  242, 11,  39,  2,   178, 197, 254, 19,  1,   237, 197, 24,
        78,  246, 13,  41,  251, 194, 3,   170, 252, 86,  5,   3,   197, 197,
        94,  167, 6,   32,  105, 130, 209, 8,   48,  26,  0,   131, 64,  37,
        28,  8,   15,  17,  3,   10,  14,  24,  12,  122, 17,  87,  10,  9,
        1,   24,  2,   123, 3,   23,  22,  22,  4,   4,   3,   6,   26,  14,
        10,  15,  14,  91,  12,  13,  74,  28,  24,  184, 1,   88,  64,  15,
        21,  26,  133, 214, 33,  27,  28,  141, 214, 32,  228, 83,  67,  6,
        132, 211, 33,  16,  60,  130, 1,   38,  253, 60,  63,  60,  237, 228,
        1,   130, 217, 32,  0,   74,  222, 7,   50,  18,  18,  20,  6,   8,
        19,  37,  7,   38,  18,  8,   21,  101, 0,   20,  6,   17,  101, 1,
        24,  79,  200, 18,  32,  51,  130, 211, 92,  164, 5,   55,  21,  33,
        21,  33,  17,  240, 160, 160, 100, 170, 142, 92,  170, 215, 198, 189,
        101, 63,  96,  44,  1,   15,  254, 241, 130, 214, 49,  111, 162, 162,
        83,  25,  249, 213, 5,   131, 45,  41,  78,  94,  157, 148, 252, 86,
        0,   113, 111, 10,  35,  19,  0,   209, 185, 130, 15,  8,   82,  97,
        64,  16,  2,   18,  1,   100, 35,  17,  2,   85,  3,   64,  15,  4,
        100, 35,  16,  3,   184, 1,   95,  64,  16,  6,   14,  5,   100, 35,
        13,  6,   85,  7,   64,  11,  8,   100, 35,  12,  7,   184, 1,   97,
        64,  9,   9,   2,   21,  71,  71,  74,  17,  17,  12,  184, 1,   50,
        64,  24,  10,  19,  11,  14,  14,  15,  15,  18,  100, 26,  19,  127,
        64,  9,   0,   8,   5,   5,   4,   4,   1,   100, 26,  0,   130, 28,
        36,  182, 7,   2,   73,  20,  113, 144, 6,   34,  60,  77,  246, 83,
        89,  6,   32,  252, 131, 242, 130, 3,   32,  26,  113, 149, 11,  32,
        16,  130, 16,  32,  246, 83,  129, 6,   33,  0,   63,  113, 155, 30,
        32,  60,  133, 73,  32,  27,  113, 184, 27,  32,  17,  113, 188, 8,
        33,  17,  37,  113, 192, 9,   113, 196, 10,  45,  1,   90,  254, 166,
        25,  254, 216, 2,   31,  25,  148, 25,  1,   238, 113, 205, 9,   33,
        254, 18,  130, 16,  51,  253, 225, 0,   1,   2,   12,  1,   213, 3,
        3,   2,   204, 0,   3,   0,   32,  64,  13,  1,   166, 74,  13,  7,
        38,  180, 33,  252, 185, 1,   21,  0,   103, 91,  8,   8,   63,  77,
        237, 49,  48,  1,   53,  51,  21,  2,   12,  247, 1,   213, 247, 247,
        0,   0,   1,   0,   200, 254, 216, 1,   191, 0,   247, 0,   11,  0,
        65,  64,  34,  1,   200, 64,  0,   167, 128, 8,   166, 6,   10,  5,
        94,  64,  67,  13,  71,  71,  74,  9,   1,   0,   166, 128, 9,   180,
        6,   73,  12,  13,  180, 33,  120, 111, 75,  6,   33,  253, 26,  84,
        129, 7,   34,  118, 26,  77,  24,  124, 201, 10,  73,  35,  11,  8,
        34,  200, 48,  48,  96,  247, 108, 254, 216, 74,  14,  107, 81,  20,
        247, 214, 134, 181, 0,   2,   0,   111, 254, 191, 2,   142, 0,   197,
        0,   12,  0,   25,  0,   107, 64,  58,  73,  195, 10,  38,  21,  8,
        106, 18,  6,   5,   10,  73,  192, 30,  73,  189, 13,  73,  188, 24,
        41,  0,   63,  16,  60,  77,  253, 60,  26,  16,  252, 73,  187, 41,
        39,  38,  33,  71,  198, 45,  96,  254, 191, 74,  116, 6,   34,  128,
        117, 93,  74,  127, 7,   130, 10,  8,   36,  0,   0,   7,   0,   12,
        255, 219, 8,   12,  5,   237, 0,   3,   0,   15,  0,   27,  0,   39,
        0,   51,  0,   63,  0,   75,  1,   73,  64,  47,  152, 8,   1,   0,
        0,   98,  3,   2,   24,  131, 241, 10,  8,   46,  4,   1,   2,   5,
        46,  116, 34,  34,  70,  116, 58,  186, 64,  116, 52,  52,  40,  116,
        28,  12,  3,   0,   13,  77,  71,  71,  74,  67,  186, 2,   63,  0,
        67,  2,   63,  178, 61,  254, 73,  184, 2,   63,  179, 55,  188, 37,
        49,  130, 7,   41,  183, 31,  254, 37,  1,   94,  2,   115, 43,  186,
        130, 30,  38,  37,  1,   63,  182, 7,   3,   94,  24,  132, 9,   16,
        49,  76,  97,  217, 24,  43,  78,  16,  244, 77,  237, 253, 237, 25,
        16,  244, 24,  237, 16,  130, 7,   133, 6,   33,  16,  246, 130, 19,
        32,  237, 71,  158, 5,   38,  63,  60,  63,  77,  237, 60,  16,  130,
        16,  130, 4,   130, 12,  130, 8,   71,  171, 6,   88,  1,   5,   39,
        122, 5,   75,  17,  15,  19,  99,  1,   24,  132, 18,  34,  43,  65,
        63,  67,  99,  1,   75,  53,  73,  99,  0,   69,  59,  130, 9,   33,
        71,  57,  130, 9,   34,  18,  14,  16,  24,  132, 38,  36,  38,  66,
        62,  64,  99,  0,   74,  54,  130, 4,   38,  68,  60,  70,  99,  1,
        72,  56,  130, 4,   32,  0,   24,  180, 156, 11,  32,  1,   139, 12,
        37,  129, 23,  1,   51,  1,   19,  116, 14,  23,  150, 23,  32,  5,
        150, 23,  32,  24,  24,  132, 90,  37,  45,  3,   12,  147, 167, 168,
        146, 147, 168, 167, 148, 77,  90,  90,  77,  24,  132, 107, 45,  32,
        99,  24,  132, 125, 16,  81,  23,  18,  32,  215, 80,  235, 8,   39,
        16,  12,  1,   2,   72,  51,  2,   16,  124, 49,  12,  33,  4,   37,
        124, 93,  10,  32,  215, 124, 93,  8,   33,  17,  13,  131, 43,  32,
        1,   138, 43,  81,  111, 15,  32,  141, 81,  111, 8,   32,  11,  132,
        43,  81,  67,  11,  39,  191, 0,   0,   4,   37,  6,   241, 2,   124,
        181, 7,   32,  142, 132, 87,  38,  22,  64,  12,  2,   1,   13,  19,
        132, 132, 33,  1,   12,  74,  15,  8,   124, 229, 18,  32,  67,  136,
        135, 33,  13,  15,  124, 229, 15,  36,  122, 0,   0,   2,   54,  132,
        179, 40,  44,  0,   0,   1,   23,  0,   141, 254, 178, 81,  159, 5,
        34,  1,   4,   7,   132, 43,  32,  4,   136, 223, 33,  255, 218, 130,
        43,  32,  116, 138, 43,  32,  215, 136, 43,  121, 45,  7,   136, 43,
        33,  0,   11,  130, 43,  32,  67,  132, 179, 133, 87,  32,  142, 132,
        43,  121, 45,  13,  139, 179, 32,  23,  130, 125, 32,  211, 133, 91,
        32,  255, 131, 135, 34,  67,  254, 177, 151, 135, 37,  0,   104, 255,
        219, 5,   208, 132, 43,  82,  83,  5,   32,  141, 82,  83,  9,   121,
        49,  6,   137, 135, 143, 43,  32,  215, 136, 43,  32,  33,  121, 9,
        6,   153, 43,  32,  67,  136, 43,  121, 95,  7,   137, 43,  36,  179,
        255, 219, 4,   217, 132, 131, 32,  56,  130, 185, 36,  23,  0,   141,
        0,   80,  134, 175, 39,  21,  23,  0,   11,  72,  51,  1,   21,  153,
        43,  32,  215, 136, 43,  33,  28,  22,  132, 43,  32,  26,  153, 43,
        32,  67,  136, 43,  33,  22,  24,  140, 87,  35,  0,   1,   0,   197,
        130, 121, 42,  138, 4,   62,  0,   3,   0,   56,  64,  25,  1,   6,
        24,  82,  179, 9,   42,  2,   3,   91,  1,   111, 0,   127, 0,   2,
        0,   73,  24,  82,  180, 19,  32,  16,  79,  14,  6,   24,  82,  181,
        8,   8,   52,  4,   62,  251, 194, 0,   1,   1,   40,  5,   3,   3,
        194, 6,   43,  0,   6,   0,   67,  64,  29,  0,   6,   181, 128, 1,
        2,   4,   5,   8,   71,  71,  74,  2,   131, 64,  1,   209, 3,   209,
        160, 4,   131, 5,   73,  7,   8,   255, 33,  207, 185, 1,   23,  0,
        73,  187, 5,   39,  24,  77,  237, 26,  253, 253, 26,  237, 76,  71,
        7,   8,   32,  47,  60,  60,  60,  26,  77,  253, 60,  49,  48,  1,
        19,  35,  39,  7,   35,  19,  2,   228, 222, 173, 160, 160, 173, 222,
        6,   43,  254, 216, 197, 197, 1,   40,  135, 103, 8,   37,  5,   237,
        0,   25,  0,   101, 64,  38,  9,   1,   6,   15,  25,  1,   22,  15,
        41,  1,   38,  15,  57,  1,   54,  15,  8,   12,  23,  94,  64,  3,
        248, 21,  9,   94,  17,  17,  0,   27,  130, 124, 47,  12,  107, 64,
        13,  255, 184, 1,   64,  64,  9,   25,  107, 0,   73,  26,  27,  137,
        126, 33,  78,  244, 131, 124, 32,  26,  69,  35,  6,   52,  47,  60,
        77,  16,  237, 124, 75,  82,  88,  176, 128, 26,  27,  176, 192, 26,
        89,  253, 24,  26,  237, 131, 135, 50,  93,  1,   54,  54,  51,  50,
        31,  2,   22,  51,  50,  54,  55,  51,  20,  6,   6,   35,  34,  106,
        12,  5,   8,   44,  7,   1,   40,  6,   99,  78,  67,  64,  47,  69,
        23,  17,  21,  49,  3,   123, 48,  75,  61,  76,  60,  60,  35,  34,
        21,  64,  9,   5,   3,   119, 115, 26,  19,  31,  10,  32,  54,  83,
        95,  56,  28,  28,  16,  14,  86,  130, 183, 110, 169, 5,   38,  151,
        0,   3,   0,   35,  64,  10,  24,  131, 177, 9,   110, 154, 13,  38,
        228, 16,  230, 0,   47,  77,  253, 130, 255, 43,  53,  33,  21,  1,
        89,  2,   56,  5,   3,   148, 148, 0,   65,  93,  10,  55,  13,  0,
        48,  64,  22,  11,  88,  4,   179, 7,   7,   1,   0,   7,   144, 8,
        74,  15,  1,   144, 0,   73,  14,  15,  141, 215, 42,  78,  16,  246,
        77,  237, 0,   63,  60,  16,  244, 237, 130, 74,  33,  51,  22,  133,
        188, 131, 187, 8,   34,  38,  1,   40,  123, 19,  124, 67,  68,  123,
        19,  123, 15,  187, 131, 130, 187, 6,   43,  89,  59,  60,  88,  135,
        161, 160, 0,   1,   2,   18,  5,   3,   2,   215, 5,   200, 72,  51,
        6,   32,  106, 72,  51,  7,   32,  242, 72,  51,  23,  56,  18,  197,
        5,   3,   197, 197, 0,   0,   2,   1,   213, 5,   3,   3,   22,  6,
        68,  0,   11,  0,   23,  0,   66,  64,  17,  122, 61,  8,   46,  25,
        71,  71,  74,  15,  162, 64,  9,   184, 1,   140, 182, 192, 21,  162,
        98,  87,  5,   39,  140, 0,   33,  1,   54,  1,   114, 0,   75,  44,
        5,   34,  237, 26,  253, 65,  131, 8,   65,  144, 5,   118, 19,  13,
        33,  39,  50,  83,  134, 9,   45,  2,   117, 66,  94,  95,  65,  66,
        95,  95,  66,  36,  50,  50,  36,  130, 3,   57,  5,   3,   94,  66,
        67,  94,  94,  67,  66,  94,  74,  51,  35,  36,  51,  51,  36,  35,
        51,  0,   1,   1,   254, 254, 120, 3,   130, 7,   57,  0,   17,  0,
        113, 64,  25,  22,  8,   11,  8,   11,  13,  9,   1,   162, 0,   100,
        16,  162, 3,   239, 8,   94,  9,   10,  19,  130, 149, 55,  10,  6,
        184, 2,   63,  178, 13,  189, 10,  184, 1,   197, 183, 0,   171, 8,
        189, 9,   73,  18,  19,  188, 1,   197, 130, 155, 32,  162, 73,  32,
        6,   37,  244, 77,  236, 228, 253, 244, 100, 201, 6,   36,  0,   63,
        77,  253, 246, 130, 13,  37,  1,   17,  18,  57,  0,   18,  108, 117,
        5,   45,  14,  14,  15,  4,   5,   4,   15,  6,   99,  1,   5,   14,
        3,   99,  94,  42,  6,   34,  53,  22,  51,  131, 177, 34,  39,  55,
        55,  127, 149, 6,   8,   55,  1,   254, 40,  24,  49,  53,  124, 87,
        91,  68,  157, 107, 80,  50,  254, 139, 79,  8,   39,  30,  75,  18,
        140, 1,   108, 38,  108, 60,  79,  0,   2,   1,   112, 5,   3,   4,
        3,   6,   43,  0,   3,   0,   7,   0,   91,  64,  39,  0,   7,   181,
        6,   128, 6,   1,   0,   9,   130, 164, 62,  5,   94,  64,  6,   250,
        128, 7,   107, 64,  4,   239, 3,   30,  1,   94,  64,  2,   250, 128,
        3,   107, 0,   73,  8,   9,   255, 33,  184, 1,   53,  177, 94,  7,
        5,   65,  52,  5,   66,  185, 10,  34,  16,  254, 26,  65,  84,  10,
        83,  123, 6,   38,  49,  48,  1,   19,  51,  3,   51,  130, 3,   40,
        1,   112, 221, 146, 248, 172, 222, 146, 249, 113, 67,  5,   8,   34,
        1,   40,  254, 216, 0,   1,   1,   244, 254, 216, 3,   68,  0,   0,
        0,   16,  0,   81,  64,  23,  16,  162, 0,   115, 2,   162, 14,  0,
        162, 16,  14,  193, 7,   10,  18,  130, 137, 39,  8,   0,   16,  188,
        8,   187, 2,   82,  130, 163, 42,  11,  2,   63,  181, 5,   179, 7,
        73,  17,  18,  188, 130, 15,  34,  33,  1,   162, 119, 214, 8,   35,
        244, 237, 16,  253, 75,  7,   10,  33,  228, 16,  130, 15,  33,  244,
        237, 130, 121, 8,   42,  6,   35,  34,  38,  53,  52,  55,  51,  6,
        6,   21,  20,  22,  51,  50,  55,  3,   68,  69,  61,  85,  121, 144,
        120, 29,  89,  47,  56,  35,  52,  254, 233, 17,  63,  64,  95,  74,
        17,  87,  39,  27,  43,  11,  66,  235, 11,  63,  6,   0,   63,  64,
        28,  3,   202, 6,   64,  1,   5,   181, 128, 6,   0,   2,   1,   4,
        6,   3,   4,   131, 5,   209, 3,   209, 160, 2,   131, 1,   73,  7,
        68,  72,  6,   43,  25,  78,  16,  244, 24,  77,  229, 26,  253, 253,
        229, 18,  87,  134, 5,   37,  47,  26,  253, 60,  26,  16,  131, 121,
        39,  3,   51,  23,  55,  51,  3,   2,   6,   68,  69,  5,   131, 241,
        49,  197, 197, 254, 216, 255, 255, 0,   100, 255, 219, 3,   233, 7,
        108, 2,   50,  0,   54,  130, 114, 36,  23,  0,   224, 255, 202, 69,
        129, 6,   39,  45,  49,  15,  38,  72,  51,  1,   44,  69,  85,  9,
        38,  157, 255, 231, 3,   119, 6,   43,  130, 43,  32,  86,  130, 43,
        45,  22,  0,   224, 149, 0,   0,   18,  64,  10,  1,   43,  47,  15,
        36,  130, 41,  42,  42,  34,  0,   63,  53,  1,   43,  53,  0,   2,
        0,   130, 91,  33,  1,   89,  65,  203, 6,   62,  61,  178, 3,   186,
        2,   184, 1,   149, 64,  26,  7,   186, 5,   0,   9,   71,  71,  74,
        2,   2,   3,   3,   6,   7,   85,  4,   4,   1,   73,  8,   9,   24,
        73,  6,   13,  33,  60,  16,  76,  63,  9,   33,  253, 246, 130, 185,
        36,  19,  17,  51,  17,  3,   130, 3,   43,  197, 148, 148, 148, 254,
        216, 2,   228, 253, 28,  4,   111, 131, 5,   8,   68,  0,   0,   2,
        255, 225, 0,   0,   5,   150, 5,   200, 0,   14,  0,   29,  0,   133,
        64,  41,  43,  8,   27,  3,   89,  28,  2,   2,   0,   6,   5,   24,
        25,  137, 5,   2,   16,  15,  137, 14,  0,   8,   28,  28,  30,  31,
        20,  121, 10,  74,  31,  25,  15,  90,  5,   0,   245, 2,   73,  30,
        31,  186, 1,   110, 0,   33,  1,   117, 177, 169, 78,  166, 12,  44,
        77,  237, 17,  18,  57,  47,  0,   63,  60,  253, 60,  63,  253, 130,
        135, 87,  119, 5,   54,  49,  48,  67,  121, 64,  30,  7,   23,  12,
        38,  18,  37,  8,   37,  22,  38,  17,  13,  20,  110, 1,   23,  7,
        130, 4,   39,  19,  11,  16,  110, 0,   21,  9,   24,  88,  169, 8,
        38,  43,  43,  129, 51,  17,  35,  53,  24,  118, 97,  21,  47,  17,
        33,  21,  33,  191, 222, 222, 1,   236, 1,   29,  1,   38,  168, 154,
        254, 24,  118, 103, 14,  44,  1,   77,  254, 179, 2,   167, 157, 2,
        132, 165, 254, 195, 223, 24,  118, 110, 12,  34,  254, 25,  157, 130,
        235, 39,  0,   109, 255, 231, 4,   98,  6,   45,  130, 233, 8,   35,
        42,  0,   228, 64,  35,  59,  8,   6,   11,  12,  12,  5,   3,   14,
        13,  4,   13,  12,  5,   5,   123, 4,   13,  20,  4,   4,   13,  0,
        36,  11,  14,  6,   3,   4,   28,  9,   130, 19,  55,  4,   184, 1,
        219, 181, 8,   12,  114, 9,   87,  8,   184, 1,   143, 64,  20,  36,
        87,  28,  6,   30,  87,  20,  11,  131, 33,  34,  0,   5,   39,  131,
        33,  60,  33,  186, 2,   59,  0,   12,  1,   221, 64,  14,  17,  74,
        44,  39,  105, 24,  4,   147, 8,   114, 24,  73,  43,  44,  184, 2,
        11,  179, 33,  24,  87,  23,  7,   47,  244, 125, 237, 24,  16,  237,
        78,  16,  246, 125, 77,  237, 24,  237, 17,  51,  130, 1,   44,  23,
        57,  0,   63,  237, 63,  237, 246, 253, 228, 16,  244, 50,  96,  106,
        5,   47,  17,  57,  135, 14,  46,  43,  135, 14,  125, 196, 7,   14,
        60,  60,  7,   16,  130, 4,   75,  83,  5,   56,  42,  37,  42,  31,
        32,  18,  27,  41,  38,  22,  37,  26,  38,  31,  19,  33,  95,  1,
        42,  21,  39,  95,  0,   37,  27,  130, 4,   38,  32,  18,  30,  95,
        0,   40,  23,  130, 4,   36,  38,  25,  36,  95,  1,   24,  141, 177,
        13,  51,  1,   38,  38,  39,  7,   39,  55,  38,  35,  53,  32,  23,
        55,  23,  7,   22,  18,  21,  16,  0,   88,  171, 9,   32,  3,   69,
        13,  10,  8,   79,  22,  2,   230, 38,  75,  75,  242, 81,  188, 136,
        173, 1,   17,  194, 169, 85,  124, 192, 223, 254, 233, 227, 147, 239,
        121, 123, 236, 137, 62,  47,  128, 175, 165, 133, 150, 156, 78,  141,
        4,   43,  62,  84,  61,  162, 121, 129, 69,  148, 112, 114, 123, 81,
        144, 254, 91,  235, 254, 244, 254, 178, 141, 1,   11,  155, 154, 1,
        3,   137, 252, 59,  211, 212, 212, 185, 223, 183, 127, 199, 88,  0,
        74,  175, 5,   35,  4,   226, 7,   108, 84,  59,  8,   34,  141, 0,
        5,   67,  21,  6,   39,  9,   11,  2,   6,   72,  51,  1,   9,   67,
        21,  9,   38,  16,  254, 117, 4,   34,  6,   43,  84,  149, 8,   37,
        141, 164, 0,   0,   7,   178, 84,  139, 5,   8,   59,  0,   0,   2,
        0,   191, 0,   0,   4,   72,  5,   200, 0,   14,  0,   24,  0,   116,
        64,  9,   41,  8,   15,  24,  19,  14,  16,  137, 13,  184, 1,   48,
        179, 0,   4,   137, 24,  184, 1,   230, 64,  22,  1,   2,   0,   8,
        19,  121, 8,   74,  26,  2,   14,  90,  1,   0,   73,  25,  26,  227,
        33,  24,  113, 93,  18,  40,  253, 237, 16,  244, 237, 1,   17,  18,
        57,  69,  119, 5,   47,  28,  5,   22,  10,  38,  6,   37,  21,  38,
        17,  11,  19,  130, 1,   22,  5,   130, 4,   40,  18,  9,   16,  130,
        0,   20,  7,   23,  130, 100, 173, 8,   35,  43,  129, 51,  17,  130,
        1,   121, 113, 5,   8,   50,  4,   35,  35,  17,  17,  51,  50,  54,
        53,  52,  38,  38,  35,  35,  191, 208, 195, 213, 187, 102, 133, 254,
        219, 178, 93,  109, 197, 170, 58,  129, 160, 129, 5,   200, 254, 219,
        84,  173, 131, 154, 227, 123, 254, 217, 1,   193, 194, 140, 76,  106,
        64,  130, 193, 32,  197, 130, 225, 46,  154, 5,   102, 0,   17,  0,
        30,  0,   213, 64,  22,  31,  64,  21,  12,  24,  101, 15,  12,  57,
        18,  30,  24,  3,   2,   184, 1,   88,  64,  17,  27,  126, 6,   7,
        21,  87,  12,  11,  0,   14,  32,  71,  71,  74,  9,   105, 130, 220,
        8,   35,  241, 64,  11,  3,   1,   0,   91,  2,   17,  17,  3,   73,
        31,  32,  186, 1,   252, 0,   33,  1,   148, 177, 198, 24,  43,  43,
        78,  244, 60,  16,  60,  77,  253, 60,  16,  244, 82,  174, 6,   36,
        63,  63,  77,  237, 63,  81,  67,  5,   33,  57,  0,   126, 95,  8,
        33,  24,  22,  24,  101, 37,  71,  41,  14,  37,  19,  15,  21,  129,
        4,   20,  13,  18,  24,  101, 37,  12,  37,  43,  43,  129, 19,  17,
        51,  24,  101, 36,  13,  32,  17,  24,  88,  138, 19,  37,  249, 215,
        63,  114, 89,  54,  24,  88,  138, 11,  35,  6,   241, 254, 12,  24,
        101, 35,  11,  32,  253, 24,  88,  140, 11,  33,  0,   1,   24,  141,
        105, 10,  34,  124, 64,  51,  24,  81,  157, 9,   8,   55,  1,   0,
        5,   2,   9,   4,   0,   5,   3,   8,   9,   8,   3,   3,   85,  2,
        9,   20,  2,   2,   9,   5,   6,   11,  11,  85,  0,   5,   20,  0,
        0,   5,   8,   9,   0,   11,  6,   5,   2,   5,   3,   184, 1,   175,
        64,  10,  9,   11,  9,   8,   5,   11,  0,   3,   8,   6,   130, 14,
        54,  180, 2,   2,   0,   221, 12,  16,  246, 60,  16,  253, 60,  51,
        17,  51,  50,  17,  51,  0,   47,  60,  253, 60,  130, 7,   130, 194,
        32,  51,  67,  132, 7,   130, 7,   37,  24,  43,  135, 14,  125, 196,
        99,  65,  5,   34,  55,  1,   1,   130, 2,   8,   45,  23,  1,   1,
        7,   1,   1,   105, 1,   181, 254, 75,  103, 1,   184, 1,   183, 104,
        254, 74,  1,   182, 102, 254, 71,  254, 73,  159, 1,   177, 1,   172,
        115, 254, 72,  1,   184, 107, 254, 76,  254, 74,  105, 1,   180, 254,
        76,  130, 189, 8,   44,  210, 2,   228, 2,   6,   5,   225, 0,   5,
        0,   59,  64,  14,  3,   4,   5,   0,   24,  2,   94,  4,   27,  7,
        71,  71,  74,  5,   184, 2,   61,  181, 0,   171, 2,   73,  6,   7,
        188, 1,   49,  0,   33,  0,   120, 1,   120, 168, 5,   33,  244, 77,
        65,  204, 8,   50,  77,  237, 63,  60,  17,  57,  49,  48,  1,   17,
        35,  53,  37,  17,  1,   90,  136, 1,   52,  130, 79,  36,  117, 111,
        25,  253, 3,   130, 91,  32,  136, 130, 11,  8,   71,  241, 5,   238,
        0,   27,  0,   82,  64,  23,  1,   27,  25,  94,  0,   24,  13,  107,
        64,  14,  108, 12,  11,  144, 16,  27,  4,   5,   23,  22,  4,   25,
        8,   184, 2,   60,  179, 64,  19,  115, 26,  184, 1,   59,  64,  10,
        192, 25,  131, 14,  0,   73,  28,  112, 92,  24,  43,  78,  16,  244,
        60,  77,  237, 26,  253, 244, 26,  237, 17,  23,  57,  0,   63,  105,
        179, 6,   37,  89,  252, 26,  237, 63,  253, 131, 114, 34,  19,  53,
        54,  24,  128, 236, 18,  8,   51,  7,   7,   6,   7,   33,  21,  136,
        35,  77,  75,  120, 68,  45,  85,  86,  116, 133, 150, 134, 152, 169,
        94,  152, 64,  96,  36,  1,   198, 2,   228, 136, 63,  71,  50,  79,
        46,  67,  38,  55,  55,  64,  134, 48,  120, 94,  75,  125, 81,  34,
        51,  62,  136, 130, 169, 36,  154, 2,   203, 2,   234, 130, 169, 8,
        36,  36,  0,   116, 64,  32,  29,  10,  107, 11,  11,  23,  35,  73,
        1,   94,  64,  0,   108, 15,  3,   144, 35,  25,  18,  144, 23,  20,
        94,  64,  21,  108, 15,  23,  27,  29,  11,  15,  130, 178, 35,  178,
        26,  115, 6,   130, 6,   49,  64,  12,  32,  74,  38,  21,  11,  156,
        0,   73,  37,  38,  253, 33,  184, 1,   38,  177, 111, 79,  6,   41,
        228, 60,  78,  16,  246, 77,  237, 244, 237, 17,  130, 186, 37,  75,
        82,  88,  176, 0,   26,  131, 185, 33,  16,  237, 139, 199, 42,  118,
        17,  18,  57,  47,  24,  237, 57,  49,  48,  19,  73,  72,  6,   35,
        38,  35,  35,  53,  67,  185, 5,   109, 42,  7,   37,  21,  20,  6,
        7,   22,  22,  130, 5,   8,   103, 35,  34,  154, 155, 100, 68,  84,
        113, 135, 49,  53,  122, 122, 74,  55,  123, 155, 153, 139, 148, 146,
        118, 118, 129, 113, 156, 150, 142, 2,   243, 142, 58,  60,  46,  55,
        73,  124, 58,  57,  42,  40,  58,  136, 46,  106, 78,  71,  100, 29,
        16,  114, 71,  82,  136, 0,   0,   3,   0,   37,  255, 219, 6,   6,
        5,   237, 0,   5,   0,   35,  0,   39,  0,   165, 64,  62,  38,  37,
        36,  36,  85,  39,  38,  20,  39,  39,  38,  7,   33,  6,   3,   4,
        5,   0,   24,  38,  37,  2,   94,  4,   27,  37,  27,  20,  107, 130,
        229, 32,  12,  130, 238, 56,  31,  36,  39,  108, 33,  85,  6,   28,
        39,  29,  41,  71,  71,  74,  35,  20,  33,  131, 6,   254, 35,  37,
        38,  193, 35,  134, 253, 52,  35,  184, 1,   63,  180, 0,   39,  36,
        224, 5,   184, 2,   61,  64,  9,   0,   171, 3,   2,   73,  40,  98,
        27,  6,   41,  60,  77,  244, 237, 25,  244, 50,  24,  16,  253, 130,
        7,   32,  16,  132, 8,   32,  237, 71,  241, 8,   36,  63,  77,  237,
        244, 60,  65,  205, 12,  38,  63,  237, 16,  60,  63,  60,  17,  112,
        252, 7,   37,  135, 125, 196, 49,  48,  19,  66,  79,  5,   65,  226,
        5,   32,  55,  75,  20,  5,   41,  7,   53,  54,  51,  50,  22,  21,
        20,  7,   6,   65,  228, 5,   8,   39,  5,   1,   51,  1,   222, 136,
        1,   53,  2,   18,  35,  75,  77,  120, 68,  23,  22,  85,  85,  118,
        132, 149, 137, 151, 168, 46,  46,  154, 64,  95,  36,  1,   197, 250,
        31,  4,   74,  179, 251, 195, 66,  144, 7,   8,   156, 253, 28,  136,
        63,  69,  51,  80,  45,  34,  33,  39,  55,  55,  64,  133, 48,  120,
        94,  74,  62,  62,  82,  34,  51,  62,  136, 37,  6,   18,  249, 238,
        0,   4,   0,   99,  255, 219, 5,   238, 5,   237, 0,   5,   0,   16,
        0,   19,  0,   23,  0,   200, 64,  90,  15,  8,   14,  11,  16,  7,
        8,   14,  6,   19,  12,  17,  13,  11,  16,  18,  17,  13,  6,   19,
        10,  9,   9,   98,  17,  19,  20,  17,  17,  19,  21,  20,  20,  85,
        23,  22,  20,  23,  23,  22,  3,   2,   4,   19,  9,   10,  17,  10,
        11,  31,  17,  13,  98,  8,   14,  224, 6,   20,  115, 23,  29,  16,
        6,   28,  5,   0,   24,  22,  21,  27,  2,   107, 3,   100, 4,   27,
        10,  19,  9,   21,  22,  115, 16,  9,   17,  94,  8,   182, 19,  6,
        184, 2,   61,  64,  10,  11,  14,  171, 16,  74,  25,  23,  20,  171,
        67,  108, 8,   42,  24,  86,  185, 1,   24,  0,   24,  43,  78,  16,
        244, 65,  70,  6,   46,  78,  16,  246, 77,  228, 60,  253, 60,  244,
        237, 60,  25,  16,  244, 50,  116, 244, 5,   35,  244, 237, 63,  60,
        132, 1,   34,  228, 16,  244, 130, 27,  33,  63,  60,  130, 22,  65,
        65,  7,   40,  125, 16,  196, 135, 4,   46,  24,  43,  5,   123, 83,
        6,   65,  78,  9,   40,  33,  53,  1,   51,  17,  51,  21,  35,  21,
        130, 6,   58,  1,   1,   51,  1,   235, 136, 1,   52,  3,   47,  254,
        142, 1,   95,  191, 124, 124, 254, 107, 233, 251, 157, 4,   67,  192,
        251, 189, 65,  49,  9,   45,  197, 155, 1,   145, 254, 80,  124, 197,
        1,   65,  1,   10,  253, 145, 90,  225, 5,   8,   87,  4,   0,   123,
        255, 219, 6,   129, 5,   238, 0,   36,  0,   47,  0,   50,  0,   54,
        0,   254, 185, 0,   32,  255, 255, 64,  86,  41,  50,  48,  48,  98,
        40,  41,  20,  40,  40,  41,  52,  51,  51,  85,  54,  53,  20,  54,
        54,  53,  40,  50,  42,  48,  29,  11,  10,  44,  48,  98,  45,  39,
        224, 37,  41,  42,  31,  37,  28,  51,  115, 54,  29,  53,  52,  27,
        10,  107, 11,  11,  23,  73,  3,   144, 35,  1,   94,  64,  0,   108,
        14,  67,  102, 9,   8,   35,  14,  23,  27,  52,  53,  156, 47,  40,
        48,  94,  39,  182, 50,  37,  184, 2,   61,  64,  18,  47,  41,  148,
        42,  42,  45,  171, 16,  47,  1,   47,  74,  56,  54,  51,  156, 0,
        66,  135, 6,   51,  6,   186, 2,   60,  0,   32,  1,   99,  64,  10,
        20,  11,  156, 16,  0,   1,   0,   73,  55,  112, 65,  64,  8,   37,
        93,  77,  228, 60,  253, 237, 66,  132, 6,   34,  78,  16,  246, 131,
        15,  34,  16,  230, 16,  65,  74,  8,   34,  0,   24,  63,  67,  155,
        12,  139, 12,  46,  118, 18,  57,  47,  24,  237, 63,  60,  63,  228,
        63,  63,  60,  16,  244, 97,  157, 5,   35,  17,  18,  57,  57,  84,
        77,  6,   40,  135, 46,  24,  43,  135, 125, 196, 1,   56,  67,  189,
        38,  33,  1,   53,  65,  124, 15,  32,  123, 67,  207, 24,  50,  4,
        78,  254, 142, 1,   96,  191, 123, 123, 254, 106, 233, 251, 194, 4,
        68,  191, 251, 189, 67,  226, 25,  33,  253, 53,  65,  164, 18,  40,
        1,   0,   111, 2,   31,  2,   43,  2,   179, 24,  147, 51,  44,  8,
        116, 2,   0,   98,  0,   191, 4,   173, 5,   10,  0,   31,  0,   47,
        0,   246, 64,  86,  89,  0,   86,  10,  86,  16,  89,  26,  4,   88,
        3,   88,  7,   87,  19,  87,  23,  4,   10,  23,  24,  24,  9,   7,
        26,  25,  25,  8,   19,  0,   1,   1,   18,  16,  3,   2,   17,  17,
        18,  1,   1,   98,  2,   17,  20,  2,   2,   17,  25,  24,  9,   9,
        98,  8,   25,  20,  8,   8,   25,  7,   10,  19,  16,  3,   0,   26,
        23,  8,   29,  13,  25,  24,  1,   2,   17,  18,  9,   8,   32,  88,
        29,  64,  24,  2,   115, 29,  184, 1,   107, 64,  31,  13,  192, 18,
        8,   100, 40,  88,  13,  7,   136, 40,  33,  21,  5,   132, 40,  8,
        47,  9,   8,   24,  18,  156, 36,  190, 2,   63,  0,   64,  0,   21,
        1,   107, 0,   192, 0,   44,  2,   63,  64,  10,  5,   2,   8,   156,
        5,   148, 48,  86,  158, 24,  43,  16,  246, 244, 60,  16,  237, 26,
        253, 26,  237, 244, 60,  17,  51,  71,  21,  5,   44,  18,  23,  57,
        0,   63,  237, 25,  244, 60,  26,  24,  16,  253, 133, 6,   74,  199,
        5,   32,  51,  107, 38,  9,   34,  135, 14,  125, 117, 86,  5,   74,
        194, 8,   36,  14,  16,  60,  60,  7,   136, 4,   8,   40,  49,  48,
        1,   93,  0,   93,  1,   7,   39,  55,  38,  53,  52,  55,  39,  55,
        23,  54,  54,  51,  50,  22,  23,  55,  23,  7,   22,  21,  20,  7,
        23,  7,   39,  6,   6,   35,  34,  38,  55,  50,  54,  24,  66,  233,
        12,  38,  1,   124, 195, 87,  195, 90,  90,  130, 4,   42,  63,  133,
        71,  71,  134, 62,  195, 88,  195, 89,  89,  130, 4,   50,  62,  134,
        71,  71,  133, 204, 84,  133, 79,  77,  135, 84,  84,  135, 77,  79,
        133, 1,   130, 130, 35,  45,  119, 148, 148, 119, 196, 87,  195, 45,
        44,  44,  45,  195, 87,  196, 131, 13,  130, 20,  51,  45,  45,  45,
        103, 77,  141, 78,  77,  140, 79,  79,  140, 77,  78,  141, 77,  0,
        0,   2,   0,   132, 0,   35,  255, 56,  0,   100, 132, 8,   143, 4,
        33,  243, 1,   133, 33,  8,   192, 3,   0,   4,   0,   5,   0,   6,
        0,   7,   0,   8,   0,   9,   0,   10,  0,   11,  0,   12,  0,   13,
        0,   14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,
        0,   21,  0,   22,  0,   23,  0,   24,  0,   25,  0,   26,  0,   27,
        0,   28,  0,   29,  0,   30,  0,   31,  0,   32,  0,   33,  0,   34,
        0,   35,  0,   36,  0,   37,  0,   38,  0,   39,  0,   40,  0,   41,
        0,   42,  0,   43,  0,   44,  0,   45,  0,   46,  0,   47,  0,   48,
        0,   49,  0,   50,  0,   51,  0,   52,  0,   53,  0,   54,  0,   55,
        0,   56,  0,   57,  0,   58,  0,   59,  0,   60,  0,   61,  0,   62,
        0,   63,  0,   64,  0,   65,  0,   66,  0,   67,  0,   68,  0,   69,
        0,   70,  0,   71,  0,   72,  0,   73,  0,   74,  0,   75,  0,   76,
        0,   77,  0,   78,  0,   79,  0,   80,  0,   81,  0,   82,  0,   83,
        0,   84,  0,   85,  0,   86,  0,   87,  0,   88,  0,   89,  0,   90,
        0,   91,  0,   92,  0,   93,  0,   94,  0,   95,  0,   96,  0,   97,
        0,   98,  0,   99,  130, 223, 9,   50,  101, 0,   102, 0,   103, 0,
        104, 0,   105, 0,   106, 0,   107, 0,   108, 0,   109, 0,   110, 0,
        111, 0,   112, 0,   113, 0,   114, 0,   115, 0,   116, 0,   117, 0,
        118, 0,   119, 0,   120, 0,   121, 0,   122, 0,   123, 0,   124, 0,
        125, 0,   126, 0,   127, 0,   128, 0,   129, 0,   130, 0,   131, 0,
        132, 0,   133, 0,   134, 0,   135, 0,   136, 0,   137, 0,   138, 0,
        139, 0,   140, 0,   141, 0,   142, 0,   143, 0,   144, 0,   145, 0,
        146, 0,   147, 0,   148, 0,   149, 0,   150, 0,   151, 0,   152, 0,
        153, 0,   154, 0,   155, 0,   156, 0,   157, 0,   158, 0,   159, 0,
        160, 0,   161, 0,   162, 0,   163, 0,   164, 0,   165, 0,   166, 0,
        167, 0,   168, 0,   169, 0,   170, 0,   171, 0,   172, 0,   173, 0,
        174, 0,   175, 0,   176, 0,   177, 0,   178, 0,   179, 0,   180, 0,
        181, 0,   182, 0,   183, 0,   184, 0,   185, 0,   186, 0,   187, 0,
        188, 1,   3,   0,   190, 0,   191, 0,   192, 0,   193, 0,   194, 0,
        195, 0,   196, 0,   197, 0,   198, 0,   199, 0,   200, 0,   201, 0,
        202, 0,   203, 0,   204, 0,   205, 0,   206, 0,   207, 0,   208, 0,
        209, 0,   211, 0,   212, 0,   213, 0,   214, 0,   215, 0,   216, 0,
        217, 0,   218, 0,   219, 0,   220, 0,   221, 0,   222, 0,   223, 0,
        224, 0,   225, 0,   228, 0,   229, 0,   232, 0,   233, 0,   234, 0,
        235, 0,   236, 0,   237, 0,   238, 0,   240, 0,   241, 0,   242, 0,
        243, 0,   244, 0,   245, 0,   246, 1,   4,   0,   189, 4,   102, 48,
        48,  48,  4,   69,  117, 114, 111, 9,   115, 102, 116, 104, 121, 112,
        104, 101, 110, 0,   0,   0,   1,   132, 1,   131, 7,   34,  0,   20,
        234, 130, 15,  32,  20,  130, 3,   130, 2,   52,  20,  226, 48,  130,
        20,  222, 6,   9,   42,  134, 72,  134, 247, 13,  1,   7,   2,   160,
        130, 20,  207, 130, 18,  41,  203, 2,   1,   1,   49,  14,  48,  12,
        6,   8,   133, 25,  55,  2,   5,   5,   0,   48,  96,  6,   10,  43,
        6,   1,   4,   1,   130, 55,  2,   1,   4,   160, 82,  48,  80,  48,
        44,  138, 17,  38,  28,  162, 30,  128, 28,  0,   60,  132, 1,   44,
        79,  0,   98,  0,   115, 0,   111, 0,   108, 0,   101, 0,   116, 130,
        3,   33,  62,  0,   130, 1,   33,  48,  32,  141, 79,  8,   49,  4,
        16,  199, 43,  99,  95,  0,   107, 5,   187, 155, 177, 108, 109, 215,
        87,  61,  147, 160, 130, 16,  145, 48,  130, 2,   64,  48,  130, 1,
        169, 2,   16,  3,   199, 143, 55,  219, 146, 40,  223, 60,  187, 26,
        173, 130, 250, 103, 16,  48,  13,  136, 167, 33,  1,   2,   130, 142,
        55,  97,  49,  17,  48,  15,  6,   3,   85,  4,   7,   19,  8,   73,
        110, 116, 101, 114, 110, 101, 116, 49,  23,  48,  21,  131, 18,  52,
        10,  19,  14,  86,  101, 114, 105, 83,  105, 103, 110, 44,  32,  73,
        110, 99,  46,  49,  51,  48,  49,  131, 24,  34,  11,  19,  42,  135,
        24,  8,   44,  32,  67,  111, 109, 109, 101, 114, 99,  105, 97,  108,
        32,  83,  111, 102, 116, 119, 97,  114, 101, 32,  80,  117, 98,  108,
        105, 115, 104, 101, 114, 115, 32,  67,  65,  48,  30,  23,  13,  57,
        54,  48,  52,  48,  57,  48,  132, 0,   34,  90,  23,  13,  130, 12,
        42,  49,  48,  55,  50,  51,  53,  57,  53,  57,  90,  48,  226, 130,
        33,  129, 159, 139, 247, 8,   146, 1,   5,   0,   3,   129, 141, 0,
        48,  129, 137, 2,   129, 129, 0,   195, 211, 105, 101, 82,  1,   148,
        84,  171, 40,  198, 98,  24,  179, 84,  85,  197, 68,  135, 69,  74,
        59,  194, 126, 216, 211, 215, 200, 128, 134, 141, 216, 12,  241, 22,
        156, 204, 107, 169, 41,  178, 143, 118, 115, 146, 200, 197, 98,  166,
        60,  237, 30,  5,   117, 240, 19,  0,   108, 20,  77,  212, 152, 144,
        7,   190, 105, 115, 129, 184, 98,  78,  49,  30,  209, 252, 201, 12,
        235, 125, 144, 191, 174, 180, 71,  81,  236, 111, 206, 100, 53,  2,
        214, 125, 103, 5,   119, 226, 143, 217, 81,  215, 251, 151, 25,  188,
        62,  215, 119, 129, 198, 67,  221, 242, 221, 223, 202, 163, 131, 139,
        203, 65,  193, 61,  34,  72,  72,  166, 25,  2,   3,   1,   0,   1,
        65,  150, 14,  32,  3,   130, 151, 8,   131, 181, 188, 176, 117, 106,
        137, 162, 134, 189, 100, 120, 195, 167, 50,  117, 114, 17,  170, 38,
        2,   23,  96,  48,  76,  227, 72,  52,  25,  185, 82,  74,  81,  24,
        128, 254, 83,  45,  123, 213, 49,  140, 197, 101, 153, 65,  65,  47,
        242, 174, 99,  122, 232, 115, 153, 21,  144, 26,  31,  122, 139, 65,
        208, 142, 58,  208, 205, 56,  52,  68,  208, 117, 248, 234, 113, 196,
        129, 25,  56,  23,  53,  74,  174, 197, 62,  50,  230, 33,  184, 5,
        192, 147, 225, 199, 56,  92,  216, 247, 147, 56,  100, 144, 237, 84,
        206, 202, 211, 211, 208, 95,  239, 4,   155, 222, 2,   130, 221, 136,
        41,  177, 195, 79,  165, 205, 113, 100, 49,  60,  60,  48,  130, 2,
        192, 130, 3,   54,  41,  2,   20,  19,  137, 180, 209, 138, 232, 167,
        196, 189, 53,  199, 155, 141, 136, 202, 31,  202, 83,  86,  145, 139,
        176, 41,  4,   5,   0,   48,  129, 158, 49,  31,  48,  29,  66,  53,
        5,   32,  22,  66,  28,  8,   44,  84,  114, 117, 115, 116, 32,  78,
        101, 116, 119, 111, 114, 107, 66,  86,  7,   32,  11,  66,  86,  16,
        34,  44,  48,  42,  131, 57,  34,  11,  19,  35,  137, 57,  60,  105,
        109, 101, 32,  83,  116, 97,  109, 112, 105, 110, 103, 32,  83,  101,
        114, 118, 105, 99,  101, 32,  82,  111, 111, 116, 49,  52,  48,  50,
        133, 45,  61,  43,  78,  79,  32,  76,  73,  65,  66,  73,  76,  73,
        84,  89,  32,  65,  67,  67,  69,  80,  84,  69,  68,  44,  32,  40,
        99,  41,  57,  55,  32,  66,  186, 13,  44,  48,  30,  23,  13,  57,
        55,  48,  53,  49,  50,  48,  55,  48,  130, 0,   32,  90,  130, 14,
        36,  57,  49,  50,  51,  49,  134, 14,  64,  192, 161, 66,  195, 27,
        8,   127, 211, 46,  32,  240, 104, 124, 44,  45,  46,  129, 28,  177,
        6,   178, 167, 11,  183, 17,  13,  87,  218, 83,  216, 117, 227, 201,
        51,  42,  178, 212, 246, 9,   91,  52,  243, 233, 144, 254, 9,   12,
        208, 219, 27,  90,  185, 205, 231, 246, 136, 177, 157, 192, 135, 37,
        235, 125, 88,  16,  115, 106, 120, 203, 113, 21,  253, 198, 88,  246,
        41,  171, 88,  94,  150, 4,   253, 45,  98,  17,  88,  129, 28,  202,
        113, 148, 213, 34,  88,  47,  213, 204, 20,  5,   132, 54,  186, 148,
        170, 180, 77,  74,  233, 238, 59,  34,  173, 86,  153, 126, 33,  156,
        108, 134, 192, 74,  71,  151, 106, 180, 166, 54,  213, 252, 9,   45,
        211, 180, 57,  155, 66,  195, 16,  8,   138, 4,   5,   0,   3,   129,
        129, 0,   58,  17,  156, 133, 5,   62,  210, 233, 128, 251, 123, 213,
        169, 244, 172, 121, 252, 5,   252, 149, 61,  113, 35,  169, 43,  40,
        223, 140, 19,  101, 137, 254, 44,  135, 1,   143, 90,  154, 98,  202,
        17,  167, 128, 244, 183, 190, 20,  183, 209, 86,  153, 107, 8,   98,
        69,  198, 162, 165, 218, 53,  127, 5,   34,  222, 114, 45,  4,   134,
        5,   167, 124, 9,   22,  147, 20,  67,  240, 247, 22,  77,  214, 7,
        142, 155, 16,  108, 88,  254, 10,  53,  151, 202, 137, 159, 223, 4,
        112, 156, 42,  125, 97,  142, 193, 232, 11,  113, 154, 168, 199, 102,
        98,  66,  61,  149, 148, 34,  50,  152, 34,  137, 138, 250, 100, 8,
        36,  245, 210, 250, 48,  130, 2,   205, 130, 3,   55,  54,  2,   21,
        0,   189, 17,  154, 218, 67,  237, 33,  251, 70,  88,  132, 137, 202,
        70,  136, 144, 37,  238, 20,  96,  66,  196, 209, 36,  172, 49,  39,
        48,  37,  67,  29,  5,   32,  30,  67,  75,  29,  67,  174, 7,   33,
        11,  19,  67,  174, 23,  67,  103, 52,  70,  59,  25,  70,  103, 17,
        34,  48,  129, 157, 65,  129, 11,  8,   144, 1,   5,   0,   3,   129,
        139, 0,   48,  129, 135, 2,   129, 129, 0,   171, 97,  237, 180, 173,
        141, 144, 71,  144, 220, 180, 17,  94,  105, 220, 10,  127, 98,  144,
        6,   49,  205, 206, 255, 136, 145, 70,  215, 73,  58,  148, 233, 212,
        6,   63,  157, 173, 162, 120, 90,  236, 249, 252, 99,  69,  79,  184,
        11,  110, 48,  239, 162, 54,  171, 45,  9,   223, 241, 111, 39,  171,
        13,  81,  96,  5,   53,  79,  127, 206, 84,  79,  208, 183, 44,  66,
        216, 11,  208, 141, 184, 94,  255, 182, 128, 208, 227, 150, 180, 127,
        34,  73,  66,  16,  108, 211, 152, 176, 1,   86,  163, 195, 207, 46,
        159, 58,  244, 127, 168, 88,  166, 215, 34,  101, 229, 140, 171, 120,
        156, 188, 217, 71,  66,  104, 91,  45,  125, 253, 181, 2,   1,   3,
        66,  30,  14,  32,  3,   130, 149, 8,   131, 109, 96,  251, 153, 95,
        164, 105, 179, 211, 123, 112, 43,  98,  35,  30,  68,  32,  81,  175,
        35,  21,  199, 116, 2,   249, 73,  242, 39,  26,  92,  172, 134, 113,
        53,  8,   43,  246, 143, 222, 224, 181, 150, 232, 139, 167, 75,  227,
        115, 200, 72,  9,   157, 176, 218, 139, 218, 21,  146, 202, 3,   229,
        9,   37,  86,  6,   231, 78,  164, 71,  165, 209, 87,  70,  212, 56,
        86,  245, 33,  205, 195, 38,  59,  45,  37,  50,  206, 155, 226, 191,
        64,  71,  234, 216, 109, 71,  118, 229, 192, 48,  163, 15,  128, 206,
        127, 216, 59,  126, 160, 249, 149, 42,  49,  43,  21,  250, 200, 20,
        237, 221, 176, 233, 85,  65,  112, 70,  45,  44,  126, 48,  130, 8,
        180, 130, 3,   55,  29,  160, 3,   2,   1,   2,   2,   16,  85,  13,
        136, 245, 63,  100, 22,  215, 12,  115, 0,   216, 69,  146, 22,  52,
        139, 177, 71,  221, 106, 35,  57,  48,  51,  49,  71,  221, 10,  36,
        48,  48,  52,  49,  54,  71,  221, 7,   34,  130, 1,   93,  72,  98,
        96,  35,  49,  70,  48,  68,  66,  244, 5,   8,   65,  61,  119, 119,
        119, 46,  118, 101, 114, 105, 115, 105, 103, 110, 46,  99,  111, 109,
        47,  114, 101, 112, 111, 115, 105, 116, 111, 114, 121, 47,  82,  80,
        65,  32,  73,  110, 99,  111, 114, 112, 46,  32,  98,  121, 32,  82,
        101, 102, 46,  44,  76,  73,  65,  66,  46,  76,  84,  68,  40,  99,
        41,  57,  56,  49,  62,  48,  60,  133, 71,  62,  53,  68,  105, 103,
        105, 116, 97,  108, 32,  73,  68,  32,  67,  108, 97,  115, 115, 32,
        51,  32,  45,  32,  77,  105, 99,  114, 111, 115, 111, 102, 116, 72,
        234, 9,   48,  86,  97,  108, 105, 100, 97,  116, 105, 111, 110, 32,
        118, 50,  49,  11,  48,  9,   131, 63,  40,  6,   19,  2,   85,  83,
        49,  19,  48,  17,  131, 12,  48,  8,   19,  10,  87,  97,  115, 104,
        105, 110, 103, 116, 111, 110, 49,  16,  48,  14,  73,  109, 5,   43,
        7,   82,  101, 100, 109, 111, 110, 100, 49,  30,  48,  28,  131, 17,
        34,  3,   20,  21,  137, 94,  32,  67,  130, 163, 33,  111, 114, 132,
        86,  135, 31,  32,  11,  150, 31,  67,  68,  28,  8,   140, 164, 31,
        99,  3,   71,  120, 140, 168, 188, 86,  153, 226, 86,  86,  22,  7,
        90,  23,  80,  198, 48,  222, 239, 41,  244, 250, 184, 222, 116, 161,
        111, 174, 91,  41,  10,  71,  31,  126, 208, 167, 84,  194, 222, 71,
        205, 186, 116, 43,  6,   212, 133, 34,  49,  29,  248, 6,   89,  154,
        11,  55,  91,  158, 249, 68,  183, 63,  92,  222, 173, 157, 64,  96,
        39,  90,  24,  87,  74,  25,  115, 56,  118, 54,  238, 177, 57,  32,
        5,   160, 43,  201, 84,  60,  49,  184, 10,  178, 34,  251, 200, 101,
        156, 163, 236, 71,  66,  232, 204, 142, 204, 103, 63,  108, 77,  34,
        114, 201, 74,  214, 216, 222, 146, 184, 238, 28,  8,   248, 119, 231,
        2,   1,   3,   163, 130, 5,   111, 48,  130, 5,   107, 48,  9,   130,
        229, 39,  29,  19,  4,   2,   48,  0,   48,  11,  131, 10,  41,  15,
        4,   4,   3,   2,   5,   160, 48,  129, 148, 131, 13,  58,  1,   4,
        129, 140, 48,  129, 137, 128, 16,  123, 150, 228, 209, 67,  253, 104,
        152, 243, 56,  204, 110, 59,  242, 11,  130, 161, 99,  74,  32,  98,
        32,  130, 75,  39,  17,  32,  33,  131, 149, 43,  4,   1,   1,   255,
        4,   23,  48,  20,  48,  14,  48,  12,  75,  165, 10,  39,  22,  3,
        2,   7,   128, 0,   48,  13,  131, 34,  35,  10,  4,   6,   48,  130,
        200, 37,  6,   64,  48,  130, 4,   54,  138, 35,  32,  10,  131, 58,
        34,  130, 4,   35,  130, 22,  44,  31,  160, 41,  128, 39,  104, 116,
        116, 112, 115, 58,  47,  47,  66,  160, 27,  62,  67,  80,  83,  161,
        130, 3,   184, 129, 130, 3,   180, 84,  104, 105, 115, 32,  99,  101,
        114, 116, 105, 102, 105, 99,  97,  116, 101, 32,  105, 110, 99,  66,
        20,  6,   58,  101, 115, 32,  98,  121, 32,  114, 101, 102, 101, 114,
        101, 110, 99,  101, 44,  32,  97,  110, 100, 32,  105, 116, 115, 32,
        117, 115, 130, 38,  57,  115, 32,  115, 116, 114, 105, 99,  116, 108,
        121, 10,  115, 117, 98,  106, 101, 99,  116, 32,  116, 111, 44,  32,
        116, 104, 101, 73,  17,  8,   33,  32,  67,  136, 87,  46,  105, 111,
        110, 32,  80,  114, 97,  99,  116, 105, 99,  101, 32,  83,  116, 130,
        104, 37,  109, 101, 110, 116, 32,  40,  130, 137, 37,  41,  10,  118,
        101, 114, 115, 131, 32,  34,  49,  46,  48,  130, 100, 38,  118, 97,
        105, 108, 97,  98,  108, 130, 98,  32,  110, 141, 77,  67,  97,  9,
        36,  32,  97,  116, 58,  10,  151, 232, 32,  59,  131, 181, 34,  69,
        45,  109, 130, 70,  130, 39,  32,  32,  130, 97,  41,  45,  114, 101,
        113, 117, 101, 115, 116, 115, 64,  67,  177, 11,  36,  59,  32,  111,
        114, 10,  130, 224, 135, 40,  73,  199, 13,  8,   45,  44,  32,  50,
        53,  57,  51,  32,  67,  111, 97,  115, 116, 32,  65,  118, 101, 46,
        44,  32,  77,  111, 117, 110, 116, 97,  105, 110, 32,  86,  105, 101,
        119, 44,  32,  67,  65,  32,  57,  52,  48,  52,  51,  10,  85,  83,
        65,  130, 39,  37,  112, 121, 114, 105, 103, 104, 130, 210, 37,  99,
        41,  49,  57,  57,  54,  65,  4,   8,   44,  44,  32,  73,  110, 99,
        46,  32,  32,  65,  108, 108, 32,  82,  131, 33,  35,  115, 32,  82,
        101, 24,  241, 48,  7,   8,   32,  67,  69,  82,  84,  65,  73,  78,
        10,  87,  65,  82,  82,  65,  78,  84,  73,  69,  83,  32,  68,  73,
        83,  67,  76,  65,  73,  77,  69,  68,  32,  65,  78,  68,  74,  119,
        10,  40,  76,  73,  77,  73,  84,  69,  68,  46,  10,  131, 45,  47,
        78,  73,  78,  71,  58,  32,  84,  72,  69,  32,  85,  83,  69,  32,
        79,  70,  130, 10,  34,  73,  83,  32,  131, 78,  39,  73,  70,  73,
        67,  65,  84,  69,  32,  130, 14,  50,  83,  84,  82,  73,  67,  84,
        76,  89,  32,  83,  85,  66,  74,  69,  67,  84,  32,  84,  79,  130,
        39,  41,  69,  10,  86,  69,  82,  73,  83,  73,  71,  78,  138, 47,
        53,  73,  79,  78,  32,  80,  82,  65,  67,  84,  73,  67,  69,  32,
        83,  84,  65,  84,  69,  77,  69,  78,  84,  130, 182, 131, 98,  35,
        73,  83,  83,  85,  130, 111, 42,  32,  65,  85,  84,  72,  79,  82,
        73,  84,  89,  10,  135, 164, 133, 114, 130, 193, 37,  32,  73,  77,
        80,  76,  73,  134, 179, 39,  69,  88,  80,  82,  69,  83,  83,  32,
        137, 213, 39,  44,  32,  73,  78,  67,  76,  85,  68,  131, 69,  137,
        21,  32,  10,  130, 183, 36,  77,  69,  82,  67,  72,  130, 250, 75,
        93,  7,   38,  79,  82,  32,  70,  73,  84,  78,  131, 62,  32,  70,
        130, 11,  8,   44,  65,  32,  80,  65,  82,  84,  73,  67,  85,  76,
        65,  82,  32,  80,  85,  82,  80,  79,  83,  69,  44,  32,  65,  78,
        68,  32,  87,  73,  76,  76,  32,  78,  79,  84,  10,  66,  69,  32,
        76,  73,  65,  66,  76,  69,  32,  131, 48,  38,  67,  79,  78,  83,
        69,  81,  85,  130, 188, 35,  73,  65,  76,  44,  130, 50,  36,  78,
        73,  84,  73,  86,  134, 51,  65,  110, 6,   33,  32,  79,  130, 213,
        46,  82,  32,  68,  65,  77,  65,  71,  69,  83,  46,  32,  83,  69,
        69,  10,  130, 17,  35,  32,  67,  80,  83,  132, 67,  52,  68,  69,
        84,  65,  73,  76,  83,  46,  10,  10,  67,  111, 110, 116, 101, 110,
        116, 115, 32,  111, 102, 66,  139, 15,  52,  103, 105, 115, 116, 101,
        114, 101, 100, 32,  110, 111, 110, 118, 101, 114, 105, 102, 105, 101,
        100, 83,  67,  8,   5,   44,  65,  116, 116, 114, 105, 98,  117, 116,
        101, 115, 10,  101, 120, 130, 63,  47,  115, 105, 111, 110, 32,  118,
        97,  108, 117, 101, 32,  115, 104, 97,  108, 108, 130, 50,  42,  116,
        32,  98,  101, 32,  99,  111, 110, 115, 105, 100, 132, 68,  39,  97,
        115, 32,  97,  99,  99,  117, 114, 67,  129, 5,   37,  102, 111, 114,
        109, 97,  116, 130, 53,  32,  10,  130, 53,  33,  105, 100, 130, 20,
        35,  100, 32,  98,  121, 132, 129, 39,  73,  65,  46,  10,  163, 54,
        128, 52,  66,  253, 23,  70,  135, 11,  66,  237, 7,   58,  108, 111,
        103, 111, 46,  103, 105, 102, 48,  54,  6,   3,   85,  29,  3,   4,
        47,  48,  45,  48,  43,  160, 41,  160, 39,  134, 37,  131, 70,  40,
        58,  47,  47,  115, 116, 97,  116, 117, 115, 70,  208, 13,  43,  99,
        108, 97,  115, 115, 49,  46,  99,  114, 108, 48,  22,  68,  111, 10,
        39,  27,  4,   8,   48,  6,   1,   1,   255, 130, 2,   72,  5,   14,
        8,   144, 3,   129, 129, 0,   85,  215, 152, 205, 84,  74,  252, 48,
        237, 180, 209, 212, 248, 1,   204, 170, 141, 22,  247, 231, 113, 241,
        74,  112, 248, 92,  56,  46,  21,  228, 70,  15,  246, 82,  66,  174,
        167, 222, 0,   109, 114, 97,  52,  102, 196, 85,  197, 85,  193, 208,
        188, 91,  1,   77,  163, 18,  242, 63,  156, 152, 196, 108, 81,  94,
        55,  223, 158, 173, 64,  112, 136, 60,  0,   234, 168, 60,  12,  42,
        241, 46,  111, 170, 239, 234, 109, 127, 135, 157, 236, 154, 155, 26,
        56,  179, 174, 108, 228, 75,  28,  79,  224, 88,  28,  206, 221, 17,
        181, 216, 151, 254, 81,  130, 252, 214, 236, 233, 206, 225, 150, 94,
        33,  189, 246, 225, 220, 172, 122, 88,  49,  130, 3,   189, 48,  130,
        3,   185, 2,   1,   1,   48,  117, 69,  208, 98,  73,  26,  18,  81,
        134, 12,  36,  160, 129, 200, 48,  25,  81,  9,   8,   34,  9,   3,
        49,  69,  222, 11,  34,  4,   48,  28,  65,  88,  10,  33,  11,  49,
        69,  252, 14,  33,  48,  31,  137, 56,  54,  4,   49,  18,  4,   16,
        17,  122, 246, 0,   209, 125, 6,   162, 33,  125, 192, 129, 245, 180,
        138, 178, 48,  108, 138, 62,  43,  12,  49,  94,  48,  92,  160, 50,
        128, 48,  0,   76,  0,   24,  244, 176, 19,  38,  32,  0,   82,  0,
        101, 0,   103, 130, 27,  50,  108, 0,   97,  0,   114, 0,   32,  0,
        70,  0,   111, 0,   110, 0,   116, 161, 38,  128, 36,  66,  6,   6,
        36,  119, 119, 119, 46,  109, 72,  123, 7,   47,  46,  99,  111, 109,
        47,  116, 121, 112, 111, 103, 114, 97,  112, 104, 121, 32,  75,  66,
        14,  8,   138, 4,   129, 128, 78,  126, 200, 24,  97,  35,  141, 102,
        140, 29,  218, 111, 155, 113, 139, 189, 164, 233, 215, 38,  4,   221,
        177, 59,  55,  85,  213, 46,  183, 182, 126, 175, 97,  221, 122, 25,
        177, 151, 226, 206, 168, 100, 90,  78,  206, 100, 240, 240, 132, 179,
        51,  40,  184, 116, 27,  122, 199, 214, 50,  77,  204, 33,  11,  12,
        154, 215, 250, 143, 240, 83,  141, 25,  6,   33,  141, 240, 79,  175,
        149, 74,  225, 88,  61,  212, 198, 223, 178, 122, 148, 141, 233, 122,
        191, 11,  255, 162, 41,  42,  152, 90,  102, 63,  236, 51,  44,  236,
        21,  204, 149, 245, 30,  189, 85,  89,  8,   203, 203, 180, 8,   105,
        35,  217, 227, 224, 103, 163, 135, 172, 161, 130, 1,   208, 48,  130,
        1,   204, 65,  38,  9,   36,  6,   49,  130, 1,   189, 130, 18,  38,
        185, 2,   1,   1,   48,  129, 184, 79,  108, 160, 78,  47,  23,  66,
        67,  13,  34,  89,  48,  24,  137, 226, 34,  3,   49,  11,  136, 12,
        35,  7,   1,   48,  28,  136, 12,  50,  9,   5,   49,  15,  23,  13,
        57,  57,  49,  49,  49,  53,  49,  56,  48,  57,  50,  52,  90,  66,
        65,  16,  47,  152, 233, 178, 181, 33,  45,  219, 249, 234, 49,  238,
        97,  84,  212, 16,  244, 65,  211, 17,  8,   129, 99,  200, 117, 77,
        3,   102, 241, 137, 84,  198, 239, 98,  253, 231, 112, 97,  130, 229,
        75,  80,  83,  173, 139, 2,   98,  91,  189, 197, 209, 236, 214, 90,
        71,  196, 233, 102, 22,  243, 193, 240, 122, 154, 226, 232, 51,  231,
        90,  215, 171, 74,  83,  101, 178, 73,  94,  173, 55,  113, 170, 236,
        167, 56,  66,  28,  183, 157, 7,   254, 57,  161, 94,  240, 69,  99,
        94,  76,  73,  156, 166, 48,  156, 186, 89,  171, 10,  192, 238, 40,
        32,  165, 107, 24,  91,  165, 25,  79,  231, 67,  94,  105, 153, 118,
        123, 243, 136, 101, 102, 156, 114, 249, 154, 248, 217, 212, 26,  167,
        171, 71,  92,  164, 192, 142, 213, 241, 212, 70,  184, 50,  0,   0,
        5,   250, 109, 99,  174, 58,
};

static const char* GetDefaultCompressedFontDataTTF(int* out_size)
{
    *out_size = lsans_ttf_compressed_size;
    return (const char*)lsans_ttf_compressed_data;
}
#endif

#endif
