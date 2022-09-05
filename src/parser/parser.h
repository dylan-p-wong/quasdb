#pragma once

#include <memory>
#include <string>

#include "../common/result.h"
#include "../common/error.h"
#include "./statement/create_table.h"

#include "lexer.h"

class Statement;

class Parser {
    std::unique_ptr<LexerIterator> lexer;
public:
    Parser(std::string input);
    std::optional<Token> NextIf(bool (*predicate)(Token));
    Result<Token, Error> NextExpect(TokenType expect);
    Result<Column, Error> ParseColumn();
    Result<Expression*, Error> ParseExpression(int minimum_precedence);
    Result<Expression*, Error> ParseExpressionAtom();
    Result<Statement*, Error> ParseStatement();
    Result<Statement*, Error> ParseCreateTable();
    Result<Statement*, Error> ParseDropTable();
    Result<Statement*, Error> ParseInsert();
    Result<Statement*, Error> ParseDelete();
    Result<Statement*, Error> ParseSelect();
    Result<Statement*, Error> ParseUpdate();
};
