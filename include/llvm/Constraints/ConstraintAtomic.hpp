#ifndef _CONSTRAINTS_ATOMIC_HPP_
#define _CONSTRAINTS_ATOMIC_HPP_
#include "llvm/Constraints/BackendSpecializations.hpp"
#include "llvm/Constraints/ConstraintCollect.hpp"
#include "llvm/Constraints/ConstraintAnd.hpp"
#include "llvm/Constraints/ConstraintOr.hpp"
#include "llvm/Constraints/ConstraintPrefix.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/ConstraintRange.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"
#include <vector>

inline std::vector<std::string> expand_variables(std::string var)
{
    std::vector<std::string> result;

    std::string::const_iterator part_begin = var.cbegin();
    std::string::const_iterator part_end   = std::find(var.cbegin(), var.cend(), ',');

    while(true)
    {
        std::string::const_iterator idx_begin;
        std::string::const_iterator idx_end;

        if((idx_begin = std::find(part_begin, part_end, '[')) != part_end)
        {
            unsigned begin_int = 0;

            for(idx_end = ++idx_begin; idx_end != part_end; idx_end++)
            {
                if(*idx_end >= '0' && *idx_end <= '9')
                {
                    begin_int = 10 * begin_int + (*idx_end - '0');
                }
                else break;
            }

            if(idx_end+0 != part_end && *(idx_end+0) == '.' && idx_end+1 != part_end && *(idx_end+1) == '.')
            {
                unsigned end_int = 0;

                for(idx_end = idx_end+2; idx_end != part_end; idx_end++)
                {
                    if(*idx_end >= '0' && *idx_end <= '9')
                    {
                        end_int = 10 * end_int + (*idx_end - '0');
                    }
                    else break;
                }

                if(idx_end != part_end && *idx_end == ']' && begin_int <= end_int && end_int < 1000)
                {
                    for(unsigned i = begin_int; i < end_int; i++)
                    {
                        std::string new_string;

                        new_string.reserve((idx_begin-part_begin) + (part_end - idx_end) + 1 + (i>=10) + (i>=100));

                        new_string.insert(new_string.end(), part_begin, idx_begin);
                        if(i>=100) new_string.push_back('0'+((i/100)%10));
                        if(i>=10)  new_string.push_back('0'+((i/10)%10));
                        if(true)   new_string.push_back('0'+((i/1)%10));

                        new_string.insert(new_string.end(), idx_end, part_end);

                        result.push_back(new_string);
                    }
                }
                else result.emplace_back(part_begin, part_end);
            }
            else result.emplace_back(part_begin, part_end);
        }
        else result.emplace_back(part_begin, part_end);

        if(part_end != var.cend())
        {
            part_begin = part_end + 1;
            part_end   = std::find(part_begin, var.cend(), ',');
        }
        else break;
    }

    return result;
}

inline std::vector<std::string> expand_variables(std::string prefix, unsigned N, std::string postfix = "")
{
    std::vector<std::string> result;

    std::string indexed_prefix = prefix+"[";
    for(unsigned i = 0; i < N; i++)
    {
        if(i == 0 || i == 10 || i == 100) indexed_prefix.push_back('0');
        if(i>=100) indexed_prefix[indexed_prefix.size()-3] = '0'+((i/100)%10);
        if(i>=10)  indexed_prefix[indexed_prefix.size()-2] = '0'+((i/10)%10);
        if(true)   indexed_prefix[indexed_prefix.size()-1] = '0'+((i/1)%10);

        result.push_back(indexed_prefix+"]"+(postfix.empty()?"":("."+postfix)));
    }

    return result;
}

template<typename Backend, unsigned idx>
class ScalarSelector : public Specialized
{
public:
    ScalarSelector(std::shared_ptr<Backend> b) : base(b) { }

    SkipResult skip_invalid(Specialized::Value& c) final { return base->template skip_invalid<idx>(c); }

    void begin()                      final { base->template begin<idx>(); }
    void fixate(Specialized::Value c) final { base->template fixate<idx>(c); }
    void resume(Specialized::Value c) final { base->template resume<idx>(c); }
    void cancel()                     final { base->template cancel<idx>(); }

    operator SpecializedContainer() const { return SpecializedContainer(new ScalarSelector<Backend,idx>(*this)); }

private:
    std::shared_ptr<Backend> base;
};

template<typename Derived,unsigned N>
class ConstraintScalar : public Constraint
{
public:
    template<typename ... LabelTypes>
    ConstraintScalar(LabelTypes ... vars)
      : variables{{vars...}} { }

    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final
    {
        for(const auto& var : variables)
            use_vector.emplace_back(var);

        return use_vector;
    }

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap, std::vector<SpecializedContainer> use_vector = {}) const final
    {
        std::shared_ptr<Derived> backend(new Derived(wrap));

        if(N > 0) use_vector.emplace_back(ScalarSelector<Derived,(N>0?0:0)>(backend));
        if(N > 1) use_vector.emplace_back(ScalarSelector<Derived,(N>1?1:0)>(backend));
        if(N > 2) use_vector.emplace_back(ScalarSelector<Derived,(N>2?2:0)>(backend));

        return use_vector;
    }

private:
    std::array<std::string,N> variables;
};

template<typename Backend, unsigned idx1>
class VectorSelector : public Specialized
{
public:
    VectorSelector(std::shared_ptr<Backend> b, unsigned i2) : base(b), idx2(i2) { }

    SkipResult skip_invalid(Specialized::Value &c) final { return base->template skip_invalid<idx1>(idx2, c); }

    void begin()                      final { base->template begin<idx1>(idx2); }
    void fixate(Specialized::Value c) final { base->template fixate<idx1>(idx2, c); }
    void resume(Specialized::Value c) final { base->template resume<idx1>(idx2, c); }
    void cancel()                     final { base->template cancel<idx1>(idx2); }

    operator SpecializedContainer() const { return SpecializedContainer(new VectorSelector<Backend,idx1>(*this)); }

private:
    std::shared_ptr<Backend> base;
    unsigned                 idx2;
};

template<typename Derived,unsigned N>
class ConstraintVector : public Constraint
{
public:
    template<typename ... LabelTypes>
    ConstraintVector(std::string v1, LabelTypes ... vars)
      : variables{{expand_variables(v1), expand_variables(vars)...}}, sizes{{(unsigned)expand_variables(v1).size(), (unsigned)expand_variables(vars).size() ...}} { }

    template<typename ... LabelTypes>
    ConstraintVector(std::vector<std::string> v1, LabelTypes ... vars)
      : variables{{v1, vars...}}, sizes{{(unsigned)v1.size(), (unsigned)vars.size() ...}} { }


    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final
    {
        for(const auto& vars : variables)
            for(const auto& var : vars)
                use_vector.emplace_back(var);

        return use_vector;
    }

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap, std::vector<SpecializedContainer> use_vector = {}) const final
    {
        auto temp_sizes = sizes;
        if(temp_sizes[2] == 0)
        {
            temp_sizes[2] = temp_sizes[1];
            temp_sizes[1] = temp_sizes[0];
            temp_sizes[0] = 0;
        }

        std::shared_ptr<Derived> backend(new Derived(temp_sizes, wrap));

        if(N > 0) for(unsigned i = 0; i < temp_sizes[0]; i++) use_vector.emplace_back(VectorSelector<Derived,(N>0?0:0)>(backend, i));
        if(N > 1) for(unsigned i = 0; i < temp_sizes[1]; i++) use_vector.emplace_back(VectorSelector<Derived,(N>1?1:0)>(backend, i));
        if(N > 2) for(unsigned i = 0; i < temp_sizes[2]; i++) use_vector.emplace_back(VectorSelector<Derived,(N>2?2:0)>(backend, i));

        return use_vector;
    }

private:
    std::array<std::vector<std::string>,N> variables;
    std::array<unsigned,N> sizes;
};

#endif
