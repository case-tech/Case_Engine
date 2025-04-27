// Copyright (c) 2025 Case Technologies

#pragma once
#include <istream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace FPLParser
        {
            struct Value
            {
                std::variant<
                    std::string,
                    double,
                    bool,
                    std::vector<Value>,
                    std::monostate>
                    data_;

                explicit Value(std::string val_a) : data_(std::move(val_a))
                {
                }

                explicit Value(double val_a) : data_(val_a)
                {
                }

                explicit Value(bool val_a) : data_(val_a)
                {
                }
                explicit Value(std::vector<Value> val_a) : data_(std::move(val_a))
                {
                }

                Value() : data_(std::monostate {})
                {
                }
            };

            class FPLParser
            {
            public:
                std::map<std::string, std::map<std::string, Value>> Parse(std::istream& input_a);

            private:
                char current_char_ = 0;
                std::istream* input_ = nullptr;
                std::string current_section_;
                std::map<std::string, std::map<std::string, Value>> result_;

            private:
                void SkipWhitespaceAndComments();
                char PeekNextChar();
                char GetNextChar();
                std::string ReadIdentifier();
                std::string ReadString(char delimiter_a);
                Value ParseValue();
                std::vector<Value> ParseArray();
                void ParseSection();
                void ParseKeyValuePair(std::map<std::string, Value>& current_map_a);
                double ParseNumber();
            };
        } // namespace FPLParser
    } // namespace Aid
} // namespace CE_Kernel
