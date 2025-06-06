// Copyright (c) 2025 Case Technologies

#pragma once
#include "SourcePosition.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Token;
            struct AST;

            class SourceArea
            {
            public:
                static const SourceArea ignore;

                SourceArea() = default;
                SourceArea(const SourceArea&) = default;
                SourceArea& operator=(const SourceArea&) = default;

                SourceArea(const SourcePosition& pos_a,
                           unsigned int length_a,
                           unsigned int offset_a = 0);

                bool IsValid() const;

                void Update(const SourceArea& area_a);
                void Update(const std::string& length_from_ident_a);
                void Update(const Token& tkn_a);
                void Update(const AST& ast_a);
                void Offset(unsigned int offset_a);
                void Offset(const SourcePosition& pos_a);

                unsigned int Offset() const;

                inline const SourcePosition& Pos() const
                {
                    return pos_;
                }

                inline unsigned int Length() const
                {
                    return length_;
                }

            private:
                SourcePosition pos_;
                unsigned int length_ = 0;
                unsigned int offset_ = 0;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel