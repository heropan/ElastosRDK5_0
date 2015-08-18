
#ifndef __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCESAVEDSTATE_H__
#define  __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCESAVEDSTATE_H__

#include "_Elastos_Droid_Preference_CMultiCheckPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CMultiCheckPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI GetValues(
        /* [out, callee] */ ArrayOf<Boolean>** values);

    CARAPI SetValues(
        /* [in] */ ArrayOf<Boolean>* values);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

private:
    AutoPtr< ArrayOf<Boolean> > mValues;
};

} // Preference
} // Droid
} // Elastos

#endif  // __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCESAVEDSTATE_H__
