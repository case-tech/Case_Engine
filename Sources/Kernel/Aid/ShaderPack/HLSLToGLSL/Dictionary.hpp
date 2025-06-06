// Copyright (c) 2025 Case Technologies

#pragma once
#include <algorithm>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            template <typename T>
            class Dictionary
            {
            public:
                Dictionary() = default;
                Dictionary(const Dictionary&) = default;

                Dictionary(
                        const std::initializer_list<std::pair<std::string, T>>&
                                string_to_enum_pairs_a)
                    : string_to_enum_ {string_to_enum_pairs_a.begin(),
                                       string_to_enum_pairs_a.end()}
                {
                    std::size_t max_index_ = 0;

                    for (const auto& pair_ : string_to_enum_pairs_a)
                        max_index_ = std::max(max_index_,
                                            static_cast<std::size_t>(
                                                    pair_.second));

                    enum_to_string_.resize(max_index_ + 1, nullptr);

                    for (const auto& pair_ : string_to_enum_pairs_a)
                    {
                        const auto idx_ = static_cast<std::size_t>(pair_.second);
                        if (enum_to_string_[idx_] == nullptr)
                        {
                            auto it_ = string_to_enum_.find(pair_.first);
                            if (it_ != string_to_enum_.end())
                                enum_to_string_[idx_] = &(it_->first);
                        }
                    }
                }

                const T* StringToEnum(const std::string& s_a) const
                {
                    auto it_ = string_to_enum_.find(s_a);
                    if (it_ != string_to_enum_.end())
                        return &(it_->second);
                    else
                        return nullptr;
                }

                T StringToEnumOrDefault(const std::string& s_a,
                                        const T& default_value_a) const
                {
                    auto it_ = string_to_enum_.find(s_a);
                    if (it_ != string_to_enum_.end())
                        return it_->second;
                    else
                        return default_value_a;
                }

                const std::string* EnumToString(const T& e_a) const
                {
                    const auto idx_ = static_cast<std::size_t>(e_a);
                    if (idx_ < enum_to_string_.size())
                        return enum_to_string_[idx_];
                    else
                        return nullptr;
                }

                std::string EnumToStringOrDefault(
                        const T& e_a,
                        const std::string& default_string_a) const
                {
                    const auto idx_ = static_cast<std::size_t>(e_a);
                    if (idx_ < enum_to_string_.size_())
                    {
                        if (auto s_ = enum_to_string_[idx_])
                            return *s_;
                    }

                    return default_string_a;
                }

            private:
                std::map<std::string, T> string_to_enum_;
                std::vector<const std::string*> enum_to_string_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel