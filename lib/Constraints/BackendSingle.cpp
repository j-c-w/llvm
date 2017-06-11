#include "llvm/Constraints/Backends.hpp"
#include <vector>

BackendSingle_::BackendSingle_(std::vector<Specialized::Value> h)
              : hits(std::move(h))
{ }

template<unsigned idx>
SkipResult BackendSingle_::skip_invalid(Specialized::Value& c) 
{
    for(; hit_start != hits.end(); hit_start++)
    {
        if(*hit_start > c)
        {
            c = *hit_start;
            return SkipResult::CHANGEPASS;
        }
        else if(*hit_start == c)
        {
            return SkipResult::PASS;
        }
    }

    return SkipResult::FAIL;
}

template SkipResult BackendSingle_::skip_invalid<0>(Specialized::Value&);
