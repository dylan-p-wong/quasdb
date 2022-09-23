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

std::unique_ptr<AbstractData> Expression::EvaluateConstant() const {
    Scope s{0};
    std::vector<AbstractData*> v;
    return Evaluate(s, v);
}

std::unique_ptr<AbstractData> Expression::Evaluate(const Scope & scope, std::vector<AbstractData*> row) const {
    if (dynamic_cast<const InfixOperation*>(this)) {
        const InfixOperation * e = dynamic_cast<const InfixOperation*>(this);

        auto left_p = e->left->Evaluate(scope, row);
        auto right_p = e->right->Evaluate(scope, row);

        AbstractData * left_data = left_p.get();
        AbstractData * right_data = right_p.get();

        if (type == ExpressionType::And) {
            return std::make_unique<Data<bool>>(DataType::Boolean, left_data->IsTruthy() && right_data->IsTruthy());   
        } else if (type == ExpressionType::Or) {
            return std::make_unique<Data<bool>>(DataType::Boolean, left_data->IsTruthy() || right_data->IsTruthy());   
        } else if (type == ExpressionType::Add) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<int>>(DataType::Integer, left_int_data->value + right_int_data->value);
        } else if (type == ExpressionType::Subtract) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<int>>(DataType::Integer, left_int_data->value - right_int_data->value);
        } else if (type == ExpressionType::Multiply) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<int>>(DataType::Integer, left_int_data->value * right_int_data->value);
        } else if (type == ExpressionType::Divide) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            // Handle divide by 0
            if (right_int_data->value == 0) {
                throw Error{ErrorType::Internal, ""};
            }

            return std::make_unique<Data<int>>(DataType::Integer, left_int_data->value / right_int_data->value);
        } else if (type == ExpressionType::Exponentiate) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<int>>(DataType::Integer, (int) pow((float) left_int_data->value, (float) right_int_data->value));
        } else if (type == ExpressionType::GreaterThan) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<bool>>(DataType::Boolean, left_int_data->value > right_int_data->value);
        } else if (type == ExpressionType::LessThan) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<bool>>(DataType::Boolean, left_int_data->value < right_int_data->value);
        } else if (type == ExpressionType::Modulo) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<int>>(DataType::Integer, left_int_data->value % right_int_data->value);
        } else if (type == ExpressionType::Equal) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<bool>>(DataType::Boolean, left_int_data->value == right_int_data->value);
        } else if (type == ExpressionType::NotEqual) {
            Data<int> * left_int_data = dynamic_cast<Data<int>*>(left_data);
            Data<int> * right_int_data = dynamic_cast<Data<int>*>(right_data);

            if (left_int_data == nullptr || right_int_data == nullptr) {
                return nullptr;
            }

            return std::make_unique<Data<bool>>(DataType::Boolean, left_int_data->value != right_int_data->value);
        } else {
            throw Error{ErrorType::Internal, ""};
        }
    } else if (dynamic_cast<const PrefixOperation*>(this)) {
        const PrefixOperation * op = dynamic_cast<const PrefixOperation*>(this);
        auto p = op->Evaluate(scope, row);

        if (type == ExpressionType::Assert) {
            Data<int> * data = dynamic_cast<Data<int>*>(p.get());

            if (data == nullptr) {
                throw Error{ErrorType::Internal, ""};
            }

            return std::make_unique<Data<bool>>(DataType::Integer, data->value);
        } else if (type == ExpressionType::Negate) {
            Data<int> * data = dynamic_cast<Data<int>*>(p.get());

            if (data == nullptr) {
                throw Error{ErrorType::Internal, ""};
            }

            return std::make_unique<Data<bool>>(DataType::Integer, data->value * -1);
        } else if (type == ExpressionType::Not) {
            return std::make_unique<Data<bool>>(DataType::Boolean, !p->IsTruthy());
        } else {
            throw Error{ErrorType::Internal, ""};
        }
    } else if (dynamic_cast<const PostfixOperation*>(this)) {
        throw Error{ErrorType::Internal, ""};
    } else if (dynamic_cast<const IsNullOperation*>(this)) {
        const IsNullOperation * op = dynamic_cast<const IsNullOperation*>(this);
        auto p = op->Evaluate(scope, row);

        if (p->type == DataType::Null) {
            return std::make_unique<Data<bool>>(DataType::Boolean, op->nullable);
        } else {
            return std::make_unique<Data<bool>>(DataType::Boolean, !op->nullable);
        }
    } else if (dynamic_cast<const StringLiteral*>(this)) {
        return std::make_unique<Data<std::string>>(DataType::Varchar, dynamic_cast<const StringLiteral*>(this)->value);
    } else if (dynamic_cast<const FloatLiteral*>(this)) {
        return std::make_unique<Data<float>>(DataType::Float, dynamic_cast<const FloatLiteral*>(this)->value);
    } else if (dynamic_cast<const IntegerLiteral*>(this)) {
        return std::make_unique<Data<int>>(DataType::Integer, dynamic_cast<const IntegerLiteral*>(this)->value);
    } else if (dynamic_cast<const BooleanLiteral*>(this)) {
        return std::make_unique<Data<bool>>(DataType::Boolean, dynamic_cast<const BooleanLiteral*>(this)->value);
    } else if (dynamic_cast<const NullLiteral*>(this)) {
        return std::make_unique<Data<int>>(DataType::Null, 0);
    } else if (dynamic_cast<const Field*>(this)) {
        std::string from = dynamic_cast<const Field*>(this)->from;
        std::string field = dynamic_cast<const Field*>(this)->field;
        
        if (scope.GetFieldIndex(from + "." + field) < 0) {
            throw Error{ErrorType::Internal, ""};
        }
        
        // temp fix
        AbstractData * d = row.at(scope.GetFieldIndex(from + "." + field));
        if (d->type == DataType::Integer) {
            return std::make_unique<Data<int>>(DataType::Integer, dynamic_cast<Data<int>*>(d)->value); 
        } else if (d->type == DataType::Boolean) {
            return std::make_unique<Data<bool>>(DataType::Boolean, dynamic_cast<Data<bool>*>(d)->value); 
        } else {
            throw Error{ErrorType::Internal, "Unsupported type."};
        }
    } else {
        throw Error{ErrorType::Internal, ""};
    }
}
