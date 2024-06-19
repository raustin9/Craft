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
    printf("Find: N is %lu\n", N);
    for (usize i = 0; i < N; i++) {
        if (arr[i].token_str == value) {
            return arr[i].token;
        }
        printf("Find: %s != %s\n", arr[i].token_str.data(), value.c_str());
    }

    return {};
}


/// Our keyword token mappings
constexpr std::array keywords {
    sort (
        std::array {
            StringifiedToken{ "and", ReservedToken::KwAnd },
            StringifiedToken{ "as", ReservedToken::KwAs },
            StringifiedToken{ "assert", ReservedToken::KwAssert },
            StringifiedToken{ "break", ReservedToken::KwBreak },
            StringifiedToken{ "class", ReservedToken::KwClass },
            StringifiedToken{ "continue", ReservedToken::KwContinue },
            StringifiedToken{ "contract", ReservedToken::KwContract },
            StringifiedToken{ "def", ReservedToken::KwDef },
            StringifiedToken{ "del", ReservedToken::KwDel },
            StringifiedToken{ "elif", ReservedToken::KwElif },
            StringifiedToken{ "else", ReservedToken::KwElse },
            StringifiedToken{ "except", ReservedToken::KwExcept },
            StringifiedToken{ "False", ReservedToken::KwFalse },
            StringifiedToken{ "finally", ReservedToken::KwFinally },
            StringifiedToken{ "for", ReservedToken::KwFor },
            StringifiedToken{ "from", ReservedToken::KwFrom },
            StringifiedToken{ "global", ReservedToken::KwGlobal },
            StringifiedToken{ "if", ReservedToken::KwIf },
            StringifiedToken{ "Import", ReservedToken::KwImport },
            StringifiedToken{ "in", ReservedToken::KwIn },
            StringifiedToken{ "is", ReservedToken::KwIs },
            StringifiedToken{ "lambda", ReservedToken::KwLambda },
            StringifiedToken{ "let", ReservedToken::KwLet },
            StringifiedToken{ "Non", ReservedToken::KwNon },
            StringifiedToken{ "None", ReservedToken::KwNone },
            StringifiedToken{ "NonLocal", ReservedToken::KwNonLocal },
            StringifiedToken{ "not", ReservedToken::KwNot },
            StringifiedToken{ "Number", ReservedToken::KwNumber },
            StringifiedToken{ "or", ReservedToken::KwOr },
            StringifiedToken{ "pass", ReservedToken::KwPass },
            StringifiedToken{ "raise", ReservedToken::KwRaise },
            StringifiedToken{ "return", ReservedToken::KwReturn },
            StringifiedToken{ "String", ReservedToken::KwString },
            StringifiedToken{ "True", ReservedToken::KwTrue },
            StringifiedToken{ "try", ReservedToken::KwTry },
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
            StringifiedToken { ";", ReservedToken::OpSemicolon} ,
            StringifiedToken { ",", ReservedToken::OpComma} ,
            StringifiedToken { ".", ReservedToken::OpDot} ,
            StringifiedToken { "+", ReservedToken::OpAdd} ,
            StringifiedToken { "/", ReservedToken::OpDiv} ,
            StringifiedToken { "**", ReservedToken::OpExp} ,
            StringifiedToken { "//", ReservedToken::OpFloorDiv} ,
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
            StringifiedToken { "**=", ReservedToken::OpAssignExp} ,
            StringifiedToken { "%=", ReservedToken::OpAssignMod} ,
            StringifiedToken { "//=", ReservedToken::OpAssignFloorDiv} ,
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
        printf("\x1b[35mEating: [%c]\x1b[0m\n", m_current_char);
        m_current_char = m_input[m_position];
    }

    m_position++;
}

Token 
Lexer::read_number() {
    usize pos = m_position - 1;
    bool floating_point = false;
    bool is_valid = true;
    printf("read_number: starting position [%lu]\n", m_position);

    while (isdigit(m_current_char) != 0 || m_current_char == '.') {
        if (m_current_char == '.') {
            if (floating_point == true) {
                is_valid = false;
            }
            floating_point = true;
        }

        read_char();
    }
    printf("read_number: ending position [%lu]\n", m_position);

    if (!is_valid) {
        // Not a valid number
        printf("INVALID TOKEN\n");
        return Token(ReservedToken::Unknown);
    } else if (floating_point) {
        // Floating point number
        f64 value;
        std::from_chars(m_input.data() + pos, m_input.data() + (m_position - pos), value);
        printf("\x1b[33mDecimal: %lf\n\x1b[0m", value);
        return Token(
            value
        );
    } else {
        // Integer number
        u64 value;
        std::from_chars(m_input.data() + pos, m_input.data() + (m_position - pos), value);
        printf("\x1b[31mInteger: %lu\n\x1b[0m", value);
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

    printf("STR: [%s]\n", str.c_str());

    auto value = find(keywords, str);
    if (value.has_value()) {
        printf("\x1b[38mKeyword: [%s]\n\x1b[0m", str.c_str());
        return Token(
            value.value()
        );
    } else {
        printf("\x1b[38mIdentifier: [%s]\n\x1b[0m", str.c_str());
        return Token(
            str
        );
    }
}

Token
Lexer::read_alphanumeric() {
    Token token;

    if (isalpha(m_current_char)) {
        token = read_identifier();
    } if (isdigit(m_current_char) != 0) {
        printf("GOT NUMBER\n");
        token = read_number();
    }

    return token;
}

/// Get the next token found from the source code
Token Lexer::next_token() {
    Token token;

    skip_whitespace();
    if (isalnum(m_current_char)) {
        printf("\x1b[32mALPHANUM: %c\n\x1b[0m", m_current_char);
        token = read_alphanumeric();
    } else {
        printf("\x1b[36mNONALPHA: %c\n\x1b[0m", m_current_char);
    }

    return token;
}

}
