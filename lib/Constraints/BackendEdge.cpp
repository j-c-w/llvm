#include "llvm/Constraints/BackendClasses.hpp"

BackendEdge::BackendEdge(const Graph& gf, const Graph& gb)
           : graphs{{gf,gb}}, amount_completed(0), src_ptr(nullptr), dst_ptr(nullptr) { }

template<unsigned idx>
SkipResult BackendEdge::skip_invalid(unsigned& c) const
{
    if(amount_completed == 0)
    {
        if(c < std::get<idx>(graphs).get().size())
            return SkipResult::PASS;

        return SkipResult::FAIL;
    }

    for(auto ptr = dst_ptr; ptr != src_ptr->end(); ptr++)
    {
        if(*ptr > c)
        {
            c = *ptr;
            return SkipResult::CHANGEPASS;
        }

        if(*ptr == c)
            return SkipResult::PASS;
    }

    return SkipResult::FAIL;
}

template SkipResult BackendEdge::skip_invalid<0>(unsigned&) const;
template SkipResult BackendEdge::skip_invalid<1>(unsigned&) const;
