
#include "am/ActivityRecord.h"
#include "am/CActivityManagerService.h"
#include "am/ActivityState.h"
#include "am/CActivityRecordToken.h"
#include "util/TimeUtils.h"
#include "AttributeCache.h"
#include "elastos/droid/os/Process.h"
// #include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Build.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::ReverseIterator;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Utility::TimeUtils;
using Elastos::Droid::Utility::CParcelableObjectContainer;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// {67D7103D-C14E-4C50-9010-AB85E468822D}
extern const InterfaceID EIID_ActivityRecord =
        { 0x67d7103d, 0xc14e, 0x4c50, { 0x90, 0x10, 0xab, 0x85, 0xe4, 0x68, 0x82, 0x2d } };

//==============================================================================
//  ActivityRecord
//==============================================================================
ActivityRecord::ActivityRecord(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ ActivityStack* stack,
    /* [in] */ ProcessRecord* caller,
    /* [in] */ Int32 launchedFromUid,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IActivityInfo* aInfo,
    /* [in] */ IConfiguration* configuration,
    /* [in] */ ActivityRecord* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 reqCode,
    /* [in] */ Boolean componentSpecified)
    : mService(service)
    , mStack(stack)
    , mInfo(aInfo)
    , mLaunchedFromUid(launchedFromUid)
    , mIntent(intent)
    , mResolvedType(resolvedType)
    , mStateNotNeeded(FALSE)
    , mFullscreen(FALSE)
    , mNoDisplay(FALSE)
    , mComponentSpecified(componentSpecified)
    , mIsHomeActivity(FALSE)
    , mLabelRes(0)
    , mIcon(0)
    , mTheme(0)
    , mRealTheme(0)
    , mWindowFlags(0)
    , mLaunchTime(0)
    , mStartTime(0)
    , mLastVisibleTime(0)
    , mCpuTimeAtResume(0)
    , mPauseTime(0)
    , mLaunchTickTime(0)
    , mConfiguration(configuration)
    , mResultTo(resultTo)
    , mResultWho(resultWho)
    , mRequestCode(reqCode)
    , mApp(NULL)
    , mState(ActivityState_INITIALIZING)
    , mFrontOfTask(FALSE)
    , mLaunchFailed(FALSE)
    , mHaveState(FALSE)
    , mStopped(FALSE)
    , mDelayedResume(FALSE)
    , mFinishing(FALSE)
    , mConfigDestroy(FALSE)
    , mConfigChangeFlags(0)
    , mKeysPaused(FALSE)
    , mLaunchMode(0)
    , mVisible(TRUE)
    , mSleeping(FALSE)
    , mWaitingVisible(FALSE)
    , mNowVisible(FALSE)
    , mThumbnailNeeded(FALSE)
    , mIdle(FALSE)
    , mHasBeenLaunched(FALSE)
    , mFrozenBeforeDestroy(FALSE)
    , mImmersive(FALSE)
    , mForceNewConfig(FALSE)
    , mInHistory(FALSE)
{
    ASSERT_SUCCEEDED(CActivityRecordToken::New((Handle32)this, (IApplicationToken**)&mAppToken));

    AutoPtr<IApplicationInfo> appInfo;
    mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 uid;
    appInfo->GetUid(&uid);

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetUserId(uid, &mUserId);

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    component->FlattenToShortString(&mShortComponentName);

    // This starts out true, since the initial state of an activity
    // is that we have everything, and we shouldn't never consider it
    // lacking in state to be removed if it dies.
    mHaveState = TRUE;

    if (mInfo != NULL) {
        String target;
        mInfo->GetTargetActivity(&target);
        Int32 mode;
        mInfo->GetLaunchMode(&mode);
        if (target.IsNull()
                || mode == IActivityInfo::LAUNCH_MULTIPLE
                || mode == IActivityInfo::LAUNCH_SINGLE_TOP) {
            mIntent->GetComponent((IComponentName**)&mRealActivity);
        }
        else {
            String pname;
            mInfo->GetPackageName(&pname);
            CComponentName::New(pname, target, (IComponentName**)&mRealActivity);
        }
        mInfo->GetTaskAffinity(&mTaskAffinity);
        Int32 flags;
        mInfo->GetFlags(&flags);
        mStateNotNeeded = (flags & IActivityInfo::FLAG_STATE_NOT_NEEDED) != 0;
        appInfo->GetSourceDir(&mBaseDir);
        appInfo->GetPublicSourceDir(&mResDir);
        appInfo->GetDataDir(&mDataDir);
        mInfo->GetNonLocalizedLabel((ICharSequence**)&mNonLocalizedLabel);
        mInfo->GetLabelRes(&mLabelRes);
        if (mNonLocalizedLabel == NULL && mLabelRes == 0) {
            appInfo->GetNonLocalizedLabel((ICharSequence**)&mNonLocalizedLabel);
            appInfo->GetLabelRes(&mLabelRes);
        }
        mInfo->GetIconResource(&mIcon);
        mInfo->GetThemeResource(&mTheme);
        mRealTheme = mTheme;
        if (mRealTheme == 0) {
            Int32 version;
            appInfo->GetTargetSdkVersion(&version);
            mRealTheme = version
                < Build::VERSION_CODES::HONEYCOMB
                ? R::style::Theme : R::style::Theme_Holo;
        }
        if ((flags & IActivityInfo::FLAG_HARDWARE_ACCELERATED) != 0) {
            mWindowFlags |= IWindowManagerLayoutParams::FLAG_HARDWARE_ACCELERATED;
        }
        Int32 appUid, aUid;
        if ((flags & IActivityInfo::FLAG_MULTIPROCESS) != 0
                && caller != NULL
                && (appInfo->GetUid(&appUid), caller->mInfo->GetUid(&aUid),
                    appUid == IProcess::SYSTEM_UID || appUid == aUid)) {
            mProcessName = caller->mProcessName;
        }
        else {
            mInfo->GetProcessName(&mProcessName);
        }

        if (intent != NULL && (flags & IActivityInfo::FLAG_EXCLUDE_FROM_RECENTS) != 0) {
            intent->AddFlags(IIntent::FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
        }

        appInfo->GetPackageName(&mPackageName);
        mInfo->GetLaunchMode(&mLaunchMode);

        AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Window),
            ARRAY_SIZE(R::styleable::Window));

        AutoPtr<AttributeCache::Entry> ent = AttributeCache::GetInstance()->Get(
            mUserId, mPackageName, mRealTheme, attrIds);
        if (ent != NULL) {
            Boolean isFloating, isTranslucent;
            ent->mArray->GetBoolean(
               R::styleable::Window_windowIsFloating, FALSE, &isFloating);
            ent->mArray->GetBoolean(
               R::styleable::Window_windowIsTranslucent, FALSE, &isTranslucent);
            mFullscreen = !isFloating && !isTranslucent;

            ent->mArray->GetBoolean(
               R::styleable::Window_windowNoDisplay, FALSE, &mNoDisplay);
        }

        if (!mComponentSpecified || mLaunchedFromUid == Process::MyUid()
                || mLaunchedFromUid == 0) {
            // If we know the system has determined the component, then
            // we can consider this to be a home activity...
            String action;
            Boolean hasHomeCategory;
            AutoPtr< ArrayOf<String> > categories;
            AutoPtr<IUri> data;
            String type;
            Int32 iflags;
            String name;
            if ((mIntent->GetAction(&action), IIntent::ACTION_MAIN.Equals(action)) &&
                (mIntent->HasCategory(IIntent::CATEGORY_HOME, &hasHomeCategory), hasHomeCategory) &&
                (mIntent->GetCategories((ArrayOf<String>**)&categories), categories != NULL && categories->GetLength() == 1) &&
                (mIntent->GetData((IUri**)&data), data == NULL) &&
                (mIntent->GetType(&type), type.IsNull()) &&
                (mIntent->GetFlags(&iflags), iflags & IIntent::FLAG_ACTIVITY_NEW_TASK) != 0 /*&&
                (mRealActivity->GetClassName(&name), ResolverActivity.class.getName().Equals(name))*/) {
                // This sure looks like a home activity!
                // Note the last check is so we don't count the resolver
                // activity as being home...  really, we don't care about
                // doing anything special with something that comes from
                // the core framework package.
                mIsHomeActivity = TRUE;
            }
        }

        mInfo->GetFlags(&flags);
        mImmersive = (flags & IActivityInfo::FLAG_IMMERSIVE) != 0;
    }
    else {
        mStateNotNeeded = FALSE;
        mFullscreen = TRUE;
        mNoDisplay = FALSE;
        mIsHomeActivity = FALSE;
        mImmersive = FALSE;
    }
}

ActivityRecord::~ActivityRecord()
{
    mNewIntents = NULL;
    mResults = NULL;
    Slogger::D("ActivityRecord", " =========== ~ActivityRecord() %s", ToString().string());
}

CAR_INTERFACE_IMPL_WITH_CPP_CAST(ActivityRecord, IWeakReferenceSource, ActivityRecord)

ECode ActivityRecord::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

void ActivityRecord::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    assert(0);
    // final long now = SystemClock.uptimeMillis();
    // pw.print(prefix); pw.print("packageName="); pw.print(packageName);
    //         pw.print(" processName="); pw.println(processName);
    // pw.print(prefix); pw.print("launchedFromUid="); pw.print(launchedFromUid);
    //         pw.print(" userId="); pw.println(userId);
    // pw.print(prefix); pw.print("app="); pw.println(app);
    // pw.print(prefix); pw.println(intent.toInsecureStringWithClip());
    // pw.print(prefix); pw.print("frontOfTask="); pw.print(frontOfTask);
    //         pw.print(" task="); pw.println(task);
    // pw.print(prefix); pw.print("taskAffinity="); pw.println(taskAffinity);
    // pw.print(prefix); pw.print("realActivity=");
    //         pw.println(realActivity.flattenToShortString());
    // pw.print(prefix); pw.print("baseDir="); pw.println(baseDir);
    // if (!resDir.equals(baseDir)) {
    //     pw.print(prefix); pw.print("resDir="); pw.println(resDir);
    // }
    // pw.print(prefix); pw.print("dataDir="); pw.println(dataDir);
    // pw.print(prefix); pw.print("stateNotNeeded="); pw.print(stateNotNeeded);
    //         pw.print(" componentSpecified="); pw.print(componentSpecified);
    //         pw.print(" isHomeActivity="); pw.println(isHomeActivity);
    // pw.print(prefix); pw.print("compat="); pw.print(compat);
    //         pw.print(" labelRes=0x"); pw.print(Integer.toHexString(labelRes));
    //         pw.print(" icon=0x"); pw.print(Integer.toHexString(icon));
    //         pw.print(" theme=0x"); pw.println(Integer.toHexString(theme));
    // pw.print(prefix); pw.print("config="); pw.println(configuration);
    // if (resultTo != null || resultWho != null) {
    //     pw.print(prefix); pw.print("resultTo="); pw.print(resultTo);
    //             pw.print(" resultWho="); pw.print(resultWho);
    //             pw.print(" resultCode="); pw.println(requestCode);
    // }
    // if (results != null) {
    //     pw.print(prefix); pw.print("results="); pw.println(results);
    // }
    // if (pendingResults != null && pendingResults.size() > 0) {
    //     pw.print(prefix); pw.println("Pending Results:");
    //     for (WeakReference<PendingIntentRecord> wpir : pendingResults) {
    //         PendingIntentRecord pir = wpir != null ? wpir.get() : null;
    //         pw.print(prefix); pw.print("  - ");
    //         if (pir == null) {
    //             pw.println("null");
    //         } else {
    //             pw.println(pir);
    //             pir.dump(pw, prefix + "    ");
    //         }
    //     }
    // }
    // if (newIntents != null && newIntents.size() > 0) {
    //     pw.print(prefix); pw.println("Pending New Intents:");
    //     for (int i=0; i<newIntents.size(); i++) {
    //         Intent intent = (Intent)newIntents.get(i);
    //         pw.print(prefix); pw.print("  - ");
    //         if (intent == null) {
    //             pw.println("null");
    //         } else {
    //             pw.println(intent.toShortString(false, true, false, true));
    //         }
    //     }
    // }
    // if (pendingOptions != null) {
    //     pw.print(prefix); pw.print("pendingOptions="); pw.println(pendingOptions);
    // }
    // if (uriPermissions != null) {
    //     if (uriPermissions.readUriPermissions != null) {
    //         pw.print(prefix); pw.print("readUriPermissions=");
    //                 pw.println(uriPermissions.readUriPermissions);
    //     }
    //     if (uriPermissions.writeUriPermissions != null) {
    //         pw.print(prefix); pw.print("writeUriPermissions=");
    //                 pw.println(uriPermissions.writeUriPermissions);
    //     }
    // }
    // pw.print(prefix); pw.print("launchFailed="); pw.print(launchFailed);
    //         pw.print(" haveState="); pw.print(haveState);
    //         pw.print(" icicle="); pw.println(icicle);
    // pw.print(prefix); pw.print("state="); pw.print(state);
    //         pw.print(" stopped="); pw.print(stopped);
    //         pw.print(" delayedResume="); pw.print(delayedResume);
    //         pw.print(" finishing="); pw.println(finishing);
    // pw.print(prefix); pw.print("keysPaused="); pw.print(keysPaused);
    //         pw.print(" inHistory="); pw.print(inHistory);
    //         pw.print(" visible="); pw.print(visible);
    //         pw.print(" sleeping="); pw.print(sleeping);
    //         pw.print(" idle="); pw.println(idle);
    // pw.print(prefix); pw.print("fullscreen="); pw.print(fullscreen);
    //         pw.print(" noDisplay="); pw.print(noDisplay);
    //         pw.print(" immersive="); pw.print(immersive);
    //         pw.print(" launchMode="); pw.println(launchMode);
    // pw.print(prefix); pw.print("frozenBeforeDestroy="); pw.print(frozenBeforeDestroy);
    //         pw.print(" thumbnailNeeded="); pw.print(thumbnailNeeded);
    //         pw.print(" forceNewConfig="); pw.println(forceNewConfig);
    // pw.print(prefix); pw.print("thumbHolder: ");
    //         pw.print(Integer.toHexString(System.identityHashCode(thumbHolder)));
    //         if (thumbHolder != null) {
    //             pw.print(" bm="); pw.print(thumbHolder.lastThumbnail);
    //             pw.print(" desc="); pw.print(thumbHolder.lastDescription);
    //         }
    //         pw.println();
    // if (launchTime != 0 || startTime != 0) {
    //     pw.print(prefix); pw.print("launchTime=");
    //             if (launchTime == 0) pw.print("0");
    //             else TimeUtils.formatDuration(launchTime, now, pw);
    //             pw.print(" startTime=");
    //             if (startTime == 0) pw.print("0");
    //             else TimeUtils.formatDuration(startTime, now, pw);
    //             pw.println();
    // }
    // if (lastVisibleTime != 0 || waitingVisible || nowVisible) {
    //     pw.print(prefix); pw.print("waitingVisible="); pw.print(waitingVisible);
    //             pw.print(" nowVisible="); pw.print(nowVisible);
    //             pw.print(" lastVisibleTime=");
    //             if (lastVisibleTime == 0) pw.print("0");
    //             else TimeUtils.formatDuration(lastVisibleTime, now, pw);
    //             pw.println();
    // }
    // if (configDestroy || configChangeFlags != 0) {
    //     pw.print(prefix); pw.print("configDestroy="); pw.print(configDestroy);
    //             pw.print(" configChangeFlags=");
    //             pw.println(Integer.toHexString(configChangeFlags));
    // }
    // if (connections != null) {
    //     pw.print(prefix); pw.print("connections="); pw.println(connections);
    // }
}

ECode ActivityRecord::ForToken(
    /* [in] */ IBinder* token,
    /* [out] */ ActivityRecord** activityRecord)
{
    VALIDATE_NOT_NULL(activityRecord);
    *activityRecord = NULL;

    // try {
    IApplicationToken* at = IApplicationToken::Probe(token);
    if (at != NULL) {
        AutoPtr<ActivityRecord> ar = ((CActivityRecordToken*)at)->GetActivityRecord();
        *activityRecord = ar;
        REFCOUNT_ADD(*activityRecord);
    }
    return NOERROR;
    // } catch (ClassCastException e) {
    //     Slog.w(ActivityManagerService.TAG, "Bad activity token: " + token, e);
    //     return null;
    // }
}

void ActivityRecord::SetTask(
    /* [in] */ TaskRecord* newTask,
    /* [in] */ ThumbnailHolder* newThumbHolder,
    /* [in] */ Boolean isRoot)
{
    if (mInHistory && !mFinishing) {
        if (mTask != NULL) {
            mTask->mNumActivities--;
        }
        if (newTask != NULL) {
            newTask->mNumActivities++;
        }
    }
    if (newThumbHolder == NULL) {
        newThumbHolder = newTask;
    }
    mTask = newTask;
    Int32 flags;
    if (!isRoot && (mIntent->GetFlags(&flags), flags & IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0) {
        // This is the start of a new sub-task.
        if (mThumbHolder == NULL) {
            mThumbHolder = new ThumbnailHolder();
        }
    }
    else {
        mThumbHolder = newThumbHolder;
    }
}

void ActivityRecord::PutInHistory()
{
    if (!mInHistory) {
        mInHistory = TRUE;
        if (mTask != NULL && !mFinishing) {
            mTask->mNumActivities++;
        }
    }
}

void ActivityRecord::TakeFromHistory()
{
    if (mInHistory) {
        mInHistory = FALSE;
        if (mTask != NULL && !mFinishing) {
            mTask->mNumActivities--;
        }
        ClearOptionsLocked();
    }
}

Boolean ActivityRecord::IsInHistory()
{
    return mInHistory;
}

void ActivityRecord::MakeFinishing()
{
    if (!mFinishing) {
        mFinishing = TRUE;
        if (mTask != NULL && mInHistory) {
            mTask->mNumActivities--;
        }
        if (mStopped) {
            ClearOptionsLocked();
        }
    }
}

AutoPtr<UriPermissionOwner> ActivityRecord::GetUriPermissionsLocked()
{
    if (mUriPermissions == NULL) {
        mUriPermissions = new UriPermissionOwner(mService, (Handle32)this);
    }
    return mUriPermissions;
}

void ActivityRecord::AddResultLocked(
    /* [in] */ ActivityRecord* from,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData)
{
    AutoPtr<ActivityResult> r = new ActivityResult(from, resultWho,
           requestCode, resultCode, resultData);
    if (mResults == NULL) {
        mResults = new List< AutoPtr<ActivityResult> >();
    }
    mResults->PushBack(r);
}

void ActivityRecord::RemoveResultsLocked(
    /* [in] */ ActivityRecord* from,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode)
{
    if (mResults != NULL) {
        String who;
        Int32 code = 0;
        List< AutoPtr<ActivityResult> >::ReverseIterator rit = mResults->RBegin();
        while (rit != mResults->REnd()) {
            AutoPtr<ActivityResult> r = *rit;
            r->mResultInfo->GetResultWho(&who);
            r->mResultInfo->GetRequestCode(&code);

            assert(r != NULL);
            if (r->mFrom != from) {
                ++rit;
                continue;
            }
            if (who.IsNull()) {
               if (!resultWho.IsNull()) {
                    ++rit;
                    continue;
                }
            }
            else {
               if (!who.Equals(resultWho)) {
                    ++rit;
                    continue;
                }
            }
            if (code != requestCode) {
                ++rit;
                continue;
            }

            rit = List< AutoPtr<ActivityResult> >::ReverseIterator(mResults->Erase(--(rit.GetBase())));
        }
    }
}

void ActivityRecord::AddNewIntentLocked(
    /* [in] */ IIntent* intent)
{
    if (mNewIntents == NULL) {
        mNewIntents = new List< AutoPtr<IIntent> >();
    }
    mNewIntents->PushBack(intent);
}

void ActivityRecord::DeliverNewIntentLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ IIntent* _intent)
{
    Boolean sent = FALSE;
    // We want to immediately deliver the intent to the activity if
    // it is currently the top resumed activity...  however, if the
    // device is sleeping, then all activities are stopped, so in that
    // case we will deliver it if this is the current top activity on its
    // stack.
    AutoPtr<ActivityRecord> r = mStack->GetTopRunningActivityLocked(NULL);
    if ((mState == ActivityState_RESUMED || (mService->mSleeping && r.Get() == this))
           && mApp != NULL && mApp->mThread != NULL) {
        // try {
        AutoPtr<IObjectContainer> ar;
        CParcelableObjectContainer::New((IObjectContainer**)&ar);
        AutoPtr<IIntent> intent;
        CIntent::New(_intent, (IIntent**)&intent);
        ar->Add(intent.Get());
        AutoPtr<UriPermissionOwner> owner = GetUriPermissionsLocked();
        ECode ec = mService->GrantUriPermissionFromIntentLocked(
            callingUid, mPackageName, intent, owner);

        ec = mApp->mThread->ScheduleNewIntent(ar, IBinder::Probe(mAppToken.Get()));
        if (SUCCEEDED(ec)) {
            sent = TRUE;
        }
        else {
            String info;
            ToString(&info);
            Slogger::W("ActivityRecord",
                "Exception thrown sending new intent to %s, error code: %08x", info.string(), ec);
        }
        // } catch (RemoteException e) {
        //     Slog.w(ActivityManagerService.TAG,
        //             "Exception thrown sending new intent to " + this, e);
        // } catch (NullPointerException e) {
        //     Slog.w(ActivityManagerService.TAG,
        //             "Exception thrown sending new intent to " + this, e);
        // }
    }
    if (!sent) {
        AutoPtr<IIntent> intent;
        CIntent::New(_intent, (IIntent**)&intent);
        AddNewIntentLocked(intent);
    }
}

void ActivityRecord::UpdateOptionsLocked(
    /* [in] */ IBundle* options)
{
    if (options != NULL) {
        ClearOptionsLocked();
        mPendingOptions = NULL;
        CActivityOptions::New(options, (IActivityOptions**)&mPendingOptions);
    }
}

void ActivityRecord::UpdateOptionsLocked(
    /* [in] */ IActivityOptions* options)
{
    if (options != NULL) {
        ClearOptionsLocked();
        mPendingOptions = options;
    }
}

void ActivityRecord::ApplyOptionsLocked()
{
    if (mPendingOptions != NULL) {
        Int32 animationType;
        mPendingOptions->GetAnimationType(&animationType);
        switch (animationType) {
            case IActivityOptions::ANIM_CUSTOM:
            {
                String packageName;
                Int32 enterResId, exitResId;
                AutoPtr<IRemoteCallback> listener;
                mPendingOptions->GetPackageName(&packageName);
                mPendingOptions->GetCustomEnterResId(&enterResId);
                mPendingOptions->GetCustomExitResId(&exitResId);
                mPendingOptions->GetOnAnimationStartListener((IRemoteCallback**)&listener);
                mService->mWindowManager->OverridePendingAppTransition(
                    packageName, enterResId, exitResId, listener);
                break;
            }
            case IActivityOptions::ANIM_SCALE_UP:
            {
                Int32 x, y, w, h;
                mPendingOptions->GetStartX(&x);
                mPendingOptions->GetStartY(&y);
                mPendingOptions->GetStartWidth(&w);
                mPendingOptions->GetStartHeight(&h);
                mService->mWindowManager->OverridePendingAppTransitionScaleUp(x, y, w, h);
                AutoPtr<IRect> rect;
                mIntent->GetSourceBounds((IRect**)&rect);
                if (rect == NULL) {
                    CRect::New(x, y, x + w, y + h, (IRect**)&rect);
                    mIntent->SetSourceBounds(rect);
                }
                break;
            }
            case IActivityOptions::ANIM_THUMBNAIL_SCALE_UP:
            case IActivityOptions::ANIM_THUMBNAIL_SCALE_DOWN:
            {
                Boolean scaleUp = (animationType == IActivityOptions::ANIM_THUMBNAIL_SCALE_UP);
                AutoPtr<IBitmap> thumb;
                mPendingOptions->GetThumbnail((IBitmap**)&thumb);
                Int32 x, y;
                mPendingOptions->GetStartX(&x);
                mPendingOptions->GetStartY(&y);
                AutoPtr<IRemoteCallback> listener;
                mPendingOptions->GetOnAnimationStartListener((IRemoteCallback**)&listener);
                mService->mWindowManager->OverridePendingAppTransitionThumb(
                    thumb, x, y, listener, scaleUp);
                AutoPtr<IRect> rect;
                mIntent->GetSourceBounds((IRect**)&rect);
                if (rect == NULL) {
                    Int32 w, h;
                    thumb->GetWidth(&w);
                    thumb->GetHeight(&h);
                    CRect::New(x, y, x + w, y + h, (IRect**)&rect);
                    mIntent->SetSourceBounds(rect);
                }
                break;
            }
        }
        mPendingOptions = NULL;
    }
}

void ActivityRecord::ClearOptionsLocked()
{
    if (mPendingOptions != NULL) {
        mPendingOptions->Abort();
        mPendingOptions = NULL;
    }
}

AutoPtr<IActivityOptions> ActivityRecord::TakeOptionsLocked()
{
    AutoPtr<IActivityOptions> opts = mPendingOptions;
    mPendingOptions = NULL;
    return opts;
}

void ActivityRecord::RemoveUriPermissionsLocked()
{
    if (mUriPermissions != NULL) {
        mUriPermissions->RemoveUriPermissionsLocked();
        mUriPermissions = NULL;
    }
}

void ActivityRecord::PauseKeyDispatchingLocked()
{
    if (!mKeysPaused) {
        mKeysPaused = TRUE;
        mService->mWindowManager->PauseKeyDispatching(IBinder::Probe(mAppToken));
    }
}

void ActivityRecord::ResumeKeyDispatchingLocked()
{
    if (mKeysPaused) {
        mKeysPaused = FALSE;
        mService->mWindowManager->ResumeKeyDispatching(IBinder::Probe(mAppToken));
    }
}

void ActivityRecord::UpdateThumbnail(
    /* [in] */ IBitmap* newThumbnail,
    /* [in] */ ICharSequence* description)
{
    Int32 flags;
    if ((mIntent->GetFlags(&flags), flags & IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET) != 0) {
        // This is a logical break in the task; it repre
    }
    if (mThumbHolder != NULL) {
        if (newThumbnail != NULL) {
            if (CActivityManagerService::DEBUG_THUMBNAILS) {
                String info;
                ToString(&info);
                Slogger::I(CActivityManagerService::TAG,
                    "Setting thumbnail of %s holder %p to %p", info.string(), mThumbHolder.Get(), newThumbnail);
            }
            mThumbHolder->mLastThumbnail = newThumbnail;
        }
        mThumbHolder->mLastDescription = description;
    }
}

void ActivityRecord::StartLaunchTickingLocked()
{
    if (CActivityManagerService::IS_USER_BUILD) {
        return;
    }
    if (mLaunchTickTime == 0) {
        mLaunchTickTime = SystemClock::GetUptimeMillis();
        ContinueLaunchTickingLocked();
    }
}

Boolean ActivityRecord::ContinueLaunchTickingLocked()
{
    if (mLaunchTickTime != 0) {
        AutoPtr<IMessage> msg;
        mStack->mHandler->ObtainMessage(ActivityStack::LAUNCH_TICK_MSG, (IMessage**)&msg);
        msg->SetObj(this->Probe(EIID_IInterface));

        mStack->mHandler->RemoveMessages(ActivityStack::LAUNCH_TICK_MSG);
        Boolean result;
        mStack->mHandler->SendMessageDelayed(msg, ActivityStack::LAUNCH_TICK, &result);
        return TRUE;
    }
    return FALSE;
}

ECode ActivityRecord::FinishLaunchTickingLocked()
{
    mLaunchTickTime = 0;
    mStack->mHandler->RemoveMessages(ActivityStack::LAUNCH_TICK_MSG);
    return NOERROR;
}

ECode ActivityRecord::MayFreezeScreenLocked(
    /* [in] */ ProcessRecord* app,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Only freeze the screen if this activity is currently attached to
    // an application, and that application is not blocked or unresponding.
    // In any other case, we can't count on getting the screen unfrozen,
    // so it is best to leave as-is.
    *result = app != NULL && (!app->mCrashing && !app->mNotResponding);
    return NOERROR;
}

ECode ActivityRecord::StartFreezingScreenLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 configChanges)
{
    Boolean b;
    MayFreezeScreenLocked(app, &b);
    if (b) {
        return mService->mWindowManager->StartAppFreezingScreen(IBinder::Probe(mAppToken), configChanges);
    }
    return NOERROR;
}

ECode ActivityRecord::StopFreezingScreenLocked(
    /* [in] */ Boolean force)
{
    if (force || mFrozenBeforeDestroy) {
        mFrozenBeforeDestroy = FALSE;
        FAIL_RETURN(mService->mWindowManager->StopAppFreezingScreen(IBinder::Probe(mAppToken), force));
    }
    return NOERROR;
}

ECode ActivityRecord::WindowsDrawn()
{
    AutoLock lock(mService->mLock);

    if (mLaunchTime != 0) {
        const Int32 curTime = SystemClock::GetUptimeMillis();
        const Int32 thisTime = curTime - mLaunchTime;
        const Int32 totalTime = mStack->mInitialStartTime != 0
               ? (curTime - mStack->mInitialStartTime) : thisTime;
        if (CActivityManagerService::SHOW_ACTIVITY_START_TIME) {
            // EventLog.writeEvent(EventLogTags.AM_ACTIVITY_LAUNCH_TIME,
            //     userId, System.identityHashCode(this), shortComponentName,
            //     thisTime, totalTime);
            AutoPtr<StringBuilder> sb = mService->mStringBuilder;
            sb->Reset();
            sb->AppendCStr("Displayed ");
            sb->AppendString(mShortComponentName);
            sb->AppendCStr(": ");
            TimeUtils::FormatDuration(thisTime, *sb);
            if (thisTime != totalTime) {
                sb->AppendCStr(" (total ");
                TimeUtils::FormatDuration(totalTime, *sb);
                sb->AppendCStr(")");
            }
            Slogger::I(CActivityManagerService::TAG, sb->ToString());
        }
        mStack->ReportActivityLaunchedLocked(FALSE, this, thisTime, totalTime);
        if (totalTime > 0) {
            mService->mUsageStatsService->NoteLaunchTime(mRealActivity, (int)totalTime);
        }
        mLaunchTime = 0;
        mStack->mInitialStartTime = 0;
    }
    mStartTime = 0;
    return FinishLaunchTickingLocked();
}

ECode ActivityRecord::WindowsVisible()
{
    AutoLock lock(mService->mLock);

    mStack->ReportActivityVisibleLocked(this);
    if (CActivityManagerService::DEBUG_SWITCH) {
        String info;
        ToString(&info);
        Slogger::V(CActivityManagerService::TAG, "windowsVisible(): %s", info.string());
    }
    if (!mNowVisible) {
        mNowVisible = TRUE;
        mLastVisibleTime = SystemClock::GetUptimeMillis();
        if (!mIdle) {
            // Instead of doing the full stop routine here, let's just
            // hide any activities we now can, and let them stop when
            // the normal idle happens.
            mStack->ProcessStoppingActivitiesLocked(FALSE);
        }
        else {
            // If this activity was already idle, then we now need to
            // make sure we perform the full stop of any activities
            // that are waiting to do so.    This is because we won't
            // do that while they are still waiting for this one to
            // become visible.
            List< AutoPtr<ActivityRecord> >& activities = mStack->mWaitingVisibleActivities;
            List< AutoPtr<ActivityRecord> >::Iterator it;
            for (it = activities.Begin(); it != activities.End(); ++it) {
                AutoPtr<ActivityRecord> r = *it;
                r->mWaitingVisible = FALSE;
                if(CActivityManagerService::DEBUG_SWITCH) {
                    String info;
                    r->ToString(&info);
                    Slogger::V(CActivityManagerService::TAG, "Was waiting for visible: %s", info.string());
                }
            }
            activities.Clear();
            mStack->ScheduleIdleLocked();
        }
        mService->ScheduleAppGcsLocked();
    }
    return NOERROR;
}

ECode ActivityRecord::WindowsGone()
{
    if (CActivityManagerService::DEBUG_SWITCH) {
        String info;
        ToString(&info);
        Slogger::V(CActivityManagerService::TAG, "windowsGone(): %s", info.string());
    }
    mNowVisible = FALSE;
    return NOERROR;
}

AutoPtr<ActivityRecord> ActivityRecord::GetWaitingHistoryRecordLocked()
{
    // First find the real culprit...  if we are waiting
    // for another app to start, then we have paused dispatching
    // for this activity.
    AutoPtr<ActivityRecord> r = this;
    if (r->mWaitingVisible) {
        // Hmmm, who might we be waiting for?
        r = mStack->mResumedActivity;
        if (r == NULL) {
            r = mStack->mPausingActivity;
        }
        // Both of those null?  Fall back to 'this' again
        if (r == NULL) {
            r = this;
        }
    }

    return r;
}

ECode ActivityRecord::KeyDispatchingTimedOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // TODO: Unify this code with ActivityManagerService.inputDispatchingTimedOut().
    AutoPtr<ActivityRecord> r;
    AutoPtr<ProcessRecord> anrApp;
    {
        AutoLock lock(mService->mLock);

        r = GetWaitingHistoryRecordLocked();
        if (r != NULL && r->mApp != NULL)
        {
            if (r->mApp->mDebugging) {
                *result = FALSE;
                return NOERROR;
            }

            if (mService->mDidDexOpt) {
                // Give more time since we were dexopting.
                mService->mDidDexOpt = FALSE;
                *result = FALSE;
                return NOERROR;
            }

            if (r->mApp->mInstrumentationClass == NULL) {
                anrApp = r->mApp;
            }
            else {
                AutoPtr<IBundle> info;
                CBundle::New((IBundle**)&info);
                info->PutString(String("shortMsg"), String("keyDispatchingTimedOut"));
                info->PutString(String("longMsg"), String("Timed out while dispatching key event"));
                mService->FinishInstrumentationLocked(
                        r->mApp, IActivity::RESULT_CANCELED, info);
            }
        }
    }

    if (anrApp != NULL) {
        mService->AppNotResponding(anrApp, r, this, FALSE, String("keyDispatchingTimedOut"));
    }

    *result = TRUE;
    return NOERROR;
}

ECode ActivityRecord::GetKeyDispatchingTimeout(
    /* [out] */ Int64* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    AutoLock lock(mService->mLock);

    AutoPtr<ActivityRecord> r = GetWaitingHistoryRecordLocked();
    if (r != NULL && r->mApp != NULL
        && (r->mApp->mInstrumentationClass != NULL || r->mApp->mUsingWrapper)) {
        *timeout = CActivityManagerService::INSTRUMENTATION_KEY_DISPATCHING_TIMEOUT;
        return NOERROR;
    }

    *timeout = CActivityManagerService::KEY_DISPATCHING_TIMEOUT;
    return NOERROR;
}

Boolean ActivityRecord::IsInterestingToUserLocked()
{
    return mVisible || mNowVisible || mState == ActivityState_PAUSING ||
            mState == ActivityState_RESUMED;
}

ECode ActivityRecord::SetSleeping(
    /* [in] */ Boolean sleeping)
{
    if (mSleeping == sleeping) {
        return NOERROR;
    }
    if (mApp != NULL && mApp->mThread != NULL) {
        // try {
        mApp->mThread->ScheduleSleeping(IBinder::Probe(mAppToken), sleeping);
        List< AutoPtr<ActivityRecord> >& goingToSleepActivities = mStack->mGoingToSleepActivities;
        if (mSleeping && Find(goingToSleepActivities.Begin(), goingToSleepActivities.End(), AutoPtr<ActivityRecord>(this))
                == goingToSleepActivities.End()) {
            goingToSleepActivities.PushBack(this);
        }
        mSleeping = sleeping;
        // } catch (RemoteException e) {
        //     Slog.w(ActivityStack.TAG, "Exception thrown when sleeping: "
        //             + intent.getComponent(), e);
        // }
    }
    return NOERROR;
}

ECode ActivityRecord::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = ToString();
    return NOERROR;
}

String ActivityRecord::ToString()
{
    if (mStringName.IsNull()) {
        StringBuilder sb(128);
        sb += "ActivityRecord{0X";
        sb += StringUtils::Int32ToHexString((Int32)this);
        sb += " userId:";
        sb += mUserId;
        sb += ' ';
        sb += mShortComponentName;
        sb += '}';
        sb.ToString(&mStringName);
    }
    return mStringName;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
