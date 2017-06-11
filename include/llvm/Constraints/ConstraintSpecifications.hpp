#ifndef _CONSTRAINTCOMPOSED_HPP_
#define _CONSTRAINTCOMPOSED_HPP_
#include "llvm/Constraints/ConstraintPrefix.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"

/* This file containts the constraint specifications that are used for idiom detection. */
ConstraintContainer ConstraintPermute(std::vector<std::string> in, std::vector<std::string> out);
ConstraintContainer ConstraintVectorSame(std::vector<std::string> in, std::vector<std::string> out);
ConstraintContainer ConstraintVectorSame(std::string a, std::string b);
ConstraintAnd       ConstraintArguments(std::string dst, std::string src1, std::string src2);
ConstraintOr        ConstraintArgumentsPermuted(std::string dst, std::string src1, std::string src2);
ConstraintAnd       ConstraintSumChain();
ConstraintAnd       ConstraintMulChain();
ConstraintAnd       ConstraintDistributive();
ConstraintAnd       ConstraintHoistSelect();
ConstraintOr        ConstraintLocallyConstant(std::string var, std::string border);
ConstraintAnd       ConstraintSESE();
ConstraintAnd       ConstraintInSESE(std::string var, std::string sese);
ConstraintAnd       ConstraintMaxOnceInSESE(std::string var, std::string sese);
ConstraintAnd       ConstraintOnceInSESE(std::string var, std::string sese);
ConstraintOr        ConstraintExtendedInt(std::string input, std::string output);
ConstraintAnd       ConstraintLoop();
ConstraintAnd       ConstraintInductionVar(std::string old_ind, std::string new_ind);
ConstraintAnd       ConstraintFor();
ConstraintAnd       ConstraintNestedFor(std::string prefix, std::string parent);
ConstraintAnd       ConstraintForNest(unsigned dim);
ConstraintPrefix    ConstraintAfterLoop(std::string var, std::string loop);
ConstraintOr        ConstraintAddition();
ConstraintOr        ConstraintMultiplication();
ConstraintAnd       ConstraintArrayAccess(std::string origin);
ConstraintAnd       ConstraintAffineAccess(unsigned dim, std::string origin);
ConstraintAnd       ConstraintStencilAccess(unsigned dim, std::string origin);
ConstraintAnd       ConstraintArrayRead(std::string origin);
ConstraintAnd       ConstraintArrayStore(std::string origin);
ConstraintAnd       ConstraintPermutedMultidimRead(std::string origin, std::vector<std::string> indices);
ConstraintAnd       ConstraintPermutedMultidimStore(std::string origin, std::vector<std::string> indices);
ConstraintAnd       ConstraintStencilRead(unsigned dim, std::string origin);
ConstraintAnd       ConstraintScale(std::string history, std::string input);
ConstraintAnd       ConstraintWeightedSum(std::string history, std::string input1, std::string input2);
ConstraintAnd       ConstraintSumReduction();
ConstraintAnd       ConstraintMatrixmatrix();
ConstraintAnd       ConstraintSparseMV();
ConstraintAnd       ConstraintPureFunction(std::string outer, std::string inner);
ConstraintAnd       ConstraintScalarReduction();
ConstraintAnd       ConstraintHistogram();
ConstraintAnd       ConstraintStencil(unsigned dim);

#endif
