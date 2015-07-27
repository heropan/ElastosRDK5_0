
#ifndef __CCIPHER_H__
#define __CCIPHER_H__

#include "_Elastosx_Crypto_CCipher.h"
#include "Cipher.h"

namespace Elastosx {
namespace Crypto {

CarClass(CCipher)
    , public Cipher
{
public:
    CAR_OBJECT_DECL()
};

}
}

#endif // __CCIPHER_H__
