#include "HLSLParser.hpp"

#include "../../AST/AST.hpp"
#include "../../AST/ASTFactory.hpp"
#include "../../Exception.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "HLSLKeywords.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            HLSLParser::HLSLParser(Log* log_a) : SLParser {log_a}
            {}

            ProgramPtr HLSLParser::ParseSource(
                    const SourceCodePtr& source_a,
                    const NameMangling& name_mangling_a,
                    const InputShaderVersion version_in_a,
                    bool row_major_alignment_a,
                    bool enable_warnings_a)
            {
                use_d3d10_semantics_ = (version_in_a >= InputShaderVersion::HLSL4);
                enable_cg_keywords_ = (version_in_a == InputShaderVersion::Cg);
                row_major_alignment_ = row_major_alignment_a;

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

            ScannerPtr HLSLParser::MakeScanner()
            {
                return std::make_shared<HLSLScanner>(enable_cg_keywords_,
                                                     GetLog());
            }

            bool HLSLParser::IsDataType() const
            {
                return (IsBaseDataType() || Is(Tokens::Vector)
                        || Is(Tokens::Matrix) || Is(Tokens::Buffer)
                        || Is(Tokens::Sampler) || Is(Tokens::SamplerState));
            }

            bool HLSLParser::IsBaseDataType() const
            {
                return (Is(Tokens::ScalarType) || Is(Tokens::VectorType)
                        || Is(Tokens::MatrixType) || Is(Tokens::StringType));
            }

            bool HLSLParser::IsLiteral() const
            {
                return (Is(Tokens::NullLiteral) || Is(Tokens::BoolLiteral)
                        || Is(Tokens::IntLiteral) || Is(Tokens::FloatLiteral)
                        || Is(Tokens::StringLiteral));
            }

            bool HLSLParser::IsArithmeticUnaryExpr() const
            {
                return (Is(Tokens::BinaryOp, "-") || Is(Tokens::BinaryOp, "+"));
            }

            bool HLSLParser::IsModifier() const
            {
                return (Is(Tokens::InputModifier) || Is(Tokens::InterpModifier)
                        || Is(Tokens::TypeModifier)
                        || Is(Tokens::StorageClass));
            }

            TypeSpecifierPtr HLSLParser::MakeTypeSpecifierIfLhsOfCastExpr(
                    const ExprPtr& expr_)
            {
                if (auto type_specifier_expr_ = expr_->As<TypeSpecifierExpr>())
                    return type_specifier_expr_->type_specifier_;

                if (auto object_expr_ = expr_->As<ObjectExpr>())
                {
                    if (IsRegisteredTypeName(object_expr_->ident_))
                    {
                        return ASTFactory::MakeTypeSpecifier(
                                std::make_shared<AliasTypeDenoter>(
                                        object_expr_->ident_));
                    }
                }

                return nullptr;
            }

            TokenPtr HLSLParser::AcceptIt()
            {
                auto tkn1_ = Parser::AcceptIt();
                while (Tkn()->Type() == Tokens::Directive)
                    ProcessDirective(AcceptIt()->Spell());

                return tkn1_;
            }

            void HLSLParser::ProcessDirective(const std::string& ident_a)
            {
                try
                {
                    if (ident_a == "line")
                        ProcessDirectiveLine();
                    else if (ident_a == "pragma")
                        ProcessDirectivePragma();
                    else
                        RuntimeErr(R_InvalidHLSLDirectiveAfterPP);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
            }

            void HLSLParser::ProcessDirectiveLine()
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

            void HLSLParser::ProcessDirectivePragma()
            {
                if (Is(Tokens::Ident) && Tkn()->Spell() == "pack_matrix")
                {
                    Parser::AcceptIt();

                    auto AcceptToken = [&](const Tokens type_) {
                        if (!Is(type_))
                            RuntimeErr(R_UnexpectedTokenInPackMatrixPragma);

                        return this->Parser::AcceptIt();
                    };

                    AcceptToken(Tokens::LBracket);
                    auto alignment_tkn_ = AcceptToken(Tokens::TypeModifier);
                    AcceptToken(Tokens::RBracket);

                    auto alignment_ = alignment_tkn_->Spell();
                    if (alignment_ == "row_major")
                        row_major_alignment_ = true;
                    else if (alignment_ == "column_major")
                        row_major_alignment_ = false;
                    else
                        Error(R_UnknownMatrixPackAlignment(alignment_),
                              alignment_tkn_.get());
                } 
                else
                    Error(R_InvalidHLSLPragmaAfterPP);
            }

            void HLSLParser::OpenScope()
            {
                type_name_symbol_table_.OpenScope();
            }

            void HLSLParser::CloseScope()
            {
                type_name_symbol_table_.CloseScope();
            }

            void HLSLParser::RegisterTypeName(const std::string& ident_a)
            {
                type_name_symbol_table_.Register(ident_a, true, nullptr, false);
            }

            bool HLSLParser::IsRegisteredTypeName(
                    const std::string& ident_a) const
            {
                return type_name_symbol_table_.Fetch(ident_a);
            }

            AliasDeclStmntPtr HLSLParser::MakeAndRegisterBuildinAlias(
                    const DataType data_type_a,
                    const std::string& ident_a)
            {
                auto ast_ = ASTFactory::MakeBaseTypeAlias(data_type_a, ident_a);
                RegisterTypeName(ident_a);
                ast_->flags_ << AST::IsBuiltin;
                return ast_;
            }

            void HLSLParser::GeneratePreDefinedTypeAliases(Program& ast_a)
            {
                static const std::vector<std::pair<DataType, std::string>>
                        pre_defined_types_ {
                                {DataType::Int, "DWORD"},
                                {DataType::Float, "FLOAT"},
                                {DataType::Float4, "VECTOR"},
                                {DataType::Float4x4, "MATRIX"},
                                {DataType::String, "STRING"},
                        };

                for (const auto& type_ : pre_defined_types_)
                {
                    ast_a.global_stmnts_.push_back(
                            MakeAndRegisterBuildinAlias(type_.first,
                                                        type_.second));
                }
            }

            TypeSpecifierPtr HLSLParser::MakeTypeSpecifierWithPackAlignment()
            {
                auto ast_ = Make<TypeSpecifier>();

                if (row_major_alignment_)
                    ast_->SetTypeModifier(TypeModifier::RowMajor);

                return ast_;
            }

            ProgramPtr HLSLParser::ParseProgram(const SourceCodePtr& source_a)
            {
                auto ast_ = Make<Program>();

                OpenScope();

                GeneratePreDefinedTypeAliases(*ast_);
                ast_->source_code_ = source_a;

                while (true)
                {
                    ParseAndIgnoreTechniquesAndNullStmnts();

                    if (Is(Tokens::EndOfStream))
                        break;

                    ParseStmntWithCommentOpt(
                            ast_->global_stmnts_,
                            std::bind(&HLSLParser::ParseGlobalStmnt, this));
                }

                CloseScope();

                return ast_;
            }

            CodeBlockPtr HLSLParser::ParseCodeBlock()
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

            VarDeclStmntPtr HLSLParser::ParseParameter()
            {
                auto ast_ = Make<VarDeclStmnt>();
                ast_->attribs_ = ParseAttributeList();
                ast_->type_specifier_ = ParseTypeSpecifier();

                ast_->var_decls_.push_back(ParseVarDecl(ast_.get()));
                ast_->flags_ << VarDeclStmnt::IsParameter;

                return UpdateSourceArea(ast_);
            }

            StmntPtr HLSLParser::ParseLocalStmnt()
            {
                return ParseStmnt(true);
            }

            StmntPtr HLSLParser::ParseForLoopInitializer()
            {
                return ParseStmnt(false);
            }

            SwitchCasePtr HLSLParser::ParseSwitchCase()
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
                                             std::bind(&HLSLParser::ParseStmnt,
                                                       this,
                                                       true));

                return ast_;
            }

            VarDeclPtr HLSLParser::ParseVarDecl(VarDeclStmnt* decl_stmnt_ref_a,
                                                const TokenPtr& ident_tkn_a)
            {
                auto ast_a = Make<VarDecl>();
                ast_a->decl_stmnt_ref_ = decl_stmnt_ref_a;
                ast_a->ident_ = ParseIdentWithNamespaceOpt(ast_a->namespace_expr_,
                                                        ident_tkn_a,
                                                        &ast_a->area_);
                
                ast_a->array_dims_ = ParseArrayDimensionList(true);

                ParseVarDeclSemantic(*ast_a);

                ast_a->annotations_ = ParseAnnotationList();
                if (Is(Tokens::AssignOp, "="))
                    ast_a->initializer_ = ParseInitializer();

                return ast_a;
            }

            SamplerValuePtr HLSLParser::ParseSamplerValue()
            {
                auto ast_ = Make<SamplerValue>();
                ast_->name_ = ParseIdent();

                Accept(Tokens::AssignOp, "=");
                ast_->value_ = ParseExpr();
                Semi();

                return ast_;
            }

            AttributePtr HLSLParser::ParseAttribute()
            {
                Accept(Tokens::LParen);

                auto ast_ = Make<Attribute>();
                auto attrib_ident_ = ParseIdent();
                ast_->attribute_type_ = HLSLKeywordToAttributeType(attrib_ident_);

                UpdateSourceArea(ast_);

                if (ast_->attribute_type_ == AttributeType::Undefined)
                    Warning(R_UnknownAttribute(attrib_ident_));

                if (Is(Tokens::LBracket))
                {
                    AcceptIt();

                    if (!Is(Tokens::RBracket))
                    {
                        while (true)
                        {
                            ast_->arguments_.push_back(ParseExpr());
                            if (Is(Tokens::Comma))
                                AcceptIt();
                            else
                                break;
                        }
                    }

                    Accept(Tokens::RBracket);
                }

                Accept(Tokens::RParen);

                return ast_;
            }

            static ShaderTarget HLSLShaderProfileToTarget(const std::string& s_a)
            {
                if (s_a.size() >= 2)
                {
                    auto p = s_a.substr(0, 2);
                    if (p == "vs")
                        return ShaderTarget::VertexShader;
                    if (p == "hs")
                        return ShaderTarget::TessellationControlShader;
                    if (p == "ds")
                        return ShaderTarget::TessellationEvaluationShader;
                    if (p == "gs")
                        return ShaderTarget::GeometryShader;
                    if (p == "ps")
                        return ShaderTarget::FragmentShader;
                    if (p == "cs")
                        return ShaderTarget::ComputeShader;
                }
                return ShaderTarget::Undefined;
            }

            RegisterPtr HLSLParser::ParseRegister(bool parse_colon_a)
            {
                if (parse_colon_a)
                    Accept(Tokens::Colon);

                auto ast_ = Make<Register>();

                Accept(Tokens::Register);
                Accept(Tokens::LBracket);

                auto type_ident_ = ParseIdent();

                if (Is(Tokens::Comma))
                {
                    ast_->shader_target_ = HLSLShaderProfileToTarget(type_ident_);

                    AcceptIt();
                    type_ident_ = ParseIdent();
                }

                ast_->area_.Offset(GetScanner().PreviousToken()->Pos());
                ast_->register_type_ = CharToRegisterType(type_ident_.front());

                if (type_ident_.size() > 1)
                    ast_->slot_ =
                            ParseIntLiteral(type_ident_.substr(1),
                                            GetScanner().PreviousToken().get());

                if (ast_->register_type_ == RegisterType::Undefined)
                    Warning(R_UnknownSlotRegister(type_ident_.substr(0, 1)));

                if (Is(Tokens::LParen))
                {
                    AcceptIt();
                    ast_->slot_ += ParseIntLiteral();
                    Accept(Tokens::RParen);
                }

                Accept(Tokens::RBracket);

                return UpdateSourceArea(ast_);
            }

            PackOffsetPtr HLSLParser::ParsePackOffset(bool parse_colon_a)
            {
                if (parse_colon_a)
                    Accept(Tokens::Colon);

                auto ast_ = Make<PackOffset>();

                Accept(Tokens::PackOffset);
                Accept(Tokens::LBracket);

                ast_->register_name_ = ParseIdent();

                if (Is(Tokens::Dot))
                {
                    AcceptIt();
                    ast_->vector_component_ = ParseIdent();
                }

                Accept(Tokens::RBracket);

                return UpdateSourceArea(ast_);
            }

            TypeSpecifierPtr HLSLParser::ParseTypeSpecifier(bool parse_void_type_a)
            {
                (void)parse_void_type_a;
                auto ast_ = MakeTypeSpecifierWithPackAlignment();

                while (IsModifier() || Is(Tokens::PrimitiveType))
                    ParseModifiers(ast_.get(), true);

                ast_->type_denoter_ = ParseTypeDenoterWithStructDeclOpt(
                        ast_->struct_decl_);

                return UpdateSourceArea(ast_);
            }

            BufferDeclPtr HLSLParser::ParseBufferDecl(
                    BufferDeclStmnt* decl_stmnt_ref_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<BufferDecl>();
                ast_->decl_stmnt_ref_ = decl_stmnt_ref_a;

                ast_->ident_ = ParseIdent(ident_tkn_a);
                ast_->array_dims_ = ParseArrayDimensionList();
                ast_->slot_registers_ = ParseRegisterList();
                ast_->annotations_ = ParseAnnotationList();

                return ast_;
            }

            SamplerDeclPtr HLSLParser::ParseSamplerDecl(
                    SamplerDeclStmnt* decl_stmnt_ref_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<SamplerDecl>();
                ast_->decl_stmnt_ref_ = decl_stmnt_ref_a;

                ast_->ident_ = ParseIdent(ident_tkn_a);
                ast_->array_dims_ = ParseArrayDimensionList();
                ast_->slot_registers_ = ParseRegisterList();

                if (Is(Tokens::AssignOp, "="))
                {
                    AcceptIt();
                    Accept(Tokens::SamplerState, "sampler_state");
                    Accept(Tokens::LCurly);

                    ast_->textureIdent = ParseSamplerStateTextureIdent();
                    ast_->sampler_values_ = ParseSamplerValueList();

                    Accept(Tokens::RCurly);
                } 
                else if (Is(Tokens::LCurly))
                {
                    AcceptIt();
                    ast_->sampler_values_ = ParseSamplerValueList();
                    Accept(Tokens::RCurly);
                }

                return ast_;
            }

            StructDeclPtr HLSLParser::ParseStructDecl(bool parse_struct_tkn_a,
                                                      const TokenPtr& ident_tkn_a)
            {
                auto tkn1_ = Tkn();
                auto ast_ = Make<StructDecl>();

                if (parse_struct_tkn_a)
                {
                    if (Is(Tokens::Class))
                    {
                        AcceptIt();
                        ast_->is_class_ = true;
                    } 
                    else
                        Accept(Tokens::Struct);
                    UpdateSourceArea(ast_);
                }

                if (Is(Tokens::Ident) || ident_tkn_a)
                {
                    tkn1_ = Tkn();
                    ast_->ident_ = (ident_tkn_a ? ident_tkn_a->Spell() : ParseIdent());
                    UpdateSourceArea(ast_);

                    RegisterTypeName(ast_->ident_);

                    if (Is(Tokens::Colon))
                    {
                        AcceptIt();

                        ast_->base_struct_name_ = ParseIdent();
                        if (ast_->base_struct_name_ == ast_->ident_)
                            Error(R_IllegalRecursiveInheritance);

                        if (Is(Tokens::Comma))
                            Error(R_IllegalMultipleInheritance, false);
                    }
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
                        else if (auto basic_decl_stmnt_ =
                                           stmnt_->As<BasicDeclStmnt>())
                        {
                            if (basic_decl_stmnt_->decl_object_->Type()
                                == AST::Types::FunctionDecl)
                            {
                                ast_->func_members_.push_back(
                                        std::static_pointer_cast<FunctionDecl>(
                                                basic_decl_stmnt_->decl_object_));
                            } 
                            else
                                Error(R_IllegalDeclStmntInsideDeclOf(
                                              ast_->ToString()),
                                      stmnt_->area_,
                                      false);
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

                    for (auto& func_decl_ : ast_->func_members_)
                        func_decl_->struct_decl_ref_ = ast_.get();
                }
                GetReportHandler().PopContextDesc();

                return ast_;
            }

            AliasDeclPtr HLSLParser::ParseAliasDecl(TypeDenoterPtr type_denoter_a)
            {
                auto ast_ = Make<AliasDecl>();
                ast_->ident_ = ParseIdent();

                RegisterTypeName(ast_->ident_);

                if (Is(Tokens::LParen))
                {
                    type_denoter_a = std::make_shared<ArrayTypeDenoter>(
                            type_denoter_a,
                            ParseArrayDimensionList());
                }

                ast_->type_denoter_ = type_denoter_a;

                return UpdateSourceArea(ast_);
            }

            FunctionDeclPtr HLSLParser::ParseFunctionDecl(
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

                ParseFunctionDeclSemantic(*ast_);
                ast_->annotations_ = ParseAnnotationList();

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

            UniformBufferDeclPtr HLSLParser::ParseUniformBufferDecl()
            {
                auto ast_ = Make<UniformBufferDecl>();
                ast_->buffer_type_ = ParseUniformBufferType();
                ast_->ident_ = ParseIdent();

                UpdateSourceArea(ast_);

                ast_->slot_registers_ = ParseRegisterList();

                GetReportHandler().PushContextDesc(ast_->ToString());
                {
                    ast_->local_stmnts_ = ParseGlobalStmntList();

                    for (auto& stmnt_ : ast_->local_stmnts_)
                    {
                        if (stmnt_->Type() == AST::Types::VarDeclStmnt)
                            ast_->var_members_.push_back(
                                    std::static_pointer_cast<VarDeclStmnt>(
                                            stmnt_));
                    }

                    for (auto& var_decl_stmnt_ : ast_->var_members_)
                    {
                        for (auto& var_decl_ : var_decl_stmnt_->var_decls_)
                            var_decl_->buffer_decl_ref_ = ast_.get();
                    }

                    if (Is(Tokens::Semicolon))
                        Semi();
                }
                GetReportHandler().PopContextDesc();

                return ast_;
            }

            StmntPtr HLSLParser::ParseGlobalStmnt()
            {
                if (Is(Tokens::LParen))
                {
                    auto attribs_ = ParseAttributeList();
                    auto ast_ = ParseGlobalStmntPrimary();
                    ast_->attribs_ = std::move(attribs_);
                    return ast_;
                } 
                else
                {
                    return ParseGlobalStmntPrimary();
                }
            }

            StmntPtr HLSLParser::ParseGlobalStmntPrimary()
            {
                switch (TknType())
                {
                case Tokens::Sampler:
                case Tokens::SamplerState:
                    return ParseGlobalStmntWithSamplerTypeDenoter();
                case Tokens::Buffer:
                    return ParseGlobalStmntWithBufferTypeDenoter();
                case Tokens::UniformBuffer:
                    return ParseUniformBufferDeclStmnt();
                case Tokens::Typedef:
                    return ParseAliasDeclStmnt();
                case Tokens::Void:
                case Tokens::Inline:
                    return ParseFunctionDeclStmnt();
                default:
                    return ParseGlobalStmntWithTypeSpecifier();
                }
            }

            StmntPtr HLSLParser::ParseGlobalStmntWithTypeSpecifier()
            {
                auto type_specifier_ = ParseTypeSpecifier();
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

            StmntPtr HLSLParser::ParseGlobalStmntWithSamplerTypeDenoter()
            {
                auto type_denoter_ = ParseSamplerTypeDenoter();
                auto ident_tkn_ = Accept(Tokens::Ident);

                if (Is(Tokens::LBracket))
                {
                    return ParseFunctionDeclStmnt(ASTFactory::MakeTypeSpecifier(
                                                          type_denoter_),
                                                  ident_tkn_);
                } 
                else
                {
                    return ParseSamplerDeclStmnt(type_denoter_, ident_tkn_);
                }
            }

            StmntPtr HLSLParser::ParseGlobalStmntWithBufferTypeDenoter()
            {
                auto type_denoter_ = ParseBufferTypeDenoter();
                auto ident_tkn_ = Accept(Tokens::Ident);

                if (Is(Tokens::LBracket))
                {
                    return ParseFunctionDeclStmnt(ASTFactory::MakeTypeSpecifier(
                                                          type_denoter_),
                                                  ident_tkn_);
                } 
                else
                {
                    return ParseBufferDeclStmnt(type_denoter_, ident_tkn_);
                }
            }

            BasicDeclStmntPtr HLSLParser::ParseFunctionDeclStmnt(
                    const TypeSpecifierPtr& return_type_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<BasicDeclStmnt>();

                if (!return_type_a)
                {
                    ast_->attribs_ = ParseAttributeList();
                }
                ast_->decl_object_ = ParseFunctionDecl(ast_.get(),
                                                    return_type_a,
                                                    ident_tkn_a);

                return ast_;
            }

            BasicDeclStmntPtr HLSLParser::ParseUniformBufferDeclStmnt()
            {
                auto ast_ = Make<BasicDeclStmnt>();
                ast_->attribs_ = ParseAttributeList();

                auto uniform_buffer_decl_ = ParseUniformBufferDecl();
                ast_->decl_object_ = uniform_buffer_decl_;

                uniform_buffer_decl_->decl_stmnt_ref_ = ast_.get();

                return ast_;
            }

            BufferDeclStmntPtr HLSLParser::ParseBufferDeclStmnt(
                    const BufferTypeDenoterPtr& type_denoter_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<BufferDeclStmnt>();

                ast_->type_denoter_ = (type_denoter_a ? type_denoter_a
                                                : ParseBufferTypeDenoter());
                ast_->buffer_decls_ = ParseBufferDeclList(ast_.get(), ident_tkn_a);

                Semi();

                if (ident_tkn_a)
                    ast_->area_ = ident_tkn_a->Area();
                else
                    UpdateSourceArea(ast_);

                return ast_;
            }

            SamplerDeclStmntPtr HLSLParser::ParseSamplerDeclStmnt(
                    const SamplerTypeDenoterPtr& type_denoter_a,
                    const TokenPtr& ident_tkn_a)
            {
                auto ast_ = Make<SamplerDeclStmnt>();

                ast_->type_denoter_ = (type_denoter_a ? type_denoter_a
                                                : ParseSamplerTypeDenoter());
                ast_->sampler_decls_ = ParseSamplerDeclList(ast_.get(), ident_tkn_a);

                Semi();

                if (ident_tkn_a)
                    ast_->area_ = ident_tkn_a->Area();
                else
                    UpdateSourceArea(ast_);

                return ast_;
            }

            VarDeclStmntPtr HLSLParser::ParseVarDeclStmnt()
            {
                auto ast_ = Make<VarDeclStmnt>();
                ast_->type_specifier_ = ParseTypeSpecifier();
                ast_->var_decls_ = ParseVarDeclList(ast_.get());

                Semi();

                return UpdateSourceArea(ast_);
            }

            AliasDeclStmntPtr HLSLParser::ParseAliasDeclStmnt()
            {
                auto ast_ = Make<AliasDeclStmnt>();
                Accept(Tokens::Typedef);

                auto type_denoter_ = ParseTypeDenoterWithStructDeclOpt(
                        ast_->struct_decl_);
                ast_->alias_decls_ = ParseAliasDeclList(type_denoter_);

                Semi();

                for (auto& decl_ : ast_->alias_decls_)
                    decl_->decl_stmnt_ref_ = ast_.get();

                return UpdateSourceArea(ast_);
            }

            StmntPtr HLSLParser::ParseStmnt(bool allow_attributes_a)
            {
                if (allow_attributes_a)
                {
                    auto attribs_ = ParseAttributeList();
                    auto ast_ = ParseStmntPrimary();
                    ast_->attribs_ = std::move(attribs_);
                    return ast_;
                } 
                else
                {
                    if (Is(Tokens::LParen))
                    {
                        Error(R_NotAllowedInThisContext(R_Attributes),
                              false,
                              false);
                 
                        ParseAttributeList();
                    }

                    return ParseStmntPrimary();
                }
            }

            StmntPtr HLSLParser::ParseStmntPrimary()
            {
                switch (TknType())
                {
                case Tokens::Semicolon:
                    return ParseNullStmnt();
                case Tokens::LCurly:
                    return ParseCodeBlockStmnt();
                case Tokens::Return:
                    return ParseReturnStmnt();
                case Tokens::Ident:
                    return ParseStmntWithIdent();
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
                case Tokens::Class:
                    return ParseStmntWithStructDecl();
                case Tokens::Typedef:
                    return ParseAliasDeclStmnt();
                case Tokens::Sampler:
                case Tokens::SamplerState:
                    return ParseSamplerDeclStmnt();
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

            StmntPtr HLSLParser::ParseStmntWithStructDecl()
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

            StmntPtr HLSLParser::ParseStmntWithIdent()
            {
                auto object_expr_ = ParseObjectExpr();

                auto expr_ = ParseExprWithSuffixOpt(object_expr_);

                if (Is(Tokens::LBracket) || Is(Tokens::UnaryOp)
                    || Is(Tokens::BinaryOp) || Is(Tokens::TernaryOp))
                {
                    PushPreParsedAST(expr_);
                    return ParseExprStmnt();
                } 
                else if (Is(Tokens::Semicolon))
                {
                    return ParseExprStmnt(expr_);
                } 
                else if (Is(Tokens::Comma))
                {
                    return ParseExprStmnt(ParseSequenceExpr(expr_));
                } 
                else if (expr_ == object_expr_)
                {
                    auto ast_ = Make<VarDeclStmnt>();

                    ast_->type_specifier_ = MakeTypeSpecifierWithPackAlignment();
                    ast_->type_specifier_->type_denoter_ =
                            ParseTypeDenoterWithArrayOpt(
                                    ParseAliasTypeDenoter(object_expr_->ident_));

                    UpdateSourceArea(ast_->type_specifier_, object_expr_.get());

                    ast_->var_decls_ = ParseVarDeclList(ast_.get());
                    Semi();

                    return UpdateSourceArea(ast_, object_expr_.get());
                } 
                else
                    return ParseExprStmnt(expr_);
            }

            ExprPtr HLSLParser::ParsePrimaryExpr()
            {
                return ParseExprWithSuffixOpt(ParsePrimaryExprPrefix());
            }

            ExprPtr HLSLParser::ParsePrimaryExprPrefix()
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
                    return ParseExprWithBracketPrefix();
                if (Is(Tokens::LCurly))
                    return ParseInitializerExpr();
                if (Is(Tokens::Ident))
                    return ParseObjectOrCallExpr();

                ErrorUnexpected(R_ExpectedPrimaryExpr, nullptr, true);

                return nullptr;
            }

            ExprPtr HLSLParser::ParseExprWithSuffixOpt(ExprPtr expr_a)
            {
                while (true)
                {
                    if (Is(Tokens::LParen))
                        expr_a = ParseArrayExpr(expr_a);
                    else if (Is(Tokens::Dot) || Is(Tokens::DColon))
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

            LiteralExprPtr HLSLParser::ParseLiteralExpr()
            {
                if (!IsLiteral())
                    ErrorUnexpected(R_ExpectedLiteralExpr);

                auto ast_ = Make<LiteralExpr>();
                if (!Is(Tokens::NullLiteral))
                    ast_->data_type_ = TokenToDataType(*Tkn());

                ast_->value_ = AcceptIt()->Spell();

                return UpdateSourceArea(ast_);
            }

            ExprPtr HLSLParser::ParseTypeSpecifierOrCallExpr()
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

            TypeSpecifierExprPtr HLSLParser::ParseTypeSpecifierExpr()
            {
                auto ast_ = Make<TypeSpecifierExpr>();
                ast_->type_specifier_ = ParseTypeSpecifier();

                return UpdateSourceArea(ast_);
            }

            UnaryExprPtr HLSLParser::ParseUnaryExpr()
            {
                if (!Is(Tokens::UnaryOp) && !IsArithmeticUnaryExpr())
                    ErrorUnexpected(R_ExpectedUnaryOp);

                auto ast_ = Make<UnaryExpr>();
                ast_->op_ = StringToUnaryOp(AcceptIt()->Spell());
                ast_->expr_ = ParsePrimaryExpr();

                return UpdateSourceArea(ast_);
            }

            PostUnaryExprPtr HLSLParser::ParsePostUnaryExpr(const ExprPtr& expr_a)
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

            ExprPtr HLSLParser::ParseExprWithBracketPrefix()
            {
                ExprPtr expr_;
                SourceArea area_(GetScanner().Pos(), 1);

                Accept(Tokens::LBracket);
                {
                    if (ActiveParsingState().active_template_)
                    {
                        auto parsing_state_ = ActiveParsingState();
                        parsing_state_.active_template_ = false;
                        PushParsingState(parsing_state_);
                        {
                            expr_ = ParseExprWithSequenceOpt();
                        }
                        PopParsingState();
                    } 
                    else
                        expr_ = ParseExprWithSequenceOpt();
                }
                Accept(Tokens::RBracket);

                if (auto type_specifier_ = MakeTypeSpecifierIfLhsOfCastExpr(expr_))
                {
                    auto ast_ = Make<CastExpr>();
                    ast_->area_ = area_;
                    ast_->type_specifier_ = type_specifier_;
                    ast_->expr_ = ParsePrimaryExpr();

                    return UpdateSourceArea(ast_);
                }

                if (Is(Tokens::LBracket))
                {
                    if (auto non_bracket_expr_ = expr_->FindFirstNotOf(
                                AST::Types::BracketExpr))
                    {
                        if (auto object_expr_ = non_bracket_expr_->As<ObjectExpr>())
                        {
                            auto ast_ = Make<CallExpr>();
                            ast_->prefix_expr_ = object_expr_->prefix_expr_;
                            ast_->is_static_ = object_expr_->is_static_;
                            ast_->ident_ = object_expr_->ident_;
                            ast_->arguments_ = ParseArgumentList();

                            return UpdateSourceArea(ast_);
                        }
                    }
                }

                auto ast_ = Make<BracketExpr>();
                ast_->area_ = area_;
                ast_->expr_ = expr_;

                return UpdateSourceArea(ast_);
            }

            ObjectExprPtr HLSLParser::ParseObjectExpr(const ExprPtr& expr_a)
            {
                bool is_static_ = false;

                if (expr_a != nullptr)
                {
                    if (Is(Tokens::DColon))
                    {
                        AcceptIt();
                        is_static_ = true;
                    } 
                    else if (Is(Tokens::Dot))
                        AcceptIt();
                    else
                        ErrorUnexpected(R_ExpectedIdentPrefix);
                }

                auto ast_ = Make<ObjectExpr>();

                if (expr_a)
                    ast_->area_ = expr_a->area_;

                ast_->prefix_expr_ = expr_a;
                ast_->is_static_ = is_static_;
                ast_->ident_ = ParseIdent();

                return UpdateSourceArea(ast_);
            }

            AssignExprPtr HLSLParser::ParseAssignExpr(const ExprPtr& expr_a)
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

            ExprPtr HLSLParser::ParseObjectOrCallExpr(const ExprPtr& expr_a)
            {
                auto object_expr_ = ParseObjectExpr(expr_a);

                if (Is(Tokens::LBracket))
                    return ParseCallExpr(object_expr_);

                return object_expr_;
            }

            CallExprPtr HLSLParser::ParseCallExpr(
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

            CallExprPtr HLSLParser::ParseCallExprWithPrefixOpt(
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

            CallExprPtr HLSLParser::ParseCallExprAsTypeCtor(
                    const TypeDenoterPtr& type_denoter_a)
            {
                auto ast_ = Make<CallExpr>();
                ast_->type_denoter_ = type_denoter_a;
                ast_->arguments_ = ParseArgumentList();

                return UpdateSourceArea(ast_);
            }

            std::vector<StmntPtr> HLSLParser::ParseGlobalStmntList()
            {
                std::vector<StmntPtr> stmnts_;

                Accept(Tokens::LCurly);
                while (!Is(Tokens::RCurly))
                {
                    ParseAndIgnoreTechniquesAndNullStmnts();
                    ParseStmntWithCommentOpt(
                            stmnts_,
                            std::bind(&HLSLParser::ParseGlobalStmnt, this));
                }

                AcceptIt();

                return stmnts_;
            }

            std::vector<VarDeclStmntPtr> HLSLParser::ParseAnnotationList()
            {
                std::vector<VarDeclStmntPtr> annotations_;

                if (Is(Tokens::BinaryOp, "<"))
                {
                    AcceptIt();

                    while (!Is(Tokens::BinaryOp, ">"))
                        annotations_.push_back(ParseVarDeclStmnt());

                    AcceptIt();
                }

                return annotations_;
            }

            std::vector<RegisterPtr> HLSLParser::ParseRegisterList(
                    bool parse_first_colon_a)
            {
                std::vector<RegisterPtr> registers_;

                if (parse_first_colon_a && Is(Tokens::Register))
                    registers_.push_back(ParseRegister(false));

                while (Is(Tokens::Colon))
                    registers_.push_back(ParseRegister());

                return registers_;
            }

            std::vector<AttributePtr> HLSLParser::ParseAttributeList()
            {
                std::vector<AttributePtr> attribs_;

                while (Is(Tokens::LParen))
                    attribs_.push_back(ParseAttribute());

                return attribs_;
            }

            std::vector<BufferDeclPtr> HLSLParser::ParseBufferDeclList(
                    BufferDeclStmnt* decl_stmnt_ref_a,
                    const TokenPtr& ident_tkn_a)
            {
                std::vector<BufferDeclPtr> buffer_decls_;

                buffer_decls_.push_back(ParseBufferDecl(decl_stmnt_ref_a, ident_tkn_a));

                while (Is(Tokens::Comma))
                {
                    AcceptIt();
                    buffer_decls_.push_back(ParseBufferDecl(decl_stmnt_ref_a));
                }

                return buffer_decls_;
            }

            std::vector<SamplerDeclPtr> HLSLParser::ParseSamplerDeclList(
                    SamplerDeclStmnt* decl_stmnt_ref_a,
                    const TokenPtr& ident_tkn_a)
            {
                std::vector<SamplerDeclPtr> sampler_decls_;

                sampler_decls_.push_back(
                        ParseSamplerDecl(decl_stmnt_ref_a, ident_tkn_a));

                while (Is(Tokens::Comma))
                {
                    AcceptIt();
                    sampler_decls_.push_back(ParseSamplerDecl(decl_stmnt_ref_a));
                }

                return sampler_decls_;
            }

            std::vector<SamplerValuePtr> HLSLParser::ParseSamplerValueList()
            {
                std::vector<SamplerValuePtr> sampler_values_;

                while (!Is(Tokens::RCurly))
                    sampler_values_.push_back(ParseSamplerValue());

                return sampler_values_;
            }

            std::vector<AliasDeclPtr> HLSLParser::ParseAliasDeclList(
                    TypeDenoterPtr type_denoter_a)
            {
                std::vector<AliasDeclPtr> alias_decls_;

                alias_decls_.push_back(ParseAliasDecl(type_denoter_a));

                while (Is(Tokens::Comma))
                {
                    AcceptIt();
                    alias_decls_.push_back(ParseAliasDecl(type_denoter_a));
                }

                return alias_decls_;
            }

            std::string HLSLParser::ParseIdentWithNamespaceOpt(
                    ObjectExprPtr& namespace_expr_a,
                    TokenPtr ident_tkn_a,
                    SourceArea* area_a)
            {
                SourceArea ident_area_;
                auto ident_ = ParseIdent(ident_tkn_a, &ident_area_);

                if (Is(Tokens::DColon))
                {
                    AcceptIt();

                    namespace_expr_a = Make<ObjectExpr>();
                    namespace_expr_a->ident_ = ident_;
                    namespace_expr_a->area_ = ident_area_;

                    ObjectExprPtr sub_object_expr_;
                    ident_ = ParseIdentWithNamespaceOpt(sub_object_expr_,
                                                       nullptr,
                                                       area_a);
                    
                    namespace_expr_a->prefix_expr_ = sub_object_expr_;

                    return ident_;
                }

                *area_a = ident_area_;

                return ident_;
            }

            TypeDenoterPtr HLSLParser::ParseTypeDenoter(
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

            TypeDenoterPtr HLSLParser::ParseTypeDenoterPrimary(
                    StructDeclPtr* struct_decl_a)
            {
                if (IsBaseDataType())
                    return ParseBaseTypeDenoter();
                else if (Is(Tokens::Vector))
                    return ParseBaseVectorTypeDenoter();
                else if (Is(Tokens::Matrix))
                    return ParseBaseMatrixTypeDenoter();
                else if (Is(Tokens::Ident))
                    return ParseAliasTypeDenoter();
                else if (Is(Tokens::Struct))
                {
                    if (struct_decl_a)
                        return ParseStructTypeDenoterWithStructDeclOpt(
                                *struct_decl_a);
                    else
                        return ParseStructTypeDenoter();
                } 
                else if (Is(Tokens::Buffer))
                    return ParseBufferTypeDenoter();
                else if (Is(Tokens::Sampler) || Is(Tokens::SamplerState))
                    return ParseSamplerTypeDenoter();

                ErrorUnexpected(R_ExpectedTypeDen,
                                GetScanner().ActiveToken().get(),
                                true);
                return nullptr;
            }

            TypeDenoterPtr HLSLParser::ParseTypeDenoterWithStructDeclOpt(
                    StructDeclPtr& struct_decl_a,
                    bool allow_void_type_a)
            {
                if (Is(Tokens::Struct) || Is(Tokens::Class))
                    return ParseStructTypeDenoterWithStructDeclOpt(struct_decl_a);
                else
                    return ParseTypeDenoter(allow_void_type_a);
            }

            VoidTypeDenoterPtr HLSLParser::ParseVoidTypeDenoter()
            {
                Accept(Tokens::Void);
                return std::make_shared<VoidTypeDenoter>();
            }

            BaseTypeDenoterPtr HLSLParser::ParseBaseTypeDenoter()
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

            BaseTypeDenoterPtr HLSLParser::ParseBaseVectorTypeDenoter()
            {
                std::string vector_type_;
                Accept(Tokens::Vector);

                if (Is(Tokens::BinaryOp, "<"))
                {
                    AcceptIt();

                    PushParsingState({true});
                    {
                        vector_type_ = Accept(Tokens::ScalarType)->Spell();

                        Accept(Tokens::Comma);
                        int dim_ = ParseAndEvaluateVectorDimension();

                        vector_type_ += std::to_string(dim_);
                    }
                    PopParsingState();

                    Accept(Tokens::BinaryOp, ">");
                } 
                else
                    vector_type_ = "float4";

                auto type_denoter_ = std::make_shared<BaseTypeDenoter>();
                type_denoter_->data_type_ = ParseDataType(vector_type_);

                return type_denoter_;
            }

            BaseTypeDenoterPtr HLSLParser::ParseBaseMatrixTypeDenoter()
            {
                std::string matrix_type_;

                Accept(Tokens::Matrix);

                if (Is(Tokens::BinaryOp, "<"))
                {
                    AcceptIt();

                    PushParsingState({true});
                    {
                        matrix_type_ = Accept(Tokens::ScalarType)->Spell();

                        Accept(Tokens::Comma);
                        int dim_m_ = ParseAndEvaluateVectorDimension();

                        Accept(Tokens::Comma);
                        int dim_n_ = ParseAndEvaluateVectorDimension();

                        matrix_type_ += std::to_string(dim_m_) + 'x'
                                      + std::to_string(dim_n_);
                    }
                    PopParsingState();

                    Accept(Tokens::BinaryOp, ">");
                } 
                else
                    matrix_type_ = "float4x4";

                auto type_denoter_ = std::make_shared<BaseTypeDenoter>();
                type_denoter_->data_type_ = ParseDataType(matrix_type_);

                return type_denoter_;
            }

            BufferTypeDenoterPtr HLSLParser::ParseBufferTypeDenoter()
            {
                auto type_denoter_ = std::make_shared<BufferTypeDenoter>();
                auto buffer_type_tkn_ = Tkn();
                type_denoter_->buffer_type_ = ParseBufferType();

                if (Is(Tokens::BinaryOp, "<"))
                {
                    PushParsingState({true});
                    {
                        AcceptIt();

                        if (Is(Tokens::TypeModifier))
                        {
                            auto modifier_str_ = Tkn()->Spell();
                            auto modifier_ = ParseTypeModifier();
                            if (modifier_ != TypeModifier::SNorm
                                && modifier_ != TypeModifier::UNorm)
                                Error(R_InvalidModifierForGenericTypeDen(
                                              modifier_str_),
                                      true,
                                      false);
                        }

                        type_denoter_->generic_type_denoter_ = ParseTypeDenoter(
                                false);

                        if (Is(Tokens::Comma))
                        {
                            AcceptIt();
                            auto gen_size_ = ParseAndEvaluateConstExprInt();

                            if (IsTextureMSBufferType(type_denoter_->buffer_type_))
                            {
                                if (gen_size_ < 1 || gen_size_ >= 128)
                                    Warning(R_TextureSampleCountLimitIs128(
                                                    gen_size_),
                                            buffer_type_tkn_.get());
                            } 
                            else if (IsPatchBufferType(
                                               type_denoter_->buffer_type_))
                            {
                                if (gen_size_ < 1 || gen_size_ > 64)
                                    Warning(R_PatchCtrlPointLimitIs64(gen_size_),
                                            buffer_type_tkn_.get());
                            } 
                            else
                                Error(R_IllegalBufferTypeGenericSize);

                            type_denoter_->generic_size_ = gen_size_;
                        }

                        Accept(Tokens::BinaryOp, ">");
                    }
                    PopParsingState();
                }

                return type_denoter_;
            }

            SamplerTypeDenoterPtr HLSLParser::ParseSamplerTypeDenoter()
            {
                auto sampler_type_ = ParseSamplerType();
                return std::make_shared<SamplerTypeDenoter>(sampler_type_);
            }

            StructTypeDenoterPtr HLSLParser::ParseStructTypeDenoter()
            {
                if (Is(Tokens::Struct))
                    AcceptIt();

                auto ident_ = ParseIdent();
                auto type_denoter_ = std::make_shared<StructTypeDenoter>(ident_);

                return type_denoter_;
            }

            StructTypeDenoterPtr
            HLSLParser::ParseStructTypeDenoterWithStructDeclOpt(
                    StructDeclPtr& struct_decl_a)
            {
                bool is_class_ = false;

                if (Is(Tokens::Class))
                {
                    AcceptIt();
                    is_class_ = true;
                } 
                else
                    Accept(Tokens::Struct);

                if (Is(Tokens::LCurly))
                {
                    struct_decl_a = ParseStructDecl(false);
                    struct_decl_a->is_class_ = is_class_;

                    return std::make_shared<StructTypeDenoter>(
                            struct_decl_a.get());
                }
                else
                {
                    auto struct_ident_tkn_ = Accept(Tokens::Ident);

                    if (Is(Tokens::LCurly) || Is(Tokens::Colon))
                    {
                        struct_decl_a = ParseStructDecl(false, struct_ident_tkn_);
                        struct_decl_a->is_class_ = is_class_;

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

            AliasTypeDenoterPtr HLSLParser::ParseAliasTypeDenoter(
                    std::string ident_a)
            {
                if (ident_a.empty())
                    ident_a = ParseIdent();

                return std::make_shared<AliasTypeDenoter>(ident_a);
            }

            void HLSLParser::ParseAndIgnoreTechniquesAndNullStmnts()
            {
                while (Is(Tokens::Semicolon) || Is(Tokens::Technique))
                {
                    if (Is(Tokens::Technique))
                        ParseAndIgnoreTechnique();
                    else
                        AcceptIt();
                }
            }

            void HLSLParser::ParseAndIgnoreTechnique()
            {
                Accept(Tokens::Technique);
                Warning(R_TechniquesAreIgnored);

                std::stack<TokenPtr> brace_tkn_stack_;
                while (!Is(Tokens::LCurly))
                    AcceptIt();

                brace_tkn_stack_.push(Accept(Tokens::LCurly));

                while (!brace_tkn_stack_.empty())
                {
                    if (Is(Tokens::LCurly))
                        brace_tkn_stack_.push(Tkn());
                    else if (Is(Tokens::RCurly))
                        brace_tkn_stack_.pop();
                    else if (Is(Tokens::EndOfStream))
                        Error(R_MissingClosingBrace, brace_tkn_stack_.top().get());
                    AcceptIt();
                }
            }

            void HLSLParser::ParseVarDeclSemantic(VarDecl& var_decl_a,
                                                  bool allow_pack_offset_a)
            {
                while (Is(Tokens::Colon))
                {
                    Accept(Tokens::Colon);

                    if (Is(Tokens::Register))
                    {
                        var_decl_a.slot_registers_.push_back(ParseRegister(false));
                    } 
                    else if (Is(Tokens::PackOffset))
                    {
                        auto pack_offset_ = ParsePackOffset(false);
                        if (allow_pack_offset_a)
                        {
                            if (var_decl_a.pack_offset_)
                                Warning(R_PackOffsetOverridden,
                                        pack_offset_->area_);
                            var_decl_a.pack_offset_ = pack_offset_;
                        } else
                            Error(R_IllegalPackOffset, pack_offset_->area_);
                    }
                    else
                    {
                        var_decl_a.semantic_ = ParseSemantic(false);
                    }
                }
            }

            void HLSLParser::ParseFunctionDeclSemantic(FunctionDecl& func_decl_a)
            {
                while (Is(Tokens::Colon))
                {
                    Accept(Tokens::Colon);

                    if (Is(Tokens::Register))
                    {
                        Warning(R_RegisterIgnoredForFuncDecls);
                        ParseRegister(false);
                    } 
                    else if (Is(Tokens::PackOffset))
                    {
                        Error(R_IllegalPackOffset, true);
                        ParsePackOffset(false);
                    } 
                    else
                    {
                        func_decl_a.semantic_ = ParseSemantic(false);
                    }
                }
            }

            DataType HLSLParser::ParseDataType(const std::string& keyword_a)
            {
                try
                {
                    if (enable_cg_keywords_)
                        return HLSLKeywordExtCgToDataType(keyword_a);
                    else
                        return HLSLKeywordToDataType(keyword_a);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return DataType::Undefined;
            }

            PrimitiveType HLSLParser::ParsePrimitiveType()
            {
                try
                {
                    return HLSLKeywordToPrimitiveType(
                            Accept(Tokens::PrimitiveType)->Spell());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return PrimitiveType::Undefined;
            }

            InterpModifier HLSLParser::ParseInterpModifier()
            {
                try
                {
                    return HLSLKeywordToInterpModifier(
                            Accept(Tokens::InterpModifier)->Spell());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return InterpModifier::Undefined;
            }

            TypeModifier HLSLParser::ParseTypeModifier()
            {
                try
                {
                    return HLSLKeywordToTypeModifier(
                            Accept(Tokens::TypeModifier)->Spell());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return TypeModifier::Undefined;
            }

            StorageClass HLSLParser::ParseStorageClass()
            {
                try
                {
                    return HLSLKeywordToStorageClass(
                            Accept(Tokens::StorageClass)->Spell());
                }
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return StorageClass::Undefined;
            }

            UniformBufferType HLSLParser::ParseUniformBufferType()
            {
                try
                {
                    return HLSLKeywordToUniformBufferType(
                            Accept(Tokens::UniformBuffer)->Spell());
                }
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return UniformBufferType::Undefined;
            }

            BufferType HLSLParser::ParseBufferType()
            {
                try
                {
                    return HLSLKeywordToBufferType(
                            Accept(Tokens::Buffer)->Spell());
                }
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return BufferType::Undefined;
            }

            SamplerType HLSLParser::ParseSamplerType()
            {
                try
                {
                    if (Is(Tokens::Sampler) || Is(Tokens::SamplerState))
                        return HLSLKeywordToSamplerType(AcceptIt()->Spell());
                    else
                        ErrorUnexpected(R_ExpectedSamplerOrSamplerState);
                } catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return SamplerType::Undefined;
            }

            IndexedSemantic HLSLParser::ParseSemantic(bool parse_colon_a)
            {
                try
                {
                    if (parse_colon_a)
                        Accept(Tokens::Colon);
                    return HLSLKeywordToSemantic(ParseIdent(),
                                                 use_d3d10_semantics_);
                }
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }
                return Semantic::UserDefined;
            }

            std::string HLSLParser::ParseSamplerStateTextureIdent()
            {
                std::string ident_;
                if (Is(Tokens::Ident))
                    Accept(Tokens::Ident, "Texture");
                else
                    Accept(Tokens::Buffer, "texture");

                Accept(Tokens::AssignOp, "=");

                if (Is(Tokens::LBracket))
                {
                    AcceptIt();
                    ident_ = ParseIdent();
                    Accept(Tokens::RBracket);
                }
                else if (Is(Tokens::BinaryOp, "<"))
                {
                    AcceptIt();
                    ident_ = ParseIdent();
                    Accept(Tokens::BinaryOp, ">");
                }
                else
                    ErrorUnexpected(R_ExpectedOpenBracketOrAngleBracket);

                Semi();

                return ident_;
            }

            bool HLSLParser::ParseModifiers(TypeSpecifier* type_specifier_a,
                                            bool allow_primitive_type_a)
            {
                if (Is(Tokens::InputModifier))
                {
                    auto modifier_ = AcceptIt()->Spell();
                    if (modifier_ == "in")
                        type_specifier_a->is_input_ = true;
                    else if (modifier_ == "out")
                        type_specifier_a->is_output_ = true;
                    else if (modifier_ == "inout")
                    {
                        type_specifier_a->is_input_ = true;
                        type_specifier_a->is_output_ = true;
                    } 
                    else if (modifier_ == "uniform")
                        type_specifier_a->is_uniform_ = true;
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