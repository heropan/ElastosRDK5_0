
#include "elastos/droid/webkit/WebSyncManager.h"
#include "elastos/droid/webkit/WebViewDatabase.h"
#include "elastos/droid/os/CMessageHelper.h"
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/os/Process.h"

using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Core::CThread;
using Elastos::Core::EIID_IRunnable;

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
        mh->Obtain(this, SYNC_MESSAGE, (IMessage**)&newmsg);
        Boolean result = FALSE;
        SendMessageDelayed(newmsg, SYNC_LATER_INTERVAL, &result);
    }
    return NOERROR;
}

ECode WebSyncManager::SyncHandler::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebSyncManager::SyncHandler";
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
    mh->Obtain(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
    Boolean result = FALSE;
    mHandler->SendMessageDelayed(msg, SYNC_LATER_INTERVAL, &result);

    Looper::Loop();

    return NOERROR;
}

/**
 * sync() forces sync manager to sync now
 */
ECode WebSyncManager::Sync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "*** WebSyncManager sync ***");
    //}

    if (mHandler == NULL) {
        return NOERROR;
    }

    mHandler->RemoveMessages(SYNC_MESSAGE);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
    Boolean result = FALSE;
    mHandler->SendMessageDelayed(msg, SYNC_NOW_INTERVAL, &result);

    return NOERROR;
}

/**
 * resetSync() resets sync manager's timer
 */
ECode WebSyncManager::ResetSync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "*** WebSyncManager resetSync ***");
    //}

    if (mHandler == NULL) {
        return NOERROR;
    }

    mHandler->RemoveMessages(SYNC_MESSAGE);
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<IMessage> msg;
    mh->Obtain(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
    Boolean result = FALSE;
    mHandler->SendMessageDelayed(msg, SYNC_LATER_INTERVAL, &result);

    return NOERROR;
}

/**
 * startSync() requests sync manager to start sync
 */
ECode WebSyncManager::StartSync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "***  WebSyncManager startSync ***, Ref count:" +
    //            mStartSyncRefCount);
    //}

    if (mHandler == NULL) {
        return NOERROR;
    }

    if (++mStartSyncRefCount == 1) {
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        AutoPtr<IMessage> msg;
        mh->Obtain(mHandler, SYNC_MESSAGE, (IMessage**)&msg);
        Boolean result = FALSE;
        mHandler->SendMessageDelayed(msg, SYNC_LATER_INTERVAL, &result);
    }

    return NOERROR;
}

/**
 * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
 * the queue to break the sync loop
 */
ECode WebSyncManager::StopSync()
{
    //if (DebugFlags.WEB_SYNC_MANAGER) {
    //    Log.v(LOGTAG, "*** WebSyncManager stopSync ***, Ref count:" +
    //            mStartSyncRefCount);
    //}

    if (mHandler == NULL) {
        return NOERROR;
    }

    if (--mStartSyncRefCount == 0) {
        mHandler->RemoveMessages(SYNC_MESSAGE);
    }

    return NOERROR;
}

AutoPtr<IInterface> WebSyncManager::Clone()
{
    assert(0);
    return NULL;
}

void WebSyncManager::OnSyncInit()
{
}

ECode WebSyncManager::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebSyncManager";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos