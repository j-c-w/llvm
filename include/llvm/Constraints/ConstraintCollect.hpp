#ifndef _CONSTRAINT_COLLECT_HPP_
#define _CONSTRAINT_COLLECT_HPP_
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/Constraints/Backends.hpp"
#include <memory>
#include <string>
#include <vector>

class ConstraintCollect : public Constraint
{
public:
    template<typename T1, typename std::enable_if<std::is_base_of<Constraint,T1>::value>::type* = nullptr>
    ConstraintCollect(unsigned n, std::string prefix, T1 c)
      : ConstraintCollect(n, prefix, ConstraintContainer(new T1(std::move(c)))) { }

    ConstraintCollect(unsigned n, std::string prefix, ConstraintContainer c);

    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final;

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                  std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    ConstraintContainer      constraint;
    std::vector<std::string> labels;
    std::vector<unsigned>    global_indices;
    std::vector<unsigned>    local_indices;
    unsigned                 size;
};

#endif
