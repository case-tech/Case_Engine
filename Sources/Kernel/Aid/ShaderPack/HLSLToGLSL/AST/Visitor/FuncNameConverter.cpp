#include "FuncNameConverter.hpp"

#include "../AST.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void FuncNameConverter::Convert(
                    Program& program_a,
                    const NameMangling& name_mangling_a,
                    const OnFuncSigantureCompare& on_func_signature_compare_a,
                    const Flags& conversion_flags_a)
            {
                name_mangling_ = name_mangling_a;
                on_func_signature_compare_ = on_func_signature_compare_a;
                conversion_flags_ = conversion_flags_a;

                Visit(&program_a);

                for (auto& it_ : func_decl_map_)
                    ConvertEqualFunctionSignatures(it_.second);
            }

            void FuncNameConverter::ConvertEqualFunctionSignatures(
                    FuncList& func_list_a)
            {
                if (on_func_signature_compare_)
                {
                    unsigned int name_index_ = 0;

                    for (std::size_t i_ = 0, n_ = func_list_a.size(); i_ + 1 < n_; ++i_)
                    {
                        if (auto func_lhs_ = func_list_a[i_])
                        {
                            for (std::size_t j_ = i_ + 1; j_ < n_; ++j_)
                            {
                                if (auto func_rhs_ = func_list_a[j_])
                                {
                                    if (on_func_signature_compare_(*func_lhs_,
                                                                *func_rhs_))
                                    {
                                        if (name_index_ == 0)
                                            ConvertMemberFunctionName(
                                                    *func_lhs_,
                                                    name_index_);

                                        ConvertMemberFunctionName(*func_rhs_,
                                                                  name_index_);

                                        func_list_a[j_] = nullptr;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void FuncNameConverter::ConvertMemberFunctionName(
                    FunctionDecl& func_decl_a,
                    unsigned int& name_index_a)
            {
                func_decl_a.ident_.AppendPrefix(name_mangling_.namespace_prefix_);
                func_decl_a.ident_ = (func_decl_a.ident_ + "_"
                                  + std::to_string(name_index_a));

                ++name_index_a;
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void FuncNameConverter::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)ast_a;
                (void)args_a;
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                (void)ast_a;
                (void)args_a;
            }

            IMPLEMENT_VISIT_PROC(SamplerDeclStmnt)
            {
                (void)ast_a;
                (void)args_a;
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                if (conversion_flags_(RenameMemberFunctions))
                {
                    if (auto struct_decl_ = ast_a->struct_decl_ref_)
                    {
                        ast_a->ident_.RemovePrefix(
                                name_mangling_.namespace_prefix_);
                       
                        ast_a->ident_ = struct_decl_->ident_ + "_"
                                        + ast_a->ident_;
                        
                        ast_a->ident_.AppendPrefix(
                                name_mangling_.namespace_prefix_);
                    }
                }

                if (conversion_flags_(RenameFunctionSignatures)
                    && !ast_a->IsForwardDecl())
                    func_decl_map_[ast_a->ident_].push_back(ast_a);
            }

#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
