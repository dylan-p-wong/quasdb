#include <algorithm>

#include "order_executor.h"
#include "executor_factory.h"

OrderExecutor::OrderExecutor(const OrderPlan * plan) : plan{plan} {}

std::vector<std::vector<AbstractData*>> OrderExecutor::Execute(Catalog * catalog) {
    std::vector<std::vector<AbstractData*>> rows = ExecutorFactory::CreateExecutor(plan->source).get()->Execute(catalog);

    std::sort(rows.begin(), rows.end(), [this](const std::vector<AbstractData*> lhs, const std::vector<AbstractData*> rhs) { // BUG: This cannot be references for some reason

        for (int i = 0; i < plan->order_by.size(); i++) {
            OrderItem * order_item = plan->order_by.at(i);
            Expression * e = order_item->order;

            assert(lhs.size() == rhs.size());

            std::unique_ptr<AbstractData> l = e->Evaluate(plan->GetScope(), lhs);
            std::unique_ptr<AbstractData> r = e->Evaluate(plan->GetScope(), rhs);

            if (*l.get() == r.get()) {
                continue;
            }

            return order_item->direction == OrderType::DESC ? *l.get() > r.get() : *l.get() < r.get();
        }
    });

    return rows;
}
