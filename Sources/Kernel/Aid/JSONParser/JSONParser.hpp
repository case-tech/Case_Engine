// Copyright (c) 2025 Case Technologies

#pragma once
#include <cctype>
#include <cmath>
#include <exception>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace JSONParser
        {
            class JSONValue
            {
            public:
                enum Type
                {
                    Null,
                    Bool,
                    Number,
                    String,
                    Array,
                    Object
                };

                virtual ~JSONValue() = default;
                virtual Type type() const = 0;
                virtual std::string to_string() const = 0;
            };

            class JSONNull : public JSONValue
            {
            public:
                Type type() const override
                {
                    return Null;
                }

                std::string to_string() const override
                {
                    return "null";
                }
            };

            class JSONBool : public JSONValue
            {
            public:
                explicit JSONBool(bool value_a)
                    : value_(value_a) {}

                Type type() const override
                {
                    return Bool;
                }

                std::string to_string() const override
                {
                    return value_ ? "true" : "false";
                }

                bool Value() const
                {
                    return value_;
                }
            private:
                bool value_;
            };

            class JSONNumber : public JSONValue
            {
            public:
                explicit JSONNumber(double value_a)
                    : value_(value_a) {}

                Type type() const override
                {
                    return Number;
                }

                std::string to_string() const override;

                double Value() const
                {
                    return value_;
                }

            private:
                double value_;
            };

            class JSONString : public JSONValue
            {
            public:
                explicit JSONString(const std::string& value_a)
                    : value_(value_a) {}

                Type type() const override
                {
                    return String;
                }

                std::string to_string() const override;

                const std::string& Value() const
                {
                    return value_;
                }

            private:
                std::string value_;
            };

            class JSONArray : public JSONValue
            {
            public:
                Type type() const override
                {
                    return Array;
                }

                std::string to_string() const override;
                void Add(std::unique_ptr<JSONValue> value_a);
                size_t Size() const;
                JSONValue* At(size_t index_a) const;

            private:
                std::vector<std::unique_ptr<JSONValue>> elements_;
            };

            class JSONObject : public JSONValue
            {
            public:
                Type type() const override
                {
                    return Object;
                }

                std::string to_string() const override;
                void Add(const std::string& key_a, std::unique_ptr<JSONValue> value_a);
                bool Has(const std::string& key_a) const;
                JSONValue* Get(const std::string& key_a) const;

            private:
                 std::map<std::string, std::unique_ptr<JSONValue>> members_;
            };

            class JSONParser
            {
            public:
                static std::unique_ptr<JSONValue> Parse(const std::string& input_a);
                static std::unique_ptr<JSONValue> ParseFromFile(const std::string& path_to_file_a);

            private:
                static void SkipWhiteSpace(const std::string& input_a, size_t& index_a);
                static std::unique_ptr<JSONValue> ParseValue(const std::string& input_a, 
                        size_t& index_a);
                
                static std::unique_ptr<JSONObject> ParseObject(const std::string& input_a, 
                        size_t& index_a);
                
                static std::unique_ptr<JSONArray> ParseArray(const std::string& input_a, 
                        size_t& index_a);
                
                static std::string ParseString(const std::string& input_a, size_t& index_a);
                static std::unique_ptr<JSONNumber> ParseNumber(const std::string& input_a, 
                        size_t& index_a);
                
                static std::unique_ptr<JSONValue> ParseKeyword(const std::string& input_a, 
                        size_t& index_a);
            };

            class JSONParseException : public std::exception
            {
            public:
                JSONParseException(const std::string& message_a, size_t position_a);
                const char* what() const noexcept override;

            private:
                std::string message_;
            };
        } // namespace JSONParser
    } // namespace Aid
} // namespace CE_Kernel