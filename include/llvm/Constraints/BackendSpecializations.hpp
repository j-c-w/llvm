#ifndef _BACKEND_SPECIALIZATIONS_HPP_
#define _BACKEND_SPECIALIZATIONS_HPP_
#include "llvm/Constraints/BackendClasses.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include <vector>

template<unsigned ... constants>
class BackendConstantValue : public BackendSingle
{
public:
    BackendConstantValue(const FunctionWrap& wrap)
      : BackendSingle(std::vector<unsigned>{constants...}) { }
};

template<typename Type>
class BackendLLVMSingle : public BackendSingle
{
public:
    BackendLLVMSingle(const FunctionWrap& wrap, std::function<bool(Type&)> pred)
      : BackendSingle(compute_hits(wrap, pred)) { }

private:
    static std::vector<unsigned> compute_hits(const FunctionWrap& wrap, std::function<bool(Type&)> pred)
    {
        std::vector<unsigned> hits;

        for(unsigned i = 0; i < wrap.size(); i++)
        {
            if(auto value = llvm::dyn_cast<Type>(wrap[i]))
            {
                if(pred == nullptr || pred(*value))
                {
                    hits.push_back(i);
                }
            }
        }

        return hits;
    }
};

class BackendNotNumericConstant : public BackendLLVMSingle<llvm::Value>
{
public:
    BackendNotNumericConstant(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Value>(wrap, [](llvm::Value& value)
                                              { return !llvm::isa<llvm::ConstantInt>(value) &&
                                                       !llvm::isa<llvm::ConstantFP>(value)  &&
                                                       !llvm::isa<llvm::Function>(value)  &&
                                                       !llvm::isa<llvm::ConstantPointerNull>(value); }) { }
};

class BackendConstant : public BackendLLVMSingle<llvm::Constant>
{
public:
    BackendConstant(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Constant>(wrap, nullptr) { }
};


class BackendPreexecution : public BackendLLVMSingle<llvm::Value>
{
public:
    BackendPreexecution(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Value>(wrap, [](llvm::Value& value)
                                             { return !llvm::isa<llvm::Instruction>(value); }) { }
};

class BackendArgument : public BackendLLVMSingle<llvm::Argument>
{
public:
    BackendArgument(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Argument>(wrap, nullptr) { }
};

class BackendInstruction : public BackendLLVMSingle<llvm::Instruction>
{
public:
    BackendInstruction(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Instruction>(wrap, nullptr) { }
};

class BackendFloatZero : public BackendLLVMSingle<llvm::ConstantFP>
{
public:
    BackendFloatZero(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::ConstantFP>(wrap, [](llvm::ConstantFP& value) { return value.isZero(); }) { }
};

template<unsigned op>
class BackendOpcode : public BackendLLVMSingle<llvm::Instruction>
{
public:
    BackendOpcode(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Instruction>(wrap, [](llvm::Instruction& inst) { return inst.getOpcode() == op; }) { }
};

template<bool(llvm::Type::*predicate)() const>
class BackendLLVMType: public BackendLLVMSingle<llvm::Value>
{
public:
    BackendLLVMType(const FunctionWrap& wrap)
      : BackendLLVMSingle<llvm::Value>(wrap, [](llvm::Value& value)
                                              { return (value.getType()->*predicate)(); }) { }
};

template<std::vector<std::vector<unsigned>> FunctionWrap::* forw_graph,
         std::vector<std::vector<unsigned>> FunctionWrap::* back_graph>
class BackendLLVMEdge : public BackendEdge
{
public:
    BackendLLVMEdge(const FunctionWrap& wrap)
      : BackendEdge(wrap.*forw_graph, wrap.*back_graph) { }
};

template<unsigned i>
class BackendLLVMOperand : public BackendEdge
{
public:
    BackendLLVMOperand(const FunctionWrap& wrap)
      : BackendEdge(wrap.odfg[i], wrap.rodfg[i]) { }
};

template<bool lt, bool eq, bool gt>
class BackendOrderWrap : public BackendOrdering<lt,eq,gt>
{
public:
    BackendOrderWrap(const FunctionWrap&)
      : BackendOrdering<lt,eq,gt>() { }
};

template<bool inverted, bool unstrict, unsigned origin_calc,
         std::vector<std::vector<unsigned>> FunctionWrap::* forw_graph>
class BackendLLVMDominate : public BackendDominate<inverted,unstrict>
{
public:
    BackendLLVMDominate(std::array<unsigned,3> sizes, const FunctionWrap& wrap)
      : BackendDominate<inverted,unstrict>({{(unsigned)std::get<0>(sizes) + (unsigned)get_origins(wrap).size(),
                                             (unsigned)std::get<1>(sizes), (unsigned)std::get<2>(sizes)}},
                                            wrap.*forw_graph),
        number_origins(get_origins(wrap).size())
    {
        std::vector<unsigned> origins = get_origins(wrap);

        for(unsigned i = 0; i < number_origins; i++)
        {
            BackendDominate<inverted,unstrict>::template begin<0>       (i);
            BackendDominate<inverted,unstrict>::template skip_invalid<0>(i, origins[i]);
            BackendDominate<inverted,unstrict>::template fixate<0>      (i, origins[i]);
        }
    }

    template<unsigned idx1> SkipResult skip_invalid(unsigned idx2, unsigned &c)
        { return BackendDominate<inverted,unstrict>::template skip_invalid<idx1>(idx2+(idx1?0:number_origins), c); }

    template<unsigned idx1> void begin(unsigned idx2)
        { BackendDominate<inverted,unstrict>::template begin<idx1>(idx2+(idx1?0:number_origins)); }

    template<unsigned idx1> void fixate(unsigned idx2, unsigned c)
        { BackendDominate<inverted,unstrict>::template fixate<idx1>(idx2+(idx1?0:number_origins), c); }

    template<unsigned idx1> void resume(unsigned idx2)
        { BackendDominate<inverted,unstrict>::template resume<idx1>(idx2+(idx1?0:number_origins)); }

private:
    static std::vector<unsigned> get_origins(const FunctionWrap& wrap)
    {
        bool data_origins    = origin_calc==0 || origin_calc==4;
        bool data_sinks      = origin_calc==1 || origin_calc==5;
        bool control_origins = origin_calc==2 || origin_calc==4;
        bool control_sinks   = origin_calc==3 || origin_calc==5;

        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.size(); i++)
        {
            if((data_sinks || data_origins) && llvm::isa<llvm::CallInst>(wrap[i]))
            {
                if(auto call_inst = llvm::dyn_cast<llvm::CallInst>(wrap[i]))
                {
                    if(call_inst->getCalledFunction() == nullptr)
                    {
                        //WOW, wait a second, this is overly optimistic!
                        continue;
                    }
                
                    auto attributes = call_inst->getCalledFunction()->getAttributes();

                    if(!(attributes.hasAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::ReadOnly) ||
                         attributes.hasAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::ReadNone)) ||
                       !attributes.hasAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::NoUnwind))
                    {
                        origins.push_back(i);
                    }
                }
            }
            else if((data_origins    && llvm::isa<llvm::LoadInst>(wrap[i]))   ||
                    (data_origins    && llvm::isa<llvm::Argument>(wrap[i])) ||
                    (data_sinks      && llvm::isa<llvm::StoreInst>(wrap[i]))  ||
                    (data_sinks      && llvm::isa<llvm::ReturnInst>(wrap[i]))    ||
                    (control_origins && wrap.rcfg[i].empty() && !wrap.cfg[i].empty()) ||
                    (control_sinks   && wrap.cfg[i].empty()  && !wrap.rcfg[i].empty()))
            {
                origins.push_back(i);
            }
        }

        return origins;
    }

private:
    unsigned number_origins;
};

#endif
