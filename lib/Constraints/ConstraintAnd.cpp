#include "llvm/Constraints/ConstraintClasses.hpp"
#include "llvm/Constraints/BackendClasses.hpp"
#include <unordered_map>
#include <vector>

ConstraintAnd::ConstraintAnd(std::vector<Constraint*> cvec)
{
    for(auto c : cvec)
        constraints.emplace_back(std::unique_ptr<Constraint>(c));

    std::vector<std::string> flat_labels;

    for(auto& constraint : this->constraints)
    {
        flat_labels.insert(flat_labels.end(), std::make_move_iterator(constraint->begin()),
                                              std::make_move_iterator(constraint->end()));
    }

    std::unordered_map<std::string,std::pair<unsigned,unsigned>> string_position(flat_labels.size() / 2);

    for(unsigned i = 0; i < flat_labels.size(); i++)
    {
        auto find_it = string_position.find(flat_labels[i]);

        if(find_it != string_position.end())
        {
            groupings[find_it->second.first].push_back(i);
            find_it->second.second++;
        }
        else
        {
            groupings.emplace_back(std::vector<unsigned>(1, i));
            string_position.emplace(std::move(flat_labels[i]), std::make_pair(string_position.size(), 1));
        }
    }

    resize(string_position.size());

    for(auto& entry : string_position)
    {
        (*this)[entry.second.first] = std::move(entry.first);
    }
}

void ConstraintAnd::get_specials(const FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const
{
    auto old_result_size = use_vector.size();

    for(auto& constraint : constraints)
    {
        constraint->get_specials(wrap, use_vector);
    }

    std::vector<std::vector<std::unique_ptr<SolverAtom>>> special_vectors(groupings.size());

    for(unsigned i = 0; i < groupings.size(); i++)
    {
        special_vectors[i].reserve(groupings[i].size());
        for(unsigned value : groupings[i])
        {
            special_vectors[i].emplace_back(std::move(use_vector[old_result_size+value]));
        }
    }

    while(use_vector.size() > old_result_size) use_vector.pop_back();

    for(unsigned i = 0; i < special_vectors.size(); i++)
    {
        if(special_vectors[i].size() > 1)
        {
            use_vector.emplace_back(std::unique_ptr<SolverAtom>(new BackendAnd(std::move(special_vectors[i]))));
        }
        else
        {
            use_vector.emplace_back(std::move(special_vectors[i][0]));
        }
    }
}
