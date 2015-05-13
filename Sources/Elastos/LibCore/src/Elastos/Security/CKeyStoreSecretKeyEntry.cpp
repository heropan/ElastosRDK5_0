
#include "CKeyStoreSecretKeyEntry.h"
#include "cmdef.h"
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
    INTERFACE_ADDREF(*key)
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

