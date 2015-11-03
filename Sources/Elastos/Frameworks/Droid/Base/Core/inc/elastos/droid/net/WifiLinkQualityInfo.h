
#ifndef __ELASTOS_DROID_NET_WIFILINKQUALITYINFO_H__
#define __ELASTOS_DROID_NET_WIFILINKQUALITYINFO_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/LinkQualityInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 *  Class that represents useful attributes of wifi network links
 *  such as the upload/download throughput or error rate etc.
 *  @hide
 */
class WifiLinkQualityInfo
    : public LinkQualityInfo
    , public IWifiLinkQualityInfo
{
public:
    CAR_INTERFACE_DECL()

    WifiLinkQualityInfo();

    CARAPI constructor();

    /**
     * @hide
     */
    static CARAPI CreateFromParcelBody(
        /* [in] */ IParcel* in,
        /* [out] */ IWifiLinkQualityInfo** result);

    /**
     * returns Wifi network type
     * @return network type or {@link android.net.LinkQualityInfo#UNKNOWN_INT}
     */
    CARAPI GetType(
        /* [out] */ Int32* result);

    /**
     * @hide
     */
    CARAPI SetType(
        /* [in] */ Int32 type);

    /**
     * returns BSSID of the access point
     * @return the BSSID, in the form of a six-byte MAC address: {@code XX:XX:XX:XX:XX:XX} or null
     */
    CARAPI GetBssid(
        /* [out] */ String* result);

    /**
     * @hide
     */
    CARAPI SetBssid(
        /* [in] */ const String& bssid);

    /**
     * returns RSSI of the network in raw form
     * @return un-normalized RSSI or {@link android.net.LinkQualityInfo#UNKNOWN_INT}
     */
    CARAPI GetRssi(
        /* [out] */ Int32* result);

    /**
     * @hide
     */
    CARAPI SetRssi(
        /* [in] */ Int32 rssi);

    /**
     * returns number of packets transmitted without error
     * @return number of packets or {@link android.net.LinkQualityInfo#UNKNOWN_LONG}
     */
    CARAPI GetTxGood(
        /* [out] */ Int64* result);

    /**
     * @hide
     */
    CARAPI SetTxGood(
        /* [in] */ Int64 txGood);

    /**
     * returns number of transmitted packets that encountered errors
     * @return number of packets or {@link android.net.LinkQualityInfo#UNKNOWN_LONG}
     */
    CARAPI GetTxBad(
        /* [out] */ Int64* result);

    /**
     * @hide
     */
    CARAPI SetTxBad(
        /* [in] */ Int64 txBad);

private:
    /* Indicates Wifi network type such as b/g etc*/
    Int32 mType;

    String mBssid;

    /* Rssi found by scans */
    Int32 mRssi;

    /* packet statistics */
    Int64 mTxGood;

    Int64 mTxBad;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_WIFILINKQUALITYINFO_H__
