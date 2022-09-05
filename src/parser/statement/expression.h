#pragma once

#include <string>

#include "../lexer.h"

enum class LiteralType { Null, Boolean, Integer, Float, String };

class Expression {
public:
    virtual ~Expression() = 0;
};

class Field : public Expression {
public:
    std::string from;
    std::string field;
    Field(std::string from, std::string field);
    ~Field();
};

class Literal : public Expression {
public:
    LiteralType type;
    Literal(LiteralType type);
    virtual ~Literal() = 0;
};

class StringLiteral : public Literal {
public:
    std::string value;
    StringLiteral(std::string value);
    ~StringLiteral();
};

class FloatLiteral : public Literal {
public:
    float value;
    FloatLiteral(float value);
    ~FloatLiteral();
};

class IntegerLiteral : public Literal {
public:
    int value;
    IntegerLiteral(int value);
    ~IntegerLiteral();
};

class BooleanLiteral : public Literal {
public:
    bool value;
    BooleanLiteral(bool value);
    ~BooleanLiteral();
};

class NullLiteral : public Literal {
public:
    NullLiteral();
    ~NullLiteral();
};

enum class OperationType { Or, And };

class Operation : public Expression {
    OperationType type;
public:
    Operation(OperationType type);
    virtual ~Operation() = 0;
};

class Prefix : public Operation {
public:
    Expression * e;
    Prefix(OperationType t, Expression * e);
};

class Postfix : public Operation {
public:
    Expression * e;
    Postfix(OperationType t, Expression * e);
};

class Infix : public Operation {
public:
    Expression * left;
    Expression * right;
    Infix(OperationType t, Expression * left, Expression * right);
};
