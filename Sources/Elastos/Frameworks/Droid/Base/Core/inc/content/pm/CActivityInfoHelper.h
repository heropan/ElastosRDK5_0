
#ifndef __ELASTOS_DROID_CONTENT_PM_CACTIVITYINFOHELPER_H__
#define __ELASTOS_DROID_CONTENT_PM_CACTIVITYINFOHELPER_H__

#include "_Elastos_Droid_Content_Pm_CActivityInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CActivityInfoHelper)
{
public:
    CARAPI GetCONFIG_NATIVE_BITS(
        /* [out, callee] */ ArrayOf<Int32>** bits);

    /** @hide
     * Convert Java change bits to native.
     */
    CARAPI ActivityInfoConfigToNative(
        /* [in] */ Int32 input,
        /* [out] */ Int32* native);
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CACTIVITYINFOHELPER_H__
