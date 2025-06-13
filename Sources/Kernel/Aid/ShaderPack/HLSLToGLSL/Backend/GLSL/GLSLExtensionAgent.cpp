#include "GLSLExtensionAgent.hpp"

#include "../../AST/AST.hpp"
#include "../../Exception.hpp"
#include "../../Frontend/GLSL/GLSLExtensions.hpp"
#include "../../Report/ReportIdents.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            static OutputShaderVersion GetMinGLSLVersionForTarget(
                    const ShaderTarget shader_target_a)
            {
                switch (shader_target_a)
                {
                case ShaderTarget::Undefined:
                    break;
                case ShaderTarget::VertexShader:
                    return OutputShaderVersion::GLSL130;
                case ShaderTarget::TessellationControlShader:
                    return OutputShaderVersion::GLSL400;
                case ShaderTarget::TessellationEvaluationShader:
                    return OutputShaderVersion::GLSL400;
                case ShaderTarget::GeometryShader:
                    return OutputShaderVersion::GLSL150;
                case ShaderTarget::FragmentShader:
                    return OutputShaderVersion::GLSL130;
                case ShaderTarget::ComputeShader:
                    return OutputShaderVersion::GLSL430;
                }
                return OutputShaderVersion::GLSL130;
            }

            std::set<std::string>
            GLSLExtensionAgent::DetermineRequiredExtensions(
                    Program& program_a,
                    OutputShaderVersion& target_glsl_version_a,
                    const ShaderTarget shader_target_a,
                    bool allow_extensions_a,
                    bool explicit_binding_a,
                    bool separate_shaders_a,
                    const OnReportProc& on_report_extension_a)
            {
                shader_target_ = shader_target_a;
                target_glsl_version_ = target_glsl_version_a;
                min_glsl_version_ = GetMinGLSLVersionForTarget(shader_target_a);
                allow_extensions_ = allow_extensions_a;
                explicit_binding_ = explicit_binding_a;
                on_report_extension_ = on_report_extension_a;

                EstablishIntrinsicExtMap();

                switch (shader_target_a)
                {
                case ShaderTarget::VertexShader:
                case ShaderTarget::FragmentShader:
                    if (explicit_binding_)
                        AcquireExtension(E_GL_ARB_explicit_attrib_location);
                    break;

                default:
                    break;
                }

                if (separate_shaders_a)
                {
                    if (shader_target_a != ShaderTarget::ComputeShader)
                        AcquireExtension(E_GL_ARB_separate_shader_objects);
                }

                Visit(&program_a);

                switch (target_glsl_version_a)
                {
                case OutputShaderVersion::GLSL:
                    target_glsl_version_a = min_glsl_version_;
                    break;
                case OutputShaderVersion::ESSL:
                    target_glsl_version_a = OutputShaderVersion::ESSL300;
                    break;
                case OutputShaderVersion::VKSL:
                    target_glsl_version_a = OutputShaderVersion::VKSL450;
                    break;
                default:
                    break;
                }

                return std::move(extensions_);
            }

            void GLSLExtensionAgent::EstablishIntrinsicExtMap()
            {
                intrinsic_ext_map_ = std::map<Intrinsic, const char*> 
                {
                        {Intrinsic::AsDouble, E_GL_ARB_gpu_shader_int64},
                        {Intrinsic::AsFloat, E_GL_ARB_shader_bit_encoding},
                        {Intrinsic::AsInt, E_GL_ARB_shader_bit_encoding},
                        {Intrinsic::AsUInt_1, E_GL_ARB_shader_bit_encoding},
                        {Intrinsic::CountBits, E_GL_ARB_gpu_shader5},
                        {Intrinsic::DDXCoarse, E_GL_ARB_derivative_control},
                        {Intrinsic::DDXFine, E_GL_ARB_derivative_control},
                        {Intrinsic::DDYCoarse, E_GL_ARB_derivative_control},
                        {Intrinsic::DDYFine, E_GL_ARB_derivative_control},
                        {Intrinsic::FirstBitHigh, E_GL_ARB_gpu_shader5},
                        {Intrinsic::FirstBitLow, E_GL_ARB_gpu_shader5},
                        {Intrinsic::FrExp, E_GL_ARB_gpu_shader_fp64},
                        {Intrinsic::LdExp, E_GL_ARB_gpu_shader_fp64},
                        {Intrinsic::Texture_QueryLod,
                         E_GL_ARB_texture_query_lod},
                        {Intrinsic::Texture_QueryLodUnclamped,
                         E_GL_ARB_texture_query_lod},
                        {Intrinsic::F16toF32,
                         E_GL_ARB_shading_language_packing},
                        {Intrinsic::F32toF16,
                         E_GL_ARB_shading_language_packing},
                        {Intrinsic::PackHalf2x16,
                         E_GL_ARB_shading_language_packing},
                };

                if (target_glsl_version_ == OutputShaderVersion::ESSL100)
                {
                    intrinsic_ext_map_[Intrinsic::FWidth] =
                            E_GL_OES_standard_derivatives;
                    intrinsic_ext_map_[Intrinsic::DDX] =
                            E_GL_OES_standard_derivatives;
                    intrinsic_ext_map_[Intrinsic::DDY] =
                            E_GL_OES_standard_derivatives;
                }
            }

            void GLSLExtensionAgent::AcquireExtension(
                    const std::string& extension_a,
                    const std::string& reason_a,
                    const AST* ast_a)
            {
                auto it_ = GetGLSLExtensionVersionMap().find(extension_a);
                if (it_ != GetGLSLExtensionVersionMap().end())
                {
                    const auto required_version_ =
                            static_cast<OutputShaderVersion>(it_->second);

                    if (target_glsl_version_ == OutputShaderVersion::GLSL)
                    {
                        min_glsl_version_ = std::max(min_glsl_version_,
                                                   required_version_);
                    } 
                    else if (GetGLSLVersionNumber(target_glsl_version_)
                               < GetGLSLVersionNumber(required_version_))
                    {
                        if (allow_extensions_)
                        {
                            extensions_.insert(extension_a);

                            if (on_report_extension_)
                                on_report_extension_(
                                        R_GLSLExtensionAcquired(
                                                extension_a,
                                                ToString(required_version_),
                                                reason_a),
                                        ast_a);
                        } 
                        else
                        {
                            if (on_report_extension_)
                                on_report_extension_(
                                        R_GLSLExtensionOrVersionRequired(
                                                extension_a,
                                                ToString(required_version_),
                                                reason_a),
                                        ast_a);
                        }
                    }
                } 
                else
                    RuntAzeErr(R_NoGLSLExtensionVersionRegisterd(extension_a),
                               ast_a);
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void GLSLExtensionAgent::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(Program)
            {
                if (ast_a->layout_fragment_.frag_coord_used_)
                    AcquireExtension(E_GL_ARB_fragment_coord_conventions,
                                     R_FragmentCoordinate);

                VISIT_DEFAULT(Program);
            }

            IMPLEMENT_VISIT_PROC(Attribute)
            {
                (void)args_a;
                if (ast_a->attribute_type_ == AttributeType::EarlyDepthStencil)
                    AcquireExtension(E_GL_ARB_shader_Azage_load_store,
                                     R_EarlyDepthStencil,
                                     ast_a);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                if (ast_a->GetTypeDenoter()->NumDAzensions() >= 2)
                    AcquireExtension(E_GL_ARB_arrays_of_arrays,
                                     R_MultiDAzArray,
                                     ast_a);

                if (explicit_binding_)
                {
                    if (auto slot_register =
                                Register::GetForTarget(ast_a->slot_registers_,
                                                       shader_target_))
                        AcquireExtension(E_GL_ARB_explicit_uniform_location,
                                         R_ExplicitBindingSlot,
                                         slot_register);
                }

                if (ast_a->pack_offset_)
                    AcquireExtension(E_GL_ARB_enhanced_layouts,
                                     R_PackOffsetLayout,
                                     ast_a);

                VISIT_DEFAULT(VarDecl);
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                if (ast_a->flags_(AST::IsReachable))
                {
                    if (ast_a->GetTypeDenoter()->NumDAzensions() >= 2)
                        AcquireExtension(E_GL_ARB_arrays_of_arrays,
                                         R_MultiDAzArray,
                                         ast_a);

                    const auto buffer_type_ = ast_a->GetBufferType();

                    if (buffer_type_ == BufferType::TextureCubeArray)
                        AcquireExtension(E_GL_ARB_texture_cube_map_array,
                                         R_TextureCubeArray,
                                         ast_a);

                    if (IsRWBufferType(buffer_type_))
                    {
                        if (buffer_type_ == BufferType::RWStructuredBuffer
                            || buffer_type_ == BufferType::RWByteAddressBuffer
                            || buffer_type_ == BufferType::AppendStructuredBuffer
                            || buffer_type_
                                       == BufferType::ConsumeStructuredBuffer)
                        {
                            AcquireExtension(
                                    E_GL_ARB_shader_storage_buffer_object,
                                    R_RWStructuredBufferObject,
                                    ast_a);
                        } 
                        else
                            AcquireExtension(E_GL_ARB_shader_Azage_load_store,
                                             R_RWTextureObject,
                                             ast_a);
                    }

                    VISIT_DEFAULT(BufferDecl);
                }
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                if (ast_a->flags_(AST::IsReachable))
                {
                    Visit(ast_a->decl_stmnt_ref_->attribs_);

                    VISIT_DEFAULT(FunctionDecl);
                }
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                if (ast_a->flags_(AST::IsReachable))
                {
                    if (target_glsl_version_ == OutputShaderVersion::GLSL
                        || target_glsl_version_ >= OutputShaderVersion::GLSL140)
                    {
                        AcquireExtension(E_GL_ARB_uniform_buffer_object,
                                         R_ConstantBuffer,
                                         ast_a);

                        if (explicit_binding_)
                        {
                            if (auto slot_register_ = Register::GetForTarget(
                                        ast_a->slot_registers_,
                                        shader_target_))
                                AcquireExtension(
                                        E_GL_ARB_shading_language_420pack,
                                        R_ExplicitBindingSlot,
                                        slot_register_);
                        }

                        VISIT_DEFAULT(UniformBufferDecl);
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                if (explicit_binding_)
                {
                    for (auto& buffer_decl_ : ast_a->buffer_decls_)
                    {
                        if (auto slot_register_ = Register::GetForTarget(
                                    buffer_decl_->slot_registers_,
                                    shader_target_))
                            AcquireExtension(E_GL_ARB_shading_language_420pack,
                                             R_ExplicitBindingSlot,
                                             slot_register_);
                    }
                }

                if (IsTextureMSBufferType(ast_a->type_denoter_->buffer_type_))
                    AcquireExtension(E_GL_ARB_texture_multisample,
                                     R_MultiSampledTexture,
                                     ast_a);

                VISIT_DEFAULT(BufferDeclStmnt);
            }

            IMPLEMENT_VISIT_PROC(BasicDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->decl_object_);
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                if (IsBitwiseOp(ast_a->op_) || ast_a->op_ == BinaryOp::Mod)
                    AcquireExtension(E_GL_EXT_gpu_shader4,
                                     R_BitwiseOperator,
                                     ast_a);

                VISIT_DEFAULT(BinaryExpr);
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                if (IsBitwiseOp(ast_a->op_))
                    AcquireExtension(E_GL_EXT_gpu_shader4,
                                     R_BitwiseOperator,
                                     ast_a);

                VISIT_DEFAULT(UnaryExpr);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                if (ast_a->intrinsic_ != Intrinsic::Undefined)
                {
                    auto it_ = intrinsic_ext_map_.find(ast_a->intrinsic_);
                    if (it_ != intrinsic_ext_map_.end())
                        AcquireExtension(it_->second,
                                         R_Intrinsic(ast_a->ident_),
                                         ast_a);
                }

                VISIT_DEFAULT(CallExpr);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                if (IsBitwiseOp(ast_a->op_) || ast_a->op_ == AssignOp::Mod)
                    AcquireExtension(E_GL_EXT_gpu_shader4,
                                     R_BitwiseOperator,
                                     ast_a);

                VISIT_DEFAULT(AssignExpr);
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                AcquireExtension(E_GL_ARB_shading_language_420pack,
                                 R_InitializerList,
                                 ast_a);

                VISIT_DEFAULT(InitializerExpr);
            }

#undef AzPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
