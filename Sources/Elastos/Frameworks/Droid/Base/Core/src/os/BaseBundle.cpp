
#include "os/BaseBundle.h"
#include <elastos/core/CoreUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>

//#include "util/CArrayMap.h"

#include <binder/Parcel.h>

using Elastos::Core::CoreUtils;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::IIterator;
using Elastos::Utility::IMapEntry;
//using Elastos::Droid::Utility::CArrayMap;

namespace Elastos {
namespace Droid {
namespace Os {

AutoPtr<IParcel> InitEMPTY_PARCEL()
{
    AutoPtr<IParcel> empty;
    CParcel::New((IParcel**)&empty);
    return empty;
}

CAR_INTERFACE_IMPL(BaseBundle, Object, IBaseBundle)

const String BaseBundle::TAG("Bundle");
const Boolean BaseBundle::DEBUG = FALSE;

const Int32 BaseBundle::BUNDLE_MAGIC = 0x4C444E42; // 'B' 'N' 'D' 'L'
const AutoPtr<IParcel> BaseBundle::EMPTY_PARCEL = InitEMPTY_PARCEL();

const Int32 BaseBundle::VAL_NULL = -1;
const Int32 BaseBundle::VAL_NOT_NULL = 0;
const Int32 BaseBundle::VAL_STRING = 1;
const Int32 BaseBundle::VAL_INTEGER32 = 2;
const Int32 BaseBundle::VAL_MAP = 3;
const Int32 BaseBundle::VAL_BUNDLE = 4;
const Int32 BaseBundle::VAL_PARCELABLE = 5;
const Int32 BaseBundle::VAL_INTEGER16 = 6;
const Int32 BaseBundle::VAL_INTEGER64 = 7;
const Int32 BaseBundle::VAL_FLOAT = 8;
const Int32 BaseBundle::VAL_DOUBLE = 9;
const Int32 BaseBundle::VAL_BOOLEAN = 10;
const Int32 BaseBundle::VAL_CHARSEQUENCE = 11;
const Int32 BaseBundle::VAL_LIST  = 12;
const Int32 BaseBundle::VAL_SPARSEARRAY = 13;
const Int32 BaseBundle::VAL_BYTEARRAY = 14;
const Int32 BaseBundle::VAL_STRINGARRAY = 15;
const Int32 BaseBundle::VAL_IBINDER = 16;
const Int32 BaseBundle::VAL_PARCELABLEARRAY = 17;
const Int32 BaseBundle::VAL_OBJECTARRAY = 18;
const Int32 BaseBundle::VAL_INTARRAY = 19;
const Int32 BaseBundle::VAL_LONGARRAY = 20;
const Int32 BaseBundle::VAL_BYTE = 21;
const Int32 BaseBundle::VAL_SERIALIZABLE = 22;
const Int32 BaseBundle::VAL_SPARSEBOOLEANARRAY = 23;
const Int32 BaseBundle::VAL_BOOLEANARRAY = 24;
const Int32 BaseBundle::VAL_CHARSEQUENCEARRAY = 25;
const Int32 BaseBundle::VAL_ARRAYOF = 26;

BaseBundle::BaseBundle()
{
}

ECode BaseBundle::constructor(
    /* [in] */ IClassLoader* loader,
    /* [in] */ Int32 capacity)
{
    if (capacity > 0) {
        //CArrayMap::New(capacity, (IArrayMap**)&mMap);
    }
    else {
        //CArrayMap::New((IArrayMap**)&mMap);
    }

    mClassLoader = loader;
    if (mClassLoader == NULL) {
        //mClassLoader = getClass().getClassLoader();
    }

    return NOERROR;
}

ECode BaseBundle::constructor()
{
    return constructor((IClassLoader*)NULL, 0);
}

ECode BaseBundle::constructor(
    /* [in] */ IParcel* parcelledData)
{
    return ReadFromParcelInner(parcelledData);
}

ECode BaseBundle::constructor(
    /* [in] */ IParcel* parcelledData,
    /* [in] */ Int32 length)
{
    return ReadFromParcelInner(parcelledData, length);
}

ECode BaseBundle::constructor(
    /* [in] */ IClassLoader* loader)
{
    return constructor(loader, 0);
}

ECode BaseBundle::constructor(
    /* [in] */ Int32 capacity)
{
    return constructor((IClassLoader*)NULL, capacity);
}

ECode BaseBundle::constructor(
    /* [in] */ IBaseBundle* b)
{
    // if (b.mParcelledData != null) {
    //     if (b.mParcelledData == EMPTY_PARCEL) {
    //         mParcelledData = EMPTY_PARCEL;
    //     } else {
    //         mParcelledData = Parcel.obtain();
    //         mParcelledData.appendFrom(b.mParcelledData, 0, b.mParcelledData.dataSize());
    //         mParcelledData.setDataPosition(0);
    //     }
    // } else {
    //     mParcelledData = null;
    // }

    // if (b.mMap != null) {
    //     mMap = new ArrayMap<String, Object>(b.mMap);
    // } else {
    //     mMap = null;
    // }

    // mClassLoader = b.mClassLoader;
    return NOERROR;
}

String BaseBundle::GetPairValue()
{
    // Unparcel();
    // int size = mMap.size();
    // if (size > 1) {
    //     Log.w(TAG, "getPairValue() used on Bundle with multiple pairs.");
    // }
    // if (size == 0) {
    //     return null;
    // }
    // Object o = mMap.valueAt(0);
    // try {
    //     return (String) o;
    // } catch (ClassCastException e) {
    //     typeWarning("getPairValue()", o, "String", e);
    //     return null;
    // }
    return String("");
}

void BaseBundle::SetClassLoader(
    /* [in] */ IClassLoader* loader)
{
    mClassLoader = loader;
}

AutoPtr<IClassLoader> BaseBundle::GetClassLoader()
{
    return mClassLoader;
}

void BaseBundle::Unparcel()
{
    // if (mParcelledData == null) {
    //     if (DEBUG) Log.d(TAG, "Unparcel " + Integer.toHexString(System.identityHashCode(this))
    //             + ": no parcelled data");
    //     return;
    // }

    // if (mParcelledData == EMPTY_PARCEL) {
    //     if (DEBUG) Log.d(TAG, "Unparcel " + Integer.toHexString(System.identityHashCode(this))
    //             + ": empty");
    //     if (mMap == null) {
    //         mMap = new ArrayMap<String, Object>(1);
    //     } else {
    //         mMap.erase();
    //     }
    //     mParcelledData = null;
    //     return;
    // }

    // int N = mParcelledData.readInt();
    // if (DEBUG) Log.d(TAG, "Unparcel " + Integer.toHexString(System.identityHashCode(this))
    //         + ": reading " + N + " maps");
    // if (N < 0) {
    //     return;
    // }
    // if (mMap == null) {
    //     mMap = new ArrayMap<String, Object>(N);
    // } else {
    //     mMap.erase();
    //     mMap.ensureCapacity(N);
    // }
    // mParcelledData.readArrayMapInternal(mMap, N, mClassLoader);
    // mParcelledData.recycle();
    // mParcelledData = null;
    // if (DEBUG) Log.d(TAG, "Unparcel " + Integer.toHexString(System.identityHashCode(this))
    //         + " final map: " + mMap);
}

Boolean BaseBundle::IsParceled()
{
    return mParcelledData != NULL;
}

ECode BaseBundle::GetSize(
    /* [out] */ Int32 * size)
{
    VALIDATE_NOT_NULL(size)
    Unparcel();
    return IMap::Probe(mMap)->GetSize(size);
}

ECode BaseBundle::IsEmpty(
    /* [out] */ Boolean* empty)
{
    VALIDATE_NOT_NULL(empty)
    Unparcel();
    return IMap::Probe(mMap)->IsEmpty(empty);
}

ECode BaseBundle::Clear()
{
    Unparcel();
    return IMap::Probe(mMap)->Clear();
}

ECode BaseBundle::ContainsKey(
    /* [in] */ String key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->ContainsKey(keyObj, result);
}

ECode BaseBundle::Get(
    /* [in] */ String key,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Get(keyObj, obj);
}

ECode BaseBundle::Remove(
    /* [in] */ String key)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Remove(keyObj);
}

ECode BaseBundle::PutAll(
    /* [in] */ IPersistableBundle* bundle)
{
    Unparcel();
    BaseBundle* bb = (BaseBundle*)bundle;
    bb->Unparcel();
    return IMap::Probe(mMap)->PutAll(IMap::Probe(bb->mMap));
}

ECode BaseBundle::PutAll(
    /* [in] */ IMap* map)
{
    Unparcel();
    return IMap::Probe(mMap)->PutAll(map);
}

ECode BaseBundle::GetKeySet(
    /* [out] */ ISet** set)
{
    VALIDATE_NOT_NULL(set)
    Unparcel();
    return IMap::Probe(mMap)->GetKeySet(set);
}

ECode BaseBundle::PutBoolean(
    /* [in] */ String key,
    /* [in] */ Boolean value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IBoolean> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutByte(
    /* [in] */ String key,
    /* [in] */ Byte value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IByte> valueObj = CoreUtils::ConvertByte(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutChar(
    /* [in] */ String key,
    /* [in] */ Char32 value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IChar32> valueObj = CoreUtils::ConvertChar32(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutInt16(
    /* [in] */ String key,
    /* [in] */ Int16 value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IInteger16> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutInt32(
    /* [in] */ String key,
    /* [in] */ Int32 value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IInteger32> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutInt64(
    /* [in] */ String key,
    /* [in] */ Int64 value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IInteger64> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutFloat(
    /* [in] */ String key,
    /* [in] */ Float value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IFloat> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutDouble(
    /* [in] */ String key,
    /* [in] */ Double value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IDouble> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());}

ECode BaseBundle::PutString(
    /* [in] */ String key,
    /* [in] */ String value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<ICharSequence> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutCharSequence(
    /* [in] */ String key,
    /* [in] */ ICharSequence* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Put(keyObj.Get(), value);
}

ECode BaseBundle::PutIntegerArrayList(
    /* [in] */ String key,
    /* [in] */ IArrayList* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Put(keyObj.Get(), value);
}

ECode BaseBundle::PutStringArrayList(
    /* [in] */ String key,
    /* [in] */ IArrayList* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Put(keyObj.Get(), value);
}

ECode BaseBundle::PutCharSequenceArrayList(
    /* [in] */ String key,
    /* [in] */ IArrayList* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Put(keyObj.Get(), value);
}

ECode BaseBundle::PutSerializable(
    /* [in] */ String key,
    /* [in] */ ISerializable* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    return IMap::Probe(mMap)->Put(keyObj.Get(), value);
}

ECode BaseBundle::PutBooleanArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Boolean>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutByteArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Byte>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::ConvertByteArray(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutCharArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Char32>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::ConvertChar32Array(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutInt16Array(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Int16>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutInt32Array(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Int32>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutInt64Array(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Int64>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutFloatArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Float>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutDoubleArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<Double>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutStringArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<String>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    return IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
}

ECode BaseBundle::PutCharSequenceArray(
    /* [in] */ String key,
    /* [in] */ ArrayOf<ICharSequence*>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> arrObj;
    if (value) {
        Int32 length = value->GetLength();
        CArrayOf::New(length, (IArrayOf**)&arrObj);
        for (Int32 i = 0; i < length; ++i) {
            arrObj->Set(i, (*value)[i]);
        }
    }

    return IMap::Probe(mMap)->Put(keyObj.Get(), arrObj.Get());
}

void BaseBundle::TypeWarning(
    /* [in] */ String key,
    /* [in] */ String className)
{
    StringBuilder sb("Error: Attempt to cast generated internal exception, Key: ");
    sb.Append(key);
    sb.Append(", className: ");
    sb.Append(className);
    Logger::W(TAG, sb.ToString());
}

AutoPtr<IInterface> BaseBundle::GetValue(
    /* [in] */ String key)
{
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IInterface> obj;
    IMap::Probe(mMap)->Get(keyObj.Get(), (IInterface**)&obj);
    return obj;
}

ECode BaseBundle::GetBoolean(
    /* [in] */ String key,
    /* [out] */ Boolean* value)
{
    return GetBoolean(key, FALSE, value);
}

ECode BaseBundle::GetBoolean(
    /* [in] */ String key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IBoolean::Probe(obj) == NULL) {
        TypeWarning(key, String("Boolean"));
        return NOERROR;
    }

    return IBoolean::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetByte(
    /* [in] */ String key,
    /* [out] */ Byte* value)
{
    return GetByte(key, (Byte)0, value);
}

ECode BaseBundle::GetByte(
    /* [in] */ String key,
    /* [in] */ Byte defaultValue,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IByte::Probe(obj) == NULL) {
        TypeWarning(key, String("Byte"));
        return NOERROR;
    }

    return IByte::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetChar(
    /* [in] */ String key,
    /* [out] */ Char32* value)
{
    return GetChar(key, (Char32)0, value);
}

ECode BaseBundle::GetChar(
    /* [in] */ String key,
    /* [in] */ Char32 defaultValue,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IChar32::Probe(obj) == NULL) {
        TypeWarning(key, String("Char32"));
        return NOERROR;
    }

    return IChar32::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetInt16(
    /* [in] */ String key,
    /* [out] */ Int16* value)
{
    return GetInt16(key, 0, value);
}

ECode BaseBundle::GetInt16(
    /* [in] */ String key,
    /* [in] */ Int16 defaultValue,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IInteger16::Probe(obj) == NULL) {
        TypeWarning(key, String("Int16"));
        return NOERROR;
    }

    return IInteger16::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetInt32(
    /* [in] */ String key,
    /* [out] */ Int32* value)
{
    return GetInt32(key, 0, value);
}

ECode BaseBundle::GetInt32(
    /* [in] */ String key,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IInteger32::Probe(obj) == NULL) {
        TypeWarning(key, String("Int32"));
        return NOERROR;
    }

    return IInteger32::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetInt64(
    /* [in] */ String key,
    /* [out] */ Int64* value)
{
    return GetInt64(key, 0, value);
}

ECode BaseBundle::GetInt64(
    /* [in] */ String key,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IInteger64::Probe(obj) == NULL) {
        TypeWarning(key, String("Int64"));
        return NOERROR;
    }

    return IInteger64::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetFloat(
    /* [in] */ String key,
    /* [out] */ Float* value)
{
    return GetFloat(key, 0, value);
}

ECode BaseBundle::GetFloat(
    /* [in] */ String key,
    /* [in] */ Float defaultValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IFloat::Probe(obj) == NULL) {
        TypeWarning(key, String("Float"));
        return NOERROR;
    }

    return IFloat::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetDouble(
    /* [in] */ String key,
    /* [out] */ Double* value)
{
    return GetDouble(key, 0, value);
}

ECode BaseBundle::GetDouble(
    /* [in] */ String key,
    /* [in] */ Double defaultValue,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (IDouble::Probe(obj) == NULL) {
        TypeWarning(key, String("Double"));
        return NOERROR;
    }

    return IDouble::Probe(obj)->GetValue(value);
}

ECode BaseBundle::GetString(
    /* [in] */ String key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (ICharSequence::Probe(obj) == NULL) {
        TypeWarning(key, String("String"));
        return NOERROR;
    }

    return ICharSequence::Probe(obj)->ToString(value);
}

ECode BaseBundle::GetString(
    /* [in] */ String key,
    /* [in] */ String defaultValue,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    *value = defaultValue;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }
    if (ICharSequence::Probe(obj) == NULL) {
        TypeWarning(key, String("String"));
        return NOERROR;
    }

    return ICharSequence::Probe(obj)->ToString(value);
}

ECode BaseBundle::GetCharSequence(
    /* [in] */ String key,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }

    if (ICharSequence::Probe(obj) == NULL) {
        TypeWarning(key, String("ICharSequence"));
        return NOERROR;
    }

    *value = ICharSequence::Probe(obj);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetCharSequence(
    /* [in] */ String key,
    /* [in] */ ICharSequence* defaultValue,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value)

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        *value = defaultValue;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }

    if (ICharSequence::Probe(obj) == NULL) {
        TypeWarning(key, String("ICharSequence"));
        *value = defaultValue;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }

    *value = ICharSequence::Probe(obj);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetSerializable(
    /* [in] */ String key,
    /* [out] */ ISerializable** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }

    if (ISerializable::Probe(obj) == NULL) {
        TypeWarning(key, String("ISerializable"));
        return NOERROR;
    }

    *value = ISerializable::Probe(obj);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetIntegerArrayList(
    /* [in] */ String key,
    /* [out] */ IArrayList** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }

    if (IArrayList::Probe(obj) == NULL) {
        TypeWarning(key, String("IntegerArrayList"));
        return NOERROR;
    }

    *value = IArrayList::Probe(obj);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetStringArrayList(
    /* [in] */ String key,
    /* [out] */ IArrayList** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }

    if (IArrayList::Probe(obj) == NULL) {
        TypeWarning(key, String("StringArrayList"));
        return NOERROR;
    }

    *value = IArrayList::Probe(obj);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetCharSequenceArrayList(
    /* [in] */ String key,
    /* [out] */ IArrayList** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NOERROR;
    }

    if (IArrayList::Probe(obj) == NULL) {
        TypeWarning(key, String("CharSequenceArrayList"));
        return NOERROR;
    }

    *value = IArrayList::Probe(obj);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

AutoPtr<IArrayOf> BaseBundle::GetIArrayOf(
    /* [in] */ String key,
    /* [in] */ String className)
{
    AutoPtr<IInterface> obj = GetValue(key);
    if (obj == NULL) {
        return NULL;
    }

    AutoPtr<IArrayOf> arrayOf = IArrayOf::Probe(obj);
    if (arrayOf == NULL) {
        TypeWarning(key, className);
        return NULL;
    }

    return arrayOf;
}

ECode BaseBundle::GetBooleanArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Boolean>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("BooleanArray"));
    AutoPtr<ArrayOf<Boolean> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Boolean>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Boolean item = FALSE;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IBoolean::Probe(obj) != NULL) {
                IBoolean::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetByteArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("ByteArray"));
    AutoPtr<ArrayOf<Byte> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Byte>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Byte item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IByte::Probe(obj) != NULL) {
                IByte::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetInt16Array(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Int16>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("Int16Array"));
    AutoPtr<ArrayOf<Int16> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Int16>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Int16 item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IInteger16::Probe(obj) != NULL) {
                IInteger16::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetCharArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Char32>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("Char32Array"));
    AutoPtr<ArrayOf<Char32> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Char32>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Char32 item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IChar32::Probe(obj) != NULL) {
                IChar32::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetInt32Array(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("Int32Array"));
    AutoPtr<ArrayOf<Int32> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Int32>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Int32 item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IInteger32::Probe(obj) != NULL) {
                IInteger32::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetInt64Array(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Int64>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("Int64Array"));
    AutoPtr<ArrayOf<Int64> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Int64>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Int64 item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IInteger64::Probe(obj) != NULL) {
                IInteger64::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetFloatArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Float>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("FloatArray"));
    AutoPtr<ArrayOf<Float> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Float>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Float item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IFloat::Probe(obj) != NULL) {
                IFloat::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetDoubleArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<Double>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("DoubleArray"));
    AutoPtr<ArrayOf<Double> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<Double>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            Double item = 0;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (IDouble::Probe(obj) != NULL) {
                IDouble::Probe(obj)->GetValue(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetStringArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("StringArray"));
    AutoPtr<ArrayOf<String> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<String>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            String item;
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            if (ICharSequence::Probe(obj) != NULL) {
                ICharSequence::Probe(obj)->ToString(&item);
            }
            array->Set(i, item);
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::GetCharSequenceArray(
    /* [in] */ String key,
    /* [out, callee] */ ArrayOf<ICharSequence*>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IArrayOf> arrObj = GetIArrayOf(key, String("CharSequenceArray"));
    AutoPtr<ArrayOf<ICharSequence*> > array;
    if (arrObj) {
        Int32 length;
        arrObj->GetLength(&length);
        array = ArrayOf<ICharSequence*>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            AutoPtr<IInterface> obj;
            arrObj->Get(i, (IInterface**)&obj);
            array->Set(i, ICharSequence::Probe(obj));
        }
    }

    *value = array;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode BaseBundle::WriteValue(
    /* [in] */ IParcel* dest,
    /* [in] */ IInterface* obj)
{
    if (obj == NULL) {
        dest->WriteInt32(VAL_NULL);
        return NOERROR;
    }
    // else if (v instanceof String) {
    //     writeInt(VAL_STRING);
    //     writeString((String) v);
    // }
    else if (IInteger32::Probe(obj) != NULL) {
        Int32 v;
        IInteger32::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER32);
        dest->WriteInt32(v);
        return NOERROR;
    }
    // else if (v instanceof Map) {
    //     writeInt(VAL_MAP);
    //     writeMap((Map) v);
    // }
    else if (IBundle::Probe(obj) != NULL) {
        // // Must be before Parcelable
        dest->WriteInt32(VAL_BUNDLE);
        dest->WriteInterfacePtr(obj);
        return NOERROR;
    }
    else if (IParcelable::Probe(obj) != NULL) {
        dest->WriteInt32(VAL_PARCELABLE);
        dest->WriteInterfacePtr(obj);
        return NOERROR;
    }
    else if (IInteger16::Probe(obj) != NULL) {
        Int16 v;
        IInteger16::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER16);
        dest->WriteInt32((Int32)v);
        return NOERROR;
    }
    else if (IInteger64::Probe(obj) != NULL) {
        Int64 v;
        IInteger64::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER64);
        dest->WriteInt64(v);
        return NOERROR;
    }
    else if (IFloat::Probe(obj) != NULL) {
        Float v;
        IFloat::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_FLOAT);
        dest->WriteFloat(v);
        return NOERROR;
    }
    else if (IDouble::Probe(obj) != NULL) {
        Double v;
        IDouble::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_DOUBLE);
        dest->WriteDouble(v);
        return NOERROR;
    }
    else if (IBoolean::Probe(obj) != NULL) {
        Boolean v;
        IBoolean::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_BOOLEAN);
        dest->WriteInt32(v ? 1 : 0);
        return NOERROR;
    }
    else if (ICharSequence::Probe(obj) != NULL) {
        // Must be after String
        String v;
        ICharSequence::Probe(obj)->ToString(&v);
        dest->WriteInt32(VAL_CHARSEQUENCE);
        // writeCharSequence((CharSequence) v);
        dest->WriteString(v);
        return NOERROR;
    }
    else if (IArrayOf::Probe(obj) != NULL){
        AutoPtr<IArrayOf> array = IArrayOf::Probe(obj);
        Int32 size = 0;
        array->GetLength(&size);
        dest->WriteInt32(VAL_ARRAYOF);
        // InterfaceID iid;
        // array->GetTypeId(&iid);
        // dest->WriteEMuid(iid);
        dest->WriteInt32(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInterface> elem;
            array->Get(i, (IInterface**)&elem);
            WriteValue(dest, elem);
        }
        return NOERROR;
    }
    // else if (v instanceof List) {
    //     writeInt(VAL_LIST);
    //     writeList((List) v);
    // }
    // else if (v instanceof SparseArray) {
    //     writeInt(VAL_SPARSEARRAY);
    //     writeSparseArray((SparseArray) v);
    // }
    // else if (v instanceof boolean[]) {
    //     writeInt(VAL_BOOLEANARRAY);
    //     writeBooleanArray((boolean[]) v);
    // }
    // else if (v instanceof byte[]) {
    //     writeInt(VAL_BYTEARRAY);
    //     writeByteArray((byte[]) v);
    // }
    // else if (v instanceof String[]) {
    //     writeInt(VAL_STRINGARRAY);
    //     writeStringArray((String[]) v);
    // }
    // else if (v instanceof CharSequence[]) {
    //     // Must be after String[] and before Object[]
    //     writeInt(VAL_CHARSEQUENCEARRAY);
    //     writeCharSequenceArray((CharSequence[]) v);
    // }
    else if (IBinder::Probe(obj) != NULL) {
        dest->WriteInt32(VAL_IBINDER);
        dest->WriteInterfacePtr(obj);
        return NOERROR;
    }
    // else if (v instanceof Parcelable[]) {
    //     writeInt(VAL_PARCELABLEARRAY);
    //     writeParcelableArray((Parcelable[]) v, 0);
    // }
    // else if (v instanceof Object[]) {
    //     writeInt(VAL_OBJECTARRAY);
    //     writeArray((Object[]) v);
    // }
    // else if (v instanceof int[]) {
    //     writeInt(VAL_INTARRAY);
    //     writeIntArray((int[]) v);
    // }
    // else if (v instanceof long[]) {
    //     writeInt(VAL_LONGARRAY);
    //     writeLongArray((long[]) v);
    // }
    else if (IByte::Probe(obj) != NULL) {
        Byte v;
        IByte::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_BYTE);
        dest->WriteInt32((Int32)v);
        return NOERROR;
    }
    // else if (v instanceof Serializable) {
    //     // Must be last
    //     writeInt(VAL_SERIALIZABLE);
    //     writeSerializable((Serializable) v);
    // }
    else {
        Logger::D(TAG, "Unable to marshal value %d", obj);

        assert(0);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode BaseBundle::WriteArrayMapInternal(
    /* [in] */ IParcel* dest,
    /* [in] */ IArrayMap* map)
{
    assert(map != NULL);

    Int32 size = 0;
    IMap::Probe(map)->GetSize(&size);

    dest->WriteInt32(size);

    AutoPtr<ISet> outset;
    IMap::Probe(map)->GetEntrySet((ISet**)&outset);
    AutoPtr<IIterator> it;
    outset->GetIterator((IIterator**)&it);
    Boolean hasNext = FALSE;
    String key;
    while ((it->HasNext(&hasNext), hasNext)) {
        AutoPtr<IInterface> outface;
        it->GetNext((IInterface**)&outface);
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(outface);
        AutoPtr<IInterface> obj;
        entry->GetKey((IInterface**)&obj);
        assert(ICharSequence::Probe(obj) != NULL);
        ICharSequence::Probe(obj)->ToString(&key);
        dest->WriteString(key);

        obj = NULL;
        entry->GetValue((IInterface**)&obj);
        WriteValue(dest, obj);
    }

    return NOERROR;
}

ECode BaseBundle::WriteToParcelInner(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    if (mParcelledData != NULL) {
        if (mParcelledData == EMPTY_PARCEL) {
            return dest->WriteInt32(0);
        }
        else {
            Int32 length;
            mParcelledData->GetElementSize(&length);
            dest->WriteInt32(length);
            dest->WriteInt32(BUNDLE_MAGIC); // 'B' 'N' 'D' 'L'
            dest->AppendFrom(mParcelledData, 0, length);
            return NOERROR;
        }
    }
    else {
        // Special case for empty bundles.
        Int32 size = 0;
        if (mMap) {
            IMap::Probe(mMap)->GetSize(&size);
        }
        if (size == 0) {
            dest->WriteInt32(0);
            return NOERROR;
        }

        Int32 lengthPos;
        dest->GetDataPosition(&lengthPos);

        dest->WriteInt32(-1); // dummy, will hold length
        dest->WriteInt32(BUNDLE_MAGIC); // 'B' 'N' 'D' 'L'

        Int32 startPos, endPos;
        dest->GetDataPosition(&startPos);
        WriteArrayMapInternal(dest, mMap);
        dest->GetDataPosition(&endPos);

        // Backpatch length
        dest->SetDataPosition(lengthPos);
        Int32 length = endPos - startPos;
        dest->WriteInt32(length);
        dest->SetDataPosition(endPos);
    }

    return NOERROR;
}

ECode BaseBundle::ReadFromParcelInner(
    /* [in] */ IParcel* parcel)
{
    Int32 length;
    parcel->ReadInt32(&length);
    if (length < 0) {
        // throw new RuntimeException("Bad length in parcel: " + length);
        return E_RUNTIME_EXCEPTION;
    }
    return ReadFromParcelInner(parcel, length);
}

ECode BaseBundle::ReadFromParcelInner(
    /* [in] */ IParcel* source,
    /* [in] */ Int32 length)
{
    if (length == 0) {
        // Empty Bundle or end of data.
        mParcelledData = EMPTY_PARCEL;
        return NOERROR;
    }

    android::Parcel* parcel;
    source->GetElementPayload((Handle32*)&parcel);

    Int32 magic;
    source->ReadInt32(&magic);
    if (magic != BUNDLE_MAGIC) {
        //noinspection ThrowableInstanceNeverThrown
        // String st = Log.getStackTraceString(new RuntimeException());
        // Log.e("Bundle", "readBundle: bad magic number");
        // Log.e("Bundle", "readBundle: trace = " + st);
        Logger::E(TAG, "Bad magic number for Bundle: 0x%08x", magic);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Advance within this Parcel
    Int32 offset;
    source->GetDataPosition(&offset);
    source->SetDataPosition(offset + length);

    CParcel::New((IParcel**)&mParcelledData);
    mParcelledData->SetDataPosition(0);
    mParcelledData->AppendFrom(source, offset, length);
    mParcelledData->SetDataPosition(0);

    return NOERROR;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos

