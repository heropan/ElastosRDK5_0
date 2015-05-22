
#ifndef __ELASTOS_CORE_OBJECT_H__
#define __ELASTOS_CORE_OBJECT_H__

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#include "coredef.h"
#include "NativeThread.h"
#else
#include "Elastos.Core.h"
#include <elastos/coredef.h>
#include <elastos/core/NativeThread.h>
#endif

using Elastos::Core::Threading::ISynchronize;
using Elastos::Core::Threading::NativeObject;

#define synchronized(obj)   for(Elastos::Core::Object::AutoLock obj##_lock(obj); obj##_lock; obj##_lock.SetUnlock())

namespace Elastos {
namespace Core {

class Object
    : public ElRefBase
    , public IObject
    , public ISynchronize
    , public IWeakReferenceSource
{
public:
    class Autolock
    {
    public:
        inline Autolock(
            /* [in] */ Object& object)
            : mObject(object)
            , mLocked(TRUE)
        {
            mObject.Lock();
        }

        inline Autolock(
            /* [in] */ Object* object)
            : mObject(*object)
        {
            mObject.Lock();
        }

        inline ~Autolock()
        {
            mObject.Unlock();
        }

        // report the state of locking when used as a boolean
        inline operator Boolean () const
        {
            return mLocked;
        }

        // unlock
        inline void SetUnlock()
        {
            mLocked = FALSE;
        }
    private:
        Object& mObject;
        Boolean mLocked;
    };

public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* info);

    CARAPI Lock();

    CARAPI Unlock();

    /**
     * Causes a thread which is waiting on this object's monitor (by means of
     * calling one of the {@code wait()} methods) to be woken up. If more than
     * one thread is waiting, one of them is chosen at the discretion of the
     * virtual machine. The chosen thread will not run immediately. The thread
     * that called {@code notify()} has to release the object's monitor first.
     * Also, the chosen thread still has to compete against other threads that
     * try to synchronize on the same object.
     * <p>
     * This method can only be invoked by a thread which owns this object's
     * monitor. A thread becomes owner of an object's monitor
     * </p>
     * <ul>
     * <li>by executing a synchronized method of that object;</li>
     * <li>by executing the body of a {@code synchronized} statement that
     * synchronizes on the object;</li>
     * <li>by executing a synchronized static method if the object is of type
     * {@code Class}.</li>
     * </ul>
     *
     * @see #notifyAll
     * @see #wait()
     * @see #wait(long)
     * @see #wait(long,int)
     * @see java.lang.Thread
     */
    CARAPI Notify();

    /**
     * Causes all threads which are waiting on this object's monitor (by means
     * of calling one of the {@code wait()} methods) to be woken up. The threads
     * will not run immediately. The thread that called {@code notify()} has to
     * release the object's monitor first. Also, the threads still have to
     * compete against other threads that try to synchronize on the same object.
     * <p>
     * This method can only be invoked by a thread which owns this object's
     * monitor. A thread becomes owner of an object's monitor
     * </p>
     * <ul>
     * <li>by executing a synchronized method of that object;</li>
     * <li>by executing the body of a {@code synchronized} statement that
     * synchronizes on the object;</li>
     * <li>by executing a synchronized static method if the object is of type
     * {@code Class}.</li>
     * </ul>
     *
     * @throws IllegalMonitorStateException
     *             if the thread calling this method is not the owner of this
     *             object's monitor.
     * @see #notify
     * @see #wait()
     * @see #wait(long)
     * @see #wait(long,int)
     * @see java.lang.Thread
     */
    CARAPI NotifyAll();

    /**
     * Causes the calling thread to wait until another thread calls the {@code
     * notify()} or {@code notifyAll()} method of this object. This method can
     * only be invoked by a thread which owns this object's monitor; see
     * {@link #notify()} on how a thread can become the owner of a monitor.
     * <p>
     * A waiting thread can be sent {@code interrupt()} to cause it to
     * prematurely stop waiting, so {@code wait} should be called in a loop to
     * check that the condition that has been waited for has been met before
     * continuing.
     * </p>
     * <p>
     * While the thread waits, it gives up ownership of this object's monitor.
     * When it is notified (or interrupted), it re-acquires the monitor before
     * it starts running.
     * </p>
     *
     * @throws IllegalMonitorStateException
     *             if the thread calling this method is not the owner of this
     *             object's monitor.
     * @throws InterruptedException
     *             if another thread interrupts this thread while it is waiting.
     * @see #notify
     * @see #notifyAll
     * @see #wait(long)
     * @see #wait(long,int)
     * @see java.lang.Thread
     */
    CARAPI Wait();

    /**
     * Causes the calling thread to wait until another thread calls the {@code
     * notify()} or {@code notifyAll()} method of this object or until the
     * specified timeout expires. This method can only be invoked by a thread
     * which owns this object's monitor; see {@link #notify()} on how a thread
     * can become the owner of a monitor.
     * <p>
     * A waiting thread can be sent {@code interrupt()} to cause it to
     * prematurely stop waiting, so {@code wait} should be called in a loop to
     * check that the condition that has been waited for has been met before
     * continuing.
     * </p>
     * <p>
     * While the thread waits, it gives up ownership of this object's monitor.
     * When it is notified (or interrupted), it re-acquires the monitor before
     * it starts running.
     * </p>
     *
     * @param millis
     *            the maximum time to wait in milliseconds.
     * @throws IllegalArgumentException
     *             if {@code millis < 0}.
     * @throws IllegalMonitorStateException
     *             if the thread calling this method is not the owner of this
     *             object's monitor.
     * @throws InterruptedException
     *             if another thread interrupts this thread while it is waiting.
     * @see #notify
     * @see #notifyAll
     * @see #wait()
     * @see #wait(long,int)
     * @see java.lang.Thread
     */
    CARAPI Wait(
        /* [in] */ Int64 millis);

    /**
     * Causes the calling thread to wait until another thread calls the {@code
     * notify()} or {@code notifyAll()} method of this object or until the
     * specified timeout expires. This method can only be invoked by a thread
     * that owns this object's monitor; see {@link #notify()} on how a thread
     * can become the owner of a monitor.
     * <p>
     * A waiting thread can be sent {@code interrupt()} to cause it to
     * prematurely stop waiting, so {@code wait} should be called in a loop to
     * check that the condition that has been waited for has been met before
     * continuing.
     * </p>
     * <p>
     * While the thread waits, it gives up ownership of this object's monitor.
     * When it is notified (or interrupted), it re-acquires the monitor before
     * it starts running.
     * </p>
     *
     * @param millis
     *            the maximum time to wait in milliseconds.
     * @param nanos
     *            the fraction of a millisecond to wait, specified in
     *            nanoseconds.
     * @throws IllegalArgumentException
     *             if {@code millis < 0}, {@code nanos < 0} or {@code nanos >
     *             999999}.
     * @throws IllegalMonitorStateException
     *             if the thread calling this method is not the owner of this
     *             object's monitor.
     * @throws InterruptedException
     *             if another thread interrupts this thread while it is waiting.
     * @see #notify
     * @see #notifyAll
     * @see #wait()
     * @see #wait(long,int)
     * @see java.lang.Thread
     */
    CARAPI Wait(
        /* [in] */ Int64 millis,
        /* [in] */ Int32 nanos);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

protected:
    Object();

    virtual ~Object();

public:
    NativeObject* mNativeObject;
};

} // namespace Core
} // namespace Elastos

#endif //__ELASTOS_CORE_OBJECT_H__
