// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../Flags.hpp"
#include "../../HTG.hpp"
#include "../TypeDenoter.hpp"
#include "VisitorTracker.hpp"

#include <functional>
#include <set>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ExprConverter : public VisitorTracker
            {
            public:
                enum : unsigned int
                {
                    ConvertVectorSubscripts = (1 << 0),
                    ConvertVectorCompare = (1 << 1),
                    ConvertImageAccess = (1 << 2),
                    ConvertImplicitCasts = (1 << 3),
                    ConvertInitializerToCtor = (1 << 4),
                    ConvertLog10 = (1 << 5),
                    ConvertUnaryExpr = (1 << 6),
                    ConvertSamplerBufferAccess = (1 << 7),
                    ConvertMatrixLayout = (1 << 8),
                    ConvertTextureBracketOp = (1 << 9),
                    ConvertTextureIntrinsicVec4 = (1 << 10),
                    ConvertMatrixSubscripts = (1 << 11),
                    ConvertCompatibleStructs = (1 << 12),
                    ConvertLiteralHalfToFloat = (1 << 13),

                    AllPreVisit = (ConvertVectorCompare | ConvertImageAccess
                                   | ConvertLog10 | ConvertSamplerBufferAccess
                                   | ConvertTextureBracketOp
                                   | ConvertCompatibleStructs),

                    AllPostVisit = (ConvertVectorSubscripts
                                    | ConvertMatrixSubscripts
                                    | ConvertTextureIntrinsicVec4),

                    All = (~0u),
                };

                void Convert(Program& program_a,
                             const Flags& conversion_flags_a,
                             const NameMangling& name_mangling_a);

                static void ConvertExprIfCastRequired(
                        ExprPtr& expr_a,
                        const DataType target_type_a,
                        bool match_type_size_a = true);
                
                static void ConvertExprIfCastRequired(
                        ExprPtr& expr_a,
                        const TypeDenoter& target_type_den_a,
                        bool match_type_size_a = true);

                static int GetTextureDimFromExpr(Expr* expr_a,
                                                 const AST* ast_a = nullptr);
                
                static std::string GetMatrixSubscriptWrapperIdent(
                        const NameMangling& name_mangling_a,
                        const MatrixSubscriptUsage& subscript_usage_a);

            private:
                DECL_VISIT_PROC(VarDecl);

                DECL_VISIT_PROC(FunctionDecl);

                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);

                DECL_VISIT_PROC(LiteralExpr);
                DECL_VISIT_PROC(TernaryExpr);
                DECL_VISIT_PROC(BinaryExpr);
                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(BracketExpr);
                DECL_VISIT_PROC(CastExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(ArrayExpr);

                void ConvertExpr(ExprPtr& expr_a, const Flags& flags_a);
                void ConvertExprList(std::vector<ExprPtr>& expr_list_a,
                                     const Flags& flags_a);
                
                void ConvertExprVectorSubscript(ExprPtr& expr_a);
                void ConvertExprVectorSubscriptObject(ExprPtr& expr_a,
                                                      ObjectExpr* object_expr_a);
                
                void ConvertExprMatrixSubscript(ExprPtr& expr_a);
                void ConvertExprMatrixSubscriptObject(ExprPtr& expr_a,
                                                      ObjectExpr* object_expr_a);
                
                void ConvertExprVectorCompare(ExprPtr& expr_a);
                void ConvertExprVectorCompareUnary(ExprPtr& expr_a,
                                                   UnaryExpr* unary_expr_a);
                
                void ConvertExprVectorCompareBinary(ExprPtr& expr_a,
                                                    BinaryExpr* binary_expr_a);
                
                void ConvertExprVectorCompareTernary(ExprPtr& expr_a,
                                                     TernaryExpr* ternary_expr_a);

                void ConvertExprImageAccess(ExprPtr& expr_a);
                void ConvertExprImageAccessAssign(ExprPtr& expr_a,
                                                  AssignExpr* assign_expr_a);
                
                void ConvertExprImageAccessArray(
                        ExprPtr& expr_a,
                        ArrayExpr* array_expr_a,
                        AssignExpr* assign_expr_a = nullptr);

                void ConvertExprSamplerBufferAccess(ExprPtr& expr_a);
                void ConvertExprSamplerBufferAccessArray(ExprPtr& expr_a,
                                                         ArrayExpr* array_expr_a);
                
                void ConvertExprIntoBracket(ExprPtr& expr_a);
                void ConvertExprIntrinsicCallLog10(ExprPtr& expr_a);
                void ConvertExprTargetType(ExprPtr& expr_a,
                                           const TypeDenoter& target_type_den_a,
                                           bool match_type_size_a = true);

                void ConvertExprTargetTypeInitializer(
                        ExprPtr& expr_a,
                        InitializerExpr* init_expr_a,
                        const TypeDenoter& target_type_den_a);

                void ConvertExprFormatInitializer(
                        ExprPtr& expr_a,
                        InitializerExpr* init_expr_a,
                        const TypeDenoter& target_type_den_a);

                void ConvertExprTextureBracketOp(ExprPtr& expr_a);
                void ConvertExprTextureIntrinsicVec4(ExprPtr& expr_a);
                void ConvertExprCompatibleStruct(ExprPtr& expr_a);

            private:
                Flags conversion_flags_;
                NameMangling name_mangling_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
