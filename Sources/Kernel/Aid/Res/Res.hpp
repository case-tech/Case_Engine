// Copyright (c) 2025 Case Technologies

#pragma once
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Res
        {
            class Writer
            {
            public:
                Writer() = default;

                void AddSection(const std::string& section_a);

                void AddFile(const std::string& section_a,
                             const std::string& name_a,
                             const std::string& file_path_a);

                void RemoveFile(const std::string& section_a,
                                const std::string& name_a);

                void UpdateFile(const std::string& section_a,
                                const std::string& name_a,
                                const std::string& new_file_path_a);

                std::vector<std::string> ListSections() const;

                std::vector<std::string> ListFiles(
                        const std::string& section_a) const;

                void WriteToFile(const std::string& out_file_path_a) const;

            private:
                struct FileEntry
                {
                    std::string name_;
                    std::vector<char> data_;
                };

                std::unordered_map<std::string, std::vector<FileEntry>>
                        sections_;

                static void WriteString(std::ofstream& out_a,
                                        const std::string& s_a)
                {
                    uint32_t l_ = uint32_t(s_a.size());
                    out_a.write(reinterpret_cast<const char*>(&l_), sizeof(l_));
                    out_a.write(s_a.data(), l_);
                }
            };

            class Reader
            {
            public:
                explicit Reader(const std::string& res_file_path_a);

                std::vector<std::string> ListSections() const;
                std::vector<std::string> ListFiles(
                        const std::string& section_a) const;
                
                bool HasFile(const std::string& section_a,
                             const std::string& name_a) const;

                std::vector<char> GetFile(const std::string& section_a,
                                          const std::string& name_a) const;

                void ExtractAll(const std::string& out_dir_a) const;

            private:
                struct FileRecord
                {
                    uint64_t offset_;
                    uint64_t size_;
                };

                std::string file_path_;
                std::unordered_map<std::string,
                                   std::unordered_map<std::string, FileRecord>> index_;
               
                std::shared_ptr<std::ifstream> in_;

                static std::string ReadString(std::ifstream& in_a)
                {
                    uint32_t l_;
                    in_a.read(reinterpret_cast<char*>(&l_), sizeof(l_));
                    std::string s_(l_, '\0');
                    in_a.read(&s_[0], l_);
                    return s_;
                }

                void ParseIndex();
                static void EnsureDir(const std::string& full_path_a);
            };
        } // namespace Res
    } // namespace Aid
} // namespace CE_Kernel