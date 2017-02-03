#ifndef _CONSTRAINT_ORDERING_HPP_
#define _CONSTRAINT_ORDERING_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

/* This class implements constraints that guarantee an order between two values. This order is shallow, i.e. it just
   compares indizes in a std::vector<llvm::Value*>.
   Therefore anything but the equality operator can just be used to remove unneccessary levels of freedom in a
   constraint formula to avoid dupicate solutions. */
template<typename ValueType, typename LabelType, bool lt, bool eq, bool gt>
class ConstraintOrdering : public Constraint<LabelType,ValueType>
{
public:
    ConstraintOrdering(LabelType var1, LabelType var2);

    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const final;

private:
    struct SharedData
    {
        unsigned temp_value1;
        unsigned temp_value2;
        unsigned amount_completed;
    };

    template<bool back>
    class SpecializedOrdering : public Specialized<ValueType>
    {
    public:
        SpecializedOrdering(std::shared_ptr<SharedData> c);

        SkipResult skip_invalid(ValueType& c) final;

        void begin () final;
        void resume() final;
        void fixate() final;
        void cancel() final;

        std::shared_ptr<SharedData> cache;
    };

    LabelType variable1;
    LabelType variable2;
};

#endif
