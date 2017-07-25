#include "llvm/Constraints/BackendClasses.hpp"

BackendAnd::BackendAnd(std::vector<SpecializedContainer> c)
           : constraints(std::move(c))
{ }

template<unsigned idx>
SkipResult BackendAnd::skip_invalid(Specialized::Value& c)
{
    for(auto& constraint : constraints)
    {
        SkipResult local_result = constraint->skip_invalid(c);

        if(local_result == SkipResult::FAIL)
        {
            return SkipResult::FAIL;
        }
        else if(local_result == SkipResult::CHANGE || local_result == SkipResult::CHANGEPASS)
        {
            return SkipResult::CHANGE;
        }
    }

    return SkipResult::PASS;
}

template<unsigned idx>
void BackendAnd::begin()
{
    for(auto& constraint : constraints)
    {
        constraint->begin();
    }
}

template<unsigned idx>
void BackendAnd::fixate(Specialized::Value c)
{
    for(auto& constraint : constraints)
    {
        constraint->fixate(c);
    }
}

template<unsigned idx>
void BackendAnd::resume(Specialized::Value c)
{
    for(auto& constraint : constraints)
    {
        constraint->resume(c);
    }
}

template<unsigned idx>
void BackendAnd::cancel()
{
    for(auto& constraint : constraints)
    {
        constraint->cancel();
    }
}

template SkipResult BackendAnd::skip_invalid<0>(Specialized::Value&);
template       void BackendAnd::begin<0>();
template       void BackendAnd::fixate<0>(Specialized::Value);
template       void BackendAnd::resume<0>(Specialized::Value);
template       void BackendAnd::cancel<0>();
