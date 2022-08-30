#pragma once

#include <string>

enum Token { SELECT };

class Lexer {
    std::string::iterator iter;
public:
    Lexer(std::string input);
    ~Lexer();
};
