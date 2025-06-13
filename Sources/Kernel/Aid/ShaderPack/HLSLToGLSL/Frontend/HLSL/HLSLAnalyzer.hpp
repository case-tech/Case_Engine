// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../Flags.hpp"
#include "../../ShaderVersion.hpp"
#include "../../Variant.hpp"
#include "../Analyzer.hpp"

#include <map>
#include <set>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct HLSLIntrinsicEntry;

            class HLSLAnalyzer : public Analyzer
            {
            public:
                HLSLAnalyzer(Log* log_a = nullptr);

            private:
                using OnOverrideProc = ASTSymbolTable::OnOverrideProc;
                using OnValidAttributeValueProc =
                        std::function<bool(const AttributeValue)>;
                
                using OnAssignTypeDenoterProc =
                        std::function<void(const TypeDenoterPtr&)>;

                struct PrefixArgs
                {
                    bool in_is_postfix_static_;
                    StructDecl* out_prefix_base_struct_;
                };

                void DecorateASTPrAzary(
                        Program& program_a,
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a) override;

                void ErrorIfAttributeNotFound(bool found_a,
                                              const std::string& attrib_desc_a);

                bool IsD3D9ShaderModel() const;

                DECL_VISIT_PROC(CodeBlock);
                DECL_VISIT_PROC(Attribute);
                DECL_VISIT_PROC(ArrayDAzension);
                DECL_VISIT_PROC(TypeSpecifier);

                DECL_VISIT_PROC(VarDecl);
                DECL_VISIT_PROC(BufferDecl);
                DECL_VISIT_PROC(SamplerDecl);
                DECL_VISIT_PROC(StructDecl);
                DECL_VISIT_PROC(AliasDecl);
                DECL_VISIT_PROC(FunctionDecl);

                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(VarDeclStmnt);
                DECL_VISIT_PROC(BasicDeclStmnt);

                DECL_VISIT_PROC(CodeBlockStmnt);
                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(ElseStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);

                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(PostUnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(ArrayExpr);

                void AnalyzeVarDecl(VarDecl* var_decl_a);
                void AnalyzeVarDeclLocal(VarDecl* var_decl_a,
                                         bool register_var_ident_a = true);
                
                void AnalyzeVarDeclStaticMember(VarDecl* var_decl_a);
                void AnalyzeCallExpr(CallExpr* call_expr_a);
                void AnalyzeCallExprPrAzary(
                        CallExpr* call_expr_a,
                        const TypeDenoter* prefix_type_denoter_a = nullptr);
                
                void AnalyzeCallExprFunction(
                        CallExpr* call_expr_a,
                        bool is_static_a = false,
                        const Expr* prefix_expr_a = nullptr,
                        const TypeDenoter* prefix_type_denoter_a = nullptr);
                
                void AnalyzeCallExprIntrinsic(
                        CallExpr* call_expr_a,
                        const HLSLIntrinsicEntry& intr_a,
                        bool is_static_a = false,
                        const TypeDenoter* prefix_type_denoter_a = nullptr);
                
                void AnalyzeCallExprIntrinsicPrAzary(
                        CallExpr* call_expr_a,
                        const HLSLIntrinsicEntry& intr_a);
                
                void AnalyzeCallExprIntrinsicFromBufferType(
                        const CallExpr* call_expr_a,
                        const BufferType buffer_type_a);

                void AnalyzeIntrinsicWrapperInlining(CallExpr* call_expr_a);

                void AnalyzeObjectExpr(ObjectExpr* expr_a, PrefixArgs* args_a);
                void AnalyzeObjectExprVarDeclFromStruct(
                        ObjectExpr* expr_a,
                        StructDecl* base_struct_decl_a,
                        const StructTypeDenoter& struct_type_den_a);
               
                void AnalyzeObjectExprBaseStructDeclFromStruct(
                        ObjectExpr* expr_a,
                        PrefixArgs& output_args_a,
                        const StructTypeDenoter& struct_type_den_a);

                bool AnalyzeStaticAccessExpr(const Expr* prefix_expr_a,
                                             bool is_static_a,
                                             const AST* ast_a = nullptr);
                
                bool AnalyzeStaticTypeSpecifier(
                        const TypeSpecifier* type_specifier_a,
                        const std::string& ident_a,
                        const Expr* expr_a,
                        bool is_static_a);

                void AnalyzeLValueExpr(Expr* expr_a,
                                       const AST* ast_a = nullptr,
                                       VarDecl* param_a = nullptr);
                
                void AnalyzeLValueExprObject(ObjectExpr* object_expr_a,
                                             const AST* ast_a = nullptr,
                                             VarDecl* param_a = nullptr);

                void AnalyzeArrayExpr(ArrayExpr* expr_a);

                void AnalyzeEntryPoint(FunctionDecl* func_decl_a);
                void AnalyzeEntryPointInputOutput(FunctionDecl* func_decl_a);
                void AnalyzeEntryPointParameter(FunctionDecl* func_decl_a,
                                                VarDeclStmnt* param_a);
                
                void AnalyzeEntryPointParameterInOut(
                        FunctionDecl* func_decl_a,
                        VarDecl* var_decl_a,
                        bool input_a,
                        TypeDenoterPtr var_type_den_a = nullptr);
                
                void AnalyzeEntryPointParameterInOutVariable(
                        FunctionDecl* func_decl_a,
                        VarDecl* var_decl_a,
                        bool input_a);
                
                void AnalyzeEntryPointParameterInOutStruct(
                        FunctionDecl* func_decl_a,
                        StructDecl* struct_decl_a,
                        bool input_a);
                
                void AnalyzeEntryPointParameterInOutBuffer(
                        FunctionDecl* func_decl_a,
                        VarDecl* var_decl_a,
                        BufferTypeDenoter* buffer_type_den_a,
                        bool input_a);

                void AnalyzeEntryPointAttributes(
                        const std::vector<AttributePtr>& attribs_a);
                void AnalyzeEntryPointAttributesTessControlShader(
                        const std::vector<AttributePtr>& attribs_a);
                void AnalyzeEntryPointAttributesTessEvaluationShader(
                        const std::vector<AttributePtr>& attribs_a);
                void AnalyzeEntryPointAttributesGeometryShader(
                        const std::vector<AttributePtr>& attribs_a);
                void AnalyzeEntryPointAttributesFragmentShader(
                        const std::vector<AttributePtr>& attribs_a);
                void AnalyzeEntryPointAttributesComputeShader(
                        const std::vector<AttributePtr>& attribs_a);

                void AnalyzeEntryPointSemantics(
                        FunctionDecl* func_decl_a,
                        const std::vector<Semantic>& in_semantics_a,
                        const std::vector<Semantic>& out_semantics_a);

                void AnalyzeEntryPointOutput(Expr* expr_a);

                void AnalyzeSecondaryEntryPoint(
                        FunctionDecl* func_decl_a,
                        bool is_patch_constant_func_a = false);
                
                void AnalyzeSecondaryEntryPointAttributes(
                        const std::vector<AttributePtr>& attribs_a);
                void AnalyzeSecondaryEntryPointAttributesTessEvaluationShader(
                        const std::vector<AttributePtr>& attribs_a);

                bool AnalyzeNumArgsAttribute(Attribute* attrib_a,
                                             std::size_t min_num_args_a,
                                             std::size_t max_num_args_a,
                                             bool required_a);
                
                bool AnalyzeNumArgsAttribute(Attribute* attrib_a,
                                             std::size_t expected_num_args_a,
                                             bool required_a = true);

                void AnalyzeAttributeDomain(Attribute* attrib_a,
                                            bool required_a = true);
                
                void AnalyzeAttributeOutputTopology(Attribute* attrib_a,
                                                    bool required_a = true);
                
                void AnalyzeAttributePartitioning(Attribute* attrib_a,
                                                  bool required_a = true);
                
                void AnalyzeAttributeOutputControlPoints(Attribute* attrib_a);
                void AnalyzeAttributePatchConstantFunc(Attribute* attrib_a);

                void AnalyzeAttributeMaxVertexCount(Attribute* attrib_a);

                void AnalyzeAttributeNumThreads(Attribute* attrib_a);
                void AnalyzeAttributeNumThreadsArgument(Expr* expr_a,
                                                        unsigned int& value_a);

                void AnalyzeAttributeValue(
                        Expr* arg_expr_a,
                        AttributeValue& value_a,
                        const OnValidAttributeValueProc& expected_value_func_a,
                        const std::string& expectation_desc_a,
                        bool required_a = true);

                bool AnalyzeAttributeValuePrAzary(
                        Expr* arg_expr_a,
                        AttributeValue& value_a,
                        const OnValidAttributeValueProc& expected_value_func_a,
                        std::string& literal_value_a);

                void AnalyzeSemantic(IndexedSemantic& semantic_a);
                void AnalyzeSemanticSM3(IndexedSemantic& semantic_a, bool input_a);
                void AnalyzeSemanticSM3Remaining();
                void AnalyzeSemanticVarDecl(IndexedSemantic& semantic_a,
                                            VarDecl* var_decl_a);
                
                void AnalyzeSemanticFunctionReturn(IndexedSemantic& semantic_a);
                void VisitProgram(Program* ast_a, void* args_a);

                void AnalyzeArrayDAzensionList(
                        const std::vector<ArrayDAzensionPtr>& array_dAzs_a);
                
                void AnalyzeParameter(VarDeclStmnt* param_a);

            private:
                Program* program_ = nullptr;

                std::string entry_point_;
                std::string secondary_entry_point_;
                bool secondary_entry_point_found_ = false;

                ShaderTarget shader_target_ = ShaderTarget::VertexShader;
                InputShaderVersion version_in_ = InputShaderVersion::HLSL5;
                ShaderVersion shader_model_ = {5, 0};
                bool prefer_wrappers_ = false;

                std::set<VarDecl*> var_decl_sm3_semantics_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
