#pragma once
#include "defines.h"

#include "pass.h"
#include <memory>

namespace compiler {
namespace core {

class CompilerContext {
public:
    // Constructors
    CompilerContext() = default;

    // Destructor
    ~CompilerContext() {

    }

    
private:
    std::unique_ptr<CompilerPass> m_pass; // the compiler pass that this context is in

};

}
}
