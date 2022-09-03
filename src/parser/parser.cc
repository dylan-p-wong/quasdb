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

Result<Column, Error> Parser::ParseColumn() {
    Column c{}; 

    Result<Token, Error> nameToken = NextExpect(TokenType::IdentifierValue);

    if (nameToken.isErr()) {
        return Err(nameToken.unwrapErr());
    }

    c.name = nameToken.unwrap().value;

    std::optional<Token> typeToken = NextIf([](Token t) { return t.type == TokenType::Integer || t.type == TokenType::Float || t.type == TokenType::Varchar || t.type == TokenType::Boolean; });

    if (!typeToken.has_value()) {
        return Err(Error{ErrorType::Parse, "Expected token but found another."});
    }

    if (typeToken.value().type == TokenType::Integer) {
        c.data_type = DataType::Integer;
    } else if (typeToken.value().type == TokenType::Float) {
        c.data_type = DataType::Float;
    } else if (typeToken.value().type == TokenType::Boolean) {
        c.data_type = DataType::Boolean;
    } else if (typeToken.value().type == TokenType::Varchar) {
        c.data_type = DataType::Varchar;
    } else {
        return Err(Error{ErrorType::Parse, "Expected token but found another."});
    }

    auto column_keyword = [](Token t) { return t.type ==  TokenType::Primary || t.type == TokenType::Null || t.type == TokenType::Not || t.type == TokenType::Default || t.type == TokenType::Unique || t.type == TokenType::Index || t.type == TokenType::References; };

    std::optional<Token> t = NextIf(column_keyword);

    while (t.has_value()) {
        if (t.value().type == TokenType::Primary) {
            if (NextExpect(TokenType::Key).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token KEY but found another."});
            }
            c.primary_key = true;
        } else if (t.value().type == TokenType::Null) {

        } else if (t.value().type == TokenType::Not) {
            
        } else if (t.value().type == TokenType::Default) {
            
        } else if (t.value().type == TokenType::Unique) {
            c.unique = true;
        } else if (t.value().type == TokenType::Index) {
            c.index = true;
        } else if (t.value().type == TokenType::References) {
            // Result<Token, Error> nameToken = NextExpect(TokenType::IdentifierValue);

            // if (nameToken.isErr()) {
            //     return Err(nameToken.unwrapErr());
            // }

            // c.references = nameToken.unwrap().value;
        } else {
            return Err(Error{ErrorType::Parse, "Expected token but found another."});
        }

        t = NextIf(column_keyword);
    }

    return Ok(c);
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

    while (true) {
        Result<Column, Error> c = ParseColumn();
        if (c.isErr()) {
            return Err(c.unwrapErr());
        }

        columns.push_back(c.unwrap());

        if (NextExpect(TokenType::Comma).isErr()) {
            break;
        }
    }

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
