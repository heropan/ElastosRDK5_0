
#include "elastos/droid/internal/utility/CFastMath.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

CAR_INTERFACE_IMPL(CFastMath, Singleton, IFastMath)

CAR_SINGLETON_IMPL(CFastMath)

Int32 CFastMath::Round(
    /* [in] */ Float value)
{
    Int64 lx = (Int64) (value * (65536 * 256.0f));
    return (Int32) ((lx + 0x800000) >> 24);
}

ECode CFastMath::Round(
    /* [in] */ Float value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = Round(value);
    return NOERROR;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
