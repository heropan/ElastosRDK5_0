
#ifndef __ACTIVITYRECORD_H__
#define __ACTIVITYRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashSet.h>
#include <elastos/core/StringBuilder.h>
#include "os/SystemClock.h"
#include "am/ActivityResult.h"
#include "am/ActivityState.h"
#include "am/ConnectionRecord.h"
#include "am/CPendingIntentRecord.h"
#include "am/TaskRecord.h"
#include "am/ActivityStack.h"
//#include "am/ProcessRecord.h"
#include "am/ThumbnailHolder.h"
#include "am/UriPermissionOwner.h"


using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashSet;
using Elastos::Core::ICharSequence;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::View::IApplicationToken;
using Elastos::Droid::App::IActivityOptions;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ActivityStack;
class CActivityManagerService;
class ProcessRecord;

extern const InterfaceID EIID_ActivityRecord;

class ActivityRecord
    : public ElRefBase
    , public IWeakReferenceSource
{
public:
    CAR_INTERFACE_DECL()

    ActivityRecord(
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
        /* [in] */ Boolean componentSpecified);

    ~ActivityRecord();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix);

    static CARAPI ForToken(
        /* [in] */ IBinder* token,
        /* [out] */ ActivityRecord** activityRecord);

    CARAPI_(void) SetTask(
        /* [in] */ TaskRecord* newTask,
        /* [in] */ ThumbnailHolder* newThumbHolder,
        /* [in] */ Boolean isRoot);

    CARAPI_(void) PutInHistory();

    CARAPI_(void) TakeFromHistory();

    CARAPI_(Boolean) IsInHistory();

    CARAPI_(void) MakeFinishing();

    CARAPI_(AutoPtr<UriPermissionOwner>) GetUriPermissionsLocked();

    CARAPI_(void) AddResultLocked(
        /* [in] */ ActivityRecord* from,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData);

    CARAPI_(void) RemoveResultsLocked(
        /* [in] */ ActivityRecord* from,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode);

    CARAPI_(void) AddNewIntentLocked(
        /* [in] */ IIntent* intent);

    /**
     * Deliver a new Intent to an existing activity, so that its onNewIntent()
     * method will be called at the proper time.
     */
    CARAPI_(void) DeliverNewIntentLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ IIntent* intent);

    CARAPI_(void) UpdateOptionsLocked(
        /* [in] */ IBundle* options);

    CARAPI_(void) UpdateOptionsLocked(
        /* [in] */ IActivityOptions* options);

    CARAPI_(void) ApplyOptionsLocked();

    CARAPI_(void) ClearOptionsLocked();

    CARAPI_(AutoPtr<IActivityOptions>) TakeOptionsLocked();

    CARAPI_(void) RemoveUriPermissionsLocked();

    CARAPI_(void) PauseKeyDispatchingLocked();

    CARAPI_(void) ResumeKeyDispatchingLocked();

    CARAPI_(void) UpdateThumbnail(
        /* [in] */ IBitmap* newThumbnail,
        /* [in] */ ICharSequence* description);

    CARAPI_(void) StartLaunchTickingLocked();

    CARAPI_(Boolean) ContinueLaunchTickingLocked();

    CARAPI FinishLaunchTickingLocked();

    // IApplicationToken

    CARAPI MayFreezeScreenLocked(
        /* [in] */ ProcessRecord* app,
        /* [out] */ Boolean* result);

    CARAPI StartFreezingScreenLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 configChanges);

    CARAPI StopFreezingScreenLocked(
        /* [in] */ Boolean force);

    CARAPI WindowsDrawn();

    CARAPI WindowsVisible();

    CARAPI WindowsGone();

    CARAPI KeyDispatchingTimedOut(
        /* [out] */ Boolean* result);

    /** Returns the key dispatching timeout for this application token. */
    CARAPI GetKeyDispatchingTimeout(
        /* [out] */ Int64* timeout);

    /**
     * This method will return true if the activity is either visible, is becoming visible, is
     * currently pausing, or is resumed.
     */
    CARAPI_(Boolean) IsInterestingToUserLocked();

    CARAPI SetSleeping(
        /* [in] */ Boolean sleeping);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI_(String) ToString();

private:
    CARAPI_(AutoPtr<ActivityRecord>) GetWaitingHistoryRecordLocked();

public:
    CActivityManagerService* mService; // owner - weak-ref
    ActivityStack* mStack; // owner - weak-ref
    AutoPtr<IApplicationToken> mAppToken; // window manager token
    AutoPtr<IActivityInfo> mInfo; // all about me
    Int32 mLaunchedFromUid; // always the uid who started the activity.
    Int32 mUserId;          // Which user is this running for?
    AutoPtr<IIntent> mIntent;    // the original intent that generated us
    AutoPtr<IComponentName> mRealActivity;  // the intent component, or target of an alias.
    String mShortComponentName; // the short component name of the intent
    String mResolvedType; // as per original caller;
    String mPackageName; // the package implementing intent's component
    String mProcessName; // process where this component wants to run
    String mTaskAffinity; // as per ActivityInfo.taskAffinity
    Boolean mStateNotNeeded; // As per ActivityInfo.flags
    Boolean mFullscreen; // covers the full screen?
    Boolean mNoDisplay;  // activity is not displayed?
    Boolean mComponentSpecified;  // did caller specifiy an explicit component?
    Boolean mIsHomeActivity; // do we consider this to be a home activity?
    String mBaseDir;   // where activity source (resources etc) located
    String mResDir;   // where public activity source (public resources etc) located
    String mDataDir;   // where activity data should go
    AutoPtr<ICharSequence> mNonLocalizedLabel;  // the label information from the package mgr.
    Int32 mLabelRes;           // the label information from the package mgr.
    Int32 mIcon;               // resource identifier of activity's icon.
    Int32 mTheme;              // resource identifier of activity's theme.
    Int32 mRealTheme;          // actual theme resource we will use, never 0.
    Int32 mWindowFlags;        // custom window flags for preview window.
    AutoPtr<TaskRecord> mTask;        // the task this is in.
    AutoPtr<ThumbnailHolder> mThumbHolder; // where our thumbnails should go.
    Int64 mLaunchTime;        // when we starting launching this activity
    Int64 mStartTime;         // last time this activity was started
    Int64 mLastVisibleTime;   // last time this activity became visible
    Int64 mCpuTimeAtResume;   // the cpu time of host process at the time of resuming activity
    Int64 mPauseTime;         // last time we started pausing the activity
    Int64 mLaunchTickTime;    // base time for launch tick messages
    AutoPtr<IConfiguration> mConfiguration; // configuration activity was last running in
    AutoPtr<ICompatibilityInfo> mCompat;// last used compatibility mode
    AutoPtr<ActivityRecord> mResultTo; // who started this entry, so will get our reply
    String mResultWho; // additional identifier for use by resultTo.
    Int32 mRequestCode;  // code given by requester (resultTo)
    AutoPtr< List< AutoPtr<ActivityResult> > > mResults;      // pending ActivityResult objs we have received
    AutoPtr< HashSet<AutoPtr<IWeakReference> > > mPendingResults; // all pending intents for this act,PendingIntentRecord
    AutoPtr< List< AutoPtr<IIntent> > > mNewIntents;   // any pending new intents for single-top mode
    AutoPtr<IActivityOptions> mPendingOptions; // most recently given options
    AutoPtr< HashSet<AutoPtr<ConnectionRecord> > > mConnections; // All ConnectionRecord we hold
    AutoPtr<UriPermissionOwner> mUriPermissions; // current special URI access perms.
    AutoPtr<ProcessRecord> mApp;      // if non-null, hosting application
    ActivityState mState;    // current state we are in
    AutoPtr<IBundle> mIcicle;         // last saved activity state
    Boolean mFrontOfTask;    // is this the root activity of its task?
    Boolean mLaunchFailed;   // set if a launched failed, to abort on 2nd try
    Boolean mHaveState;      // have we gotten the last activity state?
    Boolean mStopped;        // is activity pause finished?
    Boolean mDelayedResume;  // not yet resumed because of stopped app switches?
    Boolean mFinishing;      // activity in pending finish list?
    Boolean mConfigDestroy;  // need to destroy due to config change?
    Int32 mConfigChangeFlags;  // which config values have changed
    Boolean mKeysPaused;     // has key dispatching been paused for it?
    Int32 mLaunchMode;         // the launch mode activity attribute.
    Boolean mVisible;        // does this activity's window need to be shown?
    Boolean mSleeping;       // have we told the activity to sleep?
    Boolean mWaitingVisible; // true if waiting for a new act to become vis
    Boolean mNowVisible;     // is this activity's window visible?
    Boolean mThumbnailNeeded;// has someone requested a thumbnail?
    Boolean mIdle;           // has the activity gone idle?
    Boolean mHasBeenLaunched;// has this activity ever been launched?
    Boolean mFrozenBeforeDestroy;// has been frozen but not yet destroyed.
    Boolean mImmersive;      // immersive mode (don't interrupt if possible)
    Boolean mForceNewConfig; // force re-create with new config next time

    String mStringName;      // for caching of toString().

private:
    Boolean mInHistory;  // are we in the history stack?
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#ifndef HASH_FUNC_FOR_AUTOPTR_ACTIVITYRECORD
#define HASH_FUNC_FOR_AUTOPTR_ACTIVITYRECORD
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::ActivityRecord)
#endif

#endif // __ACTIVITYRECORD_H__
