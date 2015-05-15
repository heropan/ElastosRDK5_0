
#ifndef __CKEYSTORESECRETKEYENTRY_H__
#define __CKEYSTORESECRETKEYENTRY_H__

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

#endif // __CKEYSTORESECRETKEYENTRY_H__
