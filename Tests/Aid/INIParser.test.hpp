#pragma once
#include "Aid/INIParser/INIParser.hpp"
#include <CUtils/UnitTests.hpp>
#include <fstream>
#include <filesystem>

using namespace CUtils;

UTEST(INIParserTest, LoadValidFile)
{
    std::ofstream tmp_("test.ini");
    tmp_ << "[Section1]\nkey1=value1\n[Section2]\nkey2=value2";
    tmp_.close();

    CE_Kernel::Aid::INIParser::INIParser parser_("test.ini");
    ASSERT_TRUE(parser_.Load());
    
    std::filesystem::remove("test.ini");
}

UTEST(INIParserTest, SetAndSaveValue)
{
    CE_Kernel::Aid::INIParser::INIParser parser_("test.ini");
    parser_.SetValue("Section3", "key3", "value3");
    ASSERT_TRUE(parser_.Save());
    
    CE_Kernel::Aid::INIParser::INIParser parser2_("test.ini");
    parser2_.Load();
    ASSERT_STREQ(parser2_.GetValue("Section3", "key3").c_str(), "value3");
}

UTEST(INIParserTest, NonExistentKey)
{
    CE_Kernel::Aid::INIParser::INIParser parser_("test.ini");
    parser_.Load();
    ASSERT_TRUE(parser_.GetValue("Section1", "invalid_key").empty());
}
