#ifndef __ELASTOS_DROID_WIDGET_CABSLISTVIEWSAVEDSTATE_H__
#define __ELASTOS_DROID_WIDGET_CABSLISTVIEWSAVEDSTATE_H__

#include "_Elastos_Droid_Widget_CAbsListViewSavedState.h"
#include "elastos/droid/widget/AbsListView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CAbsListViewSavedState)
    , public AbsListView::SavedState
{
public:
    CAR_OBJECT_DECL();

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif // __ELASTOS_DROID_WIDGET_CABSLISTVIEWSAVEDSTATE_H__
