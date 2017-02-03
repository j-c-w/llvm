#include "llvm/Constraints/ConstraintEdge.hpp"
#include "llvm/Constraints/SMTSolver.hpp"

template<typename LabelType>
ConstraintEdge<LabelType>::ConstraintEdge(const Graph& gf, const Graph& gb, LabelType v1, LabelType v2)
                         : graph_forw(gf), graph_back(gb), variable1(v1), variable2(v2) { }

template<typename LabelType>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> ConstraintEdge<LabelType>::get_specializations() const
{
    std::shared_ptr<SharedData> cache(new SharedData{graph_forw, graph_back, 0, 0, 0});

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> result;

    result.emplace_back(variable1, std::unique_ptr<Specialized<unsigned>>(new SpecializedEdge<true>(cache)));
    result.emplace_back(variable2, std::unique_ptr<Specialized<unsigned>>(new SpecializedEdge<false>(cache)));

    return result;
}

template<typename LabelType>
template<bool back>
ConstraintEdge<LabelType>::SpecializedEdge<back>::SpecializedEdge(std::shared_ptr<SharedData> c)
                                     : cache(c), hits(nullptr), hit_start() { }

template<typename LabelType>
template<bool back>
SkipResult ConstraintEdge<LabelType>::SpecializedEdge<back>::skip_invalid(unsigned& c)
{
    while(hits)
    {
        if(hit_start == hits->end()) return SkipResult::FAIL;
        else if(*hit_start < c)  hit_start++;
        else if(*hit_start == c)
        {
            (back?cache->temp_value2:cache->temp_value1) = c;
            return SkipResult::PASS; 
        }
        else
        {
            (back?cache->temp_value2:cache->temp_value1) = c = *hit_start;
            return SkipResult::CHANGEPASS;
        }
    }

    if(c < (back?cache->graph_back:cache->graph_forw).size())
    {
        (back?cache->temp_value2:cache->temp_value1) = c;
        return SkipResult::PASS;
    }
    else return SkipResult::FAIL;
}

template<typename LabelType>
template<bool back>
void ConstraintEdge<LabelType>::SpecializedEdge<back>::begin()
{
    if(cache->amount_completed == 1)
    {
        hits = &(back?cache->graph_back:cache->graph_forw)[back?cache->temp_value1:cache->temp_value2];
        hit_start = hits->begin();
    }
}

template<typename LabelType>
template<bool back>
void ConstraintEdge<LabelType>::SpecializedEdge<back>::resume()
{
    --cache->amount_completed;
}

template<typename LabelType>
template<bool back>
void ConstraintEdge<LabelType>::SpecializedEdge<back>::fixate()
{
    ++cache->amount_completed;
}

template<typename LabelType>
template<bool back>
void ConstraintEdge<LabelType>::SpecializedEdge<back>::cancel()
{
    hits = nullptr;
}

template class ConstraintEdge<std::string>;
