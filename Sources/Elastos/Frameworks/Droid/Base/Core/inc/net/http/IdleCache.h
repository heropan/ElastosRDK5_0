
#ifndef __IDLECACHE_H__
#define __IDLECACHE_H__

#include "ext/frameworkext.h"
#include "Connection.h"

using namespace Org::Apache::Http;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * {@hide}
 */
class IdleCache
{
public:
    class Entry
    {
    public:
        AutoPtr<IHttpHost> mHost;
        Connection* mConnection;
        Int64 mTimeout;
    };

    class IdleReaper
        : public ThreadBase
    {
    public:
        IdleReaper(
            /* [in] */ IdleCache* parent);

        ~IdleReaper();

        virtual CARAPI Run();

        CARAPI ToString(
            /* [out] */ String* str);

    private:
        IdleCache* mParent;
    };

    friend class IdleCache::IdleReaper;

public:
    IdleCache();

    ~IdleCache();

    /**
     * Caches connection, if there is room.
     * @return true if connection cached
     */
    CARAPI CacheConnection(
        /* [in] */ IHttpHost* host,
        /* [in] */ Connection* connection,
        /* [out] */ Boolean* result);

    CARAPI GetConnection(
        /* [in] */ IHttpHost* host,
        /* [out] */ Connection** conn);

    CARAPI Clear();

    CARAPI ClearIdle();

private:
    static const Int32 IDLE_CACHE_MAX = 8;

    /* Allow five consecutive empty queue checks before shutdown */
    static const Int32 EMPTY_CHECK_MAX = 5;

    /* six second timeout for connections */
    static const Int32 TIMEOUT = 6 * 1000;

    static const Int32 CHECK_INTERVAL = 2 * 1000;

    AutoPtr<ArrayOf<Entry*> > mEntries;

    Int32 mCount;

    IdleReaper* mThread;

    /* stats */
    Int32 mCached;

    Int32 mReused;

    Object mLock;

};

}
}
}
}

#endif // __IDLECACHE_H__
