// Copyright (c) 2025 Case Technologies

#pragma once
#include "AST/AST.hpp"

#include <functional>
#include <map>
#include <stack>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            unsigned int StringDistance(const std::string& a_a,
                                        const std::string& b_a);

            [[noreturn]]
            void RuntAzeErrNoActiveScope();

            [[noreturn]]
            void RuntAzeErrIdentAlreadyDeclared(
                    const std::string& ident_a,
                    const AST* prev_decl_ast_a = nullptr);

            template <typename SymbolType>
            AST* FetchASTFromSymbol(const SymbolType& symbol_a);

            template <typename T>
            struct GenericDefaultValue
            {
                static T Get()
                {
                    return nullptr;
                }
            };

            template <>
            struct GenericDefaultValue<bool>
            {
                static bool Get()
                {
                    return false;
                }
            };

            template <typename SymbolType>
            class SymbolTable
            {
            public:
                using OnOverrideProc =
                        std::function<bool(SymbolType& prev_symbol_)>;
                using OnReleaseProc =
                        std::function<void(const SymbolType& symbol_)>;
                using SearchPredicateProc =
                        std::function<bool(const SymbolType& symbol_)>;

                SymbolTable()
                {
                    OpenScope();
                }

                void OpenScope()
                {
                    scope_stack_.push({});
                    sym_table_anonymous_.push_back({});
                }

                void CloseScope(const OnReleaseProc& release_proc_a = nullptr)
                {
                    if (!scope_stack_.empty())
                    {
                        for (const auto& ident_ : scope_stack_.top())
                        {
                            auto it_ = sym_table_.find(ident_);
                            if (it_ != sym_table_.end())
                            {
                                if (release_proc_a)
                                    release_proc_a(it_->second.top().symbol_);

                                it_->second.pop();
                                if (it_->second.empty())
                                {
                                    sym_table_.erase(it_);
                                }
                            }
                        }

                        if (release_proc_a)
                        {
                            for (const auto& sym_ : sym_table_anonymous_.back())
                                release_proc_a(sym_.symbol_);
                        }

                        scope_stack_.pop();
                        sym_table_anonymous_.pop_back();
                    }
                }

                bool Register(const std::string& ident_a,
                              SymbolType symbol_a,
                              const OnOverrideProc& override_proc_a = nullptr,
                              bool throw_on_failure_a = true)
                {
                    if (scope_stack_.empty())
                        RuntAzeErrNoActiveScope();

                    if (ident_a.empty())
                    {
                        sym_table_anonymous_.back().push_back(
                                {symbol_a, ScopeLevel()});
                    } 
                    else
                    {
                        auto it_ = sym_table_.find(ident_a);
                        if (it_ != sym_table_.end() && !it_->second.empty())
                        {
                            auto& entry_ = it_->second.top();
                            if (entry_.symbol_
                                && entry_.scope_level_ == ScopeLevel())
                            {
                                if (override_proc_a && override_proc_a(entry_.symbol_))
                                    return true;
                                else if (throw_on_failure_a)
                                    RuntAzeErrIdentAlreadyDeclared(
                                            ident_a,
                                            FetchASTFromSymbol(entry_.symbol_));
                                else
                                    return false;
                            }
                        }

                        sym_table_[ident_a].push({symbol_a, ScopeLevel()});
                        scope_stack_.top().push_back(ident_a);
                    }

                    return true;
                }

                SymbolType Fetch(const std::string& ident_a) const
                {
                    auto it_ = sym_table_.find(ident_a);
                    if (it_ != sym_table_.end() && !it_->second.empty())
                        return it_->second.top().symbol_;
                    else
                        return GenericDefaultValue<SymbolType>::Get();
                }

                SymbolType FetchFromCurrentScope(const std::string& ident_a) const
                {
                    auto it_ = sym_table_.find(ident_a);
                    if (it_ != sym_table_.end() && !it_->second.empty())
                    {
                        const auto& sym_ = it_->second.top();
                        if (sym_.scope_level_ == ScopeLevel())
                            return sym_.symbol_;
                    }
                    return GenericDefaultValue<SymbolType>::Get();
                }

                SymbolType Find(
                        const SearchPredicateProc& search_predicate_a) const
                {
                    if (search_predicate_a)
                    {
                        for (const auto& sym_ : sym_table_)
                        {
                            if (!sym_.second.empty())
                            {
                                const auto& sym_ref_ = sym_.second.top().symbol_;
                                if (search_predicate_a(sym_ref_))
                                    return sym_ref_;
                            }
                        }

                        if (!sym_table_anonymous_.empty())
                        {
                            for (auto scope_ = sym_table_anonymous_.rbegin();
                                 scope_ != sym_table_anonymous_.rend();
                                 ++scope_)
                            {
                                for (const auto& sym_ : *scope_)
                                {
                                    if (search_predicate_a(sym_.symbol_))
                                        return sym_.symbol_;
                                }
                            }
                        }
                    }
                    return GenericDefaultValue<SymbolType>::Get();
                }

                std::string FetchSAzilar(const std::string& ident_a) const
                {
                    const std::string* sAzilar_ = nullptr;
                    unsigned int dist_ = (unsigned int)~0;

                    for (const auto& symbol_ : sym_table_)
                    {
                        auto d_ = StringDistance(ident_a, symbol_.first);
                        if (d_ < dist_)
                        {
                            sAzilar_ = (&symbol_.first);
                            dist_ = d_;
                        }
                    }

                    if (sAzilar_ != nullptr && dist_ < ident_a.size())
                        return *sAzilar_;

                    return "";
                }

                std::size_t ScopeLevel() const
                {
                    return scope_stack_.size();
                }

                bool InsideGlobalScope() const
                {
                    return (ScopeLevel() == 1);
                }

            private:
                struct Symbol
                {
                    SymbolType symbol_;
                    std::size_t scope_level_;
                };

                std::map<std::string, std::stack<Symbol>> sym_table_;
                std::vector<std::vector<Symbol>> sym_table_anonymous_;
                std::stack<std::vector<std::string>> scope_stack_;
            };

            using ASTSymbolTable = SymbolTable<AST*>;

            class ASTSymbolOverload
            {
            public:
                ASTSymbolOverload(const std::string& ident_a, AST* ast_a);

                bool AddSymbolRef(AST* ast_a);
                AST* Fetch(bool throw_on_failure_a = true) const;
                VarDecl* FetchVarDecl(bool throw_on_failure_a = true) const;
                Decl* FetchType(bool throw_on_failure_a = true) const;

                FunctionDecl* FetchFunctionDecl(
                        bool throw_on_failure_a = true) const;
                
                FunctionDecl* FetchFunctionDecl(
                        const std::vector<TypeDenoterPtr>& arg_type_denoters_a)
                        const;

            private:
                std::string ident_;
                std::vector<AST*> refs_;
            };

            using ASTSymbolOverloadPtr = std::shared_ptr<ASTSymbolOverload>;
            using ASTSymbolOverloadTable = SymbolTable<ASTSymbolOverloadPtr>;

            template <typename SymbolType>
            AST* FetchASTFromSymbol(const SymbolType& symbol_a)
            {
                (void)symbol_a;
                return nullptr;
            }

            template <>
            inline AST* FetchASTFromSymbol<ASTSymbolOverloadPtr>(
                    const ASTSymbolOverloadPtr& symbol_a)
            {
                return symbol_a->Fetch(false);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel