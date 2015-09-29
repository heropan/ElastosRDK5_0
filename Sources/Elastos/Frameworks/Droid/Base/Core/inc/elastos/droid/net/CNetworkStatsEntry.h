
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATSENTRY_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATSENTRY_H__

#include "_Elastos_Droid_Net_CNetworkStatsEntry.h"
#include "elastos/droid/net/NetworkStatsEntry.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsEntry)
    , public NetworkStatsEntry
{
public:
    CAR_OBJECT_DECL()

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_CNETWORKSTATSENTRY_H__
