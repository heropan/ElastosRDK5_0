
#include "CPKCS8EncodedKeySpec.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CPKCS8EncodedKeySpec::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte> **encoded)
{
    return EncodedKeySpec::GetEncoded(encoded);
}

ECode CPKCS8EncodedKeySpec::GetFormat(
    /* [out] */ String *format)
{
    VALIDATE_NOT_NULL(format)
    *format = String("PKCS#8");
    return NOERROR;
}

ECode CPKCS8EncodedKeySpec::constructor(
    /* [in] */ ArrayOf<Byte> *encodedKey)
{
    return EncodedKeySpec::Init(encodedKey);
}

}
}
}

