#include "llvm/Constraints/Backends.hpp"

template<bool lt, bool eq, bool gt>
BackendOrdering_<lt,eq,gt>::BackendOrdering_()
                                : amount_completed(0)
{ }

template<bool lt, bool eq, bool gt>
template<unsigned idx>
SkipResult BackendOrdering_<lt,eq,gt>::skip_invalid(Specialized::Value& c)
{
    if(amount_completed == 1)
    {
        if(c < other_value)
        {
            if((lt && idx == 0) || (gt && idx == 1))
            {
                return SkipResult::PASS;
            }
            else if(eq)
            {
                c = other_value;
                return SkipResult::CHANGEPASS;
            }
            else if(((gt && idx == 0) || (lt && idx == 1)) && other_value < UINT_MAX)
            {
                c = other_value + 1;
                return SkipResult::CHANGEPASS;
            }
            else return SkipResult::FAIL;
        }
        else if(c == other_value)
        {
            if(eq)
            {
                return SkipResult::PASS;
            }
            else if(((gt && idx == 0) || (lt && idx == 1)) && other_value < UINT_MAX)
            {
                c = c + 1;
                return SkipResult::CHANGEPASS;
            }
            else return SkipResult::FAIL;
        }
        else if((gt && idx == 0) || (lt && idx == 1))
        {
            return SkipResult::PASS;
        }
        else return SkipResult::FAIL;
    }
    else
    {
        other_value = c;
        return SkipResult::PASS;
    }
}

template      class BackendOrdering_<false,false,false>;
template SkipResult BackendOrdering_<false,false,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<false,false,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<false,false,true>;
template SkipResult BackendOrdering_<false,false,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<false,false,true>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<false,true,false>;
template SkipResult BackendOrdering_<false,true,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<false,true,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<false,true,true>;
template SkipResult BackendOrdering_<false,true,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<false,true,true>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<true,false,false>;
template SkipResult BackendOrdering_<true,false,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<true,false,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<true,false,true>;
template SkipResult BackendOrdering_<true,false,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<true,false,true>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<true,true,false>;
template SkipResult BackendOrdering_<true,true,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<true,true,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering_<true,true,true>;
template SkipResult BackendOrdering_<true,true,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering_<true,true,true>::skip_invalid<1>(Specialized::Value&);