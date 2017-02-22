#ifndef _CONSTRAINT_PREFIX_HPP_
#define _CONSTRAINT_PREFIX_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

/* This constraint does nothing aside from changing the variable names in another constraint.
   More specifically it adds a prefix to each variable name.
   This is for general convenience but is also needed to separate nested ConstraintRange constraints. */
template<typename LabelType, typename ValueType>
class ConstraintPrefix : public Constraint<LabelType,ValueType>
{
public:
    template<typename  Type>
    ConstraintPrefix(std::string p, Type c) : prefix(p), constraint(new Type(c)) { }

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final
    {
        auto result = constraint->get_specializations();

        for(auto& entry : result)
        {
            if(entry.first.empty() || prefix.empty())
            {
                entry.first = prefix;
            }
            else if(entry.first[0] == '@')
            {
                entry.first = std::string(entry.first.begin() + 1, entry.first.end());
            }
            else
            {
                entry.first = prefix + "." + entry.first;
            }
        }

        return result;
    }

private:

    LabelType                                        prefix;
    std::shared_ptr<Constraint<LabelType,ValueType>> constraint;
};


#endif
