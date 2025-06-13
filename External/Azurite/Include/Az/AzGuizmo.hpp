// Copyright (c) 2025 Case Technologies

#pragma once

#ifndef AZ_API
#define AZ_API
#endif

#ifndef AZGUIZMO_NAMESPACE
#define AZGUIZMO_NAMESPACE AzGuizmo
#endif

struct AzWindow;

namespace AZGUIZMO_NAMESPACE
{

    AZ_API void SetDrawlist(AzDrawList* drawlist = nullptr);

    AZ_API void BeginFrame();

    AZ_API void SetAzContext(AzContext* ctx);

    AZ_API bool IsOver();

    AZ_API bool IsUsing();

    AZ_API bool IsUsingViewManipulate();

    AZ_API bool IsViewManipulateHovered();

    AZ_API bool IsUsingAny();

    AZ_API void Enable(bool enable);

    AZ_API void DecomposeMatrixToComponents(const float* matrix,
                                            float* translation,
                                            float* rotation,
                                            float* scale);
    AZ_API void RecomposeMatrixFromComponents(const float* translation,
                                              const float* rotation,
                                              const float* scale,
                                              float* matrix);

    AZ_API void SetRect(float x, float y, float width, float height);

    AZ_API void SetOrthographic(bool isOrthographic);

    AZ_API void DrawCubes(const float* view,
                          const float* projection,
                          const float* matrices,
                          int matrixCount);
    AZ_API void DrawGrid(const float* view,
                         const float* projection,
                         const float* matrix,
                         const float gridSize);

    enum OPERATION
    {
        TRANSLATE_X = (1u << 0),
        TRANSLATE_Y = (1u << 1),
        TRANSLATE_Z = (1u << 2),
        ROTATE_X = (1u << 3),
        ROTATE_Y = (1u << 4),
        ROTATE_Z = (1u << 5),
        ROTATE_SCREEN = (1u << 6),
        SCALE_X = (1u << 7),
        SCALE_Y = (1u << 8),
        SCALE_Z = (1u << 9),
        BOUNDS = (1u << 10),
        SCALE_XU = (1u << 11),
        SCALE_YU = (1u << 12),
        SCALE_ZU = (1u << 13),

        TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
        ROTATE = ROTATE_X | ROTATE_Y | ROTATE_Z | ROTATE_SCREEN,
        SCALE = SCALE_X | SCALE_Y | SCALE_Z,
        SCALEU = SCALE_XU | SCALE_YU | SCALE_ZU,
        UNIVERSAL = TRANSLATE | ROTATE | SCALEU
    };

    inline OPERATION operator|(OPERATION lhs, OPERATION rhs)
    {
        return static_cast<OPERATION>(static_cast<int>(lhs)
                                      | static_cast<int>(rhs));
    }

    enum MODE
    {
        LOCAL,
        WORLD
    };

    AZ_API bool Manipulate(const float* view,
                           const float* projection,
                           OPERATION operation,
                           MODE mode,
                           float* matrix,
                           float* deltaMatrix = NULL,
                           const float* snap = NULL,
                           const float* localBounds = NULL,
                           const float* boundsSnap = NULL);

    AZ_API void ViewManipulate(float* view,
                               float length,
                               AzVec2 position,
                               AzVec2 size,
                               AzU32 backgroundColor);

    AZ_API void ViewManipulate(float* view,
                               const float* projection,
                               OPERATION operation,
                               MODE mode,
                               float* matrix,
                               float length,
                               AzVec2 position,
                               AzVec2 size,
                               AzU32 backgroundColor);

    AZ_API void SetAlternativeWindow(AzWindow* window);

    [[deprecated("Use PushID/PopID instead.")]]
    AZ_API void SetID(int id);

    AZ_API void PushID(const char* str_id);
    AZ_API void PushID(const char* str_id_begin, const char* str_id_end);
    AZ_API void PushID(const void* ptr_id);
    AZ_API void PushID(int int_id);
    AZ_API void PopID();
    AZ_API AzID GetID(const char* str_id);
    AZ_API AzID GetID(const char* str_id_begin, const char* str_id_end);
    AZ_API AzID GetID(const void* ptr_id);

    AZ_API bool IsOver(OPERATION op);
    AZ_API void SetGizmoSizeClipSpace(float value);

    AZ_API void AllowAxisFlip(bool value);

    AZ_API void SetAxisLimit(float value);

    AZ_API void SetAxisMask(bool x, bool y, bool z);

    AZ_API void SetPlaneLimit(float value);

    AZ_API bool IsOver(float* position, float pixelRadius);

    enum COLOR
    {
        DIRECTION_X,
        DIRECTION_Y,
        DIRECTION_Z,
        PLANE_X,
        PLANE_Y,
        PLANE_Z,
        SELECTION,
        INACTIVE,
        TRANSLATION_LINE,
        SCALE_LINE,
        ROTATION_USING_BORDER,
        ROTATION_USING_FILL,
        HATCHED_AXIS_LINES,
        TEXT,
        TEXT_SHADOW,
        COUNT
    };

    struct Style
    {
        AZ_API Style();

        float TranslationLineThickness;
        float TranslationLineArrowSize;
        float RotationLineThickness;
        float RotationOuterLineThickness;
        float ScaleLineThickness;
        float ScaleLineCircleSize;
        float HatchedAxisLineThickness;
        float CenterCircleSize;

        AzVec4 Colors[COLOR::COUNT];
    };

    AZ_API Style& GetStyle();
} // namespace AZGUIZMO_NAMESPACE
