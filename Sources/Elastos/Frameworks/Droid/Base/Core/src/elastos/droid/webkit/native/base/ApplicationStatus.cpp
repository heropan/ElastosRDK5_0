
#include "elastos/droid/webkit/native/base/ApplicationStatus.h"
#include "elastos/droid/webkit/native/base/api/ApplicationStatus_dec.h"
#include "elastos/droid/webkit/native/base/ApplicationState.h"
#include "elastos/droid/webkit/native/base/ActivityState.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"
// TODO #include "elastos/droid/content/pm/CActivityInfo.h"

#include <elastos/core/AutoLock.h>

using Elastos::Core::AutoLock;
using Elastos::Droid::App::EIID_IActivityLifecycleCallbacks;
using Elastos::Droid::Content::EIID_IContext;
// TODO using Elastos::Droid::Content::Pm::CActivityInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Utility::ICollection;
using Elastos::Utility::IIterator;
using Elastos::Utility::EIID_IIterator;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//===============================================================
//               ApplicationStatus::ActivityInfo
//===============================================================

ApplicationStatus::ActivityInfo::ActivityInfo()
    : mStatus(ActivityState::DESTROYED)
{
    mListeners = new ObserverList();
}

/**
 * @return The current {@link ActivityState} of the activity.
 */
Int32 ApplicationStatus::ActivityInfo::GetStatus()
{
    return mStatus;
}

/**
 * @param status The new {@link ActivityState} of the activity.
 */
void ApplicationStatus::ActivityInfo::SetStatus(
    /* [in] */ Int32 status)
{
    mStatus = status;
}

/**
 * @return A list of {@link ActivityStateListener}s listening to this activity.
 */
AutoPtr<ObserverList> ApplicationStatus::ActivityInfo::GetListeners()
{
    return mListeners;
}

//===============================================================
//      ApplicationStatus::InnerWindowFocusChangedListener
//===============================================================

ApplicationStatus::InnerWindowFocusChangedListener::InnerWindowFocusChangedListener()
{
}

void ApplicationStatus::InnerWindowFocusChangedListener::OnWindowFocusChanged(
    /* [in] */ IActivity* activity,
    /* [in] */ Boolean hasFocus)
{
    if (!hasFocus || activity == sActivity) return;

    Int32 state = ApplicationStatus::GetStateForActivity(activity);

    if (state != ActivityState::DESTROYED && state != ActivityState::STOPPED) {
        sActivity = activity;
    }

    // TODO(dtrainor): Notify of active activity change?
}

//===============================================================
//      ApplicationStatus::InnerActivityLifecycleCallbacks
//===============================================================

CAR_INTERFACE_IMPL(ApplicationStatus::InnerActivityLifecycleCallbacks, Object, IActivityLifecycleCallbacks);

ApplicationStatus::InnerActivityLifecycleCallbacks::InnerActivityLifecycleCallbacks()
{
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivityCreated(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* savedInstanceState)
{
    ApplicationStatus::OnStateChange(activity, ActivityState::CREATED);
    return NOERROR;
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivityDestroyed(
    /* [in] */ IActivity* activity)
{
    ApplicationStatus::OnStateChange(activity, ActivityState::DESTROYED);
    return NOERROR;
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivityPaused(
    /* [in] */ IActivity* activity)
{
    ApplicationStatus::OnStateChange(activity, ActivityState::PAUSED);
    return NOERROR;
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivityResumed(
    /* [in] */ IActivity* activity)
{
    ApplicationStatus::OnStateChange(activity, ActivityState::RESUMED);
    return NOERROR;
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivitySaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    return NOERROR;
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivityStarted(
    /* [in] */ IActivity* activity)
{
    ApplicationStatus::OnStateChange(activity, ActivityState::STARTED);
    return NOERROR;
}

ECode ApplicationStatus::InnerActivityLifecycleCallbacks::OnActivityStopped(
    /* [in] */ IActivity* activity)
{
    ApplicationStatus::OnStateChange(activity, ActivityState::STOPPED);
    return NOERROR;
}

//===============================================================
//             ApplicationStatus::InnerRunnable
//===============================================================

ApplicationStatus::InnerRunnable::InnerRunnable()
{
}

ECode ApplicationStatus::InnerRunnable::Run()
{
    if (ApplicationStatus::sNativeApplicationStateListener != NULL) return NOERROR;

    ApplicationStatus::sNativeApplicationStateListener = new InnerApplicationStateListener();
    RegisterApplicationStateListener(sNativeApplicationStateListener);

    return NOERROR;
}

//===============================================================
//       ApplicationStatus::InnerApplicationStateListener
//===============================================================

ApplicationStatus::InnerApplicationStateListener::InnerApplicationStateListener()
{
}

void ApplicationStatus::InnerApplicationStateListener::OnApplicationStateChange(
    /* [in] */ Int32 newState)
{
    ApplicationStatus::NativeOnApplicationStateChange(newState);
}

//===============================================================
//                    ApplicationStatus
//===============================================================

AutoPtr<IApplication> ApplicationStatus::sApplication;
Object ApplicationStatus::sCachedApplicationStateLock;
AutoPtr<IInteger32> ApplicationStatus::sCachedApplicationState;
AutoPtr<IActivity> ApplicationStatus::sActivity;
AutoPtr<ApplicationStatus::ApplicationStateListener> ApplicationStatus::sNativeApplicationStateListener;
AutoPtr<IMap> ApplicationStatus::sActivityInfo;
ObserverList ApplicationStatus::sGeneralActivityStateListeners;
ObserverList ApplicationStatus::sApplicationStateListeners;

ApplicationStatus::ApplicationStatus()
{
}

/**
 * Initializes the activity status for a specified application.
 *
 * @param application The application whose status you wish to monitor.
 */
void ApplicationStatus::Initialize(
    /* [in] */ BaseChromiumApplication* application)
{
    sApplication = application;

    AutoPtr<BaseChromiumApplication::WindowFocusChangedListener> listener = new InnerWindowFocusChangedListener();
    application->RegisterWindowFocusChangedListener(listener);

    AutoPtr<IActivityLifecycleCallbacks> callback = new InnerActivityLifecycleCallbacks();
    application->RegisterActivityLifecycleCallbacks(callback);
}

/**
 * Must be called by the main activity when it changes state.
 *
 * @param activity Current activity.
 * @param newState New state value.
 */
void ApplicationStatus::OnStateChange(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 newState)
{
    if (activity == NULL) {
        assert(0);
//        throw new IllegalArgumentException("null activity is not supported");
    }

    if (sActivity == NULL
            || newState == ActivityState::CREATED
            || newState == ActivityState::RESUMED
            || newState == ActivityState::STARTED) {
        sActivity = activity;
    }

    Int32 oldApplicationState = GetStateForApplication();

    if (newState == ActivityState::CREATED) {
        Boolean result;
        sActivityInfo->ContainsKey(activity, &result);
        assert(!result);
        AutoPtr<IActivityInfo> activityInfo;
        assert(0);
        // TODO
        // CActivityInfo::New((IActivityInfo**)&activityInfo);
        sActivityInfo->Put(activity, activityInfo);
    }

    // Invalidate the cached application state.
    synchronized (sCachedApplicationStateLock) {
        sCachedApplicationState = NULL;
    }

    AutoPtr<ActivityInfo> info;
    sActivityInfo->Get(activity, (IInterface**)&info);
    info->SetStatus(newState);

    // Notify all state observers that are specifically listening to this activity.
    AutoPtr<IIterator> iter;
    info->GetListeners()->GetIterator((IIterator**)&iter);
    Boolean bNext;
    for (iter->HasNext(&bNext); bNext; iter->HasNext(&bNext)) {
        AutoPtr<ActivityStateListener> listener;
        iter->GetNext((IInterface**)&listener);
        listener->OnActivityStateChange(activity, newState);
    }

    // Notify all state observers that are listening globally for all activity state
    // changes.
    AutoPtr<IIterator> iter2;
    sGeneralActivityStateListeners.GetIterator((IIterator**)&iter2);
    Boolean bNext2;
    for (iter2->HasNext(&bNext2); bNext2; iter2->HasNext(&bNext2)) {
        AutoPtr<ActivityStateListener> listener;
        iter2->GetNext((IInterface**)&listener);
        listener->OnActivityStateChange(activity, newState);
    }

    Int32 applicationState = GetStateForApplication();
    if (applicationState != oldApplicationState) {
        AutoPtr<IIterator> iter;
        sApplicationStateListeners.GetIterator((IIterator**)&iter);
        Boolean bNext;
        for (iter->HasNext(&bNext); bNext; iter->HasNext(&bNext)) {
            AutoPtr<ApplicationStateListener> listener;
            iter->GetNext((IInterface**)&listener);
            listener->OnApplicationStateChange(applicationState);
        }
    }

    if (newState == ActivityState::DESTROYED) {
        sActivityInfo->Remove(activity);
        if (activity == sActivity) sActivity = NULL;
    }
}

/**
 * Testing method to update the state of the specified activity.
 */
void ApplicationStatus::OnStateChangeForTesting(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 newState)
{
    OnStateChange(activity, newState);
}

/**
 * @return The most recent focused {@link Activity} tracked by this class.  Being focused means
 *         out of all the activities tracked here, it has most recently gained window focus.
 */
AutoPtr<IActivity> ApplicationStatus::GetLastTrackedFocusedActivity()
{
    return sActivity;
}

/**
 * @return A {@link List} of all non-destroyed {@link Activity}s.
 */
// List<WeakReference<Activity>> ApplicationStatus::GetRunningActivities()
// {
//     List<WeakReference<Activity>> activities = new ArrayList<WeakReference<Activity>>();
//     for (Activity activity : sActivityInfo.keySet()) {
//         activities.add(new WeakReference<Activity>(activity));
//     }
//     return activities;
// }

/**
 * @return The {@link Context} for the {@link Application}.
 */
AutoPtr<IContext> ApplicationStatus::GetApplicationContext()
{
    if (sApplication != NULL) {
        AutoPtr<IContext> appContext = IContext::Probe(sApplication);
        AutoPtr<IContext> context;
        appContext->GetApplicationContext((IContext**)&context);
        return context;
    }
    return NULL;
}

/**
 * Query the state for a given activity.  If the activity is not being tracked, this will
 * return {@link ActivityState#DESTROYED}.
 *
 * <p>
 * Please note that Chrome can have multiple activities running simultaneously.  Please also
 * look at {@link #getStateForApplication()} for more details.
 *
 * <p>
 * When relying on this method, be familiar with the expected life cycle state
 * transitions:
 * <a href="http://developer.android.com/guide/components/activities.html#Lifecycle">
 *   Activity Lifecycle
 * </a>
 *
 * <p>
 * During activity transitions (activity B launching in front of activity A), A will completely
 * paused before the creation of activity B begins.
 *
 * <p>
 * A basic flow for activity A starting, followed by activity B being opened and then closed:
 * <ul>
 *   <li> -- Starting Activity A --
 *   <li> Activity A - ActivityState.CREATED
 *   <li> Activity A - ActivityState.STARTED
 *   <li> Activity A - ActivityState.RESUMED
 *   <li> -- Starting Activity B --
 *   <li> Activity A - ActivityState.PAUSED
 *   <li> Activity B - ActivityState.CREATED
 *   <li> Activity B - ActivityState.STARTED
 *   <li> Activity B - ActivityState.RESUMED
 *   <li> Activity A - ActivityState.STOPPED
 *   <li> -- Closing Activity B, Activity A regaining focus --
 *   <li> Activity B - ActivityState.PAUSED
 *   <li> Activity A - ActivityState.STARTED
 *   <li> Activity A - ActivityState.RESUMED
 *   <li> Activity B - ActivityState.STOPPED
 *   <li> Activity B - ActivityState.DESTROYED
 * </ul>
 *
 * @param activity The activity whose state is to be returned.
 * @return The state of the specified activity (see {@link ActivityState}).
 */
Int32 ApplicationStatus::GetStateForActivity(
    /* [in] */ IActivity* activity)
{
    AutoPtr<ActivityInfo> info;
    sActivityInfo->Get(activity, (IInterface**)&info);
    return info != NULL ? info->GetStatus() : ActivityState::DESTROYED;
}

/**
 * @return The state of the application (see {@link ApplicationState}).
 */
Int32 ApplicationStatus::GetStateForApplication()
{
    synchronized(sCachedApplicationStateLock) {
        if (sCachedApplicationState == NULL) {
            assert(0);
            // TODO
            // CInteger32::New(DetermineApplicationState(), (IInteger32**)&sCachedApplicationState);
        }
    }

    Int32 value;
    sCachedApplicationState->GetValue(&value);
    return value;
}

/**
 * Checks whether or not any Activity in this Application is visible to the user.  Note that
 * this includes the PAUSED state, which can happen when the Activity is temporarily covered
 * by another Activity's Fragment (e.g.).
 * @return Whether any Activity under this Application is visible.
 */
Boolean ApplicationStatus::HasVisibleActivities()
{
    Int32 state = GetStateForApplication();
    return state == ApplicationState::HAS_RUNNING_ACTIVITIES
            || state == ApplicationState::HAS_PAUSED_ACTIVITIES;
}

/**
 * Checks to see if there are any active Activity instances being watched by ApplicationStatus.
 * @return True if all Activities have been destroyed.
 */
Boolean ApplicationStatus::IsEveryActivityDestroyed()
{
    Boolean result;
    sActivityInfo->IsEmpty(&result);
    return result;
}

/**
 * Registers the given listener to receive state changes for all activities.
 * @param listener Listener to receive state changes.
 */
void ApplicationStatus::RegisterStateListenerForAllActivities(
    /* [in] */ ActivityStateListener* listener)
{
    sGeneralActivityStateListeners.AddObserver((IObject*)listener);
}

/**
 * Registers the given listener to receive state changes for {@code activity}.  After a call to
 * {@link ActivityStateListener#onActivityStateChange(Activity, int)} with
 * {@link ActivityState#DESTROYED} all listeners associated with that particular
 * {@link Activity} are removed.
 * @param listener Listener to receive state changes.
 * @param activity Activity to track or {@code null} to track all activities.
 */
void ApplicationStatus::RegisterStateListenerForActivity(
    /* [in] */ ActivityStateListener* listener,
    /* [in] */ IActivity* activity)
{
    assert(activity != NULL);

    AutoPtr<ActivityInfo> info;
    sActivityInfo->Get(activity, (IInterface**)&info);
    assert(info != NULL && info->GetStatus() != ActivityState::DESTROYED);
    info->GetListeners()->AddObserver((IObject*)listener);
}

/**
 * Unregisters the given listener from receiving activity state changes.
 * @param listener Listener that doesn't want to receive state changes.
 */
void ApplicationStatus::UnregisterActivityStateListener(
    /* [in] */ ActivityStateListener* listener)
{
    sGeneralActivityStateListeners.RemoveObserver((IObject*)listener);

    // Loop through all observer lists for all activities and remove the listener.
    AutoPtr<ICollection> collection;
    sActivityInfo->GetValues((ICollection**)&collection);
    AutoPtr<IIterator> iter;
    collection->GetIterator((IIterator**)&iter);
    AutoPtr<ActivityInfo> info;
    Boolean bNext;
    for (iter->HasNext(&bNext); bNext; iter->HasNext(&bNext)) {
        iter->GetNext((IInterface**)&info);
        info->GetListeners()->RemoveObserver((IObject*)listener);
    }
}

/**
 * Registers the given listener to receive state changes for the application.
 * @param listener Listener to receive state state changes.
 */
void ApplicationStatus::RegisterApplicationStateListener(
    /* [in] */ ApplicationStateListener* listener)
{
    sApplicationStateListeners.AddObserver((IObject*)listener);
}

/**
 * Unregisters the given listener from receiving state changes.
 * @param listener Listener that doesn't want to receive state changes.
 */
void ApplicationStatus::UnregisterApplicationStateListener(
    /* [in] */ ApplicationStateListener* listener)
{
    sApplicationStateListeners.RemoveObserver((IObject*)listener);
}

/**
 * Registers the single thread-safe native activity status listener.
 * This handles the case where the caller is not on the main thread.
 * Note that this is used by a leaky singleton object from the native
 * side, hence lifecycle management is greatly simplified.
 */
//@CalledByNative
void ApplicationStatus::RegisterThreadSafeNativeApplicationStateListener()
{
    AutoPtr<IRunnable> runnable = new InnerRunnable();
    ThreadUtils::RunOnUiThread(runnable);
}

/**
 * Determines the current application state as defined by {@link ApplicationState}.  This will
 * loop over all the activities and check their state to determine what the general application
 * state should be.
 * @return HAS_RUNNING_ACTIVITIES if any activity is not paused, stopped, or destroyed.
 *         HAS_PAUSED_ACTIVITIES if none are running and one is paused.
 *         HAS_STOPPED_ACTIVITIES if none are running/paused and one is stopped.
 *         HAS_DESTROYED_ACTIVITIES if none are running/paused/stopped.
 */
Int32 ApplicationStatus::DetermineApplicationState()
{
    Boolean hasPausedActivity = FALSE;
    Boolean hasStoppedActivity = FALSE;

    AutoPtr<ICollection> collection;
    sActivityInfo->GetValues((ICollection**)&collection);
    AutoPtr<IIterator> iter = IIterator::Probe(collection);
    Boolean bNext;
    for (iter->HasNext(&bNext); bNext; iter->HasNext(&bNext)) {
        AutoPtr<ActivityInfo> info;
        iter->GetNext((IInterface**)&info);
        Int32 state = info->GetStatus();
        if (state != ActivityState::PAUSED
                && state != ActivityState::STOPPED
                && state != ActivityState::DESTROYED) {
            return ApplicationState::HAS_RUNNING_ACTIVITIES;
        }
        else if (state == ActivityState::PAUSED) {
            hasPausedActivity = TRUE;
        }
        else if (state == ActivityState::STOPPED) {
            hasStoppedActivity = TRUE;
        }
    }

    if (hasPausedActivity) return ApplicationState::HAS_PAUSED_ACTIVITIES;
    if (hasStoppedActivity) return ApplicationState::HAS_STOPPED_ACTIVITIES;

    return ApplicationState::HAS_DESTROYED_ACTIVITIES;
}

// Called to notify the native side of state changes.
// IMPORTANT: This is always called on the main thread!
void ApplicationStatus::NativeOnApplicationStateChange(
    /* [in] */ Int32 newState)
{
    Elastos_ApplicationStatus_nativeOnApplicationStateChange(newState);
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
