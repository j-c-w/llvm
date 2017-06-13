#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/LLVMSolver.hpp"
#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/Constraints/GenerateCOperator.hpp"
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

using namespace llvm;

unsigned create_shared_data = 0;

using Solution = std::vector<std::pair<std::string,Value*>>;

struct SolutionCluster
{
    std::vector<Value*>                   intersection;
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
            std::vector<Value*> intersection_vector;

            for(auto& intersect : cluster_spec.intersection)
            {
                auto find_intersect_it = std::find_if(solution.begin(), solution.end(),
                     [&intersect](const std::pair<std::string,Value*>& a) { return a.first == intersect; });

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

class ResearchReplacer : public ModulePass
{
public:
    static char ID;

    ResearchReplacer() : ModulePass(ID) {}

    bool runOnModule(Module& module) override;
};

bool ResearchReplacer::runOnModule(Module& module)
{
    create_shared_data = 0;
    ModuleSlotTracker slot_tracker(&module);

    auto constraint_scalar_reduction = ConstraintScalarReduction();
    auto constraint_histogram        = ConstraintHistogram();
    auto constraint_matrix_matrix    = ConstraintMatrixmatrix();
    auto constraint_sparse_mv        = ConstraintSparseMV();
    auto constraint_stencil          = ConstraintStencil(3);

    std::ofstream ofs("replace-report.txt");

    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            FunctionWrapper wrap(function);

            auto scalar_solutions    = LLVMSolver(constraint_scalar_reduction, wrap).all_solutions();
            auto histogram_solutions = LLVMSolver(constraint_histogram,        wrap).all_solutions();
            auto matrix_solutions    = LLVMSolver(constraint_matrix_matrix,    wrap).all_solutions();
            auto sparsemv_solutions  = LLVMSolver(constraint_sparse_mv,        wrap).all_solutions();
            auto stencil_solutions   = LLVMSolver(constraint_stencil,          wrap).all_solutions();

            auto clustered_solutions = cluster_solutions(
                    {{"scalars",  {"body_sese.begin", "body_sese.successor"}, scalar_solutions},
                     {"histos",   {"body_sese.begin", "body_sese.successor"}, histogram_solutions},
                     {"matrix",   {"body_sese.begin", "body_sese.successor"}, matrix_solutions},
                     {"sparsemv", {"body_sese.begin", "body_sese.successor"}, sparsemv_solutions},
                     {"stencils", {"body_sese.begin", "body_sese.successor"}, stencil_solutions}});

            if(clustered_solutions.size() > 0)
            {
                {
                std::stringstream str_str;
                raw_os_ostream out_stream(str_str);
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
                           <<SolutionHierarchical(solution, slot_tracker).print_pythonesque()<<"\n"
                           <<"END HISTOGRAM\n";
                    }

                    for(auto solution : clustered_solutions[i].solutions["scalars"])
                    {
                        ofs<<"BEGIN SCALAR REDUCTION\n"
                           <<SolutionHierarchical(solution, slot_tracker).print_pythonesque()<<"\n"
                           <<"END SCALAR REDUCTION\n";
                    }

                    for(auto solution : clustered_solutions[i].solutions["matrix"])
                    {
                        ofs<<"BEGIN MATRIX MULTIPLICATIONS\n"
                           <<SolutionHierarchical(solution, slot_tracker).print_pythonesque()<<"\n"
                           <<"END MATRIX MULTIPLICATIONS\n";
                    }

                    for(auto solution : clustered_solutions[i].solutions["sparsemv"])
                    {
                        ofs<<"BEGIN SPARSE MATRIX MULTIPLICATIONS\n"
                           <<SolutionHierarchical(solution, slot_tracker).print_pythonesque()<<"\n"
                           <<"END SPARSE MATRIX MULTIPLICATIONS\n";
                    }

                    for(auto solution : clustered_solutions[i].solutions["stencils"])
                    {
                        ofs<<"BEGIN STENCIL\n"
                           <<SolutionHierarchical(solution, slot_tracker).print_pythonesque()<<"\n"
                           <<"END STENCIL\n";
                    }

                    if(clustered_solutions[i].solutions["histos"].size() > 0 ||
                       clustered_solutions[i].solutions["scalars"].size() > 0)
                    {
                        ofs<<"BEGIN OPERATOR\n";
                        std::vector<Instruction*> outputs;

                        SESEFunction sese_function(function, clustered_solutions[i].intersection[0],
                                                             clustered_solutions[i].intersection[1]);

                        Function* function = sese_function.make_function();

                        std::vector<std::map<std::string,Value*>> scalar_solutions;
                        std::vector<std::map<std::string,Value*>> histo_solutions;

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

                        function->setName("op");
                        function->arg_begin()->setName("acc");
                        ofs<<print_pretty_c_operator(*function);
                        delete function;
                    }

                    ofs<<"END LOOP\n";
                }

                ofs<<"END FUNCTION TRANSFORMATION\n";
            }
        }
    }

    ofs.close();
    return false;
}

char ResearchReplacer::ID = 0;

static RegisterPass<ResearchReplacer> X("research-replacer", "Research replacer", false, false);

ModulePass *llvm::createResearchReplacerPass() {
  return new ResearchReplacer();
}
