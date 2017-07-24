#include "llvm/Constraints/ConstraintPrefix.hpp"

ConstraintPrefix::ConstraintPrefix(std::string prefix, ConstraintContainer c)
                : constraint(std::move(c)), labels(constraint->get_labels())
{
    if(prefix.empty() || prefix.back() == '@')
    {
        for(auto& label : labels)
        {
            if(label.size() > 0 && label.back() == '@')
            {
                label.erase(label.begin());
            }
            else
            {
                label.insert(label.begin(), prefix.begin(), prefix.end());
            }
        }
    }
    else
    {
        prefix.push_back('.');

        for(auto& label : labels)
        {
            if(label.size() > 0 && label.front() == '@')
            {
                label.erase(label.begin());
            }
            else
            {
                label.insert(label.begin(), prefix.begin(), prefix.end()-(label.empty()?1:0));
            }
        }

        prefix.pop_back();
    }
}

std::vector<std::string> ConstraintPrefix::get_labels(std::vector<std::string> use_vector) const
{
    use_vector.insert(use_vector.end(), labels.begin(), labels.end());
    return use_vector;
}

std::vector<SpecializedContainer> ConstraintPrefix::get_specials(FunctionWrapper& wrap,
                                                                 std::vector<SpecializedContainer> use_vector) const
{
    return constraint->get_specials(wrap, std::move(use_vector));
}

ConstraintRename::ConstraintRename(ConstraintContainer c, std::vector<std::pair<std::string,std::string>> renames)
                : constraint(std::move(c)), labels(constraint->get_labels())
{
    for(auto& label : labels)
    {
        for(const auto& pair : renames)
        {
            if(label.size() >= pair.first.size() &&
               std::string(label.begin(), label.begin() + pair.first.size()) == pair.first &&
               (label.size() == pair.first.size() ||
                label[pair.first.size()] == '.' || label[pair.first.size()] == '['))
            {
                label = pair.second + std::string(label.begin() + pair.first.size(), label.end());
                break;
            }
        }
    }
}

std::vector<std::string> ConstraintRename::get_labels(std::vector<std::string> use_vector) const
{
    use_vector.insert(use_vector.end(), labels.begin(), labels.end());
    return use_vector;
}

std::vector<SpecializedContainer> ConstraintRename::get_specials(FunctionWrapper& wrap,
                                                                 std::vector<SpecializedContainer> use_vector) const
{
    return constraint->get_specials(wrap, std::move(use_vector));
}
