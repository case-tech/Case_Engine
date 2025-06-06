#include "IndentHandler.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct IndentHandler::OpaqueData
            {
                std::string indent_;
                std::string indent_full_;
                std::stack<std::string::size_type> indent_stack_;
            };

            IndentHandler::IndentHandler(const std::string& initial_indent_a)
                : data_ {new OpaqueData()}
            {
                data_->indent_ = initial_indent_a;
            }

            IndentHandler::~IndentHandler()
            {
                delete data_;
            }

            void IndentHandler::SetIndent(const std::string& indent_a)
            {
                data_->indent_ = indent_a;
            }

            void IndentHandler::IncIndent()
            {
                data_->indent_full_ += data_->indent_;
                data_->indent_stack_.push(data_->indent_.size());
            }

            void IndentHandler::DecIndent()
            {
                if (!data_->indent_stack_.empty())
                {
                    auto size_ = data_->indent_stack_.top();
                    data_->indent_full_.resize(data_->indent_full_.size() - size_);
                    data_->indent_stack_.pop();
                }
            }

            const std::string& IndentHandler::FullIndent() const
            {
                return data_->indent_full_;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
