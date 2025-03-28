#include "Filesystem.hpp"

namespace CE_Kernel
{
    namespace Aux
    {
        namespace System
        {
            bool Filesystem::CreateDirectory(const std::string& path_a)
            {
                return fs_.CreateDirectory(path_a);
            }

            bool Filesystem::Remove(const std::string& path_a)
            {
                return fs_.Remove(path_a);
            }

            bool Filesystem::Rename(const std::string& old_path_a, 
                                    const std::string& new_path_a)
            {
                return fs_.Rename(old_path_a, new_path_a);
            }

            bool Filesystem::Exists(const std::string& path_a)
            {
                return fs_.Exists(path_a);
            }

            bool Filesystem::Copy(const std::string& from_a,
                                  const std::string& to_a, 
                                  bool recursive)
            {
                return fs_.Copy(from_a, to_a, recursive);
            }

            std::vector<std::string> Filesystem::ListDirectory(const std::string& path_a) const
            {
                return fs_.ListDirectory(path_a);
            }

            std::string Filesystem::GetAbsolutePath(const std::string& path_a) const
            {
                return fs_.GetAbsolutePath(path_a);
            }

            bool Filesystem::IsDirectory(const std::string& path_a) const
            {
                return fs_.IsDirectory(path_a);
            }

            bool Filesystem::IsFile(const std::string& path_a) const
            {
                return fs_.IsFile(path_a);
            }

            uintmax_t Filesystem::FileSize(const std::string& path_a) const
            {
                return fs_.FileSize(path_a);
            }
        }
    }
}
