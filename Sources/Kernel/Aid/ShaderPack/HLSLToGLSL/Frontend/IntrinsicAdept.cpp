#include "IntrinsicAdept.hpp"

#include "../AST/AST.hpp"
#include "../Exception.hpp"
#include "../Report/ReportIdents.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            thread_local static IntrinsicAdept* g_intrinsic_adept_instance_ =
                    nullptr;

            IntrinsicAdept::IntrinsicAdept()
            {
                g_intrinsic_adept_instance_ = this;
            }

            IntrinsicAdept::~IntrinsicAdept()
            {
                g_intrinsic_adept_instance_ = nullptr;
            }

            const IntrinsicAdept& IntrinsicAdept::Get()
            {
                return *g_intrinsic_adept_instance_;
            }

            const std::string& IntrinsicAdept::GetIntrinsicIdent(
                    const Intrinsic intrinsic_a) const
            {
                static const std::string unknwon_intrinsic_ = R_Undefined();
                const auto idx_ = INTRINSIC_IDX(intrinsic_a);
                return (idx_ < intrinsic_idents_.size() ? intrinsic_idents_[idx_]
                                                      : unknwon_intrinsic_);
            }

            void IntrinsicAdept::SetIntrinsicIdent(const Intrinsic intrinsic_a,
                                                   const std::string& ident_a)
            {
                const auto idx_ = INTRINSIC_IDX(intrinsic_a);
                if (idx_ < intrinsic_idents_.size())
                    intrinsic_idents_[idx_] = ident_a;
            }

            void IntrinsicAdept::FillOverloadedIntrinsicIdents()
            {
                const std::string* prev_ident_ = nullptr;

                for (auto& ident_ : intrinsic_idents_)
                {
                    if (ident_.empty())
                    {
                        if (prev_ident_)
                            ident_ = *prev_ident_;
                    } 
                    else
                        prev_ident_ = (&ident_);
                }
            }

            [[noreturn]]
            void IntrinsicAdept::ThrowAmbiguousIntrinsicCall(
                    const Intrinsic intrinsic_a,
                    const std::vector<ExprPtr>& args_a)
            {
                std::string s_;

                s_ += GetIntrinsicIdent(intrinsic_a);
                s_ += '(';

                for (std::size_t i_ = 0, n_ = args_a.size(); i_ < n_; ++i_)
                {
                    s_ += args_a[i_]->GetTypeDenoter()->ToString();
                    if (i_ + 1 < n_)
                        s_ += ", ";
                }

                s_ += ')';

                RuntAzeErr(R_AmbiguousIntrinsicCall(s_));
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
