// Copyright (c) 2025 Case Technologies

#pragma once

#include <Az/Az.hpp>
#include <Az/AzInternal.hpp>
#include <stdint.h>
#include <string>
#include <vector>

namespace AzGuizmo
{

    typedef size_t NodeIndex;
    typedef size_t SlotIndex;
    typedef size_t LinkIndex;
    typedef size_t TemplateIndex;

    enum FitOnScreen
    {
        Fit_None,
        Fit_AllNodes,
        Fit_SelectedNodes
    };

    struct Options
    {
        AzRect mMinimap {{0.75f, 0.8f, 0.99f, 0.99f}};
        AzU32 mBackgroundColor {AZ_COL32(40, 40, 40, 255)};
        AzU32 mGridColor {AZ_COL32(0, 0, 0, 60)};
        AzU32 mGridColor2 {AZ_COL32(0, 0, 0, 160)};
        AzU32 mSelectedNodeBorderColor {AZ_COL32(255, 130, 30, 255)};
        AzU32 mNodeBorderColor {AZ_COL32(100, 100, 100, 0)};
        AzU32 mQuadSelection {AZ_COL32(255, 32, 32, 64)};
        AzU32 mQuadSelectionBorder {AZ_COL32(255, 32, 32, 255)};
        AzU32 mDefaultSlotColor {AZ_COL32(128, 128, 128, 255)};
        AzU32 mFrameFocus {AZ_COL32(64, 128, 255, 255)};
        float mLineThickness {5};
        float mGridSize {64.f};
        float mRounding {3.f};
        float mZoomRatio {0.1f};
        float mZoomLerpFactor {0.25f};
        float mBorderSelectionThickness {6.f};
        float mBorderThickness {6.f};
        float mNodeSlotRadius {8.f};
        float mNodeSlotHoverFactor {1.2f};
        float mMinZoom {0.2f}, mMaxZoom {1.1f};
        float mSnap {5.f};
        bool mDisplayLinksAsCurves {true};
        bool mAllowQuadSelection {true};
        bool mRenderGrid {true};
        bool mDrawIONameOnHover {true};
    };

    struct ViewState
    {
        AzVec2 mPosition {0.0f, 0.0f};
        float mFactor {1.0f};
        float mFactorTarget {1.0f};
    };

    struct Template
    {
        AzU32 mHeaderColor;
        AzU32 mBackgroundColor;
        AzU32 mBackgroundColorOver;
        AzU8 mInputCount;
        const char** mInputNames;
        AzU32* mInputColors;
        AzU8 mOutputCount;
        const char** mOutputNames;
        AzU32* mOutputColors;
    };

    struct Node
    {
        const char* mName;
        TemplateIndex mTemplateIndex;
        AzRect mRect;
        bool mSelected {false};
    };

    struct Link
    {
        NodeIndex mInputNodeIndex;
        SlotIndex mInputSlotIndex;
        NodeIndex mOutputNodeIndex;
        SlotIndex mOutputSlotIndex;
    };

    struct Delegate
    {
        virtual bool AllowedLink(NodeIndex from, NodeIndex to) = 0;

        virtual void SelectNode(NodeIndex nodeIndex, bool selected) = 0;
        virtual void MoveSelectedNodes(const AzVec2 delta) = 0;

        virtual void AddLink(NodeIndex inputNodeIndex,
                             SlotIndex inputSlotIndex,
                             NodeIndex outputNodeIndex,
                             SlotIndex outputSlotIndex) = 0;
        virtual void DelLink(LinkIndex linkIndex) = 0;

        virtual void CustomDraw(AzDrawList* drawList,
                                AzRect rectangle,
                                NodeIndex nodeIndex) = 0;

        virtual void RightClick(NodeIndex nodeIndex,
                                SlotIndex slotIndexInput,
                                SlotIndex slotIndexOutput) = 0;

        virtual const size_t GetTemplateCount() = 0;
        virtual const Template GetTemplate(TemplateIndex index) = 0;

        virtual const size_t GetNodeCount() = 0;
        virtual const Node GetNode(NodeIndex index) = 0;

        virtual const size_t GetLinkCount() = 0;
        virtual const Link GetLink(LinkIndex index) = 0;

        virtual ~Delegate() = default;
    };

    void Show(Delegate& delegate,
              const Options& options,
              ViewState& viewState,
              bool enabled,
              FitOnScreen* fit = nullptr);
    void GraphEditorClear();

    bool EditOptions(Options& options);

} // namespace AzGuizmo
