
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode PreferenceBaseSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return AbsSavedState::Init(superState);
}

ECode PreferenceBaseSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return AbsSavedState::ReadFromParcel(source);
}

ECode PreferenceBaseSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return AbsSavedState::WriteToParcel(dest);
}

ECode PreferenceBaseSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState)
    AutoPtr<IParcelable> temp = AbsSavedState::GetSuperState();
    *superState = temp;
    REFCOUNT_ADD(*superState);
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
