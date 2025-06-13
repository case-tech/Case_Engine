// Copyright (c) 2025 Case Technologies

#pragma once
#include <cstddef>

struct AzVec4;
struct AzVec2;

namespace AzGuizmo
{
    struct Delegate
    {
        virtual size_t GetPointCount() = 0;
        virtual AzVec4* GetPoints() = 0;
        virtual int EditPoint(int pointIndex, AzVec4 value) = 0;
        virtual AzVec4 GetPoint(float t) = 0;
        virtual void AddPoint(AzVec4 value) = 0;
        virtual ~Delegate() = default;
    };

    bool Edit(Delegate& delegate, const AzVec2& size, int& selection);
} // namespace AzGuizmo
