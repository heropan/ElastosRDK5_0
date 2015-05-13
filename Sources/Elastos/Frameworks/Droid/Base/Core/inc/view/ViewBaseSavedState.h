
#ifndef __VIEWBASESAVEDSTATE_H__
#define __VIEWBASESAVEDSTATE_H__

#include "view/AbsSavedState.h"

namespace Elastos{
namespace Droid{
namespace View{

class ViewBaseSavedState : public AbsSavedState
{
public:
    ViewBaseSavedState();

protected:
    ViewBaseSavedState(
        /* [in] */ IParcelable* superState);

    ViewBaseSavedState(
        /* [in] */ IParcel* source);

    ~ViewBaseSavedState();

};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif //__VIEWBASESAVEDSTATE_H__
