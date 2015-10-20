
#include "elastos/droid/preference/VolumePreferenceSavedState.h"
#include "elastos/droid/preference/VolumePreference.h"
namespace Elastos {
namespace Droid {
namespace Preference {

VolumePreferenceSavedState::VolumePreferenceSavedState()
{
    // VolumePreference::VolumeStore* vs = new VolumePreference::VolumeStore();
    // mVolumeStore = vs;
}

ECode VolumePreferenceSavedState::constructor()
{
    PreferenceBaseSavedState::constructor();
    // mVolumeStore.volume = source.readInt();
    // mVolumeStore.originalVolume = source.readInt();
    return NOERROR;
}

ECode VolumePreferenceSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return PreferenceBaseSavedState::constructor(superState);
}

CAR_INTERFACE_IMPL(VolumePreferenceSavedState, PreferenceBaseSavedState, IVolumePreferenceSavedState)

ECode VolumePreferenceSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    PreferenceBaseSavedState::ReadFromParcel(source);
    // source->ReadInt32(&mVolumeStore->mVolume);
    // source->ReadInt32(&mVolumeStore->mOriginalVolume);
    return NOERROR;
}

ECode VolumePreferenceSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    // dest->WriteInt32(mVolumeStore->mVolume);
    // dest->WriteInt32(mVolumeStore->mOriginalVolume);
    return NOERROR;
}

ECode VolumePreferenceSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    return PreferenceBaseSavedState::GetSuperState(superState);
}

ECode VolumePreferenceSavedState::GetVolumeStore(
     /* [out] */ IVolumePreferenceVolumeStore** vs)
{
    VALIDATE_NOT_NULL(vs)
    *vs = mVolumeStore;
    REFCOUNT_ADD(*vs)
    return NOERROR;
}

ECode VolumePreferenceSavedState::SetVolumeStore(
     /* [in] */ IVolumePreferenceVolumeStore* vs)
{
    mVolumeStore = vs;
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
