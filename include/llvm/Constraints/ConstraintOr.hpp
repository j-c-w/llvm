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
    ConstraintOr(Types ... constraints) { add_constraint(constraints ...); }

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final;

private:
    struct SharedData
    {
        std::vector<std::vector<unsigned>> current_possibles_stack;
        unsigned                           stack_size;
    };

    class SpecializedOr : public Specialized<ValueType> //This crashed when some branches don't use all parameters!
                                                        //Not sure whether the commen still applies? Need to investigate!
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

    void add_constraint() { }

    template<typename TFirst, typename ... TRest> 
    void add_constraint(TFirst first, TRest ... rest);

    std::vector<std::shared_ptr<Constraint<LabelType,ValueType>>> constraints;
};

template<typename LabelType, typename ValueType>
template<typename TFirst, typename ... TRest> 
void ConstraintOr<LabelType,ValueType>::add_constraint(TFirst first, TRest ... rest)
{
    if(std::is_base_of<ConstraintOr<LabelType,ValueType>, TFirst>::value)
    {
        for(auto& constraint : ((ConstraintOr<LabelType,ValueType>&)(first)).constraints)
        {
            constraints.emplace_back(std::move(constraint));
        }
    }
    else
    {
        constraints.push_back(std::shared_ptr<Constraint<LabelType,ValueType>>(new TFirst(first)));
    }

    add_constraint(rest...);
}

#endif
