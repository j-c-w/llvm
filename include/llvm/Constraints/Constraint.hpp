#ifndef _CONSTRAINT_HPP_
#define _CONSTRAINT_HPP_
#include "llvm/Constraints/SMTSolver.hpp"

using SpecializedContainer = std::unique_ptr<Specialized>;

/* Constraints essentially only exist to generate instances of the above shown Specialized class. */
class Constraint
{
public:
    using Label = std::string;

    virtual ~Constraint() { };

    virtual std::vector<Label> get_labels(std::vector<Label> use_vector = {}) const = 0;

    virtual std::vector<SpecializedContainer> get_specials(FunctionWrap& wrap,
                                                           std::vector<SpecializedContainer> use_vector = {}) const = 0;

    std::vector<std::pair<Label,SpecializedContainer>> get_specializations(FunctionWrap& wrap,
                                std::vector<std::pair<Label,SpecializedContainer>> use_vector = {}) const
    {
        auto labels   = get_labels();
        auto specials = get_specials(wrap);

        use_vector.reserve(labels.size());
        for(unsigned i = 0; i < labels.size() && i < specials.size(); i++)
        {
            use_vector.emplace_back(std::move(labels[i]), std::move(specials[i]));
        }

        return use_vector;
    }
};

#endif
