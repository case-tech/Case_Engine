
// Generated from HLSL.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "HLSLVisitor.h"


/**
 * This class provides an empty implementation of HLSLVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  HLSLBaseVisitor : public HLSLVisitor {
public:

  virtual std::any visitHlslFile(HLSLParser::HlslFileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElement(HLSLParser::ElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDeclaration(HLSLParser::StructDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructMember(HLSLParser::StructMemberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitType(HLSLParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDeclaration(HLSLParser::FunctionDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParameterList(HLSLParser::ParameterListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParameter(HLSLParser::ParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(HLSLParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(HLSLParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration(HLSLParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignment(HLSLParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStatement(HLSLParser::ReturnStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(HLSLParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionCall(HLSLParser::FunctionCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstructorExpression(HLSLParser::ConstructorExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgumentList(HLSLParser::ArgumentListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSemantic(HLSLParser::SemanticContext *ctx) override {
    return visitChildren(ctx);
  }


};

