#include "SourceCode.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            SourceCode::SourceCode(const std::shared_ptr<std::istream>& stream_a)
                : stream_ {stream_a}
            {}

            bool SourceCode::IsValid() const
            {
                return (stream_ != nullptr && stream_->good());
            }

            char SourceCode::Next()
            {
                while (pos_.Column() >= current_line_.size())
                {
                    if (!IsValid() || stream_->eof())
                        return 0;

                    std::getline(*stream_, current_line_);
                    current_line_ += '\n';
                    pos_.IncRow();

                    lines_.push_back(current_line_);
                }

                auto chr_ = current_line_[pos_.Column()];
                pos_.IncColumn();

                return chr_;
            }

            static bool BuildLineMarker(const SourceArea& area_a,
                                        const std::string& line_in_a,
                                        std::string& line_out_a,
                                        std::string& marker_out_a)
            {
                if (area_a.Pos().Column() >= line_in_a.size()
                    || area_a.Pos().Column() == 0 || area_a.Length() == 0)
                    return false;

                line_out_a = line_in_a;

                for (auto i_ = static_cast<std::size_t>(area_a.Pos().Column());
                     i_ < line_out_a.size();
                     ++i_)
                {
                    if (line_out_a[i_] == '\t')
                        line_out_a[i_] = ' ';
                }

                marker_out_a = std::string(area_a.Pos().Column() - 1, ' ');

                for (std::size_t i_ = 0, n_ = marker_out_a.size(); i_ < n_; ++i_)
                {
                    if (line_in_a[i_] == '\t')
                        marker_out_a[i_] = '\t';
                }

                auto len_ = std::min(area_a.Length(),
                                    static_cast<unsigned int>(line_in_a.size())
                                            - area_a.Pos().Column());

                if (len_ > 0)
                {
                    if (area_a.Offset() < len_)
                    {
                        marker_out_a += std::string(area_a.Offset(), '~');
                        marker_out_a += '^';
                        marker_out_a += std::string(len_ - 1 - area_a.Offset(), '~');
                    } 
                    else
                        marker_out_a += std::string(len_, '~');
                    return true;
                }

                return false;
            }

            bool SourceCode::FetchLineMarker(const SourceArea& area_a,
                                             std::string& line_a,
                                             std::string& marker_a)
            {
                if (area_a.Length() > 0)
                {
                    auto row_ = area_a.Pos().Row();
                    if (row_ == pos_.Row())
                        return BuildLineMarker(area_a, Line(), line_a, marker_a);
                    else if (row_ > 0)
                        return BuildLineMarker(area_a,
                                               GetLine(static_cast<std::size_t>(
                                                       row_ - 1)),
                                               line_a,
                                               marker_a);
                }
                return false;
            }

            void SourceCode::NextSourceOrigin(const std::string& filename_a,
                                              int line_offset_a)
            {
                auto origin_ = std::make_shared<SourceOrigin>();
                {
                    origin_->filename_ = filename_a;
                    origin_->line_offset_ = line_offset_a;
                }
                pos_.SetOrigin(origin_);
            }

            std::string SourceCode::Filename() const
            {
                if (auto origin_ = pos_.GetOrigin())
                    return origin_->filename_;
                else
                    return "";
            }

            std::string SourceCode::GetLine(std::size_t line_index_a) const
            {
                return (line_index_a < lines_.size() ? lines_[line_index_a] : "");
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel