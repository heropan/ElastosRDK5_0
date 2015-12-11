
#ifndef __ELASTOS_DROID_SERVER_AM_CACTIVITYRECORDTOKEN_H__
#define __ELASTOS_DROID_SERVER_AM_CACTIVITYRECORDTOKEN_H__

#include "_Elastos_Droid_Server_Am_CActivityRecordToken.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ActivityRecord;

CarClass(CActivityRecordToken)
{
public:
    CActivityRecordToken();

    ~CActivityRecordToken();

    CARAPI constructor(
        /* [in] */ Handle32 activity);

    CARAPI WindowsDrawn();

    CARAPI WindowsVisible();

    CARAPI WindowsGone();

    CARAPI KeyDispatchingTimedOut(
        /* [in] */ const String& reason,
        /* [out] */ Boolean* result);

    CARAPI GetKeyDispatchingTimeout(
        /* [out] */ Int64* timeout);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI_(AutoPtr<ActivityRecord>) GetActivityRecord();

public:
    AutoPtr<IWeakReference> mWeakActivity; // WeakReference<ActivityRecord>
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_CACTIVITYRECORDTOKEN_H__
