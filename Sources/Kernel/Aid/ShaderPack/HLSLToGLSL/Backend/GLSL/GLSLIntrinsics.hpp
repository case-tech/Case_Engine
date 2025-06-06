// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../AST/ASTEnums.hpp"
#include "../../AST/Token.hpp"

#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            const std::string* IntrinsicToGLSLKeyword(const Intrinsic intr_a,
                                                      bool use_glsl_120_a = false);
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
