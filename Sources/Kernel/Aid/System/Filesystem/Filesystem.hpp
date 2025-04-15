// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/Filesystem.hpp>
#include <string>
#include <vector>
#include <cstdint>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace System
        {
            class Filesystem
            {
            public:
                bool CreateDirectory(const std::string& path_a);
                bool Remove(const std::string& path_a);
                bool Rename(const std::string& old_path_a, 
                            const std::string& new_path_a);
                bool Exists(const std::string & path_a);
                bool Copy  (const std::string& from_a, const std::string& to_a,
                            bool recursive_a = false);

                std::vector<std::string> ListDirectory(const std::string& path_a) const;
                std::string GetAbsolutePath(const std::string& path_a) const;

                bool IsDirectory(const std::string& path_a) const;
                bool IsFile(const std::string& path_a) const;

                uintmax_t FileSize(const std::string& path_a) const;

            private:
                CUtils::Filesystem fs_;
            };
        } // namespace System
    } // namespace Aid
} // namespace CE_Kernel
