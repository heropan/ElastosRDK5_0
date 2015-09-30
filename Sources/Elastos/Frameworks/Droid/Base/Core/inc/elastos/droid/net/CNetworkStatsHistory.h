
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATSHISTORY_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATSHISTORY_H__

#include "_Elastos_Droid_Net_CNetworkStatsHistory.h"
#include "elastos/droid/net/NetworkStatsHistory.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Collection of historical network statistics, recorded into equally-sized
 * "buckets" in time. Internally it stores data in {@code long} series for more
 * efficient persistence.
 * <p>
 * Each bucket is defined by a {@link #bucketStart} timestamp, and lasts for
 * {@link #bucketDuration}. Internally assumes that {@link #bucketStart} is
 * sorted at all times.
 *
 * @hide
 */
CarClass(CNetworkStatsHistory)
    , public NetworkStatsHistory
{
public:
    CAR_OBJECT_DECL()

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_CNETWORKSTATSHISTORY_H__
