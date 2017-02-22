#ifndef _CONSTRAINT_COMPLEX_SAME_HPP_
#define _CONSTRAINT_COMPLEX_SAME_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/ConstraintOrdering.hpp"
#include <memory>
#include <vector>

class ConstraintComplexSame : public Constraint<std::string,unsigned>
{
public:
    template<typename  Type>
    ConstraintComplexSame(std::string p1, std::string p2, Type c)
      : prefix1(p1), prefix2(p2), constraint(new Type(c)) { }

    std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> get_specializations() const final
    {
        std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> result;

        for(auto& entry : constraint->get_specializations())
        {
            if(entry.first.empty() || entry.first[0] != '@')
            {
                std::shared_ptr<SharedData> cache(new SharedData{0, 0, 0});

                result.emplace_back(prefix1+((prefix1.empty()||entry.first.empty())?"":".")+entry.first,
                                    std::unique_ptr<Specialized<unsigned>>(new SpecializedComplexSame<false>(cache)));

                result.emplace_back(prefix2+((prefix2.empty()||entry.first.empty())?"":".")+entry.first,
                                    std::unique_ptr<Specialized<unsigned>>(new SpecializedComplexSame<true>(cache)));
            }
        }

        return result;
    }

private:

    struct SharedData
    {
        unsigned temp_value1;
        unsigned temp_value2;
        unsigned amount_completed;
    };

    template<bool back>
    class SpecializedComplexSame : public Specialized<unsigned>
    {
    public:
        SpecializedComplexSame(std::shared_ptr<SharedData> c) : cache(c) { }

        SkipResult skip_invalid(unsigned& c) final
        {
            if(cache->amount_completed == 1)
            {
                if(c < (back?cache->temp_value1:cache->temp_value2))
                {
                    (back?cache->temp_value2:cache->temp_value1) = c = (back?cache->temp_value1:cache->temp_value2);
                    return SkipResult::CHANGEPASS;
                }
                else if(c == (back?cache->temp_value1:cache->temp_value2))
                {
                    (back?cache->temp_value2:cache->temp_value1) = c = (back?cache->temp_value1:cache->temp_value2);
                    return SkipResult::PASS;
                }
                else return SkipResult::FAIL;
            }
            else
            {
                (back?cache->temp_value2:cache->temp_value1) = c;
                return SkipResult::PASS;
            }
        }

        void begin () final { }
        void resume() final { cache->amount_completed--; }
        void fixate() final { cache->amount_completed++; }
        void cancel() final { }

        std::shared_ptr<SharedData> cache;
    };


    std::string                                       prefix1;
    std::string                                       prefix2;
    std::shared_ptr<Constraint<std::string,unsigned>> constraint;
};

#endif
