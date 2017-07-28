#ifndef _CONSTRAINT_CLASSES_HPP_
#define _CONSTRAINT_CLASSES_HPP_
#include "llvm/Constraints/BackendSpecializations.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"
#include <vector>

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that all of them are
   satisfied at once. */
class ConstraintAnd : public Constraint
{
public:
    ConstraintAnd(ConstraintAnd&&) = default;

    ConstraintAnd(std::vector<Constraint*> cvec);

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final;

private:
    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<std::vector<unsigned>>       groupings;
};

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that at least one of
   them is satisfied. */
class ConstraintOr : public Constraint
{
public:
    ConstraintOr(std::vector<Constraint*> cvec);

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final;

private:
    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<std::vector<unsigned>>       groupings;
};

class ConstraintCollect : public Constraint
{
public:
    ConstraintCollect(unsigned n, std::string prefix, Constraint* c);

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final;

private:
    std::unique_ptr<Constraint> constraint;
    std::vector<unsigned>       global_indices;
    std::vector<unsigned>       local_indices;
    unsigned                    size;
};

template<typename Backend>
class ConstraintSingle : public Constraint
{
public:
    ConstraintSingle(std::string var) : Constraint {{var}} { }

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final
    {
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new Backend(wrap)));
    }

    std::tuple<Backend> get_typed_specials(FunctionWrap& wrap) const
    {
        return {wrap};
    }
};

template<typename Backend,unsigned N>
class ConstraintScalar;

template<typename Backend>
class ConstraintScalar<Backend,2> : public Constraint
{
public:
    ConstraintScalar(std::string var1, std::string var2) : Constraint{{var1,var2}} { }

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final
    {
        std::shared_ptr<Backend> backend(new Backend(wrap));
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new ScalarSelector<Backend,0>(backend)));
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new ScalarSelector<Backend,1>(backend)));
    }

    std::tuple<ScalarSelector<Backend,0>,
               ScalarSelector<Backend,1>> get_typed_specials(FunctionWrap& wrap) const
    {
        std::shared_ptr<Backend> backend(new Backend(wrap));
        return std::make_tuple(backend, backend);
    }
};

template<typename Backend>
class ConstraintScalar<Backend,3> : public Constraint
{
public:
    ConstraintScalar(std::string var1, std::string var2, std::string var3) : Constraint{{var1,var2,var3}} { }

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final
    {
        std::shared_ptr<Backend> backend(new Backend(wrap));
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new ScalarSelector<Backend,0>(backend)));
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new ScalarSelector<Backend,1>(backend)));
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new ScalarSelector<Backend,2>(backend)));
    }

    std::tuple<ScalarSelector<Backend,0>,
               ScalarSelector<Backend,1>,
               ScalarSelector<Backend,2>> get_typed_specials(FunctionWrap& wrap) const
    {
        std::shared_ptr<Backend> backend(new Backend(wrap));
        return std::make_tuple(backend, backend, backend);
    }
};

template<typename Backend,unsigned N>
class ConstraintVector;

template<typename Backend>
class ConstraintVector<Backend,1> : public Constraint
{
public:
    ConstraintVector(std::vector<std::string> var)
    {
        variables[0] = var.size();
        emplace_back(var);
    }

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final
    {
        auto specials = get_typed_specials(wrap);
        for(auto s : std::get<0>(specials)) use_vector.emplace_back(llvm::make_unique<decltype(s)>(s));
    }
    std::tuple<std::vector<VectorSelector<Backend>>> get_typed_specials(FunctionWrap& wrap) const
    {
        std::vector<VectorSelector<Backend>> special1;
        std::shared_ptr<Backend> backend(new Backend(variables, wrap));
        for(unsigned i = 0; i < variables[0]; i++) special1.emplace_back(backend, i);
        return std::make_tuple(special1);
    }

private:
    std::array<unsigned,1> variables;
};

template<typename Backend>
class ConstraintVector<Backend,2> : public Constraint
{
public:
    ConstraintVector(std::vector<std::string> var1, std::vector<std::string> var2)
    {
        variables[0] = var1.size();
        insert(end(), var1.begin(), var1.end());
        variables[1] = var2.size();
        insert(end(), var2.begin(), var2.end());
    }

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final
    {
        auto specials = get_typed_specials(wrap);
        for(auto s : std::get<0>(specials)) use_vector.emplace_back(llvm::make_unique<decltype(s)>(s));
        for(auto s : std::get<1>(specials)) use_vector.emplace_back(llvm::make_unique<decltype(s)>(s));
    }

    std::tuple<std::vector<MultiVectorSelector<Backend,0>>,
               std::vector<MultiVectorSelector<Backend,1>>> get_typed_specials(FunctionWrap& wrap) const
    {
        std::vector<MultiVectorSelector<Backend,0>> special1;
        std::vector<MultiVectorSelector<Backend,1>> special2;
        std::shared_ptr<Backend> backend(new Backend(variables, wrap));
        for(unsigned i = 0; i < variables[0]; i++) special1.emplace_back(backend, i);
        for(unsigned i = 0; i < variables[1]; i++) special2.emplace_back(backend, i);
        return std::make_tuple(special1, special2);
    }

private:
    std::array<unsigned,2> variables;
};

template<typename Backend>
class ConstraintVector<Backend,3> : public Constraint
{
public:
    ConstraintVector(std::string v1, std::string v2) : Constraint{{v1, v2}}, variables{{0,1,1}} { }
    ConstraintVector(std::string v1, std::string v2, std::string v3) : Constraint{{v1, v2, v3}}, variables{{1,1,1}} { }

    ConstraintVector(std::vector<std::string> v1, std::vector<std::string> v2, std::vector<std::string> v3)
    {
        variables[0] = v1.size();
        insert(end(), v1.begin(), v1.end());
        variables[1] = v2.size();
        insert(end(), v2.begin(), v2.end());
        variables[2] = v3.size();
        insert(end(), v3.begin(), v3.end());
    }

    void get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const final
    {
        auto specials = get_typed_specials(wrap);
        for(auto s : std::get<0>(specials)) use_vector.emplace_back(llvm::make_unique<decltype(s)>(s));
        for(auto s : std::get<1>(specials)) use_vector.emplace_back(llvm::make_unique<decltype(s)>(s));
        for(auto s : std::get<2>(specials)) use_vector.emplace_back(llvm::make_unique<decltype(s)>(s));
    }

    std::tuple<std::vector<MultiVectorSelector<Backend,0>>,
               std::vector<MultiVectorSelector<Backend,1>>,
               std::vector<MultiVectorSelector<Backend,2>>> get_typed_specials(FunctionWrap& wrap) const
    {
        std::vector<MultiVectorSelector<Backend,0>> special1;
        std::vector<MultiVectorSelector<Backend,1>> special2;
        std::vector<MultiVectorSelector<Backend,2>> special3;
        std::shared_ptr<Backend> backend(new Backend(variables, wrap));
        for(unsigned i = 0; i < variables[0]; i++) special1.emplace_back(backend, i);
        for(unsigned i = 0; i < variables[1]; i++) special2.emplace_back(backend, i);
        for(unsigned i = 0; i < variables[2]; i++) special3.emplace_back(backend, i);
        return std::make_tuple(special1, special2, special3);
    }

private:
    std::array<unsigned,3> variables;
};

#endif
