
#ifndef __ELASTOS_DROID_OS_CREMOTECALLBACKLIST_H__
#define __ELASTOS_DROID_OS_CREMOTECALLBACKLIST_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_Os_CRemoteCallbackList.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Takes care of the grunt work of maintaining a list of remote interfaces,
 * typically for the use of performing callbacks from a
 * {@link android.app.Service} to its clients.  In particular, this:
 *
 * <ul>
 * <li> Keeps track of a set of registered {@link IInterface} callbacks,
 * taking care to identify them through their underlying unique {@link IBinder}
 * (by calling {@link IInterface#asBinder IInterface.asBinder()}.
 * <li> Attaches a {@link IBinder.DeathRecipient IBinder.DeathRecipient} to
 * each registered interface, so that it can be cleaned out of the list if its
 * process goes away.
 * <li> Performs locking of the underlying list of interfaces to deal with
 * multithreaded incoming calls, and a thread-safe way to iterate over a
 * snapshot of the list without holding its lock.
 * </ul>
 *
 * <p>To use this class, simply create a single instance along with your
 * service, and call its {@link #register} and {@link #unregister} methods
 * as client register and unregister with your service.  To call back on to
 * the registered clients, use {@link #beginBroadcast},
 * {@link #getBroadcastItem}, and {@link #finishBroadcast}.
 *
 * <p>If a registered callback's process goes away, this class will take
 * care of automatically removing it from the list.  If you want to do
 * additional work in this situation, you can create a subclass that
 * implements the {@link #onCallbackDied} method.
 */
CarClass(CRemoteCallbackList)
{
private:
    class Callback
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL()

        Callback(
            /* [in] */ IInterface* callback,
            /* [in] */ IInterface* cookie,
            /* [in] */ CRemoteCallbackList* owner);

        CARAPI ProxyDied();

    public:
        AutoPtr<IInterface> mCallback;
        AutoPtr<IInterface> mCookie;
        CRemoteCallbackList* mOwner;
    };

public:
    CRemoteCallbackList();

    CARAPI Register(
        /* [in] */ IInterface* callback,
        /* [out] */ Boolean* result);

    CARAPI Register(
        /* [in] */ IInterface* callback,
        /* [in] */ IInterface* cookie,
        /* [out] */ Boolean* result);

    CARAPI Unregister(
        /* [in] */ IInterface* callback,
        /* [out] */ Boolean* result);

    CARAPI Kill();

    CARAPI OnCallbackDied(
        /* [in] */ IInterface* callback);

    CARAPI OnCallbackDied(
        /* [in] */ IInterface* callback,
        /* [in] */ IInterface* cookie);

    CARAPI BeginBroadcast(
        /* [out] */ Int32* number);

    CARAPI GetBroadcastItem(
        /* [in] */ Int32 index,
        /* [out] */ IInterface** callback);

    CARAPI GetBroadcastCookie(
        /* [in] */ Int32 index,
        /* [out] */ IInterface** cookie);

    CARAPI FinishBroadcast();

     /**
     * Returns the number of registered callbacks. Note that the number of registered
     * callbacks may differ from the value returned by {@link #beginBroadcast()} since
     * the former returns the number of callbacks registered at the time of the call
     * and the second the number of callback to which the broadcast will be delivered.
     * <p>
     * This function is useful to decide whether to schedule a broadcast if this
     * requires doing some work which otherwise would not be performed.
     * </p>
     *
     * @return The size.
     */
    CARAPI GetRegisteredCallbackCount(
        /* [out] */ Int32* count);

public:
    /*package*/
    HashMap< AutoPtr<IBinder>, AutoPtr<Callback> > mCallbacks;
    Object mCallbacksLock;

private:
    AutoPtr< ArrayOf<IInterface*> > mActiveBroadcast;
    Int32 mBroadcastCount;
    Boolean mKilled;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CREMOTECALLBACKLIST_H__
