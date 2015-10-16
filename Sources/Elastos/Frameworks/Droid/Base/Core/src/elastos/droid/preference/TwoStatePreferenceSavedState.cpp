
#include "elastos/droid/preference/TwoStatePreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

TwoStatePreferenceSavedState::TwoStatePreferenceSavedState()
    : mChecked(FALSE)
{}

ECode TwoStatePreferenceSavedState::constructor()
{
    PreferenceBaseSavedState::constructor();
    // Int32 i;
    // source->ReadInt32(&i);
    // mChecked =  i == 1;
    return NOERROR;
}

ECode TwoStatePreferenceSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return PreferenceBaseSavedState::constructor(superState);
}

CAR_INTERFACE_IMPL(TwoStatePreferenceSavedState, PreferenceBaseSavedState, ITwoStatePreferenceSavedState)

ECode TwoStatePreferenceSavedState::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    VALIDATE_NOT_NULL(isChecked)
    *isChecked = mChecked;
    return NOERROR;
}

ECode TwoStatePreferenceSavedState::SetChecked(
    /* [in] */ Boolean checked)
{
    mChecked = checked;
    return NOERROR;
}

ECode TwoStatePreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadBoolean(&mChecked);
    return NOERROR;
}

ECode TwoStatePreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteBoolean(mChecked);
    return NOERROR;
}

ECode TwoStatePreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

}
}
}

