
#include "net/CNetworkStatsHelper.h"
#include "net/CNetworkStats.h"

namespace Elastos {
namespace Droid {
namespace Net {


ECode CNetworkStatsHelper::SetToString(
    /* [in] */ Int32 set,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkStats::SetToString(set, result);
}

ECode CNetworkStatsHelper::TagToString(
    /* [in] */ Int32 tag,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkStats::TagToString(tag, result);
}

ECode CNetworkStatsHelper::Subtract(
    /* [in] */ INetworkStats* left,
    /* [in] */ INetworkStats* right,
    /* [in] */ INonMonotonicObserver* observer,
    /* [in] */ IInterface* cookie,
    /* [out] */ INetworkStats** result)
{
    VALIDATE_NOT_NULL(result);
    return CNetworkStats::Subtract(left, right, observer, cookie, result);
}
}
}
}

