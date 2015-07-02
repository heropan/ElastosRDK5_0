
#ifndef __CX509CERTIFICATEHELPER_H__
#define __CX509CERTIFICATEHELPER_H__

#include "_CX509CertificateHelper.h"
#include "X509CertificateX.h"

namespace Elastosx {
namespace Security {
namespace Cert {

CarClass(CX509CertificateHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ IInputStream *inStream,
        /* [out] */ IX509Certificate **cert);

    CARAPI GetInstance(
        /* [in] */ const ArrayOf<Byte>& certData,
        /* [out] */ IX509Certificate **cert);
};

} // namespace Cert
} // namespace Security
} // namespace Elastosx

#endif // __CX509CERTIFICATEHELPER_H__
