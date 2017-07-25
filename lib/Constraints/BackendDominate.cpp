#include "llvm/Constraints/BackendClasses.hpp"

template<bool reverse,bool allow_unstrict>
BackendDominate<reverse,allow_unstrict>::BackendDominate(std::array<unsigned,3> size, const GraphEngine::Graph& gf)
               : graph_engine(gf), used_values{{0, 0, 0}}, remaining_values(size),
                 filled_values{{std::vector<unsigned>(std::get<0>(size)), std::vector<unsigned>(std::get<1>(size)),
                                std::vector<unsigned>(std::get<2>(size))}}
{ }

template<bool reverse,bool allow_unstrict>
template<unsigned idx1>
SkipResult BackendDominate<reverse,allow_unstrict>::skip_invalid(unsigned idx2, Specialized::Value& c)
{
    if(c >= graph_engine.graph_forw.size() && c != UINT_MAX-1)
    {
        if(c < UINT_MAX-1)
        {
            c = UINT_MAX-1;
            return SkipResult::CHANGE;
        }
        else return SkipResult::FAIL;
    }
    else 
    {
        std::get<idx1>(filled_values)[std::get<idx1>(used_values)] = c;

        if(std::get<(idx1+1)%3>(remaining_values) + std::get<(idx1+2)%3>(remaining_values) > 0 ||
           (idx1 != 1 && std::get<idx1>(remaining_values) > 0))
        {
            return SkipResult::PASS;
        }
        else
        {
            bool domination_holds = graph_engine.compute_domination(
                  std::get<0>(filled_values).begin(),
                  std::get<0>(filled_values).begin() + std::get<0>(used_values) + (c != UINT_MAX-1 && idx1 == 0),
                  std::get<1>(filled_values).begin(),
                  std::get<1>(filled_values).begin() + std::get<1>(used_values) + (c != UINT_MAX-1 && idx1 == 1),
                  std::get<2>(filled_values).begin(),
                  std::get<2>(filled_values).begin() + std::get<2>(used_values) + (c != UINT_MAX-1 && idx1 == 2),
                  allow_unstrict);

            if(std::get<1>(remaining_values) == 0 && ((domination_holds && reverse) || (!domination_holds && !reverse)))
            {
                if(c >= UINT_MAX-1)
                {
                    return SkipResult::FAIL;
                }
                else if(c + 1 < graph_engine.graph_forw.size())
                {
                    c = c + 1;
                    return SkipResult::CHANGE;
                }
                else
                {
                   c = UINT_MAX-1;
                   return SkipResult::CHANGE;
                }
            }

            return SkipResult::PASS;
        }
    }
}

template      class BackendDominate<false,false>;
template SkipResult BackendDominate<false,false>::skip_invalid<0>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<false,false>::skip_invalid<1>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<false,false>::skip_invalid<2>(unsigned,Specialized::Value&);
template      class BackendDominate<false,true>;
template SkipResult BackendDominate<false,true>::skip_invalid<0>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<false,true>::skip_invalid<1>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<false,true>::skip_invalid<2>(unsigned,Specialized::Value&);
template      class BackendDominate<true,false>;
template SkipResult BackendDominate<true,false>::skip_invalid<0>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<true,false>::skip_invalid<1>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<true,false>::skip_invalid<2>(unsigned,Specialized::Value&);
template      class BackendDominate<true,true>;
template SkipResult BackendDominate<true,true>::skip_invalid<0>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<true,true>::skip_invalid<1>(unsigned,Specialized::Value&);
template SkipResult BackendDominate<true,true>::skip_invalid<2>(unsigned,Specialized::Value&);
