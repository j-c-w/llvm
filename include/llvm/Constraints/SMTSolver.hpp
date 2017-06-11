#ifndef _CONSTRAINT_INTERFACE_HPP_
#define _CONSTRAINT_INTERFACE_HPP_
#include "FunctionWrap.hpp"
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
class Specialized
{
public:
    using Value = unsigned;

    virtual ~Specialized() { }

    virtual SkipResult skip_invalid(Value& c) = 0;

    virtual void begin() = 0;
    virtual void fixate(Value c) = 0;
    virtual void resume(Value c) = 0;
    virtual void cancel() = 0;
};

/* The class performs the backtracking algorithm. */
class Solver
{
public:
    Solver(std::vector<std::pair<std::string,std::unique_ptr<Specialized>>> specializations);

    std::vector<std::pair<std::string,Specialized::Value>> next_solution(unsigned max_steps = UINT_MAX);

protected:
    unsigned                                                         iterator;
    std::vector<std::string*>                                        variables;
    std::vector<Specialized::Value>                                  solution;
    std::vector<std::pair<std::string,std::unique_ptr<Specialized>>> specializations;
};

#endif
