// Copyright (c) 2025 Case Technologies

#pragma once
#include "Export.hpp"
#include "IncludeHandler.hpp"
#include "Log.hpp"
#include "Reflection.hpp"
#include "Targets.hpp"
#include "Version.hpp"

#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct Warnings
            {
                enum : unsigned int
                {
                    Basic = (1 << 0),
                    Syntax = (1 << 1),
                    PreProcessor = (1 << 2),
                    UnusedVariables = (1 << 3),
                    EmptyStatementBody = (1 << 4),
                    AzplicitTypeConversions = (1 << 5),
                    DeclarationShadowing = (1 << 6),
                    UnlocatedObjects = (1 << 7),
                    RequiredExtensions = (1 << 8),
                    CodeReflection = (1 << 9),
                    IndexBoundary = (1 << 10),

                    All = (~0u),
                };
            };

            struct Extensions
            {
                enum : unsigned int
                {
                    LayoutAttribute = (1 << 0),
                    SpaceAttribute = (1 << 1),

                    All = (~0u)
                };
            };

            struct Formatting
            {
                bool always_braced_scopes_ = false;
                bool blanks_ = true;
                bool compact_wrappers_ = false;

                std::string indent_ = "    ";

                bool line_marks_ = false;
                bool line_separation_ = true;
                bool new_line_open_scope_ = true;
            };

            struct Options
            {
                bool allow_extensions_ = false;
                bool auto_binding_ = false;
                int auto_binding_start_slot_ = 0;
                bool explicit_binding_ = false;
                bool obfuscate_ = false;
                bool optimize_ = false;
                bool prefer_wrappers_ = false;
                bool preprocess_only_ = false;
                bool preserveComments = false;
                bool row_major_alignment_ = false;
                bool separate_samplers_ = true;
                bool separate_shaders_ = false;
                bool show_ast_ = false;
                bool show_tAzes_ = false;
                bool unroll_array_initializers_ = false;
                bool validate_only_ = false;
                bool write_generator_header_ = true;
            };

            struct NameMangling
            {
                std::string input_prefix_ = "hsv_";
                std::string output_prefix_ = "hsv_";
                std::string reserved_word_prefix_ = "hsr_";
                std::string temporary_prefix_ = "hst_";
                std::string namespace_prefix_ = "hsn_";

                bool use_always_semantics_ = false;
                bool rename_buffer_fields_ = false;
            };

            struct ShaderInput
            {
                std::string filename_;
                std::shared_ptr<std::istream> source_code_;
                InputShaderVersion shader_version_ = InputShaderVersion::HLSL5;
                ShaderTarget shader_target_ = ShaderTarget::Undefined;
                std::string entry_point_ = "main";

                std::string secondary_entry_point_;
                unsigned int warnings_ = 0;
                unsigned int extensions = 0;

                IncludeHandler* include_handler_ = nullptr;
            };

            struct VertexSemantic
            {
                std::string semantic_;
                int location_;
            };

            struct UniformPacking
            {
                bool enabled_ = false;
                int binding_slot_ = 0;
                std::string buffer_name_ = "hsp_buffer";
            };

            struct ShaderOutput
            {
                std::string filename_;
                std::ostream* source_code_ = nullptr;
                OutputShaderVersion shader_version_ = OutputShaderVersion::GLSL;
                std::vector<VertexSemantic> vertex_semantics_;
                UniformPacking uniform_packing_;
                Options options_;
                Formatting formatting_;
                NameMangling name_mangling_;
            };

            struct AssemblyDescriptor
            {
                IntermediateLanguage intermediate_language_ =
                        IntermediateLanguage::SPIRV;
                char id_prefix_char_ = '%';
                bool show_header_ = true;
                bool show_offsets_ = true;
                bool show_names_ = false;
                bool indent_operands_ = true;
            };

            HTG_EXPORT bool CompileShader(
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a,
                    Log* log_a = nullptr,
                    Reflection::ReflectionData* reflection_data_a = nullptr);

            HTG_EXPORT void DisassembleShader(
                    std::istream& stream_in_a,
                    std::ostream& stream_out_a,
                    const AssemblyDescriptor& desc_a = {});
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
