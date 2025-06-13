// Copyright (c) 2025 Case Technologies

#pragma once
#include <cstddef>

struct AzDrawList;
struct AzRect;
namespace AzGuizmo
{
    enum SEQUENCER_OPTIONS
    {
        SEQUENCER_EDIT_NONE = 0,
        SEQUENCER_EDIT_STARTEND = 1 << 1,
        SEQUENCER_CHANGE_FRAME = 1 << 3,
        SEQUENCER_ADD = 1 << 4,
        SEQUENCER_DEL = 1 << 5,
        SEQUENCER_COPYPASTE = 1 << 6,
        SEQUENCER_EDIT_ALL = SEQUENCER_EDIT_STARTEND | SEQUENCER_CHANGE_FRAME
    };

    struct SequenceInterface
    {
        bool focused = false;
        virtual int GetFrameMin() const = 0;
        virtual int GetFrameMax() const = 0;
        virtual int GetItemCount() const = 0;

        virtual void BeginEdit(int)
        {}
        virtual void EndEdit()
        {}
        virtual int GetItemTypeCount() const
        {
            return 0;
        }
        virtual const char* GetItemTypeName(int) const
        {
            return "";
        }
        virtual const char* GetItemLabel(int) const
        {
            return "";
        }
        virtual const char* GetCollapseFmt() const
        {
            return "%d Frames / %d entries";
        }

        virtual void Get(int index,
                         int** start,
                         int** end,
                         int* type,
                         unsigned int* color) = 0;
        virtual void Add(int)
        {}
        virtual void Del(int)
        {}
        virtual void Duplicate(int)
        {}

        virtual void Copy()
        {}
        virtual void Paste()
        {}

        virtual size_t GetCustomHeight(int)
        {
            return 0;
        }
        virtual void DoubleClick(int)
        {}
        virtual void CustomDraw(int,
                                AzDrawList*,
                                const AzRect&,
                                const AzRect&,
                                const AzRect&,
                                const AzRect&)
        {}
        virtual void CustomDrawCompact(int,
                                       AzDrawList*,
                                       const AzRect&,
                                       const AzRect&)
        {}

        virtual ~SequenceInterface() = default;
    };

    bool Sequencer(SequenceInterface* sequence,
                   int* currentFrame,
                   bool* expanded,
                   int* selectedEntry,
                   int* firstFrame,
                   int sequenceOptions);

} // namespace AzGuizmo
