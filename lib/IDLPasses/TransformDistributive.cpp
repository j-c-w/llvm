#include "llvm/IDLPasses/Transforms.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <sstream>
#include <string>
#include <vector>

using namespace llvm;

void transform_distributive(Function& function, std::map<std::string,Value*> solution)
{
    Instruction* sum = dyn_cast<Instruction>(solution["value"]);

    std::vector<Value*> prod1_factors;
    std::vector<Value*> prod2_factors;
    std::vector<Value*> sum1_factors;
    std::vector<Value*> sum2_factors;
    std::vector<Value*> sum1_results;
    std::vector<Value*> sum2_results;

    for(unsigned i = 0; i < 4 ; i++)
    {
        std::stringstream index_sstr;
        index_sstr<<"["<<i<<"]";

        auto find_prod1_it   = solution.find("product1.factors"+index_sstr.str());
        auto find_prod2_it   = solution.find("product2.factors"+index_sstr.str());
        auto find_sum1_it    = solution.find("sum1.factors"+index_sstr.str());
        auto find_sum2_it    = solution.find("sum2.factors"+index_sstr.str());
        auto find_sumres1_it = solution.find("sum1.results"+index_sstr.str());
        auto find_sumres2_it = solution.find("sum2.results"+index_sstr.str());

        if(find_prod1_it != solution.end() && find_prod1_it->second != nullptr)
        {
            prod1_factors.push_back(find_prod1_it->second);
        }
        if(find_prod2_it != solution.end() && find_prod2_it->second != nullptr)
        {
            prod2_factors.push_back(find_prod2_it->second);
        }
        if(find_sum1_it != solution.end() && find_sum1_it->second != nullptr)
        {
            sum1_factors.push_back(find_sum1_it->second);
        }
        if(find_sum2_it != solution.end() && find_sum2_it->second != nullptr)
        {
            sum2_factors.push_back(find_sum2_it->second);
        }
        if(find_sumres1_it != solution.end() && find_sumres1_it->second != nullptr)
        {
            sum1_results.push_back(find_sumres1_it->second);
        }
        if(find_sumres2_it != solution.end() && find_sumres2_it->second != nullptr)
        {
            sum2_results.push_back(find_sumres2_it->second);
        }
    }

    std::reverse(prod1_factors.begin(), prod1_factors.end());
    std::reverse(prod2_factors.begin(), prod2_factors.end());
    std::reverse(sum1_factors.begin(), sum1_factors.end());
    std::reverse(sum2_factors.begin(), sum2_factors.end());
    std::reverse(sum1_results.begin(), sum1_results.end());
    std::reverse(sum2_results.begin(), sum2_results.end());

    if(prod1_factors.size() > 0 && prod2_factors.size() > 0 && sum1_factors.size() > 0 && sum2_factors.size() > 0 &&
       sum1_factors.size() == sum1_results.size() && sum2_factors.size() == sum2_results.size())
    {
        Type* int_type = sum->getType();

        BasicBlock::iterator insert_point(sum);
        IRBuilder<> builder(sum->getParent(), insert_point);

        Value* first_sum = ConstantInt::get(int_type, 0);
        for(unsigned j = 1; j <= sum1_factors.size()-1; j++)
            if(j==1 || dyn_cast<Instruction>(sum1_results[j])->getOpcode() == Instruction::Add)
                first_sum = builder.CreateAdd(builder.CreateSExtOrTrunc(sum1_factors[j], int_type), first_sum);
            else
                first_sum = builder.CreateSub(builder.CreateSExtOrTrunc(sum1_factors[j], int_type), first_sum);

        Value* second_sum = ConstantInt::get(int_type, 0);
        for(unsigned j = 1; j <= sum2_factors.size()-1; j++)
            if(j==1 || dyn_cast<Instruction>(sum2_results[j])->getOpcode() == Instruction::Add)
                first_sum = builder.CreateAdd(builder.CreateSExtOrTrunc(sum2_factors[j], int_type), second_sum);
            else
                first_sum = builder.CreateSub(builder.CreateSExtOrTrunc(sum2_factors[j], int_type), second_sum);

        int factor1 = 1;
        for(unsigned j = 1; j <= sum1_factors.size()-1; j++)
            if(dyn_cast<Instruction>(sum1_results[j])->getOpcode() == Instruction::Sub)
                factor1 *= -1;

        int factor2 = 1;
        for(unsigned j = 1; j <= sum2_factors.size()-1; j++)
            if(dyn_cast<Instruction>(sum2_results[j])->getOpcode() == Instruction::Sub)
                factor2 *= -1;

        Value* first_prod  = ConstantInt::get(int_type, factor1);
        Value* second_prod = ConstantInt::get(int_type, factor2);

        for(unsigned j = 1; j <= prod1_factors.size()-1; j++)
            first_prod = builder.CreateMul(builder.CreateSExtOrTrunc(prod1_factors[j], int_type), first_prod);

        for(unsigned j = 1; j <= prod2_factors.size()-1; j++)
            second_prod = builder.CreateMul(builder.CreateSExtOrTrunc(prod2_factors[j], int_type), second_prod);

        ReplaceInstWithValue(sum->getParent()->getInstList(), insert_point,
            builder.CreateAdd(
                builder.CreateAdd(first_sum, second_sum),
                builder.CreateMul(
                    builder.CreateAdd(first_prod, second_prod),
                    builder.CreateSExtOrTrunc(prod1_factors[0], int_type))));
    }
}
