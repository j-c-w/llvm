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
                    (data_sinks      && wrap.opcodes[i] == llvm::Instruction::Store)  ||
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

using BackendAnd     = ScalarBackend<BackendAnd_,1>;
using BackendOr      = VectorBackend<BackendOr_,1>;
using BackendCollect = VectorBackend<BackendCollect_,2>;
using BackendSingle  = ScalarBackend<BackendSingle_,1>;

using BackendEdge          = ScalarBackend<BackendEdge_,2>;
using BackendSameBlock     = ScalarBackend<BackendSameBlock_,2>;
using BackendIncomingValue = ScalarBackend<BackendIncomingValue_,3>;

template<bool reverse,bool allow_unstrict>
using BackendDominate = VectorBackend<BackendDominate_<reverse,allow_unstrict>,3>;

using BackendUnused = ScalarBackend<BackendConstantValue_<UINT_MAX-1>,1>;

using BackendIntegerType = ScalarBackend<BackendLLVMType_<&llvm::Type::isIntegerTy>,1>;
using BackendFloatType   = ScalarBackend<BackendLLVMType_<&llvm::Type::isFloatTy>,1>;
using BackendPointerType = ScalarBackend<BackendLLVMType_<&llvm::Type::isPointerTy>,1>;

using BackendNotNumericConstant = ScalarBackend<BackendNotNumericConstant_,1>;
using BackendConstant           = ScalarBackend<BackendConstant_,1>;
using BackendPreexecution       = ScalarBackend<BackendPreexecution_,1>; 
using BackendArgument           = ScalarBackend<BackendArgument_,1>;
using BackendInstruction        = ScalarBackend<BackendInstruction_,1>;

using BackendStoreInst  = ScalarBackend<BackendOpcode_<llvm::Instruction::Store>,1>;
using BackendLoadInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::Load>,1>;
using BackendReturnInst = ScalarBackend<BackendOpcode_<llvm::Instruction::Ret>,1>;
using BackendBranchInst = ScalarBackend<BackendOpcode_<llvm::Instruction::Br>,1>;
using BackendAddInst    = ScalarBackend<BackendOpcode_<llvm::Instruction::Add>,1>;
using BackendSubInst    = ScalarBackend<BackendOpcode_<llvm::Instruction::Sub>,1>;
using BackendMulInst    = ScalarBackend<BackendOpcode_<llvm::Instruction::Mul>,1>;
using BackendFAddInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::FAdd>,1>;
using BackendFSubInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::FSub>,1>;
using BackendFMulInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::FMul>,1>;
using BackendFDivInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::FDiv>,1>;
using BackendSelectInst = ScalarBackend<BackendOpcode_<llvm::Instruction::Select>,1>;
using BackendSExtInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::SExt>,1>;
using BackendZExtInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::ZExt>,1>;
using BackendGEPInst    = ScalarBackend<BackendOpcode_<llvm::Instruction::GetElementPtr>,1>;
using BackendICmpInst   = ScalarBackend<BackendOpcode_<llvm::Instruction::ICmp>,1>;

using BackendSame     = ScalarBackend<BackendOrderWrap_<false,true,false>,2>;
using BackendDistinct = ScalarBackend<BackendOrderWrap_<true,false,true>,2>;
using BackendOrder    = ScalarBackend<BackendOrderWrap_<true,false,false>,2>;

using BackendDFGEdge  = ScalarBackend<BackendLLVMEdge_<&FunctionWrapper::dfg, &FunctionWrapper::rdfg>,2>;
using BackendCFGEdge  = ScalarBackend<BackendLLVMEdge_<&FunctionWrapper::cfg, &FunctionWrapper::rcfg>,2>;
using BackendCDGEdge  = ScalarBackend<BackendLLVMEdge_<&FunctionWrapper::cdg, &FunctionWrapper::rcdg>,2>;
using BackendPDGEdge  = ScalarBackend<BackendLLVMEdge_<&FunctionWrapper::pdg, &FunctionWrapper::rpdg>,2>;

using BackendFirstArgument  = ScalarBackend<BackendLLVMArgument_<0, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;
using BackendSecondArgument = ScalarBackend<BackendLLVMArgument_<1, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;
using BackendThirdArgument  = ScalarBackend<BackendLLVMArgument_<2, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;
using BackendFourthArgument = ScalarBackend<BackendLLVMArgument_<3, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;

using BackendSameBlockWrap      = ScalarBackend<BackendSameBlock_,2>;
using BackendIncomingValueWrap  = ScalarBackend<BackendIncomingValue_,3>;

using BackendDFGDominate       = VectorBackend<BackendLLVMDominate_<false, true,  0, &FunctionWrapper:: dfg>,3>;
using BackendDFGPostdom        = VectorBackend<BackendLLVMDominate_<false, true,  1, &FunctionWrapper::rdfg>,3>;
using BackendDFGDominateStrict = VectorBackend<BackendLLVMDominate_<false, false, 0, &FunctionWrapper:: dfg>,3>;
using BackendDFGPostdomStrict  = VectorBackend<BackendLLVMDominate_<false, false, 1, &FunctionWrapper::rdfg>,3>;

using BackendCFGDominate       = VectorBackend<BackendLLVMDominate_<false, true,  2, &FunctionWrapper:: cfg>,3>;
using BackendCFGPostdom        = VectorBackend<BackendLLVMDominate_<false, true,  3, &FunctionWrapper::rcfg>,3>;
using BackendCFGDominateStrict = VectorBackend<BackendLLVMDominate_<false, false, 2, &FunctionWrapper:: cfg>,3>;
using BackendCFGPostdomStrict  = VectorBackend<BackendLLVMDominate_<false, false, 3, &FunctionWrapper::rcfg>,3>;

using BackendPDGDominate       = VectorBackend<BackendLLVMDominate_<false, true,  4, &FunctionWrapper:: pdg>,3>;
using BackendPDGPostdom        = VectorBackend<BackendLLVMDominate_<false, true,  5, &FunctionWrapper::rpdg>,3>;
using BackendPDGDominateStrict = VectorBackend<BackendLLVMDominate_<false, false, 4, &FunctionWrapper:: pdg>,3>;
using BackendPDGPostdomStrict  = VectorBackend<BackendLLVMDominate_<false, false, 5, &FunctionWrapper::rpdg>,3>;

using BackendDFGNotDominate       = VectorBackend<BackendLLVMDominate_<true, true,  0, &FunctionWrapper:: dfg>,3>;
using BackendDFGNotPostdom        = VectorBackend<BackendLLVMDominate_<true, true,  1, &FunctionWrapper::rdfg>,3>;
using BackendDFGNotDominateStrict = VectorBackend<BackendLLVMDominate_<true, false, 0, &FunctionWrapper:: dfg>,3>;
using BackendDFGNotPostdomStrict  = VectorBackend<BackendLLVMDominate_<true, false, 1, &FunctionWrapper::rdfg>,3>;

using BackendCFGNotDominate       = VectorBackend<BackendLLVMDominate_<true, true,  2, &FunctionWrapper:: cfg>,3>;
using BackendCFGNotPostdom        = VectorBackend<BackendLLVMDominate_<true, true,  3, &FunctionWrapper::rcfg>,3>;
using BackendCFGNotDominateStrict = VectorBackend<BackendLLVMDominate_<true, false, 2, &FunctionWrapper:: cfg>,3>;
using BackendCFGNotPostdomStrict  = VectorBackend<BackendLLVMDominate_<true, false, 3, &FunctionWrapper::rcfg>,3>;

using BackendPDGNotDominate       = VectorBackend<BackendLLVMDominate_<true, true,  4, &FunctionWrapper:: pdg>,3>;
using BackendPDGNotPostdom        = VectorBackend<BackendLLVMDominate_<true, true,  5, &FunctionWrapper::rpdg>,3>;
using BackendPDGNotDominateStrict = VectorBackend<BackendLLVMDominate_<true, false, 4, &FunctionWrapper:: pdg>,3>;
using BackendPDGNotPostdomStrict  = VectorBackend<BackendLLVMDominate_<true, false, 5, &FunctionWrapper::rpdg>,3>;

using BackendCFGBlocked = VectorBackend<BackendLLVMDominate_<false, false, UINT_MAX, &FunctionWrapper::cfg>,3>;
using BackendDFGBlocked = VectorBackend<BackendLLVMDominate_<false, false, UINT_MAX, &FunctionWrapper::dfg>,3>;
using BackendPDGBlocked = VectorBackend<BackendLLVMDominate_<false, false, UINT_MAX, &FunctionWrapper::pdg>,3>;

#endif
