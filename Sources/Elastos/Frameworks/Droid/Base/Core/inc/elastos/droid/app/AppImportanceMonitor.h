
#ifndef __ELASTOS_DROID_APP_APP_IMPORTANCE_MONITOR_H__
#define __ELASTOS_DROID_APP_APP_IMPORTANCE_MONITOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/ViewGroup.h"
#include "elastos/droid/os/Handler.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IContext;
using Elastos::Utility::HashMap;

namespace Elastos {
namespace Droid {
namespace App {


/**
 * Helper for monitoring the current importance of applications.
 * @hide
 */
class AppImportanceMonitor
    : public Object
    , public IAppImportanceMonitor
{
private:
    class MyHandler
        : public Handler
    {
    public:
        MyHandler(
            /* [in] */ AppImportanceMonitor* host,
            /* [in] */ ILooper* looper);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AppImportanceMonitor* mHost;
    };

    class AppEntry
        : public Object
    {
    public:
        Int32 mUid;
        HashMap<Int32, Int32> mProcs;
        Int32 mImportance;

        AppEntry(
            /* [in] */ Int32 uid)
            : mUid(uid)
            , mImportance(IActivityManagerRunningAppProcessInfo.IMPORTANCE_GONE)
        {
        }
    };

    class ProcessObserver
        : public Object
        , public IProcessObserver
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        ProcessObserver(
            /* [in] */ AppImportanceMonitor* host);

        virtual ~ProcessObserver();

        CARAPI OnForegroundActivitiesChanged(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [in] */ Boolean foregroundActivities);

        CARAPI OnProcessStateChanged(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 procState);

        CARAPI OnProcessDied(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid);

    private:
        AppImportanceMonitor* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    AppImportanceMonitor();

    virtual ~AppImportanceMonitor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* looper);

    CARAPI GetImportance(
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

private:
    /**
     * Report when an app's importance changed. Called on looper given to constructor.
     */
    CARAPI OnImportanceChanged(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 importance,
        /* [in] */ Int32 oldImportance);

    void UpdateImportanceLocked(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 importance,
        /* [in] */ Boolean repChange);

    void UpdateImportanceLocked(
        /* [in] */ AppEntry* ent,
        /* [in] */ Boolean repChange);

private:
    friend class MyHandler;
    friend class AppImportanceMonitor;

    AutoPtr<IContext> mContext;

    HashMap<Int32, AutoPtr<AppEntry> > mApps;
    Object mAppsLock;

    AutoPtr<IProcessObserver> mProcessObserver;

    static const Int32 MSG_UPDATE;

    AutoPtr<IHandler> mHandler;
};


} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_APP_IMPORTANCE_MONITOR_H__

