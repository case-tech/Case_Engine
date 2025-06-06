#include "Exception.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            ASTRuntimeError::ASTRuntimeError(const char* msg_a, const AST* ast_a)
                : std::runtime_error {msg_a}, ast_ {ast_a}
            {}

            ASTRuntimeError::ASTRuntimeError(const std::string& msg_a,
                                             const AST* ast_a)
                : std::runtime_error {msg_a}, ast_ {ast_a}
            {}

            ASTRuntimeError::ASTRuntimeError(
                    const std::string& msg_a,
                    const AST* ast_a,
                    const std::vector<const AST*>& ast_appendices_a)
                : std::runtime_error {msg_a}, ast_ {ast_a},
                  ast_appendices_ {ast_appendices_a}
            {}

            [[noreturn]]
            void RuntimeErr(const char* msg_a)
            {
                throw std::runtime_error(msg_a);
            }

            [[noreturn]]
            void RuntimeErr(const std::string& msg_a)
            {
                throw std::runtime_error(msg_a);
            }

            [[noreturn]]
            void RuntimeErr(const char* msg_a, const AST* ast_a)
            {
                if (ast_a)
                    throw ASTRuntimeError(msg_a, ast_a);
                else
                    throw std::runtime_error(msg_a);
            }

            [[noreturn]]
            void RuntimeErr(const std::string& msg_a, const AST* ast_a)
            {
                if (ast_a)
                    throw ASTRuntimeError(msg_a, ast_a);
                else
                    throw std::runtime_error(msg_a);
            }

            [[noreturn]]
            void RuntimeErr(const std::string& msg_a,
                            const AST* ast_a,
                            const std::vector<const AST*>& ast_appendices_a)
            {
                if (ast_a)
                    throw ASTRuntimeError(msg_a, ast_a, ast_appendices_a);
                else
                    throw std::runtime_error(msg_a);
            }

            [[noreturn]]
            void InvalidArg(const char* msg_a)
            {
                throw std::invalid_argument(msg_a);
            }

            [[noreturn]]
            void InvalidArg(const std::string& msg_a)
            {
                throw std::invalid_argument(msg_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
