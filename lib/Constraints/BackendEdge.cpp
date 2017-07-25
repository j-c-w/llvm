#include "llvm/Constraints/BackendClasses.hpp"

BackendEdge::BackendEdge(const Graph& gf, const Graph& gb) : graphs{{gf,gb}}, src_ptr(nullptr), dst_ptr(nullptr) { }

template<unsigned idx>
SkipResult BackendEdge::skip_invalid(unsigned& c)
{
    if(src_ptr == nullptr)
    {
        if(c < std::get<idx>(graphs).get().size())
        {
            return SkipResult::PASS;
        }
        else
        {
            return SkipResult::FAIL;
        }
    }
    else
    {
        for(; dst_ptr != &*src_ptr->end(); dst_ptr++)
        {
            if(*dst_ptr > c)
            {
                c = *dst_ptr;
                return SkipResult::CHANGEPASS;
            }
            else if(*dst_ptr == c)
            {
                return SkipResult::PASS;
            }
        }

        return SkipResult::FAIL;
    }
}

template SkipResult BackendEdge::skip_invalid<0>(unsigned&);
template SkipResult BackendEdge::skip_invalid<1>(unsigned&);
