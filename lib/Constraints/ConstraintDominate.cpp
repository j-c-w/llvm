#include "llvm/Constraints/ConstraintDominate.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"

template<typename LabelType, bool reverse>
ConstraintDominate<LabelType,reverse>::ConstraintDominate(const GraphEngine::Graph& gf, const GraphEngine::Graph& gb,
                                                          std::vector<unsigned> o, std::vector<LabelType> vdyno,
                                                          std::vector<unsigned> b, std::vector<LabelType> vdynb,
                                                          std::vector<unsigned> d, std::vector<LabelType> vdynd,
                                                          bool au, std::vector<unsigned> el)
                                     : graph_forw(gf), graph_back(gb),
                                       origins(o), var_dyn_origins(vdyno),
                                       blockades(b), var_dyn_blockades(vdynb),
                                       destinies(d), var_dyn_destinies(vdynd),
                                       eliminables(el), allow_unstrict(au)  { }

template<typename LabelType, bool reverse>
std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>>
                                                            ConstraintDominate<LabelType,reverse>::get_specializations() const
{
    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> result;

    std::shared_ptr<SharedData> cache = std::make_shared<SharedData>();

    cache->graph_engine = GraphEngine(graph_forw);

    cache->filled_origins = origins;
    cache->filled_blockades = blockades;
    cache->filled_destinies = destinies;

    cache->filled_origins.resize(origins.size() + var_dyn_origins.size());
    cache->filled_blockades.resize(blockades.size() + var_dyn_blockades.size());
    cache->filled_destinies.resize(destinies.size() + var_dyn_destinies.size());
    cache->filled_eliminables.resize(eliminables.size());

    cache->used_origins = origins.size();
    cache->used_blockades = blockades.size();
    cache->used_destinies = destinies.size();
    cache->used_eliminables = 0;

    cache->unused_origins = 0;
    cache->unused_blockades = 0;
    cache->unused_destinies = 0;

    cache->remaining_origins = var_dyn_origins.size();
    cache->remaining_blockades = var_dyn_blockades.size();
    cache->remaining_destinies = var_dyn_destinies.size();

    cache->allow_unstrict = allow_unstrict;

    for(std::string var : var_dyn_origins)
    {
        result.emplace_back(var, std::unique_ptr<Specialized<unsigned>>(new SpecializedDominate<0,false>(cache)));
    }
    for(std::string var : var_dyn_blockades)
    {
        result.emplace_back(var, std::unique_ptr<Specialized<unsigned>>(new SpecializedDominate<1,false>(cache)));
    }
    for(std::string var : var_dyn_destinies)
    {
        result.emplace_back(var, std::unique_ptr<Specialized<unsigned>>(new SpecializedDominate<2,false>(cache)));
    }

    return result; 
}

template<typename LabelType, bool reverse>
template<unsigned N,bool EL>
ConstraintDominate<LabelType,reverse>::SpecializedDominate<N,EL>::SpecializedDominate(std::shared_ptr<SharedData> c)
                                                                : cache(c) { }

template<typename LabelType, bool reverse>
template<unsigned N,bool EL>
SkipResult ConstraintDominate<LabelType,reverse>::SpecializedDominate<N,EL>::skip_invalid(unsigned& c)
{
    if(c >= cache->graph_engine.graph_forw.size() && c != UINT_MAX-1)
    {
        if(c < UINT_MAX-1)
        {
            c = UINT_MAX-1;
            fixed_value = c;
            return SkipResult::CHANGE;
        }
        else
        {
            return SkipResult::FAIL;
        }
    }
    else if((N == 0 && (cache->remaining_blockades + cache->remaining_destinies > 0)) ||
            (N == 1 && (cache->remaining_origins   + cache->remaining_destinies > 0)) ||
            (N == 2 && (cache->remaining_origins   + cache->remaining_blockades > 0)) ||
            (N == 0 && (cache->remaining_origins   > 0)) ||
            (N == 2 && (cache->remaining_destinies > 0)))
    {
        if(c != UINT_MAX-1)
        {
            if     (N == 0) cache->filled_origins  [cache->used_origins]   = c;
            else if(N == 1) cache->filled_blockades[cache->used_blockades] = c;
            else if(N == 2) cache->filled_destinies[cache->used_destinies] = c;
        }

        fixed_value = c;
        return SkipResult::PASS;
    }
    else
    {
        unsigned used_origins     = cache->used_origins;
        unsigned used_blockades   = cache->used_blockades;
        unsigned used_destinies   = cache->used_destinies;

        if(c != UINT_MAX-1)
        {
            if     (N == 0) cache->filled_origins  [used_origins++]   = c;
            else if(N == 1) cache->filled_blockades[used_blockades++] = c;
            else if(N == 2) cache->filled_destinies[used_destinies++] = c;
        }

        bool domination_holds =
            cache->graph_engine.compute_domination(cache->filled_origins  .begin(),
                                                   cache->filled_origins  .begin() + used_origins,
                                                   cache->filled_blockades.begin(),
                                                   cache->filled_blockades.begin() + used_blockades,
                                                   cache->filled_destinies.begin(),
                                                   cache->filled_destinies.begin() + used_destinies,
                                                   cache->allow_unstrict);

        if(cache->remaining_blockades == 0 && ((!domination_holds && !reverse) || (domination_holds && reverse) ))
        {
            if(c >= UINT_MAX-1)
            {
                return SkipResult::FAIL;
            }
            else if(c + 1 < cache->graph_engine.graph_forw.size())
            {
                c = c + 1;
                fixed_value = c;
                return SkipResult::CHANGE;
            }
            else
            {
                c = UINT_MAX-1;
                fixed_value = c;
               return SkipResult::CHANGE;
            }
        }

        fixed_value = c;
        return SkipResult::PASS;
    }
}

template<typename LabelType, bool reverse>
template<unsigned N,bool EL>
void ConstraintDominate<LabelType,reverse>::SpecializedDominate<N,EL>::begin()
{
    if     (N == 0) cache->remaining_origins--;
    else if(N == 1) cache->remaining_blockades--;
    else if(N == 2) cache->remaining_destinies--;

    if(N == 1 && EL) cache->filled_eliminables[cache->used_eliminables++] = cache->used_blockades;
}

template<typename LabelType, bool reverse>
template<unsigned N,bool EL>
void ConstraintDominate<LabelType,reverse>::SpecializedDominate<N,EL>::resume()
{
    if     (N == 0 && fixed_value == UINT_MAX-1) cache->unused_origins--;
    else if(N == 1 && fixed_value == UINT_MAX-1) cache->unused_blockades--;
    else if(N == 2 && fixed_value == UINT_MAX-1) cache->unused_destinies--;
    else if(N == 0)                              cache->used_origins--;
    else if(N == 1)                              cache->used_blockades--;
    else if(N == 2)                              cache->used_destinies--;
}

template<typename LabelType, bool reverse>
template<unsigned N,bool EL>
void ConstraintDominate<LabelType,reverse>::SpecializedDominate<N,EL>::cancel()
{
    if     (N == 0) cache->remaining_origins++;
    else if(N == 1) cache->remaining_blockades++;
    else if(N == 2) cache->remaining_destinies++;

    if(N == 1 && EL) cache->used_eliminables--;
}

template<typename LabelType, bool reverse>
template<unsigned N,bool EL>
void ConstraintDominate<LabelType,reverse>::SpecializedDominate<N,EL>::fixate()
{
    if     (N == 0 && fixed_value == UINT_MAX-1) cache->unused_origins++;
    else if(N == 1 && fixed_value == UINT_MAX-1) cache->unused_blockades++;
    else if(N == 2 && fixed_value == UINT_MAX-1) cache->unused_destinies++;
    else if(N == 0)                              cache->used_origins++;
    else if(N == 1)                              cache->used_blockades++;
    else if(N == 2)                              cache->used_destinies++;
}

template class ConstraintDominate<std::string,false>;
template class ConstraintDominate<std::string,true>;
