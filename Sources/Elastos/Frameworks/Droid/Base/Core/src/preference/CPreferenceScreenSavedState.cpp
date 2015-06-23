
#include "CPreferenceScreenSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CPreferenceScreenSavedState::CPreferenceScreenSavedState()
    : mIsDialogShowing(FALSE)
{}

ECode CPreferenceScreenSavedState::IsDialogShowing(
    /* [out] */ Boolean* isDialogShowing)
{
    VALIDATE_NOT_NULL(isDialogShowing)
    *isDialogShowing = mIsDialogShowing;
    return NOERROR;
}

ECode CPreferenceScreenSavedState::SetDialogShowing(
    /* [in] */ Boolean isDialogShowing)
{
    mIsDialogShowing = isDialogShowing;
    return NOERROR;
}

ECode CPreferenceScreenSavedState::SetDialogBundle(
    /* [in] */ IBundle* b)
{
    mDialogBundle = b;
    return NOERROR;
}

ECode CPreferenceScreenSavedState::GetDialogBundle(
    /* [out] */ IBundle** b)
{
    VALIDATE_NOT_NULL(b)
    *b = mDialogBundle;
    REFCOUNT_ADD(*b)
    return NOERROR;
}

ECode CPreferenceScreenSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteBoolean(mIsDialogShowing);
    dest->WriteInterfacePtr(mDialogBundle);
    return NOERROR;
}

ECode CPreferenceScreenSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadBoolean(&mIsDialogShowing);
    mDialogBundle = NULL;
    source->ReadInterfacePtr((Handle32*)(IBundle**)&mDialogBundle);
    return NOERROR;
}

ECode CPreferenceScreenSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CPreferenceScreenSavedState::constructor()
{
    return NOERROR;
}

ECode CPreferenceScreenSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return PreferenceBaseSavedState::Init(superState);
}

} // Preference
} // Droid
} // Elastos
