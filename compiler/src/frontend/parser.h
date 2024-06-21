#pragma once
#include "core/tokens.h"
#include "core/logger.h"
#include "defines.h"
#include "core/ast.h"
#include "lexer.h"

#include <cstdlib>
#include <variant>
#include <vector>
namespace compiler {

class Parser {
public:
    Parser(const std::string& source_code)
        : m_source_code(source_code),
          m_lexer(new Lexer(m_source_code)),
          m_peek_token(m_lexer->next_token()),
          m_current_token(Token())
        {
            this->advance();
        }

    ~Parser() {
        delete m_lexer;
    }
   
    /// Return the next node from the input source code
    core::AstNode* next_node();
private:
    Integer INTEGER_TOKEN = 1;
    Float FLOAT_TOKEN = 1.0F;
    String STRING_TOKEN = "STRING";

    void advance();

    template <typename T>
    void expect(T expected) {
        if (m_current_token.is<ReservedToken>()) {
            if (m_current_token.get<T>() != expected) {
                core::logger::Fatal("Parser::expect. Illegal token: {}.", m_current_token.to_str());
                exit(1);
            }
        } else {
            if (!m_current_token.is<T>()) {
                core::logger::Fatal("Parser::expect. Illegal token: {}.", m_current_token.to_str());
            }
        }

        this->advance();
    }


    core::AstNode* let_stmt();
    core::AstNode* identifier();
    core::AstNode* type_annotation();

    core::AstNode* expr();
    core::AstNode* primary_expr();
    core::AstNode* prefix_expr();
    core::AstNode* true_expr();
    core::AstNode* false_expr();
    core::AstNode* integer_expr();
    core::AstNode* float_expr();

    const std::string& m_source_code;
    Lexer* m_lexer; // owned
    Token m_peek_token;
    Token m_current_token;
};

}
