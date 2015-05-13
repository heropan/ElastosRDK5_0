
#include "CHashMap.h"

namespace Elastos {
namespace Utility {

PInterface CHashMap::Probe(
    /* [in] */ REIID riid)
{
    return _CHashMap::Probe(riid);
}

UInt32 CHashMap::AddRef()
{
    return _CHashMap::AddRef();
}

UInt32 CHashMap::Release()
{
    return _CHashMap::Release();
}

ECode CHashMap::constructor()
{
    return _HashMap::Init();
}

ECode CHashMap::constructor(
    /* [in] */ Int32 capacity)
{
    return _HashMap::Init(capacity);
}

ECode CHashMap::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    return _HashMap::Init(capacity, loadFactor);
}

ECode CHashMap::constructor(
    /* [in] */ IMap* map)
{
    return _HashMap::Init(map);
}

ECode CHashMap::Clear()
{
    return _HashMap::Clear();
}

ECode CHashMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return _HashMap::ContainsKey(key, result);
}

ECode CHashMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return _HashMap::ContainsValue(value, result);
}

ECode CHashMap::EntrySet(
    /* [out] */ ISet** entries)
{
    return _HashMap::EntrySet(entries);
}

ECode CHashMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return _HashMap::Equals(object, result);
}

ECode CHashMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return _HashMap::Get(key, value);
}

ECode CHashMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return _HashMap::GetHashCode(hashCode);
}

ECode CHashMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return _HashMap::IsEmpty(result);
}

ECode CHashMap::KeySet(
    /* [out] */ ISet** keySet)
{
    return _HashMap::KeySet(keySet);
}

ECode CHashMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return _HashMap::Put(key, value, oldValue);
}

ECode CHashMap::PutAll(
    /* [in] */ IMap* map)
{
    return _HashMap::PutAll(map);
}

ECode CHashMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return _HashMap::Remove(key, value);
}

ECode CHashMap::GetSize(
    /* [out] */ Int32* size)
{
    return _HashMap::GetSize(size);
}

ECode CHashMap::Values(
    /* [out] */ ICollection** value)
{
    return _HashMap::Values(value);
}

ECode CHashMap::Clone(
    /* [out] */ IInterface** object)
{
    return _HashMap::Clone(object);
}

} // namespace Utility
} // namespace Elastos
