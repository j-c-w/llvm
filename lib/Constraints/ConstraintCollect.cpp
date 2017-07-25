#include "llvm/Constraints/ConstraintClasses.hpp"
#include "llvm/Constraints/BackendSpecializations.hpp"

ConstraintCollect::ConstraintCollect(unsigned n, std::string prefix, Constraint* c)
                 : constraint(c), size(n)
{
    std::vector<std::string>                        global_names;
    std::vector<std::pair<std::string,std::string>> local_names;
    
    auto use_vector = constraint->get_labels();

    for(unsigned i = 0; i < use_vector.size(); i++)
    {
        bool islocal = false;

        for(unsigned j = 0; j + prefix.size() + 1 < use_vector[i].size() && !islocal; j++)
        {
            if(use_vector[i][j] == '[' && use_vector[i][j+prefix.size()+1] == ']' &&
               std::string(use_vector[i].begin() + j+1, use_vector[i].begin() + j+prefix.size()+1) == prefix)
            {
                local_indices.push_back(i);
                local_names.emplace_back(std::string(use_vector[i].begin(), use_vector[i].begin()+j+1),
                                         std::string(use_vector[i].begin()+j+prefix.size()+1, use_vector[i].end()));
                islocal = true;
            }
        }

        if(!islocal)
        {
            global_indices.push_back(i);
            global_names.push_back(use_vector[i]);
        }
    }

    labels.reserve(global_names.size() + size * local_names.size());
    for(unsigned i = 0; i < global_names.size(); i++)
    {
        labels.emplace_back(global_names[i]);
    }

    std::string number_string = "";
    for(unsigned i = 0; i < size; i++)
    {
        if(i == 0 || i == 10 || i == 100) number_string.push_back('0');
        if(i>=100) number_string[number_string.size()-3] = '0'+((i/100)%10);
        if(i>=10)  number_string[number_string.size()-2] = '0'+((i/10)%10);
        if(true)   number_string[number_string.size()-1] = '0'+((i/1)%10);

        for(unsigned j = 0; j < local_names.size(); j++)
        {
            std::string final_name = local_names[j].first + number_string + local_names[j].second;
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

    std::shared_ptr<BackendCollect> backend(new BackendCollect({{size1, size2}}, std::move(globals), std::move(locals)));

    use_vector.reserve(use_vector.size() + globals.size() + size * locals.size());
    for(unsigned i = 0; i < size1; i++)
    {
        use_vector.push_back(VectorSelector<BackendCollect,0>(backend, i));
    }

    for(unsigned i = 0; i < size2; i++)
    {
        use_vector.emplace_back(VectorSelector<BackendCollect,1>(backend, i));
    }

    return use_vector;
}