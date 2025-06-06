// Copyright (c) 2025 Case Technologies

#pragma once
#include "SourceArea.hpp"

#include <map>
#include <memory>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Token
            {
            public:
                enum class Types
                {
                    Undefined,
                    Ident,
                    BoolLiteral,
                    IntLiteral,
                    FloatLiteral,
                    StringLiteral,
                    CharLiteral,
                    NullLiteral,

                    AssignOp,
                    BinaryOp,
                    UnaryOp,
                    TernaryOp,

                    Dot,
                    Colon,
                    DColon,
                    Semicolon,
                    Comma,

                    LBracket,
                    RBracket,
                    LCurly,
                    RCurly,
                    LParen,
                    RParen,

                    StringType,
                    ScalarType,
                    VectorType,
                    MatrixType,

                    UniformBuffer,
                    Sampler,

                    Void,

                    Reserved,
                    Unsupported,

                    Do,
                    While,
                    For,

                    If,
                    Else,

                    Switch,
                    Case,
                    Default,

                    Struct,
                    Class,
                    CtrlTransfer,
                    Return,

                    InputModifier,
                    InterpModifier,
                    TypeModifier,
                    StorageClass,

                    Inline,

                    SamplerState,

                    Typedef,
                    Register,
                    PackOffset,

                    Buffer,
                    PrimitiveType,
                    Vector,
                    Matrix,

                    Attribute,
                    Varying,
                    Precision,
                    LayoutQualifier,
                    MemoryQualifier,
                    InvariantQualifier,
                    PrecisionQualifier,

                    Image,
                    StorageBuffer,
                    AtomicCounter,

                    Technique,
                    Pass,
                    Compile,

                    Directive,
                    DirectiveConcat,
                    Comment,
                    WhiteSpace,
                    NewLine,
                    LineBreak,
                    VarArg,
                    Misc,

                    EndOfStream,
                };

                Token(Token&& other_a);

                Token(const SourcePosition& pos_a, const Types type_a);
                Token(const SourcePosition& pos_a,
                      const Types type_a,
                      const std::string& spell_a);
               
                Token(const SourcePosition& pos_a,
                      const Types type_a,
                      std::string&& spell_a);

                SourceArea Area() const;

                static std::string TypeToString(const Types type_a);

                std::string SpellContent() const;

                inline Types Type() const
                {
                    return type_;
                }

                inline const SourcePosition& Pos() const
                {
                    return pos_;
                }

                inline const std::string& Spell() const
                {
                    return spell_;
                }

            private:
                Types type_;
                SourcePosition pos_;
                std::string spell_;
            };

            using TokenPtr = std::shared_ptr<Token>;
            using KeywordMapType = std::map<std::string, Token::Types>;
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel