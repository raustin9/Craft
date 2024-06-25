#pragma once

#include "defines.h"
#include "tokens.h"
#include "type.h"
#include "utils.h"
#include "error.h"
#include "result.h"
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

/* #include <llvm/ADT/APFloat.h> */

namespace compiler {

namespace core {


// Incomplete Declarations
struct Program;
struct AstNode;
struct AstVarDecl;

/// Type alias for analyzing a result
/// TODO: Add aliases for returning Result<>s of 
///       AST nodes when parsing
using AnalyzeResult = Result<AstNode*, Error>;

    /// Enumeration of the precedences of the 
/// operators. Essentially which "bind
/// tighter"
enum operator_precedence {
    ADDSUB,
    MULDIVMOD,
};

/// Enumeration of valid operators
/// TODO: Finish adding the rest of the operators
enum class Operator : u8 {
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
};

/// Utility function to get an operator as a string value
/// Normally used for printing
constexpr const char * operator_to_cstr(const Operator& op) {
    switch (op) {
        case Operator::PLUS: return "+";
        case Operator::MINUS: return "+";
        case Operator::MUL: return "+";
        case Operator::DIV: return "+";
        case Operator::MOD: return "+";
    }

    return "__illegal_operator__";
}

/// Abstract interface class for AST Nodes
struct AstNode {
public:
    AstNode() noexcept = default;
    virtual ~AstNode() = 0;

    virtual AnalyzeResult analyze() = 0;
    virtual void print(u32 indent) {}
};

// Represents the program 
struct Program {
public:
    Program() noexcept = default;
    ~Program() {
        for (usize i = 0; i < m_nodes.size(); i++) {
            delete m_nodes[i];
        }
        
        m_nodes.clear();
    }

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

    void analyze() {
        for (usize i = 0; i < m_nodes.size(); i++) {
            m_nodes[i]->analyze();
        }
    }

private:
    // The top level nodes in a program
    std::vector<AstNode*> m_nodes;
};


/* Expressions evaluate to values */
struct AstExpr : public AstNode {
    ~AstExpr() override {}

    virtual const Type* get_type() { return nullptr; };
    AnalyzeResult analyze() override;
    void print(u32 indent) override {}
};

/* Represents a binary operator like 
 * `x + y` 
 */
struct AstBinaryExpr : public AstExpr {
    AstBinaryExpr(AstNode* lhs, Operator op, AstNode* rhs)
        : lhs(dynamic_cast<AstExpr*>(lhs))
          , op(op)
          , rhs(dynamic_cast<AstExpr*>(rhs))
          , type(nullptr)
        {}
    ~AstBinaryExpr() override {
        delete lhs;
        delete rhs;
        if (type) {
            delete type;
        }
    }
    
    const Type* get_type() override;
    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("[");
        lhs->print(0);
        printf(" %s ", operator_to_cstr(op));
        rhs->print(0);
        printf("]");
    }
   
    AstExpr* lhs;
    Operator op;
    AstExpr* rhs;
    Type* type;
};

/* Prefix expression: 
 * `~5` 
 * `!should_loop`
 */
struct AstPrefixExpr : public AstExpr {
    AstPrefixExpr(Operator op, AstNode* rhs)
        : op(op)
          , rhs(dynamic_cast<AstExpr*>(rhs))
          , type(nullptr)
        {}
    ~AstPrefixExpr() override {
        delete rhs;

        if (type) {
            delete type;
        }
    }

    const Type* get_type() override;
    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("[%s", operator_to_cstr(op));
        rhs->print(0);
    }

    Operator op;
    AstExpr* rhs;
    Type* type;
};

/* Variable declarations
 * `let i: i32 = 100;`
 * `let name: std::string = "John";
 */
struct AstVarDecl : public AstNode {
    AstVarDecl(AstNode* target, std::optional<Type*> annotation, AstNode* value)
        : target(target), 
        type(annotation),
        value(dynamic_cast<AstExpr*>(value))
        {}
    ~AstVarDecl() override {
        delete target;
        if (type.has_value()) {
            delete type.value();
        }
        delete value;
    }

    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("let ");
        target->print(0);
        if (type.has_value()) {
            printf(": %s",
                type.value()->to_str().c_str()
            );
        }
        printf(" = ");
        value->print(0);
    }
   
    AstNode* target;
    std::optional<Type*> type;
    AstExpr* value;
};

// Represents a boolean literal
struct AstBoolExpr : public AstExpr {
    AstBoolExpr(bool value)
        : value(value)
          , type(new TypeBoolean())
        {}
    ~AstBoolExpr() override {
        if (type) {
            delete type;
        }
    }

    const Type* get_type() override;
    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("%d", value);
    }

    bool value;
    Type* type;
};

// Represents an integer literal
struct AstIntegerExpr : public AstExpr {
    AstIntegerExpr(u64 value)
        : value(value)
          , type(new TypeInteger(false, 64))
        {}
    ~AstIntegerExpr() override {
        if (type) {
            delete type;
        }
    }

    const Type* get_type() override;
    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("%lu", value);
    }

    u64 value;
    Type* type;
};
    
// Represents a floating point literal
struct AstFloatExpr : public AstExpr {
    AstFloatExpr(f64 value)
        : value(value)
          , type(new TypeFloat(64))
    {}
    ~AstFloatExpr() override {
        if (type) {
            delete type;
        }
    }

    const Type* get_type() override;
    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("%lf", value);
    }

    f64 value;
    Type* type;
};

// Represents an identifier
struct AstIdentifierExpr : public AstExpr {
    AstIdentifierExpr(Identifier name)
        : name(name)
          , type(new TypeIdentifier())
        {}
    ~AstIdentifierExpr() override {
        if (type) {
            delete type;
        }
    }

    const Type* get_type() override;
    AnalyzeResult analyze() override;
    void print(u32 indent) override {
        printf("%s", name.name.c_str());
    }
    Identifier name;
    Type* type;
};

} // core namespace
} // compiler namespace
