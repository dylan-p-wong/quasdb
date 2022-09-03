#include "parser.h"
#include "./statement/create_table.h"
#include "./statement/drop_table.h"

Parser::Parser(std::string input) : lexer{std::unique_ptr<LexerIterator>(new LexerIterator{input})} {}

std::optional<Token> Parser::NextIf(bool (*predicate)(Token)) {
    if (lexer.get()->Peek().isOk() && predicate(lexer.get()->Peek().unwrap())) {
        Token temp = lexer.get()->Peek().unwrap();
        lexer->Next();
        return temp;
    }

    return std::nullopt;
}

Result<Token, Error> Parser::NextExpect(TokenType expect) {
    if (lexer.get()->Peek().isOk()) {
        Token t = lexer.get()->Peek().unwrap();

        if (t.type == expect) {
            lexer->Next();
            return Ok(t);
        }

        return Err(Error{ErrorType::Parse, "Expected token but found another."});
    }

    return Err(lexer.get()->Peek().unwrapErr());
}

Result<Statement*, Error> Parser::ParseStatement() {
    if (lexer.get()->Peek().isErr()) {
        return Err(lexer.get()->Peek().unwrapErr());
    }

    switch (lexer.get()->Peek().unwrap().type) {
        case TokenType::Create:
            return ParseCreateTable();
        case TokenType::Drop:
            return ParseDropTable();
        case TokenType::Insert:
            return ParseInsert();
        case TokenType::Delete:
            return ParseDelete();
        case TokenType::Select:
            return ParseSelect();
        case TokenType::Update:
            return ParseUpdate();
        default:
            return Err(Error{ErrorType::Parse, "Unexpected token " + lexer.get()->Peek().unwrap().value});
    }
}

Result<Statement*, Error> Parser::ParseCreateTable() {
    Result<Token, Error> createToken = NextExpect(TokenType::Create);

    if (createToken.isErr()) {
        return Err(createToken.unwrapErr());
    }

    Result<Token, Error> tableToken = NextExpect(TokenType::Table);

    if (tableToken.isErr()) {
        return Err(tableToken.unwrapErr());
    }

    Result<Token, Error> tableNameToken = NextExpect(TokenType::IdentifierValue);

    if (tableNameToken.isErr()) {
        return Err(tableNameToken.unwrapErr());
    }

    Result<Token, Error> openParenToken = NextExpect(TokenType::OpenParen);
    if (openParenToken.isErr()) {
        return Err(openParenToken.unwrapErr());
    }

    std::vector<Column> columns;

    Result<Token, Error> closeParenToken = NextExpect(TokenType::CloseParen);
    if (closeParenToken.isErr()) {
        return Err(closeParenToken.unwrapErr());
    }

    Statement * res = new CreateTable{tableNameToken.unwrap().value, columns};
    return Ok(res);
}

Result<Statement*, Error> Parser::ParseDropTable() {
    Result<Token, Error> dropToken = NextExpect(TokenType::Drop);

    if (dropToken.isErr()) {
        return Err(dropToken.unwrapErr());
    }

    Result<Token, Error> tableToken = NextExpect(TokenType::Table);

    if (tableToken.isErr()) {
        return Err(tableToken.unwrapErr());
    }

    Result<Token, Error> tableNameToken = NextExpect(TokenType::IdentifierValue);

    if (tableNameToken.isErr()) {
        return Err(tableNameToken.unwrapErr());
    }

    Statement * res = new DropTable{tableNameToken.unwrap().value};
    return Ok(res);
}

Result<Statement*, Error> Parser::ParseInsert() {
    return Err(Error{ErrorType::Parse, ""});
}

Result<Statement*, Error> Parser::ParseDelete() {
    return Err(Error{ErrorType::Parse, ""});
}

Result<Statement*, Error> Parser::ParseSelect() {
    return Err(Error{ErrorType::Parse, ""});
}

Result<Statement*, Error> Parser::ParseUpdate() {
    return Err(Error{ErrorType::Parse, ""});
}
