#define AZ_DEFINE_MATH_OPERATORS
#include <Az/Az.hpp>
#include <Az/AzGraphEditor.hpp>
#include <Az/AzInternal.hpp>
#include <array>
#include <float.h>
#include <math.h>
#include <vector>

namespace AzGuizmo
{

    static inline float Distance(const AzVec2& a, const AzVec2& b)
    {
        return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    static inline float sign(float v)
    {
        return (v >= 0.f) ? 1.f : -1.f;
    }

    static AzVec2 GetInputSlotPos(Delegate& delegate,
                                  const Node& node,
                                  SlotIndex slotIndex,
                                  float factor)
    {
        AzVec2 Size = node.mRect.GetSize() * factor;
        size_t InputsCount =
                delegate.GetTemplate(node.mTemplateIndex).mInputCount;
        return AzVec2(node.mRect.Min.x * factor,
                      node.mRect.Min.y * factor
                              + Size.y * ((float)slotIndex + 1)
                                        / ((float)InputsCount + 1)
                              + 8.f);
    }

    static AzVec2 GetOutputSlotPos(Delegate& delegate,
                                   const Node& node,
                                   SlotIndex slotIndex,
                                   float factor)
    {
        AzVec2 Size = node.mRect.GetSize() * factor;
        size_t OutputsCount =
                delegate.GetTemplate(node.mTemplateIndex).mOutputCount;
        return AzVec2(node.mRect.Min.x * factor + Size.x,
                      node.mRect.Min.y * factor
                              + Size.y * ((float)slotIndex + 1)
                                        / ((float)OutputsCount + 1)
                              + 8.f);
    }

    static AzRect GetNodeRect(const Node& node, float factor)
    {
        AzVec2 Size = node.mRect.GetSize() * factor;
        return AzRect(node.mRect.Min * factor, node.mRect.Min * factor + Size);
    }

    static AzVec2 editingNodeSource;
    static bool editingInput = false;
    static AzVec2 captureOffset;

    enum NodeOperation
    {
        NO_None,
        NO_EditingLink,
        NO_QuadSelecting,
        NO_MovingNodes,
        NO_EditInput,
        NO_PanView,
    };
    static NodeOperation nodeOperation = NO_None;

    static void HandleZoomScroll(AzRect regionRect,
                                 ViewState& viewState,
                                 const Options& options)
    {
        AzIO& io = Az::GetIO();

        if (regionRect.Contains(io.MousePos))
        {
            if (io.MouseWheel < -FLT_EPSILON)
            {
                viewState.mFactorTarget *= 1.f - options.mZoomRatio;
            }

            if (io.MouseWheel > FLT_EPSILON)
            {
                viewState.mFactorTarget *= 1.0f + options.mZoomRatio;
            }
        }

        AzVec2 mouseWPosPre = (io.MousePos - Az::GetCursorScreenPos())
                              / viewState.mFactor;
        viewState.mFactorTarget = AzClamp(viewState.mFactorTarget,
                                          options.mMinZoom,
                                          options.mMaxZoom);
        viewState.mFactor = AzLerp(viewState.mFactor,
                                   viewState.mFactorTarget,
                                   options.mZoomLerpFactor);
        AzVec2 mouseWPosPost = (io.MousePos - Az::GetCursorScreenPos())
                               / viewState.mFactor;
        if (Az::IsMousePosValid())
        {
            viewState.mPosition += mouseWPosPost - mouseWPosPre;
        }
    }

    void GraphEditorClear()
    {
        nodeOperation = NO_None;
    }

    static void FitNodes(Delegate& delegate,
                         ViewState& viewState,
                         const AzVec2 viewSize,
                         bool selectedNodesOnly)
    {
        const size_t nodeCount = delegate.GetNodeCount();

        if (!nodeCount)
        {
            return;
        }

        bool validNode = false;
        AzVec2 min(FLT_MAX, FLT_MAX);
        AzVec2 max(-FLT_MAX, -FLT_MAX);
        for (NodeIndex nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
        {
            const Node& node = delegate.GetNode(nodeIndex);

            if (selectedNodesOnly && !node.mSelected)
            {
                continue;
            }

            min = AzMin(min, node.mRect.Min);
            min = AzMin(min, node.mRect.Max);
            max = AzMax(max, node.mRect.Min);
            max = AzMax(max, node.mRect.Max);
            validNode = true;
        }

        if (!validNode)
        {
            return;
        }

        min -= viewSize * 0.05f;
        max += viewSize * 0.05f;
        AzVec2 nodesSize = max - min;
        AzVec2 nodeCenter = (max + min) * 0.5f;

        float ratioY = viewSize.y / nodesSize.y;
        float ratioX = viewSize.x / nodesSize.x;

        viewState.mFactor = viewState.mFactorTarget = AzMin(AzMin(ratioY,
                                                                  ratioX),
                                                            1.f);
        viewState.mPosition = AzVec2(-nodeCenter.x, -nodeCenter.y)
                              + (viewSize * 0.5f) / viewState.mFactorTarget;
    }

    static void DisplayLinks(Delegate& delegate,
                             AzDrawList* drawList,
                             const AzVec2 offset,
                             const float factor,
                             const AzRect regionRect,
                             NodeIndex hoveredNode,
                             const Options& options)
    {
        const size_t linkCount = delegate.GetLinkCount();
        for (LinkIndex linkIndex = 0; linkIndex < linkCount; linkIndex++)
        {
            const auto link = delegate.GetLink(linkIndex);
            const auto nodeInput = delegate.GetNode(link.mInputNodeIndex);
            const auto nodeOutput = delegate.GetNode(link.mOutputNodeIndex);
            AzVec2 p1 = offset
                        + GetOutputSlotPos(delegate,
                                           nodeInput,
                                           link.mInputSlotIndex,
                                           factor);
            AzVec2 p2 = offset
                        + GetInputSlotPos(delegate,
                                          nodeOutput,
                                          link.mOutputSlotIndex,
                                          factor);

            if ((p1.y < 0.f && p2.y < 0.f)
                || (p1.y > regionRect.Max.y && p2.y > regionRect.Max.y)
                || (p1.x < 0.f && p2.x < 0.f)
                || (p1.x > regionRect.Max.x && p2.x > regionRect.Max.x))
                continue;

            bool highlightCons = hoveredNode == link.mInputNodeIndex
                                 || hoveredNode == link.mOutputNodeIndex;
            uint32_t col =
                    delegate.GetTemplate(nodeInput.mTemplateIndex).mHeaderColor
                    | (highlightCons ? 0xF0F0F0 : 0);
            if (options.mDisplayLinksAsCurves)
            {
                drawList->AddBezierCubic(p1,
                                         p1 + AzVec2(50, 0) * factor,
                                         p2 + AzVec2(-50, 0) * factor,
                                         p2,
                                         0xFF000000,
                                         options.mLineThickness * 1.5f
                                                 * factor);
                drawList->AddBezierCubic(p1,
                                         p1 + AzVec2(50, 0) * factor,
                                         p2 + AzVec2(-50, 0) * factor,
                                         p2,
                                         col,
                                         options.mLineThickness * 1.5f
                                                 * factor);

            } else
            {
                std::array<AzVec2, 6> pts;
                int ptCount = 0;
                AzVec2 dif = p2 - p1;

                AzVec2 p1a, p1b;
                const float limitx = 12.f * factor;
                if (dif.x < limitx)
                {
                    AzVec2 p10 = p1 + AzVec2(limitx, 0.f);
                    AzVec2 p20 = p2 - AzVec2(limitx, 0.f);

                    dif = p20 - p10;
                    p1a = p10 + AzVec2(0.f, dif.y * 0.5f);
                    p1b = p1a + AzVec2(dif.x, 0.f);

                    pts = {p1, p10, p1a, p1b, p20, p2};
                    ptCount = 6;
                } else
                {
                    if (fabsf(dif.y) < 1.f)
                    {
                        pts = {p1, (p1 + p2) * 0.5f, p2};
                        ptCount = 3;
                    } else
                    {
                        if (fabsf(dif.y) < 10.f)
                        {
                            if (fabsf(dif.x) > fabsf(dif.y))
                            {
                                p1a = p1
                                      + AzVec2(fabsf(fabsf(dif.x)
                                                     - fabsf(dif.y))
                                                       * 0.5f * sign(dif.x),
                                               0.f);
                                p1b = p1a
                                      + AzVec2(fabsf(dif.y) * sign(dif.x),
                                               dif.y);
                            } else
                            {
                                p1a = p1
                                      + AzVec2(0.f,
                                               fabsf(fabsf(dif.y)
                                                     - fabsf(dif.x))
                                                       * 0.5f * sign(dif.y));
                                p1b = p1a
                                      + AzVec2(dif.x,
                                               fabsf(dif.x) * sign(dif.y));
                            }
                        } else
                        {
                            if (fabsf(dif.x) > fabsf(dif.y))
                            {
                                float d = fabsf(dif.y) * sign(dif.x) * 0.5f;
                                p1a = p1 + AzVec2(d, dif.y * 0.5f);
                                p1b = p1a
                                      + AzVec2(fabsf(fabsf(dif.x)
                                                     - fabsf(d) * 2.f)
                                                       * sign(dif.x),
                                               0.f);
                            } else
                            {
                                float d = fabsf(dif.x) * sign(dif.y) * 0.5f;
                                p1a = p1 + AzVec2(dif.x * 0.5f, d);
                                p1b = p1a
                                      + AzVec2(0.f,
                                               fabsf(fabsf(dif.y)
                                                     - fabsf(d) * 2.f)
                                                       * sign(dif.y));
                            }
                        }
                        pts = {p1, p1a, p1b, p2};
                        ptCount = 4;
                    }
                }
                float highLightFactor = factor * (highlightCons ? 2.0f : 1.f);
                for (int pass = 0; pass < 2; pass++)
                {
                    drawList->AddPolyline(pts.data(),
                                          ptCount,
                                          pass ? col : 0xFF000000,
                                          false,
                                          (pass ? options.mLineThickness
                                                : (options.mLineThickness
                                                   * 1.5f))
                                                  * highLightFactor);
                }
            }
        }
    }

    static void HandleQuadSelection(Delegate& delegate,
                                    AzDrawList* drawList,
                                    const AzVec2 offset,
                                    const float factor,
                                    AzRect contentRect,
                                    const Options& options)
    {
        if (!options.mAllowQuadSelection)
        {
            return;
        }
        AzIO& io = Az::GetIO();
        static AzVec2 quadSelectPos;

        auto nodeCount = delegate.GetNodeCount();

        if (nodeOperation == NO_QuadSelecting && Az::IsWindowFocused())
        {
            const AzVec2 bmin = AzMin(quadSelectPos, io.MousePos);
            const AzVec2 bmax = AzMax(quadSelectPos, io.MousePos);
            drawList->AddRectFilled(bmin, bmax, options.mQuadSelection, 1.f);
            drawList->AddRect(bmin, bmax, options.mQuadSelectionBorder, 1.f);
            if (!io.MouseDown[0])
            {
                if (!io.KeyCtrl && !io.KeyShift)
                {
                    for (size_t nodeIndex = 0; nodeIndex < nodeCount;
                         nodeIndex++)
                    {
                        delegate.SelectNode(nodeIndex, false);
                    }
                }

                nodeOperation = NO_None;
                AzRect selectionRect(bmin, bmax);
                for (unsigned int nodeIndex = 0; nodeIndex < nodeCount;
                     nodeIndex++)
                {
                    const auto node = delegate.GetNode(nodeIndex);
                    AzVec2 nodeRectangleMin = offset + node.mRect.Min * factor;
                    AzVec2 nodeRectangleMax = nodeRectangleMin
                                              + node.mRect.GetSize() * factor;
                    if (selectionRect.Overlaps(
                                AzRect(nodeRectangleMin, nodeRectangleMax)))
                    {
                        if (io.KeyCtrl)
                        {
                            delegate.SelectNode(nodeIndex, false);
                        } else
                        {
                            delegate.SelectNode(nodeIndex, true);
                        }
                    } else
                    {
                        if (!io.KeyShift)
                        {
                            delegate.SelectNode(nodeIndex, false);
                        }
                    }
                }
            }
        } else if (nodeOperation == NO_None && io.MouseDown[0]
                   && Az::IsWindowFocused()
                   && contentRect.Contains(io.MousePos))
        {
            nodeOperation = NO_QuadSelecting;
            quadSelectPos = io.MousePos;
        }
    }

    static bool HandleConnections(AzDrawList* drawList,
                                  NodeIndex nodeIndex,
                                  const AzVec2 offset,
                                  const float factor,
                                  Delegate& delegate,
                                  const Options& options,
                                  bool bDrawOnly,
                                  SlotIndex& inputSlotOver,
                                  SlotIndex& outputSlotOver,
                                  const bool inMinimap)
    {
        static NodeIndex editingNodeIndex;
        static SlotIndex editingSlotIndex;

        AzIO& io = Az::GetIO();
        const auto node = delegate.GetNode(nodeIndex);
        const auto nodeTemplate = delegate.GetTemplate(node.mTemplateIndex);
        const auto linkCount = delegate.GetLinkCount();

        size_t InputsCount = nodeTemplate.mInputCount;
        size_t OutputsCount = nodeTemplate.mOutputCount;
        inputSlotOver = -1;
        outputSlotOver = -1;

        bool hoverSlot = false;
        for (int i = 0; i < 2; i++)
        {
            float closestDistance = FLT_MAX;
            SlotIndex closestConn = -1;
            AzVec2 closestTextPos;
            AzVec2 closestPos;
            const size_t slotCount[2] = {InputsCount, OutputsCount};

            for (SlotIndex slotIndex = 0; slotIndex < slotCount[i]; slotIndex++)
            {
                const char** con = i ? nodeTemplate.mOutputNames
                                     : nodeTemplate.mInputNames;
                const char* conText = (con && con[slotIndex]) ? con[slotIndex]
                                                              : "";

                AzVec2 p = offset
                           + (i ? GetOutputSlotPos(delegate,
                                                   node,
                                                   slotIndex,
                                                   factor)
                                : GetInputSlotPos(delegate,
                                                  node,
                                                  slotIndex,
                                                  factor));
                float distance = Distance(p, io.MousePos);
                bool overCon = (nodeOperation == NO_None
                                || nodeOperation == NO_EditingLink)
                               && (distance < options.mNodeSlotRadius * 2.f)
                               && (distance < closestDistance);

                AzVec2 textSize;
                textSize = Az::CalcTextSize(conText);
                AzVec2 textPos = p
                                 + AzVec2(-options.mNodeSlotRadius
                                                          * (i ? -1.f : 1.f)
                                                          * (overCon ? 3.f
                                                                     : 2.f)
                                                  - (i ? 0 : textSize.x),
                                          -textSize.y / 2);

                AzRect nodeRect = GetNodeRect(node, factor);
                if (!inMinimap
                    && (overCon
                        || (nodeRect.Contains(io.MousePos - offset)
                            && closestConn == -1 && (editingInput == (i != 0))
                            && nodeOperation == NO_EditingLink)))
                {
                    closestDistance = distance;
                    closestConn = slotIndex;
                    closestTextPos = textPos;
                    closestPos = p;

                    if (i)
                    {
                        outputSlotOver = slotIndex;
                    } else
                    {
                        inputSlotOver = slotIndex;
                    }
                } else
                {
                    const AzU32* slotColorSource =
                            i ? nodeTemplate.mOutputColors
                              : nodeTemplate.mInputColors;
                    const AzU32 slotColor = slotColorSource
                                                    ? slotColorSource[slotIndex]
                                                    : options.mDefaultSlotColor;
                    drawList->AddCircleFilled(p,
                                              options.mNodeSlotRadius,
                                              AZ_COL32(0, 0, 0, 200));
                    drawList->AddCircleFilled(p,
                                              options.mNodeSlotRadius * 0.75f,
                                              slotColor);
                    if (!options.mDrawIONameOnHover)
                    {
                        drawList->AddText(io.FontDefault,
                                          14,
                                          textPos + AzVec2(2, 2),
                                          AZ_COL32(0, 0, 0, 255),
                                          conText);
                        drawList->AddText(io.FontDefault,
                                          14,
                                          textPos,
                                          AZ_COL32(150, 150, 150, 255),
                                          conText);
                    }
                }
            }

            if (closestConn != -1)
            {
                const char** con = i ? nodeTemplate.mOutputNames
                                     : nodeTemplate.mInputNames;
                const char* conText = (con && con[closestConn])
                                              ? con[closestConn]
                                              : "";
                const AzU32* slotColorSource = i ? nodeTemplate.mOutputColors
                                                 : nodeTemplate.mInputColors;
                const AzU32 slotColor = slotColorSource
                                                ? slotColorSource[closestConn]
                                                : options.mDefaultSlotColor;
                hoverSlot = true;
                drawList->AddCircleFilled(closestPos,
                                          options.mNodeSlotRadius
                                                  * options.mNodeSlotHoverFactor
                                                  * 0.75f,
                                          AZ_COL32(0, 0, 0, 200));
                drawList->AddCircleFilled(
                        closestPos,
                        options.mNodeSlotRadius * options.mNodeSlotHoverFactor,
                        slotColor);
                drawList->AddText(io.FontDefault,
                                  16,
                                  closestTextPos + AzVec2(1, 1),
                                  AZ_COL32(0, 0, 0, 255),
                                  conText);
                drawList->AddText(io.FontDefault,
                                  16,
                                  closestTextPos,
                                  AZ_COL32(250, 250, 250, 255),
                                  conText);
                bool inputToOutput = (!editingInput && !i)
                                     || (editingInput && i);
                if (nodeOperation == NO_EditingLink && !io.MouseDown[0]
                    && !bDrawOnly)
                {
                    if (inputToOutput)
                    {
                        Link nl;
                        if (editingInput)
                            nl = Link {nodeIndex,
                                       closestConn,
                                       editingNodeIndex,
                                       editingSlotIndex};
                        else
                            nl = Link {editingNodeIndex,
                                       editingSlotIndex,
                                       nodeIndex,
                                       closestConn};

                        if (!delegate.AllowedLink(nl.mOutputNodeIndex,
                                                  nl.mInputNodeIndex))
                        {
                            break;
                        }
                        bool alreadyExisting = false;
                        for (size_t linkIndex = 0; linkIndex < linkCount;
                             linkIndex++)
                        {
                            const auto link = delegate.GetLink(linkIndex);
                            if (!memcmp(&link, &nl, sizeof(Link)))
                            {
                                alreadyExisting = true;
                                break;
                            }
                        }

                        if (!alreadyExisting)
                        {
                            for (unsigned int linkIndex = 0;
                                 linkIndex < linkCount;
                                 linkIndex++)
                            {
                                const auto link = delegate.GetLink(linkIndex);
                                if (link.mOutputNodeIndex == nl.mOutputNodeIndex
                                    && link.mOutputSlotIndex
                                               == nl.mOutputSlotIndex)
                                {
                                    delegate.DelLink(linkIndex);

                                    break;
                                }
                            }

                            delegate.AddLink(nl.mInputNodeIndex,
                                             nl.mInputSlotIndex,
                                             nl.mOutputNodeIndex,
                                             nl.mOutputSlotIndex);
                        }
                    }
                }

                if (nodeOperation == NO_None && io.MouseClicked[0]
                    && !bDrawOnly)
                {
                    nodeOperation = NO_EditingLink;
                    editingInput = i == 0;
                    editingNodeSource = closestPos;
                    editingNodeIndex = nodeIndex;
                    editingSlotIndex = closestConn;
                    if (editingInput)
                    {
                        for (unsigned int linkIndex = 0; linkIndex < linkCount;
                             linkIndex++)
                        {
                            const auto link = delegate.GetLink(linkIndex);
                            if (link.mOutputNodeIndex == nodeIndex
                                && link.mOutputSlotIndex == closestConn)
                            {
                                delegate.DelLink(linkIndex);
                                break;
                            }
                        }
                    }
                }
            }
        }
        return hoverSlot;
    }

    static void DrawGrid(AzDrawList* drawList,
                         AzVec2 windowPos,
                         const ViewState& viewState,
                         const AzVec2 canvasSize,
                         AzU32 gridColor,
                         AzU32 gridColor2,
                         float gridSize)
    {
        float gridSpace = gridSize * viewState.mFactor;
        int divx = static_cast<int>(-viewState.mPosition.x / gridSize);
        int divy = static_cast<int>(-viewState.mPosition.y / gridSize);
        for (float x = fmodf(viewState.mPosition.x * viewState.mFactor,
                             gridSpace);
             x < canvasSize.x;
             x += gridSpace, divx++)
        {
            bool tenth = !(divx % 10);
            drawList->AddLine(AzVec2(x, 0.0f) + windowPos,
                              AzVec2(x, canvasSize.y) + windowPos,
                              tenth ? gridColor2 : gridColor);
        }
        for (float y = fmodf(viewState.mPosition.y * viewState.mFactor,
                             gridSpace);
             y < canvasSize.y;
             y += gridSpace, divy++)
        {
            bool tenth = !(divy % 10);
            drawList->AddLine(AzVec2(0.0f, y) + windowPos,
                              AzVec2(canvasSize.x, y) + windowPos,
                              tenth ? gridColor2 : gridColor);
        }
    }

    static bool DrawNode(AzDrawList* drawList,
                         NodeIndex nodeIndex,
                         const AzVec2 offset,
                         const float factor,
                         Delegate& delegate,
                         bool overInput,
                         const Options& options,
                         const bool inMinimap,
                         const AzRect& viewPort)
    {
        AzIO& io = Az::GetIO();
        const auto node = delegate.GetNode(nodeIndex);
        AZ_ASSERT((node.mRect.GetWidth() != 0.f)
                  && (node.mRect.GetHeight() != 0.f)
                  && "Nodes must have a non-zero rect.");
        const auto nodeTemplate = delegate.GetTemplate(node.mTemplateIndex);
        const AzVec2 nodeRectangleMin = offset + node.mRect.Min * factor;

        const bool old_any_active = Az::IsAnyItemActive();
        Az::SetCursorScreenPos(nodeRectangleMin);
        const AzVec2 nodeSize = node.mRect.GetSize() * factor;

        drawList->ChannelsSetCurrent(1);
        const size_t InputsCount = nodeTemplate.mInputCount;
        const size_t OutputsCount = nodeTemplate.mOutputCount;

        Az::SetCursorScreenPos(nodeRectangleMin);
        float maxHeight = AzMin(viewPort.Max.y, nodeRectangleMin.y + nodeSize.y)
                          - nodeRectangleMin.y;
        float maxWidth = AzMin(viewPort.Max.x, nodeRectangleMin.x + nodeSize.x)
                         - nodeRectangleMin.x;
        Az::InvisibleButton("node", AzVec2(maxWidth, maxHeight));

        bool nodeMovingActive = Az::IsItemActive();

        bool nodeWidgetsActive = (!old_any_active && Az::IsAnyItemActive());
        AzVec2 nodeRectangleMax = nodeRectangleMin + nodeSize;

        bool nodeHovered = false;
        if (Az::IsItemHovered() && nodeOperation == NO_None && !overInput)
        {
            nodeHovered = true;
        }

        if (Az::IsWindowFocused())
        {
            if ((nodeWidgetsActive || nodeMovingActive) && !inMinimap)
            {
                if (!node.mSelected)
                {
                    if (!io.KeyShift)
                    {
                        const auto nodeCount = delegate.GetNodeCount();
                        for (size_t i = 0; i < nodeCount; i++)
                        {
                            delegate.SelectNode(i, false);
                        }
                    }
                    delegate.SelectNode(nodeIndex, true);
                }
            }
        }
        if (nodeMovingActive && io.MouseDown[0] && nodeHovered && !inMinimap)
        {
            if (nodeOperation != NO_MovingNodes)
            {
                nodeOperation = NO_MovingNodes;
            }
        }

        const bool currentSelectedNode = node.mSelected;
        const AzU32 node_bg_color = nodeHovered
                                            ? nodeTemplate.mBackgroundColorOver
                                            : nodeTemplate.mBackgroundColor;

        drawList->AddRect(nodeRectangleMin,
                          nodeRectangleMax,
                          currentSelectedNode ? options.mSelectedNodeBorderColor
                                              : options.mNodeBorderColor,
                          options.mRounding,
                          AzDrawFlags_RoundCornersAll,
                          currentSelectedNode
                                  ? options.mBorderSelectionThickness
                                  : options.mBorderThickness);

        AzVec2 imgPos = nodeRectangleMin + AzVec2(14, 25);
        AzVec2 imgSize = nodeRectangleMax + AzVec2(-5, -5) - imgPos;
        float imgSizeComp = std::min(imgSize.x, imgSize.y);

        drawList->AddRectFilled(nodeRectangleMin,
                                nodeRectangleMax,
                                node_bg_color,
                                options.mRounding);

        AzVec2 imgPosMax = imgPos + AzVec2(imgSizeComp, imgSizeComp);

        drawList->AddRectFilled(nodeRectangleMin,
                                AzVec2(nodeRectangleMax.x,
                                       nodeRectangleMin.y + 20),
                                nodeTemplate.mHeaderColor,
                                options.mRounding);

        drawList->PushClipRect(nodeRectangleMin,
                               AzVec2(nodeRectangleMax.x,
                                      nodeRectangleMin.y + 20),
                               true);
        drawList->AddText(nodeRectangleMin + AzVec2(2, 2),
                          AZ_COL32(0, 0, 0, 255),
                          node.mName);
        drawList->PopClipRect();

        AzRect customDrawRect(nodeRectangleMin
                                      + AzVec2(options.mRounding,
                                               20 + options.mRounding),
                              nodeRectangleMax
                                      - AzVec2(options.mRounding,
                                               options.mRounding));
        if (customDrawRect.Max.y > customDrawRect.Min.y
            && customDrawRect.Max.x > customDrawRect.Min.x)
        {
            delegate.CustomDraw(drawList, customDrawRect, nodeIndex);
        }

        return nodeHovered;
    }

    bool DrawMiniMap(AzDrawList* drawList,
                     Delegate& delegate,
                     ViewState& viewState,
                     const Options& options,
                     const AzVec2 windowPos,
                     const AzVec2 canvasSize)
    {
        if (Distance(options.mMinimap.Min, options.mMinimap.Max) <= FLT_EPSILON)
        {
            return false;
        }

        const size_t nodeCount = delegate.GetNodeCount();

        if (!nodeCount)
        {
            return false;
        }

        AzVec2 min(FLT_MAX, FLT_MAX);
        AzVec2 max(-FLT_MAX, -FLT_MAX);
        const AzVec2 margin(50, 50);
        for (NodeIndex nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
        {
            const Node& node = delegate.GetNode(nodeIndex);
            min = AzMin(min, node.mRect.Min - margin);
            min = AzMin(min, node.mRect.Max + margin);
            max = AzMax(max, node.mRect.Min - margin);
            max = AzMax(max, node.mRect.Max + margin);
        }

        const AzVec2 worldSizeView = canvasSize / viewState.mFactor;
        const AzVec2 viewMin(-viewState.mPosition.x, -viewState.mPosition.y);
        const AzVec2 viewMax = viewMin + worldSizeView;
        min = AzMin(min, viewMin);
        max = AzMax(max, viewMax);
        const AzVec2 nodesSize = max - min;
        const AzVec2 middleWorld = (min + max) * 0.5f;
        const AzVec2 minScreen = windowPos + options.mMinimap.Min * canvasSize;
        const AzVec2 maxScreen = windowPos + options.mMinimap.Max * canvasSize;
        const AzVec2 viewSize = maxScreen - minScreen;
        const AzVec2 middleScreen = (minScreen + maxScreen) * 0.5f;
        const float ratioY = viewSize.y / nodesSize.y;
        const float ratioX = viewSize.x / nodesSize.x;
        const float factor = AzMin(AzMin(ratioY, ratioX), 1.f);

        drawList->AddRectFilled(minScreen,
                                maxScreen,
                                AZ_COL32(30, 30, 30, 200),
                                3,
                                AzDrawFlags_RoundCornersAll);

        for (NodeIndex nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
        {
            const Node& node = delegate.GetNode(nodeIndex);
            const auto nodeTemplate = delegate.GetTemplate(node.mTemplateIndex);

            AzRect rect = node.mRect;
            rect.Min -= middleWorld;
            rect.Min *= factor;
            rect.Min += middleScreen;

            rect.Max -= middleWorld;
            rect.Max *= factor;
            rect.Max += middleScreen;

            drawList->AddRectFilled(rect.Min,
                                    rect.Max,
                                    nodeTemplate.mBackgroundColor,
                                    1,
                                    AzDrawFlags_RoundCornersAll);
            if (node.mSelected)
            {
                drawList->AddRect(rect.Min,
                                  rect.Max,
                                  options.mSelectedNodeBorderColor,
                                  1,
                                  AzDrawFlags_RoundCornersAll);
            }
        }

        AzVec2 viewMinScreen = (viewMin - middleWorld) * factor + middleScreen;
        AzVec2 viewMaxScreen = (viewMax - middleWorld) * factor + middleScreen;
        drawList->AddRectFilled(viewMinScreen,
                                viewMaxScreen,
                                AZ_COL32(255, 255, 255, 32),
                                1,
                                AzDrawFlags_RoundCornersAll);
        drawList->AddRect(viewMinScreen,
                          viewMaxScreen,
                          AZ_COL32(255, 255, 255, 128),
                          1,
                          AzDrawFlags_RoundCornersAll);

        AzIO& io = Az::GetIO();
        const bool mouseInMinimap =
                AzRect(minScreen, maxScreen).Contains(io.MousePos);
        if (mouseInMinimap && io.MouseClicked[0])
        {
            const AzVec2 clickedRatio = (io.MousePos - minScreen) / viewSize;
            const AzVec2 worldPosCenter =
                    AzVec2(AzLerp(min.x, max.x, clickedRatio.x),
                           AzLerp(min.y, max.y, clickedRatio.y));

            AzVec2 worldPosViewMin = worldPosCenter - worldSizeView * 0.5;
            AzVec2 worldPosViewMax = worldPosCenter + worldSizeView * 0.5;
            if (worldPosViewMin.x < min.x)
            {
                worldPosViewMin.x = min.x;
                worldPosViewMax.x = worldPosViewMin.x + worldSizeView.x;
            }
            if (worldPosViewMin.y < min.y)
            {
                worldPosViewMin.y = min.y;
                worldPosViewMax.y = worldPosViewMin.y + worldSizeView.y;
            }
            if (worldPosViewMax.x > max.x)
            {
                worldPosViewMax.x = max.x;
                worldPosViewMin.x = worldPosViewMax.x - worldSizeView.x;
            }
            if (worldPosViewMax.y > max.y)
            {
                worldPosViewMax.y = max.y;
                worldPosViewMin.y = worldPosViewMax.y - worldSizeView.y;
            }
            viewState.mPosition = AzVec2(-worldPosViewMin.x,
                                         -worldPosViewMin.y);
        }
        return mouseInMinimap;
    }

    void Show(Delegate& delegate,
              const Options& options,
              ViewState& viewState,
              bool enabled,
              FitOnScreen* fit)
    {
        Az::PushStyleVar(AzStyleVar_ChildBorderSize, 0.f);
        Az::PushStyleVar(AzStyleVar_FramePadding, AzVec2(0.f, 0.f));
        Az::PushStyleVar(AzStyleVar_FrameBorderSize, 0.f);

        const AzVec2 windowPos = Az::GetCursorScreenPos();
        const AzVec2 canvasSize = Az::GetContentRegionAvail();
        const AzVec2 scrollRegionLocalPos(0, 0);

        AzRect regionRect(windowPos, windowPos + canvasSize);

        HandleZoomScroll(regionRect, viewState, options);
        AzVec2 offset = Az::GetCursorScreenPos()
                        + viewState.mPosition * viewState.mFactor;
        captureOffset = viewState.mPosition * viewState.mFactor;

        Az::BeginChild(71711, canvasSize, AzChildFlags_FrameStyle);

        Az::SetCursorPos(windowPos);
        Az::BeginGroup();

        AzIO& io = Az::GetIO();

        Az::PushStyleVar(AzStyleVar_FramePadding, AzVec2(1, 1));
        Az::PushStyleVar(AzStyleVar_WindowPadding, AzVec2(0, 0));
        Az::PushStyleColor(AzCol_ChildBg, AZ_COL32(30, 30, 30, 200));

        AzDrawList* drawList = Az::GetWindowDrawList();
        Az::PushClipRect(regionRect.Min, regionRect.Max, true);
        drawList->AddRectFilled(windowPos,
                                windowPos + canvasSize,
                                options.mBackgroundColor);

        if (options.mRenderGrid)
        {
            DrawGrid(drawList,
                     windowPos,
                     viewState,
                     canvasSize,
                     options.mGridColor,
                     options.mGridColor2,
                     options.mGridSize);
        }

        if (fit && ((*fit == Fit_AllNodes) || (*fit == Fit_SelectedNodes)))
        {
            FitNodes(delegate,
                     viewState,
                     canvasSize,
                     (*fit == Fit_SelectedNodes));
        }

        if (enabled)
        {
            static NodeIndex hoveredNode = -1;

            drawList->ChannelsSplit(3);

            drawList->ChannelsSetCurrent(2);
            const bool inMinimap = DrawMiniMap(drawList,
                                               delegate,
                                               viewState,
                                               options,
                                               windowPos,
                                               canvasSize);

            if (Az::IsWindowFocused())
            {
                drawList->AddRect(regionRect.Min,
                                  regionRect.Max,
                                  options.mFrameFocus,
                                  1.f,
                                  0,
                                  2.f);
            }

            drawList->ChannelsSetCurrent(1);

            DisplayLinks(delegate,
                         drawList,
                         offset,
                         viewState.mFactor,
                         regionRect,
                         hoveredNode,
                         options);

            if (nodeOperation == NO_EditingLink)
            {
                AzVec2 p1 = editingNodeSource;
                AzVec2 p2 = io.MousePos;
                drawList->AddLine(p1, p2, AZ_COL32(200, 200, 200, 255), 3.0f);
            }

            drawList->PushClipRect(regionRect.Min, regionRect.Max, true);
            hoveredNode = -1;

            SlotIndex inputSlotOver = -1;
            SlotIndex outputSlotOver = -1;
            NodeIndex nodeOver = -1;

            const auto nodeCount = delegate.GetNodeCount();
            for (int i = 0; i < 2; i++)
            {
                for (NodeIndex nodeIndex = 0; nodeIndex < nodeCount;
                     nodeIndex++)
                {
                    const auto node = delegate.GetNode(nodeIndex);
                    if (node.mSelected != (i != 0))
                    {
                        continue;
                    }

                    AzRect nodeRect = GetNodeRect(node, viewState.mFactor);
                    nodeRect.Min += offset;
                    nodeRect.Max += offset;
                    if (!regionRect.Overlaps(nodeRect))
                    {
                        continue;
                    }

                    Az::PushID((int)nodeIndex);
                    SlotIndex inputSlot = -1;
                    SlotIndex outputSlot = -1;

                    bool overInput = (!inMinimap)
                                     && HandleConnections(drawList,
                                                          nodeIndex,
                                                          offset,
                                                          viewState.mFactor,
                                                          delegate,
                                                          options,
                                                          false,
                                                          inputSlot,
                                                          outputSlot,
                                                          inMinimap);

                    if (DrawNode(drawList,
                                 nodeIndex,
                                 offset,
                                 viewState.mFactor,
                                 delegate,
                                 overInput,
                                 options,
                                 inMinimap,
                                 regionRect))
                    {
                        hoveredNode = nodeIndex;
                    }

                    HandleConnections(drawList,
                                      nodeIndex,
                                      offset,
                                      viewState.mFactor,
                                      delegate,
                                      options,
                                      true,
                                      inputSlot,
                                      outputSlot,
                                      inMinimap);
                    if (inputSlot != -1 || outputSlot != -1)
                    {
                        inputSlotOver = inputSlot;
                        outputSlotOver = outputSlot;
                        nodeOver = nodeIndex;
                    }

                    Az::PopID();
                }
            }

            drawList->PopClipRect();

            if (nodeOperation == NO_MovingNodes)
            {
                if (Az::IsMouseDragging(0, 1))
                {
                    AzVec2 delta = io.MouseDelta / viewState.mFactor;
                    if (fabsf(delta.x) >= 1.f || fabsf(delta.y) >= 1.f)
                    {
                        delegate.MoveSelectedNodes(delta);
                    }
                }
            }

            drawList->ChannelsSetCurrent(0);

            if (!inMinimap)
            {
                HandleQuadSelection(delegate,
                                    drawList,
                                    offset,
                                    viewState.mFactor,
                                    regionRect,
                                    options);
            }

            drawList->ChannelsMerge();

            if (nodeOperation == NO_PanView)
            {
                if (!io.MouseDown[2])
                {
                    nodeOperation = NO_None;
                }
            } else if (nodeOperation != NO_None && !io.MouseDown[0])
            {
                nodeOperation = NO_None;
            }

            if (!inMinimap && nodeOperation == NO_None
                && regionRect.Contains(io.MousePos) && (Az::IsMouseClicked(1)))
            {
                delegate.RightClick(nodeOver, inputSlotOver, outputSlotOver);
            }

            if (Az::IsWindowHovered() && !Az::IsAnyItemActive()
                && io.MouseClicked[2] && nodeOperation == NO_None)
            {
                nodeOperation = NO_PanView;
            }
            if (nodeOperation == NO_PanView)
            {
                viewState.mPosition += io.MouseDelta / viewState.mFactor;
            }
        }

        Az::PopClipRect();

        Az::PopStyleColor(1);
        Az::PopStyleVar(2);
        Az::EndGroup();
        Az::EndChild();

        Az::PopStyleVar(3);

        if (fit)
        {
            *fit = Fit_None;
        }
    }

    bool EditOptions(Options& options)
    {
        bool updated = false;
        if (Az::CollapsingHeader("Colors", nullptr))
        {
            AzColor backgroundColor(options.mBackgroundColor);
            AzColor gridColor(options.mGridColor);
            AzColor selectedNodeBorderColor(options.mSelectedNodeBorderColor);
            AzColor nodeBorderColor(options.mNodeBorderColor);
            AzColor quadSelection(options.mQuadSelection);
            AzColor quadSelectionBorder(options.mQuadSelectionBorder);
            AzColor defaultSlotColor(options.mDefaultSlotColor);
            AzColor frameFocus(options.mFrameFocus);

            updated |= Az::ColorEdit4("Background", (float*)&backgroundColor);
            updated |= Az::ColorEdit4("Grid", (float*)&gridColor);
            updated |= Az::ColorEdit4("Selected Node Border",
                                      (float*)&selectedNodeBorderColor);
            updated |= Az::ColorEdit4("Node Border", (float*)&nodeBorderColor);
            updated |= Az::ColorEdit4("Quad Selection", (float*)&quadSelection);
            updated |= Az::ColorEdit4("Quad Selection Border",
                                      (float*)&quadSelectionBorder);
            updated |= Az::ColorEdit4("Default Slot",
                                      (float*)&defaultSlotColor);
            updated |= Az::ColorEdit4("Frame when has focus",
                                      (float*)&frameFocus);

            options.mBackgroundColor = backgroundColor;
            options.mGridColor = gridColor;
            options.mSelectedNodeBorderColor = selectedNodeBorderColor;
            options.mNodeBorderColor = nodeBorderColor;
            options.mQuadSelection = quadSelection;
            options.mQuadSelectionBorder = quadSelectionBorder;
            options.mDefaultSlotColor = defaultSlotColor;
            options.mFrameFocus = frameFocus;
        }

        if (Az::CollapsingHeader("Options", nullptr))
        {
            updated |= Az::InputFloat4("Minimap", &options.mMinimap.Min.x);
            updated |= Az::InputFloat("Line Thickness",
                                      &options.mLineThickness);
            updated |= Az::InputFloat("Grid Size", &options.mGridSize);
            updated |= Az::InputFloat("Rounding", &options.mRounding);
            updated |= Az::InputFloat("Zoom Ratio", &options.mZoomRatio);
            updated |= Az::InputFloat("Zoom Lerp Factor",
                                      &options.mZoomLerpFactor);
            updated |= Az::InputFloat("Border Selection Thickness",
                                      &options.mBorderSelectionThickness);
            updated |= Az::InputFloat("Border Thickness",
                                      &options.mBorderThickness);
            updated |= Az::InputFloat("Slot Radius", &options.mNodeSlotRadius);
            updated |= Az::InputFloat("Slot Hover Factor",
                                      &options.mNodeSlotHoverFactor);
            updated |= Az::InputFloat2("Zoom min/max", &options.mMinZoom);
            updated |= Az::InputFloat("Slot Hover Factor", &options.mSnap);

            if (Az::RadioButton("Curved Links", options.mDisplayLinksAsCurves))
            {
                options.mDisplayLinksAsCurves = !options.mDisplayLinksAsCurves;
                updated = true;
            }
            if (Az::RadioButton("Straight Links",
                                !options.mDisplayLinksAsCurves))
            {
                options.mDisplayLinksAsCurves = !options.mDisplayLinksAsCurves;
                updated = true;
            }

            updated |= Az::Checkbox("Allow Quad Selection",
                                    &options.mAllowQuadSelection);
            updated |= Az::Checkbox("Render Grid", &options.mRenderGrid);
            updated |= Az::Checkbox("Draw IO names on hover",
                                    &options.mDrawIONameOnHover);
        }

        return updated;
    }

} // namespace AzGuizmo
