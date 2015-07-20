#ifndef __ELASTOS_DROID_OS_CBINDER_H__
#define __ELASTOS_DROID_OS_CBINDER_H__

#include "_Elastos_Droid_Os_CBinder.h"
#include <Elastos.CoreLibrary.h>
#include "os/Binder.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CBinder)
    , public Binder
{
public:

//    /**
//     * Get the canonical name of the interface supported by this binder.
//     */
//    CARAPI GetInterfaceDescriptor(
//        /* [out] */ String* str);
//
//    /**
//     * Check to see if the object still exists.
//     *
//     * @return Returns false if the
//     * hosting process is gone, otherwise the result (always by default
//     * true) returned by the pingBinder() implementation on the other
//     * side.
//     */
//    CARAPI PingBinder(
//        /* [out] */ Boolean* result);
//
//    /**
//     * Check to see if the process that the binder is in is still alive.
//     *
//     * @return false if the process is not alive.  Note that if it returns
//     * true, the process may have died while the call is returning.
//     */
//    CARAPI IsBinderAlive(
//        /* [out] */ Boolean* result);
//
//    /**
//     * Attempt to retrieve a local implementation of an interface
//     * for this Binder object.  If null is returned, you will need
//     * to instantiate a proxy class to marshall calls through
//     * the transact() method.
//     */
//    CARAPI QueryLocalInterface(
//        /* [in] */ const String& descriptor,
//        /* [out] */ IInterface** object);
//
//    /**
//     * Print the object's state into the given stream.
//     *
//     * @param fd The raw file descriptor that the dump is being sent to.
//     * @param args additional arguments to the dump request.
//     */
//    CARAPI Dump(
//        /* [in] */ IFileDescriptor* fd,
//        /* [in] */ ArrayOf<String>* args);
//
//    /**
//     * Like {@link #dump(FileDescriptor, String[])} but always executes
//     * asynchronously.  If the object is local, a new thread is created
//     * to perform the dump.
//     *
//     * @param fd The raw file descriptor that the dump is being sent to.
//     * @param args additional arguments to the dump request.
//     */
//    CARAPI DumpAsync(
//        /* [in] */ IFileDescriptor* fd,
//        /* [in] */ ArrayOf<String>* args);
//
//    /**
//     * Perform a generic operation with the object.
//     *
//     * @param code The action to perform.  This should
//     * be a number between {@link #FIRST_CALL_TRANSACTION} and
//     * {@link #LAST_CALL_TRANSACTION}.
//     * @param data Marshalled data to send to the target.  Must not be null.
//     * If you are not sending any data, you must create an empty Parcel
//     * that is given here.
//     * @param reply Marshalled data to be received from the target.  May be
//     * null if you are not interested in the return value.
//     * @param flags Additional operation flags.  Either 0 for a normal
//     * RPC, or {@link #FLAG_ONEWAY} for a one-way RPC.
//     */
//    CARAPI Transact(
//        /* [in] */ Int32 code,
//        /* [in] */ IParcel* data,
//        /* [in] */ IParcel* reply,
//        /* [in] */ Int32 flags);
//
//    /**
//     * Interface for receiving a callback when the process hosting an IBinder
//     * has gone away.
//     *
//     * @see #linkToDeath
//     */
//    CARAPI DeathRecipient(
//        /* [out] */ IInterface** object);
//
//    /**
//     * Register the recipient for a notification if this binder
//     * goes away.  If this binder object unexpectedly goes away
//     * (typically because its hosting process has been killed),
//     * then the given {@link DeathRecipient}'s
//     * {@link DeathRecipient#binderDied DeathRecipient.binderDied()} method
//     * will be called.
//     *
//     * <p>You will only receive death notifications for remote binders,
//     * as local binders by definition can't die without you dying as well.
//     *
//     * @throws Throws {@link RemoteException} if the target IBinder's
//     * process has already died.
//     *
//     * @see #unlinkToDeath
//     */
//    CARAPI LinkToDeath(
//        /* [in] */ IProxyDeathRecipient* recipient,
//        /* [in] */ Int32 flags);
//
//    /**
//     * Remove a previously registered death notification.
//     * The recipient will no longer be called if this object
//     * dies.
//     *
//     * @return ReturnIDeathRecipients true if the <var>recipient</var> is successfully
//     * unlinked, assuring you that its
//     * {@link DeathRecipient#binderDied DeathRecipient.binderDied()} method
//     * will not be called.  Returns false if the target IBinder has already
//     * died, meaning the method has been (or soon will be) called.
//     *
//     * @throws Throws {@link java.util.NoSuchElementException} if the given
//     * <var>recipient</var> has not been registered with the IBinder, and
//     * the IBinder is still alive.  Note that if the <var>recipient</var>
//     * was never registered, but the IBinder has already died, then this
//     * exception will <em>not</em> be thrown, and you will receive a false
//     * return value instead.
//     */
//    CARAPI UnlinkToDeath(
//        /* [in] */ IProxyDeathRecipient* recipient,
//        /* [in] */ Int32 flags);

    CARAPI ToString(
        /* [out] */ String* str);

    //
    // native methods
    //

    /**
     * Return the ID of the process that sent you the current transaction
     * that is being processed.  This pid can be used with higher-level
     * system services to determine its identity and check permissions.
     * If the current thread is not currently executing an incoming transaction,
     * then its own pid is returned.
     */
    static CARAPI_(Int32) GetCallingPid();

    /**
     * Return the Linux uid assigned to the process that sent you the
     * current transaction that is being processed.  This uid can be used with
     * higher-level system services to determine its identity and check
     * permissions.  If the current thread is not currently executing an
     * incoming transaction, then its own uid is returned.
     */
    static CARAPI_(Int32) GetCallingUid();

    /**
     * Return the UserHandle assigned to the process that sent you the
     * current transaction that is being processed.  This is the user
     * of the caller.  It is distinct from {@link #getCallingUid()} in that a
     * particular user will have multiple distinct apps running under it each
     * with their own uid.  If the current thread is not currently executing an
     * incoming transaction, then its own UserHandle is returned.
     */
    //static CARAPI_(AutoPtr<IUserHandle>) GetCallingUserHandle();

    /**
     * Reset the identity of the incoming IPC on the current thread.  This can
     * be useful if, while handling an incoming call, you will be calling
     * on interfaces of other objects that may be local to your process and
     * need to do permission checks on the calls coming into them (so they
     * will check the permission of your own local process, and not whatever
     * process originally called you).
     *
     * @return Returns an opaque token that can be used to restore the
     * original calling identity by passing it to
     * {@link #restoreCallingIdentity(long)}.
     *
     * @see #getCallingPid()
     * @see #getCallingUid()
     * @see #restoreCallingIdentity(long)
     */
    static CARAPI_(Int64) ClearCallingIdentity();

    /**
     * Restore the identity of the incoming IPC on the current thread
     * back to a previously identity that was returned by {@link
     * #clearCallingIdentity}.
     *
     * @param token The opaque token that was previously returned by
     * {@link #clearCallingIdentity}.
     *
     * @see #clearCallingIdentity
     */
    static CARAPI RestoreCallingIdentity(
        /* [in] */ Int64 token);

    /**
     * Sets the native thread-local StrictMode policy mask.
     *
     * <p>The StrictMode settings are kept in two places: a Java-level
     * threadlocal for libcore/Dalvik, and a native threadlocal (set
     * here) for propagation via Binder calls.  This is a little
     * unfortunate, but necessary to break otherwise more unfortunate
     * dependencies either of Dalvik on Android, or Android
     * native-only code on Dalvik.
     *
     * @see StrictMode
     * @hide
     */
    static CARAPI SetThreadStrictModePolicy(
        /* [in] */ Int32 policyMask);

    /**
     * Gets the current native thread-local StrictMode policy mask.
     *
     * @see #setThreadStrictModePolicy
     * @hide
     */
    static CARAPI_(Int32) GetThreadStrictModePolicy();

    /**
     * Flush any Binder commands pending in the current thread to the kernel
     * driver.  This can be
     * useful to call before performing an operation that may block for a long
     * time, to ensure that any pending object references have been released
     * in order to prevent the process from holding on to objects longer than
     * it needs to.
     */
    static CARAPI FlushPendingCommands();

    /**
     * Add the calling thread to the IPC thread pool.  This function does
     * not return until the current process is exiting.
     */
    static CARAPI JoinThreadPool();
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CBINDER_H__
