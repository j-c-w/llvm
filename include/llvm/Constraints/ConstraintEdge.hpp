#ifndef _CONSTRAINT_EDGE_HPP_
#define _CONSTRAINT_EDGE_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

/* This class implements the graph edge constraint, demanding that there is an edge between two given nodes in some
   graph that is passes as an additional parameter. */
template<typename LabelType>
class ConstraintEdge : public Constraint<std::string,unsigned>
{
public:
    using Graph = std::vector<std::vector<unsigned>>;

    ConstraintEdge(const Graph& gf, const Graph& gb, LabelType v1, LabelType v2);

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> get_specializations() const final;

private:
    struct SharedData
    {
        const    Graph& graph_forw;
        const    Graph& graph_back;
        unsigned temp_value1;
        unsigned temp_value2;
        unsigned amount_completed;
    };

    template<bool back>
    class SpecializedEdge : public Specialized<unsigned>
    {
    public:
        SpecializedEdge(std::shared_ptr<SharedData> c);

        SkipResult skip_invalid(unsigned& c) final;

        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::shared_ptr<SharedData>           cache;
        const std::vector<unsigned>*          hits;
        std::vector<unsigned>::const_iterator hit_start;
    };

    const Graph& graph_forw;
    const Graph& graph_back;
    LabelType    variable1;
    LabelType    variable2;
};

#endif
