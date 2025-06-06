#include "ExprEvaluator.hpp"

#include "../../Exception.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"

#include <sstream>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            ExprEvaluator::ExprEvaluator(Flags flags_a) : flags_ {flags_a}
            {}

            Variant ExprEvaluator::Evaluate(
                    Expr& expr_a,
                    const OnObjectExprCallback& on_object_expr_callback_a)
            {
                throw_on_failure_ = true;
                abort_ = false;

                SetObjectExprCallback(on_object_expr_callback_a);
                Visit(&expr_a);

                return Pop();
            }

            Variant ExprEvaluator::EvaluateOrDefault(
                    Expr& expr_a,
                    const Variant& default_value_a,
                    const OnObjectExprCallback& on_object_expr_callback_a)
            {
                throw_on_failure_ = false;
                abort_ = false;

                SetObjectExprCallback(on_object_expr_callback_a);
                Visit(&expr_a);

                if (auto value_ = Pop())
                    return value_;
                else
                    return default_value_a;
            }

            void ExprEvaluator::Abort()
            {
                abort_ = true;
            }

            [[noreturn]]
            static void IllegalExpr(const std::string& expr_name_a,
                                    const AST* ast_a = nullptr)
            {
                RuntimeErr(R_IllegalExprInConstExpr(expr_name_a), ast_a);
            }

            void ExprEvaluator::Push(const Variant& v_a)
            {
                if (!abort_)
                    variant_stack_.push(v_a);
            }

            Variant ExprEvaluator::Pop()
            {
                if (!abort_)
                {
                    if (!variant_stack_.empty())
                    {
                        auto v_ = variant_stack_.top();
                        variant_stack_.pop();
                        return v_;
                    } 
                    else if (throw_on_failure_)
                        RuntimeErr(R_StackUnderflow(R_ExprEvaluator));
                    else
                        Abort();
                }
                return {};
            }

            void ExprEvaluator::SetObjectExprCallback(
                    const OnObjectExprCallback& callback_a)
            {
                if (callback_a)
                {
                    on_object_expr_callback_ = callback_a;
                } 
                else
                {
                    on_object_expr_callback_ = [](ObjectExpr* expr_) -> Variant 
                    {
                        if (auto var_decl_ = expr_->FetchVarDecl())
                        {
                            if (var_decl_->HasStaticConstInitializer())
                                return var_decl_->initializer_value_;
                        }
                        return {};
                    };
                }
            }

            Variant ExprEvaluator::EvaluateBinaryOp(const BinaryExpr* ast_a,
                                                    Variant lhs_a,
                                                    Variant rhs_a)
            {
                switch (ast_a->op_)
                {
                case BinaryOp::Undefined:
                    if (throw_on_failure_)
                        IllegalExpr(R_BinaryOp, ast_a);
                    else
                        Abort();
                    break;

                case BinaryOp::LogicalAnd:
                    return (lhs_a.ToBool() && rhs_a.ToBool());

                case BinaryOp::LogicalOr:
                    return (lhs_a.ToBool() || rhs_a.ToBool());

                case BinaryOp::Or:
                    return (lhs_a | rhs_a);

                case BinaryOp::Xor:
                    return (lhs_a ^ rhs_a);

                case BinaryOp::And:
                    return (lhs_a & rhs_a);

                case BinaryOp::LShift:
                    return (lhs_a << rhs_a);

                case BinaryOp::RShift:
                    return (lhs_a >> rhs_a);

                case BinaryOp::Add:
                    return (lhs_a + rhs_a);

                case BinaryOp::Sub:
                    return (lhs_a - rhs_a);

                case BinaryOp::Mul:
                    return (lhs_a * rhs_a);

                case BinaryOp::Div:
                    if (lhs_a.Type() == Variant::Types::Int && rhs_a.Int() == 0)
                    {
                        if (throw_on_failure_)
                            IllegalExpr(R_DivisionByZero, ast_a);
                        else
                            Abort();
                        break;
                    }
                    return (lhs_a / rhs_a);

                case BinaryOp::Mod:
                    if (lhs_a.Type() == Variant::Types::Int && rhs_a.Int() == 0)
                    {
                        if (throw_on_failure_)
                            IllegalExpr(R_DivisionByZero, ast_a);
                        else
                            Abort();
                        break;
                    }
                    return (lhs_a % rhs_a);

                case BinaryOp::Equal:
                    return (lhs_a == rhs_a);

                case BinaryOp::NotEqual:
                    return (lhs_a != rhs_a);

                case BinaryOp::Less:
                    return (lhs_a < rhs_a);

                case BinaryOp::Greater:
                    return (lhs_a > rhs_a);

                case BinaryOp::LessEqual:
                    return (lhs_a <= rhs_a);

                case BinaryOp::GreaterEqual:
                    return (lhs_a >= rhs_a);
                }
                return {};
            }

            Variant ExprEvaluator::EvaluateUnaryOp(const UnaryExpr* ast_a,
                                                   Variant rhs_a)
            {
                switch (ast_a->op_)
                {
                case UnaryOp::Undefined:
                    if (throw_on_failure_)
                        IllegalExpr(R_UnaryOp, ast_a);
                    else
                        Abort();
                    break;

                case UnaryOp::LogicalNot:
                    return (!rhs_a.ToBool());

                case UnaryOp::Not:
                    return (~rhs_a);

                case UnaryOp::Nop:
                    return (rhs_a);

                case UnaryOp::Negate:
                    return (-rhs_a);

                case UnaryOp::Inc:
                    return (++rhs_a);

                case UnaryOp::Dec:
                    return (--rhs_a);
                }
                return {};
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void ExprEvaluator::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(NullExpr)
            {
                (void)args_a;
                if (throw_on_failure_)
                    IllegalExpr(R_DynamicArrayDim, ast_a);
                else
                    Abort();
            }

            IMPLEMENT_VISIT_PROC(SequenceExpr)
            {
                (void)args_a;
                Visit(ast_a->exprs_.front());
            }

            IMPLEMENT_VISIT_PROC(LiteralExpr)
            {
                (void)args_a;
                switch (ast_a->data_type_)
                {
                case DataType::Bool: {
                    if (ast_a->value_ == "true")
                        Push(true);
                    else if (ast_a->value_ == "false")
                        Push(false);
                    else if (throw_on_failure_)
                        IllegalExpr(R_BoolLiteralValue(ast_a->value_), ast_a);
                    else
                        Abort();
                }
                break;

                case DataType::Int: {
                    Push(FromStringOrDefault<long long>(ast_a->value_));
                }
                break;

                case DataType::UInt: {
                    Push(static_cast<Variant::IntType>(
                            FromStringOrDefault<unsigned long>(ast_a->value_)));
                }
                break;

                case DataType::Half:
                case DataType::Float:
                case DataType::Double: {
                    Push(FromStringOrDefault<double>(ast_a->value_));
                }
                break;

                default: {
                    if (throw_on_failure_)
                        IllegalExpr(R_LiteralType(
                                            DataTypeToString(ast_a->data_type_)),
                                    ast_a);
                    else
                        Abort();
                }
                break;
                }
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifierExpr)
            {
                (void)args_a;
                if (throw_on_failure_)
                    IllegalExpr(R_TypeSpecifier, ast_a);
                else
                    Abort();
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                (void)args_a;
                Visit(ast_a->cond_expr_);
                if (auto cond = Pop())
                {
                    if (cond.ToBool())
                        Visit(ast_a->then_expr_);
                    else
                        Visit(ast_a->else_expr_);
                }
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                (void)args_a;
                if (flags_(EvaluateReducedBinaryExpr)
                    && (ast_a->op_ == BinaryOp::LogicalAnd
                        || ast_a->op_ == BinaryOp::LogicalOr))
                {
                    if (ast_a->op_ == BinaryOp::LogicalAnd)
                    {
                        Visit(ast_a->lhs_expr_);
                        if (auto lhs_ = Pop())
                        {
                            auto lhs_value_ = lhs_.ToBool();
                            if (lhs_value_)
                            {
                                Visit(ast_a->rhs_expr_);
                                if (auto rhs_ = Pop())
                                {
                                    Push(rhs_.ToBool());
                                    return;
                                }
                            } 
                            else
                            {
                                Push(false);
                                return;
                            }
                        }
                    } 
                    else if (ast_a->op_ == BinaryOp::LogicalOr)
                    {
                        Visit(ast_a->lhs_expr_);
                        if (auto lhs_ = Pop())
                        {
                            auto lhs_value_ = lhs_.ToBool();
                            if (lhs_value_)
                            {
                                Push(true);
                                return;
                            } 
                            else
                            {
                                Visit(ast_a->rhs_expr_);
                                if (auto rhs_ = Pop())
                                {
                                    Push(rhs_.ToBool());
                                    return;
                                }
                            }
                        }
                    }
                } 
                else
                {
                    Visit(ast_a->lhs_expr_);
                    if (auto lhs_ = Pop())
                    {
                        Visit(ast_a->rhs_expr_);
                        if (auto rhs_ = Pop())
                        {
                            if (auto result_ = EvaluateBinaryOp(ast_a, lhs_, rhs_))
                            {
                                Push(result_);
                                return;
                            }
                        }
                    }
                }

                Push({});
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);

                if (auto rhs_ = Pop())
                {
                    if (auto result_ = EvaluateUnaryOp(ast_a, rhs_))
                    {
                        Push(result_);
                        return;
                    }
                }

                Push({});
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);

                if (auto lhs_ = Pop())
                {
                    switch (ast_a->op_)
                    {
                    case UnaryOp::Inc:
                    case UnaryOp::Dec:
                        Push(lhs_);
                        break;
                    default:
                        if (throw_on_failure_)
                            IllegalExpr(R_UnaryOp(UnaryOpToString(ast_a->op_)),
                                        ast_a);
                        else
                            Abort();
                        break;
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                (void)args_a;
                if (throw_on_failure_)
                    IllegalExpr(R_FunctionCall, ast_a);
                else
                    Abort();
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                (void)args_a;
                if (throw_on_failure_)
                    IllegalExpr(R_VarAssignment, ast_a);
                else
                    Abort();
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                (void)args_a;
                Push(on_object_expr_callback_(ast_a));
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                (void)args_a;
                Visit(ast_a->prefix_expr_);

                if (auto value_ = Pop())
                {
                    if (value_.IsArray())
                    {
                        for (std::size_t i_ = 0, n_ = ast_a->array_indices_.size();
                             i_ < n_;
                             ++i_)
                        {
                            Visit(ast_a->array_indices_[i_]);

                            if (auto array_idx_ = Pop())
                            {
                                if (value_.IsArray())
                                {
                                    auto array_idx_int_ = static_cast<std::size_t>(
                                            array_idx_.ToInt());
                                    if (auto sub_value_ = value_.ArraySub(
                                                array_idx_int_))
                                    {
                                        value_ = std::move(sub_value_);
                                    }
                                } 
                                else
                                    break;
                            } 
                            else
                            {
                                Abort();
                                return;
                            }
                        }

                        Push(value_);
                        return;
                    }
                }

                Abort();
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);

                if (auto value_ = Pop())
                {
                    if (auto base_type_den_ = ast_a->type_specifier_->GetTypeDenoter()
                                                   ->As<BaseTypeDenoter>())
                    {
                        switch (base_type_den_->data_type_)
                        {
                        case DataType::Bool: {
                            Push(value_.ToBool());
                        }
                        break;

                        case DataType::Int:
                        case DataType::UInt: {
                            Push(value_.ToInt());
                        }
                        break;

                        case DataType::Half:
                        case DataType::Float:
                        case DataType::Double: {
                            Push(value_.ToReal());
                        }
                        break;

                        default: {
                            if (throw_on_failure_)
                                IllegalExpr(R_TypeCast(DataTypeToString(
                                                    base_type_den_->data_type_)),
                                            ast_a);
                            else
                                Abort();
                        }
                        break;
                        }
                    } 
                    else if (throw_on_failure_)
                        IllegalExpr(R_TypeCast, ast_a);
                    else
                        Abort();
                }
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                (void)args_a;
                std::vector<Variant> sub_values_;

                for (const auto& expr_ : ast_a->exprs_)
                {
                    Visit(expr_);

                    if (auto value_ = Pop())
                    {
                        sub_values_.push_back(value_);
                    } 
                    else
                    {
                        Abort();
                        return;
                    }
                }

                Push(Variant(std::move(sub_values_)));
            }

#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
