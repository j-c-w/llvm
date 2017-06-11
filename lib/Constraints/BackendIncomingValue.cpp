#include "llvm/Constraints/Backends.hpp"

BackendIncomingValue_::BackendIncomingValue_(const FunctionWrapper& w)
                     : wrap(w), values(nullptr,nullptr,nullptr)
{ }

template<unsigned idx>
SkipResult BackendIncomingValue_::skip_invalid(unsigned& c)
{
    auto& in_value  = std::get<0>(values);
    auto& term_inst = std::get<1>(values);
    auto& phi_node  = std::get<2>(values);

    if(auto cast_val = llvm::dyn_cast<typename std::decay<decltype(*std::get<idx>(values))>::type>(wrap.get_value(c)))
    {
        std::get<idx>(values) = cast_val;

        if(phi_node == nullptr || (term_inst == nullptr && in_value  == nullptr))
        {
            return SkipResult::PASS; 
        }
        else
        {
            for(unsigned i = 0; i < phi_node->getNumIncomingValues(); i++)
            {
                if((in_value  == nullptr  && phi_node->getIncomingBlock(i) == term_inst->getParent()) ||
                   (term_inst == nullptr  && phi_node->getIncomingValue(i) == in_value) ||
                   (in_value && term_inst && phi_node->getIncomingBlock(i) == term_inst->getParent()
                                          && phi_node->getIncomingValue(i) == in_value))
                {
                    return SkipResult::PASS;
                }
            }
        }
    }

    if(c < wrap.get_size() - 1)
    {
        c = c+1;
        return SkipResult::CHANGE;
    }
    else return SkipResult::FAIL;
}

template SkipResult BackendIncomingValue_::skip_invalid<0>(unsigned&);
template SkipResult BackendIncomingValue_::skip_invalid<1>(unsigned&);
template SkipResult BackendIncomingValue_::skip_invalid<2>(unsigned&);
