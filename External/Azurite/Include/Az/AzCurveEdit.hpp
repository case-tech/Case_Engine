// Copyright (c) 2025 Case Technologies

#pragma once
#include <Az/Az.hpp>
#include <stdint.h>

struct AzRect;

namespace AzGuizmo
{
    enum CurveType
    {
        CurveNone,
        CurveDiscrete,
        CurveLinear,
        CurveSmooth,
        CurveBezier,
    };

    struct EditPoint
    {
        int curveIndex;
        int pointIndex;
        bool operator<(const EditPoint& other) const
        {
            if (curveIndex < other.curveIndex)
                return true;
            if (curveIndex > other.curveIndex)
                return false;

            if (pointIndex < other.pointIndex)
                return true;
            return false;
        }
    };

    struct Delegate
    {
        bool focused = false;
        virtual size_t GetCurveCount() = 0;
        virtual bool IsVisible(size_t)
        {
            return true;
        }
        virtual CurveType GetCurveType(size_t) const
        {
            return CurveLinear;
        }
        virtual AzVec2& GetMin() = 0;
        virtual AzVec2& GetMax() = 0;
        virtual size_t GetPointCount(size_t curveIndex) = 0;
        virtual uint32_t GetCurveColor(size_t curveIndex) = 0;
        virtual AzVec2* GetPoints(size_t curveIndex) = 0;
        virtual int EditPoint(size_t curveIndex,
                              int pointIndex,
                              AzVec2 value) = 0;
        virtual void AddPoint(size_t curveIndex, AzVec2 value) = 0;
        virtual unsigned int GetBackgroundColor()
        {
            return 0xFF202020;
        }

        virtual void BeginEdit(int)
        {}
        virtual void EndEdit()
        {}

        virtual ~Delegate() = default;
    };

    int Edit(Delegate& delegate,
             const AzVec2& size,
             unsigned int id,
             const AzRect* clippingRect = NULL,
             AzVector<EditPoint>* selectedPoints = NULL);
} // namespace AzGuizmo
