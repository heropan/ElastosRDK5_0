
#ifndef __X509CERTIFICATEX_H__
#define __X509CERTIFICATEX_H__

#include "CertificateX.h"

using Elastos::IO::IInputStream;
using Elastos::Math::IBigInteger;
using Elastos::Utility::IDate;
using Elastos::Security::IPrincipal;

namespace Elastosx {
namespace Security {
namespace Cert {

class X509Certificate
    : public ElLightRefBase
    , public Certificate {
/*
    private static Constructor constructor;
    static {
        try {
            String classname = Security.getProperty("cert.provider.x509v1");
            Class cl = Class.forName(classname);
            constructor = cl.getConstructor(new Class[] {InputStream.class});
        } catch (Throwable e) {
        }
    }
*/
public:
    static CARAPI GetInstance(
        /* [in] */ IInputStream* inStream,
        /* [out] */ IX509Certificate** cert);

    static CARAPI GetInstanceEx(
        /* [in] */ ArrayOf<Byte>* certData,
        /* [out] */ IX509Certificate** cert);

    /**
     * Creates a new {@code X509Certificate}.
     */
    X509Certificate();

    virtual CARAPI CheckValidity() = 0;

    virtual CARAPI CheckValidityEx(
        /* [in] */ IDate* date) = 0;

    virtual CARAPI GetVersion(
        /* [out] */ Int32* version) = 0;

    virtual CARAPI GetSerialNumber(
        /* [out] */ IBigInteger** number) = 0;

    virtual CARAPI GetIssuerDN(
        /* [out] */ IPrincipal** dn) = 0;

    virtual CARAPI GetSubjectDN(
        /* [out] */ IPrincipal** dn) = 0;

   virtual CARAPI GetNotBefore(
        /* [out] */ IDate** dt) = 0;

    virtual CARAPI GetSigAlgParams(
        /* [out, callee] */ ArrayOf<Byte>** params) = 0;

};

} // end Cert
} // end Security
} // end Elastosx

#endif // __X509CERTIFICATEX_H__