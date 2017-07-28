#ifndef _CONSTRAINT_HPP_
#define _CONSTRAINT_HPP_
#include "llvm/Constraints/SMTSolver.hpp"

/* Constraints essentially only exist to generate instances of the above shown SolverAtom class. */
class Constraint : public std::vector<std::string>
{
public:
    using std::vector<std::string>::vector;
    virtual ~Constraint() { };

    virtual void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const = 0;
};

#endif
