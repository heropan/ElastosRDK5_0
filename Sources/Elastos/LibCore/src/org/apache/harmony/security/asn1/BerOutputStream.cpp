
#include "CBerOutputStream.h"
#include <cmdef.h>

using Elastos.Core.IBoolean;
using Elastos.Core.IArrayOf;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {


ECode CBerOutputStream::SetEncoded(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    mEncoded = encoded;
    return NOERROR;
}

ECode CBerOutputStream::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    VALIDATE_NOT_NULL(encoded)
    *encoded = mEncoded;
    INTERFACE_ADDREF(*encoded)
    return NOERROR;
}

ECode CBerOutputStream::SetLength(
    /* [in] */ Int32 length)
{
    mLength = length;
    return NOERROR;
}

ECode CBerOutputStream::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mLength;
    return NOERROR;
}

ECode CBerOutputStream::SetContent(
    /* [in] */ IInterface* content)
{
    mContent = content;
    return NOERROR;
}

ECode CBerOutputStream::GetContent(
    /* [out] */ IInterface** content)
{
    VALIDATE_NOT_NULL(content)
    *content = mContent;
    INTERFACE_ADDREF(*content)
    return NOERROR;
}

ECode CBerOutputStream::EncodeTag(
    /* [in] */ Int32 tag)
{
    (*mEncoded)[mOffset++] = (Byte) tag; //FIXME long form?

    if (mLength > 127) { //long form
        Int32 eLen = mLength >> 8;
        Byte numOctets = 1;
        for (; eLen > 0; eLen = eLen >> 8) {
            numOctets++;
        }

        (*mEncoded)[mOffset] = (Byte) (numOctets | 0x80);
        mOffset++;

        eLen = mLength;
        Int32 numOffset = mOffset + numOctets - 1;
        for (Int32 i = 0; i < numOctets; i++, eLen = eLen >> 8) {
            (*mEncoded)[numOffset - i] = (Byte) eLen; //FIXME long value?
        }
        mOffset += numOctets;
    } else { //short form
        (*mEncoded)[mOffset++] = (Byte) mLength;
    }
    return NOERROR;
}

ECode CBerOutputStream::EncodeANY()
{
    AutoPtr<IArrayOf> arrayOf;
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Byte val;
        IByte::Probe(elem)->GetValue(&val);
        (*mEncoded)[i] = val;
    }
    mOffset = mLength;
    return NOERROR;
}

ECode CBerOutputStream::EncodeBitString()
{
        //FIXME check encoding
        AutoPtr<IBitString> bStr = IBitString::Probe(mContent);
        Int32 unusedBits;
        bStr->GetUnusedBits(&unusedBits);
        (*mEncoded)[mOffset] = (Byte)unusedBits;
        AutoPtr<ArrayOf<Byte> > bytes;
        bStr->GetBytes((ArrayOf<Byte>**)&bytes);
        mEncoded->Copy(mOffset + 1, bytes, 0, mLength - 1);
        mOffset += mLength;
        return NOERROR;
}

ECode CBerOutputStream::EncodeBoolean()
{
    Boolean val;
    IBoolean::Probe(content)->GetValue(&val);
    if (val) {
        (*mEncoded)[mOffset] = (Byte) 0xFF;
    } else {
        (*mEncoded)[mOffset] = 0x00;
    }
    mOffset++;
    return NOERROR;
}

ECode CBerOutputStream::EncodeChoice(
    /* [in] */ IASN1Choice* choice)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::EncodeExplicit(
    /* [in] */ IASN1Type* xplicit)
{
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::EncodeGeneralizedTime()
{
    AutoPtr<IArrayOf> arrayOf;
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Byte val;
        IByte::Probe(elem)->GetValue(&val);
        (*mEncoded)[i + mOffset] = val;
    }
    mOffset += mLength;
    return NOERROR;
}

ECode CBerOutputStream::EncodeUTCTime()
{
    AutoPtr<IArrayOf> arrayOf;
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Byte val;
        IByte::Probe(elem)->GetValue(&val);
        (*mEncoded)[i + mOffset] = val;
    }
    mOffset += mLength;
    return NOERROR;
}

ECode CBerOutputStream::EncodeInteger()
{
    AutoPtr<IArrayOf> arrayOf;
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Byte val;
        IByte::Probe(elem)->GetValue(&val);
        (*mEncoded)[i + mOffset] = val;
    }
    mOffset += mLength;
    return NOERROR;
}

ECode CBerOutputStream::EncodeOctetString()
{
    AutoPtr<IArrayOf> arrayOf;
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Byte val;
        IByte::Probe(elem)->GetValue(&val);
        (*mEncoded)[i + mOffset] = val;
    }
    mOffset += mLength;
    return NOERROR;
}

ECode CBerOutputStream::EncodeOID()
{
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    AutoPtr<ArrayOf<Int32> > oid = ArrayOf<Int32>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Int32 val;
        IInteger32::Probe(elem)->GetValue(&val);
        (*oid)[i] = val;
    }

    Int32 oidLen = mLength;

    // all subidentifiers except first
    Int32 elem;
    for (Int32 i = oid->GetLength() - 1; i > 1; i--, oidLen--) {
        elem = (*oid)[i];
        if (elem > 127) {
            (*mEncoded)[mOffset + oidLen - 1] = (Byte) (elem & 0x7F);
            elem = elem >> 7;
            for (; elem > 0;) {
                oidLen--;
                (*mEncoded)[mOffset + oidLen - 1] = (Byte) (elem | 0x80);
                elem = elem >> 7;
            }
        } else {
            (*mEncoded)[mOffset + oidLen - 1] = (Byte) elem;
        }
    }

    // first subidentifier
    elem = (*oid)[0] * 40 + (*oid)[1];
    if (elem > 127) {
        (*mEncoded)[mOffset + oidLen - 1] = (Byte) (elem & 0x7F);
        elem = elem >> 7;
        for (; elem > 0;) {
            oidLen--;
            (*mEncoded)[mOffset + oidLen - 1] = (Byte) (elem | 0x80);
            elem = elem >> 7;
        }
    } else {
        (*mEncoded)[mOffset + oidLen - 1] = (Byte) elem;
    }

    mOffset += mLength;
    return NOERROR;
}

ECode CBerOutputStream::EncodeSequence(
    /* [in] */ IASN1Sequence* sequence)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::EncodeSequenceOf(
    /* [in] */ IASN1SequenceOf* sequenceOf)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::EncodeSet(
    /* [in] */ IASN1Set* set)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::EncodeSetOf(
    /* [in] */ IASN1SetOf* setOf)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::EncodeString()
{
    AutoPtr<IArrayOf> arrayOf;
    Int32 length;
    IArrayOf::Probe(mContent)->GetLength(&length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> elem;
        IArrayOf::Probe(mContent)->Get(i, (IInterface**)&elem);
        Byte val;
        IByte::Probe(elem)->GetValue(&val);
        (*mEncoded)[i + mOffset] = val;
    }
    mOffset += mLength;
    return NOERROR;
}

ECode CBerOutputStream::GetChoiceLength(
    /* [in] */ IASN1Choice* choice)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::GetExplicitLength(
    /* [in] */ IASN1Type* sequence)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::GetSequenceLength(
    /* [in] */ IASN1Sequence* sequence)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::GetSequenceOfLength(
    /* [in] */ IASN1SequenceOf* sequence)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::GetSetLength(
    /* [in] */ IASN1Set* set)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

ECode CBerOutputStream::GetSetOfLength(
    /* [in] */ IASN1SetOf* setOf)
{
    //throw new RuntimeException("Is not implemented yet"); //FIXME
    return E_RUNTIME_EXCEPTION;
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

