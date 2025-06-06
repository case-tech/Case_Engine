#include "ExprConverter.hpp"

#include "../../Exception.hpp"
#include "../../Frontend/GLSL/GLSLKeywords.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"
#include "../ASTFactory.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void ExprConverter::Convert(Program& program_a,
                                        const Flags& conversion_flags_a,
                                        const NameMangling& name_mangling_a)
            {
                conversion_flags_ = conversion_flags_a;
                name_mangling_ = name_mangling_a;

                if (conversion_flags_ != 0)
                    Visit(&program_a);
            }

            static std::unique_ptr<DataType> MustCastExprToDataType(
                    const DataType target_type_a,
                    const DataType source_type_a,
                    bool match_type_size_a)
            {
                const auto target_dim_ = VectorTypeDim(target_type_a);
                const auto source_dim_ = VectorTypeDim(source_type_a);

                if ((target_dim_ != source_dim_ && match_type_size_a)
                    || (IsBooleanType(target_type_a) != IsBooleanType(source_type_a))
                    || (IsUIntType(target_type_a) && IsIntType(source_type_a))
                    || (IsIntType(target_type_a) && IsUIntType(source_type_a))
                    || (IsRealType(target_type_a) && IsIntegralType(source_type_a))
                    || (IsIntegralType(target_type_a) && IsRealType(source_type_a))
                    || (IsDoubleRealType(target_type_a)
                        != IsDoubleRealType(source_type_a)))
                {
                    if (target_dim_ != source_dim_ && !match_type_size_a)
                    {
                        return MakeUnique<DataType>(
                                VectorDataType(BaseDataType(target_type_a),
                                               VectorTypeDim(source_type_a)));
                    } 
                    else
                    {
                        return MakeUnique<DataType>(target_type_a);
                    }
                }

                return nullptr;
            }

            static std::unique_ptr<DataType> MustCastExprToDataType(
                    const TypeDenoter& target_type_den_a,
                    const TypeDenoter& source_type_den_a,
                    bool match_type_size_a)
            {
                if (auto base_target_type_den_ =
                            target_type_den_a.As<BaseTypeDenoter>())
                {
                    if (auto base_source_type_den_ =
                                source_type_den_a.As<BaseTypeDenoter>())
                    {
                        return MustCastExprToDataType(
                                base_target_type_den_->data_type_,
                                base_source_type_den_->data_type_,
                                match_type_size_a);
                    }
                }

                return nullptr;
            }

            static void ConvertCastExpr(ExprPtr& expr_a,
                                        const DataType source_type_a,
                                        const DataType target_type_a)
            {
                if (IsVectorType(source_type_a) && IsVectorType(target_type_a))
                {
                    auto target_dim_ = VectorTypeDim(target_type_a);
                    auto source_dim_ = VectorTypeDim(source_type_a);

                    if (source_dim_ < target_dim_)
                    {
                        auto type_denoter_ = std::make_shared<BaseTypeDenoter>(
                                target_type_a);

                        std::vector<ExprPtr> args_;
                        args_.push_back(expr_a);

                        auto base_data_type_ = BaseDataType(target_type_a);
                        for (auto i_ = source_dim_; i_ < target_dim_; ++i_)
                            args_.push_back(
                                    ASTFactory::MakeLiteralExpr(base_data_type_,
                                                                "0"));

                        expr_a = ASTFactory::MakeTypeCtorCallExpr(type_denoter_,
                                                                args_);
                    } 
                    else
                    {
                        expr_a = ASTFactory::ConvertExprBaseType(target_type_a,
                                                               expr_a);
                    }
                } 
                else
                {
                    expr_a = ASTFactory::ConvertExprBaseType(target_type_a, expr_a);
                }
            }

            void ExprConverter::ConvertExprIfCastRequired(
                    ExprPtr& expr_a,
                    const DataType target_type_a,
                    bool match_type_size_a)
            {
                if (expr_a)
                {
                    const auto& source_type_den_ =
                            expr_a->GetTypeDenoter()->GetAliased();
                    if (auto base_source_type_den_ =
                                source_type_den_.As<BaseTypeDenoter>())
                    {
                        if (auto data_type_ = MustCastExprToDataType(
                                    target_type_a,
                                    base_source_type_den_->data_type_,
                                    match_type_size_a))
                            ConvertCastExpr(expr_a,
                                            base_source_type_den_->data_type_,
                                            *data_type_);
                    }
                }
            }

            void ExprConverter::ConvertExprIfCastRequired(
                    ExprPtr& expr_a,
                    const TypeDenoter& target_type_den_a,
                    bool match_type_size_a)
            {
                if (expr_a)
                {
                    const auto& source_type_den_ =
                            expr_a->GetTypeDenoter()->GetAliased();
                    if (auto data_type_ = MustCastExprToDataType(target_type_den_a,
                                                               source_type_den_,
                                                               match_type_size_a))
                    {
                        if (auto base_source_type_den_ =
                                    source_type_den_.As<BaseTypeDenoter>())
                            ConvertCastExpr(expr_a,
                                            base_source_type_den_->data_type_,
                                            *data_type_);
                        else
                        {
                            expr_a = ASTFactory::ConvertExprBaseType(*data_type_,
                                                                   expr_a);
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprIntoBracket(ExprPtr& expr_a)
            {
                expr_a = ASTFactory::MakeBracketExpr(expr_a);
            }

            int ExprConverter::GetTextureDimFromExpr(Expr* expr_a, const AST* ast_a)
            {
                if (expr_a)
                {
                    const auto& type_den_ = expr_a->GetTypeDenoter()->GetAliased();
                    if (auto buffer_type_den_ = type_den_.As<BufferTypeDenoter>())
                    {
                        if (auto texture_dim_ = GetBufferTypeTextureDim(
                                    buffer_type_den_->buffer_type_))
                            return texture_dim_;
                    } 
                    
                    else if (auto sampler_type_den_ =
                                       type_den_.As<SamplerTypeDenoter>())
                    {
                        if (auto texture_dim_ = GetSamplerTypeTextureDim(
                                    sampler_type_den_->sampler_type_))
                            return texture_dim_;
                    }
                    RuntimeErr(R_FailedToGetTextureDim, ast_a);
                }

                RuntimeErr(R_FailedToGetTextureDim, ast_a);
            }

            std::string ExprConverter::GetMatrixSubscriptWrapperIdent(
                    const NameMangling& name_mangling_a,
                    const MatrixSubscriptUsage& subscript_usage_a)
            {
                return (name_mangling_a.temporary_prefix_ + "subscript"
                        + subscript_usage_a.IndicesToString());
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void ExprConverter::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                if (ast_a->initializer_)
                {
                    ConvertExpr(ast_a->initializer_, AllPreVisit);
                    {
                        VISIT_DEFAULT(VarDecl);
                    }
                    ConvertExpr(ast_a->initializer_, AllPostVisit);

                    ConvertExprTargetType(ast_a->initializer_,
                            ast_a->GetTypeDenoter()->GetAliased());
                } 
                else
                    VISIT_DEFAULT(VarDecl);
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                PushFunctionDecl(ast_a);
                {
                    VISIT_DEFAULT(FunctionDecl);
                }
                PopFunctionDecl();
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                ConvertExpr(ast_a->condition_, AllPreVisit);
                ConvertExpr(ast_a->iteration_, AllPreVisit);
                {
                    VISIT_DEFAULT(ForLoopStmnt);
                }
                ConvertExpr(ast_a->condition_, AllPostVisit);
                ConvertExpr(ast_a->iteration_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                ConvertExpr(ast_a->condition_, AllPreVisit);
                {
                    VISIT_DEFAULT(WhileLoopStmnt);
                }
                ConvertExpr(ast_a->condition_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                ConvertExpr(ast_a->condition_, AllPreVisit);
                {
                    VISIT_DEFAULT(DoWhileLoopStmnt);
                }
                ConvertExpr(ast_a->condition_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                ConvertExpr(ast_a->condition_, AllPreVisit);
                {
                    VISIT_DEFAULT(IfStmnt);
                }
                ConvertExpr(ast_a->condition_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                ConvertExpr(ast_a->selector_, AllPreVisit);
                {
                    VISIT_DEFAULT(SwitchStmnt);
                }
                ConvertExpr(ast_a->selector_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                ConvertExpr(ast_a->expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(ExprStmnt);
                }
                ConvertExpr(ast_a->expr_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                if (ast_a->expr_)
                {
                    ConvertExpr(ast_a->expr_, AllPreVisit);
                    {
                        VISIT_DEFAULT(ReturnStmnt);
                    }
                    ConvertExpr(ast_a->expr_, AllPostVisit);

                    if (auto func_decl_ = ActiveFunctionDecl())
                        ConvertExprTargetType(ast_a->expr_,
                                              func_decl_->return_type_
                                                      ->GetTypeDenoter()
                                                      ->GetAliased());
                }
            }

            IMPLEMENT_VISIT_PROC(LiteralExpr)
            {
                (void)args_a;
                if (conversion_flags_(ConvertLiteralHalfToFloat))
                {
                    if (ast_a->data_type_ == DataType::Half)
                        ast_a->ConvertDataType(DataType::Float);
                }
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                ConvertExpr(ast_a->cond_expr_, AllPreVisit);
                ConvertExpr(ast_a->then_expr_, AllPreVisit);
                ConvertExpr(ast_a->else_expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(TernaryExpr);
                }
                ConvertExpr(ast_a->cond_expr_, AllPostVisit);
                ConvertExpr(ast_a->then_expr_, AllPostVisit);
                ConvertExpr(ast_a->else_expr_, AllPostVisit);

                ExprConverter::ConvertExprIfCastRequired(ast_a->cond_expr_,
                                                         DataType::Bool,
                                                         false);
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                ConvertExpr(ast_a->lhs_expr_, AllPreVisit);
                ConvertExpr(ast_a->rhs_expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(BinaryExpr);
                }
                ConvertExpr(ast_a->lhs_expr_, AllPostVisit);
                ConvertExpr(ast_a->rhs_expr_, AllPostVisit);

                auto lhs_type_den_ = ast_a->lhs_expr_->GetTypeDenoter()->GetSub();
                auto rhs_type_den_ = ast_a->rhs_expr_->GetTypeDenoter()->GetSub();

                auto common_type_den_ =
                        TypeDenoter::FindCommonTypeDenoter(lhs_type_den_,
                                                           rhs_type_den_);

                bool match_type_size_ = true;
                if (ast_a->op_ == BinaryOp::Div)
                {
                    if (rhs_type_den_->IsScalar())
                        match_type_size_ = false;
                } 
                else if (ast_a->op_ == BinaryOp::Mul)
                {
                    if (lhs_type_den_->IsScalar() || rhs_type_den_->IsScalar())
                        match_type_size_ = false;
                }

                ConvertExprTargetType(ast_a->lhs_expr_,
                                      *common_type_den_,
                                      match_type_size_);
                
                ConvertExprTargetType(ast_a->rhs_expr_,
                                      *common_type_den_,
                                      match_type_size_);

                ast_a->ResetTypeDenoter();
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                ConvertExpr(ast_a->expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(UnaryExpr);
                }
                ConvertExpr(ast_a->expr_, AllPostVisit);

                if (ast_a->expr_->Type() == AST::Types::UnaryExpr)
                    ConvertExpr(ast_a->expr_, ConvertUnaryExpr);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                Flags pre_visit_flags_ = AllPreVisit;
                if (IsInterlockedIntristic(ast_a->intrinsic_))
                    pre_visit_flags_.Remove(ConvertImageAccess
                                         | ConvertTextureBracketOp);

                if (ast_a->intrinsic_ == Intrinsic::Mul
                    && ast_a->arguments_.size() == 2)
                {
                    const auto& type_den_arg0_ = ast_a->arguments_[0]
                                                      ->GetTypeDenoter()
                                                      ->GetAliased();
                    
                    const auto& type_den_arg1_ = ast_a->arguments_[1]
                                                      ->GetTypeDenoter()
                                                      ->GetAliased();

                    if (type_den_arg0_.IsVector() && type_den_arg1_.IsVector())
                        ast_a->intrinsic_ = Intrinsic::Dot;
                }

                ConvertExpr(ast_a->prefix_expr_, AllPreVisit);
                ConvertExprList(ast_a->arguments_, pre_visit_flags_);
                {
                    VISIT_DEFAULT(CallExpr);
                }
                ConvertExprList(ast_a->arguments_, AllPostVisit);
                ConvertExpr(ast_a->prefix_expr_, AllPostVisit);

                if (!IsInterlockedIntristic(ast_a->intrinsic_))
                {
                    ast_a->ForEachArgumentWithParameterType(
                            [this](ExprPtr& func_arg_,
                                   const TypeDenoter& param_type_den_) {
                                ConvertExprTargetType(func_arg_, param_type_den_);
                            });
                }
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                ConvertExpr(ast_a->expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(BracketExpr);
                }
                ConvertExpr(ast_a->expr_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                ConvertExpr(ast_a->expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(CastExpr);
                }
                ConvertExpr(ast_a->expr_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                ConvertExpr(ast_a->prefix_expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(ObjectExpr);
                }
                ConvertExpr(ast_a->prefix_expr_, AllPostVisit);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                ConvertExpr(ast_a->lvalue_expr_, AllPreVisit);
                ConvertExpr(ast_a->rvalue_expr_, AllPreVisit);
                {
                    VISIT_DEFAULT(AssignExpr);
                }
                ConvertExpr(ast_a->lvalue_expr_, AllPostVisit);
                ConvertExpr(ast_a->rvalue_expr_, AllPostVisit);

                ConvertExprTargetType(
                        ast_a->rvalue_expr_,
                        ast_a->lvalue_expr_->GetTypeDenoter()->GetAliased());
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                for (auto& expr_ : ast_a->array_indices_)
                    ConvertExpr(expr_, AllPreVisit);

                VISIT_DEFAULT(ArrayExpr);

                for (auto& expr_ : ast_a->array_indices_)
                {
                    ConvertExpr(expr_, AllPostVisit);

                    const auto& type_den_ = expr_->GetTypeDenoter()->GetAliased();
                    if (auto base_type_den_ = type_den_.As<BaseTypeDenoter>())
                    {
                        auto base_data_type_ = BaseDataType(base_type_den_->data_type_);
                        if (base_data_type_ != DataType::Int)
                            base_data_type_ = DataType::UInt;

                        const auto int_vec_type_ = VectorDataType(
                                base_data_type_,
                                VectorTypeDim(base_type_den_->data_type_));
                        
                        ConvertExprTargetType(expr_,
                                              BaseTypeDenoter(int_vec_type_));
                    }
                }
            }

#undef IMPLEMENT_VISIT_PROC

            static TypeDenoterPtr MakeBufferAccessCallTypeDenoter(
                    const DataType generic_data_type_a)
            {
                auto type_denoter_ = std::make_shared<BaseTypeDenoter>();

                if (IsIntType(generic_data_type_a))
                    type_denoter_->data_type_ = DataType::Int4;
                else if (IsUIntType(generic_data_type_a))
                    type_denoter_->data_type_ = DataType::UInt4;
                else
                    type_denoter_->data_type_ = DataType::Float4;

                return type_denoter_;
            }

            void ExprConverter::ConvertExpr(ExprPtr& expr_a, const Flags& flags_a)
            {
                if (expr_a)
                {
                    const auto enabled_ = Flags(flags_a & conversion_flags_);

                    if (enabled_(ConvertTextureIntrinsicVec4))
                        ConvertExprTextureIntrinsicVec4(expr_a);

                    if (enabled_(ConvertLog10))
                        ConvertExprIntrinsicCallLog10(expr_a);

                    if (enabled_(ConvertVectorCompare))
                        ConvertExprVectorCompare(expr_a);

                    if (enabled_(ConvertImageAccess))
                        ConvertExprImageAccess(expr_a);

                    if (enabled_(ConvertSamplerBufferAccess))
                        ConvertExprSamplerBufferAccess(expr_a);

                    if (enabled_(ConvertVectorSubscripts))
                        ConvertExprVectorSubscript(expr_a);

                    if (enabled_(ConvertMatrixSubscripts))
                        ConvertExprMatrixSubscript(expr_a);

                    if (enabled_(ConvertUnaryExpr))
                        ConvertExprIntoBracket(expr_a);

                    if (enabled_(ConvertTextureBracketOp))
                        ConvertExprTextureBracketOp(expr_a);

                    if (enabled_(ConvertCompatibleStructs))
                        ConvertExprCompatibleStruct(expr_a);
                }
            }

            void ExprConverter::ConvertExprList(std::vector<ExprPtr>& expr_list_a,
                                                const Flags& flags_a)
            {
                for (auto& expr_ : expr_list_a)
                    ConvertExpr(expr_, flags_a);
            }

            void ExprConverter::ConvertExprVectorSubscript(ExprPtr& expr_a)
            {
                if (auto object_expr_ = expr_a->As<ObjectExpr>())
                    ConvertExprVectorSubscriptObject(expr_a, object_expr_);
            }

            void ExprConverter::ConvertExprVectorSubscriptObject(
                    ExprPtr& expr_a,
                    ObjectExpr* object_expr_a)
            {
                if (!object_expr_a->symbol_ref_ && object_expr_a->prefix_expr_)
                {
                    const auto& prefix_type_den_ = object_expr_a->prefix_expr_
                                                        ->GetTypeDenoter()
                                                        ->GetAliased();
                    if (prefix_type_den_.IsScalar())
                    {
                        auto vector_type_den_ =
                                object_expr_a->GetTypeDenoterFromSubscript();
                        expr_a = ASTFactory::MakeCastExpr(vector_type_den_,
                                                        object_expr_a->prefix_expr_);
                    }
                }
            }

            void ExprConverter::ConvertExprMatrixSubscript(ExprPtr& expr_a)
            {
                if (auto object_expr_ = expr_a->As<ObjectExpr>())
                    ConvertExprMatrixSubscriptObject(expr_a, object_expr_);
            }

            void ExprConverter::ConvertExprMatrixSubscriptObject(
                    ExprPtr& expr_a,
                    ObjectExpr* object_expr_a)
            {
                if (!object_expr_a->symbol_ref_ && object_expr_a->prefix_expr_)
                {
                    const auto& prefix_type_den_ = object_expr_a->prefix_expr_
                                                        ->GetTypeDenoter()
                                                        ->GetAliased();
                    if (prefix_type_den_.IsMatrix())
                    {
                        auto prefix_base_type_den_ =
                                prefix_type_den_.As<BaseTypeDenoter>();
                        const MatrixSubscriptUsage subscript_usage_(
                                prefix_base_type_den_->data_type_,
                                object_expr_a->ident_);

                        if (IsScalarType(subscript_usage_.data_type_out_)
                            && !subscript_usage_.indices_.empty())
                        {
                            const auto subscript_index_ =
                                    subscript_usage_.indices_.front();
                            
                            expr_a = ASTFactory::MakeArrayExpr(
                                    object_expr_a->prefix_expr_,
                                    {subscript_index_.first,
                                     subscript_index_.second});
                        } 
                        else
                        {
                            const auto wrapper_ident_ = ExprConverter::
                                    GetMatrixSubscriptWrapperIdent(
                                            name_mangling_,
                                            subscript_usage_);
                            
                            expr_a = ASTFactory::MakeWrapperCallExpr(
                                    wrapper_ident_,
                                    std::make_shared<BaseTypeDenoter>(
                                            subscript_usage_.data_type_out_),
                                    {object_expr_a->prefix_expr_});
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprVectorCompare(ExprPtr& expr_a)
            {
                if (auto unary_expr_ = expr_a->As<UnaryExpr>())
                    ConvertExprVectorCompareUnary(expr_a, unary_expr_);
                else if (auto binary_expr_ = expr_a->As<BinaryExpr>())
                    ConvertExprVectorCompareBinary(expr_a, binary_expr_);
                else if (auto ternary_expr_ = expr_a->As<TernaryExpr>())
                    ConvertExprVectorCompareTernary(expr_a, ternary_expr_);
            }

            void ExprConverter::ConvertExprVectorCompareUnary(
                    ExprPtr& expr_a,
                    UnaryExpr* unary_expr_a)
            {
                if (IsLogicalOp(unary_expr_a->op_))
                {
                    const auto& type_den_ = unary_expr_a->GetTypeDenoter();
                    if (type_den_->GetAliased().IsVector())
                    {
                        switch (unary_expr_a->op_)
                        {
                        case UnaryOp::LogicalNot: {
                            expr_a = ASTFactory::MakeIntrinsicCallExpr(
                                    Intrinsic::Not,
                                    "not",
                                    type_den_,
                                    {unary_expr_a->expr_});
                        }
                        break;

                        default:
                            break;
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprVectorCompareBinary(
                    ExprPtr& expr_a,
                    BinaryExpr* binary_expr_a)
            {
                if (IsCompareOp(binary_expr_a->op_))
                {
                    const auto& type_den_ =
                            binary_expr_a->GetTypeDenoter()->GetAliased();
                    if (type_den_.IsVector())
                    {
                        auto intrinsic_ = CompareOpToIntrinsic(binary_expr_a->op_);
                        expr_a = ASTFactory::MakeIntrinsicCallExpr(
                                intrinsic_,
                                "vec_compare",
                                nullptr,
                                {binary_expr_a->lhs_expr_, binary_expr_a->rhs_expr_});
                    }
                }
            }

            void ExprConverter::ConvertExprVectorCompareTernary(
                    ExprPtr& expr_a,
                    TernaryExpr* ternary_expr_a)
            {
                if (ternary_expr_a->IsVectorCondition())
                {
                    expr_a = ASTFactory::MakeIntrinsicCallExpr(
                            Intrinsic::Lerp,
                            "lerp",
                            nullptr,
                            {ternary_expr_a->then_expr_,
                             ternary_expr_a->else_expr_,
                             ternary_expr_a->cond_expr_});
                }
            }

            void ExprConverter::ConvertExprImageAccess(ExprPtr& expr_a)
            {
                if (!expr_a->flags_(Expr::WasConverted))
                {
                    if (auto assign_expr_ = expr_a->As<AssignExpr>())
                        ConvertExprImageAccessAssign(expr_a, assign_expr_);
                    
                    else if (auto array_expr_ = expr_a->As<ArrayExpr>())
                        ConvertExprImageAccessArray(expr_a, array_expr_);
                }
            }

            void ExprConverter::ConvertExprImageAccessAssign(
                    ExprPtr& expr_a,
                    AssignExpr* assign_expr_a)
            {
                if (auto array_expr_ = assign_expr_a->lvalue_expr_->As<ArrayExpr>())
                    ConvertExprImageAccessArray(expr_a, array_expr_, assign_expr_a);
            }

            void ExprConverter::ConvertExprImageAccessArray(
                    ExprPtr& expr_a,
                    ArrayExpr* array_expr_a,
                    AssignExpr* assign_expr_a)
            {
                auto prefix_type_den_a =
                        array_expr_a->prefix_expr_->GetTypeDenoter()->GetSub();

                std::size_t num_dims_ = 0;
                if (auto array_type_denoter_ =
                            prefix_type_den_a->As<ArrayTypeDenoter>())
                {
                    num_dims_ = array_type_denoter_->array_dims_.size();
                    prefix_type_den_a = array_type_denoter_->sub_type_denoter_;
                }

                if (auto buffer_type_den_ = prefix_type_den_a->As<BufferTypeDenoter>())
                {
                    if (auto buffer_decl_ = buffer_type_den_->buffer_decl_ref_)
                    {
                        const auto buffer_type_ = buffer_decl_->GetBufferType();
                        if (IsRWImageBufferType(buffer_type_)
                            && num_dims_ < array_expr_a->NumIndices())
                        {
                            if (auto generic_base_type_den_ =
                                        buffer_type_den_->generic_type_denoter_
                                                ->As<BaseTypeDenoter>())
                            {
                                array_expr_a->prefix_expr_->flags_
                                        << Expr::WasConverted;
                                
                                auto call_type_den_ =
                                        MakeBufferAccessCallTypeDenoter(
                                                generic_base_type_den_->data_type_);

                                ExprPtr arg0_expr_;
                                if (num_dims_ > 0)
                                {
                                    std::vector<ExprPtr> array_indices_;
                                    for (std::size_t i_ = 0; i_ < num_dims_; ++i_)
                                        array_indices_.push_back(
                                                array_expr_a->array_indices_[i_]);

                                    arg0_expr_ = ASTFactory::MakeArrayExpr(
                                            array_expr_a->prefix_expr_,
                                            std::move(array_indices_));
                                } 
                                else
                                    arg0_expr_ = array_expr_a->prefix_expr_;

                                auto arg1_expr_ =
                                        array_expr_a->array_indices_[num_dims_];
                                
                                auto texture_dim_ =
                                        GetTextureDimFromExpr(arg0_expr_.get(),
                                                              expr_a.get());
                                
                                ExprConverter::ConvertExprIfCastRequired(
                                        arg1_expr_,
                                        VectorDataType(DataType::Int,
                                                       texture_dim_),
                                        true);

                                ExprPtr expr_out_;

                                if (assign_expr_a)
                                {
                                    ExprPtr arg2_expr_;

                                    if (assign_expr_a->op_ == AssignOp::Set)
                                    {
                                        arg2_expr_ = assign_expr_a->rvalue_expr_;
                                    } 
                                    else
                                    {
                                        auto lhs_expr_ = ASTFactory::
                                                MakeIntrinsicCallExpr(
                                                        Intrinsic::Image_Load,
                                                        "imageLoad",
                                                        call_type_den_,
                                                        {arg0_expr_, arg1_expr_});

                                        auto rhs_expr_ = assign_expr_a->rvalue_expr_;

                                        const auto binary_op_ =
                                                AssignOpToBinaryOp(
                                                        assign_expr_a->op_);

                                        arg2_expr_ = ASTFactory::MakeBinaryExpr(
                                                lhs_expr_,
                                                binary_op_,
                                                rhs_expr_);
                                    }

                                    if (IsIntType(generic_base_type_den_->data_type_))
                                        ExprConverter::
                                                ConvertExprIfCastRequired(
                                                        arg2_expr_,
                                                        DataType::Int4,
                                                        true);
                                    else if (IsUIntType(generic_base_type_den_
                                                                ->data_type_))
                                        ExprConverter::
                                                ConvertExprIfCastRequired(
                                                        arg2_expr_,
                                                        DataType::UInt4,
                                                        true);
                                    else
                                        ExprConverter::
                                                ConvertExprIfCastRequired(
                                                        arg2_expr_,
                                                        DataType::Float4,
                                                        true);

                                    expr_out_ = ASTFactory::MakeIntrinsicCallExpr(
                                            Intrinsic::Image_Store,
                                            "imageStore",
                                            nullptr,
                                            {arg0_expr_, arg1_expr_, arg2_expr_});
                                } 
                                else
                                {
                                    expr_out_ = ASTFactory::MakeIntrinsicCallExpr(
                                            Intrinsic::Image_Load,
                                            "imageLoad",
                                            call_type_den_,
                                            {arg0_expr_, arg1_expr_});
                                }

                                if (array_expr_a->NumIndices() > num_dims_ + 1)
                                {
                                    array_expr_a->prefix_expr_ = expr_out_;
                                    array_expr_a->array_indices_.erase(
                                            array_expr_a->array_indices_.begin(),
                                            array_expr_a->array_indices_.begin()
                                                    + num_dims_ + 1);
                                } 
                                else
                                {
                                    expr_a = expr_out_;
                                }
                            }
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprSamplerBufferAccess(ExprPtr& expr_a)
            {
                if (!expr_a->flags_(Expr::WasConverted))
                {
                    if (auto array_expr_ = expr_a->As<ArrayExpr>())
                        ConvertExprSamplerBufferAccessArray(expr_a, array_expr_);
                }
            }

            void ExprConverter::ConvertExprSamplerBufferAccessArray(
                    ExprPtr& expr_a,
                    ArrayExpr* array_expr_a)
            {
                auto prefix_type_den_ =
                        array_expr_a->prefix_expr_->GetTypeDenoter()->GetSub();

                std::size_t num_dims_ = 0;
                if (auto array_type_denoter_ =
                            prefix_type_den_->As<ArrayTypeDenoter>())
                {
                    num_dims_ = array_type_denoter_->array_dims_.size();
                    prefix_type_den_ = array_type_denoter_->sub_type_denoter_;
                }

                if (auto buffer_type_den_ = prefix_type_den_->As<BufferTypeDenoter>())
                {
                    if (auto buffer_decl_ = buffer_type_den_->buffer_decl_ref_)
                    {
                        const auto buffer_type_ = buffer_decl_->GetBufferType();
                        if (buffer_type_ == BufferType::Buffer
                            && num_dims_ < array_expr_a->NumIndices())
                        {
                            if (auto generic_base_type_den_ =
                                        buffer_type_den_->generic_type_denoter_
                                                ->As<BaseTypeDenoter>())
                            {
                                auto call_type_den_ =
                                        MakeBufferAccessCallTypeDenoter(
                                                generic_base_type_den_->data_type_);

                                array_expr_a->prefix_expr_->flags_
                                        << Expr::WasConverted;

                                auto arg_expr_ = array_expr_a->array_indices_.back();

                                auto call_expr_ =
                                        ASTFactory::MakeIntrinsicCallExpr(
                                                Intrinsic::Texture_Load_1,
                                                "Load",
                                                call_type_den_,
                                                {arg_expr_});

                                if (num_dims_ > 0)
                                {
                                    std::vector<ExprPtr> array_indices_;
                                    for (std::size_t i = 0; i < num_dims_; ++i)
                                        array_indices_.push_back(
                                                array_expr_a->array_indices_[i]);

                                    call_expr_->prefix_expr_ =
                                            ASTFactory::MakeArrayExpr(
                                                    array_expr_a->prefix_expr_,
                                                    std::move(array_indices_));
                                } 
                                else
                                    call_expr_->prefix_expr_ =
                                            array_expr_a->prefix_expr_;

                                expr_a = call_expr_;
                            }
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprIntrinsicCallLog10(ExprPtr& expr_a)
            {
                if (auto call_expr_ = expr_a->As<CallExpr>())
                {
                    if (call_expr_->intrinsic_ == Intrinsic::Log10
                        && call_expr_->arguments_.size() == 1)
                    {
                        const auto& arg0_ = call_expr_->arguments_.front();
                        const auto& type_den_ =
                                arg0_->GetTypeDenoter()->GetAliased();
                        
                        if (auto base_type_den_ = type_den_.As<BaseTypeDenoter>())
                        {
                            call_expr_->intrinsic_ = Intrinsic::Log;

                            auto literal_expr_ =
                                    ASTFactory::MakeLiteralExpr(DataType::Int,
                                                                "10");
                            literal_expr_->ConvertDataType(
                                    BaseDataType(base_type_den_->data_type_));

                            auto rhs_expr_ = ASTFactory::MakeIntrinsicCallExpr(
                                    Intrinsic::Log,
                                    "log",
                                    nullptr,
                                    {literal_expr_});

                            auto binary_expr_ =
                                    ASTFactory::MakeBinaryExpr(expr_a,
                                                               BinaryOp::Div,
                                                               rhs_expr_);

                            expr_a = ASTFactory::MakeBracketExpr(binary_expr_);
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprTargetType(
                    ExprPtr& expr_a,
                    const TypeDenoter& target_type_den_a,
                    bool match_type_size_a)
            {
                if (expr_a)
                {
                    if (conversion_flags_(ConvertImplicitCasts))
                        ExprConverter::ConvertExprIfCastRequired(expr_a,
                                                                 target_type_den_a,
                                                                 match_type_size_a);

                    if (auto init_expr_ = expr_a->As<InitializerExpr>())
                    {
                        if (auto array_target_type_den_ =
                                    target_type_den_a.As<ArrayTypeDenoter>())
                        {
                            const auto& sub_type_den_ =
                                    array_target_type_den_->sub_type_denoter_
                                            ->GetAliased();
                            
                            for (auto& expr_ : init_expr_->exprs_)
                                ConvertExprTargetType(expr_, sub_type_den_);
                        }

                        if (conversion_flags_(ConvertInitializerToCtor))
                            ConvertExprTargetTypeInitializer(expr_a,
                                                            init_expr_,
                                                              target_type_den_a);
                        else
                            ConvertExprFormatInitializer(expr_a,
                                                         init_expr_,
                                                         target_type_den_a);
                    }
                }
            }

            void ExprConverter::ConvertExprTargetTypeInitializer(
                    ExprPtr& expr_a,
                    InitializerExpr* init_expr_a,
                    const TypeDenoter& target_type_den_a)
            {
                expr_a = ASTFactory::MakeTypeCtorCallExpr(target_type_den_a.Copy(),
                                                        init_expr_a->exprs_);
            }

            void ExprConverter::ConvertExprFormatInitializer(
                    ExprPtr& expr_a,
                    InitializerExpr* init_expr_a,
                    const TypeDenoter& target_type_den_a)
            {
                (void)expr_a;
                if (auto base_target_type_den_ =
                            target_type_den_a.As<BaseTypeDenoter>())
                {
                    if (IsMatrixType(base_target_type_den_->data_type_))
                    {
                        const auto dims_ = MatrixTypeDim(
                                base_target_type_den_->data_type_);
                        
                        const auto num_entries_ = static_cast<std::size_t>(
                                dims_.first * dims_.second);

                        if (num_entries_ == init_expr_a->exprs_.size())
                        {
                            auto row_type_denoter_ =
                                    std::make_shared<BaseTypeDenoter>();
                            row_type_denoter_->data_type_ = VectorDataType(
                                    BaseDataType(base_target_type_den_->data_type_),
                                    dims_.second);

                            std::vector<ExprPtr> sub_init_exprs_;

                            std::size_t idx_ = 0;
                            for (int row_ = 0; row_ < dims_.first; ++row_)
                            {
                                std::vector<ExprPtr> row_init_exprs_;
                                for (int col_ = 0; col_ < dims_.second; ++col_)
                                    row_init_exprs_.push_back(
                                            init_expr_a->exprs_[idx_++]);

                                auto row_init_ = ASTFactory::MakeInitializerExpr(
                                        row_init_exprs_);
                                {
                                    row_init_->GetTypeDenoter(
                                            row_type_denoter_.get());
                                }
                                sub_init_exprs_.push_back(row_init_);
                            }

                            init_expr_a->exprs_ = std::move(sub_init_exprs_);
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprTextureBracketOp(ExprPtr& expr_a)
            {
                if (!expr_a->flags_(Expr::WasConverted)
                    && expr_a->Type() == AST::Types::ArrayExpr)
                {
                    if (auto array_expr_ = std::static_pointer_cast<ArrayExpr>(
                                expr_a))
                    {
                        for (std::size_t i_ = 0;
                             i_ + 1u < array_expr_->NumIndices();
                             ++i_)
                        {
                            auto type_den_ =
                                    array_expr_->prefix_expr_->GetTypeDenoter()
                                            ->GetSubArray(
                                                    array_expr_->NumIndices() - i_
                                                    - 1);
                            if (auto buffer_type_den_ =
                                        type_den_->As<BufferTypeDenoter>())
                            {
                                if (!IsTextureBufferType(
                                            buffer_type_den_->buffer_type_))
                                {
                                    return;
                                }

                                if (i_ > 0)
                                {
                                    array_expr_ = ASTFactory::MakeArrayExprSplit(
                                            array_expr_,
                                            array_expr_->NumIndices() - i_);
                                   
                                    ConvertExprTextureBracketOp(
                                            array_expr_->prefix_expr_);
                                    expr_a = array_expr_;
                                }
                                break;
                            }
                        }

                        auto type_den_ = array_expr_->prefix_expr_->GetTypeDenoter()
                                               ->GetSubArray(
                                                       array_expr_->NumIndices()
                                                       - 1);
                        if (auto buffer_type_den_ =
                                    type_den_->As<BufferTypeDenoter>())
                        {
                            if (!IsTextureBufferType(buffer_type_den_->buffer_type_))
                            {
                                return;
                            }

                            expr_a->flags_ << Expr::WasConverted;

                            auto call_expr_ = ASTFactory::MakeIntrinsicCallExpr(
                                    Intrinsic::Texture_Load_1,
                                    "Load",
                                    nullptr,
                                    {array_expr_->array_indices_.back()});

                            ExprPtr prefix_expr_;

                            array_expr_->array_indices_.pop_back();

                            if (array_expr_->array_indices_.empty())
                                call_expr_->prefix_expr_ = array_expr_->prefix_expr_;
                            else
                                call_expr_->prefix_expr_ = expr_a;

                            call_expr_->prefix_expr_->ResetTypeDenoter();
                            expr_a = call_expr_;
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprTextureIntrinsicVec4(ExprPtr& expr_a)
            {
                if (auto call_expr_ = expr_a->As<CallExpr>())
                {
                    const auto intrinsic_ = call_expr_->intrinsic_;
                    if (intrinsic_ != Intrinsic::Undefined)
                    {
                        bool is_sample_intrinsic_ = IsTextureSampleIntrinsic(
                                intrinsic_);
                        if (IsTextureLoadIntrinsic(intrinsic_)
                            || is_sample_intrinsic_
                            || IsTextureGatherIntrisic(intrinsic_))
                        {
                            if (!is_sample_intrinsic_
                                || !IsTextureCompareIntrinsic(intrinsic_))
                            {
                                const auto& type_den_ = call_expr_->GetTypeDenoter()
                                                              ->GetAliased();
                                if (auto base_type_den_ =
                                            type_den_.As<BaseTypeDenoter>())
                                {
                                    const auto vec_type_dim_ = VectorTypeDim(
                                            base_type_den_->data_type_);
                                    
                                    if (vec_type_dim_ >= 1 && vec_type_dim_ <= 3)
                                    {
                                        const std::string vector_subscript_ =
                                                "rgb";

                                        expr_a = ASTFactory::MakeObjectExpr(
                                                expr_a,
                                                vector_subscript_.substr(
                                                        0,
                                                        static_cast<
                                                                std::size_t>(
                                                                vec_type_dim_)));
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void ExprConverter::ConvertExprCompatibleStruct(ExprPtr& expr_a)
            {
                if (auto object_expr_ = expr_a->As<ObjectExpr>())
                {
                    if (object_expr_->prefix_expr_ && object_expr_->symbol_ref_)
                    {
                        if (auto var_decl_ = object_expr_->symbol_ref_->As<VarDecl>())
                        {
                            const auto& prefix_type_den_ =
                                    object_expr_->prefix_expr_->GetTypeDenoter()
                                            ->GetAliased();
                            
                            if (auto prefix_struct_type_den_ =
                                        prefix_type_den_.As<StructTypeDenoter>())
                            {
                                if (auto struct_decl_ =
                                            prefix_struct_type_den_->struct_decl_ref_)
                                {
                                    if (auto compat_struct_ =
                                                struct_decl_->compatible_struct_ref_)
                                    {
                                        auto idx_ = struct_decl_->MemberVarToIndex(
                                                var_decl_);
                                        
                                        if (auto compat_var_decl_ =
                                                    compat_struct_
                                                            ->IndexToMemberVar(
                                                                    idx_))
                                        {
                                            object_expr_->ReplaceSymbol(
                                                    compat_var_decl_);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel