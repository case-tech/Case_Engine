
// Generated from HLSL.g4 by ANTLR 4.13.1


#include "HLSLListener.h"
#include "HLSLVisitor.h"

#include "HLSLParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct HLSLParserStaticData final {
  HLSLParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  HLSLParserStaticData(const HLSLParserStaticData&) = delete;
  HLSLParserStaticData(HLSLParserStaticData&&) = delete;
  HLSLParserStaticData& operator=(const HLSLParserStaticData&) = delete;
  HLSLParserStaticData& operator=(HLSLParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag hlslParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
HLSLParserStaticData *hlslParserStaticData = nullptr;

void hlslParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (hlslParserStaticData != nullptr) {
    return;
  }
#else
  assert(hlslParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<HLSLParserStaticData>(
    std::vector<std::string>{
      "hlslFile", "element", "structDeclaration", "structMember", "type", 
      "functionDeclaration", "parameterList", "parameter", "block", "statement", 
      "declaration", "assignment", "returnStatement", "expression", "functionCall", 
      "constructorExpression", "argumentList", "semantic"
    },
    std::vector<std::string>{
      "", "'struct'", "'{'", "'}'", "';'", "':'", "'float'", "'float2'", 
      "'float3'", "'float4'", "'int'", "'matrix'", "'('", "')'", "','", 
      "'='", "'return'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "ID", "DIGIT", "WS", "COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,20,157,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,1,0,5,0,38,8,0,10,0,12,0,41,9,0,1,0,
  	1,0,1,1,1,1,3,1,47,8,1,1,2,1,2,1,2,1,2,4,2,53,8,2,11,2,12,2,54,1,2,1,
  	2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,5,1,5,1,5,1,5,3,5,72,8,5,1,5,
  	1,5,1,5,3,5,77,8,5,1,5,1,5,1,6,1,6,1,6,5,6,84,8,6,10,6,12,6,87,9,6,1,
  	7,1,7,1,7,1,7,1,7,1,8,1,8,5,8,96,8,8,10,8,12,8,99,9,8,1,8,1,8,1,9,1,9,
  	1,9,1,9,1,9,1,9,1,9,1,9,1,9,3,9,112,8,9,1,10,1,10,1,10,1,11,1,11,1,11,
  	1,11,1,12,1,12,1,12,1,13,1,13,1,13,3,13,127,8,13,1,14,1,14,1,14,3,14,
  	132,8,14,1,14,1,14,1,15,1,15,1,15,1,15,1,15,1,16,1,16,1,16,5,16,144,8,
  	16,10,16,12,16,147,9,16,1,17,1,17,4,17,151,8,17,11,17,12,17,152,3,17,
  	155,8,17,1,17,0,0,18,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,
  	0,1,2,0,6,11,17,17,153,0,39,1,0,0,0,2,46,1,0,0,0,4,48,1,0,0,0,6,59,1,
  	0,0,0,8,65,1,0,0,0,10,67,1,0,0,0,12,80,1,0,0,0,14,88,1,0,0,0,16,93,1,
  	0,0,0,18,111,1,0,0,0,20,113,1,0,0,0,22,116,1,0,0,0,24,120,1,0,0,0,26,
  	126,1,0,0,0,28,128,1,0,0,0,30,135,1,0,0,0,32,140,1,0,0,0,34,148,1,0,0,
  	0,36,38,3,2,1,0,37,36,1,0,0,0,38,41,1,0,0,0,39,37,1,0,0,0,39,40,1,0,0,
  	0,40,42,1,0,0,0,41,39,1,0,0,0,42,43,5,0,0,1,43,1,1,0,0,0,44,47,3,4,2,
  	0,45,47,3,10,5,0,46,44,1,0,0,0,46,45,1,0,0,0,47,3,1,0,0,0,48,49,5,1,0,
  	0,49,50,5,17,0,0,50,52,5,2,0,0,51,53,3,6,3,0,52,51,1,0,0,0,53,54,1,0,
  	0,0,54,52,1,0,0,0,54,55,1,0,0,0,55,56,1,0,0,0,56,57,5,3,0,0,57,58,5,4,
  	0,0,58,5,1,0,0,0,59,60,3,8,4,0,60,61,5,17,0,0,61,62,5,5,0,0,62,63,3,34,
  	17,0,63,64,5,4,0,0,64,7,1,0,0,0,65,66,7,0,0,0,66,9,1,0,0,0,67,68,3,8,
  	4,0,68,69,5,17,0,0,69,71,5,12,0,0,70,72,3,12,6,0,71,70,1,0,0,0,71,72,
  	1,0,0,0,72,73,1,0,0,0,73,76,5,13,0,0,74,75,5,5,0,0,75,77,3,34,17,0,76,
  	74,1,0,0,0,76,77,1,0,0,0,77,78,1,0,0,0,78,79,3,16,8,0,79,11,1,0,0,0,80,
  	85,3,14,7,0,81,82,5,14,0,0,82,84,3,14,7,0,83,81,1,0,0,0,84,87,1,0,0,0,
  	85,83,1,0,0,0,85,86,1,0,0,0,86,13,1,0,0,0,87,85,1,0,0,0,88,89,3,8,4,0,
  	89,90,5,17,0,0,90,91,5,5,0,0,91,92,3,34,17,0,92,15,1,0,0,0,93,97,5,2,
  	0,0,94,96,3,18,9,0,95,94,1,0,0,0,96,99,1,0,0,0,97,95,1,0,0,0,97,98,1,
  	0,0,0,98,100,1,0,0,0,99,97,1,0,0,0,100,101,5,3,0,0,101,17,1,0,0,0,102,
  	103,3,20,10,0,103,104,5,4,0,0,104,112,1,0,0,0,105,106,3,22,11,0,106,107,
  	5,4,0,0,107,112,1,0,0,0,108,109,3,24,12,0,109,110,5,4,0,0,110,112,1,0,
  	0,0,111,102,1,0,0,0,111,105,1,0,0,0,111,108,1,0,0,0,112,19,1,0,0,0,113,
  	114,3,8,4,0,114,115,5,17,0,0,115,21,1,0,0,0,116,117,5,17,0,0,117,118,
  	5,15,0,0,118,119,3,26,13,0,119,23,1,0,0,0,120,121,5,16,0,0,121,122,3,
  	26,13,0,122,25,1,0,0,0,123,127,5,17,0,0,124,127,3,28,14,0,125,127,3,30,
  	15,0,126,123,1,0,0,0,126,124,1,0,0,0,126,125,1,0,0,0,127,27,1,0,0,0,128,
  	129,5,17,0,0,129,131,5,12,0,0,130,132,3,32,16,0,131,130,1,0,0,0,131,132,
  	1,0,0,0,132,133,1,0,0,0,133,134,5,13,0,0,134,29,1,0,0,0,135,136,3,8,4,
  	0,136,137,5,12,0,0,137,138,3,32,16,0,138,139,5,13,0,0,139,31,1,0,0,0,
  	140,145,3,26,13,0,141,142,5,14,0,0,142,144,3,26,13,0,143,141,1,0,0,0,
  	144,147,1,0,0,0,145,143,1,0,0,0,145,146,1,0,0,0,146,33,1,0,0,0,147,145,
  	1,0,0,0,148,154,5,17,0,0,149,151,5,18,0,0,150,149,1,0,0,0,151,152,1,0,
  	0,0,152,150,1,0,0,0,152,153,1,0,0,0,153,155,1,0,0,0,154,150,1,0,0,0,154,
  	155,1,0,0,0,155,35,1,0,0,0,13,39,46,54,71,76,85,97,111,126,131,145,152,
  	154
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  hlslParserStaticData = staticData.release();
}

}

HLSLParser::HLSLParser(TokenStream *input) : HLSLParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

HLSLParser::HLSLParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  HLSLParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *hlslParserStaticData->atn, hlslParserStaticData->decisionToDFA, hlslParserStaticData->sharedContextCache, options);
}

HLSLParser::~HLSLParser() {
  delete _interpreter;
}

const atn::ATN& HLSLParser::getATN() const {
  return *hlslParserStaticData->atn;
}

std::string HLSLParser::getGrammarFileName() const {
  return "HLSL.g4";
}

const std::vector<std::string>& HLSLParser::getRuleNames() const {
  return hlslParserStaticData->ruleNames;
}

const dfa::Vocabulary& HLSLParser::getVocabulary() const {
  return hlslParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView HLSLParser::getSerializedATN() const {
  return hlslParserStaticData->serializedATN;
}


//----------------- HlslFileContext ------------------------------------------------------------------

HLSLParser::HlslFileContext::HlslFileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::HlslFileContext::EOF() {
  return getToken(HLSLParser::EOF, 0);
}

std::vector<HLSLParser::ElementContext *> HLSLParser::HlslFileContext::element() {
  return getRuleContexts<HLSLParser::ElementContext>();
}

HLSLParser::ElementContext* HLSLParser::HlslFileContext::element(size_t i) {
  return getRuleContext<HLSLParser::ElementContext>(i);
}


size_t HLSLParser::HlslFileContext::getRuleIndex() const {
  return HLSLParser::RuleHlslFile;
}

void HLSLParser::HlslFileContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHlslFile(this);
}

void HLSLParser::HlslFileContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHlslFile(this);
}


std::any HLSLParser::HlslFileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitHlslFile(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::HlslFileContext* HLSLParser::hlslFile() {
  HlslFileContext *_localctx = _tracker.createInstance<HlslFileContext>(_ctx, getState());
  enterRule(_localctx, 0, HLSLParser::RuleHlslFile);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 135106) != 0)) {
      setState(36);
      element();
      setState(41);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(42);
    match(HLSLParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementContext ------------------------------------------------------------------

HLSLParser::ElementContext::ElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::StructDeclarationContext* HLSLParser::ElementContext::structDeclaration() {
  return getRuleContext<HLSLParser::StructDeclarationContext>(0);
}

HLSLParser::FunctionDeclarationContext* HLSLParser::ElementContext::functionDeclaration() {
  return getRuleContext<HLSLParser::FunctionDeclarationContext>(0);
}


size_t HLSLParser::ElementContext::getRuleIndex() const {
  return HLSLParser::RuleElement;
}

void HLSLParser::ElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElement(this);
}

void HLSLParser::ElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElement(this);
}


std::any HLSLParser::ElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitElement(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ElementContext* HLSLParser::element() {
  ElementContext *_localctx = _tracker.createInstance<ElementContext>(_ctx, getState());
  enterRule(_localctx, 2, HLSLParser::RuleElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(46);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HLSLParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(44);
        structDeclaration();
        break;
      }

      case HLSLParser::T__5:
      case HLSLParser::T__6:
      case HLSLParser::T__7:
      case HLSLParser::T__8:
      case HLSLParser::T__9:
      case HLSLParser::T__10:
      case HLSLParser::ID: {
        enterOuterAlt(_localctx, 2);
        setState(45);
        functionDeclaration();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclarationContext ------------------------------------------------------------------

HLSLParser::StructDeclarationContext::StructDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::StructDeclarationContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

std::vector<HLSLParser::StructMemberContext *> HLSLParser::StructDeclarationContext::structMember() {
  return getRuleContexts<HLSLParser::StructMemberContext>();
}

HLSLParser::StructMemberContext* HLSLParser::StructDeclarationContext::structMember(size_t i) {
  return getRuleContext<HLSLParser::StructMemberContext>(i);
}


size_t HLSLParser::StructDeclarationContext::getRuleIndex() const {
  return HLSLParser::RuleStructDeclaration;
}

void HLSLParser::StructDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDeclaration(this);
}

void HLSLParser::StructDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDeclaration(this);
}


std::any HLSLParser::StructDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitStructDeclaration(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::StructDeclarationContext* HLSLParser::structDeclaration() {
  StructDeclarationContext *_localctx = _tracker.createInstance<StructDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 4, HLSLParser::RuleStructDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(48);
    match(HLSLParser::T__0);
    setState(49);
    match(HLSLParser::ID);
    setState(50);
    match(HLSLParser::T__1);
    setState(52); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(51);
      structMember();
      setState(54); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 135104) != 0));
    setState(56);
    match(HLSLParser::T__2);
    setState(57);
    match(HLSLParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructMemberContext ------------------------------------------------------------------

HLSLParser::StructMemberContext::StructMemberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::TypeContext* HLSLParser::StructMemberContext::type() {
  return getRuleContext<HLSLParser::TypeContext>(0);
}

tree::TerminalNode* HLSLParser::StructMemberContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

HLSLParser::SemanticContext* HLSLParser::StructMemberContext::semantic() {
  return getRuleContext<HLSLParser::SemanticContext>(0);
}


size_t HLSLParser::StructMemberContext::getRuleIndex() const {
  return HLSLParser::RuleStructMember;
}

void HLSLParser::StructMemberContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructMember(this);
}

void HLSLParser::StructMemberContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructMember(this);
}


std::any HLSLParser::StructMemberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitStructMember(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::StructMemberContext* HLSLParser::structMember() {
  StructMemberContext *_localctx = _tracker.createInstance<StructMemberContext>(_ctx, getState());
  enterRule(_localctx, 6, HLSLParser::RuleStructMember);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(59);
    type();
    setState(60);
    match(HLSLParser::ID);
    setState(61);
    match(HLSLParser::T__4);
    setState(62);
    semantic();
    setState(63);
    match(HLSLParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

HLSLParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::TypeContext::ID() {
  return getToken(HLSLParser::ID, 0);
}


size_t HLSLParser::TypeContext::getRuleIndex() const {
  return HLSLParser::RuleType;
}

void HLSLParser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void HLSLParser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}


std::any HLSLParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::TypeContext* HLSLParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 8, HLSLParser::RuleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(65);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 135104) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

HLSLParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::TypeContext* HLSLParser::FunctionDeclarationContext::type() {
  return getRuleContext<HLSLParser::TypeContext>(0);
}

tree::TerminalNode* HLSLParser::FunctionDeclarationContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

HLSLParser::BlockContext* HLSLParser::FunctionDeclarationContext::block() {
  return getRuleContext<HLSLParser::BlockContext>(0);
}

HLSLParser::ParameterListContext* HLSLParser::FunctionDeclarationContext::parameterList() {
  return getRuleContext<HLSLParser::ParameterListContext>(0);
}

HLSLParser::SemanticContext* HLSLParser::FunctionDeclarationContext::semantic() {
  return getRuleContext<HLSLParser::SemanticContext>(0);
}


size_t HLSLParser::FunctionDeclarationContext::getRuleIndex() const {
  return HLSLParser::RuleFunctionDeclaration;
}

void HLSLParser::FunctionDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclaration(this);
}

void HLSLParser::FunctionDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclaration(this);
}


std::any HLSLParser::FunctionDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitFunctionDeclaration(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::FunctionDeclarationContext* HLSLParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 10, HLSLParser::RuleFunctionDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(67);
    type();
    setState(68);
    match(HLSLParser::ID);
    setState(69);
    match(HLSLParser::T__11);
    setState(71);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 135104) != 0)) {
      setState(70);
      parameterList();
    }
    setState(73);
    match(HLSLParser::T__12);
    setState(76);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HLSLParser::T__4) {
      setState(74);
      match(HLSLParser::T__4);
      setState(75);
      semantic();
    }
    setState(78);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterListContext ------------------------------------------------------------------

HLSLParser::ParameterListContext::ParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<HLSLParser::ParameterContext *> HLSLParser::ParameterListContext::parameter() {
  return getRuleContexts<HLSLParser::ParameterContext>();
}

HLSLParser::ParameterContext* HLSLParser::ParameterListContext::parameter(size_t i) {
  return getRuleContext<HLSLParser::ParameterContext>(i);
}


size_t HLSLParser::ParameterListContext::getRuleIndex() const {
  return HLSLParser::RuleParameterList;
}

void HLSLParser::ParameterListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameterList(this);
}

void HLSLParser::ParameterListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameterList(this);
}


std::any HLSLParser::ParameterListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitParameterList(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ParameterListContext* HLSLParser::parameterList() {
  ParameterListContext *_localctx = _tracker.createInstance<ParameterListContext>(_ctx, getState());
  enterRule(_localctx, 12, HLSLParser::RuleParameterList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    parameter();
    setState(85);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HLSLParser::T__13) {
      setState(81);
      match(HLSLParser::T__13);
      setState(82);
      parameter();
      setState(87);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterContext ------------------------------------------------------------------

HLSLParser::ParameterContext::ParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::TypeContext* HLSLParser::ParameterContext::type() {
  return getRuleContext<HLSLParser::TypeContext>(0);
}

tree::TerminalNode* HLSLParser::ParameterContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

HLSLParser::SemanticContext* HLSLParser::ParameterContext::semantic() {
  return getRuleContext<HLSLParser::SemanticContext>(0);
}


size_t HLSLParser::ParameterContext::getRuleIndex() const {
  return HLSLParser::RuleParameter;
}

void HLSLParser::ParameterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameter(this);
}

void HLSLParser::ParameterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameter(this);
}


std::any HLSLParser::ParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitParameter(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ParameterContext* HLSLParser::parameter() {
  ParameterContext *_localctx = _tracker.createInstance<ParameterContext>(_ctx, getState());
  enterRule(_localctx, 14, HLSLParser::RuleParameter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    type();
    setState(89);
    match(HLSLParser::ID);
    setState(90);
    match(HLSLParser::T__4);
    setState(91);
    semantic();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

HLSLParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<HLSLParser::StatementContext *> HLSLParser::BlockContext::statement() {
  return getRuleContexts<HLSLParser::StatementContext>();
}

HLSLParser::StatementContext* HLSLParser::BlockContext::statement(size_t i) {
  return getRuleContext<HLSLParser::StatementContext>(i);
}


size_t HLSLParser::BlockContext::getRuleIndex() const {
  return HLSLParser::RuleBlock;
}

void HLSLParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void HLSLParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any HLSLParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::BlockContext* HLSLParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 16, HLSLParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    match(HLSLParser::T__1);
    setState(97);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 200640) != 0)) {
      setState(94);
      statement();
      setState(99);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(100);
    match(HLSLParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

HLSLParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::DeclarationContext* HLSLParser::StatementContext::declaration() {
  return getRuleContext<HLSLParser::DeclarationContext>(0);
}

HLSLParser::AssignmentContext* HLSLParser::StatementContext::assignment() {
  return getRuleContext<HLSLParser::AssignmentContext>(0);
}

HLSLParser::ReturnStatementContext* HLSLParser::StatementContext::returnStatement() {
  return getRuleContext<HLSLParser::ReturnStatementContext>(0);
}


size_t HLSLParser::StatementContext::getRuleIndex() const {
  return HLSLParser::RuleStatement;
}

void HLSLParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void HLSLParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any HLSLParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::StatementContext* HLSLParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 18, HLSLParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(111);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(102);
      declaration();
      setState(103);
      match(HLSLParser::T__3);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(105);
      assignment();
      setState(106);
      match(HLSLParser::T__3);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(108);
      returnStatement();
      setState(109);
      match(HLSLParser::T__3);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

HLSLParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::TypeContext* HLSLParser::DeclarationContext::type() {
  return getRuleContext<HLSLParser::TypeContext>(0);
}

tree::TerminalNode* HLSLParser::DeclarationContext::ID() {
  return getToken(HLSLParser::ID, 0);
}


size_t HLSLParser::DeclarationContext::getRuleIndex() const {
  return HLSLParser::RuleDeclaration;
}

void HLSLParser::DeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclaration(this);
}

void HLSLParser::DeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclaration(this);
}


std::any HLSLParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::DeclarationContext* HLSLParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 20, HLSLParser::RuleDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113);
    type();
    setState(114);
    match(HLSLParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

HLSLParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::AssignmentContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

HLSLParser::ExpressionContext* HLSLParser::AssignmentContext::expression() {
  return getRuleContext<HLSLParser::ExpressionContext>(0);
}


size_t HLSLParser::AssignmentContext::getRuleIndex() const {
  return HLSLParser::RuleAssignment;
}

void HLSLParser::AssignmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignment(this);
}

void HLSLParser::AssignmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignment(this);
}


std::any HLSLParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::AssignmentContext* HLSLParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 22, HLSLParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(116);
    match(HLSLParser::ID);
    setState(117);
    match(HLSLParser::T__14);
    setState(118);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

HLSLParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::ExpressionContext* HLSLParser::ReturnStatementContext::expression() {
  return getRuleContext<HLSLParser::ExpressionContext>(0);
}


size_t HLSLParser::ReturnStatementContext::getRuleIndex() const {
  return HLSLParser::RuleReturnStatement;
}

void HLSLParser::ReturnStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStatement(this);
}

void HLSLParser::ReturnStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStatement(this);
}


std::any HLSLParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ReturnStatementContext* HLSLParser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 24, HLSLParser::RuleReturnStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(120);
    match(HLSLParser::T__15);
    setState(121);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

HLSLParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::ExpressionContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

HLSLParser::FunctionCallContext* HLSLParser::ExpressionContext::functionCall() {
  return getRuleContext<HLSLParser::FunctionCallContext>(0);
}

HLSLParser::ConstructorExpressionContext* HLSLParser::ExpressionContext::constructorExpression() {
  return getRuleContext<HLSLParser::ConstructorExpressionContext>(0);
}


size_t HLSLParser::ExpressionContext::getRuleIndex() const {
  return HLSLParser::RuleExpression;
}

void HLSLParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void HLSLParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}


std::any HLSLParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ExpressionContext* HLSLParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 26, HLSLParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(126);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(123);
      match(HLSLParser::ID);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(124);
      functionCall();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(125);
      constructorExpression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionCallContext ------------------------------------------------------------------

HLSLParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::FunctionCallContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

HLSLParser::ArgumentListContext* HLSLParser::FunctionCallContext::argumentList() {
  return getRuleContext<HLSLParser::ArgumentListContext>(0);
}


size_t HLSLParser::FunctionCallContext::getRuleIndex() const {
  return HLSLParser::RuleFunctionCall;
}

void HLSLParser::FunctionCallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionCall(this);
}

void HLSLParser::FunctionCallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionCall(this);
}


std::any HLSLParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::FunctionCallContext* HLSLParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 28, HLSLParser::RuleFunctionCall);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(128);
    match(HLSLParser::ID);
    setState(129);
    match(HLSLParser::T__11);
    setState(131);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 135104) != 0)) {
      setState(130);
      argumentList();
    }
    setState(133);
    match(HLSLParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstructorExpressionContext ------------------------------------------------------------------

HLSLParser::ConstructorExpressionContext::ConstructorExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HLSLParser::TypeContext* HLSLParser::ConstructorExpressionContext::type() {
  return getRuleContext<HLSLParser::TypeContext>(0);
}

HLSLParser::ArgumentListContext* HLSLParser::ConstructorExpressionContext::argumentList() {
  return getRuleContext<HLSLParser::ArgumentListContext>(0);
}


size_t HLSLParser::ConstructorExpressionContext::getRuleIndex() const {
  return HLSLParser::RuleConstructorExpression;
}

void HLSLParser::ConstructorExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstructorExpression(this);
}

void HLSLParser::ConstructorExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstructorExpression(this);
}


std::any HLSLParser::ConstructorExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitConstructorExpression(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ConstructorExpressionContext* HLSLParser::constructorExpression() {
  ConstructorExpressionContext *_localctx = _tracker.createInstance<ConstructorExpressionContext>(_ctx, getState());
  enterRule(_localctx, 30, HLSLParser::RuleConstructorExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(135);
    type();
    setState(136);
    match(HLSLParser::T__11);
    setState(137);
    argumentList();
    setState(138);
    match(HLSLParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentListContext ------------------------------------------------------------------

HLSLParser::ArgumentListContext::ArgumentListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<HLSLParser::ExpressionContext *> HLSLParser::ArgumentListContext::expression() {
  return getRuleContexts<HLSLParser::ExpressionContext>();
}

HLSLParser::ExpressionContext* HLSLParser::ArgumentListContext::expression(size_t i) {
  return getRuleContext<HLSLParser::ExpressionContext>(i);
}


size_t HLSLParser::ArgumentListContext::getRuleIndex() const {
  return HLSLParser::RuleArgumentList;
}

void HLSLParser::ArgumentListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgumentList(this);
}

void HLSLParser::ArgumentListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgumentList(this);
}


std::any HLSLParser::ArgumentListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitArgumentList(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::ArgumentListContext* HLSLParser::argumentList() {
  ArgumentListContext *_localctx = _tracker.createInstance<ArgumentListContext>(_ctx, getState());
  enterRule(_localctx, 32, HLSLParser::RuleArgumentList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(140);
    expression();
    setState(145);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HLSLParser::T__13) {
      setState(141);
      match(HLSLParser::T__13);
      setState(142);
      expression();
      setState(147);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SemanticContext ------------------------------------------------------------------

HLSLParser::SemanticContext::SemanticContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HLSLParser::SemanticContext::ID() {
  return getToken(HLSLParser::ID, 0);
}

std::vector<tree::TerminalNode *> HLSLParser::SemanticContext::DIGIT() {
  return getTokens(HLSLParser::DIGIT);
}

tree::TerminalNode* HLSLParser::SemanticContext::DIGIT(size_t i) {
  return getToken(HLSLParser::DIGIT, i);
}


size_t HLSLParser::SemanticContext::getRuleIndex() const {
  return HLSLParser::RuleSemantic;
}

void HLSLParser::SemanticContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSemantic(this);
}

void HLSLParser::SemanticContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<HLSLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSemantic(this);
}


std::any HLSLParser::SemanticContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HLSLVisitor*>(visitor))
    return parserVisitor->visitSemantic(this);
  else
    return visitor->visitChildren(this);
}

HLSLParser::SemanticContext* HLSLParser::semantic() {
  SemanticContext *_localctx = _tracker.createInstance<SemanticContext>(_ctx, getState());
  enterRule(_localctx, 34, HLSLParser::RuleSemantic);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(148);
    match(HLSLParser::ID);
    setState(154);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HLSLParser::DIGIT) {
      setState(150); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(149);
        match(HLSLParser::DIGIT);
        setState(152); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == HLSLParser::DIGIT);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void HLSLParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  hlslParserInitialize();
#else
  ::antlr4::internal::call_once(hlslParserOnceFlag, hlslParserInitialize);
#endif
}
