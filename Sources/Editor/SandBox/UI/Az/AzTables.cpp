#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef AZ_DEFINE_MATH_OPERATORS
#define AZ_DEFINE_MATH_OPERATORS
#endif

#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include <Az/AzInternal.hpp>
#include <stdint.h>

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#pragma warning(disable : 4996)
#if defined(_MSC_VER) && _MSC_VER >= 1922
#pragma warning(disable : 5054)
#endif
#pragma warning(disable : 26451)
#pragma warning(disable : 26812)
#endif

#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wenum-enum-conversion"
#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"
#pragma clang diagnostic ignored "-Wswitch-default"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

static const int TABLE_DRAW_CHANNEL_BG0 = 0;
static const int TABLE_DRAW_CHANNEL_BG2_FROZEN = 1;
static const int TABLE_DRAW_CHANNEL_NOCLIP = 2;
static const float TABLE_BORDER_SIZE = 1.0f;
static const float TABLE_RESIZE_SEPARATOR_HALF_THICKNESS = 4.0f;
static const float TABLE_RESIZE_SEPARATOR_FEEDBACK_TAZER = 0.06f;

inline AzTableFlags TableFixFlags(AzTableFlags flags, AzWindow* outer_window)
{
    if ((flags & AzTableFlags_SizingMask_) == 0)
        flags |= ((flags & AzTableFlags_ScrollX)
                  || (outer_window->Flags & AzWindowFlags_AlwaysAutoResize))
                         ? AzTableFlags_SizingFixedFit
                         : AzTableFlags_SizingStretchSame;

    if ((flags & AzTableFlags_SizingMask_) == AzTableFlags_SizingFixedSame)
        flags |= AzTableFlags_NoKeepColumnsVisible;

    if (flags & AzTableFlags_Resizable)
        flags |= AzTableFlags_BordersInnerV;

    if (flags & (AzTableFlags_ScrollX | AzTableFlags_ScrollY))
        flags &= ~(AzTableFlags_NoHostExtendX | AzTableFlags_NoHostExtendY);

    if (flags & AzTableFlags_NoBordersInBodyUntilResize)
        flags &= ~AzTableFlags_NoBordersInBody;

    if ((flags
         & (AzTableFlags_Resizable | AzTableFlags_Hideable
            | AzTableFlags_Reorderable | AzTableFlags_Sortable))
        == 0)
        flags |= AzTableFlags_NoSavedSettings;

    if (outer_window->RootWindow->Flags & AzWindowFlags_NoSavedSettings)
        flags |= AzTableFlags_NoSavedSettings;

    return flags;
}

AzTable* Az::TableFindByID(AzID id)
{
    AzContext& g = *GAz;
    return g.Tables.GetByKey(id);
}

bool Az::BeginTable(const char* str_id,
                    int columns_count,
                    AzTableFlags flags,
                    const AzVec2& outer_size,
                    float inner_width)
{
    AzID id = GetID(str_id);
    return BeginTableEx(str_id,
                        id,
                        columns_count,
                        flags,
                        outer_size,
                        inner_width);
}

bool Az::BeginTableEx(const char* name,
                      AzID id,
                      int columns_count,
                      AzTableFlags flags,
                      const AzVec2& outer_size,
                      float inner_width)
{
    AzContext& g = *GAz;
    AzWindow* outer_window = GetCurrentWindow();
    if (outer_window->SkipItems)
        return false;

    AZ_ASSERT(columns_count > 0 && columns_count < AZ_TABLE_MAX_COLUMNS);
    if (flags & AzTableFlags_ScrollX)
        AZ_ASSERT(inner_width >= 0.0f);

    const bool use_child_window =
            (flags & (AzTableFlags_ScrollX | AzTableFlags_ScrollY)) != 0;
    const AzVec2 avail_size = GetContentRegionAvail();
    const AzVec2 actual_outer_size = AzTrunc(
            CalcItemSize(outer_size,
                         AzMax(avail_size.x, 1.0f),
                         use_child_window ? AzMax(avail_size.y, 1.0f) : 0.0f));
    const AzRect outer_rect(outer_window->DC.CursorPos,
                            outer_window->DC.CursorPos + actual_outer_size);
    const bool outer_window_is_measuring_size = (outer_window->AutoFitFramesX
                                                 > 0)
                                                || (outer_window->AutoFitFramesY
                                                    > 0);
    if (use_child_window && IsClippedEx(outer_rect, 0)
        && !outer_window_is_measuring_size)
    {
        ItemSize(outer_rect);
        ItemAdd(outer_rect, id);
        g.NextWindowData.ClearFlags();
        return false;
    }

    if (g.DebugBreakInTable == id)
        AZ_DEBUG_BREAK();

    AzTable* table = g.Tables.GetOrAddByKey(id);

    const int table_idx = g.Tables.GetIndex(table);
    if (++g.TablesTempDataStacked > g.TablesTempData.Size)
        g.TablesTempData.resize(g.TablesTempDataStacked, AzTableTempData());
    AzTableTempData* temp_data = table->TempData =
            &g.TablesTempData[g.TablesTempDataStacked - 1];
    temp_data->TableIndex = table_idx;
    table->DrawSplitter = &table->TempData->DrawSplitter;
    table->DrawSplitter->Clear();

    table->IsDefaultSizingPolicy = (flags & AzTableFlags_SizingMask_) == 0;
    flags = TableFixFlags(flags, outer_window);

    const int previous_frame_active = table->LastFrameActive;
    const int instance_no = (previous_frame_active != g.FrameCount)
                                    ? 0
                                    : table->InstanceCurrent + 1;
    const AzTableFlags previous_flags = table->Flags;
    table->ID = id;
    table->Flags = flags;
    table->LastFrameActive = g.FrameCount;
    table->OuterWindow = table->InnerWindow = outer_window;
    table->ColumnsCount = columns_count;
    table->IsLayoutLocked = false;
    table->InnerWidth = inner_width;
    table->NavLayer = (AzS8)outer_window->DC.NavLayerCurrent;
    temp_data->UserOuterSize = outer_size;

    AzID instance_id;
    table->InstanceCurrent = (AzS16)instance_no;
    if (instance_no > 0)
    {
        AZ_ASSERT(table->ColumnsCount == columns_count && "BeginTable(): Cannot change columns count mid-frame while preserving same ID");
        if (table->InstanceDataExtra.Size < instance_no)
            table->InstanceDataExtra.push_back(AzTableInstanceData());
        instance_id = GetIDWithSeed(instance_no,
                                    GetIDWithSeed("##Instances", NULL, id));
    } else
    {
        instance_id = id;
    }
    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    table_instance->TableInstanceID = instance_id;

    if (use_child_window)
    {
        AzVec2 override_content_size(FLT_MAX, FLT_MAX);
        if ((flags & AzTableFlags_ScrollX) && !(flags & AzTableFlags_ScrollY))
            override_content_size.y = FLT_MIN;

        if ((flags & AzTableFlags_ScrollX) && inner_width > 0.0f)
            override_content_size.x = inner_width;

        if (override_content_size.x != FLT_MAX
            || override_content_size.y != FLT_MAX)
            SetNextWindowContentSize(AzVec2(override_content_size.x != FLT_MAX
                                                    ? override_content_size.x
                                                    : 0.0f,
                                            override_content_size.y != FLT_MAX
                                                    ? override_content_size.y
                                                    : 0.0f));

        if ((previous_flags & (AzTableFlags_ScrollX | AzTableFlags_ScrollY))
            == 0)
            if ((g.NextWindowData.HasFlags & AzNextWindowDataFlags_HasScroll)
                == 0)
                SetNextWindowScroll(AzVec2(0.0f, 0.0f));

        AzChildFlags child_child_flags = (g.NextWindowData.HasFlags
                                          & AzNextWindowDataFlags_HasChildFlags)
                                                 ? g.NextWindowData.ChildFlags
                                                 : AzChildFlags_None;
        AzWindowFlags child_window_flags =
                (g.NextWindowData.HasFlags
                 & AzNextWindowDataFlags_HasWindowFlags)
                        ? g.NextWindowData.WindowFlags
                        : AzWindowFlags_None;
        if (flags & AzTableFlags_ScrollX)
            child_window_flags |= AzWindowFlags_HorizontalScrollbar;
        BeginChildEx(name,
                     instance_id,
                     outer_rect.GetSize(),
                     child_child_flags,
                     child_window_flags);
        table->InnerWindow = g.CurrentWindow;
        table->WorkRect = table->InnerWindow->WorkRect;
        table->OuterRect = table->InnerWindow->Rect();
        table->InnerRect = table->InnerWindow->InnerRect;
        AZ_ASSERT(table->InnerWindow->WindowPadding.x == 0.0f
                  && table->InnerWindow->WindowPadding.y == 0.0f
                  && table->InnerWindow->WindowBorderSize == 0.0f);

        if (table->InnerWindow->SkipItems && outer_window_is_measuring_size)
            table->InnerWindow->SkipItems = false;

        if (instance_no == 0)
        {
            table->HasScrollbarYPrev = table->HasScrollbarYCurr;
            table->HasScrollbarYCurr = false;
        }
        table->HasScrollbarYCurr |= table->InnerWindow->ScrollbarY;
    } else
    {
        table->WorkRect = table->OuterRect = table->InnerRect = outer_rect;
        table->HasScrollbarYPrev = table->HasScrollbarYCurr = false;
        table->InnerWindow->DC.TreeDepth++;
    }

    PushOverrideID(id);
    if (instance_no > 0)
        PushOverrideID(instance_id);

    AzWindow* inner_window = table->InnerWindow;
    table->HostIndentX = inner_window->DC.Indent.x;
    table->HostClipRect = inner_window->ClipRect;
    table->HostSkipItems = inner_window->SkipItems;
    temp_data->HostBackupWorkRect = inner_window->WorkRect;
    temp_data->HostBackupParentWorkRect = inner_window->ParentWorkRect;
    temp_data->HostBackupColumnsOffset = outer_window->DC.ColumnsOffset;
    temp_data->HostBackupPrevLineSize = inner_window->DC.PrevLineSize;
    temp_data->HostBackupCurrLineSize = inner_window->DC.CurrLineSize;
    temp_data->HostBackupCursorMaxPos = inner_window->DC.CursorMaxPos;
    temp_data->HostBackupItemWidth = outer_window->DC.ItemWidth;
    temp_data->HostBackupItemWidthStackSize =
            outer_window->DC.ItemWidthStack.Size;
    inner_window->DC.PrevLineSize = inner_window->DC.CurrLineSize =
            AzVec2(0.0f, 0.0f);

    if (inner_window != outer_window)
    {
        if (flags & AzTableFlags_BordersOuterV)
        {
            table->HostClipRect.Min.x = AzMin(table->HostClipRect.Min.x
                                                      + TABLE_BORDER_SIZE,
                                              table->HostClipRect.Max.x);
            if (inner_window->DecoOuterSizeX2 == 0.0f)
                table->HostClipRect.Max.x = AzMax(table->HostClipRect.Max.x
                                                          - TABLE_BORDER_SIZE,
                                                  table->HostClipRect.Min.x);
        }
        if (flags & AzTableFlags_BordersOuterH)
        {
            table->HostClipRect.Min.y = AzMin(table->HostClipRect.Min.y
                                                      + TABLE_BORDER_SIZE,
                                              table->HostClipRect.Max.y);
            if (inner_window->DecoOuterSizeY2 == 0.0f)
                table->HostClipRect.Max.y = AzMax(table->HostClipRect.Max.y
                                                          - TABLE_BORDER_SIZE,
                                                  table->HostClipRect.Min.y);
        }
    }

    const bool pad_outer_x = (flags & AzTableFlags_NoPadOuterX) ? false
                             : (flags & AzTableFlags_PadOuterX)
                                     ? true
                                     : (flags & AzTableFlags_BordersOuterV)
                                               != 0;
    const bool pad_inner_x = (flags & AzTableFlags_NoPadInnerX) ? false : true;
    const float inner_spacing_for_border = (flags & AzTableFlags_BordersInnerV)
                                                   ? TABLE_BORDER_SIZE
                                                   : 0.0f;
    const float inner_spacing_explicit =
            (pad_inner_x && (flags & AzTableFlags_BordersInnerV) == 0)
                    ? g.Style.CellPadding.x
                    : 0.0f;
    const float inner_padding_explicit =
            (pad_inner_x && (flags & AzTableFlags_BordersInnerV) != 0)
                    ? g.Style.CellPadding.x
                    : 0.0f;
    table->CellSpacingX1 = inner_spacing_explicit + inner_spacing_for_border;
    table->CellSpacingX2 = inner_spacing_explicit;
    table->CellPaddingX = inner_padding_explicit;

    const float outer_padding_for_border = (flags & AzTableFlags_BordersOuterV)
                                                   ? TABLE_BORDER_SIZE
                                                   : 0.0f;
    const float outer_padding_explicit = pad_outer_x ? g.Style.CellPadding.x
                                                     : 0.0f;
    table->OuterPaddingX = (outer_padding_for_border + outer_padding_explicit)
                           - table->CellPaddingX;

    table->CurrentColumn = -1;
    table->CurrentRow = -1;
    table->RowBgColorCounter = 0;
    table->LastRowFlags = AzTableRowFlags_None;
    table->InnerClipRect = (inner_window == outer_window)
                                   ? table->WorkRect
                                   : inner_window->ClipRect;
    table->InnerClipRect.ClipWith(table->WorkRect);
    table->InnerClipRect.ClipWithFull(table->HostClipRect);
    table->InnerClipRect.Max.y = (flags & AzTableFlags_NoHostExtendY)
                                         ? AzMin(table->InnerClipRect.Max.y,
                                                 inner_window->WorkRect.Max.y)
                                         : table->HostClipRect.Max.y;

    table->RowPosY1 = table->RowPosY2 = table->WorkRect.Min.y;
    table->RowTextBaseline = 0.0f;
    table->RowCellPaddingY = 0.0f;
    table->FreezeRowsRequest = table->FreezeRowsCount = 0;
    table->FreezeColumnsRequest = table->FreezeColumnsCount = 0;
    table->IsUnfrozenRows = true;
    table->DeclColumnsCount = table->AngledHeadersCount = 0;
    if (previous_frame_active + 1 < g.FrameCount)
        table->IsActiveIdInTable = false;
    table->AngledHeadersHeight = 0.0f;
    temp_data->AngledHeadersExtraWidth = 0.0f;

    table->BorderColorStrong = GetColorU32(AzCol_TableBorderStrong);
    table->BorderColorLight = GetColorU32(AzCol_TableBorderLight);

    g.CurrentTable = table;
    outer_window->DC.NavIsScrollPushableX = false;
    outer_window->DC.CurrentTableIdx = table_idx;
    if (inner_window != outer_window)
        inner_window->DC.CurrentTableIdx = table_idx;

    if ((previous_flags & AzTableFlags_Reorderable)
        && (flags & AzTableFlags_Reorderable) == 0)
        table->IsResetDisplayOrderRequest = true;

    if (table_idx >= g.TablesLastTimeActive.Size)
        g.TablesLastTimeActive.resize(table_idx + 1, -1.0f);
    g.TablesLastTimeActive[table_idx] = (float)g.Time;
    temp_data->LastTimeActive = (float)g.Time;
    table->MemoryCompacted = false;

    AzTableColumn* old_columns_to_preserve = NULL;
    void* old_columns_raw_data = NULL;
    const int old_columns_count = table->Columns.size();
    if (old_columns_count != 0 && old_columns_count != columns_count)
    {
        old_columns_to_preserve = table->Columns.Data;
        old_columns_raw_data = table->RawData;
        table->RawData = NULL;
    }
    if (table->RawData == NULL)
    {
        TableBeginInitMemory(table, columns_count);
        table->IsInitializing = table->IsSettingsRequestLoad = true;
    }
    if (table->IsResetAllRequest)
        TableResetSettings(table);
    if (table->IsInitializing)
    {
        table->SettingsOffset = -1;
        table->IsSortSpecsDirty = true;
        table->IsSettingsDirty = true;
        table->InstanceInteracted = -1;
        table->ContextPopupColumn = -1;
        table->ReorderColumn = table->ResizedColumn = table->LastResizedColumn =
                -1;
        table->AutoFitSingleColumn = -1;
        table->HoveredColumnBody = table->HoveredColumnBorder = -1;
        for (int n = 0; n < columns_count; n++)
        {
            AzTableColumn* column = &table->Columns[n];
            if (old_columns_to_preserve && n < old_columns_count)
            {
                *column = old_columns_to_preserve[n];
            } else
            {
                float width_auto = column->WidthAuto;
                *column = AzTableColumn();
                column->WidthAuto = width_auto;
                column->IsPreserveWidthAuto = true;
                column->IsEnabled = column->IsUserEnabled =
                        column->IsUserEnabledNextFrame = true;
            }
            column->DisplayOrder = table->DisplayOrderToIndex[n] =
                    (AzTableColumnIdx)n;
        }
    }
    if (old_columns_raw_data)
        AZ_FREE(old_columns_raw_data);

    if (table->IsSettingsRequestLoad)
        TableLoadSettings(table);

    const float new_ref_scale_unit = g.FontSize;
    if (table->RefScale != 0.0f && table->RefScale != new_ref_scale_unit)
    {
        const float scale_factor = new_ref_scale_unit / table->RefScale;

        for (int n = 0; n < columns_count; n++)
            table->Columns[n].WidthRequest = table->Columns[n].WidthRequest
                                             * scale_factor;
    }
    table->RefScale = new_ref_scale_unit;

    inner_window->SkipItems = true;

    if (table->ColumnsNames.Buf.Size > 0)
        table->ColumnsNames.Buf.resize(0);

    TableBeginApplyRequests(table);

    return true;
}

void Az::TableBeginInitMemory(AzTable* table, int columns_count)
{
    const int columns_bit_array_size = (int)AzBitArrayGetStorageSizeInBytes(
            columns_count);
    AzSpanAllocator<6> span_allocator;
    span_allocator.Reserve(0, columns_count * sizeof(AzTableColumn));
    span_allocator.Reserve(1, columns_count * sizeof(AzTableColumnIdx));
    span_allocator.Reserve(2, columns_count * sizeof(AzTableCellData), 4);
    for (int n = 3; n < 6; n++)
        span_allocator.Reserve(n, columns_bit_array_size);
    table->RawData = AZ_ALLOC(span_allocator.GetArenaSizeInBytes());
    memset(table->RawData, 0, span_allocator.GetArenaSizeInBytes());
    span_allocator.SetArenaBasePtr(table->RawData);
    span_allocator.GetSpan(0, &table->Columns);
    span_allocator.GetSpan(1, &table->DisplayOrderToIndex);
    span_allocator.GetSpan(2, &table->RowCellData);
    table->EnabledMaskByDisplayOrder = (AzU32*)span_allocator.GetSpanPtrBegin(
            3);
    table->EnabledMaskByIndex = (AzU32*)span_allocator.GetSpanPtrBegin(4);
    table->VisibleMaskByIndex = (AzU32*)span_allocator.GetSpanPtrBegin(5);
}

void Az::TableBeginApplyRequests(AzTable* table)
{
    if (table->InstanceCurrent == 0)
    {
        if (table->ResizedColumn != -1
            && table->ResizedColumnNextWidth != FLT_MAX)
            TableSetColumnWidth(table->ResizedColumn,
                                table->ResizedColumnNextWidth);
        table->LastResizedColumn = table->ResizedColumn;
        table->ResizedColumnNextWidth = FLT_MAX;
        table->ResizedColumn = -1;

        if (table->AutoFitSingleColumn != -1)
        {
            TableSetColumnWidth(
                    table->AutoFitSingleColumn,
                    table->Columns[table->AutoFitSingleColumn].WidthAuto);
            table->AutoFitSingleColumn = -1;
        }
    }

    if (table->InstanceCurrent == 0)
    {
        if (table->HeldHeaderColumn == -1 && table->ReorderColumn != -1)
            table->ReorderColumn = -1;
        table->HeldHeaderColumn = -1;
        if (table->ReorderColumn != -1 && table->ReorderColumnDir != 0)
        {
            const int reorder_dir = table->ReorderColumnDir;
            AZ_ASSERT(reorder_dir == -1 || reorder_dir == +1);
            AZ_ASSERT(table->Flags & AzTableFlags_Reorderable);
            AzTableColumn* src_column = &table->Columns[table->ReorderColumn];
            AzTableColumn* dst_column =
                    &table->Columns[(reorder_dir == -1)
                                            ? src_column->PrevEnabledColumn
                                            : src_column->NextEnabledColumn];
            AZ_UNUSED(dst_column);
            const int src_order = src_column->DisplayOrder;
            const int dst_order = dst_column->DisplayOrder;
            src_column->DisplayOrder = (AzTableColumnIdx)dst_order;
            for (int order_n = src_order + reorder_dir;
                 order_n != dst_order + reorder_dir;
                 order_n += reorder_dir)
                table->Columns[table->DisplayOrderToIndex[order_n]]
                        .DisplayOrder -= (AzTableColumnIdx)reorder_dir;
            AZ_ASSERT(dst_column->DisplayOrder == dst_order - reorder_dir);

            for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
                table->DisplayOrderToIndex[table->Columns[column_n]
                                                   .DisplayOrder] =
                        (AzTableColumnIdx)column_n;
            table->ReorderColumnDir = 0;
            table->IsSettingsDirty = true;
        }
    }

    if (table->IsResetDisplayOrderRequest)
    {
        for (int n = 0; n < table->ColumnsCount; n++)
            table->DisplayOrderToIndex[n] = table->Columns[n].DisplayOrder =
                    (AzTableColumnIdx)n;
        table->IsResetDisplayOrderRequest = false;
        table->IsSettingsDirty = true;
    }
}

static void TableSetupColumnFlags(AzTable* table,
                                  AzTableColumn* column,
                                  AzTableColumnFlags flags_in)
{
    AzTableColumnFlags flags = flags_in;

    if ((flags & AzTableColumnFlags_WidthMask_) == 0)
    {
        const AzTableFlags table_sizing_policy = (table->Flags
                                                  & AzTableFlags_SizingMask_);
        if (table_sizing_policy == AzTableFlags_SizingFixedFit
            || table_sizing_policy == AzTableFlags_SizingFixedSame)
            flags |= AzTableColumnFlags_WidthFixed;
        else
            flags |= AzTableColumnFlags_WidthStretch;
    } else
    {
        AZ_ASSERT(AzIsPowerOfTwo(flags & AzTableColumnFlags_WidthMask_));
    }

    if ((table->Flags & AzTableFlags_Resizable) == 0)
        flags |= AzTableColumnFlags_NoResize;

    if ((flags & AzTableColumnFlags_NoSortAscending)
        && (flags & AzTableColumnFlags_NoSortDescending))
        flags |= AzTableColumnFlags_NoSort;

    if ((flags & AzTableColumnFlags_IndentMask_) == 0)
        flags |= (table->Columns.index_from_ptr(column) == 0)
                         ? AzTableColumnFlags_IndentEnable
                         : AzTableColumnFlags_IndentDisable;

    column->Flags = flags | (column->Flags & AzTableColumnFlags_StatusMask_);

    column->SortDirectionsAvailCount = column->SortDirectionsAvailMask =
            column->SortDirectionsAvailList = 0;
    if (table->Flags & AzTableFlags_Sortable)
    {
        int count = 0, mask = 0, list = 0;
        if ((flags & AzTableColumnFlags_PreferSortAscending) != 0
            && (flags & AzTableColumnFlags_NoSortAscending) == 0)
        {
            mask |= 1 << AzSortDirection_Ascending;
            list |= AzSortDirection_Ascending << (count << 1);
            count++;
        }
        if ((flags & AzTableColumnFlags_PreferSortDescending) != 0
            && (flags & AzTableColumnFlags_NoSortDescending) == 0)
        {
            mask |= 1 << AzSortDirection_Descending;
            list |= AzSortDirection_Descending << (count << 1);
            count++;
        }
        if ((flags & AzTableColumnFlags_PreferSortAscending) == 0
            && (flags & AzTableColumnFlags_NoSortAscending) == 0)
        {
            mask |= 1 << AzSortDirection_Ascending;
            list |= AzSortDirection_Ascending << (count << 1);
            count++;
        }
        if ((flags & AzTableColumnFlags_PreferSortDescending) == 0
            && (flags & AzTableColumnFlags_NoSortDescending) == 0)
        {
            mask |= 1 << AzSortDirection_Descending;
            list |= AzSortDirection_Descending << (count << 1);
            count++;
        }
        if ((table->Flags & AzTableFlags_SortTristate) || count == 0)
        {
            mask |= 1 << AzSortDirection_None;
            count++;
        }
        column->SortDirectionsAvailList = (AzU8)list;
        column->SortDirectionsAvailMask = (AzU8)mask;
        column->SortDirectionsAvailCount = (AzU8)count;
        Az::TableFixColumnSortDirection(table, column);
    }
}

void Az::TableUpdateLayout(AzTable* table)
{
    AzContext& g = *GAz;
    AZ_ASSERT(table->IsLayoutLocked == false);

    const AzTableFlags table_sizing_policy = (table->Flags
                                              & AzTableFlags_SizingMask_);
    table->IsDefaultDisplayOrder = true;
    table->ColumnsEnabledCount = 0;
    AzBitArrayClearAllBits(table->EnabledMaskByIndex, table->ColumnsCount);
    AzBitArrayClearAllBits(table->EnabledMaskByDisplayOrder,
                           table->ColumnsCount);
    table->LeftMostEnabledColumn = -1;
    table->MinColumnWidth = AzMax(1.0f, g.Style.FramePadding.x * 1.0f);

    int count_fixed = 0;
    int count_stretch = 0;
    int prev_visible_column_idx = -1;
    bool has_auto_fit_request = false;
    bool has_resizable = false;
    float stretch_sum_width_auto = 0.0f;
    float fixed_max_width_auto = 0.0f;
    for (int order_n = 0; order_n < table->ColumnsCount; order_n++)
    {
        const int column_n = table->DisplayOrderToIndex[order_n];
        if (column_n != order_n)
            table->IsDefaultDisplayOrder = false;
        AzTableColumn* column = &table->Columns[column_n];

        if (table->DeclColumnsCount <= column_n)
        {
            TableSetupColumnFlags(table, column, AzTableColumnFlags_None);
            column->NameOffset = -1;
            column->UserID = 0;
            column->InitStretchWeightOrWidth = -1.0f;
        }

        if (!(table->Flags & AzTableFlags_Hideable)
            || (column->Flags & AzTableColumnFlags_NoHide))
            column->IsUserEnabledNextFrame = true;
        if (column->IsUserEnabled != column->IsUserEnabledNextFrame)
        {
            column->IsUserEnabled = column->IsUserEnabledNextFrame;
            table->IsSettingsDirty = true;
        }
        column->IsEnabled = column->IsUserEnabled
                            && (column->Flags & AzTableColumnFlags_Disabled)
                                       == 0;

        if (column->SortOrder != -1 && !column->IsEnabled)
            table->IsSortSpecsDirty = true;
        if (column->SortOrder > 0 && !(table->Flags & AzTableFlags_SortMulti))
            table->IsSortSpecsDirty = true;

        const bool start_auto_fit = (column->Flags
                                     & AzTableColumnFlags_WidthFixed)
                                            ? (column->WidthRequest < 0.0f)
                                            : (column->StretchWeight < 0.0f);
        if (start_auto_fit)
            column->AutoFitQueue = column->CannotSkipItemsQueue = (1 << 3) - 1;

        if (!column->IsEnabled)
        {
            column->IndexWithinEnabledSet = -1;
            continue;
        }

        column->PrevEnabledColumn = (AzTableColumnIdx)prev_visible_column_idx;
        column->NextEnabledColumn = -1;
        if (prev_visible_column_idx != -1)
            table->Columns[prev_visible_column_idx].NextEnabledColumn =
                    (AzTableColumnIdx)column_n;
        else
            table->LeftMostEnabledColumn = (AzTableColumnIdx)column_n;
        column->IndexWithinEnabledSet = table->ColumnsEnabledCount++;
        AzBitArraySetBit(table->EnabledMaskByIndex, column_n);
        AzBitArraySetBit(table->EnabledMaskByDisplayOrder,
                         column->DisplayOrder);
        prev_visible_column_idx = column_n;
        AZ_ASSERT(column->IndexWithinEnabledSet <= column->DisplayOrder);

        if (!column->IsPreserveWidthAuto && table->InstanceCurrent == 0)
            column->WidthAuto = TableGetColumnWidthAuto(table, column);

        const bool column_is_resizable = (column->Flags
                                          & AzTableColumnFlags_NoResize)
                                         == 0;
        if (column_is_resizable)
            has_resizable = true;
        if ((column->Flags & AzTableColumnFlags_WidthFixed)
            && column->InitStretchWeightOrWidth > 0.0f && !column_is_resizable)
            column->WidthAuto = column->InitStretchWeightOrWidth;

        if (column->AutoFitQueue != 0x00)
            has_auto_fit_request = true;
        if (column->Flags & AzTableColumnFlags_WidthStretch)
        {
            stretch_sum_width_auto += column->WidthAuto;
            count_stretch++;
        } else
        {
            fixed_max_width_auto = AzMax(fixed_max_width_auto,
                                         column->WidthAuto);
            count_fixed++;
        }
    }
    if ((table->Flags & AzTableFlags_Sortable) && table->SortSpecsCount == 0
        && !(table->Flags & AzTableFlags_SortTristate))
        table->IsSortSpecsDirty = true;
    table->RightMostEnabledColumn = (AzTableColumnIdx)prev_visible_column_idx;
    AZ_ASSERT(table->LeftMostEnabledColumn >= 0
              && table->RightMostEnabledColumn >= 0);

    if (has_auto_fit_request && table->OuterWindow != table->InnerWindow)
        table->InnerWindow->SkipItems = false;
    if (has_auto_fit_request)
        table->IsSettingsDirty = true;

    float sum_width_requests = 0.0f;
    float stretch_sum_weights = 0.0f;
    table->LeftMostStretchedColumn = table->RightMostStretchedColumn = -1;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        if (!AZ_BITARRAY_TESTBIT(table->EnabledMaskByIndex, column_n))
            continue;
        AzTableColumn* column = &table->Columns[column_n];

        const bool column_is_resizable = (column->Flags
                                          & AzTableColumnFlags_NoResize)
                                         == 0;
        if (column->Flags & AzTableColumnFlags_WidthFixed)
        {
            float width_auto = column->WidthAuto;
            if (table_sizing_policy == AzTableFlags_SizingFixedSame
                && (column->AutoFitQueue != 0x00 || !column_is_resizable))
                width_auto = fixed_max_width_auto;

            if (column->AutoFitQueue != 0x00)
                column->WidthRequest = width_auto;
            else if ((column->Flags & AzTableColumnFlags_WidthFixed)
                     && !column_is_resizable && column->IsRequestOutput)
                column->WidthRequest = width_auto;

            if (column->AutoFitQueue > 0x01 && table->IsInitializing
                && !column->IsPreserveWidthAuto)
                column->WidthRequest = AzMax(column->WidthRequest,
                                             table->MinColumnWidth * 4.0f);
            sum_width_requests += column->WidthRequest;
        } else
        {
            if (column->AutoFitQueue != 0x00 || column->StretchWeight < 0.0f
                || !column_is_resizable)
            {
                if (column->InitStretchWeightOrWidth > 0.0f)
                    column->StretchWeight = column->InitStretchWeightOrWidth;
                else if (table_sizing_policy == AzTableFlags_SizingStretchProp)
                    column->StretchWeight = (column->WidthAuto
                                             / stretch_sum_width_auto)
                                            * count_stretch;
                else
                    column->StretchWeight = 1.0f;
            }

            stretch_sum_weights += column->StretchWeight;
            if (table->LeftMostStretchedColumn == -1
                || table->Columns[table->LeftMostStretchedColumn].DisplayOrder
                           > column->DisplayOrder)
                table->LeftMostStretchedColumn = (AzTableColumnIdx)column_n;
            if (table->RightMostStretchedColumn == -1
                || table->Columns[table->RightMostStretchedColumn].DisplayOrder
                           < column->DisplayOrder)
                table->RightMostStretchedColumn = (AzTableColumnIdx)column_n;
        }
        column->IsPreserveWidthAuto = false;
        sum_width_requests += table->CellPaddingX * 2.0f;
    }
    table->ColumnsEnabledFixedCount = (AzTableColumnIdx)count_fixed;
    table->ColumnsStretchSumWeights = stretch_sum_weights;

    const AzRect work_rect = table->WorkRect;
    const float width_spacings = (table->OuterPaddingX * 2.0f)
                                 + (table->CellSpacingX1 + table->CellSpacingX2)
                                           * (table->ColumnsEnabledCount - 1);
    const float width_removed = (table->HasScrollbarYPrev
                                 && !table->InnerWindow->ScrollbarY)
                                        ? g.Style.ScrollbarSize
                                        : 0.0f;
    const float width_avail = AzMax(1.0f,
                                    (((table->Flags & AzTableFlags_ScrollX)
                                      && table->InnerWidth == 0.0f)
                                             ? table->InnerClipRect.GetWidth()
                                             : work_rect.GetWidth())
                                            - width_removed);
    const float width_avail_for_stretched_columns = width_avail - width_spacings
                                                    - sum_width_requests;
    float width_remaining_for_stretched_columns =
            width_avail_for_stretched_columns;
    table->ColumnsGivenWidth = width_spacings
                               + (table->CellPaddingX * 2.0f)
                                         * table->ColumnsEnabledCount;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        if (!AZ_BITARRAY_TESTBIT(table->EnabledMaskByIndex, column_n))
            continue;
        AzTableColumn* column = &table->Columns[column_n];

        if (column->Flags & AzTableColumnFlags_WidthStretch)
        {
            float weight_ratio = column->StretchWeight / stretch_sum_weights;
            column->WidthRequest = AZ_TRUNC(
                    AzMax(width_avail_for_stretched_columns * weight_ratio,
                          table->MinColumnWidth)
                    + 0.01f);
            width_remaining_for_stretched_columns -= column->WidthRequest;
        }

        if (column->NextEnabledColumn == -1
            && table->LeftMostStretchedColumn != -1)
            column->Flags |= AzTableColumnFlags_NoDirectResize_;

        column->WidthGiven = AzTrunc(
                AzMax(column->WidthRequest, table->MinColumnWidth));
        table->ColumnsGivenWidth += column->WidthGiven;
    }

    if (width_remaining_for_stretched_columns >= 1.0f
        && !(table->Flags & AzTableFlags_PreciseWidths))
        for (int order_n = table->ColumnsCount - 1;
             stretch_sum_weights > 0.0f
             && width_remaining_for_stretched_columns >= 1.0f && order_n >= 0;
             order_n--)
        {
            if (!AZ_BITARRAY_TESTBIT(table->EnabledMaskByDisplayOrder, order_n))
                continue;
            AzTableColumn* column =
                    &table->Columns[table->DisplayOrderToIndex[order_n]];
            if (!(column->Flags & AzTableColumnFlags_WidthStretch))
                continue;
            column->WidthRequest += 1.0f;
            column->WidthGiven += 1.0f;
            width_remaining_for_stretched_columns -= 1.0f;
        }

    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    table_instance->HoveredRowLast = table_instance->HoveredRowNext;
    table_instance->HoveredRowNext = -1;
    table->HoveredColumnBody = table->HoveredColumnBorder = -1;
    const AzRect mouse_hit_rect(
            table->OuterRect.Min.x,
            table->OuterRect.Min.y,
            table->OuterRect.Max.x,
            AzMax(table->OuterRect.Max.y,
                  table->OuterRect.Min.y + table_instance->LastOuterHeight));
    const AzID backup_active_id = g.ActiveId;
    g.ActiveId = 0;
    const bool is_hovering_table = ItemHoverable(mouse_hit_rect,
                                                 0,
                                                 AzItemFlags_None);
    g.ActiveId = backup_active_id;

    float mouse_skewed_x = g.IO.MousePos.x;
    if (table->AngledHeadersHeight > 0.0f)
        if (g.IO.MousePos.y >= table->OuterRect.Min.y
            && g.IO.MousePos.y
                       <= table->OuterRect.Min.y + table->AngledHeadersHeight)
            mouse_skewed_x += AzTrunc((table->OuterRect.Min.y
                                       + table->AngledHeadersHeight
                                       - g.IO.MousePos.y)
                                      * table->AngledHeadersSlope);

    int visible_n = 0;
    bool has_at_least_one_column_requesting_output = false;
    bool offset_x_frozen = (table->FreezeColumnsCount > 0);
    float offset_x = ((table->FreezeColumnsCount > 0) ? table->OuterRect.Min.x
                                                      : work_rect.Min.x)
                     + table->OuterPaddingX - table->CellSpacingX1;
    AzRect host_clip_rect = table->InnerClipRect;

    AzBitArrayClearAllBits(table->VisibleMaskByIndex, table->ColumnsCount);
    for (int order_n = 0; order_n < table->ColumnsCount; order_n++)
    {
        const int column_n = table->DisplayOrderToIndex[order_n];
        AzTableColumn* column = &table->Columns[column_n];

        column->NavLayerCurrent = (AzS8)(table->FreezeRowsCount > 0
                                                 ? AzNavLayer_Menu
                                                 : (AzNavLayer)table->NavLayer);

        if (offset_x_frozen && table->FreezeColumnsCount == visible_n)
        {
            offset_x += work_rect.Min.x - table->OuterRect.Min.x;
            offset_x_frozen = false;
        }

        column->Flags &= ~AzTableColumnFlags_StatusMask_;

        if (!AZ_BITARRAY_TESTBIT(table->EnabledMaskByDisplayOrder, order_n))
        {
            column->MinX = column->MaxX = column->WorkMinX =
                    column->ClipRect.Min.x = column->ClipRect.Max.x = offset_x;
            column->WidthGiven = 0.0f;
            column->ClipRect.Min.y = work_rect.Min.y;
            column->ClipRect.Max.y = FLT_MAX;
            column->ClipRect.ClipWithFull(host_clip_rect);
            column->IsVisibleX = column->IsVisibleY = column->IsRequestOutput =
                    false;
            column->IsSkipItems = true;
            column->ItemWidth = 1.0f;
            continue;
        }

        column->MinX = offset_x;

        column->WidthMax = TableCalcMaxColumnWidth(table, column_n);
        column->WidthGiven = AzMin(column->WidthGiven, column->WidthMax);
        column->WidthGiven = AzMax(column->WidthGiven,
                                   AzMin(column->WidthRequest,
                                         table->MinColumnWidth));
        column->MaxX = offset_x + column->WidthGiven + table->CellSpacingX1
                       + table->CellSpacingX2 + table->CellPaddingX * 2.0f;

        const float previous_instance_work_min_x = column->WorkMinX;
        column->WorkMinX = column->MinX + table->CellPaddingX
                           + table->CellSpacingX1;
        column->WorkMaxX = column->MaxX - table->CellPaddingX
                           - table->CellSpacingX2;
        column->ItemWidth = AzTrunc(column->WidthGiven * 0.65f);
        column->ClipRect.Min.x = column->MinX;
        column->ClipRect.Min.y = work_rect.Min.y;
        column->ClipRect.Max.x = column->MaxX;
        column->ClipRect.Max.y = FLT_MAX;
        column->ClipRect.ClipWithFull(host_clip_rect);

        column->IsVisibleX = (column->ClipRect.Max.x > column->ClipRect.Min.x);
        column->IsVisibleY = true;
        const bool is_visible = column->IsVisibleX;
        if (is_visible)
            AzBitArraySetBit(table->VisibleMaskByIndex, column_n);

        column->IsRequestOutput = is_visible || column->AutoFitQueue != 0
                                  || column->CannotSkipItemsQueue != 0;

        column->IsSkipItems = !column->IsEnabled || table->HostSkipItems;
        if (column->IsSkipItems)
            AZ_ASSERT(!is_visible);
        if (column->IsRequestOutput && !column->IsSkipItems)
            has_at_least_one_column_requesting_output = true;

        column->Flags |= AzTableColumnFlags_IsEnabled;
        if (is_visible)
            column->Flags |= AzTableColumnFlags_IsVisible;
        if (column->SortOrder != -1)
            column->Flags |= AzTableColumnFlags_IsSorted;

        if (is_hovering_table && mouse_skewed_x >= column->ClipRect.Min.x
            && mouse_skewed_x < column->ClipRect.Max.x)
        {
            column->Flags |= AzTableColumnFlags_IsHovered;
            table->HoveredColumnBody = (AzTableColumnIdx)column_n;
        }

        if (table->InstanceCurrent == 0)
        {
            column->ContentMaxXFrozen = column->WorkMinX;
            column->ContentMaxXUnfrozen = column->WorkMinX;
            column->ContentMaxXHeadersUsed = column->WorkMinX;
            column->ContentMaxXHeadersIdeal = column->WorkMinX;
        } else
        {
            const float offset_from_previous_instance =
                    column->WorkMinX - previous_instance_work_min_x;
            column->ContentMaxXFrozen += offset_from_previous_instance;
            column->ContentMaxXUnfrozen += offset_from_previous_instance;
            column->ContentMaxXHeadersUsed += offset_from_previous_instance;
            column->ContentMaxXHeadersIdeal += offset_from_previous_instance;
        }

        if (table->HostSkipItems == false && table->InstanceCurrent == 0)
        {
            column->AutoFitQueue >>= 1;
            column->CannotSkipItemsQueue >>= 1;
        }

        if (visible_n < table->FreezeColumnsCount)
            host_clip_rect.Min.x = AzClamp(column->MaxX + TABLE_BORDER_SIZE,
                                           host_clip_rect.Min.x,
                                           host_clip_rect.Max.x);

        offset_x += column->WidthGiven + table->CellSpacingX1
                    + table->CellSpacingX2 + table->CellPaddingX * 2.0f;
        visible_n++;
    }

    if (has_at_least_one_column_requesting_output == false)
    {
        table->Columns[table->LeftMostEnabledColumn].IsRequestOutput = true;
        table->Columns[table->LeftMostEnabledColumn].IsSkipItems = false;
    }

    const float unused_x1 =
            AzMax(table->WorkRect.Min.x,
                  table->Columns[table->RightMostEnabledColumn].ClipRect.Max.x);
    if (is_hovering_table && table->HoveredColumnBody == -1)
        if (mouse_skewed_x >= unused_x1)
            table->HoveredColumnBody = (AzTableColumnIdx)table->ColumnsCount;
    if (has_resizable == false && (table->Flags & AzTableFlags_Resizable))
        table->Flags &= ~AzTableFlags_Resizable;

    table->IsActiveIdAliveBeforeTable = (g.ActiveIdIsAlive != 0);

    if (table->RightMostStretchedColumn != -1)
        table->Flags &= ~AzTableFlags_NoHostExtendX;
    if (table->Flags & AzTableFlags_NoHostExtendX)
    {
        table->OuterRect.Max.x = table->WorkRect.Max.x = unused_x1;
        table->InnerClipRect.Max.x = AzMin(table->InnerClipRect.Max.x,
                                           unused_x1);
    }
    table->InnerWindow->ParentWorkRect = table->WorkRect;
    table->BorderX1 = table->InnerClipRect.Min.x;
    table->BorderX2 = table->InnerClipRect.Max.x;

    float window_content_max_y;
    if (table->Flags & AzTableFlags_NoHostExtendY)
        window_content_max_y = table->OuterRect.Max.y;
    else
        window_content_max_y = AzMax(
                table->InnerWindow->ContentRegionRect.Max.y,
                (table->Flags & AzTableFlags_ScrollY) ? 0.0f
                                                      : table->OuterRect.Max.y);
    table->InnerWindow->WorkRect.Max.y =
            AzClamp(window_content_max_y - g.Style.CellPadding.y,
                    table->InnerWindow->WorkRect.Min.y,
                    table->InnerWindow->WorkRect.Max.y);

    TableSetupDrawChannels(table);

    if (table->Flags & AzTableFlags_Resizable)
        TableUpdateBorders(table);
    table_instance->LastTopHeadersRowHeight = 0.0f;
    table->IsLayoutLocked = true;
    table->IsUsingHeaders = false;

    table->HighlightColumnHeader = -1;
    if (table->IsContextPopupOpen && table->ContextPopupColumn != -1
        && table->InstanceInteracted == table->InstanceCurrent)
        table->HighlightColumnHeader = table->ContextPopupColumn;
    else if ((table->Flags & AzTableFlags_HighlightHoveredColumn)
             && table->HoveredColumnBody != -1
             && table->HoveredColumnBody != table->ColumnsCount
             && table->HoveredColumnBorder == -1)
        if (g.ActiveId == 0 || (table->IsActiveIdInTable || g.DragDropActive))
            table->HighlightColumnHeader = table->HoveredColumnBody;

    if (table->DisableDefaultContextMenu == false
        && TableBeginContextMenuPopup(table))
    {
        TableDrawDefaultContextMenu(table, table->Flags);
        EndPopup();
    }

    if (table->IsSortSpecsDirty && (table->Flags & AzTableFlags_Sortable))
        TableSortSpecsBuild(table);

    if (table->FreezeColumnsRequest > 0)
        table->InnerWindow->DecoInnerSizeX1 =
                table->Columns[table->DisplayOrderToIndex
                                       [table->FreezeColumnsRequest - 1]]
                        .MaxX
                - table->OuterRect.Min.x;
    if (table->FreezeRowsRequest > 0)
        table->InnerWindow->DecoInnerSizeY1 = table_instance->LastFrozenHeight;
    table_instance->LastFrozenHeight = 0.0f;

    AzWindow* inner_window = table->InnerWindow;
    if (table->Flags & AzTableFlags_NoClip)
        table->DrawSplitter->SetCurrentChannel(inner_window->DrawList,
                                               TABLE_DRAW_CHANNEL_NOCLIP);
    else
        inner_window->DrawList->PushClipRect(inner_window->InnerClipRect.Min,
                                             inner_window->InnerClipRect.Max,
                                             false);
}

void Az::TableUpdateBorders(AzTable* table)
{
    AzContext& g = *GAz;
    AZ_ASSERT(table->Flags & AzTableFlags_Resizable);

    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    const float hit_half_width = AzTrunc(TABLE_RESIZE_SEPARATOR_HALF_THICKNESS
                                         * g.CurrentDpiScale);
    const float hit_y1 = (table->FreezeRowsCount >= 1 ? table->OuterRect.Min.y
                                                      : table->WorkRect.Min.y)
                         + table->AngledHeadersHeight;
    const float hit_y2_body = AzMax(table->OuterRect.Max.y,
                                    hit_y1 + table_instance->LastOuterHeight
                                            - table->AngledHeadersHeight);
    const float hit_y2_head = hit_y1 + table_instance->LastTopHeadersRowHeight;

    for (int order_n = 0; order_n < table->ColumnsCount; order_n++)
    {
        if (!AZ_BITARRAY_TESTBIT(table->EnabledMaskByDisplayOrder, order_n))
            continue;

        const int column_n = table->DisplayOrderToIndex[order_n];
        AzTableColumn* column = &table->Columns[column_n];
        if (column->Flags
            & (AzTableColumnFlags_NoResize
               | AzTableColumnFlags_NoDirectResize_))
            continue;

        const float border_y2_hit = (table->Flags
                                     & AzTableFlags_NoBordersInBody)
                                            ? hit_y2_head
                                            : hit_y2_body;
        if ((table->Flags & AzTableFlags_NoBordersInBody)
            && table->IsUsingHeaders == false)
            continue;

        if (!column->IsVisibleX && table->LastResizedColumn != column_n)
            continue;

        AzID column_id = TableGetColumnResizeID(table,
                                                column_n,
                                                table->InstanceCurrent);
        AzRect hit_rect(column->MaxX - hit_half_width,
                        hit_y1,
                        column->MaxX + hit_half_width,
                        border_y2_hit);
        ItemAdd(hit_rect, column_id, NULL, AzItemFlags_NoNav);

        bool hovered = false, held = false;
        bool pressed = ButtonBehavior(
                hit_rect,
                column_id,
                &hovered,
                &held,
                AzButtonFlags_FlattenChildren | AzButtonFlags_PressedOnClick
                        | AzButtonFlags_PressedOnDoubleClick
                        | AzButtonFlags_NoNavFocus);
        if (pressed && IsMouseDoubleClicked(0))
        {
            TableSetColumnWidthAutoSingle(table, column_n);
            ClearActiveID();
            held = false;
        }
        if (held)
        {
            if (table->LastResizedColumn == -1)
                table->ResizeLockMinContentsX2 =
                        table->RightMostEnabledColumn != -1
                                ? table->Columns[table->RightMostEnabledColumn]
                                          .MaxX
                                : -FLT_MAX;
            table->ResizedColumn = (AzTableColumnIdx)column_n;
            table->InstanceInteracted = table->InstanceCurrent;
        }
        if ((hovered
             && g.HoveredIdTimer > TABLE_RESIZE_SEPARATOR_FEEDBACK_TAZER)
            || held)
        {
            table->HoveredColumnBorder = (AzTableColumnIdx)column_n;
            SetMouseCursor(AzMouseCursor_ResizeEW);
        }
    }
}

void Az::EndTable()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(table != NULL,
                             "EndTable() call should only be done while in "
                             "BeginTable() scope!");
        return;
    }

    if (!table->IsLayoutLocked)
        TableUpdateLayout(table);

    const AzTableFlags flags = table->Flags;
    AzWindow* inner_window = table->InnerWindow;
    AzWindow* outer_window = table->OuterWindow;
    AzTableTempData* temp_data = table->TempData;
    AZ_ASSERT(inner_window == g.CurrentWindow);
    AZ_ASSERT(outer_window == inner_window
              || outer_window == inner_window->ParentWindow);

    if (table->IsInsideRow)
        TableEndRow(table);

    if (flags & AzTableFlags_ContextMenuInBody)
        if (table->HoveredColumnBody != -1 && !IsAnyItemHovered()
            && IsMouseReleased(AzMouseButton_Right))
            TableOpenContextMenu((int)table->HoveredColumnBody);

    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    inner_window->DC.PrevLineSize = temp_data->HostBackupPrevLineSize;
    inner_window->DC.CurrLineSize = temp_data->HostBackupCurrLineSize;
    inner_window->DC.CursorMaxPos = temp_data->HostBackupCursorMaxPos;
    const float inner_content_max_y = table->RowPosY2;
    AZ_ASSERT(table->RowPosY2 == inner_window->DC.CursorPos.y);
    if (inner_window != outer_window)
        inner_window->DC.CursorMaxPos.y = inner_content_max_y;
    else if (!(flags & AzTableFlags_NoHostExtendY))
        table->OuterRect.Max.y = table->InnerRect.Max.y =
                AzMax(table->OuterRect.Max.y, inner_content_max_y);
    table->WorkRect.Max.y = AzMax(table->WorkRect.Max.y,
                                  table->OuterRect.Max.y);
    table_instance->LastOuterHeight = table->OuterRect.GetHeight();

    if (table->Flags & AzTableFlags_ScrollX)
    {
        const float outer_padding_for_border = (table->Flags
                                                & AzTableFlags_BordersOuterV)
                                                       ? TABLE_BORDER_SIZE
                                                       : 0.0f;
        float max_pos_x = table->InnerWindow->DC.CursorMaxPos.x;
        if (table->RightMostEnabledColumn != -1)
            max_pos_x =
                    AzMax(max_pos_x,
                          table->Columns[table->RightMostEnabledColumn].WorkMaxX
                                  + table->CellPaddingX + table->OuterPaddingX
                                  - outer_padding_for_border);
        if (table->ResizedColumn != -1)
            max_pos_x = AzMax(max_pos_x, table->ResizeLockMinContentsX2);
        table->InnerWindow->DC.CursorMaxPos.x =
                max_pos_x + table->TempData->AngledHeadersExtraWidth;
    }

    if (!(flags & AzTableFlags_NoClip))
        inner_window->DrawList->PopClipRect();
    inner_window->ClipRect = inner_window->DrawList->_ClipRectStack.back();

    if ((flags & AzTableFlags_Borders) != 0)
        TableDrawBorders(table);

#if 0

    if (table->DummyDrawChannel != (AzTableColumnIdx)-1)
    {
        AzDrawChannel* dummy_channel = &table->DrawSplitter._Channels[table->DummyDrawChannel];
        dummy_channel->_CmdBuffer.resize(0);
        dummy_channel->_IdxBuffer.resize(0);
    }
#endif

    AzDrawListSplitter* splitter = table->DrawSplitter;
    splitter->SetCurrentChannel(inner_window->DrawList, 0);
    if ((table->Flags & AzTableFlags_NoClip) == 0)
        TableMergeDrawChannels(table);
    splitter->Merge(inner_window->DrawList);

    float auto_fit_width_for_fixed = 0.0f;
    float auto_fit_width_for_stretched = 0.0f;
    float auto_fit_width_for_stretched_min = 0.0f;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
        if (AZ_BITARRAY_TESTBIT(table->EnabledMaskByIndex, column_n))
        {
            AzTableColumn* column = &table->Columns[column_n];
            float column_width_request =
                    ((column->Flags & AzTableColumnFlags_WidthFixed)
                     && !(column->Flags & AzTableColumnFlags_NoResize))
                            ? column->WidthRequest
                            : TableGetColumnWidthAuto(table, column);
            if (column->Flags & AzTableColumnFlags_WidthFixed)
                auto_fit_width_for_fixed += column_width_request;
            else
                auto_fit_width_for_stretched += column_width_request;
            if ((column->Flags & AzTableColumnFlags_WidthStretch)
                && (column->Flags & AzTableColumnFlags_NoResize) != 0)
                auto_fit_width_for_stretched_min =
                        AzMax(auto_fit_width_for_stretched_min,
                              column_width_request
                                      / (column->StretchWeight
                                         / table->ColumnsStretchSumWeights));
        }
    const float width_spacings = (table->OuterPaddingX * 2.0f)
                                 + (table->CellSpacingX1 + table->CellSpacingX2)
                                           * (table->ColumnsEnabledCount - 1);
    table->ColumnsAutoFitWidth = width_spacings
                                 + (table->CellPaddingX * 2.0f)
                                           * table->ColumnsEnabledCount
                                 + auto_fit_width_for_fixed
                                 + AzMax(auto_fit_width_for_stretched,
                                         auto_fit_width_for_stretched_min);

    if ((table->Flags & AzTableFlags_ScrollX) == 0
        && inner_window != outer_window)
    {
        inner_window->Scroll.x = 0.0f;
    } else if (table->LastResizedColumn != -1 && table->ResizedColumn == -1
               && inner_window->ScrollbarX
               && table->InstanceInteracted == table->InstanceCurrent)
    {
        const float neighbor_width_to_keep_visible = table->MinColumnWidth
                                                     + table->CellPaddingX
                                                               * 2.0f;
        AzTableColumn* column = &table->Columns[table->LastResizedColumn];
        if (column->MaxX < table->InnerClipRect.Min.x)
            SetScrollFromPosX(inner_window,
                              column->MaxX - inner_window->Pos.x
                                      - neighbor_width_to_keep_visible,
                              1.0f);
        else if (column->MaxX > table->InnerClipRect.Max.x)
            SetScrollFromPosX(inner_window,
                              column->MaxX - inner_window->Pos.x
                                      + neighbor_width_to_keep_visible,
                              1.0f);
    }

    if (table->ResizedColumn != -1
        && table->InstanceCurrent == table->InstanceInteracted)
    {
        AzTableColumn* column = &table->Columns[table->ResizedColumn];
        const float new_x2 = (g.IO.MousePos.x - g.ActiveIdClickOffset.x
                              + AzTrunc(TABLE_RESIZE_SEPARATOR_HALF_THICKNESS
                                        * g.CurrentDpiScale));
        const float new_width = AzTrunc(new_x2 - column->MinX
                                        - table->CellSpacingX1
                                        - table->CellPaddingX * 2.0f);
        table->ResizedColumnNextWidth = new_width;
    }

    table->IsActiveIdInTable = (g.ActiveIdIsAlive != 0
                                && table->IsActiveIdAliveBeforeTable == false);

    AZ_ASSERT_USER_ERROR(inner_window->IDStack.back()
                                 == table_instance->TableInstanceID,
                         "Mismatching PushID/PopID!");
    AZ_ASSERT_USER_ERROR(outer_window->DC.ItemWidthStack.Size
                                 >= temp_data->HostBackupItemWidthStackSize,
                         "Too many PopItemWidth!");
    if (table->InstanceCurrent > 0)
        PopID();
    PopID();

    const AzVec2 backup_outer_max_pos = outer_window->DC.CursorMaxPos;
    inner_window->WorkRect = temp_data->HostBackupWorkRect;
    inner_window->ParentWorkRect = temp_data->HostBackupParentWorkRect;
    inner_window->SkipItems = table->HostSkipItems;
    outer_window->DC.CursorPos = table->OuterRect.Min;
    outer_window->DC.ItemWidth = temp_data->HostBackupItemWidth;
    outer_window->DC.ItemWidthStack.Size =
            temp_data->HostBackupItemWidthStackSize;
    outer_window->DC.ColumnsOffset = temp_data->HostBackupColumnsOffset;

    if (inner_window != outer_window)
    {
        short backup_nav_layers_active_mask =
                inner_window->DC.NavLayersActiveMask;
        inner_window->DC.NavLayersActiveMask |= 1 << table->NavLayer;
        g.CurrentTable = NULL;
        EndChild();
        g.CurrentTable = table;
        inner_window->DC.NavLayersActiveMask = backup_nav_layers_active_mask;
    } else
    {
        table->InnerWindow->DC.TreeDepth--;
        ItemSize(table->OuterRect.GetSize());
        ItemAdd(table->OuterRect, 0);
    }

    if (table->Flags & AzTableFlags_NoHostExtendX)
    {
        AZ_ASSERT((table->Flags & AzTableFlags_ScrollX) == 0);
        outer_window->DC.CursorMaxPos.x =
                AzMax(backup_outer_max_pos.x,
                      table->OuterRect.Min.x + table->ColumnsAutoFitWidth);
    } else if (temp_data->UserOuterSize.x <= 0.0f)
    {
        const float inner_content_max_x = table->OuterRect.Min.x
                                          + table->ColumnsAutoFitWidth;
        const float decoration_size =
                table->TempData->AngledHeadersExtraWidth
                + ((table->Flags & AzTableFlags_ScrollY)
                           ? inner_window->ScrollbarSizes.x
                           : 0.0f);
        outer_window->DC.IdealMaxPos.x =
                AzMax(outer_window->DC.IdealMaxPos.x,
                      inner_content_max_x + decoration_size
                              - temp_data->UserOuterSize.x);
        outer_window->DC.CursorMaxPos.x =
                AzMax(backup_outer_max_pos.x,
                      AzMin(table->OuterRect.Max.x,
                            inner_content_max_x + decoration_size));
    } else
    {
        outer_window->DC.CursorMaxPos.x = AzMax(backup_outer_max_pos.x,
                                                table->OuterRect.Max.x);
    }
    if (temp_data->UserOuterSize.y <= 0.0f)
    {
        const float decoration_size = (table->Flags & AzTableFlags_ScrollX)
                                              ? inner_window->ScrollbarSizes.y
                                              : 0.0f;
        outer_window->DC.IdealMaxPos.y =
                AzMax(outer_window->DC.IdealMaxPos.y,
                      inner_content_max_y + decoration_size
                              - temp_data->UserOuterSize.y);
        outer_window->DC.CursorMaxPos.y =
                AzMax(backup_outer_max_pos.y,
                      AzMin(table->OuterRect.Max.y,
                            inner_content_max_y + decoration_size));
    } else
    {
        outer_window->DC.CursorMaxPos.y = AzMax(backup_outer_max_pos.y,
                                                table->OuterRect.Max.y);
    }

    if (table->IsSettingsDirty)
        TableSaveSettings(table);
    table->IsInitializing = false;

    AZ_ASSERT(g.CurrentWindow == outer_window && g.CurrentTable == table);
    AZ_ASSERT(g.TablesTempDataStacked > 0);
    temp_data = (--g.TablesTempDataStacked > 0)
                        ? &g.TablesTempData[g.TablesTempDataStacked - 1]
                        : NULL;
    g.CurrentTable = temp_data ? g.Tables.GetByIndex(temp_data->TableIndex)
                               : NULL;
    if (g.CurrentTable)
    {
        g.CurrentTable->TempData = temp_data;
        g.CurrentTable->DrawSplitter = &temp_data->DrawSplitter;
    }
    outer_window->DC.CurrentTableIdx = g.CurrentTable ? g.Tables.GetIndex(
                                                                g.CurrentTable)
                                                      : -1;
    NavUpdateCurrentWindowIsScrollPushableX();
}

static void TableInitColumnDefaults(AzTable* table,
                                    AzTableColumn* column,
                                    AzTableColumnFlags init_mask)
{
    AzTableColumnFlags flags = column->Flags;
    if (init_mask & AzTableFlags_Resizable)
    {
        float init_width_or_weight = column->InitStretchWeightOrWidth;
        column->WidthRequest = ((flags & AzTableColumnFlags_WidthFixed)
                                && init_width_or_weight > 0.0f)
                                       ? init_width_or_weight
                                       : -1.0f;
        column->StretchWeight = (init_width_or_weight > 0.0f
                                 && (flags & AzTableColumnFlags_WidthStretch))
                                        ? init_width_or_weight
                                        : -1.0f;
        if (init_width_or_weight > 0.0f)
            column->AutoFitQueue = 0x00;
    }
    if (init_mask & AzTableFlags_Reorderable)
        column->DisplayOrder = (AzTableColumnIdx)table->Columns.index_from_ptr(
                column);
    if (init_mask & AzTableFlags_Hideable)
        column->IsUserEnabled = column->IsUserEnabledNextFrame =
                (flags & AzTableColumnFlags_DefaultHide) ? 0 : 1;
    if (init_mask & AzTableFlags_Sortable)
    {
        column->SortOrder = (flags & AzTableColumnFlags_DefaultSort) ? 0 : -1;
        column->SortDirection =
                (flags & AzTableColumnFlags_DefaultSort)
                        ? ((flags & AzTableColumnFlags_PreferSortDescending)
                                   ? (AzS8)AzSortDirection_Descending
                                   : (AzU8)(AzSortDirection_Ascending))
                        : (AzS8)AzSortDirection_None;
    }
}

void Az::TableSetupColumn(const char* label,
                          AzTableColumnFlags flags,
                          float init_width_or_weight,
                          AzID user_id)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                table != NULL,
                "Call should only be done while in BeginTable() scope!");
        return;
    }
    AZ_ASSERT(table->IsLayoutLocked == false
              && "Need to call TableSetupColumn() before first row!");
    AZ_ASSERT((flags & AzTableColumnFlags_StatusMask_) == 0
              && "Illegal to pass StatusMask values to TableSetupColumn()");
    if (table->DeclColumnsCount >= table->ColumnsCount)
    {
        AZ_ASSERT_USER_ERROR(table->DeclColumnsCount < table->ColumnsCount,
                             "Called TableSetupColumn() too many times!");
        return;
    }

    AzTableColumn* column = &table->Columns[table->DeclColumnsCount];
    table->DeclColumnsCount++;

    if (table->IsDefaultSizingPolicy
        && (flags & AzTableColumnFlags_WidthMask_) == 0
        && (flags & AzTableFlags_ScrollX) == 0)
        AZ_ASSERT(init_width_or_weight <= 0.0f && "Can only specify width/weight if sizing policy is set explicitly in either Table or Column.");

    if ((flags & AzTableColumnFlags_WidthMask_) == 0
        && init_width_or_weight > 0.0f)
        if ((table->Flags & AzTableFlags_SizingMask_)
                    == AzTableFlags_SizingFixedFit
            || (table->Flags & AzTableFlags_SizingMask_)
                       == AzTableFlags_SizingFixedSame)
            flags |= AzTableColumnFlags_WidthFixed;
    if (flags & AzTableColumnFlags_AngledHeader)
    {
        flags |= AzTableColumnFlags_NoHeaderLabel;
        table->AngledHeadersCount++;
    }

    TableSetupColumnFlags(table, column, flags);
    column->UserID = user_id;
    flags = column->Flags;

    column->InitStretchWeightOrWidth = init_width_or_weight;
    if (table->IsInitializing)
    {
        AzTableFlags init_flags = ~table->SettingsLoadedFlags;
        if (column->WidthRequest < 0.0f && column->StretchWeight < 0.0f)
            init_flags |= AzTableFlags_Resizable;
        TableInitColumnDefaults(table, column, init_flags);
    }

    column->NameOffset = -1;
    if (label != NULL && label[0] != 0)
    {
        column->NameOffset = (AzS16)table->ColumnsNames.size();
        table->ColumnsNames.append(label, label + AzStrlen(label) + 1);
    }
}

void Az::TableSetupScrollFreeze(int columns, int rows)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                table != NULL,
                "Call should only be done while in BeginTable() scope!");
        return;
    }
    AZ_ASSERT(table->IsLayoutLocked == false
              && "Need to call TableSetupColumn() before first row!");
    AZ_ASSERT(columns >= 0 && columns < AZ_TABLE_MAX_COLUMNS);
    AZ_ASSERT(rows >= 0 && rows < 128);

    table->FreezeColumnsRequest =
            (table->Flags & AzTableFlags_ScrollX)
                    ? (AzTableColumnIdx)AzMin(columns, table->ColumnsCount)
                    : 0;
    table->FreezeColumnsCount = (table->InnerWindow->Scroll.x != 0.0f)
                                        ? table->FreezeColumnsRequest
                                        : 0;
    table->FreezeRowsRequest = (table->Flags & AzTableFlags_ScrollY)
                                       ? (AzTableColumnIdx)rows
                                       : 0;
    table->FreezeRowsCount = (table->InnerWindow->Scroll.y != 0.0f)
                                     ? table->FreezeRowsRequest
                                     : 0;
    table->IsUnfrozenRows = (table->FreezeRowsCount == 0);

    for (int column_n = 0; column_n < table->FreezeColumnsRequest; column_n++)
    {
        int order_n = table->DisplayOrderToIndex[column_n];
        if (order_n != column_n && order_n >= table->FreezeColumnsRequest)
        {
            AzSwap(table->Columns[table->DisplayOrderToIndex[order_n]]
                           .DisplayOrder,
                   table->Columns[table->DisplayOrderToIndex[column_n]]
                           .DisplayOrder);
            AzSwap(table->DisplayOrderToIndex[order_n],
                   table->DisplayOrderToIndex[column_n]);
        }
    }
}

int Az::TableGetColumnCount()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    return table ? table->ColumnsCount : 0;
}

const char* Az::TableGetColumnName(int column_n)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return NULL;
    if (column_n < 0)
        column_n = table->CurrentColumn;
    return TableGetColumnName(table, column_n);
}

const char* Az::TableGetColumnName(const AzTable* table, int column_n)
{
    if (table->IsLayoutLocked == false && column_n >= table->DeclColumnsCount)
        return "";
    const AzTableColumn* column = &table->Columns[column_n];
    if (column->NameOffset == -1)
        return "";
    return &table->ColumnsNames.Buf[column->NameOffset];
}

void Az::TableSetColumnEnabled(int column_n, bool enabled)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                table != NULL,
                "Call should only be done while in BeginTable() scope!");
        return;
    }
    AZ_ASSERT(table->Flags & AzTableFlags_Hideable);
    if (column_n < 0)
        column_n = table->CurrentColumn;
    AZ_ASSERT(column_n >= 0 && column_n < table->ColumnsCount);
    AzTableColumn* column = &table->Columns[column_n];
    column->IsUserEnabledNextFrame = enabled;
}

AzTableColumnFlags Az::TableGetColumnFlags(int column_n)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return AzTableColumnFlags_None;
    if (column_n < 0)
        column_n = table->CurrentColumn;
    if (column_n == table->ColumnsCount)
        return (table->HoveredColumnBody == column_n)
                       ? AzTableColumnFlags_IsHovered
                       : AzTableColumnFlags_None;
    return table->Columns[column_n].Flags;
}

AzRect Az::TableGetCellBgRect(const AzTable* table, int column_n)
{
    const AzTableColumn* column = &table->Columns[column_n];
    float x1 = column->MinX;
    float x2 = column->MaxX;

    x1 = AzMax(x1, table->WorkRect.Min.x);
    x2 = AzMin(x2, table->WorkRect.Max.x);
    return AzRect(x1, table->RowPosY1, x2, table->RowPosY2);
}

AzID Az::TableGetColumnResizeID(AzTable* table, int column_n, int instance_no)
{
    AZ_ASSERT(column_n >= 0 && column_n < table->ColumnsCount);
    AzID instance_id = TableGetInstanceID(table, instance_no);
    return instance_id + 1 + column_n;
}

int Az::TableGetHoveredColumn()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return -1;
    return (int)table->HoveredColumnBody;
}

int Az::TableGetHoveredRow()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return -1;
    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    return (int)table_instance->HoveredRowLast;
}

void Az::TableSetBgColor(AzTableBgTarget target, AzU32 color, int column_n)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    AZ_ASSERT(target != AzTableBgTarget_None);

    if (color == AZ_COL32_DISABLE)
        color = 0;

    switch (target)
    {
    case AzTableBgTarget_CellBg: {
        if (table->RowPosY1 > table->InnerClipRect.Max.y)
            return;
        if (column_n == -1)
            column_n = table->CurrentColumn;
        if (!AZ_BITARRAY_TESTBIT(table->VisibleMaskByIndex, column_n))
            return;
        if (table->RowCellDataCurrent < 0
            || table->RowCellData[table->RowCellDataCurrent].Column != column_n)
            table->RowCellDataCurrent++;
        AzTableCellData* cell_data =
                &table->RowCellData[table->RowCellDataCurrent];
        cell_data->BgColor = color;
        cell_data->Column = (AzTableColumnIdx)column_n;
        break;
    }
    case AzTableBgTarget_RowBg0:
    case AzTableBgTarget_RowBg1: {
        if (table->RowPosY1 > table->InnerClipRect.Max.y)
            return;
        AZ_ASSERT(column_n == -1);
        int bg_idx = (target == AzTableBgTarget_RowBg1) ? 1 : 0;
        table->RowBgColor[bg_idx] = color;
        break;
    }
    default:
        AZ_ASSERT(0);
    }
}

int Az::TableGetRowIndex()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return 0;
    return table->CurrentRow;
}

void Az::TableNextRow(AzTableRowFlags row_flags, float row_min_height)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;

    if (!table->IsLayoutLocked)
        TableUpdateLayout(table);
    if (table->IsInsideRow)
        TableEndRow(table);

    table->LastRowFlags = table->RowFlags;
    table->RowFlags = row_flags;
    table->RowCellPaddingY = g.Style.CellPadding.y;
    table->RowMinHeight = row_min_height;
    TableBeginRow(table);

    table->RowPosY2 += table->RowCellPaddingY * 2.0f;
    table->RowPosY2 = AzMax(table->RowPosY2, table->RowPosY1 + row_min_height);

    table->InnerWindow->SkipItems = true;
}

void Az::TableBeginRow(AzTable* table)
{
    AzWindow* window = table->InnerWindow;
    AZ_ASSERT(!table->IsInsideRow);

    table->CurrentRow++;
    table->CurrentColumn = -1;
    table->RowBgColor[0] = table->RowBgColor[1] = AZ_COL32_DISABLE;
    table->RowCellDataCurrent = -1;
    table->IsInsideRow = true;

    float next_y1 = table->RowPosY2;
    if (table->CurrentRow == 0 && table->FreezeRowsCount > 0)
        next_y1 = window->DC.CursorPos.y = table->OuterRect.Min.y;

    table->RowPosY1 = table->RowPosY2 = next_y1;
    table->RowTextBaseline = 0.0f;
    table->RowIndentOffsetX = window->DC.Indent.x - table->HostIndentX;

    window->DC.PrevLineTextBaseOffset = 0.0f;
    window->DC.CursorPosPrevLine = AzVec2(window->DC.CursorPos.x,
                                          window->DC.CursorPos.y
                                                  + table->RowCellPaddingY);
    window->DC.PrevLineSize = window->DC.CurrLineSize = AzVec2(0.0f, 0.0f);
    window->DC.IsSameLine = window->DC.IsSetPos = false;
    window->DC.CursorMaxPos.y = next_y1;

    if (table->RowFlags & AzTableRowFlags_Headers)
    {
        TableSetBgColor(AzTableBgTarget_RowBg0,
                        GetColorU32(AzCol_TableHeaderBg));
        if (table->CurrentRow == 0)
            table->IsUsingHeaders = true;
    }
}

void Az::TableEndRow(AzTable* table)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(window == table->InnerWindow);
    AZ_ASSERT(table->IsInsideRow);

    if (table->CurrentColumn != -1)
    {
        TableEndCell(table);
        table->CurrentColumn = -1;
    }

    if (g.LogEnabled)
        LogRenderedText(NULL, "|");

    window->DC.CursorPos.y = table->RowPosY2;

    const float bg_y1 = table->RowPosY1;
    const float bg_y2 = table->RowPosY2;
    const bool unfreeze_rows_actual = (table->CurrentRow + 1
                                       == table->FreezeRowsCount);
    const bool unfreeze_rows_request = (table->CurrentRow + 1
                                        == table->FreezeRowsRequest);
    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    if ((table->RowFlags & AzTableRowFlags_Headers)
        && (table->CurrentRow == 0
            || (table->LastRowFlags & AzTableRowFlags_Headers)))
        table_instance->LastTopHeadersRowHeight += bg_y2 - bg_y1;

    const bool is_visible = (bg_y2 >= table->InnerClipRect.Min.y
                             && bg_y1 <= table->InnerClipRect.Max.y);
    if (is_visible)
    {
        if (table->HoveredColumnBody != -1 && g.IO.MousePos.y >= bg_y1
            && g.IO.MousePos.y < bg_y2 && table_instance->HoveredRowNext < 0)
            table_instance->HoveredRowNext = table->CurrentRow;

        AzU32 bg_col0 = 0;
        AzU32 bg_col1 = 0;
        if (table->RowBgColor[0] != AZ_COL32_DISABLE)
            bg_col0 = table->RowBgColor[0];
        else if (table->Flags & AzTableFlags_RowBg)
            bg_col0 = GetColorU32((table->RowBgColorCounter & 1)
                                          ? AzCol_TableRowBgAlt
                                          : AzCol_TableRowBg);
        if (table->RowBgColor[1] != AZ_COL32_DISABLE)
            bg_col1 = table->RowBgColor[1];

        AzU32 top_border_col = 0;
        const float border_size = TABLE_BORDER_SIZE;
        if (table->CurrentRow > 0
            && (table->Flags & AzTableFlags_BordersInnerH))
            top_border_col = (table->LastRowFlags & AzTableRowFlags_Headers)
                                     ? table->BorderColorStrong
                                     : table->BorderColorLight;

        const bool draw_cell_bg_color = table->RowCellDataCurrent >= 0;
        const bool draw_strong_bottom_border = unfreeze_rows_actual;
        if ((bg_col0 | bg_col1 | top_border_col) != 0
            || draw_strong_bottom_border || draw_cell_bg_color)
        {
            if ((table->Flags & AzTableFlags_NoClip) == 0)
                window->DrawList->_CmdHeader.ClipRect =
                        table->Bg0ClipRectForDrawCmd.ToVec4();
            table->DrawSplitter->SetCurrentChannel(window->DrawList,
                                                   TABLE_DRAW_CHANNEL_BG0);
        }

        if (bg_col0 || bg_col1)
        {
            AzRect row_rect(table->WorkRect.Min.x,
                            bg_y1,
                            table->WorkRect.Max.x,
                            bg_y2);
            row_rect.ClipWith(table->BgClipRect);
            if (bg_col0 != 0 && row_rect.Min.y < row_rect.Max.y)
                window->DrawList->AddRectFilled(row_rect.Min,
                                                row_rect.Max,
                                                bg_col0);
            if (bg_col1 != 0 && row_rect.Min.y < row_rect.Max.y)
                window->DrawList->AddRectFilled(row_rect.Min,
                                                row_rect.Max,
                                                bg_col1);
        }

        if (draw_cell_bg_color)
        {
            AzTableCellData* cell_data_end =
                    &table->RowCellData[table->RowCellDataCurrent];
            for (AzTableCellData* cell_data = &table->RowCellData[0];
                 cell_data <= cell_data_end;
                 cell_data++)
            {
                const AzTableColumn* column =
                        &table->Columns[cell_data->Column];
                AzRect cell_bg_rect = TableGetCellBgRect(table,
                                                         cell_data->Column);
                cell_bg_rect.ClipWith(table->BgClipRect);
                cell_bg_rect.Min.x = AzMax(cell_bg_rect.Min.x,
                                           column->ClipRect.Min.x);
                cell_bg_rect.Max.x = AzMin(cell_bg_rect.Max.x, column->MaxX);
                if (cell_bg_rect.Min.y < cell_bg_rect.Max.y)
                    window->DrawList->AddRectFilled(cell_bg_rect.Min,
                                                    cell_bg_rect.Max,
                                                    cell_data->BgColor);
            }
        }

        if (top_border_col && bg_y1 >= table->BgClipRect.Min.y
            && bg_y1 < table->BgClipRect.Max.y)
            window->DrawList->AddLine(AzVec2(table->BorderX1, bg_y1),
                                      AzVec2(table->BorderX2, bg_y1),
                                      top_border_col,
                                      border_size);

        if (draw_strong_bottom_border && bg_y2 >= table->BgClipRect.Min.y
            && bg_y2 < table->BgClipRect.Max.y)
            window->DrawList->AddLine(AzVec2(table->BorderX1, bg_y2),
                                      AzVec2(table->BorderX2, bg_y2),
                                      table->BorderColorStrong,
                                      border_size);
    }

    if (unfreeze_rows_request)
    {
        for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
            table->Columns[column_n].NavLayerCurrent = table->NavLayer;
        const float y0 = AzMax(table->RowPosY2 + 1, table->InnerClipRect.Min.y);
        table_instance->LastFrozenHeight = y0 - table->OuterRect.Min.y;

        if (unfreeze_rows_actual)
        {
            AZ_ASSERT(table->IsUnfrozenRows == false);
            table->IsUnfrozenRows = true;

            table->BgClipRect.Min.y = table->Bg2ClipRectForDrawCmd.Min.y =
                    AzMin(y0, table->InnerClipRect.Max.y);
            table->BgClipRect.Max.y = table->Bg2ClipRectForDrawCmd.Max.y =
                    table->InnerClipRect.Max.y;
            table->Bg2DrawChannelCurrent = table->Bg2DrawChannelUnfrozen;
            AZ_ASSERT(table->Bg2ClipRectForDrawCmd.Min.y
                      <= table->Bg2ClipRectForDrawCmd.Max.y);

            float row_height = table->RowPosY2 - table->RowPosY1;
            table->RowPosY2 = window->DC.CursorPos.y = table->WorkRect.Min.y
                                                       + table->RowPosY2
                                                       - table->OuterRect.Min.y;
            table->RowPosY1 = table->RowPosY2 - row_height;
            for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
            {
                AzTableColumn* column = &table->Columns[column_n];
                column->DrawChannelCurrent = column->DrawChannelUnfrozen;
                column->ClipRect.Min.y = table->Bg2ClipRectForDrawCmd.Min.y;
            }

            SetWindowClipRectBeforeSetChannel(window,
                                              table->Columns[0].ClipRect);
            table->DrawSplitter->SetCurrentChannel(
                    window->DrawList,
                    table->Columns[0].DrawChannelCurrent);
        }
    }

    if (!(table->RowFlags & AzTableRowFlags_Headers))
        table->RowBgColorCounter++;
    table->IsInsideRow = false;
}

int Az::TableGetColumnIndex()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return 0;
    return table->CurrentColumn;
}

bool Az::TableSetColumnIndex(int column_n)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return false;

    if (table->CurrentColumn != column_n)
    {
        if (table->CurrentColumn != -1)
            TableEndCell(table);
        if ((column_n >= 0 && column_n < table->ColumnsCount) == false)
        {
            AZ_ASSERT_USER_ERROR(column_n >= 0
                                         && column_n < table->ColumnsCount,
                                 "TableSetColumnIndex() invalid column index!");
            return false;
        }
        TableBeginCell(table, column_n);
    }

    return table->Columns[column_n].IsRequestOutput;
}

bool Az::TableNextColumn()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (!table)
        return false;

    if (table->IsInsideRow && table->CurrentColumn + 1 < table->ColumnsCount)
    {
        if (table->CurrentColumn != -1)
            TableEndCell(table);
        TableBeginCell(table, table->CurrentColumn + 1);
    } else
    {
        TableNextRow();
        TableBeginCell(table, 0);
    }

    return table->Columns[table->CurrentColumn].IsRequestOutput;
}

void Az::TableBeginCell(AzTable* table, int column_n)
{
    AzContext& g = *GAz;
    AzTableColumn* column = &table->Columns[column_n];
    AzWindow* window = table->InnerWindow;
    table->CurrentColumn = column_n;

    float start_x = column->WorkMinX;
    if (column->Flags & AzTableColumnFlags_IndentEnable)
        start_x += table->RowIndentOffsetX;

    window->DC.CursorPos.x = start_x;
    window->DC.CursorPos.y = table->RowPosY1 + table->RowCellPaddingY;
    window->DC.CursorMaxPos.x = window->DC.CursorPos.x;
    window->DC.ColumnsOffset.x = start_x - window->Pos.x - window->DC.Indent.x;
    window->DC.CursorPosPrevLine.x = window->DC.CursorPos.x;
    window->DC.CurrLineTextBaseOffset = table->RowTextBaseline;
    window->DC.NavLayerCurrent = (AzNavLayer)column->NavLayerCurrent;

    window->WorkRect.Min.y = window->DC.CursorPos.y;
    window->WorkRect.Min.x = column->WorkMinX;
    window->WorkRect.Max.x = column->WorkMaxX;
    window->DC.ItemWidth = column->ItemWidth;

    window->SkipItems = column->IsSkipItems;
    if (column->IsSkipItems)
    {
        g.LastItemData.ID = 0;
        g.LastItemData.StatusFlags = 0;
    }

    if (table->Flags & AzTableFlags_NoClip)
    {
        table->DrawSplitter->SetCurrentChannel(window->DrawList,
                                               TABLE_DRAW_CHANNEL_NOCLIP);

    } else
    {
        SetWindowClipRectBeforeSetChannel(window, column->ClipRect);
        table->DrawSplitter->SetCurrentChannel(window->DrawList,
                                               column->DrawChannelCurrent);
    }

    if (g.LogEnabled && !column->IsSkipItems)
    {
        LogRenderedText(&window->DC.CursorPos, "|");
        g.LogLinePosY = FLT_MAX;
    }
}

void Az::TableEndCell(AzTable* table)
{
    AzTableColumn* column = &table->Columns[table->CurrentColumn];
    AzWindow* window = table->InnerWindow;

    if (window->DC.IsSetPos)
        ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

    float* p_max_pos_x;
    if (table->RowFlags & AzTableRowFlags_Headers)
        p_max_pos_x = &column->ContentMaxXHeadersUsed;
    else
        p_max_pos_x = table->IsUnfrozenRows ? &column->ContentMaxXUnfrozen
                                            : &column->ContentMaxXFrozen;
    *p_max_pos_x = AzMax(*p_max_pos_x, window->DC.CursorMaxPos.x);
    if (column->IsEnabled)
        table->RowPosY2 = AzMax(table->RowPosY2,
                                window->DC.CursorMaxPos.y
                                        + table->RowCellPaddingY);
    column->ItemWidth = window->DC.ItemWidth;

    table->RowTextBaseline = AzMax(table->RowTextBaseline,
                                   window->DC.PrevLineTextBaseOffset);
}

float Az::TableCalcMaxColumnWidth(const AzTable* table, int column_n)
{
    const AzTableColumn* column = &table->Columns[column_n];
    float max_width = FLT_MAX;
    const float min_column_distance = table->MinColumnWidth
                                      + table->CellPaddingX * 2.0f
                                      + table->CellSpacingX1
                                      + table->CellSpacingX2;
    if (table->Flags & AzTableFlags_ScrollX)
    {
        if (column->DisplayOrder < table->FreezeColumnsRequest)
        {
            max_width = (table->InnerClipRect.Max.x
                         - (table->FreezeColumnsRequest - column->DisplayOrder)
                                   * min_column_distance)
                        - column->MinX;
            max_width = max_width - table->OuterPaddingX - table->CellPaddingX
                        - table->CellSpacingX2;
        }
    } else if ((table->Flags & AzTableFlags_NoKeepColumnsVisible) == 0)
    {
        max_width = table->WorkRect.Max.x
                    - (table->ColumnsEnabledCount
                       - column->IndexWithinEnabledSet - 1)
                              * min_column_distance
                    - column->MinX;

        max_width -= table->CellSpacingX2;
        max_width -= table->CellPaddingX * 2.0f;
        max_width -= table->OuterPaddingX;
    }
    return max_width;
}

float Az::TableGetColumnWidthAuto(AzTable* table, AzTableColumn* column)
{
    const float content_width_body = AzMax(column->ContentMaxXFrozen,
                                           column->ContentMaxXUnfrozen)
                                     - column->WorkMinX;
    const float content_width_headers = column->ContentMaxXHeadersIdeal
                                        - column->WorkMinX;
    float width_auto = content_width_body;
    if (!(column->Flags & AzTableColumnFlags_NoHeaderWidth))
        width_auto = AzMax(width_auto, content_width_headers);

    if ((column->Flags & AzTableColumnFlags_WidthFixed)
        && column->InitStretchWeightOrWidth > 0.0f)
        if (!(table->Flags & AzTableFlags_Resizable)
            || (column->Flags & AzTableColumnFlags_NoResize))
            width_auto = column->InitStretchWeightOrWidth;

    return AzMax(width_auto, table->MinColumnWidth);
}

void Az::TableSetColumnWidth(int column_n, float width)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    AZ_ASSERT(table != NULL && table->IsLayoutLocked == false);
    AZ_ASSERT(column_n >= 0 && column_n < table->ColumnsCount);
    AzTableColumn* column_0 = &table->Columns[column_n];
    float column_0_width = width;

    AZ_ASSERT(table->MinColumnWidth > 0.0f);
    const float min_width = table->MinColumnWidth;
    const float max_width = AzMax(min_width, column_0->WidthMax);
    column_0_width = AzClamp(column_0_width, min_width, max_width);
    if (column_0->WidthGiven == column_0_width
        || column_0->WidthRequest == column_0_width)
        return;

    AzTableColumn* column_1 =
            (column_0->NextEnabledColumn != -1)
                    ? &table->Columns[column_0->NextEnabledColumn]
                    : NULL;

    if (column_0->Flags & AzTableColumnFlags_WidthFixed)
        if (!column_1 || table->LeftMostStretchedColumn == -1
            || table->Columns[table->LeftMostStretchedColumn].DisplayOrder
                       >= column_0->DisplayOrder)
        {
            column_0->WidthRequest = column_0_width;
            table->IsSettingsDirty = true;
            return;
        }

    if (column_1 == NULL)
        column_1 = (column_0->PrevEnabledColumn != -1)
                           ? &table->Columns[column_0->PrevEnabledColumn]
                           : NULL;
    if (column_1 == NULL)
        return;

    float column_1_width = AzMax(
            column_1->WidthRequest - (column_0_width - column_0->WidthRequest),
            min_width);
    column_0_width = column_0->WidthRequest + column_1->WidthRequest
                     - column_1_width;
    AZ_ASSERT(column_0_width > 0.0f && column_1_width > 0.0f);
    column_0->WidthRequest = column_0_width;
    column_1->WidthRequest = column_1_width;
    if ((column_0->Flags | column_1->Flags) & AzTableColumnFlags_WidthStretch)
        TableUpdateColumnsWeightFromWidth(table);
    table->IsSettingsDirty = true;
}

void Az::TableSetColumnWidthAutoSingle(AzTable* table, int column_n)
{
    AzTableColumn* column = &table->Columns[column_n];
    if (!column->IsEnabled)
        return;
    column->CannotSkipItemsQueue = (1 << 0);
    table->AutoFitSingleColumn = (AzTableColumnIdx)column_n;
}

void Az::TableSetColumnWidthAutoAll(AzTable* table)
{
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        AzTableColumn* column = &table->Columns[column_n];
        if (!column->IsEnabled
            && !(column->Flags & AzTableColumnFlags_WidthStretch))
            continue;
        column->CannotSkipItemsQueue = (1 << 0);
        column->AutoFitQueue = (1 << 1);
    }
}

void Az::TableUpdateColumnsWeightFromWidth(AzTable* table)
{
    AZ_ASSERT(table->LeftMostStretchedColumn != -1
              && table->RightMostStretchedColumn != -1);

    float visible_weight = 0.0f;
    float visible_width = 0.0f;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        AzTableColumn* column = &table->Columns[column_n];
        if (!column->IsEnabled
            || !(column->Flags & AzTableColumnFlags_WidthStretch))
            continue;
        AZ_ASSERT(column->StretchWeight > 0.0f);
        visible_weight += column->StretchWeight;
        visible_width += column->WidthRequest;
    }
    AZ_ASSERT(visible_weight > 0.0f && visible_width > 0.0f);

    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        AzTableColumn* column = &table->Columns[column_n];
        if (!column->IsEnabled
            || !(column->Flags & AzTableColumnFlags_WidthStretch))
            continue;
        column->StretchWeight = (column->WidthRequest / visible_width)
                                * visible_weight;
        AZ_ASSERT(column->StretchWeight > 0.0f);
    }
}

void Az::TablePushBackgroundChannel()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzTable* table = g.CurrentTable;

    table->HostBackupInnerClipRect = window->ClipRect;
    SetWindowClipRectBeforeSetChannel(window, table->Bg2ClipRectForDrawCmd);
    table->DrawSplitter->SetCurrentChannel(window->DrawList,
                                           table->Bg2DrawChannelCurrent);
}

void Az::TablePopBackgroundChannel()
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzTable* table = g.CurrentTable;

    SetWindowClipRectBeforeSetChannel(window, table->HostBackupInnerClipRect);
    table->DrawSplitter->SetCurrentChannel(
            window->DrawList,
            table->Columns[table->CurrentColumn].DrawChannelCurrent);
}

void Az::TablePushColumnChannel(int column_n)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;

    if (table->Flags & AzTableFlags_NoClip)
        return;
    AzWindow* window = g.CurrentWindow;
    const AzTableColumn* column = &table->Columns[column_n];
    SetWindowClipRectBeforeSetChannel(window, column->ClipRect);
    table->DrawSplitter->SetCurrentChannel(window->DrawList,
                                           column->DrawChannelCurrent);
}

void Az::TablePopColumnChannel()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;

    if ((table->Flags & AzTableFlags_NoClip) || (table->CurrentColumn == -1))
        return;
    AzWindow* window = g.CurrentWindow;
    const AzTableColumn* column = &table->Columns[table->CurrentColumn];
    SetWindowClipRectBeforeSetChannel(window, column->ClipRect);
    table->DrawSplitter->SetCurrentChannel(window->DrawList,
                                           column->DrawChannelCurrent);
}

void Az::TableSetupDrawChannels(AzTable* table)
{
    const int freeze_row_multiplier = (table->FreezeRowsCount > 0) ? 2 : 1;
    const int channels_for_row = (table->Flags & AzTableFlags_NoClip)
                                         ? 1
                                         : table->ColumnsEnabledCount;
    const int channels_for_bg = 1 + 1 * freeze_row_multiplier;
    const int channels_for_dummy =
            (table->ColumnsEnabledCount < table->ColumnsCount
             || (memcmp(table->VisibleMaskByIndex,
                        table->EnabledMaskByIndex,
                        AzBitArrayGetStorageSizeInBytes(table->ColumnsCount))
                 != 0))
                    ? +1
                    : 0;
    const int channels_total = channels_for_bg
                               + (channels_for_row * freeze_row_multiplier)
                               + channels_for_dummy;
    table->DrawSplitter->Split(table->InnerWindow->DrawList, channels_total);
    table->DummyDrawChannel = (AzTableDrawChannelIdx)((channels_for_dummy > 0)
                                                              ? channels_total
                                                                        - 1
                                                              : -1);
    table->Bg2DrawChannelCurrent = TABLE_DRAW_CHANNEL_BG2_FROZEN;
    table->Bg2DrawChannelUnfrozen =
            (AzTableDrawChannelIdx)((table->FreezeRowsCount > 0)
                                            ? 2 + channels_for_row
                                            : TABLE_DRAW_CHANNEL_BG2_FROZEN);

    int draw_channel_current = 2;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        AzTableColumn* column = &table->Columns[column_n];
        if (column->IsVisibleX && column->IsVisibleY)
        {
            column->DrawChannelFrozen =
                    (AzTableDrawChannelIdx)(draw_channel_current);
            column->DrawChannelUnfrozen =
                    (AzTableDrawChannelIdx)(draw_channel_current
                                            + (table->FreezeRowsCount > 0
                                                       ? channels_for_row + 1
                                                       : 0));
            if (!(table->Flags & AzTableFlags_NoClip))
                draw_channel_current++;
        } else
        {
            column->DrawChannelFrozen = column->DrawChannelUnfrozen =
                    table->DummyDrawChannel;
        }
        column->DrawChannelCurrent = column->DrawChannelFrozen;
    }

    table->BgClipRect = table->InnerClipRect;
    table->Bg0ClipRectForDrawCmd = table->OuterWindow->ClipRect;
    table->Bg2ClipRectForDrawCmd = table->HostClipRect;
    AZ_ASSERT(table->BgClipRect.Min.y <= table->BgClipRect.Max.y);
}

void Az::TableMergeDrawChannels(AzTable* table)
{
    AzContext& g = *GAz;
    AzDrawListSplitter* splitter = table->DrawSplitter;
    const bool has_freeze_v = (table->FreezeRowsCount > 0);
    const bool has_freeze_h = (table->FreezeColumnsCount > 0);
    AZ_ASSERT(splitter->_Current == 0);

    struct MergeGroup
    {
        AzRect ClipRect;
        int ChannelsCount = 0;
        AzBitArrayPtr ChannelsMask = NULL;
    };
    int merge_group_mask = 0x00;
    MergeGroup merge_groups[4];

    const int max_draw_channels = (4 + table->ColumnsCount * 2);
    const int size_for_masks_bitarrays_one = (int)
            AzBitArrayGetStorageSizeInBytes(max_draw_channels);
    g.TempBuffer.reserve(size_for_masks_bitarrays_one * 5);
    memset(g.TempBuffer.Data, 0, size_for_masks_bitarrays_one * 5);
    for (int n = 0; n < AZ_ARRAYSIZE(merge_groups); n++)
        merge_groups[n].ChannelsMask =
                (AzBitArrayPtr)(void*)(g.TempBuffer.Data
                                       + (size_for_masks_bitarrays_one * n));
    AzBitArrayPtr remaining_mask =
            (AzBitArrayPtr)(void*)(g.TempBuffer.Data
                                   + (size_for_masks_bitarrays_one * 4));

    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        if (!AZ_BITARRAY_TESTBIT(table->VisibleMaskByIndex, column_n))
            continue;
        AzTableColumn* column = &table->Columns[column_n];

        const int merge_group_sub_count = has_freeze_v ? 2 : 1;
        for (int merge_group_sub_n = 0;
             merge_group_sub_n < merge_group_sub_count;
             merge_group_sub_n++)
        {
            const int channel_no = (merge_group_sub_n == 0)
                                           ? column->DrawChannelFrozen
                                           : column->DrawChannelUnfrozen;

            AzDrawChannel* src_channel = &splitter->_Channels[channel_no];
            if (src_channel->_CmdBuffer.Size > 0
                && src_channel->_CmdBuffer.back().ElemCount == 0
                && src_channel->_CmdBuffer.back().UserCallback == NULL)
                src_channel->_CmdBuffer.pop_back();
            if (src_channel->_CmdBuffer.Size != 1)
                continue;

            if (!(column->Flags & AzTableColumnFlags_NoClip))
            {
                float content_max_x;
                if (!has_freeze_v)
                    content_max_x = AzMax(column->ContentMaxXUnfrozen,
                                          column->ContentMaxXHeadersUsed);
                else if (merge_group_sub_n == 0)
                    content_max_x = AzMax(column->ContentMaxXFrozen,
                                          column->ContentMaxXHeadersUsed);
                else
                    content_max_x = column->ContentMaxXUnfrozen;
                if (content_max_x > column->ClipRect.Max.x)
                    continue;
            }

            const int merge_group_n =
                    (has_freeze_h && column_n < table->FreezeColumnsCount ? 0
                                                                          : 1)
                    + (has_freeze_v && merge_group_sub_n == 0 ? 0 : 2);
            AZ_ASSERT(channel_no < max_draw_channels);
            MergeGroup* merge_group = &merge_groups[merge_group_n];
            if (merge_group->ChannelsCount == 0)
                merge_group->ClipRect =
                        AzRect(+FLT_MAX, +FLT_MAX, -FLT_MAX, -FLT_MAX);
            AzBitArraySetBit(merge_group->ChannelsMask, channel_no);
            merge_group->ChannelsCount++;
            merge_group->ClipRect.Add(src_channel->_CmdBuffer[0].ClipRect);
            merge_group_mask |= (1 << merge_group_n);
        }

        column->DrawChannelCurrent = (AzTableDrawChannelIdx)-1;
    }

#if 0
    if (g.IO.KeyShift)
        for (int merge_group_n = 0; merge_group_n < AZ_ARRAYSIZE(merge_groups); merge_group_n++)
        {
            MergeGroup* merge_group = &merge_groups[merge_group_n];
            if (merge_group->ChannelsCount == 0)
                continue;
            char buf[32];
            AzFormatString(buf, 32, "MG%d:%d", merge_group_n, merge_group->ChannelsCount);
            AzVec2 text_pos = merge_group->ClipRect.Min + AzVec2(4, 4);
            AzVec2 text_size = CalcTextSize(buf, NULL);
            GetForegroundDrawList()->AddRectFilled(text_pos, text_pos + text_size, AZ_COL32(0, 0, 0, 255));
            GetForegroundDrawList()->AddText(text_pos, AZ_COL32(255, 255, 0, 255), buf, NULL);
            GetForegroundDrawList()->AddRect(merge_group->ClipRect.Min, merge_group->ClipRect.Max, AZ_COL32(255, 255, 0, 255));
        }
#endif

    if (merge_group_mask != 0)
    {
        const int LEADING_DRAW_CHANNELS = 2;
        g.DrawChannelsTempMergeBuffer.resize(splitter->_Count
                                             - LEADING_DRAW_CHANNELS);
        AzDrawChannel* dst_tmp = g.DrawChannelsTempMergeBuffer.Data;
        AzBitArraySetBitRange(remaining_mask,
                              LEADING_DRAW_CHANNELS,
                              splitter->_Count);
        AzBitArrayClearBit(remaining_mask, table->Bg2DrawChannelUnfrozen);
        AZ_ASSERT(has_freeze_v == false
                  || table->Bg2DrawChannelUnfrozen
                             != TABLE_DRAW_CHANNEL_BG2_FROZEN);
        int remaining_count = splitter->_Count
                              - (has_freeze_v ? LEADING_DRAW_CHANNELS + 1
                                              : LEADING_DRAW_CHANNELS);

        AzRect host_rect = table->HostClipRect;
        for (int merge_group_n = 0; merge_group_n < AZ_ARRAYSIZE(merge_groups);
             merge_group_n++)
        {
            if (int merge_channels_count =
                        merge_groups[merge_group_n].ChannelsCount)
            {
                MergeGroup* merge_group = &merge_groups[merge_group_n];
                AzRect merge_clip_rect = merge_group->ClipRect;

                if ((merge_group_n & 1) == 0 || !has_freeze_h)
                    merge_clip_rect.Min.x = AzMin(merge_clip_rect.Min.x,
                                                  host_rect.Min.x);
                if ((merge_group_n & 2) == 0 || !has_freeze_v)
                    merge_clip_rect.Min.y = AzMin(merge_clip_rect.Min.y,
                                                  host_rect.Min.y);
                if ((merge_group_n & 1) != 0)
                    merge_clip_rect.Max.x = AzMax(merge_clip_rect.Max.x,
                                                  host_rect.Max.x);
                if ((merge_group_n & 2) != 0
                    && (table->Flags & AzTableFlags_NoHostExtendY) == 0)
                    merge_clip_rect.Max.y = AzMax(merge_clip_rect.Max.y,
                                                  host_rect.Max.y);

                remaining_count -= merge_group->ChannelsCount;
                for (int n = 0; n < (size_for_masks_bitarrays_one >> 2); n++)
                    remaining_mask[n] &= ~merge_group->ChannelsMask[n];
                for (int n = 0;
                     n < splitter->_Count && merge_channels_count != 0;
                     n++)
                {
                    if (!AZ_BITARRAY_TESTBIT(merge_group->ChannelsMask, n))
                        continue;
                    AZ_BITARRAY_CLEARBIT(merge_group->ChannelsMask, n);
                    merge_channels_count--;

                    AzDrawChannel* channel = &splitter->_Channels[n];
                    AZ_ASSERT(channel->_CmdBuffer.Size == 1
                              && merge_clip_rect.Contains(
                                      AzRect(channel->_CmdBuffer[0].ClipRect)));
                    channel->_CmdBuffer[0].ClipRect = merge_clip_rect.ToVec4();
                    memcpy(dst_tmp++, channel, sizeof(AzDrawChannel));
                }
            }

            if (merge_group_n == 1 && has_freeze_v)
                memcpy(dst_tmp++,
                       &splitter->_Channels[table->Bg2DrawChannelUnfrozen],
                       sizeof(AzDrawChannel));
        }

        for (int n = 0; n < splitter->_Count && remaining_count != 0; n++)
        {
            if (!AZ_BITARRAY_TESTBIT(remaining_mask, n))
                continue;
            AzDrawChannel* channel = &splitter->_Channels[n];
            memcpy(dst_tmp++, channel, sizeof(AzDrawChannel));
            remaining_count--;
        }
        AZ_ASSERT(dst_tmp
                  == g.DrawChannelsTempMergeBuffer.Data
                             + g.DrawChannelsTempMergeBuffer.Size);
        memcpy(splitter->_Channels.Data + LEADING_DRAW_CHANNELS,
               g.DrawChannelsTempMergeBuffer.Data,
               (splitter->_Count - LEADING_DRAW_CHANNELS)
                       * sizeof(AzDrawChannel));
    }
}

static AzU32 TableGetColumnBorderCol(AzTable* table, int order_n, int column_n)
{
    const bool is_hovered = (table->HoveredColumnBorder == column_n);
    const bool is_resized = (table->ResizedColumn == column_n)
                            && (table->InstanceInteracted
                                == table->InstanceCurrent);
    const bool is_frozen_separator = (table->FreezeColumnsCount == order_n + 1);
    if (is_resized || is_hovered)
        return Az::GetColorU32(is_resized ? AzCol_SeparatorActive
                                          : AzCol_SeparatorHovered);
    if (is_frozen_separator
        || (table->Flags
            & (AzTableFlags_NoBordersInBody
               | AzTableFlags_NoBordersInBodyUntilResize)))
        return table->BorderColorStrong;
    return table->BorderColorLight;
}

void Az::TableDrawBorders(AzTable* table)
{
    AzWindow* inner_window = table->InnerWindow;
    if (!table->OuterWindow->ClipRect.Overlaps(table->OuterRect))
        return;

    AzDrawList* inner_drawlist = inner_window->DrawList;
    table->DrawSplitter->SetCurrentChannel(inner_drawlist,
                                           TABLE_DRAW_CHANNEL_BG0);
    inner_drawlist->PushClipRect(table->Bg0ClipRectForDrawCmd.Min,
                                 table->Bg0ClipRectForDrawCmd.Max,
                                 false);

    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    const float border_size = TABLE_BORDER_SIZE;
    const float draw_y1 =
            AzMax(table->InnerRect.Min.y,
                  (table->FreezeRowsCount >= 1 ? table->InnerRect.Min.y
                                               : table->WorkRect.Min.y)
                          + table->AngledHeadersHeight)
            + ((table->Flags & AzTableFlags_BordersOuterH) ? 1.0f : 0.0f);
    const float draw_y2_body = table->InnerRect.Max.y;
    const float draw_y2_head =
            table->IsUsingHeaders
                    ? AzMin(table->InnerRect.Max.y,
                            (table->FreezeRowsCount >= 1
                                     ? table->InnerRect.Min.y
                                     : table->WorkRect.Min.y)
                                    + table_instance->LastTopHeadersRowHeight)
                    : draw_y1;
    if (table->Flags & AzTableFlags_BordersInnerV)
    {
        for (int order_n = 0; order_n < table->ColumnsCount; order_n++)
        {
            if (!AZ_BITARRAY_TESTBIT(table->EnabledMaskByDisplayOrder, order_n))
                continue;

            const int column_n = table->DisplayOrderToIndex[order_n];
            AzTableColumn* column = &table->Columns[column_n];
            const bool is_hovered = (table->HoveredColumnBorder == column_n);
            const bool is_resized = (table->ResizedColumn == column_n)
                                    && (table->InstanceInteracted
                                        == table->InstanceCurrent);
            const bool is_resizable = (column->Flags
                                       & (AzTableColumnFlags_NoResize
                                          | AzTableColumnFlags_NoDirectResize_))
                                      == 0;
            const bool is_frozen_separator = (table->FreezeColumnsCount
                                              == order_n + 1);
            if (column->MaxX > table->InnerClipRect.Max.x && !is_resized)
                continue;

            if (column->NextEnabledColumn == -1 && !is_resizable)
                if ((table->Flags & AzTableFlags_SizingMask_)
                            != AzTableFlags_SizingFixedSame
                    || (table->Flags & AzTableFlags_NoHostExtendX))
                    continue;
            if (column->MaxX <= column->ClipRect.Min.x)
                continue;

            float draw_y2 = (is_hovered || is_resized || is_frozen_separator
                             || (table->Flags
                                 & (AzTableFlags_NoBordersInBody
                                    | AzTableFlags_NoBordersInBodyUntilResize))
                                        == 0)
                                    ? draw_y2_body
                                    : draw_y2_head;
            if (draw_y2 > draw_y1)
                inner_drawlist->AddLine(AzVec2(column->MaxX, draw_y1),
                                        AzVec2(column->MaxX, draw_y2),
                                        TableGetColumnBorderCol(table,
                                                                order_n,
                                                                column_n),
                                        border_size);
        }
    }

    if (table->Flags & AzTableFlags_BordersOuter)
    {
        const AzRect outer_border = table->OuterRect;
        const AzU32 outer_col = table->BorderColorStrong;
        if ((table->Flags & AzTableFlags_BordersOuter)
            == AzTableFlags_BordersOuter)
        {
            inner_drawlist->AddRect(outer_border.Min,
                                    outer_border.Max,
                                    outer_col,
                                    0.0f,
                                    0,
                                    border_size);
        } else if (table->Flags & AzTableFlags_BordersOuterV)
        {
            inner_drawlist->AddLine(outer_border.Min,
                                    AzVec2(outer_border.Min.x,
                                           outer_border.Max.y),
                                    outer_col,
                                    border_size);
            inner_drawlist->AddLine(AzVec2(outer_border.Max.x,
                                           outer_border.Min.y),
                                    outer_border.Max,
                                    outer_col,
                                    border_size);
        } else if (table->Flags & AzTableFlags_BordersOuterH)
        {
            inner_drawlist->AddLine(outer_border.Min,
                                    AzVec2(outer_border.Max.x,
                                           outer_border.Min.y),
                                    outer_col,
                                    border_size);
            inner_drawlist->AddLine(AzVec2(outer_border.Min.x,
                                           outer_border.Max.y),
                                    outer_border.Max,
                                    outer_col,
                                    border_size);
        }
    }
    if ((table->Flags & AzTableFlags_BordersInnerH)
        && table->RowPosY2 < table->OuterRect.Max.y)
    {
        const float border_y = table->RowPosY2;
        if (border_y >= table->BgClipRect.Min.y
            && border_y < table->BgClipRect.Max.y)
            inner_drawlist->AddLine(AzVec2(table->BorderX1, border_y),
                                    AzVec2(table->BorderX2, border_y),
                                    table->BorderColorLight,
                                    border_size);
    }

    inner_drawlist->PopClipRect();
}

AzTableSortSpecs* Az::TableGetSortSpecs()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    AZ_ASSERT(table != NULL);

    if (!(table->Flags & AzTableFlags_Sortable))
        return NULL;

    if (!table->IsLayoutLocked)
        TableUpdateLayout(table);

    TableSortSpecsBuild(table);
    return &table->SortSpecs;
}

static inline AzSortDirection TableGetColumnAvailSortDirection(
        AzTableColumn* column,
        int n)
{
    AZ_ASSERT(n < column->SortDirectionsAvailCount);
    return (AzSortDirection)((column->SortDirectionsAvailList >> (n << 1))
                             & 0x03);
}

void Az::TableFixColumnSortDirection(AzTable* table, AzTableColumn* column)
{
    if (column->SortOrder == -1
        || (column->SortDirectionsAvailMask & (1 << column->SortDirection))
                   != 0)
        return;
    column->SortDirection = (AzU8)TableGetColumnAvailSortDirection(column, 0);
    table->IsSortSpecsDirty = true;
}

AZ_STATIC_ASSERT(AzSortDirection_None == 0 && AzSortDirection_Ascending == 1
                 && AzSortDirection_Descending == 2);
AzSortDirection Az::TableGetColumnNextSortDirection(AzTableColumn* column)
{
    AZ_ASSERT(column->SortDirectionsAvailCount > 0);
    if (column->SortOrder == -1)
        return TableGetColumnAvailSortDirection(column, 0);
    for (int n = 0; n < 3; n++)
        if (column->SortDirection
            == TableGetColumnAvailSortDirection(column, n))
            return TableGetColumnAvailSortDirection(
                    column,
                    (n + 1) % column->SortDirectionsAvailCount);
    AZ_ASSERT(0);
    return AzSortDirection_None;
}

void Az::TableSetColumnSortDirection(int column_n,
                                     AzSortDirection sort_direction,
                                     bool append_to_sort_specs)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;

    if (!(table->Flags & AzTableFlags_SortMulti))
        append_to_sort_specs = false;
    if (!(table->Flags & AzTableFlags_SortTristate))
        AZ_ASSERT(sort_direction != AzSortDirection_None);

    AzTableColumnIdx sort_order_max = 0;
    if (append_to_sort_specs)
        for (int other_column_n = 0; other_column_n < table->ColumnsCount;
             other_column_n++)
            sort_order_max = AzMax(sort_order_max,
                                   table->Columns[other_column_n].SortOrder);

    AzTableColumn* column = &table->Columns[column_n];
    column->SortDirection = (AzU8)sort_direction;
    if (column->SortDirection == AzSortDirection_None)
        column->SortOrder = -1;
    else if (column->SortOrder == -1 || !append_to_sort_specs)
        column->SortOrder = append_to_sort_specs ? sort_order_max + 1 : 0;

    for (int other_column_n = 0; other_column_n < table->ColumnsCount;
         other_column_n++)
    {
        AzTableColumn* other_column = &table->Columns[other_column_n];
        if (other_column != column && !append_to_sort_specs)
            other_column->SortOrder = -1;
        TableFixColumnSortDirection(table, other_column);
    }
    table->IsSettingsDirty = true;
    table->IsSortSpecsDirty = true;
}

void Az::TableSortSpecsSanitize(AzTable* table)
{
    AZ_ASSERT(table->Flags & AzTableFlags_Sortable);

    int sort_order_count = 0;
    AzU64 sort_order_mask = 0x00;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        AzTableColumn* column = &table->Columns[column_n];
        if (column->SortOrder != -1 && !column->IsEnabled)
            column->SortOrder = -1;
        if (column->SortOrder == -1)
            continue;
        sort_order_count++;
        sort_order_mask |= ((AzU64)1 << column->SortOrder);
        AZ_ASSERT(sort_order_count < (int)sizeof(sort_order_mask) * 8);
    }

    const bool need_fix_linearize = ((AzU64)1 << sort_order_count)
                                    != (sort_order_mask + 1);
    const bool need_fix_single_sort_order = (sort_order_count > 1)
                                            && !(table->Flags
                                                 & AzTableFlags_SortMulti);
    if (need_fix_linearize || need_fix_single_sort_order)
    {
        AzU64 fixed_mask = 0x00;
        for (int sort_n = 0; sort_n < sort_order_count; sort_n++)
        {
            int column_with_smallest_sort_order = -1;
            for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
                if ((fixed_mask & ((AzU64)1 << (AzU64)column_n)) == 0
                    && table->Columns[column_n].SortOrder != -1)
                    if (column_with_smallest_sort_order == -1
                        || table->Columns[column_n].SortOrder
                                   < table->Columns
                                             [column_with_smallest_sort_order]
                                                     .SortOrder)
                        column_with_smallest_sort_order = column_n;
            AZ_ASSERT(column_with_smallest_sort_order != -1);
            fixed_mask |= ((AzU64)1 << column_with_smallest_sort_order);
            table->Columns[column_with_smallest_sort_order].SortOrder =
                    (AzTableColumnIdx)sort_n;

            if (need_fix_single_sort_order)
            {
                sort_order_count = 1;
                for (int column_n = 0; column_n < table->ColumnsCount;
                     column_n++)
                    if (column_n != column_with_smallest_sort_order)
                        table->Columns[column_n].SortOrder = -1;
                break;
            }
        }
    }

    if (sort_order_count == 0 && !(table->Flags & AzTableFlags_SortTristate))
        for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
        {
            AzTableColumn* column = &table->Columns[column_n];
            if (column->IsEnabled
                && !(column->Flags & AzTableColumnFlags_NoSort))
            {
                sort_order_count = 1;
                column->SortOrder = 0;
                column->SortDirection = (AzU8)
                        TableGetColumnAvailSortDirection(column, 0);
                break;
            }
        }

    table->SortSpecsCount = (AzTableColumnIdx)sort_order_count;
}

void Az::TableSortSpecsBuild(AzTable* table)
{
    bool dirty = table->IsSortSpecsDirty;
    if (dirty)
    {
        TableSortSpecsSanitize(table);
        table->SortSpecsMulti.resize(
                table->SortSpecsCount <= 1 ? 0 : table->SortSpecsCount);
        table->SortSpecs.SpecsDirty = true;
        table->IsSortSpecsDirty = false;
    }

    AzTableColumnSortSpecs* sort_specs = (table->SortSpecsCount == 0) ? NULL
                                         : (table->SortSpecsCount == 1)
                                                 ? &table->SortSpecsSingle
                                                 : table->SortSpecsMulti.Data;
    if (dirty && sort_specs != NULL)
        for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
        {
            AzTableColumn* column = &table->Columns[column_n];
            if (column->SortOrder == -1)
                continue;
            AZ_ASSERT(column->SortOrder < table->SortSpecsCount);
            AzTableColumnSortSpecs* sort_spec = &sort_specs[column->SortOrder];
            sort_spec->ColumnUserID = column->UserID;
            sort_spec->ColumnIndex = (AzTableColumnIdx)column_n;
            sort_spec->SortOrder = (AzTableColumnIdx)column->SortOrder;
            sort_spec->SortDirection = (AzSortDirection)column->SortDirection;
        }

    table->SortSpecs.Specs = sort_specs;
    table->SortSpecs.SpecsCount = table->SortSpecsCount;
}

float Az::TableGetHeaderRowHeight()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    float row_height = g.FontSize;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
        if (AZ_BITARRAY_TESTBIT(table->EnabledMaskByIndex, column_n))
            if ((table->Columns[column_n].Flags
                 & AzTableColumnFlags_NoHeaderLabel)
                == 0)
                row_height = AzMax(
                        row_height,
                        CalcTextSize(TableGetColumnName(table, column_n)).y);
    return row_height + g.Style.CellPadding.y * 2.0f;
}

float Az::TableGetHeaderAngledMaxLabelWidth()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    float width = 0.0f;
    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
        if (AZ_BITARRAY_TESTBIT(table->EnabledMaskByIndex, column_n))
            if (table->Columns[column_n].Flags
                & AzTableColumnFlags_AngledHeader)
                width = AzMax(width,
                              CalcTextSize(TableGetColumnName(table, column_n),
                                           NULL,
                                           true)
                                      .x);
    return width + g.Style.CellPadding.y * 2.0f;
}

void Az::TableHeadersRow()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                table != NULL,
                "Call should only be done while in BeginTable() scope!");
        return;
    }

    if (!table->IsLayoutLocked)
        TableUpdateLayout(table);

    const float row_height = TableGetHeaderRowHeight();
    TableNextRow(AzTableRowFlags_Headers, row_height);
    const float row_y1 = GetCursorScreenPos().y;
    if (table->HostSkipItems)
        return;

    const int columns_count = TableGetColumnCount();
    for (int column_n = 0; column_n < columns_count; column_n++)
    {
        if (!TableSetColumnIndex(column_n))
            continue;

        const char* name = (TableGetColumnFlags(column_n)
                            & AzTableColumnFlags_NoHeaderLabel)
                                   ? ""
                                   : TableGetColumnName(column_n);
        PushID(column_n);
        TableHeader(name);
        PopID();
    }

    AzVec2 mouse_pos = Az::GetMousePos();
    if (IsMouseReleased(1) && TableGetHoveredColumn() == columns_count)
        if (mouse_pos.y >= row_y1 && mouse_pos.y < row_y1 + row_height)
            TableOpenContextMenu(columns_count);
}

void Az::TableHeader(const char* label)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    AzTable* table = g.CurrentTable;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                table != NULL,
                "Call should only be done while in BeginTable() scope!");
        return;
    }

    AZ_ASSERT(table->CurrentColumn != -1);
    const int column_n = table->CurrentColumn;
    AzTableColumn* column = &table->Columns[column_n];

    if (label == NULL)
        label = "";
    const char* label_end = FindRenderedTextEnd(label);
    AzVec2 label_size = CalcTextSize(label, label_end, true);
    AzVec2 label_pos = window->DC.CursorPos;

    AzRect cell_r = TableGetCellBgRect(table, column_n);
    float label_height = AzMax(label_size.y,
                               table->RowMinHeight
                                       - table->RowCellPaddingY * 2.0f);

    float w_arrow = 0.0f;
    float w_sort_text = 0.0f;
    bool sort_arrow = false;
    char sort_order_suf[4] = "";
    const float ARROW_SCALE = 0.65f;
    if ((table->Flags & AzTableFlags_Sortable)
        && !(column->Flags & AzTableColumnFlags_NoSort))
    {
        w_arrow = AzTrunc(g.FontSize * ARROW_SCALE + g.Style.FramePadding.x);
        if (column->SortOrder != -1)
            sort_arrow = true;
        if (column->SortOrder > 0)
        {
            AzFormatString(sort_order_suf,
                           AZ_ARRAYSIZE(sort_order_suf),
                           "%d",
                           column->SortOrder + 1);
            w_sort_text = g.Style.ItemInnerSpacing.x
                          + CalcTextSize(sort_order_suf).x;
        }
    }

    float max_pos_x = label_pos.x + label_size.x + w_sort_text + w_arrow;
    column->ContentMaxXHeadersUsed = AzMax(column->ContentMaxXHeadersUsed,
                                           sort_arrow ? cell_r.Max.x
                                                      : AzMin(max_pos_x,
                                                              cell_r.Max.x));
    column->ContentMaxXHeadersIdeal = AzMax(column->ContentMaxXHeadersIdeal,
                                            max_pos_x);

    AzID id = window->GetID(label);
    AzRect bb(cell_r.Min.x,
              cell_r.Min.y,
              cell_r.Max.x,
              AzMax(cell_r.Max.y,
                    cell_r.Min.y + label_height
                            + g.Style.CellPadding.y * 2.0f));
    ItemSize(AzVec2(0.0f, label_height));
    if (!ItemAdd(bb, id))
        return;

    const bool highlight = (table->HighlightColumnHeader == column_n);
    bool hovered, held;
    bool pressed =
            ButtonBehavior(bb, id, &hovered, &held, AzButtonFlags_AllowOverlap);
    if (held || hovered || highlight)
    {
        const AzU32 col = GetColorU32(held      ? AzCol_HeaderActive
                                      : hovered ? AzCol_HeaderHovered
                                                : AzCol_Header);

        TableSetBgColor(AzTableBgTarget_CellBg, col, table->CurrentColumn);
    } else
    {
        if ((table->RowFlags & AzTableRowFlags_Headers) == 0)
            TableSetBgColor(AzTableBgTarget_CellBg,
                            GetColorU32(AzCol_TableHeaderBg),
                            table->CurrentColumn);
    }
    RenderNavCursor(bb,
                    id,
                    AzNavRenderCursorFlags_Compact
                            | AzNavRenderCursorFlags_NoRounding);
    if (held)
        table->HeldHeaderColumn = (AzTableColumnIdx)column_n;
    window->DC.CursorPos.y -= g.Style.ItemSpacing.y * 0.5f;

    if (held && (table->Flags & AzTableFlags_Reorderable) && IsMouseDragging(0)
        && !g.DragDropActive)
    {
        table->ReorderColumn = (AzTableColumnIdx)column_n;
        table->InstanceInteracted = table->InstanceCurrent;

        if (g.IO.MouseDelta.x < 0.0f && g.IO.MousePos.x < cell_r.Min.x)
            if (AzTableColumn* prev_column =
                        (column->PrevEnabledColumn != -1)
                                ? &table->Columns[column->PrevEnabledColumn]
                                : NULL)
                if (!((column->Flags | prev_column->Flags)
                      & AzTableColumnFlags_NoReorder))
                    if ((column->IndexWithinEnabledSet
                         < table->FreezeColumnsRequest)
                        == (prev_column->IndexWithinEnabledSet
                            < table->FreezeColumnsRequest))
                        table->ReorderColumnDir = -1;
        if (g.IO.MouseDelta.x > 0.0f && g.IO.MousePos.x > cell_r.Max.x)
            if (AzTableColumn* next_column =
                        (column->NextEnabledColumn != -1)
                                ? &table->Columns[column->NextEnabledColumn]
                                : NULL)
                if (!((column->Flags | next_column->Flags)
                      & AzTableColumnFlags_NoReorder))
                    if ((column->IndexWithinEnabledSet
                         < table->FreezeColumnsRequest)
                        == (next_column->IndexWithinEnabledSet
                            < table->FreezeColumnsRequest))
                        table->ReorderColumnDir = +1;
    }

    const float ellipsis_max = AzMax(cell_r.Max.x - w_arrow - w_sort_text,
                                     label_pos.x);
    if ((table->Flags & AzTableFlags_Sortable)
        && !(column->Flags & AzTableColumnFlags_NoSort))
    {
        if (column->SortOrder != -1)
        {
            float x = AzMax(cell_r.Min.x, cell_r.Max.x - w_arrow - w_sort_text);
            float y = label_pos.y;
            if (column->SortOrder > 0)
            {
                PushStyleColor(AzCol_Text, GetColorU32(AzCol_Text, 0.70f));
                RenderText(AzVec2(x + g.Style.ItemInnerSpacing.x, y),
                           sort_order_suf);
                PopStyleColor();
                x += w_sort_text;
            }
            RenderArrow(window->DrawList,
                        AzVec2(x, y),
                        GetColorU32(AzCol_Text),
                        column->SortDirection == AzSortDirection_Ascending
                                ? AzDir_Up
                                : AzDir_Down,
                        ARROW_SCALE);
        }

        if (pressed && table->ReorderColumn != column_n)
        {
            AzSortDirection sort_direction = TableGetColumnNextSortDirection(
                    column);
            TableSetColumnSortDirection(column_n,
                                        sort_direction,
                                        g.IO.KeyShift);
        }
    }

    RenderTextEllipsis(window->DrawList,
                       label_pos,
                       AzVec2(ellipsis_max, bb.Max.y),
                       ellipsis_max,
                       label,
                       label_end,
                       &label_size);

    const bool text_clipped = label_size.x > (ellipsis_max - label_pos.x);
    if (text_clipped && hovered && g.ActiveId == 0)
        SetItemTooltip("%.*s", (int)(label_end - label), label);

    if (IsMouseReleased(1) && IsItemHovered())
        TableOpenContextMenu(column_n);
}

void Az::TableAngledHeadersRow()
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    AzTableTempData* temp_data = table->TempData;
    temp_data->AngledHeadersRequests.resize(0);
    temp_data->AngledHeadersRequests.reserve(table->ColumnsEnabledCount);

    const AzID row_id = GetID("##AngledHeaders");
    AzTableInstanceData* table_instance =
            TableGetInstanceData(table, table->InstanceCurrent);
    int highlight_column_n = table->HighlightColumnHeader;
    if (highlight_column_n == -1 && table->HoveredColumnBody != -1)
        if (table_instance->HoveredRowLast == 0
            && table->HoveredColumnBorder == -1
            && (g.ActiveId == 0 || g.ActiveId == row_id
                || (table->IsActiveIdInTable || g.DragDropActive)))
            highlight_column_n = table->HoveredColumnBody;

    AzU32 col_header_bg = GetColorU32(AzCol_TableHeaderBg);
    AzU32 col_text = GetColorU32(AzCol_Text);
    for (int order_n = 0; order_n < table->ColumnsCount; order_n++)
        if (AZ_BITARRAY_TESTBIT(table->EnabledMaskByDisplayOrder, order_n))
        {
            const int column_n = table->DisplayOrderToIndex[order_n];
            AzTableColumn* column = &table->Columns[column_n];
            if ((column->Flags & AzTableColumnFlags_AngledHeader) == 0)
                continue;
            AzTableHeaderData request = {(AzTableColumnIdx)column_n,
                                         col_text,
                                         col_header_bg,
                                         (column_n == highlight_column_n)
                                                 ? GetColorU32(AzCol_Header)
                                                 : 0};
            temp_data->AngledHeadersRequests.push_back(request);
        }

    TableAngledHeadersRowEx(row_id,
                            g.Style.TableAngledHeadersAngle,
                            0.0f,
                            temp_data->AngledHeadersRequests.Data,
                            temp_data->AngledHeadersRequests.Size);
}

void Az::TableAngledHeadersRowEx(AzID row_id,
                                 float angle,
                                 float max_label_width,
                                 const AzTableHeaderData* data,
                                 int data_count)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    AzWindow* window = g.CurrentWindow;
    AzDrawList* draw_list = window->DrawList;
    if (table == NULL)
    {
        AZ_ASSERT_USER_ERROR(
                table != NULL,
                "Call should only be done while in BeginTable() scope!");
        return;
    }
    AZ_ASSERT(table->CurrentRow == -1 && "Must be first row");

    if (max_label_width == 0.0f)
        max_label_width = TableGetHeaderAngledMaxLabelWidth();

    const bool flip_label = (angle < 0.0f);
    angle -= AZ_PI * 0.5f;
    const float cos_a = AzCos(angle);
    const float sin_a = AzSin(angle);
    const float label_cos_a = flip_label ? AzCos(angle + AZ_PI) : cos_a;
    const float label_sin_a = flip_label ? AzSin(angle + AZ_PI) : sin_a;
    const AzVec2 unit_right = AzVec2(cos_a, sin_a);

    const float header_height = g.FontSize + g.Style.CellPadding.x * 2.0f;
    const float row_height = AzTrunc(AzFabs(
            AzRotate(AzVec2(max_label_width,
                            flip_label ? +header_height : -header_height),
                     cos_a,
                     sin_a)
                    .y));
    table->AngledHeadersHeight = row_height;
    table->AngledHeadersSlope = (sin_a != 0.0f) ? (cos_a / sin_a) : 0.0f;
    const AzVec2 header_angled_vector = unit_right * (row_height / -sin_a);

    TableNextRow(AzTableRowFlags_Headers, row_height);
    TableNextColumn();
    const AzRect row_r(table->WorkRect.Min.x,
                       table->BgClipRect.Min.y,
                       table->WorkRect.Max.x,
                       table->RowPosY2);
    table->DrawSplitter->SetCurrentChannel(draw_list, TABLE_DRAW_CHANNEL_BG0);
    float clip_rect_min_x = table->BgClipRect.Min.x;
    if (table->FreezeColumnsCount > 0)
        clip_rect_min_x =
                AzMax(clip_rect_min_x,
                      table->Columns[table->FreezeColumnsCount - 1].MaxX);
    TableSetBgColor(AzTableBgTarget_RowBg0, 0);
    PushClipRect(table->BgClipRect.Min, table->BgClipRect.Max, false);
    draw_list->AddRectFilled(AzVec2(table->BgClipRect.Min.x, row_r.Min.y),
                             AzVec2(table->BgClipRect.Max.x, row_r.Max.y),
                             GetColorU32(AzCol_TableHeaderBg, 0.25f));
    PushClipRect(AzVec2(clip_rect_min_x, table->BgClipRect.Min.y),
                 table->BgClipRect.Max,
                 true);

    ButtonBehavior(row_r, row_id, NULL, NULL);
    KeepAliveID(row_id);

    const float ascent_scaled = g.FontBaked->Ascent * g.FontBakedScale;
    const float line_off_for_ascent_x =
            (AzMax((g.FontSize - ascent_scaled) * 0.5f, 0.0f) / -sin_a)
            * (flip_label ? -1.0f : 1.0f);
    const AzVec2 padding = g.Style.CellPadding;
    const AzVec2 align = g.Style.TableAngledHeadersTextAlign;

    float max_x = -FLT_MAX;
    for (int pass = 0; pass < 2; pass++)
        for (int order_n = 0; order_n < data_count; order_n++)
        {
            const AzTableHeaderData* request = &data[order_n];
            const int column_n = request->Index;
            AzTableColumn* column = &table->Columns[column_n];

            AzVec2 bg_shape[4];
            bg_shape[0] = AzVec2(column->MaxX, row_r.Max.y);
            bg_shape[1] = AzVec2(column->MinX, row_r.Max.y);
            bg_shape[2] = bg_shape[1] + header_angled_vector;
            bg_shape[3] = bg_shape[0] + header_angled_vector;
            if (pass == 0)
            {
                draw_list->AddQuadFilled(bg_shape[0],
                                         bg_shape[1],
                                         bg_shape[2],
                                         bg_shape[3],
                                         request->BgColor0);
                draw_list->AddQuadFilled(bg_shape[0],
                                         bg_shape[1],
                                         bg_shape[2],
                                         bg_shape[3],
                                         request->BgColor1);
                max_x = AzMax(max_x, bg_shape[3].x);

                const char* label_name = TableGetColumnName(table, column_n);
                const char* label_name_end = FindRenderedTextEnd(label_name);
                const float line_off_step_x = (g.FontSize / -sin_a);
                const int label_lines = AzTextCountLines(label_name,
                                                         label_name_end);

                float line_off_curr_x = flip_label ? (label_lines - 1)
                                                             * line_off_step_x
                                                   : 0.0f;
                float line_off_for_align_x =
                        AzMax((((column->MaxX - column->MinX)
                                - padding.x * 2.0f)
                               - (label_lines * line_off_step_x)),
                              0.0f)
                        * align.x;
                line_off_curr_x += line_off_for_align_x - line_off_for_ascent_x;

                column->ContentMaxXHeadersUsed =
                        column->ContentMaxXHeadersIdeal =
                                column->WorkMinX
                                + AzCeil(label_lines * line_off_step_x
                                         - line_off_for_align_x);

                while (label_name < label_name_end)
                {
                    const char* label_name_eol = strchr(label_name, '\n');
                    if (label_name_eol == NULL)
                        label_name_eol = label_name_end;

                    AzVec2 label_size = CalcTextSize(label_name,
                                                     label_name_eol);
                    float clip_width = max_label_width - padding.y;
                    float clip_height = AzMin(label_size.y,
                                              column->ClipRect.Max.x
                                                      - column->WorkMinX
                                                      - line_off_curr_x);
                    AzRect clip_r(window->ClipRect.Min,
                                  window->ClipRect.Min
                                          + AzVec2(clip_width, clip_height));
                    int vtx_idx_begin = draw_list->_VtxCurrentIdx;
                    PushStyleColor(AzCol_Text, request->TextColor);
                    RenderTextEllipsis(draw_list,
                                       clip_r.Min,
                                       clip_r.Max,
                                       clip_r.Max.x,
                                       label_name,
                                       label_name_eol,
                                       &label_size);
                    PopStyleColor();
                    int vtx_idx_end = draw_list->_VtxCurrentIdx;

                    const float available_space =
                            AzMax(clip_width - label_size.x
                                          + AzAbs(padding.x * cos_a) * 2.0f
                                          - AzAbs(padding.y * sin_a) * 2.0f,
                                  0.0f);
                    const float vertical_offset = available_space * align.y
                                                  * (flip_label ? -1.0f : 1.0f);

                    AzVec2 pivot_in =
                            AzVec2(window->ClipRect.Min.x - vertical_offset,
                                   window->ClipRect.Min.y + label_size.y);
                    AzVec2 pivot_out = AzVec2(column->WorkMinX, row_r.Max.y);
                    line_off_curr_x += flip_label ? -line_off_step_x
                                                  : line_off_step_x;
                    pivot_out += unit_right * padding.y;
                    if (flip_label)
                        pivot_out += unit_right
                                     * (clip_width
                                        - AzMax(0.0f,
                                                clip_width - label_size.x));
                    pivot_out.x += flip_label
                                           ? line_off_curr_x + line_off_step_x
                                           : line_off_curr_x;
                    ShadeVertsTransformPos(draw_list,
                                           vtx_idx_begin,
                                           vtx_idx_end,
                                           pivot_in,
                                           label_cos_a,
                                           label_sin_a,
                                           pivot_out);

                    label_name = label_name_eol + 1;
                }
            }
            if (pass == 1)
            {
                draw_list->AddLine(bg_shape[0],
                                   bg_shape[3],
                                   TableGetColumnBorderCol(table,
                                                           order_n,
                                                           column_n));
            }
        }
    PopClipRect();
    PopClipRect();
    table->TempData->AngledHeadersExtraWidth =
            AzMax(0.0f,
                  max_x - table->Columns[table->RightMostEnabledColumn].MaxX);
}

void Az::TableOpenContextMenu(int column_n)
{
    AzContext& g = *GAz;
    AzTable* table = g.CurrentTable;
    if (column_n == -1 && table->CurrentColumn != -1)
        column_n = table->CurrentColumn;
    if (column_n == table->ColumnsCount)
        column_n = -1;
    AZ_ASSERT(column_n >= -1 && column_n < table->ColumnsCount);
    if (table->Flags
        & (AzTableFlags_Resizable | AzTableFlags_Reorderable
           | AzTableFlags_Hideable))
    {
        table->IsContextPopupOpen = true;
        table->ContextPopupColumn = (AzTableColumnIdx)column_n;
        table->InstanceInteracted = table->InstanceCurrent;
        const AzID context_menu_id = AzHashStr("##ContextMenu", 0, table->ID);
        OpenPopupEx(context_menu_id, AzPopupFlags_None);
    }
}

bool Az::TableBeginContextMenuPopup(AzTable* table)
{
    if (!table->IsContextPopupOpen
        || table->InstanceCurrent != table->InstanceInteracted)
        return false;
    const AzID context_menu_id = AzHashStr("##ContextMenu", 0, table->ID);
    if (BeginPopupEx(context_menu_id,
                     AzWindowFlags_AlwaysAutoResize | AzWindowFlags_NoTitleBar
                             | AzWindowFlags_NoSavedSettings))
        return true;
    table->IsContextPopupOpen = false;
    return false;
}

void Az::TableDrawDefaultContextMenu(AzTable* table,
                                     AzTableFlags flags_for_section_to_display)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    bool want_separator = false;
    const int column_n = (table->ContextPopupColumn >= 0
                          && table->ContextPopupColumn < table->ColumnsCount)
                                 ? table->ContextPopupColumn
                                 : -1;
    AzTableColumn* column = (column_n != -1) ? &table->Columns[column_n] : NULL;

    if (flags_for_section_to_display & AzTableFlags_Resizable)
    {
        if (column != NULL)
        {
            const bool can_resize = !(column->Flags
                                      & AzTableColumnFlags_NoResize)
                                    && column->IsEnabled;
            if (MenuItem(LocalizeGetMsg(AzLocKey_TableSizeOne),
                         NULL,
                         false,
                         can_resize))
                TableSetColumnWidthAutoSingle(table, column_n);
        }

        const char* size_all_desc;
        if (table->ColumnsEnabledFixedCount == table->ColumnsEnabledCount
            && (table->Flags & AzTableFlags_SizingMask_)
                       != AzTableFlags_SizingFixedSame)
            size_all_desc = LocalizeGetMsg(AzLocKey_TableSizeAllFit);
        else
            size_all_desc = LocalizeGetMsg(AzLocKey_TableSizeAllDefault);
        if (MenuItem(size_all_desc, NULL))
            TableSetColumnWidthAutoAll(table);
        want_separator = true;
    }

    if (flags_for_section_to_display & AzTableFlags_Reorderable)
    {
        if (MenuItem(LocalizeGetMsg(AzLocKey_TableResetOrder),
                     NULL,
                     false,
                     !table->IsDefaultDisplayOrder))
            table->IsResetDisplayOrderRequest = true;
        want_separator = true;
    }

#if 0
    if ((flags_for_section_to_display & AzTableFlags_Sortable) && column != NULL && (column->Flags & AzTableColumnFlags_NoSort) == 0)
    {
        if (want_separator)
            Separator();
        want_separator = true;

        bool append_to_sort_specs = g.IO.KeyShift;
        if (MenuItem("Sort in Ascending Order", NULL, column->SortOrder != -1 && column->SortDirection == AzSortDirection_Ascending, (column->Flags & AzTableColumnFlags_NoSortAscending) == 0))
            TableSetColumnSortDirection(table, column_n, AzSortDirection_Ascending, append_to_sort_specs);
        if (MenuItem("Sort in Descending Order", NULL, column->SortOrder != -1 && column->SortDirection == AzSortDirection_Descending, (column->Flags & AzTableColumnFlags_NoSortDescending) == 0))
            TableSetColumnSortDirection(table, column_n, AzSortDirection_Descending, append_to_sort_specs);
    }
#endif

    if (flags_for_section_to_display & AzTableFlags_Hideable)
    {
        if (want_separator)
            Separator();
        want_separator = true;

        PushItemFlag(AzItemFlags_AutoClosePopups, false);
        for (int other_column_n = 0; other_column_n < table->ColumnsCount;
             other_column_n++)
        {
            AzTableColumn* other_column = &table->Columns[other_column_n];
            if (other_column->Flags & AzTableColumnFlags_Disabled)
                continue;

            const char* name = TableGetColumnName(table, other_column_n);
            if (name == NULL || name[0] == 0)
                name = "<Unknown>";

            bool menu_item_active = (other_column->Flags
                                     & AzTableColumnFlags_NoHide)
                                            ? false
                                            : true;
            if (other_column->IsUserEnabled && table->ColumnsEnabledCount <= 1)
                menu_item_active = false;
            if (MenuItem(name,
                         NULL,
                         other_column->IsUserEnabled,
                         menu_item_active))
                other_column->IsUserEnabledNextFrame =
                        !other_column->IsUserEnabled;
        }
        PopItemFlag();
    }
}

static void TableSettingsInit(AzTableSettings* settings,
                              AzID id,
                              int columns_count,
                              int columns_count_max)
{
    AZ_PLACEMENT_NEW(settings) AzTableSettings();
    AzTableColumnSettings* settings_column = settings->GetColumnSettings();
    for (int n = 0; n < columns_count_max; n++, settings_column++)
        AZ_PLACEMENT_NEW(settings_column) AzTableColumnSettings();
    settings->ID = id;
    settings->ColumnsCount = (AzTableColumnIdx)columns_count;
    settings->ColumnsCountMax = (AzTableColumnIdx)columns_count_max;
    settings->WantApply = true;
}

static size_t TableSettingsCalcChunkSize(int columns_count)
{
    return sizeof(AzTableSettings)
           + (size_t)columns_count * sizeof(AzTableColumnSettings);
}

AzTableSettings* Az::TableSettingsCreate(AzID id, int columns_count)
{
    AzContext& g = *GAz;
    AzTableSettings* settings = g.SettingsTables.alloc_chunk(
            TableSettingsCalcChunkSize(columns_count));
    TableSettingsInit(settings, id, columns_count, columns_count);
    return settings;
}

AzTableSettings* Az::TableSettingsFindByID(AzID id)
{
    AzContext& g = *GAz;
    for (AzTableSettings* settings = g.SettingsTables.begin(); settings != NULL;
         settings = g.SettingsTables.next_chunk(settings))
        if (settings->ID == id)
            return settings;
    return NULL;
}

AzTableSettings* Az::TableGetBoundSettings(AzTable* table)
{
    if (table->SettingsOffset != -1)
    {
        AzContext& g = *GAz;
        AzTableSettings* settings = g.SettingsTables.ptr_from_offset(
                table->SettingsOffset);
        AZ_ASSERT(settings->ID == table->ID);
        if (settings->ColumnsCountMax >= table->ColumnsCount)
            return settings;
        settings->ID = 0;
    }
    return NULL;
}

void Az::TableResetSettings(AzTable* table)
{
    table->IsInitializing = table->IsSettingsDirty = true;
    table->IsResetAllRequest = false;
    table->IsSettingsRequestLoad = false;
    table->SettingsLoadedFlags = AzTableFlags_None;
}

void Az::TableSaveSettings(AzTable* table)
{
    table->IsSettingsDirty = false;
    if (table->Flags & AzTableFlags_NoSavedSettings)
        return;

    AzContext& g = *GAz;
    AzTableSettings* settings = TableGetBoundSettings(table);
    if (settings == NULL)
    {
        settings = TableSettingsCreate(table->ID, table->ColumnsCount);
        table->SettingsOffset = g.SettingsTables.offset_from_ptr(settings);
    }
    settings->ColumnsCount = (AzTableColumnIdx)table->ColumnsCount;

    AZ_ASSERT(settings->ID == table->ID);
    AZ_ASSERT(settings->ColumnsCount == table->ColumnsCount
              && settings->ColumnsCountMax >= settings->ColumnsCount);
    AzTableColumn* column = table->Columns.Data;
    AzTableColumnSettings* column_settings = settings->GetColumnSettings();

    bool save_ref_scale = false;
    settings->SaveFlags = AzTableFlags_None;
    for (int n = 0; n < table->ColumnsCount; n++, column++, column_settings++)
    {
        const float width_or_weight = (column->Flags
                                       & AzTableColumnFlags_WidthStretch)
                                              ? column->StretchWeight
                                              : column->WidthRequest;
        column_settings->WidthOrWeight = width_or_weight;
        column_settings->Index = (AzTableColumnIdx)n;
        column_settings->DisplayOrder = column->DisplayOrder;
        column_settings->SortOrder = column->SortOrder;
        column_settings->SortDirection = column->SortDirection;
        column_settings->IsEnabled = column->IsUserEnabled;
        column_settings->IsStretch = (column->Flags
                                      & AzTableColumnFlags_WidthStretch)
                                             ? 1
                                             : 0;
        if ((column->Flags & AzTableColumnFlags_WidthStretch) == 0)
            save_ref_scale = true;

        if (width_or_weight != column->InitStretchWeightOrWidth)
            settings->SaveFlags |= AzTableFlags_Resizable;
        if (column->DisplayOrder != n)
            settings->SaveFlags |= AzTableFlags_Reorderable;
        if (column->SortOrder != -1)
            settings->SaveFlags |= AzTableFlags_Sortable;
        if (column->IsUserEnabled
            != ((column->Flags & AzTableColumnFlags_DefaultHide) == 0))
            settings->SaveFlags |= AzTableFlags_Hideable;
    }
    settings->SaveFlags &= table->Flags;
    settings->RefScale = save_ref_scale ? table->RefScale : 0.0f;

    MarkIniSettingsDirty();
}

void Az::TableLoadSettings(AzTable* table)
{
    AzContext& g = *GAz;
    table->IsSettingsRequestLoad = false;
    if (table->Flags & AzTableFlags_NoSavedSettings)
        return;

    AzTableSettings* settings;
    if (table->SettingsOffset == -1)
    {
        settings = TableSettingsFindByID(table->ID);
        if (settings == NULL)
            return;
        if (settings->ColumnsCount != table->ColumnsCount)
            table->IsSettingsDirty = true;
        table->SettingsOffset = g.SettingsTables.offset_from_ptr(settings);
    } else
    {
        settings = TableGetBoundSettings(table);
    }

    table->SettingsLoadedFlags = settings->SaveFlags;
    table->RefScale = settings->RefScale;

    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
    {
        AzTableColumn* column = &table->Columns[column_n];
        TableInitColumnDefaults(table, column, ~0);
        column->AutoFitQueue = 0x00;
    }

    AzTableColumnSettings* column_settings = settings->GetColumnSettings();
    AzU64 display_order_mask = 0;
    for (int data_n = 0; data_n < settings->ColumnsCount;
         data_n++, column_settings++)
    {
        int column_n = column_settings->Index;
        if (column_n < 0 || column_n >= table->ColumnsCount)
            continue;

        AzTableColumn* column = &table->Columns[column_n];
        if (settings->SaveFlags & AzTableFlags_Resizable)
        {
            if (column_settings->IsStretch)
                column->StretchWeight = column_settings->WidthOrWeight;
            else
                column->WidthRequest = column_settings->WidthOrWeight;
        }
        if (settings->SaveFlags & AzTableFlags_Reorderable)
            column->DisplayOrder = column_settings->DisplayOrder;
        display_order_mask |= (AzU64)1 << column->DisplayOrder;
        if ((settings->SaveFlags & AzTableFlags_Hideable)
            && column_settings->IsEnabled != -1)
            column->IsUserEnabled = column->IsUserEnabledNextFrame =
                    column_settings->IsEnabled == 1;
        column->SortOrder = column_settings->SortOrder;
        column->SortDirection = column_settings->SortDirection;
    }

    const AzU64 expected_display_order_mask =
            (settings->ColumnsCount == 64)
                    ? ~0
                    : ((AzU64)1 << settings->ColumnsCount) - 1;
    if (display_order_mask != expected_display_order_mask)
        for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
            table->Columns[column_n].DisplayOrder = (AzTableColumnIdx)column_n;

    for (int column_n = 0; column_n < table->ColumnsCount; column_n++)
        table->DisplayOrderToIndex[table->Columns[column_n].DisplayOrder] =
                (AzTableColumnIdx)column_n;
}

static void TableSettingsHandler_ClearAll(AzContext* ctx, AzSettingsHandler*)
{
    AzContext& g = *ctx;
    for (int i = 0; i != g.Tables.GetMapSize(); i++)
        if (AzTable* table = g.Tables.TryGetMapData(i))
            table->SettingsOffset = -1;
    g.SettingsTables.clear();
}

static void TableSettingsHandler_ApplyAll(AzContext* ctx, AzSettingsHandler*)
{
    AzContext& g = *ctx;
    for (int i = 0; i != g.Tables.GetMapSize(); i++)
        if (AzTable* table = g.Tables.TryGetMapData(i))
        {
            table->IsSettingsRequestLoad = true;
            table->SettingsOffset = -1;
        }
}

static void* TableSettingsHandler_ReadOpen(AzContext*,
                                           AzSettingsHandler*,
                                           const char* name)
{
    AzID id = 0;
    int columns_count = 0;
    if (sscanf(name, "0x%08X,%d", &id, &columns_count) < 2)
        return NULL;

    if (AzTableSettings* settings = Az::TableSettingsFindByID(id))
    {
        if (settings->ColumnsCountMax >= columns_count)
        {
            TableSettingsInit(settings,
                              id,
                              columns_count,
                              settings->ColumnsCountMax);
            return settings;
        }
        settings->ID = 0;
    }
    return Az::TableSettingsCreate(id, columns_count);
}

static void TableSettingsHandler_ReadLine(AzContext*,
                                          AzSettingsHandler*,
                                          void* entry,
                                          const char* line)
{
    AzTableSettings* settings = (AzTableSettings*)entry;
    float f = 0.0f;
    int column_n = 0, r = 0, n = 0;

    if (sscanf(line, "RefScale=%f", &f) == 1)
    {
        settings->RefScale = f;
        return;
    }

    if (sscanf(line, "Column %d%n", &column_n, &r) == 1)
    {
        if (column_n < 0 || column_n >= settings->ColumnsCount)
            return;
        line = AzStrSkipBlank(line + r);
        char c = 0;
        AzTableColumnSettings* column = settings->GetColumnSettings()
                                        + column_n;
        column->Index = (AzTableColumnIdx)column_n;
        if (sscanf(line, "UserID=0x%08X%n", (AzU32*)&n, &r) == 1)
        {
            line = AzStrSkipBlank(line + r);
            column->UserID = (AzID)n;
        }
        if (sscanf(line, "Width=%d%n", &n, &r) == 1)
        {
            line = AzStrSkipBlank(line + r);
            column->WidthOrWeight = (float)n;
            column->IsStretch = 0;
            settings->SaveFlags |= AzTableFlags_Resizable;
        }
        if (sscanf(line, "Weight=%f%n", &f, &r) == 1)
        {
            line = AzStrSkipBlank(line + r);
            column->WidthOrWeight = f;
            column->IsStretch = 1;
            settings->SaveFlags |= AzTableFlags_Resizable;
        }
        if (sscanf(line, "Visible=%d%n", &n, &r) == 1)
        {
            line = AzStrSkipBlank(line + r);
            column->IsEnabled = (AzU8)n;
            settings->SaveFlags |= AzTableFlags_Hideable;
        }
        if (sscanf(line, "Order=%d%n", &n, &r) == 1)
        {
            line = AzStrSkipBlank(line + r);
            column->DisplayOrder = (AzTableColumnIdx)n;
            settings->SaveFlags |= AzTableFlags_Reorderable;
        }
        if (sscanf(line, "Sort=%d%c%n", &n, &c, &r) == 2)
        {
            line = AzStrSkipBlank(line + r);
            column->SortOrder = (AzTableColumnIdx)n;
            column->SortDirection = (c == '^') ? AzSortDirection_Descending
                                               : AzSortDirection_Ascending;
            settings->SaveFlags |= AzTableFlags_Sortable;
        }
    }
}

static void TableSettingsHandler_WriteAll(AzContext* ctx,
                                          AzSettingsHandler* handler,
                                          AzTextBuffer* buf)
{
    AzContext& g = *ctx;
    for (AzTableSettings* settings = g.SettingsTables.begin(); settings != NULL;
         settings = g.SettingsTables.next_chunk(settings))
    {
        if (settings->ID == 0)
            continue;

        const bool save_size = (settings->SaveFlags & AzTableFlags_Resizable)
                               != 0;
        const bool save_visible = (settings->SaveFlags & AzTableFlags_Hideable)
                                  != 0;
        const bool save_order = (settings->SaveFlags & AzTableFlags_Reorderable)
                                != 0;
        const bool save_sort = (settings->SaveFlags & AzTableFlags_Sortable)
                               != 0;

        buf->reserve(buf->size() + 30 + settings->ColumnsCount * 50);
        buf->appendf("[%s][0x%08X,%d]\n",
                     handler->TypeName,
                     settings->ID,
                     settings->ColumnsCount);
        if (settings->RefScale != 0.0f)
            buf->appendf("RefScale=%g\n", settings->RefScale);
        AzTableColumnSettings* column = settings->GetColumnSettings();
        for (int column_n = 0; column_n < settings->ColumnsCount;
             column_n++, column++)
        {
            bool save_column = column->UserID != 0 || save_size || save_visible
                               || save_order
                               || (save_sort && column->SortOrder != -1);
            if (!save_column)
                continue;
            buf->appendf("Column %-2d", column_n);
            if (column->UserID != 0)
            {
                buf->appendf(" UserID=%08X", column->UserID);
            }
            if (save_size && column->IsStretch)
            {
                buf->appendf(" Weight=%.4f", column->WidthOrWeight);
            }
            if (save_size && !column->IsStretch)
            {
                buf->appendf(" Width=%d", (int)column->WidthOrWeight);
            }
            if (save_visible)
            {
                buf->appendf(" Visible=%d", column->IsEnabled);
            }
            if (save_order)
            {
                buf->appendf(" Order=%d", column->DisplayOrder);
            }
            if (save_sort && column->SortOrder != -1)
            {
                buf->appendf(" Sort=%d%c",
                             column->SortOrder,
                             (column->SortDirection
                              == AzSortDirection_Ascending)
                                     ? 'v'
                                     : '^');
            }
            buf->append("\n");
        }
        buf->append("\n");
    }
}

void Az::TableSettingsAddSettingsHandler()
{
    AzSettingsHandler ini_handler;
    ini_handler.TypeName = "Table";
    ini_handler.TypeHash = AzHashStr("Table");
    ini_handler.ClearAllFn = TableSettingsHandler_ClearAll;
    ini_handler.ReadOpenFn = TableSettingsHandler_ReadOpen;
    ini_handler.ReadLineFn = TableSettingsHandler_ReadLine;
    ini_handler.ApplyAllFn = TableSettingsHandler_ApplyAll;
    ini_handler.WriteAllFn = TableSettingsHandler_WriteAll;
    AddSettingsHandler(&ini_handler);
}

void Az::TableRemove(AzTable* table)
{
    AzContext& g = *GAz;
    int table_idx = g.Tables.GetIndex(table);

    g.Tables.Remove(table->ID, table);
    g.TablesLastTimeActive[table_idx] = -1.0f;
}

void Az::TableGcCompactTransientBuffers(AzTable* table)
{
    AzContext& g = *GAz;
    AZ_ASSERT(table->MemoryCompacted == false);
    table->SortSpecs.Specs = NULL;
    table->SortSpecsMulti.clear();
    table->IsSortSpecsDirty = true;
    table->ColumnsNames.clear();
    table->MemoryCompacted = true;
    for (int n = 0; n < table->ColumnsCount; n++)
        table->Columns[n].NameOffset = -1;
    g.TablesLastTimeActive[g.Tables.GetIndex(table)] = -1.0f;
}

void Az::TableGcCompactTransientBuffers(AzTableTempData* temp_data)
{
    temp_data->DrawSplitter.ClearFreeMemory();
    temp_data->LastTimeActive = -1.0f;
}

void Az::TableGcCompactSettings()
{
    AzContext& g = *GAz;
    int required_memory = 0;
    for (AzTableSettings* settings = g.SettingsTables.begin(); settings != NULL;
         settings = g.SettingsTables.next_chunk(settings))
        if (settings->ID != 0)
            required_memory += (int)TableSettingsCalcChunkSize(
                    settings->ColumnsCount);
    if (required_memory == g.SettingsTables.Buf.Size)
        return;
    AzChunkStream<AzTableSettings> new_chunk_stream;
    new_chunk_stream.Buf.reserve(required_memory);
    for (AzTableSettings* settings = g.SettingsTables.begin(); settings != NULL;
         settings = g.SettingsTables.next_chunk(settings))
        if (settings->ID != 0)
            memcpy(new_chunk_stream.alloc_chunk(
                           TableSettingsCalcChunkSize(settings->ColumnsCount)),
                   settings,
                   TableSettingsCalcChunkSize(settings->ColumnsCount));
    g.SettingsTables.swap(new_chunk_stream);
}

#ifndef AZ_DISABLE_DEBUG_TOOLS

static const char* DebugNodeTableGetSizingPolicyDesc(AzTableFlags sizing_policy)
{
    sizing_policy &= AzTableFlags_SizingMask_;
    if (sizing_policy == AzTableFlags_SizingFixedFit)
    {
        return "FixedFit";
    }
    if (sizing_policy == AzTableFlags_SizingFixedSame)
    {
        return "FixedSame";
    }
    if (sizing_policy == AzTableFlags_SizingStretchProp)
    {
        return "StretchProp";
    }
    if (sizing_policy == AzTableFlags_SizingStretchSame)
    {
        return "StretchSame";
    }
    return "N/A";
}

void Az::DebugNodeTable(AzTable* table)
{
    AzContext& g = *GAz;
    const bool is_active = (table->LastFrameActive >= g.FrameCount - 2);
    if (!is_active)
    {
        PushStyleColor(AzCol_Text, GetStyleColorVec4(AzCol_TextDisabled));
    }
    bool open = TreeNode(table,
                         "Table 0x%08X (%d columns, in '%s')%s",
                         table->ID,
                         table->ColumnsCount,
                         table->OuterWindow->Name,
                         is_active ? "" : " *Inactive*");
    if (!is_active)
    {
        PopStyleColor();
    }
    if (IsItemHovered())
        GetForegroundDrawList()->AddRect(table->OuterRect.Min,
                                         table->OuterRect.Max,
                                         AZ_COL32(255, 255, 0, 255));
    if (IsItemVisible() && table->HoveredColumnBody != -1)
        GetForegroundDrawList()->AddRect(GetItemRectMin(),
                                         GetItemRectMax(),
                                         AZ_COL32(255, 255, 0, 255));
    if (!open)
        return;
    if (table->InstanceCurrent > 0)
        Text("** %d instances of same table! Some data below will refer to "
             "last instance.",
             table->InstanceCurrent + 1);
    if (g.IO.ConfigDebugIsDebuggerPresent)
    {
        if (DebugBreakButton("**DebugBreak**", "in BeginTable()"))
            g.DebugBreakInTable = table->ID;
        SameLine();
    }

    bool clear_settings = SmallButton("Clear settings");
    BulletText("OuterRect: Pos: (%.1f,%.1f) Size: (%.1f,%.1f) Sizing: '%s'",
               table->OuterRect.Min.x,
               table->OuterRect.Min.y,
               table->OuterRect.GetWidth(),
               table->OuterRect.GetHeight(),
               DebugNodeTableGetSizingPolicyDesc(table->Flags));
    BulletText(
            "ColumnsGivenWidth: %.1f, ColumnsAutoFitWidth: %.1f, InnerWidth: "
            "%.1f%s",
            table->ColumnsGivenWidth,
            table->ColumnsAutoFitWidth,
            table->InnerWidth,
            table->InnerWidth == 0.0f ? " (auto)" : "");
    BulletText(
            "CellPaddingX: %.1f, CellSpacingX: %.1f/%.1f, OuterPaddingX: %.1f",
            table->CellPaddingX,
            table->CellSpacingX1,
            table->CellSpacingX2,
            table->OuterPaddingX);
    BulletText("HoveredColumnBody: %d, HoveredColumnBorder: %d",
               table->HoveredColumnBody,
               table->HoveredColumnBorder);
    BulletText("ResizedColumn: %d, ReorderColumn: %d, HeldHeaderColumn: %d",
               table->ResizedColumn,
               table->ReorderColumn,
               table->HeldHeaderColumn);
    for (int n = 0; n < table->InstanceCurrent + 1; n++)
    {
        AzTableInstanceData* table_instance = TableGetInstanceData(table, n);
        BulletText("Instance %d: HoveredRow: %d, LastOuterHeight: %.2f",
                   n,
                   table_instance->HoveredRowLast,
                   table_instance->LastOuterHeight);
    }

    float sum_weights = 0.0f;
    for (int n = 0; n < table->ColumnsCount; n++)
        if (table->Columns[n].Flags & AzTableColumnFlags_WidthStretch)
            sum_weights += table->Columns[n].StretchWeight;
    for (int n = 0; n < table->ColumnsCount; n++)
    {
        AzTableColumn* column = &table->Columns[n];
        const char* name = TableGetColumnName(table, n);
        char buf[512];
        AzFormatString(buf,
                       AZ_ARRAYSIZE(buf),
                       "Column %d order %d '%s': offset %+.2f to %+.2f%s\n"
                       "Enabled: %d, VisibleX/Y: %d/%d, RequestOutput: %d, "
                       "SkipItems: %d, DrawChannels: %d,%d\n"
                       "WidthGiven: %.1f, Request/Auto: %.1f/%.1f, "
                       "StretchWeight: %.3f (%.1f%%)\n"
                       "MinX: %.1f, MaxX: %.1f (%+.1f), ClipRect: %.1f to %.1f "
                       "(+%.1f)\n"
                       "ContentWidth: %.1f,%.1f, HeadersUsed/Ideal %.1f/%.1f\n"
                       "Sort: %d%s, UserID: 0x%08X, Flags: 0x%04X: %s%s%s..",
                       n,
                       column->DisplayOrder,
                       name,
                       column->MinX - table->WorkRect.Min.x,
                       column->MaxX - table->WorkRect.Min.x,
                       (n < table->FreezeColumnsRequest) ? " (Frozen)" : "",
                       column->IsEnabled,
                       column->IsVisibleX,
                       column->IsVisibleY,
                       column->IsRequestOutput,
                       column->IsSkipItems,
                       column->DrawChannelFrozen,
                       column->DrawChannelUnfrozen,
                       column->WidthGiven,
                       column->WidthRequest,
                       column->WidthAuto,
                       column->StretchWeight,
                       column->StretchWeight > 0.0f
                               ? (column->StretchWeight / sum_weights) * 100.0f
                               : 0.0f,
                       column->MinX,
                       column->MaxX,
                       column->MaxX - column->MinX,
                       column->ClipRect.Min.x,
                       column->ClipRect.Max.x,
                       column->ClipRect.Max.x - column->ClipRect.Min.x,
                       column->ContentMaxXFrozen - column->WorkMinX,
                       column->ContentMaxXUnfrozen - column->WorkMinX,
                       column->ContentMaxXHeadersUsed - column->WorkMinX,
                       column->ContentMaxXHeadersIdeal - column->WorkMinX,
                       column->SortOrder,
                       (column->SortDirection == AzSortDirection_Ascending)
                               ? " (Asc)"
                       : (column->SortDirection == AzSortDirection_Descending)
                               ? " (Des)"
                               : "",
                       column->UserID,
                       column->Flags,
                       (column->Flags & AzTableColumnFlags_WidthStretch)
                               ? "WidthStretch "
                               : "",
                       (column->Flags & AzTableColumnFlags_WidthFixed)
                               ? "WidthFixed "
                               : "",
                       (column->Flags & AzTableColumnFlags_NoResize)
                               ? "NoResize "
                               : "");
        Bullet();
        Selectable(buf);
        if (IsItemHovered())
        {
            AzRect r(column->MinX,
                     table->OuterRect.Min.y,
                     column->MaxX,
                     table->OuterRect.Max.y);
            GetForegroundDrawList()->AddRect(r.Min,
                                             r.Max,
                                             AZ_COL32(255, 255, 0, 255));
        }
    }
    if (AzTableSettings* settings = TableGetBoundSettings(table))
        DebugNodeTableSettings(settings);
    if (clear_settings)
        table->IsResetAllRequest = true;
    TreePop();
}

void Az::DebugNodeTableSettings(AzTableSettings* settings)
{
    if (!TreeNode((void*)(intptr_t)settings->ID,
                  "Settings 0x%08X (%d columns)",
                  settings->ID,
                  settings->ColumnsCount))
        return;
    BulletText("SaveFlags: 0x%08X", settings->SaveFlags);
    BulletText("ColumnsCount: %d (max %d)",
               settings->ColumnsCount,
               settings->ColumnsCountMax);
    for (int n = 0; n < settings->ColumnsCount; n++)
    {
        AzTableColumnSettings* column_settings =
                &settings->GetColumnSettings()[n];
        AzSortDirection sort_dir =
                (column_settings->SortOrder != -1)
                        ? (AzSortDirection)column_settings->SortDirection
                        : AzSortDirection_None;
        BulletText(
                "Column %d Order %d SortOrder %d %s Vis %d %s %7.3f UserID "
                "0x%08X",
                n,
                column_settings->DisplayOrder,
                column_settings->SortOrder,
                (sort_dir == AzSortDirection_Ascending)    ? "Asc"
                : (sort_dir == AzSortDirection_Descending) ? "Des"
                                                           : "---",
                column_settings->IsEnabled,
                column_settings->IsStretch ? "Weight" : "Width ",
                column_settings->WidthOrWeight,
                column_settings->UserID);
    }
    TreePop();
}

#else

void Az::DebugNodeTable(AzTable*)
{}
void Az::DebugNodeTableSettings(AzTableSettings*)
{}

#endif

void Az::SetWindowClipRectBeforeSetChannel(AzWindow* window,
                                           const AzRect& clip_rect)
{
    AzVec4 clip_rect_vec4 = clip_rect.ToVec4();
    window->ClipRect = clip_rect;
    window->DrawList->_CmdHeader.ClipRect = clip_rect_vec4;
    window->DrawList->_ClipRectStack
            .Data[window->DrawList->_ClipRectStack.Size - 1] = clip_rect_vec4;
}

int Az::GetColumnIndex()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CurrentColumns ? window->DC.CurrentColumns->Current : 0;
}

int Az::GetColumnsCount()
{
    AzWindow* window = GetCurrentWindowRead();
    return window->DC.CurrentColumns ? window->DC.CurrentColumns->Count : 1;
}

float Az::GetColumnOffsetFromNorm(const AzOldColumns* columns,
                                  float offset_norm)
{
    return offset_norm * (columns->OffMaxX - columns->OffMinX);
}

float Az::GetColumnNormFromOffset(const AzOldColumns* columns, float offset)
{
    return offset / (columns->OffMaxX - columns->OffMinX);
}

static const float COLUMNS_HIT_RECT_HALF_THICKNESS = 4.0f;

static float GetDraggedColumnOffset(AzOldColumns* columns, int column_index)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AZ_ASSERT(column_index > 0);
    AZ_ASSERT(g.ActiveId == columns->ID + AzID(column_index));

    float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x
              + AzTrunc(COLUMNS_HIT_RECT_HALF_THICKNESS * g.CurrentDpiScale)
              - window->Pos.x;
    x = AzMax(x,
              Az::GetColumnOffset(column_index - 1)
                      + g.Style.ColumnsMinSpacing);
    if ((columns->Flags & AzOldColumnFlags_NoPreserveWidths))
        x = AzMin(x,
                  Az::GetColumnOffset(column_index + 1)
                          - g.Style.ColumnsMinSpacing);

    return x;
}

float Az::GetColumnOffset(int column_index)
{
    AzWindow* window = GetCurrentWindowRead();
    AzOldColumns* columns = window->DC.CurrentColumns;
    if (columns == NULL)
        return 0.0f;

    if (column_index < 0)
        column_index = columns->Current;
    AZ_ASSERT(column_index < columns->Columns.Size);

    const float t = columns->Columns[column_index].OffsetNorm;
    const float x_offset = AzLerp(columns->OffMinX, columns->OffMaxX, t);
    return x_offset;
}

static float GetColumnWidthEx(AzOldColumns* columns,
                              int column_index,
                              bool before_resize = false)
{
    if (column_index < 0)
        column_index = columns->Current;

    float offset_norm;
    if (before_resize)
        offset_norm = columns->Columns[column_index + 1].OffsetNormBeforeResize
                      - columns->Columns[column_index].OffsetNormBeforeResize;
    else
        offset_norm = columns->Columns[column_index + 1].OffsetNorm
                      - columns->Columns[column_index].OffsetNorm;
    return Az::GetColumnOffsetFromNorm(columns, offset_norm);
}

float Az::GetColumnWidth(int column_index)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzOldColumns* columns = window->DC.CurrentColumns;
    if (columns == NULL)
        return GetContentRegionAvail().x;

    if (column_index < 0)
        column_index = columns->Current;
    return GetColumnOffsetFromNorm(
            columns,
            columns->Columns[column_index + 1].OffsetNorm
                    - columns->Columns[column_index].OffsetNorm);
}

void Az::SetColumnOffset(int column_index, float offset)
{
    AzContext& g = *GAz;
    AzWindow* window = g.CurrentWindow;
    AzOldColumns* columns = window->DC.CurrentColumns;
    AZ_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    AZ_ASSERT(column_index < columns->Columns.Size);

    const bool preserve_width = !(columns->Flags
                                  & AzOldColumnFlags_NoPreserveWidths)
                                && (column_index < columns->Count - 1);
    const float width = preserve_width
                                ? GetColumnWidthEx(columns,
                                                   column_index,
                                                   columns->IsBeingResized)
                                : 0.0f;

    if (!(columns->Flags & AzOldColumnFlags_NoForceWithinWindow))
        offset = AzMin(offset,
                       columns->OffMaxX
                               - g.Style.ColumnsMinSpacing
                                         * (columns->Count - column_index));
    columns->Columns[column_index].OffsetNorm =
            GetColumnNormFromOffset(columns, offset - columns->OffMinX);

    if (preserve_width)
        SetColumnOffset(column_index + 1,
                        offset + AzMax(g.Style.ColumnsMinSpacing, width));
}

void Az::SetColumnWidth(int column_index, float width)
{
    AzWindow* window = GetCurrentWindowRead();
    AzOldColumns* columns = window->DC.CurrentColumns;
    AZ_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    SetColumnOffset(column_index + 1, GetColumnOffset(column_index) + width);
}

void Az::PushColumnClipRect(int column_index)
{
    AzWindow* window = GetCurrentWindowRead();
    AzOldColumns* columns = window->DC.CurrentColumns;
    if (column_index < 0)
        column_index = columns->Current;

    AzOldColumnData* column = &columns->Columns[column_index];
    PushClipRect(column->ClipRect.Min, column->ClipRect.Max, false);
}

void Az::PushColumnsBackground()
{
    AzWindow* window = GetCurrentWindowRead();
    AzOldColumns* columns = window->DC.CurrentColumns;
    if (columns->Count == 1)
        return;

    columns->HostBackupClipRect = window->ClipRect;
    SetWindowClipRectBeforeSetChannel(window, columns->HostInitialClipRect);
    columns->Splitter.SetCurrentChannel(window->DrawList, 0);
}

void Az::PopColumnsBackground()
{
    AzWindow* window = GetCurrentWindowRead();
    AzOldColumns* columns = window->DC.CurrentColumns;
    if (columns->Count == 1)
        return;

    SetWindowClipRectBeforeSetChannel(window, columns->HostBackupClipRect);
    columns->Splitter.SetCurrentChannel(window->DrawList, columns->Current + 1);
}

AzOldColumns* Az::FindOrCreateColumns(AzWindow* window, AzID id)
{
    for (int n = 0; n < window->ColumnsStorage.Size; n++)
        if (window->ColumnsStorage[n].ID == id)
            return &window->ColumnsStorage[n];

    window->ColumnsStorage.push_back(AzOldColumns());
    AzOldColumns* columns = &window->ColumnsStorage.back();
    columns->ID = id;
    return columns;
}

AzID Az::GetColumnsID(const char* str_id, int columns_count)
{
    AzWindow* window = GetCurrentWindow();

    PushID(0x11223347 + (str_id ? 0 : columns_count));
    AzID id = window->GetID(str_id ? str_id : "columns");
    PopID();

    return id;
}

void Az::BeginColumns(const char* str_id,
                      int columns_count,
                      AzOldColumnFlags flags)
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();

    AZ_ASSERT(columns_count >= 1);
    AZ_ASSERT(window->DC.CurrentColumns == NULL);

    AzID id = GetColumnsID(str_id, columns_count);
    AzOldColumns* columns = FindOrCreateColumns(window, id);
    AZ_ASSERT(columns->ID == id);
    columns->Current = 0;
    columns->Count = columns_count;
    columns->Flags = flags;
    window->DC.CurrentColumns = columns;
    window->DC.NavIsScrollPushableX = false;

    columns->HostCursorPosY = window->DC.CursorPos.y;
    columns->HostCursorMaxPosX = window->DC.CursorMaxPos.x;
    columns->HostInitialClipRect = window->ClipRect;
    columns->HostBackupParentWorkRect = window->ParentWorkRect;
    window->ParentWorkRect = window->WorkRect;

    const float column_padding = g.Style.ItemSpacing.x;
    const float half_clip_extend_x = AzTrunc(
            AzMax(window->WindowPadding.x * 0.5f, window->WindowBorderSize));
    const float max_1 = window->WorkRect.Max.x + column_padding
                        - AzMax(column_padding - window->WindowPadding.x, 0.0f);
    const float max_2 = window->WorkRect.Max.x + half_clip_extend_x;
    columns->OffMinX = window->DC.Indent.x - column_padding
                       + AzMax(column_padding - window->WindowPadding.x, 0.0f);
    columns->OffMaxX = AzMax(AzMin(max_1, max_2) - window->Pos.x,
                             columns->OffMinX + 1.0f);
    columns->LineMinY = columns->LineMaxY = window->DC.CursorPos.y;

    if (columns->Columns.Size != 0
        && columns->Columns.Size != columns_count + 1)
        columns->Columns.resize(0);

    columns->IsFirstFrame = (columns->Columns.Size == 0);
    if (columns->Columns.Size == 0)
    {
        columns->Columns.reserve(columns_count + 1);
        for (int n = 0; n < columns_count + 1; n++)
        {
            AzOldColumnData column;
            column.OffsetNorm = n / (float)columns_count;
            columns->Columns.push_back(column);
        }
    }

    for (int n = 0; n < columns_count; n++)
    {
        AzOldColumnData* column = &columns->Columns[n];
        float clip_x1 = AZ_ROUND(window->Pos.x + GetColumnOffset(n));
        float clip_x2 = AZ_ROUND(window->Pos.x + GetColumnOffset(n + 1) - 1.0f);
        column->ClipRect = AzRect(clip_x1, -FLT_MAX, clip_x2, +FLT_MAX);
        column->ClipRect.ClipWithFull(window->ClipRect);
    }

    if (columns->Count > 1)
    {
        columns->Splitter.Split(window->DrawList, 1 + columns->Count);
        columns->Splitter.SetCurrentChannel(window->DrawList, 1);
        PushColumnClipRect(0);
    }

    float offset_0 = GetColumnOffset(columns->Current);
    float offset_1 = GetColumnOffset(columns->Current + 1);
    float width = offset_1 - offset_0;
    PushItemWidth(width * 0.65f);
    window->DC.ColumnsOffset.x = AzMax(column_padding - window->WindowPadding.x,
                                       0.0f);
    window->DC.CursorPos.x = AZ_TRUNC(window->Pos.x + window->DC.Indent.x
                                      + window->DC.ColumnsOffset.x);
    window->WorkRect.Max.x = window->Pos.x + offset_1 - column_padding;
    window->WorkRect.Max.y = window->ContentRegionRect.Max.y;
}

void Az::NextColumn()
{
    AzWindow* window = GetCurrentWindow();
    if (window->SkipItems || window->DC.CurrentColumns == NULL)
        return;

    AzContext& g = *GAz;
    AzOldColumns* columns = window->DC.CurrentColumns;

    if (columns->Count == 1)
    {
        window->DC.CursorPos.x = AZ_TRUNC(window->Pos.x + window->DC.Indent.x
                                          + window->DC.ColumnsOffset.x);
        AZ_ASSERT(columns->Current == 0);
        return;
    }

    if (++columns->Current == columns->Count)
        columns->Current = 0;

    PopItemWidth();

    AzOldColumnData* column = &columns->Columns[columns->Current];
    SetWindowClipRectBeforeSetChannel(window, column->ClipRect);
    columns->Splitter.SetCurrentChannel(window->DrawList, columns->Current + 1);

    const float column_padding = g.Style.ItemSpacing.x;
    columns->LineMaxY = AzMax(columns->LineMaxY, window->DC.CursorPos.y);
    if (columns->Current > 0)
    {
        window->DC.ColumnsOffset.x = GetColumnOffset(columns->Current)
                                     - window->DC.Indent.x + column_padding;
    } else
    {
        window->DC.ColumnsOffset.x = AzMax(column_padding
                                                   - window->WindowPadding.x,
                                           0.0f);
        window->DC.IsSameLine = false;
        columns->LineMinY = columns->LineMaxY;
    }
    window->DC.CursorPos.x = AZ_TRUNC(window->Pos.x + window->DC.Indent.x
                                      + window->DC.ColumnsOffset.x);
    window->DC.CursorPos.y = columns->LineMinY;
    window->DC.CurrLineSize = AzVec2(0.0f, 0.0f);
    window->DC.CurrLineTextBaseOffset = 0.0f;

    float offset_0 = GetColumnOffset(columns->Current);
    float offset_1 = GetColumnOffset(columns->Current + 1);
    float width = offset_1 - offset_0;
    PushItemWidth(width * 0.65f);
    window->WorkRect.Max.x = window->Pos.x + offset_1 - column_padding;
}

void Az::EndColumns()
{
    AzContext& g = *GAz;
    AzWindow* window = GetCurrentWindow();
    AzOldColumns* columns = window->DC.CurrentColumns;
    AZ_ASSERT(columns != NULL);

    PopItemWidth();
    if (columns->Count > 1)
    {
        PopClipRect();
        columns->Splitter.Merge(window->DrawList);
    }

    const AzOldColumnFlags flags = columns->Flags;
    columns->LineMaxY = AzMax(columns->LineMaxY, window->DC.CursorPos.y);
    window->DC.CursorPos.y = columns->LineMaxY;
    if (!(flags & AzOldColumnFlags_GrowParentContentsSize))
        window->DC.CursorMaxPos.x = columns->HostCursorMaxPosX;

    bool is_being_resized = false;
    if (!(flags & AzOldColumnFlags_NoBorder) && !window->SkipItems)
    {
        const float y1 = AzMax(columns->HostCursorPosY, window->ClipRect.Min.y);
        const float y2 = AzMin(window->DC.CursorPos.y, window->ClipRect.Max.y);
        int dragging_column = -1;
        for (int n = 1; n < columns->Count; n++)
        {
            AzOldColumnData* column = &columns->Columns[n];
            float x = window->Pos.x + GetColumnOffset(n);
            const AzID column_id = columns->ID + AzID(n);
            const float column_hit_hw = AzTrunc(COLUMNS_HIT_RECT_HALF_THICKNESS
                                                * g.CurrentDpiScale);
            const AzRect column_hit_rect(AzVec2(x - column_hit_hw, y1),
                                         AzVec2(x + column_hit_hw, y2));
            if (!ItemAdd(column_hit_rect, column_id, NULL, AzItemFlags_NoNav))
                continue;

            bool hovered = false, held = false;
            if (!(flags & AzOldColumnFlags_NoResize))
            {
                ButtonBehavior(column_hit_rect, column_id, &hovered, &held);
                if (hovered || held)
                    SetMouseCursor(AzMouseCursor_ResizeEW);
                if (held && !(column->Flags & AzOldColumnFlags_NoResize))
                    dragging_column = n;
            }

            const AzU32 col = GetColorU32(held      ? AzCol_SeparatorActive
                                          : hovered ? AzCol_SeparatorHovered
                                                    : AzCol_Separator);
            const float xi = AZ_TRUNC(x);
            window->DrawList->AddLine(AzVec2(xi, y1 + 1.0f),
                                      AzVec2(xi, y2),
                                      col);
        }

        if (dragging_column != -1)
        {
            if (!columns->IsBeingResized)
                for (int n = 0; n < columns->Count + 1; n++)
                    columns->Columns[n].OffsetNormBeforeResize =
                            columns->Columns[n].OffsetNorm;
            columns->IsBeingResized = is_being_resized = true;
            float x = GetDraggedColumnOffset(columns, dragging_column);
            SetColumnOffset(dragging_column, x);
        }
    }
    columns->IsBeingResized = is_being_resized;

    window->WorkRect = window->ParentWorkRect;
    window->ParentWorkRect = columns->HostBackupParentWorkRect;
    window->DC.CurrentColumns = NULL;
    window->DC.ColumnsOffset.x = 0.0f;
    window->DC.CursorPos.x = AZ_TRUNC(window->Pos.x + window->DC.Indent.x
                                      + window->DC.ColumnsOffset.x);
    NavUpdateCurrentWindowIsScrollPushableX();
}

void Az::Columns(int columns_count, const char* id, bool borders)
{
    AzWindow* window = GetCurrentWindow();
    AZ_ASSERT(columns_count >= 1);

    AzOldColumnFlags flags = (borders ? 0 : AzOldColumnFlags_NoBorder);

    AzOldColumns* columns = window->DC.CurrentColumns;
    if (columns != NULL && columns->Count == columns_count
        && columns->Flags == flags)
        return;

    if (columns != NULL)
        EndColumns();

    if (columns_count != 1)
        BeginColumns(id, columns_count, flags);
}

#endif
