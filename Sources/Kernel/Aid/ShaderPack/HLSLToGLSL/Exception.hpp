// Copyright (c) 2025 Case Technologies

#pragma once
#include <stdexcept>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct AST;

            class ASTRuntimeError : public std::runtime_error
            {
            public:
                explicit ASTRuntimeError(const char* msg_a, const AST* ast_a);
                explicit ASTRuntimeError(const std::string& msg_a,
                                         const AST* ast_a);
                
                explicit ASTRuntimeError(
                        const std::string& msg_a,
                        const AST* ast_a,
                        const std::vector<const AST*>& ast_appendices_a);

                inline const AST* GetAST() const
                {
                    return ast_;
                }

                inline const std::vector<const AST*>& GetASTAppendices() const
                {
                    return ast_appendices_;
                }

            private:
                const AST* ast_ = nullptr;
                std::vector<const AST*> ast_appendices_;
            };

            [[noreturn]]
            void RuntimeErr(const char* msg_a);

            [[noreturn]]
            void RuntimeErr(const std::string& msg_a);

            [[noreturn]]
            void RuntimeErr(const char* msg_a, const AST* ast_a);

            [[noreturn]]
            void RuntimeErr(const std::string& msg_a, const AST* ast_a);

            [[noreturn]]
            void RuntimeErr(const std::string& msg_a,
                            const AST* ast_a,
                            const std::vector<const AST*>& ast_appendices_a);

            [[noreturn]]
            void InvalidArg(const char* msg_a);

            [[noreturn]]
            void InvalidArg(const std::string& msg_a);
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
