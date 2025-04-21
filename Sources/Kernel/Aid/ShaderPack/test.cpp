#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "antlr4-runtime.h"
#include "HLSLLexer.h"
#include "HLSLParser.h"
#include "HLSLBaseVisitor.h"

using namespace antlr4;

// Intermediate Representation
struct StructMember {
    std::string type;
    std::string name;
    std::string semantic;
};

struct Struct {
    std::string name;
    std::vector<StructMember> members;
};

struct ShaderIO {
    std::string type;
    std::string name;
    std::string semantic;
    int location;
};

class HLSLToGLSLVisitor : public HLSLBaseVisitor {
public:
    std::vector<Struct> structs;
    std::vector<ShaderIO> inputs;
    std::vector<ShaderIO> outputs;
    std::unordered_map<std::string, std::string> typeMap = {
        {"float", "float"},
        {"float2", "vec2"},
        {"float3", "vec3"},
        {"float4", "vec4"},
        {"matrix", "mat4"}
    };

    antlrcpp::Any visitStructDeclaration(HLSLParser::StructDeclarationContext *ctx) override {
        Struct s;
        s.name = ctx->ID()->getText();
        
        for (auto *memberCtx : ctx->structMember()) {
            StructMember m;
            m.type = memberCtx->type()->getText();
            m.name = memberCtx->ID()->getText();
            m.semantic = memberCtx->semantic()->getText();
            s.members.push_back(m);
        }
        
        structs.push_back(s);
        return nullptr;
    }

    antlrcpp::Any visitFunctionDeclaration(HLSLParser::FunctionDeclarationContext *ctx) override {
        if (ctx->ID()->getText() == "main") {
            // Process input parameters
            if (auto params = ctx->parameterList()) {
                for (auto *param : params->parameter()) {
                    ShaderIO io;
                    io.type = param->type()->getText();
                    io.name = param->ID()->getText();
                    io.semantic = param->semantic()->getText();
                    inputs.push_back(io);
                }
            }
            
            // Process return semantic
            if (ctx->semantic()) {
                ShaderIO io;
                io.type = ctx->type()->getText();
                io.semantic = ctx->semantic()->getText();
                outputs.push_back(io);
            }
        }
        return nullptr;
    }
};

std::string generateGLSL(const HLSLToGLSLVisitor& visitor) {
    std::string glsl = "#version 450\n\n";
    
    // Generate structs
    for (const auto& s : visitor.structs) {
        glsl += "struct " + s.name + " {\n";
        for (const auto& m : s.members) {
            std::string glType = visitor.typeMap.count(m.type) ? 
                                visitor.typeMap.at(m.type) : m.type;
            glsl += "    " + glType + " " + m.name + ";\n";
        }
        glsl += "};\n\n";
    }
    
    // Generate inputs
    int location = 0;
    for (const auto& input : visitor.inputs) {
        std::string glType = visitor.typeMap.count(input.type) ?
                           visitor.typeMap.at(input.type) : input.type;
        glsl += "layout(location = " + std::to_string(location++) + ") in " +
               glType + " " + input.name + "_in;\n";
    }
    glsl += "\n";
    
    // Generate outputs
    for (const auto& output : visitor.outputs) {
        std::string glType = visitor.typeMap.count(output.type) ?
                           visitor.typeMap.at(output.type) : output.type;
        glsl += "out " + glType + " " + output.semantic + ";\n";
    }
    glsl += "\n";
    
    // Generate main function
    glsl += "out gl_PerVertex {\n    vec4 gl_Position;\n};\n\n";
    glsl += "void main() {\n";
    glsl += "    VSOutput output;\n";
    
    // Generate main body
    for (const auto& input : visitor.inputs) {
        glsl += "    output." + input.name + " = " + input.name + "_in;\n";
    }
    
    glsl += "    gl_Position = output.pos;\n";
    glsl += "}\n";
    
    return glsl;
}

int main() {
    std::string hlslCode = R"(
        struct VSInput {
            float3 pos_ : POSITION;
            float2 tex_ : TEXCOORD0;
        };

        struct VSOutput {
            float4 pos_ : SV_POSITION;
            float4 tex_ : TEXCOORD0;
        };

        VSOutput main(VSInput input) : SV_POSITION {
            VSOutput output;
            output.pos = float4(input.pos, 1.0);
            output.tex = input.tex;
            return output;
        }
    )";

    ANTLRInputStream input(hlslCode);
    HLSLLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    HLSLParser parser(&tokens);

    HLSLToGLSLVisitor visitor;
    visitor.visit(parser.hlslFile());

    std::string glslCode = generateGLSL(visitor);
    std::cout << glslCode << std::endl;

    return 0;
}
