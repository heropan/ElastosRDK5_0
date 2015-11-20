#include "CProgressBarSavedState.h"

namespace Elastos {
namespace Droid {
namespace Widget {

ECode CProgressBarSavedState::constructor(
        /* [in] */ IParcelable* superState)
{
    return ProgressBarSavedState::Init(superState);
}

ECode CProgressBarSavedState::constructor()
{
    return ProgressBarSavedState::Init();
}

ECode CProgressBarSavedState::ReadFromParcel(
        /* [in] */ IParcel *source)
{
    return ProgressBarSavedState::ReadFromParcel(source);
}

ECode CProgressBarSavedState::WriteToParcel(
        /* [in] */ IParcel *dest)
{
    return ProgressBarSavedState::WriteToParcel(dest);
}

ECode CProgressBarSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    AutoPtr<IParcelable> temp;
    temp = ProgressBarSavedState::GetSuperState();
    *superState = temp;
    REFCOUNT_ADD(*superState);
    return NOERROR;

}
}// namespace Widget
}// namespace Droid
}// namespace Elastos
