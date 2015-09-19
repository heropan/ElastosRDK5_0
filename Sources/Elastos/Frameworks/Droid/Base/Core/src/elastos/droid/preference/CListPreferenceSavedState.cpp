
#include "CListPreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CListPreferenceSavedState::GetValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mValue;
    return NOERROR;
}

ECode CListPreferenceSavedState::SetValue(
    /* [in] */ const String& value)
{
    mValue = value;
    return NOERROR;
}

ECode CListPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadString(&mValue);
    return NOERROR;
}

ECode CListPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteString(mValue);
    return NOERROR;
}

ECode CListPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CListPreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CListPreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

} // Preference
} // Droid
} // Elastos
