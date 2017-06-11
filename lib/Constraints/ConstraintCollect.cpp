#include "llvm/Constraints/ConstraintCollect.hpp"
#include "llvm/Constraints/BackendSpecializations.hpp"

ConstraintCollect::ConstraintCollect(unsigned n, std::string prefix, ConstraintContainer c)
                 : constraint(std::move(c)), size(n)
{
    std::vector<std::string> global_names;
    std::vector<std::string> local_names;
    
    auto use_vector = constraint->get_labels();

    for(unsigned i = 0; i < use_vector.size(); i++)
    {
        if(use_vector[i].empty() || use_vector[i].front() != '@')
        {
            local_indices.push_back(i);
            local_names.push_back(std::move(use_vector[i]));
        }
        else
        {
            global_indices.push_back(i);
            global_names.emplace_back(use_vector[i].begin() + 1, use_vector[i].end());
        }
    }


    labels.reserve(global_names.size() + size * local_names.size());
    for(unsigned i = 0; i < global_names.size(); i++)
    {
        labels.emplace_back(global_names[i]);
    }

    std::string indexed_prefix = prefix+"[";
    for(unsigned i = 0; i < size; i++)
    {
        if(i == 0 || i == 10 || i == 100) indexed_prefix.push_back('0');
        if(i>=100) indexed_prefix[indexed_prefix.size()-3] = '0'+((i/100)%10);
        if(i>=10)  indexed_prefix[indexed_prefix.size()-2] = '0'+((i/10)%10);
        if(true)   indexed_prefix[indexed_prefix.size()-1] = '0'+((i/1)%10);

        for(unsigned j = 0; j < local_names.size(); j++)
        {
            std::string final_name = indexed_prefix+"]"+(local_names[j].empty()?"":("."+local_names[j]));
            labels.emplace_back(final_name);
        }
    }
}

std::vector<std::string> ConstraintCollect::get_labels(std::vector<std::string> use_vector) const
{
    use_vector.insert(use_vector.end(), labels.begin(), labels.end());
    return use_vector;
}


std::vector<SpecializedContainer> ConstraintCollect::get_specials(FunctionWrapper& wrap,
                                                              std::vector<SpecializedContainer> use_vector) const
{
    std::vector<SpecializedContainer> globals;
    std::vector<SpecializedContainer> locals;

    unsigned long old_result_size = use_vector.size();

    use_vector = constraint->get_specials(wrap, std::move(use_vector));

    for(auto index : global_indices)
    {
        globals.emplace_back(std::move(use_vector[old_result_size + index]));
    }

    for(auto index : local_indices)
    {
        locals.emplace_back(std::move(use_vector[old_result_size + index]));
    }

    while(use_vector.size() > old_result_size) use_vector.pop_back();

    unsigned size1 = globals.size();
    unsigned size2 = size*locals.size();

    auto backend = BackendCollect::Create({{size1, size2}}, std::move(globals), std::move(locals));

    use_vector.reserve(use_vector.size() + globals.size() + size * locals.size());
    for(unsigned i = 0; i < std::get<0>(backend).size(); i++)
    {
        use_vector.push_back(std::get<0>(backend)[i]);
    }

    for(unsigned j = 0; j < std::get<1>(backend).size(); j++)
    {
        use_vector.emplace_back(std::get<1>(backend)[j]);
    }

    return use_vector;
}