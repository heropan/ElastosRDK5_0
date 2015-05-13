
#ifndef __CBITSTRING_H__
#define __CBITSTRING_H__

#include "_CBitString.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CBitString)
{
public:
    CARAPI_(AutoPtr<ArrayOf<Byte> >) InitStatic();

    CARAPI SetBytes(
        /* [in] */ ArrayOf<Byte>* bytes);

    CARAPI GetBytes(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI SetUnusedBits(
        /* [in] */ Int32 unusedBits);

    CARAPI GetUnusedBits(
        /* [out] */ Int32* unusedBits);

    CARAPI GetBit(
        /* [in] */ Int32 bit,
        /* [out] */ Boolean* value);

    CARAPI SetBit(
        /* [in] */ Int32 bit,
        /* [in] */ Boolean value);

    CARAPI ToBooleanArray(
        /* [out, callee] */ ArrayOf<Boolean>** array);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 unusedBits);

    CARAPI constructor(
        /* [in] */ ArrayOf<Boolean>* values);

public:
    /** Sequence of bits padded with unused bits. */
    AutoPtr<ArrayOf<Byte> > mBytes;

    /** Number of unused bits in the last byte. */
    Int32 mUnusedBits;

private:
    static AutoPtr<ArrayOf<Byte> > SET_MASK;

    static AutoPtr<ArrayOf<Byte> > RESET_MASK;
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CBITSTRING_H__
