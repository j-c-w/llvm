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
ConstraintRange operator*(int size, Type constraint)
{
    if(size < 0)
        return {(unsigned)-size, std::move(constraint)};
    else
        return {(unsigned)+size, std::move(constraint)};
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

using ConstraintUnused = ConstraintScalar<BackendUnused_,1>;

using ConstraintIntegerType = ConstraintScalar<BackendIntegerType_,1>;
using ConstraintFloatType   = ConstraintScalar<BackendFloatType_,1>;
using ConstraintPointerType = ConstraintScalar<BackendPointerType_,1>;

using ConstraintNotNumericConstant = ConstraintScalar<BackendNotNumericConstant_,1>;
using ConstraintConstant           = ConstraintScalar<BackendConstant_,1>;
using ConstraintPreexecution       = ConstraintScalar<BackendPreexecution_,1>;
using ConstraintArgument           = ConstraintScalar<BackendArgument_,1>;
using ConstraintInstruction        = ConstraintScalar<BackendInstruction_,1>;

using ConstraintPHIInst    = ConstraintScalar<BackendPHIInst_,1>;
using ConstraintStoreInst  = ConstraintScalar<BackendStoreInst_,1>;
using ConstraintLoadInst   = ConstraintScalar<BackendLoadInst_,1>;
using ConstraintReturnInst = ConstraintScalar<BackendReturnInst_,1>;
using ConstraintBranchInst = ConstraintScalar<BackendBranchInst_,1>;
using ConstraintAddInst    = ConstraintScalar<BackendAddInst_,1>;
using ConstraintSubInst    = ConstraintScalar<BackendSubInst_,1>;
using ConstraintMulInst    = ConstraintScalar<BackendMulInst_,1>;
using ConstraintFAddInst   = ConstraintScalar<BackendFAddInst_,1>;
using ConstraintFSubInst   = ConstraintScalar<BackendFSubInst_,1>;
using ConstraintFMulInst   = ConstraintScalar<BackendFMulInst_,1>;
using ConstraintFDivInst   = ConstraintScalar<BackendFDivInst_,1>;
using ConstraintBitOrInst  = ConstraintScalar<BackendBitOrInst_,1>;
using ConstraintLShiftInst = ConstraintScalar<BackendLShiftInst_,1>;
using ConstraintSelectInst = ConstraintScalar<BackendSelectInst_,1>;
using ConstraintSExtInst   = ConstraintScalar<BackendSExtInst_,1>;
using ConstraintZExtInst   = ConstraintScalar<BackendZExtInst_,1>;
using ConstraintGEPInst    = ConstraintScalar<BackendGEPInst_,1>;
using ConstraintICmpInst   = ConstraintScalar<BackendICmpInst_,1>;

using ConstraintSame     = ConstraintScalar<BackendSame_,2>;
using ConstraintDistinct = ConstraintScalar<BackendDistinct_,2>;
using ConstraintOrder    = ConstraintScalar<BackendOrder_,2>;

using ConstraintSameBlock     = ConstraintScalar<BackendSameBlock_,2>;
using ConstraintIncomingValue = ConstraintScalar<BackendIncomingValue_,3>;

using ConstraintDFGEdge  = ConstraintScalar<BackendDFGEdge_,2>;
using ConstraintCFGEdge  = ConstraintScalar<BackendCFGEdge_,2>;
using ConstraintCDGEdge  = ConstraintScalar<BackendCDGEdge_,2>;
using ConstraintPDGEdge  = ConstraintScalar<BackendPDGEdge_,2>;

using ConstraintFirstArgument  = ConstraintScalar<BackendFirstArgument_,2>;
using ConstraintSecondArgument = ConstraintScalar<BackendSecondArgument_,2>;
using ConstraintThirdArgument  = ConstraintScalar<BackendThirdArgument_,2>;
using ConstraintFourthArgument = ConstraintScalar<BackendFourthArgument_,2>;

using ConstraintDFGDominate       = ConstraintVector<BackendDFGDominate_,3>;
using ConstraintDFGPostdom        = ConstraintVector<BackendDFGPostdom_,3>;
using ConstraintDFGDominateStrict = ConstraintVector<BackendDFGDominateStrict_,3>;
using ConstraintDFGPostdomStrict  = ConstraintVector<BackendDFGPostdomStrict_,3>;

using ConstraintCFGDominate       = ConstraintVector<BackendCFGDominate_,3>;
using ConstraintCFGPostdom        = ConstraintVector<BackendCFGPostdom_,3>;
using ConstraintCFGDominateStrict = ConstraintVector<BackendCFGDominateStrict_,3>;
using ConstraintCFGPostdomStrict  = ConstraintVector<BackendCFGPostdomStrict_,3>;

using ConstraintPDGDominate       = ConstraintVector<BackendPDGDominate_,3>;
using ConstraintPDGPostdom        = ConstraintVector<BackendPDGPostdom_,3>;
using ConstraintPDGDominateStrict = ConstraintVector<BackendPDGDominateStrict_,3>;
using ConstraintPDGPostdomStrict  = ConstraintVector<BackendPDGPostdomStrict_,3>;

using ConstraintDFGNotDominate       = ConstraintVector<BackendDFGNotDominate_,3>;
using ConstraintDFGNotPostdom        = ConstraintVector<BackendDFGNotPostdom_,3>;
using ConstraintDFGNotDominateStrict = ConstraintVector<BackendDFGNotDominateStrict_,3>;
using ConstraintDFGNotPostdomStrict  = ConstraintVector<BackendDFGNotPostdomStrict_,3>;

using ConstraintCFGNotDominate       = ConstraintVector<BackendCFGNotDominate_,3>;
using ConstraintCFGNotPostdom        = ConstraintVector<BackendCFGNotPostdom_,3>;
using ConstraintCFGNotDominateStrict = ConstraintVector<BackendCFGNotDominateStrict_,3>;
using ConstraintCFGNotPostdomStrict  = ConstraintVector<BackendCFGNotPostdomStrict_,3>;

using ConstraintPDGNotDominate       = ConstraintVector<BackendPDGNotDominate_,3>;
using ConstraintPDGNotPostdom        = ConstraintVector<BackendPDGNotPostdom_,3>;
using ConstraintPDGNotDominateStrict = ConstraintVector<BackendPDGNotDominateStrict_,3>;
using ConstraintPDGNotPostdomStrict  = ConstraintVector<BackendPDGNotPostdomStrict_,3>;

using ConstraintCFGBlocked = ConstraintVector<BackendCFGBlocked_,3>;
using ConstraintDFGBlocked = ConstraintVector<BackendDFGBlocked_,3>;
using ConstraintPDGBlocked = ConstraintVector<BackendPDGBlocked_,3>;

class ConstraintPHINode : public ConstraintAnd
{
public:
    ConstraintPHINode(std::string var, std::string begin)
      : ConstraintAnd(ConstraintScalar<BackendOpcode_<llvm::Instruction::PHI>,1>(var),
                      ConstraintSameBlock(begin, var)) { } 
};

#endif
