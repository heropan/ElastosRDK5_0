
#include "CPreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CPreferenceBaseSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return PreferenceBaseSavedState::ReadFromParcel(source);
}

ECode CPreferenceBaseSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return PreferenceBaseSavedState::WriteToParcel(dest);
}

ECode CPreferenceBaseSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CPreferenceBaseSavedState::constructor()
{
    return NOERROR;
}

ECode CPreferenceBaseSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    PreferenceBaseSavedState::Init(source);
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
