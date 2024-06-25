#include "type.h"
#include "core/error.h"
#include "utils.h"
#include "core/logger.h"
#include "core/tokens.h"
#include <algorithm>
#include <format>

namespace compiler {
namespace core {

/* Pure virtual destructor for the abstract base class */
Type::~Type() {
}

ResultType Type::coalesce(const Type* t1, const Type* t2) {
    if (utils::is_type<TypeInteger>(t1)
        && utils::is_type<TypeInteger>(t1)
    ) {
        // Integer to integer coalesce.
        // Need to consider the signness
        // and size of the two types
        const TypeInteger* int_t1 = dynamic_cast<const TypeInteger*>(t1);
        const TypeInteger* int_t2 = dynamic_cast<const TypeInteger*>(t2);
        bool is_signed = false;
        i32 size = std::max(int_t1->size, int_t2->size);

        if (int_t1->is_signed || int_t2->is_signed) {
            // If either t1 or t2 is signed, then the coalesced one needs to be as well.
            // If neither are signed, then the result should stay unsigned
            is_signed = true;
        }

        return Ok(new TypeInteger(is_signed, size));
    } else if (
        (utils::is_type<TypeInteger>(t1) && utils::is_type<TypeFloat>(t2))
        || 
        (utils::is_type<TypeFloat>(t1) && utils::is_type<TypeInteger>(t2))
    ) {
        // One of the two types is a floating point type
        // If either one of the two types is a float, then
        // the coalesced type is a floating point as well.
        // For now, we will just cast to a 64-bit floating
        // point number to retain as much information as 
        // possible
        i32 size = 64; 
        return Ok(new TypeFloat(size));
    } else if (
        utils::is_type<TypeFloat>(t1) && utils::is_type<TypeFloat>(t2)
    ) {
        const TypeFloat* fp_t1 = dynamic_cast<const TypeFloat*>(t1);
        const TypeFloat* fp_t2 = dynamic_cast<const TypeFloat*>(t2);
        // Both types are floating point values.
        // In this case, we take the size of the 
        // larger one as the coalesced type size
        i32 size = std::max(fp_t1->size, fp_t2->size);
        return Ok(new TypeFloat(size));
    }

    // TODO: Get better error handling here
    return Err(Error(Error::Type::Semantic, "Invalid types cannot be coalesced"));
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

/// Determine if this type is the same as the one specified
const bool TypeInteger::operator==(const Type* other) {
    const TypeInteger* casted = dynamic_cast<const TypeInteger*>(other);
    if (casted == nullptr) {
        return false;
    }

    return ((this->size == casted->size) && (this->is_signed == casted->is_signed));
}

/// Get a cloned pointer of this type value
Type* TypeInteger::clone_ptr() const {
    return new TypeInteger(this->is_signed, this->size);
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

/// Get a cloned pointer of this type value
Type* TypeFloat::clone_ptr() const {
    return new TypeFloat(this->size);
}

/// See if this type and the other are the same
const bool TypeFloat::operator==(const Type* other) {
    const TypeFloat* casted = dynamic_cast<const TypeFloat*>(other);
    if (casted == nullptr) {
        return false;
    }

    return this->size == casted->size;
}

/* Return Boolean type as string value */
std::string
TypeBoolean::to_str() {
    return "[Boolean Type]";
}

/// See if the other type is also a boolean
const bool TypeBoolean::operator==(const Type* other) {
    if (!utils::is_type<TypeBoolean>(other)) {
        return false;
    }

    return true;
}

/// Return new boolean type
Type* TypeBoolean::clone_ptr() const {
    return new TypeBoolean();
}

/* Return StringLiteral type as string value */
std::string
TypeStringLiteral::to_str() {
    return "[StringLiteral Type]";
}

/// Clone this type value
Type* TypeStringLiteral::clone_ptr() const {
    return new TypeStringLiteral();
}

/// See if the other type is also a string literal
const bool TypeStringLiteral::operator==(const Type* other) {
    if (!utils::is_type<TypeStringLiteral>(other)) {
        return false;
    }

    return true;
}

/* Return Array type as string value */
std::string
TypeArray::to_str() {
    return std::vformat(
        "[Array type. Target: {}. Length: {}]", 
        std::make_format_args(target->to_str(), length)
    );
}

/// Comparison operator for array types. 
/// Array types are equivalent if their 
/// targets are the same. i.e. if they are 
/// arrays of the same type. Their lengths do not 
/// need to be equivalent
/// TODO: Do their lengths need to be the same?
const bool TypeArray::operator==(const Type* other) {
    const TypeArray* casted = dynamic_cast<const TypeArray*>(other);
    if (casted == nullptr) {
        return false;
    }

    return this->target == casted->target;
}

/// Clone this type value
Type* TypeArray::clone_ptr() const {
    return new TypeArray(this->target->clone_ptr(), this->length);
}

/* Return Pointer type as string value */
std::string
TypePointer::to_str() {
    return std::vformat(
        "[Pointer type. Target: {}]", 
        std::make_format_args(target->to_str())
    );
}

/// Clone this type value
Type* TypePointer::clone_ptr() const {
    return new TypePointer(this->target->clone_ptr());
}

/// Comparison operator for Pointer types.
/// Pointer types are equivalent if the targets 
/// that they point to are of the same type.
const bool TypePointer::operator==(const Type* other) {
    const TypePointer* casted = dynamic_cast<const TypePointer*>(other);
    if (casted == nullptr) {
        return false;
    }

    return this->target == casted->target;
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

/// See if the other type is also an identifier
const bool TypeIdentifier::operator==(const Type* other) {
    if (!utils::is_type<TypeIdentifier>(other)) {
        return false;
    }

    return true;
}

/// Return cloned identifier type
Type* TypeIdentifier::clone_ptr() const {
    return new TypeIdentifier();
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
