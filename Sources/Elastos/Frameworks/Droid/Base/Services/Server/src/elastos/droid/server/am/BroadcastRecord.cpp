
#include "elastos/droid/server/am/BroadcastRecord.h"
#include <elastos/core/StringBuilder.h>

#include <elastos/utility/logging/Slogger.h>
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const Int32 BroadcastRecord::IDLE;
const Int32 BroadcastRecord::APP_RECEIVE;
const Int32 BroadcastRecord::CALL_IN_RECEIVE;
const Int32 BroadcastRecord::CALL_DONE_RECEIVE;

BroadcastRecord::BroadcastRecord(
    /* [in] */ BroadcastQueue* queue,
    /* [in] */ IIntent* intent,
    /* [in] */ ProcessRecord* callerApp,
    /* [in] */ const String& callerPackage,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ const String& requiredPermission,
    /* [in] */ BroadcastFilterList* receivers,
    /* [in] */ IIntentReceiver* resultTo,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Boolean serialized,
    /* [in] */ Boolean sticky,
    /* [in] */ Boolean initialSticky,
    /* [in] */ Int32 userId)
    : mIntent(intent)
    , mCallerApp(callerApp)
    , mCallerPackage(callerPackage)
    , mCallingPid(callingPid)
    , mCallingUid(callingUid)
    , mOrdered(serialized)
    , mSticky(sticky)
    , mInitialSticky(initialSticky)
    , mUserId(userId)
    , mRequiredPermission(requiredPermission)
    , mReceivers(receivers)
    , mResultTo(resultTo)
    , mDispatchTime(0)
    , mDispatchClockTime(0)
    , mReceiverTime(0)
    , mFinishTime(0)
    , mResultCode(resultCode)
    , mResultData(resultData)
    , mResultExtras(resultExtras)
    , mResultAbort(FALSE)
    , mNextReceiver(0)
    , mState(IDLE)
    , mAnrCount(0)
    , mQueue(queue)
{
}

BroadcastRecord::~BroadcastRecord()
{
    mReceivers = NULL;
    mResultTo = NULL;
    mReceiver = NULL;
    mCallerApp = NULL;
    mCurFilter = NULL;
    mCurApp = NULL;
    mCurComponent = NULL;
    mCurReceiver = NULL;
}

String BroadcastRecord::ToString()
{
    StringBuilder sb("BroadcastRecord{");
    sb += (Int32)this; // Integer.toHexString(System.identityHashCode(this));
    sb += " u";
    sb += mUserId;
    sb += " ";
    String action;
    mIntent->ToString(&action);
    sb += action;
    sb += "}";
    return sb.ToString();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
