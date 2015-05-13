
#ifndef __PROCESSRECORD_H__
#define __PROCESSRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/StringBuilder.h>
#include "os/BatteryStatsImpl.h"
#include "am/ReceiverList.h"
#include "am/ConnectionRecord.h"
#include "am/ContentProviderRecord.h"
#include "am/CServiceRecord.h"
#include "am/ActivityRecord.h"
#include "am/BroadcastRecord.h"

using Elastos::Utility::List;
using Elastos::Utility::HashMap;
using Elastos::Utility::HashSet;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::BatteryStatsImpl;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::App::IApplicationThread;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IInstrumentationWatcher;
using Elastos::Droid::App::IActivityManagerProcessErrorStateInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

extern const InterfaceID EIID_ProcessRecord;

class ProcessRecord
    : public ElRefBase
    , public IInterface
{
public:
    ProcessRecord(
        /* [in] */ BatteryStatsImpl::Uid::Proc* batteryStats,
        /* [in] */ IApplicationThread* appApartment,
        /* [in] */ IApplicationInfo* info,
        /* [in] */ const String& processName,
        /* [in] */ Int32 uid);

    ~ProcessRecord();

    CAR_INTERFACE_DECL()

    // void dump(PrintWriter pw, String prefix);

    CARAPI SetPid(
        /* [in] */ Int32 pid);

    /**
     * This method returns true if any of the activities within the process record are interesting
     * to the user. See HistoryRecord.isInterestingToUserLocked()
     */
    CARAPI_(Boolean) IsInterestingToUserLocked();

    CARAPI StopFreezingAllLocked();

    CARAPI UnlinkDeathRecipient();

    CARAPI UpdateHasAboveClientLocked();

    CARAPI_(String) ToShortString();

    CARAPI ToShortString(
        /* [in] */ StringBuilder& sb);

    CARAPI_(String) ToString();

    /*
     *  Return true if package has been added false if not
     */
    CARAPI_(Boolean) AddPackage(
        /* [in] */ const String& cap);

    /*
     *  Delete all packages from list except the package indicated in info
     */
    CARAPI ResetPackageList();

    CARAPI_(AutoPtr< ArrayOf<String> >) GetPackageList();

public:
    AutoPtr<BatteryStatsImpl::Uid::Proc> mBatteryStats; // where to collect runtime statistics
    AutoPtr<IApplicationInfo> mInfo;
    Boolean mIsolated;     // true if this is a special isolated process
    Int32 mUid;              // uid of process; may be different from 'info' if isolated
    Int32 mUserId;           // user of process.
    String mProcessName;
    // List of packages running in the process
    HashSet<String> mPkgList;
    AutoPtr<IApplicationThread> mThread; // the actual proc...  may be null only if
                                // 'persistent' is true (in which case we
                                // are in the process of launching the app)
    Int32 mPid;
    Boolean mStarting;                  // True if the process is being started
    Int64 mLastActivityTime;    // For managing the LRU list
    Int64 mLruWeight;                   // Weight for ordering in LRU list
    Int32 mMaxAdj;                  // Maximum OOM adjustment for this process
    Int32 mHiddenAdj;               // If hidden, this is the adjustment to use
    Int32 mClientHiddenAdj;        // If empty but hidden client, this is the adjustment to use
    Int32 mEmptyAdj;               // If empty, this is the adjustment to use
    Int32 mCurRawAdj;               // Current OOM unlimited adjustment for this process
    Int32 mSetRawAdj;               // Last set OOM unlimited adjustment for this process
    Int32 mNonStoppingAdj;         // Adjustment not counting any stopping activities
    Int32 mCurAdj;                  // Current OOM adjustment for this process
    Int32 mSetAdj;                  // Last set OOM adjustment for this process
    Int32 mCurSchedGroup;           // Currently desired scheduling class
    Int32 mSetSchedGroup;           // Last set to background scheduling class
    Int32 mTrimMemoryLevel;        // Last selected memory trimming level
    Int32 mMemImportance;          // Importance constant computed from curAdj
    Boolean mServiceb;           // Process currently is on the service B list
    Boolean mKeeping;               // Actively running code so don't kill due to that?
    Boolean mSetIsForeground;       // Running foreground UI when last set?
    Boolean mHasActivities;      // Are there any activities running in this process?
    Boolean mHasClientActivities;  // Are there any client services with activities?
    Boolean mForegroundServices;    // Running any services that are foreground?
    Boolean mForegroundActivities; // Running any activities that are foreground?
    Boolean mSystemNoUi;         // This is a system process, but not currently showing UI.
    Boolean mHasShownUi;         // Has UI been shown in this process since it was started?
    Boolean mPendingUiClean;     // Want to clean up resources from showing UI?
    Boolean mHasAboveClient;     // Bound using BIND_ABOVE_CLIENT, so want to be lower
    Boolean mBad;                   // True if disabled in the bad process list
    Boolean mKilledBackground;   // True when proc has been killed due to too many bg
    String mWaitingToKill;       // Process is waiting to be killed when in the bg; reason
    AutoPtr<IBinder> mForcingToForeground; // Token that is forcing this process to be foreground
    Int32 mAdjSeq;                 // Sequence id for identifying oom_adj assignment cycles
    Int32 mLruSeq;                  // Sequence id for identifying LRU update cycles
    AutoPtr<ICompatibilityInfo> mCompat;   // last used compatibility mode
    AutoPtr<IProxyDeathRecipient> mDeathRecipient; // Who is watching for the death.
    AutoPtr<IComponentName> mInstrumentationClass;// class installed to instrument app
    AutoPtr<IApplicationInfo> mInstrumentationInfo; // the application being instrumented
    String mInstrumentationProfileFile; // where to save profiling
    AutoPtr<IInstrumentationWatcher> mInstrumentationWatcher; // who is waiting
    AutoPtr<IBundle> mInstrumentationArguments; // as given to us
    AutoPtr<IComponentName> mInstrumentationResultClass;// copy of instrumentationClass
    Boolean mUsingWrapper;       // Set to true when process was launched with a wrapper attached
    AutoPtr<BroadcastRecord> mCurReceiver; // receiver currently running in the app
    Int64 mLastWakeTime;          // How long proc held wake lock at last check
    Int64 mLastCpuTime;           // How long proc has run CPU at last check
    Int64 mCurCpuTime;            // How long proc has run CPU most recently
    Int64 mLastRequestedGc; // When we last asked the app to do a gc
    Int64 mLastLowMemory;   // When we last told the app that memory is low
    Boolean mReportLowMemory;    // Set to true when waiting to report low mem
    Boolean mEmpty;              // Is this an empty background process?
    Boolean mHidden;             // Is this a hidden process?
    Int32 mLastPss;                // Last pss size reported by app.
    String mAdjType;             // Debugging: primary thing impacting oom_adj.
    Int32 mAdjTypeCode;            // Debugging: adj code to report to app.
    AutoPtr<IInterface> mAdjSource;   // Debugging: option dependent object.
    Int32 mAdjSourceOom;           // Debugging: oom_adj of adjSource's process.
    AutoPtr<IInterface> mAdjTarget;   // Debugging: target component impacting oom_adj.

    // contains HistoryRecord objects
    List< AutoPtr<ActivityRecord> > mActivities;
    // all ServiceRecord running in this process
    HashSet< AutoPtr<CServiceRecord> > mServices;
    // services that are currently executing code (need to remain foreground).
    HashSet< AutoPtr<CServiceRecord> > mExecutingServices;
    // All ConnectionRecord this process holds
    HashSet< AutoPtr<ConnectionRecord> > mConnections;
    // all IIntentReceivers that are registered from this process.
    HashSet< AutoPtr<ReceiverList> > mReceivers;
    // class (String) -> ContentProviderRecord
    HashMap<String, AutoPtr<ContentProviderRecord> > mPubProviders;
    // All ContentProviderRecord process is using
    List< AutoPtr<CContentProviderConnection> > mConProviders;

    Boolean mPersistent;         // always keep this application running?
    Boolean mCrashing;           // are we in the process of crashing?
    AutoPtr<IDialog> mCrashDialog;         // dialog being displayed due to crash.
    Boolean mNotResponding;      // does the app have a not responding dialog?
    AutoPtr<IDialog> mAnrDialog;           // dialog being displayed due to app not resp.
    Boolean mRemoved;            // has app package been removed from device?
    Boolean mDebugging;          // was app launched for debugging?
    Boolean mWaitedForDebugger;  // has process show wait for debugger dialog?
    AutoPtr<IDialog> mWaitDialog;          // current wait for debugger dialog

    String mShortStringName;     // caching of toShortString() result.
    String mStringName;          // caching of toString() result.

    // These reports are generated & stored when an app gets into an error condition.
    // They will be "null" when all is OK.
    AutoPtr<IActivityManagerProcessErrorStateInfo> mCrashingReport;
    AutoPtr<IActivityManagerProcessErrorStateInfo> mNotRespondingReport;

    // Who will be notified of the error. This is usually an activity in the
    // app that installed the package.
    AutoPtr<IComponentName> mErrorReportReceiver;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos


#endif //__PROCESSRECORD_H__
