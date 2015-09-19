#include "CAbsListViewSavedState.h"

namespace Elastos{
namespace Droid{
namespace Widget{

ECode CAbsListViewSavedState::ReadFromParcel(
        /* [in] */  IParcel* source)
{
    AbsListViewSavedState::ReadFromParcel(source);
    return NOERROR;
}

ECode CAbsListViewSavedState::WriteToParcel(
        /* [in] */  IParcel* dest)
{
    AbsListViewSavedState::WriteToParcel(dest);
    return NOERROR;
}

ECode CAbsListViewSavedState::GetSuperState(
        /* [out] */  IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = ViewBaseSavedState::GetSuperState();
    *superState = temp;
    REFCOUNT_ADD(*superState);

    return NOERROR;
}

ECode CAbsListViewSavedState::constructor(
        /* [in] */  IParcelable* superState)
{
    AbsListViewSavedState::Init(superState);
    return NOERROR;
}

ECode CAbsListViewSavedState::constructor()
{
    AbsListViewSavedState::Init();
    return NOERROR;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
