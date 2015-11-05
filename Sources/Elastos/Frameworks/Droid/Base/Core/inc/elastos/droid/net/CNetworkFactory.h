
#ifndef __ELASTOS_DROID_NET_CNETWORKFACTORY_H__
#define __ELASTOS_DROID_NET_CNETWORKFACTORY_H__

#include "_Elastos_Droid_Net_CNetworkFactory.h"
#include "elastos/droid/net/NetworkFactory.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A NetworkFactory is an entity that creates NetworkAgent objects.
 * The bearers register with ConnectivityService using {@link #register} and
 * their factory will start receiving scored NetworkRequests.  NetworkRequests
 * can be filtered 3 ways: by NetworkCapabilities, by score and more complexly by
 * overridden function.  All of these can be dynamic - changing NetworkCapabilities
 * or score forces re-evaluation of all current requests.
 *
 * If any requests pass the filter some overrideable functions will be called.
 * If the bearer only cares about very simple start/stopNetwork callbacks, those
 * functions can be overridden.  If the bearer needs more interaction, it can
 * override addNetworkRequest and removeNetworkRequest which will give it each
 * request that passes their current filters.
 * @hide
 **/
CarClass(CNetworkFactory)
    , public NetworkFactory
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_CNETWORKFACTORY_H__
