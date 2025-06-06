#include "Res.hpp"

#include <filesystem>
#include <iterator>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Res
        {
            namespace fs = std::filesystem;

            void Writer::AddSection(const std::string& section_a)
            {
                if (!sections_.count(section_a))
                    sections_.emplace(section_a, std::vector<FileEntry> {});
            }

            void Writer::AddFile(const std::string& section_a,
                                 const std::string& name_a,
                                 const std::string& file_path_a)
            {
                AddSection(section_a);
                std::ifstream inp_(file_path_a, std::ios::binary);
                if (!inp_)
                    throw std::runtime_error("Cannot open file: " + file_path_a);
                
                std::vector<char> buf_ {std::istreambuf_iterator<char>(inp_),
                                        std::istreambuf_iterator<char>()};
                
                sections_[section_a].push_back({name_a, std::move(buf_)});
            }

            void Writer::RemoveFile(const std::string& section_a,
                                    const std::string& name_a)
            {
                auto sit_ = sections_.find(section_a);
                if (sit_ == sections_.end())
                    return;
                
                auto& vec_ = sit_->second;
                vec_.erase(std::remove_if(vec_.begin(),
                                         vec_.end(),
                                         [&](auto& e) {
                                             return e.name_ == name_a;
                                         }),
                          vec_.end());
            }

            void Writer::UpdateFile(const std::string& section_a,
                                    const std::string& name_a,
                                    const std::string& new_file_path_a)
            {
                RemoveFile(section_a, name_a);
                AddFile(section_a, name_a, new_file_path_a);
            }

            std::vector<std::string> Writer::ListSections() const
            {
                std::vector<std::string> out_;
                out_.reserve(sections_.size());
                for (auto& kv : sections_)
                    out_.push_back(kv.first);
                
                return out_;
            }

            std::vector<std::string> Writer::ListFiles(
                    const std::string& section_a) const
            {
                auto sit_ = sections_.find(section_a);
                if (sit_ == sections_.end())
                    return {};
                
                const auto& vec_ = sit_->second;
                std::vector<std::string> out_;
                out_.reserve(vec_.size());
                for (auto& e : vec_)
                    out_.push_back(e.name_);

                return out_;
            }

            void Writer::WriteToFile(const std::string& out_file_path_a) const
            {
                std::ofstream out_(out_file_path_a, std::ios::binary);
                if (!out_)
                    throw std::runtime_error("Cannot create: " + out_file_path_a);

                uint32_t sec_count_ = (uint32_t)sections_.size();
                out_.write(reinterpret_cast<const char*>(&sec_count_),
                           sizeof(sec_count_));

                for (auto& [sec_name_, files_] : sections_)
                {
                    WriteString(out_, sec_name_);

                    uint32_t file_count_ = (uint32_t)files_.size();
                    out_.write(reinterpret_cast<const char*>(&file_count_),
                               sizeof(file_count_));

                    for (auto& f_ : files_)
                    {
                        WriteString(out_, f_.name_);
                        uint64_t sz_ = f_.data_.size();
                        out_.write(reinterpret_cast<const char*>(&sz_),
                                   sizeof(sz_));
                        out_.write(f_.data_.data(), sz_);
                    }
                }
            }

            Reader::Reader(const std::string& res_file_path_a)
                : file_path_(res_file_path_a),
                  in_(std::make_shared<std::ifstream>(res_file_path_a,
                                                      std::ios::binary))
            {
                if (!in_->good())
                    throw std::runtime_error("Cannot open: " + res_file_path_a);
                
                ParseIndex();
            }

            void Reader::ParseIndex()
            {
                auto& in1_ = *in_;
                in1_.seekg(0, std::ios::beg);

                uint32_t sec_count_;
                in1_.read(reinterpret_cast<char*>(&sec_count_), sizeof(sec_count_));

                for (uint32_t i_ = 0; i_ < sec_count_; ++i_)
                {
                    std::string sec_name_ = ReadString(in1_);

                    uint32_t file_count_;
                    in1_.read(reinterpret_cast<char*>(&file_count_),
                            sizeof(file_count_));

                    for (uint32_t j_ = 0; j_ < file_count_; ++j_)
                    {
                        std::string fname_ = ReadString(in1_);
                        uint64_t sz_;
                        in1_.read(reinterpret_cast<char*>(&sz_), sizeof(sz_));
                        uint64_t offset_ = in1_.tellg();

                        index_[sec_name_][fname_] = {offset_, sz_};
                        in1_.seekg(sz_, std::ios::cur);
                    }
                }
            }

            std::vector<std::string> Reader::ListSections() const
            {
                std::vector<std::string> out_;
                out_.reserve(index_.size());
                for (auto& kv_ : index_)
                    out_.push_back(kv_.first);
                return out_;
            }

            std::vector<std::string> Reader::ListFiles(
                    const std::string& section_a) const
            {
                auto sit_ = index_.find(section_a);
                if (sit_ == index_.end())
                    return {};
                
                std::vector<std::string> out_;
                out_.reserve(sit_->second.size());
                for (auto& kv_ : sit_->second)
                    out_.push_back(kv_.first);
                
                return out_;
            }

            bool Reader::HasFile(const std::string& section_a,
                                 const std::string& name_a) const
            {
                auto sit_ = index_.find(section_a);
                if (sit_ == index_.end())
                    return false;
                
                return sit_->second.count(name_a) > 0;
            }

            std::vector<char> Reader::GetFile(const std::string& section_a,
                                              const std::string& name_a) const
            {
                auto sit_ = index_.find(section_a);
                if (sit_ == index_.end())
                    throw std::runtime_error("No section: " + section_a);
                
                auto fit_ = sit_->second.find(name_a);
                if (fit_ == sit_->second.end())
                    throw std::runtime_error("No file: " + name_a);

                const auto& rec_ = fit_->second;
                std::vector<char> buf_(rec_.size_);
                in_->seekg(rec_.offset_);
                in_->read(buf_.data(), rec_.size_);
                return buf_;
            }

            void Reader::EnsureDir(const std::string& full_path_a)
            {
                fs::path p_(full_path_a);
                fs::create_directories(p_.parent_path());
            }

            void Reader::ExtractAll(const std::string& out_dir_a) const
            {
                for (auto& [sec_, files_] : index_)
                {
                    for (auto& [fname_, rec_] : files_)
                    {
                        auto data_ = GetFile(sec_, fname_);
                        std::string out_path_ = out_dir_a + "/" + sec_ + "/" + fname_;
                        EnsureDir(out_path_);
                        
                        std::ofstream out_(out_path_, std::ios::binary);
                        out_.write(data_.data(), data_.size());
                    }
                }
            }
        } // namespace Res
    } // namespace Aid
} // namespace CE_Kernel