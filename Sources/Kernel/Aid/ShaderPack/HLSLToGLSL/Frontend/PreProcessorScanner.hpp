// Copyright (c) 2025 Case Technologies

#pragma once
#include "Scanner.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class PreProcessorScanner : public Scanner
            {
            public:
                PreProcessorScanner(Log* log_a = nullptr);

                TokenPtr Next() override;

            private:
                TokenPtr ScanToken() override;
                TokenPtr ScanDirectiveOrDirectiveConcat();
                TokenPtr ScanIdentifier();
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel