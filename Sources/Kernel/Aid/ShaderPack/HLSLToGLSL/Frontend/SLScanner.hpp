// Copyright (c) 2025 Case Technologies

#pragma once
#include "Scanner.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class SLScanner : public Scanner
            {
            public:
                SLScanner(Log* log_a = nullptr);

                TokenPtr Next() override;

            protected:
                virtual TokenPtr ScanIdentifierOrKeyword(
                        std::string&& spell_a) = 0;

            private:
                TokenPtr ScanToken() override;

                TokenPtr ScanDirective();
                TokenPtr ScanIdentifier();
                TokenPtr ScanAssignShiftRelationOp(const char chr_a);
                TokenPtr ScanPlusOp();
                TokenPtr ScanMinusOp();
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel