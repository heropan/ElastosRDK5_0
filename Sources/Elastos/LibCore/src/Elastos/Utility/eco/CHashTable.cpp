
#include "CHashTable.h"

namespace Elastos {
namespace Utility {

PInterface CHashTable::Probe(
    /* [in] */ REIID riid)
{
    return _CHashTable::Probe(riid);
}

UInt32 CHashTable::AddRef()
{
    return _CHashTable::AddRef();
}

UInt32 CHashTable::Release()
{
    return _CHashTable::Release();
}

ECode CHashTable::constructor()
{
    return HashTable::Init();
}

ECode CHashTable::constructor(
    /* [in] */ Int32 capacity)
{
    return HashTable::Init(capacity);
}

ECode CHashTable::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    return HashTable::Init(capacity, loadFactor);
}

ECode CHashTable::constructor(
    /* [in] */ IMap* map)
{
    return HashTable::Init(map);
}

ECode CHashTable::Clear()
{
    return HashTable::Clear();
}

ECode CHashTable::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return HashTable::ContainsKey(key, result);
}

ECode CHashTable::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return HashTable::ContainsValue(value, result);
}

ECode CHashTable::EntrySet(
    /* [out] */ ISet** entries)
{
    return HashTable::EntrySet(entries);
}

ECode CHashTable::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return HashTable::Equals(object, result);
}

ECode CHashTable::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return HashTable::Get(key, value);
}

ECode CHashTable::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return HashTable::GetHashCode(hashCode);
}

ECode CHashTable::IsEmpty(
    /* [out] */ Boolean* result)
{
    return HashTable::IsEmpty(result);
}

ECode CHashTable::KeySet(
    /* [out] */ ISet** keySet)
{
    return HashTable::KeySet(keySet);
}

ECode CHashTable::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return HashTable::Put(key, value, oldValue);
}

ECode CHashTable::PutAll(
    /* [in] */ IMap* map)
{
    return HashTable::PutAll(map);
}

ECode CHashTable::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return HashTable::Remove(key, value);
}

ECode CHashTable::GetSize(
    /* [out] */ Int32* size)
{
    return HashTable::GetSize(size);
}

ECode CHashTable::Values(
    /* [out] */ ICollection** value)
{
    return HashTable::Values(value);
}

ECode CHashTable::Clone(
    /* [out] */ IInterface** object)
{
    return HashTable::Clone(object);
}

ECode CHashTable::Elements(
    /* [out] */ IEnumeration** enm)
{
    return HashTable::Elements(enm);
}

ECode CHashTable::Keys(
    /* [out] */ IEnumeration** enm)
{
    return HashTable::Keys(enm);
}

Mutex* CHashTable::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Utility
} // namespace Elastos
