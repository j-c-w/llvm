#ifndef _BACKEND_SPECIALIZATIONS_HPP_
#define _BACKEND_SPECIALIZATIONS_HPP_
#include "Backends.hpp"

template<unsigned ... constants>
class BackendConstantValue_ : public BackendSingle_
{
public:
    BackendConstantValue_(FunctionWrapper& wrap)
      : BackendSingle_(std::vector<unsigned>{constants...}) { }
};

template<typename Type>
class BackendLLVMSingle_ : public BackendSingle_
{
public:
    BackendLLVMSingle_(FunctionWrapper& wrap, std::function<bool(Type&)> pred)
      : BackendSingle_(compute_hits(wrap, pred)) { }

private:
    static std::vector<unsigned> compute_hits(FunctionWrapper& wrap, std::function<bool(Type&)> pred)
    {
        std::vector<unsigned> hits;

        for(unsigned i = 0; i < wrap.get_size(); i++)
        {
            Type* value = llvm::dyn_cast<Type>(wrap.get_value(i));

            if(value && (pred == nullptr || pred(*value)))
            {
                hits.push_back(i);
            }
        }

        return hits;
    }
};

class BackendNotNumericConstant_ : public BackendLLVMSingle_<llvm::Value>
{
public:
    BackendNotNumericConstant_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Value>(wrap, [](llvm::Value& value)
                                              { return !llvm::isa<llvm::ConstantInt>(value) &&
                                                       !llvm::isa<llvm::ConstantFP>(value)  &&
                                                       !llvm::isa<llvm::Function>(value)  &&
                                                       !llvm::isa<llvm::ConstantPointerNull>(value); }) { }
};

class BackendConstant_ : public BackendLLVMSingle_<llvm::Constant>
{
public:
    BackendConstant_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Constant>(wrap, nullptr) { }
};


class BackendPreexecution_ : public BackendLLVMSingle_<llvm::Value>
{
public:
    BackendPreexecution_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Value>(wrap, [](llvm::Value& value)
                                             { return !llvm::isa<llvm::Instruction>(value); }) { }
};

class BackendArgument_ : public BackendLLVMSingle_<llvm::Argument>
{
public:
    BackendArgument_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Argument>(wrap, nullptr) { }
};

class BackendInstruction_ : public BackendLLVMSingle_<llvm::Instruction>
{
public:
    BackendInstruction_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Instruction>(wrap, nullptr) { }
};

class BackendFloatZero_ : public BackendLLVMSingle_<llvm::ConstantFP>
{
public:
    BackendFloatZero_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::ConstantFP>(wrap, [](llvm::ConstantFP& value) { return value.isZero(); }) { }
};

template<unsigned op>
class BackendOpcode_ : public BackendLLVMSingle_<llvm::Instruction>
{
public:
    BackendOpcode_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Instruction>(wrap, [](llvm::Instruction& inst) { return inst.getOpcode() == op; }) { }
};

template<bool(llvm::Type::*predicate)() const>
class BackendLLVMType_: public BackendLLVMSingle_<llvm::Value>
{
public:
    BackendLLVMType_(FunctionWrapper& wrap)
      : BackendLLVMSingle_<llvm::Value>(wrap, [](llvm::Value& value)
                                              { return (value.getType()->*predicate)(); }) { }
};

template<std::vector<std::vector<unsigned>> FunctionWrapper::* forw_graph,
         std::vector<std::vector<unsigned>> FunctionWrapper::* back_graph>
class BackendLLVMEdge_ : public BackendEdge_
{
public:
    BackendLLVMEdge_(FunctionWrapper& wrap)
      : BackendEdge_(wrap.*forw_graph, wrap.*back_graph) { }
};

template<unsigned i, unsigned N,
         std::vector<std::vector<unsigned>> (FunctionWrapper::* forw_graph)[N],
         std::vector<std::vector<unsigned>> (FunctionWrapper::* back_graph)[N]>
class BackendLLVMArgument_ : public BackendEdge_
{
public:
    BackendLLVMArgument_(FunctionWrapper& wrap)
      : BackendEdge_((wrap.*forw_graph)[i], (wrap.*back_graph)[i]) { }
};

template<bool lt, bool eq, bool gt>
class BackendOrderWrap_ : public BackendOrdering_<lt,eq,gt>
{
public:
    BackendOrderWrap_(FunctionWrapper&)
      : BackendOrdering_<lt,eq,gt>() { }
};

template<bool inverted, bool unstrict, unsigned origin_calc,
         std::vector<std::vector<unsigned>> FunctionWrapper::* forw_graph>
class BackendLLVMDominate_ : public BackendDominate_<inverted,unstrict>
{
public:
    BackendLLVMDominate_(std::array<unsigned,3> sizes, const FunctionWrapper& wrap)
      : BackendDominate_<inverted,unstrict>({{(unsigned)std::get<0>(sizes) + (unsigned)get_origins(wrap).size(),
                                              (unsigned)std::get<1>(sizes), (unsigned)std::get<2>(sizes)}},
                                            wrap.*forw_graph),
        number_origins(get_origins(wrap).size())
    {
        std::vector<unsigned> origins = get_origins(wrap);

        for(unsigned i = 0; i < number_origins; i++)
        {
            BackendDominate_<inverted,unstrict>::template begin<0>       (i);
            BackendDominate_<inverted,unstrict>::template skip_invalid<0>(i, origins[i]);
            BackendDominate_<inverted,unstrict>::template fixate<0>      (i, origins[i]);
        }
    }

    template<unsigned idx1> SkipResult skip_invalid(unsigned idx2, unsigned &c)
        { return BackendDominate_<inverted,unstrict>::template skip_invalid<idx1>(idx2+(idx1?0:number_origins), c); }

    template<unsigned idx1> void begin(unsigned idx2)
        { BackendDominate_<inverted,unstrict>::template begin<idx1>(idx2+(idx1?0:number_origins)); }

    template<unsigned idx1> void fixate(unsigned idx2, unsigned c)
        { BackendDominate_<inverted,unstrict>::template fixate<idx1>(idx2+(idx1?0:number_origins), c); }

    template<unsigned idx1> void resume(unsigned idx2, unsigned c)
        { BackendDominate_<inverted,unstrict>::template resume<idx1>(idx2+(idx1?0:number_origins), c); }

    template<unsigned idx1> void cancel(unsigned idx2)
        { BackendDominate_<inverted,unstrict>::template cancel<idx1>(idx2+(idx1?0:number_origins)); }

    ~BackendLLVMDominate_()
    {
        for(unsigned i = 0; i < number_origins; i++)
        {
            BackendDominate_<inverted,unstrict>::template cancel<0>(i);
        }
    }

private:
    static std::vector<unsigned> get_origins(const FunctionWrapper& wrap)
    {
        bool data_origins    = origin_calc==0 || origin_calc==4;
        bool data_sinks      = origin_calc==1 || origin_calc==5;
        bool control_origins = origin_calc==2 || origin_calc==4;
        bool control_sinks   = origin_calc==3 || origin_calc==5;

        std::vector<unsigned> origins;

        for(unsigned i = 0; i < wrap.rdfg.size(); i++)
        {
            if(data_origins && wrap.opcodes[i] == llvm::Instruction::Call)
            {
                if(auto call_inst = llvm::dyn_cast<llvm::CallInst>(wrap.get_value(i)))
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
            else if((data_origins    && wrap.opcodes[i] == llvm::Instruction::Load)   ||
                    (data_origins    && llvm::isa<llvm::Argument>(wrap.get_value(i))) ||
                    (data_sinks      && wrap.opcodes[i])  ||
                    (data_sinks      && wrap.opcodes[i] == llvm::Instruction::Ret)    ||
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
