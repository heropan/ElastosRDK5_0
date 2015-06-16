#include "CBasicLruCache.h"

namespace Libcore {
namespace Utility {

CAR_OBJECT_IMPL(CBasicLruCache)

// /**
//  * Returns the value for {@code key} if it exists in the cache or can be
//  * created by {@code #create}. If a value was returned, it is moved to the
//  * head of the queue. This returns null if a value is not cached and cannot
//  * be created.
//  */
// ECode CBasicLruCache::Get(
//     /* [in] */ IInterface* key,
//     /* [out] */ IInterface** ouface)
// {
//     return BasicLruCache::Get
// }

// *
//  * Caches {@code value} for {@code key}. The value is moved to the head of
//  * the queue.
//  *
//  * @return the previous value mapped by {@code key}. Although that entry is
//  *     no longer cached, it has not been passed to {@link #entryEvicted}.

// ECode CBasicLruCache::Put(
//     /* [in] */ IInterface* key,
//     /* [in] */ IInterface* value,
//     /* [out] */ IInterface** outface)
// {
//     return BasicLruCache::Get
// }

// /**
//  * Returns a copy of the current contents of the cache, ordered from least
//  * recently accessed to most recently accessed.
//  */
// ECode CBasicLruCache::Snapshot(
//     /* [out] */ IMap** outmap)
// {
//     return BasicLruCache::Get
// }

// /**
//  * Clear the cache, calling {@link #entryEvicted} on each removed entry.
//  */
// ECode CBasicLruCache::EvictAll()
// {
//     return BasicLruCache::Get
// }

}
}