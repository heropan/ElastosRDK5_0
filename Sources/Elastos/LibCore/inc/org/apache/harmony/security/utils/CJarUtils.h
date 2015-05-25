
#ifndef __CJARUTILS_H__
#define __CJARUTILS_H__

#include "_CJarUtils.h"

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

#endif // __CJARUTILS_H__
