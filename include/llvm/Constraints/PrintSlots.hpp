#ifndef _PRINTSLOTS_HPP_
#define _PRINTSLOTS_HPP_
#include "llvm/Constraints/FunctionWrap.hpp"
#include <string>

/* This class is used to pretty print solutions that are output by the solver. */
class SolutionHierarchical
{
public:
    SolutionHierarchical() : dictionary{}, llvm_value(nullptr) { }

    SolutionHierarchical(std::vector<std::pair<std::string,llvm::Value*>> input);

    std::string print_pythonesque();

    std::string print_pure_function(std::vector<std::string> prefix);

private:
    std::map<std::string,SolutionHierarchical> dictionary;
    llvm::Value*                               llvm_value;
};


#endif
