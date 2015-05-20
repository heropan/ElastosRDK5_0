
#ifndef __CDEROUTPUTSTREAM_H__
#define __CDEROUTPUTSTREAM_H__

#include "_CDerOutputStream.h"
#include <BerOutputStream.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CDerOutputStream), public BerOutputStream
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
        /* [in] */ IInterface* content);

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
        /* [in] */ IASN1Explicit* xplicit);

    CARAPI GetSequenceLength(
        /* [in] */ IASN1Sequence* sequence);

    CARAPI GetSequenceOfLength(
        /* [in] */ IASN1SequenceOf* sequence);

    CARAPI GetSetLength(
        /* [in] */ IASN1Set* set);

    CARAPI GetSetOfLength(
        /* [in] */ IASN1SetOf* setOf);

    CARAPI constructor(
        /* [in] */ IASN1Type* asn1,
        /* [in] */ IInterface* object);

private:
    CARAPI EncodeValueCollection(
        /* [in] */ IASN1ValueCollection* collection);

    CARAPI Push(
        /* [in] */ ArrayOf<Int32>* lengths,
        /* [in] */ ArrayOf<IInterface*>* values);

    CARAPI GetValueOfLength(
        /* [in] */ IASN1ValueCollection* collection)

private:
    static Int32 mInitSize = 32;
    Int32 mIndex;
    AutoPtr<ArrayOf<Int32Array > > mLen; //new int[initSize][]
    AutoPtr<ArrayOf<AutoPtr<ArrayOf<IInterface*> > > > mVal;//new Object[initSize][];
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CDEROUTPUTSTREAM_H__
