// Copyright (c) 2025 Case Technologies

#pragma once
#include "../JoinString.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
#define DECL_REPORT(NAME, VALUE)                                               \
    static const CE_Kernel::Aid::ShaderPack::JoinableString R_##NAME           \
    {                                                                          \
        VALUE                                                                  \
    }

#include "ReportIdentsEN.hpp"

#undef DECL_REPORT
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel