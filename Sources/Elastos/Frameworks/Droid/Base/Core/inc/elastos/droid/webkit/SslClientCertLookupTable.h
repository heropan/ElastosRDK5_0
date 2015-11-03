
#ifndef __ELASTOS_DROID_WEBKIT_SSLCLIENTCERTLOOKUPTABLE_H__
#define __ELASTOS_DROID_WEBKIT_SSLCLIENTCERTLOOKUPTABLE_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/Set.h>

using Elastos::Utility::Set;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * A simple class to store client certificates that user has chosen.
 */
class SslClientCertLookupTable : public Object
{
public:
    //synchronized
    static CARAPI_(AutoPtr<SslClientCertLookupTable>) GetInstance();

//    CARAPI_(void) Allow(
//        /* [in] */ const String& host_and_port,
//        /* [in] */ IPrivateKey privateKey*,
//        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chain);

    CARAPI_(void) Deny(
        /* [in] */ const String& host_and_port);

    CARAPI_(Boolean) IsAllowed(
        /* [in] */ const String& host_and_port);

    CARAPI_(Boolean) IsDenied(
        /* [in] */ const String& host_and_port);

//    CARAPI_(AutoPtr<IPrivateKey>) PrivateKey(
//        /* [in] */ const String& host_and_port);

    CARAPI_(AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > >) CertificateChain(
        /* [in] */ const String& host_and_port);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    SslClientCertLookupTable();

private:
    static AutoPtr<SslClientCertLookupTable> sTable;
    static Object sLock;
    AutoPtr<IObjectStringMap> privateKeys;
    AutoPtr<IObjectStringMap> certificateChains;
    Set<String> denied;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_SSLCLIENTCERTLOOKUPTABLE_H__
