
#include "CDerOutputStream.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {


ECode CDerOutputStream::SetEncoded(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return BerOutputStream::SetEncoded(encoded);
}

ECode CDerOutputStream::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    return BerOutputStream::GetEncoded(encoded);
}

ECode CDerOutputStream::SetLength(
    /* [in] */ Int32 length)
{
    return BerOutputStream::SetLength(length);
}

ECode CDerOutputStream::GetLength(
    /* [out] */ Int32* length)
{
    return BerOutputStream::GetLength(length);
}

ECode CDerOutputStream::SetContent(
    /* [in] */ IInterface* content)
{
    return BerOutputStream::SetContent(content);
}

ECode CDerOutputStream::GetContent(
    /* [out] */ IInterface** content)
{
    return BerOutputStream::GetContent(content);
}

ECode CDerOutputStream::EncodeTag(
    /* [in] */ Int32 tag)
{
    return BerOutputStream::EncodeTag(tag);
}

ECode CDerOutputStream::EncodeANY()
{
    return BerOutputStream::EncodeANY();
}

ECode CDerOutputStream::EncodeBitString()
{
    return BerOutputStream::EncodeBitString();
}

ECode CDerOutputStream::EncodeBoolean()
{
    return BerOutputStream::EncodeBoolean();
}

ECode CDerOutputStream::EncodeChoice(
    /* [in] */ IASN1Choice* choice)
{
    AutoPtr<IASN1Type> type = IASN1Type::Probe((*(*mVal)[mIndex])[0]);
    mContent = (*(*mVal)[mIndex])[1];
    mIndex++;
    return type->EncodeASN(this);
}

ECode CDerOutputStream::EncodeExplicit(
    /* [in] */ IASN1Explicit* xplicit)
{
    mContent = (*(*mVal)[mIndex])[0];
    mLength = (*(*mLen)[mIndex])[0]
    mIndex++;
    AutoPtr<IASN1Type> type;
    xplicit->GetType((IASN1Type**)&type);
    return type->EncodeASN(this);
}

ECode CDerOutputStream::EncodeGeneralizedTime()
{
    return BerOutputStream::EncodeGeneralizedTime();
}

ECode CDerOutputStream::EncodeUTCTime()
{
    return BerOutputStream::EncodeUTCTime();
}

ECode CDerOutputStream::EncodeInteger()
{
    return BerOutputStream::EncodeInteger();
}

ECode CDerOutputStream::EncodeOctetString()
{
    return BerOutputStream::EncodeOctetString();
}

ECode CDerOutputStream::EncodeOID()
{
    return BerOutputStream::EncodeOID();
}

ECode CDerOutputStream::EncodeSequence(
    /* [in] */ IASN1Sequence* sequence)
{
    AutoPtr<ArrayOf<IASN1Type*> > type;
    sequence->GetType((ArrayOf<IASN1Type*>**)&type);
    AutoPtr<ArrayOf<IInterface*> > values = (*mVal)[mIndex];
    AutoPtr<ArrayOf<Int32> > compLens = (*mLen)[mIndex];

    mIndex++;
    for (Int32 i = 0; i < type->GetLength(); i++) {
        if ((*values)[i]) == NULL) {
            continue;
        }

        mContent = (*values)[i];
        mLength = (*compLens)[i];
        (*type)[i]->EncodeASN(this);
    }
    return NOERROR;
}

ECode CDerOutputStream::EncodeSequenceOf(
    /* [in] */ IASN1SequenceOf* sequenceOf)
{
    return EncodeValueCollection(sequenceOf);
}

ECode CDerOutputStream::EncodeSet(
    /* [in] */ IASN1Set* set)
{
    return BerOutputStream::EncodeSet(set);
}

ECode CDerOutputStream::EncodeSetOf(
    /* [in] */ IASN1SetOf* setOf)
{
    return EncodeValueCollection(setOf);
}

ECode CDerOutputStream::EncodeValueCollection(
    /* [in] */ IASN1ValueCollection* collection)
{
    AutoPtr<ArrayOf<IInterface*> > values = (*mVal)[mIndex];
    AutoPtr<ArrayOf<IInterface*> > compLens = (*mLen)[mIndex];

    mIndex++;
    for (Int32 i = 0; i < values->GetLength(); i++) {
        mContent = (*values)[i];
        mLength = (*compLens)[i];
        AutoPtr<IASN1Type> type;
        collection->GetType((IASN1Type**)&type);
        type->EncodeASN(this);
    }
    return NOERROR;
}

ECode CDerOutputStream::Push(
    /* [in] */ ArrayOf<Int32>* lengths,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    mIndex++;
    if (mIndex == mVal->GetLength()) {
        AutoPtr<ArrayOf<ArrayOf<Int32> *> > newLen = ArrayOf<ArrayOf<Int32> *>::Alloc(mVal->GetLength()*2);
        newLen->Copy(0, mLen, 0, mVal->GetLength());
        mLen = newLen;

        AutoPtr<ArrayOf<ArrayOf<IInterface*> *> > newVal = ArrayOf<ArrayOf<IInterface*> *>::Alloc(mVal->GetLength()* 2);
        newVal->Copy(0, mVal, 0, mVal->GetLength());
        mVal = newVal;
    }
    (*mLen)[mIndex] = lengths;
    (*mVal)[mIndex] = values;
    return NOERROR;
}

ECode CDerOutputStream::EncodeString()
{
    return BerOutputStream::EncodeString();
}

ECode CDerOutputStream::GetChoiceLength(
    /* [in] */ IASN1Choice* choice)
{
    Int32 i;
    choice->GetIndex(mContent, &i);
    choice->GetObjectToEncode(mContent, (IInterface**)&mContent);
    AutoPtr<ArrayOf<IInterface*> > values = ArrayOf<IInterface*>::Alloc(2);
    AutoPtr<ArrayOf<IASN1Type*> > type;
    choice->GetType((ArrayOf<IASN1Type*>**)&type);
    values->Set(0, (*type)[i]);
    values->Set(1, mContent);

    Push(NULL, values);

    (*type)[i]->SetEncodingContent(this);

    // in case if we get content bytes while getting its length
    // FIXME what about remove it: need redesign
    return values->Set(1, mContent);
}

ECode CDerOutputStream::GetExplicitLength(
    /* [in] */ IASN1Explicit* xplicit)
{
    AutoPtr<ArrayOf<IInterface*> > values = ArrayOf<IInterface*>::Alloc(1);
    AutoPtr<ArrayOf<Int32> > compLens = ArrayOf<Int32>::Alloc(1);
    values->Set(0, mContent);
    Push(compLens, values);
    AutoPtr<IASN1Type> type;
    xplicit->GetType((IASN1Type**)&type);
    type->SetEncodingContent(this);

    // in case if we get content bytes while getting its length
    // FIXME what about remove it: need redesign
    values->Set(0, mContent);
    (*compLens)[0] = mLength;
    return type->GetEncodedLength(this, &mLength);
}

ECode CDerOutputStream::GetSequenceLength(
    /* [in] */ IASN1Sequence* sequence)
{
    AutoPtr<ArrayOf<IASN1Type*> > type;
    sequence->GetType((ArrayOf<IASN1Type*>**)&type);

    AutoPtr<ArrayOf<IInterface*> > values = ArrayOf<IInterface*>::Alloc(type->GetLength());

    AutoPtr<ArrayOf<Int32> > compLens = ArrayOf<Int32>::Alloc(type->GetLength());

    sequence->GetValues(mContent, values);

    Push(compLens, values);

    AutoPtr<ArrayOf<Boolean> > optional;
    sequence->GetOptional((ArrayOf<Boolean>**)&optional);
    AutoPtr<ArrayOf<IInterface*> > def;
    sequence->GetDefault((ArrayOf<IInterface*>**)&def);
    Int32 seqLen = 0;
    for (Int32 i = 0; i < type->GetLength(); i++) {
        // check optional types
        if ((*values)[i] == NULL) {
            if ((*optional)[i]) {
                continue;
            } else {
                //throw new RuntimeException();//FIXME type & message
                return E_RUNTIME_EXCEPTION;
            }
        }

        Boolean equal;
        if ((*def)[i]->Equals((*values)[i], &equal), equal) {
            values->Set(i, NULL);
            continue;
        }

        mContent = (*values)[i];

        (*type)[i]->SetEncodingContent(this);

        (*compLens)[i] = mLength;

        // in case if we get content bytes while getting its length
        // FIXME what about remove it: need redesign
        values->Set(i, content);
        Int32 tmp;
        (*type)[i]->GetEncodedLength(this, &tmp);
        seqLen += tmp;
    }
    mLength = seqLen;
    return NOERROR;
}

ECode CDerOutputStream::GetSequenceOfLength(
    /* [in] */ IASN1SequenceOf* sequence)
{
    return GetValueOfLength(sequence);
}

ECode CDerOutputStream::GetSetLength(
    /* [in] */ IASN1Set* set)
{
    return BerOutputStream::GetSetLength(set);
}

ECode CDerOutputStream::GetSetOfLength(
    /* [in] */ IASN1SetOf* setOf)
{
    return GetValueOfLength(setOf);
}

ECode CDerOutputStream::GetValueOfLength(
    /* [in] */ IASN1ValueCollection* collection)
{
    //FIXME what about another way?
    AutoPtr<ArrayOf<IInterface*> > cv;
    AutoPtr<ICollection> col;
    collection->GetValues(mContent, (ICollection**)&col);
    col->ToArray((ArrayOf<IInterface*>**)&cv);

    AutoPtr<ArrayOf<IInterface*> > values = ArrayOf<IInterface*>::Alloc(cv->GetLength());
    AutoPtr<ArrayOf<Int32> > compLens = ArrayOf<Int32>::Alloc(values->GetLength());

    Push(compLens, values);
    Int32 seqLen = 0;
    AutoPtr<IASN1Type> type;
    collection->GetType((IASN1Type**)&type);
    for (Int32 i = 0; i < values->GetLength(); i++) {

        mContent = (*cv)[i];

        type->SetEncodingContent(this);

        (*compLens)[i] = mLength;

        // in case if we get content bytes while getting its length
        // FIXME what about remove it: need redesign
        values->Set(i, content);
        Int32 tmp;
        seqLen += (type->GetEncodedLength(this, &tmp), tmp);
    }
    mLength = seqLen;
    return NOERROR;
}

ECode CDerOutputStream::constructor(
    /* [in] */ IASN1Type* asn1,
    /* [in] */ IInterface* object)
{
    mContent = object;
    mIndex = -1;
    asn1->SetEncodingContent(this);
    Int32 length;
    asn1->GetEncodedLength(this, &length);
    mEncoded = ArrayOf<Byte>::Alloc(length);
    mIndex = 0;
    return asn1->EncodeASN(this);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

