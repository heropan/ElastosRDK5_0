
#ifndef __CWIFIP2PSERVICERESPONSEBASE_H__
#define __CWIFIP2PSERVICERESPONSEBASE_H__

#include "ext/frameworkext.h"

using Elastos::Utility::IObjectStringMap;
using Elastos::Droid::Net::Wifi::P2p::IWifiP2pDevice;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

extern "C" const InterfaceID EIID_WifiP2pServiceResponse;

#define MAX_BUF_SIZE (1024)

/**
 * The class for a response of service discovery.
 *
 * @hide
 */
class WifiP2pServiceResponse
{
public:
    WifiP2pServiceResponse();

    CARAPI Init(
        /* [in] */ Int32 serviceType,
        /* [in] */ Int32 status,
        /* [in] */ Int32 transId,
        /* [in] */ IWifiP2pDevice* device,
        /* [in] */ ArrayOf<Byte>* data);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI GetServiceType(
        /* [out] */ Int32* serviceType);

    virtual CARAPI GetStatus(
        /* [out] */ Int32* status);

    virtual CARAPI GetTransactionId(
        /* [out] */ Int32 * pTransactionId);

    virtual CARAPI GetRawData(
        /* [out, callee] */ ArrayOf<Byte>** rawData);

    virtual CARAPI GetSrcDevice(
        /* [out] */ IWifiP2pDevice** srcDevice);

    virtual CARAPI SetSrcDevice(
        /* [in] */ IWifiP2pDevice* dev);

    virtual CARAPI ToString(
        /* [out] */ String* string);

    virtual CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    static CARAPI_(String) StatusToString(
        /* [in] */ Int32 status);

protected:
    Boolean Equals(
        /* [in] */ const String& a,
        /* [in] */ const String& b);

    /**
     * Converts hex string to byte array.
     *
     * @param hex hex string. if invalid, return null.
     * @return binary data.
     */
    static ECode HexStr2Bin(
        /* [in] */ const String& hex,
        /* [out] */ ArrayOf<Byte>** array);

protected:
    /**
     * Service type. It's defined in table63 in Wi-Fi Direct specification.
     */
    Int32 mServiceType;

    /**
     * Status code of service discovery response.
     * It's defined in table65 in Wi-Fi Direct specification.
     * @see Status
     */
    Int32 mStatus;

    /**
     * Service transaction ID.
     * This is a nonzero value used to match the service request/response TLVs.
     */
    Int32 mTransId;

    /**
     * Source device.
     */
    AutoPtr<IWifiP2pDevice> mDevice;

    /**
     * Service discovery response data based on the requested on
     * the service protocol type. The protocol format depends on the service type.
     */
    AutoPtr<ArrayOf<Byte> > mData;
};

}
}
}
}
}
}
#endif // __CWIFIP2PSERVICERESPONSEBASE_H__
