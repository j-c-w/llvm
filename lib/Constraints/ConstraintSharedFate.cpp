#include "llvm/Constraints/ConstraintSharedFate.hpp"

std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> ConstraintSharedFate::get_specializations() const
{
    std::vector<std::pair<std::string,std::unique_ptr<Specialized<unsigned>>>> result;

    std::shared_ptr<SharedData> cache = std::make_shared<SharedData>();

    cache->filled_slots = 0;
    cache->invalidated  = false;

    for(std::string var : variables)
    {
        result.emplace_back(var, std::unique_ptr<Specialized<unsigned>>(new SpecializedSharedFate(cache)));
    }

    return result; 
}

ConstraintSharedFate::SpecializedSharedFate::SpecializedSharedFate(std::shared_ptr<SharedData> c)
                                           : cache(c) { }

SkipResult ConstraintSharedFate::SpecializedSharedFate::skip_invalid(unsigned& c)
{
    if(cache->invalidated) return SkipResult::FAIL;
    else                   return SkipResult::PASS;
}

void ConstraintSharedFate::SpecializedSharedFate::begin()
{
    if(cache->filled_slots == 0) {
        cache->invalidated = false;
    }
}

void ConstraintSharedFate::SpecializedSharedFate::resume()
{
    cache->filled_slots--;
}

void ConstraintSharedFate::SpecializedSharedFate::cancel()
{
    if(cache->invalidated == false) {
        cache->invalidated = true;
    }
}

void ConstraintSharedFate::SpecializedSharedFate::fixate()
{
    cache->filled_slots++;
}
