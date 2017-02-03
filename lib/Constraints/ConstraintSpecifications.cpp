#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/ConstraintSharedFate.hpp"
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

    return (   ConstraintBinaryChain(wrap, llvm::Instruction::Add, 1)   +"last_sum."
            && ConstraintSame             ("sum1.results[0]",            "last_sum.factors[0]")
            && ConstraintBinaryChain(wrap, llvm::Instruction::Add, SumL)+"sum1."
            && ConstraintSame             ("product1.results[0]",        last_addend_L.str())
            && ConstraintBinaryChain(wrap, llvm::Instruction::Mul, ProL)+"product1."
            && ConstraintSame             ("sum2.results[0]",            "last_sum.factors[1]")
            && ConstraintBinaryChain(wrap, llvm::Instruction::Add, SumR)+"sum2."
            && ConstraintSame             ("product2.results[0]",        last_addend_R.str())
            && ConstraintBinaryChain(wrap, llvm::Instruction::Mul, ProR)+"product2."
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

ConstraintAnd<std::string,unsigned> ConstraintSESE(FunctionWrapper& wrap)
{
    return (   ConstraintCFGEdge          (wrap, "precursor", "begin")
            && ConstraintCFGEdge          (wrap, "end",       "successor")
            && ConstraintCFGDominate      (wrap, "begin",     "end")
            && ConstraintCFGPostdom       (wrap, "end",       "begin")
            && ConstraintCFGDominateStrict(wrap, "precursor", "begin")
            && ConstraintCFGPostdomStrict (wrap, "successor", "end")
            && ConstraintCFGBlocked       (wrap, "begin",     "end",   "precursor")
            && ConstraintCFGBlocked       (wrap, "successor", "begin", "end"));
}

ConstraintAnd<std::string,unsigned> ConstraintSameSESE(std::string prefix1, std::string prefix2)
{
    return (ConstraintSame(prefix1+"precursor", prefix2+"precursor")
         && ConstraintSame(prefix1+"begin",     prefix2+"begin")
         && ConstraintSame(prefix1+"end",       prefix2+"end")
         && ConstraintSame(prefix1+"successor", prefix2+"successor"));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedSESE()
{
    return (ConstraintUnused("precursor")
         && ConstraintUnused("begin")
         && ConstraintUnused("end")
         && ConstraintUnused("successor"));
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
    return (   ConstraintSame          ("body_sese.successor",    "begin")
            && ConstraintOpcode  (wrap, llvm::Instruction::Br, 3, "end")
            && ConstraintSame          ("body_sese.precursor",    "end")
            && ConstraintSESE    (wrap)
            && ConstraintDistinct      ("body_sese.begin",        "successor")
            && ConstraintOpcode  (wrap, llvm::Instruction::Br,    "body_sese.end")
            && ConstraintSESE    (wrap)+"body_sese.");
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedLoop()
{
    return (ConstraintUnusedSESE() && ConstraintUnusedSESE()+"body_sese.");
}

ConstraintAnd<std::string,unsigned> ConstraintFor(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode         (wrap, llvm::Instruction::PHI,     "iterator")
            && ConstraintDFGEdge        (wrap, "iterator"       ,          "comparison")
            && ConstraintOpcode         (wrap, llvm::Instruction::ICmp, 2, "comparison")
            && ConstraintDFGEdge        (wrap, "comparison",               "end")
            && ConstraintDFGEdge        (wrap, "increment",                "iterator")
            && ConstraintOpcode         (wrap, llvm::Instruction::Add, 2,  "increment")
            && ConstraintDFGEdge        (wrap, "iterator",                 "increment")
            && ConstraintLoop           (wrap)
            && ConstraintDFGEdge        (wrap, "iter_end",   "comparison")
            && ConstraintLocallyConstant(wrap, "iter_end",   "")
            && ConstraintDFGEdge        (wrap, "iter_begin", "iterator")
            && ConstraintLocallyConstant(wrap, "iter_begin", "")
            && ConstraintDFGEdge        (wrap, "iter_step",  "increment")
            && ConstraintLocallyConstant(wrap, "iter_step",  ""));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedFor()
{
    return (   ConstraintUnused    ("iterator")
            && ConstraintUnused    ("comparison")
            && ConstraintUnused    ("end")
            && ConstraintUnused    ("increment")
            && ConstraintUnusedLoop()
            && ConstraintUnused    ("iter_end")
            && ConstraintUnused    ("iter_begin")
            && ConstraintUnused    ("iter_step"));
}

ConstraintAnd<std::string,unsigned> ConstraintNestedFor(FunctionWrapper& wrap, std::string prefix,
                                                        std::string parent)
{
    return (   ConstraintFor      (wrap)+prefix
            && ConstraintSame           (prefix+"pre_sese.precursor",  parent+"precursor")
            && ConstraintSame           (prefix+"pre_sese.begin",      parent+"begin")
            && ConstraintSame           (prefix+"pre_sese.end",        prefix+"precursor")
            && ConstraintSame           (prefix+"pre_sese.successor",  prefix+"begin")
            && (   ConstraintSESE(wrap)+(prefix+"pre_sese.")
                || (   ConstraintSame   (prefix+"pre_sese.precursor",  prefix+"pre_sese.end")
                    && ConstraintSame   (prefix+"pre_sese.begin",      prefix+"pre_sese.successor")))
            && ConstraintSame           (prefix+"post_sese.precursor", prefix+"end")
            && ConstraintSame           (prefix+"post_sese.begin",     prefix+"successor")
            && ConstraintSame           (prefix+"post_sese.end",       parent+"end")
            && ConstraintSame           (prefix+"post_sese.successor", parent+"successor")
            && (   ConstraintSESE(wrap)+(prefix+"post_sese.")
                || (   ConstraintSame   (prefix+"post_sese.precursor", prefix+"post_sese.end")
                    && ConstraintSame   (prefix+"post_sese.begin",     prefix+"post_sese.successor"))));
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

ConstraintAnd<std::string,unsigned> ConstraintUnusedAddition()
{
    return (   ConstraintUnused("value")
            && ConstraintUnused("input")
            && ConstraintUnused("addend"));
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

ConstraintAnd<std::string,unsigned> ConstraintUnusedMultiplication()
{
    return (  ConstraintUnused("value")
            && ConstraintUnused("input")
            && ConstraintUnused("multiplier"));
}

ConstraintAnd<std::string,unsigned> ConstraintAffineAccess1(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode        (wrap, llvm::Instruction::GetElementPtr, "access_pointer")
            && ConstraintDFGEdge0      (wrap, "base_pointer",                   "access_pointer")
            && ConstraintDFGEdge1      (wrap, "output_index",                   "access_pointer")
            && ConstraintExtendedInt   (wrap, "offset_add.value",               "output_index")
            && ConstraintAddition      (wrap)+"offset_add."
            && ConstraintSame                ("stride_mul[0].value",            "offset_add.input")
            && ConstraintMultiplication(wrap)+"stride_mul[0]."
            && ConstraintSame                ("index_add[0].value",             "stride_mul[0].input")
            && ConstraintAddition      (wrap)+"index_add[0]."
            && (   ConstraintDistinct        ("stride_mul[0].input",            "stride_mul[0].value")
                || ConstraintSame            ("index_add[0].input",             "index_add[0].value"))
            && ConstraintExtendedInt   (wrap, "input_index[0]",                 "index_add[0].input")
            && (   ConstraintDistinct        ("index_add[0].input",             "offset_add.value")
                || ConstraintSame            ("input_index[0]",                 "index_add[0].input"))
            && ConstraintSame                ("base_index",                     "index_add[0].addend"));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineAccess1()
{
    return (   ConstraintUnused                ("access_pointer")
            && ConstraintUnused                ("base_pointer")
            && ConstraintUnused                ("output_index")
            && ConstraintUnused                ("input_index[0]")
            && ConstraintUnused                ("base_index")
            && ConstraintUnusedAddition      ()+"index_add[0]."
            && ConstraintUnusedMultiplication()+"stride_mul[0]."
            && ConstraintUnusedAddition      ()+"offset_add.");
}

ConstraintAnd<std::string,unsigned> ConstraintAffineAccess2(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode        (wrap, llvm::Instruction::GetElementPtr, "access_pointer")
            && ConstraintDFGEdge0      (wrap, "base_pointer",                   "access_pointer")
            && ConstraintDFGEdge1      (wrap, "output_index",                   "access_pointer")
            && ConstraintExtendedInt   (wrap, "offset_add.value",               "output_index")
            && ConstraintAddition      (wrap)+"offset_add."
            && ConstraintSame                ("stride_mul[1].value",            "offset_add.input")
            && ConstraintMultiplication(wrap)+"stride_mul."
            && ConstraintSame                ("index_add[1].value",             "stride_mul[1].input")
            && ConstraintAddition      (wrap)+"index_add[1]."
            && ConstraintExtendedInt   (wrap, "input_index[1]",                 "index_add[1].addend")
            && (   ConstraintDistinct        ("index_add[1].input",             "offset_add.value")
                || ConstraintSame            ("input_index[1]",                 "index_add[1].input"))
            && ConstraintSame                ("stride_mul[0].value",            "index_add[1].input")
            && ConstraintMultiplication(wrap)+"stride_mul[0]."
            && ConstraintSame                ("index_add[0].value",             "stride_mul[0].input")
            && ConstraintAddition      (wrap)+"index_add[0]"
            && ConstraintExtendedInt   (wrap, "input_index[0]",                 "index_add[0].input")
            && (   ConstraintDistinct        ("index_add[0].input",             "input_index[1]")
                || ConstraintSame            ("input_index[0]",                 "index_add[0].input"))
            && ConstraintSame                ("base_index",                     "index_add[0].addend"));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineAccess2()
{
    return (   ConstraintUnused                ("access_pointer")
            && ConstraintUnused                ("base_pointer")
            && ConstraintUnused                ("output_index")
            && ConstraintUnused                ("input_index[0]")
            && ConstraintUnused                ("input_index[1]")
            && ConstraintUnused                ("base_index")
            && ConstraintUnusedAddition      ()+"index_add[0]."
            && ConstraintUnusedMultiplication()+"stride_mul[0]."
            && ConstraintUnusedAddition      ()+"index_add[1]."
            && ConstraintUnusedMultiplication()+"stride_mul[1]."
            && ConstraintUnusedAddition      ()+"offset_add.");
}

ConstraintAnd<std::string,unsigned> ConstraintAffineAccess3(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode        (wrap, llvm::Instruction::GetElementPtr, "access_pointer")
            && ConstraintDFGEdge0      (wrap, "base_pointer",                   "access_pointer")
            && ConstraintDFGEdge1      (wrap, "output_index",                   "access_pointer")
            && ConstraintExtendedInt   (wrap, "offset_add.value",               "output_index")
            && ConstraintAddition      (wrap)+"offset_add."
            && ConstraintSame                ("stride_mul[2].value",            "offset_add.input")
            && ConstraintMultiplication(wrap)+"stride_mul[2]."
            && ConstraintSame                ("index_add[2].value",             "stride_mul[2].input")
            && ConstraintAddition      (wrap)+"index_add[2]."
            && ConstraintExtendedInt   (wrap, "input_index[2]",                 "index_add[2].addend")
            && (   ConstraintDistinct        ("index_add[2].input",             "offset_add.value")
                || ConstraintSame            ("input_index[2]",                 "index_add[2].input"))
            && ConstraintSame                ("stride_mul[1].value",            "index_add[2].input")
            && ConstraintMultiplication(wrap)+"stride_mul[1]."
            && ConstraintSame                ("index_add[1].value",             "stride_mul[1].input")
            && ConstraintAddition      (wrap)+"index_add[1]."
            && ConstraintExtendedInt   (wrap, "input_index[1]",                 "index_add[1].addend")
            && (   ConstraintDistinct        ("index_add[1].input",             "input_index[2]")
                || ConstraintSame            ("input_index[1]",                 "index_add[1].input"))
            && ConstraintSame                ("stride_mul[0].value",            "index_add[1].input")
            && ConstraintMultiplication(wrap)+"stride_mul[0]."
            && ConstraintSame                ("index_add[0].value",             "stride_mul[0].input")
            && ConstraintAddition      (wrap)+"index_add[0]."
            && ConstraintExtendedInt   (wrap, "input_index[0]",                 "index_add[0].input")
            && (   ConstraintDistinct        ("index_add[0].input",             "input_index[1]")
                || ConstraintSame            ("input_index[0]",                 "index_add[0].input"))
            && ConstraintSame                ("base_index",                     "index_add[0].addend"));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineAccess3()
{
    return (   ConstraintUnused                ("access_pointer")
            && ConstraintUnused                ("base_pointer")
            && ConstraintUnused                ("output_index")
            && ConstraintUnused                ("input_index[0]")
            && ConstraintUnused                ("input_index[1]")
            && ConstraintUnused                ("input_index[2]")
            && ConstraintUnused                ("base_index")
            && ConstraintUnusedAddition      ()+"index_add[0]."
            && ConstraintUnusedMultiplication()+"stride_mul[0]."
            && ConstraintUnusedAddition      ()+"index_add[1]."
            && ConstraintUnusedMultiplication()+"stride_mul[1]."
            && ConstraintUnusedAddition      ()+"index_add[2]."
            && ConstraintUnusedMultiplication()+"stride_mul[2]."
            && ConstraintUnusedAddition      ()+"offset_add.");
}

ConstraintAnd<std::string,unsigned> ConstraintAffineRead1(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Load, 1, "value")
            && ConstraintDFGEdge0     (wrap, "access_pointer",           "value")
            && ConstraintAffineAccess1(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineRead1()
{
    return (   ConstraintUnused             ("value")
            && ConstraintUnusedAffineAccess1());
}

ConstraintAnd<std::string,unsigned> ConstraintAffineRead2(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Load, 1, "value")
            && ConstraintDFGEdge0     (wrap, "access_pointer",           "value")
            && ConstraintAffineAccess2(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineRead2()
{
    return (   ConstraintUnused             ("value")
            && ConstraintUnusedAffineAccess2());
}


ConstraintAnd<std::string,unsigned> ConstraintAffineRead3(FunctionWrapper& wrap)
{
    return (   ConstraintOpcode       (wrap, llvm::Instruction::Load, 1, "value")
            && ConstraintDFGEdge0     (wrap, "access_pointer",           "value")
            && ConstraintAffineAccess3(wrap));
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineRead3()
{
    return (   ConstraintUnused             ("value")
            && ConstraintUnusedAffineAccess3());
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
    return (   ConstraintFor            (wrap)+"loop1."
            && ConstraintNestedFor      (wrap, "loop2.",                    "loop1.body_sese.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_begin",          "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_step",           "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_end",            "loop1.")
            && ConstraintPermute2             ("loop1.iterator",            "loop2.iterator",
                                               "output.input_index[0]",     "output.input_index[1]")
            && ConstraintOpcode         (wrap, llvm::Instruction::Store, 2, "store_instr")
            && ConstraintDFGEdge        (wrap, "output.access_pointer",     "store_instr")
            && ConstraintDFGEdge        (wrap, "weighted_sum.output",       "store_instr")
            && ConstraintAffineAccess2  (wrap)+"output."
            && ConstraintLocallyConstant(wrap, "output.vector",             "loop1.")
            && ConstraintNestedFor      (wrap, "loop3.",                    "loop2.body_sese.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_begin",          "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_step",           "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_end",            "loop1.")
            && ConstraintPermute2             ("loop1.iterator",            "loop3.iterator",
                                               "input1.input_index[0]",     "input1.input_index[1]")
            && ConstraintAffineRead2    (wrap)+"input1."
            && ConstraintLocallyConstant(wrap, "input1.vector",             "loop1.")
            && ConstraintPermute2             ("loop2.iterator",            "loop3.iterator",
                                               "input2.input_index[0]",     "input2.input_index[1]")
            && ConstraintAffineRead2    (wrap)+"input2."
            && ConstraintLocallyConstant(wrap, "input2.vector",             "loop1.")
            && ConstraintOpcode         (wrap, llvm::Instruction::FMul, 2,  "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "input1.value",              "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "input2.value",              "dot_sum.increment")
            && ConstraintSumReduction   (wrap)+"dot_sum."
            && ConstraintWeightedSum    (wrap, "weighted_sum.",             "loop1.precursor",
                                               "dot_sum.final_value",       "old_value")
            && (   (   ConstraintOpcode (wrap, llvm::Instruction::Load, 1,  "old_value")
                    && ConstraintDFGEdge(wrap, "output.access_pointer",     "old_value"))
                || ConstraintUnused           ("old_value")));
}

ConstraintAnd<std::string,unsigned> ConstraintSparseMV(FunctionWrapper& wrap)
{
    return (   ConstraintFor            (wrap)+"loop1."
            && ConstraintSame                 ("range_begin_read.input_index[0]", "loop1.iterator")
            && ConstraintAffineRead1    (wrap)+"range_begin_read."
            && ConstraintDFGEdge0       (wrap, "loop1.iterator", "range_end_read.input_index[0]")
            && ConstraintOpcode         (wrap,  llvm::Instruction::Add, 2, "range_end_read.input_index[0]")
            && ConstraintAffineRead1    (wrap)+"range_end_read."
            && ConstraintNestedFor      (wrap, "loop2.", "loop1.body_sese.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_begin", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_step", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_end", "loop1.")
            && ConstraintSame                 ("loop2.iter_begin", "range_begin_read.value")
            && ConstraintSame                 ("loop2.iter_end", "range_end_read.value")
            && ConstraintSame                 ("input_matrix.input_index[0]", "loop2.iterator")
            && ConstraintAffineRead1    (wrap)+"input_matrix."
            && ConstraintSame                 ("input_index.input_index[0]", "loop2.iterator")
            && ConstraintAffineRead1    (wrap)+"input_index."
            && ConstraintSame                 ("input_vector.input_index[0]", "input_index.input_index[0]")
            && ConstraintAffineRead1    (wrap)+"input_vector."
            && ConstraintOpcode         (wrap, llvm::Instruction::FMul, 2, "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "input_matrix.value", "dot_sum.increment")
            && ConstraintDFGEdge        (wrap, "intput_vector.value", "dot_sum.increment")
            && ConstraintSumReduction   (wrap)+"dot_sum."
            && ConstraintSame                 ("output.input_index[0]", "loop1.iterator")
            && ConstraintAffineAccess1  (wrap)+"output."
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

    return (   ConstraintSESE                        (wrap)+"outer_sese."
            && ConstraintSESE                        (wrap)+"inner_sese."
            && ConstraintCFGDominate                 (wrap, "outer_sese.begin", "inner_sese.begin")
            && ConstraintCFGPostdom                  (wrap, "outer_sese.end",   "inner_sese.end")
            && 4 * (   (   ConstraintCDGEdge         (wrap, "control_history*", "inner_sese.begin")
                        && ConstraintOrder                 ("control_history*", "control_history+"))
                    || (   ConstraintUnused                ("control_history*")
                        && ConstraintUnused                ("control_history+")))
            && ConstraintUnused                            ("control_history[4]")
            && ConstraintSharedFate                        ("control_history", 5)
            && 64 * (   (   ConstraintLocallyConstant(wrap, "constants*", "outer_sese.")
                         && ConstraintOrder                ("constants*", "constants+"))
                     || (   ConstraintUnused               ("constants*")
                         && ConstraintUnused               ("constants+")))
            && ConstraintUnused                            ("constants[64]")
            && ConstraintSharedFate                        ("constants", 65)
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

ConstraintAnd<std::string,unsigned> ConstraintUnusedPureFunction()
{
    return (   ConstraintUnusedSESE     ()+"outer_sese."
            && ConstraintUnusedSESE     ()+"inner_sese."
            && 4 * (   ConstraintUnused ("control_history*")
                    && ConstraintUnused ("control_history+"))
            && 64 * (   ConstraintUnused("constants*")
                     && ConstraintUnused("constants+")));
}

ConstraintAnd<std::string,unsigned> ConstraintScalarReduction(FunctionWrapper& wrap)
{
    return (   ConstraintFor                  (wrap)
            && ConstraintOpcode               (wrap, llvm::Instruction::PHI, 2, "old_value")

            && ConstraintCFGDominate          (wrap, "begin",                   "old_value")
            && ConstraintCFGDominate          (wrap, "old_value",               "end")

            && ConstraintDFGReachable         (wrap, "old_value",               "final_value")
            && ConstraintDFGEdge              (wrap, "old_value",               "final_value")
            && ConstraintCFGPostdomStrict     (wrap, "final_value",             "end")

            && ConstraintDFGEdge              (wrap, "initial_value",           "old_value")
            && (   ConstraintPreexecution     (wrap, "initial_value")
                || ConstraintCFGDominateStrict(wrap, "initial_value",           "old_value"))

            && ConstraintDFGEdge              (wrap, "update_expr.output",      "old_value")
            && ConstraintDFGReachable         (wrap, "old_value",               "update_expr.output")

            && ConstraintCFGDominate          (wrap, "body_sese.begin",         "update_expr.output")
            && ConstraintCFGPostdom           (wrap, "body_sese.end",           "update_expr.output")
            && ConstraintCFGEdge              (wrap, "update_expr.output",      "post_store_instruction")
            && ConstraintCFGBlocked           (wrap, "post_store_instruction",  "end", "update_expr.output")

            && ConstraintSameSESE                 ("update_expr.outer_sese.", "")
            && ConstraintSameSESE                 ("update_expr.inner_sese.", "body_sese.")

            && 30 * (   (   ConstraintOpcode             (wrap, llvm::Instruction::Load, 1, "update_expr.input*")
                         && ConstraintDFGEdge            (wrap, "affine_access*.access_pointer", "update_expr.input*")
                         && ConstraintAffineAccess1      (wrap)+"affine_access*."
                         && ConstraintSame                     ("affine_access*.input_index[0]", "iterator")
                         && ConstraintAffineAccess1      (wrap)+"affine_access*."
                         && (   ConstraintPreexecution     (wrap, "affine_access*.base_pointer")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.base_pointer", "begin"))
                         && (   ConstraintPreexecution     (wrap, "affine_access*.offset_add.addend")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.offset_add.addend", "begin")
                             || ConstraintUnused                 ("affine_access*.offset_add.addend"))
                         && (   ConstraintPreexecution     (wrap, "affine_access*.stride_mul[0].multiplier")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.stride_mul[0].multiplier", "begin")
                             || ConstraintUnused                 ("affine_access*.stride_mul[0].multiplier"))
                         && (   ConstraintPreexecution     (wrap, "affine_access*.base_index")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.base_index", "begin")
                             || ConstraintUnused                 ("affine_access*.base_index"))
                         && ConstraintOrder                    ("update_expr.input*", "update_expr.input+"))
                     || (   ConstraintUnused                   ("update_expr.input*")
                         && ConstraintUnusedAffineAccess1    ()+"affine_access*."
                         && ConstraintUnused                   ("update_expr.input+")))

            && ConstraintUnused("update_expr.input[30]")
            && ConstraintSame  ("update_expr.input[31]", "old_value")
            && ConstraintUnused("update_expr.input[32]")

            && 4 * (   (   ConstraintOpcode           (wrap, llvm::Instruction::PHI,      "update_expr.restrictions*")
                        && ConstraintCFGDominate      (wrap, "begin",                     "update_expr.restrictions*")
                        && ConstraintCFGDominateStrict(wrap, "update_expr.restrictions*", "body_sese.begin")
                        && ConstraintDistinct               ("update_expr.restrictions*", "old_value")
                        && ConstraintOrder                  ("update_expr.restrictions*", "update_expr.restrictions+"))
                    || (   ConstraintUnused                 ("update_expr.restrictions*")
                        && ConstraintUnused                 ("update_expr.restrictions+")))

            && ConstraintUnused            ("update_expr.restrictions[4]")
            && ConstraintSharedFate        ("update_expr.restrictions", 5)
            && ConstraintPureFunction(wrap)+"update_expr.");
}

ConstraintAnd<std::string,unsigned> ConstraintUnusedScalarReduction()
{
    return (   ConstraintUnusedFor()
            && ConstraintUnused   ("old_value")
            && ConstraintUnused   ("final_value")
            && ConstraintUnused   ("initial_value")
            && ConstraintUnused   ("post_store_instruction")

            && 30 * (   ConstraintUnused               ("update_expr.input*")
                     && ConstraintUnusedAffineAccess1()+"affine_access*.")

            && ConstraintUnused("update_expr.input[30]")
            && ConstraintUnused("update_expr.input[31]")
            && ConstraintUnused("update_expr.input[32]")

            && ConstraintUnused("update_expr.restrictions[0]")
            && ConstraintUnused("update_expr.restrictions[1]")
            && ConstraintUnused("update_expr.restrictions[2]")
            && ConstraintUnused("update_expr.restrictions[3]")
            && ConstraintUnused("update_expr.restrictions[4]")

            && ConstraintUnusedPureFunction()+"update_expr.");
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

            && ConstraintSameSESE         ("update_expr.outer_sese.",        "")
            && ConstraintSameSESE         ("update_expr.inner_sese.",        "body_sese.")

            && ConstraintSameSESE         ("index_expr.outer_sese.",         "")
            && ConstraintSameSESE         ("index_expr.inner_sese.",         "body_sese.")

            && 28 * (   (  ConstraintOpcode                (wrap, llvm::Instruction::Load, 1, "update_expr.input*")
                         && ConstraintDFGEdge              (wrap, "affine_access*.access_pointer", "update_expr.input*")
                         && ConstraintSame                       ("affine_access*.input_index[0]", "iterator")
                         && ConstraintAffineAccess1        (wrap)+"affine_access*."
                         && ConstraintDistinct                   ("affine_access*.base_pointer", "reduction_array")
                         && (   ConstraintPreexecution     (wrap, "affine_access*.base_pointer")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.base_pointer", "begin"))
                         && (   ConstraintPreexecution     (wrap, "affine_access*.offset_add.addend")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.offset_add.addend", "begin")
                             || ConstraintUnused                 ("affine_access*.offset_add.addend"))
                         && (   ConstraintPreexecution     (wrap, "affine_access*.stride_mul[0].multiplier")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.stride_mul[0].multiplier", "begin")
                             || ConstraintUnused                 ("affine_access*.stride_mul[0].multiplier"))
                         && (   ConstraintPreexecution     (wrap, "affine_access*.base_index")
                             || ConstraintCFGDominateStrict(wrap, "affine_access*.base_index", "begin")
                             || ConstraintUnused                 ("affine_access*.base_index"))
                         && ConstraintOrder                      ("update_expr.input*", "update_expr.input+")
                         && ConstraintSame                       ("update_expr.input*", "index_expr.input*")
                         && ConstraintOrder                      ("index_expr.input*", "index_expr.input+"))
                     || (   ConstraintUnused                     ("update_expr.input*")
                         && ConstraintUnused                     ("index_expr.input*")
                         && ConstraintUnusedAffineAccess1      ()+"affine_access*."
                         && ConstraintUnused                     ("update_expr.input+")
                         && ConstraintUnused                     ("index_expr.input+")))
 
            && ConstraintOpcode(wrap, llvm::Instruction::GetElementPtr, "affine_access[0].access_pointer")

            && (   (   ConstraintOpcode      (wrap, llvm::Instruction::Load, 1,       "closure_read[0]")
                    && ConstraintDFGEdge     (wrap, "closure_pointer[0]",             "closure_read[0]")
                    && ConstraintOpcode      (wrap, llvm::Instruction::GetElementPtr, "closure_pointer[0]")
                    && ConstraintDFGEdge0    (wrap, "closure_vector[0]",              "closure_pointer[0]")
                    && ConstraintCFGDominate (wrap, "body_sese.begin",                "closure_read[0]")
                    && ConstraintCFGPostdom  (wrap, "body_sese.end",                  "closure_read[0]")
                    && ConstraintDistinct          ("closure_vector[0]",              "affine_access[0].base_pointer")
                    && ConstraintDistinct          ("closure_vector[0]",              "reduction_array")
                    && ConstraintOrder             ("closure_read[0]",                "closure_read[1]"))
                || (   ConstraintUnused            ("closure_read[0]")
                    && ConstraintUnused            ("closure_pointer[0]")
                    && ConstraintUnused            ("closure_vector[0]")
                    && ConstraintUnused            ("closure_read[1]")))

            && (   (   ConstraintOpcode      (wrap, llvm::Instruction::Load, 1,       "closure_read[1]")
                    && ConstraintDFGEdge     (wrap, "closure_pointer[1]",             "closure_read[1]")
                    && ConstraintOpcode      (wrap, llvm::Instruction::GetElementPtr, "closure_pointer[1]")
                    && ConstraintDFGEdge0    (wrap, "closure_vector[1]",              "closure_pointer[1]")
                    && ConstraintCFGDominate (wrap, "body_sese.begin",                "closure_read[1]")
                    && ConstraintCFGPostdom  (wrap, "body_sese.end",                  "closure_read[1]")
                    && ConstraintDistinct          ("closure_vector[1]",              "affine_access[0].base_pointer")
                    && ConstraintDistinct          ("closure_vector[1]",              "reduction_array"))
                || (   ConstraintUnused            ("closure_read[1]")
                    && ConstraintUnused            ("closure_pointer[1]")
                    && ConstraintUnused            ("closure_vector[1]")))

            && ConstraintUnused                     ("update_expr.input[28]")
            && ConstraintSame                       ("update_expr.input[29]", "closure_read[0]")
            && ConstraintSame                       ("update_expr.input[30]", "closure_read[1]")
            && ConstraintSame                       ("update_expr.input[31]", "old_value")
            && ConstraintUnused                     ("update_expr.input[32]")
            && ConstraintSharedFate                 ("update_expr.input", 33)

            && 4 * (   (   ConstraintOpcode           (wrap, llvm::Instruction::PHI,      "update_expr.restrictions*")
                        && ConstraintCFGDominate      (wrap, "begin",                     "update_expr.restrictions*")
                        && ConstraintCFGDominateStrict(wrap, "update_expr.restrictions*", "body_sese.begin")
                        && ConstraintOrder                  ("update_expr.restrictions*", "update_expr.restrictions+"))
                    || (   ConstraintUnused                 ("update_expr.restrictions*")
                        && ConstraintUnused                ("update_expr.restrictions+")))

            && ConstraintUnused            ("update_expr.restrictions[4]")
            && ConstraintSharedFate        ("update_expr.restrictions", 5)
            && ConstraintPureFunction(wrap)+"update_expr."

            && ConstraintUnused            ("index_expr.input[28]")
            && ConstraintSame              ("index_expr.input[29]", "closure_read[0]")
            && ConstraintSame              ("index_expr.input[30]", "closure_read[1]")
            && ConstraintUnused            ("index_expr.input[31]")
            && ConstraintUnused            ("index_expr.input[32]")

            && 4 * (   (   ConstraintOpcode           (wrap, llvm::Instruction::PHI, "index_expr.restrictions*")
                        && ConstraintCFGDominate      (wrap, "begin", "index_expr.restrictions*")
                        && ConstraintCFGDominateStrict(wrap, "index_expr.restrictions*", "body_sese.begin")
                        && ConstraintOrder                  ("index_expr.restrictions*", "update_expr.restrictions+"))
                    || (   ConstraintUnused                 ("index_expr.restrictions*")
                        && ConstraintUnused                 ("index_expr.restrictions+")))

            && ConstraintUnused            ("index_expr.restrictions[4]")
            && ConstraintSharedFate        ("index_expr.restrictions", 5)
 //         && ConstraintSharedFate<33>    ("index_expr.input")
            && ConstraintPureFunction(wrap)+"index_expr.");
}

ConstraintAnd<std::string,unsigned> Constraint3DStencilAccess(FunctionWrapper& wrap)
{
    return (   ConstraintAddition     (wrap)+"index[0]."
            && (   ConstraintConstant (wrap, "index[0].addend")
                || ConstraintUnused         ("index[0].addend"))
            && ConstraintSame               ("read.input_index[0]", "index[0].value")
            && ConstraintAddition     (wrap)+"index[1]."
            && (   ConstraintConstant (wrap, "index[1].addend")
                || ConstraintUnused         ("index[1].addend"))
            && ConstraintSame               ("read.input_index[1]", "index[1].value")
            && ConstraintAddition     (wrap)+"index[2]."
            && (   ConstraintConstant (wrap, "index[2].addend")
                || ConstraintUnused         ("index[2].addend"))
            && ConstraintSame               ("read.input_index[2]", "index[2].value")
            && ConstraintAffineAccess3(wrap)+"read.");
}

ConstraintAnd<std::string,unsigned> ConstraintUnused3DStencilAccess()
{
    return (   ConstraintUnusedAffineAccess3()+"read."
            && ConstraintUnusedAddition     ()+"index[0]."
            && ConstraintUnusedAddition     ()+"index[1]."
            && ConstraintUnusedAddition     ()+"index[2].");
}

ConstraintAnd<std::string,unsigned> Constraint3DStencil(FunctionWrapper& wrap)
{
    return (   ConstraintFor            (wrap)+"loop1."
            && ConstraintNestedFor      (wrap, "loop2.",           "loop1.body_sese.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_begin", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_step",  "loop1.")
            && ConstraintLocallyConstant(wrap, "loop2.iter_end",   "loop1.")
            && ConstraintNestedFor      (wrap, "loop3.",           "loop2.body_sese.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_begin", "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_step",  "loop1.")
            && ConstraintLocallyConstant(wrap, "loop3.iter_end",   "loop1.")
            && ConstraintOpcode         (wrap, llvm::Instruction::Store, 2, "store_instr")
            && ConstraintPermute3             ("loop1.iterator", "loop2.iterator", "loop3.iterator",
                                               "write_address.input_index[0]", "write_address.input_index[1]",
                                               "write_address.input_index[2]")
            && ConstraintDFGEdge1       (wrap, "write_address.access_pointer", "store_instr")
            && ConstraintAffineAccess3  (wrap)+"write_address."

            && 32 * (   (   ConstraintPermute3               ("loop1.iterator", "loop2.iterator", "loop3.iterator",
                                                              "reads*.index[0].input", "reads*.index[1].input",
                                                              "reads*.index[2].input")
                         && ConstraintOpcode           (wrap, llvm::Instruction::Load, 1, "compute_expr.input*")
                         && ConstraintDFGEdge0         (wrap, "reads*.read.access_pointer", "compute_expr.input*")
                         && Constraint3DStencilAccess  (wrap)+"reads*."
                         && ConstraintOrder                  ("compute_expr.input*", "compute_expr.input+"))
                     || (   ConstraintUnused                 ("compute_expr.input*")
                         && ConstraintUnused3DStencilAccess()+"reads*."
                         && ConstraintUnused                 ("compute_expr.input+")))

            && ConstraintUnused            ("compute_expr.input[32]")
            && ConstraintDFGEdge0    (wrap, "compute_expr.output", "store_instr")
            && ConstraintSameSESE          ("compute_expr.outer_sese.", "loop1.")
            && ConstraintSameSESE          ("compute_expr.inner_sese.", "loop3.body_sese.")
            && ConstraintSharedFate        ("compute_expr.input", 33)
            && ConstraintUnused            ("compute_expr.restrictions[0]")
            && ConstraintUnused            ("compute_expr.restrictions[1]")
            && ConstraintUnused            ("compute_expr.restrictions[2]")
            && ConstraintUnused            ("compute_expr.restrictions[3]")
            && ConstraintUnused            ("compute_expr.restrictions[4]")
            && ConstraintPureFunction(wrap)+"compute_expr.");
}
