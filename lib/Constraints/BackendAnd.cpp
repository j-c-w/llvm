#include "llvm/Constraints/Backends.hpp"

BackendAnd_::BackendAnd_(std::vector<SpecializedContainer> c)
           : constraints(std::move(c))
{ }

template<unsigned idx>
SkipResult BackendAnd_::skip_invalid(Specialized::Value& c)
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
void BackendAnd_::begin()
{
    for(auto& constraint : constraints)
    {
        constraint->begin();
    }
}

template<unsigned idx>
void BackendAnd_::fixate(Specialized::Value c)
{
    for(auto& constraint : constraints)
    {
        constraint->fixate(c);
    }
}

template<unsigned idx>
void BackendAnd_::resume(Specialized::Value c)
{
    for(auto& constraint : constraints)
    {
        constraint->resume(c);
    }
}

template<unsigned idx>
void BackendAnd_::cancel()
{
    for(auto& constraint : constraints)
    {
        constraint->cancel();
    }
}

template SkipResult BackendAnd_::skip_invalid<0>(Specialized::Value&);
template       void BackendAnd_::begin<0>();
template       void BackendAnd_::fixate<0>(Specialized::Value);
template       void BackendAnd_::resume<0>(Specialized::Value);
template       void BackendAnd_::cancel<0>();
