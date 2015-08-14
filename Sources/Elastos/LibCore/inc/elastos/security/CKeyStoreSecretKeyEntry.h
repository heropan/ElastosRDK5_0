
#ifndef __ELASTOS_SECURITY_CKEYSTORESECRETKEYENTRY_H__
#define __ELASTOS_SECURITY_CKEYSTORESECRETKEYENTRY_H__

#include "_CKeyStoreSecretKeyEntry.h"

using Elastosx::Crypto::ISecretKey;

namespace Elastos {
namespace Security {

CarClass(CKeyStoreSecretKeyEntry)
{
public:
    CARAPI GetSecretKey(
        /* [out] */ ISecretKey **key);

    CARAPI ToString(
        /* [out] */ String *str);

    CARAPI constructor(
        /* [in] */ ISecretKey *secretKey);

private:
    // Store SecretKey
    AutoPtr<ISecretKey> mSecretKey;
};

}
}

#endif // __ELASTOS_SECURITY_CKEYSTORESECRETKEYENTRY_H__
