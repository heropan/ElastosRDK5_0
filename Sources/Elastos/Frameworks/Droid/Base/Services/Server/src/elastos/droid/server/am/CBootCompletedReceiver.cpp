
#include "elastos/droid/server/am/CBootCompletedReceiver.h"
#include "elastos/droid/server/am/CActivityManagerService.h"
#include <elastos/droid/os/SystemClock.h>
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Content::EIID_IIntentReceiver;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

CAR_INTERFACE_IMPL_2(CBootCompletedReceiver, Object, IIntentReceiver, IBinder)
CAR_OBJECT_IMPL(CBootCompletedReceiver)

ECode CBootCompletedReceiver::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CActivityManagerService*)host;
    return NOERROR;
}

ECode CBootCompletedReceiver::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    synchronized (mHost) {
        // mHost->RequestPssAllProcsLocked(SystemClock::GetUptimeMillis(),
        //     TRUE, FALSE);
        assert(0);
    }
    return NOERROR;
}

ECode CBootCompletedReceiver::ToString(
    /* [out] */ String* str)
{
    return Object::ToString(str);
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos