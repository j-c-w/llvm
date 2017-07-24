#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/ConstraintCollect.hpp"
#include "llvm/Constraints/ConstraintsBasic.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"
/*
ConstraintAnd ConstraintDistributive()
{
    return ConstraintAddInst("value")
        && ConstraintFirstArgument("sum1.value", "value")
        && ConstraintSecondArgument("sum2.value", "value")
        && ConstraintIntegerType("sum1.value")
        && ConstraintSame("sum1.value", "sum1.results[0]")
        && (  (  ConstraintAddInst("sum1.results[0]")
              && (  (  ConstraintFirstArgument("sum1.results[1]", "sum1.results[0]")
                    && ConstraintSecondArgument("sum1.factors[0]", "sum1.results[0]"))
                 || (  ConstraintFirstArgument("sum1.factors[0]", "sum1.results[0]")
                    && ConstraintSecondArgument("sum1.results[1]", "sum1.results[0]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[0]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[0]"))
                 || (  ConstraintUnused("sum1.results[0]")
                    && ConstraintUnused("sum1.factors[0]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[0]", "sum1.factors[0]")
              && ConstraintUnused("sum1.results[1]")))
        && (  (  ConstraintAddInst("sum1.results[1]")
              && (  (  ConstraintFirstArgument("sum1.results[2]", "sum1.results[1]")
                    && ConstraintSecondArgument("sum1.factors[1]", "sum1.results[1]"))
                 || (  ConstraintFirstArgument("sum1.factors[1]", "sum1.results[1]")
                    && ConstraintSecondArgument("sum1.results[2]", "sum1.results[1]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[1]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[1]"))
                 || (  ConstraintUnused("sum1.results[1]")
                    && ConstraintUnused("sum1.factors[1]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[1]", "sum1.factors[1]")
              && ConstraintUnused("sum1.results[2]")))
        && (  (  ConstraintAddInst("sum1.results[2]")
              && (  (  ConstraintFirstArgument("sum1.results[3]", "sum1.results[2]")
                    && ConstraintSecondArgument("sum1.factors[2]", "sum1.results[2]"))
                 || (  ConstraintFirstArgument("sum1.factors[2]", "sum1.results[2]")
                    && ConstraintSecondArgument("sum1.results[3]", "sum1.results[2]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[2]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[2]"))
                 || (  ConstraintUnused("sum1.results[2]")
                    && ConstraintUnused("sum1.factors[2]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[2]", "sum1.factors[2]")
              && ConstraintUnused("sum1.results[3]")))
        && (  (  ConstraintAddInst("sum1.results[3]")
              && (  (  ConstraintFirstArgument("sum1.results[4]", "sum1.results[3]")
                    && ConstraintSecondArgument("sum1.factors[3]", "sum1.results[3]"))
                 || (  ConstraintFirstArgument("sum1.factors[3]", "sum1.results[3]")
                    && ConstraintSecondArgument("sum1.results[4]", "sum1.results[3]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[3]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[3]"))
                 || (  ConstraintUnused("sum1.results[3]")
                    && ConstraintUnused("sum1.factors[3]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[3]", "sum1.factors[3]")
              && ConstraintUnused("sum1.results[4]")))
        && (  (  ConstraintAddInst("sum1.results[4]")
              && (  (  ConstraintFirstArgument("sum1.results[5]", "sum1.results[4]")
                    && ConstraintSecondArgument("sum1.factors[4]", "sum1.results[4]"))
                 || (  ConstraintFirstArgument("sum1.factors[4]", "sum1.results[4]")
                    && ConstraintSecondArgument("sum1.results[5]", "sum1.results[4]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[4]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[4]"))
                 || (  ConstraintUnused("sum1.results[4]")
                    && ConstraintUnused("sum1.factors[4]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[4]", "sum1.factors[4]")
              && ConstraintUnused("sum1.results[5]")))
        && (  (  ConstraintAddInst("sum1.results[5]")
              && (  (  ConstraintFirstArgument("sum1.results[6]", "sum1.results[5]")
                    && ConstraintSecondArgument("sum1.factors[5]", "sum1.results[5]"))
                 || (  ConstraintFirstArgument("sum1.factors[5]", "sum1.results[5]")
                    && ConstraintSecondArgument("sum1.results[6]", "sum1.results[5]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[5]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[5]"))
                 || (  ConstraintUnused("sum1.results[5]")
                    && ConstraintUnused("sum1.factors[5]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[5]", "sum1.factors[5]")
              && ConstraintUnused("sum1.results[6]")))
        && (  (  ConstraintAddInst("sum1.results[6]")
              && (  (  ConstraintFirstArgument("sum1.results[7]", "sum1.results[6]")
                    && ConstraintSecondArgument("sum1.factors[6]", "sum1.results[6]"))
                 || (  ConstraintFirstArgument("sum1.factors[6]", "sum1.results[6]")
                    && ConstraintSecondArgument("sum1.results[7]", "sum1.results[6]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[6]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[6]"))
                 || (  ConstraintUnused("sum1.results[6]")
                    && ConstraintUnused("sum1.factors[6]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[6]", "sum1.factors[6]")
              && ConstraintUnused("sum1.results[7]")))
        && (  (  ConstraintAddInst("sum1.results[7]")
              && (  (  ConstraintFirstArgument("sum1.results[8]", "sum1.results[7]")
                    && ConstraintSecondArgument("sum1.factors[7]", "sum1.results[7]"))
                 || (  ConstraintFirstArgument("sum1.factors[7]", "sum1.results[7]")
                    && ConstraintSecondArgument("sum1.results[8]", "sum1.results[7]")))
              && ConstraintIntegerType("sum1.last_factor"))
           || (  (  (  ConstraintIntegerType("sum1.results[7]")
                    && ConstraintSame("sum1.last_factor", "sum1.factors[7]"))
                 || (  ConstraintUnused("sum1.results[7]")
                    && ConstraintUnused("sum1.factors[7]")
                    && ConstraintIntegerType("sum1.last_factor")))
              && ConstraintSame("sum1.results[7]", "sum1.factors[7]")
              && ConstraintUnused("sum1.results[8]")))
        && ConstraintUnused("sum1.factors[8]")
        && ConstraintSame("product1.value", "sum1.last_factor")
        && ConstraintIntegerType("product1.value")
        && ConstraintSame("product1.value", "product1.results[0]")
        && (  (  ConstraintMulInst("product1.results[0]")
              && (  (  ConstraintFirstArgument("product1.results[1]", "product1.results[0]")
                    && ConstraintSecondArgument("product1.factors[0]", "product1.results[0]"))
                 || (  ConstraintFirstArgument("product1.factors[0]", "product1.results[0]")
                    && ConstraintSecondArgument("product1.results[1]", "product1.results[0]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[0]")
                    && ConstraintSame("product1.last_factor", "product1.factors[0]"))
                 || (  ConstraintUnused("product1.results[0]")
                    && ConstraintUnused("product1.factors[0]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[0]", "product1.factors[0]")
              && ConstraintUnused("product1.results[1]")))
        && (  (  ConstraintMulInst("product1.results[1]")
              && (  (  ConstraintFirstArgument("product1.results[2]", "product1.results[1]")
                    && ConstraintSecondArgument("product1.factors[1]", "product1.results[1]"))
                 || (  ConstraintFirstArgument("product1.factors[1]", "product1.results[1]")
                    && ConstraintSecondArgument("product1.results[2]", "product1.results[1]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[1]")
                    && ConstraintSame("product1.last_factor", "product1.factors[1]"))
                 || (  ConstraintUnused("product1.results[1]")
                    && ConstraintUnused("product1.factors[1]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[1]", "product1.factors[1]")
              && ConstraintUnused("product1.results[2]")))
        && (  (  ConstraintMulInst("product1.results[2]")
              && (  (  ConstraintFirstArgument("product1.results[3]", "product1.results[2]")
                    && ConstraintSecondArgument("product1.factors[2]", "product1.results[2]"))
                 || (  ConstraintFirstArgument("product1.factors[2]", "product1.results[2]")
                    && ConstraintSecondArgument("product1.results[3]", "product1.results[2]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[2]")
                    && ConstraintSame("product1.last_factor", "product1.factors[2]"))
                 || (  ConstraintUnused("product1.results[2]")
                    && ConstraintUnused("product1.factors[2]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[2]", "product1.factors[2]")
              && ConstraintUnused("product1.results[3]")))
        && (  (  ConstraintMulInst("product1.results[3]")
              && (  (  ConstraintFirstArgument("product1.results[4]", "product1.results[3]")
                    && ConstraintSecondArgument("product1.factors[3]", "product1.results[3]"))
                 || (  ConstraintFirstArgument("product1.factors[3]", "product1.results[3]")
                    && ConstraintSecondArgument("product1.results[4]", "product1.results[3]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[3]")
                    && ConstraintSame("product1.last_factor", "product1.factors[3]"))
                 || (  ConstraintUnused("product1.results[3]")
                    && ConstraintUnused("product1.factors[3]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[3]", "product1.factors[3]")
              && ConstraintUnused("product1.results[4]")))
        && (  (  ConstraintMulInst("product1.results[4]")
              && (  (  ConstraintFirstArgument("product1.results[5]", "product1.results[4]")
                    && ConstraintSecondArgument("product1.factors[4]", "product1.results[4]"))
                 || (  ConstraintFirstArgument("product1.factors[4]", "product1.results[4]")
                    && ConstraintSecondArgument("product1.results[5]", "product1.results[4]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[4]")
                    && ConstraintSame("product1.last_factor", "product1.factors[4]"))
                 || (  ConstraintUnused("product1.results[4]")
                    && ConstraintUnused("product1.factors[4]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[4]", "product1.factors[4]")
              && ConstraintUnused("product1.results[5]")))
        && (  (  ConstraintMulInst("product1.results[5]")
              && (  (  ConstraintFirstArgument("product1.results[6]", "product1.results[5]")
                    && ConstraintSecondArgument("product1.factors[5]", "product1.results[5]"))
                 || (  ConstraintFirstArgument("product1.factors[5]", "product1.results[5]")
                    && ConstraintSecondArgument("product1.results[6]", "product1.results[5]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[5]")
                    && ConstraintSame("product1.last_factor", "product1.factors[5]"))
                 || (  ConstraintUnused("product1.results[5]")
                    && ConstraintUnused("product1.factors[5]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[5]", "product1.factors[5]")
              && ConstraintUnused("product1.results[6]")))
        && (  (  ConstraintMulInst("product1.results[6]")
              && (  (  ConstraintFirstArgument("product1.results[7]", "product1.results[6]")
                    && ConstraintSecondArgument("product1.factors[6]", "product1.results[6]"))
                 || (  ConstraintFirstArgument("product1.factors[6]", "product1.results[6]")
                    && ConstraintSecondArgument("product1.results[7]", "product1.results[6]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[6]")
                    && ConstraintSame("product1.last_factor", "product1.factors[6]"))
                 || (  ConstraintUnused("product1.results[6]")
                    && ConstraintUnused("product1.factors[6]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[6]", "product1.factors[6]")
              && ConstraintUnused("product1.results[7]")))
        && (  (  ConstraintMulInst("product1.results[7]")
              && (  (  ConstraintFirstArgument("product1.results[8]", "product1.results[7]")
                    && ConstraintSecondArgument("product1.factors[7]", "product1.results[7]"))
                 || (  ConstraintFirstArgument("product1.factors[7]", "product1.results[7]")
                    && ConstraintSecondArgument("product1.results[8]", "product1.results[7]")))
              && ConstraintIntegerType("product1.last_factor"))
           || (  (  (  ConstraintIntegerType("product1.results[7]")
                    && ConstraintSame("product1.last_factor", "product1.factors[7]"))
                 || (  ConstraintUnused("product1.results[7]")
                    && ConstraintUnused("product1.factors[7]")
                    && ConstraintIntegerType("product1.last_factor")))
              && ConstraintSame("product1.results[7]", "product1.factors[7]")
              && ConstraintUnused("product1.results[8]")))
        && ConstraintUnused("product1.factors[8]")
        && ConstraintSame("product1.last_factor", "product2.last_factor")
        && ConstraintIntegerType("sum2.value")
        && ConstraintSame("sum2.value", "sum2.results[0]")
        && (  (  ConstraintAddInst("sum2.results[0]")
              && (  (  ConstraintFirstArgument("sum2.results[1]", "sum2.results[0]")
                    && ConstraintSecondArgument("sum2.factors[0]", "sum2.results[0]"))
                 || (  ConstraintFirstArgument("sum2.factors[0]", "sum2.results[0]")
                    && ConstraintSecondArgument("sum2.results[1]", "sum2.results[0]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[0]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[0]"))
                 || (  ConstraintUnused("sum2.results[0]")
                    && ConstraintUnused("sum2.factors[0]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[0]", "sum2.factors[0]")
              && ConstraintUnused("sum2.results[1]")))
        && (  (  ConstraintAddInst("sum2.results[1]")
              && (  (  ConstraintFirstArgument("sum2.results[2]", "sum2.results[1]")
                    && ConstraintSecondArgument("sum2.factors[1]", "sum2.results[1]"))
                 || (  ConstraintFirstArgument("sum2.factors[1]", "sum2.results[1]")
                    && ConstraintSecondArgument("sum2.results[2]", "sum2.results[1]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[1]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[1]"))
                 || (  ConstraintUnused("sum2.results[1]")
                    && ConstraintUnused("sum2.factors[1]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[1]", "sum2.factors[1]")
              && ConstraintUnused("sum2.results[2]")))
        && (  (  ConstraintAddInst("sum2.results[2]")
              && (  (  ConstraintFirstArgument("sum2.results[3]", "sum2.results[2]")
                    && ConstraintSecondArgument("sum2.factors[2]", "sum2.results[2]"))
                 || (  ConstraintFirstArgument("sum2.factors[2]", "sum2.results[2]")
                    && ConstraintSecondArgument("sum2.results[3]", "sum2.results[2]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[2]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[2]"))
                 || (  ConstraintUnused("sum2.results[2]")
                    && ConstraintUnused("sum2.factors[2]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[2]", "sum2.factors[2]")
              && ConstraintUnused("sum2.results[3]")))
        && (  (  ConstraintAddInst("sum2.results[3]")
              && (  (  ConstraintFirstArgument("sum2.results[4]", "sum2.results[3]")
                    && ConstraintSecondArgument("sum2.factors[3]", "sum2.results[3]"))
                 || (  ConstraintFirstArgument("sum2.factors[3]", "sum2.results[3]")
                    && ConstraintSecondArgument("sum2.results[4]", "sum2.results[3]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[3]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[3]"))
                 || (  ConstraintUnused("sum2.results[3]")
                    && ConstraintUnused("sum2.factors[3]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[3]", "sum2.factors[3]")
              && ConstraintUnused("sum2.results[4]")))
        && (  (  ConstraintAddInst("sum2.results[4]")
              && (  (  ConstraintFirstArgument("sum2.results[5]", "sum2.results[4]")
                    && ConstraintSecondArgument("sum2.factors[4]", "sum2.results[4]"))
                 || (  ConstraintFirstArgument("sum2.factors[4]", "sum2.results[4]")
                    && ConstraintSecondArgument("sum2.results[5]", "sum2.results[4]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[4]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[4]"))
                 || (  ConstraintUnused("sum2.results[4]")
                    && ConstraintUnused("sum2.factors[4]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[4]", "sum2.factors[4]")
              && ConstraintUnused("sum2.results[5]")))
        && (  (  ConstraintAddInst("sum2.results[5]")
              && (  (  ConstraintFirstArgument("sum2.results[6]", "sum2.results[5]")
                    && ConstraintSecondArgument("sum2.factors[5]", "sum2.results[5]"))
                 || (  ConstraintFirstArgument("sum2.factors[5]", "sum2.results[5]")
                    && ConstraintSecondArgument("sum2.results[6]", "sum2.results[5]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[5]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[5]"))
                 || (  ConstraintUnused("sum2.results[5]")
                    && ConstraintUnused("sum2.factors[5]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[5]", "sum2.factors[5]")
              && ConstraintUnused("sum2.results[6]")))
        && (  (  ConstraintAddInst("sum2.results[6]")
              && (  (  ConstraintFirstArgument("sum2.results[7]", "sum2.results[6]")
                    && ConstraintSecondArgument("sum2.factors[6]", "sum2.results[6]"))
                 || (  ConstraintFirstArgument("sum2.factors[6]", "sum2.results[6]")
                    && ConstraintSecondArgument("sum2.results[7]", "sum2.results[6]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[6]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[6]"))
                 || (  ConstraintUnused("sum2.results[6]")
                    && ConstraintUnused("sum2.factors[6]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[6]", "sum2.factors[6]")
              && ConstraintUnused("sum2.results[7]")))
        && (  (  ConstraintAddInst("sum2.results[7]")
              && (  (  ConstraintFirstArgument("sum2.results[8]", "sum2.results[7]")
                    && ConstraintSecondArgument("sum2.factors[7]", "sum2.results[7]"))
                 || (  ConstraintFirstArgument("sum2.factors[7]", "sum2.results[7]")
                    && ConstraintSecondArgument("sum2.results[8]", "sum2.results[7]")))
              && ConstraintIntegerType("sum2.last_factor"))
           || (  (  (  ConstraintIntegerType("sum2.results[7]")
                    && ConstraintSame("sum2.last_factor", "sum2.factors[7]"))
                 || (  ConstraintUnused("sum2.results[7]")
                    && ConstraintUnused("sum2.factors[7]")
                    && ConstraintIntegerType("sum2.last_factor")))
              && ConstraintSame("sum2.results[7]", "sum2.factors[7]")
              && ConstraintUnused("sum2.results[8]")))
        && ConstraintUnused("sum2.factors[8]")
        && ConstraintSame("product2.value", "sum2.last_factor")
        && ConstraintIntegerType("product2.value")
        && ConstraintSame("product2.value", "product2.results[0]")
        && (  (  ConstraintMulInst("product2.results[0]")
              && (  (  ConstraintFirstArgument("product2.results[1]", "product2.results[0]")
                    && ConstraintSecondArgument("product2.factors[0]", "product2.results[0]"))
                 || (  ConstraintFirstArgument("product2.factors[0]", "product2.results[0]")
                    && ConstraintSecondArgument("product2.results[1]", "product2.results[0]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[0]")
                    && ConstraintSame("product2.last_factor", "product2.factors[0]"))
                 || (  ConstraintUnused("product2.results[0]")
                    && ConstraintUnused("product2.factors[0]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[0]", "product2.factors[0]")
              && ConstraintUnused("product2.results[1]")))
        && (  (  ConstraintMulInst("product2.results[1]")
              && (  (  ConstraintFirstArgument("product2.results[2]", "product2.results[1]")
                    && ConstraintSecondArgument("product2.factors[1]", "product2.results[1]"))
                 || (  ConstraintFirstArgument("product2.factors[1]", "product2.results[1]")
                    && ConstraintSecondArgument("product2.results[2]", "product2.results[1]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[1]")
                    && ConstraintSame("product2.last_factor", "product2.factors[1]"))
                 || (  ConstraintUnused("product2.results[1]")
                    && ConstraintUnused("product2.factors[1]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[1]", "product2.factors[1]")
              && ConstraintUnused("product2.results[2]")))
        && (  (  ConstraintMulInst("product2.results[2]")
              && (  (  ConstraintFirstArgument("product2.results[3]", "product2.results[2]")
                    && ConstraintSecondArgument("product2.factors[2]", "product2.results[2]"))
                 || (  ConstraintFirstArgument("product2.factors[2]", "product2.results[2]")
                    && ConstraintSecondArgument("product2.results[3]", "product2.results[2]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[2]")
                    && ConstraintSame("product2.last_factor", "product2.factors[2]"))
                 || (  ConstraintUnused("product2.results[2]")
                    && ConstraintUnused("product2.factors[2]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[2]", "product2.factors[2]")
              && ConstraintUnused("product2.results[3]")))
        && (  (  ConstraintMulInst("product2.results[3]")
              && (  (  ConstraintFirstArgument("product2.results[4]", "product2.results[3]")
                    && ConstraintSecondArgument("product2.factors[3]", "product2.results[3]"))
                 || (  ConstraintFirstArgument("product2.factors[3]", "product2.results[3]")
                    && ConstraintSecondArgument("product2.results[4]", "product2.results[3]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[3]")
                    && ConstraintSame("product2.last_factor", "product2.factors[3]"))
                 || (  ConstraintUnused("product2.results[3]")
                    && ConstraintUnused("product2.factors[3]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[3]", "product2.factors[3]")
              && ConstraintUnused("product2.results[4]")))
        && (  (  ConstraintMulInst("product2.results[4]")
              && (  (  ConstraintFirstArgument("product2.results[5]", "product2.results[4]")
                    && ConstraintSecondArgument("product2.factors[4]", "product2.results[4]"))
                 || (  ConstraintFirstArgument("product2.factors[4]", "product2.results[4]")
                    && ConstraintSecondArgument("product2.results[5]", "product2.results[4]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[4]")
                    && ConstraintSame("product2.last_factor", "product2.factors[4]"))
                 || (  ConstraintUnused("product2.results[4]")
                    && ConstraintUnused("product2.factors[4]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[4]", "product2.factors[4]")
              && ConstraintUnused("product2.results[5]")))
        && (  (  ConstraintMulInst("product2.results[5]")
              && (  (  ConstraintFirstArgument("product2.results[6]", "product2.results[5]")
                    && ConstraintSecondArgument("product2.factors[5]", "product2.results[5]"))
                 || (  ConstraintFirstArgument("product2.factors[5]", "product2.results[5]")
                    && ConstraintSecondArgument("product2.results[6]", "product2.results[5]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[5]")
                    && ConstraintSame("product2.last_factor", "product2.factors[5]"))
                 || (  ConstraintUnused("product2.results[5]")
                    && ConstraintUnused("product2.factors[5]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[5]", "product2.factors[5]")
              && ConstraintUnused("product2.results[6]")))
        && (  (  ConstraintMulInst("product2.results[6]")
              && (  (  ConstraintFirstArgument("product2.results[7]", "product2.results[6]")
                    && ConstraintSecondArgument("product2.factors[6]", "product2.results[6]"))
                 || (  ConstraintFirstArgument("product2.factors[6]", "product2.results[6]")
                    && ConstraintSecondArgument("product2.results[7]", "product2.results[6]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[6]")
                    && ConstraintSame("product2.last_factor", "product2.factors[6]"))
                 || (  ConstraintUnused("product2.results[6]")
                    && ConstraintUnused("product2.factors[6]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[6]", "product2.factors[6]")
              && ConstraintUnused("product2.results[7]")))
        && (  (  ConstraintMulInst("product2.results[7]")
              && (  (  ConstraintFirstArgument("product2.results[8]", "product2.results[7]")
                    && ConstraintSecondArgument("product2.factors[7]", "product2.results[7]"))
                 || (  ConstraintFirstArgument("product2.factors[7]", "product2.results[7]")
                    && ConstraintSecondArgument("product2.results[8]", "product2.results[7]")))
              && ConstraintIntegerType("product2.last_factor"))
           || (  (  (  ConstraintIntegerType("product2.results[7]")
                    && ConstraintSame("product2.last_factor", "product2.factors[7]"))
                 || (  ConstraintUnused("product2.results[7]")
                    && ConstraintUnused("product2.factors[7]")
                    && ConstraintIntegerType("product2.last_factor")))
              && ConstraintSame("product2.results[7]", "product2.factors[7]")
              && ConstraintUnused("product2.results[8]")))
        && ConstraintUnused("product2.factors[8]");
}

ConstraintAnd ConstraintHoistSelect()
{
    return ConstraintSelectInst("select")
        && ConstraintSecondArgument("input1", "select")
        && ConstraintFirstArgument("base", "input1")
        && ConstraintThirdArgument("input2", "select")
        && ConstraintFirstArgument("base", "input2")
        && ConstraintGEPInst("input1")
        && ConstraintGEPInst("input2");
}

ConstraintAnd ConstraintScalarReduction()
{
    return ConstraintBranchInst("precursor")
        && ConstraintCFGEdge("precursor", "begin")
        && ConstraintBranchInst("end")
        && ConstraintCFGEdge("end", "successor")
        && ConstraintCFGDominate("begin", "end")
        && ConstraintCFGPostdom("end", "begin")
        && ConstraintCFGDominateStrict("precursor", "begin")
        && ConstraintCFGPostdomStrict("successor", "end")
        && ConstraintCFGBlocked("begin", "end", "precursor")
        && ConstraintCFGBlocked("successor", "begin", "end")
        && ConstraintSame("body_sese.precursor", "end")
        && ConstraintSame("body_sese.successor", "begin")
        && ConstraintBranchInst("body_sese.precursor")
        && ConstraintCFGEdge("body_sese.precursor", "body_sese.begin")
        && ConstraintBranchInst("body_sese.end")
        && ConstraintCFGEdge("body_sese.end", "body_sese.successor")
        && ConstraintCFGDominate("body_sese.begin", "body_sese.end")
        && ConstraintCFGPostdom("body_sese.end", "body_sese.begin")
        && ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin")
        && ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end")
        && ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor")
        && ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end")
        && ConstraintDistinct("body_sese.begin", "successor")
        && ConstraintIncomingValue("increment", "body_sese.end", "iterator")
        && ConstraintDFGEdge("iterator", "comparison")
        && ConstraintICmpInst("comparison")
        && ConstraintDFGEdge("comparison", "end")
        && ConstraintDFGEdge("increment", "iterator")
        && ConstraintAddInst("increment")
        && ConstraintDFGEdge("iterator", "increment")
        && ConstraintDFGEdge("iter_end", "comparison")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_end"))
           || ConstraintCFGDominateStrict("iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_end")))
        && ConstraintDFGEdge("iter_begin", "iterator")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_begin"))
           || ConstraintCFGDominateStrict("iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_begin")))
        && ConstraintDFGEdge("iter_step", "increment")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_step"))
           || ConstraintCFGDominateStrict("iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_step")))
        && std::make_pair("i",32) * (  ConstraintLoadInst("read[i].value")
                                    && ConstraintFirstArgument("read[i].access_pointer", "read[i].value")
                                    && ConstraintGEPInst("read[i].access_pointer")
                                    && ConstraintFirstArgument("read[i].base_pointer", "read[i].access_pointer")
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].base_pointer"))
                                       || ConstraintCFGDominateStrict("read[i].base_pointer", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].base_pointer")))
                                    && ConstraintSecondArgument("read[i].output_index", "read[i].access_pointer")
                                    && (  (  ConstraintZExtInst("read[i].output_index")
                                          && ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index"))
                                       || (  ConstraintSExtInst("read[i].output_index")
                                          && ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index"))
                                       || ConstraintSame("read[i].offset_add.value", "read[i].output_index"))
                                    && (  (  ConstraintSame("read[i].offset_add.input", "read[i].offset_add.value")
                                          && ConstraintUnused("read[i].offset_add.addend"))
                                       || (  (  ConstraintAddInst("read[i].offset_add.value")
                                             || ConstraintBitOrInst("read[i].offset_add.value")
                                             || ConstraintFAddInst("read[i].offset_add.value"))
                                          && (  (  ConstraintFirstArgument("read[i].offset_add.input", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.addend", "read[i].offset_add.value"))
                                             || (  ConstraintFirstArgument("read[i].offset_add.addend", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.input", "read[i].offset_add.value"))))
                                       || (  (  ConstraintSubInst("read[i].offset_add.value")
                                             || ConstraintFSubInst("read[i].offset_add.value"))
                                          && (  (  ConstraintFirstArgument("read[i].offset_add.input", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.addend", "read[i].offset_add.value"))
                                             || (  ConstraintFirstArgument("read[i].offset_add.addend", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.input", "read[i].offset_add.value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].offset_add.addend"))
                                       || ConstraintCFGDominateStrict("read[i].offset_add.addend", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].offset_add.addend")))
                                    && ConstraintSame("read[i].stride_mul[0].value", "read[i].offset_add.input")
                                    && (  (  ConstraintSame("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")
                                          && ConstraintUnused("read[i].stride_mul[0].multiplier"))
                                       || (  (  ConstraintMulInst("read[i].stride_mul[0].value")
                                             || ConstraintLShiftInst("read[i].stride_mul[0].value")
                                             || ConstraintFMulInst("read[i].stride_mul[0].value"))
                                          && (  (  ConstraintFirstArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"))
                                             || (  ConstraintFirstArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"))))
                                       || (  ConstraintFDivInst("read[i].stride_mul[0].value")
                                          && (  (  ConstraintFirstArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"))
                                             || (  ConstraintFirstArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].stride_mul[0].multiplier"))
                                       || ConstraintCFGDominateStrict("read[i].stride_mul[0].multiplier", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].stride_mul[0].multiplier")))
                                    && ConstraintSame("read[i].index_add[0].value", "read[i].stride_mul[0].input")
                                    && (  (  ConstraintSame("read[i].index_add[0].input", "read[i].index_add[0].value")
                                          && ConstraintUnused("read[i].index_add[0].addend"))
                                       || (  (  ConstraintAddInst("read[i].index_add[0].value")
                                             || ConstraintBitOrInst("read[i].index_add[0].value")
                                             || ConstraintFAddInst("read[i].index_add[0].value"))
                                          && (  (  ConstraintFirstArgument("read[i].index_add[0].input", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].addend", "read[i].index_add[0].value"))
                                             || (  ConstraintFirstArgument("read[i].index_add[0].addend", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].input", "read[i].index_add[0].value"))))
                                       || (  (  ConstraintSubInst("read[i].index_add[0].value")
                                             || ConstraintFSubInst("read[i].index_add[0].value"))
                                          && (  (  ConstraintFirstArgument("read[i].index_add[0].input", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].addend", "read[i].index_add[0].value"))
                                             || (  ConstraintFirstArgument("read[i].index_add[0].addend", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].input", "read[i].index_add[0].value")))))
                                    && (  (  ConstraintZExtInst("read[i].index_add[0].input")
                                          && ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input"))
                                       || (  ConstraintSExtInst("read[i].index_add[0].input")
                                          && ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input"))
                                       || ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input"))
                                    && (  (  ConstraintDistinct("read[i].index_add[0].input", "read[i].offset_add.value")
                                          && ConstraintIntegerType("read[i].input_index[0]"))
                                       || (  ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input")
                                          && ConstraintIntegerType("read[i].offset_add.value")))
                                    && ConstraintSame("read[i].base_index", "read[i].index_add[0].addend")
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].base_index"))
                                       || ConstraintCFGDominateStrict("read[i].base_index", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].base_index")))
                                    && ConstraintSame("iterator", "read[i].input_index[0]")
                                    && ConstraintCFGDominate("body_sese.begin", "read[i].value")
                                    && ConstraintCFGPostdom("body_sese.end", "read[i].value"))
        && ConstraintLoadInst("read[0].value")
        && ConstraintSame("update_expr.input[0]", "old_value")
        && ConstraintSame("update_expr.input[1]", "read[0].value")
        && ConstraintSame("update_expr.input[2]", "read[1].value")
        && ConstraintSame("update_expr.input[3]", "read[2].value")
        && ConstraintSame("update_expr.input[4]", "read[3].value")
        && ConstraintSame("update_expr.input[5]", "read[4].value")
        && ConstraintSame("update_expr.input[6]", "read[5].value")
        && ConstraintSame("update_expr.input[7]", "read[6].value")
        && ConstraintSame("update_expr.input[8]", "read[7].value")
        && ConstraintSame("update_expr.input[9]", "read[8].value")
        && ConstraintSame("update_expr.input[10]", "read[9].value")
        && ConstraintSame("update_expr.input[11]", "read[10].value")
        && ConstraintSame("update_expr.input[12]", "read[11].value")
        && ConstraintSame("update_expr.input[13]", "read[12].value")
        && ConstraintSame("update_expr.input[14]", "read[13].value")
        && ConstraintSame("update_expr.input[15]", "read[14].value")
        && ConstraintSame("update_expr.input[16]", "read[15].value")
        && ConstraintSame("update_expr.input[17]", "read[16].value")
        && ConstraintSame("update_expr.input[18]", "read[17].value")
        && ConstraintSame("update_expr.input[19]", "read[18].value")
        && ConstraintSame("update_expr.input[20]", "read[19].value")
        && ConstraintSame("update_expr.input[21]", "read[20].value")
        && ConstraintSame("update_expr.input[22]", "read[21].value")
        && ConstraintSame("update_expr.input[23]", "read[22].value")
        && ConstraintSame("update_expr.input[24]", "read[23].value")
        && ConstraintSame("update_expr.input[25]", "read[24].value")
        && ConstraintSame("update_expr.input[26]", "read[25].value")
        && ConstraintSame("update_expr.input[27]", "read[26].value")
        && ConstraintSame("update_expr.input[28]", "read[27].value")
        && ConstraintSame("update_expr.input[29]", "read[28].value")
        && ConstraintSame("update_expr.input[30]", "read[29].value")
        && ConstraintSame("update_expr.input[31]", "read[30].value")
        && ConstraintIncomingValue("update_expr.output", "body_sese.end", "old_value")
        && ConstraintDistinct("old_value", "iterator")
        && std::make_pair("i",4) * ConstraintCFGEdge("update_expr.origin[i]", "body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("update_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("update_expr.constants[i].value", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("update_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("update_expr.constants[i].value")
                                    && ConstraintDFGEdge("update_expr.constants[i].value", "update_expr.constants[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "update_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("begin", "update_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("update_expr.restrictions[i].value", "body_sese.begin")
                                    && ConstraintDFGEdge("update_expr.restrictions[i].value", "update_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "update_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"update_expr.restrictions[0].value", "update_expr.restrictions[1].value",
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
                                  std::vector<std::string>{"update_expr.origin[0]", "update_expr.origin[1]", "update_expr.origin[2]",
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
                                  std::vector<std::string>{"update_expr.output"}})
        && std::make_pair("i",4) * (  ConstraintDFGEdge("incoming[i]", "old_value")
                                   && ConstraintDistinct("incoming[i]", "update_expr.output"));
}

ConstraintAnd ConstraintMatrixmatrix()
{
    return ConstraintBranchInst("loop[0].precursor")
        && ConstraintCFGEdge("loop[0].precursor", "loop[0].begin")
        && ConstraintBranchInst("loop[0].end")
        && ConstraintCFGEdge("loop[0].end", "loop[0].successor")
        && ConstraintCFGDominate("loop[0].begin", "loop[0].end")
        && ConstraintCFGPostdom("loop[0].end", "loop[0].begin")
        && ConstraintCFGDominateStrict("loop[0].precursor", "loop[0].begin")
        && ConstraintCFGPostdomStrict("loop[0].successor", "loop[0].end")
        && ConstraintCFGBlocked("loop[0].begin", "loop[0].end", "loop[0].precursor")
        && ConstraintCFGBlocked("loop[0].successor", "loop[0].begin", "loop[0].end")
        && ConstraintSame("loop[0].body_sese.precursor", "loop[0].end")
        && ConstraintSame("loop[0].body_sese.successor", "loop[0].begin")
        && ConstraintBranchInst("loop[0].body_sese.precursor")
        && ConstraintCFGEdge("loop[0].body_sese.precursor", "loop[0].body_sese.begin")
        && ConstraintBranchInst("loop[0].body_sese.end")
        && ConstraintCFGEdge("loop[0].body_sese.end", "loop[0].body_sese.successor")
        && ConstraintCFGDominate("loop[0].body_sese.begin", "loop[0].body_sese.end")
        && ConstraintCFGPostdom("loop[0].body_sese.end", "loop[0].body_sese.begin")
        && ConstraintCFGDominateStrict("loop[0].body_sese.precursor", "loop[0].body_sese.begin")
        && ConstraintCFGPostdomStrict("loop[0].body_sese.successor", "loop[0].body_sese.end")
        && ConstraintCFGBlocked("loop[0].body_sese.begin", "loop[0].body_sese.end", "loop[0].body_sese.precursor")
        && ConstraintCFGBlocked("loop[0].body_sese.successor", "loop[0].body_sese.begin", "loop[0].body_sese.end")
        && ConstraintDistinct("loop[0].body_sese.begin", "loop[0].successor")
        && ConstraintIncomingValue("loop[0].increment", "loop[0].body_sese.end", "loop[0].iterator")
        && ConstraintDFGEdge("loop[0].iterator", "loop[0].comparison")
        && ConstraintICmpInst("loop[0].comparison")
        && ConstraintDFGEdge("loop[0].comparison", "loop[0].end")
        && ConstraintDFGEdge("loop[0].increment", "loop[0].iterator")
        && ConstraintAddInst("loop[0].increment")
        && ConstraintDFGEdge("loop[0].iterator", "loop[0].increment")
        && ConstraintDFGEdge("loop[0].iter_end", "loop[0].comparison")
        && (  (  ConstraintInstruction("loop[0].begin")
              && ConstraintPreexecution("loop[0].iter_end"))
           || ConstraintCFGDominateStrict("loop[0].iter_end", "loop[0].begin")
           || (  ConstraintInstruction("loop[0].begin")
              && ConstraintUnused("loop[0].iter_end")))
        && ConstraintDFGEdge("loop[0].iter_begin", "loop[0].iterator")
        && (  (  ConstraintInstruction("loop[0].begin")
              && ConstraintPreexecution("loop[0].iter_begin"))
           || ConstraintCFGDominateStrict("loop[0].iter_begin", "loop[0].begin")
           || (  ConstraintInstruction("loop[0].begin")
              && ConstraintUnused("loop[0].iter_begin")))
        && ConstraintDFGEdge("loop[0].iter_step", "loop[0].increment")
        && (  (  ConstraintInstruction("loop[0].begin")
              && ConstraintPreexecution("loop[0].iter_step"))
           || ConstraintCFGDominateStrict("loop[0].iter_step", "loop[0].begin")
           || (  ConstraintInstruction("loop[0].begin")
              && ConstraintUnused("loop[0].iter_step")))
        && ConstraintSame("precursor", "loop[0].precursor")
        && ConstraintSame("begin", "loop[0].begin")
        && ConstraintSame("end", "loop[0].end")
        && ConstraintSame("successor", "loop[0].successor")
        && ConstraintBranchInst("loop[1].precursor")
        && ConstraintCFGEdge("loop[1].precursor", "loop[1].begin")
        && ConstraintBranchInst("loop[1].end")
        && ConstraintCFGEdge("loop[1].end", "loop[1].successor")
        && ConstraintCFGDominate("loop[1].begin", "loop[1].end")
        && ConstraintCFGPostdom("loop[1].end", "loop[1].begin")
        && ConstraintCFGDominateStrict("loop[1].precursor", "loop[1].begin")
        && ConstraintCFGPostdomStrict("loop[1].successor", "loop[1].end")
        && ConstraintCFGBlocked("loop[1].begin", "loop[1].end", "loop[1].precursor")
        && ConstraintCFGBlocked("loop[1].successor", "loop[1].begin", "loop[1].end")
        && ConstraintSame("loop[1].body_sese.precursor", "loop[1].end")
        && ConstraintSame("loop[1].body_sese.successor", "loop[1].begin")
        && ConstraintBranchInst("loop[1].body_sese.precursor")
        && ConstraintCFGEdge("loop[1].body_sese.precursor", "loop[1].body_sese.begin")
        && ConstraintBranchInst("loop[1].body_sese.end")
        && ConstraintCFGEdge("loop[1].body_sese.end", "loop[1].body_sese.successor")
        && ConstraintCFGDominate("loop[1].body_sese.begin", "loop[1].body_sese.end")
        && ConstraintCFGPostdom("loop[1].body_sese.end", "loop[1].body_sese.begin")
        && ConstraintCFGDominateStrict("loop[1].body_sese.precursor", "loop[1].body_sese.begin")
        && ConstraintCFGPostdomStrict("loop[1].body_sese.successor", "loop[1].body_sese.end")
        && ConstraintCFGBlocked("loop[1].body_sese.begin", "loop[1].body_sese.end", "loop[1].body_sese.precursor")
        && ConstraintCFGBlocked("loop[1].body_sese.successor", "loop[1].body_sese.begin", "loop[1].body_sese.end")
        && ConstraintDistinct("loop[1].body_sese.begin", "loop[1].successor")
        && ConstraintIncomingValue("loop[1].increment", "loop[1].body_sese.end", "loop[1].iterator")
        && ConstraintDFGEdge("loop[1].iterator", "loop[1].comparison")
        && ConstraintICmpInst("loop[1].comparison")
        && ConstraintDFGEdge("loop[1].comparison", "loop[1].end")
        && ConstraintDFGEdge("loop[1].increment", "loop[1].iterator")
        && ConstraintAddInst("loop[1].increment")
        && ConstraintDFGEdge("loop[1].iterator", "loop[1].increment")
        && ConstraintDFGEdge("loop[1].iter_end", "loop[1].comparison")
        && (  (  ConstraintInstruction("loop[1].begin")
              && ConstraintPreexecution("loop[1].iter_end"))
           || ConstraintCFGDominateStrict("loop[1].iter_end", "loop[1].begin")
           || (  ConstraintInstruction("loop[1].begin")
              && ConstraintUnused("loop[1].iter_end")))
        && ConstraintDFGEdge("loop[1].iter_begin", "loop[1].iterator")
        && (  (  ConstraintInstruction("loop[1].begin")
              && ConstraintPreexecution("loop[1].iter_begin"))
           || ConstraintCFGDominateStrict("loop[1].iter_begin", "loop[1].begin")
           || (  ConstraintInstruction("loop[1].begin")
              && ConstraintUnused("loop[1].iter_begin")))
        && ConstraintDFGEdge("loop[1].iter_step", "loop[1].increment")
        && (  (  ConstraintInstruction("loop[1].begin")
              && ConstraintPreexecution("loop[1].iter_step"))
           || ConstraintCFGDominateStrict("loop[1].iter_step", "loop[1].begin")
           || (  ConstraintInstruction("loop[1].begin")
              && ConstraintUnused("loop[1].iter_step")))
        && ConstraintSame("loop[1].pre_sese.precursor", "loop[0].body_sese.precursor")
        && ConstraintSame("loop[1].pre_sese.begin", "loop[0].body_sese.begin")
        && ConstraintSame("loop[1].pre_sese.end", "loop[1].precursor")
        && ConstraintSame("loop[1].pre_sese.successor", "loop[1].begin")
        && (  (  ConstraintBranchInst("loop[1].pre_sese.precursor")
              && ConstraintCFGEdge("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin")
              && ConstraintBranchInst("loop[1].pre_sese.end")
              && ConstraintCFGEdge("loop[1].pre_sese.end", "loop[1].pre_sese.successor")
              && ConstraintCFGDominate("loop[1].pre_sese.begin", "loop[1].pre_sese.end")
              && ConstraintCFGPostdom("loop[1].pre_sese.end", "loop[1].pre_sese.begin")
              && ConstraintCFGDominateStrict("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin")
              && ConstraintCFGPostdomStrict("loop[1].pre_sese.successor", "loop[1].pre_sese.end")
              && ConstraintCFGBlocked("loop[1].pre_sese.begin", "loop[1].pre_sese.end", "loop[1].pre_sese.precursor")
              && ConstraintCFGBlocked("loop[1].pre_sese.successor", "loop[1].pre_sese.begin", "loop[1].pre_sese.end"))
           || (  ConstraintSame("loop[1].pre_sese.precursor", "loop[1].pre_sese.end")
              && ConstraintSame("loop[1].pre_sese.begin", "loop[1].pre_sese.successor")))
        && ConstraintSame("loop[1].post_sese.precursor", "loop[1].end")
        && ConstraintSame("loop[1].post_sese.begin", "loop[1].successor")
        && ConstraintSame("loop[1].post_sese.end", "loop[0].body_sese.end")
        && ConstraintSame("loop[1].post_sese.successor", "loop[0].body_sese.successor")
        && (  (  ConstraintBranchInst("loop[1].post_sese.precursor")
              && ConstraintCFGEdge("loop[1].post_sese.precursor", "loop[1].post_sese.begin")
              && ConstraintBranchInst("loop[1].post_sese.end")
              && ConstraintCFGEdge("loop[1].post_sese.end", "loop[1].post_sese.successor")
              && ConstraintCFGDominate("loop[1].post_sese.begin", "loop[1].post_sese.end")
              && ConstraintCFGPostdom("loop[1].post_sese.end", "loop[1].post_sese.begin")
              && ConstraintCFGDominateStrict("loop[1].post_sese.precursor", "loop[1].post_sese.begin")
              && ConstraintCFGPostdomStrict("loop[1].post_sese.successor", "loop[1].post_sese.end")
              && ConstraintCFGBlocked("loop[1].post_sese.begin", "loop[1].post_sese.end", "loop[1].post_sese.precursor")
              && ConstraintCFGBlocked("loop[1].post_sese.successor", "loop[1].post_sese.begin", "loop[1].post_sese.end"))
           || (  ConstraintSame("loop[1].post_sese.precursor", "loop[1].post_sese.end")
              && ConstraintSame("loop[1].post_sese.begin", "loop[1].post_sese.successor")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[1].iter_begin"))
           || ConstraintCFGDominateStrict("loop[1].iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[1].iter_begin")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[1].iter_step"))
           || ConstraintCFGDominateStrict("loop[1].iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[1].iter_step")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[1].iter_end"))
           || ConstraintCFGDominateStrict("loop[1].iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[1].iter_end")))
        && ConstraintBranchInst("loop[2].precursor")
        && ConstraintCFGEdge("loop[2].precursor", "loop[2].begin")
        && ConstraintBranchInst("loop[2].end")
        && ConstraintCFGEdge("loop[2].end", "loop[2].successor")
        && ConstraintCFGDominate("loop[2].begin", "loop[2].end")
        && ConstraintCFGPostdom("loop[2].end", "loop[2].begin")
        && ConstraintCFGDominateStrict("loop[2].precursor", "loop[2].begin")
        && ConstraintCFGPostdomStrict("loop[2].successor", "loop[2].end")
        && ConstraintCFGBlocked("loop[2].begin", "loop[2].end", "loop[2].precursor")
        && ConstraintCFGBlocked("loop[2].successor", "loop[2].begin", "loop[2].end")
        && ConstraintSame("loop[2].body_sese.precursor", "loop[2].end")
        && ConstraintSame("loop[2].body_sese.successor", "loop[2].begin")
        && ConstraintBranchInst("loop[2].body_sese.precursor")
        && ConstraintCFGEdge("loop[2].body_sese.precursor", "loop[2].body_sese.begin")
        && ConstraintBranchInst("loop[2].body_sese.end")
        && ConstraintCFGEdge("loop[2].body_sese.end", "loop[2].body_sese.successor")
        && ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].body_sese.end")
        && ConstraintCFGPostdom("loop[2].body_sese.end", "loop[2].body_sese.begin")
        && ConstraintCFGDominateStrict("loop[2].body_sese.precursor", "loop[2].body_sese.begin")
        && ConstraintCFGPostdomStrict("loop[2].body_sese.successor", "loop[2].body_sese.end")
        && ConstraintCFGBlocked("loop[2].body_sese.begin", "loop[2].body_sese.end", "loop[2].body_sese.precursor")
        && ConstraintCFGBlocked("loop[2].body_sese.successor", "loop[2].body_sese.begin", "loop[2].body_sese.end")
        && ConstraintDistinct("loop[2].body_sese.begin", "loop[2].successor")
        && ConstraintIncomingValue("loop[2].increment", "loop[2].body_sese.end", "loop[2].iterator")
        && ConstraintDFGEdge("loop[2].iterator", "loop[2].comparison")
        && ConstraintICmpInst("loop[2].comparison")
        && ConstraintDFGEdge("loop[2].comparison", "loop[2].end")
        && ConstraintDFGEdge("loop[2].increment", "loop[2].iterator")
        && ConstraintAddInst("loop[2].increment")
        && ConstraintDFGEdge("loop[2].iterator", "loop[2].increment")
        && ConstraintDFGEdge("loop[2].iter_end", "loop[2].comparison")
        && (  (  ConstraintInstruction("loop[2].begin")
              && ConstraintPreexecution("loop[2].iter_end"))
           || ConstraintCFGDominateStrict("loop[2].iter_end", "loop[2].begin")
           || (  ConstraintInstruction("loop[2].begin")
              && ConstraintUnused("loop[2].iter_end")))
        && ConstraintDFGEdge("loop[2].iter_begin", "loop[2].iterator")
        && (  (  ConstraintInstruction("loop[2].begin")
              && ConstraintPreexecution("loop[2].iter_begin"))
           || ConstraintCFGDominateStrict("loop[2].iter_begin", "loop[2].begin")
           || (  ConstraintInstruction("loop[2].begin")
              && ConstraintUnused("loop[2].iter_begin")))
        && ConstraintDFGEdge("loop[2].iter_step", "loop[2].increment")
        && (  (  ConstraintInstruction("loop[2].begin")
              && ConstraintPreexecution("loop[2].iter_step"))
           || ConstraintCFGDominateStrict("loop[2].iter_step", "loop[2].begin")
           || (  ConstraintInstruction("loop[2].begin")
              && ConstraintUnused("loop[2].iter_step")))
        && ConstraintSame("loop[2].pre_sese.precursor", "loop[1].body_sese.precursor")
        && ConstraintSame("loop[2].pre_sese.begin", "loop[1].body_sese.begin")
        && ConstraintSame("loop[2].pre_sese.end", "loop[2].precursor")
        && ConstraintSame("loop[2].pre_sese.successor", "loop[2].begin")
        && (  (  ConstraintBranchInst("loop[2].pre_sese.precursor")
              && ConstraintCFGEdge("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin")
              && ConstraintBranchInst("loop[2].pre_sese.end")
              && ConstraintCFGEdge("loop[2].pre_sese.end", "loop[2].pre_sese.successor")
              && ConstraintCFGDominate("loop[2].pre_sese.begin", "loop[2].pre_sese.end")
              && ConstraintCFGPostdom("loop[2].pre_sese.end", "loop[2].pre_sese.begin")
              && ConstraintCFGDominateStrict("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin")
              && ConstraintCFGPostdomStrict("loop[2].pre_sese.successor", "loop[2].pre_sese.end")
              && ConstraintCFGBlocked("loop[2].pre_sese.begin", "loop[2].pre_sese.end", "loop[2].pre_sese.precursor")
              && ConstraintCFGBlocked("loop[2].pre_sese.successor", "loop[2].pre_sese.begin", "loop[2].pre_sese.end"))
           || (  ConstraintSame("loop[2].pre_sese.precursor", "loop[2].pre_sese.end")
              && ConstraintSame("loop[2].pre_sese.begin", "loop[2].pre_sese.successor")))
        && ConstraintSame("loop[2].post_sese.precursor", "loop[2].end")
        && ConstraintSame("loop[2].post_sese.begin", "loop[2].successor")
        && ConstraintSame("loop[2].post_sese.end", "loop[1].body_sese.end")
        && ConstraintSame("loop[2].post_sese.successor", "loop[1].body_sese.successor")
        && (  (  ConstraintBranchInst("loop[2].post_sese.precursor")
              && ConstraintCFGEdge("loop[2].post_sese.precursor", "loop[2].post_sese.begin")
              && ConstraintBranchInst("loop[2].post_sese.end")
              && ConstraintCFGEdge("loop[2].post_sese.end", "loop[2].post_sese.successor")
              && ConstraintCFGDominate("loop[2].post_sese.begin", "loop[2].post_sese.end")
              && ConstraintCFGPostdom("loop[2].post_sese.end", "loop[2].post_sese.begin")
              && ConstraintCFGDominateStrict("loop[2].post_sese.precursor", "loop[2].post_sese.begin")
              && ConstraintCFGPostdomStrict("loop[2].post_sese.successor", "loop[2].post_sese.end")
              && ConstraintCFGBlocked("loop[2].post_sese.begin", "loop[2].post_sese.end", "loop[2].post_sese.precursor")
              && ConstraintCFGBlocked("loop[2].post_sese.successor", "loop[2].post_sese.begin", "loop[2].post_sese.end"))
           || (  ConstraintSame("loop[2].post_sese.precursor", "loop[2].post_sese.end")
              && ConstraintSame("loop[2].post_sese.begin", "loop[2].post_sese.successor")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[2].iter_begin"))
           || ConstraintCFGDominateStrict("loop[2].iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[2].iter_begin")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[2].iter_step"))
           || ConstraintCFGDominateStrict("loop[2].iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[2].iter_step")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[2].iter_end"))
           || ConstraintCFGDominateStrict("loop[2].iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[2].iter_end")))
        && ConstraintSame("body_sese.precursor", "loop[2].body_sese.precursor")
        && ConstraintSame("body_sese.begin", "loop[2].body_sese.begin")
        && ConstraintSame("body_sese.end", "loop[2].body_sese.end")
        && ConstraintSame("body_sese.successor", "loop[2].body_sese.successor")
        && ConstraintStoreInst("output.store")
        && ConstraintSecondArgument("output.access_pointer", "output.store")
        && ConstraintGEPInst("output.access_pointer")
        && ConstraintFirstArgument("output.base_pointer", "output.access_pointer")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("output.base_pointer"))
           || ConstraintCFGDominateStrict("output.base_pointer", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("output.base_pointer")))
        && ConstraintSecondArgument("output.output_index", "output.access_pointer")
        && (  (  ConstraintZExtInst("output.output_index")
              && ConstraintDFGEdge("output.offset_add.value", "output.output_index"))
           || (  ConstraintSExtInst("output.output_index")
              && ConstraintDFGEdge("output.offset_add.value", "output.output_index"))
           || ConstraintSame("output.offset_add.value", "output.output_index"))
        && (  (  ConstraintSame("output.offset_add.input", "output.offset_add.value")
              && ConstraintUnused("output.offset_add.addend"))
           || (  (  ConstraintAddInst("output.offset_add.value")
                 || ConstraintBitOrInst("output.offset_add.value")
                 || ConstraintFAddInst("output.offset_add.value"))
              && (  (  ConstraintFirstArgument("output.offset_add.input", "output.offset_add.value")
                    && ConstraintSecondArgument("output.offset_add.addend", "output.offset_add.value"))
                 || (  ConstraintFirstArgument("output.offset_add.addend", "output.offset_add.value")
                    && ConstraintSecondArgument("output.offset_add.input", "output.offset_add.value"))))
           || (  (  ConstraintSubInst("output.offset_add.value")
                 || ConstraintFSubInst("output.offset_add.value"))
              && (  (  ConstraintFirstArgument("output.offset_add.input", "output.offset_add.value")
                    && ConstraintSecondArgument("output.offset_add.addend", "output.offset_add.value"))
                 || (  ConstraintFirstArgument("output.offset_add.addend", "output.offset_add.value")
                    && ConstraintSecondArgument("output.offset_add.input", "output.offset_add.value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("output.offset_add.addend"))
           || ConstraintCFGDominateStrict("output.offset_add.addend", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("output.offset_add.addend")))
        && ConstraintSame("output.stride_mul[1].value", "output.offset_add.input")
        && (  (  ConstraintSame("output.stride_mul[1].input", "output.stride_mul[1].value")
              && ConstraintUnused("output.stride_mul[1].multiplier"))
           || (  (  ConstraintMulInst("output.stride_mul[1].value")
                 || ConstraintLShiftInst("output.stride_mul[1].value")
                 || ConstraintFMulInst("output.stride_mul[1].value"))
              && (  (  ConstraintFirstArgument("output.stride_mul[1].input", "output.stride_mul[1].value")
                    && ConstraintSecondArgument("output.stride_mul[1].multiplier", "output.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("output.stride_mul[1].multiplier", "output.stride_mul[1].value")
                    && ConstraintSecondArgument("output.stride_mul[1].input", "output.stride_mul[1].value"))))
           || (  ConstraintFDivInst("output.stride_mul[1].value")
              && (  (  ConstraintFirstArgument("output.stride_mul[1].input", "output.stride_mul[1].value")
                    && ConstraintSecondArgument("output.stride_mul[1].multiplier", "output.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("output.stride_mul[1].multiplier", "output.stride_mul[1].value")
                    && ConstraintSecondArgument("output.stride_mul[1].input", "output.stride_mul[1].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("output.stride_mul[1].multiplier"))
           || ConstraintCFGDominateStrict("output.stride_mul[1].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("output.stride_mul[1].multiplier")))
        && ConstraintSame("output.index_add[1].value", "output.stride_mul[1].input")
        && (  (  ConstraintSame("output.index_add[1].input", "output.index_add[1].value")
              && ConstraintUnused("output.index_add[1].addend"))
           || (  (  ConstraintAddInst("output.index_add[1].value")
                 || ConstraintBitOrInst("output.index_add[1].value")
                 || ConstraintFAddInst("output.index_add[1].value"))
              && (  (  ConstraintFirstArgument("output.index_add[1].input", "output.index_add[1].value")
                    && ConstraintSecondArgument("output.index_add[1].addend", "output.index_add[1].value"))
                 || (  ConstraintFirstArgument("output.index_add[1].addend", "output.index_add[1].value")
                    && ConstraintSecondArgument("output.index_add[1].input", "output.index_add[1].value"))))
           || (  (  ConstraintSubInst("output.index_add[1].value")
                 || ConstraintFSubInst("output.index_add[1].value"))
              && (  (  ConstraintFirstArgument("output.index_add[1].input", "output.index_add[1].value")
                    && ConstraintSecondArgument("output.index_add[1].addend", "output.index_add[1].value"))
                 || (  ConstraintFirstArgument("output.index_add[1].addend", "output.index_add[1].value")
                    && ConstraintSecondArgument("output.index_add[1].input", "output.index_add[1].value")))))
        && (  (  ConstraintZExtInst("output.index_add[1].addend")
              && ConstraintDFGEdge("output.input_index[1]", "output.index_add[1].addend"))
           || (  ConstraintSExtInst("output.index_add[1].addend")
              && ConstraintDFGEdge("output.input_index[1]", "output.index_add[1].addend"))
           || ConstraintSame("output.input_index[1]", "output.index_add[1].addend"))
        && (  (  ConstraintDistinct("output.index_add[1].addend", "output.offset_add.value")
              && ConstraintIntegerType("output.input_index[1]"))
           || (  ConstraintSame("output.input_index[1]", "output.index_add[1].addend")
              && ConstraintIntegerType("output.offset_add.value")))
        && ConstraintSame("output.stride_mul[0].value", "output.index_add[1].input")
        && (  (  ConstraintSame("output.stride_mul[0].input", "output.stride_mul[0].value")
              && ConstraintUnused("output.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("output.stride_mul[0].value")
                 || ConstraintLShiftInst("output.stride_mul[0].value")
                 || ConstraintFMulInst("output.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("output.stride_mul[0].input", "output.stride_mul[0].value")
                    && ConstraintSecondArgument("output.stride_mul[0].multiplier", "output.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("output.stride_mul[0].multiplier", "output.stride_mul[0].value")
                    && ConstraintSecondArgument("output.stride_mul[0].input", "output.stride_mul[0].value"))))
           || (  ConstraintFDivInst("output.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("output.stride_mul[0].input", "output.stride_mul[0].value")
                    && ConstraintSecondArgument("output.stride_mul[0].multiplier", "output.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("output.stride_mul[0].multiplier", "output.stride_mul[0].value")
                    && ConstraintSecondArgument("output.stride_mul[0].input", "output.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("output.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("output.stride_mul[0].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("output.stride_mul[0].multiplier")))
        && ConstraintSame("output.index_add[0].value", "output.stride_mul[0].input")
        && (  (  ConstraintSame("output.index_add[0].input", "output.index_add[0].value")
              && ConstraintUnused("output.index_add[0].addend"))
           || (  (  ConstraintAddInst("output.index_add[0].value")
                 || ConstraintBitOrInst("output.index_add[0].value")
                 || ConstraintFAddInst("output.index_add[0].value"))
              && (  (  ConstraintFirstArgument("output.index_add[0].input", "output.index_add[0].value")
                    && ConstraintSecondArgument("output.index_add[0].addend", "output.index_add[0].value"))
                 || (  ConstraintFirstArgument("output.index_add[0].addend", "output.index_add[0].value")
                    && ConstraintSecondArgument("output.index_add[0].input", "output.index_add[0].value"))))
           || (  (  ConstraintSubInst("output.index_add[0].value")
                 || ConstraintFSubInst("output.index_add[0].value"))
              && (  (  ConstraintFirstArgument("output.index_add[0].input", "output.index_add[0].value")
                    && ConstraintSecondArgument("output.index_add[0].addend", "output.index_add[0].value"))
                 || (  ConstraintFirstArgument("output.index_add[0].addend", "output.index_add[0].value")
                    && ConstraintSecondArgument("output.index_add[0].input", "output.index_add[0].value")))))
        && (  (  ConstraintZExtInst("output.index_add[0].input")
              && ConstraintDFGEdge("output.input_index[0]", "output.index_add[0].input"))
           || (  ConstraintSExtInst("output.index_add[0].input")
              && ConstraintDFGEdge("output.input_index[0]", "output.index_add[0].input"))
           || ConstraintSame("output.input_index[0]", "output.index_add[0].input"))
        && (  (  ConstraintDistinct("output.index_add[0].input", "output.input_index[1]")
              && ConstraintIntegerType("output.input_index[0]"))
           || (  ConstraintSame("output.input_index[0]", "output.index_add[0].input")
              && ConstraintIntegerType("output.input_index[1]")))
        && ConstraintSame("output.base_index", "output.index_add[0].addend")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("output.base_index"))
           || ConstraintCFGDominateStrict("output.base_index", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("output.base_index")))
        && (  (  ConstraintSame("loop[0].iterator", "output.input_index[0]")
              && ConstraintSame("loop[1].iterator", "output.input_index[1]"))
           || (  ConstraintSame("loop[0].iterator", "output.input_index[1]")
              && ConstraintSame("loop[1].iterator", "output.input_index[0]")))
        && ConstraintLoadInst("input1.value")
        && ConstraintFirstArgument("input1.access_pointer", "input1.value")
        && ConstraintGEPInst("input1.access_pointer")
        && ConstraintFirstArgument("input1.base_pointer", "input1.access_pointer")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input1.base_pointer"))
           || ConstraintCFGDominateStrict("input1.base_pointer", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input1.base_pointer")))
        && ConstraintSecondArgument("input1.output_index", "input1.access_pointer")
        && (  (  ConstraintZExtInst("input1.output_index")
              && ConstraintDFGEdge("input1.offset_add.value", "input1.output_index"))
           || (  ConstraintSExtInst("input1.output_index")
              && ConstraintDFGEdge("input1.offset_add.value", "input1.output_index"))
           || ConstraintSame("input1.offset_add.value", "input1.output_index"))
        && (  (  ConstraintSame("input1.offset_add.input", "input1.offset_add.value")
              && ConstraintUnused("input1.offset_add.addend"))
           || (  (  ConstraintAddInst("input1.offset_add.value")
                 || ConstraintBitOrInst("input1.offset_add.value")
                 || ConstraintFAddInst("input1.offset_add.value"))
              && (  (  ConstraintFirstArgument("input1.offset_add.input", "input1.offset_add.value")
                    && ConstraintSecondArgument("input1.offset_add.addend", "input1.offset_add.value"))
                 || (  ConstraintFirstArgument("input1.offset_add.addend", "input1.offset_add.value")
                    && ConstraintSecondArgument("input1.offset_add.input", "input1.offset_add.value"))))
           || (  (  ConstraintSubInst("input1.offset_add.value")
                 || ConstraintFSubInst("input1.offset_add.value"))
              && (  (  ConstraintFirstArgument("input1.offset_add.input", "input1.offset_add.value")
                    && ConstraintSecondArgument("input1.offset_add.addend", "input1.offset_add.value"))
                 || (  ConstraintFirstArgument("input1.offset_add.addend", "input1.offset_add.value")
                    && ConstraintSecondArgument("input1.offset_add.input", "input1.offset_add.value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input1.offset_add.addend"))
           || ConstraintCFGDominateStrict("input1.offset_add.addend", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input1.offset_add.addend")))
        && ConstraintSame("input1.stride_mul[1].value", "input1.offset_add.input")
        && (  (  ConstraintSame("input1.stride_mul[1].input", "input1.stride_mul[1].value")
              && ConstraintUnused("input1.stride_mul[1].multiplier"))
           || (  (  ConstraintMulInst("input1.stride_mul[1].value")
                 || ConstraintLShiftInst("input1.stride_mul[1].value")
                 || ConstraintFMulInst("input1.stride_mul[1].value"))
              && (  (  ConstraintFirstArgument("input1.stride_mul[1].input", "input1.stride_mul[1].value")
                    && ConstraintSecondArgument("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value")
                    && ConstraintSecondArgument("input1.stride_mul[1].input", "input1.stride_mul[1].value"))))
           || (  ConstraintFDivInst("input1.stride_mul[1].value")
              && (  (  ConstraintFirstArgument("input1.stride_mul[1].input", "input1.stride_mul[1].value")
                    && ConstraintSecondArgument("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("input1.stride_mul[1].multiplier", "input1.stride_mul[1].value")
                    && ConstraintSecondArgument("input1.stride_mul[1].input", "input1.stride_mul[1].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input1.stride_mul[1].multiplier"))
           || ConstraintCFGDominateStrict("input1.stride_mul[1].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input1.stride_mul[1].multiplier")))
        && ConstraintSame("input1.index_add[1].value", "input1.stride_mul[1].input")
        && (  (  ConstraintSame("input1.index_add[1].input", "input1.index_add[1].value")
              && ConstraintUnused("input1.index_add[1].addend"))
           || (  (  ConstraintAddInst("input1.index_add[1].value")
                 || ConstraintBitOrInst("input1.index_add[1].value")
                 || ConstraintFAddInst("input1.index_add[1].value"))
              && (  (  ConstraintFirstArgument("input1.index_add[1].input", "input1.index_add[1].value")
                    && ConstraintSecondArgument("input1.index_add[1].addend", "input1.index_add[1].value"))
                 || (  ConstraintFirstArgument("input1.index_add[1].addend", "input1.index_add[1].value")
                    && ConstraintSecondArgument("input1.index_add[1].input", "input1.index_add[1].value"))))
           || (  (  ConstraintSubInst("input1.index_add[1].value")
                 || ConstraintFSubInst("input1.index_add[1].value"))
              && (  (  ConstraintFirstArgument("input1.index_add[1].input", "input1.index_add[1].value")
                    && ConstraintSecondArgument("input1.index_add[1].addend", "input1.index_add[1].value"))
                 || (  ConstraintFirstArgument("input1.index_add[1].addend", "input1.index_add[1].value")
                    && ConstraintSecondArgument("input1.index_add[1].input", "input1.index_add[1].value")))))
        && (  (  ConstraintZExtInst("input1.index_add[1].addend")
              && ConstraintDFGEdge("input1.input_index[1]", "input1.index_add[1].addend"))
           || (  ConstraintSExtInst("input1.index_add[1].addend")
              && ConstraintDFGEdge("input1.input_index[1]", "input1.index_add[1].addend"))
           || ConstraintSame("input1.input_index[1]", "input1.index_add[1].addend"))
        && (  (  ConstraintDistinct("input1.index_add[1].addend", "input1.offset_add.value")
              && ConstraintIntegerType("input1.input_index[1]"))
           || (  ConstraintSame("input1.input_index[1]", "input1.index_add[1].addend")
              && ConstraintIntegerType("input1.offset_add.value")))
        && ConstraintSame("input1.stride_mul[0].value", "input1.index_add[1].input")
        && (  (  ConstraintSame("input1.stride_mul[0].input", "input1.stride_mul[0].value")
              && ConstraintUnused("input1.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("input1.stride_mul[0].value")
                 || ConstraintLShiftInst("input1.stride_mul[0].value")
                 || ConstraintFMulInst("input1.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("input1.stride_mul[0].input", "input1.stride_mul[0].value")
                    && ConstraintSecondArgument("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value")
                    && ConstraintSecondArgument("input1.stride_mul[0].input", "input1.stride_mul[0].value"))))
           || (  ConstraintFDivInst("input1.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("input1.stride_mul[0].input", "input1.stride_mul[0].value")
                    && ConstraintSecondArgument("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("input1.stride_mul[0].multiplier", "input1.stride_mul[0].value")
                    && ConstraintSecondArgument("input1.stride_mul[0].input", "input1.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input1.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("input1.stride_mul[0].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input1.stride_mul[0].multiplier")))
        && ConstraintSame("input1.index_add[0].value", "input1.stride_mul[0].input")
        && (  (  ConstraintSame("input1.index_add[0].input", "input1.index_add[0].value")
              && ConstraintUnused("input1.index_add[0].addend"))
           || (  (  ConstraintAddInst("input1.index_add[0].value")
                 || ConstraintBitOrInst("input1.index_add[0].value")
                 || ConstraintFAddInst("input1.index_add[0].value"))
              && (  (  ConstraintFirstArgument("input1.index_add[0].input", "input1.index_add[0].value")
                    && ConstraintSecondArgument("input1.index_add[0].addend", "input1.index_add[0].value"))
                 || (  ConstraintFirstArgument("input1.index_add[0].addend", "input1.index_add[0].value")
                    && ConstraintSecondArgument("input1.index_add[0].input", "input1.index_add[0].value"))))
           || (  (  ConstraintSubInst("input1.index_add[0].value")
                 || ConstraintFSubInst("input1.index_add[0].value"))
              && (  (  ConstraintFirstArgument("input1.index_add[0].input", "input1.index_add[0].value")
                    && ConstraintSecondArgument("input1.index_add[0].addend", "input1.index_add[0].value"))
                 || (  ConstraintFirstArgument("input1.index_add[0].addend", "input1.index_add[0].value")
                    && ConstraintSecondArgument("input1.index_add[0].input", "input1.index_add[0].value")))))
        && (  (  ConstraintZExtInst("input1.index_add[0].input")
              && ConstraintDFGEdge("input1.input_index[0]", "input1.index_add[0].input"))
           || (  ConstraintSExtInst("input1.index_add[0].input")
              && ConstraintDFGEdge("input1.input_index[0]", "input1.index_add[0].input"))
           || ConstraintSame("input1.input_index[0]", "input1.index_add[0].input"))
        && (  (  ConstraintDistinct("input1.index_add[0].input", "input1.input_index[1]")
              && ConstraintIntegerType("input1.input_index[0]"))
           || (  ConstraintSame("input1.input_index[0]", "input1.index_add[0].input")
              && ConstraintIntegerType("input1.input_index[1]")))
        && ConstraintSame("input1.base_index", "input1.index_add[0].addend")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input1.base_index"))
           || ConstraintCFGDominateStrict("input1.base_index", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input1.base_index")))
        && (  (  ConstraintSame("loop[0].iterator", "input1.input_index[0]")
              && ConstraintSame("loop[2].iterator", "input1.input_index[1]"))
           || (  ConstraintSame("loop[0].iterator", "input1.input_index[1]")
              && ConstraintSame("loop[2].iterator", "input1.input_index[0]")))
        && ConstraintLoadInst("input2.value")
        && ConstraintFirstArgument("input2.access_pointer", "input2.value")
        && ConstraintGEPInst("input2.access_pointer")
        && ConstraintFirstArgument("input2.base_pointer", "input2.access_pointer")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input2.base_pointer"))
           || ConstraintCFGDominateStrict("input2.base_pointer", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input2.base_pointer")))
        && ConstraintSecondArgument("input2.output_index", "input2.access_pointer")
        && (  (  ConstraintZExtInst("input2.output_index")
              && ConstraintDFGEdge("input2.offset_add.value", "input2.output_index"))
           || (  ConstraintSExtInst("input2.output_index")
              && ConstraintDFGEdge("input2.offset_add.value", "input2.output_index"))
           || ConstraintSame("input2.offset_add.value", "input2.output_index"))
        && (  (  ConstraintSame("input2.offset_add.input", "input2.offset_add.value")
              && ConstraintUnused("input2.offset_add.addend"))
           || (  (  ConstraintAddInst("input2.offset_add.value")
                 || ConstraintBitOrInst("input2.offset_add.value")
                 || ConstraintFAddInst("input2.offset_add.value"))
              && (  (  ConstraintFirstArgument("input2.offset_add.input", "input2.offset_add.value")
                    && ConstraintSecondArgument("input2.offset_add.addend", "input2.offset_add.value"))
                 || (  ConstraintFirstArgument("input2.offset_add.addend", "input2.offset_add.value")
                    && ConstraintSecondArgument("input2.offset_add.input", "input2.offset_add.value"))))
           || (  (  ConstraintSubInst("input2.offset_add.value")
                 || ConstraintFSubInst("input2.offset_add.value"))
              && (  (  ConstraintFirstArgument("input2.offset_add.input", "input2.offset_add.value")
                    && ConstraintSecondArgument("input2.offset_add.addend", "input2.offset_add.value"))
                 || (  ConstraintFirstArgument("input2.offset_add.addend", "input2.offset_add.value")
                    && ConstraintSecondArgument("input2.offset_add.input", "input2.offset_add.value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input2.offset_add.addend"))
           || ConstraintCFGDominateStrict("input2.offset_add.addend", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input2.offset_add.addend")))
        && ConstraintSame("input2.stride_mul[1].value", "input2.offset_add.input")
        && (  (  ConstraintSame("input2.stride_mul[1].input", "input2.stride_mul[1].value")
              && ConstraintUnused("input2.stride_mul[1].multiplier"))
           || (  (  ConstraintMulInst("input2.stride_mul[1].value")
                 || ConstraintLShiftInst("input2.stride_mul[1].value")
                 || ConstraintFMulInst("input2.stride_mul[1].value"))
              && (  (  ConstraintFirstArgument("input2.stride_mul[1].input", "input2.stride_mul[1].value")
                    && ConstraintSecondArgument("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value")
                    && ConstraintSecondArgument("input2.stride_mul[1].input", "input2.stride_mul[1].value"))))
           || (  ConstraintFDivInst("input2.stride_mul[1].value")
              && (  (  ConstraintFirstArgument("input2.stride_mul[1].input", "input2.stride_mul[1].value")
                    && ConstraintSecondArgument("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("input2.stride_mul[1].multiplier", "input2.stride_mul[1].value")
                    && ConstraintSecondArgument("input2.stride_mul[1].input", "input2.stride_mul[1].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input2.stride_mul[1].multiplier"))
           || ConstraintCFGDominateStrict("input2.stride_mul[1].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input2.stride_mul[1].multiplier")))
        && ConstraintSame("input2.index_add[1].value", "input2.stride_mul[1].input")
        && (  (  ConstraintSame("input2.index_add[1].input", "input2.index_add[1].value")
              && ConstraintUnused("input2.index_add[1].addend"))
           || (  (  ConstraintAddInst("input2.index_add[1].value")
                 || ConstraintBitOrInst("input2.index_add[1].value")
                 || ConstraintFAddInst("input2.index_add[1].value"))
              && (  (  ConstraintFirstArgument("input2.index_add[1].input", "input2.index_add[1].value")
                    && ConstraintSecondArgument("input2.index_add[1].addend", "input2.index_add[1].value"))
                 || (  ConstraintFirstArgument("input2.index_add[1].addend", "input2.index_add[1].value")
                    && ConstraintSecondArgument("input2.index_add[1].input", "input2.index_add[1].value"))))
           || (  (  ConstraintSubInst("input2.index_add[1].value")
                 || ConstraintFSubInst("input2.index_add[1].value"))
              && (  (  ConstraintFirstArgument("input2.index_add[1].input", "input2.index_add[1].value")
                    && ConstraintSecondArgument("input2.index_add[1].addend", "input2.index_add[1].value"))
                 || (  ConstraintFirstArgument("input2.index_add[1].addend", "input2.index_add[1].value")
                    && ConstraintSecondArgument("input2.index_add[1].input", "input2.index_add[1].value")))))
        && (  (  ConstraintZExtInst("input2.index_add[1].addend")
              && ConstraintDFGEdge("input2.input_index[1]", "input2.index_add[1].addend"))
           || (  ConstraintSExtInst("input2.index_add[1].addend")
              && ConstraintDFGEdge("input2.input_index[1]", "input2.index_add[1].addend"))
           || ConstraintSame("input2.input_index[1]", "input2.index_add[1].addend"))
        && (  (  ConstraintDistinct("input2.index_add[1].addend", "input2.offset_add.value")
              && ConstraintIntegerType("input2.input_index[1]"))
           || (  ConstraintSame("input2.input_index[1]", "input2.index_add[1].addend")
              && ConstraintIntegerType("input2.offset_add.value")))
        && ConstraintSame("input2.stride_mul[0].value", "input2.index_add[1].input")
        && (  (  ConstraintSame("input2.stride_mul[0].input", "input2.stride_mul[0].value")
              && ConstraintUnused("input2.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("input2.stride_mul[0].value")
                 || ConstraintLShiftInst("input2.stride_mul[0].value")
                 || ConstraintFMulInst("input2.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("input2.stride_mul[0].input", "input2.stride_mul[0].value")
                    && ConstraintSecondArgument("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value")
                    && ConstraintSecondArgument("input2.stride_mul[0].input", "input2.stride_mul[0].value"))))
           || (  ConstraintFDivInst("input2.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("input2.stride_mul[0].input", "input2.stride_mul[0].value")
                    && ConstraintSecondArgument("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("input2.stride_mul[0].multiplier", "input2.stride_mul[0].value")
                    && ConstraintSecondArgument("input2.stride_mul[0].input", "input2.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input2.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("input2.stride_mul[0].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input2.stride_mul[0].multiplier")))
        && ConstraintSame("input2.index_add[0].value", "input2.stride_mul[0].input")
        && (  (  ConstraintSame("input2.index_add[0].input", "input2.index_add[0].value")
              && ConstraintUnused("input2.index_add[0].addend"))
           || (  (  ConstraintAddInst("input2.index_add[0].value")
                 || ConstraintBitOrInst("input2.index_add[0].value")
                 || ConstraintFAddInst("input2.index_add[0].value"))
              && (  (  ConstraintFirstArgument("input2.index_add[0].input", "input2.index_add[0].value")
                    && ConstraintSecondArgument("input2.index_add[0].addend", "input2.index_add[0].value"))
                 || (  ConstraintFirstArgument("input2.index_add[0].addend", "input2.index_add[0].value")
                    && ConstraintSecondArgument("input2.index_add[0].input", "input2.index_add[0].value"))))
           || (  (  ConstraintSubInst("input2.index_add[0].value")
                 || ConstraintFSubInst("input2.index_add[0].value"))
              && (  (  ConstraintFirstArgument("input2.index_add[0].input", "input2.index_add[0].value")
                    && ConstraintSecondArgument("input2.index_add[0].addend", "input2.index_add[0].value"))
                 || (  ConstraintFirstArgument("input2.index_add[0].addend", "input2.index_add[0].value")
                    && ConstraintSecondArgument("input2.index_add[0].input", "input2.index_add[0].value")))))
        && (  (  ConstraintZExtInst("input2.index_add[0].input")
              && ConstraintDFGEdge("input2.input_index[0]", "input2.index_add[0].input"))
           || (  ConstraintSExtInst("input2.index_add[0].input")
              && ConstraintDFGEdge("input2.input_index[0]", "input2.index_add[0].input"))
           || ConstraintSame("input2.input_index[0]", "input2.index_add[0].input"))
        && (  (  ConstraintDistinct("input2.index_add[0].input", "input2.input_index[1]")
              && ConstraintIntegerType("input2.input_index[0]"))
           || (  ConstraintSame("input2.input_index[0]", "input2.index_add[0].input")
              && ConstraintIntegerType("input2.input_index[1]")))
        && ConstraintSame("input2.base_index", "input2.index_add[0].addend")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("input2.base_index"))
           || ConstraintCFGDominateStrict("input2.base_index", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("input2.base_index")))
        && (  (  ConstraintSame("loop[1].iterator", "input2.input_index[0]")
              && ConstraintSame("loop[2].iterator", "input2.input_index[1]"))
           || (  ConstraintSame("loop[1].iterator", "input2.input_index[1]")
              && ConstraintSame("loop[2].iterator", "input2.input_index[0]")))
        && ConstraintSame("loop[2].update_expr.input[0]", "loop[2].old_value")
        && ConstraintSame("loop[2].update_expr.input[1]", "input1.value")
        && ConstraintSame("loop[2].update_expr.input[2]", "input2.value")
        && ConstraintUnused("loop[2].update_expr.input[3]")
        && ConstraintUnused("loop[2].update_expr.input[4]")
        && ConstraintUnused("loop[2].update_expr.input[5]")
        && ConstraintUnused("loop[2].update_expr.input[6]")
        && ConstraintUnused("loop[2].update_expr.input[7]")
        && ConstraintUnused("loop[2].update_expr.input[8]")
        && ConstraintUnused("loop[2].update_expr.input[9]")
        && ConstraintUnused("loop[2].update_expr.input[10]")
        && ConstraintUnused("loop[2].update_expr.input[11]")
        && ConstraintUnused("loop[2].update_expr.input[12]")
        && ConstraintUnused("loop[2].update_expr.input[13]")
        && ConstraintUnused("loop[2].update_expr.input[14]")
        && ConstraintUnused("loop[2].update_expr.input[15]")
        && ConstraintUnused("loop[2].update_expr.input[16]")
        && ConstraintUnused("loop[2].update_expr.input[17]")
        && ConstraintUnused("loop[2].update_expr.input[18]")
        && ConstraintUnused("loop[2].update_expr.input[19]")
        && ConstraintUnused("loop[2].update_expr.input[20]")
        && ConstraintUnused("loop[2].update_expr.input[21]")
        && ConstraintUnused("loop[2].update_expr.input[22]")
        && ConstraintUnused("loop[2].update_expr.input[23]")
        && ConstraintUnused("loop[2].update_expr.input[24]")
        && ConstraintUnused("loop[2].update_expr.input[25]")
        && ConstraintUnused("loop[2].update_expr.input[26]")
        && ConstraintUnused("loop[2].update_expr.input[27]")
        && ConstraintUnused("loop[2].update_expr.input[28]")
        && ConstraintUnused("loop[2].update_expr.input[29]")
        && ConstraintUnused("loop[2].update_expr.input[30]")
        && ConstraintUnused("loop[2].update_expr.input[31]")
        && ConstraintIncomingValue("loop[2].update_expr.output", "loop[2].body_sese.end", "loop[2].old_value")
        && ConstraintDistinct("loop[2].old_value", "loop[2].iterator")
        && std::make_pair("i",4) * ConstraintCFGEdge("loop[2].update_expr.origin[i]", "loop[2].body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("loop[2].begin")
                                          && ConstraintPreexecution("loop[2].update_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("loop[2].update_expr.constants[i].value", "loop[2].begin")
                                       || (  ConstraintInstruction("loop[2].begin")
                                          && ConstraintUnused("loop[2].update_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("loop[2].update_expr.constants[i].value")
                                    && ConstraintDFGEdge("loop[2].update_expr.constants[i].value", "loop[2].update_expr.constants[i].use")
                                    && ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].update_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("loop[2].begin", "loop[2].update_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("loop[2].update_expr.restrictions[i].value", "loop[2].body_sese.begin")
                                    && ConstraintDFGEdge("loop[2].update_expr.restrictions[i].value", "loop[2].update_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].update_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"loop[2].update_expr.restrictions[0].value",
                                                           "loop[2].update_expr.restrictions[1].value",
                                                           "loop[2].update_expr.restrictions[2].value",
                                                           "loop[2].update_expr.restrictions[3].value",
                                                           "loop[2].update_expr.restrictions[4].value",
                                                           "loop[2].update_expr.restrictions[5].value",
                                                           "loop[2].update_expr.restrictions[6].value",
                                                           "loop[2].update_expr.restrictions[7].value",
                                                           "loop[2].update_expr.restrictions[8].value",
                                                           "loop[2].update_expr.restrictions[9].value",
                                                           "loop[2].update_expr.restrictions[10].value",
                                                           "loop[2].update_expr.restrictions[11].value",
                                                           "loop[2].update_expr.restrictions[12].value",
                                                           "loop[2].update_expr.restrictions[13].value",
                                                           "loop[2].update_expr.restrictions[14].value",
                                                           "loop[2].update_expr.restrictions[15].value",
                                                           "loop[2].update_expr.restrictions[16].value",
                                                           "loop[2].update_expr.restrictions[17].value",
                                                           "loop[2].update_expr.restrictions[18].value",
                                                           "loop[2].update_expr.restrictions[19].value",
                                                           "loop[2].update_expr.restrictions[20].value",
                                                           "loop[2].update_expr.restrictions[21].value",
                                                           "loop[2].update_expr.restrictions[22].value",
                                                           "loop[2].update_expr.restrictions[23].value",
                                                           "loop[2].update_expr.restrictions[24].value",
                                                           "loop[2].update_expr.restrictions[25].value",
                                                           "loop[2].update_expr.restrictions[26].value",
                                                           "loop[2].update_expr.restrictions[27].value",
                                                           "loop[2].update_expr.restrictions[28].value",
                                                           "loop[2].update_expr.restrictions[29].value",
                                                           "loop[2].update_expr.restrictions[30].value",
                                                           "loop[2].update_expr.restrictions[31].value",
                                                           "loop[2].update_expr.restrictions[32].value",
                                                           "loop[2].update_expr.restrictions[33].value",
                                                           "loop[2].update_expr.restrictions[34].value",
                                                           "loop[2].update_expr.restrictions[35].value",
                                                           "loop[2].update_expr.restrictions[36].value",
                                                           "loop[2].update_expr.restrictions[37].value",
                                                           "loop[2].update_expr.restrictions[38].value",
                                                           "loop[2].update_expr.restrictions[39].value",
                                                           "loop[2].update_expr.restrictions[40].value",
                                                           "loop[2].update_expr.restrictions[41].value",
                                                           "loop[2].update_expr.restrictions[42].value",
                                                           "loop[2].update_expr.restrictions[43].value",
                                                           "loop[2].update_expr.restrictions[44].value",
                                                           "loop[2].update_expr.restrictions[45].value",
                                                           "loop[2].update_expr.restrictions[46].value",
                                                           "loop[2].update_expr.restrictions[47].value",
                                                           "loop[2].update_expr.restrictions[48].value",
                                                           "loop[2].update_expr.restrictions[49].value",
                                                           "loop[2].update_expr.restrictions[50].value",
                                                           "loop[2].update_expr.restrictions[51].value",
                                                           "loop[2].update_expr.restrictions[52].value",
                                                           "loop[2].update_expr.restrictions[53].value",
                                                           "loop[2].update_expr.restrictions[54].value",
                                                           "loop[2].update_expr.restrictions[55].value",
                                                           "loop[2].update_expr.restrictions[56].value",
                                                           "loop[2].update_expr.restrictions[57].value",
                                                           "loop[2].update_expr.restrictions[58].value",
                                                           "loop[2].update_expr.restrictions[59].value",
                                                           "loop[2].update_expr.restrictions[60].value",
                                                           "loop[2].update_expr.restrictions[61].value",
                                                           "loop[2].update_expr.restrictions[62].value",
                                                           "loop[2].update_expr.restrictions[63].value"},
                                  std::vector<std::string>{"loop[2].update_expr.origin[0]", "loop[2].update_expr.origin[1]",
                                                           "loop[2].update_expr.origin[2]", "loop[2].update_expr.origin[3]",
                                                           "loop[2].update_expr.constants[0].value",
                                                           "loop[2].update_expr.constants[1].value",
                                                           "loop[2].update_expr.constants[2].value",
                                                           "loop[2].update_expr.constants[3].value",
                                                           "loop[2].update_expr.constants[4].value",
                                                           "loop[2].update_expr.constants[5].value",
                                                           "loop[2].update_expr.constants[6].value",
                                                           "loop[2].update_expr.constants[7].value",
                                                           "loop[2].update_expr.constants[8].value",
                                                           "loop[2].update_expr.constants[9].value",
                                                           "loop[2].update_expr.constants[10].value",
                                                           "loop[2].update_expr.constants[11].value",
                                                           "loop[2].update_expr.constants[12].value",
                                                           "loop[2].update_expr.constants[13].value",
                                                           "loop[2].update_expr.constants[14].value",
                                                           "loop[2].update_expr.constants[15].value",
                                                           "loop[2].update_expr.constants[16].value",
                                                           "loop[2].update_expr.constants[17].value",
                                                           "loop[2].update_expr.constants[18].value",
                                                           "loop[2].update_expr.constants[19].value",
                                                           "loop[2].update_expr.constants[20].value",
                                                           "loop[2].update_expr.constants[21].value",
                                                           "loop[2].update_expr.constants[22].value",
                                                           "loop[2].update_expr.constants[23].value",
                                                           "loop[2].update_expr.constants[24].value",
                                                           "loop[2].update_expr.constants[25].value",
                                                           "loop[2].update_expr.constants[26].value",
                                                           "loop[2].update_expr.constants[27].value",
                                                           "loop[2].update_expr.constants[28].value",
                                                           "loop[2].update_expr.constants[29].value",
                                                           "loop[2].update_expr.constants[30].value",
                                                           "loop[2].update_expr.constants[31].value",
                                                           "loop[2].update_expr.constants[32].value",
                                                           "loop[2].update_expr.constants[33].value",
                                                           "loop[2].update_expr.constants[34].value",
                                                           "loop[2].update_expr.constants[35].value",
                                                           "loop[2].update_expr.constants[36].value",
                                                           "loop[2].update_expr.constants[37].value",
                                                           "loop[2].update_expr.constants[38].value",
                                                           "loop[2].update_expr.constants[39].value",
                                                           "loop[2].update_expr.constants[40].value",
                                                           "loop[2].update_expr.constants[41].value",
                                                           "loop[2].update_expr.constants[42].value",
                                                           "loop[2].update_expr.constants[43].value",
                                                           "loop[2].update_expr.constants[44].value",
                                                           "loop[2].update_expr.constants[45].value",
                                                           "loop[2].update_expr.constants[46].value",
                                                           "loop[2].update_expr.constants[47].value",
                                                           "loop[2].update_expr.constants[48].value",
                                                           "loop[2].update_expr.constants[49].value",
                                                           "loop[2].update_expr.constants[50].value",
                                                           "loop[2].update_expr.constants[51].value",
                                                           "loop[2].update_expr.constants[52].value",
                                                           "loop[2].update_expr.constants[53].value",
                                                           "loop[2].update_expr.constants[54].value",
                                                           "loop[2].update_expr.constants[55].value",
                                                           "loop[2].update_expr.constants[56].value",
                                                           "loop[2].update_expr.constants[57].value",
                                                           "loop[2].update_expr.constants[58].value",
                                                           "loop[2].update_expr.constants[59].value",
                                                           "loop[2].update_expr.constants[60].value",
                                                           "loop[2].update_expr.constants[61].value",
                                                           "loop[2].update_expr.constants[62].value",
                                                           "loop[2].update_expr.constants[63].value", "loop[2].update_expr.input[0]",
                                                           "loop[2].update_expr.input[1]", "loop[2].update_expr.input[2]",
                                                           "loop[2].update_expr.input[3]", "loop[2].update_expr.input[4]",
                                                           "loop[2].update_expr.input[5]", "loop[2].update_expr.input[6]",
                                                           "loop[2].update_expr.input[7]", "loop[2].update_expr.input[8]",
                                                           "loop[2].update_expr.input[9]", "loop[2].update_expr.input[10]",
                                                           "loop[2].update_expr.input[11]", "loop[2].update_expr.input[12]",
                                                           "loop[2].update_expr.input[13]", "loop[2].update_expr.input[14]",
                                                           "loop[2].update_expr.input[15]", "loop[2].update_expr.input[16]",
                                                           "loop[2].update_expr.input[17]", "loop[2].update_expr.input[18]",
                                                           "loop[2].update_expr.input[19]", "loop[2].update_expr.input[20]",
                                                           "loop[2].update_expr.input[21]", "loop[2].update_expr.input[22]",
                                                           "loop[2].update_expr.input[23]", "loop[2].update_expr.input[24]",
                                                           "loop[2].update_expr.input[25]", "loop[2].update_expr.input[26]",
                                                           "loop[2].update_expr.input[27]", "loop[2].update_expr.input[28]",
                                                           "loop[2].update_expr.input[29]", "loop[2].update_expr.input[30]",
                                                           "loop[2].update_expr.input[31]"},
                                  std::vector<std::string>{"loop[2].update_expr.output"}})
        && std::make_pair("i",4) * (  ConstraintDFGEdge("loop[2].incoming[i]", "loop[2].old_value")
                                   && ConstraintDistinct("loop[2].incoming[i]", "loop[2].update_expr.output"))
        && (  (  ConstraintLoadInst("old_value")
              && ConstraintDFGEdge("output.access_pointer", "old_value"))
           || (  ConstraintUnused("old_value")
              && ConstraintGEPInst("output.access_pointer")))
        && ConstraintFirstArgument("update_expr.output", "output.store")
        && ConstraintSame("update_expr.input[0]", "loop[2].old_value")
        && ConstraintSame("update_expr.input[1]", "old_value")
        && ConstraintUnused("update_expr.input[2]")
        && ConstraintUnused("update_expr.input[3]")
        && ConstraintUnused("update_expr.input[4]")
        && ConstraintUnused("update_expr.input[5]")
        && ConstraintUnused("update_expr.input[6]")
        && ConstraintUnused("update_expr.input[7]")
        && ConstraintUnused("update_expr.input[8]")
        && ConstraintUnused("update_expr.input[9]")
        && ConstraintUnused("update_expr.input[10]")
        && ConstraintUnused("update_expr.input[11]")
        && ConstraintUnused("update_expr.input[12]")
        && ConstraintUnused("update_expr.input[13]")
        && ConstraintUnused("update_expr.input[14]")
        && ConstraintUnused("update_expr.input[15]")
        && ConstraintUnused("update_expr.input[16]")
        && ConstraintUnused("update_expr.input[17]")
        && ConstraintUnused("update_expr.input[18]")
        && ConstraintUnused("update_expr.input[19]")
        && ConstraintUnused("update_expr.input[20]")
        && ConstraintUnused("update_expr.input[21]")
        && ConstraintUnused("update_expr.input[22]")
        && ConstraintUnused("update_expr.input[23]")
        && ConstraintUnused("update_expr.input[24]")
        && ConstraintUnused("update_expr.input[25]")
        && ConstraintUnused("update_expr.input[26]")
        && ConstraintUnused("update_expr.input[27]")
        && ConstraintUnused("update_expr.input[28]")
        && ConstraintUnused("update_expr.input[29]")
        && ConstraintUnused("update_expr.input[30]")
        && ConstraintUnused("update_expr.input[31]")
        && std::make_pair("i",4) * ConstraintCFGEdge("update_expr.origin[i]", "loop[1].body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("update_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("update_expr.constants[i].value", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("update_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("update_expr.constants[i].value")
                                    && ConstraintDFGEdge("update_expr.constants[i].value", "update_expr.constants[i].use")
                                    && ConstraintCFGDominate("loop[1].body_sese.begin", "update_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("begin", "update_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("update_expr.restrictions[i].value", "loop[1].body_sese.begin")
                                    && ConstraintDFGEdge("update_expr.restrictions[i].value", "update_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("loop[1].body_sese.begin", "update_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"update_expr.restrictions[0].value", "update_expr.restrictions[1].value",
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
                                  std::vector<std::string>{"update_expr.origin[0]", "update_expr.origin[1]", "update_expr.origin[2]",
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
                                  std::vector<std::string>{"update_expr.output"}});
}

ConstraintAnd ConstraintSparseMV()
{
    return ConstraintBranchInst("precursor")
        && ConstraintCFGEdge("precursor", "begin")
        && ConstraintBranchInst("end")
        && ConstraintCFGEdge("end", "successor")
        && ConstraintCFGDominate("begin", "end")
        && ConstraintCFGPostdom("end", "begin")
        && ConstraintCFGDominateStrict("precursor", "begin")
        && ConstraintCFGPostdomStrict("successor", "end")
        && ConstraintCFGBlocked("begin", "end", "precursor")
        && ConstraintCFGBlocked("successor", "begin", "end")
        && ConstraintSame("body_sese.precursor", "end")
        && ConstraintSame("body_sese.successor", "begin")
        && ConstraintBranchInst("body_sese.precursor")
        && ConstraintCFGEdge("body_sese.precursor", "body_sese.begin")
        && ConstraintBranchInst("body_sese.end")
        && ConstraintCFGEdge("body_sese.end", "body_sese.successor")
        && ConstraintCFGDominate("body_sese.begin", "body_sese.end")
        && ConstraintCFGPostdom("body_sese.end", "body_sese.begin")
        && ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin")
        && ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end")
        && ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor")
        && ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end")
        && ConstraintDistinct("body_sese.begin", "successor")
        && ConstraintIncomingValue("increment", "body_sese.end", "iterator")
        && ConstraintDFGEdge("iterator", "comparison")
        && ConstraintICmpInst("comparison")
        && ConstraintDFGEdge("comparison", "end")
        && ConstraintDFGEdge("increment", "iterator")
        && ConstraintAddInst("increment")
        && ConstraintDFGEdge("iterator", "increment")
        && ConstraintDFGEdge("iter_end", "comparison")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_end"))
           || ConstraintCFGDominateStrict("iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_end")))
        && ConstraintDFGEdge("iter_begin", "iterator")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_begin"))
           || ConstraintCFGDominateStrict("iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_begin")))
        && ConstraintDFGEdge("iter_step", "increment")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_step"))
           || ConstraintCFGDominateStrict("iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_step")))
        && ConstraintUnused("begin_idx.offset_add.addend")
        && ConstraintLoadInst("begin_idx.value")
        && ConstraintFirstArgument("begin_idx.access_pointer", "begin_idx.value")
        && ConstraintGEPInst("begin_idx.access_pointer")
        && ConstraintFirstArgument("begin_idx.base_pointer", "begin_idx.access_pointer")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("begin_idx.base_pointer"))
           || ConstraintCFGDominateStrict("begin_idx.base_pointer", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("begin_idx.base_pointer")))
        && ConstraintSecondArgument("begin_idx.output_index", "begin_idx.access_pointer")
        && (  (  ConstraintZExtInst("begin_idx.output_index")
              && ConstraintDFGEdge("begin_idx.offset_add.value", "begin_idx.output_index"))
           || (  ConstraintSExtInst("begin_idx.output_index")
              && ConstraintDFGEdge("begin_idx.offset_add.value", "begin_idx.output_index"))
           || ConstraintSame("begin_idx.offset_add.value", "begin_idx.output_index"))
        && (  (  ConstraintSame("begin_idx.offset_add.input", "begin_idx.offset_add.value")
              && ConstraintUnused("begin_idx.offset_add.addend"))
           || (  (  ConstraintAddInst("begin_idx.offset_add.value")
                 || ConstraintBitOrInst("begin_idx.offset_add.value")
                 || ConstraintFAddInst("begin_idx.offset_add.value"))
              && (  (  ConstraintFirstArgument("begin_idx.offset_add.input", "begin_idx.offset_add.value")
                    && ConstraintSecondArgument("begin_idx.offset_add.addend", "begin_idx.offset_add.value"))
                 || (  ConstraintFirstArgument("begin_idx.offset_add.addend", "begin_idx.offset_add.value")
                    && ConstraintSecondArgument("begin_idx.offset_add.input", "begin_idx.offset_add.value"))))
           || (  (  ConstraintSubInst("begin_idx.offset_add.value")
                 || ConstraintFSubInst("begin_idx.offset_add.value"))
              && (  (  ConstraintFirstArgument("begin_idx.offset_add.input", "begin_idx.offset_add.value")
                    && ConstraintSecondArgument("begin_idx.offset_add.addend", "begin_idx.offset_add.value"))
                 || (  ConstraintFirstArgument("begin_idx.offset_add.addend", "begin_idx.offset_add.value")
                    && ConstraintSecondArgument("begin_idx.offset_add.input", "begin_idx.offset_add.value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("begin_idx.offset_add.addend"))
           || ConstraintCFGDominateStrict("begin_idx.offset_add.addend", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("begin_idx.offset_add.addend")))
        && ConstraintSame("begin_idx.stride_mul[0].value", "begin_idx.offset_add.input")
        && (  (  ConstraintSame("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value")
              && ConstraintUnused("begin_idx.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("begin_idx.stride_mul[0].value")
                 || ConstraintLShiftInst("begin_idx.stride_mul[0].value")
                 || ConstraintFMulInst("begin_idx.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value"))))
           || (  ConstraintFDivInst("begin_idx.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("begin_idx.stride_mul[0].multiplier", "begin_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("begin_idx.stride_mul[0].input", "begin_idx.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("begin_idx.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("begin_idx.stride_mul[0].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("begin_idx.stride_mul[0].multiplier")))
        && ConstraintSame("begin_idx.index_add[0].value", "begin_idx.stride_mul[0].input")
        && (  (  ConstraintSame("begin_idx.index_add[0].input", "begin_idx.index_add[0].value")
              && ConstraintUnused("begin_idx.index_add[0].addend"))
           || (  (  ConstraintAddInst("begin_idx.index_add[0].value")
                 || ConstraintBitOrInst("begin_idx.index_add[0].value")
                 || ConstraintFAddInst("begin_idx.index_add[0].value"))
              && (  (  ConstraintFirstArgument("begin_idx.index_add[0].input", "begin_idx.index_add[0].value")
                    && ConstraintSecondArgument("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value"))
                 || (  ConstraintFirstArgument("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value")
                    && ConstraintSecondArgument("begin_idx.index_add[0].input", "begin_idx.index_add[0].value"))))
           || (  (  ConstraintSubInst("begin_idx.index_add[0].value")
                 || ConstraintFSubInst("begin_idx.index_add[0].value"))
              && (  (  ConstraintFirstArgument("begin_idx.index_add[0].input", "begin_idx.index_add[0].value")
                    && ConstraintSecondArgument("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value"))
                 || (  ConstraintFirstArgument("begin_idx.index_add[0].addend", "begin_idx.index_add[0].value")
                    && ConstraintSecondArgument("begin_idx.index_add[0].input", "begin_idx.index_add[0].value")))))
        && (  (  ConstraintZExtInst("begin_idx.index_add[0].input")
              && ConstraintDFGEdge("begin_idx.input_index[0]", "begin_idx.index_add[0].input"))
           || (  ConstraintSExtInst("begin_idx.index_add[0].input")
              && ConstraintDFGEdge("begin_idx.input_index[0]", "begin_idx.index_add[0].input"))
           || ConstraintSame("begin_idx.input_index[0]", "begin_idx.index_add[0].input"))
        && (  (  ConstraintDistinct("begin_idx.index_add[0].input", "begin_idx.offset_add.value")
              && ConstraintIntegerType("begin_idx.input_index[0]"))
           || (  ConstraintSame("begin_idx.input_index[0]", "begin_idx.index_add[0].input")
              && ConstraintIntegerType("begin_idx.offset_add.value")))
        && ConstraintSame("begin_idx.base_index", "begin_idx.index_add[0].addend")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("begin_idx.base_index"))
           || ConstraintCFGDominateStrict("begin_idx.base_index", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("begin_idx.base_index")))
        && ConstraintSame("iterator", "begin_idx.input_index[0]")
        && ConstraintSame("begin_idx.base_index", "end_idx.base_index")
        && ConstraintSame("begin_idx.base_pointer", "end_idx.base_pointer")
        && ConstraintLoadInst("end_idx.value")
        && ConstraintFirstArgument("end_idx.access_pointer", "end_idx.value")
        && ConstraintGEPInst("end_idx.access_pointer")
        && ConstraintFirstArgument("end_idx.base_pointer", "end_idx.access_pointer")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("end_idx.base_pointer"))
           || ConstraintCFGDominateStrict("end_idx.base_pointer", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("end_idx.base_pointer")))
        && ConstraintSecondArgument("end_idx.output_index", "end_idx.access_pointer")
        && (  (  ConstraintZExtInst("end_idx.output_index")
              && ConstraintDFGEdge("end_idx.offset_add.value", "end_idx.output_index"))
           || (  ConstraintSExtInst("end_idx.output_index")
              && ConstraintDFGEdge("end_idx.offset_add.value", "end_idx.output_index"))
           || ConstraintSame("end_idx.offset_add.value", "end_idx.output_index"))
        && (  (  ConstraintSame("end_idx.offset_add.input", "end_idx.offset_add.value")
              && ConstraintUnused("end_idx.offset_add.addend"))
           || (  (  ConstraintAddInst("end_idx.offset_add.value")
                 || ConstraintBitOrInst("end_idx.offset_add.value")
                 || ConstraintFAddInst("end_idx.offset_add.value"))
              && (  (  ConstraintFirstArgument("end_idx.offset_add.input", "end_idx.offset_add.value")
                    && ConstraintSecondArgument("end_idx.offset_add.addend", "end_idx.offset_add.value"))
                 || (  ConstraintFirstArgument("end_idx.offset_add.addend", "end_idx.offset_add.value")
                    && ConstraintSecondArgument("end_idx.offset_add.input", "end_idx.offset_add.value"))))
           || (  (  ConstraintSubInst("end_idx.offset_add.value")
                 || ConstraintFSubInst("end_idx.offset_add.value"))
              && (  (  ConstraintFirstArgument("end_idx.offset_add.input", "end_idx.offset_add.value")
                    && ConstraintSecondArgument("end_idx.offset_add.addend", "end_idx.offset_add.value"))
                 || (  ConstraintFirstArgument("end_idx.offset_add.addend", "end_idx.offset_add.value")
                    && ConstraintSecondArgument("end_idx.offset_add.input", "end_idx.offset_add.value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("end_idx.offset_add.addend"))
           || ConstraintCFGDominateStrict("end_idx.offset_add.addend", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("end_idx.offset_add.addend")))
        && ConstraintSame("end_idx.stride_mul[0].value", "end_idx.offset_add.input")
        && (  (  ConstraintSame("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value")
              && ConstraintUnused("end_idx.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("end_idx.stride_mul[0].value")
                 || ConstraintLShiftInst("end_idx.stride_mul[0].value")
                 || ConstraintFMulInst("end_idx.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value"))))
           || (  ConstraintFDivInst("end_idx.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("end_idx.stride_mul[0].multiplier", "end_idx.stride_mul[0].value")
                    && ConstraintSecondArgument("end_idx.stride_mul[0].input", "end_idx.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("end_idx.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("end_idx.stride_mul[0].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("end_idx.stride_mul[0].multiplier")))
        && ConstraintSame("end_idx.index_add[0].value", "end_idx.stride_mul[0].input")
        && (  (  ConstraintSame("end_idx.index_add[0].input", "end_idx.index_add[0].value")
              && ConstraintUnused("end_idx.index_add[0].addend"))
           || (  (  ConstraintAddInst("end_idx.index_add[0].value")
                 || ConstraintBitOrInst("end_idx.index_add[0].value")
                 || ConstraintFAddInst("end_idx.index_add[0].value"))
              && (  (  ConstraintFirstArgument("end_idx.index_add[0].input", "end_idx.index_add[0].value")
                    && ConstraintSecondArgument("end_idx.index_add[0].addend", "end_idx.index_add[0].value"))
                 || (  ConstraintFirstArgument("end_idx.index_add[0].addend", "end_idx.index_add[0].value")
                    && ConstraintSecondArgument("end_idx.index_add[0].input", "end_idx.index_add[0].value"))))
           || (  (  ConstraintSubInst("end_idx.index_add[0].value")
                 || ConstraintFSubInst("end_idx.index_add[0].value"))
              && (  (  ConstraintFirstArgument("end_idx.index_add[0].input", "end_idx.index_add[0].value")
                    && ConstraintSecondArgument("end_idx.index_add[0].addend", "end_idx.index_add[0].value"))
                 || (  ConstraintFirstArgument("end_idx.index_add[0].addend", "end_idx.index_add[0].value")
                    && ConstraintSecondArgument("end_idx.index_add[0].input", "end_idx.index_add[0].value")))))
        && (  (  ConstraintZExtInst("end_idx.index_add[0].input")
              && ConstraintDFGEdge("end_idx.input_index[0]", "end_idx.index_add[0].input"))
           || (  ConstraintSExtInst("end_idx.index_add[0].input")
              && ConstraintDFGEdge("end_idx.input_index[0]", "end_idx.index_add[0].input"))
           || ConstraintSame("end_idx.input_index[0]", "end_idx.index_add[0].input"))
        && (  (  ConstraintDistinct("end_idx.index_add[0].input", "end_idx.offset_add.value")
              && ConstraintIntegerType("end_idx.input_index[0]"))
           || (  ConstraintSame("end_idx.input_index[0]", "end_idx.index_add[0].input")
              && ConstraintIntegerType("end_idx.offset_add.value")))
        && ConstraintSame("end_idx.base_index", "end_idx.index_add[0].addend")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("end_idx.base_index"))
           || ConstraintCFGDominateStrict("end_idx.base_index", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("end_idx.base_index")))
        && ConstraintSame("iterator", "end_idx.input_index[0]")
        && (  (  ConstraintZExtInst("reduc.iter_begin")
              && ConstraintDFGEdge("begin_idx.value", "reduc.iter_begin"))
           || (  ConstraintSExtInst("reduc.iter_begin")
              && ConstraintDFGEdge("begin_idx.value", "reduc.iter_begin"))
           || ConstraintSame("begin_idx.value", "reduc.iter_begin"))
        && (  (  ConstraintZExtInst("reduc.iter_end")
              && ConstraintDFGEdge("end_idx.value", "reduc.iter_end"))
           || (  ConstraintSExtInst("reduc.iter_end")
              && ConstraintDFGEdge("end_idx.value", "reduc.iter_end"))
           || ConstraintSame("end_idx.value", "reduc.iter_end"))
        && ConstraintBranchInst("reduc.precursor")
        && ConstraintCFGEdge("reduc.precursor", "reduc.begin")
        && ConstraintBranchInst("reduc.end")
        && ConstraintCFGEdge("reduc.end", "reduc.successor")
        && ConstraintCFGDominate("reduc.begin", "reduc.end")
        && ConstraintCFGPostdom("reduc.end", "reduc.begin")
        && ConstraintCFGDominateStrict("reduc.precursor", "reduc.begin")
        && ConstraintCFGPostdomStrict("reduc.successor", "reduc.end")
        && ConstraintCFGBlocked("reduc.begin", "reduc.end", "reduc.precursor")
        && ConstraintCFGBlocked("reduc.successor", "reduc.begin", "reduc.end")
        && ConstraintSame("reduc.body_sese.precursor", "reduc.end")
        && ConstraintSame("reduc.body_sese.successor", "reduc.begin")
        && ConstraintBranchInst("reduc.body_sese.precursor")
        && ConstraintCFGEdge("reduc.body_sese.precursor", "reduc.body_sese.begin")
        && ConstraintBranchInst("reduc.body_sese.end")
        && ConstraintCFGEdge("reduc.body_sese.end", "reduc.body_sese.successor")
        && ConstraintCFGDominate("reduc.body_sese.begin", "reduc.body_sese.end")
        && ConstraintCFGPostdom("reduc.body_sese.end", "reduc.body_sese.begin")
        && ConstraintCFGDominateStrict("reduc.body_sese.precursor", "reduc.body_sese.begin")
        && ConstraintCFGPostdomStrict("reduc.body_sese.successor", "reduc.body_sese.end")
        && ConstraintCFGBlocked("reduc.body_sese.begin", "reduc.body_sese.end", "reduc.body_sese.precursor")
        && ConstraintCFGBlocked("reduc.body_sese.successor", "reduc.body_sese.begin", "reduc.body_sese.end")
        && ConstraintDistinct("reduc.body_sese.begin", "reduc.successor")
        && ConstraintIncomingValue("reduc.increment", "reduc.body_sese.end", "reduc.iterator")
        && ConstraintDFGEdge("reduc.iterator", "reduc.comparison")
        && ConstraintICmpInst("reduc.comparison")
        && ConstraintDFGEdge("reduc.comparison", "reduc.end")
        && ConstraintDFGEdge("reduc.increment", "reduc.iterator")
        && ConstraintAddInst("reduc.increment")
        && ConstraintDFGEdge("reduc.iterator", "reduc.increment")
        && ConstraintDFGEdge("reduc.iter_end", "reduc.comparison")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.iter_end"))
           || ConstraintCFGDominateStrict("reduc.iter_end", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.iter_end")))
        && ConstraintDFGEdge("reduc.iter_begin", "reduc.iterator")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.iter_begin"))
           || ConstraintCFGDominateStrict("reduc.iter_begin", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.iter_begin")))
        && ConstraintDFGEdge("reduc.iter_step", "reduc.increment")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.iter_step"))
           || ConstraintCFGDominateStrict("reduc.iter_step", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.iter_step")))
        && ConstraintLoadInst("reduc.idx_read.value")
        && ConstraintFirstArgument("reduc.idx_read.access_pointer", "reduc.idx_read.value")
        && ConstraintGEPInst("reduc.idx_read.access_pointer")
        && ConstraintFirstArgument("reduc.idx_read.base_pointer", "reduc.idx_read.access_pointer")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.idx_read.base_pointer"))
           || ConstraintCFGDominateStrict("reduc.idx_read.base_pointer", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.idx_read.base_pointer")))
        && ConstraintSecondArgument("reduc.idx_read.output_index", "reduc.idx_read.access_pointer")
        && (  (  ConstraintZExtInst("reduc.idx_read.output_index")
              && ConstraintDFGEdge("reduc.idx_read.offset_add.value", "reduc.idx_read.output_index"))
           || (  ConstraintSExtInst("reduc.idx_read.output_index")
              && ConstraintDFGEdge("reduc.idx_read.offset_add.value", "reduc.idx_read.output_index"))
           || ConstraintSame("reduc.idx_read.offset_add.value", "reduc.idx_read.output_index"))
        && (  (  ConstraintSame("reduc.idx_read.offset_add.input", "reduc.idx_read.offset_add.value")
              && ConstraintUnused("reduc.idx_read.offset_add.addend"))
           || (  (  ConstraintAddInst("reduc.idx_read.offset_add.value")
                 || ConstraintBitOrInst("reduc.idx_read.offset_add.value")
                 || ConstraintFAddInst("reduc.idx_read.offset_add.value"))
              && (  (  ConstraintFirstArgument("reduc.idx_read.offset_add.input", "reduc.idx_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.idx_read.offset_add.addend", "reduc.idx_read.offset_add.value"))
                 || (  ConstraintFirstArgument("reduc.idx_read.offset_add.addend", "reduc.idx_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.idx_read.offset_add.input", "reduc.idx_read.offset_add.value"))))
           || (  (  ConstraintSubInst("reduc.idx_read.offset_add.value")
                 || ConstraintFSubInst("reduc.idx_read.offset_add.value"))
              && (  (  ConstraintFirstArgument("reduc.idx_read.offset_add.input", "reduc.idx_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.idx_read.offset_add.addend", "reduc.idx_read.offset_add.value"))
                 || (  ConstraintFirstArgument("reduc.idx_read.offset_add.addend", "reduc.idx_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.idx_read.offset_add.input", "reduc.idx_read.offset_add.value")))))
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.idx_read.offset_add.addend"))
           || ConstraintCFGDominateStrict("reduc.idx_read.offset_add.addend", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.idx_read.offset_add.addend")))
        && ConstraintSame("reduc.idx_read.stride_mul[0].value", "reduc.idx_read.offset_add.input")
        && (  (  ConstraintSame("reduc.idx_read.stride_mul[0].input", "reduc.idx_read.stride_mul[0].value")
              && ConstraintUnused("reduc.idx_read.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("reduc.idx_read.stride_mul[0].value")
                 || ConstraintLShiftInst("reduc.idx_read.stride_mul[0].value")
                 || ConstraintFMulInst("reduc.idx_read.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("reduc.idx_read.stride_mul[0].input", "reduc.idx_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.stride_mul[0].multiplier", "reduc.idx_read.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("reduc.idx_read.stride_mul[0].multiplier", "reduc.idx_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.stride_mul[0].input", "reduc.idx_read.stride_mul[0].value"))))
           || (  ConstraintFDivInst("reduc.idx_read.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("reduc.idx_read.stride_mul[0].input", "reduc.idx_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.stride_mul[0].multiplier", "reduc.idx_read.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("reduc.idx_read.stride_mul[0].multiplier", "reduc.idx_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.stride_mul[0].input", "reduc.idx_read.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.idx_read.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("reduc.idx_read.stride_mul[0].multiplier", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.idx_read.stride_mul[0].multiplier")))
        && ConstraintSame("reduc.idx_read.index_add[0].value", "reduc.idx_read.stride_mul[0].input")
        && (  (  ConstraintSame("reduc.idx_read.index_add[0].input", "reduc.idx_read.index_add[0].value")
              && ConstraintUnused("reduc.idx_read.index_add[0].addend"))
           || (  (  ConstraintAddInst("reduc.idx_read.index_add[0].value")
                 || ConstraintBitOrInst("reduc.idx_read.index_add[0].value")
                 || ConstraintFAddInst("reduc.idx_read.index_add[0].value"))
              && (  (  ConstraintFirstArgument("reduc.idx_read.index_add[0].input", "reduc.idx_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.index_add[0].addend", "reduc.idx_read.index_add[0].value"))
                 || (  ConstraintFirstArgument("reduc.idx_read.index_add[0].addend", "reduc.idx_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.index_add[0].input", "reduc.idx_read.index_add[0].value"))))
           || (  (  ConstraintSubInst("reduc.idx_read.index_add[0].value")
                 || ConstraintFSubInst("reduc.idx_read.index_add[0].value"))
              && (  (  ConstraintFirstArgument("reduc.idx_read.index_add[0].input", "reduc.idx_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.index_add[0].addend", "reduc.idx_read.index_add[0].value"))
                 || (  ConstraintFirstArgument("reduc.idx_read.index_add[0].addend", "reduc.idx_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.idx_read.index_add[0].input", "reduc.idx_read.index_add[0].value")))))
        && (  (  ConstraintZExtInst("reduc.idx_read.index_add[0].input")
              && ConstraintDFGEdge("reduc.idx_read.input_index[0]", "reduc.idx_read.index_add[0].input"))
           || (  ConstraintSExtInst("reduc.idx_read.index_add[0].input")
              && ConstraintDFGEdge("reduc.idx_read.input_index[0]", "reduc.idx_read.index_add[0].input"))
           || ConstraintSame("reduc.idx_read.input_index[0]", "reduc.idx_read.index_add[0].input"))
        && (  (  ConstraintDistinct("reduc.idx_read.index_add[0].input", "reduc.idx_read.offset_add.value")
              && ConstraintIntegerType("reduc.idx_read.input_index[0]"))
           || (  ConstraintSame("reduc.idx_read.input_index[0]", "reduc.idx_read.index_add[0].input")
              && ConstraintIntegerType("reduc.idx_read.offset_add.value")))
        && ConstraintSame("reduc.idx_read.base_index", "reduc.idx_read.index_add[0].addend")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.idx_read.base_index"))
           || ConstraintCFGDominateStrict("reduc.idx_read.base_index", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.idx_read.base_index")))
        && ConstraintSame("reduc.iterator", "reduc.idx_read.input_index[0]")
        && ConstraintLoadInst("reduc.indir_read.value")
        && ConstraintFirstArgument("reduc.indir_read.access_pointer", "reduc.indir_read.value")
        && ConstraintGEPInst("reduc.indir_read.access_pointer")
        && ConstraintFirstArgument("reduc.indir_read.base_pointer", "reduc.indir_read.access_pointer")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.indir_read.base_pointer"))
           || ConstraintCFGDominateStrict("reduc.indir_read.base_pointer", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.indir_read.base_pointer")))
        && ConstraintSecondArgument("reduc.indir_read.output_index", "reduc.indir_read.access_pointer")
        && (  (  ConstraintZExtInst("reduc.indir_read.output_index")
              && ConstraintDFGEdge("reduc.indir_read.offset_add.value", "reduc.indir_read.output_index"))
           || (  ConstraintSExtInst("reduc.indir_read.output_index")
              && ConstraintDFGEdge("reduc.indir_read.offset_add.value", "reduc.indir_read.output_index"))
           || ConstraintSame("reduc.indir_read.offset_add.value", "reduc.indir_read.output_index"))
        && (  (  ConstraintSame("reduc.indir_read.offset_add.input", "reduc.indir_read.offset_add.value")
              && ConstraintUnused("reduc.indir_read.offset_add.addend"))
           || (  (  ConstraintAddInst("reduc.indir_read.offset_add.value")
                 || ConstraintBitOrInst("reduc.indir_read.offset_add.value")
                 || ConstraintFAddInst("reduc.indir_read.offset_add.value"))
              && (  (  ConstraintFirstArgument("reduc.indir_read.offset_add.input", "reduc.indir_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.indir_read.offset_add.addend", "reduc.indir_read.offset_add.value"))
                 || (  ConstraintFirstArgument("reduc.indir_read.offset_add.addend", "reduc.indir_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.indir_read.offset_add.input", "reduc.indir_read.offset_add.value"))))
           || (  (  ConstraintSubInst("reduc.indir_read.offset_add.value")
                 || ConstraintFSubInst("reduc.indir_read.offset_add.value"))
              && (  (  ConstraintFirstArgument("reduc.indir_read.offset_add.input", "reduc.indir_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.indir_read.offset_add.addend", "reduc.indir_read.offset_add.value"))
                 || (  ConstraintFirstArgument("reduc.indir_read.offset_add.addend", "reduc.indir_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.indir_read.offset_add.input", "reduc.indir_read.offset_add.value")))))
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.indir_read.offset_add.addend"))
           || ConstraintCFGDominateStrict("reduc.indir_read.offset_add.addend", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.indir_read.offset_add.addend")))
        && ConstraintSame("reduc.indir_read.stride_mul[0].value", "reduc.indir_read.offset_add.input")
        && (  (  ConstraintSame("reduc.indir_read.stride_mul[0].input", "reduc.indir_read.stride_mul[0].value")
              && ConstraintUnused("reduc.indir_read.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("reduc.indir_read.stride_mul[0].value")
                 || ConstraintLShiftInst("reduc.indir_read.stride_mul[0].value")
                 || ConstraintFMulInst("reduc.indir_read.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("reduc.indir_read.stride_mul[0].input", "reduc.indir_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.stride_mul[0].multiplier", "reduc.indir_read.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("reduc.indir_read.stride_mul[0].multiplier", "reduc.indir_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.stride_mul[0].input", "reduc.indir_read.stride_mul[0].value"))))
           || (  ConstraintFDivInst("reduc.indir_read.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("reduc.indir_read.stride_mul[0].input", "reduc.indir_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.stride_mul[0].multiplier", "reduc.indir_read.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("reduc.indir_read.stride_mul[0].multiplier", "reduc.indir_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.stride_mul[0].input", "reduc.indir_read.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.indir_read.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("reduc.indir_read.stride_mul[0].multiplier", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.indir_read.stride_mul[0].multiplier")))
        && ConstraintSame("reduc.indir_read.index_add[0].value", "reduc.indir_read.stride_mul[0].input")
        && (  (  ConstraintSame("reduc.indir_read.index_add[0].input", "reduc.indir_read.index_add[0].value")
              && ConstraintUnused("reduc.indir_read.index_add[0].addend"))
           || (  (  ConstraintAddInst("reduc.indir_read.index_add[0].value")
                 || ConstraintBitOrInst("reduc.indir_read.index_add[0].value")
                 || ConstraintFAddInst("reduc.indir_read.index_add[0].value"))
              && (  (  ConstraintFirstArgument("reduc.indir_read.index_add[0].input", "reduc.indir_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.index_add[0].addend", "reduc.indir_read.index_add[0].value"))
                 || (  ConstraintFirstArgument("reduc.indir_read.index_add[0].addend", "reduc.indir_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.index_add[0].input", "reduc.indir_read.index_add[0].value"))))
           || (  (  ConstraintSubInst("reduc.indir_read.index_add[0].value")
                 || ConstraintFSubInst("reduc.indir_read.index_add[0].value"))
              && (  (  ConstraintFirstArgument("reduc.indir_read.index_add[0].input", "reduc.indir_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.index_add[0].addend", "reduc.indir_read.index_add[0].value"))
                 || (  ConstraintFirstArgument("reduc.indir_read.index_add[0].addend", "reduc.indir_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.indir_read.index_add[0].input", "reduc.indir_read.index_add[0].value")))))
        && (  (  ConstraintZExtInst("reduc.indir_read.index_add[0].input")
              && ConstraintDFGEdge("reduc.indir_read.input_index[0]", "reduc.indir_read.index_add[0].input"))
           || (  ConstraintSExtInst("reduc.indir_read.index_add[0].input")
              && ConstraintDFGEdge("reduc.indir_read.input_index[0]", "reduc.indir_read.index_add[0].input"))
           || ConstraintSame("reduc.indir_read.input_index[0]", "reduc.indir_read.index_add[0].input"))
        && (  (  ConstraintDistinct("reduc.indir_read.index_add[0].input", "reduc.indir_read.offset_add.value")
              && ConstraintIntegerType("reduc.indir_read.input_index[0]"))
           || (  ConstraintSame("reduc.indir_read.input_index[0]", "reduc.indir_read.index_add[0].input")
              && ConstraintIntegerType("reduc.indir_read.offset_add.value")))
        && ConstraintSame("reduc.indir_read.base_index", "reduc.indir_read.index_add[0].addend")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.indir_read.base_index"))
           || ConstraintCFGDominateStrict("reduc.indir_read.base_index", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.indir_read.base_index")))
        && ConstraintSame("reduc.idx_read.value", "reduc.indir_read.input_index[0]")
        && ConstraintLoadInst("reduc.seq_read.value")
        && ConstraintFirstArgument("reduc.seq_read.access_pointer", "reduc.seq_read.value")
        && ConstraintGEPInst("reduc.seq_read.access_pointer")
        && ConstraintFirstArgument("reduc.seq_read.base_pointer", "reduc.seq_read.access_pointer")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.seq_read.base_pointer"))
           || ConstraintCFGDominateStrict("reduc.seq_read.base_pointer", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.seq_read.base_pointer")))
        && ConstraintSecondArgument("reduc.seq_read.output_index", "reduc.seq_read.access_pointer")
        && (  (  ConstraintZExtInst("reduc.seq_read.output_index")
              && ConstraintDFGEdge("reduc.seq_read.offset_add.value", "reduc.seq_read.output_index"))
           || (  ConstraintSExtInst("reduc.seq_read.output_index")
              && ConstraintDFGEdge("reduc.seq_read.offset_add.value", "reduc.seq_read.output_index"))
           || ConstraintSame("reduc.seq_read.offset_add.value", "reduc.seq_read.output_index"))
        && (  (  ConstraintSame("reduc.seq_read.offset_add.input", "reduc.seq_read.offset_add.value")
              && ConstraintUnused("reduc.seq_read.offset_add.addend"))
           || (  (  ConstraintAddInst("reduc.seq_read.offset_add.value")
                 || ConstraintBitOrInst("reduc.seq_read.offset_add.value")
                 || ConstraintFAddInst("reduc.seq_read.offset_add.value"))
              && (  (  ConstraintFirstArgument("reduc.seq_read.offset_add.input", "reduc.seq_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.seq_read.offset_add.addend", "reduc.seq_read.offset_add.value"))
                 || (  ConstraintFirstArgument("reduc.seq_read.offset_add.addend", "reduc.seq_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.seq_read.offset_add.input", "reduc.seq_read.offset_add.value"))))
           || (  (  ConstraintSubInst("reduc.seq_read.offset_add.value")
                 || ConstraintFSubInst("reduc.seq_read.offset_add.value"))
              && (  (  ConstraintFirstArgument("reduc.seq_read.offset_add.input", "reduc.seq_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.seq_read.offset_add.addend", "reduc.seq_read.offset_add.value"))
                 || (  ConstraintFirstArgument("reduc.seq_read.offset_add.addend", "reduc.seq_read.offset_add.value")
                    && ConstraintSecondArgument("reduc.seq_read.offset_add.input", "reduc.seq_read.offset_add.value")))))
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.seq_read.offset_add.addend"))
           || ConstraintCFGDominateStrict("reduc.seq_read.offset_add.addend", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.seq_read.offset_add.addend")))
        && ConstraintSame("reduc.seq_read.stride_mul[0].value", "reduc.seq_read.offset_add.input")
        && (  (  ConstraintSame("reduc.seq_read.stride_mul[0].input", "reduc.seq_read.stride_mul[0].value")
              && ConstraintUnused("reduc.seq_read.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("reduc.seq_read.stride_mul[0].value")
                 || ConstraintLShiftInst("reduc.seq_read.stride_mul[0].value")
                 || ConstraintFMulInst("reduc.seq_read.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("reduc.seq_read.stride_mul[0].input", "reduc.seq_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.stride_mul[0].multiplier", "reduc.seq_read.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("reduc.seq_read.stride_mul[0].multiplier", "reduc.seq_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.stride_mul[0].input", "reduc.seq_read.stride_mul[0].value"))))
           || (  ConstraintFDivInst("reduc.seq_read.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("reduc.seq_read.stride_mul[0].input", "reduc.seq_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.stride_mul[0].multiplier", "reduc.seq_read.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("reduc.seq_read.stride_mul[0].multiplier", "reduc.seq_read.stride_mul[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.stride_mul[0].input", "reduc.seq_read.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.seq_read.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("reduc.seq_read.stride_mul[0].multiplier", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.seq_read.stride_mul[0].multiplier")))
        && ConstraintSame("reduc.seq_read.index_add[0].value", "reduc.seq_read.stride_mul[0].input")
        && (  (  ConstraintSame("reduc.seq_read.index_add[0].input", "reduc.seq_read.index_add[0].value")
              && ConstraintUnused("reduc.seq_read.index_add[0].addend"))
           || (  (  ConstraintAddInst("reduc.seq_read.index_add[0].value")
                 || ConstraintBitOrInst("reduc.seq_read.index_add[0].value")
                 || ConstraintFAddInst("reduc.seq_read.index_add[0].value"))
              && (  (  ConstraintFirstArgument("reduc.seq_read.index_add[0].input", "reduc.seq_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.index_add[0].addend", "reduc.seq_read.index_add[0].value"))
                 || (  ConstraintFirstArgument("reduc.seq_read.index_add[0].addend", "reduc.seq_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.index_add[0].input", "reduc.seq_read.index_add[0].value"))))
           || (  (  ConstraintSubInst("reduc.seq_read.index_add[0].value")
                 || ConstraintFSubInst("reduc.seq_read.index_add[0].value"))
              && (  (  ConstraintFirstArgument("reduc.seq_read.index_add[0].input", "reduc.seq_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.index_add[0].addend", "reduc.seq_read.index_add[0].value"))
                 || (  ConstraintFirstArgument("reduc.seq_read.index_add[0].addend", "reduc.seq_read.index_add[0].value")
                    && ConstraintSecondArgument("reduc.seq_read.index_add[0].input", "reduc.seq_read.index_add[0].value")))))
        && (  (  ConstraintZExtInst("reduc.seq_read.index_add[0].input")
              && ConstraintDFGEdge("reduc.seq_read.input_index[0]", "reduc.seq_read.index_add[0].input"))
           || (  ConstraintSExtInst("reduc.seq_read.index_add[0].input")
              && ConstraintDFGEdge("reduc.seq_read.input_index[0]", "reduc.seq_read.index_add[0].input"))
           || ConstraintSame("reduc.seq_read.input_index[0]", "reduc.seq_read.index_add[0].input"))
        && (  (  ConstraintDistinct("reduc.seq_read.index_add[0].input", "reduc.seq_read.offset_add.value")
              && ConstraintIntegerType("reduc.seq_read.input_index[0]"))
           || (  ConstraintSame("reduc.seq_read.input_index[0]", "reduc.seq_read.index_add[0].input")
              && ConstraintIntegerType("reduc.seq_read.offset_add.value")))
        && ConstraintSame("reduc.seq_read.base_index", "reduc.seq_read.index_add[0].addend")
        && (  (  ConstraintInstruction("reduc.begin")
              && ConstraintPreexecution("reduc.seq_read.base_index"))
           || ConstraintCFGDominateStrict("reduc.seq_read.base_index", "reduc.begin")
           || (  ConstraintInstruction("reduc.begin")
              && ConstraintUnused("reduc.seq_read.base_index")))
        && ConstraintSame("reduc.iterator", "reduc.seq_read.input_index[0]")
        && ConstraintDistinct("reduc.idx_read.value", "reduc.seq_read.value")
        && ConstraintSame("reduc.update_expr.input[0]", "reduc.old_value")
        && ConstraintSame("reduc.update_expr.input[1]", "reduc.seq_read.value")
        && ConstraintSame("reduc.update_expr.input[2]", "reduc.indir_read.value")
        && ConstraintUnused("reduc.update_expr.input[3]")
        && ConstraintUnused("reduc.update_expr.input[4]")
        && ConstraintUnused("reduc.update_expr.input[5]")
        && ConstraintUnused("reduc.update_expr.input[6]")
        && ConstraintUnused("reduc.update_expr.input[7]")
        && ConstraintUnused("reduc.update_expr.input[8]")
        && ConstraintUnused("reduc.update_expr.input[9]")
        && ConstraintUnused("reduc.update_expr.input[10]")
        && ConstraintUnused("reduc.update_expr.input[11]")
        && ConstraintUnused("reduc.update_expr.input[12]")
        && ConstraintUnused("reduc.update_expr.input[13]")
        && ConstraintUnused("reduc.update_expr.input[14]")
        && ConstraintUnused("reduc.update_expr.input[15]")
        && ConstraintUnused("reduc.update_expr.input[16]")
        && ConstraintUnused("reduc.update_expr.input[17]")
        && ConstraintUnused("reduc.update_expr.input[18]")
        && ConstraintUnused("reduc.update_expr.input[19]")
        && ConstraintUnused("reduc.update_expr.input[20]")
        && ConstraintUnused("reduc.update_expr.input[21]")
        && ConstraintUnused("reduc.update_expr.input[22]")
        && ConstraintUnused("reduc.update_expr.input[23]")
        && ConstraintUnused("reduc.update_expr.input[24]")
        && ConstraintUnused("reduc.update_expr.input[25]")
        && ConstraintUnused("reduc.update_expr.input[26]")
        && ConstraintUnused("reduc.update_expr.input[27]")
        && ConstraintUnused("reduc.update_expr.input[28]")
        && ConstraintUnused("reduc.update_expr.input[29]")
        && ConstraintUnused("reduc.update_expr.input[30]")
        && ConstraintUnused("reduc.update_expr.input[31]")
        && ConstraintIncomingValue("reduc.update_expr.output", "reduc.body_sese.end", "reduc.old_value")
        && ConstraintDistinct("reduc.old_value", "reduc.iterator")
        && std::make_pair("i",4) * ConstraintCFGEdge("reduc.update_expr.origin[i]", "reduc.body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("reduc.begin")
                                          && ConstraintPreexecution("reduc.update_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("reduc.update_expr.constants[i].value", "reduc.begin")
                                       || (  ConstraintInstruction("reduc.begin")
                                          && ConstraintUnused("reduc.update_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("reduc.update_expr.constants[i].value")
                                    && ConstraintDFGEdge("reduc.update_expr.constants[i].value", "reduc.update_expr.constants[i].use")
                                    && ConstraintCFGDominate("reduc.body_sese.begin", "reduc.update_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("reduc.begin", "reduc.update_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("reduc.update_expr.restrictions[i].value", "reduc.body_sese.begin")
                                    && ConstraintDFGEdge("reduc.update_expr.restrictions[i].value", "reduc.update_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("reduc.body_sese.begin", "reduc.update_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"reduc.update_expr.restrictions[0].value",
                                                           "reduc.update_expr.restrictions[1].value",
                                                           "reduc.update_expr.restrictions[2].value",
                                                           "reduc.update_expr.restrictions[3].value",
                                                           "reduc.update_expr.restrictions[4].value",
                                                           "reduc.update_expr.restrictions[5].value",
                                                           "reduc.update_expr.restrictions[6].value",
                                                           "reduc.update_expr.restrictions[7].value",
                                                           "reduc.update_expr.restrictions[8].value",
                                                           "reduc.update_expr.restrictions[9].value",
                                                           "reduc.update_expr.restrictions[10].value",
                                                           "reduc.update_expr.restrictions[11].value",
                                                           "reduc.update_expr.restrictions[12].value",
                                                           "reduc.update_expr.restrictions[13].value",
                                                           "reduc.update_expr.restrictions[14].value",
                                                           "reduc.update_expr.restrictions[15].value",
                                                           "reduc.update_expr.restrictions[16].value",
                                                           "reduc.update_expr.restrictions[17].value",
                                                           "reduc.update_expr.restrictions[18].value",
                                                           "reduc.update_expr.restrictions[19].value",
                                                           "reduc.update_expr.restrictions[20].value",
                                                           "reduc.update_expr.restrictions[21].value",
                                                           "reduc.update_expr.restrictions[22].value",
                                                           "reduc.update_expr.restrictions[23].value",
                                                           "reduc.update_expr.restrictions[24].value",
                                                           "reduc.update_expr.restrictions[25].value",
                                                           "reduc.update_expr.restrictions[26].value",
                                                           "reduc.update_expr.restrictions[27].value",
                                                           "reduc.update_expr.restrictions[28].value",
                                                           "reduc.update_expr.restrictions[29].value",
                                                           "reduc.update_expr.restrictions[30].value",
                                                           "reduc.update_expr.restrictions[31].value",
                                                           "reduc.update_expr.restrictions[32].value",
                                                           "reduc.update_expr.restrictions[33].value",
                                                           "reduc.update_expr.restrictions[34].value",
                                                           "reduc.update_expr.restrictions[35].value",
                                                           "reduc.update_expr.restrictions[36].value",
                                                           "reduc.update_expr.restrictions[37].value",
                                                           "reduc.update_expr.restrictions[38].value",
                                                           "reduc.update_expr.restrictions[39].value",
                                                           "reduc.update_expr.restrictions[40].value",
                                                           "reduc.update_expr.restrictions[41].value",
                                                           "reduc.update_expr.restrictions[42].value",
                                                           "reduc.update_expr.restrictions[43].value",
                                                           "reduc.update_expr.restrictions[44].value",
                                                           "reduc.update_expr.restrictions[45].value",
                                                           "reduc.update_expr.restrictions[46].value",
                                                           "reduc.update_expr.restrictions[47].value",
                                                           "reduc.update_expr.restrictions[48].value",
                                                           "reduc.update_expr.restrictions[49].value",
                                                           "reduc.update_expr.restrictions[50].value",
                                                           "reduc.update_expr.restrictions[51].value",
                                                           "reduc.update_expr.restrictions[52].value",
                                                           "reduc.update_expr.restrictions[53].value",
                                                           "reduc.update_expr.restrictions[54].value",
                                                           "reduc.update_expr.restrictions[55].value",
                                                           "reduc.update_expr.restrictions[56].value",
                                                           "reduc.update_expr.restrictions[57].value",
                                                           "reduc.update_expr.restrictions[58].value",
                                                           "reduc.update_expr.restrictions[59].value",
                                                           "reduc.update_expr.restrictions[60].value",
                                                           "reduc.update_expr.restrictions[61].value",
                                                           "reduc.update_expr.restrictions[62].value",
                                                           "reduc.update_expr.restrictions[63].value"},
                                  std::vector<std::string>{"reduc.update_expr.origin[0]", "reduc.update_expr.origin[1]",
                                                           "reduc.update_expr.origin[2]", "reduc.update_expr.origin[3]",
                                                           "reduc.update_expr.constants[0].value", "reduc.update_expr.constants[1].value",
                                                           "reduc.update_expr.constants[2].value", "reduc.update_expr.constants[3].value",
                                                           "reduc.update_expr.constants[4].value", "reduc.update_expr.constants[5].value",
                                                           "reduc.update_expr.constants[6].value", "reduc.update_expr.constants[7].value",
                                                           "reduc.update_expr.constants[8].value", "reduc.update_expr.constants[9].value",
                                                           "reduc.update_expr.constants[10].value", "reduc.update_expr.constants[11].value",
                                                           "reduc.update_expr.constants[12].value", "reduc.update_expr.constants[13].value",
                                                           "reduc.update_expr.constants[14].value", "reduc.update_expr.constants[15].value",
                                                           "reduc.update_expr.constants[16].value", "reduc.update_expr.constants[17].value",
                                                           "reduc.update_expr.constants[18].value", "reduc.update_expr.constants[19].value",
                                                           "reduc.update_expr.constants[20].value", "reduc.update_expr.constants[21].value",
                                                           "reduc.update_expr.constants[22].value", "reduc.update_expr.constants[23].value",
                                                           "reduc.update_expr.constants[24].value", "reduc.update_expr.constants[25].value",
                                                           "reduc.update_expr.constants[26].value", "reduc.update_expr.constants[27].value",
                                                           "reduc.update_expr.constants[28].value", "reduc.update_expr.constants[29].value",
                                                           "reduc.update_expr.constants[30].value", "reduc.update_expr.constants[31].value",
                                                           "reduc.update_expr.constants[32].value", "reduc.update_expr.constants[33].value",
                                                           "reduc.update_expr.constants[34].value", "reduc.update_expr.constants[35].value",
                                                           "reduc.update_expr.constants[36].value", "reduc.update_expr.constants[37].value",
                                                           "reduc.update_expr.constants[38].value", "reduc.update_expr.constants[39].value",
                                                           "reduc.update_expr.constants[40].value", "reduc.update_expr.constants[41].value",
                                                           "reduc.update_expr.constants[42].value", "reduc.update_expr.constants[43].value",
                                                           "reduc.update_expr.constants[44].value", "reduc.update_expr.constants[45].value",
                                                           "reduc.update_expr.constants[46].value", "reduc.update_expr.constants[47].value",
                                                           "reduc.update_expr.constants[48].value", "reduc.update_expr.constants[49].value",
                                                           "reduc.update_expr.constants[50].value", "reduc.update_expr.constants[51].value",
                                                           "reduc.update_expr.constants[52].value", "reduc.update_expr.constants[53].value",
                                                           "reduc.update_expr.constants[54].value", "reduc.update_expr.constants[55].value",
                                                           "reduc.update_expr.constants[56].value", "reduc.update_expr.constants[57].value",
                                                           "reduc.update_expr.constants[58].value", "reduc.update_expr.constants[59].value",
                                                           "reduc.update_expr.constants[60].value", "reduc.update_expr.constants[61].value",
                                                           "reduc.update_expr.constants[62].value", "reduc.update_expr.constants[63].value",
                                                           "reduc.update_expr.input[0]", "reduc.update_expr.input[1]",
                                                           "reduc.update_expr.input[2]", "reduc.update_expr.input[3]",
                                                           "reduc.update_expr.input[4]", "reduc.update_expr.input[5]",
                                                           "reduc.update_expr.input[6]", "reduc.update_expr.input[7]",
                                                           "reduc.update_expr.input[8]", "reduc.update_expr.input[9]",
                                                           "reduc.update_expr.input[10]", "reduc.update_expr.input[11]",
                                                           "reduc.update_expr.input[12]", "reduc.update_expr.input[13]",
                                                           "reduc.update_expr.input[14]", "reduc.update_expr.input[15]",
                                                           "reduc.update_expr.input[16]", "reduc.update_expr.input[17]",
                                                           "reduc.update_expr.input[18]", "reduc.update_expr.input[19]",
                                                           "reduc.update_expr.input[20]", "reduc.update_expr.input[21]",
                                                           "reduc.update_expr.input[22]", "reduc.update_expr.input[23]",
                                                           "reduc.update_expr.input[24]", "reduc.update_expr.input[25]",
                                                           "reduc.update_expr.input[26]", "reduc.update_expr.input[27]",
                                                           "reduc.update_expr.input[28]", "reduc.update_expr.input[29]",
                                                           "reduc.update_expr.input[30]", "reduc.update_expr.input[31]"},
                                  std::vector<std::string>{"reduc.update_expr.output"}})
        && std::make_pair("i",4) * (  ConstraintDFGEdge("reduc.incoming[i]", "reduc.old_value")
                                   && ConstraintDistinct("reduc.incoming[i]", "reduc.update_expr.output"));
}

ConstraintAnd ConstraintHistogram()
{
    return ConstraintBranchInst("precursor")
        && ConstraintCFGEdge("precursor", "begin")
        && ConstraintBranchInst("end")
        && ConstraintCFGEdge("end", "successor")
        && ConstraintCFGDominate("begin", "end")
        && ConstraintCFGPostdom("end", "begin")
        && ConstraintCFGDominateStrict("precursor", "begin")
        && ConstraintCFGPostdomStrict("successor", "end")
        && ConstraintCFGBlocked("begin", "end", "precursor")
        && ConstraintCFGBlocked("successor", "begin", "end")
        && ConstraintSame("body_sese.precursor", "end")
        && ConstraintSame("body_sese.successor", "begin")
        && ConstraintBranchInst("body_sese.precursor")
        && ConstraintCFGEdge("body_sese.precursor", "body_sese.begin")
        && ConstraintBranchInst("body_sese.end")
        && ConstraintCFGEdge("body_sese.end", "body_sese.successor")
        && ConstraintCFGDominate("body_sese.begin", "body_sese.end")
        && ConstraintCFGPostdom("body_sese.end", "body_sese.begin")
        && ConstraintCFGDominateStrict("body_sese.precursor", "body_sese.begin")
        && ConstraintCFGPostdomStrict("body_sese.successor", "body_sese.end")
        && ConstraintCFGBlocked("body_sese.begin", "body_sese.end", "body_sese.precursor")
        && ConstraintCFGBlocked("body_sese.successor", "body_sese.begin", "body_sese.end")
        && ConstraintDistinct("body_sese.begin", "successor")
        && ConstraintIncomingValue("increment", "body_sese.end", "iterator")
        && ConstraintDFGEdge("iterator", "comparison")
        && ConstraintICmpInst("comparison")
        && ConstraintDFGEdge("comparison", "end")
        && ConstraintDFGEdge("increment", "iterator")
        && ConstraintAddInst("increment")
        && ConstraintDFGEdge("iterator", "increment")
        && ConstraintDFGEdge("iter_end", "comparison")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_end"))
           || ConstraintCFGDominateStrict("iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_end")))
        && ConstraintDFGEdge("iter_begin", "iterator")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_begin"))
           || ConstraintCFGDominateStrict("iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_begin")))
        && ConstraintDFGEdge("iter_step", "increment")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("iter_step"))
           || ConstraintCFGDominateStrict("iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("iter_step")))
        && ConstraintStoreInst("store_instr")
        && ConstraintCFGDominate("body_sese.begin", "store_instr")
        && ConstraintCFGPostdom("body_sese.end", "store_instr")
        && ConstraintCFGEdge("store_instr", "store_instr_after")
        && ConstraintCFGBlocked("store_instr_after", "body_sese.successor", "store_instr")
        && ConstraintCFGBlocked("store_instr_after", "body_sese.precursor", "store_instr")
        && ConstraintSecondArgument("index_expr.output", "store_instr")
        && ConstraintGEPInst("index_expr.output")
        && ConstraintFirstArgument("index_expr.output", "old_value")
        && ConstraintLoadInst("old_value")
        && ConstraintFirstArgument("update_expr.output", "store_instr")
        && ConstraintFirstArgument("reduction_array", "index_expr.output")
        && std::make_pair("i",30) * (  ConstraintLoadInst("read[i].value")
                                    && ConstraintFirstArgument("read[i].access_pointer", "read[i].value")
                                    && ConstraintGEPInst("read[i].access_pointer")
                                    && ConstraintFirstArgument("read[i].base_pointer", "read[i].access_pointer")
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].base_pointer"))
                                       || ConstraintCFGDominateStrict("read[i].base_pointer", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].base_pointer")))
                                    && ConstraintSecondArgument("read[i].output_index", "read[i].access_pointer")
                                    && (  (  ConstraintZExtInst("read[i].output_index")
                                          && ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index"))
                                       || (  ConstraintSExtInst("read[i].output_index")
                                          && ConstraintDFGEdge("read[i].offset_add.value", "read[i].output_index"))
                                       || ConstraintSame("read[i].offset_add.value", "read[i].output_index"))
                                    && (  (  ConstraintSame("read[i].offset_add.input", "read[i].offset_add.value")
                                          && ConstraintUnused("read[i].offset_add.addend"))
                                       || (  (  ConstraintAddInst("read[i].offset_add.value")
                                             || ConstraintBitOrInst("read[i].offset_add.value")
                                             || ConstraintFAddInst("read[i].offset_add.value"))
                                          && (  (  ConstraintFirstArgument("read[i].offset_add.input", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.addend", "read[i].offset_add.value"))
                                             || (  ConstraintFirstArgument("read[i].offset_add.addend", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.input", "read[i].offset_add.value"))))
                                       || (  (  ConstraintSubInst("read[i].offset_add.value")
                                             || ConstraintFSubInst("read[i].offset_add.value"))
                                          && (  (  ConstraintFirstArgument("read[i].offset_add.input", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.addend", "read[i].offset_add.value"))
                                             || (  ConstraintFirstArgument("read[i].offset_add.addend", "read[i].offset_add.value")
                                                && ConstraintSecondArgument("read[i].offset_add.input", "read[i].offset_add.value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].offset_add.addend"))
                                       || ConstraintCFGDominateStrict("read[i].offset_add.addend", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].offset_add.addend")))
                                    && ConstraintSame("read[i].stride_mul[0].value", "read[i].offset_add.input")
                                    && (  (  ConstraintSame("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")
                                          && ConstraintUnused("read[i].stride_mul[0].multiplier"))
                                       || (  (  ConstraintMulInst("read[i].stride_mul[0].value")
                                             || ConstraintLShiftInst("read[i].stride_mul[0].value")
                                             || ConstraintFMulInst("read[i].stride_mul[0].value"))
                                          && (  (  ConstraintFirstArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"))
                                             || (  ConstraintFirstArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value"))))
                                       || (  ConstraintFDivInst("read[i].stride_mul[0].value")
                                          && (  (  ConstraintFirstArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value"))
                                             || (  ConstraintFirstArgument("read[i].stride_mul[0].multiplier", "read[i].stride_mul[0].value")
                                                && ConstraintSecondArgument("read[i].stride_mul[0].input", "read[i].stride_mul[0].value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].stride_mul[0].multiplier"))
                                       || ConstraintCFGDominateStrict("read[i].stride_mul[0].multiplier", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].stride_mul[0].multiplier")))
                                    && ConstraintSame("read[i].index_add[0].value", "read[i].stride_mul[0].input")
                                    && (  (  ConstraintSame("read[i].index_add[0].input", "read[i].index_add[0].value")
                                          && ConstraintUnused("read[i].index_add[0].addend"))
                                       || (  (  ConstraintAddInst("read[i].index_add[0].value")
                                             || ConstraintBitOrInst("read[i].index_add[0].value")
                                             || ConstraintFAddInst("read[i].index_add[0].value"))
                                          && (  (  ConstraintFirstArgument("read[i].index_add[0].input", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].addend", "read[i].index_add[0].value"))
                                             || (  ConstraintFirstArgument("read[i].index_add[0].addend", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].input", "read[i].index_add[0].value"))))
                                       || (  (  ConstraintSubInst("read[i].index_add[0].value")
                                             || ConstraintFSubInst("read[i].index_add[0].value"))
                                          && (  (  ConstraintFirstArgument("read[i].index_add[0].input", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].addend", "read[i].index_add[0].value"))
                                             || (  ConstraintFirstArgument("read[i].index_add[0].addend", "read[i].index_add[0].value")
                                                && ConstraintSecondArgument("read[i].index_add[0].input", "read[i].index_add[0].value")))))
                                    && (  (  ConstraintZExtInst("read[i].index_add[0].input")
                                          && ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input"))
                                       || (  ConstraintSExtInst("read[i].index_add[0].input")
                                          && ConstraintDFGEdge("read[i].input_index[0]", "read[i].index_add[0].input"))
                                       || ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input"))
                                    && (  (  ConstraintDistinct("read[i].index_add[0].input", "read[i].offset_add.value")
                                          && ConstraintIntegerType("read[i].input_index[0]"))
                                       || (  ConstraintSame("read[i].input_index[0]", "read[i].index_add[0].input")
                                          && ConstraintIntegerType("read[i].offset_add.value")))
                                    && ConstraintSame("read[i].base_index", "read[i].index_add[0].addend")
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("read[i].base_index"))
                                       || ConstraintCFGDominateStrict("read[i].base_index", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("read[i].base_index")))
                                    && ConstraintSame("iterator", "read[i].input_index[0]")
                                    && ConstraintCFGDominate("body_sese.begin", "read[i].value")
                                    && ConstraintCFGPostdom("body_sese.end", "read[i].value")
                                    && ConstraintDistinct("read[i].base_pointer", "reduction_array"))
        && ConstraintLoadInst("read[0].value")
        && std::make_pair("i",2) * (  ConstraintLoadInst("closure[i].value")
                                   && ConstraintFirstArgument("closure[i].access_pointer", "closure[i].value")
                                   && ConstraintGEPInst("closure[i].access_pointer")
                                   && ConstraintFirstArgument("closure[i].base_pointer", "closure[i].access_pointer")
                                   && (  (  ConstraintInstruction("begin")
                                         && ConstraintPreexecution("closure[i].base_pointer"))
                                      || ConstraintCFGDominateStrict("closure[i].base_pointer", "begin")
                                      || (  ConstraintInstruction("begin")
                                         && ConstraintUnused("closure[i].base_pointer")))
                                   && ConstraintSecondArgument("closure[i].output_index", "closure[i].access_pointer")
                                   && ConstraintCFGDominate("body_sese.begin", "closure[i].value")
                                   && ConstraintCFGPostdom("body_sese.end", "closure[i].value")
                                   && ConstraintDistinct("closure[i].base_pointer", "read[0].base_pointer")
                                   && ConstraintDistinct("closure[i].base_pointer", "reduction_array"))
        && ConstraintSame("update_expr.input[0]", "read[0].value")
        && ConstraintSame("update_expr.input[1]", "read[1].value")
        && ConstraintSame("update_expr.input[2]", "read[2].value")
        && ConstraintSame("update_expr.input[3]", "read[3].value")
        && ConstraintSame("update_expr.input[4]", "read[4].value")
        && ConstraintSame("update_expr.input[5]", "read[5].value")
        && ConstraintSame("update_expr.input[6]", "read[6].value")
        && ConstraintSame("update_expr.input[7]", "read[7].value")
        && ConstraintSame("update_expr.input[8]", "read[8].value")
        && ConstraintSame("update_expr.input[9]", "read[9].value")
        && ConstraintSame("update_expr.input[10]", "read[10].value")
        && ConstraintSame("update_expr.input[11]", "read[11].value")
        && ConstraintSame("update_expr.input[12]", "read[12].value")
        && ConstraintSame("update_expr.input[13]", "read[13].value")
        && ConstraintSame("update_expr.input[14]", "read[14].value")
        && ConstraintSame("update_expr.input[15]", "read[15].value")
        && ConstraintSame("update_expr.input[16]", "read[16].value")
        && ConstraintSame("update_expr.input[17]", "read[17].value")
        && ConstraintSame("update_expr.input[18]", "read[18].value")
        && ConstraintSame("update_expr.input[19]", "read[19].value")
        && ConstraintSame("update_expr.input[20]", "read[20].value")
        && ConstraintSame("update_expr.input[21]", "read[21].value")
        && ConstraintSame("update_expr.input[22]", "read[22].value")
        && ConstraintSame("update_expr.input[23]", "read[23].value")
        && ConstraintSame("update_expr.input[24]", "read[24].value")
        && ConstraintSame("update_expr.input[25]", "read[25].value")
        && ConstraintSame("update_expr.input[26]", "read[26].value")
        && ConstraintSame("update_expr.input[27]", "read[27].value")
        && ConstraintSame("update_expr.input[28]", "read[28].value")
        && ConstraintSame("update_expr.input[29]", "closure[0].value")
        && ConstraintSame("update_expr.input[30]", "closure[1].value")
        && ConstraintSame("update_expr.input[31]", "old_value")
        && std::make_pair("i",4) * ConstraintCFGEdge("update_expr.origin[i]", "body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("update_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("update_expr.constants[i].value", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("update_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("update_expr.constants[i].value")
                                    && ConstraintDFGEdge("update_expr.constants[i].value", "update_expr.constants[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "update_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("begin", "update_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("update_expr.restrictions[i].value", "body_sese.begin")
                                    && ConstraintDFGEdge("update_expr.restrictions[i].value", "update_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "update_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"update_expr.restrictions[0].value", "update_expr.restrictions[1].value",
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
                                  std::vector<std::string>{"update_expr.origin[0]", "update_expr.origin[1]", "update_expr.origin[2]",
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
                                  std::vector<std::string>{"update_expr.output"}})
        && ConstraintSame("index_expr.input[0]", "read[0].value")
        && ConstraintSame("index_expr.input[1]", "read[1].value")
        && ConstraintSame("index_expr.input[2]", "read[2].value")
        && ConstraintSame("index_expr.input[3]", "read[3].value")
        && ConstraintSame("index_expr.input[4]", "read[4].value")
        && ConstraintSame("index_expr.input[5]", "read[5].value")
        && ConstraintSame("index_expr.input[6]", "read[6].value")
        && ConstraintSame("index_expr.input[7]", "read[7].value")
        && ConstraintSame("index_expr.input[8]", "read[8].value")
        && ConstraintSame("index_expr.input[9]", "read[9].value")
        && ConstraintSame("index_expr.input[10]", "read[10].value")
        && ConstraintSame("index_expr.input[11]", "read[11].value")
        && ConstraintSame("index_expr.input[12]", "read[12].value")
        && ConstraintSame("index_expr.input[13]", "read[13].value")
        && ConstraintSame("index_expr.input[14]", "read[14].value")
        && ConstraintSame("index_expr.input[15]", "read[15].value")
        && ConstraintSame("index_expr.input[16]", "read[16].value")
        && ConstraintSame("index_expr.input[17]", "read[17].value")
        && ConstraintSame("index_expr.input[18]", "read[18].value")
        && ConstraintSame("index_expr.input[19]", "read[19].value")
        && ConstraintSame("index_expr.input[20]", "read[20].value")
        && ConstraintSame("index_expr.input[21]", "read[21].value")
        && ConstraintSame("index_expr.input[22]", "read[22].value")
        && ConstraintSame("index_expr.input[23]", "read[23].value")
        && ConstraintSame("index_expr.input[24]", "read[24].value")
        && ConstraintSame("index_expr.input[25]", "read[25].value")
        && ConstraintSame("index_expr.input[26]", "read[26].value")
        && ConstraintSame("index_expr.input[27]", "read[27].value")
        && ConstraintSame("index_expr.input[28]", "read[28].value")
        && ConstraintSame("index_expr.input[29]", "closure[0].value")
        && ConstraintSame("index_expr.input[30]", "closure[1].value")
        && ConstraintUnused("index_expr.input[31]")
        && std::make_pair("i",4) * ConstraintCFGEdge("index_expr.origin[i]", "body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("index_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("index_expr.constants[i].value", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("index_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("index_expr.constants[i].value")
                                    && ConstraintDFGEdge("index_expr.constants[i].value", "index_expr.constants[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "index_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("begin", "index_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("index_expr.restrictions[i].value", "body_sese.begin")
                                    && ConstraintDFGEdge("index_expr.restrictions[i].value", "index_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "index_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"index_expr.restrictions[0].value", "index_expr.restrictions[1].value",
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
                                  std::vector<std::string>{"index_expr.origin[0]", "index_expr.origin[1]", "index_expr.origin[2]",
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
                                  std::vector<std::string>{"index_expr.output"}});
}

ConstraintAnd ConstraintStencil(unsigned dim)
{
    return ConstraintBranchInst("loop[0].precursor")
        && ConstraintCFGEdge("loop[0].precursor", "loop[0].begin")
        && ConstraintBranchInst("loop[0].end")
        && ConstraintCFGEdge("loop[0].end", "loop[0].successor")
        && ConstraintCFGDominate("loop[0].begin", "loop[0].end")
        && ConstraintCFGPostdom("loop[0].end", "loop[0].begin")
        && ConstraintCFGDominateStrict("loop[0].precursor", "loop[0].begin")
        && ConstraintCFGPostdomStrict("loop[0].successor", "loop[0].end")
        && ConstraintCFGBlocked("loop[0].begin", "loop[0].end", "loop[0].precursor")
        && ConstraintCFGBlocked("loop[0].successor", "loop[0].begin", "loop[0].end")
        && ConstraintSame("loop[0].body_sese.precursor", "loop[0].end")
        && ConstraintSame("loop[0].body_sese.successor", "loop[0].begin")
        && ConstraintBranchInst("loop[0].body_sese.precursor")
        && ConstraintCFGEdge("loop[0].body_sese.precursor", "loop[0].body_sese.begin")
        && ConstraintBranchInst("loop[0].body_sese.end")
        && ConstraintCFGEdge("loop[0].body_sese.end", "loop[0].body_sese.successor")
        && ConstraintCFGDominate("loop[0].body_sese.begin", "loop[0].body_sese.end")
        && ConstraintCFGPostdom("loop[0].body_sese.end", "loop[0].body_sese.begin")
        && ConstraintCFGDominateStrict("loop[0].body_sese.precursor", "loop[0].body_sese.begin")
        && ConstraintCFGPostdomStrict("loop[0].body_sese.successor", "loop[0].body_sese.end")
        && ConstraintCFGBlocked("loop[0].body_sese.begin", "loop[0].body_sese.end", "loop[0].body_sese.precursor")
        && ConstraintCFGBlocked("loop[0].body_sese.successor", "loop[0].body_sese.begin", "loop[0].body_sese.end")
        && ConstraintDistinct("loop[0].body_sese.begin", "loop[0].successor")
        && ConstraintIncomingValue("loop[0].increment", "loop[0].body_sese.end", "loop[0].iterator")
        && ConstraintDFGEdge("loop[0].iterator", "loop[0].comparison")
        && ConstraintICmpInst("loop[0].comparison")
        && ConstraintDFGEdge("loop[0].comparison", "loop[0].end")
        && ConstraintDFGEdge("loop[0].increment", "loop[0].iterator")
        && ConstraintAddInst("loop[0].increment")
        && ConstraintDFGEdge("loop[0].iterator", "loop[0].increment")
        && ConstraintDFGEdge("loop[0].iter_end", "loop[0].comparison")
        && (  (  ConstraintInstruction("loop[0].begin")
              && ConstraintPreexecution("loop[0].iter_end"))
           || ConstraintCFGDominateStrict("loop[0].iter_end", "loop[0].begin")
           || (  ConstraintInstruction("loop[0].begin")
              && ConstraintUnused("loop[0].iter_end")))
        && ConstraintDFGEdge("loop[0].iter_begin", "loop[0].iterator")
        && (  (  ConstraintInstruction("loop[0].begin")
              && ConstraintPreexecution("loop[0].iter_begin"))
           || ConstraintCFGDominateStrict("loop[0].iter_begin", "loop[0].begin")
           || (  ConstraintInstruction("loop[0].begin")
              && ConstraintUnused("loop[0].iter_begin")))
        && ConstraintDFGEdge("loop[0].iter_step", "loop[0].increment")
        && (  (  ConstraintInstruction("loop[0].begin")
              && ConstraintPreexecution("loop[0].iter_step"))
           || ConstraintCFGDominateStrict("loop[0].iter_step", "loop[0].begin")
           || (  ConstraintInstruction("loop[0].begin")
              && ConstraintUnused("loop[0].iter_step")))
        && ConstraintSame("precursor", "loop[0].precursor")
        && ConstraintSame("begin", "loop[0].begin")
        && ConstraintSame("end", "loop[0].end")
        && ConstraintSame("successor", "loop[0].successor")
        && ConstraintBranchInst("loop[1].precursor")
        && ConstraintCFGEdge("loop[1].precursor", "loop[1].begin")
        && ConstraintBranchInst("loop[1].end")
        && ConstraintCFGEdge("loop[1].end", "loop[1].successor")
        && ConstraintCFGDominate("loop[1].begin", "loop[1].end")
        && ConstraintCFGPostdom("loop[1].end", "loop[1].begin")
        && ConstraintCFGDominateStrict("loop[1].precursor", "loop[1].begin")
        && ConstraintCFGPostdomStrict("loop[1].successor", "loop[1].end")
        && ConstraintCFGBlocked("loop[1].begin", "loop[1].end", "loop[1].precursor")
        && ConstraintCFGBlocked("loop[1].successor", "loop[1].begin", "loop[1].end")
        && ConstraintSame("loop[1].body_sese.precursor", "loop[1].end")
        && ConstraintSame("loop[1].body_sese.successor", "loop[1].begin")
        && ConstraintBranchInst("loop[1].body_sese.precursor")
        && ConstraintCFGEdge("loop[1].body_sese.precursor", "loop[1].body_sese.begin")
        && ConstraintBranchInst("loop[1].body_sese.end")
        && ConstraintCFGEdge("loop[1].body_sese.end", "loop[1].body_sese.successor")
        && ConstraintCFGDominate("loop[1].body_sese.begin", "loop[1].body_sese.end")
        && ConstraintCFGPostdom("loop[1].body_sese.end", "loop[1].body_sese.begin")
        && ConstraintCFGDominateStrict("loop[1].body_sese.precursor", "loop[1].body_sese.begin")
        && ConstraintCFGPostdomStrict("loop[1].body_sese.successor", "loop[1].body_sese.end")
        && ConstraintCFGBlocked("loop[1].body_sese.begin", "loop[1].body_sese.end", "loop[1].body_sese.precursor")
        && ConstraintCFGBlocked("loop[1].body_sese.successor", "loop[1].body_sese.begin", "loop[1].body_sese.end")
        && ConstraintDistinct("loop[1].body_sese.begin", "loop[1].successor")
        && ConstraintIncomingValue("loop[1].increment", "loop[1].body_sese.end", "loop[1].iterator")
        && ConstraintDFGEdge("loop[1].iterator", "loop[1].comparison")
        && ConstraintICmpInst("loop[1].comparison")
        && ConstraintDFGEdge("loop[1].comparison", "loop[1].end")
        && ConstraintDFGEdge("loop[1].increment", "loop[1].iterator")
        && ConstraintAddInst("loop[1].increment")
        && ConstraintDFGEdge("loop[1].iterator", "loop[1].increment")
        && ConstraintDFGEdge("loop[1].iter_end", "loop[1].comparison")
        && (  (  ConstraintInstruction("loop[1].begin")
              && ConstraintPreexecution("loop[1].iter_end"))
           || ConstraintCFGDominateStrict("loop[1].iter_end", "loop[1].begin")
           || (  ConstraintInstruction("loop[1].begin")
              && ConstraintUnused("loop[1].iter_end")))
        && ConstraintDFGEdge("loop[1].iter_begin", "loop[1].iterator")
        && (  (  ConstraintInstruction("loop[1].begin")
              && ConstraintPreexecution("loop[1].iter_begin"))
           || ConstraintCFGDominateStrict("loop[1].iter_begin", "loop[1].begin")
           || (  ConstraintInstruction("loop[1].begin")
              && ConstraintUnused("loop[1].iter_begin")))
        && ConstraintDFGEdge("loop[1].iter_step", "loop[1].increment")
        && (  (  ConstraintInstruction("loop[1].begin")
              && ConstraintPreexecution("loop[1].iter_step"))
           || ConstraintCFGDominateStrict("loop[1].iter_step", "loop[1].begin")
           || (  ConstraintInstruction("loop[1].begin")
              && ConstraintUnused("loop[1].iter_step")))
        && ConstraintSame("loop[1].pre_sese.precursor", "loop[0].body_sese.precursor")
        && ConstraintSame("loop[1].pre_sese.begin", "loop[0].body_sese.begin")
        && ConstraintSame("loop[1].pre_sese.end", "loop[1].precursor")
        && ConstraintSame("loop[1].pre_sese.successor", "loop[1].begin")
        && (  (  ConstraintBranchInst("loop[1].pre_sese.precursor")
              && ConstraintCFGEdge("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin")
              && ConstraintBranchInst("loop[1].pre_sese.end")
              && ConstraintCFGEdge("loop[1].pre_sese.end", "loop[1].pre_sese.successor")
              && ConstraintCFGDominate("loop[1].pre_sese.begin", "loop[1].pre_sese.end")
              && ConstraintCFGPostdom("loop[1].pre_sese.end", "loop[1].pre_sese.begin")
              && ConstraintCFGDominateStrict("loop[1].pre_sese.precursor", "loop[1].pre_sese.begin")
              && ConstraintCFGPostdomStrict("loop[1].pre_sese.successor", "loop[1].pre_sese.end")
              && ConstraintCFGBlocked("loop[1].pre_sese.begin", "loop[1].pre_sese.end", "loop[1].pre_sese.precursor")
              && ConstraintCFGBlocked("loop[1].pre_sese.successor", "loop[1].pre_sese.begin", "loop[1].pre_sese.end"))
           || (  ConstraintSame("loop[1].pre_sese.precursor", "loop[1].pre_sese.end")
              && ConstraintSame("loop[1].pre_sese.begin", "loop[1].pre_sese.successor")))
        && ConstraintSame("loop[1].post_sese.precursor", "loop[1].end")
        && ConstraintSame("loop[1].post_sese.begin", "loop[1].successor")
        && ConstraintSame("loop[1].post_sese.end", "loop[0].body_sese.end")
        && ConstraintSame("loop[1].post_sese.successor", "loop[0].body_sese.successor")
        && (  (  ConstraintBranchInst("loop[1].post_sese.precursor")
              && ConstraintCFGEdge("loop[1].post_sese.precursor", "loop[1].post_sese.begin")
              && ConstraintBranchInst("loop[1].post_sese.end")
              && ConstraintCFGEdge("loop[1].post_sese.end", "loop[1].post_sese.successor")
              && ConstraintCFGDominate("loop[1].post_sese.begin", "loop[1].post_sese.end")
              && ConstraintCFGPostdom("loop[1].post_sese.end", "loop[1].post_sese.begin")
              && ConstraintCFGDominateStrict("loop[1].post_sese.precursor", "loop[1].post_sese.begin")
              && ConstraintCFGPostdomStrict("loop[1].post_sese.successor", "loop[1].post_sese.end")
              && ConstraintCFGBlocked("loop[1].post_sese.begin", "loop[1].post_sese.end", "loop[1].post_sese.precursor")
              && ConstraintCFGBlocked("loop[1].post_sese.successor", "loop[1].post_sese.begin", "loop[1].post_sese.end"))
           || (  ConstraintSame("loop[1].post_sese.precursor", "loop[1].post_sese.end")
              && ConstraintSame("loop[1].post_sese.begin", "loop[1].post_sese.successor")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[1].iter_begin"))
           || ConstraintCFGDominateStrict("loop[1].iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[1].iter_begin")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[1].iter_step"))
           || ConstraintCFGDominateStrict("loop[1].iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[1].iter_step")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[1].iter_end"))
           || ConstraintCFGDominateStrict("loop[1].iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[1].iter_end")))
        && ConstraintBranchInst("loop[2].precursor")
        && ConstraintCFGEdge("loop[2].precursor", "loop[2].begin")
        && ConstraintBranchInst("loop[2].end")
        && ConstraintCFGEdge("loop[2].end", "loop[2].successor")
        && ConstraintCFGDominate("loop[2].begin", "loop[2].end")
        && ConstraintCFGPostdom("loop[2].end", "loop[2].begin")
        && ConstraintCFGDominateStrict("loop[2].precursor", "loop[2].begin")
        && ConstraintCFGPostdomStrict("loop[2].successor", "loop[2].end")
        && ConstraintCFGBlocked("loop[2].begin", "loop[2].end", "loop[2].precursor")
        && ConstraintCFGBlocked("loop[2].successor", "loop[2].begin", "loop[2].end")
        && ConstraintSame("loop[2].body_sese.precursor", "loop[2].end")
        && ConstraintSame("loop[2].body_sese.successor", "loop[2].begin")
        && ConstraintBranchInst("loop[2].body_sese.precursor")
        && ConstraintCFGEdge("loop[2].body_sese.precursor", "loop[2].body_sese.begin")
        && ConstraintBranchInst("loop[2].body_sese.end")
        && ConstraintCFGEdge("loop[2].body_sese.end", "loop[2].body_sese.successor")
        && ConstraintCFGDominate("loop[2].body_sese.begin", "loop[2].body_sese.end")
        && ConstraintCFGPostdom("loop[2].body_sese.end", "loop[2].body_sese.begin")
        && ConstraintCFGDominateStrict("loop[2].body_sese.precursor", "loop[2].body_sese.begin")
        && ConstraintCFGPostdomStrict("loop[2].body_sese.successor", "loop[2].body_sese.end")
        && ConstraintCFGBlocked("loop[2].body_sese.begin", "loop[2].body_sese.end", "loop[2].body_sese.precursor")
        && ConstraintCFGBlocked("loop[2].body_sese.successor", "loop[2].body_sese.begin", "loop[2].body_sese.end")
        && ConstraintDistinct("loop[2].body_sese.begin", "loop[2].successor")
        && ConstraintIncomingValue("loop[2].increment", "loop[2].body_sese.end", "loop[2].iterator")
        && ConstraintDFGEdge("loop[2].iterator", "loop[2].comparison")
        && ConstraintICmpInst("loop[2].comparison")
        && ConstraintDFGEdge("loop[2].comparison", "loop[2].end")
        && ConstraintDFGEdge("loop[2].increment", "loop[2].iterator")
        && ConstraintAddInst("loop[2].increment")
        && ConstraintDFGEdge("loop[2].iterator", "loop[2].increment")
        && ConstraintDFGEdge("loop[2].iter_end", "loop[2].comparison")
        && (  (  ConstraintInstruction("loop[2].begin")
              && ConstraintPreexecution("loop[2].iter_end"))
           || ConstraintCFGDominateStrict("loop[2].iter_end", "loop[2].begin")
           || (  ConstraintInstruction("loop[2].begin")
              && ConstraintUnused("loop[2].iter_end")))
        && ConstraintDFGEdge("loop[2].iter_begin", "loop[2].iterator")
        && (  (  ConstraintInstruction("loop[2].begin")
              && ConstraintPreexecution("loop[2].iter_begin"))
           || ConstraintCFGDominateStrict("loop[2].iter_begin", "loop[2].begin")
           || (  ConstraintInstruction("loop[2].begin")
              && ConstraintUnused("loop[2].iter_begin")))
        && ConstraintDFGEdge("loop[2].iter_step", "loop[2].increment")
        && (  (  ConstraintInstruction("loop[2].begin")
              && ConstraintPreexecution("loop[2].iter_step"))
           || ConstraintCFGDominateStrict("loop[2].iter_step", "loop[2].begin")
           || (  ConstraintInstruction("loop[2].begin")
              && ConstraintUnused("loop[2].iter_step")))
        && ConstraintSame("loop[2].pre_sese.precursor", "loop[1].body_sese.precursor")
        && ConstraintSame("loop[2].pre_sese.begin", "loop[1].body_sese.begin")
        && ConstraintSame("loop[2].pre_sese.end", "loop[2].precursor")
        && ConstraintSame("loop[2].pre_sese.successor", "loop[2].begin")
        && (  (  ConstraintBranchInst("loop[2].pre_sese.precursor")
              && ConstraintCFGEdge("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin")
              && ConstraintBranchInst("loop[2].pre_sese.end")
              && ConstraintCFGEdge("loop[2].pre_sese.end", "loop[2].pre_sese.successor")
              && ConstraintCFGDominate("loop[2].pre_sese.begin", "loop[2].pre_sese.end")
              && ConstraintCFGPostdom("loop[2].pre_sese.end", "loop[2].pre_sese.begin")
              && ConstraintCFGDominateStrict("loop[2].pre_sese.precursor", "loop[2].pre_sese.begin")
              && ConstraintCFGPostdomStrict("loop[2].pre_sese.successor", "loop[2].pre_sese.end")
              && ConstraintCFGBlocked("loop[2].pre_sese.begin", "loop[2].pre_sese.end", "loop[2].pre_sese.precursor")
              && ConstraintCFGBlocked("loop[2].pre_sese.successor", "loop[2].pre_sese.begin", "loop[2].pre_sese.end"))
           || (  ConstraintSame("loop[2].pre_sese.precursor", "loop[2].pre_sese.end")
              && ConstraintSame("loop[2].pre_sese.begin", "loop[2].pre_sese.successor")))
        && ConstraintSame("loop[2].post_sese.precursor", "loop[2].end")
        && ConstraintSame("loop[2].post_sese.begin", "loop[2].successor")
        && ConstraintSame("loop[2].post_sese.end", "loop[1].body_sese.end")
        && ConstraintSame("loop[2].post_sese.successor", "loop[1].body_sese.successor")
        && (  (  ConstraintBranchInst("loop[2].post_sese.precursor")
              && ConstraintCFGEdge("loop[2].post_sese.precursor", "loop[2].post_sese.begin")
              && ConstraintBranchInst("loop[2].post_sese.end")
              && ConstraintCFGEdge("loop[2].post_sese.end", "loop[2].post_sese.successor")
              && ConstraintCFGDominate("loop[2].post_sese.begin", "loop[2].post_sese.end")
              && ConstraintCFGPostdom("loop[2].post_sese.end", "loop[2].post_sese.begin")
              && ConstraintCFGDominateStrict("loop[2].post_sese.precursor", "loop[2].post_sese.begin")
              && ConstraintCFGPostdomStrict("loop[2].post_sese.successor", "loop[2].post_sese.end")
              && ConstraintCFGBlocked("loop[2].post_sese.begin", "loop[2].post_sese.end", "loop[2].post_sese.precursor")
              && ConstraintCFGBlocked("loop[2].post_sese.successor", "loop[2].post_sese.begin", "loop[2].post_sese.end"))
           || (  ConstraintSame("loop[2].post_sese.precursor", "loop[2].post_sese.end")
              && ConstraintSame("loop[2].post_sese.begin", "loop[2].post_sese.successor")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[2].iter_begin"))
           || ConstraintCFGDominateStrict("loop[2].iter_begin", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[2].iter_begin")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[2].iter_step"))
           || ConstraintCFGDominateStrict("loop[2].iter_step", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[2].iter_step")))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("loop[2].iter_end"))
           || ConstraintCFGDominateStrict("loop[2].iter_end", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("loop[2].iter_end")))
        && ConstraintSame("body_sese.precursor", "loop[2].body_sese.precursor")
        && ConstraintSame("body_sese.begin", "loop[2].body_sese.begin")
        && ConstraintSame("body_sese.end", "loop[2].body_sese.end")
        && ConstraintSame("body_sese.successor", "loop[2].body_sese.successor")
        && ConstraintStoreInst("write.store")
        && ConstraintSecondArgument("write.access_pointer", "write.store")
        && ConstraintGEPInst("write.access_pointer")
        && ConstraintFirstArgument("write.base_pointer", "write.access_pointer")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("write.base_pointer"))
           || ConstraintCFGDominateStrict("write.base_pointer", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("write.base_pointer")))
        && ConstraintSecondArgument("write.output_index", "write.access_pointer")
        && (  (  ConstraintZExtInst("write.output_index")
              && ConstraintDFGEdge("write.offset_add.value", "write.output_index"))
           || (  ConstraintSExtInst("write.output_index")
              && ConstraintDFGEdge("write.offset_add.value", "write.output_index"))
           || ConstraintSame("write.offset_add.value", "write.output_index"))
        && (  (  ConstraintSame("write.offset_add.input", "write.offset_add.value")
              && ConstraintUnused("write.offset_add.addend"))
           || (  (  ConstraintAddInst("write.offset_add.value")
                 || ConstraintBitOrInst("write.offset_add.value")
                 || ConstraintFAddInst("write.offset_add.value"))
              && (  (  ConstraintFirstArgument("write.offset_add.input", "write.offset_add.value")
                    && ConstraintSecondArgument("write.offset_add.addend", "write.offset_add.value"))
                 || (  ConstraintFirstArgument("write.offset_add.addend", "write.offset_add.value")
                    && ConstraintSecondArgument("write.offset_add.input", "write.offset_add.value"))))
           || (  (  ConstraintSubInst("write.offset_add.value")
                 || ConstraintFSubInst("write.offset_add.value"))
              && (  (  ConstraintFirstArgument("write.offset_add.input", "write.offset_add.value")
                    && ConstraintSecondArgument("write.offset_add.addend", "write.offset_add.value"))
                 || (  ConstraintFirstArgument("write.offset_add.addend", "write.offset_add.value")
                    && ConstraintSecondArgument("write.offset_add.input", "write.offset_add.value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("write.offset_add.addend"))
           || ConstraintCFGDominateStrict("write.offset_add.addend", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("write.offset_add.addend")))
        && ConstraintSame("write.stride_mul[2].value", "write.offset_add.input")
        && (  (  ConstraintSame("write.stride_mul[2].input", "write.stride_mul[2].value")
              && ConstraintUnused("write.stride_mul[2].multiplier"))
           || (  (  ConstraintMulInst("write.stride_mul[2].value")
                 || ConstraintLShiftInst("write.stride_mul[2].value")
                 || ConstraintFMulInst("write.stride_mul[2].value"))
              && (  (  ConstraintFirstArgument("write.stride_mul[2].input", "write.stride_mul[2].value")
                    && ConstraintSecondArgument("write.stride_mul[2].multiplier", "write.stride_mul[2].value"))
                 || (  ConstraintFirstArgument("write.stride_mul[2].multiplier", "write.stride_mul[2].value")
                    && ConstraintSecondArgument("write.stride_mul[2].input", "write.stride_mul[2].value"))))
           || (  ConstraintFDivInst("write.stride_mul[2].value")
              && (  (  ConstraintFirstArgument("write.stride_mul[2].input", "write.stride_mul[2].value")
                    && ConstraintSecondArgument("write.stride_mul[2].multiplier", "write.stride_mul[2].value"))
                 || (  ConstraintFirstArgument("write.stride_mul[2].multiplier", "write.stride_mul[2].value")
                    && ConstraintSecondArgument("write.stride_mul[2].input", "write.stride_mul[2].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("write.stride_mul[2].multiplier"))
           || ConstraintCFGDominateStrict("write.stride_mul[2].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("write.stride_mul[2].multiplier")))
        && ConstraintSame("write.index_add[2].value", "write.stride_mul[2].input")
        && (  (  ConstraintSame("write.index_add[2].input", "write.index_add[2].value")
              && ConstraintUnused("write.index_add[2].addend"))
           || (  (  ConstraintAddInst("write.index_add[2].value")
                 || ConstraintBitOrInst("write.index_add[2].value")
                 || ConstraintFAddInst("write.index_add[2].value"))
              && (  (  ConstraintFirstArgument("write.index_add[2].input", "write.index_add[2].value")
                    && ConstraintSecondArgument("write.index_add[2].addend", "write.index_add[2].value"))
                 || (  ConstraintFirstArgument("write.index_add[2].addend", "write.index_add[2].value")
                    && ConstraintSecondArgument("write.index_add[2].input", "write.index_add[2].value"))))
           || (  (  ConstraintSubInst("write.index_add[2].value")
                 || ConstraintFSubInst("write.index_add[2].value"))
              && (  (  ConstraintFirstArgument("write.index_add[2].input", "write.index_add[2].value")
                    && ConstraintSecondArgument("write.index_add[2].addend", "write.index_add[2].value"))
                 || (  ConstraintFirstArgument("write.index_add[2].addend", "write.index_add[2].value")
                    && ConstraintSecondArgument("write.index_add[2].input", "write.index_add[2].value")))))
        && (  (  ConstraintZExtInst("write.index_add[2].addend")
              && ConstraintDFGEdge("write.input_index[2]", "write.index_add[2].addend"))
           || (  ConstraintSExtInst("write.index_add[2].addend")
              && ConstraintDFGEdge("write.input_index[2]", "write.index_add[2].addend"))
           || ConstraintSame("write.input_index[2]", "write.index_add[2].addend"))
        && (  (  ConstraintDistinct("write.index_add[2].addend", "write.offset_add.value")
              && ConstraintIntegerType("write.input_index[2]"))
           || (  ConstraintSame("write.input_index[2]", "write.index_add[2].addend")
              && ConstraintIntegerType("write.offset_add.value")))
        && ConstraintSame("write.stride_mul[1].value", "write.index_add[2].input")
        && (  (  ConstraintSame("write.stride_mul[1].input", "write.stride_mul[1].value")
              && ConstraintUnused("write.stride_mul[1].multiplier"))
           || (  (  ConstraintMulInst("write.stride_mul[1].value")
                 || ConstraintLShiftInst("write.stride_mul[1].value")
                 || ConstraintFMulInst("write.stride_mul[1].value"))
              && (  (  ConstraintFirstArgument("write.stride_mul[1].input", "write.stride_mul[1].value")
                    && ConstraintSecondArgument("write.stride_mul[1].multiplier", "write.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("write.stride_mul[1].multiplier", "write.stride_mul[1].value")
                    && ConstraintSecondArgument("write.stride_mul[1].input", "write.stride_mul[1].value"))))
           || (  ConstraintFDivInst("write.stride_mul[1].value")
              && (  (  ConstraintFirstArgument("write.stride_mul[1].input", "write.stride_mul[1].value")
                    && ConstraintSecondArgument("write.stride_mul[1].multiplier", "write.stride_mul[1].value"))
                 || (  ConstraintFirstArgument("write.stride_mul[1].multiplier", "write.stride_mul[1].value")
                    && ConstraintSecondArgument("write.stride_mul[1].input", "write.stride_mul[1].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("write.stride_mul[1].multiplier"))
           || ConstraintCFGDominateStrict("write.stride_mul[1].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("write.stride_mul[1].multiplier")))
        && ConstraintSame("write.index_add[1].value", "write.stride_mul[1].input")
        && (  (  ConstraintSame("write.index_add[1].input", "write.index_add[1].value")
              && ConstraintUnused("write.index_add[1].addend"))
           || (  (  ConstraintAddInst("write.index_add[1].value")
                 || ConstraintBitOrInst("write.index_add[1].value")
                 || ConstraintFAddInst("write.index_add[1].value"))
              && (  (  ConstraintFirstArgument("write.index_add[1].input", "write.index_add[1].value")
                    && ConstraintSecondArgument("write.index_add[1].addend", "write.index_add[1].value"))
                 || (  ConstraintFirstArgument("write.index_add[1].addend", "write.index_add[1].value")
                    && ConstraintSecondArgument("write.index_add[1].input", "write.index_add[1].value"))))
           || (  (  ConstraintSubInst("write.index_add[1].value")
                 || ConstraintFSubInst("write.index_add[1].value"))
              && (  (  ConstraintFirstArgument("write.index_add[1].input", "write.index_add[1].value")
                    && ConstraintSecondArgument("write.index_add[1].addend", "write.index_add[1].value"))
                 || (  ConstraintFirstArgument("write.index_add[1].addend", "write.index_add[1].value")
                    && ConstraintSecondArgument("write.index_add[1].input", "write.index_add[1].value")))))
        && (  (  ConstraintZExtInst("write.index_add[1].addend")
              && ConstraintDFGEdge("write.input_index[1]", "write.index_add[1].addend"))
           || (  ConstraintSExtInst("write.index_add[1].addend")
              && ConstraintDFGEdge("write.input_index[1]", "write.index_add[1].addend"))
           || ConstraintSame("write.input_index[1]", "write.index_add[1].addend"))
        && (  (  ConstraintDistinct("write.index_add[1].addend", "write.input_index[2]")
              && ConstraintIntegerType("write.input_index[1]"))
           || (  ConstraintSame("write.input_index[1]", "write.index_add[1].addend")
              && ConstraintIntegerType("write.input_index[2]")))
        && ConstraintSame("write.stride_mul[0].value", "write.index_add[1].input")
        && (  (  ConstraintSame("write.stride_mul[0].input", "write.stride_mul[0].value")
              && ConstraintUnused("write.stride_mul[0].multiplier"))
           || (  (  ConstraintMulInst("write.stride_mul[0].value")
                 || ConstraintLShiftInst("write.stride_mul[0].value")
                 || ConstraintFMulInst("write.stride_mul[0].value"))
              && (  (  ConstraintFirstArgument("write.stride_mul[0].input", "write.stride_mul[0].value")
                    && ConstraintSecondArgument("write.stride_mul[0].multiplier", "write.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("write.stride_mul[0].multiplier", "write.stride_mul[0].value")
                    && ConstraintSecondArgument("write.stride_mul[0].input", "write.stride_mul[0].value"))))
           || (  ConstraintFDivInst("write.stride_mul[0].value")
              && (  (  ConstraintFirstArgument("write.stride_mul[0].input", "write.stride_mul[0].value")
                    && ConstraintSecondArgument("write.stride_mul[0].multiplier", "write.stride_mul[0].value"))
                 || (  ConstraintFirstArgument("write.stride_mul[0].multiplier", "write.stride_mul[0].value")
                    && ConstraintSecondArgument("write.stride_mul[0].input", "write.stride_mul[0].value")))))
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("write.stride_mul[0].multiplier"))
           || ConstraintCFGDominateStrict("write.stride_mul[0].multiplier", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("write.stride_mul[0].multiplier")))
        && ConstraintSame("write.index_add[0].value", "write.stride_mul[0].input")
        && (  (  ConstraintSame("write.index_add[0].input", "write.index_add[0].value")
              && ConstraintUnused("write.index_add[0].addend"))
           || (  (  ConstraintAddInst("write.index_add[0].value")
                 || ConstraintBitOrInst("write.index_add[0].value")
                 || ConstraintFAddInst("write.index_add[0].value"))
              && (  (  ConstraintFirstArgument("write.index_add[0].input", "write.index_add[0].value")
                    && ConstraintSecondArgument("write.index_add[0].addend", "write.index_add[0].value"))
                 || (  ConstraintFirstArgument("write.index_add[0].addend", "write.index_add[0].value")
                    && ConstraintSecondArgument("write.index_add[0].input", "write.index_add[0].value"))))
           || (  (  ConstraintSubInst("write.index_add[0].value")
                 || ConstraintFSubInst("write.index_add[0].value"))
              && (  (  ConstraintFirstArgument("write.index_add[0].input", "write.index_add[0].value")
                    && ConstraintSecondArgument("write.index_add[0].addend", "write.index_add[0].value"))
                 || (  ConstraintFirstArgument("write.index_add[0].addend", "write.index_add[0].value")
                    && ConstraintSecondArgument("write.index_add[0].input", "write.index_add[0].value")))))
        && (  (  ConstraintZExtInst("write.index_add[0].input")
              && ConstraintDFGEdge("write.input_index[0]", "write.index_add[0].input"))
           || (  ConstraintSExtInst("write.index_add[0].input")
              && ConstraintDFGEdge("write.input_index[0]", "write.index_add[0].input"))
           || ConstraintSame("write.input_index[0]", "write.index_add[0].input"))
        && (  (  ConstraintDistinct("write.index_add[0].input", "write.input_index[1]")
              && ConstraintIntegerType("write.input_index[0]"))
           || (  ConstraintSame("write.input_index[0]", "write.index_add[0].input")
              && ConstraintIntegerType("write.input_index[1]")))
        && ConstraintSame("write.base_index", "write.index_add[0].addend")
        && (  (  ConstraintInstruction("begin")
              && ConstraintPreexecution("write.base_index"))
           || ConstraintCFGDominateStrict("write.base_index", "begin")
           || (  ConstraintInstruction("begin")
              && ConstraintUnused("write.base_index")))
        && (  (  ConstraintSame("loop[0].iterator", "write.input_index[0]")
              && (  (  ConstraintSame("loop[1].iterator", "write.input_index[1]")
                    && ConstraintSame("loop[2].iterator", "write.input_index[2]"))
                 || (  ConstraintSame("loop[1].iterator", "write.input_index[2]")
                    && ConstraintSame("loop[2].iterator", "write.input_index[1]"))))
           || (  ConstraintSame("loop[0].iterator", "write.input_index[1]")
              && (  (  ConstraintSame("loop[1].iterator", "write.input_index[0]")
                    && ConstraintSame("loop[2].iterator", "write.input_index[2]"))
                 || (  ConstraintSame("loop[1].iterator", "write.input_index[2]")
                    && ConstraintSame("loop[2].iterator", "write.input_index[0]"))))
           || (  ConstraintSame("loop[0].iterator", "write.input_index[2]")
              && (  (  ConstraintSame("loop[1].iterator", "write.input_index[1]")
                    && ConstraintSame("loop[2].iterator", "write.input_index[0]"))
                 || (  ConstraintSame("loop[1].iterator", "write.input_index[0]")
                    && ConstraintSame("loop[2].iterator", "write.input_index[1]")))))
        && ConstraintFirstArgument("compute_expr.output", "write.store")
        && std::make_pair("i",32) * (  ConstraintLoadInst("reads[i].value")
                                    && ConstraintFirstArgument("reads[i].access_pointer", "reads[i].value")
                                    && ConstraintGEPInst("reads[i].read.access_pointer")
                                    && ConstraintFirstArgument("reads[i].read.base_pointer", "reads[i].read.access_pointer")
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("reads[i].read.base_pointer"))
                                       || ConstraintCFGDominateStrict("reads[i].read.base_pointer", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("reads[i].read.base_pointer")))
                                    && ConstraintSecondArgument("reads[i].read.output_index", "reads[i].read.access_pointer")
                                    && (  (  ConstraintZExtInst("reads[i].read.output_index")
                                          && ConstraintDFGEdge("reads[i].read.offset_add.value", "reads[i].read.output_index"))
                                       || (  ConstraintSExtInst("reads[i].read.output_index")
                                          && ConstraintDFGEdge("reads[i].read.offset_add.value", "reads[i].read.output_index"))
                                       || ConstraintSame("reads[i].read.offset_add.value", "reads[i].read.output_index"))
                                    && (  (  ConstraintSame("reads[i].read.offset_add.input", "reads[i].read.offset_add.value")
                                          && ConstraintUnused("reads[i].read.offset_add.addend"))
                                       || (  (  ConstraintAddInst("reads[i].read.offset_add.value")
                                             || ConstraintBitOrInst("reads[i].read.offset_add.value")
                                             || ConstraintFAddInst("reads[i].read.offset_add.value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.offset_add.input", "reads[i].read.offset_add.value")
                                                && ConstraintSecondArgument("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value"))
                                             || (  ConstraintFirstArgument("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value")
                                                && ConstraintSecondArgument("reads[i].read.offset_add.input", "reads[i].read.offset_add.value"))))
                                       || (  (  ConstraintSubInst("reads[i].read.offset_add.value")
                                             || ConstraintFSubInst("reads[i].read.offset_add.value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.offset_add.input", "reads[i].read.offset_add.value")
                                                && ConstraintSecondArgument("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value"))
                                             || (  ConstraintFirstArgument("reads[i].read.offset_add.addend", "reads[i].read.offset_add.value")
                                                && ConstraintSecondArgument("reads[i].read.offset_add.input", "reads[i].read.offset_add.value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("reads[i].read.offset_add.addend"))
                                       || ConstraintCFGDominateStrict("reads[i].read.offset_add.addend", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("reads[i].read.offset_add.addend")))
                                    && ConstraintSame("reads[i].read.stride_mul[2].value", "reads[i].read.offset_add.input")
                                    && (  (  ConstraintSame("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value")
                                          && ConstraintUnused("reads[i].read.stride_mul[2].multiplier"))
                                       || (  (  ConstraintMulInst("reads[i].read.stride_mul[2].value")
                                             || ConstraintLShiftInst("reads[i].read.stride_mul[2].value")
                                             || ConstraintFMulInst("reads[i].read.stride_mul[2].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value"))))
                                       || (  ConstraintFDivInst("reads[i].read.stride_mul[2].value")
                                          && (  (  ConstraintFirstArgument("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.stride_mul[2].multiplier", "reads[i].read.stride_mul[2].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[2].input", "reads[i].read.stride_mul[2].value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("reads[i].read.stride_mul[2].multiplier"))
                                       || ConstraintCFGDominateStrict("reads[i].read.stride_mul[2].multiplier", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("reads[i].read.stride_mul[2].multiplier")))
                                    && ConstraintSame("reads[i].read.index_add[2].value", "reads[i].read.stride_mul[2].input")
                                    && (  (  ConstraintSame("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value")
                                          && ConstraintUnused("reads[i].read.index_add[2].addend"))
                                       || (  (  ConstraintAddInst("reads[i].read.index_add[2].value")
                                             || ConstraintBitOrInst("reads[i].read.index_add[2].value")
                                             || ConstraintFAddInst("reads[i].read.index_add[2].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value"))))
                                       || (  (  ConstraintSubInst("reads[i].read.index_add[2].value")
                                             || ConstraintFSubInst("reads[i].read.index_add[2].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.index_add[2].addend", "reads[i].read.index_add[2].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[2].input", "reads[i].read.index_add[2].value")))))
                                    && (  (  ConstraintZExtInst("reads[i].read.index_add[2].addend")
                                          && ConstraintDFGEdge("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend"))
                                       || (  ConstraintSExtInst("reads[i].read.index_add[2].addend")
                                          && ConstraintDFGEdge("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend"))
                                       || ConstraintSame("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend"))
                                    && (  (  ConstraintDistinct("reads[i].read.index_add[2].addend", "reads[i].read.offset_add.value")
                                          && ConstraintIntegerType("reads[i].read.input_index[2]"))
                                       || (  ConstraintSame("reads[i].read.input_index[2]", "reads[i].read.index_add[2].addend")
                                          && ConstraintIntegerType("reads[i].read.offset_add.value")))
                                    && ConstraintSame("reads[i].read.stride_mul[1].value", "reads[i].read.index_add[2].input")
                                    && (  (  ConstraintSame("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value")
                                          && ConstraintUnused("reads[i].read.stride_mul[1].multiplier"))
                                       || (  (  ConstraintMulInst("reads[i].read.stride_mul[1].value")
                                             || ConstraintLShiftInst("reads[i].read.stride_mul[1].value")
                                             || ConstraintFMulInst("reads[i].read.stride_mul[1].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value"))))
                                       || (  ConstraintFDivInst("reads[i].read.stride_mul[1].value")
                                          && (  (  ConstraintFirstArgument("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.stride_mul[1].multiplier", "reads[i].read.stride_mul[1].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[1].input", "reads[i].read.stride_mul[1].value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("reads[i].read.stride_mul[1].multiplier"))
                                       || ConstraintCFGDominateStrict("reads[i].read.stride_mul[1].multiplier", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("reads[i].read.stride_mul[1].multiplier")))
                                    && ConstraintSame("reads[i].read.index_add[1].value", "reads[i].read.stride_mul[1].input")
                                    && (  (  ConstraintSame("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value")
                                          && ConstraintUnused("reads[i].read.index_add[1].addend"))
                                       || (  (  ConstraintAddInst("reads[i].read.index_add[1].value")
                                             || ConstraintBitOrInst("reads[i].read.index_add[1].value")
                                             || ConstraintFAddInst("reads[i].read.index_add[1].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value"))))
                                       || (  (  ConstraintSubInst("reads[i].read.index_add[1].value")
                                             || ConstraintFSubInst("reads[i].read.index_add[1].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.index_add[1].addend", "reads[i].read.index_add[1].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[1].input", "reads[i].read.index_add[1].value")))))
                                    && (  (  ConstraintZExtInst("reads[i].read.index_add[1].addend")
                                          && ConstraintDFGEdge("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend"))
                                       || (  ConstraintSExtInst("reads[i].read.index_add[1].addend")
                                          && ConstraintDFGEdge("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend"))
                                       || ConstraintSame("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend"))
                                    && (  (  ConstraintDistinct("reads[i].read.index_add[1].addend", "reads[i].read.input_index[2]")
                                          && ConstraintIntegerType("reads[i].read.input_index[1]"))
                                       || (  ConstraintSame("reads[i].read.input_index[1]", "reads[i].read.index_add[1].addend")
                                          && ConstraintIntegerType("reads[i].read.input_index[2]")))
                                    && ConstraintSame("reads[i].read.stride_mul[0].value", "reads[i].read.index_add[1].input")
                                    && (  (  ConstraintSame("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value")
                                          && ConstraintUnused("reads[i].read.stride_mul[0].multiplier"))
                                       || (  (  ConstraintMulInst("reads[i].read.stride_mul[0].value")
                                             || ConstraintLShiftInst("reads[i].read.stride_mul[0].value")
                                             || ConstraintFMulInst("reads[i].read.stride_mul[0].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value"))))
                                       || (  ConstraintFDivInst("reads[i].read.stride_mul[0].value")
                                          && (  (  ConstraintFirstArgument("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.stride_mul[0].multiplier", "reads[i].read.stride_mul[0].value")
                                                && ConstraintSecondArgument("reads[i].read.stride_mul[0].input", "reads[i].read.stride_mul[0].value")))))
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("reads[i].read.stride_mul[0].multiplier"))
                                       || ConstraintCFGDominateStrict("reads[i].read.stride_mul[0].multiplier", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("reads[i].read.stride_mul[0].multiplier")))
                                    && ConstraintSame("reads[i].read.index_add[0].value", "reads[i].read.stride_mul[0].input")
                                    && (  (  ConstraintSame("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value")
                                          && ConstraintUnused("reads[i].read.index_add[0].addend"))
                                       || (  (  ConstraintAddInst("reads[i].read.index_add[0].value")
                                             || ConstraintBitOrInst("reads[i].read.index_add[0].value")
                                             || ConstraintFAddInst("reads[i].read.index_add[0].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value"))))
                                       || (  (  ConstraintSubInst("reads[i].read.index_add[0].value")
                                             || ConstraintFSubInst("reads[i].read.index_add[0].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value"))
                                             || (  ConstraintFirstArgument("reads[i].read.index_add[0].addend", "reads[i].read.index_add[0].value")
                                                && ConstraintSecondArgument("reads[i].read.index_add[0].input", "reads[i].read.index_add[0].value")))))
                                    && (  (  ConstraintZExtInst("reads[i].read.index_add[0].input")
                                          && ConstraintDFGEdge("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input"))
                                       || (  ConstraintSExtInst("reads[i].read.index_add[0].input")
                                          && ConstraintDFGEdge("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input"))
                                       || ConstraintSame("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input"))
                                    && (  (  ConstraintDistinct("reads[i].read.index_add[0].input", "reads[i].read.input_index[1]")
                                          && ConstraintIntegerType("reads[i].read.input_index[0]"))
                                       || (  ConstraintSame("reads[i].read.input_index[0]", "reads[i].read.index_add[0].input")
                                          && ConstraintIntegerType("reads[i].read.input_index[1]")))
                                    && ConstraintSame("reads[i].read.base_index", "reads[i].read.index_add[0].addend")
                                    && (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("reads[i].read.base_index"))
                                       || ConstraintCFGDominateStrict("reads[i].read.base_index", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("reads[i].read.base_index")))
                                    && ConstraintSame("reads[i].read.base_pointer", "reads[i].base_pointer")
                                    && ConstraintSame("reads[i].read.access_pointer", "reads[i].access_pointer")
                                    && ConstraintSame("reads[i].read.input_index[0]", "reads[i].index[0].value")
                                    && (  (  ConstraintSame("reads[i].index[0].input", "reads[i].index[0].value")
                                          && ConstraintUnused("reads[i].index[0].addend"))
                                       || (  (  ConstraintAddInst("reads[i].index[0].value")
                                             || ConstraintBitOrInst("reads[i].index[0].value")
                                             || ConstraintFAddInst("reads[i].index[0].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].index[0].input", "reads[i].index[0].value")
                                                && ConstraintSecondArgument("reads[i].index[0].addend", "reads[i].index[0].value"))
                                             || (  ConstraintFirstArgument("reads[i].index[0].addend", "reads[i].index[0].value")
                                                && ConstraintSecondArgument("reads[i].index[0].input", "reads[i].index[0].value"))))
                                       || (  (  ConstraintSubInst("reads[i].index[0].value")
                                             || ConstraintFSubInst("reads[i].index[0].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].index[0].input", "reads[i].index[0].value")
                                                && ConstraintSecondArgument("reads[i].index[0].addend", "reads[i].index[0].value"))
                                             || (  ConstraintFirstArgument("reads[i].index[0].addend", "reads[i].index[0].value")
                                                && ConstraintSecondArgument("reads[i].index[0].input", "reads[i].index[0].value")))))
                                    && (  ConstraintConstant("reads[i].index[0].addend")
                                       || ConstraintUnused("reads[i].index[0].addend"))
                                    && ConstraintSame("reads[i].index[0].input", "reads[i].input_index[0]")
                                    && ConstraintSame("reads[i].read.input_index[1]", "reads[i].index[1].value")
                                    && (  (  ConstraintSame("reads[i].index[1].input", "reads[i].index[1].value")
                                          && ConstraintUnused("reads[i].index[1].addend"))
                                       || (  (  ConstraintAddInst("reads[i].index[1].value")
                                             || ConstraintBitOrInst("reads[i].index[1].value")
                                             || ConstraintFAddInst("reads[i].index[1].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].index[1].input", "reads[i].index[1].value")
                                                && ConstraintSecondArgument("reads[i].index[1].addend", "reads[i].index[1].value"))
                                             || (  ConstraintFirstArgument("reads[i].index[1].addend", "reads[i].index[1].value")
                                                && ConstraintSecondArgument("reads[i].index[1].input", "reads[i].index[1].value"))))
                                       || (  (  ConstraintSubInst("reads[i].index[1].value")
                                             || ConstraintFSubInst("reads[i].index[1].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].index[1].input", "reads[i].index[1].value")
                                                && ConstraintSecondArgument("reads[i].index[1].addend", "reads[i].index[1].value"))
                                             || (  ConstraintFirstArgument("reads[i].index[1].addend", "reads[i].index[1].value")
                                                && ConstraintSecondArgument("reads[i].index[1].input", "reads[i].index[1].value")))))
                                    && (  ConstraintConstant("reads[i].index[1].addend")
                                       || ConstraintUnused("reads[i].index[1].addend"))
                                    && ConstraintSame("reads[i].index[1].input", "reads[i].input_index[1]")
                                    && ConstraintSame("reads[i].read.input_index[2]", "reads[i].index[2].value")
                                    && (  (  ConstraintSame("reads[i].index[2].input", "reads[i].index[2].value")
                                          && ConstraintUnused("reads[i].index[2].addend"))
                                       || (  (  ConstraintAddInst("reads[i].index[2].value")
                                             || ConstraintBitOrInst("reads[i].index[2].value")
                                             || ConstraintFAddInst("reads[i].index[2].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].index[2].input", "reads[i].index[2].value")
                                                && ConstraintSecondArgument("reads[i].index[2].addend", "reads[i].index[2].value"))
                                             || (  ConstraintFirstArgument("reads[i].index[2].addend", "reads[i].index[2].value")
                                                && ConstraintSecondArgument("reads[i].index[2].input", "reads[i].index[2].value"))))
                                       || (  (  ConstraintSubInst("reads[i].index[2].value")
                                             || ConstraintFSubInst("reads[i].index[2].value"))
                                          && (  (  ConstraintFirstArgument("reads[i].index[2].input", "reads[i].index[2].value")
                                                && ConstraintSecondArgument("reads[i].index[2].addend", "reads[i].index[2].value"))
                                             || (  ConstraintFirstArgument("reads[i].index[2].addend", "reads[i].index[2].value")
                                                && ConstraintSecondArgument("reads[i].index[2].input", "reads[i].index[2].value")))))
                                    && (  ConstraintConstant("reads[i].index[2].addend")
                                       || ConstraintUnused("reads[i].index[2].addend"))
                                    && ConstraintSame("reads[i].index[2].input", "reads[i].input_index[2]")
                                    && ConstraintSame("reads[i].input_index[0]", "write.input_index[0]")
                                    && ConstraintSame("reads[i].input_index[1]", "write.input_index[1]")
                                    && ConstraintSame("reads[i].input_index[2]", "write.input_index[2]"))
        && ConstraintLoadInst("reads[0].value")
        && ConstraintSame("compute_expr.input[0]", "reads[0].value")
        && ConstraintSame("compute_expr.input[1]", "reads[1].value")
        && ConstraintSame("compute_expr.input[2]", "reads[2].value")
        && ConstraintSame("compute_expr.input[3]", "reads[3].value")
        && ConstraintSame("compute_expr.input[4]", "reads[4].value")
        && ConstraintSame("compute_expr.input[5]", "reads[5].value")
        && ConstraintSame("compute_expr.input[6]", "reads[6].value")
        && ConstraintSame("compute_expr.input[7]", "reads[7].value")
        && ConstraintSame("compute_expr.input[8]", "reads[8].value")
        && ConstraintSame("compute_expr.input[9]", "reads[9].value")
        && ConstraintSame("compute_expr.input[10]", "reads[10].value")
        && ConstraintSame("compute_expr.input[11]", "reads[11].value")
        && ConstraintSame("compute_expr.input[12]", "reads[12].value")
        && ConstraintSame("compute_expr.input[13]", "reads[13].value")
        && ConstraintSame("compute_expr.input[14]", "reads[14].value")
        && ConstraintSame("compute_expr.input[15]", "reads[15].value")
        && ConstraintSame("compute_expr.input[16]", "reads[16].value")
        && ConstraintSame("compute_expr.input[17]", "reads[17].value")
        && ConstraintSame("compute_expr.input[18]", "reads[18].value")
        && ConstraintSame("compute_expr.input[19]", "reads[19].value")
        && ConstraintSame("compute_expr.input[20]", "reads[20].value")
        && ConstraintSame("compute_expr.input[21]", "reads[21].value")
        && ConstraintSame("compute_expr.input[22]", "reads[22].value")
        && ConstraintSame("compute_expr.input[23]", "reads[23].value")
        && ConstraintSame("compute_expr.input[24]", "reads[24].value")
        && ConstraintSame("compute_expr.input[25]", "reads[25].value")
        && ConstraintSame("compute_expr.input[26]", "reads[26].value")
        && ConstraintSame("compute_expr.input[27]", "reads[27].value")
        && ConstraintSame("compute_expr.input[28]", "reads[28].value")
        && ConstraintSame("compute_expr.input[29]", "reads[29].value")
        && ConstraintSame("compute_expr.input[30]", "reads[30].value")
        && ConstraintSame("compute_expr.input[31]", "reads[31].value")
        && std::make_pair("i",4) * ConstraintCFGEdge("compute_expr.origin[i]", "body_sese.begin")
        && std::make_pair("i",64) * (  (  (  ConstraintInstruction("begin")
                                          && ConstraintPreexecution("compute_expr.constants[i].value"))
                                       || ConstraintCFGDominateStrict("compute_expr.constants[i].value", "begin")
                                       || (  ConstraintInstruction("begin")
                                          && ConstraintUnused("compute_expr.constants[i].value")))
                                    && ConstraintNotNumericConstant("compute_expr.constants[i].value")
                                    && ConstraintDFGEdge("compute_expr.constants[i].value", "compute_expr.constants[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "compute_expr.constants[i].use"))
        && std::make_pair("i",64) * (  ConstraintCFGDominate("begin", "compute_expr.restrictions[i].value")
                                    && ConstraintCFGDominateStrict("compute_expr.restrictions[i].value", "body_sese.begin")
                                    && ConstraintDFGEdge("compute_expr.restrictions[i].value", "compute_expr.restrictions[i].use")
                                    && ConstraintCFGDominate("body_sese.begin", "compute_expr.restrictions[i].use"))
        && ConstraintPDGDominate({std::vector<std::string>{"compute_expr.restrictions[0].value", "compute_expr.restrictions[1].value",
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
                                  std::vector<std::string>{"compute_expr.origin[0]", "compute_expr.origin[1]", "compute_expr.origin[2]",
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
                                  std::vector<std::string>{"compute_expr.output"}});
}
*/
#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/ConstraintCollect.hpp"
#include "llvm/Constraints/ConstraintsBasic.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"

ConstraintContainer ConstraintPermute(std::vector<std::string> in, std::vector<std::string> out)
{
    if(in.size() == 0 || out.size() == 0)
    {
        return {};
    }
    else if(out.size() == 1)
    {
        return ConstraintSame(in[0], out[0]);
    }
    else
    {
        auto possibility1 = ConstraintSame(in[0], out[0])
                         && ConstraintPermute({in.begin()+1, in.end()}, {out.begin()+1, out.end()});

        std::swap(out[0], out[1]);

        auto possibility2 = ConstraintSame(in[0], out[0])
                         && ConstraintPermute({in.begin()+1, in.end()}, {out.begin()+1, out.end()});

        auto result = std::move(possibility1) || std::move(possibility2);

        for(unsigned i = 2; i < out.size(); i++)
        {
            std::swap(out[0], out[i]);

            result = std::move(result)
                  || (     ConstraintSame(in[0], out[0])
                      &&   ConstraintPermute({in.begin()+1, in.end()}, {out.begin()+1, out.end()}));
        }

        return std::move(result);
    }
}

ConstraintContainer ConstraintPermute(unsigned dim)
{
    std::vector<std::string> in;
    std::vector<std::string> out;

    for(unsigned i = 0; i < dim; i++)
    {
        in.emplace_back("input["+std::string(1, '0'+i)+"]");
        out.emplace_back("output["+std::string(1, '0'+i)+"]");
    }

    return ConstraintPermute(in, out);
}

ConstraintAnd ConstraintArguments()
{
    return ConstraintFirstArgument ("src1", "dst")
        && ConstraintSecondArgument("src2", "dst");
}

ConstraintOr ConstraintArgumentsPermuted()
{
    return ConstraintArguments()
        || ConstraintRename(ConstraintArguments(), {{"src1","src2"},{"src2","src1"}});
}

ConstraintAnd ConstraintSumChain()
{
    return     ConstraintIntegerType                ("value")
        && 0 + ConstraintSame                       ("value",       "results*")
        && 8 * (   (   ConstraintAddInst            ("results*")
                    && ConstraintRename(ConstraintArgumentsPermuted(), {{"dst","results*"}, {"src1","results+"}, {"src2","factors*"}})
                    && ConstraintIntegerType        ("last_factor"))
                || (   (   (   ConstraintIntegerType("results*")
                            && ConstraintSame       ("last_factor", "factors*"))
                        || (   ConstraintUnused     ("results*")
                            && ConstraintUnused     ("factors*")
                            && ConstraintIntegerType("last_factor")))
                    && ConstraintSame               ("results*",    "factors*")
                    && ConstraintUnused             ("results+")))
        && 8 + ConstraintUnused                     ("factors*");
}

ConstraintAnd ConstraintMulChain()
{
    return     ConstraintIntegerType                ("value")
        && 0 + ConstraintSame                       ("value", "results*")
        && 8 * (   (   ConstraintMulInst            ("results*")
                    && ConstraintRename(ConstraintArgumentsPermuted(), {{"dst","results*"}, {"src1","results+"}, {"src2","factors*"}})
                    && ConstraintIntegerType        ("last_factor"))
                || (   (   (   ConstraintIntegerType("results*")
                            && ConstraintSame       ("last_factor", "factors*"))
                        || (   ConstraintUnused     ("results*")
                            && ConstraintUnused     ("factors*")
                            && ConstraintIntegerType("last_factor")))
                    && ConstraintSame               ("results*",    "factors*")
                    && ConstraintUnused             ("results+")))
        && 8 + ConstraintUnused                     ("factors*");
}

ConstraintAnd ConstraintDistributive()
{
    return ConstraintAddInst              ("value")
        && ConstraintFirstArgument        ("sum1.value", "value")
        && ConstraintSecondArgument       ("sum2.value", "value")
        && "sum1" + ConstraintSumChain    ()
        && ConstraintSame                 ("product1.value",           "sum1.last_factor")
        && "product1" + ConstraintMulChain()
        && ConstraintSame                 ("product1.last_factor", "product2.last_factor")
        && "sum2" + ConstraintSumChain    ()
        && ConstraintSame                 ("product2.value",           "sum2.last_factor")
        && "product2" + ConstraintMulChain();
}

// Need to make sure that GEPs actually have same number of arguments
ConstraintAnd ConstraintHoistSelect()
{
    return ConstraintSelectInst    ("select")
        && ConstraintSecondArgument("input1", "select")
        && ConstraintFirstArgument ("base",   "input1")
        && ConstraintThirdArgument ("input2", "select")
        && ConstraintFirstArgument ("base",   "input2")
        && ConstraintGEPInst       ("input1")
        && ConstraintGEPInst       ("input2");
}

ConstraintOr ConstraintLocallyConstant()
{
    return (   ConstraintInstruction  ("border")
            && ConstraintPreexecution ("value"))
        || ConstraintCFGDominateStrict("value", "border")
        || (   ConstraintInstruction  ("border")
            && ConstraintUnused       ("value"));
}

ConstraintAnd ConstraintSESE()
{
    return ConstraintBranchInst       ("precursor")
        && ConstraintCFGEdge          ("precursor",             "begin")
        && ConstraintBranchInst       ("end")
        && ConstraintCFGEdge          ("end",               "successor")
        && ConstraintCFGDominate      ("begin",                   "end")
        && ConstraintCFGPostdom       ("end",                   "begin")
        && ConstraintCFGDominateStrict("precursor",             "begin")
        && ConstraintCFGPostdomStrict ("successor",               "end")
        && ConstraintCFGBlocked       ("begin",      "end", "precursor")
        && ConstraintCFGBlocked       ("successor", "begin",      "end");
}

ConstraintAnd ConstraintInSESE()
{
    return ConstraintCFGDominate("begin", "value")
        && ConstraintCFGPostdom ("end",   "value");
}

// PROBLEM IF VAR IS ONE BEFORE END
ConstraintAnd ConstraintMaxOnceInSESE()
{
    return ConstraintInSESE()
        && ConstraintCFGEdge   ("value",              "value_after")
        && ConstraintCFGBlocked("value_after", "successor", "value")
        && ConstraintCFGBlocked("value_after", "precursor", "value");
}

ConstraintOr ConstraintExtendedInt()
{
    return (   ConstraintZExtInst("output")
            && ConstraintDFGEdge ("input", "output"))
        || (   ConstraintSExtInst("output")
            && ConstraintDFGEdge ("input", "output"))
        || ConstraintSame        ("input", "output");
}

ConstraintAnd ConstraintLoop()
{
    return ConstraintSESE              ()
        && ConstraintSame              ("body_sese.precursor",    "end")
        && ConstraintSame              ("body_sese.successor",  "begin")
        && "body_sese" + ConstraintSESE()
        && ConstraintDistinct          ("body_sese.begin",  "successor");
}

ConstraintAnd ConstraintAfterLoop()
{
    return ConstraintCFGDominateStrict("end",             "value")
        && ConstraintCFGNotDominate   ("body_sese.begin", "value");
}

ConstraintOr ConstraintAddition()
{
    return (   ConstraintSame                   ("input", "value")
            && ConstraintUnused                 ("addend"))
        || (   (   ConstraintAddInst            ("value")
                || ConstraintFAddInst           ("value"))
            && ConstraintRename(ConstraintArgumentsPermuted(), {{"dst","value"}, {"src1","input"}, {"src2","addend"}}))
        || (   (   ConstraintSubInst            ("value")
                || ConstraintFSubInst           ("value"))
            && ConstraintRename(ConstraintArguments(), {{"dst","value"}, {"src1","input"}, {"src2","addend"}}));
}

ConstraintOr ConstraintMultiplication()
{
    return (   ConstraintSame             ("input", "value")
            && ConstraintUnused           ("multiplier"))
        || (   (   ConstraintMulInst      ("value")
                || ConstraintFMulInst     ("value"))
            && ConstraintRename(ConstraintArgumentsPermuted(), {{"dst","value"}, {"src1","input"}, {"src2","multiplier"}}))
        || (   (   ConstraintFDivInst     ("value"))
            && ConstraintRename(ConstraintArguments(), {{"dst","value"}, {"src1","input"}, {"src2","multiplier"}}));
}

ConstraintAnd ConstraintArrayAccess()
{
    return ConstraintGEPInst        ("access_pointer")
        && ConstraintFirstArgument  ("base_pointer", "access_pointer")
        && ConstraintRename(ConstraintLocallyConstant(), {{"value", "base_pointer"}, {"border","origin"}})
        && ConstraintSecondArgument ("output_index", "access_pointer");
}

ConstraintAnd ConstraintArrayRead()
{
    return ConstraintLoadInst     ("value")
        && ConstraintFirstArgument("access_pointer", "value")
        && ConstraintArrayAccess();
}

ConstraintAnd ConstraintArrayStore()
{
    return ConstraintStoreInst     ("store")
        && ConstraintSecondArgument("access_pointer", "store")
        && ConstraintArrayAccess();
}

ConstraintContainer ConstraintInductionVar()
{
    return ConstraintIncomingValue("new_ind", "body_sese.end", "old_ind");
}

ConstraintAnd ConstraintFor()
{
    return ConstraintLoop           ()
        && ConstraintRename(ConstraintInductionVar(), {{"old_ind","iterator"}, {"new_ind","increment"}})
        && ConstraintDFGEdge        ("iterator", "comparison")
        && ConstraintICmpInst       ("comparison")
        && ConstraintDFGEdge        ("comparison",      "end")
        && ConstraintDFGEdge        ("increment",  "iterator")
        && ConstraintAddInst        ("increment")
        && ConstraintDFGEdge        ("iterator",  "increment")
        && ConstraintDFGEdge        ("iter_end", "comparison")
        && ConstraintRename(ConstraintLocallyConstant(), {{"value","iter_end"}, {"border","begin"}})
        && ConstraintDFGEdge        ("iter_begin", "iterator")
        && ConstraintRename(ConstraintLocallyConstant(), {{"value","iter_begin"}, {"border","begin"}})
        && ConstraintDFGEdge        ("iter_step", "increment")
        && ConstraintRename(ConstraintLocallyConstant(), {{"value","iter_step"}, {"border","begin"}});
}

ConstraintAnd ConstraintForNest(unsigned dim)
{
    return 0 + ("loop*" + ConstraintFor()
                && ConstraintSame("precursor", "loop*.precursor")
                && ConstraintSame("begin",     "loop*.begin")
                && ConstraintSame("end",       "loop*.end")
                && ConstraintSame("successor", "loop*.successor"))

        && (dim-1) * (   "loop+" + ConstraintFor  ()
                      && ConstraintSame           ("loop+.pre_sese.precursor",  "loop*.body_sese.precursor")
                      && ConstraintSame           ("loop+.pre_sese.begin",      "loop*.body_sese.begin")
                      && ConstraintSame           ("loop+.pre_sese.end",        "loop+.precursor")
                      && ConstraintSame           ("loop+.pre_sese.successor",  "loop+.begin")
                      && (   "loop+.pre_sese" + ConstraintSESE()
                          || (   ConstraintSame   ("loop+.pre_sese.precursor",  "loop+.pre_sese.end")
                              && ConstraintSame   ("loop+.pre_sese.begin",      "loop+.pre_sese.successor")))
                      && ConstraintSame           ("loop+.post_sese.precursor", "loop+.end")
                      && ConstraintSame           ("loop+.post_sese.begin",     "loop+.successor")
                      && ConstraintSame           ("loop+.post_sese.end",       "loop*.body_sese.end")
                      && ConstraintSame           ("loop+.post_sese.successor", "loop*.body_sese.successor")
                      && (   "loop+.post_sese" + ConstraintSESE()
                          || (   ConstraintSame   ("loop+.post_sese.precursor", "loop+.post_sese.end")
                              && ConstraintSame   ("loop+.post_sese.begin",     "loop+.post_sese.successor")))
                      && ConstraintRename(ConstraintLocallyConstant(), {{"value","loop+.iter_begin"}, {"border","begin"}})
                      && ConstraintRename(ConstraintLocallyConstant(), {{"value","loop+.iter_step"}, {"border","begin"}})
                      && ConstraintRename(ConstraintLocallyConstant(), {{"value","loop+.iter_end"}, {"border","begin"}}))

        && (dim-1) + (   ConstraintSame("body_sese.precursor", "loop*.body_sese.precursor")
                      && ConstraintSame("body_sese.begin",     "loop*.body_sese.begin")
                      && ConstraintSame("body_sese.end",       "loop*.body_sese.end")
                      && ConstraintSame("body_sese.successor", "loop*.body_sese.successor"));
}

ConstraintAnd ConstraintAffineAccess(unsigned dim)
{
    ConstraintAnd result = ConstraintArrayAccess            ()
                        && ConstraintRename(ConstraintExtendedInt(), {{"input","offset_add.value"},  {"output","output_index"}})
                        && "offset_add" + ConstraintAddition()
                        && ConstraintRename(ConstraintLocallyConstant(), {{"value","offset_add.addend"}, {"border","origin"}});

    for(unsigned i = 0; i < dim; i++)
    {
        result = std::move(result) &&
                          (dim-i-1) + (   ConstraintSame            ("stride_mul*.value", "index_add+.input")
                                       && "stride_mul*" + ConstraintMultiplication()
                                       && ConstraintRename(ConstraintLocallyConstant(), {{"value","stride_mul*.multiplier"}, {"border","origin"}})
                                       && ConstraintSame            ("index_add*.value",  "stride_mul*.input")
                                       && "index_add*" + ConstraintAddition()
                                       && ConstraintRename(ConstraintExtendedInt(), {{"input","input_index*"}, {"output","add_input*"}})
                                       && (   (   ConstraintDistinct("add_input*", "input_index+")
                                               && ConstraintIntegerType("input_index*"))
                                           || (   ConstraintSame       ("input_index*",             "add_input*")
                                               && ConstraintIntegerType("input_index+"))));
    }

    return ConstraintRename(std::move(result), {{"index_add["+std::string(1, '0'+dim)+"].input","offset_add.input"},
                                                {"input_index["+std::string(1, '0'+dim)+"]","offset_add.value"},
                                                {"add_input[0]","index_add[0].input"},
                                                {"add_input[1]","index_add[1].addend"},
                                                {"add_input[2]","index_add[2].addend"},
                                                {"add_input[3]","index_add[3].addend"},
                                                {"add_input[4]","index_add[4].addend"}})
        && ConstraintSame           ("base_index", "index_add[0].addend")
        && ConstraintRename(ConstraintLocallyConstant(), {{"value","base_index"}, {"border","origin"}});
}

ConstraintAnd ConstraintStencilAccess(unsigned dim)
{
    return ConstraintRename("read" + ConstraintAffineAccess(dim), {{"read.origin", "origin"}})

        && ConstraintSame("read.base_pointer",   "base_pointer")
        && ConstraintSame("read.access_pointer", "access_pointer")

        && dim * (   ConstraintSame               ("read.input_index*", "index*.value")
                  && "index*" + ConstraintAddition()
                  && (   ConstraintConstant       ("index*.addend")
                      || ConstraintUnused         ("index*.addend"))
                  && ConstraintSame               ("index*.input",      "input_index*"));
}

ConstraintAnd ConstraintPermMultidRead(unsigned dim)
{
    return ConstraintLoadInst                    ("value")
        && ConstraintFirstArgument               ("access_pointer", "value")
        && ConstraintAffineAccess(dim)
        && ConstraintRename(ConstraintPermute(dim), {{"output","input_index"}});
}

ConstraintAnd ConstraintPermMultidStore(unsigned dim)
{
    return ConstraintStoreInst                   ("store")
        && ConstraintSecondArgument              ("access_pointer", "store")
        && ConstraintAffineAccess(dim)
        && ConstraintRename(ConstraintPermute(dim), {{"output","input_index"}});
}

ConstraintAnd ConstraintStencilRead(unsigned dim)
{
    return ConstraintLoadInst     ("value")
        && ConstraintFirstArgument("access_pointer", "value")
        && ConstraintStencilAccess(dim)
        && ConstraintVectorSame(expand_variables("input_index", dim), expand_variables("input", dim));
}

ConstraintAnd ConstraintPureFunction()
{
    return std::make_pair("i",4) * ConstraintCDGEdge("origin[i]", "inner")

        && std::make_pair("i",64) * (   "constants[i]" + ConstraintRename(ConstraintLocallyConstant(), {{"border",nonlocal("outer")}})
                                     && ConstraintNotNumericConstant("constants[i].value")
                                     && ConstraintDFGEdge           ("constants[i].value", "constants[i].use")
                                     && ConstraintCFGDominate       ("inner", "constants[i].use"))

        && std::make_pair("i",64) * (   ConstraintCFGDominate      ("outer", "restrictions[i].value")
                                     && ConstraintCFGDominateStrict("restrictions[i].value", "inner")
                                     && ConstraintDFGEdge          ("restrictions[i].value",           "restrictions[i].use")
                                     && ConstraintCFGDominate      ("inner", "restrictions[i].use"))

        && ConstraintPDGDominate("restrictions[0..64].value",
                                 "origin[0..4],constants[0..64].value,input[0..32]", "output");
}

ConstraintAnd ConstraintScalarReductionBase()
{
    return ConstraintRename(ConstraintInductionVar(), {{"old_ind","old_value"},{"new_ind","update_expr.output"}})
        && ConstraintDistinct    ("old_value",           "iterator")

        && "update_expr" + ConstraintRename(ConstraintPureFunction(), {{"outer",nonlocal("begin")},
                                                                       {"inner",nonlocal("body_sese.begin")}})

        && std::make_pair("i",4) * (   ConstraintDFGEdge ("incoming[i]",          "old_value")
                                    && ConstraintDistinct("incoming[i]", "update_expr.output"));
}

ConstraintAnd ConstraintScalarReduction()
{
    return ConstraintFor()

        && std::make_pair("i",32) * (   "read[i]" + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("iterator")},
                                                                                                   {"origin",nonlocal("begin")}})
                                     && "body_sese" + ConstraintRename(ConstraintInSESE(), {{"value",nonlocal("read[i].value")}}))
        && ConstraintLoadInst("read[0].value")

        && ConstraintVectorSame("update_expr.input[0..32]", "old_value,read[0..31].value")
        && ConstraintScalarReductionBase();
}

ConstraintAnd ConstraintMatrixmatrix()
{
    return ConstraintForNest(3)

        && "output" + ConstraintRename(ConstraintPermMultidStore(2), {{"input[0]",nonlocal("loop[0].iterator")},
                                                                      {"input[1]",nonlocal("loop[1].iterator")},
                                                                      {"origin",nonlocal("begin")}})
        && "input1" + ConstraintRename(ConstraintPermMultidRead(2), {{"input[0]",nonlocal("loop[0].iterator")},
                                                                     {"input[1]",nonlocal("loop[2].iterator")},
                                                                     {"origin",nonlocal("begin")}})
        && "input2" + ConstraintRename(ConstraintPermMultidRead(2), {{"input[0]",nonlocal("loop[1].iterator")},
                                                                     {"input[1]",nonlocal("loop[2].iterator")},
                                                                     {"origin",nonlocal("begin")}})

//           This is correct but VectorSame has a bug.
//        && ConstraintVectorSame("loop[2].update_expr.input[0..32]", "loop[2].old_value,input1.value,input2.value")
        && ConstraintSame("loop[2].update_expr.input[0]", "loop[2].old_value")
        && ConstraintSame("loop[2].update_expr.input[1]", "input1.value")
        && ConstraintSame("loop[2].update_expr.input[2]", "input2.value")
        && ConstraintUnused("loop[2].update_expr.input[3]")
        && ConstraintUnused("loop[2].update_expr.input[4]")
        && ConstraintUnused("loop[2].update_expr.input[5]")
        && ConstraintUnused("loop[2].update_expr.input[6]")
        && ConstraintUnused("loop[2].update_expr.input[7]")
        && ConstraintUnused("loop[2].update_expr.input[8]")
        && ConstraintUnused("loop[2].update_expr.input[9]")
        && ConstraintUnused("loop[2].update_expr.input[10]")
        && ConstraintUnused("loop[2].update_expr.input[11]")
        && ConstraintUnused("loop[2].update_expr.input[12]")
        && ConstraintUnused("loop[2].update_expr.input[13]")
        && ConstraintUnused("loop[2].update_expr.input[14]")
        && ConstraintUnused("loop[2].update_expr.input[15]")
        && ConstraintUnused("loop[2].update_expr.input[16]")
        && ConstraintUnused("loop[2].update_expr.input[17]")
        && ConstraintUnused("loop[2].update_expr.input[18]")
        && ConstraintUnused("loop[2].update_expr.input[19]")
        && ConstraintUnused("loop[2].update_expr.input[20]")
        && ConstraintUnused("loop[2].update_expr.input[21]")
        && ConstraintUnused("loop[2].update_expr.input[22]")
        && ConstraintUnused("loop[2].update_expr.input[23]")
        && ConstraintUnused("loop[2].update_expr.input[24]")
        && ConstraintUnused("loop[2].update_expr.input[25]")
        && ConstraintUnused("loop[2].update_expr.input[26]")
        && ConstraintUnused("loop[2].update_expr.input[27]")
        && ConstraintUnused("loop[2].update_expr.input[28]")
        && ConstraintUnused("loop[2].update_expr.input[29]")
        && ConstraintUnused("loop[2].update_expr.input[30]")
        && ConstraintUnused("loop[2].update_expr.input[31]")
        && "loop[2]" + ConstraintScalarReductionBase()

        && (   (   ConstraintLoadInst("old_value")
                && ConstraintDFGEdge ("output.access_pointer", "old_value"))
            || (  ConstraintUnused   ("old_value")
                && ConstraintGEPInst ("output.access_pointer")))

        && ConstraintFirstArgument("update_expr.output", "output.store")
        && ConstraintVectorSame("update_expr.input[0..32]", "loop[2].old_value,old_value")
        && "update_expr" + ConstraintRename(ConstraintPureFunction(), {{"outer",nonlocal("begin")},
                                                                       {"inner",nonlocal("loop[1].body_sese.begin")}});
}

ConstraintAnd ConstraintSparseMV() 
{
    return ConstraintFor()

        && ConstraintUnused("begin_idx.offset_add.addend")
        && "begin_idx" + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("iterator")}, {"origin",nonlocal("begin")}})

        && ConstraintSame("begin_idx.base_index",   "end_idx.base_index")
        && ConstraintSame("begin_idx.base_pointer", "end_idx.base_pointer")
        && "end_idx"   + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("iterator")}, {"origin",nonlocal("begin")}})

        && ConstraintRename(ConstraintExtendedInt(), {{"input","begin_idx.value"}, {"output","reduc.iter_begin"}})
        && ConstraintRename(ConstraintExtendedInt(), {{"input","end_idx.value"}, {"output","reduc.iter_end"}})

        && "reduc" + (   ConstraintFor()
                      && "idx_read"   + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("iterator")}, {"origin",nonlocal("begin")}})
                      && "indir_read" + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("idx_read.value")}, {"origin",nonlocal("begin")}})
                      && "seq_read"   + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("iterator")}, {"origin",nonlocal("begin")}})

                      && ConstraintDistinct("idx_read.value", "seq_read.value")

                      && ConstraintVectorSame("update_expr.input[0..32]", "old_value,seq_read.value,indir_read.value")
                      && ConstraintScalarReductionBase());
}

ConstraintAnd ConstraintHistogram()
{
    return ConstraintFor()

        && ConstraintStoreInst     ("store_instr")
        && "body_sese" + ConstraintRename(ConstraintMaxOnceInSESE(), {{"value",nonlocal("store_instr")},
                                                                      {"value_after",nonlocal("store_instr_after")}})
        && ConstraintSecondArgument("index_expr.output",     "store_instr")
        && ConstraintGEPInst       ("index_expr.output")
        && ConstraintFirstArgument ("index_expr.output",       "old_value")
        && ConstraintLoadInst      ("old_value")
        && ConstraintFirstArgument ("update_expr.output",    "store_instr")
        && ConstraintFirstArgument ("reduction_array", "index_expr.output")

        && std::make_pair("i",30) * (   "read[i]"   + ConstraintRename(ConstraintPermMultidRead(1), {{"input[0]",nonlocal("iterator")},
                                                                                                     {"origin",nonlocal("begin")}})
                                     && "body_sese" + ConstraintRename(ConstraintInSESE(), {{"value",nonlocal("read[i].value")}})
                                     && ConstraintDistinct      ("read[i].base_pointer", "reduction_array"))
        && ConstraintLoadInst("read[0].value")

        // This feature is needed for tpacf to work.
        && std::make_pair("i",2) * (   "closure[i]" + ConstraintRename(ConstraintArrayRead(), {{"origin",nonlocal("begin")}})
                                    && "body_sese"  + ConstraintRename(ConstraintInSESE(), {{"value",nonlocal("closure[i].value")}})
                                    && ConstraintDistinct ("closure[i].base_pointer", "read[0].base_pointer")
                                    && ConstraintDistinct ("closure[i].base_pointer",      "reduction_array"))

        && ConstraintVectorSame("update_expr.input[0..32]", "read[0..29].value,closure[0..2].value,old_value")
        && "update_expr" + ConstraintRename(ConstraintPureFunction(), {{"outer",nonlocal("begin")},
                                                                       {"inner",nonlocal("body_sese.begin")}})
        && ConstraintVectorSame("index_expr.input[0..32]", "read[0..29].value,closure[0..2].value")
        && "index_expr"  + ConstraintRename(ConstraintPureFunction(), {{"outer",nonlocal("begin")},
                                                                       {"inner",nonlocal("body_sese.begin")}});
}

ConstraintAnd ConstraintStencil(unsigned dim)
{
    std::vector<std::pair<std::string,std::string>> with_dict1({{"origin",nonlocal("begin")}});
    std::vector<std::pair<std::string,std::string>> with_dict2({{"origin",nonlocal("begin")}});

    for(unsigned i = 0; i < dim; i++)
    {
        with_dict1.emplace_back("input["+std::string(1,'0'+i)+"]", nonlocal("loop["+std::string(1,'0'+i)+"].iterator"));
        with_dict2.emplace_back("input["+std::string(1,'0'+i)+"]", nonlocal("write.input_index["+std::string(1,'0'+i)+"]"));
    }

    return ConstraintForNest(dim)

        && "write" + ConstraintRename(ConstraintPermMultidStore(dim), with_dict1)

        && ConstraintFirstArgument("compute_expr.output", "write.store")

        && std::make_pair("i",32) * (   "reads[i]" + ConstraintRename(ConstraintStencilRead(dim), with_dict2))
        && ConstraintLoadInst("reads[0].value")

        && ConstraintVectorSame("compute_expr.input[0..32]", "reads[0..32].value")
        && "compute_expr" + ConstraintRename(ConstraintPureFunction(), {{"outer",nonlocal("begin")},
                                                                        {"inner",nonlocal("body_sese.begin")}});
}

ConstraintContainer ConstraintVectorSame(std::vector<std::string> in, std::vector<std::string> out)
{
    if(in.size() == 0 && out.size() == 0) return {};
    else if(in.size()  == 0) return ConstraintUnused(out[0])
                                 && ConstraintVectorSame({}, std::vector<std::string>{out.begin()+1, out.end()});
    else if(out.size() == 0) return ConstraintUnused(in[0]) 
                                 && ConstraintVectorSame(std::vector<std::string>{in.begin()+1, in.end()},   {});
    else return ConstraintSame(in[0], out[0])
             && ConstraintVectorSame(std::vector<std::string>{in.begin()+1, in.end()},
                                     std::vector<std::string>{out.begin()+1, out.end()});
}

ConstraintContainer ConstraintVectorSame(std::string a, std::string b)
{
    return ConstraintVectorSame(expand_variables(a), expand_variables(b));
}
