#include "expression.h"

Expression::~Expression() {}

Field::Field(std::string from, std::string field) : from{from}, field{field} {}
Field::~Field() {}

Literal::Literal(LiteralType type) : type{type} {}
Literal::~Literal() {}

StringLiteral::StringLiteral(std::string value) : Literal{LiteralType::String}, value{value} {}
StringLiteral::~StringLiteral() {}

FloatLiteral::FloatLiteral(float value) : Literal{LiteralType::Float}, value{value} {}
FloatLiteral::~FloatLiteral() {}

IntegerLiteral::IntegerLiteral(int value) : Literal{LiteralType::Integer}, value{value} {}
IntegerLiteral::~IntegerLiteral() {}

BooleanLiteral::BooleanLiteral(bool value) : Literal{LiteralType::Boolean}, value{value} {}
BooleanLiteral::~BooleanLiteral() {}

NullLiteral::NullLiteral() : Literal{LiteralType::Null} {}
NullLiteral::~NullLiteral() {}

Operation::Operation(OperationType type) : type{type} {}
Operation::~Operation() {}

Prefix::Prefix(OperationType t, Expression * e) : Operation{t}, e{e} {}

Postfix::Postfix(OperationType t, Expression * e) : Operation{t}, e{e} {}

Infix::Infix(OperationType t, Expression * left, Expression * right) : Operation{t}, left{left}, right{right} {}
