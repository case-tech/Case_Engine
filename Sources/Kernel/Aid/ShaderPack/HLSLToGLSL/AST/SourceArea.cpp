#include "SourceArea.hpp"

#include "AST.hpp"
#include "Token.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            const SourceArea SourceArea::ignore {};

            SourceArea::SourceArea(const SourcePosition& pos_a,
                                   unsigned int length_a,
                                   unsigned int offset_a)
                : pos_ {pos_a}, length_ {length_a}, offset_ {offset_a}
            {}

            bool SourceArea::IsValid() const
            {
                return (pos_.IsValid() && length_ > 0);
            }

            void SourceArea::Update(const SourceArea& area_a)
            {
                if (area_a.pos_.Row() > pos_.Row())
                    length_ = (unsigned int)~0;
                else if (area_a.pos_.Row() == pos_.Row()
                         && area_a.pos_.Column() + area_a.length_
                                    > pos_.Column() + length_)
                    length_ = (area_a.pos_.Column() - pos_.Column()
                               + area_a.length_);
            }

            void SourceArea::Update(const std::string& length_from_ident_a)
            {
                length_ = std::max(length_,
                                   static_cast<unsigned int>(
                                           length_from_ident_a.size()));
            }

            void SourceArea::Update(const Token& tkn_a)
            {
                Update(tkn_a.Area());
            }

            void SourceArea::Update(const AST& ast_a)
            {
                Update(ast_a.area_);
            }

            void SourceArea::Offset(unsigned int offset_a)
            {
                offset_a = offset_a;
            }

            void SourceArea::Offset(const SourcePosition& pos_a)
            {
                if (pos_a.Row() == pos_.Row() && pos_a.Column() >= pos_.Column())
                    offset_ = (pos_a.Column() - pos_.Column());
                else
                    offset_ = (unsigned int)~0;
            }

            unsigned int SourceArea::Offset() const
            {
                return (length_ > 0 ? std::min(offset_, length_ - 1) : 0);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel