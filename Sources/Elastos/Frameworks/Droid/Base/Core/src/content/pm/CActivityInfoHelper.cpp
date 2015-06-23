
#include "ext/frameworkext.h"
#include "content/pm/CActivityInfoHelper.h"
#include "content/pm/CActivityInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CActivityInfoHelper::GetCONFIG_NATIVE_BITS(
    /* [out, callee] */ ArrayOf<Int32>** bits)
{
    VALIDATE_NOT_NULL(bits);
    *bits = CActivityInfo::CONFIG_NATIVE_BITS;
    REFCOUNT_ADD(*bits);
    return NOERROR;
}

ECode CActivityInfoHelper::ActivityInfoConfigToNative(
    /* [in] */ Int32 input,
    /* [out] */ Int32* native)
{
    VALIDATE_NOT_NULL(native);

    *native = CActivityInfo::ActivityInfoConfigToNative(input);
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

