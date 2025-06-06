#include "FPLParser.hpp"

#include <algorithm>
#include <cctype>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace FPLParser
        {
            void FPLParser::SkipWhitespaceAndComments()
            {
                while (true) 
                {
                    while (std::isspace(current_char_))
                        current_char_ = GetNextChar();

                    if (current_char_ == '#') 
                    {
                        while (current_char_ != '\n' && current_char_ != EOF)
                            current_char_ = GetNextChar();
                    } 
                    else
                        break;
                }
            }

            char FPLParser::PeekNextChar()
            {
                return char(input_->peek());
            }

            char FPLParser::GetNextChar()
            {
                current_char_ = char(input_->get());
                return current_char_;
            }

            std::string FPLParser::ReadIdentifier()
            {
                std::string ident_;
                while (std::isalnum(current_char_) || current_char_ == '_') 
                {
                    ident_ += current_char_;
                    current_char_ = GetNextChar();
                }

                return ident_;
            }

            std::string FPLParser::ReadString(char delimiter_a)
            {
                std::string str_;
                current_char_ = GetNextChar();
                while (current_char_ != delimiter_a) 
                {
                    if (current_char_ == '\\') 
                    {
                        current_char_ = GetNextChar();
                        switch (current_char_) 
                        {
                        case 'n':
                            str_ += '\n';
                            break;
                        case 't':
                            str_ += '\t';
                            break;
                        default:
                            str_ += current_char_;
                        }
                    } 

                    else 
                    {
                        str_ += current_char_;
                    }

                    current_char_ = GetNextChar();
                    if (current_char_ == EOF)
                        throw std::runtime_error("Unterminated string");
                }

                current_char_ = GetNextChar();
                return str_;
            }

            double FPLParser::ParseNumber()
            {
                std::string num_str_;
                bool has_dot_ = false;
                bool has_exp_ = false;

                if (current_char_ == '-' || current_char_ == '+') 
                {
                    num_str_ += current_char_;
                    current_char_ = GetNextChar();
                }

                while (std::isdigit(current_char_)) 
                {
                    num_str_ += current_char_;
                    current_char_ = GetNextChar();
                    if (current_char_ == '.' && !has_dot_ && !has_exp_) 
                    {
                        num_str_ += '.';
                        has_dot_ = true;
                        current_char_ = GetNextChar();
                    }

                    if ((current_char_ == 'e' || current_char_ == 'E') && !has_exp_) 
                    {
                        num_str_ += current_char_;
                        has_exp_ = true;
                        current_char_ = GetNextChar();
                        if (current_char_ == '+' || current_char_ == '-') 
                        {
                            num_str_ += current_char_;
                            current_char_ = GetNextChar();
                        }
                    }
                }

                return std::stod(num_str_);
            }

            std::vector<Value> FPLParser::ParseArray()
            {
                std::vector<Value> arr_;
                current_char_ = GetNextChar();
                while (true) 
                {
                    SkipWhitespaceAndComments();
                    if (current_char_ == ']') 
                    {
                        current_char_ = GetNextChar();
                        break;
                    }

                    arr_.push_back(ParseValue());
                    SkipWhitespaceAndComments();
                    if (current_char_ == ',') 
                    {
                        current_char_ = GetNextChar();
                        continue;
                    }
                }

                return arr_;
            }

            Value FPLParser::ParseValue()
            {
                SkipWhitespaceAndComments();

                if (current_char_ == '"' || current_char_ == '\'') 
                {
                    return Value(ReadString(current_char_));
                } 

                else if (std::isdigit(current_char_) || current_char_ == '-' || current_char_ == '+') 
                {
                    return Value(ParseNumber());
                } 

                else if (std::isalpha(current_char_)) 
                {
                    std::string ident_ = ReadIdentifier();
                    if (ident_ == "true")
                        return Value(true);
                    if (ident_ == "false")
                        return Value(false);
                    if (ident_ == "null")
                        return Value();
                    throw std::runtime_error("Unexpected identifier: " + ident_);
                } 
                
                else if (current_char_ == '[') 
                {
                    return Value(ParseArray());
                } 

                else if (current_char_ == '@') 
                {
                    std::string section_name_ = ReadIdentifier();
                    ParseSection();

                    return Value(section_name_);
                } 

                else 
                {
                    throw std::runtime_error("Unexpected character: " + std::string(1, current_char_));
                }
            }

            void FPLParser::ParseKeyValuePair(std::map<std::string, Value>& current_map_a)
            {
                std::string key_ = ReadIdentifier();
                SkipWhitespaceAndComments();
                if (current_char_ != ':')
                    throw std::runtime_error("Expected ':' after key");

                current_char_ = GetNextChar();
                Value val_ = ParseValue();
                current_map_a.emplace(std::move(key_),  std::move(val_));
            }

            void FPLParser::ParseSection()
            {
                current_section_ = ReadIdentifier();
                SkipWhitespaceAndComments();

                if (current_char_ != '{')
                    throw std::runtime_error("Expected '{'");
                current_char_ = GetNextChar();

                std::map<std::string, Value> section_data_;
                while (true) 
                {
                    SkipWhitespaceAndComments();
                    if (current_char_ == '}') 
                    {
                        current_char_ = GetNextChar();
                        break;
                    } 

                    else if (current_char_ == '@') 
                    {
                        current_char_ = GetNextChar();
                        ParseSection();
                    } 

                    else 
                    {
                        ParseKeyValuePair(section_data_);
                    }
                }

                result_.emplace(std::move(current_section_), std::move(section_data_));
            }

            std::map<std::string, std::map<std::string, Value>> FPLParser::Parse(std::istream& input_a)
            {
                input_ = &input_a;
                current_char_ = GetNextChar();
                result_.clear();

                try 
                {
                    while (current_char_ != EOF) 
                    {
                        SkipWhitespaceAndComments();
                        if (current_char_ == '@') 
                        {
                            current_char_ = GetNextChar();
                            ParseSection();
                        } 

                        else if (current_char_ == EOF) 
                        {
                            break;
                        } 

                        else 
                        {
                            throw std::runtime_error("Unexpected character outside section: " + 
                                    std::string(1, current_char_));
                        }
                    }
                } 
                catch (const std::exception& e_) 
                {
                    throw std::runtime_error("Parse error: " + std::string(e_.what()));
                }

                return result_;
            }
        } // namespace FPLParser
    } // namespace Aid
} // namespace CE_Kernel
