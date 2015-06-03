
#ifndef __ADDRESSCACHE_H__
#define __ADDRESSCACHE_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Net {

class AddressCache : public ElRefBase
{
private:
    class AddressCacheEntry : public ElRefBase
    {
    public:
        AddressCacheEntry(
            /* [in] */ ArrayOf<IInetAddress*>* value);
    public:
        AutoPtr< ArrayOf<IInetAddress*> > mValue;

        Int64 mExpiryNanos;
    };

public:
    AddressCache()
        : mCache(MAX_ENTRIES)
    {}

    ~AddressCache();

    CARAPI_(void) Clear();

    CARAPI_(AutoPtr< ArrayOf<IInetAddress*> >) Get(
        /* [in] */ const String& hostname);

    CARAPI_(void) Put(
        /* [in] */ const String& hostname,
        /* [in] */ ArrayOf<IInetAddress*>* addresses);

    CARAPI_(void) PutUnknownHost(
        /* [in] */ const String& hostname,
        /* [in] */ const String& detailMessage);

public:
    static const AutoPtr< ArrayOf<IInetAddress*> > UNKNOWN_ADDRESS;

private:
    static const Int32 MAX_ENTRIES = 16;

	//The TTL for the Java-level cache is short ,just 2s.
    static const Int64 TTL_NANOS = 2 * 1000000000ll;

    // The actual cache.
    HashMap<String, AutoPtr<AddressCacheEntry> > mCache;
};

} // namespace Net
} // namespace Elastos

#endif //__ADDRESSCACHE_H__
