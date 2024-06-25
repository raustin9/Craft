#pragma once
#include "defines.h"
#include <string>

namespace compiler {
namespace core {

/// Abstract interface for errors encountered 
/// during compilation
class Error {
public:
    enum class Type {
        FileSystem,
        Lexer,
        Parser,
        Semantic,
    };

    Error(Type type, std::string msg)
        : m_type(type)
          , m_msg(msg)
        {}

    void emit();

private:
    Type m_type;
    std::string m_msg;
};

} // namespace core
} // namespace compiler
