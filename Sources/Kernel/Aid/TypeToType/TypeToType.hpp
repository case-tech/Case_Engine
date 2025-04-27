// Copyright (c) 2025 Case Technologies

#pragma once
#include <ctime>
#include <filesystem>
#include <list>
#include <string>
#include <vector>

<<<<<<< HEAD
=======
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

>>>>>>> aa4b252 (Add open project)
namespace CE_Kernel
{
    namespace Aid
    {
        namespace TypeToType
        {

            int StringToInt(const std::string& str_a);
            double StringToDouble(const std::string& str_a);
            bool StringToBool(const std::string& str_a);

            std::string IntToString(int value_a);
            std::string DoubleToString(double value_a);
            std::string BoolToString(bool value_a);

            int HexStringToInt(const std::string& str_a);
            std::string IntToHexString(int value_a);

            std::wstring StringToWstring(const std::string& str_a);
<<<<<<< HEAD
            std::string WstringToString(const std::wstring& wstr_a);
=======
            
#if defined(_WIN32) || defined(_WIN64)
            std::string WstringToString(const std::wstring& wstr_a);
#endif
>>>>>>> aa4b252 (Add open project)

            std::string ToLower(const std::string& str_a);
            std::string ToUpper(const std::string& str_a);

            time_t StringToTime(const std::string& str_a,
                                const std::string& format_a);
            std::string TimeToString(time_t time_a, const std::string& format_a);
<<<<<<< HEAD
=======
            int ExtractNumber(const std::string& mem_info_a);
>>>>>>> aa4b252 (Add open project)

            template <typename T, typename U>
            std::list<U> VectorToList(const std::vector<T>& vec_a)
            {
                return std::list<U>(vec_a.begin(), vec_a.end());
            }

            std::string GetAbsolutePath(const std::string& path_a);
            std::string GetRelativePath(const std::string& path_a,
                                        const std::string& base_a);
            std::string GetExecutablePath();
            std::string CombinePaths(const std::string& path1_a,
                                     const std::string& path2_a);
            std::string GetParentDirectory(const std::string& path_a);
            std::string GetFilename(const std::string& path_a);
            std::string GetFileExtension(const std::string& path_a);
            bool IsAbsolutePath(const std::string& path_a);
            std::string NormalizePath(const std::string& path_a);
            std::string GetCurrentWorkingDirectory();
            bool PathExists(const std::string& path_a);
            bool CreateDirectory(const std::string& path_a);

        } // namespace TypeToType
    } // namespace Aid
} // namespace CE_Kernel
