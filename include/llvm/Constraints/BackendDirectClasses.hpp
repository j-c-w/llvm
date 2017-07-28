#ifndef _BACKEND_DIRECT_CLASSES_HPP_
#define _BACKEND_DIRECT_CLASSES_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include <vector>
#include <memory>

template<typename ... SolverAtomN>
class BackendDirectAnd;

template<typename SolverAtom1, typename SolverAtom2>
class BackendDirectAnd<SolverAtom1,SolverAtom2>
{
public:
    BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2);

    template<unsigned idx> SkipResult skip_invalid(SolverAtom::Value& c);

    template<unsigned idx> void begin ();
    template<unsigned idx> void fixate(SolverAtom::Value c);
    template<unsigned idx> void resume(SolverAtom::Value c);
    template<unsigned idx> void cancel();

private:
    std::pair<SolverAtom1,SolverAtom2> constraints;
};

template<typename SolverAtom1, typename SolverAtom2>
BackendDirectAnd<SolverAtom1,SolverAtom2>::BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2)
                                         : constraints(std::move(s1), std::move(s2)) { }

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
SkipResult BackendDirectAnd<SolverAtom1,SolverAtom2>::skip_invalid(SolverAtom::Value& c)
{
    SkipResult local_result = constraints.first.skip_invalid(c);

    if(local_result == SkipResult::FAIL)
    {
        return SkipResult::FAIL;
    }
    else if(local_result == SkipResult::CHANGE || local_result == SkipResult::CHANGEPASS)
    {
        return SkipResult::CHANGE;
    }

    local_result = constraints.second.skip_invalid(c);

    if(local_result == SkipResult::FAIL)
    {
        return SkipResult::FAIL;
    }
    else if(local_result == SkipResult::CHANGE || local_result == SkipResult::CHANGEPASS)
    {
        return SkipResult::CHANGE;
    }

    return SkipResult::PASS;
}

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::begin()
{
    constraints.first.begin();
    constraints.second.begin();
}

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::fixate(SolverAtom::Value c)
{
    constraints.first.fixate(c);
    constraints.second.fixate(c);
}

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::resume(SolverAtom::Value c)
{
    constraints.first.resume(c);
    constraints.second.resume(c);
}

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::cancel()
{
    constraints.first.cancel();
    constraints.second.cancel();
}

template<typename SolverAtom1, typename SolverAtom2, typename SolverAtom3, typename ... SolverAtomN>
class BackendDirectAnd<SolverAtom1,SolverAtom2,SolverAtom3,SolverAtomN...>
    : public BackendDirectAnd<SolverAtom1,ScalarSelector<BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>,0>>
{
public:
    BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2, SolverAtom3 s3, SolverAtomN ... sn)
     : BackendDirectAnd<SolverAtom1,ScalarSelector<BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>,0>>
       (s1, ScalarSelector<BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>,0>
             (std::make_shared<BackendDirectAnd<SolverAtom2,SolverAtom3,SolverAtomN...>>(s2, s3, sn...))) { }

};

template<typename ... SolverAtomN>
ScalarSelector<BackendDirectAnd<SolverAtomN...>,0> make_backend_direct_and(SolverAtomN ... specials)
{
    return std::make_shared<BackendDirectAnd<SolverAtomN...>>(specials ...);
}

#endif
