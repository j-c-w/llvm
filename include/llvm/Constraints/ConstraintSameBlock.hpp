#ifndef _CONSTRAINT_SAME_BLOCK_HPP_
#define _CONSTRAINT_SAME_BLOCK_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include <memory>
#include <vector>

/* This block is really only needed for PHI nodes, as the rest of the framework is basic block agnostic. */
template<typename LabelType = std::string>
class ConstraintSameBlock : public Constraint<LabelType,unsigned>
{
public:
    using Graph = std::vector<std::vector<unsigned>>;

    ConstraintSameBlock(const FunctionWrapper& wrap, LabelType v1, LabelType v2);

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<unsigned>>>> get_specializations() const final;

private:
    struct SharedData
    {
        const FunctionWrapper& wrap;
        unsigned temp_value1;
        unsigned temp_value2;
        unsigned amount_completed;
    };

    template<bool back>
    class SpecializedSameBlock : public Specialized<unsigned>
    {
    public:
        SpecializedSameBlock(std::shared_ptr<SharedData> c);

        SkipResult skip_invalid(unsigned& c) final;

        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::shared_ptr<SharedData> cache;
    };

    const FunctionWrapper& wrap;
    LabelType              variable1;
    LabelType              variable2;
};

#endif
