#ifndef _CONSTRAINT_SHARED_FATE_N_HPP_
#define _CONSTRAINT_SHARED_FATE_N_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>
#include <deque>
#include <list>
#include <sstream>

/* This constraint is crucial for fast SMT solving. Nodes that are combined with this constraint are always backtracked
   together. This means that if the value of one of them is discarded in the solving process, they are all discarded.
   The soundness of this is not assured by the solver, so is used wrecklessly it can skip solutions in the solver! */
class ConstraintSharedFate : public Constraint<std::string,unsigned>
{
public:
    ConstraintSharedFate() {}

    ConstraintSharedFate(std::string prefix, unsigned N)
    {
        for(unsigned i = 0; i < N; i++)
        {
            std::stringstream sstr;
            sstr<<prefix<<"["<<i<<"]";
            variables.push_back(sstr.str());
        }
    }

    std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> get_specializations() const final;

private:

    struct SharedData
    {
        unsigned filled_slots;
        bool     invalidated;
    };

    class SpecializedSharedFate : public Specialized<unsigned>
    {
    public:
        SpecializedSharedFate(std::shared_ptr<SharedData> cache);

        SkipResult skip_invalid(unsigned &c) final;
  
        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::shared_ptr<SharedData> cache;
    };

    std::deque<unsigned*>   slots;
    std::deque<std::string> variables;
};

#endif
