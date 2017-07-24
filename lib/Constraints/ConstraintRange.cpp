#include "llvm/Constraints/ConstraintRange.hpp"
#include "llvm/Constraints/ConstraintAtomic.hpp"
#include "llvm/Constraints/BackendSpecializations.hpp"
#include <unordered_map>
#include <vector>
#include <list>

ConstraintRange::ConstraintRange(unsigned N, ConstraintContainer c, unsigned o, bool r)
               : constraint(std::move(c)), local_size(0), offset(o), size(N), reverse(r)
{
    auto flat_labels = constraint->get_labels();

    std::vector<unsigned> blank_indices;

    for(unsigned i = 0; i < flat_labels.size(); i++)
    {
        auto find_it = std::find_if(flat_labels[i].begin(), flat_labels[i].end(),
                                    [](char c){ return c == '+' || c == '*'; });

        if(find_it != flat_labels[i].end() && *find_it == '*')
        {
            local_indices.emplace_back(i, blank_indices.size());
            blank_indices.push_back(std::distance(flat_labels[i].begin(), find_it));
        }
        else if(find_it != flat_labels[i].end() && *find_it == '+')
        {
            next_indices.emplace_back(i, std::distance(flat_labels[i].begin(), find_it));
        }
        else
        {
            global_indices.push_back(i);
        }
    }

    for(auto& next_entry : next_indices)
    {
        unsigned index = blank_indices.size();

        for(auto& local_entry : local_indices)
        {
            if(flat_labels[next_entry.first].size() == flat_labels[local_entry.first].size() &&
               next_entry.second == blank_indices[local_entry.second] &&
               std::equal(flat_labels[next_entry.first].begin(),
                          flat_labels[next_entry.first].begin() + next_entry.second,
                          flat_labels[local_entry.first].begin()) &&
               std::equal(flat_labels[next_entry.first].begin() + next_entry.second + 1,
                          flat_labels[next_entry.first].end(),
                          flat_labels[local_entry.first].begin() + next_entry.second + 1))
            {
                index = local_entry.second;
                break;
            }
        }

        if(index == blank_indices.size())
        {
            blank_indices.push_back(next_entry.second);
        }

        next_entry.second = index;
    }

    local_size = blank_indices.size();

    labels.resize(local_indices.size() + (size-1)*local_size + next_indices.size() + global_indices.size());

    for(unsigned i0 = 0; i0 <= size; i0++)
    {
        unsigned i = reverse ? (size-i0) : i0;
        if(i < size)
        {
            for(unsigned j = 0; j < local_indices.size(); j++)
            {
                labels[(i==0)?j:(local_indices.size() + (i-1)*local_size + local_indices[j].second)] =
                     print_index(flat_labels[local_indices[j].first], blank_indices[local_indices[j].second], offset+i);
            }
        }
        if(i > 0)
        {
            for(unsigned j = 0; j < next_indices.size(); j++)
            {
                labels[local_indices.size() + (i-1)*local_size + ((i==size)?j:next_indices[j].second)] =
                       print_index(flat_labels[next_indices[j].first], blank_indices[next_indices[j].second], offset+i);
            }
        }
    }

    for(unsigned j = 0; j < global_indices.size(); j++)
    {
        labels[local_indices.size() + (size-1)*local_size + next_indices.size() + j] =
                                                                          std::move(flat_labels[global_indices[j]]);
    }
}

std::vector<std::string> ConstraintRange::get_labels(std::vector<std::string> use_vector) const
{
    use_vector.insert(use_vector.end(), labels.begin(), labels.end());
    return use_vector;
}

std::vector<SpecializedContainer> ConstraintRange::get_specials(FunctionWrapper& wrap,
                                                              std::vector<SpecializedContainer> use_vector) const
{
    std::vector<std::vector<SpecializedContainer>> special_vectors(labels.size());

    for(unsigned i0 = 0; i0 < size; i0++)
    {
        unsigned i = reverse ? (size-i0-1) : i0;

        auto old_result_size = use_vector.size();
        use_vector = constraint->get_specials(wrap, std::move(use_vector));

        for(unsigned j = 0; j < global_indices.size(); j++)
        {
            special_vectors[local_indices.size() + (size-1)*local_size + next_indices.size() + j].emplace_back(
                                                          std::move(use_vector[old_result_size+global_indices[j]]));
        }

        for(unsigned j = 0; j < local_indices.size(); j++)
        {
            special_vectors[(i==0)?j:(local_indices.size() + (i-1)*local_size + local_indices[j].second)].emplace_back(
                                                     std::move(use_vector[old_result_size+local_indices[j].first]));
        }

        for(unsigned j = 0; j < next_indices.size(); j++)
        {
            special_vectors[local_indices.size() + i*local_size + ((i+1==size)?j:next_indices[j].second)].emplace_back(
                                                  std::move(use_vector[old_result_size+next_indices[j].first]));
        }

        while(use_vector.size() > old_result_size) use_vector.pop_back();
    }

    use_vector.reserve(use_vector.size() + special_vectors.size());
    for(unsigned i = 0; i < special_vectors.size(); i++)
    {
        std::shared_ptr<BackendAnd_> backend(new BackendAnd_(std::move(special_vectors[i])));

        use_vector.emplace_back(ScalarSelector<BackendAnd_,0>((backend)));
    }

    return use_vector;
}

std::string ConstraintRange::print_index(const std::string& input, unsigned wild_pos, unsigned index)
{
    std::string new_label;
    new_label.reserve(input.size() + 2 + (index>=10) + (index>=100));

    new_label.insert(new_label.end(), input.begin(), input.begin() + wild_pos);

    new_label.push_back('[');

    if(index >= 100) new_label.push_back('0' + ((index/100)%10));
    if(index >= 10)  new_label.push_back('0' + ((index/10)%10));
    if(true)         new_label.push_back('0' + ((index/1)%10));

    new_label.push_back(']');

    new_label.insert(new_label.end(), input.begin() + (wild_pos + 1), input.end());

    return new_label;
}
