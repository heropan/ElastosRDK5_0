
#include "ext/frameworkext.h"
#include "content/pm/CSignature.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CSignature::constructor()
{
    return NOERROR;
}

ECode CSignature::constructor(
    /* [in] */ ArrayOf<Byte>* signature)
{
    mSignature = signature->Clone();
    return NOERROR;
}

ECode CSignature::constructor(
    /* [in] */ const String& text)
{
    // TODO:
    // AutoPtr<ArrayOf<Byte> > input;
    // text.GetBytes(&input);
    // const Int32 N = input->GetLength();

    // if (N % 2 != 0) {
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }

    // AutoPtr<ArrayOf<Byte> > sig = ArrayOf::Alloc(N / 2);
    // Int32 sigIndex = 0;

    // for (Int32 i = 0; i < N;) {
    //     Int32 hi;
    //     Int32 lo;

    //     FAIL_RETURN(ParseHexDigit(input[i++], &hi));
    //     FAIL_RETURN(ParseHexDigit(input[i++], &lo));
    //     sig[sigIndex++] = (Byte) ((hi << 4) | lo);
    // }

    // mSignature = sig->Clone();
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CSignature::ParseHexDigit(
    /* [in] */ Int32 nibble,
    /* [out] */ Int32* digit)
{
    if ('0' <= nibble && nibble <= '9') {
        *digit = nibble - '0';
        return NOERROR;
    }
    else if ('a' <= nibble && nibble <= 'f') {
        *digit =  nibble - 'a' + 10;
        return NOERROR;
    }
    else if ('A' <= nibble && nibble <= 'F') {
        *digit =  nibble - 'A' + 10;
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CSignature::ToChars(
    /* [out, callee] */ ArrayOf<Char32>** text)
{
    return ToChars(NULL, NULL, text);
}

ECode CSignature::ToChars(
    /* [in] */ ArrayOf<Char32>* existingArray,
    /* [in] */ ArrayOf<Int32>* outLen,
    /* [out, callee] */ ArrayOf<Char32>** text)
{
    VALIDATE_NOT_NULL(text);

    // byte[] sig = mSignature;
    // final int N = sig.length;
    // final int N2 = N*2;
    // char[] text = existingArray == null || N2 > existingArray.length
    //         ? new char[N2] : existingArray;
    // for (int j=0; j<N; j++) {
    //     byte v = sig[j];
    //     int d = (v>>4)&0xf;
    //     text[j*2] = (char)(d >= 10 ? ('a' + d - 10) : ('0' + d));
    //     d = v&0xf;
    //     text[j*2+1] = (char)(d >= 10 ? ('a' + d - 10) : ('0' + d));
    // }
    // if (outLen != null) outLen[0] = N;
    // return text;
    assert(0);
    return NOERROR;
}

ECode CSignature::ToCharsString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

//    String str = mStringRef == null ? null : mStringRef.get();
//    if (str != null) {
//        return str;
//    }
//    *str = String(toChars());
//    mStringRef = new SoftReference<String>(str);
//    return str;
    assert(0);
    return NOERROR;
}

ECode CSignature::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);

    *bytes = mSignature->Clone();
    REFCOUNT_ADD(*bytes);
    return NOERROR;
}

ECode CSignature::GetPublicKey(
    /* [out] */ IPublicKey** publicKey)
{
    // final CertificateFactory certFactory = CertificateFactory.getInstance("X.509");
    // final ByteArrayInputStream bais = new ByteArrayInputStream(mSignature);
    // final Certificate cert = certFactory.generateCertificate(bais);
    // return cert.getPublicKey();
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CSignature::Equals(
    /* [in] */ ISignature* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

//    try {
    if (obj != NULL) {
        CSignature* other = (CSignature*)obj;
        if (this == other) {
            *isEqual = TRUE;
            return NOERROR;
        }
        if (mSignature->GetLength() == other->mSignature->GetLength()) {
            *isEqual = memcmp(mSignature->GetPayload(),
                    other->mSignature->GetPayload(), mSignature->GetLength()) == 0;
            return NOERROR;
        }
    }
//    } catch (ClassCastException e) {
//    }
    *isEqual = FALSE;
    return NOERROR;
}

ECode CSignature::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    if (mHaveHashCode) {
        *hashCode = mHashCode;
        return NOERROR;
    }
//    mHashCode = Arrays.hashCode(mSignature);
    assert(0);
    mHaveHashCode = TRUE;
    *hashCode = mHashCode;
    return NOERROR;;
}

ECode CSignature::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadArrayOf((Handle32*)&mSignature);
    return NOERROR;
}

ECode CSignature::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteArrayOf((Handle32)mSignature.Get());
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
