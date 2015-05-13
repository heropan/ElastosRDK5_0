#include "CDnsSdTxtRecord.h"
#include "ext/frameworkdef.h"
#include <elastos/Slogger.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

const Byte CDnsSdTxtRecord::mSeperator = '=';

CDnsSdTxtRecord::CDnsSdTxtRecord()
{
}

CDnsSdTxtRecord::~CDnsSdTxtRecord()
{
}

ECode CDnsSdTxtRecord::constructor()
{
    mData = ArrayOf<Byte>::Alloc(0);
    return NOERROR;
}

ECode CDnsSdTxtRecord::constructor(
    /* [in] */ ArrayOf<Byte>* data)
{
    mData = data->Clone();
    return NOERROR;
}

ECode CDnsSdTxtRecord::constructor(
    /* [in] */ IDnsSdTxtRecord* src)
{
    if (src != NULL) {
        src->GetRawData((ArrayOf<Byte>**)&mData);
    }
    return NOERROR;
}

ECode CDnsSdTxtRecord::Set(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    if (key.IsNull()) {
        Slogger::E("CDnsSdTxtRecord", "key can not be null.");
        return NOERROR;
    }

    AutoPtr<ArrayOf<Byte> > keyBytes;
    AutoPtr<ArrayOf<Byte> > valBytes;
    Int32 valLen = 0;

    if (!value.IsNull()) {
        valBytes = ArrayOf<Byte>::Alloc((Byte*)value.string(), value.GetByteLength());
        valLen = valBytes->GetLength();
    }

    // try {
        keyBytes = ArrayOf<Byte>::Alloc((Byte*)key.string(), key.GetByteLength());
    // }
    // catch (java.io.UnsupportedEncodingException e) {
    //     throw new IllegalArgumentException("key should be US-ASCII");
    // }

    for (Int32 i = 0; i < keyBytes->GetLength(); i++) {
        if ((*keyBytes)[i] == '=') {
            Slogger::E("CDnsSdTxtRecord", "= is not a valid character in key");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    if (keyBytes->GetLength() + valLen >= 255) {
        Slogger::E("CDnsSdTxtRecord", "Key and Value length cannot exceed 255 bytes");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 currentLoc;
    Remove(key, &currentLoc);
    if (currentLoc == -1)
        KeyCount(&currentLoc);

    Insert(keyBytes, valBytes, currentLoc);
    return NOERROR;
}

ECode CDnsSdTxtRecord::Get(
    /* [in] */ const String& key,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<ArrayOf<Byte> > val;
    GetValue(key, (ArrayOf<Byte>**)&val);
    if (val != NULL) {
        *str = String((const char*)val->GetPayload());
    }
    else {
        *str = String(NULL);
    }
    return NOERROR;
}

ECode CDnsSdTxtRecord::Remove(
    /* [in] */ const String& key,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    *val = -1;

    Int32 avStart = 0, avLen, offset;
    Int32 keyByteLength = key.GetByteLength();
    for (Int32 i = 0; avStart < mData->GetLength(); i++) {
        avLen = (*mData)[avStart];
        if (keyByteLength <= avLen &&
                (keyByteLength == avLen || (*mData)[avStart + keyByteLength + 1] == mSeperator)) {
            String s((char *)mData->GetPayload() + avStart + 1, keyByteLength);
            if (key.EqualsIgnoreCase(s)) {
                AutoPtr<ArrayOf<Byte> > oldBytes = mData;
                mData = ArrayOf<Byte>::Alloc(oldBytes->GetLength() - avLen - 1);
                mData->Copy(oldBytes, avStart);
                offset = avStart + avLen + 1;
                mData->Copy(avStart, oldBytes, offset, oldBytes->GetLength() - offset);
                *val = i;
                return NOERROR;
            }
        }
        avStart += (0xFF & (avLen + 1));
    }

    return NOERROR;
}

ECode CDnsSdTxtRecord::KeyCount(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);

    Int32 count = 0, nextKey;
    for (nextKey = 0; nextKey < mData->GetLength(); count++) {
        nextKey += (0xFF & ((*mData)[nextKey] + 1));
    }
    *val = count;
    return NOERROR;
}

ECode CDnsSdTxtRecord::Contains(
    /* [in] */ const String& key,
    /* [out] */ Boolean* val)
{
    VALIDATE_NOT_NULL(val);

    String s;
    for (Int32 i = 0; (s = GetKey(i), !s.IsNull()); i++) {
        if (key.EqualsIgnoreCase(s)) {
            *val = TRUE;
            return NOERROR;
        }
    }

    *val = FALSE;
    return NOERROR;
}

/* Gets the size in bytes */
ECode CDnsSdTxtRecord::Size(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    *val = mData->GetLength();
    return NOERROR;
}

/* Gets the raw data in bytes */
ECode CDnsSdTxtRecord::GetRawData(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mData->Clone();
    INTERFACE_ADDREF(*data);
    return NOERROR;
}

void CDnsSdTxtRecord::Insert(
    /* [in] */ ArrayOf<Byte>* keyBytes,
    /* [in] */ ArrayOf<Byte>* value,
    /* [in] */ Int32 index)
{
    AutoPtr<ArrayOf<Byte> > oldBytes = mData;
    Int32 valLen = (value != NULL) ? value->GetLength() : 0;
    Int32 insertion = 0;
    Int32 newLen, avLen;

    for (Int32 i = 0; i < index && insertion < mData->GetLength(); i++) {
        insertion += (0xFF & ((*mData)[insertion] + 1));
    }

    avLen = keyBytes->GetLength() + valLen + (value != NULL ? 1 : 0);
    newLen = avLen + oldBytes->GetLength() + 1;

    mData = ArrayOf<Byte>::Alloc(newLen);
    mData->Copy(oldBytes, insertion);


    Int32 secondHalfLen = oldBytes->GetLength() - insertion;
    mData->Copy(newLen - secondHalfLen, oldBytes, insertion, secondHalfLen);
    (*mData)[insertion] = (Byte) avLen;
    mData->Copy(insertion + 1, keyBytes, 0, keyBytes->GetLength());

    if (value != NULL) {
        (*mData)[insertion + 1 + keyBytes->GetLength()] = mSeperator;
        mData->Copy(insertion + keyBytes->GetLength() + 2, value, 0, valLen);
    }
}

String CDnsSdTxtRecord::GetKey(
    /* [in] */ Int32 index)
{
    Int32 avStart = 0;

    for (Int32 i=0; i < index && avStart < mData->GetLength(); i++) {
        avStart += (*mData)[avStart] + 1;
    }

    if (avStart < mData->GetLength()) {
        Int32 avLen = (*mData)[avStart];
        Int32 aLen = 0;

        for (aLen=0; aLen < avLen; aLen++) {
            if ((*mData)[avStart + aLen + 1] == mSeperator) break;
        }

        return String((const char*)mData->GetPayload() + avStart + 1, aLen);
    }
    return String(NULL);
}

ECode CDnsSdTxtRecord::GetValue(
    /* [in] */ Int32 index,
    /* [out] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);

    Int32 avStart = 0;
    for (Int32 i=0; i < index && avStart < mData->GetLength(); i++) {
        avStart += (*mData)[avStart] + 1;
    }

    AutoPtr<ArrayOf<Byte> > array;
    if (avStart < mData->GetLength()) {
        Int32 avLen = (*mData)[avStart];
        Int32 aLen = 0;

        for (aLen=0; aLen < avLen; aLen++) {
            if ((*mData)[avStart + aLen + 1] == mSeperator) {
                array = ArrayOf<Byte>::Alloc(avLen - aLen - 1);
                array->Copy(mData,  avStart + aLen + 2, avLen - aLen - 1);
                break;
            }
        }
    }

    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
}

String CDnsSdTxtRecord::GetValueAsString(
    /* [in] */ Int32 index)
{
    AutoPtr<ArrayOf<Byte> > value;
    GetValue(index, (ArrayOf<Byte>**)&value);
    return value != NULL ? String((const char*)value->GetPayload()) : String(NULL);
}

ECode CDnsSdTxtRecord::GetValue(
    /* [in] */ const String& forKey,
    /* [out, callee] */ ArrayOf<Byte>** val)
{
    VALIDATE_NOT_NULL(val);

    String s;
    for (Int32 i = 0; (s = GetKey(i), !s.IsNull()); i++) {
        if (forKey.EqualsIgnoreCase(s)) {
            return GetValue(i, val);
        }
    }

    return NOERROR;
}

ECode CDnsSdTxtRecord::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* val)
{
    VALIDATE_NOT_NULL(val);
    return EqualsEx(IDnsSdTxtRecord::Probe(o), val);
}

ECode CDnsSdTxtRecord::EqualsEx(
    /* [in] */ IDnsSdTxtRecord* o,
    /* [out] */ Boolean* val)
{
    VALIDATE_NOT_NULL(val);
    *val = FALSE;

    if (IDnsSdTxtRecord::Probe(o)) {
        *val = TRUE;
        return NOERROR;
    }

    CDnsSdTxtRecord* record = (CDnsSdTxtRecord*)o;
    if (record == NULL) {
        *val = FALSE;
        return NOERROR;
    }

    *val = ArrayOf<Byte>::Equals(record->mData, mData);
    return NOERROR;
}

ECode CDnsSdTxtRecord::GetHashCode(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    *val = (Int32)mData.Get();
    return NOERROR;
}

ECode CDnsSdTxtRecord::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteArrayOf((Handle32)mData.Get());
}

ECode CDnsSdTxtRecord::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mData = NULL;
    return source->ReadArrayOf((Handle32*)&mData);
}

ECode CDnsSdTxtRecord::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);

    String a, result, val;

    StringBuilder av;
    for (Int32 i = 0; (a = GetKey(i), !a.IsNull()); i++) {
        if (i != 0) {
            av += ", ";
        }
        av += "{";
        av += a;
        val = GetValueAsString(i);
        if (!val.IsNull()) {
            av += "=";
            av += val;
            av += "}";
        }
        else
            av += "}";
    }

    result = av.ToString();
    *info = !result.IsNull() ? result : String("");
    return NOERROR;
}

}
}
}
}
