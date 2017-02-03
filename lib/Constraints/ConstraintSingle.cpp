#include "llvm/Constraints/ConstraintSingle.hpp"

template<typename LabelType, typename ValueType>
ConstraintSingle<LabelType,ValueType>::ConstraintSingle(LabelType var)
                : variable(var) { }


template<typename LabelType, typename ValueType>
ConstraintSingle<LabelType,ValueType>::ConstraintSingle(LabelType var, std::vector<ValueType> h) : variable(var), hits(h) { }

template<typename LabelType, typename ValueType>
ConstraintSingle<LabelType,ValueType>::ConstraintSingle(LabelType var, const std::function<bool(ValueType)>& p, ValueType b, ValueType e)
                   : ConstraintSingle(var)
{
    for(ValueType i = b; i < e; i++)
    {
        if(p(i)) hits.push_back(i);
    }
}

template<typename LabelType, typename ValueType>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> ConstraintSingle<LabelType,ValueType>::get_specializations() const
{
    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> result;

    result.emplace_back(variable, std::unique_ptr<Specialized<ValueType>>(new SpecializedSingle(hits)));

    return result;
}

template<typename LabelType, typename ValueType>
ConstraintSingle<LabelType,ValueType>::SpecializedSingle::SpecializedSingle(const std::vector<ValueType>& h)
                                      : hits(h) { }

template<typename LabelType, typename ValueType>
SkipResult ConstraintSingle<LabelType,ValueType>::SpecializedSingle::skip_invalid(ValueType& c) 
{
    while(true)
    {
        if(hit_start == hits.end()) return SkipResult::FAIL;
        else if(*hit_start < c)  hit_start++;
        else if(*hit_start == c) return SkipResult::PASS;
        else
        {
            c = *hit_start;
            return SkipResult::CHANGEPASS;
        }
    }
}

template class ConstraintSingle<std::string,unsigned>;
