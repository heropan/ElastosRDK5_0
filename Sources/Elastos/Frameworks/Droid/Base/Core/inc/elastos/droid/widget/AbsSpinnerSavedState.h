
#ifndef __ELASTOS_DROID_WIDGET_ABSSPINNERSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_ABSSPINNERSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"

using Elastos::Droid::View::ViewBaseSavedState;

namespace Elastos {
namespace Droid {
namespace Widget {

class AbsSpinnerSavedState : public ViewBaseSavedState
{
protected:

    AbsSpinnerSavedState();

    AbsSpinnerSavedState(
        /* [in] */ IParcelable* superState);

public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

protected:
    virtual CARAPI Init();

    virtual CARAPI Init(
        /* [in] */ IParcelable* superState);

public:
    Int64 mSelectedId;
    Int32 mPosition;
};

}// namespace Widget
}// Droid
}// Elastos

#endif //__ELASTOS_DROID_WIDGET_ABSSPINNERSAVEDSTATE_H__
