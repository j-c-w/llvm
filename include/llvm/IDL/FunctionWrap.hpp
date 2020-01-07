#ifndef _FUNCTION_WRAP_HH_
#define _FUNCTION_WRAP_HH_
#include <unordered_map>
#include <vector>

namespace llvm {
class Function;
class Value;
}

class Tree : public std::vector<std::pair<unsigned,Tree>> {};

/* This class is used as a wrapper around llvm::Function. */
class FunctionWrap : public std::vector<llvm::Value*>
{
public:
    FunctionWrap(llvm::Function& llvm_function);

    const Tree& derive_set(Tree(*func)(FunctionWrap&,const llvm::Function&)) {
        auto find_it = cache.find(func);
        if(find_it != cache.end())
            return find_it->second;

        return cache.insert({func, func(*this, function)}).first->second;
    }


    llvm::Function& function;

    std::vector<std::vector<unsigned>>              cfg;
    std::vector<std::vector<unsigned>>              rcfg;
    std::vector<std::vector<std::vector<unsigned>>> ocfg;
    std::vector<std::vector<std::vector<unsigned>>> rocfg;
    std::vector<std::vector<unsigned>>              dfg;
    std::vector<std::vector<unsigned>>              rdfg;
    std::vector<std::vector<std::vector<unsigned>>> odfg;
    std::vector<std::vector<std::vector<unsigned>>> rodfg;
    std::vector<std::vector<unsigned>>              cdg;
    std::vector<std::vector<unsigned>>              rcdg;
    std::vector<std::vector<unsigned>>              pdg;
    std::vector<std::vector<unsigned>>              rpdg;
    std::vector<std::vector<unsigned>>              blocks;
    std::vector<std::vector<unsigned>>              rblocks;

    static double stopped_time;

private:
    std::unordered_map<Tree(*)(FunctionWrap&,const llvm::Function&),Tree> cache;
};

#endif
