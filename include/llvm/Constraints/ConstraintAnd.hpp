#ifndef _CONSTRAINT_AND_HPP_
#define _CONSTRAINT_AND_HPP_
#include "llvm/Constraints/Constraint.hpp"
#include <memory>
#include <vector>

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that all of them are
   satisfied at once. */
class ConstraintAnd : public Constraint
{
public:
  template<typename T1, typename T2,
        typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr,
        typename std::enable_if<std::is_base_of<Constraint,T2>::value>::type* = nullptr,
        typename std::enable_if< std::is_base_of<ConstraintAnd,T1>::value>::type* = nullptr,
        typename std::enable_if<!std::is_base_of<ConstraintAnd,T2>::value>::type* = nullptr>
    ConstraintAnd(T1 const1, T2 const2)
    {
        std::swap(constraints, const1.constraints);
        constraints.emplace_back(std::move(const2));
        generate_groupings();
    }

    template<typename T1, typename T2,
        typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr,
        typename std::enable_if<std::is_base_of<Constraint,T2>::value>::type* = nullptr,
        typename std::enable_if<std::is_base_of<ConstraintAnd,T1>::value>::type* = nullptr,
        typename std::enable_if<std::is_base_of<ConstraintAnd,T2>::value>::type* = nullptr>
    ConstraintAnd(T1 const1, T2 const2)
    {
        std::swap(constraints, const1.constraints);
        for(auto& element : const2.constraints)
            constraints.emplace_back(std::move(element));
        generate_groupings();
    }

    template<typename T1, typename T2,
        typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr,
        typename std::enable_if<std::is_base_of<Constraint,T2>::value>::type* = nullptr,
        typename std::enable_if<!std::is_base_of<ConstraintAnd,T1>::value>::type* = nullptr,
        typename std::enable_if< std::is_base_of<ConstraintAnd,T2>::value>::type* = nullptr>
    ConstraintAnd(T1 const1, T2 const2)
    {
        constraints.emplace_back(std::move(const1));
        for(auto& element : const2.constraints)
            constraints.emplace_back(std::move(element));
        generate_groupings();
    }

    template<typename T1, typename T2,
        typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr,
        typename std::enable_if<std::is_base_of<Constraint,T2>::value>::type* = nullptr,
        typename std::enable_if<!std::is_base_of<ConstraintAnd,T1>::value>::type* = nullptr,
        typename std::enable_if<!std::is_base_of<ConstraintAnd,T2>::value>::type* = nullptr>
    ConstraintAnd(T1 const1, T2 const2)
    {
        constraints.emplace_back(std::move(const1));
        constraints.emplace_back(std::move(const2));
        generate_groupings();
    }

    std::vector<Constraint::Label> get_labels(std::vector<Constraint::Label> use_vector = {}) const final;

    std::vector<SpecializedContainer>
             get_specials(FunctionWrapper& wrap, std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    void generate_groupings();

    std::vector<ConstraintContainer>   constraints;
    std::vector<Constraint::Label>     labels;
    std::vector<std::vector<unsigned>> groupings;
};

#endif
