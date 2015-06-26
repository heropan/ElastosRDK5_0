
#ifndef __ORG_APACHE_HTTP_CONN_UTIL_INETADDRESSUTILS_H__
#define __ORG_APACHE_HTTP_CONN_UTIL_INETADDRESSUTILS_H__

#include <Org.Apache.Http_server.h>

using Elastos::Utility::Regex::IPattern;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Util {

/**
 * A collection of utilities relating to InetAddresses.
 */
class InetAddressUtils
{
private:
    InetAddressUtils() {}

public:
    CARAPI IsIPv4Address(
        /* [in] */ const String& input,
        /* [out] */ Boolean* result);

    CARAPI IsIPv6StdAddress(
        /* [in] */ const String& input,
        /* [out] */ Boolean* result);

    CARAPI IsIPv6HexCompressedAddress(
        /* [in] */ const String& input,
        /* [out] */ Boolean* result);

    CARAPI IsIPv6Address(
        /* [in] */ const String& input,
        /* [out] */ Boolean* result);

private:
    static const AutoPtr<IPattern> IPV4_PATTERN;

    static const AutoPtr<IPattern> IPV6_STD_PATTERN;

    static const AutoPtr<IPattern> IPV6_HEX_COMPRESSED_PATTERN;
};

} // namespace Util
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CONN_UTIL_INETADDRESSUTILS_H__
