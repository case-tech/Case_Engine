#include "ShaderPack.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void ShaderPack::CreateShaderPack(const std::string& hlsl_path_a,
                                              const std::string& entry_point_a,
                                              ShaderTarget target_a)
            {
                std::ifstream hlsl_file_(hlsl_path_a, std::ios::binary);
                if (!hlsl_file_.is_open())
                {
                    throw std::runtime_error("Failed to open HLSL file: "
                                             + hlsl_path_a);
                }

                std::stringstream hlsl_buffer_;
                hlsl_buffer_ << hlsl_file_.rdbuf();
                hlsl_file_.close();
                std::string hlsl_source_ = hlsl_buffer_.str();

                auto input_stream_ =
                        std::make_shared<std::ifstream>(hlsl_path_a,
                                                        std::ios::binary);
                std::stringstream glsl_output_;

                ShaderInput input_desc_;
                input_desc_.source_code_ = input_stream_;
                input_desc_.shader_version_ = InputShaderVersion::HLSL5;
                input_desc_.entry_point_ = entry_point_a;
                input_desc_.shader_target_ = target_a;

                ShaderOutput output_desc_;
                output_desc_.source_code_ = &glsl_output_;

                StdLog log_;
                try
                {
                    CompileShader(input_desc_, output_desc_, &log_, nullptr);
                } 
                catch (const std::exception& e_)
                {
                    throw std::runtime_error("HLSL to GLSL translation failed: "
                                             + std::string(e_.what()));
                }

                std::string glsl_source_ = glsl_output_.str();
                input_stream_->close();

                std::vector<uint8_t> hlsl_binary_;
#if defined(_WIN32) || defined(_WIN64)
                hlsl_binary_ = CompileHLSL(hlsl_source_, entry_point_a, target_a);
#endif

                std::vector<uint8_t> glsl_binary_ = CompileGLSL(glsl_source_,
                                                              target_a);
                std::string output_file_ =
                        hlsl_path_a.substr(0, hlsl_path_a.find_last_of('.'))
                        + ".shader";
                
                WriteShaderPack(output_file_, hlsl_binary_, glsl_binary_);
            }

            std::vector<uint8_t> ShaderPack::LoadHLSLSection(
                    const std::string& shader_pack_path_a)
            {
                return ReadShaderSection(shader_pack_path_a, "HLSL");
            }

            std::vector<uint8_t> ShaderPack::LoadGLSLSection(
                    const std::string& shader_pack_path_a)
            {
                return ReadShaderSection(shader_pack_path_a, "GLSL");
            }

            std::vector<uint8_t> ShaderPack::CompileHLSL(
                    const std::string& source_a,
                    const std::string& entry_a,
                    ShaderTarget target_a)
            {
#if defined(_WIN32) || defined(_WIN64)
                const char* target_str_ = nullptr;
                switch (target_a)
                {
                case ShaderTarget::VertexShader:
                    target_str_ = "vs_5_0";
                    break;
                case ShaderTarget::FragmentShader:
                    target_str_ = "ps_5_0";
                    break;
                default:
                    throw std::runtime_error("Unsupported shader target");
                }

                ID3DBlob* blob_ = nullptr;
                ID3DBlob* error_ = nullptr;

                HRESULT hr_ = D3DCompile(source_a.c_str(),
                                         source_a.size(),
                                         nullptr,
                                         nullptr,
                                         nullptr,
                                         entry_a.c_str(),
                                         target_str_,
                                         D3DCOMPILE_OPTIMIZATION_LEVEL3,
                                         0,
                                         &blob_,
                                         &error_);

                if (FAILED(hr_))
                {
                    if (error_)
                    {
                        std::string msg_(static_cast<char*>(
                                                error_->GetBufferPointer()),
                                                error_->GetBufferSize());
                        error_->Release();
                        throw std::runtime_error("HLSL compilation failed: "
                                                 + msg_);
                    }
                    throw std::runtime_error(
                            "HLSL compilation failed without error message");
                }

                std::vector<uint8_t> binary_(blob_->GetBufferSize());
                memcpy(binary_.data(), blob_->GetBufferPointer(), binary_.size());
                blob_->Release();
                return binary_;
#else
                return std::vector<uint8_t>();
#endif
            }

            std::vector<uint8_t> ShaderPack::CompileGLSL(
                    const std::string& source_a,
                    ShaderTarget target_a)
            {
                GLenum shader_type_ = 0;
                switch (target_a)
                {
                case ShaderTarget::VertexShader:
                    shader_type_ = GL_VERTEX_SHADER;
                    break;
                case ShaderTarget::FragmentShader:
                    shader_type_ = GL_FRAGMENT_SHADER;
                    break;
                default:
                    throw std::runtime_error(
                            "Unsupported shader target for GLSL compilation");
                }

                GLuint shader_ = glCreateShader(shader_type_);
                const char* src_ptr_ = source_a.c_str();
                glShaderSource(shader_, 1, &src_ptr_, nullptr);
                glCompileShader(shader_);

                GLint compile_status_ = GL_FALSE;
                glGetShaderiv(shader_, GL_COMPILE_STATUS, &compile_status_);
                if (compile_status_ != GL_TRUE)
                {
                    GLint log_length_ = 0;
                    glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &log_length_);
                    std::string info_log_(log_length_, '\0');
                    glGetShaderInfoLog(shader_, log_length_, nullptr, &info_log_[0]);
                    glDeleteShader(shader_);
                    throw std::runtime_error("GLSL compilation failed: "
                                             + info_log_);
                }

                GLuint program_ = glCreateProgram();
                glAttachShader(program_, shader_);
                glLinkProgram(program_);

                GLint link_status_ = GL_FALSE;
                glGetProgramiv(program_, GL_LINK_STATUS, &link_status_);
                if (link_status_ != GL_TRUE)
                {
                    GLint log_length_ = 0;
                    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &log_length_);
                    std::string info_log_(log_length_, '\0');
                    glGetProgramInfoLog(program_,
                                        log_length_,
                                        nullptr,
                                        &info_log_[0]);
                    
                    glDeleteShader(shader_);
                    glDeleteProgram(program_);
                    throw std::runtime_error("GLSL linking failed: " + info_log_);
                }

                GLint binary_size_ = 0;
                glGetProgramiv(program_, GL_PROGRAM_BINARY_LENGTH, &binary_size_);
                if (binary_size_ <= 0)
                {
                    glDeleteShader(shader_);
                    glDeleteProgram(program_);
                    throw std::runtime_error(
                            "Driver does not support program binary retrieval");
                }

                std::vector<uint8_t> binary_(binary_size_);
                GLenum binary_format_ = 0;
                glGetProgramBinary(program_,
                                   binary_size_,
                                   nullptr,
                                   &binary_format_,
                                   binary_.data());

                glDetachShader(program_, shader_);
                glDeleteShader(shader_);
                glDeleteProgram(program_);

                return binary_;
            }

            void ShaderPack::WriteShaderPack(
                    const std::string& path_a,
                    const std::vector<uint8_t>& hlsl_binary_a,
                    const std::vector<uint8_t>& glsl_binary_a)
            {
                std::ofstream file_(path_a, std::ios::binary);
                if (!file_)
                    throw std::runtime_error("Cannot create shader pack file: "
                                             + path_a);

                file_.write("HLSL", 4);
                uint32_t hlsl_size_ = static_cast<uint32_t>(hlsl_binary_a.size());
                file_.write(reinterpret_cast<const char*>(&hlsl_size_),
                           sizeof(hlsl_size_));
                
                if (hlsl_size_ > 0)
                {
                    file_.write(reinterpret_cast<const char*>(hlsl_binary_a.data()),
                               hlsl_size_);
                }

                file_.write("GLSL", 4);
                uint32_t glsl_size_ = static_cast<uint32_t>(glsl_binary_a.size());
                file_.write(reinterpret_cast<const char*>(&glsl_size_),
                           sizeof(glsl_size_));
                
                if (glsl_size_ > 0)
                {
                    file_.write(reinterpret_cast<const char*>(glsl_binary_a.data()),
                               glsl_size_);
                }

                file_.close();
            }

            std::vector<uint8_t> ShaderPack::ReadShaderSection(
                    const std::string& path_a,
                    const char* section_tag_a)
            {
                std::ifstream file_(path_a, std::ios::binary);
                if (!file_)
                    throw std::runtime_error("Cannot open shader pack file: "
                                             + path_a);

                char tag_[5] = {0};
                while (file_.read(tag_, 4))
                {
                    uint32_t size_ = 0;
                    file_.read(reinterpret_cast<char*>(&size_), sizeof(size_));

                    if (std::strncmp(tag_, section_tag_a, 4) == 0)
                    {
                        std::vector<uint8_t> data_(size_);
                        if (size_ > 0)
                        {
                            file_.read(reinterpret_cast<char*>(data_.data()),
                                      size_);
                        }
                        return data_;
                    } 
                    else
                    {
                        file_.seekg(size_, std::ios::cur);
                    }
                }

                throw std::runtime_error(
                        std::string("Section not found in shader pack: ")
                        + section_tag_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel