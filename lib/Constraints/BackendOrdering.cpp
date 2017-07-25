#include "llvm/Constraints/BackendClasses.hpp"

template<bool lt, bool eq, bool gt>
BackendOrdering<lt,eq,gt>::BackendOrdering() : amount_completed(0) { }

template<bool lt, bool eq, bool gt>
template<unsigned idx>
SkipResult BackendOrdering<lt,eq,gt>::skip_invalid(Specialized::Value& c)
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

template      class BackendOrdering<false,false,false>;
template SkipResult BackendOrdering<false,false,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<false,false,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<false,false,true>;
template SkipResult BackendOrdering<false,false,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<false,false,true>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<false,true,false>;
template SkipResult BackendOrdering<false,true,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<false,true,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<false,true,true>;
template SkipResult BackendOrdering<false,true,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<false,true,true>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<true,false,false>;
template SkipResult BackendOrdering<true,false,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<true,false,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<true,false,true>;
template SkipResult BackendOrdering<true,false,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<true,false,true>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<true,true,false>;
template SkipResult BackendOrdering<true,true,false>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<true,true,false>::skip_invalid<1>(Specialized::Value&);
template      class BackendOrdering<true,true,true>;
template SkipResult BackendOrdering<true,true,true>::skip_invalid<0>(Specialized::Value&);
template SkipResult BackendOrdering<true,true,true>::skip_invalid<1>(Specialized::Value&);