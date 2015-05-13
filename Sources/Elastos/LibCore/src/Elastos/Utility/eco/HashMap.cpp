
#include "CHashMap.h"
#include "CFloat.h"
#include "elastos/ObjectUtils.h"

using Elastos::Core::ObjectUtils;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::IBoolean;
using Elastos::Core::EIID_IBoolean;
using Elastos::Core::IByte;
using Elastos::Core::EIID_IByte;
using Elastos::Core::IChar16;
using Elastos::Core::EIID_IChar16;
using Elastos::Core::IChar32;
using Elastos::Core::EIID_IChar32;
using Elastos::Core::IChar8;
using Elastos::Core::EIID_IChar8;
using Elastos::Core::ICharSequence;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::EIID_IDouble;
using Elastos::Core::IDouble;
using Elastos::Core::EIID_IFloat;
using Elastos::Core::IInteger16;
using Elastos::Core::EIID_IInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::EIID_IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::EIID_IInteger64;
using Elastos::IO::IObjectOutputStreamPutField;

namespace Elastos {
namespace Utility {


//==========================================================
//       HashMap
//==========================================================
const Int64 _HashMap::sSerialVersionUID;

AutoPtr< ArrayOf<IObjectStreamField*> > _HashMap::sSerialPersistentFields;

const Int32 _HashMap::MINIMUM_CAPACITY;

const Int32 _HashMap::MAXIMUM_CAPACITY;

const AutoPtr< ArrayOf<_HashMap::HashMapEntry*> > _HashMap::EMPTY_TABLE =  ArrayOf<_HashMap::HashMapEntry*>::Alloc(MINIMUM_CAPACITY >> 1) ; // new HashMapEntry[MINIMUM_CAPACITY >> 1];

const Float _HashMap::DEFAULT_LOAD_FACTOR;

_HashMap::_HashMap()
    : mModCount(0)
    , mSize(0)
    , mThreshold(0)
{
}

ECode _HashMap::Init()
{
    mTable = EMPTY_TABLE;
    mThreshold = -1; // Forces first put invocation to replace EMPTY_TABLE
    return NOERROR;
}

ECode _HashMap::Init(
    /* [in] */ Int32 capacity)
{
    if (capacity < 0) {
        // throw new IllegalArgumentException("Capacity: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (capacity == 0) {
        // @SuppressWarnings("unchecked")
        AutoPtr< ArrayOf<HashMapEntry*> > tab = EMPTY_TABLE;
        mTable = tab;
        mThreshold = -1; // Forces first put() to replace EMPTY_TABLE
        return NOERROR;
    }

    if (capacity < MINIMUM_CAPACITY) {
        capacity = MINIMUM_CAPACITY;
    }
    else if (capacity > MAXIMUM_CAPACITY) {
        capacity = MAXIMUM_CAPACITY;
    }
    else {
        capacity = RoundUpToPowerOfTwo(capacity);
    }
    MakeTable(capacity);
    return NOERROR;
}

ECode _HashMap::Init(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    FAIL_RETURN(this->Init(capacity));

    AutoPtr<IFloat> flo;
    CFloat::New(loadFactor, (IFloat**)&flo);
    Boolean isflag = FALSE;
    if (loadFactor <= 0 || (flo->IsNaN(&isflag), isflag)) {
        // throw new IllegalArgumentException("Load factor: " + loadFactor);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    /*
     * Note that this implementation ignores loadFactor; it always uses
     * a load factor of 3/4. This simplifies the code and generally
     * improves performance.
     */
    return NOERROR;
}

ECode _HashMap::Init(
    /* [in] */ IMap* map)
{
    Int32 sizelen = 0;
    map->GetSize(&sizelen);
    FAIL_RETURN(this->Init(CapacityForInitSize(sizelen)));
    return ConstructorPutAll(map);
}

ECode _HashMap::ConstructorPutAll(
    /* [in] */ IMap* map)
{
    AutoPtr< ArrayOf<IInterface*> > es;
    AutoPtr<ISet> outset;
    map->EntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&es);
    for (Int32 i = 0; i< es->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*es)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        e->GetKey((IInterface**)&keyface);
        e->GetValue((IInterface**)&valueface);
        ConstructorPut(keyface, valueface);
    }
    return NOERROR;
}

Int32 _HashMap::CapacityForInitSize(
    /* [in] */ Int32 size)
{
    Int32 result = (size >> 1) + size; // Multiply by 3/2 to allow for growth

    // boolean expr is equivalent to result >= 0 && result<MAXIMUM_CAPACITY
    return (result & ~(MAXIMUM_CAPACITY-1))==0 ? result : MAXIMUM_CAPACITY;
}

ECode _HashMap::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    /*
     * This could be made more efficient. It unnecessarily hashes all of
     * the elements in the map.
     */
    AutoPtr<CHashMap> result;
    // try {
    // result = (HashMap<K, V>) super.clone();
    CHashMap::NewByFriend((CHashMap**)&result);
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }

    // Restore clone to empty state, retaining our capacity and threshold
    result->MakeTable(mTable->GetLength());
    result->mEntryForNullKey = NULL;
    result->mSize = 0;
    result->mKeySet = NULL;
    result->mEntrySet = NULL;
    result->mValues = NULL;

    result->Init_(); // Give subclass a chance to initialize itself
    result->ConstructorPutAll((IMap*)this->Probe(EIID_IMap)); // Calls method overridden in subclass!!
    AutoPtr<IMap> outmap = (IMap*) result->Probe(EIID_IMap);
    *object = outmap;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode _HashMap::Init_()
{
    return NOERROR;
}

ECode _HashMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mSize == 0;
    return NOERROR;
}

ECode _HashMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mSize;
    return NOERROR;
}

ECode _HashMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    if (key == NULL) {
        AutoPtr<HashMapEntry> e = mEntryForNullKey;
        *value = e == NULL ? NULL : e->mValue;
        INTERFACE_ADDREF(*value)
        return NOERROR;
    }

    // Doug Lea's supplemental secondaryHash function (inlined)
    Int32 hash = ObjectUtils::GetHashCode(key);
    hash ^= ((UInt32)hash >> 20) ^ ((UInt32)hash >> 12);
    hash ^= ((UInt32)hash >> 7) ^ ((UInt32)hash >> 4);

    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    for (AutoPtr<HashMapEntry> e = (*tab)[hash & (tab->GetLength() - 1)]; e != NULL; e = e->mNext) {
        AutoPtr<IInterface> eKey = e->mKey;
        if (eKey.Get() == key || (e->mHash == hash && ObjectUtils::Equals(key, eKey))) {
            *value = e->mValue;
            INTERFACE_ADDREF(*value)
            return NOERROR;
        }
    }
    *value = NULL;
    return NOERROR;
}

ECode _HashMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (key == NULL) {
        *result = mEntryForNullKey != NULL;
        return NOERROR;
    }

    // Doug Lea's supplemental secondaryHash function (inlined)
    Int32 hash = ObjectUtils::GetHashCode(key);
    hash ^= ((UInt32)hash >> 20) ^ ((UInt32)hash >> 12);
    hash ^= ((UInt32)hash >> 7) ^ ((UInt32)hash >> 4);

    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    for (AutoPtr<HashMapEntry> e = (*tab)[hash & (tab->GetLength() - 1)]; e != NULL; e = e->mNext) {
        AutoPtr<IInterface> eKey = e->mKey;
        if (eKey.Get() == key || (e->mHash == hash && ObjectUtils::Equals(key, eKey))) {
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode _HashMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    Int32 len = tab->GetLength();
    if (value == NULL) {
        for (Int32 i = 0; i < len; i++) {
            for (AutoPtr<HashMapEntry> e = (*tab)[i]; e != NULL; e = e->mNext) {
                if (e->mValue == NULL) {
                    *result = TRUE;
                    return NOERROR;
                }
            }
        }
        *result = mEntryForNullKey != NULL && mEntryForNullKey->mValue == NULL;
        return NOERROR;
    }

    // value is non-null
    for (Int32 i = 0; i < len; i++) {
        for (AutoPtr<HashMapEntry> e = (*tab)[i]; e != NULL; e = e->mNext) {
            if (value == e->mValue) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = mEntryForNullKey != NULL && value == mEntryForNullKey->mValue;
    return NOERROR;
}

ECode _HashMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    if (key == NULL) {
        if (oldValue) {
            AutoPtr<IInterface> outface = PutValueForNullKey(value);
            *oldValue = outface;
            INTERFACE_ADDREF(*oldValue)
        }
        else {
            PutValueForNullKey(value);
        }
        return NOERROR;
    }

    Int32 keyhash = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keyhash);
    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    for (AutoPtr<HashMapEntry> e = (*tab)[index]; e != NULL; e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            PreModify(e);
            if (oldValue) {
                *oldValue = e->mValue;
                INTERFACE_ADDREF(*oldValue)
            }
            e->mValue = value;
            return NOERROR;
        }
    }

    // No entry for (non-null) key is present; create one
    mModCount++;
    if (mSize++ > mThreshold) {
        tab = DoubleCapacity();
        index = hash & (tab->GetLength() - 1);
    }
    AddNewEntry(key, value, hash, index);
    if (oldValue) {
        *oldValue = NULL;
    }
    return NOERROR;
}

AutoPtr<IInterface> _HashMap::PutValueForNullKey(
    /* [in] */ IInterface* value)
{
    AutoPtr<HashMapEntry> entry = mEntryForNullKey;
    if (entry == NULL) {
        AddNewEntryForNullKey(value);
        mSize++;
        mModCount++;
        return NULL;
    }
    else {
        PreModify(entry);
        AutoPtr<IInterface> oldValue = entry->mValue;
        entry->mValue = value;
        return oldValue;
    }
}

ECode _HashMap::PreModify(
    /* [in] */ HashMapEntry* e)
{
    return NOERROR;
}

ECode _HashMap::ConstructorPut(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    if (key == NULL) {
        AutoPtr<HashMapEntry> entry = mEntryForNullKey;
        if (entry == NULL) {
            mEntryForNullKey = ConstructorNewEntry(NULL, value, 0, NULL);
            mSize++;
        }
        else {
            entry->mValue = value;
        }
        return NOERROR;
    }

    Int32 keycode = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keycode);
    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    AutoPtr<HashMapEntry> first = (*tab)[index];
    for (AutoPtr<HashMapEntry> e = first; e != NULL; e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            e->mValue = value;
            return NOERROR;
        }
    }

    // No entry for (non-null) key is present; create one
    AutoPtr<HashMapEntry> outentry = ConstructorNewEntry(key, value, hash, first);
    tab->Set(index, outentry);
    mSize++;
    return NOERROR;
}

ECode _HashMap::AddNewEntry(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 hash,
    /* [in] */ Int32 index)
{
    AutoPtr<HashMapEntry> res = new HashMapEntry(key, value, hash, (*mTable)[index]);
    mTable->Set(index, res);
    return NOERROR;
}

ECode _HashMap::AddNewEntryForNullKey(
    /* [in] */ IInterface* value)
{
    mEntryForNullKey = new HashMapEntry(NULL, value, 0, NULL);
    return NOERROR;
}

AutoPtr<_HashMap::HashMapEntry> _HashMap::ConstructorNewEntry(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 hash,
    /* [in] */ HashMapEntry* first)
{
    AutoPtr<HashMapEntry> res = new HashMapEntry(key, value, hash, first);
    return res;
}

ECode _HashMap::PutAll(
    /* [in] */ IMap* map)
{
    Int32 sizelen = 0;
    map->GetSize(&sizelen);
    EnsureCapacity(sizelen);
    return AbstractMap::PutAll(map);
}

ECode _HashMap::EnsureCapacity(
    /* [in] */ Int32 numMappings)
{
    Int32 newCapacity = RoundUpToPowerOfTwo(CapacityForInitSize(numMappings));
    AutoPtr< ArrayOf<HashMapEntry*> > oldTable = mTable;
    Int32 oldCapacity = oldTable->GetLength();
    if (newCapacity <= oldCapacity) {
        return NOERROR;
    }
    if (newCapacity == oldCapacity * 2) {
        DoubleCapacity();
        return NOERROR;
    }

    // We're growing by at least 4x, rehash in the obvious way
    AutoPtr< ArrayOf<HashMapEntry*> > newTable = MakeTable(newCapacity);
    if (mSize != 0) {
        Int32 newMask = newCapacity - 1;
        for (Int32 i = 0; i < oldCapacity; i++) {
            for (AutoPtr<HashMapEntry> e = (*oldTable)[i]; e != NULL;) {
                AutoPtr<HashMapEntry> oldNext = e->mNext;
                Int32 newIndex = e->mHash & newMask;
                AutoPtr<HashMapEntry> newNext = (*newTable)[newIndex];
                newTable->Set(newIndex, e);
                e->mNext = newNext;
                e = oldNext;
            }
        }
    }
    return NOERROR;
}

AutoPtr< ArrayOf<_HashMap::HashMapEntry*> > _HashMap::MakeTable(
    /* [in] */ Int32 newCapacity)
{
    // @SuppressWarnings("unchecked")
    AutoPtr< ArrayOf<HashMapEntry*> > newTable = ArrayOf<HashMapEntry*>::Alloc(newCapacity);
    mTable = newTable;
    mThreshold = (newCapacity >> 1) + (newCapacity >> 2); // 3/4 capacity
    return newTable;
}

AutoPtr< ArrayOf<_HashMap::HashMapEntry*> > _HashMap::DoubleCapacity()
{
    AutoPtr< ArrayOf<HashMapEntry*> > oldTable = mTable;
    Int32 oldCapacity = oldTable->GetLength();
    if (oldCapacity == MAXIMUM_CAPACITY) {
        return oldTable;
    }
    Int32 newCapacity = oldCapacity * 2;
    AutoPtr< ArrayOf<HashMapEntry*> > newTable = MakeTable(newCapacity);
    if (mSize == 0) {
        return newTable;
    }

    for (Int32 j = 0; j < oldCapacity; j++) {
        /*
         * Rehash the bucket using the minimum number of field writes.
         * This is the most subtle and delicate code in the class.
         */
        AutoPtr<HashMapEntry> e = (*oldTable)[j];
        if (e == NULL) {
            continue;
        }
        Int32 highBit = e->mHash & oldCapacity;
        AutoPtr<HashMapEntry> broken;
        newTable->Set(j | highBit, e);
        for (AutoPtr<HashMapEntry> n = e->mNext; n != NULL; e = n, n = n->mNext) {
            Int32 nextHighBit = n->mHash & oldCapacity;
            if (nextHighBit != highBit) {
                if (broken == NULL) {
                    newTable->Set(j | nextHighBit, n);
                }
                else {
                    broken->mNext = n;
                }
                broken = e;
                highBit = nextHighBit;
            }
        }
        if (broken != NULL) {
            broken->mNext = NULL;
        }
    }
    return newTable;
}

ECode _HashMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)

    if (key == NULL) {
        *value = RemoveNullKey();
        INTERFACE_ADDREF(*value)
        return NOERROR;
    }
    Int32 keyhash = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keyhash);
    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    AutoPtr<HashMapEntry> prev;
    AutoPtr<HashMapEntry> e;
    for (e = (*tab)[index], prev = NULL; e != NULL; prev = e, e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            if (prev == NULL) {
                tab->Set(index, e->mNext);
            }
            else {
                prev->mNext = e->mNext;
            }
            mModCount++;
            mSize--;
            PostRemove(e);
            *value = e->mValue;
            INTERFACE_ADDREF(*value)
            return NOERROR;
        }
    }
    *value = NULL;
    return NOERROR;
}

AutoPtr<IInterface> _HashMap::RemoveNullKey()
{
    AutoPtr<HashMapEntry> e = mEntryForNullKey;
    if (e == NULL) {
        return NULL;
    }
    mEntryForNullKey = NULL;
    mModCount++;
    mSize--;
    PostRemove(e);
    return e->mValue;
}

ECode _HashMap::PostRemove(
    /* [in] */ HashMapEntry* e)
{
    return NOERROR;
}

ECode _HashMap::Clear()
{
    if (mSize != 0) {
        // Arrays.fill(table, null);
        for (Int32 i = 0; i < mTable->GetLength(); ++i) {
            mTable->Set(i, NULL);
        }
        mEntryForNullKey = NULL;
        mModCount++;
        mSize = 0;
    }
    return NOERROR;
}

ECode _HashMap::KeySet(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    AutoPtr<ISet> ks = mKeySet;
    if (ks != NULL) {
        *keySet = ks;
        INTERFACE_ADDREF(*keySet);
        return NOERROR;
    }
    else {
        mKeySet = (ISet*) new _KeySet(this);
        *keySet = mKeySet;
        INTERFACE_ADDREF(*keySet);
        return NOERROR;
    }
}

ECode _HashMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<ICollection> vs = mValues;
    *value = (vs != NULL) ? vs : (mValues = (ICollection*) new _HashMap::_Values(this));
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode _HashMap::EntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)

    AutoPtr<ISet> es = mEntrySet;
    if (es != NULL) {
        *entries = es;
        INTERFACE_ADDREF(*entries)
        return NOERROR;
    }
    else {
        mEntrySet = (ISet*) new _EntrySet(this);
        *entries = mEntrySet;
        INTERFACE_ADDREF(*entries)
        return NOERROR;
    }
}

Boolean _HashMap::ContainsMapping(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    if (key == NULL) {
        AutoPtr<HashMapEntry> e = mEntryForNullKey;
        return e != NULL && (value == e->mValue);
    }
    Int32 keyhash = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keyhash);
    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    for (AutoPtr<HashMapEntry> e = (*tab)[index]; e != NULL; e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            return value == e->mValue;
        }
    }
    return FALSE; // No entry for key
}

Boolean _HashMap::RemoveMapping(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    if (key == NULL) {
        AutoPtr<HashMapEntry> e = mEntryForNullKey;
        if (e == NULL || !(value == e->mValue)) {
            return FALSE;
        }
        mEntryForNullKey = NULL;
        mModCount++;
        mSize--;
        PostRemove(e);
        return TRUE;
    }

    Int32 keyhash = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keyhash);
    AutoPtr< ArrayOf<HashMapEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    for (AutoPtr<HashMapEntry> e = (*tab)[index], prev = NULL;
            e != NULL; prev = e, e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            if (!(value == e->mValue)) {
                return FALSE;  // Map has wrong value for key
            }
            if (prev == NULL) {
                tab->Set(index, e->mNext);
            }
            else {
                prev->mNext = e->mNext;
            }
            mModCount++;
            mSize--;
            PostRemove(e);
            return TRUE;
        }
    }
    return FALSE; // No entry for key
}

AutoPtr<IIterator> _HashMap::NewKeyIterator()
{
    AutoPtr<IIterator> res = (IIterator*) new KeyIterator(this);
    return res;
}

AutoPtr<IIterator> _HashMap::NewValueIterator()
{
    AutoPtr<IIterator> res = (IIterator*) new ValueIterator(this);
    return res;
}

AutoPtr<IIterator> _HashMap::NewEntryIterator()
{
    AutoPtr<IIterator> res = (IIterator*) new EntryIterator(this);
    return res;
}

Int32 _HashMap::SecondaryHash(
    /* [in] */ Int32 h)
{
    // Doug Lea's supplemental hash function
    h ^= ((UInt32)h >> 20) ^ ((UInt32)h >> 12);
    return h ^ ((UInt32)h >> 7) ^ ((UInt32)h >> 4);
}

Int32 _HashMap::RoundUpToPowerOfTwo(
    /* [in] */ Int32 i)
{
    i--; // If input is a power of two, shift its high-order bit right

    // "Smear" the high-order bit all the way to the right
    i |= (UInt32)i >>  1;
    i |= (UInt32)i >>  2;
    i |= (UInt32)i >>  4;
    i |= (UInt32)i >>  8;
    i |= (UInt32)i >> 16;

    return i + 1;
}

ECode _HashMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    // Emulate loadFactor field for other implementations to read
    AutoPtr<IObjectOutputStreamPutField> fields;
    stream->PutFields((IObjectOutputStreamPutField**)&fields);
    fields->PutFloat(String("loadFactor"), DEFAULT_LOAD_FACTOR);
    stream->WriteFields();

    stream->Write(mTable->GetLength()); // Capacity
    stream->Write(mSize);
    AutoPtr< ArrayOf<IInterface*> > outentry;
    AutoPtr<ISet> outset;
    EntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&outentry);
    for (Int32 i = 0; i < outentry->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*outentry)[i]);
        AutoPtr<IInterface> eKey;
        e->GetKey((IInterface**)&eKey);
        AutoPtr<IInterface> valuekey;
        e->GetValue((IInterface**)&valuekey);
        assert(0 && "TODO");
        // stream->WriteObject(eKey);
        // stream->WriteObject(valuekey);
    }
    return NOERROR;
}

ECode _HashMap::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    Int32 capacity = 0;
    stream->Read(&capacity);
    if (capacity < 0) {
        // throw new InvalidObjectException("Capacity: " + capacity);
        return E_INVALID_OBJECT_EXCEPTION;
    }
    if (capacity < MINIMUM_CAPACITY) {
        capacity = MINIMUM_CAPACITY;
    }
    else if (capacity > MAXIMUM_CAPACITY) {
        capacity = MAXIMUM_CAPACITY;
    }
    else {
        capacity = RoundUpToPowerOfTwo(capacity);
    }
    MakeTable(capacity);

    Int32 size = 0;
    stream->Read(&size);
    if (size < 0) {
        // throw new InvalidObjectException("Size: " + size);
        return E_INVALID_OBJECT_EXCEPTION;
    }

    Init_(); // Give subclass (LinkedHashMap) a chance to initialize itself
    for (Int32 i = 0; i < size; i++) {
        assert(0 && "TODO");
        // @SuppressWarnings("unchecked") K key = (K) stream.readObject();
        // @SuppressWarnings("unchecked") V val = (V) stream.readObject();
        // constructorPut(key, val);
    }
    return NOERROR;
}


//==========================================================
//       HashMap::HashMapEntry
//==========================================================
CAR_INTERFACE_IMPL(_HashMap::HashMapEntry, IMapEntry)

_HashMap::HashMapEntry::HashMapEntry(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 hash,
    /* [in] */ HashMapEntry* next)
{
    mKey = key;
    mValue = value;
    mHash = hash;
    mNext = next;
}

ECode _HashMap::HashMapEntry::GetKey(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key)

    *key = mKey;
    INTERFACE_ADDREF(*key)
    return NOERROR;
}

ECode _HashMap::HashMapEntry::GetValue(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key)

    *key = mValue;
    INTERFACE_ADDREF(*key)
    return NOERROR;
}

ECode _HashMap::HashMapEntry::SetValue(
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** valueReplacee)
{
    VALIDATE_NOT_NULL(valueReplacee)

    AutoPtr<IInterface> oldValue = mValue;
    mValue = value;
    *valueReplacee = oldValue;
    INTERFACE_ADDREF(*valueReplacee)
    return NOERROR;
}

ECode _HashMap::HashMapEntry::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (!(IMapEntry::Probe(o))) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> eKey;
    e->GetKey((IInterface**)&eKey);
    AutoPtr<IInterface> valuekey;
    e->GetValue((IInterface**)&valuekey);

    *result = ObjectUtils::Equals(eKey, mKey) && ObjectUtils::Equals(valuekey, mValue);
    return NOERROR;
}

ECode _HashMap::HashMapEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    Int32 keycode = ObjectUtils::GetHashCode(mKey);
    Int32 valuecode = ObjectUtils::GetHashCode(mValue);
    *hashCode = (mKey == NULL ? 0 : keycode) ^ (mValue == NULL ? 0 : valuecode);
    return NOERROR;
}

ECode _HashMap::HashMapEntry::ToString(
    /* [out] */ String *str)
{
    VALIDATE_NOT_NULL(str)

    *str = ObjectUtils::ToString(mKey) + String("=") + ObjectUtils::ToString(mValue);
    return NOERROR;
}


//==========================================================
//       HashMap::HashIterator
//==========================================================
_HashMap::HashIterator::HashIterator()
{
}

_HashMap::HashIterator::HashIterator(
    /* [in] */ _HashMap* host)
{
    mHost = host;
    mExpectedModCount = mHost->mModCount;
    mNextEntry = mHost->mEntryForNullKey;
    mLastEntryReturned = NULL;
    mNextIndex = 0;
    if (mNextEntry == NULL) {
        AutoPtr< ArrayOf<HashMapEntry*> > tab = mHost->mTable;
        AutoPtr<HashMapEntry> next;
        while (next == NULL && mNextIndex < tab->GetLength()) {
            next = (*tab)[mNextIndex++];
        }
        mNextEntry = next;
    }
}

ECode _HashMap::HashIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mNextEntry != NULL;
    return NOERROR;
}

AutoPtr<_HashMap::HashMapEntry> _HashMap::HashIterator::NextEntry()
{
    if (mHost->mModCount != mExpectedModCount) {
        // throw new ConcurrentModificationException();
        return NULL;
    }
    if (mNextEntry == NULL) {
        // throw new NoSuchElementException();
        return NULL;
    }

    AutoPtr<HashMapEntry> entryToReturn = mNextEntry;
    AutoPtr< ArrayOf<HashMapEntry*> > tab = mHost->mTable;
    AutoPtr<HashMapEntry> next = entryToReturn->mNext;
    while (next == NULL && mNextIndex < tab->GetLength()) {
        next = (*tab)[mNextIndex++];
    }
    mNextEntry = next;
    mLastEntryReturned = entryToReturn;

    return mLastEntryReturned;
}

ECode _HashMap::HashIterator::Remove()
{
    if (mLastEntryReturned == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mHost->mModCount != mExpectedModCount) {
        // throw new ConcurrentModificationException();
        return E_CONCURRENT_MODIFICATION_EXCEPTION;
    }
    AutoPtr<IInterface> outface;
    mHost->Remove(mLastEntryReturned->mKey, (IInterface**)&outface);
    mLastEntryReturned = NULL;
    mExpectedModCount = mHost->mModCount;
    return NOERROR;
}


//==========================================================
//       HashMap::KeyIterator
//==========================================================
CAR_INTERFACE_IMPL(_HashMap::KeyIterator, IIterator);

_HashMap::KeyIterator::KeyIterator(
    /* [in] */ _HashMap* host)
    : HashIterator(host)
{
}

ECode _HashMap::KeyIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashMapEntry> res = NextEntry();
    *object = res->mKey;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode _HashMap::KeyIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode _HashMap::KeyIterator::Remove()
{
    return HashIterator::Remove();
}


//==========================================================
//       HashMap::ValueIterator
//==========================================================
CAR_INTERFACE_IMPL(_HashMap::ValueIterator, IIterator);

_HashMap::ValueIterator::ValueIterator(
    /* [in] */ _HashMap* host)
    : HashIterator(host)
{
}

ECode _HashMap::ValueIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashMapEntry> res = NextEntry();
    *object = res->mValue;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode _HashMap::ValueIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode _HashMap::ValueIterator::Remove()
{
    return HashIterator::Remove();
}


//==========================================================
//       HashMap::EntryIterator
//==========================================================
CAR_INTERFACE_IMPL(_HashMap::EntryIterator, IIterator);

_HashMap::EntryIterator::EntryIterator(
    /* [in] */ _HashMap* host)
    : HashIterator(host)
{
}

ECode _HashMap::EntryIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashMapEntry> outhash = NextEntry();
    *object = outhash;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode _HashMap::EntryIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode _HashMap::EntryIterator::Remove()
{
    return HashIterator::Remove();
}


//==========================================================
//       HashMap::_KeySet
//==========================================================
UInt32 _HashMap::_KeySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 _HashMap::_KeySet::Release()
{
    return ElRefBase::Release();
}

PInterface _HashMap::_KeySet::Probe(
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

ECode _HashMap::_KeySet::GetInterfaceID(
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

_HashMap::_KeySet::_KeySet(
    /* [in] */ _HashMap* host)
{
    mHost = host;
}

ECode _HashMap::_KeySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IIterator> res = mHost->NewKeyIterator();
    *result = res;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode _HashMap::_KeySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mHost->mSize;
    return NOERROR;
}

ECode _HashMap::_KeySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mHost->mSize ==0;
    return NOERROR;
}

ECode _HashMap::_KeySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return mHost->ContainsKey(o, result);
}

ECode _HashMap::_KeySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 oldSize = mHost->mSize;
    AutoPtr<IInterface> outface;
    mHost->Remove(o, (IInterface**)&outface);
    *result = mHost->mSize != oldSize;
    return NOERROR;
}

ECode _HashMap::_KeySet::Clear()
{
    return mHost->Clear();
}

ECode _HashMap::_KeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode _HashMap::_KeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode _HashMap::_KeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode _HashMap::_KeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RemoveAll(collection, result);
}

ECode _HashMap::_KeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RetainAll(collection, result);
}

ECode _HashMap::_KeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode _HashMap::_KeySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArrayEx(contents, outArray);
}

ECode _HashMap::_KeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode _HashMap::_KeySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

//==========================================================
//       HashMap::_Values
//==========================================================
UInt32 _HashMap::_Values::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 _HashMap::_Values::Release()
{
    return ElRefBase::Release();
}

PInterface _HashMap::_Values::Probe(
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

ECode _HashMap::_Values::GetInterfaceID(
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

_HashMap::_Values::_Values()
{
}

_HashMap::_Values::_Values(
    /* [in] */ _HashMap* host)
{
    mHost = host;
}

ECode _HashMap::_Values::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IIterator> outiter = mHost->NewValueIterator();
    *result = outiter;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode _HashMap::_Values::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mHost->mSize;
    return NOERROR;
}

ECode _HashMap::_Values::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mHost->mSize == 0;
    return NOERROR;
}

ECode _HashMap::_Values::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return mHost->ContainsValue(o, result);
}

ECode _HashMap::_Values::Clear()
{
    return mHost->Clear();
}

ECode _HashMap::_Values::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::Add(object, modified);
}

ECode _HashMap::_Values::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::AddAll(collection, modified);
}

ECode _HashMap::_Values::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::ContainsAll(collection, result);
}

ECode _HashMap::_Values::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::Remove(object, result);
}

ECode _HashMap::_Values::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::RemoveAll(collection, result);
}

ECode _HashMap::_Values::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::RetainAll(collection, result);
}

ECode _HashMap::_Values::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractCollection::ToArray(array);
}

ECode _HashMap::_Values::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractCollection::ToArrayEx(contents, outArray);
}

ECode _HashMap::_Values::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode _HashMap::_Values::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

//==========================================================
//       HashMap::_EntrySet
//==========================================================
UInt32 _HashMap::_EntrySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 _HashMap::_EntrySet::Release()
{
    return ElRefBase::Release();
}

PInterface _HashMap::_EntrySet::Probe(
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

ECode _HashMap::_EntrySet::GetInterfaceID(
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

_HashMap::_EntrySet::_EntrySet(
    /* [in] */ _HashMap* host)
{
    mHost = host;
}

ECode _HashMap::_EntrySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IIterator> res = mHost->NewEntryIterator();
    *result = res;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode _HashMap::_EntrySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (!(IMapEntry::Probe(o))) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    AutoPtr<IInterface> valueface;
    e->GetKey((IInterface**)&keyface);
    e->GetValue((IInterface**)&valueface);
    *result = mHost->ContainsMapping(keyface, valueface);
    return NOERROR;
}

ECode _HashMap::_EntrySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (!(IMapEntry::Probe(o))) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    AutoPtr<IInterface> valueface;
    e->GetKey((IInterface**)&keyface);
    e->GetValue((IInterface**)&valueface);
    *result = mHost->RemoveMapping(keyface, valueface);
    return NOERROR;
}

ECode _HashMap::_EntrySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mHost->mSize;
    return NOERROR;
}

ECode _HashMap::_EntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mHost->mSize == 0;
    return NOERROR;
}

ECode _HashMap::_EntrySet::Clear()
{
    return mHost->Clear();
}

ECode _HashMap::_EntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode _HashMap::_EntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode _HashMap::_EntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode _HashMap::_EntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode _HashMap::_EntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode _HashMap::_EntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode _HashMap::_EntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode _HashMap::_EntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode _HashMap::_EntrySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArrayEx(inArray, outArray);
}

} // namespace Utility
} // namespace Elastos
