
#include "util/AsyncChannel.h"
#ifdef DROID_CORE
#include "os/CHandlerThread.h"
#include "os/CMessenger.h"
#include "os/CMessageHelper.h"
#include "content/CIntent.h"
#endif
#include <elastos/core/Thread.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::StringUtils;
using Elastos::Core::Mutex;
using Elastos::Core::Thread;
using Elastos::Core::IThread;
using Elastos::Core::CThread;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::CMessenger;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IIMessenger;
using Elastos::Droid::Os::EIID_IHandler;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

Stack< AutoPtr<AsyncChannel::SyncMessenger> > AsyncChannel::SyncMessenger::sStack;
Mutex AsyncChannel::SyncMessenger::sStackLock;
Int32 AsyncChannel::SyncMessenger::sCount = 0;

const String AsyncChannel::TAG("AsyncChannel");
const Boolean AsyncChannel::DBG = FALSE;
const Int32 AsyncChannel::BASE;
const Int32 AsyncChannel::CMD_CHANNEL_HALF_CONNECTED;
const Int32 AsyncChannel::CMD_CHANNEL_FULL_CONNECTION;
const Int32 AsyncChannel::CMD_CHANNEL_FULLY_CONNECTED;
const Int32 AsyncChannel::CMD_CHANNEL_DISCONNECT;
const Int32 AsyncChannel::CMD_CHANNEL_DISCONNECTED;
const Int32 AsyncChannel::STATUS_SUCCESSFUL;
const Int32 AsyncChannel::STATUS_BINDING_UNSUCCESSFUL;
const Int32 AsyncChannel::STATUS_SEND_UNSUCCESSFUL;
const Int32 AsyncChannel::STATUS_FULL_CONNECTION_REFUSED_ALREADY_CONNECTED;


//=============================================
// AsyncChannel::SyncMessenger::SyncHandler
//=============================================
ECode AsyncChannel::SyncMessenger::SyncHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    mResultMsg = NULL;
    helper->Obtain((IMessage**)&mResultMsg);
    mResultMsg->CopyFrom(msg);
    {
        mLockObject.Lock();
        mLockObject.Notify();
        mLockObject.Unlock();
    }
    return NOERROR;
}


//=============================================
// AsyncChannel::SyncMessenger
//=============================================
AutoPtr<AsyncChannel::SyncMessenger> AsyncChannel::SyncMessenger::Obtain()
{
    AutoPtr<SyncMessenger> sm;
    {
        Mutex::Autolock lock(sStackLock);
        if (sStack.IsEmpty()) {
            sm = new SyncMessenger();
            AutoPtr<IThread> thread;
            //for new CHandlerThread
            Thread::Attach((IThread**)&thread);
            sm->mHandlerThread = NULL;
            CHandlerThread::New(String("SyncHandler-") + StringUtils::Int32ToString(sCount++),
                (IHandlerThread**)&sm->mHandlerThread);
            sm->mHandlerThread->Start();
            AutoPtr<ILooper> l;
            sm->mHandlerThread->GetLooper((ILooper**)&l);
            sm->mHandler = new SyncHandler(l);
            sm->mMessenger = NULL;
            CMessenger::New(sm->mHandler, (IMessenger**)&sm->mMessenger);
        }
        else {
            sm = sStack.GetTop();
            sStack.Pop();
        }
    }
    return sm;
}

void AsyncChannel::SyncMessenger::Recycle()
{
    {
        Mutex::Autolock lock(sStackLock);
        sStack.Push(this);
    }
}

AutoPtr<IMessage> AsyncChannel::SyncMessenger::SendMessageSynchronously(
    /* [in] */ IMessenger* dstMessenger,
    /* [in] */ IMessage* msg)
{
    AutoPtr<SyncMessenger> sm = SyncMessenger::Obtain();
    // try {
    if (dstMessenger != NULL && msg != NULL) {
        msg->SetReplyTo(sm->mMessenger);
        AutoPtr<IThread> thread;
        Thread::Attach((IThread**)&thread);
        {
            sm->mHandler->mLockObject.Lock();
            dstMessenger->Send(msg);
            sm->mHandler->mLockObject.Wait();
            sm->mHandler->mLockObject.Unlock();
        }
    }
    else {
        sm->mHandler->mResultMsg = NULL;
    }
    // } catch (InterruptedException e) {
    //     sm.mHandler.mResultMsg = null;
    // } catch (RemoteException e) {
    //     sm.mHandler.mResultMsg = null;
    // }
    AutoPtr<IMessage> resultMsg = sm->mHandler->mResultMsg;
    sm->Recycle();
    return resultMsg;
}

//=============================================
// AsyncChannel::AsyncChannelConnection
//=============================================
CAR_INTERFACE_IMPL(AsyncChannel::AsyncChannelConnection, IServiceConnection)

AsyncChannel::AsyncChannelConnection::AsyncChannelConnection(
    /* [in] */ IWeakReference* host)
    : mWeakHost(host)
{
}

ECode AsyncChannel::AsyncChannelConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    Slogger::D("AsyncChannel::AsyncChannelConnection", " >> OnServiceConnected");
    AutoPtr<IWeakReferenceSource> wrs;
    mWeakHost->Resolve(EIID_IWeakReferenceSource, (IInterface**)&wrs);
    if (wrs) {
        AsyncChannel* ac = (AsyncChannel*)wrs.Get();
        ac->mDstMessenger = NULL;
        AutoPtr<IIMessenger> msger = IIMessenger::Probe(service);
        assert(msger != NULL);
        CMessenger::New(msger, (IMessenger**)&ac->mDstMessenger);
        ac->ReplyHalfConnected(AsyncChannel::STATUS_SUCCESSFUL);
    }
    return NOERROR;
}

ECode AsyncChannel::AsyncChannelConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    Slogger::D("AsyncChannel::AsyncChannelConnection", " >> OnServiceDisconnected");
    AutoPtr<IWeakReferenceSource> wrs;
    mWeakHost->Resolve(EIID_IWeakReferenceSource, (IInterface**)&wrs);
    if (wrs) {
        AsyncChannel* ac = (AsyncChannel*)wrs.Get();
        ac->ReplyDisconnected(AsyncChannel::STATUS_SUCCESSFUL);
    }
    return NOERROR;
}

//=============================================
// AsyncChannel::ConnectAsyncRunnable
//=============================================
AsyncChannel::ConnectAsyncRunnable::ConnectAsyncRunnable(
    /* [in] */ IWeakReference* wr,
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ const String& dstPackageName,
    /* [in] */ const String& dstClassName)
    : mWeakHost(wr)
    , mSrcCtx(srcContext)
    , mSrcHdlr(srcHandler)
    , mDstPackageName(dstPackageName)
    , mDstClassName(dstClassName)
{
}

ECode AsyncChannel::ConnectAsyncRunnable::Run()
{
    AutoPtr<IWeakReferenceSource> wrs;
    mWeakHost->Resolve(EIID_IWeakReferenceSource, (IInterface**)&wrs);
    if (wrs) {
        AsyncChannel* ac = (AsyncChannel*)wrs.Get();
        Int32 result = ac->ConnectSrcHandlerToPackageSync(mSrcCtx, mSrcHdlr, mDstPackageName,
                mDstClassName);
        ac->ReplyHalfConnected(result);
    }
    return NOERROR;
}

//=============================================
// AsyncChannel
//=============================================
CAR_INTERFACE_IMPL(AsyncChannel, IWeakReferenceSource)

AsyncChannel::AsyncChannel()
{
    Slogger::D("AsyncChannel", " == AsyncChannel(): %p", this);
}

AsyncChannel::~AsyncChannel()
{
    Slogger::D("AsyncChannel", " == ~AsyncChannel(): %p", this);
}

ECode AsyncChannel::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

Int32 AsyncChannel::ConnectSrcHandlerToPackageSync(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ const String& dstPackageName,
    /* [in] */ const String& dstClassName)
{
    if (DBG) {
        Slogger::D("AsyncChannel", " +++ ConnectSrcHandlerToPackageSync srcHandler to dst Package %s & class %s",
            dstPackageName.string(), dstClassName.string());
    }

    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    mConnection = new AsyncChannelConnection(wr);

    /* Initialize the source information */
    mSrcContext = srcContext;
    mSrcHandler = srcHandler;
    mSrcMessenger = NULL;
    CMessenger::New(mSrcHandler, (IMessenger**)&mSrcMessenger);

    /*
     * Initialize destination information to null they will
     * be initialized when the AsyncChannelConnection#onServiceConnected
     * is called
     */
    mDstMessenger = NULL;

    /* Send intent to create the connection */
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_MAIN, (IIntent**)&intent);
    intent->SetClassName(dstPackageName, dstClassName);
    Boolean result;
    srcContext->BindService(intent, mConnection, IContext::BIND_AUTO_CREATE, &result);
    if (DBG) {
        Slogger::D("AsyncChannel", " --- ConnectSrcHandlerToPackageSync srcHandler to dst Package %s & class %s, result=%d",
            dstPackageName.string(), dstClassName.string(), result);
    }
    return result ? STATUS_SUCCESSFUL : STATUS_BINDING_UNSUCCESSFUL;
}

Int32 AsyncChannel::FullyConnectSync(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ IHandler* dstHandler)
{
    Int32 status = ConnectSync(srcContext, srcHandler, dstHandler);
    if (status == STATUS_SUCCESSFUL) {
        Slogger::D(TAG, " FullyConnectSync == CMD_CHANNEL_FULL_CONNECTION");
        AutoPtr<IMessage> response = SendMessageSynchronously(CMD_CHANNEL_FULL_CONNECTION);
        response->GetArg1(&status);
    }
    return status;
}

void AsyncChannel::Connect(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ const String& dstPackageName,
    /* [in] */ const String& dstClassName)
{
    if (DBG) {
        Slogger::D("AsyncChannel", " +++ Connect srcHandler to dst Package %s & class %s",
            dstPackageName.string(), dstClassName.string());
    }

    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    AutoPtr<IRunnable> ca = new ConnectAsyncRunnable(wr, srcContext, srcHandler, dstPackageName, dstClassName);
    AutoPtr<IThread> thread;
    CThread::New(ca, (IThread**)&thread);
    thread->Start();

    if (DBG) {
        Slogger::D("AsyncChannel", " --- Connect srcHandler to dst Package %s & class %s",
            dstPackageName.string(), dstClassName.string());
    }
}

Int32 AsyncChannel::ConnectSync(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ IHandler* dstHandler)
{
    AutoPtr<IMessenger> msger;
    CMessenger::New(dstHandler, (IMessenger**)&msger);
    return ConnectSync(srcContext, srcHandler, msger);
}

Int32 AsyncChannel::ConnectSync(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ IMessenger* dstMessenger)
{
    if (DBG) Slogger::D("AsyncChannel", " +++ halfConnectSync srcHandler to the dstMessenger  E");

    // We are connected
    Connected(srcContext, srcHandler, dstMessenger);

    if (DBG) Slogger::D("AsyncChannel", " --- halfConnectSync srcHandler to the dstMessenger X");
    return STATUS_SUCCESSFUL;
}

void AsyncChannel::Connect(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ IMessenger* dstMessenger)
{
    if (DBG) Slogger::D("AsyncChannel", " +++ Connect srcHandler to the dstMessenger");

    // We are connected
    Connected(srcContext, srcHandler, dstMessenger);

    // Tell source we are half connected
    ReplyHalfConnected(STATUS_SUCCESSFUL);

    if (DBG) Slogger::D("AsyncChannel", " --- Connect srcHandler to the dstMessenger");
}

void AsyncChannel::Connected(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ IMessenger* dstMessenger)
{
    if (DBG) Slogger::D("AsyncChannel", " +++ Connected srcHandler to the dstMessenger  E");

    // Initialize source fields
    mSrcContext = srcContext;
    mSrcHandler = srcHandler;
    mSrcMessenger = NULL;
    CMessenger::New(mSrcHandler, (IMessenger**)&mSrcMessenger);

    // Initialize destination fields
    mDstMessenger = dstMessenger;
    if (mDstMessenger == NULL) {
        Slogger::E("AsyncChannel", "connected srcHandler to null dstMessenger!");
    }

    if (DBG) Slogger::D("AsyncChannel", " --- Connected srcHandler to the dstMessenger X");
}

void AsyncChannel::Connect(
    /* [in] */ IContext* srcContext,
    /* [in] */ IHandler* srcHandler,
    /* [in] */ IHandler* dstHandler)
{
    AutoPtr<IMessenger> mgr;
    CMessenger::New(dstHandler, (IMessenger**)&mgr);
    Connect(srcContext, srcHandler, mgr);
}

void AsyncChannel::SendMessage(
    /* [in] */ IMessage* msg)
{
    msg->SetReplyTo(mSrcMessenger);
    // try {
        if (mDstMessenger != NULL) {
            ECode ec = mDstMessenger->Send(msg);
            if (FAILED(ec)) {
                ReplyDisconnected(STATUS_SEND_UNSUCCESSFUL);
            }
        }
    // } catch (RemoteException e) {
    //     replyDisconnected(STATUS_SEND_UNSUCCESSFUL);
    // }
}

void AsyncChannel::Disconnected()
{
    mSrcContext = NULL;
    mSrcHandler = NULL;
    mSrcMessenger = NULL;
    mDstMessenger = NULL;
    mConnection = NULL;
}

void AsyncChannel::Disconnect()
{
    if (DBG) Slogger::D("AsyncChannel", " >> AsyncChannel::Disconnect()");
    if ((mConnection != NULL) && (mSrcContext != NULL)) {
        mSrcContext->UnbindService(mConnection);
    }

    // try {
        // Send the DISCONNECTED, although it may not be received
        // but its the best we can do.
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);
    msg->SetWhat(CMD_CHANNEL_DISCONNECTED);
    msg->SetReplyTo(mSrcMessenger);
    mDstMessenger->Send(msg);
    // } catch(Exception e) {
    // }

    // Tell source we're disconnected.
    if (mSrcHandler != NULL) {
        ReplyDisconnected(STATUS_SUCCESSFUL);
    }
}

void AsyncChannel::SendMessage(
    /* [in] */ Int32 what)
{
    SendMessage(what, 0);
}

void AsyncChannel::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1)
{
    SendMessage(what, arg1, 0);
}

void AsyncChannel::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    SendMessage(what, arg1, 0, NULL);
}

void AsyncChannel::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);
    msg->SetWhat(what);
    msg->SetArg1(arg1);
    msg->SetArg2(arg2);
    msg->SetObj(obj);

    SendMessage(msg);
}

void AsyncChannel::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    SendMessage(what, 0, 0, obj);
}

void AsyncChannel::ReplyToMessage(
    /* [in] */ IMessage* srcMsg,
    /* [in] */ IMessage* dstMsg)
{
    // try {
    dstMsg->SetReplyTo(mSrcMessenger);
    AutoPtr<IMessenger> replyTo;
    srcMsg->GetReplyTo((IMessenger**)&replyTo);
    replyTo->Send(dstMsg);
    // } catch (RemoteException e) {
    //     log("TODO: handle replyToMessage RemoteException" + e);
    //     e.printStackTrace();
    // }
}

void AsyncChannel::ReplyToMessage(
    /* [in] */ IMessage* srcMsg,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);
    msg->SetWhat(what);
    msg->SetArg1(arg1);
    ReplyToMessage(srcMsg, msg);
}

void AsyncChannel::ReplyToMessage(
    /* [in] */ IMessage* srcMsg,
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);
    msg->SetWhat(what);
    msg->SetObj(obj);
    ReplyToMessage(srcMsg, msg);
}

AutoPtr<IMessage> AsyncChannel::SendMessageSynchronously(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IMessage> resultMsg = SyncMessenger::SendMessageSynchronously(mDstMessenger, msg);
    return resultMsg;
}

AutoPtr<IMessage> AsyncChannel::SendMessageSynchronously(
    /* [in] */ Int32 what)
{
    return SendMessageSynchronously(what, 0);
}

AutoPtr<IMessage> AsyncChannel::SendMessageSynchronously(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1)
{
    return SendMessageSynchronously(what, arg1, 0);
}

AutoPtr<IMessage> AsyncChannel::SendMessageSynchronously(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);
    msg->SetWhat(what);
    msg->SetArg1(arg1);
    msg->SetArg2(arg2);
    AutoPtr<IMessage> resultMsg = SendMessageSynchronously(msg);
    return resultMsg;
}

AutoPtr<IMessage> AsyncChannel::SendMessageSynchronously(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);
    msg->SetWhat(what);
    msg->SetObj(obj);
    AutoPtr<IMessage> resultMsg = SendMessageSynchronously(msg);
    return resultMsg;
}

void AsyncChannel::ReplyHalfConnected(
    /* [in] */ Int32 status)
{
    AutoPtr<IMessage> msg;
    mSrcHandler->ObtainMessage(CMD_CHANNEL_HALF_CONNECTED, (IMessage**)&msg);
    msg->SetArg1(status);
    msg->SetObj(this);
    msg->SetReplyTo(mDstMessenger);
    Boolean result;
    mSrcHandler->SendMessage(msg, &result);
}

void AsyncChannel::ReplyDisconnected(
    /* [in] */ Int32 status)
{
    AutoPtr<IMessage> msg;
    mSrcHandler->ObtainMessage(CMD_CHANNEL_DISCONNECTED, (IMessage**)&msg);
    msg->SetArg1(status);
    msg->SetObj(this);
    msg->SetReplyTo(mDstMessenger);
    Boolean result;
    mSrcHandler->SendMessage(msg, &result);
}


} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
