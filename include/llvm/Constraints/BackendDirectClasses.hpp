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

/*
template<unsigned globals,unsigned locals>
class BackendCollectDirect
{
public:
    BackendCollectDirect(std::array<unsigned,2> size, std::vector<std::unique_ptr<SolverAtom>> nloc,
                                                      std::vector<std::unique_ptr<SolverAtom>> loc);

    template<unsigned idx1> SkipResult skip_invalid(unsigned idx2, SolverAtom::Value &c) const;

    template<unsigned idx1> void begin (unsigned idx2);
    template<unsigned idx1> void fixate(unsigned idx2, SolverAtom::Value c);
    template<unsigned idx1> void resume(unsigned idx2);

private:
    std::vector<std::unique_ptr<SolverAtom>> nonlocals;
    std::vector<std::unique_ptr<SolverAtom>> locals;
    unsigned                                 filled_nonlocals;
    std::vector<unsigned>                    filled_locals;
    std::vector<SolverAtom::Value>           solutions;
};

template<unsigned globals,unsigned locals>
BackendCollectDirect<globals,locals>::BackendCollectDirect(std::array<unsigned,2>,
                                                           std::vector<std::unique_ptr<SolverAtom>> nloc,
                                                           std::vector<std::unique_ptr<SolverAtom>> loc)
               : nonlocals(std::move(nloc)), locals(std::move(loc)),
                 filled_nonlocals(0), filled_locals(locals.size(), 0)
{ }

template<unsigned globals,unsigned locals>
template<unsigned idx1>
SkipResult BackendCollectDirect<globals,locals>::skip_invalid(unsigned idx2, SolverAtom::Value &c) const
{
    if(idx1 == 0)
    {
        return nonlocals[idx2]->skip_invalid(c);
    }
    else if(idx1 == 1)
    {
        unsigned flat_solution_idx = filled_locals[idx2%locals.size()] * locals.size() + (idx2%locals.size());

        if(flat_solution_idx < solutions.size())
        {
            if(c == solutions[flat_solution_idx])
            {
                return SkipResult::PASS;
            }
            else if(c < solutions[flat_solution_idx])
            {
                c = solutions[flat_solution_idx];
                return SkipResult::CHANGEPASS;
            }
            else return SkipResult::FAIL;
        }
        else if(c == UINT_MAX - 1)
        {
            return SkipResult::PASS;
        }
        else if(c < UINT_MAX - 1)
        {
            c = UINT_MAX - 1;
            return SkipResult::CHANGEPASS;
        }
        else return SkipResult::FAIL;
    }
}

template<unsigned globals,unsigned locals>
template<unsigned idx1>
void BackendCollectDirect<globals,locals>::begin(unsigned idx2)
{
    if(idx1 == 0)
    {
        nonlocals[idx2]->begin();
    }
}

template<unsigned globals,unsigned locals>
template<unsigned idx1>
void BackendCollectDirect<globals,locals>::fixate(unsigned idx2, SolverAtom::Value c)
{
    if(idx1 == 0)
    {
        nonlocals[idx2]->fixate(c);

        if(++filled_nonlocals == nonlocals.size())
        {
            Solver solver(std::move(locals));

            while(true)
            {
                auto solution = solver.next_solution();

                if(solution.empty()) break;

                solutions.insert(solutions.end(), solution.begin(), solution.end());
            }

            locals = solver.swap_specials();
        }
    }
    else if(idx1 == 1)
    {
        filled_locals[idx2%locals.size()]++;
    }
}

template<unsigned globals,unsigned locals>
template<unsigned idx1>
void BackendCollectDirect<globals,locals>::resume(unsigned idx2)
{
    if(idx1 == 0)
    {
        if(filled_nonlocals-- == nonlocals.size())
        {
            solutions.clear();
        }

        nonlocals[idx2]->resume();
    }
    else if(idx1 == 1)
    {
        filled_locals[idx2%locals.size()]--;
    }
}
*/

/* PROBLEM: THE AMOUNT OF ORIGINS IS UNKNOWN AT THIS POINT.
template<unsigned size1,unsigned size2,unsigned size3,bool reverse,bool allow_unstrict>
class BackendDominateDirect
{
public:
    BackendDominateDirect(const std::vector<std::vector<unsigned>>& graph_forw);

    template<unsigned idx1> SkipResult skip_invalid(unsigned& c) const;

    template<unsigned idx1> void begin() { }

    template<unsigned idx> void fixate(unsigned c)
    {
        constexpr unsigned idx1   = (idx < size1) ? 0 : (idx < size1+size2) ? 1 : 2;
        constexpr unsigned offset = (idx1 == 0) ? 0 : (idx1 == 1) ? size1 : size2;

        if(c != UINT_MAX-1)
        {
            std::get<idx>(value_masks) = true;
            filled_values[offset+used_values[idx1]++] = c;
        }

        remaining_values[idx1]--;
    }
    template<unsigned idx> void resume()
    {
        constexpr unsigned idx1   = (idx < size1) ? 0 : (idx < size1+size2) ? 1 : 2;
        constexpr unsigned offset = (idx1 == 0) ? 0 : (idx1 == 1) ? size1 : size2;

        if(std::get<idx>(value_masks))
        {
            std::get<idx>(value_masks) = false;
            used_values[idx1]--;
        }

        remaining_values[idx1]++;
    }
    
private:
    mutable GraphEngine                    graph_engine;
    std::vector<std::vector<unsigned>>     graph_forw;
    std::array<unsigned,3>                 used_values;
    std::array<unsigned,3>                 remaining_values;
    std::array<unsigned,size1+size2+size3> filled_values;
    std::array<bool,size1+size2+size3>     value_masks;
};


template<unsigned size1,unsigned size2,unsigned size3,bool reverse,bool allow_unstrict>
BackendDominateDirect<size1,size2,size3,reverse,allow_unstrict>
                     ::BackendDominateDirect(const std::vector<std::vector<unsigned>>& gf)
                     : graph_engine(gf.size()), graph_forw(gf), used_values{{0,0,0}},
                       remaining_values{{size1,size2,size3}},
                       filled_values{},
                       value_masks{} { }

template<unsigned size1,unsigned size2,unsigned size3,bool reverse,bool allow_unstrict>
template<unsigned idx>
SkipResult BackendDominateDirect<size1,size2,size3,reverse,allow_unstrict>::skip_invalid(SolverAtom::Value& c) const
{
    constexpr unsigned idx1 = (idx < size1) ? 0 : (idx < size1+size2) ? 1 : 2;

    if(c >= graph_forw.size() && c != UINT_MAX-1)
    {
        if(c < UINT_MAX-1)
        {
            c = UINT_MAX-1;
            return SkipResult::CHANGE;
        }
        else return SkipResult::FAIL;
    }
    else 
    {
        if(remaining_values[(idx1+1)%3] + remaining_values[(idx1+2)%3] > 0 || (idx1 != 1 && remaining_values[idx1] > 1))
        {
            return SkipResult::PASS;
        }
        else
        {
            bool domination_holds = false;

            graph_engine.initialize();

            if(allow_unstrict)
            {
                graph_engine.set_destinations(filled_values.begin()+size1+size2, filled_values.begin()+size1+size2 + used_values[2]);

                if(c != UINT_MAX-1 && idx1 == 2) graph_engine.set_destinations(&c, &c+1);

                graph_engine.set_dominators(filled_values.begin()+size1, filled_values.begin()+size1 + used_values[1]);

                if(c != UINT_MAX-1 && idx1 == 1)  graph_engine.set_dominators(&c, &c+1);
            }
            else
            {
                graph_engine.set_dominators(filled_values.begin()+size1, filled_values.begin()+size1 + used_values[1]);

                if(c != UINT_MAX-1 && idx1 == 1) graph_engine.set_dominators(&c, &c+1);

                graph_engine.set_destinations(filled_values.begin()+size1+size2, filled_values.begin()+size1+size2 + used_values[2]);

                if(c != UINT_MAX-1 && idx1 == 2) graph_engine.set_destinations(&c, &c+1);
            }


            if(graph_engine.set_origins(filled_values.begin(), filled_values.begin() + used_values[0]) &&
               (c == UINT_MAX-1 || idx1 != 0 || graph_engine.set_origins(&c, &c+1)))
            {
                domination_holds = graph_engine.fill(graph_forw);
            }

            if(remaining_values[1] == (idx1==1) && ((domination_holds && reverse) || (!domination_holds && !reverse)))
            {
                if(c >= UINT_MAX-1)
                {
                    return SkipResult::FAIL;
                }
                else if(c + 1 < graph_forw.size())
                {
                    c = c + 1;
                    return SkipResult::CHANGE;
                }
                else
                {
                   c = UINT_MAX-1;
                   return SkipResult::CHANGE;
                }
            }

            return SkipResult::PASS;
        }
    }
}
*/
#endif
