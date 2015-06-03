
#include "CKeyStoreSecretKeyEntry.h"
#include "StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Security {


ECode CKeyStoreSecretKeyEntry::GetSecretKey(
    /* [out] */ ISecretKey **key)
{
    // TODO: Add your code here
    VALIDATE_NOT_NULL(key)
    *key = mSecretKey;
    REFCOUNT_ADD(*key)
    return NOERROR;
}

ECode CKeyStoreSecretKeyEntry::ToString(
    /* [out] */ String *str)
{
    StringBuilder sb("SecretKeyEntry: algorithm - ");
    String algo;
    mSecretKey->GetAlgorithm(&algo);
    sb.AppendString(algo);
    return sb.ToString(str);
}

ECode CKeyStoreSecretKeyEntry::constructor(
    /* [in] */ ISecretKey *secretKey)
{
    if (secretKey == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mSecretKey = secretKey;
    return NOERROR;
}

}
}

