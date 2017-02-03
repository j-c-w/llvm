#include "llvm/Constraints/ConstraintRange.hpp"
#include <algorithm>
#include <sstream>
#include <vector>
#include <list>

template<typename ValueType>
std::vector<std::pair<std::string,std::unique_ptr<Specialized<ValueType>>>> ConstraintRange<ValueType>::get_specializations() const
{
    std::vector<std::pair<std::string,std::vector<std::unique_ptr<Specialized<ValueType>>>>> result_temp;

    for(unsigned i = 0; i < size; i++)
    {
        auto local_result = constraint->get_specializations();

        for(auto& result : local_result)
        {
            std::string new_label;

            unsigned last_wildcard = result.first.size();

            for(unsigned j = 0; j < result.first.size(); j++)
            {
                if(result.first[j] == '*' || result.first[j] == '+')
                {
                    last_wildcard = j;
                }
            }

            for(unsigned j = 0; j < result.first.size(); j++)
            {
                if(j == last_wildcard && result.first[j] == '*')
                {
                    std::stringstream sstr;
                    sstr<<std::move(new_label)<<"["<<i<<"]";
                    new_label = sstr.str();
                }
                else if(j == last_wildcard && result.first[j] == '+')
                {
                    std::stringstream sstr;
                    sstr<<std::move(new_label)<<"["<<(i+1)<<"]";
                    new_label = sstr.str();
                }
                else
                {
                    new_label.push_back(result.first[j]);
                }
            }

            auto find_it = std::find_if(result_temp.begin(), result_temp.end(),
                                        [new_label](decltype(result_temp.front())& a) { return a.first == new_label; });

            if(find_it != result_temp.end())
            {
                find_it->second.emplace_back(std::move(result.second));
            }
            else
            {
                std::vector<std::unique_ptr<Specialized<ValueType>>> temp;
                temp.emplace_back(std::move(result.second));

                result_temp.emplace_back(new_label, std::move(temp));
            }
        }
    }

    std::vector<std::pair<std::string,std::unique_ptr<Specialized<ValueType>>>> result;

    for(auto& element : result_temp)
    {
        result.emplace_back(element.first, std::unique_ptr<Specialized<ValueType>>(new SpecializedRange{std::move(element.second)}));
    }

    return result;
}

template<typename ValueType>
SkipResult ConstraintRange<ValueType>::SpecializedRange::skip_invalid(ValueType& c)
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

template<typename ValueType>
void ConstraintRange<ValueType>::SpecializedRange::begin()
{
    for(auto& constraint : constraints)
        constraint->begin();
}

template<typename ValueType>
void ConstraintRange<ValueType>::SpecializedRange::resume()
{
    for(auto& constraint : constraints)
        constraint->resume();
}

template<typename ValueType>
void ConstraintRange<ValueType>::SpecializedRange::fixate()
{
    for(auto& constraint : constraints)
        constraint->fixate(); 
}

template<typename ValueType>
void ConstraintRange<ValueType>::SpecializedRange::cancel()
{
    for(auto& constraint : constraints)
        constraint->cancel();
}

template class ConstraintRange<unsigned>;
