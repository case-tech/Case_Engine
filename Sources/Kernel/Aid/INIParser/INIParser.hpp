// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/INIParser.hpp>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace INIParser
        {
            class INIParser
            {
            public:
                INIParser(const std::string& path_to_file_a);
                bool Load();
                bool Save();

                std::string GetValue(const std::string& section_a, const std::string& key_a) const;
                void SetValue(const std::string& section_a, const std::string& key_a, 
                        const std::string& value_a);
                
                std::vector<std::string> GetSections() const;
                std::map<std::string, std::string> GetKeys(const std::string& section_a) const;

            private:
                CUtils::INIParser parser_;
            };
        } // namespace INIParser
    } // namespace Aid
} // namespace CE_Kernel
