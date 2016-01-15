
#ifndef __ELASTOS_DROID_SERVER_AM_CBOOTCOMPLETEDRECEIVER_H__
#define __ELASTOS_DROID_SERVER_AM_CBOOTCOMPLETEDRECEIVER_H__

#include "_Elastos_Droid_Server_Am_CBootCompletedReceiver.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentReceiver;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class CActivityManagerService;

CarClass(CBootCompletedReceiver)
    , public Object
    , public IIntentReceiver
    , public IBinder
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 host);

    CARAPI PerformReceive(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 sendingUser);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    CActivityManagerService* mHost;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_AM_CBOOTCOMPLETEDRECEIVER_H__
