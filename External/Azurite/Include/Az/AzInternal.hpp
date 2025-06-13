// Copyright (c) 2025 Case Technologies

#pragma once
#ifndef AZ_DISABLE

#ifndef AZ_VERSION
#include <Az/Az.hpp>
#endif

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if (defined __SSE__ || defined __x86_64__ || defined _M_X64                   \
     || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)))                            \
        && !defined(AZ_DISABLE_SSE)
#define AZ_ENABLE_SSE
#include <immintrin.h>
#if (defined __AVX__ || defined __SSE4_2__)
#define AZ_ENABLE_SSE4_2
#include <nmmintrin.h>
#endif
#endif

#if defined(AZ_ENABLE_SSE4_2) && !defined(AZ_USE_LEGACY_CRC32_ADLER)           \
        && !defined(__EMSCRIPTEN__)
#define AZ_ENABLE_SSE4_2_CRC
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#if defined(_MSC_VER) && _MSC_VER >= 1922
#pragma warning(disable : 5054)
#endif
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#endif

#if defined(AZ_DEFINE_MATH_OPERATORS)                                          \
        && !defined(AZ_DEFINE_MATH_OPERATORS_AZPLEMENTED)
#error Please '#define AZ_DEFINE_MATH_OPERATORS' _BEFORE_ including imgui.h!
#endif

#ifdef AZ_DISABLE_FORMAT_STRING_FUNCTIONS
#error Use AZ_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#endif
#ifdef AZ_DISABLE_MATH_FUNCTIONS
#error Use AZ_DISABLE_DEFAULT_MATH_FUNCTIONS
#endif

#ifndef AZ_ENABLE_FREETYPE
#define AZ_ENABLE_STB_TRUETYPE
#endif

struct AzBitVector;
struct AzRect;
struct AzTextIndex;

struct AzDrawDataBuilder;
struct AzDrawListSharedData;
struct AzFontAtlasBuilder;
struct AzFontAtlasPostProcessData;
struct AzFontAtlasRectEntry;

struct AzBoxSelectState;
struct AzColorMod;
struct AzContext;
struct AzContextHook;
struct AzDataTypeInfo;
struct AzDeactivatedItemData;
struct AzDockContext;
struct AzDockRequest;
struct AzDockNode;
struct AzDockNodeSettings;
struct AzErrorRecoveryState;
struct AzGroupData;
struct AzInputTextState;
struct AzInputTextDeactivateData;
struct AzLastItemData;
struct AzLocEntry;
struct AzMenuColumns;
struct AzMultiSelectState;
struct AzMultiSelectTempData;
struct AzNavItemData;
struct AzMetricsConfig;
struct AzNextWindowData;
struct AzNextItemData;
struct AzOldColumnData;
struct AzOldColumns;
struct AzPopupData;
struct AzSettingsHandler;
struct AzStyleMod;
struct AzStyleVarInfo;
struct AzTabBar;
struct AzTabItem;
struct AzTable;
struct AzTableHeaderData;
struct AzTableColumn;
struct AzTableInstanceData;
struct AzTableTempData;
struct AzTableSettings;
struct AzTableColumnsSettings;
struct AzTreeNodeStackData;
struct AzTypingSelectState;
struct AzTypingSelectRequest;
struct AzWindow;
struct AzWindowDockStyle;
struct AzWindowTempData;
struct AzWindowSettings;

enum AzLocKey : int;
typedef int AzDataAuthority;
typedef int AzLayoutType;

typedef int AzActivateFlags;
typedef int AzDebugLogFlags;
typedef int AzFocusRequestFlags;
typedef int AzItemStatusFlags;
typedef int AzOldColumnFlags;
typedef int AzLogFlags;
typedef int AzNavRenderCursorFlags;
typedef int AzNavMoveFlags;
typedef int AzNextItemDataFlags;
typedef int AzNextWindowDataFlags;
typedef int AzScrollFlags;
typedef int AzSeparatorFlags;
typedef int AzTextFlags;
typedef int AzTooltipFlags;
typedef int AzTypingSelectFlags;
typedef int AzWindowRefreshFlags;

typedef AzS16 AzTableColumnIdx;
typedef AzU16 AzTableDrawChannelIdx;

#ifndef GAz
extern AZ_API AzContext* GAz;
#endif

#define AZ_PAYLOAD_TYPE_WINDOW "_AZWINDOW"

#ifndef AZ_DEBUG_PRINTF
#ifndef AZ_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#define AZ_DEBUG_PRINTF(_FMT, ...) printf(_FMT, __VA_ARGS__)
#else
#define AZ_DEBUG_PRINTF(_FMT, ...) ((void)0)
#endif
#endif

#define AZ_DEBUG_LOG_ERROR(...)                                                \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventError)                      \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
        else                                                                   \
            g.DebugLogSkippedErrors++;                                         \
    } while (0)
#define AZ_DEBUG_LOG_ACTIVEID(...)                                             \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventActiveId)                   \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_FOCUS(...)                                                \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventFocus)                      \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_POPUP(...)                                                \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventPopup)                      \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_NAV(...)                                                  \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventNav)                        \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_SELECTION(...)                                            \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventSelection)                  \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_CLIPPER(...)                                              \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventClipper)                    \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_IO(...)                                                   \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventIO)                         \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_FONT(...)                                                 \
    do                                                                         \
    {                                                                          \
        AzContext* g2 = GAz;                                                   \
        if (g2 && g2->DebugLogFlags & AzDebugLogFlags_EventFont)               \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_INPUTROUTING(...)                                         \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventInputRouting)               \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_DOCKING(...)                                              \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventDocking)                    \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)
#define AZ_DEBUG_LOG_VIEWPORT(...)                                             \
    do                                                                         \
    {                                                                          \
        if (g.DebugLogFlags & AzDebugLogFlags_EventViewport)                   \
            AZ_DEBUG_LOG(__VA_ARGS__);                                         \
    } while (0)

#define AZ_STATIC_ASSERT(_COND) static_assert(_COND, "")

#ifdef AZ_DEBUG_PARANOID
#define AZ_ASSERT_PARANOID(_EXPR) AZ_ASSERT(_EXPR)
#else
#define AZ_ASSERT_PARANOID(_EXPR)
#endif

#define AZ_PI 3.14159265358979323846f
#ifdef _WIN32
#define AZ_NEWLINE "\r\n"
#else
#define AZ_NEWLINE "\n"
#endif
#ifndef AZ_TABSIZE
#define AZ_TABSIZE (4)
#endif
#define AZ_MEMALIGN(_OFF, _ALIGN) (((_OFF) + ((_ALIGN) - 1)) & ~((_ALIGN) - 1))
#define AZ_F32_TO_INT8_UNBOUND(_VAL)                                           \
    ((int)((_VAL) * 255.0f + ((_VAL) >= 0 ? 0.5f : -0.5f)))
#define AZ_F32_TO_INT8_SAT(_VAL) ((int)(AzSaturate(_VAL) * 255.0f + 0.5f))
#define AZ_TRUNC(_VAL) ((float)(int)(_VAL))
#define AZ_ROUND(_VAL) ((float)(int)((_VAL) + 0.5f))
#define AZ_STRINGIFY_HELPER(_X) #_X
#define AZ_STRINGIFY(_X) AZ_STRINGIFY_HELPER(_X)
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
#define AZ_FLOOR AZ_TRUNC
#endif

#if (defined(__cplusplus) && (__cplusplus >= 202002L))                         \
        || (defined(_MSVC_LANG) && (_MSVC_LANG >= 202002L))
#define AZ_LIKELY [[likely]]
#define AZ_UNLIKELY [[unlikely]]
#else
#define AZ_LIKELY
#define AZ_UNLIKELY
#endif

#ifdef _MSC_VER
#define AZ_CDECL __cdecl
#else
#define AZ_CDECL
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define AZ_MSVC_WARNING_SUPPRESS(XXXX) __pragma(warning(suppress : XXXX))
#else
#define AZ_MSVC_WARNING_SUPPRESS(XXXX)
#endif

#ifndef AZ_DEBUG_BREAK
#if defined(_MSC_VER)
#define AZ_DEBUG_BREAK() __debugbreak()
#elif defined(__clang__)
#define AZ_DEBUG_BREAK() __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define AZ_DEBUG_BREAK() __asm__ volatile("int3;nop")
#elif defined(__GNUC__) && defined(__thumb__)
#define AZ_DEBUG_BREAK() __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define AZ_DEBUG_BREAK() __asm__ volatile(".inst 0xe7f001f0")
#else
#define AZ_DEBUG_BREAK() AZ_ASSERT(0)
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define AZ_PRId64 "I64d"
#define AZ_PRIu64 "I64u"
#define AZ_PRIX64 "I64X"
#else
#define AZ_PRId64 "lld"
#define AZ_PRIu64 "llu"
#define AZ_PRIX64 "llX"
#endif

AZ_API AzID AzHashData(const void* data, size_t data_size, AzID seed = 0);
AZ_API AzID AzHashStr(const char* data, size_t data_size = 0, AzID seed = 0);

#ifndef AzQsort
static inline void AzQsort(void* base,
                           size_t count,
                           size_t size_of_element,
                           int(AZ_CDECL* compare_func)(void const*,
                                                       void const*))
{
    if (count > 1)
        qsort(base, count, size_of_element, compare_func);
}
#endif

AZ_API AzU32 AzAlphaBlendColors(AzU32 col_a, AzU32 col_b);

static inline bool AzIsPowerOfTwo(int v)
{
    return v != 0 && (v & (v - 1)) == 0;
}
static inline bool AzIsPowerOfTwo(AzU64 v)
{
    return v != 0 && (v & (v - 1)) == 0;
}
static inline int AzUpperPowerOfTwo(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
static inline unsigned int AzCountSetBits(unsigned int v)
{
    unsigned int count = 0;
    while (v > 0)
    {
        v = v & (v - 1);
        count++;
    }
    return count;
}

#define AzStrlen strlen
#define AzMemchr memchr
AZ_API int AzStricmp(const char* str1, const char* str2);
AZ_API int AzStrnicmp(const char* str1, const char* str2, size_t count);
AZ_API void AzStrncpy(char* dst, const char* src, size_t count);
AZ_API char* AzStrdup(const char* str);
AZ_API void* AzMemdup(const void* src, size_t size);
AZ_API char* AzStrdupcpy(char* dst, size_t* p_dst_size, const char* str);
AZ_API const char* AzStrchrRange(const char* str_begin,
                                 const char* str_end,
                                 char c);
AZ_API const char* AzStreolRange(const char* str, const char* str_end);
AZ_API const char* AzStristr(const char* haystack,
                             const char* haystack_end,
                             const char* needle,
                             const char* needle_end);
AZ_API void AzStrTrimBlanks(char* str);
AZ_API const char* AzStrSkipBlank(const char* str);
AZ_API int AzStrlenW(const AzWchar* str);
AZ_API const char* AzStrbol(const char* buf_mid_line, const char* buf_begin);
AZ_MSVC_RUNTIME_CHECKS_OFF
static inline char AzToUpper(char c)
{
    return (c >= 'a' && c <= 'z') ? c &= ~32 : c;
}
static inline bool AzCharIsBlankA(char c)
{
    return c == ' ' || c == '\t';
}
static inline bool AzCharIsBlankW(unsigned int c)
{
    return c == ' ' || c == '\t' || c == 0x3000;
}
static inline bool AzCharIsXdigitA(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')
           || (c >= 'a' && c <= 'f');
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

AZ_API int AzFormatString(char* buf, size_t buf_size, const char* fmt, ...)
        AZ_FMTARGS(3);
AZ_API int AzFormatStringV(char* buf,
                           size_t buf_size,
                           const char* fmt,
                           va_list args) AZ_FMTLIST(3);
AZ_API void AzFormatStringToTempBuffer(const char** out_buf,
                                       const char** out_buf_end,
                                       const char* fmt,
                                       ...) AZ_FMTARGS(3);
AZ_API void AzFormatStringToTempBufferV(const char** out_buf,
                                        const char** out_buf_end,
                                        const char* fmt,
                                        va_list args) AZ_FMTLIST(3);
AZ_API const char* AzParseFormatFindStart(const char* format);
AZ_API const char* AzParseFormatFindEnd(const char* format);
AZ_API const char* AzParseFormatTrimDecorations(const char* format,
                                                char* buf,
                                                size_t buf_size);
AZ_API void AzParseFormatSanitizeForPrinting(const char* fmt_in,
                                             char* fmt_out,
                                             size_t fmt_out_size);
AZ_API const char* AzParseFormatSanitizeForScanning(const char* fmt_in,
                                                    char* fmt_out,
                                                    size_t fmt_out_size);
AZ_API int AzParseFormatPrecision(const char* format, int default_value);

AZ_API const char* AzTextCharToUtf8(char out_buf[5], unsigned int c);
AZ_API int AzTextStrToUtf8(char* out_buf,
                           int out_buf_size,
                           const AzWchar* in_text,
                           const AzWchar* in_text_end);
AZ_API int AzTextCharFromUtf8(unsigned int* out_char,
                              const char* in_text,
                              const char* in_text_end);
AZ_API int AzTextStrFromUtf8(AzWchar* out_buf,
                             int out_buf_size,
                             const char* in_text,
                             const char* in_text_end,
                             const char** in_remaining = NULL);
AZ_API int AzTextCountCharsFromUtf8(const char* in_text,
                                    const char* in_text_end);
AZ_API int AzTextCountUtf8BytesFromChar(const char* in_text,
                                        const char* in_text_end);
AZ_API int AzTextCountUtf8BytesFromStr(const AzWchar* in_text,
                                       const AzWchar* in_text_end);
AZ_API const char* AzTextFindPreviousUtf8Codepoint(const char* in_text_start,
                                                   const char* in_text_curr);
AZ_API int AzTextCountLines(const char* in_text, const char* in_text_end);

#ifdef AZ_DISABLE_FILE_FUNCTIONS
#define AZ_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* AzFileHandle;
static inline AzFileHandle AzFileOpen(const char*, const char*)
{
    return NULL;
}
static inline bool AzFileClose(AzFileHandle)
{
    return false;
}
static inline AzU64 AzFileGetSize(AzFileHandle)
{
    return (AzU64)-1;
}
static inline AzU64 AzFileRead(void*, AzU64, AzU64, AzFileHandle)
{
    return 0;
}
static inline AzU64 AzFileWrite(const void*, AzU64, AzU64, AzFileHandle)
{
    return 0;
}
#endif
#ifndef AZ_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* AzFileHandle;
AZ_API AzFileHandle AzFileOpen(const char* filename, const char* mode);
AZ_API bool AzFileClose(AzFileHandle file);
AZ_API AzU64 AzFileGetSize(AzFileHandle file);
AZ_API AzU64 AzFileRead(void* data, AzU64 size, AzU64 count, AzFileHandle file);
AZ_API AzU64 AzFileWrite(const void* data,
                         AzU64 size,
                         AzU64 count,
                         AzFileHandle file);
#else
#define AZ_DISABLE_TTY_FUNCTIONS
#endif
AZ_API void* AzFileLoadToMemory(const char* filename,
                                const char* mode,
                                size_t* out_file_size = NULL,
                                int padding_bytes = 0);

AZ_MSVC_RUNTIME_CHECKS_OFF

#ifndef AZ_DISABLE_DEFAULT_MATH_FUNCTIONS
#define AzFabs(X) fabsf(X)
#define AzSqrt(X) sqrtf(X)
#define AzFmod(X, Y) fmodf((X), (Y))
#define AzCos(X) cosf(X)
#define AzSin(X) sinf(X)
#define AzAcos(X) acosf(X)
#define AzAtan2(Y, X) atan2f((Y), (X))
#define AzAtof(STR) atof(STR)
#define AzCeil(X) ceilf(X)
static inline float AzPow(float x, float y)
{
    return powf(x, y);
}
static inline double AzPow(double x, double y)
{
    return pow(x, y);
}
static inline float AzLog(float x)
{
    return logf(x);
}
static inline double AzLog(double x)
{
    return log(x);
}
static inline int AzAbs(int x)
{
    return x < 0 ? -x : x;
}
static inline float AzAbs(float x)
{
    return fabsf(x);
}
static inline double AzAbs(double x)
{
    return fabs(x);
}
static inline float AzSign(float x)
{
    return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f;
}
static inline double AzSign(double x)
{
    return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0;
}
#ifdef AZ_ENABLE_SSE
static inline float AzRsqrt(float x)
{
    return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x)));
}
#else
static inline float AzRsqrt(float x)
{
    return 1.0f / sqrtf(x);
}
#endif
static inline double AzRsqrt(double x)
{
    return 1.0 / sqrt(x);
}
#endif

template <typename T>
static inline T AzMin(T lhs, T rhs)
{
    return lhs < rhs ? lhs : rhs;
}
template <typename T>
static inline T AzMax(T lhs, T rhs)
{
    return lhs >= rhs ? lhs : rhs;
}
template <typename T>
static inline T AzClamp(T v, T mn, T mx)
{
    return (v < mn) ? mn : (v > mx) ? mx : v;
}
template <typename T>
static inline T AzLerp(T a, T b, float t)
{
    return (T)(a + (b - a) * t);
}
template <typename T>
static inline void AzSwap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
template <typename T>
static inline T AzAddClampOverflow(T a, T b, T mn, T mx)
{
    if (b < 0 && (a < mn - b))
        return mn;
    if (b > 0 && (a > mx - b))
        return mx;
    return a + b;
}
template <typename T>
static inline T AzSubClampOverflow(T a, T b, T mn, T mx)
{
    if (b > 0 && (a < mn + b))
        return mn;
    if (b < 0 && (a > mx + b))
        return mx;
    return a - b;
}

static inline AzVec2 AzMin(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y);
}
static inline AzVec2 AzMax(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x >= rhs.x ? lhs.x : rhs.x,
                  lhs.y >= rhs.y ? lhs.y : rhs.y);
}
static inline AzVec2 AzClamp(const AzVec2& v,
                             const AzVec2& mn,
                             const AzVec2& mx)
{
    return AzVec2((v.x < mn.x)   ? mn.x
                  : (v.x > mx.x) ? mx.x
                                 : v.x,
                  (v.y < mn.y)   ? mn.y
                  : (v.y > mx.y) ? mx.y
                                 : v.y);
}
static inline AzVec2 AzLerp(const AzVec2& a, const AzVec2& b, float t)
{
    return AzVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}
static inline AzVec2 AzLerp(const AzVec2& a, const AzVec2& b, const AzVec2& t)
{
    return AzVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y);
}
static inline AzVec4 AzLerp(const AzVec4& a, const AzVec4& b, float t)
{
    return AzVec4(a.x + (b.x - a.x) * t,
                  a.y + (b.y - a.y) * t,
                  a.z + (b.z - a.z) * t,
                  a.w + (b.w - a.w) * t);
}
static inline float AzSaturate(float f)
{
    return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f;
}
static inline float AzLengthSqr(const AzVec2& lhs)
{
    return (lhs.x * lhs.x) + (lhs.y * lhs.y);
}
static inline float AzLengthSqr(const AzVec4& lhs)
{
    return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z)
           + (lhs.w * lhs.w);
}
static inline float AzInvLength(const AzVec2& lhs, float fail_value)
{
    float d = (lhs.x * lhs.x) + (lhs.y * lhs.y);
    if (d > 0.0f)
        return AzRsqrt(d);
    return fail_value;
}
static inline float AzTrunc(float f)
{
    return (float)(int)(f);
}
static inline AzVec2 AzTrunc(const AzVec2& v)
{
    return AzVec2((float)(int)(v.x), (float)(int)(v.y));
}
static inline float AzFloor(float f)
{
    return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1);
}
static inline AzVec2 AzFloor(const AzVec2& v)
{
    return AzVec2(AzFloor(v.x), AzFloor(v.y));
}
static inline float AzTrunc64(float f)
{
    return (float)(AzS64)(f);
}
static inline float AzRound64(float f)
{
    return (float)(AzS64)(f + 0.5f);
}
static inline int AzModPositive(int a, int b)
{
    return (a + b) % b;
}
static inline float AzDot(const AzVec2& a, const AzVec2& b)
{
    return a.x * b.x + a.y * b.y;
}
static inline AzVec2 AzRotate(const AzVec2& v, float cos_a, float sin_a)
{
    return AzVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}
static inline float AzLinearSweep(float current, float target, float speed)
{
    if (current < target)
        return AzMin(current + speed, target);
    if (current > target)
        return AzMax(current - speed, target);
    return current;
}
static inline float AzLinearRemapClamp(float s0,
                                       float s1,
                                       float d0,
                                       float d1,
                                       float x)
{
    return AzSaturate((x - s0) / (s1 - s0)) * (d1 - d0) + d0;
}
static inline AzVec2 AzMul(const AzVec2& lhs, const AzVec2& rhs)
{
    return AzVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}
static inline bool AzIsFloatAboveGuaranteedIntegerPrecision(float f)
{
    return f <= -16777216 || f >= 16777216;
}
static inline float AzExponentialMovingAverage(float avg, float sample, int n)
{
    avg -= avg / n;
    avg += sample / n;
    return avg;
}
AZ_MSVC_RUNTIME_CHECKS_RESTORE

AZ_API AzVec2 AzBezierCubicCalc(const AzVec2& p1,
                                const AzVec2& p2,
                                const AzVec2& p3,
                                const AzVec2& p4,
                                float t);
AZ_API AzVec2 AzBezierCubicClosestPoint(const AzVec2& p1,
                                        const AzVec2& p2,
                                        const AzVec2& p3,
                                        const AzVec2& p4,
                                        const AzVec2& p,
                                        int num_segments);
AZ_API AzVec2 AzBezierCubicClosestPointCasteljau(const AzVec2& p1,
                                                 const AzVec2& p2,
                                                 const AzVec2& p3,
                                                 const AzVec2& p4,
                                                 const AzVec2& p,
                                                 float tess_tol);
AZ_API AzVec2 AzBezierQuadraticCalc(const AzVec2& p1,
                                    const AzVec2& p2,
                                    const AzVec2& p3,
                                    float t);
AZ_API AzVec2 AzLineClosestPoint(const AzVec2& a,
                                 const AzVec2& b,
                                 const AzVec2& p);
AZ_API bool AzTriangleContainsPoint(const AzVec2& a,
                                    const AzVec2& b,
                                    const AzVec2& c,
                                    const AzVec2& p);
AZ_API AzVec2 AzTriangleClosestPoint(const AzVec2& a,
                                     const AzVec2& b,
                                     const AzVec2& c,
                                     const AzVec2& p);
AZ_API void AzTriangleBarycentricCoords(const AzVec2& a,
                                        const AzVec2& b,
                                        const AzVec2& c,
                                        const AzVec2& p,
                                        float& out_u,
                                        float& out_v,
                                        float& out_w);
inline float AzTriangleArea(const AzVec2& a, const AzVec2& b, const AzVec2& c)
{
    return AzFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y))
                  + (c.x * (a.y - b.y)))
           * 0.5f;
}
inline bool AzTriangleIsClockwise(const AzVec2& a,
                                  const AzVec2& b,
                                  const AzVec2& c)
{
    return ((b.x - a.x) * (c.y - b.y)) - ((c.x - b.x) * (b.y - a.y)) > 0.0f;
}

AZ_MSVC_RUNTIME_CHECKS_OFF
struct AzVec1
{
    float x;
    constexpr AzVec1() : x(0.0f)
    {}
    constexpr AzVec1(float _x) : x(_x)
    {}
};

struct AzVec2i
{
    int x, y;
    constexpr AzVec2i() : x(0), y(0)
    {}
    constexpr AzVec2i(int _x, int _y) : x(_x), y(_y)
    {}
};

struct AzVec2ih
{
    short x, y;
    constexpr AzVec2ih() : x(0), y(0)
    {}
    constexpr AzVec2ih(short _x, short _y) : x(_x), y(_y)
    {}
    constexpr explicit AzVec2ih(const AzVec2& rhs)
        : x((short)rhs.x), y((short)rhs.y)
    {}
};

struct AZ_API AzRect
{
    AzVec2 Min;
    AzVec2 Max;

    constexpr AzRect() : Min(0.0f, 0.0f), Max(0.0f, 0.0f)
    {}
    constexpr AzRect(const AzVec2& min, const AzVec2& max) : Min(min), Max(max)
    {}
    constexpr AzRect(const AzVec4& v) : Min(v.x, v.y), Max(v.z, v.w)
    {}
    constexpr AzRect(float x1, float y1, float x2, float y2)
        : Min(x1, y1), Max(x2, y2)
    {}

    AzVec2 GetCenter() const
    {
        return AzVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f);
    }
    AzVec2 GetSize() const
    {
        return AzVec2(Max.x - Min.x, Max.y - Min.y);
    }
    float GetWidth() const
    {
        return Max.x - Min.x;
    }
    float GetHeight() const
    {
        return Max.y - Min.y;
    }
    float GetArea() const
    {
        return (Max.x - Min.x) * (Max.y - Min.y);
    }
    AzVec2 GetTL() const
    {
        return Min;
    }
    AzVec2 GetTR() const
    {
        return AzVec2(Max.x, Min.y);
    }
    AzVec2 GetBL() const
    {
        return AzVec2(Min.x, Max.y);
    }
    AzVec2 GetBR() const
    {
        return Max;
    }
    bool Contains(const AzVec2& p) const
    {
        return p.x >= Min.x && p.y >= Min.y && p.x < Max.x && p.y < Max.y;
    }
    bool Contains(const AzRect& r) const
    {
        return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x
               && r.Max.y <= Max.y;
    }
    bool ContainsWithPad(const AzVec2& p, const AzVec2& pad) const
    {
        return p.x >= Min.x - pad.x && p.y >= Min.y - pad.y
               && p.x < Max.x + pad.x && p.y < Max.y + pad.y;
    }
    bool Overlaps(const AzRect& r) const
    {
        return r.Min.y < Max.y && r.Max.y > Min.y && r.Min.x < Max.x
               && r.Max.x > Min.x;
    }
    void Add(const AzVec2& p)
    {
        if (Min.x > p.x)
            Min.x = p.x;
        if (Min.y > p.y)
            Min.y = p.y;
        if (Max.x < p.x)
            Max.x = p.x;
        if (Max.y < p.y)
            Max.y = p.y;
    }
    void Add(const AzRect& r)
    {
        if (Min.x > r.Min.x)
            Min.x = r.Min.x;
        if (Min.y > r.Min.y)
            Min.y = r.Min.y;
        if (Max.x < r.Max.x)
            Max.x = r.Max.x;
        if (Max.y < r.Max.y)
            Max.y = r.Max.y;
    }
    void Expand(const float amount)
    {
        Min.x -= amount;
        Min.y -= amount;
        Max.x += amount;
        Max.y += amount;
    }
    void Expand(const AzVec2& amount)
    {
        Min.x -= amount.x;
        Min.y -= amount.y;
        Max.x += amount.x;
        Max.y += amount.y;
    }
    void Translate(const AzVec2& d)
    {
        Min.x += d.x;
        Min.y += d.y;
        Max.x += d.x;
        Max.y += d.y;
    }
    void TranslateX(float dx)
    {
        Min.x += dx;
        Max.x += dx;
    }
    void TranslateY(float dy)
    {
        Min.y += dy;
        Max.y += dy;
    }
    void ClipWith(const AzRect& r)
    {
        Min = AzMax(Min, r.Min);
        Max = AzMin(Max, r.Max);
    }
    void ClipWithFull(const AzRect& r)
    {
        Min = AzClamp(Min, r.Min, r.Max);
        Max = AzClamp(Max, r.Min, r.Max);
    }
    void Floor()
    {
        Min.x = AZ_TRUNC(Min.x);
        Min.y = AZ_TRUNC(Min.y);
        Max.x = AZ_TRUNC(Max.x);
        Max.y = AZ_TRUNC(Max.y);
    }
    bool IsInverted() const
    {
        return Min.x > Max.x || Min.y > Max.y;
    }
    AzVec4 ToVec4() const
    {
        return AzVec4(Min.x, Min.y, Max.x, Max.y);
    }
};

#define AZ_BITARRAY_TESTBIT(_ARRAY, _N)                                        \
    ((_ARRAY[(_N) >> 5] & ((AzU32)1 << ((_N) & 31))) != 0)
#define AZ_BITARRAY_CLEARBIT(_ARRAY, _N)                                       \
    ((_ARRAY[(_N) >> 5] &= ~((AzU32)1 << ((_N) & 31))))
inline size_t AzBitArrayGetStorageSizeInBytes(int bitcount)
{
    return (size_t)((bitcount + 31) >> 5) << 2;
}
inline void AzBitArrayClearAllBits(AzU32* arr, int bitcount)
{
    memset(arr, 0, AzBitArrayGetStorageSizeInBytes(bitcount));
}
inline bool AzBitArrayTestBit(const AzU32* arr, int n)
{
    AzU32 mask = (AzU32)1 << (n & 31);
    return (arr[n >> 5] & mask) != 0;
}
inline void AzBitArrayClearBit(AzU32* arr, int n)
{
    AzU32 mask = (AzU32)1 << (n & 31);
    arr[n >> 5] &= ~mask;
}
inline void AzBitArraySetBit(AzU32* arr, int n)
{
    AzU32 mask = (AzU32)1 << (n & 31);
    arr[n >> 5] |= mask;
}
inline void AzBitArraySetBitRange(AzU32* arr, int n, int n2)
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        AzU32 mask = (AzU32)(((AzU64)1 << b_mod) - 1)
                     & ~(AzU32)(((AzU64)1 << a_mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

typedef AzU32* AzBitArrayPtr;

template <int BITCOUNT, int OFFSET = 0>
struct AzBitArray
{
    AzU32 Storage[(BITCOUNT + 31) >> 5];
    AzBitArray()
    {
        ClearAllBits();
    }
    void ClearAllBits()
    {
        memset(Storage, 0, sizeof(Storage));
    }
    void SetAllBits()
    {
        memset(Storage, 255, sizeof(Storage));
    }
    bool TestBit(int n) const
    {
        n += OFFSET;
        AZ_ASSERT(n >= 0 && n < BITCOUNT);
        return AZ_BITARRAY_TESTBIT(Storage, n);
    }
    void SetBit(int n)
    {
        n += OFFSET;
        AZ_ASSERT(n >= 0 && n < BITCOUNT);
        AzBitArraySetBit(Storage, n);
    }
    void ClearBit(int n)
    {
        n += OFFSET;
        AZ_ASSERT(n >= 0 && n < BITCOUNT);
        AzBitArrayClearBit(Storage, n);
    }
    void SetBitRange(int n, int n2)
    {
        n += OFFSET;
        n2 += OFFSET;
        AZ_ASSERT(n >= 0 && n < BITCOUNT && n2 > n && n2 <= BITCOUNT);
        AzBitArraySetBitRange(Storage, n, n2);
    }
    bool operator[](int n) const
    {
        n += OFFSET;
        AZ_ASSERT(n >= 0 && n < BITCOUNT);
        return AZ_BITARRAY_TESTBIT(Storage, n);
    }
};

struct AZ_API AzBitVector
{
    AzVector<AzU32> Storage;
    void Create(int sz)
    {
        Storage.resize((sz + 31) >> 5);
        memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0]));
    }
    void Clear()
    {
        Storage.clear();
    }
    bool TestBit(int n) const
    {
        AZ_ASSERT(n < (Storage.Size << 5));
        return AZ_BITARRAY_TESTBIT(Storage.Data, n);
    }
    void SetBit(int n)
    {
        AZ_ASSERT(n < (Storage.Size << 5));
        AzBitArraySetBit(Storage.Data, n);
    }
    void ClearBit(int n)
    {
        AZ_ASSERT(n < (Storage.Size << 5));
        AzBitArrayClearBit(Storage.Data, n);
    }
};
AZ_MSVC_RUNTIME_CHECKS_RESTORE

template <typename T>
struct AzSpan
{
    T* Data;
    T* DataEnd;

    inline AzSpan()
    {
        Data = DataEnd = NULL;
    }
    inline AzSpan(T* data, int size)
    {
        Data = data;
        DataEnd = data + size;
    }
    inline AzSpan(T* data, T* data_end)
    {
        Data = data;
        DataEnd = data_end;
    }

    inline void set(T* data, int size)
    {
        Data = data;
        DataEnd = data + size;
    }
    inline void set(T* data, T* data_end)
    {
        Data = data;
        DataEnd = data_end;
    }
    inline int size() const
    {
        return (int)(ptrdiff_t)(DataEnd - Data);
    }
    inline int size_in_bytes() const
    {
        return (int)(ptrdiff_t)(DataEnd - Data) * (int)sizeof(T);
    }
    inline T& operator[](int i)
    {
        T* p = Data + i;
        AZ_ASSERT(p >= Data && p < DataEnd);
        return *p;
    }
    inline const T& operator[](int i) const
    {
        const T* p = Data + i;
        AZ_ASSERT(p >= Data && p < DataEnd);
        return *p;
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
        return DataEnd;
    }
    inline const T* end() const
    {
        return DataEnd;
    }

    inline int index_from_ptr(const T* it) const
    {
        AZ_ASSERT(it >= Data && it < DataEnd);
        const ptrdiff_t off = it - Data;
        return (int)off;
    }
};

template <int CHUNKS>
struct AzSpanAllocator
{
    char* BasePtr;
    int CurrOff;
    int CurrIdx;
    int Offsets[CHUNKS];
    int Sizes[CHUNKS];

    AzSpanAllocator()
    {
        memset(this, 0, sizeof(*this));
    }
    inline void Reserve(int n, size_t sz, int a = 4)
    {
        AZ_ASSERT(n == CurrIdx && n < CHUNKS);
        CurrOff = AZ_MEMALIGN(CurrOff, a);
        Offsets[n] = CurrOff;
        Sizes[n] = (int)sz;
        CurrIdx++;
        CurrOff += (int)sz;
    }
    inline int GetArenaSizeInBytes()
    {
        return CurrOff;
    }
    inline void SetArenaBasePtr(void* base_ptr)
    {
        BasePtr = (char*)base_ptr;
    }
    inline void* GetSpanPtrBegin(int n)
    {
        AZ_ASSERT(n >= 0 && n < CHUNKS && CurrIdx == CHUNKS);
        return (void*)(BasePtr + Offsets[n]);
    }
    inline void* GetSpanPtrEnd(int n)
    {
        AZ_ASSERT(n >= 0 && n < CHUNKS && CurrIdx == CHUNKS);
        return (void*)(BasePtr + Offsets[n] + Sizes[n]);
    }
    template <typename T>
    inline void GetSpan(int n, AzSpan<T>* span)
    {
        span->set((T*)GetSpanPtrBegin(n), (T*)GetSpanPtrEnd(n));
    }
};

template <typename T, int BLOCK_SIZE>
struct AzStableVector
{
    int Size = 0;
    int Capacity = 0;
    AzVector<T*> Blocks;

    inline ~AzStableVector()
    {
        for (T* block : Blocks)
            AZ_FREE(block);
    }

    inline void clear()
    {
        Size = Capacity = 0;
        Blocks.clear_delete();
    }
    inline void resize(int new_size)
    {
        if (new_size > Capacity)
            reserve(new_size);
        Size = new_size;
    }
    inline void reserve(int new_cap)
    {
        new_cap = AZ_MEMALIGN(new_cap, BLOCK_SIZE);
        int old_count = Capacity / BLOCK_SIZE;
        int new_count = new_cap / BLOCK_SIZE;
        if (new_count <= old_count)
            return;
        Blocks.resize(new_count);
        for (int n = old_count; n < new_count; n++)
            Blocks[n] = (T*)AZ_ALLOC(sizeof(T) * BLOCK_SIZE);
        Capacity = new_cap;
    }
    inline T& operator[](int i)
    {
        AZ_ASSERT(i >= 0 && i < Size);
        return Blocks[i / BLOCK_SIZE][i % BLOCK_SIZE];
    }
    inline const T& operator[](int i) const
    {
        AZ_ASSERT(i >= 0 && i < Size);
        return Blocks[i / BLOCK_SIZE][i % BLOCK_SIZE];
    }
    inline T* push_back(const T& v)
    {
        int i = Size;
        AZ_ASSERT(i >= 0);
        if (Size == Capacity)
            reserve(Capacity + BLOCK_SIZE);
        void* ptr = &Blocks[i / BLOCK_SIZE][i % BLOCK_SIZE];
        memcpy(ptr, &v, sizeof(v));
        Size++;
        return (T*)ptr;
    }
};

typedef int AzPoolIdx;
template <typename T>
struct AzPool
{
    AzVector<T> Buf;
    AzStorage Map;
    AzPoolIdx FreeIdx;
    AzPoolIdx AliveCount;

    AzPool()
    {
        FreeIdx = AliveCount = 0;
    }
    ~AzPool()
    {
        Clear();
    }
    T* GetByKey(AzID key)
    {
        int idx = Map.GetInt(key, -1);
        return (idx != -1) ? &Buf[idx] : NULL;
    }
    T* GetByIndex(AzPoolIdx n)
    {
        return &Buf[n];
    }
    AzPoolIdx GetIndex(const T* p) const
    {
        AZ_ASSERT(p >= Buf.Data && p < Buf.Data + Buf.Size);
        return (AzPoolIdx)(p - Buf.Data);
    }
    T* GetOrAddByKey(AzID key)
    {
        int* p_idx = Map.GetIntRef(key, -1);
        if (*p_idx != -1)
            return &Buf[*p_idx];
        *p_idx = FreeIdx;
        return Add();
    }
    bool Contains(const T* p) const
    {
        return (p >= Buf.Data && p < Buf.Data + Buf.Size);
    }
    void Clear()
    {
        for (int n = 0; n < Map.Data.Size; n++)
        {
            int idx = Map.Data[n].val_i;
            if (idx != -1)
                Buf[idx].~T();
        }
        Map.Clear();
        Buf.clear();
        FreeIdx = AliveCount = 0;
    }
    T* Add()
    {
        int idx = FreeIdx;
        if (idx == Buf.Size)
        {
            Buf.resize(Buf.Size + 1);
            FreeIdx++;
        } else
        {
            FreeIdx = *(int*)&Buf[idx];
        }
        AZ_PLACEMENT_NEW(&Buf[idx]) T();
        AliveCount++;
        return &Buf[idx];
    }
    void Remove(AzID key, const T* p)
    {
        Remove(key, GetIndex(p));
    }
    void Remove(AzID key, AzPoolIdx idx)
    {
        Buf[idx].~T();
        *(int*)&Buf[idx] = FreeIdx;
        FreeIdx = idx;
        Map.SetInt(key, -1);
        AliveCount--;
    }
    void Reserve(int capacity)
    {
        Buf.reserve(capacity);
        Map.Data.reserve(capacity);
    }

    int GetAliveCount() const
    {
        return AliveCount;
    }
    int GetBufSize() const
    {
        return Buf.Size;
    }
    int GetMapSize() const
    {
        return Map.Data.Size;
    }
    T* TryGetMapData(AzPoolIdx n)
    {
        int idx = Map.Data[n].val_i;
        if (idx == -1)
            return NULL;
        return GetByIndex(idx);
    }
};

template <typename T>
struct AzChunkStream
{
    AzVector<char> Buf;

    void clear()
    {
        Buf.clear();
    }
    bool empty() const
    {
        return Buf.Size == 0;
    }
    int size() const
    {
        return Buf.Size;
    }
    T* alloc_chunk(size_t sz)
    {
        size_t HDR_SZ = 4;
        sz = AZ_MEMALIGN(HDR_SZ + sz, 4u);
        int off = Buf.Size;
        Buf.resize(off + (int)sz);
        ((int*)(void*)(Buf.Data + off))[0] = (int)sz;
        return (T*)(void*)(Buf.Data + off + (int)HDR_SZ);
    }
    T* begin()
    {
        size_t HDR_SZ = 4;
        if (!Buf.Data)
            return NULL;
        return (T*)(void*)(Buf.Data + HDR_SZ);
    }
    T* next_chunk(T* p)
    {
        size_t HDR_SZ = 4;
        AZ_ASSERT(p >= begin() && p < end());
        p = (T*)(void*)((char*)(void*)p + chunk_size(p));
        if (p == (T*)(void*)((char*)end() + HDR_SZ))
            return (T*)0;
        AZ_ASSERT(p < end());
        return p;
    }
    int chunk_size(const T* p)
    {
        return ((const int*)p)[-1];
    }
    T* end()
    {
        return (T*)(void*)(Buf.Data + Buf.Size);
    }
    int offset_from_ptr(const T* p)
    {
        AZ_ASSERT(p >= begin() && p < end());
        const ptrdiff_t off = (const char*)p - Buf.Data;
        return (int)off;
    }
    T* ptr_from_offset(int off)
    {
        AZ_ASSERT(off >= 4 && off < Buf.Size);
        return (T*)(void*)(Buf.Data + off);
    }
    void swap(AzChunkStream<T>& rhs)
    {
        rhs.Buf.swap(Buf);
    }
};

struct AzTextIndex
{
    AzVector<int> LineOffsets;
    int EndOffset = 0;

    void clear()
    {
        LineOffsets.clear();
        EndOffset = 0;
    }
    int size()
    {
        return LineOffsets.Size;
    }
    const char* get_line_begin(const char* base, int n)
    {
        return base + LineOffsets[n];
    }
    const char* get_line_end(const char* base, int n)
    {
        return base
               + (n + 1 < LineOffsets.Size ? (LineOffsets[n + 1] - 1)
                                           : EndOffset);
    }
    void append(const char* base, int old_size, int new_size);
};

AZ_API AzStoragePair* AzLowerBound(AzStoragePair* in_begin,
                                   AzStoragePair* in_end,
                                   AzID key);

#define AZ_ROUNDUP_TO_EVEN(_V) ((((_V) + 1) / 2) * 2)
#define AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN 4
#define AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX 512
#define AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD, _MAXERROR)                  \
    AzClamp(AZ_ROUNDUP_TO_EVEN((int)AzCeil(                                    \
                    AZ_PI / AzAcos(1 - AzMin((_MAXERROR), (_RAD)) / (_RAD)))), \
            AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN,                               \
            AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

#define AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N, _MAXERROR)                  \
    ((_MAXERROR) / (1 - AzCos(AZ_PI / AzMax((float)(_N), AZ_PI))))
#define AZ_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_ERROR(_N, _RAD)                   \
    ((1 - AzCos(AZ_PI / AzMax((float)(_N), AZ_PI))) / (_RAD))

#ifndef AZ_DRAWLIST_ARCFAST_TABLE_SIZE
#define AZ_DRAWLIST_ARCFAST_TABLE_SIZE 48
#endif
#define AZ_DRAWLIST_ARCFAST_SAMPLE_MAX AZ_DRAWLIST_ARCFAST_TABLE_SIZE

struct AZ_API AzDrawListSharedData
{
    AzVec2 TexUvWhitePixel;
    const AzVec4* TexUvLines;
    AzFont* Font;
    float FontSize;
    float FontScale;
    float CurveTessellationTol;
    float CircleSegmentMaxError;
    float InitialFringeScale;
    AzDrawListFlags InitialFlags;
    AzVec4 ClipRectFullscreen;
    AzVector<AzVec2> TempBuffer;
    AzVector<AzDrawList*> DrawLists;
    AzContext* Context;

    AzVec2 ArcFastVtx[AZ_DRAWLIST_ARCFAST_TABLE_SIZE];
    float ArcFastRadiusCutoff;
    AzU8 CircleSegmentCounts[64];

    AzDrawListSharedData();
    ~AzDrawListSharedData();
    void SetCircleTessellationMaxError(float max_error);
};

struct AzDrawDataBuilder
{
    AzVector<AzDrawList*>* Layers[2];
    AzVector<AzDrawList*> LayerData1;

    AzDrawDataBuilder()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzFontStackData
{
    AzFont* Font;
    float FontSizeBeforeScaling;
    float FontSizeAfterScaling;
};

struct AzStyleVarInfo
{
    AzU32 Count : 8;
    AzDataType DataType : 8;
    AzU32 Offset : 16;
    void* GetVarPtr(void* parent) const
    {
        return (void*)((unsigned char*)parent + Offset);
    }
};

struct AzColorMod
{
    AzCol Col;
    AzVec4 BackupValue;
};

struct AzStyleMod
{
    AzStyleVar VarIdx;
    union
    {
        int BackupInt[2];
        float BackupFloat[2];
    };
    AzStyleMod(AzStyleVar idx, int v)
    {
        VarIdx = idx;
        BackupInt[0] = v;
    }
    AzStyleMod(AzStyleVar idx, float v)
    {
        VarIdx = idx;
        BackupFloat[0] = v;
    }
    AzStyleMod(AzStyleVar idx, AzVec2 v)
    {
        VarIdx = idx;
        BackupFloat[0] = v.x;
        BackupFloat[1] = v.y;
    }
};

struct AzDataTypeStorage
{
    AzU8 Data[8];
};

struct AzDataTypeInfo
{
    size_t Size;
    const char* Name;
    const char* PrintFmt;
    const char* ScanFmt;
};

enum AzDataTypePrivate_
{
    AzDataType_Pointer = AzDataType_COUNT,
    AzDataType_ID,
};

enum AzItemFlagsPrivate_
{
    AzItemFlags_Disabled = 1 << 10,
    AzItemFlags_ReadOnly = 1 << 11,
    AzItemFlags_MixedValue = 1 << 12,
    AzItemFlags_NoWindowHoverableCheck = 1 << 13,
    AzItemFlags_AllowOverlap = 1 << 14,
    AzItemFlags_NoNavDisableMouseHover = 1 << 15,
    AzItemFlags_NoMarkEdited = 1 << 16,

    AzItemFlags_Inputable = 1 << 20,
    AzItemFlags_HasSelectionUserData = 1 << 21,
    AzItemFlags_IsMultiSelect = 1 << 22,

    AzItemFlags_Default_ = AzItemFlags_AutoClosePopups,
};

enum AzItemStatusFlags_
{
    AzItemStatusFlags_None = 0,
    AzItemStatusFlags_HoveredRect = 1 << 0,
    AzItemStatusFlags_HasDisplayRect = 1 << 1,
    AzItemStatusFlags_Edited = 1 << 2,
    AzItemStatusFlags_ToggledSelection = 1 << 3,
    AzItemStatusFlags_ToggledOpen = 1 << 4,
    AzItemStatusFlags_HasDeactivated = 1 << 5,
    AzItemStatusFlags_Deactivated = 1 << 6,
    AzItemStatusFlags_HoveredWindow = 1 << 7,
    AzItemStatusFlags_Visible = 1 << 8,
    AzItemStatusFlags_HasClipRect = 1 << 9,
    AzItemStatusFlags_HasShortcut = 1 << 10,

#ifdef AZ_ENABLE_TEST_ENGINE
    AzItemStatusFlags_Openable = 1 << 20,
    AzItemStatusFlags_Opened = 1 << 21,
    AzItemStatusFlags_Checkable = 1 << 22,
    AzItemStatusFlags_Checked = 1 << 23,
    AzItemStatusFlags_Inputable = 1 << 24,
#endif
};

enum AzHoveredFlagsPrivate_
{
    AzHoveredFlags_DelayMask_ = AzHoveredFlags_DelayNone
                                | AzHoveredFlags_DelayShort
                                | AzHoveredFlags_DelayNormal
                                | AzHoveredFlags_NoSharedDelay,
    AzHoveredFlags_AllowedMaskForIsWindowHovered =
            AzHoveredFlags_ChildWindows | AzHoveredFlags_RootWindow
            | AzHoveredFlags_AnyWindow | AzHoveredFlags_NoPopupHierarchy
            | AzHoveredFlags_DockHierarchy
            | AzHoveredFlags_AllowWhenBlockedByPopup
            | AzHoveredFlags_AllowWhenBlockedByActiveItem
            | AzHoveredFlags_ForTooltip | AzHoveredFlags_Stationary,
    AzHoveredFlags_AllowedMaskForIsItemHovered =
            AzHoveredFlags_AllowWhenBlockedByPopup
            | AzHoveredFlags_AllowWhenBlockedByActiveItem
            | AzHoveredFlags_AllowWhenOverlapped
            | AzHoveredFlags_AllowWhenDisabled | AzHoveredFlags_NoNavOverride
            | AzHoveredFlags_ForTooltip | AzHoveredFlags_Stationary
            | AzHoveredFlags_DelayMask_,
};

enum AzInputTextFlagsPrivate_
{
    AzInputTextFlags_Multiline = 1 << 26,
    AzInputTextFlags_MergedItem = 1 << 27,
    AzInputTextFlags_LocalizeDecimalPoint = 1 << 28,
};

enum AzButtonFlagsPrivate_
{
    AzButtonFlags_PressedOnClick = 1 << 4,
    AzButtonFlags_PressedOnClickRelease = 1 << 5,
    AzButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6,
    AzButtonFlags_PressedOnRelease = 1 << 7,
    AzButtonFlags_PressedOnDoubleClick = 1 << 8,
    AzButtonFlags_PressedOnDragDropHold = 1 << 9,

    AzButtonFlags_FlattenChildren = 1 << 11,
    AzButtonFlags_AllowOverlap = 1 << 12,

    AzButtonFlags_AlignTextBaseLine = 1 << 15,
    AzButtonFlags_NoKeyModsAllowed = 1 << 16,
    AzButtonFlags_NoHoldingActiveId = 1 << 17,
    AzButtonFlags_NoNavFocus = 1 << 18,
    AzButtonFlags_NoHoveredOnFocus = 1 << 19,
    AzButtonFlags_NoSetKeyOwner = 1 << 20,
    AzButtonFlags_NoTestKeyOwner = 1 << 21,
    AzButtonFlags_PressedOnMask_ = AzButtonFlags_PressedOnClick
                                   | AzButtonFlags_PressedOnClickRelease
                                   | AzButtonFlags_PressedOnClickReleaseAnywhere
                                   | AzButtonFlags_PressedOnRelease
                                   | AzButtonFlags_PressedOnDoubleClick
                                   | AzButtonFlags_PressedOnDragDropHold,
    AzButtonFlags_PressedOnDefault_ = AzButtonFlags_PressedOnClickRelease,
};

enum AzComboFlagsPrivate_
{
    AzComboFlags_CustomPreview = 1 << 20,
};

enum AzSliderFlagsPrivate_
{
    AzSliderFlags_Vertical = 1 << 20,
    AzSliderFlags_ReadOnly = 1 << 21,
};

enum AzSelectableFlagsPrivate_
{
    AzSelectableFlags_NoHoldingActiveID = 1 << 20,
    AzSelectableFlags_SelectOnNav = 1 << 21,
    AzSelectableFlags_SelectOnClick = 1 << 22,
    AzSelectableFlags_SelectOnRelease = 1 << 23,
    AzSelectableFlags_SpanAvailWidth = 1 << 24,
    AzSelectableFlags_SetNavIdOnHover = 1 << 25,
    AzSelectableFlags_NoPadWithHalfSpacing = 1 << 26,
    AzSelectableFlags_NoSetKeyOwner = 1 << 27,
};

enum AzTreeNodeFlagsPrivate_
{
    AzTreeNodeFlags_NoNavFocus = 1 << 27,
    AzTreeNodeFlags_ClipLabelForTrailingButton = 1 << 28,
    AzTreeNodeFlags_UpsideDownArrow = 1 << 29,
    AzTreeNodeFlags_OpenOnMask_ = AzTreeNodeFlags_OpenOnDoubleClick
                                  | AzTreeNodeFlags_OpenOnArrow,
    AzTreeNodeFlags_DrawLinesMask_ = AzTreeNodeFlags_DrawLinesNone
                                     | AzTreeNodeFlags_DrawLinesFull
                                     | AzTreeNodeFlags_DrawLinesToNodes,
};

enum AzSeparatorFlags_
{
    AzSeparatorFlags_None = 0,
    AzSeparatorFlags_Horizontal = 1 << 0,
    AzSeparatorFlags_Vertical = 1 << 1,
    AzSeparatorFlags_SpanAllColumns = 1 << 2,
};

enum AzFocusRequestFlags_
{
    AzFocusRequestFlags_None = 0,
    AzFocusRequestFlags_RestoreFocusedChild = 1 << 0,
    AzFocusRequestFlags_UnlessBelowModal = 1 << 1,
};

enum AzTextFlags_
{
    AzTextFlags_None = 0,
    AzTextFlags_NoWidthForLargeClippedText = 1 << 0,
};

enum AzTooltipFlags_
{
    AzTooltipFlags_None = 0,
    AzTooltipFlags_OverridePrevious = 1 << 1,
};

enum AzLayoutType_
{
    AzLayoutType_Horizontal = 0,
    AzLayoutType_Vertical = 1
};

enum AzLogFlags_
{
    AzLogFlags_None = 0,

    AzLogFlags_OutputTTY = 1 << 0,
    AzLogFlags_OutputFile = 1 << 1,
    AzLogFlags_OutputBuffer = 1 << 2,
    AzLogFlags_OutputClipboard = 1 << 3,
    AzLogFlags_OutputMask_ = AzLogFlags_OutputTTY | AzLogFlags_OutputFile
                             | AzLogFlags_OutputBuffer
                             | AzLogFlags_OutputClipboard,
};

enum AzAxis
{
    AzAxis_None = -1,
    AzAxis_X = 0,
    AzAxis_Y = 1
};

enum AzPlotType
{
    AzPlotType_Lines,
    AzPlotType_Histogram,
};

struct AZ_API AzComboPreviewData
{
    AzRect PreviewRect;
    AzVec2 BackupCursorPos;
    AzVec2 BackupCursorMaxPos;
    AzVec2 BackupCursorPosPrevLine;
    float BackupPrevLineTextBaseOffset;
    AzLayoutType BackupLayout;

    AzComboPreviewData()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AZ_API AzGroupData
{
    AzID WindowID;
    AzVec2 BackupCursorPos;
    AzVec2 BackupCursorMaxPos;
    AzVec2 BackupCursorPosPrevLine;
    AzVec1 BackupIndent;
    AzVec1 BackupGroupOffset;
    AzVec2 BackupCurrLineSize;
    float BackupCurrLineTextBaseOffset;
    AzID BackupActiveIdIsAlive;
    bool BackupDeactivatedIdIsAlive;
    bool BackupHoveredIdIsAlive;
    bool BackupIsSameLine;
    bool EmitItem;
};

struct AZ_API AzMenuColumns
{
    AzU32 TotalWidth;
    AzU32 NextTotalWidth;
    AzU16 Spacing;
    AzU16 OffsetIcon;
    AzU16 OffsetLabel;
    AzU16 OffsetShortcut;
    AzU16 OffsetMark;
    AzU16 Widths[4];

    AzMenuColumns()
    {
        memset(this, 0, sizeof(*this));
    }
    void Update(float spacing, bool window_reappearing);
    float DeclColumns(float w_icon,
                      float w_label,
                      float w_shortcut,
                      float w_mark);
    void CalcNextTotalWidth(bool update_offsets);
};

struct AZ_API AzInputTextDeactivatedState
{
    AzID ID;
    AzVector<char> TextA;

    AzInputTextDeactivatedState()
    {
        memset(this, 0, sizeof(*this));
    }
    void ClearFreeMemory()
    {
        ID = 0;
        TextA.clear();
    }
};

#undef AZSTB_TEXTEDIT_STRING
#undef AZSTB_TEXTEDIT_CHARTYPE
#define AZSTB_TEXTEDIT_STRING AzInputTextState
#define AZSTB_TEXTEDIT_CHARTYPE char
#define AZSTB_TEXTEDIT_GETWIDTH_NEWLINE (-1.0f)
#define AZSTB_TEXTEDIT_UNDOSTATECOUNT 99
#define AZSTB_TEXTEDIT_UNDOCHARCOUNT 999
namespace AzStb
{
    struct STB_TexteditState;
}
typedef AzStb::STB_TexteditState AzStbTexteditState;

struct AZ_API AzInputTextState
{
    AzContext* Ctx;
    AzStbTexteditState* Stb;
    AzInputTextFlags Flags;
    AzID ID;
    int TextLen;
    const char* TextSrc;
    AzVector<char> TextA;
    AzVector<char> TextToRevertTo;
    AzVector<char> CallbackTextBackup;
    int BufCapacity;
    AzVec2 Scroll;
    float CursorAnim;
    bool CursorFollow;
    bool SelectedAllMouseLock;
    bool Edited;
    bool WantReloadUserBuf;
    int ReloadSelectionStart;
    int ReloadSelectionEnd;

    AzInputTextState();
    ~AzInputTextState();
    void ClearText()
    {
        TextLen = 0;
        TextA[0] = 0;
        CursorClamp();
    }
    void ClearFreeMemory()
    {
        TextA.clear();
        TextToRevertTo.clear();
    }
    void OnKeyPressed(int key);
    void OnCharPressed(unsigned int c);

    void CursorAnimReset();
    void CursorClamp();
    bool HasSelection() const;
    void ClearSelection();
    int GetCursorPos() const;
    int GetSelectionStart() const;
    int GetSelectionEnd() const;
    void SelectAll();

    void ReloadUserBufAndSelectAll();
    void ReloadUserBufAndKeepSelection();
    void ReloadUserBufAndMoveToEnd();
};

enum AzWindowRefreshFlags_
{
    AzWindowRefreshFlags_None = 0,
    AzWindowRefreshFlags_TryToAvoidRefresh = 1 << 0,
    AzWindowRefreshFlags_RefreshOnHover = 1 << 1,
    AzWindowRefreshFlags_RefreshOnFocus = 1 << 2,
};

enum AzNextWindowDataFlags_
{
    AzNextWindowDataFlags_None = 0,
    AzNextWindowDataFlags_HasPos = 1 << 0,
    AzNextWindowDataFlags_HasSize = 1 << 1,
    AzNextWindowDataFlags_HasContentSize = 1 << 2,
    AzNextWindowDataFlags_HasCollapsed = 1 << 3,
    AzNextWindowDataFlags_HasSizeConstraint = 1 << 4,
    AzNextWindowDataFlags_HasFocus = 1 << 5,
    AzNextWindowDataFlags_HasBgAlpha = 1 << 6,
    AzNextWindowDataFlags_HasScroll = 1 << 7,
    AzNextWindowDataFlags_HasWindowFlags = 1 << 8,
    AzNextWindowDataFlags_HasChildFlags = 1 << 9,
    AzNextWindowDataFlags_HasRefreshPolicy = 1 << 10,
    AzNextWindowDataFlags_HasViewport = 1 << 11,
    AzNextWindowDataFlags_HasDock = 1 << 12,
    AzNextWindowDataFlags_HasWindowClass = 1 << 13,
};

struct AzNextWindowData
{
    AzNextWindowDataFlags HasFlags;

    AzCond PosCond;
    AzCond SizeCond;
    AzCond CollapsedCond;
    AzCond DockCond;
    AzVec2 PosVal;
    AzVec2 PosPivotVal;
    AzVec2 SizeVal;
    AzVec2 ContentSizeVal;
    AzVec2 ScrollVal;
    AzWindowFlags WindowFlags;
    AzChildFlags ChildFlags;
    bool PosUndock;
    bool CollapsedVal;
    AzRect SizeConstraintRect;
    AzSizeCallback SizeCallback;
    void* SizeCallbackUserData;
    float BgAlphaVal;
    AzID ViewportId;
    AzID DockId;
    AzWindowClass WindowClass;
    AzVec2 MenuBarOffsetMinVal;
    AzWindowRefreshFlags RefreshFlagsVal;

    AzNextWindowData()
    {
        memset(this, 0, sizeof(*this));
    }
    inline void ClearFlags()
    {
        HasFlags = AzNextWindowDataFlags_None;
    }
};

enum AzNextItemDataFlags_
{
    AzNextItemDataFlags_None = 0,
    AzNextItemDataFlags_HasWidth = 1 << 0,
    AzNextItemDataFlags_HasOpen = 1 << 1,
    AzNextItemDataFlags_HasShortcut = 1 << 2,
    AzNextItemDataFlags_HasRefVal = 1 << 3,
    AzNextItemDataFlags_HasStorageID = 1 << 4,
};

struct AzNextItemData
{
    AzNextItemDataFlags HasFlags;
    AzItemFlags ItemFlags;

    AzID FocusScopeId;
    AzSelectionUserData SelectionUserData;
    float Width;
    AzKeyChord Shortcut;
    AzInputFlags ShortcutFlags;
    bool OpenVal;
    AzU8 OpenCond;
    AzDataTypeStorage RefVal;
    AzID StorageId;

    AzNextItemData()
    {
        memset(this, 0, sizeof(*this));
        SelectionUserData = -1;
    }
    inline void ClearFlags()
    {
        HasFlags = AzNextItemDataFlags_None;
        ItemFlags = AzItemFlags_None;
    }
};

struct AzLastItemData
{
    AzID ID;
    AzItemFlags ItemFlags;
    AzItemStatusFlags StatusFlags;
    AzRect Rect;
    AzRect NavRect;

    AzRect DisplayRect;
    AzRect ClipRect;
    AzKeyChord Shortcut;

    AzLastItemData()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzTreeNodeStackData
{
    AzID ID;
    AzTreeNodeFlags TreeFlags;
    AzItemFlags ItemFlags;
    AzRect NavRect;
    float DrawLinesX1;
    float DrawLinesToNodesY2;
    AzTableColumnIdx DrawLinesTableColumn;
};

struct AZ_API AzErrorRecoveryState
{
    short SizeOfWindowStack;
    short SizeOfIDStack;
    short SizeOfTreeStack;
    short SizeOfColorStack;
    short SizeOfStyleVarStack;
    short SizeOfFontStack;
    short SizeOfFocusScopeStack;
    short SizeOfGroupStack;
    short SizeOfItemFlagsStack;
    short SizeOfBeginPopupStack;
    short SizeOfDisabledStack;

    AzErrorRecoveryState()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzWindowStackData
{
    AzWindow* Window;
    AzLastItemData ParentLastItemDataBackup;
    AzErrorRecoveryState StackSizesInBegin;
    bool DisabledOverrideReenable;
    float DisabledOverrideReenableAlphaBackup;
};

struct AzShrinkWidthItem
{
    int Index;
    float Width;
    float InitialWidth;
};

struct AzPtrOrIndex
{
    void* Ptr;
    int Index;

    AzPtrOrIndex(void* ptr)
    {
        Ptr = ptr;
        Index = -1;
    }
    AzPtrOrIndex(int index)
    {
        Ptr = NULL;
        Index = index;
    }
};

struct AzDeactivatedItemData
{
    AzID ID;
    int ElapseFrame;
    bool HasBeenEditedBefore;
    bool IsAlive;
};

enum AzPopupPositionPolicy
{
    AzPopupPositionPolicy_Default,
    AzPopupPositionPolicy_ComboBox,
    AzPopupPositionPolicy_Tooltip,
};

struct AzPopupData
{
    AzID PopupId;
    AzWindow* Window;
    AzWindow* RestoreNavWindow;
    int ParentNavLayer;
    int OpenFrameCount;
    AzID OpenParentId;
    AzVec2 OpenPopupPos;
    AzVec2 OpenMousePos;

    AzPopupData()
    {
        memset(this, 0, sizeof(*this));
        ParentNavLayer = OpenFrameCount = -1;
    }
};

typedef AzBitArray<AzKey_NamedKey_COUNT, -AzKey_NamedKey_BEGIN>
        AzBitArrayForNamedKeys;

#define AzKey_LegacyNativeKey_BEGIN 0
#define AzKey_LegacyNativeKey_END 512
#define AzKey_Keyboard_BEGIN (AzKey_NamedKey_BEGIN)
#define AzKey_Keyboard_END (AzKey_GamepadStart)
#define AzKey_Gamepad_BEGIN (AzKey_GamepadStart)
#define AzKey_Gamepad_END (AzKey_GamepadRStickDown + 1)
#define AzKey_Mouse_BEGIN (AzKey_MouseLeft)
#define AzKey_Mouse_END (AzKey_MouseWheelY + 1)
#define AzKey_Aliases_BEGIN (AzKey_Mouse_BEGIN)
#define AzKey_Aliases_END (AzKey_Mouse_END)

#define AzKey_NavKeyboardTweakSlow AzMod_Ctrl
#define AzKey_NavKeyboardTweakFast AzMod_Shift
#define AzKey_NavGamepadTweakSlow AzKey_GamepadL1
#define AzKey_NavGamepadTweakFast AzKey_GamepadR1
#define AzKey_NavGamepadActivate                                               \
    (g.IO.ConfigNavSwapGamepadButtons ? AzKey_GamepadFaceRight                 \
                                      : AzKey_GamepadFaceDown)
#define AzKey_NavGamepadCancel                                                 \
    (g.IO.ConfigNavSwapGamepadButtons ? AzKey_GamepadFaceDown                  \
                                      : AzKey_GamepadFaceRight)
#define AzKey_NavGamepadMenu AzKey_GamepadFaceLeft
#define AzKey_NavGamepadInput AzKey_GamepadFaceUp

enum AzInputEventType
{
    AzInputEventType_None = 0,
    AzInputEventType_MousePos,
    AzInputEventType_MouseWheel,
    AzInputEventType_MouseButton,
    AzInputEventType_MouseViewport,
    AzInputEventType_Key,
    AzInputEventType_Text,
    AzInputEventType_Focus,
    AzInputEventType_COUNT
};

enum AzInputSource
{
    AzInputSource_None = 0,
    AzInputSource_Mouse,
    AzInputSource_Keyboard,
    AzInputSource_Gamepad,
    AzInputSource_COUNT
};

struct AzInputEventMousePos
{
    float PosX, PosY;
    AzMouseSource MouseSource;
};
struct AzInputEventMouseWheel
{
    float WheelX, WheelY;
    AzMouseSource MouseSource;
};
struct AzInputEventMouseButton
{
    int Button;
    bool Down;
    AzMouseSource MouseSource;
};
struct AzInputEventMouseViewport
{
    AzID HoveredViewportID;
};
struct AzInputEventKey
{
    AzKey Key;
    bool Down;
    float AnalogValue;
};
struct AzInputEventText
{
    unsigned int Char;
};
struct AzInputEventAppFocused
{
    bool Focused;
};

struct AzInputEvent
{
    AzInputEventType Type;
    AzInputSource Source;
    AzU32 EventId;
    union
    {
        AzInputEventMousePos MousePos;
        AzInputEventMouseWheel MouseWheel;
        AzInputEventMouseButton MouseButton;
        AzInputEventMouseViewport MouseViewport;
        AzInputEventKey Key;
        AzInputEventText Text;
        AzInputEventAppFocused AppFocused;
    };
    bool AddedByTestEngine;

    AzInputEvent()
    {
        memset(this, 0, sizeof(*this));
    }
};

#define AzKeyOwner_Any ((AzID)0)
#define AzKeyOwner_NoOwner ((AzID) - 1)

typedef AzS16 AzKeyRoutingIndex;

struct AzKeyRoutingData
{
    AzKeyRoutingIndex NextEntryIndex;
    AzU16 Mods;
    AzU8 RoutingCurrScore;
    AzU8 RoutingNextScore;
    AzID RoutingCurr;
    AzID RoutingNext;

    AzKeyRoutingData()
    {
        NextEntryIndex = -1;
        Mods = 0;
        RoutingCurrScore = RoutingNextScore = 255;
        RoutingCurr = RoutingNext = AzKeyOwner_NoOwner;
    }
};

struct AzKeyRoutingTable
{
    AzKeyRoutingIndex Index[AzKey_NamedKey_COUNT];
    AzVector<AzKeyRoutingData> Entries;
    AzVector<AzKeyRoutingData> EntriesNext;

    AzKeyRoutingTable()
    {
        Clear();
    }
    void Clear()
    {
        for (int n = 0; n < AZ_ARRAYSIZE(Index); n++)
            Index[n] = -1;
        Entries.clear();
        EntriesNext.clear();
    }
};

struct AzKeyOwnerData
{
    AzID OwnerCurr;
    AzID OwnerNext;
    bool LockThisFrame;
    bool LockUntilRelease;

    AzKeyOwnerData()
    {
        OwnerCurr = OwnerNext = AzKeyOwner_NoOwner;
        LockThisFrame = LockUntilRelease = false;
    }
};

enum AzInputFlagsPrivate_
{
    AzInputFlags_RepeatRateDefault = 1 << 1,
    AzInputFlags_RepeatRateNavMove = 1 << 2,
    AzInputFlags_RepeatRateNavTweak = 1 << 3,

    AzInputFlags_RepeatUntilRelease = 1 << 4,
    AzInputFlags_RepeatUntilKeyModsChange = 1 << 5,
    AzInputFlags_RepeatUntilKeyModsChangeFromNone = 1 << 6,
    AzInputFlags_RepeatUntilOtherKeyPress = 1 << 7,

    AzInputFlags_LockThisFrame = 1 << 20,
    AzInputFlags_LockUntilRelease = 1 << 21,

    AzInputFlags_CondHovered = 1 << 22,
    AzInputFlags_CondActive = 1 << 23,
    AzInputFlags_CondDefault_ = AzInputFlags_CondHovered
                                | AzInputFlags_CondActive,

    AzInputFlags_RepeatRateMask_ = AzInputFlags_RepeatRateDefault
                                   | AzInputFlags_RepeatRateNavMove
                                   | AzInputFlags_RepeatRateNavTweak,
    AzInputFlags_RepeatUntilMask_ =
            AzInputFlags_RepeatUntilRelease
            | AzInputFlags_RepeatUntilKeyModsChange
            | AzInputFlags_RepeatUntilKeyModsChangeFromNone
            | AzInputFlags_RepeatUntilOtherKeyPress,
    AzInputFlags_RepeatMask_ = AzInputFlags_Repeat
                               | AzInputFlags_RepeatRateMask_
                               | AzInputFlags_RepeatUntilMask_,
    AzInputFlags_CondMask_ = AzInputFlags_CondHovered | AzInputFlags_CondActive,
    AzInputFlags_RouteTypeMask_ = AzInputFlags_RouteActive
                                  | AzInputFlags_RouteFocused
                                  | AzInputFlags_RouteGlobal
                                  | AzInputFlags_RouteAlways,
    AzInputFlags_RouteOptionsMask_ = AzInputFlags_RouteOverFocused
                                     | AzInputFlags_RouteOverActive
                                     | AzInputFlags_RouteUnlessBgFocused
                                     | AzInputFlags_RouteFromRootWindow,
    AzInputFlags_SupportedByIsKeyPressed = AzInputFlags_RepeatMask_,
    AzInputFlags_SupportedByIsMouseClicked = AzInputFlags_Repeat,
    AzInputFlags_SupportedByShortcut = AzInputFlags_RepeatMask_
                                       | AzInputFlags_RouteTypeMask_
                                       | AzInputFlags_RouteOptionsMask_,
    AzInputFlags_SupportedBySetNextItemShortcut =
            AzInputFlags_RepeatMask_ | AzInputFlags_RouteTypeMask_
            | AzInputFlags_RouteOptionsMask_ | AzInputFlags_Tooltip,
    AzInputFlags_SupportedBySetKeyOwner = AzInputFlags_LockThisFrame
                                          | AzInputFlags_LockUntilRelease,
    AzInputFlags_SupportedBySetItemKeyOwner =
            AzInputFlags_SupportedBySetKeyOwner | AzInputFlags_CondMask_,
};

struct AzListClipperRange
{
    int Min;
    int Max;
    bool PosToIndexConvert;
    AzS8 PosToIndexOffsetMin;
    AzS8 PosToIndexOffsetMax;

    static AzListClipperRange FromIndices(int min, int max)
    {
        AzListClipperRange r = {min, max, false, 0, 0};
        return r;
    }
    static AzListClipperRange FromPositions(float y1,
                                            float y2,
                                            int off_min,
                                            int off_max)
    {
        AzListClipperRange r = {(int)y1,
                                (int)y2,
                                true,
                                (AzS8)off_min,
                                (AzS8)off_max};
        return r;
    }
};

struct AzListClipperData
{
    AzListClipper* ListClipper;
    float LossynessOffset;
    int StepNo;
    int ItemsFrozen;
    AzVector<AzListClipperRange> Ranges;

    AzListClipperData()
    {
        memset(this, 0, sizeof(*this));
    }
    void Reset(AzListClipper* clipper)
    {
        ListClipper = clipper;
        StepNo = ItemsFrozen = 0;
        Ranges.resize(0);
    }
};

enum AzActivateFlags_
{
    AzActivateFlags_None = 0,
    AzActivateFlags_PreferInput = 1 << 0,
    AzActivateFlags_PreferTweak = 1 << 1,
    AzActivateFlags_TryToPreserveState = 1 << 2,
    AzActivateFlags_FromTabbing = 1 << 3,
    AzActivateFlags_FromShortcut = 1 << 4,
};

enum AzScrollFlags_
{
    AzScrollFlags_None = 0,
    AzScrollFlags_KeepVisibleEdgeX = 1 << 0,
    AzScrollFlags_KeepVisibleEdgeY = 1 << 1,
    AzScrollFlags_KeepVisibleCenterX = 1 << 2,
    AzScrollFlags_KeepVisibleCenterY = 1 << 3,
    AzScrollFlags_AlwaysCenterX = 1 << 4,
    AzScrollFlags_AlwaysCenterY = 1 << 5,
    AzScrollFlags_NoScrollParent = 1 << 6,
    AzScrollFlags_MaskX_ = AzScrollFlags_KeepVisibleEdgeX
                           | AzScrollFlags_KeepVisibleCenterX
                           | AzScrollFlags_AlwaysCenterX,
    AzScrollFlags_MaskY_ = AzScrollFlags_KeepVisibleEdgeY
                           | AzScrollFlags_KeepVisibleCenterY
                           | AzScrollFlags_AlwaysCenterY,
};

enum AzNavRenderCursorFlags_
{
    AzNavRenderCursorFlags_None = 0,
    AzNavRenderCursorFlags_Compact = 1 << 1,
    AzNavRenderCursorFlags_AlwaysDraw = 1 << 2,
    AzNavRenderCursorFlags_NoRounding = 1 << 3,
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    AzNavHighlightFlags_None = AzNavRenderCursorFlags_None,
    AzNavHighlightFlags_Compact = AzNavRenderCursorFlags_Compact,
    AzNavHighlightFlags_AlwaysDraw = AzNavRenderCursorFlags_AlwaysDraw,
    AzNavHighlightFlags_NoRounding = AzNavRenderCursorFlags_NoRounding,
#endif
};

enum AzNavMoveFlags_
{
    AzNavMoveFlags_None = 0,
    AzNavMoveFlags_LoopX = 1 << 0,
    AzNavMoveFlags_LoopY = 1 << 1,
    AzNavMoveFlags_WrapX = 1 << 2,
    AzNavMoveFlags_WrapY = 1 << 3,
    AzNavMoveFlags_WrapMask_ = AzNavMoveFlags_LoopX | AzNavMoveFlags_LoopY
                               | AzNavMoveFlags_WrapX | AzNavMoveFlags_WrapY,
    AzNavMoveFlags_AllowCurrentNavId = 1 << 4,
    AzNavMoveFlags_AlsoScoreVisibleSet = 1 << 5,
    AzNavMoveFlags_ScrollToEdgeY = 1 << 6,
    AzNavMoveFlags_Forwarded = 1 << 7,
    AzNavMoveFlags_DebugNoResult = 1 << 8,
    AzNavMoveFlags_FocusApi = 1 << 9,
    AzNavMoveFlags_IsTabbing = 1 << 10,
    AzNavMoveFlags_IsPageMove = 1 << 11,
    AzNavMoveFlags_Activate = 1 << 12,
    AzNavMoveFlags_NoSelect = 1 << 13,
    AzNavMoveFlags_NoSetNavCursorVisible = 1 << 14,
    AzNavMoveFlags_NoClearActiveId = 1 << 15,
};

enum AzNavLayer
{
    AzNavLayer_Main = 0,
    AzNavLayer_Menu = 1,
    AzNavLayer_COUNT
};

struct AzNavItemData
{
    AzWindow* Window;
    AzID ID;
    AzID FocusScopeId;
    AzRect RectRel;
    AzItemFlags ItemFlags;
    float DistBox;
    float DistCenter;
    float DistAxial;
    AzSelectionUserData SelectionUserData;

    AzNavItemData()
    {
        Clear();
    }
    void Clear()
    {
        Window = NULL;
        ID = FocusScopeId = 0;
        ItemFlags = 0;
        SelectionUserData = -1;
        DistBox = DistCenter = DistAxial = FLT_MAX;
    }
};

struct AzFocusScopeData
{
    AzID ID;
    AzID WindowID;
};

enum AzTypingSelectFlags_
{
    AzTypingSelectFlags_None = 0,
    AzTypingSelectFlags_AllowBackspace = 1 << 0,
    AzTypingSelectFlags_AllowSingleCharMode = 1 << 1,
};

struct AZ_API AzTypingSelectRequest
{
    AzTypingSelectFlags Flags;
    int SearchBufferLen;
    const char* SearchBuffer;
    bool SelectRequest;
    bool SingleCharMode;
    AzS8 SingleCharSize;
};

struct AZ_API AzTypingSelectState
{
    AzTypingSelectRequest Request;
    char SearchBuffer[64];
    AzID FocusScope;
    int LastRequestFrame = 0;
    float LastRequestTime = 0.0f;
    bool SingleCharModeLock = false;

    AzTypingSelectState()
    {
        memset(this, 0, sizeof(*this));
    }
    void Clear()
    {
        SearchBuffer[0] = 0;
        SingleCharModeLock = false;
    }
};

enum AzOldColumnFlags_
{
    AzOldColumnFlags_None = 0,
    AzOldColumnFlags_NoBorder = 1 << 0,
    AzOldColumnFlags_NoResize = 1 << 1,
    AzOldColumnFlags_NoPreserveWidths = 1 << 2,
    AzOldColumnFlags_NoForceWithinWindow = 1 << 3,
    AzOldColumnFlags_GrowParentContentsSize = 1 << 4,

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

#endif
};

struct AzOldColumnData
{
    float OffsetNorm;
    float OffsetNormBeforeResize;
    AzOldColumnFlags Flags;
    AzRect ClipRect;

    AzOldColumnData()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzOldColumns
{
    AzID ID;
    AzOldColumnFlags Flags;
    bool IsFirstFrame;
    bool IsBeingResized;
    int Current;
    int Count;
    float OffMinX, OffMaxX;
    float LineMinY, LineMaxY;
    float HostCursorPosY;
    float HostCursorMaxPosX;
    AzRect HostInitialClipRect;
    AzRect HostBackupClipRect;
    AzRect HostBackupParentWorkRect;
    AzVector<AzOldColumnData> Columns;
    AzDrawListSplitter Splitter;

    AzOldColumns()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzBoxSelectState
{
    AzID ID;
    bool IsActive;
    bool IsStarting;
    bool IsStartedFromVoid;
    bool IsStartedSetNavIdOnce;
    bool RequestClear;
    AzKeyChord KeyMods : 16;
    AzVec2 StartPosRel;
    AzVec2 EndPosRel;
    AzVec2 ScrollAccum;
    AzWindow* Window;

    bool UnclipMode;
    AzRect UnclipRect;
    AzRect BoxSelectRectPrev;
    AzRect BoxSelectRectCurr;

    AzBoxSelectState()
    {
        memset(this, 0, sizeof(*this));
    }
};

#define AzSelectionUserData_Invalid ((AzSelectionUserData) - 1)

struct AZ_API AzMultiSelectTempData
{
    AzMultiSelectIO IO;
    AzMultiSelectState* Storage;
    AzID FocusScopeId;
    AzMultiSelectFlags Flags;
    AzVec2 ScopeRectMin;
    AzVec2 BackupCursorMaxPos;
    AzSelectionUserData LastSubmittedItem;
    AzID BoxSelectId;
    AzKeyChord KeyMods;
    AzS8 LoopRequestSetAll;
    bool IsEndIO;
    bool IsFocused;
    bool IsKeyboardSetRange;
    bool NavIdPassedBy;
    bool RangeSrcPassedBy;
    bool RangeDstPassedBy;

    AzMultiSelectTempData()
    {
        Clear();
    }
    void Clear()
    {
        size_t io_sz = sizeof(IO);
        ClearIO();
        memset((void*)(&IO + 1), 0, sizeof(*this) - io_sz);
    }
    void ClearIO()
    {
        IO.Requests.resize(0);
        IO.RangeSrcItem = IO.NavIdItem = AzSelectionUserData_Invalid;
        IO.NavIdSelected = IO.RangeSrcReset = false;
    }
};

struct AZ_API AzMultiSelectState
{
    AzWindow* Window;
    AzID ID;
    int LastFrameActive;
    int LastSelectionSize;
    AzS8 RangeSelected;
    AzS8 NavIdSelected;
    AzSelectionUserData RangeSrcItem;
    AzSelectionUserData NavIdItem;

    AzMultiSelectState()
    {
        Window = NULL;
        ID = 0;
        LastFrameActive = LastSelectionSize = 0;
        RangeSelected = NavIdSelected = -1;
        RangeSrcItem = NavIdItem = AzSelectionUserData_Invalid;
    }
};

#define DOCKING_HOST_DRAW_CHANNEL_BG 0
#define DOCKING_HOST_DRAW_CHANNEL_FG 1

#ifdef AZ_HAS_DOCK

enum AzDockNodeFlagsPrivate_
{
    AzDockNodeFlags_DockSpace = 1 << 10,
    AzDockNodeFlags_CentralNode = 1 << 11,
    AzDockNodeFlags_NoTabBar = 1 << 12,
    AzDockNodeFlags_HiddenTabBar = 1 << 13,
    AzDockNodeFlags_NoWindowMenuButton = 1 << 14,
    AzDockNodeFlags_NoCloseButton = 1 << 15,
    AzDockNodeFlags_NoResizeX = 1 << 16,
    AzDockNodeFlags_NoResizeY = 1 << 17,
    AzDockNodeFlags_DockedWindowsInFocusRoute = 1 << 18,

    AzDockNodeFlags_NoDockingSplitOther = 1 << 19,
    AzDockNodeFlags_NoDockingOverMe = 1 << 20,
    AzDockNodeFlags_NoDockingOverOther = 1 << 21,
    AzDockNodeFlags_NoDockingOverEmpty = 1 << 22,
    AzDockNodeFlags_NoDocking = AzDockNodeFlags_NoDockingOverMe
                                | AzDockNodeFlags_NoDockingOverOther
                                | AzDockNodeFlags_NoDockingOverEmpty
                                | AzDockNodeFlags_NoDockingSplit
                                | AzDockNodeFlags_NoDockingSplitOther,

    AzDockNodeFlags_SharedFlagsInheritMask_ = ~0,
    AzDockNodeFlags_NoResizeFlagsMask_ = (int)AzDockNodeFlags_NoResize
                                         | AzDockNodeFlags_NoResizeX
                                         | AzDockNodeFlags_NoResizeY,

    AzDockNodeFlags_LocalFlagsTransferMask_ =
            (int)AzDockNodeFlags_NoDockingSplit
            | AzDockNodeFlags_NoResizeFlagsMask_
            | (int)AzDockNodeFlags_AutoHideTabBar | AzDockNodeFlags_CentralNode
            | AzDockNodeFlags_NoTabBar | AzDockNodeFlags_HiddenTabBar
            | AzDockNodeFlags_NoWindowMenuButton
            | AzDockNodeFlags_NoCloseButton,
    AzDockNodeFlags_SavedFlagsMask_ = AzDockNodeFlags_NoResizeFlagsMask_
                                      | AzDockNodeFlags_DockSpace
                                      | AzDockNodeFlags_CentralNode
                                      | AzDockNodeFlags_NoTabBar
                                      | AzDockNodeFlags_HiddenTabBar
                                      | AzDockNodeFlags_NoWindowMenuButton
                                      | AzDockNodeFlags_NoCloseButton,
};

enum AzDataAuthority_
{
    AzDataAuthority_Auto,
    AzDataAuthority_DockNode,
    AzDataAuthority_Window,
};

enum AzDockNodeState
{
    AzDockNodeState_Unknown,
    AzDockNodeState_HostWindowHiddenBecauseSingleWindow,
    AzDockNodeState_HostWindowHiddenBecauseWindowsAreResizing,
    AzDockNodeState_HostWindowVisible,
};

struct AZ_API AzDockNode
{
    AzID ID;
    AzDockNodeFlags SharedFlags;
    AzDockNodeFlags LocalFlags;
    AzDockNodeFlags LocalFlagsInWindows;
    AzDockNodeFlags MergedFlags;
    AzDockNodeState State;
    AzDockNode* ParentNode;
    AzDockNode* ChildNodes[2];
    AzVector<AzWindow*> Windows;
    AzTabBar* TabBar;
    AzVec2 Pos;
    AzVec2 Size;
    AzVec2 SizeRef;
    AzAxis SplitAxis;
    AzWindowClass WindowClass;
    AzU32 LastBgColor;

    AzWindow* HostWindow;
    AzWindow* VisibleWindow;
    AzDockNode* CentralNode;
    AzDockNode* OnlyNodeWithWindows;
    int CountNodeWithWindows;
    int LastFrameAlive;
    int LastFrameActive;
    int LastFrameFocused;
    AzID LastFocusedNodeId;
    AzID SelectedTabId;
    AzID WantCloseTabId;
    AzID RefViewportId;
    AzDataAuthority AuthorityForPos : 3;
    AzDataAuthority AuthorityForSize : 3;
    AzDataAuthority AuthorityForViewport : 3;
    bool IsVisible : 1;
    bool IsFocused : 1;
    bool IsBgDrawnThisFrame : 1;
    bool HasCloseButton : 1;
    bool HasWindowMenuButton : 1;
    bool HasCentralNodeChild : 1;
    bool WantCloseAll : 1;
    bool WantLockSizeOnce : 1;
    bool WantMouseMove : 1;
    bool WantHiddenTabBarUpdate : 1;
    bool WantHiddenTabBarToggle : 1;

    AzDockNode(AzID id);
    ~AzDockNode();
    bool IsRootNode() const
    {
        return ParentNode == NULL;
    }
    bool IsDockSpace() const
    {
        return (MergedFlags & AzDockNodeFlags_DockSpace) != 0;
    }
    bool IsFloatingNode() const
    {
        return ParentNode == NULL
               && (MergedFlags & AzDockNodeFlags_DockSpace) == 0;
    }
    bool IsCentralNode() const
    {
        return (MergedFlags & AzDockNodeFlags_CentralNode) != 0;
    }
    bool IsHiddenTabBar() const
    {
        return (MergedFlags & AzDockNodeFlags_HiddenTabBar) != 0;
    }
    bool IsNoTabBar() const
    {
        return (MergedFlags & AzDockNodeFlags_NoTabBar) != 0;
    }
    bool IsSplitNode() const
    {
        return ChildNodes[0] != NULL;
    }
    bool IsLeafNode() const
    {
        return ChildNodes[0] == NULL;
    }
    bool IsEmpty() const
    {
        return ChildNodes[0] == NULL && Windows.Size == 0;
    }
    AzRect Rect() const
    {
        return AzRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }

    void SetLocalFlags(AzDockNodeFlags flags)
    {
        LocalFlags = flags;
        UpdateMergedFlags();
    }
    void UpdateMergedFlags()
    {
        MergedFlags = SharedFlags | LocalFlags | LocalFlagsInWindows;
    }
};

enum AzWindowDockStyleCol
{
    AzWindowDockStyleCol_Text,
    AzWindowDockStyleCol_TabHovered,
    AzWindowDockStyleCol_TabFocused,
    AzWindowDockStyleCol_TabSelected,
    AzWindowDockStyleCol_TabSelectedOverline,
    AzWindowDockStyleCol_TabDimmed,
    AzWindowDockStyleCol_TabDimmedSelected,
    AzWindowDockStyleCol_TabDimmedSelectedOverline,
    AzWindowDockStyleCol_COUNT
};

struct AzWindowDockStyle
{
    AzU32 Colors[AzWindowDockStyleCol_COUNT];
};

struct AzDockContext
{
    AzStorage Nodes;
    AzVector<AzDockRequest> Requests;
    AzVector<AzDockNodeSettings> NodesSettings;
    bool WantFullRebuild;
    AzDockContext()
    {
        memset(this, 0, sizeof(*this));
    }
};

#endif

struct AzViewportP : public AzViewport
{
    AzWindow* Window;
    int Idx;
    int LastFrameActive;
    int LastFocusedStampCount;
    AzID LastNameHash;
    AzVec2 LastPos;
    AzVec2 LastSize;
    float Alpha;
    float LastAlpha;
    bool LastFocusedHadNavWindow;
    short PlatformMonitor;
    int BgFgDrawListsLastFrame[2];
    AzDrawList* BgFgDrawLists[2];
    AzDrawData DrawDataP;
    AzDrawDataBuilder DrawDataBuilder;
    AzVec2 LastPlatformPos;
    AzVec2 LastPlatformSize;
    AzVec2 LastRendererSize;

    AzVec2 WorkInsetMin;
    AzVec2 WorkInsetMax;
    AzVec2 BuildWorkInsetMin;
    AzVec2 BuildWorkInsetMax;

    AzViewportP()
    {
        Window = NULL;
        Idx = -1;
        LastFrameActive = BgFgDrawListsLastFrame[0] =
                BgFgDrawListsLastFrame[1] = LastFocusedStampCount = -1;
        LastNameHash = 0;
        Alpha = LastAlpha = 1.0f;
        LastFocusedHadNavWindow = false;
        PlatformMonitor = -1;
        BgFgDrawLists[0] = BgFgDrawLists[1] = NULL;
        LastPlatformPos = LastPlatformSize = LastRendererSize = AzVec2(FLT_MAX,
                                                                       FLT_MAX);
    }
    ~AzViewportP()
    {
        if (BgFgDrawLists[0])
            AZ_DELETE(BgFgDrawLists[0]);
        if (BgFgDrawLists[1])
            AZ_DELETE(BgFgDrawLists[1]);
    }
    void ClearRequestFlags()
    {
        PlatformRequestClose = PlatformRequestMove = PlatformRequestResize =
                false;
    }

    AzVec2 CalcWorkRectPos(const AzVec2& inset_min) const
    {
        return AzVec2(Pos.x + inset_min.x, Pos.y + inset_min.y);
    }
    AzVec2 CalcWorkRectSize(const AzVec2& inset_min,
                            const AzVec2& inset_max) const
    {
        return AzVec2(AzMax(0.0f, Size.x - inset_min.x - inset_max.x),
                      AzMax(0.0f, Size.y - inset_min.y - inset_max.y));
    }
    void UpdateWorkRect()
    {
        WorkPos = CalcWorkRectPos(WorkInsetMin);
        WorkSize = CalcWorkRectSize(WorkInsetMin, WorkInsetMax);
    }

    AzRect GetMainRect() const
    {
        return AzRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
    AzRect GetWorkRect() const
    {
        return AzRect(WorkPos.x,
                      WorkPos.y,
                      WorkPos.x + WorkSize.x,
                      WorkPos.y + WorkSize.y);
    }
    AzRect GetBuildWorkRect() const
    {
        AzVec2 pos = CalcWorkRectPos(BuildWorkInsetMin);
        AzVec2 size = CalcWorkRectSize(BuildWorkInsetMin, BuildWorkInsetMax);
        return AzRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }
};

struct AzWindowSettings
{
    AzID ID;
    AzVec2ih Pos;
    AzVec2ih Size;
    AzVec2ih ViewportPos;
    AzID ViewportId;
    AzID DockId;
    AzID ClassId;
    short DockOrder;
    bool Collapsed;
    bool IsChild;
    bool WantApply;
    bool WantDelete;

    AzWindowSettings()
    {
        memset(this, 0, sizeof(*this));
        DockOrder = -1;
    }
    char* GetName()
    {
        return (char*)(this + 1);
    }
};

struct AzSettingsHandler
{
    const char* TypeName;
    AzID TypeHash;
    void (*ClearAllFn)(AzContext* ctx, AzSettingsHandler* handler);
    void (*ReadInitFn)(AzContext* ctx, AzSettingsHandler* handler);
    void* (*ReadOpenFn)(AzContext* ctx,
                        AzSettingsHandler* handler,
                        const char* name);
    void (*ReadLineFn)(AzContext* ctx,
                       AzSettingsHandler* handler,
                       void* entry,
                       const char* line);
    void (*ApplyAllFn)(AzContext* ctx, AzSettingsHandler* handler);
    void (*WriteAllFn)(AzContext* ctx,
                       AzSettingsHandler* handler,
                       AzTextBuffer* out_buf);
    void* UserData;

    AzSettingsHandler()
    {
        memset(this, 0, sizeof(*this));
    }
};

enum AzLocKey : int
{
    AzLocKey_VersionStr,
    AzLocKey_TableSizeOne,
    AzLocKey_TableSizeAllFit,
    AzLocKey_TableSizeAllDefault,
    AzLocKey_TableResetOrder,
    AzLocKey_WindowingMainMenuBar,
    AzLocKey_WindowingPopup,
    AzLocKey_WindowingUntitled,
    AzLocKey_OpenLink_s,
    AzLocKey_CopyLink,
    AzLocKey_DockingHideTabBar,
    AzLocKey_DockingHoldShiftToDock,
    AzLocKey_DockingDragToUndockOrMoveNode,
    AzLocKey_COUNT
};

struct AzLocEntry
{
    AzLocKey Key;
    const char* Text;
};

#ifndef AZ_ASSERT_USER_ERROR
#define AZ_ASSERT_USER_ERROR(_EXPR, _MSG)                                      \
    do                                                                         \
    {                                                                          \
        if (!(_EXPR) && Az::ErrorLog(_MSG))                                    \
        {                                                                      \
            AZ_ASSERT((_EXPR) && _MSG);                                        \
        }                                                                      \
    } while (0)
#endif

typedef void (*AzErrorCallback)(AzContext* ctx,
                                void* user_data,
                                const char* msg);

enum AzDebugLogFlags_
{
    AzDebugLogFlags_None = 0,
    AzDebugLogFlags_EventError = 1 << 0,
    AzDebugLogFlags_EventActiveId = 1 << 1,
    AzDebugLogFlags_EventFocus = 1 << 2,
    AzDebugLogFlags_EventPopup = 1 << 3,
    AzDebugLogFlags_EventNav = 1 << 4,
    AzDebugLogFlags_EventClipper = 1 << 5,
    AzDebugLogFlags_EventSelection = 1 << 6,
    AzDebugLogFlags_EventIO = 1 << 7,
    AzDebugLogFlags_EventFont = 1 << 8,
    AzDebugLogFlags_EventInputRouting = 1 << 9,
    AzDebugLogFlags_EventDocking = 1 << 10,
    AzDebugLogFlags_EventViewport = 1 << 11,

    AzDebugLogFlags_EventMask_ =
            AzDebugLogFlags_EventError | AzDebugLogFlags_EventActiveId
            | AzDebugLogFlags_EventFocus | AzDebugLogFlags_EventPopup
            | AzDebugLogFlags_EventNav | AzDebugLogFlags_EventClipper
            | AzDebugLogFlags_EventSelection | AzDebugLogFlags_EventIO
            | AzDebugLogFlags_EventFont | AzDebugLogFlags_EventInputRouting
            | AzDebugLogFlags_EventDocking | AzDebugLogFlags_EventViewport,
    AzDebugLogFlags_OutputToTTY = 1 << 20,
    AzDebugLogFlags_OutputToTestEngine = 1 << 21,
};

struct AzDebugAllocEntry
{
    int FrameCount;
    AzS16 AllocCount;
    AzS16 FreeCount;
};

struct AzDebugAllocInfo
{
    int TotalAllocCount;
    int TotalFreeCount;
    AzS16 LastEntriesIdx;
    AzDebugAllocEntry LastEntriesBuf[6];

    AzDebugAllocInfo()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzMetricsConfig
{
    bool ShowDebugLog = false;
    bool ShowIDStackTool = false;
    bool ShowWindowsRects = false;
    bool ShowWindowsBeginOrder = false;
    bool ShowTablesRects = false;
    bool ShowDrawCmdMesh = true;
    bool ShowDrawCmdBoundingBoxes = true;
    bool ShowTextEncodingViewer = false;
    bool ShowTextureUsedRect = false;
    bool ShowDockingNodes = false;
    int ShowWindowsRectsType = -1;
    int ShowTablesRectsType = -1;
    int HighlightMonitorIdx = -1;
    AzID HighlightViewportID = 0;
    bool ShowFontPreview = true;
};

struct AzStackLevelInfo
{
    AzID ID;
    AzS8 QueryFrameCount;
    bool QuerySuccess;
    AzDataType DataType : 8;
    char Desc[57];

    AzStackLevelInfo()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzIDStackTool
{
    int LastActiveFrame;
    int StackLevel;
    AzID QueryId;
    AzVector<AzStackLevelInfo> Results;
    bool CopyToClipboardOnCtrlC;
    float CopyToClipboardLastTime;
    AzTextBuffer ResultPathBuf;

    AzIDStackTool()
    {
        memset(this, 0, sizeof(*this));
        CopyToClipboardLastTime = -FLT_MAX;
    }
};

typedef void (*AzContextHookCallback)(AzContext* ctx, AzContextHook* hook);
enum AzContextHookType
{
    AzContextHookType_NewFramePre,
    AzContextHookType_NewFramePost,
    AzContextHookType_EndFramePre,
    AzContextHookType_EndFramePost,
    AzContextHookType_RenderPre,
    AzContextHookType_RenderPost,
    AzContextHookType_Shutdown,
    AzContextHookType_PendingRemoval_
};

struct AzContextHook
{
    AzID HookId;
    AzContextHookType Type;
    AzID Owner;
    AzContextHookCallback Callback;
    void* UserData;

    AzContextHook()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct AzContext
{
    bool Initialized;
    AzIO IO;
    AzPlatformIO PlatformIO;
    AzStyle Style;
    AzConfigFlags ConfigFlagsCurrFrame;
    AzConfigFlags ConfigFlagsLastFrame;
    AzVector<AzFontAtlas*> FontAtlases;
    AzFont* Font;
    AzFontBaked* FontBaked;
    float FontSize;
    float FontSizeBase;
    float FontBakedScale;
    float FontRasterizerDensity;
    float CurrentDpiScale;
    AzDrawListSharedData DrawListSharedData;
    double Time;
    int FrameCount;
    int FrameCountEnded;
    int FrameCountPlatformEnded;
    int FrameCountRendered;
    AzID WithinEndChildID;
    bool WithinFrameScope;
    bool WithinFrameScopeWithAzplicitWindow;
    bool GcCompactAll;
    bool TestEngineHookItems;
    void* TestEngine;
    char ContextName[16];

    AzVector<AzInputEvent> InputEventsQueue;
    AzVector<AzInputEvent> InputEventsTrail;
    AzMouseSource InputEventsNextMouseSource;
    AzU32 InputEventsNextEventId;

    AzVector<AzWindow*> Windows;
    AzVector<AzWindow*> WindowsFocusOrder;
    AzVector<AzWindow*> WindowsTempSortBuffer;
    AzVector<AzWindowStackData> CurrentWindowStack;
    AzStorage WindowsById;
    int WindowsActiveCount;
    float WindowsBorderHoverPadding;
    AzID DebugBreakInWindow;
    AzWindow* CurrentWindow;
    AzWindow* HoveredWindow;
    AzWindow* HoveredWindowUnderMovingWindow;
    AzWindow* HoveredWindowBeforeClear;
    AzWindow* MovingWindow;
    AzWindow* WheelingWindow;
    AzVec2 WheelingWindowRefMousePos;
    int WheelingWindowStartFrame;
    int WheelingWindowScrolledFrame;
    float WheelingWindowReleaseTimer;
    AzVec2 WheelingWindowWheelRemainder;
    AzVec2 WheelingAxisAvg;

    AzID DebugDrawIdConflicts;
    AzID DebugHookIdInfo;
    AzID HoveredId;
    AzID HoveredIdPreviousFrame;
    int HoveredIdPreviousFrameItemCount;
    float HoveredIdTimer;
    float HoveredIdNotActiveTimer;
    bool HoveredIdAllowOverlap;
    bool HoveredIdIsDisabled;
    bool ItemUnclipByLog;
    AzID ActiveId;
    AzID ActiveIdIsAlive;
    float ActiveIdTimer;
    bool ActiveIdIsJustActivated;
    bool ActiveIdAllowOverlap;
    bool ActiveIdNoClearOnFocusLoss;
    bool ActiveIdHasBeenPressedBefore;
    bool ActiveIdHasBeenEditedBefore;
    bool ActiveIdHasBeenEditedThisFrame;
    bool ActiveIdFromShortcut;
    int ActiveIdMouseButton : 8;
    AzVec2 ActiveIdClickOffset;
    AzWindow* ActiveIdWindow;
    AzInputSource ActiveIdSource;
    AzID ActiveIdPreviousFrame;
    AzDeactivatedItemData DeactivatedItemData;
    AzDataTypeStorage ActiveIdValueOnActivation;
    AzID LastActiveId;
    float LastActiveIdTimer;

    double LastKeyModsChangeTime;
    double LastKeyModsChangeFromNoneTime;
    double LastKeyboardKeyPressTime;
    AzBitArrayForNamedKeys KeysMayBeCharInput;
    AzKeyOwnerData KeysOwnerData[AzKey_NamedKey_COUNT];
    AzKeyRoutingTable KeysRoutingTable;
    AzU32 ActiveIdUsingNavDirMask;
    bool ActiveIdUsingAllKeyboardKeys;
    AzKeyChord DebugBreakInShortcutRouting;

    AzID CurrentFocusScopeId;
    AzItemFlags CurrentItemFlags;
    AzID DebugLocateId;
    AzNextItemData NextItemData;
    AzLastItemData LastItemData;
    AzNextWindowData NextWindowData;
    bool DebugShowGroupRects;

    AzCol DebugFlashStyleColorIdx;
    AzVector<AzColorMod> ColorStack;
    AzVector<AzStyleMod> StyleVarStack;
    AzVector<AzFontStackData> FontStack;
    AzVector<AzFocusScopeData> FocusScopeStack;
    AzVector<AzItemFlags> ItemFlagsStack;
    AzVector<AzGroupData> GroupStack;
    AzVector<AzPopupData> OpenPopupStack;
    AzVector<AzPopupData> BeginPopupStack;
    AzVector<AzTreeNodeStackData> TreeNodeStack;

    AzVector<AzViewportP*> Viewports;
    AzViewportP* CurrentViewport;
    AzViewportP* MouseViewport;
    AzViewportP* MouseLastHoveredViewport;
    AzID PlatformLastFocusedViewportId;
    AzPlatformMonitor FallbackMonitor;
    AzRect PlatformMonitorsFullWorkRect;
    int ViewportCreatedCount;
    int PlatformWindowsCreatedCount;
    int ViewportFocusedStampCount;

    bool NavCursorVisible;
    bool NavHighlightItemUnderNav;

    bool NavMousePosDirty;
    bool NavIdIsAlive;
    AzID NavId;
    AzWindow* NavWindow;
    AzID NavFocusScopeId;
    AzNavLayer NavLayer;
    AzID NavActivateId;
    AzID NavActivateDownId;
    AzID NavActivatePressedId;
    AzActivateFlags NavActivateFlags;
    AzVector<AzFocusScopeData> NavFocusRoute;
    AzID NavHighlightActivatedId;
    float NavHighlightActivatedTimer;
    AzID NavNextActivateId;
    AzActivateFlags NavNextActivateFlags;
    AzInputSource NavInputSource;
    AzSelectionUserData NavLastValidSelectionUserData;
    AzS8 NavCursorHideFrames;

    bool NavAnyRequest;
    bool NavInitRequest;
    bool NavInitRequestFromMove;
    AzNavItemData NavInitResult;
    bool NavMoveSubmitted;
    bool NavMoveScoringItems;
    bool NavMoveForwardToNextFrame;
    AzNavMoveFlags NavMoveFlags;
    AzScrollFlags NavMoveScrollFlags;
    AzKeyChord NavMoveKeyMods;
    AzDir NavMoveDir;
    AzDir NavMoveDirForDebug;
    AzDir NavMoveClipDir;
    AzRect NavScoringRect;
    AzRect NavScoringNoClipRect;
    int NavScoringDebugCount;
    int NavTabbingDir;
    int NavTabbingCounter;
    AzNavItemData NavMoveResultLocal;
    AzNavItemData NavMoveResultLocalVisible;
    AzNavItemData NavMoveResultOther;
    AzNavItemData NavTabbingResultFirst;

    AzID NavJustMovedFromFocusScopeId;
    AzID NavJustMovedToId;
    AzID NavJustMovedToFocusScopeId;
    AzKeyChord NavJustMovedToKeyMods;
    bool NavJustMovedToIsTabbing;
    bool NavJustMovedToHasSelectionData;

    bool ConfigNavWindowingWithGamepad;
    AzKeyChord ConfigNavWindowingKeyNext;
    AzKeyChord ConfigNavWindowingKeyPrev;
    AzWindow* NavWindowingTarget;
    AzWindow* NavWindowingTargetAnim;
    AzWindow* NavWindowingListWindow;
    float NavWindowingTimer;
    float NavWindowingHighlightAlpha;
    AzInputSource NavWindowingInputSource;
    bool NavWindowingToggleLayer;
    AzKey NavWindowingToggleKey;
    AzVec2 NavWindowingAccumDeltaPos;
    AzVec2 NavWindowingAccumDeltaSize;

    float DimBgRatio;

    bool DragDropActive;
    bool DragDropWithinSource;
    bool DragDropWithinTarget;
    AzDragDropFlags DragDropSourceFlags;
    int DragDropSourceFrameCount;
    int DragDropMouseButton;
    AzPayload DragDropPayload;
    AzRect DragDropTargetRect;
    AzRect DragDropTargetClipRect;
    AzID DragDropTargetId;
    AzDragDropFlags DragDropAcceptFlags;
    float DragDropAcceptIdCurrRectSurface;
    AzID DragDropAcceptIdCurr;
    AzID DragDropAcceptIdPrev;
    int DragDropAcceptFrameCount;
    AzID DragDropHoldJustPressedId;
    AzVector<unsigned char> DragDropPayloadBufHeap;
    unsigned char DragDropPayloadBufLocal[16];

    int ClipperTempDataStacked;
    AzVector<AzListClipperData> ClipperTempData;

    AzTable* CurrentTable;
    AzID DebugBreakInTable;
    int TablesTempDataStacked;
    AzVector<AzTableTempData> TablesTempData;
    AzPool<AzTable> Tables;
    AzVector<float> TablesLastTimeActive;
    AzVector<AzDrawChannel> DrawChannelsTempMergeBuffer;

    AzTabBar* CurrentTabBar;
    AzPool<AzTabBar> TabBars;
    AzVector<AzPtrOrIndex> CurrentTabBarStack;
    AzVector<AzShrinkWidthItem> ShrinkWidthBuffer;

    AzBoxSelectState BoxSelectState;
    AzMultiSelectTempData* CurrentMultiSelect;
    int MultiSelectTempDataStacked;
    AzVector<AzMultiSelectTempData> MultiSelectTempData;
    AzPool<AzMultiSelectState> MultiSelectStorage;

    AzID HoverItemDelayId;
    AzID HoverItemDelayIdPreviousFrame;
    float HoverItemDelayTimer;
    float HoverItemDelayClearTimer;
    AzID HoverItemUnlockedStationaryId;
    AzID HoverWindowUnlockedStationaryId;

    AzMouseCursor MouseCursor;
    float MouseStationaryTimer;
    AzVec2 MouseLastValidPos;

    AzInputTextState InputTextState;
    AzInputTextDeactivatedState InputTextDeactivatedState;
    AzFontBaked InputTextPasswordFontBackupBaked;
    AzFontFlags InputTextPasswordFontBackupFlags;
    AzID TempInputId;
    AzDataTypeStorage DataTypeZeroValue;
    int BeginMenuDepth;
    int BeginComboDepth;
    AzColorEditFlags ColorEditOptions;
    AzID ColorEditCurrentID;
    AzID ColorEditSavedID;
    float ColorEditSavedHue;
    float ColorEditSavedSat;
    AzU32 ColorEditSavedColor;
    AzVec4 ColorPickerRef;
    AzComboPreviewData ComboPreviewData;
    AzRect WindowResizeBorderExpectedRect;
    bool WindowResizeRelativeMode;
    short ScrollbarSeekMode;
    float ScrollbarClickDeltaToGrabCenter;
    float SliderGrabClickOffset;
    float SliderCurrentAccum;
    bool SliderCurrentAccumDirty;
    bool DragCurrentAccumDirty;
    float DragCurrentAccum;
    float DragSpeedDefaultRatio;
    float DisabledAlphaBackup;
    short DisabledStackSize;
    short TooltipOverrideCount;
    AzWindow* TooltipPreviousWindow;
    AzVector<char> ClipboardHandlerData;
    AzVector<AzID> MenusIdSubmittedThisFrame;
    AzTypingSelectState TypingSelectState;

    AzPlatformAzeData PlatformAzeData;
    AzPlatformAzeData PlatformAzeDataPrev;

    AzVector<AzTextureData*> UserTextures;
    AzDockContext DockContext;
    void (*DockNodeWindowMenuHandler)(AzContext* ctx,
                                      AzDockNode* node,
                                      AzTabBar* tab_bar);

    bool SettingsLoaded;
    float SettingsDirtyTimer;
    AzTextBuffer SettingsIniData;
    AzVector<AzSettingsHandler> SettingsHandlers;
    AzChunkStream<AzWindowSettings> SettingsWindows;
    AzChunkStream<AzTableSettings> SettingsTables;
    AzVector<AzContextHook> Hooks;
    AzID HookIdNext;

    const char* LocalizationTable[AzLocKey_COUNT];

    bool LogEnabled;
    AzLogFlags LogFlags;
    AzWindow* LogWindow;
    AzFileHandle LogFile;
    AzTextBuffer LogBuffer;
    const char* LogNextPrefix;
    const char* LogNextSuffix;
    float LogLinePosY;
    bool LogLineFirstItem;
    int LogDepthRef;
    int LogDepthToExpand;
    int LogDepthToExpandDefault;

    AzErrorCallback ErrorCallback;
    void* ErrorCallbackUserData;
    AzVec2 ErrorTooltipLockedPos;
    bool ErrorFirst;
    int ErrorCountCurrentFrame;
    AzErrorRecoveryState StackSizesInNewFrame;
    AzErrorRecoveryState* StackSizesInBeginForCurrentWindow;

    int DebugDrawIdConflictsCount;
    AzDebugLogFlags DebugLogFlags;
    AzTextBuffer DebugLogBuf;
    AzTextIndex DebugLogIndex;
    int DebugLogSkippedErrors;
    AzDebugLogFlags DebugLogAutoDisableFlags;
    AzU8 DebugLogAutoDisableFrames;
    AzU8 DebugLocateFrames;
    bool DebugBreakInLocateId;
    AzKeyChord DebugBreakKeyChord;
    AzS8 DebugBeginReturnValueCullDepth;
    bool DebugItemPickerActive;
    AzU8 DebugItemPickerMouseButton;
    AzID DebugItemPickerBreakId;
    float DebugFlashStyleColorTime;
    AzVec4 DebugFlashStyleColorBackup;
    AzMetricsConfig DebugMetricsConfig;
    AzIDStackTool DebugIDStackTool;
    AzDebugAllocInfo DebugAllocInfo;
    AzDockNode* DebugHoveredDockNode;

    float FramerateSecPerFrame[60];
    int FramerateSecPerFrameIdx;
    int FramerateSecPerFrameCount;
    float FramerateSecPerFrameAccum;
    int WantCaptureMouseNextFrame;
    int WantCaptureKeyboardNextFrame;
    int WantTextInputNextFrame;
    AzVector<char> TempBuffer;
    char TempKeychordName[64];

    AzContext(AzFontAtlas* shared_font_atlas);
};

struct AZ_API AzWindowTempData
{
    AzVec2 CursorPos;
    AzVec2 CursorPosPrevLine;
    AzVec2 CursorStartPos;
    AzVec2 CursorMaxPos;
    AzVec2 IdealMaxPos;
    AzVec2 CurrLineSize;
    AzVec2 PrevLineSize;
    float CurrLineTextBaseOffset;
    float PrevLineTextBaseOffset;
    bool IsSameLine;
    bool IsSetPos;
    AzVec1 Indent;
    AzVec1 ColumnsOffset;
    AzVec1 GroupOffset;
    AzVec2 CursorStartPosLossyness;

    AzNavLayer NavLayerCurrent;
    short NavLayersActiveMask;
    short NavLayersActiveMaskNext;
    bool NavIsScrollPushableX;
    bool NavHideHighlightOneFrame;
    bool NavWindowHasScrollY;

    bool MenuBarAppending;
    AzVec2 MenuBarOffset;
    AzMenuColumns MenuColumns;
    int TreeDepth;
    AzU32 TreeHasStackDataDepthMask;
    AzU32 TreeRecordsClippedNodesY2Mask;
    AzVector<AzWindow*> ChildWindows;
    AzStorage* StateStorage;
    AzOldColumns* CurrentColumns;
    int CurrentTableIdx;
    AzLayoutType LayoutType;
    AzLayoutType ParentLayoutType;
    AzU32 ModalDimBgColor;

    AzItemStatusFlags WindowItemStatusFlags;
    AzItemStatusFlags ChildItemStatusFlags;
    AzItemStatusFlags DockTabItemStatusFlags;
    AzRect DockTabItemRect;

    float ItemWidth;
    float TextWrapPos;
    AzVector<float> ItemWidthStack;
    AzVector<float> TextWrapPosStack;
};

struct AZ_API AzWindow
{
    AzContext* Ctx;
    char* Name;
    AzID ID;
    AzWindowFlags Flags, FlagsPreviousFrame;
    AzChildFlags ChildFlags;
    AzWindowClass WindowClass;
    AzViewportP* Viewport;
    AzID ViewportId;
    AzVec2 ViewportPos;
    int ViewportAllowPlatformMonitorExtend;
    AzVec2 Pos;
    AzVec2 Size;
    AzVec2 SizeFull;
    AzVec2 ContentSize;
    AzVec2 ContentSizeIdeal;
    AzVec2 ContentSizeExplicit;
    AzVec2 WindowPadding;
    float WindowRounding;
    float WindowBorderSize;
    float TitleBarHeight, MenuBarHeight;
    float DecoOuterSizeX1, DecoOuterSizeY1;
    float DecoOuterSizeX2, DecoOuterSizeY2;
    float DecoInnerSizeX1, DecoInnerSizeY1;
    int NameBufLen;
    AzID MoveId;
    AzID TabId;
    AzID ChildId;
    AzID PopupId;
    AzVec2 Scroll;
    AzVec2 ScrollMax;
    AzVec2 ScrollTarget;
    AzVec2 ScrollTargetCenterRatio;
    AzVec2 ScrollTargetEdgeSnapDist;
    AzVec2 ScrollbarSizes;
    bool ScrollbarX, ScrollbarY;
    bool ScrollbarXStabilizeEnabled;
    AzU8 ScrollbarXStabilizeToggledHistory;
    bool ViewportOwned;
    bool Active;
    bool WasActive;
    bool WriteAccessed;
    bool Collapsed;
    bool WantCollapseToggle;
    bool SkipItems;
    bool SkipRefresh;
    bool Appearing;
    bool Hidden;
    bool IsFallbackWindow;
    bool IsExplicitChild;
    bool HasCloseButton;
    signed char ResizeBorderHovered;
    signed char ResizeBorderHeld;
    short BeginCount;
    short BeginCountPreviousFrame;
    short BeginOrderWithinParent;
    short BeginOrderWithinContext;
    short FocusOrder;
    AzS8 AutoFitFramesX, AutoFitFramesY;
    bool AutoFitOnlyGrows;
    AzDir AutoPosLastDirection;
    AzS8 HiddenFramesCanSkipItems;
    AzS8 HiddenFramesCannotSkipItems;
    AzS8 HiddenFramesForRenderOnly;
    AzS8 DisableInputsFrames;
    AzCond SetWindowPosAllowFlags : 8;
    AzCond SetWindowSizeAllowFlags : 8;
    AzCond SetWindowCollapsedAllowFlags : 8;
    AzCond SetWindowDockAllowFlags : 8;
    AzVec2 SetWindowPosVal;
    AzVec2 SetWindowPosPivot;

    AzVector<AzID> IDStack;
    AzWindowTempData DC;

    AzRect OuterRectClipped;
    AzRect InnerRect;
    AzRect InnerClipRect;
    AzRect WorkRect;
    AzRect ParentWorkRect;
    AzRect ClipRect;
    AzRect ContentRegionRect;
    AzVec2ih HitTestHoleSize;
    AzVec2ih HitTestHoleOffset;

    int LastFrameActive;
    int LastFrameJustFocused;
    float LastTimeActive;
    float ItemWidthDefault;
    AzStorage StateStorage;
    AzVector<AzOldColumns> ColumnsStorage;
    float FontWindowScale;
    float FontWindowScaleParents;
    float FontRefSize;
    int SettingsOffset;

    AzDrawList* DrawList;
    AzDrawList DrawListInst;
    AzWindow* ParentWindow;
    AzWindow* ParentWindowInBeginStack;
    AzWindow* RootWindow;
    AzWindow* RootWindowPopupTree;
    AzWindow* RootWindowDockTree;
    AzWindow* RootWindowForTitleBarHighlight;
    AzWindow* RootWindowForNav;
    AzWindow* ParentWindowForFocusRoute;

    AzWindow* NavLastChildNavWindow;
    AzID NavLastIds[AzNavLayer_COUNT];
    AzRect NavRectRel[AzNavLayer_COUNT];
    AzVec2 NavPreferredScoringPosRel[AzNavLayer_COUNT];
    AzID NavRootFocusScopeId;

    int MemoryDrawListIdxCapacity;
    int MemoryDrawListVtxCapacity;
    bool MemoryCompacted;

    bool DockIsActive : 1;
    bool DockNodeIsVisible : 1;
    bool DockTabIsVisible : 1;
    bool DockTabWantClose : 1;
    short DockOrder;
    AzWindowDockStyle DockStyle;
    AzDockNode* DockNode;
    AzDockNode* DockNodeAsHost;
    AzID DockId;

public:
    AzWindow(AzContext* context, const char* name);
    ~AzWindow();

    AzID GetID(const char* str, const char* str_end = NULL);
    AzID GetID(const void* ptr);
    AzID GetID(int n);
    AzID GetIDFromPos(const AzVec2& p_abs);
    AzID GetIDFromRectangle(const AzRect& r_abs);

    AzRect Rect() const
    {
        return AzRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
    AzRect TitleBarRect() const
    {
        return AzRect(Pos, AzVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight));
    }
    AzRect MenuBarRect() const
    {
        float y1 = Pos.y + TitleBarHeight;
        return AzRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight);
    }
};

enum AzTabBarFlagsPrivate_
{
    AzTabBarFlags_DockNode = 1 << 20,
    AzTabBarFlags_IsFocused = 1 << 21,
    AzTabBarFlags_SaveSettings = 1 << 22,
};

enum AzTabItemFlagsPrivate_
{
    AzTabItemFlags_SectionMask_ = AzTabItemFlags_Leading
                                  | AzTabItemFlags_Trailing,
    AzTabItemFlags_NoCloseButton = 1 << 20,
    AzTabItemFlags_Button = 1 << 21,
    AzTabItemFlags_Invisible = 1 << 22,
    AzTabItemFlags_Unsorted = 1 << 23,
};

struct AzTabItem
{
    AzID ID;
    AzTabItemFlags Flags;
    AzWindow* Window;
    int LastFrameVisible;
    int LastFrameSelected;
    float Offset;
    float Width;
    float ContentWidth;
    float RequestedWidth;
    AzS32 NameOffset;
    AzS16 BeginOrder;
    AzS16 IndexDuringLayout;
    bool WantClose;

    AzTabItem()
    {
        memset(this, 0, sizeof(*this));
        LastFrameVisible = LastFrameSelected = -1;
        RequestedWidth = -1.0f;
        NameOffset = -1;
        BeginOrder = IndexDuringLayout = -1;
    }
};

struct AZ_API AzTabBar
{
    AzWindow* Window;
    AzVector<AzTabItem> Tabs;
    AzTabBarFlags Flags;
    AzID ID;
    AzID SelectedTabId;
    AzID NextSelectedTabId;
    AzID VisibleTabId;
    int CurrFrameVisible;
    int PrevFrameVisible;
    AzRect BarRect;
    float CurrTabsContentsHeight;
    float PrevTabsContentsHeight;
    float WidthAllTabs;
    float WidthAllTabsIdeal;
    float ScrollingAnim;
    float ScrollingTarget;
    float ScrollingTargetDistToVisibility;
    float ScrollingSpeed;
    float ScrollingRectMinX;
    float ScrollingRectMaxX;
    float SeparatorMinX;
    float SeparatorMaxX;
    AzID ReorderRequestTabId;
    AzS16 ReorderRequestOffset;
    AzS8 BeginCount;
    bool WantLayout;
    bool VisibleTabWasSubmitted;
    bool TabsAddedNew;
    AzS16 TabsActiveCount;
    AzS16 LastTabItemIdx;
    float ItemSpacingY;
    AzVec2 FramePadding;
    AzVec2 BackupCursorPos;
    AzTextBuffer TabsNames;

    AzTabBar();
};

#define AZ_COL32_DISABLE AZ_COL32(0, 0, 0, 1)
#define AZ_TABLE_MAX_COLUMNS 512

struct AzTableColumn
{
    AzTableColumnFlags Flags;
    float WidthGiven;
    float MinX;
    float MaxX;
    float WidthRequest;
    float WidthAuto;
    float WidthMax;
    float StretchWeight;
    float InitStretchWeightOrWidth;
    AzRect ClipRect;
    AzID UserID;
    float WorkMinX;
    float WorkMaxX;
    float ItemWidth;
    float ContentMaxXFrozen;
    float ContentMaxXUnfrozen;
    float ContentMaxXHeadersUsed;
    float ContentMaxXHeadersIdeal;
    AzS16 NameOffset;
    AzTableColumnIdx DisplayOrder;
    AzTableColumnIdx IndexWithinEnabledSet;
    AzTableColumnIdx PrevEnabledColumn;
    AzTableColumnIdx NextEnabledColumn;
    AzTableColumnIdx SortOrder;
    AzTableDrawChannelIdx DrawChannelCurrent;
    AzTableDrawChannelIdx DrawChannelFrozen;
    AzTableDrawChannelIdx DrawChannelUnfrozen;
    bool IsEnabled;
    bool IsUserEnabled;
    bool IsUserEnabledNextFrame;
    bool IsVisibleX;
    bool IsVisibleY;
    bool IsRequestOutput;
    bool IsSkipItems;
    bool IsPreserveWidthAuto;
    AzS8 NavLayerCurrent;
    AzU8 AutoFitQueue;
    AzU8 CannotSkipItemsQueue;
    AzU8 SortDirection : 2;
    AzU8 SortDirectionsAvailCount : 2;
    AzU8 SortDirectionsAvailMask : 4;
    AzU8 SortDirectionsAvailList;

    AzTableColumn()
    {
        memset(this, 0, sizeof(*this));
        StretchWeight = WidthRequest = -1.0f;
        NameOffset = -1;
        DisplayOrder = IndexWithinEnabledSet = -1;
        PrevEnabledColumn = NextEnabledColumn = -1;
        SortOrder = -1;
        SortDirection = AzSortDirection_None;
        DrawChannelCurrent = DrawChannelFrozen = DrawChannelUnfrozen = (AzU8)-1;
    }
};

struct AzTableCellData
{
    AzU32 BgColor;
    AzTableColumnIdx Column;
};

struct AzTableHeaderData
{
    AzTableColumnIdx Index;
    AzU32 TextColor;
    AzU32 BgColor0;
    AzU32 BgColor1;
};

struct AzTableInstanceData
{
    AzID TableInstanceID;
    float LastOuterHeight;
    float LastTopHeadersRowHeight;
    float LastFrozenHeight;
    int HoveredRowLast;
    int HoveredRowNext;

    AzTableInstanceData()
    {
        TableInstanceID = 0;
        LastOuterHeight = LastTopHeadersRowHeight = LastFrozenHeight = 0.0f;
        HoveredRowLast = HoveredRowNext = -1;
    }
};

struct AZ_API AzTable
{
    AzID ID;
    AzTableFlags Flags;
    void* RawData;
    AzTableTempData* TempData;
    AzSpan<AzTableColumn> Columns;
    AzSpan<AzTableColumnIdx> DisplayOrderToIndex;
    AzSpan<AzTableCellData> RowCellData;
    AzBitArrayPtr EnabledMaskByDisplayOrder;
    AzBitArrayPtr EnabledMaskByIndex;
    AzBitArrayPtr VisibleMaskByIndex;
    AzTableFlags SettingsLoadedFlags;
    int SettingsOffset;
    int LastFrameActive;
    int ColumnsCount;
    int CurrentRow;
    int CurrentColumn;
    AzS16 InstanceCurrent;
    AzS16 InstanceInteracted;
    float RowPosY1;
    float RowPosY2;
    float RowMinHeight;
    float RowCellPaddingY;
    float RowTextBaseline;
    float RowIndentOffsetX;
    AzTableRowFlags RowFlags : 16;
    AzTableRowFlags LastRowFlags : 16;
    int RowBgColorCounter;
    AzU32 RowBgColor[2];
    AzU32 BorderColorStrong;
    AzU32 BorderColorLight;
    float BorderX1;
    float BorderX2;
    float HostIndentX;
    float MinColumnWidth;
    float OuterPaddingX;
    float CellPaddingX;
    float CellSpacingX1;
    float CellSpacingX2;
    float InnerWidth;
    float ColumnsGivenWidth;
    float ColumnsAutoFitWidth;
    float ColumnsStretchSumWeights;
    float ResizedColumnNextWidth;
    float ResizeLockMinContentsX2;
    float RefScale;
    float AngledHeadersHeight;
    float AngledHeadersSlope;
    AzRect OuterRect;
    AzRect InnerRect;
    AzRect WorkRect;
    AzRect InnerClipRect;
    AzRect BgClipRect;
    AzRect Bg0ClipRectForDrawCmd;
    AzRect Bg2ClipRectForDrawCmd;
    AzRect HostClipRect;
    AzRect HostBackupInnerClipRect;
    AzWindow* OuterWindow;
    AzWindow* InnerWindow;
    AzTextBuffer ColumnsNames;
    AzDrawListSplitter* DrawSplitter;
    AzTableInstanceData InstanceDataFirst;
    AzVector<AzTableInstanceData> InstanceDataExtra;
    AzTableColumnSortSpecs SortSpecsSingle;
    AzVector<AzTableColumnSortSpecs> SortSpecsMulti;
    AzTableSortSpecs SortSpecs;
    AzTableColumnIdx SortSpecsCount;
    AzTableColumnIdx ColumnsEnabledCount;
    AzTableColumnIdx ColumnsEnabledFixedCount;
    AzTableColumnIdx DeclColumnsCount;
    AzTableColumnIdx AngledHeadersCount;
    AzTableColumnIdx HoveredColumnBody;
    AzTableColumnIdx HoveredColumnBorder;
    AzTableColumnIdx HighlightColumnHeader;
    AzTableColumnIdx AutoFitSingleColumn;
    AzTableColumnIdx ResizedColumn;
    AzTableColumnIdx LastResizedColumn;
    AzTableColumnIdx HeldHeaderColumn;
    AzTableColumnIdx ReorderColumn;
    AzTableColumnIdx ReorderColumnDir;
    AzTableColumnIdx LeftMostEnabledColumn;
    AzTableColumnIdx RightMostEnabledColumn;
    AzTableColumnIdx LeftMostStretchedColumn;
    AzTableColumnIdx RightMostStretchedColumn;
    AzTableColumnIdx ContextPopupColumn;
    AzTableColumnIdx FreezeRowsRequest;
    AzTableColumnIdx FreezeRowsCount;
    AzTableColumnIdx FreezeColumnsRequest;
    AzTableColumnIdx FreezeColumnsCount;
    AzTableColumnIdx RowCellDataCurrent;
    AzTableDrawChannelIdx DummyDrawChannel;
    AzTableDrawChannelIdx Bg2DrawChannelCurrent;
    AzTableDrawChannelIdx Bg2DrawChannelUnfrozen;
    AzS8 NavLayer;
    bool IsLayoutLocked;
    bool IsInsideRow;
    bool IsInitializing;
    bool IsSortSpecsDirty;
    bool IsUsingHeaders;
    bool IsContextPopupOpen;
    bool DisableDefaultContextMenu;
    bool IsSettingsRequestLoad;
    bool IsSettingsDirty;
    bool IsDefaultDisplayOrder;
    bool IsResetAllRequest;
    bool IsResetDisplayOrderRequest;
    bool IsUnfrozenRows;
    bool IsDefaultSizingPolicy;
    bool IsActiveIdAliveBeforeTable;
    bool IsActiveIdInTable;
    bool HasScrollbarYCurr;
    bool HasScrollbarYPrev;
    bool MemoryCompacted;
    bool HostSkipItems;

    AzTable()
    {
        memset(this, 0, sizeof(*this));
        LastFrameActive = -1;
    }
    ~AzTable()
    {
        AZ_FREE(RawData);
    }
};

struct AZ_API AzTableTempData
{
    int TableIndex;
    float LastTimeActive;
    float AngledHeadersExtraWidth;
    AzVector<AzTableHeaderData> AngledHeadersRequests;

    AzVec2 UserOuterSize;
    AzDrawListSplitter DrawSplitter;

    AzRect HostBackupWorkRect;
    AzRect HostBackupParentWorkRect;
    AzVec2 HostBackupPrevLineSize;
    AzVec2 HostBackupCurrLineSize;
    AzVec2 HostBackupCursorMaxPos;
    AzVec1 HostBackupColumnsOffset;
    float HostBackupItemWidth;
    int HostBackupItemWidthStackSize;

    AzTableTempData()
    {
        memset(this, 0, sizeof(*this));
        LastTimeActive = -1.0f;
    }
};

struct AzTableColumnSettings
{
    float WidthOrWeight;
    AzID UserID;
    AzTableColumnIdx Index;
    AzTableColumnIdx DisplayOrder;
    AzTableColumnIdx SortOrder;
    AzU8 SortDirection : 2;
    AzS8 IsEnabled : 2;
    AzU8 IsStretch : 1;

    AzTableColumnSettings()
    {
        WidthOrWeight = 0.0f;
        UserID = 0;
        Index = -1;
        DisplayOrder = SortOrder = -1;
        SortDirection = AzSortDirection_None;
        IsEnabled = -1;
        IsStretch = 0;
    }
};

struct AzTableSettings
{
    AzID ID;
    AzTableFlags SaveFlags;
    float RefScale;
    AzTableColumnIdx ColumnsCount;
    AzTableColumnIdx ColumnsCountMax;
    bool WantApply;

    AzTableSettings()
    {
        memset(this, 0, sizeof(*this));
    }
    AzTableColumnSettings* GetColumnSettings()
    {
        return (AzTableColumnSettings*)(this + 1);
    }
};

namespace Az
{

    AZ_API AzIO& GetIO(AzContext* ctx);
    AZ_API AzPlatformIO& GetPlatformIO(AzContext* ctx);
    inline AzWindow* GetCurrentWindowRead()
    {
        AzContext& g = *GAz;
        return g.CurrentWindow;
    }
    inline AzWindow* GetCurrentWindow()
    {
        AzContext& g = *GAz;
        g.CurrentWindow->WriteAccessed = true;
        return g.CurrentWindow;
    }
    AZ_API AzWindow* FindWindowByID(AzID id);
    AZ_API AzWindow* FindWindowByName(const char* name);
    AZ_API void UpdateWindowParentAndRootLinks(AzWindow* window,
                                               AzWindowFlags flags,
                                               AzWindow* parent_window);
    AZ_API void UpdateWindowSkipRefresh(AzWindow* window);
    AZ_API AzVec2 CalcWindowNextAutoFitSize(AzWindow* window);
    AZ_API bool IsWindowChildOf(AzWindow* window,
                                AzWindow* potential_parent,
                                bool popup_hierarchy,
                                bool dock_hierarchy);
    AZ_API bool IsWindowWithinBeginStackOf(AzWindow* window,
                                           AzWindow* potential_parent);
    AZ_API bool IsWindowAbove(AzWindow* potential_above,
                              AzWindow* potential_below);
    AZ_API bool IsWindowNavFocusable(AzWindow* window);
    AZ_API void SetWindowPos(AzWindow* window,
                             const AzVec2& pos,
                             AzCond cond = 0);
    AZ_API void SetWindowSize(AzWindow* window,
                              const AzVec2& size,
                              AzCond cond = 0);
    AZ_API void SetWindowCollapsed(AzWindow* window,
                                   bool collapsed,
                                   AzCond cond = 0);
    AZ_API void SetWindowHitTestHole(AzWindow* window,
                                     const AzVec2& pos,
                                     const AzVec2& size);
    AZ_API void SetWindowHiddenAndSkipItemsForCurrentFrame(AzWindow* window);
    inline void SetWindowParentWindowForFocusRoute(AzWindow* window,
                                                   AzWindow* parent_window)
    {
        window->ParentWindowForFocusRoute = parent_window;
    }
    inline AzRect WindowRectAbsToRel(AzWindow* window, const AzRect& r)
    {
        AzVec2 off = window->DC.CursorStartPos;
        return AzRect(r.Min.x - off.x,
                      r.Min.y - off.y,
                      r.Max.x - off.x,
                      r.Max.y - off.y);
    }
    inline AzRect WindowRectRelToAbs(AzWindow* window, const AzRect& r)
    {
        AzVec2 off = window->DC.CursorStartPos;
        return AzRect(r.Min.x + off.x,
                      r.Min.y + off.y,
                      r.Max.x + off.x,
                      r.Max.y + off.y);
    }
    inline AzVec2 WindowPosAbsToRel(AzWindow* window, const AzVec2& p)
    {
        AzVec2 off = window->DC.CursorStartPos;
        return AzVec2(p.x - off.x, p.y - off.y);
    }
    inline AzVec2 WindowPosRelToAbs(AzWindow* window, const AzVec2& p)
    {
        AzVec2 off = window->DC.CursorStartPos;
        return AzVec2(p.x + off.x, p.y + off.y);
    }

    AZ_API void FocusWindow(AzWindow* window, AzFocusRequestFlags flags = 0);
    AZ_API void FocusTopMostWindowUnderOne(AzWindow* under_this_window,
                                           AzWindow* ignore_window,
                                           AzViewport* filter_viewport,
                                           AzFocusRequestFlags flags);
    AZ_API void BringWindowToFocusFront(AzWindow* window);
    AZ_API void BringWindowToDisplayFront(AzWindow* window);
    AZ_API void BringWindowToDisplayBack(AzWindow* window);
    AZ_API void BringWindowToDisplayBehind(AzWindow* window,
                                           AzWindow* above_window);
    AZ_API int FindWindowDisplayIndex(AzWindow* window);
    AZ_API AzWindow* FindBottomMostVisibleWindowWithinBeginStack(
            AzWindow* window);

    AZ_API void SetNextWindowRefreshPolicy(AzWindowRefreshFlags flags);

    AZ_API void RegisterUserTexture(AzTextureData* tex);
    AZ_API void UnregisterUserTexture(AzTextureData* tex);
    AZ_API void RegisterFontAtlas(AzFontAtlas* atlas);
    AZ_API void UnregisterFontAtlas(AzFontAtlas* atlas);
    AZ_API void SetCurrentFont(AzFont* font,
                               float font_size_before_scaling,
                               float font_size_after_scaling);
    AZ_API void UpdateCurrentFontSize(float restore_font_size_after_scaling);
    AZ_API void SetFontRasterizerDensity(float rasterizer_density);
    inline float GetFontRasterizerDensity()
    {
        return GAz->FontRasterizerDensity;
    }
    inline float GetRoundedFontSize(float size)
    {
        return AZ_ROUND(size);
    }
    AZ_API AzFont* GetDefaultFont();
    AZ_API void PushPasswordFont();
    AZ_API void PopPasswordFont();
    inline AzDrawList* GetForegroundDrawList(AzWindow* window)
    {
        return GetForegroundDrawList(window->Viewport);
    }
    AZ_API void AddDrawListToDrawDataEx(AzDrawData* draw_data,
                                        AzVector<AzDrawList*>* out_list,
                                        AzDrawList* draw_list);

    AZ_API void Initialize();
    AZ_API void Shutdown();

    AZ_API void UpdateInputEvents(bool trickle_fast_inputs);
    AZ_API void UpdateHoveredWindowAndCaptureFlags(const AzVec2& mouse_pos);
    AZ_API void FindHoveredWindowEx(
            const AzVec2& pos,
            bool find_first_and_in_any_viewport,
            AzWindow** out_hovered_window,
            AzWindow** out_hovered_window_under_moving_window);
    AZ_API void StartMouseMovingWindow(AzWindow* window);
    AZ_API void StartMouseMovingWindowOrNode(AzWindow* window,
                                             AzDockNode* node,
                                             bool undock);
    AZ_API void UpdateMouseMovingWindowNewFrame();
    AZ_API void UpdateMouseMovingWindowEndFrame();

    AZ_API AzID AddContextHook(AzContext* context, const AzContextHook* hook);
    AZ_API void RemoveContextHook(AzContext* context, AzID hook_to_remove);
    AZ_API void CallContextHooks(AzContext* context, AzContextHookType type);

    AZ_API void TranslateWindowsInViewport(AzViewportP* viewport,
                                           const AzVec2& old_pos,
                                           const AzVec2& new_pos,
                                           const AzVec2& old_size,
                                           const AzVec2& new_size);
    AZ_API void ScaleWindowsInViewport(AzViewportP* viewport, float scale);
    AZ_API void DestroyPlatformWindow(AzViewportP* viewport);
    AZ_API void SetWindowViewport(AzWindow* window, AzViewportP* viewport);
    AZ_API void SetCurrentViewport(AzWindow* window, AzViewportP* viewport);
    AZ_API const AzPlatformMonitor* GetViewportPlatformMonitor(
            AzViewport* viewport);
    AZ_API AzViewportP* FindHoveredViewportFromPlatformWindowStack(
            const AzVec2& mouse_platform_pos);

    AZ_API void MarkIniSettingsDirty();
    AZ_API void MarkIniSettingsDirty(AzWindow* window);
    AZ_API void ClearIniSettings();
    AZ_API void AddSettingsHandler(const AzSettingsHandler* handler);
    AZ_API void RemoveSettingsHandler(const char* type_name);
    AZ_API AzSettingsHandler* FindSettingsHandler(const char* type_name);

    AZ_API AzWindowSettings* CreateNewWindowSettings(const char* name);
    AZ_API AzWindowSettings* FindWindowSettingsByID(AzID id);
    AZ_API AzWindowSettings* FindWindowSettingsByWindow(AzWindow* window);
    AZ_API void ClearWindowSettings(const char* name);

    AZ_API void LocalizeRegisterEntries(const AzLocEntry* entries, int count);
    inline const char* LocalizeGetMsg(AzLocKey key)
    {
        AzContext& g = *GAz;
        const char* msg = g.LocalizationTable[key];
        return msg ? msg : "*Missing Text*";
    }

    AZ_API void SetScrollX(AzWindow* window, float scroll_x);
    AZ_API void SetScrollY(AzWindow* window, float scroll_y);
    AZ_API void SetScrollFromPosX(AzWindow* window,
                                  float local_x,
                                  float center_x_ratio);
    AZ_API void SetScrollFromPosY(AzWindow* window,
                                  float local_y,
                                  float center_y_ratio);

    AZ_API void ScrollToItem(AzScrollFlags flags = 0);
    AZ_API void ScrollToRect(AzWindow* window,
                             const AzRect& rect,
                             AzScrollFlags flags = 0);
    AZ_API AzVec2 ScrollToRectEx(AzWindow* window,
                                 const AzRect& rect,
                                 AzScrollFlags flags = 0);

    inline void ScrollToBringRectIntoView(AzWindow* window, const AzRect& rect)
    {
        ScrollToRect(window, rect, AzScrollFlags_KeepVisibleEdgeY);
    }

    inline AzItemStatusFlags GetItemStatusFlags()
    {
        AzContext& g = *GAz;
        return g.LastItemData.StatusFlags;
    }
    inline AzItemFlags GetItemFlags()
    {
        AzContext& g = *GAz;
        return g.LastItemData.ItemFlags;
    }
    inline AzID GetActiveID()
    {
        AzContext& g = *GAz;
        return g.ActiveId;
    }
    inline AzID GetFocusID()
    {
        AzContext& g = *GAz;
        return g.NavId;
    }
    AZ_API void SetActiveID(AzID id, AzWindow* window);
    AZ_API void SetFocusID(AzID id, AzWindow* window);
    AZ_API void ClearActiveID();
    AZ_API AzID GetHoveredID();
    AZ_API void SetHoveredID(AzID id);
    AZ_API void KeepAliveID(AzID id);
    AZ_API void MarkItemEdited(AzID id);
    AZ_API void PushOverrideID(AzID id);
    AZ_API AzID GetIDWithSeed(const char* str_id_begin,
                              const char* str_id_end,
                              AzID seed);
    AZ_API AzID GetIDWithSeed(int n, AzID seed);

    AZ_API void ItemSize(const AzVec2& size, float text_baseline_y = -1.0f);
    inline void ItemSize(const AzRect& bb, float text_baseline_y = -1.0f)
    {
        ItemSize(bb.GetSize(), text_baseline_y);
    }
    AZ_API bool ItemAdd(const AzRect& bb,
                        AzID id,
                        const AzRect* nav_bb = NULL,
                        AzItemFlags extra_flags = 0);
    AZ_API bool ItemHoverable(const AzRect& bb,
                              AzID id,
                              AzItemFlags item_flags);
    AZ_API bool IsWindowContentHoverable(AzWindow* window,
                                         AzHoveredFlags flags = 0);
    AZ_API bool IsClippedEx(const AzRect& bb, AzID id);
    AZ_API void SetLastItemData(AzID item_id,
                                AzItemFlags item_flags,
                                AzItemStatusFlags status_flags,
                                const AzRect& item_rect);
    AZ_API AzVec2 CalcItemSize(AzVec2 size, float default_w, float default_h);
    AZ_API float CalcWrapWidthForPos(const AzVec2& pos, float wrap_pos_x);
    AZ_API void PushMultiItemsWidths(int components, float width_full);
    AZ_API void ShrinkWidths(AzShrinkWidthItem* items,
                             int count,
                             float width_excess);

    AZ_API const AzStyleVarInfo* GetStyleVarInfo(AzStyleVar idx);
    AZ_API void BeginDisabledOverrideReenable();
    AZ_API void EndDisabledOverrideReenable();

    AZ_API void LogBegin(AzLogFlags flags, int auto_open_depth);
    AZ_API void LogToBuffer(int auto_open_depth = -1);
    AZ_API void LogRenderedText(const AzVec2* ref_pos,
                                const char* text,
                                const char* text_end = NULL);
    AZ_API void LogSetNextTextDecoration(const char* prefix,
                                         const char* suffix);

    AZ_API bool BeginChildEx(const char* name,
                             AzID id,
                             const AzVec2& size_arg,
                             AzChildFlags child_flags,
                             AzWindowFlags window_flags);

    AZ_API bool BeginPopupEx(AzID id, AzWindowFlags extra_window_flags);
    AZ_API bool BeginPopupMenuEx(AzID id,
                                 const char* label,
                                 AzWindowFlags extra_window_flags);
    AZ_API void OpenPopupEx(AzID id,
                            AzPopupFlags popup_flags = AzPopupFlags_None);
    AZ_API void ClosePopupToLevel(int remaining,
                                  bool restore_focus_to_window_under_popup);
    AZ_API void ClosePopupsOverWindow(AzWindow* ref_window,
                                      bool restore_focus_to_window_under_popup);
    AZ_API void ClosePopupsExceptModals();
    AZ_API bool IsPopupOpen(AzID id, AzPopupFlags popup_flags);
    AZ_API AzRect GetPopupAllowedExtentRect(AzWindow* window);
    AZ_API AzWindow* GetTopMostPopupModal();
    AZ_API AzWindow* GetTopMostAndVisiblePopupModal();
    AZ_API AzWindow* FindBlockingModal(AzWindow* window);
    AZ_API AzVec2 FindBestWindowPosForPopup(AzWindow* window);
    AZ_API AzVec2 FindBestWindowPosForPopupEx(const AzVec2& ref_pos,
                                              const AzVec2& size,
                                              AzDir* last_dir,
                                              const AzRect& r_outer,
                                              const AzRect& r_avoid,
                                              AzPopupPositionPolicy policy);

    AZ_API bool BeginTooltipEx(AzTooltipFlags tooltip_flags,
                               AzWindowFlags extra_window_flags);
    AZ_API bool BeginTooltipHidden();

    AZ_API bool BeginViewportSideBar(const char* name,
                                     AzViewport* viewport,
                                     AzDir dir,
                                     float size,
                                     AzWindowFlags window_flags);
    AZ_API bool BeginMenuEx(const char* label,
                            const char* icon,
                            bool enabled = true);
    AZ_API bool MenuItemEx(const char* label,
                           const char* icon,
                           const char* shortcut = NULL,
                           bool selected = false,
                           bool enabled = true);

    AZ_API bool BeginComboPopup(AzID popup_id,
                                const AzRect& bb,
                                AzComboFlags flags);
    AZ_API bool BeginComboPreview();
    AZ_API void EndComboPreview();

    AZ_API void NavInitWindow(AzWindow* window, bool force_reinit);
    AZ_API void NavInitRequestApplyResult();
    AZ_API bool NavMoveRequestButNoResultYet();
    AZ_API void NavMoveRequestSubmit(AzDir move_dir,
                                     AzDir clip_dir,
                                     AzNavMoveFlags move_flags,
                                     AzScrollFlags scroll_flags);
    AZ_API void NavMoveRequestForward(AzDir move_dir,
                                      AzDir clip_dir,
                                      AzNavMoveFlags move_flags,
                                      AzScrollFlags scroll_flags);
    AZ_API void NavMoveRequestResolveWithLastItem(AzNavItemData* result);
    AZ_API void NavMoveRequestResolveWithPastTreeNode(
            AzNavItemData* result,
            const AzTreeNodeStackData* tree_node_data);
    AZ_API void NavMoveRequestCancel();
    AZ_API void NavMoveRequestApplyResult();
    AZ_API void NavMoveRequestTryWrapping(AzWindow* window,
                                          AzNavMoveFlags move_flags);
    AZ_API void NavHighlightActivated(AzID id);
    AZ_API void NavClearPreferredPosForAxis(AzAxis axis);
    AZ_API void SetNavCursorVisibleAfterMove();
    AZ_API void NavUpdateCurrentWindowIsScrollPushableX();
    AZ_API void SetNavWindow(AzWindow* window);
    AZ_API void SetNavID(AzID id,
                         AzNavLayer nav_layer,
                         AzID focus_scope_id,
                         const AzRect& rect_rel);
    AZ_API void SetNavFocusScope(AzID focus_scope_id);

    AZ_API void FocusItem();
    AZ_API void ActivateItemByID(AzID id);

    inline bool IsNamedKey(AzKey key)
    {
        return key >= AzKey_NamedKey_BEGIN && key < AzKey_NamedKey_END;
    }
    inline bool IsNamedKeyOrMod(AzKey key)
    {
        return (key >= AzKey_NamedKey_BEGIN && key < AzKey_NamedKey_END)
               || key == AzMod_Ctrl || key == AzMod_Shift || key == AzMod_Alt
               || key == AzMod_Super;
    }
    inline bool IsLegacyKey(AzKey key)
    {
        return key >= AzKey_LegacyNativeKey_BEGIN
               && key < AzKey_LegacyNativeKey_END;
    }
    inline bool IsKeyboardKey(AzKey key)
    {
        return key >= AzKey_Keyboard_BEGIN && key < AzKey_Keyboard_END;
    }
    inline bool IsGamepadKey(AzKey key)
    {
        return key >= AzKey_Gamepad_BEGIN && key < AzKey_Gamepad_END;
    }
    inline bool IsMouseKey(AzKey key)
    {
        return key >= AzKey_Mouse_BEGIN && key < AzKey_Mouse_END;
    }
    inline bool IsAliasKey(AzKey key)
    {
        return key >= AzKey_Aliases_BEGIN && key < AzKey_Aliases_END;
    }
    inline bool IsLRModKey(AzKey key)
    {
        return key >= AzKey_LeftCtrl && key <= AzKey_RightSuper;
    }
    AzKeyChord FixupKeyChord(AzKeyChord key_chord);
    inline AzKey ConvertSingleModFlagToKey(AzKey key)
    {
        if (key == AzMod_Ctrl)
            return AzKey_ReservedForModCtrl;
        if (key == AzMod_Shift)
            return AzKey_ReservedForModShift;
        if (key == AzMod_Alt)
            return AzKey_ReservedForModAlt;
        if (key == AzMod_Super)
            return AzKey_ReservedForModSuper;
        return key;
    }

    AZ_API AzKeyData* GetKeyData(AzContext* ctx, AzKey key);
    inline AzKeyData* GetKeyData(AzKey key)
    {
        AzContext& g = *GAz;
        return GetKeyData(&g, key);
    }
    AZ_API const char* GetKeyChordName(AzKeyChord key_chord);
    inline AzKey MouseButtonToKey(AzMouseButton button)
    {
        AZ_ASSERT(button >= 0 && button < AzMouseButton_COUNT);
        return (AzKey)(AzKey_MouseLeft + button);
    }
    AZ_API bool IsMouseDragPastThreshold(AzMouseButton button,
                                         float lock_threshold = -1.0f);
    AZ_API AzVec2 GetKeyMagnitude2d(AzKey key_left,
                                    AzKey key_right,
                                    AzKey key_up,
                                    AzKey key_down);
    AZ_API float GetNavTweakPressedAmount(AzAxis axis);
    AZ_API int CalcTypematicRepeatAmount(float t0,
                                         float t1,
                                         float repeat_delay,
                                         float repeat_rate);
    AZ_API void GetTypematicRepeatRate(AzInputFlags flags,
                                       float* repeat_delay,
                                       float* repeat_rate);
    AZ_API void TeleportMousePos(const AzVec2& pos);
    AZ_API void SetActiveIdUsingAllKeyboardKeys();
    inline bool IsActiveIdUsingNavDir(AzDir dir)
    {
        AzContext& g = *GAz;
        return (g.ActiveIdUsingNavDirMask & (1 << dir)) != 0;
    }

    AZ_API AzID GetKeyOwner(AzKey key);
    AZ_API void SetKeyOwner(AzKey key, AzID owner_id, AzInputFlags flags = 0);
    AZ_API void SetKeyOwnersForKeyChord(AzKeyChord key,
                                        AzID owner_id,
                                        AzInputFlags flags = 0);
    AZ_API void SetItemKeyOwner(AzKey key, AzInputFlags flags);
    AZ_API bool TestKeyOwner(AzKey key, AzID owner_id);
    inline AzKeyOwnerData* GetKeyOwnerData(AzContext* ctx, AzKey key)
    {
        if (key & AzMod_Mask_)
            key = ConvertSingleModFlagToKey(key);
        AZ_ASSERT(IsNamedKey(key));
        return &ctx->KeysOwnerData[key - AzKey_NamedKey_BEGIN];
    }

    AZ_API bool IsKeyDown(AzKey key, AzID owner_id);
    AZ_API bool IsKeyPressed(AzKey key, AzInputFlags flags, AzID owner_id = 0);
    AZ_API bool IsKeyReleased(AzKey key, AzID owner_id);
    AZ_API bool IsKeyChordPressed(AzKeyChord key_chord,
                                  AzInputFlags flags,
                                  AzID owner_id = 0);
    AZ_API bool IsMouseDown(AzMouseButton button, AzID owner_id);
    AZ_API bool IsMouseClicked(AzMouseButton button,
                               AzInputFlags flags,
                               AzID owner_id = 0);
    AZ_API bool IsMouseReleased(AzMouseButton button, AzID owner_id);
    AZ_API bool IsMouseDoubleClicked(AzMouseButton button, AzID owner_id);

    AZ_API bool Shortcut(AzKeyChord key_chord,
                         AzInputFlags flags,
                         AzID owner_id);
    AZ_API bool SetShortcutRouting(AzKeyChord key_chord,
                                   AzInputFlags flags,
                                   AzID owner_id);
    AZ_API bool TestShortcutRouting(AzKeyChord key_chord, AzID owner_id);
    AZ_API AzKeyRoutingData* GetShortcutRoutingData(AzKeyChord key_chord);

    AZ_API void DockContextInitialize(AzContext* ctx);
    AZ_API void DockContextShutdown(AzContext* ctx);
    AZ_API void DockContextClearNodes(AzContext* ctx,
                                      AzID root_id,
                                      bool clear_settings_refs);
    AZ_API void DockContextRebuildNodes(AzContext* ctx);
    AZ_API void DockContextNewFrameUpdateUndocking(AzContext* ctx);
    AZ_API void DockContextNewFrameUpdateDocking(AzContext* ctx);
    AZ_API void DockContextEndFrame(AzContext* ctx);
    AZ_API AzID DockContextGenNodeID(AzContext* ctx);
    AZ_API void DockContextQueueDock(AzContext* ctx,
                                     AzWindow* target,
                                     AzDockNode* target_node,
                                     AzWindow* payload,
                                     AzDir split_dir,
                                     float split_ratio,
                                     bool split_outer);
    AZ_API void DockContextQueueUndockWindow(AzContext* ctx, AzWindow* window);
    AZ_API void DockContextQueueUndockNode(AzContext* ctx, AzDockNode* node);
    AZ_API void DockContextProcessUndockWindow(
            AzContext* ctx,
            AzWindow* window,
            bool clear_persistent_docking_ref = true);
    AZ_API void DockContextProcessUndockNode(AzContext* ctx, AzDockNode* node);
    AZ_API bool DockContextCalcDropPosForDocking(AzWindow* target,
                                                 AzDockNode* target_node,
                                                 AzWindow* payload_window,
                                                 AzDockNode* payload_node,
                                                 AzDir split_dir,
                                                 bool split_outer,
                                                 AzVec2* out_pos);
    AZ_API AzDockNode* DockContextFindNodeByID(AzContext* ctx, AzID id);
    AZ_API void DockNodeWindowMenuHandler_Default(AzContext* ctx,
                                                  AzDockNode* node,
                                                  AzTabBar* tab_bar);
    AZ_API bool DockNodeBeginAmendTabBar(AzDockNode* node);
    AZ_API void DockNodeEndAmendTabBar();
    inline AzDockNode* DockNodeGetRootNode(AzDockNode* node)
    {
        while (node->ParentNode)
            node = node->ParentNode;
        return node;
    }
    inline bool DockNodeIsInHierarchyOf(AzDockNode* node, AzDockNode* parent)
    {
        while (node)
        {
            if (node == parent)
                return true;
            node = node->ParentNode;
        }
        return false;
    }
    inline int DockNodeGetDepth(const AzDockNode* node)
    {
        int depth = 0;
        while (node->ParentNode)
        {
            node = node->ParentNode;
            depth++;
        }
        return depth;
    }
    inline AzID DockNodeGetWindowMenuButtonId(const AzDockNode* node)
    {
        return AzHashStr("#COLLAPSE", 0, node->ID);
    }
    inline AzDockNode* GetWindowDockNode()
    {
        AzContext& g = *GAz;
        return g.CurrentWindow->DockNode;
    }
    AZ_API bool GetWindowAlwaysWantOwnTabBar(AzWindow* window);
    AZ_API void BeginDocked(AzWindow* window, bool* p_open);
    AZ_API void BeginDockableDragDropSource(AzWindow* window);
    AZ_API void BeginDockableDragDropTarget(AzWindow* window);
    AZ_API void SetWindowDock(AzWindow* window, AzID dock_id, AzCond cond);

    AZ_API void DockBuilderDockWindow(const char* window_name, AzID node_id);
    AZ_API AzDockNode* DockBuilderGetNode(AzID node_id);
    inline AzDockNode* DockBuilderGetCentralNode(AzID node_id)
    {
        AzDockNode* node = DockBuilderGetNode(node_id);
        if (!node)
            return NULL;
        return DockNodeGetRootNode(node)->CentralNode;
    }
    AZ_API AzID DockBuilderAddNode(AzID node_id = 0, AzDockNodeFlags flags = 0);
    AZ_API void DockBuilderRemoveNode(AzID node_id);
    AZ_API void DockBuilderRemoveNodeDockedWindows(
            AzID node_id,
            bool clear_settings_refs = true);
    AZ_API void DockBuilderRemoveNodeChildNodes(AzID node_id);
    AZ_API void DockBuilderSetNodePos(AzID node_id, AzVec2 pos);
    AZ_API void DockBuilderSetNodeSize(AzID node_id, AzVec2 size);
    AZ_API AzID DockBuilderSplitNode(AzID node_id,
                                     AzDir split_dir,
                                     float size_ratio_for_node_at_dir,
                                     AzID* out_id_at_dir,
                                     AzID* out_id_at_opposite_dir);
    AZ_API void DockBuilderCopyDockSpace(
            AzID src_dockspace_id,
            AzID dst_dockspace_id,
            AzVector<const char*>* in_window_remap_pairs);
    AZ_API void DockBuilderCopyNode(AzID src_node_id,
                                    AzID dst_node_id,
                                    AzVector<AzID>* out_node_remap_pairs);
    AZ_API void DockBuilderCopyWindowSettings(const char* src_name,
                                              const char* dst_name);
    AZ_API void DockBuilderFinish(AzID node_id);

    AZ_API void PushFocusScope(AzID id);
    AZ_API void PopFocusScope();
    inline AzID GetCurrentFocusScope()
    {
        AzContext& g = *GAz;
        return g.CurrentFocusScopeId;
    }

    AZ_API bool IsDragDropActive();
    AZ_API bool BeginDragDropTargetCustom(const AzRect& bb, AzID id);
    AZ_API void ClearDragDrop();
    AZ_API bool IsDragDropPayloadBeingAccepted();
    AZ_API void RenderDragDropTargetRect(const AzRect& bb,
                                         const AzRect& item_clip_rect);

    AZ_API AzTypingSelectRequest* GetTypingSelectRequest(
            AzTypingSelectFlags flags = AzTypingSelectFlags_None);
    AZ_API int TypingSelectFindMatch(AzTypingSelectRequest* req,
                                     int items_count,
                                     const char* (*get_item_name_func)(void*,
                                                                       int),
                                     void* user_data,
                                     int nav_item_idx);
    AZ_API int TypingSelectFindNextSingleCharMatch(
            AzTypingSelectRequest* req,
            int items_count,
            const char* (*get_item_name_func)(void*, int),
            void* user_data,
            int nav_item_idx);
    AZ_API int TypingSelectFindBestLeadingMatch(
            AzTypingSelectRequest* req,
            int items_count,
            const char* (*get_item_name_func)(void*, int),
            void* user_data);

    AZ_API bool BeginBoxSelect(const AzRect& scope_rect,
                               AzWindow* window,
                               AzID box_select_id,
                               AzMultiSelectFlags ms_flags);
    AZ_API void EndBoxSelect(const AzRect& scope_rect,
                             AzMultiSelectFlags ms_flags);

    AZ_API void MultiSelectItemHeader(AzID id,
                                      bool* p_selected,
                                      AzButtonFlags* p_button_flags);
    AZ_API void MultiSelectItemFooter(AzID id,
                                      bool* p_selected,
                                      bool* p_pressed);
    AZ_API void MultiSelectAddSetAll(AzMultiSelectTempData* ms, bool selected);
    AZ_API void MultiSelectAddSetRange(AzMultiSelectTempData* ms,
                                       bool selected,
                                       int range_dir,
                                       AzSelectionUserData first_item,
                                       AzSelectionUserData last_item);
    inline AzBoxSelectState* GetBoxSelectState(AzID id)
    {
        AzContext& g = *GAz;
        return (id != 0 && g.BoxSelectState.ID == id
                && g.BoxSelectState.IsActive)
                       ? &g.BoxSelectState
                       : NULL;
    }
    inline AzMultiSelectState* GetMultiSelectState(AzID id)
    {
        AzContext& g = *GAz;
        return g.MultiSelectStorage.GetByKey(id);
    }

    AZ_API void SetWindowClipRectBeforeSetChannel(AzWindow* window,
                                                  const AzRect& clip_rect);
    AZ_API void BeginColumns(const char* str_id,
                             int count,
                             AzOldColumnFlags flags = 0);
    AZ_API void EndColumns();
    AZ_API void PushColumnClipRect(int column_index);
    AZ_API void PushColumnsBackground();
    AZ_API void PopColumnsBackground();
    AZ_API AzID GetColumnsID(const char* str_id, int count);
    AZ_API AzOldColumns* FindOrCreateColumns(AzWindow* window, AzID id);
    AZ_API float GetColumnOffsetFromNorm(const AzOldColumns* columns,
                                         float offset_norm);
    AZ_API float GetColumnNormFromOffset(const AzOldColumns* columns,
                                         float offset);

    AZ_API void TableOpenContextMenu(int column_n = -1);
    AZ_API void TableSetColumnWidth(int column_n, float width);
    AZ_API void TableSetColumnSortDirection(int column_n,
                                            AzSortDirection sort_direction,
                                            bool append_to_sort_specs);
    AZ_API int TableGetHoveredRow();
    AZ_API float TableGetHeaderRowHeight();
    AZ_API float TableGetHeaderAngledMaxLabelWidth();
    AZ_API void TablePushBackgroundChannel();
    AZ_API void TablePopBackgroundChannel();
    AZ_API void TablePushColumnChannel(int column_n);
    AZ_API void TablePopColumnChannel();
    AZ_API void TableAngledHeadersRowEx(AzID row_id,
                                        float angle,
                                        float max_label_width,
                                        const AzTableHeaderData* data,
                                        int data_count);

    inline AzTable* GetCurrentTable()
    {
        AzContext& g = *GAz;
        return g.CurrentTable;
    }
    AZ_API AzTable* TableFindByID(AzID id);
    AZ_API bool BeginTableEx(const char* name,
                             AzID id,
                             int columns_count,
                             AzTableFlags flags = 0,
                             const AzVec2& outer_size = AzVec2(0, 0),
                             float inner_width = 0.0f);
    AZ_API void TableBeginInitMemory(AzTable* table, int columns_count);
    AZ_API void TableBeginApplyRequests(AzTable* table);
    AZ_API void TableSetupDrawChannels(AzTable* table);
    AZ_API void TableUpdateLayout(AzTable* table);
    AZ_API void TableUpdateBorders(AzTable* table);
    AZ_API void TableUpdateColumnsWeightFromWidth(AzTable* table);
    AZ_API void TableDrawBorders(AzTable* table);
    AZ_API void TableDrawDefaultContextMenu(
            AzTable* table,
            AzTableFlags flags_for_section_to_display);
    AZ_API bool TableBeginContextMenuPopup(AzTable* table);
    AZ_API void TableMergeDrawChannels(AzTable* table);
    inline AzTableInstanceData* TableGetInstanceData(AzTable* table,
                                                     int instance_no)
    {
        if (instance_no == 0)
            return &table->InstanceDataFirst;
        return &table->InstanceDataExtra[instance_no - 1];
    }
    inline AzID TableGetInstanceID(AzTable* table, int instance_no)
    {
        return TableGetInstanceData(table, instance_no)->TableInstanceID;
    }
    AZ_API void TableSortSpecsSanitize(AzTable* table);
    AZ_API void TableSortSpecsBuild(AzTable* table);
    AZ_API AzSortDirection
    TableGetColumnNextSortDirection(AzTableColumn* column);
    AZ_API void TableFixColumnSortDirection(AzTable* table,
                                            AzTableColumn* column);
    AZ_API float TableGetColumnWidthAuto(AzTable* table, AzTableColumn* column);
    AZ_API void TableBeginRow(AzTable* table);
    AZ_API void TableEndRow(AzTable* table);
    AZ_API void TableBeginCell(AzTable* table, int column_n);
    AZ_API void TableEndCell(AzTable* table);
    AZ_API AzRect TableGetCellBgRect(const AzTable* table, int column_n);
    AZ_API const char* TableGetColumnName(const AzTable* table, int column_n);
    AZ_API AzID TableGetColumnResizeID(AzTable* table,
                                       int column_n,
                                       int instance_no = 0);
    AZ_API float TableCalcMaxColumnWidth(const AzTable* table, int column_n);
    AZ_API void TableSetColumnWidthAutoSingle(AzTable* table, int column_n);
    AZ_API void TableSetColumnWidthAutoAll(AzTable* table);
    AZ_API void TableRemove(AzTable* table);
    AZ_API void TableGcCompactTransientBuffers(AzTable* table);
    AZ_API void TableGcCompactTransientBuffers(AzTableTempData* table);
    AZ_API void TableGcCompactSettings();

    AZ_API void TableLoadSettings(AzTable* table);
    AZ_API void TableSaveSettings(AzTable* table);
    AZ_API void TableResetSettings(AzTable* table);
    AZ_API AzTableSettings* TableGetBoundSettings(AzTable* table);
    AZ_API void TableSettingsAddSettingsHandler();
    AZ_API AzTableSettings* TableSettingsCreate(AzID id, int columns_count);
    AZ_API AzTableSettings* TableSettingsFindByID(AzID id);

    inline AzTabBar* GetCurrentTabBar()
    {
        AzContext& g = *GAz;
        return g.CurrentTabBar;
    }
    AZ_API bool BeginTabBarEx(AzTabBar* tab_bar,
                              const AzRect& bb,
                              AzTabBarFlags flags);
    AZ_API AzTabItem* TabBarFindTabByID(AzTabBar* tab_bar, AzID tab_id);
    AZ_API AzTabItem* TabBarFindTabByOrder(AzTabBar* tab_bar, int order);
    AZ_API AzTabItem* TabBarFindMostRecentlySelectedTabForActiveWindow(
            AzTabBar* tab_bar);
    AZ_API AzTabItem* TabBarGetCurrentTab(AzTabBar* tab_bar);
    inline int TabBarGetTabOrder(AzTabBar* tab_bar, AzTabItem* tab)
    {
        return tab_bar->Tabs.index_from_ptr(tab);
    }
    AZ_API const char* TabBarGetTabName(AzTabBar* tab_bar, AzTabItem* tab);
    AZ_API void TabBarAddTab(AzTabBar* tab_bar,
                             AzTabItemFlags tab_flags,
                             AzWindow* window);
    AZ_API void TabBarRemoveTab(AzTabBar* tab_bar, AzID tab_id);
    AZ_API void TabBarCloseTab(AzTabBar* tab_bar, AzTabItem* tab);
    AZ_API void TabBarQueueFocus(AzTabBar* tab_bar, AzTabItem* tab);
    AZ_API void TabBarQueueFocus(AzTabBar* tab_bar, const char* tab_name);
    AZ_API void TabBarQueueReorder(AzTabBar* tab_bar,
                                   AzTabItem* tab,
                                   int offset);
    AZ_API void TabBarQueueReorderFromMousePos(AzTabBar* tab_bar,
                                               AzTabItem* tab,
                                               AzVec2 mouse_pos);
    AZ_API bool TabBarProcessReorder(AzTabBar* tab_bar);
    AZ_API bool TabItemEx(AzTabBar* tab_bar,
                          const char* label,
                          bool* p_open,
                          AzTabItemFlags flags,
                          AzWindow* docked_window);
    AZ_API void TabItemSpacing(const char* str_id,
                               AzTabItemFlags flags,
                               float width);
    AZ_API AzVec2 TabItemCalcSize(const char* label,
                                  bool has_close_button_or_unsaved_marker);
    AZ_API AzVec2 TabItemCalcSize(AzWindow* window);
    AZ_API void TabItemBackground(AzDrawList* draw_list,
                                  const AzRect& bb,
                                  AzTabItemFlags flags,
                                  AzU32 col);
    AZ_API void TabItemLabelAndCloseButton(AzDrawList* draw_list,
                                           const AzRect& bb,
                                           AzTabItemFlags flags,
                                           AzVec2 frame_padding,
                                           const char* label,
                                           AzID tab_id,
                                           AzID close_button_id,
                                           bool is_contents_visible,
                                           bool* out_just_closed,
                                           bool* out_text_clipped);

    AZ_API void RenderText(AzVec2 pos,
                           const char* text,
                           const char* text_end = NULL,
                           bool hide_text_after_hash = true);
    AZ_API void RenderTextWrapped(AzVec2 pos,
                                  const char* text,
                                  const char* text_end,
                                  float wrap_width);
    AZ_API void RenderTextClipped(const AzVec2& pos_min,
                                  const AzVec2& pos_max,
                                  const char* text,
                                  const char* text_end,
                                  const AzVec2* text_size_if_known,
                                  const AzVec2& align = AzVec2(0, 0),
                                  const AzRect* clip_rect = NULL);
    AZ_API void RenderTextClippedEx(AzDrawList* draw_list,
                                    const AzVec2& pos_min,
                                    const AzVec2& pos_max,
                                    const char* text,
                                    const char* text_end,
                                    const AzVec2* text_size_if_known,
                                    const AzVec2& align = AzVec2(0, 0),
                                    const AzRect* clip_rect = NULL);
    AZ_API void RenderTextEllipsis(AzDrawList* draw_list,
                                   const AzVec2& pos_min,
                                   const AzVec2& pos_max,
                                   float ellipsis_max_x,
                                   const char* text,
                                   const char* text_end,
                                   const AzVec2* text_size_if_known);
    AZ_API void RenderFrame(AzVec2 p_min,
                            AzVec2 p_max,
                            AzU32 fill_col,
                            bool borders = true,
                            float rounding = 0.0f);
    AZ_API void RenderFrameBorder(AzVec2 p_min,
                                  AzVec2 p_max,
                                  float rounding = 0.0f);
    AZ_API void RenderColorRectWithAlphaCheckerboard(AzDrawList* draw_list,
                                                     AzVec2 p_min,
                                                     AzVec2 p_max,
                                                     AzU32 fill_col,
                                                     float grid_step,
                                                     AzVec2 grid_off,
                                                     float rounding = 0.0f,
                                                     AzDrawFlags flags = 0);
    AZ_API void RenderNavCursor(
            const AzRect& bb,
            AzID id,
            AzNavRenderCursorFlags flags = AzNavRenderCursorFlags_None);
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
    inline void RenderNavHighlight(
            const AzRect& bb,
            AzID id,
            AzNavRenderCursorFlags flags = AzNavRenderCursorFlags_None)
    {
        RenderNavCursor(bb, id, flags);
    }
#endif
    AZ_API const char* FindRenderedTextEnd(const char* text,
                                           const char* text_end = NULL);
    AZ_API void RenderMouseCursor(AzVec2 pos,
                                  float scale,
                                  AzMouseCursor mouse_cursor,
                                  AzU32 col_fill,
                                  AzU32 col_border,
                                  AzU32 col_shadow);

    AZ_API void RenderArrow(AzDrawList* draw_list,
                            AzVec2 pos,
                            AzU32 col,
                            AzDir dir,
                            float scale = 1.0f);
    AZ_API void RenderBullet(AzDrawList* draw_list, AzVec2 pos, AzU32 col);
    AZ_API void RenderCheckMark(AzDrawList* draw_list,
                                AzVec2 pos,
                                AzU32 col,
                                float sz);
    AZ_API void RenderArrowPointingAt(AzDrawList* draw_list,
                                      AzVec2 pos,
                                      AzVec2 half_sz,
                                      AzDir direction,
                                      AzU32 col);
    AZ_API void RenderArrowDockMenu(AzDrawList* draw_list,
                                    AzVec2 p_min,
                                    float sz,
                                    AzU32 col);
    AZ_API void RenderRectFilledRangeH(AzDrawList* draw_list,
                                       const AzRect& rect,
                                       AzU32 col,
                                       float x_start_norm,
                                       float x_end_norm,
                                       float rounding);
    AZ_API void RenderRectFilledWithHole(AzDrawList* draw_list,
                                         const AzRect& outer,
                                         const AzRect& inner,
                                         AzU32 col,
                                         float rounding);
    AZ_API AzDrawFlags CalcRoundingFlagsForRectInRect(const AzRect& r_in,
                                                      const AzRect& r_outer,
                                                      float threshold);

    AZ_API void TextEx(const char* text,
                       const char* text_end = NULL,
                       AzTextFlags flags = 0);
    AZ_API void TextAligned(float align_x, float size_x, const char* fmt, ...);
    AZ_API void TextAlignedV(float align_x,
                             float size_x,
                             const char* fmt,
                             va_list args);

    AZ_API bool ButtonEx(const char* label,
                         const AzVec2& size_arg = AzVec2(0, 0),
                         AzButtonFlags flags = 0);
    AZ_API bool ArrowButtonEx(const char* str_id,
                              AzDir dir,
                              AzVec2 size_arg,
                              AzButtonFlags flags = 0);
    AZ_API bool AzageButtonEx(AzID id,
                              AzTextureRef tex_ref,
                              const AzVec2& image_size,
                              const AzVec2& uv0,
                              const AzVec2& uv1,
                              const AzVec4& bg_col,
                              const AzVec4& tint_col,
                              AzButtonFlags flags = 0);
    AZ_API void SeparatorEx(AzSeparatorFlags flags, float thickness = 1.0f);
    AZ_API void SeparatorTextEx(AzID id,
                                const char* label,
                                const char* label_end,
                                float extra_width);
    AZ_API bool CheckboxFlags(const char* label,
                              AzS64* flags,
                              AzS64 flags_value);
    AZ_API bool CheckboxFlags(const char* label,
                              AzU64* flags,
                              AzU64 flags_value);

    AZ_API bool CloseButton(AzID id, const AzVec2& pos);
    AZ_API bool CollapseButton(AzID id,
                               const AzVec2& pos,
                               AzDockNode* dock_node);
    AZ_API void Scrollbar(AzAxis axis);
    AZ_API bool ScrollbarEx(const AzRect& bb,
                            AzID id,
                            AzAxis axis,
                            AzS64* p_scroll_v,
                            AzS64 avail_v,
                            AzS64 contents_v,
                            AzDrawFlags draw_rounding_flags = 0);
    AZ_API AzRect GetWindowScrollbarRect(AzWindow* window, AzAxis axis);
    AZ_API AzID GetWindowScrollbarID(AzWindow* window, AzAxis axis);
    AZ_API AzID GetWindowResizeCornerID(AzWindow* window, int n);
    AZ_API AzID GetWindowResizeBorderID(AzWindow* window, AzDir dir);

    AZ_API bool ButtonBehavior(const AzRect& bb,
                               AzID id,
                               bool* out_hovered,
                               bool* out_held,
                               AzButtonFlags flags = 0);
    AZ_API bool DragBehavior(AzID id,
                             AzDataType data_type,
                             void* p_v,
                             float v_speed,
                             const void* p_min,
                             const void* p_max,
                             const char* format,
                             AzSliderFlags flags);
    AZ_API bool SliderBehavior(const AzRect& bb,
                               AzID id,
                               AzDataType data_type,
                               void* p_v,
                               const void* p_min,
                               const void* p_max,
                               const char* format,
                               AzSliderFlags flags,
                               AzRect* out_grab_bb);
    AZ_API bool SplitterBehavior(const AzRect& bb,
                                 AzID id,
                                 AzAxis axis,
                                 float* size1,
                                 float* size2,
                                 float min_size1,
                                 float min_size2,
                                 float hover_extend = 0.0f,
                                 float hover_visibility_delay = 0.0f,
                                 AzU32 bg_col = 0);

    AZ_API bool TreeNodeBehavior(AzID id,
                                 AzTreeNodeFlags flags,
                                 const char* label,
                                 const char* label_end = NULL);
    AZ_API void TreeNodeDrawLineToChildNode(const AzVec2& target_pos);
    AZ_API void TreeNodeDrawLineToTreePop(const AzTreeNodeStackData* data);
    AZ_API void TreePushOverrideID(AzID id);
    AZ_API bool TreeNodeGetOpen(AzID storage_id);
    AZ_API void TreeNodeSetOpen(AzID storage_id, bool open);
    AZ_API bool TreeNodeUpdateNextOpen(AzID storage_id, AzTreeNodeFlags flags);

    template <typename T, typename SIGNED_T, typename FLOAT_T>
    AZ_API float ScaleRatioFromValueT(AzDataType data_type,
                                      T v,
                                      T v_min,
                                      T v_max,
                                      bool is_logarithmic,
                                      float logarithmic_zero_epsilon,
                                      float zero_deadzone_size);
    template <typename T, typename SIGNED_T, typename FLOAT_T>
    AZ_API T ScaleValueFromRatioT(AzDataType data_type,
                                  float t,
                                  T v_min,
                                  T v_max,
                                  bool is_logarithmic,
                                  float logarithmic_zero_epsilon,
                                  float zero_deadzone_size);
    template <typename T, typename SIGNED_T, typename FLOAT_T>
    AZ_API bool DragBehaviorT(AzDataType data_type,
                              T* v,
                              float v_speed,
                              T v_min,
                              T v_max,
                              const char* format,
                              AzSliderFlags flags);
    template <typename T, typename SIGNED_T, typename FLOAT_T>
    AZ_API bool SliderBehaviorT(const AzRect& bb,
                                AzID id,
                                AzDataType data_type,
                                T* v,
                                T v_min,
                                T v_max,
                                const char* format,
                                AzSliderFlags flags,
                                AzRect* out_grab_bb);
    template <typename T>
    AZ_API T RoundScalarWithFormatT(const char* format,
                                    AzDataType data_type,
                                    T v);
    template <typename T>
    AZ_API bool CheckboxFlagsT(const char* label, T* flags, T flags_value);

    AZ_API const AzDataTypeInfo* DataTypeGetInfo(AzDataType data_type);
    AZ_API int DataTypeFormatString(char* buf,
                                    int buf_size,
                                    AzDataType data_type,
                                    const void* p_data,
                                    const char* format);
    AZ_API void DataTypeApplyOp(AzDataType data_type,
                                int op,
                                void* output,
                                const void* arg_1,
                                const void* arg_2);
    AZ_API bool DataTypeApplyFromText(const char* buf,
                                      AzDataType data_type,
                                      void* p_data,
                                      const char* format,
                                      void* p_data_when_empty = NULL);
    AZ_API int DataTypeCompare(AzDataType data_type,
                               const void* arg_1,
                               const void* arg_2);
    AZ_API bool DataTypeClamp(AzDataType data_type,
                              void* p_data,
                              const void* p_min,
                              const void* p_max);
    AZ_API bool DataTypeIsZero(AzDataType data_type, const void* p_data);

    AZ_API bool InputTextEx(const char* label,
                            const char* hint,
                            char* buf,
                            int buf_size,
                            const AzVec2& size_arg,
                            AzInputTextFlags flags,
                            AzInputTextCallback callback = NULL,
                            void* user_data = NULL);
    AZ_API void InputTextDeactivateHook(AzID id);
    AZ_API bool TempInputText(const AzRect& bb,
                              AzID id,
                              const char* label,
                              char* buf,
                              int buf_size,
                              AzInputTextFlags flags);
    AZ_API bool TempInputScalar(const AzRect& bb,
                                AzID id,
                                const char* label,
                                AzDataType data_type,
                                void* p_data,
                                const char* format,
                                const void* p_clamp_min = NULL,
                                const void* p_clamp_max = NULL);
    inline bool TempInputIsActive(AzID id)
    {
        AzContext& g = *GAz;
        return (g.ActiveId == id && g.TempInputId == id);
    }
    inline AzInputTextState* GetInputTextState(AzID id)
    {
        AzContext& g = *GAz;
        return (id != 0 && g.InputTextState.ID == id) ? &g.InputTextState
                                                      : NULL;
    }
    AZ_API void SetNextItemRefVal(AzDataType data_type, void* p_data);
    inline bool IsItemActiveAsInputText()
    {
        AzContext& g = *GAz;
        return g.ActiveId != 0 && g.ActiveId == g.LastItemData.ID
               && g.InputTextState.ID == g.LastItemData.ID;
    }

    AZ_API void ColorTooltip(const char* text,
                             const float* col,
                             AzColorEditFlags flags);
    AZ_API void ColorEditOptionsPopup(const float* col, AzColorEditFlags flags);
    AZ_API void ColorPickerOptionsPopup(const float* ref_col,
                                        AzColorEditFlags flags);

    AZ_API int PlotEx(AzPlotType plot_type,
                      const char* label,
                      float (*values_getter)(void* data, int idx),
                      void* data,
                      int values_count,
                      int values_offset,
                      const char* overlay_text,
                      float scale_min,
                      float scale_max,
                      const AzVec2& size_arg);

    AZ_API void ShadeVertsLinearColorGradientKeepAlpha(AzDrawList* draw_list,
                                                       int vert_start_idx,
                                                       int vert_end_idx,
                                                       AzVec2 gradient_p0,
                                                       AzVec2 gradient_p1,
                                                       AzU32 col0,
                                                       AzU32 col1);
    AZ_API void ShadeVertsLinearUV(AzDrawList* draw_list,
                                   int vert_start_idx,
                                   int vert_end_idx,
                                   const AzVec2& a,
                                   const AzVec2& b,
                                   const AzVec2& uv_a,
                                   const AzVec2& uv_b,
                                   bool clamp);
    AZ_API void ShadeVertsTransformPos(AzDrawList* draw_list,
                                       int vert_start_idx,
                                       int vert_end_idx,
                                       const AzVec2& pivot_in,
                                       float cos_a,
                                       float sin_a,
                                       const AzVec2& pivot_out);

    AZ_API void GcCompactTransientMiscBuffers();
    AZ_API void GcCompactTransientWindowBuffers(AzWindow* window);
    AZ_API void GcAwakeTransientWindowBuffers(AzWindow* window);

    AZ_API bool ErrorLog(const char* msg);
    AZ_API void ErrorRecoveryStoreState(AzErrorRecoveryState* state_out);
    AZ_API void ErrorRecoveryTryToRecoverState(
            const AzErrorRecoveryState* state_in);
    AZ_API void ErrorRecoveryTryToRecoverWindowState(
            const AzErrorRecoveryState* state_in);
    AZ_API void ErrorCheckUsingSetCursorPosToExtendParentBoundaries();
    AZ_API void ErrorCheckEndFrameFinalizeErrorTooltip();
    AZ_API bool BeginErrorTooltip();
    AZ_API void EndErrorTooltip();

    AZ_API void DebugAllocHook(AzDebugAllocInfo* info,
                               int frame_count,
                               void* ptr,
                               size_t size);
    AZ_API void DebugDrawCursorPos(AzU32 col = AZ_COL32(255, 0, 0, 255));
    AZ_API void DebugDrawLineExtents(AzU32 col = AZ_COL32(255, 0, 0, 255));
    AZ_API void DebugDrawItemRect(AzU32 col = AZ_COL32(255, 0, 0, 255));
    AZ_API void DebugTextUnformattedWithLocateItem(const char* line_begin,
                                                   const char* line_end);
    AZ_API void DebugLocateItem(AzID target_id);
    AZ_API void DebugLocateItemOnHover(AzID target_id);
    AZ_API void DebugLocateItemResolveWithLastItem();
    AZ_API void DebugBreakClearData();
    AZ_API bool DebugBreakButton(const char* label,
                                 const char* description_of_location);
    AZ_API void DebugBreakButtonTooltip(bool keyboard_only,
                                        const char* description_of_location);
    AZ_API void ShowFontAtlas(AzFontAtlas* atlas);
    AZ_API void DebugHookIdInfo(AzID id,
                                AzDataType data_type,
                                const void* data_id,
                                const void* data_id_end);
    AZ_API void DebugNodeColumns(AzOldColumns* columns);
    AZ_API void DebugNodeDockNode(AzDockNode* node, const char* label);
    AZ_API void DebugNodeDrawList(AzWindow* window,
                                  AzViewportP* viewport,
                                  const AzDrawList* draw_list,
                                  const char* label);
    AZ_API void DebugNodeDrawCmdShowMeshAndBoundingBox(
            AzDrawList* out_draw_list,
            const AzDrawList* draw_list,
            const AzDrawCmd* draw_cmd,
            bool show_mesh,
            bool show_aabb);
    AZ_API void DebugNodeFont(AzFont* font);
    AZ_API void DebugNodeFontGlyphesForSrcMask(AzFont* font,
                                               AzFontBaked* baked,
                                               int src_mask);
    AZ_API void DebugNodeFontGlyph(AzFont* font, const AzFontGlyph* glyph);
    AZ_API void DebugNodeTexture(AzTextureData* tex,
                                 int int_id,
                                 const AzFontAtlasRect* highlight_rect = NULL);
    AZ_API void DebugNodeStorage(AzStorage* storage, const char* label);
    AZ_API void DebugNodeTabBar(AzTabBar* tab_bar, const char* label);
    AZ_API void DebugNodeTable(AzTable* table);
    AZ_API void DebugNodeTableSettings(AzTableSettings* settings);
    AZ_API void DebugNodeInputTextState(AzInputTextState* state);
    AZ_API void DebugNodeTypingSelectState(AzTypingSelectState* state);
    AZ_API void DebugNodeMultiSelectState(AzMultiSelectState* state);
    AZ_API void DebugNodeWindow(AzWindow* window, const char* label);
    AZ_API void DebugNodeWindowSettings(AzWindowSettings* settings);
    AZ_API void DebugNodeWindowsList(AzVector<AzWindow*>* windows,
                                     const char* label);
    AZ_API void DebugNodeWindowsListByBeginStackParent(
            AzWindow** windows,
            int windows_size,
            AzWindow* parent_in_begin_stack);
    AZ_API void DebugNodeViewport(AzViewportP* viewport);
    AZ_API void DebugNodePlatformMonitor(AzPlatformMonitor* monitor,
                                         const char* label,
                                         int idx);
    AZ_API void DebugRenderKeyboardPreview(AzDrawList* draw_list);
    AZ_API void DebugRenderViewportThumbnail(AzDrawList* draw_list,
                                             AzViewportP* viewport,
                                             const AzRect& bb);

#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS

#endif

} // namespace Az

struct AzFontLoader
{
    const char* Name;
    bool (*LoaderInit)(AzFontAtlas* atlas);
    void (*LoaderShutdown)(AzFontAtlas* atlas);
    bool (*FontSrcInit)(AzFontAtlas* atlas, AzFontConfig* src);
    void (*FontSrcDestroy)(AzFontAtlas* atlas, AzFontConfig* src);
    bool (*FontSrcContainsGlyph)(AzFontAtlas* atlas,
                                 AzFontConfig* src,
                                 AzWchar codepoint);
    bool (*FontBakedInit)(AzFontAtlas* atlas,
                          AzFontConfig* src,
                          AzFontBaked* baked,
                          void* loader_data_for_baked_src);
    void (*FontBakedDestroy)(AzFontAtlas* atlas,
                             AzFontConfig* src,
                             AzFontBaked* baked,
                             void* loader_data_for_baked_src);
    bool (*FontBakedLoadGlyph)(AzFontAtlas* atlas,
                               AzFontConfig* src,
                               AzFontBaked* baked,
                               void* loader_data_for_baked_src,
                               AzWchar codepoint,
                               AzFontGlyph* out_glyph);

    size_t FontBakedSrcLoaderDataSize;

    AzFontLoader()
    {
        memset(this, 0, sizeof(*this));
    }
};

#ifdef AZ_ENABLE_STB_TRUETYPE
AZ_API const AzFontLoader* AzFontAtlasGetFontLoaderForStbTruetype();
#endif
#ifndef AZ_DISABLE_OBSOLETE_FUNCTIONS
typedef AzFontLoader AzFontBuilderIO;
#endif

static inline bool operator==(const AzTextureRef& lhs, const AzTextureRef& rhs)
{
    return lhs._TexID == rhs._TexID && lhs._TexData == rhs._TexData;
}
static inline bool operator!=(const AzTextureRef& lhs, const AzTextureRef& rhs)
{
    return lhs._TexID != rhs._TexID || lhs._TexData != rhs._TexData;
}

#define AzFontAtlasRectId_IndexMask_ (0x000FFFFF)
#define AzFontAtlasRectId_GenerationMask_ (0x3FF00000)
#define AzFontAtlasRectId_GenerationShift_ (20)
inline int AzFontAtlasRectId_GetIndex(AzFontAtlasRectId id)
{
    return id & AzFontAtlasRectId_IndexMask_;
}
inline int AzFontAtlasRectId_GetGeneration(AzFontAtlasRectId id)
{
    return (id & AzFontAtlasRectId_GenerationMask_)
           >> AzFontAtlasRectId_GenerationShift_;
}
inline AzFontAtlasRectId AzFontAtlasRectId_Make(int index_idx, int gen_idx)
{
    AZ_ASSERT(index_idx < AzFontAtlasRectId_IndexMask_
              && gen_idx < (AzFontAtlasRectId_GenerationMask_
                            >> AzFontAtlasRectId_GenerationShift_));
    return (AzFontAtlasRectId)(index_idx
                               | (gen_idx
                                  << AzFontAtlasRectId_GenerationShift_));
}

struct AzFontAtlasRectEntry
{
    int TargetIndex : 20;
    int Generation : 10;
    unsigned int IsUsed : 1;
};

struct AzFontAtlasPostProcessData
{
    AzFontAtlas* FontAtlas;
    AzFont* Font;
    AzFontConfig* FontSrc;
    AzFontBaked* FontBaked;
    AzFontGlyph* Glyph;

    unsigned char* Pixels;
    AzTextureFormat Format;
    int Pitch;
    int Width;
    int Height;
};

#ifdef AZ_STB_NAMESPACE
namespace AZ_STB_NAMESPACE
{
    struct stbrp_node;
}
typedef AZ_STB_NAMESPACE::stbrp_node stbrp_node_im;
#else
struct stbrp_node;
typedef stbrp_node stbrp_node_im;
#endif
struct stbrp_context_opaque
{
    char data[80];
};

struct AzFontAtlasBuilder
{
    stbrp_context_opaque PackContext;
    AzVector<stbrp_node_im> PackNodes;
    AzVector<AzTextureRect> Rects;
    AzVector<AzFontAtlasRectEntry> RectsIndex;
    AzVector<unsigned char> TempBuffer;
    int RectsIndexFreeListStart;
    int RectsPackedCount;
    int RectsPackedSurface;
    int RectsDiscardedCount;
    int RectsDiscardedSurface;
    int FrameCount;
    AzVec2i MaxRectSize;
    AzVec2i MaxRectBounds;
    bool LockDisableResize;
    bool PreloadedAllGlyphsRanges;

    AzStableVector<AzFontBaked, 32> BakedPool;
    AzStorage BakedMap;
    int BakedDiscardedCount;

    AzFontAtlasRectId PackIdMouseCursors;
    AzFontAtlasRectId PackIdLinesTexData;

    AzFontAtlasBuilder()
    {
        memset(this, 0, sizeof(*this));
        FrameCount = -1;
        RectsIndexFreeListStart = -1;
        PackIdMouseCursors = PackIdLinesTexData = -1;
    }
};

AZ_API void AzFontAtlasBuildInit(AzFontAtlas* atlas);
AZ_API void AzFontAtlasBuildDestroy(AzFontAtlas* atlas);
AZ_API void AzFontAtlasBuildMain(AzFontAtlas* atlas);
AZ_API void AzFontAtlasBuildSetupFontLoader(AzFontAtlas* atlas,
                                            const AzFontLoader* font_loader);
AZ_API void AzFontAtlasBuildUpdatePointers(AzFontAtlas* atlas);
AZ_API void AzFontAtlasBuildRenderBitmapFromString(AzFontAtlas* atlas,
                                                   int x,
                                                   int y,
                                                   int w,
                                                   int h,
                                                   const char* in_str,
                                                   char in_marker_char);
AZ_API void AzFontAtlasBuildClear(AzFontAtlas* atlas);

AZ_API AzTextureData* AzFontAtlasTextureAdd(AzFontAtlas* atlas, int w, int h);
AZ_API void AzFontAtlasTextureMakeSpace(AzFontAtlas* atlas);
AZ_API void AzFontAtlasTextureRepack(AzFontAtlas* atlas, int w, int h);
AZ_API void AzFontAtlasTextureGrow(AzFontAtlas* atlas,
                                   int old_w = -1,
                                   int old_h = -1);
AZ_API void AzFontAtlasTextureCompact(AzFontAtlas* atlas);
AZ_API AzVec2i AzFontAtlasTextureGetSizeEstimate(AzFontAtlas* atlas);

AZ_API void AzFontAtlasBuildSetupFontSpecialGlyphs(AzFontAtlas* atlas,
                                                   AzFont* font,
                                                   AzFontConfig* src);
AZ_API void AzFontAtlasBuildLegacyPreloadAllGlyphRanges(AzFontAtlas* atlas);
AZ_API void AzFontAtlasBuildGetOversampleFactors(AzFontConfig* src,
                                                 AzFontBaked* baked,
                                                 int* out_oversample_h,
                                                 int* out_oversample_v);
AZ_API void AzFontAtlasBuildDiscardBakes(AzFontAtlas* atlas, int unused_frames);

AZ_API bool AzFontAtlasFontSourceInit(AzFontAtlas* atlas, AzFontConfig* src);
AZ_API void AzFontAtlasFontSourceAddToFont(AzFontAtlas* atlas,
                                           AzFont* font,
                                           AzFontConfig* src);
AZ_API void AzFontAtlasFontDestroySourceData(AzFontAtlas* atlas,
                                             AzFontConfig* src);
AZ_API bool AzFontAtlasFontInitOutput(AzFontAtlas* atlas, AzFont* font);
AZ_API void AzFontAtlasFontDestroyOutput(AzFontAtlas* atlas, AzFont* font);
AZ_API void AzFontAtlasFontDiscardBakes(AzFontAtlas* atlas,
                                        AzFont* font,
                                        int unused_frames);

AZ_API AzID AzFontAtlasBakedGetId(AzID font_id,
                                  float baked_size,
                                  float rasterizer_density);
AZ_API AzFontBaked* AzFontAtlasBakedGetOrAdd(AzFontAtlas* atlas,
                                             AzFont* font,
                                             float font_size,
                                             float font_rasterizer_density);
AZ_API AzFontBaked* AzFontAtlasBakedGetClosestMatch(
        AzFontAtlas* atlas,
        AzFont* font,
        float font_size,
        float font_rasterizer_density);
AZ_API AzFontBaked* AzFontAtlasBakedAdd(AzFontAtlas* atlas,
                                        AzFont* font,
                                        float font_size,
                                        float font_rasterizer_density,
                                        AzID baked_id);
AZ_API void AzFontAtlasBakedDiscard(AzFontAtlas* atlas,
                                    AzFont* font,
                                    AzFontBaked* baked);
AZ_API AzFontGlyph* AzFontAtlasBakedAddFontGlyph(AzFontAtlas* atlas,
                                                 AzFontBaked* baked,
                                                 AzFontConfig* src,
                                                 const AzFontGlyph* in_glyph);
AZ_API void AzFontAtlasBakedDiscardFontGlyph(AzFontAtlas* atlas,
                                             AzFont* font,
                                             AzFontBaked* baked,
                                             AzFontGlyph* glyph);
AZ_API void AzFontAtlasBakedSetFontGlyphBitmap(AzFontAtlas* atlas,
                                               AzFontBaked* baked,
                                               AzFontConfig* src,
                                               AzFontGlyph* glyph,
                                               AzTextureRect* r,
                                               const unsigned char* src_pixels,
                                               AzTextureFormat src_fmt,
                                               int src_pitch);

AZ_API void AzFontAtlasPackInit(AzFontAtlas* atlas);
AZ_API AzFontAtlasRectId
AzFontAtlasPackAddRect(AzFontAtlas* atlas,
                       int w,
                       int h,
                       AzFontAtlasRectEntry* overwrite_entry = NULL);
AZ_API AzTextureRect* AzFontAtlasPackGetRect(AzFontAtlas* atlas,
                                             AzFontAtlasRectId id);
AZ_API AzTextureRect* AzFontAtlasPackGetRectSafe(AzFontAtlas* atlas,
                                                 AzFontAtlasRectId id);
AZ_API void AzFontAtlasPackDiscardRect(AzFontAtlas* atlas,
                                       AzFontAtlasRectId id);

AZ_API void AzFontAtlasUpdateNewFrame(AzFontAtlas* atlas, int frame_count);
AZ_API void AzFontAtlasAddDrawListSharedData(AzFontAtlas* atlas,
                                             AzDrawListSharedData* data);
AZ_API void AzFontAtlasRemoveDrawListSharedData(AzFontAtlas* atlas,
                                                AzDrawListSharedData* data);
AZ_API void AzFontAtlasUpdateDrawListsTextures(AzFontAtlas* atlas,
                                               AzTextureRef old_tex,
                                               AzTextureRef new_tex);
AZ_API void AzFontAtlasUpdateDrawListsSharedData(AzFontAtlas* atlas);

AZ_API void AzFontAtlasTextureBlockConvert(const unsigned char* src_pixels,
                                           AzTextureFormat src_fmt,
                                           int src_pitch,
                                           unsigned char* dst_pixels,
                                           AzTextureFormat dst_fmt,
                                           int dst_pitch,
                                           int w,
                                           int h);
AZ_API void AzFontAtlasTextureBlockPostProcess(
        AzFontAtlasPostProcessData* data);
AZ_API void AzFontAtlasTextureBlockPostProcessMultiply(
        AzFontAtlasPostProcessData* data,
        float multiply_factor);
AZ_API void AzFontAtlasTextureBlockFill(AzTextureData* dst_tex,
                                        int dst_x,
                                        int dst_y,
                                        int w,
                                        int h,
                                        AzU32 col);
AZ_API void AzFontAtlasTextureBlockCopy(AzTextureData* src_tex,
                                        int src_x,
                                        int src_y,
                                        AzTextureData* dst_tex,
                                        int dst_x,
                                        int dst_y,
                                        int w,
                                        int h);
AZ_API void AzFontAtlasTextureBlockQueueUpload(AzFontAtlas* atlas,
                                               AzTextureData* tex,
                                               int x,
                                               int y,
                                               int w,
                                               int h);

AZ_API int AzTextureDataGetFormatBytesPerPixel(AzTextureFormat format);
AZ_API const char* AzTextureDataGetStatusName(AzTextureStatus status);
AZ_API const char* AzTextureDataGetFormatName(AzTextureFormat format);

#ifndef AZ_DISABLE_DEBUG_TOOLS
AZ_API void AzFontAtlasDebugLogTextureRequests(AzFontAtlas* atlas);
#endif

AZ_API bool AzFontAtlasGetMouseCursorTexData(AzFontAtlas* atlas,
                                             AzMouseCursor cursor_type,
                                             AzVec2* out_offset,
                                             AzVec2* out_size,
                                             AzVec2 out_uv_border[2],
                                             AzVec2 out_uv_fill[2]);

#ifdef AZ_ENABLE_TEST_ENGINE
extern void AzTestEngineHook_ItemAdd(AzContext* ctx,
                                     AzID id,
                                     const AzRect& bb,
                                     const AzLastItemData* item_data);
extern void AzTestEngineHook_ItemInfo(AzContext* ctx,
                                      AzID id,
                                      const char* label,
                                      AzItemStatusFlags flags);
extern void AzTestEngineHook_Log(AzContext* ctx, const char* fmt, ...);
extern const char* AzTestEngine_FindItemDebugLabel(AzContext* ctx, AzID id);

#define AZ_TEST_ENGINE_ITEM_ADD(_ID, _BB, _ITEM_DATA)                          \
    if (g.TestEngineHookItems)                                                 \
    AzTestEngineHook_ItemAdd(&g, _ID, _BB, _ITEM_DATA)
#define AZ_TEST_ENGINE_ITEM_INFO(_ID, _LABEL, _FLAGS)                          \
    if (g.TestEngineHookItems)                                                 \
    AzTestEngineHook_ItemInfo(&g, _ID, _LABEL, _FLAGS)
#define AZ_TEST_ENGINE_LOG(_FMT, ...)                                          \
    AzTestEngineHook_Log(&g, _FMT, __VA_ARGS__)
#else
#define AZ_TEST_ENGINE_ITEM_ADD(_BB, _ID) ((void)0)
#define AZ_TEST_ENGINE_ITEM_INFO(_ID, _LABEL, _FLAGS) ((void)g)
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
