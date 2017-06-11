#ifndef _CONSTRAINTS_BASIC_HPP_
#define _CONSTRAINTS_BASIC_HPP_
#include "llvm/Constraints/BackendSpecializations.hpp"
#include "llvm/Constraints/ConstraintCollect.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"
#include "llvm/Constraints/ConstraintPrefix.hpp"
#include "llvm/Constraints/ConstraintAtomic.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/ConstraintRange.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"

/* In this file, many individual constraints are defined that are simply specializations of some of the very genric
   constraint definitions in the included files. */
template<typename Type1, typename Type2,
         typename std::enable_if<std::is_base_of<Constraint,Type1>::value>::type* = nullptr,
         typename std::enable_if<std::is_base_of<Constraint,Type2>::value>::type* = nullptr>
ConstraintAnd operator&&(Type1 t1, Type2 t2)
{
    return ConstraintAnd(std::move(t1), std::move(t2));
}

template<typename Type1, typename Type2,
         typename std::enable_if<std::is_base_of<Constraint,Type1>::value>::type* = nullptr,
         typename std::enable_if<std::is_base_of<Constraint,Type2>::value>::type* = nullptr>
ConstraintOr operator||(Type1 t1, Type2 t2)
{
    return ConstraintOr(std::move(t1), std::move(t2));
}

template<typename Type,
         typename std::enable_if<std::is_base_of<Constraint,Type>::value>::type* = nullptr>
ConstraintRange operator*(unsigned size, Type constraint)
{
    return {size, std::move(constraint)};
}

template<typename Type,
         typename std::enable_if<std::is_base_of<Constraint,Type>::value>::type* = nullptr>
ConstraintRange operator+(unsigned size, Type constraint)
{
    return {1, std::move(constraint), size};
}

template<typename Type,
         typename std::enable_if<std::is_base_of<Constraint,Type>::value>::type* = nullptr>
ConstraintPrefix operator+(std::string prefix, Type constraint)
{
    return {prefix, std::move(constraint)};
}

template<typename Type,
         typename std::enable_if<std::is_base_of<Constraint,Type>::value>::type* = nullptr>
ConstraintCollect operator*(std::pair<std::string,unsigned> params, Type constraint)
{
    return {params.second, std::move(params.first), std::move(constraint)};
}

inline std::string nonlocal(std::string s)
{
    std::string sout = "@";

    for(char c : s)
    {
        if(c == ',') sout += ",@";
        else         sout.push_back(c);
    }

    return sout;
}

using ConstraintUnused = ConstraintScalar<BackendUnused>;

using ConstraintIntegerType = ConstraintScalar<BackendIntegerType>;
using ConstraintFloatType   = ConstraintScalar<BackendFloatType>;
using ConstraintPointerType = ConstraintScalar<BackendPointerType>;

using ConstraintNotNumericConstant = ConstraintScalar<BackendNotNumericConstant>;
using ConstraintConstant           = ConstraintScalar<BackendConstant>;
using ConstraintPreexecution       = ConstraintScalar<BackendPreexecution>;
using ConstraintArgument           = ConstraintScalar<BackendArgument>;
using ConstraintInstruction        = ConstraintScalar<BackendInstruction>;

using ConstraintStoreInst  = ConstraintScalar<BackendStoreInst>;
using ConstraintLoadInst   = ConstraintScalar<BackendLoadInst>;
using ConstraintReturnInst = ConstraintScalar<BackendReturnInst>;
using ConstraintBranchInst = ConstraintScalar<BackendBranchInst>;
using ConstraintAddInst    = ConstraintScalar<BackendAddInst>;
using ConstraintSubInst    = ConstraintScalar<BackendSubInst>;
using ConstraintMulInst    = ConstraintScalar<BackendMulInst>;
using ConstraintFAddInst   = ConstraintScalar<BackendFAddInst>;
using ConstraintFSubInst   = ConstraintScalar<BackendFSubInst>;
using ConstraintFMulInst   = ConstraintScalar<BackendFMulInst>;
using ConstraintFDivInst   = ConstraintScalar<BackendFDivInst>;
using ConstraintSelectInst = ConstraintScalar<BackendSelectInst>;
using ConstraintSExtInst   = ConstraintScalar<BackendSExtInst>;
using ConstraintZExtInst   = ConstraintScalar<BackendZExtInst>;
using ConstraintGEPInst    = ConstraintScalar<BackendGEPInst>;
using ConstraintICmpInst   = ConstraintScalar<BackendICmpInst>;

using ConstraintSame     = ConstraintScalar<BackendSame>;
using ConstraintDistinct = ConstraintScalar<BackendDistinct>;
using ConstraintOrder    = ConstraintScalar<BackendOrder>;

using ConstraintSameBlock     = ConstraintScalar<BackendSameBlockWrap>;
using ConstraintIncomingValue = ConstraintScalar<BackendIncomingValueWrap>;

using ConstraintDFGEdge  = ConstraintScalar<BackendDFGEdge>;
using ConstraintCFGEdge  = ConstraintScalar<BackendCFGEdge>;
using ConstraintCDGEdge  = ConstraintScalar<BackendCDGEdge>;
using ConstraintPDGEdge  = ConstraintScalar<BackendPDGEdge>;

using ConstraintFirstArgument  = ConstraintScalar<BackendFirstArgument>;
using ConstraintSecondArgument = ConstraintScalar<BackendSecondArgument>;
using ConstraintThirdArgument  = ConstraintScalar<BackendThirdArgument>;
using ConstraintFourthArgument = ConstraintScalar<BackendFourthArgument>;

using ConstraintDFGDominate       = ConstraintVector<BackendDFGDominate>;
using ConstraintDFGPostdom        = ConstraintVector<BackendDFGPostdom>;
using ConstraintDFGDominateStrict = ConstraintVector<BackendDFGDominateStrict>;
using ConstraintDFGPostdomStrict  = ConstraintVector<BackendDFGPostdomStrict>;

using ConstraintCFGDominate       = ConstraintVector<BackendCFGDominate>;
using ConstraintCFGPostdom        = ConstraintVector<BackendCFGPostdom>;
using ConstraintCFGDominateStrict = ConstraintVector<BackendCFGDominateStrict>;
using ConstraintCFGPostdomStrict  = ConstraintVector<BackendCFGPostdomStrict>;

using ConstraintPDGDominate       = ConstraintVector<BackendPDGDominate>;
using ConstraintPDGPostdom        = ConstraintVector<BackendPDGPostdom>;
using ConstraintPDGDominateStrict = ConstraintVector<BackendPDGDominateStrict>;
using ConstraintPDGPostdomStrict  = ConstraintVector<BackendPDGPostdomStrict>;

using ConstraintDFGNotDominate       = ConstraintVector<BackendDFGNotDominate>;
using ConstraintDFGNotPostdom        = ConstraintVector<BackendDFGNotPostdom>;
using ConstraintDFGNotDominateStrict = ConstraintVector<BackendDFGNotDominateStrict>;
using ConstraintDFGNotPostdomStrict  = ConstraintVector<BackendDFGNotPostdomStrict>;

using ConstraintCFGNotDominate       = ConstraintVector<BackendCFGNotDominate>;
using ConstraintCFGNotPostdom        = ConstraintVector<BackendCFGNotPostdom>;
using ConstraintCFGNotDominateStrict = ConstraintVector<BackendCFGNotDominateStrict>;
using ConstraintCFGNotPostdomStrict  = ConstraintVector<BackendCFGNotPostdomStrict>;

using ConstraintPDGNotDominate       = ConstraintVector<BackendPDGNotDominate>;
using ConstraintPDGNotPostdom        = ConstraintVector<BackendPDGNotPostdom>;
using ConstraintPDGNotDominateStrict = ConstraintVector<BackendPDGNotDominateStrict>;
using ConstraintPDGNotPostdomStrict  = ConstraintVector<BackendPDGNotPostdomStrict>;

using ConstraintCFGBlocked = ConstraintVector<BackendCFGBlocked>;
using ConstraintDFGBlocked = ConstraintVector<BackendDFGBlocked>;
using ConstraintPDGBlocked = ConstraintVector<BackendPDGBlocked>;

class ConstraintPHINode : public ConstraintAnd
{
public:
    ConstraintPHINode(std::string var, std::string begin)
      : ConstraintAnd(ConstraintScalar<ScalarBackend<BackendOpcode_<llvm::Instruction::PHI>,1>>(var),
                                            ConstraintSameBlock(begin, var)) { } 
};

#endif
