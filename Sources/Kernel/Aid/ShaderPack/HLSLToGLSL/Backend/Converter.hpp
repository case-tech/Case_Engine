// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/Identifier.hpp"
#include "../AST/TypeDenoter.hpp"
#include "../AST/Visitor/VisitorTracker.hpp"
#include "../HTG.hpp"
#include "../SymbolTable.hpp"

#include <stack>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Converter : public VisitorTracker
            {
            public:
                bool ConvertAST(Program& program_a,
                                const ShaderInput& input_desc_a,
                                const ShaderOutput& output_desc_a);

            protected:
                virtual void ConvertASTPrimary(
                        Program& program_a,
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a) = 0;

                void OpenScope();
                void CloseScope();
                void Register(const std::string& ident_a);

                bool Fetch(const std::string& ident_a) const;
                bool FetchFromCurrentScope(const std::string& ident_a) const;

                void PushSelfParameter(VarDecl* parameter_a);
                void PopSelfParameter();

                VarDecl* ActiveSelfParameter() const;

                void RenameIdent(Identifier& ident_a);
                void RenameIdentObfuscated(Identifier& ident_a);
                void RenameIdentOf(Decl* obj);
                void RenameIdentOfInOutVarDecls(
                        const std::vector<VarDecl*>& var_decls_a,
                        bool input_a,
                        bool use_semantic_only_a = false);
                
                void LabelAnonymousDecl(Decl* decl_obj_a);
                void VisitScopedStmnt(StmntPtr& stmnt_a, void* args_a = nullptr);
                void VisitScopedStmntList(std::vector<StmntPtr>& stmnt_list_a,
                                          void* args_a = nullptr);
                
                void InsertStmntBefore(const StmntPtr& stmnt_a,
                                       bool global_scope_a = false);
                
                void InsertStmntAfter(const StmntPtr& stmnt_a,
                                      bool global_scope_a = false);
                
                void MoveNestedStructDecls(std::vector<StmntPtr>& local_stmnts_a,
                                           bool global_scope_a = false);

                bool IsGlobalInOutVarDecl(VarDecl* var_decl_a) const;
                bool IsSamplerStateTypeDenoter(
                        const TypeDenoterPtr& type_denoter_a) const;
                
                void RemoveDeadCode(std::vector<StmntPtr>& stmnts_a);
                std::string MakeTempVarIdent();

                inline Program* GetProgram() const
                {
                    return program_;
                }

                inline const NameMangling& GetNameMangling() const
                {
                    return name_mangling_;
                }

            private:
                class StmntScopeHandler
                {
                public:
                    StmntScopeHandler(const StmntScopeHandler&) = default;
                    StmntScopeHandler& operator=(const StmntScopeHandler&) =
                            default;

                    StmntScopeHandler(StmntPtr& stmnt_a);
                    StmntScopeHandler(std::vector<StmntPtr>& stmnts_a);

                    Stmnt* Next();

                    void InsertStmntBefore(const StmntPtr& stmnt_a);
                    void InsertStmntAfter(const StmntPtr& stmnt_a);

                private:
                    void EnsureStmntList();
                    void InsertStmntAt(const StmntPtr& stmnt_a, std::size_t pos_a);

                    StmntPtr* stmnt_ = nullptr;
                    std::vector<StmntPtr>* stmnt_list_ = nullptr;
                    std::size_t idx_ = 0;
                };

                void VisitScopedStmntsFromHandler(
                        const StmntScopeHandler& handler_a,
                        void* args_a);

            private:
                StmntScopeHandler& ActiveStmntScopeHandler();
                SymbolTable<bool> sym_table_;

                Program* program_ = nullptr;
                NameMangling name_mangling_;

                std::vector<VarDecl*> self_param_stack_;

                std::stack<StmntScopeHandler> stmnt_scope_handler_stack_;
                StmntScopeHandler* stmnt_scope_handler_global_ref_ = nullptr;

                unsigned int anonym_counter_ = 0;
                unsigned int obfuscation_counter_ = 0;
                unsigned int temp_var_counter_ = 0;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
