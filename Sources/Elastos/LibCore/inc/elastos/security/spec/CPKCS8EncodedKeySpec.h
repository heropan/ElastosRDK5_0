
#ifndef __ELASTOS_SECURITY_SPEC_CPKCS8ENCODEDKEYSPEC_H__
#define __ELASTOS_SECURITY_SPEC_CPKCS8ENCODEDKEYSPEC_H__

#include "_CPKCS8EncodedKeySpec.h"
#include "EncodedKeySpec.h"

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CPKCS8EncodedKeySpec), public EncodedKeySpec
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> **encoded);

    CARAPI GetFormat(
        /* [out] */ String *format);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> *encodedKey);
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CPKCS8ENCODEDKEYSPEC_H__
