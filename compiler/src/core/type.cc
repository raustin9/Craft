#include "type.h"
#include "core/logger.h"
#include "core/tokens.h"

namespace compiler {
namespace core {

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
