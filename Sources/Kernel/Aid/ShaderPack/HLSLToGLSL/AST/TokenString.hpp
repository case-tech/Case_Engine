// Copyright (c) 2025 Case Technologies

#pragma once
#include "Token.hpp"

#include <ostream>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void AssertReachedEnd(bool reached_end_a);
            void AssertCurrentTokenType(const Token::Types type_a,
                                        const Token::Types expected_type_a);

            template <typename TokenType, typename TokenOfInterestFunctor>
            class BasicTokenString
            {
            public:
                using ValueType = TokenType;
                using Container = std::vector<TokenType>;

                class ConstIterator
                {
                public:
                    ConstIterator() = default;

                    ConstIterator(const ConstIterator&) = default;
                    ConstIterator& operator=(const ConstIterator&) = default;

                    ConstIterator(
                            const typename Container::const_iterator& it_a,
                            const typename Container::const_iterator& it_end_a)
                        : it_ {it_a}, it_end_ {it_end_a}
                    {
                        NextTokenOfInterest();
                    }

                    ConstIterator& operator++()
                    {
                        if (it_ != it_end_)
                        {
                            ++it_;
                            NextTokenOfInterest();
                        }
                        return *this;
                    }

                    ConstIterator operator++(int)
                    {
                        ConstIterator tmp_ = *this;
                        operator++();
                        return tmp_;
                    }

                    bool ReachedEnd() const
                    {
                        return (it_ == it_end_);
                    }

                    ValueType AcceptIt()
                    {
                        AssertReachedEnd(ReachedEnd());
                        return *((*this)++);
                    }

                    ValueType Accept(const Token::Types type_a)
                    {
                        AssertReachedEnd(ReachedEnd());
                        AssertCurrentTokenType((*it_)->Type(), type_a);
                        return AcceptIt();
                    }

                    const ValueType& operator*()
                    {
                        return *it_;
                    }

                    typename Container::const_iterator operator->()
                    {
                        return it_;
                    }

                private:
                    void NextTokenOfInterest()
                    {
                        while (it_ != it_end_
                               && !TokenOfInterestFunctor::IsOfInterest(*it_))
                            ++it_;
                    }

                    typename Container::const_iterator it_, it_end_;
                };

                BasicTokenString() = default;
                BasicTokenString(const BasicTokenString&) = default;
                BasicTokenString& operator=(const BasicTokenString&) = default;

                BasicTokenString(const TokenType& token_a)
                {
                    PushBack(token_a);
                }

                ConstIterator Begin() const
                {
                    return ConstIterator(tokens_.begin(), tokens_.end());
                }

                ConstIterator End() const
                {
                    return ConstIterator(tokens_.end(), tokens_.end());
                }

                void PushBack(const TokenType& token_a)
                {
                    tokens_.push_back(token_a);
                }

                void PushBack(const BasicTokenString& token_string_a)
                {
                    tokens_.insert(tokens_.end(),
                                   token_string_a.tokens_.begin(),
                                   token_string_a.tokens_.end());
                }

                void PopFront()
                {
                    tokens_.erase(tokens_.begin());
                }

                void PopBack()
                {
                    tokens_.pop_back();
                }

                const ValueType& Front() const
                {
                    return tokens_.front();
                }

                const ValueType& Back() const
                {
                    return tokens_.back();
                }

                bool Empty() const
                {
                    return tokens_.empty();
                }

                const Container& GetTokens() const
                {
                    return tokens_;
                }

                Container& GetTokens()
                {
                    return tokens_;
                }

                void TrimFront()
                {
                    while (!Empty()
                           && !TokenOfInterestFunctor::IsOfInterest(Front()))
                        PopFront();
                }

                void TrimBack()
                {
                    while (!Empty()
                           && !TokenOfInterestFunctor::IsOfInterest(Back()))
                        PopBack();
                }

            private:
                Container tokens_;
            };

            template <typename TokenType, typename TokenOfInterestFunctor>
            bool operator==(const BasicTokenString<TokenType,
                                                   TokenOfInterestFunctor>& lhs_a,
                            const BasicTokenString<TokenType,
                                                   TokenOfInterestFunctor>& rhs_a)
            {
                auto lhs_it_ = lhs_a.Begin();
                auto rhs_it_ = rhs_a.Begin();

                for (; (!lhs_it_.ReachedEnd() && !rhs_it_.ReachedEnd());
                     ++lhs_it_, ++rhs_it_)
                {
                    auto lhs_tkn_ = lhs_it_->get();
                    auto rhs_tkn_ = rhs_it_->get();

                    if (lhs_tkn_->Type() != rhs_tkn_->Type())
                        return false;

                    if (lhs_tkn_->Spell() != rhs_tkn_->Spell())
                        return false;
                }

                return (lhs_it_.ReachedEnd() && rhs_it_.ReachedEnd());
            }

            template <typename TokenType, typename TokenOfInterestFunctor>
            bool operator!=(const BasicTokenString<TokenType,
                                                   TokenOfInterestFunctor>& lhs_a,
                            const BasicTokenString<TokenType,
                                                   TokenOfInterestFunctor>& rhs_a)
            {
                return !(lhs_a == rhs_a);
            }

            template <typename TokenType, typename TokenOfInterestFunctor>
            std::ostream& operator<<(
                    std::ostream& lhs_a,
                    const BasicTokenString<TokenType, TokenOfInterestFunctor>&
                            rhs_a)
            {
                for (const auto& tkn_ : rhs_a.GetTokens())
                    lhs_a << tkn_->Spell();
                return lhs_a;
            }

            struct DefaultTokenOfInterestFunctor
            {
                static bool IsOfInterest(const TokenPtr& token_a);
            };

            using TokenPtrString =
                    BasicTokenString<TokenPtr, DefaultTokenOfInterestFunctor>;
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel