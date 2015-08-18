
#ifndef __ELASTOS_DROID_SERVER_AM_CACTIVITYMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_AM_CACTIVITYMANAGERSERVICE_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_Server_Am_CActivityManagerService.h"
#include "ProcessMap.h"
#include "IntentResolver.h"
#include "wm/CWindowManagerService.h"
#include "am/ContentProviderRecord.h"
#include "am/CPendingIntentRecord.h"
#include "am/CBatteryStatsService.h"
#include "am/CUsageStatsService.h"
#include "am/CoreSettingsObserver.h"
#include "am/BackupRecord.h"
#include "am/ProviderMap.h"
#include "am/UriPermission.h"
#include "am/UriPermissionOwner.h"
#include "am/PendingThumbnailsRecord.h"
#include "am/ActivityRecord.h"
#include "am/ProcessRecord.h"
#include "am/ProcessList.h"
#include "am/CompatModePackages.h"
#include "os/FileObserver.h"
#include "os/HandlerBase.h"
#include "pm/CUserManagerService.h"
// #include <binder/IPermissionController.h>
#include <Elastos.Droid.Core.h>
#include <Elastos.CoreLibrary.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/Core/Object.h>
#include <elastos/utility/etl/List.h>
#include <pthread.h>


using Libcore::ICU::ILocale;
using Elastos::Core::IInteger32;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;
using Elastos::Utility::IObjectStringMap;
using Elastos::IO::IFile;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::FileObserver;
using Elastos::Droid::Os::IIUserManager;
using Elastos::Droid::Os::IDropBoxManager;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IDebugMemoryInfo;
using Elastos::Droid::Os::IStrictModeViolationInfo;
using Elastos::Droid::Os::IRemoteCallbackList;
using Elastos::Droid::Internal::Os::IProcessStats;
using Elastos::Droid::Internal::Os::CProcessStats;
using Elastos::Droid::Internal::Os::IProcessStatsStats;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::IProxyProperties;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IIIntentSender;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IConfigurationInfo;
using Elastos::Droid::Content::Pm::IPackageDataObserver;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::App::IActivityManagerTaskThumbnails;
using Elastos::Droid::App::IActivityManagerMemoryInfo;
using Elastos::Droid::App::IActivityManagerRunningAppProcessInfo;
using Elastos::Droid::App::IActivityManagerProcessErrorStateInfo;
using Elastos::Droid::App::IActivityThread;
using Elastos::Droid::App::IActivityController;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::IApplicationThread;
using Elastos::Droid::App::IApplicationErrorReport;
using Elastos::Droid::App::IApplicationErrorReportCrashInfo;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IActivityManagerWaitResult;
using Elastos::Droid::App::IContentProviderHolder;
using Elastos::Droid::App::IInstrumentationWatcher;
using Elastos::Droid::App::IIServiceConnection;
using Elastos::Droid::App::INotification;
using Elastos::Droid::App::IProcessObserver;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IStopUserCallback;
using Elastos::Droid::App::IThumbnailReceiver;
using Elastos::Droid::App::IUserSwitchObserver;
using Elastos::Droid::Server::ProcessMap;
using Elastos::Droid::Server::IntentResolver;
using Elastos::Droid::Server::Wm::CWindowManagerService;
using Elastos::Droid::Server::Pm::CUserManagerService;
using Elastos::Utility::Concurrent::Atomic::IAtomicBoolean;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger64;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern "C" const InterfaceID EIID_StringObjectHashMap;

class ActiveServices;
class ActivityStack;
class BackupRecord;
class BroadcastFilter;
class BroadcastFilterList;
class BroadcastQueue;
class BroadcastRecord;
//class CBatteryStatsService;
class CompatModePackages;
class CCompatModeDialog;
class ContentProviderRecord;
class CContentProviderConnection;
class CoreSettingsObserver;
class ProcessList;
class ProviderMap;
class ReceiverList;
class TaskRecord;
class UserStartedState;
class CUsageStatsService;
class CompatModeDialog;

//
// NeededUriGrants
//
class NeededUriGrants
    : public List< AutoPtr<IUri> >
{
public:
    NeededUriGrants(
        /* [in] */ const String& targetPkg,
        /* [in] */ Int32 targetUid,
        /* [in] */ Int32 flags);

public:
    String mTargetPkg;
    Int32 mTargetUid;
    Int32 mFlags;
};


//
// CActivityManagerService
//
CarClass(CActivityManagerService)
{
    friend class ActivityRecord;
    friend class CStoppingReceiver;
    friend class CompatModePackages;
    friend class CoreSettingsObserver;

public:
    class MyHandler
        : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ CActivityManagerService* host);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CActivityManagerService* mHost;
    };

    class StringObjectHashMap
        : public IInterface
        , public HashMap<String, AutoPtr<IInterface> >
    {
    public:
        CAR_INTERFACE_DECL()
    };

    /**
     * Description of a request to start a new activity, which has been held
     * due to app switches being disabled.
     */
    class PendingActivityLaunch : public ElRefBase
    {
    public:
        PendingActivityLaunch();
    public:
        AutoPtr<ActivityRecord> mR;
        AutoPtr<ActivityRecord> mSourceRecord;
        Int32 mStartFlags;
    };

    /**
     * All of the processes that have been forced to be foreground.  The key
     * is the pid of the caller who requested it (we hold a death
     * link on it).
     */
    class ForegroundToken
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        ForegroundToken();

        virtual CARAPI ProxyDied();

        CAR_INTERFACE_DECL()
    public:
        Int32 mPid;
        AutoPtr<IBinder> mToken;
    };

    class ProcessChangeItem : public ElRefBase
    {
    public:
        ProcessChangeItem();
    public:
        static const Int32 CHANGE_ACTIVITIES = 1<<0;
        static const Int32 CHANGE_IMPORTANCE= 1<<1;
        Int32 mChanges;
        Int32 mUid;
        Int32 mPid;
        Int32 mImportance;
        Boolean mForegroundActivities;
    };

    class AThread
        : public ThreadBase
    {
    public:
        AThread();

        virtual CARAPI Run();

    public:
        AutoPtr<CActivityManagerService> mService;
        Boolean mReady;
    };

    class ProcessStatsThread
        : public ThreadBase
    {
    public:
        ProcessStatsThread(
            /* [in] */ CActivityManagerService* host);

        CARAPI Run();

    public:
        CActivityManagerService* mHost;
    };

    class ItemMatcher : public ElRefBase
    {
    public:
        ItemMatcher();

        ~ItemMatcher();

        CARAPI_(void) Build(
            /* [in] */ const String& name);

        CARAPI_(Int32) Build(
            /* [in] */ const ArrayOf<String>* args,
            /* [in] */ Int32 opti);

        CARAPI_(Boolean) Match(
            /* [in] */ IInterface* object,
            /* [in] */ IComponentName* comp);

    public:
        List< AutoPtr<IComponentName> > mComponents;
        List<String> mStrings;
        List< AutoPtr<IInteger32> > mObjects;
        Boolean mAll;
    };

    class MemItem : public ElRefBase
    {
    public:
        MemItem(
            /* [in] */ const String& label,
            /* [in] */ const String& shortLabel,
            /* [in] */ Int64 pss,
            /* [in] */ Int32 id);

        ~MemItem();

    public:
        const String mLabel;
        const String mShortLabel;
        const Int64 mPss;
        const Int32 mId;
        AutoPtr<List< AutoPtr<MemItem> > > mSubitems;
    };

    class ReceiverResolver
        : public IntentResolver<BroadcastFilter, BroadcastFilter>
    {
    protected:
        Boolean AllowFilterResult(
                /* [in] */ BroadcastFilter* filter,
                /* [in] */ List<AutoPtr<BroadcastFilter> >* dest);

        AutoPtr<BroadcastFilter> NewResult(
            /* [in] */ BroadcastFilter* filter,
            /* [in] */ Int32 match,
            /* [in] */ Int32 userId);

        AutoPtr< ArrayOf<BroadcastFilter*> > NewArray(
            /* [in] */ Int32 size);

        String PackageForFilter(
            /* [in] */ BroadcastFilter* filter);
    };

public:
    /**
     * Thread-local storage used to carry caller permissions over through
     * indirect content-provider access.
     * @see #ActivityManagerService.openContentUri()
     */
    class Identity : public ElRefBase
    {
    public:
        Identity(
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid);
    public:
        Int32 mPid;
        Int32 mUid;
    };

private:
    class AppDeathRecipient
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        AppDeathRecipient(
            /* [in] */ ProcessRecord* app,
            /* [in] */ Int32 pid,
            /* [in] */ IApplicationThread* thread,
            /* [in] */ CActivityManagerService* owner);

        CARAPI ProxyDied();

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<ProcessRecord> mApp;
        Int32 mPid;
        AutoPtr<IApplicationThread> mAppThread;
        CActivityManagerService* mOwner;
    };

    class SwitchUserIntentReceiver
        : public ElRefBase
        , public IIntentReceiver
    {
    public:
        SwitchUserIntentReceiver(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ UserStartedState* uss,
            /* [in] */ Int32 userId)
            : mHost(host)
            , mUss(uss)
            , mUserId(userId)
        {}

        CAR_INTERFACE_DECL()

        CARAPI PerformReceive(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ const String& data,
            /* [in] */ IBundle* extras,
            /* [in] */ Boolean ordered,
            /* [in] */ Boolean sticky,
            /* [in] */ Int32 sendingUser);

    private:
        CActivityManagerService* mHost;
        UserStartedState* mUss;
        Int32 mUserId;
    };

    class NeedStartIntentReceiver
        : public ElRefBase
        , public IIntentReceiver
    {
    public:
        NeedStartIntentReceiver()
        {}

        CAR_INTERFACE_DECL()

        CARAPI PerformReceive(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ const String& data,
            /* [in] */ IBundle* extras,
            /* [in] */ Boolean ordered,
            /* [in] */ Boolean sticky,
            /* [in] */ Int32 sendingUser)
        {
            return NOERROR;
        }
    };

    class ReportMemUsageThread
        : public ThreadBase
    {
    public:
        ReportMemUsageThread(
            /* [in] */ CActivityManagerService* host);

        CARAPI Run();

    public:
        CActivityManagerService* mHost;
    };

    class DumpStackTracesFileObserver
        : public FileObserver
        , public Object
    {
    public:
        DumpStackTracesFileObserver(
            /* [in] */ const String& path,
            /* [in] */ Int32 mask)
            : FileObserver(path, mask)
        {}

        CARAPI OnEvent(
            /* [in] */ Int32 event,
            /* [in] */ const String& path);
    };

    class ShowRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        ShowRunnable(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ IDialog* dialog)
            : mHost(host)
            , mDialog(dialog)
        {}

        CARAPI Run();

        CAR_INTERFACE_DECL()

    private:
        CActivityManagerService* mHost;
        AutoPtr<IDialog> mDialog;
    };

    class ShowLaunchWarningLockedRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        ShowLaunchWarningLockedRunnable(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ ActivityRecord* cur,
            /* [in] */ ActivityRecord* next)
            : mHost(host)
            , mCur(cur)
            , mNext(next)
        {}

        CARAPI Run();

        CAR_INTERFACE_DECL()

    private:
        CActivityManagerService* mHost;
        AutoPtr<ActivityRecord> mCur;
        AutoPtr<ActivityRecord> mNext;
    };

    class FinishBootingBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        FinishBootingBroadcastReceiver(
            /* [in] */ CActivityManagerService* host)
            : mHost(host)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CActivityManagerService::FinishBootingBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CActivityManagerService* mHost;
    };

    class SetProcessForegroundToken
        : public ForegroundToken
    {
    public:
        SetProcessForegroundToken(
            /* [in] */ CActivityManagerService* host)
            : mHost(host)
        {}

        CARAPI ProxyDied();

    private:
        CActivityManagerService* mHost;
    };

    // class FinisherReceiver
    //     : public ElRefBase
    //     , public IIntentReceiver
    // {
    // public:
    //     FinisherReceiver(
    //         /* [in] */ CActivityManagerService* host,
    //         /* [in] */ List< AutoPtr<IComponentName> >* receivers,
    //         /* [in] */ IRunnable* goingCallback)
    //         : mHost(host)
    //         , mReceivers(receivers)
    //         , mCallback(goingCallback)
    //     {}

    //     CAR_INTERFACE_DECL()

    //     CARAPI PerformReceive(
    //        /* [in] */ IIntent* intent,
    //        /* [in] */ Int32 resultCode,
    //        /* [in] */ const String& data,
    //        /* [in] */ IBundle* extras,
    //        /* [in] */ Boolean ordered,
    //        /* [in] */ Boolean sticky,
    //        /* [in] */ Int32 sendingUser);

    // private:
    //     CActivityManagerService* mHost;
    //     AutoPtr<List< AutoPtr<IComponentName> > > mReceivers;
    //     AutoPtr<IRunnable> mCallback;
    // };

    // class PerformReceiveRunnable
    //     : public ElRefBase
    //     , public IRunnable
    // {
    // public:
    //     PerformReceiveRunnable(
    //         /* [in] */ CActivityManagerService* host,
    //         /* [in] */ List< AutoPtr<IComponentName> >* receivers,
    //         /* [in] */ IRunnable* callback)
    //         : mHost(host)
    //         , mCallback(callback)
    //         , mReceivers(receivers)
    //     {}

    //     CAR_INTERFACE_DECL()

    //     CARAPI Run();

    // private:
    //     CActivityManagerService* mHost;
    //     AutoPtr<List< AutoPtr<IComponentName> > > mReceivers;
    //     AutoPtr<IRunnable> mCallback;
    // };

    class ErrorMsgButtonOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        ErrorMsgButtonOnClickListener(
            /* [in] */ CActivityManagerService* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        CActivityManagerService* mHost;
    };

    // class SystemBroadcastReceiver
    //     : public ElRefBase
    //     , public IIntentReceiver
    // {
    // public:
    //     SystemBroadcastReceiver()
    //     {}

    //     CAR_INTERFACE_DECL()

    //     CARAPI PerformReceive(
    //         /* [in] */ IIntent* intent,
    //         /* [in] */ Int32 resultCode,
    //         /* [in] */ const String& data,
    //         /* [in] */ IBundle* extras,
    //         /* [in] */ Boolean ordered,
    //         /* [in] */ Boolean sticky,
    //         /* [in] */ Int32 sendingUser);
    // };

    class DropBoxTagThread
        : public ThreadBase
    {
    public:
        DropBoxTagThread(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ StringBuilder* sb,
            /* [in] */ Object* lock,
            /* [in] */ IDropBoxManager* dbox,
            /* [in] */ const String& tag);

        CARAPI Run();

    public:
        CActivityManagerService* mHost;
        AutoPtr<StringBuilder> mSb;
        Object* mSbLock;
        AutoPtr<IDropBoxManager> mDbox;
        String mTag;
    };

    class DropBoxTagThread2
        : public ThreadBase
    {
    public:
        DropBoxTagThread2(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ IDropBoxManager* dbox,
            /* [in] */ const String& tag);

        CARAPI Run();

    public:
        CActivityManagerService* mHost;
        AutoPtr<IDropBoxManager> mDbox;
        String mTag;
    };

    class WorkerThread
        : public ThreadBase
    {
    public:
        WorkerThread(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ const String& dropboxTag,
            /* [in] */ const String& report,
            /* [in] */ StringBuilder* sb,
            /* [in] */ IFile* logFile,
            /* [in] */ IApplicationErrorReportCrashInfo* crashInfo,
            /* [in] */ IDropBoxManager* dbox);

        CARAPI Run();

    public:
        CActivityManagerService* mHost;
        String mDropboxTag;
        String mReport;
        AutoPtr<StringBuilder> mSb;
        AutoPtr<IFile> mLogFile;
        AutoPtr<IApplicationErrorReportCrashInfo> mCrashInfo;
        AutoPtr<IDropBoxManager> mDbox;
    };

    class StopUserLockedRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        StopUserLockedRunnable(
            /* [in] */ Int32 userId,
            /* [in] */ IStopUserCallback* callback)
            : mUserId(userId)
            , mCallback(callback)
        {}

        CARAPI Run();

        CAR_INTERFACE_DECL()

    private:
        Int32 mUserId;
        AutoPtr<IStopUserCallback> mCallback;
    };

    class DispatchUserSwitchCallback
        : public ElRefBase
        , public IRemoteCallback
    {
    public:
        DispatchUserSwitchCallback(
            /* [in] */ CActivityManagerService* host,
            /* [in] */ Int32 n,
            /* [in] */ UserStartedState* uss,
            /* [in] */ Int32 oldUserId,
            /* [in] */ Int32 newUserId)
            : mHost(host)
            , mCount(0)
            , N(n)
            , mUss(uss)
            , mOldUserId(oldUserId)
            , mNewUserId(newUserId)
        {}

        CAR_INTERFACE_DECL()

        CARAPI SendResult(
            /* [in] */ IBundle* data);

    private:
        CActivityManagerService* mHost;
        Int32 mCount;
        Int32 N;
        AutoPtr<UserStartedState> mUss;
        Int32 mOldUserId;
        Int32 mNewUserId;
    };

public:
    CActivityManagerService();

    ~CActivityManagerService();

    CARAPI constructor();

public:
    //
    // interface IActivityManagerService
    //
    CARAPI GetSystemContext(
        /* [out] */ IContext** ctx);

    static CARAPI SetSystemProcess();

    CARAPI_(void) SetWindowManager(
        /* [in] */ CWindowManagerService* wm);

    static CARAPI_(AutoPtr<IContext>) Main(
        /* [in] */ Int32 factoryTest);

    static CARAPI_(AutoPtr<CActivityManagerService>) Self();

    //
    // interface IIActivityManager
    //
    CARAPI StartActivity(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 startFlags,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ IBundle* options,
        /* [out] */ Int32* result);

    CARAPI StartActivityAsUser(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 flags,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI StartActivityAndWait(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 flags,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ IActivityManagerWaitResult** result);

    CARAPI StartActivityWithConfig(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 startFlags,
        /* [in] */ IConfiguration* newConfig,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI StartActivityIntentSender(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ IBundle* options,
        /* [out] */ Int32* result);

    CARAPI StartNextMatchingActivity(
        /* [in] */ IBinder* callingActivity,
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [out] */ Boolean* result);

    CARAPI FinishActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* data,
        /* [out] */ Boolean* result);

    CARAPI FinishSubActivity(
        /* [in] */ IBinder* token,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode);

    CARAPI FinishActivityAffinity(
        /* [in] */ IBinder* token,
        /* [out] */ Boolean* result);

    CARAPI WillActivityBeVisible(
        /* [in] */ IBinder* token,
        /* [out] */ Boolean* visible);

    CARAPI RegisterReceiver(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ const String& callerPackage,
        /* [in] */ IIntentReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& requiredPermission,
        /* [in] */ Int32 userId,
        /* [out] */ IIntent** intent);

    CARAPI UnregisterReceiver(
        /* [in] */ IIntentReceiver* receiver);

    CARAPI BroadcastIntent(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* map,
        /* [in] */ const String& requiredPermission,
        /* [in] */ Boolean serialized,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI UnbroadcastIntent(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 userId);

    CARAPI FinishReceiver(
        /* [in] */ IBinder* who,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* map,
        /* [in] */ Boolean abortBroadcast);

    CARAPI AttachApplication(
        /* [in] */ IApplicationThread* app);

    CARAPI ActivityResumed(
        /* [in] */ IBinder* token);

    CARAPI ActivityIdle(
        /* [in] */ IBinder* token,
        /* [in] */ IConfiguration* config,
        /* [in] */ Boolean stopProfiling);

    CARAPI ActivityPaused(
        /* [in] */ IBinder* token);

    CARAPI ActivityStopped(
        /* [in] */ IBinder* token,
        /* [in] */ IBundle* state,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ ICharSequence* description);

    CARAPI ActivitySlept(
        /* [in] */ IBinder* token);

    CARAPI ActivityDestroyed(
        /* [in] */ IBinder* token);

    CARAPI GetCallingPackage(
        /* [in] */ IBinder* token,
        /* [out] */ String* pkg);

    CARAPI GetCallingActivity(
        /* [in] */ IBinder* token,
        /* [out] */ IComponentName** activity);

    CARAPI GetTasks(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [in] */ IThumbnailReceiver* receiver,
        /* [out] */ IObjectContainer** tasks);

    CARAPI GetRecentTasks(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** tasks);

    CARAPI GetTaskThumbnails(
        /* [in] */ Int32 taskId,
        /* [out] */ IActivityManagerTaskThumbnails** taskThumbnails);

    CARAPI GetTaskTopThumbnail(
        /* [in] */ Int32 taskId,
        /* [out] */ IBitmap** bitmap);

    CARAPI GetServices(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** services);

    CARAPI GetProcessesInErrorState(
        /* [out] */ IObjectContainer** processes);

    CARAPI MoveTaskToFront(
        /* [in] */ Int32 task,
        /* [in] */ Int32 flags,
        /* [in] */ IBundle* options);

    CARAPI MoveTaskToBack(
        /* [in] */ Int32 task);

    CARAPI MoveActivityTaskToBack(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean nonRoot,
        /* [out] */ Boolean* completed);

    CARAPI MoveTaskBackwards(
        /* [in] */ Int32 task);

    CARAPI GetTaskForActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean onlyRoot,
        /* [out] */ Int32* task);

    /* oneway */
    CARAPI ReportThumbnail(
        /* [in] */ IBinder* token,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ ICharSequence* description);

    CARAPI GetContentProvider(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ const String& name,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean stable,
        /* [out] */ IContentProviderHolder** contentProvider);

    CARAPI GetContentProviderExternal(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId,
        /* [in] */ IBinder* token,
        /* [out] */ IContentProviderHolder** holder);

    CARAPI RemoveContentProvider(
        /* [in] */ IBinder* connection,
        /* [in] */ Boolean stable);

    CARAPI RemoveContentProviderExternal(
        /* [in] */ const String& name,
        /* [in] */ IBinder* token);

    CARAPI PublishContentProviders(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IObjectContainer* providers);

    CARAPI RefContentProvider(
        /* [in] */ IBinder* connection,
        /* [in] */ Int32 stableDelta,
        /* [in] */ Int32 unstableDelta,
        /* [out] */ Boolean* result);

    CARAPI UnstableProviderDied(
        /* [in] */ IBinder* connection);

    CARAPI GetRunningServiceControlPanel(
        /* [in] */ IComponentName* service,
        /* [out] */ IPendingIntent** pendingIntent);

    CARAPI StartService(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 userId,
        /* [out] */ IComponentName** componentName);

    CARAPI StopService(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI StopServiceToken(
        /* [in] */ IComponentName* className,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 startId,
        /* [out] */ Boolean* result);

    CARAPI SetServiceForeground(
        /* [in] */ IComponentName* className,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification,
        /* [in] */ Boolean keepNotification);

    CARAPI BindService(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIServiceConnection* connection,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI UnbindService(
        /* [in] */ IIServiceConnection* connection,
        /* [out] */ Boolean* result);

    CARAPI PublishService(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent,
        /* [in] */ IBinder* service);

    CARAPI UnbindFinished(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* service,
        /* [in] */ Boolean doRebind);

    /* oneway */
    CARAPI ServiceDoneExecuting(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 type,
        /* [in] */ Int32 startId,
        /* [in] */ Int32 res);

    CARAPI PeekService(
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [out] */ IBinder** binder);

    CARAPI BindBackupAgent(
        /* [in] */ IApplicationInfo* appInfo,
        /* [in] */ Int32 backupRestoreMode,
        /* [out] */ Boolean* result);

    CARAPI ClearPendingBackup();

    CARAPI BackupAgentCreated(
        /* [in] */ const String& packageName,
        /* [in] */ IBinder* agent);

    CARAPI UnbindBackupAgent(
        /* [in] */ IApplicationInfo* appInfo);

    CARAPI KillApplicationProcess(
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid);

    CARAPI StartInstrumentation(
        /* [in] */ IComponentName* className,
        /* [in] */ const String& profileFile,
        /* [in] */ Int32 flags,
        /* [in] */ IBundle* arguments,
        /* [in] */ IInstrumentationWatcher* watcher,
        /* [in] */ Int32 userId,
        /* [out] */ Boolean* result);

    CARAPI FinishInstrumentation(
        /* [in] */ IApplicationThread* target,
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI GetConfiguration(
        /* [out] */ IConfiguration** config);

    CARAPI UpdateConfiguration(
        /* [in] */ IConfiguration* values);

    CARAPI SetRequestedOrientation(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 requestedOrientation);

    CARAPI GetRequestedOrientation(
        /* [in] */ IBinder* token,
        /* [out] */ Int32* orientation);

    CARAPI GetActivityClassForToken(
        /* [in] */ IBinder* token,
        /* [out] */ IComponentName** klass);

    CARAPI GetPackageForToken(
        /* [in] */ IBinder* token,
        /* [out] */ String* pkg);

    CARAPI GetIntentSender(
        /* [in] */ Int32 type,
        /* [in] */ const String& packageName,
        /* [in] */ IBinder* token,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ ArrayOf<String>* resolvedTypes,
        /* [in] */ Int32 flags,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ IIIntentSender** intentSender);

    CARAPI CancelIntentSender(
        /* [in] */ IIIntentSender* sender);

    CARAPI ClearApplicationUserData(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDataObserver* observer,
        /* [in] */ Int32 userId,
        /* [out] */ Boolean* result);

    CARAPI GetPackageForIntentSender(
        /* [in] */ IIIntentSender* sender,
        /* [out] */ String* pkg);

    CARAPI GetUidForIntentSender(
        /* [in] */ IIIntentSender* sender,
        /* [out] */ Int32* result);

    CARAPI HandleIncomingUser(
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean allowAll,
        /* [in] */ Boolean requireFull,
        /* [in] */ const String& name,
        /* [in] */ const String& callerPackage,
        /* [out] */ Int32* result);

    CARAPI GetIntentForIntentSender(
        /* [in] */ IIIntentSender* sender,
        /* [out] */ IIntent** intent);

    CARAPI SetProcessLimit(
        /* [in] */ Int32 max);

    CARAPI GetProcessLimit(
        /* [out] */ Int32* limit);

    CARAPI SetProcessForeground(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 pid,
        /* [in] */ Boolean isForeground);

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 mode,
        /* [out] */ Int32* result);

    CARAPI GrantUriPermission(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ const String& targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 mode);

    CARAPI RevokeUriPermission(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 mode);

    CARAPI ShowWaitingForDebugger(
        /* [in] */ IApplicationThread* who,
        /* [in] */ Boolean waiting);

    CARAPI GetMemoryInfo(
        /* [out] */ IActivityManagerMemoryInfo** outInfo);

    CARAPI KillBackgroundProcesses(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    CARAPI KillAllBackgroundProcesses();

    CARAPI ForceStopPackage(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 userId);

    // Note: probably don't want to allow applications access to these.
    CARAPI GoingToSleep();

    CARAPI WakingUp();

    CARAPI SetLockScreenShown(
        /* [in] */ Boolean shown);

    CARAPI UnhandledBack();

    CARAPI OpenContentUri(
        /* [in] */ IUri* uri,
        /* [out] */ IParcelFileDescriptor** descriptor);

    CARAPI SetDebugApp(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean waitForDebugger,
        /* [in] */ Boolean persistent);

    CARAPI SetAlwaysFinish(
        /* [in] */ Boolean enabled);

    CARAPI SetActivityController(
        /* [in] */ IActivityController* watcher);

    CARAPI EnterSafeMode();

    CARAPI NoteWakeupAlarm(
        /* [in] */ IIIntentSender* sender);

    CARAPI KillPids(
        /* [in] */ ArrayOf<Int32>* pids,
        /* [in] */ const String& reason,
        /* [in] */ Boolean secure,
        /* [out] */ Boolean* result);

    CARAPI KillProcessesBelowForeground(
        /* [in] */ const String& reason,
        /* [out] */ Boolean* result);

    // Special low-level communication with activity manager.
    CARAPI StartRunning(
        /* [in] */ const String& pkg,
        /* [in] */ const String& cls,
        /* [in] */ const String& action,
        /* [in] */ const String& data);

    CARAPI HandleApplicationCrash(
        /* [in] */ IBinder* app,
        /* [in] */ IApplicationErrorReportCrashInfo* crashInfo);

    CARAPI HandleApplicationWtf(
        /* [in] */ IBinder* app,
        /* [in] */ const String& tag,
        /* [in] */ IApplicationErrorReportCrashInfo* crashInfo,
        /* [out] */ Boolean* result);

    // A StrictMode violation to be handled.  The violationMask is a
    // subset of the original StrictMode policy bitmask, with only the
    // bit violated and penalty bits to be executed by the
    // ActivityManagerService remaining set.
    CARAPI HandleApplicationStrictModeViolation(
        /* [in] */ IBinder* app,
        /* [in] */ Int32 violationMask,
        /* [in] */ IStrictModeViolationInfo* crashInfo);

    /*
     * This will deliver the specified signal to all the persistent processes. Currently only
     * SIGUSR1 is delivered. All others are ignored.
     */
    CARAPI SignalPersistentProcesses(
        /* [in] */ Int32 signal);

    // Retrieve running application processes in the system
    CARAPI GetRunningAppProcesses(
        /* [out] */ IObjectContainer** processes);

    // Retrieve info of applications installed on external media that are currently
    // running.
    CARAPI GetRunningExternalApplications(
        /* [out] */ IObjectContainer** applications);

    // Get memory information about the calling process.
    CARAPI GetMyMemoryState(
        /* [in] */ IActivityManagerRunningAppProcessInfo* outInfo);

    // Get device configuration
    CARAPI GetDeviceConfigurationInfo(
        /* [out] */ IConfigurationInfo** configInfo);

    // Turn on/off profiling in a particular process.
    CARAPI ProfileControl(
        /* [in] */ const String& process,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean start,
        /* [in] */ const String& path,
        /* [in] */ IParcelFileDescriptor* fd,
        /* [in] */ Int32 profileType,
        /* [out] */ Boolean* result);

    CARAPI Shutdown(
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* result);

    CARAPI StopAppSwitches();

    CARAPI ResumeAppSwitches();

    CARAPI KillApplicationWithAppId(
        /* [in] */ const String& pkg,
        /* [in] */ Int32 appid);

    CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

    CARAPI GetProcessMemoryInfo(
        /* [in] */ ArrayOf<Int32>* pids,
        /* [out, callee] */ ArrayOf<IDebugMemoryInfo*>** memoryInfo);

    CARAPI OverridePendingTransition(
        /* [in] */ IBinder* token,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 enterAnim,
        /* [in] */ Int32 exitAnim);

    CARAPI IsUserAMonkey(
        /* [out] */ Boolean* result);

    CARAPI FinishHeavyWeightApp();

    CARAPI SetImmersive(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean immersive);

    CARAPI IsImmersive(
        /* [in] */ IBinder* token,
        /* [out] */ Boolean* result);

    CARAPI IsTopActivityImmersive(
        /* [out] */ Boolean* result);

    CARAPI CrashApplication(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 initialPid,
        /* [in] */ const String& packageName,
        /* [in] */ const String& message);

    CARAPI GetProviderMimeType(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 userId,
        /* [out] */ String* type);

    CARAPI NewUriPermissionOwner(
        /* [in] */ const String& name,
        /* [out] */ IBinder** binder);

    CARAPI GrantUriPermissionFromOwner(
        /* [in] */ IBinder* owner,
        /* [in] */ Int32 fromUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 mode);

    CARAPI RevokeUriPermissionFromOwner(
        /* [in] */ IBinder* owner,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 mode);

    CARAPI CheckGrantUriPermission(
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    // Cause the specified process to dump the specified heap.
    CARAPI DumpHeap(
        /* [in] */ const String& process,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean managed,
        /* [in] */ const String& path,
        /* [in] */ IParcelFileDescriptor* fd,
        /* [out] */ Boolean* result);

    CARAPI StartActivities(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ ArrayOf<String>* resolvedTypes,
        /* [in] */ IBinder* resultTo,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI GetFrontActivityScreenCompatMode(
        /* [out] */ Int32* mode);

    CARAPI SetFrontActivityScreenCompatMode(
        /* [in] */ Int32 mode);

    CARAPI GetPackageScreenCompatMode(
        /* [in] */ const String& packageName,
        /* [out] */ Int32* mode);

    CARAPI SetPackageScreenCompatMode(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 mode);

    CARAPI GetPackageAskScreenCompat(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI SetPackageAskScreenCompat(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean ask);

    // Multi-user APIs
    CARAPI SwitchUser(
        /* [in] */ Int32 userid,
        /* [out] */ Boolean* swith);

    CARAPI StopUser(
        /* [in] */ Int32 userid,
        /* [in] */ IStopUserCallback* cb,
        /* [out] */ Int32* result);

    CARAPI GetCurrentUser(
        /* [out] */ IUserInfo** user);

    CARAPI IsUserRunning(
        /* [in] */ Int32 userid,
        /* [in] */ Boolean orStopping,
        /* [out] */ Boolean* running);

    CARAPI GetRunningUserIds(
        /* [out, callee] */ ArrayOf<Int32>** ids);

    CARAPI RemoveSubTask(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 subTaskIndex,
        /* [out] */ Boolean* removed);

    CARAPI RemoveTask(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* removed);

    CARAPI RegisterProcessObserver(
        /* [in] */ IProcessObserver* observer);

    CARAPI UnregisterProcessObserver(
        /* [in] */ IProcessObserver* observer);

    CARAPI IsIntentSenderTargetedToPackage(
        /* [in] */ IIIntentSender* sender,
        /* [out] */ Boolean* result);

    CARAPI IsIntentSenderAnActivity(
        /* [in] */ IIIntentSender* sender,
        /* [out] */ Boolean* result);

    CARAPI UpdatePersistentConfiguration(
        /* [in] */ IConfiguration* values);

    CARAPI GetProcessPss(
        /* [in] */ ArrayOf<Int32>* pids,
        /* [out, callee] */ ArrayOf<Int64>** processPss);

    CARAPI ShowBootMessage(
        /* [in] */ ICharSequence* msg,
        /* [in] */ Boolean always);

    CARAPI DismissKeyguardOnNextActivity();

    CARAPI TargetTaskAffinityMatchesActivity(
        /* [in] */ IBinder* token,
        /* [in] */ const String& destAffinity,
        /* [out] */ Boolean* result);

    CARAPI NavigateUpTo(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* target,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [out] */ Boolean* result);

    // This is not public because you need to be very careful in how you
    // manage your activity to make sure it is always the uid you expect.
    CARAPI GetLaunchedFromUid(
        /* [in] */ IBinder* activityToken,
        /* [out] */ Int32* result);

    CARAPI RegisterUserSwitchObserver(
        /* [in] */ IUserSwitchObserver* observer);

    CARAPI UnregisterUserSwitchObserver(
        /* [in] */ IUserSwitchObserver* observer);

    CARAPI RequestBugReport();

    CARAPI InputDispatchingTimedOut(
        /* [in] */ Int32 pid,
        /* [in] */ Boolean aboveSystem,
        /* [out] */ Int64* value);

    CARAPI TestIsSystemReady(
        /* [out] */ Boolean* isSystemReady);

public:
    CARAPI_(AutoPtr<BroadcastQueue>) BroadcastQueueForIntent(
        /* [in] */ IIntent* intent);

    CARAPI_(AutoPtr<BroadcastRecord>) BroadcastRecordForReceiverLocked(
        /* [in] */ IBinder* receiver);

    CARAPI UpdateCpuStats();

    CARAPI UpdateCpuStatsNow();

    CARAPI BatteryNeedsCpuUpdate();

    CARAPI BatteryPowerChanged(
        /* [in] */ Boolean onBattery);

    CARAPI SetFocusedActivityLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI UpdateLruProcessLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean oomAdj);

    AutoPtr<ProcessRecord> GetProcessRecordLocked(
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid);

    CARAPI EnsurePackageDexOpt(
        /* [in] */ const String& packageName);

    CARAPI_(Boolean) IsNextTransitionForward();

    AutoPtr<ProcessRecord> StartProcessLocked(
        /* [in] */ const String& processName,
        /* [in] */ IApplicationInfo* info,
        /* [in] */ Boolean knownToBeDead,
        /* [in] */ Int32 intentFlags,
        /* [in] */ const String& hostingType,
        /* [in] */ IComponentName* hostingName,
        /* [in] */ Boolean allowWhileBooting,
        /* [in] */ Boolean isolated);

    CARAPI_(Boolean) IsAllowedWhileBooting(
        /* [in] */ IApplicationInfo* ai);

    CARAPI UpdateUsageStats(
        /* [in] */ ActivityRecord* resumedComponent,
        /* [in] */ Boolean resumed);

    /* add by Gary. start {{----------------------------------- */
    /* 2012-9-8 */
    /* launch the defualt launcher when the system boots for the first time */
    CARAPI SetDefaultLauncher(
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) StartHomeActivityLocked(
        /* [in] */ Int32 userId);

    CARAPI StartSetupActivityLocked();

    CARAPI_(AutoPtr<ICompatibilityInfo>) CompatibilityInfoForPackageLocked(
        /* [in] */ IApplicationInfo* ai);

    CARAPI EnforceNotIsolatedCaller(
        /* [in] */ const String& caller);

    CARAPI ReportResumedActivityLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI DoPendingActivityLaunchesLocked(
        /* [in] */ Boolean doResume);

    CARAPI AddRecentTaskLocked(
        /* [in] */ TaskRecord* task);

    CARAPI_(AutoPtr<ProcessRecord>) GetRecordForAppLocked(
        /* [in] */ IApplicationThread* thread);

    CARAPI AppDiedLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 pid,
        /* [in] */ IApplicationThread* thread);

    /**
     * If a stack trace dump file is configured, dump process stack traces.
     * @param clearTraces causes the dump file to be erased prior to the new
     *    traces being written, if true; when false, the new traces will be
     *    appended to any existing file content.
     * @param firstPids of dalvik VM processes to dump stack traces for first
     * @param lastPids of dalvik VM processes to dump stack traces for last
     * @param nativeProcs optional list of native process names to dump stack crawls
     * @return file containing stack traces, or null if no dump file is configured
     */
    static CARAPI_(AutoPtr<IFile>) DumpStackTraces(
        /* [in] */ Boolean clearTraces,
        /* [in] */ List< AutoPtr<IInteger32> >* firstPids,
        /* [in] */ IProcessStats* processStats,
        /* [in] */ HashMap<Int32, Boolean>* lastPids,
        /* [in] */ ArrayOf<String>* nativeProcs);

    CARAPI LogAppTooSlow(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int64 startTime,
        /* [in] */ const String& msg);

    CARAPI AppNotResponding(
        /* [in] */ ProcessRecord* app,
        /* [in] */ ActivityRecord* activity,
        /* [in] */ ActivityRecord* parent,
        /* [in] */ Boolean aboveSystem,
        /* [in] */ const String& annotation);

    CARAPI ShowLaunchWarningLocked(
        /* [in] */ ActivityRecord* cur,
        /* [in] */ ActivityRecord* next);

    CARAPI CloseSystemDialogsLocked(
        /* [in] */ const String& reason);

    CARAPI EnableScreenAfterBoot();

    CARAPI FinishBooting();

    CARAPI SendBootFastComplete();

    CARAPI EnsureBootCompleted();

    CARAPI GetIntentSenderLocked(
        /* [in] */ Int32 type,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ IBinder* token,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ ArrayOf<String>* resolvedTypes,
        /* [in] */ Int32 flags,
        /* [in] */ IBundle* options,
        /* [out] */ IIIntentSender** sender);

    CARAPI CancelIntentSenderLocked(
        /* [in] */ IIIntentSender* rec,
        /* [in] */ Boolean cleanActivity);

    CARAPI ForegroundTokenDied(
        /* [in] */ ForegroundToken* token);

    /**
     * This can be called with or without the global lock held.
     */
    CARAPI_(Int32) CheckComponentPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 owningUid,
        /* [in] */ Boolean exported);

    /**
     * Binder IPC calls go through the public entry point.
     * This can be called with or without the global lock held.
     */
    CARAPI_(Int32) CheckCallingPermission(
        /* [in] */ const String& permission);

    /**
     * This can be called with or without the global lock held.
     */
    CARAPI EnforceCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& func);

    CARAPI GrantUriPermissionUncheckedLocked(
        /* [in] */ Int32 targetUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI GrantUriPermissionLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI_(AutoPtr<NeededUriGrants>) CheckGrantUriPermissionFromIntentLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 mode,
        /* [in] */ NeededUriGrants* needed);

    CARAPI GrantUriPermissionUncheckedFromIntentLocked(
        /* [in] */ NeededUriGrants* needed,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI GrantUriPermissionFromIntentLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IIntent* intent,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI RemoveUriPermissionIfNeededLocked(
        /* [in] */ UriPermission* perm);

    CARAPI_(Int32) GetTaskForActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean onlyRoot);

    CARAPI SendPendingThumbnail(
        /* [in] */ ActivityRecord* r,
        /* [in] */ IBinder* token,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ ICharSequence* description,
        /* [in] */ Boolean always);

    CARAPI GenerateApplicationProvidersLocked(
        /* [in] */ ProcessRecord* app,
        /* [out] */ IObjectContainer** providers);

    CARAPI_(AutoPtr<CContentProviderConnection>) IncProviderCountLocked(
        /* [in] */ ProcessRecord* r,
        /* [in] */ ContentProviderRecord* cpr,
        /* [in] */ IBinder* externalProcessToken,
        /* [in] */ Boolean stable);

    CARAPI_(Boolean) DecProviderCountLocked(
        /* [in] */ CContentProviderConnection* conn,
        /* [in] */ ContentProviderRecord* cpr,
        /* [in] */ IBinder* externalProcessToken,
        /* [in] */ Boolean stable);

    static CARAPI InstallSystemProviders();

    CARAPI_(AutoPtr<ProcessRecord>) NewProcessRecordLocked(
        /* [in] */ IApplicationThread* thread,
        /* [in] */ IApplicationInfo* info,
        /* [in] */ const String& customProcess,
        /* [in] */ Boolean isolated);

    CARAPI_(AutoPtr<ProcessRecord>) AddAppLocked(
        /* [in] */ IApplicationInfo* info,
        /* [in] */ Boolean isolated);

    CARAPI_(Boolean) IsSleeping();

    CARAPI_(Boolean) CheckAppSwitchAllowedLocked(
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& name);

    CARAPI SetOpenGlTraceApp(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ const String& processName);

    CARAPI SetProfileApp(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ const String& processName,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ Boolean autoStopProfiler);

    CARAPI ShowSafeModeOverlay();

    CARAPI_(Boolean) TestIsSystemReady();

    CARAPI SystemReady(
        /* [in] */ IRunnable* goingCallback);

    CARAPI KillAppAtUsersRequest(
        /* [in] */ ProcessRecord* app,
        /* [in] */ IDialog* fromDialog);

    CARAPI StartAppProblemLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI SkipCurrentReceiverLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI AddErrorToDropBox(
        /* [in] */ const String& eventType,
        /* [in] */ ProcessRecord* process,
        /* [in] */ const String& processName,
        /* [in] */ ActivityRecord* activity,
        /* [in] */ ActivityRecord* parent,
        /* [in] */ const String& subject,
        /* [in] */ const String& report,
        /* [in] */ IFile* logFile,
        /* [in] */ IApplicationErrorReportCrashInfo* crashInfo);

    CARAPI_(AutoPtr<IIntent>) CreateAppErrorIntentLocked(
        /* [in] */ ProcessRecord* r,
        /* [in] */ Int64 timeMillis,
        /* [in] */ IApplicationErrorReportCrashInfo* crashInfo);

    static CARAPI_(Int32) OomAdjToImportance(
        /* [in] */ Int32 adj,
        /* [in] */ IActivityManagerRunningAppProcessInfo* currApp);

    CARAPI_(Boolean) DumpActivitiesLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll,
        /* [in] */ Boolean dumpClient,
        /* [in] */ const String& dumpPackage);

    CARAPI_(Boolean) DumpProcessesLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll,
        /* [in] */ const String& dumpPackage);

    CARAPI_(Boolean) DumpProcessesToGc(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean needSep,
        /* [in] */ Boolean dumpAll,
        /* [in] */ const String& dumpPackage);

    CARAPI_(Boolean) DumpOomLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll);

    CARAPI_(Boolean) DumpBroadcastsLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll,
        /* [in] */ const String& dumpPackage);

    CARAPI_(Boolean) DumpProvidersLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll,
        /* [in] */ const String& dumpPackage);

    CARAPI_(Boolean) DumpPendingIntentsLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll,
        /* [in] */ const String& dumpPackage);

    CARAPI DumpGraphicsHardwareUsage(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    CARAPI DumpDbInfo(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    static CARAPI DumpMemItems(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ List< AutoPtr<MemItem> >* items,
        /* [in] */ Boolean sort);

    static CARAPI AppendMemBucket(
        /* [in] */ StringBuilder& out,
        /* [in] */ Int64 memKB,
        /* [in] */ const String& label,
        /* [in] */ Boolean stackLike);

    CARAPI DumpApplicationMemoryUsage(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Boolean brief,
        /* [in] */ IPrintWriter* categoryPw,
        /* [in] */ StringBuilder& outTag,
        /* [in] */ StringBuilder& outStack);

    CARAPI Monitor();

    CARAPI OnCoreSettingsChange(
        /* [in] */ IBundle* settings);

    CARAPI SendUserSwitchBroadcastsLocked(
        /* [in] */ Int32 oldUserId,
        /* [in] */ Int32 newUserId);

    CARAPI DispatchUserSwitch(
        /* [in] */ UserStartedState* uss,
        /* [in] */ Int32 oldUserId,
        /* [in] */ Int32 newUserId);

    CARAPI TimeoutUserSwitch(
        /* [in] */ UserStartedState* uss,
        /* [in] */ Int32 oldUserId,
        /* [in] */ Int32 newUserId);

    CARAPI SendContinueUserSwitchLocked(
        /* [in] */ UserStartedState* uss,
        /* [in] */ Int32 oldUserId,
        /* [in] */ Int32 newUserId);

    CARAPI UserInitialized(
        /* [in] */ UserStartedState* uss,
        /* [in] */ Int32 newUserId);

    CARAPI ContinueUserSwitch(
        /* [in] */ UserStartedState* uss,
        /* [in] */ Int32 oldUserId,
        /* [in] */ Int32 newUserId);

    CARAPI CompleteSwitchAndInitalize(
        /* [in] */ UserStartedState* uss,
        /* [in] */ Int32 newUserId,
        /* [in] */ Boolean clearInitializing,
        /* [in] */ Boolean clearSwitching);

    CARAPI FinishUserSwitch(
        /* [in] */ UserStartedState* uss);

    CARAPI FinishUserStop(
        /* [in] */ UserStartedState* uss);

    CARAPI_(Int32) GetCurrentUserIdLocked();

    CARAPI_(Boolean) IsUserRunningLocked(
        /* [in] */ Int32 userId,
        /* [in] */ Boolean orStopped);

    CARAPI_(AutoPtr< ArrayOf<Int32> >) GetUsersLocked();

    CARAPI_(AutoPtr<CUserManagerService>) GetUserManagerLocked();

    CARAPI_(AutoPtr<IApplicationInfo>) GetAppInfoForUser(
        /* [in] */ IApplicationInfo* aInfo,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<IActivityInfo>) GetActivityInfoForUser(
        /* [in] */ IActivityInfo* aInfo,
        /* [in] */ Int32 userId);

    CARAPI StartActivityInPackage(
        /* [in] */ Int32 uid,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 startFlags,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* status);

    CARAPI StartActivitiesInPackage(
        /* [in] */ Int32 uid,
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ ArrayOf<String>* resolvedTypes,
        /* [in] */ IBinder* resultTo,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* status);

    CARAPI StartServiceInPackage(
        /* [in] */ Int32 uid,
        /* [in] */ IIntent* service,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 userId,
        /* [out] */ IComponentName** componentName);

    CARAPI BroadcastIntentInPackage(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* map,
        /* [in] */ const String& requiredPermission,
        /* [in] */ Boolean serialized,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* status);

    CARAPI_(Boolean) UpdateConfigurationLocked(
        /* [in] */ IConfiguration* values,
        /* [in] */ ActivityRecord* starting,
        /* [in] */ Boolean persistent,
        /* [in] */ Boolean initLocale);

    CARAPI_(Boolean) IsSingleton(
        /* [in] */ const String& componentProcessName,
        /* [in] */ IApplicationInfo* aInfo,
        /* [in] */ const String& className,
        /* [in] */ Int32 flags);

    /**
     * Schedule the execution of all pending app GCs.
     */
    CARAPI ScheduleAppGcsLocked();

    CARAPI_(Boolean) UpdateOomAdjLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI UpdateOomAdjLocked();

    CARAPI TrimApplications();

    CARAPI_(void) HandleShowErrorMsg(
        /* [in] */ StringObjectHashMap* data);

    CARAPI_(void) HandleShowNotRespondingMsg(
        /* [in] */ StringObjectHashMap* data,
        /* [in] */ Int32 arg1);

    CARAPI_(void) HandleShowStrictModeViolationMsg(
        /* [in] */ StringObjectHashMap* map);

    CARAPI_(void) HandleShowFactoryErrorMsg(
        /* [in] */ ICharSequence* msg);

    CARAPI_(void) HandleUpdateConfigurationMsg(
        /* [in] */ IConfiguration* config);

    CARAPI_(void) HandleGCBackgroundProcessesMsg();

    CARAPI_(void) HandleWaitForDebuggerMsg(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 arg1);

    CARAPI_(void) HandleServiceTimeoutMsg(
        /* [in] */ ProcessRecord* processRecord);

    CARAPI_(void) HandleUpdateTimeZone();

    CARAPI_(void) HandleClearDnsCache();

    CARAPI_(void) HandleUpdateHttpProxy(
        /* [in] */ IProxyProperties* proxy);

    CARAPI_(void) HandleShowUidErrorMsg();

    CARAPI_(void) HandleIMFeelingLuckyMsg();

    CARAPI_(void) HandleProcStartTimeoutMsg(
        /* [in] */ ProcessRecord* pr);

    CARAPI_(void) HandleDoPendingActivityLaunchesMsg();

    CARAPI_(void) HandleKillApplicationMsg(
        /* [in] */ Int32 appid,
        /* [in] */ Boolean restart,
        /* [in] */ const String& pkg);

    CARAPI_(void) HandlePostHeavyNotificationMsg(
        /* [in] */ ActivityRecord* activityRecord);

    CARAPI_(void) HandleCancelHeavyNotificationMsg(
        /* [in] */ Int32 id);

    CARAPI_(void) HandleCheckExcessiveWakeLocksMsg();

    CARAPI_(void) HandleShowCompatModeDialogMsg(
        /* [in] */ ActivityRecord* activityRecord);

    CARAPI_(void) HandleReportMemUsage();

private:
    /**
     * Initialize the application bind args. These are passed to each
     * process when the bindApplication() IPC is sent to the process. They're
     * lazily setup to make sure the services are running when they're asked for.
     */
    CARAPI GetCommonServicesLocked(
        /* [out] */ IObjectStringMap** services);

    CARAPI UpdateLruProcessInternalLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 bestPos);

    CARAPI StartProcessLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ const String& hostingType,
        /* [in] */ const String& hostingNameStr);

    AutoPtr<IActivityInfo> ResolveActivityInfo(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI DispatchProcessesChanged();

    CARAPI DispatchProcessDied(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid);

    /// patch for videoplayer crashed

    /**
     * Main function for removing an existing process from the activity manager
     * as a result of that process going away.  Clears out all connections
     * to the process.
     */
    CARAPI HandleAppDiedLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean restarting,
        /* [in] */ Boolean allowRestart);

    CARAPI_(Int32) GetLRURecordIndexForAppLocked(
        /* [in] */ IApplicationThread* thread);

    static CARAPI_(void) DumpStackTraces(
        /* [in] */ const String& tracesPath,
        /* [in] */ List< AutoPtr<IInteger32> >* firstPids,
        /* [in] */ IProcessStats* processStats,
        /* [in] */ HashMap<Int32, Boolean>* lastPids,
        /* [in] */ ArrayOf<String>* nativeProcs);

    CARAPI ForceStopPackageLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    CARAPI ForceStopUserLocked(
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) KillPackageProcessesLocked(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 appId,
        /* [in] */ Int32 userId,
        /* [in] */ Int32 minOomAdj,
        /* [in] */ Boolean callerWillRestart,
        /* [in] */ Boolean allowRestart,
        /* [in] */ Boolean doit,
        /* [in] */ Boolean evenPersistent,
        /* [in] */ const String& reason);

    CARAPI_(Boolean) ForceStopPackageLocked(
        /* [in] */ const String& name,
        /* [in] */ Int32 appId,
        /* [in] */ Boolean callerWillRestart,
        /* [in] */ Boolean purgeCache,
        /* [in] */ Boolean doit,
        /* [in] */ Boolean evenPersistent,
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) RemoveProcessLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean callerWillRestart,
        /* [in] */ Boolean allowRestart,
        /* [in] */ const String& reason);

    CARAPI ProcessStartTimedOutLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(Boolean) AttachApplicationLocked(
        /* [in] */ IApplicationThread* thread,
        /* [in] */ Int32 pid);

    CARAPI_(AutoPtr<ActivityRecord>) GetCallingRecordLocked(
        /* [in] */ IBinder* token);

    CARAPI_(Boolean) CheckHoldingPermissionsLocked(
        /* [in] */ IIPackageManager* pm,
        /* [in] */ IProviderInfo* pi,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags);

    CARAPI_(Boolean) CheckUriPermissionLocked(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckGrantUriPermissionLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ Int32 lastTargetUid,
        /* [out] */ Int32* permission);

    CARAPI RevokeUriPermissionLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI_(AutoPtr<TaskRecord>) TaskForIdLocked(
        /* [in] */ Int32 id);

    CARAPI CleanUpRemovedTaskLocked(
        /* [in] */ TaskRecord* tr,
        /* [in] */ Int32 flags);

    CARAPI_(Int32) FindAffinityTaskTopLocked(
        /* [in] */ Int32 startIndex,
        /* [in] */ const String& affinity);

    CARAPI MoveTaskBackwardsLocked(
        /* [in] */ Int32 task);

    CARAPI_(String) CheckContentProviderPermissionLocked(
        /* [in] */ IProviderInfo* cpi,
        /* [in] */ ProcessRecord* r);

    CARAPI GetContentProviderImpl(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ const String& name,
        /* [in] */ IBinder* token,
        /* [in] */ Boolean stable,
        /* [in] */ Int32 userId,
        /* [out] */ IContentProviderHolder** providerHolder);

    CARAPI GetContentProviderExternalUnchecked(
        /* [in] */ const String& name,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 userId,
        /* [out] */ IContentProviderHolder** providerHolder);

    CARAPI RemoveContentProviderExternalUnchecked(
        /* [in] */ const String& name,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 userId);

    CARAPI ComeOutOfSleepIfNeededLocked();

    CARAPI UpdateEventDispatchingLocked();

    CARAPI_(Boolean) KillProcessesBelowAdj(
        /* [in] */ Int32 belowAdj,
        /* [in] */ const String& reason);

    CARAPI RetrieveSettings();

    static CARAPI_(AutoPtr<IFile>) GetCalledPreBootReceiversFile();

    static CARAPI_(AutoPtr<List< AutoPtr<IComponentName> > >) ReadLastDonePreBootReceivers();

    static CARAPI WriteLastDonePreBootReceivers(
        /* [in] */ const List< AutoPtr<IComponentName> >* list);

    CARAPI_(Boolean) MakeAppCrashingLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ const String& shortMsg,
        /* [in] */ const String& longMsg,
        /* [in] */ const String& stackTrace);

    CARAPI MakeAppNotRespondingLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ const String& activity,
        /* [in] */ const String& shortMsg,
        /* [in] */ const String& longMsg);

    CARAPI_(AutoPtr<IActivityManagerProcessErrorStateInfo>) GenerateProcessError(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 condition,
        /* [in] */ const String& activity,
        /* [in] */ const String& shortMsg,
        /* [in] */ const String& longMsg,
        /* [in] */ const String& stackTrace);

    CARAPI_(Boolean) HandleAppCrashLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI LogStrictModeViolationToDropBox(
        /* [in] */ ProcessRecord* process,
        /* [in] */ IStrictModeViolationInfo* info);

    /**
     * @param app object of some object (as stored in {@link com.android.internal.os.RuntimeInit})
     * @return the corresponding {@link ProcessRecord} object, or null if none could be found
     */
    CARAPI_(AutoPtr<ProcessRecord>) FindAppProcess(
        /* [in] */ IBinder* app,
        /* [in] */ const String& reason);

    /**
     * Utility function for addErrorToDropBox and handleStrictModeViolation's logging
     * to append various headers to the dropbox log text.
     */
    CARAPI AppendDropBoxProcessHeaders(
        /* [in] */ ProcessRecord* process,
        /* [in] */ const String& processName,
        /* [in] */ StringBuilder& sb);

    static CARAPI_(String) ProcessClass(
        /* [in] */ ProcessRecord* process);

    CARAPI CrashApplication(
        /* [in] */ ProcessRecord* r,
        /* [in] */ IApplicationErrorReportCrashInfo* crashInfo);

    CARAPI_(AutoPtr<IApplicationErrorReport>) CreateAppErrorReportLocked(
        /* [in] */ ProcessRecord* r,
        /* [in] */ Int64 timeMillis,
        /* [in] */ IApplicationErrorReportCrashInfo* crashInfo);

    CARAPI FillInProcMemInfo(
        /* [in] */ ProcessRecord* app,
        /* [in] */ IActivityManagerRunningAppProcessInfo* outInfo);

    static CARAPI DumpHistoryList(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ List< AutoPtr<ActivityRecord> >* list,
        /* [in] */ const String& prefix,
        /* [in] */ const String& label,
        /* [in] */ Boolean complete,
        /* [in] */ Boolean brief,
        /* [in] */ Boolean client,
        /* [in] */ const String& dumpPackage);

    static CARAPI_(String) BuildOomTag(
        /* [in] */ const String& prefix,
        /* [in] */ const String& space,
        /* [in] */ Int32 val,
        /* [in] */ Int32 base);

    static CARAPI_(Int32) DumpProcessList(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ CActivityManagerService* service,
        /* [in] */ List< AutoPtr<ProcessRecord> >* list,
        /* [in] */ const String& prefix,
        /* [in] */ const String& normalLabel,
        /* [in] */ const String& persistentLabel,
        /* [in] */ const String& dumpPackage);

    static CARAPI_(Boolean) DumpProcessOomList(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ CActivityManagerService* service,
        /* [in] */ List< AutoPtr<ProcessRecord> >* origList,
        /* [in] */ const String& prefix,
        /* [in] */ const String& normalLabel,
        /* [in] */ const String& persistentLabel,
        /* [in] */ Boolean inclDetails,
        /* [in] */ const String& dumpPackage);

    CARAPI_(AutoPtr< List< AutoPtr<ProcessRecord> > >) CollectProcesses(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 start,
        /* [in] */ ArrayOf<String>* args);

    CARAPI_(Boolean) CheckAppInLaunchingProvidersLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean alwaysBad);

    CARAPI_(Boolean) IsPendingBroadcastProcessLocked(
        /* [in] */ Int32 pid);

    CARAPI_(void) SkipPendingBroadcastLocked(
        /* [in] */ Int32 pid);

    CARAPI_(Boolean) SendPendingBroadcastsLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) RemoveReceiverLocked(
        /* [in] */ ReceiverList* rl);

    CARAPI VerifyBroadcastLocked(
        /* [in] */ IIntent* _intent,
        /* [out] */ IIntent** intent);

    CARAPI FinishInstrumentationLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI PerformAppGcLocked(
        /* [in] */ ProcessRecord* app);

    /**
     * Returns true if things are idle enough to perform GCs.
     */
    CARAPI_(Boolean) CanGcNowLocked();

    /**
     * Perform GCs on all processes that are waiting for it, but only
     * if things are idle.
     */
    CARAPI PerformAppGcsLocked();

    /**
     * If all looks good, perform GCs on all processes waiting for them.
     */
    CARAPI PerformAppGcsIfAppropriateLocked();

    CARAPI AddProcessToGcListLocked(
        /* [in] */ ProcessRecord* proc);

    CARAPI ScheduleAppGcLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI CheckExcessivePowerUsageLocked(
        /* [in] */ Boolean doKills);

protected:
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    CARAPI_(Boolean) DumpProvider(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll);

    CARAPI_(Boolean) DumpActivity(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int32 opti,
        /* [in] */ Boolean dumpAll);

    CARAPI DumpActivity(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ActivityRecord* r,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Boolean dumpAll);

    static CARAPI_(Boolean) ScanArgs(
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ const String& value);

    CARAPI_(Boolean) RemoveDyingProviderLocked(
        /* [in] */ ProcessRecord* proc,
        /* [in] */ ContentProviderRecord* cpr,
        /* [in] */ Boolean always);

    ///**
    // * Main code for cleaning up a process when it has gone away.  This is
    // * called both as a result of the process dying, or directly when stopping
    // * a process when running in single process mode.
    // */
    CARAPI CleanUpApplicationRecordLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean restarting,
        /* [in] */ Boolean allowRestart,
        /* [in] */ Int32 index);

    CARAPI_(AutoPtr< List< AutoPtr<IIntent> > >) GetStickiesLocked(
        /* [in] */ const String& action,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ List< AutoPtr<IIntent> >* cur,
        /* [in] */ Int32 userId);

    CARAPI_(void) SendPackageBroadcastLocked(
        /* [in] */ Int32 cmd,
        /* [in] */ ArrayOf<String>* capsules,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<BroadcastFilterList>) CollectReceiverComponents(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ ArrayOf<Int32>* users);

    CARAPI BroadcastIntentLocked(
        /* [in] */ ProcessRecord* callerApp,
        /* [in] */ const String& callerPackage,
        /* [in] */ IIntent *intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* map,
        /* [in] */ const String& requiredPermission,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* result);

    CARAPI_(Boolean) FinishReceiverLocked(
        /* [in] */ IBinder* receiver,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* resultExtras,
        /* [in] */ Boolean resultAbort,
        /* [in] */ Boolean _explicit);

    CARAPI ReportStartInstrumentationFailure(
        /* [in] */ IInstrumentationWatcher* watcher,
        /* [in] */ IComponentName* cn,
        /* [in] */ const String& report);

    static CARAPI_(Boolean) ShouldShowDialogs(
        /* [in] */ IConfiguration* config);

    CARAPI SaveLocaleLocked(
        /* [in] */ ILocale* l,
        /* [in] */ Boolean isDiff,
        /* [in] */ Boolean isPersist);

    CARAPI_(AutoPtr<BroadcastQueue>) IsReceivingBroadcast(
        /* [in] */ ProcessRecord* app);

    CARAPI_(Int32) ComputeOomAdjLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 hiddenAdj,
        /* [in] */ Int32 clientHiddenAdj,
        /* [in] */ Int32 emptyAdj,
        /* [in] */ ProcessRecord* TOP_APP,
        /* [in] */ Boolean recursed,
        /* [in] */ Boolean doingAll);

    CARAPI_(Boolean) UpdateOomAdjLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 hiddenAdj,
        /* [in] */ Int32 clientHiddenAdj,
        /* [in] */ Int32 emptyAdj,
        /* [in] */ ProcessRecord* TOP_APP,
        /* [in] */ Boolean doingAll);

    CARAPI_(AutoPtr<ActivityRecord>) ResumedAppLocked();

    CARAPI StopProfilerLocked(
        /* [in] */ ProcessRecord* proc,
        /* [in] */ const String& path,
        /* [in] */ Int32 profileType);

    CARAPI ClearProfilerLocked();

    CARAPI_(AutoPtr<ProcessRecord>) FindProcessLocked(
        /* [in] */ const String& process,
        /* [in] */ Int32 userId,
        /* [in] */ const String& callName);

    CARAPI_(Int32) StopUserLocked(
        /* [in] */ Int32 userId,
        /* [in] */ IStopUserCallback* callback);

    CARAPI UpdateStartedUserArrayLocked();

    CARAPI_(Boolean) UserExists(
        /* [in] */ Int32 userId);

    CARAPI CheckValidCaller(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) ApplyUserId(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId);

public:
    static const String TAG;
    static const String TAG_MU;
    static const Boolean DEBUG;
    static const Boolean localLOGV;
    static const Boolean DEBUG_SWITCH;
    static const Boolean DEBUG_TASKS;
    static const Boolean DEBUG_THUMBNAILS;
    static const Boolean DEBUG_PAUSE;
    static const Boolean DEBUG_OOM_ADJ;
    static const Boolean DEBUG_TRANSITION;
    static const Boolean DEBUG_BROADCAST;
    static const Boolean DEBUG_BACKGROUND_BROADCAST;
    static const Boolean DEBUG_BROADCAST_LIGHT;
    static const Boolean DEBUG_SERVICE;
    static const Boolean DEBUG_SERVICE_EXECUTING;
    static const Boolean DEBUG_VISBILITY;
    static const Boolean DEBUG_PROCESSES;
    static const Boolean DEBUG_PROCESS_OBSERVERS;
    static const Boolean DEBUG_CLEANUP;
    static const Boolean DEBUG_PROVIDER;
    static const Boolean DEBUG_URI_PERMISSION;
    static const Boolean DEBUG_USER_LEAVING;
    static const Boolean DEBUG_RESULTS;
    static const Boolean DEBUG_BACKUP;
    static const Boolean DEBUG_CONFIGURATION;
    static const Boolean DEBUG_POWER;
    static const Boolean DEBUG_POWER_QUICK;
    static const Boolean DEBUG_MU;
    static const Boolean VALIDATE_TOKENS;
    static const Boolean SHOW_ACTIVITY_START_TIME;

    // Control over CPU and battery monitoring.
    static const Int64 BATTERY_STATS_TIME;      // write battery stats every 30 minutes.
    static const Boolean MONITOR_CPU_USAGE;
    static const Int64 MONITOR_CPU_MIN_TIME;        // don't sample cpu less than every 5 seconds.
    static const Int64 MONITOR_CPU_MAX_TIME;    // wait possibly forever for next cpu sample.
    static const Boolean MONITOR_THREAD_CPU_USAGE;

    // The flags that are set for all calls we make to the package manager.
    static const Int32 STOCK_PM_FLAGS;

    static const Boolean IS_USER_BUILD;

    // Maximum number of recent tasks that we can remember.
    static const Int32 MAX_RECENT_TASKS = 20;

    // Amount of time after a call to stopAppSwitches() during which we will
    // prevent further untrusted switches from happening.
    static const Int64 APP_SWITCH_DELAY_TIME = 5 * 1000;

    // How long we wait for a launched process to attach to the activity manager
    // before we decide it's never going to come up for real.
    static const Int32 PROC_START_TIMEOUT = 10 * 1000;

    // How long we wait for a launched process to attach to the activity manager
    // before we decide it's never going to come up for real, when the process was
    // started with a wrapper for instrumentation (such as Valgrind) because it
    // could take much longer than usual.
    static const Int32 PROC_START_TIMEOUT_WITH_WRAPPER = 300 * 1000;

    // How long to wait after going idle before forcing apps to GC.
    static const Int32 GC_TIMEOUT = 5 * 1000;

    // The minimum amount of time between successive GC requests for a process.
    static const Int32 GC_MIN_INTERVAL = 60  *1000;

    // The rate at which we check for apps using excessive power -- 15 mins.
    static const Int32 POWER_CHECK_DELAY;

    // The minimum sample duration we will allow before deciding we have
    // enough data on wake locks to start killing things.
    static const Int32 WAKE_LOCK_MIN_CHECK_DURATION;

    // The minimum sample duration we will allow before deciding we have
    // enough data on CPU usage to start killing things.
    static const Int32 CPU_MIN_CHECK_DURATION;

    // How long we allow a receiver to run before giving up on it.
    static const Int32 BROADCAST_FG_TIMEOUT;
    static const Int32 BROADCAST_BG_TIMEOUT;

    // How long we wait until we timeout on key dispatching.
    static const Int32 KEY_DISPATCHING_TIMEOUT;

    // How long we wait until we timeout on key dispatching during instrumentation.
    static const Int32 INSTRUMENTATION_KEY_DISPATCHING_TIMEOUT;

    // Amount of time we wait for observers to handle a user switch before
    // giving up on them and unfreezing the screen.
    static const Int32 USER_SWITCH_TIMEOUT;

    // Maximum number of users we allow to be running at a time.
    static const Int32 MAX_RUNNING_USERS;

    static const Int32 MY_PID;

    static const AutoPtr< ArrayOf<String> > EMPTY_STRING_ARRAY;

    AutoPtr<ActivityStack> mMainStack;

    List< AutoPtr<PendingActivityLaunch> > mPendingActivityLaunches;

    AutoPtr<BroadcastQueue> mFgBroadcastQueue;
    AutoPtr<BroadcastQueue> mBgBroadcastQueue;
    // Convenient for easy iteration over the queues. Foreground is first
    // so that dispatch of foreground broadcasts gets precedence.
    AutoPtr< ArrayOf<BroadcastQueue*> > mBroadcastQueues;// new BroadcastQueue[2];

    /**
     * Activity we have told the window manager to have key focus.
     */
    AutoPtr<ActivityRecord> mFocusedActivity;
    /**
     * List of intents that were used to start the most recent tasks.
     */
    List< AutoPtr<TaskRecord> > mRecentTasks;

    /**
     * Process management.
     */
    AutoPtr<ProcessList> mProcessList;

    /**
     * All of the applications we currently have running organized by name.
     * The keys are strings of the application package name (as
     * returned by the package manager), and the keys are ApplicationRecord
     * objects.
     */
    AutoPtr< ProcessMap< AutoPtr<ProcessRecord> > > mProcessNames;

    /**
     * The currently running isolated processes.
     */
    HashMap<Int32, AutoPtr<ProcessRecord> > mIsolatedProcesses;

    /**
     * Counter for assigning isolated process uids, to avoid frequently reusing the
     * same ones.
     */
    Int32 mNextIsolatedProcessUid;

    /**
     * The currently running heavy-weight process, if any.
     */
    AutoPtr<ProcessRecord> mHeavyWeightProcess;

    /**
     * The last time that various processes have crashed.
     */
    AutoPtr< ProcessMap<Int64> > mProcessCrashTimes;

    /**
     * Set of applications that we consider to be bad, and will reject
     * incoming broadcasts from (which the user has no control over).
     * Processes are added to this set when they have crashed twice within
     * a minimum amount of time; they are removed from it when they are
     * later restarted (hopefully due to some user action).  The value is the
     * time it was added to the list.
     */
    AutoPtr< ProcessMap<Int64> > mBadProcesses;

    /**
     * All of the processes we currently have running organized by pid.
     * The keys are the pid running the application.
     *
     * <p>NOTE: This object is protected by its own lock, NOT the global
     * activity manager lock!
     */
    HashMap<Int32, AutoPtr<ProcessRecord> > mPidsSelfLocked;

    HashMap<Int32, AutoPtr<ForegroundToken> > mForegroundProcesses;

    /**
     * List of records for processes that someone had tried to start before the
     * system was ready.  We don't start them at that point, but ensure they
     * are started by the time booting is complete.
     */
    List< AutoPtr<ProcessRecord> > mProcessesOnHold;

    /**
     * List of records for processes that we have started and are waiting
     * for them to call back.  This is really only needed when running in
     * single processes mode, in which case we do not have a unique pid for
     * each process.
     */
    List<AutoPtr<ProcessRecord> > mStartingProcesses;

    /**
     * List of persistent applications that are in the process
     * of being started.
     */
    List< AutoPtr<ProcessRecord> > mPersistentStartingProcesses;

    /**
     * Processes that are being forcibly torn down.
     */
    List< AutoPtr<ProcessRecord> > mRemovedProcesses;

    /**
     * List of running applications, sorted by recent usage.
     * The first entry in the list is the least recently used.
     * It contains ApplicationRecord objects.  This list does NOT include
     * any persistent application records (since we never want to exit them).
     */
    List< AutoPtr<ProcessRecord> > mLruProcesses;

    /**
     * List of processes that should gc as soon as things are idle.
     */
    List< AutoPtr<ProcessRecord> > mProcessesToGc;

    /**
     * This is the process holding what we currently consider to be
     * the "home" activity.
     */
    AutoPtr<ProcessRecord> mHomeProcess;

    /**
     * This is the process holding the activity the user last visited that
     * is in a different process from the one they are currently in.
     */
    AutoPtr<ProcessRecord> mPreviousProcess;

    /**
     * The time at which the previous process was last visible.
     */
    Int64 mPreviousProcessVisibleTime;

    /**
     * Which uses have been started, so are allowed to run code.
     */
    HashMap<Int32, AutoPtr<UserStartedState> > mStartedUsers;

    /**
     * LRU list of history of current users.  Most recently current is at the end.
     */
    List< AutoPtr<IInteger32> > mUserLru;

    /**
     * Constant array of the users that are currently started.
     */
    AutoPtr< ArrayOf<Int32> > mStartedUserArray;

    /**
     * Registered observers of the user switching mechanics.
     */
    //AutoPtr< RemoteCallbackList<IUserSwitchObserver> > mUserSwitchObservers;
    AutoPtr<IRemoteCallbackList> mUserSwitchObservers;

    /**
     * Currently active user switch.
     */
    AutoPtr<IInterface> mCurUserSwitchCallback;

    /**
     * Packages that the user has asked to have run in screen size
     * compatibility mode instead of filling the screen.
     */
    AutoPtr<CompatModePackages> mCompatModePackages;

    /**
     * Set of PendingResultRecord objects that are currently active.
     */
    //HashSet mPendingResultRecords = new HashSet();

    /**
     * Set of IntentSenderRecord objects that are currently active.
     */
    typedef HashMap< AutoPtr<CPendingIntentRecordKey>, AutoPtr<IWeakReference> > PendingIntentRecordHashMap;
    PendingIntentRecordHashMap mIntentSenderRecords;

     /**
     * Keeps track of all IIntentReceivers that have been registered for
     * broadcasts.  Hash keys are the receiver IBinder, hash value is
     * a ReceiverList.
     */
    HashMap<AutoPtr<IIntentReceiver>, AutoPtr<ReceiverList> > mRegisteredReceivers;

    /**
     * Resolver for broadcast intents to registered receivers.
     * Holds BroadcastFilter (subclass of IntentFilter).
     */
    AutoPtr<ReceiverResolver> mReceiverResolver;

    /**
     * State of all active sticky broadcasts per user.  Keys are the action of the
     * sticky Intent, values are an ArrayList of all broadcasted intents with
     * that action (which should usually be one).  The SparseArray is keyed
     * by the user ID the sticky is for, and can include UserHandle.USER_ALL
     * for stickies that are sent to all users.
     */
    typedef List< AutoPtr<IIntent> > StringIntentList;
    typedef HashMap<String, AutoPtr<StringIntentList> > StringIntentMap;
    typedef HashMap<Int32, AutoPtr<StringIntentMap> > StickyBroadcastHashMap;
    typedef typename StickyBroadcastHashMap::Iterator StickyBroadcastIterator;
    StickyBroadcastHashMap mStickyBroadcasts;

    AutoPtr<ActiveServices> mServices;

    /**
     * Backup/restore process management
     */
    String mBackupAppName;
    AutoPtr<BackupRecord> mBackupTarget;

    /**
     * List of PendingThumbnailsRecord objects of clients who are still
     * waiting to receive all of the thumbnails for a task.
     */
    List< AutoPtr<PendingThumbnailsRecord> > mPendingThumbnails;

    /**
     * List of HistoryRecord objects that have been finished and must
     * still report back to a pending thumbnail receiver.
     */
    List< AutoPtr<ActivityRecord> > mCancelledThumbnails;

    AutoPtr<ProviderMap> mProviderMap;

    /**
     * List of content providers who have clients waiting for them.  The
     * application is currently being launched and the provider will be
     * removed from this list once it is published.
     */
    List< AutoPtr<ContentProviderRecord> > mLaunchingProviders;

    AutoPtr<CoreSettingsObserver> mCoreSettingsObserver;

    /**
     * All information we have collected about the runtime performance of
     * any user id that can impact battery performance.
     */
    AutoPtr<CBatteryStatsService> mBatteryStatsService;

    /**
     * information about component usage
     */
    AutoPtr<CUsageStatsService> mUsageStatsService;

    /**
     * Current configuration information.  HistoryRecord objects are given
     * a reference to this object to indicate which configuration they are
     * currently running in, so this object must be kept immutable.
     */
    AutoPtr<IConfiguration> mConfiguration;

    /**
     * Current sequencing integer of the configuration, for skipping old
     * configurations.
     */
    Int32 mConfigurationSeq;

    /**
     * Hardware-reported OpenGLES version.
     */
    Int32 GL_ES_VERSION;

    /**
     * List of initialization arguments to pass to all processes when binding applications to them.
     * For example, references to the commonly used services.
     */
    HashMap<String, AutoPtr<IInterface> > mAppBindArgs;

    /**
     * Temporary to avoid allocations.  Protected by main lock.
     */
    AutoPtr<StringBuilder> mStringBuilder;

    /**
     * Used to control how we initialize the service.
     */
    Boolean mStartRunning;
    AutoPtr<IComponentName> mTopComponent;
    String mTopAction;
    String mTopData;
    Boolean mProcessesReady;
    Boolean mSystemReady;
    Boolean mBooting;
    Boolean mWaitingUpdate;
    Boolean mDidUpdate;
    Boolean mOnBattery;
    Boolean mLaunchWarningShown;

    AutoPtr<IContext> mContext;
    //todo: sSystemContext maybe not its memeber
    static AutoPtr<IContext> sSystemContext;

    Int32 mFactoryTest;

    Boolean mCheckedForSetup;

    /**
     * The time at which we will allow normal application switches again,
     * after a call to {@link #stopAppSwitches()}.
     */
    Int64 mAppSwitchesAllowedTime;

    /**
     * This is set to true after the first switch after mAppSwitchesAllowedTime
     * is set; any switches after that will clear the time.
     */
    Boolean mDidAppSwitch;

    /**
     * Last time (in realtime) at which we checked for power usage.
     */
    Int64 mLastPowerCheckRealtime;

    /**
     * Last time (in uptime) at which we checked for power usage.
     */
    Int64 mLastPowerCheckUptime;

    /**
     * Set while we are wanting to sleep, to prevent any
     * activities from being started/resumed.
     */
    Boolean mSleeping;

    /**
     * State of external calls telling us if the device is asleep.
     */
    Boolean mWentToSleep;

    /**
     * State of external call telling us if the lock screen is shown.
     */
    Boolean mLockScreenShown;

    /**
     * Set if we are shutting down the system, similar to sleeping.
     */
    Boolean mShuttingDown;

    /**
     * Task identifier that activities are currently being started
     * in.  Incremented each time a new task is created.
     * todo: Replace this with a TokenSpace class that generates non-repeating
     * integers that won't wrap.
     */
    Int32 mCurTask;

    /**
     * Current sequence id for oom_adj computation traversal.
     */
    Int32 mAdjSeq;

    /**
     * Current sequence id for process LRU updating.
     */
    Int32 mLruSeq;

    /**
     * Keep track of the non-hidden/empty process we last found, to help
     * determine how to distribute hidden/empty processes next time.
     */
    Int32 mNumNonHiddenProcs;

    /**
     * Keep track of the number of hidden procs, to balance oom adj
     * distribution between those and empty procs.
     */
    Int32 mNumHiddenProcs;

    /**
     * Keep track of the number of service processes we last found, to
     * determine on the next iteration which should be B services.
     */
    Int32 mNumServiceProcs;
    Int32 mNewNumServiceProcs;

    /**
     * System monitoring: number of processes that died since the last
     * N procs were started.
     */
    AutoPtr< ArrayOf<Int32> > mProcDeaths;

    /**
     * This is set if we had to do a delayed dexopt of an app before launching
     * it, to increasing the ANR timeouts in that case.
     */
    Boolean mDidDexOpt;

    String mDebugApp;
    Boolean mWaitForDebugger;
    Boolean mDebugTransient;
    String mOrigDebugApp;
    Boolean mOrigWaitForDebugger;
    Boolean mAlwaysFinishActivities;
    AutoPtr<IActivityController> mController;
    String mProfileApp;
    AutoPtr<ProcessRecord> mProfileProc;
    String mProfileFile;
    AutoPtr<IParcelFileDescriptor> mProfileFd;
    Int32 mProfileType;
    Boolean mAutoStopProfiler;
    String mOpenGlTraceApp;

    AutoPtr<IRemoteCallbackList> mProcessObservers;
//            = new RemoteCallbackList<IProcessObserver>();
    AutoPtr< ArrayOf<ProcessChangeItem*> > mActiveProcessChanges;

    List< AutoPtr<ProcessChangeItem> > mPendingProcessChanges;
    List< AutoPtr<ProcessChangeItem> > mAvailProcessChanges;

    /**
     * Callback of last caller to {@link #requestPss}.
     */
    AutoPtr<IRunnable> mRequestPssCallback;

    /**
     * Remaining processes for which we are waiting results from the last
     * call to {@link #requestPss}.
     */
    List< AutoPtr<ProcessRecord> > mRequestPssList;

    /**
     * Runtime statistics collection thread.  This object's lock is used to
     * protect all related state.
     */
    AutoPtr<ProcessStatsThread> mProcessStatsThread;

    Object mProcessStatsThreadLock;

    /**
     * Used to collect process stats when showing not responding dialog.
     * Protected by mProcessStatsThread.
     */
    AutoPtr<IProcessStats> mProcessStats;
    AutoPtr<IAtomicInteger64> mLastCpuTime;
    AutoPtr<IAtomicBoolean> mProcessStatsMutexFree;

    Int64 mLastWriteTime;

    /**
     * Set to true after the system has finished booting.
     */
    Boolean mBooted;

    Int32 mProcessLimit;
    Int32 mProcessLimitOverride;

    AutoPtr<CWindowManagerService> mWindowManager;

    static AutoPtr<CActivityManagerService> mSelf;
    static AutoPtr<IActivityThread> mSystemThread;

    static const Int32 SHOW_ERROR_MSG;
    static const Int32 SHOW_NOT_RESPONDING_MSG;
    static const Int32 SHOW_FACTORY_ERROR_MSG;
    static const Int32 UPDATE_CONFIGURATION_MSG;
    static const Int32 GC_BACKGROUND_PROCESSES_MSG;
    static const Int32 WAIT_FOR_DEBUGGER_MSG;
    static const Int32 SERVICE_TIMEOUT_MSG;
    static const Int32 UPDATE_TIME_ZONE;
    static const Int32 SHOW_UID_ERROR_MSG;
    static const Int32 IM_FEELING_LUCKY_MSG;
    static const Int32 PROC_START_TIMEOUT_MSG;
    static const Int32 DO_PENDING_ACTIVITY_LAUNCHES_MSG;
    static const Int32 KILL_APPLICATION_MSG;
    static const Int32 FINALIZE_PENDING_INTENT_MSG;
    static const Int32 POST_HEAVY_NOTIFICATION_MSG;
    static const Int32 CANCEL_HEAVY_NOTIFICATION_MSG;
    static const Int32 SHOW_STRICT_MODE_VIOLATION_MSG;
    static const Int32 CHECK_EXCESSIVE_WAKE_LOCKS_MSG;
    static const Int32 CLEAR_DNS_CACHE;
    static const Int32 UPDATE_HTTP_PROXY;
    static const Int32 SHOW_COMPAT_MODE_DIALOG_MSG;
    static const Int32 DISPATCH_PROCESSES_CHANGED;
    static const Int32 DISPATCH_PROCESS_DIED;
    static const Int32 REPORT_MEM_USAGE;
    static const Int32 REPORT_USER_SWITCH_MSG;
    static const Int32 CONTINUE_USER_SWITCH_MSG;
    static const Int32 USER_SWITCH_TIMEOUT_MSG;

    static const Int32 FIRST_ACTIVITY_STACK_MSG;
    static const Int32 FIRST_BROADCAST_QUEUE_MSG;
    static const Int32 FIRST_COMPAT_MODE_MSG;

    AutoPtr<IAlertDialog> mUidAlert;
    AutoPtr<CompatModeDialog> mCompatModeDialog;
    Int64 mLastMemUsageReportTime;

    static const Int32 LAST_DONE_VERSION;

    // These are in KB.
    static const AutoPtr< ArrayOf<Int64> > DUMP_MEM_BUCKETS;

    static const AutoPtr< ArrayOf<Int32> > DUMP_MEM_OOM_ADJ;
    static const AutoPtr< ArrayOf<String> > DUMP_MEM_OOM_LABEL;

    static Boolean INIT_SUCCEEDED;

    Object mPidsSelfLock;

    AutoPtr<IHandler> mHandler;

    Object mLock;

private:
    static const String USER_DATA_DIR;

    static const String SYSTEM_DEBUGGABLE;

    Boolean mHeadless;

    // Whether we should show our dialogs (ANR, crash, etc) or just perform their
    // default actuion automatically.  Important for devices without direct input
    // devices.
    Boolean mShowDialogs;

    /* add by Gary. start {{----------------------------------- */
    /* 2012-9-8 */
    /* launch the defualt launcher when the system boots for the first time */
    Boolean mFirstLaunch;
    /* add by Gary. end   -----------------------------------}} */

    /**
     * Fingerprints (hashCode()) of stack traces that we've
     * already logged DropBox entries for.  Guarded by itself.  If
     * something (rogue user app) forces this over
     * MAX_DUP_SUPPRESSED_STACKS entries, the contents are cleared.
     */
    HashSet< AutoPtr<IInteger32> > mAlreadyLoggedViolatedStacks;
    Object mAlreadyLoggedViolatedStacksLock;

    static const Int32 MAX_DUP_SUPPRESSED_STACKS;

    /**
     * Strict Mode background batched logging state.
     *
     * The string buffer is guarded by itself, and its lock is also
     * used to determine if another batched write is already
     * in-flight.
     */
    AutoPtr<StringBuilder> mStrictModeBuffer;
    Object mStrictModeBufferLock;

    /**
     * Global set of specific Uri permissions that have been granted.
     */
    typedef HashMap<AutoPtr<IUri>, AutoPtr<UriPermission> > PermissionMap;
    typedef HashMap<Int32, AutoPtr<PermissionMap> > UriPermissionHashMap;
    typedef typename UriPermissionHashMap::ValueType UriPermissionValueType;
    typedef typename UriPermissionHashMap::Iterator UriPermissionIterator;
    UriPermissionHashMap mGrantedUriPermissions;

    static pthread_key_t sCallerIdentity;

    Int32 mCurrentUserId;
    AutoPtr< ArrayOf<Int32> > mCurrentUserArray;
    AutoPtr<CUserManagerService> mUserManager;

    AutoPtr<AThread> mThread;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_AM_CACTIVITYMANAGERSERVICE_H__
