#ifndef _CONSTRAINT_COLLECT_HPP_
#define _CONSTRAINT_COLLECT_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/ConstraintsBasic.hpp"
#include "llvm/Constraints/ConstraintOrdering.hpp"
#include "llvm/Constraints/ConstraintComplexUnused.hpp"
#include <memory>
#include <vector>

class ConstraintCollect : public ConstraintAnd<std::string,unsigned>
{
public:
    template<typename  Type>
    ConstraintCollect(unsigned N, std::string prefix, Type constraint, unsigned Min = 0)
      : ConstraintAnd<std::string,unsigned>(helper_function(N, prefix, constraint, Min)) { }

private:
    template<typename  Type>
    ConstraintAnd<std::string,unsigned> helper_function(unsigned N, std::string prefix, Type constraint, unsigned Min)
    {
        for(auto& entry : constraint.get_specializations())
        {
            if(entry.first.empty() || entry.first.front() != '@')
            {
                std::string pivot = entry.first.empty() ? "" : ("."+entry.first);

                std::stringstream sstr;
                sstr<<"["<<N<<"]";

                if(Min > N) Min = N;

                return (   Min     *     (   constraint + (prefix+"*")
                                          && ConstraintOrder(prefix+"*"+pivot, prefix+"+"+pivot))
                        && (N-Min) * (   (   constraint + (prefix+"*")
                                          && ConstraintOrder(prefix+"*"+pivot, prefix+"+"+pivot))
                                      || (   ConstraintComplexUnused(constraint) + (prefix + "*")
                                          && ConstraintUnused(prefix+"+"+pivot)))
                        && ConstraintUnused(prefix+sstr.str()+pivot)
                        && ConstraintSharedFate(prefix, N+1, pivot));
            }
        }

        return ConstraintAnd<std::string,unsigned>();
    }
};


#endif
