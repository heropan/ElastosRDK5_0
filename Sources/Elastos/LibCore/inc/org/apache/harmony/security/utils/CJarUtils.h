
#ifndef __ORG_APACHE_HARMONY_SECURITY_UTILS_CJARUTILS_H__
#define __ORG_APACHE_HARMONY_SECURITY_UTILS_CJARUTILS_H__

#include "_Org_Apache_Harmony_Security_Utils_CJarUtils.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

CarClass(CJarUtils)
{
public:
    CARAPI VerifySignature(
        /* [in] */ Elastos::IO::IInputStream * pSignature,
        /* [in] */ Elastos::IO::IInputStream * pSignatureBlock,
        /* [out, callee] */ ArrayOf<Elastos::Security::Cert::ICertificate *> ** ppSign);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_UTILS_CJARUTILS_H__
