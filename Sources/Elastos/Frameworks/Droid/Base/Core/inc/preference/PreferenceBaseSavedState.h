
#ifndef __ELASTOS_DROID_PREFERENCE_PREFERENCEBASESAVEDSTATE_H__
#define __ELASTOS_DROID_PREFERENCE_PREFERENCEBASESAVEDSTATE_H__

#include "ext/frameworkdef.h"
#include "view/AbsSavedState.h"

using Elastos::Droid::View::AbsSavedState;

namespace Elastos {
namespace Droid {
namespace Preference {

class PreferenceBaseSavedState: public AbsSavedState
{
public:
    virtual ~PreferenceBaseSavedState() {}

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    virtual CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

protected:
    CARAPI Init(
        /* [in] */ IParcelable* superState);
};

} // Preference
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_PREFERENCE_PREFERENCEBASESAVEDSTATE_H__
