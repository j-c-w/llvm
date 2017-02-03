#ifndef _CONSTRAINT_INTERFACE_HPP_
#define _CONSTRAINT_INTERFACE_HPP_
#include <vector>
#include <map>
#include <climits>
#include <memory>
#include <deque>
#include <iostream>

enum class SkipResult
{
    FAIL       = 0,
    PASS       = 1,
    CHANGE     = 2,
    CHANGEPASS = 3
};

/* This class implements the representation of constraints that the solver expects to perform its backtracking
   algorithm.
   Essentially the solver guarantees to call the begin, resume, fixate, cancel members whenever it moves between
   variables and they allow the instances of this class to "prepare" for being needed.
   The "skip_invalid" member function is the crucial one and increments the argument that is passed to it until
   it does not contradict any constraint anymore.
   the return value depends on whether this was successfull. */
template<typename ValueType>
class Specialized
{
public:
    virtual ~Specialized() { }

    virtual SkipResult skip_invalid(ValueType& c) = 0;

    virtual void begin () = 0;
    virtual void resume() = 0;
    virtual void fixate() = 0;
    virtual void cancel() = 0;
};

/* Constraints essentially only exist to generate instances of the above shown Specialized class. */
template<typename LabelType,typename ValueType>
class Constraint
{
public:
    virtual ~Constraint() { };

    virtual std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> get_specializations() const = 0;
};

/* The class performs the backtracking algorithm. */
template<typename LabelType, typename ValueType>
class Solver
{
public:
    Solver(const Constraint<LabelType,ValueType>& constraint);

    std::vector<std::pair<LabelType,ValueType>> next_solution(unsigned max_steps = UINT_MAX);

protected:
    unsigned                                                                  iterator;
    std::vector<LabelType*>                                                   variables;
    std::vector<ValueType>                                                    solution;
    std::vector<std::pair<LabelType,std::unique_ptr<Specialized<ValueType>>>> specializations;
};

#endif
