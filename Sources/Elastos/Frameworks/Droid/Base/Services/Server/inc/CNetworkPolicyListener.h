
#ifndef __CNETWORKPOLICYLISTENER_H__
#define __CNETWORKPOLICYLISTENER_H__

#include "_CNetworkPolicyListener.h"

namespace Elastos {
namespace Droid {
namespace Server {

class CConnectivityService;

CarClass(CNetworkPolicyListener)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 owner);

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

#endif // __CNETWORKPOLICYLISTENER_H__
