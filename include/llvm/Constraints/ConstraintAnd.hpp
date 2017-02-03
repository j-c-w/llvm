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
    ConstraintAnd(Types ... constraints) { add_constraint(constraints ...); }

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

    void add_constraint() { }

    template<typename TFirst>
    void add_constraint(TFirst);

    template<typename TFirst, typename TSecond, typename ... TRest> 
    void add_constraint(TFirst first, TSecond second, TRest ... rest);

    std::vector<std::shared_ptr<Constraint<LabelType,ValueType>>> constraints;
};

template<typename LabelType, typename ValueType>
template<typename TFirst> 
void ConstraintAnd<LabelType,ValueType>::add_constraint(TFirst first)
{
    if(std::is_base_of<ConstraintAnd<LabelType,ValueType>, TFirst>::value)
    {
        for(auto& constraint : ((ConstraintAnd<LabelType,ValueType>&)(first)).constraints)
        {
            constraints.emplace_back(std::move(constraint));
        }
    }
    else
    {
        constraints.push_back(std::shared_ptr<Constraint<LabelType,ValueType>>((new TFirst(first))));
    }
}

template<typename LabelType, typename ValueType>
template<typename TFirst, typename TSecond, typename ... TRest> 
void ConstraintAnd<LabelType,ValueType>::add_constraint(TFirst first, TSecond second, TRest ... rest)
{
    if(std::is_same<bool,TFirst>::value)   // This hack should be removed soon!
    {
        if(*(bool*)&first)
        {
            add_constraint(second);
        }

        add_constraint(rest...);
    }
    else
    {
        add_constraint(first);
        add_constraint(second, rest...);
    }
}

#endif
