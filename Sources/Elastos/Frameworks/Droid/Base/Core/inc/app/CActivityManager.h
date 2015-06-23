
#ifndef __CACTIVITYMANAGER_H__
#define __CACTIVITYMANAGER_H__

#include "_CActivityManager.h"

using Elastos::Utility::IObjectStringMap;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IPackageDataObserver;
using Elastos::Droid::Content::Pm::IConfigurationInfo;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IDebugMemoryInfo;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Internal::Os::IPkgUsageStats;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityManager)
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler);

    /** @hide */
    CARAPI GetFrontActivityScreenCompatMode(
        /* [out] */ Int32* mode);

    /** @hide */
    CARAPI SetFrontActivityScreenCompatMode(
        /* [in] */ Int32 mode);

    /** @hide */
    CARAPI GetPackageScreenCompatMode(
        /* [in] */ const String& packageName,
        /* [out] */ Int32* mode);

    /** @hide */
    CARAPI SetPackageScreenCompatMode(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 mode);

    /** @hide */
    CARAPI GetPackageAskScreenCompat(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* ask);

    /** @hide */
    CARAPI SetPackageAskScreenCompat(
        /* [in] */ const String& packageName,
        /* [in] */ Boolean ask);

    /**
     * Return the approximate per-application memory class of the current
     * device.  This gives you an idea of how hard a memory limit you should
     * impose on your application to let the overall system work best.  The
     * returned value is in megabytes; the baseline Android memory class is
     * 16 (which happens to be the Java heap limit of those devices); some
     * device with more memory may return 24 or even higher numbers.
     */
    CARAPI GetMemoryClass(
        /* [out] */ Int32* cls);

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
    CARAPI GetLargeMemoryClass(
        /* [out] */ Int32* cls);

    /**
     * Return a list of the tasks that the user has recently launched, with
     * the most recent being first and older ones after in order.
     *
     * <p><b>Note: this method is only intended for debugging and presenting
     * task management user interfaces</b>.  This should never be used for
     * core logic in an application, such as deciding between different
     * behaviors based on the information found here.  Such uses are
     * <em>not</em> supported, and will likely break in the future.  For
     * example, if multiple applications can be actively running at the
     * same time, assumptions made about the meaning of the data here for
     * purposes of control flow will be incorrect.</p>
     *
     * @param maxNum The maximum number of entries to return in the list.  The
     * actual number returned may be smaller, depending on how many tasks the
     * user has started and the maximum number the system can remember.
     * @param flags Information about what to return.  May be any combination
     * of {@link #RECENT_WITH_EXCLUDED} and {@link #RECENT_IGNORE_UNAVAILABLE}.
     *
     * @return Returns a list of RecentTaskInfo records describing each of
     * the recent tasks.
     *
     * @throws SecurityException Throws SecurityException if the caller does
     * not hold the {@link android.Manifest.permission#GET_TASKS} permission.
     */
    CARAPI GetRecentTasks(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** tasks);

    /**
     * Same as {@link #getRecentTasks(int, int)} but returns the recent tasks for a
     * specific user. It requires holding
     * the {@link android.Manifest.permission#INTERACT_ACROSS_USERS_FULL} permission.
     * @param maxNum The maximum number of entries to return in the list.  The
     * actual number returned may be smaller, depending on how many tasks the
     * user has started and the maximum number the system can remember.
     * @param flags Information about what to return.  May be any combination
     * of {@link #RECENT_WITH_EXCLUDED} and {@link #RECENT_IGNORE_UNAVAILABLE}.
     *
     * @return Returns a list of RecentTaskInfo records describing each of
     * the recent tasks.
     *
     * @throws SecurityException Throws SecurityException if the caller does
     * not hold the {@link android.Manifest.permission#GET_TASKS} or the
     * {@link android.Manifest.permission#INTERACT_ACROSS_USERS_FULL} permissions.
     * @hide
     */
    CARAPI GetRecentTasksForUser(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userId,
        /* [out] */ IObjectContainer** tasks);

    /**
     * Return a list of the tasks that are currently running, with
     * the most recent being first and older ones after in order.  Note that
     * "running" does not mean any of the task's code is currently loaded or
     * activity -- the task may have been frozen by the system, so that it
     * can be restarted in its previous state when next brought to the
     * foreground.
     *
     * @param maxNum The maximum number of entries to return in the list.  The
     * actual number returned may be smaller, depending on how many tasks the
     * user has started.
     *
     * @param flags Optional flags
     * @param receiver Optional receiver for delayed thumbnails
     *
     * @return Returns a list of RunningTaskInfo records describing each of
     * the running tasks.
     *
     * Some thumbnails may not be available at the time of this call. The optional
     * receiver may be used to receive those thumbnails.
     *
     * @throws SecurityException Throws SecurityException if the caller does
     * not hold the {@link android.Manifest.permission#GET_TASKS} permission.
     *
     * @hide
     */
    CARAPI GetRunningTasks(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [in] */ IThumbnailReceiver* receiver,
        /* [out] */ IObjectContainer** tasks);

    /**
     * Return a list of the tasks that are currently running, with
     * the most recent being first and older ones after in order.  Note that
     * "running" does not mean any of the task's code is currently loaded or
     * activity -- the task may have been frozen by the system, so that it
     * can be restarted in its previous state when next brought to the
     * foreground.
     *
     * <p><b>Note: this method is only intended for debugging and presenting
     * task management user interfaces</b>.  This should never be used for
     * core logic in an application, such as deciding between different
     * behaviors based on the information found here.  Such uses are
     * <em>not</em> supported, and will likely break in the future.  For
     * example, if multiple applications can be actively running at the
     * same time, assumptions made about the meaning of the data here for
     * purposes of control flow will be incorrect.</p>
     *
     * @param maxNum The maximum number of entries to return in the list.  The
     * actual number returned may be smaller, depending on how many tasks the
     * user has started.
     *
     * @return Returns a list of RunningTaskInfo records describing each of
     * the running tasks.
     *
     * @throws SecurityException Throws SecurityException if the caller does
     * not hold the {@link android.Manifest.permission#GET_TASKS} permission.
     */
    CARAPI GetRunningTasks(
        /* [in] */ Int32 maxNum,
        /* [out] */ IObjectContainer** tasks);

    /**
     * Remove some end of a task's activity stack that is not part of
     * the main application.  The selected activities will be finished, so
     * they are no longer part of the main task.
     *
     * @param taskId The identifier of the task.
     * @param subTaskIndex The number of the sub-task; this corresponds
     * to the index of the thumbnail returned by {@link #getTaskThumbnails(int)}.
     * @return Returns true if the sub-task was found and was removed.
     *
     * @hide
     */
    CARAPI RemoveSubTask(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 subTaskIndex,
        /* [out] */ Boolean* removed);

    /**
     * Completely remove the given task.
     *
     * @param taskId Identifier of the task to be removed.
     * @param flags Additional operational flags.  May be 0 or
     * {@link #REMOVE_TASK_KILL_PROCESS}.
     * @return Returns true if the given task was found and removed.
     *
     * @hide
     */
    CARAPI RemoveTask(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* removed);

    /** @hide */
    CARAPI GetTaskThumbnails(
        /* [in] */ Int32 id,
        /* [out] */ IActivityManagerTaskThumbnails** taskThumbnail);

    /** @hide */
    CARAPI GetTaskTopThumbnail(
        /* [in] */ Int32 id,
        /* [out] */ IBitmap** thumbnail);

    /**
     * Equivalent to calling {@link #moveTaskToFront(int, int, Bundle)}
     * with a null options argument.
     *
     * @param taskId The identifier of the task to be moved, as found in
     * {@link RunningTaskInfo} or {@link RecentTaskInfo}.
     * @param flags Additional operational flags, 0 or more of
     * {@link #MOVE_TASK_WITH_HOME}.
     */
    CARAPI MoveTaskToFront(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 flags);

    /**
     * Ask that the task associated with a given task ID be moved to the
     * front of the stack, so it is now visible to the user.  Requires that
     * the caller hold permission {@link android.Manifest.permission#REORDER_TASKS}
     * or a SecurityException will be thrown.
     *
     * @param taskId The identifier of the task to be moved, as found in
     * {@link RunningTaskInfo} or {@link RecentTaskInfo}.
     * @param flags Additional operational flags, 0 or more of
     * {@link #MOVE_TASK_WITH_HOME}.
     * @param options Additional options for the operation, either null or
     * as per {@link Context#startActivity(Intent, android.os.Bundle)
     * Context.startActivity(Intent, Bundle)}.
     */
    CARAPI MoveTaskToFront(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 flags,
        /* [in] */ IBundle* options);

    /**
     * Return a list of the services that are currently running.
     *
     * <p><b>Note: this method is only intended for debugging or implementing
     * service management type user interfaces.</b></p>
     *
     * @param maxNum The maximum number of entries to return in the list.  The
     * actual number returned may be smaller, depending on how many services
     * are running.
     *
     * @return Returns a list of RunningServiceInfo records describing each of
     * the running tasks.
     */
    CARAPI GetRunningServices(
        /* [in] */ Int32 maxNum,
        /* [out] */ IObjectContainer** runningServices);

    /**
     * Returns a PendingIntent you can start to show a control panel for the
     * given running service.  If the service does not have a control panel,
     * null is returned.
     */
    CARAPI GetRunningServiceControlPanel(
        /* [in] */ IComponentName* service,
        /* [out] */ IPendingIntent** pendingIntent);

    /**
     * Return general information about the memory state of the system.  This
     * can be used to help decide how to manage your own memory, though note
     * that polling is not recommended and
     * {@link android.content.ComponentCallbacks2#onTrimMemory(int)
     * ComponentCallbacks2.onTrimMemory(int)} is the preferred way to do this.
     * Also see {@link #getMyMemoryState} for how to retrieve the current trim
     * level of your process as needed, which gives a better hint for how to
     * manage its memory.
     */
    CARAPI GetMemoryInfo(
        /* [out] */ IActivityManagerMemoryInfo** outInfo);

    /**
     * @hide
     */
    CARAPI ClearApplicationUserData(
        /* [in] */ const String& packageName,
        /* [in] */ IPackageDataObserver* observer,
        /* [out] */ Boolean* result);

    /**
     * Returns a list of any processes that are currently in an error condition.  The result
     * will be null if all processes are running properly at this time.
     *
     * @return Returns a list of ProcessErrorStateInfo records, or null if there are no
     * current error conditions (it will not return an empty list).  This list ordering is not
     * specified.
     */
    CARAPI GetProcessesInErrorState(
        /* [out] */ IObjectContainer** records);

    /**
     * Returns a list of application processes installed on external media
     * that are running on the device.
     *
     * <p><b>Note: this method is only intended for debugging or building
     * a user-facing process management UI.</b></p>
     *
     * @return Returns a list of ApplicationInfo records, or null if none
     * This list ordering is not specified.
     * @hide
     */
    CARAPI GetRunningExternalApplications(
        /* [out] */ IObjectContainer** records);

    /**
     * Returns a list of application processes that are running on the device.
     *
     * <p><b>Note: this method is only intended for debugging or building
     * a user-facing process management UI.</b></p>
     *
     * @return Returns a list of RunningAppProcessInfo records, or null if there are no
     * running processes (it will not return an empty list).  This list ordering is not
     * specified.
     */
    CARAPI GetRunningAppProcesses(
        /* [out] */ IObjectContainer** records);

    /**
     * Return information about the memory usage of one or more processes.
     *
     * <p><b>Note: this method is only intended for debugging or building
     * a user-facing process management UI.</b></p>
     *
     * @param pids The pids of the processes whose memory usage is to be
     * retrieved.
     * @return Returns an array of memory information, one for each
     * requested pid.
     */
    CARAPI GetProcessMemoryInfo(
        /* [in] */ ArrayOf<Int32>* pids,
        /* [out] */ ArrayOf<IDebugMemoryInfo*>** memoryInfos);

    /**
     * @deprecated This is now just a wrapper for
     * {@link #killBackgroundProcesses(String)}; the previous behavior here
     * is no longer available to applications because it allows them to
     * break other applications by removing their alarms, stopping their
     * services, etc.
     */
    //@Deprecated
    CARAPI RestartPackage(
        /* [in] */ const String& packageName);

    /**
     * Have the system immediately kill all background processes associated
     * with the given package.  This is the same as the kernel killing those
     * processes to reclaim memory; the system will take care of restarting
     * these processes in the future as needed.
     *
     * <p>You must hold the permission
     * {@link android.Manifest.permission#KILL_BACKGROUND_PROCESSES} to be able to
     * call this method.
     *
     * @param packageName The name of the package whose processes are to
     * be killed.
     */
    CARAPI KillBackgroundProcesses(
        /* [in] */ const String& packageName);

    /**
     * Have the system perform a force stop of everything associated with
     * the given application package.  All processes that share its uid
     * will be killed, all services it has running stopped, all activities
     * removed, etc.  In addition, a {@link Intent#ACTION_PACKAGE_RESTARTED}
     * broadcast will be sent, so that any of its registered alarms can
     * be stopped, notifications removed, etc.
     *
     * <p>You must hold the permission
     * {@link android.Manifest.permission#FORCE_STOP_PACKAGES} to be able to
     * call this method.
     *
     * @param packageName The name of the package to be stopped.
     *
     * @hide This is not available to third party applications due to
     * it allowing them to break other applications by stopping their
     * services, removing their alarms, etc.
     */
    CARAPI ForceStopPackage(
        /* [in] */ const String& packageName);

    /**
     * Get the device configuration attributes.
     */
    CARAPI GetDeviceConfigurationInfo(
        /* [out] */ IConfigurationInfo** configurationInfo);

    /**
     * Get the preferred density of icons for the launcher. This is used when
     * custom drawables are created (e.g., for shortcuts).
     *
     * @return density in terms of DPI
     */
    CARAPI GetLauncherLargeIconDensity(
        /* [out] */ Int32* density);

    /**
     * Get the preferred launcher icon size. This is used when custom drawables
     * are created (e.g., for shortcuts).
     *
     * @return dimensions of square icons in terms of pixels
     */
    CARAPI GetLauncherLargeIconSize(
        /* [out] */ Int32* size);

    /**
     * Returns the launch count of each installed package.
     *
     * @hide
     */
    CARAPI GetAllPackageLaunchCounts(
        /* [out] */ IObjectStringMap** counts);

    /**
     * Returns the usage statistics of each installed package.
     *
     * @hide
     */
    CARAPI GetAllPackageUsageStats(
        /* [out, callee] */ ArrayOf<IPkgUsageStats*>** stats);

    /**
     * @param userid the user's id. Zero indicates the default user
     * @hide
     */
    CARAPI SwitchUser(
        /* [in] */ Int32 userid,
        /* [out] */ Boolean* switched);

    /**
     * Return whether the given user is actively running.  This means that
     * the user is in the "started" state, not "stopped" -- it is currently
     * allowed to run code through scheduled alarms, receiving broadcasts,
     * etc.  A started user may be either the current foreground user or a
     * background user; the result here does not distinguish between the two.
     * @param userid the user's id. Zero indicates the default user.
     * @hide
     */
    CARAPI IsUserRunning(
        /* [in] */ Int32 userid,
        /* [out] */ Boolean* isRunning);

public:
    /** @hide */
    static CARAPI_(Int32) GetMemoryClass();

    /** @hide */
    static CARAPI_(Int32) GetLargeMemoryClass();

    /**
     * Used by persistent processes to determine if they are running on a
     * higher-end device so should be okay using hardware drawing acceleration
     * (which tends to consume a lot more RAM).
     * @hide
     */
    static CARAPI_(Boolean) IsHighEndGfx();

    /**
     * Use to decide whether the running device can be considered a "large
     * RAM" device.  Exactly what memory limit large RAM is will vary, but
     * it essentially means there is plenty of RAM to have lots of background
     * processes running under decent loads.
     * @hide
     */
    static CARAPI_(Boolean) IsLargeRAM();

    /**
     * Return global memory state information for the calling process.  This
     * does not fill in all fields of the {@link RunningAppProcessInfo}.  The
     * only fields that will be filled in are
     * {@link RunningAppProcessInfo#pid},
     * {@link RunningAppProcessInfo#uid},
     * {@link RunningAppProcessInfo#lastTrimLevel},
     * {@link RunningAppProcessInfo#importance},
     * {@link RunningAppProcessInfo#lru}, and
     * {@link RunningAppProcessInfo#importanceReasonCode}.
     */
    static CARAPI_(AutoPtr<IActivityManagerRunningAppProcessInfo>) GetMyMemoryState();

    /**
     * Returns "true" if the user interface is currently being messed with
     * by a monkey.
     */
    static CARAPI_(Boolean) IsUserAMonkey();

    /**
     * Returns "true" if device is running in a test harness.
     */
    static CARAPI_(Boolean) IsRunningInTestHarness();

    /** @hide */
    static CARAPI_(Int32) CheckComponentPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 owningUid,
        /* [in] */ Boolean exported);

    /** @hide */
    static CARAPI_(Int32) CheckUidPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 uid);

    /** @hide */
    static CARAPI HandleIncomingUser(
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean allowAll,
        /* [in] */ Boolean requireFull,
        /* [in] */ const String& name,
        /* [in] */ const String& callerPackage,
        /* [out] */ Int32* value);

    /** @hide */
    static CARAPI_(Int32) GetCurrentUser();

private:
    static String TAG;
    static Boolean localLOGV;

    AutoPtr<IContext> mContext;
    AutoPtr<IHandler> mHandler;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYMANAGER_H__
