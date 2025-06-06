// Copyright (c) 2025 Case Technologies

#pragma once
#include "../SLScanner.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class HLSLScanner : public SLScanner
            {
            public:
                HLSLScanner(bool enable_cg_keywords_a, Log* log_a = nullptr);

            private:
                TokenPtr ScanIdentifierOrKeyword(std::string&& spell_a) override;
                bool enable_cg_keywords_ = false;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
