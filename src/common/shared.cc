#include "shared.h"

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
