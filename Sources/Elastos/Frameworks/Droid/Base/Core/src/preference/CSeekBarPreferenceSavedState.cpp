
#include "CSeekBarPreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CSeekBarPreferenceSavedState::CSeekBarPreferenceSavedState()
    : mProgress(0)
    , mMax(0)
{}

ECode CSeekBarPreferenceSavedState::SetProgress(
    /* [in] */ Int32 progress)
{
    mProgress = progress;
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress)
    *progress = mProgress;
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::SetMax(
    /* [in] */ Int32 max)
{
    mMax = max;
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::GetMax(
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max)
    *max = mMax;
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mProgress);
    dest->WriteInt32(mMax);
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadInt32(&mProgress);
    source->ReadInt32(&mMax);
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode CSeekBarPreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CSeekBarPreferenceSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return PreferenceBaseSavedState::Init(superState);
}

} // Preference
} // Droid
} // Elastos
