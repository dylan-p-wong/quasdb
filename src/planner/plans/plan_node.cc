#include "plan_node.h"

PlanNode::PlanNode(PlanType type, Catalog * catalog) : type{type}, catalog{catalog} {}
PlanNode::~PlanNode() {}
