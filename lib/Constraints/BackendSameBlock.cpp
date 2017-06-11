#include "llvm/Constraints/Backends.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include <tuple>

BackendSameBlock_::BackendSameBlock_(const FunctionWrapper& w)
                 : wrap(w), blocks{nullptr,nullptr} { }

template<bool idx>
SkipResult BackendSameBlock_::skip_invalid(unsigned& c)
{
    if(auto* instr = wrap.get_instruction(c))
    {
        if(std::get<(idx+1)%2>(blocks) == nullptr ||
           std::get<(idx+1)%2>(blocks) == instr->getParent())
        {
            std::get<idx>(blocks) = instr->getParent();
            return SkipResult::PASS;
        }
    }

    if(c < wrap.get_size() - 1)
    {
        c = c+1;
        return SkipResult::CHANGE;
    }
    else return SkipResult::FAIL;
}

template SkipResult BackendSameBlock_::skip_invalid<0>(unsigned&);
template SkipResult BackendSameBlock_::skip_invalid<1>(unsigned&);
