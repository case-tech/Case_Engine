// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/SourceArea.hpp"
#include "../AST/Token.hpp"
#include "../AST/TokenString.hpp"
#include "../HTG.hpp"
#include "../Log.hpp"
#include "../SourceCode.hpp"

#include <functional>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Scanner
            {
            public:
                Scanner(Log* log_a = nullptr);
                virtual ~Scanner() = default;

                bool ScanSource(const SourceCodePtr& source_a);

                void PushTokenString(const TokenPtrString& token_string_a);
                void PopTokenString();

                TokenPtrString::ConstIterator TopTokenStringIterator() const;

                virtual TokenPtr Next() = 0;

                TokenPtr ActiveToken() const;
                TokenPtr PreviousToken() const;

                inline const SourcePosition& Pos() const
                {
                    return next_start_pos_;
                }

                inline SourceCode* Source() const
                {
                    return source_.get();
                }

                inline const SourceCodePtr& GetSharedSource() const
                {
                    return source_;
                }

                inline const std::string& GetComment() const
                {
                    return comment_;
                }

            protected:
                using Tokens = Token::Types;

                TokenPtr NextToken(bool scan_comments_a, bool scan_white_spaces_a);

                void StoreStartPos();

                virtual TokenPtr ScanToken() = 0;

                char Take(char chr_a);
                char TakeIt();

                TokenPtr Make(const Token::Types& type_a, bool take_chr_a = false);
                TokenPtr Make(const Token::Types& type_a,
                              std::string& spell_a,
                              bool take_chr_a = false);
                
                TokenPtr Make(const Token::Types& type_a,
                              std::string& spell_a,
                              const SourcePosition& pos_a,
                              bool take_chr_a = false);

                [[noreturn]]
                void Error(const std::string& msg_a);

                [[noreturn]]
                void ErrorUnexpected();

                [[noreturn]]
                void ErrorUnexpected(char expected_char_a);

                [[noreturn]]
                void ErrorUnexpectedEOS();

                void Ignore(const std::function<bool(char)>& pred_a);
                void IgnoreWhiteSpaces(bool include_new_lines_a = true);

                TokenPtr ScanWhiteSpaces(bool include_new_lines_a = true);
                TokenPtr ScanCommentLine(bool scan_comments_a);
                TokenPtr ScanCommentBlock(bool scan_comments_a);
                TokenPtr ScanStringLiteral();
                TokenPtr ScanCharLiteral();
                TokenPtr ScanNumber(bool start_with_period_a = false);
                TokenPtr ScanNumberOrDot();
                TokenPtr ScanVarArg(std::string& spell_a);

                bool ScanDigitSequence(std::string& spell_a);

                inline bool IsNewLine() const
                {
                    return (chr_ == '\n' || chr_ == '\r');
                }

                inline bool Is(char chr_a) const
                {
                    return (chr_ == chr_a);
                }

                inline char Chr() const
                {
                    return chr_;
                }

                inline unsigned char UChr() const
                {
                    return static_cast<unsigned char>(chr_);
                }

            private:
                TokenPtr NextTokenScan(bool scan_comments_a, bool scan_white_spaces_a);

                void AppendComment(const std::string& s_a);
                void AppendMultiLineComment(const std::string& s_a);

            private:
                SourceCodePtr source_;
                char chr_ = 0;

                Log* log_ = nullptr;

                SourcePosition next_start_pos_;
                TokenPtr active_token_;
                TokenPtr prev_token_;

                std::vector<TokenPtrString::ConstIterator> token_string_it_stack_;

                std::string comment_;
                unsigned int comment_start_pos_ = 0;
                bool comment_first_line_ = false;
            };

            using ScannerPtr = std::shared_ptr<Scanner>;
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel