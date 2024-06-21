#pragma once

#include "defines.h"
#include "tokens.h"
#include <optional>
#include <string>
#include <memory>
#include <vector>

namespace core {
using namespace compiler;

// Incomplete Declarations
struct Program;
struct AstNode;
struct AstVarDecl;

enum operator_precedence {
    ADDSUB,
    MULDIVMOD,
};

// TODO: Add more operators
enum class Operator : u8 {
    PLUS = ADDSUB,
    MINUS = ADDSUB,
    MUL = MULDIVMOD,
    DIV = MULDIVMOD,
    MOD = MULDIVMOD,
};

// Represents the program 
struct Program {
public:
    Program() noexcept = default;

    void add_node(AstNode* node) {
        this->m_nodes.push_back(node);
    }

private:
    // The top level nodes in a program
    std::vector<AstNode*> m_nodes;
};

struct AstNode {
public:
    AstNode() noexcept = default;
};

// Represents a type annotation
// TODO: This needs to be much more complex
struct AstTypeAnnotation : public AstNode {
    AstTypeAnnotation(Token type)
        : type(type)
        {}
    
    Token type;
};

/* Expressions evaluate to values */
struct AstExpr : public AstNode {

};

/* Represents a binary operator like x + y */
struct AstBinaryExpr : public AstExpr {
    AstBinaryExpr(AstNode* lhs, Operator op, AstNode* rhs)
        : lhs(lhs), op(op), rhs(rhs)
        {}
    
    AstNode* lhs;
    Operator op;
    AstNode* rhs;
};

/* Prefix expression: '~5' or '!should_loop' etc */
struct AstPrefixExpr : public AstExpr {
    AstPrefixExpr(Operator op, AstNode* rhs)
        : op(op), rhs(rhs)
        {}

    Operator op;
    AstNode* rhs;
};

struct AstVarDecl : public AstNode {
    AstVarDecl(AstNode* target, std::optional<AstNode*> annotation, AstNode* value)
        : target(target), 
        type_annotation(annotation),
        value(value)
        {}
   
    AstNode* target;
    std::optional<AstNode*> type_annotation;
    AstNode* value;
};

// Represents a boolean literal
struct AstBoolExpr : public AstExpr {
    AstBoolExpr(bool value)
        : value(value)
        {}

    bool value;
};

// Represents an integer literal
struct AstIntegerExpr : public AstExpr {
    AstIntegerExpr(u64 value)
        : value(value)
        {}

    u64 value;
};
    
// Represents a floating point literal
struct AstFloatExpr : public AstExpr {
    AstFloatExpr(f64 value)
        : value(value)
    {}

    f64 value;
};

// Represents an identifier
struct AstIdentifierExpr : public AstExpr {
    AstIdentifierExpr(Identifier name)
        : name(name)
        {}
    Identifier name;
};

} // core namespace
