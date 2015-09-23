
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATSHELPER_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATSHELPER_H__

#include "_Elastos_Droid_Net_CNetworkStatsHelper.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsHelper)
{
public:

    CARAPI SetToString(
        /* [in] */ Int32 set,
        /* [out] */ String* result);

    CARAPI TagToString(
        /* [in] */ Int32 tag,
        /* [out] */ String* result);

    CARAPI Subtract(
        /* [in] */ INetworkStats* left,
        /* [in] */ INetworkStats* right,
        /* [in] */ INonMonotonicObserver* observer,
        /* [in] */ IInterface* cookie,
        /* [out] */ INetworkStats** result);
};

}
}
}

#endif // __CNETWORKSTATSHISTORYHELPER_H__
