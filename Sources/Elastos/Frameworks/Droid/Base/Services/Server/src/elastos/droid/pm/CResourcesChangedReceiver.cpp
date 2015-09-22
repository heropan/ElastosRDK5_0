
#include "pm/CResourcesChangedReceiver.h"
#include "os/Handler.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

ECode CResourcesChangedReceiver::constructor(
    /* [in] */ IIPackageManager* owner,
    /* [in] */ ISet* keys,
    /* [in] */ Boolean reportStatus)
{
    mOwner = (CPackageManagerService*)owner;
    mKeys = keys;
    mReportStatus = reportStatus;
    return NOERROR;
}

CResourcesChangedReceiver::~CResourcesChangedReceiver()
{
}

ECode CResourcesChangedReceiver::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    AutoPtr<IMessage> msg;
    mOwner->mHandler->ObtainMessage(CPackageManagerService::UPDATED_MEDIA_STATUS, (IMessage**)&msg);
    msg->SetObj(mKeys);
    msg->SetArg1(mReportStatus ? 1 : 0);
    msg->SetArg2(1);
    Boolean result;
    return mOwner->mHandler->SendMessage(msg, &result);
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
