
#ifndef __ELASTOS_DROID_SERVER_AM_CSERVICERECORD_H__
#define __ELASTOS_DROID_SERVER_AM_CSERVICERECORD_H__

#include "_Elastos_Droid_Server_Am_CServiceRecord.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include "elastos/droid/os/BatteryStatsImpl.h"
#include "elastos/droid/server/am/ConnectionRecord.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Core::IRunnable;
using Elastos::Droid::Os::BatteryStatsImpl;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilterComparison;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::App::INotification;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class AppBindRecord;
class CActivityManagerService;
class NeededUriGrants;
class CServiceRecord;
class IntentBindRecord;
class ProcessRecord;
class UriPermissionOwner;

CarClass(CServiceRecord)
{
public:
    class StartItem : public Object
    {
    public:
        StartItem(
            /* [in] */ CServiceRecord* sr,
            /* [in] */ Boolean taskRemoved,
            /* [in] */ Int32 id,
            /* [in] */ IIntent* intent,
            /* [in] */ NeededUriGrants* neededGrants);

        CARAPI_(AutoPtr<UriPermissionOwner>) GetUriPermissionsLocked();

        CARAPI RemoveUriPermissionsLocked();

    public:
        CServiceRecord* mSr;
        Boolean mTaskRemoved;
        Int32 mId;
        AutoPtr<IIntent> mIntent;
        AutoPtr<NeededUriGrants> mNeededGrants;
        Int64 mDeliveredTime;
        Int32 mDeliveryCount;
        Int32 mDoneExecutingCount;
        AutoPtr<UriPermissionOwner> mUriPermissions;

        String mStringName;      // caching of toString
    };

private:
    class PostNotificationRunnable
        : public Object
        , public IRunnable
    {
    public:
        PostNotificationRunnable(
            /* [in] */ CServiceRecord* host,
            /* [in] */ const String& localPackageName,
            /* [in] */ Int32 appUid,
            /* [in] */ Int32 appPid,
            /* [in] */ Int32 localForegroundId,
            /* [in] */ INotification* localForegroundNoti)
            : mHost(host)
            , mLocalPackageName(localPackageName)
            , mAppUid(appUid)
            , mAppPid(appPid)
            , mLocalForegroundId(localForegroundId)
            , mLocalForegroundNoti(localForegroundNoti)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CServiceRecord* mHost;
        String mLocalPackageName;
        Int32 mAppUid;
        Int32 mAppPid;
        Int32 mLocalForegroundId;
        AutoPtr<INotification> mLocalForegroundNoti;
    };

private:
    class CancelNotificationRunnable
        : public Object
        , public IRunnable
    {
    public:
        CancelNotificationRunnable(
            /* [in] */ CServiceRecord* host,
            /* [in] */ const String& localPackageName,
            /* [in] */ Int32 localForegroundId)
            : mHost(host)
            , mLocalPackageName(localPackageName)
            , mLocalForegroundId(localForegroundId)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CServiceRecord* mHost;
        String mLocalPackageName;
        Int32 mLocalForegroundId;
    };

public:
    CServiceRecord();

    virtual ~CServiceRecord();

    CARAPI Init(
        /* [in] */ CActivityManagerService* ams,
        /* [in] */ BatteryStatsImpl::Uid::Pkg::Serv* servStats,
        /* [in] */ IComponentName* name,
        /* [in] */ IIntentFilterComparison* intent,
        /* [in] */ IServiceInfo* sInfo,
        /* [in] */ IRunnable* restarter);

    // void dumpStartList(PrintWriter pw, String prefix, List<StartItem> list, long now);

    // void dump(PrintWriter pw, String prefix);

    CARAPI_(AutoPtr<AppBindRecord>) RetrieveAppBindingLocked(
        /* [in] */ IIntent* intent,
        /* [in] */ ProcessRecord* app);

    CARAPI ResetRestartCounter();

    CARAPI_(AutoPtr<StartItem>) FindDeliveredStart(
        /* [in] */ Int32 id,
        /* [in] */ Boolean remove);

    CARAPI_(Int32) GetLastStartId();

    CARAPI_(Int32) MakeNextStartId();

    CARAPI PostNotification();

    CARAPI CancelNotification();

    CARAPI ClearDeliveredStartsLocked();

    CARAPI ToString(
        /* [out] */ String* str);

public:
    // Maximum number of delivery attempts before giving up.
    static const Int32 MAX_DELIVERY_COUNT = 3;

    // Maximum number of times it can fail during execution before giving up.
    static const Int32 MAX_DONE_EXECUTING_COUNT = 6;

    AutoPtr<CActivityManagerService> mAms;
    AutoPtr<BatteryStatsImpl::Uid::Pkg::Serv> mStats;
    AutoPtr<IComponentName> mName; // service component.
    String mShortName; // name.flattenToShortString().
    AutoPtr<IIntentFilterComparison> mIntent; // original intent used to find service.
    AutoPtr<IServiceInfo> mServiceInfo;  // all information about the service.
    AutoPtr<IApplicationInfo> mAppInfo; // information about service's app.
    Int32 mUserId;       // user that this service is running as
    String mPackageName; // the package implementing intent's component
    String mProcessName; // process where this component wants to run
    String mPermission; // permission needed to access service
    String mBaseDir;   // where activity source (resources etc) located
    String mResDir;   // where public activity source (public resources etc) located
    String mDataDir;   // where activity data should go
    Boolean mExported; // from ServiceInfo.exported
    AutoPtr<IRunnable> mRestarter; // used to schedule retries of starting the service
    Int64 mCreateTime;  // when this service was created
    HashMap< AutoPtr<IIntentFilterComparison>, AutoPtr<IntentBindRecord> > mBindings; // All active bindings to the service.
    typedef HashMap< AutoPtr<IBinder>, AutoPtr<List< AutoPtr<ConnectionRecord> > > > ConnectionHashMap;
    typedef typename ConnectionHashMap::Iterator ConnectionIterator;
    ConnectionHashMap mConnections; // IBinder -> ConnectionRecord of all bound clients

    AutoPtr<ProcessRecord> mApp;  // where this service is running or null.
    AutoPtr<ProcessRecord> mIsolatedProc; // keep track of isolated process, if requested
    Boolean mIsForeground;   // asked to run as a foreground service?
    Int32 mForegroundId;       // Notification ID of last foreground req.
    AutoPtr<INotification> mForegroundNoti; // Notification record of foreground state.
    Int64 mLastActivity;      // last time there was some activity on the service.
    Boolean mStartRequested; // someone explicitly called start?
    Boolean mStopIfKilled;   // last onStart() said to stop if service killed?
    Boolean mCallStart;      // last onStart() has asked to alway be called on restart.
    Int32 mExecuteNesting;     // number of outstanding operations keeping foreground.
    Int64 mExecutingStart;    // start time of last execute request.
    Int32 mCrashCount;         // number of times proc has crashed with service running
    Int32 mTotalRestartCount;  // number of times we have had to restart.
    Int32 mRestartCount;       // number of restarts performed in a row.
    Int64 mRestartDelay;      // delay until next restart attempt.
    Int64 mRestartTime;       // time of last restart.
    Int64 mNextRestartTime;   // time when restartDelay will expire.
    String mStringName;  // caching of toString
    List< AutoPtr<StartItem> > mDeliveredStarts;
                            // start() arguments which been delivered.
    List< AutoPtr<StartItem> > mPendingStarts;
                            // start() arguments that haven't yet been delivered.

private:
    Int32 mLastStartId;        // identifier of most recent start request.
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos


#define HASH_FUNC_FOR_AUTOPTR_CSERVICERECORD
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::CServiceRecord)
#endif

#endif //__ELASTOS_DROID_SERVER_AM_CSERVICERECORD_H__
