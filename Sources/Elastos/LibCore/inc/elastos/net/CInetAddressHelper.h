#ifndef __CINETADDRESSHELPER_H__
#define __CINETADDRESSHELPER_H__

#include "_CInetAddressHelper.h"
#include "InetAddress.h"

namespace Elastos {
namespace Net {

CarClass(CInetAddressHelper)
{
public:
    CARAPI GetAllByName(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** interAddress);

    CARAPI GetByName(
        /* [in] */ const String& host,
        /* [out] */ IInetAddress** interAddr);

    CARAPI GetLocalHost(
        /* [out] */ IInetAddress** interAddr);

    CARAPI ClearDnsCache();

    CARAPI IsNumeric(
        /* [in] */ const String& address,
        /* [out] */ Boolean* isNumeric);

    CARAPI ParseNumericAddress(
        /* [in] */ const String& numericAddress,
        /* [out] */ IInetAddress** interAddr);

    CARAPI GetLoopbackAddress(
        /* [out] */ IInetAddress** interAddr);

    CARAPI GetByAddress(
        /* [in] */ ArrayOf<Byte>* ipAddress,
        /* [out] */ IInetAddress** interAddr);

    CARAPI GetByAddress(
        /* [in] */ const String& hostName,
        /* [in] */ ArrayOf<Byte>* ipAddress,
        /* [out] */ IInetAddress** interAddr);
};

} // namespace Net
} // namespace Elastos

#endif //__CINETADDRESSHELPER_H__
