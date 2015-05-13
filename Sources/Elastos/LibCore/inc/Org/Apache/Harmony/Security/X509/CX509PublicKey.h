
#ifndef __CX509PUBLICKEY_H__
#define __CX509PUBLICKEY_H__

#include "_CX509PublicKey.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CX509PublicKey)
{
public:
    CARAPI GetAlgorithm(
        /* [out] */ String * pAlgorithm);

    CARAPI GetFormat(
        /* [out] */ String * pFormat);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI constructor(
        /* [in] */ const String& algorithm,
        /* [in] */ ArrayOf<Byte> * pEncoded,
        /* [in] */ ArrayOf<Byte> * pKeyBytes);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CX509PUBLICKEY_H__
