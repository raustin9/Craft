#pragma once
#include "defines.h"
#include "result.h"
#include "error.h"
#include "tokens.h"
#include <string>
#include <vector>

namespace compiler {

namespace core {

// Incomplete definitions...
struct Type;

using ResultType = Result<Type*, Error>;

/* Abstract interface that should never be instantiated */
struct Type {
    virtual ~Type() = 0;
    virtual std::string to_str() = 0;
    virtual Type* clone_ptr() const = 0;

    // TODO: we might not need this?
    // If we do not want to provide implicit type 
    // conversions then we should not allow this 
    // behavior.
    static ResultType coalesce(const Type* t1, const Type* t2);

    virtual const bool operator==(const Type* other) = 0;

    Type* arg;
};


// TODO: finish representing proceudre types
// Represents a procedure type
struct TypeProcedure : public Type {
    ~TypeProcedure() override {}
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;
};

// TODO: Use symbols for this type
// This is meant to be used for structs and enums
// not variables. Variables will be BOUND
// to a type that can be an identifier.
// Essentially:
// `let x: MyEnum = ...`
// MyEnum will be the type
struct TypeIdentifier : public Type {
    TypeIdentifier() {}
    ~TypeIdentifier() override {}
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;
};

// Represents an integer type
struct TypeInteger : public Type {
    ~TypeInteger() override {}
    TypeInteger(const ReservedToken& token);
    TypeInteger(bool is_signed, i32 size)
        : is_signed(is_signed)
          , size(size)
        {}
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;

    bool is_signed; // whether the integer is signed or not
    i32 size;       // size of the integer in BYTES
};

// Represents a floating point value type
struct TypeFloat : public Type {
    ~TypeFloat() override {}
    TypeFloat(const ReservedToken& token);
    TypeFloat(i32 size) : size(size) {}
    Type* clone_ptr() const override;

    const bool operator==(const Type* other) override;
    
    std::string to_str() override;

    i32 size;
};

// Represents a boolean value type
struct TypeBoolean : public Type {
    TypeBoolean() noexcept = default;
    ~TypeBoolean() override {}
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;
};

// Represents the type of a string literal value
struct TypeStringLiteral : public Type {
    ~TypeStringLiteral() override {}
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;
};

// Represents the type of a pointer value
struct TypePointer : public Type {
    TypePointer(Type* target)
        : target(target)
        {}
    ~TypePointer() override {
        delete target;
    }
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;

    Type* target; // the type that this is a pointer to
};

// Represents the type of an array value
struct TypeArray : public Type {
    TypeArray(Type* target, i32 length)
        : target(target)
          , length(length)
        {}
    ~TypeArray() override {
        delete target;
    }
    Type* clone_ptr() const override;
    std::string to_str() override;
    const bool operator==(const Type* other) override;

    Type* target; // The data type that this is an array of
    i32 length;       // The amount of elements in this array
};

// TODO: Create representations for `struct` and `enum` types

} // namespace core
  
} // namespace compiler
