#include "elastos/droid/content/BroadcastReceiver.h"
//#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/CBundle.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Os::IBaseBundle;
using Elastos::Droid::Os::CBundle;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Content {

//==============================================================================
// BroadcastReceiver::PendingResult
//==============================================================================
CAR_INTERFACE_IMPL(BroadcastReceiver::PendingResult, Object, IPendingResult)

const Int32 BroadcastReceiver::PendingResult::TYPE_COMPONENT;
const Int32 BroadcastReceiver::PendingResult::TYPE_REGISTERED;
const Int32 BroadcastReceiver::PendingResult::TYPE_UNREGISTERED;

BroadcastReceiver::PendingResult::PendingResult()
    : mType(0)
    , mOrderedHint(FALSE)
    , mInitialStickyHint(FALSE)
    , mToken(NULL)
    , mSendingUser(0)
    , mResultCode(0)
    , mResultExtras(NULL)
    , mAbortBroadcast(FALSE)
    , mFinished(FALSE)
{}

BroadcastReceiver::PendingResult::PendingResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Int32 type,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 userId)
    : mType(type)
    , mOrderedHint(ordered)
    , mInitialStickyHint(sticky)
    , mToken(token)
    , mSendingUser(userId)
    , mResultCode(resultCode)
    , mResultData(resultData)
    , mResultExtras(resultExtras)
    , mAbortBroadcast(FALSE)
    , mFinished(FALSE)
{}

BroadcastReceiver::PendingResult::~PendingResult()
{}

ECode BroadcastReceiver::PendingResult::constructor(
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Int32 type,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 userId)
{
    mResultCode = resultCode;
    mResultData = resultData;
    mResultExtras = resultExtras;
    mType = type;
    mOrderedHint = ordered;
    mInitialStickyHint = sticky;
    mToken = token;
    mSendingUser = userId;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::SetResultCode(
    /* [in] */ Int32 code)
{
    FAIL_RETURN(CheckSynchronousHint());
    mResultCode = code;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    VALIDATE_NOT_NULL(resultCode);
    *resultCode = mResultCode;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::SetResultData(
    /* [in] */ const String& data)
{
    FAIL_RETURN(CheckSynchronousHint());
    mResultData = data;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::GetResultData(
    /* [out] */ String* resultData)
{
    VALIDATE_NOT_NULL(resultData);
    *resultData = mResultData;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(CheckSynchronousHint());
    mResultExtras = extras;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** resultExtras)
{
    VALIDATE_NOT_NULL(resultExtras);
    *resultExtras = mResultExtras;
    REFCOUNT_ADD(*resultExtras)

    if (!makeMap) {
        return NOERROR;
    }

    if (NULL == *resultExtras) {
        FAIL_RETURN(CBundle::New(resultExtras))
        mResultExtras = *resultExtras;
    }

    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(CheckSynchronousHint());
    mResultCode = code;
    mResultData = data;
    mResultExtras = extras;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    VALIDATE_NOT_NULL(isAborted);
    *isAborted = mAbortBroadcast;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::AbortBroadcast()
{
    FAIL_RETURN(CheckSynchronousHint());
    mAbortBroadcast = TRUE;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::ClearAbortBroadcast()
{
    mAbortBroadcast = FALSE;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::Finish()
{
    assert(0 && "TODO");
    if (mType == TYPE_COMPONENT) {
        // AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
        // if (QueuedWork.hasPendingWork()) {
        //     // If this is a broadcast component, we need to make sure any
        //     // queued work is complete before telling AM we are done, so
        //     // we don't have our process killed before that.  We now know
        //     // there is pending work; put another piece of work at the end
        //     // of the list to finish the broadcast, so we don't block this
        //     // thread (which may be the main thread) to have it finished.
        //     //
        //     // Note that we don't need to use QueuedWork.add() with the
        //     // runnable, since we know the AM is waiting for us until the
        //     // executor gets to it.
        //     QueuedWork.singleThreadExecutor().execute( new Runnable() {
        //         @Override public void run() {
        //             if (ActivityThread.DEBUG_BROADCAST) Slog.i(ActivityThread.TAG,
        //                     "Finishing broadcast after work to component " + mToken);
        //             sendFinished(mgr);
        //         }
        //     });
        // } else {
        // if (ActivityThread::DEBUG_BROADCAST) {
        //     Slogger::I("BroadcastReceiver::PendingResult", "Finishing broadcast to component %p", mToken.Get());
        // }
        // SendFinished(mgr);
        // }
    }
    else if (mOrderedHint && mType != TYPE_UNREGISTERED) {
        // if (ActivityThread::DEBUG_BROADCAST) {
        //     Slogger::I("BroadcastReceiver::PendingResult", "Finishing broadcast to %p", mToken.Get());
        // }
        // AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
        // SendFinished(mgr);
    }
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::SetExtrasClassLoader(
    /* [in] */ IClassLoader* cl)
{
    if (NULL != mResultExtras) {
        FAIL_RETURN(IBaseBundle::Probe(mResultExtras)->SetClassLoader(cl))
    }
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::SendFinished(
    /* [in] */ IIActivityManager* am)
{
    AutoLock lock(mPendingResultLock);
    if (mFinished) return E_ILLEGAL_STATE_EXCEPTION;
    mFinished = TRUE;

    if (NULL != mResultExtras) {
        Boolean result = FALSE;
        FAIL_RETURN(mResultExtras->SetAllowFds(FALSE, &result));
    }

    VALIDATE_NOT_NULL(am);

    ECode ec = NOERROR;
    if (mOrderedHint) {
        ec = am->FinishReceiver(mToken, mResultCode, mResultData, mResultExtras, mAbortBroadcast);
    }
    else {
        // This broadcast was sent to a component; it is not ordered,
        // but we still need to tell the activity manager we are done.
        ec = am->FinishReceiver(mToken, 0, String(NULL), (IBundle*)NULL, FALSE);
    }

    return ec;
}

ECode BroadcastReceiver::PendingResult::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    VALIDATE_NOT_NULL(userId);
    *userId = mSendingUser;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::GetOrderedHint(
    /* [out] */ Boolean* orderedHint)
{
    VALIDATE_NOT_NULL(orderedHint);
    *orderedHint = mOrderedHint;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::GetInitialStickyHint(
    /* [out] */ Boolean* initialStickyHint)
{
    VALIDATE_NOT_NULL(initialStickyHint);
    *initialStickyHint = mInitialStickyHint;
    return NOERROR;
}

ECode BroadcastReceiver::PendingResult::CheckSynchronousHint()
{
    // Note that we don't assert when receiving the initial sticky value,
    // since that may have come from an ordered broadcast.  We'll catch
    // them later when the real broadcast happens again.
    if (mOrderedHint || mInitialStickyHint) return NOERROR;

//    RuntimeException e = new RuntimeException(
//            "BroadcastReceiver trying to return result during a non-ordered broadcast");
//    e.fillInStackTrace();
    Slogger::E("BroadcastReceiver", "E_RUNTIME_EXCEPTION BroadcastReceiver trying to return result during a non-ordered broadcast");
    return E_RUNTIME_EXCEPTION;
}

//==============================================================================
// BroadcastReceiver
//==============================================================================
CAR_INTERFACE_IMPL(BroadcastReceiver, Object, IBroadcastReceiver)

BroadcastReceiver::BroadcastReceiver()
    : mDebugUnregister(FALSE)
{}

BroadcastReceiver::~BroadcastReceiver()
{}

ECode BroadcastReceiver::constructor()
{
    return NOERROR;
}

ECode BroadcastReceiver::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    String str("BroadcastReceiver:(");
    str.AppendFormat("%p", THIS_PROBE(IBroadcastReceiver));
    *info = str;
    return NOERROR;
}

ECode BroadcastReceiver::GoAsync(
    /* [out] */ IPendingResult** pendingResult)
{
    VALIDATE_NOT_NULL(pendingResult);
    *pendingResult = mPendingResult;
    REFCOUNT_ADD(*pendingResult);
    mPendingResult = NULL;
    return NOERROR;
}

ECode BroadcastReceiver::PeekService(
    /* [in] */ IContext* myContext,
    /* [in] */ IIntent* service,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder)
    *binder = NULL;

    assert(0 && "TODO");
    AutoPtr<IIActivityManager> am ;//= ActivityManagerNative::GetDefault();
    AutoPtr<IContentResolver> resolver;
    String type;

    FAIL_RETURN(service->PrepareToLeaveProcess())
    FAIL_RETURN(myContext->GetContentResolver((IContentResolver**)&resolver))
    FAIL_RETURN(service->ResolveTypeIfNeeded(resolver, &type))
    return am->PeekService(service, type, binder);
}

ECode BroadcastReceiver::SetResultCode(
    /* [in] */ Int32 code)
{
    FAIL_RETURN(CheckSynchronousHint());
    FAIL_RETURN(mPendingResult->SetResultCode(code));
    return NOERROR;
}

ECode BroadcastReceiver::GetResultCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);

    if (NULL != mPendingResult) {
        FAIL_RETURN(mPendingResult->GetResultCode(code));
    }
    else {
        *code = 0;
    }

    return NOERROR;
}

ECode BroadcastReceiver::SetResultData(
    /* [in] */ const String& data)
{
    FAIL_RETURN(CheckSynchronousHint());
    FAIL_RETURN(mPendingResult->SetResultData(data));
    return NOERROR;
}

ECode BroadcastReceiver::GetResultData(
    /* [out] */ String* data)
{
    VALIDATE_NOT_NULL(data);

    if (NULL != mPendingResult) {
        FAIL_RETURN(mPendingResult->GetResultData(data));
    }
    else {
        *data = NULL;
    }

    return NOERROR;
}

ECode BroadcastReceiver::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(CheckSynchronousHint());
    FAIL_RETURN(mPendingResult->SetResultExtras(extras));
    return NOERROR;
}

ECode BroadcastReceiver::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = NULL;

    if (NULL == mPendingResult) {
        return NOERROR;
    }

    FAIL_RETURN(mPendingResult->GetResultExtras(FALSE, extras));
    if (!makeMap) return NOERROR;

    if (NULL == *extras) {
        FAIL_RETURN(CBundle::New(extras));
        FAIL_RETURN(mPendingResult->SetResultExtras(*extras));
    }

    return NOERROR;
}

ECode BroadcastReceiver::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(CheckSynchronousHint());
    FAIL_RETURN(mPendingResult->SetResultCode(code));
    FAIL_RETURN(mPendingResult->SetResultData(data));
    FAIL_RETURN(mPendingResult->SetResultExtras(extras));
    return NOERROR;
}

ECode BroadcastReceiver::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    VALIDATE_NOT_NULL(isAborted);

    if (NULL != mPendingResult) {
        FAIL_RETURN(mPendingResult->GetAbortBroadcast(isAborted));
    }
    else {
        *isAborted = FALSE;
    }

    return NOERROR;
}

ECode BroadcastReceiver::AbortBroadcast()
{
    FAIL_RETURN(mPendingResult->AbortBroadcast());
    return NOERROR;
}

ECode BroadcastReceiver::ClearAbortBroadcast()
{
    if (NULL != mPendingResult) FAIL_RETURN(mPendingResult->ClearAbortBroadcast());
    return NOERROR;
}

ECode BroadcastReceiver::IsOrderedBroadcast(
    /* [out] */ Boolean* isOrdered)
{
    VALIDATE_NOT_NULL(isOrdered);

    if (NULL != mPendingResult) {
        FAIL_RETURN(mPendingResult->GetOrderedHint(isOrdered));
    }
    else {
        *isOrdered = FALSE;
    }

    return NOERROR;
}

ECode BroadcastReceiver::IsInitialStickyBroadcast(
    /* [out] */ Boolean* isInitial)
{
    VALIDATE_NOT_NULL(isInitial);

    if (NULL != mPendingResult) {
        FAIL_RETURN(mPendingResult->GetInitialStickyHint(isInitial));
    }
    else {
        *isInitial = FALSE;
    }

    return NOERROR;
}

ECode BroadcastReceiver::SetOrderedHint(
    /* [in] */ Boolean isOrdered)
{
    // Accidentally left in the SDK.
    return NOERROR;
}

ECode BroadcastReceiver::SetPendingResult(
    /* [in] */ IPendingResult* result)
{
    mPendingResult = result;
    return NOERROR;
}

ECode BroadcastReceiver::GetPendingResult(
    /* [out] */ IPendingResult** pendingResult)
{
    VALIDATE_NOT_NULL(pendingResult);
    *pendingResult = mPendingResult;
    REFCOUNT_ADD(*pendingResult);
    return NOERROR;
}

ECode BroadcastReceiver::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return mPendingResult->GetSendingUserId(userId);
}

ECode BroadcastReceiver::SetDebugUnregister(
    /* [in] */ Boolean debug)
{
    mDebugUnregister = debug;
    return NOERROR;
}

ECode BroadcastReceiver::GetDebugUnregister(
    /* [out] */ Boolean* debugUnregister)
{
    VALIDATE_NOT_NULL(debugUnregister);;
    *debugUnregister = mDebugUnregister;
    return NOERROR;
}

ECode BroadcastReceiver::CheckSynchronousHint()
{
    if (NULL == mPendingResult) return E_ILLEGAL_STATE_EXCEPTION;

    // Note that we don't assert when receiving the initial sticky value,
    // since that may have come from an ordered broadcast.  We'll catch
    // them later when the real broadcast happens again.
    Boolean isOrdered = FALSE;
    Boolean isInitial = FALSE;
    FAIL_RETURN(mPendingResult->GetOrderedHint(&isOrdered));
    FAIL_RETURN(mPendingResult->GetInitialStickyHint(&isInitial));
    if (isOrdered || isInitial) return NOERROR;

//    RuntimeException e = new RuntimeException(
//            "BroadcastReceiver trying to return result during a non-ordered broadcast");
//    e.fillInStackTrace();
    Slogger::E("BroadcastReceiver", String("E_RUNTIME_EXCEPTION BroadcastReceiver trying to return result during a non-ordered broadcast"));
    return E_RUNTIME_EXCEPTION;
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
