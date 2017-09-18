#include "llvm/Constraints/BackendClasses.hpp"

BackendAnd::BackendAnd(std::vector<std::unique_ptr<SolverAtom>> c)
  : constraints_head(std::move(c.front())),
    constraints_tail(std::make_move_iterator(c.begin() + 1),
                     std::make_move_iterator(c.end())) { }

SkipResult BackendAnd::skip_invalid(SolverAtom::Value& c) const
{
    SkipResult result = constraints_head->skip_invalid(c);

    if(result == SkipResult::FAIL || result == SkipResult::CHANGE)
        return result;

    for(auto& constraint : constraints_tail)
    {
        SkipResult local_result = constraint->skip_invalid(c);

        if(local_result == SkipResult::FAIL)
            return SkipResult::FAIL;

        if(local_result != SkipResult::PASS)
            return SkipResult::CHANGE;
    }

    return result;
}

void BackendAnd::begin()
{
    constraints_head->begin();
    for(auto& constraint : constraints_tail)
        constraint->begin();
}

void BackendAnd::fixate(SolverAtom::Value c)
{
    constraints_head->fixate(c);
    for(auto& constraint : constraints_tail)
        constraint->fixate(c);
}

void BackendAnd::resume()
{
    constraints_head->resume();
    for(auto& constraint : constraints_tail)
        constraint->resume();
}
