
#include "CMultiSelectListPreferenceSavedState.h"

using Elastos::Utility::CHashSet;

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CMultiSelectListPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    mValues = NULL;
    source->ReadInterfacePtr((Handle32*)(ISet**)&mValues);
    return NOERROR;
}

ECode CMultiSelectListPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteInterfacePtr(mValues);
    return NOERROR;
}

ECode CMultiSelectListPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CMultiSelectListPreferenceSavedState::GetValues(
    /* [out] */ ISet** values)
{
    VALIDATE_NOT_NULL(values)
    *values = mValues;
    REFCOUNT_ADD(*values)
    return NOERROR;
}

ECode CMultiSelectListPreferenceSavedState::SetValues(
    /* [in] */ ISet* values)
{
    mValues = values;
    return NOERROR;
}

ECode CMultiSelectListPreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CMultiSelectListPreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

} // Preference
} // Droid
} // Elastos
