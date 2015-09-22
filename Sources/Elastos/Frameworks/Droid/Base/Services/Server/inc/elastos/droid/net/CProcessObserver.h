
#ifndef __ELASTOS_DROID_SERVER_NET_CPROCESSOBSERVER_H__
#define __ELASTOS_DROID_SERVER_NET_CPROCESSOBSERVER_H__

#include "_Elastos_Droid_Server_Net_CProcessObserver.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

class CNetworkPolicyManagerService;

CarClass(CProcessObserver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 owner);

    CARAPI OnForegroundActivitiesChanged(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean foregroundActivities);

    CARAPI OnImportanceChanged(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 importance);

    CARAPI OnProcessDied(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid);

private:
    CNetworkPolicyManagerService* mOwner;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_NET_CPROCESSOBSERVER_H__
