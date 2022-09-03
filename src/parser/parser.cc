#include "parser.h"

Parser::Parser(std::string input) : lexer{std::unique_ptr<LexerIterator>(new LexerIterator{input})} {}

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
    return Err(Error{ErrorType::Parse, ""});
}

Result<Statement*, Error> Parser::ParseDropTable() {
    return Err(Error{ErrorType::Parse, ""});
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
