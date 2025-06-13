#include <Az/Az.hpp>
#include <Az/AzGradient.hpp>
#include <Az/AzInternal.hpp>

namespace AzGuizmo
{
#ifndef AZ_DEFINE_MATH_OPERATORS
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
#endif

    static int DrawPoint(AzDrawList* draw_list,
                         AzVec4 color,
                         const AzVec2 size,
                         bool editing,
                         AzVec2 pos)
    {
        AzIO& io = Az::GetIO();

        AzVec2 p1 = AzLerp(pos,
                           AzVec2(pos + AzVec2(size.x - size.y, 0.f)),
                           color.w)
                    + AzVec2(3, 3);
        AzVec2 p2 = AzLerp(pos + AzVec2(size.y, size.y),
                           AzVec2(pos + size),
                           color.w)
                    - AzVec2(3, 3);
        AzRect rc(p1, p2);

        color.w = 1.f;
        draw_list->AddRectFilled(p1, p2, AzColor(color));
        if (editing)
            draw_list->AddRect(p1, p2, 0xFFFFFFFF, 2.f, 15, 2.5f);
        else
            draw_list->AddRect(p1, p2, 0x80FFFFFF, 2.f, 15, 1.25f);

        if (rc.Contains(io.MousePos))
        {
            if (io.MouseClicked[0])
                return 2;
            return 1;
        }
        return 0;
    }

    bool Edit(Delegate& delegate, const AzVec2& size, int& selection)
    {
        bool ret = false;
        AzIO& io = Az::GetIO();
        Az::PushStyleVar(AzStyleVar_FramePadding, AzVec2(0, 0));
        Az::BeginChild(137, size, AzChildFlags_FrameStyle);

        AzDrawList* draw_list = Az::GetWindowDrawList();
        const AzVec2 offset = Az::GetCursorScreenPos();

        const AzVec4* pts = delegate.GetPoints();
        static int currentSelection = -1;
        static int movingPt = -1;
        if (currentSelection >= int(delegate.GetPointCount()))
            currentSelection = -1;
        if (movingPt != -1)
        {
            AzVec4 current = pts[movingPt];
            current.w += io.MouseDelta.x / size.x;
            current.w = AzClamp(current.w, 0.f, 1.f);
            delegate.EditPoint(movingPt, current);
            ret = true;
            if (!io.MouseDown[0])
                movingPt = -1;
        }
        for (size_t i = 0; i < delegate.GetPointCount(); i++)
        {
            int ptSel = DrawPoint(draw_list,
                                  pts[i],
                                  size,
                                  i == currentSelection,
                                  offset);
            if (ptSel == 2)
            {
                currentSelection = int(i);
                ret = true;
            }
            if (ptSel == 1 && io.MouseDown[0] && movingPt == -1)
            {
                movingPt = int(i);
            }
        }
        AzRect rc(offset, offset + size);
        if (rc.Contains(io.MousePos) && io.MouseDoubleClicked[0])
        {
            float t = (io.MousePos.x - offset.x) / size.x;
            delegate.AddPoint(delegate.GetPoint(t));
            ret = true;
        }
        Az::EndChild();
        Az::PopStyleVar();

        selection = currentSelection;
        return ret;
    }
} // namespace AzGuizmo
