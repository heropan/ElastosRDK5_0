
#ifndef __CSHUTDOWNRECEIVER_H__
#define __CSHUTDOWNRECEIVER_H__

#include "ext/frameworkext.h"
#include "_CShutdownReceiver.h"
#include "am/UserStartedState.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IIntent;

class CActivityManagerService;

CarClass(CShutdownReceiver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ Handle32 uss);

    CARAPI PerformReceive(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 sendingUser);

private:
    CActivityManagerService* mHost;
    AutoPtr<UserStartedState> mUss;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __CSHUTDOWNRECEIVER_H__