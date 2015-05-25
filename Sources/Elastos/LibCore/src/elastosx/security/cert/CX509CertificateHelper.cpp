
#include "CX509CertificateHelper.h"

namespace Elastosx {
namespace Security {
namespace Cert {


ECode CX509CertificateHelper::GetInstance(
    /* [in] */ IInputStream *inStream,
    /* [out] */ IX509Certificate **cert)
{
    return X509Certificate::GetInstance(inStream, cert);
}

ECode CX509CertificateHelper::GetInstanceEx(
    /* [in] */ const ArrayOf<Byte>& certData,
    /* [out] */ IX509Certificate **cert)
{
    return X509Certificate::GetInstanceEx(certData, cert);
}

}
}
}

