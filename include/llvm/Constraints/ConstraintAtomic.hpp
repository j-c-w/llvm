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

template<typename BackendType>
class ConstraintScalar;

template<typename Derived,unsigned N>
class ConstraintScalar<ScalarBackend<Derived,N>> : public Constraint
{
private:
    class UntypedBacked
    {
    public:
        template<typename Type, unsigned M, typename DUMMY=void>
        struct VectorBuilder
        {
            static std::vector<SpecializedContainer> build(Type backend)
            {
                auto last = std::move(std::get<M-1>(backend));

                std::vector<SpecializedContainer> result = VectorBuilder<Type,M-1>::build(backend);

                result.emplace_back(std::move(last));

                return result;
            }
        };

        template<typename Type,typename DUMMY>
        struct VectorBuilder<Type,0,DUMMY>
        {
            static std::vector<SpecializedContainer> build(Type backend)
            {
                return std::vector<SpecializedContainer>();
            }
        };

        template<typename ... ConstructorParam>
        static typename std::vector<SpecializedContainer> Create(std::tuple<ConstructorParam...> backend)
        {
            return VectorBuilder<std::tuple<ConstructorParam...>,
                                 std::tuple_size<std::tuple<ConstructorParam...>>::value>::build(std::move(backend));
        }
    };

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
        for(auto& special : UntypedBacked::Create(ScalarBackend<Derived,N>::Create(wrap)))
            use_vector.emplace_back(std::move(special));

        return use_vector;
    }

private:
    std::array<std::string,N> variables;
};

template<typename BackendType>
class ConstraintVector;

template<typename Derived,unsigned N>
class ConstraintVector<VectorBackend<Derived,N>> : public Constraint
{
private:
    class UntypedBacked
    {
    public:
        template<typename Type, unsigned M, typename DUMMY=void>
        struct VectorBuilder
        {
            static std::vector<SpecializedContainer> build(Type backend)
            {
                auto last = std::move(std::get<M-1>(backend));

                std::vector<SpecializedContainer> result = VectorBuilder<Type,M-1>::build(backend);

                for(auto& special : last)
                {
                    result.emplace_back(std::move(special));
                }

                return result;
            }
        };

        template<typename Type,typename DUMMY>
        struct VectorBuilder<Type,0,DUMMY>
        {
            static std::vector<SpecializedContainer> build(Type backend)
            {
                return std::vector<SpecializedContainer>();
            }
        };

        template<typename ... ConstructorParam>
        static typename std::vector<SpecializedContainer> Create(std::tuple<ConstructorParam...> backend)
        {
            return VectorBuilder<std::tuple<ConstructorParam...>,
                                 std::tuple_size<std::tuple<ConstructorParam...>>::value>::build(std::move(backend));
        }
    };

public:
    template<typename ... LabelTypes>
    ConstraintVector(LabelTypes ... vars)
      : variables{{expand_variables(vars)...}}, sizes{{(unsigned)expand_variables(vars).size() ...}} { }

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

        for(auto& special : UntypedBacked::Create(VectorBackend<Derived,N>::Create(temp_sizes, wrap)))
            use_vector.emplace_back(std::move(special));

        return use_vector;
    }

private:
    std::array<std::vector<std::string>,N> variables;
    std::array<unsigned,N> sizes;
};

#endif
