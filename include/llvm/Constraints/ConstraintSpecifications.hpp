#ifndef _CONSTRAINTCOMPOSED_HPP_
#define _CONSTRAINTCOMPOSED_HPP_
#include "llvm/Constraints/ConstraintAtomic.hpp"

// This file containts the constraint specifications that are used for idiom detection. 
ConstraintAnd ConstraintDistributive();
ConstraintAnd ConstraintHoistSelect();
ConstraintAnd ConstraintGEMM();
ConstraintAnd ConstraintGEMV();
ConstraintAnd ConstraintAXPY();
ConstraintAnd ConstraintDOT();
ConstraintAnd ConstraintSPMV();
ConstraintAnd ConstraintReduction();
ConstraintAnd ConstraintHisto();
ConstraintAnd ConstraintStencil();

#endif
