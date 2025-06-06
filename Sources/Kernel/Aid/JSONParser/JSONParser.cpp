#include "JSONParser.hpp"

#include <fstream>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace JSONParser
        {
            namespace
            {
                void ThrowParseError(const std::string& message_a, size_t position_a)
                {
                    throw JSONParseException(message_a, position_a);
                }

                bool IsDigit(char c_a)
                {
                    return c_a >= '0' && c_a <= '9';
                }

            } // namespace

            JSONParseException::JSONParseException(const std::string& message_a, size_t position_a)
            {
                message_ = "JSON parse error at position " + std::to_string(position_a) + ": " + message_a;
            }

            const char* JSONParseException::what() const noexcept
            {
                return message_.c_str();
            }

            std::string JSONNumber::to_string() const
            {
                std::stringstream ss_;
                if (value_ == floor(value_) && value_ <= 1e15 && value_ >= -1e15) 
                {
                    ss_ << static_cast<long long>(value_);
                }
                else 
                {
                    ss_ << value_;
                }

                return ss_.str();
            }

            std::string JSONString::to_string() const
            {
                std::string result_ = "\"";
                for (char c : value_) 
                {
                    switch (c) 
                    {
                    case '"':
                        result_ += "\\\"";
                        break;
                    case '\\':
                        result_ += "\\\\";
                        break;
                    case '\b':
                        result_ += "\\b";
                        break;
                    case '\f':
                        result_ += "\\f";
                        break;
                    case '\n':
                        result_ += "\\n";
                        break;
                    case '\r':
                        result_ += "\\r";
                        break;
                    case '\t':
                        result_ += "\\t";
                        break;

                    default:
                        result_ += c;
                        break;
                    }
                }

                return result_ + "\"";
            }

            std::string JSONArray::to_string() const
            {
                std::string result_ = "[";
                for (size_t i_ = 0; i_ < elements_.size(); ++i_) 
                {
                    if (i_ > 0)
                        result_ += ", ";

                    result_ += elements_[i_]->to_string();
                }

                return result_ + "]";
            }

            void JSONArray::Add(std::unique_ptr<JSONValue> value_a)
            {
                elements_.push_back(std::move(value_a));
            }

            size_t JSONArray::Size() const
            {
                return elements_.size();
            }

            JSONValue* JSONArray::At(size_t index_a) const
            {
                return elements_[index_a].get();
            }

            std::string JSONObject::to_string() const
            {
                std::string result_ = "{";
                bool first_ = true;
                for (const auto& pair_ : members_) 
                {
                    if (!first_)
                        result_ += ", ";

                    first_   = false;
                    result_ += "\"" + pair_.first + "\": " + pair_.second->to_string();
                }

                return result_ + "}";
            }

            void JSONObject::Add(const std::string& key_a, std::unique_ptr<JSONValue> value_a)
            {
                members_[key_a] = std::move(value_a);
            }

            bool JSONObject::Has(const std::string& key_a) const
            {
                return members_.count(key_a) > 0;
            }

            JSONValue* JSONObject::Get(const std::string& key_a) const
            {
                auto it_ = members_.find(key_a);
                return it_ != members_.end() ? it_->second.get() : nullptr;
            }

            std::unique_ptr<JSONValue> JSONParser::Parse(const std::string& input_a)
            {
                size_t index_ = 0;
                SkipWhiteSpace(input_a, index_);

                auto result_ = ParseValue(input_a, index_);
                SkipWhiteSpace(input_a, index_);

                if (index_ < input_a.size()) 
                {
                    ThrowParseError("Unexpected characters after JSON value.", index_);
                }

                return result_;
            }

            void JSONParser::SkipWhiteSpace(const std::string& input_a, size_t& index_a)
            {
                while (index_a < input_a.size() && std::isspace(input_a[index_a])) 
                {
                    ++index_a;
                }
            }

            std::unique_ptr<JSONValue> JSONParser::ParseValue(const std::string& input_a, size_t& index_a)
            {
                SkipWhiteSpace(input_a, index_a);
                if (index_a >= input_a.size()) 
                {
                    ThrowParseError("Unexpected end of input", index_a);
                }

                char c_ = input_a[index_a];
                switch (c_) 
                {
                case '{':
                    return ParseObject(input_a, index_a);
                case '[':
                    return ParseArray(input_a, index_a);
                case '"': 
                    {
                        auto str_ = ParseString(input_a, index_a);
                        return std::make_unique<JSONString>(str_);
                    }
                case 't':
                case 'f':
                case 'n':
                    return ParseKeyword(input_a, index_a);
                case '-':
                    if (index_a + 1 >= input_a.size() || !IsDigit(input_a[index_a + 1])) 
                    {
                        ThrowParseError("Invalid number format", index_a);
                    }
                    [[fallthrough]];
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    return ParseNumber(input_a, index_a);

                default:
                    ThrowParseError("Unexpected character", index_a);
                }

                return nullptr;
            }

            std::unique_ptr<JSONObject> JSONParser::ParseObject(const std::string& input_a, size_t& index_a)
            {
                auto obj_ = std::make_unique<JSONObject>();
                ++index_a; 
                SkipWhiteSpace(input_a, index_a);

                while (index_a < input_a.size() && input_a[index_a] != '}') 
                {
                    if (input_a[index_a] != '"')
                        ThrowParseError("Expected string key", index_a);

                    std::string key_ = ParseString(input_a, index_a);

                    SkipWhiteSpace(input_a, index_a);
                    if (index_a >= input_a.size() || input_a[index_a] != ':') 
                    {
                        ThrowParseError("Expected ':' after key", index_a);
                    }

                    ++index_a;

                    auto value_ = ParseValue(input_a, index_a);
                    obj_->Add(key_, std::move(value_));

                    SkipWhiteSpace(input_a, index_a);
                    if (index_a < input_a.size() && input_a[index_a] == ',') 
                    {
                        ++index_a;
                        SkipWhiteSpace(input_a, index_a);
                    } 
                    
                    else 
                    {
                        break;
                    }
                }

                if (index_a >= input_a.size() || input_a[index_a] != '}') 
                {
                    ThrowParseError("Expected '}' to end object", index_a);
                }

                ++index_a;

                return obj_;
            }

            std::unique_ptr<JSONArray> JSONParser::ParseArray(const std::string& input_a, size_t& index_a)
            {
                auto arr_ = std::make_unique<JSONArray>();
                ++index_a;
                SkipWhiteSpace(input_a, index_a);

                while (index_a < input_a.size() && input_a[index_a] != ']') 
                {
                    auto value_ = ParseValue(input_a, index_a);
                    arr_->Add(std::move(value_));

                    SkipWhiteSpace(input_a, index_a);
                    if (index_a < input_a.size() && input_a[index_a] == ',') 
                    {
                        ++index_a;
                        SkipWhiteSpace(input_a, index_a);
                    } 
                    
                    else 
                    {
                        break;
                    }
                }

                if (index_a >= input_a.size() || input_a[index_a] != ']')
                {
                    ThrowParseError("Expected ']' to end array", index_a);
                }

                ++index_a;

                return arr_;
            }

            std::string JSONParser::ParseString(const std::string& input_a, size_t& index_a)
            {
                std::string result_;
                ++index_a;

                while (index_a < input_a.size() && input_a[index_a] != '"') 
                {
                    char c_ = input_a[index_a];
                    if (c_ == '\\') 
                    {
                        ++index_a;
                        if (index_a >= input_a.size())
                            ThrowParseError("Unterminated escape sequence.", index_a);

                        switch (input_a[index_a]) 
                        {
                        case '"':
                            result_ += '"';
                            break;
                        case '\\':
                            result_ += '\\';
                            break;
                        case '/':
                            result_ += '/';
                            break;
                        case 'b':
                            result_ += '\b';
                            break;
                        case 'f':
                            result_ += '\f';
                            break;
                        case 'n':
                            result_ += '\n';
                            break;
                        case 'r':
                            result_ += '\r';
                            break;
                        case 't':
                            result_ += '\t';
                            break;
                        case 'u':
                            ThrowParseError("Unicode escape not supported.", index_a);
                            break;

                        default:
                            ThrowParseError("Invalid escape character.", index_a);
                            break;
                        }
                    } 
                    
                    else 
                    {
                        if (static_cast<unsigned char>(c_) < 0x20) 
                        {
                            ThrowParseError("Invalid control character in string.", index_a);
                        }

                        result_ += c_;
                    }

                    ++index_a;
                }

                if (index_a >= input_a.size())
                    ThrowParseError("Unterminated string", index_a);

                ++index_a;

                return result_;
            }

            std::unique_ptr<JSONNumber> JSONParser::ParseNumber(const std::string& input_a, size_t& index_a)
            {
                size_t start_ = index_a;

                if (input_a[index_a] == '-') 
                {
                    ++index_a;
                }

                if (input_a[index_a] == '0') 
                {
                    ++index_a;

                    if (index_a < input_a.size() && IsDigit(input_a[index_a])) 
                    {
                        ThrowParseError("Leading zero is not allowed", index_a);
                    }
                } 
                
                else 
                {
                    while (index_a < input_a.size() && IsDigit(input_a[index_a]))
                        ++index_a;
                }

                if (index_a < input_a.size() && input_a[index_a] == '.') 
                {
                    ++index_a;
                    if (index_a >= input_a.size() || !IsDigit(input_a[index_a])) 
                    {
                        ThrowParseError("Invalid fractional part.", index_a);
                    }

                    while (index_a < input_a.size() && IsDigit(input_a[index_a]))
                        ++index_a;
                }

                if (index_a < input_a.size() && (input_a[index_a] == 'e' || input_a[index_a] == 'E')) 
                {
                    ++index_a;
                    if (index_a < input_a.size() && (input_a[index_a] == '+' || input_a[index_a] == '-'))
                        ++index_a;

                    if (index_a >= input_a.size() || !IsDigit(input_a[index_a])) 
                    {
                        ThrowParseError("Invalid exponent part.", index_a);
                    }

                    while (index_a < input_a.size() && IsDigit(input_a[index_a]))
                        ++index_a;
                }

                try 
                {
                    double value_ = std::stod(input_a.substr(start_, index_a - start_));
                    return std::make_unique<JSONNumber>(value_);
                } 
                catch (...) 
                {
                    ThrowParseError("Invalid number format.", start_);
                }

                return nullptr;
            }

            std::unique_ptr<JSONValue> JSONParser::ParseKeyword(const std::string& input_a, size_t& index_a)
            {
                const size_t start_ = index_a;
                const char c_ = input_a[start_];

                try 
                {
                    if (c_ == 't') 
                    {
                        if (input_a.substr(start_, 4) == "true") 
                        {
                            index_a += 4;
                            return std::make_unique<JSONBool>(true);
                        }
                    } 
                    
                    else if (c_ == 'f') 
                    {
                        if (input_a.substr(start_, 5) == "false") 
                        {
                            index_a += 5;
                            return std::make_unique<JSONBool>(false);
                        }
                    } 
                    
                    else if (c_ == 'n') 
                    {
                        if (input_a.substr(start_, 4) == "null") 
                        {
                            index_a += 4;
                            return std::make_unique<JSONNull>();
                        }
                    }
                } 
                catch (const std::out_of_range&) 
                {
                    ThrowParseError("Incomplete keyword.", start_);
                }

                ThrowParseError("Invalid keyword.", start_);

                return nullptr;
            }

            std::unique_ptr<JSONValue> JSONParser::ParseFromFile(const std::string& path_to_file_a)
            {
                std::ifstream file_(path_to_file_a);
                if (!file_.is_open()) 
                {
                    throw JSONParseException("Failed to open file: " + path_to_file_a, 0);
                }

                std::stringstream buffer_;
                buffer_ << file_.rdbuf();
                file_.close();

                return Parse(buffer_.str());
            }
        } // namespace JSONParser
    } // namespace Aid
} // namespace CE_Kernel