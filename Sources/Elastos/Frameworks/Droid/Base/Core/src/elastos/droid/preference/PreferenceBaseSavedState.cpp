
#include "elastos/droid/preference/PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

//CAR_INTERFACE_IMPL(PreferenceBaseSavedState, AbsSavedState, IPreferenceBaseSavedState)
CAR_INTERFACE_IMPL_2(PreferenceBaseSavedState, Object, IPreferenceBaseSavedState, IParcelable)

PreferenceBaseSavedState::~PreferenceBaseSavedState()
{}

ECode PreferenceBaseSavedState::constructor()
{
    // return AbsSavedState::Init(source);
    return NOERROR;
}

ECode PreferenceBaseSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    // return AbsSavedState::Init(source);
    return NOERROR;
}

ECode PreferenceBaseSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // return AbsSavedState::ReadFromParcel(source);
    return NOERROR;
}

ECode PreferenceBaseSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // return AbsSavedState::WriteToParcel(dest);
    return NOERROR;
}

ECode PreferenceBaseSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    // VALIDATE_NOT_NULL(superState)
    // AutoPtr<IParcelable> temp = AbsSavedState::GetSuperState();
    // *superState = temp;
    // REFCOUNT_ADD(*superState);
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
