#include "llvm/Constraints/BackendClasses.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"

BackendSameBlock::BackendSameBlock(const FunctionWrap& w) : wrap(w), blocks{{nullptr,nullptr}} { }

template<bool idx>
SkipResult BackendSameBlock::skip_invalid(unsigned& c)
{
    if(auto* instr = wrap.get_instruction(c))
    {
        if(std::get<1-idx>(blocks) == nullptr ||
           std::get<1-idx>(blocks) == instr->getParent())
        {
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

template SkipResult BackendSameBlock::skip_invalid<0>(unsigned&);
template SkipResult BackendSameBlock::skip_invalid<1>(unsigned&);
