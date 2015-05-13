
#include "content/CSyncContext.h"
#include "os/SystemClock.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Content {

const Int64 CSyncContext::HEARTBEAT_SEND_INTERVAL_IN_MS;

CSyncContext::CSyncContext()
    : mSyncContext(NULL)
    , mLastHeartbeatSendTime(0)
{}

CSyncContext::~CSyncContext()
{}

ECode CSyncContext::SetStatusText(
    /* [in] */ const String& message)
{
    return UpdateHeartbeat();
}

ECode CSyncContext::OnFinished(
    /* [in] */ ISyncResult* result)
{
    if (NULL != mSyncContext) {
        FAIL_RETURN(mSyncContext->OnFinished(result))
    }

    return NOERROR;
}

ECode CSyncContext::GetSyncContextBinder(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder)
    if (NULL == mSyncContext) {
        *binder = NULL;
    }
    else {
//        *binder = mSyncContext.asBinder();
    }

    return NOERROR;
}

ECode CSyncContext::constructor(
    /* [in] */ IISyncContext* syncContextInterface)
{
    mSyncContext = syncContextInterface;
    mLastHeartbeatSendTime = 0;
    return NOERROR;
}

ECode CSyncContext::UpdateHeartbeat()
{
    Int64 now = SystemClock::GetElapsedRealtime();
    if (now < mLastHeartbeatSendTime + HEARTBEAT_SEND_INTERVAL_IN_MS) return NOERROR;

    mLastHeartbeatSendTime = now;
    if (NULL != mSyncContext) {
        FAIL_RETURN(mSyncContext->SendHeartbeat())
    }

    return NOERROR;
}

}
}
}

