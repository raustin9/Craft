#include "core/ast.h"
#include "core/type.h"
#include "core/utils.h"
#include "core/result.h"
#include "core/error.h"

namespace compiler {
namespace core {

AnalyzeResult AstExpr::analyze() {
    return Err(Error(Error::Type::Semantic, "AstExpr should never be instantiated"));
}

/// Semantic analysis of Boolean expression
/// Nothing to analyze so return itself
AnalyzeResult AstBoolExpr::analyze() {
    return Ok(this);
}

/// Semantic analysis of Integer expression
/// Nothing to analyze so return itself
AnalyzeResult AstIntegerExpr::analyze() {
    return Ok(this);
}

/// Semantic analysis of Float expression
/// Nothing to analyze so return itself
AnalyzeResult AstFloatExpr::analyze() {
    return Ok(this);
}

AnalyzeResult AstIdentifierExpr::analyze() {
    return Err(Error(Error::Type::Semantic, "NOT DONE"));
}

/// Semantic analysis of Variable Declaration node
AnalyzeResult AstVarDecl::analyze() {
    AnalyzeResult value_res = this->value->analyze();

    if (value_res.is_ok()) {
        AstExpr* new_value = dynamic_cast<AstExpr*>(value_res.unwrap());
        if (!new_value) {
            // The value returned was not an expression. This is not allowed.
            return Err(Error(Error::Type::Semantic, "Cannot set value of variable to a non-expression!"));
        }

        // We do not provide implicit type conversion,
        // so the type of the value must match the one 
        // specified through the annotation

        if (type != new_value->get_type()) {
            return Err(Error(Error::Type::Semantic, "AstVarDecl::analyze. Assigned expression type is not the same as specified"));
        }

        return Ok(this);
    } else {
        return Err(value_res.unwrap_err());
    }
}

/// Analyze prefix expressions
/// TODO: this is not done lol
AnalyzeResult AstPrefixExpr::analyze() {
    AnalyzeResult rhs_res = rhs->analyze();

    if (rhs_res.is_ok()) {
        // AstExpr* new_rhs = dynamic_cast<AstExpr*>(rhs_res.unwrap());
        switch (op) {
            case Operator::MINUS:
                if (utils::is_type<TypeInteger>(rhs->get_type())
                    || 
                    utils::is_type<TypeFloat>(rhs->get_type())
                ) {
                    // This operator only allows for numerical types 
                    // to be negated
                }
                break;

            default:
                return Err(Error(Error::Type::Semantic, "Invalid operator for prefix expression."));
            
        }
        
        return Ok(this);
    } else {
        return Err(rhs_res.unwrap_err());
    }
}


AnalyzeResult AstBinaryExpr::analyze() {
    return Err(Error(Error::Type::Semantic, "NOT DONE"));
}

}
}
