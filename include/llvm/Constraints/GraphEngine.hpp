#ifndef _GRAPHENGINE_HPP_
#define _GRAPHENGINE_HPP_
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include <vector>
#include <deque>
#include <map>

/* This class implements helper functions to calculate fast graph domination tests. */
class GraphEngine
{
public:
    using Graph = std::vector<std::vector<unsigned>>;

    GraphEngine(const Graph& gf, const Graph& gb) :
        graph_forw(gf), graph_back(gb), counter(UINT_MAX),
        working_status(graph_forw.size(), 0), working_queue(graph_forw.size(), 0) { }

    GraphEngine(const Graph& gf) :
        graph_forw(gf), graph_back(gf), counter(UINT_MAX),
        working_status(graph_forw.size(), 0), working_queue(graph_forw.size(), 0) { }

    GraphEngine() = default;


    Graph                 graph_forw;
    Graph                 graph_back;
    unsigned              counter;
    std::vector<unsigned> working_status;
    std::vector<unsigned> working_queue;
    unsigned*             queue_front;
    unsigned*             queue_back;

    inline void initialize()
    {
        ++counter;
        queue_front = &working_queue[0];
        queue_back  = &working_queue[0];
    }

    template<typename iter_type>
    inline void set_destinations(iter_type destinations_begin, iter_type destinations_end)
    {
        for(auto destination = destinations_begin; destination != destinations_end; ++destination)
        {
            working_status[*destination] = 5*counter+2;
        }
    }

    template<typename iter_type>
    inline void set_dominators(iter_type dominators_begin, iter_type dominators_end)
    {
        for(auto dominator = dominators_begin; dominator != dominators_end; ++dominator)
        {
            working_status[*dominator] = 5*counter+1;
        }
    }

    template<typename iter_type>
    inline bool set_origins(iter_type origins_begin, iter_type origins_end)
    {
        for(auto origin = origins_begin; origin != origins_end; ++origin)
        {
            if(working_status[*origin] == 5*counter+2)
            {
                return false;
            }
            else if(working_status[*origin] <= 5*counter)
            {
                working_status[*origin] = 5*counter+3;
                *(queue_back++) = *origin;
            }
        }

        return true;
    }

    inline bool fill()
    {
        while(queue_front != queue_back)
        {
            auto element = *(queue_front++);
    
            for(unsigned i : graph_forw[element])
            {
                if(working_status[i] == 5*counter+2)
                {
                    return false;
                }
                else if(working_status[i] <= 5*counter)
                {
                    working_status[i] = 5*counter+3;
                    *(queue_back++) = i;
                }
            }
        }

        return true;
    }

public:

    template<typename iter_type1, typename iter_type2, typename iter_type3>
    bool compute_domination(iter_type1 origins_begin, iter_type1 origins_end,
                            iter_type2 dominators_begin, iter_type2 dominators_end,
                            iter_type3 destinations_begin, iter_type3 destinations_end, bool allow_unstrict = true)
    {
        initialize();

        if(allow_unstrict)
        {
            set_destinations(destinations_begin, destinations_end);
            set_dominators(dominators_begin, dominators_end);
        }
        else
        {
            set_dominators(dominators_begin, dominators_end);
            set_destinations(destinations_begin, destinations_end);
        }
                
        if(!set_origins(origins_begin, origins_end))
            return false;
        else
        {
            bool temp = fill();
            return temp;
        }
    }

    template<typename iter_type>
    unsigned next_reachable(iter_type origins_begin, iter_type origins_end, unsigned destination)
    {
        initialize();
        set_destinations(&destination, &destination + 1);

        if((!set_origins(origins_begin, origins_end)) || (!fill()))
        {
            return destination;
        }
        else
        {
            for(unsigned i = destination + 1; i < working_status.size(); i++)
            {
                if(working_status[i] == 5*counter+3 || working_status[i] == 5*counter+2)
                {
                    return i;
                }
            }

            return working_status.size();
        }
    }
};

#endif
