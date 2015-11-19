
#ifndef __ELASTOS_DROID_PREFERENCE_PREFERENCEBASESAVEDSTATE_H__
#define __ELASTOS_DROID_PREFERENCE_PREFERENCEBASESAVEDSTATE_H__

#include "elastos/droid/ext/frameworkdef.h"
#include <elastos/coredef.h>
#include <elastos/core/Object.h>
// #include "elastos/droid/view/AbsSavedState.h"

// using Elastos::Droid::View::AbsSavedState;

namespace Elastos {
namespace Droid {
namespace Preference {

// class PreferenceBaseSavedState
//     : public AbsSavedState
//     , public IPreferenceBaseSavedState
class PreferenceBaseSavedState
    : public Object
    // : public AbsSavedState
    , public IPreferenceBaseSavedState
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    PreferenceBaseSavedState();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    virtual CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);
};

} // Preference
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_PREFERENCE_PREFERENCEBASESAVEDSTATE_H__
