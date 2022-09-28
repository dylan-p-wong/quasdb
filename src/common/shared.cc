#include <memory>
#include "shared.h"
#include "error.h"

bool AbstractData::IsTruthy() {
    if (type == DataType::Boolean) {
        return dynamic_cast<Data<bool>*>(this)->value;
    } else if (type == DataType::Integer) {
        return dynamic_cast<Data<int>*>(this)->value > 0;
    } else if (type == DataType::Float) {
        return dynamic_cast<Data<float>*>(this)->value > 0;
    } else if (type == DataType::Varchar) {
        return true;
    } else if (type == DataType::Null) {
        return false;
    }
    return false;
}

AbstractData::~AbstractData() {}

std::unique_ptr<AbstractData> AbstractData::Copy() const {
    if (type == DataType::Boolean) {
        auto t = dynamic_cast<const Data<bool>*>(this);
        return std::make_unique<Data<bool>>(DataType::Boolean, t->value);
    } else if (type == DataType::Integer) {
        auto t = dynamic_cast<const Data<int>*>(this);
        return std::make_unique<Data<int>>(DataType::Integer, t->value);
    } else if (type == DataType::Float) {
        auto t = dynamic_cast<const Data<float>*>(this);
        return std::make_unique<Data<float>>(DataType::Float, t->value);
    } else if (type == DataType::Varchar) {
        auto t = dynamic_cast<const Data<std::string>*>(this);
        return std::make_unique<Data<std::string>>(DataType::Varchar, t->value);
    } else if (type == DataType::Null) {
        return std::make_unique<Data<int>>(DataType::Null, 0);
    }
    throw Error{ErrorType::Internal, ""};
}

bool AbstractData::operator<(const AbstractData * other) const {
    if (type == DataType::Varchar && other->type == DataType::Varchar) {
        return dynamic_cast<const Data<std::string>*>(this)->value < dynamic_cast<const Data<std::string>*>(other)->value;
    }

    if (type == DataType::Integer && other->type == DataType::Integer) {
        return dynamic_cast<const Data<int>*>(this)->value < dynamic_cast<const Data<int>*>(other)->value;
    }

    if (type == DataType::Float && other->type == DataType::Float) {
        return dynamic_cast<const Data<float>*>(this)->value < dynamic_cast<const Data<float>*>(other)->value;
    }

    throw Error{ErrorType::Internal, ""};
}

bool AbstractData::operator>(const AbstractData * other) const {
    return *other < this;
}

bool AbstractData::operator==(const AbstractData * other) const {
    if (type == DataType::Varchar && other->type == DataType::Varchar) {
        return dynamic_cast<const Data<std::string>*>(this)->value == dynamic_cast<const Data<std::string>*>(other)->value;
    }

    if (type == DataType::Integer && other->type == DataType::Integer) {
        return dynamic_cast<const Data<int>*>(this)->value == dynamic_cast<const Data<int>*>(other)->value;
    }

    if (type == DataType::Float && other->type == DataType::Float) {
        return dynamic_cast<const Data<float>*>(this)->value == dynamic_cast<const Data<float>*>(other)->value;
    }

    throw Error{ErrorType::Internal, ""};
}

bool AbstractData::operator!=(const AbstractData * other) const {
    return !(*this == other);
}
