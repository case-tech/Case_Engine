// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../AST/AST.hpp"
#include "../../AST/ASTEnums.hpp"
#include "../../AST/Token.hpp"
#include "../../AST/Visitor/Visitor.hpp"
#include "../../CiString.hpp"
#include "../../Flags.hpp"
#include "../../HTG.hpp"
#include "../Generator.hpp"

#include <functional>
#include <initializer_list>
#include <map>
#include <set>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct TypeDenoter;
            struct BaseTypeDenoter;

            class GLSLGenerator : public Generator
            {
            public:
                GLSLGenerator(Log* log_a);

            private:
                using LayoutEntryFunctor = std::function<void()>;

                void GenerateCodePrimary(
                        Program& program_a,
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a) override;

                std::unique_ptr<std::string> SystemValueToKeyword(
                        const IndexedSemantic& semantic_a) const;

                bool IsWrappedIntrinsic(const Intrinsic intrinsic_a) const;
                bool IsGLSL() const;
                bool IsESSL() const;
                bool IsVKSL() const;
                bool IsGLSL120OrESSL100() const;
                bool HasShadingLanguage420Pack() const;
                bool UseSeparateSamplers() const;

                const std::string* BufferTypeToKeyword(
                        const BufferType buffer_type_a,
                        const AST* ast_a = nullptr);
                
                const std::string* SamplerTypeToKeyword(
                        const SamplerType sampler_type_a,
                        const AST* ast_a = nullptr);

                bool IsTypeCompatibleWithSemantic(
                        const Semantic semantic_a,
                        const TypeDenoter& type_denoter_a);

                void ReportOptionalFeedback();
                void ErrorIntrinsic(const std::string& intrinsic_name_a,
                                    const AST* ast_a = nullptr);

                int GetNumBindingLocations(const TypeDenoter* type_denoter_a);
                int GetBindingLocation(const TypeDenoter* type_denoter_a,
                                       bool input_a);

                DECL_VISIT_PROC(Program);
                DECL_VISIT_PROC(CodeBlock);
                DECL_VISIT_PROC(SwitchCase);
                DECL_VISIT_PROC(ArrayDimension);
                DECL_VISIT_PROC(TypeSpecifier);

                DECL_VISIT_PROC(VarDecl);
                DECL_VISIT_PROC(StructDecl);
                DECL_VISIT_PROC(SamplerDecl);

                DECL_VISIT_PROC(FunctionDecl);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(SamplerDeclStmnt);
                DECL_VISIT_PROC(VarDeclStmnt);
                DECL_VISIT_PROC(AliasDeclStmnt);
                DECL_VISIT_PROC(BasicDeclStmnt);

                DECL_VISIT_PROC(NullStmnt);
                DECL_VISIT_PROC(CodeBlockStmnt);
                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(ElseStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);
                DECL_VISIT_PROC(CtrlTransferStmnt);

                DECL_VISIT_PROC(SequenceExpr);
                DECL_VISIT_PROC(LiteralExpr);
                DECL_VISIT_PROC(TypeSpecifierExpr);
                DECL_VISIT_PROC(TernaryExpr);
                DECL_VISIT_PROC(BinaryExpr);
                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(PostUnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(BracketExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(ArrayExpr);
                DECL_VISIT_PROC(CastExpr);
                DECL_VISIT_PROC(InitializerExpr);

                void PreProcessAST(const ShaderInput& input_desc_a,
                                   const ShaderOutput& output_desc_a);
                
                void PreProcessStructParameterAnalyzer(
                        const ShaderInput& input_desc_a);
                
                void PreProcessTypeConverter();
                void PreProcessExprConverterPrimary();
                void PreProcessGLSLConverter(const ShaderInput& input_desc_a,
                                             const ShaderOutput& output_desc_a);
                
                void PreProcessFuncNameConverter();
                void PreProcessReferenceAnalyzer(const ShaderInput& input_desc_a);
                void PreProcessExprConverterSecondary();
                void PreProcessPackedUniforms();

                void WriteComment(const std::string& text_a);

                void WriteLineMark(int line_number_a);
                void WriteLineMark(const TokenPtr& tkn_a);
                void WriteLineMark(const AST* ast_a);

                void WriteProgramHeader();
                void WriteProgramHeaderVersion();
                void WriteProgramHeaderComment();
                void WriteProgramHeaderExtension(
                        const std::string& extension_name_a);

                void WriteGlobalLayouts();
                bool WriteGlobalLayoutsTessControl(
                        const Program::LayoutTessControlShader& layout_a);
                
                bool WriteGlobalLayoutsTessEvaluation(
                        const Program::LayoutTessEvaluationShader& layout_a);
                
                bool WriteGlobalLayoutsGeometry(
                        const Program::LayoutGeometryShader& layout_a);
                
                bool WriteGlobalLayoutsFragment(
                        const Program::LayoutFragmentShader& layout_a);
                
                bool WriteGlobalLayoutsCompute(
                        const Program::LayoutComputeShader& layout_a);

                void WriteBuiltinBlockRedeclarations();
                void WriteBuiltinBlockRedeclarationsPerVertex(
                        bool input_a,
                        const std::string& name_a = "");

                void WriteLayout(
                        const std::initializer_list<LayoutEntryFunctor>&
                                entry_functors_a);
               
                void WriteLayout(const std::string& value_a);
                void WriteLayoutGlobal(
                        const std::initializer_list<LayoutEntryFunctor>&
                                entry_functors_a,
                        const LayoutEntryFunctor& var_functor_a,
                        const std::string& modifier_a);
                
                void WriteLayoutGlobalIn(
                        const std::initializer_list<LayoutEntryFunctor>&
                                entry_functors_a,
                        const LayoutEntryFunctor& var_functor_a = nullptr);
                
                void WriteLayoutGlobalOut(
                        const std::initializer_list<LayoutEntryFunctor>&
                                entry_functors_a,
                        const LayoutEntryFunctor& var_functor_a = nullptr);
                
                void WriteLayoutBinding(
                        const std::vector<RegisterPtr>& slot_registers_a);
                
                void WriteLayoutBindingOrLocation(
                        const std::vector<RegisterPtr>& slot_registers_a);

                void WriteLocalInputSemantics(FunctionDecl* entry_point_a);
                void WriteLocalInputSemanticsVarDecl(VarDecl* var_decl_a);
                void WriteLocalInputSemanticsStructDeclParam(
                        VarDeclStmnt* param_a,
                        StructDecl* struct_decl_a);

                void WriteGlobalInputSemantics(FunctionDecl* entry_point_a);
                void WriteGlobalInputSemanticsVarDecl(VarDecl* var_decl_a);

                void WriteLocalOutputSemantics(FunctionDecl* entry_point_a);
                void WriteLocalOutputSemanticsStructDeclParam(
                        VarDeclStmnt* param_a,
                        StructDecl* struct_decl_a);

                void WriteGlobalOutputSemantics(FunctionDecl* entry_point_a);
                void WriteGlobalOutputSemanticsVarDecl(
                        VarDecl* var_decl_a,
                        bool use_semantic_name_a = false);
                
                void WriteGlobalOutputSemanticsSlot(
                        TypeSpecifier* type_specifier_a,
                        IndexedSemantic& semantic_a,
                        const std::string& ident_a,
                        VarDecl* var_decl_a = nullptr);

                void WriteOutputSemanticsAssignment(
                        Expr* expr_a,
                        bool write_as_listed_expr_a = false);
                
                void WriteOutputSemanticsAssignmentStructDeclParam(
                        const FunctionDecl::ParameterStructure& param_struct_a,
                        bool write_as_listed_expr_a = false,
                        const std::string& temp_ident_a = "output");

                void WriteGlobalUniforms();
                void WriteGlobalUniformsParameter(VarDeclStmnt* param_a);
                void WriteVarDeclIdentOrSystemValue(VarDecl* var_decl_a,
                                                    int array_index_a = -1);

                void WriteObjectExpr(const ObjectExpr& object_expr_a);
                void WriteObjectExprIdent(const ObjectExpr& object_expr_a,
                                          bool write_prefix_a = true);
                
                void WriteObjectExprIdentOrSystemValue(
                        const ObjectExpr& object_expr_a,
                        Decl* symbol_a);

                void WriteArrayExpr(const ArrayExpr& array_expr_a);
                void WriteArrayIndices(
                        const std::vector<ExprPtr>& array_indices_a);

                void WriteStorageClasses(
                        const std::set<StorageClass>& storage_ñlasses_a,
                        const AST* ast_a = nullptr);
               
                void WriteInterpModifiers(
                        const std::set<InterpModifier>& interp_modifiers_a,
                        const AST* ast_a = nullptr);
                
                void WriteTypeModifiers(
                        const std::set<TypeModifier>& type_modifiers_a,
                        const TypeDenoterPtr& type_denoter_a = nullptr);
                
                void WriteTypeModifiersFrom(
                        const TypeSpecifierPtr& type_specifier_a);

                void WriteDataType(DataType data_type_a,
                                   bool write_precision_specifier_a = false,
                                   const AST* ast_a = nullptr);
                
                void WriteTypeDenoter(const TypeDenoter& type_denoter_a,
                                      bool write_precision_specifier_a = false,
                                      const AST* ast_a = nullptr);

                void WriteFunction(FunctionDecl* ast_a);
                void WriteFunctionEntryPoint(FunctionDecl* ast_a);
                void WriteFunctionEntryPointBody(FunctionDecl* ast_a);
                void WriteFunctionSecondaryEntryPoint(FunctionDecl* ast_a);

                void AssertIntrinsicNumArgs(CallExpr* call_expr_a,
                                            std::size_t num_args_min_a,
                                            std::size_t num_args_max_a = ~0);

                void WriteCallExprStandard(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicMul(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicRcp(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicClip(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicAtomic(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicAtomicCompSwap(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicImageAtomic(CallExpr* call_expr_a);
                void WriteCallExprIntrinsicImageAtomicCompSwap(
                        CallExpr* call_expr_a);
                
                void WriteCallExprIntrinsicStreamOutputAppend(
                        CallExpr* call_expr_a);
                
                void WriteCallExprIntrinsicTextureQueryLod(CallExpr* call_expr_a,
                                                           bool clamped_a);

                void WriteCallExprArguments(CallExpr* call_expr_a,
                                            std::size_t first_arg_index_a = 0,
                                            std::size_t num_write_args_a = ~0u);

                void WriteWrapperIntrinsics();
                void WriteWrapperIntrinsicsClip(const IntrinsicUsage& usage_a);
                void WriteWrapperIntrinsicsLit(const IntrinsicUsage& usage_a);
                void WriteWrapperIntrinsicsSinCos(const IntrinsicUsage& usage_a);
                void WriteWrapperIntrinsicsMemoryBarrier(
                        const Intrinsic intrinsic_a,
                        bool group_sync_a);
                
                void WriteWrapperIntrinsicsF16toF32();
                void WriteWrapperMatrixSubscript(
                        const MatrixSubscriptUsage& usage_a);

                bool WriteStructDecl(StructDecl* struct_decl_a,
                                     bool end_with_semicolon_a);

                void WriteBufferDecl(BufferDecl* buffer_decl_a);
                void WriteBufferDeclTexture(BufferDecl* buffer_decl_a);
                void WriteBufferDeclStorageBuffer(BufferDecl* buffer_decl_a);

                void WriteSamplerDecl(SamplerDecl& sampler_decl_a);
                void WriteStmntComment(Stmnt* ast_a, bool insert_blank_a = false);

                template <typename T>
                void WriteStmntList(const std::vector<T>& stmnts_a,
                                    bool is_global_scope_a = false);

                void WriteParameter(VarDeclStmnt* ast_a);
                void WriteScopedStmnt(Stmnt* ast_a);

                void WriteLiteral(const std::string& value_a,
                                  const DataType& data_type_a,
                                  const AST* ast_a = nullptr);

                struct VertexSemanticLoc
                {
                    int location_;
                    bool found_;
                };

            private:
                OutputShaderVersion version_out_ = OutputShaderVersion::GLSL;
                NameMangling name_mangling_;
                std::map<CiString, VertexSemanticLoc> vertex_semantics_map_;
                UniformPacking uniform_packing_;
                std::string entry_point_name_;

                bool allow_extensions_ = false;
                bool explicit_binding_ = false;
                bool preserve_comments_ = false;
                bool allow_line_marks_ = false;
                bool compact_wrappers_ = false;
                bool always_braced_scopes_ = false;
                bool separate_shaders_ = false;
                bool separate_samplers_ = true;
                bool auto_binding_ = false;
                bool write_header_comment_ = true;

                std::set<int> used_in_locations_set_;
                std::set<int> used_out_locations_set_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
