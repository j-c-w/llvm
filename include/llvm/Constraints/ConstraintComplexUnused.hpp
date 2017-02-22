#ifndef _CONSTRAINT_COMPLEX_UNUSED_HPP_
#define _CONSTRAINT_COMPLEX_UNUSED_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>
#include <vector>

class ConstraintComplexUnused : public Constraint<std::string,unsigned>
{
public:
    template<typename  Type>
    ConstraintComplexUnused(Type c) : constraint(new Type(c)) { }

    std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> get_specializations() const final
    {
        std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> result;

        for(auto& entry : constraint->get_specializations())
        {
            if(entry.first.empty() || entry.first[0] != '@')
            {
                result.emplace_back(entry.first, std::unique_ptr<Specialized<unsigned>>(new SpecializedComplexUnused));
            }
        }

        return result;
    }

private:

    class SpecializedComplexUnused : public Specialized<unsigned>
    {
    public:
        SkipResult skip_invalid(unsigned& c) final
        {
            if(c < UINT_MAX-1)
            {
                c = UINT_MAX-1;
                return SkipResult::CHANGEPASS;
            }
            else if(c == UINT_MAX-1)
            {
                return SkipResult::PASS;
            }
            else return SkipResult::FAIL;
        }

        void begin () final { };
        void resume() final { };
        void fixate() final { };
        void cancel() final { };
    };

    std::shared_ptr<Constraint<std::string,unsigned>> constraint;
};

#endif
