
#ifndef __PROGRESSBARSAVEDSTATE_H__
#define __PROGRESSBARSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"

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

#endif //__PROGRESSBARSAVEDSTATE_H__
