
#ifndef __CDSAPRIVATEKEYIMPL_H__
#define __CDSAPRIVATEKEYIMPL_H__

#include "_CDSAPrivateKeyImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Provider {
namespace Crypto {

CarClass(CDSAPrivateKeyImpl)
{
public:
    CARAPI GetAlgorithm(
        /* [out] */ String * pAlgorithm);

    CARAPI GetFormat(
        /* [out] */ String * pFormat);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI SetAlgorithm(
        /* [in] */ const String& algorithm);

    CARAPI SetEncoding(
        /* [in] */ ArrayOf<Byte> * pEncoding);

    CARAPI GetParams(
        /* [out] */ Elastos::Security::Interfaces::IDSAParams ** ppParams);

    CARAPI GetX(
        /* [out] */ Elastos::Math::IBigInteger ** ppX);

    CARAPI constructor(
        /* [in] */ Elastos::Security::Spec::IDSAPrivateKeySpec * pKeySpec);

    CARAPI constructor(
        /* [in] */ Elastos::Security::Spec::IPKCS8EncodedKeySpec * pKeySpec);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CDSAPRIVATEKEYIMPL_H__
