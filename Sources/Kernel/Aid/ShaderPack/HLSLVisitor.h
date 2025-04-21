
// Generated from HLSL.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "HLSLParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by HLSLParser.
 */
class  HLSLVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by HLSLParser.
   */
    virtual std::any visitHlslFile(HLSLParser::HlslFileContext *context) = 0;

    virtual std::any visitElement(HLSLParser::ElementContext *context) = 0;

    virtual std::any visitStructDeclaration(HLSLParser::StructDeclarationContext *context) = 0;

    virtual std::any visitStructMember(HLSLParser::StructMemberContext *context) = 0;

    virtual std::any visitType(HLSLParser::TypeContext *context) = 0;

    virtual std::any visitFunctionDeclaration(HLSLParser::FunctionDeclarationContext *context) = 0;

    virtual std::any visitParameterList(HLSLParser::ParameterListContext *context) = 0;

    virtual std::any visitParameter(HLSLParser::ParameterContext *context) = 0;

    virtual std::any visitBlock(HLSLParser::BlockContext *context) = 0;

    virtual std::any visitStatement(HLSLParser::StatementContext *context) = 0;

    virtual std::any visitDeclaration(HLSLParser::DeclarationContext *context) = 0;

    virtual std::any visitAssignment(HLSLParser::AssignmentContext *context) = 0;

    virtual std::any visitReturnStatement(HLSLParser::ReturnStatementContext *context) = 0;

    virtual std::any visitExpression(HLSLParser::ExpressionContext *context) = 0;

    virtual std::any visitFunctionCall(HLSLParser::FunctionCallContext *context) = 0;

    virtual std::any visitConstructorExpression(HLSLParser::ConstructorExpressionContext *context) = 0;

    virtual std::any visitArgumentList(HLSLParser::ArgumentListContext *context) = 0;

    virtual std::any visitSemantic(HLSLParser::SemanticContext *context) = 0;


};

