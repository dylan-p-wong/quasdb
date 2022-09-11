#pragma once

enum class DataType { Boolean, Integer, Float, Varchar, Null };

class AbstractData {
public:
    DataType type;
    AbstractData(DataType type) : type{type} {}
    virtual ~AbstractData() = 0;
};

template <typename T>
class Data : public AbstractData {
public:
    Data(DataType type, T value) : AbstractData{type}, value{value} {}
    T value;
};
