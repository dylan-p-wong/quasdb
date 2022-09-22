#include "select.h"

FromItem::FromItem(FromType type) : type{type} {}
FromItem::~FromItem() {}

TableFromItem::TableFromItem(std::string name, std::string alias) : FromItem{FromType::Table}, name{name}, alias{alias} {}

JoinFromItem::JoinFromItem(JoinType join_type, FromItem * left, FromItem * right, Expression * on) : FromItem{FromType::Join}, join_type{join_type}, left{left}, right{right}, predicate{on} {}

OrderItem::OrderItem(OrderType direction, Expression * order) : direction{direction}, order{order} {}

SelectStatement::SelectStatement() : Statement{StatementType::Select} {}
