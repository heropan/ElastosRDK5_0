
#ifndef __ELASTOS_DROID_PREFERENCE_CMULTISELECTLISTPREFERENCESAVEDSTATE_H__
#define  __ELASTOS_DROID_PREFERENCE_CMULTISELECTLISTPREFERENCESAVEDSTATE_H__

#include "_Elastos_Droid_Preference_CMultiSelectListPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

using Elastos::Utility::ISet;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CMultiSelectListPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI GetValues(
        /* [out] */ ISet** values);

    CARAPI SetValues(
        /* [in] */ ISet* values);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

private:
    AutoPtr<ISet> mValues;
};

} // Preference
} // Droid
} // Elastos

#endif  // __ELASTOS_DROID_PREFERENCE_CMULTISELECTLISTPREFERENCESAVEDSTATE_H__
