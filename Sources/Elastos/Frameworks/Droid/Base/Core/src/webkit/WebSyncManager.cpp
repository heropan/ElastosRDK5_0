
#include "webkit/WebSyncManager.h"
#include "webkit/WebViewDatabase.h"
#include "os/CMessageHelper.h"
#include "os/Looper.h"
#include "os/Process.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                   WebSyncManager::SyncHandler
//===============================================================

WebSyncManager::SyncHandler::SyncHandler(
    /* [in] */ WebSyncManager* owner)
    : mOwner(owner)
{
}

ECode WebSyncManager::SyncHandler::HandleMessage(
            /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    Int32 what;
    msg->GetWhat(&what);
    if (what == SYNC_MESSAGE) {
        //if (DebugFlags.WEB_SYNC_MANAGER) {
        //    Log.v(LOGTAG, "*** WebSyncManager sync ***");
        //}
        mOwner->SyncFromRamToFlash();

        // send a delayed message to request sync later
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        AutoPtr<IMessage> newmsg;
        mh->ObtainEx3(this, SYNC_MESSAGE, (IMessage**)&newmsg);
        Boolean result = FALSE;
        SendMessageDelayed(newmsg, SYNC_LATER_INTERVAL, &result);
    }
    return NOERROR;
}

//===============================================================
//                       WebSyncManager
//===============================================================

// log tag
const String WebSyncManager::LOGTAG("websync");

// message code for sync message
const Int32 WebSyncManager::SYNC_MESSAGE = 101;
// time delay in millisec for a sync (now) message
Int32 WebSyncManager::SYNC_NOW_INTERVAL = 100; // 100 millisec
// time delay in millisec for a sync (later) message
Int32 WebSyncManager::SYNC_LATER_INTERVAL = 5 * 60 * 1000; // 5 minutes

WebSyncManager::WebSyncManager()
{
}

WebSyncManager::WebSyncManager(
    /* [in] */ IContext* context,
    /* [in] */ const String& name)
{
    mThreadName = name;
    if (context != NULL) {
        mDataBase = WebViewDatabase::GetInstance(context);
        CThread::New(this, (IThread**)&mSyncThread);
        mSyncThread->SetName(mThreadName);
        mSyncThread->Start();
    }
    else {
    //    throw new IllegalStateException(
    //            "WebSyncManager can't be created without context");
        assert(0);
    }
}

CAR_INTERFACE_IMPL(WebSyncManager, IRunnable);

ECode WebSyncManager::Run()
{
    // prepare Looper for sync handler
    Looper::Prepare();
    mHandler = new SyncHandler(this);
    OnSyncInit();
    // lower the priority after onSyncInit() is done
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_BACKGROUND);

    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->ObtainEx3(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
    Boolean result = FALSE;
    mHandler->SendMessageDelayed(msg, SYNC_LATER_INTERVAL, &result);

    Looper::Loop();

    return NOERROR;
}

/**
 * sync() forces sync manager to sync now
 */
void WebSyncManager::Sync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "*** WebSyncManager sync ***");
    //}

    if (mHandler == NULL) {
        return;
    }

    mHandler->RemoveMessages(SYNC_MESSAGE);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->ObtainEx3(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
    Boolean result = FALSE;
    mHandler->SendMessageDelayed(msg, SYNC_NOW_INTERVAL, &result);
}

/**
 * resetSync() resets sync manager's timer
 */
void WebSyncManager::ResetSync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "*** WebSyncManager resetSync ***");
    //}

    if (mHandler == NULL) {
        return;
    }

    mHandler->RemoveMessages(SYNC_MESSAGE);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->ObtainEx3(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
    Boolean result = FALSE;
    mHandler->SendMessageDelayed(msg, SYNC_LATER_INTERVAL, &result);
}

/**
 * startSync() requests sync manager to start sync
 */
void WebSyncManager::StartSync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "***  WebSyncManager startSync ***, Ref count:" +
    //            mStartSyncRefCount);
    //}

    if (mHandler == NULL) {
        return;
    }

    if (++mStartSyncRefCount == 1) {
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        AutoPtr<IMessage> msg;
        mh->ObtainEx3(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
        Boolean result = FALSE;
        mHandler->SendMessageDelayed(msg, SYNC_LATER_INTERVAL, &result);
    }
}

/**
 * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
 * the queue to break the sync loop
 */
void WebSyncManager::StopSync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "*** WebSyncManager stopSync ***, Ref count:" +
    //            mStartSyncRefCount);
    //}

    if (mHandler == NULL) {
        return;
    }

    if (--mStartSyncRefCount == 0) {
        mHandler->RemoveMessages(SYNC_MESSAGE);
    }
}

AutoPtr<IInterface> WebSyncManager::Clone()
{
    assert(0);
    return NULL;
}

void WebSyncManager::OnSyncInit()
{
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos