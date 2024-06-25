#include "ast.h"

namespace compiler {
namespace core {

AstNode::~AstNode() {}

/// Type getters for primitive types in the AST
const Type* AstIntegerExpr::get_type() { return type; }
const Type* AstFloatExpr::get_type() { return type; }
const Type* AstBoolExpr::get_type() { return type; }

/// Identifiers get their types from the symbol table
/// TODO: Get the type from the symbol table
const Type* AstIdentifierExpr::get_type() {
    return type;
}

/// Binary expressions have types on both the lhs and rhs of 
/// themselves. This value should get initialized through 
/// coalescing the two type values when initializing 
/// this object
const Type* AstBinaryExpr::get_type() {
    if (this->type) {
        return this->type;
    }
    
    this->analyze();
    switch(op) {
        // Arithmetic Operators
        // These operators can only be used on numerical values
        // like Integers and Floating Point values
        case Operator::PLUS:
        case Operator::MINUS:
        case Operator::MUL:
        case Operator::DIV:
        case Operator::MOD:
            
            break;
        // TODO: Fill this out as more operators get created
    }
    return type;
}

/// Get the type of the prefix expression. This type is confirmed after 
/// being analyzed. If our type is null, then we analyze first.
const Type* AstPrefixExpr::get_type() {
    if (this->type) {
        return type;
    }

    this->analyze();
    return this->type;
}

} // namespace core
} // namespace compiler
