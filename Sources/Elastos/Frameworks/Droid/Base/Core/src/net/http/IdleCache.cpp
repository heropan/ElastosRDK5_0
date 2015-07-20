
#include "IdleCache.h"
#include "HttpLog.h"
// #include "SystemClock.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

IdleCache::IdleCache()
    : mCount(0)
    , mThread(NULL)
    , mCached(0)
    , mReused(0)
{
    mEntries = ArrayOf<Entry*>::Alloc(IDLE_CACHE_MAX);
    for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
        (*mEntries)[i] = new Entry();
    }
}

IdleCache::~IdleCache()
{}

ECode IdleCache::CacheConnection(
        /* [in] */ IHttpHost* host,
        /* [in] */ Connection* connection,
        /* [out] */ Boolean* result)
{
    AutoLock lock(mLock);

    Boolean ret = FALSE;

    if (HttpLog::LOGV) {
        HttpLog::V(String("IdleCache size ") + StringUtils::Int32ToString(mCount) + String(" host ")/* + host*/);
    }

    if (mCount < IDLE_CACHE_MAX) {
        Int64 time;
        // time = SystemClock::uptimeMillis();
        for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
            Entry* entry = (*mEntries)[i];
            if (entry->mHost == NULL) {
                entry->mHost = host;
                // entry->mConnection = connection;
                entry->mTimeout = time + TIMEOUT;
                mCount++;
                if (HttpLog::LOGV) mCached++;
                ret = true;
                if (mThread == NULL) {
                    mThread = new IdleReaper(this);
                    mThread->Start();
                }
                break;
            }
        }
    }
    return ret;
}

ECode IdleCache::GetConnection(
    /* [in] */ IHttpHost* host,
    /* [out] */ Connection** conn)
{
    AutoLock lock(mLock);

    Connection* ret = NULL;

    if (mCount > 0) {
        for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
            Entry* entry = (*mEntries)[i];
            AutoPtr<IHttpHost> eHost = entry->mHost;
            // TODO:
            if (eHost != NULL /*&& eHost->Equals(host)*/) {
                ret = entry->mConnection;
                entry->mHost = NULL;
                entry->mConnection = NULL;
                mCount--;
                if (HttpLog::LOGV) mReused++;
                break;
            }
        }
    }

    *conn = ret;
    return NOERROR;
}

ECode IdleCache::Clear()
{
    AutoLock lock(mLock);

    for (Int32 i = 0; mCount > 0 && i < IDLE_CACHE_MAX; i++) {
        Entry* entry = (*mEntries)[i];
        if (entry->mHost != NULL) {
            entry->mHost = NULL;
            // entry->mConnection->CloseConnection();
            // entry->mConnection = NULL;
            mCount--;
        }
    }

    return NOERROR;
}

ECode IdleCache::ClearIdle()
{
    AutoLock lock(mLock);

    if (mCount > 0) {
        Int64 time;
        // time = SystemClock::uptimeMillis();
        for (Int32 i = 0; i < IDLE_CACHE_MAX; i++) {
            Entry* entry = (*mEntries)[i];
            if (entry->mHost != NULL && time > entry->mTimeout) {
                entry->mHost = NULL;
                // entry->mConnection->CloseConnection();
                // entry->mConnection = NULL;
                mCount--;
            }
        }
    }

    return NOERROR;
}

ECode IdleCache::IdleReaper::Run()
{
    Int32 check = 0;

    SetName(String("IdleReaper"));
    // AutoPtr<Elastos::Os::IProcessHelper> helper;
    // Elastos::os::CProcessHelper::AcquireSingleton((Elastos::Os::IProcessHelper**)&helper);
    // helper->SetThreadPriority(
    //         Elastos::os::Process::THREAD_PRIORITY_BACKGROUND);

    {
        AutoLock lock(mParent->mLock);

        while (check < EMPTY_CHECK_MAX) {
            Wait(CHECK_INTERVAL);

            if (mParent->mCount == 0) {
                check++;
            } else {
                check = 0;
                mParent->ClearIdle();
            }
        }
        mParent->mThread = NULL;
    }

    if (HttpLog::LOGV) {
        HttpLog::V(String("IdleCache IdleReaper shutdown: cached ") + StringUtils::Int32ToString(mParent->mCached) +
                  String(" reused ") + StringUtils::Int32ToString(mParent->mReused));
        mParent->mCached = 0;
        mParent->mReused = 0;
    }

    return NOERROR;
}

IdleCache::IdleReaper::IdleReaper(
    /* [in] */ IdleCache* parent)
    : mParent(parent)
{
    Thread::Init("IdleReaper");
}

IdleCache::IdleReaper::~IdleReaper()
{}

}
}
}
}
