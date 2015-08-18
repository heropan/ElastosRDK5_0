#ifndef __ELASTOS_DROID_OS_CSYSTEMVIBRATOR_H__
#define __ELASTOS_DROID_OS_CSYSTEMVIBRATOR_H__

#include "_Elastos_Droid_Os_CSystemVibrator.h"
#include "ext/frameworkdef.h"

using Elastos::Droid::Os::IVibratorService;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CSystemVibrator)
{
public:
    CSystemVibrator();

    CARAPI constructor();

    //@Override
    CARAPI HasVibrator(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Vibrate(
        /* [in] */ Int64 milliseconds);

    //@Override
    CARAPI Vibrate(
        /* [in] */ const ArrayOf<Int64>& pattern,
        /* [in] */ Int32 repeat);

    //@Override
    CARAPI Cancel();

private:
    static const String TAG;

    AutoPtr<IVibratorService> mService;
    AutoPtr<IBinder> mToken;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CSYSTEMVIBRATOR_H__
