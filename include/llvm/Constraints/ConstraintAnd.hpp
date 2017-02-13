#ifndef _CONSTRAINT_AND_HPP_
#define _CONSTRAINT_AND_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that all of them are
   satisfied at once. */
template<typename LabelType, typename ValueType>
class ConstraintAnd : public Constraint<LabelType,ValueType>
{
public:
    template<typename ... Types>
    ConstraintAnd(Types ... constraints) { add_constraints(constraints ...); }

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final;

private:
    class SpecializedAnd : public Specialized<ValueType>
    {
    public:
        SpecializedAnd(std::vector<std::unique_ptr<Specialized<ValueType>>> c) : constraints(std::move(c)) { }

        SkipResult skip_invalid(ValueType& c) final;

        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::vector<std::unique_ptr<Specialized<ValueType>>> constraints;
    };

    void add_constraints() { }

    template<typename TFirst, typename ... TRest> 
    void add_constraints(TFirst first, TRest ... rest);

    template<typename Type>
    void add_constraint(Type constraint,
            typename std::enable_if<std::is_base_of<ConstraintAnd<LabelType,ValueType>,Type>::value>::type* = nullptr);

    template<typename Type>
    void add_constraint(Type first,
            typename std::enable_if<std::is_base_of<Constraint<LabelType,ValueType>,Type>::value>::type* = nullptr,
            typename std::enable_if<!std::is_base_of<ConstraintAnd<LabelType,ValueType>,Type>::value>::type* = nullptr);

    std::vector<std::shared_ptr<Constraint<LabelType,ValueType>>> constraints;
};

template<typename LabelType, typename ValueType>
template<typename TFirst, typename ... TRest> 
void ConstraintAnd<LabelType,ValueType>::add_constraints(TFirst first, TRest ... rest)
{
    add_constraint(first);
    add_constraints(rest...);
}

template<typename LabelType, typename ValueType>
template<typename Type>
void ConstraintAnd<LabelType,ValueType>::add_constraint(Type constraint,
            typename std::enable_if<std::is_base_of<ConstraintAnd<LabelType,ValueType>,Type>::value>::type*)
{
    for(auto& constraint : constraint.constraints)
    {
        constraints.emplace_back(std::move(constraint));
    }
}

template<typename LabelType, typename ValueType>
template<typename Type>
void ConstraintAnd<LabelType,ValueType>::add_constraint(Type constraint,
            typename std::enable_if<std::is_base_of<Constraint<LabelType,ValueType>,Type>::value>::type*,
            typename std::enable_if<!std::is_base_of<ConstraintAnd<LabelType,ValueType>,Type>::value>::type*)
{
        constraints.push_back(std::shared_ptr<Constraint<LabelType,ValueType>>(new Type(constraint)));
}

#endif
