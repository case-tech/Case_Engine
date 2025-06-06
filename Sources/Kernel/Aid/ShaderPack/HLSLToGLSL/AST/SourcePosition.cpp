#include "SourcePosition.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            const SourcePosition SourcePosition::ignore {};

            SourcePosition::SourcePosition(unsigned int row_a,
                                           unsigned int column_a,
                                           const SourceOriginPtr& origin_a)
                : row_ {row_a}, column_ {column_a}, origin_ {origin_a}
            {}

            std::string SourcePosition::ToString(bool print_filename_a) const
            {
                std::string s_;

                auto r_ = row_;
                auto c_ = column_;

                if (origin_)
                {
                    if (print_filename_a && !origin_->filename_.empty())
                    {
                        s_ += origin_->filename_;
                        s_ += ':';
                    }
                    s_ += std::to_string(static_cast<int>(r_)
                                        + origin_->line_offset_);
                } 
                else
                    s_ += std::to_string(r_);

                s_ += ':';
                s_ += std::to_string(c_);

                return s_;
            }

            void SourcePosition::IncRow()
            {
                ++row_;
                column_ = 0;
            }

            void SourcePosition::IncColumn()
            {
                ++column_;
            }

            bool SourcePosition::IsValid() const
            {
                return (row_ > 0 && column_ > 0);
            }

            void SourcePosition::Reset()
            {
                row_ = column_ = 0;
            }

            bool SourcePosition::operator<(const SourcePosition& rhs_a) const
            {
                if (origin_.get() < rhs_a.origin_.get())
                    return true;
                else if (origin_.get() > rhs_a.origin_.get())
                    return false;

                if (row_ < rhs_a.row_)
                    return true;
                if (row_ > rhs_a.row_)
                    return false;

                return (column_ < rhs_a.column_);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel