#pragma once
#include "defines.h"
#include "core/logger.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <array>
#include <variant>
#include <iostream>

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

    OpFatArrow,
    OpArrow,
    OpColon,
    OpDoubleColon,
    OpSemicolon,
    OpComma,
    OpDot,

    OpAdd,
    OpDiv,
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
    OpAssignMod,

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
    KwStruct,
    KwEnum,
    KwStatic,
    KwMethod,
    KwDefer,
    KwMatch,
    KwDefault,
    KwDefine,
    KwDel,
    KwElif,
    KwElse,
    KwFalse,
    KwFor,
    KwFrom,
    KwGlobal,
    KwIf,
    KwImport,
    KwIn,
    KwIs,
    KwLambda,
    KwLet,
    KwI8,
    KwI16,
    KwI32,
    KwI64,
    KwU8,
    KwU16,
    KwU32,
    KwU64,
    KwF32,
    KwF64,
    KwPass,
    KwReturn,
    KwTrue,
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

constexpr const char* 
reserved_to_str(ReservedToken token) {
    const char* str;
    switch (token) {
        case ReservedToken::Unknown:
            str = "UNKNOWN";
            break;
        case ReservedToken::OpParenOpen:
            str = "(";
            break;
        case ReservedToken::OpParenClose:
            str = ")";
            break;
        case ReservedToken::OpSubscriptOpen:
            str = "[";
            break;
        case ReservedToken::OpSubscriptClose:
            str = "]";
            break;
        case ReservedToken::OpCurlyBracketOpen:
            str = "{";
            break;
        case ReservedToken::OpCurlyBracketClose:
            str = "}";
            break;
        case ReservedToken::OpFatArrow:
            str = "=>";
            break;
        case ReservedToken::OpArrow:
            str = "->";
            break;
        case ReservedToken::OpColon:
            str = ":";
            break;
        case ReservedToken::OpDoubleColon:
            str = "::";
            break;
        case ReservedToken::OpSemicolon:
            str = ";";
            break;
        case ReservedToken::OpComma:
            str = ",";
            break;
        case ReservedToken::OpDot:
            str = ".";
            break;
        case ReservedToken::OpAdd:
            str = "+";
            break;
        case ReservedToken::OpDiv:
            str = "/";
            break;
        case ReservedToken::OpMod:
            str = "%";
            break;
        case ReservedToken::OpMul:
            str = "*";
            break;
        case ReservedToken::OpSub:
            str = "-";
            break;
        case ReservedToken::OpBinaryAnd:
            str = "&";
            break;
        case ReservedToken::OpBinaryOr:
            str = "|";
            break;
        case ReservedToken::OpBinaryXor:
            str = "^";
            break;
        case ReservedToken::OpBinaryNot:
            str = "~";
            break;
        case ReservedToken::OpBinaryLeftShift:
            str = "<<";
            break;
        case ReservedToken::OpBinaryRightShift:
            str = ">>";
            break;
        case ReservedToken::OpLogicalAnd:
            str = "&&";
            break;
        case ReservedToken::OpLogicalOr:
            str = "||";
            break;
        case ReservedToken::OpLogicalNot:
            str = "!";
            break;
        case ReservedToken::OpAssign:
            str = "=";
            break;
        case ReservedToken::OpAssignAdd:
            str = "+=";
            break;
        case ReservedToken::OpAssignSub:
            str = "-=";
            break;
        case ReservedToken::OpAssignDiv:
            str = "/=";
            break;
        case ReservedToken::OpAssignMul:
            str = "*=";
            break;
        case ReservedToken::OpAssignMod:
            str = "%=";
            break;
        case ReservedToken::OpAssignBinaryAnd:
            str = "&=";
            break;
        case ReservedToken::OpAssignBinaryOr:
            str = "|=";
            break;
        case ReservedToken::OpAssignBinaryXor:
            str = "^=";
            break;
        case ReservedToken::OpAssignBinaryNot:
            str = "~=";
            break;
        case ReservedToken::OpAssignBinaryLeftShift:
            str = "<<=";
            break;
        case ReservedToken::OpAssignBinaryRightShift:
            str = ">>=";
            break;
        case ReservedToken::OpEqualTo:
            str = "==";
            break;
        case ReservedToken::OpGreaterThan:
            str = ">";
            break;
        case ReservedToken::OpLessThan:
            str = "<";
            break;
        case ReservedToken::OpGreaterThanEqualTo:
            str = ">=";
            break;
        case ReservedToken::OpLessThanEqualTo:
            str = "<=";
            break;
        case ReservedToken::OpNotEqual:
            str = "!=";
            break;
        case ReservedToken::KwAnd:
            str = "and";
            break;
        case ReservedToken::KwAs:
            str = "as";
            break;
        case ReservedToken::KwAssert:
            str = "assert";
            break;
        case ReservedToken::KwBreak:
            str = "break";
            break;
        case ReservedToken::KwClass:
            str = "class";
            break;
        case ReservedToken::KwContinue:
            str = "continue";
            break;
        case ReservedToken::KwContract:
            str = "contract";
            break;
        case ReservedToken::KwDefine:
            str = "define";
            break;
        case ReservedToken::KwDel:
            str = "del";
            break;
        case ReservedToken::KwElif:
            str = "elif";
            break;
        case ReservedToken::KwElse:
            str = "else";
            break;
        case ReservedToken::KwFalse:
            str = "false";
            break;
        case ReservedToken::KwFor:
            str = "for";
            break;
        case ReservedToken::KwFrom:
            str = "from";
            break;
        case ReservedToken::KwGlobal:
            str = "global";
            break;
        case ReservedToken::KwIf:
            str = "if";
            break;
        case ReservedToken::KwImport:
            str = "import";
            break;
        case ReservedToken::KwIn:
            str = "import";
            break;
        case ReservedToken::KwIs:
            str = "is";
            break;
        case ReservedToken::KwLambda:
            str = "lambda";
            break;
        case ReservedToken::KwLet:
            str = "let";
            break;
        case ReservedToken::KwI8:
            str = "i8";
            break;
        case ReservedToken::KwI16:
            str = "i16";
            break;
        case ReservedToken::KwI32:
            str = "i32";
            break;
        case ReservedToken::KwI64:
            str = "i64";
            break;
        case ReservedToken::KwU8:
            str = "u8";
            break;
        case ReservedToken::KwU16:
            str = "u16";
            break;
        case ReservedToken::KwU32:
            str = "u32";
            break;
        case ReservedToken::KwU64:
            str = "u64";
            break;
        case ReservedToken::KwF32:
            str = "f32";
            break;
        case ReservedToken::KwF64:
            str = "f64";
            break;
        case ReservedToken::KwPass:
            str = "pass";
            break;
        case ReservedToken::KwReturn:
            str = "return";
            break;
        case ReservedToken::KwTrue:
            str = "true";
            break;
        case ReservedToken::KwWhile:
            str = "while";
            break;
        case ReservedToken::KwWith:
            str = "with";
            break;
        case ReservedToken::KwYield:
            str = "yield";
            break;
        default:
            str = "__invalid_reserved__";
            break;
    }

    return str;
}

struct Identifier {
    Identifier(std::string name) : name(name) {}
    std::string name;
};

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

    void print() {
        if (this->is<ReservedToken>()) {
            core::logger::Debug("Token <[{}] : ReservedToken>", reserved_to_str(this->get<ReservedToken>()));
        } else if (this->is<Integer>()) {
            core::logger::Debug("Token<[{}] : Integer>", this->get<Integer>());
        } else if (this->is<Float>()) {
            core::logger::Debug("Token<[{}] : Float>", this->get<Float>());
        } else if (this->is<String>()) {
            core::logger::Debug("Token<[{}] : String>", this->get<String>());
        } else if (this->is<Identifier>()) {
            core::logger::Debug("Token<[{}] : Identifier>", this->get<Identifier>().name);
        }else if (this->is<Eof>()) {
            core::logger::Debug("Token <__EOF__>");
        }
    }

    std::string to_str() {
        if (this->is<ReservedToken>()) {
            return std::vformat("Token <[{}] : ReservedToken>", std::make_format_args(reserved_to_str(this->get<ReservedToken>())));
        } else if (this->is<Integer>()) {
            return std::vformat("Token<[{}] : Integer>", std::make_format_args(this->get<Integer>()));
        } else if (this->is<Float>()) {
            return std::vformat("Token<[{}] : Float>", std::make_format_args(this->get<Float>()));
        } else if (this->is<String>()) {
            return std::vformat("Token<[{}] : String>", std::make_format_args(this->get<String>()));
        } else if (this->is<Identifier>()) {
            return std::vformat("Token<[{}] : Identifier>", std::make_format_args(this->get<Identifier>().name));
        }else if (this->is<Eof>()) {
            return "Token <__EOF__>";
        }

        return "Invalid Token";
    }
private:
    ValueType m_value;
};

}
