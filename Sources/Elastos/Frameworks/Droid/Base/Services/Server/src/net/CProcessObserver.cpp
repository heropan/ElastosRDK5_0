
#include "net/CProcessObserver.h"
#include "net/CNetworkPolicyManagerService.h"
#include "os/Handler.h"

using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

ECode CProcessObserver::constructor(
    /* [in] */ Handle32 owner)
{
    mOwner = (CNetworkPolicyManagerService*)owner;
    return NOERROR;
}

ECode CProcessObserver::OnForegroundActivitiesChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean foregroundActivities)
{
    AutoPtr<IBoolean> obj;
    CBoolean::New(foregroundActivities, (IBoolean**)&obj);
    AutoPtr<IMessage> msg;
    mOwner->mHandler->ObtainMessage(
        CNetworkPolicyManagerService::MSG_FOREGROUND_ACTIVITIES_CHANGED,
        (IMessage**)&msg);
    msg->SetArg1(pid);
    msg->SetArg2(uid);
    msg->SetObj(obj);
    Boolean result;
    return mOwner->mHandler->SendMessage(msg, &result);
}

ECode CProcessObserver::OnImportanceChanged(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 importance)
{
    return NOERROR;
}

ECode CProcessObserver::OnProcessDied(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid)
{
    AutoPtr<IMessage> msg;
    mOwner->mHandler->ObtainMessage(
        CNetworkPolicyManagerService::MSG_PROCESS_DIED,
        (IMessage**)&msg);
    msg->SetArg1(pid);
    msg->SetArg2(uid);
    Boolean result;
    return mOwner->mHandler->SendMessage(msg, &result);
}

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos
