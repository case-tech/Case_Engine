#include "UniformPacker.hpp"

#include "../AST.hpp"
#include "../ASTFactory.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void UniformPacker::Convert(Program& program_a,
                                        const CbufferAttributes& cbuffer_attribs_a,
                                        bool only_reachable_stmnts_a)
            {
                if (cbuffer_attribs_a.name_.empty())
                    return;

                cbuffer_attribs_ = cbuffer_attribs_a;

                auto& global_stmnts_ = program_a.global_stmnts_;

                for (auto it_ = global_stmnts_.begin(); it_ != global_stmnts_.end();)
                {
                    bool IsReachable = (*it_)->flags_(AST::IsReachable);
                    if ((*it_)->Type() == AST::Types::VarDeclStmnt
                        && (IsReachable || !only_reachable_stmnts_a))
                    {
                        auto var_decl_stmnt_ =
                                std::static_pointer_cast<VarDeclStmnt>(*it_);

                        if (var_decl_stmnt_->IsUniform()
                            && CanConvertUniformWithTypeDenoter(*(
                                    var_decl_stmnt_->type_specifier_->type_denoter_)))
                        {
                            if (!decl_stmnt_)
                            {
                                MakeUniformBuffer();

                                AppendUniform(var_decl_stmnt_);
                                it_ = global_stmnts_.erase(it_);

                                it_ = global_stmnts_.insert(it_, decl_stmnt_);
                            } 
                            else
                            {
                                AppendUniform(var_decl_stmnt_);
                                it_ = global_stmnts_.erase(it_);
                                continue;
                            }

                            if (IsReachable)
                            {
                                decl_stmnt_->flags_ << AST::IsReachable;
                                decl_stmnt_->decl_object_->flags_
                                        << AST::IsReachable;
                            }
                        }
                    }

                    ++it_;
                }

                if (auto entry_point_ = program_a.entry_point_ref_)
                {
                    auto& parameters_ = entry_point_->parameters_;
                    for (auto it_ = parameters_.begin(); it_ != parameters_.end();)
                    {
                        auto var_decl_stmnt_ = *it_;

                        if (var_decl_stmnt_->IsUniform()
                            && CanConvertUniformWithTypeDenoter(*(
                                    var_decl_stmnt_->type_specifier_->type_denoter_)))
                        {
                            if (!decl_stmnt_)
                            {
                                MakeUniformBuffer();

                                AppendUniform(var_decl_stmnt_);
                                it_ = parameters_.erase(it_);

                                global_stmnts_.insert(global_stmnts_.begin(),
                                                    decl_stmnt_);
                            } 
                            else
                            {
                                AppendUniform(var_decl_stmnt_);
                                it_ = parameters_.erase(it_);
                            }

                            {
                                decl_stmnt_->flags_ << AST::IsReachable;
                                decl_stmnt_->decl_object_->flags_
                                        << AST::IsReachable;
                            }
                        } 
                        else
                            ++it_;
                    }
                }
            }

            void UniformPacker::MakeUniformBuffer()
            {
                decl_stmnt_ = std::make_shared<BasicDeclStmnt>(
                        SourcePosition::ignore);
                {
                    uniform_buffer_decl_ = ASTFactory::MakeUniformBufferDecl(
                            cbuffer_attribs_.name_,
                            cbuffer_attribs_.binding_slot_);
                    uniform_buffer_decl_->decl_stmnt_ref_ = decl_stmnt_.get();
                }
                decl_stmnt_->decl_object_ = uniform_buffer_decl_;
            }

            void UniformPacker::AppendUniform(
                    const VarDeclStmntPtr& var_decl_stmnt_a)
            {
                uniform_buffer_decl_->local_stmnts_.push_back(var_decl_stmnt_a);
                uniform_buffer_decl_->var_members_.push_back(var_decl_stmnt_a);

                var_decl_stmnt_a->type_specifier_->is_uniform_ = false;

                for (auto& var_decl_ : var_decl_stmnt_a->var_decls_)
                    var_decl_->initializer_.reset();
            }

            bool UniformPacker::CanConvertUniformWithTypeDenoter(
                    const TypeDenoter& type_den_a) const
            {
                return !(type_den_a.IsSampler() || type_den_a.IsBuffer());
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel