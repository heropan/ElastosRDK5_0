
#ifndef __ELASTOS_DROID_NET_WIFI_CRSSIPACKETCOUNTINFO_H__
#define __ELASTOS_DROID_NET_WIFI_CRSSIPACKETCOUNTINFO_H__

#include "_CRssiPacketCountInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CRssiPacketCountInfo)
{
public:
    CARAPI GetRssi(
        /* [out] */ Int32 * pRssi);

    CARAPI SetRssi(
        /* [in] */ Int32 rssi);

    CARAPI GetTxgood(
        /* [out] */ Int32 * pTxgood);

    CARAPI SetTxgood(
        /* [in] */ Int32 txgood);

    CARAPI GetTxbad(
        /* [out] */ Int32 * pTxbad);

    CARAPI SetTxbad(
        /* [in] */ Int32 txbad);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    Int32 mRssi;
    Int32 mTxgood;
    Int32 mTxbad;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_WIFI_CRSSIPACKETCOUNTINFO_H__
