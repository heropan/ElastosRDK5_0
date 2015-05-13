
#include "EncodedKeySpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {

EncodedKeySpec::EncodedKeySpec()
{}

EncodedKeySpec::EncodedKeySpec(
    /* [in] */ ArrayOf<Byte> *encodedKey)
{
    // Defensively copies parameter
    // to prevent subsequent modification
    mEncodedKey = ArrayOf<Byte>::Alloc(encodedKey->GetLength());
    mEncodedKey->Copy(0, encodedKey, 0, mEncodedKey->GetLength());
}

/**
 * Returns the encoded key bytes.
 *
 * @return the encoded key bytes.
 */
ECode EncodedKeySpec::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte> **encodedKey)
{
    // Defensively copies private array
    // to prevent subsequent modification
    VALIDATE_NOT_NULL(encodedKey)
    AutoPtr<ArrayOf<Byte> > ret = ArrayOf<Byte>::Alloc(mEncodedKey->GetLength());
    ret->Copy(0, mEncodedKey, 0, ret->GetLength());
    *encodedKey = ret;
    INTERFACE_ADDREF(*encodedKey)
    return NOERROR;
}

ECode EncodedKeySpec::Init(
    /* [in] */ ArrayOf<Byte> *encodedKey)
{
    // Defensively copies parameter
    // to prevent subsequent modification
    mEncodedKey = ArrayOf<Byte>::Alloc(encodedKey->GetLength());
    return mEncodedKey->Copy(0, encodedKey, 0, mEncodedKey->GetLength());
}

} // namespace Spec
} // namespace Security
} // namespace Elastos
