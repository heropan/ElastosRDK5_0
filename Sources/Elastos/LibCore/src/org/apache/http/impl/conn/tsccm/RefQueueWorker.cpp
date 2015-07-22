
#include "RefQueueWorker.h"
#include <elastos/Logger.h>
#include <elastos/core/Thread.h>

using Elastos::Core::Thread;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Conn {
namespace Tsccm {

RefQueueWorker::RefQueueWorker(
    /* [in] */ IList* queue,
    /* [in] */ IRefQueueHandler* handler)
{
    if (queue == NULL) {
        Logger::E("RefQueueWorker", "Queue must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (handler == NULL) {
        Logger::E("RefQueueWorker", "Handler must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mRefQueue   = queue;
    mRefHandler = handler;
}

CAR_INTERFACE_IMPL(RefQueueWorker, Object, IRunnable)

ECode RefQueueWorker::Run()
{
    if (mWorkerThread == NULL) {
        mWorkerThread = Thread::GetCurrentThread();
    }

    while (mWorkerThread == Thread::GetCurrentThread()) {
        // try {
        // remove the next reference and process it
        AutoPtr<IInterface> value;
        mRefQueue->Remove((IInterface**)&value);
        mRefHandler->HandleReference(ref);
        // } catch (InterruptedException e) {
        //     //@@@ is logging really necessary? this here is the
        //     //@@@ only reason for having a log in this class
        //     if (log.isDebugEnabled()) {
        //         log.debug(this.toString() + " interrupted", e);
        //     }
        // }
    }
}

void RefQueueWorker::Shutdown()
{
    AutoPtr<IThread> wt = mWorkerThread;
    if (wt != NULL) {
        mWorkerThread = NULL; // indicate shutdown
        wt->Interrupt();
    }
}

ECode RefQueueWorker::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    String s;
    IObject::Probe(mWorkerThread)->ToString(&s);
    *str = String("RefQueueWorker::") + s;
    return NOERROR;
}

} // namespace Tsccm
} // namespace Conn
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org