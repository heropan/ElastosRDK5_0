
#ifndef __CNULLCIPHER_H__
#define __CNULLCIPHER_H__

#include "_Elastosx_Crypto_CNullCipher.h"
#include "Cipher.h"

namespace Elastosx {
namespace Crypto {

CarClass(CNullCipher)
    , public Cipher
    , public INullCipher
{
public:
    CAR_INTERFACE_DECL()

    CNullCipher();

    /**
     * Creates a new {@code NullCipher} instance.
     */
    CARAPI constructor();

};

}
}

#endif // __CNULLCIPHER_H__
