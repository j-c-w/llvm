#include "llvm/Constraints/ConstraintOrdering.hpp"

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::ConstraintOrdering(LabelType var1, LabelType var2)
                              : variable1(var1), variable2(var2) { }

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::get_specializations() const
{
    std::shared_ptr<SharedData> cache(new SharedData{0, 0, 0});

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> result;

    result.emplace_back(variable1, std::unique_ptr<Specialized<ValueType>>(new SpecializedOrdering<false>(cache)));
    result.emplace_back(variable2, std::unique_ptr<Specialized<ValueType>>(new SpecializedOrdering<true>(cache)));

    return result;
}

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
template<bool back>
ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::SpecializedOrdering<back>::SpecializedOrdering(std::shared_ptr<SharedData> c) : cache(c) { }

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
template<bool back>
SkipResult ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::SpecializedOrdering<back>::skip_invalid(ValueType& c)
{
    if(cache->amount_completed == 1)
    {
        if(c < (back?cache->temp_value1:cache->temp_value2))
        {
            if((lt && !back) || (gt && back))
            {
                (back?cache->temp_value2:cache->temp_value1) = c;
                return SkipResult::PASS;
            }
            else if(eq)
            {
                (back?cache->temp_value2:cache->temp_value1) = c = (back?cache->temp_value1:cache->temp_value2);
                return SkipResult::CHANGEPASS;
            }
            else if(((gt && !back) || (lt && back)) && (back?cache->temp_value1:cache->temp_value2) < UINT_MAX)
            {
                (back?cache->temp_value2:cache->temp_value1) = c = (back?cache->temp_value1:cache->temp_value2) + 1;
                return SkipResult::CHANGEPASS;
            }
            else return SkipResult::FAIL;
        }
        else if(c == (back?cache->temp_value1:cache->temp_value2))
        {
            if(eq)
            {
                (back?cache->temp_value2:cache->temp_value1) = c = (back?cache->temp_value1:cache->temp_value2);
                return SkipResult::PASS;
            }
            else if(((gt && !back) || (lt && back)) && (back?cache->temp_value1:cache->temp_value2) < UINT_MAX)
            {
                (back?cache->temp_value2:cache->temp_value1) = c = (back?cache->temp_value1:cache->temp_value2) + 1;
                return SkipResult::CHANGEPASS;
            }
            else return SkipResult::FAIL;
        }
        else if((gt && !back) || (lt && back))
        {
            (back?cache->temp_value2:cache->temp_value1) = c;
            return SkipResult::PASS;
        }
        else return SkipResult::FAIL;
    }
    else
    {
        (back?cache->temp_value2:cache->temp_value1) = c;
        return SkipResult::PASS;
    }
}

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
template<bool back>
void ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::SpecializedOrdering<back>::begin() { }

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
template<bool back>
void ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::SpecializedOrdering<back>::resume()
{
    --cache->amount_completed;
}

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
template<bool back>
void ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::SpecializedOrdering<back>::fixate()
{
    ++cache->amount_completed;
}

template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
template<bool back>
void ConstraintOrdering<ValueType,LabelType,lt,eq,gt>::SpecializedOrdering<back>::cancel() { }

template class ConstraintOrdering<unsigned,std::string,false,false,false>;
template class ConstraintOrdering<unsigned,std::string,false,false,true>;
template class ConstraintOrdering<unsigned,std::string,false,true,false>;
template class ConstraintOrdering<unsigned,std::string,false,true,true>;
template class ConstraintOrdering<unsigned,std::string,true,false,false>;
template class ConstraintOrdering<unsigned,std::string,true,false,true>;
template class ConstraintOrdering<unsigned,std::string,true,true,false>;
template class ConstraintOrdering<unsigned,std::string,true,true,true>;

