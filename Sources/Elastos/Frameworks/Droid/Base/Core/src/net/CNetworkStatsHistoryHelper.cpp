
#include "net/CNetworkStatsHistoryHelper.h"
#include "net/CNetworkStatsHistory.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkStatsHistoryHelper::RandomLong(
    /* [in] */ Elastos::Core::IRandom * random,
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [out] */ Int64 * result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkStatsHistory::RandomLong(random, start, end, result);
}

}
}
}

