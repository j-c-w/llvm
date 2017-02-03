#ifndef _CONSTRAINT_RANGE_HPP_
#define _CONSTRAINT_RANGE_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

/* This constraint duplicated a constraint a specified amount of time.
   It searches for characters '*' and '+' in the variable names and replaces them with strings "[0]", ..., "[N-1]" and
   "[1]", ..., "[N]" for the different instanciations of the constraints.
   Nested ConstraintRange instances have to be separated by ConstraintPrefix to distinguish the wildcard symbols
   (the ones in the prefix then refer to the outer constraint). */
template<typename ValueType>
class ConstraintRange : public Constraint<std::string,ValueType>
{
public:
    template<typename Type>
    ConstraintRange(unsigned N, Type c) : size(N), constraint(new Type(c)) { }

    class SpecializedRange : public Specialized<ValueType>
    {
    public:
        SpecializedRange(std::vector<std::unique_ptr<Specialized<ValueType>>> c) : constraints(std::move(c)) { }

        SkipResult skip_invalid(ValueType& c) final;

        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::vector<std::unique_ptr<Specialized<ValueType>>> constraints;
    };

    std::vector<std::pair<std::string,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final;

private:

    unsigned                                           size;
    std::shared_ptr<Constraint<std::string,ValueType>> constraint;
};

#endif
