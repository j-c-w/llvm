#ifndef _CONSTRAINT_DOMINATE_HPP_
#define _CONSTRAINT_DOMINATE_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include <memory>
#include <vector>

/* This class implements graph domination constraints. It is very generic and some of the options are non-obvious.
   A dominance relationship between nodes x nd y in the most standard way means that for any path from the origin of
   the graph to y has to pass through x.
   This implementation generalized this by taking three sets of nodes R, X, Y and stipulating that each path from
   an element of R to an element of Y has to pass through at least one element of X.
   Furthermore the sets R, X and Y are each split into some constant elements and some solver variables.
   Lastly, the reverse option inverts the meaning of the constraint, i.e. it demands that a path from an element of R
   to an element of Y exists that avoids the set X. */
template<typename LabelType, bool reverse>
class ConstraintDominate : public Constraint<std::string,unsigned>
{
public:
    ConstraintDominate(const GraphEngine::Graph& graph_forw,
                       const GraphEngine::Graph& graph_back,
                       std::vector<unsigned> origins,   std::vector<LabelType> var_dyn_origins,
                       std::vector<unsigned> blockades, std::vector<LabelType> var_dyn_blockades,
                       std::vector<unsigned> destinies, std::vector<LabelType> var_dyn_destinies,
                       bool allow_unstrict,
                       std::vector<unsigned> eliminables = {});

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> get_specializations() const final;

private:

    struct SharedData
    {
        GraphEngine           graph_engine;
        std::vector<unsigned> filled_origins;
        std::vector<unsigned> filled_blockades;
        std::vector<unsigned> filled_destinies;
        std::vector<unsigned> filled_eliminables;
        unsigned              used_origins;
        unsigned              used_blockades;
        unsigned              used_destinies;
        unsigned              used_eliminables;
        unsigned              remaining_origins;
        unsigned              remaining_blockades;
        unsigned              remaining_destinies;
        unsigned              unused_origins;
        unsigned              unused_blockades;
        unsigned              unused_destinies;
        bool                  allow_unstrict;
    };

    template<unsigned N,bool EL>
    class SpecializedDominate : public Specialized<unsigned>
    {
    public:
        SpecializedDominate(std::shared_ptr<SharedData> cache);

        SkipResult skip_invalid(unsigned &c) final;
  
        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::shared_ptr<SharedData> cache;
        unsigned                    fixed_value;
    };

    const GraphEngine::Graph& graph_forw;
    const GraphEngine::Graph& graph_back;
    std::vector<unsigned>     origins;
    std::vector<LabelType>    var_dyn_origins;
    std::vector<unsigned>     blockades;
    std::vector<LabelType>    var_dyn_blockades;
    std::vector<unsigned>     destinies;
    std::vector<LabelType>    var_dyn_destinies;
    std::vector<unsigned>     eliminables;
    unsigned                  fixed_blockades;
    bool                      allow_unstrict;
};

#endif
