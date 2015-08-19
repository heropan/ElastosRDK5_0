
#include "BerInputStream.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

ECode BerInputStream::GetLength(
        /* [in] */ ArrayOf<Byte>* encoding,
        /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    Int32 length = (*encoding)[1] & 0xFF;
    Int32 numOctets = 0;
    if ((length & 0x80) != 0) { // long form
        numOctets = length & 0x7F;

        // collect this value length
        length = (*encoding)[2] & 0xFF;
        for (Int32 i = 3; i < numOctets + 2; i++) {
            length = (length << 8) + ((*encoding)[i] & 0xFF);
        }
    }
    //    tag length long_form content
    *ret = 1 + 1 + numOctets + length;
    return NOERROR;
}

ECode BerInputStream::SetTag(
    /* [in] */ Int32 tag)
{
    mTag = tag;
    return NOERROR;
}

ECode BerInputStream::GetTag(
    /* [out] */ Int32* tag)
{
    VALIDATE_NOT_NULL(tag)
    *tag = mTag;
    return NOERROR;
}

ECode BerInputStream::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mLength;
    return NOERROR;
}

ECode BerInputStream::SetLength(
    /* [in] */ Int32 length)
{
    mLength = length;
    return NOERROR;
}

ECode BerInputStream::SetContent(
    /* [in] */ IInterface* content)
{
    mContent = content;
    return NOERROR;
}

ECode BerInputStream::GetContent(
    /* [out] */ IInterface** content)
{
    VALIDATE_NOT_NULL(content)
    *content = mContent;
    REFCOUNT_ADD(*content)
    return NOERROR;
}

ECode BerInputStream::GetTagOffSet(
    /* [out] */ Int32* tagOffset)
{
    VALIDATE_NOT_NULL(tagOffset)
    *tagOffset = mTagOffset;
    return NOERROR;
}
ECode BerInputStream::SetTagOffSet(
    /* [in] */ Int32 tagOffset)
{
    mTagOffset = tagOffset;
    return NOERROR;
}

ECode BerInputStream::GetContentOffset(
    /* [out] */ Int32* contentOffset)
{
    VALIDATE_NOT_NULL(contentOffset)
    *contentOffset = mContentOffset;
    return NOERROR;
}

ECode BerInputStream::SetContentOffset(
    /* [in] */ Int32 contentOffset)
{
    mContentOffset = contentOffset;
    return NOERROR;
}

ECode BerInputStream::Reset(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    mBuffer = encoded;
    Int32 ret;
    return Next(&ret);
}

ECode BerInputStream::Next(
    /* [out] */ Int32* next)
{
    VALIDATE_NOT_NULL(next)
    mTagOffset = mOffset;

    // read tag
    Read(&mTag);

    // read length
    Read(&mLength);
    if (mLength != 0x80) { // definite form
        // long or short length form
        if ((mLength & 0x80) != 0) { // long form
            Int32 numOctets = mLength & 0x7F;

            if (numOctets > 5) {
                //throw new ASN1Exception("Too long encoding at [" + tagOffset + "]"); //FIXME message
                return E_ASN1_EXCEPTION;
            }

            // collect this value length
            Read(&mLength);
            for (Int32 i = 1; i < numOctets; i++) {
                Int32 ch;
                Read(&ch);
                mLength = (mLength << 8) + ch;//read();
            }

            if (mLength > 0xFFFFFF) {
                //throw new ASN1Exception("Too long encoding at [" + tagOffset + "]"); //FIXME message
                return E_ASN1_EXCEPTION;
            }
        }
    } else { //indefinite form
        mLength = INDEFINIT_LENGTH;
    }
    mContentOffset = mOffset;

    *next = mTag;
    return NOERROR;
}

ECode BerInputStream::ReadBitString()
{
    if (mTag == IASN1Constants::TAG_BITSTRING) {

        if (mLength == 0) {
            //throw new ASN1Exception("ASN.1 Bitstring: wrong length. Tag at [" + tagOffset + "]");
            return E_ASN1_EXCEPTION;
        }

        ReadContent();

        // content: check unused bits
        if ((*mBuffer)[mContentOffset] > 7) {
            /*throw new ASN1Exception("ASN.1 Bitstring: wrong content at [" + contentOffset
                    + "]. A number of unused bits MUST be in range 0 to 7");*/
            return E_ASN1_EXCEPTION;
        }

        if (mLength == 1 && (*mBuffer)[mContentOffset] != 0) {
            /*throw new ASN1Exception("ASN.1 Bitstring: wrong content at [" + contentOffset
                    + "]. For empty string unused bits MUST be 0");*/
            return E_ASN1_EXCEPTION;
        }

    } else if (mTag == IASN1Constants::TAG_C_BITSTRING) {
        //throw new ASN1Exception("Decoding constructed ASN.1 bitstring  type is not provided");
        return E_ASN1_EXCEPTION;
    } else {
        //throw expected("bitstring");
        return E_ASN1_EXCEPTION;
    }
    return NOERROR;
}

ECode BerInputStream::ReadEnumerated()
{
    if (mTag != IASN1Constants::TAG_ENUM) {
        //throw expected("enumerated");
        return E_ASN1_EXCEPTION;
    }

    // check encoded length
    if (mLength == 0) {
        /*throw new ASN1Exception("ASN.1 enumerated: wrong length for identifier at ["
                + tagOffset + "]");*/
        return E_ASN1_EXCEPTION;
    }

    ReadContent();

    // check encoded content
    if (mLength > 1) {
        Int32 bits = (*mBuffer)[mContentOffset] & 0xFF;
        if ((*mBuffer)[mContentOffset + 1] < 0) {
            bits += 0x100;
        }

        if (bits == 0 || bits == 0x1FF) {
            /*throw new ASN1Exception("ASN.1 enumerated: wrong content at [" + contentOffset
                    + "]. An integer MUST be encoded in minimum number of octets");*/
            return E_ASN1_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode BerInputStream::ReadBoolean()
{
    if (mTag != IASN1Constants::TAG_BOOLEAN) {
        //throw expected("boolean");
        return E_ASN1_EXCEPTION;
    }

    // check encoded length
    if (mLength != 1) {
        //throw new ASN1Exception("Wrong length for ASN.1 boolean at [" + tagOffset + "]");
        return E_ASN1_EXCEPTION;
    }

    return ReadContent();
}

ECode BerInputStream::SetChoiceIndex(
    /* [in] */ Int32 choiceIndex)
{
    mChoiceIndex = choiceIndex;
    return NOERROR;
}

ECode BerInputStream::GetChoiceIndex(
    /* [out] */ Int32* choiceIndex)
{
    VALIDATE_NOT_NULL(choiceIndex)
    *choiceIndex = mChoiceIndex;
    return NOERROR;
}

ECode BerInputStream::SetTimes(
    /* [in] */ ArrayOf<Int32>* times)
{
    mTimes = times;
    return NOERROR;
}

ECode BerInputStream::GetTimes(
    /* [out, callee] */ ArrayOf<Int32>** times)
{
    VALIDATE_NOT_NULL(times)
    *times = mTimes;
    REFCOUNT_ADD(*times)
    return NOERROR;
}

ECode BerInputStream::ReadGeneralizedTime()
{
    if (mTag == IASN1Constants::TAG_GENERALIZEDTIME) {
        // FIXME: any other optimizations?
        ReadContent();
        // FIXME store string somewhere to allow a custom time type perform
        // additional checks

        // check syntax: the last char MUST be Z
        if ((*mBuffer)[mOffset - 1] != 'Z') {
            // FIXME support only format that is acceptable for DER
            //throw new ASN1Exception("ASN.1 GeneralizedTime: encoded format is not implemented");
            return E_ASN1_EXCEPTION;
        }

        // check syntax: MUST be YYYYMMDDHHMMSS[(./,)DDD]'Z'
        if (mLength != 15 && (mLength < 17 || mLength > 19)) {
            //throw new ASN1Exception("ASN.1 GeneralizedTime wrongly encoded at ["
            //        + contentOffset + "]");
            return E_ASN1_EXCEPTION;
        }

        // check content: milliseconds
        if (mLength > 16) {
            Byte char14 = (*mBuffer)[mContentOffset + 14];
            if (char14 != '.' && char14 != ',') {
                //throw new ASN1Exception("ASN.1 GeneralizedTime wrongly encoded at ["
                //       + contentOffset + "]");
                return E_ASN1_EXCEPTION;
            }
        }

        if (mTimes == null) {
            mTimes = ArrayOf<Int32>::Alloc(7);
        }
        StrToInt(mContentOffset, 4, &(*times)[0]); // year
        StrToInt(mContentOffset + 4, 2, &(*times)[1]); // month
        StrToInt(mContentOffset + 6, 2, &(*times)[2]); // day
        StrToInt(mContentOffset + 8, 2, &(*times)[3]); // hour
        StrToInt(mContentOffset + 10, 2, &(*times)[4]); // minute
        StrToInt(mContentOffset + 12, 2, &(*times)[5]); // second

        if (mLength > 16) {
            // FIXME optimize me
            StrToInt(mContentOffset + 15, mLength - 16, &(*times)[6]);

            if (mLength == 17) {
                (*times)[6] = (*times)[6] * 100;
            } else if (mLength == 18) {
                (*times)[6] = (*times)[6] * 10;
            }
        }

        // FIXME check all values for valid numbers!!!
    } else if (mTag == IASN1Constants::TAG_C_GENERALIZEDTIME) {
        //throw new ASN1Exception("Decoding constructed ASN.1 GeneralizedTime type is not supported");
        return E_ASN1_EXCEPTION;
    } else {
        //throw expected("GeneralizedTime");
        return E_ASN1_EXCEPTION;
    }
}

ECode BerInputStream::ReadUTCTime()
{
    if (mTag == IASN1Constants::TAG_UTCTIME) {
        switch (mLength) {
        case IASN1UTCTime::UTC_HM:
        case IASN1UTCTime::UTC_HMS:
            break;
        case IASN1UTCTime::UTC_LOCAL_HM:
        case IASN1UTCTime::UTC_LOCAL_HMS:
            // FIXME only coordinated universal time formats are supported
            //throw new ASN1Exception("ASN.1 UTCTime: local time format is not supported");
            return E_ASN1_EXCEPTION;
        default:
            //throw new ASN1Exception("ASN.1 UTCTime: wrong length, identifier at " + tagOffset);
            return E_ASN1_EXCEPTION;
        }

        // FIXME: any other optimizations?
        ReadContent();

        // FIXME store string somewhere to allow a custom time type perform
        // additional checks

        // check syntax: the last char MUST be Z
        if ((*mBuffer)[mOffset - 1] != 'Z') {
            /*throw new ASN1Exception("ASN.1 UTCTime wrongly encoded at ["
                    + contentOffset + ']');*/
            return E_ASN1_EXCEPTION;
        }

        if (mTimes == NULL) {
            mTimes = ArrayOf<Int32>::Alloc(7);
        }

        StrToInt(mContentOffset, 2, &(*mTimes)[0]); // year
        if ((*mTimes)[0] > 49) {
            (*mTimes)[0] += 1900;
        } else {
            (*mTimes)[0] += 2000;
        }

        StrToInt(mContentOffset + 2, 2 &(*mTimes)[1]); // month
        StrToInt(mContentOffset + 4, 2, &(*mTimes)[2]); // day
        StrToInt(mContentOffset + 6, 2, &(*mTimes)[3]); // hour
        StrToInt(mContentOffset + 8, 2, &(*mTimes)[4]); // minute

        if (mLength == IASN1UTCTime::UTC_HMS) {
            StrToInt(mContentOffset + 10, 2, &(*mTimes)[5]); // second
        }

        // FIXME check all time values for valid numbers!!!
    } else if (mTag == IASN1Constants::TAG_C_UTCTIME) {
        //throw new ASN1Exception("Decoding constructed ASN.1 UTCTime type is not supported");
        return E_ASN1_EXCEPTION;
    } else {
        //throw expected("UTCTime");
        return E_ASN1_EXCEPTION;
    }
}

ECode BerInputStream::ReadInteger()
{
    if (mTag != IASN1Constants::TAG_INTEGER) {
        //throw expected("integer");
        return E_ASN1_EXCEPTION;
    }

    // check encoded length
    if (mLength < 1) {
        //throw new ASN1Exception("Wrong length for ASN.1 integer at [" + tagOffset + "]");
        return E_ASN1_EXCEPTION;
    }

    ReadContent();

    // check encoded content
    if (mLength > 1) {
        Byte firstByte = (*mBuffer)[mOffset - mLength];
        Byte secondByte = (Byte) ((*mBuffer)[mOffset - mLength + 1] & 0x80);

        if (firstByte == 0 && secondByte == 0 || firstByte == (Byte) 0xFF
                && secondByte == (Byte) 0x80) {
            //throw new ASN1Exception("Wrong content for ASN.1 integer at [" + (offset - length) + "]. An integer MUST be encoded in minimum number of octets");
            return E_ASN1_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode BerInputStream::ReadOctetString()
{
    if (mTag == IASN1Constants::TAG_OCTETSTRING) {
        ReadContent();
    } else if (mTag == IASN1Constants::TAG_C_OCTETSTRING) {
        //throw new ASN1Exception("Decoding constructed ASN.1 octet string type is not supported");
        return E_ASN1_EXCEPTION;
    } else {
        //throw expected("octetstring");
        return E_ASN1_EXCEPTION;
    }
    return NOERROR;
}

ECode BerInputStream::SetOidElement(
    /* [in] */ Int32 oidElement)
{
    mOidElement = oidElement;
    return NOERROR;
}

ECode BerInputStream::GetOidElement(
    /* [out] */ Int32* oidElement)
{
    VALIDATE_NOT_NULL(oidElement)
    *oidElement = mOidElement;
    return NOERROR;
}

ECode BerInputStream::ReadOID()
{
    if (mTag != IASN1Constants::TAG_OID) {
        //throw expected("OID");
        return E_ASN1_EXCEPTION;
    }

    // check encoded length
    if (mLength < 1) {
        //throw new ASN1Exception("Wrong length for ASN.1 object identifier at [" + tagOffset + "]");
        return E_ASN1_EXCEPTION;
    }

    ReadContent();

    // check content: last encoded byte (8th bit MUST be zero)
    if (((*mBuffer)[mOffset - 1] & 0x80) != 0) {
        //throw new ASN1Exception("Wrong encoding at [" + (offset - 1) + "]");
        return E_ASN1_EXCEPTION;
    }

    mOidElement = 1;
    for (Int32 i = 0; i < mLength; i++, ++mOidElement) {
        while (((*mBuffer)[mContentOffset + i] & 0x80) == 0x80) {
            i++;
        }
    }
    return NOERROR;
}

ECode BerInputStream::ReadSequence(
    /* [out] */ IASN1Sequence** sequence)
{
    if (mTag != IASN1Constants::TAG_C_SEQUENCE) {
        //throw expected("sequence");
        return E_ASN1_EXCEPTION;
    }

    Int32 begOffset = mOffset;
    Int32 endOffset = begOffset + mLength;

    AutoPtr<ArrayOf<IASN1Type*> > type;
    sequence->GetType((ArrayOf<IASN1Type*>**)&type);

    Int32 i = 0;

    if (mIsVerify) {

        for (; (mOffset < endOffset) && (i < type->GetLength()); i++) {

            Next();
            Boolean chk;
            while ((*type)[i]->CheckTag(mTag, &chk), !chk) {
                // check whether it is optional component or not
                AutoPtr<ArrayOf<Boolean> > option;
                sequence->GetOptional((ArrayOf<Boolean>**)&option);
                if (!(*option)[i] || (i == type->GetLength() - 1)) {
                    //throw new ASN1Exception("ASN.1 Sequence: mandatory value is missing at [" + tagOffset + "]");
                    return E_ASN1_EXCEPTION;
                }
                i++;
            }
            AutoPtr<IInterface> ret;
            (*type)[i]->DecodeEx3(this, (IInterface**)&ret);
        }

        // check the rest of components
        AutoPtr<ArrayOf<Boolean> > option;
        sequence->GetOptional((ArrayOf<Boolean>**)&option);
        for (; i < type->GetLength(); i++) {
            if (!(*option)[i]) {
                //throw new ASN1Exception("ASN.1 Sequence: mandatory value is missing at [" + tagOffset + "]");
                return E_ASN1_EXCEPTION;
            }
        }

    } else {
        Int32 seqTagOffset = mTagOffset; //store tag offset

        //Object[] values = new Object[type.length];
        AutoPtr<ArrayOf<IInterface*> > values = ArrayOf<IInterface*>::Alloc(type->GetLength());
        for (; (mOffset < endOffset) && (i < type->GetLength()); i++) {
            Int next;
            Next(&next);
            Boolean chk;
            while ((*type)[i]->CheckTag(mTag, &chk), !chk) {
                // check whether it is optional component or not
                AutoPtr<ArrayOf<Boolean> > option;
                sequence->GetOptional((ArrayOf<Boolean>**)&option);
                if (!(*option)[i] || (i == type->GetLength() - 1)) {
                    //throw new ASN1Exception("ASN.1 Sequence: mandatory value is missing at [" + tagOffset + "]");
                    return E_ASN1_EXCEPTION;
                }

                // sets default value
                AutoPtr<ArrayOf<IInterface*> > def;
                sequence->GetDefault((ArrayOf<IInterface*>**)&def);
                if ((*def)[i]) != NULL) {
                    values->Set(i, (*def)[i]);
                }
                i++;
            }
            (*type)[i]->DecodeEx3(this, (IInterface**)&(*values)[i])
        }

        // check the rest of components
        AutoPtr<ArrayOf<IInterface*> > def;
        sequence->GetDefault((ArrayOf<IInterface*>**)&def);
        AutoPtr<ArrayOf<Boolean> > option;
        sequence->GetOptional((ArrayOf<Boolean>**)&option);
        for (; i < type->GetLength(); i++) {
            if (!(*option)[i]) {
                //throw new ASN1Exception("ASN.1 Sequence: mandatory value is missing at [" + tagOffset + "]");
                return E_ASN1_EXCEPTION;
            }
            if ((*def)[i] != NULL) {
                values->Set(i, (*def)[i]);
            }
        }
        AutoPtr<IArrayOf> arr;
        CArrayOf::New(EIID_IInterface, values->GetLength(), (IArrayOf**)&arr);
        for(Int32 i = 0; i < values->GetLength(); i++) {
            arr->Set(i, (*values)[i]);
        }
        content = arr.Get();

        mTagOffset = seqTagOffset; //retrieve tag offset
    }

    if (mOffset != endOffset) {
        //throw new ASN1Exception("Wrong encoding at [" + begOffset + "]. Content's length and encoded length are not the same");
        return E_ASN1_EXCEPTION;
    }
}

ECode BerInputStream::ReadSequenceOf(
    /* [in] */ IASN1SequenceOf* sequenceOf)
{
    if (mTag != IASN1Constants::TAG_C_SEQUENCEOF) {
        //throw expected("sequenceOf");
        return E_ASN1_EXCEPTION;
    }

    return DecodeValueCollection(sequenceOf);
}

ECode BerInputStream::ReadSet(
    /* [in] */ IASN1Set* set)
{
    if (mTag != IASN1Constants::TAG_C_SET) {
        //throw expected("set");
        return E_ASN1_EXCEPTION;
    }

    //throw new ASN1Exception("Decoding ASN.1 Set type is not supported");
    return E_ASN1_EXCEPTION;
}

ECode BerInputStream::ReadSetOf(
    /* [in] */ IASN1SetOf* setOf)
{
    if (mTag != IASN1Constants::TAG_C_SETOF) {
        //throw expected("setOf");
        return E_ASN1_EXCEPTION;
    }

    return DecodeValueCollection(setOf);
}

ECode BerInputStream::DecodeValueCollection(
    /* [in] */ IASN1ValueCollection* collection)
{
    Int32 begOffset = mOffset;
    Int32 endOffset = begOffset + mLength;

    AutoPtr<IASN1Type> type;
    collection->GetType((IASN1Type**)&type);

    if (mIsVerify) {
        while (endOffset > mOffset) {
            Int32 ret;
            Next(&ret);
            AutoPtr<IInterface> tmp;
            type->DecodeEx3(this, (IInterface**)&tmp);
        }
    } else {
        Int32 seqTagOffset = mTagOffset; //store tag offset

        AutoPtr<IArrayList> values;
        CArrayList::New((IArrayList**)&values);
        while (endOffset > mOffset) {
            Int32 ret;
            Next(&ret);
            AutoPtr<IInterface> elem;
            type->DecodeEx3(this, (IInterface**)&elem);
            values->Add(elem);
        }

        values->TrimToSize();
        mContent = values;

        mTagOffset = seqTagOffset; //retrieve tag offset
    }

    if (mOffset != endOffset) {
        //throw new ASN1Exception("Wrong encoding at [" + begOffset + "]. Content's length and encoded length are not the same");
        return E_ASN1_EXCEPTION;
    }
    return NOERROR;
}

ECode BerInputStream::ReadString(
    /* [in] */ IASN1StringType* type)
{
    Int32 id, constrId;
    type->GetId(&id);
    type->GetConstrId(&constrId);
    if (mTag == id) {
        ReadContent();
    } else if (mTag == constrId) {
        //throw new ASN1Exception("Decoding constructed ASN.1 string type is not provided");
        return E_ASN1_EXCEPTION;
    } else {
        //throw expected("string");
        return E_ASN1_EXCEPTION;
    }
}

ECode BerInputStream::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    VALIDATE_NOT_NULL(encoded)
    AutoPtr<ArrayOf<Byte> > enc = ArrayOf<Byte>::Alloc(mOffset - mTagOffset);
    enc->Copy(0, mBuffer, mTagOffset, enc->GetLength());
    *encoded = enc;
    REFCOUNT_ADD(*encoded)
    return NOERROR;
}

ECode BerInputStream::GetBuffer(
    /* [out, callee] */ ArrayOf<Byte>** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    *buffer = mBuffer;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode BerInputStream::GetOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset)
    *offset = mOffset;
    return NOERROR;
}

ECode BerInputStream::GetEndOffset(
    /* [out] */ Int32* endOffset)
{
    VALIDATE_NOT_NULL(endOffset)
    *endOffset = mOffset + mLength;
    return NOERROR;
}

ECode BerInputStream::GetTagOffset(
    /* [out] */ Int32* tagOffset)
{
    VALIDATE_NOT_NULL(tagOffset)
    *tagOffset = mTagOffset;
    return NOERROR;
}

ECode BerInputStream::SetVerify()
{
    mIsVerify = TRUE;
    return NOERROR;
}

ECode BerInputStream::Read(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    if (mOffset == mBuffer->GetLength()) {
        //throw new ASN1Exception("Unexpected end of encoding");
        return E_ASN1_EXCEPTION;
    }

    if (mIn == NULL) {
        *ret = (*mBuffer)[mOffset++] & 0xFF;
    } else {
        Int32 octet;
        mIn->Read(&octet);
        if (octet == -1) {
            //throw new ASN1Exception("Unexpected end of encoding");
            return E_ASN1_EXCEPTION;
        }

        (*mBuffer)[mOffset++] = (Byte)octet;

        *ret = octet;
    }
    return NOERROR;
}

ECode BerInputStream::ReadContent()
{
    if (mOffset + mLength > mBuffer->GetLength()) {
        //throw new ASN1Exception("Unexpected end of encoding");
        return E_ASN1_EXCEPTION;
    }

    if (mIn == NULL) {
        mOffset += mLength;
    } else {
        Int32 bytesRead;
        mIn->Read(mBuffer, mOffset, mLength, &bytesRead);

        if (bytesRead != mLength) {
            // if input stream didn't return all data at once
            // try to read it in several blocks
            Int32 c = bytesRead;
            do {
                if (c < 1 || bytesRead > mLength) {
                    //throw new ASN1Exception("Failed to read encoded content");
                    return E_ASN1_EXCEPTION;
                }
                mIn->Read(mBuffer, mOffset + bytesRead, mLength - bytesRead, &c);
                bytesRead += c;
            } while (bytesRead != mLength);
        }

        mOffset += mLength;
    }
    return NOERROR;
}

ECode BerInputStream::CompactBuffer()
{
    if (mOffset != mBuffer->GetLength()) {
        AutoPtr<ArrayOf<Byte> > newBuffer = ArrayOf<Byte>::Alloc(mOffset);
        // restore buffer content
        newBuffer->Copy(0, mBuffer, 0, mOffset)
        // set new buffer
        mBuffer = newBuffer;
    }
    return NOERROR;
}

ECode BerInputStream::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* entry)
{
    if (mPool == NULL) {
        mPool = ArrayOf<IArrayOf*>::Alloc(2);
        CArrayOf::New(EIID_IInterface, 10, (IArrayOf**)&(*mPool)[0]);
        CArrayOf::New(EIID_IInterface, 10, (IArrayOf**)&(*mPool)[1]);
    }

    Int32 i = 0, length0;
    (*mPool)[0]->GetLength(&length0);
    AutoPtr<IInterface> tmp;
    for (; i < length0 && ((*mPool)[0]->Get(i, (IInterface**)&tmp), tmp) != NULL; i++) {
        if (tmp == key) {
            (*mPool)[1]->Set(i, entry);
            return NOERROR;
        }

        tmp = NULL;
    }

    //Todo
    /*
        if (i == length0) {
            Object[][] newPool = new Object[pool[0].length * 2][2];
            System.arraycopy(pool[0], 0, newPool[0], 0, pool[0].length);
            System.arraycopy(pool[1], 0, newPool[1], 0, pool[0].length);
            pool = newPool;
        } else {
            pool[0][i] = key;
            pool[1][i] = entry;
        }
    */
    return NOERROR;
}

ECode BerInputStream::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** entry)
{
    if (mPool == NULL) {
        return NOERROR;
    }

    Int32 length0;
    (*mPool)[0]->GetLength(&length0);
    for (Int32 i = 0; i < length0; i++) {
        AutoPtr<IInterface> tmp;
        (*mPool)[0]->Get(i, (IInterface**)&tmp);
        if (tmp == key) {
            *entry = tmp;
            REFCOUNT_ADD(*entry)
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode BerInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return InnerConstructor(encoded, 0, encoded->GetLength());
}

ECode BerInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 expectedLength)
{
    return InnerConstructor(encoded, offset, expectedLength);
}

ECode BerInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return InnerConstructorEx(is, BUF_INCREASE_SIZE);
}

ECode BerInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 initialSize)
{
    return InnerConstructorEx(is, initialSize);
}

ECode BerInputStream::InnerConstructor(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 expectedLength)
{
    mIn = NULL;
    mBuffer = encoded;
    mOffset = offset;

    Next();

    // compare expected and decoded length
    if (mLength != INDEFINIT_LENGTH
            && (offset + expectedLength) != (mOffset + mLength)) {
        return E_ASN1_EXCEPTION;
        //throw new ASN1Exception("Wrong content length");
    }
    return NOERROR;
}

ECode BerInputStream::InnerConstructorEx(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 initialSize)
{
    mIn = is;
    mBuffer = ArrayOf<Byte>::Alloc(initialSize);

    Next();

    if (mLength != INDEFINIT_LENGTH) {
        // input stream has definite length encoding
        // check allocated length to avoid further reallocations
        if (mBuffer->GetLength() < (mLength + mOffset)) {
            AutoPtr<ArrayOf<Byte> > newBuffer = ArrayOf<Byte>::Alloc(mLength + mOffset);
            newBuffer->Copy(0, mBuffer, 0, mOffset);
            mBuffer = newBuffer;
        }
    } else {
        mIsIndefinedLength = TRUE;
        return E_ASN1_EXCEPTION;
        //throw new ASN1Exception("Decoding indefinite length encoding is not supported");
    }
    return NOERROR;
}

ECode BerInputStream::StrToInt(
    /* [in] */ Int32 off,
    /* [in] */ Int32 count,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    Int32 result = 0;
    for (Int32 i = off, end = off + count; i < end; i++) {
        Int32 c = (*mBuffer)[i] - 48;
        if (c < 0 || c > 9) {
            //throw new ASN1Exception("Time encoding has invalid char");
            return E_ASN1_EXCEPTION;
        }
        result = result * 10 + c;
    }
    *ret = result;
    return NOERROR;
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

