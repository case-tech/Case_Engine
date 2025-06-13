// Copyright (c) 2025 Case Technologies

#pragma once

namespace AzGuizmo
{
    typedef int AzZoomSliderFlags;
    enum AzPopupFlags_
    {
        AzZoomSliderFlags_None = 0,
        AzZoomSliderFlags_Vertical = 1,
        AzZoomSliderFlags_NoAnchors = 2,
        AzZoomSliderFlags_NoMiddleCarets = 4,
        AzZoomSliderFlags_NoWheel = 8,
    };

    template <typename T>
    bool AzZoomSlider(const T lower,
                      const T higher,
                      T& viewLower,
                      T& viewHigher,
                      float wheelRatio = 0.01f,
                      AzZoomSliderFlags flags = AzZoomSliderFlags_None)
    {
        bool interacted = false;
        AzIO& io = Az::GetIO();
        AzDrawList* draw_list = Az::GetWindowDrawList();

        static const float handleSize = 12;
        static const float roundRadius = 3.f;
        static const char* controlName = "AzZoomSlider";

        static bool movingScrollBarSvg = false;
        static bool sizingRBarSvg = false;
        static bool sizingLBarSvg = false;
        static AzID editingId = (AzID)-1;
        static float scrollingSource = 0.f;
        static float saveViewLower;
        static float saveViewHigher;

        const bool isVertical = flags & AzZoomSliderFlags_Vertical;
        const AzVec2 canvasPos = Az::GetCursorScreenPos();
        const AzVec2 canvasSize = Az::GetContentRegionAvail();
        const float canvasSizeLength = isVertical ? Az::GetItemRectSize().y
                                                  : canvasSize.x;
        const AzVec2 scrollBarSize = isVertical
                                             ? AzVec2(14.f, canvasSizeLength)
                                             : AzVec2(canvasSizeLength, 14.f);

        Az::InvisibleButton(controlName, scrollBarSize);
        const AzID currentId = Az::GetID(controlName);

        const bool usingEditingId = currentId == editingId;
        const bool canUseControl = usingEditingId || editingId == -1;
        const bool movingScrollBar = usingEditingId ? movingScrollBarSvg
                                                    : false;
        const bool sizingRBar = usingEditingId ? sizingRBarSvg : false;
        const bool sizingLBar = usingEditingId ? sizingLBarSvg : false;
        const int componentIndex = isVertical ? 1 : 0;
        const AzVec2 scrollBarMin = Az::GetItemRectMin();
        const AzVec2 scrollBarMax = Az::GetItemRectMax();
        const AzVec2 scrollBarA = AzVec2(scrollBarMin.x, scrollBarMin.y)
                                  - (isVertical ? AzVec2(2, 0) : AzVec2(0, 2));
        const AzVec2 scrollBarB =
                isVertical ? AzVec2(scrollBarMax.x - 1.f,
                                    scrollBarMin.y + canvasSizeLength)
                           : AzVec2(scrollBarMin.x + canvasSizeLength,
                                    scrollBarMax.y - 1.f);
        const float scrollStart = ((viewLower - lower) / (higher - lower))
                                          * canvasSizeLength
                                  + scrollBarMin[componentIndex];
        const float scrollEnd = ((viewHigher - lower) / (higher - lower))
                                        * canvasSizeLength
                                + scrollBarMin[componentIndex];
        const float screenSize = scrollEnd - scrollStart;
        const AzVec2 scrollTopLeft = isVertical ? AzVec2(scrollBarMin.x,
                                                         scrollStart)
                                                : AzVec2(scrollStart,
                                                         scrollBarMin.y);
        const AzVec2 scrollBottomRight = isVertical
                                                 ? AzVec2(scrollBarMax.x - 2.f,
                                                          scrollEnd)
                                                 : AzVec2(scrollEnd,
                                                          scrollBarMax.y - 2.f);
        const bool inScrollBar = canUseControl
                                 && AzRect(scrollTopLeft, scrollBottomRight)
                                            .Contains(io.MousePos);
        const AzRect scrollBarRect(scrollBarA, scrollBarB);
        const float deltaScreen = io.MousePos[componentIndex] - scrollingSource;
        const float deltaView = ((higher - lower) / canvasSizeLength)
                                * deltaScreen;
        const uint32_t barColor = Az::GetColorU32(
                (inScrollBar || movingScrollBar) ? AzCol_FrameBgHovered
                                                 : AzCol_FrameBg);
        const float middleCoord = (scrollStart + scrollEnd) * 0.5f;
        const bool insideControl =
                canUseControl
                && AzRect(scrollBarMin, scrollBarMax).Contains(io.MousePos);
        const bool hasAnchors = !(flags & AzZoomSliderFlags_NoAnchors);
        const float viewMinSize = ((3.f * handleSize) / canvasSizeLength)
                                  * (higher - lower);
        const auto ClipView = [lower, higher, &viewLower, &viewHigher]() {
            if (viewLower < lower)
            {
                const float deltaClip = lower - viewLower;
                viewLower += deltaClip;
                viewHigher += deltaClip;
            }
            if (viewHigher > higher)
            {
                const float deltaClip = viewHigher - higher;
                viewLower -= deltaClip;
                viewHigher -= deltaClip;
            }
        };

        bool onLeft = false;
        bool onRight = false;

        draw_list->AddRectFilled(scrollBarA,
                                 scrollBarB,
                                 0xFF101010,
                                 roundRadius);
        draw_list->AddRectFilled(scrollBarA, scrollBarB, 0xFF222222, 0);
        draw_list->AddRectFilled(scrollTopLeft,
                                 scrollBottomRight,
                                 barColor,
                                 roundRadius);

        if (!(flags & AzZoomSliderFlags_NoMiddleCarets))
        {
            for (float i = 0.5f; i < 3.f; i += 1.f)
            {
                const float coordA = middleCoord - handleSize * 0.5f;
                const float coordB = middleCoord + handleSize * 0.5f;
                AzVec2 base = scrollBarMin;
                base.x += scrollBarSize.x * 0.25f * i;
                base.y += scrollBarSize.y * 0.25f * i;

                if (isVertical)
                {
                    draw_list->AddLine(AzVec2(base.x, coordA),
                                       AzVec2(base.x, coordB),
                                       Az::GetColorU32(AzCol_SliderGrab));
                } else
                {
                    draw_list->AddLine(AzVec2(coordA, base.y),
                                       AzVec2(coordB, base.y),
                                       Az::GetColorU32(AzCol_SliderGrab));
                }
            }
        }

        if (io.MouseClicked[0] && insideControl && !inScrollBar)
        {
            const float ratio = (io.MousePos[componentIndex]
                                 - scrollBarMin[componentIndex])
                                / (scrollBarMax[componentIndex]
                                   - scrollBarMin[componentIndex]);
            const float size = (higher - lower);
            const float halfViewSize = (viewHigher - viewLower) * 0.5f;
            const float middle = ratio * size + lower;
            viewLower = middle - halfViewSize;
            viewHigher = middle + halfViewSize;
            ClipView();
            interacted = true;
        }

        if (!(flags & AzZoomSliderFlags_NoWheel) && inScrollBar
            && fabsf(io.MouseWheel) > 0.f)
        {
            const float ratio = (io.MousePos[componentIndex] - scrollStart)
                                / (scrollEnd - scrollStart);
            const float amount = io.MouseWheel * wheelRatio
                                 * (viewHigher - viewLower);

            viewLower -= ratio * amount;
            viewHigher += (1.f - ratio) * amount;
            ClipView();
            interacted = true;
        }

        if (screenSize > handleSize * 2.f && hasAnchors)
        {
            const AzRect barHandleLeft(
                    scrollTopLeft,
                    isVertical ? AzVec2(scrollBottomRight.x,
                                        scrollTopLeft.y + handleSize)
                               : AzVec2(scrollTopLeft.x + handleSize,
                                        scrollBottomRight.y));
            const AzRect barHandleRight(
                    isVertical ? AzVec2(scrollTopLeft.x,
                                        scrollBottomRight.y - handleSize)
                               : AzVec2(scrollBottomRight.x - handleSize,
                                        scrollTopLeft.y),
                    scrollBottomRight);

            onLeft = barHandleLeft.Contains(io.MousePos);
            onRight = barHandleRight.Contains(io.MousePos);

            draw_list->AddRectFilled(barHandleLeft.Min,
                                     barHandleLeft.Max,
                                     Az::GetColorU32(
                                             (onLeft || sizingLBar)
                                                     ? AzCol_SliderGrabActive
                                                     : AzCol_SliderGrab),
                                     roundRadius);
            draw_list->AddRectFilled(barHandleRight.Min,
                                     barHandleRight.Max,
                                     Az::GetColorU32(
                                             (onRight || sizingRBar)
                                                     ? AzCol_SliderGrabActive
                                                     : AzCol_SliderGrab),
                                     roundRadius);
        }

        if (sizingRBar)
        {
            if (!io.MouseDown[0])
            {
                sizingRBarSvg = false;
                editingId = (AzID)-1;
            } else
            {
                viewHigher = AzMin(saveViewHigher + deltaView, higher);
            }
        } else if (sizingLBar)
        {
            if (!io.MouseDown[0])
            {
                sizingLBarSvg = false;
                editingId = (AzID)-1;
            } else
            {
                viewLower = AzMax(saveViewLower + deltaView, lower);
            }
        } else
        {
            if (movingScrollBar)
            {
                if (!io.MouseDown[0])
                {
                    movingScrollBarSvg = false;
                    editingId = (AzID)-1;
                } else
                {
                    viewLower = saveViewLower + deltaView;
                    viewHigher = saveViewHigher + deltaView;
                    ClipView();
                }
            } else
            {
                if (inScrollBar && Az::IsMouseClicked(0))
                {
                    movingScrollBarSvg = true;
                    scrollingSource = io.MousePos[componentIndex];
                    saveViewLower = viewLower;
                    saveViewHigher = viewHigher;
                    editingId = currentId;
                }
                if (!sizingRBar && onRight && Az::IsMouseClicked(0)
                    && hasAnchors)
                {
                    sizingRBarSvg = true;
                    editingId = currentId;
                }
                if (!sizingLBar && onLeft && Az::IsMouseClicked(0)
                    && hasAnchors)
                {
                    sizingLBarSvg = true;
                    editingId = currentId;
                }
            }
        }

        if ((viewHigher - viewLower) < viewMinSize)
        {
            const float middle = (viewLower + viewHigher) * 0.5f;
            viewLower = middle - viewMinSize * 0.5f;
            viewHigher = middle + viewMinSize * 0.5f;
            ClipView();
        }

        return movingScrollBar || sizingRBar || sizingLBar || interacted;
    }

} // namespace AzGuizmo
