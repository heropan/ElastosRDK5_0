#include "CAsynchronousCloseMonitor.h"
#include "AsynchronousCloseMonitorNative.h"

namespace Libcore {
namespace IO {

CAR_SINGLETON_IMPL(CAsynchronousCloseMonitor)

CAR_INTERFACE_IMPL(CAsynchronousCloseMonitor, Singleton, IAsynchronousCloseMonitor)

ECode CAsynchronousCloseMonitor::SignalBlockedThreads(
    /* [in] */ IFileDescriptor* fd)
{
    Int32 iFd;
    fd->GetDescriptor(&iFd);
    AsynchronousCloseMonitorNative::SignalBlockedThreads(iFd);
    return NOERROR;
}

} // namespace IO
} // namespace Libcore