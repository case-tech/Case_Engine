#include "SymbolTable.hpp"

#include "Exception.hpp"
#include "Report/ReportHandler.hpp"
#include "Report/ReportIdents.hpp"

#include <algorithm>
#include <cctype>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            static unsigned int StringDistancePrimary(const std::string& lhs_a,
                                                      const std::string& rhs_a,
                                                      unsigned int shift_on_uneq_a)
            {
                static const unsigned int diff_uneq_case_eq_ = 1;
                static const unsigned int diff_uneq_ = 2;

                unsigned int diff_ = 0, sim_ = 0;
                std::size_t shift_ = 0;

                for (std::size_t i_ = 0;
                     (i_ + shift_ < lhs_a.size() && i_ < rhs_a.size());
                     ++i_)
                {
                    auto a_ = lhs_a[i_ + shift_];
                    auto b_ = rhs_a[i_];

                    if (a_ == b_)
                        sim_ += diff_uneq_;
                    else
                    {
                        if (std::toupper(a_) == std::toupper(b_))
                        {
                            diff_ += diff_uneq_case_eq_;
                            sim_ += diff_uneq_case_eq_;
                        } 
                        else
                        {
                            diff_ += diff_uneq_;
                            if (shift_on_uneq_a > 0)
                            {
                                --shift_on_uneq_a;
                                ++shift_;
                            }
                        }
                    }
                }

                return (diff_ >= sim_ ? ~0 : diff_);
            }

            unsigned int StringDistance(const std::string& a_a,
                                        const std::string& b_a)
            {
                static const unsigned int max_dist_ = (unsigned int)~0;
                static const unsigned int max_len_diff_ = 3;
                static const unsigned int max_shift_ = 2;

                if (a_a == b_a)
                    return 0;

                unsigned int dist_ = max_dist_;

                if ((a_a.size() == b_a.size())
                    || (a_a.size() > b_a.size()
                        && a_a.size() <= b_a.size() + max_len_diff_)
                    || (b_a.size() > a_a.size()
                        && b_a.size() <= a_a.size() + max_len_diff_))
                {
                    for (unsigned int shift_ = 0; shift_ <= max_shift_; ++shift_)
                    {
                        dist_ = std::min({dist_,
                                         StringDistancePrimary(a_a, b_a, shift_),
                                         StringDistancePrimary(b_a, a_a, shift_)});
                    }
                }

                return dist_;
            }

            [[noreturn]]
            void RuntimeErrNoActiveScope()
            {
                RuntimeErr(R_NoActiveScopeToRegisterSymbol);
            }

            [[noreturn]]
            void RuntimeErrIdentAlreadyDeclared(const std::string& ident_a,
                                                const AST* prev_decl_ast_a)
            {
                if (prev_decl_ast_a != nullptr)
                    RuntimeErr(R_IdentAlreadyDeclared(
                            ident_a,
                            prev_decl_ast_a->area_.Pos().ToString()));
                else
                    RuntimeErr(R_IdentAlreadyDeclared(ident_a));
            }

            ASTSymbolOverload::ASTSymbolOverload(const std::string& ident_a,
                                                 AST* ast_a)
                : ident_ {ident_a}
            {
                refs_.push_back(ast_a);
            }

            bool ASTSymbolOverload::AddSymbolRef(AST* ast_a)
            {
                if (!ast_a)
                    return false;

                if (!refs_.empty())
                {
                    if (refs_.front()->Type() != ast_a->Type())
                        return false;

                    if (ast_a->Type() != AST::Types::FunctionDecl)
                        return false;

                    auto new_func_decl_ = static_cast<FunctionDecl*>(ast_a);
                    if (new_func_decl_->IsForwardDecl())
                    {
                        for (auto ref_ : refs_)
                        {
                            auto func_decl_ = static_cast<FunctionDecl*>(ref_);
                            if (!func_decl_->IsForwardDecl()
                                && func_decl_->EqualsSignature(*new_func_decl_))
                            {
                                new_func_decl_->SetFuncImplRef(func_decl_);
                                break;
                            }
                        }
                        return true;
                    } 
                    else
                    {
                        for (auto& ref_ : refs_)
                        {
                            auto func_decl_ = static_cast<FunctionDecl*>(ref_);
                            if (func_decl_->EqualsSignature(*new_func_decl_))
                            {
                                if (func_decl_->IsForwardDecl())
                                {
                                    func_decl_->SetFuncImplRef(new_func_decl_);

                                    ref_ = new_func_decl_;
                                    return true;
                                } 
                                else
                                {
                                    return false;
                                }
                            }
                        }
                    }
                }

                refs_.push_back(ast_a);

                return true;
            }

            AST* ASTSymbolOverload::Fetch(bool throw_on_failure_a) const
            {
                if (throw_on_failure_a)
                {
                    if (refs_.empty())
                        RuntimeErr(R_UndefinedSymbol(ident_));
                    if (refs_.size() > 1)
                        RuntimeErr(R_AmbiguousSymbol(ident_));
                    return refs_.front();
                } else
                    return (refs_.size() == 1 ? refs_.front() : nullptr);
            }

            VarDecl* ASTSymbolOverload::FetchVarDecl(bool throw_on_failure_a) const
            {
                if (auto ref_ = Fetch(throw_on_failure_a))
                {
                    if (auto var_decl_ = ref_->As<VarDecl>())
                        return var_decl_;
                    if (throw_on_failure_a)
                        RuntimeErr(R_IdentIsNotVar(ident_));
                }
                return nullptr;
            }

            Decl* ASTSymbolOverload::FetchType(bool throw_on_failure_) const
            {
                if (auto ref_ = Fetch(throw_on_failure_))
                {
                    auto type_ = ref_->Type();
                    if (type_ == AST::Types::StructDecl
                        || type_ == AST::Types::AliasDecl)
                        return static_cast<Decl*>(ref_);
                    if (throw_on_failure_)
                        RuntimeErr(R_IdentIsNotType(ident_));
                }
                return nullptr;
            }

            FunctionDecl* ASTSymbolOverload::FetchFunctionDecl(
                    bool throw_on_failure_a) const
            {
                auto ref_ = Fetch(throw_on_failure_a);
                if (auto func_decl_ = ref_->As<FunctionDecl>())
                    return func_decl_;
                else if (throw_on_failure_a)
                    RuntimeErr(R_IdentIsNotFunc(ident_));
                return nullptr;
            }

            FunctionDecl* ASTSymbolOverload::FetchFunctionDecl(
                    const std::vector<TypeDenoterPtr>& arg_type_denoters_a) const
            {
                if (refs_.empty())
                    RuntimeErr(R_UndefinedSymbol(ident_));
                if (refs_.front()->Type() != AST::Types::FunctionDecl)
                    RuntimeErr(R_IdentIsNotFunc(ident_));

                std::vector<FunctionDecl*> func_decl_list_;
                func_decl_list_.reserve(refs_.size());

                for (auto ref_ : refs_)
                {
                    if (auto func_decl_ = ref_->As<FunctionDecl>())
                        func_decl_list_.push_back(func_decl_);
                    else
                        RuntimeErr(R_AmbiguousSymbol(ident_));
                }

                return FunctionDecl::FetchFunctionDeclFromList(func_decl_list_,
                                                               ident_,
                                                               arg_type_denoters_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel