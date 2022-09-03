#pragma once

#include <algorithm>
#include <string>
#include <optional>
#include <unordered_map>

#include "../common/result.h"
#include "../common/error.h"

enum TokenType {
    NumberValue,
    StringValue,
    IdentifierValue,
    // Symbols
    Period,
    Equal,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,
    LessOrGreaterThan,
    Plus,
    Minus,
    Asterisk,
    Slash,
    Caret,
    Percent,
    Exclamation,
    NotEqual,
    Question,
    OpenParen,
    CloseParen,
    Comma,
    Semicolon,
    // Keywords
    And,
    As,
    Asc,
    Begin,
    Bool,
    Boolean,
    By,
    Char,
    Commit,
    Create,
    Cross,
    Default,
    Delete,
    Desc,
    Double,
    Drop,
    Explain,
    False,
    Float,
    From,
    Group,
    Having,
    Index,
    Infinity,
    Inner,
    Insert,
    Int,
    Integer,
    Into,
    Is,
    Join,
    Key,
    Left,
    Like,
    Limit,
    NaN,
    Not,
    Null,
    Of,
    Offset,
    On,
    Only,
    Or,
    Order,
    Outer,
    Primary,
    Read,
    References,
    Right,
    Rollback,
    Select,
    Set,
    String,
    System,
    Table,
    Text,
    Time,
    Transaction,
    True,
    Unique,
    Update,
    Values,
    Varchar,
    Where,
    Write,
    // Extra
    Invalid
};

struct Token {
    TokenType type;
    std::string value;
    Token(TokenType type, std::string value);
};

class Lexer {
    std::string input;
    std::string::iterator iter;
public:
    Lexer(std::string input);
    ~Lexer();
    void ConsumeWhitespace();
    std::optional<char> NextIf(bool (*predicate)(char));
    std::optional<Token> NextIfToken(Token (*tokenizer)(char));
    std::optional<std::string> NextWhile(bool (*predicate)(char));
    std::optional<Result<Token, Error>> Scan();
    Result<Token, Error> ScanIdentifier();
    Result<Token, Error> ScanIdentifierQuoted();
    Result<Token, Error> ScanNumber();
    Result<Token, Error> ScanString();
    Result<Token, Error> ScanSymbol();
};

class LexerIterator {
    std::optional<Token> peeked;
    Lexer l;
public:
    LexerIterator(std::string input);
    Result<Token, Error> Peek();
    Result<Token, Error> Next();
};
