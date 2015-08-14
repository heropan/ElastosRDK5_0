
#ifndef __ORG_APACHE_HARMONY_SECURITY_ASN1_CBEROUTPUTSTREAM_H__
#define __ORG_APACHE_HARMONY_SECURITY_ASN1_CBEROUTPUTSTREAM_H__

#include "_CBerOutputStream.h"
#include <BerOutputStream.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CBerOutputStream), public BerOutputStream
{
public:
    CARAPI SetEncoded(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    CARAPI SetLength(
        /* [in] */ Int32 length);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI SetContent(
        /* [in] */ ArrayOf<Byte>* content);

    CARAPI GetContent(
        /* [out] */ IInterface** content);

    CARAPI EncodeTag(
        /* [in] */ Int32 tag);

    CARAPI EncodeANY();

    CARAPI EncodeBitString();

    CARAPI EncodeBoolean();

    CARAPI EncodeChoice(
        /* [in] */ IASN1Choice* choice);

    CARAPI EncodeExplicit(
        /* [in] */ IASN1Type* xplicit);

    CARAPI EncodeGeneralizedTime();

    CARAPI EncodeUTCTime();

    CARAPI EncodeInteger();

    CARAPI EncodeOctetString();

    CARAPI EncodeOID();

    CARAPI EncodeSequence(
        /* [in] */ IASN1Sequence* sequence);

    CARAPI EncodeSequenceOf(
        /* [in] */ IASN1SequenceOf* sequenceOf);

    CARAPI EncodeSet(
        /* [in] */ IASN1Set* set);

    CARAPI EncodeSetOf(
        /* [in] */ IASN1SetOf* setOf);

    CARAPI EncodeString();

    CARAPI GetChoiceLength(
        /* [in] */ IASN1Choice* choice);

    CARAPI GetExplicitLength(
        /* [in] */ IASN1Type* sequence);

    CARAPI GetSequenceLength(
        /* [in] */ IASN1Sequence* sequence);

    CARAPI GetSequenceOfLength(
        /* [in] */ IASN1SequenceOf* sequence);

    CARAPI GetSetLength(
        /* [in] */ IASN1Set* set);

    CARAPI GetSetOfLength(
        /* [in] */ IASN1SetOf* setOf);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_ASN1_CBEROUTPUTSTREAM_H__
