#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/LLVMSolver.hpp"
#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Constraints/SeparateSESE.hpp"
#include "llvm/Support/raw_os_ostream.h"
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>

using Solution = std::vector<std::pair<std::string,llvm::Value*>>;

struct SolutionCluster
{
    std::vector<llvm::Value*>                   intersection;
    std::map<std::string,std::vector<Solution>> solutions;
};

struct ClusterSpecification
{
    std::string              idiom_name;
    std::vector<std::string> intersection;
    std::vector<Solution>    solutions;
};

// This function applies some postprocessing to bundle together reductions.
// The proper way would be to encode it in a new constraint specification "composed reduction" but this will do for now.
std::vector<SolutionCluster> cluster_solutions(std::vector<ClusterSpecification> inputs)
{
    std::vector<SolutionCluster> result;

    for(auto& cluster_spec : inputs)
    {
        for(const auto& solution : cluster_spec.solutions)
        {
            std::vector<llvm::Value*> intersection_vector;

            for(auto& intersect : cluster_spec.intersection)
            {
                auto find_intersect_it = std::find_if(solution.begin(), solution.end(),
                     [&intersect](const std::pair<std::string,llvm::Value*>& a) { return a.first == intersect; });

                if(find_intersect_it != solution.end() && find_intersect_it->second != nullptr)
                {
                    intersection_vector.push_back(find_intersect_it->second);
                }
            }

            if(intersection_vector.size() == cluster_spec.intersection.size())
            {
                bool new_entry = true;
                for(unsigned i = 0; i < result.size(); i++)
                {
                    if(result[i].intersection == intersection_vector)
                    {
                        new_entry = false;

                        result[i].solutions[cluster_spec.idiom_name].push_back(solution);
                    }
                }

                if(new_entry)
                {
                    result.push_back({intersection_vector, std::map<std::string,std::vector<Solution>>()});
                    result.back().solutions[cluster_spec.idiom_name].push_back(solution);
                }
            }
        }
    }

    return result;
}



class ReplacerPass : public llvm::ModulePass
{
public:
    static char ID;

    ReplacerPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override
    {
        std::ofstream ofs("replace-report.txt");

        for(llvm::Function& function : module.getFunctionList())
        {
            FunctionWrapper wrap(function);

            auto scalar_solutions    = LLVMSolver(ConstraintScalarReduction(wrap), wrap).all_solutions();
            auto histogram_solutions = LLVMSolver(ConstraintHistogram      (wrap), wrap).all_solutions();
            auto stencil_solutions   = LLVMSolver(Constraint3DStencil      (wrap), wrap).all_solutions();

            auto clustered_solutions = cluster_solutions(
                    {{"scalars",  {"body_sese.begin", "body_sese.successor"}, scalar_solutions},
                     {"histos",   {"body_sese.begin", "body_sese.successor"}, histogram_solutions},
                     {"stencils", {"loop3.begin",     "loop3.successor"},     stencil_solutions}});

            if(clustered_solutions.size() > 0)
            {
                {
                std::stringstream str_str;
                llvm::raw_os_ostream out_stream(str_str);
                function.printAsOperand(out_stream);
                out_stream.flush();
                str_str.flush();
                ofs<<"BEGIN FUNCTION TRANSFORMATION "<< str_str.str()<<"\n";
                }

                for(unsigned i = 0; i < clustered_solutions.size(); i++)
                {
                    ofs<<"BEGIN LOOP\n";

                    for(auto solution : clustered_solutions[i].solutions["histos"])
                    {
                        ofs<<"BEGIN HISTOGRAM\n"
                           <<SolutionHierarchical(solution).print_pythonesque()<<"\n"
                           <<"END HISTOGRAM\n";
                    }

                    for(auto solution : clustered_solutions[i].solutions["scalars"])
                    {
                        ofs<<"BEGIN SCALAR REDUCTION\n"
                           <<SolutionHierarchical(solution).print_pythonesque()<<"\n"
                           <<"END SCALAR REDUCTION\n";
                    }

                    for(auto solution : clustered_solutions[i].solutions["stencils"])
                    {
                        ofs<<"BEGIN STENCIL\n"
                           <<SolutionHierarchical(solution).print_pythonesque()<<"\n"
                           <<"END STENCIL\n";
                    }

                    ofs<<"BEGIN OPERATOR\n";
                    std::vector<llvm::Instruction*> outputs;


                    SESEFunction sese_function(function, clustered_solutions[i].intersection[0],
                                                         clustered_solutions[i].intersection[1]);

                    llvm::Function* function = sese_function.make_function();

                    std::vector<std::map<std::string,llvm::Value*>> scalar_solutions;
                    std::vector<std::map<std::string,llvm::Value*>> histo_solutions;

                    for(auto& solution : clustered_solutions[i].solutions["scalars"])
                    {
                        sese_function.transform_forw(solution);
                        scalar_solutions.emplace_back(solution.begin(), solution.end());
                    }

                    for(auto& solution : clustered_solutions[i].solutions["histos"])
                    {
                        sese_function.transform_forw(solution);
                        histo_solutions.emplace_back(solution.begin(), solution.end());
                    }

                    transform_reduction_operator(*function, scalar_solutions, histo_solutions);

                    {
                    std::stringstream str_str;
                    llvm::raw_os_ostream out_stream(str_str);
                    out_stream<<*function;
                    out_stream.flush();
                    str_str.flush();
                    ofs<<str_str.str();
                    }

                    delete function;
                    ofs<<"END LOOP\n";
                }

                ofs<<"END FUNCTION TRANSFORMATION\n";
            }
        }

        ofs.close();
        return false;
    }
};

char ReplacerPass::ID = 0;

llvm::ModulePass *llvm::createReplacerPass() {
  return new ReplacerPass();
}
