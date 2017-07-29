#ifndef _PRINTSLOTS_HPP_
#define _PRINTSLOTS_HPP_
#include <string>
#include <vector>
#include <map>

namespace llvm {
class ModuleSlotTracker;
class Value;
}

/* This class is used to pretty print solutions that are output by the solver. */
class SolutionHierarchical
{
public:
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
