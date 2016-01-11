
#ifndef __ELASTOS_DROID_SERVER_CNETWORKPOLICYLISTENER_H__
#define __ELASTOS_DROID_SERVER_CNETWORKPOLICYLISTENER_H__

#include "_Elastos_Droid_Server_CNetworkPolicyListener.h"

namespace Elastos {
namespace Droid {
namespace Server {

class CConnectivityService;

CarClass(CNetworkPolicyListener)
    , public Object
    , public IINetworkPolicyListener
    , public IBinder
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ IIConnectivityManager* owner);

    CARAPI OnUidRulesChanged(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 uidRules);

    CARAPI OnMeteredIfacesChanged(
        /* [in] */ ArrayOf<String>* meteredIfaces);

    CARAPI OnRestrictBackgroundChanged(
        /* [in] */ Boolean restrictBackground);

private:
    CConnectivityService* mOwner;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_CNETWORKPOLICYLISTENER_H__
