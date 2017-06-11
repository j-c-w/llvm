#ifndef _BACKEND_BASE_CLASSES_HPP_
#define _BACKEND_BASE_CLASSES_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/Constraint.hpp"
#include <memory>
#include <tuple>

template<typename Derived,unsigned N>
class ScalarBackend
{
public:
    template<unsigned idx,typename std::enable_if<(idx<N)>::type* = nullptr>
    class Selector : public Specialized
    {
    public:
        Selector(std::shared_ptr<Derived> b) : base(b) { }

        SkipResult skip_invalid(Specialized::Value& c) final { return base->template skip_invalid<idx>(c); }

        void begin()                      final { base->template begin<idx>(); }
        void fixate(Specialized::Value c) final { base->template fixate<idx>(c); }
        void resume(Specialized::Value c) final { base->template resume<idx>(c); }
        void cancel()                     final { base->template cancel<idx>(); }

        operator SpecializedContainer() const { return SpecializedContainer(new Selector<idx>(*this)); }

    private:
        std::shared_ptr<Derived> base;
    };

    template<unsigned M, typename DUMMY=void>
    struct TupleBuilder
    {
        using Type = decltype(std::tuple_cat(TupleBuilder<M-1>::build(nullptr), std::tuple<Selector<M-1>>({nullptr})));

        static Type build(std::shared_ptr<Derived> backend)
        {
            return std::tuple_cat(TupleBuilder<M-1>::build(backend), std::tuple<Selector<M-1>>({backend}));
        }
    };

    template<typename DUMMY>
    struct TupleBuilder<0,DUMMY>
    {
        using Type = std::tuple<>;

        static Type build(std::shared_ptr<Derived> backend)
        {
            return {};
        }
    };

public:
    template<typename ... ConstructorParam>
    static typename TupleBuilder<N>::Type Create(ConstructorParam&& ... param)
    {
        std::shared_ptr<Derived> shared_backend(new Derived(std::forward<ConstructorParam>(param) ...));

        return TupleBuilder<N>::build(shared_backend);
    }
};

template<typename Derived,unsigned N>
class VectorBackend
{
public:
    template<unsigned idx1, typename std::enable_if<(idx1<N)>::type* = nullptr>
    class Selector : public Specialized
    {
    public:
        Selector(std::shared_ptr<Derived> b, unsigned i2) : base(b), idx2(i2) { }

        SkipResult skip_invalid(Specialized::Value &c) final { return base->template skip_invalid<idx1>(idx2, c); }

        void begin()                      final { base->template begin<idx1>(idx2); }
        void fixate(Specialized::Value c) final { base->template fixate<idx1>(idx2, c); }
        void resume(Specialized::Value c) final { base->template resume<idx1>(idx2, c); }
        void cancel()                     final { base->template cancel<idx1>(idx2); }

        operator SpecializedContainer() const { return SpecializedContainer(new Selector<idx1>(*this)); }

    private:
        std::shared_ptr<Derived> base;
        unsigned                 idx2;
    };

    template<unsigned M, typename DUMMY=void>
    struct TupleBuilder
    {
        using Type = decltype(std::tuple_cat(std::declval<typename TupleBuilder<M-1>::Type>(),
                                             std::declval<std::tuple<std::vector<Selector<M-1>>>>()));

        static Type build(std::shared_ptr<Derived> backend, std::array<unsigned,N> sizes)
        {
            std::vector<Selector<M-1>> result_part;

            for(unsigned i = 0; i < std::get<M-1>(sizes); i++)
            {
                result_part.emplace_back(backend, i);
            }

            return std::tuple_cat(TupleBuilder<M-1>::build(backend, sizes), std::make_tuple(std::move(result_part)));
        }
    };

    template<typename DUMMY>
    struct TupleBuilder<0,DUMMY>
    {
        using Type = std::tuple<>;

        static Type build(std::shared_ptr<Derived> backend, std::array<unsigned,N>)
        {
            return {};
        }
    };

    template<typename ... ConstructorParam>
    static typename TupleBuilder<N>::Type Create(std::array<unsigned,N> size, ConstructorParam&& ... context)
    {
        std::shared_ptr<Derived> shared_backend(new Derived(size, std::forward<ConstructorParam>(context) ...));

        return TupleBuilder<N>::build(shared_backend, size);
    }
};

#endif