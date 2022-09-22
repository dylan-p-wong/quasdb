#pragma once

#include <memory>
#include <string>

#include "../common/result.h"
#include "../common/error.h"
#include "./statement/create_table.h"
#include "./statement/select.h"

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
    Result<std::vector<SelectItem*>, Error> ParseSelectClause();
    Result<std::vector<FromItem*>, Error> ParseFromClause();
    Result<FromItem*, Error> ParseFromItem();
    Result<JoinType, Error> ParseJoinType();
    Result<Expression*, Error> ParseWhereClause();
    Result<std::vector<Expression*>, Error> ParseGroupByClause();
    Result<Expression*, Error> ParseHavingClause();
    Result<std::vector<OrderItem*>, Error> ParseOrderByClause();
    Result<Expression*, Error> ParseLimitClause();
    Result<Expression*, Error> ParseOffsetClause();
};
