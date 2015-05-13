
#include "CEditTextPreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CEditTextPreferenceSavedState::GetText(
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text)
    *text = mText;
    return NOERROR;
}

ECode CEditTextPreferenceSavedState::SetText(
    /* [in] */ const String& text)
{
    mText = text;
    return NOERROR;
}

ECode CEditTextPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadString(&mText);
    return NOERROR;
}

ECode CEditTextPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteString(mText);
    return NOERROR;
}

ECode CEditTextPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CEditTextPreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CEditTextPreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

} // Preference
} // Droid
} // Elastos
