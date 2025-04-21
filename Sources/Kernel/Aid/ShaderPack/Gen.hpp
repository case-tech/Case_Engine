#include "HlslBaseVisitor.h"
#include <string>

class HlslToPseudocodeVisitor : public HlslBaseVisitor {
public:
    std::string output;
    
    antlrcpp::Any visitVariableDecl(HlslParser::VariableDeclContext *ctx) override {
        std::string type = ctx->type()->getText();
        type = type.substr(0, type.find_first_of("1234"));  // Удаляем размерность
        
        output += type + " " + ctx->ID()->getText() + " = ";
        visit(ctx->expression());
        output += ";\n";
        return nullptr;
    }

    antlrcpp::Any visitConstructorCall(HlslParser::ConstructorCallContext *ctx) override {
        std::string type = ctx->type()->getText();
        type = type.substr(0, type.find_first_of("1234"));
        
        output += type + "(";
        if (ctx->argumentList()) visit(ctx->argumentList());
        output += ")";
        return nullptr;
    }
    
    antlrcpp::Any visitArgumentList(HlslParser::ArgumentListContext *ctx) override {
        for (size_t i = 0; i < ctx->expression().size(); ++i) {
            visit(ctx->expression(i));
            if (i < ctx->expression().size() - 1) {
                output += ", ";
            }
        }
        return nullptr;
    }

    // Добавляем обработку идентификаторов
    antlrcpp::Any visitTerminal(antlr4::tree::TerminalNode *node) override {
        output += node->getText();
        return nullptr;
    }

    // Добавляем обработку литералов
    antlrcpp::Any visitLiteral(HlslParser::LiteralContext *ctx) override {
        output += ctx->getText();
        return nullptr;
    }
};
