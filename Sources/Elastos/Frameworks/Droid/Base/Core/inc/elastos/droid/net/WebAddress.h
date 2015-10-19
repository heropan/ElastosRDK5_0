
#ifndef __ELASTOS_DROID_NET_WEBADDRESS_H__
#define __ELASTOS_DROID_NET_WEBADDRESS_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Utility::Regex::IPattern;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * {@hide}
 *
 * Web Address Parser
 *
 * This is called WebAddress, rather than URL or URI, because it
 * attempts to parse the stuff that a user will actually type into a
 * browser address widget.
 *
 * Unlike java.net.uri, this parser will not choke on URIs missing
 * schemes.  It will only throw a ParseException if the input is
 * really hosed.
 *
 * If given an https scheme but no port, fills in port
 *
 */
class WebAddress
    : public Object
    , public IWebAddress
{
public:
    CAR_INTERFACE_DECL()

    /** parses given uriString. */
    CARAPI constructor(
        /* [in] */ const String& address);

    // @Override
    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI SetScheme(
        /* [in] */ const String& scheme);

    CARAPI GetScheme(
        /* [out] */ String* result);

    CARAPI SetHost(
        /* [in] */ const String& host);

    CARAPI GetHost(
        /* [out] */ String* result);

    CARAPI SetPort(
        /* [in] */ Int32 port);

    CARAPI GetPort(
        /* [out] */ Int32* result);

    CARAPI SetPath(
        /* [in] */ const String& path);

    CARAPI GetPath(
        /* [out] */ String* result);

    CARAPI SetAuthInfo(
        /* [in] */ const String& authInfo);

    CARAPI GetAuthInfo(
        /* [out] */ String* result);

    CARAPI GetAddressPattern(
        /* [out] */ IPattern** result);

    CARAPI SetAddressPattern(
        /* [in] */ IPattern* addressPattern);

    static const Int32 MATCH_GROUP_SCHEME;

    static const Int32 MATCH_GROUP_AUTHORITY;

    static const Int32 MATCH_GROUP_HOST;

    static const Int32 MATCH_GROUP_PORT;

    static const Int32 MATCH_GROUP_PATH;

private:
    static CARAPI_(AutoPtr<IPattern>) CreateAddressPattern();

    static AutoPtr<IPattern> sAddressPattern;

    String mScheme;

    String mHost;

    Int32 mPort;

    String mPath;

    String mAuthInfo;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_WEBADDRESS_H__

#if 0 // old WebAddress.h
#ifndef __ELASTOS_DROID_NET_WEBADDRESS_H__
#define __ELASTOS_DROID_NET_WEBADDRESS_H__

#include <elastos/AutoPtr.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * {@hide}
 *
 * Web Address Parser
 *
 * This is called WebAddress, rather than URL or URI, because it
 * attempts to parse the stuff that a user will actually type into a
 * browser address widget.
 *
 * Unlike java.net.uri, this parser will not choke on URIs missing
 * schemes.  It will only throw a ParseException if the input is
 * really hosed.
 *
 * If given an https scheme but no port, fills in port
 *
 */
class WebAddress
{
public:
    WebAddress(
        /* [in] */ const String& address);

    CARAPI_(String) ToString();

    CARAPI_(void) SetScheme(
        /* [in] */ const String& scheme);

    CARAPI_(String) GetScheme();

    CARAPI_(void) SetHost(
        /* [in] */ const String& host);

    CARAPI_(String) GetHost();

    CARAPI_(void) SetPort(
        /* [in] */ Int32 port);

    CARAPI_(Int32) GetPort();

    CARAPI_(void) SetPath(
        /* [in] */ const String& path);

    CARAPI_(String) GetPath();

    CARAPI_(void) SetAuthInfo(
        /* [in] */ const String& authInfo);

    CARAPI_(String) GetAuthInfo();

public:
    static const Int32 MATCH_GROUP_SCHEME;
    static const Int32 MATCH_GROUP_AUTHORITY;
    static const Int32 MATCH_GROUP_HOST;
    static const Int32 MATCH_GROUP_PORT;
    static const Int32 MATCH_GROUP_PATH;

private:
    static AutoPtr<IPattern> sAddressPattern;

    String mScheme;
    String mHost;
    Int32 mPort;
    String mPath;
    String mAuthInfo;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_WEBADDRESS_H__
#endif