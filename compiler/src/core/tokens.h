#pragma once
#include "defines.h"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <array>
#include <variant>

namespace compiler {

enum class ReservedToken : std::uint8_t {
    Unknown,

    OpParenOpen,
    OpParenClose,
    OpSubscriptOpen,
    OpSubscriptClose,

    OpArrow,
    OpColon,
    OpComma,
    OpDot,

    OpAdd,
    OpDiv,
    OpExp,
    OpFloorDiv,
    OpMod,
    OpMul,
    OpSub,

    OpBinaryAnd,
    OpBinaryOr,
    OpBinaryXor,
    OpBinaryNot,
    OpBinaryLeftShift,
    OpBinaryRightShift,

    OpLogicalAnd,
    OpLogicalOr,
    OpLogicalNot,

    OpAssign,
    OpAssignAdd,
    OpAssignSub,
    OpAssignDiv,
    OpAssignMul,
    OpAssignExp,
    OpAssignMod,
    OpAssignFloorDiv,

    OpAssignBinaryAnd,
    OpAssignBinaryOr,
    OpAssignBinaryXor,
    OpAssignBinaryNot,
    OpAssignBinaryLeftShift,
    OpAssignBinaryRightShift,

    OpEqualTo,
    OpGreaterThan,
    OpLessThan,
    OpGreaterThanEqualTo,
    OpLessThanEqualTo,
    OpNotEqual,

    KwAnd,
    KwAs,
    KwAssert,
    KwBreak,
    KwClass,
    KwContinue,
    KwContract,
    KwDef,
    KwDel,
    KwElif,
    KwElse,
    KwExcept,
    KwFalse,
    KwFinally,
    KwFor,
    KwFrom,
    KwGlobal,
    KwIf,
    KwImport,
    KwIn,
    KwIs,
    KwLambda,
    KwLet,
    KwNon,
    KwNone,
    KwNonLocal,
    KwNot,
    KwNumber,
    KwOr,
    KwPass,
    KwRaise,
    KwReturn,
    KwString,
    KwTrue,
    KwTry,
    KwWhile,
    KwWith,
    KwYield,

    TOKEN_COUNT,
};

struct StringifiedToken {
    std::string_view token_str;
    ReservedToken token { ReservedToken::Unknown };

    /// Comparison for this token is comparing the values of the tokens themselves
    [[ nodiscard ]]
    constexpr bool operator<( StringifiedToken const & rhs ) const {
        return token_str < rhs.token_str;
    }
};

/// Sorting method that helps us match character streams from the source file to the corresponding token
template <typename T, std::size_t N>
[[ nodiscard ]]
constexpr std::array<T, N> sort(std::array<T,N> arr) noexcept {
    std::sort(std::begin(arr), std::end(arr));
    return arr;
}

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
            StringifiedToken { "->", ReservedToken::OpArrow} ,
            StringifiedToken { ":", ReservedToken::OpColon} ,
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

struct Identifier {
    std::string name;
};

struct Indentation {};
struct Newline {};
struct Eof{};

using Number = u32;
using String = std::string;

class Token {
public:
    using ValueType = std::variant<ReservedToken, Identifier, Number, String, Indentation, Newline, Eof>;

    Token() noexcept = default;
    Token( ValueType value) : m_value{std::move(value)} {}

    /// Check if this token is of a certain type
    template <typename T>
    [[ nodiscard ]]
    bool is() const noexcept {
        return std::holds_alternative<T>(m_value);
    }

    /// Get the value from the variant being held
    template <typename T>
    [[ nodiscard ]]
    T const& get() const noexcept {
        assert(is<T>());
        return std::get<T>(m_value);
    }

    /// Get the value being held
    [[ nodiscard ]]
    ValueType const& value() const noexcept {
        return m_value;
    }
private:
    ValueType m_value;
};

}
