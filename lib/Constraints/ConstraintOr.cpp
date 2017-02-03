#include "llvm/Constraints/ConstraintOr.hpp"
#include <algorithm>
#include <limits>
#include <memory>
#include <vector>
#include <list>

template<typename LabelType, typename ValueType>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> ConstraintOr<LabelType,ValueType>::get_specializations() const
{
    std::vector<std::pair<LabelType,std::vector<std::unique_ptr<Specialized<ValueType>>>>> result_temp;

    for(auto& constraint : constraints)
    {
        auto local_result = constraint->get_specializations();

        for(auto& result : local_result)
        {
            auto find_it = std::find_if(result_temp.begin(), result_temp.end(),
                                        [&result](decltype(result_temp.front())& a) { return a.first == result.first; });

            if(find_it == result_temp.end())
            {
                if(result_temp.empty())
                {
                    result_temp.emplace_back(result.first, std::vector<std::unique_ptr<Specialized<ValueType>>>{});
                }
                else
                {
                    std::vector<std::unique_ptr<Specialized<ValueType>>> temp;

                    for(unsigned i = 0; i < result_temp.front().second.size(); i++)
                        temp.emplace_back(nullptr);
    
                    result_temp.emplace_back(result.first,std::move(temp));
                }
            }
        }

        for(auto& result : result_temp)
        {
            auto find_it = std::find_if(local_result.begin(), local_result.end(),
                                        [&result](decltype(local_result.front()) a) { return a.first == result.first; });

            if(find_it != local_result.end())
            {
                result.second.push_back(std::move(find_it->second));
            }
            else
            {
                result.second.push_back(nullptr);
            }
        }
    }

    std::shared_ptr<SharedData> cache(new SharedData{std::vector<std::vector<unsigned>>(result_temp.size()), 0});
    for(auto& vec : cache->current_possibles_stack)
    {
        vec.reserve(constraints.size());
    }

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> result;

    for(auto& element : result_temp)
    {
        result.emplace_back(element.first, std::unique_ptr<Specialized<ValueType>>(new SpecializedOr(std::move(element.second), cache)));
    }

    return result;
}

template<typename LabelType, typename ValueType>
ConstraintOr<LabelType,ValueType>::SpecializedOr::SpecializedOr(std::vector<std::unique_ptr<Specialized<ValueType>>> c, std::shared_ptr<SharedData> c2)
                              : cache(c2), constraints(std::move(c))
{
    preselection.reserve(c.size());
}

template<typename LabelType, typename ValueType>
SkipResult ConstraintOr<LabelType,ValueType>::SpecializedOr::skip_invalid(ValueType& c)
{
    SkipResult result = SkipResult::FAIL;
    ValueType min = std::numeric_limits<unsigned int>::max();

    for(auto& possible : preselection)
    {
        unsigned temp = c;
        SkipResult local_result = (possible.second == nullptr) ? SkipResult::PASS
                                : possible.second->skip_invalid(temp);

        if(local_result == SkipResult::PASS)
        {
            temp_result = c;
            result = SkipResult::PASS;
            cache->current_possibles_stack[cache->stack_size-1].assign(1, possible.first);
            break;
        }
        else if(local_result == SkipResult::CHANGE || local_result == SkipResult::CHANGEPASS)
        {
            result = SkipResult::CHANGE;

            if(temp < min)
            {
                min = temp;
            }
        }
    }

    if(result == SkipResult::CHANGE)
    {
        c = min;
        temp_result = c;
    }

    return result;
}

template<typename LabelType, typename ValueType>
void ConstraintOr<LabelType,ValueType>::SpecializedOr::begin()
{
    preselection.resize(0);

    if(cache->stack_size > 0)
    {
        for(unsigned index : cache->current_possibles_stack[cache->stack_size-1])
        {
            if(constraints[index] != nullptr) constraints[index]->begin();
            preselection.emplace_back(index, &*constraints[index]);
        }
    }
    else
    {
        for(unsigned index = 0; index < constraints.size(); index++)
        {
            if(constraints[index] != nullptr) constraints[index]->begin();
            preselection.emplace_back(index, &*constraints[index]);
        }
    }

    ++cache->stack_size;
}

template<typename LabelType, typename ValueType>
void ConstraintOr<LabelType,ValueType>::SpecializedOr::resume()
{
    for(unsigned idx : cache->current_possibles_stack[cache->stack_size-1])
    {
        if(constraints[idx] != nullptr) constraints[idx]->resume();
    }
}

template<typename LabelType, typename ValueType>
void ConstraintOr<LabelType,ValueType>::SpecializedOr::fixate()
{
    for(auto& possible : preselection)
    {
        if(possible.first <= cache->current_possibles_stack[cache->stack_size-1].front()) continue;

        unsigned temp = temp_result;
        SkipResult local_result = (possible.second == nullptr) ? SkipResult::PASS
                                : possible.second->skip_invalid(temp);

        if(local_result == SkipResult::PASS)
        {
            cache->current_possibles_stack[cache->stack_size-1].push_back(possible.first);
        }
    }

    for(unsigned idx : cache->current_possibles_stack[cache->stack_size-1])
    {
        if(constraints[idx] != nullptr) constraints[idx]->fixate();
    }
}

template<typename LabelType, typename ValueType>
void ConstraintOr<LabelType,ValueType>::SpecializedOr::cancel()
{
    for(auto& possible : preselection)
    {
        if(possible.second != nullptr) possible.second->cancel();
    }

    --cache->stack_size;
}


template class ConstraintOr<std::string,unsigned>;
