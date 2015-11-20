
#include "elastos/droid/view/CViewBaseSavedState.h"
#include "elastos/droid/ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CViewBaseSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ViewBaseSavedState::ReadFromParcel(source);
}

ECode CViewBaseSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ViewBaseSavedState::WriteToParcel(dest);
}

ECode CViewBaseSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = ViewBaseSavedState::GetSuperState();
    *superState = temp;
    REFCOUNT_ADD(*superState);

    return NOERROR;
}

ECode CViewBaseSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

ECode CViewBaseSavedState::constructor(
    /* [in] */ IParcel* source)
{
    return ViewBaseSavedState::Init(source);
}

ECode CViewBaseSavedState::constructor()
{
    return ViewBaseSavedState::Init();
}

}// namespace View
}// namespace Droid
}// namespace Elastos
