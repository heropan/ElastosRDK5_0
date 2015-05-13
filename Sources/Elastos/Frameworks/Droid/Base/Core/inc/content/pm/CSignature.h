
#ifndef __CSIGNATURE_H__
#define __CSIGNATURE_H__

#include "_CSignature.h"

using Elastos::Security::IPublicKey;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Opaque, immutable representation of a signature associated with an
 * application package.
 */
CarClass(CSignature)
{
public:
    CARAPI constructor();

    /**
     * Create Signature from an existing raw byte array.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* signature);

    /**
     * Create Signature from a text representation previously returned by
     * {@link #toChars} or {@link #toCharsString()}. Signatures are expected to
     * be a hex-encoded ASCII string.
     *
     * @param text hex-encoded string representing the signature
     * @throws IllegalArgumentException when signature is odd-length
     */
    CARAPI constructor(
        /* [in] */ const String& text);

    /**
     * Encode the Signature as ASCII text.
     */
    CARAPI ToChars(
        /* [out, callee] */ ArrayOf<Char32>** text);

    /**
     * Encode the Signature as ASCII text in to an existing array.
     *
     * @param existingArray Existing char array or null.
     * @param outLen Output parameter for the number of characters written in
     * to the array.
     * @return Returns either <var>existingArray</var> if it was large enough
     * to hold the ASCII representation, or a newly created char[] array if
     * needed.
     */
    CARAPI ToCharsEx(
        /* [in] */ ArrayOf<Char32>* existingArray,
        /* [in] */ ArrayOf<Int32>* outLen,
        /* [out, callee] */ ArrayOf<Char32>** text);

    /**
     * Return the result of {@link #toChars()} as a String.
     */
    CARAPI ToCharsString(
        /* [out] */ String* str);

    /**
     * @return the contents of this signature as a byte array.
     */
    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    /**
     * Returns the public key for this signature.
     *
     * @throws CertificateException when Signature isn't a valid X.509
     *             certificate; shouldn't happen.
     * @hide
     */
    CARAPI GetPublicKey(
        /* [out] */ IPublicKey** publicKey);

    CARAPI Equals(
        /* [in] */ ISignature* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    static CARAPI ParseHexDigit(
        /* [in] */ Int32 nibble,
        /* [out] */ Int32* digit);

private:
    AutoPtr< ArrayOf<Byte> > mSignature;
    Int32 mHashCode;
    Boolean mHaveHashCode;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CSIGNATURE_H__
