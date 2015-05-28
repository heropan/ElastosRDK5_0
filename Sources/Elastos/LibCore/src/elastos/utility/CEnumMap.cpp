
#include "CEnumMap.h"
#include "CArrays.h"
#include "Enum.h"
#include "elastos/ObjectUtils.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::ObjectUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::Enum;
using Elastos::Core::EIID_IEnum;

namespace Elastos {
namespace Utility {


//==========================================================
//       CEnumMap
//==========================================================
PInterface CEnumMap::Probe(
    /* [in] */ REIID riid)
{
    return _CEnumMap::Probe(riid);
}

CEnumMap::CEnumMap()
    : mMappingsCount(0)
    , mEnumSize(0)
{
    CArrays::AcquireSingleton((IArrays**)&mResarr);
}

ECode CEnumMap::constructor()
{
    return NOERROR;
}

ECode CEnumMap::constructor(
    /* [in] */ InterfaceID mKeyType)
{
    return Initialization(mKeyType);
}

ECode CEnumMap::constructor(
    /* [in] */ IEnumMap* enummap)
{
    return Initialization(enummap);
}

ECode CEnumMap::constructor(
    /* [in] */ IMap* map)
{
    if (IEnumMap::Probe(map)) {
        Initialization(IEnumMap::Probe(map));
    }
    else {
        Int32 lensize = 0;
        if ((map->GetSize(&lensize), lensize) == 0) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        AutoPtr<ISet> outset;
        map->KeySet((ISet**)&outset);
        AutoPtr<IIterator> iter;
        outset->GetIterator((IIterator**)&iter);
        AutoPtr<IInterface> enumKey;
        iter->Next((IInterface**)&enumKey);
        if (IEnum::Probe(enumKey)) {
            Initialization(EIID_IEnum);
        }
        else {
            Initialization(EIID_IInterface);
        }
        PutAllImpl(map);
    }
    return NOERROR;
}

ECode CEnumMap::Clear()
{
    mResarr->FillObject(mValues, NULL);
    mResarr->FillBoolean(mHasMapping, FALSE);
    mMappingsCount = 0;
    return NOERROR;
}

ECode CEnumMap::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    // try {
    AutoPtr<CEnumMap> enumMap;
    FAIL_RETURN(CEnumMap::NewByFriend((CEnumMap**)&enumMap));
    enumMap->Initialization((IEnumMap*)this->Probe(EIID_IEnumMap));
    *object = (IEnumMap*) enumMap->Probe(EIID_IEnumMap);
    INTERFACE_ADDREF(*object)
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }
    return NOERROR;
}

ECode CEnumMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (IsValidKeyType(key)) {
        Int32 keyOrdinal = 0;
        IEnum::Probe(key)->Ordinal(&keyOrdinal);
        *result = (*mHasMapping)[keyOrdinal];
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CEnumMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (value == NULL) {
        for (Int32 i = 0; i < mEnumSize; i++) {
            if ((*mHasMapping)[i] && (*mValues)[i] == NULL) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = 0; i < mEnumSize; i++) {
            if ((*mHasMapping)[i] && ObjectUtils::Equals(value, (*mValues)[i])) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode CEnumMap::EntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)

    if (mEntrySet == NULL) {
        mEntrySet = new EnumMapEntrySet(this);
    }
    *entries = (ISet*)mEntrySet->Probe(EIID_ISet);
    INTERFACE_ADDREF(*entries)
    return NOERROR;
}

ECode CEnumMap::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (this->Probe(EIID_IInterface) == other) {
        *value = TRUE;
        return NOERROR;
    }
    if (!IEnumMap::Probe(other)) {
        return AbstractMap::Equals(other, value);
    }
    AutoPtr<CEnumMap> enumMap = (CEnumMap*)IEnumMap::Probe(other);
    Int32 len1 = 0;
    Int32 len2 = 0;
    if (mKeyType != enumMap->mKeyType
        || (GetSize(&len1), len1) != (enumMap->GetSize(&len2), len2)) {
        *value = FALSE;
        return NOERROR;
    }

    Boolean isflag1 = FALSE;
    Boolean isflag2 = FALSE;
    mResarr->EqualsBoolean(mHasMapping, enumMap->mHasMapping, &isflag1);
    mResarr->EqualsObject(mValues, enumMap->mValues, &isflag2);
    *value = isflag1 && isflag2;
    return NOERROR;
}

ECode CEnumMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    if (!IsValidKeyType(key)) {
        *value = NULL;
        return NOERROR;
    }
    Int32 keyOrdinal = 0;
    IEnum::Probe(key)->Ordinal(&keyOrdinal);
    *value = (*mValues)[keyOrdinal];
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode CEnumMap::KeySet(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    if (mKeySet == NULL) {
        mKeySet = new EnumMapKeySet(this);
    }
    *keySet = mKeySet;
    INTERFACE_ADDREF(*keySet)
    return NOERROR;
}

ECode CEnumMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return PutImpl(key, value, oldValue);
}

ECode CEnumMap::PutAll(
    /* [in] */ IMap* map)
{
    return PutAllImpl(map);
}

ECode CEnumMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    if (!IsValidKeyType(key)) {
        *value = NULL;
        return NOERROR;
    }
    Int32 keyOrdinal = 0;
    IEnum::Probe(key)->Ordinal(&keyOrdinal);
    if ((*mHasMapping)[keyOrdinal]) {
        (*mHasMapping)[keyOrdinal] = FALSE;
        mMappingsCount--;
    }
    AutoPtr<IInterface> oldValue = (*mValues)[keyOrdinal];
    mValues->Set(keyOrdinal, NULL);
    *value = oldValue;
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode CEnumMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mMappingsCount;
    return NOERROR;
}

ECode CEnumMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)

    if (mValuesCollection == NULL) {
        mValuesCollection = new EnumMapValueCollection(this);
    }
    *value = (ICollection*) mValuesCollection->Probe(EIID_ICollection);
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode CEnumMap::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    assert(0 && "TODO");
    // Initialization((InterfaceID)mKeyType);
    Int32 elementCount = 0;
    stream->Read(&elementCount);
    AutoPtr<IEnum> enumKey;
    AutoPtr<IInterface> value;
    // for (int i = elementCount; i > 0; i--) {
    //     enumKey = (Enum<K>) stream.readObject();
    //     value = stream.readObject();
    //     putImpl((K) enumKey, (V) value);
    // }
    return NOERROR;
}

ECode CEnumMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    stream->DefaultWriteObject();
    stream->Write(mMappingsCount);
    AutoPtr<ISet> outset;
    EntrySet((ISet**)&outset);
    AutoPtr<IIterator> iterator;
    outset->GetIterator((IIterator**)&iterator);
    Boolean isflag = FALSE;
    while (iterator->HasNext(&isflag), isflag) {
        AutoPtr<IMapEntry> entry;
        AutoPtr<IInterface> outface;
        iterator->Next((IInterface**)&outface);
        entry = IMapEntry::Probe(outface);
        assert(0 && "TODO");
        // stream.writeObject(entry.getKey());
        // stream.writeObject(entry.getValue());
    }
    return NOERROR;
}

Boolean CEnumMap::IsValidKeyType(
    /* [in] */ IInterface* key)
{
    if (key != NULL && key->Probe(mKeyType)) {
        return TRUE;
    }
    return FALSE;
}

ECode CEnumMap::Initialization(
    /* [in] */ IEnumMap* enumMap)
{
    AutoPtr<CEnumMap> cenum = (CEnumMap*)enumMap;
    mKeyType = cenum->mKeyType;
    mKeys = cenum->mKeys->Clone();
    mEnumSize = cenum->mEnumSize;
    mValues = cenum->mValues->Clone();
    mHasMapping = cenum->mHasMapping->Clone();
    mMappingsCount = cenum->mMappingsCount;
    return NOERROR;
}

ECode CEnumMap::Initialization(
    /* [in] */ InterfaceID type)
{
    mKeyType = type;
    AutoPtr< ArrayOf<IInterface*> > outarr = Enum::GetSharedConstants(mKeyType);
    mEnumSize = mKeys->GetLength();
    for (Int32 i = 0; i < mEnumSize; ++i) {
        mKeys->Set(i, IEnum::Probe((*outarr)[i]));
    }
    mValues = ArrayOf<IInterface*>::Alloc(mEnumSize);
    mHasMapping = ArrayOf<Boolean>::Alloc(mEnumSize);
    return NOERROR;
}

ECode CEnumMap::PutAllImpl(
    /* [in] */ IMap* map)
{
    AutoPtr<ISet> outset;
    map->EntrySet((ISet**)&outset);
    AutoPtr<IIterator> iter;
    outset->GetIterator((IIterator**)&iter);
    Boolean isflag = FALSE;
    while (iter->HasNext(&isflag), isflag) {
        AutoPtr<IInterface> outface;
        iter->Next((IInterface**)&outface);
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(outface);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        entry->GetKey((IInterface**)&keyface);
        entry->GetValue((IInterface**)&valueface);
        FAIL_RETURN(PutImpl(keyface, valueface, NULL));
    }
    return NOERROR;
}

ECode CEnumMap::PutImpl(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** oldValue)
{
    if (oldValue) {
        *oldValue = NULL;
    }

    if (key == NULL) {
        // throw new NullPointerException("key == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // Called to throw ClassCastException.
    if (key->Probe(mKeyType) == NULL) {
        return NOERROR;
    }
    Int32 keyOrdinal = 0;
    IEnum::Probe(key)->Ordinal(&keyOrdinal);
    if (!(*mHasMapping)[keyOrdinal]) {
        (*mHasMapping)[keyOrdinal] = TRUE;
        mMappingsCount++;
    }

    if (oldValue) {
        *oldValue = (*mValues)[keyOrdinal];
        INTERFACE_ADDREF(*oldValue);
    }

    mValues->Set(keyOrdinal, value);
    return NOERROR;
}

ECode CEnumMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode CEnumMap::ToString(
    /* [out] */ String* str)
{
    return AbstractMap::ToString(str);
}

ECode CEnumMap::GetHashCode(
    /* [out] */ Int32* value)
{
    return AbstractMap::GetHashCode(value);
}

//==========================================================
//       CEnumMap::EnumMapEntry
//==========================================================
CEnumMap::EnumMapEntry::EnumMapEntry(
    /* [in] */ IInterface* theKey,
    /* [in] */ IInterface* theValue,
    /* [in] */ CEnumMap* em)
    : MapEntry(theKey, theValue)
{
    mEnumMap = em;
    IEnum::Probe(theKey)->Ordinal(&mOrdinal);
}

ECode CEnumMap::EnumMapEntry::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(*mEnumMap->mHasMapping)[mOrdinal]) {
        *value = FALSE;
        return NOERROR;
    }
    Boolean isEqual = FALSE;
    if (IMapEntry::Probe(object)) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(object);
        AutoPtr<IInterface> enumKey;
        entry->GetKey((IInterface**)&enumKey);
        if (ObjectUtils::Equals(mKey, enumKey)) {
            AutoPtr<IInterface> theValue;
            entry->GetValue((IInterface**)&theValue);
            if ((*mEnumMap->mValues)[mOrdinal] == NULL) {
                isEqual = (theValue == NULL);
            }
            else {
                isEqual = ObjectUtils::Equals((*mEnumMap->mValues)[mOrdinal], theValue);
            }
        }
    }
    *value = isEqual;
    return NOERROR;
}

ECode CEnumMap::EnumMapEntry::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = ((*mEnumMap->mKeys)[mOrdinal] == NULL ? 0 : ObjectUtils::GetHashCode((*mEnumMap->mKeys)[mOrdinal]))
            ^ ((*mEnumMap->mValues)[mOrdinal] == NULL ? 0 : ObjectUtils::GetHashCode((*mEnumMap->mValues)[mOrdinal]));
    return NOERROR;
}

ECode CEnumMap::EnumMapEntry::GetKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    FAIL_RETURN(CheckEntryStatus());
    *outface = (*mEnumMap->mKeys)[mOrdinal];
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CEnumMap::EnumMapEntry::GetValue(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    FAIL_RETURN(CheckEntryStatus());
    *outface = (*mEnumMap->mValues)[mOrdinal];
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CEnumMap::EnumMapEntry::SetValue(
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    FAIL_RETURN(CheckEntryStatus());
    return mEnumMap->Put((*mEnumMap->mKeys)[mOrdinal], value, outface);
}

ECode CEnumMap::EnumMapEntry::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    StringBuilder result(ObjectUtils::ToString((*mEnumMap->mKeys)[mOrdinal]));
    result.AppendCStr("=");
    result.AppendString((*mEnumMap->mValues)[mOrdinal] == NULL
            ? String("null") : ObjectUtils::ToString((*mEnumMap->mValues)[mOrdinal]));
    return result.ToString(str);
}

ECode CEnumMap::EnumMapEntry::CheckEntryStatus()
{
    if (!(*mEnumMap->mHasMapping)[mOrdinal]) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}


//==========================================================
//       CEnumMap::EnumMapIterator
//==========================================================
CAR_INTERFACE_IMPL(CEnumMap::EnumMapIterator, IIterator);

CEnumMap::EnumMapIterator::EnumMapIterator(
    /* [in] */ MapEntry::Type* value,
    /* [in] */ CEnumMap* em)
    : mPosition(0)
    , mPrePosition(-1)
    , mEnumMap(em)
    , mType(value)
{
}

ECode CEnumMap::EnumMapIterator::HasNext(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Int32 length = mEnumMap->mEnumSize;
    for (; mPosition < length; mPosition++) {
        if ((*mEnumMap->mHasMapping)[mPosition]) {
            break;
        }
    }
    *value = mPosition != length;
    return NOERROR;
}

ECode CEnumMap::EnumMapIterator::Next(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    Boolean isflag = FALSE;
    if (!(HasNext(&isflag), isflag)) {
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    mPrePosition = mPosition++;
    AutoPtr<MapEntry> resent = new MapEntry((*mEnumMap->mKeys)[mPrePosition], (*mEnumMap->mValues)[mPrePosition]);
    return mType->Get(resent, outface);
}

ECode CEnumMap::EnumMapIterator::Remove()
{
    FAIL_RETURN(CheckStatus());
    AutoPtr<IInterface> outface;
    if ((*mEnumMap->mHasMapping)[mPrePosition]) {
        mEnumMap->Remove((*mEnumMap->mKeys)[mPrePosition], (IInterface**)&outface);
    }
    mPrePosition = -1;
    return NOERROR;
}

ECode CEnumMap::EnumMapIterator::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    if (-1 == mPrePosition) {
        *str = ObjectUtils::ToString(this->Probe(EIID_IInterface));
        return NOERROR;
    }
    AutoPtr<MapEntry> resent =  new MapEntry((*mEnumMap->mKeys)[mPrePosition], (*mEnumMap->mValues)[mPrePosition]);
    AutoPtr<IInterface> outface;
    mType->Get(resent, (IInterface**)&outface);
    *str = ObjectUtils::ToString(outface);
    return NOERROR;
}

ECode CEnumMap::EnumMapIterator::CheckStatus()
{
    if (-1 == mPrePosition) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}


//==========================================================
//       CEnumMap::EnumMapKeySet
//==========================================================

UInt32 CEnumMap::EnumMapKeySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CEnumMap::EnumMapKeySet::Release()
{
    return ElRefBase::Release();
}

PInterface CEnumMap::EnumMapKeySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode CEnumMap::EnumMapKeySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CEnumMap::EnumMapKeySet::EnumMapKeySet(
    /*  */ CEnumMap* em)
{
    mEnumMap = em;
}

ECode CEnumMap::EnumMapKeySet::Clear()
{
    return mEnumMap->Clear();
}

ECode CEnumMap::EnumMapKeySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    return mEnumMap->ContainsKey(object, value);
}

ECode CEnumMap::EnumMapKeySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<MapEntry::Type> restype = new EnumMapKeySetType();
    AutoPtr<IIterator> outres = (IIterator*) new EnumMapIterator(restype, mEnumMap);
    *outiter = outres;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode CEnumMap::EnumMapKeySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean isflag = FALSE;
    AutoPtr<IInterface> outface;
    if (Contains(object, &isflag), isflag) {
        mEnumMap->Remove(object, (IInterface**)&outface);
        *value = TRUE;
        return NOERROR;
    }
    *value = FALSE;
    return NOERROR;
}

ECode CEnumMap::EnumMapKeySet::GetSize(
    /* [out] */ Int32* value)
{
    return mEnumMap->GetSize(value);
}

ECode CEnumMap::EnumMapKeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CEnumMap::EnumMapKeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CEnumMap::EnumMapKeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CEnumMap::EnumMapKeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CEnumMap::EnumMapKeySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CEnumMap::EnumMapKeySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CEnumMap::EnumMapKeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CEnumMap::EnumMapKeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CEnumMap::EnumMapKeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CEnumMap::EnumMapKeySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

//==========================================================
//       CEnumMap::EnumMapKeySetType
//==========================================================

ECode CEnumMap::EnumMapKeySetType::Get(
    /* [in] */ IMapEntry* entry,
    /* [out] */ IInterface** outface)
{
    return entry->GetKey(outface);
}

//==========================================================
//       CEnumMap::EnumMapValueCollection
//==========================================================

UInt32 CEnumMap::EnumMapValueCollection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CEnumMap::EnumMapValueCollection::Release()
{
    return ElRefBase::Release();
}

PInterface CEnumMap::EnumMapValueCollection::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ICollection*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ICollection*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)this;
    }

    return NULL;
}

ECode CEnumMap::EnumMapValueCollection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ICollection*)this) {
        *pIID = EIID_ICollection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CEnumMap::EnumMapValueCollection::EnumMapValueCollection(
    /*  */ CEnumMap* em)
{
    mEnumMap = em;
}

ECode CEnumMap::EnumMapValueCollection::Clear()
{
    return mEnumMap->Clear();
}

ECode CEnumMap::EnumMapValueCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    return mEnumMap->ContainsValue(object, value);
}

ECode CEnumMap::EnumMapValueCollection::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<MapEntry::Type> outtype = new EnumMapValueCollectionType();
    AutoPtr<IIterator> resiter = (IIterator*) new EnumMapIterator(outtype, mEnumMap);
    *outiter = resiter;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode CEnumMap::EnumMapValueCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IInterface> outface;
    if (object == NULL) {
        for (Int32 i = 0; i < mEnumMap->mEnumSize; i++) {
            if ((*mEnumMap->mHasMapping)[i] && (*mEnumMap->mValues)[i] == NULL) {
                mEnumMap->Remove((*mEnumMap->mKeys)[i], (IInterface**)&outface);
                *value = TRUE;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = 0; i < mEnumMap->mEnumSize; i++) {
            if ((*mEnumMap->mHasMapping)[i]
                    && ObjectUtils::Equals(object, (*mEnumMap->mValues)[i])) {
                mEnumMap->Remove((*mEnumMap->mKeys)[i], (IInterface**)&outface);
                *value = TRUE;
                return NOERROR;
            }
        }
    }
    *value = FALSE;
    return NOERROR;
}

ECode CEnumMap::EnumMapValueCollection::GetSize(
    /* [out] */ Int32* value)
{
    return mEnumMap->GetSize(value);
}

ECode CEnumMap::EnumMapValueCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::Add(object, modified);
}

ECode CEnumMap::EnumMapValueCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::AddAll(collection, modified);
}

ECode CEnumMap::EnumMapValueCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::ContainsAll(collection, result);
}

ECode CEnumMap::EnumMapValueCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = object == this->Probe(EIID_IInterface);
    return NOERROR;
}

ECode CEnumMap::EnumMapValueCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    *hashCode = (Int32)this;
    return NOERROR;
}

ECode CEnumMap::EnumMapValueCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractCollection::IsEmpty(result);
}

ECode CEnumMap::EnumMapValueCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::RemoveAll(collection, modified);
}

ECode CEnumMap::EnumMapValueCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::RetainAll(collection, modified);
}

ECode CEnumMap::EnumMapValueCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractCollection::ToArray(array);
}

ECode CEnumMap::EnumMapValueCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractCollection::ToArray(inArray, outArray);
}


//==========================================================
//       CEnumMap::EnumMapValueCollectionType
//==========================================================

ECode CEnumMap::EnumMapValueCollectionType::Get(
    /* [in] */ IMapEntry* entry,
    /* [out] */ IInterface** outface)
{
    return entry->GetValue(outface);
}


//==========================================================
//       CEnumMap::EnumMapEntryIterator
//==========================================================
CEnumMap::EnumMapEntryIterator::EnumMapEntryIterator(
    /* [in] */ MapEntry::Type* value,
    /* [in] */ CEnumMap* em)
    : EnumMapIterator(value, em)
{
}

ECode CEnumMap::EnumMapEntryIterator::Next(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    Boolean isflag = FALSE;
    if (!(HasNext(&isflag), isflag)) {
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    mPrePosition = mPosition++;
    AutoPtr<MapEntry> outent= new EnumMapEntry((*mEnumMap->mKeys)[mPrePosition], (*mEnumMap->mValues)[mPrePosition], mEnumMap);
    return mType->Get(outent, outface);
}


//==========================================================
//       CEnumMap::EnumMapEntrySet
//==========================================================

UInt32 CEnumMap::EnumMapEntrySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CEnumMap::EnumMapEntrySet::Release()
{
    return ElRefBase::Release();
}

PInterface CEnumMap::EnumMapEntrySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode CEnumMap::EnumMapEntrySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CEnumMap::EnumMapEntrySet::EnumMapEntrySet(
    /*  */ CEnumMap* em)
{
    mEnumMap = em;
}

ECode CEnumMap::EnumMapEntrySet::Clear()
{
    return mEnumMap->Clear();
}

ECode CEnumMap::EnumMapEntrySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean isEqual = FALSE;
    if (IMapEntry::Probe(object)) {
        AutoPtr<IInterface> enumKey;
        IMapEntry::Probe(object)->GetKey((IInterface**)&enumKey);
        AutoPtr<IInterface> enumValue;
        IMapEntry::Probe(object)->GetValue((IInterface**)&enumValue);
        Boolean isflag = FALSE;
        if (mEnumMap->ContainsKey(enumKey, &isflag), isflag) {
            AutoPtr<IInterface> outvalue;
            mEnumMap->Get(enumKey, (IInterface**)&outvalue);
            if (outvalue == NULL) {
                isEqual = enumValue == NULL;
            }
            else {
                isEqual = ObjectUtils::Equals(outvalue, enumValue);
            }
        }
    }
    *value = isEqual;
    return NOERROR;
}

ECode CEnumMap::EnumMapEntrySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<MapEntry::Type> outtype = new EnumMapEntrySetType();
    AutoPtr<IIterator> outres = (IIterator*) new EnumMapEntryIterator(outtype, mEnumMap);
    *outiter = outres;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode CEnumMap::EnumMapEntrySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean isflag = FALSE;
    if (Contains(object, &isflag), isflag) {
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> outface;
        IMapEntry::Probe(object)->GetKey((IInterface**)&keyface);
        mEnumMap->Remove(keyface, (IInterface**)&outface);
        *value = TRUE;
        return NOERROR;
    }
    *value = FALSE;
    return NOERROR;
}

ECode CEnumMap::EnumMapEntrySet::GetSize(
    /* [out] */ Int32* value)
{
    return mEnumMap->GetSize(value);
}

ECode CEnumMap::EnumMapEntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CEnumMap::EnumMapEntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CEnumMap::EnumMapEntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CEnumMap::EnumMapEntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CEnumMap::EnumMapEntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CEnumMap::EnumMapEntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CEnumMap::EnumMapEntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CEnumMap::EnumMapEntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CEnumMap::EnumMapEntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)

    Int32 len = 0;
    mEnumMap->GetSize(&len);
    AutoPtr< ArrayOf<IInterface*> > entryArray = ArrayOf<IInterface*>::Alloc(len);
    return ToArray(entryArray, array);
}

ECode CEnumMap::EnumMapEntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)

    Int32 size = 0;
    mEnumMap->GetSize(&size);
    Int32 index = 0;
    AutoPtr< ArrayOf<IInterface*> > entryArray = inArray;
    if (size > inArray->GetLength()) {
        // Class<?> clazz = inArray.getClass().getComponentType();
        // entryArray = (Object[]) Array.newInstance(clazz, size);
        entryArray = ArrayOf<IInterface*>::Alloc(size);
    }
    AutoPtr<IIterator> iter;
    GetIterator((IIterator**)&iter);
    for (; index < size; index++) {
        AutoPtr<IInterface> outface;
        iter->Next((IInterface**)&outface);
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(outface);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        entry->GetKey((IInterface**)&keyface);
        entry->GetValue((IInterface**)&valueface);
        AutoPtr<IMapEntry> outentr =  new MapEntry(keyface, valueface);
        entryArray->Set(index, outentr);
    }
    if (index < inArray->GetLength()) {
        (*entryArray)[index] = NULL;
    }
    *outArray = entryArray;
    INTERFACE_ADDREF(*outArray)
    return NOERROR;
}


//==========================================================
//       CEnumMap::EnumMapEntrySetType
//==========================================================

ECode CEnumMap::EnumMapEntrySetType::Get(
    /* [in] */ IMapEntry* entry,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    *outface = entry;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
