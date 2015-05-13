
#include "os/Handler.h"
#include "os/SystemClock.h"
#ifdef DROID_CORE
#include "os/CMessengerImpl.h"
#include "os/CMessageHelper.h"
#include "os/CLooperHelper.h"
#endif

#include <elastos/Logger.h>
#include <elastos/StringUtils.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Threading::EIID_ISynchronize;

namespace Elastos {
namespace Droid {
namespace Os {

//===========================================================================
// Handler::BlockingRunnable
//===========================================================================
Handler::BlockingRunnable::BlockingRunnable(
    /* [in] */ IRunnable* task)
    : mTask(task)
    , mDone(FALSE)
{}

PInterface Handler::BlockingRunnable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    else if (riid == EIID_ISynchronize) {
        return (ISynchronize*)this;
    }

    return NULL;
}

UInt32 Handler::BlockingRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Handler::BlockingRunnable::Release()
{
    return ElRefBase::Release();
}

ECode Handler::BlockingRunnable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else if (pObject == (IInterface*)(ISynchronize*)this) {
        *pIID = EIID_ISynchronize;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode Handler::BlockingRunnable::Lock()
{
    return Object::Lock();
}

ECode Handler::BlockingRunnable::Unlock()
{
    return Object::Unlock();
}

ECode Handler::BlockingRunnable::Notify()
{
    return Object::Notify();
}

ECode Handler::BlockingRunnable::NotifyAll()
{
    return Object::NotifyAll();
}

ECode Handler::BlockingRunnable::Wait()
{
    return Object::Wait();
}

ECode Handler::BlockingRunnable::WaitEx(
    /* [in] */ Int64 millis)
{
    return Object::Wait(millis);
}

ECode Handler::BlockingRunnable::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Object::Wait(millis, nanos);
}

ECode Handler::BlockingRunnable::Run()
{
    // try {
    mTask->Run();
    // } finally {
    {
        Lock();

        mDone = TRUE;
        NotifyAll();

        Unlock();
    }
    // }
    return NOERROR;
}

Boolean Handler::BlockingRunnable::PostAndWait(
    /* [in] */ Handler* handler,
    /* [in] */ Int64 timeout)
{
    Boolean result;
    if (handler->Post(this, &result), !result) {
        return FALSE;
    }

    {
        Lock();

        if (timeout > 0) {
            const Int64 expirationTime = SystemClock::GetUptimeMillis() + timeout;
            while (!mDone) {
                Int64 delay = expirationTime - SystemClock::GetUptimeMillis();
                if (delay <= 0) {
                    Unlock();
                    return FALSE; // timeout
                }
                // try {
                WaitEx(delay);
                // } catch (InterruptedException ex) {
                // }
            }
        }
        else {
            while (!mDone) {
                // try {
                Wait();
                // } catch (InterruptedException ex) {
                // }
            }
        }

        Unlock();
    }
    return TRUE;
}

//===========================================================================
// Handler
//===========================================================================
Handler::Handler()
    : mTakeStrongRefOfCallback(TRUE)
    , mAsynchronous(FALSE)
{}

Handler::Handler(
    /* [in] */ Boolean async)
    : mTakeStrongRefOfCallback(TRUE)
    , mAsynchronous(async)
{
    Init(async);
}

Handler::Handler(
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
    : mTakeStrongRefOfCallback(takeStrongRefOfCallback)
    , mAsynchronous(async)
{
    Init(callback, takeStrongRefOfCallback, async);
}

Handler::Handler(
    /* [in] */ ILooper* looper,
    /* [in] */ Boolean async)
    : mTakeStrongRefOfCallback(TRUE)
    , mAsynchronous(async)
{
    Init(looper, async);
}

Handler::Handler(
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
    : mTakeStrongRefOfCallback(takeStrongRefOfCallback)
    , mAsynchronous(async)
{
    Init(looper, callback, takeStrongRefOfCallback, async);
}

ECode Handler::RunWithScissors(
    /* [in] */ IRunnable* r,
    /* [in] */ Int64 timeout,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (r == NULL) {
        Logger::E("Handler", "runnable must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (timeout < 0) {
        Logger::E("Handler", "timeout must be non-negative.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    AutoPtr<ILooper> myLooper;
    helper->MyLooper((ILooper**)&myLooper);
    if (myLooper == mLooper) {
        ECode ec = r->Run();
        *result = TRUE;
        return ec;
    }

    AutoPtr<BlockingRunnable> br = new BlockingRunnable(r);
    *result = br->PostAndWait(this, timeout);
    return NOERROR;
}

ECode Handler::ObtainMessage(
    /* [in] */ Int32 what,
    /* [out] */ IMessage** msg)
{
    return ObtainMessageEx3(what, 0, 0, NULL, msg);
}

ECode Handler::ObtainMessageEx(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [out] */ IMessage** msg)
{
    return ObtainMessageEx3(what, 0, 0, obj, msg);
}

ECode Handler::ObtainMessageEx2(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [out] */ IMessage** msg)
{
    return ObtainMessageEx3(what, arg1, arg2, NULL, msg);
}

ECode Handler::ObtainMessageEx3(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    return helper->ObtainEx6(THIS_PROBE(IHandler), what, arg1, arg2, obj, msg);
}

AutoPtr<IMessage> Handler::GetPostMessage(
    /* [in] */ IRunnable* r)
{
    return GetPostMessage(r, NULL);
}

AutoPtr<IMessage> Handler::GetPostMessage(
    /* [in] */ IRunnable* r,
    /* [in] */ IInterface* token)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->ObtainEx2(THIS_PROBE(IHandler), (IMessage**)&msg);
    msg->SetWhat((Int32)r);  // as Id
    msg->SetObj(token);
    msg->SetCallback(r);
    return msg;
}

ECode Handler::Post(
    /* [in] */ IRunnable* action,
    /* [out] */ Boolean* result)
{
   return PostDelayed(action, 0, result);
}

ECode Handler::PostAtTime(
    /* [in] */ IRunnable* action,
    /* [in] */ Int64 uptimeMillis,
    /* [out] */ Boolean* result)
{
   return PostAtTimeEx(action, NULL, uptimeMillis, result);
}

ECode Handler::PostAtTimeEx(
    /* [in] */ IRunnable* action,
    /* [in] */ IInterface* token,
    /* [in] */ Int64 uptimeMillis,
    /* [out] */ Boolean* result)
{
   VALIDATE_NOT_NULL(result);
   AutoPtr<IMessage> msg = GetPostMessage(action, token);
   return SendMessageAtTime(msg, uptimeMillis, result);
}

ECode Handler::PostDelayed(
    /* [in] */ IRunnable* action,
    /* [in] */ Int64 delayMillis,
    /* [out] */ Boolean* result)
{
   VALIDATE_NOT_NULL(result);
   AutoPtr<IMessage> msg = GetPostMessage(action);
   return SendMessageDelayed(msg, delayMillis, result);
}

ECode Handler::PostAtFrontOfQueue(
    /* [in] */ IRunnable* action,
    /* [out] */ Boolean* result)
{
   VALIDATE_NOT_NULL(result);
   AutoPtr<IMessage> msg = GetPostMessage(action);
   return SendMessageAtFrontOfQueue(msg, result);
}

ECode Handler::RemoveCallbacks(
    /* [in] */ IRunnable* action)
{
    return RemoveMessages((Int32)action);
}

ECode Handler::RemoveCallbacksEx(
    /* [in] */ IRunnable* action,
    /* [in] */ IInterface* token)
{
    return RemoveMessagesEx((Int32)action, token);
}

ECode Handler::GetIMessenger(
    /* [out] */ IIMessenger** mgr)
{
    VALIDATE_NOT_NULL(mgr);

    Mutex::Autolock lock(mQueueLock);

    if (mMessenger == NULL) {
        CMessengerImpl::New(THIS_PROBE(IHandler), (IIMessenger**)&mMessenger);
    }
    *mgr = mMessenger;
    INTERFACE_ADDREF(*mgr);
    return NOERROR;
}

ECode Handler::SendEmptyMessage(
    /* [in] */ Int32 what,
    /* [out] */ Boolean* result)
{
    return Handler::SendEmptyMessageDelayed(what, 0, result);
}

ECode Handler::SendEmptyMessageDelayed(
    /* [in] */ Int32 what,
    /* [in] */ Int64 delayMillis,
    /* [out] */ Boolean* result)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->ObtainEx2(THIS_PROBE(IHandler), (IMessage**)&msg);
    msg->SetWhat(what);
    return Handler::SendMessageDelayed(msg, delayMillis, result);
}

ECode Handler::SendEmptyMessageAtTime(
    /* [in] */ Int32 what,
    /* [in] */ Int64 uptimeMillis,
    /* [out] */ Boolean* result)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->ObtainEx2(THIS_PROBE(IHandler), (IMessage**)&msg);
    msg->SetWhat(what);
    return Handler::SendMessageAtTime(msg, uptimeMillis, result);
}

ECode Handler::SendMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    return Handler::SendMessageDelayed(msg, 0, result);
}

ECode Handler::SendMessageDelayed(
    /* [in] */ IMessage* msg,
    /* [in] */ Int64 delayMillis,
    /* [out] */ Boolean* result)
{
    if (delayMillis < 0) {
        delayMillis = 0;
    }

    return SendMessageAtTime(msg, SystemClock::GetUptimeMillis() + delayMillis, result);
}

ECode Handler::SendMessageAtTime(
    /* [in] */ IMessage* msg,
    /* [in] */ Int64 uptimeMillis,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(msg);

    AutoPtr<IMessageQueue> queue = mQueue;
    if (queue == NULL) {
        Logger::E("Handler", "Error: SendMessageAtTime called with no mQueue");
        return E_RUNTIME_EXCEPTION;
    }

    *result = EnqueueMessage(queue, msg, uptimeMillis);
    return NOERROR;
}

ECode Handler::SendMessageAtFrontOfQueue(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(msg);

    AutoPtr<IMessageQueue> queue = mQueue;
    if (queue == NULL) {
        Logger::E("Handler", "Error: SendMessageAtFrontOfQueue called with no mQueue");
        return E_RUNTIME_EXCEPTION;
    }

    *result = EnqueueMessage(queue, msg, 0);
    return NOERROR;
}

Boolean Handler::EnqueueMessage(
    /* [in] */ IMessageQueue* queue,
    /* [in] */ IMessage* msg,
    /* [in] */ Int64 uptimeMillis)
{
    msg->SetTarget(THIS_PROBE(IHandler));
    if (mAsynchronous) {
        msg->SetAsynchronous(true);
    }

    Boolean result;
    mQueue->EnqueueMessage(msg, uptimeMillis, &result);
    return result;
}

ECode Handler::HasCallbacks(
    /* [in] */ IRunnable* action,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IMessageQueue> queue = mQueue;
    if (queue == NULL) {
        Logger::E("Handler", "Error: HasCallbacks called with no mQueue");
        return E_RUNTIME_EXCEPTION;
    }

    return queue->HasMessagesEx(THIS_PROBE(IHandler), action, NULL, result);
}

ECode Handler::HasMessages(
    /* [in] */ Int32 what,
    /* [out] */ Boolean* result)
{
    return HasMessagesEx3(what, what, NULL, result);
}

ECode Handler::HasMessagesEx(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return HasMessagesEx3(what, what, obj, result);
}

ECode Handler::HasMessagesEx2(
    /* [in] */ Int32 id,
    /* [in] */ Int32 what,
    /* [out] */ Boolean* result)
{
    return HasMessagesEx3(id, what, NULL, result);
}

ECode Handler::HasMessagesEx3(
    /* [in] */ Int32 id,
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IMessageQueue> queue = mQueue;
    if (queue == NULL) {
        Logger::E("Handler", "Error: HasMessagesEx3 called with no mQueue");
        return E_RUNTIME_EXCEPTION;
    }

    return queue->HasMessages(THIS_PROBE(IHandler), what, obj, result);
}

ECode Handler::RemoveMessages(
    /* [in] */ Int32 what)
{
    return RemoveMessagesEx2(what, what, NULL);
}

ECode Handler::RemoveMessagesEx(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    return RemoveMessagesEx2(what, what, obj);
}

ECode Handler::RemoveMessagesEx2(
    /* [in] */ Int32 id,
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageQueue> queue = mQueue;
    if (queue == NULL) {
        Logger::E("Handler", "Error: RemoveMessagesEx2 called with no mQueue");
        return E_RUNTIME_EXCEPTION;
    }

    return queue->RemoveMessages(THIS_PROBE(IHandler), what, obj);
}

/**
 * Remove any pending posts of callbacks and sent messages whose
 * <var>obj</var> is <var>token</var>.  If <var>token</var> is null,
 * all callbacks and messages will be removed.
 */
ECode Handler::RemoveCallbacksAndMessages(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessageQueue> queue = mQueue;
    if (queue == NULL) {
        Logger::E("Handler", "Error: RemoveCallbacksAndMessages called with no mQueue");
        return E_RUNTIME_EXCEPTION;
    }

    return queue->RemoveCallbacksAndMessages(THIS_PROBE(IHandler), obj);
}

ECode Handler::GetMessageName(
    /* [in] */ IMessage* msg,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = GetMessageNameImpl(msg);
    return NOERROR;
}

String Handler::GetMessageNameImpl(
    /* [in] */ IMessage* msg)
{
    if (msg == NULL) {
        return String("Null message");
    }

    AutoPtr<IRunnable> callback;
    msg->GetCallback((IRunnable**)&callback);
    if (callback != NULL) {
        // return message.callback.getClass().getName();
        String name("Runable:0x");
        name += StringUtils::Int32ToHexString((Int32)callback.Get());
        return name;
    }

    Int32 what = 0;
    msg->GetWhat(&what);
    String name("Message:0x");
    name += StringUtils::Int32ToHexString(what);
    return name;
}

/**
 * Subclasses must implement this to receive messages.
 */
ECode Handler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return NOERROR;
}

/**
 * Handle system messages here.
 */
ECode Handler::DispatchMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IRunnable> callback;
    msg->GetCallback((IRunnable**)&callback);
    if (callback != NULL) {
        return callback->Run();
    }
    else {
        AutoPtr<IHandlerCallback> handlerCallBack = mCallback;
        if (handlerCallBack == NULL && mWeakCallback != NULL) {
            AutoPtr<IInterface> obj;
            mWeakCallback->Resolve(EIID_IInterface, (IInterface**)&obj);
            handlerCallBack = IHandlerCallback::Probe(obj);
            assert(handlerCallBack != NULL);
        }

        if (handlerCallBack != NULL) {
            Boolean result = FALSE;
            ECode ec = handlerCallBack->HandleMessage(msg, &result);
            if (result) {
                return ec;
            }
        }

        return HandleMessage(msg);
    }
}

ECode Handler::Init(
    /* [in] */ Boolean async)
{
    return Init(NULL, TRUE, async);
}

ECode Handler::Init(
    /* [in] */ ILooper* looper,
    /* [in] */ Boolean async)
{
    assert(looper != NULL);
    mLooper = looper;
    mLooper->GetQueue((IMessageQueue**)&mQueue);
    mTakeStrongRefOfCallback = TRUE;
    mAsynchronous = async;
    return NOERROR;
}

ECode Handler::Init(
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
{
    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    helper->MyLooper((ILooper**)&mLooper);
    if (mLooper == NULL) {
        Logger::E("Handler", "Can't create handler inside thread that has not called Looper.prepare()");
        assert(0);
        return E_RUNTIME_EXCEPTION;
    }

    mLooper->GetQueue((IMessageQueue**)&mQueue);
    mTakeStrongRefOfCallback = takeStrongRefOfCallback;
    mAsynchronous = async;

    if (callback) {
        if (mTakeStrongRefOfCallback) {
            mCallback = callback;
        }
        else {
            AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)callback->Probe(EIID_IWeakReferenceSource);
            if (wrs) {
                wrs->GetWeakReference((IWeakReference**)&mWeakCallback);
            }
            else {
                assert(0 && "IWeakReferenceSource was not impelemented!");
            }
        }
    }
    return NOERROR;
}

ECode Handler::Init(
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallback* callback,
    /* [in] */ Boolean takeStrongRefOfCallback,
    /* [in] */ Boolean async)
{
    assert(looper != NULL);
    mLooper = looper;
    mLooper->GetQueue((IMessageQueue**)&mQueue);
    mTakeStrongRefOfCallback = takeStrongRefOfCallback;
    mAsynchronous = async;

    if (callback) {
        if (mTakeStrongRefOfCallback) {
            mCallback = callback;
        }
        else {
            AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)callback->Probe(EIID_IWeakReferenceSource);
            if (wrs) {
                wrs->GetWeakReference((IWeakReference**)&mWeakCallback);
            }
            else {
                assert(0 && "IWeakReferenceSource was not impelemented!");
            }
        }
    }
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
