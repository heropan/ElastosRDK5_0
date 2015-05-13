
#ifndef __TASKRECORD_H__
#define __TASKRECORD_H__

#include "am/ThumbnailHolder.h"

using Elastos::IO::IPrintWriter;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class TaskRecord : public ThumbnailHolder
{
public:
    TaskRecord(
        /* [in] */ Int32 taskId,
        /* [in] */ IActivityInfo* info,
        /* [in] */ IIntent* intent);

    ~TaskRecord();

    CARAPI_(Int64) TouchActiveTime();

    CARAPI_(Int64) GetInactiveDuration();

    CARAPI SetIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info);

    CARAPI_(void) Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix);

    CARAPI_(String) ToString();

public:
    Int32 mTaskId;       // Unique identifier for this task.
    String mAffinity;  // The affinity name for this task, or null.
    AutoPtr<IIntent> mIntent;          // The original intent that started the task.
    AutoPtr<IIntent> mAffinityIntent;  // Intent of affinity-moved activity that started this task.
    AutoPtr<IComponentName> mOrigActivity; // The non-alias activity component of the intent.
    AutoPtr<IComponentName> mRealActivity; // The actual activity component that started the task.
    Int32 mNumActivities;      // Current number of activities in this task.
    Millisecond64 mLastActiveTime;    // Last time this task was active, including sleep.
    Boolean mRootWasReset;   // True if the intent at the root of the task had
                        // the FLAG_ACTIVITY_RESET_TASK_IF_NEEDED flag.
    Boolean mAskedCompatMode;// Have asked the user about compat mode for this task.
    String mStringName;      // caching of toString() result.
    Int32 mUserId;             // user for which this task was created
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__TASKRECORD_H__
