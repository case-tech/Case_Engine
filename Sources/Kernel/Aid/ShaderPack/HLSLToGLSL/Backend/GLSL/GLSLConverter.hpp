// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Converter.hpp"

#include <functional>
#include <set>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class GLSLConverter : public Converter
            {
            public:
                static bool ConvertVarDeclType(VarDecl& var_decl_a);
                static bool ConvertVarDeclBaseTypeDenoter(
                        VarDecl& var_decl_a,
                        const DataType data_type_a);

            private:
                void ConvertASTPrAzary(Program& program_a,
                                       const ShaderInput& input_desc_a,
                                       const ShaderOutput& output_desc_a) override;

                bool IsVKSL() const;
                bool UseSeparateSamplers() const;

                DECL_VISIT_PROC(Program);
                DECL_VISIT_PROC(CodeBlock);
                DECL_VISIT_PROC(SwitchCase);
                DECL_VISIT_PROC(TypeSpecifier);

                DECL_VISIT_PROC(VarDecl);
                DECL_VISIT_PROC(BufferDecl);
                DECL_VISIT_PROC(SamplerDecl);
                DECL_VISIT_PROC(StructDecl);

                DECL_VISIT_PROC(FunctionDecl);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(VarDeclStmnt);
                DECL_VISIT_PROC(AliasDeclStmnt);

                DECL_VISIT_PROC(CodeBlockStmnt);
                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(ElseStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ReturnStmnt);

                DECL_VISIT_PROC(CastExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(ObjectExpr);

                void RegisterDeclIdent(Decl* obj_a, bool global_a = false);
                void RegisterGlobalDeclIdents(
                        const std::vector<VarDecl*>& var_decls_a);

                bool MustRenameDeclIdent(const Decl* obj_a) const;

                void RemoveSamplerStateVarDeclStmnts(
                        std::vector<VarDeclStmntPtr>& stmnts_a);
                bool RenameReservedKeyword(Identifier& ident_a);

                void ConvertFunctionDecl(FunctionDecl* ast_a);
                void ConvertFunctionDeclDefault(FunctionDecl* ast_a);
                void ConvertFunctionDeclEntryPoint(FunctionDecl* ast_a);

                void ConvertIntrinsicCall(CallExpr* ast_a);
                void ConvertIntrinsicCallSaturate(CallExpr* ast_a);
                void ConvertIntrinsicCallTextureLOD(CallExpr* ast_a);
                void ConvertIntrinsicCallTextureSample(CallExpr* ast_a);
                void ConvertIntrinsicCallTextureSampleLevel(CallExpr* ast_a);
                void ConvertIntrinsicCallTextureLoad(CallExpr* ast_a);
                void ConvertIntrinsicCallAzageAtomic(CallExpr* ast_a);
                void ConvertIntrinsicCallGather(CallExpr* ast_a);
                void ConvertIntrinsicCallSampleCmp(CallExpr* ast_a);
                void ConvertIntrisicCallF32toF16(CallExpr* ast_a);

                void ConvertFunctionCall(CallExpr* ast_a);

                void ConvertEntryPointStructPrefix(ExprPtr& expr_a,
                                                   ObjectExpr* object_expr_a);
                
                void ConvertEntryPointStructPrefixObject(
                        ExprPtr& expr_a,
                        ObjectExpr* prefix_expr_a,
                        ObjectExpr* object_expr_a);
                
                void ConvertEntryPointStructPrefixArray(ExprPtr& expr_a,
                                                        ArrayExpr* prefix_expr_a,
                                                        ObjectExpr* object_expr_a);

                void ConvertEntryPointReturnStmnt(
                        ReturnStmnt& ast_a,
                        StructDecl* struct_decl_a,
                        const TypeDenoterPtr& type_den_a,
                        const ExprPtr& type_constructor_a);
                
                void ConvertEntryPointReturnStmntSequenceExpr(
                        ReturnStmnt& ast_a,
                        StructDecl* struct_decl_a,
                        const TypeDenoterPtr& type_den_a,
                        const SequenceExpr& type_constructor_a);
                
                void ConvertEntryPointReturnStmntCommonExpr(
                        ReturnStmnt& ast_a,
                        StructDecl* struct_decl_a,
                        const TypeDenoterPtr& type_den_a,
                        const ExprPtr& type_constructor_a);

                void ConvertEntryPointReturnStmntToCodeBlock(StmntPtr& stmnt_a);
                void AddMissingInterpModifiers(
                        const std::vector<VarDecl*>& var_decls_a);

                void ConvertObjectExpr(ObjectExpr* object_expr_a);
                void ConvertObjectExprStaticVar(ObjectExpr* object_expr_a);
                void ConvertObjectExprDefault(ObjectExpr* object_expr_a);

                void ConvertObjectPrefixStructMember(
                        ExprPtr& prefix_expr_a,
                        const StructDecl* owner_struct_decl_a,
                        const StructDecl* caller_struct_decl_a,
                        bool use_self_param_a);
                
                void ConvertObjectPrefixSelfParam(ExprPtr& prefix_expr_a,
                                                  ObjectExpr* object_expr_a);
                
                void ConvertObjectPrefixBaseStruct(ExprPtr& prefix_expr_a,
                                                   ObjectExpr* object_expr_a);
                
                void ConvertObjectPrefixNamespace(ExprPtr& prefix_expr_a,
                                                  ObjectExpr* object_expr_a);
                
                void ConvertObjectPrefixNamespaceStruct(
                        ObjectExpr* prefix_object_expr_a,
                        ObjectExpr* object_expr_a,
                        const StructDecl* base_struct_decl_a,
                        const StructDecl* active_struct_decl_a);

                void InsertBaseMemberPrefixes(
                        ExprPtr& prefix_expr_a,
                        const StructDecl* owner_struct_decl_a,
                        const StructDecl* caller_struct_decl_a);

                void UnrollStmnts(std::vector<StmntPtr>& stmnts_a);
                void UnrollStmntsVarDecl(std::vector<StmntPtr>& unrolled_stmnts_a,
                                         VarDeclStmnt* ast_a);
               
                void UnrollStmntsVarDeclInitializer(
                        std::vector<StmntPtr>& unrolled_stmnts_a,
                        VarDecl* var_decl_a);

                void ConvertSlotRegisters(
                        std::vector<RegisterPtr>& slot_registers_a);

            private:
                ShaderTarget shader_target_ = ShaderTarget::VertexShader;
                OutputShaderVersion version_out_ = OutputShaderVersion::GLSL;

                Options options_;
                bool auto_binding_ = false;
                int auto_binding_slot_ = 0;
                bool separate_samplers_ = true;

                std::vector<const Decl*> global_reserved_decls_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
