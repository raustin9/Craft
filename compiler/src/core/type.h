#pragma once
#include "defines.h"
#include "tokens.h"
#include <vector>

namespace compiler {

namespace core {

struct TypeBase {
};

// TODO: finish representing proceudre types
// Represents a procedure type
struct TypeProcedure : public TypeBase {
};

// TODO: Use symbols for this type
struct TypeIdentifier : public TypeBase {

};

// Represents an integer type
struct TypeInteger : public TypeBase {
    TypeInteger(const ReservedToken& token);

    bool is_signed; // whether the integer is signed or not
    i32 size;       // size of the integer in BYTES
};

// Represents a floating point value type
struct TypeFloat : public TypeBase {
    TypeFloat(const ReservedToken& token);

    i32 size;
};

// Represents a boolean value type
struct TypeBoolean : public TypeBase {
    TypeBoolean() noexcept = default;
};

// Represents the type of a string literal value
struct TypeStringLiteral : public TypeBase {
};

// Represents the type of a pointer value
struct TypePointer : public TypeBase {

    TypeBase* target; // the type that this is a pointer to
};

// Represents the type of an array value
struct TypeArray : public TypeBase {
    TypeArray(TypeBase* target, i32 length)
        : target(target)
          , length(length)
        {}

    TypeBase* target; // The data type that this is an array of
    i32 length;       // The amount of elements in this array
};

// Represents a data type
struct Type {
    Type() {}

    std::vector<Type*> args;
};

} // namespace core
  
} // namespace compiler
