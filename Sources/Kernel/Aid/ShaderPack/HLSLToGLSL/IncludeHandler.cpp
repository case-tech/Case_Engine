#include "IncludeHandler.hpp"

#include "Exception.hpp"
#include "Report/ReportIdents.hpp"

#include <fstream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct IncludeHandler::OpaqueData
            {
                std::vector<std::string> search_paths_;
            };

            IncludeHandler::IncludeHandler() : data_ {new OpaqueData()}
            {}

            IncludeHandler::~IncludeHandler()
            {
                delete data_;
            }

            static std::unique_ptr<std::istream> ReadFile(
                    const std::string& filename_a)
            {
                auto stream_ = std::unique_ptr<std::istream>(
                        new std::ifstream(filename_a));
                return (stream_->good() ? std::move(stream_) : nullptr);
            }

            std::unique_ptr<std::istream> IncludeHandler::Include(
                    const std::string& filename_a,
                    bool use_search_paths_first_a)
            {
                if (!use_search_paths_first_a)
                {
                    if (auto file_ = ReadFile(filename_a))
                        return file_;
                }

                for (const auto& path_ : data_->search_paths_)
                {
                    if (!path_.empty())
                    {
                        std::string s_ = path_;
                        if (path_.back() != '/' && path_.back() != '\\')
                            s_ += '/';
                        s_ += filename_a;

                        if (auto file_ = ReadFile(s_))
                            return file_;
                    }
                }

                if (use_search_paths_first_a)
                {
                    if (auto file_ = ReadFile(filename_a))
                        return file_;
                }

                RuntimeErr(R_FailedToIncludeFile(filename_a));
            }

            std::vector<std::string>& IncludeHandler::GetSearchPaths()
            {
                return data_->search_paths_;
            }

            const std::vector<std::string>& IncludeHandler::GetSearchPaths()
                    const
            {
                return data_->search_paths_;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
