#include "../parser/parser.h"

#include "quas.h"

ExecutionOutput QuasDB::Execute(std::string query) {
    Parser parser{query};
    try {
        auto statement_res = parser.ParseStatement();

        if (statement_res.isErr()) {
            ExecutionOutput res;
            res.error = true;
            res.error_message = statement_res.unwrapErr().message;
            return res;
        }
        auto plan = planner->CreatePlan(statement_res.unwrap());

        if (plan == nullptr) {
            ExecutionOutput res;
            res.error = true;
            res.error_message = "Planner error.";
            return res;
        }

        ExecutionOutput res = executor->Execute(plan.get());
        return res;
    } catch (Error & e) {
        ExecutionOutput res;
        res.error = true;
        res.error_message = e.message;
        return res;
    }
}
