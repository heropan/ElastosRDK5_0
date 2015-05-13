#include <ext/frameworkdef.h>
#include "os/CBundle.h"
#include <elastos/Logger.h>
#include <elastos/StringBuffer.h>
#include <elastos/StringBuilder.h>
#include <binder/Parcel.h>

using android::Parcel;
using Elastos::Core::IBoolean;
using Elastos::Core::IByte;
using Elastos::Core::IChar32;
using Elastos::Core::IInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::IFloat;
using Elastos::Core::IDouble;
using Elastos::Core::IArrayOf;
using Elastos::Core::CBoolean;
using Elastos::Core::CByte;
using Elastos::Core::CChar32;
using Elastos::Core::CInteger16;
using Elastos::Core::CInteger32;
using Elastos::Core::CInteger64;
using Elastos::Core::CFloat;
using Elastos::Core::CDouble;
using Elastos::Core::CArrayOf;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IInteger32;
using Elastos::Core::EIID_IChar32;
using Elastos::Core::EIID_IBoolean;
using Elastos::Core::EIID_IByte;
using Elastos::Core::EIID_IInteger64;
using Elastos::Core::EIID_IFloat;
using Elastos::Core::EIID_IDouble;
using Elastos::Core::EIID_IInteger16;
using Elastos::Droid::Content::IIntent;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::CObjectStringMap;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Os {

static AutoPtr<IBundle> InitEMPTY()
{
    AutoPtr<CBundle> empty;
    CBundle::NewByFriend((CBundle**)&empty);
    return empty;
}

AutoPtr<IBundle> CBundle::EMPTY = InitEMPTY();
const String CBundle::TAG("CBundle");
const Int32 CBundle::VAL_NULL;
const Int32 CBundle::VAL_NOT_NULL;
const Int32 CBundle::VAL_STRING;
const Int32 CBundle::VAL_INTEGER32;
const Int32 CBundle::VAL_MAP;
const Int32 CBundle::VAL_BUNDLE;
const Int32 CBundle::VAL_PARCELABLE;
const Int32 CBundle::VAL_INTEGER16;
const Int32 CBundle::VAL_INTEGER64;
const Int32 CBundle::VAL_FLOAT;
const Int32 CBundle::VAL_DOUBLE;
const Int32 CBundle::VAL_BOOLEAN;
const Int32 CBundle::VAL_CHARSEQUENCE;
const Int32 CBundle::VAL_LIST;
const Int32 CBundle::VAL_SPARSEARRAY;
const Int32 CBundle::VAL_BYTEARRAY;
const Int32 CBundle::VAL_STRINGARRAY;
const Int32 CBundle::VAL_IBINDER;
const Int32 CBundle::VAL_PARCELABLEARRAY;
const Int32 CBundle::VAL_OBJECTARRAY;
const Int32 CBundle::VAL_INTARRAY;
const Int32 CBundle::VAL_LONGARRAY;
const Int32 CBundle::VAL_BYTE;
const Int32 CBundle::VAL_SERIALIZABLE;
const Int32 CBundle::VAL_SPARSEBOOLEANARRAY;
const Int32 CBundle::VAL_BOOLEANARRAY;
const Int32 CBundle::VAL_CHARSEQUENCEARRAY;
const Int32 CBundle::VAL_ARRAYOF;
const Int32 CBundle::VAL_OBJECTSTRINGMAP;
const Int32 CBundle::VAL_OBJECTINT32MAP;

CBundle::CBundle()
    : mHasFds(FALSE)
    , mFdsKnown(TRUE)
    , mAllowFds(TRUE)
{
}

CBundle::~CBundle()
{
    // TODO: for debug
    mParcelledData = NULL;
    mClassLoader = NULL;

    mMap = NULL;
}

ECode CBundle::constructor()
{
    mMap = new HashMap<String, AutoPtr<IInterface> >(5);
    //mClassLoader = getClass().getClassLoader();
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IClassLoader* loader)
{
    mMap = new HashMap<String, AutoPtr<IInterface> >(5);
    mClassLoader = loader;
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ Int32 capacity)
{
    if (capacity <= 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMap = new HashMap<String, AutoPtr<IInterface> >(capacity);
    // mClassLoader = getClass().getClassLoader();
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);

    CBundle* b = (CBundle*)bundle;
    if (b->mParcelledData != 0) {
        CParcel::New((IParcel**)&mParcelledData);
        Int32 length;
        b->mParcelledData->GetElementSize(&length);
        mParcelledData->AppendFrom(b->mParcelledData, 0, length);
        mParcelledData->SetDataPosition(0);
    }

    if (b->mMap != NULL) {
        mMap = new HashMap<String, AutoPtr<IInterface> >(b->mMap->Begin(), b->mMap->End());
    }
    else {
        mMap = NULL;
    }

    mHasFds = b->mHasFds;
    mFdsKnown = b->mFdsKnown;
    mClassLoader = b->mClassLoader;
    return NOERROR;
}

ECode CBundle::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CBundle");

    if (mParcelledData != NULL) {
        StringBuilder sb("Bundle[mParcelledData.dataSize=");
        Int32 length;
        mParcelledData->GetElementSize(&length);
        sb += length;
        sb += "]";
        *str = sb.ToString();
        return NOERROR;
    }

    StringBuilder sb("Bundle[");
    if (mMap != NULL) sb += mMap->GetSize();
    sb += "]";
    *str = sb.ToString();
    return NOERROR;
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 size;
    source->ReadInt32(&size);
    if (size < 0) {
        // throw new RuntimeException("Bad length in parcel: " + length);
        return E_RUNTIME_EXCEPTION;
    }
    return ReadFromParcelInner(source, size);
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel* source,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    *bundle = NULL;

    Int32 size;
    source->ReadInt32(&size);
    if (size > 0) {
        FAIL_RETURN(CBundle::New(size, bundle));
        return ((CBundle*)*bundle)->ReadFromParcelInner(source, size);
    }
    return NOERROR;
}

ECode CBundle::ReadFromParcelInner(
    /* [in] */ IParcel* source,
    /* [in] */ Int32 length)
{
    android::Parcel* parcel;
    source->GetElementPayload((Handle32*)&parcel);

    Int32 magic;
    source->ReadInt32(&magic);
    if (magic != 0x4C444E42) {
        //noinspection ThrowableInstanceNeverThrown
        // String st = Log.getStackTraceString(new RuntimeException());
        // Log.e("Bundle", "readBundle: bad magic number");
        // Log.e("Bundle", "readBundle: trace = " + st);
    }

    // Advance within this Parcel
    Int32 offset;
    source->GetDataPosition(&offset);
    source->SetDataPosition(offset + length);

    CParcel::New((IParcel**)&mParcelledData);
    mParcelledData->SetDataPosition(0);
    mParcelledData->AppendFrom(source, offset, length);
    mParcelledData->SetDataPosition(0);

    mParcelledData->HasFileDescriptors(&mHasFds);
    mFdsKnown = TRUE;

    return NOERROR;
}

ECode CBundle::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (mParcelledData != 0) {
        Int32 length;
        mParcelledData->GetElementSize(&length);
        dest->WriteInt32(length);
        dest->WriteInt32(0x4C444E42); // 'B' 'N' 'D' 'L'
        dest->AppendFrom(mParcelledData, 0, length);
    }
    else {
        dest->WriteInt32(-1); // dummy, will hold length
        dest->WriteInt32(0x4C444E42); // 'B' 'N' 'D' 'L'

        Int32 oldPos, newPos;
        dest->GetDataPosition(&oldPos);
        WriteMapInternal(dest);
        dest->GetDataPosition(&newPos);

        // Backpatch length
        dest->SetDataPosition(oldPos - 8);
        Int32 length = newPos - oldPos;
        dest->WriteInt32(length);
        dest->SetDataPosition(newPos);
    }
    return NOERROR;
}

ECode CBundle::WriteToParcel(
    /* [in] */ IBundle *bundle,
    /* [in] */ IParcel *dest)
{
    if (bundle != NULL) {
        return IParcelable::Probe(bundle)->WriteToParcel(dest);
    }
    else {
        dest->WriteInt32(-1);
    }
    return NOERROR;
}

ECode CBundle::ReadMapInternal(
    /* [in] */ IParcel* source,
    /* [in] */ Int32 size,
    /* [in] */ IClassLoader* classLoader)
{
    while (size > 0) {
        String key;
        source->ReadString(&key);
        AutoPtr<IInterface> value = ReadValue(source);
        (*mMap)[key] = value;
        size--;
    }
    return NOERROR;
}

ECode CBundle::WriteMapInternal(
    /* [in] */ IParcel* dest)
{
    if (mMap == NULL) {
        dest->WriteInt32(-1);
        return NOERROR;
    }
    dest->WriteInt32(mMap->GetSize());
    HashMap<String, AutoPtr<IInterface> >::Iterator it;
    for (it = mMap->Begin(); it != mMap->End(); ++it) {
        dest->WriteString(it->mFirst);
        WriteValue(dest, it->mSecond);
    }
    return NOERROR;
}

ECode CBundle::WriteValue(
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
    else if (IObjectStringMap::Probe(obj) != NULL){
        AutoPtr<IObjectStringMap> map = IObjectStringMap::Probe(obj);
        dest->WriteInt32(VAL_OBJECTSTRINGMAP);

        AutoPtr<ArrayOf<String> > keys;
        map->GetKeys((ArrayOf<String>**)&keys);
        if (keys == NULL) {
            return NOERROR;
        }

        Int32 size = keys->GetLength();
        dest->WriteInt32(size);
        for (Int32 i = 0; i < size; i++) {
            dest->WriteString((*keys)[i]);

            AutoPtr<IInterface> value;
            map->Get((*keys)[i], (IInterface**)&value);
            WriteValue(dest, value);
        }
        return NOERROR;
    }
    else if (IObjectInt32Map::Probe(obj) != NULL){
        AutoPtr<IObjectInt32Map> map = IObjectInt32Map::Probe(obj);
        dest->WriteInt32(VAL_OBJECTINT32MAP);

        AutoPtr<ArrayOf<Int32> > keys;
        map->GetKeys((ArrayOf<Int32>**)&keys);
        if (keys == NULL) {
            dest->WriteInt32(0);
            return NOERROR;
        }

        Int32 size = keys->GetLength();
        dest->WriteInt32(size);
        for (Int32 i = 0; i < size; i++) {
            dest->WriteInt32((*keys)[i]);

            AutoPtr<IInterface> value;
            map->Get((*keys)[i], (IInterface**)&value);
            WriteValue(dest, value);
        }
        return NOERROR;
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
        Logger::D("CBundle", "Unable to marshal value %d", obj);

        assert(0);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<IInterface> CBundle::ReadValue(
    /* [in] */ IParcel* source)
{
    Int32 type;
    source->ReadInt32(&type);

    switch (type) {
    case VAL_NULL:
        return NULL;
    // case VAL_STRING:
    //     return readString();
    case VAL_INTEGER32: {
        Int32 v;
        source->ReadInt32(&v);
        AutoPtr<IInteger32> obj;
        CInteger32::New(v, (IInteger32**)&obj);
        return obj;
    }
    // case VAL_MAP:
    //     return readHashMap(loader);

    case VAL_PARCELABLE:{
        AutoPtr<IParcelable> obj;
        source->ReadInterfacePtr((Handle32*)&obj);
        if(obj == NULL){
            Logger::E("CBundle", "Read IParcelable got null");
        }
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
    case VAL_CHARSEQUENCE: {
        String v;
        source->ReadString(&v);
        AutoPtr<ICharSequence> obj;
        CStringWrapper::New(v, (ICharSequence**)&obj);
        return obj;
    }
    // case VAL_LIST:
    //     return readArrayList(loader);
    // case VAL_BOOLEANARRAY:
    //     return createBooleanArray();
    // case VAL_BYTEARRAY:
    //     return createByteArray();
    // case VAL_STRINGARRAY:
    //     return readStringArray();
    // case VAL_CHARSEQUENCEARRAY:
    //     return readCharSequenceArray();
    case VAL_IBINDER: {
        AutoPtr<IBinder> obj;
        source->ReadInterfacePtr((Handle32*)&obj);
        if(obj == NULL){
            Logger::E("CBundle", "Read IBinder got null");
        }
        return obj;
    }
    // case VAL_OBJECTARRAY:
    //     return readArray(loader);
    // case VAL_INTARRAY:
    //     return createIntArray();
    // case VAL_LONGARRAY:
    //     return createLongArray();
    case VAL_BYTE: {
        Int32 v;
        source->ReadInt32(&v);
        AutoPtr<IByte> obj;
        CByte::New((Byte)v, (IByte**)&obj);
        return obj;
    }
    // case VAL_SERIALIZABLE:
    //     return readSerializable();
    // case VAL_PARCELABLEARRAY:
    //     return readParcelableArray(loader);
    case VAL_OBJECTSTRINGMAP: {
        AutoPtr<IObjectStringMap> map;
        Int32 size = 0;
        source->ReadInt32(&size);
        CObjectStringMap::New((IObjectStringMap**)&map);
        for (Int32 i = 0; i < size; ++i) {
            String key;
            source->ReadString(&key);

            AutoPtr<IInterface> value = ReadValue(source);
            map->Put(key, value);
        }
        return map;
    }
    case VAL_OBJECTINT32MAP: {
        AutoPtr<IObjectInt32Map> map;
        Int32 size = 0;
        source->ReadInt32(&size);
        CObjectInt32Map::New((IObjectInt32Map**)&map);
        for (Int32 i = 0; i < size; ++i) {
            Int32 key = 0;
            source->ReadInt32(&key);

            AutoPtr<IInterface> value = ReadValue(source);
            map->Put(key, value);
        }
        return map;
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
    // case VAL_SPARSEARRAY:
    //     return readSparseArray(loader);
    // case VAL_SPARSEBOOLEANARRAY:
    //     return readSparseBooleanArray();
    case VAL_BUNDLE:{
        AutoPtr<IParcelable> obj;
        source->ReadInterfacePtr((Handle32*)&obj);
        if(obj == NULL){
            Logger::E("CBundle", "Read IBundle got null");
        }
        return obj;
    }
    default:
        Logger::D("CBundle", "- Unmarshalling unknown type code %d", type);
        // int off = dataPosition() - 4;
        // throw new RuntimeException(
        //     "Parcel " + this + ": Unmarshalling unknown type code " + type + " at offset " + off);
        assert(0);
    }
    return NULL;
}

AutoPtr<IBundle> CBundle::ForPair(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    // TODO: optimize this case.
    AutoPtr<IBundle> b;
    CBundle::New(1, (IBundle**)&b);
    b->PutString(key, value);
    return b;
}

ECode CBundle::GetPairValue(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    Unparcel();
    Int32 size = mMap->GetSize();
    if (size > 1) {
        Logger::W(LOG_TAG, String("getPairValue() used on Bundle with multiple pairs."));
    }
    if (size == 0) {
        *result = NULL;
       return NOERROR;
    }

    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Begin();
    if (it != mMap->End()) {
        ICharSequence* obj = ICharSequence::Probe(it->mSecond);
        if (obj == NULL) {
            TypeWarning(String("GetPairValue()"), obj, String("String"));
            *result = NULL;
            return NOERROR;
        }
        return obj->ToString(result);
    }

    *result = NULL;
   return NOERROR;
}

ECode CBundle::SetClassLoader(
    /* [in] */ IClassLoader* loader)
{
    mClassLoader = loader;
    return NOERROR;
}

ECode CBundle::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);
    *loader = mClassLoader;
    INTERFACE_ADDREF(*loader);
    return NOERROR;
}

/** @hide */
ECode CBundle::SetAllowFds(
    /* [in] */ Boolean allowFds)
{
    mAllowFds = allowFds;
    return NOERROR;
}

ECode CBundle::SetAllowFdsEx(
    /* [in] */ Boolean allowFds,
    /* [out] */ Boolean* prev)
{
    if (prev) {
        *prev = mAllowFds;
    }

    mAllowFds = allowFds;
    return NOERROR;
}

ECode CBundle::Clone(
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result);
    return CBundle::New((IBundle*)this, result);
}

ECode CBundle::Unparcel()
{
    Mutex::Autolock lock(mThisLock);

    if (mParcelledData == NULL) {
        return NOERROR;
    }

    Int32 N;
    mParcelledData->ReadInt32(&N);
    if (N < 0) {
        return NOERROR;
    }
    if (mMap == NULL) {
        mMap = new HashMap<String, AutoPtr<IInterface> >(7);
    }
    FAIL_RETURN(ReadMapInternal(mParcelledData, N, mClassLoader));
    mParcelledData = NULL;
    return NOERROR;
}

ECode CBundle::IsParcelled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mParcelledData != 0;
    return NOERROR;
}

ECode CBundle::GetSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    *result = mMap->GetSize();
    return NOERROR;
}

ECode CBundle::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    *result = mMap->IsEmpty();
    return NOERROR;
}

ECode CBundle::Clear()
{
    Unparcel();
    if (mMap) mMap->Clear();
    mHasFds = FALSE;
    mFdsKnown = TRUE;
    return NOERROR;
}

ECode CBundle::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    *result = (mMap->Find(key) != mMap->End());
    return NOERROR;
}

ECode CBundle::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it != mMap->End()) {
        *value = it->mSecond;
        INTERFACE_ADDREF(*value)
    }
    return NOERROR;
}

ECode CBundle::Remove(
    /* [in] */ const String& key)
{
    Unparcel();
    mMap->Erase(key);
    return NOERROR;
}

ECode CBundle::PutAll(
    /* [in] */ IBundle* map)
{
    VALIDATE_NOT_NULL(map);
    CBundle* mapObj = (CBundle*)map;

    Unparcel();
    mapObj->Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mapObj->mMap->Begin();
    while (it != mapObj->mMap->End()) {
        (*mMap)[it->mFirst] = it->mSecond;
        ++it;
    }

    // fd state is now known if and only if both bundles already knew
    mHasFds |= mapObj->mHasFds;
    mFdsKnown = mFdsKnown && mapObj->mFdsKnown;
    return NOERROR;
}


ECode CBundle::KeySet(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    FAIL_RETURN(CObjectContainer::New(result));
    HashMap<String, AutoPtr<IInterface> >::Iterator it;
    for (it = mMap->Begin(); it != mMap->End(); ++it) {
        AutoPtr<ICharSequence> str;
        CStringWrapper::New(it->mFirst, (ICharSequence**)&str);
        (*result)->Add(str.Get());
    }
    return NOERROR;
}


ECode CBundle::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // if (!mFdsKnown) {
    //         boolean fdFound = false;    // keep going until we find one or run out of data

    //         if (mParcelledData != null) {
    //             if (mParcelledData.hasFileDescriptors()) {
    //                 fdFound = true;
    //             }
    //         } else {
    //             // It's been unparcelled, so we need to walk the map
    //             Iterator<Map.Entry<String, Object>> iter = mMap->entrySet().iterator();
    //             while (!fdFound && iter.hasNext()) {
    //                 Object obj = iter.next().getValue();
    //                 if (obj instanceof Parcelable) {
    //                     if ((((Parcelable)obj).describeContents()
    //                             & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
    //                         fdFound = true;
    //                         break;
    //                     }
    //                 } else if (obj instanceof Parcelable[]) {
    //                     Parcelable[] array = (Parcelable[]) obj;
    //                     for (int n = array.length - 1; n >= 0; n--) {
    //                         if ((array[n].describeContents()
    //                                 & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
    //                             fdFound = true;
    //                             break;
    //                         }
    //                     }
    //                 } else if (obj instanceof SparseArray) {
    //                     SparseArray<? extends Parcelable> array =
    //                             (SparseArray<? extends Parcelable>) obj;
    //                     for (int n = array.size() - 1; n >= 0; n--) {
    //                         if ((array.get(n).describeContents()
    //                                 & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
    //                             fdFound = true;
    //                             break;
    //                         }
    //                     }
    //                 } else if (obj instanceof ArrayList) {
    //                     ArrayList array = (ArrayList) obj;
    //                     // an ArrayList here might contain either Strings or
    //                     // Parcelables; only look inside for Parcelables
    //                     if ((array.size() > 0)
    //                             && (array.get(0) instanceof Parcelable)) {
    //                         for (int n = array.size() - 1; n >= 0; n--) {
    //                             Parcelable p = (Parcelable) array.get(n);
    //                             if (p != null && ((p.describeContents()
    //                                     & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0)) {
    //                                 fdFound = true;
    //                                 break;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }

    //         mHasFds = fdFound;
    //         mFdsKnown = true;
    //     }
    //     return mHasFds;
    *result = FALSE;
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IBoolean> obj;
    ECode ec = CBoolean::New(value, (IBoolean**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutByte(
    /* [in] */ const String& key,
    /* [in] */ Byte value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IByte> obj;
    ECode ec = CByte::New(value, (IByte**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutChar(
    /* [in] */ const String& key,
    /* [in] */ Char32 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IChar32> obj;
    ECode ec = CChar32::New(value, (IChar32**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutInt16(
    /* [in] */ const String& key,
    /* [in] */ Int16 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IInteger16> obj;
    ECode ec = CInteger16::New(value, (IInteger16**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IInteger32> obj;
    ECode ec = CInteger32::New(value, (IInteger32**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IInteger64> obj;
    ECode ec = CInteger64::New(value, (IInteger64**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IFloat> obj;
    ECode ec = CFloat::New(value, (IFloat**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ Double value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IDouble> obj;
    ECode ec = CDouble::New(value, (IDouble**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutString(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<ICharSequence> obj;
    ECode ec = CStringWrapper::New(value, (ICharSequence**)&obj);
    if (SUCCEEDED(ec)) {
        (*mMap)[key] = obj.Get();
    }
    return ec;
}

ECode CBundle::PutCharSequence(
    /* [in] */ const String& key,
    /* [in] */ ICharSequence* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutParcelable(
    /* [in] */ const String& key,
    /* [in] */ IParcelable* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutObjectStringMap(
    /* [in] */ const String& key,
    /* [in] */ IObjectStringMap* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<IParcelable*>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        ECode ec = CArrayOf::New(EIID_IInterface, value->GetLength(), (IArrayOf**)&obj);
        if (SUCCEEDED(ec)) {
            for (Int32 i = 0; i < value->GetLength(); ++i) {
                obj->Put(i, (*value)[i]);
            }
        }
    }
    (*mMap)[key] = obj;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutSparseParcelableArray(
    /* [in] */ const String& key,
    /* [in] */ IObjectInt32Map* value)
{
    VALIDATE_STRING_NOT_NULL(key);
    Unparcel();
    (*mMap)[key] = value;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableMap(
    /* [in] */ const String& key,
    /* [in] */ IInterface* parcelableMap)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = parcelableMap;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutIntegerArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutStringArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutCharSequenceArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutSerializable(
    /* [in] */ const String& key,
    /* [in] */ ISerializable* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutBooleanArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Boolean>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IBoolean, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IBoolean> bv;
            CBoolean::New((*value)[i], (IBoolean**)&bv);
            obj->Put(i, bv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutByteArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IByte, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IByte> bv;
            CByte::New((*value)[i], (IByte**)&bv);
            obj->Put(i, bv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt16Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int16>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IInteger16, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IInteger16> iv;
            CInteger16::New((*value)[i], (IInteger16**)&iv);
            obj->Put(i, iv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutCharArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Char32>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IChar32, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IChar32> cv;
            CChar32::New((*value)[i], (IChar32**)&cv);
            obj->Put(i, cv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt32Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int32>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IInteger32, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IInteger32> iv;
            CInteger32::New((*value)[i], (IInteger32**)&iv);
            obj->Put(i, iv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt64Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int64>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IInteger64, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IInteger64> iv;
            CInteger64::New((*value)[i], (IInteger64**)&iv);
            obj->Put(i, iv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutFloatArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Float>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IFloat, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IFloat> fv;
            CFloat::New((*value)[i], (IFloat**)&fv);
            obj->Put(i, fv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutDoubleArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Double>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IDouble, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IDouble> dv;
            CDouble::New((*value)[i], (IDouble**)&dv);
            obj->Put(i, dv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutStringArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<String>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IInterface, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<ICharSequence> sv;
            CStringWrapper::New((*value)[i], (ICharSequence**)&sv);
            obj->Put(i, sv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutCharSequenceArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<ICharSequence*>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArrayOf> obj;
    if (value != NULL) {
        CArrayOf::New(EIID_IInterface, value->GetLength(), (IArrayOf**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            obj->Put(i, (*value)[i]);
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutBundle(
    /* [in] */ const String& key,
    /* [in] */ IBundle* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutIBinder(
    /* [in] */ const String& key,
    /* [in] */ IBinder* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::GetBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetBooleanEx(key, FALSE, value);
}

ECode CBundle::GetBooleanEx(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IBoolean* obj = IBoolean::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Boolean"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Boolean", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetByte(
    /* [in] */ const String& key,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetByteEx(key, 0, value);
}

ECode CBundle::GetByteEx(
    /* [in] */ const String& key,
    /* [in] */ Byte defaultValue,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IByte* obj = IByte::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Byte"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Byte", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetChar(
    /* [in] */ const String& key,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetCharEx(key, 0, value);
}

ECode CBundle::GetCharEx(
    /* [in] */ const String& key,
    /* [in] */ Char32 defaultValue,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IChar32* obj = IChar32::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Char32"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Character", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt16(
    /* [in] */ const String& key,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetInt16Ex(key, 0, value);
}

ECode CBundle::GetInt16Ex(
    /* [in] */ const String& key,
    /* [in] */ Int16 defaultValue,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IInteger16* obj = IInteger16::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Int16"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Short", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetInt32Ex(key, 0, value);
}

ECode CBundle::GetInt32Ex(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IInteger32* obj = IInteger32::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Int32"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Integer", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetInt64Ex(key, 0ll, value);
}

ECode CBundle::GetInt64Ex(
    /* [in] */ const String& key,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IInteger64* obj = IInteger64::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Int64"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Long", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetFloat(
    /* [in] */ const String& key,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetFloatEx(key, 0.0f, value);
}

ECode CBundle::GetFloatEx(
    /* [in] */ const String& key,
    /* [in] */ Float defaultValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IFloat* obj = IFloat::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Float"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Float", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetDouble(
    /* [in] */ const String& key,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetDoubleEx(key, 0.0, value);
}

ECode CBundle::GetDoubleEx(
    /* [in] */ const String& key,
    /* [in] */ Double defaultValue,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IDouble* obj = IDouble::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("Double"));
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Double", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetString(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    ICharSequence* obj = ICharSequence::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("String"));
        return NOERROR;
    }
    return obj->ToString(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "String", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetStringEx(
    /* [in] */ const String& key,
    /* [in] */ const String& defaultValue,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    ICharSequence* obj = ICharSequence::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("String"));
        return NOERROR;
    }
    return obj->ToString(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "String", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharSequence(
    /* [in] */ const String& key,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = ICharSequence::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("ICharSequence"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "CharSequence", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetBundle(
    /* [in] */ const String& key,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IBundle::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IBundle"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Bundle", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetParcelable(
    /* [in] */ const String& key,
    /* [out] */ IParcelable** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IParcelable::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IParcelable"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Parcelable", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetObjectStringMap(
    /* [in] */ const String& key,
    /* [out] */ IObjectStringMap** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IObjectStringMap::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IObjectStringMap"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ObjectStringMap", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetParcelableArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<IParcelable*>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<IParcelable*> > array = ArrayOf<IParcelable*>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (elem != NULL) {
            if (IParcelable::Probe(elem) == NULL) {
                TypeWarning(key, it->mSecond, String("IArrayOf-IParcelable"));
                return NOERROR;
            }
            array->Set(i, IParcelable::Probe(elem));
        }
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Parcelable[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetParcelableArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IObjectContainer"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetSparseParcelableArray(
    /* [in] */ const String& key,
    /* [out] */ IObjectInt32Map** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);
    *value = NULL;

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }

    *value = IObjectInt32Map::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IObjectInt32Map"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value);
    return NOERROR;
}

ECode CBundle::GetParcelableMap(
    /* [in] */ const String& key,
    /* [out] */ IInterface** parcelableMap)
{
    VALIDATE_NOT_NULL(parcelableMap);
    *parcelableMap = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }

    *parcelableMap = it->mSecond;
    INTERFACE_ADDREF(*parcelableMap);
    return NOERROR;
    // try {
    //     return (SparseArray<T>) value;
    // } catch (ClassCastException e) {
    //     typeWarning(key, value, "SparseArray", e);
    //     return null;
    // }
}

ECode CBundle::GetSerializable(
    /* [in] */ const String& key,
    /* [out] */ ISerializable** result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetIntegerArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IObjectContainer"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList<Integer>", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetStringArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IObjectContainer"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList<String>", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharSequenceArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }

    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IObjectContainer"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList<CharSequence>", defaultValue, e);
    //     return defaultValue;
    // }
}


ECode CBundle::GetBooleanArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Boolean>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Boolean> > array = ArrayOf<Boolean>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IBoolean::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Boolean"));
            return NOERROR;
        }
        Boolean bv;
        IBoolean::Probe(elem)->GetValue(&bv);
        (*array)[i] = bv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "boolean[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetByteArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IByte::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Byte"));
            return NOERROR;
        }
        Byte bv;
        IByte::Probe(elem)->GetValue(&bv);
        (*array)[i] = bv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "byte[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt16Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int16>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Int16> > array = ArrayOf<Int16>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IInteger16::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Int16"));
            return NOERROR;
        }
        Int16 iv;
        IInteger16::Probe(elem)->GetValue(&iv);
        (*array)[i] = iv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "short[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Char32>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Char32> > array = ArrayOf<Char32>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IChar32::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Char32"));
            return NOERROR;
        }
        Char32 cv;
        IChar32::Probe(elem)->GetValue(&cv);
        (*array)[i] = cv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "char[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt32Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IInteger32::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Int32"));
            return NOERROR;
        }
        Int32 iv;
        IInteger32::Probe(elem)->GetValue(&iv);
        (*array)[i] = iv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "int[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt64Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int64>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Int64> > array = ArrayOf<Int64>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IInteger64::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Int64"));
            return NOERROR;
        }
        Int64 iv;
        IInteger64::Probe(elem)->GetValue(&iv);
        (*array)[i] = iv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "long[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetFloatArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Float>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IFloat::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Float"));
            return NOERROR;
        }
        Float fv;
        IFloat::Probe(elem)->GetValue(&fv);
        (*array)[i] = fv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "float[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetDoubleArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Double>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<Double> > array = ArrayOf<Double>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IDouble::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-Double"));
            return NOERROR;
        }
        Double dv;
        IDouble::Probe(elem)->GetValue(&dv);
        (*array)[i] = dv;
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "double[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetStringArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (ICharSequence::Probe(elem) == NULL) {
            TypeWarning(key, it->mSecond, String("IArrayOf-String"));
            return NOERROR;
        }
        String sv;
        ICharSequence::Probe(elem)->ToString(&sv);
        array->Set(i, sv);
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "String[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharSequenceArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<ICharSequence*>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    IArrayOf* obj = IArrayOf::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, String("IArrayOf"));
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    AutoPtr<ArrayOf<ICharSequence*> > array = ArrayOf<ICharSequence*>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (elem != NULL) {
            if (ICharSequence::Probe(elem) == NULL) {
                TypeWarning(key, it->mSecond, String("IArrayOf-ICharSequence"));
                return NOERROR;
            }
            array->Set(i, ICharSequence::Probe(elem));
        }
    }
    *value = array;
    INTERFACE_ADDREF(*value);
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "CharSequence[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetIBinder(
    /* [in] */ const String& key,
    /* [out] */ IBinder** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        return NOERROR;
    }
    // try {
    *value = IBinder::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, String("IBinder"));
        return NOERROR;
    }
    INTERFACE_ADDREF(*value)
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "IBinder", defaultValue, e);
    //     return defaultValue;
    // }
}

// Log a message if the value was non-null but not of the expected type
void CBundle::TypeWarning(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value,
    /* [in] */ const String& className)
{
    StringBuilder sb;
    sb += "Key ";
    sb += key;
    sb += " expected ";
    sb += className;
    sb += " but value was a ";
    // sb.append(value.getClass().getName());
    sb += ".";
    Logger::W(LOG_TAG, sb.ToString().string());
    //Logger::W(LOG_TAG, "Attempt to cast generated internal exception:", e);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
