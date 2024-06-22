#pragma once
#include "defines.h"
#include "tokens.h"
#include <string>
#include <vector>

namespace compiler {

namespace core {

/* Abstract interface that should never be instantiated */
struct Type {
    virtual ~Type() = 0;
    virtual std::string to_str() = 0;
};

// TODO: finish representing proceudre types
// Represents a procedure type
struct TypeProcedure : public Type {
    ~TypeProcedure() override {}
    std::string to_str() override;
};

// TODO: Use symbols for this type
struct TypeIdentifier : public Type {
    TypeIdentifier() {}
    ~TypeIdentifier() override {}
    std::string to_str() override;
};

// Represents an integer type
struct TypeInteger : public Type {
    ~TypeInteger() override {}
    TypeInteger(const ReservedToken& token);
    std::string to_str() override;

    bool is_signed; // whether the integer is signed or not
    i32 size;       // size of the integer in BYTES
};

// Represents a floating point value type
struct TypeFloat : public Type {
    ~TypeFloat() override {}
    TypeFloat(const ReservedToken& token);
    std::string to_str() override;

    i32 size;
};

// Represents a boolean value type
struct TypeBoolean : public Type {
    TypeBoolean() noexcept = default;
    ~TypeBoolean() override {}
    std::string to_str() override;
};

// Represents the type of a string literal value
struct TypeStringLiteral : public Type {
    ~TypeStringLiteral() override {}
    std::string to_str() override;
};

// Represents the type of a pointer value
struct TypePointer : public Type {
    TypePointer(Type* target)
        : target(target)
        {}
    ~TypePointer() override {
        delete target;
    }
    std::string to_str() override;

    Type* target; // the type that this is a pointer to
};

// Represents the type of an array value
struct TypeArray : public Type {
    ~TypeArray() override {}
    TypeArray(Type* target, i32 length)
        : target(target)
          , length(length)
        {}
    std::string to_str() override;

    Type* target; // The data type that this is an array of
    i32 length;       // The amount of elements in this array
};

// TODO: Create representations for `struct` and `enum` types

} // namespace core
  
} // namespace compiler
