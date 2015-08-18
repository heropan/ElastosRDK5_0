#include "_Elastos_Droid_Os_CCancellationSignalHelper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CCancellationSignalHelper)
{
public:
    CARAPI CreateTransport(
        /* [out] */ IICancellationSignal** cancellationSignal);

    CARAPI FromTransport(
        /* [in] */ IICancellationSignal* transport,
        /* [out] */ ICancellationSignal** cancellationSignal);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos
