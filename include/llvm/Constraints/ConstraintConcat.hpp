#ifndef _CONSTRAINT_CONCAT_HPP_
#define _CONSTRAINT_CONCAT_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/ConstraintsBasic.hpp"
#include "llvm/Constraints/ConstraintOrdering.hpp"
#include "llvm/Constraints/ConstraintComplexUnused.hpp"
#include <memory>
#include <vector>

ConstraintAnd<std::string,unsigned> ConstraintConcat(unsigned offset, std::pair<std::string,std::string> dst)
{
    return ConstraintAnd<std::string,unsigned>();
}

template<typename ... Types>
ConstraintAnd<std::string,unsigned> ConstraintConcat(unsigned offset, std::pair<std::string,std::string> dst,
                                                     std::string scalar_variable, Types ... tail);

template<typename ... Types>
ConstraintAnd<std::string,unsigned> ConstraintConcat(unsigned offset, std::pair<std::string,std::string> dst,
                                                     std::pair<std::string,std::string> src, unsigned srclen, 
                                                     Types ... tail)
{
    std::vector<ConstraintSame> constraints;

    for(unsigned j = 0; j < srclen; j++)
    {
        std::stringstream sstr1, sstr2;
        sstr1<<dst.first<<"["<<offset+j<<"]"<<(dst.second.empty()?"":".")<<dst.second;
        sstr2<<src.first<<"["<<j<<"]"<<(src.second.empty()?"":".")<<src.second;

        constraints.emplace_back(sstr1.str(), sstr2.str());
    }

    return ConstraintAnd<std::string,unsigned>(constraints,
                                               ConstraintConcat(offset+srclen, dst, tail...));
}

template<typename ... Types>
ConstraintAnd<std::string,unsigned> ConstraintConcat(unsigned offset, std::pair<std::string,std::string> dst,
                                                     std::string scalar_variable, Types ... tail)
{
    std::stringstream sstr;
    sstr<<dst.first<<"["<<offset<<"]"<<(dst.second.empty()?"":".")<<dst.second;

    return ConstraintAnd<std::string,unsigned>(ConstraintSame(sstr.str(), scalar_variable),
                                               ConstraintConcat(offset+1, dst, tail...));
}

template<typename ... Types>
ConstraintAnd<std::string,unsigned> ConstraintConcat(std::pair<std::string,std::string> dst, Types ... tail)
{
    return ConstraintConcat(0, dst, tail...);
}

#endif
