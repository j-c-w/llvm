#ifndef _CONSTRAINT_RANGE_HPP_
#define _CONSTRAINT_RANGE_HPP_
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/Constraints/Backends.hpp"
#include <memory>
#include <vector>

/* This constraint duplicated a constraint a specified amount of time.
   It searches for characters '*' and '+' in the variable names and replaces them with strings "[0]", ..., "[N-1]" and
   "[1]", ..., "[N]" for the different instanciations of the constraints.
   Nested ConstraintRange instances have to be separated by ConstraintPrefix to distinguish the wildcard symbols
   (the ones in the prefix then refer to the outer constraint). */
class ConstraintRange : public Constraint
{
public:
    template<typename T1, typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr>
    ConstraintRange(unsigned N, T1 c, unsigned o = 0)
      : ConstraintRange(N, ConstraintContainer(new T1(std::move(c))), o) { }

    ConstraintRange(unsigned N, ConstraintContainer c, unsigned o = 0);

    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final;

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                   std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    std::string print_index(const std::string& input, unsigned wild_pos, unsigned index);

    ConstraintContainer                       constraint;
    std::vector<std::string>                  labels;
    unsigned                                  local_size;
    std::vector<std::pair<unsigned,unsigned>> local_indices;
    std::vector<std::pair<unsigned,unsigned>> next_indices;
    std::vector<unsigned>                     global_indices;
    unsigned                                  offset;
    unsigned                                  size;
};

#endif
