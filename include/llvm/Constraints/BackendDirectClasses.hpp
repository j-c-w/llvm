#ifndef _BACKEND_DIRECT_CLASSES_HPP_
#define _BACKEND_DIRECT_CLASSES_HPP_
#include "BackendSelectors.hpp"

template<typename ... SolverAtomN>
class BackendAnd;

template<typename SolverAtom1, typename SolverAtom2>
class BackendAnd<SolverAtom1,SolverAtom2> : public SolverAtom
{
public:
    BackendAnd(SolverAtom1 s1, SolverAtom2 s2);

    SkipResult skip_invalid(SolverAtom::Value& c) const final;

    void begin () final;
    void fixate(SolverAtom::Value c) final;
    void resume() final;

private:
    SolverAtom1 constraint_head;
    SolverAtom2 constraint_tail;
};

template<typename SolverAtom1, typename SolverAtom2>
BackendAnd<SolverAtom1,SolverAtom2>::BackendAnd(SolverAtom1 s1, SolverAtom2 s2)
                                   : constraint_head(std::move(s1)), constraint_tail(std::move(s2)) { }

template<typename SolverAtom1, typename SolverAtom2>
SkipResult BackendAnd<SolverAtom1,SolverAtom2>::skip_invalid(SolverAtom::Value& c) const
{
    SkipResult first_result = constraint_head.skip_invalid(c);

    if(first_result == SkipResult::FAIL)
        return SkipResult::FAIL;

    if(first_result == SkipResult::CHANGE)
        return SkipResult::CHANGE;

    SkipResult second_result = constraint_tail.skip_invalid(c);

    if(second_result == SkipResult::FAIL)
        return SkipResult::FAIL;

    if(second_result != SkipResult::PASS)
        return SkipResult::CHANGE;

    return first_result;
}

template<typename SolverAtom1, typename SolverAtom2>
void BackendAnd<SolverAtom1,SolverAtom2>::begin()
{
    constraint_head.begin();
    constraint_tail.begin();
}

template<typename SolverAtom1, typename SolverAtom2>
void BackendAnd<SolverAtom1,SolverAtom2>::fixate(SolverAtom::Value c)
{
    constraint_head.fixate(c);
    constraint_tail.fixate(c);
}

template<typename SolverAtom1, typename SolverAtom2>
void BackendAnd<SolverAtom1,SolverAtom2>::resume()
{
    constraint_head.resume();
    constraint_tail.resume();
}

template<typename SolverAtom1, typename SolverAtom2, typename SolverAtom3, typename ... SolverAtomN>
class BackendAnd<SolverAtom1,SolverAtom2,SolverAtom3,SolverAtomN...>
    : public BackendAnd<SolverAtom1,BackendAnd<SolverAtom2,SolverAtom3,SolverAtomN...>>
{
public:
    BackendAnd(SolverAtom1 s1, SolverAtom2 s2, SolverAtom3 s3, SolverAtomN ... sn)
     : BackendAnd<SolverAtom1,BackendAnd<SolverAtom2,SolverAtom3,SolverAtomN...>>
       (s1, BackendAnd<SolverAtom2,SolverAtom3,SolverAtomN...>(s2, s3, sn...)) { }
};








namespace {
template<unsigned,typename>
class HelperClass;

template<unsigned n, typename ... T>
class HelperClass<n,std::tuple<T...>>
{
private:
    static const auto N = std::tuple_size<std::tuple<T...>>::value;

public:
    static SkipResult skip_invalid(const std::array<unsigned,N>& disabled_since, const std::tuple<T...>& constraints,
                                   SolverAtom::Value& c, SolverAtom::Value c_so_far, SkipResult result_so_far)
    {
        if(std::get<n>(disabled_since) == 0)
        {
            SolverAtom::Value c_copy = c;
            SkipResult new_result = std::get<n>(constraints).skip_invalid(c_copy);

            if(new_result == SkipResult::PASS)
            {
                return SkipResult::PASS;
            }

            if(new_result != SkipResult::FAIL && (result_so_far == SkipResult::FAIL || c_copy < c_so_far))
            {
                c_so_far = c_copy;
                result_so_far = new_result;
            }
        }

        if(n+1 < N)
        {
            return HelperClass<(n+1)%N,std::tuple<T...>>::skip_invalid(disabled_since, constraints, c,
                                                                       c_so_far, result_so_far);
        }
        else
        {
            c = c_so_far;
            return result_so_far;
        }
    }

    static void begin(std::array<unsigned,N>& disabled_since, std::tuple<T...>& constraints)
    {
        if(std::get<n>(disabled_since) == 0) std::get<n>(constraints).begin();

        if(n+1 < N) HelperClass<(n+1)%N,std::tuple<T...>>::begin(disabled_since, constraints);
    }

    static void fixate(std::array<unsigned,N>& disabled_since, std::tuple<T...>& constraints, SolverAtom::Value c)
    {
        if(std::get<n>(disabled_since) > 0) std::get<n>(disabled_since) ++;
        else
        {
            SolverAtom::Value temp = c;

            if(std::get<n>(constraints).skip_invalid(temp) != SkipResult::PASS)
            {
                std::get<n>(disabled_since) = 1;
            }
            else
            {
                std::get<n>(constraints).fixate(c);
            }
        }

        if(n+1 < N) HelperClass<(n+1)%N,std::tuple<T...>>::fixate(disabled_since, constraints, c);
    }

    static void resume(std::array<unsigned,N>& disabled_since, std::tuple<T...>& constraints)
    {
        if(std::get<n>(disabled_since) > 0) std::get<n>(disabled_since)--;
        else                                std::get<n>(constraints).resume();

        if(n+1 < N) HelperClass<(n+1)%N,std::tuple<T...>>::resume(disabled_since, constraints);
    }

};
}

template<unsigned N,typename ... SolverAtomN>
class BackendOr;

template<unsigned N>
class BackendOr<N>
{
public:
    BackendOr() : disabled_since_member{} { }

    template<unsigned idx> SkipResult skip_invalid(SolverAtom::Value& c) const { return SkipResult::PASS; }

    template<unsigned idx> void begin () {}
    template<unsigned idx> void fixate(SolverAtom::Value c) {}
    template<unsigned idx> void resume() {}

    std::array<unsigned,N>& disabled_since() { return disabled_since_member; }
    const std::array<unsigned,N>& disabled_since() const { return disabled_since_member; }

private:
    std::array<unsigned,N> disabled_since_member;
};

template<unsigned N, typename ... SolverAtom1, typename ... SolverAtomN>
class BackendOr<N, std::tuple<SolverAtom1...>,SolverAtomN...>
{
public:
    BackendOr(std::tuple<SolverAtom1...> atoms, SolverAtomN ... oN);

    template<unsigned idx> SkipResult skip_invalid(SolverAtom::Value& c) const;

    template<unsigned idx> void begin ();
    template<unsigned idx> void fixate(SolverAtom::Value c);
    template<unsigned idx> void resume();

    std::array<unsigned,N>& disabled_since() { return next.disabled_since(); }
    const std::array<unsigned,N>& disabled_since() const { return next.disabled_since(); }

private:
    std::tuple<SolverAtom1...>  constraints;
    BackendOr<N,SolverAtomN...> next;
};

template<unsigned N, typename ... SolverAtom1, typename ... SolverAtomN>
BackendOr<N,std::tuple<SolverAtom1...>,SolverAtomN...>::BackendOr(std::tuple<SolverAtom1...> atoms, SolverAtomN ... oN)
                                                                   : constraints(atoms), next(oN...) { }

template<unsigned N, typename ... SolverAtom1, typename ... SolverAtomN>
template<unsigned idx>
SkipResult BackendOr<N,std::tuple<SolverAtom1...>,SolverAtomN...>::skip_invalid(SolverAtom::Value& c) const
{
    if(idx != 0) return next.template skip_invalid<idx-1>(c);

    return HelperClass<0,std::tuple<SolverAtom1...>>::skip_invalid(disabled_since(), constraints, c, c, SkipResult::FAIL);
}

template<unsigned N, typename ... SolverAtom1, typename ... SolverAtomN>
template<unsigned idx>
void BackendOr<N, std::tuple<SolverAtom1...>,SolverAtomN...>::begin()
{
    if(idx != 0) return next.template begin<idx-1>();

    HelperClass<0,std::tuple<SolverAtom1...>>::begin(disabled_since(), constraints);
}

template<unsigned N, typename ... SolverAtom1, typename ... SolverAtomN>
template<unsigned idx>
void BackendOr<N, std::tuple<SolverAtom1...>,SolverAtomN...>::fixate(SolverAtom::Value c)
{
    if(idx != 0) return next.template fixate<idx-1>(c);

    HelperClass<0,std::tuple<SolverAtom1...>>::fixate(disabled_since(), constraints, c);
}

template<unsigned N, typename ... SolverAtom1, typename ... SolverAtomN>
template<unsigned idx>
void BackendOr<N, std::tuple<SolverAtom1...>,SolverAtomN...>::resume()
{
    if(idx != 0) return next.template resume<idx-1>();

    HelperClass<0,std::tuple<SolverAtom1...>>::resume(disabled_since(), constraints);
}

#endif
