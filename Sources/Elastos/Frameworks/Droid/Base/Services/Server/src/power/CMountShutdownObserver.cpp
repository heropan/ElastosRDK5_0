
#include "ext/frameworkdef.h"
#include "power/CMountShutdownObserver.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

ECode CMountShutdownObserver::OnShutDownComplete(
    /* [in] */ Int32 statusCode)
{
    Slogger::W(String("ShutdownThread"), "Result code %d from MountService.shutdown", statusCode);
    mHost->ActionDone();
    return NOERROR;
}

ECode CMountShutdownObserver::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (ShutdownThread*)host;
    return NOERROR;
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
