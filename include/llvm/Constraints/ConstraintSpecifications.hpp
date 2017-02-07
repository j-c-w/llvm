#ifndef _CONSTRAINTCOMPOSED_HPP_
#define _CONSTRAINTCOMPOSED_HPP_
#include "llvm/Constraints/ConstraintPrefix.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"

/* This file containts the constraint specifications that are used for idiom detection. */
ConstraintOr <std::string,unsigned> ConstraintPermute2(std::string in1,  std::string in2,
                                                       std::string out1, std::string out2);
ConstraintOr <std::string,unsigned> ConstraintPermute3(std::string in1,  std::string in2,  std::string in3,
                                                       std::string out1, std::string out2, std::string out3);

ConstraintAnd<std::string,unsigned> ConstraintBinaryChain         (FunctionWrapper& wrap, unsigned Opcode, unsigned N);
ConstraintAnd<std::string,unsigned> ConstraintDistributive        (FunctionWrapper& wrap, unsigned SumL, unsigned SumR,
                                                                                          unsigned ProL, unsigned ProR);
ConstraintAnd<std::string,unsigned> ConstraintHoistSelect         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintPointerIterator     (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintSESE                (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintSameSESE            (std::string prefix1, std::string prefix2);
ConstraintAnd<std::string,unsigned> ConstraintUnusedSESE          ();
ConstraintOr <std::string,unsigned> ConstraintExtendedInt         (FunctionWrapper& wrap, std::string input,
                                                                                          std::string output);
ConstraintOr <std::string,unsigned> ConstraintLocallyConstant     (FunctionWrapper& wrap, std::string var,
                                                                                          std::string sese);
ConstraintAnd<std::string,unsigned> ConstraintLoop                (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedLoop          ();
ConstraintAnd<std::string,unsigned> ConstraintFor                 (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedFor           ();
ConstraintAnd<std::string,unsigned> ConstraintNestedFor           (FunctionWrapper& wrap, std::string prefix, std::string parent);
ConstraintOr <std::string,unsigned> ConstraintAddition            (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAddition      ();
ConstraintOr <std::string,unsigned> ConstraintMultiplication      (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedMultiplication();
ConstraintAnd<std::string,unsigned> ConstraintAffineAccess1       (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineAccess1 ();
ConstraintAnd<std::string,unsigned> ConstraintAffineAccess2       (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineAccess2 ();
ConstraintAnd<std::string,unsigned> ConstraintAffineAccess3       (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineAccess3 ();
ConstraintAnd<std::string,unsigned> ConstraintAffineRead1         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineRead1   ();
ConstraintAnd<std::string,unsigned> ConstraintAffineRead2         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineRead2   ();
ConstraintAnd<std::string,unsigned> ConstraintAffineRead3         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedAffineRead3   ();
ConstraintAnd<std::string,unsigned> ConstraintScale               (FunctionWrapper& wrap, std::string prefix,
                                                                                          std::string history,
                                                                                          std::string input);
ConstraintAnd<std::string,unsigned> ConstraintWeightedSum         (FunctionWrapper& wrap, std::string prefix,
                                                                                          std::string history,
                                                                                          std::string input1,
                                                                                          std::string input2);
ConstraintAnd<std::string,unsigned> ConstraintSumReduction         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintMatrixmatrix         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintSparseMV             (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintPureFunction         (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedPureFunction   ();
ConstraintAnd<std::string,unsigned> ConstraintScalarReduction      (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnusedScalarReduction();
ConstraintAnd<std::string,unsigned> ConstraintHistogram            (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> Constraint3DStencilAccess      (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintUnused3DStencilAccess();
ConstraintAnd<std::string,unsigned> Constraint3DStencil            (FunctionWrapper& wrap);
ConstraintAnd<std::string,unsigned> ConstraintReductionLoop        (FunctionWrapper& wrap);

#endif
