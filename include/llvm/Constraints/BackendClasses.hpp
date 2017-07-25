#ifndef _BACKENDS_HPP_
#define _BACKENDS_HPP_
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include <vector>

/* This class implements he logical dicjunction in the constraint description system.
   The constructor takes an arbitrary amount of constraints and the resulting constraints enforces that all of them are
   satisfied at once. */
class BackendAnd
{
public:
    BackendAnd(std::vector<SpecializedContainer> c);

    template<unsigned idx> SkipResult skip_invalid(Specialized::Value& c);

    template<unsigned idx> void begin ();
    template<unsigned idx> void fixate(Specialized::Value c);
    template<unsigned idx> void resume(Specialized::Value c);
    template<unsigned idx> void cancel();

private:
    std::vector<SpecializedContainer> constraints;
};

class BackendOr
{
public:
    BackendOr(std::array<unsigned,1>, std::vector<std::vector<SpecializedContainer>> c);

    template<unsigned idx1> SkipResult skip_invalid(unsigned idx, Specialized::Value& c);

    template<unsigned idx1> void begin (unsigned idx);
    template<unsigned idx1> void fixate(unsigned idx, Specialized::Value c);
    template<unsigned idx1> void resume(unsigned idx, Specialized::Value c);
    template<unsigned idx1> void cancel(unsigned idx);

private:
    std::vector<std::vector<SpecializedContainer>> constraints;
    std::vector<unsigned>                          disabled_since;
};

class BackendCollect
{
public:
    BackendCollect(std::array<unsigned,2> size, std::vector<SpecializedContainer> nloc,
                                                std::vector<SpecializedContainer> loc);

    template<unsigned idx1> SkipResult skip_invalid(unsigned idx2, Specialized::Value &c);

    template<unsigned idx1> void begin (unsigned idx2);
    template<unsigned idx1> void fixate(unsigned idx2, Specialized::Value c);
    template<unsigned idx1> void resume(unsigned idx2, Specialized::Value c);
    template<unsigned idx1> void cancel(unsigned idx2);

private:
    std::vector<SpecializedContainer>  nonlocals;
    std::vector<SpecializedContainer>  locals;
    unsigned                           filled_nonlocals;
    std::vector<unsigned>              filled_locals;
    std::vector<Specialized::Value>    solutions;
};

class BackendSingle
{
public:
    BackendSingle(std::vector<Specialized::Value> hits);

    template<unsigned idx> SkipResult skip_invalid(Specialized::Value& c);

    template<unsigned idx> void begin ()                     { hit_start = hits.begin(); }
    template<unsigned idx> void fixate(Specialized::Value c) { }
    template<unsigned idx> void resume(Specialized::Value c) { }
    template<unsigned idx> void cancel()                     { }

private:
    std::vector<Specialized::Value>                          hits;
    typename std::vector<Specialized::Value>::const_iterator hit_start;
};

class BackendEdge
{
public:
    using Graph = std::vector<std::vector<unsigned>>;
    BackendEdge(const Graph& gf, const Graph& gb);

    template<unsigned idx> SkipResult skip_invalid(unsigned& c);

    template<unsigned idx> void begin ()           { if(src_ptr != nullptr) dst_ptr = &src_ptr->front(); }
    template<unsigned idx> void fixate(unsigned c) { if(dst_ptr == nullptr) src_ptr = &std::get<idx>(graphs).get()[c]; }
    template<unsigned idx> void resume(unsigned c) { if(dst_ptr == nullptr) src_ptr = nullptr; }
    template<unsigned idx> void cancel()           { if(dst_ptr != nullptr) dst_ptr = nullptr;}

private:
    std::array<std::reference_wrapper<const Graph>,2> graphs;
    const std::vector<unsigned>*                      src_ptr;
    const unsigned*                                   dst_ptr;
};

class BackendSameBlock
{
public:
    BackendSameBlock(const FunctionWrapper& w);

    template<bool idx> SkipResult skip_invalid(unsigned& c);

    template<bool idx> void begin()            { }
    template<bool idx> void fixate(unsigned c) { }
    template<bool idx> void resume(unsigned c) { }
    template<bool idx> void cancel()           { std::get<idx>(blocks) = nullptr; }

private:
    const FunctionWrapper&                          wrap;
    std::tuple<llvm::BasicBlock*,llvm::BasicBlock*> blocks;
};

template<bool lt, bool eq, bool gt>
class BackendOrdering
{
public:
    BackendOrdering();

    template<unsigned idx> SkipResult skip_invalid(Specialized::Value& c);

    template<unsigned idx> void begin ()                     { }
    template<unsigned idx> void fixate(Specialized::Value c) { amount_completed++; }
    template<unsigned idx> void resume(Specialized::Value c) { amount_completed--; }
    template<unsigned idx> void cancel()                     { }

private:
    unsigned amount_completed;
    unsigned other_value;
};

class BackendIncomingValue
{
public:
    BackendIncomingValue(const FunctionWrapper& w);

    template<unsigned idx> SkipResult skip_invalid(unsigned& c);

    template<unsigned idx> void begin ()           { }
    template<unsigned idx> void resume(unsigned c) { }
    template<unsigned idx> void fixate(unsigned c) { }
    template<unsigned idx> void cancel()           { std::get<idx>(values) = nullptr; }

private:
    const FunctionWrapper&                                        wrap;
    std::tuple<llvm::Value*,llvm::TerminatorInst*,llvm::PHINode*> values;
};

template<bool reverse,bool allow_unstrict>
class BackendDominate
{
public:
    BackendDominate(std::array<unsigned,3> size, const GraphEngine::Graph& graph_forw);

    template<unsigned idx1> SkipResult skip_invalid(unsigned idx2, unsigned& c);

    template<unsigned idx1> void begin (unsigned)             { std::get<idx1>(remaining_values)--; }
    template<unsigned idx1> void fixate(unsigned, unsigned c) { if(c != UINT_MAX-1) std::get<idx1>(used_values)++; }
    template<unsigned idx1> void resume(unsigned, unsigned c) { if(c != UINT_MAX-1) std::get<idx1>(used_values)--; }
    template<unsigned idx1> void cancel(unsigned)             { std::get<idx1>(remaining_values)++; }
    
private:
    GraphEngine                         graph_engine;
    std::array<unsigned,3>              used_values;
    std::array<unsigned,3>              remaining_values;
    std::array<std::vector<unsigned>,3> filled_values;
};

#endif
