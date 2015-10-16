
#ifndef __ELASTOS_DROID_WIDGET_PROGRESSBARSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_PROGRESSBARSAVEDSTATE_H__

#include "elastos/droid/view/ViewBaseSavedState.h"

using Elastos::Droid::View::ViewBaseSavedState;

namespace Elastos {
namespace Droid {
namespace Widget {

class ProgressBarSavedState : public ViewBaseSavedState
{
protected:

    ProgressBarSavedState();

    ProgressBarSavedState(
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
    Int32 mProgress;
    Int32 mSecondaryProgress;
};

}// namespace Widget
}// Droid
}// Elastos

#endif //__ELASTOS_DROID_WIDGET_PROGRESSBARSAVEDSTATE_H__
