
#ifndef __ELASTOS_DROID_OS_CPOWERMANAGERHELPER_H__
#define __ELASTOS_DROID_OS_CPOWERMANAGERHELPER_H__

#include "_CPowerManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CPowerManagerHelper)
{
public:
    /**
     * Returns true if the screen auto-brightness adjustment setting should
     * be available in the UI.  This setting is experimental and disabled by default.
     * @hide
     */
    CARAPI UseScreenAutoBrightnessAdjustmentFeature(
        /* [out] */ Boolean* result);

    /**
     * Returns true if the twilight service should be used to adjust screen brightness
     * policy.  This setting is experimental and disabled by default.
     * @hide
     */
    CARAPI UseTwilightAdjustmentFeature(
        /* [out] */ Boolean* result);

    CARAPI ValidateWakeLockParameters(
        /* [in] */ Int32 levelAndFlags,
        /* [in] */ const String& tag);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CPOWERMANAGERHELPER_H__
