#include "llvm/Constraints/ConstraintSameBlock.hpp"
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"

template<typename LabelType>
ConstraintSameBlock<LabelType>::ConstraintSameBlock(const FunctionWrapper& w, LabelType v1, LabelType v2)
                         : wrap(w), variable1(v1), variable2(v2) { }

template<typename LabelType>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> ConstraintSameBlock<LabelType>::get_specializations() const
{
    std::shared_ptr<SharedData> cache(new SharedData{wrap, 0, 0, 0});

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> result;

    result.emplace_back(variable1, std::unique_ptr<Specialized<unsigned>>(new SpecializedSameBlock<true>(cache)));
    result.emplace_back(variable2, std::unique_ptr<Specialized<unsigned>>(new SpecializedSameBlock<false>(cache)));

    return result;
}

template<typename LabelType>
template<bool back>
ConstraintSameBlock<LabelType>::SpecializedSameBlock<back>::SpecializedSameBlock(std::shared_ptr<SharedData> c)
                                     : cache(c) { }

template<typename LabelType>
template<bool back>
SkipResult ConstraintSameBlock<LabelType>::SpecializedSameBlock<back>::skip_invalid(unsigned& c)
{
    if(cache->amount_completed == 0)
    {
        if(cache->wrap.get_instruction(c))
        {
            (back?cache->temp_value2:cache->temp_value1) = c;
            return SkipResult::PASS;
        }
        else if (c+1 < cache->wrap.get_size())
        {
            (back?cache->temp_value2:cache->temp_value1) = c+1;
            return SkipResult::CHANGE;
        }
        else return SkipResult::FAIL;
    }
    else
    {
        if(c < cache->wrap.get_size())
        {
            if(auto* instr1 = cache->wrap.get_instruction(back?cache->temp_value1:cache->temp_value2))
            {
                if(auto* instr2 = cache->wrap.get_instruction(c))
                {
                    if(instr1->getParent() == instr2->getParent())
                    {
                        (back?cache->temp_value2:cache->temp_value1) = c;
                        return SkipResult::PASS;
                    }
                    else if(c+1 < cache->wrap.get_size())
                    {
                        (back?cache->temp_value2:cache->temp_value1) = c = c+1;
                        return SkipResult::CHANGE;
                    }
                    else return SkipResult::FAIL;
                }
                else if(c+1 < cache->wrap.get_size())
                {
                    (back?cache->temp_value2:cache->temp_value1) = c = c+1;
                    return SkipResult::CHANGE;
                }
                else return SkipResult::FAIL;
            }
            else return SkipResult::FAIL;
        }
        else return SkipResult::FAIL;
    }
}

template<typename LabelType>
template<bool back>
void ConstraintSameBlock<LabelType>::SpecializedSameBlock<back>::begin() { }

template<typename LabelType>
template<bool back>
void ConstraintSameBlock<LabelType>::SpecializedSameBlock<back>::resume() { cache->amount_completed--; }

template<typename LabelType>
template<bool back>
void ConstraintSameBlock<LabelType>::SpecializedSameBlock<back>::fixate() { cache->amount_completed++; }

template<typename LabelType>
template<bool back>
void ConstraintSameBlock<LabelType>::SpecializedSameBlock<back>::cancel() { }

template class ConstraintSameBlock<std::string>;
