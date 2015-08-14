

#ifndef __ELASTOS_DROID_PREFERENCE_CSEEKBARPREFERENCESAVEDSATAE_H__
#define  __ELASTOS_DROID_PREFERENCE_CSEEKBARPREFERENCESAVEDSATAE_H__

#include "_CSeekBarPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CSeekBarPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CSeekBarPreferenceSavedState();

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI GetMax(
        /* [out] */ Int32* max);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

private:
    Int32 mProgress;
    Int32 mMax;
};

} // Preference
} // Droid
} // Elastos

#endif  // __ELASTOS_DROID_PREFERENCE_CSEEKBARPREFERENCESAVEDSATAE_H__
