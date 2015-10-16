
#ifndef __ELASTOS_DROID_SERVER_AM_BROADCASTRECORD_H__
#define __ELASTOS_DROID_SERVER_AM_BROADCASTRECORD_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "am/BroadcastFilter.h"
#include "am/BroadcastQueue.h"
#include <Elastos.Droid.Core.h>
#include <elastos/utility/etl/List.h>


using Elastos::Utility::Etl::List;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ProcessRecord;
class BroadcastFilter;
class BroadcastQueue;

class BroadcastFilterList
     : public List< AutoPtr<IInterface> >
{
};

/**
 * An active intent broadcast.
 */
class BroadcastRecord : public ElRefBase
{
public:
    BroadcastRecord(
        /* [in] */ BroadcastQueue* queue,
        /* [in] */ IIntent* intent,
        /* [in] */ ProcessRecord* callerApp,
        /* [in] */ const String& callerCapsule,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& requiredPermission,
        /* [in] */ BroadcastFilterList* receivers,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* resultExtras,
        /* [in] */ Boolean serialized,
        /* [in] */ Boolean sticky,
        /* [in] */ Boolean initialSticky,
        /* [in] */ Int32 userId);

    ~BroadcastRecord();

public:
    CARAPI_(String) ToString();

public:
    static const Int32 IDLE = 0;
    static const Int32 APP_RECEIVE = 1;
    static const Int32 CALL_IN_RECEIVE = 2;
    static const Int32 CALL_DONE_RECEIVE = 3;

    AutoPtr<IIntent> mIntent;       // the original intent that generated us
    AutoPtr<ProcessRecord> mCallerApp;      // process that sent this
    String mCallerPackage;          // who sent this
    Int32 mCallingPid;              // the pid of who sent this
    Int32 mCallingUid;              // the uid of who sent this
    Boolean mOrdered;               // serialize the send to receivers?
    Boolean mSticky;                // originated from existing sticky data?
    Boolean mInitialSticky;         // initial broadcast from register to sticky?
    Int32 mUserId;       // user id this broadcast was for
    String mRequiredPermission;     // a permission the caller has required
    AutoPtr<BroadcastFilterList> mReceivers;  // contains BroadcastFilter and ResolveInfo
    AutoPtr<IIntentReceiver> mResultTo; // who receives final result if non-null
    Int64 mDispatchTime;      // when dispatch started on this set of receivers
    Int64 mDispatchClockTime; // the clock time the dispatch started
    Int64 mReceiverTime;      // when current receiver started for timeouts.
    Int64 mFinishTime;        // when we finished the broadcast.
    Int32 mResultCode;         // current result code value.
    String mResultData;      // current result data value.
    AutoPtr<IBundle> mResultExtras;    // current result extra data values.
    Boolean mResultAbort;    // current result abortBroadcast value.
    Int32 mNextReceiver; // next receiver to be executed.
    AutoPtr<IBinder> mReceiver;// who is currently running, null if none.
    Int32 mState;
    Int32 mAnrCount;           // has this broadcast record hit any ANRs?
    BroadcastQueue* mQueue;   // the outbound queue handling this broadcast

    // The following are set when we are calling a receiver (one that
    // was found in our list of registered receivers).
    AutoPtr<BroadcastFilter> mCurFilter;

    AutoPtr<ProcessRecord> mCurApp;         // hosting application of current receiver.
    AutoPtr<IComponentName> mCurComponent;   // the receiver class that is currently running.
    AutoPtr<IActivityInfo> mCurReceiver;   // info about the receiver that is currently running.
};


} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_BROADCASTRECORD_H__
