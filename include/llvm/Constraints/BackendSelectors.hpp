#ifndef _BACKEND_SELECTOR_HPP_
#define _BACKEND_SELECTOR_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include <memory>

template<typename Backend, unsigned idx>
class ScalarSelector : public SolverAtom
{
public:
    ScalarSelector(std::shared_ptr<Backend> b) : base(b) { }

    SkipResult skip_invalid(SolverAtom::Value& c) final { return base->template skip_invalid<idx>(c); }

    void begin()                     final { base->template begin<idx>(); }
    void fixate(SolverAtom::Value c) final { base->template fixate<idx>(c); }
    void resume()                    final { base->template resume<idx>(); }

private:
    std::shared_ptr<Backend> base;
};

template<typename Backend>
class VectorSelector : public SolverAtom
{
public:
    VectorSelector(std::shared_ptr<Backend> b, unsigned i) : base(b), idx(i) { }

    SkipResult skip_invalid(SolverAtom::Value &c) final { return base->template skip_invalid(idx, c); }

    void begin()                     final { base->begin(idx); }
    void fixate(SolverAtom::Value c) final { base->fixate(idx, c); }
    void resume()                    final { base->resume(idx); }

private:
    std::shared_ptr<Backend> base;
    unsigned                 idx;
};

template<typename Backend, unsigned idx1>
class MultiVectorSelector : public SolverAtom
{
public:
    MultiVectorSelector(std::shared_ptr<Backend> b, unsigned i2) : base(b), idx2(i2) { }

    SkipResult skip_invalid(SolverAtom::Value &c) final { return base->template skip_invalid<idx1>(idx2, c); }

    void begin()                     final { base->template begin<idx1>(idx2); }
    void fixate(SolverAtom::Value c) final { base->template fixate<idx1>(idx2, c); }
    void resume()                    final { base->template resume<idx1>(idx2); }

private:
    std::shared_ptr<Backend> base;
    unsigned                 idx2;
};

#endif
