#ifndef _PRINTSLOTS_HPP_
#define _PRINTSLOTS_HPP_
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include <string>

/* This class is used to pretty print solutions that are output by the solver. */
class SolutionHierarchical
{
public:
    SolutionHierarchical() { }

    SolutionHierarchical(std::vector<std::pair<std::string,llvm::Value*>> input,
                         llvm::ModuleSlotTracker& slot_tracker)
      : SolutionHierarchical(generate_strings(input, slot_tracker)) { }

    SolutionHierarchical(std::vector<std::pair<std::string,std::string>> input);

    static std::vector<std::pair<std::string,std::string>>
        generate_strings(std::vector<std::pair<std::string,llvm::Value*>> input, llvm::ModuleSlotTracker& slot_tracker);

    std::string print_pythonesque() const;

    std::string print_pure_function(std::vector<std::string> prefix);

private:
    std::map<std::string,SolutionHierarchical> dictionary;
    std::string                                printed_value;
};


#endif
