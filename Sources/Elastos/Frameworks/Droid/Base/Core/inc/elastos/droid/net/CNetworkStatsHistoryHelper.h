
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATSHISTORYHELPER_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATSHISTORYHELPER_H__

#include "_Elastos_Droid_Net_CNetworkStatsHistoryHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsHistoryHelper)
{
public:
    CARAPI RandomLong(
        /* [in] */ Elastos::Core::IRandom * pR,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [out] */ Int64 * result);
};

}
}
}

#endif // __ELASTOS_DROID_NET_CNETWORKSTATSHISTORYHELPER_H__
