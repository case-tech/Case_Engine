#include <Az/Az.hpp>
#include <Az/AzInternal.hpp>
#include <Az/AzSequencer.hpp>
#include <cstdlib>

namespace AzGuizmo
{
#ifndef AZ_DEFINE_MATH_OPERATORS
    static AzVec2 operator+(const AzVec2& a, const AzVec2& b)
    {
        return AzVec2(a.x + b.x, a.y + b.y);
    }
#endif
    static bool SequencerAddDelButton(AzDrawList* draw_list,
                                      AzVec2 pos,
                                      bool add = true)
    {
        AzIO& io = Az::GetIO();
        AzRect btnRect(pos, AzVec2(pos.x + 16, pos.y + 16));
        bool overBtn = btnRect.Contains(io.MousePos);
        bool containedClick = overBtn
                              && btnRect.Contains(io.MouseClickedPos[0]);
        bool clickedBtn = containedClick && io.MouseReleased[0];
        int btnColor = overBtn ? 0xAAEAFFAA : 0x77A3B2AA;
        if (containedClick && io.MouseDownDuration[0] > 0)
            btnRect.Expand(2.0f);

        float midy = pos.y + 16 / 2 - 0.5f;
        float midx = pos.x + 16 / 2 - 0.5f;
        draw_list->AddRect(btnRect.Min, btnRect.Max, btnColor, 4);
        draw_list->AddLine(AzVec2(btnRect.Min.x + 3, midy),
                           AzVec2(btnRect.Max.x - 3, midy),
                           btnColor,
                           2);
        if (add)
            draw_list->AddLine(AzVec2(midx, btnRect.Min.y + 3),
                               AzVec2(midx, btnRect.Max.y - 3),
                               btnColor,
                               2);
        return clickedBtn;
    }

    bool Sequencer(SequenceInterface* sequence,
                   int* currentFrame,
                   bool* expanded,
                   int* selectedEntry,
                   int* firstFrame,
                   int sequenceOptions)
    {
        bool ret = false;
        AzIO& io = Az::GetIO();
        int cx = (int)(io.MousePos.x);
        int cy = (int)(io.MousePos.y);
        static float framePixelWidth = 10.f;
        static float framePixelWidthTarget = 10.f;
        int legendWidth = 200;

        static int movingEntry = -1;
        static int movingPos = -1;
        static int movingPart = -1;
        int delEntry = -1;
        int dupEntry = -1;
        int ItemHeight = 20;

        bool popupOpened = false;
        int sequenceCount = sequence->GetItemCount();
        if (!sequenceCount)
            return false;
        Az::BeginGroup();

        AzDrawList* draw_list = Az::GetWindowDrawList();
        AzVec2 canvas_pos = Az::GetCursorScreenPos();
        AzVec2 canvas_size = Az::GetContentRegionAvail();
        int firstFrameUsed = firstFrame ? *firstFrame : 0;

        int controlHeight = sequenceCount * ItemHeight;
        for (int i = 0; i < sequenceCount; i++)
            controlHeight += int(sequence->GetCustomHeight(i));
        int frameCount = AzMax(sequence->GetFrameMax()
                                       - sequence->GetFrameMin(),
                               1);

        static bool MovingScrollBar = false;
        static bool MovingCurrentFrame = false;
        struct CustomDraw
        {
            int index;
            AzRect customRect;
            AzRect legendRect;
            AzRect clippingRect;
            AzRect legendClippingRect;
        };
        AzVector<CustomDraw> customDraws;
        AzVector<CustomDraw> compactCustomDraws;

        const int visibleFrameCount = (int)floorf((canvas_size.x - legendWidth)
                                                  / framePixelWidth);
        const float barWidthRatio = AzMin(visibleFrameCount / (float)frameCount,
                                          1.f);
        const float barWidthInPixels = barWidthRatio
                                       * (canvas_size.x - legendWidth);

        AzRect regionRect(canvas_pos, canvas_pos + canvas_size);

        static bool panningView = false;
        static AzVec2 panningViewSource;
        static int panningViewFrame;
        if (Az::IsWindowFocused() && io.KeyAlt && io.MouseDown[2])
        {
            if (!panningView)
            {
                panningViewSource = io.MousePos;
                panningView = true;
                panningViewFrame = *firstFrame;
            }
            *firstFrame = panningViewFrame
                          - int((io.MousePos.x - panningViewSource.x)
                                / framePixelWidth);
            *firstFrame = AzClamp(*firstFrame,
                                  sequence->GetFrameMin(),
                                  sequence->GetFrameMax() - visibleFrameCount);
        }
        if (panningView && !io.MouseDown[2])
        {
            panningView = false;
        }
        framePixelWidthTarget = AzClamp(framePixelWidthTarget, 0.1f, 50.f);

        framePixelWidth = AzLerp(framePixelWidth, framePixelWidthTarget, 0.33f);

        frameCount = sequence->GetFrameMax() - sequence->GetFrameMin();
        if (visibleFrameCount >= frameCount && firstFrame)
            *firstFrame = sequence->GetFrameMin();

        if (expanded && !*expanded)
        {
            Az::InvisibleButton("canvas",
                                AzVec2(canvas_size.x - canvas_pos.x,
                                       (float)ItemHeight));
            draw_list->AddRectFilled(canvas_pos,
                                     AzVec2(canvas_size.x + canvas_pos.x,
                                            canvas_pos.y + ItemHeight),
                                     0xFF3D3837,
                                     0);
            char tmps[512];
            AzFormatString(tmps,
                           AZ_ARRAYSIZE(tmps),
                           sequence->GetCollapseFmt(),
                           frameCount,
                           sequenceCount);
            draw_list->AddText(AzVec2(canvas_pos.x + 26, canvas_pos.y + 2),
                               0xFFFFFFFF,
                               tmps);
        } else
        {
            bool hasScrollBar(true);

            AzVec2 headerSize(canvas_size.x, (float)ItemHeight);
            AzVec2 scrollBarSize(canvas_size.x, 14.f);
            Az::InvisibleButton("topBar", headerSize);
            draw_list->AddRectFilled(canvas_pos,
                                     canvas_pos + headerSize,
                                     0xFFFF0000,
                                     0);
            AzVec2 childFramePos = Az::GetCursorScreenPos();
            AzVec2 childFrameSize(canvas_size.x,
                                  canvas_size.y - 8.f - headerSize.y
                                          - (hasScrollBar ? scrollBarSize.y
                                                          : 0));
            Az::PushStyleColor(AzCol_FrameBg, 0);
            Az::BeginChild(889, childFrameSize, AzChildFlags_FrameStyle);
            sequence->focused = Az::IsWindowFocused();
            Az::InvisibleButton("contentBar",
                                AzVec2(canvas_size.x, float(controlHeight)));
            const AzVec2 contentMin = Az::GetItemRectMin();
            const AzVec2 contentMax = Az::GetItemRectMax();
            const AzRect contentRect(contentMin, contentMax);
            const float contentHeight = contentMax.y - contentMin.y;

            draw_list->AddRectFilled(canvas_pos,
                                     canvas_pos + canvas_size,
                                     0xFF242424,
                                     0);

            AzRect topRect(AzVec2(canvas_pos.x + legendWidth, canvas_pos.y),
                           AzVec2(canvas_pos.x + canvas_size.x,
                                  canvas_pos.y + ItemHeight));

            if (!MovingCurrentFrame && !MovingScrollBar && movingEntry == -1
                && sequenceOptions & SEQUENCER_CHANGE_FRAME && currentFrame
                && *currentFrame >= 0 && topRect.Contains(io.MousePos)
                && io.MouseDown[0])
            {
                MovingCurrentFrame = true;
            }
            if (MovingCurrentFrame)
            {
                if (frameCount)
                {
                    *currentFrame = (int)((io.MousePos.x - topRect.Min.x)
                                          / framePixelWidth)
                                    + firstFrameUsed;
                    if (*currentFrame < sequence->GetFrameMin())
                        *currentFrame = sequence->GetFrameMin();
                    if (*currentFrame >= sequence->GetFrameMax())
                        *currentFrame = sequence->GetFrameMax();
                }
                if (!io.MouseDown[0])
                    MovingCurrentFrame = false;
            }

            draw_list->AddRectFilled(canvas_pos,
                                     AzVec2(canvas_size.x + canvas_pos.x,
                                            canvas_pos.y + ItemHeight),
                                     0xFF3D3837,
                                     0);
            if (sequenceOptions & SEQUENCER_ADD)
            {
                if (SequencerAddDelButton(draw_list,
                                          AzVec2(canvas_pos.x + legendWidth
                                                         - ItemHeight,
                                                 canvas_pos.y + 2),
                                          true))
                    Az::OpenPopup("addEntry");

                if (Az::BeginPopup("addEntry"))
                {
                    for (int i = 0; i < sequence->GetItemTypeCount(); i++)
                        if (Az::Selectable(sequence->GetItemTypeName(i)))
                        {
                            sequence->Add(i);
                            *selectedEntry = sequence->GetItemCount() - 1;
                        }

                    Az::EndPopup();
                    popupOpened = true;
                }
            }

            int modFrameCount = 10;
            int frameStep = 1;
            while ((modFrameCount * framePixelWidth) < 150)
            {
                modFrameCount *= 2;
                frameStep *= 2;
            };
            int halfModFrameCount = modFrameCount / 2;

            auto drawLine = [&](int i, int regionHeight) {
                bool baseIndex = ((i % modFrameCount) == 0)
                                 || (i == sequence->GetFrameMax()
                                     || i == sequence->GetFrameMin());
                bool halfIndex = (i % halfModFrameCount) == 0;
                int px = (int)canvas_pos.x + int(i * framePixelWidth)
                         + legendWidth - int(firstFrameUsed * framePixelWidth);
                int tiretStart = baseIndex ? 4 : (halfIndex ? 10 : 14);
                int tiretEnd = baseIndex ? regionHeight : ItemHeight;

                if (px <= (canvas_size.x + canvas_pos.x)
                    && px >= (canvas_pos.x + legendWidth))
                {
                    draw_list->AddLine(
                            AzVec2((float)px, canvas_pos.y + (float)tiretStart),
                            AzVec2((float)px,
                                   canvas_pos.y + (float)tiretEnd - 1),
                            0xFF606060,
                            1);

                    draw_list->AddLine(
                            AzVec2((float)px, canvas_pos.y + (float)ItemHeight),
                            AzVec2((float)px,
                                   canvas_pos.y + (float)regionHeight - 1),
                            0x30606060,
                            1);
                }

                if (baseIndex && px > (canvas_pos.x + legendWidth))
                {
                    char tmps[512];
                    AzFormatString(tmps, AZ_ARRAYSIZE(tmps), "%d", i);
                    draw_list->AddText(AzVec2((float)px + 3.f, canvas_pos.y),
                                       0xFFBBBBBB,
                                       tmps);
                }
            };

            auto drawLineContent = [&](int i, int) {
                int px = (int)canvas_pos.x + int(i * framePixelWidth)
                         + legendWidth - int(firstFrameUsed * framePixelWidth);
                int tiretStart = int(contentMin.y);
                int tiretEnd = int(contentMax.y);

                if (px <= (canvas_size.x + canvas_pos.x)
                    && px >= (canvas_pos.x + legendWidth))
                {
                    draw_list->AddLine(AzVec2(float(px), float(tiretStart)),
                                       AzVec2(float(px), float(tiretEnd)),
                                       0x30606060,
                                       1);
                }
            };
            for (int i = sequence->GetFrameMin(); i <= sequence->GetFrameMax();
                 i += frameStep)
            {
                drawLine(i, ItemHeight);
            }
            drawLine(sequence->GetFrameMin(), ItemHeight);
            drawLine(sequence->GetFrameMax(), ItemHeight);

            draw_list->PushClipRect(childFramePos,
                                    childFramePos + childFrameSize,
                                    true);

            size_t customHeight = 0;
            for (int i = 0; i < sequenceCount; i++)
            {
                int type;
                sequence->Get(i, NULL, NULL, &type, NULL);
                AzVec2 tpos(contentMin.x + 3,
                            contentMin.y + i * ItemHeight + 2 + customHeight);
                draw_list->AddText(tpos, 0xFFFFFFFF, sequence->GetItemLabel(i));

                if (sequenceOptions & SEQUENCER_DEL)
                {
                    if (SequencerAddDelButton(draw_list,
                                              AzVec2(contentMin.x + legendWidth
                                                             - ItemHeight + 2
                                                             - 10,
                                                     tpos.y + 2),
                                              false))
                        delEntry = i;

                    if (SequencerAddDelButton(
                                draw_list,
                                AzVec2(contentMin.x + legendWidth - ItemHeight
                                               - ItemHeight + 2 - 10,
                                       tpos.y + 2),
                                true))
                        dupEntry = i;
                }
                customHeight += sequence->GetCustomHeight(i);
            }

            customHeight = 0;
            for (int i = 0; i < sequenceCount; i++)
            {
                unsigned int col = (i & 1) ? 0xFF3A3636 : 0xFF413D3D;

                size_t localCustomHeight = sequence->GetCustomHeight(i);
                AzVec2 pos = AzVec2(contentMin.x + legendWidth,
                                    contentMin.y + ItemHeight * i + 1
                                            + customHeight);
                AzVec2 sz = AzVec2(canvas_size.x + canvas_pos.x,
                                   pos.y + ItemHeight - 1 + localCustomHeight);
                if (!popupOpened && cy >= pos.y
                    && cy < pos.y + (ItemHeight + localCustomHeight)
                    && movingEntry == -1 && cx > contentMin.x
                    && cx < contentMin.x + canvas_size.x)
                {
                    col += 0x80201008;
                    pos.x -= legendWidth;
                }
                draw_list->AddRectFilled(pos, sz, col, 0);
                customHeight += localCustomHeight;
            }

            draw_list->PushClipRect(childFramePos
                                            + AzVec2(float(legendWidth), 0.f),
                                    childFramePos + childFrameSize,
                                    true);

            for (int i = sequence->GetFrameMin(); i <= sequence->GetFrameMax();
                 i += frameStep)
            {
                drawLineContent(i, int(contentHeight));
            }
            drawLineContent(sequence->GetFrameMin(), int(contentHeight));
            drawLineContent(sequence->GetFrameMax(), int(contentHeight));

            bool selected = selectedEntry && (*selectedEntry >= 0);
            if (selected)
            {
                customHeight = 0;
                for (int i = 0; i < *selectedEntry; i++)
                    customHeight += sequence->GetCustomHeight(i);
                draw_list->AddRectFilled(
                        AzVec2(contentMin.x,
                               contentMin.y + ItemHeight * *selectedEntry
                                       + customHeight),
                        AzVec2(contentMin.x + canvas_size.x,
                               contentMin.y + ItemHeight * (*selectedEntry + 1)
                                       + customHeight),
                        0x801080FF,
                        1.f);
            }

            customHeight = 0;
            for (int i = 0; i < sequenceCount; i++)
            {
                int *start, *end;
                unsigned int color;
                sequence->Get(i, &start, &end, NULL, &color);
                size_t localCustomHeight = sequence->GetCustomHeight(i);

                AzVec2 pos = AzVec2(contentMin.x + legendWidth
                                            - firstFrameUsed * framePixelWidth,
                                    contentMin.y + ItemHeight * i + 1
                                            + customHeight);
                AzVec2 slotP1(pos.x + *start * framePixelWidth, pos.y + 2);
                AzVec2 slotP2(pos.x + *end * framePixelWidth + framePixelWidth,
                              pos.y + ItemHeight - 2);
                AzVec2 slotP3(pos.x + *end * framePixelWidth + framePixelWidth,
                              pos.y + ItemHeight - 2 + localCustomHeight);
                unsigned int slotColor = color | 0xFF000000;
                unsigned int slotColorHalf = (color & 0xFFFFFF) | 0x40000000;

                if (slotP1.x <= (canvas_size.x + contentMin.x)
                    && slotP2.x >= (contentMin.x + legendWidth))
                {
                    draw_list->AddRectFilled(slotP1, slotP3, slotColorHalf, 2);
                    draw_list->AddRectFilled(slotP1, slotP2, slotColor, 2);
                }
                if (AzRect(slotP1, slotP2).Contains(io.MousePos)
                    && io.MouseDoubleClicked[0])
                {
                    sequence->DoubleClick(i);
                }

                const float max_handle_width = slotP2.x - slotP1.x / 3.0f;
                const float min_handle_width = AzMin(10.0f, max_handle_width);
                const float handle_width = AzClamp(framePixelWidth / 2.0f,
                                                   min_handle_width,
                                                   max_handle_width);
                AzRect rects[3] = {
                        AzRect(slotP1,
                               AzVec2(slotP1.x + handle_width, slotP2.y)),
                        AzRect(AzVec2(slotP2.x - handle_width, slotP1.y),
                               slotP2),
                        AzRect(slotP1, slotP2)};

                const unsigned int quadColor[] = {
                        0xFFFFFFFF,
                        0xFFFFFFFF,
                        slotColor + (selected ? 0 : 0x202020)};
                if (movingEntry == -1
                    && (sequenceOptions & SEQUENCER_EDIT_STARTEND))
                {
                    for (int j = 2; j >= 0; j--)
                    {
                        AzRect& rc = rects[j];
                        if (!rc.Contains(io.MousePos))
                            continue;
                        draw_list->AddRectFilled(rc.Min,
                                                 rc.Max,
                                                 quadColor[j],
                                                 2);
                    }

                    for (int j = 0; j < 3; j++)
                    {
                        AzRect& rc = rects[j];
                        if (!rc.Contains(io.MousePos))
                            continue;
                        if (!AzRect(childFramePos,
                                    childFramePos + childFrameSize)
                                     .Contains(io.MousePos))
                            continue;
                        if (Az::IsMouseClicked(0) && !MovingScrollBar
                            && !MovingCurrentFrame)
                        {
                            movingEntry = i;
                            movingPos = cx;
                            movingPart = j + 1;
                            sequence->BeginEdit(movingEntry);
                            break;
                        }
                    }
                }

                if (localCustomHeight > 0)
                {
                    AzVec2 rp(canvas_pos.x,
                              contentMin.y + ItemHeight * i + 1 + customHeight);
                    AzRect customRect(
                            rp
                                    + AzVec2(
                                            legendWidth
                                                    - (firstFrameUsed
                                                       - sequence->GetFrameMin()
                                                       - 0.5f)
                                                              * framePixelWidth,
                                            float(ItemHeight)),
                            rp
                                    + AzVec2(
                                            legendWidth
                                                    + (sequence->GetFrameMax()
                                                       - firstFrameUsed - 0.5f
                                                       + 2.f) * framePixelWidth,
                                            float(localCustomHeight
                                                  + ItemHeight)));
                    AzRect clippingRect(rp
                                                + AzVec2(float(legendWidth),
                                                         float(ItemHeight)),
                                        rp
                                                + AzVec2(canvas_size.x,
                                                         float(localCustomHeight
                                                               + ItemHeight)));

                    AzRect legendRect(
                            rp + AzVec2(0.f, float(ItemHeight)),
                            rp
                                    + AzVec2(float(legendWidth),
                                             float(localCustomHeight)));
                    AzRect legendClippingRect(
                            canvas_pos + AzVec2(0.f, float(ItemHeight)),
                            canvas_pos
                                    + AzVec2(float(legendWidth),
                                             float(localCustomHeight
                                                   + ItemHeight)));
                    customDraws.push_back({i,
                                           customRect,
                                           legendRect,
                                           clippingRect,
                                           legendClippingRect});
                } else
                {
                    AzVec2 rp(canvas_pos.x,
                              contentMin.y + ItemHeight * i + customHeight);
                    AzRect customRect(
                            rp
                                    + AzVec2(
                                            legendWidth
                                                    - (firstFrameUsed
                                                       - sequence->GetFrameMin()
                                                       - 0.5f)
                                                              * framePixelWidth,
                                            float(0.f)),
                            rp
                                    + AzVec2(
                                            legendWidth
                                                    + (sequence->GetFrameMax()
                                                       - firstFrameUsed - 0.5f
                                                       + 2.f) * framePixelWidth,
                                            float(ItemHeight)));
                    AzRect clippingRect(
                            rp + AzVec2(float(legendWidth), float(0.f)),
                            rp + AzVec2(canvas_size.x, float(ItemHeight)));

                    compactCustomDraws.push_back(
                            {i, customRect, AzRect(), clippingRect, AzRect()});
                }
                customHeight += localCustomHeight;
            }

            if (movingEntry >= 0)
            {
#if AZ_VERSION_NUM >= 18723
                Az::SetNextFrameWantCaptureMouse(true);
#else
                Az::CaptureMouseFromApp();
#endif
                int diffFrame = int((cx - movingPos) / framePixelWidth);
                if (std::abs(diffFrame) > 0)
                {
                    int *start, *end;
                    sequence->Get(movingEntry, &start, &end, NULL, NULL);
                    if (selectedEntry)
                        *selectedEntry = movingEntry;
                    int& l = *start;
                    int& r = *end;
                    if (movingPart & 1)
                        l += diffFrame;
                    if (movingPart & 2)
                        r += diffFrame;
                    if (l < 0)
                    {
                        if (movingPart & 2)
                            r -= l;
                        l = 0;
                    }
                    if (movingPart & 1 && l > r)
                        l = r;
                    if (movingPart & 2 && r < l)
                        r = l;
                    movingPos += int(diffFrame * framePixelWidth);
                }
                if (!io.MouseDown[0])
                {
                    if (!diffFrame && movingPart && selectedEntry)
                    {
                        *selectedEntry = movingEntry;
                        ret = true;
                    }

                    movingEntry = -1;
                    sequence->EndEdit();
                }
            }

            if (currentFrame && firstFrame && *currentFrame >= *firstFrame
                && *currentFrame <= sequence->GetFrameMax())
            {
                static const float cursorWidth = 8.f;
                float cursorOffset = contentMin.x + legendWidth
                                     + (*currentFrame - firstFrameUsed)
                                               * framePixelWidth
                                     + framePixelWidth / 2 - cursorWidth * 0.5f;
                draw_list->AddLine(AzVec2(cursorOffset, canvas_pos.y),
                                   AzVec2(cursorOffset, contentMax.y),
                                   0xA02A2AFF,
                                   cursorWidth);
                char tmps[512];
                AzFormatString(tmps, AZ_ARRAYSIZE(tmps), "%d", *currentFrame);
                draw_list->AddText(AzVec2(cursorOffset + 10, canvas_pos.y + 2),
                                   0xFF2A2AFF,
                                   tmps);
            }

            draw_list->PopClipRect();
            draw_list->PopClipRect();

            for (auto& customDraw : customDraws)
                sequence->CustomDraw(customDraw.index,
                                     draw_list,
                                     customDraw.customRect,
                                     customDraw.legendRect,
                                     customDraw.clippingRect,
                                     customDraw.legendClippingRect);
            for (auto& customDraw : compactCustomDraws)
                sequence->CustomDrawCompact(customDraw.index,
                                            draw_list,
                                            customDraw.customRect,
                                            customDraw.clippingRect);

            if (sequenceOptions & SEQUENCER_COPYPASTE)
            {
                AzRect rectCopy(AzVec2(contentMin.x + 100, canvas_pos.y + 2),
                                AzVec2(contentMin.x + 100 + 30,
                                       canvas_pos.y + ItemHeight - 2));
                bool inRectCopy = rectCopy.Contains(io.MousePos);
                unsigned int copyColor = inRectCopy ? 0xFF1080FF : 0xFF000000;
                draw_list->AddText(rectCopy.Min, copyColor, "Copy");

                AzRect rectPaste(AzVec2(contentMin.x + 140, canvas_pos.y + 2),
                                 AzVec2(contentMin.x + 140 + 30,
                                        canvas_pos.y + ItemHeight - 2));
                bool inRectPaste = rectPaste.Contains(io.MousePos);
                unsigned int pasteColor = inRectPaste ? 0xFF1080FF : 0xFF000000;
                draw_list->AddText(rectPaste.Min, pasteColor, "Paste");

                if (inRectCopy && io.MouseReleased[0])
                {
                    sequence->Copy();
                }
                if (inRectPaste && io.MouseReleased[0])
                {
                    sequence->Paste();
                }
            }

            Az::EndChild();
            Az::PopStyleColor();
            if (hasScrollBar)
            {
                Az::InvisibleButton("scrollBar", scrollBarSize);
                AzVec2 scrollBarMin = Az::GetItemRectMin();
                AzVec2 scrollBarMax = Az::GetItemRectMax();

                float startFrameOffset = ((float)(firstFrameUsed
                                                  - sequence->GetFrameMin())
                                          / (float)frameCount)
                                         * (canvas_size.x - legendWidth);
                AzVec2 scrollBarA(scrollBarMin.x + legendWidth,
                                  scrollBarMin.y - 2);
                AzVec2 scrollBarB(scrollBarMin.x + canvas_size.x,
                                  scrollBarMax.y - 1);
                draw_list->AddRectFilled(scrollBarA, scrollBarB, 0xFF222222, 0);

                AzRect scrollBarRect(scrollBarA, scrollBarB);
                bool inScrollBar = scrollBarRect.Contains(io.MousePos);

                draw_list->AddRectFilled(scrollBarA, scrollBarB, 0xFF101010, 8);

                AzVec2 scrollBarC(scrollBarMin.x + legendWidth
                                          + startFrameOffset,
                                  scrollBarMin.y);
                AzVec2 scrollBarD(scrollBarMin.x + legendWidth
                                          + barWidthInPixels + startFrameOffset,
                                  scrollBarMax.y - 2);
                draw_list->AddRectFilled(scrollBarC,
                                         scrollBarD,
                                         (inScrollBar || MovingScrollBar)
                                                 ? 0xFF606060
                                                 : 0xFF505050,
                                         6);

                AzRect barHandleLeft(scrollBarC,
                                     AzVec2(scrollBarC.x + 14, scrollBarD.y));
                AzRect barHandleRight(AzVec2(scrollBarD.x - 14, scrollBarC.y),
                                      scrollBarD);

                bool onLeft = barHandleLeft.Contains(io.MousePos);
                bool onRight = barHandleRight.Contains(io.MousePos);

                static bool sizingRBar = false;
                static bool sizingLBar = false;

                draw_list->AddRectFilled(barHandleLeft.Min,
                                         barHandleLeft.Max,
                                         (onLeft || sizingLBar) ? 0xFFAAAAAA
                                                                : 0xFF666666,
                                         6);
                draw_list->AddRectFilled(barHandleRight.Min,
                                         barHandleRight.Max,
                                         (onRight || sizingRBar) ? 0xFFAAAAAA
                                                                 : 0xFF666666,
                                         6);

                AzRect scrollBarThumb(scrollBarC, scrollBarD);
                static const float MinBarWidth = 44.f;
                if (sizingRBar)
                {
                    if (!io.MouseDown[0])
                    {
                        sizingRBar = false;
                    } else
                    {
                        float barNewWidth = AzMax(barWidthInPixels
                                                          + io.MouseDelta.x,
                                                  MinBarWidth);
                        float barRatio = barNewWidth / barWidthInPixels;
                        framePixelWidthTarget = framePixelWidth =
                                framePixelWidth / barRatio;
                        int newVisibleFrameCount = int(
                                (canvas_size.x - legendWidth)
                                / framePixelWidthTarget);
                        int lastFrame = *firstFrame + newVisibleFrameCount;
                        if (lastFrame > sequence->GetFrameMax())
                        {
                            framePixelWidthTarget = framePixelWidth =
                                    (canvas_size.x - legendWidth)
                                    / float(sequence->GetFrameMax()
                                            - *firstFrame);
                        }
                    }
                } else if (sizingLBar)
                {
                    if (!io.MouseDown[0])
                    {
                        sizingLBar = false;
                    } else
                    {
                        if (fabsf(io.MouseDelta.x) > FLT_EPSILON)
                        {
                            float barNewWidth = AzMax(barWidthInPixels
                                                              - io.MouseDelta.x,
                                                      MinBarWidth);
                            float barRatio = barNewWidth / barWidthInPixels;
                            float previousFramePixelWidthTarget =
                                    framePixelWidthTarget;
                            framePixelWidthTarget = framePixelWidth =
                                    framePixelWidth / barRatio;
                            int newVisibleFrameCount = int(visibleFrameCount
                                                           / barRatio);
                            int newFirstFrame = *firstFrame
                                                + newVisibleFrameCount
                                                - visibleFrameCount;
                            newFirstFrame =
                                    AzClamp(newFirstFrame,
                                            sequence->GetFrameMin(),
                                            AzMax(sequence->GetFrameMax()
                                                          - visibleFrameCount,
                                                  sequence->GetFrameMin()));
                            if (newFirstFrame == *firstFrame)
                            {
                                framePixelWidth = framePixelWidthTarget =
                                        previousFramePixelWidthTarget;
                            } else
                            {
                                *firstFrame = newFirstFrame;
                            }
                        }
                    }
                } else
                {
                    if (MovingScrollBar)
                    {
                        if (!io.MouseDown[0])
                        {
                            MovingScrollBar = false;
                        } else
                        {
                            float framesPerPixelInBar =
                                    barWidthInPixels / (float)visibleFrameCount;
                            *firstFrame = int((io.MousePos.x
                                               - panningViewSource.x)
                                              / framesPerPixelInBar)
                                          - panningViewFrame;
                            *firstFrame =
                                    AzClamp(*firstFrame,
                                            sequence->GetFrameMin(),
                                            AzMax(sequence->GetFrameMax()
                                                          - visibleFrameCount,
                                                  sequence->GetFrameMin()));
                        }
                    } else
                    {
                        if (scrollBarThumb.Contains(io.MousePos)
                            && Az::IsMouseClicked(0) && firstFrame
                            && !MovingCurrentFrame && movingEntry == -1)
                        {
                            MovingScrollBar = true;
                            panningViewSource = io.MousePos;
                            panningViewFrame = -*firstFrame;
                        }
                        if (!sizingRBar && onRight && Az::IsMouseClicked(0))
                            sizingRBar = true;
                        if (!sizingLBar && onLeft && Az::IsMouseClicked(0))
                            sizingLBar = true;
                    }
                }
            }
        }

        Az::EndGroup();

        if (regionRect.Contains(io.MousePos))
        {
            bool overCustomDraw = false;
            for (auto& custom : customDraws)
            {
                if (custom.customRect.Contains(io.MousePos))
                {
                    overCustomDraw = true;
                }
            }
            if (overCustomDraw)
            {
            } else
            {
#if 0
            frameOverCursor = *firstFrame + (int)(visibleFrameCount * ((io.MousePos.x - (float)legendWidth - canvas_pos.x) / (canvas_size.x - legendWidth)));

            if (io.MouseWheel < -FLT_EPSILON)
            {
               *firstFrame -= frameOverCursor;
               *firstFrame = int(*firstFrame * 1.1f);
               framePixelWidthTarget *= 0.9f;
               *firstFrame += frameOverCursor;
            }

            if (io.MouseWheel > FLT_EPSILON)
            {
               *firstFrame -= frameOverCursor;
               *firstFrame = int(*firstFrame * 0.9f);
               framePixelWidthTarget *= 1.1f;
               *firstFrame += frameOverCursor;
            }
#endif
            }
        }

        if (expanded)
        {
            if (SequencerAddDelButton(draw_list,
                                      AzVec2(canvas_pos.x + 2,
                                             canvas_pos.y + 2),
                                      !*expanded))
                *expanded = !*expanded;
        }

        if (delEntry != -1)
        {
            sequence->Del(delEntry);
            if (selectedEntry
                && (*selectedEntry == delEntry
                    || *selectedEntry >= sequence->GetItemCount()))
                *selectedEntry = -1;
        }

        if (dupEntry != -1)
        {
            sequence->Duplicate(dupEntry);
        }
        return ret;
    }
} // namespace AzGuizmo
