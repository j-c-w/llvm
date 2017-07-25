#include "llvm/Constraints/ConstraintClasses.hpp"
#include "llvm/Constraints/BackendClasses.hpp"
#include "llvm/Constraints/ConstraintSpecializations.hpp"
#include <unordered_map>
#include <vector>

ConstraintOr::ConstraintOr(std::vector<Constraint*> cvec)
{
    for(auto c : cvec)
        constraints.emplace_back(std::unique_ptr<Constraint>(c));

    std::vector<std::string> flat_labels;
    std::vector<unsigned>    result_boundaries;

    for(auto& constraint : this->constraints)
    {
        flat_labels = constraint->get_labels(std::move(flat_labels));
        result_boundaries.push_back(flat_labels.size());
    }

    std::reverse(result_boundaries.begin(), result_boundaries.end());

    unsigned active_branch = 0;

    std::unordered_map<Label,unsigned> string_position(flat_labels.size() / 2);
    for(unsigned i = 0; i < flat_labels.size(); i++)
    {
        if(i == result_boundaries.back())
        {
            result_boundaries.pop_back();
            active_branch++;
        }

        auto find_it = string_position.find(flat_labels[i]);

        if(find_it != string_position.end())
        {
            while(groupings[find_it->second].size() < active_branch)
            {
                groupings[find_it->second].push_back(UINT_MAX);
            }

            groupings[find_it->second].push_back(i);
        }
        else
        {
            groupings.emplace_back(std::vector<unsigned>(active_branch, UINT_MAX));
            groupings.back().push_back(i);

            string_position.emplace(std::move(flat_labels[i]), string_position.size());
        }
    }

    labels.resize(string_position.size());

    for(auto& entry : string_position)
    {
        labels[entry.second] = std::move(entry.first);
    }

    for(auto& group : groupings)
    {
        while(group.size() < this->constraints.size())
        {
            group.push_back(UINT_MAX);
        }
    }
}

std::vector<Constraint::Label> ConstraintOr::get_labels(std::vector<Constraint::Label> use_vector) const
{
    use_vector.insert(use_vector.end(), labels.begin(), labels.end());
    return use_vector;
}

std::vector<SpecializedContainer> ConstraintOr::get_specials(FunctionWrap& wrap,
                                                             std::vector<SpecializedContainer> use_vector) const
{
    auto old_result_size = use_vector.size();

    for(auto& constraint : constraints)
    {
        use_vector = constraint->get_specials(wrap, std::move(use_vector));
    }

    std::vector<std::vector<SpecializedContainer>> special_vectors(groupings.size());

    for(unsigned i = 0; i < groupings.size(); i++)
    {
        special_vectors[i].reserve(groupings[i].size());
        for(unsigned value : groupings[i])
        {
            if(value < use_vector.size() - old_result_size)
            {
                special_vectors[i].emplace_back(std::move(use_vector[old_result_size+value]));
            }
            else
            {   // This is highly unsatisfactory and a big problem, sad!
                special_vectors[i].emplace_back(std::move(ConstraintUnused().get_specials(wrap)[0]));
            }
        }
    }

    while(use_vector.size() > old_result_size) use_vector.pop_back();

    unsigned size = special_vectors.size();


    std::shared_ptr<BackendOr> backend(new BackendOr({{size}}, std::move(special_vectors)));

    for(unsigned i = 0; i < size && i < labels.size(); i++)
    {
        use_vector.emplace_back((VectorSelector<BackendOr,0>(backend, i)));
    }

    return use_vector;
}
