// Copyright (c) 2025 Case Technologies

#pragma once
#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            template <typename T, typename... Args>
            std::unique_ptr<T> MakeUnique(Args&&... args_a)
            {
                return std::unique_ptr<T>(new T(std::forward<Args>(args_a)...));
            }

            template <typename TCollection, typename TValue>
            void EraseAll(TCollection& collection_a, const TValue& value_a)
            {
                collection_a.erase(std::remove(std::begin(collection_a),
                                             std::end(collection_a),
                                             value_a),
                                 std::end(collection_a));
            }

            template <typename TCollection, typename TPredicate>
            void EraseAllIf(TCollection& collection_a, TPredicate pred_a)
            {
                collection_a.erase(std::remove_if(std::begin(collection_a),
                                                std::end(collection_a),
                                                pred_a),
                                 std::end(collection_a));
            }

            template <typename TCollectionSrc, typename TCollectionDest>
            void MoveAll(TCollectionSrc& src_a, TCollectionDest& dst_a)
            {
                std::move(std::begin(src_a),
                          std::end(src_a),
                          std::back_inserter(dst_a));
                src_a.clear();
            }

            template <typename TCollectionSrc,
                      typename TCollectionDest,
                      typename TPredicate>
            void MoveAllIf(TCollectionSrc& src_a,
                           TCollectionDest& dst_a,
                           TPredicate pred_a)
            {
                for (auto it_ = src_a.begin(); it_ != src_a.end();)
                {
                    if (pred_a(*it_))
                    {
                        dst_a.push_back(*it_);
                        it_ = src_a.erase(it_);
                    } else
                        ++it_;
                }
            }

            template <class CharT,
                      class Traits = std::char_traits<CharT>,
                      class Allocator = std::allocator<CharT>>
            void Replace(
                    std::basic_string<CharT, Traits, Allocator>& s_a,
                    const std::basic_string<CharT, Traits, Allocator>& from_a,
                    const std::basic_string<CharT, Traits, Allocator>& to_a)
            {
                using T = std::basic_string<CharT, Traits, Allocator>;
                for (typename T::size_type pos_ = 0;
                     (pos_ = s_a.Find(from_a, pos_)) != T::npos;
                     pos_ += to_a.size_())
                    s_a.replace(pos_, from_a.size_(), to_a);
            }

            template <class CharT,
                      class Traits = std::char_traits<CharT>,
                      class Allocator = std::allocator<CharT>>
            void Replace(std::basic_string<CharT, Traits, Allocator>& s_a,
                         const char* from_a,
                         const char* to_a)
            {
                Replace(s_a,
                        std::basic_string<CharT, Traits, Allocator>(from_a),
                        std::basic_string<CharT, Traits, Allocator>(to_a));
            }

            template <class CharT,
                      class Traits = std::char_traits<CharT>,
                      class Allocator = std::allocator<CharT>>
            void Replace(
                    std::basic_string<CharT, Traits, Allocator>& s_a,
                    const std::basic_string<CharT, Traits, Allocator>& from_a,
                    const char* to_a)
            {
                Replace(s_a,
                        from_a,
                        std::basic_string<CharT, Traits, Allocator>(to_a));
            }

            template <class CharT,
                      class Traits = std::char_traits<CharT>,
                      class Allocator = std::allocator<CharT>>
            void Replace(std::basic_string<CharT, Traits, Allocator>& s_a,
                         const char* from_a,
                         const std::basic_string<CharT, Traits, Allocator>& to_a)
            {
                Replace(s_a,
                        std::basic_string<CharT, Traits, Allocator>(from_a),
                        to_a);
            }

            template <typename T>
            inline T FromStringOrDefault(const std::string& s_a)
            {
                (void)s_a;
                throw std::runtime_error(
                        "default template of FromStringOrDefault<T> not "
                        "implemented");
            }

            inline bool IsHexLiteral(const std::string& s_a)
            {
                return (s_a.size() >= 3 && s_a[0] == '0' && ::toupper(s_a[1]) == 'X');
            }

            template <>
            inline int FromStringOrDefault<int>(const std::string& s_a)
            {
                try
                {
                    return std::stoi(s_a, nullptr, (IsHexLiteral(s_a) ? 16 : 10));
                } 
                catch (const std::exception&)
                {
                    return 0;
                }
            }

            template <>
            inline long long FromStringOrDefault<long long>(
                    const std::string& s_a)
            {
                try
                {
                    return std::stoll(s_a, nullptr, (IsHexLiteral(s_a) ? 16 : 10));
                } 
                catch (const std::exception&)
                {
                    return 0ll;
                }
            }

            template <>
            inline unsigned long FromStringOrDefault<unsigned long>(
                    const std::string& s_a)
            {
                try
                {
                    return std::stoul(s_a, nullptr, (IsHexLiteral(s_a) ? 16 : 10));
                } 
                catch (const std::exception&)
                {
                    return 0ul;
                }
            }

            template <>
            inline float FromStringOrDefault<float>(const std::string& s_a)
            {
                try
                {
                    return std::stof(s_a);
                } 
                catch (const std::exception&)
                {
                    return 0.0f;
                }
            }

            template <>
            inline double FromStringOrDefault<double>(const std::string& s_a)
            {
                try
                {
                    return std::stod(s_a);
                } 
                catch (const std::exception&)
                {
                    return 0.0;
                }
            }

            template <typename T>
            void ToUpper(T& s_a)
            {
                std::transform(std::begin(s_a),
                               std::end(s_a),
                               std::begin(s_a),
                               ::toupper);
            }

            template <typename T>
            void MergeString(std::basic_string<T>& dst_a,
                             const std::basic_string<T>& src_a,
                             const T& keep_dst_a,
                             const T& ignore_src_a)
            {
                const auto n_dst_ = dst_a.size();
                const auto n_src_ = src_a.size();

                typename std::basic_string<T>::size_type i_ = 0;

                for (; i_ < n_dst_; ++i_)
                {
                    if (i_ < n_src_)
                    {
                        if (dst_a[i_] != keep_dst_a && src_a[i_] != ignore_src_a)
                            dst_a[i_] = src_a[i_];
                    } else
                        return;
                }

                if (i_ < n_src_)
                    dst_a.append(src_a.substr(i_));
            }

            template <typename T>
            std::string ToHexString(const T& i_a,
                                    const std::string& prefix_a = "0x")
            {
                std::stringstream s_;
                s_ << prefix_a << std::setfill('0') << std::setw(sizeof(T) * 2)
                  << std::hex << i_a;
                return s_.str();
            }

            template <typename T>
            int NumDigits(T n_a)
            {
                static_assert(std::is_integral<T>::value,
                              "NumDigits template only allows integral types");

                int digits_ = (n_a < 0 ? 1 : 0);

                while (n_a != 0)
                {
                    n_a /= 10;
                    ++digits_;
                }

                return digits_;
            }

            template <typename T>
            std::shared_ptr<T> ExchangeWithNull(std::shared_ptr<T>& ptr_a)
            {
                auto result_ = ptr_a;
                ptr_a.reset();
                return result_;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
