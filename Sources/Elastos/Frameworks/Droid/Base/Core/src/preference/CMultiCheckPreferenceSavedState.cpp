
#include "CMultiCheckPreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CMultiCheckPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    mValues = NULL;
    source->ReadArrayOf((Handle32*)(&mValues));
    return NOERROR;
}

ECode CMultiCheckPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteArrayOf((Handle32)mValues.Get());
    return NOERROR;
}

ECode CMultiCheckPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CMultiCheckPreferenceSavedState::GetValues(
    /* [out, callee] */ ArrayOf<Boolean>** values)
{
    VALIDATE_NOT_NULL(values)
    *values = mValues;
    REFCOUNT_ADDREF(*values)
    return NOERROR;
}

ECode CMultiCheckPreferenceSavedState::SetValues(
    /* [in] */ ArrayOf<Boolean>* values)
{
    mValues = values;
    return NOERROR;
}

ECode CMultiCheckPreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CMultiCheckPreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

} // Preference
} // Droid
} // Elastos
