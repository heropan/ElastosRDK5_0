#include "elastos/droid/widget/CAbsListViewSavedState.h"
#include "elastos/droid/view/View.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_OBJECT_IMPL(CAbsListViewSavedState);

ECode CAbsListViewSavedState::GetSuperState(
    /* [out] */  IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = View::BaseSavedState::GetSuperState();
    *superState = temp;
    REFCOUNT_ADD(*superState);

    return NOERROR;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
