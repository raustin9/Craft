#pragma once
#include "defines.h"
#include "core/tokens.h"
#include <string_view>

namespace compiler {

class Lexer {
public:
    Lexer() noexcept = default;
    Lexer(std::string_view input) 
        : m_input(input), 
        m_current_char(m_input[0]),
        m_position(0)
    {
        read_char();
    }
    
    Token next_token();
private:
    std::string_view m_input;
    char m_current_char;
    u64 m_position;

    void skip_whitespace();
    void read_char();
    Token read_alphanumeric();
    Token read_punctuator();
    Token read_number();
    Token read_identifier();
};

}
