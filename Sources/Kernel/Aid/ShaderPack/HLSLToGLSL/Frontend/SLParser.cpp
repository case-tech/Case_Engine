#include "SLParser.hpp"

#include "../AST/AST.hpp"
#include "../AST/ASTFactory.hpp"
#include "../AST/Visitor/ExprEvaluator.hpp"
#include "../Exception.hpp"
#include "../Helper.hpp"
#include "../Report/ReportIdents.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            SLParser::SLParser(Log* log_a) : Parser {log_a}
            {}

            void SLParser::Semi()
            {
                Accept(Tokens::Semicolon);
            }

            ArrayDimensionPtr SLParser::ParseArrayDimension(
                    bool allow_dynamic_dimension_a)
            {
                auto ast_ = Make<ArrayDimension>();

                Accept(Tokens::LParen);

                if (Is(Tokens::RParen))
                {
                    if (!allow_dynamic_dimension_a)
                        Error(R_ExpectedExplicitArrayDim, false);
                    ast_->expr_ = Make<NullExpr>();
                } 
                else
                    ast_->expr_ = ParseExpr();

                Accept(Tokens::RParen);

                return UpdateSourceArea(ast_);
            }

            NullStmntPtr SLParser::ParseNullStmnt()
            {
                auto ast_ = Make<NullStmnt>();
                Semi();
                return ast_;
            }

            CodeBlockStmntPtr SLParser::ParseCodeBlockStmnt()
            {
                auto ast_ = Make<CodeBlockStmnt>();
                ast_->code_block_ = ParseCodeBlock();
                return ast_;
            }

            ForLoopStmntPtr SLParser::ParseForLoopStmnt()
            {
                auto ast_ = Make<ForLoopStmnt>();
                Accept(Tokens::For);
                Accept(Tokens::LBracket);

                ast_->init_stmnt_ = ParseForLoopInitializer();

                if (!Is(Tokens::Semicolon))
                    ast_->condition_ = ParseExprWithSequenceOpt();
                Semi();

                if (!Is(Tokens::RBracket))
                    ast_->iteration_ = ParseExprWithSequenceOpt();
                Accept(Tokens::RBracket);

                ast_->body_stmnt_ = ParseLocalStmnt();

                return ast_;
            }

            WhileLoopStmntPtr SLParser::ParseWhileLoopStmnt()
            {
                auto ast_ = Make<WhileLoopStmnt>();

                Accept(Tokens::While);

                Accept(Tokens::LBracket);
                ast_->condition_ = ParseExprWithSequenceOpt();
                Accept(Tokens::RBracket);

                ast_->body_stmnt_ = ParseLocalStmnt();
                return ast_;
            }

            DoWhileLoopStmntPtr SLParser::ParseDoWhileLoopStmnt()
            {
                auto ast_ = Make<DoWhileLoopStmnt>();
                Accept(Tokens::Do);
                ast_->body_stmnt_ = ParseLocalStmnt();

                Accept(Tokens::While);

                Accept(Tokens::LBracket);
                ast_->condition_ = ParseExprWithSequenceOpt();
                Accept(Tokens::RBracket);

                Semi();

                return ast_;
            }

            IfStmntPtr SLParser::ParseIfStmnt()
            {
                auto ast_ = Make<IfStmnt>();

                Accept(Tokens::If);

                Accept(Tokens::LBracket);
                ast_->condition_ = ParseExprWithSequenceOpt();
                Accept(Tokens::RBracket);

                ast_->body_stmnt_ = ParseLocalStmnt();

                if (Is(Tokens::Else))
                    ast_->else_stmnt_ = ParseElseStmnt();

                return ast_;
            }

            ElseStmntPtr SLParser::ParseElseStmnt()
            {
                auto ast_ = Make<ElseStmnt>();

                Accept(Tokens::Else);
                ast_->body_stmnt_ = ParseLocalStmnt();

                return ast_;
            }

            SwitchStmntPtr SLParser::ParseSwitchStmnt()
            {
                auto ast_ = Make<SwitchStmnt>();
                Accept(Tokens::Switch);

                Accept(Tokens::LBracket);
                ast_->selector_ = ParseExprWithSequenceOpt();
                Accept(Tokens::RBracket);

                Accept(Tokens::LCurly);
                ast_->cases_ = ParseSwitchCaseList();
                Accept(Tokens::RCurly);

                return ast_;
            }

            CtrlTransferStmntPtr SLParser::ParseCtrlTransferStmnt()
            {
                auto ast_ = Make<CtrlTransferStmnt>();
                auto ctrl_transfer_ = Accept(Tokens::CtrlTransfer)->Spell();
                ast_->transfer_ = StringToCtrlTransfer(ctrl_transfer_);

                UpdateSourceArea(ast_);

                Semi();

                return ast_;
            }

            ReturnStmntPtr SLParser::ParseReturnStmnt()
            {
                auto ast_ = Make<ReturnStmnt>();

                Accept(Tokens::Return);

                if (!Is(Tokens::Semicolon))
                    ast_->expr_ = ParseExprWithSequenceOpt();

                UpdateSourceArea(ast_);

                Semi();

                return ast_;
            }

            ExprStmntPtr SLParser::ParseExprStmnt(const ExprPtr& expr_a)
            {
                auto ast_ = Make<ExprStmnt>();

                if (expr_a)
                {
                    ast_->expr_ = expr_a;
                    ast_->area_ = expr_a->area_;
                } 
                else
                    ast_->expr_ = ParseExprWithSequenceOpt();

                Semi();

                return UpdateSourceArea(ast_);
            }

            ExprPtr SLParser::ParseExpr()
            {
                return ParseGenericExpr();
            }

            ExprPtr SLParser::ParseExprWithSequenceOpt()
            {
                auto ast_ = ParseExpr();

                if (Is(Tokens::Comma))
                    return ParseSequenceExpr(ast_);

                return ast_;
            }

            ExprPtr SLParser::ParseArrayIndex()
            {
                auto area_ = Tkn()->Area();

                Accept(Tokens::LParen);

                auto ast_ = ParseExpr();
                ast_->area_ = area_;

                Accept(Tokens::RParen);

                return UpdateSourceArea(ast_);
            }

            ExprPtr SLParser::ParseInitializer()
            {
                Accept(Tokens::AssignOp, "=");
                return ParseExpr();
            }

            SequenceExprPtr SLParser::ParseSequenceExpr(
                    const ExprPtr& first_expr_a)
            {
                auto ast_ = Make<SequenceExpr>();
                if (first_expr_a)
                    ast_->Append(first_expr_a);
                else
                    ast_->Append(ParseExpr());

                Accept(Tokens::Comma);

                ast_->Append(ParseExprWithSequenceOpt());

                return ast_;
            }

            ArrayExprPtr SLParser::ParseArrayExpr(const ExprPtr& expr_a)
            {
                auto ast_ = Make<ArrayExpr>();
                ast_->prefix_expr_ = expr_a;
                ast_->array_indices_ = ParseArrayIndexList();

                return UpdateSourceArea(ast_, expr_a.get());
            }

            InitializerExprPtr SLParser::ParseInitializerExpr()
            {
                auto ast_ = Make<InitializerExpr>();
                ast_->exprs_ = ParseInitializerList();
                return UpdateSourceArea(ast_);
            }

            std::vector<VarDeclPtr> SLParser::ParseVarDeclList(
                    VarDeclStmnt* decl_stmnt_ref_a,
                    TokenPtr first_ident_tkn_a)
            {
                std::vector<VarDeclPtr> var_decls_;

                while (true)
                {
                    var_decls_.push_back(
                            ParseVarDecl(decl_stmnt_ref_a, first_ident_tkn_a));
                    first_ident_tkn_a = nullptr;
                    if (Is(Tokens::Comma))
                        AcceptIt();
                    else
                        break;
                }

                return var_decls_;
            }

            std::vector<VarDeclStmntPtr> SLParser::ParseParameterList()
            {
                std::vector<VarDeclStmntPtr> parameters_;

                Accept(Tokens::LBracket);

                if (!Is(Tokens::RBracket))
                {
                    while (true)
                    {
                        parameters_.push_back(ParseParameter());
                        if (Is(Tokens::Comma))
                            AcceptIt();
                        else
                            break;
                    }
                }

                Accept(Tokens::RBracket);

                return parameters_;
            }

            std::vector<StmntPtr> SLParser::ParseLocalStmntList()
            {
                std::vector<StmntPtr> stmnts_;

                while (!Is(Tokens::RCurly))
                {
                    ParseStmntWithCommentOpt(stmnts_, [this]() {
                        return this->ParseLocalStmnt();
                    });
                }

                return stmnts_;
            }

            std::vector<ExprPtr> SLParser::ParseExprList(
                    const Tokens list_terminator_token_a,
                    bool allow_last_comma_a)
            {
                std::vector<ExprPtr> exprs_;
                if (!Is(list_terminator_token_a))
                {
                    while (true)
                    {
                        exprs_.push_back(ParseExpr());
                        if (Is(Tokens::Comma))
                        {
                            AcceptIt();
                            if (allow_last_comma_a && Is(list_terminator_token_a))
                                break;
                        } 
                        else
                            break;
                    }
                }

                return exprs_;
            }

            std::vector<ArrayDimensionPtr> SLParser::ParseArrayDimensionList(
                    bool allow_dynamic_dimension_a)
            {
                std::vector<ArrayDimensionPtr> array_dims_;

                while (Is(Tokens::LParen))
                    array_dims_.push_back(
                            ParseArrayDimension(allow_dynamic_dimension_a));

                return array_dims_;
            }

            std::vector<ExprPtr> SLParser::ParseArrayIndexList()
            {
                std::vector<ExprPtr> exprs_;

                while (Is(Tokens::LParen))
                    exprs_.push_back(ParseArrayIndex());

                return exprs_;
            }

            std::vector<ExprPtr> SLParser::ParseArgumentList()
            {
                Accept(Tokens::LBracket);
                auto exprs_ = ParseExprList(Tokens::RBracket);
                Accept(Tokens::RBracket);
                return exprs_;
            }

            std::vector<ExprPtr> SLParser::ParseInitializerList()
            {
                Accept(Tokens::LCurly);
                auto exprs_ = ParseExprList(Tokens::RCurly, true);
                Accept(Tokens::RCurly);
                return exprs_;
            }

            std::vector<SwitchCasePtr> SLParser::ParseSwitchCaseList()
            {
                std::vector<SwitchCasePtr> cases_;

                while (Is(Tokens::Case) || Is(Tokens::Default))
                    cases_.push_back(ParseSwitchCase());

                return cases_;
            }

            std::string SLParser::ParseIdent(TokenPtr ident_tkn_a,
                                             SourceArea* area_a)
            {
                if (!ident_tkn_a)
                    ident_tkn_a = Accept(Tokens::Ident);

                auto ident_ = ident_tkn_a->Spell();

                if (area_a)
                    *area_a = ident_tkn_a->Area();

                if (auto prefix_ = FindNameManglingPrefix(ident_))
                    Error(R_IdentNameManglingConflict(ident_, *prefix_),
                          ident_tkn_a.get(),
                          false);

                return ident_;
            }

            TypeDenoterPtr SLParser::ParseTypeDenoterWithArrayOpt(
                    const TypeDenoterPtr& base_type_denoter_a)
            {
                if (Is(Tokens::LParen))
                {
                    auto array_type_denoter_ = std::make_shared<ArrayTypeDenoter>(
                            base_type_denoter_a);
                    array_type_denoter_->array_dims_ = ParseArrayDimensionList();

                    return array_type_denoter_;
                }
                return base_type_denoter_a;
            }

            VoidTypeDenoterPtr SLParser::ParseVoidTypeDenoter()
            {
                Accept(Tokens::Void);
                return std::make_shared<VoidTypeDenoter>();
            }

            Variant SLParser::ParseAndEvaluateConstExpr()
            {
                auto tkn1_ = Tkn();
                auto expr_ = ParseExpr();

                try
                {
                    ExprEvaluator expr_evaluator_;
                    return expr_evaluator_.Evaluate(
                            *expr_,
                            [](ObjectExpr* expr_) -> Variant { throw expr_; });
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), tkn1_.get());
                } 
                catch (const ObjectExpr* expr_)
                {
                    GetReportHandler().SubmitReport(true,
                                                    ReportTypes::Error,
                                                    R_SyntaxError,
                                                    R_ExpectedConstExpr,
                                                    GetScanner().Source(),
                                                    expr_->area_);
                }

                return Variant();
            }

            int SLParser::ParseAndEvaluateConstExprInt()
            {
                auto tkn1_ = Tkn();
                auto value_ = ParseAndEvaluateConstExpr();

                if (value_.Type() != Variant::Types::Int)
                    Error(R_ExpectedConstIntExpr, tkn1_.get());

                return static_cast<int>(value_.Int());
            }

            int SLParser::ParseAndEvaluateVectorDimension()
            {
                auto tkn1_ = Tkn();
                auto value_ = ParseAndEvaluateConstExprInt();

                if (value_ < 1 || value_ > 4)
                    Error(R_VectorAndMatrixDimOutOfRange(value_), tkn1_.get());

                return value_;
            }

            void SLParser::ParseStmntWithCommentOpt(
                    std::vector<StmntPtr>& stmnts_a,
                    const std::function<StmntPtr()>& parse_function_a)
            {
                auto comment_ = GetScanner().GetComment();
                auto ast_ = parse_function_a();
                stmnts_a.push_back(ast_);

                ast_->comment_ = std::move(comment_);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel