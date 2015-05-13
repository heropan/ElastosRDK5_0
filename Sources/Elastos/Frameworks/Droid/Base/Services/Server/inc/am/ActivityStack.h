
#ifndef __ACTIVITYSTACK_H__
#define __ACTIVITYSTACK_H__

#include "ext/frameworkext.h"
#include <elastos/StringBuilder.h>
#include <elastos/Mutex.h>
#include "am/UserStartedState.h"
#include "am/ActivityState.h"
#include "am/TaskAccessInfo.h"
#include "os/HandlerBase.h"

using Elastos::Core::Threading::Mutex;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::App::IApplicationThread;
using Elastos::Droid::App::IActivityManagerWaitResult;
using Elastos::Droid::App::IActivityManagerTaskThumbnails;
using Elastos::Droid::App::EIID_IActivityManagerTaskThumbnails;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class CActivityManagerService;
class ActivityRecord;
class UserStartedState;
class TaskAccessInfo;
class TaskRecord;
class ProcessRecord;

/**
 * State and management of a single stack of activities.
 */
class ActivityStack : public ElRefBase
{
public:
    class ScheduleDestroyArgs
        : public ElLightRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        ScheduleDestroyArgs(
            /* [in] */ ProcessRecord* owner,
            /* [in] */ Boolean oomAdj,
            /* [in] */ const String& reason);

    public:
        ProcessRecord* mOwner;
        Boolean mOomAdj;
        String mReason;
    };

private:
    class MyHandler
        : public HandlerBase
    {
    public:
        MyHandler(
           /* [in] */ ActivityStack* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        ActivityStack* mHost;
    };

    class ThumbnailRetriever
        : public ElRefBase
        , public Elastos::Droid::App::IThumbnailRetriever
    {
    public:
        CAR_INTERFACE_DECL()

        ThumbnailRetriever(
           /* [in] */ ActivityStack* host,
           /* [in] */ TaskAccessInfo* thumbs);

        CARAPI GetThumbnail(
            /* [in] */ Int32 index,
            /* [out] */ IBitmap** thumbnail);

    private:
        ActivityStack* mHost;
        TaskAccessInfo* mThumbs;
    };

public:
    ActivityStack(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IContext* context,
        /* [in] */ Boolean mainStack);

    ~ActivityStack();

    CARAPI_(AutoPtr<ActivityRecord>) GetTopRunningActivityLocked(
        /* [in] */ ActivityRecord* notTop);

    CARAPI_(AutoPtr<ActivityRecord>) GetTopRunningNonDelayedActivityLocked(
        /* [in] */ ActivityRecord* notTop);

    /**
     * This is a simplified version of topRunningActivityLocked that provides a number of
     * optional skip-over modes.  It is intended for use with the ActivityController hook only.
     *
     * @param token If non-null, any history records matching this token will be skipped.
     * @param taskId If non-zero, we'll attempt to skip over records with the same task ID.
     *
     * @return Returns the HistoryRecord of the next activity on the stack.
     */
    CARAPI_(AutoPtr<ActivityRecord>) GetTopRunningActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 taskId);

    CARAPI_(Int32) GetIndexOfTokenLocked(
        /* [in] */ IBinder* token);

    CARAPI_(Int32) GetIndexOfActivityLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI_(AutoPtr<ActivityRecord>) IsInStackLocked(
        /* [in] */ IBinder* token);

    CARAPI ShowAskCompatModeDialogLocked(
        /* [in] */ ActivityRecord* r);

    /*
     * Move the activities around in the stack to bring a user to the foreground.
     * @return whether there are any activities for the specified user.
     */
    CARAPI_(Boolean) SwitchUserLocked(
        /* [in] */ Int32 userId,
        /* [in] */ UserStartedState* uss);

    CARAPI RealStartActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean andResume,
        /* [in] */ Boolean checkConfig,
        /* [out] */ Boolean* succeeded);

    CARAPI StopIfSleepingLocked();

    CARAPI AwakeFromSleepingLocked();

    CARAPI ActivitySleptLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI CheckReadyForSleepLocked();

    CARAPI_(AutoPtr<IBitmap>) ScreenshotActivities(
        /* [in] */ ActivityRecord* who);

    CARAPI ActivityResumed(
        /* [in] */ IBinder* token);

    CARAPI ActivityPaused(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean timeout);

    CARAPI ActivityStoppedLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ IBundle* icicle,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ ICharSequence* description);

    /**
     * Make sure that all activities that need to be visible (that is, they
     * currently can be seen by the user) actually are.
     */
    CARAPI EnsureActivitiesVisibleLocked(
        /* [in] */ ActivityRecord* top,
        /* [in] */ ActivityRecord* starting,
        /* [in] */ const char* onlyThisProcess,
        /* [in] */ Int32 configChanges);

    /**
     * Version of ensureActivitiesVisible that can easily be called anywhere.
     */
    CARAPI EnsureActivitiesVisibleLocked(
        /* [in] */ ActivityRecord* starting,
        /* [in] */ Int32 configChanges);

    /**
     * Ensure that the top activity in the stack is resumed.
     *
     * @param prev The previously resumed activity, for when in the process
     * of pausing; can be null to call from elsewhere.
     *
     * @return Returns true if something is being resumed, or false if
     * nothing happened.
     */
    CARAPI_(Boolean) ResumeTopActivityLocked(
        /* [in] */ ActivityRecord* prev);

    Boolean ResumeTopActivityLocked(
        /* [in] */ ActivityRecord* prev,
        /* [in] */ IBundle* options);

    CARAPI ValidateAppTokensLocked();

    CARAPI StartActivityLocked(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IActivityInfo* aInfo,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 startFlags,
        /* [in] */ IBundle* options,
        /* [in] */ Boolean componentSpecified,
        /* [in] */ ArrayOf<ActivityRecord*>* outActivity,
        /* [out] */ Int32* status);

    CARAPI MoveHomeToFrontFromLaunchLocked(
        /* [in] */ Int32 launchFlags);

    CARAPI StartActivityUncheckedLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ ActivityRecord* sourceRecord,
        /* [in] */ Int32 startFlags,
        /* [in] */ Boolean doResume,
        /* [in] */ IBundle* options,
        /* [out] */ Int32* status);

    AutoPtr<IActivityInfo> ResolveActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Int32 startFlags,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ Int32 userId);

    CARAPI StartActivityMayWait(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ Int32 callingUid,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 startFlags,
        /* [in] */ const String& profileFile,
        /* [in] */ IParcelFileDescriptor* profileFd,
        /* [in] */ IActivityManagerWaitResult* outResult,
        /* [in] */ IConfiguration* config,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* status);

    CARAPI StartActivities(
        /* [in] */ IApplicationThread* caller,
        /* [in] */ Int32 callingUid,
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ ArrayOf<String>* resolvedTypes,
        /* [in] */ IBinder* resultTo,
        /* [in] */ IBundle* options,
        /* [in] */ Int32 userId,
        /* [out] */ Int32* stats);

    CARAPI ReportActivityLaunchedLocked(
        /* [in] */ Boolean timeout,
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int64 thisTime,
        /* [in] */ Int64 totalTime);

    CARAPI ReportActivityVisibleLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI SendActivityResultLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ ActivityRecord* r,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    CARAPI_(AutoPtr< List<AutoPtr<ActivityRecord> > >)
        ProcessStoppingActivitiesLocked(
        /* [in] */ Boolean remove);

    CARAPI ScheduleIdleLocked();

    CARAPI_(AutoPtr<ActivityRecord>) ActivityIdleInternal(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean fromTimeout,
        /* [in] */ IConfiguration* config);

    /**
     * @return Returns true if the activity is being finished, false if for
     * some reason it is being left as-is.
     */
    CARAPI_(Boolean) RequestFinishActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [in] */ const String& reason,
        /* [in] */ Boolean oomAdj);

    CARAPI FinishSubActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode);

    CARAPI_(Boolean) FinishActivityAffinityLocked(
        /* [in] */ IBinder* token);

    CARAPI FinishActivityResultsLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData);

    /**
     * @return Returns true if this activity has been removed from the history
     * list, or false if it is still in the list and will be removed later.
     */
    CARAPI_(Boolean) FinishActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 index,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [in] */ const String& reason,
        /* [in] */ Boolean oomAdj);

    /**
     * @return Returns true if this activity has been removed from the history
     * list, or false if it is still in the list and will be removed later.
     */
    CARAPI_(Boolean) FinishActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 index,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [in] */ const String& reason,
        /* [in] */ Boolean immediate,
        /* [in] */ Boolean oomAdj);

    /**
     * Perform the common clean-up of an activity record.  This is called both
     * as part of destroyActivityLocked() (when destroying the client-side
     * representation) and cleaning things up as a result of its hosting
     * processing going away, in which case there is no remaining client-side
     * state to destroy so only the cleanup here is needed.
     */
    CARAPI CleanUpActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Boolean cleanServices,
        /* [in] */ Boolean setState);

    CARAPI RemoveActivityFromHistoryLocked(
        /* [in] */ ActivityRecord* r);

    /**
     * Perform clean-up of service connections in an activity record.
     */
    CARAPI CleanUpActivityServicesLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI ScheduleDestroyActivities(
        /* [in] */ ProcessRecord* owner,
        /* [in] */ Boolean oomAdj,
        /* [in] */ const String& reason);

    CARAPI DestroyActivitiesLocked(
        /* [in] */ ProcessRecord* owner,
        /* [in] */ Boolean oomAdj,
        /* [in] */ const String& reason);

    /**
     * Destroy the current CLIENT SIDE instance of an activity.  This may be
     * called both when actually finishing an activity, or when performing
     * a configuration switch where we destroy the current client-side object
     * but then create a new client-side object for this same HistoryRecord.
     */
    CARAPI_(Boolean) DestroyActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Boolean removeFromApp,
        /* [in] */ Boolean oomAdj,
        /* [in] */ const String& reason);

    CARAPI ActivityDestroyed(
        /* [in] */ IBinder* token);

    CARAPI_(Boolean) RemoveHistoryRecordsForAppLocked(
        /* [in] */ ProcessRecord* app);

    /**
     * Move the current home activity's task (if one exists) to the front
     * of the stack.
     */
    CARAPI MoveHomeToFrontLocked();

    CARAPI UpdateTransitLocked(
        /* [in] */ Int32 transit,
        /* [in] */ IBundle* options);

    CARAPI MoveTaskToFrontLocked(
        /* [in] */ TaskRecord* tr,
        /* [in] */ ActivityRecord* reason,
        /* [in] */ IBundle* options);

    CARAPI_(Boolean) MoveTaskToBackLocked(
        /* [in] */ Int32 task,
        /* [in] */ ActivityRecord* reason);

    CARAPI_(AutoPtr<IActivityManagerTaskThumbnails>) GetTaskThumbnailsLocked(
        /* [in] */ TaskRecord* tr);

    CARAPI_(AutoPtr<IBitmap>) GetTaskTopThumbnailLocked(
        /* [in] */ TaskRecord* tr);

    CARAPI_(AutoPtr<ActivityRecord>) RemoveTaskActivitiesLocked(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 subTaskIndex,
        /* [in] */ Boolean taskRequired);

    CARAPI_(AutoPtr<TaskAccessInfo>) GetTaskAccessInfoLocked(
        /* [in] */ Int32 taskId,
        /* [in] */ Boolean inclThumbs);

    /**
     * Make sure the given activity matches the current configuration.  Returns
     * false if the activity had to be destroyed.  Returns true if the
     * configuration is the same, or the activity will remain running as-is
     * for whatever reason.  Ensures the HistoryRecord is updated with the
     * correct configuration and all other bookkeeping is handled.
     */
    CARAPI_(Boolean) EnsureActivityConfigurationLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 globalChanges);

    CARAPI DismissKeyguardOnNextActivityLocked();

    CARAPI HandleSleepTimeout();

    CARAPI HandlePauseTimeout(
        /* [in] */ ActivityRecord* activiyRecord);

    CARAPI HandleIdleTimeout(
        /* [in] */ ActivityRecord* activiyRecord);

    CARAPI HandleLaunchTick(
        /* [in] */ ActivityRecord* activiyRecord);

    CARAPI HandleStopTimeout(
        /* [in] */ ActivityRecord* activiyRecord);

    CARAPI HandleLaunchTimeout();

    CARAPI HandleResumeTopActivity();

    CARAPI HandleDestroyActivities(
        /* [in] */ ScheduleDestroyArgs* args);

private:
    CARAPI_(Boolean) OkToShow(
        /* [in] */ ActivityRecord* r);

    CARAPI_(Boolean) UpdateLRUListLocked(
        /* [in] */ ActivityRecord* r);

    /**
     * Returns the top activity in any existing task matching the given
     * Intent.  Returns null if no such task is found.
     */
    CARAPI_(AutoPtr<ActivityRecord>) FindTaskLocked(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info);

    /**
     * Returns the first activity (starting from the top of the stack) that
     * is the same as the given activity.  Returns null if no such activity
     * is found.
     */
    CARAPI_(AutoPtr<ActivityRecord>) FindActivityLocked(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info);

    CARAPI StartSpecificActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Boolean andResume,
        /* [in] */ Boolean checkConfig);

    CARAPI StartPausingLocked(
        /* [in] */ Boolean userLeaving,
        /* [in] */ Boolean uiSleeping);

    CARAPI CompletePauseLocked();

    /**
     * Once we know that we have asked an application to put an activity in
     * the resumed state (either by launching it or explicitly telling it),
     * this function updates the rest of our state to match that fact.
     */
    CARAPI CompleteResumeLocked(
        /* [in] */ ActivityRecord* next);

    CARAPI StartActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Boolean newTask,
        /* [in] */ Boolean doResume,
        /* [in] */ Boolean keepCurTransition,
        /* [in] */ IBundle* options);

    /**
     * Perform a reset of the given task, if needed as part of launching it.
     * Returns the new HistoryRecord at the top of the task.
     */
    CARAPI_(AutoPtr<ActivityRecord>) ResetTaskIfNeededLocked(
        /* [in] */ ActivityRecord* taskTop,
        /* [in] */ ActivityRecord* newActivity);

    /**
     * Perform clear operation as requested by
     * {@link Intent#FLAG_ACTIVITY_CLEAR_TOP}: search from the top of the
     * stack to the given task, then look for
     * an instance of that activity in the stack and, if found, finish all
     * activities on top of it and return the instance.
     *
     * @param newR Description of the new activity being started.
     * @return Returns the old activity that should be continued to be used,
     * or null if none was found.
     */
    CARAPI_(AutoPtr<ActivityRecord>) PerformClearTaskLocked(
        /* [in] */ Int32 taskId,
        /* [in] */ ActivityRecord* newR,
        /* [in] */ Int32 launchFlags);

    /**
     * Completely remove all activities associated with an existing
     * task starting at a specified index.
     */
    CARAPI PerformClearTaskAtIndexLocked(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 i);

    /**
     * Completely remove all activities associated with an existing task.
     */
    CARAPI PerformClearTaskLocked(
        /* [in] */ Int32 taskId);

    /**
     * Find the activity in the history stack within the given task.  Returns
     * the index within the history at which it's found, or < 0 if not found.
     */
    CARAPI_(Int32) FindActivityInHistoryLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 task);

    /**
     * Reorder the history stack so that the activity at the given index is
     * brought to the front.
     */
    CARAPI_(AutoPtr<ActivityRecord>) MoveActivityToFrontLocked(
        /* [in] */ Int32 where);

    CARAPI StopActivityLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI FinishCurrentActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 mode,
        /* [in] */ Boolean oomAdj,
        /* [out] */ ActivityRecord** fr);


    CARAPI FinishCurrentActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 index,
        /* [in] */ Int32 mode,
        /* [in] */ Boolean oomAdj,
        /* [out] */ ActivityRecord** fr);

    CARAPI RemoveTimeoutsForActivityLocked(
        /* [in] */ ActivityRecord* r);

    CARAPI RemoveHistoryRecordsForAppLocked(
        /* [in] */ List<AutoPtr<ActivityRecord> > & list,
        /* [in] */ ProcessRecord* app,
        /* [in] */ const String& listName);

    CARAPI FinishTaskMoveLocked(
        /* [in] */ Int32 task);

    CARAPI LogStartActivity(
        /* [in] */ Int32 tag,
        /* [in] */ ActivityRecord* r,
        /* [in] */ TaskRecord* task);

    CARAPI_(Boolean) RelaunchActivityLocked(
        /* [in] */ ActivityRecord* r,
        /* [in] */ Int32 changes,
        /* [in] */ Boolean andResume);

public:
    static const char* TAG;
    static const Boolean localLOGV;
    static const Boolean DEBUG_SWITCH;
    static const Boolean DEBUG_PAUSE;
    static const Boolean DEBUG_VISBILITY;
    static const Boolean DEBUG_USER_LEAVING;
    static const Boolean DEBUG_TRANSITION;
    static const Boolean DEBUG_RESULTS;
    static const Boolean DEBUG_CONFIGURATION;
    static const Boolean DEBUG_TASKS;
    static const Boolean DEBUG_CLEANUP;
    static const Boolean DEBUG_STATES;
    static const Boolean DEBUG_ADD_REMOVE;
    static const Boolean DEBUG_SAVED_STATE;

    static const Boolean VALIDATE_TOKENS;

    static const Int32 SLEEP_TIMEOUT_MSG;
    static const Int32 PAUSE_TIMEOUT_MSG;
    static const Int32 IDLE_TIMEOUT_MSG;
    static const Int32 IDLE_NOW_MSG;
    static const Int32 LAUNCH_TIMEOUT_MSG;
    static const Int32 DESTROY_TIMEOUT_MSG;
    static const Int32 RESUME_TOP_ACTIVITY_MSG;
    static const Int32 LAUNCH_TICK_MSG;
    static const Int32 STOP_TIMEOUT_MSG;
    static const Int32 DESTROY_ACTIVITIES_MSG;

    // How long we wait until giving up on the last activity telling us it
    // is idle.
    static const Int64 IDLE_TIMEOUT = 10*1000;

    // Ticks during which we check progress while waiting for an app to launch.
    static const Int32 LAUNCH_TICK = 500;

    // How long we wait until giving up on the last activity to pause.  This
    // is short because it directly impacts the responsiveness of starting the
    // next activity.
    static const Int32 PAUSE_TIMEOUT = 500;


    // How long we wait for the activity to tell us it has stopped before
    // giving up.  This is a good amount of time because we really need this
    // from the application in order to get its saved state.
    static const Int32 STOP_TIMEOUT = 10*1000;

    // How long we can hold the sleep wake lock before giving up.
    static const Int32 SLEEP_TIMEOUT = 5*1000;

    /**
     * How long we can hold the launch wake lock before giving up.
     */
    static const Int32 LAUNCH_TIMEOUT = 10*1000;

    // How long we wait until giving up on an activity telling us it has
    // finished destroying itself.
    static const Int64 DESTROY_TIMEOUT = 10*1000;

    // How long until we reset a task when the user returns to it.  Currently
    // disabled.
    static const Millisecond64 ACTIVITY_INACTIVE_RESET_TIME = 0;

    // How long between activity launches that we consider safe to not warn
    // the user about an unexpected activity being launched on top.
    static const Int64 START_WARN_TIME = 5*1000;

    // Set to false to disable the preview that is shown while a new activity
    // is being started.
    static const Boolean SHOW_APP_STARTING_PREVIEW = TRUE;

public:
    CActivityManagerService* mService; // owner
    Boolean mMainStack;

    AutoPtr<IContext> mContext;

    typedef List< AutoPtr<ActivityRecord> > ActivityRecordList;
    typedef typename ActivityRecordList::Iterator ActivityRecordListIterator;

    /**
     * The back history of all previous (and possibly still
     * running) activities.  It contains HistoryRecord objects.
     */
    List< AutoPtr<ActivityRecord> > mHistory;

    /**
     * Used for validating app tokens with window manager.
     */
    List< AutoPtr<IBinder> > mValidateAppTokens;

    /**
     * List of running activities, sorted by recent usage.
     * The first entry in the list is the least recently used.
     * It contains HistoryRecord objects.
     */
    List< AutoPtr<ActivityRecord> > mLRUActivities;

    /**
     * List of activities that are waiting for a new activity
     * to become visible before completing whatever operation they are
     * supposed to do.
     */
    List< AutoPtr<ActivityRecord> > mWaitingVisibleActivities;

    /**
     * List of activities that are ready to be stopped, but waiting
     * for the next activity to settle down before doing so.  It contains
     * HistoryRecord objects.
     */
    List< AutoPtr<ActivityRecord> > mStoppingActivities;

    /**
     * List of activities that are in the process of going to sleep.
     */
    List< AutoPtr<ActivityRecord> > mGoingToSleepActivities;

    /**
     * Animations that for the current transition have requested not to
     * be considered for the transition animation.
     */
    List< AutoPtr<ActivityRecord> > mNoAnimActivities;

    /**
     * List of activities that are ready to be finished, but waiting
     * for the previous activity to settle down before doing so.  It contains
     * HistoryRecord objects.
     */
    List< AutoPtr<ActivityRecord> > mFinishingActivities;

    /**
     * List of people waiting to find out about the next launched activity.
     */
    List< AutoPtr<IActivityManagerWaitResult> > mWaitingActivityLaunched;

    /**
     * List of people waiting to find out about the next visible activity.
     */
    List< AutoPtr<IActivityManagerWaitResult> > mWaitingActivityVisible;

    List< AutoPtr<UserStartedState> > mStartingUsers;

    /**
     * Set when the system is going to sleep, until we have
     * successfully paused the current activity and released our wake lock.
     * At that point the system is allowed to actually sleep.
     */
    AutoPtr<IPowerManagerWakeLock> mGoingToSleep;

    /**
     * We don't want to allow the device to go to sleep while in the process
     * of launching an activity.  This is primarily to allow alarm intent
     * receivers to launch an activity and get that to run before the device
     * goes back to sleep.
     */
    AutoPtr<IPowerManagerWakeLock> mLaunchingActivity;

    /**
     * When we are in the process of pausing an activity, before starting the
     * next one, this variable holds the activity that is currently being paused.
     */
    AutoPtr<ActivityRecord> mPausingActivity;

    /**
     * This is the last activity that we put into the paused state.  This is
     * used to determine if we need to do an activity transition while sleeping,
     * when we normally hold the top activity paused.
     */
    AutoPtr<ActivityRecord> mLastPausedActivity;

    /**
     * Current activity that is resumed, or null if there is none.
     */
    AutoPtr<ActivityRecord> mResumedActivity;

    /**
     * This is the last activity that has been started.  It is only used to
     * identify when multiple activities are started at once so that the user
     * can be warned they may not be in the activity they think they are.
     */
    AutoPtr<ActivityRecord> mLastStartedActivity;

    /**
     * Set when we know we are going to be calling updateConfiguration()
     * soon, so want to skip intermediate config checks.
     */
    Boolean mConfigWillChange;

    /**
     * Set to indicate whether to issue an onUserLeaving callback when a
     * newly launched activity is being brought in front of us.
     */
    Boolean mUserLeaving;

    Int64 mInitialStartTime;

        /**
     * Set when we have taken too long waiting to go to sleep.
     */
    Boolean mSleepTimeout;

    /**
     * Dismiss the keyguard after the next activity is displayed?
     */
    Boolean mDismissKeyguardOnNextActivity;

    Int32 mThumbnailWidth;
    Int32 mThumbnailHeight;

    AutoPtr<IHandler> mHandler;

private:
    Int32 mCurrentUser;

    static const Int32 FINISH_IMMEDIATELY = 0;
    static const Int32 FINISH_AFTER_PAUSE = 1;
    static const Int32 FINISH_AFTER_VISIBLE = 2;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ACTIVITYSTACK_H__
