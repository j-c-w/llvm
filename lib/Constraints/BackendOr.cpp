#include "llvm/Constraints/Backends.hpp"
#include <memory>
#include <vector>
#include <limits>

BackendOr_::BackendOr_(std::array<unsigned,1>, std::vector<std::vector<SpecializedContainer>> c)
          : constraints(std::move(c)), disabled_since(constraints.empty()?0:constraints.front().size(), 0)
{ }

template<unsigned idx1> 
SkipResult BackendOr_::skip_invalid(unsigned idx2, Specialized::Value& c)
{
    SkipResult result = SkipResult::FAIL;
    Specialized::Value min = std::numeric_limits<Specialized::Value>::max();

    for(unsigned i = 0; i < constraints[idx2].size(); i++)
    {
        unsigned temp = c;
        SkipResult local_result = (disabled_since[i] > 0) ? SkipResult::FAIL : constraints[idx2][i]->skip_invalid(temp);

        if(local_result == SkipResult::PASS)
        {
            result = SkipResult::PASS;
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
    }

    return result;
}

template<unsigned idx1> 
void BackendOr_::begin(unsigned idx2)
{
    for(unsigned i = 0; i < constraints[idx2].size(); i++)
    {
        if(disabled_since[i] == 0)
        {
            constraints[idx2][i]->begin();
        }
    }
}

template<unsigned idx1> 
void BackendOr_::resume(unsigned idx2, Specialized::Value c)
{
    for(unsigned i = 0; i < constraints[idx2].size(); i++)
    {
        if(disabled_since[i] > 0)
        {
            disabled_since[i]--;
        }
        else
        {
            constraints[idx2][i]->resume(c);
        }
    }
}

template<unsigned idx1> 
void BackendOr_::fixate(unsigned idx2, Specialized::Value c)
{
    for(unsigned i = 0; i < constraints[idx2].size(); i++)
    {
        if(disabled_since[i] > 0) disabled_since[i] ++;
        else
        {
            Specialized::Value temp = c;

            if(constraints[idx2][i]->skip_invalid(temp) != SkipResult::PASS)
            {
                disabled_since[i] = 1;
            }
            else
            {
                constraints[idx2][i]->fixate(c);
            }
        }
    }
}

template<unsigned idx1> 
void BackendOr_::cancel(unsigned idx2)
{
    for(unsigned i = 0; i < constraints[idx2].size(); i++)
    {
        if(disabled_since[i] == 0)
        {
            constraints[idx2][i]->cancel();
        }
    }
}

template SkipResult BackendOr_::skip_invalid<0>(unsigned,Specialized::Value&);
template       void BackendOr_::begin<0>(unsigned);
template       void BackendOr_::fixate<0>(unsigned,Specialized::Value);
template       void BackendOr_::resume<0>(unsigned,Specialized::Value);
template       void BackendOr_::cancel<0>(unsigned);