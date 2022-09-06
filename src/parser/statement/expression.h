#pragma once

#include <string>

#include "../lexer.h"

enum class ExpressionType {
    // Postfix
    IsNull, 
    Factorial,
    // Prefix
    Minus,
    Not,
    Plus,
    // Field
    Field,
    // Literals 
    NullLiteral, 
    BooleanLiteral, 
    IntegerLiteral, 
    FloatLiteral, 
    StringLiteral,
    // Logical Infix
    And, 
    Or,
    // Math Infix
    Add,
    Subtract,
    Multiply,
    Divide,
    Exponentiate,
    GreaterThan,
    LessThan,
    Modulo,
    // Equality
    Equal,
    NotEqual
};

class Expression {
public:
    ExpressionType type;
    virtual std::string Display() = 0;
    Expression(ExpressionType type);
    virtual ~Expression() = 0;
};

class Field : public Expression {
public:
    std::string from;
    std::string field;
    Field(std::string from, std::string field);
    ~Field();
    std::string Display() override;
};

class StringLiteral : public Expression {
public:
    std::string value;
    StringLiteral(std::string value);
    ~StringLiteral();
    std::string Display() override;
};

class FloatLiteral : public Expression {
public:
    float value;
    FloatLiteral(float value);
    ~FloatLiteral();
    std::string Display() override;
};

class IntegerLiteral : public Expression {
public:
    int value;
    IntegerLiteral(int value);
    ~IntegerLiteral();
    std::string Display() override;
};

class BooleanLiteral : public Expression {
public:
    bool value;
    BooleanLiteral(bool value);
    ~BooleanLiteral();
    std::string Display() override;
};

class NullLiteral : public Expression {
public:
    NullLiteral();
    ~NullLiteral();
    std::string Display() override;
};

class PrefixOperation : public Expression {
public:
    Expression * e;
    PrefixOperation(ExpressionType t, Expression * e);
    std::string Display() override;
};

class PostfixOperation : public Expression {
public:
    Expression * e;
    PostfixOperation(ExpressionType t, Expression * e);
    std::string Display() override;
};

class IsNullOperation : public Expression {
public:
    bool nullable;
    Expression * e;
    IsNullOperation(bool nullable, Expression * e);
    std::string Display() override;
};

class InfixOperation : public Expression {
public:
    Expression * left;
    Expression * right;
    InfixOperation(ExpressionType t, Expression * left, Expression * right);
    std::string Display() override;
};
