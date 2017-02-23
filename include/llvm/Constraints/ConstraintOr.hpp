#ifndef _CONSTRAINT_OR_HPP_
#define _CONSTRAINT_OR_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that at least one of
   them is satisfied. */
template<typename LabelType, typename ValueType>
class ConstraintOr : public Constraint<LabelType,ValueType>
{
public:
    template<typename ... Types>
    ConstraintOr(Types ... constraints) { add_constraints(constraints ...); }

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final;

private:
    struct SharedData
    {
        std::vector<std::vector<unsigned>> current_possibles_stack;
        unsigned                           stack_size;
    };

    class SpecializedOr : public Specialized<ValueType> //This crashed when some branches don't use all parameters!
                                                        //Update: Not sure whether the comment still applies.
                                                        //Need to investigate!
    {
    public:
        SpecializedOr(std::vector<std::unique_ptr<Specialized<ValueType>>> c, std::shared_ptr<SharedData> c2);

        SkipResult skip_invalid(ValueType& c) final;

        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

    private:
        std::shared_ptr<SharedData>                              cache;
        std::vector<std::unique_ptr<Specialized<ValueType>>>     constraints;
        std::vector<std::pair<unsigned,Specialized<ValueType>*>> preselection;
        unsigned                                                 temp_result;
    };

    void add_constraints() { }

    template<typename TFirst, typename ... TRest> 
    void add_constraints(TFirst first, TRest ... rest);

    template<typename Type>
    void add_constraint(Type constraint,
            typename std::enable_if<std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type* = nullptr);

    template<typename Type>
    void add_constraint(Type first,
            typename std::enable_if<std::is_base_of<Constraint<LabelType,ValueType>,Type>::value>::type* = nullptr,
            typename std::enable_if<!std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type* = nullptr);

    template<typename Type>
    void add_constraint(std::vector<Type> constraint,
            typename std::enable_if<std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type* = nullptr);

    template<typename Type>
    void add_constraint(std::vector<Type> first,
            typename std::enable_if<std::is_base_of<Constraint<LabelType,ValueType>,Type>::value>::type* = nullptr,
            typename std::enable_if<!std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type* = nullptr);

    std::vector<std::shared_ptr<Constraint<LabelType,ValueType>>> constraints;
};

template<typename LabelType, typename ValueType>
template<typename TFirst, typename ... TRest> 
void ConstraintOr<LabelType,ValueType>::add_constraints(TFirst first, TRest ... rest)
{
    add_constraint(first);
    add_constraints(rest...);
}

template<typename LabelType, typename ValueType>
template<typename Type>
void ConstraintOr<LabelType,ValueType>::add_constraint(Type constraint,
            typename std::enable_if<std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type*)
{
    for(auto& constraint : constraint.constraints)
    {
        constraints.emplace_back(std::move(constraint));
    }
}

template<typename LabelType, typename ValueType>
template<typename Type>
void ConstraintOr<LabelType,ValueType>::add_constraint(Type constraint,
            typename std::enable_if<std::is_base_of<Constraint<LabelType,ValueType>,Type>::value>::type*,
            typename std::enable_if<!std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type*)
{
    constraints.push_back(std::shared_ptr<Constraint<LabelType,ValueType>>(new Type(constraint)));
}

template<typename LabelType, typename ValueType>
template<typename Type>
void ConstraintOr<LabelType,ValueType>::add_constraint(std::vector<Type> constraint_vector,
            typename std::enable_if<std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type*)
{
    for(auto& and_constraint : constraint_vector)
    {
        for(auto& constraint : and_constraint.constraints)
        {
            constraints.push_back(std::shared_ptr<Constraint<LabelType,ValueType>>(new Type(constraint)));
        }
    }
}

template<typename LabelType, typename ValueType>
template<typename Type>
void ConstraintOr<LabelType,ValueType>::add_constraint(std::vector<Type> constraint_vector,
            typename std::enable_if<std::is_base_of<Constraint<LabelType,ValueType>,Type>::value>::type*,
            typename std::enable_if<!std::is_base_of<ConstraintOr<LabelType,ValueType>,Type>::value>::type*)
{
    for(auto& constraint : constraint_vector)
    {
        constraints.push_back(std::shared_ptr<Constraint<LabelType,ValueType>>(new Type(constraint)));
    }
}

#endif
