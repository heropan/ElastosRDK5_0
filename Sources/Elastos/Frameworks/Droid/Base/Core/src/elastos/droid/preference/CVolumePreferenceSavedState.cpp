
#include "CVolumePreferenceSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CVolumePreferenceSavedState::CVolumePreferenceSavedState()
{
    mVolumeStore = new VolumePreference::VolumeStore();
}

ECode CVolumePreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    source->ReadInt32(&mVolumeStore->mVolume);
    source->ReadInt32(&mVolumeStore->mOriginalVolume);
    return NOERROR;
}

ECode CVolumePreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mVolumeStore->mVolume);
    dest->WriteInt32(mVolumeStore->mOriginalVolume);
    return NOERROR;
}

ECode CVolumePreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

AutoPtr<VolumePreference::VolumeStore> CVolumePreferenceSavedState::GetVolumeStore()
{
    return mVolumeStore;
}

ECode CVolumePreferenceSavedState::constructor()
{
    return NOERROR;
}

ECode CVolumePreferenceSavedState::constructor(
    /* [in] */ IParcelable* source)
{
    return PreferenceBaseSavedState::Init(source);
}

} // Preference
} // Droid
} // Elastos
