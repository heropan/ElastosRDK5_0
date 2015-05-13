
#include "AddressCache.h"
#include <elastos/CSystem.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;

namespace Elastos {
namespace Net {

/**
 * When the cache contains more entries than this, we start dropping the oldest ones.
 * This should be a power of two to avoid wasted space in our custom map.
 */
const Int32 AddressCache::MAX_ENTRIES;

// The TTL for the Java-level cache is short, just 2s.
const Int64 AddressCache::TTL_NANOS;

const AutoPtr< ArrayOf<IInetAddress*> > AddressCache::UNKNOWN_ADDRESS = ArrayOf<IInetAddress*>::Alloc(0);

AddressCache::AddressCacheEntry::AddressCacheEntry(
    /* [in] */ ArrayOf<IInetAddress*>* value)
    : mValue(value)
    , mExpiryNanos(0)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetNanoTime(&mExpiryNanos);
    mExpiryNanos += TTL_NANOS;
}

AddressCache::~AddressCache()
{
    mCache.Clear();
}

/**
 * Removes all entries from the cache.
 */
void AddressCache::Clear()
{
    mCache.Clear();
}

/**
 * Returns the cached addresses associated with 'hostname'. Returns null if nothing is known
 * about 'hostname'. Returns an empty array if 'hostname' is known not to exist.
 */
AutoPtr< ArrayOf<IInetAddress*> > AddressCache::Get(
    /* [in] */ const String& hostname)
{
    AutoPtr<AddressCacheEntry> entry;
    HashMap<String, AutoPtr<AddressCacheEntry> >::Iterator it = mCache.Find(hostname);
    if (it != mCache.End()) entry = it->mSecond;

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 nanos;
    system->GetNanoTime(&nanos);
    // Do we have a valid cache entry?
    if (entry != NULL && entry->mExpiryNanos >= nanos) {
        return entry->mValue;
    }
    // Either we didn't find anything, or it had expired.
    // No need to remove expired entries: the caller will provide a replacement shortly.
    return NULL;
}

/**
 * Associates the given 'addresses' with 'hostname'. The association will expire after a
 * certain length of time.
 */
void AddressCache::Put(
    /* [in] */ const String& hostname,
    /* [in] */ ArrayOf<IInetAddress*>* addresses)
{
    // Calculate the expiry time.
    mCache[hostname] = new AddressCacheEntry(addresses);
}

/**
 * Records that 'hostname' is known not to have any associated addresses. (I.e. insert a
 * negative cache entry.)
 */
void AddressCache::PutUnknownHost(
    /* [in] */ const String& hostname,
    /* [in] */ const String& detailMessage)
{
    mCache[hostname] = new AddressCacheEntry(UNKNOWN_ADDRESS);
}

} // namespace Net
} // namespace Elastos
