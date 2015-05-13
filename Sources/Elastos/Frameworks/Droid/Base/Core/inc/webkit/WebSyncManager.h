
#ifndef __WEBSYNCMANAGER_H__
#define __WEBSYNCMANAGER_H__

#include "ext/frameworkext.h"
#include "os/HandlerBase.h"

using Elastos::Core::IRunnable;
using Elastos::Core::Threading::IThread;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace Webkit {

class WebViewDatabase;

class WebSyncManager
    : public ElRefBase
    , public IRunnable
{
private:
    class SyncHandler : public HandlerBase
    {
    public:
        SyncHandler(
            /* [in] */ WebSyncManager* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        WebSyncManager* mOwner;
    };

public:
    CARAPI Run();

    CAR_INTERFACE_DECL();

    /**
     * sync() forces sync manager to sync now
     */
    virtual CARAPI_(void) Sync();

    /**
     * resetSync() resets sync manager's timer
     */
    virtual CARAPI_(void) ResetSync();

    /**
     * startSync() requests sync manager to start sync
     */
    virtual CARAPI_(void) StartSync();

    /**
     * stopSync() requests sync manager to stop sync. remove any SYNC_MESSAGE in
     * the queue to break the sync loop
     */
    virtual CARAPI_(void) StopSync();

    virtual CARAPI_(void) SyncFromRamToFlash() = 0;

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

#endif //__WEBSYNCMANAGER_H__
