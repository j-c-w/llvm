#include "llvm/Constraints/IdiomSpecifications.hpp"
#include "llvm/Constraints/ConstraintSpecializations.hpp"
#include "llvm/Constraints/Solution.hpp"

std::vector<Solution> DetectDistributive(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintAddInst("value"),
        new ConstraintFirstOperand("sum1.value", "value"),
        new ConstraintSecondOperand("sum2.value", "value"),
        new ConstraintIntegerType("sum1.value"),
        new ConstraintSame("sum1.value", "sum1.results[0]"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[0]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[1]", "sum1.results[0]"),
                new ConstraintSecondOperand("sum1.factors[0]", "sum1.results[0]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[0]", "sum1.results[0]"),
                new ConstraintSecondOperand("sum1.results[1]", "sum1.results[0]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[0]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[0]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[0]"),
                new ConstraintUnused("sum1.factors[0]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[0]", "sum1.factors[0]"),
            new ConstraintUnused("sum1.results[1]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[1]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[2]", "sum1.results[1]"),
                new ConstraintSecondOperand("sum1.factors[1]", "sum1.results[1]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[1]", "sum1.results[1]"),
                new ConstraintSecondOperand("sum1.results[2]", "sum1.results[1]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[1]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[1]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[1]"),
                new ConstraintUnused("sum1.factors[1]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[1]", "sum1.factors[1]"),
            new ConstraintUnused("sum1.results[2]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[2]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[3]", "sum1.results[2]"),
                new ConstraintSecondOperand("sum1.factors[2]", "sum1.results[2]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[2]", "sum1.results[2]"),
                new ConstraintSecondOperand("sum1.results[3]", "sum1.results[2]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[2]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[2]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[2]"),
                new ConstraintUnused("sum1.factors[2]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[2]", "sum1.factors[2]"),
            new ConstraintUnused("sum1.results[3]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[3]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[4]", "sum1.results[3]"),
                new ConstraintSecondOperand("sum1.factors[3]", "sum1.results[3]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[3]", "sum1.results[3]"),
                new ConstraintSecondOperand("sum1.results[4]", "sum1.results[3]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[3]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[3]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[3]"),
                new ConstraintUnused("sum1.factors[3]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[3]", "sum1.factors[3]"),
            new ConstraintUnused("sum1.results[4]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[4]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[5]", "sum1.results[4]"),
                new ConstraintSecondOperand("sum1.factors[4]", "sum1.results[4]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[4]", "sum1.results[4]"),
                new ConstraintSecondOperand("sum1.results[5]", "sum1.results[4]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[4]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[4]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[4]"),
                new ConstraintUnused("sum1.factors[4]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[4]", "sum1.factors[4]"),
            new ConstraintUnused("sum1.results[5]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[5]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[6]", "sum1.results[5]"),
                new ConstraintSecondOperand("sum1.factors[5]", "sum1.results[5]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[5]", "sum1.results[5]"),
                new ConstraintSecondOperand("sum1.results[6]", "sum1.results[5]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[5]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[5]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[5]"),
                new ConstraintUnused("sum1.factors[5]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[5]", "sum1.factors[5]"),
            new ConstraintUnused("sum1.results[6]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[6]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[7]", "sum1.results[6]"),
                new ConstraintSecondOperand("sum1.factors[6]", "sum1.results[6]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[6]", "sum1.results[6]"),
                new ConstraintSecondOperand("sum1.results[7]", "sum1.results[6]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[6]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[6]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[6]"),
                new ConstraintUnused("sum1.factors[6]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[6]", "sum1.factors[6]"),
            new ConstraintUnused("sum1.results[7]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum1.results[7]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.results[8]", "sum1.results[7]"),
                new ConstraintSecondOperand("sum1.factors[7]", "sum1.results[7]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum1.factors[7]", "sum1.results[7]"),
                new ConstraintSecondOperand("sum1.results[8]", "sum1.results[7]")})}),
            new ConstraintIntegerType("sum1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum1.results[7]"),
                new ConstraintSame("sum1.last_factor", "sum1.factors[7]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum1.results[7]"),
                new ConstraintUnused("sum1.factors[7]"),
                new ConstraintIntegerType("sum1.last_factor")})}),
            new ConstraintSame("sum1.results[7]", "sum1.factors[7]"),
            new ConstraintUnused("sum1.results[8]")})}),
        new ConstraintUnused("sum1.factors[8]"),
        new ConstraintSame("product1.value", "sum1.last_factor"),
        new ConstraintIntegerType("product1.value"),
        new ConstraintSame("product1.value", "product1.results[0]"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[0]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[1]", "product1.results[0]"),
                new ConstraintSecondOperand("product1.factors[0]", "product1.results[0]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[0]", "product1.results[0]"),
                new ConstraintSecondOperand("product1.results[1]", "product1.results[0]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[0]"),
                new ConstraintSame("product1.last_factor", "product1.factors[0]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[0]"),
                new ConstraintUnused("product1.factors[0]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[0]", "product1.factors[0]"),
            new ConstraintUnused("product1.results[1]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[1]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[2]", "product1.results[1]"),
                new ConstraintSecondOperand("product1.factors[1]", "product1.results[1]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[1]", "product1.results[1]"),
                new ConstraintSecondOperand("product1.results[2]", "product1.results[1]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[1]"),
                new ConstraintSame("product1.last_factor", "product1.factors[1]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[1]"),
                new ConstraintUnused("product1.factors[1]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[1]", "product1.factors[1]"),
            new ConstraintUnused("product1.results[2]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[2]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[3]", "product1.results[2]"),
                new ConstraintSecondOperand("product1.factors[2]", "product1.results[2]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[2]", "product1.results[2]"),
                new ConstraintSecondOperand("product1.results[3]", "product1.results[2]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[2]"),
                new ConstraintSame("product1.last_factor", "product1.factors[2]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[2]"),
                new ConstraintUnused("product1.factors[2]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[2]", "product1.factors[2]"),
            new ConstraintUnused("product1.results[3]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[3]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[4]", "product1.results[3]"),
                new ConstraintSecondOperand("product1.factors[3]", "product1.results[3]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[3]", "product1.results[3]"),
                new ConstraintSecondOperand("product1.results[4]", "product1.results[3]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[3]"),
                new ConstraintSame("product1.last_factor", "product1.factors[3]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[3]"),
                new ConstraintUnused("product1.factors[3]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[3]", "product1.factors[3]"),
            new ConstraintUnused("product1.results[4]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[4]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[5]", "product1.results[4]"),
                new ConstraintSecondOperand("product1.factors[4]", "product1.results[4]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[4]", "product1.results[4]"),
                new ConstraintSecondOperand("product1.results[5]", "product1.results[4]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[4]"),
                new ConstraintSame("product1.last_factor", "product1.factors[4]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[4]"),
                new ConstraintUnused("product1.factors[4]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[4]", "product1.factors[4]"),
            new ConstraintUnused("product1.results[5]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[5]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[6]", "product1.results[5]"),
                new ConstraintSecondOperand("product1.factors[5]", "product1.results[5]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[5]", "product1.results[5]"),
                new ConstraintSecondOperand("product1.results[6]", "product1.results[5]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[5]"),
                new ConstraintSame("product1.last_factor", "product1.factors[5]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[5]"),
                new ConstraintUnused("product1.factors[5]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[5]", "product1.factors[5]"),
            new ConstraintUnused("product1.results[6]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[6]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[7]", "product1.results[6]"),
                new ConstraintSecondOperand("product1.factors[6]", "product1.results[6]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[6]", "product1.results[6]"),
                new ConstraintSecondOperand("product1.results[7]", "product1.results[6]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[6]"),
                new ConstraintSame("product1.last_factor", "product1.factors[6]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[6]"),
                new ConstraintUnused("product1.factors[6]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[6]", "product1.factors[6]"),
            new ConstraintUnused("product1.results[7]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product1.results[7]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.results[8]", "product1.results[7]"),
                new ConstraintSecondOperand("product1.factors[7]", "product1.results[7]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product1.factors[7]", "product1.results[7]"),
                new ConstraintSecondOperand("product1.results[8]", "product1.results[7]")})}),
            new ConstraintIntegerType("product1.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product1.results[7]"),
                new ConstraintSame("product1.last_factor", "product1.factors[7]")}),
              new ConstraintAnd({
                new ConstraintUnused("product1.results[7]"),
                new ConstraintUnused("product1.factors[7]"),
                new ConstraintIntegerType("product1.last_factor")})}),
            new ConstraintSame("product1.results[7]", "product1.factors[7]"),
            new ConstraintUnused("product1.results[8]")})}),
        new ConstraintUnused("product1.factors[8]"),
        new ConstraintSame("product1.last_factor", "product2.last_factor"),
        new ConstraintIntegerType("sum2.value"),
        new ConstraintSame("sum2.value", "sum2.results[0]"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[0]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[1]", "sum2.results[0]"),
                new ConstraintSecondOperand("sum2.factors[0]", "sum2.results[0]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[0]", "sum2.results[0]"),
                new ConstraintSecondOperand("sum2.results[1]", "sum2.results[0]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[0]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[0]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[0]"),
                new ConstraintUnused("sum2.factors[0]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[0]", "sum2.factors[0]"),
            new ConstraintUnused("sum2.results[1]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[1]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[2]", "sum2.results[1]"),
                new ConstraintSecondOperand("sum2.factors[1]", "sum2.results[1]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[1]", "sum2.results[1]"),
                new ConstraintSecondOperand("sum2.results[2]", "sum2.results[1]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[1]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[1]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[1]"),
                new ConstraintUnused("sum2.factors[1]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[1]", "sum2.factors[1]"),
            new ConstraintUnused("sum2.results[2]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[2]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[3]", "sum2.results[2]"),
                new ConstraintSecondOperand("sum2.factors[2]", "sum2.results[2]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[2]", "sum2.results[2]"),
                new ConstraintSecondOperand("sum2.results[3]", "sum2.results[2]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[2]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[2]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[2]"),
                new ConstraintUnused("sum2.factors[2]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[2]", "sum2.factors[2]"),
            new ConstraintUnused("sum2.results[3]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[3]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[4]", "sum2.results[3]"),
                new ConstraintSecondOperand("sum2.factors[3]", "sum2.results[3]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[3]", "sum2.results[3]"),
                new ConstraintSecondOperand("sum2.results[4]", "sum2.results[3]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[3]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[3]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[3]"),
                new ConstraintUnused("sum2.factors[3]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[3]", "sum2.factors[3]"),
            new ConstraintUnused("sum2.results[4]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[4]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[5]", "sum2.results[4]"),
                new ConstraintSecondOperand("sum2.factors[4]", "sum2.results[4]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[4]", "sum2.results[4]"),
                new ConstraintSecondOperand("sum2.results[5]", "sum2.results[4]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[4]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[4]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[4]"),
                new ConstraintUnused("sum2.factors[4]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[4]", "sum2.factors[4]"),
            new ConstraintUnused("sum2.results[5]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[5]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[6]", "sum2.results[5]"),
                new ConstraintSecondOperand("sum2.factors[5]", "sum2.results[5]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[5]", "sum2.results[5]"),
                new ConstraintSecondOperand("sum2.results[6]", "sum2.results[5]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[5]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[5]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[5]"),
                new ConstraintUnused("sum2.factors[5]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[5]", "sum2.factors[5]"),
            new ConstraintUnused("sum2.results[6]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[6]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[7]", "sum2.results[6]"),
                new ConstraintSecondOperand("sum2.factors[6]", "sum2.results[6]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[6]", "sum2.results[6]"),
                new ConstraintSecondOperand("sum2.results[7]", "sum2.results[6]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[6]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[6]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[6]"),
                new ConstraintUnused("sum2.factors[6]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[6]", "sum2.factors[6]"),
            new ConstraintUnused("sum2.results[7]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintAddInst("sum2.results[7]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.results[8]", "sum2.results[7]"),
                new ConstraintSecondOperand("sum2.factors[7]", "sum2.results[7]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum2.factors[7]", "sum2.results[7]"),
                new ConstraintSecondOperand("sum2.results[8]", "sum2.results[7]")})}),
            new ConstraintIntegerType("sum2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("sum2.results[7]"),
                new ConstraintSame("sum2.last_factor", "sum2.factors[7]")}),
              new ConstraintAnd({
                new ConstraintUnused("sum2.results[7]"),
                new ConstraintUnused("sum2.factors[7]"),
                new ConstraintIntegerType("sum2.last_factor")})}),
            new ConstraintSame("sum2.results[7]", "sum2.factors[7]"),
            new ConstraintUnused("sum2.results[8]")})}),
        new ConstraintUnused("sum2.factors[8]"),
        new ConstraintSame("product2.value", "sum2.last_factor"),
        new ConstraintIntegerType("product2.value"),
        new ConstraintSame("product2.value", "product2.results[0]"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[0]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[1]", "product2.results[0]"),
                new ConstraintSecondOperand("product2.factors[0]", "product2.results[0]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[0]", "product2.results[0]"),
                new ConstraintSecondOperand("product2.results[1]", "product2.results[0]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[0]"),
                new ConstraintSame("product2.last_factor", "product2.factors[0]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[0]"),
                new ConstraintUnused("product2.factors[0]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[0]", "product2.factors[0]"),
            new ConstraintUnused("product2.results[1]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[1]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[2]", "product2.results[1]"),
                new ConstraintSecondOperand("product2.factors[1]", "product2.results[1]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[1]", "product2.results[1]"),
                new ConstraintSecondOperand("product2.results[2]", "product2.results[1]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[1]"),
                new ConstraintSame("product2.last_factor", "product2.factors[1]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[1]"),
                new ConstraintUnused("product2.factors[1]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[1]", "product2.factors[1]"),
            new ConstraintUnused("product2.results[2]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[2]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[3]", "product2.results[2]"),
                new ConstraintSecondOperand("product2.factors[2]", "product2.results[2]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[2]", "product2.results[2]"),
                new ConstraintSecondOperand("product2.results[3]", "product2.results[2]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[2]"),
                new ConstraintSame("product2.last_factor", "product2.factors[2]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[2]"),
                new ConstraintUnused("product2.factors[2]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[2]", "product2.factors[2]"),
            new ConstraintUnused("product2.results[3]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[3]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[4]", "product2.results[3]"),
                new ConstraintSecondOperand("product2.factors[3]", "product2.results[3]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[3]", "product2.results[3]"),
                new ConstraintSecondOperand("product2.results[4]", "product2.results[3]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[3]"),
                new ConstraintSame("product2.last_factor", "product2.factors[3]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[3]"),
                new ConstraintUnused("product2.factors[3]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[3]", "product2.factors[3]"),
            new ConstraintUnused("product2.results[4]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[4]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[5]", "product2.results[4]"),
                new ConstraintSecondOperand("product2.factors[4]", "product2.results[4]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[4]", "product2.results[4]"),
                new ConstraintSecondOperand("product2.results[5]", "product2.results[4]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[4]"),
                new ConstraintSame("product2.last_factor", "product2.factors[4]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[4]"),
                new ConstraintUnused("product2.factors[4]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[4]", "product2.factors[4]"),
            new ConstraintUnused("product2.results[5]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[5]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[6]", "product2.results[5]"),
                new ConstraintSecondOperand("product2.factors[5]", "product2.results[5]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[5]", "product2.results[5]"),
                new ConstraintSecondOperand("product2.results[6]", "product2.results[5]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[5]"),
                new ConstraintSame("product2.last_factor", "product2.factors[5]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[5]"),
                new ConstraintUnused("product2.factors[5]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[5]", "product2.factors[5]"),
            new ConstraintUnused("product2.results[6]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[6]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[7]", "product2.results[6]"),
                new ConstraintSecondOperand("product2.factors[6]", "product2.results[6]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[6]", "product2.results[6]"),
                new ConstraintSecondOperand("product2.results[7]", "product2.results[6]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[6]"),
                new ConstraintSame("product2.last_factor", "product2.factors[6]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[6]"),
                new ConstraintUnused("product2.factors[6]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[6]", "product2.factors[6]"),
            new ConstraintUnused("product2.results[7]")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintMulInst("product2.results[7]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.results[8]", "product2.results[7]"),
                new ConstraintSecondOperand("product2.factors[7]", "product2.results[7]")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("product2.factors[7]", "product2.results[7]"),
                new ConstraintSecondOperand("product2.results[8]", "product2.results[7]")})}),
            new ConstraintIntegerType("product2.last_factor")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintIntegerType("product2.results[7]"),
                new ConstraintSame("product2.last_factor", "product2.factors[7]")}),
              new ConstraintAnd({
                new ConstraintUnused("product2.results[7]"),
                new ConstraintUnused("product2.factors[7]"),
                new ConstraintIntegerType("product2.last_factor")})}),
            new ConstraintSame("product2.results[7]", "product2.factors[7]"),
            new ConstraintUnused("product2.results[8]")})}),
        new ConstraintUnused("product2.factors[8]")});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectHoistSelect(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintSelectInst("select"),
        new ConstraintSecondOperand("input1", "select"),
        new ConstraintFirstOperand("base", "input1"),
        new ConstraintThirdOperand("input2", "select"),
        new ConstraintFirstOperand("base", "input2"),
        new ConstraintGEPInst("input1"),
        new ConstraintGEPInst("input2")});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectReduction(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintCollect(32, "i",
          new ConstraintAnd({
            new ConstraintLoadInst("read[i].value"),
            new ConstraintFirstOperand("read[i].access_pointer", "read[i].value"),
            new ConstraintGEPInst("read[i].access_pointer"),
            new ConstraintFirstOperand("read[i].base_pointer", "read[i].access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].base_pointer")}),
              new ConstraintCFGDominateStrict("read[i].base_pointer", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].base_pointer")})}),
            new ConstraintSecondOperand("read[i].output_index", "read[i].access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("read[i].output_index"),
                new ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index")}),
              new ConstraintAnd({
                new ConstraintSExtInst("read[i].output_index"),
                new ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index")}),
              new ConstraintSame("read[i].offset_add.value", "read[i].output_index")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("read[i].offset_add.input", "read[i].offset_add.value"),
                new ConstraintUnused("read[i].offset_add.addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("read[i].offset_add.value"),
                  new ConstraintBitOrInst("read[i].offset_add.value"),
                  new ConstraintFAddInst("read[i].offset_add.value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.input", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.addend", "read[i].offset_add.value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.addend", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.input", "read[i].offset_add.value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("read[i].offset_add.value"),
                  new ConstraintFSubInst("read[i].offset_add.value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.input", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.addend", "read[i].offset_add.value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.addend", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.input", "read[i].offset_add.value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].offset_add.addend")}),
              new ConstraintCFGDominateStrict("read[i].offset_add.addend", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].offset_add.addend")})}),
            new ConstraintSame("read[i].stride_mul[0].value", "read[i].offset_add.input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"),
                new ConstraintUnused("read[i].stride_mul[0].multiplier")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintMulInst("read[i].stride_mul[0].value"),
                  new ConstraintLShiftInst("read[i].stride_mul[0].value"),
                  new ConstraintFMulInst("read[i].stride_mul[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")})})}),
              new ConstraintAnd({
                new ConstraintFDivInst("read[i].stride_mul[0].value"),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].stride_mul[0].multiplier")}),
              new ConstraintCFGDominateStrict("read[i].stride_mul[0].multiplier", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].stride_mul[0].multiplier")})}),
            new ConstraintSame("read[i].index_add[0].value", "read[i].stride_mul[0].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("read[i].index_add[0].input", "read[i].index_add[0].value"),
                new ConstraintUnused("read[i].index_add[0].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("read[i].index_add[0].value"),
                  new ConstraintBitOrInst("read[i].index_add[0].value"),
                  new ConstraintFAddInst("read[i].index_add[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].input", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].addend", "read[i].index_add[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].addend", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].input", "read[i].index_add[0].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("read[i].index_add[0].value"),
                  new ConstraintFSubInst("read[i].index_add[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].input", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].addend", "read[i].index_add[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].addend", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].input", "read[i].index_add[0].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("read[i].index_add[0].input"),
                new ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input")}),
              new ConstraintAnd({
                new ConstraintSExtInst("read[i].index_add[0].input"),
                new ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input")}),
              new ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintDistinct("read[i].index_add[0].input", "read[i].offset_add.value"),
                new ConstraintIntegerType("read[i].input_index[0]")}),
              new ConstraintAnd({
                new ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input"),
                new ConstraintIntegerType("read[i].offset_add.value")})}),
            new ConstraintSame("read[i].base_index", "read[i].index_add[0].addend"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].base_index")}),
              new ConstraintCFGDominateStrict("read[i].base_index", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].base_index")})}),
            new ConstraintSame("iterator", "read[i].input_index[0]"),
            new ConstraintCFGDominate("body_sese.begin", "read[i].value"),
            new ConstraintCFGPostdom("body_sese.end", "read[i].value")})),
        new ConstraintLoadInst("read[0].value"),
        new ConstraintSame("update_expr.input[0]", "old_value"),
        new ConstraintSame("update_expr.input[1]", "read[0].value"),
        new ConstraintSame("update_expr.input[2]", "read[1].value"),
        new ConstraintSame("update_expr.input[3]", "read[2].value"),
        new ConstraintSame("update_expr.input[4]", "read[3].value"),
        new ConstraintSame("update_expr.input[5]", "read[4].value"),
        new ConstraintSame("update_expr.input[6]", "read[5].value"),
        new ConstraintSame("update_expr.input[7]", "read[6].value"),
        new ConstraintSame("update_expr.input[8]", "read[7].value"),
        new ConstraintSame("update_expr.input[9]", "read[8].value"),
        new ConstraintSame("update_expr.input[10]", "read[9].value"),
        new ConstraintSame("update_expr.input[11]", "read[10].value"),
        new ConstraintSame("update_expr.input[12]", "read[11].value"),
        new ConstraintSame("update_expr.input[13]", "read[12].value"),
        new ConstraintSame("update_expr.input[14]", "read[13].value"),
        new ConstraintSame("update_expr.input[15]", "read[14].value"),
        new ConstraintSame("update_expr.input[16]", "read[15].value"),
        new ConstraintSame("update_expr.input[17]", "read[16].value"),
        new ConstraintSame("update_expr.input[18]", "read[17].value"),
        new ConstraintSame("update_expr.input[19]", "read[18].value"),
        new ConstraintSame("update_expr.input[20]", "read[19].value"),
        new ConstraintSame("update_expr.input[21]", "read[20].value"),
        new ConstraintSame("update_expr.input[22]", "read[21].value"),
        new ConstraintSame("update_expr.input[23]", "read[22].value"),
        new ConstraintSame("update_expr.input[24]", "read[23].value"),
        new ConstraintSame("update_expr.input[25]", "read[24].value"),
        new ConstraintSame("update_expr.input[26]", "read[25].value"),
        new ConstraintSame("update_expr.input[27]", "read[26].value"),
        new ConstraintSame("update_expr.input[28]", "read[27].value"),
        new ConstraintSame("update_expr.input[29]", "read[28].value"),
        new ConstraintSame("update_expr.input[30]", "read[29].value"),
        new ConstraintSame("update_expr.input[31]", "read[30].value"),
        new ConstraintIncomingValue("update_expr.output", "body_sese.end", "old_value"),
        new ConstraintDistinct("old_value", "iterator"),
        new ConstraintCollect(4, "i",
          new ConstraintCFGEdge("update_expr.origin[i]", "body_sese.begin")),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("update_expr.constants[i].value")}),
              new ConstraintCFGDominateStrict("update_expr.constants[i].value", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("update_expr.constants[i].value")})}),
            new ConstraintNotNumericConstant("update_expr.constants[i].value"),
            new ConstraintDFGEdge("update_expr.constants[i].value", "update_expr.constants[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "update_expr.constants[i].use")})),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintCFGDominate("begin", "update_expr.restrictions[i].value"),
            new ConstraintCFGDominateStrict("update_expr.restrictions[i].value", "body_sese.begin"),
            new ConstraintDFGEdge("update_expr.restrictions[i].value", "update_expr.restrictions[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "update_expr.restrictions[i].use")})),
        new ConstraintPDGDominate({
          std::vector<std::string>{
            "update_expr.restrictions[0].value", "update_expr.restrictions[1].value",
            "update_expr.restrictions[2].value", "update_expr.restrictions[3].value",
            "update_expr.restrictions[4].value", "update_expr.restrictions[5].value",
            "update_expr.restrictions[6].value", "update_expr.restrictions[7].value",
            "update_expr.restrictions[8].value", "update_expr.restrictions[9].value",
            "update_expr.restrictions[10].value", "update_expr.restrictions[11].value",
            "update_expr.restrictions[12].value", "update_expr.restrictions[13].value",
            "update_expr.restrictions[14].value", "update_expr.restrictions[15].value",
            "update_expr.restrictions[16].value", "update_expr.restrictions[17].value",
            "update_expr.restrictions[18].value", "update_expr.restrictions[19].value",
            "update_expr.restrictions[20].value", "update_expr.restrictions[21].value",
            "update_expr.restrictions[22].value", "update_expr.restrictions[23].value",
            "update_expr.restrictions[24].value", "update_expr.restrictions[25].value",
            "update_expr.restrictions[26].value", "update_expr.restrictions[27].value",
            "update_expr.restrictions[28].value", "update_expr.restrictions[29].value",
            "update_expr.restrictions[30].value", "update_expr.restrictions[31].value",
            "update_expr.restrictions[32].value", "update_expr.restrictions[33].value",
            "update_expr.restrictions[34].value", "update_expr.restrictions[35].value",
            "update_expr.restrictions[36].value", "update_expr.restrictions[37].value",
            "update_expr.restrictions[38].value", "update_expr.restrictions[39].value",
            "update_expr.restrictions[40].value", "update_expr.restrictions[41].value",
            "update_expr.restrictions[42].value", "update_expr.restrictions[43].value",
            "update_expr.restrictions[44].value", "update_expr.restrictions[45].value",
            "update_expr.restrictions[46].value", "update_expr.restrictions[47].value",
            "update_expr.restrictions[48].value", "update_expr.restrictions[49].value",
            "update_expr.restrictions[50].value", "update_expr.restrictions[51].value",
            "update_expr.restrictions[52].value", "update_expr.restrictions[53].value",
            "update_expr.restrictions[54].value", "update_expr.restrictions[55].value",
            "update_expr.restrictions[56].value", "update_expr.restrictions[57].value",
            "update_expr.restrictions[58].value", "update_expr.restrictions[59].value",
            "update_expr.restrictions[60].value", "update_expr.restrictions[61].value",
            "update_expr.restrictions[62].value", "update_expr.restrictions[63].value"},
          std::vector<std::string>{
            "update_expr.origin[0]", "update_expr.origin[1]", "update_expr.origin[2]",
            "update_expr.origin[3]", "update_expr.constants[0].value",
            "update_expr.constants[1].value", "update_expr.constants[2].value",
            "update_expr.constants[3].value", "update_expr.constants[4].value",
            "update_expr.constants[5].value", "update_expr.constants[6].value",
            "update_expr.constants[7].value", "update_expr.constants[8].value",
            "update_expr.constants[9].value", "update_expr.constants[10].value",
            "update_expr.constants[11].value", "update_expr.constants[12].value",
            "update_expr.constants[13].value", "update_expr.constants[14].value",
            "update_expr.constants[15].value", "update_expr.constants[16].value",
            "update_expr.constants[17].value", "update_expr.constants[18].value",
            "update_expr.constants[19].value", "update_expr.constants[20].value",
            "update_expr.constants[21].value", "update_expr.constants[22].value",
            "update_expr.constants[23].value", "update_expr.constants[24].value",
            "update_expr.constants[25].value", "update_expr.constants[26].value",
            "update_expr.constants[27].value", "update_expr.constants[28].value",
            "update_expr.constants[29].value", "update_expr.constants[30].value",
            "update_expr.constants[31].value", "update_expr.constants[32].value",
            "update_expr.constants[33].value", "update_expr.constants[34].value",
            "update_expr.constants[35].value", "update_expr.constants[36].value",
            "update_expr.constants[37].value", "update_expr.constants[38].value",
            "update_expr.constants[39].value", "update_expr.constants[40].value",
            "update_expr.constants[41].value", "update_expr.constants[42].value",
            "update_expr.constants[43].value", "update_expr.constants[44].value",
            "update_expr.constants[45].value", "update_expr.constants[46].value",
            "update_expr.constants[47].value", "update_expr.constants[48].value",
            "update_expr.constants[49].value", "update_expr.constants[50].value",
            "update_expr.constants[51].value", "update_expr.constants[52].value",
            "update_expr.constants[53].value", "update_expr.constants[54].value",
            "update_expr.constants[55].value", "update_expr.constants[56].value",
            "update_expr.constants[57].value", "update_expr.constants[58].value",
            "update_expr.constants[59].value", "update_expr.constants[60].value",
            "update_expr.constants[61].value", "update_expr.constants[62].value",
            "update_expr.constants[63].value", "update_expr.input[0]",
            "update_expr.input[1]", "update_expr.input[2]", "update_expr.input[3]",
            "update_expr.input[4]", "update_expr.input[5]", "update_expr.input[6]",
            "update_expr.input[7]", "update_expr.input[8]", "update_expr.input[9]",
            "update_expr.input[10]", "update_expr.input[11]", "update_expr.input[12]",
            "update_expr.input[13]", "update_expr.input[14]", "update_expr.input[15]",
            "update_expr.input[16]", "update_expr.input[17]", "update_expr.input[18]",
            "update_expr.input[19]", "update_expr.input[20]", "update_expr.input[21]",
            "update_expr.input[22]", "update_expr.input[23]", "update_expr.input[24]",
            "update_expr.input[25]", "update_expr.input[26]", "update_expr.input[27]",
            "update_expr.input[28]", "update_expr.input[29]", "update_expr.input[30]",
            "update_expr.input[31]"},
          std::vector<std::string>{
            "update_expr.output"}}),
        new ConstraintCollect(4, "i",
          new ConstraintAnd({
            new ConstraintDFGEdge("incoming[i]", "old_value"),
            new ConstraintDistinct("incoming[i]", "update_expr.output")}))});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectAXPY(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintLoadInst("src.value"),
        new ConstraintFirstOperand("src.access_pointer", "src.value"),
        new ConstraintGEPInst("src.access_pointer"),
        new ConstraintFirstOperand("src.base_pointer", "src.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src.base_pointer")}),
          new ConstraintCFGDominateStrict("src.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src.base_pointer")})}),
        new ConstraintSecondOperand("src.output_index", "src.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("src.output_index"),
            new ConstraintDFGEdge("src.offset_add.value", "src.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("src.output_index"),
            new ConstraintDFGEdge("src.offset_add.value", "src.output_index")}),
          new ConstraintSame("src.offset_add.value", "src.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src.offset_add.input", "src.offset_add.value"),
            new ConstraintUnused("src.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("src.offset_add.value"),
              new ConstraintBitOrInst("src.offset_add.value"),
              new ConstraintFAddInst("src.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.offset_add.input", "src.offset_add.value"),
                new ConstraintSecondOperand("src.offset_add.addend", "src.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src.offset_add.addend", "src.offset_add.value"),
                new ConstraintSecondOperand("src.offset_add.input", "src.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("src.offset_add.value"),
              new ConstraintFSubInst("src.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.offset_add.input", "src.offset_add.value"),
                new ConstraintSecondOperand("src.offset_add.addend", "src.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src.offset_add.addend", "src.offset_add.value"),
                new ConstraintSecondOperand("src.offset_add.input", "src.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src.offset_add.addend")}),
          new ConstraintCFGDominateStrict("src.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src.offset_add.addend")})}),
        new ConstraintSame("src.stride_mul[0].value", "src.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src.stride_mul[0].input", "src.stride_mul[0].value"),
            new ConstraintUnused("src.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("src.stride_mul[0].value"),
              new ConstraintLShiftInst("src.stride_mul[0].value"),
              new ConstraintFMulInst("src.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.stride_mul[0].input", "src.stride_mul[0].value"),
                new ConstraintSecondOperand("src.stride_mul[0].multiplier", "src.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src.stride_mul[0].multiplier", "src.stride_mul[0].value"),
                new ConstraintSecondOperand("src.stride_mul[0].input", "src.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("src.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.stride_mul[0].input", "src.stride_mul[0].value"),
                new ConstraintSecondOperand("src.stride_mul[0].multiplier", "src.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src.stride_mul[0].multiplier", "src.stride_mul[0].value"),
                new ConstraintSecondOperand("src.stride_mul[0].input", "src.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("src.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src.stride_mul[0].multiplier")})}),
        new ConstraintSame("src.index_add[0].value", "src.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src.index_add[0].input", "src.index_add[0].value"),
            new ConstraintUnused("src.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("src.index_add[0].value"),
              new ConstraintBitOrInst("src.index_add[0].value"),
              new ConstraintFAddInst("src.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.index_add[0].input", "src.index_add[0].value"),
                new ConstraintSecondOperand("src.index_add[0].addend", "src.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src.index_add[0].addend", "src.index_add[0].value"),
                new ConstraintSecondOperand("src.index_add[0].input", "src.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("src.index_add[0].value"),
              new ConstraintFSubInst("src.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.index_add[0].input", "src.index_add[0].value"),
                new ConstraintSecondOperand("src.index_add[0].addend", "src.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src.index_add[0].addend", "src.index_add[0].value"),
                new ConstraintSecondOperand("src.index_add[0].input", "src.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("src.index_add[0].input"),
            new ConstraintDFGEdge("src.input_index[0]", "src.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("src.index_add[0].input"),
            new ConstraintDFGEdge("src.input_index[0]", "src.index_add[0].input")}),
          new ConstraintSame("src.input_index[0]", "src.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("src.index_add[0].input", "src.offset_add.value"),
            new ConstraintIntegerType("src.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("src.input_index[0]", "src.index_add[0].input"),
            new ConstraintIntegerType("src.offset_add.value")})}),
        new ConstraintSame("src.base_index", "src.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src.base_index")}),
          new ConstraintCFGDominateStrict("src.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src.base_index")})}),
        new ConstraintSame("iterator", "src.input_index[0]"),
        new ConstraintStoreInst("dst.store"),
        new ConstraintSecondOperand("dst.access_pointer", "dst.store"),
        new ConstraintGEPInst("dst.access_pointer"),
        new ConstraintFirstOperand("dst.base_pointer", "dst.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("dst.base_pointer")}),
          new ConstraintCFGDominateStrict("dst.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("dst.base_pointer")})}),
        new ConstraintSecondOperand("dst.output_index", "dst.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("dst.output_index"),
            new ConstraintDFGEdge("dst.offset_add.value", "dst.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("dst.output_index"),
            new ConstraintDFGEdge("dst.offset_add.value", "dst.output_index")}),
          new ConstraintSame("dst.offset_add.value", "dst.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("dst.offset_add.input", "dst.offset_add.value"),
            new ConstraintUnused("dst.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("dst.offset_add.value"),
              new ConstraintBitOrInst("dst.offset_add.value"),
              new ConstraintFAddInst("dst.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.offset_add.input", "dst.offset_add.value"),
                new ConstraintSecondOperand("dst.offset_add.addend", "dst.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.offset_add.addend", "dst.offset_add.value"),
                new ConstraintSecondOperand("dst.offset_add.input", "dst.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("dst.offset_add.value"),
              new ConstraintFSubInst("dst.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.offset_add.input", "dst.offset_add.value"),
                new ConstraintSecondOperand("dst.offset_add.addend", "dst.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.offset_add.addend", "dst.offset_add.value"),
                new ConstraintSecondOperand("dst.offset_add.input", "dst.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("dst.offset_add.addend")}),
          new ConstraintCFGDominateStrict("dst.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("dst.offset_add.addend")})}),
        new ConstraintSame("dst.stride_mul[0].value", "dst.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("dst.stride_mul[0].input", "dst.stride_mul[0].value"),
            new ConstraintUnused("dst.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("dst.stride_mul[0].value"),
              new ConstraintLShiftInst("dst.stride_mul[0].value"),
              new ConstraintFMulInst("dst.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.stride_mul[0].input", "dst.stride_mul[0].value"),
                new ConstraintSecondOperand("dst.stride_mul[0].multiplier", "dst.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.stride_mul[0].multiplier", "dst.stride_mul[0].value"),
                new ConstraintSecondOperand("dst.stride_mul[0].input", "dst.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("dst.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.stride_mul[0].input", "dst.stride_mul[0].value"),
                new ConstraintSecondOperand("dst.stride_mul[0].multiplier", "dst.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.stride_mul[0].multiplier", "dst.stride_mul[0].value"),
                new ConstraintSecondOperand("dst.stride_mul[0].input", "dst.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("dst.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("dst.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("dst.stride_mul[0].multiplier")})}),
        new ConstraintSame("dst.index_add[0].value", "dst.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("dst.index_add[0].input", "dst.index_add[0].value"),
            new ConstraintUnused("dst.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("dst.index_add[0].value"),
              new ConstraintBitOrInst("dst.index_add[0].value"),
              new ConstraintFAddInst("dst.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.index_add[0].input", "dst.index_add[0].value"),
                new ConstraintSecondOperand("dst.index_add[0].addend", "dst.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.index_add[0].addend", "dst.index_add[0].value"),
                new ConstraintSecondOperand("dst.index_add[0].input", "dst.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("dst.index_add[0].value"),
              new ConstraintFSubInst("dst.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.index_add[0].input", "dst.index_add[0].value"),
                new ConstraintSecondOperand("dst.index_add[0].addend", "dst.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("dst.index_add[0].addend", "dst.index_add[0].value"),
                new ConstraintSecondOperand("dst.index_add[0].input", "dst.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("dst.index_add[0].input"),
            new ConstraintDFGEdge("dst.input_index[0]", "dst.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("dst.index_add[0].input"),
            new ConstraintDFGEdge("dst.input_index[0]", "dst.index_add[0].input")}),
          new ConstraintSame("dst.input_index[0]", "dst.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("dst.index_add[0].input", "dst.offset_add.value"),
            new ConstraintIntegerType("dst.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("dst.input_index[0]", "dst.index_add[0].input"),
            new ConstraintIntegerType("dst.offset_add.value")})}),
        new ConstraintSame("dst.base_index", "dst.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("dst.base_index")}),
          new ConstraintCFGDominateStrict("dst.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("dst.base_index")})}),
        new ConstraintSame("iterator", "dst.input_index[0]"),
        new ConstraintDistinct("src.base_pointer", "dst.base_pointer"),
        new ConstraintLoadInst("sum.input"),
        new ConstraintFirstOperand("dst.access_pointer", "sum.input"),
        new ConstraintFirstOperand("sum.value", "dst.store"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("sum.input", "sum.value"),
            new ConstraintUnused("sum.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("sum.value"),
              new ConstraintBitOrInst("sum.value"),
              new ConstraintFAddInst("sum.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum.input", "sum.value"),
                new ConstraintSecondOperand("sum.addend", "sum.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum.addend", "sum.value"),
                new ConstraintSecondOperand("sum.input", "sum.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("sum.value"),
              new ConstraintFSubInst("sum.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("sum.input", "sum.value"),
                new ConstraintSecondOperand("sum.addend", "sum.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("sum.addend", "sum.value"),
                new ConstraintSecondOperand("sum.input", "sum.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src.value", "sum.addend"),
            new ConstraintUnused("alpha")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("sum.addend"),
              new ConstraintLShiftInst("sum.addend"),
              new ConstraintFMulInst("sum.addend")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.value", "sum.addend"),
                new ConstraintSecondOperand("alpha", "sum.addend")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("alpha", "sum.addend"),
                new ConstraintSecondOperand("src.value", "sum.addend")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("sum.addend"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src.value", "sum.addend"),
                new ConstraintSecondOperand("alpha", "sum.addend")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("alpha", "sum.addend"),
                new ConstraintSecondOperand("src.value", "sum.addend")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("alpha")}),
          new ConstraintCFGDominateStrict("alpha", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("alpha")})})});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectDOT(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintLoadInst("src1.value"),
        new ConstraintFirstOperand("src1.access_pointer", "src1.value"),
        new ConstraintGEPInst("src1.access_pointer"),
        new ConstraintFirstOperand("src1.base_pointer", "src1.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src1.base_pointer")}),
          new ConstraintCFGDominateStrict("src1.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src1.base_pointer")})}),
        new ConstraintSecondOperand("src1.output_index", "src1.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("src1.output_index"),
            new ConstraintDFGEdge("src1.offset_add.value", "src1.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("src1.output_index"),
            new ConstraintDFGEdge("src1.offset_add.value", "src1.output_index")}),
          new ConstraintSame("src1.offset_add.value", "src1.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src1.offset_add.input", "src1.offset_add.value"),
            new ConstraintUnused("src1.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("src1.offset_add.value"),
              new ConstraintBitOrInst("src1.offset_add.value"),
              new ConstraintFAddInst("src1.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.offset_add.input", "src1.offset_add.value"),
                new ConstraintSecondOperand("src1.offset_add.addend", "src1.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.offset_add.addend", "src1.offset_add.value"),
                new ConstraintSecondOperand("src1.offset_add.input", "src1.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("src1.offset_add.value"),
              new ConstraintFSubInst("src1.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.offset_add.input", "src1.offset_add.value"),
                new ConstraintSecondOperand("src1.offset_add.addend", "src1.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.offset_add.addend", "src1.offset_add.value"),
                new ConstraintSecondOperand("src1.offset_add.input", "src1.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src1.offset_add.addend")}),
          new ConstraintCFGDominateStrict("src1.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src1.offset_add.addend")})}),
        new ConstraintSame("src1.stride_mul[0].value", "src1.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src1.stride_mul[0].input", "src1.stride_mul[0].value"),
            new ConstraintUnused("src1.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("src1.stride_mul[0].value"),
              new ConstraintLShiftInst("src1.stride_mul[0].value"),
              new ConstraintFMulInst("src1.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.stride_mul[0].input", "src1.stride_mul[0].value"),
                new ConstraintSecondOperand("src1.stride_mul[0].multiplier", "src1.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.stride_mul[0].multiplier", "src1.stride_mul[0].value"),
                new ConstraintSecondOperand("src1.stride_mul[0].input", "src1.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("src1.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.stride_mul[0].input", "src1.stride_mul[0].value"),
                new ConstraintSecondOperand("src1.stride_mul[0].multiplier", "src1.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.stride_mul[0].multiplier", "src1.stride_mul[0].value"),
                new ConstraintSecondOperand("src1.stride_mul[0].input", "src1.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src1.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("src1.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src1.stride_mul[0].multiplier")})}),
        new ConstraintSame("src1.index_add[0].value", "src1.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src1.index_add[0].input", "src1.index_add[0].value"),
            new ConstraintUnused("src1.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("src1.index_add[0].value"),
              new ConstraintBitOrInst("src1.index_add[0].value"),
              new ConstraintFAddInst("src1.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.index_add[0].input", "src1.index_add[0].value"),
                new ConstraintSecondOperand("src1.index_add[0].addend", "src1.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.index_add[0].addend", "src1.index_add[0].value"),
                new ConstraintSecondOperand("src1.index_add[0].input", "src1.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("src1.index_add[0].value"),
              new ConstraintFSubInst("src1.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.index_add[0].input", "src1.index_add[0].value"),
                new ConstraintSecondOperand("src1.index_add[0].addend", "src1.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src1.index_add[0].addend", "src1.index_add[0].value"),
                new ConstraintSecondOperand("src1.index_add[0].input", "src1.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("src1.index_add[0].input"),
            new ConstraintDFGEdge("src1.input_index[0]", "src1.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("src1.index_add[0].input"),
            new ConstraintDFGEdge("src1.input_index[0]", "src1.index_add[0].input")}),
          new ConstraintSame("src1.input_index[0]", "src1.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("src1.index_add[0].input", "src1.offset_add.value"),
            new ConstraintIntegerType("src1.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("src1.input_index[0]", "src1.index_add[0].input"),
            new ConstraintIntegerType("src1.offset_add.value")})}),
        new ConstraintSame("src1.base_index", "src1.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src1.base_index")}),
          new ConstraintCFGDominateStrict("src1.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src1.base_index")})}),
        new ConstraintSame("iterator", "src1.input_index[0]"),
        new ConstraintLoadInst("src2.value"),
        new ConstraintFirstOperand("src2.access_pointer", "src2.value"),
        new ConstraintGEPInst("src2.access_pointer"),
        new ConstraintFirstOperand("src2.base_pointer", "src2.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src2.base_pointer")}),
          new ConstraintCFGDominateStrict("src2.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src2.base_pointer")})}),
        new ConstraintSecondOperand("src2.output_index", "src2.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("src2.output_index"),
            new ConstraintDFGEdge("src2.offset_add.value", "src2.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("src2.output_index"),
            new ConstraintDFGEdge("src2.offset_add.value", "src2.output_index")}),
          new ConstraintSame("src2.offset_add.value", "src2.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src2.offset_add.input", "src2.offset_add.value"),
            new ConstraintUnused("src2.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("src2.offset_add.value"),
              new ConstraintBitOrInst("src2.offset_add.value"),
              new ConstraintFAddInst("src2.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.offset_add.input", "src2.offset_add.value"),
                new ConstraintSecondOperand("src2.offset_add.addend", "src2.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.offset_add.addend", "src2.offset_add.value"),
                new ConstraintSecondOperand("src2.offset_add.input", "src2.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("src2.offset_add.value"),
              new ConstraintFSubInst("src2.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.offset_add.input", "src2.offset_add.value"),
                new ConstraintSecondOperand("src2.offset_add.addend", "src2.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.offset_add.addend", "src2.offset_add.value"),
                new ConstraintSecondOperand("src2.offset_add.input", "src2.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src2.offset_add.addend")}),
          new ConstraintCFGDominateStrict("src2.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src2.offset_add.addend")})}),
        new ConstraintSame("src2.stride_mul[0].value", "src2.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src2.stride_mul[0].input", "src2.stride_mul[0].value"),
            new ConstraintUnused("src2.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("src2.stride_mul[0].value"),
              new ConstraintLShiftInst("src2.stride_mul[0].value"),
              new ConstraintFMulInst("src2.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.stride_mul[0].input", "src2.stride_mul[0].value"),
                new ConstraintSecondOperand("src2.stride_mul[0].multiplier", "src2.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.stride_mul[0].multiplier", "src2.stride_mul[0].value"),
                new ConstraintSecondOperand("src2.stride_mul[0].input", "src2.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("src2.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.stride_mul[0].input", "src2.stride_mul[0].value"),
                new ConstraintSecondOperand("src2.stride_mul[0].multiplier", "src2.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.stride_mul[0].multiplier", "src2.stride_mul[0].value"),
                new ConstraintSecondOperand("src2.stride_mul[0].input", "src2.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src2.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("src2.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src2.stride_mul[0].multiplier")})}),
        new ConstraintSame("src2.index_add[0].value", "src2.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("src2.index_add[0].input", "src2.index_add[0].value"),
            new ConstraintUnused("src2.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("src2.index_add[0].value"),
              new ConstraintBitOrInst("src2.index_add[0].value"),
              new ConstraintFAddInst("src2.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.index_add[0].input", "src2.index_add[0].value"),
                new ConstraintSecondOperand("src2.index_add[0].addend", "src2.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.index_add[0].addend", "src2.index_add[0].value"),
                new ConstraintSecondOperand("src2.index_add[0].input", "src2.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("src2.index_add[0].value"),
              new ConstraintFSubInst("src2.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.index_add[0].input", "src2.index_add[0].value"),
                new ConstraintSecondOperand("src2.index_add[0].addend", "src2.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("src2.index_add[0].addend", "src2.index_add[0].value"),
                new ConstraintSecondOperand("src2.index_add[0].input", "src2.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("src2.index_add[0].input"),
            new ConstraintDFGEdge("src2.input_index[0]", "src2.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("src2.index_add[0].input"),
            new ConstraintDFGEdge("src2.input_index[0]", "src2.index_add[0].input")}),
          new ConstraintSame("src2.input_index[0]", "src2.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("src2.index_add[0].input", "src2.offset_add.value"),
            new ConstraintIntegerType("src2.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("src2.input_index[0]", "src2.index_add[0].input"),
            new ConstraintIntegerType("src2.offset_add.value")})}),
        new ConstraintSame("src2.base_index", "src2.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("src2.base_index")}),
          new ConstraintCFGDominateStrict("src2.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("src2.base_index")})}),
        new ConstraintSame("iterator", "src2.input_index[0]"),
        new ConstraintDistinct("src1.base_pointer", "src2.base_pointer"),
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintIncomingValue("new_ind", "body_sese.end", "old_ind"),
        new ConstraintFAddInst("new_ind"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("old_ind", "new_ind"),
            new ConstraintSecondOperand("dotvalue", "new_ind")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("dotvalue", "new_ind"),
            new ConstraintSecondOperand("old_ind", "new_ind")})}),
        new ConstraintFMulInst("dotvalue"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("src1.value", "dotvalue"),
            new ConstraintSecondOperand("src2.value", "dotvalue")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("src2.value", "dotvalue"),
            new ConstraintSecondOperand("src1.value", "dotvalue")})}),
        new ConstraintIncomingValue("initial", "precursor", "old_ind")});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectGEMM(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("loop[0].precursor"),
        new ConstraintCFGEdge("loop[0].precursor", "loop[0].begin"),
        new ConstraintBranchInst("loop[0].end"),
        new ConstraintCFGEdge("loop[0].end", "loop[0].successor"),
        new ConstraintCFGDominate("loop[0].begin", "loop[0].end"),
        new ConstraintCFGPostdom("loop[0].end", "loop[0].begin"),
        new ConstraintCFGDominateStrict("loop[0].precursor", "loop[0].begin"),
        new ConstraintCFGPostdomStrict("loop[0].successor", "loop[0].end"),
        new ConstraintCFGBlocked("loop[0].begin", "loop[0].end", "loop[0].precursor"),
        new ConstraintCFGBlocked("loop[0].successor", "loop[0].begin", "loop[0].end"),
        new ConstraintSame("loop[0].body_sese.precursor", "loop[0].end"),
        new ConstraintSame("loop[0].body_sese.successor", "loop[0].begin"),
        new ConstraintBranchInst("loop[0].body_sese.precursor"),
        new ConstraintCFGEdge("loop[0].body_sese.precursor", "loop[0].body_sese.begin"),
        new ConstraintBranchInst("loop[0].body_sese.end"),
        new ConstraintCFGEdge("loop[0].body_sese.end", "loop[0].body_sese.successor"),
        new ConstraintCFGDominate("loop[0].body_sese.begin", "loop[0].body_sese.end"),
        new ConstraintCFGPostdom("loop[0].body_sese.end", "loop[0].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[0].body_sese.precursor", "loop[0].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[0].body_sese.successor", "loop[0].body_sese.end"),
        new ConstraintCFGBlocked("loop[0].body_sese.begin", "loop[0].body_sese.end", "loop[0].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[0].body_sese.successor", "loop[0].body_sese.begin", "loop[0].body_sese.end"),
        new ConstraintDistinct("loop[0].body_sese.begin", "loop[0].successor"),
        new ConstraintIncomingValue("loop[0].increment", "loop[0].body_sese.end", "loop[0].iterator"),
        new ConstraintDFGEdge("loop[0].iterator", "loop[0].comparison"),
        new ConstraintICmpInst("loop[0].comparison"),
        new ConstraintDFGEdge("loop[0].comparison", "loop[0].end"),
        new ConstraintDFGEdge("loop[0].increment", "loop[0].iterator"),
        new ConstraintAddInst("loop[0].increment"),
        new ConstraintDFGEdge("loop[0].iterator", "loop[0].increment"),
        new ConstraintDFGEdge("loop[0].iter_end", "loop[0].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_end")}),
          new ConstraintCFGDominateStrict("loop[0].iter_end", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_end")})}),
        new ConstraintDFGEdge("loop[0].iter_begin", "loop[0].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[0].iter_begin", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_begin")})}),
        new ConstraintDFGEdge("loop[0].iter_step", "loop[0].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_step")}),
          new ConstraintCFGDominateStrict("loop[0].iter_step", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_step")})}),
        new ConstraintSame("precursor", "loop[0].body_sese.precursor"),
        new ConstraintSame("begin", "loop[0].body_sese.begin"),
        new ConstraintSame("end", "loop[0].body_sese.end"),
        new ConstraintSame("successor", "loop[0].body_sese.successor"),
        new ConstraintBranchInst("loop[1].precursor"),
        new ConstraintCFGEdge("loop[1].precursor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].end"),
        new ConstraintCFGEdge("loop[1].end", "loop[1].successor"),
        new ConstraintCFGDominate("loop[1].begin", "loop[1].end"),
        new ConstraintCFGPostdom("loop[1].end", "loop[1].begin"),
        new ConstraintCFGDominateStrict("loop[1].precursor", "loop[1].begin"),
        new ConstraintCFGPostdomStrict("loop[1].successor", "loop[1].end"),
        new ConstraintCFGBlocked("loop[1].begin", "loop[1].end", "loop[1].precursor"),
        new ConstraintCFGBlocked("loop[1].successor", "loop[1].begin", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.precursor", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.successor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].body_sese.precursor"),
        new ConstraintCFGEdge("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintBranchInst("loop[1].body_sese.end"),
        new ConstraintCFGEdge("loop[1].body_sese.end", "loop[1].body_sese.successor"),
        new ConstraintCFGDominate("loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintCFGPostdom("loop[1].body_sese.end", "loop[1].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[1].body_sese.successor", "loop[1].body_sese.end"),
        new ConstraintCFGBlocked("loop[1].body_sese.begin", "loop[1].body_sese.end", "loop[1].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[1].body_sese.successor", "loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintDistinct("loop[1].body_sese.begin", "loop[1].successor"),
        new ConstraintIncomingValue("loop[1].increment", "loop[1].body_sese.end", "loop[1].iterator"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].comparison"),
        new ConstraintICmpInst("loop[1].comparison"),
        new ConstraintDFGEdge("loop[1].comparison", "loop[1].end"),
        new ConstraintDFGEdge("loop[1].increment", "loop[1].iterator"),
        new ConstraintAddInst("loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iter_end", "loop[1].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintDFGEdge("loop[1].iter_begin", "loop[1].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintDFGEdge("loop[1].iter_step", "loop[1].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintSame("loop[0].body_sese.precursor", "loop[1].pre_sese.precursor"),
        new ConstraintSame("loop[0].body_sese.begin", "loop[1].pre_sese.begin"),
        new ConstraintSame("loop[1].pre_sese.end", "loop[1].precursor"),
        new ConstraintSame("loop[1].pre_sese.successor", "loop[1].begin"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[1].pre_sese.precursor"),
            new ConstraintCFGEdge("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin"),
            new ConstraintBranchInst("loop[1].pre_sese.end"),
            new ConstraintCFGEdge("loop[1].pre_sese.end", "loop[1].pre_sese.successor"),
            new ConstraintCFGDominate("loop[1].pre_sese.begin", "loop[1].pre_sese.end"),
            new ConstraintCFGPostdom("loop[1].pre_sese.end", "loop[1].pre_sese.begin"),
            new ConstraintCFGDominateStrict("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[1].pre_sese.successor", "loop[1].pre_sese.end"),
            new ConstraintCFGBlocked("loop[1].pre_sese.begin", "loop[1].pre_sese.end", "loop[1].pre_sese.precursor"),
            new ConstraintCFGBlocked("loop[1].pre_sese.successor", "loop[1].pre_sese.begin", "loop[1].pre_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].pre_sese.precursor", "loop[1].pre_sese.end"),
            new ConstraintSame("loop[1].pre_sese.begin", "loop[1].pre_sese.successor")})}),
        new ConstraintSame("loop[1].end", "loop[1].post_sese.precursor"),
        new ConstraintSame("loop[1].successor", "loop[1].post_sese.begin"),
        new ConstraintSame("loop[0].body_sese.end", "loop[1].post_sese.end"),
        new ConstraintSame("loop[0].body_sese.successor", "loop[1].post_sese.successor"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[1].post_sese.precursor"),
            new ConstraintCFGEdge("loop[1].post_sese.precursor", "loop[1].post_sese.begin"),
            new ConstraintBranchInst("loop[1].post_sese.end"),
            new ConstraintCFGEdge("loop[1].post_sese.end", "loop[1].post_sese.successor"),
            new ConstraintCFGDominate("loop[1].post_sese.begin", "loop[1].post_sese.end"),
            new ConstraintCFGPostdom("loop[1].post_sese.end", "loop[1].post_sese.begin"),
            new ConstraintCFGDominateStrict("loop[1].post_sese.precursor", "loop[1].post_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[1].post_sese.successor", "loop[1].post_sese.end"),
            new ConstraintCFGBlocked("loop[1].post_sese.begin", "loop[1].post_sese.end", "loop[1].post_sese.precursor"),
            new ConstraintCFGBlocked("loop[1].post_sese.successor", "loop[1].post_sese.begin", "loop[1].post_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].post_sese.precursor", "loop[1].post_sese.end"),
            new ConstraintSame("loop[1].post_sese.begin", "loop[1].post_sese.successor")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintBranchInst("loop[2].precursor"),
        new ConstraintCFGEdge("loop[2].precursor", "loop[2].begin"),
        new ConstraintBranchInst("loop[2].end"),
        new ConstraintCFGEdge("loop[2].end", "loop[2].successor"),
        new ConstraintCFGDominate("loop[2].begin", "loop[2].end"),
        new ConstraintCFGPostdom("loop[2].end", "loop[2].begin"),
        new ConstraintCFGDominateStrict("loop[2].precursor", "loop[2].begin"),
        new ConstraintCFGPostdomStrict("loop[2].successor", "loop[2].end"),
        new ConstraintCFGBlocked("loop[2].begin", "loop[2].end", "loop[2].precursor"),
        new ConstraintCFGBlocked("loop[2].successor", "loop[2].begin", "loop[2].end"),
        new ConstraintSame("loop[2].body_sese.precursor", "loop[2].end"),
        new ConstraintSame("loop[2].body_sese.successor", "loop[2].begin"),
        new ConstraintBranchInst("loop[2].body_sese.precursor"),
        new ConstraintCFGEdge("loop[2].body_sese.precursor", "loop[2].body_sese.begin"),
        new ConstraintBranchInst("loop[2].body_sese.end"),
        new ConstraintCFGEdge("loop[2].body_sese.end", "loop[2].body_sese.successor"),
        new ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].body_sese.end"),
        new ConstraintCFGPostdom("loop[2].body_sese.end", "loop[2].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[2].body_sese.precursor", "loop[2].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[2].body_sese.successor", "loop[2].body_sese.end"),
        new ConstraintCFGBlocked("loop[2].body_sese.begin", "loop[2].body_sese.end", "loop[2].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[2].body_sese.successor", "loop[2].body_sese.begin", "loop[2].body_sese.end"),
        new ConstraintDistinct("loop[2].body_sese.begin", "loop[2].successor"),
        new ConstraintIncomingValue("loop[2].increment", "loop[2].body_sese.end", "loop[2].iterator"),
        new ConstraintDFGEdge("loop[2].iterator", "loop[2].comparison"),
        new ConstraintICmpInst("loop[2].comparison"),
        new ConstraintDFGEdge("loop[2].comparison", "loop[2].end"),
        new ConstraintDFGEdge("loop[2].increment", "loop[2].iterator"),
        new ConstraintAddInst("loop[2].increment"),
        new ConstraintDFGEdge("loop[2].iterator", "loop[2].increment"),
        new ConstraintDFGEdge("loop[2].iter_end", "loop[2].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_end")}),
          new ConstraintCFGDominateStrict("loop[2].iter_end", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_end")})}),
        new ConstraintDFGEdge("loop[2].iter_begin", "loop[2].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[2].iter_begin", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_begin")})}),
        new ConstraintDFGEdge("loop[2].iter_step", "loop[2].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_step")}),
          new ConstraintCFGDominateStrict("loop[2].iter_step", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_step")})}),
        new ConstraintSame("loop[1].body_sese.precursor", "loop[2].pre_sese.precursor"),
        new ConstraintSame("loop[1].body_sese.begin", "loop[2].pre_sese.begin"),
        new ConstraintSame("loop[2].pre_sese.end", "loop[2].precursor"),
        new ConstraintSame("loop[2].pre_sese.successor", "loop[2].begin"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[2].pre_sese.precursor"),
            new ConstraintCFGEdge("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin"),
            new ConstraintBranchInst("loop[2].pre_sese.end"),
            new ConstraintCFGEdge("loop[2].pre_sese.end", "loop[2].pre_sese.successor"),
            new ConstraintCFGDominate("loop[2].pre_sese.begin", "loop[2].pre_sese.end"),
            new ConstraintCFGPostdom("loop[2].pre_sese.end", "loop[2].pre_sese.begin"),
            new ConstraintCFGDominateStrict("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[2].pre_sese.successor", "loop[2].pre_sese.end"),
            new ConstraintCFGBlocked("loop[2].pre_sese.begin", "loop[2].pre_sese.end", "loop[2].pre_sese.precursor"),
            new ConstraintCFGBlocked("loop[2].pre_sese.successor", "loop[2].pre_sese.begin", "loop[2].pre_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[2].pre_sese.precursor", "loop[2].pre_sese.end"),
            new ConstraintSame("loop[2].pre_sese.begin", "loop[2].pre_sese.successor")})}),
        new ConstraintSame("loop[2].end", "loop[2].post_sese.precursor"),
        new ConstraintSame("loop[2].successor", "loop[2].post_sese.begin"),
        new ConstraintSame("loop[1].body_sese.end", "loop[2].post_sese.end"),
        new ConstraintSame("loop[1].body_sese.successor", "loop[2].post_sese.successor"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[2].post_sese.precursor"),
            new ConstraintCFGEdge("loop[2].post_sese.precursor", "loop[2].post_sese.begin"),
            new ConstraintBranchInst("loop[2].post_sese.end"),
            new ConstraintCFGEdge("loop[2].post_sese.end", "loop[2].post_sese.successor"),
            new ConstraintCFGDominate("loop[2].post_sese.begin", "loop[2].post_sese.end"),
            new ConstraintCFGPostdom("loop[2].post_sese.end", "loop[2].post_sese.begin"),
            new ConstraintCFGDominateStrict("loop[2].post_sese.precursor", "loop[2].post_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[2].post_sese.successor", "loop[2].post_sese.end"),
            new ConstraintCFGBlocked("loop[2].post_sese.begin", "loop[2].post_sese.end", "loop[2].post_sese.precursor"),
            new ConstraintCFGBlocked("loop[2].post_sese.successor", "loop[2].post_sese.begin", "loop[2].post_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[2].post_sese.precursor", "loop[2].post_sese.end"),
            new ConstraintSame("loop[2].post_sese.begin", "loop[2].post_sese.successor")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[2].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[2].iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[2].iter_begin")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[2].iter_step")}),
          new ConstraintCFGDominateStrict("loop[2].iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[2].iter_step")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[2].iter_end")}),
          new ConstraintCFGDominateStrict("loop[2].iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[2].iter_end")})}),
        new ConstraintSame("body_sese.precursor", "loop[2].body_sese.precursor"),
        new ConstraintSame("body_sese.begin", "loop[2].body_sese.begin"),
        new ConstraintSame("body_sese.end", "loop[2].body_sese.end"),
        new ConstraintSame("body_sese.successor", "loop[2].body_sese.successor"),
        new ConstraintStoreInst("output.store"),
        new ConstraintSecondOperand("output.access_pointer", "output.store"),
        new ConstraintGEPInst("output.access_pointer"),
        new ConstraintFirstOperand("output.base_pointer", "output.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.base_pointer")}),
          new ConstraintCFGDominateStrict("output.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.base_pointer")})}),
        new ConstraintSecondOperand("output.output_index", "output.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("output.output_index"),
            new ConstraintDFGEdge("output.offset_add.value", "output.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("output.output_index"),
            new ConstraintDFGEdge("output.offset_add.value", "output.output_index")}),
          new ConstraintSame("output.offset_add.value", "output.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.offset_add.input", "output.offset_add.value"),
            new ConstraintUnused("output.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("output.offset_add.value"),
              new ConstraintBitOrInst("output.offset_add.value"),
              new ConstraintFAddInst("output.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.input", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.addend", "output.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.addend", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.input", "output.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("output.offset_add.value"),
              new ConstraintFSubInst("output.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.input", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.addend", "output.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.addend", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.input", "output.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.offset_add.addend")}),
          new ConstraintCFGDominateStrict("output.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.offset_add.addend")})}),
        new ConstraintSame("output.stride_mul[1].value", "output.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.stride_mul[1].input", "output.stride_mul[1].value"),
            new ConstraintUnused("output.stride_mul[1].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("output.stride_mul[1].value"),
              new ConstraintLShiftInst("output.stride_mul[1].value"),
              new ConstraintFMulInst("output.stride_mul[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[1].input", "output.stride_mul[1].value"),
                new ConstraintSecondOperand("output.stride_mul[1].multiplier", "output.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[1].multiplier", "output.stride_mul[1].value"),
                new ConstraintSecondOperand("output.stride_mul[1].input", "output.stride_mul[1].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("output.stride_mul[1].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[1].input", "output.stride_mul[1].value"),
                new ConstraintSecondOperand("output.stride_mul[1].multiplier", "output.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[1].multiplier", "output.stride_mul[1].value"),
                new ConstraintSecondOperand("output.stride_mul[1].input", "output.stride_mul[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.stride_mul[1].multiplier")}),
          new ConstraintCFGDominateStrict("output.stride_mul[1].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.stride_mul[1].multiplier")})}),
        new ConstraintSame("output.index_add[1].value", "output.stride_mul[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.index_add[1].input", "output.index_add[1].value"),
            new ConstraintUnused("output.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("output.index_add[1].value"),
              new ConstraintBitOrInst("output.index_add[1].value"),
              new ConstraintFAddInst("output.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[1].input", "output.index_add[1].value"),
                new ConstraintSecondOperand("output.index_add[1].addend", "output.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[1].addend", "output.index_add[1].value"),
                new ConstraintSecondOperand("output.index_add[1].input", "output.index_add[1].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("output.index_add[1].value"),
              new ConstraintFSubInst("output.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[1].input", "output.index_add[1].value"),
                new ConstraintSecondOperand("output.index_add[1].addend", "output.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[1].addend", "output.index_add[1].value"),
                new ConstraintSecondOperand("output.index_add[1].input", "output.index_add[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("output.index_add[1].addend"),
            new ConstraintDFGEdge("output.input_index[1]", "output.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintSExtInst("output.index_add[1].addend"),
            new ConstraintDFGEdge("output.input_index[1]", "output.index_add[1].addend")}),
          new ConstraintSame("output.input_index[1]", "output.index_add[1].addend")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("output.index_add[1].addend", "output.offset_add.value"),
            new ConstraintIntegerType("output.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("output.input_index[1]", "output.index_add[1].addend"),
            new ConstraintIntegerType("output.offset_add.value")})}),
        new ConstraintSame("output.stride_mul[0].value", "output.index_add[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.stride_mul[0].input", "output.stride_mul[0].value"),
            new ConstraintUnused("output.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("output.stride_mul[0].value"),
              new ConstraintLShiftInst("output.stride_mul[0].value"),
              new ConstraintFMulInst("output.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].input", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].input", "output.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("output.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].input", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].input", "output.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("output.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.stride_mul[0].multiplier")})}),
        new ConstraintSame("output.index_add[0].value", "output.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.index_add[0].input", "output.index_add[0].value"),
            new ConstraintUnused("output.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("output.index_add[0].value"),
              new ConstraintBitOrInst("output.index_add[0].value"),
              new ConstraintFAddInst("output.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].input", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].addend", "output.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].addend", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].input", "output.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("output.index_add[0].value"),
              new ConstraintFSubInst("output.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].input", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].addend", "output.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].addend", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].input", "output.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("output.index_add[0].input"),
            new ConstraintDFGEdge("output.input_index[0]", "output.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("output.index_add[0].input"),
            new ConstraintDFGEdge("output.input_index[0]", "output.index_add[0].input")}),
          new ConstraintSame("output.input_index[0]", "output.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("output.index_add[0].input", "output.input_index[1]"),
            new ConstraintIntegerType("output.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("output.input_index[0]", "output.index_add[0].input"),
            new ConstraintIntegerType("output.input_index[1]")})}),
        new ConstraintSame("output.base_index", "output.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.base_index")}),
          new ConstraintCFGDominateStrict("output.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.base_index")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "output.input_index[0]"),
            new ConstraintSame("loop[1].iterator", "output.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "output.input_index[1]"),
            new ConstraintSame("loop[1].iterator", "output.input_index[0]")})}),
        new ConstraintLoadInst("input1.value"),
        new ConstraintFirstOperand("input1.access_pointer", "input1.value"),
        new ConstraintGEPInst("input1.access_pointer"),
        new ConstraintFirstOperand("input1.base_pointer", "input1.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.base_pointer")}),
          new ConstraintCFGDominateStrict("input1.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.base_pointer")})}),
        new ConstraintSecondOperand("input1.output_index", "input1.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input1.output_index"),
            new ConstraintDFGEdge("input1.offset_add.value", "input1.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input1.output_index"),
            new ConstraintDFGEdge("input1.offset_add.value", "input1.output_index")}),
          new ConstraintSame("input1.offset_add.value", "input1.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.offset_add.input", "input1.offset_add.value"),
            new ConstraintUnused("input1.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input1.offset_add.value"),
              new ConstraintBitOrInst("input1.offset_add.value"),
              new ConstraintFAddInst("input1.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.input", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.addend", "input1.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.addend", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.input", "input1.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input1.offset_add.value"),
              new ConstraintFSubInst("input1.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.input", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.addend", "input1.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.addend", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.input", "input1.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.offset_add.addend")}),
          new ConstraintCFGDominateStrict("input1.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.offset_add.addend")})}),
        new ConstraintSame("input1.stride_mul[1].value", "input1.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.stride_mul[1].input", "input1.stride_mul[1].value"),
            new ConstraintUnused("input1.stride_mul[1].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input1.stride_mul[1].value"),
              new ConstraintLShiftInst("input1.stride_mul[1].value"),
              new ConstraintFMulInst("input1.stride_mul[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input1.stride_mul[1].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.stride_mul[1].multiplier")}),
          new ConstraintCFGDominateStrict("input1.stride_mul[1].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.stride_mul[1].multiplier")})}),
        new ConstraintSame("input1.index_add[1].value", "input1.stride_mul[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.index_add[1].input", "input1.index_add[1].value"),
            new ConstraintUnused("input1.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input1.index_add[1].value"),
              new ConstraintBitOrInst("input1.index_add[1].value"),
              new ConstraintFAddInst("input1.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].input", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].addend", "input1.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].addend", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].input", "input1.index_add[1].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input1.index_add[1].value"),
              new ConstraintFSubInst("input1.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].input", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].addend", "input1.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].addend", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].input", "input1.index_add[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input1.index_add[1].addend"),
            new ConstraintDFGEdge("input1.input_index[1]", "input1.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input1.index_add[1].addend"),
            new ConstraintDFGEdge("input1.input_index[1]", "input1.index_add[1].addend")}),
          new ConstraintSame("input1.input_index[1]", "input1.index_add[1].addend")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input1.index_add[1].addend", "input1.offset_add.value"),
            new ConstraintIntegerType("input1.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("input1.input_index[1]", "input1.index_add[1].addend"),
            new ConstraintIntegerType("input1.offset_add.value")})}),
        new ConstraintSame("input1.stride_mul[0].value", "input1.index_add[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.stride_mul[0].input", "input1.stride_mul[0].value"),
            new ConstraintUnused("input1.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input1.stride_mul[0].value"),
              new ConstraintLShiftInst("input1.stride_mul[0].value"),
              new ConstraintFMulInst("input1.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input1.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("input1.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.stride_mul[0].multiplier")})}),
        new ConstraintSame("input1.index_add[0].value", "input1.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.index_add[0].input", "input1.index_add[0].value"),
            new ConstraintUnused("input1.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input1.index_add[0].value"),
              new ConstraintBitOrInst("input1.index_add[0].value"),
              new ConstraintFAddInst("input1.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].input", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].addend", "input1.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].addend", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].input", "input1.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input1.index_add[0].value"),
              new ConstraintFSubInst("input1.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].input", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].addend", "input1.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].addend", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].input", "input1.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input1.index_add[0].input"),
            new ConstraintDFGEdge("input1.input_index[0]", "input1.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input1.index_add[0].input"),
            new ConstraintDFGEdge("input1.input_index[0]", "input1.index_add[0].input")}),
          new ConstraintSame("input1.input_index[0]", "input1.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input1.index_add[0].input", "input1.input_index[1]"),
            new ConstraintIntegerType("input1.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("input1.input_index[0]", "input1.index_add[0].input"),
            new ConstraintIntegerType("input1.input_index[1]")})}),
        new ConstraintSame("input1.base_index", "input1.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.base_index")}),
          new ConstraintCFGDominateStrict("input1.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.base_index")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "input1.input_index[0]"),
            new ConstraintSame("loop[2].iterator", "input1.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "input1.input_index[1]"),
            new ConstraintSame("loop[2].iterator", "input1.input_index[0]")})}),
        new ConstraintLoadInst("input2.value"),
        new ConstraintFirstOperand("input2.access_pointer", "input2.value"),
        new ConstraintGEPInst("input2.access_pointer"),
        new ConstraintFirstOperand("input2.base_pointer", "input2.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.base_pointer")}),
          new ConstraintCFGDominateStrict("input2.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.base_pointer")})}),
        new ConstraintSecondOperand("input2.output_index", "input2.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input2.output_index"),
            new ConstraintDFGEdge("input2.offset_add.value", "input2.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input2.output_index"),
            new ConstraintDFGEdge("input2.offset_add.value", "input2.output_index")}),
          new ConstraintSame("input2.offset_add.value", "input2.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.offset_add.input", "input2.offset_add.value"),
            new ConstraintUnused("input2.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input2.offset_add.value"),
              new ConstraintBitOrInst("input2.offset_add.value"),
              new ConstraintFAddInst("input2.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.input", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.addend", "input2.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.addend", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.input", "input2.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input2.offset_add.value"),
              new ConstraintFSubInst("input2.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.input", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.addend", "input2.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.addend", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.input", "input2.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.offset_add.addend")}),
          new ConstraintCFGDominateStrict("input2.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.offset_add.addend")})}),
        new ConstraintSame("input2.stride_mul[1].value", "input2.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.stride_mul[1].input", "input2.stride_mul[1].value"),
            new ConstraintUnused("input2.stride_mul[1].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input2.stride_mul[1].value"),
              new ConstraintLShiftInst("input2.stride_mul[1].value"),
              new ConstraintFMulInst("input2.stride_mul[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[1].input", "input2.stride_mul[1].value"),
                new ConstraintSecondOperand("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value"),
                new ConstraintSecondOperand("input2.stride_mul[1].input", "input2.stride_mul[1].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input2.stride_mul[1].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[1].input", "input2.stride_mul[1].value"),
                new ConstraintSecondOperand("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value"),
                new ConstraintSecondOperand("input2.stride_mul[1].input", "input2.stride_mul[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.stride_mul[1].multiplier")}),
          new ConstraintCFGDominateStrict("input2.stride_mul[1].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.stride_mul[1].multiplier")})}),
        new ConstraintSame("input2.index_add[1].value", "input2.stride_mul[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.index_add[1].input", "input2.index_add[1].value"),
            new ConstraintUnused("input2.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input2.index_add[1].value"),
              new ConstraintBitOrInst("input2.index_add[1].value"),
              new ConstraintFAddInst("input2.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[1].input", "input2.index_add[1].value"),
                new ConstraintSecondOperand("input2.index_add[1].addend", "input2.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[1].addend", "input2.index_add[1].value"),
                new ConstraintSecondOperand("input2.index_add[1].input", "input2.index_add[1].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input2.index_add[1].value"),
              new ConstraintFSubInst("input2.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[1].input", "input2.index_add[1].value"),
                new ConstraintSecondOperand("input2.index_add[1].addend", "input2.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[1].addend", "input2.index_add[1].value"),
                new ConstraintSecondOperand("input2.index_add[1].input", "input2.index_add[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input2.index_add[1].addend"),
            new ConstraintDFGEdge("input2.input_index[1]", "input2.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input2.index_add[1].addend"),
            new ConstraintDFGEdge("input2.input_index[1]", "input2.index_add[1].addend")}),
          new ConstraintSame("input2.input_index[1]", "input2.index_add[1].addend")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input2.index_add[1].addend", "input2.offset_add.value"),
            new ConstraintIntegerType("input2.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("input2.input_index[1]", "input2.index_add[1].addend"),
            new ConstraintIntegerType("input2.offset_add.value")})}),
        new ConstraintSame("input2.stride_mul[0].value", "input2.index_add[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.stride_mul[0].input", "input2.stride_mul[0].value"),
            new ConstraintUnused("input2.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input2.stride_mul[0].value"),
              new ConstraintLShiftInst("input2.stride_mul[0].value"),
              new ConstraintFMulInst("input2.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input2.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("input2.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.stride_mul[0].multiplier")})}),
        new ConstraintSame("input2.index_add[0].value", "input2.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.index_add[0].input", "input2.index_add[0].value"),
            new ConstraintUnused("input2.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input2.index_add[0].value"),
              new ConstraintBitOrInst("input2.index_add[0].value"),
              new ConstraintFAddInst("input2.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].input", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].addend", "input2.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].addend", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].input", "input2.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input2.index_add[0].value"),
              new ConstraintFSubInst("input2.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].input", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].addend", "input2.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].addend", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].input", "input2.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input2.index_add[0].input"),
            new ConstraintDFGEdge("input2.input_index[0]", "input2.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input2.index_add[0].input"),
            new ConstraintDFGEdge("input2.input_index[0]", "input2.index_add[0].input")}),
          new ConstraintSame("input2.input_index[0]", "input2.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input2.index_add[0].input", "input2.input_index[1]"),
            new ConstraintIntegerType("input2.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("input2.input_index[0]", "input2.index_add[0].input"),
            new ConstraintIntegerType("input2.input_index[1]")})}),
        new ConstraintSame("input2.base_index", "input2.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.base_index")}),
          new ConstraintCFGDominateStrict("input2.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.base_index")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[1].iterator", "input2.input_index[0]"),
            new ConstraintSame("loop[2].iterator", "input2.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].iterator", "input2.input_index[1]"),
            new ConstraintSame("loop[2].iterator", "input2.input_index[0]")})}),
        new ConstraintBranchInst("loop[2].precursor"),
        new ConstraintCFGEdge("loop[2].precursor", "loop[2].begin"),
        new ConstraintBranchInst("loop[2].end"),
        new ConstraintCFGEdge("loop[2].end", "loop[2].successor"),
        new ConstraintCFGDominate("loop[2].begin", "loop[2].end"),
        new ConstraintCFGPostdom("loop[2].end", "loop[2].begin"),
        new ConstraintCFGDominateStrict("loop[2].precursor", "loop[2].begin"),
        new ConstraintCFGPostdomStrict("loop[2].successor", "loop[2].end"),
        new ConstraintCFGBlocked("loop[2].begin", "loop[2].end", "loop[2].precursor"),
        new ConstraintCFGBlocked("loop[2].successor", "loop[2].begin", "loop[2].end"),
        new ConstraintSame("loop[2].body_sese.precursor", "loop[2].end"),
        new ConstraintSame("loop[2].body_sese.successor", "loop[2].begin"),
        new ConstraintBranchInst("loop[2].body_sese.precursor"),
        new ConstraintCFGEdge("loop[2].body_sese.precursor", "loop[2].body_sese.begin"),
        new ConstraintBranchInst("loop[2].body_sese.end"),
        new ConstraintCFGEdge("loop[2].body_sese.end", "loop[2].body_sese.successor"),
        new ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].body_sese.end"),
        new ConstraintCFGPostdom("loop[2].body_sese.end", "loop[2].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[2].body_sese.precursor", "loop[2].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[2].body_sese.successor", "loop[2].body_sese.end"),
        new ConstraintCFGBlocked("loop[2].body_sese.begin", "loop[2].body_sese.end", "loop[2].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[2].body_sese.successor", "loop[2].body_sese.begin", "loop[2].body_sese.end"),
        new ConstraintDistinct("loop[2].body_sese.begin", "loop[2].successor"),
        new ConstraintIncomingValue("loop[2].increment", "loop[2].body_sese.end", "loop[2].iterator"),
        new ConstraintDFGEdge("loop[2].iterator", "loop[2].comparison"),
        new ConstraintICmpInst("loop[2].comparison"),
        new ConstraintDFGEdge("loop[2].comparison", "loop[2].end"),
        new ConstraintDFGEdge("loop[2].increment", "loop[2].iterator"),
        new ConstraintAddInst("loop[2].increment"),
        new ConstraintDFGEdge("loop[2].iterator", "loop[2].increment"),
        new ConstraintDFGEdge("loop[2].iter_end", "loop[2].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_end")}),
          new ConstraintCFGDominateStrict("loop[2].iter_end", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_end")})}),
        new ConstraintDFGEdge("loop[2].iter_begin", "loop[2].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[2].iter_begin", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_begin")})}),
        new ConstraintDFGEdge("loop[2].iter_step", "loop[2].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_step")}),
          new ConstraintCFGDominateStrict("loop[2].iter_step", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_step")})}),
        new ConstraintIncomingValue("loop[2].new_ind", "loop[2].body_sese.end", "loop[2].old_ind"),
        new ConstraintFAddInst("loop[2].new_ind"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("loop[2].old_ind", "loop[2].new_ind"),
            new ConstraintSecondOperand("loop[2].dotvalue", "loop[2].new_ind")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("loop[2].dotvalue", "loop[2].new_ind"),
            new ConstraintSecondOperand("loop[2].old_ind", "loop[2].new_ind")})}),
        new ConstraintFMulInst("loop[2].dotvalue"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("input1.value", "loop[2].dotvalue"),
            new ConstraintSecondOperand("input2.value", "loop[2].dotvalue")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("input2.value", "loop[2].dotvalue"),
            new ConstraintSecondOperand("input1.value", "loop[2].dotvalue")})}),
        new ConstraintIncomingValue("loop[2].initial", "loop[2].precursor", "loop[2].old_ind"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintLoadInst("loop[2].initial"),
            new ConstraintDFGEdge("output.access_pointer", "loop[2].initial")}),
          new ConstraintAnd({
            new ConstraintFloatZero("loop[2].initial"),
            new ConstraintGEPInst("output.access_pointer")})}),
        new ConstraintFirstOperand("final_value", "output.store"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("scaled_reduc", "final_value"),
            new ConstraintUnused("scaled_prev")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("final_value"),
              new ConstraintBitOrInst("final_value"),
              new ConstraintFAddInst("final_value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_reduc", "final_value"),
                new ConstraintSecondOperand("scaled_prev", "final_value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_prev", "final_value"),
                new ConstraintSecondOperand("scaled_reduc", "final_value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("final_value"),
              new ConstraintFSubInst("final_value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_reduc", "final_value"),
                new ConstraintSecondOperand("scaled_prev", "final_value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_prev", "final_value"),
                new ConstraintSecondOperand("scaled_reduc", "final_value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[2].old_ind", "scaled_reduc"),
            new ConstraintUnused("alpha")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("scaled_reduc"),
              new ConstraintLShiftInst("scaled_reduc"),
              new ConstraintFMulInst("scaled_reduc")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("loop[2].old_ind", "scaled_reduc"),
                new ConstraintSecondOperand("alpha", "scaled_reduc")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("alpha", "scaled_reduc"),
                new ConstraintSecondOperand("loop[2].old_ind", "scaled_reduc")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("scaled_reduc"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("loop[2].old_ind", "scaled_reduc"),
                new ConstraintSecondOperand("alpha", "scaled_reduc")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("alpha", "scaled_reduc"),
                new ConstraintSecondOperand("loop[2].old_ind", "scaled_reduc")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("alpha")}),
          new ConstraintCFGDominateStrict("alpha", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("alpha")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("prev", "scaled_prev"),
            new ConstraintUnused("beta")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("scaled_prev"),
              new ConstraintLShiftInst("scaled_prev"),
              new ConstraintFMulInst("scaled_prev")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("prev", "scaled_prev"),
                new ConstraintSecondOperand("beta", "scaled_prev")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("beta", "scaled_prev"),
                new ConstraintSecondOperand("prev", "scaled_prev")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("scaled_prev"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("prev", "scaled_prev"),
                new ConstraintSecondOperand("beta", "scaled_prev")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("beta", "scaled_prev"),
                new ConstraintSecondOperand("prev", "scaled_prev")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintUnused("prev"),
            new ConstraintGEPInst("output.access_pointer")}),
          new ConstraintAnd({
            new ConstraintLoadInst("prev"),
            new ConstraintFirstOperand("output.access_pointer", "prev")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("beta")}),
          new ConstraintCFGDominateStrict("beta", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("beta")})})});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectGEMV(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("loop[0].precursor"),
        new ConstraintCFGEdge("loop[0].precursor", "loop[0].begin"),
        new ConstraintBranchInst("loop[0].end"),
        new ConstraintCFGEdge("loop[0].end", "loop[0].successor"),
        new ConstraintCFGDominate("loop[0].begin", "loop[0].end"),
        new ConstraintCFGPostdom("loop[0].end", "loop[0].begin"),
        new ConstraintCFGDominateStrict("loop[0].precursor", "loop[0].begin"),
        new ConstraintCFGPostdomStrict("loop[0].successor", "loop[0].end"),
        new ConstraintCFGBlocked("loop[0].begin", "loop[0].end", "loop[0].precursor"),
        new ConstraintCFGBlocked("loop[0].successor", "loop[0].begin", "loop[0].end"),
        new ConstraintSame("loop[0].body_sese.precursor", "loop[0].end"),
        new ConstraintSame("loop[0].body_sese.successor", "loop[0].begin"),
        new ConstraintBranchInst("loop[0].body_sese.precursor"),
        new ConstraintCFGEdge("loop[0].body_sese.precursor", "loop[0].body_sese.begin"),
        new ConstraintBranchInst("loop[0].body_sese.end"),
        new ConstraintCFGEdge("loop[0].body_sese.end", "loop[0].body_sese.successor"),
        new ConstraintCFGDominate("loop[0].body_sese.begin", "loop[0].body_sese.end"),
        new ConstraintCFGPostdom("loop[0].body_sese.end", "loop[0].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[0].body_sese.precursor", "loop[0].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[0].body_sese.successor", "loop[0].body_sese.end"),
        new ConstraintCFGBlocked("loop[0].body_sese.begin", "loop[0].body_sese.end", "loop[0].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[0].body_sese.successor", "loop[0].body_sese.begin", "loop[0].body_sese.end"),
        new ConstraintDistinct("loop[0].body_sese.begin", "loop[0].successor"),
        new ConstraintIncomingValue("loop[0].increment", "loop[0].body_sese.end", "loop[0].iterator"),
        new ConstraintDFGEdge("loop[0].iterator", "loop[0].comparison"),
        new ConstraintICmpInst("loop[0].comparison"),
        new ConstraintDFGEdge("loop[0].comparison", "loop[0].end"),
        new ConstraintDFGEdge("loop[0].increment", "loop[0].iterator"),
        new ConstraintAddInst("loop[0].increment"),
        new ConstraintDFGEdge("loop[0].iterator", "loop[0].increment"),
        new ConstraintDFGEdge("loop[0].iter_end", "loop[0].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_end")}),
          new ConstraintCFGDominateStrict("loop[0].iter_end", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_end")})}),
        new ConstraintDFGEdge("loop[0].iter_begin", "loop[0].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[0].iter_begin", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_begin")})}),
        new ConstraintDFGEdge("loop[0].iter_step", "loop[0].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_step")}),
          new ConstraintCFGDominateStrict("loop[0].iter_step", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_step")})}),
        new ConstraintSame("precursor", "loop[0].body_sese.precursor"),
        new ConstraintSame("begin", "loop[0].body_sese.begin"),
        new ConstraintSame("end", "loop[0].body_sese.end"),
        new ConstraintSame("successor", "loop[0].body_sese.successor"),
        new ConstraintBranchInst("loop[1].precursor"),
        new ConstraintCFGEdge("loop[1].precursor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].end"),
        new ConstraintCFGEdge("loop[1].end", "loop[1].successor"),
        new ConstraintCFGDominate("loop[1].begin", "loop[1].end"),
        new ConstraintCFGPostdom("loop[1].end", "loop[1].begin"),
        new ConstraintCFGDominateStrict("loop[1].precursor", "loop[1].begin"),
        new ConstraintCFGPostdomStrict("loop[1].successor", "loop[1].end"),
        new ConstraintCFGBlocked("loop[1].begin", "loop[1].end", "loop[1].precursor"),
        new ConstraintCFGBlocked("loop[1].successor", "loop[1].begin", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.precursor", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.successor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].body_sese.precursor"),
        new ConstraintCFGEdge("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintBranchInst("loop[1].body_sese.end"),
        new ConstraintCFGEdge("loop[1].body_sese.end", "loop[1].body_sese.successor"),
        new ConstraintCFGDominate("loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintCFGPostdom("loop[1].body_sese.end", "loop[1].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[1].body_sese.successor", "loop[1].body_sese.end"),
        new ConstraintCFGBlocked("loop[1].body_sese.begin", "loop[1].body_sese.end", "loop[1].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[1].body_sese.successor", "loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintDistinct("loop[1].body_sese.begin", "loop[1].successor"),
        new ConstraintIncomingValue("loop[1].increment", "loop[1].body_sese.end", "loop[1].iterator"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].comparison"),
        new ConstraintICmpInst("loop[1].comparison"),
        new ConstraintDFGEdge("loop[1].comparison", "loop[1].end"),
        new ConstraintDFGEdge("loop[1].increment", "loop[1].iterator"),
        new ConstraintAddInst("loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iter_end", "loop[1].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintDFGEdge("loop[1].iter_begin", "loop[1].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintDFGEdge("loop[1].iter_step", "loop[1].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintSame("loop[0].body_sese.precursor", "loop[1].pre_sese.precursor"),
        new ConstraintSame("loop[0].body_sese.begin", "loop[1].pre_sese.begin"),
        new ConstraintSame("loop[1].pre_sese.end", "loop[1].precursor"),
        new ConstraintSame("loop[1].pre_sese.successor", "loop[1].begin"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[1].pre_sese.precursor"),
            new ConstraintCFGEdge("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin"),
            new ConstraintBranchInst("loop[1].pre_sese.end"),
            new ConstraintCFGEdge("loop[1].pre_sese.end", "loop[1].pre_sese.successor"),
            new ConstraintCFGDominate("loop[1].pre_sese.begin", "loop[1].pre_sese.end"),
            new ConstraintCFGPostdom("loop[1].pre_sese.end", "loop[1].pre_sese.begin"),
            new ConstraintCFGDominateStrict("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[1].pre_sese.successor", "loop[1].pre_sese.end"),
            new ConstraintCFGBlocked("loop[1].pre_sese.begin", "loop[1].pre_sese.end", "loop[1].pre_sese.precursor"),
            new ConstraintCFGBlocked("loop[1].pre_sese.successor", "loop[1].pre_sese.begin", "loop[1].pre_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].pre_sese.precursor", "loop[1].pre_sese.end"),
            new ConstraintSame("loop[1].pre_sese.begin", "loop[1].pre_sese.successor")})}),
        new ConstraintSame("loop[1].end", "loop[1].post_sese.precursor"),
        new ConstraintSame("loop[1].successor", "loop[1].post_sese.begin"),
        new ConstraintSame("loop[0].body_sese.end", "loop[1].post_sese.end"),
        new ConstraintSame("loop[0].body_sese.successor", "loop[1].post_sese.successor"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[1].post_sese.precursor"),
            new ConstraintCFGEdge("loop[1].post_sese.precursor", "loop[1].post_sese.begin"),
            new ConstraintBranchInst("loop[1].post_sese.end"),
            new ConstraintCFGEdge("loop[1].post_sese.end", "loop[1].post_sese.successor"),
            new ConstraintCFGDominate("loop[1].post_sese.begin", "loop[1].post_sese.end"),
            new ConstraintCFGPostdom("loop[1].post_sese.end", "loop[1].post_sese.begin"),
            new ConstraintCFGDominateStrict("loop[1].post_sese.precursor", "loop[1].post_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[1].post_sese.successor", "loop[1].post_sese.end"),
            new ConstraintCFGBlocked("loop[1].post_sese.begin", "loop[1].post_sese.end", "loop[1].post_sese.precursor"),
            new ConstraintCFGBlocked("loop[1].post_sese.successor", "loop[1].post_sese.begin", "loop[1].post_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].post_sese.precursor", "loop[1].post_sese.end"),
            new ConstraintSame("loop[1].post_sese.begin", "loop[1].post_sese.successor")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintSame("body_sese.precursor", "loop[1].body_sese.precursor"),
        new ConstraintSame("body_sese.begin", "loop[1].body_sese.begin"),
        new ConstraintSame("body_sese.end", "loop[1].body_sese.end"),
        new ConstraintSame("body_sese.successor", "loop[1].body_sese.successor"),
        new ConstraintStoreInst("output.store"),
        new ConstraintSecondOperand("output.access_pointer", "output.store"),
        new ConstraintGEPInst("output.access_pointer"),
        new ConstraintFirstOperand("output.base_pointer", "output.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.base_pointer")}),
          new ConstraintCFGDominateStrict("output.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.base_pointer")})}),
        new ConstraintSecondOperand("output.output_index", "output.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("output.output_index"),
            new ConstraintDFGEdge("output.offset_add.value", "output.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("output.output_index"),
            new ConstraintDFGEdge("output.offset_add.value", "output.output_index")}),
          new ConstraintSame("output.offset_add.value", "output.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.offset_add.input", "output.offset_add.value"),
            new ConstraintUnused("output.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("output.offset_add.value"),
              new ConstraintBitOrInst("output.offset_add.value"),
              new ConstraintFAddInst("output.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.input", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.addend", "output.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.addend", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.input", "output.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("output.offset_add.value"),
              new ConstraintFSubInst("output.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.input", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.addend", "output.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.offset_add.addend", "output.offset_add.value"),
                new ConstraintSecondOperand("output.offset_add.input", "output.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.offset_add.addend")}),
          new ConstraintCFGDominateStrict("output.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.offset_add.addend")})}),
        new ConstraintSame("output.stride_mul[0].value", "output.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.stride_mul[0].input", "output.stride_mul[0].value"),
            new ConstraintUnused("output.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("output.stride_mul[0].value"),
              new ConstraintLShiftInst("output.stride_mul[0].value"),
              new ConstraintFMulInst("output.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].input", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].input", "output.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("output.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].input", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.stride_mul[0].multiplier", "output.stride_mul[0].value"),
                new ConstraintSecondOperand("output.stride_mul[0].input", "output.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("output.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.stride_mul[0].multiplier")})}),
        new ConstraintSame("output.index_add[0].value", "output.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("output.index_add[0].input", "output.index_add[0].value"),
            new ConstraintUnused("output.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("output.index_add[0].value"),
              new ConstraintBitOrInst("output.index_add[0].value"),
              new ConstraintFAddInst("output.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].input", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].addend", "output.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].addend", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].input", "output.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("output.index_add[0].value"),
              new ConstraintFSubInst("output.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].input", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].addend", "output.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("output.index_add[0].addend", "output.index_add[0].value"),
                new ConstraintSecondOperand("output.index_add[0].input", "output.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("output.index_add[0].input"),
            new ConstraintDFGEdge("output.input_index[0]", "output.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("output.index_add[0].input"),
            new ConstraintDFGEdge("output.input_index[0]", "output.index_add[0].input")}),
          new ConstraintSame("output.input_index[0]", "output.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("output.index_add[0].input", "output.offset_add.value"),
            new ConstraintIntegerType("output.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("output.input_index[0]", "output.index_add[0].input"),
            new ConstraintIntegerType("output.offset_add.value")})}),
        new ConstraintSame("output.base_index", "output.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("output.base_index")}),
          new ConstraintCFGDominateStrict("output.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("output.base_index")})}),
        new ConstraintSame("loop[0].iterator", "output.input_index[0]"),
        new ConstraintLoadInst("input1.value"),
        new ConstraintFirstOperand("input1.access_pointer", "input1.value"),
        new ConstraintGEPInst("input1.access_pointer"),
        new ConstraintFirstOperand("input1.base_pointer", "input1.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.base_pointer")}),
          new ConstraintCFGDominateStrict("input1.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.base_pointer")})}),
        new ConstraintSecondOperand("input1.output_index", "input1.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input1.output_index"),
            new ConstraintDFGEdge("input1.offset_add.value", "input1.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input1.output_index"),
            new ConstraintDFGEdge("input1.offset_add.value", "input1.output_index")}),
          new ConstraintSame("input1.offset_add.value", "input1.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.offset_add.input", "input1.offset_add.value"),
            new ConstraintUnused("input1.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input1.offset_add.value"),
              new ConstraintBitOrInst("input1.offset_add.value"),
              new ConstraintFAddInst("input1.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.input", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.addend", "input1.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.addend", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.input", "input1.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input1.offset_add.value"),
              new ConstraintFSubInst("input1.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.input", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.addend", "input1.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.offset_add.addend", "input1.offset_add.value"),
                new ConstraintSecondOperand("input1.offset_add.input", "input1.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.offset_add.addend")}),
          new ConstraintCFGDominateStrict("input1.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.offset_add.addend")})}),
        new ConstraintSame("input1.stride_mul[1].value", "input1.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.stride_mul[1].input", "input1.stride_mul[1].value"),
            new ConstraintUnused("input1.stride_mul[1].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input1.stride_mul[1].value"),
              new ConstraintLShiftInst("input1.stride_mul[1].value"),
              new ConstraintFMulInst("input1.stride_mul[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input1.stride_mul[1].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value"),
                new ConstraintSecondOperand("input1.stride_mul[1].input", "input1.stride_mul[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.stride_mul[1].multiplier")}),
          new ConstraintCFGDominateStrict("input1.stride_mul[1].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.stride_mul[1].multiplier")})}),
        new ConstraintSame("input1.index_add[1].value", "input1.stride_mul[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.index_add[1].input", "input1.index_add[1].value"),
            new ConstraintUnused("input1.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input1.index_add[1].value"),
              new ConstraintBitOrInst("input1.index_add[1].value"),
              new ConstraintFAddInst("input1.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].input", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].addend", "input1.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].addend", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].input", "input1.index_add[1].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input1.index_add[1].value"),
              new ConstraintFSubInst("input1.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].input", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].addend", "input1.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[1].addend", "input1.index_add[1].value"),
                new ConstraintSecondOperand("input1.index_add[1].input", "input1.index_add[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input1.index_add[1].addend"),
            new ConstraintDFGEdge("input1.input_index[1]", "input1.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input1.index_add[1].addend"),
            new ConstraintDFGEdge("input1.input_index[1]", "input1.index_add[1].addend")}),
          new ConstraintSame("input1.input_index[1]", "input1.index_add[1].addend")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input1.index_add[1].addend", "input1.offset_add.value"),
            new ConstraintIntegerType("input1.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("input1.input_index[1]", "input1.index_add[1].addend"),
            new ConstraintIntegerType("input1.offset_add.value")})}),
        new ConstraintSame("input1.stride_mul[0].value", "input1.index_add[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.stride_mul[0].input", "input1.stride_mul[0].value"),
            new ConstraintUnused("input1.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input1.stride_mul[0].value"),
              new ConstraintLShiftInst("input1.stride_mul[0].value"),
              new ConstraintFMulInst("input1.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input1.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value"),
                new ConstraintSecondOperand("input1.stride_mul[0].input", "input1.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("input1.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.stride_mul[0].multiplier")})}),
        new ConstraintSame("input1.index_add[0].value", "input1.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input1.index_add[0].input", "input1.index_add[0].value"),
            new ConstraintUnused("input1.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input1.index_add[0].value"),
              new ConstraintBitOrInst("input1.index_add[0].value"),
              new ConstraintFAddInst("input1.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].input", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].addend", "input1.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].addend", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].input", "input1.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input1.index_add[0].value"),
              new ConstraintFSubInst("input1.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].input", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].addend", "input1.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input1.index_add[0].addend", "input1.index_add[0].value"),
                new ConstraintSecondOperand("input1.index_add[0].input", "input1.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input1.index_add[0].input"),
            new ConstraintDFGEdge("input1.input_index[0]", "input1.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input1.index_add[0].input"),
            new ConstraintDFGEdge("input1.input_index[0]", "input1.index_add[0].input")}),
          new ConstraintSame("input1.input_index[0]", "input1.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input1.index_add[0].input", "input1.input_index[1]"),
            new ConstraintIntegerType("input1.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("input1.input_index[0]", "input1.index_add[0].input"),
            new ConstraintIntegerType("input1.input_index[1]")})}),
        new ConstraintSame("input1.base_index", "input1.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input1.base_index")}),
          new ConstraintCFGDominateStrict("input1.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input1.base_index")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "input1.input_index[0]"),
            new ConstraintSame("loop[1].iterator", "input1.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "input1.input_index[1]"),
            new ConstraintSame("loop[1].iterator", "input1.input_index[0]")})}),
        new ConstraintLoadInst("input2.value"),
        new ConstraintFirstOperand("input2.access_pointer", "input2.value"),
        new ConstraintGEPInst("input2.access_pointer"),
        new ConstraintFirstOperand("input2.base_pointer", "input2.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.base_pointer")}),
          new ConstraintCFGDominateStrict("input2.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.base_pointer")})}),
        new ConstraintSecondOperand("input2.output_index", "input2.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input2.output_index"),
            new ConstraintDFGEdge("input2.offset_add.value", "input2.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input2.output_index"),
            new ConstraintDFGEdge("input2.offset_add.value", "input2.output_index")}),
          new ConstraintSame("input2.offset_add.value", "input2.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.offset_add.input", "input2.offset_add.value"),
            new ConstraintUnused("input2.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input2.offset_add.value"),
              new ConstraintBitOrInst("input2.offset_add.value"),
              new ConstraintFAddInst("input2.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.input", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.addend", "input2.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.addend", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.input", "input2.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input2.offset_add.value"),
              new ConstraintFSubInst("input2.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.input", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.addend", "input2.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.offset_add.addend", "input2.offset_add.value"),
                new ConstraintSecondOperand("input2.offset_add.input", "input2.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.offset_add.addend")}),
          new ConstraintCFGDominateStrict("input2.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.offset_add.addend")})}),
        new ConstraintSame("input2.stride_mul[0].value", "input2.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.stride_mul[0].input", "input2.stride_mul[0].value"),
            new ConstraintUnused("input2.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("input2.stride_mul[0].value"),
              new ConstraintLShiftInst("input2.stride_mul[0].value"),
              new ConstraintFMulInst("input2.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("input2.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value"),
                new ConstraintSecondOperand("input2.stride_mul[0].input", "input2.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("input2.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.stride_mul[0].multiplier")})}),
        new ConstraintSame("input2.index_add[0].value", "input2.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("input2.index_add[0].input", "input2.index_add[0].value"),
            new ConstraintUnused("input2.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("input2.index_add[0].value"),
              new ConstraintBitOrInst("input2.index_add[0].value"),
              new ConstraintFAddInst("input2.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].input", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].addend", "input2.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].addend", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].input", "input2.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("input2.index_add[0].value"),
              new ConstraintFSubInst("input2.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].input", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].addend", "input2.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("input2.index_add[0].addend", "input2.index_add[0].value"),
                new ConstraintSecondOperand("input2.index_add[0].input", "input2.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("input2.index_add[0].input"),
            new ConstraintDFGEdge("input2.input_index[0]", "input2.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("input2.index_add[0].input"),
            new ConstraintDFGEdge("input2.input_index[0]", "input2.index_add[0].input")}),
          new ConstraintSame("input2.input_index[0]", "input2.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("input2.index_add[0].input", "input2.offset_add.value"),
            new ConstraintIntegerType("input2.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("input2.input_index[0]", "input2.index_add[0].input"),
            new ConstraintIntegerType("input2.offset_add.value")})}),
        new ConstraintSame("input2.base_index", "input2.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("input2.base_index")}),
          new ConstraintCFGDominateStrict("input2.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("input2.base_index")})}),
        new ConstraintSame("loop[1].iterator", "input2.input_index[0]"),
        new ConstraintBranchInst("loop[1].precursor"),
        new ConstraintCFGEdge("loop[1].precursor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].end"),
        new ConstraintCFGEdge("loop[1].end", "loop[1].successor"),
        new ConstraintCFGDominate("loop[1].begin", "loop[1].end"),
        new ConstraintCFGPostdom("loop[1].end", "loop[1].begin"),
        new ConstraintCFGDominateStrict("loop[1].precursor", "loop[1].begin"),
        new ConstraintCFGPostdomStrict("loop[1].successor", "loop[1].end"),
        new ConstraintCFGBlocked("loop[1].begin", "loop[1].end", "loop[1].precursor"),
        new ConstraintCFGBlocked("loop[1].successor", "loop[1].begin", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.precursor", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.successor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].body_sese.precursor"),
        new ConstraintCFGEdge("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintBranchInst("loop[1].body_sese.end"),
        new ConstraintCFGEdge("loop[1].body_sese.end", "loop[1].body_sese.successor"),
        new ConstraintCFGDominate("loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintCFGPostdom("loop[1].body_sese.end", "loop[1].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[1].body_sese.successor", "loop[1].body_sese.end"),
        new ConstraintCFGBlocked("loop[1].body_sese.begin", "loop[1].body_sese.end", "loop[1].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[1].body_sese.successor", "loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintDistinct("loop[1].body_sese.begin", "loop[1].successor"),
        new ConstraintIncomingValue("loop[1].increment", "loop[1].body_sese.end", "loop[1].iterator"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].comparison"),
        new ConstraintICmpInst("loop[1].comparison"),
        new ConstraintDFGEdge("loop[1].comparison", "loop[1].end"),
        new ConstraintDFGEdge("loop[1].increment", "loop[1].iterator"),
        new ConstraintAddInst("loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iter_end", "loop[1].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintDFGEdge("loop[1].iter_begin", "loop[1].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintDFGEdge("loop[1].iter_step", "loop[1].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintIncomingValue("loop[1].new_ind", "loop[1].body_sese.end", "loop[1].old_ind"),
        new ConstraintFAddInst("loop[1].new_ind"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("loop[1].old_ind", "loop[1].new_ind"),
            new ConstraintSecondOperand("loop[1].dotvalue", "loop[1].new_ind")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("loop[1].dotvalue", "loop[1].new_ind"),
            new ConstraintSecondOperand("loop[1].old_ind", "loop[1].new_ind")})}),
        new ConstraintFMulInst("loop[1].dotvalue"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("input1.value", "loop[1].dotvalue"),
            new ConstraintSecondOperand("input2.value", "loop[1].dotvalue")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("input2.value", "loop[1].dotvalue"),
            new ConstraintSecondOperand("input1.value", "loop[1].dotvalue")})}),
        new ConstraintIncomingValue("loop[1].initial", "loop[1].precursor", "loop[1].old_ind"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintLoadInst("loop[1].initial"),
            new ConstraintDFGEdge("output.access_pointer", "loop[1].initial")}),
          new ConstraintAnd({
            new ConstraintFloatZero("loop[1].initial"),
            new ConstraintGEPInst("output.access_pointer")})}),
        new ConstraintFirstOperand("final_value", "output.store"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("scaled_reduc", "final_value"),
            new ConstraintUnused("scaled_prev")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("final_value"),
              new ConstraintBitOrInst("final_value"),
              new ConstraintFAddInst("final_value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_reduc", "final_value"),
                new ConstraintSecondOperand("scaled_prev", "final_value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_prev", "final_value"),
                new ConstraintSecondOperand("scaled_reduc", "final_value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("final_value"),
              new ConstraintFSubInst("final_value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_reduc", "final_value"),
                new ConstraintSecondOperand("scaled_prev", "final_value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("scaled_prev", "final_value"),
                new ConstraintSecondOperand("scaled_reduc", "final_value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[1].old_ind", "scaled_reduc"),
            new ConstraintUnused("alpha")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("scaled_reduc"),
              new ConstraintLShiftInst("scaled_reduc"),
              new ConstraintFMulInst("scaled_reduc")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("loop[1].old_ind", "scaled_reduc"),
                new ConstraintSecondOperand("alpha", "scaled_reduc")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("alpha", "scaled_reduc"),
                new ConstraintSecondOperand("loop[1].old_ind", "scaled_reduc")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("scaled_reduc"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("loop[1].old_ind", "scaled_reduc"),
                new ConstraintSecondOperand("alpha", "scaled_reduc")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("alpha", "scaled_reduc"),
                new ConstraintSecondOperand("loop[1].old_ind", "scaled_reduc")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("alpha")}),
          new ConstraintCFGDominateStrict("alpha", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("alpha")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("prev", "scaled_prev"),
            new ConstraintUnused("beta")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("scaled_prev"),
              new ConstraintLShiftInst("scaled_prev"),
              new ConstraintFMulInst("scaled_prev")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("prev", "scaled_prev"),
                new ConstraintSecondOperand("beta", "scaled_prev")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("beta", "scaled_prev"),
                new ConstraintSecondOperand("prev", "scaled_prev")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("scaled_prev"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("prev", "scaled_prev"),
                new ConstraintSecondOperand("beta", "scaled_prev")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("beta", "scaled_prev"),
                new ConstraintSecondOperand("prev", "scaled_prev")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintUnused("prev"),
            new ConstraintGEPInst("output.access_pointer")}),
          new ConstraintAnd({
            new ConstraintLoadInst("prev"),
            new ConstraintFirstOperand("output.access_pointer", "prev")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("beta")}),
          new ConstraintCFGDominateStrict("beta", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("beta")})})});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectSPMV(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintUnused("begin_idx.offset_add.addend"),
        new ConstraintLoadInst("begin_idx.value"),
        new ConstraintFirstOperand("begin_idx.access_pointer", "begin_idx.value"),
        new ConstraintGEPInst("begin_idx.access_pointer"),
        new ConstraintFirstOperand("begin_idx.base_pointer", "begin_idx.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("begin_idx.base_pointer")}),
          new ConstraintCFGDominateStrict("begin_idx.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("begin_idx.base_pointer")})}),
        new ConstraintSecondOperand("begin_idx.output_index", "begin_idx.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("begin_idx.output_index"),
            new ConstraintDFGEdge("begin_idx.offset_add.value", "begin_idx.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("begin_idx.output_index"),
            new ConstraintDFGEdge("begin_idx.offset_add.value", "begin_idx.output_index")}),
          new ConstraintSame("begin_idx.offset_add.value", "begin_idx.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("begin_idx.offset_add.input", "begin_idx.offset_add.value"),
            new ConstraintUnused("begin_idx.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("begin_idx.offset_add.value"),
              new ConstraintBitOrInst("begin_idx.offset_add.value"),
              new ConstraintFAddInst("begin_idx.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.offset_add.input", "begin_idx.offset_add.value"),
                new ConstraintSecondOperand("begin_idx.offset_add.addend", "begin_idx.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.offset_add.addend", "begin_idx.offset_add.value"),
                new ConstraintSecondOperand("begin_idx.offset_add.input", "begin_idx.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("begin_idx.offset_add.value"),
              new ConstraintFSubInst("begin_idx.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.offset_add.input", "begin_idx.offset_add.value"),
                new ConstraintSecondOperand("begin_idx.offset_add.addend", "begin_idx.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.offset_add.addend", "begin_idx.offset_add.value"),
                new ConstraintSecondOperand("begin_idx.offset_add.input", "begin_idx.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("begin_idx.offset_add.addend")}),
          new ConstraintCFGDominateStrict("begin_idx.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("begin_idx.offset_add.addend")})}),
        new ConstraintSame("begin_idx.stride_mul[0].value", "begin_idx.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value"),
            new ConstraintUnused("begin_idx.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("begin_idx.stride_mul[0].value"),
              new ConstraintLShiftInst("begin_idx.stride_mul[0].value"),
              new ConstraintFMulInst("begin_idx.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("begin_idx.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("begin_idx.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("begin_idx.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("begin_idx.stride_mul[0].multiplier")})}),
        new ConstraintSame("begin_idx.index_add[0].value", "begin_idx.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("begin_idx.index_add[0].input", "begin_idx.index_add[0].value"),
            new ConstraintUnused("begin_idx.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("begin_idx.index_add[0].value"),
              new ConstraintBitOrInst("begin_idx.index_add[0].value"),
              new ConstraintFAddInst("begin_idx.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.index_add[0].input", "begin_idx.index_add[0].value"),
                new ConstraintSecondOperand("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value"),
                new ConstraintSecondOperand("begin_idx.index_add[0].input", "begin_idx.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("begin_idx.index_add[0].value"),
              new ConstraintFSubInst("begin_idx.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.index_add[0].input", "begin_idx.index_add[0].value"),
                new ConstraintSecondOperand("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value"),
                new ConstraintSecondOperand("begin_idx.index_add[0].input", "begin_idx.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("begin_idx.index_add[0].input"),
            new ConstraintDFGEdge("begin_idx.input_index[0]", "begin_idx.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("begin_idx.index_add[0].input"),
            new ConstraintDFGEdge("begin_idx.input_index[0]", "begin_idx.index_add[0].input")}),
          new ConstraintSame("begin_idx.input_index[0]", "begin_idx.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("begin_idx.index_add[0].input", "begin_idx.offset_add.value"),
            new ConstraintIntegerType("begin_idx.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("begin_idx.input_index[0]", "begin_idx.index_add[0].input"),
            new ConstraintIntegerType("begin_idx.offset_add.value")})}),
        new ConstraintSame("begin_idx.base_index", "begin_idx.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("begin_idx.base_index")}),
          new ConstraintCFGDominateStrict("begin_idx.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("begin_idx.base_index")})}),
        new ConstraintSame("iterator", "begin_idx.input_index[0]"),
        new ConstraintSame("begin_idx.base_index", "end_idx.base_index"),
        new ConstraintSame("begin_idx.base_pointer", "end_idx.base_pointer"),
        new ConstraintLoadInst("end_idx.value"),
        new ConstraintFirstOperand("end_idx.access_pointer", "end_idx.value"),
        new ConstraintGEPInst("end_idx.access_pointer"),
        new ConstraintFirstOperand("end_idx.base_pointer", "end_idx.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("end_idx.base_pointer")}),
          new ConstraintCFGDominateStrict("end_idx.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("end_idx.base_pointer")})}),
        new ConstraintSecondOperand("end_idx.output_index", "end_idx.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("end_idx.output_index"),
            new ConstraintDFGEdge("end_idx.offset_add.value", "end_idx.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("end_idx.output_index"),
            new ConstraintDFGEdge("end_idx.offset_add.value", "end_idx.output_index")}),
          new ConstraintSame("end_idx.offset_add.value", "end_idx.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("end_idx.offset_add.input", "end_idx.offset_add.value"),
            new ConstraintUnused("end_idx.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("end_idx.offset_add.value"),
              new ConstraintBitOrInst("end_idx.offset_add.value"),
              new ConstraintFAddInst("end_idx.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.offset_add.input", "end_idx.offset_add.value"),
                new ConstraintSecondOperand("end_idx.offset_add.addend", "end_idx.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.offset_add.addend", "end_idx.offset_add.value"),
                new ConstraintSecondOperand("end_idx.offset_add.input", "end_idx.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("end_idx.offset_add.value"),
              new ConstraintFSubInst("end_idx.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.offset_add.input", "end_idx.offset_add.value"),
                new ConstraintSecondOperand("end_idx.offset_add.addend", "end_idx.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.offset_add.addend", "end_idx.offset_add.value"),
                new ConstraintSecondOperand("end_idx.offset_add.input", "end_idx.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("end_idx.offset_add.addend")}),
          new ConstraintCFGDominateStrict("end_idx.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("end_idx.offset_add.addend")})}),
        new ConstraintSame("end_idx.stride_mul[0].value", "end_idx.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value"),
            new ConstraintUnused("end_idx.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("end_idx.stride_mul[0].value"),
              new ConstraintLShiftInst("end_idx.stride_mul[0].value"),
              new ConstraintFMulInst("end_idx.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("end_idx.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value"),
                new ConstraintSecondOperand("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("end_idx.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("end_idx.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("end_idx.stride_mul[0].multiplier")})}),
        new ConstraintSame("end_idx.index_add[0].value", "end_idx.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("end_idx.index_add[0].input", "end_idx.index_add[0].value"),
            new ConstraintUnused("end_idx.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("end_idx.index_add[0].value"),
              new ConstraintBitOrInst("end_idx.index_add[0].value"),
              new ConstraintFAddInst("end_idx.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.index_add[0].input", "end_idx.index_add[0].value"),
                new ConstraintSecondOperand("end_idx.index_add[0].addend", "end_idx.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.index_add[0].addend", "end_idx.index_add[0].value"),
                new ConstraintSecondOperand("end_idx.index_add[0].input", "end_idx.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("end_idx.index_add[0].value"),
              new ConstraintFSubInst("end_idx.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.index_add[0].input", "end_idx.index_add[0].value"),
                new ConstraintSecondOperand("end_idx.index_add[0].addend", "end_idx.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("end_idx.index_add[0].addend", "end_idx.index_add[0].value"),
                new ConstraintSecondOperand("end_idx.index_add[0].input", "end_idx.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("end_idx.index_add[0].input"),
            new ConstraintDFGEdge("end_idx.input_index[0]", "end_idx.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("end_idx.index_add[0].input"),
            new ConstraintDFGEdge("end_idx.input_index[0]", "end_idx.index_add[0].input")}),
          new ConstraintSame("end_idx.input_index[0]", "end_idx.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("end_idx.index_add[0].input", "end_idx.offset_add.value"),
            new ConstraintIntegerType("end_idx.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("end_idx.input_index[0]", "end_idx.index_add[0].input"),
            new ConstraintIntegerType("end_idx.offset_add.value")})}),
        new ConstraintSame("end_idx.base_index", "end_idx.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("end_idx.base_index")}),
          new ConstraintCFGDominateStrict("end_idx.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("end_idx.base_index")})}),
        new ConstraintSame("iterator", "end_idx.input_index[0]"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("reduc.iter_begin"),
            new ConstraintDFGEdge("begin_idx.value", "reduc.iter_begin")}),
          new ConstraintAnd({
            new ConstraintSExtInst("reduc.iter_begin"),
            new ConstraintDFGEdge("begin_idx.value", "reduc.iter_begin")}),
          new ConstraintSame("begin_idx.value", "reduc.iter_begin")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("reduc.iter_end"),
            new ConstraintDFGEdge("end_idx.value", "reduc.iter_end")}),
          new ConstraintAnd({
            new ConstraintSExtInst("reduc.iter_end"),
            new ConstraintDFGEdge("end_idx.value", "reduc.iter_end")}),
          new ConstraintSame("end_idx.value", "reduc.iter_end")}),
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintLoadInst("idx_read.value"),
        new ConstraintFirstOperand("idx_read.access_pointer", "idx_read.value"),
        new ConstraintGEPInst("idx_read.access_pointer"),
        new ConstraintFirstOperand("idx_read.base_pointer", "idx_read.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("idx_read.base_pointer")}),
          new ConstraintCFGDominateStrict("idx_read.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("idx_read.base_pointer")})}),
        new ConstraintSecondOperand("idx_read.output_index", "idx_read.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("idx_read.output_index"),
            new ConstraintDFGEdge("idx_read.offset_add.value", "idx_read.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("idx_read.output_index"),
            new ConstraintDFGEdge("idx_read.offset_add.value", "idx_read.output_index")}),
          new ConstraintSame("idx_read.offset_add.value", "idx_read.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("idx_read.offset_add.input", "idx_read.offset_add.value"),
            new ConstraintUnused("idx_read.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("idx_read.offset_add.value"),
              new ConstraintBitOrInst("idx_read.offset_add.value"),
              new ConstraintFAddInst("idx_read.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.offset_add.input", "idx_read.offset_add.value"),
                new ConstraintSecondOperand("idx_read.offset_add.addend", "idx_read.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.offset_add.addend", "idx_read.offset_add.value"),
                new ConstraintSecondOperand("idx_read.offset_add.input", "idx_read.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("idx_read.offset_add.value"),
              new ConstraintFSubInst("idx_read.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.offset_add.input", "idx_read.offset_add.value"),
                new ConstraintSecondOperand("idx_read.offset_add.addend", "idx_read.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.offset_add.addend", "idx_read.offset_add.value"),
                new ConstraintSecondOperand("idx_read.offset_add.input", "idx_read.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("idx_read.offset_add.addend")}),
          new ConstraintCFGDominateStrict("idx_read.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("idx_read.offset_add.addend")})}),
        new ConstraintSame("idx_read.stride_mul[0].value", "idx_read.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("idx_read.stride_mul[0].input", "idx_read.stride_mul[0].value"),
            new ConstraintUnused("idx_read.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("idx_read.stride_mul[0].value"),
              new ConstraintLShiftInst("idx_read.stride_mul[0].value"),
              new ConstraintFMulInst("idx_read.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.stride_mul[0].input", "idx_read.stride_mul[0].value"),
                new ConstraintSecondOperand("idx_read.stride_mul[0].multiplier", "idx_read.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.stride_mul[0].multiplier", "idx_read.stride_mul[0].value"),
                new ConstraintSecondOperand("idx_read.stride_mul[0].input", "idx_read.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("idx_read.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.stride_mul[0].input", "idx_read.stride_mul[0].value"),
                new ConstraintSecondOperand("idx_read.stride_mul[0].multiplier", "idx_read.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.stride_mul[0].multiplier", "idx_read.stride_mul[0].value"),
                new ConstraintSecondOperand("idx_read.stride_mul[0].input", "idx_read.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("idx_read.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("idx_read.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("idx_read.stride_mul[0].multiplier")})}),
        new ConstraintSame("idx_read.index_add[0].value", "idx_read.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("idx_read.index_add[0].input", "idx_read.index_add[0].value"),
            new ConstraintUnused("idx_read.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("idx_read.index_add[0].value"),
              new ConstraintBitOrInst("idx_read.index_add[0].value"),
              new ConstraintFAddInst("idx_read.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.index_add[0].input", "idx_read.index_add[0].value"),
                new ConstraintSecondOperand("idx_read.index_add[0].addend", "idx_read.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.index_add[0].addend", "idx_read.index_add[0].value"),
                new ConstraintSecondOperand("idx_read.index_add[0].input", "idx_read.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("idx_read.index_add[0].value"),
              new ConstraintFSubInst("idx_read.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.index_add[0].input", "idx_read.index_add[0].value"),
                new ConstraintSecondOperand("idx_read.index_add[0].addend", "idx_read.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("idx_read.index_add[0].addend", "idx_read.index_add[0].value"),
                new ConstraintSecondOperand("idx_read.index_add[0].input", "idx_read.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("idx_read.index_add[0].input"),
            new ConstraintDFGEdge("idx_read.input_index[0]", "idx_read.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("idx_read.index_add[0].input"),
            new ConstraintDFGEdge("idx_read.input_index[0]", "idx_read.index_add[0].input")}),
          new ConstraintSame("idx_read.input_index[0]", "idx_read.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("idx_read.index_add[0].input", "idx_read.offset_add.value"),
            new ConstraintIntegerType("idx_read.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("idx_read.input_index[0]", "idx_read.index_add[0].input"),
            new ConstraintIntegerType("idx_read.offset_add.value")})}),
        new ConstraintSame("idx_read.base_index", "idx_read.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("idx_read.base_index")}),
          new ConstraintCFGDominateStrict("idx_read.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("idx_read.base_index")})}),
        new ConstraintSame("iterator", "idx_read.input_index[0]"),
        new ConstraintLoadInst("indir_read.value"),
        new ConstraintFirstOperand("indir_read.access_pointer", "indir_read.value"),
        new ConstraintGEPInst("indir_read.access_pointer"),
        new ConstraintFirstOperand("indir_read.base_pointer", "indir_read.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("indir_read.base_pointer")}),
          new ConstraintCFGDominateStrict("indir_read.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("indir_read.base_pointer")})}),
        new ConstraintSecondOperand("indir_read.output_index", "indir_read.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("indir_read.output_index"),
            new ConstraintDFGEdge("indir_read.offset_add.value", "indir_read.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("indir_read.output_index"),
            new ConstraintDFGEdge("indir_read.offset_add.value", "indir_read.output_index")}),
          new ConstraintSame("indir_read.offset_add.value", "indir_read.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("indir_read.offset_add.input", "indir_read.offset_add.value"),
            new ConstraintUnused("indir_read.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("indir_read.offset_add.value"),
              new ConstraintBitOrInst("indir_read.offset_add.value"),
              new ConstraintFAddInst("indir_read.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.offset_add.input", "indir_read.offset_add.value"),
                new ConstraintSecondOperand("indir_read.offset_add.addend", "indir_read.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.offset_add.addend", "indir_read.offset_add.value"),
                new ConstraintSecondOperand("indir_read.offset_add.input", "indir_read.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("indir_read.offset_add.value"),
              new ConstraintFSubInst("indir_read.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.offset_add.input", "indir_read.offset_add.value"),
                new ConstraintSecondOperand("indir_read.offset_add.addend", "indir_read.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.offset_add.addend", "indir_read.offset_add.value"),
                new ConstraintSecondOperand("indir_read.offset_add.input", "indir_read.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("indir_read.offset_add.addend")}),
          new ConstraintCFGDominateStrict("indir_read.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("indir_read.offset_add.addend")})}),
        new ConstraintSame("indir_read.stride_mul[0].value", "indir_read.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("indir_read.stride_mul[0].input", "indir_read.stride_mul[0].value"),
            new ConstraintUnused("indir_read.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("indir_read.stride_mul[0].value"),
              new ConstraintLShiftInst("indir_read.stride_mul[0].value"),
              new ConstraintFMulInst("indir_read.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.stride_mul[0].input", "indir_read.stride_mul[0].value"),
                new ConstraintSecondOperand("indir_read.stride_mul[0].multiplier", "indir_read.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.stride_mul[0].multiplier", "indir_read.stride_mul[0].value"),
                new ConstraintSecondOperand("indir_read.stride_mul[0].input", "indir_read.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("indir_read.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.stride_mul[0].input", "indir_read.stride_mul[0].value"),
                new ConstraintSecondOperand("indir_read.stride_mul[0].multiplier", "indir_read.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.stride_mul[0].multiplier", "indir_read.stride_mul[0].value"),
                new ConstraintSecondOperand("indir_read.stride_mul[0].input", "indir_read.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("indir_read.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("indir_read.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("indir_read.stride_mul[0].multiplier")})}),
        new ConstraintSame("indir_read.index_add[0].value", "indir_read.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("indir_read.index_add[0].input", "indir_read.index_add[0].value"),
            new ConstraintUnused("indir_read.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("indir_read.index_add[0].value"),
              new ConstraintBitOrInst("indir_read.index_add[0].value"),
              new ConstraintFAddInst("indir_read.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.index_add[0].input", "indir_read.index_add[0].value"),
                new ConstraintSecondOperand("indir_read.index_add[0].addend", "indir_read.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.index_add[0].addend", "indir_read.index_add[0].value"),
                new ConstraintSecondOperand("indir_read.index_add[0].input", "indir_read.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("indir_read.index_add[0].value"),
              new ConstraintFSubInst("indir_read.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.index_add[0].input", "indir_read.index_add[0].value"),
                new ConstraintSecondOperand("indir_read.index_add[0].addend", "indir_read.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("indir_read.index_add[0].addend", "indir_read.index_add[0].value"),
                new ConstraintSecondOperand("indir_read.index_add[0].input", "indir_read.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("indir_read.index_add[0].input"),
            new ConstraintDFGEdge("indir_read.input_index[0]", "indir_read.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("indir_read.index_add[0].input"),
            new ConstraintDFGEdge("indir_read.input_index[0]", "indir_read.index_add[0].input")}),
          new ConstraintSame("indir_read.input_index[0]", "indir_read.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("indir_read.index_add[0].input", "indir_read.offset_add.value"),
            new ConstraintIntegerType("indir_read.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("indir_read.input_index[0]", "indir_read.index_add[0].input"),
            new ConstraintIntegerType("indir_read.offset_add.value")})}),
        new ConstraintSame("indir_read.base_index", "indir_read.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("indir_read.base_index")}),
          new ConstraintCFGDominateStrict("indir_read.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("indir_read.base_index")})}),
        new ConstraintSame("idx_read.value", "indir_read.input_index[0]"),
        new ConstraintLoadInst("seq_read.value"),
        new ConstraintFirstOperand("seq_read.access_pointer", "seq_read.value"),
        new ConstraintGEPInst("seq_read.access_pointer"),
        new ConstraintFirstOperand("seq_read.base_pointer", "seq_read.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("seq_read.base_pointer")}),
          new ConstraintCFGDominateStrict("seq_read.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("seq_read.base_pointer")})}),
        new ConstraintSecondOperand("seq_read.output_index", "seq_read.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("seq_read.output_index"),
            new ConstraintDFGEdge("seq_read.offset_add.value", "seq_read.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("seq_read.output_index"),
            new ConstraintDFGEdge("seq_read.offset_add.value", "seq_read.output_index")}),
          new ConstraintSame("seq_read.offset_add.value", "seq_read.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("seq_read.offset_add.input", "seq_read.offset_add.value"),
            new ConstraintUnused("seq_read.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("seq_read.offset_add.value"),
              new ConstraintBitOrInst("seq_read.offset_add.value"),
              new ConstraintFAddInst("seq_read.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.offset_add.input", "seq_read.offset_add.value"),
                new ConstraintSecondOperand("seq_read.offset_add.addend", "seq_read.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.offset_add.addend", "seq_read.offset_add.value"),
                new ConstraintSecondOperand("seq_read.offset_add.input", "seq_read.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("seq_read.offset_add.value"),
              new ConstraintFSubInst("seq_read.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.offset_add.input", "seq_read.offset_add.value"),
                new ConstraintSecondOperand("seq_read.offset_add.addend", "seq_read.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.offset_add.addend", "seq_read.offset_add.value"),
                new ConstraintSecondOperand("seq_read.offset_add.input", "seq_read.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("seq_read.offset_add.addend")}),
          new ConstraintCFGDominateStrict("seq_read.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("seq_read.offset_add.addend")})}),
        new ConstraintSame("seq_read.stride_mul[0].value", "seq_read.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("seq_read.stride_mul[0].input", "seq_read.stride_mul[0].value"),
            new ConstraintUnused("seq_read.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("seq_read.stride_mul[0].value"),
              new ConstraintLShiftInst("seq_read.stride_mul[0].value"),
              new ConstraintFMulInst("seq_read.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.stride_mul[0].input", "seq_read.stride_mul[0].value"),
                new ConstraintSecondOperand("seq_read.stride_mul[0].multiplier", "seq_read.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.stride_mul[0].multiplier", "seq_read.stride_mul[0].value"),
                new ConstraintSecondOperand("seq_read.stride_mul[0].input", "seq_read.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("seq_read.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.stride_mul[0].input", "seq_read.stride_mul[0].value"),
                new ConstraintSecondOperand("seq_read.stride_mul[0].multiplier", "seq_read.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.stride_mul[0].multiplier", "seq_read.stride_mul[0].value"),
                new ConstraintSecondOperand("seq_read.stride_mul[0].input", "seq_read.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("seq_read.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("seq_read.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("seq_read.stride_mul[0].multiplier")})}),
        new ConstraintSame("seq_read.index_add[0].value", "seq_read.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("seq_read.index_add[0].input", "seq_read.index_add[0].value"),
            new ConstraintUnused("seq_read.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("seq_read.index_add[0].value"),
              new ConstraintBitOrInst("seq_read.index_add[0].value"),
              new ConstraintFAddInst("seq_read.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.index_add[0].input", "seq_read.index_add[0].value"),
                new ConstraintSecondOperand("seq_read.index_add[0].addend", "seq_read.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.index_add[0].addend", "seq_read.index_add[0].value"),
                new ConstraintSecondOperand("seq_read.index_add[0].input", "seq_read.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("seq_read.index_add[0].value"),
              new ConstraintFSubInst("seq_read.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.index_add[0].input", "seq_read.index_add[0].value"),
                new ConstraintSecondOperand("seq_read.index_add[0].addend", "seq_read.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("seq_read.index_add[0].addend", "seq_read.index_add[0].value"),
                new ConstraintSecondOperand("seq_read.index_add[0].input", "seq_read.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("seq_read.index_add[0].input"),
            new ConstraintDFGEdge("seq_read.input_index[0]", "seq_read.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("seq_read.index_add[0].input"),
            new ConstraintDFGEdge("seq_read.input_index[0]", "seq_read.index_add[0].input")}),
          new ConstraintSame("seq_read.input_index[0]", "seq_read.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("seq_read.index_add[0].input", "seq_read.offset_add.value"),
            new ConstraintIntegerType("seq_read.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("seq_read.input_index[0]", "seq_read.index_add[0].input"),
            new ConstraintIntegerType("seq_read.offset_add.value")})}),
        new ConstraintSame("seq_read.base_index", "seq_read.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("seq_read.base_index")}),
          new ConstraintCFGDominateStrict("seq_read.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("seq_read.base_index")})}),
        new ConstraintSame("iterator", "seq_read.input_index[0]"),
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintIncomingValue("new_ind", "body_sese.end", "old_ind"),
        new ConstraintFAddInst("new_ind"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("old_ind", "new_ind"),
            new ConstraintSecondOperand("dotvalue", "new_ind")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("dotvalue", "new_ind"),
            new ConstraintSecondOperand("old_ind", "new_ind")})}),
        new ConstraintFMulInst("dotvalue"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintFirstOperand("indir_read.value", "dotvalue"),
            new ConstraintSecondOperand("seq_read.value", "dotvalue")}),
          new ConstraintAnd({
            new ConstraintFirstOperand("seq_read.value", "dotvalue"),
            new ConstraintSecondOperand("indir_read.value", "dotvalue")})}),
        new ConstraintIncomingValue("initial", "precursor", "old_ind")});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectHisto(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("precursor"),
        new ConstraintCFGEdge("precursor", "begin"),
        new ConstraintBranchInst("end"),
        new ConstraintCFGEdge("end", "successor"),
        new ConstraintCFGDominate("begin", "end"),
        new ConstraintCFGPostdom("end", "begin"),
        new ConstraintCFGDominateStrict("precursor", "begin"),
        new ConstraintCFGPostdomStrict("successor", "end"),
        new ConstraintCFGBlocked("begin", "end", "precursor"),
        new ConstraintCFGBlocked("successor", "begin", "end"),
        new ConstraintSame("body_sese.precursor", "end"),
        new ConstraintSame("body_sese.successor", "begin"),
        new ConstraintBranchInst("body_sese.precursor"),
        new ConstraintCFGEdge("body_sese.precursor", "body_sese.begin"),
        new ConstraintBranchInst("body_sese.end"),
        new ConstraintCFGEdge("body_sese.end", "body_sese.successor"),
        new ConstraintCFGDominate("body_sese.begin", "body_sese.end"),
        new ConstraintCFGPostdom("body_sese.end", "body_sese.begin"),
        new ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin"),
        new ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end"),
        new ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor"),
        new ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end"),
        new ConstraintDistinct("body_sese.begin", "successor"),
        new ConstraintIncomingValue("increment", "body_sese.end", "iterator"),
        new ConstraintDFGEdge("iterator", "comparison"),
        new ConstraintICmpInst("comparison"),
        new ConstraintDFGEdge("comparison", "end"),
        new ConstraintDFGEdge("increment", "iterator"),
        new ConstraintAddInst("increment"),
        new ConstraintDFGEdge("iterator", "increment"),
        new ConstraintDFGEdge("iter_end", "comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_end")}),
          new ConstraintCFGDominateStrict("iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_end")})}),
        new ConstraintDFGEdge("iter_begin", "iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_begin")}),
          new ConstraintCFGDominateStrict("iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_begin")})}),
        new ConstraintDFGEdge("iter_step", "increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("iter_step")}),
          new ConstraintCFGDominateStrict("iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("iter_step")})}),
        new ConstraintStoreInst("store_instr"),
        new ConstraintCFGDominate("body_sese.begin", "store_instr"),
        new ConstraintCFGPostdom("body_sese.end", "store_instr"),
        new ConstraintCFGEdge("store_instr", "store_instr_after"),
        new ConstraintCFGBlocked("store_instr_after", "body_sese.successor", "store_instr"),
        new ConstraintCFGBlocked("store_instr_after", "body_sese.precursor", "store_instr"),
        new ConstraintSecondOperand("index_expr.output", "store_instr"),
        new ConstraintGEPInst("index_expr.output"),
        new ConstraintFirstOperand("index_expr.output", "old_value"),
        new ConstraintLoadInst("old_value"),
        new ConstraintFirstOperand("update_expr.output", "store_instr"),
        new ConstraintFirstOperand("reduction_array", "index_expr.output"),
        new ConstraintCollect(30, "i",
          new ConstraintAnd({
            new ConstraintLoadInst("read[i].value"),
            new ConstraintFirstOperand("read[i].access_pointer", "read[i].value"),
            new ConstraintGEPInst("read[i].access_pointer"),
            new ConstraintFirstOperand("read[i].base_pointer", "read[i].access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].base_pointer")}),
              new ConstraintCFGDominateStrict("read[i].base_pointer", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].base_pointer")})}),
            new ConstraintSecondOperand("read[i].output_index", "read[i].access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("read[i].output_index"),
                new ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index")}),
              new ConstraintAnd({
                new ConstraintSExtInst("read[i].output_index"),
                new ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index")}),
              new ConstraintSame("read[i].offset_add.value", "read[i].output_index")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("read[i].offset_add.input", "read[i].offset_add.value"),
                new ConstraintUnused("read[i].offset_add.addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("read[i].offset_add.value"),
                  new ConstraintBitOrInst("read[i].offset_add.value"),
                  new ConstraintFAddInst("read[i].offset_add.value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.input", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.addend", "read[i].offset_add.value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.addend", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.input", "read[i].offset_add.value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("read[i].offset_add.value"),
                  new ConstraintFSubInst("read[i].offset_add.value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.input", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.addend", "read[i].offset_add.value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].offset_add.addend", "read[i].offset_add.value"),
                    new ConstraintSecondOperand("read[i].offset_add.input", "read[i].offset_add.value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].offset_add.addend")}),
              new ConstraintCFGDominateStrict("read[i].offset_add.addend", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].offset_add.addend")})}),
            new ConstraintSame("read[i].stride_mul[0].value", "read[i].offset_add.input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"),
                new ConstraintUnused("read[i].stride_mul[0].multiplier")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintMulInst("read[i].stride_mul[0].value"),
                  new ConstraintLShiftInst("read[i].stride_mul[0].value"),
                  new ConstraintFMulInst("read[i].stride_mul[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")})})}),
              new ConstraintAnd({
                new ConstraintFDivInst("read[i].stride_mul[0].value"),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"),
                    new ConstraintSecondOperand("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].stride_mul[0].multiplier")}),
              new ConstraintCFGDominateStrict("read[i].stride_mul[0].multiplier", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].stride_mul[0].multiplier")})}),
            new ConstraintSame("read[i].index_add[0].value", "read[i].stride_mul[0].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("read[i].index_add[0].input", "read[i].index_add[0].value"),
                new ConstraintUnused("read[i].index_add[0].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("read[i].index_add[0].value"),
                  new ConstraintBitOrInst("read[i].index_add[0].value"),
                  new ConstraintFAddInst("read[i].index_add[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].input", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].addend", "read[i].index_add[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].addend", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].input", "read[i].index_add[0].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("read[i].index_add[0].value"),
                  new ConstraintFSubInst("read[i].index_add[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].input", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].addend", "read[i].index_add[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("read[i].index_add[0].addend", "read[i].index_add[0].value"),
                    new ConstraintSecondOperand("read[i].index_add[0].input", "read[i].index_add[0].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("read[i].index_add[0].input"),
                new ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input")}),
              new ConstraintAnd({
                new ConstraintSExtInst("read[i].index_add[0].input"),
                new ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input")}),
              new ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintDistinct("read[i].index_add[0].input", "read[i].offset_add.value"),
                new ConstraintIntegerType("read[i].input_index[0]")}),
              new ConstraintAnd({
                new ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input"),
                new ConstraintIntegerType("read[i].offset_add.value")})}),
            new ConstraintSame("read[i].base_index", "read[i].index_add[0].addend"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("read[i].base_index")}),
              new ConstraintCFGDominateStrict("read[i].base_index", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("read[i].base_index")})}),
            new ConstraintSame("iterator", "read[i].input_index[0]"),
            new ConstraintCFGDominate("body_sese.begin", "read[i].value"),
            new ConstraintCFGPostdom("body_sese.end", "read[i].value"),
            new ConstraintDistinct("read[i].base_pointer", "reduction_array")})),
        new ConstraintLoadInst("read[0].value"),
        new ConstraintCollect(2, "i",
          new ConstraintAnd({
            new ConstraintLoadInst("closure[i].value"),
            new ConstraintFirstOperand("closure[i].access_pointer", "closure[i].value"),
            new ConstraintGEPInst("closure[i].access_pointer"),
            new ConstraintFirstOperand("closure[i].base_pointer", "closure[i].access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("closure[i].base_pointer")}),
              new ConstraintCFGDominateStrict("closure[i].base_pointer", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("closure[i].base_pointer")})}),
            new ConstraintSecondOperand("closure[i].output_index", "closure[i].access_pointer"),
            new ConstraintCFGDominate("body_sese.begin", "closure[i].value"),
            new ConstraintCFGPostdom("body_sese.end", "closure[i].value"),
            new ConstraintDistinct("closure[i].base_pointer", "read[0].base_pointer"),
            new ConstraintDistinct("closure[i].base_pointer", "reduction_array")})),
        new ConstraintSame("update_expr.input[0]", "read[0].value"),
        new ConstraintSame("update_expr.input[1]", "read[1].value"),
        new ConstraintSame("update_expr.input[2]", "read[2].value"),
        new ConstraintSame("update_expr.input[3]", "read[3].value"),
        new ConstraintSame("update_expr.input[4]", "read[4].value"),
        new ConstraintSame("update_expr.input[5]", "read[5].value"),
        new ConstraintSame("update_expr.input[6]", "read[6].value"),
        new ConstraintSame("update_expr.input[7]", "read[7].value"),
        new ConstraintSame("update_expr.input[8]", "read[8].value"),
        new ConstraintSame("update_expr.input[9]", "read[9].value"),
        new ConstraintSame("update_expr.input[10]", "read[10].value"),
        new ConstraintSame("update_expr.input[11]", "read[11].value"),
        new ConstraintSame("update_expr.input[12]", "read[12].value"),
        new ConstraintSame("update_expr.input[13]", "read[13].value"),
        new ConstraintSame("update_expr.input[14]", "read[14].value"),
        new ConstraintSame("update_expr.input[15]", "read[15].value"),
        new ConstraintSame("update_expr.input[16]", "read[16].value"),
        new ConstraintSame("update_expr.input[17]", "read[17].value"),
        new ConstraintSame("update_expr.input[18]", "read[18].value"),
        new ConstraintSame("update_expr.input[19]", "read[19].value"),
        new ConstraintSame("update_expr.input[20]", "read[20].value"),
        new ConstraintSame("update_expr.input[21]", "read[21].value"),
        new ConstraintSame("update_expr.input[22]", "read[22].value"),
        new ConstraintSame("update_expr.input[23]", "read[23].value"),
        new ConstraintSame("update_expr.input[24]", "read[24].value"),
        new ConstraintSame("update_expr.input[25]", "read[25].value"),
        new ConstraintSame("update_expr.input[26]", "read[26].value"),
        new ConstraintSame("update_expr.input[27]", "read[27].value"),
        new ConstraintSame("update_expr.input[28]", "read[28].value"),
        new ConstraintSame("update_expr.input[29]", "closure[0].value"),
        new ConstraintSame("update_expr.input[30]", "closure[1].value"),
        new ConstraintSame("update_expr.input[31]", "old_value"),
        new ConstraintCollect(4, "i",
          new ConstraintCFGEdge("update_expr.origin[i]", "body_sese.begin")),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("update_expr.constants[i].value")}),
              new ConstraintCFGDominateStrict("update_expr.constants[i].value", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("update_expr.constants[i].value")})}),
            new ConstraintNotNumericConstant("update_expr.constants[i].value"),
            new ConstraintDFGEdge("update_expr.constants[i].value", "update_expr.constants[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "update_expr.constants[i].use")})),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintCFGDominate("begin", "update_expr.restrictions[i].value"),
            new ConstraintCFGDominateStrict("update_expr.restrictions[i].value", "body_sese.begin"),
            new ConstraintDFGEdge("update_expr.restrictions[i].value", "update_expr.restrictions[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "update_expr.restrictions[i].use")})),
        new ConstraintPDGDominate({
          std::vector<std::string>{
            "update_expr.restrictions[0].value", "update_expr.restrictions[1].value",
            "update_expr.restrictions[2].value", "update_expr.restrictions[3].value",
            "update_expr.restrictions[4].value", "update_expr.restrictions[5].value",
            "update_expr.restrictions[6].value", "update_expr.restrictions[7].value",
            "update_expr.restrictions[8].value", "update_expr.restrictions[9].value",
            "update_expr.restrictions[10].value", "update_expr.restrictions[11].value",
            "update_expr.restrictions[12].value", "update_expr.restrictions[13].value",
            "update_expr.restrictions[14].value", "update_expr.restrictions[15].value",
            "update_expr.restrictions[16].value", "update_expr.restrictions[17].value",
            "update_expr.restrictions[18].value", "update_expr.restrictions[19].value",
            "update_expr.restrictions[20].value", "update_expr.restrictions[21].value",
            "update_expr.restrictions[22].value", "update_expr.restrictions[23].value",
            "update_expr.restrictions[24].value", "update_expr.restrictions[25].value",
            "update_expr.restrictions[26].value", "update_expr.restrictions[27].value",
            "update_expr.restrictions[28].value", "update_expr.restrictions[29].value",
            "update_expr.restrictions[30].value", "update_expr.restrictions[31].value",
            "update_expr.restrictions[32].value", "update_expr.restrictions[33].value",
            "update_expr.restrictions[34].value", "update_expr.restrictions[35].value",
            "update_expr.restrictions[36].value", "update_expr.restrictions[37].value",
            "update_expr.restrictions[38].value", "update_expr.restrictions[39].value",
            "update_expr.restrictions[40].value", "update_expr.restrictions[41].value",
            "update_expr.restrictions[42].value", "update_expr.restrictions[43].value",
            "update_expr.restrictions[44].value", "update_expr.restrictions[45].value",
            "update_expr.restrictions[46].value", "update_expr.restrictions[47].value",
            "update_expr.restrictions[48].value", "update_expr.restrictions[49].value",
            "update_expr.restrictions[50].value", "update_expr.restrictions[51].value",
            "update_expr.restrictions[52].value", "update_expr.restrictions[53].value",
            "update_expr.restrictions[54].value", "update_expr.restrictions[55].value",
            "update_expr.restrictions[56].value", "update_expr.restrictions[57].value",
            "update_expr.restrictions[58].value", "update_expr.restrictions[59].value",
            "update_expr.restrictions[60].value", "update_expr.restrictions[61].value",
            "update_expr.restrictions[62].value", "update_expr.restrictions[63].value"},
          std::vector<std::string>{
            "update_expr.origin[0]", "update_expr.origin[1]", "update_expr.origin[2]",
            "update_expr.origin[3]", "update_expr.constants[0].value",
            "update_expr.constants[1].value", "update_expr.constants[2].value",
            "update_expr.constants[3].value", "update_expr.constants[4].value",
            "update_expr.constants[5].value", "update_expr.constants[6].value",
            "update_expr.constants[7].value", "update_expr.constants[8].value",
            "update_expr.constants[9].value", "update_expr.constants[10].value",
            "update_expr.constants[11].value", "update_expr.constants[12].value",
            "update_expr.constants[13].value", "update_expr.constants[14].value",
            "update_expr.constants[15].value", "update_expr.constants[16].value",
            "update_expr.constants[17].value", "update_expr.constants[18].value",
            "update_expr.constants[19].value", "update_expr.constants[20].value",
            "update_expr.constants[21].value", "update_expr.constants[22].value",
            "update_expr.constants[23].value", "update_expr.constants[24].value",
            "update_expr.constants[25].value", "update_expr.constants[26].value",
            "update_expr.constants[27].value", "update_expr.constants[28].value",
            "update_expr.constants[29].value", "update_expr.constants[30].value",
            "update_expr.constants[31].value", "update_expr.constants[32].value",
            "update_expr.constants[33].value", "update_expr.constants[34].value",
            "update_expr.constants[35].value", "update_expr.constants[36].value",
            "update_expr.constants[37].value", "update_expr.constants[38].value",
            "update_expr.constants[39].value", "update_expr.constants[40].value",
            "update_expr.constants[41].value", "update_expr.constants[42].value",
            "update_expr.constants[43].value", "update_expr.constants[44].value",
            "update_expr.constants[45].value", "update_expr.constants[46].value",
            "update_expr.constants[47].value", "update_expr.constants[48].value",
            "update_expr.constants[49].value", "update_expr.constants[50].value",
            "update_expr.constants[51].value", "update_expr.constants[52].value",
            "update_expr.constants[53].value", "update_expr.constants[54].value",
            "update_expr.constants[55].value", "update_expr.constants[56].value",
            "update_expr.constants[57].value", "update_expr.constants[58].value",
            "update_expr.constants[59].value", "update_expr.constants[60].value",
            "update_expr.constants[61].value", "update_expr.constants[62].value",
            "update_expr.constants[63].value", "update_expr.input[0]",
            "update_expr.input[1]", "update_expr.input[2]", "update_expr.input[3]",
            "update_expr.input[4]", "update_expr.input[5]", "update_expr.input[6]",
            "update_expr.input[7]", "update_expr.input[8]", "update_expr.input[9]",
            "update_expr.input[10]", "update_expr.input[11]", "update_expr.input[12]",
            "update_expr.input[13]", "update_expr.input[14]", "update_expr.input[15]",
            "update_expr.input[16]", "update_expr.input[17]", "update_expr.input[18]",
            "update_expr.input[19]", "update_expr.input[20]", "update_expr.input[21]",
            "update_expr.input[22]", "update_expr.input[23]", "update_expr.input[24]",
            "update_expr.input[25]", "update_expr.input[26]", "update_expr.input[27]",
            "update_expr.input[28]", "update_expr.input[29]", "update_expr.input[30]",
            "update_expr.input[31]"},
          std::vector<std::string>{
            "update_expr.output"}}),
        new ConstraintSame("index_expr.input[0]", "read[0].value"),
        new ConstraintSame("index_expr.input[1]", "read[1].value"),
        new ConstraintSame("index_expr.input[2]", "read[2].value"),
        new ConstraintSame("index_expr.input[3]", "read[3].value"),
        new ConstraintSame("index_expr.input[4]", "read[4].value"),
        new ConstraintSame("index_expr.input[5]", "read[5].value"),
        new ConstraintSame("index_expr.input[6]", "read[6].value"),
        new ConstraintSame("index_expr.input[7]", "read[7].value"),
        new ConstraintSame("index_expr.input[8]", "read[8].value"),
        new ConstraintSame("index_expr.input[9]", "read[9].value"),
        new ConstraintSame("index_expr.input[10]", "read[10].value"),
        new ConstraintSame("index_expr.input[11]", "read[11].value"),
        new ConstraintSame("index_expr.input[12]", "read[12].value"),
        new ConstraintSame("index_expr.input[13]", "read[13].value"),
        new ConstraintSame("index_expr.input[14]", "read[14].value"),
        new ConstraintSame("index_expr.input[15]", "read[15].value"),
        new ConstraintSame("index_expr.input[16]", "read[16].value"),
        new ConstraintSame("index_expr.input[17]", "read[17].value"),
        new ConstraintSame("index_expr.input[18]", "read[18].value"),
        new ConstraintSame("index_expr.input[19]", "read[19].value"),
        new ConstraintSame("index_expr.input[20]", "read[20].value"),
        new ConstraintSame("index_expr.input[21]", "read[21].value"),
        new ConstraintSame("index_expr.input[22]", "read[22].value"),
        new ConstraintSame("index_expr.input[23]", "read[23].value"),
        new ConstraintSame("index_expr.input[24]", "read[24].value"),
        new ConstraintSame("index_expr.input[25]", "read[25].value"),
        new ConstraintSame("index_expr.input[26]", "read[26].value"),
        new ConstraintSame("index_expr.input[27]", "read[27].value"),
        new ConstraintSame("index_expr.input[28]", "read[28].value"),
        new ConstraintSame("index_expr.input[29]", "closure[0].value"),
        new ConstraintSame("index_expr.input[30]", "closure[1].value"),
        new ConstraintUnused("index_expr.input[31]"),
        new ConstraintCollect(4, "i",
          new ConstraintCFGEdge("index_expr.origin[i]", "body_sese.begin")),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("index_expr.constants[i].value")}),
              new ConstraintCFGDominateStrict("index_expr.constants[i].value", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("index_expr.constants[i].value")})}),
            new ConstraintNotNumericConstant("index_expr.constants[i].value"),
            new ConstraintDFGEdge("index_expr.constants[i].value", "index_expr.constants[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "index_expr.constants[i].use")})),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintCFGDominate("begin", "index_expr.restrictions[i].value"),
            new ConstraintCFGDominateStrict("index_expr.restrictions[i].value", "body_sese.begin"),
            new ConstraintDFGEdge("index_expr.restrictions[i].value", "index_expr.restrictions[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "index_expr.restrictions[i].use")})),
        new ConstraintPDGDominate({
          std::vector<std::string>{
            "index_expr.restrictions[0].value", "index_expr.restrictions[1].value",
            "index_expr.restrictions[2].value", "index_expr.restrictions[3].value",
            "index_expr.restrictions[4].value", "index_expr.restrictions[5].value",
            "index_expr.restrictions[6].value", "index_expr.restrictions[7].value",
            "index_expr.restrictions[8].value", "index_expr.restrictions[9].value",
            "index_expr.restrictions[10].value", "index_expr.restrictions[11].value",
            "index_expr.restrictions[12].value", "index_expr.restrictions[13].value",
            "index_expr.restrictions[14].value", "index_expr.restrictions[15].value",
            "index_expr.restrictions[16].value", "index_expr.restrictions[17].value",
            "index_expr.restrictions[18].value", "index_expr.restrictions[19].value",
            "index_expr.restrictions[20].value", "index_expr.restrictions[21].value",
            "index_expr.restrictions[22].value", "index_expr.restrictions[23].value",
            "index_expr.restrictions[24].value", "index_expr.restrictions[25].value",
            "index_expr.restrictions[26].value", "index_expr.restrictions[27].value",
            "index_expr.restrictions[28].value", "index_expr.restrictions[29].value",
            "index_expr.restrictions[30].value", "index_expr.restrictions[31].value",
            "index_expr.restrictions[32].value", "index_expr.restrictions[33].value",
            "index_expr.restrictions[34].value", "index_expr.restrictions[35].value",
            "index_expr.restrictions[36].value", "index_expr.restrictions[37].value",
            "index_expr.restrictions[38].value", "index_expr.restrictions[39].value",
            "index_expr.restrictions[40].value", "index_expr.restrictions[41].value",
            "index_expr.restrictions[42].value", "index_expr.restrictions[43].value",
            "index_expr.restrictions[44].value", "index_expr.restrictions[45].value",
            "index_expr.restrictions[46].value", "index_expr.restrictions[47].value",
            "index_expr.restrictions[48].value", "index_expr.restrictions[49].value",
            "index_expr.restrictions[50].value", "index_expr.restrictions[51].value",
            "index_expr.restrictions[52].value", "index_expr.restrictions[53].value",
            "index_expr.restrictions[54].value", "index_expr.restrictions[55].value",
            "index_expr.restrictions[56].value", "index_expr.restrictions[57].value",
            "index_expr.restrictions[58].value", "index_expr.restrictions[59].value",
            "index_expr.restrictions[60].value", "index_expr.restrictions[61].value",
            "index_expr.restrictions[62].value", "index_expr.restrictions[63].value"},
          std::vector<std::string>{
            "index_expr.origin[0]", "index_expr.origin[1]", "index_expr.origin[2]",
            "index_expr.origin[3]", "index_expr.constants[0].value",
            "index_expr.constants[1].value", "index_expr.constants[2].value",
            "index_expr.constants[3].value", "index_expr.constants[4].value",
            "index_expr.constants[5].value", "index_expr.constants[6].value",
            "index_expr.constants[7].value", "index_expr.constants[8].value",
            "index_expr.constants[9].value", "index_expr.constants[10].value",
            "index_expr.constants[11].value", "index_expr.constants[12].value",
            "index_expr.constants[13].value", "index_expr.constants[14].value",
            "index_expr.constants[15].value", "index_expr.constants[16].value",
            "index_expr.constants[17].value", "index_expr.constants[18].value",
            "index_expr.constants[19].value", "index_expr.constants[20].value",
            "index_expr.constants[21].value", "index_expr.constants[22].value",
            "index_expr.constants[23].value", "index_expr.constants[24].value",
            "index_expr.constants[25].value", "index_expr.constants[26].value",
            "index_expr.constants[27].value", "index_expr.constants[28].value",
            "index_expr.constants[29].value", "index_expr.constants[30].value",
            "index_expr.constants[31].value", "index_expr.constants[32].value",
            "index_expr.constants[33].value", "index_expr.constants[34].value",
            "index_expr.constants[35].value", "index_expr.constants[36].value",
            "index_expr.constants[37].value", "index_expr.constants[38].value",
            "index_expr.constants[39].value", "index_expr.constants[40].value",
            "index_expr.constants[41].value", "index_expr.constants[42].value",
            "index_expr.constants[43].value", "index_expr.constants[44].value",
            "index_expr.constants[45].value", "index_expr.constants[46].value",
            "index_expr.constants[47].value", "index_expr.constants[48].value",
            "index_expr.constants[49].value", "index_expr.constants[50].value",
            "index_expr.constants[51].value", "index_expr.constants[52].value",
            "index_expr.constants[53].value", "index_expr.constants[54].value",
            "index_expr.constants[55].value", "index_expr.constants[56].value",
            "index_expr.constants[57].value", "index_expr.constants[58].value",
            "index_expr.constants[59].value", "index_expr.constants[60].value",
            "index_expr.constants[61].value", "index_expr.constants[62].value",
            "index_expr.constants[63].value", "index_expr.input[0]", "index_expr.input[1]",
            "index_expr.input[2]", "index_expr.input[3]", "index_expr.input[4]",
            "index_expr.input[5]", "index_expr.input[6]", "index_expr.input[7]",
            "index_expr.input[8]", "index_expr.input[9]", "index_expr.input[10]",
            "index_expr.input[11]", "index_expr.input[12]", "index_expr.input[13]",
            "index_expr.input[14]", "index_expr.input[15]", "index_expr.input[16]",
            "index_expr.input[17]", "index_expr.input[18]", "index_expr.input[19]",
            "index_expr.input[20]", "index_expr.input[21]", "index_expr.input[22]",
            "index_expr.input[23]", "index_expr.input[24]", "index_expr.input[25]",
            "index_expr.input[26]", "index_expr.input[27]", "index_expr.input[28]",
            "index_expr.input[29]", "index_expr.input[30]", "index_expr.input[31]"},
          std::vector<std::string>{
            "index_expr.output"}})});

    return Solution::Find(constraint, function, max_solutions);
}

std::vector<Solution> DetectStencil(llvm::Function& function, unsigned max_solutions)
{
    static const ConstraintAnd constraint({
        new ConstraintBranchInst("loop[0].precursor"),
        new ConstraintCFGEdge("loop[0].precursor", "loop[0].begin"),
        new ConstraintBranchInst("loop[0].end"),
        new ConstraintCFGEdge("loop[0].end", "loop[0].successor"),
        new ConstraintCFGDominate("loop[0].begin", "loop[0].end"),
        new ConstraintCFGPostdom("loop[0].end", "loop[0].begin"),
        new ConstraintCFGDominateStrict("loop[0].precursor", "loop[0].begin"),
        new ConstraintCFGPostdomStrict("loop[0].successor", "loop[0].end"),
        new ConstraintCFGBlocked("loop[0].begin", "loop[0].end", "loop[0].precursor"),
        new ConstraintCFGBlocked("loop[0].successor", "loop[0].begin", "loop[0].end"),
        new ConstraintSame("loop[0].body_sese.precursor", "loop[0].end"),
        new ConstraintSame("loop[0].body_sese.successor", "loop[0].begin"),
        new ConstraintBranchInst("loop[0].body_sese.precursor"),
        new ConstraintCFGEdge("loop[0].body_sese.precursor", "loop[0].body_sese.begin"),
        new ConstraintBranchInst("loop[0].body_sese.end"),
        new ConstraintCFGEdge("loop[0].body_sese.end", "loop[0].body_sese.successor"),
        new ConstraintCFGDominate("loop[0].body_sese.begin", "loop[0].body_sese.end"),
        new ConstraintCFGPostdom("loop[0].body_sese.end", "loop[0].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[0].body_sese.precursor", "loop[0].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[0].body_sese.successor", "loop[0].body_sese.end"),
        new ConstraintCFGBlocked("loop[0].body_sese.begin", "loop[0].body_sese.end", "loop[0].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[0].body_sese.successor", "loop[0].body_sese.begin", "loop[0].body_sese.end"),
        new ConstraintDistinct("loop[0].body_sese.begin", "loop[0].successor"),
        new ConstraintIncomingValue("loop[0].increment", "loop[0].body_sese.end", "loop[0].iterator"),
        new ConstraintDFGEdge("loop[0].iterator", "loop[0].comparison"),
        new ConstraintICmpInst("loop[0].comparison"),
        new ConstraintDFGEdge("loop[0].comparison", "loop[0].end"),
        new ConstraintDFGEdge("loop[0].increment", "loop[0].iterator"),
        new ConstraintAddInst("loop[0].increment"),
        new ConstraintDFGEdge("loop[0].iterator", "loop[0].increment"),
        new ConstraintDFGEdge("loop[0].iter_end", "loop[0].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_end")}),
          new ConstraintCFGDominateStrict("loop[0].iter_end", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_end")})}),
        new ConstraintDFGEdge("loop[0].iter_begin", "loop[0].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[0].iter_begin", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_begin")})}),
        new ConstraintDFGEdge("loop[0].iter_step", "loop[0].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintPreexecution("loop[0].iter_step")}),
          new ConstraintCFGDominateStrict("loop[0].iter_step", "loop[0].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[0].begin"),
            new ConstraintUnused("loop[0].iter_step")})}),
        new ConstraintSame("precursor", "loop[0].body_sese.precursor"),
        new ConstraintSame("begin", "loop[0].body_sese.begin"),
        new ConstraintSame("end", "loop[0].body_sese.end"),
        new ConstraintSame("successor", "loop[0].body_sese.successor"),
        new ConstraintBranchInst("loop[1].precursor"),
        new ConstraintCFGEdge("loop[1].precursor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].end"),
        new ConstraintCFGEdge("loop[1].end", "loop[1].successor"),
        new ConstraintCFGDominate("loop[1].begin", "loop[1].end"),
        new ConstraintCFGPostdom("loop[1].end", "loop[1].begin"),
        new ConstraintCFGDominateStrict("loop[1].precursor", "loop[1].begin"),
        new ConstraintCFGPostdomStrict("loop[1].successor", "loop[1].end"),
        new ConstraintCFGBlocked("loop[1].begin", "loop[1].end", "loop[1].precursor"),
        new ConstraintCFGBlocked("loop[1].successor", "loop[1].begin", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.precursor", "loop[1].end"),
        new ConstraintSame("loop[1].body_sese.successor", "loop[1].begin"),
        new ConstraintBranchInst("loop[1].body_sese.precursor"),
        new ConstraintCFGEdge("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintBranchInst("loop[1].body_sese.end"),
        new ConstraintCFGEdge("loop[1].body_sese.end", "loop[1].body_sese.successor"),
        new ConstraintCFGDominate("loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintCFGPostdom("loop[1].body_sese.end", "loop[1].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[1].body_sese.precursor", "loop[1].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[1].body_sese.successor", "loop[1].body_sese.end"),
        new ConstraintCFGBlocked("loop[1].body_sese.begin", "loop[1].body_sese.end", "loop[1].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[1].body_sese.successor", "loop[1].body_sese.begin", "loop[1].body_sese.end"),
        new ConstraintDistinct("loop[1].body_sese.begin", "loop[1].successor"),
        new ConstraintIncomingValue("loop[1].increment", "loop[1].body_sese.end", "loop[1].iterator"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].comparison"),
        new ConstraintICmpInst("loop[1].comparison"),
        new ConstraintDFGEdge("loop[1].comparison", "loop[1].end"),
        new ConstraintDFGEdge("loop[1].increment", "loop[1].iterator"),
        new ConstraintAddInst("loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iterator", "loop[1].increment"),
        new ConstraintDFGEdge("loop[1].iter_end", "loop[1].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintDFGEdge("loop[1].iter_begin", "loop[1].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintDFGEdge("loop[1].iter_step", "loop[1].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "loop[1].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[1].begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintSame("loop[0].body_sese.precursor", "loop[1].pre_sese.precursor"),
        new ConstraintSame("loop[0].body_sese.begin", "loop[1].pre_sese.begin"),
        new ConstraintSame("loop[1].pre_sese.end", "loop[1].precursor"),
        new ConstraintSame("loop[1].pre_sese.successor", "loop[1].begin"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[1].pre_sese.precursor"),
            new ConstraintCFGEdge("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin"),
            new ConstraintBranchInst("loop[1].pre_sese.end"),
            new ConstraintCFGEdge("loop[1].pre_sese.end", "loop[1].pre_sese.successor"),
            new ConstraintCFGDominate("loop[1].pre_sese.begin", "loop[1].pre_sese.end"),
            new ConstraintCFGPostdom("loop[1].pre_sese.end", "loop[1].pre_sese.begin"),
            new ConstraintCFGDominateStrict("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[1].pre_sese.successor", "loop[1].pre_sese.end"),
            new ConstraintCFGBlocked("loop[1].pre_sese.begin", "loop[1].pre_sese.end", "loop[1].pre_sese.precursor"),
            new ConstraintCFGBlocked("loop[1].pre_sese.successor", "loop[1].pre_sese.begin", "loop[1].pre_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].pre_sese.precursor", "loop[1].pre_sese.end"),
            new ConstraintSame("loop[1].pre_sese.begin", "loop[1].pre_sese.successor")})}),
        new ConstraintSame("loop[1].end", "loop[1].post_sese.precursor"),
        new ConstraintSame("loop[1].successor", "loop[1].post_sese.begin"),
        new ConstraintSame("loop[0].body_sese.end", "loop[1].post_sese.end"),
        new ConstraintSame("loop[0].body_sese.successor", "loop[1].post_sese.successor"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[1].post_sese.precursor"),
            new ConstraintCFGEdge("loop[1].post_sese.precursor", "loop[1].post_sese.begin"),
            new ConstraintBranchInst("loop[1].post_sese.end"),
            new ConstraintCFGEdge("loop[1].post_sese.end", "loop[1].post_sese.successor"),
            new ConstraintCFGDominate("loop[1].post_sese.begin", "loop[1].post_sese.end"),
            new ConstraintCFGPostdom("loop[1].post_sese.end", "loop[1].post_sese.begin"),
            new ConstraintCFGDominateStrict("loop[1].post_sese.precursor", "loop[1].post_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[1].post_sese.successor", "loop[1].post_sese.end"),
            new ConstraintCFGBlocked("loop[1].post_sese.begin", "loop[1].post_sese.end", "loop[1].post_sese.precursor"),
            new ConstraintCFGBlocked("loop[1].post_sese.successor", "loop[1].post_sese.begin", "loop[1].post_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[1].post_sese.precursor", "loop[1].post_sese.end"),
            new ConstraintSame("loop[1].post_sese.begin", "loop[1].post_sese.successor")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[1].iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_begin")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_step")}),
          new ConstraintCFGDominateStrict("loop[1].iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_step")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[1].iter_end")}),
          new ConstraintCFGDominateStrict("loop[1].iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[1].iter_end")})}),
        new ConstraintBranchInst("loop[2].precursor"),
        new ConstraintCFGEdge("loop[2].precursor", "loop[2].begin"),
        new ConstraintBranchInst("loop[2].end"),
        new ConstraintCFGEdge("loop[2].end", "loop[2].successor"),
        new ConstraintCFGDominate("loop[2].begin", "loop[2].end"),
        new ConstraintCFGPostdom("loop[2].end", "loop[2].begin"),
        new ConstraintCFGDominateStrict("loop[2].precursor", "loop[2].begin"),
        new ConstraintCFGPostdomStrict("loop[2].successor", "loop[2].end"),
        new ConstraintCFGBlocked("loop[2].begin", "loop[2].end", "loop[2].precursor"),
        new ConstraintCFGBlocked("loop[2].successor", "loop[2].begin", "loop[2].end"),
        new ConstraintSame("loop[2].body_sese.precursor", "loop[2].end"),
        new ConstraintSame("loop[2].body_sese.successor", "loop[2].begin"),
        new ConstraintBranchInst("loop[2].body_sese.precursor"),
        new ConstraintCFGEdge("loop[2].body_sese.precursor", "loop[2].body_sese.begin"),
        new ConstraintBranchInst("loop[2].body_sese.end"),
        new ConstraintCFGEdge("loop[2].body_sese.end", "loop[2].body_sese.successor"),
        new ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].body_sese.end"),
        new ConstraintCFGPostdom("loop[2].body_sese.end", "loop[2].body_sese.begin"),
        new ConstraintCFGDominateStrict("loop[2].body_sese.precursor", "loop[2].body_sese.begin"),
        new ConstraintCFGPostdomStrict("loop[2].body_sese.successor", "loop[2].body_sese.end"),
        new ConstraintCFGBlocked("loop[2].body_sese.begin", "loop[2].body_sese.end", "loop[2].body_sese.precursor"),
        new ConstraintCFGBlocked("loop[2].body_sese.successor", "loop[2].body_sese.begin", "loop[2].body_sese.end"),
        new ConstraintDistinct("loop[2].body_sese.begin", "loop[2].successor"),
        new ConstraintIncomingValue("loop[2].increment", "loop[2].body_sese.end", "loop[2].iterator"),
        new ConstraintDFGEdge("loop[2].iterator", "loop[2].comparison"),
        new ConstraintICmpInst("loop[2].comparison"),
        new ConstraintDFGEdge("loop[2].comparison", "loop[2].end"),
        new ConstraintDFGEdge("loop[2].increment", "loop[2].iterator"),
        new ConstraintAddInst("loop[2].increment"),
        new ConstraintDFGEdge("loop[2].iterator", "loop[2].increment"),
        new ConstraintDFGEdge("loop[2].iter_end", "loop[2].comparison"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_end")}),
          new ConstraintCFGDominateStrict("loop[2].iter_end", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_end")})}),
        new ConstraintDFGEdge("loop[2].iter_begin", "loop[2].iterator"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[2].iter_begin", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_begin")})}),
        new ConstraintDFGEdge("loop[2].iter_step", "loop[2].increment"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintPreexecution("loop[2].iter_step")}),
          new ConstraintCFGDominateStrict("loop[2].iter_step", "loop[2].begin"),
          new ConstraintAnd({
            new ConstraintInstruction("loop[2].begin"),
            new ConstraintUnused("loop[2].iter_step")})}),
        new ConstraintSame("loop[1].body_sese.precursor", "loop[2].pre_sese.precursor"),
        new ConstraintSame("loop[1].body_sese.begin", "loop[2].pre_sese.begin"),
        new ConstraintSame("loop[2].pre_sese.end", "loop[2].precursor"),
        new ConstraintSame("loop[2].pre_sese.successor", "loop[2].begin"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[2].pre_sese.precursor"),
            new ConstraintCFGEdge("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin"),
            new ConstraintBranchInst("loop[2].pre_sese.end"),
            new ConstraintCFGEdge("loop[2].pre_sese.end", "loop[2].pre_sese.successor"),
            new ConstraintCFGDominate("loop[2].pre_sese.begin", "loop[2].pre_sese.end"),
            new ConstraintCFGPostdom("loop[2].pre_sese.end", "loop[2].pre_sese.begin"),
            new ConstraintCFGDominateStrict("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[2].pre_sese.successor", "loop[2].pre_sese.end"),
            new ConstraintCFGBlocked("loop[2].pre_sese.begin", "loop[2].pre_sese.end", "loop[2].pre_sese.precursor"),
            new ConstraintCFGBlocked("loop[2].pre_sese.successor", "loop[2].pre_sese.begin", "loop[2].pre_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[2].pre_sese.precursor", "loop[2].pre_sese.end"),
            new ConstraintSame("loop[2].pre_sese.begin", "loop[2].pre_sese.successor")})}),
        new ConstraintSame("loop[2].end", "loop[2].post_sese.precursor"),
        new ConstraintSame("loop[2].successor", "loop[2].post_sese.begin"),
        new ConstraintSame("loop[1].body_sese.end", "loop[2].post_sese.end"),
        new ConstraintSame("loop[1].body_sese.successor", "loop[2].post_sese.successor"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintBranchInst("loop[2].post_sese.precursor"),
            new ConstraintCFGEdge("loop[2].post_sese.precursor", "loop[2].post_sese.begin"),
            new ConstraintBranchInst("loop[2].post_sese.end"),
            new ConstraintCFGEdge("loop[2].post_sese.end", "loop[2].post_sese.successor"),
            new ConstraintCFGDominate("loop[2].post_sese.begin", "loop[2].post_sese.end"),
            new ConstraintCFGPostdom("loop[2].post_sese.end", "loop[2].post_sese.begin"),
            new ConstraintCFGDominateStrict("loop[2].post_sese.precursor", "loop[2].post_sese.begin"),
            new ConstraintCFGPostdomStrict("loop[2].post_sese.successor", "loop[2].post_sese.end"),
            new ConstraintCFGBlocked("loop[2].post_sese.begin", "loop[2].post_sese.end", "loop[2].post_sese.precursor"),
            new ConstraintCFGBlocked("loop[2].post_sese.successor", "loop[2].post_sese.begin", "loop[2].post_sese.end")}),
          new ConstraintAnd({
            new ConstraintSame("loop[2].post_sese.precursor", "loop[2].post_sese.end"),
            new ConstraintSame("loop[2].post_sese.begin", "loop[2].post_sese.successor")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[2].iter_begin")}),
          new ConstraintCFGDominateStrict("loop[2].iter_begin", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[2].iter_begin")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[2].iter_step")}),
          new ConstraintCFGDominateStrict("loop[2].iter_step", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[2].iter_step")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("loop[2].iter_end")}),
          new ConstraintCFGDominateStrict("loop[2].iter_end", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("loop[2].iter_end")})}),
        new ConstraintSame("body_sese.precursor", "loop[2].body_sese.precursor"),
        new ConstraintSame("body_sese.begin", "loop[2].body_sese.begin"),
        new ConstraintSame("body_sese.end", "loop[2].body_sese.end"),
        new ConstraintSame("body_sese.successor", "loop[2].body_sese.successor"),
        new ConstraintStoreInst("write.store"),
        new ConstraintSecondOperand("write.access_pointer", "write.store"),
        new ConstraintGEPInst("write.access_pointer"),
        new ConstraintFirstOperand("write.base_pointer", "write.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("write.base_pointer")}),
          new ConstraintCFGDominateStrict("write.base_pointer", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("write.base_pointer")})}),
        new ConstraintSecondOperand("write.output_index", "write.access_pointer"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("write.output_index"),
            new ConstraintDFGEdge("write.offset_add.value", "write.output_index")}),
          new ConstraintAnd({
            new ConstraintSExtInst("write.output_index"),
            new ConstraintDFGEdge("write.offset_add.value", "write.output_index")}),
          new ConstraintSame("write.offset_add.value", "write.output_index")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.offset_add.input", "write.offset_add.value"),
            new ConstraintUnused("write.offset_add.addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("write.offset_add.value"),
              new ConstraintBitOrInst("write.offset_add.value"),
              new ConstraintFAddInst("write.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.offset_add.input", "write.offset_add.value"),
                new ConstraintSecondOperand("write.offset_add.addend", "write.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.offset_add.addend", "write.offset_add.value"),
                new ConstraintSecondOperand("write.offset_add.input", "write.offset_add.value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("write.offset_add.value"),
              new ConstraintFSubInst("write.offset_add.value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.offset_add.input", "write.offset_add.value"),
                new ConstraintSecondOperand("write.offset_add.addend", "write.offset_add.value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.offset_add.addend", "write.offset_add.value"),
                new ConstraintSecondOperand("write.offset_add.input", "write.offset_add.value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("write.offset_add.addend")}),
          new ConstraintCFGDominateStrict("write.offset_add.addend", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("write.offset_add.addend")})}),
        new ConstraintSame("write.stride_mul[2].value", "write.offset_add.input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.stride_mul[2].input", "write.stride_mul[2].value"),
            new ConstraintUnused("write.stride_mul[2].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("write.stride_mul[2].value"),
              new ConstraintLShiftInst("write.stride_mul[2].value"),
              new ConstraintFMulInst("write.stride_mul[2].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[2].input", "write.stride_mul[2].value"),
                new ConstraintSecondOperand("write.stride_mul[2].multiplier", "write.stride_mul[2].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[2].multiplier", "write.stride_mul[2].value"),
                new ConstraintSecondOperand("write.stride_mul[2].input", "write.stride_mul[2].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("write.stride_mul[2].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[2].input", "write.stride_mul[2].value"),
                new ConstraintSecondOperand("write.stride_mul[2].multiplier", "write.stride_mul[2].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[2].multiplier", "write.stride_mul[2].value"),
                new ConstraintSecondOperand("write.stride_mul[2].input", "write.stride_mul[2].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("write.stride_mul[2].multiplier")}),
          new ConstraintCFGDominateStrict("write.stride_mul[2].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("write.stride_mul[2].multiplier")})}),
        new ConstraintSame("write.index_add[2].value", "write.stride_mul[2].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.index_add[2].input", "write.index_add[2].value"),
            new ConstraintUnused("write.index_add[2].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("write.index_add[2].value"),
              new ConstraintBitOrInst("write.index_add[2].value"),
              new ConstraintFAddInst("write.index_add[2].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[2].input", "write.index_add[2].value"),
                new ConstraintSecondOperand("write.index_add[2].addend", "write.index_add[2].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[2].addend", "write.index_add[2].value"),
                new ConstraintSecondOperand("write.index_add[2].input", "write.index_add[2].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("write.index_add[2].value"),
              new ConstraintFSubInst("write.index_add[2].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[2].input", "write.index_add[2].value"),
                new ConstraintSecondOperand("write.index_add[2].addend", "write.index_add[2].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[2].addend", "write.index_add[2].value"),
                new ConstraintSecondOperand("write.index_add[2].input", "write.index_add[2].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("write.index_add[2].addend"),
            new ConstraintDFGEdge("write.input_index[2]", "write.index_add[2].addend")}),
          new ConstraintAnd({
            new ConstraintSExtInst("write.index_add[2].addend"),
            new ConstraintDFGEdge("write.input_index[2]", "write.index_add[2].addend")}),
          new ConstraintSame("write.input_index[2]", "write.index_add[2].addend")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("write.index_add[2].addend", "write.offset_add.value"),
            new ConstraintIntegerType("write.input_index[2]")}),
          new ConstraintAnd({
            new ConstraintSame("write.input_index[2]", "write.index_add[2].addend"),
            new ConstraintIntegerType("write.offset_add.value")})}),
        new ConstraintSame("write.stride_mul[1].value", "write.index_add[2].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.stride_mul[1].input", "write.stride_mul[1].value"),
            new ConstraintUnused("write.stride_mul[1].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("write.stride_mul[1].value"),
              new ConstraintLShiftInst("write.stride_mul[1].value"),
              new ConstraintFMulInst("write.stride_mul[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[1].input", "write.stride_mul[1].value"),
                new ConstraintSecondOperand("write.stride_mul[1].multiplier", "write.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[1].multiplier", "write.stride_mul[1].value"),
                new ConstraintSecondOperand("write.stride_mul[1].input", "write.stride_mul[1].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("write.stride_mul[1].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[1].input", "write.stride_mul[1].value"),
                new ConstraintSecondOperand("write.stride_mul[1].multiplier", "write.stride_mul[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[1].multiplier", "write.stride_mul[1].value"),
                new ConstraintSecondOperand("write.stride_mul[1].input", "write.stride_mul[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("write.stride_mul[1].multiplier")}),
          new ConstraintCFGDominateStrict("write.stride_mul[1].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("write.stride_mul[1].multiplier")})}),
        new ConstraintSame("write.index_add[1].value", "write.stride_mul[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.index_add[1].input", "write.index_add[1].value"),
            new ConstraintUnused("write.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("write.index_add[1].value"),
              new ConstraintBitOrInst("write.index_add[1].value"),
              new ConstraintFAddInst("write.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[1].input", "write.index_add[1].value"),
                new ConstraintSecondOperand("write.index_add[1].addend", "write.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[1].addend", "write.index_add[1].value"),
                new ConstraintSecondOperand("write.index_add[1].input", "write.index_add[1].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("write.index_add[1].value"),
              new ConstraintFSubInst("write.index_add[1].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[1].input", "write.index_add[1].value"),
                new ConstraintSecondOperand("write.index_add[1].addend", "write.index_add[1].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[1].addend", "write.index_add[1].value"),
                new ConstraintSecondOperand("write.index_add[1].input", "write.index_add[1].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("write.index_add[1].addend"),
            new ConstraintDFGEdge("write.input_index[1]", "write.index_add[1].addend")}),
          new ConstraintAnd({
            new ConstraintSExtInst("write.index_add[1].addend"),
            new ConstraintDFGEdge("write.input_index[1]", "write.index_add[1].addend")}),
          new ConstraintSame("write.input_index[1]", "write.index_add[1].addend")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("write.index_add[1].addend", "write.input_index[2]"),
            new ConstraintIntegerType("write.input_index[1]")}),
          new ConstraintAnd({
            new ConstraintSame("write.input_index[1]", "write.index_add[1].addend"),
            new ConstraintIntegerType("write.input_index[2]")})}),
        new ConstraintSame("write.stride_mul[0].value", "write.index_add[1].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.stride_mul[0].input", "write.stride_mul[0].value"),
            new ConstraintUnused("write.stride_mul[0].multiplier")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintMulInst("write.stride_mul[0].value"),
              new ConstraintLShiftInst("write.stride_mul[0].value"),
              new ConstraintFMulInst("write.stride_mul[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[0].input", "write.stride_mul[0].value"),
                new ConstraintSecondOperand("write.stride_mul[0].multiplier", "write.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[0].multiplier", "write.stride_mul[0].value"),
                new ConstraintSecondOperand("write.stride_mul[0].input", "write.stride_mul[0].value")})})}),
          new ConstraintAnd({
            new ConstraintFDivInst("write.stride_mul[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[0].input", "write.stride_mul[0].value"),
                new ConstraintSecondOperand("write.stride_mul[0].multiplier", "write.stride_mul[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.stride_mul[0].multiplier", "write.stride_mul[0].value"),
                new ConstraintSecondOperand("write.stride_mul[0].input", "write.stride_mul[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("write.stride_mul[0].multiplier")}),
          new ConstraintCFGDominateStrict("write.stride_mul[0].multiplier", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("write.stride_mul[0].multiplier")})}),
        new ConstraintSame("write.index_add[0].value", "write.stride_mul[0].input"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("write.index_add[0].input", "write.index_add[0].value"),
            new ConstraintUnused("write.index_add[0].addend")}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAddInst("write.index_add[0].value"),
              new ConstraintBitOrInst("write.index_add[0].value"),
              new ConstraintFAddInst("write.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[0].input", "write.index_add[0].value"),
                new ConstraintSecondOperand("write.index_add[0].addend", "write.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[0].addend", "write.index_add[0].value"),
                new ConstraintSecondOperand("write.index_add[0].input", "write.index_add[0].value")})})}),
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintSubInst("write.index_add[0].value"),
              new ConstraintFSubInst("write.index_add[0].value")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[0].input", "write.index_add[0].value"),
                new ConstraintSecondOperand("write.index_add[0].addend", "write.index_add[0].value")}),
              new ConstraintAnd({
                new ConstraintFirstOperand("write.index_add[0].addend", "write.index_add[0].value"),
                new ConstraintSecondOperand("write.index_add[0].input", "write.index_add[0].value")})})})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintZExtInst("write.index_add[0].input"),
            new ConstraintDFGEdge("write.input_index[0]", "write.index_add[0].input")}),
          new ConstraintAnd({
            new ConstraintSExtInst("write.index_add[0].input"),
            new ConstraintDFGEdge("write.input_index[0]", "write.index_add[0].input")}),
          new ConstraintSame("write.input_index[0]", "write.index_add[0].input")}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintDistinct("write.index_add[0].input", "write.input_index[1]"),
            new ConstraintIntegerType("write.input_index[0]")}),
          new ConstraintAnd({
            new ConstraintSame("write.input_index[0]", "write.index_add[0].input"),
            new ConstraintIntegerType("write.input_index[1]")})}),
        new ConstraintSame("write.base_index", "write.index_add[0].addend"),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintPreexecution("write.base_index")}),
          new ConstraintCFGDominateStrict("write.base_index", "begin"),
          new ConstraintAnd({
            new ConstraintInstruction("begin"),
            new ConstraintUnused("write.base_index")})}),
        new ConstraintOr({
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "write.input_index[0]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("loop[1].iterator", "write.input_index[1]"),
                new ConstraintSame("loop[2].iterator", "write.input_index[2]")}),
              new ConstraintAnd({
                new ConstraintSame("loop[1].iterator", "write.input_index[2]"),
                new ConstraintSame("loop[2].iterator", "write.input_index[1]")})})}),
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "write.input_index[1]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("loop[1].iterator", "write.input_index[0]"),
                new ConstraintSame("loop[2].iterator", "write.input_index[2]")}),
              new ConstraintAnd({
                new ConstraintSame("loop[1].iterator", "write.input_index[2]"),
                new ConstraintSame("loop[2].iterator", "write.input_index[0]")})})}),
          new ConstraintAnd({
            new ConstraintSame("loop[0].iterator", "write.input_index[2]"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("loop[1].iterator", "write.input_index[1]"),
                new ConstraintSame("loop[2].iterator", "write.input_index[0]")}),
              new ConstraintAnd({
                new ConstraintSame("loop[1].iterator", "write.input_index[0]"),
                new ConstraintSame("loop[2].iterator", "write.input_index[1]")})})})}),
        new ConstraintFirstOperand("compute_expr.output", "write.store"),
        new ConstraintCollect(32, "i",
          new ConstraintAnd({
            new ConstraintLoadInst("reads[i].value"),
            new ConstraintFirstOperand("reads[i].access_pointer", "reads[i].value"),
            new ConstraintGEPInst("reads[i].read.access_pointer"),
            new ConstraintFirstOperand("reads[i].read.base_pointer", "reads[i].read.access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("reads[i].read.base_pointer")}),
              new ConstraintCFGDominateStrict("reads[i].read.base_pointer", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("reads[i].read.base_pointer")})}),
            new ConstraintSecondOperand("reads[i].read.output_index", "reads[i].read.access_pointer"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("reads[i].read.output_index"),
                new ConstraintDFGEdge("reads[i].read.offset_add.value", "reads[i].read.output_index")}),
              new ConstraintAnd({
                new ConstraintSExtInst("reads[i].read.output_index"),
                new ConstraintDFGEdge("reads[i].read.offset_add.value", "reads[i].read.output_index")}),
              new ConstraintSame("reads[i].read.offset_add.value", "reads[i].read.output_index")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.offset_add.input", "reads[i].read.offset_add.value"),
                new ConstraintUnused("reads[i].read.offset_add.addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].read.offset_add.value"),
                  new ConstraintBitOrInst("reads[i].read.offset_add.value"),
                  new ConstraintFAddInst("reads[i].read.offset_add.value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.offset_add.input", "reads[i].read.offset_add.value"),
                    new ConstraintSecondOperand("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value"),
                    new ConstraintSecondOperand("reads[i].read.offset_add.input", "reads[i].read.offset_add.value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].read.offset_add.value"),
                  new ConstraintFSubInst("reads[i].read.offset_add.value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.offset_add.input", "reads[i].read.offset_add.value"),
                    new ConstraintSecondOperand("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value"),
                    new ConstraintSecondOperand("reads[i].read.offset_add.input", "reads[i].read.offset_add.value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("reads[i].read.offset_add.addend")}),
              new ConstraintCFGDominateStrict("reads[i].read.offset_add.addend", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("reads[i].read.offset_add.addend")})}),
            new ConstraintSame("reads[i].read.stride_mul[2].value", "reads[i].read.offset_add.input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value"),
                new ConstraintUnused("reads[i].read.stride_mul[2].multiplier")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintMulInst("reads[i].read.stride_mul[2].value"),
                  new ConstraintLShiftInst("reads[i].read.stride_mul[2].value"),
                  new ConstraintFMulInst("reads[i].read.stride_mul[2].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value")})})}),
              new ConstraintAnd({
                new ConstraintFDivInst("reads[i].read.stride_mul[2].value"),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("reads[i].read.stride_mul[2].multiplier")}),
              new ConstraintCFGDominateStrict("reads[i].read.stride_mul[2].multiplier", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("reads[i].read.stride_mul[2].multiplier")})}),
            new ConstraintSame("reads[i].read.index_add[2].value", "reads[i].read.stride_mul[2].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value"),
                new ConstraintUnused("reads[i].read.index_add[2].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].read.index_add[2].value"),
                  new ConstraintBitOrInst("reads[i].read.index_add[2].value"),
                  new ConstraintFAddInst("reads[i].read.index_add[2].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].read.index_add[2].value"),
                  new ConstraintFSubInst("reads[i].read.index_add[2].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("reads[i].read.index_add[2].addend"),
                new ConstraintDFGEdge("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend")}),
              new ConstraintAnd({
                new ConstraintSExtInst("reads[i].read.index_add[2].addend"),
                new ConstraintDFGEdge("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend")}),
              new ConstraintSame("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintDistinct("reads[i].read.index_add[2].addend", "reads[i].read.offset_add.value"),
                new ConstraintIntegerType("reads[i].read.input_index[2]")}),
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend"),
                new ConstraintIntegerType("reads[i].read.offset_add.value")})}),
            new ConstraintSame("reads[i].read.stride_mul[1].value", "reads[i].read.index_add[2].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value"),
                new ConstraintUnused("reads[i].read.stride_mul[1].multiplier")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintMulInst("reads[i].read.stride_mul[1].value"),
                  new ConstraintLShiftInst("reads[i].read.stride_mul[1].value"),
                  new ConstraintFMulInst("reads[i].read.stride_mul[1].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value")})})}),
              new ConstraintAnd({
                new ConstraintFDivInst("reads[i].read.stride_mul[1].value"),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("reads[i].read.stride_mul[1].multiplier")}),
              new ConstraintCFGDominateStrict("reads[i].read.stride_mul[1].multiplier", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("reads[i].read.stride_mul[1].multiplier")})}),
            new ConstraintSame("reads[i].read.index_add[1].value", "reads[i].read.stride_mul[1].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value"),
                new ConstraintUnused("reads[i].read.index_add[1].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].read.index_add[1].value"),
                  new ConstraintBitOrInst("reads[i].read.index_add[1].value"),
                  new ConstraintFAddInst("reads[i].read.index_add[1].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].read.index_add[1].value"),
                  new ConstraintFSubInst("reads[i].read.index_add[1].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("reads[i].read.index_add[1].addend"),
                new ConstraintDFGEdge("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend")}),
              new ConstraintAnd({
                new ConstraintSExtInst("reads[i].read.index_add[1].addend"),
                new ConstraintDFGEdge("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend")}),
              new ConstraintSame("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintDistinct("reads[i].read.index_add[1].addend", "reads[i].read.input_index[2]"),
                new ConstraintIntegerType("reads[i].read.input_index[1]")}),
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend"),
                new ConstraintIntegerType("reads[i].read.input_index[2]")})}),
            new ConstraintSame("reads[i].read.stride_mul[0].value", "reads[i].read.index_add[1].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value"),
                new ConstraintUnused("reads[i].read.stride_mul[0].multiplier")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintMulInst("reads[i].read.stride_mul[0].value"),
                  new ConstraintLShiftInst("reads[i].read.stride_mul[0].value"),
                  new ConstraintFMulInst("reads[i].read.stride_mul[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value")})})}),
              new ConstraintAnd({
                new ConstraintFDivInst("reads[i].read.stride_mul[0].value"),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value"),
                    new ConstraintSecondOperand("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("reads[i].read.stride_mul[0].multiplier")}),
              new ConstraintCFGDominateStrict("reads[i].read.stride_mul[0].multiplier", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("reads[i].read.stride_mul[0].multiplier")})}),
            new ConstraintSame("reads[i].read.index_add[0].value", "reads[i].read.stride_mul[0].input"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value"),
                new ConstraintUnused("reads[i].read.index_add[0].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].read.index_add[0].value"),
                  new ConstraintBitOrInst("reads[i].read.index_add[0].value"),
                  new ConstraintFAddInst("reads[i].read.index_add[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].read.index_add[0].value"),
                  new ConstraintFSubInst("reads[i].read.index_add[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value"),
                    new ConstraintSecondOperand("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value")})})})}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintZExtInst("reads[i].read.index_add[0].input"),
                new ConstraintDFGEdge("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input")}),
              new ConstraintAnd({
                new ConstraintSExtInst("reads[i].read.index_add[0].input"),
                new ConstraintDFGEdge("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input")}),
              new ConstraintSame("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input")}),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintDistinct("reads[i].read.index_add[0].input", "reads[i].read.input_index[1]"),
                new ConstraintIntegerType("reads[i].read.input_index[0]")}),
              new ConstraintAnd({
                new ConstraintSame("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input"),
                new ConstraintIntegerType("reads[i].read.input_index[1]")})}),
            new ConstraintSame("reads[i].read.base_index", "reads[i].read.index_add[0].addend"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("reads[i].read.base_index")}),
              new ConstraintCFGDominateStrict("reads[i].read.base_index", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("reads[i].read.base_index")})}),
            new ConstraintSame("reads[i].read.base_pointer", "reads[i].base_pointer"),
            new ConstraintSame("reads[i].read.access_pointer", "reads[i].access_pointer"),
            new ConstraintSame("reads[i].read.input_index[0]", "reads[i].index[0].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].index[0].input", "reads[i].index[0].value"),
                new ConstraintUnused("reads[i].index[0].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].index[0].value"),
                  new ConstraintBitOrInst("reads[i].index[0].value"),
                  new ConstraintFAddInst("reads[i].index[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[0].input", "reads[i].index[0].value"),
                    new ConstraintSecondOperand("reads[i].index[0].addend", "reads[i].index[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[0].addend", "reads[i].index[0].value"),
                    new ConstraintSecondOperand("reads[i].index[0].input", "reads[i].index[0].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].index[0].value"),
                  new ConstraintFSubInst("reads[i].index[0].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[0].input", "reads[i].index[0].value"),
                    new ConstraintSecondOperand("reads[i].index[0].addend", "reads[i].index[0].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[0].addend", "reads[i].index[0].value"),
                    new ConstraintSecondOperand("reads[i].index[0].input", "reads[i].index[0].value")})})})}),
            new ConstraintOr({
              new ConstraintConstant("reads[i].index[0].addend"),
              new ConstraintUnused("reads[i].index[0].addend")}),
            new ConstraintSame("reads[i].index[0].input", "reads[i].input_index[0]"),
            new ConstraintSame("reads[i].read.input_index[1]", "reads[i].index[1].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].index[1].input", "reads[i].index[1].value"),
                new ConstraintUnused("reads[i].index[1].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].index[1].value"),
                  new ConstraintBitOrInst("reads[i].index[1].value"),
                  new ConstraintFAddInst("reads[i].index[1].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[1].input", "reads[i].index[1].value"),
                    new ConstraintSecondOperand("reads[i].index[1].addend", "reads[i].index[1].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[1].addend", "reads[i].index[1].value"),
                    new ConstraintSecondOperand("reads[i].index[1].input", "reads[i].index[1].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].index[1].value"),
                  new ConstraintFSubInst("reads[i].index[1].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[1].input", "reads[i].index[1].value"),
                    new ConstraintSecondOperand("reads[i].index[1].addend", "reads[i].index[1].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[1].addend", "reads[i].index[1].value"),
                    new ConstraintSecondOperand("reads[i].index[1].input", "reads[i].index[1].value")})})})}),
            new ConstraintOr({
              new ConstraintConstant("reads[i].index[1].addend"),
              new ConstraintUnused("reads[i].index[1].addend")}),
            new ConstraintSame("reads[i].index[1].input", "reads[i].input_index[1]"),
            new ConstraintSame("reads[i].read.input_index[2]", "reads[i].index[2].value"),
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintSame("reads[i].index[2].input", "reads[i].index[2].value"),
                new ConstraintUnused("reads[i].index[2].addend")}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintAddInst("reads[i].index[2].value"),
                  new ConstraintBitOrInst("reads[i].index[2].value"),
                  new ConstraintFAddInst("reads[i].index[2].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[2].input", "reads[i].index[2].value"),
                    new ConstraintSecondOperand("reads[i].index[2].addend", "reads[i].index[2].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[2].addend", "reads[i].index[2].value"),
                    new ConstraintSecondOperand("reads[i].index[2].input", "reads[i].index[2].value")})})}),
              new ConstraintAnd({
                new ConstraintOr({
                  new ConstraintSubInst("reads[i].index[2].value"),
                  new ConstraintFSubInst("reads[i].index[2].value")}),
                new ConstraintOr({
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[2].input", "reads[i].index[2].value"),
                    new ConstraintSecondOperand("reads[i].index[2].addend", "reads[i].index[2].value")}),
                  new ConstraintAnd({
                    new ConstraintFirstOperand("reads[i].index[2].addend", "reads[i].index[2].value"),
                    new ConstraintSecondOperand("reads[i].index[2].input", "reads[i].index[2].value")})})})}),
            new ConstraintOr({
              new ConstraintConstant("reads[i].index[2].addend"),
              new ConstraintUnused("reads[i].index[2].addend")}),
            new ConstraintSame("reads[i].index[2].input", "reads[i].input_index[2]"),
            new ConstraintSame("reads[i].input_index[0]", "write.input_index[0]"),
            new ConstraintSame("reads[i].input_index[1]", "write.input_index[1]"),
            new ConstraintSame("reads[i].input_index[2]", "write.input_index[2]")})),
        new ConstraintLoadInst("reads[0].value"),
        new ConstraintSame("compute_expr.input[0]", "reads[0].value"),
        new ConstraintSame("compute_expr.input[1]", "reads[1].value"),
        new ConstraintSame("compute_expr.input[2]", "reads[2].value"),
        new ConstraintSame("compute_expr.input[3]", "reads[3].value"),
        new ConstraintSame("compute_expr.input[4]", "reads[4].value"),
        new ConstraintSame("compute_expr.input[5]", "reads[5].value"),
        new ConstraintSame("compute_expr.input[6]", "reads[6].value"),
        new ConstraintSame("compute_expr.input[7]", "reads[7].value"),
        new ConstraintSame("compute_expr.input[8]", "reads[8].value"),
        new ConstraintSame("compute_expr.input[9]", "reads[9].value"),
        new ConstraintSame("compute_expr.input[10]", "reads[10].value"),
        new ConstraintSame("compute_expr.input[11]", "reads[11].value"),
        new ConstraintSame("compute_expr.input[12]", "reads[12].value"),
        new ConstraintSame("compute_expr.input[13]", "reads[13].value"),
        new ConstraintSame("compute_expr.input[14]", "reads[14].value"),
        new ConstraintSame("compute_expr.input[15]", "reads[15].value"),
        new ConstraintSame("compute_expr.input[16]", "reads[16].value"),
        new ConstraintSame("compute_expr.input[17]", "reads[17].value"),
        new ConstraintSame("compute_expr.input[18]", "reads[18].value"),
        new ConstraintSame("compute_expr.input[19]", "reads[19].value"),
        new ConstraintSame("compute_expr.input[20]", "reads[20].value"),
        new ConstraintSame("compute_expr.input[21]", "reads[21].value"),
        new ConstraintSame("compute_expr.input[22]", "reads[22].value"),
        new ConstraintSame("compute_expr.input[23]", "reads[23].value"),
        new ConstraintSame("compute_expr.input[24]", "reads[24].value"),
        new ConstraintSame("compute_expr.input[25]", "reads[25].value"),
        new ConstraintSame("compute_expr.input[26]", "reads[26].value"),
        new ConstraintSame("compute_expr.input[27]", "reads[27].value"),
        new ConstraintSame("compute_expr.input[28]", "reads[28].value"),
        new ConstraintSame("compute_expr.input[29]", "reads[29].value"),
        new ConstraintSame("compute_expr.input[30]", "reads[30].value"),
        new ConstraintSame("compute_expr.input[31]", "reads[31].value"),
        new ConstraintCollect(4, "i",
          new ConstraintCFGEdge("compute_expr.origin[i]", "body_sese.begin")),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintOr({
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintPreexecution("compute_expr.constants[i].value")}),
              new ConstraintCFGDominateStrict("compute_expr.constants[i].value", "begin"),
              new ConstraintAnd({
                new ConstraintInstruction("begin"),
                new ConstraintUnused("compute_expr.constants[i].value")})}),
            new ConstraintNotNumericConstant("compute_expr.constants[i].value"),
            new ConstraintDFGEdge("compute_expr.constants[i].value", "compute_expr.constants[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "compute_expr.constants[i].use")})),
        new ConstraintCollect(64, "i",
          new ConstraintAnd({
            new ConstraintCFGDominate("begin", "compute_expr.restrictions[i].value"),
            new ConstraintCFGDominateStrict("compute_expr.restrictions[i].value", "body_sese.begin"),
            new ConstraintDFGEdge("compute_expr.restrictions[i].value", "compute_expr.restrictions[i].use"),
            new ConstraintCFGDominate("body_sese.begin", "compute_expr.restrictions[i].use")})),
        new ConstraintPDGDominate({
          std::vector<std::string>{
            "compute_expr.restrictions[0].value", "compute_expr.restrictions[1].value",
            "compute_expr.restrictions[2].value", "compute_expr.restrictions[3].value",
            "compute_expr.restrictions[4].value", "compute_expr.restrictions[5].value",
            "compute_expr.restrictions[6].value", "compute_expr.restrictions[7].value",
            "compute_expr.restrictions[8].value", "compute_expr.restrictions[9].value",
            "compute_expr.restrictions[10].value", "compute_expr.restrictions[11].value",
            "compute_expr.restrictions[12].value", "compute_expr.restrictions[13].value",
            "compute_expr.restrictions[14].value", "compute_expr.restrictions[15].value",
            "compute_expr.restrictions[16].value", "compute_expr.restrictions[17].value",
            "compute_expr.restrictions[18].value", "compute_expr.restrictions[19].value",
            "compute_expr.restrictions[20].value", "compute_expr.restrictions[21].value",
            "compute_expr.restrictions[22].value", "compute_expr.restrictions[23].value",
            "compute_expr.restrictions[24].value", "compute_expr.restrictions[25].value",
            "compute_expr.restrictions[26].value", "compute_expr.restrictions[27].value",
            "compute_expr.restrictions[28].value", "compute_expr.restrictions[29].value",
            "compute_expr.restrictions[30].value", "compute_expr.restrictions[31].value",
            "compute_expr.restrictions[32].value", "compute_expr.restrictions[33].value",
            "compute_expr.restrictions[34].value", "compute_expr.restrictions[35].value",
            "compute_expr.restrictions[36].value", "compute_expr.restrictions[37].value",
            "compute_expr.restrictions[38].value", "compute_expr.restrictions[39].value",
            "compute_expr.restrictions[40].value", "compute_expr.restrictions[41].value",
            "compute_expr.restrictions[42].value", "compute_expr.restrictions[43].value",
            "compute_expr.restrictions[44].value", "compute_expr.restrictions[45].value",
            "compute_expr.restrictions[46].value", "compute_expr.restrictions[47].value",
            "compute_expr.restrictions[48].value", "compute_expr.restrictions[49].value",
            "compute_expr.restrictions[50].value", "compute_expr.restrictions[51].value",
            "compute_expr.restrictions[52].value", "compute_expr.restrictions[53].value",
            "compute_expr.restrictions[54].value", "compute_expr.restrictions[55].value",
            "compute_expr.restrictions[56].value", "compute_expr.restrictions[57].value",
            "compute_expr.restrictions[58].value", "compute_expr.restrictions[59].value",
            "compute_expr.restrictions[60].value", "compute_expr.restrictions[61].value",
            "compute_expr.restrictions[62].value", "compute_expr.restrictions[63].value"},
          std::vector<std::string>{
            "compute_expr.origin[0]", "compute_expr.origin[1]", "compute_expr.origin[2]",
            "compute_expr.origin[3]", "compute_expr.constants[0].value",
            "compute_expr.constants[1].value", "compute_expr.constants[2].value",
            "compute_expr.constants[3].value", "compute_expr.constants[4].value",
            "compute_expr.constants[5].value", "compute_expr.constants[6].value",
            "compute_expr.constants[7].value", "compute_expr.constants[8].value",
            "compute_expr.constants[9].value", "compute_expr.constants[10].value",
            "compute_expr.constants[11].value", "compute_expr.constants[12].value",
            "compute_expr.constants[13].value", "compute_expr.constants[14].value",
            "compute_expr.constants[15].value", "compute_expr.constants[16].value",
            "compute_expr.constants[17].value", "compute_expr.constants[18].value",
            "compute_expr.constants[19].value", "compute_expr.constants[20].value",
            "compute_expr.constants[21].value", "compute_expr.constants[22].value",
            "compute_expr.constants[23].value", "compute_expr.constants[24].value",
            "compute_expr.constants[25].value", "compute_expr.constants[26].value",
            "compute_expr.constants[27].value", "compute_expr.constants[28].value",
            "compute_expr.constants[29].value", "compute_expr.constants[30].value",
            "compute_expr.constants[31].value", "compute_expr.constants[32].value",
            "compute_expr.constants[33].value", "compute_expr.constants[34].value",
            "compute_expr.constants[35].value", "compute_expr.constants[36].value",
            "compute_expr.constants[37].value", "compute_expr.constants[38].value",
            "compute_expr.constants[39].value", "compute_expr.constants[40].value",
            "compute_expr.constants[41].value", "compute_expr.constants[42].value",
            "compute_expr.constants[43].value", "compute_expr.constants[44].value",
            "compute_expr.constants[45].value", "compute_expr.constants[46].value",
            "compute_expr.constants[47].value", "compute_expr.constants[48].value",
            "compute_expr.constants[49].value", "compute_expr.constants[50].value",
            "compute_expr.constants[51].value", "compute_expr.constants[52].value",
            "compute_expr.constants[53].value", "compute_expr.constants[54].value",
            "compute_expr.constants[55].value", "compute_expr.constants[56].value",
            "compute_expr.constants[57].value", "compute_expr.constants[58].value",
            "compute_expr.constants[59].value", "compute_expr.constants[60].value",
            "compute_expr.constants[61].value", "compute_expr.constants[62].value",
            "compute_expr.constants[63].value", "compute_expr.input[0]",
            "compute_expr.input[1]", "compute_expr.input[2]", "compute_expr.input[3]",
            "compute_expr.input[4]", "compute_expr.input[5]", "compute_expr.input[6]",
            "compute_expr.input[7]", "compute_expr.input[8]", "compute_expr.input[9]",
            "compute_expr.input[10]", "compute_expr.input[11]", "compute_expr.input[12]",
            "compute_expr.input[13]", "compute_expr.input[14]", "compute_expr.input[15]",
            "compute_expr.input[16]", "compute_expr.input[17]", "compute_expr.input[18]",
            "compute_expr.input[19]", "compute_expr.input[20]", "compute_expr.input[21]",
            "compute_expr.input[22]", "compute_expr.input[23]", "compute_expr.input[24]",
            "compute_expr.input[25]", "compute_expr.input[26]", "compute_expr.input[27]",
            "compute_expr.input[28]", "compute_expr.input[29]", "compute_expr.input[30]",
            "compute_expr.input[31]"},
          std::vector<std::string>{
            "compute_expr.output"}})});

    return Solution::Find(constraint, function, max_solutions);
}
