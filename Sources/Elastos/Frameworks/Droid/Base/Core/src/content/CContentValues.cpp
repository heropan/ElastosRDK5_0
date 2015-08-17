
#include "content/CContentValues.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::INumber;
using Elastos::Core::CByte;
using Elastos::Core::CString;
using Elastos::Core::CInteger16;
using Elastos::Core::CInteger32;
using Elastos::Core::CInteger64;
using Elastos::Core::CFloat;
using Elastos::Core::CDouble;
using Elastos::Core::CBoolean;
using Elastos::Core::CArrayOf;
using Elastos::Utility::CObjectStringMap;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Content {

const Int32 CContentValues::VAL_NULL;
const Int32 CContentValues::VAL_STRING;
const Int32 CContentValues::VAL_BYTE;
const Int32 CContentValues::VAL_INTEGER16;
const Int32 CContentValues::VAL_INTEGER32;
const Int32 CContentValues::VAL_INTEGER64;
const Int32 CContentValues::VAL_FLOAT;
const Int32 CContentValues::VAL_DOUBLE;
const Int32 CContentValues::VAL_BOOLEAN;
const Int32 CContentValues::VAL_ARRAYOF;

const String CContentValues::TAG("CContentValues");

CContentValues::CContentValues()
{}

CContentValues::~CContentValues()
{
}

ECode CContentValues::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (_CObject_Compare((IContentValues*) this, object)) {
        *result = TRUE;
        return NOERROR;
    }

    if (IContentValues::Probe(object) != NULL) {
        Int32 size = 0;
        FAIL_RETURN(((IContentValues*) object)->GetSize(&size))
        if (size != mValues.GetSize()) {
            *result = FALSE;
            return NOERROR;
        }

        Boolean isContain = FALSE;
        String key;
        AutoPtr<IInterface> value;
        HashMap<String, AutoPtr<IInterface> >::Iterator it;

        for (it = mValues.Begin(); it != mValues.End(); ++it) {
            key = it->mFirst;
            value = it->mSecond;
            AutoPtr<IInterface> otherValue;
            FAIL_RETURN(((IContentValues*) object)->Get(key, (IInterface**)&otherValue))
            if (NULL == value ) {
                if (NULL != otherValue || !(((IContentValues*) object)->ContainsKey(key, &isContain), isContain)) {
                    *result = FALSE;
                    return NOERROR;
                }
            }
            else if (!_CObject_Compare(value, otherValue)) {
                *result = FALSE;
                return NOERROR;
            }
        }

        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

ECode CContentValues::GetHashCode(
    /* [out] */ Int32* hashcode)
{
    VALIDATE_NOT_NULL(hashcode)
    *hashcode = (Int32)this;
    return NOERROR;
}

ECode CContentValues::PutString(
    /* [in] */ const String& key,
    /* [in] */ ICharSequence* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutAll(
    /* [in] */ IContentValues* contentvalues)
{
    VALIDATE_NOT_NULL(contentvalues)
    AutoPtr<ArrayOf<String> > keyArray;
    FAIL_RETURN(contentvalues->KeySet((ArrayOf<String>**)&keyArray))

    if (NULL != keyArray) {
        String key;
        AutoPtr<IInterface> value;
        for (Int32 i = 0; i < keyArray->GetLength(); i++) {
            key = (*keyArray)[i];
            value = NULL;
            FAIL_RETURN(contentvalues->Get(key, (IInterface**)&value))
            mValues[key] = value;
        }
    }

    return NOERROR;
}

ECode CContentValues::PutByte(
    /* [in] */ const String& key,
    /* [in] */ IByte* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutInt16(
    /* [in] */ const String& key,
    /* [in] */ IInteger16* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ IInteger32* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ IInteger64* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ IFloat* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ IDouble* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ IBoolean* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutBytes(
    /* [in] */ const String& key,
    /* [in] */ IArrayOf* value)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = value;
    return NOERROR;
}

ECode CContentValues::PutNull(
    /* [in] */ const String& key)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    mValues[key] = (IInterface*)NULL;
    return NOERROR;
}

ECode CContentValues::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = mValues.GetSize();
    return NOERROR;
}

ECode CContentValues::Remove(
    /* [in] */ const String& key)
{
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mValues.Find(key);
    if (it == mValues.End()) return NOERROR;
    mValues.Erase(it);
    return NOERROR;
}

ECode CContentValues::Clear()
{
    mValues.Clear();
    return NOERROR;
}

ECode CContentValues::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result =  FALSE;
    if (key.IsNull()) return E_INVALID_ARGUMENT;

    HashMap<String, AutoPtr<IInterface> >::Iterator it = mValues.Find(key);
    *result = (it != mValues.End());
    return NOERROR;
}

ECode CContentValues::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mValues.Find(key);
    if (it == mValues.End()) {
        return NOERROR;
    }

    *value = it->mSecond;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CContentValues::GetAsString(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    *value = String(NULL);
    if (key.IsNull()) return E_INVALID_ARGUMENT;
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mValues.Find(key);
    if (it == mValues.End()) {
        return NOERROR;
    }

    AutoPtr<IInterface> obj = it->mSecond;
    if (ICharSequence::Probe(obj) != NULL) {
        FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(value))
    }
    else if (IInteger16::Probe(obj) != NULL) {
        Int16 tmp = 0;
        FAIL_RETURN(((IInteger16*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::Int32ToString((Int32)tmp);
    }
    else if (IInteger32::Probe(obj) != NULL) {
        Int32 tmp = 0;
        FAIL_RETURN(((IInteger32*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::Int32ToString(tmp);
    }
    else if (IInteger64::Probe(obj) != NULL) {
        Int64 tmp = 0;
        FAIL_RETURN(((IInteger64*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::Int64ToString(tmp);
    }
    else if (IBoolean::Probe(obj) != NULL) {
        Boolean tmp = FALSE;
        FAIL_RETURN(((IBoolean*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::BooleanToString(tmp);
    }
    else if (IFloat::Probe(obj) != NULL) {
        Float tmp = 0.0f;
        FAIL_RETURN(((IFloat*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::DoubleToString((Double)tmp);
    }
    else if (IDouble::Probe(obj) != NULL) {
        Double tmp = 0.0;
        FAIL_RETURN(((IDouble*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::DoubleToString(tmp);
    }
    else if (IByte::Probe(obj) != NULL) {
        Byte tmp = 0;
        FAIL_RETURN(((IByte*) (IInterface*) obj)->GetValue(&tmp))
        *value = StringUtils::Int32ToString((Int32)tmp);
    }
    else {
        *value = String(NULL);
    }

    return NOERROR;
}

ECode CContentValues::GetAsInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (INumber::Probe(obj) != NULL) {
            FAIL_RETURN(((INumber*) (IInterface*) obj)->Int64Value(value))
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = StringUtils::ParseInt64(str);
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Int64");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (INumber::Probe(obj) != NULL) {
            FAIL_RETURN(((INumber*) (IInterface*) obj)->Int32Value(value))
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = StringUtils::ParseInt32(str);
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Int32");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsInt16(
    /* [in] */ const String& key,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (INumber::Probe(obj) != NULL) {
            FAIL_RETURN(((INumber*) (IInterface*) obj)->Int16Value(value))
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = StringUtils::ParseInt16(str);
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Int16");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsByte(
    /* [in] */ const String& key,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (INumber::Probe(obj) != NULL) {
            Int16 tmpValue = 0;
            FAIL_RETURN(((INumber*) (IInterface*) obj)->Int16Value(&tmpValue))
            *value = (Byte)tmpValue;
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = (Byte)StringUtils::ParseInt32(str);
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Byte");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsDouble(
    /* [in] */ const String& key,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0.0;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (INumber::Probe(obj) != NULL) {
            FAIL_RETURN(((INumber*) (IInterface*) obj)->DoubleValue(value))
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = StringUtils::ParseDouble(str);
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Double");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsFloat(
    /* [in] */ const String& key,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0.0;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (INumber::Probe(obj) != NULL) {
            FAIL_RETURN(((INumber*) (IInterface*) obj)->FloatValue(value))
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = StringUtils::ParseFloat(str);
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Float");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = FALSE;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        if (IBoolean::Probe(obj) != NULL) {
            FAIL_RETURN(((IBoolean*) (IInterface*) obj)->GetValue(value))
            return NOERROR;
        }
        else if (INumber::Probe(obj) != NULL) {
            Int32 tmp = 0;
            FAIL_RETURN(((INumber*) (IInterface*) obj)->Int32Value(&tmp))
            *value = tmp != 0;
            return NOERROR;
        }
        else if (ICharSequence::Probe(obj) != NULL) {
            String str;
            FAIL_RETURN(((ICharSequence*) (IInterface*) obj)->ToString(&str))
            *value = str.EqualsIgnoreCase("TRUE");
            return NOERROR;
        }
        else {
            // E_CLASS_CAST_EXCEPTION
            Logger::E(TAG, String("Cannot cast value for ") + key + " to a Boolean");
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::GetAsByteArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;
    AutoPtr<IInterface> obj;
    FAIL_RETURN(Get(key, (IInterface**)&obj))

    if (NULL != obj) {
        AutoPtr<IArrayOf> array = IArrayOf::Probe(obj);
        if (array != NULL) {
            Int32 length = 0;
            FAIL_RETURN(array->GetLength(&length))
            AutoPtr<ArrayOf<Byte> > byteArray = ArrayOf<Byte>::Alloc(length);
            AutoPtr<IByte> byteObj;
            Byte b;
            for (Int32 i = 0; i != length; i++) {
                byteObj = NULL;
                FAIL_RETURN(array->Get(i, (IInterface**)&byteObj))
                FAIL_RETURN(byteObj->GetValue(&b))
                (*byteArray)[i] = b;
            }
            *value = byteArray;
            REFCOUNT_ADD(*value);
            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CContentValues::ValueSet(
    /* [out] */ IObjectStringMap** values)
{
    VALIDATE_NOT_NULL(values)
    *values = NULL;
    FAIL_RETURN(CObjectStringMap::New(values))
    HashMap<String, AutoPtr<IInterface> >::Iterator it;

    for (it = mValues.Begin(); it != mValues.End(); ++it) {
        FAIL_RETURN((*values)->Put(it->mFirst, it->mSecond))
    }

    return NOERROR;
}

ECode CContentValues::KeySet(
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value)
    Int32 len = mValues.GetSize();
    Int32 i = 0;
    AutoPtr<ArrayOf<String> > strArray = ArrayOf<String>::Alloc(len);
    HashMap<String, AutoPtr<IInterface> >::Iterator it;

    for (it = mValues.Begin(); it != mValues.End(); ++it) {
        (*strArray)[i] = it->mFirst;
        i++;
    }

    *value = strArray;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CContentValues::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    String strTmp("");
    HashMap<String, AutoPtr<IInterface> >::Iterator it;

    for (it = mValues.Begin(); it != mValues.End(); ++it) {
        if (it != mValues.Begin())
            strTmp.Append(" ");

        String key = it->mFirst;
        String value;
        FAIL_RETURN(GetAsString(key, &value))
        strTmp.Append(key);
        strTmp.Append("=");
        strTmp.Append(value);
    }

    *str = strTmp;
    return NOERROR;
}

ECode CContentValues::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mValues.Clear();
    Int32 size = 0;
    source->ReadInt32(&size);
    if (size == -1) {
        return NOERROR;
    }

    while (size > 0) {
        String key;
        source->ReadString(&key);
        AutoPtr<IInterface> value = ReadValue(source);
        mValues[key] = value;
        size--;
    }

    return NOERROR;
}

AutoPtr<IInterface> CContentValues::ReadValue(
    /* [in] */ IParcel* source)
{
    Int32 type;
    source->ReadInt32(&type);

    switch (type) {
    case VAL_NULL:
        return NULL;
    case VAL_BYTE: {
        Int32 v;
        source->ReadInt32(&v);
        AutoPtr<IByte> obj;
        CByte::New((Byte)v, (IByte**)&obj);
        return obj;
    }
    case VAL_STRING: {
        String v;
        source->ReadString(&v);
        AutoPtr<ICharSequence> obj;
        CString::New(v, (ICharSequence**)&obj);
        return obj;
    }
    case VAL_INTEGER16: {
        Int32 v;
        source->ReadInt32(&v);
        Int16 sv = (Int16)v;
        AutoPtr<IInteger16> obj;
        CInteger16::New(sv, (IInteger16**)&obj);
        return obj;
    }
    case VAL_INTEGER32: {
        Int32 v;
        source->ReadInt32(&v);
        AutoPtr<IInteger32> obj;
        CInteger32::New(v, (IInteger32**)&obj);
        return obj;
    }
    case VAL_INTEGER64: {
        Int64 v;
        source->ReadInt64(&v);
        AutoPtr<IInteger64> obj;
        CInteger64::New(v, (IInteger64**)&obj);
        return obj;
    }
    case VAL_FLOAT: {
        Float v;
        source->ReadFloat(&v);
        AutoPtr<IFloat> obj;
        CFloat::New(v, (IFloat**)&obj);
        return obj;
    }
    case VAL_DOUBLE: {
        Double v;
        source->ReadDouble(&v);
        AutoPtr<IDouble> obj;
        CDouble::New(v, (IDouble**)&obj);
        return obj;
    }
    case VAL_BOOLEAN: {
        Int32 v;
        source->ReadInt32(&v);
        AutoPtr<IBoolean> obj;
        CBoolean::New(v == 1, (IBoolean**)&obj);
        return obj;
    }
    case VAL_ARRAYOF: {
        AutoPtr<IArrayOf> array;
        InterfaceID iid;
        source->ReadEMuid(&iid);
        Int32 size = 0;
        source->ReadInt32(&size);
        CArrayOf::New(iid, size, (IArrayOf**)&array);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInterface> elem = ReadValue(source);
            array->Put(i, elem);
        }
        return array;
    }
    default:
        Logger::D("CContentValues", "- Unmarshalling unknown type code %d", type);
        assert(0);
    }
    return NULL;
}

ECode CContentValues::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (mValues.IsEmpty()) {
        dest->WriteInt32(-1);
        return NOERROR;
    }

    dest->WriteInt32(mValues.GetSize());
    HashMap<String, AutoPtr<IInterface> >::Iterator it;
    for (it = mValues.Begin(); it != mValues.End(); ++it) {
        dest->WriteString(it->mFirst);
        WriteValue(dest, it->mSecond);
    }
    return NOERROR;
}

ECode CContentValues::WriteValue(
    /* [in] */ IParcel* dest,
    /* [in] */ IInterface* obj)
{
    if (obj == NULL) {
        dest->WriteInt32(VAL_NULL);
    }
    else if (ICharSequence::Probe(obj) != NULL) {
        String v;
        ICharSequence::Probe(obj)->ToString(&v);
        dest->WriteInt32(VAL_STRING);
        dest->WriteString(v);
    }
    else if (IByte::Probe(obj) != NULL) {
        Byte v;
        IByte::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_BYTE);
        dest->WriteInt32((Int32)v);
    }
    else if (IInteger16::Probe(obj) != NULL) {
        Int16 v;
        IInteger16::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER16);
        dest->WriteInt32((Int32)v);
    }
    else if (IInteger32::Probe(obj) != NULL) {
        Int32 v;
        IInteger32::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER32);
        dest->WriteInt32(v);
    }
    else if (IInteger64::Probe(obj) != NULL) {
        Int64 v;
        IInteger64::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER64);
        dest->WriteInt64(v);
    }
    else if (IFloat::Probe(obj) != NULL) {
        Float v;
        IFloat::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_FLOAT);
        dest->WriteFloat(v);
    }
    else if (IDouble::Probe(obj) != NULL) {
        Double v;
        IDouble::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_DOUBLE);
        dest->WriteDouble(v);
    }
    else if (IBoolean::Probe(obj) != NULL) {
        Boolean v;
        IBoolean::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_BOOLEAN);
        dest->WriteInt32(v ? 1 : 0);
    }
    else if (IArrayOf::Probe(obj) != NULL){
        AutoPtr<IArrayOf> array = IArrayOf::Probe(obj);
        Int32 size = 0;
        array->GetLength(&size);
        dest->WriteInt32(VAL_ARRAYOF);
        InterfaceID iid;
        array->GetTypeId(&iid);
        dest->WriteEMuid(iid);
        dest->WriteInt32(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInterface> elem;
            array->Get(i, (IInterface**)&elem);
            WriteValue(dest, elem);
        }
    }
    else {
        assert(0);
        // throw new RuntimeException("Parcel: unable to marshal value " + v);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CContentValues::constructor()
{
    // Choosing a default size of 8 based on analysis of typical
    // consumption by applications.
    mValues.Resize(8);
    return NOERROR;
}

ECode CContentValues::constructor(
    /* [in] */ Int32 size)
{
    if (size < 0) {
        return E_INVALID_ARGUMENT;
    }
    mValues.Resize(size);
    return NOERROR;
}

ECode CContentValues::constructor(
    /* [in] */ IContentValues* from)
{
    VALIDATE_NOT_NULL(from)
    Int32 size = 0;
    FAIL_RETURN(from->GetSize(&size))
    if (size > 0) {
        mValues.Resize(size);
    }
    FAIL_RETURN(PutAll(from))
    return NOERROR;
}

}
}
}

