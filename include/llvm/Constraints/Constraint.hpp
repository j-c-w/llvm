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

    virtual std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                std::vector<SpecializedContainer> use_vector = {}) const = 0;

    std::vector<std::pair<Label,SpecializedContainer>> get_specializations(FunctionWrapper& wrap,
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

class ConstraintContainer : public Constraint, public std::unique_ptr<Constraint>
{
private:
    class TrivialConstraint : public Constraint
    {
    public:
        std::vector<Constraint::Label> get_labels(std::vector<Constraint::Label> use_vector = {}) const final
        {
            return use_vector;
        }

        std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                        std::vector<SpecializedContainer> use_vector) const final
        {
            return use_vector;
        }
    };

public:
    ConstraintContainer() : std::unique_ptr<Constraint>(new TrivialConstraint) { }

    explicit ConstraintContainer(Constraint* c) : std::unique_ptr<Constraint>(std::move(c)) { }


    template<typename Type,
             typename std::enable_if< std::is_base_of<Constraint,Type>::value>::type* = nullptr>
    ConstraintContainer(Type value) : std::unique_ptr<Constraint>(new Type(std::move(value))) { }

    ConstraintContainer(ConstraintContainer&& c) = default;

    std::vector<Label> get_labels(std::vector<Label> use_vector = {}) const final
    {
        return this->get()->get_labels(std::move(use_vector));
    }

    std::vector<SpecializedContainer> get_specials(FunctionWrapper& wrap,
                                                   std::vector<SpecializedContainer> use_vector = {}) const final
    {
        return this->get()->get_specials(wrap, std::move(use_vector));
    }
};

#endif