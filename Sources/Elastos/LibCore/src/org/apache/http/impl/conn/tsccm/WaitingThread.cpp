
#include "WaitingThread.h"
#include "core/Thread.h"
#include "Logger.h"

using Elastos::Core::Thread;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Conn {
namespace Tsccm {

WaitingThread::WaitingThread(
    /* [in] */ ICondition* cond,
    /* [in] */ RouteSpecificPool* pool)
    : mAborted(FALSE)
{
    if (cond == NULL) {
        Logger::E("WaitingThread", "Condition must not be null.");
        assert(0);
        // return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mCond = cond;
    mPool = pool;
}

AutoPtr<ICondition> WaitingThread::GetCondition()
{
    return mCond;
}

AutoPtr<RouteSpecificPool> WaitingThread::GetPool()
{
    return mPool;
}

AutoPtr<IThread> WaitingThread::GetThread()
{
    return mWaiter;
}

ECode WaitingThread::Await(
    /* [in] */ IDate* deadline,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    // This is only a sanity check. We cannot synchronize here,
    // the lock would not be released on calling cond.await() below.
    if (mWaiter != NULL) {
        Logger::E("WaitingThread", "A thread is already waiting on this object.\ncaller: %p\nwaiter: %p"
                , Thread::GetCurrentThread().Get(), mWaiter.Get());
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mAborted) {
        Logger::E("WaitingThread", "Operation interrupted");
        return E_INTERRUPTED_EXCEPTION;
    }

    mWaiter = Thread::GetCurrentThread();

    Boolean success = FALSE;
    // try {
    if (deadline != NULL) {
        mCond->AwaitUntil(deadline, &success);
    }
    else {
        mCond->Await();
        success = TRUE;
    }
    if (mAborted) {
        Logger::E("WaitingThread", "Operation interrupted");
        return E_INTERRUPTED_EXCEPTION;
    }
    // } finally {
    //     this.waiter = null;
    // }
    mWaiter = NULL;
    *result = success;
    return NOERROR;
}

ECode WaitingThread::Wakeup()
{
    // If external synchronization and pooling works properly,
    // this cannot happen. Just a sanity check.
    if (mWaiter == NULL) {
        Logger::E("WaitingThread", "Nobody waiting on this object.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // One condition might be shared by several WaitingThread instances.
    // It probably isn't, but just in case: wake all, not just one.
    mCond->SignalAll();
    return NOERROR;
}

void WaitingThread::Interrupt()
{
    mAborted = TRUE;
    mCond->SignalAll();
}

} // namespace Tsccm
} // namespace Conn
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org