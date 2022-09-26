#pragma once

enum class DataType { Boolean, Integer, Float, Varchar, Null };

class AbstractData {
public:
    DataType type;
    std::unique_ptr<AbstractData> Copy() const;
    AbstractData(DataType type) : type{type} {}
    bool IsTruthy();
    virtual ~AbstractData() = 0;
};

template <typename T>
class Data : public AbstractData {
public:
    Data(DataType type, T value) : AbstractData{type}, value{value} {}
    T value;
};
