#pragma once

#include <memory>
#include <string>

#include "../common/result.h"
#include "../common/error.h"

#include "lexer.h"

class Statement;

class Parser {
    std::unique_ptr<LexerIterator> lexer;
public:
    Parser(std::string input);
    std::optional<Token> NextIf(bool (*predicate)(Token));
    Result<Token, Error> NextExpect(TokenType expect);
    Result<Statement*, Error> ParseStatement();
    Result<Statement*, Error> ParseCreateTable();
    Result<Statement*, Error> ParseDropTable();
    Result<Statement*, Error> ParseInsert();
    Result<Statement*, Error> ParseDelete();
    Result<Statement*, Error> ParseSelect();
    Result<Statement*, Error> ParseUpdate();
};
