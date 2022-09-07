#include "expression.h"

Expression::Expression(ExpressionType type) : type{type} {}
Expression::~Expression() {}

Field::Field(std::string from, std::string field) : Expression{ExpressionType::Field}, from{from}, field{field} {}
Field::~Field() {}
std::string Field::Display() {
    return from.size() > 0 ? from + "." + field : field;
}

StringLiteral::StringLiteral(std::string value) : Expression{ExpressionType::StringLiteral}, value{value} {}
StringLiteral::~StringLiteral() {}
std::string StringLiteral::Display() {
    return "'" + value + "'";
}

FloatLiteral::FloatLiteral(float value) : Expression{ExpressionType::FloatLiteral}, value{value} {}
FloatLiteral::~FloatLiteral() {}
std::string FloatLiteral::Display() {
    return std::to_string(value);
}

IntegerLiteral::IntegerLiteral(int value) : Expression{ExpressionType::IntegerLiteral}, value{value} {}
IntegerLiteral::~IntegerLiteral() {}
std::string IntegerLiteral::Display() {
    return std::to_string(value);
}

BooleanLiteral::BooleanLiteral(bool value) : Expression{ExpressionType::BooleanLiteral}, value{value} {}
BooleanLiteral::~BooleanLiteral() {}
std::string BooleanLiteral::Display() {
    return value == true ? "TRUE" : "FALSE";
}

NullLiteral::NullLiteral() : Expression{ExpressionType::NullLiteral} {}
NullLiteral::~NullLiteral() {}
std::string NullLiteral::Display() {
    return "NULL";
}

PrefixOperation::PrefixOperation(ExpressionType t, Expression * e) : Expression{t}, e{e} {}
std::string PrefixOperation::Display() {
    if (type == ExpressionType::Assert) {
        return "ASSERT(" + e->Display() + ")"; 
    } else if (type == ExpressionType::Negate) {
        return "NEGATE(" + e->Display() + ")"; 
    } else if (type == ExpressionType::Not) {
        return "NOT(" + e->Display() + ")"; 
    } else {
        return "ERROR(" + e->Display() + ")";
    }
}

PostfixOperation::PostfixOperation(ExpressionType t, Expression * e) : Expression{t}, e{e} {}
std::string PostfixOperation::Display() {
    if (type == ExpressionType::Factorial) {
        return "(" + e->Display() + ")!"; 
    } else {
        return "(" + e->Display() + "ERROR)";
    }
}

IsNullOperation::IsNullOperation(bool nullable, Expression * e) : Expression{ExpressionType::IsNull}, nullable{nullable}, e{e} {}
std::string IsNullOperation::Display() {
    return "(" + e->Display() + " IS " + (nullable ? "" : "NOT ") + "NULL)";
}

InfixOperation::InfixOperation(ExpressionType t, Expression * l, Expression * r) : Expression{t}, left{l}, right{r} {}
std::string InfixOperation::Display() {
    if (type == ExpressionType::And) {
        return "(" + left->Display() + " AND " + right->Display() + ")"; 
    } else if (type == ExpressionType::Or) {
        return "(" + left->Display() + " OR " + right->Display() + ")"; 
    } else if (type == ExpressionType::Add) {
        return "(" + left->Display() + " + " + right->Display() + ")"; 
    } else if (type == ExpressionType::Subtract) {
        return "(" + left->Display() + " - " + right->Display() + ")"; 
    } else if (type == ExpressionType::Multiply) {
        return "(" + left->Display() + " * " + right->Display() + ")"; 
    } else if (type == ExpressionType::Divide) {
        return "(" + left->Display() + " / " + right->Display() + ")"; 
    } else if (type == ExpressionType::Exponentiate) {
        return "(" + left->Display() + " ^ " + right->Display() + ")"; 
    } else if (type == ExpressionType::GreaterThan) {
        return "(" + left->Display() + " > " + right->Display() + ")"; 
    } else if (type == ExpressionType::LessThan) {
        return "(" + left->Display() + " < " + right->Display() + ")"; 
    } else if (type == ExpressionType::Modulo) {
        return "(" + left->Display() + " % " + right->Display() + ")"; 
    } else if (type == ExpressionType::Equal) {
        return "(" + left->Display() + " = " + right->Display() + ")"; 
    } else if (type == ExpressionType::NotEqual) {
        return "(" + left->Display() + " != " + right->Display() + ")"; 
    } else {
        return "(" + left->Display() + " ERROR " + right->Display() + ")"; 
    }
}
