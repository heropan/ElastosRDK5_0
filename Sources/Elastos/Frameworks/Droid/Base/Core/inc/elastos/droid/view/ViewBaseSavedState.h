
#ifndef __ELASTOS_DROID_VIEW_VIEWBASESAVEDSTATE_H__
#define __ELASTOS_DROID_VIEW_VIEWBASESAVEDSTATE_H__

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
#endif //__ELASTOS_DROID_VIEW_VIEWBASESAVEDSTATE_H__
