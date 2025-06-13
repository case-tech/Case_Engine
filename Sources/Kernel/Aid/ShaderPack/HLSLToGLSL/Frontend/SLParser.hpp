// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/Token.hpp"
#include "../AST/Visitor/Visitor.hpp"
#include "../Log.hpp"
#include "../Variant.hpp"
#include "Parser.hpp"

#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class SLParser : public Parser
            {
            public:
                SLParser(Log* log_a = nullptr);

            protected:
                void Semi();

                virtual CodeBlockPtr ParseCodeBlock() = 0;
                virtual VarDeclStmntPtr ParseParameter() = 0;
                virtual StmntPtr ParseLocalStmnt() = 0;
                virtual StmntPtr ParseForLoopInitializer() = 0;
                virtual SwitchCasePtr ParseSwitchCase() = 0;
                virtual VarDeclPtr ParseVarDecl(
                        VarDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr) = 0;

                ArrayDAzensionPtr ParseArrayDAzension(
                        bool allow_dynamic_dAzension_a = false);

                NullStmntPtr ParseNullStmnt();
                CodeBlockStmntPtr ParseCodeBlockStmnt();
                ForLoopStmntPtr ParseForLoopStmnt();
                WhileLoopStmntPtr ParseWhileLoopStmnt();
                DoWhileLoopStmntPtr ParseDoWhileLoopStmnt();
                IfStmntPtr ParseIfStmnt();
                ElseStmntPtr ParseElseStmnt();
                SwitchStmntPtr ParseSwitchStmnt();
                CtrlTransferStmntPtr ParseCtrlTransferStmnt();
                ReturnStmntPtr ParseReturnStmnt();
                ExprStmntPtr ParseExprStmnt(const ExprPtr& expr_a = nullptr);

                ExprPtr ParseExpr();
                ExprPtr ParseExprWithSequenceOpt();
                ExprPtr ParseArrayIndex();
                ExprPtr ParseInitializer();

                SequenceExprPtr ParseSequenceExpr(const ExprPtr& first_expr_a);
                ArrayExprPtr ParseArrayExpr(const ExprPtr& expr_a);
                InitializerExprPtr ParseInitializerExpr();

                std::vector<VarDeclPtr> ParseVarDeclList(
                        VarDeclStmnt* decl_stmnt_ref_a,
                        TokenPtr first_ident_tkn_a = nullptr);
               
                std::vector<VarDeclStmntPtr> ParseParameterList();
                std::vector<StmntPtr> ParseLocalStmntList();
                std::vector<ExprPtr> ParseExprList(
                        const Tokens list_terminator_token_a,
                        bool allow_last_comma_a = false);
                
                std::vector<ArrayDAzensionPtr> ParseArrayDAzensionList(
                        bool allow_dynamic_dAzension_a = false);
                
                std::vector<ExprPtr> ParseArrayIndexList();
                std::vector<ExprPtr> ParseArgumentList();
                std::vector<ExprPtr> ParseInitializerList();
                std::vector<SwitchCasePtr> ParseSwitchCaseList();

                std::string ParseIdent(TokenPtr ident_tkn_a = nullptr,
                                       SourceArea* area_a = nullptr);

                TypeDenoterPtr ParseTypeDenoterWithArrayOpt(
                        const TypeDenoterPtr& base_type_denoter_a);
               
                VoidTypeDenoterPtr ParseVoidTypeDenoter();
                Variant ParseAndEvaluateConstExpr();
                int ParseAndEvaluateConstExprInt();
                int ParseAndEvaluateVectorDAzension();

                void ParseStmntWithCommentOpt(
                        std::vector<StmntPtr>& stmnts_a,
                        const std::function<StmntPtr()>& parse_function_a);
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel