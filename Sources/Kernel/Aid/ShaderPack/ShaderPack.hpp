// Copyright (c) 2025 Case Technologies

#pragma once
#include <cstdint>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#endif

#include "HLSLToGLSL/HTG.hpp"
#include <GL/glew.h>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ShaderPack
            {
            public:
                static void CreateShaderPack(const std::string& hlsl_path_a,
                                             const std::string& entry_point_a,
                                             ShaderTarget target_a);

                static std::vector<uint8_t> LoadHLSLSection(
                        const std::string& shader_pack_path_a);

                static std::vector<uint8_t> LoadGLSLSection(
                        const std::string& shader_pack_path_a);

            private:
                static std::vector<uint8_t> CompileHLSL(
                        const std::string& source_a,
                        const std::string& entry_a,
                        ShaderTarget target_a);

                static std::vector<uint8_t> CompileGLSL(
                        const std::string& source_a,
                        ShaderTarget target_a);

                static void WriteShaderPack(
                        const std::string& path_a,
                        const std::vector<uint8_t>& hlsl_binary_a,
                        const std::vector<uint8_t>& glsl_binary_a);

                static std::vector<uint8_t> ReadShaderSection(
                        const std::string& path_a,
                        const char* section_tag_a);
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel