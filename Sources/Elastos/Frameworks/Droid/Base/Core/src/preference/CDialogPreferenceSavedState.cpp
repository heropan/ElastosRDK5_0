
#include "CDialogPreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CDialogPreferenceSavedState::CDialogPreferenceSavedState()
    : mIsDialogShowing(FALSE)
{}

ECode CDialogPreferenceSavedState::IsDialogShowing(
    /* [out] */ Boolean* isShow)
{
    VALIDATE_NOT_NULL(isShow)
    *isShow = mIsDialogShowing;
    return NOERROR;
}

ECode CDialogPreferenceSavedState::SetDialogShowing(
    /* [in] */ Boolean isShow)
{
    mIsDialogShowing = isShow;
    return NOERROR;
}

ECode CDialogPreferenceSavedState::GetDialogBundle(
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    *bundle = mDialogBundle;
    INTERFACE_ADDREF(*bundle)
    return NOERROR;
}

ECode CDialogPreferenceSavedState::SetDialogBundle(
    /* [in] */ IBundle* bundle)
{
    mDialogBundle = bundle;
    return NOERROR;
}

ECode CDialogPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadBoolean(&mIsDialogShowing);
    mDialogBundle = NULL;
    source->ReadInterfacePtr((Handle32*)(IBundle**)&mDialogBundle);
    return NOERROR;
}

ECode CDialogPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteBoolean(mIsDialogShowing);
    dest->WriteInterfacePtr(mDialogBundle);
    return NOERROR;
}

ECode CDialogPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CDialogPreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CDialogPreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

} // Preference
} // Droid
} // Elastos
