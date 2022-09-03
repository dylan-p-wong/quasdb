#pragma once

#include <string>

enum class LiteralType { Null, Boolean, Integer, Float, String };

class Expression {
public:
    virtual ~Expression() = 0;
};

class Field : public Expression {
    std::string from;
    std::string field;
};

template <typename T>
class Literal : public Expression {
    LiteralType type;
    T value;
};

class Operation : public Expression {
public:
    virtual ~Operation() = 0;
};

// Logical
class And : public Operation {
    Expression * left;
    Expression * right;
};
class Not : public Operation {
    Expression * e;
};
class Or : public Operation {
    Expression * left;
    Expression * right;
};

// Comparison

// Mathematical

// String
