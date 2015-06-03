
#include "CBasicLruCache.h"

namespace Libcore {
namespace Utility {

ECode CBasicLruCache::constructor()
{
    return BasicLruCache::Init();
}

ECode CBasicLruCache::constructor(
    /* [in] */ Int32 maxSize)
{
    return BasicLruCache::Init(maxSize);
}

ECode CBasicLruCache::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** ouface)
{
    return BasicLruCache::Get(key, ouface);
}

ECode CBasicLruCache::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** outface)
{
    return BasicLruCache::Put(key, value, outface);
}

ECode CBasicLruCache::Snapshot(
    /* [out] */ IMap** outmap)
{
    return BasicLruCache::Snapshot(outmap);
}

ECode CBasicLruCache::EvictAll()
{
    return BasicLruCache::EvictAll();
}

} // namespace Utility
} // namespace Libcore
