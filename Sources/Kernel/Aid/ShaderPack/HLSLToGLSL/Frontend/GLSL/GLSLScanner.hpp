// Copyright (c) 2025 Case Technologies\

#pragma once
#include "../SLScanner.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class GLSLScanner : public SLScanner
            {
            public:
                GLSLScanner(Log* log_a = nullptr);

            private:
                TokenPtr ScanIdentifierOrKeyword(std::string&& spell_a) override;
            };
        } // namespace Xsc
    } // namespace Aid
} // namespace CE_Kernel
