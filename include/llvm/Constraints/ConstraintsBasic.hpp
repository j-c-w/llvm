#ifndef _CONSTRAINTS_BASIC_HPP_
#define _CONSTRAINTS_BASIC_HPP_
#include "llvm/Constraints/ConstraintLLVMSingle.hpp"
#include "llvm/Constraints/ConstraintDominate.hpp"
#include "llvm/Constraints/ConstraintOrdering.hpp"
#include "llvm/Constraints/ConstraintSingle.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/ConstraintEdge.hpp"
#include "llvm/Constraints/ConstraintRange.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"

/* In this file, many individual constraints are defined that are simply specializations of some of the very genric
   constraint definitions in the included files. */

template<typename Type1, typename Type2,
         typename = std::enable_if_t<std::is_base_of<Constraint<std::string,unsigned>,Type1>::value>,
         typename = std::enable_if_t<std::is_base_of<Constraint<std::string,unsigned>,Type2>::value>>
ConstraintAnd<std::string,unsigned> operator&&(Type1 t1, Type2 t2)
{
    return ConstraintAnd<std::string,unsigned>(t1, t2);
}

template<typename Type1, typename Type2,
         typename = std::enable_if_t<std::is_base_of<Constraint<std::string,unsigned>,Type1>::value>,
         typename = std::enable_if_t<std::is_base_of<Constraint<std::string,unsigned>,Type2>::value>>
ConstraintAnd<std::string,unsigned> operator||(Type1 t1, Type2 t2)
{
    return ConstraintOr<std::string,unsigned>(t1, t2);
}

ConstraintPrefix<std::string,unsigned> operator+(ConstraintAnd<std::string,unsigned> constraint, std::string prefix)
{
    return ConstraintPrefix<std::string,unsigned>(prefix, constraint);
}

ConstraintPrefix<std::string,unsigned> operator+(ConstraintOr<std::string,unsigned> constraint, std::string prefix)
{
    return ConstraintPrefix<std::string,unsigned>(prefix, constraint);
}

ConstraintRange<unsigned> operator*(unsigned size, ConstraintAnd<std::string,unsigned> constraint)
{
    return ConstraintRange<unsigned>(size, constraint);
}

ConstraintRange<unsigned> operator*(unsigned size, ConstraintOr<std::string,unsigned> constraint)
{
    return ConstraintRange<unsigned>(size, constraint);
}

class ConstraintUnused : public ConstraintSingle<std::string,unsigned>
{
public:
    ConstraintUnused(std::string var)
      : ConstraintSingle<std::string,unsigned>(var, {UINT_MAX-1}) { }
};

class ConstraintLLVMIntConstant : public ConstraintLLVMSingle
{
public:
    ConstraintLLVMIntConstant(FunctionWrapper& cdfg, int64_t value, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([=](llvm::Value& llvm_value)
    {
        if(llvm::ConstantInt* const_int_cast = llvm::dyn_cast<llvm::ConstantInt>(&llvm_value))
        {
            return (const_int_cast->getSExtValue() == value);
        }
        else return false;
    })) { }

    ConstraintLLVMIntConstant(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([=](llvm::Value& llvm_value)
    {
        return llvm::isa<llvm::ConstantInt>(&llvm_value);
    })) { }
};

class ConstraintConstant : public ConstraintLLVMSingle
{
public:
    ConstraintConstant(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([=](llvm::Value& value)
                                       { return llvm::isa<llvm::Constant>(value); })) { }
};

class ConstraintPreexecution : public ConstraintLLVMSingle
{
public:
    ConstraintPreexecution(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([=](llvm::Value& value)
                                       { return !llvm::isa<llvm::Instruction>(value); })) { }
};

class ConstraintArgument : public ConstraintLLVMSingle
{
public:
    ConstraintArgument(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([=](llvm::Value& value)
                                       { return llvm::isa<llvm::Argument>(value); })) { }
};

class ConstraintOpcode : public ConstraintLLVMSingle
{
public:
    ConstraintOpcode(FunctionWrapper& cdfg, unsigned opcode, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Instruction&)>([=](llvm::Instruction& instruction)
                                       { return instruction.getOpcode() == opcode; })) { }

    ConstraintOpcode(FunctionWrapper& cdfg, unsigned opcode, unsigned argn, std::string var)
     : ConstraintLLVMSingle(cdfg, var,
                            std::function<bool(llvm::Instruction&)>([=](llvm::Instruction& instruction)
                            { return instruction.getOpcode() == opcode && instruction.getNumOperands() == argn; })) { }
};

class ConstraintIntegerType : public ConstraintLLVMSingle
{
public:
    ConstraintIntegerType(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([](llvm::Value& value)
                                       { return value.getType()->isIntegerTy(); })) { }
};

class ConstraintPointerType : public ConstraintLLVMSingle
{
public:
    ConstraintPointerType(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([](llvm::Value& value)
                                       { return value.getType()->isPointerTy(); })) { }
};

class ConstraintFloatType : public ConstraintLLVMSingle
{
public:
    ConstraintFloatType(FunctionWrapper& cdfg, std::string var)
     : ConstraintLLVMSingle(cdfg, var, std::function<bool(llvm::Value&)>([](llvm::Value& value)
                                       { return value.getType()->isFloatingPointTy(); })) { }
};

typedef ConstraintOrdering<unsigned,std::string,false,true,false> ConstraintSame;
typedef ConstraintOrdering<unsigned,std::string,true,false,true>  ConstraintDistinct;
typedef ConstraintOrdering<unsigned,std::string,true,false,false> ConstraintOrder;

class ConstraintDFGEdge : public ConstraintEdge<std::string>
{
public:
    ConstraintDFGEdge(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.dfg, g.rdfg, n1, n2) { }
};

class ConstraintDFGEdge0 : public ConstraintEdge<std::string>
{
public:
    ConstraintDFGEdge0(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.odfg[0], g.rodfg[0], n1, n2) { }
};

class ConstraintDFGEdge1 : public ConstraintEdge<std::string>
{
public:
    ConstraintDFGEdge1(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.odfg[1], g.rodfg[1], n1, n2) { }
};

class ConstraintDFGEdge2 : public ConstraintEdge<std::string>
{
public:
    ConstraintDFGEdge2(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.odfg[2], g.rodfg[2], n1, n2) { }
};

class ConstraintDFGEdge3 : public ConstraintEdge<std::string>
{
public:
    ConstraintDFGEdge3(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.odfg[3], g.rodfg[3], n1, n2) { }
};


class ConstraintCFGEdge : public ConstraintEdge<std::string>
{
public:
    ConstraintCFGEdge(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.cfg, g.rcfg, n1, n2) { }
};

class ConstraintCDGEdge : public ConstraintEdge<std::string>
{
public:
    ConstraintCDGEdge(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintEdge<std::string>(g.cdg, g.rcdg, n1, n2) { }
};

class ConstraintDFGReachable : public ConstraintDominate<std::string,true>
{
public:
    ConstraintDFGReachable(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintDominate<std::string,true>(g.dfg, g.rdfg, {}, {n1}, {}, {}, {}, {n2}, true) { }
};

class ConstraintCFGReachable : public ConstraintDominate<std::string,true>
{
public:
    ConstraintCFGReachable(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintDominate<std::string,true>(g.cfg, g.rcfg, {}, {n1}, {}, {}, {}, {n2}, true) { }
};

class ConstraintCDGReachable : public ConstraintDominate<std::string,true>
{
public:
    ConstraintCDGReachable(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintDominate<std::string,true>(g.cdg, g.rcdg, {}, {n1}, {}, {}, {}, {n2}, true) { }
};

class ConstraintPDGReachable : public ConstraintDominate<std::string,true>
{
public:
    ConstraintPDGReachable(const FunctionWrapper& g, std::string n1, std::string n2)
     : ConstraintDominate<std::string,true>(g.pdg, g.rpdg, {}, {n1}, {}, {}, {}, {n2}, true) { }
};

class ConstraintCFGBlocked : public ConstraintDominate<std::string,false>
{
public:
    ConstraintCFGBlocked(const FunctionWrapper& g, std::string n1, std::string n2, std::string n3)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, {}, {n1}, {}, {n2}, {}, {n3}, false) { }
};

class ConstraintDFGBlocked : public ConstraintDominate<std::string,false>
{
public:
    ConstraintDFGBlocked(const FunctionWrapper& g, std::string n1, std::string n2, std::string n3)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, {}, {n1}, {}, {n2}, {}, {n3}, false) { }
};


class ConstraintDFGDominate : public ConstraintDominate<std::string,false>
{
public:
    ConstraintDFGDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, v1, {}, v2, true) { }

    ConstraintDFGDominate(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintDFGDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintDFGDominate(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rdfg.size(); i++)
        {
            if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintDFGPostdom : public ConstraintDominate<std::string,false>
{
public:
    ConstraintDFGPostdom(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, v1, {}, v2, true) { }

    ConstraintDFGPostdom(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintDFGPostdom(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintDFGPostdom(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_destinies(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> destinies;

        for(unsigned i = 0; i < wrap.dfg.size(); i++)
        {
            if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                destinies.push_back(i);
            }
        }

        return destinies;
    }
};

class ConstraintCFGDominate : public ConstraintDominate<std::string,false>
{
public:
    ConstraintCFGDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, v1, {}, v2, true) { }

    ConstraintCFGDominate(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintCFGDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintCFGDominate(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rcfg.size(); i++)
        {
            if(wrap.rcfg[i].empty() && !wrap.cfg[i].empty())
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintCFGPostdom : public ConstraintDominate<std::string,false>
{
public:
    ConstraintCFGPostdom(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, v1, {}, v2, true) { }

    ConstraintCFGPostdom(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintCFGPostdom(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintCFGPostdom(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_destinies(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> destinies;

        for(unsigned i = 0; i < wrap.cfg.size(); i++)
        {
            if(wrap.cfg[i].empty() && !wrap.rcfg[i].empty())
            {
                destinies.push_back(i);
            }
        }

        return destinies;
    }
};

class ConstraintPDGDominate : public ConstraintDominate<std::string,false>
{
public:
    ConstraintPDGDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, v1, {}, v2, true) { }

    ConstraintPDGDominate(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintPDGDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintPDGDominate(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rpdg.size(); i++)
        {
            if(auto call_inst = llvm::dyn_cast<llvm::CallInst>(wrap.get_value(i)))
            {
                if(auto function = llvm::dyn_cast<llvm::Function>(call_inst->getCalledValue()))
                {
                    if(function->getName() != "sqrt" &&
                       function->getName() != "log" &&
                       function->getName() != "fabs")
                    {
                        origins.push_back(i);
                    }
                }
            }
            else if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintPDGNotDominate : public ConstraintDominate<std::string,true>
{
public:
    ConstraintPDGNotDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,true>(g.pdg, g.rpdg, get_origins(g), {}, {}, v1, {}, v2, true) { }

    ConstraintPDGNotDominate(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,true>(g.pdg, g.rpdg, get_origins(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintPDGNotDominate(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,true>(g.pdg, g.rpdg, get_origins(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintPDGNotDominate(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,true>(g.pdg, g.rpdg, get_origins(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rpdg.size(); i++)
        {
            if(auto call_inst = llvm::dyn_cast<llvm::CallInst>(wrap.get_value(i)))
            {
                if(auto function = llvm::dyn_cast<llvm::Function>(call_inst->getCalledValue()))
                {
                    if(function->getName() != "sqrt" &&
                       function->getName() != "log" &&
                       function->getName() != "fabs")
                    {
                        origins.push_back(i);
                    }
                }
            }
            else if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintPDGPostdom : public ConstraintDominate<std::string,false>
{
public:
    ConstraintPDGPostdom(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, v1, {}, v2, true) { }

    ConstraintPDGPostdom(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, {v1}, {}, v2, true) { }

    ConstraintPDGPostdom(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, v1, {}, {v2}, true) { }

    ConstraintPDGPostdom(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, {v1}, {}, {v2}, true) { }

private:
    static std::vector<unsigned> get_destinies(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> destinies;

        for(unsigned i = 0; i < wrap.pdg.size(); i++)
        {
            if(/*(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||*/
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                destinies.push_back(i);
            }
        }

        return destinies;
    }
};

class ConstraintDFGDominateStrict : public ConstraintDominate<std::string,false>
{
public:
    ConstraintDFGDominateStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, v1, {}, v2, false) { }

    ConstraintDFGDominateStrict(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, {v1}, {}, v2, false) { }

    ConstraintDFGDominateStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, v1, {}, {v2}, false) { }

    ConstraintDFGDominateStrict(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.dfg, g.rdfg, get_origins(g), {}, {}, {v1}, {}, {v2}, false) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rdfg.size(); i++)
        {
            if(/*(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||*/
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintDFGPostdomStrict : public ConstraintDominate<std::string,false>
{
public:
    ConstraintDFGPostdomStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, v1, {}, v2, false) { }

    ConstraintDFGPostdomStrict(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, {v1}, {}, v2, false) { }

    ConstraintDFGPostdomStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, v1, {}, {v2}, false) { }

    ConstraintDFGPostdomStrict(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rdfg, g.dfg, get_destinies(g), {}, {}, {v1}, {}, {v2}, false) { }

private:
    static std::vector<unsigned> get_destinies(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> destinies;

        for(unsigned i = 0; i < wrap.dfg.size(); i++)
        {
            if(/*(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||*/
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                destinies.push_back(i);
            }
        }

        return destinies;
    }
};

class ConstraintCFGDominateStrict : public ConstraintDominate<std::string,false>
{
public:
    ConstraintCFGDominateStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, v1, {}, v2, false) { }

    ConstraintCFGDominateStrict(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, {v1}, {}, v2, false) { }

    ConstraintCFGDominateStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, v1, {}, {v2}, false) { }

    ConstraintCFGDominateStrict(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.cfg, g.rcfg, get_origins(g), {}, {}, {v1}, {}, {v2}, false) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rcfg.size(); i++)
        {
            if(wrap.rcfg[i].empty() && !wrap.cfg[i].empty())
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintCFGPostdomStrict : public ConstraintDominate<std::string,false>
{
public:
    ConstraintCFGPostdomStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, v1, {}, v2, false) { }

    ConstraintCFGPostdomStrict(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, {v1}, {}, v2, false) { }

    ConstraintCFGPostdomStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, v1, {}, {v2}, false) { }

    ConstraintCFGPostdomStrict(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rcfg, g.cfg, get_destinies(g), {}, {}, {v1}, {}, {v2}, false) { }

private:
    static std::vector<unsigned> get_destinies(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> destinies;

        for(unsigned i = 0; i < wrap.cfg.size(); i++)
        {
            if(wrap.cfg[i].empty() && !wrap.rcfg[i].empty())
            {
                destinies.push_back(i);
            }
        }

        return destinies;
    }
};

class ConstraintPDGDominateStrict : public ConstraintDominate<std::string,false>
{
public:
    ConstraintPDGDominateStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, v1, {}, v2, false) { }

    ConstraintPDGDominateStrict(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, {v1}, {}, v2, false) { }

    ConstraintPDGDominateStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, v1, {}, {v2}, false) { }

    ConstraintPDGDominateStrict(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.pdg, g.rpdg, get_origins(g), {}, {}, {v1}, {}, {v2}, false) { }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rpdg.size(); i++)
        {
            if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                origins.push_back(i);
            }
        }

        return origins;
    }
};

class ConstraintPDGPostdomStrict : public ConstraintDominate<std::string,false>
{
public:
    ConstraintPDGPostdomStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, v1, {}, v2, false) { }

    ConstraintPDGPostdomStrict(const FunctionWrapper& g, std::string v1, std::vector<std::string> v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, {v1}, {}, v2, false) { }

    ConstraintPDGPostdomStrict(const FunctionWrapper& g, std::vector<std::string> v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, v1, {}, {v2}, false) { }

    ConstraintPDGPostdomStrict(const FunctionWrapper& g, std::string v1, std::string v2)
     : ConstraintDominate<std::string,false>(g.rpdg, g.pdg, get_destinies(g), {}, {}, {v1}, {}, {v2}, false) { }

private:
    static std::vector<unsigned> get_destinies(const FunctionWrapper& wrap)
    {
        std::vector<unsigned> destinies;

        for(unsigned i = 0; i < wrap.pdg.size(); i++)
        {
            if(//(wrap.rpdg[i].empty() && !wrap.pdg[i].empty()) ||
               wrap.opcodes[i] == llvm::Instruction::Load ||
               llvm::isa<llvm::Argument>(wrap.get_value(i)))
            {
                destinies.push_back(i);
            }
        }

        return destinies;
    }
};


#endif
