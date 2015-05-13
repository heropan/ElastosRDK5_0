
#ifndef __CPREFERENCEBASESAVEDSTATE_H__
#define __CPREFERENCEBASESAVEDSTATE_H__

#include "_CPreferenceBaseSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreferenceBaseSavedState), public PreferenceBaseSavedState
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);
};

}
}
}

#endif // __CPREFERENCEBASESAVEDSTATE_H__
