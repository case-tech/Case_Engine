#pragma once
#include "Aid/FPLParser/FPLParser.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>

using namespace CUtils;

UTEST(FPLParserTest, InvalidSyntaxHandling)
{
    std::stringstream input_;
    input_ << "key = [1, 2, ";
    
    CE_Kernel::Aid::FPLParser::FPLParser parser_;
    ASSERT_THROW(parser_.Parse(input_), std::runtime_error);
}
