
// Generated from HLSL.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "HLSLListener.h"


/**
 * This class provides an empty implementation of HLSLListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  HLSLBaseListener : public HLSLListener {
public:

  virtual void enterHlslFile(HLSLParser::HlslFileContext * /*ctx*/) override { }
  virtual void exitHlslFile(HLSLParser::HlslFileContext * /*ctx*/) override { }

  virtual void enterElement(HLSLParser::ElementContext * /*ctx*/) override { }
  virtual void exitElement(HLSLParser::ElementContext * /*ctx*/) override { }

  virtual void enterStructDeclaration(HLSLParser::StructDeclarationContext * /*ctx*/) override { }
  virtual void exitStructDeclaration(HLSLParser::StructDeclarationContext * /*ctx*/) override { }

  virtual void enterStructMember(HLSLParser::StructMemberContext * /*ctx*/) override { }
  virtual void exitStructMember(HLSLParser::StructMemberContext * /*ctx*/) override { }

  virtual void enterType(HLSLParser::TypeContext * /*ctx*/) override { }
  virtual void exitType(HLSLParser::TypeContext * /*ctx*/) override { }

  virtual void enterFunctionDeclaration(HLSLParser::FunctionDeclarationContext * /*ctx*/) override { }
  virtual void exitFunctionDeclaration(HLSLParser::FunctionDeclarationContext * /*ctx*/) override { }

  virtual void enterParameterList(HLSLParser::ParameterListContext * /*ctx*/) override { }
  virtual void exitParameterList(HLSLParser::ParameterListContext * /*ctx*/) override { }

  virtual void enterParameter(HLSLParser::ParameterContext * /*ctx*/) override { }
  virtual void exitParameter(HLSLParser::ParameterContext * /*ctx*/) override { }

  virtual void enterBlock(HLSLParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(HLSLParser::BlockContext * /*ctx*/) override { }

  virtual void enterStatement(HLSLParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(HLSLParser::StatementContext * /*ctx*/) override { }

  virtual void enterDeclaration(HLSLParser::DeclarationContext * /*ctx*/) override { }
  virtual void exitDeclaration(HLSLParser::DeclarationContext * /*ctx*/) override { }

  virtual void enterAssignment(HLSLParser::AssignmentContext * /*ctx*/) override { }
  virtual void exitAssignment(HLSLParser::AssignmentContext * /*ctx*/) override { }

  virtual void enterReturnStatement(HLSLParser::ReturnStatementContext * /*ctx*/) override { }
  virtual void exitReturnStatement(HLSLParser::ReturnStatementContext * /*ctx*/) override { }

  virtual void enterExpression(HLSLParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(HLSLParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterFunctionCall(HLSLParser::FunctionCallContext * /*ctx*/) override { }
  virtual void exitFunctionCall(HLSLParser::FunctionCallContext * /*ctx*/) override { }

  virtual void enterConstructorExpression(HLSLParser::ConstructorExpressionContext * /*ctx*/) override { }
  virtual void exitConstructorExpression(HLSLParser::ConstructorExpressionContext * /*ctx*/) override { }

  virtual void enterArgumentList(HLSLParser::ArgumentListContext * /*ctx*/) override { }
  virtual void exitArgumentList(HLSLParser::ArgumentListContext * /*ctx*/) override { }

  virtual void enterSemantic(HLSLParser::SemanticContext * /*ctx*/) override { }
  virtual void exitSemantic(HLSLParser::SemanticContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

