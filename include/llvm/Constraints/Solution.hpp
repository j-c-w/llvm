#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_
#include <climits>
#include <memory>
#include <vector>
#include <string>
#include <map>

class FunctionWrap;
class SolverAtom;

namespace llvm {
class ModuleSlotTracker;
class Function;
class Value;
}

// Constraints essentially only exist to generate instances of the above shown SolverAtom class and to map these
// instances to string labels.
class Constraint : public std::vector<std::string>
{
public:
    using std::vector<std::string>::vector;
    virtual ~Constraint() { };

    virtual void get_specials(const FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const = 0;
};

class Solution
{
public:
    operator llvm::Value* () const;
    operator std::map<std::string,llvm::Value*> () const;

    Solution operator[](std::string str) const;
    Solution operator[](unsigned idx) const;

    Solution get() const { return *this; }

    template<typename T1, typename ... Tn>
    Solution get(T1 t1, Tn ... tn) const { return operator[](t1).get(tn...); }

    Solution prune() const;
    std::string print_json(llvm::ModuleSlotTracker&) const;

    static std::vector<Solution> Find(const Constraint& constraint, llvm::Function& function,
                                      unsigned max_solutions = UINT_MAX);

private:
    Solution() : single_value(nullptr) {}
    Solution(std::vector<std::string> labels, std::vector<llvm::Value*> values);

    llvm::Value*                   single_value;
    std::vector<Solution>          vector_value;
    std::map<std::string,Solution> map_value;
};

#endif
