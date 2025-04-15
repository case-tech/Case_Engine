#include "INIParser.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace INIParser
        {
            INIParser::INIParser(const std::string& path_to_file_a)
                : parser_(path_to_file_a) {}

            bool INIParser::Load()
            {
                return parser_.Load();
            }

            bool INIParser::Save()
            {
                return parser_.Save();
            }

            std::string INIParser::GetValue(const std::string& section_a, const std::string& key_a) const
            {
                return parser_.GetValue(section_a, key_a);
            }

            void INIParser::SetValue(const std::string& section_a, const std::string& key_a, const std::string& value_a)
            {
                parser_.SetValue(section_a, key_a, value_a);
            }

            std::vector<std::string> INIParser::GetSections() const
            {
                return parser_.GetSections();
            }

            std::map<std::string, std::string> INIParser::GetKeys(const std::string& section_a) const
            {
                return parser_.GetKeys(section_a);
            }
        } // namespace INIParser
    } // namespace Aid
} // namespace CE_Kernel
