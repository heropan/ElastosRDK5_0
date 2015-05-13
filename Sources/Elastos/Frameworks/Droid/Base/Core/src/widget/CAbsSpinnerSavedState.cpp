#include "CAbsSpinnerSavedState.h"

namespace Elastos{
namespace Droid {
namespace Widget {

ECode CAbsSpinnerSavedState::constructor(
        /* [in] */ IParcelable* superState)
{
    return AbsSpinnerSavedState::Init(superState);
}

ECode CAbsSpinnerSavedState::constructor()
{
    return AbsSpinnerSavedState::Init();
}

ECode CAbsSpinnerSavedState::ReadFromParcel(
        /* [in] */ IParcel *source)
{
    return AbsSpinnerSavedState::ReadFromParcel(source);
}

ECode CAbsSpinnerSavedState::WriteToParcel(
        /* [in] */ IParcel *dest)
{
    return AbsSpinnerSavedState::WriteToParcel(dest);
}

ECode CAbsSpinnerSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    AutoPtr<IParcelable> temp;
    temp = AbsSpinnerSavedState::GetSuperState();
    *superState = temp;
    INTERFACE_ADDREF(*superState);
    return NOERROR;

}
}// namespace Widget
}// namespace Droid
}// namespace Elastos
