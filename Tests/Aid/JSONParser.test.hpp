#pragma once
#include "Aid/JSONParser/JSONParser.hpp"
#include <CUtils/UnitTests.hpp>

using namespace CUtils;

UTEST(JSONParserTest, ParseSimpleObject)
{
    std::string json_ = R"({"name":"John", "age":30, "is_student":false})";
    auto value_ = CE_Kernel::Aid::JSONParser::JSONParser::Parse(json_);
    
    ASSERT_EQ(value_->type(), CE_Kernel::Aid::JSONParser::JSONValue::Object);
    auto obj_ = static_cast<CE_Kernel::Aid::JSONParser::JSONObject*>(value_.get());
    
    ASSERT_STREQ(obj_->Get("name")->to_string().c_str(), "\"John\"");
    ASSERT_STREQ(obj_->Get("age")->to_string().c_str(), "30");
    ASSERT_STREQ(obj_->Get("is_student")->to_string().c_str(), "false");
}

UTEST(JSONParserTest, ParseInvalidJSON)
{
    std::string json_ = R"({"key": "value")";
    ASSERT_THROW(
        CE_Kernel::Aid::JSONParser::JSONParser::Parse(json_),
        CE_Kernel::Aid::JSONParser::JSONParseException
    );
}

UTEST(JSONParserTest, NumberPrecision)
{
    std::string json_ = R"({"pi":3.1415926535})";
    auto value_ = CE_Kernel::Aid::JSONParser::JSONParser::Parse(json_);
    auto num_ = static_cast<CE_Kernel::Aid::JSONParser::JSONNumber*>(
        static_cast<CE_Kernel::Aid::JSONParser::JSONObject*>(value_.get())->Get("pi")
    );
    ASSERT_NEAR(num_->Value(), 3.1415926535, 0.0000001);
}

UTEST(JSONParserTest, NullHandling)
{
    std::string json_ = R"({"data":null})";
    auto value_ = CE_Kernel::Aid::JSONParser::JSONParser::Parse(json_);
    auto null_val_ = static_cast<CE_Kernel::Aid::JSONParser::JSONNull*>(
        static_cast<CE_Kernel::Aid::JSONParser::JSONObject*>(value_.get())->Get("data")
    );
    ASSERT_EQ(null_val_->type(), CE_Kernel::Aid::JSONParser::JSONValue::Null);
}
