#pragma once

#include <vector>
#include <string>

#include "expression.h"
#include "statement.h"

enum class FromType { Join, Table };
enum class JoinType { Outer, Inner, Left, Right };
enum class OrderType { ASC, DESC };

class FromItem {
public:
    FromType type;
    FromItem(FromType type);
    virtual ~FromItem() = 0;
};

class TableFromItem : public FromItem {
public:
    std::string name;
    std::string alias;
    TableFromItem(std::string name, std::string alias);
};

class JoinFromItem : public FromItem {
public:
    FromItem * left;
    FromItem * right;
    JoinType join_type;
    Expression * predicate;
    JoinFromItem(JoinType type, FromItem * left, FromItem * right, Expression * on);
};

class SelectItem {
public:
    Expression * field;
    std::string alias;
};

class OrderItem {
public:
    OrderType direction = OrderType::ASC;
    Expression * order;
    OrderItem(OrderType direction, Expression * order);
};

class SelectStatement : public Statement {
public:
    std::vector<SelectItem*> select;
    std::vector<FromItem*> from;
    Expression * where = nullptr;
    std::vector<Expression*> group_by;
    Expression * having = nullptr;
    std::vector<OrderItem*> order_by;
    Expression * offset = nullptr;
    Expression * limit = nullptr;
    SelectStatement();
};
