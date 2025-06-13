// Copyright (c) 2025 Case Technologies

#pragma once
#include <memory>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            #define DECL_PTR(CLASS_NAME)                                                       \
                    struct CLASS_NAME;                                                         \
                    using CLASS_NAME##Ptr = std::shared_ptr<CLASS_NAME>

            DECL_PTR(AST);
            DECL_PTR(TypedAST);
            DECL_PTR(Stmnt);
            DECL_PTR(Expr);
            DECL_PTR(Decl);

            DECL_PTR(Program);
            DECL_PTR(CodeBlock);
            DECL_PTR(Attribute);
            DECL_PTR(SwitchCase);
            DECL_PTR(SamplerValue);
            DECL_PTR(Register);
            DECL_PTR(PackOffset);
            DECL_PTR(ArrayDAzension);
            DECL_PTR(TypeSpecifier);

            DECL_PTR(VarDecl);
            DECL_PTR(BufferDecl);
            DECL_PTR(SamplerDecl);
            DECL_PTR(StructDecl);
            DECL_PTR(AliasDecl);
            DECL_PTR(FunctionDecl);
            DECL_PTR(UniformBufferDecl);

            DECL_PTR(BufferDeclStmnt);
            DECL_PTR(SamplerDeclStmnt);
            DECL_PTR(VarDeclStmnt);
            DECL_PTR(AliasDeclStmnt);
            DECL_PTR(BasicDeclStmnt);

            DECL_PTR(NullStmnt);
            DECL_PTR(CodeBlockStmnt);
            DECL_PTR(ForLoopStmnt);
            DECL_PTR(WhileLoopStmnt);
            DECL_PTR(DoWhileLoopStmnt);
            DECL_PTR(IfStmnt);
            DECL_PTR(ElseStmnt);
            DECL_PTR(SwitchStmnt);
            DECL_PTR(ExprStmnt);
            DECL_PTR(ReturnStmnt);
            DECL_PTR(CtrlTransferStmnt);
            DECL_PTR(LayoutStmnt);

            DECL_PTR(NullExpr);
            DECL_PTR(SequenceExpr);
            DECL_PTR(LiteralExpr);
            DECL_PTR(TypeSpecifierExpr);
            DECL_PTR(TernaryExpr);
            DECL_PTR(BinaryExpr);
            DECL_PTR(UnaryExpr);
            DECL_PTR(PostUnaryExpr);
            DECL_PTR(CallExpr);
            DECL_PTR(BracketExpr);
            DECL_PTR(ObjectExpr);
            DECL_PTR(AssignExpr);
            DECL_PTR(ArrayExpr);
            DECL_PTR(CastExpr);
            DECL_PTR(InitializerExpr);

            #undef DECL_PTR

            #define VISITOR_VISIT_PROC(CLASS_NAME)                                         \
                virtual void Visit##CLASS_NAME(CLASS_NAME* ast_a, void* args_a)

            #define DECL_VISIT_PROC(CLASS_NAME)                                            \
                void Visit##CLASS_NAME(CLASS_NAME* ast_a, void* args_a) override

            #define VISIT_DEFAULT(CLASS_NAME) Visitor::Visit##CLASS_NAME(ast_a, args_a)

            class Visitor
            {
            public:
                virtual ~Visitor() = default;

                VISITOR_VISIT_PROC(Program);
                VISITOR_VISIT_PROC(CodeBlock);
                VISITOR_VISIT_PROC(Attribute);
                VISITOR_VISIT_PROC(SwitchCase);
                VISITOR_VISIT_PROC(SamplerValue);
                VISITOR_VISIT_PROC(Register);
                VISITOR_VISIT_PROC(PackOffset);
                VISITOR_VISIT_PROC(ArrayDAzension);
                VISITOR_VISIT_PROC(TypeSpecifier);

                VISITOR_VISIT_PROC(VarDecl);
                VISITOR_VISIT_PROC(BufferDecl);
                VISITOR_VISIT_PROC(SamplerDecl);
                VISITOR_VISIT_PROC(StructDecl);
                VISITOR_VISIT_PROC(AliasDecl);
                VISITOR_VISIT_PROC(FunctionDecl);
                VISITOR_VISIT_PROC(UniformBufferDecl);

                VISITOR_VISIT_PROC(BufferDeclStmnt);
                VISITOR_VISIT_PROC(SamplerDeclStmnt);
                VISITOR_VISIT_PROC(VarDeclStmnt);
                VISITOR_VISIT_PROC(AliasDeclStmnt);
                VISITOR_VISIT_PROC(BasicDeclStmnt);

                VISITOR_VISIT_PROC(NullStmnt);
                VISITOR_VISIT_PROC(CodeBlockStmnt);
                VISITOR_VISIT_PROC(ForLoopStmnt);
                VISITOR_VISIT_PROC(WhileLoopStmnt);
                VISITOR_VISIT_PROC(DoWhileLoopStmnt);
                VISITOR_VISIT_PROC(IfStmnt);
                VISITOR_VISIT_PROC(ElseStmnt);
                VISITOR_VISIT_PROC(SwitchStmnt);
                VISITOR_VISIT_PROC(ExprStmnt);
                VISITOR_VISIT_PROC(ReturnStmnt);
                VISITOR_VISIT_PROC(CtrlTransferStmnt);
                VISITOR_VISIT_PROC(LayoutStmnt);

                VISITOR_VISIT_PROC(NullExpr);
                VISITOR_VISIT_PROC(SequenceExpr);
                VISITOR_VISIT_PROC(LiteralExpr);
                VISITOR_VISIT_PROC(TypeSpecifierExpr);
                VISITOR_VISIT_PROC(TernaryExpr);
                VISITOR_VISIT_PROC(BinaryExpr);
                VISITOR_VISIT_PROC(UnaryExpr);
                VISITOR_VISIT_PROC(PostUnaryExpr);
                VISITOR_VISIT_PROC(CallExpr);
                VISITOR_VISIT_PROC(BracketExpr);
                VISITOR_VISIT_PROC(AssignExpr);
                VISITOR_VISIT_PROC(ObjectExpr);
                VISITOR_VISIT_PROC(ArrayExpr);
                VISITOR_VISIT_PROC(CastExpr);
                VISITOR_VISIT_PROC(InitializerExpr);

            protected:
                template <typename T>
                void Visit(const T& ast_a, void* args_a = nullptr)
                {
                    if (ast_a)
                        ast_a->Visit(this, args_a);
                }

                template <typename T>
                void Visit(const std::vector<T>& ast_list_a, void* args_a = nullptr)
                {
                    for (const auto& ast_ : ast_list_a)
                        Visit(ast_, args_a);
                }
            };

#undef VISITOR_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel