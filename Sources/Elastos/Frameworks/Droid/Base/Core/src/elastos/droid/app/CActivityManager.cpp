#include <ext/frameworkdef.h>
#include "elastos/droid/app/CActivityManager.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/CUserHandleHelper.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/droid/app/AppGlobals.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::CObjectStringMap;
using Elastos::Droid::R;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Internal::App::IIUsageStats;

namespace Elastos {
namespace Droid {
namespace App {

String CActivityManager::TAG("CActivityManager");
Boolean CActivityManager::localLOGV = FALSE;

/*package*/
ECode CActivityManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler)
{
    mContext = context;
    mHandler = handler;
    return NOERROR;
}

ECode CActivityManager::GetFrontActivityScreenCompatMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = 0;

    ECode ec = NOERROR;
//     try {
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am) {
        ec = am->GetFrontActivityScreenCompatMode(mode);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            // System dead, we will be dead too soon!
            *mode = 0;
            ec = NOERROR;
        }
    }
//     } catch (RemoteException e) {
//         // System dead, we will be dead too soon!
//         return 0;
//     }

    return ec;
}

/** @hide */
ECode CActivityManager::SetFrontActivityScreenCompatMode(
    /* [in] */ Int32 mode)
{
    // try {
    return ActivityManagerNative::GetDefault()->SetFrontActivityScreenCompatMode(mode);
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    // }
}

/** @hide */
ECode CActivityManager::GetPackageScreenCompatMode(
    /* [in] */ const String& packageName,
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode)
    // try {
    ECode ec = ActivityManagerNative::GetDefault()->GetPackageScreenCompatMode(packageName, mode);
    if (FAILED(ec)) {
        *mode = 0;
    }
    return ec;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return 0;
    // }
}

/** @hide */
ECode CActivityManager::SetPackageScreenCompatMode(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 mode)
{
    // try {
    return ActivityManagerNative::GetDefault()->SetPackageScreenCompatMode(packageName, mode);
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    // }
}

/** @hide */
ECode CActivityManager::GetPackageAskScreenCompat(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* ask)
{
    // try {
    VALIDATE_NOT_NULL(ask)
    ECode ec = ActivityManagerNative::GetDefault()->GetPackageAskScreenCompat(packageName, ask);
    if (FAILED(ec)) {
        *ask = FALSE;
    }
    return ec;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return false;
    // }
}

/** @hide */
ECode CActivityManager::SetPackageAskScreenCompat(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean ask)
{
    // try {
    return ActivityManagerNative::GetDefault()->SetPackageAskScreenCompat(packageName, ask);
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    // }
}

ECode CActivityManager::GetMemoryClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls)
    *cls = 0;
//     return staticGetMemoryClass();
    return E_NOT_IMPLEMENTED;
}

/**
 * Return the approximate per-application memory class of the current
 * device when an application is running with a large heap.  This is the
 * space available for memory-intensive applications; most applications
 * should not need this amount of memory, and should instead stay with the
 * {@link #getMemoryClass()} limit.  The returned value is in megabytes.
 * This may be the same size as {@link #getMemoryClass()} on memory
 * constrained devices, or it may be significantly larger on devices with
 * a large amount of available RAM.
 *
 * <p>The is the size of the application's Dalvik heap if it has
 * specified <code>android:largeHeap="true"</code> in its manifest.
 */
ECode CActivityManager::GetLargeMemoryClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls)
    *cls = 0;
//     return staticGetLargeMemoryClass();
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManager::GetRecentTasks(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** tasks)
{
    VALIDATE_NOT_NULL(tasks);
    *tasks = NULL;

    // try {
    AutoPtr<IObjectContainer> temp;
    FAIL_RETURN(ActivityManagerNative::GetDefault()->GetRecentTasks(
            maxNum, flags, UserHandle::GetMyUserId(), (IObjectContainer**)&temp));
    *tasks = temp;
    REFCOUNT_ADD(*tasks);
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return null;
    // }
}

ECode CActivityManager::GetRecentTasksForUser(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userId,
    /* [out] */ IObjectContainer** tasks)
{
    VALIDATE_NOT_NULL(tasks);
    *tasks = NULL;

    // try {
    AutoPtr<IObjectContainer> temp;
    FAIL_RETURN(ActivityManagerNative::GetDefault()->GetRecentTasks(
        maxNum, flags, userId, (IObjectContainer**)&temp));
    *tasks = temp;
    REFCOUNT_ADD(*tasks);
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return null;
    // }
}

ECode CActivityManager::GetRunningTasks(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags,
    /* [in] */ IThumbnailReceiver* receiver,
    /* [out] */ IObjectContainer** tasks)
{
    VALIDATE_NOT_NULL(tasks);
    *tasks = NULL;
    // try {
    AutoPtr<IObjectContainer> temp;
    FAIL_RETURN(ActivityManagerNative::GetDefault()->GetTasks(
        maxNum, flags, receiver, (IObjectContainer**)&temp));
    *tasks = temp;
    REFCOUNT_ADD(*tasks);
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return null;
    // }
}

ECode CActivityManager::GetRunningTasks(
    /* [in] */ Int32 maxNum,
    /* [out] */ IObjectContainer** tasks)
{
    return GetRunningTasks(maxNum, 0, NULL, tasks);
}

ECode CActivityManager::RemoveSubTask(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 subTaskIndex,
    /* [out] */ Boolean* removed)
{
//     try {
    ECode ec = ActivityManagerNative::GetDefault()->RemoveSubTask(taskId, subTaskIndex, removed);
    if (FAILED(ec)) *removed = FALSE;
    return NOERROR;
//     } catch (RemoteException e) {
//         // System dead, we will be dead too soon!
//         return false;
//     }
}

ECode CActivityManager::RemoveTask(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* removed)
{
    // try {
    ECode ec = ActivityManagerNative::GetDefault()->RemoveTask(taskId, flags, removed);
    if (FAILED(ec)) *removed = FALSE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return false;
    // }
}

ECode CActivityManager::GetTaskThumbnails(
    /* [in] */ Int32 id,
    /* [out] */ IActivityManagerTaskThumbnails** taskThumbnail)
{
    // try {
    VALIDATE_NOT_NULL(taskThumbnail)
    ECode ec = ActivityManagerNative::GetDefault()->GetTaskThumbnails(id, taskThumbnail);
    if (FAILED(ec)) {
        *taskThumbnail = NULL;
    }
    return ec;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return null;
    // }
}

/** @hide */
ECode CActivityManager::GetTaskTopThumbnail(
    /* [in] */ Int32 id,
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);
    // try {
    ECode ec = ActivityManagerNative::GetDefault()->GetTaskTopThumbnail(id, thumbnail);
    if (FAILED(ec)) {
        *thumbnail = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return null;
    // }
}

ECode CActivityManager::MoveTaskToFront(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 flags)
{
    return MoveTaskToFront(taskId, flags, NULL);
}

ECode CActivityManager::MoveTaskToFront(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle* options)
{
    // try {
    ActivityManagerNative::GetDefault()->MoveTaskToFront(taskId, flags, options);
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    // }
}

ECode CActivityManager::GetRunningServices(
    /* [in] */ Int32 maxNum,
    /* [out] */ IObjectContainer** runningServices)
{
    VALIDATE_NOT_NULL(runningServices);

    ECode ec = ActivityManagerNative::GetDefault()->GetServices(maxNum, 0, runningServices);
    if (FAILED(ec)) {
        *runningServices = NULL;
    }
    return NOERROR;
}

ECode CActivityManager::GetRunningServiceControlPanel(
    /* [in] */ IComponentName* service,
    /* [out] */ IPendingIntent** pendingIntent)
{
    // try {
    VALIDATE_NOT_NULL(pendingIntent)
    ECode ec = ActivityManagerNative::GetDefault()->GetRunningServiceControlPanel(service, pendingIntent);
    if (FAILED(ec)) {
        *pendingIntent = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     // System dead, we will be dead too soon!
    //     return null;
    // }
}

ECode CActivityManager::GetMemoryInfo(
    /* [out] */ IActivityManagerMemoryInfo** outInfo)
{
    VALIDATE_NOT_NULL(outInfo);

//     try {
    ECode ec = ActivityManagerNative::GetDefault()->GetMemoryInfo(outInfo);
    if (FAILED(ec)) {
        *outInfo = NULL;
    }
    return NOERROR;
//     } catch (RemoteException e) {
//     }
}

ECode CActivityManager::ClearApplicationUserData(
    /* [in] */ const String& packageName,
    /* [in] */ IPackageDataObserver* observer,
    /* [out] */ Boolean* result)
{
    // try {
    VALIDATE_NOT_NULL(result)
    ECode ec = ActivityManagerNative::GetDefault()->ClearApplicationUserData(packageName,
            observer, UserHandle::GetMyUserId(), result);
    if (FAILED(ec)) {
        *result = FALSE;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CActivityManager::GetProcessesInErrorState(
    /* [out] */ IObjectContainer** records)
{
    // try {
    VALIDATE_NOT_NULL(records)
    ECode ec = ActivityManagerNative::GetDefault()->GetProcessesInErrorState(records);
    if (FAILED(ec)) {
        *records = NULL;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CActivityManager::GetRunningExternalApplications(
    /* [out] */ IObjectContainer** records)
{
    // try {
    VALIDATE_NOT_NULL(records)
    ECode ec = ActivityManagerNative::GetDefault()->GetRunningExternalApplications(records);
    if (FAILED(ec)) {
        *records = NULL;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CActivityManager::GetRunningAppProcesses(
    /* [out] */ IObjectContainer** records)
{
    // try {
    VALIDATE_NOT_NULL(records)
    ECode ec = ActivityManagerNative::GetDefault()->GetRunningAppProcesses(records);
    if (FAILED(ec)) {
        *records = NULL;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CActivityManager::GetProcessMemoryInfo(
    /* [in] */ ArrayOf<Int32>* pids,
    /* [out] */ ArrayOf<IDebugMemoryInfo*>** memoryInfos)
{
    // try {
    VALIDATE_NOT_NULL(memoryInfos)
    ECode ec = ActivityManagerNative::GetDefault()->GetProcessMemoryInfo(pids, memoryInfos);
    if (FAILED(ec)) {
        *memoryInfos = NULL;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CActivityManager::RestartPackage(
    /* [in] */ const String& packageName)
{
    return KillBackgroundProcesses(packageName);
}

ECode CActivityManager::KillBackgroundProcesses(
    /* [in] */ const String& packageName)
{
    // try {
    return ActivityManagerNative::GetDefault()->KillBackgroundProcesses(packageName, UserHandle::GetMyUserId());
    // } catch (RemoteException e) {
    // }
}

ECode CActivityManager::ForceStopPackage(
    /* [in] */ const String& packageName)
{
    // try {
    return ActivityManagerNative::GetDefault()->ForceStopPackage(packageName, UserHandle::GetMyUserId());
    // } catch (RemoteException e) {
    // }
}

ECode CActivityManager::GetDeviceConfigurationInfo(
    /* [out] */ IConfigurationInfo** configurationInfo)
{
    // try {
    VALIDATE_NOT_NULL(configurationInfo)
    return ActivityManagerNative::GetDefault()->GetDeviceConfigurationInfo(configurationInfo);
    // } catch (RemoteException e) {
    // }
    // return null;
}

ECode CActivityManager::GetLauncherLargeIconDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);

    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> dm;
    Int32 den = 0;
    res->GetDisplayMetrics((IDisplayMetrics**)&dm);
    dm->GetDensityDpi(&den);
    AutoPtr<IConfiguration> conf;
    Int32 sw;
    res->GetConfiguration((IConfiguration**)&conf);
    conf->GetSmallestScreenWidthDp(&sw);

    String tabeltUI = Build::TABLETUI;
    if (tabeltUI.EqualsIgnoreCase("TRUE")){
            *density = IDisplayMetrics::DENSITY_HIGH;
            return NOERROR;
    }

    if (sw < 600) {
        // Smaller than approx 7" tablets, use the regular icon size.
        *density = den;
        return NOERROR;
    }

    switch (den) {
        case IDisplayMetrics::DENSITY_LOW:
            *density = IDisplayMetrics::DENSITY_MEDIUM;
        case IDisplayMetrics::DENSITY_MEDIUM:
            *density = IDisplayMetrics::DENSITY_HIGH;
        case IDisplayMetrics::DENSITY_TV:
            *density = IDisplayMetrics::DENSITY_XHIGH;
        case IDisplayMetrics::DENSITY_HIGH:
            *density = IDisplayMetrics::DENSITY_XHIGH;
        case IDisplayMetrics::DENSITY_XHIGH:
            *density = IDisplayMetrics::DENSITY_XXHIGH;
        case IDisplayMetrics::DENSITY_XXHIGH:
            *density = IDisplayMetrics::DENSITY_XHIGH * 2;
        default:
            // The density is some abnormal value.  Return some other
            // abnormal value that is a reasonable scaling of it.
            *density = (Int32)((den * 1.5) + 0.5);
    }
    return NOERROR;
}

ECode CActivityManager::GetLauncherLargeIconSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    Int32 pSize;
    res->GetDimensionPixelSize(R::dimen::app_icon_size, &pSize);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    Int32 sw;
    config->GetSmallestScreenWidthDp(&sw);

    if (sw < 600) {
        // Smaller than approx 7" tablets, use the regular icon size.
        *size = pSize;
        return NOERROR;
    }

    AutoPtr<IDisplayMetrics> dm;
    res->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Int32 density;
    dm->GetDensityDpi(&density);

    switch (density) {
        case IDisplayMetrics::DENSITY_LOW:
            *size = (pSize * IDisplayMetrics::DENSITY_MEDIUM) / IDisplayMetrics::DENSITY_LOW;
        case IDisplayMetrics::DENSITY_MEDIUM:
            *size = (pSize * IDisplayMetrics::DENSITY_HIGH) / IDisplayMetrics::DENSITY_MEDIUM;
        case IDisplayMetrics::DENSITY_TV:
            *size = (pSize * IDisplayMetrics::DENSITY_XHIGH) / IDisplayMetrics::DENSITY_HIGH;
        case IDisplayMetrics::DENSITY_HIGH:
            *size = (pSize * IDisplayMetrics::DENSITY_XHIGH) / IDisplayMetrics::DENSITY_HIGH;
        case IDisplayMetrics::DENSITY_XHIGH:
            *size = (pSize * IDisplayMetrics::DENSITY_XXHIGH) / IDisplayMetrics::DENSITY_XHIGH;
        case IDisplayMetrics::DENSITY_XXHIGH:
            *size = (pSize * IDisplayMetrics::DENSITY_XHIGH*2) / IDisplayMetrics::DENSITY_XXHIGH;
        default:
            // The density is some abnormal value.  Return some other
            // abnormal value that is a reasonable scaling of it.
            *size = (Int32)((pSize*1.5f) + .5f);
    }
    return NOERROR;
}

ECode CActivityManager::GetAllPackageLaunchCounts(
    /* [out] */ IObjectStringMap** counts)
{
    VALIDATE_NOT_NULL(counts)
    *counts = NULL;
    // try {
    AutoPtr<IInterface> service = ServiceManager::GetService(String("usagestats"));
    AutoPtr<IIUsageStats> usageStatsService = IIUsageStats::Probe(service);
    if (usageStatsService == NULL) {
        return CObjectStringMap::New(counts);
    }

    AutoPtr< ArrayOf<IPkgUsageStats*> > allPkgUsageStats;
    if (FAILED(usageStatsService->GetAllPkgUsageStats((ArrayOf<IPkgUsageStats*>**)&allPkgUsageStats))) {
        Slogger::W(TAG, "Could not query launch counts");
        return CObjectStringMap::New(counts);
    }
    if (allPkgUsageStats == NULL) {
        return CObjectStringMap::New(counts);
    }

    AutoPtr<IObjectStringMap> launchCounts;
    CObjectStringMap::New((IObjectStringMap**)&launchCounts);
    for (Int32 i = 0; i < allPkgUsageStats->GetLength(); ++i) {
        Slogger::E(TAG, "TODO: PkgUsageStats has not been realized!!!!!!!!!!!!!!");
        assert(0);
        // launchCounts.put(pkgUsageStats.packageName, pkgUsageStats.launchCount);
    }

    *counts = launchCounts;
    REFCOUNT_ADD(*counts);
    return NOERROR;
    // } catch (RemoteException e) {
    //     Log.w(TAG, "Could not query launch counts", e);
    //     return new HashMap<String, Integer>();
    // }
}

ECode CActivityManager::GetAllPackageUsageStats(
    /* [out, callee] */ ArrayOf<IPkgUsageStats*>** stats)
{
    VALIDATE_NOT_NULL(stats)
    *stats = NULL;
    // try {
    AutoPtr<IInterface> service = ServiceManager::GetService(String("usagestats"));
    AutoPtr<IIUsageStats> usageStatsService = IIUsageStats::Probe(service);
    if (usageStatsService != NULL) {
        ECode ec = usageStatsService->GetAllPkgUsageStats(stats);
        if (FAILED(ec)) {
            Slogger::W(TAG, "Could not query launch counts");
            AutoPtr< ArrayOf<IPkgUsageStats*> > array = ArrayOf<IPkgUsageStats*>::Alloc(0);
            *stats = array;
            REFCOUNT_ADD(*stats);
        }
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     Log.w(TAG, "Could not query usage stats", e);
    // }
}

ECode CActivityManager::SwitchUser(
    /* [in] */ Int32 userid,
    /* [out] */ Boolean* switched)
{
    VALIDATE_NOT_NULL(switched);
    *switched = FALSE;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am) {
        am->SwitchUser(userid, switched);
    }

    return NOERROR;
}

ECode CActivityManager::IsUserRunning(
    /* [in] */ Int32 userid,
    /* [out] */ Boolean* isRunning)
{
    VALIDATE_NOT_NULL(isRunning);
    *isRunning = FALSE;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am) {
        am->IsUserRunning(userid, FALSE, isRunning);
    }

    return NOERROR;
}

Int32 CActivityManager::GetMemoryClass()
{
    //     // Really brain dead right now -- just take this from the configured
    //     // vm heap size, and assume it is in megabytes and thus ends with "m".
    //     String vmHeapSize = SystemProperties.get("dalvik.vm.heapgrowthlimit", "");
    //     if (vmHeapSize != null && !"".equals(vmHeapSize)) {
    //         return Integer.parseInt(vmHeapSize.substring(0, vmHeapSize.length()-1));
    //     }
    //     return staticGetLargeMemoryClass();
    return E_NOT_IMPLEMENTED;
}

Int32 CActivityManager::GetLargeMemoryClass()
{
    //     // Really brain dead right now -- just take this from the configured
    //     // vm heap size, and assume it is in megabytes and thus ends with "m".
    //     String vmHeapSize = SystemProperties.get("dalvik.vm.heapsize", "16m");
    //     return Integer.parseInt(vmHeapSize.substring(0, vmHeapSize.length()-1));
    return E_NOT_IMPLEMENTED;
}

Boolean CActivityManager::IsHighEndGfx()
{
    // MemInfoReader reader = new MemInfoReader();
    // reader.readMemInfo();
    // if (reader.getTotalSize() >= (512*1024*1024)) {
    //     // If the device has at least 512MB RAM available to the kernel,
    //     // we can afford the overhead of graphics acceleration.
    //     return true;
    // }

    // Display display = DisplayManagerGlobal.getInstance().getRealDisplay(
    //         Display.DEFAULT_DISPLAY);
    // Point p = new Point();
    // display.getRealSize(p);
    // int pixels = p.x * p.y;
    // if (pixels >= (1024*600)) {
    //     // If this is a sufficiently large screen, then there are enough
    //     // pixels on it that we'd really like to use hw drawing.
    //     return true;
    // }
    // return false;
    return FALSE;
}

Boolean CActivityManager::IsLargeRAM()
{
    //     MemInfoReader reader = new MemInfoReader();
    //     reader.readMemInfo();
    //     if (reader.getTotalSize() >= (640*1024*1024)) {
    //         // Currently 640MB RAM available to the kernel is the point at
    //         // which we have plenty of RAM to spare.
    //         return true;
    //     }
    //     return false;
    return FALSE;
}

AutoPtr<IActivityManagerRunningAppProcessInfo> CActivityManager::GetMyMemoryState()
{
    //     try {
    //         ActivityManagerNative.getDefault().getMyMemoryState(outState);
    //     } catch (RemoteException e) {
    //     }
    AutoPtr<IActivityManagerRunningAppProcessInfo> info;
    return info;
}

Boolean CActivityManager::IsUserAMonkey()
{
    // try {
    Boolean result;
    if (FAILED(ActivityManagerNative::GetDefault()->IsUserAMonkey(&result))) {
        return FALSE;
    }
    return result;
    // } catch (RemoteException e) {
    // }
    // return false;
}

Boolean CActivityManager::IsRunningInTestHarness()
{
    return SystemProperties::GetBoolean(String("ro.test_harness"), FALSE);
}

Int32 CActivityManager::CheckComponentPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 owningUid,
    /* [in] */ Boolean exported)
{
    // Root, system server get to do everything.
    if (uid == 0 || uid == IProcess::SYSTEM_UID) {
        return IPackageManager::PERMISSION_GRANTED;
    }
    // Isolated processes don't get any permissions.
    if (UserHandle::IsIsolated(uid)) {
        return IPackageManager::PERMISSION_DENIED;
    }
    // If there is a uid that owns whatever is being accessed, it has
    // blanket access to it regardless of the permissions it requires.
    if (owningUid >= 0 && UserHandle::IsSameApp(uid, owningUid)) {
        return IPackageManager::PERMISSION_GRANTED;
    }
    // If the target is not exported, then nobody else can get to it.
    if (!exported) {
        //Slogger::W(TAG, "Permission denied: checkComponentPermission() owningUid=%d", owningUid);
        return IPackageManager::PERMISSION_DENIED;
    }
    if (permission.IsNull()) {
        return IPackageManager::PERMISSION_GRANTED;
    }

    Int32 result;
    AutoPtr<IIPackageManager> pm = AppGlobals::GetPackageManager();
    if (FAILED(pm->CheckUidPermission(permission, uid, &result))) {
        // Should never happen, but if it does... deny!
//        Slogger::E(TAG, "PackageManager is dead?!?");
        return IPackageManager::PERMISSION_DENIED;
    }

    return result;
}

Int32 CActivityManager::CheckUidPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 uid)
{
    Int32 result;
    AutoPtr<IIPackageManager> pm = AppGlobals::GetPackageManager();
    if (FAILED(pm->CheckUidPermission(permission, uid, &result))) {
        return IPackageManager::PERMISSION_DENIED;
    }

    return result;
}

ECode CActivityManager::HandleIncomingUser(
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean allowAll,
    /* [in] */ Boolean requireFull,
    /* [in] */ const String& name,
    /* [in] */ const String& callerPackage,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    Int32 uid = 0;
    helper->GetUserId(callingUid, &uid);
    if (uid == userId) {
        *value = userId;
        return NOERROR;
    }

    //     try {
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am) {
        ECode ec = am->HandleIncomingUser(
            callingPid, callingUid, userId, allowAll, requireFull, name, callerPackage, value);
        if (FAILED(ec)) {
            *value = 0;
            return E_SECURITY_EXCEPTION;
        }
    }
    return NOERROR;
}

Int32 CActivityManager::GetCurrentUser()
{
    Int32 uid = 0;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    if (am) {
        AutoPtr<IUserInfo> ui;
        am->GetCurrentUser((IUserInfo**)&ui);
        if (ui != NULL) {
            ui->GetId(&uid);
        }
    }

    return uid;
}


} // namespace App
} // namespace Droid
} // namespace Elastos
