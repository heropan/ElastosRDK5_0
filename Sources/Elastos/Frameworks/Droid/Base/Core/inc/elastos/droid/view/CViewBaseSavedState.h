
#ifndef __ELASTOS_DROID_VIEW_CVIEWBASESAVEDSTATE_H__
#define __ELASTOS_DROID_VIEW_CVIEWBASESAVEDSTATE_H__

#include "_Elastos_Droid_View_CViewBaseSavedState.h"
#include "elastos/droid/view/ViewBaseSavedState.h"

namespace Elastos{
namespace Droid{
namespace View{

CarClass(CViewBaseSavedState), public ViewBaseSavedState
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor(
        /* [in] */ IParcel* source);

    CARAPI constructor();

private:
};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif // __ELASTOS_DROID_VIEW_CVIEWBASESAVEDSTATE_H__
