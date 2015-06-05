
#include "CHashMap.h"

namespace Elastos {
namespace Utility {

ECode CHashMap::constructor()
{
    return HashMap::Init();
}

ECode CHashMap::constructor(
    /* [in] */ Int32 capacity)
{
    return HashMap::Init(capacity);
}

ECode CHashMap::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    return HashMap::Init(capacity, loadFactor);
}

ECode CHashMap::constructor(
    /* [in] */ IMap* map)
{
    return HashMap::Init(map);
}

ECode CHashMap::Clear()
{
    return HashMap::Clear();
}

ECode CHashMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return HashMap::ContainsKey(key, result);
}

ECode CHashMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return HashMap::ContainsValue(value, result);
}

ECode CHashMap::EntrySet(
    /* [out] */ ISet** entries)
{
    return HashMap::EntrySet(entries);
}

ECode CHashMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return HashMap::Equals(object, result);
}

ECode CHashMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return HashMap::Get(key, value);
}

ECode CHashMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return HashMap::GetHashCode(hashCode);
}

ECode CHashMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return HashMap::IsEmpty(result);
}

ECode CHashMap::KeySet(
    /* [out] */ ISet** keySet)
{
    return HashMap::KeySet(keySet);
}

ECode CHashMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return HashMap::Put(key, value, oldValue);
}

ECode CHashMap::PutAll(
    /* [in] */ IMap* map)
{
    return HashMap::PutAll(map);
}

ECode CHashMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return HashMap::Remove(key, value);
}

ECode CHashMap::GetSize(
    /* [out] */ Int32* size)
{
    return HashMap::GetSize(size);
}

ECode CHashMap::Values(
    /* [out] */ ICollection** value)
{
    return HashMap::Values(value);
}

ECode CHashMap::Clone(
    /* [out] */ IInterface** object)
{
    return HashMap::Clone(object);
}

ECode CHashMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode CHashMap::Remove(
    /* [in] */ PInterface key)
{
    assert(0 && "TODO");
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
