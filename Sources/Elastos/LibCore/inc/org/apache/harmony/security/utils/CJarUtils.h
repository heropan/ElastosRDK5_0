
#ifndef __ORG_APACHE_HARMONY_SECURITY_UTILS_CJARUTILS_H__
#define __ORG_APACHE_HARMONY_SECURITY_UTILS_CJARUTILS_H__

#include "_Org_Apache_Harmony_Security_Utils_CJarUtils.h"
#include "core/Singleton.h"

using Elastos::Core::Singleton;
using Elastos::IO::IInputStream;
using Elastos::Security::IPrincipal;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::Cert::IX509Certificate;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

CarClass(CJarUtils)
    , public Singleton
    , public IJarUtils
{
private:
    /**
     * For legacy reasons we need to return exactly the original encoded
     * certificate bytes, instead of letting the underlying implementation have
     * a shot at re-encoding the data.
     */
    // private static class VerbatimX509Certificate extends WrappedX509Certificate {
    //     private byte[] encodedVerbatim;

    //     public VerbatimX509Certificate(X509Certificate wrapped, byte[] encodedVerbatim) {
    //         super(wrapped);
    //         this.encodedVerbatim = encodedVerbatim;
    //     }

    //     @Override
    //     public byte[] getEncoded() throws CertificateEncodingException {
    //         return encodedVerbatim;
    //     }
    // };

public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI VerifySignature(
        /* [in] */ IInputStream* signature,
        /* [in] */ IInputStream* signatureBlock,
        /* [out, callee] */ ArrayOf<ICertificate*>** sign);

private:
    static CARAPI_(AutoPtr< ArrayOf<IX509Certificate*> >) CreateChain(
        /* [in] */ IX509Certificate* signer,
        /* [in] */ const ArrayOf<IX509Certificate*>& candidates);

    static CARAPI_(AutoPtr<IX509Certificate>) FindCert(
        /* [in] */ IPrincipal* issuer,
        /* [in] */ const ArrayOf<IX509Certificate*>& candidates);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) Init_MESSAGE_DIGEST_OID();

private:
    // as defined in PKCS #9: Selected Attribute Types:
    // http://www.ietf.org/rfc/rfc2985.txt
    static const AutoPtr< ArrayOf<Int32> > MESSAGE_DIGEST_OID;
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_UTILS_CJARUTILS_H__
