
#ifndef __CRESOURCESCHANGEDRECEIVER_H__
#define __CRESOURCESCHANGEDRECEIVER_H__

#include "_CResourcesChangedReceiver.h"
#include "ext/frameworkext.h"
#include "pm/CPackageManagerService.h"

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IIPackageManager;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

CarClass(CResourcesChangedReceiver)
{
public:
    constructor(
        /* [in] */ IIPackageManager* owner,
        /* [in] */ ISet* keys,
        /* [in] */ Boolean reportStatus);

    ~CResourcesChangedReceiver();

    CARAPI PerformReceive(
       /* [in] */ IIntent* intent,
       /* [in] */ Int32 resultCode,
       /* [in] */ const String& data,
       /* [in] */ IBundle* extras,
       /* [in] */ Boolean ordered,
       /* [in] */ Boolean sticky,
       /* [in] */ Int32 sendingUser);

private:
    CPackageManagerService* mOwner;
    AutoPtr<ISet> mKeys; //Set< AutoPtr<CPackageManagerService::AsecInstallArgs> >
    Boolean mReportStatus;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __CRESOURCESCHANGEDRECEIVER_H__
