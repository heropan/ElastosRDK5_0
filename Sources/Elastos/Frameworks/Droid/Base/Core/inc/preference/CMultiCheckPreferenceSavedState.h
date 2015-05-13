
#ifndef  __CMULTICHECKPREFERENCESAVEDSTATE_H__
#define  __CMULTICHECKPREFERENCESAVEDSTATE_H__

#include "_CMultiCheckPreferenceSavedState.h"
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

#endif  // __CMULTICHECKPREFERENCESAVEDSTATE_H__
