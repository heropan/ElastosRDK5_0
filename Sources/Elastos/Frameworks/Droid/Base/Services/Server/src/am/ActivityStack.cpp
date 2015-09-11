
#include "am/ActivityStack.h"
#include "am/CActivityManagerService.h"
#include "am/ActivityRecord.h"
#include "am/ActiveServices.h"
#include "am/ProcessRecord.h"
#include "app/AppGlobals.h"
#include "os/SystemClock.h"
#include "os/UserHandle.h"
#include "os/Process.h"
#include "os/Binder.h"
#include "os/Handler.h"
#include "util/TimeUtils.h"
//#include "util/CEventLogTags.h"
//#include "view/WindowManagerPolicy.h"
#include "Manifest.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::CIntentSender;
using Elastos::Droid::Content::EIID_IIIntentSender;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Utility::TimeUtils;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Utility::IEventLogTags;
using Elastos::Droid::App::IActivityManagerWaitResult;
using Elastos::Droid::App::CActivityOptionsHelper;
using Elastos::Droid::App::AppGlobals;
using Elastos::Droid::Internal::App::IHeavyWeightSwitcherActivity;

#define UNUSED(x) (void)x

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const char* ActivityStack::TAG  = CActivityManagerService::TAG;
const Boolean ActivityStack::localLOGV = CActivityManagerService::localLOGV;
const Boolean ActivityStack::DEBUG_SWITCH = CActivityManagerService::DEBUG_SWITCH;
const Boolean ActivityStack::DEBUG_PAUSE = CActivityManagerService::DEBUG_PAUSE;
const Boolean ActivityStack::DEBUG_VISBILITY = CActivityManagerService::DEBUG_VISBILITY;
const Boolean ActivityStack::DEBUG_USER_LEAVING = CActivityManagerService::DEBUG_USER_LEAVING;
const Boolean ActivityStack::DEBUG_TRANSITION = CActivityManagerService::DEBUG_TRANSITION;
const Boolean ActivityStack::DEBUG_RESULTS = CActivityManagerService::DEBUG_RESULTS;
const Boolean ActivityStack::DEBUG_CONFIGURATION = CActivityManagerService::DEBUG_CONFIGURATION;
const Boolean ActivityStack::DEBUG_TASKS = CActivityManagerService::DEBUG_TASKS;
const Boolean ActivityStack::DEBUG_CLEANUP = CActivityManagerService::DEBUG_CLEANUP;
const Boolean ActivityStack::DEBUG_STATES = FALSE;
const Boolean ActivityStack::DEBUG_ADD_REMOVE = FALSE;
const Boolean ActivityStack::DEBUG_SAVED_STATE = FALSE;

const Boolean ActivityStack::VALIDATE_TOKENS = CActivityManagerService::VALIDATE_TOKENS;

const Int32 ActivityStack::SLEEP_TIMEOUT_MSG = 100;//ActivityManagerService.FIRST_ACTIVITY_STACK_MSG;
const Int32 ActivityStack::PAUSE_TIMEOUT_MSG = 100 + 1; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 1;
const Int32 ActivityStack::IDLE_TIMEOUT_MSG = 100 + 2; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 2;
const Int32 ActivityStack::IDLE_NOW_MSG = 100 + 3; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 3;
const Int32 ActivityStack::LAUNCH_TIMEOUT_MSG = 100 + 4; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 4;
const Int32 ActivityStack::DESTROY_TIMEOUT_MSG = 100 + 5; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 5;
const Int32 ActivityStack::RESUME_TOP_ACTIVITY_MSG = 100 + 6; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 6;
const Int32 ActivityStack::LAUNCH_TICK_MSG = 100 + 7; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 7;
const Int32 ActivityStack::STOP_TIMEOUT_MSG = 100 + 8; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 8;
const Int32 ActivityStack::DESTROY_ACTIVITIES_MSG = 100 + 9; //= ActivityManagerService.FIRST_ACTIVITY_STACK_MSG + 9;

const Int64 ActivityStack::IDLE_TIMEOUT;
const Int32 ActivityStack::LAUNCH_TICK;
const Int32 ActivityStack::PAUSE_TIMEOUT;
const Int32 ActivityStack::STOP_TIMEOUT;
const Int32 ActivityStack::SLEEP_TIMEOUT;
const Int32 ActivityStack::LAUNCH_TIMEOUT;
const Int64 ActivityStack::DESTROY_TIMEOUT;
const Millisecond64 ActivityStack::ACTIVITY_INACTIVE_RESET_TIME;
const Int64 ActivityStack::START_WARN_TIME;
const Boolean ActivityStack::SHOW_APP_STARTING_PREVIEW;

const Int32 ActivityStack::FINISH_IMMEDIATELY;
const Int32 ActivityStack::FINISH_AFTER_PAUSE;
const Int32 ActivityStack::FINISH_AFTER_VISIBLE;

//==================================================================================
// ActivityStack::ScheduleDestroyArgs
//==================================================================================
CAR_INTERFACE_IMPL_LIGHT(ActivityStack::ScheduleDestroyArgs, IInterface)

ActivityStack::ScheduleDestroyArgs::ScheduleDestroyArgs(
    /* [in] */ ProcessRecord* owner,
    /* [in] */ Boolean oomAdj,
    /* [in] */ const String& reason)
    : mOwner(owner)
    , mOomAdj(oomAdj)
    , mReason(reason)
{}


//==================================================================================
// ActivityStack::MyHandler
//==================================================================================
ECode ActivityStack::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch (what) {
        case ActivityStack::SLEEP_TIMEOUT_MSG:
            mHost->HandleSleepTimeout();
            break;

        case ActivityStack::PAUSE_TIMEOUT_MSG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ActivityRecord* r = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));
            mHost->HandlePauseTimeout(r);
            break;
        }

        case ActivityStack::IDLE_TIMEOUT_MSG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ActivityRecord* r = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));
            mHost->HandleIdleTimeout(r);
            break;
        }

        case ActivityStack::LAUNCH_TICK_MSG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ActivityRecord* r = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));
            mHost->HandleLaunchTick(r);
            break;
        }

        case ActivityStack::DESTROY_TIMEOUT_MSG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ActivityRecord* r = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));

            // We don't at this point know if the activity is fullscreen,
            // so we need to be conservative and assume it isn't.
            Slogger::W(ActivityStack::TAG, "Activity destroy timeout for %s", r->ToString().string());
            mHost->ActivityDestroyed(r != NULL ? IBinder::Probe(r->mAppToken) : NULL);
            break;
        }

        case ActivityStack::IDLE_NOW_MSG: {
            AutoPtr<ActivityRecord> r;
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            if (obj != NULL) {
                r = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));
            }
            mHost->ActivityIdleInternal(r != NULL ? IBinder::Probe(r->mAppToken) : NULL, FALSE, NULL);
            break;
        }

        case ActivityStack::LAUNCH_TIMEOUT_MSG: {
            mHost->HandleLaunchTimeout();
            break;
        }

        case ActivityStack::RESUME_TOP_ACTIVITY_MSG: {
            mHost->HandleResumeTopActivity();
            break;
        }

        case ActivityStack::STOP_TIMEOUT_MSG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ActivityRecord* r = reinterpret_cast<ActivityRecord*>(obj->Probe(EIID_ActivityRecord));
            mHost->HandleStopTimeout(r);
            break;
        }

        case ActivityStack::DESTROY_ACTIVITIES_MSG: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            ScheduleDestroyArgs * args = (ScheduleDestroyArgs*)obj.Get();
            mHost->HandleDestroyActivities(args);
            break;
        }
    }

    return NOERROR;
}

//==================================================================================
// ActivityStack::ThumbnailRetriever
//==================================================================================
CAR_INTERFACE_IMPL(ActivityStack::ThumbnailRetriever, IThumbnailRetriever)

ActivityStack::ThumbnailRetriever::ThumbnailRetriever(
    /* [in] */  ActivityStack* host,
    /* [in] */ TaskAccessInfo* thumbs)
    : mHost(host)
    , mThumbs(thumbs)
{}

ECode ActivityStack::ThumbnailRetriever::GetThumbnail(
    /* [in] */ Int32 index,
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);
    *thumbnail = NULL;

    if (index < 0 || index >= mThumbs->mSubtasks.GetSize()) {
        return NOERROR;
    }
    AutoPtr<TaskAccessInfo::SubTask> sub = mThumbs->mSubtasks[index];
    AutoPtr<ActivityRecord> resumed = mHost->mResumedActivity;
    if (resumed != NULL && resumed->mThumbHolder == sub->mHolder) {
        *thumbnail =  resumed->mStack->ScreenshotActivities(resumed);
        REFCOUNT_ADD(*thumbnail);
        return NOERROR;
    }
    *thumbnail = sub->mHolder->mLastThumbnail;
    REFCOUNT_ADD(*thumbnail);
    return NOERROR;
}

ECode ActivityStack::HandleSleepTimeout()
{
    AutoLock lock(mService->mLock);
    if (mService->IsSleeping()) {
        Slogger::W(TAG, "Sleep timeout!  Sleeping now.");
        mSleepTimeout = TRUE;
        CheckReadyForSleepLocked();
    }
    return NOERROR;
}

ECode ActivityStack::HandlePauseTimeout(
    /* [in] */ ActivityRecord* r)
{
    // We don't at this point know if the activity is fullscreen,
    // so we need to be conservative and assume it isn't.
    Slogger::W(TAG, "Activity pause timeout for %s", r->ToString().string());
    {
        AutoLock lock(mService->mLock);
        if (r != NULL && r->mApp != NULL) {
            String rStr;
            r->ToString(&rStr);
            mService->LogAppTooSlow(r->mApp, r->mPauseTime, String("pausing ") + rStr);
        }
    }
    ECode ec = ActivityPaused((r != NULL && r->mAppToken != NULL)
        ? IBinder::Probe(r->mAppToken) : NULL, TRUE);
    return ec;
}

ECode ActivityStack::HandleIdleTimeout(
    /* [in] */ ActivityRecord* r)
{
    if (mService->mDidDexOpt) {
        mService->mDidDexOpt = FALSE;

        AutoPtr<IMessage> nmsg;
        mHandler->ObtainMessage(IDLE_TIMEOUT_MSG, (IMessage**)&nmsg);
        nmsg->SetObj(r->Probe(EIID_IInterface));
        Boolean result;
        return mHandler->SendMessageDelayed(nmsg, IDLE_TIMEOUT, &result);
    }

    // We don't at this point know if the activity is fullscreen,
    // so we need to be conservative and assume it isn't.
    Slogger::W(TAG, "Activity idle timeout for %s", r->ToString().string());
    ActivityIdleInternal(r != NULL ? IBinder::Probe(r->mAppToken) : NULL, TRUE, NULL);
    return NOERROR;
}

ECode ActivityStack::HandleLaunchTick(
    /* [in] */ ActivityRecord* r)
{
    AutoLock lock(mService->mLock);
    if (r != NULL && r->ContinueLaunchTickingLocked()) {
        String rStr;
        r->ToString(&rStr);
        mService->LogAppTooSlow(r->mApp, r->mLaunchTickTime,
            String("launching ") + rStr);
    }
    return NOERROR;
}

ECode ActivityStack::HandleStopTimeout(
    /* [in] */ ActivityRecord* r)
{
    // We don't at this point know if the activity is fullscreen,
    // so we need to be conservative and assume it isn't.
    Slogger::W(TAG, "Activity stop timeout for %s", r->ToString().string());
    AutoLock lock(mService->mLock);
    return ActivityStoppedLocked(r, NULL, NULL, NULL);
}

ECode ActivityStack::HandleLaunchTimeout()
{
    if (mService->mDidDexOpt) {
        mService->mDidDexOpt = FALSE;

        AutoPtr<IMessage> nmsg;
        mHandler->ObtainMessage(LAUNCH_TIMEOUT_MSG, (IMessage**)&nmsg);
        Boolean result;
        return mHandler->SendMessageDelayed(nmsg, LAUNCH_TIMEOUT, &result);
    }

    AutoLock lock(mService->mLock);
    Boolean held;
    mLaunchingActivity->IsHeld(&held);
    if (held) {
        Slogger::W(TAG, "Launch timeout has expired, giving up wake lock!");
        mLaunchingActivity->ReleaseLock();
    }
    return NOERROR;
}

ECode ActivityStack::HandleResumeTopActivity()
{
    AutoLock lock(mService->mLock);

    ResumeTopActivityLocked(NULL);
    return NOERROR;
}

ECode ActivityStack::HandleDestroyActivities(
    /* [in] */ ScheduleDestroyArgs* args)
{
    AutoLock lock(mService->mLock);
    DestroyActivitiesLocked(args->mOwner, args->mOomAdj, args->mReason);
    return NOERROR;
}

ActivityStack::ActivityStack(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ IContext* context,
    /* [in] */ Boolean mainStack)
    : mConfigWillChange(FALSE)
    , mUserLeaving(FALSE)
    , mInitialStartTime(0)
    , mSleepTimeout(FALSE)
    , mDismissKeyguardOnNextActivity(FALSE)
    , mCurrentUser(0)
    , mThumbnailHeight(-1)
    , mThumbnailWidth(-1)
{
    mService = service;
    mContext = context;
    mMainStack = mainStack;

    mHandler = new MyHandler(this);

    AutoPtr<IInterface> s;
    context->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&s);
    AutoPtr<IPowerManager> pm = IPowerManager::Probe(s);
    pm->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, String("ActivityManager-Sleep"), (IPowerManagerWakeLock**)&mGoingToSleep);
    pm->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, String("ActivityManager-Launch"),
            (IPowerManagerWakeLock**)&mLaunchingActivity);
    mLaunchingActivity->SetReferenceCounted(FALSE);
}

ActivityStack::~ActivityStack()
{}

Boolean ActivityStack::OkToShow(
    /* [in] */ ActivityRecord* r)
{
    VALIDATE_NOT_NULL(r);

    Int32 flags;
    r->mInfo->GetFlags(&flags);
    return r->mUserId == mCurrentUser
            || (flags & IActivityInfo::FLAG_SHOW_ON_LOCK_SCREEN) != 0;
}

AutoPtr<ActivityRecord> ActivityStack::GetTopRunningActivityLocked(
    /* [in] */ ActivityRecord* notTop)
{
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        ActivityRecord* r = *rit;
        if (!r->mFinishing && r != notTop && OkToShow(r)) {
            return r;
        }
        ++rit;
    }
    return NULL;
}

AutoPtr<ActivityRecord> ActivityStack::GetTopRunningNonDelayedActivityLocked(
    /* [in] */ ActivityRecord* notTop)
{
    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        ActivityRecord* r = *rit;
        if (!r->mFinishing && !r->mDelayedResume && r != notTop && OkToShow(r)) {
            return r;
        }
        ++rit;
    }
    return NULL;
}

AutoPtr<ActivityRecord> ActivityStack::GetTopRunningActivityLocked(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 taskId)
{
    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        ActivityRecord* r = *rit;
        // Note: the taskId check depends on real taskId fields being non-zero
        if (!r->mFinishing &&
                (token != IBinder::Probe(r->mAppToken)) && (taskId != r->mTask->mTaskId) && OkToShow(r)) {
            return r;
        }
        ++rit;
    }
    return NULL;
}

Int32 ActivityStack::GetIndexOfTokenLocked(
    /* [in] */ IBinder* token)
{
    if (token == NULL)
        return -1;

    AutoPtr<ActivityRecord> r;
    ActivityRecord::ForToken(token, (ActivityRecord**)&r);
    return GetIndexOfActivityLocked(r);
}

Int32 ActivityStack::GetIndexOfActivityLocked(
    /* [in] */ ActivityRecord* r)
{
    List<AutoPtr<ActivityRecord> >::Iterator iter = mHistory.Begin();
    for (Int32 i = 0; iter != mHistory.End(); ++iter, ++i) {
        if (iter->Get() == r) {
            return i;
        }
    }

    return -1;
}

AutoPtr<ActivityRecord> ActivityStack::IsInStackLocked(
    /* [in] */ IBinder* token)
{
    AutoPtr<ActivityRecord> r;
    ActivityRecord::ForToken(token, (ActivityRecord**)&r);
    if (GetIndexOfActivityLocked(r) >= 0) {
        return r;
    }

    return NULL;
}

Boolean ActivityStack::UpdateLRUListLocked(
    /* [in] */ ActivityRecord* r)
{
    Boolean hadit = FALSE;
    List< AutoPtr<ActivityRecord> >::Iterator it =
            Find(mLRUActivities.Begin(), mLRUActivities.End(), AutoPtr<ActivityRecord>(r));
    if (it != mLRUActivities.End()) {
        mLRUActivities.Erase(it);
        hadit = TRUE;
    }
    mLRUActivities.PushBack(r);
    return hadit;
}

AutoPtr<ActivityRecord> ActivityStack::FindTaskLocked(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info)
{
    AutoPtr<IComponentName> cls;
    intent->GetComponent((IComponentName**)&cls);
    String cname, target;
    info->GetPackageName(&cname);
    info->GetTargetActivity(&target);
    if (!target.IsNull()) {
        cls = NULL;
        CComponentName::New(cname, target, (IComponentName**)&cls);
    }

    AutoPtr<TaskRecord> cp;

    AutoPtr<IApplicationInfo> appInfo;
    info->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    Int32 userId = UserHandle::GetUserId(uid);
    List<AutoPtr<ActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        ActivityRecord* r = *rit;
        if (!r->mFinishing && r->mTask != cp && r->mUserId == userId
                && r->mLaunchMode != IActivityInfo::LAUNCH_SINGLE_INSTANCE) {
            cp = r->mTask;
            //Slogger::I(TAG, "Comparing existing cls=" + r.task.intent.getComponent().flattenToShortString()
            //        + "/aff=" + r.task.affinity + " to new cls="
            //        + intent.getComponent().flattenToShortString() + "/aff=" + taskAffinity);
            AutoPtr<IComponentName> component;
            Boolean isEqual;
            if (!r->mTask->mAffinity.IsNull()) {
                String task;
                info->GetTaskAffinity(&task);
                if (r->mTask->mAffinity.Equals(task)) {
                    //Slogger::I(TAG, "Found matching affinity!");
                    return r;
                }
            }
            else if (r->mTask->mIntent != NULL
                    && (r->mTask->mIntent->GetComponent((IComponentName**)&component),
                        component->Equals(cls, &isEqual), isEqual)) {
                //Slogger::I(TAG, "Found matching class!");
                //dump();
                //Slogger::I(TAG, "For Intent " + intent + " bringing to top: " + r.intent);
                return r;
            }
            else if (r->mTask->mAffinityIntent != NULL
                    && (r->mTask->mAffinityIntent->GetComponent((IComponentName**)&component),
                        component->Equals(cls, &isEqual), isEqual)) {
                //Slogger::I(TAG, "Found matching class!");
                //dump();
                //Slogger::I(TAG, "For Intent " + intent + " bringing to top: " + r.intent);
                return r;
            }
        }
    }

    return NULL;
}

AutoPtr<ActivityRecord> ActivityStack::FindActivityLocked(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info)
{
    AutoPtr<IComponentName> cls;
    intent->GetComponent((IComponentName**)&cls);
    String cname, target;
    info->GetPackageName(&cname);
    info->GetTargetActivity(&target);
    if (!target.IsNull()) {
        cls = NULL;
        CComponentName::New(cname, target, (IComponentName**)&cls);
    }
    AutoPtr<IApplicationInfo> appInfo;
    info->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    const Int32 userId = UserHandle::GetUserId(uid);

    List<AutoPtr<ActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        ActivityRecord* r = *rit;
        if (!r->mFinishing) {
            AutoPtr<IComponentName> component;
            Boolean isEqual;
            r->mIntent->GetComponent((IComponentName**)&component);
            if (component->Equals(cls, &isEqual), isEqual /*&& r->mUserId == userId*/) {
                //Slogger::I(TAG, "Found matching class!");
                //dump();
                //Slogger::I(TAG, "For Intent " + intent + " bringing to top: " + r.intent);
                return r;
            }
        }
    }

    return NULL;
}

ECode ActivityStack::ShowAskCompatModeDialogLocked(
    /* [in] */ ActivityRecord* r)
{
    AutoPtr<IMessage> msg;
    mService->mHandler->ObtainMessage(
        CActivityManagerService::SHOW_COMPAT_MODE_DIALOG_MSG,
        (IMessage**)&msg);
    msg->SetObj(r->mTask->mAskedCompatMode ? NULL : r->Probe(EIID_IInterface));
    Boolean result;
    return mService->mHandler->SendMessage(msg, &result);
}

Boolean ActivityStack::SwitchUserLocked(
    /* [in] */ Int32 userId,
    /* [in] */ UserStartedState* uss)
{
    mCurrentUser = userId;
    mStartingUsers.PushBack(uss);

    // Only one activity? Nothing to do...
    if (mHistory.GetSize() < 2) {
        return FALSE;
    }

    Boolean haveActivities = FALSE;
    // Check if the top activity is from the new user.
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    AutoPtr<ActivityRecord> top = *rit;
    if (top->mUserId == userId) return TRUE;
    // Otherwise, move the user's activities to the top.
    List< AutoPtr<ActivityRecord> >::Iterator it = mHistory.Begin();
    while(it != mHistory.End()) {
        AutoPtr<ActivityRecord> r = *it;
        if (r->mUserId == userId) {
            AutoPtr<ActivityRecord> moveToTop = *it;
            it = mHistory.Erase(it);
            mHistory.PushBack(moveToTop);
            // No need to check the top one now
            haveActivities = TRUE;
        }
        else {
            ++it;
        }
    }
    // Transition from the old top to the new top
    ResumeTopActivityLocked(top);
    return haveActivities;
}

ECode ActivityStack::RealStartActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean andResume,
    /* [in] */ Boolean checkConfig,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    r->StartFreezingScreenLocked(app, 0);
    mService->mWindowManager->SetAppVisibility(IBinder::Probe(r->mAppToken), TRUE);

    // schedule launch ticks to collect information about slow apps.
    r->StartLaunchTickingLocked();

    // Have the window manager re-evaluate the orientation of
    // the screen based on the new activity order.  Note that
    // as a result of this, it can call back into the activity
    // manager with a new orientation.  We don't care about that,
    // because the activity is not currently running so we are
    // just restarting it anyway.
    if (checkConfig) {
        AutoPtr<IConfiguration> config;
        Boolean res;
        mService->mWindowManager->UpdateOrientationFromAppTokens(
                mService->mConfiguration,
                (r->MayFreezeScreenLocked(app, &res), res) ? IBinder::Probe(r->mAppToken) : NULL,
                (IConfiguration**)&config);
        mService->UpdateConfigurationLocked(config, r, FALSE, FALSE);
    }

    r->mApp = app;
    app->mWaitingToKill = NULL;

    if (localLOGV) {
        String rStr;
        r->ToString(&rStr);
        Slogger::V(TAG, "Launching: %s", rStr.string());
    }

    List< AutoPtr<ActivityRecord> >::Iterator it = Find(
        app->mActivities.Begin(), app->mActivities.End(), AutoPtr<ActivityRecord>(r));
    if (it == app->mActivities.End()) {
        app->mActivities.PushBack(r);
    }
    mService->UpdateLruProcessLocked(app, TRUE);

   // try {
    if (app->mThread == NULL) {
        return E_REMOTE_EXCEPTION;
    }
    AutoPtr<IObjectContainer> results;
    AutoPtr<IObjectContainer> newIntents;
    if (andResume) {
        if (r->mResults != NULL) {
            CParcelableObjectContainer::New((IObjectContainer**)&results);
            List<AutoPtr<ActivityResult> >::Iterator it;
            for (it = r->mResults->Begin(); it != r->mResults->End(); ++it) {
                results->Add((*it)->mResultInfo);
            }
        }
        if (r->mNewIntents != NULL) {
            CParcelableObjectContainer::New((IObjectContainer**)&newIntents);
            List<AutoPtr<IIntent> >::Iterator it;
            for (it = r->mNewIntents->Begin(); it != r->mNewIntents->End(); ++it) {
                newIntents->Add((IIntent*)*it);
            }
        }
    }
    if (DEBUG_SWITCH) {
        String rStr, icicleStr;
        r->ToString(&rStr);
        if (r->mIcicle.Get() != NULL) r->mIcicle->ToString(&icicleStr);
        Slogger::V(TAG, "Launching: %s icicle=%s with results=%p newIntents=%p andResume=%d", rStr.string(), icicleStr.string(),
                results.Get(), newIntents.Get(), andResume);
    }
    if (andResume) {
       // EventLog.writeEvent(IEventLogTags.AM_RESTART_ACTIVITY,
       //         r.userId, System.identityHashCode(r),
       //         r.task.taskId, r.shortComponentName);
    }
    if (r->mIsHomeActivity) {
        mService->mHomeProcess = app;
    }
    AutoPtr<IComponentName> component;
    r->mIntent->GetComponent((IComponentName**)&component);
    String cname;
    component->GetPackageName(&cname);
    mService->EnsurePackageDexOpt(cname);
    r->mSleeping = FALSE;
    r->mForceNewConfig = FALSE;
    ShowAskCompatModeDialogLocked(r);
    AutoPtr<IApplicationInfo> appInfo;
    r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    r->mCompat = mService->CompatibilityInfoForPackageLocked(appInfo);
    String profileFile;
    AutoPtr<IParcelFileDescriptor> profileFd = NULL;
    Boolean profileAutoStop = FALSE;
    if (!mService->mProfileApp.IsNull() && mService->mProfileApp.Equals(app->mProcessName)) {
        if (mService->mProfileProc == NULL || mService->mProfileProc.Get() == app) {
            mService->mProfileProc = app;
            profileFile = mService->mProfileFile;
            profileFd = mService->mProfileFd;
            profileAutoStop = mService->mAutoStopProfiler;
        }
    }
    app->mHasShownUi = TRUE;
    app->mPendingUiClean = TRUE;
    if (profileFd != NULL) {
       // try {
        AutoPtr<IFileDescriptor> fd;
        profileFd->GetFileDescriptor((IFileDescriptor**)&fd);
        AutoPtr<IParcelFileDescriptorHelper> helper;
        CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);
        if (FAILED(helper->Dup(fd, (IParcelFileDescriptor**)&profileFd))) {
            profileFd = NULL;
        }
       // } catch (IOException e) {
       //     profileFd = NULL;
       // }
    }
    AutoPtr<IConfiguration> config;
    CConfiguration::New(mService->mConfiguration, (IConfiguration**)&config);
    ECode ec = app->mThread->ScheduleLaunchActivity(
        r->mIntent, IBinder::Probe(r->mAppToken),
        (Int32)r,
        r->mInfo, config, r->mCompat, r->mIcicle, results, newIntents, !andResume,
        mService->IsNextTransitionForward(), profileFile, profileFd, profileAutoStop);

    Int32 flags;
    app->mInfo->GetFlags(&flags);
    if ((flags & IApplicationInfo::FLAG_CANT_SAVE_STATE) != 0) {
        // This may be a heavy-weight process!  Note that the package
        // manager will ensure that only activity can run in the main
        // process of the .apk, which is the only thing that will be
        // considered heavy-weight.
        String cName;
        app->mInfo->GetPackageName(&cName);
        if (app->mProcessName.Equals(cName)) {

            if (mService->mHeavyWeightProcess != NULL
                    && mService->mHeavyWeightProcess.Get() != app) {
                Slogger::W(TAG, "Starting new heavy weight process %p when already running %p", app,
                        mService->mHeavyWeightProcess.Get());
            }
            mService->mHeavyWeightProcess = app;

            AutoPtr<IMessage> msg;
            mService->mHandler->ObtainMessage(
                CActivityManagerService::POST_HEAVY_NOTIFICATION_MSG,
                (IMessage**)&msg);
            msg->SetObj(r->Probe(EIID_IInterface));
            Boolean result;
            mService->mHandler->SendMessage(msg, &result);
        }
    }

#if defined(_DEBUG)
    if (SUCCEEDED(ec)) {
        AutoPtr<IComponentName> component;
        String packageName, className;

        r->mIntent->GetComponent((IComponentName**)&component);
        component->GetPackageName(&packageName);
        component->GetClassName(&className);
        Slogger::D(TAG, "Schedule launch %s/%s activity.", packageName.string(), className.string());
    }
#endif
//    } catch (RemoteException e) {
//        if (r.launchFailed) {
//            // This is the second time we failed -- finish activity
//            // and give up.
//            Slog.e(TAG, "Second failure launching "
//                  + r.intent.getComponent().flattenToShortString()
//                  + ", giving up", e);
//            mService.appDiedLocked(app, app.pid, app.thread);
//            requestFinishActivityLocked(r.appToken, Activity.RESULT_CANCELED, NULL,
//                    "2nd-crash", false);
//            return false;
//        }
//
//        // This is the first time we failed -- restart process and
//        // retry.
//        app.activities.remove(r);
//        throw e;
//    }

    r->mLaunchFailed = FALSE;
    if (UpdateLRUListLocked(r)) {
        String rStr;
        r->ToString(&rStr);
        Slogger::W(TAG, "Activity %s being launched, but already in LRU list", rStr.string());
    }

    if (andResume) {
        // As part of the process of launching, ActivityThread also performs
        // a resume.
        r->mState = ActivityState_RESUMED;
        if (DEBUG_STATES) Slogger::V(TAG, "Moving to RESUMED: %p (starting new instance)", r);

        r->mStopped = FALSE;
        mResumedActivity = r;
        r->mTask->TouchActiveTime();
        if (mMainStack) {
                mService->AddRecentTaskLocked(r->mTask);
        }
        CompleteResumeLocked(r);
        CheckReadyForSleepLocked();
        if (DEBUG_SAVED_STATE) Slogger::I(TAG, "Launch completed; removing icicle of %p", r->mIcicle.Get());
    }
    else {
        // This activity is not starting in the resumed state... which
        // should look like we asked it to pause+stop (but remain visible),
        // and it has done so and reported back the current icicle and
        // other state.
        if (DEBUG_STATES) Slogger::V(TAG, "Moving to STOPPED: %p (starting in stopped state)", r);

        r->mState = ActivityState_STOPPED;
        r->mStopped = TRUE;
    }

    // Launch the new version setup screen if needed.  We do this -after-
    // launching the initial activity (that is, home), so that it can have
    // a chance to initialize itself while in the background, making the
    // switch back to it faster and look better.
    if (mMainStack) {
        mService->StartSetupActivityLocked();
    }

    *succeeded = TRUE;
    return NOERROR;
}

ECode ActivityStack::StartSpecificActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Boolean andResume,
    /* [in] */ Boolean checkConfig)
{
    // Is this activity's application already running?
    AutoPtr<IApplicationInfo> appInfo;
    r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    AutoPtr<ProcessRecord> app = mService->GetProcessRecordLocked(r->mProcessName, uid);

    if (r->mLaunchTime == 0) {
        r->mLaunchTime = SystemClock::GetUptimeMillis();
        if (mInitialStartTime == 0) {
            mInitialStartTime = r->mLaunchTime;
        }
    }
    else if (mInitialStartTime == 0) {
        mInitialStartTime = SystemClock::GetUptimeMillis();
    }

    if (app != NULL && app->mThread != NULL) {
        String pkgName;
        r->mInfo->GetPackageName(&pkgName);
        app->AddPackage(pkgName);
        Boolean succeeded;
        if (SUCCEEDED(RealStartActivityLocked(r, app,
                andResume, checkConfig, &succeeded))) {
            return NOERROR;
        }
        else {
            String acDes;
            AutoPtr<IComponentName> component;
            r->mIntent->GetComponent((IComponentName**)&component);
            component->FlattenToShortString(&acDes);
            Slogger::W(TAG, "Exception when starting activity %s", acDes.string());
        }

        // If a dead object exception was thrown -- fall through to
        // restart the application.
    }

    AutoPtr<IComponentName> componentName;
    r->mIntent->GetComponent((IComponentName**)&componentName);
    mService->StartProcessLocked(r->mProcessName, appInfo,
            TRUE, 0, String("activity"), componentName, FALSE, FALSE);
    return NOERROR;
}

ECode ActivityStack::StopIfSleepingLocked()
{
    if (mService->IsSleeping()) {
        Boolean sleepHeld;
        mGoingToSleep->IsHeld(&sleepHeld);
        if (!sleepHeld) {
            mGoingToSleep->AcquireLock();
            Boolean launchHeld;
            mLaunchingActivity->IsHeld(&launchHeld);
            if (launchHeld) {
                mLaunchingActivity->ReleaseLock();

                mService->mHandler->RemoveMessages(LAUNCH_TIMEOUT_MSG);
            }
        }

        mHandler->RemoveMessages(SLEEP_TIMEOUT_MSG);
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(SLEEP_TIMEOUT_MSG, (IMessage**)&msg);
        Boolean result;
        mHandler->SendMessageDelayed(msg, SLEEP_TIMEOUT, &result);

        CheckReadyForSleepLocked();
    }
    return NOERROR;
}

ECode ActivityStack::AwakeFromSleepingLocked()
{
    mHandler->RemoveMessages(SLEEP_TIMEOUT_MSG);
    mSleepTimeout = FALSE;
    Boolean held;
    mGoingToSleep->IsHeld(&held);
    if (held) {
        mGoingToSleep->ReleaseLock();
    }
    // Ensure activities are no longer sleeping.
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        AutoPtr<ActivityRecord> r = *rit;
        r->SetSleeping(FALSE);
    }
    mGoingToSleepActivities.Clear();
    return NOERROR;
}

ECode ActivityStack::ActivitySleptLocked(
    /* [in] */ ActivityRecord* r)
{
    mGoingToSleepActivities.Remove(r);
    CheckReadyForSleepLocked();
    return NOERROR;
}

ECode ActivityStack::CheckReadyForSleepLocked()
{
    if (!mService->IsSleeping()) {
        // Do not care.
        return NOERROR;
    }

    if (!mSleepTimeout) {
        if (mResumedActivity != NULL) {
            // Still have something resumed; can't sleep until it is paused.
            if (DEBUG_PAUSE) Slogger::V(TAG, "Sleep needs to pause %p", mResumedActivity.Get());
            if (DEBUG_USER_LEAVING) Slogger::V(TAG, "Sleep => pause with userLeaving=false");
            StartPausingLocked(FALSE, TRUE);
            return NOERROR;
        }
        if (mPausingActivity != NULL) {
            // Still waiting for something to pause; can't sleep yet.
            if (DEBUG_PAUSE) Slogger::V(TAG, "Sleep still waiting to pause %p", mPausingActivity.Get());
            return NOERROR;
        }

        if (!mStoppingActivities.IsEmpty()) {
            // Still need to tell some activities to stop; can't sleep yet.
            if (DEBUG_PAUSE) Slogger::V(TAG, "Sleep still need to stop %d activities",
                    mStoppingActivities.GetSize());
            ScheduleIdleLocked();
            return NOERROR;
        }

        EnsureActivitiesVisibleLocked(NULL, 0);

        // Make sure any stopped but visible activities are now sleeping.
        // This ensures that the activity's onStop() is called.
        List< AutoPtr<ActivityRecord> >::ReverseIterator rit;
        for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
            AutoPtr<ActivityRecord> r = *rit;
            if (r->mState == ActivityState_STOPPING || r->mState == ActivityState_STOPPED) {
                r->SetSleeping(TRUE);
            }
        }

        if (!mGoingToSleepActivities.IsEmpty()) {
            // Still need to tell some activities to sleep; can't sleep yet.
            if (DEBUG_PAUSE) Slogger::V(TAG, "Sleep still need to sleep %d activities",
                    mGoingToSleepActivities.GetSize());
            return NOERROR;
        }
    }

    mHandler->RemoveMessages(SLEEP_TIMEOUT_MSG);

    Boolean held;
    if (mGoingToSleep->IsHeld(&held), held) {
        mGoingToSleep->ReleaseLock();
    }
    if (mService->mShuttingDown) {
        mService->mLock.NotifyAll();
    }
    return NOERROR;
}

AutoPtr<IBitmap> ActivityStack::ScreenshotActivities(
    /* [in] */ ActivityRecord* who)
{
    if (who->mNoDisplay) {
        return NULL;
    }

    AutoPtr<IResources> res;
    mService->mContext->GetResources((IResources**)&res);
    Int32 w = mThumbnailWidth;
    Int32 h = mThumbnailHeight;
    if (w < 0) {
        res->GetDimensionPixelSize(R::dimen::thumbnail_width, &mThumbnailWidth);
        w = mThumbnailWidth;
        res->GetDimensionPixelSize(R::dimen::thumbnail_height, &mThumbnailHeight);
        h = mThumbnailHeight;
    }

    if (w > 0) {
        AutoPtr<IBitmap> bitmap;
        mService->mWindowManager->ScreenshotApplications(IBinder::Probe(who->mAppToken),
                IDisplay::DEFAULT_DISPLAY, w, h, (IBitmap**)&bitmap);
        return bitmap;
    }
    return NULL;
}

ECode ActivityStack::StartPausingLocked(
    /* [in] */ Boolean userLeaving,
    /* [in] */ Boolean uiSleeping)
{
    if (mPausingActivity != NULL) {
        // String arDes;
        // mPausingActivity->GetDescription(&arDes);
        // Slogger::E(TAG, StringBuffer("Trying to pause when pause is already pending for ")
        //         + arDes);
    }
    AutoPtr<ActivityRecord> prev = mResumedActivity;
    if (prev == NULL) {
        Slogger::E(TAG, String("Trying to pause when nothing is resumed"));
        ResumeTopActivityLocked(NULL);
        return NOERROR;
    }
    // if (DEBUG_STATES) Slogger::V(TAG, StringBuffer("Moving to PAUSING: ") + prev);
    else if (DEBUG_PAUSE) {
        // String arDes;
        // prev->GetDescription(&arDes);
        // Slogger::V(TAG, StringBuffer("Start pausing: ") + arDes);
    }
    mResumedActivity = NULL;
    mPausingActivity = prev;
    mLastPausedActivity = prev;
    prev->mState = ActivityState_PAUSING;
    prev->mTask->TouchActiveTime();
    AutoPtr<IBitmap> thumbnailBitmap = ScreenshotActivities(prev);
    prev->UpdateThumbnail(thumbnailBitmap, NULL);

    mService->UpdateCpuStats();

    if (prev->mApp != NULL && prev->mApp->mThread != NULL) {
        if (DEBUG_PAUSE) {
            // String arDes;
            // prev->GetDescription(&arDes);
            // Slogger::V(TAG, StringBuffer("Enqueueing pending pause: ") + arDes);
        }
//        EventLog.writeEvent(IEventLogTags.AM_PAUSE_ACTIVITY,
//                prev.userId, System.identityHashCode(prev),
//                prev.shortComponentName);
        if (SUCCEEDED(prev->mApp->mThread->SchedulePauseActivity(IBinder::Probe(prev->mAppToken),
                prev->mFinishing, userLeaving, prev->mConfigChangeFlags))) {
            if (mMainStack) {
                mService->UpdateUsageStats(prev, FALSE);
            }
        }
        else {
            // Ignore exception, if process died other code will cleanup.
            Slogger::W(TAG, "Exception thrown during pause");
            mPausingActivity = NULL;
            mLastPausedActivity = NULL;
        }
    } else {
        mPausingActivity = NULL;
        mLastPausedActivity = NULL;
    }

    // If we are not going to sleep, we want to ensure the device is
    // awake until the next activity is started.
    if (!mService->mSleeping && !mService->mShuttingDown) {
        mLaunchingActivity->AcquireLock();
        Boolean hasMsg;
        mHandler->HasMessages(LAUNCH_TIMEOUT_MSG, &hasMsg);
        if (!hasMsg) {
            // To be safe, don't allow the wake lock to be held for too long.
            Boolean result;
            mHandler->SendEmptyMessageDelayed(
                LAUNCH_TIMEOUT_MSG, LAUNCH_TIMEOUT, &result);
        }
    }

    if (mPausingActivity != NULL) {
        // Have the window manager pause its key dispatching until the new
        // activity has started.  If we're pausing the activity just because
        // the screen is being turned off and the UI is sleeping, don't interrupt
        // key dispatch; the same activity will pick it up again on wakeup.
        if (!uiSleeping) {
            prev->PauseKeyDispatchingLocked();
        } else {
            if (DEBUG_PAUSE) {
                Slogger::V(TAG, "Key dispatch not paused for screen off");
            }
        }

        // Schedule a pause timeout in case the app doesn't respond.
        // We don't give it much time because this directly impacts the
        // responsiveness seen by the user.
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(PAUSE_TIMEOUT_MSG, (IMessage**)&msg);
        prev->mPauseTime = SystemClock::GetUptimeMillis();
        msg->SetObj(prev->Probe(EIID_IInterface));
        Boolean result;
        mHandler->SendMessageDelayed(msg, PAUSE_TIMEOUT, &result);
        if (DEBUG_PAUSE) Slogger::V(TAG, String("Waiting for pause to complete..."));
    }
    else {
        // This activity failed to schedule the
        // pause, so just treat it as being paused now.
        if (DEBUG_PAUSE) {
            Slogger::V(TAG, "Activity not running, resuming next.");
        }
        ResumeTopActivityLocked(NULL);
    }

    return NOERROR;
}

ECode ActivityStack::ActivityResumed(
    /* [in] */ IBinder* token)
{
    AutoPtr<ActivityRecord> r;

    AutoLock lock(mService->mLock);
    Int32 index = GetIndexOfTokenLocked(token);
    if (index >= 0) {
        r = mHistory[index];
        if (DEBUG_SAVED_STATE) Slogger::I(TAG, "Resumed activity; dropping state of: %p", r.Get());
        r->mIcicle = NULL;
        r->mHaveState = FALSE;
    }

    return NOERROR;
}

ECode ActivityStack::ActivityPaused(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean timeout)
{
    if (DEBUG_PAUSE) {
        // String tokenDes;
        // token->GetDescription(&tokenDes);
        // Slogger::V(TAG, StringBuffer("Activity paused: token=") + tokenDes + ", timeout=" + timeout);
    }

    AutoPtr<ActivityRecord> r;

    AutoLock lock(mService->mLock);
    Int32 index = GetIndexOfTokenLocked(token);
    if (index >= 0) {
        r = mHistory[index];
        mHandler->RemoveMessages(PAUSE_TIMEOUT_MSG, r->Probe(EIID_IInterface));

        if (mPausingActivity == r) {
            // if (DEBUG_STATES) Slogger::V(TAG, String("Moving to PAUSED: ") + r
            //                 + (timeout ? " (due to timeout)" : " (pause complete)"));
            r->mState = ActivityState_PAUSED;
            CompletePauseLocked();
        } else {
//            EventLog.writeEvent(IEventLogTags.AM_FAILED_TO_PAUSE,
//                    r.userId, System.identityHashCode(r), r.shortComponentName,
//                    mPausingActivity != NULL
//                        ? mPausingActivity.shortComponentName : "(none)");
        }
    }
    return NOERROR;
}

ECode ActivityStack::ActivityStoppedLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ IBundle* icicle,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ ICharSequence* description)
{
    assert(r != NULL);
    if (r->mState != ActivityState_STOPPING) {
        Slogger::I(TAG, "Activity reported stop, but no longer stopping: %p", r);
        mHandler->RemoveMessages(STOP_TIMEOUT_MSG, r->Probe(EIID_IInterface));
        return NOERROR;
    }
    if (DEBUG_SAVED_STATE) Slogger::I(TAG, "Saving icicle of %p : ", r, icicle);
    if (icicle != NULL) {
        // If icicle is NULL, this is happening due to a timeout, so we
        // haven't really saved the state.
        r->mIcicle = icicle;
        r->mHaveState = TRUE;
        r->UpdateThumbnail(thumbnail, description);
    }
    if (!r->mStopped) {
        if (DEBUG_STATES) Slogger::V(TAG, "Moving to STOPPED: %p (stop complete)", r);
        mHandler->RemoveMessages(STOP_TIMEOUT_MSG, r->Probe(EIID_IInterface));

        r->mStopped = TRUE;
        r->mState = ActivityState_STOPPED;
        if (r->mFinishing) {
            r->ClearOptionsLocked();
        } else {
            if (r->mConfigDestroy) {
                DestroyActivityLocked(r, TRUE, FALSE, String("stop-config"));
                ResumeTopActivityLocked(NULL);
            } else {
                // Now that this process has stopped, we may want to consider
                // it to be the previous app to try to keep around in case
                // the user wants to return to it.
                AutoPtr<ProcessRecord> fgApp;
                if (mResumedActivity != NULL) {
                    fgApp = mResumedActivity->mApp;
                } else if (mPausingActivity != NULL) {
                    fgApp = mPausingActivity->mApp;
                }
                if (r->mApp != NULL && fgApp != NULL && r->mApp != fgApp
                        && r->mLastVisibleTime > mService->mPreviousProcessVisibleTime
                        && r->mApp != mService->mHomeProcess) {
                    mService->mPreviousProcess = r->mApp;
                    mService->mPreviousProcessVisibleTime = r->mLastVisibleTime;
                }
            }
        }
    }
    return NOERROR;
}

ECode ActivityStack::CompletePauseLocked()
{
    AutoPtr<ActivityRecord> prev = mPausingActivity;
    if (DEBUG_PAUSE) {
        // String arDes;
        // prev->GetDescription(&arDes);
        // Slogger::V(TAG, StringBuffer("Complete pause: ") + arDes);
    }

    if (prev != NULL) {
        if (prev->mFinishing) {
            if (DEBUG_PAUSE) {
                // String arDes;
                // prev->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Executing finish of activity: ") + arDes);
            }
            AutoPtr<ActivityRecord> r;
            FinishCurrentActivityLocked(prev,
                    FINISH_AFTER_VISIBLE, FALSE, (ActivityRecord**)&r);
            prev = r;
        } else if (prev->mApp != NULL) {
            if (DEBUG_PAUSE) {
                // String arDes;
                // prev->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Enqueueing pending stop: ") + arDes);
            }
            if (prev->mWaitingVisible) {
                prev->mWaitingVisible = FALSE;
                mWaitingVisibleActivities.Remove(prev);
                if (DEBUG_SWITCH || DEBUG_PAUSE) {
                    // String arDes;
                    // prev->GetDescription(&arDes);
                    // Slogger::V(TAG, StringBuffer("Complete pause, no longer waiting: ")
                    //         + arDes);
                }
            }
            if (prev->mConfigDestroy) {
                // The previous is being paused because the configuration
                // is changing, which means it is actually stopping...
                // To juggle the fact that we are also starting a new
                // instance right now, we need to first completely stop
                // the current instance before starting the new one.
                if (DEBUG_PAUSE) {
                    // String arDes;
                    // prev->GetDescription(&arDes);
                    // Slogger::V(TAG, StringBuffer("Destroying after pause: ") + arDes);
                }
                DestroyActivityLocked(prev, TRUE, FALSE, String("pause-config"));
            } else {
                mStoppingActivities.PushBack(prev);
                if (mStoppingActivities.GetSize() > 3) {
                    // If we already have a few activities waiting to stop,
                    // then give up on things going idle and start clearing
                    // them out.
                    if (DEBUG_PAUSE) Slogger::V(TAG, String("To many pending stops, forcing idle"));
                    ScheduleIdleLocked();
                }
                else {
                    CheckReadyForSleepLocked();
                }
            }
        } else {
            if (DEBUG_PAUSE) {
                // String arDes;
                // prev->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("App died during pause, not stopping: ") + arDes);
            }
            prev = NULL;
        }
        mPausingActivity = NULL;
    }

    Boolean sleeped = mService->IsSleeping();
    if (!sleeped) {
        ResumeTopActivityLocked(prev);
    }
   else {
        CheckReadyForSleepLocked();
        AutoPtr<ActivityRecord> top = GetTopRunningActivityLocked(NULL);
        if (top == NULL || (prev != NULL && top != prev)) {
            // If there are no more activities available to run,
            // do resume anyway to start something.  Also if the top
            // activity on the stack is not the just paused activity,
            // we need to go ahead and resume it to ensure we complete
            // an in-flight app switch.
            ResumeTopActivityLocked(NULL);
        }
    }

    if (prev != NULL) {
        prev->ResumeKeyDispatchingLocked();
        prev->mCpuTimeAtResume = 0; // reset it
    }

    if (prev->mApp != NULL && prev->mCpuTimeAtResume > 0
            && mService->mBatteryStatsService->IsOnBattery()) {
        Int64 diff = 0;
        {
            AutoLock lock(mService->mProcessStatsThreadLock);
            Int64 pid;
            mService->mProcessStats->GetCpuTimeForPid(prev->mApp->mPid, &pid);
            diff = pid - prev->mCpuTimeAtResume;
        }
        if (diff > 0) {
            AutoPtr<BatteryStatsImpl> bsi = mService->mBatteryStatsService->GetActiveStatistics();
            bsi->Lock();
            AutoPtr<IApplicationInfo> appInfo;
            prev->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            String packageName;
            prev->mInfo->GetPackageName(&packageName);
            AutoPtr<BatteryStatsImpl::Uid::Proc> ps = bsi->GetProcessStatsLocked(uid, packageName);
            if (ps != NULL) {
                ps->AddForegroundTimeLocked(diff);
            }
            bsi->Unlock();
        }
    }
    return NOERROR;
}

ECode ActivityStack::CompleteResumeLocked(
    /* [in] */ ActivityRecord* next)
{
    next->mIdle = FALSE;
    next->mResults = NULL;
    next->mNewIntents = NULL;

    // schedule an idle timeout in case the app doesn't do it for us.
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(IDLE_TIMEOUT_MSG, (IMessage**)&msg);
    msg->SetObj(next->Probe(EIID_IInterface));
    Boolean result;
    mHandler->SendMessageDelayed(msg, IDLE_TIMEOUT, &result);

    if (FALSE) {
        // The activity was never told to pause, so just keep
        // things going as-is.  To maintain our own state,
        // we need to emulate it coming back and saying it is
        // idle.

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(IDLE_NOW_MSG, (IMessage**)&msg);
        msg->SetObj(next->Probe(EIID_IInterface));
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }

    if (mMainStack) {
        mService->ReportResumedActivityLocked(next);
    }

    if (mMainStack) {
        mService->SetFocusedActivityLocked(next);
    }
    next->ResumeKeyDispatchingLocked();
    EnsureActivitiesVisibleLocked(NULL, 0);
    mService->mWindowManager->ExecuteAppTransition();
    mNoAnimActivities.Clear();

    // Mark the point when the activity is resuming
    // TODO: To be more accurate, the mark should be before the onCreate,
    //       not after the onResume. But for subsequent starts, onResume is fine.
    if (next->mApp != NULL) {
        AutoLock lock(mService->mProcessStatsThreadLock);
        mService->mProcessStats->GetCpuTimeForPid(next->mApp->mPid, &next->mCpuTimeAtResume);
    }
    else {
        next->mCpuTimeAtResume = 0; // Couldn't get the cpu time of process
    }

    return NOERROR;
}

ECode ActivityStack::EnsureActivitiesVisibleLocked(
    /* [in] */ ActivityRecord* top,
    /* [in] */ ActivityRecord* starting,
    /* [in] */ const char* onlyThisProcess,
    /* [in] */ Int32 configChanges)
{
    if (DEBUG_VISBILITY) {
        // String arDes;
        // top->GetDescription(&arDes);
        // Slogger::V(TAG, StringBuffer("ensureActivitiesVisible behind ")
        //         + arDes + " configChanges=" + configChanges);
    }

    // If the top activity is not fullscreen, then we need to
    // make sure any activities under it are now visible.
    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while ((*rit).Get() != top) {
        ++rit;
    }
    ActivityRecord* r;
    Boolean behindFullscreen = FALSE;
    for (; rit != mHistory.REnd(); ++rit) {
        r = (ActivityRecord*)*rit;
        if (DEBUG_VISBILITY) {
            // String arDes;
            // r->GetDescription(&arDes);
            // Slogger::V(TAG, StringBuffer("Make visible? ") + arDes
            //         + " finishing=" + r->mFinishing + " state=" + r->mState);
        }
        if (r->mFinishing) {
            continue;
        }

        Boolean doThisProcess = onlyThisProcess == NULL
                || r->mProcessName.Equals(onlyThisProcess);

        // First: if this is not the current activity being started, make
        // sure it matches the current configuration.
        if (r != starting && doThisProcess) {
            EnsureActivityConfigurationLocked(r, 0);
        }

        if (r->mApp == NULL || r->mApp->mThread == NULL) {
            if (onlyThisProcess == NULL
                    || r->mProcessName.Equals(onlyThisProcess)) {
                // This activity needs to be visible, but isn't even
                // running...  get it started, but don't resume it
                // at this point.
                if (DEBUG_VISBILITY) {
                    // String arDes;
                    // r->GetDescription(&arDes);
                    // Slogger::V(TAG,
                    //         StringBuffer("Start and freeze screen for ") + arDes);
                }
                if (r != starting) {
                    r->StartFreezingScreenLocked(r->mApp, configChanges);
                }
                if (!r->mVisible) {
                    if (DEBUG_VISBILITY) {
                        // String arDes;
                        // r->GetDescription(&arDes);
                        // Slogger::V(TAG,
                        //         StringBuffer("Starting and making visible: ") + arDes);
                    }
                    mService->mWindowManager->SetAppVisibility(IBinder::Probe(r->mAppToken), TRUE);
                }
                if (r != starting) {
                    StartSpecificActivityLocked(r, FALSE, FALSE);
                }
            }
        }
        else if (r->mVisible) {
            // If this activity is already visible, then there is nothing
            // else to do here.
            if (DEBUG_VISBILITY) {
                // String arDes;
                // r->GetDescription(&arDes);
                // Slogger::V(TAG,
                //         StringBuffer("Skipping: already visible at ") + arDes);
            }
            r->StopFreezingScreenLocked(FALSE);
        }
        else if (onlyThisProcess == NULL) {
            // This activity is not currently visible, but is running.
            // Tell it to become visible.
            r->mVisible = TRUE;
            if (r->mState != ActivityState_RESUMED && r != starting) {
                // If this activity is paused, tell it
                // to now show its window.
                if (DEBUG_VISBILITY) {
                    // String arDes;
                    // r->GetDescription(&arDes);
                    // Slogger::V(TAG,
                    //         StringBuffer("Making visible and scheduling visibility: ")
                    //         + arDes);
                }
               // try {
                    mService->mWindowManager->SetAppVisibility(IBinder::Probe(r->mAppToken), TRUE);
                    r->mSleeping = FALSE;
                    r->mApp->mPendingUiClean = TRUE;
                    r->mApp->mThread->ScheduleWindowVisibility(IBinder::Probe(r->mAppToken), TRUE);
                    r->StopFreezingScreenLocked(FALSE);
               // } catch (Exception e) {
               //     // Just skip on any failure; we'll make it
               //     // visible when it next restarts.
               //     Slog.w(TAG, "Exception thrown making visibile: "
               //             + r.intent.getComponent(), e);
               // }
            }
        }

        // Aggregate current change flags.
        configChanges |= r->mConfigChangeFlags;

        if (r->mFullscreen) {
            // At this point, nothing else needs to be shown
            if (DEBUG_VISBILITY) {
                // String arDes;
                // r->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Stopping: fullscreen at ") + arDes);

            }
            behindFullscreen = TRUE;
            ++rit;
            break;
        }
    }

    // Now for any activities that aren't visible to the user, make
    // sure they no longer are keeping the screen frozen.
    while (rit != mHistory.REnd()) {
        r = (ActivityRecord*)*rit;
        if (DEBUG_VISBILITY) {
            // String arDes;
            // r->GetDescription(&arDes);
            // Slogger::V(TAG, StringBuffer("Make invisible? ") + arDes
            //     + " finishing=" + r->mFinishing
            //     + " state=" + r->mState
            //     + " behindFullscreen=" + behindFullscreen);

        }
        if (!r->mFinishing) {
            if (behindFullscreen) {
                if (r->mVisible) {
                    if (DEBUG_VISBILITY) {
                        // String arDes;
                        // r->GetDescription(&arDes);
                        // Slogger::V(TAG, StringBuffer("Making invisible: ") + arDes);
                    }
                    r->mVisible = FALSE;
                   // try {
                        mService->mWindowManager->SetAppVisibility(IBinder::Probe(r->mAppToken), FALSE);
                        if ((r->mState == ActivityState_STOPPING
                                || r->mState == ActivityState_STOPPED)
                                && r->mApp != NULL && r->mApp->mThread != NULL) {
                            if (DEBUG_VISBILITY) {
                                // String arDes;
                                // r->GetDescription(&arDes);
                                // Slogger::V(TAG,
                                //         StringBuffer("Scheduling invisibility: ") + arDes);
                            }
                            r->mApp->mThread->ScheduleWindowVisibility(IBinder::Probe(r->mAppToken), FALSE);
                        }
                   // } catch (Exception e) {
                   //     // Just skip on any failure; we'll make it
                   //     // visible when it next restarts.
                   //     Slog.w(TAG, "Exception thrown making hidden: "
                   //             + r.intent.getComponent(), e);
                   // }
                }
                else {
                    if (DEBUG_VISBILITY) {
                        // String arDes;
                        // r->GetDescription(&arDes);
                        // Slogger::V(TAG,
                        //         StringBuffer("Already invisible: ") + arDes);
                    }
                }
            }
            else if (r->mFullscreen) {
                if (DEBUG_VISBILITY) {
                    // String arDes;
                    // r->GetDescription(&arDes);
                    // Slogger::V(TAG,
                    //         StringBuffer("Now behindFullscreen: ") + arDes);
                }
                behindFullscreen = TRUE;
            }
        }
        ++rit;
    }
    return NOERROR;
}

ECode ActivityStack::EnsureActivitiesVisibleLocked(
    /* [in] */ ActivityRecord* starting,
    /* [in] */ Int32 configChanges)
{
    AutoPtr<ActivityRecord> r = GetTopRunningActivityLocked(NULL);
    if (r != NULL) {
        EnsureActivitiesVisibleLocked(r, starting, NULL, configChanges);
    }
    return NOERROR;
}

Boolean ActivityStack::ResumeTopActivityLocked(
    /* [in] */ ActivityRecord* prev)
{
    return ResumeTopActivityLocked(prev, NULL);
}

Boolean ActivityStack::ResumeTopActivityLocked(
    /* [in] */ ActivityRecord* prev,
    /* [in] */ IBundle* options)
{
    // Find the first activity that is not finishing.
    AutoPtr<ActivityRecord> next = GetTopRunningActivityLocked(NULL);

    // Remember how we'll process this pause/resume situation, and ensure
    // that the state is reset however we wind up proceeding.
    Boolean userLeaving = mUserLeaving;
    mUserLeaving = FALSE;

    if (next == NULL) {
        // There are no more activities!  Let's just start up the
        // Launcher...
        if (mMainStack) {
            AutoPtr<IActivityOptionsHelper> helper;
            CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
            helper->Abort(options);
            return mService->StartHomeActivityLocked(mCurrentUser);
        }
    }

    next->mDelayedResume = FALSE;

    // If the top activity is the resumed one, nothing to do.
    if (mResumedActivity == next && next->mState == ActivityState_RESUMED) {
        // Make sure we have executed any pending transitions, since there
        // should be nothing left to do at this point.
        mService->mWindowManager->ExecuteAppTransition();
        mNoAnimActivities.Clear();
        AutoPtr<IActivityOptionsHelper> helper;
        CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
        helper->Abort(options);
        return FALSE;
    }

    // If we are sleeping, and there is no resumed activity, and the top
    // activity is paused, well that is the state we want.
    if ((mService->mSleeping || mService->mShuttingDown)
            && mLastPausedActivity == next && (next->mState == ActivityState_PAUSED
            || next->mState == ActivityState_STOPPED
            || next->mState == ActivityState_STOPPING)) {
        // Make sure we have executed any pending transitions, since there
        // should be nothing left to do at this point.
        mService->mWindowManager->ExecuteAppTransition();
        mNoAnimActivities.Clear();
        AutoPtr<IActivityOptionsHelper> helper;
        CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
        helper->Abort(options);
        return FALSE;
    }

    // Make sure that the user who owns this activity is started.  If not,
    // we will just leave it as is because someone should be bringing
    // another user's activities to the top of the stack.
    if (mService->mStartedUsers[next->mUserId] == NULL) {
        Slogger::W(TAG, "Skipping resume of top activity %p: user %d is stopped", next.Get(),
                next->mUserId);
        return FALSE;
    }

    // The activity may be waiting for stop, but that is no longer
    // appropriate for it.
    mStoppingActivities.Remove(next);
    mGoingToSleepActivities.Remove(next);
    next->mSleeping = FALSE;
    mWaitingVisibleActivities.Remove(next);

    next->UpdateOptionsLocked(options);

    if (DEBUG_SWITCH || DEBUG_PAUSE) {
        String nextStr;
        next->ToString(&nextStr);
        Slogger::V(TAG, "Resuming %s", nextStr.string());
    }

    // If we are currently pausing an activity, then don't do anything
    // until that is done.
    if (mPausingActivity != NULL) {
        if (DEBUG_SWITCH) {
            String arDes;
            mPausingActivity->ToString(&arDes);
            Slogger::V(TAG, "Skip resume: pausing=%s", arDes.string());
        }
        return FALSE;
    }

    // Okay we are now going to start a switch, to 'next'.  We may first
    // have to pause the current activity, but this is an important point
    // where we have decided to go to 'next' so keep track of that.
    // XXX "App Redirected" dialog is getting too many false positives
    // at this point, so turn off for now.
    if (0) {
        if (mLastStartedActivity != NULL && !mLastStartedActivity->mFinishing) {
            Int32 now = SystemClock::GetUptimeMillis();
            const Boolean inTime = mLastStartedActivity->mStartTime != 0
                    && (mLastStartedActivity->mStartTime + START_WARN_TIME) >= now;
            AutoPtr<IApplicationInfo> appInfo;
            mLastStartedActivity->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            const Int32 lastUid = uid;
            appInfo = NULL;
            next->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            appInfo->GetUid(&uid);
            const Int32 nextUid = uid;
            Int32 permission;
            mService->CheckPermission(
                            Elastos::Droid::Manifest::Permission::STOP_APP_SWITCHES,
                            -1, next->mLaunchedFromUid, &permission);
            if (inTime && lastUid != nextUid
                    && lastUid != next->mLaunchedFromUid
                    && permission != IPackageManager::PERMISSION_GRANTED) {
                mService->ShowLaunchWarningLocked(mLastStartedActivity, next);
            }
            else {
                next->mStartTime = now;
                mLastStartedActivity = next;
            }
        }
        else {
            next->mStartTime = SystemClock::GetUptimeMillis();
            mLastStartedActivity = next;
        }
    }

    // We need to start pausing the current activity so the top one
    // can be resumed...
    if (mResumedActivity != NULL) {
        if (DEBUG_SWITCH) {
            Slogger::V(TAG, String("Skip resume: need to start pausing"));
        }
        // At this point we want to put the upcoming activity's process
        // at the top of the LRU list, since we know we will be needing it
        // very soon and it would be a waste to let it get killed if it
        // happens to be sitting towards the end.
        if (next->mApp != NULL && next->mApp->mThread != NULL) {
            // No reason to do full oom adj update here; we'll let that
            // happen whenever it needs to later.
            mService->UpdateLruProcessLocked(next->mApp, FALSE);
        }
        StartPausingLocked(userLeaving, FALSE);
        return TRUE;
    }

    // If the most recent activity was noHistory but was only stopped rather
    // than stopped+finished because the device went to sleep, we need to make
    // sure to finish it as we're making a new activity topmost.
    AutoPtr<ActivityRecord> last = mLastPausedActivity;
    if (mService->mSleeping && last != NULL && !last->mFinishing) {
        Int32 flags;
        last->mIntent->GetFlags(&flags);
        Int32 infoFlags;
        last->mInfo->GetFlags(&infoFlags);
        if ((flags&IIntent::FLAG_ACTIVITY_NO_HISTORY) != 0
                || (infoFlags&IActivityInfo::FLAG_NO_HISTORY) != 0) {
            if (DEBUG_STATES) {
                // Slogger::D(TAG, String("no-history finish of ") + last + " on new resume");
            }
            RequestFinishActivityLocked(IBinder::Probe(last->mAppToken), IActivity::RESULT_CANCELED, NULL,
                    String("no-history"), FALSE);
        }
    }

    if (prev != NULL && prev != next) {
        if (!prev->mWaitingVisible && next != NULL && !next->mNowVisible) {
            prev->mWaitingVisible = TRUE;
            mWaitingVisibleActivities.PushBack(prev);
            if (DEBUG_SWITCH) {
                // String arDes;
                // prev->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Resuming top, waiting visible to hide: ")
                //         + arDes);
            }
        }
        else {
            // The next activity is already visible, so hide the previous
            // activity's windows right now so we can show the new one ASAP.
            // We only do this if the previous is finishing, which should mean
            // it is on top of the one being resumed so hiding it quickly
            // is good.  Otherwise, we want to do the normal route of allowing
            // the resumed activity to be shown so we can decide if the
            // previous should actually be hidden depending on whether the
            // new one is found to be full-screen or not.
            if (prev->mFinishing) {
                mService->mWindowManager->SetAppVisibility(IBinder::Probe(prev->mAppToken), FALSE);
                if (DEBUG_SWITCH) {
                    // String arDes;
                    // prev->GetDescription(&arDes);
                    // Slogger::V(TAG, StringBuffer("Not waiting for visible to hide: ")
                    //         + arDes + ", waitingVisible=" + prev->mWaitingVisible
                    //         + ", nowVisible=" + next->mNowVisible);
                }
            }
            else {
                if (DEBUG_SWITCH) {
                    // String arDes;
                    // prev->GetDescription(&arDes);
                    // Slogger::V(TAG,
                    //     StringBuffer("Previous already visible but still waiting to hide: ")
                    //     + arDes + ", waitingVisible=" + prev->mWaitingVisible
                    //     + ", nowVisible=" + next->mNowVisible);
                }
            }
        }
    }

    // Launching this app's activity, make sure the app is no longer
    // considered stopped.
    // try {
    AutoPtr<IIPackageManager> pm = AppGlobals::GetPackageManager();
    pm->SetPackageStoppedState(next->mPackageName, FALSE, next->mUserId); /* TODO: Verify if correct userid */
    // } catch (RemoteException e1) {
    // } catch (IllegalArgumentException e) {
    //     Slog.w(TAG, "Failed trying to unstop package "
    //             + next.packageName + ": " + e);
    // }

    // We are starting up the next activity, so tell the window manager
    // that the previous one will be hidden soon.  This way it can know
    // to ignore it when computing the desired screen orientation.
    Boolean noAnim = FALSE;
    if (prev != NULL) {
        if (prev->mFinishing) {
            if (DEBUG_TRANSITION) {
                // String arDes;
                // prev->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Prepare close transition: prev=") + arDes);
            }
            if (Find(mNoAnimActivities.Begin(), mNoAnimActivities.End(),
                    AutoPtr<ActivityRecord>(prev)) != mNoAnimActivities.End()) {
                        mService->mWindowManager->PrepareAppTransition(IWindowManagerPolicy::TRANSIT_NONE, FALSE);
            }
            else {
                // if use WindowManagerPolicy::TRANSIT_TASK_CLOSE directly,
                // it has link errors in linux_android. I don't know the reason!
                Int32 taskClose = IWindowManagerPolicy::TRANSIT_TASK_CLOSE;
                mService->mWindowManager->PrepareAppTransition(prev->mTask == next->mTask
                    ? IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE
                    : taskClose, FALSE);
            }
            mService->mWindowManager->SetAppWillBeHidden(IBinder::Probe(prev->mAppToken));
            mService->mWindowManager->SetAppVisibility(IBinder::Probe(prev->mAppToken), FALSE);
        }
        else {
            if (DEBUG_TRANSITION) {
                // String arDes;
                // prev->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Prepare open transition: prev=") + arDes);
            }
            if (Find(mNoAnimActivities.Begin(), mNoAnimActivities.End(), next) !=
                    mNoAnimActivities.End()) {
                noAnim = TRUE;
                mService->mWindowManager->PrepareAppTransition(
                        IWindowManagerPolicy::TRANSIT_NONE, FALSE);
            }
            else {
                Int32 taskOpen = IWindowManagerPolicy::TRANSIT_TASK_OPEN;
                mService->mWindowManager->PrepareAppTransition(prev->mTask == next->mTask
                    ? IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN
                    : taskOpen, FALSE);
            }
        }
        if (FALSE) {
            mService->mWindowManager->SetAppWillBeHidden(IBinder::Probe(prev->mAppToken));
            mService->mWindowManager->SetAppVisibility(IBinder::Probe(prev->mAppToken), FALSE);
        }
    }
    else if (mHistory.GetSize() > 1) {
        if (DEBUG_TRANSITION) {
            Slogger::V(TAG, String("Prepare open transition: no previous"));
        }
        if (Find(mNoAnimActivities.Begin(), mNoAnimActivities.End(), next) !=
                mNoAnimActivities.End()) {
            noAnim = TRUE;
            mService->mWindowManager->PrepareAppTransition(
                    IWindowManagerPolicy::TRANSIT_NONE, FALSE);
        }
        else {
            mService->mWindowManager->PrepareAppTransition(
                IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN, FALSE);
        }
    }
    if (!noAnim) {
        next->ApplyOptionsLocked();
    }
    else {
        next->ClearOptionsLocked();
    }

    if (next->mApp != NULL && next->mApp->mThread != NULL) {
        if (DEBUG_SWITCH) {
            // String arDes;
            // next->GetDescription(&arDes);
            // Slogger::V(TAG, "Resume running: " + arDes);
        }

        // This activity is now becoming visible.
        mService->mWindowManager->SetAppVisibility(IBinder::Probe(next->mAppToken), TRUE);

        // schedule launch ticks to collect information about slow apps.
        next->StartLaunchTickingLocked();

        AutoPtr<ActivityRecord> lastResumedActivity = mResumedActivity;
        ActivityState lastState = next->mState;
        UNUSED(lastState);

        mService->UpdateCpuStats();

        // if (DEBUG_STATES) Slogger::V(TAG, StringBuffer("Moving to RESUMED: ") + next + " (in existing)");
        next->mState = ActivityState_RESUMED;
        mResumedActivity = next;
        next->mTask->TouchActiveTime();
        if (mMainStack) {
            mService->AddRecentTaskLocked(next->mTask);
        }
        mService->UpdateLruProcessLocked(next->mApp, TRUE);
        UpdateLRUListLocked(next);

        // Have the window manager re-evaluate the orientation of
        // the screen based on the new activity order.
        Boolean updated = FALSE;
        if (mMainStack) {
            AutoLock lock(mService->mLock);

            AutoPtr<IConfiguration> config;
            Boolean res;
            mService->mWindowManager->UpdateOrientationFromAppTokens(
                    mService->mConfiguration,
                    (next->MayFreezeScreenLocked(next->mApp, &res), res)
                        ? IBinder::Probe(next->mAppToken) : NULL,
                    (IConfiguration**)&config);
            if (config != NULL) {
                next->mFrozenBeforeDestroy = TRUE;
            }
            updated = mService->UpdateConfigurationLocked(config, next, FALSE, FALSE);
        }
        if (!updated) {
            // The configuration update wasn't able to keep the existing
            // instance of the activity, and instead started a new one.
            // We should be all done, but let's just make sure our activity
            // is still at the top and schedule another run if something
            // weird happened.
            AutoPtr<ActivityRecord> nextNext = GetTopRunningActivityLocked(NULL);
            if (DEBUG_SWITCH) {
                // String nDes, nnDes;
                // next->GetDescription(&nDes);
                // nextNext->GetDescription(&nnDes);
                // Slogger::I(TAG, StringBuffer("Activity config changed during resume: ")
                //         + nDes + ", new next: " + nnDes);
            }
            if (nextNext != next) {
                // Do over!
                Boolean result;
                mHandler->SendEmptyMessage(RESUME_TOP_ACTIVITY_MSG, &result);
            }
            if (mMainStack) {
                mService->SetFocusedActivityLocked(next);
            }
            EnsureActivitiesVisibleLocked(NULL, 0);
            mService->mWindowManager->ExecuteAppTransition();
            mNoAnimActivities.Clear();
            return TRUE;
        }

        // try {
        // Deliver all pending results.
        if (next->mResults != NULL) {
            List<AutoPtr<ActivityResult> >::Iterator it = next->mResults->Begin();
            if (!next->mFinishing && it != next->mResults->End()) {
                AutoPtr<IObjectContainer> res;
                CParcelableObjectContainer::New((IObjectContainer**)&res);
                for (; it != next->mResults->End(); ++it) {
                    res->Add((*it)->mResultInfo);
                }
                // if (DEBUG_RESULTS) Slogger::V(
                //         TAG, StringBuffer("Delivering results to ") + next
                //         + ": " + next->mResults);
                next->mApp->mThread->ScheduleSendResult(IBinder::Probe(next->mAppToken),
                        (IObjectContainer*)res);
            }
        }

        if (next->mNewIntents != NULL) {
            AutoPtr<IObjectContainer> res;
            CParcelableObjectContainer::New((IObjectContainer**)&res);
            List<AutoPtr<IIntent> >::Iterator it;
            for (it = next->mNewIntents->Begin(); it != next->mNewIntents->End(); ++it) {
                res->Add((*it)->Probe(EIID_IInterface));
            }
            next->mApp->mThread->ScheduleNewIntent(res, IBinder::Probe(next->mAppToken));
        }

       // EventLog.writeEvent(IEventLogTags.AM_RESUME_ACTIVITY,
       //         next.userId, System.identityHashCode(next),
       //         next.task.taskId, next.shortComponentName);

        next->mSleeping = FALSE;
        ShowAskCompatModeDialogLocked(next);
        next->mApp->mPendingUiClean = TRUE;
        next->mApp->mThread->ScheduleResumeActivity(IBinder::Probe(next->mAppToken),
                mService->IsNextTransitionForward());

        CheckReadyForSleepLocked();

        // } catch (Exception e) {
        //     // Whoops, need to restart this activity!
        //     if (DEBUG_STATES) Slog.v(TAG, "Resume failed; resetting state to "
        //             + lastState + ": " + next);
        //     next.state = lastState;
        //     mResumedActivity = lastResumedActivity;
        //     Slog.i(TAG, "Restarting because process died: " + next);
        //     if (!next.hasBeenLaunched) {
        //         next.hasBeenLaunched = true;
        //     } else {
        //         if (SHOW_APP_STARTING_PREVIEW && mMainStack) {
        //             mService.mWindowManager.setAppStartingWindow(
        //                     next.appToken, next.packageName, next.theme,
        //                     next.nonLocalizedLabel,
        //                     next.labelRes, next.icon, next.windowFlags,
        //                         NULL, true);
        //         }
        //     }
        //     startSpecificActivityLocked(next, true, false);
        //     return true;
        // }

        // From this point on, if something goes wrong there is no way
        // to recover the activity.
        // try {
        next->mVisible = TRUE;
        CompleteResumeLocked(next);
        // } catch (Exception e) {
        //     // If any exception gets thrown, toss away this
        //     // activity and try the next one.
        //     String arDes;
        //     next->GetDescription(&arDes);
        //     Slogger::W(TAG, StringBuffer("Exception thrown during resume of ") + arDes);
        //     RequestFinishActivityLocked(next.appToken, IActivity::RESULT_CANCELED, NULL,
        //             "resume-exception", true);
        //     return TRUE;
        // }
        next->mStopped = FALSE;
    }
    else {
        // Whoops, need to restart this activity!
        if (!next->mHasBeenLaunched) {
            next->mHasBeenLaunched = TRUE;
        }
        else {
            if (SHOW_APP_STARTING_PREVIEW) {
                AutoPtr<IApplicationInfo> appInfo;
                next->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                mService->mWindowManager->SetAppStartingWindow(
                        IBinder::Probe(next->mAppToken), next->mPackageName, next->mTheme,
                        mService->CompatibilityInfoForPackageLocked(appInfo),
                        next->mNonLocalizedLabel,
                        next->mLabelRes, next->mIcon, next->mWindowFlags,
                            NULL, TRUE);
            }
            if (DEBUG_SWITCH) {
                // String arDes;
                // next->GetDescription(&arDes);
                // Slogger::V(TAG, StringBuffer("Restarting: ") + arDes);
            }
        }
        StartSpecificActivityLocked(next, TRUE, TRUE);
    }

    return TRUE;
}

ECode ActivityStack::StartActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Boolean newTask,
    /* [in] */ Boolean doResume,
    /* [in] */ Boolean keepCurTransition,
    /* [in] */ IBundle* options)
{
    Int32 NH = mHistory.GetSize();

    Int32 addPos = -1;

    if (!newTask) {
        // If starting in an existing task, find where that is...
        Boolean startIt = TRUE;
        for (int i = NH - 1; i >= 0; i--) {
            ActivityRecord* p = mHistory[i];
            if (p->mFinishing) {
                continue;
            }
            if (p->mTask == r->mTask) {
                // Here it is!  Now, if this is not yet visible to the
                // user, then just add it without starting; it will
                // get started when the user navigates back to it.
                addPos = i + 1;
                if (!startIt) {
                    if (DEBUG_ADD_REMOVE) {
                       // RuntimeException here = new RuntimeException("here");
                       // here.fillInStackTrace();
                       // Slogger::I(TAG, "Adding activity " + r + " to stack at " + addPos,
                       //         here);
                    }
                    mHistory.Insert(addPos, r);
                    r->PutInHistory();
                    Int32 flags;
                    r->mInfo->GetFlags(&flags);
                    Int32 screenOrientation;
                    r->mInfo->GetScreenOrientation(&screenOrientation);
                    mService->mWindowManager->AddAppToken(
                        addPos, r->mUserId, r->mAppToken, r->mTask->mTaskId,
                        screenOrientation, r->mFullscreen,
                        (flags & IActivityInfo::FLAG_SHOW_ON_LOCK_SCREEN) != 0);
                    if (VALIDATE_TOKENS) {
                        ValidateAppTokensLocked();
                    }
                    AutoPtr<IActivityOptionsHelper> helper;
                    CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
                    helper->Abort(options);
                    return NOERROR;
                }
                break;
            }
            if (p->mFullscreen) {
                startIt = FALSE;
            }
        }
    }

    // Place a new activity at top of stack, so it is next to interact
    // with the user.
    if (addPos < 0) {
        addPos = NH;
    }

    // If we are not placing the new activity frontmost, we do not want
    // to deliver the onUserLeaving callback to the actual frontmost
    // activity
    if (addPos < NH) {
        mUserLeaving = FALSE;
        if (DEBUG_USER_LEAVING) {
            Slogger::V(TAG, String("startActivity() behind front, mUserLeaving=false"));
        }
    }

    // Slot the activity into the history stack and proceed
    if (DEBUG_ADD_REMOVE) {
       // RuntimeException here = new RuntimeException("here");
       // here.fillInStackTrace();
       // Slogger::I(TAG, "Adding activity " + r + " to stack at " + addPos, here);
    }
    mHistory.Insert(addPos, r);
    r->PutInHistory();
    r->mFrontOfTask = newTask;
    if (NH > 0) {
        // We want to show the starting preview window if we are
        // switching to a new task, or the next activity's process is
        // not currently running.
        Boolean showStartingIcon = newTask;
        AutoPtr<ProcessRecord> proc = r->mApp;
        if (proc == NULL) {
            AutoPtr<IApplicationInfo> appInfo;
            r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            proc = mService->mProcessNames->Get(r->mProcessName, uid);
        }
        if (proc == NULL || proc->mThread == NULL) {
            showStartingIcon = TRUE;
        }
        if (DEBUG_TRANSITION) {
            // String arDes;
            // r->GetDescription(&arDes);
            // Slogger::V(TAG, StringBuffer("Prepare open transition: starting ") + arDes);
        }
        Int32 flags;
        r->mIntent->GetFlags(&flags);
        if ((flags & IIntent::FLAG_ACTIVITY_NO_ANIMATION) != 0) {
            mService->mWindowManager->PrepareAppTransition(IWindowManagerPolicy::TRANSIT_NONE,
                    keepCurTransition);
            mNoAnimActivities.PushBack(r);
        }
        else {
            Int32 activityOpen = IWindowManagerPolicy::TRANSIT_ACTIVITY_OPEN;
            mService->mWindowManager->PrepareAppTransition(newTask
                ? IWindowManagerPolicy::TRANSIT_TASK_OPEN
                : activityOpen, keepCurTransition);
            mNoAnimActivities.Remove(r);
        }
        r->UpdateOptionsLocked(options);
        Int32 orientation;
        r->mInfo->GetScreenOrientation(&orientation);
        Int32 rFlags;
        r->mInfo->GetFlags(&rFlags);

        mService->mWindowManager->AddAppToken(
                addPos, r->mUserId, r->mAppToken, r->mTask->mTaskId, orientation, r->mFullscreen,
                (rFlags & IActivityInfo::FLAG_SHOW_ON_LOCK_SCREEN) != 0);
        Boolean doShow = TRUE;
        if (newTask) {
            // Even though this activity is starting fresh, we still need
            // to reset it to make sure we apply affinities to move any
            // existing activities from other tasks in to it.
            // If the caller has requested that the target task be
            // reset, then do so.
            if ((flags & IIntent::FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) != 0) {
                ResetTaskIfNeededLocked(r, r);
                doShow = GetTopRunningNonDelayedActivityLocked(NULL).Get() == r;
            }
        }
        if (SHOW_APP_STARTING_PREVIEW && doShow) {
            // Figure out if we are transitioning from another activity that is
            // "has the same starting icon" as the next one.  This allows the
            // window manager to keep the previous window it had previously
            // created, if it still had one.
            AutoPtr<ActivityRecord> prev = mResumedActivity;
            if (prev != NULL) {
                // We don't want to reuse the previous starting preview if:
                // (1) The current activity is in a different task.
                if (prev->mTask != r->mTask) prev = NULL;
                // (2) The current activity is already displayed.
                else if (prev->mNowVisible) prev = NULL;
            }
            AutoPtr<IApplicationInfo> appInfo;
            r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            mService->mWindowManager->SetAppStartingWindow(
                    IBinder::Probe(r->mAppToken), r->mPackageName, r->mTheme,
                    mService->CompatibilityInfoForPackageLocked(appInfo), r->mNonLocalizedLabel,
                    r->mLabelRes, r->mIcon, r->mWindowFlags,
                    prev != NULL ? IBinder::Probe(prev->mAppToken) : NULL,
                    showStartingIcon);
        }
    }
    else {
        Int32 orientation;
        r->mInfo->GetScreenOrientation(&orientation);
        // If this is the first activity, don't do any fancy animations,
        // because there is nothing for it to animate on top of.
        Int32 flags;
        r->mInfo->GetFlags(&flags);
        mService->mWindowManager->AddAppToken(
                addPos, r->mUserId, r->mAppToken, r->mTask->mTaskId, orientation, r->mFullscreen,
                (flags & IActivityInfo::FLAG_SHOW_ON_LOCK_SCREEN) != 0);
        AutoPtr<IActivityOptionsHelper> helper;
        CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
        helper->Abort(options);
    }
    if (VALIDATE_TOKENS) {
        ValidateAppTokensLocked();
    }

    if (doResume) {
        ResumeTopActivityLocked(NULL);
    }

    return NOERROR;
}

ECode ActivityStack::ValidateAppTokensLocked()
{
    mValidateAppTokens.Clear();
// TODO:
//     mValidateAppTokens.ensureCapacity(mHistory.size());
    List< AutoPtr<ActivityRecord> >::Iterator it;
    for (it = mHistory.Begin(); it != mHistory.End(); ++it) {
        mValidateAppTokens.PushBack(IBinder::Probe((*it)->mAppToken));
    }
    mService->mWindowManager->ValidateAppTokens(mValidateAppTokens);
    return NOERROR;
}

AutoPtr<ActivityRecord> ActivityStack::ResetTaskIfNeededLocked(
    /* [in] */ ActivityRecord* taskTop,
    /* [in] */ ActivityRecord* newActivity)
{
    Int32 newFlags;
    newActivity->mInfo->GetFlags(&newFlags);
    Boolean forceReset = (newFlags & IActivityInfo::FLAG_CLEAR_TASK_ON_LAUNCH) != 0;
    if (ACTIVITY_INACTIVE_RESET_TIME > 0
                && taskTop->mTask->GetInactiveDuration() > ACTIVITY_INACTIVE_RESET_TIME) {
        if ((newFlags & IActivityInfo::FLAG_ALWAYS_RETAIN_TASK_STATE) == 0) {
            forceReset = TRUE;
        }
    }

    AutoPtr<TaskRecord> task = taskTop->mTask;

    // We are going to move through the history list so that we can look
    // at each activity 'target' with 'below' either the interesting
    // activity immediately below it in the stack or NULL.
    AutoPtr<ActivityRecord> target;
    Int32 targetI = 0;
    Int32 taskTopI = -1;
    Int32 replyChainEnd = -1;
    Int32 lastReparentPos = -1;
    AutoPtr<IActivityOptions> topOptions = NULL;
    Boolean canMoveOptions = TRUE;
    for (Int32 i = mHistory.GetSize() - 1; i >= -1; i--) {
        AutoPtr<ActivityRecord> below = i >= 0 ? mHistory[i] : NULL;

        if (below != NULL && below->mFinishing) {
            continue;
        }
        // Don't check any lower in the stack if we're crossing a user boundary.
        if (below != NULL && below->mUserId != taskTop->mUserId) {
            break;
        }
        if (target == NULL) {
            target = below;
            targetI = i;
            // If we were in the middle of a reply chain before this
            // task, it doesn't appear like the root of the chain wants
            // anything interesting, so drop it.
            replyChainEnd = -1;
            continue;
        }

        Int32 flags;
        target->mInfo->GetFlags(&flags);

        Boolean finishOnTaskLaunch =
            (flags & IActivityInfo::FLAG_FINISH_ON_TASK_LAUNCH) != 0;
        Boolean allowTaskReparenting =
            (flags & IActivityInfo::FLAG_ALLOW_TASK_REPARENTING) != 0;

        if (target->mTask == task) {
            // We are inside of the task being reset...  we'll either
            // finish this activity, push it out for another task,
            // or leave it as-is.  We only do this
            // for activities that are not the root of the task (since
            // if we finish the root, we may no longer have the task!).
            if (taskTopI < 0) {
                taskTopI = targetI;
            }
            if (below != NULL && below->mTask == task) {
                Int32 iflags;
                target->mIntent->GetFlags(&iflags);
                Boolean clearWhenTaskReset =
                        (iflags & IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0;
                if (!finishOnTaskLaunch && !clearWhenTaskReset && target->mResultTo != NULL) {
                    // If this activity is sending a reply to a previous
                    // activity, we can't do anything with it now until
                    // we reach the start of the reply chain.
                    // XXX note that we are assuming the result is always
                    // to the previous activity, which is almost always
                    // the case but we really shouldn't count on.
                    if (replyChainEnd < 0) {
                        replyChainEnd = targetI;
                    }
                }
                else if (!finishOnTaskLaunch && !clearWhenTaskReset && allowTaskReparenting
                        && !target->mTaskAffinity.IsNull()
                        && target->mTaskAffinity.Compare(task->mAffinity)) {
                    // If this activity has an affinity for another
                    // task, then we need to move it out of here.  We will
                    // move it as far out of the way as possible, to the
                    // bottom of the activity stack.  This also keeps it
                    // correctly ordered with any activities we previously
                    // moved.
                    AutoPtr<ActivityRecord> p = mHistory[0];
                    if (!target->mTaskAffinity.IsNull()
                            && !target->mTaskAffinity.Compare(p->mTask->mAffinity)) {
                        // If the activity currently at the bottom has the
                        // same task affinity as the one we are moving,
                        // then merge it into the same task.
                        target->SetTask(p->mTask, p->mThumbHolder, FALSE);
                        if (DEBUG_TASKS) {
                            // String arDes, trDes;
                            // target->GetDescription(&arDes);
                            // p->mTask->GetDescription(&trDes);
                            // Slogger::V(TAG, StringBuffer("Start pushing activity ") + arDes
                            //     + " out to bottom task " + trDes);
                        }
                    }
                    else {
                        mService->mCurTask++;
                        if (mService->mCurTask <= 0) {
                            mService->mCurTask = 1;
                        }
                        AutoPtr<TaskRecord> task = new TaskRecord(mService->mCurTask, target->mInfo, NULL);
                        target->SetTask(task, NULL, FALSE);
                        target->mTask->mAffinityIntent = target->mIntent;
                        if (DEBUG_TASKS) {
                            // String arDes, trDes;
                            // target->GetDescription(&arDes);
                            // target->mTask->GetDescription(&trDes);
                            // Slogger::V(TAG, StringBuffer("Start pushing activity ") + arDes
                            //     + " out to new task " + trDes);
                        }
                    }

                    mService->mWindowManager->SetAppGroupId(
                        IBinder::Probe(target->mAppToken), task->mTaskId);
                    if (replyChainEnd < 0) {
                        replyChainEnd = targetI;
                    }
                    Int32 dstPos = 0;
                    AutoPtr<ThumbnailHolder> curThumbHolder = target->mThumbHolder;
                    Boolean gotOptions = !canMoveOptions;
                    for (Int32 srcPos = targetI; srcPos <= replyChainEnd; srcPos++) {
                        p = mHistory[srcPos];
                        if (p->mFinishing) {
                            continue;
                        }
                        if (DEBUG_TASKS) {
                            // String arDes, trDes;
                            // p->GetDescription(&arDes);
                            // target->mTask->GetDescription(&trDes);
                            // Slogger::V(TAG, StringBuffer("Pushing next activity ") + arDes
                            //     + " out to target's task " + trDes);
                        }
                        p->SetTask(target->mTask, curThumbHolder, FALSE);
                        curThumbHolder = p->mThumbHolder;
                        canMoveOptions = FALSE;
                        if (!gotOptions && topOptions == NULL) {
                            topOptions = p->TakeOptionsLocked();
                            if (topOptions != NULL) {
                                gotOptions = TRUE;
                            }
                        }
                        if (DEBUG_ADD_REMOVE) {
                           // RuntimeException here = new RuntimeException("here");
                           // here.fillInStackTrace();
                           // Slogger::I(TAG, "Removing and adding activity " + p + " to stack at "
                           //         + dstPos, here);
                        }
                        List<AutoPtr<ActivityRecord> >::Iterator it1 = mHistory.Begin();
                        for (Int32 pos = srcPos; pos > 0; --pos) ++it1;
                        mHistory.Erase(it1);
                        it1 = mHistory.Begin();
                        for (Int32 pos = dstPos; pos > 0; --pos) ++it1;
                        mHistory.Insert(it1, p);

                        mService->mWindowManager->MoveAppToken(
                            dstPos, IBinder::Probe(p->mAppToken));
                        mService->mWindowManager->SetAppGroupId(
                            IBinder::Probe(p->mAppToken), p->mTask->mTaskId);
                        dstPos++;
                        if (VALIDATE_TOKENS) {
                            ValidateAppTokensLocked();
                        }
                        i++;
                    }
                    if (taskTop == p) {
                        taskTop = below;
                    }
                    if (taskTopI == replyChainEnd) {
                        taskTopI = -1;
                    }
                    replyChainEnd = -1;
                } else if (forceReset || finishOnTaskLaunch
                        || clearWhenTaskReset) {
                    // If the activity should just be removed -- either
                    // because it asks for it, or the task should be
                    // cleared -- then finish it and anything that is
                    // part of its reply chain.
                    if (clearWhenTaskReset) {
                        // In this case, we want to finish this activity
                        // and everything above it, so be sneaky and pretend
                        // like these are all in the reply chain.
                        replyChainEnd = targetI + 1;
                        while (replyChainEnd < (Int32)mHistory.GetSize() &&
                                (mHistory[replyChainEnd])->mTask == task) {
                            replyChainEnd++;
                        }
                        replyChainEnd--;
                    } else if (replyChainEnd < 0) {
                        replyChainEnd = targetI;
                    }
                    AutoPtr<ActivityRecord> p;
                    Boolean gotOptions = !canMoveOptions;
                    for (Int32 srcPos = targetI; srcPos <= replyChainEnd; srcPos++) {
                        p = mHistory[srcPos];
                        if (p->mFinishing) {
                            continue;
                        }
                        canMoveOptions = FALSE;
                        if (!gotOptions && topOptions == NULL) {
                            topOptions = p->TakeOptionsLocked();
                            if (topOptions != NULL) {
                                gotOptions = TRUE;
                            }
                        }
                        if (FinishActivityLocked(p, srcPos,
                                IActivity::RESULT_CANCELED, NULL, String("reset"), FALSE)) {
                            replyChainEnd--;
                            srcPos--;
                        }
                    }
                    if (taskTop == p) {
                        taskTop = below;
                    }
                    if (taskTopI == replyChainEnd) {
                        taskTopI = -1;
                    }
                    replyChainEnd = -1;
                } else {
                    // If we were in the middle of a chain, well the
                    // activity that started it all doesn't want anything
                    // special, so leave it all as-is.
                    replyChainEnd = -1;
                }
            } else {
                // Reached the bottom of the task -- any reply chain
                // should be left as-is.
                replyChainEnd = -1;
            }

        } else if (target->mResultTo != NULL && (below == NULL
                    || below->mTask == target->mTask)) {
            // If this activity is sending a reply to a previous
            // activity, we can't do anything with it now until
            // we reach the start of the reply chain.
            // XXX note that we are assuming the result is always
            // to the previous activity, which is almost always
            // the case but we really shouldn't count on.
            if (replyChainEnd < 0) {
                replyChainEnd = targetI;
            }

        } else if (taskTopI >= 0 && allowTaskReparenting
                && !task->mAffinity.IsNull()
                && !task->mAffinity.Compare(target->mTaskAffinity)) {
            // We are inside of another task...  if this activity has
            // an affinity for our task, then either remove it if we are
            // clearing or move it over to our task.  Note that
            // we currently punt on the case where we are resetting a
            // task that is not at the top but who has activities above
            // with an affinity to it...  this is really not a normal
            // case, and we will need to later pull that task to the front
            // and usually at that point we will do the reset and pick
            // up those remaining activities.  (This only happens if
            // someone starts an activity in a new task from an activity
            // in a task that is not currently on top.)
            if (forceReset || finishOnTaskLaunch) {
                if (replyChainEnd < 0) {
                    replyChainEnd = targetI;
                }
                AutoPtr<ActivityRecord> p;
                if (DEBUG_TASKS) Slogger::V(TAG, "Finishing task at index %d to %d",
                        targetI, replyChainEnd);
                for (Int32 srcPos = targetI; srcPos <= replyChainEnd; srcPos++) {
                    p = mHistory[srcPos];
                    if (p->mFinishing) {
                        continue;
                    }
                    if (FinishActivityLocked(p, srcPos,
                            IActivity::RESULT_CANCELED, NULL, String("reset"), FALSE)) {
                        taskTopI--;
                        lastReparentPos--;
                        replyChainEnd--;
                        srcPos--;
                    }
                }
                replyChainEnd = -1;
            } else {
                if (replyChainEnd < 0) {
                    replyChainEnd = targetI;
                }
                if (DEBUG_TASKS) Slogger::V(TAG, "Reparenting task at index %d to %d",
                        targetI, replyChainEnd);
                for (Int32 srcPos = replyChainEnd; srcPos >= targetI; srcPos--) {
                    AutoPtr<ActivityRecord> p = mHistory[srcPos];
                    if (p->mFinishing) {
                        continue;
                    }
                    if (lastReparentPos < 0) {
                        lastReparentPos = taskTopI;
                        taskTop = p;
                    } else {
                        lastReparentPos--;
                    }
                    if (DEBUG_ADD_REMOVE) {
                       // RuntimeException here = new RuntimeException("here");
                       // here.fillInStackTrace();
                       // Slogger::I(TAG, "Removing and adding activity " + p + " to stack at "
                       //         + lastReparentPos, here);
                    }
                    List<AutoPtr<ActivityRecord> >::Iterator it1 = mHistory.Begin();
                    for (Int32 pos = srcPos; pos > 0; --pos) ++it1;
                    mHistory.Erase(it1);
                    p->SetTask(task, NULL, FALSE);
                    it1 = mHistory.Begin();
                    for (Int32 pos = lastReparentPos; pos > 0; --pos) ++it1;
                    mHistory.Insert(it1, p);
                    if (DEBUG_TASKS) {
                        // String arDes, trDes;
                        // p->GetDescription(&arDes);
                        // task->GetDescription(&trDes);
                        // Slogger::V(TAG, StringBuffer("Pulling activity ") + arDes
                        //     + " from " + srcPos + " to " + lastReparentPos
                        //     + " in to resetting task " + trDes);
                    }

                    mService->mWindowManager->MoveAppToken(
                        lastReparentPos, IBinder::Probe(p->mAppToken));
                    mService->mWindowManager->SetAppGroupId(
                        IBinder::Probe(p->mAppToken), p->mTask->mTaskId);
                    if (VALIDATE_TOKENS) {
                        ValidateAppTokensLocked();
                    }
                }
                replyChainEnd = -1;

                // Now we've moved it in to place...  but what if this is
                // a singleTop activity and we have put it on top of another
                // instance of the same activity?  Then we drop the instance
                // below so it remains singleTop.
                Int32 mode;
                target->mInfo->GetLaunchMode(&mode);
                if (mode == IActivityInfo::LAUNCH_SINGLE_TOP) {
                    for (Int32 j = lastReparentPos - 1; j >= 0; j--) {
                        AutoPtr<ActivityRecord> p = mHistory[j];
                        if (p->mFinishing) {
                            continue;
                        }
                        AutoPtr<IComponentName> component;
                        p->mIntent->GetComponent((IComponentName**)&component);
                        AutoPtr<IComponentName> targetComponent;
                        target->mIntent->GetComponent((IComponentName**)&targetComponent);
                        Boolean isEqual;
                        if (component->Equals(targetComponent, &isEqual), isEqual) {
                            if (FinishActivityLocked(p, j,
                                    IActivity::RESULT_CANCELED, NULL, String("replace"), FALSE)) {
                                taskTopI--;
                                lastReparentPos--;
                            }
                        }
                    }
                }
            }
        } else if (below != NULL && below->mTask != target->mTask) {
            // We hit the botton of a task; the reply chain can't
            // pass through it.
            replyChainEnd = -1;
        }

        target = below;
        targetI = i;
    }

    if (topOptions != NULL) {
        // If we got some ActivityOptions from an activity on top that
        // was removed from the task, propagate them to the new real top.
        if (taskTop != NULL) {
            taskTop->UpdateOptionsLocked(topOptions);
        } else {
            topOptions->Abort();
        }
    }

    return taskTop;
}

AutoPtr<ActivityRecord> ActivityStack::PerformClearTaskLocked(
    /* [in] */ Int32 taskId,
    /* [in] */ ActivityRecord* newR,
    /* [in] */ Int32 launchFlags)
{
    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    Int32 i = mHistory.GetSize();

    // First find the requested task.
    while (rit != mHistory.REnd() && i > 0) {
        AutoPtr<ActivityRecord> r = *rit;
        if (r->mTask->mTaskId == taskId) {
            break;
        }
        rit++;
        i--;
    }

    // Now clear it.
    while (rit != mHistory.REnd() && i > 0) {
        i--;
        AutoPtr<ActivityRecord> r = *rit;
        if (r->mFinishing) {
            continue;
        }
        if (r->mTask->mTaskId != taskId) {
            return NULL;
        }
        Boolean isEqual;
        if (r->mRealActivity->Equals(newR->mRealActivity, &isEqual), isEqual) {
            // Here it is!  Now finish everything in front...
            AutoPtr<ActivityRecord> ret = r;
            while (i < (Int32)(mHistory.GetSize() - 1)) {
                rit--;
                i++;
                r = *rit;
                if (r->mTask->mTaskId != taskId) {
                    break;
                }
                if (r->mFinishing) {
                    continue;
                }
                AutoPtr<IActivityOptions> opts = r->TakeOptionsLocked();
                if (opts != NULL) {
                    ret->UpdateOptionsLocked(opts);
                }
                if (FinishActivityLocked(r, i, IActivity::RESULT_CANCELED,
                        NULL, String("clear"), FALSE)) {
                    rit++;
                    i--;
                }
            }

            // Finally, if this is a normal launch mode (that is, not
            // expecting onNewIntent()), then we will finish the current
            // instance of the activity so a new fresh one can be started.
            if (ret->mLaunchMode == IActivityInfo::LAUNCH_MULTIPLE
                        && (launchFlags & IIntent::FLAG_ACTIVITY_SINGLE_TOP) == 0) {
                if (!ret->mFinishing) {
                    Int32 index = GetIndexOfTokenLocked(IBinder::Probe(ret->mAppToken));
                    if (index >= 0) {
                        FinishActivityLocked(ret, index, IActivity::RESULT_CANCELED,
                                NULL, String("clear"), FALSE);
                    }
                    return NULL;
                }
            }

            return ret;
        }
        rit++;
    }

    return NULL;
}

ECode ActivityStack::PerformClearTaskAtIndexLocked(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 i)
{
    Slogger::D(TAG, "ActivityStack::PerformClearTaskAtIndexLocked %d, index %d", taskId, i);
    while (i < mHistory.GetSize()) {
        AutoPtr<ActivityRecord> r = mHistory[i];
        if (r->mTask->mTaskId != taskId) {
            // Whoops hit the end.
            return NOERROR;
        }
        if (r->mFinishing) {
            i++;
            continue;
        }
        if (!FinishActivityLocked(r, i, IActivity::RESULT_CANCELED, NULL, String("clear"), FALSE)) {
            i++;
        }
    }
    return NOERROR;
}

ECode ActivityStack::PerformClearTaskLocked(
    /* [in] */ Int32 taskId)
{
    Int32 i = mHistory.GetSize();

    // First find the requested task.
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        i--;
        AutoPtr<ActivityRecord> r = *rit;
        if (r->mTask->mTaskId == taskId) {
            i++;
            break;
        }
        ++rit;
    }

    // Now find the start and clear it.
    while (rit != mHistory.REnd()) {
        i--;
        AutoPtr<ActivityRecord> r = *rit;
        if (r->mFinishing) {
            continue;
        }
        if (r->mTask->mTaskId != taskId) {
            // We hit the bottom.  Now finish it all...
            PerformClearTaskAtIndexLocked(taskId, i + 1);
            return NOERROR;
        }
        ++rit;
    }
    return NOERROR;
}

Int32 ActivityStack::FindActivityInHistoryLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 task)
{
    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    Int32 i = mHistory.GetSize() - 1;
    while (rit != mHistory.REnd() && i >= 0) {
        AutoPtr<ActivityRecord> candidate = *rit;
        if (candidate->mFinishing) {
            continue;
        }
        if (candidate->mTask->mTaskId != task) {
            break;
        }
        Boolean isEqual;
        if (candidate->mRealActivity->Equals(r->mRealActivity, &isEqual), isEqual) {
            return i;
        }
        rit++;
        i--;
    }

    return -1;
}

AutoPtr<ActivityRecord> ActivityStack::MoveActivityToFrontLocked(
    /* [in] */ Int32 where)
{
    List<AutoPtr<ActivityRecord> >::Iterator next = mHistory.Begin();
    for (; where > 0; --where) ++next;
    AutoPtr<ActivityRecord> newTop = *next;
    mHistory.Erase(next);
    AutoPtr<ActivityRecord> oldTop = mHistory.GetBack();
    if (DEBUG_ADD_REMOVE) {
       // RuntimeException here = new RuntimeException("here");
       // here.fillInStackTrace();
       // Slogger::I(TAG, "Removing and adding activity " + newTop + " to stack at "
       //         + top, here);
    }
    mHistory.Insert(mHistory.End(), newTop);
    oldTop->mFrontOfTask = FALSE;
    newTop->mFrontOfTask = TRUE;
    return newTop;
}

ECode ActivityStack::StartActivityLocked(
    /* [in] */ IApplicationThread* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IActivityInfo* aInfo,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& who,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 startFlags,
    /* [in] */ IBundle* options,
    /* [in] */ Boolean componentSpecified,
    /* [in] */ ArrayOf<ActivityRecord*>* outActivity,
    /* [out] */ Int32* status)
{
    String resultWho(who);

    Int32 err = IActivityManager::START_SUCCESS;

    AutoPtr<ProcessRecord> callerApp;
    if (caller != NULL) {
        callerApp = mService->GetRecordForAppLocked(caller);
        if (callerApp != NULL) {
            callingPid = callerApp->mPid;
            callerApp->mInfo->GetUid(&callingUid);
        }
        else {
            // String appApDes, intDes;
            // caller->GetDescription(&appApDes);
            // intent->GetDescription(&intDes);
            // Slogger::W(TAG, StringBuffer("Unable to find app for caller ")
            //         + appApDes + " (pid=" + callingPid + ") when starting: "
            //         + intDes);
            err = IActivityManager::START_PERMISSION_DENIED;
        }
    }

    if (err == IActivityManager::START_SUCCESS) {
        Int32 userId = 0;
        if (aInfo != NULL) {
            AutoPtr<IApplicationInfo> appInfo;
            aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            appInfo->GetUid(&userId);
        }
        String intDes;
        intent->ToShortString(TRUE, TRUE, TRUE, FALSE, &intDes);
        Slogger::I(TAG, "START u %d {%s} from pid %d",
            userId, intDes.string(), (callerApp != NULL ? callerApp->mPid : callingPid));
    }

    AutoPtr<ActivityRecord> sourceRecord;
    AutoPtr<ActivityRecord> resultRecord;
    if (resultTo != NULL) {
        Int32 index = GetIndexOfTokenLocked(resultTo);
        if (DEBUG_RESULTS) {
            // String ibDes;
            // resultTo->GetDescription(&ibDes);
            // Slogger::V(TAG, StringBuffer("Will send result to ") + ibDes
            //         + " (index " + index + ")");
        }
        if (index >= 0) {
            sourceRecord = mHistory[index];
            if (requestCode >= 0 && !sourceRecord->mFinishing) {
                resultRecord = sourceRecord;
            }
        }
    }

    Int32 launchFlags;
    intent->GetFlags(&launchFlags);
    if ((launchFlags & IIntent::FLAG_ACTIVITY_FORWARD_RESULT) != 0
            && sourceRecord != NULL) {
        // Transfer the result target from the source activity to the new
        // one being started, including any failures.
        if (requestCode >= 0) {
            AutoPtr<IActivityOptionsHelper> helper;
            CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
            helper->Abort(options);
            *status = IActivityManager::START_FORWARD_AND_REQUEST_CONFLICT;
            return NOERROR;
        }
        resultRecord = sourceRecord->mResultTo;
        resultWho = sourceRecord->mResultWho;
        requestCode = sourceRecord->mRequestCode;
        sourceRecord->mResultTo = NULL;
        if (resultRecord != NULL) {
            resultRecord->RemoveResultsLocked(
                sourceRecord, resultWho, requestCode);
        }
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);

    if (err == IActivityManager::START_SUCCESS && component == NULL) {
        // We couldn't find a class that can handle the given Intent.
        // That's the end of that!
        err = IActivityManager::START_INTENT_NOT_RESOLVED;
    }

    if (err == IActivityManager::START_SUCCESS && aInfo == NULL) {
        // We couldn't find the specific class specified in the Intent.
        // Also the end of the line.
        err = IActivityManager::START_CLASS_NOT_FOUND;
    }

    if (err != IActivityManager::START_SUCCESS) {
        if (resultRecord != NULL) {
            SendActivityResultLocked(-1,
                    resultRecord, resultWho, requestCode,
                    IActivity::RESULT_CANCELED, NULL);
        }
        mDismissKeyguardOnNextActivity = FALSE;
        AutoPtr<IActivityOptionsHelper> helper;
        CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
        helper->Abort(options);
        *status = err;
        return NOERROR;
    }

    Int32 startAnyPerm;
    FAIL_RETURN(mService->CheckPermission(
           Elastos::Droid::Manifest::Permission::START_ANY_ACTIVITY, callingPid, callingUid, &startAnyPerm));
    String permission;
    aInfo->GetPermission(&permission);
    AutoPtr<IApplicationInfo> appInfo;
    aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);
    Boolean exported;
    aInfo->GetExported(&exported);
    Int32 componentPerm = mService->CheckComponentPermission(permission, callingPid,
           callingUid, uid, exported);
    if (startAnyPerm != IPackageManager::PERMISSION_GRANTED
            && componentPerm != IPackageManager::PERMISSION_GRANTED) {
        if (resultRecord != NULL) {
            SendActivityResultLocked(-1,
                resultRecord, resultWho, requestCode,
                IActivity::RESULT_CANCELED, NULL);
        }
        mDismissKeyguardOnNextActivity = FALSE;
        StringBuilder msg;
        Boolean exported;
        aInfo->GetExported(&exported);
        String intDes, appDes, permission;
        intent->ToString(&intDes);
        appDes = callerApp->ToString();
        aInfo->GetPermission(&permission);
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 uid;
        appInfo->GetUid(&uid);
        if (!exported) {
            msg += "Permission Denial: starting ";
            msg += intDes;
            msg += " from ";
            msg += appDes;
            msg += " (pid=";
            msg += callingPid;
            msg += ", uid=";
            msg += callingUid;
            msg += ")";
            msg += " not exported from uid ";
            msg += uid;
        } else {
            msg += "Permission Denial: starting ";
            msg += intDes;
            msg += " from ";
            msg += appDes;
            msg += " (pid=";
            msg += callingPid;
            msg += ", uid=";
            msg += callingUid;
            msg += ")";
            msg += " requires ";
            msg += permission;
        }
        Slogger::W(TAG, msg.ToString());
        return E_SECURITY_EXCEPTION;
    }

    if (mMainStack) {
        if (mService->mController != NULL) {
            Boolean abort = FALSE;
            // The Intent we give to the watcher has the extra data
            // stripped off, since it can contain private information.
            AutoPtr<IIntent> watchIntent;
            intent->CloneFilter((IIntent**)&watchIntent);
            AutoPtr<IApplicationInfo> appInfo;
            aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String cname;
            appInfo->GetPackageName(&cname);
            if (SUCCEEDED(mService->mController->ActivityStarting(
                    watchIntent, cname, &abort))) {
                abort = !abort;
            }
            else {
                mService->mController = NULL;
            }

            if (abort) {
                if (resultRecord != NULL) {
                    SendActivityResultLocked(-1,
                        resultRecord, resultWho, requestCode,
                        IActivity::RESULT_CANCELED, NULL);
                }
                // We pretend to the caller that it was really started, but
                // they will just get a cancel result.
                mDismissKeyguardOnNextActivity = FALSE;
                AutoPtr<IActivityOptionsHelper> helper;
                CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
                helper->Abort(options);
                *status = IActivityManager::START_SUCCESS;
                return NOERROR;
            }
        }
    }

    AutoPtr<ActivityRecord> r = new ActivityRecord(mService, this, callerApp, callingUid,
            intent, resolvedType, aInfo, mService->mConfiguration,
            resultRecord, resultWho, requestCode, componentSpecified);
    if (outActivity != NULL) {
        outActivity->Set(0, r);
    }

    if (mMainStack) {
        Int32 uid;
        if (mResumedActivity != NULL) {
            AutoPtr<IApplicationInfo> appInfo;
            mResumedActivity->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            appInfo->GetUid(&uid);
        }

        if (mResumedActivity == NULL || uid != callingUid) {
            if (!mService->CheckAppSwitchAllowedLocked(callingPid, callingUid, String("Activity start"))) {
                CActivityManagerService::PendingActivityLaunch* pal = \
                        new CActivityManagerService::PendingActivityLaunch();
                pal->mR = r;
                pal->mSourceRecord = sourceRecord;
                pal->mStartFlags = startFlags;
                mService->mPendingActivityLaunches.PushBack(pal);
                mDismissKeyguardOnNextActivity = FALSE;
                AutoPtr<IActivityOptionsHelper> helper;
                CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
                helper->Abort(options);
                return IActivityManager::START_SWITCHES_CANCELED;
            }
        }

        if (mService->mDidAppSwitch) {
            // This is the second allowed switch since we stopped switches,
            // so now just generally allow switches.  Use case: user presses
            // home (switches disabled, switch to home, mDidAppSwitch now true);
            // user taps a home icon (coming from home so allowed, we hit here
            // and now allow anyone to switch again).
            mService->mAppSwitchesAllowedTime = 0;
        }
        else {
            mService->mDidAppSwitch = TRUE;
        }

        mService->DoPendingActivityLaunchesLocked(FALSE);
    }

    ECode ec = StartActivityUncheckedLocked(r, sourceRecord,
            startFlags, TRUE, options, status);
    if (mDismissKeyguardOnNextActivity && mPausingActivity == NULL) {
        // Someone asked to have the keyguard dismissed on the next
        // activity start, but we are not actually doing an activity
        // switch...  just dismiss the keyguard now, because we
        // probably want to see whatever is behind it.
        mDismissKeyguardOnNextActivity = FALSE;
        mService->mWindowManager->DismissKeyguard();
    }
    return ec;
}

ECode ActivityStack::MoveHomeToFrontFromLaunchLocked(
    /* [in] */ Int32 launchFlags)
{
    if ((launchFlags &
            (IIntent::FLAG_ACTIVITY_NEW_TASK | IIntent::FLAG_ACTIVITY_TASK_ON_HOME))
            == (IIntent::FLAG_ACTIVITY_NEW_TASK | IIntent::FLAG_ACTIVITY_TASK_ON_HOME)) {
        // Caller wants to appear on home activity, so before starting
        // their own activity we will bring home to the front.
        return MoveHomeToFrontLocked();
    }
    return NOERROR;
}

ECode ActivityStack::StartActivityUncheckedLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ ActivityRecord* sourceRecord,
    /* [in] */ Int32 startFlags,
    /* [in] */ Boolean doResume,
    /* [in] */ IBundle* options,
    /* [out] */ Int32* status)
{
    AutoPtr<IIntent> intent = r->mIntent;
    Int32 callingUid = r->mLaunchedFromUid;

    Int32 launchFlags;
    intent->GetFlags(&launchFlags);

    // We'll invoke onUserLeaving before onPause only if the launching
    // activity did not explicitly state that this is an automated launch.
    mUserLeaving = (launchFlags & IIntent::FLAG_ACTIVITY_NO_USER_ACTION) == 0;
    if (DEBUG_USER_LEAVING) {
        Slogger::V(TAG, "startActivity() => mUserLeaving=%d", mUserLeaving);
    }

    // If the caller has asked not to resume at this point, we make note
    // of this in the record so that we can skip it when trying to find
    // the top running activity.
    if (!doResume) {
        r->mDelayedResume = TRUE;
    }

    AutoPtr<ActivityRecord> notTop =
            (launchFlags & IIntent::FLAG_ACTIVITY_PREVIOUS_IS_TOP) != 0 ? r : NULL;

    // If the onlyIfNeeded flag is set, then we can do this if the activity
    // being launched is the same as the one making the call...  or, as
    // a special case, if we do not know the caller then we count the
    // current top activity as the caller.
    Boolean isEqual = FALSE;
    if ((startFlags&IActivityManager::START_FLAG_ONLY_IF_NEEDED) != 0) {
        AutoPtr<ActivityRecord> checkedCaller = sourceRecord;
        if (checkedCaller == NULL) {
            checkedCaller = GetTopRunningNonDelayedActivityLocked(notTop);
        }
        if (checkedCaller->mRealActivity->Equals(r->mRealActivity, &isEqual), !isEqual) {
            // Caller is not the same as launcher, so always needed.
            startFlags &= ~IActivityManager::START_FLAG_ONLY_IF_NEEDED;
        }
    }

    if (sourceRecord == NULL) {
        // This activity is not being started from another...  in this
        // case we -always- start a new task.
        if ((launchFlags & IIntent::FLAG_ACTIVITY_NEW_TASK) == 0) {
            String intDes;
            intent->ToString(&intDes);
            Slogger::W(TAG,
                    "startActivity called from non-Activity context; forcing Intent.FLAG_ACTIVITY_NEW_TASK for: %s",
                    intDes.string());
            launchFlags |= IIntent::FLAG_ACTIVITY_NEW_TASK;
        }
    }
    else if (sourceRecord->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_INSTANCE) {
        // The original activity who is starting us is running as a single
        // instance...  this new activity it is starting must go on its
        // own task.
        launchFlags |= IIntent::FLAG_ACTIVITY_NEW_TASK;
    }
    else if (r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_INSTANCE
            || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_TASK) {
        // The activity being started is a single instance...  it always
        // gets launched into its own task.
        launchFlags |= IIntent::FLAG_ACTIVITY_NEW_TASK;
    }

    if (r->mResultTo != NULL && (launchFlags & IIntent::FLAG_ACTIVITY_NEW_TASK) != 0) {
        // For whatever reason this activity is being launched into a new
        // task...  yet the caller has requested a result back.  Well, that
        // is pretty messed up, so instead immediately send back a cancel
        // and let the new task continue launched as normal without a
        // dependency on its originator.
        Slogger::W(TAG, String("Activity is launching as a new task, so cancelling activity result."));
        SendActivityResultLocked(-1,
                r->mResultTo, r->mResultWho, r->mRequestCode,
                IActivity::RESULT_CANCELED, NULL);
        r->mResultTo = NULL;
    }

    Boolean addingToTask = FALSE;
    Boolean movedHome = FALSE;
    AutoPtr<TaskRecord> reuseTask;
    if (((launchFlags & IIntent::FLAG_ACTIVITY_NEW_TASK) != 0 &&
            (launchFlags & IIntent::FLAG_ACTIVITY_MULTIPLE_TASK) == 0)
            || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_TASK
            || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_INSTANCE) {
        // If bring to front is requested, and no result is requested, and
        // we can find a task that was started with this same
        // component, then instead of launching bring that one to the front.
        if (r->mResultTo == NULL) {
            // See if there is a task to bring to the front.  If this is
            // a SINGLE_INSTANCE activity, there can be one and only one
            // instance of it in the history, and it is always in its own
            // unique task, so we do a special search.
            AutoPtr<ActivityRecord> taskTop =
                    r->mLaunchMode != IActivityInfo::LAUNCH_SINGLE_INSTANCE
                    ? FindTaskLocked(intent, r->mInfo)
                    : FindActivityLocked(intent, r->mInfo);
            if (taskTop != NULL) {
                if (taskTop->mTask->mIntent == NULL) {
                    // This task was started because of movement of
                    // the activity based on affinity...  now that we
                    // are actually launching it, we can assign the
                    // base intent.
                    taskTop->mTask->SetIntent(intent, r->mInfo);
                }
                // If the target task is not in the front, then we need
                // to bring it to the front...  except...  well, with
                // SINGLE_TASK_LAUNCH it's not entirely clear.  We'd like
                // to have the same behavior as if a new instance was
                // being started, which means not bringing it to the front
                // if the caller is not itself in the front.
                AutoPtr<ActivityRecord> curTop =
                        GetTopRunningNonDelayedActivityLocked(notTop);
                if (curTop != NULL && curTop->mTask != taskTop->mTask) {
                    r->mIntent->AddFlags(IIntent::FLAG_ACTIVITY_BROUGHT_TO_FRONT);
                    Boolean callerAtFront = sourceRecord == NULL
                            || curTop->mTask == sourceRecord->mTask;
                    if (callerAtFront) {
                        // We really do want to push this one into the
                        // user's face, right now.
                        movedHome = TRUE;
                        MoveHomeToFrontFromLaunchLocked(launchFlags);
                        MoveTaskToFrontLocked(taskTop->mTask, r, options);
                        options = NULL;
                    }
                }
                // If the caller has requested that the target task be
                // reset, then do so.
                if ((launchFlags & IIntent::FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) != 0) {
                    taskTop = ResetTaskIfNeededLocked(taskTop, r);
                }
                if ((startFlags&IActivityManager::START_FLAG_ONLY_IF_NEEDED)  != 0) {
                    // We don't need to start a new activity, and
                    // the client said not to do anything if that
                    // is the case, so this is it!  And for paranoia, make
                    // sure we have correctly resumed the top activity.
                    if (doResume) {
                        ResumeTopActivityLocked(NULL, options);
                    }
                    else {
                       // ActivityOptions.abort(options);
                    }
                    *status = IActivityManager::START_RETURN_INTENT_TO_CALLER;
                    return NOERROR;
                }
                if ((launchFlags &
                        (IIntent::FLAG_ACTIVITY_NEW_TASK|IIntent::FLAG_ACTIVITY_CLEAR_TASK))
                        == (IIntent::FLAG_ACTIVITY_NEW_TASK|IIntent::FLAG_ACTIVITY_CLEAR_TASK)) {
                    // The caller has requested to completely replace any
                    // existing task with its new activity.  Well that should
                    // not be too hard...
                    reuseTask = taskTop->mTask;
                    PerformClearTaskLocked(taskTop->mTask->mTaskId);
                    reuseTask->SetIntent(r->mIntent, r->mInfo);
                }
                else if ((launchFlags & IIntent::FLAG_ACTIVITY_CLEAR_TOP) != 0
                        || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_TASK
                        || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_INSTANCE) {
                    // In this situation we want to remove all activities
                    // from the task up to the one being started.  In most
                    // cases this means we are resetting the task to its
                    // initial state.
                    AutoPtr<ActivityRecord> top = PerformClearTaskLocked(
                            taskTop->mTask->mTaskId, r, launchFlags);
                    if (top != NULL) {
                        if (top->mFrontOfTask) {
                            // Activity aliases may mean we use different
                            // intents for the top activity, so make sure
                            // the task now has the identity of the new
                            // intent.
                            top->mTask->SetIntent(r->mIntent, r->mInfo);
                        }
                        LogStartActivity(IEventLogTags::AM_NEW_INTENT, r, top->mTask);
                        top->DeliverNewIntentLocked(callingUid, r->mIntent);
                    }
                    else {
                        // A special case: we need to
                        // start the activity because it is not currently
                        // running, and the caller has asked to clear the
                        // current task to have this activity at the top.
                        addingToTask = TRUE;
                        // Now pretend like this activity is being started
                        // by the top of its task, so it is put in the
                        // right place.
                        sourceRecord = taskTop;
                    }
                }
                else if (r->mRealActivity->Equals(taskTop->mTask->mRealActivity,
                        &isEqual), isEqual) {
                    // In this case the top activity on the task is the
                    // same as the one being launched, so we take that
                    // as a request to bring the task to the foreground.
                    // If the top activity in the task is the root
                    // activity, deliver this new intent to it if it
                    // desires.
                    if (((launchFlags & IIntent::FLAG_ACTIVITY_SINGLE_TOP) != 0
                            || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_TOP)
                            && (taskTop->mRealActivity->Equals(r->mRealActivity, &isEqual), isEqual)) {
                        LogStartActivity(IEventLogTags::AM_NEW_INTENT, r, taskTop->mTask);
                        if (taskTop->mFrontOfTask) {
                            taskTop->mTask->SetIntent(r->mIntent, r->mInfo);
                        }
                        taskTop->DeliverNewIntentLocked(callingUid, r->mIntent);
                    }
                    else if (r->mIntent->FilterEquals(taskTop->mTask->mIntent, &isEqual), !isEqual) {
                        // In this case we are launching the root activity
                        // of the task, but with a different intent.  We
                        // should start a new instance on top.
                        addingToTask = TRUE;
                        sourceRecord = taskTop;
                    }
                }
                else if ((launchFlags & IIntent::FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) == 0) {
                    // In this case an activity is being launched in to an
                    // existing task, without resetting that task.  This
                    // is typically the situation of launching an activity
                    // from a notification or shortcut.  We want to place
                    // the new activity on top of the current task.
                    addingToTask = TRUE;
                    sourceRecord = taskTop;
                }
                else if (!taskTop->mTask->mRootWasReset) {
                    // In this case we are launching in to an existing task
                    // that has not yet been started from its front door.
                    // The current task has been brought to the front.
                    // Ideally, we'd probably like to place this new task
                    // at the bottom of its stack, but that's a little hard
                    // to do with the current organization of the code so
                    // for now we'll just drop it.
                    taskTop->mTask->SetIntent(r->mIntent, r->mInfo);
                }
                if (!addingToTask && reuseTask == NULL) {
                    // We didn't do anything...  but it was needed (a.k.a., client
                    // don't use that intent!)  And for paranoia, make
                    // sure we have correctly resumed the top activity.
                    if (doResume) {
                        ResumeTopActivityLocked(NULL, options);
                    }
                    else {
                       // ActivityOptions.abort(options);
                    }
                    *status = IActivityManager::START_TASK_TO_FRONT;
                    return NOERROR;
                }
            }
        }
    }

    if (!r->mPackageName.IsNull()) {
        // If the activity being launched is the same as the one currently
        // at the top, then we need to check if it should only be launched
        // once.
        AutoPtr<ActivityRecord> top = GetTopRunningNonDelayedActivityLocked(notTop);
        if (top != NULL && r->mResultTo == NULL) {
            if (top->mRealActivity->Equals(r->mRealActivity, &isEqual), isEqual
                    && top->mUserId == r->mUserId) {
                if (top->mApp != NULL && top->mApp->mThread != NULL) {
                    if ((launchFlags & IIntent::FLAG_ACTIVITY_SINGLE_TOP) != 0
                        || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_TOP
                        || r->mLaunchMode == IActivityInfo::LAUNCH_SINGLE_TASK) {
                        LogStartActivity(IEventLogTags::AM_NEW_INTENT, top, top->mTask);
                        // For paranoia, make sure we have correctly
                        // resumed the top activity.
                        if (doResume) {
                            ResumeTopActivityLocked(NULL);
                        }
                       // ActivityOptions.abort(options);
                        if ((startFlags&IActivityManager::START_FLAG_ONLY_IF_NEEDED) != 0) {
                            // We don't need to start a new activity, and
                            // the client said not to do anything if that
                            // is the case, so this is it!
                            *status = IActivityManager::START_RETURN_INTENT_TO_CALLER;
                            return NOERROR;
                        }
                        top->DeliverNewIntentLocked(callingUid, r->mIntent);
                        *status = IActivityManager::START_DELIVERED_TO_TOP;
                        return NOERROR;
                    }
                }
            }
        }
    }
    else {
        if (r->mResultTo != NULL) {
            SendActivityResultLocked(-1,
                    r->mResultTo, r->mResultWho, r->mRequestCode,
                    IActivity::RESULT_CANCELED, NULL);
        }
        AutoPtr<IActivityOptionsHelper> helper;
        CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
        helper->Abort(options);
        *status = IActivityManager::START_CLASS_NOT_FOUND;
        return NOERROR;
    }

    Boolean newTask = FALSE;
    Boolean keepCurTransition = FALSE;

    // Should this be considered a new task?
    if (r->mResultTo == NULL && !addingToTask
            && (launchFlags & IIntent::FLAG_ACTIVITY_NEW_TASK) != 0) {
        if (reuseTask == NULL) {
            // todo: should do better management of integers.
            mService->mCurTask++;
            if (mService->mCurTask <= 0) {
                mService->mCurTask = 1;
            }
            AutoPtr<TaskRecord> task = new TaskRecord(mService->mCurTask, r->mInfo, intent);
            r->SetTask(task, NULL, TRUE);
            if (DEBUG_TASKS) {
                String arDes, trDes;
                r->ToString(&arDes);
                trDes = r->mTask->ToString();
                Slogger::V(TAG, "Starting new activity %s in new task %s", arDes.string(),
                        trDes.string());
            }
        }
        else {
             r->SetTask(reuseTask, reuseTask, TRUE);
        }
        newTask = TRUE;
        if (!movedHome) {
            MoveHomeToFrontFromLaunchLocked(launchFlags);
        }
    }
    else if (sourceRecord != NULL) {
        if (!addingToTask &&
                (launchFlags & IIntent::FLAG_ACTIVITY_CLEAR_TOP) != 0) {
            // In this case, we are adding the activity to an existing
            // task, but the caller has asked to clear that task if the
            // activity is already running.
            AutoPtr<ActivityRecord> top = PerformClearTaskLocked(
                    sourceRecord->mTask->mTaskId, r, launchFlags);
            keepCurTransition = TRUE;
            if (top != NULL) {
                LogStartActivity(IEventLogTags::AM_NEW_INTENT, r, top->mTask);
                top->DeliverNewIntentLocked(callingUid, r->mIntent);
                // For paranoia, make sure we have correctly
                // resumed the top activity.
                if (doResume) {
                    ResumeTopActivityLocked(NULL);
                }
                AutoPtr<IActivityOptionsHelper> helper;
                CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
                helper->Abort(options);
                *status = IActivityManager::START_DELIVERED_TO_TOP;
                return NOERROR;
            }
        }
        else if (!addingToTask &&
                (launchFlags & IIntent::FLAG_ACTIVITY_REORDER_TO_FRONT) != 0) {
            // In this case, we are launching an activity in our own task
            // that may already be running somewhere in the history, and
            // we want to shuffle it to the front of the stack if so.
            Int32 where = FindActivityInHistoryLocked(r, sourceRecord->mTask->mTaskId);
            if (where >= 0) {
                AutoPtr<ActivityRecord> top = MoveActivityToFrontLocked(where);
                LogStartActivity(IEventLogTags::AM_NEW_INTENT, r, top->mTask);
                top->UpdateOptionsLocked(options);
                top->DeliverNewIntentLocked(callingUid, r->mIntent);
                if (doResume) {
                    ResumeTopActivityLocked(NULL);
                }
                *status = IActivityManager::START_DELIVERED_TO_TOP;
                return NOERROR;
            }
        }
        // An existing activity is starting this new activity, so we want
        // to keep the new one in the same task as the one that is starting
        // it.
        r->SetTask(sourceRecord->mTask, sourceRecord->mThumbHolder, FALSE);
        if (DEBUG_TASKS) {
            String arDes, trDes;
            r->ToString(&arDes);
            trDes = r->mTask->ToString();
            Slogger::V(TAG, "Starting new activity %s in existing task %s", arDes.string(),
                    trDes.string());
        }
    }
    else {
        // This not being started from an existing activity, and not part
        // of a new task...  just put it in the top task, though these days
        // this case should never happen.
        Int32 size = mHistory.GetSize();
        AutoPtr<ActivityRecord> prev = size > 0 ? mHistory[size - 1] : NULL;
        Int32 flags;
        r->mInfo->GetFlags(&flags);
        AutoPtr<TaskRecord> tr;
        if(prev != NULL){
            tr = prev->mTask;
        }
        else{
            tr = new TaskRecord(mService->mCurTask, r->mInfo, intent);
        }
        r->SetTask(tr, NULL, TRUE);
        if (DEBUG_TASKS) {
            String arDes, trDes;
            r->ToString(&arDes);
            trDes = r->mTask->ToString();
            Slogger::V(TAG, "Starting new activity %s in new guessed %s", arDes.string(),
                    trDes.string());
        }
    }

    mService->GrantUriPermissionFromIntentLocked(callingUid, r->mPackageName,
            intent, r->GetUriPermissionsLocked());

   // if (newTask) {
   //     EventLog.writeEvent(IEventLogTags.AM_CREATE_TASK, r.userId, r.task.taskId);
   // }
    LogStartActivity(IEventLogTags::AM_CREATE_ACTIVITY, r, r->mTask);
    StartActivityLocked(r, newTask, doResume, keepCurTransition, options);
    *status = IActivityManager::START_SUCCESS;
    return NOERROR;
}

AutoPtr<IActivityInfo> ActivityStack::ResolveActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 startFlags,
    /* [in] */ const String& profileFile,
    /* [in] */ IParcelFileDescriptor* profileFd,
    /* [in] */ Int32 userId)
{
    // Collect information about the target of the Intent.
    AutoPtr<IActivityInfo> aInfo;
//     try {
    AutoPtr<IResolveInfo> rInfo;
    AppGlobals::GetPackageManager()->ResolveIntent(
            intent, resolvedType,
            IPackageManager::MATCH_DEFAULT_ONLY
            | CActivityManagerService::STOCK_PM_FLAGS, userId, (IResolveInfo**)&rInfo);
    if (rInfo != NULL) {
        rInfo->GetActivityInfo((IActivityInfo**)&aInfo);
    }
//     } catch (RemoteException e) {
//         aInfo = NULL;
//     }

    if (aInfo != NULL) {
        // Store the found target back into the intent, because now that
        // we have it we never want to do this again.  For example, if the
        // user navigates back to this point in the history, we should
        // always restart the exact same activity.
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        String pkgName, name;
        appInfo->GetPackageName(&pkgName);
        aInfo->GetName(&name);
        AutoPtr<IComponentName> component;
        CComponentName::New(pkgName, name, (IComponentName**)&component);
        intent->SetComponent(component);

        // Don't debug things in the system process
        String processName;
        aInfo->GetProcessName(&processName);
        if ((startFlags & IActivityManager::START_FLAG_DEBUG) != 0) {
            if (!processName.Equals("system")) {
                mService->SetDebugApp(processName, TRUE, FALSE);
            }
        }

        if ((startFlags & IActivityManager::START_FLAG_OPENGL_TRACES) != 0) {
            if (!processName.Equals("system")) {
                AutoPtr<IApplicationInfo> appInfo;
                aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                mService->SetOpenGlTraceApp(appInfo, processName);
            }
        }

        if (profileFile != NULL) {
            if (!processName.Equals("system")) {
                AutoPtr<IApplicationInfo> appInfo;
                aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                mService->SetProfileApp(appInfo, processName,
                        profileFile, profileFd,
                        (startFlags & IActivityManager::START_FLAG_AUTO_STOP_PROFILER) != 0);
            }
        }
    }
    return aInfo;
}

ECode ActivityStack::StartActivityMayWait(
    /* [in] */ IApplicationThread* caller,
    /* [in] */ Int32 callingUid,
    /* [in] */ IIntent* intent_,
    /* [in] */ const String& resolvedType_,
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
    /* [out] */ Int32* status)
{
    assert(status != NULL);
    String resolvedType(resolvedType_);

    AutoPtr<IIntent> intent = intent_;
    // Refuse possible leaked file descriptors
    Boolean hasFD = FALSE;
    if (intent != NULL && (intent->HasFileDescriptors(&hasFD), hasFD)) {
        Slogger::E(TAG, "File descriptors passed in Intent.\n");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    Boolean componentSpecified = component != NULL;

    // Don't modify the client's object!
    AutoPtr<IIntent> tempIntent;
    CIntent::New(intent, (IIntent**)&tempIntent);
    intent = tempIntent;

    // Collect information about the target of the Intent.
    AutoPtr<IActivityInfo> aInfo = ResolveActivity(intent, resolvedType, startFlags,
                profileFile, profileFd, userId);

    AutoLock lock(mService->mLock);

    Int32 callingPid;
    if (callingUid >= 0) {
        callingPid = -1;
    }
    else if (caller == NULL) {
        callingPid = Binder::GetCallingPid();
        callingUid = Binder::GetCallingUid();
    }
    else {
        callingPid = callingUid = -1;
    }

    Int32 result;
    mConfigWillChange = config != NULL &&
            (mService->mConfiguration->Diff(config, &result), result != 0);
    if (DEBUG_CONFIGURATION) {
        Slogger::V(TAG, "Starting activity when config will change = %d",
                mConfigWillChange);
    }

    const Int64 origId = Binder::ClearCallingIdentity();
    if (mMainStack && aInfo != NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 flags;
        appInfo->GetFlags(&flags);
        if ((flags & IApplicationInfo::FLAG_CANT_SAVE_STATE) != 0) {
            // This may be a heavy-weight process!  Check to see if we already
            // have another, different heavy-weight process running.
            String pname, cname;
            Int32 uid;
            aInfo->GetProcessName(&pname);
            appInfo->GetPackageName(&cname);
            appInfo->GetUid(&uid);
            if (!pname.Compare(cname)) {
                if (mService->mHeavyWeightProcess != NULL) {
                    Int32 uid2;
                    mService->mHeavyWeightProcess->mInfo->GetUid(&uid2);
                    if (uid2 != uid || mService->mHeavyWeightProcess->mProcessName.Compare(pname)) {
                        Int32 realCallingPid = callingPid;
                        Int32 realCallingUid = callingUid;
                        if (caller != NULL) {
                            AutoPtr<ProcessRecord> callerApp = mService->GetRecordForAppLocked(caller);
                            if (callerApp != NULL) {
                                realCallingPid = callerApp->mPid;
                                callerApp->mInfo->GetUid(&realCallingUid);
                            }
                            else {
                                // String appApDes, intDes;
                                // caller->GetDescription(&appApDes);
                                // intent->GetDescription(&intDes);
                                // Slogger::W(TAG, StringBuffer("Unable to find app for caller ") + appApDes
                                //         + " (pid=" + realCallingPid + ") when starting: " + intDes);
                                AutoPtr<IActivityOptionsHelper> helper;
                                CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
                                helper->Abort(options);
                                return IActivityManager::START_PERMISSION_DENIED;
                            }
                        }

                        AutoPtr<IIIntentSender> target;
                        AutoPtr<ArrayOf<IIntent*> > intentArr = ArrayOf<IIntent*>::Alloc(1);
                        intentArr->Set(0, intent);
                        AutoPtr<ArrayOf<String> > strArr = ArrayOf<String>::Alloc(1);
                        (*strArr)[0] = resolvedType;

                        mService->GetIntentSenderLocked(
                                IActivityManager::INTENT_SENDER_ACTIVITY, String("elastos"),
                                realCallingUid, userId, NULL, String(NULL), 0, intentArr,
                                strArr, IPendingIntent::FLAG_CANCEL_CURRENT
                                | IPendingIntent::FLAG_ONE_SHOT, NULL, (IIIntentSender**)&target);

                        AutoPtr<IIntent> newIntent;
                        CIntent::New((IIntent**)&newIntent);
                        if (requestCode >= 0) {
                            // Caller is requesting a result.
                            newIntent->PutBooleanExtra(
                                    IHeavyWeightSwitcherActivity::KEY_HAS_RESULT,
                                    TRUE);
                        }
                        AutoPtr<IParcelable> is;
                        CIntentSender::New(target.Get(), (IParcelable**)&is);
                        newIntent->PutExtra(
                                IHeavyWeightSwitcherActivity::KEY_INTENT,
                                is.Get());
                        if (mService->mHeavyWeightProcess->mActivities.IsEmpty() == FALSE) {
                            AutoPtr<ActivityRecord> hist = mService->mHeavyWeightProcess->mActivities.GetFront();
                            newIntent->PutExtra(
                                    IHeavyWeightSwitcherActivity::KEY_CUR_APP,
                                    hist->mPackageName);
                            newIntent->PutExtra(
                                    IHeavyWeightSwitcherActivity::KEY_CUR_TASK,
                                    hist->mTask->mTaskId);
                        }
                        String acname;
                        aInfo->GetPackageName(&acname);
                        newIntent->PutExtra(
                                IHeavyWeightSwitcherActivity::KEY_NEW_APP,
                                acname);
                        Int32 flags;
                        intent->GetFlags(&flags);
                        newIntent->SetFlags(flags);
                       // newIntent.setClassName("android",
                       //         HeavyWeightSwitcherActivity.class.getName());
                        intent = newIntent;
                        resolvedType = NULL;
                        caller = NULL;
                        callingUid = Binder::GetCallingUid();
                        callingPid = Binder::GetCallingPid();
                        componentSpecified = TRUE;

                        AutoPtr<IResolveInfo> rInfo;
                        ECode ec = AppGlobals::GetPackageManager()->ResolveIntent(newIntent, String(NULL),
                                 IPackageManager::MATCH_DEFAULT_ONLY | CActivityManagerService::STOCK_PM_FLAGS,
                                userId, (IResolveInfo**)&rInfo);
                        if (SUCCEEDED(ec) && rInfo.Get()) {
                            AutoPtr<IActivityInfo> temp;
                            rInfo->GetActivityInfo((IActivityInfo**)&temp);
                            aInfo = mService->GetActivityInfoForUser(temp, userId);
                        }
                        else {
                            aInfo = NULL;
                        }
                    }
                }
            }
        }
    }

    ECode ec = StartActivityLocked(caller, intent, resolvedType,
            aInfo,
            resultTo, resultWho, requestCode, callingPid, callingUid,
            startFlags, options, componentSpecified, NULL, status);

    //ToDo: when to free _grantedUriPermissions;

    if (mConfigWillChange && mMainStack) {
        // If the caller also wants to switch to a new configuration,
        // do so now.  This allows a clean switch, as we are waiting
        // for the current activity to pause (so we will not destroy
        // it), and have not yet started the next activity.
        FAIL_RETURN(mService->EnforceCallingPermission(Elastos::Droid::Manifest::Permission::CHANGE_CONFIGURATION, String("updateConfiguration()")));
        mConfigWillChange = FALSE;
        if (DEBUG_CONFIGURATION) {
            Slogger::V(TAG, "Updating to new configuration after starting activity.");
        }
        mService->UpdateConfigurationLocked(config, NULL, FALSE, FALSE);
    }

    Binder::RestoreCallingIdentity(origId);

    if (outResult != NULL) {
        outResult->SetResult(*status);
        if (*status == IActivityManager::START_SUCCESS) {
            mWaitingActivityLaunched.PushBack(outResult);
            Boolean isTimeout;
            AutoPtr<IComponentName> who;
            do {
                mService->mLock.Wait();
                who = NULL;
                outResult->GetTimeout(&isTimeout);
                outResult->GetWho((IComponentName**)&who);
            } while (!isTimeout && who == NULL);
        }
        else if (*status == IActivityManager::START_TASK_TO_FRONT) {
            AutoPtr<ActivityRecord> r = GetTopRunningActivityLocked(NULL);
            if (r->mNowVisible) {
                outResult->SetTimeout(FALSE);
                String cname, name;
                r->mInfo->GetPackageName(&cname);
                r->mInfo->GetName(&name);
                AutoPtr<IComponentName> who;
                CComponentName::New(cname, name, (IComponentName**)&who);
                outResult->SetWho(who);
                outResult->SetTotalTime(0);
                outResult->SetThisTime(0);
            }
            else {
                outResult->SetThisTime(SystemClock::GetUptimeMillis());
                mWaitingActivityVisible.PushBack(outResult);
                Boolean isTimeout;
                AutoPtr<IComponentName> who;
                do {
                    mService->mLock.Wait();
                    who = NULL;
                    outResult->GetTimeout(&isTimeout);
                    outResult->GetWho((IComponentName**)&who);
                } while (!isTimeout && who == NULL);
            }
        }
    }
    return ec;
}

ECode ActivityStack::StartActivities(
    /* [in] */ IApplicationThread* caller,
    /* [in] */ Int32 callingUid,
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ ArrayOf<String>* resolvedTypes,
    /* [in] */ IBinder* resultTo,
    /* [in] */ IBundle* options,
    /* [in] */ Int32 userId,
    /* [out] */ Int32* stats)
{
    assert(stats != NULL);

    if (intents == NULL) {
        // throw new NULLPointerException("intents is NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (resolvedTypes == NULL) {
        // throw new NULLPointerException("resolvedTypes is NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (intents->GetLength() != resolvedTypes->GetLength()) {
        // throw new IllegalArgumentException("intents are length different than resolvedTypes");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ArrayOf<ActivityRecord*> > outActivity = ArrayOf<ActivityRecord*>::Alloc(1);

    Int32 callingPid;
    if (callingUid >= 0) {
        callingPid = -1;
    }
    else if (caller == NULL) {
        callingPid = Binder::GetCallingPid();
        callingUid = Binder::GetCallingUid();
    }
    else {
        callingPid = callingUid = -1;
    }
    Int64 origId = Binder::ClearCallingIdentity();
    // try {
    {
        AutoLock lock(mService->mLock);

        for (Int32 i = 0; i < intents->GetLength(); i++) {
            AutoPtr<IIntent> intent = (*intents)[i];
            if (intent == NULL) {
                continue;
            }

            // Refuse possible leaked file descriptors
            Boolean hasDes;
            intent->HasFileDescriptors(&hasDes);
            if (hasDes) {
                // throw new IllegalArgumentException("File descriptors passed in Intent");
                Binder::RestoreCallingIdentity(origId);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            AutoPtr<IComponentName> component;
            intent->GetComponent((IComponentName**)&component);
            Boolean componentSpecified = component != NULL;

            // Don't modify the client's object!
            AutoPtr<IIntent> newIntent;
            CIntent::New(intent, (IIntent**)&newIntent);

            // Collect information about the target of the Intent.
            AutoPtr<IActivityInfo> aInfo = ResolveActivity(intent, (*resolvedTypes)[i],
                    0, String(NULL), NULL, userId);
            // TODO: New, check if this is correct
            aInfo = mService->GetActivityInfoForUser(aInfo, userId);

            if (mMainStack && aInfo != NULL) {
                AutoPtr<IApplicationInfo> appInfo;
                aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                Int32 flags;
                appInfo->GetFlags(&flags);
                if ((flags & IApplicationInfo::FLAG_CANT_SAVE_STATE) != 0) {
                    // throw new IllegalArgumentException(
                    //         "FLAG_CANT_SAVE_STATE not supported here");
                    Binder::RestoreCallingIdentity(origId);
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }

            AutoPtr<IBundle> theseOptions;
            if (options != NULL && i == intents->GetLength() - 1) {
                theseOptions = options;
            }
            else {
                theseOptions = NULL;
            }
            Int32 res;
            StartActivityLocked(caller, intent, (*resolvedTypes)[i],
                    aInfo, resultTo, String(NULL), -1, callingPid, callingUid,
                    0, theseOptions, componentSpecified, outActivity, &res);
            if (res < 0) {
                *stats = res;
                Binder::RestoreCallingIdentity(origId);
                return NOERROR;
            }

            resultTo = (*outActivity)[0] != NULL ? IBinder::Probe((*outActivity)[0]->mAppToken) : NULL;
        }
    }
    // } finally {
    Binder::RestoreCallingIdentity(origId);
    // }

    *stats = IActivityManager::START_SUCCESS;
    return NOERROR;
}

ECode ActivityStack::ReportActivityLaunchedLocked(
    /* [in] */ Boolean timeout,
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int64 thisTime,
    /* [in] */ Int64 totalTime)
{
    List<AutoPtr<IActivityManagerWaitResult> >::ReverseIterator it;
    for(it = mWaitingActivityLaunched.RBegin(); it != mWaitingActivityLaunched.REnd();
        ++it) {
        AutoPtr<IActivityManagerWaitResult> w = *it;
        if (r != NULL) {
            AutoPtr<IComponentName> component;
            String cName,name;
            r->mInfo->GetPackageName(&cName);
            r->mInfo->GetName(&name);
            CComponentName::New(cName, name, (IComponentName**)&component);
            w->SetWho(component);
        }
        w->SetThisTime(thisTime);
        w->SetTotalTime(totalTime);
    }
    mService->mLock.NotifyAll();
    return NOERROR;
}

ECode ActivityStack::ReportActivityVisibleLocked(
        /* [in] */ ActivityRecord* r)
{
    List<AutoPtr<IActivityManagerWaitResult> >::ReverseIterator it;
    for(it = mWaitingActivityVisible.RBegin(); it != mWaitingActivityVisible.REnd(); ++it){
        AutoPtr<IActivityManagerWaitResult> w = *it;
        w->SetTimeout(FALSE);
        String cName, name;
        r->mInfo->GetPackageName(&cName);
        r->mInfo->GetName(&name);
        if(r != NULL){
            AutoPtr<IComponentName> component;
            CComponentName::New(cName, name, (IComponentName**)&component);
            w->SetWho(component);
        }
        Int64 thisTime;
        w->GetThisTime(&thisTime);
        w->SetTotalTime(SystemClock::GetUptimeMillis() - thisTime);
        Int64 totalTime;
        w->GetTotalTime(&totalTime);
        w->SetThisTime(totalTime);
    }
    mService->mLock.NotifyAll();

    if (mDismissKeyguardOnNextActivity) {
        mDismissKeyguardOnNextActivity = FALSE;
        mService->mWindowManager->DismissKeyguard();
    }
    return NOERROR;
}

ECode ActivityStack::SendActivityResultLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ ActivityRecord* r,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    ECode ec = NOERROR;
    if (callingUid > 0) {
        mService->GrantUriPermissionFromIntentLocked(callingUid, r->mPackageName,
                data, r->GetUriPermissionsLocked());
    }

    if (DEBUG_RESULTS) {
        String arDes, intDes;
        r->ToString(&arDes);
        data->ToString(&intDes);
        Slogger::V(TAG, "Send activity result to %s : who=%s req=%d res=%d data=%s", arDes.string(),
                resultWho.string(), requestCode, resultCode, intDes.string());
    }
    if ((ActivityRecord*)mResumedActivity == r &&
            r->mApp != NULL && r->mApp->mThread != NULL) {
        AutoPtr<IResultInfo> info;
        CResultInfo::New(resultWho, requestCode,
                resultCode, data, (IResultInfo**)&info);

        AutoPtr<IObjectContainer> list;
        CParcelableObjectContainer::New((IObjectContainer**)&list);
        list->Add(info);

        ec = r->mApp->mThread->ScheduleSendResult(IBinder::Probe(r->mAppToken), list);
        if (SUCCEEDED(ec)) return ec;
        else {
            String arDes;
            r->ToString(&arDes);
            Slogger::W(TAG, "Exception thrown sending result to %s", arDes.string());
        }
    }

    r->AddResultLocked(NULL, resultWho, requestCode, resultCode, data);
    return ec;
}

ECode ActivityStack::StopActivityLocked(
    /* [in] */ ActivityRecord* r)
{
    if (DEBUG_SWITCH) Slogger::D(TAG, "Stopping: %p", r);
    Int32 intentFlags, infoFlags;
    r->mIntent->GetFlags(&intentFlags);
    r->mInfo->GetFlags(&infoFlags);
    if ((intentFlags & IIntent::FLAG_ACTIVITY_NO_HISTORY) != 0
            || (infoFlags & IActivityInfo::FLAG_NO_HISTORY) != 0) {
        if (!r->mFinishing) {
            if (!mService->mSleeping) {
                if (DEBUG_STATES) {
                    Slogger::D(TAG, "no-history finish of %p", r);
                }
                RequestFinishActivityLocked(IBinder::Probe(r->mAppToken), IActivity::RESULT_CANCELED, NULL,
                        String("no-history"), FALSE);
            } else {
                if (DEBUG_STATES) Slogger::D(TAG, "Not finishing noHistory %p on stop because we're just sleeping", r);
            }
        }
    }
    if (r->mApp != NULL && r->mApp->mThread != NULL) {
        if (mMainStack) {
            if ((ActivityRecord*)(mService->mFocusedActivity) == r) {
                mService->SetFocusedActivityLocked(GetTopRunningActivityLocked(NULL));
            }
        }
        r->ResumeKeyDispatchingLocked();
        // try {
            r->mStopped = FALSE;
            if (DEBUG_STATES) Slogger::V(TAG, "Moving to STOPPING: %p (stop requested)", r);
            r->mState = ActivityState_STOPPING;
            if (DEBUG_VISBILITY) Slogger::V(
                    TAG, "Stopping visible=%d for %p" + r->mVisible, r);
            ECode res1,res2;
            if (!r->mVisible) {
                res1 = mService->mWindowManager->SetAppVisibility(
                    IBinder::Probe(r->mAppToken), FALSE);
            }
            res2 = r->mApp->mThread->ScheduleStopActivity(IBinder::Probe(r->mAppToken), r->mVisible, r->mConfigChangeFlags);
            Boolean sleeped = mService->IsSleeping();
            if (sleeped) {
                r->SetSleeping(TRUE);
            }

            AutoPtr<IMessage> msg;
            mHandler->ObtainMessage(STOP_TIMEOUT_MSG, (IMessage**)&msg);
            msg->SetObj(r->Probe(EIID_IInterface));
            Boolean result;
            ECode ec = mHandler->SendMessageDelayed(msg, STOP_TIMEOUT, &result);
        // } catch (Exception e) {
            if(FAILED(res1) || FAILED(res2) || FAILED(ec)) {
            // Maybe just ignore exceptions here...  if the process
            // has crashed, our death notification will clean things
            // up.
                Slogger::W(TAG, String("Exception thrown during pause"));
                // Just in case, assume it to be stopped.
                r->mStopped = TRUE;
                // if (DEBUG_STATES) Slogger::V(TAG, String("Stop failed; moving to STOPPED: ") + r);
                r->mState = ActivityState_STOPPED;
                if (r->mConfigDestroy) {
                    DestroyActivityLocked(r, TRUE, FALSE, String("stop-except"));
                }
            }
    }
    return NOERROR;
}

AutoPtr< List<AutoPtr<ActivityRecord> > >
ActivityStack::ProcessStoppingActivitiesLocked(
    /* [in] */ Boolean remove)
{
    if (mStoppingActivities.IsEmpty()) return NULL;

    AutoPtr< List<AutoPtr<ActivityRecord> > > stops;

    const Boolean nowVisible = mResumedActivity != NULL
            && mResumedActivity->mNowVisible
            && !mResumedActivity->mWaitingVisible;
    List<AutoPtr<ActivityRecord> >::Iterator it;
    for (it = mStoppingActivities.Begin(); it != mStoppingActivities.End();) {
        AutoPtr<ActivityRecord> s = *it;
        if (localLOGV) Slogger::V(TAG, "Stopping %p: nowVisible=%d waitingVisible=%d finishing=%d", s.Get(),
                nowVisible, s->mWaitingVisible, s->mFinishing);
        if (s->mWaitingVisible && nowVisible) {
            mWaitingVisibleActivities.Remove(s);
            s->mWaitingVisible = FALSE;
            if (s->mFinishing) {
                // If this activity is finishing, it is sitting on top of
                // everyone else but we now know it is no longer needed...
                // so get rid of it.  Otherwise, we need to go through the
                // normal flow and hide it once we determine that it is
                // hidden by the activities in front of it.
                if (localLOGV) Slogger::V(TAG, "Before stopping, can hide: %p", s.Get());
                mService->mWindowManager->SetAppVisibility(IBinder::Probe(s->mAppToken), FALSE);
            }
        }
        if ((!s->mWaitingVisible || mService->IsSleeping()) && remove) {
            if (localLOGV) Slogger::V(TAG, "Ready to stop: %p", s.Get());
            if (stops == NULL) {
                stops = new List<AutoPtr<ActivityRecord> >();
            }
            stops->PushBack(s);
            it = mStoppingActivities.Erase(it);
        }
        else {
            ++it;
        }
    }

    return stops;
}

ECode ActivityStack::ScheduleIdleLocked()
{
    Boolean result;
    return mHandler->SendEmptyMessage(IDLE_NOW_MSG, &result);
}

AutoPtr<ActivityRecord> ActivityStack::ActivityIdleInternal(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean fromTimeout,
    /* [in] */ IConfiguration* config)
{
    if (localLOGV) Slogger::V(TAG, "Activity idle: %p", token);

    AutoPtr<ActivityRecord> res;
    AutoPtr<ActivityRecordList> stops;
    AutoPtr<ActivityRecordList> finishes;
    AutoPtr<ActivityRecordList> thumbnails;
    AutoPtr<List<AutoPtr<UserStartedState> > > startingUsers;
    AutoPtr<IApplicationThread> sendThumbnail;
    Boolean booting = FALSE;
    Boolean enableScreen = FALSE;
    Boolean activityRemoved = FALSE;

    {
        AutoLock lock(mService->mLock);

        AutoPtr<ActivityRecord> r;
        ActivityRecord::ForToken(token, (ActivityRecord**)&r);
        if (r != NULL) {
            mHandler->RemoveMessages(IDLE_TIMEOUT_MSG, r->Probe(EIID_IInterface));
            r->FinishLaunchTickingLocked();
        }

        // Get the activity record.
        Int32 index = GetIndexOfActivityLocked(r);
        if (index >= 0) {
            res = r;

            if (fromTimeout) {
                ReportActivityLaunchedLocked(fromTimeout, r, -1, -1);
            }

            // This is a hack to semi-deal with a race condition
            // in the client where it can be constructed with a
            // newer configuration from when we asked it to launch.
            // We'll update with whatever configuration it now says
            // it used to launch.
            if (config != NULL) {
                r->mConfiguration = config;
            }

            // No longer need to keep the device awake.
            Boolean held;
            if (mResumedActivity == r && (mLaunchingActivity->IsHeld(&held), held)) {
                mHandler->RemoveMessages(LAUNCH_TIMEOUT_MSG);
                mLaunchingActivity->ReleaseLock();
            }

            // We are now idle.  If someone is waiting for a thumbnail from
            // us, we can now deliver.
            r->mIdle = TRUE;
            mService->ScheduleAppGcsLocked();
            if (r->mThumbnailNeeded && r->mApp != NULL && r->mApp->mThread != NULL) {
                sendThumbnail = r->mApp->mThread;
                r->mThumbnailNeeded = FALSE;
            }

            // If this activity is fullscreen, set up to hide those under it.

            if (DEBUG_VISBILITY) Slogger::V(TAG, "Idle activity for %p", r.Get());
            EnsureActivitiesVisibleLocked(NULL, 0);

            //Slogger::I(TAG, StringBuffer("IDLE: mBooted=") + mBooted + StringBuffer(", fromTimeout=") + fromTimeout);
            if (mMainStack) {
                if (!mService->mBooted) {
                    mService->mBooted = TRUE;
                    enableScreen = TRUE;
                }
            }

        }
        else if (fromTimeout) {
            ReportActivityLaunchedLocked(fromTimeout, NULL, -1, -1);
        }

        // Atomically retrieve all of the other things to do.
        stops = ProcessStoppingActivitiesLocked(TRUE);
        if (!mFinishingActivities.IsEmpty()) {
            finishes = new List<AutoPtr<ActivityRecord> >(mFinishingActivities);
            mFinishingActivities.Clear();
        }
        if (!mService->mCancelledThumbnails.IsEmpty()) {
            thumbnails = new List<AutoPtr<ActivityRecord> >(mService->mCancelledThumbnails);
            mService->mCancelledThumbnails.Clear();
        }

        if (mMainStack) {
            booting = mService->mBooting;
            mService->mBooting = FALSE;
        }
        if (!mStartingUsers.IsEmpty()) {
            startingUsers = new List<AutoPtr<UserStartedState> >(mStartingUsers);
            mStartingUsers.Clear();
        }
    }

    Int32 i;

    // Send thumbnail if requested.
    if (sendThumbnail != NULL) {
    //     try {
        ECode res = sendThumbnail->RequestThumbnail(token);
    //     } catch (Exception e) {
        if (res != NOERROR) {
            // Slogger::W(TAG, StringBuffer("Exception thrown when requesting thumbnail") + res);
            // mService->SendPendingThumbnail(NULL, token, NULL, NULL, TRUE);
        }
    }

    // Stop any activities that are scheduled to do so but have been
    // waiting for the next one to start.
    List<AutoPtr<ActivityRecord> >::Iterator it;
    if (stops != NULL) {
        for (it = stops->Begin(); it != stops->End(); ++it) {
            AutoPtr<ActivityRecord> r = *it;
            {
                AutoLock lock(mService->mLock);
                if (r->mFinishing) {
                    AutoPtr<ActivityRecord> outR;
                    FinishCurrentActivityLocked(r, FINISH_IMMEDIATELY, FALSE, (ActivityRecord**)&outR);
                }
                else {
                    StopActivityLocked(r);
                }
            }
        }
    }

    // Finish any activities that are scheduled to do so but have been
    // waiting for the next one to start.
    if (finishes != NULL) {
        for(it = finishes->Begin(); it != finishes->End(); ++it) {
            AutoPtr<ActivityRecord> r = *it;
            {
                AutoLock lock(mService->mLock);
                activityRemoved = DestroyActivityLocked(r, TRUE, FALSE, String("finish-idle"));
            }
        }
    }

    // Report back to any thumbnail receivers.
    if (thumbnails != NULL) {
        for(it = thumbnails->Begin(); it != thumbnails->End(); ++it) {
            AutoPtr<ActivityRecord> r = *it;
            mService->SendPendingThumbnail(r, NULL, NULL, NULL, TRUE);
        }
    }

    if (booting) {
        mService->FinishBooting();
    }
    else if (startingUsers != NULL) {
        List<AutoPtr<UserStartedState> >::Iterator it;
        for (it = startingUsers->Begin(); it != startingUsers->End(); ++it) {
            mService->FinishUserSwitch(*it);
        }
    }

    mService->TrimApplications();
    //dump();
    //mWindowManager.dump();

    if (enableScreen) {
        mService->EnableScreenAfterBoot();
    }

    if (activityRemoved) {
        ResumeTopActivityLocked(NULL);
    }

    return res;
}

Boolean ActivityStack::RequestFinishActivityLocked(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData,
    /* [in] */ const String& reason,
    /* [in] */ Boolean oomAdj)
{
    Int32 index = GetIndexOfTokenLocked(token);
    if (DEBUG_RESULTS) {
        // String tkDes, intDes;
        // token->GetDescription(&tkDes);
        // resultData->GetDescription(&intDes);
        // Slogger::V(TAG, StringBuffer("Finishing activity @") + index + ": token=" + tkDes
        //         + ", result=" + resultCode + ", data=" + intDes
        //         + ", reason=" + reason);
    }
    if (index < 0) {
        return FALSE;
    }
    AutoPtr<ActivityRecord> r = mHistory[index];

    return FinishActivityLocked(r, index, resultCode, resultData, reason, oomAdj);
}

ECode ActivityStack::FinishSubActivityLocked(
    /* [in] */ IBinder* token,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode)
{
    AutoPtr<ActivityRecord> self = IsInStackLocked(token);
    if (self == NULL) {
        return NOERROR;
    }

    Int32 i;
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(), i = mHistory.GetSize() - 1; rit != mHistory.REnd(); ++rit, --i) {
        AutoPtr<ActivityRecord> r = *rit;
        if (r->mResultTo == self && r->mRequestCode == requestCode) {
            if ((r->mResultWho.IsNull() && resultWho.IsNull()) ||
                (!r->mResultWho.IsNull() && r->mResultWho.Equals(resultWho))) {
                FinishActivityLocked(r, i,
                        IActivity::RESULT_CANCELED, NULL, String("request-sub"), FALSE);
            }
        }
    }
    mService->UpdateOomAdjLocked();
    return NOERROR;
}

Boolean ActivityStack::FinishActivityAffinityLocked(
    /* [in] */ IBinder* token)
{
    Int32 index = GetIndexOfTokenLocked(token);
    if (DEBUG_RESULTS) Slogger::V(
            TAG, "Finishing activity affinity @%d: token=%p", index, token);
    if (index < 0) {
        return FALSE;
    }
    AutoPtr<ActivityRecord> r = mHistory[index];

    while (index >= 0) {
        AutoPtr<ActivityRecord> cur = mHistory[index];
        if (cur->mTask != r->mTask) {
            break;
        }
        if (cur->mTaskAffinity == NULL && r->mTaskAffinity != NULL) {
            break;
        }
        if (cur->mTaskAffinity != NULL && !cur->mTaskAffinity.Equals(r->mTaskAffinity)) {
            break;
        }
        FinishActivityLocked(cur, index, IActivity::RESULT_CANCELED, NULL,
                String("request-affinity"), TRUE);
        index--;
    }
    return TRUE;
}

ECode ActivityStack::FinishActivityResultsLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData)
{
    // send the result
    AutoPtr<ActivityRecord> resultTo = r->mResultTo;
    if (resultTo != NULL) {
        if (DEBUG_RESULTS) Slogger::V(TAG, "Adding result to %p who=%s req=%d res=%d data=%p", resultTo.Get(),
                r->mResultWho.string(), r->mRequestCode,
                resultCode, resultData);
        AutoPtr<IApplicationInfo> appInfo;
        r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 uid = 0;
        appInfo->GetUid(&uid);
        if (uid > 0) {
            AutoPtr<IApplicationInfo> appInfo;
            r->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            mService->GrantUriPermissionFromIntentLocked(uid,
                    resultTo->mPackageName, resultData,
                    resultTo->GetUriPermissionsLocked());
        }
        resultTo->AddResultLocked(r, r->mResultWho, r->mRequestCode, resultCode, resultData);
        r->mResultTo = NULL;
    }
    else if (DEBUG_RESULTS) Slogger::V(TAG, "No result destination from %p", r);

    // Make sure this HistoryRecord is not holding on to other resources,
    // because clients have remote IPC references to this object so we
    // can't assume that will go away and want to avoid circular IPC refs.
    r->mResults = NULL;
    r->mPendingResults = NULL;
    r->mNewIntents = NULL;
    r->mIcicle = NULL;
    return NOERROR;
}

Boolean ActivityStack::FinishActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 index,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData,
    /* [in] */ const String& reason,
    /* [in] */ Boolean oomAdj)
{
    return FinishActivityLocked(r, index, resultCode, resultData, reason, FALSE, oomAdj);
}

Boolean ActivityStack::FinishActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 index,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData,
    /* [in] */ const String& reason,
    /* [in] */ Boolean immediate,
    /* [in] */ Boolean oomAdj)
{
    if (r->mFinishing) {
        String arDes;
        r->ToString(&arDes);
        Slogger::W(TAG, "Duplicate finish request for %s", arDes.string());
        return TRUE;
    }

    r->MakeFinishing();
//    EventLog.writeEvent(IEventLogTags.AM_FINISH_ACTIVITY,
//            r.userId, System.identityHashCode(r),
//            r.task.taskId, r.shortComponentName, reason);
    if (index < (Int32)(mHistory.GetSize() - 1)) {
        AutoPtr<ActivityRecord> next = (ActivityRecord*)mHistory[index + 1];
        if (next->mTask == r->mTask) {
            if (r->mFrontOfTask) {
                // The next activity is now the front of the task.
                next->mFrontOfTask = TRUE;
            }
            Int32 flags;
            r->mIntent->GetFlags(&flags);
            if ((flags & IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0) {
                // If the caller asked that this activity (and all above it)
                // be cleared when the task is reset, don't lose that information,
                // but propagate it up to the next activity.
                next->mIntent->AddFlags(IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
            }
        }
    }

    r->PauseKeyDispatchingLocked();
    if (mMainStack) {
        if ((ActivityRecord*)mService->mFocusedActivity == r) {
            mService->SetFocusedActivityLocked(GetTopRunningActivityLocked(NULL));
        }
    }

    FinishActivityResultsLocked(r, resultCode, resultData);

    if (mService->mPendingThumbnails.IsEmpty() == FALSE) {
        // There are clients waiting to receive thumbnails so, in case
        // this is an activity that someone is waiting for, add it
        // to the pending list so we can correctly update the clients.
        mService->mCancelledThumbnails.PushBack(r);
    }

    if (immediate) {
        AutoPtr<ActivityRecord> ar;
        FinishCurrentActivityLocked(r, index, FINISH_IMMEDIATELY, oomAdj, (ActivityRecord**)&ar);
        return (ar == NULL);
    } else if ((ActivityRecord*)mResumedActivity == r) {
        Boolean endTask = index <= 0
                || ((ActivityRecord*)mHistory[index - 1])->mTask != r->mTask;
        if (DEBUG_TRANSITION) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Prepare close transition: finishing %s", arDes.string());
        }

        Int32 activityClose = IWindowManagerPolicy::TRANSIT_ACTIVITY_CLOSE;
        mService->mWindowManager->PrepareAppTransition(endTask
                ? IWindowManagerPolicy::TRANSIT_TASK_CLOSE
                : activityClose, FALSE);

        // Tell window manager to prepare for this one to be removed.
        mService->mWindowManager->SetAppVisibility(IBinder::Probe(r->mAppToken), FALSE);

        if (mPausingActivity == NULL) {
            if (DEBUG_PAUSE) {
                String arDes;
                r->ToString(&arDes);
                Slogger::V(TAG, "Finish needs to pause: %s", arDes.string());
            }
            if (DEBUG_USER_LEAVING) {
                Slogger::V(TAG, "finish() => pause with userLeaving=false");
            }
            StartPausingLocked(FALSE, FALSE);
        }

    } else if (r->mState != ActivityState_PAUSING) {
        // If the activity is PAUSING, we will complete the finish once
        // it is done pausing; else we can just directly finish it here.
        if (DEBUG_PAUSE) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Finish not pausing: %s", arDes.string());
        }
        AutoPtr<ActivityRecord> fr;
        FinishCurrentActivityLocked(r, index,
                FINISH_AFTER_PAUSE, oomAdj, (ActivityRecord**)&fr);
        return fr == NULL;
    } else {
        if (DEBUG_PAUSE) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Finish waiting for pause of: %s", arDes.string());
        }
    }

    return FALSE;
}

ECode ActivityStack::FinishCurrentActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 mode,
    /* [in] */ Boolean oomAdj,
    /* [out] */ ActivityRecord** fr)
{
    VALIDATE_NOT_NULL(fr);
    *fr = NULL;

    Int32 index = GetIndexOfActivityLocked(r);
    if (index < 0) {
        return NOERROR;
    }

    return FinishCurrentActivityLocked(r, index, mode, oomAdj, fr);
}

ECode ActivityStack::FinishCurrentActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 index,
    /* [in] */ Int32 mode,
    /* [in] */ Boolean oomAdj,
    /* [out] */ ActivityRecord** fr)
{
    VALIDATE_NOT_NULL(fr);
    *fr = NULL;

    // First things first: if this activity is currently visible,
    // and the resumed activity is not yet visible, then hold off on
    // finishing until the resumed one becomes visible.
    if (mode == FINISH_AFTER_VISIBLE && r->mNowVisible) {
        if (Find(mStoppingActivities.Begin(), mStoppingActivities.End(),
                AutoPtr<ActivityRecord>(r)) == mStoppingActivities.End()) {
            mStoppingActivities.PushBack(r);
            if (mStoppingActivities.GetSize() > 3) {
                // If we already have a few activities waiting to stop,
                // then give up on things going idle and start clearing
                // them out.
                ScheduleIdleLocked();
            }
            else {
                CheckReadyForSleepLocked();
            }
        }
        if (DEBUG_STATES) Slogger::V(TAG, "Moving to STOPPING: %p (finish requested)", r);
        r->mState = ActivityState_STOPPING;
        if (oomAdj) {
            mService->UpdateOomAdjLocked();
        }
        *fr = r;
        REFCOUNT_ADD(*fr);
        return NOERROR;
    }

    // make sure the record is cleaned out of other places.
    mStoppingActivities.Remove(r);
    mGoingToSleepActivities.Remove(r);
    mWaitingVisibleActivities.Remove(r);
    if (mResumedActivity.Get() == r) {
        mResumedActivity = NULL;
    }
    ActivityState prevState = r->mState;
    if (DEBUG_STATES) Slogger::V(TAG, "Moving to FINISHING: %p", r);
    r->mState = ActivityState_FINISHING;

    if (mode == FINISH_IMMEDIATELY
            || prevState == ActivityState_STOPPED
            || prevState == ActivityState_INITIALIZING) {
        // If this activity is already stopped, we can just finish
        // it right now.
        Boolean activityRemoved = DestroyActivityLocked(r, TRUE, oomAdj, String("finish-imm"));
        if (activityRemoved) {
            ResumeTopActivityLocked(NULL);
        }
        *fr = activityRemoved ? NULL : r;
        REFCOUNT_ADD(*fr);
        return NOERROR;
    }
    else {
        // Need to go through the full pause cycle to get this
        // activity into the stopped state and then finish it.
        if (localLOGV) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Enqueueing pending finish: %s", arDes.string());
        }
        mFinishingActivities.PushBack(r);
        ResumeTopActivityLocked(NULL);
    }
    *fr = r;
    REFCOUNT_ADD(*fr);
    return NOERROR;
}

ECode ActivityStack::CleanUpActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Boolean cleanServices,
    /* [in] */ Boolean setState)
{
    if ((ActivityRecord*)mResumedActivity == r) {
        mResumedActivity = NULL;
    }
    if (mService->mFocusedActivity.Get() == r) {
        mService->mFocusedActivity = NULL;
    }

    r->mConfigDestroy = FALSE;
    r->mFrozenBeforeDestroy = FALSE;

    if (setState) {
        if (DEBUG_STATES) {
            String info;
            r->ToString(&info);
            Slogger::V(TAG, "Moving to DESTROYED: %s (cleaning up)", info.string());
        }
        r->mState = ActivityState_DESTROYED;
        r->mApp = NULL;
    }

    // Make sure this record is no longer in the pending finishes list.
    // This could happen, for example, if we are trimming activities
    // down to the max limit while they are still waiting to finish.
    mFinishingActivities.Remove(r);
    mWaitingVisibleActivities.Remove(r);

    // Remove any pending results.
    if (r->mFinishing && r->mPendingResults != NULL) {
        HashSet< AutoPtr<IWeakReference> >::Iterator it = r->mPendingResults->Begin();
        for (; it != r->mPendingResults->End(); ++it) {
            AutoPtr<IWeakReference> wr = *it;
            if (wr) {
                AutoPtr<IIIntentSender> obj;
                wr->Resolve(EIID_IIIntentSender, (IInterface**)&obj);
                if (obj) {
                    mService->CancelIntentSenderLocked(obj, FALSE);
                }
            }
        }
        r->mPendingResults->Clear();
        r->mPendingResults = NULL;
    }

    if (cleanServices) {
        CleanUpActivityServicesLocked(r);
    }

    if (mService->mPendingThumbnails.IsEmpty() == FALSE) {
        // There are clients waiting to receive thumbnails so, in case
        // this is an activity that someone is waiting for, add it
        // to the pending list so we can correctly update the clients.
        mService->mCancelledThumbnails.PushBack(r);
    }

    // Get rid of any pending idle timeouts.
    RemoveTimeoutsForActivityLocked(r);
    return NOERROR;
}

ECode ActivityStack::RemoveTimeoutsForActivityLocked(
    /* [in] */ ActivityRecord* r)
{
    mHandler->RemoveMessages(PAUSE_TIMEOUT_MSG, r->Probe(EIID_IInterface));
    mHandler->RemoveMessages(STOP_TIMEOUT_MSG, r->Probe(EIID_IInterface));
    mHandler->RemoveMessages(IDLE_TIMEOUT_MSG, r->Probe(EIID_IInterface));
    mHandler->RemoveMessages(DESTROY_TIMEOUT_MSG, r->Probe(EIID_IInterface));
    r->FinishLaunchTickingLocked();
    return NOERROR;
}

ECode ActivityStack::RemoveActivityFromHistoryLocked(
    /* [in] */ ActivityRecord* r)
{
    FinishActivityResultsLocked(r, IActivity::RESULT_CANCELED, NULL);
    r->MakeFinishing();
    if (DEBUG_ADD_REMOVE) {
       // RuntimeException here = new RuntimeException("here");
       // here.fillInStackTrace();
        String info;
        r->ToString(&info);
        Slogger::I(TAG, "Removing activity %s from stack", info.string());
    }
    List< AutoPtr<ActivityRecord> >::Iterator it;
    it = Find(mHistory.Begin(), mHistory.End(), AutoPtr<ActivityRecord>(r));
    if (it != mHistory.End()) mHistory.Erase(it);
    r->TakeFromHistory();
    RemoveTimeoutsForActivityLocked(r);
    r->mState = ActivityState_DESTROYED;
    r->mApp = NULL;
    mService->mWindowManager->RemoveAppToken(IBinder::Probe(r->mAppToken));
    if (VALIDATE_TOKENS) {
        ValidateAppTokensLocked();
    }
    CleanUpActivityServicesLocked(r);
    r->RemoveUriPermissionsLocked();

    if (DEBUG_STATES) {
        String info;
        r->ToString(&info);
        Slogger::V(TAG, "Moving to DESTROYED: %s, ref count: %d (removed from history)", info.string(), r->GetStrongCount());
    }
    return NOERROR;
}

ECode ActivityStack::CleanUpActivityServicesLocked(
    /* [in] */ ActivityRecord* r)
{
    // Throw away any services that have been bound by this activity.
    if (r->mConnections != NULL) {
        HashSet<AutoPtr<ConnectionRecord> >::Iterator it = r->mConnections->Begin();
        for (; it != r->mConnections->End(); ++it) {
            AutoPtr<ConnectionRecord> c = *it;
            mService->mServices->RemoveConnectionLocked(c, NULL, r);
        }
        r->mConnections->Clear();
        r->mConnections = NULL;
    }
    return NOERROR;
}

ECode ActivityStack::ScheduleDestroyActivities(
    /* [in] */ ProcessRecord* owner,
    /* [in] */ Boolean oomAdj,
    /* [in] */ const String& reason)
{
    AutoPtr<ScheduleDestroyArgs> args = new ScheduleDestroyArgs(owner, oomAdj, reason);
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(DESTROY_ACTIVITIES_MSG, (IMessage**)&msg);
    msg->SetObj((IInterface*)args.Get());
    Boolean result;
    return mHandler->SendMessage(msg, &result);
}

ECode ActivityStack::DestroyActivitiesLocked(
    /* [in] */ ProcessRecord* owner,
    /* [in] */ Boolean oomAdj,
    /* [in] */ const String& reason)
{
    Boolean lastIsOpaque = FALSE;
    Boolean activityRemoved = FALSE;
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        AutoPtr<ActivityRecord> r = *rit;
        if (r->mFinishing) {
            continue;
        }
        if (r->mFullscreen) {
            lastIsOpaque = TRUE;
        }
        if (owner != NULL && r->mApp.Get() != owner) {
            continue;
        }
        if (!lastIsOpaque) {
            continue;
        }
        // We can destroy this one if we have its icicle saved and
        // it is not in the process of pausing/stopping/finishing.
        if (r->mApp != NULL && r != mResumedActivity && r != mPausingActivity
                && r->mHaveState && !r->mVisible && r->mStopped
                && r->mState != ActivityState_DESTROYING
                && r->mState != ActivityState_DESTROYED) {
            if (DEBUG_SWITCH) {
                String info, resumed, pausing;
                r->ToString(&info);
                if (mResumedActivity) mResumedActivity->ToString(&resumed);
                if (mPausingActivity) mPausingActivity->ToString(&pausing);
                Slogger::V(TAG, "Destroying %s in state %d resumed=%s pausing=%s",
                    info.string(), r->mState, resumed.string(), pausing.string());
            }
            if (DestroyActivityLocked(r, TRUE, oomAdj, reason)) {
                activityRemoved = TRUE;
            }
        }
    }
    if (activityRemoved) {
        ResumeTopActivityLocked(NULL);
    }
    return NOERROR;
}

Boolean ActivityStack::DestroyActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Boolean removeFromApp,
    /* [in] */ Boolean oomAdj,
    /* [in] */ const String& reason)
{
    if (DEBUG_SWITCH || DEBUG_CLEANUP) {
        String arDes;
        r->ToString(&arDes);
        Slogger::V(TAG, "Removing activity: from %s : token=%s, app=%s", reason.string(), arDes.string(),
            (r->mApp != NULL ? r->mApp->mProcessName.string() : String("(NULL)")));
    }
   // EventLog.writeEvent(IEventLogTags.AM_DESTROY_ACTIVITY,
   //         r.userId, System.identityHashCode(r),
   //         r.task.taskId, r.shortComponentName, reason);

    Boolean removedFromHistory = FALSE;

    CleanUpActivityLocked(r, FALSE, FALSE);

    Boolean hadApp = r->mApp != NULL;

    if (hadApp) {
        if (removeFromApp) {
            r->mApp->mActivities.Remove(r);
            if (mService->mHeavyWeightProcess.Get() == r->mApp &&
                    r->mApp->mActivities.IsEmpty()) {
                mService->mHeavyWeightProcess = NULL;
                Boolean result;
                mService->mHandler->SendEmptyMessage(
                    CActivityManagerService::CANCEL_HEAVY_NOTIFICATION_MSG, &result);
            }
            if (r->mApp->mActivities.IsEmpty()) {
                // No longer have activities, so update oom adj.
                mService->UpdateOomAdjLocked();
            }
        }

        Boolean skipDestroy = FALSE;

        if (DEBUG_SWITCH) {
            String arDes;
            r->ToString(&arDes);
            Slogger::I(TAG, "Destroying: %s", arDes.string());
        }
        if (FAILED(r->mApp->mThread->ScheduleDestroyActivity(IBinder::Probe(r->mAppToken),
                r->mFinishing, r->mConfigChangeFlags))) {
            // We can just ignore exceptions here...  if the process
            // has crashed, our death notification will clean things
            // up.
            Slogger::W(TAG, String("Exception thrown during finish"));
            if (r->mFinishing) {
                RemoveActivityFromHistoryLocked(r);
                removedFromHistory = TRUE;
                skipDestroy = TRUE;
            }
        }

        r->mNowVisible = FALSE;

        // If the activity is finishing, we need to wait on removing it
        // from the list to give it a chance to do its cleanup.  During
        // that time it may make calls back with its token so we need to
        // be able to find it on the list and so we don't want to remove
        // it from the list yet.  Otherwise, we can just immediately put
        // it in the destroyed state since we are not removing it from the
        // list.
        if (r->mFinishing && !skipDestroy) {
            if (DEBUG_STATES) {
                String info;
                r->ToString(&info);
                Slogger::V(TAG, "Moving to DESTROYING: %s (destroy requested)", info.string());
            }
            r->mState = ActivityState_DESTROYING;

            AutoPtr<IMessage> msg;
            mHandler->ObtainMessage(DESTROY_TIMEOUT_MSG, (IMessage**)&msg);
            msg->SetObj(r->Probe(EIID_IInterface));
            Boolean result;
            mHandler->SendMessageDelayed(msg, DESTROY_TIMEOUT, &result);
        }
        else {
            if (DEBUG_STATES) {
                String info;
                r->ToString(&info);
                Slogger::V(TAG, "Moving to DESTROYED: %s (destroy skipped)", info.string());
            }
            r->mState = ActivityState_DESTROYED;
            r->mApp = NULL;
        }
    } else {
        // remove this record from the history.
        if (r->mFinishing) {
            RemoveActivityFromHistoryLocked(r);
            removedFromHistory = TRUE;
        } else {
            if (DEBUG_STATES) {
                String info;
                r->ToString(&info);
                Slogger::V(TAG, "Moving to DESTROYED: %s (no app)", info.string());
            }
            r->mState = ActivityState_DESTROYED;
            r->mApp = NULL;
        }
    }

    r->mConfigChangeFlags = 0;

    List< AutoPtr<ActivityRecord> >::Iterator it;
    it = Find(mLRUActivities.Begin(), mLRUActivities.End(), AutoPtr<ActivityRecord>(r));
    Boolean found = FALSE;
    if (it != mLRUActivities.End()) {
        mLRUActivities.Erase(it);
        found = TRUE;
    }
    if (!found && hadApp) {
        String arDes;
        r->ToString(&arDes);
        Slogger::W(TAG, "Activity %s being finished, but not in LRU list", arDes.string());
    }

    return removedFromHistory;
}

ECode ActivityStack::ActivityDestroyed(
    /* [in] */ IBinder* token)
{
    AutoLock lock(mService->mLock);
    const Int64 origId = Binder::ClearCallingIdentity();
   // try {
    AutoPtr<ActivityRecord> r;
    ActivityRecord::ForToken(token, (ActivityRecord**)&r);
    if (r != NULL) {
         mHandler->RemoveMessages(DESTROY_TIMEOUT_MSG, r->Probe(EIID_IInterface));
    }

    Int32 index = GetIndexOfActivityLocked(r);
    if (index >= 0) {
        if (r->mState == ActivityState_DESTROYING) {
            CleanUpActivityLocked(r, TRUE, FALSE);
            RemoveActivityFromHistoryLocked(r);
        }
    }
    ResumeTopActivityLocked(NULL);
   // } finally {
    Binder::RestoreCallingIdentity(origId);
   // }

    return NOERROR;
}

ECode ActivityStack::RemoveHistoryRecordsForAppLocked(
    /* [in] */ List<AutoPtr<ActivityRecord> > & list,
    /* [in] */ ProcessRecord* app,
    /* [in] */ const String& listName)
{
    if (DEBUG_CLEANUP) {
        String appDes = app->ToString();
        Slogger::V(TAG, "Removing app %s from list %s with %d entries", appDes.string(),
            listName.string(), list.GetSize());
    }

    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = list.RBegin();
    for (; rit != list.REnd();) {
        AutoPtr<ActivityRecord> r = *rit;
        if (DEBUG_CLEANUP) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "RemoveHistoryRecordsForAppLocked Record %s", arDes.string());
        }
        if (r->mApp.Get() == app) {
            if (DEBUG_CLEANUP) Slogger::V(TAG, "---> REMOVING this entry!");
            RemoveTimeoutsForActivityLocked(r);
            rit = List<AutoPtr<ActivityRecord> >::ReverseIterator(list.Erase(--(rit.GetBase())));
        }
        else {
            ++rit;
        }
    }
    return NOERROR;
}

Boolean ActivityStack::RemoveHistoryRecordsForAppLocked(
    /* [in] */ ProcessRecord* app)
{
    RemoveHistoryRecordsForAppLocked(mLRUActivities, app, String("mLRUActivities"));
    RemoveHistoryRecordsForAppLocked(mStoppingActivities, app, String("mStoppingActivities"));
    RemoveHistoryRecordsForAppLocked(mGoingToSleepActivities, app, String("mGoingToSleepActivities"));
    RemoveHistoryRecordsForAppLocked(mWaitingVisibleActivities, app, String("mWaitingVisibleActivities"));
    RemoveHistoryRecordsForAppLocked(mFinishingActivities, app, String("mFinishingActivities"));

    Boolean hasVisibleActivities = FALSE;

    // Clean out the history list.
    Int32 i = mHistory.GetSize();
    if (DEBUG_CLEANUP)
        Slogger::V(TAG, "Removing app %s from history with %d entries", app->ToString().string(), i);

    List<AutoPtr<ActivityRecord> >::ReverseIterator rit = mHistory.RBegin();
    while (rit != mHistory.REnd()) {
        i--;
        AutoPtr<ActivityRecord> r = *rit;
        if (DEBUG_CLEANUP)
            Slogger::V(TAG, "Record #%d %p: app=%p", i, r.Get(), r->mApp.Get());

        Boolean addIterator = TRUE;
        if (r->mApp.Get() == app) {
            if ((!r->mHaveState && !r->mStateNotNeeded) || r->mFinishing) {
                if (DEBUG_ADD_REMOVE || DEBUG_CLEANUP) {
                    // RuntimeException here = new RuntimeException("here");
                    // here.fillInStackTrace();
                    String info;
                    r->ToString(&info);
                    Slogger::I(TAG, "Removing activity %s from stack at %d"": haveState="
                        " stateNotNeeded=%d finishing=%d state=%d", info.string(), r->mHaveState,
                        r->mStateNotNeeded, r->mFinishing, r->mState);
                }
                if (!r->mFinishing) {
                    String info;
                    r->ToString(&info);
                    Slogger::W(TAG, "Force removing %s: app died, no saved state", info.string());
                    // EventLog.writeEvent(IEventLogTags.AM_FINISH_ACTIVITY,
                    //         r->mUserId, System.identityHashCode(r),
                    //         r->mTask.taskId, r->mShortComponentName,
                    //         "proc died without state saved");
                }
                RemoveActivityFromHistoryLocked(r);
                addIterator = FALSE;
            }
            else {
                // We have the current state for this activity, so
                // it can be restarted later when needed.
                if (localLOGV)
                    Slogger::V(TAG, "Keeping entry, setting app to NULL");
                if (r->mVisible) {
                    hasVisibleActivities = TRUE;
                }
                r->mApp = NULL;
                r->mNowVisible = FALSE;
                if (!r->mHaveState) {
                    if (DEBUG_SAVED_STATE)
                        Slogger::I(TAG, "App died, clearing saved state of %p", r.Get());
                    r->mIcicle = NULL;
                }
            }

            r->mStack->CleanUpActivityLocked(r, TRUE, TRUE);
        }

        if (addIterator)
            ++rit;
    }

    return hasVisibleActivities;
}

ECode ActivityStack::MoveHomeToFrontLocked()
{
    AutoPtr<TaskRecord> homeTask;
    List< AutoPtr<ActivityRecord> >::ReverseIterator rit;
    for (rit = mHistory.RBegin(); rit != mHistory.REnd(); ++rit) {
        AutoPtr<ActivityRecord> hr = *rit;
        if (hr->mIsHomeActivity) {
            homeTask = hr->mTask;
            break;
        }
    }
    if (homeTask != NULL) {
        return MoveTaskToFrontLocked(homeTask, NULL, NULL);
    }
    return NOERROR;
}

ECode ActivityStack::UpdateTransitLocked(
    /* [in] */ Int32 transit,
    /* [in] */ IBundle* options)
{
    if (options != NULL) {
        AutoPtr<ActivityRecord> r = GetTopRunningActivityLocked(NULL);
        if (r != NULL && r->mState != ActivityState_RESUMED) {
            r->UpdateOptionsLocked(options);
        } else {
            AutoPtr<IActivityOptionsHelper> helper;
            CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
            helper->Abort(options);
        }
    }

    return mService->mWindowManager->PrepareAppTransition(transit, false);
}

ECode ActivityStack::MoveTaskToFrontLocked(
    /* [in] */ TaskRecord* tr,
    /* [in] */ ActivityRecord* reason,
    /* [in] */ IBundle* options)
{
    if (DEBUG_SWITCH) {
        String trDes = tr->ToString();
        Slogger::V(TAG, "moveTaskToFront: %s", trDes.string());
    }

    Int32 task = tr->mTaskId;
    Int32 top = mHistory.GetSize() - 1;

    if (top < 0 || mHistory[top]->mTask->mTaskId == task) {
        // nothing to do!
        Int32 rFlags;
        reason->mIntent->GetFlags(&rFlags);
        if (reason != NULL &&
                (rFlags & IIntent::FLAG_ACTIVITY_NO_ANIMATION) != 0) {
            AutoPtr<IActivityOptionsHelper> helper;
            CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
            helper->Abort(options);
        } else {
            UpdateTransitLocked(IWindowManagerPolicy::TRANSIT_TASK_TO_FRONT, options);
        }
        return NOERROR;
    }

    AutoPtr<IObjectContainer> moved;
    CParcelableObjectContainer::New((IObjectContainer**)&moved);

    // Applying the affinities may have removed entries from the history,
    // so get the size again.
    top = mHistory.GetSize() - 1;
    Int32 pos = top;

    // Shift all activities with this task up to the top
    // of the stack, keeping them in the same internal order.
    while (pos >= 0) {
        AutoPtr<ActivityRecord> r = mHistory[pos];
        if (localLOGV) {
            String trDes, arDes;
            trDes = r->mTask->ToString();
            r->ToString(&arDes);
            Slogger::V(TAG, "At %d ckp %s: %s", pos, trDes.string(), arDes.string());
        }
        if (r->mTask->mTaskId == task) {
            if (localLOGV) {
                Slogger::V(TAG, "Removing and adding at %d", top);
            }
            if (DEBUG_ADD_REMOVE) {
                // RuntimeException here = new RuntimeException("here");
                // here.fillInStackTrace();
                String info;
                r->ToString(&info);
                Slogger::I(TAG, "Removing and adding activity %s to stack at %d", info.string(), top);
            }
            List<AutoPtr<ActivityRecord> >::Iterator it1 = mHistory.Begin();
            for (Int32 idx = 0; idx < pos; ++idx) ++it1;
            mHistory.Erase(it1);
            it1 = mHistory.Begin();
            for (Int32 idx = 0; idx < top; ++idx) ++it1;
            mHistory.Insert(it1, r);
            IInterface* obj = r->mAppToken->Probe(EIID_IInterface);
            moved->Add(obj);
            top--;
        }
        pos--;
    }

    if (DEBUG_TRANSITION) {
        String trDes = tr->ToString();
        Slogger::V(TAG, "Prepare to front transition: task=%s", trDes.string());
    }

    Int32 flags;
    if (reason != NULL && ((reason->mIntent->GetFlags(&flags),
        flags) & IIntent::FLAG_ACTIVITY_NO_ANIMATION) != 0) {
        mService->mWindowManager->PrepareAppTransition(IWindowManagerPolicy::TRANSIT_NONE, FALSE);
        AutoPtr<ActivityRecord> r = GetTopRunningActivityLocked(NULL);
        if (r != NULL) {
            mNoAnimActivities.PushBack(r);
        }
        AutoPtr<IActivityOptionsHelper> helper;
        CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
        helper->Abort(options);
    }
    else {
        UpdateTransitLocked(IWindowManagerPolicy::TRANSIT_TASK_TO_FRONT, options);
    }

    mService->mWindowManager->MoveAppTokensToTop(moved.Get());
    if (VALIDATE_TOKENS) {
        ValidateAppTokensLocked();
    }

    return FinishTaskMoveLocked(task);
   // EventLog.writeEvent(IEventLogTags.AM_TASK_TO_FRONT, tr.userId, task);
}

ECode ActivityStack::FinishTaskMoveLocked(
    /* [in] */ Int32 task)
{
    ResumeTopActivityLocked(NULL);
    return NOERROR;
}

Boolean ActivityStack::MoveTaskToBackLocked(
    /* [in] */ Int32 task,
    /* [in] */ ActivityRecord* reason)
{
    Slogger::I(TAG, "moveTaskToBack: %d", task);

    // If we have a watcher, preflight the move before committing to it.  First check
    // for *other* available tasks, but if none are available, then try again allowing the
    // current task to be selected.
    if (mMainStack && mService->mController != NULL) {
        AutoPtr<ActivityRecord> next = GetTopRunningActivityLocked(NULL, task);
        if (next == NULL) {
            next = GetTopRunningActivityLocked(NULL, 0);
        }
        if (next != NULL) {
            // ask watcher if this is allowed
            Boolean moveOK = TRUE;
           // try {
                mService->mController->ActivityResuming(next->mPackageName,&moveOK);
           // } catch (RemoteException e) {
           //     mService.mController = NULL;
           // }
            if (!moveOK) {
                mService->mController = NULL;
                return FALSE;
            }
        }
    }

    AutoPtr<IObjectContainer> moved;
    CParcelableObjectContainer::New((IObjectContainer**)&moved);

    if (DEBUG_TRANSITION)
        Slogger::V(TAG, "Prepare to back transition: task=%d", task);

    const Int32 N = mHistory.GetSize();
    Int32 bottom = 0;
    Int32 pos = 0;

    // Shift all activities with this task down to the bottom
    // of the stack, keeping them in the same internal order.
    while (pos < N) {
        AutoPtr<ActivityRecord> r = mHistory[pos];
        if (localLOGV) {
            // Slogger::V(
            // TAG, StringBuffer("At ") + pos + StringBuffer(" ckp ") + r->mTask + StringBuffer(": ") + r);
        }
        if (r->mTask->mTaskId == task) {
            // if (localLOGV) Slogger::V(TAG, StringBuffer("Removing and adding at ") + (N-1));
            if (DEBUG_ADD_REMOVE) {
               // RuntimeException here = new RuntimeException("here");
               // here.fillInStackTrace();
               // Slogger::I(TAG, "Removing and adding activity " + r + " to stack at "
               //         + bottom, here);
            }
            mHistory.Remove(pos);
            mHistory.Insert(bottom, r);
            moved->Add(IBinder::Probe(r->mAppToken));
            bottom++;
        }
        pos++;
    }

    Int32 gFlags;
    if (reason != NULL &&
            (reason->mIntent->GetFlags(&gFlags), (gFlags & IIntent::FLAG_ACTIVITY_NO_ANIMATION) != 0)) {
        mService->mWindowManager->PrepareAppTransition(IWindowManagerPolicy::TRANSIT_NONE, FALSE);
        AutoPtr<ActivityRecord> r = GetTopRunningActivityLocked(NULL);
        if (r != NULL) {
            mNoAnimActivities.PushBack(r);
        }
    } else {
        mService->mWindowManager->PrepareAppTransition(IWindowManagerPolicy::TRANSIT_TASK_TO_BACK, FALSE);
    }
    mService->mWindowManager->MoveAppTokensToBottom(moved);
    if (VALIDATE_TOKENS) {
        ValidateAppTokensLocked();
    }

    FinishTaskMoveLocked(task);
    return TRUE;
}

AutoPtr<IActivityManagerTaskThumbnails> ActivityStack::GetTaskThumbnailsLocked(
    /* [in] */ TaskRecord* tr)
{
    AutoPtr<TaskAccessInfo> info = GetTaskAccessInfoLocked(tr->mTaskId, TRUE);
    AutoPtr<ActivityRecord> resumed = mResumedActivity;
    if (resumed != NULL && resumed->mThumbHolder == tr) {
        info->mMainThumbnail = resumed->mStack->ScreenshotActivities(resumed);
    }
    if (info->mMainThumbnail == NULL) {
        info->mMainThumbnail = tr->mLastThumbnail;
    }
    return (IActivityManagerTaskThumbnails*)info->Probe(EIID_IActivityManagerTaskThumbnails);
}

AutoPtr<IBitmap> ActivityStack::GetTaskTopThumbnailLocked(
    /* [in] */ TaskRecord* tr)
{
    AutoPtr<ActivityRecord> resumed = mResumedActivity;
    if (resumed != NULL && resumed->mTask.Get() == tr) {
        // This task is the current resumed task, we just need to take
        // a screenshot of it and return that.
        return resumed->mStack->ScreenshotActivities(resumed);
    }
    // Return the information about the task, to figure out the top
    // thumbnail to return.
    AutoPtr<TaskAccessInfo> info = GetTaskAccessInfoLocked(tr->mTaskId, TRUE);
    Int32 numSubThumbbails;
    info->GetNumSubThumbbails(&numSubThumbbails);
    AutoPtr<IBitmap> mainThumbnail;
    info->GetMainThumbnail((IBitmap**)&mainThumbnail);
    if (numSubThumbbails <= 0) {
        return mainThumbnail != NULL ? mainThumbnail : tr->mLastThumbnail;
    } else {
        return info->mSubtasks[numSubThumbbails-1]->mHolder->mLastThumbnail;
    }
}

AutoPtr<ActivityRecord> ActivityStack::RemoveTaskActivitiesLocked(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 subTaskIndex,
    /* [in] */ Boolean taskRequired)
{
    AutoPtr<TaskAccessInfo> info = GetTaskAccessInfoLocked(taskId, FALSE);
    if (info->mRoot == NULL) {
        if (taskRequired) {
            Slogger::W(TAG, "removeTaskLocked: unknown taskId %d", taskId);
        }
        return NULL;
    }

    if (subTaskIndex < 0) {
        // Just remove the entire task.
        PerformClearTaskAtIndexLocked(taskId, info->mRootIndex);
        return info->mRoot;
    }

    if (subTaskIndex >= info->mSubtasks.GetSize()) {
        if (taskRequired) {
            Slogger::W(TAG, "removeTaskLocked: unknown subTaskIndex %d", subTaskIndex);
        }
        return NULL;
    }

    // Remove all of this task's activities starting at the sub task.
    AutoPtr<TaskAccessInfo::SubTask> subtask = info->mSubtasks[subTaskIndex];
    PerformClearTaskAtIndexLocked(taskId, subtask->mIndex);
    return subtask->mActivity;
}

AutoPtr<TaskAccessInfo> ActivityStack::GetTaskAccessInfoLocked(
    /* [in] */ Int32 taskId,
    /* [in] */ Boolean inclThumbs)
{
    AutoPtr<TaskAccessInfo> thumbs = new TaskAccessInfo();
    // How many different sub-thumbnails?
    Int32 NA = mHistory.GetSize();
    Int32 j = 0;
    AutoPtr<ThumbnailHolder> holder;
    while (j < NA) {
        AutoPtr<ActivityRecord> ar = mHistory[j];
        if (!ar->mFinishing && ar->mTask->mTaskId == taskId) {
            thumbs->mRoot = ar;
            thumbs->mRootIndex = j;
            holder = ar->mThumbHolder;
            if (holder != NULL) {
                thumbs->SetMainThumbnail(holder->mLastThumbnail);
            }
            j++;
            break;
        }
        j++;
    }

    if (j >= NA) {
        return thumbs;
    }

    List<AutoPtr<TaskAccessInfo::SubTask> > subtasks;
    thumbs->mSubtasks = subtasks;
    while (j < NA) {
        AutoPtr<ActivityRecord> ar = mHistory[j];
        j++;
        if (ar->mFinishing) {
            continue;
        }
        if (ar->mTask->mTaskId != taskId) {
            break;
        }
        if (ar->mThumbHolder != holder && holder != NULL) {
            Int32 num;
            thumbs->GetNumSubThumbbails(&num);
            thumbs->SetNumSubThumbbails(num + 1);
            holder = ar->mThumbHolder;
            AutoPtr<TaskAccessInfo::SubTask> sub = new TaskAccessInfo::SubTask();
            sub->mHolder = holder;
            sub->mActivity = ar;
            sub->mIndex = j-1;
            subtasks.PushBack(sub);
        }
    }
    Int32 numSubThumbbails;
    thumbs->GetNumSubThumbbails(&numSubThumbbails);
    if (numSubThumbbails > 0) {
        AutoPtr<ThumbnailRetriever> retriever = new ThumbnailRetriever(this, thumbs);
        thumbs->SetRetriever(retriever);
    }
    return thumbs;
}

ECode ActivityStack::LogStartActivity(
    /* [in] */ Int32 tag,
    /* [in] */ ActivityRecord* r,
    /* [in] */ TaskRecord* task)
{
   // EventLog.writeEvent(tag,
   //             r.userId, System.identityHashCode(r), task.taskId,
   //             r.shortComponentName, r.intent.getAction(),
   //             r.intent.getType(), r.intent.getDataString(),
   //             r.intent.getFlags());
    return E_NOT_IMPLEMENTED;
}

Boolean ActivityStack::EnsureActivityConfigurationLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 globalChanges)
{
    if (mConfigWillChange) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Skipping config check (will change): %s",
                    arDes.string());
        }
        return TRUE;
    }

    if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
        String arDes;
        r->ToString(&arDes);
        Slogger::V(TAG, "Ensuring correct configuration: %s", arDes.string());
    }

    // Short circuit: if the two configurations are the exact same
    // object (the common case), then there is nothing to do.
    AutoPtr<IConfiguration> newConfig = mService->mConfiguration;
    if (r->mConfiguration == newConfig && !r->mForceNewConfig) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Configuration unchanged in %s", arDes.string());
        }
        return TRUE;
    }

    // We don't worry about activities that are finishing.
    if (r->mFinishing) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Configuration doesn't matter in finishing %s",
                    arDes.string());
        }
        r->StopFreezingScreenLocked(FALSE);
        return TRUE;
    }

    // Okay we now are going to make this activity have the new config.
    // But then we need to figure out how it needs to deal with that.
    AutoPtr<IConfiguration> oldConfig = r->mConfiguration;
    r->mConfiguration = newConfig;

    // Determine what has changed.  May be nothing, if this is a config
    // that has come back from the app after going idle.  In that case
    // we just want to leave the official config object now in the
    // activity and do nothing else.
    Int32 changes;
    oldConfig->Diff(newConfig, &changes);
    if (changes == 0 && !r->mForceNewConfig) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) Slogger::V(TAG,
                "Configuration no differences in %p", r);
        return TRUE;
    }

    // If the activity isn't currently running, just leave the new
    // configuration and it will pick that up next time it starts.
    if (r->mApp == NULL || r->mApp->mThread == NULL) {
        if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Configuration doesn't matter not running %s",
                    arDes.string());
        }
        r->StopFreezingScreenLocked(FALSE);
        r->mForceNewConfig = FALSE;
        return TRUE;
    }

    // Figure out how to handle the changes between the configurations.
    oldConfig->Diff(newConfig.Get(), &changes);
    if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
        // String cfgDes, name;
        // Int32 confChanges;
        // newConfig->GetDescription(&cfgDes);
        // r->mInfo->GetName(&name);
        // r->mInfo->GetRealConfigChanged(&confChanges);
        // Slogger::V(TAG, StringBuffer("Checking to restart ")
        //         + name + ": changed="
        //         + changes + ", handles="
        //         + confChanges
        //         + ", newConfig=" + cfgDes);
    }
    Int32 confChanges;
    r->mInfo->GetRealConfigChanged(&confChanges);
    if ((changes & ~confChanges) != 0 || r->mForceNewConfig) {
        // Aha, the activity isn't handling the change, so DIE DIE DIE.
        r->mConfigChangeFlags |= changes;
        r->StartFreezingScreenLocked(r->mApp, globalChanges);
        r->mForceNewConfig = FALSE;
        if (r->mApp == NULL || r->mApp->mThread == NULL) {
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->ToString(&arDes);
                Slogger::V(TAG,"Config is destroying non-running %s", arDes.string());
            }
            DestroyActivityLocked(r, TRUE, FALSE, String("config"));
        } else if (r->mState == ActivityState_PAUSING) {
            // A little annoying: we are waiting for this activity to
            // finish pausing.  Let's not do anything now, but just
            // flag that it needs to be restarted when done pausing.
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->ToString(&arDes);
                Slogger::V(TAG, "Config is skipping already pausing %s", arDes.string());
            }
            r->mConfigDestroy = TRUE;
            return TRUE;
        } else if (r->mState == ActivityState_RESUMED) {
            // Try to optimize this case: the configuration is changing
            // and we need to restart the top, resumed activity.
            // Instead of doing the normal handshaking, just say
            // "restart!".
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->ToString(&arDes);
                Slogger::V(TAG, "Config is relaunching resumed %s", arDes.string());
            }
            RelaunchActivityLocked(r, r->mConfigChangeFlags, TRUE);
            r->mConfigChangeFlags = 0;
        } else {
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String arDes;
                r->ToString(&arDes);
                Slogger::V(TAG, "Config is relaunching non-resumed %s", arDes.string());
            }
            RelaunchActivityLocked(r, r->mConfigChangeFlags, FALSE);
            r->mConfigChangeFlags = 0;
        }

        // All done...  tell the caller we weren't able to keep this
        // activity around.
        return FALSE;
    }

    // Default case: the activity can handle this new configuration, so
    // hand it over.  Note that we don't need to give it the new
    // configuration, since we always send configuration changes to all
    // process when they happen so it can just use whatever configuration
    // it last got.
    if (r->mApp != NULL && r->mApp->mThread != NULL) {
        if (DEBUG_CONFIGURATION) {
            String arDes;
            r->ToString(&arDes);
            Slogger::V(TAG, "Sending new config to %s", arDes.string());
        }
        r->mApp->mThread->ScheduleActivityConfigurationChanged(IBinder::Probe(r->mAppToken));
    }
    r->StopFreezingScreenLocked(FALSE);

    return TRUE;
}

Boolean ActivityStack::RelaunchActivityLocked(
    /* [in] */ ActivityRecord* r,
    /* [in] */ Int32 changes,
    /* [in] */ Boolean andResume)
{
    AutoPtr<ArrayOf<IResultInfo*> > results;
    AutoPtr<ArrayOf<IIntent*> > newIntents;
    if (andResume) {
        if (r->mResults != NULL) {
            results = ArrayOf<IResultInfo*>::Alloc(r->mResults->GetSize());
            List<AutoPtr<ActivityResult> >::Iterator it = r->mResults->Begin();
            for (Int32 i = 0; it != r->mResults->End(); ++it, ++i) {
                results->Set(i, (*it)->mResultInfo.Get());
            }
        }
        if (r->mNewIntents != NULL) {
            newIntents = ArrayOf<IIntent*>::Alloc(r->mNewIntents->GetSize());
            List<AutoPtr<IIntent> >::Iterator it = r->mNewIntents->Begin();
            for (Int32 i = 0; it != r->mNewIntents->End(); ++it, ++i) {
                newIntents->Set(i, (*it).Get());
            }
        }
    }

    if (DEBUG_SWITCH) {
        String arDes;
        r->ToString(&arDes);
        Slogger::V(TAG, "Relaunching: %s with results=%p newIntents=%p andResume=%d", arDes.string(),
                results.Get(), newIntents.Get(), andResume);
    }
   // EventLog.writeEvent(andResume ? IEventLogTags.AM_RELAUNCH_RESUME_ACTIVITY
   //         : IEventLogTags.AM_RELAUNCH_ACTIVITY, r.userId, System.identityHashCode(r),
   //         r.task.taskId, r.shortComponentName);

    r->StartFreezingScreenLocked(r->mApp, 0);

    if (DEBUG_SWITCH || DEBUG_STATES) {
        // String arDes;
        // r->GetDescription(&arDes);
        // Slogger::I(TAG, (andResume ? StringBuffer("Relaunching to RESUMED ")
        //         : StringBuffer("Relaunching to PAUSED ")) + arDes);
    }
    r->mForceNewConfig = FALSE;
    AutoPtr<IConfiguration> configuration;
    CConfiguration::New(mService->mConfiguration, (IConfiguration**)&configuration);
    if (SUCCEEDED(r->mApp->mThread->ScheduleRelaunchActivity(
            IBinder::Probe(r->mAppToken), results, newIntents, changes, !andResume, configuration))) {
        // Note: don't need to call pauseIfSleepingLocked() here, because
        // the caller will only pass in 'andResume' if this activity is
        // currently resumed, which implies we aren't sleeping.
    }
    else {
        if (DEBUG_SWITCH || DEBUG_STATES) Slogger::I(TAG, String("Relaunch failed"));
    }

    if (andResume) {
        if (r->mResults) {
            r->mResults = NULL;
        }
        if (r->mNewIntents) {
            r->mNewIntents = NULL;
        }
        if (mMainStack) {
            mService->ReportResumedActivityLocked(r);
        }
        r->mState = ActivityState_RESUMED;
    }
    else {
        mHandler->RemoveMessages(PAUSE_TIMEOUT_MSG, r->Probe(EIID_IInterface));
    }

    return TRUE;
}

ECode ActivityStack::DismissKeyguardOnNextActivityLocked()
{
    mDismissKeyguardOnNextActivity = TRUE;
    return NOERROR;
}

}
}
}
}
