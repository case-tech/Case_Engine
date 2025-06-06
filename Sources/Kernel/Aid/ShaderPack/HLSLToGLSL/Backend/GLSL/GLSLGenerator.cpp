#include "GLSLGenerator.hpp"

#include "../../AST/TypeDenoter.hpp"
#include "../../AST/Visitor/ExprConverter.hpp"
#include "../../AST/Visitor/FuncNameConverter.hpp"
#include "../../AST/Visitor/ReferenceAnalyzer.hpp"
#include "../../AST/Visitor/StructParameterAnalyzer.hpp"
#include "../../AST/Visitor/TypeConverter.hpp"
#include "../../AST/Visitor/UniformPacker.hpp"
#include "../../Exception.hpp"
#include "../../Frontend/GLSL/GLSLKeywords.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "GLSLConverter.hpp"
#include "GLSLExtensionAgent.hpp"
#include "GLSLIntrinsics.hpp"

#include <algorithm>
#include <cctype>
#include <initializer_list>
#include <set>
#include <sstream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct IfStmntArgs
            {
                bool in_has_else_parent_node_;
            };

            struct StructDeclArgs
            {
                bool in_end_with_semicolon_;
            };

            GLSLGenerator::GLSLGenerator(Log* log_a) : Generator {log_a}
            {}

            void GLSLGenerator::GenerateCodePrimary(
                    Program& program_a,
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a)
            {
                version_out_ = output_desc_a.shader_version_;
                name_mangling_ = output_desc_a.name_mangling_;
                allow_extensions_ = output_desc_a.options_.allow_extensions_;
                explicit_binding_ = output_desc_a.options_.explicit_binding_;
                preserve_comments_ = output_desc_a.options_.preserveComments;
                separate_shaders_ = output_desc_a.options_.separate_shaders_;
                separate_samplers_ = output_desc_a.options_.separate_samplers_;
                auto_binding_ = output_desc_a.options_.auto_binding_;
                write_header_comment_ = output_desc_a.options_.write_generator_header_;
                allow_line_marks_ = output_desc_a.formatting_.line_marks_;
                compact_wrappers_ = output_desc_a.formatting_.compact_wrappers_;
                always_braced_scopes_ = output_desc_a.formatting_.always_braced_scopes_;
                uniform_packing_ = output_desc_a.uniform_packing_;
                entry_point_name_ = input_desc_a.entry_point_;

                for (const auto& s_ : output_desc_a.vertex_semantics_)
                {
                    const auto semantic_ci_ = ToCiString(s_.semantic_);
                    vertex_semantics_map_[semantic_ci_] = {s_.location_, 0};

                    if (s_.location_ >= 0)
                        used_in_locations_set_.insert(s_.location_);
                }

                if (program_a.entry_point_ref_)
                {
                    try
                    {
                        PreProcessAST(input_desc_a, output_desc_a);

                        Visit(&program_a);

                        ReportOptionalFeedback();
                    } 
                    catch (const Report&)
                    {
                        throw;
                    } 
                    catch (const ASTRuntimeError& e_)
                    {
                        Error(e_.what(), e_.GetAST());
                    } 
                    catch (const std::exception& e_)
                    {
                        Error(e_.what());
                    }
                } 
                else
                    Error(R_EntryPointNotFound(input_desc_a.entry_point_));
            }

            std::unique_ptr<std::string> GLSLGenerator::SystemValueToKeyword(
                    const IndexedSemantic& semantic_a) const
            {
                if (semantic_a == Semantic::Target && !IsGLSL120OrESSL100())
                    return MakeUnique<std::string>(semantic_a.ToString());
                else
                    return SemanticToGLSLKeyword(semantic_a, IsVKSL());
            }

            bool GLSLGenerator::IsWrappedIntrinsic(
                    const Intrinsic intrinsic_a) const
            {
                static const std::set<Intrinsic> wrapped_intrinsics_ 
                {
                        Intrinsic::Clip,
                        Intrinsic::Lit,
                        Intrinsic::SinCos,
                        Intrinsic::GroupMemoryBarrierWithGroupSync,
                        Intrinsic::DeviceMemoryBarrier,
                        Intrinsic::DeviceMemoryBarrierWithGroupSync,
                        Intrinsic::AllMemoryBarrierWithGroupSync,
                        Intrinsic::F16toF32
                };
                
                return (wrapped_intrinsics_.find(intrinsic_a)
                        != wrapped_intrinsics_.end());
            }

            bool GLSLGenerator::IsGLSL() const
            {
                return IsLanguageGLSL(version_out_);
            }

            bool GLSLGenerator::IsESSL() const
            {
                return IsLanguageESSL(version_out_);
            }

            bool GLSLGenerator::IsVKSL() const
            {
                return IsLanguageVKSL(version_out_);
            }

            bool GLSLGenerator::IsGLSL120OrESSL100() const
            {
                return (version_out_ <= OutputShaderVersion::GLSL120
                        || version_out_ == OutputShaderVersion::ESSL100);
            }

            bool GLSLGenerator::HasShadingLanguage420Pack() const
            {
                return (IsVKSL()
                        || (version_out_ >= OutputShaderVersion::GLSL420
                            && version_out_ <= OutputShaderVersion::GLSL450));
            }

            bool GLSLGenerator::UseSeparateSamplers() const
            {
                return (IsVKSL() && separate_samplers_);
            }

            const std::string* GLSLGenerator::BufferTypeToKeyword(
                    const BufferType buffer_type_a,
                    const AST* ast_a)
            {
                if (auto keyword_ =
                            BufferTypeToGLSLKeyword(buffer_type_a,
                                                    IsVKSL(),
                                                    UseSeparateSamplers()))
                    return keyword_;
                else
                    Error(R_FailedToMapToGLSLKeyword(R_BufferType), ast_a);
                return nullptr;
            }

            const std::string* GLSLGenerator::SamplerTypeToKeyword(
                    const SamplerType sampler_type_a,
                    const AST* ast_a)
            {
                if (auto keyword_ = SamplerTypeToGLSLKeyword(sampler_type_a))
                    return keyword_;
                else
                    Error(R_FailedToMapToGLSLKeyword(R_SamplerType), ast_a);
                return nullptr;
            }

            bool GLSLGenerator::IsTypeCompatibleWithSemantic(
                    const Semantic semantic_a,
                    const TypeDenoter& type_denoter_a)
            {
                if (auto base_type_den_ = type_denoter_a.As<BaseTypeDenoter>())
                {
                    auto data_type_ = base_type_den_->data_type_;

                    switch (semantic_a)
                    {
                    case Semantic::DispatchThreadID:
                    case Semantic::GroupID:
                    case Semantic::GroupThreadID:
                        return (data_type_ == DataType::UInt3);

                    case Semantic::GroupIndex:
                        return (data_type_ == DataType::UInt);

                    case Semantic::GSInstanceID:
                    case Semantic::InstanceID:
                    case Semantic::OutputControlPointID:
                    case Semantic::PrimitiveID:
                    case Semantic::SampleIndex:
                    case Semantic::VertexID:
                        return (data_type_ == DataType::Int);

                    default:
                        break;
                    }
                    return true;
                }
                return false;
            }

            void GLSLGenerator::ReportOptionalFeedback()
            {
                if (WarnEnabled(Warnings::UnlocatedObjects) && explicit_binding_
                    && IsVertexShader())
                {
                    std::map<int, int> location_use_count_;

                    for (const auto& vert_semantic_ : vertex_semantics_map_)
                    {
                        const auto& sem_ = vert_semantic_.second;
                        if (sem_.found_)
                            ++location_use_count_[sem_.location_];
                        else
                            Warning(R_VertexSemanticNotFound(
                                    ToString(vert_semantic_.first)));
                    }

                    for (const auto& loc_ : location_use_count_)
                    {
                        if (loc_.second > 1)
                            Warning(R_MultiUseOfVertexSemanticLocation(
                                    loc_.first,
                                    loc_.second));
                    }
                }
            }

            void GLSLGenerator::ErrorIntrinsic(const std::string& intrinsic_name_a,
                                               const AST* ast_a)
            {
                Error(R_FailedToMapToGLSLKeyword(R_Intrinsic(intrinsic_name_a)),
                      ast_a);
            }

            int GLSLGenerator::GetNumBindingLocations(
                    const TypeDenoter* type_denoter_a)
            {
                if (!type_denoter_a)
                    return -1;

                int num_array_elements_ = 1;

                while (auto array_type_den_ = type_denoter_a->As<ArrayTypeDenoter>())
                {
                    num_array_elements_ *= array_type_den_->NumArrayElements();
                    type_denoter_a = array_type_den_->sub_type_denoter_.get();
                }

                if (num_array_elements_ == 0)
                    return -1;

                if (auto base_type_den_ =
                            type_denoter_a->GetAliased().As<BaseTypeDenoter>())
                {
                    const auto data_type_ = base_type_den_->data_type_;

                    int element_size_ = 0;

                    if (IsScalarType(data_type_))
                    {
                        element_size_ = 1;
                    } 
                    else if (IsVectorType(data_type_))
                    {
                        int dims_ = VectorTypeDim(data_type_);

                        if (IsDoubleRealType(data_type_) && dims_ > 2)
                            element_size_ = 2;
                        else
                            element_size_ = 1;
                    } 
                    else if (IsMatrixType(data_type_))
                    {
                        auto dims_ = MatrixTypeDim(data_type_);

                        int row_dim_ = dims_.second;
                        int row_size_ = 0;

                        if (IsDoubleRealType(data_type_) && row_dim_ > 2)
                            row_size_ = 2;
                        else
                            row_size_ = 1;

                        element_size_ = dims_.first * row_size_;
                    }

                    if (element_size_ != 0)
                        return element_size_ * num_array_elements_;
                }

                return -1;
            }

            int GLSLGenerator::GetBindingLocation(
                    const TypeDenoter* type_denoter_a,
                    bool input_a)
            {
                int num_locations_ = GetNumBindingLocations(type_denoter_a);
                if (num_locations_ == -1)
                    return -1;

                int start_location_ = 0;
                int end_location_ = start_location_ + num_locations_ - 1;

                auto& used_locations_set_ = (input_a ? used_in_locations_set_
                                                : used_out_locations_set_);
                for (auto entry_ : used_locations_set_)
                {
                    if (entry_ >= start_location_ && entry_ <= end_location_)
                    {
                        start_location_ = entry_ + 1;
                        end_location_ = start_location_ + num_locations_ - 1;
                    } 
                    else if (entry_ > end_location_)
                        break;
                }

                for (auto i_ = start_location_; i_ <= end_location_; ++i_)
                    used_locations_set_.insert(i_);

                return start_location_;
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void GLSLGenerator::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(Program)
            {
                (void)args_a;
                WriteProgramHeader();
                WriteGlobalLayouts();

                if (separate_shaders_
                    && version_out_ > OutputShaderVersion::GLSL140)
                    WriteBuiltinBlockRedeclarations();

                WriteWrapperIntrinsics();
                WriteGlobalUniforms();

                if (auto entry_point_ = GetProgram()->entry_point_ref_)
                {
                    BeginSep();
                    {
                        WriteGlobalInputSemantics(entry_point_);
                    }
                    EndSep();

                    BeginSep();
                    {
                        WriteGlobalOutputSemantics(entry_point_);
                    }
                    EndSep();
                }

                WriteStmntList(ast_a->global_stmnts_, true);
            }

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                WriteScopeOpen();
                {
                    WriteStmntList(ast_a->stmnts_);
                }
                WriteScopeClose();
            }

            IMPLEMENT_VISIT_PROC(SwitchCase)
            {
                (void)args_a;
                if (ast_a->expr_)
                {
                    BeginLn();
                    {
                        Write("case ");
                        Visit(ast_a->expr_);
                        Write(":");
                    }
                    EndLn();
                } 
                else
                    WriteLn("default:");

                IncIndent();
                {
                    Visit(ast_a->stmnts_);
                }
                DecIndent();
            }

            IMPLEMENT_VISIT_PROC(ArrayDimension)
            {
                (void)args_a;
                Write(ast_a->ToString());
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifier)
            {
                (void)args_a;
                if (ast_a->struct_decl_)
                    Visit(ast_a->struct_decl_);
                else
                    WriteTypeDenoter(*ast_a->type_denoter_, IsESSL(), ast_a);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                (void)args_a;
                if (auto static_member_var_ = ast_a->FetchStaticVarDeclRef())
                    Write(static_member_var_->ident_);
                else
                    Write(InsideStructDecl() ? ast_a->ident_.Original()
                                             : ast_a->ident_.Final());

                Visit(ast_a->array_dims_);

                if (ast_a->initializer_)
                {
                    const auto& type_den_ =
                            ast_a->initializer_->GetTypeDenoter()->GetAliased();
                   
                    if (!type_den_.IsNull())
                    {
                        Write(" = ");
                        Visit(ast_a->initializer_);
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                if (ast_a->flags_(StructDecl::isNonEntryPointParam)
                    || !ast_a->flags_(StructDecl::IsShaderInput
                                   | StructDecl::IsShaderOutput))
                {
                    PushStructDecl(ast_a);
                    {
                        if (auto struct_decl_args_ =
                                    reinterpret_cast<StructDeclArgs*>(args_a))
                            WriteStructDecl(ast_a,
                                            struct_decl_args_->in_end_with_semicolon_);
                        else
                            WriteStructDecl(ast_a, false);
                    }
                    PopStructDecl();
                }
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                (void)args_a;
                WriteSamplerDecl(*ast_a);
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                if (!ast_a->flags_(AST::IsReachable))
                {
                    if (WarnEnabled(Warnings::Basic)
                        && ast_a->flags_(FunctionDecl::HasNonReturnControlPath))
                        Warning(R_InvalidControlPathInUnrefFunc(
                                        ast_a->ToString()),
                                ast_a);
                    return;
                }

                if (ast_a->flags_(FunctionDecl::HasNonReturnControlPath))
                    Error(R_InvalidControlPathInFunc(ast_a->ToString()), ast_a);

                WriteLineMark(ast_a);

                PushFunctionDecl(ast_a);
                {
                    if (ast_a->flags_(FunctionDecl::IsEntryPoint))
                        WriteFunctionEntryPoint(ast_a);
                    else if (ast_a->flags_(FunctionDecl::IsSecondaryEntryPoint))
                        WriteFunctionSecondaryEntryPoint(ast_a);
                    else
                        WriteFunction(ast_a);
                }
                PopFunctionDecl();

                Blank();
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)args_a;
                if (!ast_a->flags_(AST::IsReachable))
                    return;

                if (version_out_ < OutputShaderVersion::GLSL140)
                {
                    for (auto& var_decl_stmnt_ : ast_a->var_members_)
                    {
                        var_decl_stmnt_->type_specifier_->is_uniform_ = true;
                        Visit(var_decl_stmnt_);
                    }
                } 
                else
                {
                    WriteLineMark(ast_a);
                    ast_a->DeriveCommonStorageLayout();

                    BeginLn();

                    WriteLayout({
                            [&]() { Write("std140"); },
                            [&]() {
                                if (ast_a->common_storage_layout_
                                    == TypeModifier::RowMajor)
                                    Write("row_major");
                            },
                            [&]() { WriteLayoutBinding(ast_a->slot_registers_); },
                    });

                    Write("uniform " + ast_a->ident_);
                    WriteScopeOpen(false, true);
                    BeginSep();
                    {
                        PushUniformBufferDecl(ast_a);
                        {
                            WriteStmntList(ast_a->var_members_);
                        }
                        PopUniformBufferDecl();
                    }
                    EndSep();
                    WriteScopeClose();
                }

                Blank();
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                (void)args_a;
                if (ast_a->flags_(AST::IsReachable))
                {
                    for (auto& buffer_decl_ : ast_a->buffer_decls_)
                        WriteBufferDecl(buffer_decl_.get());
                }
            }

            IMPLEMENT_VISIT_PROC(SamplerDeclStmnt)
            {
                (void)args_a;
                if (ast_a->flags_(AST::IsReachable))
                {
                    if (UseSeparateSamplers()
                        || !IsSamplerStateType(ast_a->type_denoter_->sampler_type_))
                        Visit(ast_a->sampler_decls_);
                }
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                (void)args_a;
                if (!ast_a->flags_(AST::IsReachable) && !InsideFunctionDecl()
                    && !InsideStructDecl())
                    return;

                auto var_decls_ = ast_a->var_decls_;
                auto var_type_struct_decl_ = ast_a->type_specifier_->GetStructDeclRef();

                for (auto it_ = var_decls_.begin(); it_ != var_decls_.end();)
                {
                    auto var_decl_ = it_->get();

                    if (var_decl_->flags_(VarDecl::IsEntryPointLocal)
                        && (!var_type_struct_decl_
                            || !var_type_struct_decl_->flags_(
                                    StructDecl::isNonEntryPointParam)))
                    {
                        it_ = var_decls_.erase(it_);
                    } 
                    else
                        ++it_;
                }

                if (var_decls_.empty())
                {
                    return;
                }

                const auto& var_decl0_ = var_decls_.front();

                if (ast_a->type_specifier_->HasAnyStorageClassOf(
                            {StorageClass::Static})
                    && ast_a->FetchStructDeclRef() != nullptr)
                    return;

                PushVarDeclStmnt(ast_a);
                {
                    BeginLn();

                    if (!var_decl0_->slot_registers_.empty())
                    {
                        WriteLayout({
                                [&]() {
                                    WriteLayoutBindingOrLocation(
                                            var_decl0_->slot_registers_);
                                },
                        });
                    }

                    if (!InsideStructDecl())
                    {
                        WriteInterpModifiers(
                                ast_a->type_specifier_->interp_modifiers_,
                                ast_a);
                        WriteStorageClasses(ast_a->type_specifier_->storage_ñlasses_,
                                            ast_a);
                    }

                    Separator();

                    if (ast_a->flags_(VarDeclStmnt::IsShaderInput))
                        Write("in ");
                    else if (ast_a->flags_(VarDeclStmnt::IsShaderOutput))
                        Write("out ");
                    else if (ast_a->IsUniform())
                        Write("uniform ");

                    Separator();

                    WriteTypeModifiersFrom(ast_a->type_specifier_);
                    Separator();

                    if (ast_a->type_specifier_->struct_decl_)
                    {
                        Visit(ast_a->type_specifier_);
                        BeginLn();
                    } else
                    {
                        Visit(ast_a->type_specifier_);
                        Write(" ");
                    }

                    Separator();

                    for (std::size_t i = 0; i < var_decls_.size(); ++i)
                    {
                        Visit(var_decls_[i]);
                        if (i + 1 < var_decls_.size())
                            Write(", ");
                    }

                    Write(";");
                    EndLn();
                }
                PopVarDeclStmnt();

                if (InsideGlobalScope())
                    Blank();
            }

            IMPLEMENT_VISIT_PROC(AliasDeclStmnt)
            {
                (void)args_a;
                if (ast_a->struct_decl_ && !ast_a->struct_decl_->IsAnonymous())
                {
                    WriteLineMark(ast_a);

                    StructDeclArgs struct_decl_args_;
                    struct_decl_args_.in_end_with_semicolon_ = true;

                    Visit(ast_a->struct_decl_, &struct_decl_args_);
                }
            }

            IMPLEMENT_VISIT_PROC(BasicDeclStmnt)
            {
                (void)args_a;
                if (ast_a->flags_(AST::IsReachable))
                {
                    if (auto struct_decl_ = ast_a->decl_object_->As<StructDecl>())
                    {
                        if (struct_decl_->flags_(StructDecl::isNonEntryPointParam)
                            || !struct_decl_->flags_(StructDecl::IsShaderInput
                                                  | StructDecl::IsShaderOutput))
                        {
                            WriteLineMark(ast_a);

                            StructDeclArgs struct_decl_args_;
                            struct_decl_args_.in_end_with_semicolon_ = true;

                            Visit(struct_decl_, &struct_decl_args_);
                        }
                    } 
                    else
                    {
                        Visit(ast_a->decl_object_);
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(NullStmnt)
            {
                (void)ast_a;
                (void)args_a;
                WriteLn(";");
            }

            IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
            {
                (void)args_a;
                Visit(ast_a->code_block_);
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                BeginLn();

                Write("for (");

                PushOptions({false, false});
                {
                    if (ast_a->init_stmnt_->Type() == AST::Types::SamplerDeclStmnt
                        && !UseSeparateSamplers())
                        Write(";");
                    else
                        Visit(ast_a->init_stmnt_);
                    Write(" ");
                    Visit(ast_a->condition_);
                    Write("; ");
                    Visit(ast_a->iteration_);
                }
                PopOptions();

                Write(")");

                WriteScopedStmnt(ast_a->body_stmnt_.get());
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                BeginLn();

                Write("while (");
                Visit(ast_a->condition_);
                Write(")");

                WriteScopedStmnt(ast_a->body_stmnt_.get());
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                BeginLn();

                Write("do");
                WriteScopedStmnt(ast_a->body_stmnt_.get());
                WriteScopeContinue();

                Write("while (");
                Visit(ast_a->condition_);
                Write(");");

                EndLn();
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                bool has_else_parent_node_ =
                        (args_a != nullptr ? reinterpret_cast<IfStmntArgs*>(args_a)
                                                   ->in_has_else_parent_node_
                                         : false);

                if (!has_else_parent_node_)
                    BeginLn();

                Write("if (");
                Visit(ast_a->condition_);
                Write(")");

                WriteScopedStmnt(ast_a->body_stmnt_.get());

                Visit(ast_a->else_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                (void)args_a;
                if (ast_a->body_stmnt_->Type() == AST::Types::IfStmnt)
                {
                    WriteScopeContinue();
                    Write("else ");

                    if (ast_a->body_stmnt_->Type() == AST::Types::IfStmnt)
                    {
                        IfStmntArgs ifStmntArgs;
                        ifStmntArgs.in_has_else_parent_node_ = true;
                        Visit(ast_a->body_stmnt_, &ifStmntArgs);
                    } 
                    else
                        Visit(ast_a->body_stmnt_);
                } 
                else
                {
                    WriteScopeContinue();
                    Write("else");
                    WriteScopedStmnt(ast_a->body_stmnt_.get());
                }
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                (void)args_a;
                BeginLn();

                Write("switch (");
                Visit(ast_a->selector_);
                Write(")");

                WriteScopeOpen();
                {
                    Visit(ast_a->cases_);
                }
                WriteScopeClose();
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                (void)args_a;
                BeginLn();
                {
                    Visit(ast_a->expr_);
                    Write(";");
                }
                EndLn();
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                (void)args_a;
                if (InsideEntryPoint() || InsideSecondaryEntryPoint())
                {
                    WriteOutputSemanticsAssignment(ast_a->expr_.get());

                    if (!ast_a->flags_(ReturnStmnt::IsEndOfFunction))
                        WriteLn("return;");
                } 
                else
                {
                    if (ast_a->expr_)
                    {
                        BeginLn();
                        {
                            Write("return ");
                            Visit(ast_a->expr_);
                            Write(";");
                        }
                        EndLn();
                    } 
                    else if (!ast_a->flags_(ReturnStmnt::IsEndOfFunction))
                        WriteLn("return;");
                }
            }

            IMPLEMENT_VISIT_PROC(CtrlTransferStmnt)
            {
                (void)args_a;
                WriteLn(CtrlTransformToString(ast_a->transfer_) + ";");
            }

            IMPLEMENT_VISIT_PROC(SequenceExpr)
            {
                (void)args_a;
                for (std::size_t i_ = 0, n_ = ast_a->exprs_.size(); i_ < n_; ++i_)
                {
                    Visit(ast_a->exprs_[i_]);
                    if (i_ + 1 < n_)
                        Write(", ");
                }
            }

            IMPLEMENT_VISIT_PROC(LiteralExpr)
            {
                (void)args_a;
                const bool enable_suffix_ = !(
                        version_out_ == OutputShaderVersion::GLSL110
                        || version_out_ == OutputShaderVersion::ESSL100);
                Write(ast_a->GetLiteralValue(enable_suffix_));
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifierExpr)
            {
                (void)args_a;
                WriteTypeDenoter(*ast_a->type_specifier_->type_denoter_, false, ast_a);
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                (void)args_a;
                Visit(ast_a->cond_expr_);
                Write(" ? ");
                Visit(ast_a->then_expr_);
                Write(" : ");
                Visit(ast_a->else_expr_);
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                (void)args_a;
                Visit(ast_a->lhs_expr_);
                Write(" " + BinaryOpToString(ast_a->op_) + " ");
                Visit(ast_a->rhs_expr_);
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                (void)args_a;
                Write(UnaryOpToString(ast_a->op_));
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);
                Write(UnaryOpToString(ast_a->op_));
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                (void)args_a;
                if (ast_a->intrinsic_ == Intrinsic::Mul)
                    WriteCallExprIntrinsicMul(ast_a);
                else if (ast_a->intrinsic_ == Intrinsic::Rcp)
                    WriteCallExprIntrinsicRcp(ast_a);
                else if (ast_a->intrinsic_ == Intrinsic::Clip
                         && ast_a->flags_(CallExpr::CanInlineIntrinsicWrapper))
                    WriteCallExprIntrinsicClip(ast_a);
                else if (ast_a->intrinsic_
                         == Intrinsic::InterlockedCompareExchange)
                    WriteCallExprIntrinsicAtomicCompSwap(ast_a);
                else if (ast_a->intrinsic_ >= Intrinsic::InterlockedAdd
                         && ast_a->intrinsic_ <= Intrinsic::InterlockedXor)
                    WriteCallExprIntrinsicAtomic(ast_a);
                else if (ast_a->intrinsic_ == Intrinsic::Image_AtomicCompSwap)
                    WriteCallExprIntrinsicImageAtomicCompSwap(ast_a);
                else if (ast_a->intrinsic_ >= Intrinsic::Image_AtomicAdd
                         && ast_a->intrinsic_ <= Intrinsic::Image_AtomicExchange)
                    WriteCallExprIntrinsicImageAtomic(ast_a);
                else if (ast_a->intrinsic_ == Intrinsic::StreamOutput_Append)
                    WriteCallExprIntrinsicStreamOutputAppend(ast_a);
                else if (ast_a->intrinsic_ == Intrinsic::Texture_QueryLod)
                    WriteCallExprIntrinsicTextureQueryLod(ast_a, true);
                else if (ast_a->intrinsic_ == Intrinsic::Texture_QueryLodUnclamped)
                    WriteCallExprIntrinsicTextureQueryLod(ast_a, false);
                else
                    WriteCallExprStandard(ast_a);
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                (void)args_a;
                Write("(");
                Visit(ast_a->expr_);
                Write(")");
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                (void)args_a;
                WriteObjectExpr(*ast_a);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                (void)args_a;
                Visit(ast_a->lvalue_expr_);
                Write(" " + AssignOpToString(ast_a->op_) + " ");
                Visit(ast_a->rvalue_expr_);
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                (void)args_a;
                WriteArrayExpr(*ast_a);
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                (void)args_a;
                WriteTypeDenoter(*ast_a->type_specifier_->type_denoter_, false, ast_a);
                Write("(");
                Visit(ast_a->expr_);
                Write(")");
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                (void)args_a;
                if (ast_a->GetTypeDenoter()->GetAliased().IsArray())
                {
                    WriteScopeOpen();

                    for (std::size_t i_ = 0; i_ < ast_a->exprs_.size(); ++i_)
                    {
                        BeginLn();
                        Visit(ast_a->exprs_[i_]);
                        if (i_ + 1 < ast_a->exprs_.size())
                            Write(",");
                        EndLn();
                    }

                    WriteScopeClose();
                    BeginLn();
                } 
                else
                {
                    Write("{ ");

                    for (std::size_t i_ = 0; i_ < ast_a->exprs_.size(); ++i_)
                    {
                        Visit(ast_a->exprs_[i_]);
                        if (i_ + 1 < ast_a->exprs_.size())
                            Write(", ");
                    }

                    Write(" }");
                }
            }

#undef IMPLEMENT_VISIT_PROC
            void GLSLGenerator::PreProcessAST(const ShaderInput& input_desc_a,
                                              const ShaderOutput& output_desc_a)
            {
                PreProcessStructParameterAnalyzer(input_desc_a);
                PreProcessTypeConverter();
                PreProcessExprConverterPrimary();
                PreProcessGLSLConverter(input_desc_a, output_desc_a);
                PreProcessFuncNameConverter();
                PreProcessReferenceAnalyzer(input_desc_a);
                PreProcessExprConverterSecondary();
                PreProcessPackedUniforms();
            }

            void GLSLGenerator::PreProcessStructParameterAnalyzer(
                    const ShaderInput& input_desc_a)
            {
                StructParameterAnalyzer struct_analyzer_;
                struct_analyzer_.MarkStructsFromEntryPoint(
                        *GetProgram(),
                        input_desc_a.shader_target_);
            }

            void GLSLGenerator::PreProcessTypeConverter()
            {
                TypeConverter type_converter_;
                type_converter_.Convert(*GetProgram(),
                                      GLSLConverter::ConvertVarDeclType);
            }

            void GLSLGenerator::PreProcessExprConverterPrimary()
            {
                ExprConverter converter_;
                Flags converter_flags_ = ExprConverter::All;

                converter_flags_.Remove(ExprConverter::ConvertMatrixSubscripts);

                if (HasShadingLanguage420Pack())
                {
                    converter_flags_.Remove(
                            ExprConverter::ConvertVectorSubscripts);
                    converter_flags_.Remove(
                            ExprConverter::ConvertInitializerToCtor);
                }

                converter_.Convert(*GetProgram(), converter_flags_, name_mangling_);
            }

            void GLSLGenerator::PreProcessGLSLConverter(
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a)
            {
                GLSLConverter converter_;
                converter_.ConvertAST(*GetProgram(), input_desc_a, output_desc_a);
            }

            void GLSLGenerator::PreProcessFuncNameConverter()
            {
                FuncNameConverter func_name_converter_;
                func_name_converter_.Convert(
                        *GetProgram(),
                        name_mangling_,
                        [](const FunctionDecl& lhs, const FunctionDecl& rhs) {
                            return lhs.EqualsSignature(
                                    rhs,
                                    TypeDenoter::IgnoreGenericSubType);
                        },
                        FuncNameConverter::All);
            }

            void GLSLGenerator::PreProcessReferenceAnalyzer(
                    const ShaderInput& input_desc_a)
            {
                ReferenceAnalyzer ref_analyzer_;
                ref_analyzer_.MarkReferencesFromEntryPoint(
                        *GetProgram(),
                        input_desc_a.shader_target_);
            }

            void GLSLGenerator::PreProcessExprConverterSecondary()
            {
                ExprConverter converter_;
                converter_.Convert(*GetProgram(),
                                  ExprConverter::ConvertMatrixSubscripts,
                                  name_mangling_);
            }

            void GLSLGenerator::PreProcessPackedUniforms()
            {
                if (uniform_packing_.enabled_)
                {
                    UniformPacker packer_;
                    UniformPacker::CbufferAttributes attribs_;
                    {
                        attribs_.binding_slot_ = uniform_packing_.binding_slot_;
                        attribs_.name_ = uniform_packing_.buffer_name_;
                    }
                    packer_.Convert(*GetProgram(), attribs_);
                }
            }

            void GLSLGenerator::WriteComment(const std::string& text_a)
            {
                std::size_t start_ = 0, end_ = 0;

                while (end_ < text_a.size())
                {
                    end_ = text_a.find('\n', start_);
                    auto line_ = (end_ < text_a.size()
                                         ? text_a.substr(start_, end_ - start_)
                                         : text_a.substr(start_));

                    BeginLn();
                    {
                        Write("// ");
                        Write(line_);
                    }
                    EndLn();

                    start_ = end_ + 1;
                }
            }

            void GLSLGenerator::WriteLineMark(int line_number_a)
            {
                if (allow_line_marks_)
                    WriteLn("#line " + std::to_string(line_number_a));
            }

            void GLSLGenerator::WriteLineMark(const TokenPtr& tkn_a)
            {
                WriteLineMark(tkn_a->Pos().Row());
            }

            void GLSLGenerator::WriteLineMark(const AST* ast_a)
            {
                WriteLineMark(ast_a->area_.Pos().Row());
            }

            void GLSLGenerator::WriteProgramHeader()
            {
                GLSLExtensionAgent extension_agent_;
                auto required_extensions_ =
                        extension_agent_.DetermineRequiredExtensions(
                                *GetProgram(),
                                version_out_,
                                GetShaderTarget(),
                                allow_extensions_,
                                explicit_binding_,
                                separate_shaders_,
                                [this](const std::string& msg_, const AST* ast_) {
                                    if (!allow_extensions_)
                                        Error(msg_, ast_, false);
                                    else if (
                                            WarnEnabled(
                                                    Warnings::
                                                            RequiredExtensions))
                                        Warning(msg_, ast_);
                                });

                if (IsESSL())
                {
                    WriteProgramHeaderVersion();
                    WriteProgramHeaderComment();
                } 
                else
                {
                    WriteProgramHeaderComment();
                    WriteProgramHeaderVersion();
                }

                if (!required_extensions_.empty())
                {
                    for (const auto& ext_ : required_extensions_)
                        WriteProgramHeaderExtension(ext_);
                    Blank();
                }
            }

            void GLSLGenerator::WriteProgramHeaderVersion()
            {
                int version_number_ = (static_cast<int>(version_out_))
                                    & static_cast<int>(
                                            OutputShaderVersion::GLSL);

                BeginLn();
                {
                    Write("#version " + std::to_string(version_number_));

                    if (IsLanguageESSL(version_out_)
                        && version_out_ != OutputShaderVersion::ESSL100)
                        Write(" es");
                }
                EndLn();
                Blank();
            }

            void GLSLGenerator::WriteProgramHeaderComment()
            {
                if (write_header_comment_)
                {
                    if (entry_point_name_.empty())
                        WriteComment("GLSL " + ToString(GetShaderTarget()));
                    else
                        WriteComment("GLSL " + ToString(GetShaderTarget())
                                     + " \"" + entry_point_name_ + "\"");

                    WriteComment(TimePoint());

                    Blank();
                }
            }

            void GLSLGenerator::WriteProgramHeaderExtension(
                    const std::string& extension_name_a)
            {
                WriteLn("#extension " + extension_name_a + " : enable");
            }

            void GLSLGenerator::WriteGlobalLayouts()
            {
                auto program_l_ = GetProgram();

                bool layouts_written_ = false;

                switch (GetShaderTarget())
                {
                case ShaderTarget::TessellationControlShader:
                    layouts_written_ = WriteGlobalLayoutsTessControl(
                            program_l_->layout_tess_control_);
                    break;
                case ShaderTarget::TessellationEvaluationShader:
                    layouts_written_ = WriteGlobalLayoutsTessEvaluation(
                            program_l_->layoutTessEvaluation);
                    break;
                case ShaderTarget::GeometryShader:
                    layouts_written_ = WriteGlobalLayoutsGeometry(
                            program_l_->layout_geometry_);
                    break;
                case ShaderTarget::FragmentShader:
                    layouts_written_ = WriteGlobalLayoutsFragment(
                            program_l_->layout_fragment_);
                    break;
                case ShaderTarget::ComputeShader:
                    layouts_written_ = WriteGlobalLayoutsCompute(
                            program_l_->layoutCompute);
                    break;
                default:
                    break;
                }

                if (layouts_written_)
                    Blank();
            }

            bool GLSLGenerator::WriteGlobalLayoutsTessControl(
                    const Program::LayoutTessControlShader& layout_a)
            {
                WriteLayoutGlobalIn({
                        [&]() {
                            Write("vertices = "
                                  + std::to_string(layout_a.outputControlPoints));
                        },
                });
                return true;
            }

            bool GLSLGenerator::WriteGlobalLayoutsTessEvaluation(
                    const Program::LayoutTessEvaluationShader& layout_a)
            {
                WriteLayoutGlobalIn({
                        [&]() {
                            if (auto keyword_ = AttributeValueToGLSLKeyword(
                                        layout_a.domain_type_))
                                Write(*keyword_);
                            else
                                Error(R_FailedToMapToGLSLKeyword(
                                        R_DomainType,
                                        R_TessAbstractPatchType));
                        },

                        [&]() {
                            if (IsAttributeValuePartitioning(
                                        layout_a.partitioning_))
                            {
                                if (auto keyword_ = AttributeValueToGLSLKeyword(
                                            layout_a.partitioning_))
                                    Write(*keyword_);
                                else
                                    Error(R_FailedToMapToGLSLKeyword(
                                            R_Partitioning,
                                            R_TessSpacing));
                            }
                        },

                        [&]() {
                            if (IsAttributeValueTrianglePartitioning(
                                        layout_a.output_topology_))
                            {
                                if (auto keyword_ = AttributeValueToGLSLKeyword(
                                            layout_a.output_topology_))
                                    Write(*keyword_);
                                else
                                    Error(R_FailedToMapToGLSLKeyword(
                                            R_OutputToplogy,
                                            R_TessPrimitiveOrdering));
                            }
                        },
                });
                return true;
            }

            bool GLSLGenerator::WriteGlobalLayoutsGeometry(
                    const Program::LayoutGeometryShader& layout_a)
            {
                WriteLayoutGlobalIn({
                        [&]() {
                            if (layout_a.input_primitive_
                                == PrimitiveType::Undefined)
                                Error(R_MissingInputPrimitiveType(
                                        R_GeometryShader));
                            else if (auto keyword_ = PrimitiveTypeToGLSLKeyword(
                                             layout_a.input_primitive_))
                                Write(*keyword_);
                            else
                                Error(R_FailedToMapToGLSLKeyword(
                                        R_InputGeometryPrimitive));
                        },
                });

                WriteLayoutGlobalOut({
                        [&]() {
                            if (layout_a.output_primitive_ == BufferType::Undefined)
                                Error(R_MissingOutputPrimitiveType(
                                        R_GeometryShader));
                            else if (auto keyword_ = BufferTypeToGLSLKeyword(
                                             layout_a.output_primitive_))
                                Write(*keyword_);
                            else
                                Error(R_FailedToMapToGLSLKeyword(
                                        R_OutputGeometryPrimitive));
                        },

                        [&]() {
                            Write("max_vertices = "
                                  + std::to_string(layout_a.max_vertices_));
                        },
                });

                return true;
            }

            bool GLSLGenerator::WriteGlobalLayoutsFragment(
                    const Program::LayoutFragmentShader& layout_a)
            {
                bool layouts_written_ = false;
                if (!IsVKSL() && !IsESSL()
                    && GetProgram()->layout_fragment_.frag_coord_used_)
                {
                    WriteLayoutGlobalIn(
                            {
                                    [&]() { Write("origin_upper_left"); },

                                    [&]() {
                                        if (layout_a.pixel_center_integer_)
                                            Write("pixel_center_integer");
                                    },
                            },
                            [&]() { Write("vec4 gl_FragCoord"); });
                    layouts_written_ = true;
                }

                if (layout_a.early_depth_stencil_)
                {
                    WriteLayoutGlobalIn(
                            {[&]() { Write("early_fragment_tests"); }});
                    layouts_written_ = true;
                }

                return layouts_written_;
            }

            bool GLSLGenerator::WriteGlobalLayoutsCompute(
                    const Program::LayoutComputeShader& layout_a)
            {
                WriteLayoutGlobalIn({
                        [&]() {
                            Write("local_size_x = "
                                  + std::to_string(layout_a.num_threads_[0]));
                        },
                        [&]() {
                            Write("local_size_y = "
                                  + std::to_string(layout_a.num_threads_[1]));
                        },
                        [&]() {
                            Write("local_size_z = "
                                  + std::to_string(layout_a.num_threads_[2]));
                        },
                });
                return true;
            }

            void GLSLGenerator::WriteBuiltinBlockRedeclarations()
            {
                switch (GetShaderTarget())
                {
                case ShaderTarget::TessellationControlShader:
                    WriteBuiltinBlockRedeclarationsPerVertex(
                            true,
                            "gl_in[gl_MaxPatchVertices]");
                    WriteBuiltinBlockRedeclarationsPerVertex(false, "gl_out[]");
                    break;
                case ShaderTarget::TessellationEvaluationShader:
                    WriteBuiltinBlockRedeclarationsPerVertex(
                            true,
                            "gl_in[gl_MaxPatchVertices]");
                    WriteBuiltinBlockRedeclarationsPerVertex(false);
                    break;
                case ShaderTarget::GeometryShader:
                    WriteBuiltinBlockRedeclarationsPerVertex(true, "gl_in[]");
                    WriteBuiltinBlockRedeclarationsPerVertex(false);
                    break;
                case ShaderTarget::VertexShader:
                    WriteBuiltinBlockRedeclarationsPerVertex(false);
                    break;
                default:
                    break;
                }
            }

            void GLSLGenerator::WriteBuiltinBlockRedeclarationsPerVertex(
                    bool input_a,
                    const std::string& name_a)
            {
                auto entry_point_ = GetProgram()->entry_point_ref_;
                std::vector<Semantic> semantics_;

                if (input_a)
                {
                    for (const auto& param_ :
                         entry_point_->input_semantics_.var_decl_refs_sv_)
                        semantics_.push_back(param_->semantic_);
                } 
                else
                {
                    for (const auto& param_ :
                         entry_point_->output_semantics_.var_decl_refs_sv_)
                        semantics_.push_back(param_->semantic_);

                    if (IsSystemSemantic(entry_point_->semantic_))
                        semantics_.push_back(entry_point_->semantic_);
                }

                if (semantics_.empty())
                    return;

                BeginLn();
                {
                    Write(input_a ? "in" : "out");
                    Write(" gl_PerVertex");

                    WriteScopeOpen(false, name_a.empty());
                    {
                        for (const auto& semantic_ : semantics_)
                        {
                            switch (semantic_)
                            {
                            case Semantic::VertexPosition:
                                WriteLn("vec4 gl_Position;");
                                break;
                            case Semantic::PointSize:
                                WriteLn("float gl_PointSize;");
                                break;
                            case Semantic::CullDistance:
                                if (IsVKSL()
                                    || (IsGLSL()
                                        && version_out_ >= OutputShaderVersion::
                                                           GLSL450))
                                    WriteLn("float gl_CullDistance[];");
                                break;
                            case Semantic::ClipDistance:
                                WriteLn("float gl_ClipDistance[];");
                                break;
                            default:
                                break;
                            }
                        }
                    }
                    WriteScopeClose();

                    if (!name_a.empty())
                        WriteLn(name_a + ";");
                }
                EndLn();

                Blank();
            }

            void GLSLGenerator::WriteLayout(
                    const std::initializer_list<LayoutEntryFunctor>&
                            entry_functors_a)
            {
                PushWritePrefix("layout(");
                {
                    for (const auto& entry_func_ : entry_functors_a)
                    {
                        if (TopWritePrefix())
                        {
                            PushWritePrefix(", ");
                            {
                                entry_func_();
                            }
                            PopWritePrefix();
                        } 
                        else
                        {
                            entry_func_();
                        }
                    }
                }
                PopWritePrefix(") ");
            }

            void GLSLGenerator::WriteLayout(const std::string& value_a)
            {
                WriteLayout({[&]() { Write(value_a); }});
            }

            void GLSLGenerator::WriteLayoutGlobal(
                    const std::initializer_list<LayoutEntryFunctor>&
                            entry_functors_a,
                    const LayoutEntryFunctor& var_functor_a,
                    const std::string& modifier_a)
            {
                BeginLn();
                {
                    WriteLayout(entry_functors_a);
                    if (var_functor_a)
                    {
                        Write(modifier_a + ' ');
                        var_functor_a();
                        Write(";");
                    } else
                        Write(modifier_a + ';');
                }
                EndLn();
            }

            void GLSLGenerator::WriteLayoutGlobalIn(
                    const std::initializer_list<LayoutEntryFunctor>&
                            entry_functors_a,
                    const LayoutEntryFunctor& var_functor_a)
            {
                WriteLayoutGlobal(entry_functors_a, var_functor_a, "in");
            }

            void GLSLGenerator::WriteLayoutGlobalOut(
                    const std::initializer_list<LayoutEntryFunctor>&
                            entry_functors_a,
                    const LayoutEntryFunctor& var_functor_a)
            {
                WriteLayoutGlobal(entry_functors_a, var_functor_a, "out");
            }

            void GLSLGenerator::WriteLayoutBinding(
                    const std::vector<RegisterPtr>& slot_registers_a)
            {
                if (explicit_binding_
                    && (!IsESSL()
                        || version_out_ >= OutputShaderVersion::ESSL310))
                {
                    if (auto slot_register_ =
                                Register::GetForTarget(slot_registers_a,
                                                       GetShaderTarget()))
                        Write("binding = "
                              + std::to_string(slot_register_->slot_));
                }
            }

            void GLSLGenerator::WriteLayoutBindingOrLocation(
                    const std::vector<RegisterPtr>& slot_registers_a)
            {
                if (explicit_binding_
                    && (!IsESSL()
                        || version_out_ >= OutputShaderVersion::ESSL310))
                {
                    if (auto slot_register_ =
                                Register::GetForTarget(slot_registers_a,
                                                       GetShaderTarget()))
                    {
                        switch (slot_register_->register_type_)
                        {
                        case RegisterType::BufferOffset:
                            Write("location = "
                                  + std::to_string(slot_register_->slot_));
                            break;
                        case RegisterType::UnorderedAccessView:
                        case RegisterType::ConstantBuffer:
                        case RegisterType::TextureBuffer:
                        case RegisterType::Sampler:
                            Write("binding = "
                                  + std::to_string(slot_register_->slot_));
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

            void GLSLGenerator::WriteLocalInputSemantics(
                    FunctionDecl* entry_point_a)
            {
                entry_point_a->input_semantics_.ForEach([this](VarDecl* var_decl_) {
                    if (var_decl_->flags_(Decl::IsWrittenTo))
                        WriteLocalInputSemanticsVarDecl(var_decl_);
                });

                for (auto& param_ : entry_point_a->parameters_)
                {
                    const auto& type_den_ = param_->type_specifier_->GetTypeDenoter()
                                                  ->GetAliased();
                    if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                        WriteLocalInputSemanticsStructDeclParam(
                                param_.get(),
                                struct_type_den_->struct_decl_ref_);
                }
            }

            void GLSLGenerator::WriteLocalInputSemanticsVarDecl(
                    VarDecl* var_decl_a)
            {
                auto semantic_keyword_ = SystemValueToKeyword(var_decl_a->semantic_);
                if (!semantic_keyword_)
                {
                    semantic_keyword_ = MakeUnique<std::string>(var_decl_a->ident_);
                    var_decl_a->ident_.AppendPrefix(name_mangling_.temporary_prefix_);
                }

                BeginLn();
                {
                    auto type_specifier_ =
                            var_decl_a->decl_stmnt_ref_->type_specifier_.get();

                    Visit(type_specifier_);
                    Write(" " + var_decl_a->ident_ + " = ");

                    if (!IsTypeCompatibleWithSemantic(
                                var_decl_a->semantic_,
                                type_specifier_->type_denoter_->GetAliased()))
                    {
                        Visit(type_specifier_);
                        Write("(" + *semantic_keyword_ + ");");
                    } 
                    else
                    {
                        Write(*semantic_keyword_ + ";");
                    }
                }
                EndLn();
            }

            void GLSLGenerator::WriteLocalInputSemanticsStructDeclParam(
                    VarDeclStmnt* param_a,
                    StructDecl* struct_decl_a)
            {
                if (struct_decl_a
                    && struct_decl_a->flags_(StructDecl::isNonEntryPointParam)
                    && struct_decl_a->flags_(StructDecl::IsShaderInput))
                {
                    Visit(param_a);

                    BeginSep();
                    {
                        auto param_var_ = param_a->var_decls_.front().get();

                        if (param_var_->array_dims_.empty())
                        {
                            struct_decl_a->ForEachVarDecl(
                                    [&](VarDeclPtr& var_decl_) {
                                        BeginLn();
                                        {
                                            Separator();
                                            Write(param_var_->ident_ + "."
                                                  + var_decl_->ident_.Original());
                                            Separator();
                                            Write(" = ");
                                            WriteVarDeclIdentOrSystemValue(
                                                    var_decl_.get());
                                            Write(";");
                                        }
                                        EndLn();
                                    });
                        } 
                        else if (param_var_->array_dims_.size() == 1)
                        {
                            auto array_size_ = param_var_->array_dims_.front()->size_;
                            for (int i_ = 0; i_ < array_size_; ++i_)
                            {
                                struct_decl_a->ForEachVarDecl(
                                        [&](VarDeclPtr& var_decl_) {
                                            BeginLn();
                                            {
                                                Separator();
                                                Write(param_var_->ident_ + "["
                                                      + std::to_string(i_) + "]."
                                                      + var_decl_->ident_
                                                                .Original());
                                                Separator();
                                                Write(" = ");
                                                WriteVarDeclIdentOrSystemValue(
                                                        var_decl_.get(),
                                                        i_);
                                                Write(";");
                                            }
                                            EndLn();
                                        });
                            }
                        } 
                        else
                            Error(R_TooManyIndicesForShaderInputParam,
                                  param_var_);
                    }
                    EndSep();
                }
            }

            void GLSLGenerator::WriteGlobalInputSemantics(
                    FunctionDecl* entry_point_a)
            {
                auto& var_decl_refs_ = entry_point_a->input_semantics_.var_decl_refs_;

                for (auto var_decl_ : var_decl_refs_)
                    WriteGlobalInputSemanticsVarDecl(var_decl_);

                if (!var_decl_refs_.empty())
                    Blank();
            }

            void GLSLGenerator::WriteGlobalInputSemanticsVarDecl(
                    VarDecl* var_decl_a)
            {
                BeginLn();
                {
                    const auto& interp_modifiers_ =
                            var_decl_a->decl_stmnt_ref_->type_specifier_
                                    ->interp_modifiers_;

                    if (IsGLSL120OrESSL100())
                    {
                        if (WarnEnabled(Warnings::Basic)
                            && !interp_modifiers_.empty())
                            Warning(R_InterpModNotSupportedForGLSL120, var_decl_a);

                        if (IsVertexShader())
                            Write("attribute ");
                        else
                            Write("varying ");
                        Separator();
                    } 
                    else
                    {
                        WriteInterpModifiers(interp_modifiers_,
                                             var_decl_a->decl_stmnt_ref_);
                        Separator();

                        if ((!IsESSL() && explicit_binding_)
                            || (IsESSL() && IsVertexShader()))
                        {
                            int location_ = -1;

                            if (IsVertexShader() && var_decl_a->semantic_.IsValid())
                            {
                                auto it_ = vertex_semantics_map_.find(ToCiString(
                                        var_decl_a->semantic_.ToString()));
                                if (it_ != vertex_semantics_map_.end())
                                {
                                    location_ = it_->second.location_;
                                    it_->second.found_ = true;
                                }
                            }

                            if (location_ == -1 && auto_binding_)
                                location_ = GetBindingLocation(
                                        var_decl_a->GetTypeDenoter().get(),
                                        true);

                            if (location_ != -1)
                            {
                                WriteLayout({[&]() {
                                    Write("location = "
                                          + std::to_string(location_));
                                }});

                                var_decl_a->semantic_.ResetIndex(location_);
                            }
                        }

                        Separator();
                        Write("in ");
                        Separator();
                    }

                    Visit(var_decl_a->decl_stmnt_ref_->type_specifier_);
                    Separator();

                    Write(" " + var_decl_a->ident_);

                    if (var_decl_a->flags_(VarDecl::IsDynamicArray))
                        Write("[]");
                    else
                        Visit(var_decl_a->array_dims_);

                    Write(";");
                }
                EndLn();
            }

            void GLSLGenerator::WriteLocalOutputSemantics(
                    FunctionDecl* entry_point_a)
            {
                for (const auto& param_ : entry_point_a->parameters_)
                {
                    const auto& type_den_ = param_->type_specifier_->GetTypeDenoter()
                                                  ->GetAliased();
                    if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                        WriteLocalOutputSemanticsStructDeclParam(
                                param_.get(),
                                struct_type_den_->struct_decl_ref_);
                }
            }

            void GLSLGenerator::WriteLocalOutputSemanticsStructDeclParam(
                    VarDeclStmnt* param_a,
                    StructDecl* struct_decl_a)
            {
                if (struct_decl_a
                    && struct_decl_a->flags_(StructDecl::isNonEntryPointParam)
                    && struct_decl_a->flags_(StructDecl::IsShaderOutput))
                {
                    Visit(param_a);
                }
            }

            void GLSLGenerator::WriteGlobalOutputSemantics(
                    FunctionDecl* entry_point_a)
            {
                auto& var_decl_refs_ = entry_point_a->output_semantics_.var_decl_refs_;
                bool params_written_ = (!var_decl_refs_.empty());

                for (auto var_decl_ : var_decl_refs_)
                    WriteGlobalOutputSemanticsVarDecl(var_decl_);

                if (IsFragmentShader()
                    && version_out_ > OutputShaderVersion::GLSL120)
                {
                    auto& var_decl_refs_l_ =
                            entry_point_a->output_semantics_.var_decl_refs_sv_;

                    for (auto var_decl_ : var_decl_refs_l_)
                    {
                        if (var_decl_->semantic_ == Semantic::Target)
                        {
                            WriteGlobalOutputSemanticsVarDecl(var_decl_, true);
                            params_written_ = true;
                        }
                    }

                    if (entry_point_a->semantic_ == Semantic::Target)
                    {
                        WriteGlobalOutputSemanticsSlot(
                                entry_point_a->return_type_.get(),
                                entry_point_a->semantic_,
                                entry_point_a->semantic_.ToString());
                        params_written_ = true;
                    }
                }

                if (entry_point_a->semantic_.IsUserDefined())
                {
                    WriteGlobalOutputSemanticsSlot(
                            entry_point_a->return_type_.get(),
                            entry_point_a->semantic_,
                            name_mangling_.output_prefix_
                                    + entry_point_a->semantic_.ToString());
                    params_written_ = true;
                }

                if (params_written_)
                    Blank();
            }

            void GLSLGenerator::WriteGlobalOutputSemanticsVarDecl(
                    VarDecl* var_decl_a,
                    bool use_semantic_name_a)
            {
                WriteGlobalOutputSemanticsSlot(
                        var_decl_a->decl_stmnt_ref_->type_specifier_.get(),
                        var_decl_a->semantic_,
                        (use_semantic_name_a ? var_decl_a->semantic_.ToString()
                                         : var_decl_a->ident_.Final()),
                        var_decl_a);
            }

            void GLSLGenerator::WriteGlobalOutputSemanticsSlot(
                    TypeSpecifier* type_specifier_a,
                    IndexedSemantic& semantic_a,
                    const std::string& ident_a,
                    VarDecl* var_decl_a)
            {
                BeginLn();
                {
                    VarDeclStmnt* var_decl_stmnt_ =
                            (var_decl_a != nullptr ? var_decl_a->decl_stmnt_ref_
                                                : nullptr);

                    if (IsGLSL120OrESSL100())
                    {
                        if (WarnEnabled(Warnings::Basic) && var_decl_stmnt_
                            && !var_decl_stmnt_->type_specifier_->interp_modifiers_
                                        .empty())
                            Warning(R_InterpModNotSupportedForGLSL120, var_decl_a);

                        Write("varying ");
                        Separator();
                    } 
                    else
                    {
                        if (var_decl_stmnt_)
                            WriteInterpModifiers(var_decl_stmnt_->type_specifier_
                                                         ->interp_modifiers_,
                                                 var_decl_a);
                        Separator();

                        if ((!IsESSL() && explicit_binding_)
                            || (IsESSL() && IsFragmentShader()))
                        {
                            int location_ = -1;

                            if (IsFragmentShader())
                                location_ = semantic_a.Index();
                            else if (auto_binding_)
                                location_ = GetBindingLocation(
                                        type_specifier_a->type_denoter_.get(),
                                        false);

                            if (location_ != -1)
                            {
                                WriteLayout({[&]() {
                                    Write("location = "
                                          + std::to_string(location_));
                                }});

                                semantic_a.ResetIndex(location_);
                            }
                        }

                        Write("out ");
                        Separator();
                    }

                    Visit(type_specifier_a);
                    Separator();

                    Write(" " + ident_a);

                    if (var_decl_a != nullptr)
                    {
                        if (var_decl_a->flags_(VarDecl::IsDynamicArray))
                            Write("[]");
                        else
                            Visit(var_decl_a->array_dims_);
                    }

                    Write(";");
                }
                EndLn();
            }

            void GLSLGenerator::WriteOutputSemanticsAssignment(
                    Expr* expr_a,
                    bool write_as_listed_expr_a)
            {
                auto entry_point_ = GetProgram()->entry_point_ref_;
                const ObjectExpr* lvalue_expr_ = nullptr;
                if (expr_a)
                    lvalue_expr_ = expr_a->FetchLValueExpr();

                for (const auto& param_struct_ : entry_point_->param_structs_)
                {
                    if (param_struct_.expr_ == nullptr || param_struct_.expr_ == expr_a)
                        WriteOutputSemanticsAssignmentStructDeclParam(
                                param_struct_,
                                write_as_listed_expr_a);
                }

                auto semantic_ = entry_point_->semantic_;
                if (expr_a)
                {
                    if (semantic_.IsValid())
                    {
                        if (semantic_.IsSystemValue())
                        {
                            if (auto semantic_keyword_ = SystemValueToKeyword(
                                        semantic_))
                            {
                                BeginLn();
                                {
                                    Write(*semantic_keyword_);
                                    Write(" = ");
                                    Visit(expr_a);
                                    Write(";");
                                }
                                EndLn();
                            } 
                            else
                                Error(R_FailedToMapToGLSLKeyword(
                                              R_OutputSemantic),
                                      entry_point_);
                        } 
                        else if (semantic_.IsUserDefined())
                        {
                            BeginLn();
                            {
                                Write(name_mangling_.output_prefix_
                                      + semantic_.ToString());
                                Write(" = ");
                                Visit(expr_a);
                                Write(";");
                            }
                            EndLn();
                        }
                    }
                }
            }

            void GLSLGenerator::WriteOutputSemanticsAssignmentStructDeclParam(
                    const FunctionDecl::ParameterStructure& param_struct_a,
                    bool write_as_listed_expr_a,
                    const std::string& temp_ident_a)
            {
                auto param_expr_ = param_struct_a.expr_;
                auto param_var_ = param_struct_a.var_decl_;
                auto struct_decl_ = param_struct_a.struct_decl_;

                if (struct_decl_
                    && struct_decl_->flags_(StructDecl::isNonEntryPointParam)
                    && struct_decl_->flags_(StructDecl::IsShaderOutput))
                {
                    struct_decl_->ForEachVarDecl([&](VarDeclPtr& var_decl_) 
                    {
                        auto open_line_ = IsOpenLine();
                        if (!write_as_listed_expr_a && !open_line_)
                            BeginLn();

                        if (auto semantic_keyword_ = SystemValueToKeyword(
                                    var_decl_->semantic_))
                            Write(*semantic_keyword_);
                        else
                            Write(var_decl_->ident_);

                        Write(" = ");

                        if (param_expr_)
                            Visit(param_expr_);
                        else if (param_var_)
                            Write(param_var_->ident_);
                        else
                            Write(temp_ident_a);

                        Write("." + var_decl_->ident_.Original()
                              + (write_as_listed_expr_a ? ", " : ";"));

                        if (!write_as_listed_expr_a)
                        {
                            EndLn();
                            if (open_line_)
                                BeginLn();
                        }
                    });
                }
            }

            void GLSLGenerator::WriteGlobalUniforms()
            {
                bool uniforms_written_ = false;

                for (auto& param_ : GetProgram()->entry_point_ref_->parameters_)
                {
                    if (param_->IsUniform())
                    {
                        WriteGlobalUniformsParameter(param_.get());
                        uniforms_written_ = true;
                    }
                }

                if (uniforms_written_)
                    Blank();
            }

            void GLSLGenerator::WriteGlobalUniformsParameter(
                    VarDeclStmnt* param_a)
            {
                BeginLn();
                {
                    Write("uniform ");
                    Visit(param_a->type_specifier_);
                    Write(" ");

                    if (param_a->var_decls_.size() == 1)
                        Visit(param_a->var_decls_.front());
                    else
                        Error(R_InvalidParamVarCount, param_a);

                    Write(";");
                }
                EndLn();
            }

            void GLSLGenerator::WriteVarDeclIdentOrSystemValue(VarDecl* var_decl_a,
                                                               int array_index_a)
            {
                if (auto semantic_keyword_ = SystemValueToKeyword(
                            var_decl_a->semantic_))
                {
                    if (array_index_a >= 0)
                    {
                        if (var_decl_a->flags_(VarDecl::IsShaderInput))
                            Write("gl_in");
                        else
                            Write("gl_out");
                        Write("[" + std::to_string(array_index_a) + "].");
                    }
                    Write(*semantic_keyword_);
                } 
                else
                {
                    Write(var_decl_a->ident_);
                    if (array_index_a >= 0)
                        Write("[" + std::to_string(array_index_a) + "]");
                }
            }

            void GLSLGenerator::WriteObjectExpr(const ObjectExpr& object_expr_a)
            {
                if (object_expr_a.flags_(ObjectExpr::IsImmutable))
                    WriteObjectExprIdent(object_expr_a);
                else if (auto symbol_ = object_expr_a.symbol_ref_)
                    WriteObjectExprIdentOrSystemValue(object_expr_a, symbol_);
                else
                    WriteObjectExprIdent(object_expr_a);
            }

            void GLSLGenerator::WriteObjectExprIdent(
                    const ObjectExpr& object_expr_a,
                    bool write_prefix_a)
            {
                if (object_expr_a.prefix_expr_ && !object_expr_a.is_static_
                    && write_prefix_a)
                {
                    Visit(object_expr_a.prefix_expr_);

                    if (auto literal_expr_ =
                                object_expr_a.prefix_expr_->As<LiteralExpr>())
                    {
                        if (literal_expr_->IsSpaceRequiredForSubscript())
                            Write(" ");
                    }

                    Write(".");
                }

                if (auto symbol_ = object_expr_a.symbol_ref_)
                {
                    if (object_expr_a.flags_(ObjectExpr::IsImmutable))
                        Write(symbol_->ident_.Original());
                    else
                        Write(symbol_->ident_);
                } else
                    Write(object_expr_a.ident_);
            }

            void GLSLGenerator::WriteObjectExprIdentOrSystemValue(
                    const ObjectExpr& object_expr_a,
                    Decl* symbol_a)
            {
                std::unique_ptr<std::string> semantic_keyword_;
                Flags var_flags_;

                if (auto var_decl_ = symbol_a->As<VarDecl>())
                {
                    var_flags_ = var_decl_->flags_;
                    if (GetProgram()->entry_point_ref_->output_semantics_.Contains(
                                var_decl_)
                        || !var_decl_->flags_(Decl::IsWrittenTo))
                    {
                        semantic_keyword_ = SystemValueToKeyword(
                                var_decl_->semantic_);
                    }
                }

                if (var_flags_(VarDecl::IsShaderInput | VarDecl::IsShaderOutput)
                    && object_expr_a.prefix_expr_)
                {
                    if (auto array_expr_ =
                                object_expr_a.prefix_expr_
                                        ->FindFirstNotOf(
                                                AST::Types::BracketExpr)
                                        ->As<ArrayExpr>())
                    {
                        if (semantic_keyword_)
                        {
                            if (var_flags_(VarDecl::IsShaderInput))
                                Write("gl_in");
                            else
                                Write("gl_out");
                            WriteArrayIndices(array_expr_->array_indices_);
                            Write("." + *semantic_keyword_);
                        } 
                        else
                        {
                            WriteObjectExprIdent(object_expr_a, false);
                            WriteArrayIndices(array_expr_->array_indices_);
                        }
                    } 
                    else
                        Error(R_MissingArrayPrefixForIOSemantic(
                                      object_expr_a.ident_),
                              &object_expr_a);
                } 
                else if (semantic_keyword_)
                {
                    Write(*semantic_keyword_);
                } 
                else
                {
                    WriteObjectExprIdent(object_expr_a);
                }
            }

            void GLSLGenerator::WriteArrayExpr(const ArrayExpr& array_expr_a)
            {
                Visit(array_expr_a.prefix_expr_);
                WriteArrayIndices(array_expr_a.array_indices_);
            }

            void GLSLGenerator::WriteArrayIndices(
                    const std::vector<ExprPtr>& array_indices_a)
            {
                for (auto& array_index_ : array_indices_a)
                {
                    Write("[");
                    Visit(array_index_);
                    Write("]");
                }
            }

            void GLSLGenerator::WriteStorageClasses(
                    const std::set<StorageClass>& storage_ñlasses_a,
                    const AST* ast_a)
            {
                for (auto storage_ : storage_ñlasses_a)
                {
                    if (storage_ != StorageClass::Static)
                    {
                        if (auto keyword_ = StorageClassToGLSLKeyword(storage_))
                            Write(*keyword_ + " ");
                        else if (WarnEnabled(Warnings::Basic))
                            Warning(R_NotAllStorageClassesMappedToGLSL, ast_a);
                    }
                }
            }

            void GLSLGenerator::WriteInterpModifiers(
                    const std::set<InterpModifier>& interp_modifiers_a,
                    const AST* ast_a)
            {
                for (auto modifier_ : interp_modifiers_a)
                {
                    if (auto keyword_ = InterpModifierToGLSLKeyword(modifier_))
                        Write(*keyword_ + " ");
                    else if (WarnEnabled(Warnings::Basic))
                        Warning(R_NotAllInterpModMappedToGLSL, ast_a);
                }
            }

            void GLSLGenerator::WriteTypeModifiers(
                    const std::set<TypeModifier>& type_modifiers_a,
                    const TypeDenoterPtr& type_denoter_a)
            {
                if (InsideUniformBufferDecl() && type_denoter_a
                    && type_denoter_a->IsMatrix())
                {
                    const auto common_storage_layout_ =
                            GetUniformBufferDeclStack()
                                    .back()
                                    ->common_storage_layout_;

                    if (common_storage_layout_ == TypeModifier::ColumnMajor)
                    {
                        if (type_modifiers_a.find(TypeModifier::RowMajor)
                            != type_modifiers_a.end())
                            WriteLayout("row_major");
                    } 
                    else
                    {
                        if (type_modifiers_a.find(TypeModifier::ColumnMajor)
                            != type_modifiers_a.end())
                            WriteLayout("column_major");
                    }
                }

                if (type_modifiers_a.find(TypeModifier::Const)
                    != type_modifiers_a.end())
                {
                    if ((IsGLSL()
                         && version_out_ >= OutputShaderVersion::GLSL420)
                        || IsVKSL())
                        Write("const ");
                }
            }

            void GLSLGenerator::WriteTypeModifiersFrom(
                    const TypeSpecifierPtr& type_specifier_a)
            {
                WriteTypeModifiers(type_specifier_a->type_modifiers_,
                                   type_specifier_a->GetTypeDenoter()->GetSub());
            }

            void GLSLGenerator::WriteDataType(DataType data_type_a,
                                              bool write_precision_specifier_a,
                                              const AST* ast_a)
            {
                if (version_out_ < OutputShaderVersion::GLSL400)
                    data_type_a = DoubleToFloatDataType(data_type_a);

                if (write_precision_specifier_a)
                {
                    if (IsHalfRealType(data_type_a))
                        Write("mediump ");
                    else
                        Write("highp ");
                }

                if (auto keyword_ = DataTypeToGLSLKeyword(data_type_a))
                    Write(*keyword_);
                else
                    Error(R_FailedToMapToGLSLKeyword(R_DataType), ast_a);
            }

            void GLSLGenerator::WriteTypeDenoter(const TypeDenoter& type_denoter_a,
                                                 bool write_precision_specifier_a,
                                                 const AST* ast_a)
            {
                try
                {
                    if (type_denoter_a.IsVoid())
                    {
                        Write("void");
                    } 
                    else if (auto base_type_den_ =
                                       type_denoter_a.As<BaseTypeDenoter>())
                    {
                        WriteDataType(base_type_den_->data_type_,
                                      write_precision_specifier_a,
                                      ast_a);
                    } 
                    else if (auto buffer_type_den_ =
                                       type_denoter_a.As<BufferTypeDenoter>())
                    {
                        auto buffer_type_ = buffer_type_den_->buffer_type_;
                        if (buffer_type_ == BufferType::Undefined)
                        {
                            if (auto buffer_decl_ = buffer_type_den_->buffer_decl_ref_)
                                buffer_type_ = buffer_decl_->GetBufferType();
                            else
                                Error(R_MissingRefInTypeDen(R_BufferTypeDen),
                                      ast_a);
                        }

                        if (auto keyword_ = BufferTypeToKeyword(buffer_type_, ast_a))
                            Write(*keyword_);
                    } 
                    else if (auto sampler_type_den_ =
                                       type_denoter_a.As<SamplerTypeDenoter>())
                    {
                        auto sampler_type_ = sampler_type_den_->sampler_type_;
                        if (sampler_type_ == SamplerType::Undefined)
                        {
                            if (auto sampler_decl_ =
                                        sampler_type_den_->sampler_decl_ref_)
                                sampler_type_ = sampler_decl_->GetSamplerType();
                            else
                                Error(R_MissingRefInTypeDen(R_SamplerTypeDen),
                                      ast_a);
                        }

                        if (!IsSamplerStateType(sampler_type_)
                            || UseSeparateSamplers())
                        {
                            if (auto keyword_ = SamplerTypeToKeyword(sampler_type_,
                                                                    ast_a))
                                Write(*keyword_);
                        } 
                        else
                            Error(R_CantTranslateSamplerToGLSL, ast_a);
                    } 
                    else if (auto struct_type_den_ =
                                       type_denoter_a.As<StructTypeDenoter>())
                    {
                        if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                            Write(struct_decl_->ident_);
                        else
                            Write(type_denoter_a.Ident());
                    } 
                    else if (type_denoter_a.IsAlias())
                    {
                        WriteTypeDenoter(type_denoter_a.GetAliased(),
                                         write_precision_specifier_a,
                                         ast_a);
                    } 
                    else if (auto array_type_den_ =
                                       type_denoter_a.As<ArrayTypeDenoter>())
                    {
                        WriteTypeDenoter(*array_type_den_->sub_type_denoter_,
                                         write_precision_specifier_a,
                                         ast_a);
                        Visit(array_type_den_->array_dims_);
                    }
                    else
                        Error(R_FailedToDetermineGLSLDataType, ast_a);
                } 
                catch (const Report&)
                {
                    throw;
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
            }

            void GLSLGenerator::WriteFunction(FunctionDecl* ast_a)
            {
                if (auto struct_decl_ = ast_a->return_type_->struct_decl_.get())
                {
                    StructDeclArgs struct_decl_args_;
                    struct_decl_args_.in_end_with_semicolon_ = true;

                    Visit(struct_decl_, &struct_decl_args_);

                    BeginLn();
                    Write(struct_decl_->ident_ + " " + ast_a->ident_ + "(");
                } 
                else
                {
                    BeginLn();
                    Visit(ast_a->return_type_);
                    Write(" " + ast_a->ident_ + "(");
                }

                for (std::size_t i_ = 0; i_ < ast_a->parameters_.size(); ++i_)
                {
                    WriteParameter(ast_a->parameters_[i_].get());
                    if (i_ + 1 < ast_a->parameters_.size())
                        Write(", ");
                }

                Write(")");

                if (ast_a->code_block_)
                {
                    Visit(ast_a->code_block_);
                } 
                else
                {
                    Write(";");
                    EndLn();
                }
            }

            void GLSLGenerator::WriteFunctionEntryPoint(FunctionDecl* ast_a)
            {
                if (ast_a->IsForwardDecl())
                    return;

                BeginLn();
                Write("void main()");

                WriteScopeOpen();
                {
                    if (IsTessControlShader())
                    {
                        if (auto patch_const_func_ =
                                    GetProgram()
                                            ->layout_tess_control_
                                            .patch_const_function_ref_)
                        {
                            WriteLn("if (gl_InvocationID == 0)");
                            IncIndent();
                            {
                                WriteLn(patch_const_func_->ident_ + "();");
                            }
                            DecIndent();
                            Blank();
                        }
                    }

                    WriteFunctionEntryPointBody(ast_a);
                }
                WriteScopeClose();
            }

            void GLSLGenerator::WriteFunctionEntryPointBody(FunctionDecl* ast_a)
            {
                WriteLocalInputSemantics(ast_a);
                WriteLocalOutputSemantics(ast_a);

                WriteStmntList(ast_a->code_block_->stmnts_);

                if (ast_a->HasVoidReturnType()
                    && (ast_a->code_block_->stmnts_.empty()
                        || ast_a->code_block_->stmnts_.back()->Type()
                                   != AST::Types::ReturnStmnt))
                {
                    WriteOutputSemanticsAssignment(nullptr);
                }
            }

            void GLSLGenerator::WriteFunctionSecondaryEntryPoint(
                    FunctionDecl* ast_a)
            {
                if (ast_a->IsForwardDecl())
                    return;

                BeginLn();
                Write("void " + ast_a->ident_ + "()");

                WriteScopeOpen();
                {
                    WriteFunctionEntryPointBody(ast_a);
                }
                WriteScopeClose();
            }

            void GLSLGenerator::AssertIntrinsicNumArgs(CallExpr* func_call_a,
                                                       std::size_t num_args_min_a,
                                                       std::size_t num_args_max_a)
            {
                auto num_args_ = func_call_a->arguments_.size();
                if (num_args_ < num_args_min_a || num_args_ > num_args_max_a)
                    Error(R_InvalidIntrinsicArgCount(func_call_a->ident_),
                          func_call_a);
            }

            void GLSLGenerator::WriteCallExprStandard(CallExpr* func_call_a)
            {
                if (func_call_a->intrinsic_ != Intrinsic::Undefined)
                {
                    if (!IsWrappedIntrinsic(func_call_a->intrinsic_))
                    {
                        if (auto keyword_ = IntrinsicToGLSLKeyword(
                                    func_call_a->intrinsic_,
                                    IsGLSL120OrESSL100()))
                            Write(*keyword_);
                        else
                            ErrorIntrinsic(func_call_a->ident_, func_call_a);
                    } 
                    else if (!func_call_a->ident_.empty())
                    {
                        Write(func_call_a->ident_);
                    } 
                    else
                        Error(R_MissingFuncName, func_call_a);
                } 
                else if (auto func_decl_ = func_call_a->GetFunctionImpl())
                {
                    Write(func_decl_->ident_);
                } 
                else if (func_call_a->flags_(CallExpr::IsWrapperCall))
                {
                    Write(func_call_a->ident_);
                } 
                else if (func_call_a->type_denoter_)
                {
                    WriteTypeDenoter(*func_call_a->type_denoter_, false, func_call_a);
                } 
                else
                    Error(R_MissingFuncName, func_call_a);

                Write("(");
                WriteCallExprArguments(func_call_a);
                Write(")");
            }

            void GLSLGenerator::WriteCallExprIntrinsicMul(CallExpr* func_ñall_a)
            {
                AssertIntrinsicNumArgs(func_ñall_a, 2, 2);

                auto WriteMulArgument = [&](const ExprPtr& expr_) 
                {
                    auto type_ = expr_->Type();
                    if (type_ == AST::Types::TernaryExpr
                        || type_ == AST::Types::BinaryExpr
                        || type_ == AST::Types::UnaryExpr
                        || type_ == AST::Types::PostUnaryExpr)
                    {
                        Write("(");
                        Visit(expr_);
                        Write(")");
                    } 
                    else
                        Visit(expr_);
                };

                Write("(");
                {
                    WriteMulArgument(func_ñall_a->arguments_[1]);
                    Write(" * ");
                    WriteMulArgument(func_ñall_a->arguments_[0]);
                }
                Write(")");
            }

            void GLSLGenerator::WriteCallExprIntrinsicRcp(CallExpr* func_call_a)
            {
                AssertIntrinsicNumArgs(func_call_a, 1, 1);

                auto& expr_ = func_call_a->arguments_.front();
                const auto& type_den_ = expr_->GetTypeDenoter()->GetAliased();

                if (auto base_type_den_ = type_den_.As<BaseTypeDenoter>())
                {
                    Write("(");
                    {
                        WriteTypeDenoter(*base_type_den_, false, func_call_a);
                        Write("(");
                        WriteLiteral("1", base_type_den_->data_type_, func_call_a);
                        Write(") / (");
                        Visit(expr_);
                    }
                    Write("))");
                } 
                else
                    Error(R_InvalidIntrinsicArgType("rcp"), expr_.get());
            }

            void GLSLGenerator::WriteCallExprIntrinsicClip(CallExpr* func_call_a)
            {
                AssertIntrinsicNumArgs(func_call_a, 1, 1);
                auto& expr_ = func_call_a->arguments_.front();
                const auto& type_den_ = expr_->GetTypeDenoter()->GetAliased();

                if (auto base_type_den_ = type_den_.As<BaseTypeDenoter>())
                {
                    Write("if (");

                    if (base_type_den_->IsVector())
                    {
                        Write("any(lessThan(");

                        auto binary_expr_ = expr_->As<BinaryExpr>();

                        if (binary_expr_ && binary_expr_->op_ == BinaryOp::Sub)
                        {
                            Visit(binary_expr_->lhs_expr_);
                            Write(", ");
                            Visit(binary_expr_->rhs_expr_);
                        } 
                        else
                        {
                            Visit(expr_);
                            Write(", ");
                            WriteLiteral("0",
                                         base_type_den_->data_type_,
                                         expr_.get());
                        }

                        Write("))");
                    }
                    else if (base_type_den_->IsScalar())
                    {
                        auto binary_expr_ = expr_->As<BinaryExpr>();
                        if (binary_expr_ && binary_expr_->op_ == BinaryOp::Sub)
                        {
                            Visit(binary_expr_->lhs_expr_);
                            Write(" < ");
                            Visit(binary_expr_->rhs_expr_);
                        } 
                        else
                        {
                            Visit(expr_);
                            Write(" < ");
                            WriteLiteral("0",
                                         base_type_den_->data_type_,
                                         expr_.get());
                        }
                    } 
                    else
                        Error(R_InvalidIntrinsicArgType("clip"), expr_.get());
                } 
                else
                    Error(R_InvalidIntrinsicArgType("clip"), expr_.get());

                Write(")");

                EndLn();
                IncIndent();
                BeginLn();
                Write("discard");
                DecIndent();
            }

            void GLSLGenerator::WriteCallExprIntrinsicAtomic(CallExpr* call_expr_a)
            {
                AssertIntrinsicNumArgs(call_expr_a, 2, 3);
                if (auto keyword_ = IntrinsicToGLSLKeyword(call_expr_a->intrinsic_))
                {
                    if (call_expr_a->arguments_.size() >= 3)
                    {
                        Visit(call_expr_a->arguments_[2]);
                        Write(" = ");
                    }
                    Write(*keyword_ + "(");
                    WriteCallExprArguments(call_expr_a, 0, 2);
                    Write(")");
                } 
                else
                    ErrorIntrinsic(call_expr_a->ident_, call_expr_a);
            }

            void GLSLGenerator::WriteCallExprIntrinsicAtomicCompSwap(
                    CallExpr* call_expr_a)
            {
                AssertIntrinsicNumArgs(call_expr_a, 4, 4);
                if (auto keyword_ = IntrinsicToGLSLKeyword(call_expr_a->intrinsic_))
                {
                    Visit(call_expr_a->arguments_[3]);
                    Write(" = " + *keyword_ + "(");
                    WriteCallExprArguments(call_expr_a, 0, 3);
                    Write(")");
                } 
                else
                    ErrorIntrinsic(call_expr_a->ident_, call_expr_a);
            }

            void GLSLGenerator::WriteCallExprIntrinsicImageAtomic(
                    CallExpr* call_expr_a)
            {
                AssertIntrinsicNumArgs(call_expr_a, 3, 4);
                if (auto keyword_ = IntrinsicToGLSLKeyword(call_expr_a->intrinsic_))
                {
                    if (call_expr_a->arguments_.size() >= 4)
                    {
                        Visit(call_expr_a->arguments_[3]);
                        Write(" = ");
                    }

                    Write(*keyword_ + "(");
                    WriteCallExprArguments(call_expr_a, 0, 3);
                    Write(")");
                } 
                else
                    ErrorIntrinsic(call_expr_a->ident_, call_expr_a);
            }

            void GLSLGenerator::WriteCallExprIntrinsicImageAtomicCompSwap(
                    CallExpr* call_expr_a)
            {
                AssertIntrinsicNumArgs(call_expr_a, 5, 5);
                if (auto keyword_ = IntrinsicToGLSLKeyword(call_expr_a->intrinsic_))
                {
                    Visit(call_expr_a->arguments_[4]);
                    Write(" = " + *keyword_ + "(");
                    WriteCallExprArguments(call_expr_a, 0, 4);
                    Write(")");
                } 
                else
                    ErrorIntrinsic(call_expr_a->ident_, call_expr_a);
            }

            void GLSLGenerator::WriteCallExprIntrinsicStreamOutputAppend(
                    CallExpr* func_call_a)
            {
                AssertIntrinsicNumArgs(func_call_a, 1, 1);
                auto expr_ = func_call_a->arguments_.front().get();
                WriteOutputSemanticsAssignment(expr_, true);

                Write("EmitVertex()");
            }

            void GLSLGenerator::WriteCallExprIntrinsicTextureQueryLod(
                    CallExpr* func_call_a,
                    bool clamped_a)
            {
                AssertIntrinsicNumArgs(func_call_a, 2, 2);

                if (auto keyword_ = IntrinsicToGLSLKeyword(func_call_a->intrinsic_))
                {
                    Write(*keyword_ + "(");
                    Visit(func_call_a->arguments_[0]);
                    Write(", ");
                    Visit(func_call_a->arguments_[1]);
                    Write(").");
                    Write(clamped_a ? "y" : "x");
                } 
                else
                    ErrorIntrinsic(func_call_a->ident_, func_call_a);
            }

            void GLSLGenerator::WriteCallExprArguments(
                    CallExpr* call_expr_a,
                    std::size_t first_arg_index_a,
                    std::size_t num_write_args_a)
            {
                if (num_write_args_a <= num_write_args_a + first_arg_index_a)
                    num_write_args_a = num_write_args_a + first_arg_index_a;
                else
                    num_write_args_a = ~0u;

                const auto n_ = call_expr_a->arguments_.size();
                const auto m_ = std::min(num_write_args_a,
                                        n_ + call_expr_a->default_param_refs_.size());

                for (std::size_t i_ = first_arg_index_a; i_ < m_; ++i_)
                {
                    if (i_ < n_)
                        Visit(call_expr_a->arguments_[i_]);
                    else
                    {
                        auto default_param_ = call_expr_a->default_param_refs_[i_ - n_];
                        if (default_param_->initializer_value_
                                    .IsRepresentableAsString())
                            Write(default_param_->initializer_value_.ToString());
                        else
                            Visit(default_param_->initializer_);
                    }

                    if (i_ + 1 < m_)
                        Write(", ");
                }
            }

            void GLSLGenerator::WriteWrapperIntrinsics()
            {
                auto program_l_ = GetProgram();
                if (auto usage_ = program_l_->FetchIntrinsicUsage(Intrinsic::Clip))
                    WriteWrapperIntrinsicsClip(*usage_);
                if (auto usage_ = program_l_->FetchIntrinsicUsage(Intrinsic::Lit))
                    WriteWrapperIntrinsicsLit(*usage_);
                if (auto usage_ = program_l_->FetchIntrinsicUsage(
                            Intrinsic::SinCos))
                    WriteWrapperIntrinsicsSinCos(*usage_);

                if (program_l_->FetchIntrinsicUsage(
                            Intrinsic::GroupMemoryBarrierWithGroupSync)
                    != nullptr)
                    WriteWrapperIntrinsicsMemoryBarrier(
                            Intrinsic::GroupMemoryBarrier,
                            true);
                if (program_l_->FetchIntrinsicUsage(Intrinsic::DeviceMemoryBarrier)
                    != nullptr)
                    WriteWrapperIntrinsicsMemoryBarrier(
                            Intrinsic::DeviceMemoryBarrier,
                            false);
                if (program_l_->FetchIntrinsicUsage(
                            Intrinsic::DeviceMemoryBarrierWithGroupSync)
                    != nullptr)
                    WriteWrapperIntrinsicsMemoryBarrier(
                            Intrinsic::DeviceMemoryBarrier,
                            true);
                if (program_l_->FetchIntrinsicUsage(
                            Intrinsic::AllMemoryBarrierWithGroupSync)
                    != nullptr)
                    WriteWrapperIntrinsicsMemoryBarrier(
                            Intrinsic::AllMemoryBarrier,
                            true);
                if (program_l_->FetchIntrinsicUsage(Intrinsic::F16toF32)
                    != nullptr)
                    WriteWrapperIntrinsicsF16toF32();

                for (const auto& usage_ : program_l_->used_matrix_subscripts_)
                    WriteWrapperMatrixSubscript(usage_);
            }

            void GLSLGenerator::WriteWrapperIntrinsicsClip(
                    const IntrinsicUsage& usage_a)
            {
                bool any_wrappers_written_ = false;

                for (const auto& arg_list_ : usage_a.arg_lists_)
                {
                    auto arg0_type_ = (!arg_list_.arg_types_.empty()
                                             ? arg_list_.arg_types_.front()
                                             : DataType::Undefined);

                    if (IsScalarType(arg0_type_) || IsVectorType(arg0_type_))
                    {
                        BeginLn();
                        {
                            Write("void clip(");
                            WriteDataType(arg0_type_, IsESSL());
                            Write(" x)");

                            WriteScopeOpen(compact_wrappers_);
                            {
                                Write("if (");

                                if (IsScalarType(arg0_type_))
                                {
                                    Write("x < ");
                                    WriteLiteral("0", arg0_type_);
                                } 
                                else if (IsVectorType(arg0_type_))
                                {
                                    Write("any(lessThan(x, ");
                                    WriteDataType(arg0_type_);
                                    Write("(0)))");
                                }

                                Write(")");
                                WriteScopeOpen(compact_wrappers_);
                                {
                                    Write("discard;");
                                }
                                WriteScopeClose();
                            }
                            WriteScopeClose();
                        }
                        EndLn();

                        any_wrappers_written_ = true;
                    }
                }

                if (any_wrappers_written_)
                    Blank();
            }

            void GLSLGenerator::WriteWrapperIntrinsicsLit(
                    const IntrinsicUsage& usage_a)
            {
                (void)usage_a;
                BeginLn();
                {
                    Write("vec4 lit(");
                    WriteDataType(DataType::Float, IsESSL());
                    Write(" n_dot_l, ");
                    WriteDataType(DataType::Float, IsESSL());
                    Write(" n_dot_h, ");
                    WriteDataType(DataType::Float, IsESSL());
                    Write(" m)");

                    WriteScopeOpen(compact_wrappers_);
                    {
                        Write("return vec4(1.0f, max(0.0f, n_dot_l), max(0.0f, "
                              "n_dot_h * m), 1.0f);");
                    }
                    WriteScopeClose();
                }
                EndLn();

                Blank();
            }

            void GLSLGenerator::WriteWrapperIntrinsicsSinCos(
                    const IntrinsicUsage& usage_a)
            {
                bool any_wrappers_written_ = false;

                for (const auto& arg_list_ : usage_a.arg_lists_)
                {
                    if (arg_list_.arg_types_.size() == 3)
                    {
                        BeginLn();
                        {
                            Write("void sincos(");
                            WriteDataType(arg_list_.arg_types_[0], IsESSL());
                            Write(" x, out ");
                            WriteDataType(arg_list_.arg_types_[1], IsESSL());
                            Write(" s, out ");
                            WriteDataType(arg_list_.arg_types_[2], IsESSL());
                            Write(" c)");

                            WriteScopeOpen(compact_wrappers_);
                            {
                                Write("s = sin(x), c = cos(x);");
                            }
                            WriteScopeClose();
                        }
                        EndLn();

                        any_wrappers_written_ = true;
                    }
                }

                if (any_wrappers_written_)
                    Blank();
            }

            void GLSLGenerator::WriteWrapperIntrinsicsF16toF32()
            {
                BeginLn();
                {
                    Write("float f16tof32(");
                    WriteDataType(DataType::UInt, IsESSL());
                    Write(" x)");

                    WriteScopeOpen(compact_wrappers_);
                    {
                        Write("return unpackHalf2x16(x).x;");
                    }
                    WriteScopeClose();
                }
                EndLn();

                Blank();
            }

            static std::string GetWrapperNameForMemoryBarrier(
                    const Intrinsic intrinsic_a,
                    bool grou_sync_a)
            {
                std::string s_;

                switch (intrinsic_a)
                {
                case Intrinsic::GroupMemoryBarrier:
                    s_ += "Group";
                    break;
                case Intrinsic::DeviceMemoryBarrier:
                    s_ += "Device";
                    break;
                case Intrinsic::AllMemoryBarrier:
                    s_ += "All";
                    break;
                default:
                    return "";
                }

                s_ += "MemoryBarrier";

                if (grou_sync_a)
                    s_ += "WithGroupSync";

                return s_;
            }

            void GLSLGenerator::WriteWrapperIntrinsicsMemoryBarrier(
                    const Intrinsic intrinsic_a,
                    bool group_sync_a)
            {
                BeginLn();
                {
                    Write("void ");
                    Write(GetWrapperNameForMemoryBarrier(intrinsic_a, group_sync_a));
                    Write("()");

                    WriteScopeOpen(compact_wrappers_);
                    {
                        switch (intrinsic_a)
                        {
                        case Intrinsic::GroupMemoryBarrier:
                            WriteLn("groupMemoryBarrier();");
                            break;
                        case Intrinsic::DeviceMemoryBarrier:
                            WriteLn("memoryBarrierAtomicCounter();");
                            WriteLn("memoryBarrierImage();");
                            WriteLn("memoryBarrierBuffer();");
                            break;
                        case Intrinsic::AllMemoryBarrier:
                            WriteLn("memoryBarrier();");
                            break;
                        default:
                            break;
                        }

                        if (group_sync_a)
                            WriteLn("barrier();");
                    }
                    WriteScopeClose();
                }
                EndLn();

                Blank();
            }

            void GLSLGenerator::WriteWrapperMatrixSubscript(
                    const MatrixSubscriptUsage& usage_a)
            {
                if (IsScalarType(usage_a.data_type_out_))
                    return;

                BeginLn();
                {
                    WriteDataType(usage_a.data_type_out_, IsESSL());

                    Write(" ");
                    Write(ExprConverter::GetMatrixSubscriptWrapperIdent(
                            name_mangling_,
                            usage_a));
                    Write("(");
                    WriteDataType(usage_a.data_type_in_, IsESSL());
                    Write(" m)");

                    WriteScopeOpen(compact_wrappers_);
                    {
                        BeginLn();
                        {
                            Write("return ");

                            WriteDataType(usage_a.data_type_out_, IsESSL());
                            Write("(");

                            for (std::size_t i_ = 0, n_ = usage_a.indices_.size();
                                 i_ < n_;
                                 ++i_)
                            {
                                const auto& idx_ = usage_a.indices_[i_];
                                Write("m[" + std::to_string(idx_.first) + "]["
                                      + std::to_string(idx_.second) + "]");
                                if (i_ + 1 < n_)
                                    Write(", ");
                            }

                            Write(");");
                        }
                        EndLn();
                    }
                    WriteScopeClose();
                }
                EndLn();

                Blank();
            }

            bool GLSLGenerator::WriteStructDecl(StructDecl* struct_decl_a,
                                                bool end_with_semicolon_a)
            {
                BeginLn();

                Write("struct");
                if (!struct_decl_a->ident_.Empty())
                    Write(' ' + struct_decl_a->ident_);

                WriteScopeOpen(false, end_with_semicolon_a);
                BeginSep();
                {
                    WriteStmntList(struct_decl_a->var_members_);
                }
                EndSep();
                WriteScopeClose();

                if (!InsideVarDeclStmnt())
                    Blank();

                std::vector<BasicDeclStmnt*> func_member_stmnts_;
                func_member_stmnts_.reserve(struct_decl_a->func_members_.size());

                for (auto& func_decl_ : struct_decl_a->func_members_)
                    func_member_stmnts_.push_back(func_decl_->decl_stmnt_ref_);

                WriteStmntList(func_member_stmnts_);

                return true;
            }

            void GLSLGenerator::WriteBufferDecl(BufferDecl* buffer_decl_a)
            {
                if (buffer_decl_a->flags_(AST::IsReachable))
                {
                    if (IsStorageBufferType(buffer_decl_a->GetBufferType()))
                        WriteBufferDeclStorageBuffer(buffer_decl_a);
                    else
                        WriteBufferDeclTexture(buffer_decl_a);
                    Blank();
                }
            }

            void GLSLGenerator::WriteBufferDeclTexture(BufferDecl* buffer_decl_a)
            {
                const std::string* buffer_type_keyword_ = nullptr;

                if (buffer_decl_a->flags_(BufferDecl::IsUsedForCompare)
                    && !UseSeparateSamplers())
                {
                    SamplerType sampler_type_ = TextureTypeToSamplerType(
                            buffer_decl_a->GetBufferType());
                    SamplerType shadow_sampler_type_ =
                            SamplerTypeToShadowSamplerType(sampler_type_);

                    buffer_type_keyword_ =
                            SamplerTypeToKeyword(shadow_sampler_type_,
                                                 buffer_decl_a->decl_stmnt_ref_);
                } 
                else
                {
                    buffer_type_keyword_ =
                            BufferTypeToKeyword(buffer_decl_a->GetBufferType(),
                                                buffer_decl_a->decl_stmnt_ref_);
                }

                if (!buffer_type_keyword_)
                    return;

                bool is_write_only_ = (!buffer_decl_a->flags_(
                        BufferDecl::IsUsedForImageRead));

                auto image_layout_format_ = ImageLayoutFormat::Undefined;
                auto is_rw_buffer_ = IsRWImageBufferType(
                        buffer_decl_a->GetBufferType());

                if (!is_write_only_ && is_rw_buffer_)
                {
                    if (image_layout_format_ == ImageLayoutFormat::Undefined)
                    {
                        if (buffer_decl_a->decl_stmnt_ref_->type_denoter_
                                    ->generic_type_denoter_)
                        {
                            if (auto base_type_den_ =
                                        buffer_decl_a->decl_stmnt_ref_->type_denoter_
                                                ->generic_type_denoter_
                                                ->As<BaseTypeDenoter>())
                               
                                image_layout_format_ = DataTypeToImageLayoutFormat(
                                        base_type_den_->data_type_);
                        }
                    }
                }

                BeginLn();
                {
                    WriteLayout({
                            [&]() {
                                if (!is_write_only_)
                                {
                                    if (auto keyword_ =
                                                ImageLayoutFormatToGLSLKeyword(
                                                        image_layout_format_))
                                        Write(*keyword_);
                                }
                            },

                            [&]() {
                                WriteLayoutBinding(buffer_decl_a->slot_registers_);
                            },
                    });

                    if (is_rw_buffer_
                        && (is_write_only_
                            || image_layout_format_
                                       == ImageLayoutFormat::Undefined))
                        Write("writeonly ");

                    Write("uniform ");

                    if (auto generic_type_den_ =
                                buffer_decl_a->decl_stmnt_ref_->type_denoter_
                                        ->generic_type_denoter_)
                    {
                        if (auto base_type_den_ =
                                    generic_type_den_->As<BaseTypeDenoter>())
                        {
                            if (IsIntType(base_type_den_->data_type_))
                                Write("i");
                            else if (IsUIntType(base_type_den_->data_type_))
                                Write("u");
                        }
                    }

                    Write(*buffer_type_keyword_ + " " + buffer_decl_a->ident_);

                    Visit(buffer_decl_a->array_dims_);
                    Write(";");
                }
                EndLn();
            }

            void GLSLGenerator::WriteBufferDeclStorageBuffer(
                    BufferDecl* buffer_decl_a)
            {
                auto buffer_type_keyword_ =
                        BufferTypeToKeyword(buffer_decl_a->GetBufferType(),
                                            buffer_decl_a->decl_stmnt_ref_);
                if (!buffer_type_keyword_)
                    return;

                BeginLn();
                {
                    WriteLayout({
                            [&]() { Write("std430"); },
                            [&]() {
                                WriteLayoutBinding(buffer_decl_a->slot_registers_);
                            },
                    });
                    Write(*buffer_type_keyword_ + " ");

                    if (name_mangling_.rename_buffer_fields_)
                    {
                        Write(buffer_decl_a->ident_);
                        buffer_decl_a->ident_.AppendPrefix(
                                name_mangling_.temporary_prefix_);
                    } 
                    else
                        Write(name_mangling_.temporary_prefix_
                              + buffer_decl_a->ident_);

                    WriteScopeOpen(false, true);
                    {
                        if (!IsRWBufferType(buffer_decl_a->GetBufferType()))
                            Write("readonly ");

                        auto generic_type_den_ =
                                buffer_decl_a->decl_stmnt_ref_->type_denoter_
                                        ->GetGenericTypeDenoter();
                       
                        WriteTypeDenoter(*generic_type_den_, IsESSL(), buffer_decl_a);
                        Write(" " + buffer_decl_a->ident_ + "[];");
                    }
                    WriteScopeClose();
                }
                EndLn();
            }

            void GLSLGenerator::WriteSamplerDecl(SamplerDecl& sampler_decl_a)
            {
                if (UseSeparateSamplers()
                    || !IsSamplerStateType(
                            sampler_decl_a.decl_stmnt_ref_->type_denoter_->sampler_type_))
                {
                    auto sampler_type_keyword_ =
                            SamplerTypeToKeyword(sampler_decl_a.GetSamplerType(),
                                                 sampler_decl_a.decl_stmnt_ref_);
                    if (!sampler_type_keyword_)
                        return;

                    BeginLn();
                    {
                        WriteLayout({[&]() {
                            WriteLayoutBinding(sampler_decl_a.slot_registers_);
                        }});

                        Write("uniform " + *sampler_type_keyword_ + " "
                              + sampler_decl_a.ident_);

                        Visit(sampler_decl_a.array_dims_);
                        Write(";");
                    }
                    EndLn();

                    Blank();
                }
            }

            void GLSLGenerator::WriteStmntComment(Stmnt* ast_a, bool insert_blank_a)
            {
                if (ast_a && !ast_a->comment_.empty())
                {
                    if (insert_blank_a)
                        Blank();
                    WriteComment(ast_a->comment_);
                }
            }

            template <typename T>
            T* GetRawPtr(T* ptr_a)
            {
                return ptr_a;
            }

            template <typename T>
            T* GetRawPtr(const std::shared_ptr<T>& ptr_a)
            {
                return ptr_a.get();
            }

            template <typename T>
            void GLSLGenerator::WriteStmntList(const std::vector<T>& stmnts_a,
                                               bool is_global_scope_a)
            {
                if (preserve_comments_)
                {
                    for (std::size_t i_ = 0; i_ < stmnts_a.size(); ++i_)
                    {
                        auto ast_ = GetRawPtr(stmnts_a[i_]);

                        if (!is_global_scope_a || ast_->flags_(AST::IsReachable))
                            WriteStmntComment(ast_, (!is_global_scope_a && (i_ > 0)));

                        Visit(ast_);
                    }
                } 
                else
                {
                    Visit(stmnts_a);
                }
            }

            void GLSLGenerator::WriteParameter(VarDeclStmnt* ast_a)
            {
                if (ast_a->IsOutput())
                {
                    if (ast_a->IsInput())
                        Write("inout ");
                    else
                        Write("out ");
                }

                WriteTypeModifiersFrom(ast_a->type_specifier_);

                Visit(ast_a->type_specifier_);
                Write(" ");

                if (ast_a->var_decls_.size() == 1)
                {
                    auto param_var_ = ast_a->var_decls_.front().get();
                    Write(param_var_->ident_);
                    Visit(param_var_->array_dims_);
                } 
                else
                    Error(R_InvalidParamVarCount, ast_a);
            }

            void GLSLGenerator::WriteScopedStmnt(Stmnt* ast_a)
            {
                if (ast_a)
                {
                    if (ast_a->Type() != AST::Types::CodeBlockStmnt)
                    {
                        WriteScopeOpen(false, false, always_braced_scopes_);
                        {
                            Visit(ast_a);
                        }
                        WriteScopeClose();
                    } 
                    else
                        Visit(ast_a);
                }
            }

            void GLSLGenerator::WriteLiteral(const std::string& value_a,
                                             const DataType& data_type_a,
                                             const AST* ast_a)
            {
                if (IsScalarType(data_type_a))
                {
                    Write(value_a);

                    switch (data_type_a)
                    {
                    case DataType::UInt:
                        if (!value_a.empty() && value_a.back() != 'u'
                            && value_a.back() != 'U')
                            Write("u");
                        break;
                    case DataType::Float:
                        if (value_a.find_first_of(".eE") == std::string::npos)
                            Write(".0");
                        Write("f");
                        break;
                    default:
                        break;
                    }
                } 
                else if (IsVectorType(data_type_a))
                {
                    WriteDataType(data_type_a, false, ast_a);
                    Write("(");
                    Write(value_a);
                    Write(")");
                } 
                else
                    Error(R_FailedToWriteLiteralType(value_a), ast_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel