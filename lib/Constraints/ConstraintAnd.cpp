#include "llvm/Constraints/ConstraintAnd.hpp"
#include <algorithm>
#include <vector>
#include <list>

template<typename LabelType, typename ValueType>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> ConstraintAnd<LabelType,ValueType>::get_specializations() const
{
    std::vector<std::pair<LabelType,std::vector<std::unique_ptr<Specialized<ValueType>>>>> result_temp;

    for(auto& constraint : constraints)
    {
        auto local_result = constraint->get_specializations();

        for(auto& result : local_result)
        {
            auto find_it = std::find_if(result_temp.begin(), result_temp.end(),
                                        [&result](decltype(result_temp.front())& a) { return a.first == result.first; });

            if(find_it != result_temp.end())
            {
                find_it->second.emplace_back(std::move(result.second));
            }
            else
            {
                std::vector<std::unique_ptr<Specialized<ValueType>>> temp;
                temp.emplace_back(std::move(result.second));

                result_temp.emplace_back(result.first, std::move(temp));
            }
        }
    }

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> result;

    for(auto& element : result_temp)
    {
        result.emplace_back(element.first, std::unique_ptr<Specialized<ValueType>>(new SpecializedAnd{std::move(element.second)}));
    }

    return result;
}

template<typename LabelType, typename ValueType>
SkipResult ConstraintAnd<LabelType,ValueType>::SpecializedAnd::skip_invalid(ValueType& c)
{
    for(auto& constraint : constraints)
    {
        SkipResult local_result = constraint->skip_invalid(c);

        if(local_result == SkipResult::FAIL)
        {
            return SkipResult::FAIL;
        }
        else if(local_result == SkipResult::CHANGE ||
                local_result == SkipResult::CHANGEPASS)
        {
           return SkipResult::CHANGE;
        }
    }

    return SkipResult::PASS;
}

template<typename LabelType, typename ValueType>
void ConstraintAnd<LabelType,ValueType>::SpecializedAnd::begin()
{
    for(auto& constraint : constraints)
        constraint->begin();
}

template<typename LabelType, typename ValueType>
void ConstraintAnd<LabelType,ValueType>::SpecializedAnd::resume()
{
    for(auto& constraint : constraints)
        constraint->resume();
}

template<typename LabelType, typename ValueType>
void ConstraintAnd<LabelType,ValueType>::SpecializedAnd::fixate()
{
    for(auto& constraint : constraints)
        constraint->fixate(); 
}

template<typename LabelType, typename ValueType>
void ConstraintAnd<LabelType,ValueType>::SpecializedAnd::cancel()
{
    for(auto& constraint : constraints)
        constraint->cancel();
}

template class ConstraintAnd<std::string,unsigned>;
