#include "type.h"
#include "core/logger.h"
#include "core/tokens.h"
#include <format>

namespace compiler {
namespace core {

/* Pure virtual destructor for the abstract base class */
Type::~Type() {
}

// Constructor for the [TypeInteger] struct
TypeInteger::TypeInteger(const ReservedToken& t) {
    switch (t) {
        // Unsigned integers
        case ReservedToken::KwU8:
            is_signed = false;
            size = 1;
            return;
        case ReservedToken::KwU16:
            is_signed = false;
            size = 2;
            return;
        case ReservedToken::KwU32:
            is_signed = false;
            size = 4;
            return;
        case ReservedToken::KwU64:
            is_signed = false;
            size = 8;
            return;
       
        // Signed integers
        case ReservedToken::KwI8:
            is_signed = true;
            size = 1;
            return;
        case ReservedToken::KwI16:
            is_signed = true;
            size = 2;
            return;
        case ReservedToken::KwI32:
            is_signed = true;
            size = 4;
            return;
        case ReservedToken::KwI64:
            is_signed = true;
            size = 8;
            return;

        default:
            is_signed = false;
            size = -1;
            core::logger::Error("Cannot form integer type with token {}", reserved_to_str(t));
    }
}

/* Return Integer type as a string value */
std::string
TypeInteger::to_str() {
    return std::vformat(
        "[Integer type. Size: {} bytes. Signed: {}]", 
        std::make_format_args(size, is_signed)
    );
}

/* Return Floating Point type as string value */
std::string
TypeFloat::to_str() {
    return std::vformat(
        "[Float type. Size: {}]", 
        std::make_format_args(size)
    );
}

/* Return Boolean type as string value */
std::string
TypeBoolean::to_str() {
    return "[Boolean Type]";
}

/* Return StringLiteral type as string value */
std::string
TypeStringLiteral::to_str() {
    return "[StringLiteral Type]";
}

/* Return Array type as string value */
std::string
TypeArray::to_str() {
    return std::vformat(
        "[Array type. Target: {}. Length: {}]", 
        std::make_format_args(target->to_str(), length)
    );
}

/* Return Pointer type as string value */
std::string
TypePointer::to_str() {
    return std::vformat(
        "[Pointer type. Target: {}]", 
        std::make_format_args(target->to_str())
    );
}

/* Return Pointer type as string value */
std::string
TypeIdentifier::to_str() {
    return "[Identifier Type]";
    /* return std::vformat( */
    /*     "[Pointer type. Target: {}]",  */
    /*     std::make_format_args(target->to_str()) */
    /* ); */
}


// Constructor for the [TypeFloat] struct
TypeFloat::TypeFloat(const ReservedToken& t) {
    switch (t) {
        case ReservedToken::KwF32:
            size = 4;
            return;
        case ReservedToken::KwF64:
            size = 8;
            return;
        
        default:
            size = -1;
            core::logger::Error("Cannot form integer type with token {}", reserved_to_str(t));
    }
}

} // namespace core
} // namespace compiler
