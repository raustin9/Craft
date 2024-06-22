#include "parser.h"
#include "core/ast.h"
#include "core/logger.h"
#include "core/tokens.h"
#include "core/type.h"

namespace compiler {

// Advance the token that the parser is currently looking at
void Parser::advance() {
    m_current_token = m_peek_token;
    m_peek_token = m_lexer->next_token();
    core::logger::Debug("Current {}. Peek {}", m_current_token.to_str(), m_peek_token.to_str());
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

/* Parse type annotations */
core::Type* Parser::type() {
    Token type_token = m_current_token;

    if (type_token.is<ReservedToken>()) {
        ReservedToken t  = type_token.get<ReservedToken>();
        switch (t) {
            case ReservedToken::KwU8:
            case ReservedToken::KwU16:
            case ReservedToken::KwU32:
            case ReservedToken::KwU64:
            case ReservedToken::KwI8:
            case ReservedToken::KwI16:
            case ReservedToken::KwI32:
            case ReservedToken::KwI64: {
                advance();
                return new core::TypeInteger(t);
            }
            
            case ReservedToken::KwF32:
            case ReservedToken::KwF64: {
                advance();
                return new core::TypeFloat(t);
            }

            case ReservedToken::OpMul: {
                expect(ReservedToken::OpMul);
                core::Type* target = this->type();
                return new core::TypePointer(target);
            }
            
            default: {
                core::logger::Fatal("Illegal token when parsing type: {}.", reserved_to_str(type_token.get<ReservedToken>()));
                return nullptr;
            }
        }
        
    } else if (type_token.is<Identifier>()) {
        return new core::TypeIdentifier();
    } else {
        return nullptr;
    }
}

// let i: i32 = 100;
core::AstNode* Parser::let_stmt() {
    expect(ReservedToken::KwLet);

    core::AstNode* target = identifier();

    expect(ReservedToken::OpColon);

    core::Type* type = this->type();

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
