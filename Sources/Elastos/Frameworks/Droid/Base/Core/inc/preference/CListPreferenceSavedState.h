
#ifndef __ELASTOS_DROID_PREFERENCE_CLISTPREFERENCESAVEDSTATE_H__
#define __ELASTOS_DROID_PREFERENCE_CLISTPREFERENCESAVEDSTATE_H__

#include "_CListPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CListPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CARAPI GetValue(
        /* [out] */ String* text);

    CARAPI SetValue(
        /* [in] */ const String& text);

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
    String mValue;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CLISTPREFERENCESAVEDSTATE_H__
