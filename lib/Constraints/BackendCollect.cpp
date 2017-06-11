#include "llvm/Constraints/Backends.hpp"

BackendCollect_::BackendCollect_(std::array<unsigned,2>, std::vector<SpecializedContainer> nloc,
                                                         std::vector<SpecializedContainer> loc)
               : nonlocals(std::move(nloc)), locals(std::move(loc)),
                 filled_nonlocals(0), filled_locals(locals.size(), 0)
{ }

template<unsigned idx1>
SkipResult BackendCollect_::skip_invalid(unsigned idx2, Specialized::Value &c)
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

template<unsigned idx1>
void BackendCollect_::begin(unsigned idx2)
{
    if(idx1 == 0)
    {
        nonlocals[idx2]->begin();
    }
}

template<unsigned idx1>
void BackendCollect_::fixate(unsigned idx2, Specialized::Value c)
{
	unsigned max_steps = 25000000;
    if(idx1 == 0)
    {
        nonlocals[idx2]->fixate(c);

        if(++filled_nonlocals == nonlocals.size())
        {
            std::vector<Specialized::Value> solution;

            unsigned iterator = 0;

            if(iterator < locals.size())
            {
                solution.push_back(0);
                locals[0]->begin();
            }

            while(iterator < locals.size() && (max_steps--))
            {
                while(iterator < locals.size())
                {
                    SkipResult result = SkipResult::CHANGE;

                    while(result == SkipResult::CHANGE)
                    {
                        result = locals[iterator]->skip_invalid(solution[iterator]);
                    }

                    if(result != SkipResult::FAIL)
                    {
                        locals[iterator]->fixate(solution[iterator]);
                        ++iterator;

                        if(iterator <  locals.size())
                        {
                            solution.push_back(0);
                            locals[iterator]->begin();
                        }
                    }
                    else
                    {
                        locals[iterator--]->cancel();

                        solution.pop_back();

                        if(iterator <  locals.size())
                        {
                            locals[iterator]->resume(solution[iterator]);
                            ++solution[iterator];
                        }
                    }
                }

                if(iterator == locals.size())
                {
                    solutions.insert(solutions.end(), solution.begin(), solution.end());

                    iterator--;
                    locals[iterator]->resume(solution[iterator]);
                    solution[iterator]++;
                }
            }
        }
    }
    else if(idx1 == 1)
    {
        filled_locals[idx2%locals.size()]++;
    }
}

template<unsigned idx1>
void BackendCollect_::resume(unsigned idx2, Specialized::Value c)
{
    if(idx1 == 0)
    {
        if(filled_nonlocals-- == nonlocals.size())
        {
            solutions.clear();
        }

        nonlocals[idx2]->resume(c);
    }
    else if(idx1 == 1)
    {
        filled_locals[idx2%locals.size()]--;
    }
}

template<unsigned idx1>
void BackendCollect_::cancel(unsigned idx2)
{
    if(idx1 == 0)
    {
        nonlocals[idx2]->cancel();
    }
}

template SkipResult BackendCollect_::skip_invalid<0>(unsigned,Specialized::Value&);
template SkipResult BackendCollect_::skip_invalid<1>(unsigned,Specialized::Value&);
template       void BackendCollect_::begin<0>(unsigned);
template       void BackendCollect_::begin<1>(unsigned);
template       void BackendCollect_::fixate<0>(unsigned,Specialized::Value);
template       void BackendCollect_::fixate<1>(unsigned,Specialized::Value);
template       void BackendCollect_::resume<0>(unsigned,Specialized::Value);
template       void BackendCollect_::resume<1>(unsigned,Specialized::Value);
template       void BackendCollect_::cancel<0>(unsigned);
template       void BackendCollect_::cancel<1>(unsigned);
