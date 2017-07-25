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
    ConstraintAnd(std::vector<Constraint*> cvec);

    std::vector<Constraint::Label> get_labels(std::vector<Constraint::Label> use_vector = {}) const final;

    std::vector<SpecializedContainer>
             get_specials(FunctionWrap& wrap, std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<Constraint::Label>           labels;
    std::vector<std::vector<unsigned>>       groupings;
};

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that at least one of
   them is satisfied. */
class ConstraintOr : public Constraint
{
public:
    ConstraintOr(std::vector<Constraint*> cvec);

    std::vector<Constraint::Label>    get_labels(std::vector<Constraint::Label> use_vector = {}) const final;
    std::vector<SpecializedContainer> get_specials(FunctionWrap& wrap, std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<Constraint::Label>           labels;
    std::vector<std::vector<unsigned>>       groupings;
};

class ConstraintCollect : public Constraint
{
public:
    ConstraintCollect(unsigned n, std::string prefix, Constraint* c);

    std::vector<std::string> get_labels(std::vector<std::string> use_vector = {}) const final;

    std::vector<SpecializedContainer> get_specials(FunctionWrap& wrap,
                                                  std::vector<SpecializedContainer> use_vector = {}) const final;

private:
    std::unique_ptr<Constraint> constraint;
    std::vector<std::string>    labels;
    std::vector<unsigned>       global_indices;
    std::vector<unsigned>       local_indices;
    unsigned                    size;
};

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

template<typename Backend,unsigned N>
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

    std::vector<SpecializedContainer> get_specials(FunctionWrap& wrap, std::vector<SpecializedContainer> use_vector = {}) const final
    {
        std::shared_ptr<Backend> backend(new Backend(wrap));

        if(N > 0) use_vector.emplace_back(ScalarSelector<Backend,(N>0?0:0)>(backend));
        if(N > 1) use_vector.emplace_back(ScalarSelector<Backend,(N>1?1:0)>(backend));
        if(N > 2) use_vector.emplace_back(ScalarSelector<Backend,(N>2?2:0)>(backend));

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

template<typename Backend,unsigned N>
class ConstraintVector : public Constraint
{
public:
    template<typename ... LabelTypes>
    ConstraintVector(std::string v1, LabelTypes ... vars)
      : variables{{{v1}, {vars}...}}, sizes{{1, (unsigned)sizeof(vars)*0+1 ...}} { }

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

    std::vector<SpecializedContainer> get_specials(FunctionWrap& wrap, std::vector<SpecializedContainer> use_vector = {}) const final
    {
        auto temp_sizes = sizes;
        if(temp_sizes[2] == 0)
        {
            temp_sizes[2] = temp_sizes[1];
            temp_sizes[1] = temp_sizes[0];
            temp_sizes[0] = 0;
        }

        std::shared_ptr<Backend> backend(new Backend(temp_sizes, wrap));

        if(N > 0) for(unsigned i = 0; i < temp_sizes[0]; i++) use_vector.emplace_back(VectorSelector<Backend,(N>0?0:0)>(backend, i));
        if(N > 1) for(unsigned i = 0; i < temp_sizes[1]; i++) use_vector.emplace_back(VectorSelector<Backend,(N>1?1:0)>(backend, i));
        if(N > 2) for(unsigned i = 0; i < temp_sizes[2]; i++) use_vector.emplace_back(VectorSelector<Backend,(N>2?2:0)>(backend, i));

        return use_vector;
    }

private:
    std::array<std::vector<std::string>,N> variables;
    std::array<unsigned,N> sizes;
};

#endif
