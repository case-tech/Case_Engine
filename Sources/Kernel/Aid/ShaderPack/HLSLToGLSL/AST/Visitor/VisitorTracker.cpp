#include "VisitorTracker.hpp"

#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            bool VisitorTracker::InsideGlobalScope() const
            {
                return (!InsideFunctionDecl() && !InsideStructDecl()
                        && !InsideUniformBufferDecl() && !InsideVarDeclStmnt());
            }

            void VisitorTracker::PushFunctionDecl(FunctionDecl* func_decl_a)
            {
                func_decl_stack_.push(func_decl_a);
                if (func_decl_a->flags_(FunctionDecl::IsEntryPoint))
                    stack_level_of_entry_point_ = func_decl_stack_.size();
                
                else if (func_decl_a->flags_(FunctionDecl::IsSecondaryEntryPoint))
                    stack_level_of_2nd_entry_point_ = func_decl_stack_.size();
            }

            void VisitorTracker::PopFunctionDecl()
            {
                if (!func_decl_stack_.empty())
                {
                    if (stack_level_of_entry_point_ == func_decl_stack_.size())
                        stack_level_of_entry_point_ = (std::size_t)~0;
                    if (stack_level_of_2nd_entry_point_ == func_decl_stack_.size())
                        stack_level_of_2nd_entry_point_ = (std::size_t)~0;
                    func_decl_stack_.pop();
                } 
                else
                    throw std::underflow_error(R_FuncDeclStackUnderflow);
            }

            bool VisitorTracker::InsideFunctionDecl() const
            {
                return (!func_decl_stack_.empty());
            }

            bool VisitorTracker::InsideEntryPoint() const
            {
                return (func_decl_stack_.size() >= stack_level_of_entry_point_);
            }

            bool VisitorTracker::InsideSecondaryEntryPoint() const
            {
                return (func_decl_stack_.size() >= stack_level_of_2nd_entry_point_);
            }

            FunctionDecl* VisitorTracker::ActiveFunctionDecl() const
            {
                return (func_decl_stack_.empty() ? nullptr
                                               : func_decl_stack_.top());
            }

            StructDecl* VisitorTracker::ActiveFunctionStructDecl() const
            {
                if (auto func_decl_ = ActiveFunctionDecl())
                    return func_decl_->struct_decl_ref_;
                else
                    return nullptr;
            }

            void VisitorTracker::PushCallExpr(CallExpr* call_expr_a)
            {
                call_expr_stack_.push(call_expr_a);
            }

            void VisitorTracker::PopCallExpr()
            {
                if (!call_expr_stack_.empty())
                    call_expr_stack_.pop();
                else
                    throw std::underflow_error(R_CallExprStackUnderflow);
            }

            CallExpr* VisitorTracker::ActiveCallExpr() const
            {
                return (call_expr_stack_.empty() ? nullptr
                                               : call_expr_stack_.top());
            }

            void VisitorTracker::PushLValueExpr(Expr* expr_a)
            {
                lvalue_expr_stack_.push(expr_a);
            }

            void VisitorTracker::PopLValueExpr()
            {
                if (!lvalue_expr_stack_.empty())
                    lvalue_expr_stack_.pop();
                else
                    throw std::runtime_error(R_LValueExprStackUnderflow);
            }

            Expr* VisitorTracker::ActiveLValueExpr() const
            {
                return (lvalue_expr_stack_.empty() ? nullptr
                                                 : lvalue_expr_stack_.top());
            }

            void VisitorTracker::PushStructDecl(StructDecl* struct_decl_a)
            {
                struct_decl_stack_.push_back(struct_decl_a);
            }

            void VisitorTracker::PopStructDecl()
            {
                if (!struct_decl_stack_.empty())
                    struct_decl_stack_.pop_back();
                else
                    throw std::underflow_error(R_StructDeclStackUnderflow);
            }

            bool VisitorTracker::InsideStructDecl() const
            {
                return (!struct_decl_stack_.empty());
            }

            StructDecl* VisitorTracker::ActiveStructDecl() const
            {
                return (struct_decl_stack_.empty() ? nullptr
                                                 : struct_decl_stack_.back());
            }

            void VisitorTracker::PushUniformBufferDecl(
                    UniformBufferDecl* uniform_buffer_decl_a)
            {
                uniform_buffer_decl_stack_.push_back(uniform_buffer_decl_a);
            }

            void VisitorTracker::PopUniformBufferDecl()
            {
                if (!uniform_buffer_decl_stack_.empty())
                    uniform_buffer_decl_stack_.pop_back();
                else
                    throw std::underflow_error(
                            R_UniformBufferDeclStackUnderflow);
            }

            bool VisitorTracker::InsideUniformBufferDecl() const
            {
                return (!uniform_buffer_decl_stack_.empty());
            }

            void VisitorTracker::PushVarDeclStmnt(VarDeclStmnt* var_decl_stmnt_a)
            {
                var_decl_stmnt_stack_.push(var_decl_stmnt_a);
            }

            void VisitorTracker::PopVarDeclStmnt()
            {
                if (!var_decl_stmnt_stack_.empty())
                    var_decl_stmnt_stack_.pop();
                else
                    throw std::underflow_error(R_VarDeclStmntStackUnderflow);
            }

            bool VisitorTracker::InsideVarDeclStmnt() const
            {
                return (!var_decl_stmnt_stack_.empty());
            }

            VarDeclStmnt* VisitorTracker::ActiveVarDeclStmnt() const
            {
                return (var_decl_stmnt_stack_.empty() ? nullptr
                                                   : var_decl_stmnt_stack_.top());
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel