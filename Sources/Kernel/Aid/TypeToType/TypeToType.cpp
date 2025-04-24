#include "TypeToType.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace CE_Kernel
{
    namespace Aid
    {
        namespace TypeToType
        {

            int StringToInt(const std::string& str_a)
            {
                std::istringstream iss_(str_a);
                int value_;
                if (!(iss_ >> value_))
                    throw std::invalid_argument("Invalid integer string");

                return value_;
            }

            double StringToDouble(const std::string& str_a)
            {
                std::istringstream iss_(str_a);
                double value_;
                if (!(iss_ >> value_))
                    throw std::invalid_argument("Invalid double string");

                return value_;
            }

            bool StringToBool(const std::string& str_a)
            {
                std::string lower_str_ = ToLower(str_a);
                if (lower_str_ == "true" || lower_str_ == "1")
                    return true;
                if (lower_str_ == "false" || lower_str_ == "0")
                    return false;

                throw std::invalid_argument("Invalid boolean string");
            }

            std::string IntToString(int value_a)
            {
                return std::to_string(value_a);
            }

            std::string DoubleToString(double value_a)
            {
                return std::to_string(value_a);
            }

            std::string BoolToString(bool value_a)
            {
                return value_a ? "true" : "false";
            }

            int HexStringToInt(const std::string& str_a)
            {
                return std::stoi(str_a, nullptr, 16);
            }

            std::string IntToHexString(int value_a)
            {
                std::stringstream ss_;
                ss_ << std::hex << value_a;
                return ss_.str();
            }

            std::wstring StringToWstring(const std::string& str_a)
            {
                return std::wstring(str_a.begin(), str_a.end());
            }

            std::string WstringToString(const std::wstring& wstr_a)
            {
                return std::string(wstr_a.begin(), wstr_a.end());
            }

            std::string ToLower(const std::string& str_a)
            {
                std::string result_ = str_a;
                std::transform(result_.begin(),
                               result_.end(),
                               result_.begin(),
                               ::tolower);
                return result_;
            }

            std::string ToUpper(const std::string& str_a)
            {
                std::string result_ = str_a;
                std::transform(result_.begin(),
                               result_.end(),
                               result_.begin(),
                               ::toupper);
                return result_;
            }

            time_t StringToTime(const std::string& str_a,
                                const std::string& format_a)
            {
                std::tm tm_ = {};
                std::istringstream ss_(str_a);
                ss_ >> std::get_time(&tm_, format_a.c_str());
                if (ss_.fail())
                    throw std::runtime_error("Failed to parse time string");

                return std::mktime(&tm_);
            }

            std::string TimeToString(time_t time_a, const std::string& format_a)
            {
                std::tm* tm_ = std::localtime(&time_a);
                char buffer_[80];
                std::strftime(buffer_, sizeof(buffer_), format_a.c_str(), tm_);
                return std::string(buffer_);
            }

            std::string GetAbsolutePath(const std::string& path_a)
            {
                return fs::absolute(fs::path(path_a)).string();
            }

            std::string GetRelativePath(const std::string& path_a,
                                        const std::string& base_a)
            {
                return fs::relative(fs::path(path_a), fs::path(base_a)).string();
            }

            std::string GetExecutablePath()
            {
#if defined(__linux__)
                return fs::read_symlink("/proc/self/exe").string();
#elif defined(_WIN32) || defined(_WIN64))
                char path_[MAX_PATH];
                GetModuleFileNameA(NULL, path_, MAX_PATH);
                return std::string(path_);
#else
#error "Unsupported platform"
#endif
            }

            std::string CombinePaths(const std::string& path1_a,
                                     const std::string& path2_a)
            {
                return (fs::path(path1_a) / path2_a).string();
            }

            std::string GetParentDirectory(const std::string& path_a)
            {
                return fs::path(path_a).parent_path().string();
            }

            std::string GetFilename(const std::string& path_a)
            {
                return fs::path(path_a).filename().string();
            }

            std::string GetFileExtension(const std::string& path_a)
            {
                return fs::path(path_a).extension().string();
            }

            bool IsAbsolutePath(const std::string& path_a)
            {
                return fs::path(path_a).is_absolute();
            }

            std::string NormalizePath(const std::string& path_a)
            {
                return fs::weakly_canonical(fs::path(path_a)).string();
            }

            std::string GetCurrentWorkingDirectory()
            {
                return fs::current_path().string();
            }

            bool PathExists(const std::string& path_a)
            {
                return fs::exists(fs::path(path_a));
            }

            bool CreateDirectory(const std::string& path_a)
            {
                return fs::create_directory(fs::path(path_a));
            }

        } // namespace TypeToType
    } // namespace Aid
} // namespace CE_Kernel
