
#ifndef __CNETWORKSTATSHISTORYHELPER_H__
#define __CNETWORKSTATSHISTORYHELPER_H__

#include "_CNetworkStatsHistoryHelper.h"

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

#endif // __CNETWORKSTATSHISTORYHELPER_H__
