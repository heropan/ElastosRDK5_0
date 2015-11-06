
#include "elastos/droid/app/AppImportanceMonitor.h"

namespace Elastos {
namespace Droid {
namespace App {

// using Elastos::Droid::Content::IContext;
// using Elastos::Droid::Os::IHandler;
// using Elastos::Droid::Os::ILooper;
// using Elastos::Droid::Os::IMessage;

/**
 * Helper for monitoring the current importance of applications.
 * @hide
 */
class AppImportanceMonitor
    : public Object
    , public IAppImportanceMonitor
{
private:
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
            uid = _uid;
        }
    };

    class ProcessObserver
        : public Object
        , public IProcessObserver
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        virtual ~ProcessObserver();

        CARAPI OnForegroundActivitiesChanged(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [in] */ Boolean foregroundActivities)
        {
        }

        CARAPI OnProcessStateChanged(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 procState)
        {
            synchronized (mApps) {
                updateImportanceLocked(pid, uid,
                        ActivityManager.RunningAppProcessInfo.procStateToImportance(procState),
                        true);
            }
        }

        CARAPI OnProcessDied(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid)
        {
            synchronized (mApps) {
                updateImportanceLocked(pid, uid,
                        ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE, true);
            }
        }
    };

public:
    CAR_INTERFACE_DECL()

    AppImportanceMonitor();

    virtual ~AppImportanceMonitor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* looper)
    {
        mContext = context;
        mHandler = new Handler(looper) {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case MSG_UPDATE:
                        onImportanceChanged(msg.arg1, msg.arg2&0xffff, msg.arg2>>16);
                        break;
                    default:
                        super.handleMessage(msg);
                }
            }
        };
        ActivityManager am = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
        try {
            ActivityManagerNative.getDefault().registerProcessObserver(mProcessObserver);
        } catch (RemoteException e) {
        }
        List<ActivityManager.RunningAppProcessInfo> apps = am.getRunningAppProcesses();
        if (apps != null) {
            for (int i=0; i<apps.size(); i++) {
                ActivityManager.RunningAppProcessInfo app = apps.get(i);
                updateImportanceLocked(app.uid, app.pid, app.importance, false);
            }
        }
    }

    public CARAPI GetImportance(
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result)
    {
        AppEntry ent = mApps.get(uid);
        if (ent == null) {
            return ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE;
        }
        return ent.importance;
    }

    /**
     * Report when an app's importance changed. Called on looper given to constructor.
     */
    CARAPI OnImportanceChanged(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 importance,
        /* [in] */ Int32 oldImportance)
    {
    }

    void UpdateImportanceLocked(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 importance,
        /* [in] */ Boolean repChange)
    {
        AppEntry ent = mApps.get(uid);
        if (ent == null) {
            ent = new AppEntry(uid);
            mApps.put(uid, ent);
        }
        if (importance >= ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE) {
            ent.procs.remove(pid);
        } else {
            ent.procs.put(pid, importance);
        }
        updateImportanceLocked(ent, repChange);
    }

    void UpdateImportanceLocked(
        /* [in] */ AppEntry* ent,
        /* [in] */ Boolean repChange)
    {
        int appImp = ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE;
        for (int i=0; i<ent.procs.size(); i++) {
            int procImp = ent.procs.valueAt(i);
            if (procImp < appImp) {
                appImp = procImp;
            }
        }
        if (appImp != ent.importance) {
            int impCode = appImp | (ent.importance<<16);
            ent.importance = appImp;
            if (appImp >= ActivityManager.RunningAppProcessInfo.IMPORTANCE_GONE) {
                mApps.remove(ent.uid);
            }
            if (repChange) {
                mHandler.obtainMessage(MSG_UPDATE, ent.uid, impCode).sendToTarget();
            }
        }
    }

private:
    AutoPtr<IContext> mContext;

    HashMap<Int32, AutoPtr<AppEntry> > mApps;

    AutoPtr<IProcessObserver> mProcessObserver;

    static const Int32 MSG_UPDATE = 1;

    AutoPtr<IHandler> mHandler;
};


} // namespace App
} // namespace Droid
} // namespace Elastos
