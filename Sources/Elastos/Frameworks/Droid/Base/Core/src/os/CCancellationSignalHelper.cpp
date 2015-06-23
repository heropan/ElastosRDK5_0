#include "os/CCancellationSignalHelper.h"
#include "os/CCancellationSignal.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CCancellationSignalHelper::CreateTransport(
    /* [out] */ IICancellationSignal** cancellationSignal)
{
    VALIDATE_NOT_NULL(cancellationSignal);
    AutoPtr<IICancellationSignal> transport = CCancellationSignal::CreateTransport();
    *cancellationSignal = transport.Get();
    REFCOUNT_ADD(*cancellationSignal);
    return NOERROR;
}

ECode CCancellationSignalHelper::FromTransport(
    /* [in] */ IICancellationSignal* transport,
    /* [out] */ ICancellationSignal** cancellationSignal)
{
    VALIDATE_NOT_NULL(cancellationSignal)
    AutoPtr<ICancellationSignal> value = CCancellationSignal::FromTransport(transport);
    *cancellationSignal = value;
    REFCOUNT_ADD(*cancellationSignal)
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
