
#include "am/BroadcastQueue.h"
#include "am/BroadcastRecord.h"
#include "am/ProcessRecord.h"
#include "am/BroadcastFilter.h"
#include "os/SystemClock.h"
#include "os/UserHandle.h"
#include "os/Handler.h"
#include "app/AppGlobals.h"
#include "Manifest.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::EIID_IResolveInfo;
using Elastos::Droid::App::AppGlobals;
using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::IActivityManagerHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const String BroadcastQueue::TAG("BroadcastQueue");
const String BroadcastQueue::TAG_MU("BroadcastQueueMU");
const Boolean BroadcastQueue::DEBUG_BROADCAST = FALSE;
const Boolean BroadcastQueue::DEBUG_BROADCAST_LIGHT = FALSE;
const Boolean BroadcastQueue::DEBUG_MU = FALSE;
const Int32 BroadcastQueue::MAX_BROADCAST_HISTORY = 25;
const Int32 BroadcastQueue::MAX_BROADCAST_SUMMARY_HISTORY = 100;
const Int32 BroadcastQueue::BROADCAST_INTENT_MSG = 200;//ActivityManagerService.FIRST_BROADCAST_QUEUE_MSG;
const Int32 BroadcastQueue::BROADCAST_TIMEOUT_MSG = 201;//ActivityManagerService.FIRST_BROADCAST_QUEUE_MSG + 1;

ECode BroadcastQueue::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case BroadcastQueue::BROADCAST_INTENT_MSG:      // CActivityManagerService::FIRST_BROADCAST_QUEUE_MSG
            mHost->ProcessNextBroadcast(TRUE);
            break;
        case BroadcastQueue::BROADCAST_TIMEOUT_MSG: {   // (CActivityManagerService::FIRST_BROADCAST_QUEUE_MSG + 1)
            AutoLock lock(mHost->mService->mLock);
            mHost->BroadcastTimeoutLocked(TRUE);
            break;
        }
    }

    return NOERROR;
}

BroadcastQueue::AppNotResponding::AppNotResponding(
    /* [in] */ ProcessRecord* app,
    /* [in] */ const String& annotation,
    /* [in] */ BroadcastQueue* host)
    : mApp(app)
    , mAnnotation(annotation)
    , mHost(host)
{}

CAR_INTERFACE_IMPL(BroadcastQueue::AppNotResponding, IRunnable)

ECode BroadcastQueue::AppNotResponding::Run()
{
    ECode ec = mHost->mService->AppNotResponding(mApp, NULL, NULL, FALSE, mAnnotation);
    return ec;
}

CAR_INTERFACE_IMPL(BroadcastQueue, IInterface)

BroadcastQueue::BroadcastQueue(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ const String& name,
    /* [in] */ Int64 timeoutPeriod)
    : mService(service)
    , mQueueName(name)
    , mTimeoutPeriod(timeoutPeriod)
    , mBroadcastsScheduled(FALSE)
    , mPendingBroadcastTimeoutMessage(FALSE)
    , mPendingBroadcastRecvIndex(0)
{
    mBroadcastHistory = ArrayOf<BroadcastRecord*>::Alloc(MAX_BROADCAST_HISTORY);
    mBroadcastSummaryHistory = ArrayOf<IIntent*>::Alloc(MAX_BROADCAST_SUMMARY_HISTORY);

    mHandler = new MyHandler(this);
}

Boolean BroadcastQueue::IsPendingBroadcastProcessLocked(
    /* [in] */ Int32 pid)
{
    return mPendingBroadcast != NULL && mPendingBroadcast->mCurApp->mPid == pid;
}

void BroadcastQueue::EnqueueParallelBroadcastLocked(
    /* [in] */ BroadcastRecord* r)
{
    AutoPtr<BroadcastRecord> ar = r;
    mParallelBroadcasts.PushBack(ar);
}

void BroadcastQueue::EnqueueOrderedBroadcastLocked(
    /* [in] */ BroadcastRecord* r)
{
    AutoPtr<BroadcastRecord> ar = r;
    mOrderedBroadcasts.PushBack(ar);
}

Boolean BroadcastQueue::ReplaceParallelBroadcastLocked(
    /* [in] */ BroadcastRecord* r)
{
    List< AutoPtr<BroadcastRecord> >::ReverseIterator it = mParallelBroadcasts.RBegin();
    for(; it != mParallelBroadcasts.REnd(); ++it) {
        Boolean b;
        r->mIntent->FilterEquals((*it)->mIntent, &b);
        if (b) {
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, "***** DROPPING PARALLEL [%s]:%p", mQueueName.string(), r->mIntent.Get());
            }
            *it = r;
            return TRUE;
        }
    }
    return FALSE;
}

Boolean BroadcastQueue::ReplaceOrderedBroadcastLocked(
    /* [in] */ BroadcastRecord* r)
{
    List< AutoPtr<BroadcastRecord> >::ReverseIterator it = mOrderedBroadcasts.RBegin();
    for(; it != mOrderedBroadcasts.REnd(); ++it) {
        Boolean b;
        r->mIntent->FilterEquals((*it)->mIntent, &b);
        if (b) {
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, "***** DROPPING ORDERED [%s]:%p", mQueueName.string(), r->mIntent.Get());
            }
            *it = r;
            return TRUE;
        }
    }
    return FALSE;
}

ECode BroadcastQueue::ProcessCurBroadcastLocked(
    /* [in] */ BroadcastRecord* r,
    /* [in] */ ProcessRecord* app)
{
    if (DEBUG_BROADCAST) {
        String rs;
        if (r) {
            rs = r->ToString();
        }

        Slogger::V(TAG, "Process cur broadcast %s for app %p", rs.string(), app);
    }

    if (app->mThread == NULL) {
        // throw new RemoteException();
        return E_REMOTE_EXCEPTION;
    }
    r->mReceiver = IBinder::Probe(app->mThread.Get());
    r->mCurApp = app;
    app->mCurReceiver = r;
    mService->UpdateLruProcessLocked(app, TRUE);

    // Tell the application to launch this receiver.
    r->mIntent->SetComponent(r->mCurComponent);

    Boolean started = FALSE;
    // try {
    if (DEBUG_BROADCAST_LIGHT) Slogger::V(TAG, "Delivering to component %p: %p", r->mCurComponent.Get(), r);
    AutoPtr<IComponentName> name;
    r->mIntent->GetComponent((IComponentName**)&name);
    String pkgName;
    name->GetPackageName(&pkgName);
    mService->EnsurePackageDexOpt(pkgName);
    AutoPtr<IIntent> intent;
    CIntent::New(r->mIntent, (IIntent**)&intent);
    AutoPtr<IApplicationInfo> info;
    r->mCurReceiver->GetApplicationInfo((IApplicationInfo**)&info);
    if (FAILED(app->mThread->ScheduleReceiver(intent, r->mCurReceiver,
            mService->CompatibilityInfoForPackageLocked(info),
            r->mResultCode, r->mResultData, r->mResultExtras, r->mOrdered, r->mUserId))) {
        goto failed;
    }
    // if (DEBUG_BROADCAST)  Slog.v(TAG,
    //         "Process cur broadcast " + r + " DELIVERED for app " + app);
    started = TRUE;
    // } finally {
failed:
    if (!started) {
        // if (DEBUG_BROADCAST)  Slog.v(TAG,
        //         "Process cur broadcast " + r + ": NOT STARTED!");
        r->mReceiver = NULL;
        r->mCurApp = NULL;
        app->mCurReceiver = NULL;
    }
    // }
    return NOERROR;
}

ECode BroadcastQueue::SendPendingBroadcastsLocked(
    /* [in] */ ProcessRecord* app,
    /* [out] */ Boolean* result)
{
    AutoPtr<BroadcastRecord> br = mPendingBroadcast;
    if (br != NULL && br->mCurApp->mPid == app->mPid) {
        // try {
        mPendingBroadcast = NULL;
        ECode ec = ProcessCurBroadcastLocked(br, app);
        if(FAILED(ec)) {
            String str;
            br->mCurComponent->FlattenToShortString(&str);
            Slogger::W(TAG, "Exception in new application when starting receiver %s 0x%08x", str.string(), ec);
            LogBroadcastReceiverDiscardLocked(br);
            FinishReceiverLocked(br, br->mResultCode, br->mResultData,
                    br->mResultExtras, br->mResultAbort, TRUE);
            ScheduleBroadcastsLocked();
            // We need to reset the state if we failed to start the receiver.
            br->mState = BroadcastRecord::IDLE;
            // throw new RuntimeException(e.getMessage());
            *result = FALSE;
            return E_RUNTIME_EXCEPTION;
        }
    // }catch (Exception e) {
    // }
    }
    *result = TRUE;
    return NOERROR;
}

void BroadcastQueue::SkipPendingBroadcastLocked(
    /* [in] */ Int32 pid)
{
    AutoPtr<BroadcastRecord> br = mPendingBroadcast;
    if (br != NULL && br->mCurApp->mPid == pid) {
        br->mState = BroadcastRecord::IDLE;
        br->mNextReceiver = mPendingBroadcastRecvIndex;
        mPendingBroadcast = NULL;
        ScheduleBroadcastsLocked();
    }
}

void BroadcastQueue::SkipCurrentReceiverLocked(
    /* [in] */ ProcessRecord* app)
{
    Boolean reschedule = FALSE;
    AutoPtr<BroadcastRecord> r = app->mCurReceiver;
    if (r != NULL) {
        // The current broadcast is waiting for this app's receiver
        // to be finished.  Looks like that's not going to happen, so
        // let the broadcast continue.
        LogBroadcastReceiverDiscardLocked(r);
        FinishReceiverLocked(r, r->mResultCode, r->mResultData,
                r->mResultExtras, r->mResultAbort, TRUE);
        reschedule = TRUE;
    }

    r = mPendingBroadcast;
    if (r != NULL && r->mCurApp.Get() == app) {
        if (DEBUG_BROADCAST) Slogger::V(TAG, "[%s] skip & discard pending app %p", mQueueName.string(), r.Get());
        LogBroadcastReceiverDiscardLocked(r);
        FinishReceiverLocked(r, r->mResultCode, r->mResultData,
                r->mResultExtras, r->mResultAbort, TRUE);
        reschedule = TRUE;
    }
    if (reschedule) {
        ScheduleBroadcastsLocked();
    }
}

void BroadcastQueue::ScheduleBroadcastsLocked()
{
    if (DEBUG_BROADCAST) Slogger::V(TAG, "Schedule broadcasts [%s]: current=%d", mQueueName.string(), mBroadcastsScheduled);

    if (mBroadcastsScheduled) {
        return;
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(BROADCAST_INTENT_MSG, (IMessage**)&msg);
    msg->SetObj((IInterface*)this);
    Boolean result;
    mHandler->SendMessage(msg, &result);

    mBroadcastsScheduled = TRUE;
}

AutoPtr<BroadcastRecord> BroadcastQueue::GetMatchingOrderedReceiver(
    /* [in] */ IBinder* receiver)
{
    if (!mOrderedBroadcasts.IsEmpty()) {
        AutoPtr<BroadcastRecord> r = *mOrderedBroadcasts.Begin();
        if (r != NULL && r->mReceiver.Get() == receiver) {
            return r;
        }
    }
    return NULL;
}

Boolean BroadcastQueue::FinishReceiverLocked(
    /* [in] */ BroadcastRecord* r,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Boolean resultAbort,
    /* [in] */ Boolean _explicit)
{
    Int32 state = r->mState;
    r->mState = BroadcastRecord::IDLE;
    if (state == BroadcastRecord::IDLE) {
        if (_explicit) {
            Slogger::W(TAG, "finishReceiver [%s] called but state is IDLE", mQueueName.string());
        }
    }
    r->mReceiver = NULL;
    r->mIntent->SetComponent(NULL);
    if (r->mCurApp != NULL) {
        r->mCurApp->mCurReceiver = NULL;
    }
    if (r->mCurFilter != NULL) {
        r->mCurFilter->mReceiverList->mCurBroadcast = NULL;
    }
    r->mCurFilter = NULL;
    r->mCurApp = NULL;
    r->mCurComponent = NULL;
    r->mCurReceiver = NULL;
    mPendingBroadcast = NULL;

    r->mResultCode = resultCode;
    r->mResultData = resultData;
    r->mResultExtras = resultExtras;
    r->mResultAbort = resultAbort;

    // We will process the next receiver right now if this is finishing
    // an app receiver (which is always asynchronous) or after we have
    // come back from calling a receiver.
    return state == BroadcastRecord::APP_RECEIVE
            || state == BroadcastRecord::CALL_DONE_RECEIVE;
}

ECode BroadcastQueue::PerformReceiveLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ IIntentReceiver* receiver,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    // Send the intent to the receiver asynchronously using one-way binder calls.
    if (app != NULL && app->mThread != NULL) {
        // If we have an app thread, do the call through that so it is
        // correctly ordered with other one-way calls.
        return app->mThread->ScheduleRegisteredReceiver(receiver, intent, resultCode,
                data, extras, ordered, sticky, sendingUser);
    }
    else {
        return receiver->PerformReceive(intent, resultCode, data, extras, ordered,
                sticky, sendingUser);
    }
}

void BroadcastQueue::DeliverToRegisteredReceiverLocked(
    /* [in] */ BroadcastRecord* r,
    /* [in] */ BroadcastFilter* filter,
    /* [in] */ Boolean ordered)
{
    if (filter->mReceiverList == NULL) {
        if (ordered) {
            r->mReceiver = NULL;
            r->mCurFilter = NULL;
            r->mState = BroadcastRecord::CALL_IN_RECEIVE;
        }
        return;
    }

    if (DEBUG_BROADCAST) {
        String rs, fs;
        if (r) rs = r->ToString();
        if (filter) fs = filter->ToString();
        Slogger::W(TAG, "DeliverToRegisteredReceiverLocked: \n >>> record:[%s]\n >>> filter:[%s]",
            rs.string(), fs.string());
    }

    Boolean skip = FALSE;
    if (!filter->mRequiredPermission.IsNull()) {
        Int32 perm = mService->CheckComponentPermission(filter->mRequiredPermission,
                r->mCallingPid, r->mCallingUid, -1, TRUE);
        if (perm != IPackageManager::PERMISSION_GRANTED) {
            String str;
            r->mIntent->ToString(&str);
            Slogger::W(TAG, "Permission Denial: broadcasting %s from %s (pid=%d, uid=%d) requires %s due to registered receiver %p"
                    , str.string(), r->mCallerPackage.string(),  r->mCallingPid, r->mCallingUid
                    , filter->mRequiredPermission.string(), filter);
            skip = TRUE;
        }
    }
    if (!skip && !r->mRequiredPermission.IsNull()) {
        Int32 perm = mService->CheckComponentPermission(r->mRequiredPermission,
                filter->mReceiverList->mPid, filter->mReceiverList->mUid, -1, TRUE);
        if (perm != IPackageManager::PERMISSION_GRANTED) {
            String str;
            r->mIntent->ToString(&str);
            Slogger::W(TAG, "Permission Denial: receiving %s to %p (pid=%d, uid=%d) requires %s due to sender %s (uid %d)"
                    , str.string(), filter->mReceiverList->mApp.Get(),  filter->mReceiverList->mPid, filter->mReceiverList->mUid
                    , r->mRequiredPermission.string(), r->mCallerPackage.string(), r->mCallingUid);
            skip = TRUE;
        }
    }

    if (!skip) {
        // If this is not being sent as an ordered broadcast, then we
        // don't want to touch the fields that keep track of the current
        // state of ordered broadcasts.
        if (ordered) {
            r->mReceiver = IBinder::Probe(filter->mReceiverList->mReceiver.Get());
            r->mCurFilter = filter;
            filter->mReceiverList->mCurBroadcast = r;
            r->mState = BroadcastRecord::CALL_IN_RECEIVE;
            if (filter->mReceiverList->mApp != NULL) {
                // Bump hosting application to no longer be in background
                // scheduling class.  Note that we can't do that if there
                // isn't an app...  but we can only be in that case for
                // things that directly call the IActivityManager API, which
                // are already core system stuff so don't matter for this.
                r->mCurApp = filter->mReceiverList->mApp;
                filter->mReceiverList->mApp->mCurReceiver = r;
                mService->UpdateOomAdjLocked();
            }
        }

        // try {
        if (DEBUG_BROADCAST_LIGHT) {
            Int32 seq;
            r->mIntent->GetInt32Extra(String("seq"), -1, &seq);
            Slogger::I(TAG, "Delivering to %p (seq=%d): %p", filter, seq, r);
        }

        AutoPtr<IIntent> newIntent;
        CIntent::New(r->mIntent, (IIntent**)&newIntent);
        ECode ec = PerformReceiveLocked(filter->mReceiverList->mApp, filter->mReceiverList->mReceiver,
            newIntent, r->mResultCode, r->mResultData,
            r->mResultExtras, r->mOrdered, r->mInitialSticky, r->mUserId);
        if (FAILED(ec)) {
            Slogger::W(TAG, "Failure sending broadcast %p 0x%08x", r->mIntent.Get(), ec);
            if (ordered) {
                r->mReceiver = NULL;
                r->mCurFilter = NULL;
                filter->mReceiverList->mCurBroadcast = NULL;
                if (filter->mReceiverList->mApp != NULL) {
                    filter->mReceiverList->mApp->mCurReceiver = NULL;
                }
            }
            return;
        }

        if (ordered) {
            r->mState = BroadcastRecord::CALL_DONE_RECEIVE;
        }
        return;
        // } catch (RemoteException e) {
        //     Slog.w(TAG, "Failure sending broadcast " + r->mIntent, e);
        //     if (ordered) {
        //         r->mReceiver = NULL;
        //         r->mCurFilter = NULL;
        //         filter->mReceiverList.curBroadcast = NULL;
        //         if (filter->mReceiverList.app != NULL) {
        //             filter->mReceiverList.app.curReceiver = NULL;
        //         }
        //     }
        // }
    }
}

ECode BroadcastQueue::ProcessNextBroadcast(
    /* [in] */ Boolean fromMsg)
{
    AutoLock lock(mService->mLock);
    AutoPtr<BroadcastRecord> r;
    if (DEBUG_BROADCAST) {
        Slogger::V(TAG, "processNextBroadcast [%s]: %d broadcasts, %d ordered broadcasts"
                , mQueueName.string(), mParallelBroadcasts.GetSize(), mOrderedBroadcasts.GetSize());
    }

    mService->UpdateCpuStats();

    if (fromMsg) {
        mBroadcastsScheduled = FALSE;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    // First, deliver any non-serialized broadcasts right away.
    while (!mParallelBroadcasts.IsEmpty()) {
        r = mParallelBroadcasts.GetFront();
        mParallelBroadcasts.PopFront();
        r->mDispatchTime = SystemClock::GetUptimeMillis();
        system->GetCurrentTimeMillis(&r->mDispatchClockTime);
        if (DEBUG_BROADCAST_LIGHT) {
            Slogger::V(TAG, "Processing parallel broadcast [%s] %p", mQueueName.string(), r.Get());
        }

        List<AutoPtr<IInterface> >::Iterator ite;
        for (ite = r->mReceivers->Begin(); ite != r->mReceivers->End(); ++ite) {
            AutoPtr<IInterface> tmpObj = *ite;
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, "Delivering non-ordered on [%s] to registered %p: %p"
                        , mQueueName.string(), tmpObj.Get(), r.Get());
            }
            BroadcastFilter* target = reinterpret_cast<BroadcastFilter*>(tmpObj->Probe(EIID_BroadcastFilter));
            DeliverToRegisteredReceiverLocked(r, target, FALSE);
        }
        AddBroadcastToHistoryLocked(r);
        if (DEBUG_BROADCAST_LIGHT) {
            Slogger::V(TAG, "Done with parallel broadcast [%s] %p", mQueueName.string(), r.Get());
        }
    }

    // Now take care of the next serialized one...

    // If we are waiting for a process to come up to handle the next
    // broadcast, then do nothing at this point.  Just in case, we
    // check that the process we're waiting for still exists.
    if (mPendingBroadcast != NULL) {
        if (DEBUG_BROADCAST_LIGHT) {
            Slogger::V(TAG, "processNextBroadcast [%s]: waiting for ", mQueueName.string(), mPendingBroadcast->mCurApp.Get());
        }

        HashMap<Int32, AutoPtr<ProcessRecord> >::Iterator it;
        {
            AutoLock lock(mService->mPidsSelfLock);
            it = mService->mPidsSelfLocked.Find(mPendingBroadcast->mCurApp->mPid);
        }
        if (it != mService->mPidsSelfLocked.End()) {
            // It's still alive, so keep waiting
            return NOERROR;
        }
        else {
            Slogger::W(TAG, "pending app  [%s]%p died before responding to broadcast"
                    , mQueueName.string(), mPendingBroadcast->mCurApp.Get());
            mPendingBroadcast->mState = BroadcastRecord::IDLE;
            mPendingBroadcast->mNextReceiver = mPendingBroadcastRecvIndex;
            mPendingBroadcast = NULL;
        }
    }

    Boolean looped = FALSE;

    do {
        if (mOrderedBroadcasts.IsEmpty()) {
            // No more broadcasts pending, so all done!
            mService->ScheduleAppGcsLocked();
            if (looped) {
                // If we had finished the last ordered broadcast, then
                // make sure all processes have correct oom and sched
                // adjustments.
                mService->UpdateOomAdjLocked();
            }
            return NOERROR;
        }
        r = mOrderedBroadcasts.GetFront();
        assert(r != NULL);

        Boolean forceReceive = FALSE;

        // Ensure that even if something goes awry with the timeout
        // detection, we catch "hung" broadcasts here, discard them,
        // and continue to make progress.
        //
        // This is only done if the system is ready so that PRE_BOOT_COMPLETED
        // receivers don't get executed with timeouts. They're intended for
        // one time heavy lifting after system upgrades and can take
        // significant amounts of time.
        Int32 numReceivers = (r->mReceivers != NULL) ? r->mReceivers->GetSize() : 0;
        if (mService->mProcessesReady && r->mDispatchTime > 0) {
            Int64 now = SystemClock::GetUptimeMillis();
            if ((numReceivers > 0) &&
                    (now > r->mDispatchTime + (2 * mTimeoutPeriod * numReceivers))) {
                Slogger::W(TAG, "Hung broadcast [%s] discarded after timeout failure: now=%lld dispatchTime=%lld startTime=%lld intent=%p numReceivers=%d nextReceiver=%d state=%d"
                        , mQueueName.string(), now, r->mDispatchTime, r->mReceiverTime, r->mIntent.Get()
                        , numReceivers, r->mNextReceiver, r->mState);
                BroadcastTimeoutLocked(FALSE); // forcibly finish this broadcast
                forceReceive = TRUE;
                r->mState = BroadcastRecord::IDLE;
            }
        }

        if (r->mState != BroadcastRecord::IDLE) {
            if (DEBUG_BROADCAST) {
                Slogger::D(TAG, "processNextBroadcast(%s) called when not idle (state=%d)"
                        , mQueueName.string(), r->mState);
            }
            return NOERROR;
        }

        if (r->mReceivers == NULL || r->mNextReceiver >= numReceivers || r->mResultAbort || forceReceive) {
            // No more receivers for this broadcast!  Send the final
            // result if requested...
            if (r->mResultTo != NULL) {
                // try {
                if (DEBUG_BROADCAST) {
                    Int32 seq = -1;
                    r->mIntent->GetInt32Extra(String("seq"), -1, &seq);
                    String action;
                    r->mIntent->GetAction(&action);
                    Slogger::I(TAG, "Finishing broadcast [%s] %s seq=%d app=%p"
                            , mQueueName.string(), action.string(), seq, r->mCallerApp.Get());
                }
                AutoPtr<IIntent> newIntent;
                CIntent::New(r->mIntent, (IIntent**)&newIntent);
                ECode ec = PerformReceiveLocked(r->mCallerApp, r->mResultTo, newIntent, r->mResultCode,
                        r->mResultData, r->mResultExtras, FALSE, FALSE, r->mUserId);
                if (FAILED(ec)) {
                    Slogger::W(TAG, "Failure [%s] sending broadcast result of %p 0x%08x"
                            , mQueueName.string(), r->mIntent.Get(), ec);
                    break;
                }
                // Set this to NULL so that the reference
                // (local and remote) isnt kept in the mBroadcastHistory.
                r->mResultTo = NULL;
                // } catch (RemoteException e) {
                //     Slog.w(TAG, "Failure ["
                //             + mQueueName + "] sending broadcast result of "
                //             + r->mIntent, e);
                // }
            }

            if (DEBUG_BROADCAST) Slogger::V(TAG, "Cancelling BROADCAST_TIMEOUT_MSG");
            CancelBroadcastTimeoutLocked();

            if (DEBUG_BROADCAST_LIGHT) Slogger::V(TAG, "Finished with ordered broadcast %p", r.Get());

            // ... and on to the next...
            AddBroadcastToHistoryLocked(r);
            mOrderedBroadcasts.PopFront();
            r = NULL;
            looped = TRUE;
            continue;
        }
    } while (r == NULL);

    // Get the next receiver...
    Int32 recIdx = r->mNextReceiver++;

    // Keep track of when this receiver started, and make sure there
    // is a timeout message pending to kill it if need be.
    r->mReceiverTime = SystemClock::GetUptimeMillis();
    if (recIdx == 0) {
        r->mDispatchTime = r->mReceiverTime;
        system->GetCurrentTimeMillis(&r->mDispatchClockTime);
        if (DEBUG_BROADCAST_LIGHT) {
            Slogger::V(TAG, "Processing ordered broadcast [%s] %p", mQueueName.string(), r.Get());
        }
    }
    if (!mPendingBroadcastTimeoutMessage) {
        Int64 timeoutTime = r->mReceiverTime + mTimeoutPeriod;
        if (DEBUG_BROADCAST) {
            Slogger::V(TAG, "Submitting BROADCAST_TIMEOUT_MSG [%s] for %p at %d", mQueueName.string(), r.Get(), timeoutTime);
        }
        SetBroadcastTimeoutLocked(timeoutTime);
    }

    assert(recIdx >= 0 && recIdx < r->mReceivers->GetSize());
    AutoPtr<IInterface> nextReceiver = (*r->mReceivers)[recIdx];
    if (nextReceiver->Probe(EIID_BroadcastFilter) != NULL) {
        // Simple case: this is a registered receiver who gets
        // a direct call.
        AutoPtr<BroadcastFilter> filter = reinterpret_cast<BroadcastFilter*>(nextReceiver->Probe(EIID_BroadcastFilter));
        if (DEBUG_BROADCAST)  {
            Slogger::V(TAG, "Delivering ordered [%s] to registered %s: %s"
                    , mQueueName.string(), filter->ToString().string(), r->ToString().string());
        }
        DeliverToRegisteredReceiverLocked(r, filter, r->mOrdered);
        if (r->mReceiver == NULL || !r->mOrdered) {
            // The receiver has already finished, so schedule to
            // process the next one.
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, "Quick finishing [%s]: ordered=%d receiver=%p"
                        , mQueueName.string(), r->mOrdered, r->mReceiver.Get());
            }
            r->mState = BroadcastRecord::IDLE;
            ScheduleBroadcastsLocked();
        }
        return NOERROR;
    }

    // Hard case: need to instantiate the receiver, possibly
    // starting its application process to host it.

    AutoPtr<IResolveInfo> info = IResolveInfo::Probe(nextReceiver);
    AutoPtr<IActivityInfo> aInfo;
    info->GetActivityInfo((IActivityInfo**)&aInfo);
    AutoPtr<IApplicationInfo> appInfo;
    aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String permission;
    aInfo->GetPermission(&permission);
    String processName;
    aInfo->GetProcessName(&processName);
    String packageName;
    aInfo->GetPackageName(&packageName);
    String name;
    aInfo->GetName(&name);
    Int32 flags;
    aInfo->GetFlags(&flags);
    Boolean exported;
    aInfo->GetExported(&exported);
    Int32 appUid;
    appInfo->GetUid(&appUid);
    String appPackageName;
    appInfo->GetPackageName(&appPackageName);

    AutoPtr<IComponentName> component;
    CComponentName::New(appPackageName, name, (IComponentName**)&component);

    Boolean skip = FALSE;
    Int32 perm = mService->CheckComponentPermission(permission,
            r->mCallingPid, r->mCallingUid, appUid, exported);
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        if (!exported) {
            String str;
            r->mIntent->ToString(&str);
            String str2;
            component->FlattenToShortString(&str2);
            Slogger::W(TAG, "Permission Denial: broadcasting %s from %s (pid=%d, uid=%d) is not exported from uid %d due to receiver %s"
                    , str.string(), r->mCallerPackage.string(), r->mCallingPid, r->mCallingUid, appUid, str2.string());
        }
        else {
            String str;
            r->mIntent->ToString(&str);
            String str2;
            component->FlattenToShortString(&str2);
            Slogger::W(TAG, "Permission Denial: broadcasting %s from %s (pid=%d, uid=%d) requires %s due to receiver "
                    , str.string(), r->mCallerPackage.string(), r->mCallingPid, r->mCallingUid, permission.string(), str2.string());
        }
        skip = TRUE;
    }
    if (appUid != IProcess::SYSTEM_UID && !r->mRequiredPermission.IsNull()) {
        // try {
        if (FAILED(AppGlobals::GetPackageManager()->CheckPermission(r->mRequiredPermission, appPackageName, &perm))) {
            perm = IPackageManager::PERMISSION_DENIED;
        }
        // } catch (RemoteException e) {
        //     perm = IPackageManager::PERMISSION_DENIED;
        // }
        if (perm != IPackageManager::PERMISSION_GRANTED) {
            String str;
            component->FlattenToShortString(&str);
            Slogger::W(TAG, "Permission Denial: receiving %p to %s requires %s due to sender %s (uid %d)"
                    , r->mIntent.Get(), str.string(), r->mRequiredPermission.string(), r->mCallerPackage.string(), r->mCallingUid);
            skip = TRUE;
        }
    }
    Boolean isSingleton = FALSE;
    // try {
    isSingleton = mService->IsSingleton(processName, appInfo, name, flags);
    // } catch (SecurityException e) {
    //     // Slog.w(TAG, e.getMessage());
    //     skip = TRUE;
    // }
    if ((flags & IActivityInfo::FLAG_SINGLE_USER) != 0) {
        AutoPtr<IActivityManagerHelper> helper;
        CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&helper);
        Int32 result;
        if (helper->CheckUidPermission(Elastos::Droid::Manifest::Permission::INTERACT_ACROSS_USERS, appUid, &result)
                , result != IPackageManager::PERMISSION_GRANTED) {
            String str;
            component->FlattenToShortString(&str);
            Slogger::W(TAG, "Permission Denial: Receiver %s requests FLAG_SINGLE_USER, but app does not hold Elastos::Droid::Manifest::Permission::INTERACT_ACROSS_USERS"
                    , str.string());
            skip = TRUE;
        }
    }
    if (r->mCurApp != NULL && r->mCurApp->mCrashing) {
        // If the target process is crashing, just skip it.
        if (DEBUG_BROADCAST) {
            Slogger::V(TAG, "Skipping deliver ordered [%s] %p to %p: process crashing"
                    , mQueueName.string(), r.Get(), r->mCurApp.Get());
        }
        skip = TRUE;
    }

    if (skip) {
        if (DEBUG_BROADCAST)  {
            Slogger::V(TAG, "Skipping delivery of ordered [%s] %p for whatever reason", mQueueName.string(), r.Get());
        }
        r->mReceiver = NULL;
        r->mCurFilter = NULL;
        r->mState = BroadcastRecord::IDLE;
        ScheduleBroadcastsLocked();
        return NOERROR;
    }

    r->mState = BroadcastRecord::APP_RECEIVE;
    String targetProcess = processName;
    r->mCurComponent = component;
    if (r->mCallingUid != IProcess::SYSTEM_UID && isSingleton) {
        aInfo = mService->GetActivityInfoForUser(aInfo, 0);
    }
    r->mCurReceiver = aInfo;
    if (DEBUG_MU && r->mCallingUid > IUserHandle::PER_USER_RANGE) {
        Slogger::V(TAG_MU, "Updated broadcast record activity info for secondary user, %p, callingUid = %d, uid = %d"
                , aInfo.Get(), r->mCallingUid,  appUid);
    }

    // Broadcast is being executed, its package can't be stopped.
    // try {
    String pkgName;
    r->mCurComponent->GetPackageName(&pkgName);
    AppGlobals::GetPackageManager()->SetPackageStoppedState(
            pkgName, FALSE, UserHandle::GetUserId(r->mCallingUid));
    // } catch (RemoteException e) {
    // } catch (IllegalArgumentException e) {
    //     Slog.w(TAG, "Failed trying to unstop package "
    //             + r->mCurComponent.getPackageName() + ": " + e);
    // }

    // Is this receiver's application already running?
    AutoPtr<ProcessRecord> app = mService->GetProcessRecordLocked(targetProcess, appUid);
    if (app != NULL && app->mThread != NULL) {
        // try {
        app->AddPackage(packageName);
        ProcessCurBroadcastLocked(r, app);
        return NOERROR;
        // } catch (RemoteException e) {
        //     Slog.w(TAG, "Exception when sending broadcast to "
        //           + r->mCurComponent, e);
        // } catch (RuntimeException e) {
        //             Log.wtf(TAG, "Failed sending broadcast to "
        //                     + r.curComponent + " with " + r.intent, e);
        //             // If some unexpected exception happened, just skip
        //             // this broadcast.  At this point we are not in the call
        //             // from a client, so throwing an exception out from here
        //             // will crash the entire system instead of just whoever
        //             // sent the broadcast.
        //             logBroadcastReceiverDiscardLocked(r);
        //             finishReceiverLocked(r, r.resultCode, r.resultData,
        //                     r.resultExtras, r.resultAbort, true);
        //             scheduleBroadcastsLocked();
        //             // We need to reset the state if we failed to start the receiver.
        //             r.state = BroadcastRecord.IDLE;
        //             return;
        //         }

        // If a dead object exception was thrown -- fall through to
        // restart the application.
    }

    // Not running -- get it started, to be executed when the app comes up.
    if (DEBUG_BROADCAST) {
        Slogger::V(TAG, "Need to start app [%s] %s for broadcast %p"
                , mQueueName.string(), targetProcess.string(), r.Get());
    }
    Int32 intentFlags;
    r->mIntent->GetFlags(&intentFlags);
    if ((r->mCurApp = mService->StartProcessLocked(targetProcess,
            appInfo, TRUE,
            intentFlags | IIntent::FLAG_FROM_BACKGROUND,
            String("broadcast"), r->mCurComponent,
            (intentFlags & IIntent::FLAG_RECEIVER_BOOT_UPGRADE) != 0, FALSE))
                    == NULL) {
        // Ah, this recipient is unavailable.  Finish it if necessary,
        // and mark the broadcast record as ready for the next.
        Slogger::W(TAG, "Unable to launch app %s/%d for broadcast %p: process is bad"
                , appPackageName.string(), appUid, r->mIntent.Get());
        LogBroadcastReceiverDiscardLocked(r);
        FinishReceiverLocked(r, r->mResultCode, r->mResultData,
                r->mResultExtras, r->mResultAbort, TRUE);
        ScheduleBroadcastsLocked();
        r->mState = BroadcastRecord::IDLE;
        return NOERROR;
    }

    mPendingBroadcast = r;
    mPendingBroadcastRecvIndex = recIdx;

    return NOERROR;
}

void BroadcastQueue::SetBroadcastTimeoutLocked(
    /* [in] */ Millisecond64 timeoutTime)
{
    if (!mPendingBroadcastTimeoutMessage) {
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(BROADCAST_TIMEOUT_MSG, (IMessage**)&msg);
        msg->SetObj((IInterface*)this);
        Boolean result;
        mHandler->SendMessageAtTime(msg, timeoutTime, &result);

        mPendingBroadcastTimeoutMessage = TRUE;
    }
}

void BroadcastQueue::CancelBroadcastTimeoutLocked()
{
    if (mPendingBroadcastTimeoutMessage) {
        mHandler->RemoveMessages(BROADCAST_TIMEOUT_MSG, (IInterface*)this);
        mPendingBroadcastTimeoutMessage = FALSE;
    }
}

void BroadcastQueue::BroadcastTimeoutLocked(
    /* [in] */ Boolean fromMsg)
{
    if (fromMsg) {
        mPendingBroadcastTimeoutMessage = FALSE;
    }

    if (mOrderedBroadcasts.IsEmpty()) {
        return;
    }

    Int64 now = SystemClock::GetUptimeMillis();
    AutoPtr<BroadcastRecord> r = mOrderedBroadcasts.GetFront();
    if (fromMsg) {
        if (mService->mDidDexOpt) {
            // Delay timeouts until dexopt finishes.
            mService->mDidDexOpt = FALSE;
            Int64 timeoutTime = SystemClock::GetUptimeMillis() + mTimeoutPeriod;
            SetBroadcastTimeoutLocked(timeoutTime);
            return;
        }
        if (!mService->mProcessesReady) {
            // Only process broadcast timeouts if the system is ready. That way
            // PRE_BOOT_COMPLETED broadcasts can't timeout as they are intended
            // to do heavy lifting for system up.
            return;
        }

        Int64 timeoutTime = r->mReceiverTime + mTimeoutPeriod;
        if (timeoutTime > now) {
            // We can observe premature timeouts because we do not cancel and reset the
            // broadcast timeout message after each receiver finishes.  Instead, we set up
            // an initial timeout then kick it down the road a little further as needed
            // when it expires.
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, "Premature timeout [%s] @ %d: resetting BROADCAST_TIMEOUT_MSG for %d"
                        , mQueueName.string(), now, timeoutTime);
            }
            SetBroadcastTimeoutLocked(timeoutTime);
            return;
        }
    }

    String record = r->ToString();
    String receiver;
    if (r->mReceiver) {
        r->mReceiver->ToString(&receiver);
    }

    Slogger::W(TAG, "Timeout of broadcast %s - receiver=%s, started %d ms ago"
            , record.string(), receiver.string(), (now - r->mReceiverTime));

    r->mReceiverTime = now;
    r->mAnrCount++;

    // Current receiver has passed its expiration date.
    if (r->mNextReceiver <= 0) {
        Slogger::W(TAG, "Timeout on receiver with nextReceiver <= 0");
        return;
    }

    AutoPtr<ProcessRecord> app;
    String anrMessage;

    AutoPtr<IInterface> curReceiver = (*r->mReceivers)[r->mNextReceiver - 1];
    Slogger::W(TAG, "Receiver during timeout: %p", curReceiver.Get());
    LogBroadcastReceiverDiscardLocked(r);
    if (curReceiver->Probe(EIID_BroadcastFilter) != NULL) {
        AutoPtr<BroadcastFilter> bf = reinterpret_cast<BroadcastFilter*>(curReceiver->Probe(EIID_BroadcastFilter));
        Slogger::W(TAG, "Receiver during timeout BroadcastFilter: %s", bf->ToString().string());
        if (bf->mReceiverList->mPid != 0
                && bf->mReceiverList->mPid != CActivityManagerService::MY_PID) {
            AutoLock lock(mService->mPidsSelfLock);
            app = mService->mPidsSelfLocked[bf->mReceiverList->mPid];
        }
    }
    else {
        app = r->mCurApp;
    }

    if (app != NULL) {
        String str;
        r->mIntent->ToString(&str);
        anrMessage.AppendFormat("Broadcast of %s", str.string());
    }

    if (mPendingBroadcast == r) {
        mPendingBroadcast = NULL;
    }

    // Move on to the next receiver.
    FinishReceiverLocked(r, r->mResultCode, r->mResultData,
            r->mResultExtras, r->mResultAbort, TRUE);
    ScheduleBroadcastsLocked();

    if (!anrMessage.IsNull()) {
        // Post the ANR to the handler since we do not want to process ANRs while
        // potentially holding our lock.

        AutoPtr<IRunnable> anr = new AppNotResponding(app, anrMessage, this);
        Boolean result;
        mHandler->Post(anr, &result);
    }
}

void BroadcastQueue::AddBroadcastToHistoryLocked(
    /* [in] */ BroadcastRecord* r)
{
    if (r->mCallingUid < 0) {
        // This was from a registerReceiver() call; ignore it.
        return;
    }

    for (Int32 i = MAX_BROADCAST_HISTORY - 1; i > 0; i--) {
        mBroadcastHistory->Set(i, (*mBroadcastHistory)[i - 1]);
    }
    r->mFinishTime = SystemClock::GetUptimeMillis();
    mBroadcastHistory->Set(0, r);

    for (Int32 i = MAX_BROADCAST_SUMMARY_HISTORY - 1; i > 0; i--) {
        mBroadcastSummaryHistory->Set(i, (*mBroadcastSummaryHistory)[i - 1]);
    }
    mBroadcastSummaryHistory->Set(0, r->mIntent);
}

void BroadcastQueue::LogBroadcastReceiverDiscardLocked(
    /* [in] */ BroadcastRecord* r)
{
    if (r->mNextReceiver > 0) {
        AutoPtr<IInterface> curReceiver = (*r->mReceivers)[r->mNextReceiver-1];
        AutoPtr<BroadcastFilter> bFilter = reinterpret_cast<BroadcastFilter*>(curReceiver->Probe(EIID_BroadcastFilter));
        if (bFilter != NULL) {
            String filter = bFilter->ToString();
            String record = r->ToString();
            Slogger::W(TAG, "BroadcastRecord: %s, BroadcastFilter: %s", record.string(), filter.string());
    //         EventLog.writeEvent(EventLogTags.AM_BROADCAST_DISCARD_FILTER,
    //                 bf.owningUserId, System.identityHashCode(r),
    //                 r->mIntent.getAction(),
    //                 r->mNextReceiver - 1,
    //                 System.identityHashCode(bf));
        }
        else {
            AutoPtr<IResolveInfo> ri = IResolveInfo::Probe(curReceiver);
            String record = r->ToString();
            String resolve;
            if (ri) {
                ri->ToString(&resolve);
            }
            Slogger::W(TAG, "BroadcastRecord: %s, ResolveInfo: %s", record.string(), resolve.string());

    //         EventLog.writeEvent(EventLogTags.AM_BROADCAST_DISCARD_APP,
    //                 UserHandle.getUserId(ri.activityInfo.applicationInfo.uid),
    //                 System.identityHashCode(r), r->mIntent.getAction(),
    //                 r->mNextReceiver - 1, ri.toString());
        }
    }
    else {
        Slogger::W(TAG, "Discarding broadcast before first receiver is invoked: %s",
                r->ToString().string());
    //     EventLog.writeEvent(EventLogTags.AM_BROADCAST_DISCARD_APP,
    //             -1, System.identityHashCode(r),
    //             r->mIntent.getAction(),
    //             r->mNextReceiver,
    //             "NONE");
    }
}

Boolean BroadcastQueue::DumpLocked(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Int32 opti,
    /* [in] */ Boolean dumpAll,
    /* [in] */ const String& dumpPackage,
    /* [in] */ Boolean needSep)
{
    if (!mParallelBroadcasts.IsEmpty() || !mOrderedBroadcasts.IsEmpty() || mPendingBroadcast != NULL) {
        Boolean printed = FALSE;
        Int32 i = mParallelBroadcasts.GetSize() - 1;
        List< AutoPtr<BroadcastRecord> >::ReverseIterator rit;
        for (rit = mParallelBroadcasts.RBegin(); rit != mParallelBroadcasts.REnd(); ++rit, --i) {
            AutoPtr<BroadcastRecord> br = *rit;
            if (!dumpPackage.IsNull() && !dumpPackage.Equals(br->mCallerPackage)) {
                continue;
            }
            if (!printed) {
                if (needSep) {
                    pw->Println();
                }
                needSep = TRUE;
                printed = TRUE;
                pw->PrintStringln(String("  Active broadcasts [") + mQueueName + "]:");
            }
            StringBuilder sb("  Active Broadcast ");
            sb += mQueueName;
            sb += " #";
            sb += i;
            sb += ":";
            pw->PrintStringln(sb.ToString());
            // TODO:
            // br.dump(pw, "    ");
        }
        printed = FALSE;
        needSep = TRUE;
        i = mOrderedBroadcasts.GetSize() - 1;
        for (rit = mOrderedBroadcasts.RBegin(); rit != mOrderedBroadcasts.REnd(); ++rit, --i) {
            AutoPtr<BroadcastRecord> br = *rit;
            if (!dumpPackage.IsNull() && !dumpPackage.Equals(br->mCallerPackage)) {
                continue;
            }
            if (!printed) {
                if (needSep) {
                    pw->Println();
                }
                needSep = TRUE;
                printed = TRUE;
                pw->PrintStringln(String("  Active ordered broadcasts [") + mQueueName + "]:");
            }
            StringBuilder sb("  Active Ordered Broadcast ");
            sb += mQueueName;
            sb += " #";
            sb += i;
            sb += ":";
            pw->PrintStringln(sb.ToString());
            // TODO:
            // br->Dump(pw, "    ");
        }
        if (dumpPackage.IsNull() || (mPendingBroadcast != NULL
                && dumpPackage.Equals(mPendingBroadcast->mCallerPackage))) {
            if (needSep) {
                pw->Println();
            }
            pw->PrintStringln(String("  Pending broadcast [") + mQueueName + "]:");
            if (mPendingBroadcast != NULL) {
                // TODO:
                // mPendingBroadcast.dump(pw, "    ");
            }
            else {
                pw->PrintStringln(String("    (null)"));
            }
            needSep = TRUE;
        }
    }

    Int32 i;
    Boolean printed = FALSE;
    for (i = 0; i < MAX_BROADCAST_HISTORY; i++) {
        AutoPtr<BroadcastRecord> r = (*mBroadcastHistory)[i];
        if (r == NULL) {
            break;
        }
        if (dumpPackage != NULL && !dumpPackage.Equals(r->mCallerPackage)) {
            continue;
        }
        if (!printed) {
            if (needSep) {
                pw->Println();
            }
            needSep = TRUE;
            pw->PrintStringln(String("  Historical broadcasts [") + mQueueName + "]:");
            printed = TRUE;
        }
        if (dumpAll) {
            pw->PrintString(String("  Historical Broadcast ") + mQueueName + " #");
            pw->PrintInt32(i);
            pw->PrintStringln(String(":"));
            // TODO:
            // r.dump(pw, "    ");
        }
        else {
            pw->PrintString(String("  #"));
            pw->PrintInt32(i);
            pw->PrintString(String(": "));
            // TODO:
            // pw->Println(r);
            pw->PrintString(String("    "));
            String intentStr;
            r->mIntent->ToShortString(FALSE, TRUE, TRUE, FALSE, &intentStr);
            pw->PrintStringln(intentStr);
            AutoPtr<IBundle> bundle;
            r->mIntent->GetExtras((IBundle**)&bundle);
            if (bundle != NULL) {
                pw->PrintString(String("    extras: "));
                String bundleStr;
                bundle->ToString(&bundleStr);
                pw->PrintStringln(bundleStr);
            }
        }
    }

    if (dumpPackage.IsNull()) {
        if (dumpAll) {
            i = 0;
            printed = FALSE;
        }
        for (; i < MAX_BROADCAST_SUMMARY_HISTORY; i++) {
            AutoPtr<IIntent> intent = (*mBroadcastSummaryHistory)[i];
            if (intent == NULL) {
                break;
            }
            if (!printed) {
                if (needSep) {
                    pw->Println();
                }
                needSep = TRUE;
                pw->PrintStringln(String("  Historical broadcasts summary [") + mQueueName + "]:");
                printed = TRUE;
            }
            if (!dumpAll && i >= 50) {
                pw->PrintStringln(String("  ..."));
                break;
            }
            pw->PrintString(String("  #"));
            pw->PrintInt32(i);
            pw->PrintString(String(": "));
            String iStr;
            intent->ToShortString(FALSE, TRUE, TRUE, FALSE, &iStr);
            pw->PrintStringln(iStr);
            AutoPtr<IBundle> bundle;
            intent->GetExtras((IBundle**)&bundle);
            if (bundle != NULL) {
                pw->PrintString(String("    extras: "));
                String bundleStr;
                bundle->ToString(&bundleStr);
                pw->PrintStringln(bundleStr);
            }
        }
    }

    return needSep;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
