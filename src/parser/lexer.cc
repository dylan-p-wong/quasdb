#include "lexer.h"

Token::Token(TokenType type, std::string value) : type{type}, value{value} {}

TokenType ValueToTokenType(std::string v) {
    std::transform(v.begin(), v.end(), v.begin(), ::toupper);

    if (v == "AND") return TokenType::And;
    if (v == "AS") return TokenType::As;
    if (v == "ASC") return TokenType::Asc;
    if (v == "BEGIN") return TokenType::Begin;
    if (v == "BOOL") return TokenType::Bool;
    if (v == "BOOLEAN") return TokenType::Boolean;
    if (v == "BY") return TokenType::By;
    if (v == "CHAR") return TokenType::Char;
    if (v == "COMMIT") return TokenType::Commit;
    if (v == "CREATE") return TokenType::Create;
    if (v == "CROSS") return TokenType::Cross;
    if (v == "DEFAULT") return TokenType::Default;
    if (v == "DELETE") return TokenType::Delete;
    if (v == "DESC") return TokenType::Desc;
    if (v == "DOUBLE") return TokenType::Double;
    if (v == "DROP") return TokenType::Drop;
    if (v == "EXPLAIN") return TokenType::Explain;
    if (v == "FALSE") return TokenType::False;
    if (v == "FLOAT") return TokenType::Float;
    if (v == "FOREIGN") return TokenType::Foreign;
    if (v == "FROM") return TokenType::From;
    if (v == "GROUP") return TokenType::Group;
    if (v == "HAVING") return TokenType::Having;
    if (v == "INDEX") return TokenType::Index;
    if (v == "INFINITY") return TokenType::Infinity;
    if (v == "INNER") return TokenType::Inner;
    if (v == "INSERT") return TokenType::Insert;
    if (v == "INT") return TokenType::Int;
    if (v == "INTEGER") return TokenType::Integer;
    if (v == "INTO") return TokenType::Into;
    if (v == "IS") return TokenType::Is;
    if (v == "JOIN") return TokenType::Join;
    if (v == "KEY") return TokenType::Key;
    if (v == "LEFT") return TokenType::Left;
    if (v == "LIKE") return TokenType::Like;
    if (v == "LIMIT") return TokenType::Limit;
    if (v == "NAN") return TokenType::NaN;
    if (v == "NOT") return TokenType::Not;
    if (v == "NULL") return TokenType::Null;
    if (v == "OF") return TokenType::Of;
    if (v == "OFFSET") return TokenType::Offset;
    if (v == "ON") return TokenType::On;
    if (v == "ONLY") return TokenType::Only;
    if (v == "OR") return TokenType::Or;
    if (v == "ORDER") return TokenType::Order;
    if (v == "OUTER") return TokenType::Outer;
    if (v == "PRIMARY") return TokenType::Primary;
    if (v == "READ") return TokenType::Read;
    if (v == "REFERENCES") return TokenType::References;
    if (v == "RIGHT") return TokenType::Right;
    if (v == "ROLLBACK") return TokenType::Rollback;
    if (v == "SELECT") return TokenType::Select;
    if (v == "SET") return TokenType::Set;
    if (v == "STRING") return TokenType::String;
    if (v == "SYSTEM") return TokenType::System;
    if (v == "TABLE") return TokenType::Table;
    if (v == "TEXT") return TokenType::Text;
    if (v == "TIME") return TokenType::Time;
    if (v == "TRANSACTION") return TokenType::Transaction;
    if (v == "TRUE") return TokenType::True;
    if (v == "UNIQUE") return TokenType::Unique;
    if (v == "UPDATE") return TokenType::Update;
    if (v == "VALUES") return TokenType::Values;
    if (v == "VARCHAR") return TokenType::Varchar;
    if (v == "WHERE") return TokenType::Where;
    if (v == "WRITE") return TokenType::Write;

    return TokenType::IdentifierValue;
}

Lexer::Lexer(std::string input) : input{input}, iter{this->input.begin()} {}

Lexer::~Lexer() {}

void Lexer::ConsumeWhitespace() {
    NextWhile([](char c) { return c == ' '; });
}

std::optional<char> Lexer::NextIf(bool (*predicate)(char)) {
    char c = *iter;
    ++iter;
    if (predicate(c)) {
        return c;
    }
    return {};
}

std::optional<Token> Lexer::NextIfToken(Token (*tokenizer)(char)) {
    std::optional<Token> t = tokenizer(*iter);
    ++iter;
    return t;
}

std::optional<std::string> Lexer::NextWhile(bool (*predicate)(char)) {
    std::string value = "";

    while(predicate(*iter)) {
        value += *iter;
        ++iter;
    }

    if (value.size() == 0) {
        return {};
    } else {
        return value;
    }
}

std::optional<Result<Token, Error>> Lexer::Scan() {
    ConsumeWhitespace();

    if (iter == input.end()) {
        return {};
    }

    if (*iter == '\'') {
        return ScanString();
    } else if (*iter == '"') {
        return ScanIdentifierQuoted();
    } else if (isdigit(*iter)) {
        return ScanNumber();
    } else if (isalpha(*iter)) {
        return ScanIdentifier();
    } else {
        return ScanSymbol();
    }
}

Result<Token, Error> Lexer::ScanIdentifier() {
    std::optional<char> c = NextIf([](char c) { return isalpha(c) != 0; });
    std::string name = "";
    
    if (c.has_value()) {
        name += c.value();
    }
    
    name += NextWhile([](char c) { return isalnum(c) != 0; }).value_or("");
    
    return Ok(Token{ValueToTokenType(name), name});
}

Result<Token, Error> Lexer::ScanIdentifierQuoted() {
    if (!NextIf([](char c) { return c == '"'; }).has_value()) {
        return Err(Error{ErrorType::Lex, "Invalid quoted identifier."});
    }
    std::string identifier = "";

    while (iter != input.end()) {
        char c = NextIf([](char) { return true; }).value();
        if (c == '"') {
            break;
        }
        identifier += c;
    }

    return Ok(Token{TokenType::IdentifierValue, identifier});
}

Result<Token, Error> Lexer::ScanNumber() {
    std::string num = NextWhile([](char c) { return isdigit(c) != 0; }).value();

    return Ok(Token{TokenType::NumberValue, num});
}

Result<Token, Error> Lexer::ScanString() {
    if (!NextIf([](char c) { return c == '\''; }).has_value()) {
        return Err(Error{ErrorType::Lex, "Invalid string."});
    }
    std::string s = "";

    while (iter != input.end()) {
        char c = NextIf([](char) { return true; }).value();
        if (c == '\'') {
            return Ok(Token{TokenType::StringValue, s});
        }
        s += c;
    }

    return Ok(Token{TokenType::StringValue, s});
}

Result<Token, Error> Lexer::ScanSymbol() {
    char c = NextIf([](char) { return true; }).value();

    switch (c) {
        case '.':
            return Ok(Token{TokenType::Period, std::string{c}});
        case '=':
            return Ok(Token{TokenType::Equal, std::string{c}});
        case '>':
            return Ok(Token{TokenType::GreaterThan, std::string{c}});
        case '<':
            return Ok(Token{TokenType::LessThan, std::string{c}});
        case '+':
            return Ok(Token{TokenType::Plus, std::string{c}});
        case '-':
            return Ok(Token{TokenType::Minus, std::string{c}});
        case '*':
            return Ok(Token{TokenType::Asterisk, std::string{c}});
        case '/':
            return Ok(Token{TokenType::Slash, std::string{c}});
        case '^':
            return Ok(Token{TokenType::Caret, std::string{c}});
        case '%':
            return Ok(Token{TokenType::Percent, std::string{c}});
        case '!':
            return Ok(Token{TokenType::Exclamation, std::string{c}});
        case '?':
            return Ok(Token{TokenType::Question, std::string{c}});
        case '(':
            return Ok(Token{TokenType::OpenParen, std::string{c}});
        case ')':
            return Ok(Token{TokenType::CloseParen, std::string{c}});
        case ',':
            return Ok(Token{TokenType::Comma, std::string{c}});
        case ';':
            return Ok(Token{TokenType::Semicolon, std::string{c}});
        default:
            return Err(Error{ErrorType::Lex, "Invalid symbol."});
    }
}

LexerIterator::LexerIterator(std::string input) : l{Lexer{input}} {
    peeked = l.Scan().value().unwrap();
}

Result<Token, Error> LexerIterator::Peek() {
    if (peeked.has_value()) {
        return Ok(peeked.value());
    }
    return Err(Error{ErrorType::Lex, "Unexpected end of input."});
}

Result<Token, Error> LexerIterator::Next() {
    std::optional<Token> temp = peeked;

    if (!peeked.has_value()) {
        return Err(Error{ErrorType::Lex, "Unexpected end of input."});
    }

    std::optional<Result<Token, Error>> p = l.Scan();

    if (p.has_value() && p.value().isOk()) {
        peeked = p.value().unwrap();
    } else {
        peeked = std::nullopt;
    }

    return Ok(temp.value());
}
