#include "llvm/Constraints/Transforms.hpp"
#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include <unordered_map>
#include <sstream>

class Expression
{
public:
    Expression(std::string h = "", std::vector<std::pair<Expression,std::string>> t = {},
               unsigned p1 = 0, unsigned p2 = 0)
      : head(h), tail(t), precedence1(p1), precedence2(p2) { }

    bool operator==(const Expression& expr) const
    {
        return head == expr.head && tail == expr.tail &&
               precedence1 == expr.precedence1 && precedence2 == expr.precedence2;
    }

    static Expression Atomic(std::string s) { return Expression(s); }

    static Expression Cast(Expression e1, std::string t) {return Expression("("+t+")", {{e1, ""}}, 3); }

    static Expression Neg (Expression e1) { return Expression("-", {{e1, ""}}, 3); }
    static Expression LNot(Expression e1) { return Expression("!", {{e1, ""}}, 3); }
    static Expression BNot(Expression e1) { return Expression("~", {{e1, ""}}, 3); }

    static Expression Add(Expression e1, Expression e2) { return Expression("", {{e1, "+"}, {e2, ""}}, 6, 5); }
    static Expression Sub(Expression e1, Expression e2) { return Expression("", {{e1, "-"}, {e2, ""}}, 6, 5); }
    static Expression Mul(Expression e1, Expression e2) { return Expression("", {{e1, "*"}, {e2, ""}}, 5, 4); }
    static Expression Div(Expression e1, Expression e2) { return Expression("", {{e1, "/"}, {e2, ""}}, 5, 4); }

    static Expression And(Expression e1, Expression e2) { return Expression("", {{e1, "&&"}, {e2, ""}}, 13, 12); }
    static Expression Or (Expression e1, Expression e2) { return Expression("", {{e1, "||"}, {e2, ""}}, 14, 13); }

    static Expression Reference(Expression e1)
    {
        if(e1.head == "*" && e1.tail.size() == 1)
        {
            return e1.tail[0].first;
        }
        else if(e1.head == "" && e1.tail.size() == 2 &&
                e1.tail[0].second            == "["  &&
                e1.tail[1].first.head        == "0"  &&
                e1.tail[1].first.tail.size() ==  0   &&
                e1.tail[1].second            == "]")
        {
            return e1.tail[0].first;
        }
        else
        {
            return Expression("&", {{e1, ""}}, 3);
        }
    }

    static Expression Deref(Expression e1)
    {
        if(e1.head == "&" && e1.tail.size() == 1) return e1.tail[0].first;
        else                                      return Expression("*", {{e1, ""}}, 3);
    }

    static Expression Index(Expression e1, Expression e2)
    {
        return Expression("", {{e1, "["}, {e2, "]"}}, 2, UINT_MAX);
    }

    static Expression Member(Expression e1, std::string m)
    {
        if(e1.head == "&" && e1.tail.size() == 1) return Expression("", {{e1.tail[0].first, "."+m}}, 2);
        else                                      return Expression("", {{e1,              "->"+m}}, 2);
    }

    static Expression Less     (Expression e1, Expression e2) { return Expression("", {{e1,  "<"}, {e2, ""}}, 8, 7); }
    static Expression Lesseq   (Expression e1, Expression e2) { return Expression("", {{e1, "<="}, {e2, ""}}, 8, 7); }
    static Expression Greatereq(Expression e1, Expression e2) { return Expression("", {{e1, ">="}, {e2, ""}}, 8, 7); }
    static Expression Greater  (Expression e1, Expression e2) { return Expression("", {{e1,  ">"}, {e2, ""}}, 8, 7); }

    static Expression Select(Expression e1, Expression e2, Expression e3)
    {
        return Expression("", {{e1, "?"}, {e2, ":"}, {e3, ""}}, 14, 13);
    }

    static Expression Assign(Expression e1, Expression e2)
    {
        if(e2.head == "" && e2.tail.size() == 2 &&
           (e2.tail[0].second == "+" || e2.tail[0].second == "*") &&
           e2.tail[1].second == "")
        {
            if(e2.tail[0].first == e1)
            {
                return Expression("", {{e1, " "+e2.tail[0].second+"= "}, {e2.tail[1].first, ""}}, 15, 15);
            }
            else if(e2.tail[1].first == e1)
            {
                return Expression("", {{e1, " "+e2.tail[0].second+"= "}, {e2.tail[0].first, ""}}, 15, 15);
            }
        }
        else if(e2.head == "" && e2.tail.size() == 2 &&
                (e2.tail[0].second == "*" || e2.tail[0].second == "/") &&
                e2.tail[1].second == "")
        {
            if(e2.tail[0].first == e1)
            {
                return Expression("", {{e1, " "+e2.tail[0].second+"= "}, {e2.tail[1].first, ""}}, 15, 15);
            }
        }

        return Expression("", {{e1, " = "}, {e2, ""}}, 15, 15);
    }

    operator std::string() const
    {
        std::string result = head;

        for(const auto& elem : tail)
        {
            if((elem.first.precedence1 > precedence1 && &elem == &tail.front()) ||
               (elem.first.precedence1 > precedence2 && &elem != &tail.front()))
            {
                result += "("+(std::string)elem.first+")"+elem.second;
            }
            else
            {
                result += (std::string)elem.first+elem.second;
            }
        }

        return result;
    }

private:
    std::string                                    head;
    std::vector<std::pair<Expression,std::string>> tail;
    unsigned                                       precedence1;
    unsigned                                       precedence2;
};

std::string print_c_type(llvm::Type* type)
{
    if(type)
    {
        if     (type->isVoidTy())      return "void";
        else if(type->isIntegerTy(1))  return "bool";
        else if(type->isIntegerTy(8))  return "char";
        else if(type->isIntegerTy(16)) return "short";
        else if(type->isIntegerTy(32)) return "int";
        else if(type->isIntegerTy(64)) return "long";
        else if(type->isFloatTy())     return "float";
        else if(type->isDoubleTy())    return "double";
        else if(auto pointer_ty_cast = llvm::dyn_cast<llvm::PointerType>(type))
        {
            return print_c_type(pointer_ty_cast->getElementType())+"*";
        }
        else if(auto struct_ty_cast = llvm::dyn_cast<llvm::StructType>(type))
        {
            std::stringstream sstr;

            sstr<<"struct { ";

            unsigned index = 0;
            for(auto element : struct_ty_cast->elements())
            {
                sstr<<print_c_type(element)<<" _"<<(index++)<<"; ";
            }

            sstr<<"}";

            return sstr.str();
        }
        else
        {
            return "<type>";
        }
    }
    else return "";
}

Expression get_c_expr(llvm::Value* value, const std::unordered_map<llvm::Value*,Expression>& expressions)
{
    if(value == nullptr)
    {
        return Expression();
    }
    if(auto* function_cast = llvm::dyn_cast<llvm::GlobalValue>(value))
    {
        return Expression::Reference(Expression::Atomic(function_cast->getName()));
    }
    if(auto* int_cast = llvm::dyn_cast<llvm::ConstantInt>(value))
    {
        return int_cast->getValue().toString(10, true);
    }
    else if(auto* float_cast = llvm::dyn_cast<llvm::ConstantFP>(value))
    {
        std::stringstream sstr;
        sstr<<float_cast->getValueAPF().convertToDouble();
        return sstr.str();
    }
    else
    {
        auto find_it = expressions.find(value);

        if(find_it != expressions.end())
        {
            return find_it->second;
        }
        else
        {
            std::stringstream str_str;
            llvm::raw_os_ostream out_stream(str_str);
            out_stream<<*value;
            out_stream.flush();
            str_str.flush();
            return Expression::Atomic("/*"+str_str.str()+"*/");
        }
    }
}

std::string print_c_operator(llvm::Function& function)
{
    std::unique_ptr<llvm::FunctionPass>(llvm::createRemovePHIPass())->runOnFunction(function);

    std::string output;

    std::unordered_map<llvm::Value*,Expression> expressions;
    for(auto& argument : function.args())
    {
        std::string name = argument.getName();

        if(name.empty())
        {
            std::stringstream sstr;
            sstr<<"in"<<expressions.size();
            name = sstr.str();
        }

        output += (expressions.empty()?"":", ")+print_c_type(argument.getType())+" "+name;
        expressions[&argument] = Expression::Atomic(name);
    }

    std::unordered_map<llvm::BasicBlock*,std::string> block_labels;
    for(auto& block: function.getBasicBlockList())
    {
        std::stringstream sstr;
        sstr<<"label"<<(block_labels.size());
        block_labels[&block] = sstr.str();
    }

    output += ")\n{\n";

    unsigned counter = 0;

    for(auto& block: function.getBasicBlockList())
    {
        output += block_labels[&block]+":\n";

        for(auto& inst : block.getInstList())
        {
            if(auto* alloca_cast = llvm::dyn_cast<llvm::AllocaInst>(&inst))
            {
                std::stringstream sstr;
                sstr<<"t"<<(counter++);

                output += "  "+print_c_type(alloca_cast->getAllocatedType())+" "+sstr.str()+";\n";

                expressions[alloca_cast] = Expression::Reference(Expression::Atomic(sstr.str()));
            }
            else if(auto* store_cast = llvm::dyn_cast<llvm::StoreInst>(&inst))
            {
                Expression in_expr1 = get_c_expr(store_cast->getOperand(0), expressions);
                Expression in_expr2 = get_c_expr(store_cast->getOperand(1), expressions);

                output += "  "+(std::string)Expression::Assign(Expression::Deref(in_expr2), in_expr1)+";\n";
            }
            else if(auto branch_cast = llvm::dyn_cast<llvm::BranchInst>(&inst))
            {
                std::string cond = branch_cast->isConditional()?
                                   "if("+(std::string)get_c_expr(branch_cast->getCondition(), expressions)+") ":"";

                output += "  "+cond+"goto "+block_labels[branch_cast->getSuccessor(0)]+";"
                              +(cond.empty()?"":" else goto "+block_labels[branch_cast->getSuccessor(1)]+";")+"\n";
            }
            else if(auto ret_cast = llvm::dyn_cast<llvm::ReturnInst>(&inst))
            {
                output += "  return "+(std::string)get_c_expr(ret_cast->getReturnValue(), expressions)+";\n";
            }
            else
            {
                Expression new_expr;

                if(auto* gep_cast = llvm::dyn_cast<llvm::GetElementPtrInst>(&inst))
                {
                    new_expr = get_c_expr(gep_cast->getOperand(0), expressions);

                    for(unsigned j = 1; j < gep_cast->getNumOperands(); j++)
                    {
                        Expression index = get_c_expr(gep_cast->getOperand(j), expressions);

                        if(auto* constint_cast = llvm::dyn_cast<llvm::ConstantInt>(gep_cast->getOperand(j)))
                        {
                            if(constint_cast->getType()->isIntegerTy(32))
                            {
                                new_expr = Expression::Reference(Expression::Member(new_expr, "_"+(std::string)index));
                            }
                            else if(constint_cast->getType()->isIntegerTy(64))
                            {
                                new_expr = Expression::Reference(Expression::Index(new_expr, index));
                            }
                        }
                        else
                        {
                            Expression index = get_c_expr(gep_cast->getOperand(j), expressions);

                            new_expr = Expression::Reference(Expression::Index(new_expr, index));
                        }
                    }
                }
                else
                {
                    if(auto* load_cast = llvm::dyn_cast<llvm::LoadInst>(&inst))
                    {
                        new_expr = Expression::Deref(get_c_expr(load_cast->getPointerOperand(), expressions));
                    }
                    else if(auto* cast_cast = llvm::dyn_cast<llvm::ZExtInst>(&inst))
                    {
                        Expression temp_expr = Expression::Cast(get_c_expr(cast_cast->getOperand(0), expressions),
                                                                "unsigned "+print_c_type(cast_cast->getSrcTy()));

                        new_expr = Expression::Cast(temp_expr, "unsigned "+print_c_type(cast_cast->getDestTy()));
                    }
                    else if(auto* cast_cast = llvm::dyn_cast<llvm::UIToFPInst>(&inst))
                    {
                        Expression temp_expr = Expression::Cast(get_c_expr(cast_cast->getOperand(0), expressions),
                                                                "unsigned "+print_c_type(cast_cast->getSrcTy()));

                        new_expr = Expression::Cast(temp_expr, print_c_type(cast_cast->getDestTy()));
                    }
                    else if(auto* cast_cast = llvm::dyn_cast<llvm::FPToUIInst>(&inst))
                    {
                        Expression temp_expr = Expression::Cast(get_c_expr(cast_cast->getOperand(0), expressions),
                                                                "unsigned "+print_c_type(cast_cast->getSrcTy()));
                    }
                    else if(auto* cast_cast = llvm::dyn_cast<llvm::CastInst>(&inst)) // All other casts
                    {
                        new_expr = Expression::Cast(get_c_expr(cast_cast->getOperand(0), expressions),
                                                    print_c_type(cast_cast->getType()));
                    }
                    else if(auto* binary_cast = llvm::dyn_cast<llvm::BinaryOperator>(&inst))
                    {
                        Expression left  = get_c_expr(binary_cast->getOperand(0), expressions);
                        Expression right = get_c_expr(binary_cast->getOperand(1), expressions);

                        switch(binary_cast->getOpcode())
                        {
                        case llvm::Instruction::Add:  new_expr = Expression::Add(left, right); break;
                        case llvm::Instruction::Sub:  new_expr = Expression::Sub(left, right); break;
                        case llvm::Instruction::Mul:  new_expr = Expression::Mul(left, right); break;
                        case llvm::Instruction::FAdd: new_expr = Expression::Add(left, right); break;
                        case llvm::Instruction::FSub: new_expr = Expression::Sub(left, right); break;
                        case llvm::Instruction::FMul: new_expr = Expression::Mul(left, right); break;
                        case llvm::Instruction::FDiv: new_expr = Expression::Div(left, right); break;
                        case llvm::Instruction::And:  new_expr = Expression::And(left, right); break;
                        case llvm::Instruction::Or:   new_expr = Expression::Or (left, right); break;
                        default:;
                        }
                    }
                    else if(auto* cmp_cast = llvm::dyn_cast<llvm::CmpInst>(&inst))
                    {
                        Expression left  = get_c_expr(cmp_cast->getOperand(0), expressions);
                        Expression right = get_c_expr(cmp_cast->getOperand(1), expressions);

                        switch(cmp_cast->getPredicate())
                        {
                        case llvm::CmpInst::Predicate::ICMP_SLT: new_expr = Expression::Less     (left, right); break;
                        case llvm::CmpInst::Predicate::ICMP_SLE: new_expr = Expression::Lesseq   (left, right); break;
                        case llvm::CmpInst::Predicate::ICMP_SGE: new_expr = Expression::Greatereq(left, right); break;
                        case llvm::CmpInst::Predicate::ICMP_SGT: new_expr = Expression::Greater  (left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_OLT: new_expr = Expression::Less     (left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_OLE: new_expr = Expression::Lesseq   (left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_OGE: new_expr = Expression::Greatereq(left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_OGT: new_expr = Expression::Greater  (left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_ULT: new_expr = Expression::Less     (left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_ULE: new_expr = Expression::Lesseq   (left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_UGE: new_expr = Expression::Greatereq(left, right); break;
                        case llvm::CmpInst::Predicate::FCMP_UGT: new_expr = Expression::Greater  (left, right); break;
                        default:;
                        }
                    }
                    else if(auto* call_cast = llvm::dyn_cast<llvm::CallInst>(&inst))
                    {
                        std::vector<std::pair<Expression,std::string>> tail;

                        tail.emplace_back(Expression::Deref(get_c_expr(call_cast->getCalledValue(), expressions)), "(");

                        for(auto it = call_cast->arg_begin(); it != call_cast->arg_end(); ++it)
                        {
                            tail.emplace_back(get_c_expr(*it, expressions), ", ");
                        }

                        tail.back().second = (tail.back().second == "(") ? "()" : ")";

                        new_expr = Expression("", tail, 2, UINT_MAX);
                    }
                    else if(auto* select_cast = llvm::dyn_cast<llvm::SelectInst>(&inst))
                    {
                        new_expr = Expression::Select(get_c_expr(select_cast->getCondition (), expressions),
                                                      get_c_expr(select_cast->getTrueValue (), expressions),
                                                      get_c_expr(select_cast->getFalseValue(), expressions));
                    }
                    else
                    {
                        std::stringstream str_str;
                        llvm::raw_os_ostream out_stream(str_str);
                        out_stream<<inst;
                        out_stream.flush();
                        str_str.flush();
                        new_expr = Expression::Atomic("/*"+str_str.str()+"  */");
                    }

                    unsigned relevant_uses = 0;
                    for(auto& use : inst.uses())
                    {
                        if(auto* store_cast = llvm::dyn_cast<llvm::StoreInst>(use.getUser()))
                        {
                            if(use.getOperandNo() == 1)
                            {
                                if(auto binary_cast = llvm::dyn_cast<llvm::BinaryOperator>(store_cast->getOperand(0)))
                                {
                                    bool hit = false;

                                    if(auto* load_cast = llvm::dyn_cast<llvm::LoadInst>(binary_cast->getOperand(0)))
                                    {
                                        if(load_cast->getOperand(0) == store_cast->getOperand(1)) hit = true;
                                    }
                                    if(auto* load_cast = llvm::dyn_cast<llvm::LoadInst>(binary_cast->getOperand(1)))
                                    {
                                        if(load_cast->getOperand(0) == store_cast->getOperand(1)) hit = true;
                                    }

                                    if(!hit) relevant_uses++;
                                }
                                else relevant_uses++;
                            }
                            else relevant_uses++;
                        }
                        else relevant_uses++;
                    }

                    if(relevant_uses > 1)
                    {
                        std::stringstream sstr;
                        sstr<<"t"<<(counter++);

                        Expression store_expr = Expression::Assign(Expression::Atomic(sstr.str()), new_expr);

                        output += "  "+print_c_type(inst.getType())+" "+(std::string)store_expr+";\n";

                        new_expr = Expression::Atomic(sstr.str());
                    }
                }

                if((std::string)new_expr != "")
                {
                    expressions[&inst] = new_expr;
                }
            }
        }
    }

    return print_c_type(function.getReturnType())+" "+std::string(function.getName())+"("+output+"}\n";
}

std::string print_pretty_c_operator(llvm::Function& function)
{
    return print_c_operator(function);
}
