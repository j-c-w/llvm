#ifndef _BACKEND_DIRECT_CLASSES_HPP_
#define _BACKEND_DIRECT_CLASSES_HPP_

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
    template<unsigned idx> void resume();

private:
    SolverAtom1 constraint_head;
    SolverAtom2 constraint_tail;
};

template<typename SolverAtom1, typename SolverAtom2>
BackendDirectAnd<SolverAtom1,SolverAtom2>::BackendDirectAnd(SolverAtom1 s1, SolverAtom2 s2)
                                         : constraint_head(std::move(s1)), constraint_tail(std::move(s2)) { }

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
SkipResult BackendDirectAnd<SolverAtom1,SolverAtom2>::skip_invalid(SolverAtom::Value& c)
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
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::begin()
{
    constraint_head.begin();
    constraint_tail.begin();
}

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::fixate(SolverAtom::Value c)
{
    constraint_head.fixate(c);
    constraint_tail.fixate(c);
}

template<typename SolverAtom1, typename SolverAtom2>
template<unsigned idx>
void BackendDirectAnd<SolverAtom1,SolverAtom2>::resume()
{
    constraint_head.resume();
    constraint_tail.resume();
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
