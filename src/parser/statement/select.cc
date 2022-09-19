#include "select.h"

FromItem::FromItem(FromType type) : type{type} {}
FromItem::~FromItem() {}

TableFromItem::TableFromItem(std::string name, std::string alias) : FromItem{FromType::Table}, name{name}, alias{alias} {}

OrderItem::OrderItem(OrderType direction, Expression * order) : direction{direction}, order{order} {}

SelectStatement::SelectStatement() : Statement{StatementType::Select} {}
