#include <iostream>
#include <string>
#include "defines.h"
#include "frontend/lexer.h"

int
main(i32 argc, char** argv) {
    // std::string input = "let x = 5;";
    std::string input = "define main(argc: i32, argv: Vector<String>) {let i = \"Hello String\";}";
    compiler::Lexer lexer = compiler::Lexer(input);
    
    for (usize i = 0; i < input.length(); i++) {
        lexer.next_token();
    }

    return EXIT_SUCCESS;
}
