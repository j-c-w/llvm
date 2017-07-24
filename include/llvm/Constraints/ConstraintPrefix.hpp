#ifndef _CONSTRAINT_PREFIX_HPP_
#define _CONSTRAINT_PREFIX_HPP_
#include "llvm/Constraints/Constraint.hpp"
#include <memory>
#include <vector>

/* This constraint does nothing aside from changing the variable names in another constraint.
   More specifically it adds a prefix to each variable name.
   This is for general convenience but is also needed to separate nested ConstraintRange constraints. */
class ConstraintPrefix : public Constraint
{
public:
    template<typename T1, typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr>
    ConstraintPrefix(std::string prefix, T1 c)
      : ConstraintPrefix(prefix, ConstraintContainer(new T1(std::move(c)))) { }

    ConstraintPrefix(std::string prefix, ConstraintContainer c);

    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final;

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                   std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    ConstraintContainer      constraint;
    std::vector<std::string> labels;
};

class ConstraintRename : public Constraint
{
public:
    template<typename T1, typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr>
    ConstraintRename(T1 c, std::vector<std::pair<std::string,std::string>> renames)
      : ConstraintRename(ConstraintContainer(new T1(std::move(c))), renames) { }

    ConstraintRename(ConstraintContainer c, std::vector<std::pair<std::string,std::string>> renames);

    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final;

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                   std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    ConstraintContainer      constraint;
    std::vector<std::string> labels;
};

#endif
