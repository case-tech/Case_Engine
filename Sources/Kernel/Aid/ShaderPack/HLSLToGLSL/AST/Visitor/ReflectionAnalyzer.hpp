#include "../../Reflection.hpp"
#include "../../Report/ReportHandler.hpp"
#include "../../Targets.hpp"
#include "../../Variant.hpp"
#include "../Token.hpp"
#include "../TypeDenoter.hpp"
#include "Visitor.hpp"

#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ReflectionAnalyzer : private Visitor
            {
            public:
                ReflectionAnalyzer(Log* log_a);

                void Reflect(Program& program_a,
                             const ShaderTarget shader_target_a,
                             Reflection::ReflectionData& reflection_data_a,
                             bool enable_warnings_a);

            private:
                void Warning(const std::string& msg_a, const AST* ast_a = nullptr);

                int GetBindingPoint(
                        const std::vector<RegisterPtr>& slot_registers_a) const;

                int EvaluateConstExprInt(Expr& expr_a);
                float EvaluateConstExprFloat(Expr& expr_a);

                DECL_VISIT_PROC(Program);

                DECL_VISIT_PROC(SamplerDecl);
                DECL_VISIT_PROC(StructDecl);

                DECL_VISIT_PROC(FunctionDecl);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(BufferDeclStmnt);

                DECL_VISIT_PROC(VarDecl);

                void ReflectSamplerValue(SamplerValue* ast_a,
                                         Reflection::SamplerStateDesc& desc_a);
                
                void ReflectSamplerValueFilter(const std::string& value_a,
                                               Reflection::Filter& filter_a,
                                               const AST* ast_a = nullptr);
                
                void ReflectSamplerValueTextureAddressMode(
                        const std::string& value_a,
                        Reflection::TextureAddressMode& address_mode_a,
                        const AST* ast_a = nullptr);
                
                void ReflectSamplerValueComparisonFunc(
                        const std::string& value_a,
                        Reflection::ComparisonFunc& comparison_func_a,
                        const AST* ast_a = nullptr);

                void ReflectAttributes(
                        const std::vector<AttributePtr>& attribs_a);
                
                void ReflectAttributesNumThreads(Attribute* ast_a);
                void ReflectField(VarDecl* ast_a,
                                  Reflection::Field& field_a,
                                  unsigned int& accum_size_a,
                                  unsigned int& accum_padding_a);
               
                void ReflectFieldType(Reflection::Field& field_a,
                                      const TypeDenoter& type_den_a);
                
                int FindRecordIndex(const StructDecl* struct_decl_a) const;

            private:
                ReportHandler report_handler_;

                ShaderTarget shader_target_ = ShaderTarget::VertexShader;
                Program* program_ = nullptr;

                Reflection::ReflectionData* data_ = nullptr;

                bool enable_warnings_ = false;

                std::map<const StructDecl*, std::size_t> record_indices_map_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel