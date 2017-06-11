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

ConstraintAnd ConstraintArguments(std::string dst, std::string src1, std::string src2)
{
    return ConstraintFirstArgument (src1, dst)
        && ConstraintSecondArgument(src2, dst);
}

ConstraintOr ConstraintArgumentsPermuted(std::string dst, std::string src1, std::string src2)
{
    return ConstraintArguments(dst, src1, src2)
        || ConstraintArguments(dst, src2, src1);
}

ConstraintAnd ConstraintSumChain()
{
    return     ConstraintIntegerType                ("value")
        && 0 + ConstraintSame                       ("value",       "results*")
        && 8 * (   (   ConstraintAddInst            ("results*")
                    && ConstraintArgumentsPermuted  ("results*",    "results+", "factors*")
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
                    && ConstraintArgumentsPermuted  ("results*", "results+", "factors*")
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

ConstraintOr ConstraintLocallyConstant(std::string var, std::string border)
{
    return (   ConstraintInstruction  (border)
            && ConstraintPreexecution (var))
        || ConstraintCFGDominateStrict(var, border)
        || (   ConstraintInstruction  (border)
            && ConstraintUnused       (var));
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

ConstraintAnd ConstraintInSESE(std::string var, std::string sese)
{
    return sese + ConstraintCFGDominate("begin", nonlocal(var))
        && sese + ConstraintCFGPostdom ("end",   nonlocal(var));
}

// PROBLEM IF VAR IS ONE BEFORE END
ConstraintAnd ConstraintMaxOnceInSESE(std::string var, std::string sese)
{
    return ConstraintInSESE           (var, sese)
        && sese + ConstraintCFGEdge   (nonlocal(var),              nonlocal(var+"_after"))
        && sese + ConstraintCFGBlocked(nonlocal(var+"_after"), "successor", nonlocal(var))
        && sese + ConstraintCFGBlocked(nonlocal(var+"_after"), "precursor", nonlocal(var));
}

ConstraintAnd ConstraintOnceInSESE(std::string var, std::string sese)
{
    return ConstraintMaxOnceInSESE     (var, sese)
        && sese + ConstraintCFGDominate(nonlocal(var), "end");
}

ConstraintOr ConstraintExtendedInt(std::string input, std::string output)
{
    return (   ConstraintZExtInst(output)
            && ConstraintDFGEdge (input, output))
        || (   ConstraintSExtInst(output)
            && ConstraintDFGEdge (input, output))
        || ConstraintSame        (input, output);
}

ConstraintAnd ConstraintLoop()
{
    return ConstraintSESE              ()
        && ConstraintSame              ("body_sese.precursor",    "end")
        && ConstraintSame              ("body_sese.successor",  "begin")
        && "body_sese" + ConstraintSESE()
        && ConstraintDistinct          ("body_sese.begin",  "successor");
}

ConstraintAnd ConstraintInductionVar(std::string old_ind, std::string new_ind)
{
    return ConstraintPHINode      (old_ind,         "begin")
        && ConstraintIncomingValue(new_ind, "body_sese.end", old_ind);
}

ConstraintAnd ConstraintFor()
{
    return ConstraintLoop           ()
        && ConstraintInductionVar   ("iterator",  "increment")
        && ConstraintDFGEdge        ("iterator", "comparison")
        && ConstraintICmpInst       ("comparison")
        && ConstraintDFGEdge        ("comparison",      "end")
        && ConstraintDFGEdge        ("increment",  "iterator")
        && ConstraintAddInst        ("increment")
        && ConstraintDFGEdge        ("iterator",  "increment")
        && ConstraintDFGEdge        ("iter_end", "comparison")
        && ConstraintLocallyConstant("iter_end",      "begin")
        && ConstraintDFGEdge        ("iter_begin", "iterator")
        && ConstraintLocallyConstant("iter_begin",    "begin")
        && ConstraintDFGEdge        ("iter_step", "increment")
        && ConstraintLocallyConstant("iter_step",     "begin");
}

ConstraintAnd ConstraintForNest(unsigned dim)
{
    return 0 + ("loop*" + ConstraintFor())

        && 0 + (   ConstraintSame("precursor", "loop*.precursor")
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
                      && ConstraintLocallyConstant("loop+.iter_begin",    "begin")
                      && ConstraintLocallyConstant("loop+.iter_step",     "begin")
                      && ConstraintLocallyConstant("loop+.iter_end",      "begin"))

        && (dim-1) + (   ConstraintSame("body_sese.precursor", "loop*.body_sese.precursor")
                      && ConstraintSame("body_sese.begin",     "loop*.body_sese.begin")
                      && ConstraintSame("body_sese.end",       "loop*.body_sese.end")
                      && ConstraintSame("body_sese.successor", "loop*.body_sese.successor"));
}

ConstraintPrefix ConstraintAfterLoop(std::string var, std::string loop)
{
    return loop + (   ConstraintCFGDominateStrict("end",             nonlocal(var))
                   && ConstraintCFGNotDominate   ("body_sese.begin", nonlocal(var)));
}

ConstraintOr ConstraintAddition()
{
    return (   ConstraintSame                   ("input", "value")
            && ConstraintUnused                 ("addend"))
        || (   (   ConstraintAddInst            ("value")
                || ConstraintFAddInst           ("value"))
            && ConstraintArgumentsPermuted      ("value", "input", "addend"))
        || (   (   ConstraintSubInst            ("value")
                || ConstraintFSubInst           ("value"))
            && ConstraintArguments              ("value", "input", "addend"));
}

ConstraintOr ConstraintMultiplication()
{
    return (   ConstraintSame             ("input", "value")
            && ConstraintUnused           ("multiplier"))
        || (   (   ConstraintMulInst      ("value")
                || ConstraintFMulInst     ("value"))
            && ConstraintArgumentsPermuted("value", "input", "multiplier"))
        || (   (   ConstraintFDivInst     ("value"))
            && ConstraintArguments        ("value", "input", "multiplier"));
}

ConstraintAnd ConstraintArrayAccess(std::string origin)
{
    return ConstraintGEPInst        ("access_pointer")
        && ConstraintFirstArgument  ("base_pointer", "access_pointer")
        && ConstraintLocallyConstant("base_pointer",   origin)
        && ConstraintSecondArgument ("output_index", "access_pointer");
}

ConstraintAnd ConstraintAffineAccess(unsigned dim, std::string origin)
{
    std::vector<ConstraintRange> result_parts;

    for(unsigned i = 0; i < dim; i++)
    {
        std::string output     = (i+1==dim)?"offset_add.input":("index_add+.input");
        std::string comparison = (i+1==dim)?"offset_add.value":("input_index+");
        std::string add_input  = (i==0)?("index_add*.input"):("index_add*.addend");

        result_parts.emplace_back(i + (   ConstraintSame            ("stride_mul*.value",              output)
                                       && "stride_mul*" + ConstraintMultiplication()
                                       && ConstraintLocallyConstant ("stride_mul*.multiplier", origin)
                                       && ConstraintSame            ("index_add*.value",  "stride_mul*.input")
                                       && "index_add*" + ConstraintAddition()
                                       && ConstraintExtendedInt     ("input_index*",                add_input)
                                       && (   (   ConstraintDistinct(add_input,                    comparison)
                                               && ConstraintIntegerType("input_index*"))
                                           || (   ConstraintSame       ("input_index*",             add_input)
                                               && ConstraintIntegerType(comparison)))));
    }

    ConstraintAnd result = ConstraintArrayAccess            (origin)
                                              && ConstraintExtendedInt            ("offset_add.value",  "output_index")
                                              && "offset_add" + ConstraintAddition()
                                              && ConstraintLocallyConstant        ("offset_add.addend", origin);

    for(auto it = result_parts.rbegin(); it != result_parts.rend(); ++it)
    {
        result = std::move(result) && std::move(*it);
    }

    return std::move(result)
        && ConstraintSame           ("base_index", "index_add[0].addend")
        && ConstraintLocallyConstant("base_index",        origin);
}

ConstraintAnd ConstraintStencilAccess(std::string origin, std::vector<std::string> indices)
{
    return "read" + ConstraintAffineAccess(indices.size(), nonlocal(origin))

        && ConstraintSame("read.base_pointer",   "base_pointer")
        && ConstraintSame("read.access_pointer", "access_pointer")

        && indices.size() * (   ConstraintSame               ("read.input_index*", "index*.value")
                             && "index*" + ConstraintAddition()
                             && (   ConstraintConstant       ("index*.addend")
                                 || ConstraintUnused         ("index*.addend"))
                             && ConstraintSame               ("index*.input",      "input_index*"))

        && ConstraintVectorSame(expand_variables("input_index", indices.size()), indices);
}

ConstraintAnd ConstraintArrayRead(std::string origin)
{
    return ConstraintLoadInst     ("value")
        && ConstraintFirstArgument("access_pointer", "value")
        && ConstraintArrayAccess  (origin);
}

ConstraintAnd ConstraintArrayStore(std::string origin)
{
    return ConstraintStoreInst     ("store")
        && ConstraintSecondArgument("access_pointer", "store")
        && ConstraintArrayAccess   (origin);
}

ConstraintAnd ConstraintPermMultidRead(std::string origin, std::vector<std::string> indices)
{
    return ConstraintLoadInst                    ("value")
        && ConstraintFirstArgument               ("access_pointer", "value")
        && ConstraintAffineAccess(indices.size(), origin)
        && ConstraintPermute                     (expand_variables("input_index", indices.size()), indices);
}

ConstraintAnd ConstraintPermMultidStore(std::string origin, std::vector<std::string> indices)
{
    return ConstraintStoreInst                   ("store")
        && ConstraintSecondArgument              ("access_pointer", "store")
        && ConstraintAffineAccess(indices.size(), origin)
        && ConstraintPermute                     (expand_variables("input_index", indices.size()), indices);
}

ConstraintAnd ConstraintStencilRead(std::string origin, std::vector<std::string> indices)
{
    return ConstraintLoadInst     ("value")
        && ConstraintFirstArgument("access_pointer", "value")
        && ConstraintStencilAccess(origin, indices);
}

ConstraintAnd ConstraintPureFunction(std::string input, std::string outer, std::string inner)
{
    return std::make_pair("origin",4) * (   ConstraintCDGEdge("", nonlocal(inner)))

        && std::make_pair("constants",32) * (   ConstraintLocallyConstant   ("value", nonlocal(outer))
                                             && ConstraintNotNumericConstant("value")
                                             && ConstraintDFGEdge           ("value",           "use")
                                             && ConstraintCFGDominate       (nonlocal(inner),   "use"))

        && std::make_pair("restrictions",32) * (   ConstraintCFGDominate      (nonlocal(outer), "value")
                                                && ConstraintCFGDominateStrict("value", nonlocal(inner))
                                                && ConstraintDFGEdge          ("value",           "use")
                                                && ConstraintCFGDominate      (nonlocal(inner),   "use"))

        && ConstraintVectorSame("input[0..32]", input)

        && ConstraintPDGDominate("restrictions[0..32].value",
                                 "origin[0..4],constants[0..32].value,"+input, "output");
}

ConstraintAnd ConstraintScalarReductionBase(std::string input)
{
    return ConstraintInductionVar("old_value", "update_expr.output")
        && ConstraintDistinct    ("old_value",           "iterator")

        && std::make_pair("uses",4) * (   ConstraintDFGEdge  (nonlocal("old_value"), "")
                                       && ConstraintAfterLoop("",          nonlocal("")))
        && ConstraintDFGEdge("old_value", "uses[0]")

        && "update_expr" + ConstraintPureFunction(nonlocal("old_value,"+input),
                                                  nonlocal("begin"), nonlocal("body_sese.begin"))

        && std::make_pair("incoming",4) * (   ConstraintDFGEdge ("",          nonlocal("old_value"))
                                           && ConstraintDistinct("", nonlocal("update_expr.output")));
}

ConstraintAnd ConstraintScalarReduction()
{
    return ConstraintFor()

        && std::make_pair("read",32) * (   ConstraintPermMultidRead(nonlocal("begin"), {nonlocal("iterator")})
                                        && ConstraintInSESE        ("value",  nonlocal("body_sese")))
        && ConstraintLoadInst("read[0].value")

        && ConstraintScalarReductionBase("read[0..31].value");
}

ConstraintAnd ConstraintMatrixmatrix()
{
    return ConstraintForNest(3)

        && "output" + ConstraintPermMultidStore(nonlocal("begin"), {nonlocal("loop[0].iterator"),
                                                                    nonlocal("loop[1].iterator")})
        && "input1" + ConstraintPermMultidRead (nonlocal("begin"), {nonlocal("loop[0].iterator"),
                                                                    nonlocal("loop[2].iterator")})
        && "input2" + ConstraintPermMultidRead (nonlocal("begin"), {nonlocal("loop[1].iterator"),
                                                                    nonlocal("loop[2].iterator")})

        && "loop[2]" + ConstraintScalarReductionBase(nonlocal("input1.value,input2.value"))

        && (   (   ConstraintLoadInst("old_value")
                && ConstraintDFGEdge ("output.access_pointer", "old_value"))
            || (  ConstraintUnused   ("old_value")
                && ConstraintGEPInst ("output.access_pointer")))

        && ConstraintFirstArgument("update_expr.output", "output.store")
        && "update_expr" + ConstraintPureFunction(nonlocal("loop[2].old_value,old_value"),
                                                  nonlocal("begin"), nonlocal("loop[1].body_sese.begin"));
}

ConstraintAnd ConstraintSparseMV() 
{
    return ConstraintFor()

        && ConstraintUnused("begin_idx.offset_add.addend")
        && "begin_idx" + ConstraintPermMultidRead(nonlocal("begin"), {nonlocal("iterator")})

        && ConstraintSame("begin_idx.base_index",   "end_idx.base_index")
        && ConstraintSame("begin_idx.base_pointer", "end_idx.base_pointer")
        && "end_idx"   + ConstraintPermMultidRead(nonlocal("begin"), {nonlocal("iterator")})

        && ConstraintExtendedInt("begin_idx.value", "reduc.iter_begin")
        && ConstraintExtendedInt("end_idx.value",   "reduc.iter_end")

        && "reduc" + (   ConstraintFor()
                      && "idx_read"   + ConstraintPermMultidRead(nonlocal("begin"),    {nonlocal("iterator")})
                      && "indir_read" + ConstraintPermMultidRead(nonlocal("begin"), {nonlocal("idx_read.value")})
                      && "seq_read"   + ConstraintPermMultidRead(nonlocal("begin"),    {nonlocal("iterator")})

                      && ConstraintDistinct("idx_read.value", "seq_read.value")

                      && ConstraintScalarReductionBase("seq_read.value,indir_read.value"));
}

ConstraintAnd ConstraintHistogram()
{
    return ConstraintFor()

        && ConstraintStoreInst     ("store_instr")
        && ConstraintMaxOnceInSESE ("store_instr",             "body_sese")
        && ConstraintSecondArgument("index_expr.output",     "store_instr")
        && ConstraintGEPInst       ("index_expr.output")
        && ConstraintFirstArgument ("index_expr.output",       "old_value")
        && ConstraintLoadInst      ("old_value")
        && ConstraintFirstArgument ("update_expr.output",    "store_instr")
        && ConstraintFirstArgument ("reduction_array", "index_expr.output")

        && std::make_pair("read",30) * (   ConstraintPermMultidRead(nonlocal("begin"),    {nonlocal("iterator")})
                                        && ConstraintInSESE        ("value",              nonlocal("body_sese"))
                                        && ConstraintDistinct      ("base_pointer", nonlocal("reduction_array")))
        && ConstraintLoadInst("read[0].value")

        // This feature is needed for tpacf to work.
        && std::make_pair("closure",2) * (   ConstraintArrayRead(nonlocal("begin"))
                                          && ConstraintInSESE   ("value",                   nonlocal("body_sese"))
                                          && ConstraintDistinct ("base_pointer", nonlocal("read[0].base_pointer"))
                                          && ConstraintDistinct ("base_pointer",      nonlocal("reduction_array")))

        && "update_expr" + ConstraintPureFunction(nonlocal("read[0..29].value,closure[0..2].value,old_value"),
                                                  nonlocal("begin"), nonlocal("body_sese.begin"))
        && "index_expr"  + ConstraintPureFunction(nonlocal("read[0..30].value,closure[0..2].value"),
                                                  nonlocal("begin"), nonlocal("body_sese.begin"));
}

ConstraintAnd ConstraintStencil(unsigned dim)
{
    auto loop_iterators = expand_variables(nonlocal("loop"), dim, "iterator");
    auto access_indizes = expand_variables(nonlocal("write.input_index"), dim);

    return ConstraintForNest(dim)

        && "write" + ConstraintPermMultidStore(nonlocal("begin"), loop_iterators)

        && ConstraintFirstArgument("compute_expr.output", "write.store")

        && std::make_pair("reads",32) * ConstraintStencilRead(nonlocal("begin"), access_indizes)
        && ConstraintLoadInst("reads[0].value")

        && "compute_expr" + ConstraintPureFunction(nonlocal("reads[0..32].value"),
                                                   nonlocal("begin"), nonlocal("body_sese.begin"));
}
