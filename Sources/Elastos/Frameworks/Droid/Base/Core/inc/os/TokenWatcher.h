#ifndef __TOKENWATCHER_H__
#define __TOKENWATCHER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::Mutex;
using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Os {
/**
 * Helper class that helps you use IBinder objects as reference counted
 * tokens.  IBinders make good tokens because we find out when they are
 * removed
 *
 */
class TokenWatcher
{
private:
    class NotificationTaskRunnable
        : public Runnable
    {
    public:
        NotificationTaskRunnable(
            /* [in] */ TokenWatcher* service)
            : mHost(service)
        {}

        CARAPI Run();
    private:
        TokenWatcher* mHost;
    };

    class Death
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        Death(
            /* [in] */ IBinder* token,
            /* [in] */ const String& tag,
            /* [in] */ TokenWatcher* host);

        CAR_INTERFACE_DECL();

        CARAPI ProxyDied();

        ~Death();
    public:
        AutoPtr<IBinder> mToken;
        String mTag;
        TokenWatcher* mHost;
    };

public:
    /**
     * Construct the TokenWatcher
     *
     * @param h A handler to call {@link #acquired} and {@link #released}
     * on.  If you don't care, just call it like this, although your thread
     * will have to be a Looper thread.
     * <code>new TokenWatcher(new Handler())</code>
     * @param tag A debugging tag for this TokenWatcher
     */
    TokenWatcher(
        /* [in] */ IHandler* h,
        /* [in] */ const String& tag);

    /**
     * Called when the number of active tokens goes from 0 to 1.
     */
    virtual CARAPI Acquired() = 0;

    /**
     * Called when the number of active tokens goes from 1 to 0.
     */
    virtual CARAPI Released() = 0;

    /**
     * Record that this token has been acquired.  When acquire is called, and
     * the current count is 0, the acquired method is called on the given
     * handler.
     *
     * @param token An IBinder object.  If this token has already been acquired,
     *              no action is taken.
     * @param tag   A string used by the {@link #dump} method for debugging,
     *              to see who has references.
     */
    virtual CARAPI AcquireToken(
        /* [in] */ IBinder* token,
        /* [in] */ const String& tag);

    virtual CARAPI Cleanup(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean unlink);

    virtual CARAPI ReleaseToken(
        /* [in] */ IBinder* token);

    virtual CARAPI IsAcquired(
        /* [out] */ Boolean* result);

    virtual CARAPI Dump();

    virtual CARAPI Dump(
        /* [in] */ IPrintWriter* pw);

private:
    CARAPI_(AutoPtr< List<String> >) DumpInternal();

    CARAPI NotificationTask();

    CARAPI_(void) SendNotificationLocked(
        /* [in] */ Boolean on);

private:
    HashMap<AutoPtr<IBinder>, AutoPtr<Death> > mTokens;
    Mutex mLock;
    AutoPtr<IRunnable> mNotificationTask;
    AutoPtr<IHandler> mHandler;
    String mTag;
    Int32 mNotificationQueue;
    Boolean mAcquired;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__TOKENWATCHER_H__
