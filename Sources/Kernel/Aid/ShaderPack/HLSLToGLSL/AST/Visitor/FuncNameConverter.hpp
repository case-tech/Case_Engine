// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../Flags.hpp"
#include "../../HTG.hpp"
#include "VisitorTracker.hpp"

#include <functional>
#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class FuncNameConverter : public VisitorTracker
            {
            public:
                enum : unsigned int
                {
                    RenameMemberFunctions = (1 << 0),
                    RenameFunctionSignatures = (1 << 1),

                    All = (RenameMemberFunctions | RenameFunctionSignatures),
                };

                using OnFuncSigantureCompare = std::function<
                        bool(const FunctionDecl& lhs_, const FunctionDecl& rhs_)>;

                void Convert(
                        Program& program_a,
                        const NameMangling& name_mangling_a,
                        const OnFuncSigantureCompare& on_func_signature_compare_a,
                        const Flags& conversion_flags_a);

            private:
                using FuncList = std::vector<FunctionDecl*>;

                void ConvertEqualFunctionSignatures(FuncList& func_list_a);
                void ConvertMemberFunctionName(FunctionDecl& func_decl_a,
                                               unsigned int& name_index_a);

                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(SamplerDeclStmnt);

                DECL_VISIT_PROC(FunctionDecl);

            private:
                NameMangling name_mangling_;
                OnFuncSigantureCompare on_func_signature_compare_;
                Flags conversion_flags_;

                std::map<std::string, FuncList> func_decl_map_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
