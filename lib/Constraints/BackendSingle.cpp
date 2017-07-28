#include "llvm/Constraints/BackendClasses.hpp"

BackendSingle::BackendSingle(std::vector<SolverAtom::Value> h) : hits(std::move(h)), hit_start(hits.begin()) { }

SkipResult BackendSingle::skip_invalid(SolverAtom::Value& c) 
{
    for(; hit_start != hits.end(); hit_start++)
    {
        if(*hit_start > c)
        {
            c = *hit_start;
            return SkipResult::CHANGEPASS;
        }

        if(*hit_start == c)
            return SkipResult::PASS;
    }

    return SkipResult::FAIL;
}
