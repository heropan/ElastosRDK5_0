
#ifndef __CPUBLICKEYIMPL_H__
#define __CPUBLICKEYIMPL_H__

#include "_CPublicKeyImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {

CarClass(CPublicKeyImpl)
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

    CARAPI constructor(
        /* [in] */ const String& algorithm);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __CPUBLICKEYIMPL_H__
