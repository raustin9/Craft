#include <iostream>
#include <string>
#include "defines.h"
#include "frontend/lexer.h"

int main(void) {
    // std::string input = "let x = 5;";
    std::string input = "let variable";
    compiler::Lexer lexer = compiler::Lexer(input);
    
    for (usize i = 0; i < input.length(); i++) {
        lexer.next_token();
    }

    return EXIT_SUCCESS;
}
