#include "Converter.hpp"

#include "../AST/AST.hpp"
#include "../AST/ASTFactory.hpp"
#include "../Helper.hpp"
#include "../Report/ReportIdents.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            bool Converter::ConvertAST(Program& program_a,
                                       const ShaderInput& input_desc_a,
                                       const ShaderOutput& output_desc_a)
            {
                program_ = (&program_a);
                name_mangling_ = output_desc_a.name_mangling_;

                ConvertASTPrimary(program_a, input_desc_a, output_desc_a);

                return true;
            }

            void Converter::OpenScope()
            {
                sym_table_.OpenScope();
            }

            void Converter::CloseScope()
            {
                sym_table_.CloseScope();
            }

            void Converter::Register(const std::string& ident_a)
            {
                sym_table_.Register(ident_a, true);
            }

            bool Converter::Fetch(const std::string& ident_a) const
            {
                return sym_table_.Fetch(ident_a);
            }

            bool Converter::FetchFromCurrentScope(
                    const std::string& ident_a) const
            {
                return sym_table_.FetchFromCurrentScope(ident_a);
            }

            void Converter::PushSelfParameter(VarDecl* parameter_a)
            {
                self_param_stack_.push_back(parameter_a);
            }

            void Converter::PopSelfParameter()
            {
                if (self_param_stack_.empty())
                    throw std::underflow_error(R_SelfParamStackUnderflow);
                else
                    return self_param_stack_.pop_back();
            }

            VarDecl* Converter::ActiveSelfParameter() const
            {
                return (self_param_stack_.empty() ? nullptr
                                                : self_param_stack_.back());
            }

            void Converter::RenameIdent(Identifier& ident_a)
            {
                ident_a.AppendPrefix(name_mangling_.temporary_prefix_);
            }

            void Converter::RenameIdentObfuscated(Identifier& ident_a)
            {
                ident_a = "_" + std::to_string(obfuscation_counter_);
                ++obfuscation_counter_;
            }

            void Converter::RenameIdentOf(Decl* decl_obj_a)
            {
                RenameIdent(decl_obj_a->ident_);
            }

            void Converter::RenameIdentOfInOutVarDecls(
                    const std::vector<VarDecl*>& var_decls_a,
                    bool input_a,
                    bool use_semantic_only_a)
            {
                for (auto var_decl_ : var_decls_a)
                {
                    if (use_semantic_only_a)
                        var_decl_->ident_ = var_decl_->semantic_.ToString();
                    else if (input_a)
                        var_decl_->ident_ = name_mangling_.input_prefix_
                                         + var_decl_->semantic_.ToString();
                    else
                        var_decl_->ident_ = name_mangling_.output_prefix_
                                         + var_decl_->semantic_.ToString();
                }
            }

            void Converter::LabelAnonymousDecl(Decl* decl_obj_a)
            {
                if (decl_obj_a && decl_obj_a->IsAnonymous())
                {
                    decl_obj_a->ident_ = name_mangling_.temporary_prefix_ + "anonym"
                                     + std::to_string(anonym_counter_);
                    ++anonym_counter_;
                }
            }

            void Converter::VisitScopedStmnt(StmntPtr& stmnt_a, void* args_a)
            {
                VisitScopedStmntsFromHandler({stmnt_a}, args_a);
            }

            void Converter::VisitScopedStmntList(
                    std::vector<StmntPtr>& stmnt_list_a,
                    void* args_a)
            {
                VisitScopedStmntsFromHandler({stmnt_list_a}, args_a);
            }

            void Converter::InsertStmntBefore(const StmntPtr& stmnt_a,
                                              bool global_scope_a)
            {
                if (global_scope_a)
                    stmnt_scope_handler_global_ref_->InsertStmntBefore(stmnt_a);
                else
                    ActiveStmntScopeHandler().InsertStmntBefore(stmnt_a);
            }

            void Converter::InsertStmntAfter(const StmntPtr& stmnt_a,
                                             bool global_scope_a)
            {
                if (global_scope_a)
                    stmnt_scope_handler_global_ref_->InsertStmntAfter(stmnt_a);
                else
                    ActiveStmntScopeHandler().InsertStmntAfter(stmnt_a);
            }

            void Converter::MoveNestedStructDecls(
                    std::vector<StmntPtr>& local_stmnts_a,
                    bool global_scope_a)
            {
                for (auto it_ = local_stmnts_a.begin(); it_ != local_stmnts_a.end();)
                {
                    if (auto var_decl_stmnt_ = (*it_)->As<VarDeclStmnt>())
                    {
                        if (var_decl_stmnt_->type_specifier_->struct_decl_ != nullptr)
                        {
                            auto struct_decl_stmnt_ =
                                    ASTFactory::MakeStructDeclStmnt(
                                            ExchangeWithNull(
                                                    var_decl_stmnt_->type_specifier_
                                                            ->struct_decl_));

                            InsertStmntBefore(struct_decl_stmnt_, global_scope_a);
                        }
                    } 
                    else if (auto basic_decl_stmnt_ =
                                       (*it_)->As<BasicDeclStmnt>())
                    {
                        if (basic_decl_stmnt_->decl_object_->Type()
                            == AST::Types::StructDecl)
                        {
                            InsertStmntBefore(*it_, global_scope_a);
                            it_ = local_stmnts_a.erase(it_);

                            continue;
                        }
                    }

                    ++it_;
                }
            }

            bool Converter::IsGlobalInOutVarDecl(VarDecl* var_decl_a) const
            {
                if (var_decl_a)
                {
                    auto entry_point_ = program_->entry_point_ref_;
                    return (entry_point_->input_semantics_.Contains(var_decl_a)
                            || entry_point_->output_semantics_.Contains(var_decl_a));
                }

                return false;
            }

            bool Converter::IsSamplerStateTypeDenoter(
                    const TypeDenoterPtr& type_denoter_a) const
            {
                if (type_denoter_a)
                {
                    if (auto sampler_type_den_ = type_denoter_a->GetAliased()
                                                      .As<SamplerTypeDenoter>())
                    {
                        return IsSamplerStateType(sampler_type_den_->sampler_type_);
                    }
                }

                return false;
            }

            void Converter::RemoveDeadCode(std::vector<StmntPtr>& stmnts_a)
            {
                for (auto it_ = stmnts_a.begin(); it_ != stmnts_a.end();)
                {
                    if ((*it_)->flags_(AST::IsDeadCode))
                        it_ = stmnts_a.erase(it_);
                    else
                        ++it_;
                }
            }

            std::string Converter::MakeTempVarIdent()
            {
                return name_mangling_.temporary_prefix_ + "temp"
                       + std::to_string(temp_var_counter_++);
            }

            void Converter::VisitScopedStmntsFromHandler(
                    const StmntScopeHandler& handler_a,
                    void* args_a)
            {
                stmnt_scope_handler_stack_.push(handler_a);

                if (!stmnt_scope_handler_global_ref_)
                    stmnt_scope_handler_global_ref_ = &(
                            stmnt_scope_handler_stack_.top());

                auto& active_handler_ = ActiveStmntScopeHandler();
                while (auto stmnt_ = active_handler_.Next())
                    Visit(stmnt_, args_a);

                stmnt_scope_handler_stack_.pop();

                if (stmnt_scope_handler_stack_.empty())
                    stmnt_scope_handler_global_ref_ = nullptr;
            }

            Converter::StmntScopeHandler& Converter::ActiveStmntScopeHandler()
            {
                if (stmnt_scope_handler_stack_.empty())
                    throw std::underflow_error(R_NoActiveStmntScopeHandler);
                else
                    return stmnt_scope_handler_stack_.top();
            }

            Converter::StmntScopeHandler::StmntScopeHandler(StmntPtr& stmnt_a)
                : stmnt_ {&stmnt_a}
            {}

            Converter::StmntScopeHandler::StmntScopeHandler(
                    std::vector<StmntPtr>& stmnts_a)
                : stmnt_list_ {&stmnts_a}
            {}

            Stmnt* Converter::StmntScopeHandler::Next()
            {
                if (stmnt_list_)
                {
                    if (idx_ < stmnt_list_->size())
                    {
                        return stmnt_list_->at(idx_++).get();
                    }
                } 
                else if (stmnt_)
                {
                    if (idx_ == 0)
                    {
                        ++idx_;
                        return stmnt_->get();
                    }
                }
                return nullptr;
            }

            void Converter::StmntScopeHandler::InsertStmntBefore(
                    const StmntPtr& stmnt_a)
            {
                EnsureStmntList();

                if (idx_ > 0)
                    InsertStmntAt(stmnt_a, idx_ - 1);
                else
                    InsertStmntAt(stmnt_a, idx_);

                ++idx_;
            }

            void Converter::StmntScopeHandler::InsertStmntAfter(
                    const StmntPtr& stmnt_a)
            {
                EnsureStmntList();
                InsertStmntAt(stmnt_a, idx_);
            }

            void Converter::StmntScopeHandler::EnsureStmntList()
            {
                if (!stmnt_list_)
                {
                    if (!stmnt_)
                        throw std::runtime_error(R_MissingScopedStmntRef);

                    auto single_stmnt_ = *stmnt_;
                    auto code_block_stmnt_ = ASTFactory::MakeCodeBlockStmnt(
                            single_stmnt_);

                    stmnt_list_ = &(code_block_stmnt_->code_block_->stmnts_);
                    *stmnt_ = code_block_stmnt_;
                }
            }

            void Converter::StmntScopeHandler::InsertStmntAt(
                    const StmntPtr& stmnt_a,
                    std::size_t pos_a)
            {
                if (stmnt_list_)
                {
                    if (pos_a < stmnt_list_->size())
                        stmnt_list_->insert(stmnt_list_->begin() + pos_a, stmnt_a);
                    else
                        stmnt_list_->push_back(stmnt_a);
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
