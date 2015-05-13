
#ifndef __CACTIVITYINFOHELPER_H__
#define __CACTIVITYINFOHELPER_H__

#include "_CActivityInfoHelper.h"

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

#endif // __CACTIVITYINFOHELPER_H__
