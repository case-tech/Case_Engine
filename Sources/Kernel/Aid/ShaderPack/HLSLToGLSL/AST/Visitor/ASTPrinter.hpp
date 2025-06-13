// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../CodeWriter.hpp"
#include "../../HTG.hpp"
#include "../Token.hpp"
#include "Visitor.hpp"

#include <iostream>
#include <map>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ASTPrinter : private Visitor
            {
            public:
                void PrintAST(Program* program_a,
                              std::ostream& output_a = std::cout);

            private:
                struct PrintableTree
                {
                    std::string row_;
                    std::string col_;
                    std::string label_;
                    std::string value_;
                    std::vector<PrintableTree> children_;
                };

                DECL_VISIT_PROC(Program);
                DECL_VISIT_PROC(CodeBlock);
                DECL_VISIT_PROC(Attribute);
                DECL_VISIT_PROC(SwitchCase);
                DECL_VISIT_PROC(SamplerValue);
                DECL_VISIT_PROC(Register);
                DECL_VISIT_PROC(PackOffset);
                DECL_VISIT_PROC(ArrayDAzension);
                DECL_VISIT_PROC(TypeSpecifier);

                DECL_VISIT_PROC(VarDecl);
                DECL_VISIT_PROC(BufferDecl);
                DECL_VISIT_PROC(SamplerDecl);
                DECL_VISIT_PROC(StructDecl);
                DECL_VISIT_PROC(AliasDecl);
                DECL_VISIT_PROC(FunctionDecl);

                DECL_VISIT_PROC(VarDeclStmnt);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(SamplerDeclStmnt);
                DECL_VISIT_PROC(AliasDeclStmnt);
                DECL_VISIT_PROC(BasicDeclStmnt);

                DECL_VISIT_PROC(NullStmnt);
                DECL_VISIT_PROC(CodeBlockStmnt);
                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(ElseStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);
                DECL_VISIT_PROC(CtrlTransferStmnt);
                DECL_VISIT_PROC(LayoutStmnt);

                DECL_VISIT_PROC(NullExpr);
                DECL_VISIT_PROC(SequenceExpr);
                DECL_VISIT_PROC(LiteralExpr);
                DECL_VISIT_PROC(TypeSpecifierExpr);
                DECL_VISIT_PROC(TernaryExpr);
                DECL_VISIT_PROC(BinaryExpr);
                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(PostUnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(BracketExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(ArrayExpr);
                DECL_VISIT_PROC(CastExpr);
                DECL_VISIT_PROC(InitializerExpr);

                template <typename T>
                void VisitMember(T ast_a, const std::string& name_a)
                {
                    if (ast_a)
                    {
                        PushMemberName(name_a);
                        ast_a->Visit(this, nullptr);
                        PopMemberName();
                    }
                }

                template <typename T>
                void VisitMember(const std::vector<T>& ast_list_a,
                                 const std::string& name_a)
                {
                    for (std::size_t i_ = 0; i_ < ast_list_a.size(); ++i_)
                        VisitMember(ast_list_a[i_],
                                    name_a + "[" + std::to_string(i_) + "]");
                }

                std::string WriteLabel(const std::string& ast_name_a,
                                       TypedAST* ast_a = nullptr);

                void Print(const PrintableTree& tree_a, std::ostream& output_a);

                bool PushPrintable(const AST* ast_a,
                                   const std::string& label_a,
                                   const std::string& value_a = "");
                void PopPrintable();

                void Printable(const AST* ast_a,
                               const std::string& label_a,
                               const std::string& value_a = "");

                PrintableTree* TopPrintable();

                void PushMemberName(const std::string& name_a);
                void PopMemberName();

                const std::string& TopMemberName() const;

            private:
                PrintableTree tree_root_;
                std::stack<PrintableTree*> parent_node_stack_;
                std::vector<bool> last_sub_node_stack_;
                std::stack<std::string> member_name_stack_;

                std::size_t max_row_str_len_ = 0, max_col_str_len_ = 0;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
