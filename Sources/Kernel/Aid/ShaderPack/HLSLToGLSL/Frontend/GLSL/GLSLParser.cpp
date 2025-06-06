#include "GLSLParser.hpp"

#include "../../AST/AST.hpp"
#include "../../AST/ASTFactory.hpp"
#include "../../Exception.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "GLSLKeywords.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            GLSLParser::GLSLParser(Log* log_a) : SLParser {log_a}
            {}

            ProgramPtr GLSLParser::ParseSource(
                    const SourceCodePtr& source_a,
                    const NameMangling& name_mangling_a,
                    const InputShaderVersion version_in_a,
                    bool enable_warnings_a)
            {
                (void)version_in_a;
                EnableWarnings(enable_warnings_a);

                GetNameMangling() = name_mangling_a;

                PushScannerSource(source_a);

                try
                {
                    auto ast_ = ParseProgram(source_a);
                    return (GetReportHandler().HasErrors() ? nullptr : ast_);
                } 
                catch (const Report& err_)
                {
                    if (GetLog())
                        GetLog()->SubmitReport(err_);
                }

                return nullptr;
            }

            ScannerPtr GLSLParser::MakeScanner()
            {
                return std::make_shared<GLSLScanner>(GetLog());
            }

            bool GLSLParser::IsDataType() const
            {
                return (IsBaseDataType() || Is(Tokens::Buffer)
                        || Is(Tokens::Sampler) || Is(Tokens::SamplerState));
            }

            bool GLSLParser::IsBaseDataType() const
            {
                return (Is(Tokens::ScalarType) || Is(Tokens::VectorType)
                        || Is(Tokens::MatrixType) || Is(Tokens::StringType));
            }

            bool GLSLParser::IsLiteral() const
            {
                return (Is(Tokens::BoolLiteral) || Is(Tokens::IntLiteral)
                        || Is(Tokens::FloatLiteral));
            }

            bool GLSLParser::IsArithmeticUnaryExpr() const
            {
                return (Is(Tokens::BinaryOp, "-") || Is(Tokens::BinaryOp, "+"));
            }

            bool GLSLParser::IsModifier() const
            {
                return (Is(Tokens::InputModifier) || Is(Tokens::InterpModifier)
                        || Is(Tokens::TypeModifier)
                        || Is(Tokens::StorageClass));
            }

            TokenPtr GLSLParser::AcceptIt()
            {
                auto tkn_l_ = Parser::AcceptIt();

                while (Tkn()->Type() == Tokens::Directive)
                    ProcessDirective(AcceptIt()->Spell());

                return tkn_l_;
            }

            void GLSLParser::ProcessDirective(const std::string& ident_a)
            {
                try
                {
                    if (ident_a == "line")
                        ProcessDirectiveLine();
                    else if (ident_a == "version")
                        ProcessDirectiveVersion();
                    else if (ident_a == "extension")
                        ProcessDirectiveExtension();
                    else
                        RuntimeErr(R_InvalidGLSLDirectiveAfterPP);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
            }

            void GLSLParser::ProcessDirectiveLine()
            {
                int line_no_ = 0;
                std::string filename_;

                if (Is(Tokens::IntLiteral))
                    line_no_ = ParseIntLiteral(Parser::AcceptIt());
                else
                    ErrorUnexpected(Tokens::IntLiteral);

                if (Is(Tokens::StringLiteral))
                    filename_ = Parser::AcceptIt()->SpellContent();
                else
                    filename_ = GetScanner().Source()->Filename();

                auto current_line_ = static_cast<int>(
                        GetScanner().PreviousToken()->Pos().Row());
                GetScanner().Source()->NextSourceOrigin(filename_,
                                                        (line_no_ - current_line_
                                                         - 1));
            }

            void GLSLParser::ProcessDirectiveVersion()
            {
                if (Is(Tokens::IntLiteral))
                    version_ = ParseIntLiteral(Parser::AcceptIt());
                else
                    ErrorUnexpected(Tokens::IntLiteral);

                if (Is(Tokens::Ident))
                {
                    const auto profile_ = Parser::AcceptIt()->Spell();
                    if (profile_ == "es")
                        is_essl_ = true;
                    else if (profile_ == "core")
                        is_core_profile_ = true;
                    else if (profile_ == "compatibility")
                        is_core_profile_ = false;
                    else
                        Error(R_InvalidGLSLVersionProfile(profile_));
                }
            }

            void GLSLParser::ProcessDirectiveExtension()
            {
                std::string extension_, behavior_;

                if (Is(Tokens::Ident))
                    extension_ = Parser::AcceptIt()->Spell();
                else
                    ErrorUnexpected(Tokens::Ident);

                if (Is(Tokens::Colon))
                    Parser::AcceptIt();
                else
                    ErrorUnexpected(Tokens::Colon);

                if (Is(Tokens::Ident))
                    behavior_ = Parser::AcceptIt()->Spell();
                else
                    ErrorUnexpected(Tokens::Ident);
            }

            void GLSLParser::OpenScope()
            {
                type_name_symbol_table_.OpenScope();
            }

            void GLSLParser::CloseScope()
            {
                type_name_symbol_table_.CloseScope();
            }

            void GLSLParser::RegisterTypeName(const std::string& ident_a)
            {
                type_name_symbol_table_.Register(ident_a, true, nullptr, false);
            }

            bool GLSLParser::IsRegisteredTypeName(
                    const std::string& ident_a) const
            {
                return type_name_symbol_table_.Fetch(ident_a);
            }

            ProgramPtr GLSLParser::ParseProgram(const SourceCodePtr& source_a)
            {
                auto ast_ = Make<Program>();

                OpenScope();
                ast_->source_code_ = source_a;

                while (true)
                {
                    if (Is(Tokens::EndOfStream))
                        break;

                    ParseStmntWithCommentOpt(
                            ast_->global_stmnts_,
                            std::bind(&GLSLParser::ParseGlobalStmnt, this));
                }

                CloseScope();

                return ast_;
            }

            CodeBlockPtr GLSLParser::ParseCodeBlock()
            {
                auto ast_ = Make<CodeBlock>();

                Accept(Tokens::LCurly);
                OpenScope();
                {
                    ast_->stmnts_ = ParseLocalStmntList();
                }
                CloseScope();
                Accept(Tokens::RCurly);

                return ast_;
            }

            VarDeclStmntPtr GLSLParser::ParseParameter()
            {
                auto ast_ = Make<VarDeclStmnt>();
                ast_->type_specifier_ = ParseTypeSpecifier();

                ast_->var_decls_.push_back(ParseVarDecl(ast_.get()));
                ast_->flags_ << VarDeclStmnt::IsParameter;

                return UpdateSourceArea(ast_);
            }

            StmntPtr GLSLParser::ParseLocalStmnt()
            {
                return ParseStmnt();
            }

            StmntPtr GLSLParser::ParseForLoopInitializer()
            {
                return ParseStmnt();
            }

            SwitchCasePtr GLSLParser::ParseSwitchCase()
            {
                auto ast_ = Make<SwitchCase>();
                if (Is(Tokens::Case))
                {
                    Accept(Tokens::Case);
                    ast_->expr_ = ParseExpr();
                } 
                else
                    Accept(Tokens::Default);
                Accept(Tokens::Colon);

                while (!Is(Tokens::Case) && !Is(Tokens::Default)
                       && !Is(Tokens::RCurly))
                    ParseStmntWithCommentOpt(ast_->stmnts_,
                                             std::bind(&GLSLParser::ParseStmnt,
                                                       this));

                return ast_;
            }

            VarDeclPtr GLSLParser::ParseVarDecl(VarDeclStmnt* decl_stmnt_ref_a,
                                                const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<VarDecl>();
                ast_->decl_stmnt_ref_ = decl_stmnt_ref_a;
                ast_->ident_ = ParseIdent(ident_tkn_a, &ast_->area_);
                ast_->array_dims_ = ParseArrayDimensionList(true);

                if (Is(Tokens::AssignOp, "="))
                    ast_->initializer_ = ParseInitializer();

                return ast_;
            }

            AttributePtr GLSLParser::ParseAttribute()
            {
                auto ast_ = Make<Attribute>();
                auto attrib_ident_ = ParseIdent();
                ast_->attribute_type_ = GLSLKeywordToAttributeType(attrib_ident_);

                UpdateSourceArea(ast_);

                if (ast_->attribute_type_ == AttributeType::Undefined)
                    Error(R_UnknownLayoutQualifier(attrib_ident_));

                if (Is(Tokens::AssignOp, "="))
                {
                    AcceptIt();
                    ast_->arguments_.push_back(ParseExpr());
                }

                return ast_;
            }

            TypeSpecifierPtr GLSLParser::ParseTypeSpecifier(
                    bool parse_void_type_a,
                    const TokenPtr& input_tkn_a)
            {
                (void)parse_void_type_a;
                auto ast_ = Make<TypeSpecifier>();
                if (input_tkn_a)
                    ParseModifiers(ast_.get(), true, input_tkn_a);

                while (IsModifier() || Is(Tokens::PrimitiveType))
                    ParseModifiers(ast_.get(), true);

                ast_->type_denoter_ = ParseTypeDenoterWithStructDeclOpt(
                        ast_->struct_decl_);

                return UpdateSourceArea(ast_);
            }

            StructDeclPtr GLSLParser::ParseStructDecl(bool parse_struct_tkn_a,
                                                      const TokenPtr& ident_tkn_a)
            {
                auto tkn_l_ = Tkn();
                auto ast_ = Make<StructDecl>();

                if (parse_struct_tkn_a)
                {
                    Accept(Tokens::Struct);
                    UpdateSourceArea(ast_);
                }

                if (Is(Tokens::Ident) || ident_tkn_a)
                {
                    tkn_l_ = Tkn();
                    ast_->ident_ = (ident_tkn_a ? ident_tkn_a->Spell() : ParseIdent());
                    UpdateSourceArea(ast_);

                    RegisterTypeName(ast_->ident_);

                    if (Is(Tokens::Colon))
                        Error(R_IllegalInheritance);
                }

                GetReportHandler().PushContextDesc(ast_->ToString());
                {
                    ast_->local_stmnts_ = ParseGlobalStmntList();

                    for (auto& stmnt_ : ast_->local_stmnts_)
                    {
                        if (stmnt_->Type() == AST::Types::VarDeclStmnt)
                        {
                            ast_->var_members_.push_back(
                                    std::static_pointer_cast<VarDeclStmnt>(
                                            stmnt_));
                        } 
                        else
                            Error(R_IllegalDeclStmntInsideDeclOf(
                                          ast_->ToString()),
                                  stmnt_->area_,
                                  false);
                    }

                    for (auto& var_decl_stmnt_ : ast_->var_members_)
                    {
                        for (auto& var_decl_ : var_decl_stmnt_->var_decls_)
                            var_decl_->struct_decl_ref_ = ast_.get();
                    }
                }
                GetReportHandler().PopContextDesc();

                return ast_;
            }

            FunctionDeclPtr GLSLParser::ParseFunctionDecl(
                    BasicDeclStmnt* decl_stmnt_ref_a,
                    const TypeSpecifierPtr& return_type_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<FunctionDecl>();
                ast_->decl_stmnt_ref_ = decl_stmnt_ref_a;

                if (return_type_a)
                {
                    ast_->return_type_ = return_type_a;
                } 
                else
                {
                    if (Is(Tokens::Inline))
                        AcceptIt();

                    ast_->return_type_ = ParseTypeSpecifier(true);
                }

                if (ident_tkn_a)
                {
                    ast_->area_ = ident_tkn_a->Area();
                    ast_->ident_ = ident_tkn_a->Spell();
                } 
                else
                {
                    ast_->area_ = GetScanner().ActiveToken()->Area();
                    ast_->ident_ = ParseIdent();
                }

                ast_->parameters_ = ParseParameterList();
                if (Is(Tokens::Semicolon))
                    AcceptIt();
                else
                {
                    GetReportHandler().PushContextDesc(ast_->ToString(false));
                    {
                        ast_->code_block_ = ParseCodeBlock();
                    }
                    GetReportHandler().PopContextDesc();
                }

                return ast_;
            }

            UniformBufferDeclPtr GLSLParser::ParseUniformBufferDecl(
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_a = Make<UniformBufferDecl>();
                ast_a->buffer_type_ = UniformBufferType::ConstantBuffer;
                ast_a->ident_ = ParseIdent(ident_tkn_a);

                UpdateSourceArea(ast_a);

                GetReportHandler().PushContextDesc(ast_a->ToString());
                {
                    ast_a->local_stmnts_ = ParseGlobalStmntList();
                    for (auto& stmnt_ : ast_a->local_stmnts_)
                    {
                        if (stmnt_->Type() == AST::Types::VarDeclStmnt)
                            ast_a->var_members_.push_back(
                                    std::static_pointer_cast<VarDeclStmnt>(
                                            stmnt_));
                        else
                            Error(R_OnlyFieldsAllowedInUniformBlock,
                                  stmnt_->area_,
                                  false);
                    }

                    for (auto& var_decl_stmnt_ : ast_a->var_members_)
                    {
                        for (auto& var_decl_ : var_decl_stmnt_->var_decls_)
                            var_decl_->buffer_decl_ref_ = ast_a.get();
                    }

                    Semi();
                }
                GetReportHandler().PopContextDesc();

                return ast_a;
            }

            StmntPtr GLSLParser::ParseGlobalStmnt()
            {
                if (Is(Tokens::LayoutQualifier))
                {
                    auto attribs_ = ParseAttributeList();
                    auto ast_ = ParseGlobalStmntPrimary(!attribs_.empty());
                    ast_->attribs_ = std::move(attribs_);
                    return ast_;
                } 
                else
                {
                    return ParseGlobalStmntPrimary();
                }
            }

            StmntPtr GLSLParser::ParseGlobalStmntPrimary(bool has_attribs_a)
            {
                switch (TknType())
                {
                case Tokens::UniformBuffer:
                    return ParseUniformDeclStmnt();
                case Tokens::Struct:
                    return ParseStmntWithStructDecl();
                default:
                    if (has_attribs_a
                        && (Is(Tokens::InputModifier, "in")
                            || Is(Tokens::InputModifier, "out")))
                        return ParseGlobalStmntWithLayoutQualifier();
                    else
                        return ParseGlobalStmntWithTypeSpecifier();
                }
            }

            StmntPtr GLSLParser::ParseGlobalStmntWithTypeSpecifier(
                    const TokenPtr& input_tkn_a)
            {
                auto type_specifier_ = ParseTypeSpecifier(false, input_tkn_a);
                if (type_specifier_->struct_decl_ && Is(Tokens::Semicolon))
                {
                    auto ast_ = Make<BasicDeclStmnt>();

                    auto struct_decl_ = type_specifier_->struct_decl_;
                    struct_decl_->decl_stmnt_ref_ = ast_.get();

                    ast_->decl_object_ = struct_decl_;

                    Semi();

                    return ast_;
                }

                auto ident_tkn_ = Accept(Tokens::Ident);
                if (Is(Tokens::LBracket))
                {
                    return ParseFunctionDeclStmnt(type_specifier_, ident_tkn_);
                } 
                else
                {
                    auto ast_ = Make<VarDeclStmnt>();

                    ast_->type_specifier_ = type_specifier_;
                    ast_->var_decls_ = ParseVarDeclList(ast_.get(), ident_tkn_);

                    Semi();

                    return UpdateSourceArea(ast_, ast_->type_specifier_.get());
                }
            }

            StmntPtr GLSLParser::ParseGlobalStmntWithLayoutQualifier()
            {
                auto input_tkn_ = Accept(Tokens::InputModifier);

                if (Is(Tokens::Semicolon))
                {
                    AcceptIt();

                    auto ast_ = Make<LayoutStmnt>();
                    if (input_tkn_->Spell() == "in")
                        ast_->is_input_ = true;
                    else if (input_tkn_->Spell() == "out")
                        ast_->is_output_ = true;

                    return ast_;
                }

                return ParseGlobalStmntWithTypeSpecifier(input_tkn_);
            }

            BasicDeclStmntPtr GLSLParser::ParseFunctionDeclStmnt(
                    const TypeSpecifierPtr& return_type_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<BasicDeclStmnt>();
                ast_->decl_object_ = ParseFunctionDecl(ast_.get(),
                                                    return_type_a,
                                                    ident_tkn_a);

                return ast_;
            }

            StmntPtr GLSLParser::ParseUniformDeclStmnt()
            {
                Accept(Tokens::UniformBuffer);

                if (Is(Tokens::Ident))
                {
                    auto ident_tkn_ = AcceptIt();

                    if (IsRegisteredTypeName(ident_tkn_->Spell()))
                    {
                        return ParseVarDeclStmnt(true, ident_tkn_);
                    } else
                    {
                        return ParseUniformBufferDeclStmnt(ident_tkn_);
                    }
                } 
                else
                {
                    return ParseVarDeclStmnt(true);
                }
            }

            BasicDeclStmntPtr GLSLParser::ParseUniformBufferDeclStmnt(
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<BasicDeclStmnt>();

                auto uniform_buffer_decl_ = ParseUniformBufferDecl(ident_tkn_a);
                ast_->decl_object_ = uniform_buffer_decl_;

                uniform_buffer_decl_->decl_stmnt_ref_ = ast_.get();

                return ast_;
            }

            VarDeclStmntPtr GLSLParser::ParseVarDeclStmnt(
                    bool is_uniform_a,
                    const TokenPtr& ident_tkn_a)
            {
                (void)is_uniform_a;
                (void)ident_tkn_a;
                auto ast_ = Make<VarDeclStmnt>();

                ast_->type_specifier_ = ParseTypeSpecifier();
                ast_->var_decls_ = ParseVarDeclList(ast_.get());

                Semi();

                return UpdateSourceArea(ast_);
            }

            StmntPtr GLSLParser::ParseStmnt()
            {
                switch (TknType())
                {
                case Tokens::Semicolon:
                    return ParseNullStmnt();
                case Tokens::LCurly:
                    return ParseCodeBlockStmnt();
                case Tokens::Return:
                    return ParseReturnStmnt();
                case Tokens::For:
                    return ParseForLoopStmnt();
                case Tokens::While:
                    return ParseWhileLoopStmnt();
                case Tokens::Do:
                    return ParseDoWhileLoopStmnt();
                case Tokens::If:
                    return ParseIfStmnt();
                case Tokens::Switch:
                    return ParseSwitchStmnt();
                case Tokens::CtrlTransfer:
                    return ParseCtrlTransferStmnt();
                case Tokens::Struct:
                    return ParseStmntWithStructDecl();
                case Tokens::StorageClass:
                case Tokens::InterpModifier:
                case Tokens::TypeModifier:
                    return ParseVarDeclStmnt();
                default:
                    break;
                }

                if (IsDataType())
                    return ParseVarDeclStmnt();

                return ParseExprStmnt();
            }

            StmntPtr GLSLParser::ParseStmntWithStructDecl()
            {
                auto ast_ = Make<BasicDeclStmnt>();
                auto struct_decl_ = ParseStructDecl();
                struct_decl_->decl_stmnt_ref_ = ast_.get();

                ast_->decl_object_ = struct_decl_;

                if (!Is(Tokens::Semicolon))
                {
                    auto var_decl_stmnt_ = Make<VarDeclStmnt>();

                    var_decl_stmnt_->type_specifier_ = ASTFactory::MakeTypeSpecifier(
                            struct_decl_);
                    var_decl_stmnt_->var_decls_ = ParseVarDeclList(
                            var_decl_stmnt_.get());
                    Semi();

                    return UpdateSourceArea(var_decl_stmnt_);
                } 
                else
                    Semi();

                return ast_;
            }

            ExprPtr GLSLParser::ParsePrimaryExpr()
            {
                return ParseExprWithSuffixOpt(ParsePrimaryExprPrefix());
            }

            ExprPtr GLSLParser::ParsePrimaryExprPrefix()
            {
                if (auto pre_parsed_ast_ = PopPreParsedAST())
                {
                    if (pre_parsed_ast_->Type() == AST::Types::ObjectExpr)
                    {
                        auto object_expr_ = std::static_pointer_cast<ObjectExpr>(
                                pre_parsed_ast_);
                        if (Is(Tokens::LBracket))
                            return ParseCallExpr(object_expr_);
                        else
                            return object_expr_;
                    } 
                    else if (pre_parsed_ast_->Type() == AST::Types::CallExpr)
                    {
                        return std::static_pointer_cast<CallExpr>(pre_parsed_ast_);
                    } 
                    else
                        ErrorInternal(R_UnexpectedPreParsedAST, __FUNCTION__);
                }

                if (IsLiteral())
                    return ParseLiteralExpr();
                if (IsModifier())
                    return ParseTypeSpecifierExpr();
                if (IsDataType() || Is(Tokens::Struct))
                    return ParseTypeSpecifierOrCallExpr();
                if (Is(Tokens::UnaryOp) || IsArithmeticUnaryExpr())
                    return ParseUnaryExpr();
                if (Is(Tokens::LBracket))
                    return ParseBracketExpr();
                if (Is(Tokens::LCurly))
                    return ParseInitializerExpr();
                if (Is(Tokens::Ident))
                    return ParseObjectOrCallExpr();

                ErrorUnexpected(R_ExpectedPrimaryExpr, nullptr, true);

                return nullptr;
            }

            ExprPtr GLSLParser::ParseExprWithSuffixOpt(ExprPtr expr_a)
            {
                while (true)
                {
                    if (Is(Tokens::LParen))
                        expr_a = ParseArrayExpr(expr_a);
                    else if (Is(Tokens::Dot))
                        expr_a = ParseObjectOrCallExpr(expr_a);
                    else if (Is(Tokens::AssignOp))
                        expr_a = ParseAssignExpr(expr_a);
                    else if (Is(Tokens::UnaryOp))
                        expr_a = ParsePostUnaryExpr(expr_a);
                    else
                        break;
                }

                return UpdateSourceArea(expr_a);
            }

            LiteralExprPtr GLSLParser::ParseLiteralExpr()
            {
                if (!IsLiteral())
                    ErrorUnexpected(R_ExpectedLiteralExpr);

                auto ast_ = Make<LiteralExpr>();

                ast_->data_type_ = TokenToDataType(*Tkn());
                ast_->value_ = AcceptIt()->Spell();

                return UpdateSourceArea(ast_);
            }

            ExprPtr GLSLParser::ParseTypeSpecifierOrCallExpr()
            {
                if (!IsDataType() && !Is(Tokens::Struct))
                    ErrorUnexpected(R_ExpectedTypeNameOrFuncCall);

                StructDeclPtr struct_decl_;
                auto type_denoter_ = ParseTypeDenoter(true, &struct_decl_);

                if (Is(Tokens::LBracket) && !struct_decl_)
                {
                    return ParseCallExpr(nullptr, type_denoter_);
                }

                auto ast_ = Make<TypeSpecifierExpr>();
                {
                    ast_->type_specifier_ = ASTFactory::MakeTypeSpecifier(
                            type_denoter_);
                    ast_->type_specifier_->struct_decl_ = struct_decl_;
                }
                UpdateSourceArea(ast_->type_specifier_, struct_decl_.get());

                return UpdateSourceArea(ast_, struct_decl_.get());
            }

            TypeSpecifierExprPtr GLSLParser::ParseTypeSpecifierExpr()
            {
                auto ast_ = Make<TypeSpecifierExpr>();
                ast_->type_specifier_ = ParseTypeSpecifier();

                return UpdateSourceArea(ast_);
            }

            UnaryExprPtr GLSLParser::ParseUnaryExpr()
            {
                if (!Is(Tokens::UnaryOp) && !IsArithmeticUnaryExpr())
                    ErrorUnexpected(R_ExpectedUnaryOp);

                auto ast_ = Make<UnaryExpr>();
                ast_->op_ = StringToUnaryOp(AcceptIt()->Spell());
                ast_->expr_ = ParsePrimaryExpr();

                return UpdateSourceArea(ast_);
            }

            PostUnaryExprPtr GLSLParser::ParsePostUnaryExpr(const ExprPtr& expr_a)
            {
                if (!Is(Tokens::UnaryOp))
                    ErrorUnexpected(R_ExpectedUnaryOp);

                auto ast_ = Make<PostUnaryExpr>();
                ast_->expr_ = expr_a;
                ast_->op_ = StringToUnaryOp(AcceptIt()->Spell());

                UpdateSourceArea(ast_, expr_a.get());
                UpdateSourceAreaOffset(ast_);

                return ast_;
            }

            BracketExprPtr GLSLParser::ParseBracketExpr()
            {
                auto ast_ = Make<BracketExpr>();

                Accept(Tokens::LBracket);
                {
                    ast_->expr_ = ParseExpr();
                }
                Accept(Tokens::RBracket);

                return UpdateSourceArea(ast_);
            }

            ObjectExprPtr GLSLParser::ParseObjectExpr(const ExprPtr& expr_a)
            {
                if (expr_a != nullptr)
                {
                    if (Is(Tokens::Dot))
                        AcceptIt();
                    else
                        ErrorUnexpected(R_ExpectedIdentPrefix);
                }

                auto ast_ = Make<ObjectExpr>();

                if (expr_a)
                    ast_->area_ = expr_a->area_;

                ast_->prefix_expr_ = expr_a;
                ast_->ident_ = ParseIdent();

                return UpdateSourceArea(ast_);
            }

            AssignExprPtr GLSLParser::ParseAssignExpr(const ExprPtr& expr_a)
            {
                auto ast_ = Make<AssignExpr>();
                ast_->area_ = expr_a->area_;
                ast_->lvalue_expr_ = expr_a;

                if (Is(Tokens::AssignOp))
                {
                    ast_->op_ = StringToAssignOp(AcceptIt()->Spell());
                    UpdateSourceAreaOffset(ast_);
                    ast_->rvalue_expr_ = ParseExpr();
                } 
                else
                    ErrorUnexpected(Tokens::AssignOp);

                return UpdateSourceArea(ast_);
            }

            ExprPtr GLSLParser::ParseObjectOrCallExpr(const ExprPtr& expr_a)
            {
                auto object_expr_ = ParseObjectExpr(expr_a);

                if (Is(Tokens::LBracket))
                    return ParseCallExpr(object_expr_);

                return object_expr_;
            }

            CallExprPtr GLSLParser::ParseCallExpr(
                    const ObjectExprPtr& object_expr_a,
                    const TypeDenoterPtr& type_denoter_a)
            {
                if (object_expr_a)
                {
                    auto ident_tkn_ =
                            std::make_shared<Token>(object_expr_a->area_.Pos(),
                                                    Tokens::Ident,
                                                    object_expr_a->ident_);
                    
                    return ParseCallExprWithPrefixOpt(object_expr_a->prefix_expr_,
                                                      object_expr_a->is_static_,
                                                      ident_tkn_);
                } 
                else if (type_denoter_a)
                {
                    return ParseCallExprAsTypeCtor(type_denoter_a);
                } 
                else
                {
                    return ParseCallExprWithPrefixOpt();
                }
            }

            CallExprPtr GLSLParser::ParseCallExprWithPrefixOpt(
                    const ExprPtr& prefix_expr_a,
                    bool is_static_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<CallExpr>();
                ast_->prefix_expr_ = prefix_expr_a;
                ast_->is_static_ = is_static_a;

                if (ident_tkn_a)
                {
                    ast_->ident_ = ident_tkn_a->Spell();
                    ast_->area_ = ident_tkn_a->Area();
                } 
                else
                {
                    ast_->ident_ = ParseIdent();
                    UpdateSourceArea(ast_);
                }

                ast_->arguments_ = ParseArgumentList();
                return UpdateSourceArea(ast_);
            }

            CallExprPtr GLSLParser::ParseCallExprAsTypeCtor(
                    const TypeDenoterPtr& type_denoter_a)
            {
                auto ast_ = Make<CallExpr>();
                ast_->type_denoter_ = type_denoter_a;
                ast_->arguments_ = ParseArgumentList();

                return UpdateSourceArea(ast_);
            }

            std::vector<StmntPtr> GLSLParser::ParseGlobalStmntList()
            {
                std::vector<StmntPtr> stmnts_;

                Accept(Tokens::LCurly);
                while (!Is(Tokens::RCurly))
                {
                    ParseStmntWithCommentOpt(
                            stmnts_,
                            std::bind(&GLSLParser::ParseGlobalStmnt, this));
                }

                AcceptIt();

                return stmnts_;
            }

            std::vector<AttributePtr> GLSLParser::ParseAttributeList()
            {
                std::vector<AttributePtr> attribs_;
                Accept(Tokens::LayoutQualifier);
                Accept(Tokens::LBracket);

                while (true)
                {
                    attribs_.push_back(ParseAttribute());

                    if (Is(Tokens::Comma))
                        AcceptIt();
                    else
                        break;
                }

                Accept(Tokens::RBracket);

                return attribs_;
            }

            TypeDenoterPtr GLSLParser::ParseTypeDenoter(
                    bool allow_void_type_a,
                    StructDeclPtr* struct_decl_a)
            {
                if (Is(Tokens::Void))
                {
                    if (allow_void_type_a)
                        return ParseVoidTypeDenoter();

                    Error(R_NotAllowedInThisContext(R_VoidTypeDen));
                    return nullptr;
                } 
                else
                {
                    auto type_denoter_ = ParseTypeDenoterPrimary(struct_decl_a);
                    if (Is(Tokens::LParen))
                    {
                        type_denoter_ = std::make_shared<ArrayTypeDenoter>(
                                type_denoter_,
                                ParseArrayDimensionList());
                    }

                    return type_denoter_;
                }
            }

            TypeDenoterPtr GLSLParser::ParseTypeDenoterPrimary(
                    StructDeclPtr* struct_decl_a)
            {
                if (IsBaseDataType())
                    return ParseBaseTypeDenoter();
                else if (Is(Tokens::Ident) || Is(Tokens::Struct))
                {
                    if (struct_decl_a)
                        return ParseStructTypeDenoterWithStructDeclOpt(
                                *struct_decl_a);
                    else
                        return ParseStructTypeDenoter();
                } 
                else if (Is(Tokens::StorageBuffer))
                    return ParseBufferTypeDenoter();
                else if (Is(Tokens::Sampler) || Is(Tokens::SamplerState))
                    return ParseSamplerTypeDenoter();

                ErrorUnexpected(R_ExpectedTypeDen,
                                GetScanner().ActiveToken().get(),
                                true);
                return nullptr;
            }

            TypeDenoterPtr GLSLParser::ParseTypeDenoterWithStructDeclOpt(
                    StructDeclPtr& struct_decl_a,
                    bool allow_void_type_a)
            {
                if (Is(Tokens::Struct))
                    return ParseStructTypeDenoterWithStructDeclOpt(struct_decl_a);
                else
                    return ParseTypeDenoter(allow_void_type_a);
            }

            VoidTypeDenoterPtr GLSLParser::ParseVoidTypeDenoter()
            {
                Accept(Tokens::Void);
                return std::make_shared<VoidTypeDenoter>();
            }

            BaseTypeDenoterPtr GLSLParser::ParseBaseTypeDenoter()
            {
                if (IsBaseDataType())
                {
                    auto keyword_ = AcceptIt()->Spell();
                    auto type_denoter_ = std::make_shared<BaseTypeDenoter>();
                    type_denoter_->data_type_ = ParseDataType(keyword_);
                    return type_denoter_;
                }
                ErrorUnexpected(R_ExpectedBaseTypeDen, nullptr, true);
                return nullptr;
            }

            BufferTypeDenoterPtr GLSLParser::ParseBufferTypeDenoter()
            {
                Accept(Tokens::StorageBuffer);
                return std::make_shared<BufferTypeDenoter>(
                        BufferType::GenericBuffer);
            }

            SamplerTypeDenoterPtr GLSLParser::ParseSamplerTypeDenoter()
            {
                auto sampler_type_ = ParseSamplerType();
                return std::make_shared<SamplerTypeDenoter>(sampler_type_);
            }

            StructTypeDenoterPtr GLSLParser::ParseStructTypeDenoter()
            {
                if (Is(Tokens::Struct))
                    AcceptIt();

                auto ident_ = ParseIdent();
                auto type_denoter_ = std::make_shared<StructTypeDenoter>(ident_);

                return type_denoter_;
            }

            StructTypeDenoterPtr
            GLSLParser::ParseStructTypeDenoterWithStructDeclOpt(
                    StructDeclPtr& struct_decl_a)
            {
                Accept(Tokens::Struct);

                if (Is(Tokens::LCurly))
                {
                    struct_decl_a = ParseStructDecl(false);
                    return std::make_shared<StructTypeDenoter>(
                            struct_decl_a.get());
                } 
                else
                {
                    auto struct_ident_tkn_ = Accept(Tokens::Ident);
                    if (Is(Tokens::LCurly) || Is(Tokens::Colon))
                    {
                        struct_decl_a = ParseStructDecl(false, struct_ident_tkn_);
                        return std::make_shared<StructTypeDenoter>(
                                struct_decl_a.get());
                    } 
                    else
                    {
                        return std::make_shared<StructTypeDenoter>(
                                struct_ident_tkn_->Spell());
                    }
                }
            }

            DataType GLSLParser::ParseDataType(const std::string& keyword_a)
            {
                try
                {
                    return GLSLKeywordToDataType(keyword_a);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return DataType::Undefined;
            }

            PrimitiveType GLSLParser::ParsePrimitiveType()
            {
                try
                {
                    return GLSLKeywordToPrimitiveType(
                            Accept(Tokens::PrimitiveType)->Spell());
                }
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return PrimitiveType::Undefined;
            }

            InterpModifier GLSLParser::ParseInterpModifier()
            {
                try
                {
                    return GLSLKeywordToInterpModifier(
                            Accept(Tokens::InterpModifier)->Spell());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return InterpModifier::Undefined;
            }

            TypeModifier GLSLParser::ParseTypeModifier()
            {
                Accept(Tokens::TypeModifier, "const");
                return TypeModifier::Const;
            }

            StorageClass GLSLParser::ParseStorageClass()
            {
                try
                {
                    return GLSLKeywordToStorageClass(
                            Accept(Tokens::StorageClass)->Spell());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return StorageClass::Undefined;
            }

            SamplerType GLSLParser::ParseSamplerType()
            {
                try
                {
                    if (Is(Tokens::Sampler) || Is(Tokens::SamplerState))
                        return GLSLKeywordToSamplerType(AcceptIt()->Spell());
                    else
                        ErrorUnexpected(R_ExpectedSamplerOrSamplerState);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return SamplerType::Undefined;
            }

            bool GLSLParser::ParseModifiers(TypeSpecifier* type_specifier_a,
                                            bool allow_primitive_type_a,
                                            const TokenPtr& input_tkn_a)
            {
                if (Is(Tokens::InputModifier) || input_tkn_a)
                {
                    std::string modifier_;

                    if (input_tkn_a)
                        modifier_ = input_tkn_a->Spell();
                    else
                        modifier_ = AcceptIt()->Spell();

                    if (modifier_ == "in")
                        type_specifier_a->is_input_ = true;
                    else if (modifier_ == "out")
                        type_specifier_a->is_output_ = true;
                    else if (modifier_ == "inout")
                    {
                        type_specifier_a->is_input_ = true;
                        type_specifier_a->is_output_ = true;
                    }
                } 
                else if (Is(Tokens::InterpModifier))
                {
                    type_specifier_a->interp_modifiers_.insert(
                            ParseInterpModifier());
                } 
                else if (Is(Tokens::TypeModifier))
                {
                    type_specifier_a->SetTypeModifier(ParseTypeModifier());
                } 
                else if (Is(Tokens::StorageClass))
                {
                    type_specifier_a->storage_ñlasses_.insert(ParseStorageClass());
                } 
                else if (Is(Tokens::PrimitiveType))
                {
                    if (!allow_primitive_type_a)
                        Error(R_NotAllowedInThisContext(R_PrimitiveType),
                              false,
                              false);

                    auto primitive_type_ = ParsePrimitiveType();

                    if (type_specifier_a->primitive_type_
                        == PrimitiveType::Undefined)
                        type_specifier_a->primitive_type_ = primitive_type_;
                    else if (type_specifier_a->primitive_type_ == primitive_type_)
                        Error(R_DuplicatedPrimitiveType, true, false);
                    else if (type_specifier_a->primitive_type_ != primitive_type_)
                        Error(R_ConflictingPrimitiveTypes, true, false);
                } 
                else
                    return false;

                return true;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
