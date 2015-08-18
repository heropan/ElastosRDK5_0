#ifndef __ELASTOS_DROID_WIDGET_CPROGRESSBARSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_CPROGRESSBARSAVEDSTATE_H__

#include "_Elastos_Droid_Widget_CProgressBarSavedState.h"
#include "ProgressBarSavedState.h"

namespace Elastos{
namespace Droid {
namespace Widget {

CarClass(CProgressBarSavedState) , ProgressBarSavedState
{
public:

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    using ProgressBarSavedState::mProgress;
    using ProgressBarSavedState::mSecondaryProgress;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
