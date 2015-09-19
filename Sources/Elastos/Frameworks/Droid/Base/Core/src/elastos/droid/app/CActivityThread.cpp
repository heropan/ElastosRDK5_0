
//#include <unistd.h>
//#include <new>
#include "ext/frameworkext.h"
#include "app/CActivityThread.h"
#include "app/LoadedPkg.h"
#include "app/CContextImpl.h"
#include "app/CInstrumentation.h"
#include "app/CActivityManager.h"
#include "app/ActivityManagerNative.h"
#include "app/CContentProviderHolder.h"
#include "app/ApplicationPackageManager.h"
#include "app/QueuedWork.h"
#include "content/CComponentName.h"
#include "content/pm/CInstrumentationInfo.h"
#include "content/res/CConfigurationHelper.h"
#include "graphics/Canvas.h"
#include "graphics/CCanvas.h"
#include "graphics/CBitmap.h"
#include "hardware/display/DisplayManagerGlobal.h"
#include "os/UserHandle.h"
#include "os/ServiceManager.h"
#include "os/SystemClock.h"
#include "os/Looper.h"
#include "os/Build.h"
#include "os/Process.h"
#include "os/AsyncTask.h"
#include "os/CSystemProperties.h"
#include "view/ViewRootImpl.h"
#include "view/CCompatibilityInfoHolder.h"
#include "view/CWindowManagerGlobal.h"
#include "utility/CParcelableObjectContainer.h"
#include "database/sqlite/CSQLiteDatabaseHelper.h"
#include <elastos/core/Thread.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include "R.h"
#include <unistd.h>

using Elastos::Core::ISystem;
using Elastos::Core::IClassLoader;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::Thread;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Elastos::Droid::R;
using Elastos::Droid::App::Backup::EIID_IBackupAgent;
using Elastos::Droid::App::CInstrumentation;
using Elastos::Droid::App::LoadedPkg;
using Elastos::Droid::App::CContextImpl;
using Elastos::Droid::App::CContentProviderHolder;
using Elastos::Droid::App::QueuedWork;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::CCompatibilityInfoHolder;
using Elastos::Droid::View::CWindowManagerGlobal;
using Elastos::Droid::View::IWindowManagerGlobal;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::Canvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::EIID_IPendingResult;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IInstrumentationInfo;
using Elastos::Droid::Content::Pm::CInstrumentationInfo;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::CAssetManager;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::IConfigurationHelper;
using Elastos::Droid::Content::Res::CConfigurationHelper;
using Elastos::Droid::Content::Res::EIID_IResources;
using Elastos::Droid::Content::EIID_IComponentCallbacks;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::Content::IPendingResult;
using Elastos::Droid::Hardware::Display::DisplayManagerGlobal;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabaseHelper;
using Elastos::Droid::Database::Sqlite::CSQLiteDatabaseHelper;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::AsyncTask;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Utility::CParcelableObjectContainer;

namespace Elastos {
namespace Droid {
namespace App {

#define UNUSED(x) (void)x

const String CActivityThread::TAG("CActivityThread");
const Boolean CActivityThread::localLOGV = FALSE;
const Boolean CActivityThread::DEBUG_MESSAGES = FALSE;
const Boolean CActivityThread::DEBUG_BROADCAST = FALSE;
const Boolean CActivityThread::DEBUG_RESULTS = FALSE;
const Boolean CActivityThread::DEBUG_BACKUP = FALSE;
const Boolean CActivityThread::DEBUG_CONFIGURATION = FALSE;
const Boolean CActivityThread::DEBUG_SERVICE = FALSE;
const Boolean CActivityThread::DEBUG_MEMORY_TRIM = FALSE;
const Boolean CActivityThread::DEBUG_PROVIDER = FALSE;

const Int64 CActivityThread::MIN_TIME_BETWEEN_GCS;
const Int32 CActivityThread::SQLITE_MEM_RELEASED_EVENT_LOG_TAG;
const Int32 CActivityThread::LOG_ON_PAUSE_CALLED;
const Int32 CActivityThread::LOG_ON_RESUME_CALLED;

static AutoPtr<IPattern> InitPATTERN_SEMICOLON()
{
    AutoPtr<IPattern> pattern;
    AutoPtr<IPatternHelper> helper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
    helper->Compile(String(";"), (IPattern**)&pattern);
    return pattern;
}

AutoPtr<IContextImpl> CActivityThread::sSystemContext;
AutoPtr<IIPackageManager> CActivityThread::sPackageManager;
pthread_key_t CActivityThread::sKey;
pthread_key_t CActivityThread::sCurrentBroadcastIntentKey;
Boolean CActivityThread::sHaveKey = FALSE;
AutoPtr<IHandler> CActivityThread::sMainThreadHandler;
AutoPtr<IPattern> CActivityThread::PATTERN_SEMICOLON = InitPATTERN_SEMICOLON();

//==============================================================================
//  CAR_INTERFACE_IMPL
//==============================================================================
CAR_INTERFACE_IMPL(CActivityThread::ActivityClientRecord, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::AppBindData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::NewIntentData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::CreateServiceData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::BindServiceData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::ServiceArgsData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::ContextCleanupInfo, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::ProfilerControlData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::CreateBackupAgentData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::ProviderRefCount, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::DumpHeapData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::DumpComponentInfo, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::UpdateCompatibilityData, IInterface)
CAR_INTERFACE_IMPL(CActivityThread::ResultData, IInterface)


//==============================================================================
// ResourcesKey
//==============================================================================
ResourcesKey::ResourcesKey(
    /* [in] */ const String& resDir,
    /* [in] */ Int32 displayId,
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [in] */ Float scale)
{
    mResDir = resDir;
    mDisplayId = displayId;
    if (overrideConfiguration != NULL) {
        AutoPtr<IConfigurationHelper> confHelper;
        CConfigurationHelper::AcquireSingleton((IConfigurationHelper**)&confHelper);
        assert(confHelper != NULL);

        AutoPtr<IConfiguration> empty;
        confHelper->GetEmpty((IConfiguration**)&empty);
        assert(empty != NULL);

        Boolean e = FALSE;
        if (empty->Equals(overrideConfiguration, &e), e) {
           overrideConfiguration = NULL;
        }
    }

    mOverrideConfiguration = overrideConfiguration;
    mScale = scale;
    Int32 hash = 17;
    hash = 31 * hash + mResDir.GetHashCode();
    hash = 31 * hash + mDisplayId;
//            hash = 31 * hash + (mOverrideConfiguration != NULL
//                    ? (Int32)(mOverrideConfiguration->GetHashCode()) : 0);
//            hash = 31 * hash + Float.floatToIntBits(mScale);
    mHash = hash;
}

ResourcesKey::~ResourcesKey()
{}

Int32 ResourcesKey::GetHashCode() const
{
    return mHash;
}

Boolean ResourcesKey::Equals(
    /* [in] */ const ResourcesKey* peer) const
{
    if (!mResDir.Equals(peer->mResDir)) {
        return FALSE;
    }
    if (mDisplayId != peer->mDisplayId) {
        return FALSE;
    }
    if (mOverrideConfiguration != peer->mOverrideConfiguration) {
        if (mOverrideConfiguration == NULL || peer->mOverrideConfiguration == NULL) {
            return FALSE;
        }
        Boolean res;
        mOverrideConfiguration->Equals(peer->mOverrideConfiguration, &res);
        if (!res) {
            return FALSE;
        }
    }
    if (mScale != peer->mScale) {
        return FALSE;
    }
    return TRUE;
}

//==============================================================================
// CActivityThread::ProviderKey
//==============================================================================
CActivityThread::ProviderKey::ProviderKey(
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId)
    : mAuthority(authority)
    , mUserId(userId)
{}

Boolean CActivityThread::ProviderKey::Equals(
    /* [in] */ const ProviderKey* o) const
{
    if (o == NULL) {
        return FALSE;
    }

    if (o == this) {
        return TRUE;
    }

    if (o->mUserId == mUserId && o->mAuthority == mAuthority) {
        return TRUE;
    }

    return FALSE;
}

Int32 CActivityThread::ProviderKey::GetHashCode() const
{
    return (!mAuthority.IsNull() ? mAuthority.GetHashCode() : 0) ^ mUserId;
}

//==============================================================================
// CActivityThread::Idler
//==============================================================================
CAR_INTERFACE_IMPL(CActivityThread::Idler, IIdleHandler);

CActivityThread::Idler::Idler(
    /* [in] */ CActivityThread* owner)
    : mOwner(owner)
{
}

ECode CActivityThread::Idler::QueueIdle(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<ActivityClientRecord> a = mOwner->mNewActivities;
    Boolean stopProfiling = FALSE;
    if (mOwner->mBoundApplication != NULL && mOwner->mProfiler->mProfileFd != NULL
        && mOwner->mProfiler->mAutoStopProfiler) {
        stopProfiling = TRUE;
    }

    if (a != NULL) {
        mOwner->mNewActivities = NULL;
        AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
        AutoPtr<ActivityClientRecord> prev;
        do {
            Boolean finished = FALSE;
            if (a->mActivity != NULL)
                a->mActivity->IsFinishing(&finished);

            if (localLOGV) {
                String str;
                a->ToString(&str);
                Slogger::V(TAG, "Reporting idle of %s finished=", str.string(), finished);
            }

            if (finished) {
                am->ActivityIdle(a->mToken, a->mCreatedConfig, stopProfiling);
                a->mCreatedConfig = NULL;
            }
            prev = a;
            a = a->mNextIdle;
            prev->mNextIdle = NULL;
        } while (a != NULL);
    }

    if (stopProfiling) {
        mOwner->mProfiler->StopProfiling();
    }
    mOwner->EnsureJitEnabled();
    *res = FALSE;
    return NOERROR;
}

//==============================================================================
// CActivityThread::ActivityClientRecord
//==============================================================================

CActivityThread::ActivityClientRecord::ActivityClientRecord()
    : mIdent(0)
    , mPaused(FALSE)
    , mStopped(FALSE)
    , mHideForNow(FALSE)
    , mAutoStopProfiler(FALSE)
    , mStartsNotResumed(FALSE)
    , mIsForward(FALSE)
    , mPendingConfigChanges(0)
    , mOnlyLocalRequest(FALSE)
{
}

CActivityThread::ActivityClientRecord::~ActivityClientRecord()
{
    mPendingResults = NULL;
    mPendingIntents = NULL;
    mActivity = NULL;
    mParent = NULL;
}

Boolean CActivityThread::ActivityClientRecord::IsPreHoneycomb()
{
    if (mActivity != NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        mActivity->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 version;
        appInfo->GetTargetSdkVersion(&version);
        return version < Build::VERSION_CODES::HONEYCOMB;
    }
    return FALSE;
}

ECode CActivityThread::ActivityClientRecord::ToString(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    *description = String(NULL);

    String tkDes, nmDes;
    AutoPtr<IComponentName> componentName;
    if (mIntent != NULL)
        mIntent->GetComponent((IComponentName**)&componentName);

//    mToken->ToString(&tkDes);
    if (componentName != NULL)
        componentName->ToShortString(&nmDes);
    else
        nmDes = "no component name";

    String str;
    str.AppendFormat("ActivityRecord{%p token=%p %s}", this, mToken.Get(), nmDes.string());
    *description = str;
    return NOERROR;
}

//==============================================================================
// CActivityThread::ProviderClientRecord
//==============================================================================

CActivityThread::ProviderClientRecord::ProviderClientRecord(
    /* [in] */ ArrayOf<String>* names,
    /* [in] */ IIContentProvider* provider,
    /* [in] */ IContentProvider* localProvider,
    /* [in] */ IContentProviderHolder* holder)
    : mNames(names)
    , mProvider(provider)
    , mLocalProvider(localProvider)
    , mHolder(holder)
{}

//==============================================================================
// CActivityThread::ReceiverData
//==============================================================================
CAR_INTERFACE_IMPL(CActivityThread::ReceiverData, IPendingResult)

CActivityThread::ReceiverData::ReceiverData(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 sendingUser)
    : BroadcastReceiver::PendingResult(
        resultCode, resultData, resultExtras,
        CActivityThread::ReceiverData::TYPE_COMPONENT,
        ordered, sticky, token, sendingUser)
    , mIntent(intent)
{}

ECode CActivityThread::ReceiverData::SetResultCode(
    /* [in] */ Int32 code)
{
    return PendingResult::SetResultCode(code);
}

ECode CActivityThread::ReceiverData::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    return PendingResult::GetResultCode(resultCode);
}

ECode CActivityThread::ReceiverData::SetResultData(
    /* [in] */ const String& data)
{
    return PendingResult::SetResultData(data);
}

ECode CActivityThread::ReceiverData::GetResultData(
    /* [out] */ String* resultData)
{
    return PendingResult::GetResultData(resultData);
}

ECode CActivityThread::ReceiverData::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    return PendingResult::SetResultExtras(extras);
}

ECode CActivityThread::ReceiverData::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** resultExtras)
{
    return PendingResult::GetResultExtras(makeMap, resultExtras);
}

ECode CActivityThread::ReceiverData::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    return PendingResult::SetResult(code, data, extras);
}

ECode CActivityThread::ReceiverData::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    return PendingResult::GetAbortBroadcast(isAborted);
}

ECode CActivityThread::ReceiverData::AbortBroadcast()
{
    return PendingResult::AbortBroadcast();
}

ECode CActivityThread::ReceiverData::ClearAbortBroadcast()
{
    return PendingResult::ClearAbortBroadcast();
}

ECode CActivityThread::ReceiverData::Finish()
{
    return PendingResult::Finish();
}

ECode CActivityThread::ReceiverData::SetExtrasClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return PendingResult::SetExtrasClassLoader(cl);
}

ECode CActivityThread::ReceiverData::SendFinished(
    /* [in] */ IIActivityManager* am)
{
    return PendingResult::SendFinished(am);
}

ECode CActivityThread::ReceiverData::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return PendingResult::GetSendingUserId(userId);
}

ECode CActivityThread::ReceiverData::GetOrderedHint(
    /* [out] */ Boolean* orderedHint)
{
    return PendingResult::GetOrderedHint(orderedHint);
}

ECode CActivityThread::ReceiverData::GetInitialStickyHint(
    /* [out] */ Boolean* initialStickyHint)
{
    return PendingResult::GetInitialStickyHint(initialStickyHint);
}

//==============================================================================
// CActivityThread::CreateBackupAgentData
//==============================================================================

ECode CActivityThread::CreateBackupAgentData::ToString(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    String baName;
    mAppInfo->GetBackupAgentName(&baName);
    StringBuilder sb("CreateBackupAgentData{appInfo=");
    sb += mAppInfo;
    sb += " backupAgent=";
    sb += baName;
    sb += " mode=";
    sb += mBackupMode;
    sb += "}";
    *description = sb.ToString();
    return NOERROR;
}

//==============================================================================
// CActivityThread::Profiler
//==============================================================================

void CActivityThread::Profiler::SetProfiler(
    /* [in] */ const String& file,
    /* [in] */ IParcelFileDescriptor* fd)
{
    if (mProfiling) {
        if (fd != NULL) {
//            try {
                fd->Close();
//            } catch (IOException e) {
//                // Ignore
//           }
        }
        return;
    }
    if (mProfileFd != NULL) {
//        try {
            mProfileFd->Close();
//        } catch (IOException e) {
//            // Ignore
//        }
    }
    mProfileFile = file;
    mProfileFd = fd;
}

void CActivityThread::Profiler::StartProfiling()
{
    if (mProfileFd == NULL || mProfiling) {
        return;
    }
//     try {
//         Debug.startMethodTracing(profileFile, profileFd.getFileDescriptor(),
//                 8 * 1024 * 1024, 0);
//         profiling = TRUE;
//     } catch (RuntimeException e) {
//         Slog.w(TAG, "Profiling failed on path " + profileFile);
//         try {
//             profileFd.close();
//             profileFd = null;
//         } catch (IOException e2) {
//             Slog.w(TAG, "Failure closing profile fd", e2);
//         }
//     }
}

void CActivityThread::Profiler::StopProfiling()
{
//     if (profiling) {
//         profiling = FALSE;
//         Debug.stopMethodTracing();
//         if (profileFd != null) {
//             try {
//                 profileFd.close();
//             } catch (IOException e) {
//             }
//         }
//         profileFd = null;
//         profileFile = null;
//     }
}
//==============================================================================
// CActivityThread::ResultData
//==============================================================================

ECode CActivityThread::ResultData::ToString(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    StringBuilder sb("ResultData{token=");
    sb += mToken;
    sb += " results";
    sb += mResults.GetSize();
    sb += "}";
    *description = sb.ToString();
    return NOERROR;
}

//==============================================================================
// CActivityThread::StopInfo
//==============================================================================
ECode CActivityThread::StopInfo::Run()
{
    // Tell activity manager we have been stopped.
    if (DEBUG_MEMORY_TRIM) Slogger::V(TAG, "Reporting activity stopped: %p", mActivity.Get());
    return ActivityManagerNative::GetDefault()->ActivityStopped(mActivity->mToken, mState, mThumbnail, mDescription);
}

//==============================================================================
// CActivityThread::ConfigurationChangedCallbacks
//==============================================================================
CAR_INTERFACE_IMPL_2(CActivityThread::ConfigurationChangedCallbacks, IComponentCallbacks, IComponentCallbacks2)

CActivityThread::ConfigurationChangedCallbacks::ConfigurationChangedCallbacks(
    /* [in] */ IWeakReference* activityThread)
    : mWeakHost(activityThread)
{
}

ECode CActivityThread::ConfigurationChangedCallbacks::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<IActivityThread> at;
    mWeakHost->Resolve(EIID_IActivityThread, (IInterface**)&at);
    if (at == NULL) {
        return NOERROR;
    }

    CActivityThread* host = (CActivityThread*)at.Get();
    AutoLock lock(host->mPackagesLock);

    // We need to apply this change to the resources
    // immediately, because upon returning the view
    // hierarchy will be informed about it.
    if (host->ApplyConfigurationToResourcesLocked(newConfig, NULL)) {
        // This actually changed the resources!  Tell
        // everyone about it.
        Boolean isOtherSeqNewer = FALSE;
        if (host->mPendingConfiguration) {
            host->mPendingConfiguration->IsOtherSeqNewer(newConfig, &isOtherSeqNewer);
        }
        if (host->mPendingConfiguration == NULL || isOtherSeqNewer) {
            host->mPendingConfiguration = newConfig;

            host->QueueOrSendMessage(CActivityThread::H::CONFIGURATION_CHANGED, newConfig);
        }
    }

    return NOERROR;
}

ECode CActivityThread::ConfigurationChangedCallbacks::OnLowMemory()
{
    return NOERROR;
}

ECode CActivityThread::ConfigurationChangedCallbacks::OnTrimMemory(
    /* [in] */ Int32 level)
{

    return NOERROR;
}

//==============================================================================
// CActivityThread::H
//==============================================================================

const Int32 CActivityThread::H::LAUNCH_ACTIVITY         = 100;
const Int32 CActivityThread::H::PAUSE_ACTIVITY          = 101;
const Int32 CActivityThread::H::PAUSE_ACTIVITY_FINISHING= 102;
const Int32 CActivityThread::H::STOP_ACTIVITY_SHOW      = 103;
const Int32 CActivityThread::H::STOP_ACTIVITY_HIDE      = 104;
const Int32 CActivityThread::H::SHOW_WINDOW             = 105;
const Int32 CActivityThread::H::HIDE_WINDOW             = 106;
const Int32 CActivityThread::H::RESUME_ACTIVITY         = 107;
const Int32 CActivityThread::H::SEND_RESULT             = 108;
const Int32 CActivityThread::H::DESTROY_ACTIVITY        = 109;
const Int32 CActivityThread::H::BIND_APPLICATION        = 110;
const Int32 CActivityThread::H::EXIT_APPLICATION        = 111;
const Int32 CActivityThread::H::NEW_INTENT              = 112;
const Int32 CActivityThread::H::RECEIVER                = 113;
const Int32 CActivityThread::H::CREATE_SERVICE          = 114;
const Int32 CActivityThread::H::SERVICE_ARGS            = 115;
const Int32 CActivityThread::H::STOP_SERVICE            = 116;
const Int32 CActivityThread::H::REQUEST_THUMBNAIL       = 117;
const Int32 CActivityThread::H::CONFIGURATION_CHANGED   = 118;
const Int32 CActivityThread::H::CLEAN_UP_CONTEXT        = 119;
const Int32 CActivityThread::H::GC_WHEN_IDLE            = 120;
const Int32 CActivityThread::H::BIND_SERVICE            = 121;
const Int32 CActivityThread::H::UNBIND_SERVICE          = 122;
const Int32 CActivityThread::H::DUMP_SERVICE            = 123;
const Int32 CActivityThread::H::LOW_MEMORY              = 124;
const Int32 CActivityThread::H::ACTIVITY_CONFIGURATION_CHANGED = 125;
const Int32 CActivityThread::H::RELAUNCH_ACTIVITY       = 126;
const Int32 CActivityThread::H::PROFILER_CONTROL        = 127;
const Int32 CActivityThread::H::CREATE_BACKUP_AGENT     = 128;
const Int32 CActivityThread::H::DESTROY_BACKUP_AGENT    = 129;
const Int32 CActivityThread::H::SUICIDE                 = 130;
const Int32 CActivityThread::H::REMOVE_PROVIDER         = 131;
const Int32 CActivityThread::H::ENABLE_JIT              = 132;
const Int32 CActivityThread::H::DISPATCH_PACKAGE_BROADCAST = 133;
const Int32 CActivityThread::H::SCHEDULE_CRASH          = 134;
const Int32 CActivityThread::H::DUMP_HEAP               = 135;
const Int32 CActivityThread::H::DUMP_ACTIVITY           = 136;
const Int32 CActivityThread::H::SLEEPING                = 137;
const Int32 CActivityThread::H::SET_CORE_SETTINGS       = 138;
const Int32 CActivityThread::H::UPDATE_PACKAGE_COMPATIBILITY_INFO = 139;
const Int32 CActivityThread::H::TRIM_MEMORY             = 140;
const Int32 CActivityThread::H::DUMP_PROVIDER           = 141;
const Int32 CActivityThread::H::UNSTABLE_PROVIDER_DIED  = 142;

CActivityThread::H::H(
    /* [in] */ CActivityThread* host)
    : HandlerBase(FALSE)
    , mHost(host)
{
}

String CActivityThread::H::CodeToString(
    /* [in] */ Int32 code)
{
   if (CActivityThread::DEBUG_MESSAGES) {
        switch (code) {
            case LAUNCH_ACTIVITY: return String("LAUNCH_ACTIVITY");
            case PAUSE_ACTIVITY: return String("PAUSE_ACTIVITY");
            case PAUSE_ACTIVITY_FINISHING: return String("PAUSE_ACTIVITY_FINISHING");
            case STOP_ACTIVITY_SHOW: return String("STOP_ACTIVITY_SHOW");
            case STOP_ACTIVITY_HIDE: return String("STOP_ACTIVITY_HIDE");
            case SHOW_WINDOW: return String("SHOW_WINDOW");
            case HIDE_WINDOW: return String("HIDE_WINDOW");
            case RESUME_ACTIVITY: return String("RESUME_ACTIVITY");
            case SEND_RESULT: return String("SEND_RESULT");
            case DESTROY_ACTIVITY: return String("DESTROY_ACTIVITY");
            case BIND_APPLICATION: return String("BIND_APPLICATION");
            case EXIT_APPLICATION: return String("EXIT_APPLICATION");
            case NEW_INTENT: return String("NEW_INTENT");
            case RECEIVER: return String("RECEIVER");
            case CREATE_SERVICE: return String("CREATE_SERVICE");
            case SERVICE_ARGS: return String("SERVICE_ARGS");
            case STOP_SERVICE: return String("STOP_SERVICE");
            case REQUEST_THUMBNAIL: return String("REQUEST_THUMBNAIL");
            case CONFIGURATION_CHANGED: return String("CONFIGURATION_CHANGED");
            case CLEAN_UP_CONTEXT: return String("CLEAN_UP_CONTEXT");
            case GC_WHEN_IDLE: return String("GC_WHEN_IDLE");
            case BIND_SERVICE: return String("BIND_SERVICE");
            case UNBIND_SERVICE: return String("UNBIND_SERVICE");
            case DUMP_SERVICE: return String("DUMP_SERVICE");
            case LOW_MEMORY: return String("LOW_MEMORY");
            case ACTIVITY_CONFIGURATION_CHANGED: return String("ACTIVITY_CONFIGURATION_CHANGED");
            case RELAUNCH_ACTIVITY: return String("RELAUNCH_ACTIVITY");
            case PROFILER_CONTROL: return String("PROFILER_CONTROL");
            case CREATE_BACKUP_AGENT: return String("CREATE_BACKUP_AGENT");
            case DESTROY_BACKUP_AGENT: return String("DESTROY_BACKUP_AGENT");
            case SUICIDE: return String("SUICIDE");
            case REMOVE_PROVIDER: return String("REMOVE_PROVIDER");
            case ENABLE_JIT: return String("ENABLE_JIT");
            case DISPATCH_PACKAGE_BROADCAST: return String("DISPATCH_PACKAGE_BROADCAST");
            case SCHEDULE_CRASH: return String("SCHEDULE_CRASH");
            case DUMP_HEAP: return String("DUMP_HEAP");
            case DUMP_ACTIVITY: return String("DUMP_ACTIVITY");
            case SLEEPING: return String("SLEEPING");
            case SET_CORE_SETTINGS: return String("SET_CORE_SETTINGS");
            case UPDATE_PACKAGE_COMPATIBILITY_INFO: return String("UPDATE_PACKAGE_COMPATIBILITY_INFO");
            case TRIM_MEMORY: return String("TRIM_MEMORY");
            case DUMP_PROVIDER: return String("DUMP_PROVIDER");
            case UNSTABLE_PROVIDER_DIED: return String("UNSTABLE_PROVIDER_DIED");
        }
    }

    return StringUtils::Int32ToString(code);
}

void CActivityThread::H::MaybeSnapshot()
{
    // if (mHost->mBoundApplication != NULL && SamplingProfilerIntegration.isEnabled()) {
    //     // convert the *private* ActivityThread.PackageInfo to *public* known
    //     // android.content.pm.PackageInfo
    //     String packageName = mBoundApplication.info.mPackageName;
    //     android.content.pm.PackageInfo packageInfo = null;
    //     try {
    //         Context context = getSystemContext();
    //         if(context == null) {
    //             Log.e(TAG, "cannot get a valid context");
    //             return;
    //         }
    //         PackageManager pm = context.getPackageManager();
    //         if(pm == null) {
    //             Log.e(TAG, "cannot get a valid PackageManager");
    //             return;
    //         }
    //         packageInfo = pm.getPackageInfo(
    //                 packageName, PackageManager.GET_ACTIVITIES);
    //     } catch (NameNotFoundException e) {
    //         Log.e(TAG, "cannot get package info for " + packageName, e);
    //     }
    //     SamplingProfilerIntegration.writeSnapshot(mBoundApplication.processName, packageInfo);
    // }
}

ECode CActivityThread::H::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    if (CActivityThread::DEBUG_MESSAGES) {
        Slogger::V("CActivityThread", ">>> handling: %s", CodeToString(what).string());
    }

    ECode ec = NOERROR;
    switch (what) {
        case LAUNCH_ACTIVITY: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityStart");
            AutoPtr<ActivityClientRecord> r = (ActivityClientRecord*)obj.Get();
            assert(r != NULL);
            AutoPtr<IApplicationInfo> appInfo;
            r->mActivityInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            r->mPackageInfo = mHost->GetPackageInfoNoCheck(appInfo, r->mCompatInfo);
            ec = mHost->HandleLaunchActivity(r, NULL);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
        } break;
        case RELAUNCH_ACTIVITY: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityRestart");
            AutoPtr<ActivityClientRecord> r = (ActivityClientRecord*)obj.Get();
            assert(r != NULL);
            ec = mHost->HandleRelaunchActivity(r);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
        } break;
        case PAUSE_ACTIVITY:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityPause");
            ec = mHost->HandlePauseActivity(IBinder::Probe(obj.Get()), FALSE, arg1 != 0, arg2);
            MaybeSnapshot();
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case PAUSE_ACTIVITY_FINISHING:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityPause");
            ec = mHost->HandlePauseActivity(IBinder::Probe(obj.Get()), TRUE, arg1 != 0, arg2);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case STOP_ACTIVITY_SHOW:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityStop");
            ec = mHost->HandleStopActivity(IBinder::Probe(obj.Get()), TRUE, arg2);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case STOP_ACTIVITY_HIDE:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityStop");
            ec = mHost->HandleStopActivity(IBinder::Probe(obj.Get()), FALSE, arg2);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case SHOW_WINDOW:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityShowWindow");
            ec = mHost->HandleWindowVisibility(IBinder::Probe(obj.Get()), TRUE);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case HIDE_WINDOW:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityHideWindow");
            ec = mHost->HandleWindowVisibility(IBinder::Probe(obj.Get()), FALSE);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case RESUME_ACTIVITY:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityResume");
            ec = mHost->HandleResumeActivity(IBinder::Probe(obj.Get()), TRUE, arg1 != 0, TRUE);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case SEND_RESULT: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityDeliverResult");
            AutoPtr<ResultData> data = (ResultData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleSendResult(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case DESTROY_ACTIVITY:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityDestroy");
            ec = mHost->HandleDestroyActivity(IBinder::Probe(obj.Get()), arg1 != 0, arg2, FALSE);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case BIND_APPLICATION: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "bindApplication");
            AutoPtr<AppBindData> data = (AppBindData*)obj.Get();
            ec = mHost->HandleBindApplication(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case EXIT_APPLICATION:
            if (mHost->mInitialApplication != NULL) {
                mHost->mInitialApplication->OnTerminate();
            }
            Looper::GetMyLooper()->Quit();
            break;
        case NEW_INTENT: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityNewIntent");
            AutoPtr<NewIntentData> data = (NewIntentData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleNewIntent(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case RECEIVER: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "broadcastReceiveComp");
            AutoPtr<ReceiverData> data = (ReceiverData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleReceiver(data);
            MaybeSnapshot();
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case CREATE_SERVICE: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "serviceCreate");
            AutoPtr<CreateServiceData> data = (CreateServiceData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleCreateService(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case BIND_SERVICE: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "serviceBind");
            AutoPtr<BindServiceData> data = (BindServiceData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleBindService(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case UNBIND_SERVICE: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "serviceUnbind");
            AutoPtr<BindServiceData> data = (BindServiceData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleUnbindService(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case SERVICE_ARGS: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "serviceStart");
            AutoPtr<ServiceArgsData> data = (ServiceArgsData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleServiceArgs(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case STOP_SERVICE:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "serviceStop");
            ec = mHost->HandleStopService(IBinder::Probe(obj.Get()));
            MaybeSnapshot();
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case REQUEST_THUMBNAIL:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "requestThumbnail");
            ec = mHost->HandleRequestThumbnail(IBinder::Probe(obj.Get()));
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case CONFIGURATION_CHANGED: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "configChanged");
            AutoPtr<IConfiguration> cfg = IConfiguration::Probe(obj.Get());
            assert(cfg != NULL);
            cfg->GetDensityDpi(&mHost->mCurDefaultDisplayDpi);
            ec = mHost->HandleConfigurationChanged(cfg, NULL);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case CLEAN_UP_CONTEXT: {
            AutoPtr<ContextCleanupInfo> cci = (ContextCleanupInfo*)obj.Get();
            assert(cci != NULL);

            ec = ((CContextImpl*)(IContext*)cci->mContext)->PerformFinalCleanup(
                cci->mWho, cci->mWhat);
            break;
        }
        case GC_WHEN_IDLE:
            mHost->ScheduleGcIdler();
            break;
        case DUMP_SERVICE: {
            AutoPtr<DumpComponentInfo> data = (DumpComponentInfo*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleDumpService(data);
            break;
        }
        case LOW_MEMORY:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "lowMemory");
            ec = mHost->HandleLowMemory();
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case ACTIVITY_CONFIGURATION_CHANGED:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "activityConfigChanged");
            ec = mHost->HandleActivityConfigurationChanged(IBinder::Probe(obj.Get()));
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case PROFILER_CONTROL: {
            AutoPtr<ProfilerControlData> data = (ProfilerControlData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleProfilerControl(arg1 != 0, data, arg2);
            break;
        }
        case CREATE_BACKUP_AGENT: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "backupCreateAgent");
            AutoPtr<CreateBackupAgentData> data = (CreateBackupAgentData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleCreateBackupAgent(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case DESTROY_BACKUP_AGENT: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "backupDestroyAgent");
            AutoPtr<CreateBackupAgentData> data = (CreateBackupAgentData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleDestroyBackupAgent(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case SUICIDE:
            Process::KillProcess(Process::MyPid());
            break;
        case REMOVE_PROVIDER: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "providerRemove");
            AutoPtr<ProviderRefCount> data = (ProviderRefCount*)obj.Get();
            assert(data != NULL);
            ec = mHost->CompleteRemoveProvider(data);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case ENABLE_JIT:
            ec = mHost->EnsureJitEnabled();
            break;
        case DISPATCH_PACKAGE_BROADCAST: {
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "broadcastPackage");
            AutoPtr<IObjectContainer> container = IObjectContainer::Probe(obj.Get());
            AutoPtr<ArrayOf<String> > array;
            if (container) {
                Int32 count;
                container->GetObjectCount(&count);
                array = ArrayOf<String>::Alloc(count);

                AutoPtr<IObjectEnumerator> emu;
                container->GetObjectEnumerator((IObjectEnumerator**)&emu);
                Boolean hasNext;
                Int32 i = 0;
                while (emu->MoveNext(&hasNext), hasNext) {
                    AutoPtr<ICharSequence> seq;
                    emu->Current((IInterface**)&seq);
                    String str;
                    seq->ToString(&str);
                    (*array)[i++] = str;
                }
            }
            else {
                array = ArrayOf<String>::Alloc(0);
            }
            ec = mHost->HandleDispatchPackageBroadcast(arg1, array);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        }
        case SCHEDULE_CRASH: {
            AutoPtr<ICharSequence> seq = ICharSequence::Probe(obj.Get());
            String info;
            seq->ToString(&info);
            Slogger::E("CActivityThread", "RemoteServiceException: %s", info.string());
            ec = E_REMOTE_SERVICE_EXCEPTION;
            // throw new RemoteServiceException((String)msg.obj);
            break;
        }
        case DUMP_HEAP: {
            AutoPtr<DumpHeapData> data = (DumpHeapData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleDumpHeap(arg1 != 0, data);
            break;
        }
        case DUMP_ACTIVITY: {
            AutoPtr<DumpComponentInfo> data = (DumpComponentInfo*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleDumpActivity(data);
            break;
        }
        case DUMP_PROVIDER: {
            AutoPtr<DumpComponentInfo> data = (DumpComponentInfo*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleDumpProvider(data);
            break;
        }
        case SLEEPING:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "sleeping");
            ec = mHost->HandleSleeping(IBinder::Probe(obj.Get()), arg1 != 0);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case SET_CORE_SETTINGS:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "setCoreSettings");
            ec = mHost->HandleSetCoreSettings(IBundle::Probe(obj));
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case UPDATE_PACKAGE_COMPATIBILITY_INFO: {
            AutoPtr<UpdateCompatibilityData> data = (UpdateCompatibilityData*)obj.Get();
            assert(data != NULL);
            ec = mHost->HandleUpdatePackageCompatibilityInfo(data);
            break;
        }
        case TRIM_MEMORY:
            //Trace.traceBegin(//Trace.trace_TAG_ACTIVITY_MANAGER, "trimMemory");
            ec = mHost->HandleTrimMemory(arg1);
            //Trace.traceEnd(//Trace.trace_TAG_ACTIVITY_MANAGER);
            break;
        case UNSTABLE_PROVIDER_DIED:
            ec = mHost->HandleUnstableProviderDied(IBinder::Probe(obj.Get()), FALSE);
            break;
    }

    if (CActivityThread::DEBUG_MESSAGES) {
        Slogger::V("CActivityThread", "<<< handled: %s", CodeToString(what).string());
    }
    return ec;
}

//==============================================================================
// CActivityThread
//==============================================================================
CActivityThread::CActivityThread()
    : mCurDefaultDisplayDpi(0)
    , mDensityCompatMode(0)
    , mSystemThread(FALSE)
    , mJitEnabled(FALSE)
    , mGcIdlerScheduled(FALSE)
{
    ASSERT_SUCCEEDED(CApplicationThread::New((IApplicationThread**)&mAppThread));
    ((CApplicationThread*)mAppThread.Get())->mAThread = this;
    mLooper = Looper::GetMyLooper();

    mH = new H(this);
}

CActivityThread::~CActivityThread()
{
    mOnPauseListeners.Clear();
}

ECode CActivityThread::constructor()
{
    InitTLS();
    return NOERROR;
}

static void ActivityThreadDestructor(void* st)
{
    IActivityThread* obj = static_cast<IActivityThread*>(st);
    if (obj) {
        obj->Release();
    }
}

static void IntentDestructor(void* st)
{
    IIntent* obj = static_cast<IIntent*>(st);
    if (obj) {
        obj->Release();
    }
}

void CActivityThread::InitTLS()
{
    if (!sHaveKey) {
        Int32 result = pthread_key_create(&sKey, ActivityThreadDestructor);
        assert(result == 0);

        result = pthread_key_create(&sCurrentBroadcastIntentKey, IntentDestructor);
        assert(result == 0);

        sHaveKey = TRUE;
    }
}

AutoPtr<IActivityThread> CActivityThread::GetCurrentActivityThread()
{
    AutoPtr<IActivityThread> thr = (IActivityThread*)pthread_getspecific(sKey);
    return thr;
}

String CActivityThread::GetCurrentPackageName()
{
    assert(0);
    return String(NULL);
}

AutoPtr<IApplication> CActivityThread::GetCurrentApplication()
{
    assert(0);
    return NULL;
}

AutoPtr<IIPackageManager> CActivityThread::GetPackageManager()
{
    if (sPackageManager != NULL) {
        //Slogger::V("PackageManager", "returning cur default = " + sPackageManager);
        return sPackageManager;
    }
    AutoPtr<IInterface> b = ServiceManager::GetService(String("package"));
    assert(b != NULL);
    //Slogger::V("PackageManager", "default service binder = " + b);
    sPackageManager = IIPackageManager::Probe(b);
    assert(sPackageManager != NULL);
    //Slogger::V("PackageManager", "default service = " + sPackageManager);
    return sPackageManager;
}

void CActivityThread::FlushDisplayMetricsLocked()
{
     mDefaultDisplayMetrics.Clear();
}

ECode CActivityThread::GetDisplayMetricsLocked(
    /* [in] */ Int32 displayId,
    /* [in] */ ICompatibilityInfo* ci,
    /* [out] */ IDisplayMetrics** dms)
{
    VALIDATE_NOT_NULL(dms);
    *dms = NULL;

    Boolean isDefaultDisplay = (displayId == IDisplay::DEFAULT_DISPLAY);
    HashMap<AutoPtr<ICompatibilityInfo>, AutoPtr<IDisplayMetrics> >::Iterator it =
            mDefaultDisplayMetrics.Find(ci);
    AutoPtr<IDisplayMetrics> dm;
    if (isDefaultDisplay && it != mDefaultDisplayMetrics.End()) {
        dm = it->mSecond;
    }
    if (dm != NULL) {
        *dms = dm;
        REFCOUNT_ADD(*dms);
        return NOERROR;
    }
    CDisplayMetrics::New((IDisplayMetrics**)&dm);

    AutoPtr<DisplayManagerGlobal> displayManager = DisplayManagerGlobal::GetInstance();
    if (displayManager == NULL) {
        // may be null early in system startup
        dm->SetToDefaults();
        *dms = dm;
        REFCOUNT_ADD(*dms);
        return NOERROR;
    }

    if (isDefaultDisplay) {
        mDefaultDisplayMetrics[ci] = dm;
    }

    AutoPtr<ICompatibilityInfoHolder> cih;
    CCompatibilityInfoHolder::New((ICompatibilityInfoHolder**)&cih);
    cih->Set(ci);
    AutoPtr<IDisplay> d;
    displayManager->GetCompatibleDisplay(displayId, cih, (IDisplay**)&d);
    if (d != NULL) {
        d->GetMetrics(dm);
    }
    else {
        // Display no longer exists
        // FIXME: This would not be a problem if we kept the Display object around
        // instead of using the raw display id everywhere.  The Display object caches
        // its information even after the display has been removed.
        dm->SetToDefaults();
    }
    //Slog.i("foo", "New metrics: w=" + metrics.widthPixels + " h="
    //        + metrics.heightPixels + " den=" + metrics.density
    //        + " xdpi=" + metrics.xdpi + " ydpi=" + metrics.ydpi);
    *dms = dm;
    REFCOUNT_ADD(*dms);
    return NOERROR;
}

AutoPtr<IConfiguration> CActivityThread::ApplyConfigCompatMainThread(
    /* [in] */ Int32 displayDensity,
    /* [in] */ IConfiguration* config,
    /* [in] */ ICompatibilityInfo* compat)
{
    if (config == NULL) {
        return NULL;
    }
    Boolean isSupport;
    if (compat != NULL) {
        compat->SupportsScreen(&isSupport);
        if (!isSupport) {
            mMainThreadConfig->SetTo(config);
            config = mMainThreadConfig;
            compat->ApplyToConfiguration(displayDensity, config);
        }
    }
    return config;
}

ECode CActivityThread::GetTopLevelResources(
    /* [in] */ const String& resDir,
    /* [in] */ Int32 displayId,
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [in] */ CCompatibilityInfo* compInfo,
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);
    *res = NULL;
    VALIDATE_NOT_NULL(compInfo);

    AutoPtr<ResourcesKey> key = new ResourcesKey(resDir, displayId,
            overrideConfiguration, compInfo->mApplicationScale);
    AutoPtr<IResources> r;
    {
        AutoLock lock(mPackagesLock);

        // Resources is app scale dependent.
        if (FALSE) {
            Slogger::W(TAG, "getTopLevelResources: %s / %f", resDir.string(),
                    compInfo->mApplicationScale);
        }
        ActiveResourcesMapIterator it = mActiveResources.Find(key);
        if (it != mActiveResources.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            wr->Resolve(EIID_IResources, (IInterface**)&r);
        }
        //if (r != null) Slog.i(TAG, "isUpToDate " + resDir + ": " + r.getAssets().isUpToDate());
        if (r != NULL) {
            Boolean updated;
            AutoPtr<IAssetManager> assets;
            r->GetAssets((IAssetManager**)&assets);
            assets->IsUpToDate(&updated);
            if (updated) {
                if (FALSE) {
                    AutoPtr<CCompatibilityInfo> ccInfo;
                    r->GetCompatibilityInfo((ICompatibilityInfo**)&ccInfo);
                    Slogger::W(TAG, "Returning cached resources %p %s: appScale=%f",
                       r.Get(), resDir.string(), ccInfo->mApplicationScale);
                }
                *res = (IResources*)(r.Get());
                REFCOUNT_ADD(*res);
                return NOERROR;
            }
        }
    }

    //if (r != null) {
    //    Slog.w(TAG, "Throwing away out-of-date resources!!!! "
    //            + r + " " + resDir);
    //}

    AutoPtr<CAssetManager> assets;
    CAssetManager::NewByFriend((CAssetManager**)&assets);
    Int32 cookie;
    assets->AddAssetPath(resDir, &cookie);
    if (cookie == 0) {
        return E_FAIL;
    }

    //Slog.i(TAG, "Resource: key=" + key + ", display metrics=" + metrics);
    AutoPtr<IDisplayMetrics> dm;
    GetDisplayMetricsLocked(displayId, NULL, (IDisplayMetrics**)&dm);
    AutoPtr<IConfiguration> config;
    Boolean isDefaultDisplay = (displayId == IDisplay::DEFAULT_DISPLAY);
    if (!isDefaultDisplay || key->mOverrideConfiguration != NULL) {
        AutoPtr<IConfiguration> gConfig;
        GetConfiguration((IConfiguration**)&gConfig);
        CConfiguration::New(gConfig, (IConfiguration**)&config);
        if (!isDefaultDisplay) {
            ApplyNonDefaultDisplayMetricsToConfigurationLocked(dm, config);
        }
        if (key->mOverrideConfiguration != NULL) {
            Int32 changes;
            config->UpdateFrom(key->mOverrideConfiguration, &changes);
        }
    } else {
        GetConfiguration((IConfiguration**)&config);
    }

    r = NULL;
    CResources::New((IAssetManager*)assets.Get(), dm.Get(),
            config.Get(), (ICompatibilityInfo*)compInfo, (IResources**)&r);
    if (FALSE) {
        AutoPtr<IConfiguration> rconfig;
        r->GetConfiguration((IConfiguration**)&rconfig);
        AutoPtr<ICompatibilityInfo> comInfo;
        r->GetCompatibilityInfo((ICompatibilityInfo**)&comInfo);
        Float scale;
        comInfo->GetApplicationScale(&scale);
        Slogger::I(TAG, "Created app resources %s %p: %p appScale=%f",
                resDir.string(), r.Get(), rconfig.Get(), scale);
    }

    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IResources> existing;
        ActiveResourcesMapIterator it = mActiveResources.Find(key);
        if (it != mActiveResources.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            wr->Resolve(EIID_IResources, (IInterface**)&existing);
        }
        if (existing != NULL) {
            Boolean updated;
            AutoPtr<IAssetManager> assets;
            existing->GetAssets((IAssetManager**)&assets);
            assets->IsUpToDate(&updated);
            if (updated) {
                // Someone else already created the resources while we were
                // unlocked; go ahead and use theirs.
                AutoPtr<IAssetManager> assets1;
                r->GetAssets((IAssetManager**)&assets1);
                assets1->Close();
                *res = existing.Get();
                REFCOUNT_ADD(*res);
                return NOERROR;
            }
        }

        // XXX need to remove entries when weak references go away
        AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(r);
        assert(wrs != NULL);
        AutoPtr<IWeakReference> wr;
        wrs->GetWeakReference((IWeakReference**)&wr);
        mActiveResources[key] = wr;
        *res = r.Get();
        REFCOUNT_ADD(*res);
        return NOERROR;
    }
}

ECode CActivityThread::GetTopLevelResources(
    /* [in] */ const String& resDir,
    /* [in] */ Int32 displayId,
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [in] */ LoadedPkg* pkgInfo,
    /* [out] */ IResources** res)
{
    AutoPtr<ICompatibilityInfo> info;
    pkgInfo->mCompatibilityInfo->Get((ICompatibilityInfo**)&info);
    return GetTopLevelResources(resDir, displayId,
            overrideConfiguration, (CCompatibilityInfo*)info.Get(), res);
}

ECode CActivityThread::GetHandler(
    /* [out] */ IHandler** h)
{
    VALIDATE_NOT_NULL(h)
    *h = GetHandler();
    REFCOUNT_ADD(*h)
    return NOERROR;
}

AutoPtr<IHandler> CActivityThread::GetHandler()
{
    return mH;
}

AutoPtr<LoadedPkg> CActivityThread::GetPackageInfo(
    /* [in] */ const String& packageName,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ Int32 flags)
{
    return GetPackageInfo(packageName, compatInfo, flags, UserHandle::GetMyUserId());
}

AutoPtr<LoadedPkg> CActivityThread::GetPackageInfo(
    /* [in] */ const String& packageName,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId)
{
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<LoadedPkg> packageInfo;
        if ((flags & IContext::CONTEXT_INCLUDE_CODE) != 0) {
            LoadedApkMapIterator it = mPackages.Find(packageName);
            if (it != mPackages.End()) {
                AutoPtr<IWeakReference> wr = it->mSecond;
                AutoPtr<IInterface> obj;
                wr->Resolve(EIID_IInterface, (IInterface**)&obj);
                packageInfo = (LoadedPkg*)obj.Get();
            }
        }
        else {
            LoadedApkMapIterator it = mResourcePackages.Find(packageName);
            if (it != mResourcePackages.End()) {
                AutoPtr<IWeakReference> wr = it->mSecond;
                AutoPtr<IInterface> obj;
                wr->Resolve(EIID_IInterface, (IInterface**)&obj);
                packageInfo = (LoadedPkg*)obj.Get();
            }
        }
        //Slog.i(TAG, "getPackageInfo " + packageName + ": " + packageInfo);
        //if (packageInfo != null) Slog.i(TAG, "isUptoDate " + packageInfo.mResDir
        //        + ": " + packageInfo.mResources.getAssets().isUpToDate());
        AutoPtr<IAssetManager> assertmr;
        Boolean isUpdated;
        if (packageInfo != NULL && (packageInfo->mResources == NULL ||
            (packageInfo->mResources->GetAssets((IAssetManager**)&assertmr), assertmr->IsUpToDate(&isUpdated), isUpdated))) {
            if (packageInfo->IsSecurityViolation()
                    && (flags & IContext::CONTEXT_IGNORE_SECURITY) == 0) {
    //            throw new SecurityException(
    //                    "Requesting code from " + packageName
    //                    + " to be run in process "
    //                    + mBoundApplication.processName
    //                    + "/" + mBoundApplication.appInfo.uid);
            }
            return packageInfo;
        }
    }

    AutoPtr<IApplicationInfo> ai;
//    try {
    GetPackageManager()->GetApplicationInfo(packageName,
        IPackageManager::GET_SHARED_LIBRARY_FILES, userId,
        (IApplicationInfo**)&ai);
//    } catch (RemoteException e) {
//    }

    if (ai != NULL) {
        return GetPackageInfo(ai, compatInfo, flags);
    }

    return NULL;
}

AutoPtr<LoadedPkg> CActivityThread::GetPackageInfo(
    /* [in] */ IApplicationInfo* ai,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ Int32 flags)
{
    Boolean includeCode = (flags & IContext::CONTEXT_INCLUDE_CODE) != 0;
    Int32 aiUid, baUid;
    ai->GetUid(&aiUid);
    if (mBoundApplication != NULL) mBoundApplication->mAppInfo->GetUid(&baUid);
    Boolean securityViolation = includeCode && aiUid != 0
            && aiUid != IProcess::SYSTEM_UID
            && (mBoundApplication != NULL ? !UserHandle::IsSameApp(aiUid, baUid) : TRUE);
    if ((flags & (IContext::CONTEXT_INCLUDE_CODE
            | IContext::CONTEXT_IGNORE_SECURITY)) == IContext::CONTEXT_INCLUDE_CODE) {
        if (securityViolation) {
            String packageName;
            ai->GetPackageName(&packageName);
            StringBuilder sb("Requesting code from ");
            sb += packageName;
            sb += " (with uid ";
            sb += aiUid;
            if (mBoundApplication != NULL) {
                sb += " to be run in process ";
                sb += mBoundApplication->mProcessName;
                sb += " (with uid ";
                sb += baUid;
                sb += ")";
            }
//            throw new SecurityException(msg);
            Slogger::E(TAG, sb.ToString());
            return NULL;
        }
    }
    return GetPackageInfo(ai, compatInfo, NULL, securityViolation, includeCode);
}

AutoPtr<LoadedPkg> CActivityThread::GetPackageInfoNoCheck(
    /* [in] */ IApplicationInfo* ai,
    /* [in] */ ICompatibilityInfo* compatInfo)
{
    return GetPackageInfo(ai, compatInfo, NULL, FALSE, TRUE);
}

AutoPtr<LoadedPkg> CActivityThread::PeekPackageInfo(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean includeCode)
{
    AutoLock lock(mPackagesLock);

    AutoPtr<LoadedPkg> packageInfo;
    if (includeCode) {
        LoadedApkMapIterator it = mPackages.Find(packageName);
        if (it != mPackages.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            AutoPtr<IInterface> obj;
            wr->Resolve(EIID_IInterface, (IInterface**)&obj);
            packageInfo = (LoadedPkg*)obj.Get();
        }
    }
    else {
        LoadedApkMapIterator it = mResourcePackages.Find(packageName);
        if (it != mResourcePackages.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            AutoPtr<IInterface> obj;
            wr->Resolve(EIID_IInterface, (IInterface**)&obj);
            packageInfo = (LoadedPkg*)obj.Get();
        }
    }
    return packageInfo;
}

AutoPtr<LoadedPkg> CActivityThread::GetPackageInfo(
    /* [in] */ IApplicationInfo* aInfo,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ IClassLoader* baseLoader,
    /* [in] */ Boolean securityViolation,
    /* [in] */ Boolean includeCode)
{
    AutoPtr<LoadedPkg> packageInfo;

    AutoLock lock(mPackagesLock);

    String pkgName;
    aInfo->GetPackageName(&pkgName);
    if (includeCode) {
        LoadedApkMapIterator it = mPackages.Find(pkgName);
        if (it != mPackages.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            AutoPtr<IInterface> obj;
            wr->Resolve(EIID_IInterface, (IInterface**)&obj);
            packageInfo = (LoadedPkg*)obj.Get();
        }
    }
    else {
        LoadedApkMapIterator it = mResourcePackages.Find(pkgName);
        if (it != mResourcePackages.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            AutoPtr<IInterface> obj;
            wr->Resolve(EIID_IInterface, (IInterface**)&obj);
            packageInfo = (LoadedPkg*)obj.Get();
        }
    }
    AutoPtr<IAssetManager> asset;
    Boolean isUp;
    if (packageInfo == NULL || (packageInfo->mResources != NULL
            && (packageInfo->mResources->GetAssets((IAssetManager**)&asset), asset->IsUpToDate(&isUp), !isUp))) {
        if (localLOGV) {
            StringBuilder sb(includeCode ? "Loading code package "
                   : "Loading resource-only package ");
            sb += pkgName;
            sb += " (in ";
            sb += (mBoundApplication != NULL
                    ? mBoundApplication->mProcessName : String(NULL));
            sb += ")";
            Slogger::V(TAG, sb.ToString());
        }
        Int32 flags;
        aInfo->GetFlags(&flags);
        packageInfo = new LoadedPkg(this,
            aInfo, compatInfo, this, baseLoader, securityViolation, includeCode &&
            (flags & IApplicationInfo::FLAG_HAS_CODE) != 0);
        AutoPtr<IWeakReference> wr;
        packageInfo->GetWeakReference((IWeakReference**)&wr);
        assert(wr != NULL);
        if (includeCode) {
            mPackages[pkgName] = wr;
        }
        else {
            mResourcePackages[pkgName] = wr;
        }
    }
    return packageInfo;
}

ECode CActivityThread::GetApplicationThread(
    /* [out] */ IApplicationThread** thread)
{
    VALIDATE_NOT_NULL(thread);
    *thread = (IApplicationThread*)mAppThread;
    REFCOUNT_ADD(*thread);
    return NOERROR;
}

ECode CActivityThread::GetInstrumentation(
    /* [out] */ IInstrumentation** instrumentation)
{
    VALIDATE_NOT_NULL(instrumentation);
    *instrumentation = mInstrumentation;
    REFCOUNT_ADD(*instrumentation);
    return NOERROR;
}

ECode CActivityThread::GetConfiguration(
    /* [out] */ IConfiguration** configuration)
{
    VALIDATE_NOT_NULL(configuration);
    *configuration = mResConfiguration;
    REFCOUNT_ADD(*configuration);
    return NOERROR;
}

ECode CActivityThread::IsProfiling(
    /* [out] */ Boolean* profiled)
{
    VALIDATE_NOT_NULL(profiled);
    *profiled = mProfiler != NULL && !mProfiler->mProfileFile.IsNull()
            && mProfiler->mProfileFd == NULL;
    return NOERROR;
}

ECode CActivityThread::GetProfileFilePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mProfiler->mProfileFile;
    return NOERROR;
}

ECode CActivityThread::GetLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper);
    *looper = mLooper;
    REFCOUNT_ADD(*looper);
    return NOERROR;
}

ECode CActivityThread::GetApplication(
    /* [out] */ IApplication** application)
{
    VALIDATE_NOT_NULL(application);
    *application = mInitialApplication;
    REFCOUNT_ADD(*application);
    return NOERROR;
}

ECode CActivityThread::GetProcessName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mBoundApplication->mProcessName;
    return NOERROR;
}

ECode CActivityThread::GetSystemContext(
    /* [out] */ IContextImpl** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    {
        AutoLock lock(mLock);

        if (sSystemContext == NULL) {
            AutoPtr<CContextImpl> context = CContextImpl::CreateSystemContext(this);
            AutoPtr<LoadedPkg> info = new LoadedPkg(this, String("android"), context, NULL,
                    CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO);
            context->Init(info, NULL, this);
            AutoPtr<CResources> resources;
            context->GetResources((IResources**)&resources);
            AutoPtr<IConfiguration> configuration;
            GetConfiguration((IConfiguration**)&configuration);
            AutoPtr<IDisplayMetrics> dmetrics;
            GetDisplayMetricsLocked(
                IDisplay::DEFAULT_DISPLAY,
                CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO,
                (IDisplayMetrics**)&dmetrics);
            resources->UpdateConfiguration(
                configuration, dmetrics);
            sSystemContext = context;
            //Slog.i(TAG, "Created system resources " + context.getResources()
            //        + ": " + context.getResources().getConfiguration());
        }
    }
    *ctx = sSystemContext;
    REFCOUNT_ADD(*ctx);
    return NOERROR;
}

ECode CActivityThread::InstallSystemApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    Slogger::D(TAG, " > TODO: CActivityThread::InstallSystemApplicationInfo");
    AutoLock lock(mLock);
    #if 0
    AutoPtr<IContextImpl> context;
    GetSystemContext((IContextImpl**)&context);
    AutoPtr<LoadedPkg> loadedPkg = new LoadedPkg(
        this, String("android"), context, info,
        CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO);
    ((CContextImpl*)context.Get())->Init(loadedPkg, NULL, this);
    #endif
    // give ourselves a default profiler
    mProfiler = new Profiler();
    return NOERROR;
}

ECode CActivityThread::EnsureJitEnabled()
{
//     if (!mJitEnabled) {
//         mJitEnabled = TRUE;
//         dalvik.system.VMRuntime.getRuntime().startJitCompilation();
//     }
    return NOERROR;
}

ECode CActivityThread::ScheduleGcIdler()
{
//    if (!mGcIdlerScheduled) {
//        mGcIdlerScheduled = TRUE;
// //       Looper::GetMyQueue()->AddIdleHandler(mGcIdler);
//    }
//  mH.removeMessages(H.GC_WHEN_IDLE);
    return NOERROR;
}

ECode CActivityThread::UnscheduleGcIdler()
{
//     if (mGcIdlerScheduled) {
//         mGcIdlerScheduled = FALSE;
// //        Looper.myQueue().removeIdleHandler(mGcIdler);
//     }
//  mH.removeMessages(H.GC_WHEN_IDLE);
    return NOERROR;
}

void CActivityThread::DoGcIfNeeded()
{
    mGcIdlerScheduled = FALSE;
//    const Int64 now = SystemClock::GetUptimeMillis();
    //Slog.i(TAG, "**** WE MIGHT WANT TO GC: then=" + Binder.getLastGcTime()
    //        + "m now=" + now);
//     if ((BinderInternal.getLastGcTime()+MIN_TIME_BETWEEN_GCS) < now) {
//         //Slog.i(TAG, "**** WE DO, WE DO WANT TO GC!");
//         BinderInternal.forceGc("bg");
//     }
}

ECode CActivityThread::RegisterOnActivityPausedListener(
    /* [in] */ IActivity* activity,
    /* [in] */ IOnActivityPausedListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    AutoLock lock(mOnPauseListenersLock);
    AutoPtr<OnPauseListenerList> list;
    OnPauseListenerMapIterator it = mOnPauseListeners.Find(activity);
    if (it != mOnPauseListeners.End()) {
        list = it->mSecond;
    }

    if (list == NULL) {
        list = new List<AutoPtr<IOnActivityPausedListener> >();
        mOnPauseListeners[activity] = list;
    }
    list->PushBack(listener);
    return NOERROR;
}

ECode CActivityThread::UnregisterOnActivityPausedListener(
    /* [in] */ IActivity* activity,
    /* [in] */ IOnActivityPausedListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    AutoLock lock(mOnPauseListenersLock);

    OnPauseListenerMapIterator it = mOnPauseListeners.Find(activity);
    if (it != mOnPauseListeners.End()) {
        AutoPtr<OnPauseListenerList> list = it->mSecond;
        if (list != NULL) {
            list->Remove(listener);
        }
    }

    return NOERROR;
}

ECode CActivityThread::ResolveActivityInfo(
    /* [in] */ IIntent* intent,
    /* [out] */ IActivityInfo** info)
{
    VALIDATE_NOT_NULL(info);

    AutoPtr<IPackageManager> pManager;
    mInitialApplication->GetPackageManager((IPackageManager**)&pManager);
    AutoPtr<IActivityInfo> aInfo;
    intent->ResolveActivityInfo(
            pManager, IPackageManager::GET_SHARED_LIBRARY_FILES,
            (IActivityInfo**)&aInfo);
    if (aInfo == NULL) {
        // Throw an exception.
        CInstrumentation::CheckStartActivityResult(
                IActivityManager::START_CLASS_NOT_FOUND, intent);
    }
    *info = aInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode CActivityThread::StartActivityNow(
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* activityInfo,
    /* [in] */ IBinder* token,
    /* [in] */ IBundle* state,
    /* [in] */ IActivityNonConfigurationInstances* lastNonConfigurationInstances,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);

    AutoPtr<ActivityClientRecord> r = new ActivityClientRecord();
    r->mToken = token;
    r->mIdent = 0;
    r->mIntent = intent;
    r->mState = (CBundle*)state;
    r->mParent = parent;
    r->mEmbeddedID = id;
    r->mActivityInfo = (CActivityInfo*)activityInfo;
    r->mLastNonConfigurationInstances = lastNonConfigurationInstances;
    if (localLOGV) {
        AutoPtr<IComponentName> compname;
        intent->GetComponent((IComponentName**)&compname);
        String name;
        if (compname != NULL) {
            compname->ToShortString(&name);
        } else {
            StringBuilder sb("(Intent ");
            sb += (IInterface*)intent;
            sb += ").getComponent() returned null";
            name = sb.ToString();
        }
        String action;
        intent->GetAction(&action);
        Slogger::V(TAG, "Performing launch: action=%s, comp=%s, token=%p",
                action.string(), name.string(), token);
    }
    return PerformLaunchActivity(r, NULL, activity);
}

ECode CActivityThread::GetActivity(
    /* [in] */ IBinder* token,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);
    *activity = NULL;
    VALIDATE_NOT_NULL(token);

    AutoPtr<ActivityClientRecord> clientRecord = GetActivityClientRecord(token);
    if (clientRecord != NULL) {
        *activity = clientRecord->mActivity;
        REFCOUNT_ADD(*activity);
    }
    return NOERROR;
}

ECode CActivityThread::SendActivityResult(
    /* [in] */ IBinder* token,
    /* [in] */ const String& id,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    VALIDATE_NOT_NULL(token);
    VALIDATE_NOT_NULL(data);

    if (DEBUG_RESULTS) {
        Slogger::V(TAG, "sendActivityResult: id=%s req=%d res=%d data=%p",
                id.string(), requestCode, resultCode, data);
    }
    AutoPtr<IObjectContainer> list;
    CParcelableObjectContainer::New((IObjectContainer**)&list);
    AutoPtr<CResultInfo> info;
    CResultInfo::NewByFriend(id, requestCode, resultCode, data, (CResultInfo**)&info);
    list->Add((IParcelable*)(CResultInfo*)info);
    return mAppThread->ScheduleSendResult(token, list);
}

ECode CActivityThread::QueueOrSendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    return QueueOrSendMessage(what, obj, 0, 0);
}

ECode CActivityThread::QueueOrSendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 arg1)
{
    return QueueOrSendMessage(what, obj, arg1, 0);
}

ECode CActivityThread::QueueOrSendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    AutoLock lock(mLock);

    if (DEBUG_MESSAGES) {
        Slogger::V(TAG, "QueueOrSendMessage %d %s: arg1=%d, arg2=%d, obj=%p",
            what, H::CodeToString(what).string(), arg1, arg2, obj);
    }

    AutoPtr<IMessage> msg;
    mH->ObtainMessage(what, (IMessage**)&msg);
    msg->SetArg1(arg1);
    msg->SetArg2(arg2);
    msg->SetObj(obj);

    Boolean result;
    return mH->SendMessage(msg, &result);
}

ECode CActivityThread::ScheduleContextCleanup(
    /* [in] */ IContext* context,
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    VALIDATE_NOT_NULL(context);

    AutoPtr<ContextCleanupInfo> cci = new ContextCleanupInfo();
    cci->mContext = (IContextImpl*)context;
    cci->mWho = who;
    cci->mWhat = what;

    return QueueOrSendMessage(H::CLEAN_UP_CONTEXT, cci);
}

ECode CActivityThread::PerformLaunchActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ IIntent* customIntent,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);
    *activity = NULL;

    ECode ec = NOERROR;
    AutoPtr<IActivityInfo> aInfo = r->mActivityInfo;
    if (r->mPackageInfo == NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        r->mPackageInfo = GetPackageInfo(
                appInfo, r->mCompatInfo, IContext::CONTEXT_INCLUDE_CODE);
    }

    AutoPtr<IComponentName> component;
    r->mIntent->GetComponent((IComponentName**)&component);
    if (component == NULL) {
        AutoPtr<IPackageManager> pManager;
        mInitialApplication->GetPackageManager((IPackageManager**)&pManager);
        r->mIntent->ResolveActivity(pManager, (IComponentName**)&component);
        r->mIntent->SetComponent(component);
    }

    String targetActivity;
    r->mActivityInfo->GetTargetActivity(&targetActivity);
    if (!targetActivity.IsNull()) {
        component = NULL;
        String pkgName;
        r->mActivityInfo->GetPackageName(&pkgName);
        CComponentName::New(pkgName, targetActivity, (IComponentName**)&component);
    }

    String packageName;
    component->GetPackageName(&packageName);
    String className;
    component->GetClassName(&className);
    assert(!packageName.IsNull());
    assert(!className.IsNull());

    // for epk
    String appDir;
    r->mPackageInfo->GetAppDir(&appDir);
    StringBuilder sb;
    if (appDir.EndWith(".epk")) {
        sb.Append("/data/elastos/");
        sb.Append(packageName);
        sb.Append(".eco");
    }
    else {
        sb.Append("/data/data/com.elastos.runtime/elastos/");
        sb.Append(packageName);
        sb.Append("/");
        sb.Append(packageName);
        sb.Append(".eco");
    }
    String path = sb.ToString();
    AutoPtr<IModuleInfo> moduleInfo;
    ec = CReflector::AcquireModuleInfo(path.string(), (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        if (localLOGV) {
           Slogger::E(TAG, "PerformLaunchActivity: Cann't Find the path is %s", path.string());
        }
        return E_RUNTIME_EXCEPTION;
    }
    Int32 index = className.LastIndexOf('.');
    String shortClassName = index > 0 ? className.Substring(index + 1) : className;
    AutoPtr<IClassInfo> classInfo;
    ec = moduleInfo->GetClassInfo(shortClassName, (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        if (localLOGV) {
           Slogger::E(TAG, "PerformLaunchActivity: Get class info of %s failed.", shortClassName.string());
        }
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IInterface> object;
    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        if (localLOGV) {
           Slogger::E(TAG, "PerformLaunchActivity: Create activity object failed.");
        }
        return E_RUNTIME_EXCEPTION;
    }
    AutoPtr<IActivity> a;
    a = (IActivity*)object->Probe(EIID_IActivity);

//    try {
    AutoPtr<IApplication> app;
    r->mPackageInfo->MakeApplication(FALSE, mInstrumentation, (IApplication**)&app);

    if (localLOGV) Slogger::V(TAG, "Performing launch of %p", r);
    if (localLOGV) {
        String appName;
        app->GetPackageName(&appName);
        String pkg;
        r->mPackageInfo->GetPackageName(&pkg);
        AutoPtr<IComponentName> component;
        r->mIntent->GetComponent((IComponentName**)&component);
        String comp;
        component->ToShortString(&comp);
        String dir;
        r->mPackageInfo->GetAppDir(&dir);
        Slogger::V(
            TAG, "%p: app=%p, appName=%s, pkg=%s, comp=%s, dir=%s",
            r, app.Get(), appName.string(), pkg.string(), comp.string(), dir.string());
    }

    if (a != NULL) {
        AutoPtr<IContext> appContext = CreateBaseContextForActivity(r, a);
        AutoPtr<ICharSequence> title;
        AutoPtr<IPackageManager> pManager;
        appContext->GetPackageManager((IPackageManager**)&pManager);
        r->mActivityInfo->LoadLabel(pManager, (ICharSequence**)&title);
        AutoPtr<IConfiguration> config;
        CConfiguration::New(mCompatConfiguration, (IConfiguration**)&config);
//       if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Launching activity %s with config %p"
//               , r->mActivityInfo->mName.string(), config.Get());
//        activity.attach(appContext, this, getInstrumentation(), r.token,
//                r.ident, app, r.intent, r.activityInfo, title, r.parent,
//                r.embeddedID, r.lastNonConfigurationInstance,
//                r.lastNonConfigurationChildInstances, config);

        a->Attach(appContext,
            this, mInstrumentation, r->mToken, r->mIdent, app, r->mIntent,
            r->mActivityInfo, title, r->mParent, r->mEmbeddedID,
            r->mLastNonConfigurationInstances, config);

        if (customIntent != NULL) {
            a->SetIntent(customIntent);
        }

        r->mLastNonConfigurationInstances = NULL;
        a->SetStartedActivity(FALSE);
        Int32 theme = 0;
        r->mActivityInfo->GetThemeResource(&theme);
        if (theme != 0) {
            a->SetTheme(theme);
        }

        String debugOnKey("debug.on");
        String defaultValue("0");
        String debugOnValue;
        AutoPtr<ISystemProperties> properties;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&properties);
        properties->Get(debugOnKey, String("0"), &debugOnValue);
        if (debugOnValue.Equals("1")) {
            Slogger::D(TAG, "Waiting for attaching...");
            String debugAttachedKey("debug.attached");
            String debugAttachedValue;
            properties->Get(debugAttachedKey, defaultValue, &debugAttachedValue);
            while (debugAttachedValue.Equals("0")) {
                Thread::Sleep(500);
                properties->Get(debugAttachedKey, defaultValue, &debugAttachedValue);
            }
        }

        a->SetCalled(FALSE);
        mInstrumentation->CallActivityOnCreate(a, r->mState);

        Boolean called;
        a->IsCalled(&called);
        if (!called) {
            String activityName;
            AutoPtr<IComponentName> cn;
            r->mIntent->GetComponent((IComponentName**)&cn);
            cn->ToShortString(&activityName);
            Slogger::E(TAG, "Activity %s did not call through to super.onCreate().", activityName.string());
            return E_SUPER_NOT_CALLED_EXCEPTION;
//            throw new SuperNotCalledException(
//                "Activity " + r.intent.getComponent().toShortString() +
//                " did not call through to super.onCreate()");
        }
        r->mActivity = a;
        r->mStopped = TRUE;
        Boolean finished;
        if (r->mActivity->IsFinishing(&finished), !finished) {
            a->PerformStart();
            r->mStopped = FALSE;
        }
        r->mActivity->IsFinishing(&finished);
        if (!finished) {
            if (r->mState != NULL) {
                mInstrumentation->CallActivityOnRestoreInstanceState(a, r->mState);
            }
        }

        r->mActivity->IsFinishing(&finished);
        if (!finished) {
            a->SetCalled(FALSE);
            mInstrumentation->CallActivityOnPostCreate(a, r->mState);
            a->IsCalled(&called);
           if (!called) {
                String activityName;
                AutoPtr<IComponentName> cn;
                r->mIntent->GetComponent((IComponentName**)&cn);
                cn->ToShortString(&activityName);
                Slogger::E(TAG, "Activity %s did not call through to super.onPostCreate().", activityName.string());
                return E_SUPER_NOT_CALLED_EXCEPTION;
//                throw new SuperNotCalledException(
//                    "Activity " + r.intent.getComponent().toShortString() +
//                    " did not call through to super.onPostCreate()");
           }
        }
    }
    r->mPaused = TRUE;
    mActivities[r->mToken] = r;

//    } catch (SuperNotCalledException e) {
//        throw e;
//
//    } catch (Exception e) {
//        if (!mInstrumentation.onException(activity, e)) {
//            throw new RuntimeException(
//                "Unable to start activity " + component
//                + ": " + e.toString(), e);
//        }
//    }

    *activity = a;
    REFCOUNT_ADD(*activity);
    return NOERROR;
}

AutoPtr<IContext> CActivityThread::CreateBaseContextForActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ IActivity* activity)
{
    AutoPtr<CContextImpl> appContext;
    CContextImpl::NewByFriend((CContextImpl**)&appContext);
    appContext->Init(r->mPackageInfo, r->mToken, this);
    appContext->SetOuterContext(activity);

    // For debugging purposes, if the activity's package name contains the value of
    // the "debug.use-second-display" system property as a substring, then show
    // its content on a secondary display if there is one.
    AutoPtr<IContext> baseContext = (IContext*)appContext.Get();
//     String pkgName = SystemProperties.get("debug.second-display.pkg");
//     if (pkgName != null && !pkgName.isEmpty()
//             && r.packageInfo.mPackageName.contains(pkgName)) {
//         DisplayManagerGlobal dm = DisplayManagerGlobal.getInstance();
//         for (int displayId : dm.getDisplayIds()) {
//             if (displayId != Display.DEFAULT_DISPLAY) {
//                 Display display = dm.getRealDisplay(displayId);
//                 baseContext = appContext.createDisplayContext(display);
//                 break;
//             }
//         }
//     }
    return baseContext;
}

ECode CActivityThread::HandleLaunchActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ IIntent* customIntent)
{
    assert(r);
    AutoPtr<IApplicationInfo> appInfo;
    r->mActivityInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    r->mPackageInfo = GetPackageInfoNoCheck(
        (CApplicationInfo*)appInfo.Get(), r->mCompatInfo);

    // If we are getting ready to gc after going to the background, well
    // we are back active so skip it.
    UnscheduleGcIdler();

    if (r->mProfileFd != NULL) {
        mProfiler->SetProfiler(r->mProfileFile, r->mProfileFd);
        mProfiler->StartProfiling();
        mProfiler->mAutoStopProfiler = r->mAutoStopProfiler;
    }

    // Make sure we are running with the most recent config.
    HandleConfigurationChanged(NULL, NULL);

    if (localLOGV) {
        String acrDes;
        r->ToString(&acrDes);
        Slogger::V(TAG, "Handling launch of %s", acrDes.string());
    }
    AutoPtr<IActivity> a;
    PerformLaunchActivity(r, customIntent, (IActivity**)&a);

    if (a != NULL) {
        r->mCreatedConfig = NULL;
        CConfiguration::New(mConfiguration, (IConfiguration**)&r->mCreatedConfig);
        AutoPtr<CBundle> oldState = r->mState;
        if (localLOGV) {
            Slogger::V(TAG, "Handling resume of activity.");
        }

        Boolean finished;
        r->mActivity->IsFinishing(&finished);
        HandleResumeActivity(r->mToken, FALSE, r->mIsForward,
            !finished && !r->mStartsNotResumed);

        if ((!finished) && r->mStartsNotResumed) {
            // The activity manager actually wants this one to start out
            // paused, because it needs to be visible but isn't in the
            // foreground.  We accomplish this by going through the
            // normal startup (because activities expect to go through
            // onResume() the first time they run, before their window
            // is displayed), and then pausing it.  However, in this case
            // we do -not- need to do the full pause cycle (of freezing
            // and such) because the activity manager assumes it can just
            // retain the current state it has.
            r->mActivity->SetCalled(FALSE);
            mInstrumentation->CallActivityOnPause(r->mActivity);
            // We need to keep around the original state, in case
            // we need to be created again.  But we only do this
            // for pre-Honeycomb apps, which always save their state
            // when pausing, so we can not have them save their state
            // when restarting from a paused state.  For HC and later,
            // we want to (and can) let the state be saved as the normal
            // part of stopping the activity.
            if (r->IsPreHoneycomb()) {
                r->mState = oldState;
            }
            Boolean called;
            r->mActivity->IsCalled(&called);
            if (!called) {
                String activityName;
                AutoPtr<IComponentName> cn;
                r->mActivity->GetComponentName((IComponentName**)&cn);
                cn->ToShortString(&activityName);
                Slogger::E(TAG, "Activity %s did not call through to super.onPause().", activityName.string());
                return E_SUPER_NOT_CALLED_EXCEPTION;
//                throw new SuperNotCalledException(
//                    "Activity " + r.intent.getComponent().toShortString() +
//                    " did not call through to super.onPause()");
            }

            r->mPaused = TRUE;
        }
    }
    else {
        Slogger::E(TAG, "Handling resume of activity there was an error.");
        // If there was an error, for any reason, tell the activity
        // manager to stop us.
//        try {
        AutoPtr<IIActivityManager> service = ActivityManagerNative::GetDefault();
        Boolean isFinish;
        service->FinishActivity(r->mToken, IActivity::RESULT_CANCELED, NULL, &isFinish);
//        } catch (RemoteException ex) {
//        }
    }

    return NOERROR;
}

void CActivityThread::DeliverNewIntents(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ List< AutoPtr<IIntent> >* intents)
{
    List< AutoPtr<IIntent> >::Iterator it;
    for (it = intents->Begin(); it != intents->End(); ++it) {
        AutoPtr<IIntent> intent = *it;
        AutoPtr<IClassLoader> classLoader;
        r->mActivity->GetClassLoader((IClassLoader**)&classLoader);
        // TODO intent->SetExtrasClassLoader(classLoader);

        AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
        if (activity) activity->mFragments->NoteStateNotSaved();
        mInstrumentation->CallActivityOnNewIntent(r->mActivity, intent);
    }
}

CARAPI CActivityThread::PerformNewIntents(
    /* [in] */ IBinder* token,
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    if (r != NULL) {
        AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
        Boolean resumed = !r->mPaused;
        if (resumed) {
            if (activity) activity->mTemporaryPause = TRUE;
            mInstrumentation->CallActivityOnPause(r->mActivity);
        }
        List< AutoPtr<IIntent> > lIntents;
        for (Int32 i = 0; i < intents->GetLength(); ++i) {
            lIntents.PushBack((*intents)[i]);
        }
        DeliverNewIntents(r, &lIntents);
        if (resumed) {
            r->mActivity->PerformResume();
            if (activity) activity->mTemporaryPause = FALSE;
        }
    }
    return NOERROR;
}

ECode CActivityThread::HandleNewIntent(
    /* [in] */ NewIntentData* data)
{
    assert(data != NULL);
    Int32 size = data->mIntents.GetSize();
    AutoPtr< ArrayOf<IIntent*> > aIntents = ArrayOf<IIntent*>::Alloc(size);
    List< AutoPtr<IIntent> >::Iterator it = data->mIntents.Begin();
    Int32 i = 0;
    for (; it != data->mIntents.End(); ++it, ++i) {
        aIntents->Set(i, *it);
    }
    PerformNewIntents(data->mToken, aIntents);

    return NOERROR;
}

ECode CActivityThread::HandleReceiver(
    /* [in] */ ReceiverData* data)
{
    // If we are getting ready to gc after going to the background, well
    // we are back active so skip it.
    UnscheduleGcIdler();

    AutoPtr<IComponentName> component;
    String packageName, className;

    AutoPtr<IIntent> intent = data->mIntent;
    data->mIntent->GetComponent((IComponentName**)&component);
    component->GetPackageName(&packageName);
    component->GetClassName(&className);

    AutoPtr<IApplicationInfo> appInfo;
    data->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    AutoPtr<LoadedPkg> packageInfo = GetPackageInfoNoCheck(appInfo, data->mCompatInfo);

    AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();

    AutoPtr<IBroadcastReceiver> receiver;
    String path, shortClassName;
    //TODO: com.android.server.BootReceiver is declared in android system manifest xml file.
    // the package declared in the same xml file is "android"
    // so maybe we should change the package name in that xml file
    if (className.Equals("com.android.server.BootReceiver")
        || className.Equals("com.android.server.MasterClearReceiver")
    ) {
        path = "/system/lib/Elastos.Droid.Server.eco";
        Int32 lastIndex = className.LastIndexOf(".");
        if (lastIndex != -1) {
            String temp(className);
            className = String("C");
            className.AppendFormat(temp.Substring(lastIndex + 1));
        }
        shortClassName = className;
    }
    else {
        StringBuilder sb;
        sb.Append("/data/elastos/");
        sb.Append(packageName);
        sb.Append(".eco");
        path = sb.ToString();

        Int32 index = className.LastIndexOf('.');
        shortClassName = index > 0 ? className.Substring(index + 1) : className;

        Slogger::D(TAG, "HandleReceiver: load object from pakcage: %s, class: %s",
            packageName.string(), className.string());
    }

//    try {
    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    ECode ec = CReflector::AcquireModuleInfo(path, (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        Slogger::E(TAG, "HandleReceiver: Cann't Find the path is %s", path.string());
        return ec;
    }

    ec = moduleInfo->GetClassInfo(shortClassName, (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        Slogger::E(TAG, "HandleReceiver: Get class info of %s failed.", shortClassName.string());
        return ec;
    }

    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        Slogger::E(TAG, "HandleReceiver: Create activity object %s failed.", shortClassName.string());
        return ec;
    }

    receiver = IBroadcastReceiver::Probe(object);
//    } catch (Exception e) {
//         if (DEBUG_BROADCAST) Slog.i(TAG,
//                 "Finishing failed broadcast to " + data.intent.getComponent());
//         data.sendFinished(mgr);
//         throw new RuntimeException(
//             "Unable to instantiate receiver " + component
//             + ": " + e.toString(), e);
//    }

//        try {
    AutoPtr<IApplication> app;
    packageInfo->MakeApplication(FALSE, mInstrumentation, (IApplication**)&app);

    if (localLOGV) {
        String appName;
        app->GetPackageName(&appName);
        String pkg;
        packageInfo->GetPackageName(&pkg);

        String comp;
        component->ToShortString(&comp);
        String dir;
        packageInfo->GetAppDir(&dir);
        Slogger::V(TAG, "Performing receive of %p: app=%p, appName=%s, pkg=%s, comp=%s, dir=%s",
            data->mIntent.Get(), app.Get(), appName.string(), pkg.string(),
            comp.string(), dir.string());
    }

    AutoPtr<IContextImpl> context;
    app->GetBaseContext((IContext**)&context);

    pthread_setspecific(sCurrentBroadcastIntentKey, data->mIntent.Get());
    REFCOUNT_ADD(data->mIntent);

    receiver->SetPendingResult((IPendingResult*)data);
    AutoPtr<IContext> ic;
    ((CContextImpl*)context.Get())->GetReceiverRestrictedContext((IContext**)&ic);
    ec = receiver->OnReceive(ic, data->mIntent);
    if (FAILED(ec)) {
        String comp;
        component->ToShortString(&comp);

        if (DEBUG_BROADCAST) {
            Slogger::I(TAG, "Finishing failed broadcast to %s", comp.string());
        }

        data->SendFinished(mgr);
        ec = E_RUNTIME_EXCEPTION;
        Slogger::E(TAG, "Unable to start receiver %s", comp.string());
//        } catch (Exception e) {
//             if (DEBUG_BROADCAST) Slog.i(TAG,
//                     "Finishing failed broadcast to " + data.intent.getComponent());
//             data.sendFinished(mgr);
//             if (!mInstrumentation.onException(receiver, e)) {
//                 throw new RuntimeException(
//                     "Unable to start receiver " + component
//                     + ": " + e.toString(), e);
//             }
//        }
    }

    IIntent* curIntent = (IIntent*)pthread_getspecific(sCurrentBroadcastIntentKey);
    REFCOUNT_RELEASE(curIntent);
    pthread_setspecific(sCurrentBroadcastIntentKey, NULL);

    if (SUCCEEDED(ec)) {
        AutoPtr<IPendingResult> result;
        receiver->GetPendingResult((IPendingResult**)&result);
        if (result != NULL) {
            data->Finish();
        }
    }

    return ec;
}

// Instantiate a BackupAgent and tell it that it's alive
ECode CActivityThread::HandleCreateBackupAgent(
    /* [in] */ CreateBackupAgentData* data)
{
    assert(data != NULL);
    if (DEBUG_BACKUP) Slogger::V(TAG, "handleCreateBackupAgent: %p", data);

    // Sanity check the requested target package's uid against ours
//     try {
    String pkgName;
    data->mAppInfo->GetPackageName(&pkgName);
    AutoPtr<IPackageInfo> requestedPackage;
    GetPackageManager()->GetPackageInfo(pkgName, 0, UserHandle::GetMyUserId(),
            (IPackageInfo**)&requestedPackage);
    AutoPtr<IApplicationInfo> appInfo;
    requestedPackage->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    if (uid != Process::MyUid()) {
        Slogger::W(TAG, "Asked to instantiate non-matching package %s", pkgName.string());
        return NOERROR;
    }
//     } catch (RemoteException e) {
//         Slog.e(TAG, "Can't reach package manager", e);
//         return;
//     }

    // no longer idle; we have backup work to do
    UnscheduleGcIdler();

    // instantiate the BackupAgent class named in the manifest
    AutoPtr<LoadedPkg> packageInfo = GetPackageInfoNoCheck(data->mAppInfo, data->mCompatInfo);
    String packageName = packageInfo->mPackageName;
    if (packageName.IsNull()) {
        Slogger::D(TAG, "Asked to create backup agent for nonexistent package");
        return NOERROR;
    }

    if (mBackupAgents[packageName] != NULL) {
        Slogger::D(TAG, "BackupAgent   for %s already exists", packageName.string());
        return NOERROR;
    }

    AutoPtr<IBackupAgent> agent;
    String classname;
    data->mAppInfo->GetBackupAgentName(&classname);

    // full backup operation but no app-supplied agent?  use the default implementation
    if (classname == NULL && (data->mBackupMode == IApplicationThread::BACKUP_MODE_FULL
            || data->mBackupMode == IApplicationThread::BACKUP_MODE_RESTORE_FULL)) {
        classname = "Elastos.Droid.App.Backup.FullBackupAgent";
    }

//     try {
    AutoPtr<IBinder> binder = NULL;
//         try {
    String appDir;
    packageInfo->GetAppDir(&appDir);
    if (DEBUG_BACKUP) Slogger::V(TAG, "Initializing agent class[%s], pkgName[%s], appDir[%s]", classname.string(), pkgName.string(), appDir.string());

    StringBuilder sb;
    if (appDir.EndWith(".epk")) {
        sb.Append("/data/elastos/");
        sb.Append(pkgName);
        sb.Append(".eco");
    }
    else if(appDir.EndWith("framework-res.apk")) {
        sb.Append("/system/lib/Elastos.Droid.Server.eco");
    }
    else {
        sb.Append("/data/data/com.elastos.runtime/elastos/");
        sb.Append(pkgName);
        sb.Append("/");
        sb.Append(pkgName);
        sb.Append(".eco");
    }
    String path = sb.ToString();

    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    ECode ec = CReflector::AcquireModuleInfo(path.string(), (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        Slogger::E(TAG, "CreateBackupAgent: Can't find the path is %s", path.string());
        return E_RUNTIME_EXCEPTION;
    }
    Int32 index = classname.LastIndexOf('.');
    String shortClassName = index > 0 ? classname.Substring(index + 1) : classname;
    ec = moduleInfo->GetClassInfo(shortClassName, (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        Slogger::E(TAG, "CreateBackupAgent: Get class info of %s failed.", classname.string());
        return E_RUNTIME_EXCEPTION;
    }
    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        Slogger::E(TAG, "CreateBackupAgent: Create BackupAgent object failed.");
        return E_RUNTIME_EXCEPTION;
    }
    agent = (IBackupAgent*)object->Probe(EIID_IBackupAgent);
    if (agent == NULL) {
        Slogger::E(TAG, "CreateBackupAgent: Probe error.");
        return E_RUNTIME_EXCEPTION;
    }

    // set up the agent's context
    AutoPtr<CContextImpl> context;
    CContextImpl::NewByFriend((CContextImpl**)&context);
    context->Init(packageInfo, NULL, this);
    context->SetOuterContext(agent);
    agent->Attach(context);

    agent->OnCreate();
    agent->OnBind((IBinder**)&binder);
    mBackupAgents[packageName] = agent;

//         } catch (Exception e) {
//             // If this is during restore, fail silently; otherwise go
//             // ahead and let the user see the crash.
//             Slog.e(TAG, "Agent threw during creation: " + e);
//             if (data.backupMode != IApplicationThread.BACKUP_MODE_RESTORE
//                     && data.backupMode != IApplicationThread.BACKUP_MODE_RESTORE_FULL) {
//                 throw e;
//             }
//             // falling through with 'binder' still null
//         }

    // tell the OS that we're live now
//         try {
    ActivityManagerNative::GetDefault()->BackupAgentCreated(packageName, binder);
//         } catch (RemoteException e) {
//             // nothing to do.
//         }
//     } catch (Exception e) {
//         throw new RuntimeException("Unable to create BackupAgent "
//                 + classname + ": " + e.toString(), e);
//     }
    return NOERROR;
}

    // Tear down a BackupAgent
ECode CActivityThread::HandleDestroyBackupAgent(
    /* [in] */ CreateBackupAgentData* data)
{
    assert(data != NULL);
    if (DEBUG_BACKUP) Slogger::V(TAG, "handleDestroyBackupAgent: %p", data);

    LoadedPkg* packageInfo = GetPackageInfoNoCheck(data->mAppInfo, data->mCompatInfo);
    String packageName = packageInfo->mPackageName;
    AutoPtr<IBackupAgent> agent = mBackupAgents[packageName];
    if (agent != NULL) {
//         try {
        agent->OnDestroy();
//         } catch (Exception e) {
//             Slog.w(TAG, "Exception thrown in onDestroy by backup agent of " + data.appInfo);
//             e.printStackTrace();
//         }
        mBackupAgents.Erase(packageName);
    } else {
        Slogger::W(TAG, "Attempt to destroy unknown backup agent %p", data);
    }
    return NOERROR;
}

ECode CActivityThread::HandleCreateService(
    /* [in] */ CreateServiceData* data)
{
    assert(data != NULL);
    // If we are getting ready to gc after going to the background, well
    // we are back active so skip it.
    UnscheduleGcIdler();

    AutoPtr<IApplicationInfo> appInfo;
    data->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    AutoPtr<LoadedPkg> packageInfo = GetPackageInfoNoCheck(appInfo, data->mCompatInfo);

    String pkgName;
    data->mInfo->GetPackageName(&pkgName);
    String appDir;
    packageInfo->GetAppDir(&appDir);
    StringBuilder sb;
    // for epk
    if (appDir.EndWith(".epk")) {
        sb.Append("/data/elastos/");
        sb.Append(pkgName);
        sb.Append(".eco");
    }
    else {
        sb.Append("/data/data/com.elastos.runtime/elastos/");
        sb.Append(pkgName);
        sb.Append("/");
        sb.Append(pkgName);
        sb.Append(".eco");
    }

    String path = sb.ToString();
    AutoPtr<IModuleInfo> moduleInfo;
    ECode ec = CReflector::AcquireModuleInfo(path.string(), (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        if (localLOGV) {
           Slogger::E(TAG, "HandleCreateService: Cann't Find the path is %s", path.string());
        }
        return E_RUNTIME_EXCEPTION;
    }
    String className;
    data->mInfo->GetName(&className);
    Int32 index = className.LastIndexOf('.');
    String shortClassName = index > 0 ? className.Substring(index + 1) : className;
    AutoPtr<IClassInfo> classInfo;
    ec = moduleInfo->GetClassInfo(shortClassName, (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        if (localLOGV) {
           Slogger::E(TAG, "HandleCreateService: Get class info of %s failed.", shortClassName.string());
        }
        return E_RUNTIME_EXCEPTION;
    }
    AutoPtr<IInterface> object;
    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        if (localLOGV) {
           Slogger::E(TAG, "HandleCreateService: Create activity object failed.");
        }
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IService> service;
    service = (IService*)object->Probe(EIID_IService);

//    try {
    if (localLOGV) {
        Slogger::V(TAG, "Creating service %s, path %s", className.string(), path.string());
    }

    AutoPtr<CContextImpl> appContext;
    CContextImpl::NewByFriend((CContextImpl**)&appContext);
    appContext->Init(packageInfo, NULL, this);
    AutoPtr<IApplication> app;
    packageInfo->MakeApplication(FALSE, mInstrumentation, (IApplication**)&app);
    appContext->SetOuterContext((IContext*)service.Get());
    AutoPtr<IIActivityManager> activityManager = ActivityManagerNative::GetDefault();
    service->Attach((IContext*)(IContextImpl*)appContext.Get(), this, className, data->mToken, app,
            activityManager);
    service->OnCreate();
    mServices[data->mToken] = service;
//    try {
    activityManager->ServiceDoneExecuting(data->mToken, 0, 0, 0);
//    } catch (RemoteException e) {
//        // nothing to do.
//    }
//    } catch (Exception e) {
//        if (!mInstrumentation.onException(service, e)) {
//            throw new RuntimeException(
//                "Unable to create service " + data.info.name
//                + ": " + e.toString(), e);
//        }
//    }
    return NOERROR;
}

ECode CActivityThread::HandleBindService(
    /* [in] */ BindServiceData* data)
{
    assert(data != NULL);
    AutoPtr<IService> s;
    HashMap<AutoPtr<IBinder>, AutoPtr<IService> >::Iterator it =
            mServices.Find(data->mToken);
    if (it != mServices.End()) {
        s = it->mSecond;
    }
    if (DEBUG_SERVICE) {
        Slogger::V(TAG, "handleBindService s=%p rebind%d", s.Get(),  data->mRebind);
    }
    if (s != NULL) {
//        try {
        AutoPtr<IClassLoader> classLoader;
//        s->GetClassLoader((IClassLoader**)&classLoader);
//        data->mIntent->SetExtrasClassLoader(classLoader);
//        try {
        AutoPtr<IIActivityManager> activityManager = ActivityManagerNative::GetDefault();
        if (!data->mRebind) {
            AutoPtr<IBinder> binder;
            s->OnBind(data->mIntent, (IBinder**)&binder);
            activityManager->PublishService(
                    data->mToken, data->mIntent, binder);
        }
        else {
            s->OnRebind(data->mIntent);
            activityManager->ServiceDoneExecuting(
                    data->mToken, 0, 0, 0);
        }
        EnsureJitEnabled();
//        } catch (RemoteException ex) {
//        }
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to bind to service " + s
//                        + " with " + data.intent + ": " + e.toString(), e);
//            }
//        }
    }
    return NOERROR;
}

ECode CActivityThread::HandleUnbindService(
    /* [in] */ BindServiceData* data)
{
    assert(data != NULL);
    AutoPtr<IService> s;
    HashMap<AutoPtr<IBinder>, AutoPtr<IService> >::Iterator it =
            mServices.Find(data->mToken);
    if (it != mServices.End()) {
        s = it->mSecond;
    }
    if (s != NULL) {
//        try {
        AutoPtr<IClassLoader> classLoader;
//        s->GetClassLoader((IClassLoader**)&classLoader);
//        data->mIntent->SetExtrasClassLoader(classLoader);
//            try {
        AutoPtr<IIActivityManager> activityManager = ActivityManagerNative::GetDefault();
        Boolean doRebind;
        s->OnUnbind(data->mIntent, &doRebind);
        if (doRebind) {
            activityManager->UnbindFinished(data->mToken, data->mIntent, doRebind);
        }
        else {
            activityManager->ServiceDoneExecuting(data->mToken, 0, 0, 0);
        }
        // } catch (Exception e) {
        //     if (!mInstrumentation.onException(s, e)) {
        //         throw new RuntimeException(
        //                 "Unable to unbind to service " + s
        //                 + " with " + data.intent + ": " + e.toString(), e);
        //     }
        // }
    }
    return NOERROR;
}

ECode CActivityThread::HandleDumpService(
    /* [in] */ DumpComponentInfo* info)
{
    assert(info != NULL);
//     final StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskWrites();
//     try {
    AutoPtr<IService> s = mServices[info->mToken];
    if (s != NULL) {
//        PrintWriter pw = new PrintWriter(new FileOutputStream(info.fd.getFileDescriptor()));
//        s.dump(info.fd.getFileDescriptor(), pw, info.args);
//        pw.flush();
    }
//     } finally {
//    IoUtils::CloseQuietly(info->mFd);
//    StrictMode.setThreadPolicy(oldPolicy);
//     }
    return NOERROR;
}

ECode CActivityThread::HandleDumpActivity(
    /* [in] */ DumpComponentInfo* info)
{
    assert(info != NULL);
//     final StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskWrites();
//     try {
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(info->mToken);
    if (r != NULL && r->mActivity != NULL) {
//        PrintWriter pw = new PrintWriter(new FileOutputStream(info.fd.getFileDescriptor()));
//        r.activity.dump(info.prefix, info.fd.getFileDescriptor(), pw, info.args);
//        pw.flush();
    }
//     } finally {
//    IoUtils.closeQuietly(info.fd);
//    StrictMode.setThreadPolicy(oldPolicy);
//     }
    return NOERROR;
}

ECode CActivityThread::HandleDumpProvider(
    /* [in] */ DumpComponentInfo* info)
{
    assert(info != NULL);
//     final StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskWrites();
//     try {
    ProviderClientRecord* r = mLocalProviders[info->mToken];
    if (r != NULL && r->mLocalProvider != NULL) {
//        PrintWriter pw = new PrintWriter(new FileOutputStream(info.fd.getFileDescriptor()));
//        r.mLocalProvider.dump(info.fd.getFileDescriptor(), pw, info.args);
//        pw.flush();
    }
//     } finally {
//    IoUtils.closeQuietly(info.fd);
//    StrictMode.setThreadPolicy(oldPolicy);
//     }
    return NOERROR;
}

ECode CActivityThread::HandleServiceArgs(
    /* [in] */ ServiceArgsData* data)
{
    assert(data != NULL);
    AutoPtr<IService> s;
    HashMap<AutoPtr<IBinder>, AutoPtr<IService> >::Iterator it =
            mServices.Find(data->mToken);
    if (it != mServices.End()) {
        s = it->mSecond;
    }
    if (s != NULL) {
//        try {
        if (data->mArgs != NULL) {
            AutoPtr<IClassLoader> classLoader;
//            s->GetClassLoader((IClassLoader**)&classLoader);
//            data->mArgs->SetExtrasClassLoader(classLoader);
        }
        Int32 res;
        if (!data->mTaskRemoved) {
            s->OnStartCommand(data->mArgs, data->mFlags, data->mStartId, &res);
        }
        else {
            s->OnTaskRemoved(data->mArgs);
            res = IService::START_TASK_REMOVED_COMPLETE;
        }
        QueuedWork::WaitToFinish();
//        try {
        AutoPtr<IIActivityManager> activityManager = ActivityManagerNative::GetDefault();
        activityManager->ServiceDoneExecuting(data->mToken, 1, data->mStartId, res);
//        } catch (RemoteException e) {
//            // nothing to do.
//        }
        EnsureJitEnabled();
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to start service " + s
//                        + " with " + data.args + ": " + e.toString(), e);
//            }
//        }
    }
    return NOERROR;
}

ECode CActivityThread::HandleStopService(
    /* [in] */ IBinder* token)
{
    AutoPtr<IService> s;
    HashMap<AutoPtr<IBinder>, AutoPtr<IService> >::Iterator it =
            mServices.Find(token);
    if (it != mServices.End()) {
        s = it->mSecond;
    }
    if (s != NULL) {
//        try {
       if (localLOGV) Slogger::V(TAG, "Destroying service %p", s.Get());
        s->OnDestroy();
        AutoPtr<IContext> context;
        s->GetBaseContext((IContext**)&context);

        AutoPtr<IContextImpl> con = IContextImpl::Probe(context.Get());
        if (con != NULL) {
            String who;
            s->GetClassName(&who);
           ((CContextImpl*)(IContext*)context)->ScheduleFinalCleanup(who, String("Service"));
        }
        QueuedWork::WaitToFinish();

//        try {
        AutoPtr<IIActivityManager> activityManager = ActivityManagerNative::GetDefault();
        activityManager->ServiceDoneExecuting(token, 0, 0, 0);
//        } catch (RemoteException e) {
//            // nothing to do.
//        }
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to stop service " + s
//                        + ": " + e.toString(), e);
//            }
//        }
        mServices.Erase(token);
    }
    //Slog.i(TAG, "Running services: " + mServices);
    return NOERROR;
}

AutoPtr<CActivityThread::ActivityClientRecord> CActivityThread::PerformResumeActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean clearHide)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    Boolean finished = FALSE;
    if (r != NULL) r->mActivity->IsFinishing(&finished);
    if (localLOGV) {
        Slogger::V(TAG, "Performing resume of %p finished=%d, clearHide=%d", r.Get(), finished, clearHide);
    }
    if (r != NULL && !finished) {
        if (clearHide) {
            r->mHideForNow = FALSE;
            r->mActivity->SetStartedActivity(FALSE);
        }
//        try {
        AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
        activity->mFragments->NoteStateNotSaved();
        if (r->mPendingIntents != NULL) {
            DeliverNewIntents(r, r->mPendingIntents);
            r->mPendingIntents = NULL;
        }
        if (r->mPendingResults != NULL) {
            DeliverResults(r, r->mPendingResults);
            r->mPendingResults = NULL;
        }
        r->mActivity->PerformResume();

//        EventLog.writeEvent(LOG_ON_RESUME_CALLED,
//                UserHandle.myUserId(), r.activity.getComponentName().getClassName());

        r->mPaused = FALSE;
        r->mStopped = FALSE;
        r->mState = NULL;
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(r.activity, e)) {
//                throw new RuntimeException(
//                    "Unable to resume activity "
//                    + r.intent.getComponent().toShortString()
//                    + ": " + e.toString(), e);
//            }
//        }
    }
    return r;
}

void CActivityThread::CleanUpPendingRemoveWindows(
    /* [in] */ ActivityClientRecord* r)
{
    if (r->mPendingRemoveWindow != NULL) {
        r->mPendingRemoveWindowManager->RemoveViewImmediate(r->mPendingRemoveWindow);
        AutoPtr<IBinder> wtoken;
        r->mPendingRemoveWindow->GetWindowToken((IBinder**)&wtoken);
        if (wtoken != NULL) {
            String activityName;
            AutoPtr<IComponentName> cn;
            r->mActivity->GetComponentName((IComponentName**)&cn);
            cn->ToShortString(&activityName);
            CWindowManagerGlobal::GetInstance()->CloseAll(wtoken, activityName, String("Activity"));
        }
    }
    r->mPendingRemoveWindow = NULL;
    r->mPendingRemoveWindowManager = NULL;
}

ECode CActivityThread::HandleResumeActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean clearHide,
    /* [in] */ Boolean isForward,
    /* [in] */ Boolean reallyResume)
{
    // If we are getting ready to gc after going to the background, well
    // we are back active so skip it.
    UnscheduleGcIdler();

    AutoPtr<ActivityClientRecord> r = PerformResumeActivity(token, clearHide);
    if (r != NULL) {
        AutoPtr<IActivity> a = r->mActivity;

        if (localLOGV) {
            AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
            String rdes;
            r->ToString(&rdes);
            Slogger::V(TAG, "Resume %s started activity: %d, hideForNow: %d, finished: "
                , rdes.string(), activity->mStartedActivity
                , r->mHideForNow, activity->mFinished);
        }

        Int32 forwardBit = isForward ?
            IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION : 0;

        // If the window hasn't yet been added to the window manager,
        // and this guy didn't finish itself or start another activity,
        // then go ahead and add the window.
        Boolean willBeVisible = FALSE;
        a->IsStartedActivity(&willBeVisible);
        if (!willBeVisible) {
            AutoPtr<IIActivityManager> service = ActivityManagerNative::GetDefault();
            AutoPtr<IBinder> t;
            a->GetActivityToken((IBinder**)&t);
            service->WillActivityBeVisible(t.Get(), &willBeVisible);
        }

        Boolean finished = FALSE;
        a->IsFinishing(&finished);
        if (r->mWindow == NULL && !finished && willBeVisible) {
            r->mActivity->GetWindow((IWindow**)&r->mWindow);
            AutoPtr<IView> decor;
            r->mWindow->GetDecorView((IView**)&decor);
            decor->SetVisibility(IView::INVISIBLE);
            AutoPtr<IViewManager> wm;
            a->GetWindowManager((IWindowManager**)&wm);
            AutoPtr<IWindowManagerLayoutParams> l;
            r->mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
            a->SetDecorView(decor);
            l->SetType(IWindowManagerLayoutParams::TYPE_BASE_APPLICATION);
            Int32 softInputMode;
            l->GetSoftInputMode(&softInputMode);
            l->SetSoftInputMode(softInputMode | forwardBit);

            Boolean visibleFromClient = FALSE;
            a->IsVisibleFromClient(&visibleFromClient);
            if (visibleFromClient) {
                a->SetWindowAdded(TRUE);
                wm->AddView(decor, l);
            }

        // If the window has already been added, but during resume
        // we started another activity, then don't yet make the
        // window visisble.
        }
        else if (!willBeVisible) {
            if (localLOGV) {
                Slogger::V(TAG, "Launch %p mStartedActivity set", r.Get());
            }
            r->mHideForNow = TRUE;
        }

        // Get rid of anything left hanging around.
        CleanUpPendingRemoveWindows(r);

        // The window is now visible if it has been added, we are not
        // simply finishing, and we are not starting another activity.
        r->mActivity->IsFinishing(&finished);
        AutoPtr<IView> decor;
        r->mActivity->GetDecorView((IView**)&decor);
        if (!finished && willBeVisible && decor != NULL && !r->mHideForNow) {
            if (r->mNewConfig != NULL) {
//                if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Resuming activity %s with newConfig %p"
//                        , r->mActivityInfo->mName.string(), r->mNewConfig.Get());
                AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
                PerformConfigurationChanged(activity, r->mNewConfig);
                Int32 changed;
                activity->mCurrentConfig->Diff(r->mNewConfig, &changed);
                FreeTextLayoutCachesIfNeeded(changed);
                r->mNewConfig = NULL;
            }

            if (localLOGV) Slogger::V(TAG, "Resuming %p with isForward=%d", r.Get(), isForward);

            AutoPtr<IWindowManagerLayoutParams> l;
            r->mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
            Int32 mode;
            l->GetSoftInputMode(&mode);
            if ((mode & IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION) != forwardBit) {
                mode = (mode & (~IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION))
                        | forwardBit;
                Boolean isVisibleFromClient;
                r->mActivity->IsVisibleFromClient(&isVisibleFromClient);
                if (isVisibleFromClient) {
                    AutoPtr<IViewManager> wm;
                    a->GetWindowManager((IWindowManager**)&wm);
                    AutoPtr<IView> decor;
                    r->mWindow->GetDecorView((IView**)&decor);
                    wm->UpdateViewLayout(decor, l);
                }
            }
            r->mActivity->SetVisibleFromServer(TRUE);
            mNumVisibleActivities++;
            Boolean isVisibleFromClient;
            r->mActivity->IsVisibleFromClient(&isVisibleFromClient);
            if (isVisibleFromClient) {
                r->mActivity->MakeVisible();
            }
        }

        if (!r->mOnlyLocalRequest) {
            r->mNextIdle = mNewActivities;
            mNewActivities = r;
            if (localLOGV)  {
                Slogger::V(TAG, "Scheduling idle handler for %p", r.Get());
            }
            Looper::GetMyQueue()->AddIdleHandler(new Idler(this));
        }
        r->mOnlyLocalRequest = FALSE;

        // Tell the activity manager we have resumed.
        if (reallyResume) {
//            try {
            ActivityManagerNative::GetDefault()->ActivityResumed(token);
//            } catch (RemoteException ex) {
//            }
        }
    }
    else {
        Slogger::E(TAG, "An exception was thrown when trying to resume.");
        // If an exception was thrown when trying to resume, then
        // just end this activity.
//            try {
        AutoPtr<IIActivityManager> serv = ActivityManagerNative::GetDefault();
        Boolean isFinish;
        serv->FinishActivity(token, IActivity::RESULT_CANCELED, NULL, &isFinish);
//            } catch (RemoteException ex) {
//            }
    }
    return NOERROR;
}

AutoPtr<IBitmap> CActivityThread::CreateThumbnailBitmap(
    /* [in] */ ActivityClientRecord* r)
{
    AutoPtr<IBitmap> thumbnail = mAvailThumbnailBitmap;
//     try {
    if (thumbnail == NULL) {
        Int32 w = mThumbnailWidth;
        Int32 h;
        if (w < 0) {
            AutoPtr<IResources> res;
            r->mActivity->GetResources((IResources**)&res);
            res->GetDimensionPixelSize(R::dimen::thumbnail_height, &h);
            mThumbnailHeight = h;

            res->GetDimensionPixelSize(R::dimen::thumbnail_width, &w);
            mThumbnailWidth = w;
        } else {
            h = mThumbnailHeight;
        }

        // On platforms where we don't want thumbnails, set dims to (0,0)
        if ((w > 0) && (h > 0)) {
            AutoPtr<IResources> resources;
            r->mActivity->GetResources((IResources**)&resources);
            AutoPtr<IDisplayMetrics> dm;
            resources->GetDisplayMetrics((IDisplayMetrics**)&dm);
            CBitmap::CreateBitmap(dm, w, h, THUMBNAIL_FORMAT, (IBitmap**)&thumbnail);
            thumbnail->EraseColor(0);
        }
    }

    if (thumbnail != NULL) {
        AutoPtr<ICanvas> cv = mThumbnailCanvas;
        if (cv == NULL) {
            CCanvas::New((ICanvas**)&cv);
            mThumbnailCanvas = cv;
        }

        cv->SetBitmap(thumbnail);
        Boolean value;
        r->mActivity->OnCreateThumbnail(thumbnail, cv, &value);
        if (!value) {
            mAvailThumbnailBitmap = thumbnail;
            thumbnail = NULL;
        }
        cv->SetBitmap(NULL);
    }

//     } catch (Exception e) {
//         if (!mInstrumentation.onException(r.activity, e)) {
//             throw new RuntimeException(
//                     "Unable to create thumbnail of "
//                     + r.intent.getComponent().toShortString()
//                     + ": " + e.toString(), e);
//         }
//         thumbnail = null;
//     }

    return thumbnail;
}

ECode CActivityThread::HandlePauseActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean userLeaving,
    /* [in] */ Int32 configChanges)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    if (r != NULL) {
        Slogger::V(TAG, "userLeaving=%d handling pause of %p", userLeaving, r.Get());
        if (userLeaving) {
            PerformUserLeavingActivity(r);
        }

        Int32 ccFlags;
        r->mActivity->GetConfigChangeFlags(&ccFlags);
        r->mActivity->SetConfigChangeFlags(ccFlags | configChanges);
        AutoPtr<IBundle> state;
        PerformPauseActivity(token, finished, r->IsPreHoneycomb(), (IBundle**)&state);

       // Make sure any pending writes are now committed.
       if (r->IsPreHoneycomb()) {
           QueuedWork::WaitToFinish();
       }

        // Tell the activity manager we have paused.
        AutoPtr<IIActivityManager> amService = ActivityManagerNative::GetDefault();
        amService->ActivityPaused(token);
    }
    return NOERROR;
}

ECode CActivityThread::PerformUserLeavingActivity(
    /* [in] */ ActivityClientRecord* r)
{
    return mInstrumentation->CallActivityOnUserLeaving(r->mActivity);
}

ECode CActivityThread::PerformPauseActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean saveState,
    /* [out] */ IBundle** state)
{
    VALIDATE_NOT_NULL(state);

    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    if (r != NULL) {
        return PerformPauseActivity(r, finished, saveState, state);
    }
    else {
        *state = NULL;
        return NOERROR;
    }
}

ECode CActivityThread::PerformPauseActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean saveState,
    /* [out] */ IBundle** state)
{
    VALIDATE_NOT_NULL(state);
    *state = NULL;
    Boolean isFinished;
    if (r->mPaused) {
        if (r->mActivity->IsFinishing(&isFinished), isFinished) {
            // If we are finishing, we won't call onResume() in certain cases.
            // So here we likewise don't want to call onPause() if the activity
            // isn't resumed.
            return NOERROR;
        }
//        RuntimeException e = new RuntimeException(
//                "Performing pause of activity that is not resumed: "
//                + r.intent.getComponent().toShortString());
//        Slog.e(TAG, e.getMessage(), e);
    }
    AutoPtr<CBundle> st;
    if (finished) {
        r->mActivity->SetFinishing(TRUE);
    }
//    try {
    // Next have the activity save its current state and managed dialogs...
    if ((r->mActivity->IsFinishing(&isFinished), !isFinished) && saveState) {
        CBundle::NewByFriend((CBundle**)&st);
        st->SetAllowFds(FALSE);
        mInstrumentation->CallActivityOnSaveInstanceState(
                r->mActivity, (IBundle*)(CBundle*)st);
        r->mState = st;
    }
    // Now we are idle.
    r->mActivity->SetCalled(FALSE);
    mInstrumentation->CallActivityOnPause(r->mActivity);
//    EventLog.writeEvent(LOG_ON_PAUSE_CALLED, UserHandle.myUserId(),
//         r.activity.getComponentName().getClassName());
    Boolean called;
    if (r->mActivity->IsCalled(&called), !called) {
        String activityName;
        AutoPtr<IComponentName> cn;
        r->mActivity->GetComponentName((IComponentName**)&cn);
        cn->ToShortString(&activityName);
        Slogger::E(TAG, "Activity %s did not call through to super.onPause().", activityName.string());
        return E_SUPER_NOT_CALLED_EXCEPTION;
//        throw new SuperNotCalledException(
//            "Activity " + r.intent.getComponent().toShortString() +
//            " did not call through to super.onPause()");
    }

//    } catch (SuperNotCalledException e) {
//        throw e;
//
//    } catch (Exception e) {
//        if (!mInstrumentation.onException(r.activity, e)) {
//            throw new RuntimeException(
//                    "Unable to pause activity "
//                    + r.intent.getComponent().toShortString()
//                    + ": " + e.toString(), e);
//        }
//    }
    r->mPaused = TRUE;

    // Notify any outstanding on paused listeners
    AutoPtr<OnPauseListenerList> listeners;
    {
        AutoLock lock(mOnPauseListenersLock);
        OnPauseListenerMapIterator it = mOnPauseListeners.Find(r->mActivity);
        if (it != mOnPauseListeners.End()) {
            listeners = it->mSecond;
            mOnPauseListeners.Erase(r->mActivity);
        }
    }

    if (listeners != NULL) {
        OnPauseListenerListIterator it;
        for (it = listeners->Begin(); it != listeners->End(); ++it) {
            (*it)->OnPaused(r->mActivity);
        }
    }

    *state = st;
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode CActivityThread::PerformStopActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean saveState)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    return PerformStopActivityInner(r, NULL, FALSE, saveState);
}

ECode CActivityThread::PerformStopActivityInner(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ StopInfo* info,
    /* [in] */ Boolean keepShown,
    /* [in] */ Boolean saveState)
{
    if (localLOGV) Slogger::V(TAG, "Performing stop of %p", r);
    AutoPtr<CBundle> state;
    if (r != NULL) {
        if (!keepShown && r->mStopped) {
            Boolean finished;
            if (r->mActivity->IsFinishing(&finished), finished) {
                // If we are finishing, we won't call onResume() in certain
                // cases.  So here we likewise don't want to call onStop()
                // if the activity isn't resumed.
                return NOERROR;
            }
//            RuntimeException e = new RuntimeException(
//                    "Performing stop of activity that is not resumed: "
//                    + r.intent.getComponent().toShortString());
//            Slog.e(TAG, e.getMessage(), e);
        }

        if (info != NULL) {
//            try {
            // First create a thumbnail for the activity...
            // For now, don't create the thumbnail here; we are
            // doing that by doing a screen snapshot.
            info->mThumbnail = NULL; //createThumbnailBitmap(r);
            info->mDescription = NULL;
            r->mActivity->OnCreateDescription((ICharSequence**)&info->mDescription);
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to save state of activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
        }

        // Next have the activity save its current state and managed dialogs...
        AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
        if (!activity->mFinished && saveState) {
            if (r->mState == NULL) {
                CBundle::NewByFriend((CBundle**)&state);
                state->SetAllowFds(FALSE);
                mInstrumentation->CallActivityOnSaveInstanceState(r->mActivity, state);
                r->mState = state;
            } else {
                state = r->mState;
            }
        }

        if (!keepShown) {
//            try {
                // Now we are idle.
                ECode ec = r->mActivity->PerformStop();
                if (FAILED(ec)) {
                    String activityName;
                    AutoPtr<IComponentName> cn;
                    r->mActivity->GetComponentName((IComponentName**)&cn);
                    cn->ToShortString(&activityName);
                    Slogger::E(TAG, "Unable to stop activity %s, ec=%08x", activityName.string(), ec);
                }
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to stop activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
            r->mStopped = TRUE;
        }

        r->mPaused = TRUE;
    }
    return NOERROR;
}

void CActivityThread::UpdateVisibility(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ Boolean show)
{
    AutoPtr<IView> v;
    r->mActivity->GetDecorView((IView**)&v);
    if (v != NULL) {
        if (show) {
            Boolean isVisible;
            r->mActivity->IsVisibleFromServer(&isVisible);
            if (!isVisible) {
                r->mActivity->SetVisibleFromServer(TRUE);
                mNumVisibleActivities++;
                Boolean isVClient;
                r->mActivity->IsVisibleFromClient(&isVClient);
                if (isVClient) {
                    r->mActivity->MakeVisible();
                }
            }
            if (r->mNewConfig != NULL) {
//                if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Updating activity vis %s with new config %p"
//                        , r->mActivityInfo->mName.string(), r->mNewConfig.Get());
                AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
                PerformConfigurationChanged(activity, r->mNewConfig);
                Int32 changed;
                activity->mCurrentConfig->Diff(r->mNewConfig, &changed);
                FreeTextLayoutCachesIfNeeded(changed);
                r->mNewConfig = NULL;
            }
        } else {
            Boolean isVisible;
            r->mActivity->IsVisibleFromServer(&isVisible);
            if (isVisible) {
                r->mActivity->SetVisibleFromServer(FALSE);
                mNumVisibleActivities--;
                v->SetVisibility(IView::INVISIBLE);
            }
        }
    }
}

ECode CActivityThread::HandleStopActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean show,
    /* [in] */ Int32 configChanges)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    Int32 ccFlags;
    r->mActivity->GetConfigChangeFlags(&ccFlags);
    r->mActivity->SetConfigChangeFlags(ccFlags | configChanges);

    AutoPtr<StopInfo> info = new StopInfo();
    PerformStopActivityInner(r, info, show, TRUE);

    if (localLOGV) Slogger::V(
            TAG, "Finishing stop of %p: show=%d win=%p", r.Get(), show, r->mWindow.Get());

    UpdateVisibility(r, show);

    // Make sure any pending writes are now committed.
    if (!r->IsPreHoneycomb()) {
        QueuedWork::WaitToFinish();
    }

    // Schedule the call to tell the activity manager we have
    // stopped.  We don't do this immediately, because we want to
    // have a chance for any other pending work (in particular memory
    // trim requests) to complete before you tell the activity
    // manager to proceed and allow us to go fully into the background.
    info->mActivity = r;
    info->mState = r->mState;
    Boolean result = FALSE;
    return mH->Post(info, &result);
}

void CActivityThread::PerformRestartActivity(
    /* [in] */ IBinder* token)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    if (r->mStopped) {
        r->mActivity->PerformRestart();
        r->mStopped = FALSE;
    }
}

ECode CActivityThread::HandleWindowVisibility(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean show)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);

    if (r == NULL) {
        Slogger::W(TAG, "handleWindowVisibility: no activity for token %p", token);
        return NOERROR;
    }
    if (!show && !r->mStopped) {
        PerformStopActivityInner(r, NULL, show, FALSE);
    } else if (show && r->mStopped) {
        r->mActivity->PerformRestart();
        r->mStopped = FALSE;
    }
    AutoPtr<IView> decorview;
    r->mActivity->GetDecorView((IView**)&decorview);
    if (decorview != NULL) {
        if (FALSE) Slogger::V(
           TAG, "Handle window %p visibility: %d", r.Get(), show);
        UpdateVisibility(r, show);
   }
    return NOERROR;
}

ECode CActivityThread::HandleSleeping(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean sleeping)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);

    if (r == NULL) {
        Slogger::W(TAG, "handleSleeping: no activity for token %p", token);
        return NOERROR;
    }

    if (sleeping) {
        if (!r->mStopped && !r->IsPreHoneycomb()) {
//            try {
            // Now we are idle.
            r->mActivity->PerformStop();
//            } catch (Exception e) {
//                 if (!mInstrumentation.onException(r.activity, e)) {
//                     throw new RuntimeException(
//                             "Unable to stop activity "
//                             + r.intent.getComponent().toShortString()
//                             + ": " + e.toString(), e);
//                 }
//             }
            r->mStopped = TRUE;
        }

        // Make sure any pending writes are now committed.
        if (!r->IsPreHoneycomb()) {
            QueuedWork::WaitToFinish();
        }

        // Tell activity manager we slept.
//         try {
        ActivityManagerNative::GetDefault()->ActivitySlept(r->mToken);
//         } catch (RemoteException ex) {
//         }
    } else {
        Boolean isVisible;
        r->mActivity->IsVisibleFromServer(&isVisible);
        if (r->mStopped && isVisible) {
            r->mActivity->PerformRestart();
            r->mStopped = FALSE;
        }
    }
    return NOERROR;
}

ECode CActivityThread::HandleSetCoreSettings(
    /* [in] */ IBundle* coreSettings)
{
    AutoLock lock(mPackagesLock);
    mCoreSettings = coreSettings;
    return NOERROR;
}

ECode CActivityThread::HandleUpdatePackageCompatibilityInfo(
    /* [in] */ UpdateCompatibilityData* data)
{
    assert(data != NULL);
    LoadedPkg* pkg = PeekPackageInfo(data->mPkg, FALSE);
    if (pkg != NULL) {
        pkg->mCompatibilityInfo->Set(data->mInfo);
    }
    pkg = PeekPackageInfo(data->mPkg, TRUE);
    if (pkg != NULL) {
        pkg->mCompatibilityInfo->Set(data->mInfo);
    }
    HandleConfigurationChanged(mConfiguration, data->mInfo);
    AutoPtr<IWindowManagerGlobal> wmg = CWindowManagerGlobal::GetInstance();
    wmg->ReportNewConfiguration(mConfiguration);
    return NOERROR;
}

ECode CActivityThread::DeliverResults(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ List< AutoPtr<IResultInfo> >* results)
{
    List< AutoPtr<IResultInfo> >::Iterator it1 = results->Begin();
    List< AutoPtr<IResultInfo> >::Iterator it2 = results->End();
    for (; it1 != it2; ++it1) {
//        try {
        AutoPtr<IResultInfo> ri = *it1;
        AutoPtr<IIntent> intent;
        ri->GetData((IIntent**)&intent);
        if (intent != NULL) {
            AutoPtr<IClassLoader> classLoader;
            r->mActivity->GetClassLoader((IClassLoader**)&classLoader);
            // intent->SetExtrasClassLoader(classLoader);
        }

        if (DEBUG_RESULTS) {
            Slogger::V(TAG, "Delivering result to activity %p : %p", r, ri.Get());
        }

        String resultWho;
        Int32 requestCode = 0;
        Int32 resultCode = 0;
        AutoPtr<IIntent> data;

        ri->GetResultWho(&resultWho);
        ri->GetRequestCode(&requestCode);
        ri->GetResultCode(&resultCode);
        ri->GetData((IIntent**)&data);

        AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
        activity->DispatchActivityResult(resultWho, requestCode, resultCode, data);
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(r.activity, e)) {
//                throw new RuntimeException(
//                        "Failure delivering result " + ri + " to activity "
//                        + r.intent.getComponent().toShortString()
//                        + ": " + e.toString(), e);
//            }
//        }
    }
    return NOERROR;
}

ECode CActivityThread::HandleSendResult(
    /* [in] */ ResultData* res)
{
    assert(res != NULL);
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(res->mToken);
    if (DEBUG_RESULTS) Slogger::V(TAG, "Handling send result to %p", r.Get());
    if (r != NULL) {
        Boolean resumed = !r->mPaused;
        Boolean isFinish;
        r->mActivity->IsFinishing(&isFinish);
        AutoPtr<IView> decorview;
        r->mActivity->GetDecorView((IView**)&decorview);
        if (!isFinish && decorview != NULL && r->mHideForNow && resumed) {
            // We had hidden the activity because it started another
            // one...  we have gotten a result back and we are not
            // paused, so make sure our window is visible.
            UpdateVisibility(r, TRUE);
        }
        AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
        if (resumed) {
//            try {
                // Now we are idle.
            r->mActivity->SetCalled(FALSE);
            activity->mTemporaryPause = TRUE;
            mInstrumentation->CallActivityOnPause(r->mActivity);
            Boolean called;
            if (r->mActivity->IsCalled(&called), !called) {
                String activityName;
                AutoPtr<IComponentName> cn;
                r->mActivity->GetComponentName((IComponentName**)&cn);
                cn->ToShortString(&activityName);
                Slogger::E(TAG, "Activity %s did not call through to super.onPause().", activityName.string());
                return E_SUPER_NOT_CALLED_EXCEPTION;
//                    throw new SuperNotCalledException(
//                        "Activity " + r.intent.getComponent().toShortString()
//                        + " did not call through to super.onPause()");
            }
//            } catch (SuperNotCalledException e) {
//                throw e;
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to pause activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
        }
        DeliverResults(r, &res->mResults);
        if (resumed) {
            r->mActivity->PerformResume();
            activity->mTemporaryPause = FALSE;
        }
    }

    return NOERROR;
}

AutoPtr<CActivityThread::ActivityClientRecord>
CActivityThread::PerformDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing)
{
    return PerformDestroyActivity(token, finishing, 0, FALSE);
}

AutoPtr<CActivityThread::ActivityClientRecord>
CActivityThread::PerformDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean getNonConfigInstance)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
//    Class activityClass = null;
    if (localLOGV) Slogger::V(TAG, "Performing finish of %p", r.Get());
    if (r != NULL) {
//        activityClass = r.activity.getClass();
        String activityName;
        AutoPtr<IComponentName> cn;
        r->mActivity->GetComponentName((IComponentName**)&cn);
        cn->ToShortString(&activityName);

        Int32 ccFlags;
        r->mActivity->GetConfigChangeFlags(&ccFlags);
        r->mActivity->SetConfigChangeFlags(ccFlags | configChanges);
        if (finishing) {
            r->mActivity->SetFinishing(TRUE);
        }
        if (!r->mPaused) {
            r->mActivity->SetCalled(FALSE);
            ECode ec = mInstrumentation->CallActivityOnPause(r->mActivity);
//            EventLog.writeEvent(LOG_ON_PAUSE_CALLED, UserHandle.myUserId(),
//                r.activity.getComponentName().getClassName());
            if (SUCCEEDED(ec)) {
                Boolean isCalled;
                r->mActivity->IsCalled(&isCalled);
                if (!isCalled) {
                    Slogger::E(TAG, "Activity %s did not call through to super.onPause().", activityName.string());
                }

                Boolean res = FALSE;
                mInstrumentation->OnException(r->mActivity, ec, &res);
                if (!res) {
                    Slogger::E(TAG, "Unable to pause activity %s : %08x", activityName.string(), ec);
                }
            }

            if (FAILED(ec)) {
                Boolean res = FALSE;
                mInstrumentation->OnException(r->mActivity, ec, &res);
                if (!res) {
                    Slogger::E(TAG, "Unable to pause activity %s : %08x", activityName.string(), ec);
                }
            }
            r->mPaused = TRUE;
        }
        if (!r->mStopped) {
            ECode ec = r->mActivity->PerformStop();

            if (FAILED(ec)) {
                Boolean res = FALSE;
                mInstrumentation->OnException(r->mActivity, ec, &res);
                if (!res) {
                    Slogger::E(TAG, "Unable to stop activity %s : %08x", activityName.string(), ec);
                }
            }

            r->mStopped = TRUE;
        }
        if (getNonConfigInstance) {
            r->mLastNonConfigurationInstances = NULL;
            ECode ec = r->mActivity->RetainNonConfigurationInstances(
                (IActivityNonConfigurationInstances**)&r->mLastNonConfigurationInstances);

           if (FAILED(ec)) {
                Boolean res = FALSE;
                mInstrumentation->OnException(r->mActivity, ec, &res);
                if (!res) {
                    Slogger::E(TAG, "Unable to retain activity %s : %08x", activityName.string(), ec);
                }
            }
        }

        r->mActivity->SetCalled(FALSE);
        ECode ec = mInstrumentation->CallActivityOnDestroy(r->mActivity);

        if (SUCCEEDED(ec)) {
            Boolean isCalled;
            r->mActivity->IsCalled(&isCalled);
            if (!isCalled) {
                Boolean reslut = FALSE;
                mInstrumentation->OnException(r->mActivity, ec, &reslut);
                if (!reslut) {
                    Slogger::E(TAG, "Activity %s did not call through to super.onDestroy().", activityName.string());
                }
            }

            if (r->mWindow != NULL) {
                r->mWindow->CloseAllPanels();
            }
        }

        if (FAILED(ec)) {
            Boolean res = FALSE;
            mInstrumentation->OnException(r->mActivity, ec, &res);
            if (!res) {
                Slogger::E(TAG, "Unable to destroy activity %s : %08x", activityName.string(), ec);
            }
        }

    }
    mActivities.Erase(token);
//    StrictMode.decrementExpectedActivityCount(activityClass);
    return r;
}

String CActivityThread::SafeToComponentShortString(
    /* [in] */ IIntent* intent)
{
    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    String comName;
    component->ToShortString(&comName);
    return component == NULL ? String("[Unknown]") : comName;
}

ECode CActivityThread::HandleDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean getNonConfigInstance)
{
    AutoPtr<ActivityClientRecord> r = PerformDestroyActivity(token, finishing,
            configChanges, getNonConfigInstance);
    if (r != NULL) {
        CleanUpPendingRemoveWindows(r);
        AutoPtr<IWindowManager> wm;
        r->mActivity->GetWindowManager((IWindowManager**)&wm);
        AutoPtr<IView> v;
        r->mActivity->GetDecorView((IView**)&v);
        String activityName;
        AutoPtr<IComponentName> cn;
        r->mActivity->GetComponentName((IComponentName**)&cn);
        cn->ToShortString(&activityName);
        if (v != NULL) {
            Boolean visible;
            r->mActivity->IsVisibleFromServer(&visible);
            if (visible) {
               mNumVisibleActivities--;
            }
            AutoPtr<IBinder> wtoken;
            v->GetWindowToken((IBinder**)&wtoken);
            Boolean isAdded = TRUE;
            r->mActivity->IsWindowAdded(&isAdded);
            if (isAdded) {
                if (r->mOnlyLocalRequest) {
                    // Hold off on removing this until the new activity's
                    // window is being added.
                    r->mPendingRemoveWindow = v;
                    r->mPendingRemoveWindowManager = wm;
                } else {
                    wm->RemoveViewImmediate(v);
                }
            }

            if (wtoken != NULL && r->mPendingRemoveWindow == NULL) {
                AutoPtr<IWindowManagerGlobal> wmg = CWindowManagerGlobal::GetInstance();
                wmg->CloseAll(wtoken, activityName, String("Activity"));
            }
            r->mActivity->SetDecorView(NULL);
        }

        if (r->mPendingRemoveWindow == NULL) {
            // If we are delaying the removal of the activity window, then
            // we can't clean up all windows here.  Note that we can't do
            // so later either, which means any windows that aren't closed
            // by the app will leak.  Well we try to warning them a lot
            // about leaking windows, because that is a bug, so if they are
            // using this recreate facility then they get to live with leaks.
            AutoPtr<IWindowManagerGlobal> wmg = CWindowManagerGlobal::GetInstance();
            wmg->CloseAll(token, activityName, String("Activity"));
        }

        // Mocked out contexts won't be participating in the normal
        // process lifecycle, but if we're running with a proper
        // ApplicationContext we need to have it tear down things
        // cleanly.
        AutoPtr<IContext> c;
        r->mActivity->GetBaseContext((IContext**)&c);
        IContextImpl* ci = IContextImpl::Probe(c);
        if (ci) {
            ((CContextImpl*)ci)->ScheduleFinalCleanup(activityName, String("Activity"));
        }
    }

    if (finishing) {
        AutoPtr<IIActivityManager> amService = ActivityManagerNative::GetDefault();
        ECode ec = amService->ActivityDestroyed(token);
        UNUSED(ec);
    }
    return NOERROR;
}

ECode CActivityThread::RequestRelaunchActivity(
    /* [in] */ IBinder* token,
    /* [in] */ ArrayOf<IResultInfo*>* pendingResults,
    /* [in] */ ArrayOf<IIntent*>* pendingNewIntents,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean notResumed,
    /* [in] */ IConfiguration* config,
    /* [in] */ Boolean fromServer)
{
    AutoPtr<ActivityClientRecord> target;

    {
        AutoLock lock(mPackagesLock);

        List< AutoPtr<ActivityClientRecord> >::Iterator it;
        for (it = mRelaunchingActivities.Begin(); it != mRelaunchingActivities.End();
                ++it) {
            AutoPtr<ActivityClientRecord> r = *it;
            if (r->mToken.Get() == token) {
                target = r;
                if (pendingResults != NULL) {
                    if (r->mPendingResults != NULL) {
                        Int32 length = pendingResults->GetLength();
                        for (Int32 i = 0; i < length; ++i) {
                            r->mPendingResults->PushBack((*pendingResults)[i]);
                        }
                    }
                    else {
                        r->mPendingResults = new List< AutoPtr<IResultInfo> >();
                        Int32 length = pendingResults->GetLength();
                        for (Int32 i = 0; i < length; ++i) {
                            r->mPendingResults->PushBack((*pendingResults)[i]);
                        }
                    }
                }
                if (pendingNewIntents != NULL) {
                    if (r->mPendingIntents != NULL) {
                        Int32 length = pendingNewIntents->GetLength();
                        for (Int32 i = 0; i < length; ++i) {
                            r->mPendingIntents->PushBack((*pendingNewIntents)[i]);
                        }
                    }
                    else {
                        r->mPendingIntents = new List< AutoPtr<IIntent> >();
                        Int32 length = pendingNewIntents->GetLength();
                        for (Int32 i = 0; i < length; ++i) {
                            r->mPendingIntents->PushBack((*pendingNewIntents)[i]);
                        }
                    }
                }
                break;
            }
        }

        if (target == NULL) {
            target = new ActivityClientRecord();
            target->mToken = token;
            target->mPendingResults = new List< AutoPtr<IResultInfo> >();
            target->mPendingIntents = new List< AutoPtr<IIntent> >();

            if (pendingResults != NULL) {
                Int32 rlength = pendingResults->GetLength();
                for (Int32 i = 0; i < rlength; ++i) {
                    target->mPendingResults->PushBack((*pendingResults)[i]);
                }
            }

            if (pendingNewIntents != NULL) {
                Int32 ilength = pendingNewIntents->GetLength();
                for (Int32 i = 0; i < ilength; ++i) {
                    target->mPendingIntents->PushBack((*pendingNewIntents)[i]);
                }
            }

            if (!fromServer) {
                AutoPtr<ActivityClientRecord> existing = GetActivityClientRecord(token);
                if (existing != NULL) {
                    target->mStartsNotResumed = existing->mPaused;
                }
                target->mOnlyLocalRequest = TRUE;
            }
            mRelaunchingActivities.PushBack(target);

            QueueOrSendMessage(CActivityThread::H::RELAUNCH_ACTIVITY, target);
        }

        if (fromServer) {
            target->mStartsNotResumed = notResumed;
            target->mOnlyLocalRequest = FALSE;
        }
        if (config != NULL) {
            target->mCreatedConfig = config;
        }
        target->mPendingConfigChanges |= configChanges;
    }
    return NOERROR;
}

ECode CActivityThread::HandleRelaunchActivity(
    /* [in] */ ActivityClientRecord* acr)
{
    assert(acr != NULL);
    AutoPtr<ActivityClientRecord> tmp = acr;

    // If we are getting ready to gc after going to the background, well
    // we are back active so skip it.
    UnscheduleGcIdler();

    AutoPtr<IConfiguration> changedConfig;
    Int32 configChanges = 0;

    // First: make sure we have the most recent configuration and most
    // recent version of the activity, or skip it if some previous call
    // had taken a more recent version.
    {
        AutoLock lock(mPackagesLock);
        AutoPtr<IBinder> token = tmp->mToken;
        tmp = NULL;
        List< AutoPtr<ActivityClientRecord> >::Iterator it;
        for (it = mRelaunchingActivities.Begin(); it != mRelaunchingActivities.End();) {
            ActivityClientRecord* r = *it;
            if (r->mToken == token) {
                tmp = r;
                configChanges |= tmp->mPendingConfigChanges;
                it = mRelaunchingActivities.Erase(it);
            }
            else {
                ++it;
            }
        }

        if (tmp == NULL) {
            if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Abort, activity not relaunching!");
            return NOERROR;
        }

       if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Relaunching activity %p with configChanges=0x"
               , tmp->mToken.Get()/*, Integer.toHexString(configChanges)*/);

        if (mPendingConfiguration != NULL) {
            changedConfig = mPendingConfiguration;
            mPendingConfiguration = NULL;
        }
    }

    if (tmp->mCreatedConfig != NULL) {
        // If the activity manager is passing us its current config,
        // assume that is really what we want regardless of what we
        // may have pending.
        Boolean isNewer;
        tmp->mCreatedConfig->IsOtherSeqNewer(mConfiguration, &isNewer);
        Int32 diffRes;
        mConfiguration->Diff(tmp->mCreatedConfig, &diffRes);
        if (mConfiguration == NULL || (isNewer && diffRes != 0)) {
            Boolean isother;
            tmp->mCreatedConfig->IsOtherSeqNewer(changedConfig, &isother);
            if (changedConfig == NULL || isother) {
                changedConfig = tmp->mCreatedConfig;
            }
        }
    }

   if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Relaunching activity %p: changedConfig=%p"
           , tmp->mToken.Get(), changedConfig.Get());

    // If there was a pending configuration change, execute it first.
    if (changedConfig != NULL) {
        changedConfig->GetDensityDpi(&mCurDefaultDisplayDpi);
        UpdateDefaultDensity();
        HandleConfigurationChanged(changedConfig, NULL);
    }

    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(tmp->mToken);
    if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Handling relaunch of %p", r.Get());
    if (r == NULL) {
        return NOERROR;
    }

    Int32 changeFlags;
    r->mActivity->GetConfigChangeFlags(&changeFlags);
    r->mActivity->SetConfigChangeFlags(changeFlags | configChanges);
    r->mOnlyLocalRequest = tmp->mOnlyLocalRequest;
    AutoPtr<IIntent> currentIntent;
    r->mActivity->GetIntent((IIntent**)&currentIntent);

    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
    activity->mChangingConfigurations = TRUE;

    // Need to ensure state is saved.
    AutoPtr<CBundle> savedState;
    if (!r->mPaused) {
        PerformPauseActivity(r->mToken, FALSE, r->IsPreHoneycomb(), (IBundle**)&savedState);
    }
    if (r->mState == NULL && !r->mStopped && !r->IsPreHoneycomb()) {
        CBundle::NewByFriend((CBundle**)&r->mState);
        r->mState->SetAllowFds(FALSE);
        mInstrumentation->CallActivityOnSaveInstanceState(r->mActivity, r->mState);
    }

    HandleDestroyActivity(r->mToken, FALSE, configChanges, TRUE);

    r->mActivity = NULL;
    r->mWindow = NULL;
    r->mHideForNow = FALSE;
    r->mNextIdle = NULL;
    // Merge any pending results and pending intents; don't just replace them
    if (tmp->mPendingResults != NULL) {
        if (r->mPendingResults == NULL) {
            r->mPendingResults = tmp->mPendingResults;
        } else {
            List< AutoPtr<IResultInfo> >::Iterator it;
            for (it = tmp->mPendingResults->Begin(); it != tmp->mPendingResults->End(); ++it)
                r->mPendingResults->PushBack(*it);
        }
    }
    if (tmp->mPendingIntents != NULL) {
        if (r->mPendingIntents == NULL) {
            r->mPendingIntents = tmp->mPendingIntents;
        } else {
            List< AutoPtr<IIntent> >::Iterator it;
            for (it = tmp->mPendingIntents->Begin(); it != tmp->mPendingIntents->End(); ++it)
                r->mPendingIntents->PushBack(*it);
        }
    }
    r->mStartsNotResumed = tmp->mStartsNotResumed;

    HandleLaunchActivity(r, currentIntent);
    return NOERROR;
}

ECode CActivityThread::HandleRequestThumbnail(
    /* [in] */ IBinder* token)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    AutoPtr<IBitmap> thumbnail = CreateThumbnailBitmap(r);
    AutoPtr<ICharSequence> description;
//     try {
    r->mActivity->OnCreateDescription((ICharSequence**)&description);
//     } catch (Exception e) {
//         if (!mInstrumentation.onException(r.activity, e)) {
//             throw new RuntimeException(
//                     "Unable to create description of activity "
//                     + r.intent.getComponent().toShortString()
//                     + ": " + e.toString(), e);
//         }
//     }
//     //System.out.println("Reporting top thumbnail " + thumbnail);
//     try {
    ActivityManagerNative::GetDefault()->ReportThumbnail(
        token, thumbnail, description);
//     } catch (RemoteException ex) {
//     }
    return NOERROR;
}

AutoPtr< List<AutoPtr<IComponentCallbacks2> > > CActivityThread::CollectComponentCallbacks(
    /* [in] */ Boolean allActivities,
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr< List<AutoPtr<IComponentCallbacks2> > > callbacks = new List<AutoPtr<IComponentCallbacks2> >();

    {
        AutoLock lock(mPackagesLock);
        List<AutoPtr<IApplication> >::Iterator it;
        for (it = mAllApplications.Begin(); it != mAllApplications.End(); ++it) {
            callbacks->PushBack(IComponentCallbacks2::Probe(*it));
        }

        if (mActivities.Begin() != mActivities.End()) {
            HashMap<AutoPtr<IBinder>, AutoPtr<ActivityClientRecord> >::Iterator it;
            for (it = mActivities.Begin(); it != mActivities.End(); ++it) {
                ActivityClientRecord* ar = it->mSecond;
                AutoPtr<IActivity> a = ar != NULL ? ar->mActivity : NULL;
                if (a != NULL) {
                    AutoPtr<ICompatibilityInfo> compInfo;
                    ar->mPackageInfo->mCompatibilityInfo->GetIfNeeded((ICompatibilityInfo**)&compInfo);
                    AutoPtr<IConfiguration> thisConfig = ApplyConfigCompatMainThread(
                            mCurDefaultDisplayDpi, newConfig, compInfo);
                    Boolean isFinish;
                    ar->mActivity->IsFinishing(&isFinish);
                    if (!isFinish && (allActivities || !ar->mPaused)) {
                        // If the activity is currently resumed, its configuration
                        // needs to change right now.
                        callbacks->PushBack(IComponentCallbacks2::Probe(a));
                    }
                    else if (thisConfig != NULL) {
                        // Otherwise, we will tell it about the change
                        // the next time it is resumed or shown.  Note that
                        // the activity manager may, before then, decide the
                        // activity needs to be destroyed to handle its new
                        // configuration.
//                        if (DEBUG_CONFIGURATION) {
//                            Slogger::V(TAG, "Setting activity %s newConfig=%p"
//                                    , ar->mActivityInfo->mName.string(), thisConfig.Get());
//                        }
                        ar->mNewConfig = thisConfig;
                    }
                }
            }
        }
        if (mServices.Begin() != mServices.End()) {
            HashMap<AutoPtr<IBinder>, AutoPtr<IService> >:: Iterator it;
            for (it = mServices.Begin(); it != mServices.End(); ++it) {
                AutoPtr<IService> service = it->mSecond;
                callbacks->PushBack(IComponentCallbacks2::Probe(service));
            }
        }
    }
    {
        AutoLock lock(mProviderMapLock);
        if (mLocalProviders.Begin() != mLocalProviders.End()) {
            HashMap<AutoPtr<IBinder>, AutoPtr<ProviderClientRecord> >::Iterator it;
            for (it = mLocalProviders.Begin(); it != mLocalProviders.End(); ++it) {
                AutoPtr<ProviderClientRecord> providerClientRecord = it->mSecond;
                callbacks->PushBack(IComponentCallbacks2::Probe(providerClientRecord->mLocalProvider));
            }
        }
    }

    return callbacks;
}

ECode CActivityThread::PerformConfigurationChanged(
    /* [in] */ IComponentCallbacks2* cb,
    /* [in] */ IConfiguration* config)
{
    VALIDATE_NOT_NULL(cb);

    // Only for Activity objects, check that they actually call up to their
    // superclass implementation.  ComponentCallbacks2 is an interface, so
    // we check the runtime type and act accordingly.

    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(cb->Probe(EIID_Activity));
    if (activity != NULL) {
        activity->SetCalled(FALSE);
    }

    Boolean shouldChangeConfig = FALSE;
    if ((activity == NULL) || activity->mCurrentConfig == NULL) {
        shouldChangeConfig = TRUE;
    }
    else {
        // If the new config is the same as the config this Activity
        // is already running with then don't bother calling
        // onConfigurationChanged
        Int32 diff;
        activity->mCurrentConfig->Diff(config, &diff);
        if (diff != 0) {
            // If this activity doesn't handle any of the config changes
            // then don't bother calling onConfigurationChanged as we're
            // going to destroy it.
            Int32 configChange;
            activity->mActivityInfo->GetRealConfigChanged(&configChange);
            if ((~configChange & diff) == 0) {
                shouldChangeConfig = TRUE;
            }
        }
    }

    if (DEBUG_CONFIGURATION)
        Slogger::V(TAG, "Config callback %p: shouldChangeConfig=%d", cb, shouldChangeConfig);
    if (shouldChangeConfig) {
        cb->OnConfigurationChanged(config);

        if (activity != NULL) {
            Boolean isCalled;
            if (!(activity->IsCalled(&isCalled), isCalled)) {
                String activityName;
                AutoPtr<IComponentName> cn;
                activity->GetComponentName((IComponentName**)&cn);
                cn->ToShortString(&activityName);
                Slogger::E(TAG, "Activity %s did not call through to super.onConfigurationChanged().", activityName.string());
                return E_SUPER_NOT_CALLED_EXCEPTION;
//                 throw new SuperNotCalledException(
//                         "Activity " + activity.getLocalClassName() +
//                     " did not call through to super.onConfigurationChanged()");
                //return E_SUPER_NOT_CALLED_EXCEPTION;
            }
            activity->SetConfigChangeFlags(0);
            activity->mCurrentConfig = NULL;
            CConfiguration::New(config, (IConfiguration**)&activity->mCurrentConfig);
        }
    }
    return NOERROR;
}

ECode CActivityThread::ApplyConfigurationToResources(
    /* [in] */ IConfiguration* config)
{
    AutoLock lock(mPackagesLock);
    ApplyConfigurationToResourcesLocked(config, NULL);
    return NOERROR;
}

Boolean CActivityThread::ApplyConfigurationToResourcesLocked(
    /* [in] */ IConfiguration* config,
    /* [in] */ ICompatibilityInfo* compat)
{
    if (config == NULL) {
        return FALSE;
    }

    if (mResConfiguration == NULL) {
        CConfiguration::New((IConfiguration**)&mResConfiguration);
    }
    Boolean isBetter;
    mResConfiguration->IsOtherSeqNewer(config, &isBetter);
    if (!isBetter && compat == NULL) {
        Int32 curSeq;
        mResConfiguration->GetSeq(&curSeq);
        Int32 newSeq;
        config->GetSeq(&newSeq);
        if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Skipping new config: curSeq=%d, newSeq=%d"
                , curSeq, newSeq);
        return FALSE;
    }
    Int32 changes;
    mResConfiguration->UpdateFrom(config, &changes);
    FlushDisplayMetricsLocked();
    AutoPtr<IDisplayMetrics> defaultDisplayMetrics;
    GetDisplayMetricsLocked(
            IDisplay::DEFAULT_DISPLAY, NULL, (IDisplayMetrics**)&defaultDisplayMetrics);

    Boolean isEqual;
    if (compat != NULL && (mResCompatibilityInfo == NULL ||
            (mResCompatibilityInfo->Equals(compat, &isEqual), !isEqual))) {
        mResCompatibilityInfo = compat;
        changes |= IActivityInfo::CONFIG_SCREEN_LAYOUT
                | IActivityInfo::CONFIG_SCREEN_SIZE
                | IActivityInfo::CONFIG_SMALLEST_SCREEN_SIZE;
    }

    // set it for java, this also affects newly created Resources
    AutoPtr<ILocale> locale;
    config->GetLocale((ILocale**)&locale);
    if (locale != NULL) {
        AutoPtr<ILocaleHelper> localeHelper;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
        localeHelper->SetDefault(locale);
    }

    CResources::UpdateSystemConfiguration(config, defaultDisplayMetrics, compat);

    ApplicationPackageManager::ConfigurationChanged();
    //Slog.i(TAG, "Configuration changed in " + currentPackageName());

    AutoPtr<IConfiguration> tmpConfig;

    ActiveResourcesMapIterator it;
    for (it = mActiveResources.Begin(); it != mActiveResources.End();) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        AutoPtr<IResources> r;
        wr->Resolve(EIID_IResources, (IInterface**)&r);
        if (r != NULL) {
            if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Changing resources %p config to: %p"
                    , r.Get(), config);
            Int32 displayId = it->mFirst->mDisplayId;
            Boolean isDefaultDisplay = (displayId == IDisplay::DEFAULT_DISPLAY);
            AutoPtr<IDisplayMetrics> dm = defaultDisplayMetrics;
            AutoPtr<IConfiguration> overrideConfig = it->mFirst->mOverrideConfiguration;
            if (!isDefaultDisplay || overrideConfig != NULL) {
                if (tmpConfig == NULL) {
                    CConfiguration::New((IConfiguration**)&tmpConfig);
                }
                tmpConfig->SetTo(config);
                if (!isDefaultDisplay) {
                    dm = NULL;
                    GetDisplayMetricsLocked(displayId, NULL, (IDisplayMetrics**)&dm);
                    ApplyNonDefaultDisplayMetricsToConfigurationLocked(dm, tmpConfig);
                }
                if (overrideConfig != NULL) {
                    Int32 ch;
                    tmpConfig->UpdateFrom(overrideConfig, &ch);
                }
                r->UpdateConfiguration(tmpConfig, dm, compat);
            }
            else {
                r->UpdateConfiguration(config, dm, compat);
            }

            ++it;
            //Slog.i(TAG, "Updated app resources " + v.getKey()
            //        + " " + r + ": " + r.getConfiguration());
        }
        else {
            //Slog.i(TAG, "Removing old resources " + v.getKey());
            mActiveResources.Erase(it++);
        }
    }

    return changes != 0;
}

void CActivityThread::ApplyNonDefaultDisplayMetricsToConfigurationLocked(
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ IConfiguration* config)
{
    config->SetTouchscreen(IConfiguration::TOUCHSCREEN_NOTOUCH);
    Int32 densityD;
    dm->GetDensityDpi(&densityD);
    config->SetDensityDpi(densityD);
    Int32 widthP;
    dm->GetWidthPixels(&widthP);
    Float density;
    dm->GetDensity(&density);
    config->SetScreenWidthDp((Int32)(widthP / density));
    Int32 heightP;
    dm->GetHeightPixels(&heightP);
    config->SetScreenHeightDp((Int32)(heightP / density));
    Int32 sl;
    AutoPtr<IConfigurationHelper> confHelper;
    CConfigurationHelper::AcquireSingleton((IConfigurationHelper**)&confHelper);
    Int32 layout;
    config->GetScreenLayout(&layout);
    confHelper->ResetScreenLayout(layout, &sl);
    Int32 screenWidthDp, screenHeightDp;
    config->GetScreenWidthDp(&screenWidthDp);
    config->GetScreenHeightDp(&screenHeightDp);
    if (widthP > heightP) {
        config->SetOrientation(IConfiguration::ORIENTATION_LANDSCAPE);
        Int32 layout;
        confHelper->ReduceScreenLayout(sl, screenWidthDp, screenHeightDp, &layout);
        config->SetScreenLayout(layout);
    } else {
        config->SetOrientation(IConfiguration::ORIENTATION_PORTRAIT);
        Int32 layout;
        confHelper->ReduceScreenLayout(sl,screenHeightDp, screenWidthDp, &layout);
        config->SetScreenLayout(layout);
    }
    config->SetSmallestScreenWidthDp(screenWidthDp); // assume screen does not rotate
    config->SetCompatScreenWidthDp(screenWidthDp);
    config->SetCompatScreenHeightDp(screenHeightDp);
    Int32 smallestScreenWidthDp;
    config->GetSmallestScreenWidthDp(&smallestScreenWidthDp);
    config->SetCompatSmallestScreenWidthDp(smallestScreenWidthDp);
}

AutoPtr<IConfiguration> CActivityThread::ApplyCompatConfiguration(
    /* [in] */ Int32 displayDensity)
{
    AutoPtr<IConfiguration> config = mConfiguration;
    if (mCompatConfiguration == NULL) {
        CConfiguration::New((IConfiguration**)&mCompatConfiguration);
    }
    mCompatConfiguration->SetTo(mConfiguration);

    if (mResCompatibilityInfo != NULL) {
        Boolean isSupport;
        mResCompatibilityInfo->SupportsScreen(&isSupport);
        if (!isSupport) {
            mResCompatibilityInfo->ApplyToConfiguration(displayDensity, mCompatConfiguration);
            config = mCompatConfiguration;
        }
    }

    return config;
}

ECode CActivityThread::HandleConfigurationChanged(
    /* [in] */ IConfiguration* inConfig,
    /* [in] */ ICompatibilityInfo* compat)
{
    Int32 configDiff = 0;
    AutoPtr<IConfiguration> config = inConfig;

    {
        AutoLock lock(mPackagesLock);
        if (mPendingConfiguration != NULL) {
            Boolean isBetter;
            mPendingConfiguration->IsOtherSeqNewer(config, &isBetter);
            if (!isBetter) {
                config = mPendingConfiguration;
                config->GetDensityDpi(&mCurDefaultDisplayDpi);
                UpdateDefaultDensity();
            }
            mPendingConfiguration = NULL;
        }

        if (config == NULL) {
            return NOERROR;
        }

        if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Handle configuration changed: %p", config.Get());

        ApplyConfigurationToResourcesLocked(config, compat);

        if (mConfiguration == NULL) {
            CConfiguration::New((IConfiguration**)&mConfiguration);
        }
        Boolean isBetter;
        mConfiguration->IsOtherSeqNewer(config, &isBetter);
        if (!isBetter && compat == NULL) {
            return NOERROR;
        }
        mConfiguration->Diff(config, &configDiff);
        Int32 updateChanges;
        mConfiguration->UpdateFrom(config, &updateChanges);
        config = ApplyCompatConfiguration(mCurDefaultDisplayDpi);
    }

    AutoPtr< List<AutoPtr<IComponentCallbacks2> > > callbacks = CollectComponentCallbacks(FALSE, config);

    // Cleanup hardware accelerated stuff
//     WindowManagerGlobal.getInstance().trimLocalMemory();

    FreeTextLayoutCachesIfNeeded(configDiff);

    if (callbacks != NULL) {
        List<AutoPtr<IComponentCallbacks2> >::Iterator it;
        for (it = callbacks->Begin(); it != callbacks->End(); ++it) {
            PerformConfigurationChanged(*it, config);
        }
    }

    return NOERROR;
}

void CActivityThread::FreeTextLayoutCachesIfNeeded(
    /* [in] */ Int32 configDiff)
{
    if (configDiff != 0) {
        // Ask text layout engine to free its caches if there is a locale change
        Boolean hasLocaleConfigChange = ((configDiff & IActivityInfo::CONFIG_LOCALE) != 0);
        if (hasLocaleConfigChange) {
            Canvas::FreeTextLayoutCaches();
            if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Cleared TextLayout Caches");
        }
    }
}

ECode CActivityThread::HandleActivityConfigurationChanged(
    /* [in] */ IBinder* token)
{
    AutoPtr<ActivityClientRecord> r = GetActivityClientRecord(token);
    if (r == NULL || r->mActivity == NULL) {
        return NOERROR;
    }

//    if (DEBUG_CONFIGURATION) Slogger::V(TAG, "Handle activity config changed: %s"
//            , r->mActivityInfo->mName.string());

    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(r->mActivity->Probe(EIID_Activity));
    PerformConfigurationChanged(IComponentCallbacks2::Probe(r->mActivity), mCompatConfiguration);

    Int32 diffChanges;
    activity->mCurrentConfig->Diff(mCompatConfiguration, &diffChanges);
    FreeTextLayoutCachesIfNeeded(diffChanges);
    return NOERROR;
}

ECode CActivityThread::HandleProfilerControl(
    /* [in] */ Boolean start,
    /* [in] */ ProfilerControlData* pcd,
    /* [in] */ Int32 profileType)
{
    if (start) {
//         try {
        switch (profileType) {
            default:
                mProfiler->SetProfiler(pcd->mPath, pcd->mFd);
                mProfiler->mAutoStopProfiler = FALSE;
                mProfiler->StartProfiling();
                break;
        }
//         } catch (RuntimeException e) {
//             Slog.w(TAG, "Profiling failed on path " + pcd.path
//                     + " -- can the process access this path?");
//         } finally {
//             try {
        pcd->mFd->Close();
//             } catch (IOException e) {
//                 Slog.w(TAG, "Failure closing profile fd", e);
//             }
//         }
    } else {
        switch (profileType) {
            default:
                mProfiler->StopProfiling();
                break;
        }
    }

    return NOERROR;
}

ECode CActivityThread::HandleDumpHeap(
    /* [in] */ Boolean managed,
    /* [in] */ DumpHeapData* dhd)
{
    if (managed) {
//         try {
//             Debug.dumpHprofData(dhd.path, dhd.fd.getFileDescriptor());
//         } catch (IOException e) {
//             Slog.w(TAG, "Managed heap dump failed on path " + dhd.path
//                     + " -- can the process access this path?");
//         } finally {
//             try {
        dhd->mFd->Close();
//             } catch (IOException e) {
//                 Slog.w(TAG, "Failure closing profile fd", e);
//             }
//         }
    } else {
//         Debug.dumpNativeHeap(dhd.fd.getFileDescriptor());
    }
    return NOERROR;
}

ECode CActivityThread::HandleDispatchPackageBroadcast(
    /* [in] */ Int32 cmd,
    /* [in] */ ArrayOf<String>* packages)
{
    Boolean hasPkgInfo = FALSE;
    if (packages != NULL) {
        for (Int32 i = packages->GetLength() - 1; i >= 0; --i) {
            //Slogger::I(TAG, "Cleaning old package: %s", (*packages)[i].string());
            if (!hasPkgInfo) {
                AutoPtr<IWeakReference> wr = mPackages[(*packages)[i]];
                AutoPtr<IInterface> obj;
                if (wr != NULL)
                    wr->Resolve(EIID_IInterface, (IInterface**)&obj);
                if (obj != NULL) {
                    hasPkgInfo = TRUE;
                }
                else {
                    wr = mResourcePackages[(*packages)[i]];
                    if (wr != NULL) {
                        wr->Resolve(EIID_IInterface, (IInterface**)&obj);
                    }

                    if (obj != NULL) {
                        hasPkgInfo = TRUE;
                    }
                }
            }

            mPackages.Erase((*packages)[i]);
            mResourcePackages.Erase((*packages)[i]);
        }
    }
    ApplicationPackageManager::HandlePackageBroadcast(cmd, packages, hasPkgInfo);
    return NOERROR;
}

ECode CActivityThread::HandleLowMemory()
{
    AutoPtr< List<AutoPtr<IComponentCallbacks2> > > callbacks = CollectComponentCallbacks(TRUE, NULL);

    if (callbacks) {
        List<AutoPtr<IComponentCallbacks2> >::Iterator it;
        for (it = callbacks->Begin(); it != callbacks->End(); ++it) {
            (*it)->OnLowMemory();
        }
        callbacks = NULL;
    }

    // Ask SQLite to free up as much memory as it can, mostly from its page caches.
    if (Process::MyUid() != IProcess::SYSTEM_UID) {
        AutoPtr<ISQLiteDatabaseHelper> helper;
        CSQLiteDatabaseHelper::AcquireSingleton((ISQLiteDatabaseHelper**)&helper);
        Int32 sqliteReleased;
        helper->ReleaseMemory(&sqliteReleased);
//         EventLog.writeEvent(SQLITE_MEM_RELEASED_EVENT_LOG_TAG, sqliteReleased);
    }

    // Ask graphics to free up as much as possible (font/image caches)
    Canvas::FreeCaches();

    // Ask text layout engine to free also as much as possible
    Canvas::FreeTextLayoutCaches();

//     BinderInternal.forceGc("mem");
    return NOERROR;
}

ECode CActivityThread::HandleTrimMemory(
    /* [in] */ Int32 level)
{
    if (DEBUG_MEMORY_TRIM) Slogger::V(TAG, "Trimming memory to level: %d", level);

//     final WindowManagerGlobal windowManager = WindowManagerGlobal.getInstance();
//     windowManager.startTrimMemory(level);

    AutoPtr< List<AutoPtr<IComponentCallbacks2> > > callbacks = CollectComponentCallbacks(TRUE, NULL);

    if (callbacks) {
        List<AutoPtr<IComponentCallbacks2> >::Iterator it;
        for (it = callbacks->Begin(); it != callbacks->End(); ++it) {
            (*it)->OnTrimMemory(level);
        }
    }

//     windowManager.endTrimMemory();
    return NOERROR;
}

void CActivityThread::SetupGraphicsSupport(
    /* [in] */ LoadedPkg* info,
    /* [in] */ IFile* cacheDir)
{
    if (Process::IsIsolated()) {
        // Isolated processes aren't going to do UI.
        return;
    }
//     try {
        Int32 uid = Process::MyUid();
        AutoPtr<ArrayOf<String> > packages;
        GetPackageManager()->GetPackagesForUid(uid, (ArrayOf<String>**)&packages);

        // If there are several packages in this application we won't
        // initialize the graphics disk caches
        if (packages != NULL && packages->GetLength() == 1) {
//             HardwareRenderer.setupDiskCache(cacheDir);
//             RenderScript.setupDiskCache(cacheDir);
        }
//     } catch (RemoteException e) {
//         // Ignore
//     }
}

void CActivityThread::UpdateDefaultDensity()
{
    if (mCurDefaultDisplayDpi != IConfiguration::DENSITY_DPI_UNDEFINED
            && mCurDefaultDisplayDpi != CDisplayMetrics::DENSITY_DEVICE
            && !mDensityCompatMode) {
        Slogger::I(TAG, "Switching default density from %d to %d", CDisplayMetrics::DENSITY_DEVICE
                , mCurDefaultDisplayDpi);
        CDisplayMetrics::DENSITY_DEVICE = mCurDefaultDisplayDpi;
        CBitmap::SetDefaultDensity(IDisplayMetrics::DENSITY_DEFAULT);
    }
}

ECode CActivityThread::HandleBindApplication(
    /* [in] */ AppBindData* data)
{
    String wdir;
    data->mAppInfo->GetSourceDir(&wdir);
    chdir((const char*)wdir);

    mBoundApplication = data;
    mConfiguration = NULL;
    CConfiguration::New(data->mConfig, (IConfiguration**)&mConfiguration);
    mCompatConfiguration = NULL;
    CConfiguration::New(data->mConfig, (IConfiguration**)&mCompatConfiguration);

    mProfiler = new Profiler();
    mProfiler->mProfileFile = data->mInitProfileFile;
    mProfiler->mProfileFd = data->mInitProfileFd;
    mProfiler->mAutoStopProfiler = data->mInitAutoStopProfiler;

    // send up app name; do this *before* waiting for debugger
    Process::SetArgV0(data->mProcessName);
//    android.ddm.DdmHandleAppName.setAppName(data.processName, UserHandle.myUserId());

    if (data->mPersistent) {
        // Persistent processes on low-memory devices do not get to
        // use hardware accelerated drawing, since this can add too much
        // overhead to the process.
       if (!CActivityManager::IsHighEndGfx()) {
//            HardwareRenderer.disable(FALSE);
       }
    }

    if (mProfiler->mProfileFd != NULL) {
        mProfiler->StartProfiling();
    }

    // If the app is Honeycomb MR1 or earlier, switch its AsyncTask
    // implementation to use the pool executor.  Normally, we use the
    // serialized executor as the default. This has to happen in the
    // main thread so the main looper is set right.
    Int32 targetSdkVersion;
    FAIL_RETURN(data->mAppInfo->GetTargetSdkVersion(&targetSdkVersion));
    if (targetSdkVersion <= Build::VERSION_CODES::HONEYCOMB_MR1) {
        AsyncTask::SetDefaultExecutor(AsyncTask::THREAD_POOL_EXECUTOR);
    }

   /*
    * Before spawning a new process, reset the time zone to be the system time zone.
    * This needs to be done because the system time zone could have changed after the
    * the spawning of this process. Without doing this this process would have the incorrect
    * system time zone.
    */
    AutoPtr<ITimeZoneHelper> tzHelper;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzHelper);
    tzHelper->SetDefault(NULL);

    /*
     * Initialize the default locale in this process for the reasons we set the time zone.
     */
    AutoPtr<ILocale> lc;
    data->mConfig->GetLocale((ILocale**)&lc);
    AutoPtr<ILocaleHelper> localehelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localehelper);
    localehelper->SetDefault(lc);

    /*
     * Update the system configuration since its preloaded and might not
     * reflect configuration changes. The configuration object passed
     * in AppBindData can be safely assumed to be up to date
     */
    ApplyConfigurationToResourcesLocked(data->mConfig, data->mCompatInfo);
    data->mConfig->GetDensityDpi(&mCurDefaultDisplayDpi);
    ApplyCompatConfiguration(mCurDefaultDisplayDpi);

    data->mInfo = GetPackageInfoNoCheck(data->mAppInfo, data->mCompatInfo);

    /**
     * Switch this process to density compatibility mode if needed.
     */
    Int32 appInfoflags;
    data->mAppInfo->GetFlags(&appInfoflags);
    if ((appInfoflags & IApplicationInfo::FLAG_SUPPORTS_SCREEN_DENSITIES) == 0) {
        mDensityCompatMode = TRUE;
        CBitmap::SetDefaultDensity(IDisplayMetrics::DENSITY_DEFAULT);
    }
    UpdateDefaultDensity();

    AutoPtr<CContextImpl> appContext;
    CContextImpl::NewByFriend((CContextImpl**)&appContext);
    appContext->Init(data->mInfo, NULL, this);
    if (!Process::IsIsolated()) {
        AutoPtr<IFile> cacheDir;
        appContext->GetCacheDir((IFile**)&cacheDir);
        if (cacheDir != NULL) {
            // Provide a usable directory for temporary files
            AutoPtr<ISystem> system;
            Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
            String path, oldValue;
            cacheDir->GetAbsolutePath(&path);
            system->SetProperty(String("java.io.tmpdir"), path, &oldValue);

            SetupGraphicsSupport(data->mInfo, cacheDir);
        }
        else {
            Slogger::E(TAG, "Unable to setupGraphicsSupport due to missing cache directory");
        }
    }

    /**
     * For system applications on userdebug/eng builds, log stack
     * traces of disk and network access to dropbox for analysis.
     */
    if ((appInfoflags & (IApplicationInfo::FLAG_SYSTEM |
          IApplicationInfo::FLAG_UPDATED_SYSTEM_APP)) != 0) {
//        StrictMode.conditionallyEnableDebugLogging();
    }

    /**
     * For apps targetting SDK Honeycomb or later, we don't allow
     * network usage on the main event loop / UI thread.
     *
     * Note to those grepping:  this is what ultimately throws
     * NetworkOnMainThreadException ...
     */
    Int32 sdkVersion;
    data->mAppInfo->GetTargetSdkVersion(&sdkVersion);
    if (sdkVersion > 9) {
//        StrictMode.enableDeathOnNetwork();
    }

    if (data->mDebugMode != IApplicationThread::DEBUG_OFF) {
        // XXX should have option to change the port.
//        Debug.changeDebugPort(8100);
        if (data->mDebugMode == IApplicationThread::DEBUG_WAIT) {
            String cName;
            data->mInfo->GetPackageName(&cName);
            Slogger::W(TAG, "Application %s is waiting for the debugger on port 8100...", cName.string());

            AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
//            try {
            mgr->ShowWaitingForDebugger(mAppThread, TRUE);
//            } catch (RemoteException ex) {
//            }
//
//            Debug.waitForDebugger();
//
//            try {
            mgr->ShowWaitingForDebugger(mAppThread, FALSE);
//            } catch (RemoteException ex) {
//            }
//
        } else {
            String cName;
            data->mInfo->GetPackageName(&cName);
            Slogger::W(TAG, "Application %s can be debugged on port 8100...", cName.string());
        }
    }

    // Enable OpenGL tracing if required
    if (data->mEnableOpenGlTrace) {
//        GLUtils.enableTracing();
    }

    /**
     * Initialize the default http proxy in this process for the reasons we set the time zone.
     */
    AutoPtr<IInterface> b = ServiceManager::GetService(IContext::CONNECTIVITY_SERVICE);
    if (b != NULL) {
        // In pre-boot mode (doing initial launch to collect password), not
        // all system is up.  This includes the connectivity service, so don't
        // crash if we can't get it.
//         IConnectivityManager service = IConnectivityManager.Stub.asInterface(b);
//         try {
//             ProxyProperties proxyProperties = service.getProxy();
//             Proxy.setHttpProxySystemProperty(proxyProperties);
//         } catch (RemoteException e) {}
    }

    if (data->mInstrumentationName != NULL) {
        AutoPtr<IInstrumentationInfo> ii;
        GetPackageManager()->GetInstrumentationInfo(
                data->mInstrumentationName, 0, (IInstrumentationInfo**)&ii);
        if (ii == NULL) {
            String instName;
            data->mInstrumentationName->FlattenToShortString(&instName);
            Slogger::E(TAG, "Unable to find instrumentation info for: %s", instName.string());

            return E_RUNTIME_EXCEPTION;
        }

        CInstrumentationInfo* ci = (CInstrumentationInfo*)(IInstrumentationInfo*)ii;
        mInstrumentationAppDir = ci->mSourceDir;
        mInstrumentationAppLibraryDir = ci->mNativeLibraryDir;
        mInstrumentationAppPackage = ci->mPackageName;
        data->mInfo->GetAppDir(&mInstrumentedAppDir);
        data->mInfo->GetLibDir(&mInstrumentedAppLibraryDir);

        AutoPtr<CApplicationInfo> instrApp;
        CApplicationInfo::NewByFriend((CApplicationInfo**)&instrApp);
        instrApp->mPackageName = ci->mPackageName;
        instrApp->mSourceDir = ci->mSourceDir;
        instrApp->mPublicSourceDir = ci->mPublicSourceDir;
        instrApp->mDataDir = ci->mDataDir;
        AutoPtr<LoadedPkg> pi = GetPackageInfo(instrApp, data->mCompatInfo, NULL/*appContext.getClassLoader()*/, FALSE, TRUE);
        AutoPtr<CContextImpl> instrContext;
        CContextImpl::NewByFriend((CContextImpl**)&instrContext);
        instrContext->Init(pi, NULL, this);

//        try {
        String className;
        data->mInstrumentationName->GetClassName(&className);
        Int32 index = className.LastIndexOf('.');
        if (index < 0) {
            Slogger::E(TAG, "HandleBindApplication: Cann't Find the Instrumentation %s's path", className.string());
            return E_RUNTIME_EXCEPTION;
        }
        String path = className.Substring(0, index) + ".eco";
        AutoPtr<IModuleInfo> moduleInfo;
        if (FAILED(CReflector::AcquireModuleInfo(path.string(), (IModuleInfo**)&moduleInfo))) {
            Slogger::E(TAG, "HandleBindApplication: Cann't Find the Instrumentation path is %s", path.string());
            return E_RUNTIME_EXCEPTION;
        }

        String shortClassName = className.Substring(index + 1);
        AutoPtr<IClassInfo> classInfo;
        if (FAILED(moduleInfo->GetClassInfo(shortClassName, (IClassInfo**)&classInfo))) {
            Slogger::E(TAG, "HandleBindApplication: Get class info of %s failed.", shortClassName.string());
            return E_RUNTIME_EXCEPTION;
        }
        AutoPtr<IInterface> object;
        if (FAILED(classInfo->CreateObject((IInterface**)&object))) {
            Slogger::E(TAG, "HandleBindApplication: Create Instrumentation object failed.");
            return E_RUNTIME_EXCEPTION;
        }
        mInstrumentation = IInstrumentation::Probe(object);
//        } catch (Exception e) {
//            throw new RuntimeException(
//                "Unable to instantiate instrumentation "
//                + data.instrumentationName + ": " + e.toString(), e);
//        }

        String cName, name;
        ii->GetPackageName(&cName);
        ii->GetName(&name);
        AutoPtr<IComponentName> component;
        CComponentName::New(cName, name, (IComponentName**)&component);
        mInstrumentation->Init(this, instrContext, appContext, component, data->mInstrumentationWatcher);

        Boolean isHandle;
        ii->GetHandleProfiling(&isHandle);
        if (mProfiler->mProfileFile != NULL && !isHandle && mProfiler->mProfileFd == NULL) {
            mProfiler->mHandlingProfiling = TRUE;
            AutoPtr<IFile> file;
//TODO
//            CFile::New(mProfiler->mProfileFile, (IFile**)&file);
            AutoPtr<IFile> parentfile;
            file->GetParentFile((IFile**)&parentfile);
            Boolean hasdirs;
            parentfile->Mkdirs(&hasdirs);
//            Debug.startMethodTracing(file.toString(), 8 * 1024 * 1024);
        }
    }
    else {
        mInstrumentation = NULL;
        CInstrumentation::New((IInstrumentation**)&mInstrumentation);
    }

    if ((appInfoflags & IApplicationInfo::FLAG_LARGE_HEAP) != 0) {
//         dalvik.system.VMRuntime.getRuntime().clearGrowthLimit();
    }

    // Allow disk access during application and provider setup. This could
    // block processing ordered broadcasts, but later processing would
    // probably end up doing the same disk access.
//     final StrictMode.ThreadPolicy savedPolicy = StrictMode.allowThreadDiskWrites();
//     try {
//         // If the app is being launched for full backup or restore, bring it up in
//         // a restricted environment with the base application class.
    AutoPtr<IApplication> app;
    data->mInfo->MakeApplication(data->mRestrictedBackupMode, NULL, (IApplication**)&app);
    mInitialApplication = app;

    // don't bring up providers in restricted mode; they may depend on the
    // app's custom Application class
    if (!data->mRestrictedBackupMode) {
        AutoPtr<IObjectContainer> providers = data->mProviders;
        if (providers != NULL) {
            InstallContentProviders(app, providers);
            // For process that contains content providers, we want to
            // ensure that the JIT is enabled "at some point".
//                mH.sendEmptyMessageDelayed(H.ENABLE_JIT, 10*1000);
        }
    }

    // Do this after providers, since instrumentation tests generally start their
    // test thread at this point, and we don't want that racing.

//        try {
     ECode ec = mInstrumentation->OnCreate(data->mInstrumentationArgs);
//        }
//        catch (Exception e) {
//            throw new RuntimeException(
//                "Exception thrown in onCreate() of "
//                + data.instrumentationName + ": " + e.toString(), e);
//        }
    if(FAILED(ec)) {
        String instName;
        if (data->mInstrumentationName != NULL) {
            data->mInstrumentationName->FlattenToShortString(&instName);
        }
        else {
            data->mInfo->GetPackageName(&instName);
        }
        Slogger::E(TAG, "Exception thrown in onCreate() of %s, error: 0x%08x", instName.string(), ec);
        return E_RUNTIME_EXCEPTION;
    }

    ec = mInstrumentation->CallApplicationOnCreate(app);
    if (FAILED(ec)) {
        String instName;
        if (data->mInstrumentationName != NULL) {
            data->mInstrumentationName->FlattenToShortString(&instName);
        }
        else {
            data->mInfo->GetPackageName(&instName);
        }
        Slogger::E(TAG, "Unable to create application %s, error: 0x%08x", instName.string(), ec);
        return E_RUNTIME_EXCEPTION;
    }
//    StrictMode.setThreadPolicy(savedPolicy);

    return NOERROR;
}

/*package*/
ECode CActivityThread::FinishInstrumentation(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (mProfiler->mProfileFile != NULL && mProfiler->mHandlingProfiling
            && mProfiler->mProfileFd == NULL) {
//         Debug.stopMethodTracing();
    }
    //Slog.i(TAG, "am: " + ActivityManagerNative.getDefault()
    //      + ", app thr: " + mAppThread);
//     try {
    am->FinishInstrumentation(mAppThread, resultCode, results);
//     } catch (RemoteException ex) {
//     }
    return NOERROR;
}

ECode CActivityThread::InstallContentProviders(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* providers)
{
    AutoPtr<IObjectContainer> results;
    CParcelableObjectContainer::New((IObjectContainer**)&results);

    Boolean hasNext = FALSE;
    AutoPtr<IObjectEnumerator> enumerator;
    providers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IProviderInfo> cpi;
        enumerator->Current((IInterface**)&cpi);
        if (DEBUG_PROVIDER) {
            StringBuilder buf(128);
            buf.AppendCStr("Pub ");
            String auth;
            cpi->GetAuthority(&auth);
            buf.AppendString(auth);
            buf.AppendCStr(": ");
            String name;
            cpi->GetName(&name);
            buf.AppendString(name);
            Slogger::I(TAG, "%s", buf.ToString().string());
        }
        // StringBuffer buf(128);
        // buf += "Pub ";
        // buf += cpi->mAuthority;
        // buf += ": ";
        // buf += cpi->mName;
        // Slogger::I(TAG, buf->ToString());
        AutoPtr<IContentProviderHolder> cph = InstallProvider(context, NULL, cpi,
                FALSE /*noisy*/, TRUE /*noReleaseNeeded*/, TRUE /*stable*/);
        if (cph != NULL) {
            cph->SetNoReleaseNeeded(TRUE);
            results->Add(cph.Get());
        }
    }

    AutoPtr<IApplicationThread> appThread;
    GetApplicationThread((IApplicationThread**)&appThread);
    return ActivityManagerNative::GetDefault()->PublishContentProviders(appThread, results);
}

ECode CActivityThread::AcquireProvider(
    /* [in] */ IContext* c,
    /* [in] */ const String& auth,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean stable,
    /* [out] */ IIContentProvider** pr)
{
    VALIDATE_NOT_NULL(pr);
    *pr = NULL;

    AutoPtr<IIContentProvider> provider;
    AcquireExistingProvider(c, auth, userId, stable, (IIContentProvider**)&provider);
    if (provider != NULL) {
        *pr = provider;
        REFCOUNT_ADD(*pr);
        return NOERROR;
    }

    // There is a possible race here.  Another thread may try to acquire
    // the same provider at the same time.  When this happens, we want to ensure
    // that the first one wins.
    // Note that we cannot hold the lock while acquiring and installing the
    // provider since it might take a long time to run and it could also potentially
    // be re-entrant in the case where the provider is in the same process.
    AutoPtr<IContentProviderHolder> holder;
//     try {
    AutoPtr<IApplicationThread> appThread;
    GetApplicationThread((IApplicationThread**)&appThread);
    ActivityManagerNative::GetDefault()->GetContentProvider(
            appThread, auth, userId,
            stable, (IContentProviderHolder**)&holder);
//     } catch (RemoteException ex) {
//     }
    if (holder == NULL) {
        Slogger::E(TAG, "Failed to find provider info for %s", auth.string());
        return NOERROR;
    }

    // Install provider will increment the reference count for us, and break
    // any ties in the race.
    Boolean noNeeded;
    holder->GetNoReleaseNeeded(&noNeeded);
    AutoPtr<IProviderInfo> pInfo;
    holder->GetProviderInfo((IProviderInfo**)&pInfo);
    AutoPtr<IContentProviderHolder> retholder = InstallProvider(c, holder, pInfo,
            TRUE /*noisy*/, noNeeded, stable);
    if (retholder) {
        retholder->GetContentProvider(pr);
    }
    else {
        Slogger::E(TAG, "Failed to find provider info for %s, userId %d", auth.string(), userId);
    }
    return NOERROR;
}

void CActivityThread::IncProviderRefLocked(
    /* [in] */ ProviderRefCount* prc,
    /* [in] */ Boolean stable)
{
    if (stable) {
        prc->mStableCount += 1;
        if (prc->mStableCount == 1) {
            // We are acquiring a new stable reference on the provider.
            Int32 unstableDelta;
            if (prc->mRemovePending) {
                // We have a pending remove operation, which is holding the
                // last unstable reference.  At this point we are converting
                // that unstable reference to our new stable reference.
                unstableDelta = -1;
                // Cancel the removal of the provider.
                if (DEBUG_PROVIDER) {
                    Slogger::V(TAG, "incProviderRef: stable snatched provider from the jaws of death");
                }
                prc->mRemovePending = FALSE;
                mH->RemoveMessages(H::REMOVE_PROVIDER, prc);
            } else {
                unstableDelta = 0;
            }
//             try {
                if (DEBUG_PROVIDER) {
                    AutoPtr<IProviderInfo> pInfo;
                    prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
                    String name;
                    pInfo->GetName(&name);
                    Slogger::V(TAG, "incProviderRef Now stable - %s: unstableDelta=%d"
                        , name.string(), unstableDelta);
                }
                AutoPtr<IBinder> connection;
                prc->mHolder->GetConnection((IBinder**)&connection);
                Boolean res;
                ActivityManagerNative::GetDefault()->RefContentProvider(
                    connection, 1, unstableDelta, &res);
//             } catch (RemoteException e) {
//                 //do nothing content provider object is dead any way
//             }
        }
    }
    else {
        prc->mUnstableCount += 1;
        if (prc->mUnstableCount == 1) {
            // We are acquiring a new unstable reference on the provider.
            if (prc->mRemovePending) {
                // Oh look, we actually have a remove pending for the
                // provider, which is still holding the last unstable
                // reference.  We just need to cancel that to take new
                // ownership of the reference.
                if (DEBUG_PROVIDER) {
                    Slogger::V(TAG, "incProviderRef: unstable snatched provider from the jaws of death");
                }
                prc->mRemovePending = FALSE;
                mH->RemoveMessages(H::REMOVE_PROVIDER, prc);
            }
            else {
                // First unstable ref, increment our count in the
                // activity manager.
//                 try {
                    if (DEBUG_PROVIDER) {
                        AutoPtr<IProviderInfo> pInfo;
                        prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
                        String name;
                        pInfo->GetName(&name);
                        Slogger::V(TAG, "incProviderRef: Now unstable - %s", name.string());
                    }
                    AutoPtr<IBinder> connection;
                    prc->mHolder->GetConnection((IBinder**)&connection);
                    Boolean res;
                    ActivityManagerNative::GetDefault()->RefContentProvider(
                            connection, 0, 1, &res);
//                 } catch (RemoteException e) {
//                     //do nothing content provider object is dead any way
//                 }
            }
        }
    }
}

ECode CActivityThread::AcquireExistingProvider(
    /* [in] */ IContext* ctx,
    /* [in] */ const String& auth,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean stable,
    /* [out] */ IIContentProvider** cpr)
{
    AutoLock lock(mProviderMapLock);

    AutoPtr<ProviderKey> key = new ProviderKey(auth, userId);
    AutoPtr<ProviderClientRecord> pr;
    HashMap< AutoPtr<ProviderKey>, AutoPtr<ProviderClientRecord>, HashPK, PKEq >::Iterator it =
            mProviderMap.Find(key);
    if (it != mProviderMap.End()) {
        pr = it->mSecond;
    }
    if (pr == NULL) {
        *cpr  = NULL;
        return NOERROR;
    }

    AutoPtr<IIContentProvider> provider = pr->mProvider;
    IBinder* jBinder = IBinder::Probe(provider);
    IProxy* proxy = (IProxy*)provider->Probe(EIID_IProxy);
    Boolean alive;
    if (proxy != NULL && (proxy->IsStubAlive(&alive), !alive)) {
        // The hosting process of the provider has died; we can't
        // use this one.
        Slogger::I(TAG, "Acquiring provider %s for user %d: existing object's process dead",
                auth.string(), userId);
        ECode ec = HandleUnstableProviderDiedLocked(jBinder, TRUE);
        *cpr = NULL;
        return ec;
    }

    // Only increment the ref count if we have one.  If we don't then the
    // provider is not reference counted and never needs to be released.
    AutoPtr<ProviderRefCount> prc;
    HashMap< AutoPtr<IBinder>, AutoPtr<ProviderRefCount> >::Iterator cit =
            mProviderRefCountMap.Find(jBinder);
    if (cit != mProviderRefCountMap.End()) {
        prc = cit->mSecond;
    }
    if (prc != NULL) {
        IncProviderRefLocked(prc, stable);
    }
    *cpr = provider;
    REFCOUNT_ADD(*cpr);
    return NOERROR;
}

ECode CActivityThread::ReleaseProvider(
    /* [in] */ IIContentProvider* provider,
    /* [in] */ Boolean stable,
    /* [out] */ Boolean* released)
{
    if (provider == NULL) {
        *released = FALSE;
        return NOERROR;
    }

    IBinder* jBinder = IBinder::Probe(provider);
    {
        AutoLock lock(mProviderMapLock);
        AutoPtr<ProviderRefCount> prc;
        HashMap< AutoPtr<IBinder>, AutoPtr<ProviderRefCount> >::Iterator it =
                mProviderRefCountMap.Find(jBinder);
        if (it != mProviderRefCountMap.End()) {
            prc = it->mSecond;
        }
        if (prc == NULL) {
            // The provider has no ref count, no release is needed.
            *released = FALSE;
            return NOERROR;
        }

        Boolean lastRef = FALSE;
        if (stable) {
            if (prc->mStableCount == 0) {
                if (DEBUG_PROVIDER) Slogger::V(TAG,
                        "releaseProvider: stable ref count already 0, how?");
                *released = FALSE;
                return NOERROR;
            }
            prc->mStableCount -= 1;
            if (prc->mStableCount == 0) {
                // What we do at this point depends on whether there are
                // any unstable refs left: if there are, we just tell the
                // activity manager to decrement its stable count; if there
                // aren't, we need to enqueue this provider to be removed,
                // and convert to holding a single unstable ref while
                // doing so.
                lastRef = prc->mUnstableCount == 0;
//                 try {
                    if (DEBUG_PROVIDER) {
                        AutoPtr<IProviderInfo> pInfo;
                        prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
                        String name;
                        pInfo->GetName(&name);
                        Slogger::V(TAG, "releaseProvider: No longer stable w/lastRef=%d - %s"
                                , lastRef, name.string());
                    }
                    AutoPtr<IBinder> connection;
                    prc->mHolder->GetConnection((IBinder**)&connection);
                    Boolean res;
                    ActivityManagerNative::GetDefault()->RefContentProvider(
                            connection, -1, lastRef ? 1 : 0, &res);
//                 } catch (RemoteException e) {
//                     //do nothing content provider object is dead any way
//                 }
            }
        } else {
            if (prc->mUnstableCount == 0) {
                if (DEBUG_PROVIDER) Slogger::V(TAG,
                        "releaseProvider: unstable ref count already 0, how?");
                *released = FALSE;
                return NOERROR;
            }
            prc->mUnstableCount -= 1;
            if (prc->mUnstableCount == 0) {
                // If this is the last reference, we need to enqueue
                // this provider to be removed instead of telling the
                // activity manager to remove it at this point.
                lastRef = prc->mStableCount == 0;
                if (!lastRef) {
//                     try {
                        if (DEBUG_PROVIDER) {
                            AutoPtr<IProviderInfo> pInfo;
                            prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
                            String name;
                            pInfo->GetName(&name);
                            Slogger::V(TAG, "releaseProvider: No longer unstable - %s"
                                    , name.string());
                        }
                        AutoPtr<IBinder> connection;
                        prc->mHolder->GetConnection((IBinder**)&connection);
                        Boolean res;
                        ActivityManagerNative::GetDefault()->RefContentProvider(
                                connection, 0, -1, &res);
//                     } catch (RemoteException e) {
//                         //do nothing content provider object is dead any way
//                     }
                }
            }
        }

        if (lastRef) {
            if (!prc->mRemovePending) {
                // Schedule the actual remove asynchronously, since we don't know the context
                // this will be called in.
                // TODO: it would be nice to post a delayed message, so
                // if we come back and need the same provider quickly
                // we will still have it available.
                if (DEBUG_PROVIDER) {
                    AutoPtr<IProviderInfo> pInfo;
                    prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
                    String name;
                    pInfo->GetName(&name);
                    Slogger::V(TAG, "releaseProvider: Enqueueing pending removal - %s"
                            , name.string());
                }
                prc->mRemovePending = TRUE;
                AutoPtr<IMessage> msg;
                mH->ObtainMessage(H::REMOVE_PROVIDER, prc, (IMessage**)&msg);
                Boolean bval;
                mH->SendMessage(msg, &bval);
            }
            else {
                AutoPtr<IProviderInfo> pInfo;
                prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
                String name;
                pInfo->GetName(&name);
                Slogger::W(TAG, "Duplicate remove pending of provider %s", name.string());
            }
        }
        *released = TRUE;
        return NOERROR;
    }
}

ECode CActivityThread::CompleteRemoveProvider(
    /* [in] */ ProviderRefCount* prc)
{
    {
        AutoLock lock(mProviderMapLock);
        if (!prc->mRemovePending) {
            // There was a race!  Some other client managed to acquire
            // the provider before the removal was completed.
            // Abort the removal.  We will do it later.
            String ss("completeRemoveProvider: lost the race,");
            ss += " provider still in use";
            if (DEBUG_PROVIDER) Slogger::V(TAG, ss.string());
            return NOERROR;
        }

        AutoPtr<IIContentProvider> provider;
        prc->mHolder->GetContentProvider((IIContentProvider**)&provider);
        IBinder* jBinder = IBinder::Probe(provider);
        AutoPtr<ProviderRefCount> existingPrc;
        HashMap< AutoPtr<IBinder>, AutoPtr<ProviderRefCount> >::Iterator it =
                mProviderRefCountMap.Find(jBinder);
        if (it != mProviderRefCountMap.End()) {
            prc = it->mSecond;
        }
        if (existingPrc.Get() == prc) {
            mProviderRefCountMap.Erase(it);
        }

        HashMap< AutoPtr<ProviderKey>, AutoPtr<ProviderClientRecord>, HashPK, PKEq >::Iterator pit =
                mProviderMap.Begin();
        while (pit != mProviderMap.End()) {
            AutoPtr<ProviderClientRecord> pr = pit->mSecond;
            IBinder* myBinder = IBinder::Probe(pr->mProvider);
            if (myBinder == jBinder) {
                mProviderMap.Erase(pit++);
            }
            else {
                ++pit;
            }
        }
    }

//     try {
    if (DEBUG_PROVIDER) {
        AutoPtr<IProviderInfo> pInfo;
        prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
        String name;
        pInfo->GetName(&name);
        Slogger::V(TAG, "removeProvider: Invoking ActivityManagerNative.removeContentProvider(%s)"
            , name.string());
    }
    AutoPtr<IBinder> connection;
    prc->mHolder->GetConnection((IBinder**)&connection);
    ActivityManagerNative::GetDefault()->RemoveContentProvider(connection, FALSE);
//     } catch (RemoteException e) {
//         //do nothing content provider object is dead any way
//     }
    return NOERROR;
}

ECode CActivityThread::HandleUnstableProviderDied(
    /* [in] */ IBinder* provider,
    /* [in] */ Boolean fromClient)
{
    AutoLock lock(mProviderMapLock);
    return HandleUnstableProviderDiedLocked(provider, fromClient);
}

ECode CActivityThread::HandleUnstableProviderDiedLocked(
    /* [in] */ IBinder* provider,
    /* [in] */ Boolean fromClient)
{
    AutoPtr<ProviderRefCount> prc;
    HashMap< AutoPtr<IBinder>, AutoPtr<ProviderRefCount> >::Iterator it =
            mProviderRefCountMap.Find(provider);
    if (it != mProviderRefCountMap.End()) {
        prc = it->mSecond;
    }
    if (prc != NULL) {
        AutoPtr<IProviderInfo> pInfo;
        prc->mHolder->GetProviderInfo((IProviderInfo**)&pInfo);
        String name;
        pInfo->GetName(&name);
        if (DEBUG_PROVIDER) {
            Slogger::V(TAG, "Cleaning up dead provider %p %s", provider, name.string());
        }
        mProviderRefCountMap.Erase(it);
        if (prc->mClient != NULL && prc->mClient->mNames != NULL) {
            for (Int32 i = 0; i < prc->mClient->mNames->GetLength(); ++i) {
                String name = (*prc->mClient->mNames)[i];
                AutoPtr<ProviderKey> key = new ProviderKey(name, 0);// TODO: source code maybe wrong.
                AutoPtr<ProviderClientRecord> pr;
                HashMap< AutoPtr<ProviderKey>, AutoPtr<ProviderClientRecord>, HashPK, PKEq >::Iterator it =
                        mProviderMap.Find(key);
                if (it != mProviderMap.End()) {
                    pr = it->mSecond;
                }
                if (pr != NULL && IBinder::Probe(pr->mProvider) == provider) {
                    Slogger::I(TAG, "Removing dead content provider: %s", name.string());
                    mProviderMap.Erase(it);
                }
            }
        }

        if (fromClient) {
            // We found out about this due to execution in our client
            // code.  Tell the activity manager about it now, to ensure
            // that the next time we go to do anything with the provider
            // it knows it is dead (so we don't race with its death
            // notification).
//             try {
            AutoPtr<IBinder> connection;
            prc->mHolder->GetConnection((IBinder**)&connection);
                ActivityManagerNative::GetDefault()->UnstableProviderDied(connection);
//             } catch (RemoteException e) {
//                 //do nothing content provider object is dead any way
//             }
        }
    }
    return NOERROR;
}

AutoPtr<CActivityThread::ProviderClientRecord> CActivityThread::InstallProviderAuthoritiesLocked(
    /* [in] */ IIContentProvider* provider,
    /* [in] */ IContentProvider* localProvider,
    /* [in] */ IContentProviderHolder* holder)
{
    AutoPtr<IProviderInfo> info;
    holder->GetProviderInfo((IProviderInfo**)&info);
    String authority;
    info->GetAuthority(&authority);

    AutoPtr<ArrayOf<String> > auths;
    PATTERN_SEMICOLON->Split(authority, (ArrayOf<String>**)&auths);

    AutoPtr<IApplicationInfo> appInfo;
    info->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    Int32 userId = UserHandle::GetUserId(uid);

    AutoPtr<ProviderClientRecord> pcr = new ProviderClientRecord(auths, provider, localProvider, holder);
    HashMap< AutoPtr<ProviderKey>, AutoPtr<ProviderClientRecord>, HashPK, PKEq >::Iterator it;
    if (auths != NULL) {
        for (Int32 i = 0; i < auths->GetLength(); ++i) {
            String auth = (*auths)[i];
            AutoPtr<ProviderKey> key = new ProviderKey(auth, userId);
            AutoPtr<ProviderClientRecord> existing;
            it = mProviderMap.Find(key);
            if (it != mProviderMap.End()) {
                existing = it->mSecond;
            }
            if (existing != NULL) {
                AutoPtr<IProviderInfo> info;
                pcr->mHolder->GetProviderInfo((IProviderInfo**)&info);
                String name;
                if (info) {
                    info->GetName(&name);
                }
                Slogger::W(TAG, "Content provider %s already published as %s", name.string(), auth.string());
            }
            else {
                mProviderMap[key] = pcr;
            }
        }
    }

    return pcr;
}

/**
 * Installs the provider.
 *
 * Providers that are local to the process or that come from the system server
 * may be installed permanently which is indicated by setting noReleaseNeeded to TRUE.
 * Other remote providers are reference counted.  The initial reference count
 * for all reference counted providers is one.  Providers that are not reference
 * counted do not have a reference count (at all).
 *
 * This method detects when a provider has already been installed.  When this happens,
 * it increments the reference count of the existing provider (if appropriate)
 * and returns the existing provider.  This can happen due to concurrent
 * attempts to acquire the same provider.
 */
AutoPtr<IContentProviderHolder> CActivityThread::InstallProvider(
    /* [in] */ IContext* context,
    /* [in] */ IContentProviderHolder* _holder,
    /* [in] */ IProviderInfo* info,
    /* [in] */ Boolean noisy,
    /* [in] */ Boolean noReleaseNeeded,
    /* [in] */ Boolean stable)
{
    String name, authority;
    info->GetName(&name);
    info->GetAuthority(&authority);

    AutoPtr<IContentProviderHolder> holder = _holder;
    AutoPtr<IContentProvider> localProvider;
    AutoPtr<IIContentProvider> provider, holderProvider;
    if (holder == NULL ||
        (holder->GetContentProvider((IIContentProvider**)&holderProvider), holderProvider == NULL)) {
        if (DEBUG_PROVIDER || noisy) {
            Slogger::D(TAG, "Loading provider %s: %s", authority.string(), name.string());
        }

        AutoPtr<IApplicationInfo> ai;
        info->GetApplicationInfo((IApplicationInfo**)&ai);
        String pkgName, appPkgName;
        context->GetPackageName(&pkgName);
        ai->GetPackageName(&appPkgName);

        Boolean isEqualInitAppPkgName = FALSE;
        if (mInitialApplication != NULL) {
            String initAppPkgName;
            mInitialApplication->GetPackageName(&initAppPkgName);
            isEqualInitAppPkgName = initAppPkgName.Equals(appPkgName);
        }

        AutoPtr<IContext> c;
        if (pkgName.Equals(appPkgName)) {
            c = context;
        }
        else if (isEqualInitAppPkgName) {
            c = mInitialApplication;
        }
        else {
//             try {
            context->CreatePackageContext(appPkgName, IContext::CONTEXT_INCLUDE_CODE, (IContext**)&c);
//             } catch (PackageManager.NameNotFoundException e) {
//                 // Ignore
//             }
        }

        if (c == NULL) {
            Slogger::W(TAG, "Unable to get context for package %s while loading content provider %s",
                appPkgName.string(), name.string());
            return NULL;
        }
//         try {
//             final java.lang.ClassLoader cl = c.getClassLoader();

        String packagePath, className;
        if (name.Equals("com.android.providers.settings.SettingsProvider")) {
            packagePath = String("/data/elastos/SettingsProvider.eco");
            className = String("CSettingsProvider");
        }
        else {
            Int32 index = name.LastIndexOf('.');
            String packageName = name.Substring(0, index);
            StringBuilder sb;
            sb.Append("/data/elastos/");
            sb.Append(packageName);
            sb.Append(".eco");

            packagePath = sb.ToString();
            className = name.Substring(index + 1);
        }

        AutoPtr<IModuleInfo> moduleInfo;
        ECode ec = CReflector::AcquireModuleInfo(packagePath.string(), (IModuleInfo**)&moduleInfo);
        if (FAILED(ec)) {
            Slogger::E(TAG, "InstallProvider: Cann't Find the path is %s", packagePath.string());
            return NULL;
        }

        AutoPtr<IClassInfo> classInfo;
        ec = moduleInfo->GetClassInfo(className, (IClassInfo**)&classInfo);
        if (FAILED(ec)) {
            Slogger::E(TAG, "InstallProvider: Get class info of %s failed.", className.string());
            return NULL;
        }
        AutoPtr<IInterface> object;
        ec = classInfo->CreateObject((IInterface**)&object);
        if (FAILED(ec)) {
            Slogger::E(TAG, "InstallProvider: Create ContentProvider object failed.");
            return NULL;
        }

        localProvider = IContentProvider::Probe(object);
        localProvider->GetIContentProvider((IIContentProvider**)&provider);
        if (provider == NULL) {
            String sourceDir;
            ai->GetSourceDir(&sourceDir);
            Slogger::E(TAG, "Failed to instantiate class %s from sourceDir ",
                name.string(), sourceDir.string());
            return NULL;
        }

        if (DEBUG_PROVIDER) {
            Slogger::V(TAG, "Instantiating local provider %s", name.string());
        }

        // XXX Need to create the correct context for this provider.
        ec = localProvider->AttachInfo(c, info);
        if (FAILED(ec)) {
            Slogger::E(TAG, "Unable to get provider %s, ec=%08x", name.string(), ec);
            return NULL;
        }
//         } catch (java.lang.Exception e) {
//             if (!mInstrumentation.onException(null, e)) {
//                 throw new RuntimeException(
//                         "Unable to get provider " + info.name
//                         + ": " + e.toString(), e);
//             }
//             return null;
//         }
    }
    else {
        provider = holderProvider;
        if (DEBUG_PROVIDER) {
            Slogger::V(TAG, "Installing external provider %s: %s",
                authority.string(), name.string());
        }
    }

    AutoPtr<IContentProviderHolder> retHolder;

    {
        AutoLock lock(mProviderMapLock);

        if (DEBUG_PROVIDER) {
            Slogger::V(TAG, "Checking to add %p / %s", provider.Get(), name.string());
        }
        AutoPtr<IBinder> jBinder = IBinder::Probe(provider);
        if (localProvider != NULL) {
            String pkgName;
            info->GetPackageName(&pkgName);
            AutoPtr<IComponentName> cname;
            CComponentName::New(pkgName, name, (IComponentName**)&cname);
            AutoPtr<ProviderClientRecord> pr;
            HashMap<AutoPtr<IComponentName>, AutoPtr<ProviderClientRecord> >::Iterator it =
                    mLocalProvidersByName.Find(cname);
            if (it != mLocalProvidersByName.End()) {
                pr = it->mSecond;
            }
            if (pr != NULL) {
                if (DEBUG_PROVIDER) {
                    Slogger::V(TAG, "installProvider: lost the race, using existing local provider");
                }
                provider = pr->mProvider;
            }
            else {
                holder = NULL;
                CContentProviderHolder::New(info, (IContentProviderHolder**)&holder);
                holder->SetContentProvider(provider);
                holder->SetNoReleaseNeeded(TRUE);
                pr = InstallProviderAuthoritiesLocked(provider, localProvider, holder);
                mLocalProviders[jBinder] = pr;
                mLocalProvidersByName[cname] = pr;
            }
            retHolder = pr->mHolder;
        }
        else {
            AutoPtr<ProviderRefCount> prc;
            HashMap< AutoPtr<IBinder>, AutoPtr<ProviderRefCount> >::Iterator it =
                    mProviderRefCountMap.Find(jBinder);
            if (it != mProviderRefCountMap.End()) {
                prc = it->mSecond;
            }
            if (prc != NULL) {
                if (DEBUG_PROVIDER) {
                    Slogger::V(TAG, "installProvider: lost the race, updating ref count");
                }
                // We need to transfer our new reference to the existing
                // ref count, releasing the old one...  but only if
                // release is needed (that is, it is not running in the
                // system process).
                if (!noReleaseNeeded) {
                    IncProviderRefLocked(prc, stable);
//                     try {
                    AutoPtr<IBinder> connection;
                    holder->GetConnection((IBinder**)&connection);
                    ActivityManagerNative::GetDefault()->RemoveContentProvider(
                            connection, stable);
//                     } catch (RemoteException e) {
//                         //do nothing content provider object is dead any way
//                     }
                }
            }
            else {
                AutoPtr<ProviderClientRecord> client = InstallProviderAuthoritiesLocked(
                        provider, localProvider, holder);
                if (noReleaseNeeded) {
                    prc = new ProviderRefCount(holder, client, 1000, 1000);
                }
                else {
                    prc = stable
                            ? new ProviderRefCount(holder, client, 1, 0)
                            : new ProviderRefCount(holder, client, 0, 1);
                }
                mProviderRefCountMap[jBinder] = prc;
            }
            retHolder = prc->mHolder;
        }
    }

    return retHolder;
}

ECode CActivityThread::Attach(
    /* [in] */ Boolean sys)
{
    pthread_setspecific(sKey, (IActivityThread*)this);
    this->AddRef();
    mSystemThread = sys;
    if (!sys) {
//         ViewRootImpl.addFirstDrawHandler(new Runnable() {
//             public void run() {
//                 ensureJitEnabled();
//             }
//         });
//         android.ddm.DdmHandleAppName.setAppName("<pre-initialized>",
//                                                 UserHandle.myUserId());
//         RuntimeInit.setApplicationObject(mAppThread.asBinder());
        AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
//         try {
        mgr->AttachApplication(mAppThread);
//         } catch (RemoteException ex) {
//             // Ignore
//         }
    }
    else {
        // Don't set application object here -- if the system crashes,
        // we can't display an alert, we just want to die die die.
//         android.ddm.DdmHandleAppName.setAppName("system_process",
//                                                 UserHandle.myUserId());
        // try {
        mInstrumentation = NULL;
        CInstrumentation::New((IInstrumentation**)&mInstrumentation);
        AutoPtr<CContextImpl> context;
        CContextImpl::NewByFriend((CContextImpl**)&context);
        AutoPtr<CContextImpl> ctx;
        GetSystemContext((IContextImpl**)&ctx);
        context->Init(ctx->mPackageInfo, NULL, this);
        AutoPtr<IApplication> app;
        if (FAILED(CInstrumentation::NewApplication(ECLSID_CApplication, context, (IApplication**)&app))) {
            Slogger::E(TAG, "Unable to instantiate Application()");
            return E_RUNTIME_EXCEPTION;
        }
        mAllApplications.PushBack(app);
        mInitialApplication = app;
        app->OnCreate();
        // } catch (Exception e) {
        //     throw new RuntimeException(
        //             "Unable to instantiate Application():" + e.toString(), e);
        // }
    }

//     // add dropbox logging to libcore
//     DropBox.setReporter(new DropBoxReporter());

    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    AutoPtr<IComponentCallbacks2> callbacks = new ConfigurationChangedCallbacks(wr);
    ViewRootImpl::AddConfigCallback(callbacks);
    return NOERROR;
}

AutoPtr<IActivityThread> CActivityThread::GetSystemMain()
{
//     HardwareRenderer.disable(TRUE);
    AutoPtr<CActivityThread> thread;
    CActivityThread::NewByFriend((CActivityThread**)&thread);
    thread->Attach(TRUE);
    return (IActivityThread*)thread.Get();
}

ECode CActivityThread::InstallSystemProviders(
    /* [in] */ ArrayOf<IProviderInfo*>* providers)
{
    if (providers != NULL) {
        AutoPtr<IObjectContainer> proContainer;
        CParcelableObjectContainer::New((IObjectContainer**)&proContainer);
        Int32 length = providers->GetLength();
        for (Int32 i = 0; i< length; ++i) {
            proContainer->Add((*providers)[i]);
        }
        InstallContentProviders(mInitialApplication, proContainer);
    }
    return NOERROR;
}

ECode CActivityThread::GetIntCoreSetting(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(setting);

    AutoLock lock(mPackagesLock);
    if (mCoreSettings != NULL) {
        mCoreSettings->GetInt32(key, defaultValue, setting);
        return NOERROR;
    } else {
        *setting = defaultValue;
        return NOERROR;
    }
}

ECode CActivityThread::Main(
    /* [in] */ const ArrayOf<String>& args)
{
//    SamplingProfilerIntegration.start();

    // CloseGuard defaults to TRUE and can be quite spammy.  We
    // disable it here, but selectively enable it later (via
    // StrictMode) on debug builds, but using DropBox, not logs.
//    CloseGuard.setEnabled(FALSE);

//    Environment.initForCurrentUser();

    // Set the reporter for event logging in libcore
//    EventLogger.setReporter(new EventLoggingReporter());

    Process::SetArgV0(String("<pre-initialized>"));

    Looper::PrepareMainLooper();

    AutoPtr<CActivityThread> thread;
    CActivityThread::NewByFriend((CActivityThread**)&thread);
    thread->Attach(FALSE);

    if (sMainThreadHandler == NULL) {
        sMainThreadHandler = thread->GetHandler();
    }

    AsyncTask::Init();

//    if (FALSE) {
//        Looper.myLooper().setMessageLogging(new
//                LogPrinter(Log.DEBUG, "ActivityThread"));
//    }

    Looper::Loop();

    // throw new RuntimeException("Main thread loop unexpectedly exited");
    return E_RUNTIME_EXCEPTION;
}

AutoPtr<CActivityThread::ActivityClientRecord> CActivityThread::GetActivityClientRecord(
    /* [in] */ IBinder* token)
{
    assert(token);
    HashMap<AutoPtr<IBinder>, AutoPtr<ActivityClientRecord> >::Iterator find
        = mActivities.Find(token);
    return find != mActivities.End() ? find->mSecond : NULL;
}

} // namespace App
} // namespace Droid
} // namespace Elastos

