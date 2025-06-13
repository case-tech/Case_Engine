#include "ASTPrinter.hpp"

#include "../../ConsoleManip.hpp"
#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"

#include <algorithm>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void ASTPrinter::PrintAST(Program* program_a, std::ostream& output_a)
            {
                Visit(program_a);

                for (const auto& child_ : tree_root_.children_)
                    Print(child_, output_a);

                tree_root_.label_.clear();
                tree_root_.children_.clear();
            }

            template <typename T>
            inline std::string MemberToString(const T& member_a)
            {
                return "";
            }

            template <>
            inline std::string MemberToString<Identifier>(
                    const Identifier& member_a)
            {
                return member_a.Original();
            }

            template <>
            inline std::string MemberToString<std::string>(
                    const std::string& member_a)
            {
                return member_a;
            }

            template <>
            inline std::string MemberToString<int>(const int& member_a)
            {
                return std::to_string(member_a);
            }

            template <>
            inline std::string MemberToString<bool>(const bool& member_a)
            {
                if (member_a)
                    return "true";
                else
                    return "false";
            }

            template <>
            inline std::string MemberToString<IndexedSemantic>(
                    const IndexedSemantic& member_a)
            {
                return member_a.ToString();
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void ASTPrinter::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

#define VISIT_MEMBER(MEMBER) VisitMember(ast_a->MEMBER, #MEMBER)

#define ADD_PRINTABLE_MEMBER(MEMBER)                                           \
    Printable(ast_a, std::string(#MEMBER), MemberToString(ast_a->MEMBER))

            IMPLEMENT_VISIT_PROC(Program)
            {
                PushPrintable(ast_a, WriteLabel("Program"));
                {
                    VISIT_MEMBER(global_stmnts_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                PushPrintable(ast_a, WriteLabel("CodeBlock"));
                {
                    VISIT_MEMBER(stmnts_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(Attribute)
            {
                PushPrintable(ast_a, WriteLabel("Attribute"));
                {
                    Printable(ast_a, "attributeType", ast_a->ToString());
                    VISIT_MEMBER(arguments_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(SwitchCase)
            {
                PushPrintable(ast_a, WriteLabel("SwitchCase"));
                {
                    VISIT_MEMBER(expr_);
                    VISIT_MEMBER(stmnts_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(SamplerValue)
            {
                PushPrintable(ast_a, WriteLabel("SamplerValue"));
                {
                    VISIT_MEMBER(value_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(Register)
            {
                PushPrintable(ast_a, WriteLabel("Register"));
                {
                    Printable(ast_a,
                              "registerType",
                              std::string(1,
                                          RegisterTypeToChar(
                                                  ast_a->register_type_)));
                    ADD_PRINTABLE_MEMBER(slot_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(PackOffset)
            {
                PushPrintable(ast_a, WriteLabel("PackOffset"));
                {
                    ADD_PRINTABLE_MEMBER(register_name_);
                    ADD_PRINTABLE_MEMBER(vector_component_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ArrayDAzension)
            {
                PushPrintable(ast_a, WriteLabel("ArrayDAzension"));
                {
                    VISIT_MEMBER(expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifier)
            {
                PushPrintable(ast_a, WriteLabel("TypeSpecifier", ast_a));
                {
                    VISIT_MEMBER(struct_decl_);
                    Printable(ast_a, "typeDenoter", ast_a->ToString());
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                PushPrintable(ast_a, WriteLabel("VarDecl", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    VISIT_MEMBER(namespace_expr_);
                    VISIT_MEMBER(array_dAzs_);
                    VISIT_MEMBER(slot_registers_);
                    VISIT_MEMBER(pack_offset_);
                    VISIT_MEMBER(annotations_);
                    VISIT_MEMBER(initializer_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                PushPrintable(ast_a, WriteLabel("BufferDecl", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    VISIT_MEMBER(array_dAzs_);
                    VISIT_MEMBER(slot_registers_);
                    VISIT_MEMBER(annotations_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                PushPrintable(ast_a, WriteLabel("SamplerDecl", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    VISIT_MEMBER(array_dAzs_);
                    VISIT_MEMBER(slot_registers_);
                    VISIT_MEMBER(sampler_values_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                PushPrintable(ast_a, WriteLabel("StructDecl", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    ADD_PRINTABLE_MEMBER(base_struct_name_);
                    VISIT_MEMBER(local_stmnts_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(AliasDecl)
            {
                if (!ast_a->flags_(AST::IsBuiltin))
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    Printable(ast_a, WriteLabel("AliasDecl", ast_a));
                }
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                PushPrintable(ast_a, WriteLabel("FunctionDecl", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    VISIT_MEMBER(return_type_);
                    VISIT_MEMBER(parameters_);
                    if (ast_a->semantic_.IsValid())
                        ADD_PRINTABLE_MEMBER(semantic_);
                    VISIT_MEMBER(annotations_);
                    VISIT_MEMBER(code_block_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                PushPrintable(ast_a, WriteLabel("UniformBufferDecl", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(ident_);
                    Printable(ast_a,
                              "bufferType",
                              std::string(ast_a->buffer_type_
                                                          == UniformBufferType::
                                                                  ConstantBuffer
                                                  ? "cbuffer"
                                                  : "tbuffer"));
                    VISIT_MEMBER(slot_registers_);
                    VISIT_MEMBER(local_stmnts_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                PushPrintable(ast_a, WriteLabel("BufferDeclStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(buffer_decls_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(SamplerDeclStmnt)
            {
                PushPrintable(ast_a, WriteLabel("SamplerDeclStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(sampler_decls_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                PushPrintable(ast_a, WriteLabel("VarDeclStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(type_specifier_);
                    VISIT_MEMBER(var_decls_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(AliasDeclStmnt)
            {
                if (!ast_a->flags_(AST::IsBuiltin))
                {
                    PushPrintable(ast_a, WriteLabel("AliasDeclStmnt"));
                    {
                        VISIT_MEMBER(attribs_);
                        VISIT_MEMBER(struct_decl_);
                        VISIT_MEMBER(alias_decls_);
                    }
                    PopPrintable();
                }
            }

            IMPLEMENT_VISIT_PROC(BasicDeclStmnt)
            {
                PushPrintable(ast_a, WriteLabel("BasicDeclStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(decl_object_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(NullStmnt)
            {
                PushPrintable(ast_a, WriteLabel("NullStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
            {
                PushPrintable(ast_a, WriteLabel("CodeBlockStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(code_block_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                PushPrintable(ast_a, WriteLabel("ForLoopStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(init_stmnt_);
                    VISIT_MEMBER(condition_);
                    VISIT_MEMBER(iteration_);
                    VISIT_MEMBER(body_stmnt_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                PushPrintable(ast_a, WriteLabel("WhileLoopStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(condition_);
                    VISIT_MEMBER(body_stmnt_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                PushPrintable(ast_a, WriteLabel("DoWhileLoopStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(body_stmnt_);
                    VISIT_MEMBER(condition_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                PushPrintable(ast_a, WriteLabel("IfStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(condition_);
                    VISIT_MEMBER(body_stmnt_);
                    VISIT_MEMBER(else_stmnt_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                PushPrintable(ast_a, WriteLabel("ElseStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(body_stmnt_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                PushPrintable(ast_a, WriteLabel("SwitchStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(selector_);
                    VISIT_MEMBER(cases_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                PushPrintable(ast_a, WriteLabel("ExprStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                PushPrintable(ast_a, WriteLabel("ReturnStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    VISIT_MEMBER(expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(CtrlTransferStmnt)
            {
                PushPrintable(ast_a, WriteLabel("CtrlTransferStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    Printable(ast_a,
                              "transfer",
                              CtrlTransformToString(ast_a->transfer_));
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(LayoutStmnt)
            {
                PushPrintable(ast_a, WriteLabel("LayoutStmnt"));
                {
                    VISIT_MEMBER(attribs_);
                    Printable(ast_a,
                              "isInput",
                              (ast_a->is_input_ ? "true" : "false"));
                    Printable(ast_a,
                              "isOutput",
                              (ast_a->is_output_ ? "true" : "false"));
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(NullExpr)
            {
                Printable(ast_a, WriteLabel("NullExpr", ast_a));
            }

            IMPLEMENT_VISIT_PROC(SequenceExpr)
            {
                PushPrintable(ast_a, WriteLabel("SequenceExpr", ast_a));
                {
                    VISIT_MEMBER(exprs_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(LiteralExpr)
            {
                PushPrintable(ast_a, WriteLabel("LiteralExpr", ast_a));
                {
                    ADD_PRINTABLE_MEMBER(value_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifierExpr)
            {
                PushPrintable(ast_a, WriteLabel("TypeSpecifierExpr", ast_a));
                {
                    VISIT_MEMBER(type_specifier_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                PushPrintable(ast_a, WriteLabel("TernaryExpr", ast_a));
                {
                    VISIT_MEMBER(cond_expr_);
                    VISIT_MEMBER(then_expr_);
                    VISIT_MEMBER(else_expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                PushPrintable(ast_a, WriteLabel("BinaryExpr", ast_a));
                {
                    VISIT_MEMBER(lhs_expr_);
                    Printable(ast_a, "op", BinaryOpToString(ast_a->op_));
                    VISIT_MEMBER(rhs_expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                PushPrintable(ast_a, WriteLabel("UnaryExpr", ast_a));
                {
                    Printable(ast_a, "op", UnaryOpToString(ast_a->op_));
                    VISIT_MEMBER(expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                PushPrintable(ast_a, WriteLabel("PostUnaryExpr", ast_a));
                {
                    VISIT_MEMBER(expr_);
                    Printable(ast_a, "op", UnaryOpToString(ast_a->op_));
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                PushPrintable(ast_a, WriteLabel("CallExpr", ast_a));
                {
                    VISIT_MEMBER(prefix_expr_);
                    ADD_PRINTABLE_MEMBER(is_static_);
                    ADD_PRINTABLE_MEMBER(ident_);
                    VISIT_MEMBER(arguments_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                PushPrintable(ast_a, WriteLabel("BracketExpr", ast_a));
                {
                    VISIT_MEMBER(expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                PushPrintable(ast_a, WriteLabel("AssignExpr", ast_a));
                {
                    VISIT_MEMBER(lvalue_expr_);
                    Printable(ast_a, "op", AssignOpToString(ast_a->op_));
                    VISIT_MEMBER(rvalue_expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                PushPrintable(ast_a, WriteLabel("ObjectExpr", ast_a));
                {
                    VISIT_MEMBER(prefix_expr_);
                    ADD_PRINTABLE_MEMBER(is_static_);
                    ADD_PRINTABLE_MEMBER(ident_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                PushPrintable(ast_a, WriteLabel("ArrayExpr", ast_a));
                {
                    VISIT_MEMBER(prefix_expr_);
                    VISIT_MEMBER(array_indices_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                PushPrintable(ast_a, WriteLabel("CastExpr", ast_a));
                {
                    VISIT_MEMBER(type_specifier_);
                    VISIT_MEMBER(expr_);
                }
                PopPrintable();
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                PushPrintable(ast_a, WriteLabel("InitializerExpr", ast_a));
                {
                    VISIT_MEMBER(exprs_);
                }
                PopPrintable();
            }

#undef AzPLEMENT_VISIT_PROC
#undef VISIT_MEMBER
#undef ADD_PRINTABLE_MEMBER

            std::string ASTPrinter::WriteLabel(const std::string& ast_name_a,
                                               TypedAST* ast_a)
            {
                std::string s_;

                const auto& member_name_ = TopMemberName();
                if (!member_name_.empty())
                {
                    s_ += member_name_;
                    s_ += " : ";
                }

                s_ += ast_name_a;

                if (ast_a)
                {
                    s_ += " <";

                    try
                    {
                        s_ += ast_a->GetTypeDenoter()->ToString();
                    } 
                    catch (const std::exception&)
                    {
                        s_ += R_Unspecified;
                    }

                    s_ += '>';
                }

                return s_;
            }

            void ASTPrinter::Print(const PrintableTree& tree_a,
                                   std::ostream& output_a)
            {
                using Colors = ConsoleManip::ColorFlags;

                {
                    ConsoleManip::ScopedColor highlight(Colors::Green
                                                                | Colors::Blue,
                                                        output_a);

                    output_a << std::string(max_row_str_len_ - tree_a.row_.size(), ' ');
                    output_a << tree_a.row_;
                    output_a << ':';

                    output_a << std::string(max_col_str_len_ - tree_a.col_.size(), ' ');
                    output_a << tree_a.col_;
                    output_a << "  ";
                }

                if (!last_sub_node_stack_.empty())
                {
                    for (std::size_t i_ = 0; i_ + 1 < last_sub_node_stack_.size();
                         ++i_)
                    {
                        if (last_sub_node_stack_[i_])
                            output_a << "  ";
                        else
                            output_a << "| ";
                    }

                    if (last_sub_node_stack_.back())
                        output_a << "`-";
                    else
                        output_a << "|-";
                }

                output_a << tree_a.label_;

                if (!tree_a.value_.empty())
                {
                    output_a << " : ";
                    ConsoleManip::ScopedColor highlight_(Colors::Yellow, output_a);
                    output_a << tree_a.value_;
                }

                output_a << std::endl;

                if (!tree_a.children_.empty())
                {
                    last_sub_node_stack_.push_back(false);
                    {
                        for (std::size_t i_ = 0, n_ = tree_a.children_.size();
                             i_ + 1 < n_;
                             ++i_)
                            Print(tree_a.children_[i_], output_a);

                        last_sub_node_stack_.back() = true;
                        Print(tree_a.children_.back(), output_a);
                    }
                    last_sub_node_stack_.pop_back();
                }
            }

            bool ASTPrinter::PushPrintable(const AST* ast_a,
                                           const std::string& label_a,
                                           const std::string& value_a)
            {
                if (!label_a.empty())
                {
                    const auto& pos_ = ast_a->area_.Pos();

                    const auto row_str_ = std::to_string(
                            pos_.GetOrigin()
                                    ? pos_.Row() + pos_.GetOrigin()->line_offset_
                                    : pos_.Row());
                    const auto col_str_ = std::to_string(pos_.Column());

                    max_row_str_len_ = std::max(max_row_str_len_, row_str_.size());
                    max_col_str_len_ = std::max(max_col_str_len_, col_str_.size());

                    auto& children_ = TopPrintable()->children_;
                    children_.push_back({row_str_, col_str_, label_a, value_a, {}});
                    parent_node_stack_.push(&(children_.back()));

                    return true;
                }

                return false;
            }

            void ASTPrinter::PopPrintable()
            {
                parent_node_stack_.pop();
            }

            void ASTPrinter::Printable(const AST* ast_a,
                                       const std::string& label_a,
                                       const std::string& value_a)
            {
                PushPrintable(ast_a, label_a, value_a);
                PopPrintable();
            }

            ASTPrinter::PrintableTree* ASTPrinter::TopPrintable()
            {
                if (parent_node_stack_.empty())
                    return (&tree_root_);
                else
                    return parent_node_stack_.top();
            }

            void ASTPrinter::PushMemberName(const std::string& name_a)
            {
                member_name_stack_.push(name_a);
            }

            void ASTPrinter::PopMemberName()
            {
                if (!member_name_stack_.empty())
                    member_name_stack_.pop();
            }

            const std::string& ASTPrinter::TopMemberName() const
            {
                if (member_name_stack_.empty())
                {
                    static const std::string default_name_;
                    return default_name_;
                } 
                else
                    return member_name_stack_.top();
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
