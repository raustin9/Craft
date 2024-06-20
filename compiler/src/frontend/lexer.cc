#include "core/tokens.h"
#include "defines.h"
#include "lexer.h"
#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <array>
#include <variant>

namespace compiler {

/// Sorting method that helps us match character streams from the source file to the corresponding token
template <typename T, std::size_t N>
[[ nodiscard ]]
constexpr std::array<T, N> sort(std::array<T,N> arr) noexcept {
    std::sort(std::begin(arr), std::end(arr));
    return arr;
}

template <typename T, std::size_t N>
std::optional<ReservedToken> find(std::array<T, N> arr, const std::string& value) {
    for (usize i = 0; i < N; i++) {
        if (arr[i].token_str == value) {
            // printf("FOUND: [%s]\n", arr[i].token_str.data());
            return arr[i].token;
        }
    }

    return {};
}


/// Our keyword token mappings
constexpr std::array keywords {
    sort (
        std::array {
            StringifiedToken{ "as", ReservedToken::KwAs },
            StringifiedToken{ "assert", ReservedToken::KwAssert },
            StringifiedToken{ "break", ReservedToken::KwBreak },
            StringifiedToken{ "struct", ReservedToken::KwStruct },
            StringifiedToken{ "defer", ReservedToken::KwDefer },
            StringifiedToken{ "match", ReservedToken::KwMatch },
            StringifiedToken{ "continue", ReservedToken::KwContinue },
            StringifiedToken{ "contract", ReservedToken::KwContract },
            StringifiedToken{ "define", ReservedToken::KwDefine },
            StringifiedToken{ "del", ReservedToken::KwDel },
            StringifiedToken{ "elif", ReservedToken::KwElif },
            StringifiedToken{ "else", ReservedToken::KwElse },
            StringifiedToken{ "False", ReservedToken::KwFalse },
            StringifiedToken{ "for", ReservedToken::KwFor },
            StringifiedToken{ "from", ReservedToken::KwFrom },
            StringifiedToken{ "global", ReservedToken::KwGlobal },
            StringifiedToken{ "if", ReservedToken::KwIf },
            StringifiedToken{ "Import", ReservedToken::KwImport },
            StringifiedToken{ "in", ReservedToken::KwIn },
            StringifiedToken{ "is", ReservedToken::KwIs },
            StringifiedToken{ "lambda", ReservedToken::KwLambda },
            StringifiedToken{ "let", ReservedToken::KwLet },
            StringifiedToken{ "I8", ReservedToken::KwI8},
            StringifiedToken{ "I16", ReservedToken::KwI16},
            StringifiedToken{ "I32", ReservedToken::KwI32},
            StringifiedToken{ "I64", ReservedToken::KwI64},
            StringifiedToken{ "U8", ReservedToken::KwU8},
            StringifiedToken{ "U16", ReservedToken::KwU16},
            StringifiedToken{ "U32", ReservedToken::KwU32},
            StringifiedToken{ "U64", ReservedToken::KwU64},
            StringifiedToken{ "F32", ReservedToken::KwF32},
            StringifiedToken{ "F64", ReservedToken::KwF64},
            StringifiedToken{ "pass", ReservedToken::KwPass },
            StringifiedToken{ "return", ReservedToken::KwReturn },
            StringifiedToken{ "True", ReservedToken::KwTrue },
            StringifiedToken{ "while", ReservedToken::KwWhile },
            StringifiedToken{ "with", ReservedToken::KwWith },
            StringifiedToken{ "yield", ReservedToken::KwYield },
        }
    )
};

/// Our operator token mappings
constexpr std::array operators {
    sort (
        std::array {
            StringifiedToken { "(", ReservedToken::OpParenOpen} ,
            StringifiedToken { ")", ReservedToken::OpParenClose} ,
            StringifiedToken { "[", ReservedToken::OpSubscriptOpen} ,
            StringifiedToken { "]", ReservedToken::OpSubscriptClose} ,
            StringifiedToken { "{", ReservedToken::OpCurlyBracketOpen} ,
            StringifiedToken { "}", ReservedToken::OpCurlyBracketClose} ,
            StringifiedToken { "->", ReservedToken::OpArrow} ,
            StringifiedToken { ":", ReservedToken::OpColon} ,
            StringifiedToken { "::", ReservedToken::OpDoubleColon} ,
            StringifiedToken { ";", ReservedToken::OpSemicolon} ,
            StringifiedToken { ",", ReservedToken::OpComma} ,
            StringifiedToken { ".", ReservedToken::OpDot} ,
            StringifiedToken { "+", ReservedToken::OpAdd} ,
            StringifiedToken { "/", ReservedToken::OpDiv} ,
            StringifiedToken { "%", ReservedToken::OpMod} ,
            StringifiedToken { "*", ReservedToken::OpMul} ,
            StringifiedToken { "-", ReservedToken::OpSub} ,
            StringifiedToken { "&", ReservedToken::OpBinaryAnd} ,
            StringifiedToken { "|", ReservedToken::OpBinaryOr} ,
            StringifiedToken { "^", ReservedToken::OpBinaryXor} ,
            StringifiedToken { "~", ReservedToken::OpBinaryNot} ,
            StringifiedToken { "<<", ReservedToken::OpBinaryLeftShift} ,
            StringifiedToken { ">>", ReservedToken::OpBinaryRightShift} ,
            StringifiedToken { "&&", ReservedToken::OpLogicalAnd} ,
            StringifiedToken { "||", ReservedToken::OpLogicalOr} ,
            StringifiedToken { "!", ReservedToken::OpLogicalNot} ,
            StringifiedToken { "=", ReservedToken::OpAssign} ,
            StringifiedToken { "+=", ReservedToken::OpAssignAdd} ,
            StringifiedToken { "-=", ReservedToken::OpAssignSub} ,
            StringifiedToken { "/=", ReservedToken::OpAssignDiv} ,
            StringifiedToken { "*=", ReservedToken::OpAssignMul} ,
            StringifiedToken { "%=", ReservedToken::OpAssignMod} ,
            StringifiedToken { "&=", ReservedToken::OpAssignBinaryAnd} ,
            StringifiedToken { "|=", ReservedToken::OpAssignBinaryOr} ,
            StringifiedToken { "^=", ReservedToken::OpAssignBinaryXor} ,
            StringifiedToken { "~=", ReservedToken::OpAssignBinaryNot} ,
            StringifiedToken { "<<=", ReservedToken::OpAssignBinaryLeftShift} ,
            StringifiedToken { ">>=", ReservedToken::OpAssignBinaryRightShift} ,
            StringifiedToken { "==", ReservedToken::OpEqualTo} ,
            StringifiedToken { ">", ReservedToken::OpGreaterThan} ,
            StringifiedToken { "<", ReservedToken::OpLessThan} ,
            StringifiedToken { "<=", ReservedToken::OpGreaterThanEqualTo} ,
            StringifiedToken { ">=", ReservedToken::OpLessThanEqualTo} ,
            StringifiedToken { "!=", ReservedToken::OpNotEqual} ,
        }
    )
};

char
Lexer::peek_char() {
    if (m_position+1 >= m_input.length()) {
        return '\0';
    } else {
        // printf("\x1b[35mEating: [%c]\x1b[0m\n", m_current_char);
        return m_input[m_position+1];
    }
}

void
Lexer::skip_whitespace() {
    while (m_current_char == ' ' ||
            m_current_char == '\t' ||
            m_current_char == '\r' ||
            m_current_char == '\n'
    ) {
        read_char();
    }
}

void
Lexer::read_char() {
    if (m_position >= m_input.length()) {
        m_current_char = '\0';
    } else {
        // printf("\x1b[35mEating: [%c]\x1b[0m\n", m_current_char);
        m_current_char = m_input[m_position];
    }

    m_position++;
}

Token 
Lexer::read_number() {
    usize pos = m_position - 1;
    bool floating_point = false;
    bool is_valid = true;
    // printf("read_number: starting position [%lu]\n", m_position);

    while (isdigit(m_current_char) != 0 || m_current_char == '.') {
        if (m_current_char == '.') {
            if (floating_point == true) {
                is_valid = false;
            }
            floating_point = true;
        }

        read_char();
    }
    // printf("read_number: ending position [%lu]\n", m_position);

    if (!is_valid) {
        // Not a valid number
        // printf("INVALID TOKEN\n");
        return Token(ReservedToken::Unknown);
    } else if (floating_point) {
        // Floating point number
        f64 value;
        std::from_chars(m_input.data() + pos, m_input.data() + (m_position - pos), value);
        // printf("\x1b[33mDecimal: %lf\n\x1b[0m", value);
        return Token(
            value
        );
    } else {
        // Integer number
        u64 value;
        std::from_chars(m_input.data() + pos, m_input.data() + (m_position - pos), value);
        // printf("\x1b[31mInteger: %lu\n\x1b[0m", value);
        return Token(
            value
        );
    }
}

Token
Lexer::read_identifier() {
    usize pos = m_position -1;
    while (
        isalpha(m_current_char) != 0
        || isdigit(m_current_char) != 0 
        || m_current_char == '_'
    ) {
        read_char();
    }
    
    char* buf = (char*)calloc(m_position-pos, sizeof(char));
    std::strncpy(buf, m_input.data() + pos, m_position-pos-1);
    std::string str = buf;

    // printf("STR: [%s]\n", str.c_str());

    auto value = find(keywords, str);
    if (value.has_value()) {
        // printf("\x1b[38mKeyword: [%s]\n\x1b[0m", str.c_str());
        return Token(
            value.value()
        );
    } else {
        // printf("\x1b[38mIdentifier: [%s]\n\x1b[0m", str.c_str());
        return Token(
            Identifier(str)
        );
    }
}

Token
Lexer::read_punctuator() {
    Token token;

    switch (m_current_char) {
        case '(': {
            token = Token(ReservedToken::OpParenOpen);
        } break;
        case ')': {
            token = Token(ReservedToken::OpParenClose);
        } break;
        case '[': {
            token = Token(ReservedToken::OpSubscriptOpen);
        } break;
        case ']': {
            token = Token(ReservedToken::OpSubscriptClose);
        } break;
        case '{': {
            token = Token(ReservedToken::OpCurlyBracketOpen);
        } break;
        case '}': {
            token = Token(ReservedToken::OpCurlyBracketClose);
        } break;
        case '-': {
            if (peek_char() == '>') {
                // ->
                read_char();
                token = Token(ReservedToken::OpArrow);
            } else if (peek_char() == '=') {
                // -=
                read_char();
                token = Token(ReservedToken::OpAssignSub);
            }
            token = Token(ReservedToken::OpSub);
        } break;
        case ':': {
            if (peek_char() == ':') {
                read_char();
                token = Token(ReservedToken::OpDoubleColon);
            }
            token = Token(ReservedToken::OpColon);
        } break;
        case ';': {
            token = Token(ReservedToken::OpSemicolon);
        } break;
        case ',': {
            token = Token(ReservedToken::OpComma);
        } break;
        case '.': {
            token = Token(ReservedToken::OpDot);
        } break;
        case '+': {
            if (peek_char() == '=') {
                // +=
                read_char();
                token = Token(ReservedToken::OpAssignAdd);
            }
            token = Token(ReservedToken::OpAdd);
        } break;
        case '/': {
            if (peek_char() == '=') {
                // /=
                read_char();
                token = Token(ReservedToken::OpAssignDiv);
            } 
            token = Token(ReservedToken::OpDiv);
        } break;
        case '%': {
            if (peek_char() == '=') {
                // %=
                read_char();
                token = Token(ReservedToken::OpAssignMod);
            }
            token = Token(ReservedToken::OpMod);
        } break;
        case '*': {
            if (peek_char() == '=') {
                // *=
                read_char();
                token = Token(ReservedToken::OpAssignMul);
            }
            token = Token(ReservedToken::OpMul);
        } break;
        case '&': {
            if (peek_char() == '=') {
                // &&
                read_char();
                token = Token(ReservedToken::OpAssignBinaryAnd);
            } else if (peek_char() == '&') {
                read_char();
                token = Token(ReservedToken::OpLogicalAnd);
            }
            token = Token(ReservedToken::OpBinaryAnd);
        } break;
        case '|': {
            if (peek_char() == '=') {
                // ||
                read_char();
                token = Token(ReservedToken::OpAssignBinaryOr);
            } else if (peek_char() == '|') {
                read_char();
                token = Token(ReservedToken::OpLogicalOr);
            }
            token = Token(ReservedToken::OpBinaryOr);
        } break;
        case '^': {
            if (peek_char() == '=') {
                read_char();
                token = Token(ReservedToken::OpAssignBinaryXor);
            }
            token = Token(ReservedToken::OpBinaryXor);
        } break;
        case '=': {
            if (peek_char() == '=') {
                read_char();
                token = Token(ReservedToken::OpEqualTo);
            }
            token = Token(ReservedToken::OpAssign);
        } break;
        case '<': {
            if (peek_char() == '=') {
                // <=
                read_char();
                token = Token(ReservedToken::OpLessThanEqualTo);
            } else if (peek_char() == '<') {
                read_char();
                if (peek_char() == '=') {
                    // <<=
                    read_char();
                    token = Token(ReservedToken::OpAssignBinaryLeftShift);
                }
                token = Token(ReservedToken::OpBinaryLeftShift);
            }
            token = Token(ReservedToken::OpLessThan);
        } break;
        case '>': {
            if (peek_char() == '=') {
                // >=
                read_char();
                token = Token(ReservedToken::OpGreaterThanEqualTo);
            } else if (peek_char() == '<') {
                read_char();
                if (peek_char() == '=') {
                    // >>=
                    read_char();
                    token = Token(ReservedToken::OpAssignBinaryRightShift);
                }
                token = Token(ReservedToken::OpBinaryRightShift);
            }
            token = Token(ReservedToken::OpGreaterThan);
        } break;
        case '~': {
            if (peek_char() == '=') {
                // ~=
                read_char();
                token = Token(ReservedToken::OpAssignBinaryNot);
            }
            token = Token(ReservedToken::OpBinaryNot);
        } break;
        case '!': {
            if (peek_char() == '=') {
                // !=
                read_char();
                token = Token(ReservedToken::OpNotEqual);
            }
            token = Token(ReservedToken::OpLogicalNot);
        } break;

        default:
            token = Token(ReservedToken::Unknown);
    }

    read_char();
    return token;
}

Token
Lexer::read_alphanumeric() {
    Token token;

    if (isalpha(m_current_char)) {
        token = read_identifier();
    } if (isdigit(m_current_char) != 0) {
        token = read_number();
    }

    return token;
}

Token
Lexer::read_string_literal() {
    usize pos = m_position - 1;
    read_char(); // eat first '"'
    while (m_current_char != '"') {
        read_char();
    }
    read_char(); // eat second '"'

    char* buf = (char*)calloc(m_position-pos, sizeof(char));
    std::strncpy(buf, m_input.data() + pos, m_position-pos-1);
    std::string str = buf;

    return Token(str);
}

/// Get the next token found from the source code
Token Lexer::next_token() {
    Token token;

    skip_whitespace();
    if (isalnum(m_current_char)) {
        // printf("\x1b[32mALPHANUM: %c\n\x1b[0m", m_current_char);
        token = read_alphanumeric();
    } else {
        if (m_current_char == '\0') {
            return Token(Eof());
        } else if (m_current_char == '"') {
            // printf("STRING LITERAL\n");
            token = read_string_literal();
        } else {
            // printf("\x1b[36mNONALPHA: %c\n\x1b[0m", m_current_char);
            token = read_punctuator();
        }
    }

    token.print();
    return token;
}

}
