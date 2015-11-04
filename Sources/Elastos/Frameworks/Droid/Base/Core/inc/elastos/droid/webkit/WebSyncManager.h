
#ifndef __ELASTOS_DROID_WEBKIT_WEBSYNCMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_WEBSYNCMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::Runnable;
using Elastos::Core::IRunnable;
using Elastos::Core::IThread;

namespace Elastos {
namespace Droid {
namespace Webkit {

class WebViewDatabase;

class WebSyncManager : public Runnable
{
private:
    class SyncHandler : public Handler
    {
    public:
        SyncHandler(
            /* [in] */ WebSyncManager* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        WebSyncManager* mOwner;
    };

public:
    CARAPI Run();

    /**
     * sync() forces sync manager to sync now
     */
    virtual CARAPI Sync();

    /**
     * resetSync() resets sync manager's timer
     */
    virtual CARAPI ResetSync();

    /**
     * startSync() requests sync manager to start sync
     */
    virtual CARAPI StartSync();

    /**
     * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
     * the queue to break the sync loop
     */
    virtual CARAPI StopSync();

    virtual CARAPI_(void) SyncFromRamToFlash() = 0;

    CARAPI ToString(
        /* [out] */ String* info);

protected:
    WebSyncManager();

    WebSyncManager(
        /* [in] */ IContext* context,
        /* [in] */ const String& name);

    virtual CARAPI_(AutoPtr<IInterface>) Clone();

    virtual CARAPI_(void) OnSyncInit();

protected:
    // handler of the sync thread
    AutoPtr<IHandler> mHandler;

    // database for the persistent storage
    IWebViewDatabase* mDataBase;
    // log tag
    static const String LOGTAG;

private:
    // message code for sync message
    static const Int32 SYNC_MESSAGE;
    // time delay in millisec for a sync (now) message
    static Int32 SYNC_NOW_INTERVAL;
    // time delay in millisec for a sync (later) message
    static Int32 SYNC_LATER_INTERVAL;
    // thread for syncing
    AutoPtr<IThread> mSyncThread;

    // Name of thread
    String mThreadName;
    // Ref count for calls to start/stop sync
    Int32 mStartSyncRefCount;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBSYNCMANAGER_H__
