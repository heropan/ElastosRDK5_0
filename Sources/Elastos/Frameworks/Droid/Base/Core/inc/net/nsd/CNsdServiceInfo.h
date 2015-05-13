#ifndef __CNSDSERVICEINFO_H__
#define __CNSDSERVICEINFO_H__

#include "_CNsdServiceInfo.h"

using Elastos::Net::IInetAddress;
using Elastos::Net::EIID_IInetAddress;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

CarClass(CNsdServiceInfo)
{
public:
    CNsdServiceInfo();

    ~CNsdServiceInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& sn,
        /* [in] */ const String& rt,
        /* [in] */ IDnsSdTxtRecord* tr);

    /** Get the service name */
    CARAPI GetServiceName(
        /* [out] */ String* name);

    /** Set the service name */
    CARAPI SetServiceName(
        /* [in] */ const String& s);

    /** Get the service type */
    CARAPI GetServiceType(
        /* [out] */ String* type);

    /** Set the service type */
    CARAPI SetServiceType(
        /* [in] */ const String& s);

    /** @hide */
    CARAPI GetTxtRecord(
        /* [out] */ IDnsSdTxtRecord** rec);

    /** @hide */
    CARAPI SetTxtRecord(
        /* [in] */ IDnsSdTxtRecord* t);

    /** Get the host address. The host address is valid for a resolved service. */
    CARAPI GetHost(
        /* [out] */ IInetAddress** host);

    /** Set the host address */
    CARAPI SetHost(
        /* [in] */ IInetAddress* s);

    /** Get port number. The port number is valid for a resolved service. */
    CARAPI GetPort(
        /* [out] */ Int32* port);

    /** Set port number */
    CARAPI SetPort(
        /* [in] */ Int32 p);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI ToString(
        /* [out] */ String* info);
public:
    String mServiceName;

    String mServiceType;

    AutoPtr<IDnsSdTxtRecord> mTxtRecord;

    AutoPtr<IInetAddress> mHost;

    Int32 mPort;
};

}
}
}
}

#endif // __CNSDSERVICEINFO_H__
