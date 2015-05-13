
#ifndef __ENCODEDKEYSPEC_H__
#define __ENCODEDKEYSPEC_H__

#include "_CX509EncodedKeySpec.h"

namespace Elastos {
namespace Security {
namespace Spec {

class EncodedKeySpec
{
public:
    EncodedKeySpec();

    EncodedKeySpec(
        /* [in] */ ArrayOf<Byte> *encodedKey);

    /**
     * Returns the encoded key bytes.
     *
     * @return the encoded key bytes.
     */
    virtual CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> **encodedKey);

    virtual CARAPI GetFormat(
        /* [out] */ String *format) = 0;

    CARAPI Init(
        /* [in] */ ArrayOf<Byte> *encodedKey);
private:
    // Encoded key
    const AutoPtr<ArrayOf<Byte> > mEncodedKey;
};

}
}
}

#endif // __ENCODEDKEYSPEC_H__
