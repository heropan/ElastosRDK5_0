
#ifndef __CEDITTEXTPREFERENCESAVEDSTATE_H__
#define __CEDITTEXTPREFERENCESAVEDSTATE_H__

#include "_CEditTextPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CEditTextPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CARAPI GetText(
        /* [out] */ String* text);

    CARAPI SetText(
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
    String mText;
};

}
}
}

#endif // __CEDITTEXTPREFERENCESAVEDSTATE_H__
