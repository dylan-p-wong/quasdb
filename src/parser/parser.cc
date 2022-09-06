#include <limits>

#include "parser.h"
#include "./statement/create_table.h"
#include "./statement/drop_table.h"

namespace {
    // https://docs.microsoft.com/en-us/sql/t-sql/language-elements/operator-precedence-transact-sql?view=sql-server-ver16
    int GetPrecedence(TokenType t) {
        if (t == TokenType::Exclamation || t == TokenType::Is) {
            return 8;
        } else if (t == TokenType::Caret) {
            return 7;
        } else if (t == TokenType::Asterisk || t == TokenType::Slash || t == TokenType::Percent) {
            return 6;
        } else if (t == TokenType::Plus || t == TokenType::Minus) {
            return 5;
        } else if (t == TokenType::GreaterThan || t == TokenType::LessThan || t == TokenType::LessOrGreaterThan || t == TokenType::LessThanOrEqual) {
            return 4;
        } else if (t == TokenType::Equal || t == TokenType::NotEqual) {
            return 3;
        } else if (t == TokenType::And) {
            return 2;
        } else if (t == TokenType::Or) {
            return 1;
        } else {
            return -1;
        }
    }

    enum class AssociativityType { Left, Right };

    AssociativityType GetAssociativity(TokenType t) {
        if (t == TokenType::Caret) {
            return AssociativityType::Right;
        }

        return AssociativityType::Left;
    }

    Expression * GetOperatorInfix(TokenType t, Expression * lhs, Expression * rhs) {
        switch (t) {
            case TokenType::Or:
                return new InfixOperation{ExpressionType::Or, lhs, rhs};
            case TokenType::And:
                return new InfixOperation{ExpressionType::And, lhs, rhs};
            case TokenType::Equal:
                return new InfixOperation{ExpressionType::Equal, lhs, rhs};
            case TokenType::NotEqual:
                return new InfixOperation{ExpressionType::NotEqual, lhs, rhs};
            case TokenType::GreaterThan:
                return new InfixOperation{ExpressionType::GreaterThan, lhs, rhs};
            case TokenType::LessThan:
                return new InfixOperation{ExpressionType::LessThan, lhs, rhs};
            case TokenType::Plus:
                return new InfixOperation{ExpressionType::Add, lhs, rhs};
            case TokenType::Minus:
                return new InfixOperation{ExpressionType::Minus, lhs, rhs};
            case TokenType::Asterisk:
                return new InfixOperation{ExpressionType::Multiply, lhs, rhs};
            case TokenType::Slash:
                return new InfixOperation{ExpressionType::Divide, lhs, rhs};
            case TokenType::Percent:
                return new InfixOperation{ExpressionType::Modulo, lhs, rhs};
            case TokenType::Caret:
                return new InfixOperation{ExpressionType::Exponentiate, lhs, rhs};
            default:
                return nullptr; // Error?
        }
    }

    Expression * GetOperatorPostfix(TokenType t, Expression * e) {
        switch (t) {
            case TokenType::Exclamation:
                return new PostfixOperation{ExpressionType::Factorial, e};
            default:
                return nullptr; // Error?
        }
    }

    bool IsPostfixOperatorKeyword(Token t) {
        return t.type == TokenType::Is || t.type == TokenType::Exclamation;
    }

    bool IsInfixOperatorKeyword(Token t) { 
        return GetPrecedence(t.type) > 0 && !IsPostfixOperatorKeyword(t); 
    };
}

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

    auto column_keyword = [](Token t) { return t.type ==  TokenType::Primary || t.type == TokenType::Null || t.type == TokenType::Not || t.type == TokenType::Default || t.type == TokenType::Unique || t.type == TokenType::Index || t.type == TokenType::Foreign; };

    std::optional<Token> t = NextIf(column_keyword);

    while (t.has_value()) {
        if (t.value().type == TokenType::Primary) {
            if (NextExpect(TokenType::Key).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token KEY but found another."});
            }
            c.primary_key = true;
        } else if (t.value().type == TokenType::Not) {
            if (NextExpect(TokenType::Null).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token NULL but found another."});
            }
            c.nullable = false;
        } else if (t.value().type == TokenType::Default) {
            Result<Expression *, Error> r = ParseExpression(0);

            if (r.isErr()) {
                return Err(r.unwrapErr());
            }

            c.default_value = r.unwrap();
        } else if (t.value().type == TokenType::Unique) {
            c.unique = true;
        } else if (t.value().type == TokenType::Index) {
            c.index = true;
        } else if (t.value().type == TokenType::Foreign) {
            if (NextExpect(TokenType::Key).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token KEY but found another."});
            }
            if (NextExpect(TokenType::References).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token REFERENCES but found another."});
            }

            Result<Token, Error> tableToken = NextExpect(TokenType::IdentifierValue);

            if (tableToken.isErr()) {
                return Err(tableToken.unwrapErr());
            }

            if (NextExpect(TokenType::OpenParen).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token OPENPAREN but found another."});
            }

            Result<Token, Error> columnToken = NextExpect(TokenType::IdentifierValue);

            if (columnToken.isErr()) {
                return Err(columnToken.unwrapErr());
            }

            c.references = std::make_pair(tableToken.unwrap().value, columnToken.unwrap().value);

            if (NextExpect(TokenType::CloseParen).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token CLOSEDPAREN but found another."});
            }
        } else {
            return Err(Error{ErrorType::Parse, "Expected token but found another."});
        }

        t = NextIf(column_keyword);
    }

    return Ok(c);
}

// Precedence Climbing Algorithm
// https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing
Result<Expression*, Error> Parser::ParseExpression(int minimum_precedence) {
    Result<Expression*, Error> result_w_error = ParseExpressionAtom();

    if (result_w_error.isErr()) {
        return Err(result_w_error.unwrapErr());
    }

    Expression * result = result_w_error.unwrap();
    
    // Postfix
    while (lexer->Peek().isOk() && IsPostfixOperatorKeyword(lexer->Peek().unwrap()) && GetPrecedence(lexer->Peek().unwrap().type) >= minimum_precedence) {
        std::optional<Token> t = NextIf(IsPostfixOperatorKeyword);

        if (t.value().type == TokenType::Is) {
            bool nullable = true;

            if (NextExpect(TokenType::Not).isOk()) {
                nullable = false;
            }

            if (NextExpect(TokenType::Null).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected NULL got another."});
            }

            result = new IsNullOperation{nullable, result};
        } else if (TokenType::Exclamation) {
            result = GetOperatorPostfix(t.value().type, result);
        } else {
            return Err(Error{ErrorType::Parse, "Unknown postfix error."});
        }
        
        if (result == nullptr) {
            return Err(Error{ErrorType::Parse, "Error getting postfix operator."});
        }
    }

    // Infix
    while (lexer->Peek().isOk() && IsInfixOperatorKeyword(lexer->Peek().unwrap()) && GetPrecedence(lexer->Peek().unwrap().type) >= minimum_precedence) {
        std::optional<Token> t = NextIf(IsInfixOperatorKeyword);

        int next_minimum_precedence = GetPrecedence(t.value().type);

        if (GetAssociativity(t.value().type) == AssociativityType::Left) {
            ++next_minimum_precedence;
        }

        Result<Expression*, Error> right_hand_side = ParseExpression(next_minimum_precedence);
        
        if (right_hand_side.isErr()) {
            return Err(right_hand_side.unwrapErr());
        }
        
        result = GetOperatorInfix(t.value().type, result, right_hand_side.unwrap());
        if (result == nullptr) {
            return Err(Error{ErrorType::Parse, "Error getting infix operator."});
        }
    }
    
    return Ok(result);
}

Result<Expression*, Error> Parser::ParseExpressionAtom() {
    Result<Token, Error> t = lexer->Next();

    if (t.isErr()) {
        return Err(t.unwrapErr());
    }

    switch (t.unwrap().type) {
        case TokenType::IdentifierValue:
        {
            std::string from = "";
            std::string field = t.unwrap().value;

            if (NextIf([](Token t){ return t.type == TokenType::Period; })) {
                Result<Token, Error> fieldToken = NextExpect(TokenType::IdentifierValue);

                if (fieldToken.isErr()) {
                    return Err(fieldToken.unwrapErr());
                }

                from = field;
                field = fieldToken.unwrap().value;
            }
            Expression * res = new Field(from, field);
            return Ok(res);
        }
        case TokenType::NumberValue:
        {
            try {
                Expression * res = new IntegerLiteral{std::stoi(t.unwrap().value)};
                return Ok(res);
            } catch (...) {
                return Err(Error{ErrorType::Parse, "Error parsing number value."});
            }
        }
        case TokenType::OpenParen:
        {
            Result<Expression*, Error> expression = ParseExpression(0);
            if (expression.isErr()) {
                return Err(expression.unwrapErr());
            }

            if (NextExpect(TokenType::CloseParen).isErr()) {
                return Err(Error{ErrorType::Parse, "Expected token CloseParen but found another."});
            }

            return Ok(expression.unwrap());
        }
        case TokenType::StringValue:
        {
            Expression * res = new StringLiteral{t.unwrap().value};
            return Ok(res);
        }
        case TokenType::False:
        {
            Expression * res = new BooleanLiteral{false};
            return Ok(res);
        }
        case TokenType::True:
        {
            Expression * res = new BooleanLiteral{false};
            return Ok(res);
        }
        case TokenType::Infinity:
        {
            Expression * res = new FloatLiteral{std::numeric_limits<float>::max()};
            return Ok(res);
        }
        case TokenType::NaN:
        {
            Expression * res = new FloatLiteral{std::numeric_limits<float>::quiet_NaN()};
            return Ok(res);
        }
        case TokenType::Null:
        {
            Expression * res = new NullLiteral{};
            return Ok(res);
        }       
    default:
        return Err(Error{ErrorType::Parse, "Expected expression atom, found another."});
    }
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
