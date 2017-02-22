#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/ConstraintComplexUnused.hpp"
#include "llvm/Constraints/ConstraintComplexSame.hpp"
#include "llvm/Constraints/ConstraintSharedFate.hpp"
#include "llvm/Constraints/ConstraintCollect.hpp"
#include "llvm/Constraints/ConstraintsBasic.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"

ConstraintOr<std::string,unsigned> ConstraintPermute2(std::string in1,  std::string in2,
                                                      std::string out1, std::string out2)
{
    return (   (ConstraintSame(in1, out1) && ConstraintSame(in2, out2))
            || (ConstraintSame(in2, out1) && ConstraintSame(in1, out2)));
}

ConstraintOr<std::string,unsigned> ConstraintPermute3(std::string in1,  std::string in2,  std::string in3,
                                                      std::string out1, std::string out2, std::string out3)
{
    return (   (ConstraintSame(in1, out1) && ConstraintSame(in2, out2) && ConstraintSame(in3, out3))
            || (ConstraintSame(in1, out3) && ConstraintSame(in2, out1) && ConstraintSame(in3, out2))
            || (ConstraintSame(in1, out2) && ConstraintSame(in2, out3) && ConstraintSame(in3, out1))
            || (ConstraintSame(in1, out3) && ConstraintSame(in2, out2) && ConstraintSame(in3, out1))
            || (ConstraintSame(in1, out2) && ConstraintSame(in2, out1) && ConstraintSame(in3, out3))
            || (ConstraintSame(in1, out1) && ConstraintSame(in2, out3) && ConstraintSame(in3, out2)));
}

ConstraintAnd<std::string,unsigned> ConstraintBinaryChain(FunctionWrapper& wrap, unsigned Opcode, unsigned N)
{
    std::stringstream last_factor;
    last_factor<<"factors["<<N<<"]";

    std::stringstream last_result;
    last_result<<"results["<<N<<"]";

    return ( N * (   ConstraintOpcode            (wrap, Opcode, 2,       "results*")
                  && (   (   ConstraintDFGEdge0(wrap, "results+",        "results*")
                          && ConstraintDFGEdge1(wrap, "factors*",        "results*"))
                      || (   ConstraintDFGEdge1(wrap, "results+",        "results*")
                          && ConstraintDFGEdge0(wrap, "factors*",        "results*"))))
            && ConstraintSame                        (last_factor.str(), last_result.str()));
}

ConstraintAnd<std::string,unsigned> ConstraintDistributive(FunctionWrapper& wrap, unsigned SumL, unsigned SumR,
                                                                                  unsigned ProL, unsigned ProR)
{
    std::stringstream last_factor_L;
    last_factor_L<<"product1.factors["<<ProL<<"]";

    std::stringstream last_factor_R;
    last_factor_R<<"product2.factors["<<ProR<<"]";

    std::stringstream last_addend_L;
    last_addend_L<<"sum1.factors["<<SumL<<"]";

    std::stringstream last_addend_R;
    last_addend_R<<"sum2.factors["<<SumR<<"]";

    return (   ConstraintBinaryChain(wrap, llvm::Instruction::Add, 1)   +"last_sum"
            && ConstraintSame             ("sum1.results[0]",            "last_sum.factors[0]")
            && ConstraintBinaryChain(wrap, llvm::Instruction::Add, SumL)+"sum1"
            && ConstraintSame             ("product1.results[0]",        last_addend_L.str())
            && ConstraintBinaryChain(wrap, llvm::Instruction::Mul, ProL)+"product1"
            && ConstraintSame             ("sum2.results[0]",            "last_sum.factors[1]")
            && ConstraintBinaryChain(wrap, llvm::Instruction::Add, SumR)+"sum2"
            && ConstraintSame             ("product2.results[0]",        last_addend_R.str())
            && ConstraintBinaryChain(wrap, llvm::Instruction::Mul, ProR)+"product2"
            && ConstraintSame             (last_factor_L.str(),          last_factor_R.str())
            && ConstraintSame             ("last_sum.results[0]",        "sum"));
}

ConstraintAnd<std::string,unsigned> ConstraintHoistSelect(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode        (wrap, llvm::Instruction::Select,           "select")
            && ConstraintDFGEdge1      (wrap, "input1",                            "select")
            && ConstraintDFGEdge0      (wrap, "base",                              "input1")
            && ConstraintDFGEdge2      (wrap, "input2",                            "select")
            && ConstraintDFGEdge0      (wrap, "base",                              "input2")
            && (   (ConstraintOpcode   (wrap, llvm::Instruction::GetElementPtr, 2, "input1")
                    && ConstraintOpcode(wrap, llvm::Instruction::GetElementPtr, 2, "input2"))
                || (ConstraintOpcode   (wrap, llvm::Instruction::GetElementPtr, 3, "input1")
                    && ConstraintOpcode(wrap, llvm::Instruction::GetElementPtr, 3, "input2"))
                || (ConstraintOpcode   (wrap, llvm::Instruction::GetElementPtr, 4, "input1")
                    && ConstraintOpcode(wrap, llvm::Instruction::GetElementPtr, 4, "input2"))));
}

ConstraintAnd<std::string,unsigned> ConstraintPointerIterator(FunctionWrapper& wrap)
{
    return (   ConstraintLoop           (wrap)
            && ConstraintOpcode         (wrap, llvm::Instruction::PHI, 2,           "old_value")
            && ConstraintCFGDominate    (wrap, "begin",                             "old_value")
            && ConstraintCFGPostdom     (wrap, "old_value",                         "begin")
            && ConstraintDFGEdge        (wrap, "initial_value",                     "old_value")
            && ConstraintLocallyConstant(wrap, "initial_value",                     "")
            && ConstraintDFGEdge0       (wrap, "old_value",                         "new_value")
            && ConstraintOpcode         (wrap, llvm::Instruction::GetElementPtr, 2, "new_value")
            && ConstraintDFGEdge        (wrap, "new_value",                         "old_value")
            && ConstraintDFGEdge1       (wrap, "stride",                            "new_value")
            && ConstraintConstant       (wrap, "stride"));
}

ConstraintAnd<std::string,unsigned> ConstraintSESE(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode           (wrap, llvm::Instruction::Br, "precursor")
            && ConstraintCFGEdge          (wrap, "precursor",           "begin")
            && ConstraintOpcode           (wrap, llvm::Instruction::Br, "end")
            && ConstraintCFGEdge          (wrap, "end",                 "successor")
            && ConstraintCFGDominate      (wrap, "begin",               "end")
            && ConstraintCFGPostdom       (wrap, "end",                 "begin")
            && ConstraintCFGDominateStrict(wrap, "precursor",           "begin")
            && ConstraintCFGPostdomStrict (wrap, "successor",           "end")
            && ConstraintCFGBlocked       (wrap, "begin",               "end",   "precursor")
            && ConstraintCFGBlocked       (wrap, "successor",           "begin", "end"));
}

ConstraintOr<std::string,unsigned> ConstraintExtendedInt(FunctionWrapper& wrap, std::string input, std::string output)
{
    return ((ConstraintOpcode    (wrap, llvm::Instruction::ZExt, 1, output)
             && ConstraintDFGEdge(wrap, input, output))
         || (ConstraintOpcode    (wrap, llvm::Instruction::SExt, 1, output)
             && ConstraintDFGEdge(wrap, input, output))
         || ConstraintSame             (input, output));
}

ConstraintOr<std::string,unsigned> ConstraintLocallyConstant(FunctionWrapper& wrap, std::string var, std::string sese)
{
    return (   ConstraintPreexecution     (wrap, var)
            || ConstraintCFGDominateStrict(wrap, var, sese+"begin")
            || ConstraintUnused                 (var));
}

ConstraintAnd<std::string,unsigned> ConstraintLoop(FunctionWrapper& wrap)
{
    return (   ConstraintSESE  (wrap)
            && ConstraintOpcode(wrap, llvm::Instruction::Br, 3, "end")
            && ConstraintSame        ("body_sese.precursor",    "end")
            && ConstraintSame        ("body_sese.successor",    "begin")
            && ConstraintSESE  (wrap)+"body_sese"
            && ConstraintDistinct    ("body_sese.begin",        "successor"));
}

ConstraintAnd<std::string,unsigned> ConstraintInductionVar(FunctionWrapper& wrap, std::string prefix)
{
    if(!prefix.empty()) prefix = prefix + ".";

    return (   ConstraintOpcode     (wrap, llvm::Instruction::PHI, 2, prefix+"iterator")
            && ConstraintCFGDominate(wrap, "begin",                   prefix+"iterator")
            && ConstraintCFGPostdom (wrap, "end",                     prefix+"iterator")
            && ConstraintDFGEdge    (wrap, prefix+"increment",        prefix+"iterator")
            && ConstraintCFGDominate(wrap, "body_sese.begin",         prefix+"increment")
            && ConstraintCFGPostdom (wrap, "body_sese.end",           prefix+"increment")
            && ConstraintCFGDominate(wrap, prefix+"iterator",         prefix+"increment"));
}

ConstraintAnd<std::string,unsigned> ConstraintFor(FunctionWrapper& wrap)
{
    return (   ConstraintLoop           (wrap)
            && ConstraintInductionVar   (wrap, "")
            && ConstraintDFGEdge        (wrap, "iterator"       ,          "comparison")
            && ConstraintOpcode         (wrap, llvm::Instruction::ICmp, 2, "comparison")
            && ConstraintDFGEdge        (wrap, "comparison",               "end")
            && ConstraintDFGEdge        (wrap, "increment",                "iterator")
            && ConstraintOpcode         (wrap, llvm::Instruction::Add, 2,  "increment")
            && ConstraintDFGEdge        (wrap, "iterator",                 "increment")
            && ConstraintDFGEdge        (wrap, "iter_end",                 "comparison")
            && ConstraintLocallyConstant(wrap, "iter_end",                 "")
            && ConstraintDFGEdge        (wrap, "iter_begin",               "iterator")
            && ConstraintLocallyConstant(wrap, "iter_begin",               "")
            && ConstraintDFGEdge        (wrap, "iter_step",                "increment")
            && ConstraintLocallyConstant(wrap, "iter_step",                ""));
}

ConstraintAnd<std::string,unsigned> ConstraintNestedFor(FunctionWrapper& wrap, std::string prefix,
                                                        std::string parent)
{
    return (   ConstraintFor      (wrap)+prefix
            && ConstraintSame           (prefix+".pre_sese.precursor",  parent+".precursor")
            && ConstraintSame           (prefix+".pre_sese.begin",      parent+".begin")
            && ConstraintSame           (prefix+".pre_sese.end",        prefix+".precursor")
            && ConstraintSame           (prefix+".pre_sese.successor",  prefix+".begin")
            && (   ConstraintSESE(wrap)+(prefix+".pre_sese")
                || (   ConstraintSame   (prefix+".pre_sese.precursor",  prefix+".pre_sese.end")
                    && ConstraintSame   (prefix+".pre_sese.begin",      prefix+".pre_sese.successor")))
            && ConstraintSame           (prefix+".post_sese.precursor", prefix+".end")
            && ConstraintSame           (prefix+".post_sese.begin",     prefix+".successor")
            && ConstraintSame           (prefix+".post_sese.end",       parent+".end")
            && ConstraintSame           (prefix+".post_sese.successor", parent+".successor")
            && (   ConstraintSESE(wrap)+(prefix+".post_sese")
                || (   ConstraintSame   (prefix+".post_sese.precursor", prefix+".post_sese.end")
                    && ConstraintSame   (prefix+".post_sese.begin",     prefix+".post_sese.successor"))));
}

ConstraintOr<std::string,unsigned> ConstraintAddition(FunctionWrapper& wrap)
{
    return (   (   ConstraintSame                  ("input", "value")
                && ConstraintUnused                ("addend"))
            || (   (   ConstraintOpcode      (wrap, llvm::Instruction::Add, 2, "value")
                    || ConstraintOpcode      (wrap, llvm::Instruction::Sub, 2, "value"))
                && (   (   ConstraintDFGEdge0(wrap, "input" , "value")
                        && ConstraintDFGEdge1(wrap, "addend", "value"))
                    || (   ConstraintDFGEdge1(wrap, "input",  "value")
                        && ConstraintDFGEdge0(wrap, "addend", "value")))));
}

ConstraintOr<std::string,unsigned> ConstraintMultiplication(FunctionWrapper& wrap)
{
    return (   (   ConstraintSame                  ("input", "value")
                && ConstraintUnused                ("multiplier"))
            || (   ConstraintOpcode          (wrap, llvm::Instruction::Mul, 2, "value")
                && (   (   ConstraintDFGEdge0(wrap, "input" ,     "value")
                        && ConstraintDFGEdge1(wrap, "multiplier", "value"))
                    || (   ConstraintDFGEdge1(wrap, "input",      "value")
                        && ConstraintDFGEdge0(wrap, "multiplier", "value")))));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineAccess1(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode         (wrap, llvm::Instruction::GetElementPtr, "access_pointer")
            && ConstraintDFGEdge0       (wrap, "base_pointer",                   "access_pointer")
            && ConstraintLocallyConstant(wrap, "base_pointer",                   "sese.")
            && ConstraintDFGEdge1       (wrap, "output_index",                   "access_pointer")
            && ConstraintExtendedInt    (wrap, "offset_add.value",               "output_index")
            && ConstraintAddition       (wrap)+"offset_add"
            && ConstraintLocallyConstant(wrap, "offset_add.addend",              "sese.")
            && ConstraintSame                 ("stride_mul[0].value",            "offset_add.input")
            && ConstraintMultiplication (wrap)+"stride_mul[0]"
            && ConstraintLocallyConstant(wrap, "stride_mul[0].multiplier",       "sese.")
            && ConstraintSame                 ("index_add[0].value",             "stride_mul[0].input")
            && ConstraintAddition       (wrap)+"index_add[0]"
            && ConstraintLocallyConstant(wrap, "index_add[0].addend",            "sese.")
            && (   ConstraintDistinct         ("stride_mul[0].input",            "stride_mul[0].value")
                || ConstraintSame             ("index_add[0].input",             "index_add[0].value"))
            && ConstraintExtendedInt    (wrap, "input_index[0]",                 "index_add[0].input")
            && (   ConstraintDistinct         ("index_add[0].input",             "offset_add.value")
                || ConstraintSame             ("input_index[0]",                 "index_add[0].input"))
            && ConstraintSame                 ("base_index",                     "index_add[0].addend"));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineAccess2(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode        (wrap, llvm::Instruction::GetElementPtr, "access_pointer")
            && ConstraintDFGEdge0      (wrap, "base_pointer",                   "access_pointer")
            && ConstraintDFGEdge1      (wrap, "output_index",                   "access_pointer")
            && ConstraintExtendedInt   (wrap, "offset_add.value",               "output_index")
            && ConstraintAddition      (wrap)+"offset_add"
            && ConstraintSame                ("stride_mul[1].value",            "offset_add.input")
            && ConstraintMultiplication(wrap)+"stride_mul"
            && ConstraintSame                ("index_add[1].value",             "stride_mul[1].input")
            && ConstraintAddition      (wrap)+"index_add[1]"
            && ConstraintExtendedInt   (wrap, "input_index[1]",                 "index_add[1].addend")
            && (   ConstraintDistinct        ("index_add[1].input",             "offset_add.value")
                || ConstraintSame            ("input_index[1]",                 "index_add[1].input"))
            && ConstraintSame                ("stride_mul[0].value",            "index_add[1].input")
            && ConstraintMultiplication(wrap)+"stride_mul[0]"
            && ConstraintSame                ("index_add[0].value",             "stride_mul[0].input")
            && ConstraintAddition      (wrap)+"index_add[0]"
            && ConstraintExtendedInt   (wrap, "input_index[0]",                 "index_add[0].input")
            && (   ConstraintDistinct        ("index_add[0].input",             "input_index[1]")
                || ConstraintSame            ("input_index[0]",                 "index_add[0].input"))
            && ConstraintSame                ("base_index",                     "index_add[0].addend"));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineAccess3(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode        (wrap, llvm::Instruction::GetElementPtr, "access_pointer")
            && ConstraintDFGEdge0      (wrap, "base_pointer",                   "access_pointer")
            && ConstraintDFGEdge1      (wrap, "output_index",                   "access_pointer")
            && ConstraintExtendedInt   (wrap, "offset_add.value",               "output_index")
            && ConstraintAddition      (wrap)+"offset_add"
            && ConstraintSame                ("stride_mul[2].value",            "offset_add.input")
            && ConstraintMultiplication(wrap)+"stride_mul[2]"
            && ConstraintSame                ("index_add[2].value",             "stride_mul[2].input")
            && ConstraintAddition      (wrap)+"index_add[2]"
            && ConstraintExtendedInt   (wrap, "input_index[2]",                 "index_add[2].addend")
            && (   ConstraintDistinct        ("index_add[2].input",             "offset_add.value")
                || ConstraintSame            ("input_index[2]",                 "index_add[2].input"))
            && ConstraintSame                ("stride_mul[1].value",            "index_add[2].input")
            && ConstraintMultiplication(wrap)+"stride_mul[1]"
            && ConstraintSame                ("index_add[1].value",             "stride_mul[1].input")
            && ConstraintAddition      (wrap)+"index_add[1]"
            && ConstraintExtendedInt   (wrap, "input_index[1]",                 "index_add[1].addend")
            && (   ConstraintDistinct        ("index_add[1].input",             "input_index[2]")
                || ConstraintSame            ("input_index[1]",                 "index_add[1].input"))
            && ConstraintSame                ("stride_mul[0].value",            "index_add[1].input")
            && ConstraintMultiplication(wrap)+"stride_mul[0]"
            && ConstraintSame                ("index_add[0].value",             "stride_mul[0].input")
            && ConstraintAddition      (wrap)+"index_add[0]"
            && ConstraintExtendedInt   (wrap, "input_index[0]",                 "index_add[0].input")
            && (   ConstraintDistinct        ("index_add[0].input",             "input_index[1]")
                || ConstraintSame            ("input_index[0]",                 "index_add[0].input"))
            && ConstraintSame                ("base_index",                     "index_add[0].addend"));
}

ConstraintAnd<std::string,unsigned> ConstraintStencilAccess3(FunctionWrapper& wrap)
{
    return (   ConstraintAffineAccess3(wrap)+"read"
            && ConstraintAddition     (wrap)+"index[0]"
            && (   ConstraintConstant (wrap, "index[0].addend")
                || ConstraintUnused         ("index[0].addend"))
            && ConstraintSame               ("read.input_index[0]", "index[0].value")
            && ConstraintAddition     (wrap)+"index[1]"
            && (   ConstraintConstant (wrap, "index[1].addend")
                || ConstraintUnused         ("index[1].addend"))
            && ConstraintSame               ("read.input_index[1]", "index[1].value")
            && ConstraintAddition     (wrap)+"index[2]"
            && (   ConstraintConstant (wrap, "index[2].addend")
                || ConstraintUnused         ("index[2].addend"))
            && ConstraintSame               ("read.input_index[2]", "index[2].value"));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineRead1(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Load, "value")
            && ConstraintDFGEdge0     (wrap, "access_pointer",        "value")
            && ConstraintAffineAccess1(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineRead2(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Load, "value")
            && ConstraintDFGEdge0     (wrap, "access_pointer",        "value")
            && ConstraintAffineAccess2(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineRead3(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Load, "value")
            && ConstraintDFGEdge0     (wrap, "access_pointer",        "value")
            && ConstraintAffineAccess3(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineStore1(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Store, "store")
            && ConstraintDFGEdge1     (wrap, "access_pointer",         "store")
            && ConstraintAffineAccess1(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineStore2(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Store, "store")
            && ConstraintDFGEdge1     (wrap, "access_pointer",         "store")
            && ConstraintAffineAccess2(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineStore3(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Store, "store")
            && ConstraintDFGEdge1     (wrap, "access_pointer",         "store")
            && ConstraintAffineAccess3(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintScale(FunctionWrapper& wrap, std::string prefix,
                                                                           std::string history,
                                                                           std::string input)
{
    return (   ConstraintSame                               (prefix+"history",           history)
            && ConstraintSame                               (prefix+"input",             input)
            && (   ( ConstraintOpcode                 (wrap, llvm::Instruction::FMul, 2, prefix+"output")
                    && ConstraintDFGEdge              (wrap, prefix+"input",             prefix+"output")
                    && ConstraintDFGEdge              (wrap, prefix+"factor",            prefix+"output")
                    && ConstraintDistinct                   (prefix+"input",             prefix+"factor")
                    && (   ConstraintPreexecution     (wrap, prefix+"factor")
                        || ConstraintCFGDominateStrict(wrap, prefix+"factor",            prefix+"history")))
                || (    ConstraintUnused                    (prefix+"factor")
                     && ConstraintSame                      (prefix+"input",             prefix+"output"))
                || (    ConstraintUnused                    (prefix+"output")
                     && ConstraintUnused                    (prefix+"input")
                     && ConstraintUnused                    (prefix+"factor"))));
}

ConstraintAnd<std::string,unsigned> ConstraintWeightedSum(FunctionWrapper& wrap, std::string prefix,
                                                                                 std::string history,
                                                                                 std::string input1,
                                                                                 std::string input2)
{
    return (   (   (   ConstraintOpcode (wrap, llvm::Instruction::FAdd, 2,    prefix+"output")
                    && ConstraintDFGEdge(wrap, prefix+"constr_scale1.output", prefix+"output")
                    && ConstraintDFGEdge(wrap, prefix+"constr_scale2.output", prefix+"output")
                    && ConstraintDistinct     (prefix+"constr_scale1.output", prefix+"constr_scale2.output"))
                || (   ConstraintUnused       (prefix+"constr_scale1.output")
                    && ConstraintSame         (prefix+"constr_scale2.output", prefix+"output"))
                || (   ConstraintUnused       (prefix+"constr_scale2.output")
                    && ConstraintSame         (prefix+"constr_scale1.output", prefix+"output"))
                || (   ConstraintUnused       (prefix+"output")
                    && ConstraintUnused       (prefix+"constr_scale1.output")
                    && ConstraintUnused       (prefix+"constr_scale2.output")))
            && ConstraintScale      (wrap, prefix+"constr_scale1.",       history, input1)
            && ConstraintScale      (wrap, prefix+"constr_scale2.",       history, input2));
}

ConstraintAnd<std::string,unsigned> ConstraintSumReduction(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode               (wrap, llvm::Instruction::PHI, 2,  "old_value")
            && ConstraintOpcode               (wrap, llvm::Instruction::FAdd, 2, "new_value")
            && ConstraintOpcode               (wrap, llvm::Instruction::PHI, 1,  "final_value")
            && ConstraintDFGEdge              (wrap, "initial_value",            "old_value")
            && ConstraintDFGEdge              (wrap, "new_value",                "old_value")
            && ConstraintDFGEdge              (wrap, "old_value",                "new_value")
            && ConstraintDFGEdge              (wrap, "increment",                "new_value")
            && ConstraintDistinct                   ("old_value",                "increment")
            && ConstraintDFGEdge              (wrap, "old_value",                "final_value")
            && (   ConstraintPreexecution     (wrap, "initial_value")
                || ConstraintCFGDominateStrict(wrap, "initial_value",            "old_value"))
            && ConstraintCFGPostdomStrict     (wrap, "final_value",              "new_value"));
}

ConstraintAnd<std::string,unsigned> ConstraintMatrixmatrix(FunctionWrapper& wrap)
{
    return (   ConstraintFor            (wrap)+"loop1"
            && ConstraintNestedFor      (wrap, "loop2",                     "loop1.body_sese")
            && ConstraintLocallyConstant(wrap, "loop2.iter_begin",          "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_step",           "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_end",            "loop1.")
            && ConstraintPermute2             ("loop1.iterator",            "loop2.iterator",
                                               "output.input_index[0]",     "output.input_index[1]")
            && ConstraintOpcode         (wrap, llvm::Instruction::Store, 2, "store_instr")
            && ConstraintDFGEdge        (wrap, "output.access_pointer",     "store_instr")
            && ConstraintDFGEdge        (wrap, "weighted_sum.output",       "store_instr")
            && ConstraintAffineAccess2  (wrap)+"output"
            && ConstraintLocallyConstant(wrap, "output.vector",             "loop1.")
            && ConstraintNestedFor      (wrap, "loop3",                     "loop2.body_sese")
            && ConstraintLocallyConstant(wrap, "loop3.iter_begin",          "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_step",           "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_end",            "loop1.")
            && ConstraintPermute2             ("loop1.iterator",            "loop3.iterator",
                                               "input1.input_index[0]",     "input1.input_index[1]")
            && ConstraintAffineRead2    (wrap)+"input1"
            && ConstraintLocallyConstant(wrap, "input1.vector",             "loop1.")
            && ConstraintPermute2             ("loop2.iterator",            "loop3.iterator",
                                               "input2.input_index[0]",     "input2.input_index[1]")
            && ConstraintAffineRead2    (wrap)+"input2"
            && ConstraintLocallyConstant(wrap, "input2.vector",             "loop1.")
            && ConstraintOpcode         (wrap, llvm::Instruction::FMul, 2,  "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "input1.value",              "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "input2.value",              "dot_sum.increment")
            && ConstraintSumReduction   (wrap)+"dot_sum"
            && ConstraintWeightedSum    (wrap, "weighted_sum.",             "loop1.precursor",
                                               "dot_sum.final_value",       "old_value")
            && (   (   ConstraintOpcode (wrap, llvm::Instruction::Load, 1,  "old_value")
                    && ConstraintDFGEdge(wrap, "output.access_pointer",     "old_value"))
                || ConstraintUnused           ("old_value")));
}

ConstraintAnd<std::string,unsigned> ConstraintSparseMV(FunctionWrapper& wrap)
{
    return (   ConstraintFor            (wrap)+"loop1"
            && ConstraintSame                 ("range_begin_read.input_index[0]", "loop1.iterator")
            && ConstraintAffineRead1    (wrap)+"range_begin_read"
            && ConstraintDFGEdge0       (wrap, "loop1.iterator", "range_end_read.input_index[0]")
            && ConstraintOpcode         (wrap,  llvm::Instruction::Add, 2, "range_end_read.input_index[0]")
            && ConstraintAffineRead1    (wrap)+"range_end_read"
            && ConstraintNestedFor      (wrap, "loop2", "loop1.body_sese")
            && ConstraintLocallyConstant(wrap, "loop2.iter_begin", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_step", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_end", "loop1.")
            && ConstraintSame                 ("loop2.iter_begin", "range_begin_read.value")
            && ConstraintSame                 ("loop2.iter_end", "range_end_read.value")
            && ConstraintSame                 ("input_matrix.input_index[0]", "loop2.iterator")
            && ConstraintAffineRead1    (wrap)+"input_matrix"
            && ConstraintSame                 ("input_index.input_index[0]", "loop2.iterator")
            && ConstraintAffineRead1    (wrap)+"input_index"
            && ConstraintSame                 ("input_vector.input_index[0]", "input_index.input_index[0]")
            && ConstraintAffineRead1    (wrap)+"input_vector"
            && ConstraintOpcode         (wrap, llvm::Instruction::FMul, 2, "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "input_matrix.value", "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "intput_vector.value", "dot_sum.increment")
            && ConstraintSumReduction   (wrap)+"dot_sum"
            && ConstraintSame                 ("output.input_index[0]", "loop1.iterator")
            && ConstraintAffineAccess1  (wrap)+"output"
            && ConstraintOpcode         (wrap, llvm::Instruction::Store, 2, "store_instr")
            && ConstraintDFGEdge0       (wrap, "dot_sum.final_value", "store_instr"));
}

ConstraintAnd<std::string,unsigned> ConstraintPureFunction(FunctionWrapper& wrap)
{
    std::vector<unsigned> origins;

    for(unsigned i = 0; i < wrap.rpdg.size(); i++)
    {
        if(auto call_inst = llvm::dyn_cast<llvm::CallInst>(wrap.get_value(i)))
        {
            if(auto function = llvm::dyn_cast<llvm::Function>(call_inst->getCalledValue()))
            {
                if(function->getName() != "sqrt" &&
                   function->getName() != "log" &&
                   function->getName() != "fabs")
                {
                    origins.push_back(i);
                }
            }
        }
        else if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
           wrap.opcodes[i] == llvm::Instruction::Load ||
           llvm::isa<llvm::Argument>(wrap.get_value(i)))
        {
            origins.push_back(i);
        }
    }

    return (   ConstraintSESE       (wrap)+"outer_sese"
            && ConstraintSESE       (wrap)+"inner_sese"
            && ConstraintCFGDominate(wrap, "outer_sese.begin", "inner_sese.begin")
            && ConstraintCFGPostdom (wrap, "outer_sese.end",   "inner_sese.end")

            && ConstraintCollect (4, "control_history", ConstraintCDGEdge        (wrap, "", "@inner_sese.begin"))
            && ConstraintCollect(64, "constants",       ConstraintLocallyConstant(wrap, "", "@outer_sese."))

            && ConstraintDominate<std::string,false>(wrap.pdg, wrap.rpdg,
                 origins,
                 {"restrictions[0]", "restrictions[1]", "restrictions[2]", "restrictions[3]"},
                 {},
                 {"control_history[0]", "control_history[1]", "control_history[2]",
                  "control_history[3]",
                  "input[0]",  "input[1]",  "input[2]",  "input[3]",  "input[4]",  "input[5]",  "input[6]",
                  "input[7]",  "input[8]",  "input[9]",  "input[10]", "input[11]", "input[12]", "input[13]",
                  "input[14]", "input[15]", "input[16]", "input[17]", "input[18]", "input[19]", "input[20]",
                  "input[21]", "input[22]", "input[23]", "input[24]", "input[25]", "input[26]", "input[27]",
                  "input[28]", "input[29]", "input[30]", "input[31]",
                  "constants[0]",  "constants[1]",  "constants[2]",  "constants[3]",  "constants[4]",
                  "constants[5]",  "constants[6]",  "constants[7]",  "constants[8]",  "constants[9]",
                  "constants[10]", "constants[11]", "constants[12]", "constants[13]", "constants[14]",
                  "constants[15]", "constants[16]", "constants[17]", "constants[18]", "constants[19]",
                  "constants[20]", "constants[21]", "constants[22]", "constants[23]", "constants[24]",
                  "constants[25]", "constants[26]", "constants[27]", "constants[28]", "constants[29]",
                  "constants[30]", "constants[31]", "constants[32]", "constants[33]", "constants[34]",
                  "constants[35]", "constants[36]", "constants[37]", "constants[38]", "constants[39]",
                  "constants[40]", "constants[41]", "constants[42]", "constants[43]", "constants[44]",
                  "constants[45]", "constants[46]", "constants[47]", "constants[48]", "constants[49]",
                  "constants[50]", "constants[51]", "constants[52]", "constants[53]", "constants[54]",
                  "constants[55]", "constants[56]", "constants[57]", "constants[58]", "constants[59]",
                  "constants[60]", "constants[61]", "constants[62]", "constants[63]"},
                  {}, {"output"}, true));
}

ConstraintAnd<std::string,unsigned> ConstraintScalarReduction(FunctionWrapper& wrap)
{
    return (   ConstraintFor             (wrap)
            && ConstraintOpcode          (wrap, llvm::Instruction::PHI, 2, "old_value")
            && ConstraintCFGDominate     (wrap, "begin",                   "old_value")
            && ConstraintCFGDominate     (wrap, "old_value",               "end")
            && ConstraintDFGReachable    (wrap, "old_value",               "final_value")
            && ConstraintDFGEdge         (wrap, "old_value",               "final_value")
            && ConstraintCFGPostdomStrict(wrap, "final_value",             "end")
            && ConstraintDFGEdge         (wrap, "initial_value",           "old_value")
            && ConstraintLocallyConstant (wrap, "initial_value",           "")
            && ConstraintDFGEdge         (wrap, "update_expr.output",      "old_value")
            && ConstraintDFGReachable    (wrap, "old_value",               "update_expr.output")
            && ConstraintCFGDominate     (wrap, "body_sese.begin",         "update_expr.output")
            && ConstraintCFGPostdom      (wrap, "body_sese.end",           "update_expr.output")
            && ConstraintCFGEdge         (wrap, "update_expr.output",      "post_store_instruction")
            && ConstraintCFGBlocked      (wrap, "post_store_instruction",  "end", "update_expr.output")
            && ConstraintComplexSame         ("update_expr.outer_sese",    "",          ConstraintSESE(wrap))
            && ConstraintComplexSame         ("update_expr.inner_sese",    "body_sese", ConstraintSESE(wrap))

            && ConstraintCollect(31, "read",
                                 ConstraintAffineRead1(wrap)
                              && ConstraintSame       ("input_index[0]", "@iterator")
                              && ConstraintSame       ("sese.begin",     "@begin"))

            && 31 * ConstraintSame("update_expr.input*",    "read*.value")
            && ConstraintSame     ("update_expr.input[31]", "old_value")
            && ConstraintUnused   ("update_expr.input[32]")

            && ConstraintCollect(4, "update_expr.restrictions",
                                 ConstraintOpcode           (wrap, llvm::Instruction::PHI, "")
                              && ConstraintCFGDominate      (wrap, "@begin",               "")
                              && ConstraintCFGDominateStrict(wrap, "", "@body_sese.begin")
                              && ConstraintDistinct               ("", "@old_value"))

            && ConstraintPureFunction(wrap)+"update_expr");
}

ConstraintAnd<std::string,unsigned> ConstraintHistogram(FunctionWrapper& wrap)
{
    return (   ConstraintFor        (wrap)
            && ConstraintOpcode     (wrap, llvm::Instruction::Store, 2,      "store_instr")
            && ConstraintDFGEdge1   (wrap, "index_expr.output",              "store_instr")
            && ConstraintDFGEdge0   (wrap, "update_expr.output",             "store_instr")

            && ConstraintDFGEdge0   (wrap, "index_expr.output",              "old_value")
            && ConstraintOpcode     (wrap, llvm::Instruction::Load, 1,       "old_value")

            && ConstraintCFGDominate(wrap, "body_sese.begin",                "store_instr")
            && ConstraintCFGPostdom (wrap, "body_sese.end",                  "store_instr")
            && ConstraintCFGEdge    (wrap, "store_instr",                    "post_store_instruction")
            && ConstraintCFGBlocked (wrap, "post_store_instruction",         "end", "store_instr")

            && ConstraintOpcode     (wrap, llvm::Instruction::GetElementPtr, "index_expr.output")
            && ConstraintDFGEdge0   (wrap, "reduction_array",                "index_expr.output")

            && ConstraintComplexSame      ("update_expr.outer_sese",         "",          ConstraintSESE(wrap))
            && ConstraintComplexSame      ("update_expr.inner_sese",         "body_sese", ConstraintSESE(wrap))

            && ConstraintComplexSame      ("index_expr.outer_sese",          "",          ConstraintSESE(wrap))
            && ConstraintComplexSame      ("index_expr.inner_sese",          "body_sese", ConstraintSESE(wrap))

            && ConstraintOpcode(wrap, llvm::Instruction::GetElementPtr, "read[0].access_pointer")

            && ConstraintCollect(29, "read",
                                 ConstraintAffineRead1(wrap)
                              && ConstraintSame       ("input_index[0]", "@iterator")
                              && ConstraintSame       ("sese.begin",     "@begin"))

            && ConstraintCollect(2, "closure",                // This is quite a hack to make tpacf work
                                 ConstraintOpcode     (wrap, llvm::Instruction::Load, 1,       "read")
                              && ConstraintDFGEdge    (wrap, "pointer",                        "read")
                              && ConstraintOpcode     (wrap, llvm::Instruction::GetElementPtr, "pointer")
                              && ConstraintDFGEdge0   (wrap, "vector",                         "pointer")
                              && ConstraintCFGDominate(wrap, "@body_sese.begin",               "read")
                              && ConstraintCFGPostdom (wrap, "@body_sese.end",                 "read")
                              && ConstraintDistinct         ("vector",                         "@read[0].base_pointer")
                              && ConstraintDistinct         ("vector",                         "@reduction_array"))

            && 29 * ConstraintSame("update_expr.input*",    "read*.value")
            && ConstraintSame     ("update_expr.input[29]", "closure[0].read")
            && ConstraintSame     ("update_expr.input[30]", "closure[1].read")
            && ConstraintSame     ("update_expr.input[31]", "old_value")
            && ConstraintUnused   ("update_expr.input[32]")

            && ConstraintCollect(4, "update_expr.restrictions",
                                 ConstraintOpcode           (wrap, llvm::Instruction::PHI, "")
                              && ConstraintCFGDominate      (wrap, "@begin", "")
                              && ConstraintCFGDominateStrict(wrap, "", "@body_sese.begin")
                              && ConstraintDistinct               ("", "@old_value"))

            && ConstraintPureFunction(wrap)+"update_expr"

            && 29 * ConstraintSame("index_expr.input*",    "read*.value")
            && ConstraintSame     ("index_expr.input[29]", "closure[0].read")
            && ConstraintSame     ("index_expr.input[30]", "closure[1].read")
            && ConstraintUnused   ("index_expr.input[31]")
            && ConstraintUnused   ("index_expr.input[32]")

            && ConstraintCollect(4, "index_expr.restrictions",
                                 ConstraintOpcode           (wrap, llvm::Instruction::PHI, "")
                              && ConstraintCFGDominate      (wrap, "@begin",               "")
                              && ConstraintCFGDominateStrict(wrap, "", "@body_sese.begin"))

            && ConstraintPureFunction(wrap)+"index_expr");
}

ConstraintAnd<std::string,unsigned> Constraint3DStencil(FunctionWrapper& wrap)
{
    return (   ConstraintFor            (wrap)+"loop1"
            && ConstraintNestedFor      (wrap, "loop2",            "loop1.body_sese")
            && ConstraintLocallyConstant(wrap, "loop2.iter_begin", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_step",  "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_end",   "loop1.")
            && ConstraintNestedFor      (wrap, "loop3",            "loop2.body_sese")
            && ConstraintLocallyConstant(wrap, "loop3.iter_begin", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_step",  "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_end",   "loop1.")

            && ConstraintAffineStore3   (wrap)+"write"
            && ConstraintPermute3             ("loop1.iterator",       "loop2.iterator",       "loop3.iterator",
                                               "write.input_index[0]", "write.input_index[1]", "write.input_index[2]")

            && ConstraintCollect(32, "reads",
                                 ConstraintOpcode         (wrap, llvm::Instruction::Load, 1, "value")
                              && ConstraintDFGEdge0       (wrap, "read.access_pointer", "value")
                              && ConstraintStencilAccess3(wrap)
                              && ConstraintSame                  ("index[0].input", "@write.input_index[0]")
                              && ConstraintSame                  ("index[1].input", "@write.input_index[1]")
                              && ConstraintSame                  ("index[2].input", "@write.input_index[2]"))

            && 33 * ConstraintSame("compute_expr.input*", "reads*.value")

            && ConstraintDFGEdge0    (wrap, "compute_expr.output", "write.store")

            && ConstraintComplexSame       ("compute_expr.outer_sese", "loop1",           ConstraintSESE(wrap))
            && ConstraintComplexSame       ("compute_expr.inner_sese", "loop3.body_sese", ConstraintSESE(wrap))

            && ConstraintUnused            ("compute_expr.restrictions[0]")
            && ConstraintUnused            ("compute_expr.restrictions[1]")
            && ConstraintUnused            ("compute_expr.restrictions[2]")
            && ConstraintUnused            ("compute_expr.restrictions[3]")
            && ConstraintUnused            ("compute_expr.restrictions[4]")
            && ConstraintPureFunction(wrap)+"compute_expr");
}
