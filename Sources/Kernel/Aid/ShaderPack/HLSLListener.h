
// Generated from HLSL.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "HLSLParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by HLSLParser.
 */
class  HLSLListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterHlslFile(HLSLParser::HlslFileContext *ctx) = 0;
  virtual void exitHlslFile(HLSLParser::HlslFileContext *ctx) = 0;

  virtual void enterElement(HLSLParser::ElementContext *ctx) = 0;
  virtual void exitElement(HLSLParser::ElementContext *ctx) = 0;

  virtual void enterStructDeclaration(HLSLParser::StructDeclarationContext *ctx) = 0;
  virtual void exitStructDeclaration(HLSLParser::StructDeclarationContext *ctx) = 0;

  virtual void enterStructMember(HLSLParser::StructMemberContext *ctx) = 0;
  virtual void exitStructMember(HLSLParser::StructMemberContext *ctx) = 0;

  virtual void enterType(HLSLParser::TypeContext *ctx) = 0;
  virtual void exitType(HLSLParser::TypeContext *ctx) = 0;

  virtual void enterFunctionDeclaration(HLSLParser::FunctionDeclarationContext *ctx) = 0;
  virtual void exitFunctionDeclaration(HLSLParser::FunctionDeclarationContext *ctx) = 0;

  virtual void enterParameterList(HLSLParser::ParameterListContext *ctx) = 0;
  virtual void exitParameterList(HLSLParser::ParameterListContext *ctx) = 0;

  virtual void enterParameter(HLSLParser::ParameterContext *ctx) = 0;
  virtual void exitParameter(HLSLParser::ParameterContext *ctx) = 0;

  virtual void enterBlock(HLSLParser::BlockContext *ctx) = 0;
  virtual void exitBlock(HLSLParser::BlockContext *ctx) = 0;

  virtual void enterStatement(HLSLParser::StatementContext *ctx) = 0;
  virtual void exitStatement(HLSLParser::StatementContext *ctx) = 0;

  virtual void enterDeclaration(HLSLParser::DeclarationContext *ctx) = 0;
  virtual void exitDeclaration(HLSLParser::DeclarationContext *ctx) = 0;

  virtual void enterAssignment(HLSLParser::AssignmentContext *ctx) = 0;
  virtual void exitAssignment(HLSLParser::AssignmentContext *ctx) = 0;

  virtual void enterReturnStatement(HLSLParser::ReturnStatementContext *ctx) = 0;
  virtual void exitReturnStatement(HLSLParser::ReturnStatementContext *ctx) = 0;

  virtual void enterExpression(HLSLParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(HLSLParser::ExpressionContext *ctx) = 0;

  virtual void enterFunctionCall(HLSLParser::FunctionCallContext *ctx) = 0;
  virtual void exitFunctionCall(HLSLParser::FunctionCallContext *ctx) = 0;

  virtual void enterConstructorExpression(HLSLParser::ConstructorExpressionContext *ctx) = 0;
  virtual void exitConstructorExpression(HLSLParser::ConstructorExpressionContext *ctx) = 0;

  virtual void enterArgumentList(HLSLParser::ArgumentListContext *ctx) = 0;
  virtual void exitArgumentList(HLSLParser::ArgumentListContext *ctx) = 0;

  virtual void enterSemantic(HLSLParser::SemanticContext *ctx) = 0;
  virtual void exitSemantic(HLSLParser::SemanticContext *ctx) = 0;


};

