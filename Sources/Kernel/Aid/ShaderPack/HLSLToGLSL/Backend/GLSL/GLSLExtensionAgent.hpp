// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../AST/ASTEnums.hpp"
#include "../../AST/Visitor/Visitor.hpp"
#include "../../Report/ReportHandler.hpp"
#include "../../Targets.hpp"

#include <map>
#include <set>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class GLSLExtensionAgent : private Visitor
            {
            public:
                std::set<std::string> DetermineRequiredExtensions(
                        Program& program_a,
                        OutputShaderVersion& target_glsl_version_a,
                        const ShaderTarget shader_target_a,
                        bool allow_extensions_a,
                        bool explicit_binding_a,
                        bool separate_shaders_a,
                        const OnReportProc& on_report_extension_a = nullptr);

            private:
                void EstablishIntrinsicExtMap();
                void AcquireExtension(const std::string& extension_a,
                                      const std::string& reason_a = "",
                                      const AST* ast_a = nullptr);

                DECL_VISIT_PROC(Program);
                DECL_VISIT_PROC(Attribute);

                DECL_VISIT_PROC(VarDecl);
                DECL_VISIT_PROC(BufferDecl);

                DECL_VISIT_PROC(FunctionDecl);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(BasicDeclStmnt);

                DECL_VISIT_PROC(BinaryExpr);
                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(InitializerExpr);

            private:
                ShaderTarget shader_target_ = ShaderTarget::Undefined;

                OutputShaderVersion target_glsl_version_ =
                        OutputShaderVersion::GLSL330;
                OutputShaderVersion min_glsl_version_ =
                        OutputShaderVersion::GLSL130;

                bool allow_extensions_ = false;
                bool explicit_binding_ = false;

                OnReportProc on_report_extension_;
                std::set<std::string> extensions_;
                std::map<Intrinsic, const char*> intrinsic_ext_map_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
