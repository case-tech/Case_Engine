// Copyright (c) 2025 Case Technologies

#pragma once
#include "Export.hpp"

#include <stack>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class HTG_EXPORT IndentHandler
            {
            public:
                IndentHandler(const std::string& initial_indent_a = "  ");
                virtual ~IndentHandler();

                void SetIndent(const std::string& indent_a);
                void IncIndent();
                void DecIndent();

                const std::string& FullIndent() const;

            private:
                struct OpaqueData;
                OpaqueData* data_ = nullptr;
            };

            class ScopedIndent
            {
            public:
                inline ScopedIndent(IndentHandler& handler_a) : handler_ {handler_a}
                {
                    handler_.IncIndent();
                }

                inline ~ScopedIndent()
                {
                    handler_.DecIndent();
                }

            private:
                IndentHandler& handler_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
