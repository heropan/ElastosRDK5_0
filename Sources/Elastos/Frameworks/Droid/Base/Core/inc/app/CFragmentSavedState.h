
#ifndef __ELASTOS_DROID_APP_CFRAGMENTSAVEDSTATE_H__
#define __ELASTOS_DROID_APP_CFRAGMENTSAVEDSTATE_H__

#include "_CFragmentSavedState.h"

using Elastos::Core::IClassLoader;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CFragmentSavedState)
{
public:
    CFragmentSavedState()
        : mState(NULL)
    {}

    CARAPI GetState(
        /* [out] */ IBundle** state);

    CARAPI SetState(
        /* [in] */ IBundle* state);

    CARAPI constructor(
        /* [in] */ IBundle* state);

    CARAPI constructor(
        /* [in] */ IParcel* in,
        /* [in] */ IClassLoader* loader);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    AutoPtr<IBundle> mState;
    AutoPtr<IClassLoader> mLoader;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CFRAGMENTSAVEDSTATE_H__
