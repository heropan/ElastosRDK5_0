
#ifndef __CTWOSTATEPREFERENCESAVEDSTATE_H__
#define __CTWOSTATEPREFERENCESAVEDSTATE_H__

#include "_CTwoStatePreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CTwoStatePreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CTwoStatePreferenceSavedState();

    CARAPI IsChecked(
        /* [out] */ Boolean* isChecked);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

private:
    Boolean mChecked;
};

}
}
}

#endif // __CTWOSTATEPREFERENCESAVEDSTATE_H__
