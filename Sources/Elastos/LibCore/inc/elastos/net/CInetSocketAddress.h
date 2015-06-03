
#ifndef __CINETSOCKETADDRESS_H__
#define __CINETSOCKETADDRESS_H__

#include "_CInetSocketAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInetSocketAddress)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ const String& hostname,
        /* [in] */ Int32 port,
        /* [in] */ Boolean needResolved);

    static CARAPI CreateUnresolved(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ IInetSocketAddress** addr);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI SetPort(
        /* [in] */ Int32 port);

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI SetAddress(
        /* [in] */ IInetAddress* address);

    CARAPI GetHostName(
        /* [out] */ String* hostname);

    CARAPI GetHostString(
        /* [out] */ String* hostString);

    CARAPI IsUnresolved(
        /* [out] */ Boolean* isUnresolved);

    CARAPI Equals(
        /*[in] */ IInterface* socketAddr,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);


public:
    AutoPtr<IInetAddress> mAddr;

    Int32 mPort;

private:
    static const Int64 sSerialVersionUID;

    String mHostname;
};

} // namespace Net
} // namespace Elastos

#endif //__CINETSOCKETADDRESS_H__
