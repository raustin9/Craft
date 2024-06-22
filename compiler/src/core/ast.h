#pragma once

#include "defines.h"
#include "tokens.h"
#include "type.h"
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

namespace compiler {

namespace core {

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
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
};

constexpr const char * operator_to_cstr(const Operator& op) {
    switch (op) {
        case Operator::PLUS: return "+";
        case Operator::MINUS: return "+";
        case Operator::MUL: return "+";
        case Operator::DIV: return "+";
        case Operator::MOD: return "+";
    }
}

struct AstNode {
public:
    AstNode() noexcept = default;

    virtual void print(u32 indent) {}
};

// Represents the program 
struct Program {
public:
    Program() noexcept = default;

    void add_node(AstNode* node) {
        this->m_nodes.push_back(node);
    }

    void print() {
        core::logger::Trace("Program:\nNumber of Nodes: {}", m_nodes.size());
        for (usize i = 0; i < m_nodes.size(); i++) {
            m_nodes[i]->print(0);
            printf("\n");
        }
    }

private:
    // The top level nodes in a program
    std::vector<AstNode*> m_nodes;
};


// Represents a type annotation
// TODO: This needs to be much more complex
struct AstTypeAnnotation : public AstNode {
    AstTypeAnnotation(Type* type)
        : type(type)
        {}

    // Delete the type for now.
    // This may not be owned later
    ~AstTypeAnnotation() {
        delete type;
    }
    
    Type* type;

    void print(u32 indent) override {
        printf("%s", type->to_str().c_str());
    }
};

/* Expressions evaluate to values */
struct AstExpr : public AstNode {

    void print(u32 indent) override {}
};

/* Represents a binary operator like 
 * `x + y` 
 */
struct AstBinaryExpr : public AstExpr {
    AstBinaryExpr(AstNode* lhs, Operator op, AstNode* rhs)
        : lhs(lhs), op(op), rhs(rhs)
        {}
    
    void print(u32 indent) override {
        printf("[");
        lhs->print(0);
        printf(" %s ", operator_to_cstr(op));
        rhs->print(0);
        printf("]");
    }
    
    AstNode* lhs;
    Operator op;
    AstNode* rhs;
};

/* Prefix expression: 
 * `~5` 
 * `!should_loop`
 */
struct AstPrefixExpr : public AstExpr {
    AstPrefixExpr(Operator op, AstNode* rhs)
        : op(op), rhs(rhs)
        {}

    void print(u32 indent) override {
        printf("[%s", operator_to_cstr(op));
        rhs->print(0);
    }

    Operator op;
    AstNode* rhs;
};

/* Variable declarations
 * `let i: i32 = 100;`
 * `let name: std::string = "John";
 */
struct AstVarDecl : public AstNode {
    AstVarDecl(AstNode* target, std::optional<AstNode*> annotation, AstNode* value)
        : target(target), 
        type_annotation(annotation),
        value(value)
        {}

    void print(u32 indent) override {
        printf("let ");
        target->print(0);
        if (type_annotation.has_value()) {
            printf(": ");
            type_annotation.value()->print(0);
        }
        printf(" = ");
        value->print(0);
    }
   
    AstNode* target;
    std::optional<AstNode*> type_annotation;
    AstNode* value;
};

// Represents a boolean literal
struct AstBoolExpr : public AstExpr {
    AstBoolExpr(bool value)
        : value(value)
        {}

    void print(u32 indent) override {
        printf("%b", value);
    }

    bool value;
};

// Represents an integer literal
struct AstIntegerExpr : public AstExpr {
    AstIntegerExpr(u64 value)
        : value(value)
        {}

    void print(u32 indent) override {
        printf("%lu", value);
    }

    u64 value;
};
    
// Represents a floating point literal
struct AstFloatExpr : public AstExpr {
    AstFloatExpr(f64 value)
        : value(value)
    {}

    void print(u32 indent) override {
        printf("%lf", value);
    }

    f64 value;
};

// Represents an identifier
struct AstIdentifierExpr : public AstExpr {
    AstIdentifierExpr(Identifier name)
        : name(name)
        {}

    void print(u32 indent) override {
        printf("%s", name.name.c_str());
    }
    Identifier name;
};

} // core namespace
} // compiler namespace
