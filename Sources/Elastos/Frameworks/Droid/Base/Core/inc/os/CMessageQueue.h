#ifndef __CMESSAGEQUEUE_H__
#define __CMESSAGEQUEUE_H__

#include "_CMessageQueue.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::IRunnable;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CMessageQueue)
{
public:
    CMessageQueue();

    ~CMessageQueue();

    CARAPI constructor(
        /* [in] */ Boolean quitAllowed);

    /**
     * Add a new {@link IdleHandler} to this message queue.  This may be
     * removed automatically for you by returning false from
     * {@link IdleHandler#queueIdle IdleHandler.queueIdle()} when it is
     * invoked, or explicitly removing it with {@link #removeIdleHandler}.
     *
     * <p>This method is safe to call from any thread.
     *
     * @param handler The IdleHandler to be added.
     */
    CARAPI AddIdleHandler(
        /* [in] */ IIdleHandler* handler);

    /**
     * Remove an {@link IdleHandler} from the queue that was previously added
     * with {@link #addIdleHandler}.  If the given object is not currently
     * in the idle list, nothing is done.
     *
     * @param handler The IdleHandler to be removed.
     */
    CARAPI RemoveIdleHandler(
        /* [in] */ IIdleHandler* handler);

    CARAPI GetNext(
        /* [out] */ IMessage** msg);

    CARAPI Quit();

    CARAPI EnqueueMessage(
        /* [in] */ IMessage* msg,
        /* [in] */ Int64 when,
        /* [out] */ Boolean* result);

    CARAPI HasMessages(
        /* [in] */ IHandler* h,
        /* [in] */ Int32 what,
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI HasMessages(
        /* [in] */ IHandler* h,
        /* [in] */ IRunnable* r,
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI RemoveMessages(
        /* [in] */ IHandler* h,
        /* [in] */ Int32 what,
        /* [in] */ IInterface* object);

    CARAPI RemoveMessages(
        /* [in] */ IHandler* h,
        /* [in] */ IRunnable* r,
        /* [in] */ IInterface* object);

    CARAPI RemoveCallbacksAndMessages(
        /* [in] */ IHandler* h,
        /* [in] */ IInterface* object);

    CARAPI GetNativeMessageQueue(
        /* [out] */ Handle32* queue);

    CARAPI RemoveSyncBarrier(
        /* [in] */ Int32 token);

    CARAPI EnqueueSyncBarrier(
        /* [in] */ Int64 when,
        /* [out] */ Int32* result);

private:

    CARAPI NativeInit();

    CARAPI_(void) NativeDestroy();

    CARAPI_(void) NativePollOnce(
        /* [in] */ Int32 timeoutMillis);

    CARAPI_(void) NativeWake();

    CARAPI DebugMessage(
        /* [in] */ IMessage* msg,
        /* [in] */ const char* info);
private:
    static const String TAG;
    static const Boolean DBG;

    // True if the message queue can be quit.
    Boolean mQuitAllowed;

    Handle32 mPtr; // used by native code

    AutoPtr<IMessage> mMessages;
    List<AutoPtr<IIdleHandler> > mIdleHandlers;
    AutoPtr<ArrayOf<IIdleHandler*> > mPendingIdleHandlers;
    Boolean mQuiting;

    // Indicates whether next() is blocked waiting in pollOnce() with a non-zero timeout.
    Boolean mBlocked;

    // The next barrier token.
    // Barriers are indicated by messages with a null target whose arg1 field carries the token.
    Int32 mNextBarrierToken;

    Object mLock;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CMESSAGEQUEUE_H__
