
#include "ext/frameworkext.h"
#include "os/Looper.h"
#include "os/Binder.h"
#include "os/SystemClock.h"
#include <elastos/Thread.h>
#include <elastos/Slogger.h>
#ifdef DROID_CORE
#include "os/CMessageQueue.h"
#endif

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::IRunnable;
using Elastos::Core::Threading::Thread;

extern "C" pthread_key_t sLooperKey;
extern "C" Boolean sLooperKeyInited;

namespace Elastos {
namespace Droid {
namespace Os {

AutoPtr<ILooper> Looper::sMainLooper;
Mutex Looper::sClassLock;

Looper::Looper(
    /* [in] */ Boolean quitAllowed)
{
    CMessageQueue::New(quitAllowed, (IMessageQueue**)&mQueue);
    mRun = TRUE;
    mThread = Thread::GetCurrentThread();
}

PInterface Looper::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILooper*)this;
    }
    else if (riid == EIID_ILooper) {
        return (ILooper*)this;
    }

    return NULL;
}

UInt32 Looper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Looper::Release()
{
    return ElRefBase::Release();
}

ECode Looper::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

static void LooperDestructor(void* st)
{
    Looper* obj = static_cast<Looper*>(st);
    if (obj) {
        obj->Release();
    }
}

void Looper::InitTLS()
{
    if (!sLooperKeyInited) {
        ASSERT_TRUE(pthread_key_create(&sLooperKey, LooperDestructor) == 0);
        sLooperKeyInited = TRUE;
    }
}

ECode Looper::Prepare()
{
    return Prepare(TRUE);
}

ECode Looper::Prepare(
    /* [in] */ Boolean quitAllowed)
{
    InitTLS();
    if (pthread_getspecific(sLooperKey) != NULL) {
        // throw new RuntimeException("Only one Looper may be created per thread");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<Looper> l = new Looper(quitAllowed);
    ASSERT_TRUE(pthread_setspecific(sLooperKey, l.Get()) == 0);
    l->AddRef();
    return NOERROR;
}

ECode Looper::PrepareMainLooper()
{
    FAIL_RETURN(Prepare(FALSE));
    {
        Mutex::Autolock lock(sClassLock);

        if (sMainLooper != NULL) {
            // throw new IllegalStateException("The main Looper has already been prepared.");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        sMainLooper = MyLooper();
    }
    return NOERROR;
}

AutoPtr<ILooper> Looper::GetMainLooper()
{
    Mutex::Autolock lock(sClassLock);

    return sMainLooper;
}

ECode Looper::Loop()
{
    AutoPtr<Looper> me = (Looper*)MyLooper().Get();
    if (me == NULL) {
        Slogger::E("Looper", "No Looper; Looper.prepare() wasn't called on this thread.");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IMessageQueue> queue = me->mQueue;

    // Make sure the identity of this thread is that of the local process,
    // and keep track of what that identity token actually is.
    Binder::ClearCallingIdentity();
    const Int64 ident = Binder::ClearCallingIdentity();

    for (;;) {
        AutoPtr<IMessage> msg;
        queue->GetNext((IMessage**)&msg); // might block
        if (msg == NULL) {
            // No message indicates that the message queue is quitting.
            return NOERROR;
        }

        // This must be in a local variable, in case a UI event sets the logger
        // Printer logging = me.mLogging;
        // if (logging != null) {
        //     logging.println(">>>>> Dispatching to " + msg.target + " " +
        //             msg.callback + ": " + msg.what);
        // }

        AutoPtr<IHandler> target;
        msg->GetTarget((IHandler**)&target);
        assert(target != NULL);
        if (target != NULL) {
#if 0
            Int32 what;
            msg->GetWhat(&what);
            AutoPtr<IRunnable> r;
            msg->GetCallback((IRunnable**)&r);
            Slogger::D("Looper", " >>>>> Dispatching to %p, msg: %d, callback: %p", target.Get(), what, r.Get());
#endif
            target->DispatchMessage(msg);
        }
        // if (logging != null) {
        //     logging.println("<<<<< Finished to " + msg.target + " " + msg.callback);
        // }

        // Make sure that during the course of dispatching the
        // identity of the thread wasn't corrupted.
        const Int64 newIdent = Binder::ClearCallingIdentity();
        if (ident != newIdent) {
            // Log.wtf(TAG, "Thread identity changed from 0x"
            //         + Long.toHexString(ident) + " to 0x"
            //         + Long.toHexString(newIdent) + " while dispatching to "
            //         + msg.target.getClass().getName() + " "
            //         + msg.callback + " what=" + msg.what);
        }

        msg->Recycle();
    }
}

AutoPtr<ILooper> Looper::MyLooper()
{
    AutoPtr<ILooper> l = (Looper*)pthread_getspecific(sLooperKey);
    return l;
}

ECode Looper::SetCallbackLogging(
    /* [in] */ IPrinter* printer)
{
    mLogging = printer;
    return NOERROR;
}

AutoPtr<IMessageQueue> Looper::MyQueue()
{
    AutoPtr<ILooper> looper = MyLooper();
    return ((Looper*)looper.Get())->mQueue;
}

ECode Looper::Quit()
{
    mQueue->Quit();
    return NOERROR;
}

ECode Looper::PostSyncBarrier(
    /* [in] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mQueue->EnqueueSyncBarrier(SystemClock::GetUptimeMillis(), result);
}

ECode Looper::RemoveSyncBarrier(
    /* [in] */ Int32 token)
{
    return mQueue->RemoveSyncBarrier(token);
}

ECode Looper::GetThread(
    /* [out] */ IThread** thread)
{
    VALIDATE_NOT_NULL(thread);

    *thread = mThread;
    INTERFACE_ADDREF(*thread);
    return NOERROR;
}

AutoPtr<IMessageQueue> Looper::GetQueue()
{
    return mQueue;
}

ECode Looper::GetQueue(
    /* [out] */ IMessageQueue** queue)
{
    VALIDATE_NOT_NULL(queue);
    *queue = mQueue;
    INTERFACE_ADDREF(*queue);
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
