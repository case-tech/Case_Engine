#include "ReflectionAnalyzer.hpp"

#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"
#include "ExprEvaluator.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            ReflectionAnalyzer::ReflectionAnalyzer(Log* log_a)
                : report_handler_ {log_a}
            {}

            void ReflectionAnalyzer::Reflect(
                    Program& program_a,
                    const ShaderTarget shader_target_a,
                    Reflection::ReflectionData& reflection_data_a,
                    bool enable_warnings_a)
            {
                shader_target_ = shader_target_a;
                program_ = (&program_a);
                data_ = (&reflection_data_a);
                enable_warnings_ = enable_warnings_a;

                Visit(program_);
            }

            void ReflectionAnalyzer::Warning(const std::string& msg_a,
                                             const AST* ast_a)
            {
                if (enable_warnings_)
                    report_handler_.Warning(false,
                                           msg_a,
                                           program_->source_code_.get(),
                                           (ast_a ? ast_a->area_
                                                : SourceArea::ignore));
            }

            int ReflectionAnalyzer::GetBindingPoint(
                    const std::vector<RegisterPtr>& slot_registers_a) const
            {
                if (auto slot_register_ = Register::GetForTarget(slot_registers_a,
                                                               shader_target_))
                    return slot_register_->slot_;
                else
                    return -1;
            }

            int ReflectionAnalyzer::EvaluateConstExprInt(Expr& expr_a)
            {
                ExprEvaluator expr_evaluator_;
                return static_cast<int>(
                        expr_evaluator_
                                .EvaluateOrDefault(expr_a, Variant::IntType(0))
                                .ToInt());
            }

            float ReflectionAnalyzer::EvaluateConstExprFloat(Expr& expr_a)
            {
                ExprEvaluator expr_evaluator_;
                return static_cast<float>(
                        expr_evaluator_
                                .EvaluateOrDefault(expr_a, Variant::RealType(0.0))
                                .ToReal());
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void ReflectionAnalyzer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(Program)
            {
                (void)args_a;
                Visit(ast_a->global_stmnts_);
                Visit(ast_a->disabled_ast_);

                if (auto entry_point_ = ast_a->entry_point_ref_)
                {
                    for (auto var_decl_ : entry_point_->input_semantics_.var_decl_refs_)
                        data_->input_attributes_.push_back(
                                {var_decl_->ident_, var_decl_->semantic_.Index()});
                    for (auto var_decl_ :
                         entry_point_->input_semantics_.var_decl_refs_sv_)
                        data_->input_attributes_.push_back(
                                {var_decl_->semantic_.ToString(),
                                 var_decl_->semantic_.Index()});

                    for (auto var_decl_ : entry_point_->output_semantics_.var_decl_refs_)
                        data_->output_attributes_.push_back(
                                {var_decl_->ident_, var_decl_->semantic_.Index()});
                    for (auto var_decl_ :
                         entry_point_->output_semantics_.var_decl_refs_sv_)
                        data_->output_attributes_.push_back(
                                {var_decl_->semantic_.ToString(),
                                 var_decl_->semantic_.Index()});

                    if (entry_point_->semantic_.IsSystemValue())
                        data_->output_attributes_.push_back(
                                {entry_point_->semantic_.ToString(),
                                 entry_point_->semantic_.Index()});
                }
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                (void)args_a;
                if (ast_a->sampler_values_.empty())
                {
                    Reflection::SamplerState sampler_state_;
                    {
                        sampler_state_.referenced_ = ast_a->flags_(
                                AST::IsReachable);
                        sampler_state_.type_ = SamplerTypeToResourceType(
                                ast_a->GetSamplerType());
                        sampler_state_.name_ = ast_a->ident_;
                        sampler_state_.slot_ = GetBindingPoint(
                                ast_a->slot_registers_);
                    }
                    data_->sampler_states_.push_back(sampler_state_);
                } 
                else
                {
                    Reflection::StaticSamplerState sampler_state_;
                    {
                        sampler_state_.type_ = SamplerTypeToResourceType(
                                ast_a->GetSamplerType());
                        sampler_state_.name_ = ast_a->ident_;
                        for (auto& value_ : ast_a->sampler_values_)
                            ReflectSamplerValue(value_.get(), sampler_state_.desc_);
                    }
                    data_->static_sampler_states_.push_back(sampler_state_);
                }
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                Visitor::VisitStructDecl(ast_a, args_a);

                const auto record_index_ = data_->records_.size();

                Reflection::Record record_;
                {
                    record_.referenced_ = ast_a->flags_(AST::IsReachable);
                    record_.name_ = ast_a->ident_;
                    record_.base_record_index_ = FindRecordIndex(
                            ast_a->base_struct_ref_);

                    record_.size_ = 0;
                    record_.padding_ = 0;

                    for (const auto& member_ : ast_a->var_members_)
                    {
                        for (const auto& var_ : member_->var_decls_)
                        {
                            Reflection::Field field_;
                            ReflectField(var_.get(),
                                         field_,
                                         record_.size_,
                                         record_.padding_);
                            record_.fields_.push_back(field_);
                        }
                    }
                }
                data_->records_.push_back(record_);

                record_indices_map_[ast_a] = record_index_;
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                if (ast_a->flags_(FunctionDecl::IsEntryPoint))
                    ReflectAttributes(ast_a->decl_stmnt_ref_->attribs_);

                Visitor::VisitFunctionDecl(ast_a, args_a);
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)args_a;
                Reflection::ConstantBuffer constant_buffer_;
                {
                    constant_buffer_.referenced_ = ast_a->flags_(
                            AST::IsReachable);
                    constant_buffer_.type_ = UniformBufferTypeToResourceType(
                            ast_a->buffer_type_);
                    constant_buffer_.name_ = ast_a->ident_;
                    constant_buffer_.slot_ = GetBindingPoint(
                            ast_a->slot_registers_);

                    constant_buffer_.size_ = 0;
                    constant_buffer_.padding_ = 0;

                    for (const auto& member_ : ast_a->var_members_)
                    {
                        for (const auto& var_ : member_->var_decls_)
                        {
                            Reflection::Field field_;
                            ReflectField(var_.get(),
                                         field_,
                                         constant_buffer_.size_,
                                         constant_buffer_.padding_);
                            constant_buffer_.fields_.push_back(field_);
                        }
                    }
                }
                data_->constant_buffers_.push_back(constant_buffer_);
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                (void)args_a;
                for (auto& buffer_decl_ : ast_a->buffer_decls_)
                {
                    Reflection::Resource resource_;
                    {
                        resource_.referenced_ = buffer_decl_->flags_(
                                AST::IsReachable);
                        resource_.type_ = BufferTypeToResourceType(
                                ast_a->type_denoter_->buffer_type_);
                        resource_.name_ = buffer_decl_->ident_;
                        resource_.slot_ = GetBindingPoint(
                                buffer_decl_->slot_registers_);
                    };
                    data_->resources_.push_back(resource_);
                }
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                (void)args_a;
                if (auto type_specifier_ = ast_a->FetchTypeSpecifier())
                {
                    if (type_specifier_->is_uniform_)
                    {
                        Reflection::Attribute attribute_;
                        {
                            attribute_.referenced_ = ast_a->flags_(
                                    AST::IsReachable);
                            attribute_.name_ = ast_a->ident_;
                            attribute_.slot_ = GetBindingPoint(
                                    ast_a->slot_registers_);
                        }
                        data_->uniforms_.push_back(attribute_);
                    }
                }
            }

#undef IMPLEMENT_VISIT_PROC

            void ReflectionAnalyzer::ReflectSamplerValue(
                    SamplerValue* ast_a,
                    Reflection::SamplerStateDesc& desc_a)
            {
                const auto& name_ = ast_a->name_;

                if (auto literal_expr_ = ast_a->value_->As<LiteralExpr>())
                {
                    const auto& value_ = literal_expr_->value_;

                    if (name_ == "MipLODBias")
                        desc_a.mip_lod_bias_ = FromStringOrDefault<float>(value_);
                    else if (name_ == "MaxAnisotropy")
                        desc_a.max_anisotropy_ = static_cast<unsigned int>(
                                FromStringOrDefault<unsigned long>(value_));
                    else if (name_ == "MinLOD")
                        desc_a.min_lod_ = FromStringOrDefault<float>(value_);
                    else if (name_ == "MaxLOD")
                        desc_a.max_lod_ = FromStringOrDefault<float>(value_);
                } else if (auto object_expr_ = ast_a->value_->As<ObjectExpr>())
                {
                    const auto& value_ = object_expr_->ident_;

                    if (name_ == "Filter")
                        ReflectSamplerValueFilter(value_, desc_a.filter_, ast_a);
                    else if (name_ == "AddressU")
                        ReflectSamplerValueTextureAddressMode(value_,
                                                              desc_a.address_u_,
                                                              ast_a);
                    else if (name_ == "AddressV")
                        ReflectSamplerValueTextureAddressMode(value_,
                                                              desc_a.address_v_,
                                                              ast_a);
                    else if (name_ == "AddressW")
                        ReflectSamplerValueTextureAddressMode(value_,
                                                              desc_a.address_w_,
                                                              ast_a);
                    else if (name_ == "ComparisonFunc")
                        ReflectSamplerValueComparisonFunc(value_,
                                                          desc_a.comparison_func_,
                                                          ast_a);
                } 
                else if (name_ == "BorderColor")
                {
                    try
                    {
                        if (auto call_expr_ = ast_a->value_->As<CallExpr>())
                        {
                            if (call_expr_->type_denoter_
                                && call_expr_->type_denoter_->IsVector()
                                && call_expr_->arguments_.size() == 4)
                            {
                                for (std::size_t i_ = 0; i_ < 4; ++i_)
                                    desc_a.border_color_[i_] =
                                            EvaluateConstExprFloat(
                                                    *call_expr_->arguments_[i_]);
                            } 
                            else
                                throw std::string(R_InvalidTypeOrArgCount);
                        } 
                        else if (auto cast_expr_ = ast_a->value_->As<CastExpr>())
                        {
                            auto sub_value_src_ = EvaluateConstExprFloat(
                                    *cast_expr_->expr_);
                            for (std::size_t i_ = 0; i_ < 4; ++i_)
                                desc_a.border_color_[i_] = sub_value_src_;
                        } 
                        else if (auto init_expr_ =
                                           ast_a->value_->As<InitializerExpr>())
                        {
                            if (init_expr_->exprs_.size() == 4)
                            {
                                for (std::size_t i_ = 0; i_ < 4; ++i_)
                                    desc_a.border_color_[i_] =
                                            EvaluateConstExprFloat(
                                                    *init_expr_->exprs_[i_]);
                            } 
                            else
                                throw std::string(R_InvalidArgCount);
                        }
                    } 
                    catch (const std::string& s_)
                    {
                        Warning(R_FailedToInitializeSamplerValue(s_,
                                                                 "BorderColor"),
                                ast_a->value_.get());
                    }
                }
            }

            void ReflectionAnalyzer::ReflectSamplerValueFilter(
                    const std::string& value_a,
                    Reflection::Filter& filter_a,
                    const AST* ast_a)
            {
                try
                {
                    filter_a = StringToFilter(value_a);
                } 
                catch (const std::invalid_argument& e_)
                {
                    Warning(e_.what(), ast_a);
                }
            }

            void ReflectionAnalyzer::ReflectSamplerValueTextureAddressMode(
                    const std::string& value_a,
                    Reflection::TextureAddressMode& address_mode_a,
                    const AST* ast_a)
            {
                try
                {
                    address_mode_a = StringToTexAddressMode(value_a);
                } 
                catch (const std::invalid_argument& e_)
                {
                    Warning(e_.what(), ast_a);
                }
            }

            void ReflectionAnalyzer::ReflectSamplerValueComparisonFunc(
                    const std::string& value_a,
                    Reflection::ComparisonFunc& comparison_func_a,
                    const AST* ast_a)
            {
                try
                {
                    comparison_func_a = StringToCompareFunc(value_a);
                } 
                catch (const std::invalid_argument& e_)
                {
                    Warning(e_.what(), ast_a);
                }
            }

            void ReflectionAnalyzer::ReflectAttributes(
                    const std::vector<AttributePtr>& attribs_a)
            {
                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::NumThreads:
                        ReflectAttributesNumThreads(attr_.get());
                        break;
                    default:
                        break;
                    }
                }
            }

            void ReflectionAnalyzer::ReflectAttributesNumThreads(Attribute* ast_a)
            {
                if (shader_target_ == ShaderTarget::ComputeShader
                    && ast_a->arguments_.size() == 3)
                {
                    data_->num_threads_.x_ = EvaluateConstExprInt(
                            *ast_a->arguments_[0]);
                    data_->num_threads_.y_ = EvaluateConstExprInt(
                            *ast_a->arguments_[1]);
                    data_->num_threads_.z_ = EvaluateConstExprInt(
                            *ast_a->arguments_[2]);
                }
            }

            static Reflection::FieldType ToFieldType(const DataType t_a)
            {
                using T = Reflection::FieldType;
                switch (BaseDataType(t_a))
                {
                case DataType::Bool:
                    return T::Bool;
                case DataType::Int:
                    return T::Int;
                case DataType::UInt:
                    return T::UInt;
                case DataType::Half:
                    return T::Half;
                case DataType::Float:
                    return T::Float;
                case DataType::Double:
                    return T::Double;
                default:
                    return T::Undefined;
                }
            }

            static void ReflectFieldBaseType(const DataType data_type_a,
                                             Reflection::Field& field_a)
            {
                field_a.type_ = ToFieldType(data_type_a);

                auto type_dim_ = MatrixTypeDim(data_type_a);
                field_a.dimensions_[0] = type_dim_.first;
                field_a.dimensions_[1] = type_dim_.second;
            }

            void ReflectionAnalyzer::ReflectField(VarDecl* ast_a,
                                                  Reflection::Field& field_a,
                                                  unsigned int& accum_size_a,
                                                  unsigned int& accum_padding_a)
            {
                field_a.referenced_ = ast_a->flags_(AST::IsReachable);
                field_a.name_ = ast_a->ident_;

                ReflectFieldType(field_a, ast_a->GetTypeDenoter()->GetAliased());

                const auto current_size_ = accum_size_a;
                const auto current_padding_ = accum_padding_a;

                if (ast_a->AccumAlignedVectorSize(accum_size_a,
                                                accum_padding_a,
                                                &(field_a.offset_)))
                {
                    const auto local_padding_ = (accum_padding_a - current_padding_);
                    field_a.size_ = (accum_size_a - current_size_ - local_padding_);
                } 
                else
                    field_a.size_ = (unsigned int)~0;
            }

            void ReflectionAnalyzer::ReflectFieldType(
                    Reflection::Field& field_a,
                    const TypeDenoter& type_den_a)
            {
                if (auto base_type_den_ = type_den_a.As<BaseTypeDenoter>())
                {
                    ReflectFieldBaseType(base_type_den_->data_type_, field_a);
                } 
                else if (auto struct_type_den_ = type_den_a.As<StructTypeDenoter>())
                {
                    field_a.type_ = Reflection::FieldType::Record;
                    field_a.dimensions_[0] = 0;
                    field_a.dimensions_[1] = 0;
                    field_a.type_record_index_ = FindRecordIndex(
                            struct_type_den_->struct_decl_ref_);
                } 
                else if (auto array_type_den_ = type_den_a.As<ArrayTypeDenoter>())
                {
                    ReflectFieldType(
                            field_a,
                            array_type_den_->sub_type_denoter_->GetAliased());

                    auto dim_sizes_ = array_type_den_->GetDimensionSizes();
                    field_a.array_elements_.reserve(dim_sizes_.size());

                    for (auto size_ : dim_sizes_)
                    {
                        if (size_ >= 0)
                            field_a.array_elements_.push_back(
                                    static_cast<unsigned int>(size_));
                        else
                            field_a.array_elements_.push_back(0);
                    }
                }
            }

            int ReflectionAnalyzer::FindRecordIndex(
                    const StructDecl* struct_decl_a) const
            {
                auto it_ = record_indices_map_.find(struct_decl_a);
                if (it_ != record_indices_map_.end())
                    return (int)it_->second;
                else
                    return -1;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel