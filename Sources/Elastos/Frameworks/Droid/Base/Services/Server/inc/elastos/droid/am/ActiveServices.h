
#ifndef __ELASTOS_DROID_SERVER_AM_ACTIVESERVICES_H__
#define __ELASTOS_DROID_SERVER_AM_ACTIVESERVICES_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include "am/TaskRecord.h"
#include "am/CServiceRecord.h"
#include "am/TransferPipe.h"

using Elastos::Utility::Etl::List;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::App::IIServiceConnection;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IActivityManagerRunningServiceInfo;
using Elastos::Droid::App::IApplicationThread;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern "C" const InterfaceID EIID_ServiceRestarter;

class CActivityManagerService;
class ProcessRecord;

typedef HashMap< AutoPtr<IComponentName>, AutoPtr<CServiceRecord> > IComponentNameCServiceRecordHashMap;
typedef HashMap<AutoPtr<IIntentFilterComparison>, AutoPtr<CServiceRecord> > IIntentFilterComparisonCServiceRecordHashMap;

class ActiveServices : public ElRefBase
{
public:
    class ServiceMap : public ElRefBase
    {
    friend class ActiveServices;

    public:
        ServiceMap();

        ~ServiceMap();

        CARAPI_(AutoPtr<CServiceRecord>) GetServiceByName(
            /* [in] */ IComponentName* name,
            /* [in] */ Int32 callingUser);

        CARAPI_(AutoPtr<CServiceRecord>) GetServiceByName(
            /* [in] */ IComponentName* name);

        CARAPI_(AutoPtr<CServiceRecord>) GetServiceByIntent(
            /* [in] */ IIntentFilterComparison* filter,
            /* [in] */ Int32 callingUser);

        CARAPI_(AutoPtr<CServiceRecord>) GetServiceByIntent(
            /* [in] */ IIntentFilterComparison* filter);

        CARAPI_(void) PutServiceByName(
            /* [in] */ IComponentName* name,
            /* [in] */ Int32 callingUser,
            /* [in] */ CServiceRecord* value);

        CARAPI_(void) PutServiceByIntent(
            /* [in] */ IIntentFilterComparison* filter,
            /* [in] */ Int32 callingUser,
            /* [in] */ CServiceRecord* value);

        CARAPI_(void) RemoveServiceByName(
            /* [in] */ IComponentName* name,
            /* [in] */ Int32 callingUser);

        CARAPI_(void) RemoveServiceByIntent(
            /* [in] */ IIntentFilterComparison* filter,
            /* [in] */ Int32 callingUser);

    private:
        CARAPI_(AutoPtr<IComponentNameCServiceRecordHashMap> ) GetServices(
            /* [in] */ Int32 callingUser);

        CARAPI_(AutoPtr<IIntentFilterComparisonCServiceRecordHashMap>) GetServicesByIntent(
            /* [in] */ Int32 callingUser);

    private:
        HashMap<Int32, AutoPtr<IComponentNameCServiceRecordHashMap> > mServicesByNamePerUser;
        HashMap<Int32, AutoPtr<IIntentFilterComparisonCServiceRecordHashMap> > mServicesByIntentPerUser;
    };

    class ServiceLookupResult : public ElRefBase
    {
    public:
        ServiceLookupResult(
            /* [in] */ CServiceRecord* record,
            /* [in] */ const String& permission);
    public:
        AutoPtr<CServiceRecord> mRecord;
        String mPermission;
    };

private:
    class ServiceRestarter
        : public ElRefBase
        , public IRunnable
    {
    public:
        ServiceRestarter(
            /* [in] */ ActiveServices* owner);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) SetService(
            /* [in] */ CServiceRecord* service);

        CARAPI Run();

    private:
        CServiceRecord* mService;
        ActiveServices* mOwner;
    };

public:
    ActiveServices(
        /* [in] */ CActivityManagerService* service);

    ~ActiveServices();

    CARAPI StartServiceLocked(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [out] */ IComponentName** name);

    CARAPI StopServiceLocked(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI PeekServiceLocked(
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [out] */ IBinder** token);

    CARAPI StopServiceTokenLocked(
        /* [in] */ IComponentName* className,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 startId,
        /* [out] */ Boolean* succeeded);

    CARAPI SetServiceForegroundLocked(
        /* [in] */ IComponentName* className,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification,
        /* [in] */ Boolean removeNotification);

    CARAPI BindServiceLocked(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIServiceConnection* connection,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI PublishServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ IIntent* intent,
        /* [in] */ IBinder* service);

    CARAPI UnbindServiceLocked(
        /* [in] */ IIServiceConnection* connection,
        /* [out] */ Boolean* result);

    CARAPI UnbindFinishedLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean doRebind);

    CARAPI PerformServiceRestartLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI RemoveConnectionLocked(
        /* [in] */ ConnectionRecord* c,
        /* [in] */ ProcessRecord* skipApp,
        /* [in] */ ActivityRecord* skipAct);

    CARAPI ServiceDoneExecutingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Int32 type,
        /* [in] */ Int32 startId,
        /* [in] */ Int32 res);

    CARAPI AttachApplicationLocked(
        /* [in] */ ProcessRecord* proc,
        /* [in] */ const String& processName,
        /* [out] */ Boolean* result);

    CARAPI ProcessStartTimedOutLocked(
        /* [in] */ ProcessRecord* proc);

    CARAPI ForceStopLocked(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean evenPersistent,
        /* [in] */ Boolean doit,
        /* [out] */ Boolean* result);

    CARAPI CleanUpRemovedTaskLocked(
        /* [in] */ TaskRecord* tr,
        /* [in] */ IComponentName* component,
        /* [in] */ IIntent* baseIntent);

    CARAPI KillServicesLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean allowRestart);

    CARAPI_(AutoPtr<IActivityManagerRunningServiceInfo>) MakeRunningServiceInfoLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI_(AutoPtr<List<AutoPtr<IActivityManagerRunningServiceInfo> > >) GetRunningServiceInfoLocked(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags);

    CARAPI GetRunningServiceControlPanelLocked(
        /* [in] */ IComponentName* name,
        /* [out] */ IPendingIntent** intent);

    CARAPI ServiceTimeout(
        /* [in] */ ProcessRecord* proc);

    CARAPI_(Boolean) DumpServicesLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll,
        /* [in] */ Boolean dumpClient,
        /* [in] */ const String& dumpPackage);

    /**
     * There are three ways to call this:
     *  - no service specified: dump all the services
     *  - a flattened component name that matched an existing service was specified as the
     *    first arg: dump that one service
     *  - the first arg isn't the flattened component name of an existing service:
     *    dump all services whose component contains the first arg as a substring
     */
    CARAPI_(Boolean) DumpService(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll);

private:
    CARAPI StopServiceLocked(
        /* [in] */ CServiceRecord* service);

    CARAPI UpdateServiceForegroundLocked(
        /* [in] */ ProcessRecord* proc,
        /* [in] */ Boolean oomAdj);

    CARAPI_(AutoPtr<CServiceRecord>) FindServiceLocked(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<ServiceLookupResult>) RetrieveServiceLocked(
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean createIfNeeded);

    CARAPI_(void) BumpServiceExecutingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ const String& why);

    CARAPI_(Boolean) RequestServiceBindingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ IntentBindRecord* i,
        /* [in] */ Boolean rebind);

    CARAPI_(Boolean) ScheduleServiceRestartLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean allowCancel);

    CARAPI_(Boolean) UnscheduleServiceRestartLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI_(String) BringUpServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Int32 intentFlags,
        /* [in] */ Boolean whileRestarting);

    CARAPI RequestServiceBindingsLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI RealStartServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ ProcessRecord* app);

    CARAPI SendServiceArgsLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean oomAdjusted);

    CARAPI BringDownServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean force);

    CARAPI ServiceDoneExecutingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean inStopping);

    CARAPI_(Boolean) CollectForceStopServicesLocked(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean evenPersistent,
        /* [in] */ Boolean doit,
        /* [in] */ IComponentNameCServiceRecordHashMap* services,
        /* [in] */ List<AutoPtr<CServiceRecord> >* result);

    /**
     * Invokes IApplicationThread.dumpService() on the thread of the specified service if
     * there is a thread associated with the service.
     */
    CARAPI_(void) DumpService(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ CServiceRecord* r,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Boolean dumpAll);

public:
    static const Boolean DEBUG_SERVICE;
    static const Boolean DEBUG_SERVICE_EXECUTING;
    static const Boolean DEBUG_MU;
    static const String TAG;
    static const String TAG_MU;

    // How long we wait for a service to finish executing.
    static const Int32 SERVICE_TIMEOUT = 20*1000;

    // How long a service needs to be running until restarting its process
    // is no longer considered to be a relaunch of the service.
    static const Int32 SERVICE_RESTART_DURATION = 5*1000;

    // How long a service needs to be running until it will start back at
    // SERVICE_RESTART_DURATION after being killed.
    static const Int32 SERVICE_RESET_RUN_DURATION = 60*1000;

    // Multiplying factor to increase restart duration time by, for each time
    // a service is killed before it has run for SERVICE_RESET_RUN_DURATION.
    static const Int32 SERVICE_RESTART_DURATION_FACTOR = 4;

    // The minimum amount of time between restarting services that we allow.
    // That is, when multiple services are restarting, we won't allow each
    // to restart less than this amount of time from the last one.
    static const Int32 SERVICE_MIN_RESTART_TIME_BETWEEN = 10*1000;

    // Maximum amount of time for there to be no activity on a service before
    // we consider it non-essential and allow its process to go on the
    // LRU background list.
    static const Int32 MAX_SERVICE_INACTIVITY = 30*60*1000;

    CActivityManagerService* mAm; // weak-ref

    AutoPtr<ServiceMap> mServiceMap;

    /**
     * All currently bound service connections.  Keys are the IBinder of
     * the client's IServiceConnection.
     */
    typedef List<AutoPtr<ConnectionRecord> > ConnectionRecordList;
    typedef ConnectionRecordList::Iterator ConnectionRecordListIterator;
    HashMap< AutoPtr<IBinder>, AutoPtr<ConnectionRecordList> > mServiceConnections;

    /**
     * List of services that we have been asked to start,
     * but haven't yet been able to.  It is used to hold start requests
     * while waiting for their corresponding application thread to get
     * going.
     */
    List< AutoPtr<CServiceRecord> > mPendingServices;

    /**
     * List of services that are scheduled to restart following a crash.
     */
    List< AutoPtr<CServiceRecord> > mRestartingServices;

    /**
     * List of services that are in the process of being stopped.
     */
    List< AutoPtr<CServiceRecord> > mStoppingServices;

    Object mLock;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVESERVICES_H__
