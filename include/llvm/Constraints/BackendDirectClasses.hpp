#ifndef _BACKEND_DIRECT_CLASSES_HPP_
#define _BACKEND_DIRECT_CLASSES_HPP_
#include "BackendSelectors.hpp"

template<typename ... SolverAtomN>
class BackendDirectAnd;

template<typename SolverAtom1, typename SolverAtom2>
class BackendDirectAnd<SolverAtom1,SolverAtom2> : public SolverAtom
{
public:
    BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2);

    SkipResult skip_invalid(SolverAtom::Value& c) const final;

    void begin () final;
    void fixate(SolverAtom::Value c) final;
    void resume() final;

private:
    SolverAtom1 constraint_head;
    SolverAtom2 constraint_tail;
};

template<typename SolverAtom1, typename SolverAtom2>
BackendDirectAnd<SolverAtom1,SolverAtom2>::BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2)
                                         : constraint_head(std::move(s1)), constraint_tail(std::move(s2)) { }

template<typename SolverAtom1, typename SolverAtom2>
SkipResult BackendDirectAnd<SolverAtom1,SolverAtom2>::skip_invalid(SolverAtom::Value& c) const
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
void BackendDirectAnd<SolverAtom1,SolverAtom2>::begin()
{
    constraint_head.begin();
    constraint_tail.begin();
}

template<typename SolverAtom1, typename SolverAtom2>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::fixate(SolverAtom::Value c)
{
    constraint_head.fixate(c);
    constraint_tail.fixate(c);
}

template<typename SolverAtom1, typename SolverAtom2>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::resume()
{
    constraint_head.resume();
    constraint_tail.resume();
}

template<typename SolverAtom1, typename SolverAtom2, typename SolverAtom3, typename ... SolverAtomN>
class BackendDirectAnd<SolverAtom1,SolverAtom2,SolverAtom3,SolverAtomN...>
    : public BackendDirectAnd<SolverAtom1,BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>>
{
public:
    BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2, SolverAtom3 s3, SolverAtomN ... sn)
     : BackendDirectAnd<SolverAtom1,BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>>
       (s1, BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>(s2, s3, sn...)) { }

};

template<typename ... SolverAtomN>
BackendDirectAnd<SolverAtomN...> make_backend_direct_and(SolverAtomN ... specials)
{
    return std::make_shared<BackendDirectAnd<SolverAtomN...>>(specials ...);
}

#endif
