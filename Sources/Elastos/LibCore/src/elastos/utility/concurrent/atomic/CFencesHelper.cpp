
#include "CFencesHelper.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

Int32 CFencesHelper::sTheVolatile = 0;

ECode CFencesHelper::OrderReads(
    /* [in] */ IInterface* ref,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    Int32 ignore = sTheVolatile;
    *outface = ref;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CFencesHelper::OrderWrites(
    /* [in] */ IInterface* ref,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    sTheVolatile = 0;
    *outface = ref;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CFencesHelper::OrderAccesses(
    /* [in] */ IInterface* ref,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    sTheVolatile = 0;
    *outface = ref;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CFencesHelper::ReachabilityFence(
    /* [in] */ IInterface* ref)
{
    if (ref != NULL) {
        assert(0 && "TODO");
        // synchronized (ref) {}
    }
    return NOERROR;
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
