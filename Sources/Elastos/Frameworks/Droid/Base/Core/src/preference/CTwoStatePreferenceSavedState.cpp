
#include "CTwoStatePreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CTwoStatePreferenceSavedState::CTwoStatePreferenceSavedState()
    : mChecked(FALSE)
{}

ECode CTwoStatePreferenceSavedState::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    VALIDATE_NOT_NULL(isChecked)
    *isChecked = mChecked;
    return NOERROR;
}

ECode CTwoStatePreferenceSavedState::SetChecked(
    /* [in] */ Boolean checked)
{
    mChecked = checked;
    return NOERROR;
}

ECode CTwoStatePreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadBoolean(&mChecked);
    return NOERROR;
}

ECode CTwoStatePreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteBoolean(mChecked);
    return NOERROR;
}

ECode CTwoStatePreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CTwoStatePreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CTwoStatePreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

}
}
}

