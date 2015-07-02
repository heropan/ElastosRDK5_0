
#include "X509CertificateX.h"
#include "CByteArrayInputStream.h"

using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;

namespace Elastosx {
namespace Security {
namespace Cert {

X509Certificate::X509Certificate()
{}

CARAPI X509Certificate::GetInstance(
    /* [in] */ IInputStream* inStream,
    /* [out] */  IX509Certificate** cert)
{
    return E_NOT_IMPLEMENTED;
}

CARAPI X509Certificate::GetInstance(
    /* [in] */ ArrayOf<Byte>* certData,
    /* [out] */ IX509Certificate** cert)
{
    VALIDATE_NOT_NULL(cert)
    if (!certData) {
        return E_CERTIFICATE_EXCEPTION;
    }
    AutoPtr<IByteArrayInputStream> bais;
    CByteArrayInputStream::New(certData, (IByteArrayInputStream**)&bais);
    return GetInstance(bais.Get(), cert);
}

} // end Cert
} // end Security
} // end Elastosx
