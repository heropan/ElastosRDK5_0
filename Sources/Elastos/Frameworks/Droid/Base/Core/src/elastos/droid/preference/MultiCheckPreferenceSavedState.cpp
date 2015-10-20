
#include "elastos/droid/preference/MultiCheckPreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode MultiCheckPreferenceSavedState::constructor()
{
    PreferenceBaseSavedState::constructor();
    // source->CreateBooleanArray((ArrayOf<Boolean>**)&mValues);
    return NOERROR;
}

ECode MultiCheckPreferenceSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return PreferenceBaseSavedState::constructor(superState);
}

CAR_INTERFACE_IMPL(MultiCheckPreferenceSavedState, PreferenceBaseSavedState, IMultiCheckPreferenceSavedState)

ECode MultiCheckPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    mValues = NULL;
    source->ReadArrayOf((Handle32*)(&mValues));
    return NOERROR;
}

ECode MultiCheckPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteArrayOf((Handle32)mValues.Get());
    return NOERROR;
}

ECode MultiCheckPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode MultiCheckPreferenceSavedState::GetValues(
    /* [out, callee] */ ArrayOf<Boolean>** values)
{
    VALIDATE_NOT_NULL(values)
    *values = mValues;
    REFCOUNT_ADD(*values)
    return NOERROR;
}

ECode MultiCheckPreferenceSavedState::SetValues(
    /* [in] */ ArrayOf<Boolean>* values)
{
    mValues = values;
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
