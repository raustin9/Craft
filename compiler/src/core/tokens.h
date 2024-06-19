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

/// Reserved tokens for keywords and operators/punctuators
enum class ReservedToken : std::uint8_t {
    Unknown,

    OpParenOpen,
    OpParenClose,
    OpSubscriptOpen,
    OpSubscriptClose,
    OpCurlyBracketOpen,
    OpCurlyBracketClose,

    OpArrow,
    OpColon,
    OpSemicolon,
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

    bool operator==(std::string const & str) const {
        return (str == token_str);
    }
};


struct Identifier {
    std::string name;
};

struct Indentation {};
struct Newline {};
struct Eof{};

using Integer = u64;
using Float = f64;
using String = std::string;

class Token {
public:
    using ValueType = 
        std::variant<
            ReservedToken, 
            Identifier, 
            Integer,
            Float,
            String, 
            Indentation, 
            Newline, 
            Eof
        >;

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
