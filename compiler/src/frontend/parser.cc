#include "parser.h"
#include "core/ast.h"
#include "core/logger.h"
#include "core/tokens.h"

namespace compiler {

// Advance the token that the parser is currently looking at
void Parser::advance() {
    m_current_token = m_peek_token;
    m_peek_token = m_lexer->next_token();
}

core::AstNode* Parser::next_node() {
    // Only declarations are allowed at the top level
    // This assumes that only "ReservedToken"s are occuring here
    core::AstNode* node;

    if (m_current_token.is<Eof>()) {
        core::logger::Info("Found EOF");
        return nullptr;
    }

    switch(m_current_token.get<ReservedToken>()) {
        case ReservedToken::KwLet:
            node = let_stmt();
            expect(ReservedToken::OpSemicolon);
            break;

        default:
            node = nullptr;
            break;
    }

    return node;
}

core::AstNode* Parser::type_annotation() {
    Token type = m_current_token;
    advance();
    return new core::AstTypeAnnotation(type);
}

// let i: i32 = 100;
core::AstNode* Parser::let_stmt() {
    expect(ReservedToken::KwLet);

    core::AstNode* target = identifier();

    expect(ReservedToken::OpColon);

    core::AstNode* type = type_annotation();

    expect(ReservedToken::OpAssign);

    core::AstNode* value = expr();

    return new core::AstVarDecl(target, type, value);
}

core::AstNode* Parser::identifier() {
    Identifier ident = m_current_token.get<Identifier>();
    advance(); // eat the identifier
    return new core::AstIdentifierExpr(ident);
}

core::AstNode* Parser::expr() {
    return primary_expr();
}

// Parse a 'true' boolean literal
core::AstNode* Parser::true_expr() {
    advance();
    return new core::AstBoolExpr(true);
}

// Parse a 'false' boolean literal
core::AstNode* Parser::false_expr() {
    advance();
    return new core::AstBoolExpr(false);
}

// Parse an integer literal
core::AstNode* Parser::integer_expr() {
    Integer value = m_current_token.get<Integer>();
    advance(); // eat the number

    return new core::AstIntegerExpr(value);
}

// Parse a floating point number literal
core::AstNode* Parser::float_expr() {
    Float value = m_current_token.get<Float>();
    advance(); // eat the number

    return new core::AstFloatExpr(value);
}

core::AstNode* Parser::prefix_expr() {
    return nullptr;
}

// Parse a primary expression
core::AstNode* Parser::primary_expr() {
    if (m_current_token.is<ReservedToken>()) {
        // Operators
        switch (m_current_token.get<ReservedToken>()) {
            case ReservedToken::OpLogicalNot:
            case ReservedToken::OpSub:
            case ReservedToken::OpBinaryNot:
                return prefix_expr();

            case ReservedToken::KwTrue:
                return true_expr();
            case ReservedToken::KwFalse:
                return false_expr();

            default:
                return nullptr;
                break;
        }
    } else if (m_current_token.is<Integer>()) {
        return integer_expr();
    } else if (m_current_token.is<Float>()) {
        return float_expr();
    } else if (m_current_token.is<Identifier>()) {
        // TODO: parse identifier
    }

    return nullptr;
}

}
