#include <iostream>
#include <string>
#include "defines.h"
#include "frontend/parser.h"
#include "core/ast.h"

int
main(i32 argc, char** argv) {
    std::string input = "let x: i32[4] = 5;";
    // std::string input = "define main(argc: i32, argv: Vector<String>) {let i = \"Hello String\";}";
    compiler::Parser parser = compiler::Parser(input);

    compiler::core::Program program = compiler::core::Program();
    compiler::core::AstNode* node = parser.next_node();
    while (node != nullptr) {
        program.add_node(node);
        node = parser.next_node();
    }

    program.print();

    return EXIT_SUCCESS;
}
