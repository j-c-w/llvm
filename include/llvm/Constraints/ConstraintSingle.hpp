#ifndef _CONSTRAINT_SINGLE_HPP_
#define _CONSTRAINT_SINGLE_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"
#include <iostream>
#include <sstream>
#include <list>

/* This is the base class for constraints that only depend on a single variable. */
template<typename LabelType, typename ValueType>
class ConstraintSingle : public Constraint<LabelType,ValueType>
{
public:
    class SpecializedSingle : public Specialized<ValueType>
    {
    public:
        SpecializedSingle(const std::vector<ValueType>& hits);

        SkipResult skip_invalid(ValueType& c) final;

        void begin () final { hit_start = hits.begin(); };
        void resume() final { };
        void fixate() final { };
        void cancel() final { };

    private:
        std::vector<ValueType>                          hits;
        typename std::vector<ValueType>::const_iterator hit_start;
    };

    ConstraintSingle(LabelType var);
    ConstraintSingle(LabelType var, std::vector<ValueType> hits);
    ConstraintSingle(LabelType var, const std::function<bool(ValueType)>& pred, ValueType begin, ValueType end);

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final;

protected:
    LabelType              variable;
    std::vector<ValueType> hits;
};

#endif
