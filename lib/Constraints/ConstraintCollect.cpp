#include "llvm/Constraints/ConstraintClasses.hpp"
#include "llvm/Constraints/BackendSpecializations.hpp"
#include <vector>

ConstraintCollect::ConstraintCollect(unsigned n, std::string prefix, Constraint* c)
                 : constraint(c), size(n)
{
    std::vector<std::string>                        global_names;
    std::vector<std::pair<std::string,std::string>> local_names;

    for(auto& label : *constraint)
    {
        bool islocal = false;

        for(unsigned j = 0; j + prefix.size() + 1 < label.size() && !islocal; j++)
        {
            if(label[j] == '[' && label[j+prefix.size()+1] == ']' &&
               std::string(label.begin() + j+1, label.begin() + j+prefix.size()+1) == prefix)
            {
                local_indices.push_back(local_indices.size() + global_indices.size());
                local_names.emplace_back(std::string(label.begin(), label.begin()+j+1),
                                         std::string(label.begin()+j+prefix.size()+1, label.end()));
                islocal = true;
                break;
            }
        }

        if(!islocal)
        {
            global_indices.push_back(local_indices.size() + global_indices.size());
            global_names.push_back(label);
        }
    }

    reserve(global_names.size() + size * local_names.size());
    for(unsigned i = 0; i < global_names.size(); i++)
    {
        emplace_back(global_names[i]);
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
            emplace_back(final_name);
        }
    }
}

void ConstraintCollect::get_specials(FunctionWrap& wrap, std::vector<std::unique_ptr<SolverAtom>>& use_vector) const
{
    std::vector<std::unique_ptr<SolverAtom>> globals;
    std::vector<std::unique_ptr<SolverAtom>> locals;

    unsigned long old_result_size = use_vector.size();

    constraint->get_specials(wrap, use_vector);

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
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new MultiVectorSelector<BackendCollect,0>(backend, i)));
    }

    for(unsigned i = 0; i < size2; i++)
    {
        use_vector.emplace_back(std::unique_ptr<SolverAtom>(new MultiVectorSelector<BackendCollect,1>(backend, i)));
    }
}
