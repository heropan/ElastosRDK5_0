
#ifndef __CACTIVITYTHREAD_H__
#define __CACTIVITYTHREAD_H__

#include "ext/frameworkext.h"
#include "_CActivityThread.h"
#include "content/pm/CApplicationInfo.h"
#include "content/res/CCompatibilityInfo.h"
#include "content/res/CResources.h"
#include "content/pm/CActivityInfo.h"
#include "content/pm/CServiceInfo.h"
#include "content/BroadcastReceiver.h"
#include "app/CApplicationThread.h"
#include "app/CResultInfo.h"
#include "app/Activity.h"
#include "app/LoadedPkg.h"
#include "os/CBundle.h"
#include "os/Runnable.h"
#include "os/HandlerBase.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include <elastos/Map.h>

using Elastos::Utility::Regex::IPattern;
using Elastos::Core::IClassLoader;
using Elastos::IO::IFile;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIContentProvider;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContentProvider;
using Elastos::Droid::Content::IIContentProvider;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IBroadcastReceiverPendingResult;
using Elastos::Droid::Content::Pm::CActivityInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::CServiceInfo;
using Elastos::Droid::Content::Pm::CApplicationInfo;
using Elastos::Droid::Content::Pm::IProviderInfo;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::CCompatibilityInfo;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::App::IContextImpl;
using Elastos::Droid::App::CResultInfo;
using Elastos::Droid::App::Activity;
using Elastos::Droid::App::IOnActivityPausedListener;
using Elastos::Droid::App::IContentProviderHolder;
using Elastos::Droid::App::Backup::IBackupAgent;

namespace Elastos {
namespace Droid {
namespace App {

class ResourcesKey : public ElRefBase
{
public:
    friend class CActivityThread;

    ResourcesKey(
        /* [in] */ const String& resDir,
        /* [in] */ Int32 displayId,
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [in] */ Float scale);

    virtual ~ResourcesKey();

    CARAPI_(Int32) GetHashCode() const;

    CARAPI_(Boolean) Equals(
        /* [in] */ const ResourcesKey* peer) const;

private:
    String mResDir;
    Int32 mDisplayId;
    AutoPtr<IConfiguration> mOverrideConfiguration;
    Float mScale;
    Int32 mHash;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

_ETL_NAMESPACE_BEGIN

#ifndef AUTOPTR_CAT_RESOURCESKEY_EQUALTO_FUNC
#define AUTOPTR_CAT_RESOURCESKEY_EQUALTO_FUNC
template<> struct EqualTo<AutoPtr<Elastos::Droid::App::ResourcesKey> >
{
    Boolean operator()(const AutoPtr<Elastos::Droid::App::ResourcesKey> x,
                       const AutoPtr<Elastos::Droid::App::ResourcesKey> y) const
    {
        assert(y != NULL);
        return x->Equals(y);
    }
};
#endif //AUTOPTR_CAT_RESOURCESKEY_EQUALTO_FUNC

#ifndef AUTOPTR_CAT_RESOURCESKEY_HASH_FUNC
#define AUTOPTR_CAT_RESOURCESKEY_HASH_FUNC
template<> struct Hash<AutoPtr<Elastos::Droid::App::ResourcesKey> >
{
    size_t operator()(const AutoPtr<Elastos::Droid::App::ResourcesKey> s) const
    {
        assert(s != NULL);
        Int32 hashCode = s->GetHashCode();
        return (size_t)hashCode;
    }
};
#endif
_ETL_NAMESPACE_END

namespace Elastos {
namespace Droid {
namespace App {

class LoadedPkg;

CarClass(CActivityThread)
{
public:
    class H
        : public HandlerBase
    {
    public:
        H(
            /* [in] */ CActivityThread* host);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        static String CodeToString(
            /* [in] */ Int32 code);

    private:
        void MaybeSnapshot();
    public:
        static const Int32 LAUNCH_ACTIVITY         ; //=100;
        static const Int32 PAUSE_ACTIVITY          ; //=101;
        static const Int32 PAUSE_ACTIVITY_FINISHING; //=102;
        static const Int32 STOP_ACTIVITY_SHOW      ; //=103;
        static const Int32 STOP_ACTIVITY_HIDE      ; //=104;
        static const Int32 SHOW_WINDOW             ; //=105;
        static const Int32 HIDE_WINDOW             ; //=106;
        static const Int32 RESUME_ACTIVITY         ; //=107;
        static const Int32 SEND_RESULT             ; //=108;
        static const Int32 DESTROY_ACTIVITY        ; //=109;
        static const Int32 BIND_APPLICATION        ; //=110;
        static const Int32 EXIT_APPLICATION        ; //=111;
        static const Int32 NEW_INTENT              ; //=112;
        static const Int32 RECEIVER                ; //=113;
        static const Int32 CREATE_SERVICE          ; //=114;
        static const Int32 SERVICE_ARGS            ; //=115;
        static const Int32 STOP_SERVICE            ; //=116;
        static const Int32 REQUEST_THUMBNAIL       ; //=117;
        static const Int32 CONFIGURATION_CHANGED   ; //=118;
        static const Int32 CLEAN_UP_CONTEXT        ; //=119;
        static const Int32 GC_WHEN_IDLE            ; //=120;
        static const Int32 BIND_SERVICE            ; //=121;
        static const Int32 UNBIND_SERVICE          ; //=122;
        static const Int32 DUMP_SERVICE            ; //=123;
        static const Int32 LOW_MEMORY              ; //=124;
        static const Int32 ACTIVITY_CONFIGURATION_CHANGED ; //=125;
        static const Int32 RELAUNCH_ACTIVITY       ; //=126;
        static const Int32 PROFILER_CONTROL        ; //=127;
        static const Int32 CREATE_BACKUP_AGENT     ; //=128;
        static const Int32 DESTROY_BACKUP_AGENT    ; //=129;
        static const Int32 SUICIDE                 ; //=130;
        static const Int32 REMOVE_PROVIDER         ; //=131;
        static const Int32 ENABLE_JIT              ; //=132;
        static const Int32 DISPATCH_PACKAGE_BROADCAST ; //=133;
        static const Int32 SCHEDULE_CRASH          ; //=134;
        static const Int32 DUMP_HEAP               ; //=135;
        static const Int32 DUMP_ACTIVITY           ; //=136;
        static const Int32 SLEEPING                ; //=137;
        static const Int32 SET_CORE_SETTINGS       ; //=138;
        static const Int32 UPDATE_PACKAGE_COMPATIBILITY_INFO ; //=139;
        static const Int32 TRIM_MEMORY             ; //=140;
        static const Int32 DUMP_PROVIDER           ; //=141;
        static const Int32 UNSTABLE_PROVIDER_DIED  ; //=142;

    private:
        CActivityThread* mHost;
    };

    class ActivityClientRecord
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        ActivityClientRecord();

        virtual ~ActivityClientRecord();

        CARAPI_(Boolean) IsPreHoneycomb();

        CARAPI ToString(
            /* [out] */ String* description);

    public:
        AutoPtr<IBinder> mToken;
        Int32 mIdent;
        AutoPtr<IIntent> mIntent;
        AutoPtr<CBundle> mState;
        AutoPtr<IActivity> mActivity;
        AutoPtr<IWindow> mWindow;
        AutoPtr<IActivity> mParent;
        String mEmbeddedID;
        AutoPtr<IActivityNonConfigurationInstances> mLastNonConfigurationInstances;
        Boolean mPaused;
        Boolean mStopped;
        Boolean mHideForNow;
        AutoPtr<IConfiguration> mNewConfig;
        AutoPtr<IConfiguration> mCreatedConfig;
        AutoPtr<ActivityClientRecord> mNextIdle;

        String mProfileFile;
        AutoPtr<IParcelFileDescriptor> mProfileFd;
        Boolean mAutoStopProfiler;

        AutoPtr<IActivityInfo> mActivityInfo;
        AutoPtr<ICompatibilityInfo> mCompatInfo;
        AutoPtr<LoadedPkg> mPackageInfo;

        AutoPtr< List< AutoPtr<IResultInfo> > > mPendingResults;
        AutoPtr< List< AutoPtr<IIntent> > > mPendingIntents;

        Boolean mStartsNotResumed;
        Boolean mIsForward;
        Int32 mPendingConfigChanges;
        Boolean mOnlyLocalRequest;

        AutoPtr<IView> mPendingRemoveWindow;
        AutoPtr<IWindowManager> mPendingRemoveWindowManager;
    };

    class ProviderClientRecord : public ElRefBase
    {
    public:
        ProviderClientRecord(
            /* [in] */ ArrayOf<String>* names,
            /* [in] */ IIContentProvider* provider,
            /* [in] */ IContentProvider* localProvider,
            /* [in] */ IContentProviderHolder* holder);
    public:
        AutoPtr< ArrayOf<String> > mNames;
        AutoPtr<IIContentProvider> mProvider;
        AutoPtr<IContentProvider> mLocalProvider;
        AutoPtr<IContentProviderHolder> mHolder;
    };

    class NewIntentData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        NewIntentData()
            : mToken(NULL)
        {}
    public:
        List< AutoPtr<IIntent> > mIntents;
        AutoPtr<IBinder> mToken;
    };

    class ReceiverData
        : public ElRefBase
        , public BroadcastReceiver::PendingResult
        , public IBroadcastReceiverPendingResult
    {
    public:
        CAR_INTERFACE_DECL()

        ReceiverData(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ const String& resultData,
            /* [in] */ IBundle* resultExtras,
            /* [in] */ Boolean ordered,
            /* [in] */ Boolean sticky,
            /* [in] */ IBinder* token,
            /* [in] */ Int32 sendingUser);

        /**
         * Version of {@link BroadcastReceiver#setResultCode(int)
         * BroadcastReceiver.setResultCode(int)} for
         * asynchronous broadcast handling.
         */
        CARAPI SetResultCode(
            /* [in] */ Int32 code);

        /**
         * Version of {@link BroadcastReceiver#getResultCode()
         * BroadcastReceiver.getResultCode()} for
         * asynchronous broadcast handling.
         */
        CARAPI GetResultCode(
            /* [out] */ Int32* resultCode);

        /**
         * Version of {@link BroadcastReceiver#setResultData(String)
         * BroadcastReceiver.setResultData(String)} for
         * asynchronous broadcast handling.
         */
        CARAPI SetResultData(
            /* [in] */ const String& data);

        /**
         * Version of {@link BroadcastReceiver#getResultData()
         * BroadcastReceiver.getResultData()} for
         * asynchronous broadcast handling.
         */
        CARAPI GetResultData(
            /* [out] */ String* resultData);

        /**
         * Version of {@link BroadcastReceiver#setResultExtras(Bundle)
         * BroadcastReceiver.setResultExtras(Bundle)} for
         * asynchronous broadcast handling.
         */
        CARAPI SetResultExtras(
            /* [in] */ IBundle* extras);

        /**
         * Version of {@link BroadcastReceiver#getResultExtras(boolean)
         * BroadcastReceiver.getResultExtras(boolean)} for
         * asynchronous broadcast handling.
         */
        CARAPI GetResultExtras(
            /* [in] */ Boolean makeMap,
            /* [out] */ IBundle** resultExtras);

        /**
         * Version of {@link BroadcastReceiver#setResult(int, String, Bundle)
         * BroadcastReceiver.setResult(int, String, Bundle)} for
         * asynchronous broadcast handling.
         */
        CARAPI SetResult(
            /* [in] */ Int32 code,
            /* [in] */ const String& data,
            /* [in] */ IBundle* extras);

        /**
         * Version of {@link BroadcastReceiver#getAbortBroadcast()
         * BroadcastReceiver.getAbortBroadcast()} for
         * asynchronous broadcast handling.
         */
        CARAPI GetAbortBroadcast(
            /* [out] */ Boolean* isAborted);

        /**
         * Version of {@link BroadcastReceiver#abortBroadcast()
         * BroadcastReceiver.abortBroadcast()} for
         * asynchronous broadcast handling.
         */
        CARAPI AbortBroadcast();

        /**
         * Version of {@link BroadcastReceiver#clearAbortBroadcast()
         * BroadcastReceiver.clearAbortBroadcast()} for
         * asynchronous broadcast handling.
         */
        CARAPI ClearAbortBroadcast();

        /**
         * Finish the broadcast.  The current result will be sent and the
         * next broadcast will proceed.
         */
        CARAPI Finish();

        /** @hide */
        CARAPI SetExtrasClassLoader(
            /* [in] */ IClassLoader* cl);

        /** @hide */
        CARAPI SendFinished(
            /* [in] */ IIActivityManager* am);

        /** @hide */
        CARAPI GetSendingUserId(
            /* [out] */ Int32* userId);

        CARAPI GetOrderedHint(
            /* [out] */ Boolean* orderedHint);

        CARAPI GetInitialStickyHint(
            /* [out] */ Boolean* initialStickyHint);

    public:
        AutoPtr<IIntent> mIntent;
        AutoPtr<IActivityInfo> mInfo;
        AutoPtr<ICompatibilityInfo> mCompatInfo;
    };

    class CreateBackupAgentData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        CreateBackupAgentData()
            : mBackupMode(0)
        {}

    public:
        CARAPI ToString(
            /* [out] */ String* description);

    public:
        AutoPtr<IApplicationInfo> mAppInfo;
        AutoPtr<ICompatibilityInfo> mCompatInfo;
        Int32 mBackupMode;
    };

    class CreateServiceData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        AutoPtr<IBinder> mToken;
        AutoPtr<IServiceInfo> mInfo;
        AutoPtr<ICompatibilityInfo> mCompatInfo;
        AutoPtr<IIntent> mIntent;
    };

    class BindServiceData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        BindServiceData()
            : mTaskRemoved(FALSE)
            , mRebind(FALSE)
        {}

        AutoPtr<IBinder> mToken;
        AutoPtr<IIntent> mIntent;
        Boolean mTaskRemoved;
        Boolean mRebind;
    };

    class ServiceArgsData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        ServiceArgsData()
            : mStartId(0)
            , mFlags(0)
            , mTaskRemoved(FALSE)
        {}

        AutoPtr<IBinder> mToken;
        AutoPtr<IIntent> mArgs;
        Int32 mStartId;
        Int32 mFlags;
        Boolean mTaskRemoved;
    };

    class AppBindData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        AppBindData()
            : mDebugMode(0)
            , mEnableOpenGlTrace(FALSE)
            , mRestrictedBackupMode(FALSE)
            , mPersistent(FALSE)
            , mInitAutoStopProfiler(FALSE)
        {}

    public:
        AutoPtr<LoadedPkg> mInfo;
        String mProcessName;
        AutoPtr<IApplicationInfo> mAppInfo;
        AutoPtr<IObjectContainer> mProviders;
        AutoPtr<IComponentName> mInstrumentationName;
        AutoPtr<IBundle> mInstrumentationArgs;
        AutoPtr<IInstrumentationWatcher> mInstrumentationWatcher;
        AutoPtr<IConfiguration> mConfig;
        AutoPtr<ICompatibilityInfo> mCompatInfo;

        /** Initial values for {@link Profiler}. */
        String mInitProfileFile;
        AutoPtr<IParcelFileDescriptor> mInitProfileFd;

        Int32 mDebugMode;
        Boolean mEnableOpenGlTrace;
        Boolean mRestrictedBackupMode;
        Boolean mPersistent;
        Boolean mInitAutoStopProfiler;
    };

    class Profiler : public ElRefBase
    {
    public:
        Profiler()
            : mProfileFile("")
            , mProfileFd(NULL)
            , mAutoStopProfiler(FALSE)
            , mProfiling(FALSE)
            , mHandlingProfiling(FALSE)
        {}

    public:
        CARAPI_(void) SetProfiler(
            /* [in] */ const String& file,
            /* [in] */ IParcelFileDescriptor* fd);

        CARAPI_(void) StartProfiling();

        CARAPI_(void) StopProfiling();

    public:
        String mProfileFile;
        AutoPtr<IParcelFileDescriptor> mProfileFd;
        Boolean mAutoStopProfiler;
        Boolean mProfiling;
        Boolean mHandlingProfiling;
    };

    class DumpComponentInfo
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        AutoPtr<IParcelFileDescriptor> mFd;
        AutoPtr<IBinder> mToken;
        String mPrefix;
        AutoPtr<ArrayOf<String> > mArgs;
    };

    class ResultData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI ToString(
            /* [out] */ String* description);

    public:
        AutoPtr<IBinder> mToken;
        List< AutoPtr<IResultInfo> > mResults;
    };

    class ContextCleanupInfo
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        AutoPtr<IContextImpl> mContext;
        String mWhat;
        String mWho;
    };

    class ProfilerControlData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        String mPath;
        AutoPtr<IParcelFileDescriptor> mFd;
    };

    class DumpHeapData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        String mPath;
        AutoPtr<IParcelFileDescriptor> mFd;
    };

    class UpdateCompatibilityData
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        String mPkg;
        AutoPtr<ICompatibilityInfo> mInfo;
    };

    class ConfigurationChangedCallbacks
        : public ElRefBase
        , public IComponentCallbacks2
    {
    public:
        CAR_INTERFACE_DECL()

        ConfigurationChangedCallbacks(
            /* [in] */ IWeakReference* activityThread);

        CARAPI OnConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        CARAPI OnLowMemory();

        CARAPI OnTrimMemory(
            /* [in] */ Int32 level);
    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

private:
    class ProviderKey : public ElRefBase
    {
    public:
        ProviderKey(
            /* [in] */ const String& authority,
            /* [in] */ Int32 userId);

        //@Override
        CARAPI_(Boolean) Equals(
            /* [in] */ const ProviderKey* o) const;

        // @Override
        CARAPI_(Int32) GetHashCode() const;

    public:
        String mAuthority;
        Int32 mUserId;
    };

    class Idler
        : public ElRefBase
        , public IIdleHandler
    {
    public:
        CAR_INTERFACE_DECL()

        Idler(
            /* [in] */ CActivityThread* owner);

        CARAPI QueueIdle(
            /* [out] */ Boolean* res);

    private:
        CActivityThread* mOwner;
    };

    class StopInfo : public Runnable
    {
    public:
        CARAPI Run();

    public:
        AutoPtr<ActivityClientRecord> mActivity;
        AutoPtr<CBundle> mState;
        AutoPtr<IBitmap> mThumbnail;
        AutoPtr<ICharSequence> mDescription;
    };

    class ProviderRefCount
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        ProviderRefCount(
            /* [in] */ IContentProviderHolder* inHolder,
            /* [in] */ ProviderClientRecord* inClient,
            /* [in] */ Int32 sCount,
            /* [in] */ Int32 uCount)
            : mHolder(inHolder)
            , mClient(inClient)
            , mStableCount(sCount)
            , mUnstableCount(uCount)
            , mRemovePending(FALSE)
        {}

    public:
        AutoPtr<IContentProviderHolder> mHolder;
        AutoPtr<ProviderClientRecord> mClient;
        Int32 mStableCount;
        Int32 mUnstableCount;

        // When this is set, the stable and unstable ref counts are 0 and
        // we have a pending operation scheduled to remove the ref count
        // from the activity manager.  On the activity manager we are still
        // holding an unstable ref, though it is not reflected in the counts
        // here.
        Boolean mRemovePending;
    };

public:
    CActivityThread();

    virtual ~CActivityThread();

    CARAPI constructor();

    static CARAPI_(AutoPtr<IActivityThread>) GetCurrentActivityThread();

    static CARAPI_(String) GetCurrentPackageName();

    static CARAPI_(AutoPtr<IApplication>) GetCurrentApplication();

    static CARAPI_(AutoPtr<IIPackageManager>) GetPackageManager();

    CARAPI GetDisplayMetricsLocked(
        /* [in] */ Int32 displayId,
        /* [in] */ ICompatibilityInfo* ci,
        /* [out] */ IDisplayMetrics** dms);

    CARAPI_(AutoPtr<IConfiguration>) ApplyConfigCompatMainThread(
        /* [in] */ Int32 displayDensity,
        /* [in] */ IConfiguration* config,
        /* [in] */ ICompatibilityInfo* compat);

    /**
     * Creates the top level Resources for applications with the given compatibility info.
     *
     * @param resDir the resource directory.
     * @param compInfo the compability info. It will use the default compatibility info when it's
     * null.
     */
    CARAPI GetTopLevelResources(
        /* [in] */ const String& resDir,
        /* [in] */ Int32 displayId,
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [in] */ CCompatibilityInfo* compInfo,
        /* [out] */ IResources** res);

    /**
     * Creates the top level resources for the given package.
     */
    CARAPI GetTopLevelResources(
        /* [in] */ const String& resDir,
        /* [in] */ Int32 displayId,
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [in] */ LoadedPkg* pkgInfo,
        /* [out] */ IResources** res);

    CARAPI GetHandler(
        /* [out] */ IHandler** h);

    CARAPI_(AutoPtr<IHandler>) GetHandler();

    CARAPI_(AutoPtr<LoadedPkg>) GetPackageInfo(
        /* [in] */ const String& packageName,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ Int32 flags);

    CARAPI_(AutoPtr<LoadedPkg>) GetPackageInfo(
        /* [in] */ const String& packageName,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<LoadedPkg>) GetPackageInfo(
        /* [in] */ IApplicationInfo* ai,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ Int32 flags);

    CARAPI_(AutoPtr<LoadedPkg>) GetPackageInfoNoCheck(
        /* [in] */ IApplicationInfo* ai,
        /* [in] */ ICompatibilityInfo* compatInfo);

    CARAPI_(AutoPtr<LoadedPkg>) PeekPackageInfo(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean includeCode);

    CARAPI GetApplicationThread(
        /* [out] */ IApplicationThread** thread);

    CARAPI GetInstrumentation(
        /* [out] */ IInstrumentation** instrumentation);

    CARAPI  GetConfiguration(
        /* [out] */ IConfiguration** configuration);

    CARAPI IsProfiling(
        /* [out] */ Boolean* profiled);

    CARAPI GetProfileFilePath(
        /* [out] */ String* path);

    CARAPI GetLooper(
        /* [out] */ ILooper** looper);

    CARAPI GetApplication(
        /* [out] */ IApplication** application);

    CARAPI GetProcessName(
        /* [out] */ String* name);

    CARAPI GetSystemContext(
        /* [out] */ IContextImpl** ctx);

    CARAPI InstallSystemApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI EnsureJitEnabled();

    CARAPI ScheduleGcIdler();

    CARAPI UnscheduleGcIdler();

    CARAPI_(void) DoGcIfNeeded();

    CARAPI RegisterOnActivityPausedListener(
        /* [in] */ IActivity* activity,
        /* [in] */ IOnActivityPausedListener* listener);

    CARAPI UnregisterOnActivityPausedListener(
        /* [in] */ IActivity* activity,
        /* [in] */ IOnActivityPausedListener* listener);

    CARAPI ResolveActivityInfo(
        /* [in] */ IIntent* intent,
        /* [out] */ IActivityInfo** info);

    CARAPI StartActivityNow(
        /* [in] */ IActivity* parent,
        /* [in] */ const String& id,
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* activityInfo,
        /* [in] */ IBinder* token,
        /* [in] */ IBundle* state,
        /* [in] */ IActivityNonConfigurationInstances* lastNonConfigurationInstances,
        /* [out] */ IActivity** activity);

    CARAPI GetActivity(
        /* [in] */ IBinder* token,
        /* [out] */ IActivity** activity);

    CARAPI SendActivityResult(
        /* [in] */ IBinder* token,
        /* [in] */ const String& id,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    CARAPI ScheduleContextCleanup(
        /* [in] */ IContext* context,
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI PerformNewIntents(
        /* [in] */ IBinder* token,
        /* [in] */ ArrayOf<IIntent*>* intents);

    CARAPI_(AutoPtr<ActivityClientRecord>) PerformResumeActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean clearHide);

    static CARAPI_(void) CleanUpPendingRemoveWindows(
        /* [in] */ ActivityClientRecord* r);

    CARAPI HandleResumeActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean clearHide,
        /* [in] */ Boolean isForward,
        /* [in] */ Boolean reallyResume);

    CARAPI PerformUserLeavingActivity(
        /* [in] */ ActivityClientRecord* r);

    CARAPI PerformPauseActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean saveState,
        /* [out] */ IBundle** state);

    CARAPI PerformPauseActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean saveState,
        /* [out] */ IBundle** state);

    CARAPI PerformStopActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean saveState);

    CARAPI_(void) PerformRestartActivity(
        /* [in] */ IBinder* token);

    CARAPI_(AutoPtr<ActivityClientRecord>) PerformDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing);

    CARAPI RequestRelaunchActivity(
        /* [in] */ IBinder* token,
        /* [in] */ ArrayOf<IResultInfo*>* pendingResults,
        /* [in] */ ArrayOf<IIntent*>* pendingNewIntents,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean notResumed,
        /* [in] */ IConfiguration* config,
        /* [in] */ Boolean fromServer);

    CARAPI ApplyConfigurationToResources(
        /* [in] */ IConfiguration* config);

    CARAPI_(Boolean) ApplyConfigurationToResourcesLocked(
        /* [in] */ IConfiguration* config,
        /* [in] */ ICompatibilityInfo* compat);

    CARAPI_(void) ApplyNonDefaultDisplayMetricsToConfigurationLocked(
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ IConfiguration* config);

    CARAPI_(AutoPtr<IConfiguration>) ApplyCompatConfiguration(
        /* [in] */ Int32 displayDensity);

    CARAPI HandleConfigurationChanged(
        /* [in] */ IConfiguration* config,
        /* [in] */ ICompatibilityInfo* compat);

    CARAPI_(void) FreeTextLayoutCachesIfNeeded(
        /* [in] */ Int32 configDiff);

    CARAPI HandleActivityConfigurationChanged(
        /* [in] */ IBinder* token);

    CARAPI HandleProfilerControl(
        /* [in] */ Boolean start,
        /* [in] */ ProfilerControlData* pcd,
        /* [in] */ Int32 profileType);

    static CARAPI HandleDumpHeap(
        /* [in] */ Boolean managed,
        /* [in] */ DumpHeapData* dhd);

    CARAPI HandleDispatchPackageBroadcast(
        /* [in] */ Int32 cmd,
        /* [in] */ ArrayOf<String>* packages);

    CARAPI HandleLowMemory();

    CARAPI HandleTrimMemory(
        /* [in] */ Int32 level);

    /*package*/
    CARAPI FinishInstrumentation(
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI AcquireProvider(
        /* [in] */ IContext* c,
        /* [in] */ const String& auth,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean stable,
        /* [out] */ IIContentProvider** pr);

    CARAPI AcquireExistingProvider(
        /* [in] */ IContext* ctx,
        /* [in] */ const String& auth,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean stable,
        /* [out] */ IIContentProvider** cpr);

    CARAPI ReleaseProvider(
        /* [in] */ IIContentProvider* provider,
        /* [in] */ Boolean stable,
        /* [out] */ Boolean* released);

    CARAPI CompleteRemoveProvider(
        /* [in] */ ProviderRefCount* prc);

    CARAPI HandleUnstableProviderDied(
        /* [in] */ IBinder* provider,
        /* [in] */ Boolean fromClient);

    CARAPI HandleUnstableProviderDiedLocked(
        /* [in] */ IBinder* provider,
        /* [in] */ Boolean fromClient);

    static CARAPI_(AutoPtr<IActivityThread>) GetSystemMain();

    CARAPI InstallSystemProviders(
        /* [in] */ ArrayOf<IProviderInfo*>* providers);

    CARAPI GetIntCoreSetting(
        /* [in] */ const String& key,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* setting);

    static CARAPI Main(
        /* [in] */ const ArrayOf<String>& args);

private:
    CARAPI_(void) FlushDisplayMetricsLocked();

    CARAPI_(AutoPtr<LoadedPkg>) GetPackageInfo(
        /* [in] */ IApplicationInfo* aInfo,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ IClassLoader* baseLoader,
        /* [in] */ Boolean securityViolation,
        /* [in] */ Boolean includeCode);

    // if the thread hasn't started yet, we don't have the handler, so just
    // save the messages until we're ready.
    CARAPI QueueOrSendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI QueueOrSendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 arg1);

    CARAPI QueueOrSendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    CARAPI PerformLaunchActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ IIntent* customIntent,
        /* [out] */ IActivity** activity);

    CARAPI_(AutoPtr<IContext>) CreateBaseContextForActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ IActivity* activity);

    CARAPI HandleLaunchActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ IIntent* customIntent);

    CARAPI_(void) DeliverNewIntents(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ List< AutoPtr<IIntent> >* intents);

    CARAPI HandleNewIntent(
        /* [in] */ NewIntentData* data);

    CARAPI HandleReceiver(
        /* [in] */ ReceiverData* data);

    // Instantiate a BackupAgent and tell it that it's alive
    CARAPI HandleCreateBackupAgent(
        /* [in] */ CreateBackupAgentData* data);

    // Tear down a BackupAgent
    CARAPI HandleDestroyBackupAgent(
        /* [in] */ CreateBackupAgentData* data);

    CARAPI HandleCreateService(
        /* [in] */ CreateServiceData* data);

    CARAPI HandleBindService(
        /* [in] */ BindServiceData* data);

    CARAPI HandleUnbindService(
        /* [in] */ BindServiceData* data);

    CARAPI HandleDumpService(
        /* [in] */ DumpComponentInfo* info);

    CARAPI HandleDumpActivity(
        /* [in] */ DumpComponentInfo* info);

    CARAPI HandleDumpProvider(
        /* [in] */ DumpComponentInfo* info);

    CARAPI HandleServiceArgs(
        /* [in] */ ServiceArgsData* data);

    CARAPI HandleStopService(
        /* [in] */ IBinder* token);

    CARAPI_(AutoPtr<IBitmap>) CreateThumbnailBitmap(
        /* [in] */ ActivityClientRecord* r);

    CARAPI HandlePauseActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean userLeaving,
        /* [in] */ Int32 configChanges);

    /**
     * Core implementation of stopping an activity.  Note this is a little
     * tricky because the server's meaning of stop is slightly different
     * than our client -- for the server, stop means to save state and give
     * it the result when it is done, but the window may still be visible.
     * For the client, we want to call onStop()/onStart() to indicate when
     * the activity's UI visibillity changes.
     */
    CARAPI PerformStopActivityInner(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ StopInfo* info,
        /* [in] */ Boolean keepShown,
        /* [in] */ Boolean saveState);

    CARAPI_(void) UpdateVisibility(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ Boolean show);

    CARAPI HandleStopActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean show,
        /* [in] */ Int32 configChanges);

    CARAPI HandleWindowVisibility(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean show);

    CARAPI HandleSleeping(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean sleeping);

    CARAPI HandleSetCoreSettings(
        /* [in] */ IBundle* coreSettings);

    CARAPI HandleUpdatePackageCompatibilityInfo(
        /* [in] */ UpdateCompatibilityData* data);

    CARAPI DeliverResults(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ List< AutoPtr<IResultInfo> >* results);

    CARAPI HandleSendResult(
        /* [in] */ ResultData* res);

    CARAPI_(AutoPtr<ActivityClientRecord>) PerformDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean getNonConfigInstance);

    static CARAPI_(String) SafeToComponentShortString(
        /* [in] */ IIntent* intent);

    CARAPI HandleDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean getNonConfigInstance);

    CARAPI HandleRelaunchActivity(
        /* [in] */ ActivityClientRecord* tmp);

    CARAPI HandleRequestThumbnail(
        /* [in] */ IBinder* token);

    CARAPI_(AutoPtr< List<AutoPtr<IComponentCallbacks2> > >) CollectComponentCallbacks(
        /* [in] */ Boolean allActivities,
        /* [in] */ IConfiguration* newConfig);

    static CARAPI PerformConfigurationChanged(
        /* [in] */ IComponentCallbacks2* cb,
        /* [in] */ IConfiguration* config);

    CARAPI_(void) SetupGraphicsSupport(
        /* [in] */ LoadedPkg* info,
        /* [in] */ IFile* cacheDir);

    CARAPI_(void) UpdateDefaultDensity();

    CARAPI HandleBindApplication(
        /* [in] */ AppBindData* data);

    CARAPI InstallContentProviders(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* providers);

    CARAPI_(void) IncProviderRefLocked(
        /* [in] */ ProviderRefCount* prc,
        /* [in] */ Boolean stable);

    CARAPI_(AutoPtr<ProviderClientRecord>) InstallProviderAuthoritiesLocked(
        /* [in] */ IIContentProvider* provider,
        /* [in] */ IContentProvider* localProvider,
        /* [in] */ IContentProviderHolder* holder);

    CARAPI_(AutoPtr<IContentProviderHolder>) InstallProvider(
        /* [in] */ IContext* context,
        /* [in] */ IContentProviderHolder* holder,
        /* [in] */ IProviderInfo* info,
        /* [in] */ Boolean noisy,
        /* [in] */ Boolean noReleaseNeeded,
        /* [in] */ Boolean stable);

    CARAPI Attach(
        /* [in] */ Boolean sys);

    CARAPI_(AutoPtr<ActivityClientRecord>) GetActivityClientRecord(
        /* [in] */ IBinder* token);

    static CARAPI_(void) InitTLS();

public:
    struct HashPK
    {
        size_t operator()(const ProviderKey* s) const
        {
            return (size_t)s->GetHashCode();
        }
    };

    struct PKEq
    {
        Boolean operator()(const ProviderKey* x,
                           const ProviderKey* y) const
        {
            return x->Equals(y);
        }
    };

    /** @hide */
    static const String TAG;
    static const Boolean localLOGV;
    static const Boolean DEBUG_MESSAGES;
    /** @hide */
    static const Boolean DEBUG_BROADCAST;

    static AutoPtr<IContextImpl> sSystemContext;

    static AutoPtr<IIPackageManager> sPackageManager;

    AutoPtr<CApplicationThread> mAppThread;
    AutoPtr<ILooper> mLooper;
    AutoPtr<IHandler> mH;
    HashMap<AutoPtr<IBinder>, AutoPtr<ActivityClientRecord> > mActivities;

    // List of new activities (via ActivityRecord.nextIdle) that should
    // be reported when next we idle.
    AutoPtr<ActivityClientRecord> mNewActivities;
    // Number of activities that are currently visible on-screen.
    Int32 mNumVisibleActivities;
    HashMap<AutoPtr<IBinder>, AutoPtr<IService> > mServices;

    AutoPtr<AppBindData> mBoundApplication;
    AutoPtr<Profiler> mProfiler;
    Int32 mCurDefaultDisplayDpi;
    Boolean mDensityCompatMode;
    AutoPtr<IConfiguration> mConfiguration;
    AutoPtr<IConfiguration> mCompatConfiguration;
    AutoPtr<IConfiguration> mResConfiguration;
    AutoPtr<ICompatibilityInfo> mResCompatibilityInfo;
    AutoPtr<IApplication> mInitialApplication;
    List< AutoPtr<IApplication> > mAllApplications;

    // set of instantiated backup agents, keyed by package name
    HashMap<String, AutoPtr<IBackupAgent> > mBackupAgents;
    static pthread_key_t sKey;
    static Boolean sHaveKey;
    AutoPtr<IInstrumentation> mInstrumentation;
    String mInstrumentationAppDir;
    String mInstrumentationAppLibraryDir;
    String mInstrumentationAppPackage;
    String mInstrumentedAppDir;
    String mInstrumentedAppLibraryDir;
    Boolean mSystemThread;
    Boolean mJitEnabled;

    // These can be accessed by multiple threads; mPackages is the lock.
    // XXX For now we keep around information about all packages we have
    // seen, not removing entries from this map.
    // NOTE: The activity and window managers need to call in to
    // ActivityThread to do things like update resource configurations,
    // which means this lock gets held while the activity and window managers
    // holds their own lock.  Thus you MUST NEVER call back into the activity manager
    // or window manager or anything that depends on them while holding this lock.

    //HashMap<String, WeakReference<LoadedApk>>
    typedef HashMap<String, AutoPtr<IWeakReference> > LoadedApkMap;
    typedef typename LoadedApkMap::Iterator LoadedApkMapIterator;

    LoadedApkMap mPackages;
    LoadedApkMap mResourcePackages;

    HashMap<AutoPtr<ICompatibilityInfo>, AutoPtr<IDisplayMetrics> > mDefaultDisplayMetrics;

    //HashMap<ResourcesKey, WeakReference<Resources> >
    typedef HashMap<AutoPtr<ResourcesKey>, AutoPtr<IWeakReference> > ActiveResourcesMap;
    typedef typename ActiveResourcesMap::Iterator ActiveResourcesMapIterator;
    ActiveResourcesMap mActiveResources;

    List< AutoPtr<ActivityClientRecord> > mRelaunchingActivities;
    AutoPtr<IConfiguration> mPendingConfiguration;

    // The lock of mProviderMap protects the following variables.
    HashMap< AutoPtr<ProviderKey>, AutoPtr<ProviderClientRecord>, HashPK, PKEq > mProviderMap;
    HashMap< AutoPtr<IBinder>, AutoPtr<ProviderRefCount> > mProviderRefCountMap;
    HashMap<AutoPtr<IBinder>, AutoPtr<ProviderClientRecord> > mLocalProviders;
    HashMap<AutoPtr<IComponentName>, AutoPtr<ProviderClientRecord> > mLocalProvidersByName;

    typedef List< AutoPtr<IOnActivityPausedListener> > OnPauseListenerList;
    typedef typename OnPauseListenerList::Iterator OnPauseListenerListIterator;
    typedef HashMap<AutoPtr<IActivity>, AutoPtr<OnPauseListenerList> > OnPauseListenerMap;
    typedef typename OnPauseListenerMap::Iterator OnPauseListenerMapIterator;
    OnPauseListenerMap mOnPauseListeners;

//    GcIdler* mGcIdler;
    Boolean mGcIdlerScheduled;

    static AutoPtr<IHandler> sMainThreadHandler;  // set once in main()

    AutoPtr<IBundle> mCoreSettings;

private:
    friend class LoadedPkg;
    friend class CApplicationThread;
    friend class CActivityThreadHelper;
    friend class ConfigurationChangedCallbacks;

    static const Int32 THUMBNAIL_FORMAT = Elastos::Droid::Graphics::BitmapConfig_RGB_565;

    static const Boolean DEBUG_RESULTS;
    static const Boolean DEBUG_BACKUP;
    static const Boolean DEBUG_CONFIGURATION;
    static const Boolean DEBUG_SERVICE;
    static const Boolean DEBUG_MEMORY_TRIM;
    static const Boolean DEBUG_PROVIDER;

    static const Int64 MIN_TIME_BETWEEN_GCS = 5*1000;
    static AutoPtr<IPattern> PATTERN_SEMICOLON;//Pattern.compile(";");
    static const Int32 SQLITE_MEM_RELEASED_EVENT_LOG_TAG = 75003;
    static const Int32 LOG_ON_PAUSE_CALLED = 30021;
    static const Int32 LOG_ON_RESUME_CALLED = 30022;

    AutoPtr<IConfiguration> mMainThreadConfig;

    static pthread_key_t sCurrentBroadcastIntentKey;
//    static ThreadLocal<Intent> sCurrentBroadcastIntent = new ThreadLocal<Intent>();

    Int32 mThumbnailWidth;
    Int32 mThumbnailHeight;
    AutoPtr<IBitmap> mAvailThumbnailBitmap;
    AutoPtr<ICanvas> mThumbnailCanvas;

    Object mOnPauseListenersLock;
    Object mProviderMapLock;
    Object mPackagesLock;
    Object mLock;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __CACTIVITYTHREAD_H__
