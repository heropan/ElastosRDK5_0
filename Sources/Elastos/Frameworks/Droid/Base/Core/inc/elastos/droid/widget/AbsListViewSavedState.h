
#ifndef __ELASTOS_DROID_WIDGET_ABSLISTVIEWSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_ABSLISTVIEWSAVEDSTATE_H__

#include "elastos/droid/view/ViewBaseSavedState.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::View::ViewBaseSavedState;

namespace Elastos {
namespace Droid {
namespace Widget {

class AbsListViewSavedState : public ViewBaseSavedState
{
public:
    AbsListViewSavedState();

    ~AbsListViewSavedState();
protected:
    AbsListViewSavedState(
        /* [in] */ IParcelable* superState);

public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

protected:
    CARAPI Init(
        /* [in] */ IParcelable* superState);

    CARAPI Init();

protected:
    Int64 mSelectedId;
    Int64 mFirstId;
    Int32 mViewTop;
    Int32 mPosition;
    Int32 mHeight;
    String mFilter;
    Boolean mInActionMode;
    Int32 mCheckedItemCount;
    AutoPtr< HashMap<Int32, Boolean> > mCheckState;
    AutoPtr< HashMap<Int64, Int32> > mCheckIdState;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif //__ELASTOS_DROID_WIDGET_ABSLISTVIEWSAVEDSTATE_H__
